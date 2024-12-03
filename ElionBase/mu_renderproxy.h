#ifndef __MU_RENDER_PROXY_H__
#define __MU_RENDER_PROXY_H__

#pragma once

#include "mu_hashtypes.h"

#define MAX_RENDERPROXY 5000

#define INVALID_RENDERHASH ((mu_uint64)-1)
#define WAITING_PROXY ((mu_uint32)-1)

EINLINE mu_modelhash RetrieveRenderProxyHash(mu_uint8 RenderIndex, mu_uint16 Model, mu_uint8 RenderType, mu_uint8 BlendType, mu_uint16 ExtraFlag, mu_uint16 Texture, mu_uint8 MeshIndex)
{
	using namespace EModelBits;
	return ((mu_modelhash)RenderIndex << RenderIndexLshBits) |
		((mu_modelhash)Model << ModelIndexLshBits) |
		((mu_modelhash)RenderType << RenderTypeLshBits) |
		((mu_modelhash)BlendType << BlendBits) |
		((mu_modelhash)ExtraFlag << RexfLshBits) |
		((mu_modelhash)Texture << TextureLshBits) |
		((mu_modelhash)MeshIndex << MeshIndexLshBits);
}

EINLINE mu_texturehash RetrieveTextureHash(mu_uint16 Type, mu_uint16 RenderType)
{
	using namespace ETextureBits;
	return (((mu_texturehash)Type << 16) | (mu_texturehash)RenderType);
}

class MURenderProxy
{
public:
	mu_modelhash Hash;

	mu_uint16 ModelIndex;
	mu_uint16 TextureIndex;
	mu_uint16 ExtraFlag;

	mu_uint8 RenderIndex;
	mu_uint8 RenderType;
	mu_uint8 BlendType;
	mu_uint8 MeshIndex;

	mu_atomic_bool Enabled;
	mu_atomic_bool ShadowEnabled;

	mu_atomic_uint32_t InstanceCount;
	mu_uint32 InstanceBase;
};

extern mu_uint32 g_RenderProxyCount;
extern mu_boolean g_RenderProxySort;
extern MURenderProxy g_RenderProxy[MAX_RENDERPROXY];
extern mu_uint32 g_RenderProxyIndex[MAX_RENDERPROXY];
extern std::map<mu_uint64, mu_uint32> g_RenderProxyMap;
extern std::map<mu_uint64, mu_uint32> g_RenderProxyFlyMap;

mu_uint32 FindRenderProxy(mu_uint8 RenderIndex, mu_uint16 Model, mu_uint8 RenderType, mu_uint8 BlendType, mu_uint16 ExtraFlag, mu_uint16 Texture, mu_uint8 MeshIndex);
void MergeRenderProxy();

struct RenderProxyFastSort
{
	EINLINE constexpr mu_boolean operator()(mu_uint32 a, mu_uint32 b) const
	{
		return g_RenderProxy[a].Hash < g_RenderProxy[b].Hash;
	}
};

extern RenderProxyFastSort g_RenderProxyFastSort;

#endif