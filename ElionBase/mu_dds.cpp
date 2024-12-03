#include "stdafx.h"
#include "mu_dds.h"
#include "mu_texturehelpers.h"

#if DIRECTX_BACKEND_ENABLE == 1
HRESULT FillInitData(mu_size width,
	mu_size height,
	mu_size depth,
	mu_size mipCount,
	mu_size arraySize,
	DXGI_FORMAT format,
	mu_size maxsize,
	mu_size bitSize,
	const mu_uint8* bitData,
	mu_size& twidth,
	mu_size& theight,
	mu_size& tdepth,
	mu_size& skipMip,
	D3D11_SUBRESOURCE_DATA* initData)
{
	if (!bitData || !initData)
	{
		return E_POINTER;
	}

	skipMip = 0;
	twidth = 0;
	theight = 0;
	tdepth = 0;

	mu_size NumBytes = 0;
	mu_size RowBytes = 0;
	const mu_uint8* pSrcBits = bitData;
	const mu_uint8* pEndBits = bitData + bitSize;

	mu_size index = 0;
	for (mu_size j = 0; j < arraySize; ++j)
	{
		mu_size w = width;
		mu_size h = height;
		mu_size d = depth;
		for (mu_size i = 0; i < mipCount; ++i)
		{
			MU::GetSurfaceInfo(w,
				h,
				format,
				&NumBytes,
				&RowBytes,
				nullptr
			);

			if ((mipCount <= 1) || !maxsize || (w <= maxsize && h <= maxsize && d <= maxsize))
			{
				if (!twidth)
				{
					twidth = w;
					theight = h;
					tdepth = d;
				}

				mu_assert(index < mipCount * arraySize);
				_Analysis_assume_(index < mipCount * arraySize);
				initData[index].pSysMem = (const void*)pSrcBits;
				initData[index].SysMemPitch = static_cast<UINT>(RowBytes);
				initData[index].SysMemSlicePitch = static_cast<UINT>(NumBytes);
				++index;
			}
			else if (!j)
			{
				// Count number of skipped mipmaps (first item only)
				++skipMip;
			}

			if (pSrcBits + (NumBytes*d) > pEndBits)
			{
				return HRESULT_FROM_WIN32(ERROR_HANDLE_EOF);
			}

			pSrcBits += NumBytes * d;

			w = w >> 1;
			h = h >> 1;
			d = d >> 1;
			if (w == 0)
			{
				w = 1;
			}
			if (h == 0)
			{
				h = 1;
			}
			if (d == 0)
			{
				d = 1;
			}
		}
	}

	return (index > 0) ? S_OK : E_FAIL;
}

