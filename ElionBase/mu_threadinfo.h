#ifndef __MU_THREADINFO_H__
#define __MU_THREADINFO_H__

#pragma once

struct MapGrid
{
	mu_uint32 PathCost : 22;
	mu_uint16 MoveCount : 16;
	mu_uint8 MoveIndex : 3;
};

struct NodeGrid
{
	mu_uint8 PathX;
	mu_uint8 PathY;
};

class MUThreadInfo
{
public:
	MUThreadInfo();

	mu_boolean NoLockedAllowed;

	mu_boolean Translate;
	mu_boolean EnableShadow;
	mu_boolean RenderGUI;
	mu_boolean EnableScissor;

	EFRect ScissorRect;

	mu_uint8 RenderIndex;
	mu_uint8 CulledType;

	mu_uint16 GUIRenderIndex;

	mu_uint32 ExtraFlag;
	mu_uint32 ObjectIndex;
	mu_uint32 SkeletalIndex;
	mu_uint32 AlphaTestIndex;

	mu_int32 CurrentSkill;
	mu_int32 LimitAttackTime;
	mu_int32 EquipmentLevelSet;

	mu_float MinAlpha;
	mu_float BoneScale;

	mu_float ShadowHeight;
	mu_uint32 ShadowLight;
	mu_uint32 RenderLight;

#ifdef USE_DUAL_QUATERNION
	EDualQuat ParentMatrix;
	EDualQuat BoneTransform[MAX_BONES];
	EDualQuat BoneTransform2[MAX_BONES];
#else
	EMatrix4 ParentMatrix;
	EMatrix4 BoneTransform[MAX_BONES];
	EMatrix4 BoneTransform2[MAX_BONES];
#endif

	EVector3 BonePosition[MAX_BONES];
	EVector3 VertexTransform[MAX_VERTICES];
	EVector3 NormalTransform[MAX_VERTICES];

	mu_boolean SelectFlag;
	mu_float SelectXF;
	mu_float SelectYF;

	mu_float CollisionDistance;
	EVector3 CollisionPosition;

	MUBitset<MAX_MAP_SIZE * MAX_MAP_SIZE> MapNodesVisited;
	MapGrid MapGrids[MAX_MAP_SIZE*MAX_MAP_SIZE];
	NodeGrid NodesQueue[MAX_GRID_QUEUE];

	mu_int32 NumPath;
	mu_uint8 DirPath[MAX_COUNT_PATH];
	mu_uint8 PathX[MAX_COUNT_PATH];
	mu_uint8 PathY[MAX_COUNT_PATH];
};

extern MUThreadInfo *g_ThreadInfo;

#define TYPE_QUEUECOUNT std::array<mu_uint32, MAX_TASKING_THREADS>
#define TYPE_QUEUE(N) std::array<mu_uint16, N + MAX_TASKING_THREADS>

namespace MUThreadQueues
{
	extern TYPE_QUEUECOUNT EnvironmentObjectsQueueCount;
	extern TYPE_QUEUECOUNT CharactersQueueCount;
	extern TYPE_QUEUECOUNT PetObjectsQueueCount;
	extern TYPE_QUEUECOUNT BoidsQueueCount;
	extern TYPE_QUEUECOUNT FishsQueueCount;
	extern TYPE_QUEUECOUNT ItemsQueueCount;
	extern TYPE_QUEUECOUNT ButterflesQueueCount;
	extern TYPE_QUEUECOUNT EffectsQueueCount;
	extern TYPE_QUEUECOUNT JointsQueueCount;
	extern TYPE_QUEUECOUNT BlursQueueCount;
	extern TYPE_QUEUECOUNT ObjectBlursQueueCount;
	extern TYPE_QUEUECOUNT ParticlesQueueCount;
	extern TYPE_QUEUECOUNT PointersQueueCount;
	extern TYPE_QUEUECOUNT TerrainCollisionQueueCount;

	extern TYPE_QUEUECOUNT EnvironmentObjectsQueueIndex;
	extern TYPE_QUEUECOUNT CharactersQueueIndex;
	extern TYPE_QUEUECOUNT PetObjectsQueueIndex;
	extern TYPE_QUEUECOUNT BoidsQueueIndex;
	extern TYPE_QUEUECOUNT FishsQueueIndex;
	extern TYPE_QUEUECOUNT ItemsQueueIndex;
	extern TYPE_QUEUECOUNT ButterflesQueueIndex;
	extern TYPE_QUEUECOUNT EffectsQueueIndex;
	extern TYPE_QUEUECOUNT JointsQueueIndex;
	extern TYPE_QUEUECOUNT BlursQueueIndex;
	extern TYPE_QUEUECOUNT ObjectBlursQueueIndex;
	extern TYPE_QUEUECOUNT ParticlesQueueIndex;
	extern TYPE_QUEUECOUNT PointersQueueIndex;
	extern TYPE_QUEUECOUNT TerrainCollisionQueueIndex;

	extern TYPE_QUEUE(MAX_OBJECTS) EnvironmentObjectsQueue;
	extern TYPE_QUEUE(MAX_CHARACTERS_CLIENT) CharactersQueue;
	extern TYPE_QUEUE(MAX_PETS) PetObjectsQueue;
	extern TYPE_QUEUE(MAX_BOIDS) BoidsQueue;
	extern TYPE_QUEUE(MAX_FISHS) FishsQueue;
	extern TYPE_QUEUE(MAX_ITEMS) ItemsQueue;
	extern TYPE_QUEUE(MAX_BUTTERFLES) ButterflesQueue;
	extern TYPE_QUEUE(MAX_EFFECTS) EffectsQueue;
	extern TYPE_QUEUE(MAX_JOINTS) JointsQueue;
	extern TYPE_QUEUE(MAX_BLURS) BlursQueue;
	extern TYPE_QUEUE(MAX_OBJECTBLURS) ObjectBlursQueue;
	extern TYPE_QUEUE(MAX_PARTICLES) ParticlesQueue;
	extern TYPE_QUEUE(MAX_POINTERS) PointersQueue;
	extern TYPE_QUEUE(MAX_TERRAIN_SIZE) TerrainCollisionQueue;

	extern std::array<mu_uint32, MAX_OBJECTBLURS> PerObjectBlursRenderCount;
	extern std::array<mu_uint32, MAX_TILESPRITES> PerTileSpritesRenderCount;
};

#endif