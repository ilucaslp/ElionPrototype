#include "stdafx.h"

#if DIRECTX_BACKEND_ENABLE == 1
#include "mu_directx_texture_formats.h"
#include "mu_dds.h"

namespace EDirectXTexture
{
	const DXGI_FORMAT RetrieveFormat(const ETextureFormat format)
	{
		return DirectXFormats[ECastEnum(format)];
	}

	const DXGI_FORMAT RetrieveFormatTypeless(const ETextureFormat format)
	{
		return DirectXFormatsTypeless[ECastEnum(format)];
	}

	const DXGI_FORMAT RetrieveFormatDirect(const ETextureFormat format)
	{
		return DirectXFormatsDirect[ECastEnum(format)];
	}

	const mu_uint32 RetrieveFormatSize(const ETextureFormat format)
	{
		return DirectXFormatSizes[ECastEnum(format)];
	}

	const D3D11_USAGE RetrieveUsage(const ETextureUsage usage)
	{
		return DirectXUsages[ECastEnum(usage)];
	}

	const mu_boolean LoadFromBuffer(const mu_uint8 *DataBuffer, const mu_uint32 DataSize, ETextureBackend *Texture)
	{
#if ELION_TEXTURETYPE_FORMAT != ELION_TEXTURETYPE_DDS
#error "DirectX support DDS only"
		return false;
#elif ELION_TEXTURETYPE_FORMAT == ELION_TEXTURETYPE_DDS
		HRESULT result = CreateDDSTextureFromMemoryEx(EDirectXInit::Device, DataBuffer, DataSize, 0, D3D11_USAGE_IMMUTABLE, D3D11_BIND_SHADER_RESOURCE, 0, 0, false, &Texture->DirectXInternal.TextureResource, &Texture->DirectXInternal.ShaderView);
		if (FAILED(result))
		{
			return false;
		}

		return true;
#endif
	}

