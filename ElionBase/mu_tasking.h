#ifndef __MU_TASKING_H__
#define __MU_TASKING_H__

#pragma once

#define RUNTIME_CHECKTIME (60000.0f) // 1 Minute
#define RUNTIME_EXPIRETIME (5 * 60 * 1000) // 5 Minute

#define MAX_MODEL_INSTANCES 100000
#define MAX_EFFECTS_INSTANCES (MAX_BLURS * MAX_BLUR_TAILS + MAX_OBJECTBLURS * MAX_OBJECTBLUR_TAILS + MAX_JOINTS * MAX_TAILS + MAX_PARTICLES + MAX_POINTERS + MAX_SPRITES + MAX_TILESPRITES * 5 + MAX_LEAVES + MAX_GUILDSPRITES)

#define MAX_CLOTH_QUADS 50000
#if 1
#define MAX_CLOTH_VERTICES (MAX_CLOTH_QUADS * 6) // Max Quads * 6 vertices
#else
#define MAX_CLOTH_VERTICES (MAX_CLOTH_QUADS * 4) // Max Quads * 4 vertices
#define MAX_CLOTH_INDEXES (64 * 64 * 6)
#endif

struct EEffectCulledData
{
	union
	{
		struct
		{
			mu_uint16 Type;
			mu_uint16 Index;
		};

		mu_uint32 Hash;
	};
};

struct EEffectCulledFastSort
{
	EINLINE constexpr mu_boolean operator()(const EEffectCulledData &a, const EEffectCulledData &b) const
	{
		return a.Hash < b.Hash;
	}
};

extern mu_uint32 g_EffectsInstancesCount;
extern mu_atomic_uint32_t g_EffectsCulledDataCount;
extern mu_uint32 g_EffectsCulledDataIndex;
extern std::array<EEffectCulledData, MAX_EFFECTS_INSTANCES> g_EffectsCulledData;

extern mu_int32 Time_Effect;
extern mu_int32 Change_weather;

extern mu_uint8 ActionRequest;
extern mu_int32 ActionRequestTarget;
extern mu_uint16 ActionRequestMagic;
extern mu_uint8 ActionRequestX;
extern mu_uint8 ActionRequestY;
extern mu_float ActionRequestDirection;

enum : mu_uint32
{
	TASKING_LOGIN,
	TASKING_LOGIN_RENDER,
	TASKING_CHARACTER,
	TASKING_CHARACTER_RENDER,
	TASKING_GAME,
	TASKING_GAME_RENDER,
	TASKING_CLEAN_COUNTERS,
};

#define FAILED_SKELETAL (mu_uint32)-1
//#define DIVIDE_TASKING

FORCEINLINE mu_int32 RenderTerrainBitmapCalculate(mu_float SizeX, mu_float SizeY)
{
	const mu_int32 Size = (mu_int32)EMath::Floor(EMath::Max(SizeX, SizeY) + 1.0f) * 2 + 1;

	return Size * Size;
}

class MUTasking
{
public:
	MUTasking();
	~MUTasking();

	mu_boolean Create();
	void Destroy();

	void ReleaseModelsMemory();

	void CreateInterface();
	void DestroyInterface();

	void RequestTakeScreenshot();
	void TakeScreenshot();

	void LoginProcessGFX(mu_uint64 CurrentTime);
	void LoginProcess(mu_float MillisecondsElapse, mu_uint64 CurrentTime);
	void CharacterProcessGFX(mu_uint64 CurrentTime);
	void CharacterProcess(mu_float MillisecondsElapse, mu_uint64 CurrentTime);
	void GameProcessGFX(mu_uint64 CurrentTime);
	void GameProcess(mu_float MillisecondsElapse, mu_uint64 CurrentTime);
	void TaskProcess(const mu_uint32 ThreadIndex);

	void RunGameProcess(const mu_uint32 ThreadIndex);
	void RunGameVisual(const mu_uint32 ThreadIndex);
	void RunCleanCountersProcess(const mu_uint32 ThreadIndex);

