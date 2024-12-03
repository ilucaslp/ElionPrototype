#include "stdafx.h"
#include "mu_cpuculling.h"

MUCPUCulling g_CpuCulling;

mu_atomic_uint32_t g_EffectsRenderCount(0);
mu_uint32 g_EffectsCulledCount = 0;
mu_uint32 g_TexturesCulledCount = 0;
mu_uint32 g_PreTexturesCulledCount = 0;
mu_uint32 g_PostTexturesCulledCount = 0;
mu_uint32 g_EffectsCulledSize = CPUCulling::CULLED_EFFECTS_SIZE;
mu_uint32 g_TexturesCulledSize = CPUCulling::CULLED_TEXTURES_SIZE;
mu_uint32 g_PreTexturesCulledSize = CPUCulling::CULLED_TEXTURES_SIZE;
mu_uint32 g_PostTexturesCulledSize = CPUCulling::CULLED_TEXTURES_SIZE;
mu_uint32 *g_TexturesCulledList = nullptr;
CULLED_DATA *g_PreTexturesCulledList = nullptr;
CULLED_DATA *g_PostTexturesCulledList = nullptr;

const MUCulledListFastSort g_CulledListFastSort;

MUCriticalSectionSpinLock g_CulledLocking;
MU_ALIGN(16, std::array<mu_boolean, CPUCulling::BLOCK_SIZE>) g_FastCullingCheck;
MU_ALIGN(16, std::array<mu_boolean, CPUCulling::BLOCK_COUNT>) g_CulledBlockVisible;

void InitializeGlobalCulling()
{
	ZeroArray(g_FastCullingCheck);

	g_PreTexturesCulledList = new CULLED_DATA[CPUCulling::CULLED_TEXTURES_SIZE];
	g_PostTexturesCulledList = new CULLED_DATA[CPUCulling::CULLED_TEXTURES_SIZE];
	g_TexturesCulledList = new mu_uint32[CPUCulling::CULLED_TEXTURES_SIZE];

	ZeroArray(g_CulledBlockVisible);
}

void DestroyGlobalCulling()
{
	MU_SAFE_DELETE_ARRAY(g_PreTexturesCulledList);
	MU_SAFE_DELETE_ARRAY(g_PostTexturesCulledList);
	MU_SAFE_DELETE_ARRAY(g_TexturesCulledList);
}

mu_uint32 MUCPUCulling::GetBlockIndex(mu_uint32 X, mu_uint32 Y)
{
	return Y * CPUCulling::BLOCK_SIZE + X;
}

mu_uint32 MUCPUCulling::GetBoundingIndex(mu_uint32 X, mu_uint32 Y)
{
	return Y * CPUCulling::BLOCK_SIZE + X;
}

MUCPUCulling::MUCPUCulling()
{
	for (mu_uint32 y1 = 0; y1 < CPUCulling::BLOCK_SIZE; ++y1)
	{
		for (mu_uint32 x1 = 0; x1 < CPUCulling::BLOCK_SIZE; ++x1)
		{
			mu_uint32 x = x1, y = y1;
			mu_uint32 boundingIndex = GetBoundingIndex(x1, y1);
			EMathAABB &box = _BoundingBox[boundingIndex];
			box.CreateFromPoints(EVector3(x * MUTerrain::TERRAIN_BLOCK * g_TerrainScale,
				y * MUTerrain::TERRAIN_BLOCK * g_TerrainScale, -10000.0f),
				EVector3((x + 1) * MUTerrain::TERRAIN_BLOCK * g_TerrainScale,
				(y + 1) * MUTerrain::TERRAIN_BLOCK * g_TerrainScale, 10000.0f));
		}
	}
}

MUCPUCulling::~MUCPUCulling()
{
	
}

void MUCPUCulling::Update()
{
	_Frustum.Update(MUVideoManager::_CurrentProjection * MUVideoManager::_CurrentView);
}