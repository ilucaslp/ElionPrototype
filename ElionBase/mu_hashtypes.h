#ifndef __MU_HASHTYPES_H__
#define __MU_HASHTYPES_H__

#pragma once

namespace EModelBits
{
	constexpr mu_uint32 MeshIndexBits = ComputeBitsNeeded(MAX_MESH);
	constexpr mu_uint32 TextureIndexBits = ComputeBitsNeeded(MAX_TEXTURES);
	constexpr mu_uint32 RexfBits = ComputeBitsNeeded(REXF_MAX);
	constexpr mu_uint32 BlendBits = ComputeBitsNeeded(BLEND::TXB_MAX);
	constexpr mu_uint32 RenderTypeBits = ComputeBitsNeeded(RT_MAX);
	constexpr mu_uint32 ModelIndexBits = ComputeBitsNeeded(MAX_MODELS);
	constexpr mu_uint32 RenderIndexBits = ComputeBitsNeeded(255);
	constexpr mu_uint32 ModelHashBits = (RenderIndexBits + ModelIndexBits + RenderTypeBits + BlendBits + RexfBits + TextureIndexBits + MeshIndexBits);

	constexpr mu_uint32 MeshIndexLshBits = 0;
	constexpr mu_uint32 TextureLshBits = (MeshIndexLshBits + MeshIndexBits);
	constexpr mu_uint32 RexfLshBits = (TextureLshBits + TextureIndexBits);
	constexpr mu_uint32 BlendLshBits = (RexfLshBits + RexfBits);
	constexpr mu_uint32 RenderTypeLshBits = (BlendLshBits + BlendBits);
	constexpr mu_uint32 ModelIndexLshBits = (RenderTypeLshBits + RenderTypeBits);
	constexpr mu_uint32 RenderIndexLshBits = (ModelIndexLshBits + ModelIndexBits);
};

typedef mu_uint64 mu_modelhash;

namespace ETextureBits
{
	constexpr mu_uint32 TextureIndexBits = ComputeBitsNeeded(MAX_TEXTURES);
	constexpr mu_uint32 RenderTypeBits = ComputeBitsNeeded(BLEND::TXB_MAX);
	constexpr mu_uint32 TextureHashBits = (TextureIndexBits + RenderTypeBits);

	constexpr mu_uint32 RenderTypeLshBits = 0;
	constexpr mu_uint32 TextureIndexLshBits = (RenderTypeLshBits + RenderTypeBits);
};

typedef mu_uint32 mu_texturehash;

#endif