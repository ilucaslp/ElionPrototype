#ifndef __MU_RENDERSLOT_H__
#define __MU_RENDERSLOT_H__

#pragma once

#define MAX_RENDERSLOT 300000
#define MAX_CLOTH_RENDERSLOT (1000 * 20)
#define MAX_GUIRENDERSLOT 50000

EINLINE mu_uint64 RetrieveRenderSlotHash(mu_uint8 CulledType, mu_uint16 ObjectIndex, mu_uint8 RenderIndex)
{
	return (((mu_uint64)CulledType << 24) | ((mu_uint64)ObjectIndex << 8) | (mu_uint64)RenderIndex);
}

EINLINE mu_uint64 RetrieveClothHighHash(mu_uint8 RenderIndex, mu_uint16 Model, mu_uint16 Texture)
{
	return (((mu_uint64)RenderIndex << 32) | ((mu_uint64)Model << 16) | (mu_uint64)Texture);
}

EINLINE mu_uint64 RetrieveClothLowHash(mu_uint8 RenderType, mu_uint8 BlendType, mu_uint16 ExtraFlag, mu_uint8 MeshIndex)
{
	return (((mu_uint64)RenderType << 32) | ((mu_uint64)BlendType << 24) | ((mu_uint64)ExtraFlag << 8) | (mu_uint64)MeshIndex);
}

EINLINE mu_uint64 RetrieveGUIHash(mu_uint16 RenderIndex, mu_uint16 Model, mu_uint8 BlendType, mu_uint8 MeshIndex)
{
	return (((mu_uint64)RenderIndex << 32) | ((mu_uint64)Model << 16) | ((mu_uint64)BlendType << 8) | (mu_uint64)MeshIndex);
}

// CulledType(uint8) | ObjectIndex(uint16) | RenderIndex(uint8)
class MURenderSlot
{
public:
	enum
	{
		eRSF_SHADOW = (1 << 0),
		eRSF_WAVE = (1 << 1),
		eRSF_LIGHT = (1 << 2),
		eRSF_TRANSLATE = (1 << 3),
	};
public:
	mu_uint64 Hash;

	MUFlagset<mu_uint8> Flags;

// 	mu_boolean EnableShadow;
// 	mu_boolean EnableWave;
// 	mu_boolean EnableLight;
// 	mu_boolean Translate;

	mu_uint8 CulledType;
	mu_uint8 RenderIndex;
	mu_uint8 MeshIndex;
	mu_uint8 RenderType;
	mu_uint8 BlendType;

	mu_uint16 ExtraFlag;
	mu_uint16 ModelIndex;
	mu_uint16 Texture;

	mu_uint32 SkeletalIndex;
	mu_uint32 InstanceIndex;
	mu_uint32 ProxyIndex;

	mu_float MinAlpha;
	mu_float BlendMeshLight;
	mu_float BlendMeshTexCoordU;
	mu_float BlendMeshTexCoordV;
	mu_float ShadowHeight;

	mu_float BoneScale;
	EVector3 BodyOrigin;
	EVector3 BodyAngle;
	mu_float BodyScale;
	mu_uint32 BodyLight;
	mu_uint32 ShadowLight;
};

class MUClothRenderSlot
{
public:
	mu_uint64 HighHash;
	mu_uint64 LowHash;

	mu_boolean EnableShadow;

	mu_uint8 CulledType;
	mu_uint8 RenderIndex;
	mu_uint8 MeshIndex;
	mu_uint8 RenderType;
	mu_uint8 BlendType;

	mu_uint16 ExtraFlag;
	mu_uint16 ModelIndex;
	mu_uint16 Texture;

	mu_uint32 BaseIndex;
	mu_uint32 IndexCount;
	mu_uint32 InstanceIndex;
};

class MUGUIRenderSlot
{
public:
	mu_uint64 Hash;

	mu_uint8 MeshIndex;
	mu_uint8 RenderType;
	mu_uint8 BlendType;

	mu_uint16 ExtraFlag;
	mu_uint16 ModelIndex;
	mu_uint16 Texture;

	mu_uint32 InstanceIndex;
};

extern MURenderSlot g_PreRenderSlot[MAX_RENDERSLOT];
extern MURenderSlot g_PostRenderSlot[MAX_RENDERSLOT];
extern MUClothRenderSlot g_ClothRenderSlot[MAX_CLOTH_RENDERSLOT];
extern MUGUIRenderSlot g_GUIRenderSlot[MAX_GUIRENDERSLOT];
extern mu_uint32 g_PreRenderSlotUsed[MAX_RENDERSLOT]; // Fast Arrange
extern mu_uint32 g_ClothRenderSlotUsed[MAX_CLOTH_RENDERSLOT]; // Fast Arrange
extern mu_uint32 g_GUIRenderSlotUsed[MAX_GUIRENDERSLOT]; // Fast Arrange
extern mu_atomic_uint32_t g_PreRenderSlotCount;
extern mu_atomic_uint32_t g_PostRenderSlotCount;
extern mu_atomic_uint32_t g_ClothRenderSlotCount;
extern mu_uint32 g_GUIRenderSlotCount;

struct MURenderSlotFastSort
{
	EINLINE constexpr mu_boolean operator()(const mu_uint32 &a, const mu_uint32 &b) const
	{
		return g_PreRenderSlot[a].Hash < g_PreRenderSlot[b].Hash;
	}
};

struct MUClothRenderSlotFastSort
{
	EINLINE constexpr mu_boolean operator()(const mu_uint32 &a, const mu_uint32 &b) const
	{
		return g_ClothRenderSlot[a].HighHash < g_ClothRenderSlot[b].HighHash ||
			(g_ClothRenderSlot[a].HighHash == g_ClothRenderSlot[b].HighHash &&
				g_ClothRenderSlot[a].LowHash < g_ClothRenderSlot[b].LowHash);
	}
};

struct MUGUIRenderSlotFastSort
{
	EINLINE constexpr mu_boolean operator()(const mu_uint32 &a, const mu_uint32 &b) const
	{
		return g_GUIRenderSlot[a].Hash < g_GUIRenderSlot[b].Hash;
	}
};

#endif