	void RenderTerrainVisual(const mu_uint32 ThreadIndex);

#ifdef USE_DUAL_QUATERNION
	mu_uint32 RegisterSkeletal(const EDualQuat *BoneMatrix, mu_uint32 BoneCount);
#else
	mu_uint32 RegisterSkeletal(const EMatrix4 *BoneMatrix, mu_uint32 BoneCount);
#endif

	void CleanCharacters();
	void CleanSounds();
	void CleanEffects();
	void CleanBoids();
	void CleanPets();
	void CleanLeaves();
	void CleanOperates();
	void CleanWorldObjects();
	void CleanProxy();

	void SettingBoids();

	void UpdateSounds(const mu_uint32 ThreadIndex);
	void ProcessBGM();

	void CheckObjects(const mu_uint32 ThreadIndex);
	void MoveObjects(const mu_uint32 ThreadIndex);
	void PrepareObjects(const mu_uint32 ThreadIndex);

	void MoveBoidsProcess(const mu_uint32 ThreadIndex);
	void MoveFishsProcess(const mu_uint32 ThreadIndex);
	void MoveBoids(const mu_uint32 ThreadIndex);
	void CheckBoids(const mu_uint32 ThreadIndex);
	void CheckFishs(const mu_uint32 ThreadIndex);
	void PrepareBoids(const mu_uint32 ThreadIndex);
	void PrepareFishs(const mu_uint32 ThreadIndex);

	void MoveBugs(const mu_uint32 ThreadIndex);
	void CheckBugs(const mu_uint32 ThreadIndex);
	void PrepareBugs(const mu_uint32 ThreadIndex);

	void MovePets(const mu_uint32 ThreadIndex);
	void CheckPets(const mu_uint32 ThreadIndex);
	void PreparePets(const mu_uint32 ThreadIndex);

	void MoveMonsterClient(const mu_uint32 ThreadIndex, CHARACTER *c, OBJECT *o);
	void MoveCharacter(const mu_uint32 ThreadIndex, CHARACTER *c, OBJECT *o);
	void MoveCharacterVisual(const mu_uint32 ThreadIndex, CHARACTER *c, OBJECT *o, mu_boolean Visible);
	void MoveCharacterView();
	void MoveHero();
	void MoveCharacters(const mu_uint32 ThreadIndex);
	void RenderCharacter(const mu_uint32 ThreadIndex, CHARACTER *c, OBJECT *o, mu_int32 Select = 0);
	void CheckCharacters(const mu_uint32 ThreadIndex);
	void PrepareCharacters(const mu_uint32 ThreadIndex);

	void MoveItems(const mu_uint32 ThreadIndex);
	void CheckItems(const mu_uint32 ThreadIndex);
	void PrepareItems(const mu_uint32 ThreadIndex);

	void TerrainCollision(const mu_uint32 ThreadIndex);

	void DeleteEffects();
	void DeleteParticles();
	void DeletePoints();
	void DeletePointers();
	void DeleteButterfles();
	void DeleteBlurs();
	void EraseEffects(const mu_uint32 ThreadIndex);

	void MoveEffect(const mu_uint32 ThreadIndex, OBJECT *o, mu_uint32 iIndex);
	void CheckMoveEffects(const mu_uint32 ThreadIndex);
	void MoveEffects(const mu_uint32 ThreadIndex);
	void CheckEffects(const mu_uint32 ThreadIndex);
	void PrepareEffects(const mu_uint32 ThreadIndex);

	void CheckMoveJoints(const mu_uint32 ThreadIndex);
	void MoveJoints(const mu_uint32 ThreadIndex);
	void CheckJoints(const mu_uint32 ThreadIndex);
	void PrepareJoints(const mu_uint32 ThreadIndex);

	void CheckMoveParticles(const mu_uint32 ThreadIndex);
	void MoveParticles(const mu_uint32 ThreadIndex);
	void CheckParticles(const mu_uint32 ThreadIndex);

	void MovePointers(const mu_uint32 ThreadIndex);
	void CheckPointers(const mu_uint32 ThreadIndex);

