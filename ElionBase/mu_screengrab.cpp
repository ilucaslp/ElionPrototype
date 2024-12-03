#include "stdafx.h"
#include "mu_screengrab.h"

#if DIRECTX_BACKEND_ENABLE == 1 && (defined(WIN32) || defined(WIN64))
#include "mu_dds.h"
#include "mu_texturehelpers.h"

#include <wincodec.h>
#pragma comment(lib, "Windowscodecs.lib")

#ifndef IID_GRAPHICS_PPV_ARGS
#define IID_GRAPHICS_PPV_ARGS(x) IID_PPV_ARGS(x)
#endif

class auto_delete_file_wic
{
public:
	auto_delete_file_wic(IWICStream *hFile, LPCWSTR szFile) : m_handle(hFile), m_filename(szFile) {}

	auto_delete_file_wic(const auto_delete_file_wic&) = delete;
	auto_delete_file_wic& operator=(const auto_delete_file_wic&) = delete;

	~auto_delete_file_wic()
	{
		if (m_filename)
		{
			m_handle = nullptr;
			DeleteFileW(m_filename);
		}
	}

	void clear() { m_filename = 0; }

private:
	LPCWSTR m_filename;
	IWICStream *m_handle;
};

mu_boolean g_WIC2 = false;

bool _IsWIC2()
{
	return g_WIC2;
}

IWICImagingFactory* _GetWIC()
{
	static INIT_ONCE s_initOnce = INIT_ONCE_STATIC_INIT;

	IWICImagingFactory* factory = nullptr;
	InitOnceExecuteOnce(&s_initOnce,
		[](PINIT_ONCE, PVOID, PVOID *factory) -> BOOL
	{
#if (_WIN32_WINNT >= _WIN32_WINNT_WIN8) || defined(_WIN7_PLATFORM_UPDATE)
		HRESULT hr = CoCreateInstance(
			CLSID_WICImagingFactory2,
			nullptr,
			CLSCTX_INPROC_SERVER,
			__uuidof(IWICImagingFactory2),
			factory
		);

		if (SUCCEEDED(hr))
		{
			// WIC2 is available on Windows 10, Windows 8.x, and Windows 7 SP1 with KB 2670838 installed
			g_WIC2 = true;
			return TRUE;
		}
		else
		{
			hr = CoCreateInstance(
				CLSID_WICImagingFactory1,
				nullptr,
				CLSCTX_INPROC_SERVER,
				__uuidof(IWICImagingFactory),
				factory
			);
			return SUCCEEDED(hr) ? TRUE : FALSE;
		}
#else
		return SUCCEEDED(CoCreateInstance(
			CLSID_WICImagingFactory,
			nullptr,
			CLSCTX_INPROC_SERVER,
			__uuidof(IWICImagingFactory),
			factory)) ? TRUE : FALSE;
#endif
	}, nullptr, reinterpret_cast<LPVOID*>(&factory));

	return factory;
}

