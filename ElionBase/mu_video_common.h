#ifndef __MU_VIDEO_COMMON_H__
#define __MU_VIDEO_COMMON_H__

#pragma once

#define SHADOWMAP_FORMAT (ETextureFormat::eDepth16_Unorm)
#define SHADOWMAP_SAMPLER (ESampler::eShadowPCF)
#define SHADOWMAP_RESOLUTION 2048
#define SHADOWMAP_ORTHOWIDTH (3072)
#define SHADOWMAP_ORTHOHEIGHT (3072)
#define SHADOWMAP_TRANSPARENCY 0.5f

#define SKELETON_TEXTURE_WIDTH 1024
#if ELION_MOBILE_DEVICE == 1
#define SKELETON_TEXTURE_HEIGHT 64
#else
#define SKELETON_TEXTURE_HEIGHT 512
#endif
#define SKELETON_TEXTURE_FORMAT (ETextureFormat::eRGBA32_Float)
#define SKELETON_TEXTURE_TEXELSIZE (sizeof(mu_float) * 4)
#ifdef USE_DUAL_QUATERNION
#define MAX_SKELETON ((SKELETON_TEXTURE_WIDTH * SKELETON_TEXTURE_HEIGHT) / 2)
#else
#define MAX_SKELETON ((SKELETON_TEXTURE_WIDTH * SKELETON_TEXTURE_HEIGHT) / 4)
#endif

#define FONT_TEXTURE_FORMAT (ETextureFormat::eRG8_Unorm)
#define FONT_TEXTURE_SAMPLER (ESampler::eUcVcWcMinLMagLMipL_Anisotropy)
#define FONT_TEXTURE_PITCH(Width) (sizeof(mu_uint8) * GetTextureComponents(FONT_TEXTURE_FORMAT) * Width)

constexpr mu_size SkeletonMemorySize = SKELETON_TEXTURE_WIDTH * SKELETON_TEXTURE_HEIGHT * 4 * sizeof(mu_float);

enum TEXTURE_SAMPLER_ENUM
{
	TSAMPLER_NEAREST_CLAMP,
	TSAMPLER_NEAREST_REPEAT,
	TSAMPLER_LINEAR_CLAMP,
	TSAMPLER_LINEAR_REPEAT,
	TSAMPLER_MAX,
};

extern const ESampler TextureEnumToSamplers[TSAMPLER_MAX];

#if defined(USE_COMPRESSED_TERRAINLIGHT_RGB8)
#define TERRAIN_LIGHT_FORMAT (ETextureFormat::eRGBA8_Unorm)
#define TERRAIN_LIGHT_FORMAT_TYPESIZE (sizeof(mu_uint8) * 4)
#define TERRAIN_LIGHT_FORMAT_MEMORYSIZE (TERRAIN_SIZE * TERRAIN_SIZE * TERRAIN_LIGHT_FORMAT_TYPESIZE)
#else
#define TERRAIN_LIGHT_FORMAT (ETextureFormat::eRGBA32_Float)
#define TERRAIN_LIGHT_FORMAT_TYPESIZE (sizeof(mu_float) * 4)
#define TERRAIN_LIGHT_FORMAT_MEMORYSIZE (TERRAIN_SIZE * TERRAIN_SIZE * TERRAIN_LIGHT_FORMAT_TYPESIZE)
#endif
#define TERRAIN_LIGHT_SAMPLER (ESampler::eUcVcWcMinNMagNMipN)
#define TERRAIN_ALPHA_SAMPLER (ESampler::eUcVcWcMinNMagNMipN)
#define TERRAIN_HEIGHT_SAMPLER (ESampler::eUcVcWcMinNMagNMipN)
#define TERRAIN_ATTRIBUTE_SAMPLER (ESampler::eUcVcWcMinNMagNMipN)

struct EViewport
{
	mu_float OffsetX;
	mu_float OffsetY;
	mu_float Width;
	mu_float Height;
	mu_float MinDepth;
	mu_float MaxDepth;
};

const mu_boolean RetrieveCompatibleResolution(SDL_DisplayMode &Resolution, const mu_uint32 Width, const mu_uint32 Height, const mu_boolean AnyCloseResolution = false);

#endif