	const mu_boolean CreateBones(ETextureBackend *Texture)
	{
		constexpr ETextureFormat Format = SKELETON_TEXTURE_FORMAT;
		constexpr mu_uint32 Width = SKELETON_TEXTURE_WIDTH;
		constexpr mu_uint32 Height = SKELETON_TEXTURE_HEIGHT;

		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = Width;
		textureDesc.Height = Height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = RetrieveFormat(Format);
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		HRESULT result = EDirectXInit::Device->CreateTexture2D(&textureDesc, nullptr, reinterpret_cast<ID3D11Texture2D**>(&Texture->DirectXInternal.TextureResource));
		if (FAILED(result))
		{
			return false;
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = RetrieveFormat(Format);
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
		srvDesc.Texture2D.MostDetailedMip = 0;

		result = EDirectXInit::Device->CreateShaderResourceView(Texture->DirectXInternal.TextureResource, &srvDesc, &Texture->DirectXInternal.ShaderView);
		if (FAILED(result))
		{
			return false;
		}

		Texture->DirectXInternal.Usage = textureDesc.Usage;
		Texture->TextureInternal.Width = Width;
		Texture->TextureInternal.Height = Height;
		Texture->TextureInternal.Depth = 1;
		Texture->TextureInternal.Components = GetTextureComponents(Format);
		Texture->TextureInternal.Sampler = ESampler::eUcVcWcMinNMagNMipN;
		Texture->TextureInternal.Loaded = true;

		return true;
	}

	const mu_boolean CreateShadowMap(ETextureBackend *Texture)
	{
		constexpr ETextureFormat Format = SHADOWMAP_FORMAT;
		constexpr mu_uint32 Width = SHADOWMAP_RESOLUTION;
		constexpr mu_uint32 Height = SHADOWMAP_RESOLUTION;

		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = Width;
		textureDesc.Height = Height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = RetrieveFormatTypeless(Format);
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		HRESULT result = EDirectXInit::Device->CreateTexture2D(&textureDesc, nullptr, reinterpret_cast<ID3D11Texture2D**>(&Texture->DirectXInternal.TextureResource));
		if (FAILED(result))
		{
			return false;
		}

		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
		dsvDesc.Format = RetrieveFormat(Format);
		dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

		result = EDirectXInit::Device->CreateDepthStencilView(Texture->DirectXInternal.TextureResource, &dsvDesc, &Texture->DirectXInternal.DepthStencilView);
		if (FAILED(result))
		{
			return false;
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = RetrieveFormatDirect(Format);
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
		srvDesc.Texture2D.MostDetailedMip = 0;

		result = EDirectXInit::Device->CreateShaderResourceView(Texture->DirectXInternal.TextureResource, &srvDesc, &Texture->DirectXInternal.ShaderView);
		if (FAILED(result))
		{
			return false;
		}

		Texture->DirectXInternal.Usage = textureDesc.Usage;
		Texture->TextureInternal.Width = Width;
		Texture->TextureInternal.Height = Height;
		Texture->TextureInternal.Depth = 1;
		Texture->TextureInternal.Components = GetTextureComponents(Format);
		Texture->TextureInternal.Sampler = ESampler::eUcVcWcMinNMagNMipN_Anisotropy;
		Texture->TextureInternal.Loaded = true;

		return true;
	}

	const mu_boolean CreateMultisample(ETextureBackend *Texture, const mu_uint32 Count, const mu_uint32 Quality)
	{
		const DXGI_FORMAT Format = EDirectXInit::SwapchainDesc.BufferDesc.Format;
		const mu_uint32 Width = MU_RESOURCE->GetWindowWidth();
		const mu_uint32 Height = MU_RESOURCE->GetWindowHeight();

		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = Width;
		textureDesc.Height = Height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = Format;
		textureDesc.SampleDesc.Count = Count;
		textureDesc.SampleDesc.Quality = Quality;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		HRESULT result = EDirectXInit::Device->CreateTexture2D(&textureDesc, nullptr, reinterpret_cast<ID3D11Texture2D**>(&Texture->DirectXInternal.TextureResource));
		if (FAILED(result))
		{
			return false;
		}

		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
		rtvDesc.Format = textureDesc.Format;
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;

		result = EDirectXInit::Device->CreateRenderTargetView(Texture->DirectXInternal.TextureResource, &rtvDesc, &Texture->DirectXInternal.RenderTargetView);
		if (FAILED(result))
		{
			return false;
		}

		Texture->DirectXInternal.Usage = textureDesc.Usage;
		Texture->TextureInternal.Width = Width;
		Texture->TextureInternal.Height = Height;
		Texture->TextureInternal.Depth = 1;
		Texture->TextureInternal.Components = 4;
		Texture->TextureInternal.Sampler = ESampler::eUcVcWcMinNMagNMipN_Anisotropy;
		Texture->TextureInternal.Loaded = true;
		Texture->TextureInternal.RenderTarget = true;

		return true;
	}

	const mu_boolean Create1D(const mu_uint32 Width,
		const ETextureFormat Format,
		const ETextureUsage Usage,
		const ESampler Sampler,
		const mu_uint8 *DataBuffer,
		ETextureBackend *Texture)
	{
		D3D11_TEXTURE1D_DESC textureDesc = {};
		textureDesc.Width = Width;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = RetrieveFormat(Format);
		textureDesc.Usage = RetrieveUsage(Usage);
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = Usage == ETextureUsage::eStatic ? 0 : D3D11_CPU_ACCESS_WRITE;
		textureDesc.MiscFlags = 0;

		HRESULT result;

		if (Usage == ETextureUsage::eStatic)
		{
			D3D11_SUBRESOURCE_DATA subresourceData = {};
			subresourceData.pSysMem = DataBuffer;

			result = EDirectXInit::Device->CreateTexture1D(&textureDesc, nullptr, reinterpret_cast<ID3D11Texture1D**>(&Texture->DirectXInternal.TextureResource));
		}
		else
		{
			result = EDirectXInit::Device->CreateTexture1D(&textureDesc, nullptr, reinterpret_cast<ID3D11Texture1D**>(&Texture->DirectXInternal.TextureResource));
		}

		if (FAILED(result))
		{
			return false;
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = RetrieveFormat(Format);
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
		srvDesc.Texture2D.MipLevels = 1;
		srvDesc.Texture2D.MostDetailedMip = 0;

		result = EDirectXInit::Device->CreateShaderResourceView(Texture->DirectXInternal.TextureResource, &srvDesc, &Texture->DirectXInternal.ShaderView);
		if (FAILED(result))
		{
			return false;
		}

		if (Usage != ETextureUsage::eStatic &&
			DataBuffer != nullptr)
		{
			const mu_uint32 formatSize = RetrieveFormatSize(Format);

			D3D11_MAPPED_SUBRESOURCE mappedResource = {};
			result = EDirectXInit::DeviceContext->Map(Texture->DirectXInternal.TextureResource, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			if (FAILED(result))
			{
				return false;
			}

			mu_memcpy(mappedResource.pData, DataBuffer, formatSize * Width);

			EDirectXInit::DeviceContext->Unmap(Texture->DirectXInternal.TextureResource, 0);
		}

		Texture->DirectXInternal.Usage = textureDesc.Usage;
		Texture->TextureInternal.Width = Width;
		Texture->TextureInternal.Height = 1;
		Texture->TextureInternal.Depth = 1;
		Texture->TextureInternal.Components = GetTextureComponents(Format);
		Texture->TextureInternal.Sampler = Sampler;
		Texture->TextureInternal.Loaded = true;

		return true;
	}

	const mu_boolean Create2D(const mu_uint32 Width,
		const mu_uint32 Height,
		const ETextureFormat Format,
		const ETextureUsage Usage,
		const ESampler Sampler,
		const mu_uint8 *DataBuffer,
		const mu_uint32 Pitch,
		ETextureBackend *Texture)
	{
		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = Width;
		textureDesc.Height = Height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = RetrieveFormat(Format);
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = RetrieveUsage(Usage);
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = Usage == ETextureUsage::eStatic ? 0 : D3D11_CPU_ACCESS_WRITE;
		textureDesc.MiscFlags = 0;

		HRESULT result;
		if (Usage == ETextureUsage::eStatic)
		{
			const mu_uint32 formatSize = RetrieveFormatSize(Format);

			D3D11_SUBRESOURCE_DATA subresourceData = {};
			subresourceData.pSysMem = DataBuffer;
			subresourceData.SysMemPitch = Width * formatSize;

			result = EDirectXInit::Device->CreateTexture2D(&textureDesc, &subresourceData, reinterpret_cast<ID3D11Texture2D**>(&Texture->DirectXInternal.TextureResource));
		}
		else
		{
			result = EDirectXInit::Device->CreateTexture2D(&textureDesc, nullptr, reinterpret_cast<ID3D11Texture2D**>(&Texture->DirectXInternal.TextureResource));
		}
		
		if (FAILED(result))
		{
			return false;
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = RetrieveFormat(Format);
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
		srvDesc.Texture2D.MostDetailedMip = 0;

		result = EDirectXInit::Device->CreateShaderResourceView(Texture->DirectXInternal.TextureResource, &srvDesc, &Texture->DirectXInternal.ShaderView);
		if (FAILED(result))
		{
			return false;
		}

		if (Usage != ETextureUsage::eStatic &&
			DataBuffer != nullptr)
		{
			const mu_uint32 formatSize = RetrieveFormatSize(Format);

			D3D11_MAPPED_SUBRESOURCE mappedResource = {};
			result = EDirectXInit::DeviceContext->Map(Texture->DirectXInternal.TextureResource, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			if (FAILED(result))
			{
				return false;
			}

			const mu_uint32 srcPitch = formatSize * Width;
			const mu_uint8 *srcBuffer = reinterpret_cast<const mu_uint8*>(DataBuffer);
			mu_uint8 *destBuffer = reinterpret_cast<mu_uint8*>(mappedResource.pData);
			for (mu_uint32 y = 0; y < Height; ++y, destBuffer += mappedResource.RowPitch, srcBuffer += srcPitch)
			{
				mu_memcpy(destBuffer, srcBuffer, srcPitch);
			}

			EDirectXInit::DeviceContext->Unmap(Texture->DirectXInternal.TextureResource, 0);
		}

		Texture->DirectXInternal.Usage = textureDesc.Usage;
		Texture->TextureInternal.Width = Width;
		Texture->TextureInternal.Height = Height;
		Texture->TextureInternal.Depth = 1;
		Texture->TextureInternal.Components = GetTextureComponents(Format);
		Texture->TextureInternal.Sampler = Sampler;
		Texture->TextureInternal.Loaded = true;

		return true;
	}

	const mu_boolean Create2DArray(const mu_uint32 Width,
		const mu_uint32 Height,
		const mu_uint32 Layers,
		const ETextureFormat Format,
		const ETextureUsage Usage,
		const ESampler Sampler,
		const mu_uint8 *DataBuffer,
		const mu_uint32 Pitch,
		ETextureBackend *Texture)
	{
		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = Width;
		textureDesc.Height = Height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = Layers;
		textureDesc.Format = RetrieveFormat(Format);
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = RetrieveUsage(Usage);
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = Usage == ETextureUsage::eStatic ? 0 : D3D11_CPU_ACCESS_WRITE;
		textureDesc.MiscFlags = 0;

		HRESULT result;
		if (Usage == ETextureUsage::eStatic)
		{
			const mu_uint32 formatSize = RetrieveFormatSize(Format);

			D3D11_SUBRESOURCE_DATA subresourceData = {};
			subresourceData.pSysMem = DataBuffer;
			subresourceData.SysMemPitch = Width * formatSize;
			subresourceData.SysMemSlicePitch = Width * Height * formatSize;

			result = EDirectXInit::Device->CreateTexture2D(&textureDesc, &subresourceData, reinterpret_cast<ID3D11Texture2D**>(&Texture->DirectXInternal.TextureResource));
		}
		else
		{
			result = EDirectXInit::Device->CreateTexture2D(&textureDesc, nullptr, reinterpret_cast<ID3D11Texture2D**>(&Texture->DirectXInternal.TextureResource));
		}

		if (FAILED(result))
		{
			return false;
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = RetrieveFormat(Format);
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
		srvDesc.Texture2D.MostDetailedMip = 0;

		result = EDirectXInit::Device->CreateShaderResourceView(Texture->DirectXInternal.TextureResource, &srvDesc, &Texture->DirectXInternal.ShaderView);
		if (FAILED(result))
		{
			return false;
		}

		if (Usage != ETextureUsage::eStatic &&
			DataBuffer != nullptr)
		{
			const mu_uint32 formatSize = RetrieveFormatSize(Format);

			D3D11_MAPPED_SUBRESOURCE mappedResource = {};
			result = EDirectXInit::DeviceContext->Map(Texture->DirectXInternal.TextureResource, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			if (FAILED(result))
			{
				return false;
			}

			const mu_uint32 srcPitch = formatSize * Width;
			const mu_uint32 srcSlicePitch = formatSize * Width * Height;
			const mu_uint8 *srcBuffer = reinterpret_cast<const mu_uint8*>(DataBuffer);
			mu_uint8 *destBuffer = reinterpret_cast<mu_uint8*>(mappedResource.pData);
			for (mu_uint32 l = 0; l < Layers; ++l, destBuffer += mappedResource.DepthPitch, srcBuffer += srcSlicePitch)
			{
				const mu_uint8 *srcBufferRow = srcBuffer;
				mu_uint8 *destBufferRow = destBuffer;
				for (mu_uint32 y = 0; y < Height; ++y, destBufferRow += mappedResource.RowPitch, srcBufferRow += srcPitch)
				{
					mu_memcpy(destBufferRow, srcBufferRow, srcPitch);
				}
			}

			EDirectXInit::DeviceContext->Unmap(Texture->DirectXInternal.TextureResource, 0);
		}

		Texture->DirectXInternal.Usage = textureDesc.Usage;
		Texture->TextureInternal.Width = Width;
		Texture->TextureInternal.Height = Height;
		Texture->TextureInternal.Depth = Layers;
		Texture->TextureInternal.Components = GetTextureComponents(Format);
		Texture->TextureInternal.Sampler = Sampler;
		Texture->TextureInternal.Loaded = true;

		return true;
	}
};
#endif