HRESULT CreateD3DResources(ID3D11Device* d3dDevice,
	mu_uint32 resDim,
	mu_size width,
	mu_size height,
	mu_size depth,
	mu_size mipCount,
	mu_size arraySize,
	DXGI_FORMAT format,
	D3D11_USAGE usage,
	mu_uint32 bindFlags,
	mu_uint32 cpuAccessFlags,
	mu_uint32 miscFlags,
	mu_boolean forceSRGB,
	mu_boolean isCubeMap,
	D3D11_SUBRESOURCE_DATA* initData,
	ID3D11Resource** texture,
	ID3D11ShaderResourceView** textureView)
{
	if (!d3dDevice)
		return E_POINTER;

	HRESULT hr = E_FAIL;

	if (forceSRGB)
	{
		format = MU::MakeSRGB(format);
	}

	switch (resDim)
	{
	case D3D11_RESOURCE_DIMENSION_TEXTURE1D:
		{
			D3D11_TEXTURE1D_DESC desc;
			desc.Width = static_cast<UINT>(width);
			desc.MipLevels = static_cast<UINT>(mipCount);
			desc.ArraySize = static_cast<UINT>(arraySize);
			desc.Format = format;
			desc.Usage = usage;
			desc.BindFlags = bindFlags;
			desc.CPUAccessFlags = cpuAccessFlags;
			desc.MiscFlags = miscFlags & ~D3D11_RESOURCE_MISC_TEXTURECUBE;

			ID3D11Texture1D* tex = nullptr;
			hr = d3dDevice->CreateTexture1D(&desc,
				initData,
				&tex
			);
			if (SUCCEEDED(hr) && tex != 0)
			{
				if (textureView != 0)
				{
					D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
					memset(&SRVDesc, 0, sizeof(SRVDesc));
					SRVDesc.Format = format;

					if (arraySize > 1)
					{
						SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1DARRAY;
						SRVDesc.Texture1DArray.MipLevels = (!mipCount) ? -1 : desc.MipLevels;
						SRVDesc.Texture1DArray.ArraySize = static_cast<UINT>(arraySize);
					}
					else
					{
						SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
						SRVDesc.Texture1D.MipLevels = (!mipCount) ? -1 : desc.MipLevels;
					}

					hr = d3dDevice->CreateShaderResourceView(tex,
						&SRVDesc,
						textureView
					);
					if (FAILED(hr))
					{
						tex->Release();
						return hr;
					}
				}

				if (texture != 0)
				{
					*texture = tex;
				}
				else
				{
					tex->Release();
				}
			}
		}
		break;

	case D3D11_RESOURCE_DIMENSION_TEXTURE2D:
		{
			D3D11_TEXTURE2D_DESC desc;
			desc.Width = static_cast<UINT>(width);
			desc.Height = static_cast<UINT>(height);
			desc.MipLevels = static_cast<UINT>(mipCount);
			desc.ArraySize = static_cast<UINT>(arraySize);
			desc.Format = format;
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
			desc.Usage = usage;
			desc.BindFlags = bindFlags;
			desc.CPUAccessFlags = cpuAccessFlags;
			if (isCubeMap)
			{
				desc.MiscFlags = miscFlags | D3D11_RESOURCE_MISC_TEXTURECUBE;
			}
			else
			{
				desc.MiscFlags = miscFlags & ~D3D11_RESOURCE_MISC_TEXTURECUBE;
			}

			ID3D11Texture2D* tex = nullptr;
			hr = d3dDevice->CreateTexture2D(&desc,
				initData,
				&tex
			);
			if (SUCCEEDED(hr) && tex != 0)
			{
				if (textureView != 0)
				{
					D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
					memset(&SRVDesc, 0, sizeof(SRVDesc));
					SRVDesc.Format = format;

					if (isCubeMap)
					{
						if (arraySize > 6)
						{
							SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBEARRAY;
							SRVDesc.TextureCubeArray.MipLevels = (!mipCount) ? -1 : desc.MipLevels;

							// Earlier we set arraySize to (NumCubes * 6)
							SRVDesc.TextureCubeArray.NumCubes = static_cast<UINT>(arraySize / 6);
						}
						else
						{
							SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
							SRVDesc.TextureCube.MipLevels = (!mipCount) ? -1 : desc.MipLevels;
						}
					}
					else if (arraySize > 1)
					{
						SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
						SRVDesc.Texture2DArray.MipLevels = (!mipCount) ? -1 : desc.MipLevels;
						SRVDesc.Texture2DArray.ArraySize = static_cast<UINT>(arraySize);
					}
					else
					{
						SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
						SRVDesc.Texture2D.MipLevels = (!mipCount) ? -1 : desc.MipLevels;
					}

					hr = d3dDevice->CreateShaderResourceView(tex,
						&SRVDesc,
						textureView
					);
					if (FAILED(hr))
					{
						tex->Release();
						return hr;
					}
				}

				if (texture != 0)
				{
					*texture = tex;
				}
				else
				{
					tex->Release();
				}
			}
		}
		break;

	case D3D11_RESOURCE_DIMENSION_TEXTURE3D:
		{
			D3D11_TEXTURE3D_DESC desc;
			desc.Width = static_cast<UINT>(width);
			desc.Height = static_cast<UINT>(height);
			desc.Depth = static_cast<UINT>(depth);
			desc.MipLevels = static_cast<UINT>(mipCount);
			desc.Format = format;
			desc.Usage = usage;
			desc.BindFlags = bindFlags;
			desc.CPUAccessFlags = cpuAccessFlags;
			desc.MiscFlags = miscFlags & ~D3D11_RESOURCE_MISC_TEXTURECUBE;

			ID3D11Texture3D* tex = nullptr;
			hr = d3dDevice->CreateTexture3D(&desc,
				initData,
				&tex
			);
			if (SUCCEEDED(hr) && tex != 0)
			{
				if (textureView != 0)
				{
					D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
					memset(&SRVDesc, 0, sizeof(SRVDesc));
					SRVDesc.Format = format;

					SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
					SRVDesc.Texture3D.MipLevels = (!mipCount) ? -1 : desc.MipLevels;

					hr = d3dDevice->CreateShaderResourceView(tex,
						&SRVDesc,
						textureView
					);
					if (FAILED(hr))
					{
						tex->Release();
						return hr;
					}
				}

				if (texture != 0)
				{
					*texture = tex;
				}
				else
				{
					tex->Release();
				}
			}
		}
		break;
	}

	return hr;
}

