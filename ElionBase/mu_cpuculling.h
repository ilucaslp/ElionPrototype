#ifndef __MU_CPU_CULLING_H__
#define __MU_CPU_CULLING_H__

#pragma once

#include <set>

namespace CPUCulling
{
	enum
	{
		BLOCK_SIZE = 64,
		BLOCK_COUNT = BLOCK_SIZE * BLOCK_SIZE,
		EXTRA_EFFECT_SIZE = 1000,
		CULLED_EFFECTS_SIZE = (MAX_BLURS + MAX_OBJECTBLURS + MAX_JOINTS + MAX_PARTICLES + MAX_POINTERS + MAX_SPRITES + MAX_TILESPRITES + MAX_GUILDSPRITES + MAX_LEAVES + EXTRA_EFFECT_SIZE),
		CULLED_TEXTURES_SIZE = 2000,
	};
};

//#define USE_CULLING_PARALLEL_FOR

extern MU_ALIGN(16, std::array<mu_boolean, CPUCulling::BLOCK_SIZE>) g_FastCullingCheck;

void InitializeGlobalCulling();
void DestroyGlobalCulling();

class MUTerrain;

class MUCPUCulling
{
public:
	static mu_uint32 GetBlockIndex(mu_uint32 X, mu_uint32 Y);
	static mu_uint32 GetBoundingIndex(mu_uint32 X, mu_uint32 Y);

public:
	MUCPUCulling();
	~MUCPUCulling();

	void Update();

	EMathFrustum _Frustum;
	MU_ALIGN(16, EMathAABB) _BoundingBox[CPUCulling::BLOCK_COUNT];
};

enum CULLED_TYPE : mu_uint16
{
	CULLED_OBJECTS,
	CULLED_BOIDS,
	CULLED_CHARACTERS,
	CULLED_PETS,
	CULLED_ITEMS,
	CULLED_FISHS,
	CULLED_BUGS,
	CULLED_LEAVES,
	CULLED_JOINTS,
	CULLED_EFFECTS,
	CULLED_BLURS,
	CULLED_OBJECTBLURS,
	CULLED_SPRITES,
	CULLED_PARTICLES,
	CULLED_POINTERS,
	CULLED_GUILDSPRITES,
	CULLED_TILESPRITES,
};

struct EFFECT_DATA
{
	mu_uint16 Index;
	mu_uint8 Type;
	mu_uint8 Enabled;
};

struct CULLED_DATA
{
	union
	{
		struct 
		{
			mu_uint16 RenderType;
			mu_uint16 TexType;
		};

		mu_texturehash Hash;
	};

	mu_boolean operator < (const CULLED_DATA &data) const
	{
		return Hash < data.Hash;
	}
};

struct MUCulledListFastSort
{
	EINLINE mu_boolean operator()(const CULLED_DATA &a, const CULLED_DATA &b) const
	{
		return a.Hash < b.Hash;
	}
};

extern MUCPUCulling g_CpuCulling;

extern mu_atomic_uint32_t g_EffectsRenderCount;
extern mu_uint32 g_EffectsCulledCount;
extern mu_uint32 g_TexturesCulledCount;
extern mu_uint32 g_PreTexturesCulledCount;
extern mu_uint32 g_PostTexturesCulledCount;
extern mu_uint32 g_EffectsCulledSize;
extern mu_uint32 g_TexturesCulledSize;
extern mu_uint32 g_PreTexturesCulledSize;
extern mu_uint32 g_PostTexturesCulledSize;
extern mu_uint32 *g_TexturesCulledList;
extern CULLED_DATA *g_PreTexturesCulledList;
extern CULLED_DATA *g_PostTexturesCulledList;

extern const MUCulledListFastSort g_CulledListFastSort;

extern MU_ALIGN(16, std::array<mu_boolean, CPUCulling::BLOCK_COUNT>) g_CulledBlockVisible;

extern MUCriticalSectionSpinLock g_CulledLocking;

#endif