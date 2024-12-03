#include "stdafx.h"
#include "mu_tasking.h"

MUThreadInfo *g_ThreadInfo = nullptr;

namespace MUThreadQueues
{
	TYPE_QUEUECOUNT EnvironmentObjectsQueueCount;
	TYPE_QUEUECOUNT CharactersQueueCount;
	TYPE_QUEUECOUNT PetObjectsQueueCount;
	TYPE_QUEUECOUNT BoidsQueueCount;
	TYPE_QUEUECOUNT FishsQueueCount;
	TYPE_QUEUECOUNT ItemsQueueCount;
	TYPE_QUEUECOUNT ButterflesQueueCount;
	TYPE_QUEUECOUNT EffectsQueueCount;
	TYPE_QUEUECOUNT JointsQueueCount;
	TYPE_QUEUECOUNT BlursQueueCount;
	TYPE_QUEUECOUNT ObjectBlursQueueCount;
	TYPE_QUEUECOUNT ParticlesQueueCount;
	TYPE_QUEUECOUNT PointersQueueCount;
	TYPE_QUEUECOUNT TerrainCollisionQueueCount;

	TYPE_QUEUECOUNT EnvironmentObjectsQueueIndex;
	TYPE_QUEUECOUNT CharactersQueueIndex;
	TYPE_QUEUECOUNT PetObjectsQueueIndex;
	TYPE_QUEUECOUNT BoidsQueueIndex;
	TYPE_QUEUECOUNT FishsQueueIndex;
	TYPE_QUEUECOUNT ItemsQueueIndex;
	TYPE_QUEUECOUNT ButterflesQueueIndex;
	TYPE_QUEUECOUNT EffectsQueueIndex;
	TYPE_QUEUECOUNT JointsQueueIndex;
	TYPE_QUEUECOUNT BlursQueueIndex;
	TYPE_QUEUECOUNT ObjectBlursQueueIndex;
	TYPE_QUEUECOUNT ParticlesQueueIndex;
	TYPE_QUEUECOUNT PointersQueueIndex;
	TYPE_QUEUECOUNT TerrainCollisionQueueIndex;

	TYPE_QUEUE(MAX_OBJECTS) EnvironmentObjectsQueue;
	TYPE_QUEUE(MAX_CHARACTERS_CLIENT) CharactersQueue;
	TYPE_QUEUE(MAX_PETS) PetObjectsQueue;
	TYPE_QUEUE(MAX_BOIDS) BoidsQueue;
	TYPE_QUEUE(MAX_FISHS) FishsQueue;
	TYPE_QUEUE(MAX_ITEMS) ItemsQueue;
	TYPE_QUEUE(MAX_BUTTERFLES) ButterflesQueue;
	TYPE_QUEUE(MAX_EFFECTS) EffectsQueue;
	TYPE_QUEUE(MAX_JOINTS) JointsQueue;
	TYPE_QUEUE(MAX_BLURS) BlursQueue;
	TYPE_QUEUE(MAX_OBJECTBLURS) ObjectBlursQueue;
	TYPE_QUEUE(MAX_PARTICLES) ParticlesQueue;
	TYPE_QUEUE(MAX_POINTERS) PointersQueue;
	TYPE_QUEUE(MAX_TERRAIN_SIZE) TerrainCollisionQueue;

	std::array<mu_uint32, MAX_OBJECTBLURS> PerObjectBlursRenderCount;
	std::array<mu_uint32, MAX_TILESPRITES> PerTileSpritesRenderCount;
};

MUThreadInfo::MUThreadInfo()
{
	NoLockedAllowed = false;
	EnableShadow = false;
	RenderGUI = false;
	EnableScissor = false;
	GUIRenderIndex = 0;
	MinAlpha = 0.25f;
	ExtraFlag = 0;
	CurrentSkill = 0;
	LimitAttackTime = 15;
	EquipmentLevelSet = 0;
	AlphaTestIndex = 0;
}