HRESULT CreateTextureFromDDS(ID3D11Device* d3dDevice,
	ID3D11DeviceContext* d3dContext,
	const DDS_HEADER* header,
	_In_reads_bytes_(bitSize) const mu_uint8* bitData,
	mu_size bitSize,
	mu_size maxsize,
	D3D11_USAGE usage,
	mu_uint32 bindFlags,
	mu_uint32 cpuAccessFlags,
	mu_uint32 miscFlags,
	mu_boolean forceSRGB,
	ID3D11Resource** texture,
	ID3D11ShaderResourceView** textureView)
{
	HRESULT hr = S_OK;

	UINT width = header->width;
	UINT height = header->height;
	UINT depth = header->depth;

	mu_uint32 resDim = D3D11_RESOURCE_DIMENSION_UNKNOWN;
	UINT arraySize = 1;
	DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN;
	mu_boolean isCubeMap = false;

	mu_size mipCount = header->mipMapCount;
	if (0 == mipCount)
	{
		mipCount = 1;
	}

	if ((header->ddspf.flags & DDS_FOURCC) &&
		(MAKEFOURCC('D', 'X', '1', '0') == header->ddspf.fourCC))
	{
		auto d3d10ext = reinterpret_cast<const DDS_HEADER_DXT10*>((const char*)header + sizeof(DDS_HEADER));

		arraySize = d3d10ext->arraySize;
		if (arraySize == 0)
		{
			return HRESULT_FROM_WIN32(ERROR_INVALID_DATA);
		}

		switch (d3d10ext->dxgiFormat)
		{
#if (_WIN32_WINNT >= _WIN32_WINNT_WIN8)
		case DXGI_FORMAT_AI44:
		case DXGI_FORMAT_IA44:
		case DXGI_FORMAT_P8:
		case DXGI_FORMAT_A8P8:
			return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);
#endif
		default:
			if (MU::BitsPerPixel(d3d10ext->dxgiFormat) == 0)
			{
				return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);
			}
		}

		format = d3d10ext->dxgiFormat;

		switch (d3d10ext->resourceDimension)
		{
		case D3D11_RESOURCE_DIMENSION_TEXTURE1D:
			// D3DX writes 1D textures with a fixed Height of 1
			if ((header->flags & DDS_HEIGHT) && height != 1)
			{
				return HRESULT_FROM_WIN32(ERROR_INVALID_DATA);
			}
			height = depth = 1;
			break;

		case D3D11_RESOURCE_DIMENSION_TEXTURE2D:
			if (d3d10ext->miscFlag & D3D11_RESOURCE_MISC_TEXTURECUBE)
			{
				arraySize *= 6;
				isCubeMap = true;
			}
			depth = 1;
			break;

		case D3D11_RESOURCE_DIMENSION_TEXTURE3D:
			if (!(header->flags & DDS_HEADER_FLAGS_VOLUME))
			{
				return HRESULT_FROM_WIN32(ERROR_INVALID_DATA);
			}

			if (arraySize > 1)
			{
				return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);
			}
			break;

		default:
			return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);
		}

		resDim = d3d10ext->resourceDimension;
	}
	else
	{
		format = MU::GetDXGIFormat(header->ddspf);

		if (format == DXGI_FORMAT_UNKNOWN)
		{
			return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);
		}

		if (header->flags & DDS_HEADER_FLAGS_VOLUME)
		{
			resDim = D3D11_RESOURCE_DIMENSION_TEXTURE3D;
		}
		else
		{
			if (header->caps2 & DDS_CUBEMAP)
			{
				// We require all six faces to be defined
				if ((header->caps2 & DDS_CUBEMAP_ALLFACES) != DDS_CUBEMAP_ALLFACES)
				{
					return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);
				}

				arraySize = 6;
				isCubeMap = true;
			}

			depth = 1;
			resDim = D3D11_RESOURCE_DIMENSION_TEXTURE2D;

			// Note there's no way for a legacy Direct3D 9 DDS to express a '1D' texture
		}

		mu_assert(MU::BitsPerPixel(format) != 0);
	}

	// Bound sizes (for security purposes we don't trust DDS file metadata larger than the Direct3D hardware requirements)
	if (mipCount > D3D11_REQ_MIP_LEVELS)
	{
		return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);
	}

	switch (resDim)
	{
	case D3D11_RESOURCE_DIMENSION_TEXTURE1D:
		if ((arraySize > D3D11_REQ_TEXTURE1D_ARRAY_AXIS_DIMENSION) ||
			(width > D3D11_REQ_TEXTURE1D_U_DIMENSION))
		{
			return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);
		}
		break;

	case D3D11_RESOURCE_DIMENSION_TEXTURE2D:
		if (isCubeMap)
		{
			// This is the right bound because we set arraySize to (NumCubes*6) above
			if ((arraySize > D3D11_REQ_TEXTURE2D_ARRAY_AXIS_DIMENSION) ||
				(width > D3D11_REQ_TEXTURECUBE_DIMENSION) ||
				(height > D3D11_REQ_TEXTURECUBE_DIMENSION))
			{
				return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);
			}
		}
		else if ((arraySize > D3D11_REQ_TEXTURE2D_ARRAY_AXIS_DIMENSION) ||
			(width > D3D11_REQ_TEXTURE2D_U_OR_V_DIMENSION) ||
			(height > D3D11_REQ_TEXTURE2D_U_OR_V_DIMENSION))
		{
			return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);
		}
		break;

	case D3D11_RESOURCE_DIMENSION_TEXTURE3D:
		if ((arraySize > 1) ||
			(width > D3D11_REQ_TEXTURE3D_U_V_OR_W_DIMENSION) ||
			(height > D3D11_REQ_TEXTURE3D_U_V_OR_W_DIMENSION) ||
			(depth > D3D11_REQ_TEXTURE3D_U_V_OR_W_DIMENSION))
		{
			return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);
		}
		break;

	default:
		return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);
	}

	mu_boolean autogen = false;
	if (mipCount == 1 && d3dContext != 0 && textureView != 0) // Must have context and shader-view to auto generate mipmaps
	{
		// See if format is supported for auto-gen mipmaps (varies by feature level)
		UINT fmtSupport = 0;
		hr = d3dDevice->CheckFormatSupport(format, &fmtSupport);
		if (SUCCEEDED(hr) && (fmtSupport & D3D11_FORMAT_SUPPORT_MIP_AUTOGEN))
		{
			// 10level9 feature levels do not support auto-gen mipgen for volume textures
			if ((resDim != D3D11_RESOURCE_DIMENSION_TEXTURE3D)
				|| (d3dDevice->GetFeatureLevel() >= D3D_FEATURE_LEVEL_10_0))
			{
				autogen = true;
#if defined(_XBOX_ONE) && defined(_TITLE)
				if (!d3dDeviceX || !d3dContextX)
					return E_INVALIDARG;
#endif
			}
		}
	}

	if (autogen)
	{
		// Create texture with auto-generated mipmaps
		ID3D11Resource* tex = nullptr;
		hr = CreateD3DResources(d3dDevice, resDim, width, height, depth, 0, arraySize,
			format, usage,
			bindFlags | D3D11_BIND_RENDER_TARGET,
			cpuAccessFlags,
			miscFlags | D3D11_RESOURCE_MISC_GENERATE_MIPS, forceSRGB,
			isCubeMap, nullptr, &tex, textureView);
		if (SUCCEEDED(hr))
		{
			mu_size numBytes = 0;
			mu_size rowBytes = 0;
			MU::GetSurfaceInfo(width, height, format, &numBytes, &rowBytes, nullptr);

			if (numBytes > bitSize)
			{
				(*textureView)->Release();
				*textureView = nullptr;
				tex->Release();
				return HRESULT_FROM_WIN32(ERROR_HANDLE_EOF);
			}

			D3D11_SHADER_RESOURCE_VIEW_DESC desc;
			(*textureView)->GetDesc(&desc);

			UINT mipLevels = 1;

			switch (desc.ViewDimension)
			{
			case D3D_SRV_DIMENSION_TEXTURE1D:       mipLevels = desc.Texture1D.MipLevels; break;
			case D3D_SRV_DIMENSION_TEXTURE1DARRAY:  mipLevels = desc.Texture1DArray.MipLevels; break;
			case D3D_SRV_DIMENSION_TEXTURE2D:       mipLevels = desc.Texture2D.MipLevels; break;
			case D3D_SRV_DIMENSION_TEXTURE2DARRAY:  mipLevels = desc.Texture2DArray.MipLevels; break;
			case D3D_SRV_DIMENSION_TEXTURECUBE:     mipLevels = desc.TextureCube.MipLevels; break;
			case D3D_SRV_DIMENSION_TEXTURECUBEARRAY:mipLevels = desc.TextureCubeArray.MipLevels; break;
			case D3D_SRV_DIMENSION_TEXTURE3D:       mipLevels = desc.Texture3D.MipLevels; break;
			default:
				(*textureView)->Release();
				*textureView = nullptr;
				tex->Release();
				return E_UNEXPECTED;
			}

			if (arraySize > 1)
			{
				const mu_uint8* pSrcBits = bitData;
				const mu_uint8* pEndBits = bitData + bitSize;
				for (UINT item = 0; item < arraySize; ++item)
				{
					if ((pSrcBits + numBytes) > pEndBits)
					{
						(*textureView)->Release();
						*textureView = nullptr;
						tex->Release();
						return HRESULT_FROM_WIN32(ERROR_HANDLE_EOF);
					}

					UINT res = D3D11CalcSubresource(0, item, mipLevels);
					d3dContext->UpdateSubresource(tex, res, nullptr, pSrcBits, static_cast<UINT>(rowBytes), static_cast<UINT>(numBytes));
					pSrcBits += numBytes;
				}
			}
			else
			{
				d3dContext->UpdateSubresource(tex, 0, nullptr, bitData, static_cast<UINT>(rowBytes), static_cast<UINT>(numBytes));
			}

			d3dContext->GenerateMips(*textureView);

			if (texture)
			{
				*texture = tex;
			}
			else
			{
				tex->Release();
			}
		}
	}
	else
	{
		// Create the texture
		std::unique_ptr<D3D11_SUBRESOURCE_DATA[]> initData(new (std::nothrow) D3D11_SUBRESOURCE_DATA[mipCount * arraySize]);
		if (!initData)
		{
			return E_OUTOFMEMORY;
		}

		mu_size skipMip = 0;
		mu_size twidth = 0;
		mu_size theight = 0;
		mu_size tdepth = 0;
		hr = FillInitData(width, height, depth, mipCount, arraySize, format, maxsize, bitSize, bitData,
			twidth, theight, tdepth, skipMip, initData.get());

		if (SUCCEEDED(hr))
		{
			hr = CreateD3DResources(d3dDevice, resDim, twidth, theight, tdepth, mipCount - skipMip, arraySize,
				format, usage, bindFlags, cpuAccessFlags, miscFlags, forceSRGB,
				isCubeMap, initData.get(), texture, textureView);

			if (FAILED(hr) && !maxsize && (mipCount > 1))
			{
				// Retry with a maxsize determined by feature level
				switch (d3dDevice->GetFeatureLevel())
				{
				case D3D_FEATURE_LEVEL_9_1:
				case D3D_FEATURE_LEVEL_9_2:
					if (isCubeMap)
					{
						maxsize = 512 /*D3D_FL9_1_REQ_TEXTURECUBE_DIMENSION*/;
					}
					else
					{
						maxsize = (resDim == D3D11_RESOURCE_DIMENSION_TEXTURE3D)
							? 256 /*D3D_FL9_1_REQ_TEXTURE3D_U_V_OR_W_DIMENSION*/
							: 2048 /*D3D_FL9_1_REQ_TEXTURE2D_U_OR_V_DIMENSION*/;
					}
					break;

				case D3D_FEATURE_LEVEL_9_3:
					maxsize = (resDim == D3D11_RESOURCE_DIMENSION_TEXTURE3D)
						? 256 /*D3D_FL9_1_REQ_TEXTURE3D_U_V_OR_W_DIMENSION*/
						: 4096 /*D3D_FL9_3_REQ_TEXTURE2D_U_OR_V_DIMENSION*/;
					break;

				default: // D3D_FEATURE_LEVEL_10_0 & D3D_FEATURE_LEVEL_10_1
					maxsize = (resDim == D3D11_RESOURCE_DIMENSION_TEXTURE3D)
						? 2048 /*D3D10_REQ_TEXTURE3D_U_V_OR_W_DIMENSION*/
						: 8192 /*D3D10_REQ_TEXTURE2D_U_OR_V_DIMENSION*/;
					break;
				}

				hr = FillInitData(width, height, depth, mipCount, arraySize, format, maxsize, bitSize, bitData,
					twidth, theight, tdepth, skipMip, initData.get());
				if (SUCCEEDED(hr))
				{
					hr = CreateD3DResources(d3dDevice, resDim, twidth, theight, tdepth, mipCount - skipMip, arraySize,
						format, usage, bindFlags, cpuAccessFlags, miscFlags, forceSRGB,
						isCubeMap, initData.get(), texture, textureView);
				}
			}
		}
	}

	return hr;
}

