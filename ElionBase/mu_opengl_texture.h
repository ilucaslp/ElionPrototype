#ifndef __MU_OPENGL_TEXTURE_H__
#define __MU_OPENGL_TEXTURE_H__

#pragma once

class MUTexture;
class ETextureBackend;

namespace EOpenGLTexture
{
	const GLenum RetrieveFormat(const ETextureFormat format);
	const GLenum RetrieveFlatFormat(const ETextureFormat format);
	const GLenum RetrieveScalarFormat(const ETextureFormat format);

	const mu_boolean LoadFromBuffer(const mu_uint8 *DataBuffer, const mu_uint32 DataSize, const EImageViewType ImageViewType, ETextureBackend *Texture);
	
	const mu_boolean CreateBones(ETextureBackend *Texture);
	const mu_boolean CreateShadowMap(ETextureBackend *Texture);
	const mu_boolean CreateMultisample(ETextureBackend *Texture, mu_uint32 Samples);

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