HRESULT CaptureTexture(_In_ ID3D11DeviceContext* pContext,
	_In_ ID3D11Resource* pSource,
	D3D11_TEXTURE2D_DESC& desc,
	ID3D11Texture2D *&pStaging)
{
	if (!pContext || !pSource)
		return E_INVALIDARG;

	D3D11_RESOURCE_DIMENSION resType = D3D11_RESOURCE_DIMENSION_UNKNOWN;
	pSource->GetType(&resType);

	if (resType != D3D11_RESOURCE_DIMENSION_TEXTURE2D)
		return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);

	ID3D11Texture2D *pTexture = nullptr;
	HRESULT hr = pSource->QueryInterface(IID_GRAPHICS_PPV_ARGS(&pTexture));
	if (FAILED(hr))
		return hr;

	pTexture->GetDesc(&desc);

	ID3D11Device *d3dDevice = nullptr;
	pContext->GetDevice(&d3dDevice);

	if (desc.SampleDesc.Count > 1)
	{
		// MSAA content must be resolved before being copied to a staging texture
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;

		ID3D11Texture2D *pTemp;
		hr = d3dDevice->CreateTexture2D(&desc, 0, &pTemp);
		if (FAILED(hr))
		{
			MU_SAFE_RELEASE(pTexture);
			return hr;
		}

		DXGI_FORMAT fmt = MU::EnsureNotTypeless(desc.Format);

		UINT support = 0;
		hr = d3dDevice->CheckFormatSupport(fmt, &support);
		if (FAILED(hr))
		{
			MU_SAFE_RELEASE(pTemp);
			MU_SAFE_RELEASE(pTexture);
			return hr;
		}

		if (!(support & D3D11_FORMAT_SUPPORT_MULTISAMPLE_RESOLVE))
		{
			MU_SAFE_RELEASE(pTemp);
			MU_SAFE_RELEASE(pTexture);
			return E_FAIL;
		}

		for (UINT item = 0; item < desc.ArraySize; ++item)
		{
			for (UINT level = 0; level < desc.MipLevels; ++level)
			{
				UINT index = D3D11CalcSubresource(level, item, desc.MipLevels);
				pContext->ResolveSubresource(pTemp, index, pSource, index, fmt);
			}
		}

		desc.BindFlags = 0;
		desc.MiscFlags &= D3D11_RESOURCE_MISC_TEXTURECUBE;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		desc.Usage = D3D11_USAGE_STAGING;

		MU_SAFE_RELEASE(pStaging);

		hr = d3dDevice->CreateTexture2D(&desc, 0, &pStaging);
		if (FAILED(hr))
		{
			MU_SAFE_RELEASE(pTemp);
			MU_SAFE_RELEASE(pTexture);
			return E_FAIL;
		}

		pContext->CopyResource(pStaging, pTemp);

		MU_SAFE_RELEASE(pTemp);
		MU_SAFE_RELEASE(pTexture);
	}
	else if ((desc.Usage == D3D11_USAGE_STAGING) && (desc.CPUAccessFlags & D3D11_CPU_ACCESS_READ))
	{
		// Handle case where the source is already a staging texture we can use directly
		pStaging = pTexture;
	}
	else
	{
		// Otherwise, create a staging texture from the non-MSAA source
		desc.BindFlags = 0;
		desc.MiscFlags &= D3D11_RESOURCE_MISC_TEXTURECUBE;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		desc.Usage = D3D11_USAGE_STAGING;

		MU_SAFE_RELEASE(pStaging);

		hr = d3dDevice->CreateTexture2D(&desc, 0, &pStaging);
		if (FAILED(hr))
		{
			MU_SAFE_RELEASE(pTexture);
			return E_FAIL;
		}

		assert(pStaging);

		pContext->CopyResource(pStaging, pSource);

		MU_SAFE_RELEASE(pTexture);
	}

	return S_OK;
}