HRESULT CreateDDSTextureFromMemoryEx(ID3D11Device* d3dDevice,
	const mu_uint8* ddsData,
	mu_size ddsDataSize,
	mu_size maxsize,
	D3D11_USAGE usage,
	mu_uint32 bindFlags,
	mu_uint32 cpuAccessFlags,
	mu_uint32 miscFlags,
	mu_boolean forceSRGB,
	ID3D11Resource** texture,
	ID3D11ShaderResourceView** textureView)
{
	if (texture)
	{
		*texture = nullptr;
	}
	if (textureView)
	{
		*textureView = nullptr;
	}

	if (!d3dDevice || !ddsData || (!texture && !textureView))
	{
		return E_INVALIDARG;
	}

	// Validate DDS file in memory
	if (ddsDataSize < (sizeof(mu_uint32) + sizeof(DDS_HEADER)))
	{
		return E_FAIL;
	}

	mu_uint32 dwMagicNumber = *(const mu_uint32*)(ddsData);
	if (dwMagicNumber != DDS_MAGIC)
	{
		return E_FAIL;
	}

	auto header = reinterpret_cast<const DDS_HEADER*>(ddsData + sizeof(mu_uint32));

	// Verify header to validate DDS file
	if (header->size != sizeof(DDS_HEADER) ||
		header->ddspf.size != sizeof(DDS_PIXELFORMAT))
	{
		return E_FAIL;
	}

	// Check for DX10 extension
	mu_boolean bDXT10Header = false;
	if ((header->ddspf.flags & DDS_FOURCC) &&
		(MAKEFOURCC('D', 'X', '1', '0') == header->ddspf.fourCC))
	{
		// Must be long enough for both headers and magic value
		if (ddsDataSize < (sizeof(DDS_HEADER) + sizeof(mu_uint32) + sizeof(DDS_HEADER_DXT10)))
		{
			return E_FAIL;
		}

		bDXT10Header = true;
	}

	ptrdiff_t offset = sizeof(mu_uint32)
		+ sizeof(DDS_HEADER)
		+ (bDXT10Header ? sizeof(DDS_HEADER_DXT10) : 0);

	HRESULT hr = CreateTextureFromDDS(d3dDevice, nullptr,
		header, ddsData + offset, ddsDataSize - offset, maxsize,
		usage, bindFlags, cpuAccessFlags, miscFlags, forceSRGB,
		texture, textureView);

	return hr;
}
#endif