	void MoveLeaves(const mu_uint32 ThreadIndex);
	void CheckLeaves(const mu_uint32 ThreadIndex);

	void MoveBlurs(const mu_uint32 ThreadIndex);
	void CheckBlurs(const mu_uint32 ThreadIndex);

	void MovePoints(const mu_uint32 ThreadIndex);
	void RenderPoints();

	void PreConfigureRenderSlots(const mu_uint32 ThreadIndex);
	void PostConfigureRenderSlots(const mu_uint32 ThreadIndex);
	void ConfigureRenderProxy(const mu_uint32 ThreadIndex);
	template<const mu_boolean useBaseInstance, const EVideoBackendType backendType>
	void RenderPreModels(const mu_boolean shadowPass = false);
	template<const mu_boolean useBaseInstance, const EVideoBackendType backendType>
	void RenderPreAlphaModels(const mu_boolean shadowPass = false);
	template<const mu_boolean useBaseInstance, const EVideoBackendType backendType>
	void RenderPostModels(const mu_boolean shadowPass = false);
	template<const mu_boolean useBaseInstance, const EVideoBackendType backendType>
	void RenderGUIModels(const mu_uint32 slotStart, const mu_uint32 slotCount);

	void RenderGUIModelsAtRuntime(const mu_uint32 slotStart, const mu_uint32 slotCount);

	template<const mu_boolean useBaseInstance, const EVideoBackendType backendType>
	void RenderShadowModels();
	void RenderShadowMapping();

	void PreConfigureTextures(const mu_uint32 ThreadIndex);
	void PostConfigureTextures(const mu_uint32 ThreadIndex, const mu_uint32 effectsCount);
	template<const mu_boolean useBaseInstance, const EVideoBackendType backendType>
	void RenderPreTextures();
	template<const mu_boolean useBaseInstance, const EVideoBackendType backendType>
	void RenderPostTextures();

	void RenderSprite(SHEffectInstance *Instance, mu_uint32 TextureId, EVector3 Position, mu_float Width, mu_float Height, EVector3 Light, mu_float Rotation = 0.0f, mu_float U = 0.0f, mu_float V = 0.0f, mu_float uWidth = 1.0f, mu_float vHeight = 1.0f);
	void RenderSpriteADD(SHEffectInstance *Instance, mu_uint32 TextureId, EVector3 Position, mu_float Width, mu_float Height, EVector3 Light, mu_float Rotation = 0.0f, mu_float U = 0.0f, mu_float V = 0.0f, mu_float uWidth = 1.0f, mu_float vHeight = 1.0f);
	void RenderPlane3D(SHEffectInstance *Instance, mu_uint32 TextureId, EVector3 Position, mu_float Width, mu_float Height, EVector3 Light, EVector3 Angle, mu_float U = 0.0f, mu_float V = 0.0f, mu_float uWidth = 1.0f, mu_float vHeight = 1.0f);
	void RenderGuildSprite(SHEffectInstance *Instance, MUGuildSprite *GuildSprite);
	void RenderTail(SHEffectInstance *Instance, const mu_uint32 TextureId, const EVector3 &LeftTop, const EVector3 &RightTop, const EVector3 &RightBottom, const EVector3 &LeftBottom, const EVector2 &UV, const mu_boolean UVType, const EVector4 &Light);
	void RenderBlur(SHEffectInstance *Instance, mu_uint32 TextureId, MUBlur *b, mu_uint32 c, mu_uint32 n, mu_uint32 j);
	void RenderObjectBlur(SHEffectInstance *Instance, mu_uint32 TextureId, MUObjectBlur *b, mu_uint32 c, mu_uint32 n, mu_uint32 j);

	void RenderTileSprite(SHEffectInstance *Instance, const mu_uint32 Texture, const mu_float xf, const mu_float yf, const mu_float SizeX, const mu_float SizeY, const mu_uint32 Light, const mu_float Rotation = 0.0f, const mu_float Height = 5.0f);

	mu_uint32 GetSkeletonCount()
	{
		return _SkeletonCount;
	}

