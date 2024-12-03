#ifndef __MU_DIRECTX_TEXTURE_H__
#define __MU_DIRECTX_TEXTURE_H__

#pragma once

class MUTexture;
class ETextureBackend;

namespace EDirectXTexture
{
	const DXGI_FORMAT RetrieveFormat(const ETextureFormat format);
	const mu_uint32 RetrieveFormatSize(const ETextureFormat format);
	const D3D11_USAGE RetrieveUsage(const ETextureUsage usage);

	const mu_boolean LoadFromBuffer(const mu_uint8 *DataBuffer, const mu_uint32 DataSize, ETextureBackend *Texture);

	const mu_boolean CreateBones(ETextureBackend *Texture);
	const mu_boolean CreateShadowMap(ETextureBackend *Texture);
	const mu_boolean CreateMultisample(ETextureBackend *Texture, const mu_uint32 Count, const mu_uint32 Quality);

	const mu_boolean Create1D(const mu_uint32 Width,
		const ETextureFormat Format,
		const ETextureUsage Usage,
		const ESampler Sampler,
		const mu_uint8 *DataBuffer,
		ETextureBackend *Texture);
	const mu_boolean Create2D(const mu_uint32 Width,
		const mu_uint32 Height,
		const ETextureFormat Format,
		const ETextureUsage Usage,
		const ESampler Sampler,
		const mu_uint8 *DataBuffer,
		const mu_uint32 Pitch,
		ETextureBackend *Texture);
	const mu_boolean Create2DArray(const mu_uint32 Width,
		const mu_uint32 Height,
		const mu_uint32 Layers,
		const ETextureFormat Format,
		const ETextureUsage Usage,
		const ESampler Sampler,
		const mu_uint8 *DataBuffer,
		const mu_uint32 Pitch,
		ETextureBackend *Texture);
};

#endif