HRESULT MU::SaveWICTextureToFile(ID3D11DeviceContext *pContext,
	ID3D11Resource *pSource,
	REFGUID guidContainerFormat,
	const mu_text *filename,
	const GUID* targetFormat)
{
	if (!filename)
		return E_INVALIDARG;

	D3D11_TEXTURE2D_DESC desc = {};
	ID3D11Texture2D *pStaging = nullptr;
	HRESULT hr = CaptureTexture(pContext, pSource, desc, pStaging);
	if (FAILED(hr))
		return hr;

	// Determine source format's WIC equivalent
	WICPixelFormatGUID pfGuid;
	bool sRGB = false;
	switch (desc.Format)
	{
	case DXGI_FORMAT_R32G32B32A32_FLOAT:            pfGuid = GUID_WICPixelFormat128bppRGBAFloat; break;
	case DXGI_FORMAT_R16G16B16A16_FLOAT:            pfGuid = GUID_WICPixelFormat64bppRGBAHalf; break;
	case DXGI_FORMAT_R16G16B16A16_UNORM:            pfGuid = GUID_WICPixelFormat64bppRGBA; break;
	case DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM:    pfGuid = GUID_WICPixelFormat32bppRGBA1010102XR; break; // DXGI 1.1
	case DXGI_FORMAT_R10G10B10A2_UNORM:             pfGuid = GUID_WICPixelFormat32bppRGBA1010102; break;
	case DXGI_FORMAT_B5G5R5A1_UNORM:                pfGuid = GUID_WICPixelFormat16bppBGRA5551; break;
	case DXGI_FORMAT_B5G6R5_UNORM:                  pfGuid = GUID_WICPixelFormat16bppBGR565; break;
	case DXGI_FORMAT_R32_FLOAT:                     pfGuid = GUID_WICPixelFormat32bppGrayFloat; break;
	case DXGI_FORMAT_R16_FLOAT:                     pfGuid = GUID_WICPixelFormat16bppGrayHalf; break;
	case DXGI_FORMAT_R16_UNORM:                     pfGuid = GUID_WICPixelFormat16bppGray; break;
	case DXGI_FORMAT_R8_UNORM:                      pfGuid = GUID_WICPixelFormat8bppGray; break;
	case DXGI_FORMAT_A8_UNORM:                      pfGuid = GUID_WICPixelFormat8bppAlpha; break;

	case DXGI_FORMAT_R8G8B8A8_UNORM:
		pfGuid = GUID_WICPixelFormat32bppRGBA;
		break;

	case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
		pfGuid = GUID_WICPixelFormat32bppRGBA;
		sRGB = true;
		break;

	case DXGI_FORMAT_B8G8R8A8_UNORM: // DXGI 1.1
		pfGuid = GUID_WICPixelFormat32bppBGRA;
		break;

	case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB: // DXGI 1.1
		pfGuid = GUID_WICPixelFormat32bppBGRA;
		sRGB = true;
		break;

	case DXGI_FORMAT_B8G8R8X8_UNORM: // DXGI 1.1
		pfGuid = GUID_WICPixelFormat32bppBGR;
		break;

	case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB: // DXGI 1.1
		pfGuid = GUID_WICPixelFormat32bppBGR;
		sRGB = true;
		break;

	default:
		return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);
	}

	auto pWIC = _GetWIC();
	if (!pWIC)
	{
		MU_SAFE_RELEASE(pStaging);
		return E_NOINTERFACE;
	}

	IWICStream *stream = nullptr;
	hr = pWIC->CreateStream(&stream);
	if (FAILED(hr))
	{
		MU_SAFE_RELEASE(pStaging);
		return hr;
	}

	hr = stream->InitializeFromFilename(filename, GENERIC_WRITE);
	if (FAILED(hr))
	{
		MU_SAFE_RELEASE(stream);
		MU_SAFE_RELEASE(pStaging);
		return hr;
	}

	auto_delete_file_wic delonfail(stream, filename);

	IWICBitmapEncoder *encoder = nullptr;
	hr = pWIC->CreateEncoder(guidContainerFormat, 0, &encoder);
	if (FAILED(hr))
	{
		MU_SAFE_RELEASE(stream);
		MU_SAFE_RELEASE(pStaging);
		return hr;
	}

	hr = encoder->Initialize(stream, WICBitmapEncoderNoCache);
	if (FAILED(hr))
	{
		MU_SAFE_RELEASE(encoder);
		MU_SAFE_RELEASE(stream);
		MU_SAFE_RELEASE(pStaging);
		return hr;
	}

	IWICBitmapFrameEncode *frame = nullptr;
	IPropertyBag2 *props = nullptr;
	hr = encoder->CreateNewFrame(&frame, &props);
	if (FAILED(hr))
	{
		MU_SAFE_RELEASE(encoder);
		MU_SAFE_RELEASE(stream);
		MU_SAFE_RELEASE(pStaging);
		return hr;
	}

	if (targetFormat && memcmp(&guidContainerFormat, &GUID_ContainerFormatBmp, sizeof(WICPixelFormatGUID)) == 0 && _IsWIC2())
	{
		// Opt-in to the WIC2 support for writing 32-bit Windows BMP files with an alpha channel
		PROPBAG2 option = {};
		option.pstrName = L"EnableV5Header32bppBGRA";

		VARIANT varValue;
		varValue.vt = VT_BOOL;
		varValue.boolVal = VARIANT_TRUE;
		(void)props->Write(1, &option, &varValue);
	}

	hr = frame->Initialize(props);
	if (FAILED(hr))
	{
		MU_SAFE_RELEASE(props);
		MU_SAFE_RELEASE(frame);
		MU_SAFE_RELEASE(encoder);
		MU_SAFE_RELEASE(stream);
		MU_SAFE_RELEASE(pStaging);
		return hr;
	}

	hr = frame->SetSize(desc.Width, desc.Height);
	if (FAILED(hr))
	{
		MU_SAFE_RELEASE(props);
		MU_SAFE_RELEASE(frame);
		MU_SAFE_RELEASE(encoder);
		MU_SAFE_RELEASE(stream);
		MU_SAFE_RELEASE(pStaging);
		return hr;
	}

	hr = frame->SetResolution(72, 72);
	if (FAILED(hr))
	{
		MU_SAFE_RELEASE(props);
		MU_SAFE_RELEASE(frame);
		MU_SAFE_RELEASE(encoder);
		MU_SAFE_RELEASE(stream);
		MU_SAFE_RELEASE(pStaging);
		return hr;
	}

	// Pick a target format
	WICPixelFormatGUID targetGuid;
	if (targetFormat)
	{
		targetGuid = *targetFormat;
	}
	else
	{
		// Screenshots don’t typically include the alpha channel of the render target
		switch (desc.Format)
		{
#if (_WIN32_WINNT >= _WIN32_WINNT_WIN8) || defined(_WIN7_PLATFORM_UPDATE)
		case DXGI_FORMAT_R32G32B32A32_FLOAT:
		case DXGI_FORMAT_R16G16B16A16_FLOAT:
			if (_IsWIC2())
			{
				targetGuid = GUID_WICPixelFormat96bppRGBFloat;
			}
			else
			{
				targetGuid = GUID_WICPixelFormat24bppBGR;
			}
			break;
#endif

		case DXGI_FORMAT_R16G16B16A16_UNORM: targetGuid = GUID_WICPixelFormat48bppBGR; break;
		case DXGI_FORMAT_B5G5R5A1_UNORM:     targetGuid = GUID_WICPixelFormat16bppBGR555; break;
		case DXGI_FORMAT_B5G6R5_UNORM:       targetGuid = GUID_WICPixelFormat16bppBGR565; break;

		case DXGI_FORMAT_R32_FLOAT:
		case DXGI_FORMAT_R16_FLOAT:
		case DXGI_FORMAT_R16_UNORM:
		case DXGI_FORMAT_R8_UNORM:
		case DXGI_FORMAT_A8_UNORM:
			targetGuid = GUID_WICPixelFormat8bppGray;
			break;

		default:
			targetGuid = GUID_WICPixelFormat24bppBGR;
			break;
		}
	}

	hr = frame->SetPixelFormat(&targetGuid);
	if (FAILED(hr))
	{
		MU_SAFE_RELEASE(props);
		MU_SAFE_RELEASE(frame);
		MU_SAFE_RELEASE(encoder);
		MU_SAFE_RELEASE(stream);
		MU_SAFE_RELEASE(pStaging);
		return hr;
	}

	if (targetFormat && memcmp(targetFormat, &targetGuid, sizeof(WICPixelFormatGUID)) != 0)
	{
		MU_SAFE_RELEASE(props);
		MU_SAFE_RELEASE(frame);
		MU_SAFE_RELEASE(encoder);
		MU_SAFE_RELEASE(stream);
		MU_SAFE_RELEASE(pStaging);
		// Requested output pixel format is not supported by the WIC codec
		return E_FAIL;
	}

	// Encode WIC metadata
	IWICMetadataQueryWriter *metawriter = nullptr;
	if (SUCCEEDED(frame->GetMetadataQueryWriter(&metawriter)))
	{
		PROPVARIANT value;
		PropVariantInit(&value);

		value.vt = VT_LPSTR;
		value.pszVal = "DirectXTK";

		if (memcmp(&guidContainerFormat, &GUID_ContainerFormatPng, sizeof(GUID)) == 0)
		{
			// Set Software name
			(void)metawriter->SetMetadataByName(L"/tEXt/{str=Software}", &value);

			// Set sRGB chunk
			if (sRGB)
			{
				value.vt = VT_UI1;
				value.bVal = 0;
				(void)metawriter->SetMetadataByName(L"/sRGB/RenderingIntent", &value);
			}
		}
		else
		{
			// Set Software name
			(void)metawriter->SetMetadataByName(L"System.ApplicationName", &value);

			if (sRGB)
			{
				// Set EXIF Colorspace of sRGB
				value.vt = VT_UI2;
				value.uiVal = 1;
				(void)metawriter->SetMetadataByName(L"System.Image.ColorSpace", &value);
			}
		}
	}

	D3D11_MAPPED_SUBRESOURCE mapped;
	hr = pContext->Map(pStaging, 0, D3D11_MAP_READ, 0, &mapped);
	if (FAILED(hr))
	{
		MU_SAFE_RELEASE(metawriter);
		MU_SAFE_RELEASE(props);
		MU_SAFE_RELEASE(frame);
		MU_SAFE_RELEASE(encoder);
		MU_SAFE_RELEASE(stream);
		MU_SAFE_RELEASE(pStaging);
		return hr;
	}

	if (memcmp(&targetGuid, &pfGuid, sizeof(WICPixelFormatGUID)) != 0)
	{
		// Conversion required to write
		IWICBitmap *source = nullptr;
		hr = pWIC->CreateBitmapFromMemory(desc.Width, desc.Height, pfGuid,
			mapped.RowPitch, mapped.RowPitch * desc.Height,
			reinterpret_cast<BYTE*>(mapped.pData), &source);
		if (FAILED(hr))
		{
			pContext->Unmap(pStaging, 0);
			MU_SAFE_RELEASE(metawriter);
			MU_SAFE_RELEASE(props);
			MU_SAFE_RELEASE(frame);
			MU_SAFE_RELEASE(encoder);
			MU_SAFE_RELEASE(stream);
			MU_SAFE_RELEASE(pStaging);
			return hr;
		}

		IWICFormatConverter *FC = nullptr;
		hr = pWIC->CreateFormatConverter(&FC);
		if (FAILED(hr))
		{
			pContext->Unmap(pStaging, 0);
			MU_SAFE_RELEASE(source);
			MU_SAFE_RELEASE(metawriter);
			MU_SAFE_RELEASE(props);
			MU_SAFE_RELEASE(frame);
			MU_SAFE_RELEASE(encoder);
			MU_SAFE_RELEASE(stream);
			MU_SAFE_RELEASE(pStaging);
			return hr;
		}

		BOOL canConvert = FALSE;
		hr = FC->CanConvert(pfGuid, targetGuid, &canConvert);
		if (FAILED(hr) || !canConvert)
		{
			pContext->Unmap(pStaging, 0);
			MU_SAFE_RELEASE(FC);
			MU_SAFE_RELEASE(source);
			MU_SAFE_RELEASE(metawriter);
			MU_SAFE_RELEASE(props);
			MU_SAFE_RELEASE(frame);
			MU_SAFE_RELEASE(encoder);
			MU_SAFE_RELEASE(stream);
			MU_SAFE_RELEASE(pStaging);
			return E_UNEXPECTED;
		}

		hr = FC->Initialize(source, targetGuid, WICBitmapDitherTypeNone, 0, 0, WICBitmapPaletteTypeCustom);
		if (FAILED(hr))
		{
			pContext->Unmap(pStaging, 0);
			MU_SAFE_RELEASE(FC);
			MU_SAFE_RELEASE(source);
			MU_SAFE_RELEASE(metawriter);
			MU_SAFE_RELEASE(props);
			MU_SAFE_RELEASE(frame);
			MU_SAFE_RELEASE(encoder);
			MU_SAFE_RELEASE(stream);
			MU_SAFE_RELEASE(pStaging);
			return hr;
		}

		WICRect rect = { 0, 0, static_cast<INT>(desc.Width), static_cast<INT>(desc.Height) };
		hr = frame->WriteSource(FC, &rect);

		MU_SAFE_RELEASE(FC);
		MU_SAFE_RELEASE(source);

		if (FAILED(hr))
		{
			pContext->Unmap(pStaging, 0);
			MU_SAFE_RELEASE(metawriter);
			MU_SAFE_RELEASE(props);
			MU_SAFE_RELEASE(frame);
			MU_SAFE_RELEASE(encoder);
			MU_SAFE_RELEASE(stream);
			MU_SAFE_RELEASE(pStaging);
			return hr;
		}
	}
	else
	{
		// No conversion required
		hr = frame->WritePixels(desc.Height, mapped.RowPitch, mapped.RowPitch * desc.Height, reinterpret_cast<BYTE*>(mapped.pData));
		if (FAILED(hr))
		{
			pContext->Unmap(pStaging, 0);
			MU_SAFE_RELEASE(metawriter);
			MU_SAFE_RELEASE(props);
			MU_SAFE_RELEASE(frame);
			MU_SAFE_RELEASE(encoder);
			MU_SAFE_RELEASE(stream);
			MU_SAFE_RELEASE(pStaging);
			return hr;
		}
	}

	pContext->Unmap(pStaging, 0);

	hr = frame->Commit();
	if (FAILED(hr))
	{
		MU_SAFE_RELEASE(metawriter);
		MU_SAFE_RELEASE(props);
		MU_SAFE_RELEASE(frame);
		MU_SAFE_RELEASE(encoder);
		MU_SAFE_RELEASE(stream);
		MU_SAFE_RELEASE(pStaging);
		return hr;
	}

	hr = encoder->Commit();
	if (FAILED(hr))
	{
		MU_SAFE_RELEASE(metawriter);
		MU_SAFE_RELEASE(props);
		MU_SAFE_RELEASE(frame);
		MU_SAFE_RELEASE(encoder);
		MU_SAFE_RELEASE(stream);
		MU_SAFE_RELEASE(pStaging);
		return hr;
	}

	delonfail.clear();

	MU_SAFE_RELEASE(metawriter);
	MU_SAFE_RELEASE(props);
	MU_SAFE_RELEASE(frame);
	MU_SAFE_RELEASE(encoder);
	MU_SAFE_RELEASE(stream);
	MU_SAFE_RELEASE(pStaging);

	return S_OK;
}
#endif