	mu_uint32 GetThreadsCount()
	{
		return _ThreadsCount;
	}

public:
	void RenderDebugInformation(const mu_double elapsedTime, const mu_uint64 currentTime);

public:
	void GetIndexTasking(const mu_uint32 ThreadIndex, const mu_uint32 count, mu_uint32 &beginIndex, mu_uint32 &endIndex);

private:
	mu_uint32 _ThreadsCount;
	std::thread *_Threads;
	mu_atomic_bool _ThreadsDestroy;
	mu_uint32 _ThreadTasking;

public:
#ifdef USE_DUAL_QUATERNION
	EDualQuat *_SkeletonBuffer;
#else
	EMatrix4 *_SkeletonBuffer;
#endif
	mu_atomic_uint32_t _SkeletonCount;

private:
	MUThreadBarrier *_ThreadBarrier;

	mu_atomic_bool _RequestObjectsCulledIncrease;
	mu_atomic_bool _DisableObjectRender;
	mu_atomic_bool _DisableTextureRender;

	UIBase *_RootUI;

public:
	mu_boolean _RequestedScreenshot;

	mu_float _ElapsedTime;
	mu_double _ElapsedFrame;
	mu_float _MillisecondsElapse;
	mu_float _CheckRuntimeElapse;
	mu_uint32 _UpdateCounter;
	mu_uint32 _CheckRuntimeCounter;

	SHModelInstance *_ModelInstanceBuffer;
	SHEffectInstance *_EffectInstanceBuffer;
	SHClothVertexData *_DynamicClothBuffer;
	mu_atomic_uint32_t _EffectInstanceCount;
	mu_atomic_uint32_t _ModelInstanceCount;
	mu_atomic_uint32_t _DynamicVerticesCount;

public:
	EINLINE SHModelInstance *GetModelInstanceBuffer(const mu_uint32 index)
	{
		if (index >= MAX_MODEL_INSTANCES)
		{
			DEBUG_BREAK();
		}

		return &_ModelInstanceBuffer[index];
	}

	EINLINE SHEffectInstance *GetEffectInstanceBuffer(const mu_uint32 index)
	{
		if(index >= MAX_EFFECTS_INSTANCES)
		{
			DEBUG_BREAK();
		}

		return &_EffectInstanceBuffer[index];
	}
};

EINLINE void MUTasking::GetIndexTasking(const mu_uint32 ThreadIndex, const mu_uint32 count, mu_uint32 &beginIndex, mu_uint32 &endIndex)
{
	const mu_uint32 perThread = (count + _ThreadsCount) / _ThreadsCount;
	beginIndex = perThread * ThreadIndex;
	endIndex = perThread * (ThreadIndex + 1);
	if(endIndex > count) endIndex = count;
}

#ifdef USE_RENDERING_DISABLE_FLAGS
extern mu_boolean g_DisableTerrainRender;
extern mu_boolean g_DisableGrassRender;
extern mu_boolean g_DisablePreModelRender;
extern mu_boolean g_DisablePreAlphaModelRender;
extern mu_boolean g_DisablePostModelRender;
extern mu_boolean g_DisableGUIModelRender;
extern mu_boolean g_DisablePreTextureRender;
extern mu_boolean g_DisablePostTextureRender;
extern mu_boolean g_DisableSkeletonUpload;
#endif

extern MUTasking g_Tasking;
extern mu_uint64 g_CurrentTime;
extern mu_uint64 g_LiveCheckTime;
extern mu_uint64 g_CreationTime;
extern mu_uint64 g_CaretTime;
extern mu_uint64 g_CaretBlinkTime;
extern mu_boolean g_CaretEnabled;
extern mu_float g_Luminosity;
extern mu_float g_Hellas01MeshLight;
extern mu_float g_Hellas01AddLight;

#define MU_UPDATECOUNTER g_Tasking._UpdateCounter
#define MU_ELAPSEDTIME g_Tasking._ElapsedTime
#define MU_FRAMEELAPSED g_Tasking._ElapsedFrame

void MoveParticle(OBJECT *o, mu_boolean Turn);
void MoveParticle(OBJECT *o, EVector3 angle);

#endif