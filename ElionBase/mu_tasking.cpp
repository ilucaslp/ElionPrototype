#include "stdafx.h"
#include "mu_tasking.h"

mu_uint32 g_EffectsInstancesCount = 0;
mu_atomic_uint32_t g_EffectsCulledDataCount(0);
mu_uint32 g_EffectsCulledDataIndex = 0;
std::array<EEffectCulledData, MAX_EFFECTS_INSTANCES> g_EffectsCulledData;

#ifdef USE_RENDERING_DISABLE_FLAGS
mu_boolean g_DisableTerrainRender = false;
mu_boolean g_DisableGrassRender = false;
mu_boolean g_DisablePreModelRender = false;
mu_boolean g_DisablePreAlphaModelRender = false;
mu_boolean g_DisablePostModelRender = false;
mu_boolean g_DisableGUIModelRender = false;
mu_boolean g_DisablePreTextureRender = false;
mu_boolean g_DisablePostTextureRender = false;
mu_boolean g_DisableSkeletonUpload = false;
#endif

MUTasking g_Tasking;
mu_uint64 g_CurrentTime = 0;
mu_uint64 g_LiveCheckTime = 0;
mu_uint64 g_CreationTime = 0;
mu_uint64 g_CaretTime = 0;
mu_uint64 g_CaretBlinkTime = 530;
mu_boolean g_CaretEnabled = true;
mu_float g_Luminosity;
mu_float g_Hellas01MeshLight = 0.500f;
mu_float g_Hellas01AddLight = 0.01f;

MUTasking::MUTasking()
{
	_ThreadsCount = 0;
	_Threads = nullptr;
	_ThreadsDestroy = false;
	_ThreadBarrier = nullptr;

	_SkeletonBuffer = nullptr;

	_RootUI = nullptr;

	_RequestedScreenshot = false;

	_ElapsedTime = 0.0f;
	_ElapsedFrame = 0.0;
	_MillisecondsElapse = 0.0f;
	_CheckRuntimeElapse = 0.0f;
	_UpdateCounter = 0;
	_CheckRuntimeCounter = 0;
}

MUTasking::~MUTasking()
{
}

mu_boolean MUTasking::Create()
{
	_ThreadsCount = EMath::Min(g_DeviceCoreCount, (mu_uint32)MAX_TASKING_THREADS);

	if (MU_RESOURCE->GetMaxThreadsCount() > 0 &&
		_ThreadsCount > MU_RESOURCE->GetMaxThreadsCount())
	{
		_ThreadsCount = MU_RESOURCE->GetMaxThreadsCount();
	}

	if (_ThreadsCount > MAX_TASKING_THREADS)
	{
		_ThreadsCount = MAX_TASKING_THREADS;
	}

	// Initialize Threads Queues (with + _ThreadsCount we don't need to round the value)
	const mu_uint32 objectsCount = (MAX_OBJECTS + _ThreadsCount) / _ThreadsCount;
	const mu_uint32 charactersCount = (MAX_CHARACTERS_CLIENT + _ThreadsCount) / _ThreadsCount;
	const mu_uint32 petsCount = (MAX_PETS + _ThreadsCount) / _ThreadsCount;
	const mu_uint32 boidsCount = (MAX_BOIDS + _ThreadsCount) / _ThreadsCount;
	const mu_uint32 fishsCount = (MAX_FISHS + _ThreadsCount) / _ThreadsCount;
	const mu_uint32 itemsCount = (MAX_ITEMS + _ThreadsCount) / _ThreadsCount;
	const mu_uint32 butterflesCount = (MAX_BUTTERFLES + _ThreadsCount) / _ThreadsCount;
	const mu_uint32 effectsCount = (MAX_EFFECTS + _ThreadsCount) / _ThreadsCount;
	const mu_uint32 jointsCount = (MAX_JOINTS + _ThreadsCount) / _ThreadsCount;
	const mu_uint32 blursCount = (MAX_BLURS + _ThreadsCount) / _ThreadsCount;
	const mu_uint32 objectBlursCount = (MAX_OBJECTBLURS + _ThreadsCount) / _ThreadsCount;
	const mu_uint32 particlesCount = (MAX_PARTICLES + _ThreadsCount) / _ThreadsCount;
	const mu_uint32 pointersCount = (MAX_POINTERS + _ThreadsCount) / _ThreadsCount;
	const mu_uint32 collisionsCount = (MAX_TERRAIN_SIZE + _ThreadsCount) / _ThreadsCount;

	for (mu_uint32 n = 0; n < _ThreadsCount; ++n)
	{
		MUThreadQueues::EnvironmentObjectsQueueIndex[n] = objectsCount * n;
		MUThreadQueues::CharactersQueueIndex[n] = charactersCount * n;
		MUThreadQueues::PetObjectsQueueIndex[n] = petsCount * n;
		MUThreadQueues::BoidsQueueIndex[n] = boidsCount * n;
		MUThreadQueues::FishsQueueIndex[n] = fishsCount * n;
		MUThreadQueues::ItemsQueueIndex[n] = itemsCount * n;
		MUThreadQueues::ButterflesQueueIndex[n] = butterflesCount * n;
		MUThreadQueues::EffectsQueueIndex[n] = effectsCount * n;
		MUThreadQueues::JointsQueueIndex[n] = jointsCount * n;
		MUThreadQueues::BlursQueueIndex[n] = blursCount * n;
		MUThreadQueues::ObjectBlursQueueIndex[n] = objectBlursCount * n;
		MUThreadQueues::ParticlesQueueIndex[n] = particlesCount * n;
		MUThreadQueues::PointersQueueIndex[n] = pointersCount * n;
		MUThreadQueues::TerrainCollisionQueueIndex[n] = collisionsCount * n;
	}

	g_ThreadInfo = new MUThreadInfo[_ThreadsCount];
	_ThreadBarrier = new MUThreadBarrier(_ThreadsCount);

	if (_ThreadsCount > 1)
	{
		_Threads = new std::thread[_ThreadsCount - 1];

		for (mu_uint32 index = 0; index < _ThreadsCount - 1; ++index)
		{
			_Threads[index] = std::thread(&MUTasking::TaskProcess, this, index + 1);
		}
	}

	return true;
}

void MUTasking::Destroy()
{
	_ThreadsDestroy = true;

	if (_ThreadBarrier != nullptr)
	{
		if (_ThreadsCount > 0)
		{
			if (_Threads != nullptr)
			{
				_ThreadBarrier->SyncSemaphore();

				for (mu_uint32 index = 0; index < _ThreadsCount - 1; ++index)
				{
					_Threads[index].join();
				}

				MU_SAFE_DELETE_ARRAY(_Threads);
			}
		}

		MU_SAFE_DELETE(_ThreadBarrier);
	}

	DestroyInterface();
	PetProcess::Destroy();

	MU_SAFE_DELETE_ARRAY(g_ThreadInfo);
}

void MUTasking::ReleaseModelsMemory()
{
	for (mu_uint32 n = MODEL_MONSTER01; n < MODEL_MONSTER_END; ++n)
	{
		MODELS::Data[n].ReleaseData();
	}

	for (mu_uint32 n = MODEL_NPC_BEGIN; n < MODEL_NPC_END; ++n)
	{
		MODELS::Data[n].ReleaseData();
	}
}

#include "mu_screengrab.h"
#if DIRECTX_BACKEND_ENABLE == 1
#include <wincodec.h>
#endif

void MUTasking::RequestTakeScreenshot()
{
	_RequestedScreenshot = true;
}

void MUTasking::TakeScreenshot()
{
	if (_RequestedScreenshot == true)
	{
		MU::MakeDirectory(_T("./Screenshots/"));

		std::time_t rawtime;
		std::tm* timeinfo;
		std::time(&rawtime);
		timeinfo = std::localtime(&rawtime);

		mu_text textBuffer[2048];
		mu_sprintf_s(textBuffer, mu_countof(textBuffer), _T("./Screenshots/%02d-%02d_%02d-%02d-%02d.jpg"), timeinfo->tm_mon + 1, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

		if (EVideoBackend::TakeScreenshot(textBuffer) == true)
		{
			mu_text textBuffer2[2048];
			mu_sprintf_s(textBuffer2, mu_countof(textBuffer2), GlobalText[459], textBuffer);

			mu_text strServerName[MAX_TEXT_LENGTH];
			const mu_text *sFormat;

			switch (g_pServerList->GetSelectedServerType())
			{
			default:
			case CS2CL::SERVER_TYPE::ST_NORMAL:
				{
					if ((g_pServerList->GetSelectedServerFlag() & CS2CL::SERVER_FLAG::SF_NONPVP) != 0)
					{
						sFormat = GlobalText[460];
					}
					else
					{
						sFormat = GlobalText[461];
					}
				}
				break;

			case CS2CL::SERVER_TYPE::ST_GOLD:
				{
					if ((g_pServerList->GetSelectedServerFlag() & CS2CL::SERVER_FLAG::SF_NONPVP) != 0)
					{
						sFormat = GlobalText[3131];
					}
					else
					{
						sFormat = GlobalText[3130];
					}
				}
				break;
			}

			mu_sprintf_s(strServerName, mu_countof(strServerName), sFormat, g_pServerList->GetSelectedGroupName().c_str(), g_pServerList->GetSelectedServerIndex() + 1);

			mu_strcat_s(textBuffer2, mu_countof(textBuffer2), _T(" "));
			mu_strcat_s(textBuffer2, mu_countof(textBuffer2), strServerName);

			switch (g_CurrentScene)
			{
			case LOG_IN_SCENE:
				g_pLoginRoot->_ServerMessage->AddText(textBuffer2);
				break;
			case CHARACTER_SCENE:
				g_pCharacterRoot->_ServerMessage->AddText(textBuffer2);
				break;
			case MAIN_SCENE:
				g_pChatListBox->AddText(_T(""), textBuffer2, TYPE_SYSTEM_MESSAGE);
				break;
			}
		}

		_RequestedScreenshot = false;
	}
}

void MUTasking::LoginProcessGFX(mu_uint64 CurrentTime)
{
	_RootUI = g_pLoginRoot;

	// Update Process
	g_pLoginRoot->Update(CurrentTime);
	g_pOptionRoot->Update(CurrentTime);
	g_pMessageRoot->Update(CurrentTime);
	g_pLoginRoot->UpdateGFX();
	g_pOptionRoot->UpdateGFX();
	g_pMessageRoot->UpdateGFX();
}

void MUTasking::LoginProcess(mu_float MillisecondsElapse, mu_uint64 CurrentTime)
{
	MU_RESOURCE->UpdateWorldTime(CurrentTime);

	TakeScreenshot();

	_ElapsedTime = MillisecondsElapse;
	_ElapsedFrame = (mu_double)_ElapsedTime / (mu_double)GAME_FPS;
	_MillisecondsElapse += MillisecondsElapse;
	_UpdateCounter = _MillisecondsElapse / GAME_FPS;
	_MillisecondsElapse -= _UpdateCounter * GAME_FPS;
	_CheckRuntimeElapse += MillisecondsElapse;
	_CheckRuntimeCounter = _CheckRuntimeElapse / RUNTIME_CHECKTIME;
	_CheckRuntimeElapse -= _CheckRuntimeCounter * RUNTIME_CHECKTIME;
	g_CurrentTime = CurrentTime;

	mu_uint64 diffTime = g_CurrentTime - g_CaretTime;

	if (diffTime >= g_CaretBlinkTime)
	{
		mu_boolean caretChange = (diffTime / g_CaretBlinkTime) % 2;

		g_CaretTime = g_CurrentTime;

		if (caretChange)
		{
			g_CaretEnabled = !g_CaretEnabled;
		}
	}

	mu_boolean isLightUpload = false;

	if (_UpdateCounter >= 1)
	{
		g_SpritesCount.store(0, std::memory_order_relaxed);
		g_TileSpritesCount.store(0, std::memory_order_relaxed);
		g_GuildSpritesCount.store(0, std::memory_order_relaxed);
		g_TerrainModificationCount.store(0, std::memory_order_relaxed);
		isLightUpload = true;
		g_LightEnabled = true;

		g_Terrain.RestoreLight();
	}

	ZeroArray(g_CulledBlockVisible);

	g_EffectsCulledCount = 0;
	g_EffectsInstancesCount = 0;
	g_EffectsCulledDataCount.store(0, std::memory_order_relaxed);
	g_PreRenderSlotCount.store(0, std::memory_order_relaxed);
	g_PostRenderSlotCount.store(0, std::memory_order_relaxed);
	g_ClothRenderSlotCount.store(0, std::memory_order_relaxed);
	g_GUIRenderSlotCount = 0;
	g_EffectsRenderCount.store(0, std::memory_order_relaxed);
	g_TexturesCulledCount = 0;
	g_PreTexturesCulledCount = 0;
	g_PostTexturesCulledCount = 0;

	g_JointsRenderCount = 0;

	_SkeletonCount.store(0, std::memory_order_relaxed);
	_ModelInstanceCount.store(0, std::memory_order_relaxed);
	_EffectInstanceCount.store(0, std::memory_order_relaxed);
	_DynamicVerticesCount.store(0, std::memory_order_relaxed);

	_DisableObjectRender = false;
	_DisableTextureRender = false;

#ifndef DISABLE_AUDIO
	EVector3 &audioPosition = MU_CAMERA->GetCameraPosition();
	MU_AUDIO->_Listener.Position.x = audioPosition.x * g_TerrainScaleRev;
	MU_AUDIO->_Listener.Position.y = 0.0f;
	MU_AUDIO->_Listener.Position.z = audioPosition.y * g_TerrainScaleRev;
#endif

	CollisionDistance = FLT_MAX;

	mu_float mouseX = MU_INPUT->GetMouseX();
	mu_float mouseY = MU_VIDEO->_3DViewport.Height - (MU_INPUT->GetMouseY() - MU_VIDEO->_3DViewport.OffsetY);

	MousePosition = EMath::unProject(EVector3(mouseX, mouseY, 0.0f), MU_VIDEO->_CurrentView, MU_VIDEO->_CurrentRayProjection, EDVector4(MU_VIDEO->_3DViewport.OffsetX, MU_VIDEO->_3DViewport.OffsetY, MU_VIDEO->_3DViewport.Width, MU_VIDEO->_3DViewport.Height));
	MouseTarget = EMath::unProject(EVector3(mouseX, mouseY, 1.0f), MU_VIDEO->_CurrentView, MU_VIDEO->_CurrentRayProjection, EDVector4(MU_VIDEO->_3DViewport.OffsetX, MU_VIDEO->_3DViewport.OffsetY, MU_VIDEO->_3DViewport.Width, MU_VIDEO->_3DViewport.Height));
	MouseMin = EMath::Min(MousePosition, MouseTarget);
	MouseMax = EMath::Max(MousePosition, MouseTarget);
	VectorSubtract(MouseTarget, MousePosition, MouseDirection);
	MouseAbsDirection = EMath::Abs(MouseDirection);

	MouseAbsMin = MouseAbsDirection[0];
	MouseAbsMinIndex = 0;

	if (MouseAbsDirection[1] > MouseAbsDirection[2])
	{
		if (MouseAbsMin > MouseAbsDirection[2])
		{
			MouseAbsMin = MouseAbsDirection[2];
			MouseAbsMinIndex = 2;
		}
	}
	else
	{
		if (MouseAbsMin > MouseAbsDirection[1])
		{
			MouseAbsMin = MouseAbsDirection[1];
			MouseAbsMinIndex = 1;
		}
	}

	if (EVideoBackend::GetVideoStatus(EVideoStatusFlag::VSF_Available) == false)
	{
		_SkeletonBuffer = nullptr;
		_ModelInstanceBuffer = nullptr;
		_EffectInstanceBuffer = nullptr;
		_DynamicClothBuffer = nullptr;
	}
	else
	{
		_SkeletonBuffer = EVideoBuffer::GetBonesBuffer();
		_ModelInstanceBuffer = EVideoBuffer::GetModelInstancesBuffer();
		_EffectInstanceBuffer = EVideoBuffer::GetEffectInstancesBuffer();
		_DynamicClothBuffer = EVideoBuffer::GetClothVerticesBuffer();
	}

	_ThreadTasking = TASKING_LOGIN;
	_ThreadBarrier->SyncSemaphore();

	RunGameProcess(0);

	MUVideoManager::OnUpdateBuffer();

	// Moved Code from RunGameVisual
	{
		if (_UpdateCounter > 0)
		{
			RenderChaosCastleVisual(0);
		}

		if (_UpdateCounter > 0)
		{
			if (Time_Effect > 40)
				Time_Effect = 0;
			++Time_Effect;

			if (GET_WORLD == WD_34CRYWOLF_1ST)
			{
				if (++Change_weather > 700)
				{
					weather = GetLargeRand(3);
					Change_weather = 0;
				}
			}
		}
	}

	if (EVideoBackend::GetVideoStatus(EVideoStatusFlag::VSF_Visible | EVideoStatusFlag::VSF_Available) == true)
	{
		_ThreadTasking = TASKING_LOGIN_RENDER;
		_ThreadBarrier->SyncSemaphore();

		RunGameVisual(0);
	}

	if (MU_UPDATECOUNTER > 0 &&
		g_pCSWaterTerrain != nullptr)
	{
		g_pCSWaterTerrain->Update();
	}

	EVideoBackend::UpdateBuffers(isLightUpload);
	EVideoBackend::EmptyTrash();

	if (EVideoBackend::GetVideoStatus(EVideoStatusFlag::VSF_Visible | EVideoStatusFlag::VSF_Available) == true)
	{
		if (MU_RESOURCE->GetShadowType() >= EShadowType::ePCF)
		{
			EVideoBackend::BeginShadowRenderPass();
			EVideoBackend::BindGlobalConstant(EGlobalConstant::eShadow);
			RenderShadowMapping();
			EVideoBackend::EndShadowRenderPass();
		}

		EVideoBackend::BeginRenderPass(g_Terrain._ClearColor);

		EVideoBackend::SetViewport(MUVideoManager::_3DViewport.OffsetX,
			MUVideoManager::_3DViewport.OffsetY,
			MUVideoManager::_3DViewport.Width,
			MUVideoManager::_3DViewport.Height);
		EVideoBackend::CleanScissor();

		EVideoBackend::BindGlobalConstant(EGlobalConstant::eGame);

		switch (EVideoBackend::GetBackend())
		{
#if DIRECTX_BACKEND_ENABLE == 1
		case EVideoBackendType::eDirectX:
			{
				// Terrain rendering
				g_Terrain.Render<false, EVideoBackendType::eDirectX>();

				// Pre-Models rendering
				if (MU_RESOURCE->GetShadowType() <= EShadowType::eOriginalPlus &&
					MU_RESOURCE->GetShadowType() > EShadowType::eNone)
				{
					RenderShadowModels<false, EVideoBackendType::eDirectX>();
				}

				RenderPreModels<false, EVideoBackendType::eDirectX>();

				// Pre-Effects rendering
				RenderPreTextures<false, EVideoBackendType::eDirectX>();

				// Pre-Models rendering with Alpha
				RenderPreAlphaModels<false, EVideoBackendType::eDirectX>();

				// Terrain post rendering
				g_Terrain.PostRender<false, EVideoBackendType::eDirectX>();

				if (g_pCSWaterTerrain != nullptr)
				{
					g_pCSWaterTerrain->Render();
				}

				// Post-Models rendering
				RenderPostModels<false, EVideoBackendType::eDirectX>();

				// Post-Effects rendering
				RenderPostTextures<false, EVideoBackendType::eDirectX>();

				RenderPoints();
			}
			break;
#endif

#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
			{
				if (EOpenGLExtensions::IsFeatureSupported(OpenGLFeatures::BaseInstance) == true)
				{
					// Terrain rendering
					g_Terrain.Render<true, EVideoBackendType::eOpenGL>();

					// Pre-Models rendering
					if (MU_RESOURCE->GetShadowType() <= EShadowType::eOriginalPlus &&
						MU_RESOURCE->GetShadowType() > EShadowType::eNone)
					{
						RenderShadowModels<true, EVideoBackendType::eOpenGL>();
					}

					RenderPreModels<true, EVideoBackendType::eOpenGL>();

					// Pre-Effects rendering
					RenderPreTextures<true, EVideoBackendType::eOpenGL>();

					// Pre-Models rendering with Alpha
					RenderPreAlphaModels<true, EVideoBackendType::eOpenGL>();

					// Terrain post rendering
					g_Terrain.PostRender<true, EVideoBackendType::eOpenGL>();

					if (g_pCSWaterTerrain != nullptr)
					{
						g_pCSWaterTerrain->Render();
					}

					// Post-Models rendering
					RenderPostModels<true, EVideoBackendType::eOpenGL>();

					// Post-Effects rendering
					RenderPostTextures<true, EVideoBackendType::eOpenGL>();

					RenderPoints();
				}
				else
				{
					// Terrain rendering
					g_Terrain.Render<false, EVideoBackendType::eOpenGL>();

					// Pre-Models rendering
					if (MU_RESOURCE->GetShadowType() <= EShadowType::eOriginalPlus &&
						MU_RESOURCE->GetShadowType() > EShadowType::eNone)
					{
						RenderShadowModels<false, EVideoBackendType::eOpenGL>();
					}

					RenderPreModels<false, EVideoBackendType::eOpenGL>();

					// Pre-Effects rendering
					RenderPreTextures<false, EVideoBackendType::eOpenGL>();

					// Pre-Models rendering with Alpha
					RenderPreAlphaModels<false, EVideoBackendType::eOpenGL>();

					// Terrain post rendering
					g_Terrain.PostRender<false, EVideoBackendType::eOpenGL>();

					if (g_pCSWaterTerrain != nullptr)
					{
						g_pCSWaterTerrain->Render();
					}

					// Post-Models rendering
					RenderPostModels<false, EVideoBackendType::eOpenGL>();

					// Post-Effects rendering
					RenderPostTextures<false, EVideoBackendType::eOpenGL>();

					RenderPoints();
				}
			}
			break;
#endif

#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
			{
				// Terrain rendering
				g_Terrain.Render<false, EVideoBackendType::eVulkan>();

				// Pre-Models rendering
				if (MU_RESOURCE->GetShadowType() <= EShadowType::eOriginalPlus &&
					MU_RESOURCE->GetShadowType() > EShadowType::eNone)
				{
					RenderShadowModels<false, EVideoBackendType::eVulkan>();
				}

				RenderPreModels<false, EVideoBackendType::eVulkan>();

				// Pre-Effects rendering
				RenderPreTextures<false, EVideoBackendType::eVulkan>();

				// Pre-Models rendering with Alpha
				RenderPreAlphaModels<false, EVideoBackendType::eVulkan>();

				// Terrain post rendering
				g_Terrain.PostRender<false, EVideoBackendType::eVulkan>();

				if (g_pCSWaterTerrain != nullptr)
				{
					g_pCSWaterTerrain->Render();
				}

				// Post-Models rendering
				RenderPostModels<false, EVideoBackendType::eVulkan>();

				// Post-Effects rendering
				RenderPostTextures<false, EVideoBackendType::eVulkan>();

				RenderPoints();
			}
			break;
#endif

		default: break;
		}

		//EVideoBackend::ClearDepthAndStencil();
		EVideoBackend::SetViewport(MUVideoManager::_2DViewport.OffsetY,
			MUVideoManager::_2DViewport.OffsetX,
			MUVideoManager::_2DViewport.Width,
			MUVideoManager::_2DViewport.Height);

		EVideoBackend::BindGlobalConstant(EGlobalConstant::eUI);

		g_pLoginRoot->Draw();
		g_pLoginRoot->PostDraw();
		g_pOptionRoot->Draw();
		g_pOptionRoot->PostDraw();
		g_pPickedItem.Draw();
		g_pMessageRoot->Draw();
		g_pMessageRoot->PostDraw();

		EVideoBackend::RenderMouse(MillisecondsElapse, CurrentTime);

		EVideoBackend::EndRenderPass();
		EVideoPipeline::UnbindVAO();
	}

	_ThreadTasking = TASKING_CLEAN_COUNTERS;
	_ThreadBarrier->SyncSemaphore();

	RunCleanCountersProcess(0);
}

void MUTasking::CharacterProcessGFX(mu_uint64 CurrentTime)
{
	_RootUI = g_pCharacterRoot;

	// Update Process
	g_pCharacterRoot->Update(CurrentTime);
	g_pOptionRoot->Update(CurrentTime);
	g_pMessageRoot->Update(CurrentTime);
	g_pCharacterRoot->UpdateGFX();
	g_pOptionRoot->UpdateGFX();
	g_pMessageRoot->UpdateGFX();
}

void MUTasking::CharacterProcess(mu_float MillisecondsElapse, mu_uint64 CurrentTime)
{
	MU_RESOURCE->UpdateWorldTime(CurrentTime);

	TakeScreenshot();

	_ElapsedTime = MillisecondsElapse;
	_ElapsedFrame = (mu_double)_ElapsedTime / (mu_double)GAME_FPS;
	_MillisecondsElapse += MillisecondsElapse;
	_UpdateCounter = _MillisecondsElapse / GAME_FPS;
	_MillisecondsElapse -= _UpdateCounter * GAME_FPS;
	_CheckRuntimeElapse += MillisecondsElapse;
	_CheckRuntimeCounter = _CheckRuntimeElapse / RUNTIME_CHECKTIME;
	_CheckRuntimeElapse -= _CheckRuntimeCounter * RUNTIME_CHECKTIME;
	g_CurrentTime = CurrentTime;

	mu_uint64 diffTime = g_CurrentTime - g_CaretTime;

	if (diffTime >= g_CaretBlinkTime)
	{
		mu_boolean caretChange = (diffTime / g_CaretBlinkTime) % 2;

		g_CaretTime = g_CurrentTime;

		if (caretChange)
		{
			g_CaretEnabled = !g_CaretEnabled;
		}
	}

	mu_boolean isLightUpload = false;

	if (_UpdateCounter >= 1)
	{
		g_SpritesCount.store(0, std::memory_order_relaxed);
		g_TileSpritesCount.store(0, std::memory_order_relaxed);
		g_GuildSpritesCount.store(0, std::memory_order_relaxed);
		g_TerrainModificationCount.store(0, std::memory_order_relaxed);
		isLightUpload = true;
		g_LightEnabled = true;

		g_Terrain.RestoreLight();
	}

	ZeroArray(g_CulledBlockVisible);

	g_EffectsCulledCount = 0;
	g_EffectsInstancesCount = 0;
	g_EffectsCulledDataCount.store(0, std::memory_order_relaxed);
	g_PreRenderSlotCount.store(0, std::memory_order_relaxed);
	g_PostRenderSlotCount.store(0, std::memory_order_relaxed);
	g_ClothRenderSlotCount.store(0, std::memory_order_relaxed);
	g_GUIRenderSlotCount = 0;
	g_EffectsRenderCount.store(0, std::memory_order_relaxed);
	g_TexturesCulledCount = 0;
	g_PreTexturesCulledCount = 0;
	g_PostTexturesCulledCount = 0;

	g_JointsRenderCount = 0;

	_SkeletonCount.store(0, std::memory_order_relaxed);
	_ModelInstanceCount.store(0, std::memory_order_relaxed);
	_EffectInstanceCount.store(0, std::memory_order_relaxed);
	_DynamicVerticesCount.store(0, std::memory_order_relaxed);

	_DisableObjectRender = false;
	_DisableTextureRender = false;

#ifndef DISABLE_AUDIO
	EVector3 &audioPosition = MU_CAMERA->GetCameraPosition();
	MU_AUDIO->_Listener.Position.x = audioPosition.x * g_TerrainScaleRev;
	MU_AUDIO->_Listener.Position.y = 0.0f;
	MU_AUDIO->_Listener.Position.z = audioPosition.y * g_TerrainScaleRev;
#endif

	CollisionDistance = FLT_MAX;

	mu_float mouseX = MU_INPUT->GetMouseX();
	mu_float mouseY = MU_VIDEO->_3DViewport.Height - (MU_INPUT->GetMouseY() - MU_VIDEO->_3DViewport.OffsetY);

	MousePosition = EMath::unProject(EVector3(mouseX, mouseY, 0.0f), MU_VIDEO->_CurrentView, MU_VIDEO->_CurrentRayProjection, EDVector4(MU_VIDEO->_3DViewport.OffsetX, MU_VIDEO->_3DViewport.OffsetY, MU_VIDEO->_3DViewport.Width, MU_VIDEO->_3DViewport.Height));
	MouseTarget = EMath::unProject(EVector3(mouseX, mouseY, 1.0f), MU_VIDEO->_CurrentView, MU_VIDEO->_CurrentRayProjection, EDVector4(MU_VIDEO->_3DViewport.OffsetX, MU_VIDEO->_3DViewport.OffsetY, MU_VIDEO->_3DViewport.Width, MU_VIDEO->_3DViewport.Height));
	MouseMin = EMath::Min(MousePosition, MouseTarget);
	MouseMax = EMath::Max(MousePosition, MouseTarget);
	VectorSubtract(MouseTarget, MousePosition, MouseDirection);
	MouseAbsDirection = EMath::Abs(MouseDirection);

	MouseAbsMin = MouseAbsDirection[0];
	MouseAbsMinIndex = 0;

	if (MouseAbsDirection[1] > MouseAbsDirection[2])
	{
		if (MouseAbsMin > MouseAbsDirection[2])
		{
			MouseAbsMin = MouseAbsDirection[2];
			MouseAbsMinIndex = 2;
		}
	}
	else
	{
		if (MouseAbsMin > MouseAbsDirection[1])
		{
			MouseAbsMin = MouseAbsDirection[1];
			MouseAbsMinIndex = 1;
		}
	}

	if (EVideoBackend::GetVideoStatus(EVideoStatusFlag::VSF_Available) == false)
	{
		_SkeletonBuffer = nullptr;
		_ModelInstanceBuffer = nullptr;
		_EffectInstanceBuffer = nullptr;
		_DynamicClothBuffer = nullptr;
	}
	else
	{
		_SkeletonBuffer = EVideoBuffer::GetBonesBuffer();
		_ModelInstanceBuffer = EVideoBuffer::GetModelInstancesBuffer();
		_EffectInstanceBuffer = EVideoBuffer::GetEffectInstancesBuffer();
		_DynamicClothBuffer = EVideoBuffer::GetClothVerticesBuffer();
	}

	_ThreadTasking = TASKING_CHARACTER;
	_ThreadBarrier->SyncSemaphore();

	RunGameProcess(0);

	MUVideoManager::OnUpdateBuffer();

	CHARACTERSCENE::SelectCharacter();

	if (MU_INPUT->Is3DMousePressed(MOUSE_BUTTON_LEFT) == true)
	{
		MU_INPUT->SetMouseUsed(MOUSE_BUTTON_LEFT, MOUSE_USED_ALL);

		if (SelectedCharacter >= BEGIN_CHARACTERLIST_COUNT + 0 &&
			SelectedCharacter <= BEGIN_CHARACTERLIST_COUNT + MAX_CHARACTERLIST_COUNT - 1)
		{
			SelectedHero = SelectedCharacter;

			g_pCharacterFrame->_ConnectButton->ChangeState(UIButton::UI_NORMAL);
			g_pCharacterFrame->_DeleteButton->ChangeState(UIButton::UI_NORMAL);
		}
	}
	else if (MU_INPUT->Is3DMouseDoublePressed(MOUSE_BUTTON_LEFT) == true)
	{
		MU_INPUT->SetMouseUsed(MOUSE_BUTTON_LEFT, MOUSE_USED_ALL);

		if (SelectedCharacter >= BEGIN_CHARACTERLIST_COUNT + 0 &&
			SelectedCharacter <= BEGIN_CHARACTERLIST_COUNT + MAX_CHARACTERLIST_COUNT - 1)
		{
			SelectedHero = SelectedCharacter;

			g_pCharacterFrame->_ConnectButton->ChangeState(UIButton::UI_NORMAL);
			g_pCharacterFrame->_DeleteButton->ChangeState(UIButton::UI_NORMAL);

			g_pCharacterRoot->OnGFxMessagePush(g_pCharacterFrame->_ConnectButton->_ObjectID, GFX_MESSAGE_TYPE::GFX_PRESSED, 0);
		}
	}

	for (mu_int32 i = 0; i < 5; ++i)
	{
		CHARACTER *pCha = &CharactersClient[i];
		OBJECT *pObj = &pCha->Object;

		Vector(0.0f, 0.0f, 0.0f, CharactersClient[i].Object.Light);

		if (pCha->Helper.Type == MODEL_HELPER + 3)
		{
			pObj->Position[2] = 194.5f;
		}
		else
		{
			pObj->Position[2] = 169.5f;
		}
	}

	if (MU_UPDATECOUNTER > 0)
	{
		if (SelectedHero != -1)
		{
			OBJECT *o = &CharactersClient[SelectedHero].Object;

			if(o->Flags.Get(OBJECT::eOF_LIVE) == true)
			{
				EVector3 Light;
				Vector(1.0f, 1.0f, 1.0f, Light);
				Vector(1.0f, 1.0f, 1.0f, o->Light);
				AddTerrainLight(o->Position[0], o->Position[1], Light, 1, PrimaryTerrainLight);

				EVector3 vLight;

				Vector(1.0f, 1.0f, 1.0f, vLight);
				mu_float fLumi = EMath::Sin(FWorldTime*0.0015f)*0.3f + 0.5f;
				Vector(fLumi*vLight[0], fLumi*vLight[1], fLumi*vLight[2], vLight);

				RenderTerrainAlphaBitmap(BITMAP_GM_AURORA, BLEND::TXB_BLEND01, o->Position[0], o->Position[1], 1.8f, 1.8f, vLight, FWorldTime*0.01f);
				RenderTerrainAlphaBitmap(BITMAP_GM_AURORA, BLEND::TXB_BLEND01, o->Position[0], o->Position[1], 1.2f, 1.2f, vLight, -FWorldTime * 0.01f);

				Vector(0.15f, 0.15f, 0.15f, o->Light);
				CreateParticle(0, BITMAP_EFFECT, o->Position, o->Angle, o->Light, 4);
				CreateParticle(0, BITMAP_EFFECT, o->Position, o->Angle, o->Light, 5);
			}
		}
	}

	// Moved Code from RunGameVisual
	{
		if (_UpdateCounter > 0)
		{
			RenderChaosCastleVisual(0);
		}

		if (_UpdateCounter > 0)
		{
			if (Time_Effect > 40)
				Time_Effect = 0;
			++Time_Effect;

			if (GET_WORLD == WD_34CRYWOLF_1ST)
			{
				if (++Change_weather > 700)
				{
					weather = GetLargeRand(3);
					Change_weather = 0;
				}
			}
		}
	}

	if (EVideoBackend::GetVideoStatus(EVideoStatusFlag::VSF_Visible | EVideoStatusFlag::VSF_Available) == true)
	{
		_ThreadTasking = TASKING_CHARACTER_RENDER;
		_ThreadBarrier->SyncSemaphore();

		RunGameVisual(0);
	}

	if (MU_UPDATECOUNTER > 0 &&
		g_pCSWaterTerrain != nullptr)
	{
		g_pCSWaterTerrain->Update();
	}

	EVideoBackend::UpdateBuffers(isLightUpload);
	EVideoBackend::EmptyTrash();

	if (EVideoBackend::GetVideoStatus(EVideoStatusFlag::VSF_Visible | EVideoStatusFlag::VSF_Available) == true)
	{
		if (MU_RESOURCE->GetShadowType() >= EShadowType::ePCF)
		{
			EVideoBackend::BeginShadowRenderPass();
			EVideoBackend::BindGlobalConstant(EGlobalConstant::eShadow);
			RenderShadowMapping();
			EVideoBackend::EndShadowRenderPass();
		}

		EVideoBackend::BeginRenderPass(g_Terrain._ClearColor);

		EVideoBackend::SetViewport(MUVideoManager::_3DViewport.OffsetX,
			MUVideoManager::_3DViewport.OffsetY,
			MUVideoManager::_3DViewport.Width,
			MUVideoManager::_3DViewport.Height);
		EVideoBackend::CleanScissor();

		EVideoBackend::BindGlobalConstant(EGlobalConstant::eGame);

		switch (EVideoBackend::GetBackend())
		{
#if DIRECTX_BACKEND_ENABLE == 1
		case EVideoBackendType::eDirectX:
			{
				// Terrain rendering
				g_Terrain.Render<false, EVideoBackendType::eDirectX>();

				// Pre-Models rendering
				if (MU_RESOURCE->GetShadowType() <= EShadowType::eOriginalPlus &&
					MU_RESOURCE->GetShadowType() > EShadowType::eNone)
				{
					RenderShadowModels<false, EVideoBackendType::eDirectX>();
				}

				RenderPreModels<false, EVideoBackendType::eDirectX>();

				// Pre-Effects rendering
				RenderPreTextures<false, EVideoBackendType::eDirectX>();

				// Pre-Models rendering with Alpha
				RenderPreAlphaModels<false, EVideoBackendType::eDirectX>();

				// Terrain post rendering
				g_Terrain.PostRender<false, EVideoBackendType::eDirectX>();

				if (g_pCSWaterTerrain != nullptr)
				{
					g_pCSWaterTerrain->Render();
				}

				// Post-Models rendering
				RenderPostModels<false, EVideoBackendType::eDirectX>();

				// Post-Effects rendering
				RenderPostTextures<false, EVideoBackendType::eDirectX>();

				RenderPoints();
			}
			break;
#endif

#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
			{
				if (EOpenGLExtensions::IsFeatureSupported(OpenGLFeatures::BaseInstance) == true)
				{
					// Terrain rendering
					g_Terrain.Render<true, EVideoBackendType::eOpenGL>();

					// Pre-Models rendering
					if (MU_RESOURCE->GetShadowType() <= EShadowType::eOriginalPlus &&
						MU_RESOURCE->GetShadowType() > EShadowType::eNone)
					{
						RenderShadowModels<true, EVideoBackendType::eOpenGL>();
					}

					RenderPreModels<true, EVideoBackendType::eOpenGL>();

					// Pre-Effects rendering
					RenderPreTextures<true, EVideoBackendType::eOpenGL>();

					// Pre-Models rendering with Alpha
					RenderPreAlphaModels<true, EVideoBackendType::eOpenGL>();

					// Terrain post rendering
					g_Terrain.PostRender<true, EVideoBackendType::eOpenGL>();

					if (g_pCSWaterTerrain != nullptr)
					{
						g_pCSWaterTerrain->Render();
					}

					// Post-Models rendering
					RenderPostModels<true, EVideoBackendType::eOpenGL>();

					// Post-Effects rendering
					RenderPostTextures<true, EVideoBackendType::eOpenGL>();

					RenderPoints();
				}
				else
				{
					// Terrain rendering
					g_Terrain.Render<false, EVideoBackendType::eOpenGL>();

					// Pre-Models rendering
					if (MU_RESOURCE->GetShadowType() <= EShadowType::eOriginalPlus &&
						MU_RESOURCE->GetShadowType() > EShadowType::eNone)
					{
						RenderShadowModels<false, EVideoBackendType::eOpenGL>();
					}

					RenderPreModels<false, EVideoBackendType::eOpenGL>();

					// Pre-Effects rendering
					RenderPreTextures<false, EVideoBackendType::eOpenGL>();

					// Pre-Models rendering with Alpha
					RenderPreAlphaModels<false, EVideoBackendType::eOpenGL>();

					// Terrain post rendering
					g_Terrain.PostRender<false, EVideoBackendType::eOpenGL>();

					if (g_pCSWaterTerrain != nullptr)
					{
						g_pCSWaterTerrain->Render();
					}

					// Post-Models rendering
					RenderPostModels<false, EVideoBackendType::eOpenGL>();

					// Post-Effects rendering
					RenderPostTextures<false, EVideoBackendType::eOpenGL>();

					RenderPoints();
				}
			}
			break;
#endif

#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
			{
				// Terrain rendering
				g_Terrain.Render<false, EVideoBackendType::eVulkan>();

				// Pre-Models rendering
				if (MU_RESOURCE->GetShadowType() <= EShadowType::eOriginalPlus &&
					MU_RESOURCE->GetShadowType() > EShadowType::eNone)
				{
					RenderShadowModels<false, EVideoBackendType::eVulkan>();
				}

				RenderPreModels<false, EVideoBackendType::eVulkan>();

				// Pre-Effects rendering
				RenderPreTextures<false, EVideoBackendType::eVulkan>();

				// Pre-Models rendering with Alpha
				RenderPreAlphaModels<false, EVideoBackendType::eVulkan>();

				// Terrain post rendering
				g_Terrain.PostRender<false, EVideoBackendType::eVulkan>();

				if (g_pCSWaterTerrain != nullptr)
				{
					g_pCSWaterTerrain->Render();
				}

				// Post-Models rendering
				RenderPostModels<false, EVideoBackendType::eVulkan>();

				// Post-Effects rendering
				RenderPostTextures<false, EVideoBackendType::eVulkan>();

				RenderPoints();
			}
			break;
#endif

		default: break;
		}

		//EVideoBackend::ClearDepthAndStencil();
		EVideoBackend::SetViewport(MUVideoManager::_2DViewport.OffsetY,
			MUVideoManager::_2DViewport.OffsetX,
			MUVideoManager::_2DViewport.Width,
			MUVideoManager::_2DViewport.Height);

		EVideoBackend::BindGlobalConstant(EGlobalConstant::eUI);

		g_pCharacterRoot->Draw();
		g_pCharacterRoot->PostDraw();
		g_pOptionRoot->Draw();
		g_pOptionRoot->PostDraw();
		g_pPickedItem.Draw();
		g_pMessageRoot->Draw();
		g_pMessageRoot->PostDraw();

		EVideoBackend::RenderMouse(MillisecondsElapse, CurrentTime);

		EVideoBackend::EndRenderPass();
		EVideoPipeline::UnbindVAO();
	}

	_ThreadTasking = TASKING_CLEAN_COUNTERS;
	_ThreadBarrier->SyncSemaphore();

	RunCleanCountersProcess(0);
}

void MUTasking::GameProcessGFX(mu_uint64 CurrentTime)
{
	_RootUI = g_pNewUISystem;

	// Update Process
	g_pNewUISystem->Update(CurrentTime);
	g_pOptionRoot->Update(CurrentTime);
	g_pMessageRoot->Update(CurrentTime);
	g_pNewUISystem->UpdateGFX();
	g_pOptionRoot->UpdateGFX();
	g_pMessageRoot->UpdateGFX();
}

void MUTasking::GameProcess(mu_float MillisecondsElapse, mu_uint64 CurrentTime)
{
	MUVideoManager::_3DViewport.OffsetX = 0;
	MUVideoManager::_3DViewport.OffsetY = 0;
	MUVideoManager::_3DViewport.Width = (mu_float)MU_TO_WINDOW(GetScreenWidth(), MU_RESOURCE->_640x480Rate[0]);
	MUVideoManager::_3DViewport.Height = (mu_float)MU_RESOURCE->GetWindowHeight() * 0.9f;
#if DIRECTX_BACKEND_ENABLE == 1
	MUVideoManager::_3DViewport.MinDepth = 0.0f;
	MUVideoManager::_3DViewport.MaxDepth = 1.0f;
#endif

	MU_RESOURCE->UpdateWorldTime(CurrentTime);

	TakeScreenshot();

	_ElapsedTime = MillisecondsElapse;
	_ElapsedFrame = (mu_double)_ElapsedTime / (mu_double)GAME_FPS;
	_MillisecondsElapse += MillisecondsElapse;
	_UpdateCounter = _MillisecondsElapse / GAME_FPS;
	_MillisecondsElapse -= _UpdateCounter * GAME_FPS;
	_CheckRuntimeElapse += MillisecondsElapse;
	_CheckRuntimeCounter = _CheckRuntimeElapse / RUNTIME_CHECKTIME;
	_CheckRuntimeElapse -= _CheckRuntimeCounter * RUNTIME_CHECKTIME;
	g_CurrentTime = CurrentTime;

	if (CurrentTime - g_LiveCheckTime >= GAME::LIVECHECK_TIME)
	{
		g_LiveCheckTime = CurrentTime;

		CL2GS_LIVE Live;
		g_GameServerObject->Send(&Live);
	}

	mu_uint64 diffTime = g_CurrentTime - g_CaretTime;

	if (diffTime >= g_CaretBlinkTime)
	{
		mu_boolean caretChange = (diffTime / g_CaretBlinkTime) % 2;

		g_CaretTime = g_CurrentTime;

		if (caretChange)
		{
			g_CaretEnabled = !g_CaretEnabled;
		}
	}

	mu_boolean isLightUpload = false;

	if (_UpdateCounter >= 1)
	{
		g_SpritesCount.store(0, std::memory_order_relaxed);
		g_TileSpritesCount.store(0, std::memory_order_relaxed);
		g_GuildSpritesCount.store(0, std::memory_order_relaxed);
		g_TerrainModificationCount.store(0, std::memory_order_relaxed);
		isLightUpload = true;
		g_LightEnabled = true;

		g_Terrain.RestoreLight();
	}

	ZeroArray(g_CulledBlockVisible);

	g_EffectsCulledCount = 0;
	g_EffectsInstancesCount = 0;
	g_EffectsCulledDataCount.store(0, std::memory_order_relaxed);
	g_PreRenderSlotCount.store(0, std::memory_order_relaxed);
	g_PostRenderSlotCount.store(0, std::memory_order_relaxed);
	g_ClothRenderSlotCount.store(0, std::memory_order_relaxed);
	g_GUIRenderSlotCount = 0;
	g_EffectsRenderCount.store(0, std::memory_order_relaxed);
	g_TexturesCulledCount = 0;
	g_PreTexturesCulledCount = 0;
	g_PostTexturesCulledCount = 0;

	g_JointsRenderCount = 0;

	_SkeletonCount.store(0, std::memory_order_relaxed);
	_ModelInstanceCount.store(0, std::memory_order_relaxed);
	_EffectInstanceCount.store(0, std::memory_order_relaxed);
	_DynamicVerticesCount.store(0, std::memory_order_relaxed);

	_DisableObjectRender = false;
	_DisableTextureRender = false;

#ifndef DISABLE_AUDIO
	EVector3 &CameraDir = MU_CAMERA->GetCameraInverseDirection();
	MU_AUDIO->_Listener.OrientTop.x = CameraDir.x;
	MU_AUDIO->_Listener.OrientTop.y = -CameraDir.y;
	MU_AUDIO->_Listener.OrientTop.z = CameraDir.z;

	EVector3 &CameraUp = MU_CAMERA->GetCameraInverseUp();
	MU_AUDIO->_Listener.OrientTop.x = CameraUp.x;
	MU_AUDIO->_Listener.OrientTop.y = -CameraUp.y;
	MU_AUDIO->_Listener.OrientTop.z = CameraUp.z;

	EVector3 &CameraPos = MU_CAMERA->GetCameraPosition();
	MU_AUDIO->_Listener.Position.x = CameraPos.x * g_TerrainScaleRev;
	MU_AUDIO->_Listener.Position.y = -CameraPos.y * g_TerrainScaleRev;
	MU_AUDIO->_Listener.Position.z = CameraPos.z * g_TerrainScaleRev;
#endif

	CollisionDistance = FLT_MAX;

	mu_float mouseX = MU_INPUT->GetMouseX();
	mu_float mouseY = MU_VIDEO->_3DViewport.Height - (MU_INPUT->GetMouseY() - MU_VIDEO->_3DViewport.OffsetY);

	MousePosition = EMath::unProject(EVector3(mouseX, mouseY, 0.0f), MU_VIDEO->_CurrentView, MU_VIDEO->_CurrentRayProjection, EDVector4(MU_VIDEO->_3DViewport.OffsetX, MU_VIDEO->_3DViewport.OffsetY, MU_VIDEO->_3DViewport.Width, MU_VIDEO->_3DViewport.Height));
	MouseTarget = EMath::unProject(EVector3(mouseX, mouseY, 1.0f), MU_VIDEO->_CurrentView, MU_VIDEO->_CurrentRayProjection, EDVector4(MU_VIDEO->_3DViewport.OffsetX, MU_VIDEO->_3DViewport.OffsetY, MU_VIDEO->_3DViewport.Width, MU_VIDEO->_3DViewport.Height));
	MouseMin = EMath::Min(MousePosition, MouseTarget);
	MouseMax = EMath::Max(MousePosition, MouseTarget);
	VectorSubtract(MouseTarget, MousePosition, MouseDirection);
	MouseAbsDirection = EMath::Abs(MouseDirection);

	MouseAbsMin = MouseAbsDirection[0];
	MouseAbsMinIndex = 0;

	if (MouseAbsDirection[1] > MouseAbsDirection[2])
	{
		if (MouseAbsMin > MouseAbsDirection[2])
		{
			MouseAbsMin = MouseAbsDirection[2];
			MouseAbsMinIndex = 2;
		}
	}
	else
	{
		if (MouseAbsMin > MouseAbsDirection[1])
		{
			MouseAbsMin = MouseAbsDirection[1];
			MouseAbsMinIndex = 1;
		}
	}

	if (EVideoBackend::GetVideoStatus(EVideoStatusFlag::VSF_Available) == false)
	{
		_SkeletonBuffer = nullptr;
		_ModelInstanceBuffer = nullptr;
		_EffectInstanceBuffer = nullptr;
		_DynamicClothBuffer = nullptr;
	}
	else
	{
		_SkeletonBuffer = EVideoBuffer::GetBonesBuffer();
		_ModelInstanceBuffer = EVideoBuffer::GetModelInstancesBuffer();
		_EffectInstanceBuffer = EVideoBuffer::GetEffectInstancesBuffer();
		_DynamicClothBuffer = EVideoBuffer::GetClothVerticesBuffer();
	}

	_ThreadTasking = TASKING_GAME;
	_ThreadBarrier->SyncSemaphore();

	RunGameProcess(0);

	MUVideoManager::OnUpdateBuffer();

	SelectObjects();

	// Moved Code from RunGameVisual
	{
		if (_UpdateCounter > 0)
		{
			RenderChaosCastleVisual(0);
		}

		if (_UpdateCounter > 0)
		{
			if (Time_Effect > 40)
				Time_Effect = 0;
			++Time_Effect;

			if (GET_WORLD == WD_34CRYWOLF_1ST)
			{
				if (++Change_weather > 700)
				{
					weather = GetLargeRand(3);
					Change_weather = 0;
				}
			}
		}
	}

	if (EVideoBackend::GetVideoStatus(EVideoStatusFlag::VSF_Visible | EVideoStatusFlag::VSF_Available) == true)
	{
		_ThreadTasking = TASKING_GAME_RENDER;
		_ThreadBarrier->SyncSemaphore();

		RunGameVisual(0);
	}

	ProcessBGM();

	if (MU_UPDATECOUNTER > 0 &&
		g_pCSWaterTerrain != nullptr)
	{
		g_pCSWaterTerrain->Update();
	}

	EVideoBackend::UpdateBuffers(isLightUpload);
	EVideoBackend::EmptyTrash();

	if (EVideoBackend::GetVideoStatus(EVideoStatusFlag::VSF_Visible | EVideoStatusFlag::VSF_Available) == true)
	{
		if (MU_RESOURCE->GetShadowType() >= EShadowType::ePCF)
		{
			EVideoBackend::BeginShadowRenderPass();
			EVideoBackend::BindGlobalConstant(EGlobalConstant::eShadow);
			RenderShadowMapping();
			EVideoBackend::EndShadowRenderPass();
		}

		EVideoBackend::BeginRenderPass(g_Terrain._ClearColor);

		EVideoBackend::SetViewport(MUVideoManager::_3DViewport.OffsetX,
			MUVideoManager::_3DViewport.OffsetY,
			MUVideoManager::_3DViewport.Width,
			MUVideoManager::_3DViewport.Height);
		EVideoBackend::CleanScissor();

		EVideoBackend::BindGlobalConstant(EGlobalConstant::eGame);

		switch (EVideoBackend::GetBackend())
		{
#if DIRECTX_BACKEND_ENABLE == 1
		case EVideoBackendType::eDirectX:
			{
				// Terrain rendering
				g_Terrain.Render<false, EVideoBackendType::eDirectX>();

				// Pre-Models rendering
				if (MU_RESOURCE->GetShadowType() <= EShadowType::eOriginalPlus &&
					MU_RESOURCE->GetShadowType() > EShadowType::eNone)
				{
					RenderShadowModels<false, EVideoBackendType::eDirectX>();
				}

				RenderPreModels<false, EVideoBackendType::eDirectX>();

				// Pre-Effects rendering
				RenderPreTextures<false, EVideoBackendType::eDirectX>();

				// Pre-Models rendering with Alpha
				RenderPreAlphaModels<false, EVideoBackendType::eDirectX>();

				// Terrain post rendering
				g_Terrain.PostRender<false, EVideoBackendType::eDirectX>();

				if (g_pCSWaterTerrain != nullptr)
				{
					g_pCSWaterTerrain->Render();
				}

				// Post-Models rendering
				RenderPostModels<false, EVideoBackendType::eDirectX>();

				// Post-Effects rendering
				RenderPostTextures<false, EVideoBackendType::eDirectX>();

				RenderPoints();
			}
			break;
#endif

#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
			{
				if (EOpenGLExtensions::IsFeatureSupported(OpenGLFeatures::BaseInstance) == true)
				{
					// Terrain rendering
					g_Terrain.Render<true, EVideoBackendType::eOpenGL>();

					// Pre-Models rendering
					if (MU_RESOURCE->GetShadowType() <= EShadowType::eOriginalPlus &&
						MU_RESOURCE->GetShadowType() > EShadowType::eNone)
					{
						RenderShadowModels<true, EVideoBackendType::eOpenGL>();
					}

					RenderPreModels<true, EVideoBackendType::eOpenGL>();

					// Pre-Effects rendering
					RenderPreTextures<true, EVideoBackendType::eOpenGL>();

					// Pre-Models rendering with Alpha
					RenderPreAlphaModels<true, EVideoBackendType::eOpenGL>();

					// Terrain post rendering
					g_Terrain.PostRender<true, EVideoBackendType::eOpenGL>();

					if (g_pCSWaterTerrain != nullptr)
					{
						g_pCSWaterTerrain->Render();
					}

					// Post-Models rendering
					RenderPostModels<true, EVideoBackendType::eOpenGL>();

					// Post-Effects rendering
					RenderPostTextures<true, EVideoBackendType::eOpenGL>();

					RenderPoints();
				}
				else
				{
					// Terrain rendering
					g_Terrain.Render<false, EVideoBackendType::eOpenGL>();

					// Pre-Models rendering
					if (MU_RESOURCE->GetShadowType() <= EShadowType::eOriginalPlus &&
						MU_RESOURCE->GetShadowType() > EShadowType::eNone)
					{
						RenderShadowModels<false, EVideoBackendType::eOpenGL>();
					}

					RenderPreModels<false, EVideoBackendType::eOpenGL>();

					// Pre-Effects rendering
					RenderPreTextures<false, EVideoBackendType::eOpenGL>();

					// Pre-Models rendering with Alpha
					RenderPreAlphaModels<false, EVideoBackendType::eOpenGL>();

					// Terrain post rendering
					g_Terrain.PostRender<false, EVideoBackendType::eOpenGL>();

					if (g_pCSWaterTerrain != nullptr)
					{
						g_pCSWaterTerrain->Render();
					}

					// Post-Models rendering
					RenderPostModels<false, EVideoBackendType::eOpenGL>();

					// Post-Effects rendering
					RenderPostTextures<false, EVideoBackendType::eOpenGL>();

					RenderPoints();
				}
			}
			break;
#endif

#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
			{
				// Terrain rendering
				g_Terrain.Render<false, EVideoBackendType::eVulkan>();

				// Pre-Models rendering
				if (MU_RESOURCE->GetShadowType() <= EShadowType::eOriginalPlus &&
					MU_RESOURCE->GetShadowType() > EShadowType::eNone)
				{
					RenderShadowModels<false, EVideoBackendType::eVulkan>();
				}

				RenderPreModels<false, EVideoBackendType::eVulkan>();

				// Pre-Effects rendering
				RenderPreTextures<false, EVideoBackendType::eVulkan>();

				// Pre-Models rendering with Alpha
				RenderPreAlphaModels<false, EVideoBackendType::eVulkan>();

				// Terrain post rendering
				g_Terrain.PostRender<false, EVideoBackendType::eVulkan>();

				if (g_pCSWaterTerrain != nullptr)
				{
					g_pCSWaterTerrain->Render();
				}

				// Post-Models rendering
				RenderPostModels<false, EVideoBackendType::eVulkan>();

				// Post-Effects rendering
				RenderPostTextures<false, EVideoBackendType::eVulkan>();

				RenderPoints();
			}
			break;
#endif

		default: break;
		}

		//EVideoBackend::ClearDepthAndStencil();
		EVideoBackend::SetViewport(MUVideoManager::_2DViewport.OffsetY,
			MUVideoManager::_2DViewport.OffsetX,
			MUVideoManager::_2DViewport.Width,
			MUVideoManager::_2DViewport.Height);

		EVideoBackend::BindGlobalConstant(EGlobalConstant::eUI);

		SCENES::GameScene.RenderOutsides();

		g_pNewUISystem->Draw();
		g_pNewUISystem->PostDraw();
		g_pOptionRoot->Draw();
		g_pOptionRoot->PostDraw();
		g_pPickedItem.Draw();
		g_pMessageRoot->Draw();
		g_pMessageRoot->PostDraw();

		EVideoBackend::RenderMouse(MillisecondsElapse, CurrentTime);

		EVideoBackend::EndRenderPass();
		EVideoPipeline::UnbindVAO();
	}

	_ThreadTasking = TASKING_CLEAN_COUNTERS;
	_ThreadBarrier->SyncSemaphore();

	RunCleanCountersProcess(0);
}

void MUTasking::TaskProcess(const mu_uint32 ThreadIndex)
{
	CRASHRPT_AUTOTHREAD;

	while (true)
	{
		_ThreadBarrier->SyncSemaphore();

		if (_ThreadsDestroy == true)
		{
			break;
		}

		switch (_ThreadTasking)
		{
		case TASKING_LOGIN:
		case TASKING_CHARACTER:
		case TASKING_GAME:
			{
				RunGameProcess(ThreadIndex);
			}
			break;

		case TASKING_LOGIN_RENDER:
		case TASKING_CHARACTER_RENDER:
		case TASKING_GAME_RENDER:
			{
				RunGameVisual(ThreadIndex);
			}
			break;

		case TASKING_CLEAN_COUNTERS:
			{
				RunCleanCountersProcess(ThreadIndex);
			}
			break;
		}
	}
}

extern EVector3 g_vParticleWind;
extern EVector3 g_vParticleWindVelo;

void MUTasking::RunGameProcess(const mu_uint32 ThreadIndex)
{
	UpdateSounds(ThreadIndex);

	if(ThreadIndex == 0)
	{
		g_Luminosity = EMath::Sin(FWorldTime*0.004f)*0.15f + 0.5f;

		if (MU_UPDATECOUNTER > 0)
		{
			if (g_Hellas01MeshLight > 1.0f)
			{
				g_Hellas01MeshLight = 1.00f;
				g_Hellas01AddLight = -0.01f;
			}

			if (g_Hellas01MeshLight < 0.01f)
			{
				g_Hellas01MeshLight = 0.01f;
				g_Hellas01AddLight = 0.01f;
			}

			g_Hellas01MeshLight += g_Hellas01AddLight;
		}
	}

	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	for (mu_uint32 updateTotal = 0; updateTotal < _UpdateCounter; ++updateTotal)
	{
		if (updateTotal == _UpdateCounter - 1)
		{
			if (ThreadIndex == 0)
			{
				g_LightEnabled = true;
			}
		}
		else if (updateTotal == 0)
		{
			if (ThreadIndex == 0)
			{
				g_LightEnabled = false;
			}
		}

		if (ThreadIndex == 0)
		{
			EarthQuake.StoreDirect(EarthQuake.Load() * 0.2f);

			for (mu_uint32 i = 0; i < 2; ++i)
			{
				g_vParticleWindVelo[i] += (GetLargeRand(2001) - 1000) * (0.001f * 0.6f);
				g_vParticleWindVelo[i] = EMath::Max(-.6f, EMath::Min(g_vParticleWindVelo[i], 0.6f));
			}
			for (mu_uint32 i = 0; i < 2; ++i)
			{
				g_vParticleWind[i] += g_vParticleWindVelo[i];
				g_vParticleWind[i] = EMath::Max(-1.7f, EMath::Min(g_vParticleWind[i], 1.7f));
			}

			g_EffectsLocked = true;
			g_JointsLocked = true;
			g_ParticlesLocked = true;
		}

		CheckMoveEffects(ThreadIndex);
		CheckMoveParticles(ThreadIndex);
		CheckMoveJoints(ThreadIndex);

		if (ThreadIndex == 0 &&
			g_CurrentScene == MAIN_SCENE)
		{
			MoveHero();
			g_pSimpleNotice->Move();
		}

		_ThreadBarrier->Sync();

		MoveObjects(ThreadIndex);
		MoveItems(ThreadIndex);

		if ((GET_WORLD == WD_0LORENCIA && HeroTile != 4) ||
			(GET_WORLD == WD_2DEVIAS && HeroTile != 3 && HeroTile < 10)
			|| GET_WORLD == WD_3NORIA
			|| GET_WORLD == WD_7ATLANSE
			|| InDevilSquare() == true
			|| GET_WORLD == WD_10HEAVEN
			|| InChaosCastle() == true
			|| GET_WORLD == WD_30BATTLECASTLE
			|| M31HuntingGround::IsInHuntingGround() == true
			|| M33Aida::IsInAida() == true
			|| M34CryWolf1st::IsCyrWolf1st() == true
			|| GET_WORLD == WD_42CHANGEUP3RD_2ND
			|| IsIceCity()
			|| IsSantaTown()
			|| IsPKField()
			|| IsDoppelGanger2()
			|| IsEmpireGuardian1()
			|| IsEmpireGuardian2()
			|| IsEmpireGuardian3()
			|| IsEmpireGuardian4()
			|| IsUnitedMarketPlace()
			)
		{
			MoveLeaves(ThreadIndex);
		}

		MoveBugs(ThreadIndex);

		if (g_CurrentScene == MAIN_SCENE)
		{
			MoveBoids(ThreadIndex); // Boids and Fishs (2 threads)
		}

		MoveCharacters(ThreadIndex);

		if (ThreadIndex == 0 &&
			g_CurrentScene == CHARACTER_SCENE)
		{
			MoveCharacterView();
		}

#ifdef ENABLE_SKILLPROCESSORS
		// Skill Processor
		{
			_ThreadBarrier->Sync();
			ProcessSkills(ThreadIndex, _ThreadsCount);
		}
#endif

		MovePets(ThreadIndex);
		MoveBlurs(ThreadIndex);
		MoveEffects(ThreadIndex);
		MovePoints(ThreadIndex);
		MoveJoints(ThreadIndex);
		MoveParticles(ThreadIndex);
		MovePointers(ThreadIndex);

		_ThreadBarrier->Sync();

		if (g_CurrentScene == MAIN_SCENE)
		{
			g_pSimpleChat->MultithreadUpdate(ThreadIndex);
		}

		if (ThreadIndex == 0)
		{
			g_EffectsLocked = false;
			g_JointsLocked = false;
			g_ParticlesLocked = false;
		}

		ThreadInfo.NoLockedAllowed = true;

		mu_uint32 effectsWait = g_EffectsWaitCount.load(std::memory_order_relaxed);
		if (effectsWait > MAX_EFFECTS_WAIT) effectsWait = MAX_EFFECTS_WAIT;

		for (mu_uint32 i = ThreadIndex; i < effectsWait; i += _ThreadsCount)
		{
			EFFECT_WAIT &effect = g_EffectsWait[i];

			CreateEffect<false>(ThreadIndex, effect.Type, effect.Position, effect.Angle, effect.Light, effect.SubType, effect.Owner, effect.PKKey, effect.SkillIndex, effect.Skill, effect.SkillSerialNum, effect.Scale, effect.sTargetIndex);
		}

		mu_uint32 jointsWait = g_JointsWaitCount.load(std::memory_order_relaxed);
		if (jointsWait > MAX_JOINTS_WAIT) jointsWait = MAX_JOINTS_WAIT;

		for (mu_uint32 i = ThreadIndex; i < jointsWait; i += _ThreadsCount)
		{
			JOINT_WAIT &joint = g_JointsWait[i];

			if (joint.EnablePriorColor == false)
			{
				CreateJoint<false>(ThreadIndex, joint.Type, joint.Position, joint.TargetPosition, joint.Angle, joint.SubType, joint.Target, joint.Scale, joint.PKKey, joint.SkillIndex, joint.SkillSerialNum, joint.iChaIndex, nullptr, joint.sTargetIndex);
			}
			else
			{
				CreateJoint<false>(ThreadIndex, joint.Type, joint.Position, joint.TargetPosition, joint.Angle, joint.SubType, joint.Target, joint.Scale, joint.PKKey, joint.SkillIndex, joint.SkillSerialNum, joint.iChaIndex, &joint.vPriorColor, joint.sTargetIndex);
			}
		}

		mu_uint32 particlesWait = g_ParticlesWaitCount.load(std::memory_order_relaxed);
		if (particlesWait > MAX_PARTICLES_WAIT) particlesWait = MAX_PARTICLES_WAIT;

		for (mu_uint32 i = ThreadIndex; i < particlesWait; i += _ThreadsCount)
		{
			PARTICLE_WAIT &particle = g_ParticlesWait[i];

			CreateParticle<false>(ThreadIndex, particle.Type, particle.Position, particle.Angle, particle.Light, particle.SubType, particle.Scale, particle.Owner);
		}

		ThreadInfo.NoLockedAllowed = false;

		_ThreadBarrier->Sync();

		EraseEffects(ThreadIndex);

		// Move Physics here too

		if(ThreadIndex == 0)
		{
			g_EffectsWaitCount.store(0, std::memory_order_relaxed);
			g_JointsWaitCount.store(0, std::memory_order_relaxed);
			g_ParticlesWaitCount.store(0, std::memory_order_relaxed);

			for (mu_int32 k = 0; k < 5; ++k)
			{
				mu_float fPlus = (GetLargeRand(200) - 100) * 0.001f;
				CPhysicsManager::s_fWindBase += fPlus;
				CPhysicsManager::s_fWindBase = EMath::Min(EMath::Max(-0.2f, CPhysicsManager::s_fWindBase), 1.0f);
			}

			for (mu_int32 k = 0; k < MAX_TASKING_THREADS; ++k)
			{
				CPhysicsManager::s_fWind[k] = CPhysicsManager::s_fWindBase;
			}

			++g_CreationTime;
			if (ChatTime > 0) --ChatTime;
			if (MacroTime > 0) --MacroTime;
			WaterTextureNumber = (WaterTextureNumber + 1) % 32;
			++MoveSceneFrame;

			if (g_LightEnabled == true)
			{
				mu_uint32 modCount = g_TerrainModificationCount.load(std::memory_order_relaxed);
				if(modCount > MAX_TERRAIN_MODIFICATION) modCount = MAX_TERRAIN_MODIFICATION;
				for (mu_uint32 n = 0; n < modCount; ++n)
				{
					TerrainModificationRequest &request = g_TerrainModification[n];

					AddTerrainHeight(request.PositionX, request.PositionY, request.Height, request.Range, request.Buffer);

					if (request.CreateNormal == true)
					{
						CreateTerrainNormal_Part(request.CollisionX, request.CollisionY);
					}

					if (request.CreateLight == true)
					{
						CreateTerrainLight_Part(request.CollisionX, request.CollisionY);
					}
				}
			}
		}
	}

	if (ThreadIndex == 0)
	{
		switch (_ThreadTasking)
		{
		case TASKING_LOGIN:
		case TASKING_CHARACTER:
			{
				EVector3 tmp;
				MU_CAMERA->Update(_ElapsedTime, tmp);
			}
			break;

		case TASKING_GAME:
			{
				MU_CAMERA->Update(_ElapsedTime, Hero->Object.Position);
			}
			break;
		}

		g_CpuCulling.Update();
	}

	_ThreadBarrier->Sync();

	if (EVideoBackend::GetVideoStatus(EVideoStatusFlag::VSF_Visible | EVideoStatusFlag::VSF_Available) == true)
	{
#ifndef USE_CULLING_PARALLEL_FOR
		mu_uint32 queueCount = 0;
		const mu_uint32 queueIndex = MUThreadQueues::TerrainCollisionQueueIndex[ThreadIndex];

		mu_uint32 beginIndex, endIndex;
		GetIndexTasking(ThreadIndex, CPUCulling::BLOCK_SIZE, beginIndex, endIndex);

		for (mu_uint32 by = beginIndex; by < endIndex; ++by)
		{
			mu_uint32 startBlock = (mu_uint32)-1;
			mu_uint32 endBlock = (mu_uint32)-1;
			const mu_uint32 sbx = by * CPUCulling::BLOCK_SIZE;
			const mu_uint32 ebx = sbx + CPUCulling::BLOCK_SIZE;
			mu_uint32 bx = sbx;

			for (; bx < ebx; ++bx)
			{
				if (EMath::Contains(g_CpuCulling._Frustum, g_CpuCulling._BoundingBox[bx]) != EMath::FR_EXCLUDED)
				{
					startBlock = bx;
					break;
				}
			}

			for (; bx < ebx; ++bx)
			{
				if (EMath::Contains(g_CpuCulling._Frustum, g_CpuCulling._BoundingBox[bx]) == EMath::FR_EXCLUDED)
				{
					endBlock = bx;
					break;
				}
			}

			if (startBlock != (mu_uint32)-1)
			{
				if (endBlock == (mu_uint32)-1)
				{
					endBlock = bx;
				}

				const mu_uint32 blocksCount = endBlock - startBlock;
				const mu_uint32 queueIncCount = (blocksCount * MUTerrain::TERRAIN_BLOCK);

				FillArray(g_CulledBlockVisible, sbx, (startBlock - sbx), false);
				FillArray(g_CulledBlockVisible, startBlock, blocksCount, true);
				FillArray(g_CulledBlockVisible, endBlock, ebx - endBlock, false);

				mu_uint32 esindex = ((startBlock / CPUCulling::BLOCK_SIZE) * MUTerrain::TERRAIN_BLOCK) * TERRAIN_SIZE + ((startBlock % CPUCulling::BLOCK_SIZE) * MUTerrain::TERRAIN_BLOCK);
				for (mu_uint32 ey = 0; ey < MUTerrain::TERRAIN_BLOCK; ++ey, esindex += TERRAIN_SIZE)
				{
					mu_uint32 baseIndex = queueCount;
					queueCount += queueIncCount;

					mu_uint32 n = esindex;
					for (; baseIndex < queueCount; ++baseIndex)
					{
						MUThreadQueues::TerrainCollisionQueue[queueIndex + baseIndex] = n++;
					}
				}
			}
			else
			{
				FillArray(g_CulledBlockVisible, sbx, CPUCulling::BLOCK_SIZE, false);
			}
		}

		MUThreadQueues::TerrainCollisionQueueCount[ThreadIndex] = queueCount;
#endif
	}

	RenderTerrainVisual(ThreadIndex);
}

FORCEINLINE void ConfigureInstancesIndex(const EEffectCulledData *array, const mu_uint32 count)
{
	mu_uint32 n = 0;
	mu_uint32 instancesCount = g_EffectsInstancesCount;
	mu_uint16 jointsCount = g_JointsRenderCount;
	for(;n < count;++n)
	{
		const EEffectCulledData &data = array[n];
		switch(static_cast<const EEffectProxyEnum>(data.Type))
		{
		case EEffectProxyEnum::eLeaves:
			{
				constexpr mu_uint32 RenderCount =  1;
				if(instancesCount + RenderCount > MAX_EFFECTS_INSTANCES)
				{
					break;
				}
				instancesCount += RenderCount;

				const PARTICLE *o = &Leaves[data.Index];
				ETextureRenderProxy *proxy = &TEXTURES::Proxy[o->Type];
				const mu_uint32 instanceIndex = proxy->RequestInstanceIndex<std::memory_order_relaxed>(o->RenderType, RenderCount);
				MURenderProxyEffects::SetInstanceIndex<EEffectProxyEnum::eLeaves, 0>(data.Index, instanceIndex);

				const mu_uint32 renderFlag = 1 << o->RenderType;
				const mu_uint32 modesEnabled = proxy->RegisterInstancesMode(renderFlag);

				if (modesEnabled == 0)
				{
					g_TexturesCulledList[g_TexturesCulledCount++] = o->Type;
				}

				if ((modesEnabled & renderFlag) == 0)
				{
					CULLED_DATA *cData = nullptr;

					if (o->RenderType >= BLEND::TXB_DEPTH_BEGIN &&
						o->RenderType < BLEND::TXB_DEPTH_END)
					{
						const mu_uint32 effectIndex = g_PreTexturesCulledCount++;
						cData = &g_PreTexturesCulledList[effectIndex];
					}
					else
					{
						const mu_uint32 effectIndex = g_PostTexturesCulledCount++;
						cData = &g_PostTexturesCulledList[effectIndex];
					}

					cData->Hash = RetrieveTextureHash(o->Type, o->RenderType);
				}
			}
			break;

		case EEffectProxyEnum::eJoints:
			{
				const JOINT *o = &g_Joints[data.Index];

				mu_uint32 multiplier = 1;

				if (o->Type == BITMAP_JOINT_FORCE &&
					o->SubType == 0)
				{
					multiplier = 1;
				}
				else if (o->Features.Get(JOINT::eJFS_FACEONE | JOINT::eJFS_FACETWO) == true)
				{
					multiplier = 2;
				}

				mu_int32 numTails = o->NumTails * multiplier;

				if (o->Type == BITMAP_SMOKE &&
					o->SubType == 0)
				{
					numTails -= multiplier;
				}
				else if (o->Type == BITMAP_JOINT_HEALING &&
					(o->SubType == 9 ||
						o->SubType == 10))
				{
					numTails -= multiplier;
				}

				if (instancesCount + numTails > MAX_EFFECTS_INSTANCES)
				{
					break;
				}
				instancesCount += numTails;
				++jointsCount;

				ETextureRenderProxy *proxy = &TEXTURES::Proxy[o->TexType];
				const mu_uint32 instanceIndex = proxy->RequestInstanceIndex<std::memory_order_relaxed>(o->RenderType, numTails);
				MURenderProxyEffects::SetInstanceIndex<EEffectProxyEnum::eJoints, 0>(data.Index, instanceIndex);

				const mu_uint32 renderFlag = 1 << o->RenderType;
				const mu_uint32 modesEnabled = proxy->RegisterInstancesMode(renderFlag);

				if (modesEnabled == 0)
				{
					g_TexturesCulledList[g_TexturesCulledCount++] = o->TexType;
				}

				if ((modesEnabled & renderFlag) == 0)
				{
					CULLED_DATA *cData = nullptr;

					if (o->RenderType >= BLEND::TXB_DEPTH_BEGIN &&
						o->RenderType < BLEND::TXB_DEPTH_END)
					{
						const mu_uint32 effectIndex = g_PreTexturesCulledCount++;
						cData = &g_PreTexturesCulledList[effectIndex];
					}
					else
					{
						const mu_uint32 effectIndex = g_PostTexturesCulledCount++;
						cData = &g_PostTexturesCulledList[effectIndex];
					}

					cData->Hash = RetrieveTextureHash(o->TexType, o->RenderType);
				}
			}
			break;

		case EEffectProxyEnum::eParticles:
			{
				const PARTICLE *o = &g_Particles[data.Index];

				if (o->Type == BITMAP_SHINY + 6)
				{
					constexpr mu_uint32 RenderCount = 2;
					if (instancesCount + RenderCount > MAX_EFFECTS_INSTANCES)
					{
						break;
					}
					instancesCount += RenderCount;

					ETextureRenderProxy *proxy = &TEXTURES::Proxy[o->TexType];
					const mu_uint32 instanceIndex = proxy->RequestInstanceIndex<std::memory_order_relaxed>(o->RenderType, 1);
					MURenderProxyEffects::SetInstanceIndex<EEffectProxyEnum::eParticles, 0>(data.Index, instanceIndex);
					ETextureRenderProxy *extraProxy = &TEXTURES::Proxy[BITMAP_LIGHT];
					const mu_uint32 extraInstanceIndex = extraProxy->RequestInstanceIndex<std::memory_order_relaxed>(o->RenderType, 1);
					MURenderProxyEffects::SetInstanceIndex<EEffectProxyEnum::eParticles, 1>(data.Index, extraInstanceIndex);

					const mu_uint32 renderFlag = 1 << o->RenderType;
					const mu_uint32 modesEnabled1 = proxy->RegisterInstancesMode(renderFlag);
					const mu_uint32 modesEnabled2 = extraProxy->RegisterInstancesMode(renderFlag);

					if (modesEnabled1 == 0)
					{
						g_TexturesCulledList[g_TexturesCulledCount++] = o->TexType;
					}

					if (modesEnabled2 == 0)
					{
						g_TexturesCulledList[g_TexturesCulledCount++] = BITMAP_LIGHT;
					}

					if ((modesEnabled1 & renderFlag) == 0)
					{
						CULLED_DATA *cData = nullptr;

						if (o->RenderType >= BLEND::TXB_DEPTH_BEGIN &&
							o->RenderType < BLEND::TXB_DEPTH_END)
						{
							const mu_uint32 effectIndex = g_PreTexturesCulledCount++;
							cData = &g_PreTexturesCulledList[effectIndex];
						}
						else
						{
							const mu_uint32 effectIndex = g_PostTexturesCulledCount++;
							cData = &g_PostTexturesCulledList[effectIndex];
						}

						cData->Hash = RetrieveTextureHash(o->TexType, o->RenderType);
					}

					if ((modesEnabled2 & renderFlag) == 0)
					{
						CULLED_DATA *cData = nullptr;

						if (o->RenderType >= BLEND::TXB_DEPTH_BEGIN &&
							o->RenderType < BLEND::TXB_DEPTH_END)
						{
							const mu_uint32 effectIndex = g_PreTexturesCulledCount++;
							cData = &g_PreTexturesCulledList[effectIndex];
						}
						else
						{
							const mu_uint32 effectIndex = g_PostTexturesCulledCount++;
							cData = &g_PostTexturesCulledList[effectIndex];
						}

						cData->Hash = RetrieveTextureHash(BITMAP_LIGHT, o->RenderType);
					}
				}
				else
				{
					ETextureRenderProxy *proxy = &TEXTURES::Proxy[o->TexType];

					if (o->Type == BITMAP_FIRECRACKER)
					{
						if (instancesCount + o->Frame + 1 > MAX_EFFECTS_INSTANCES)
						{
							break;
						}
						instancesCount += o->Frame + 1;

						const mu_uint32 instanceIndex = proxy->RequestInstanceIndex<std::memory_order_relaxed>(o->RenderType, o->Frame + 1);
						MURenderProxyEffects::SetInstanceIndex<EEffectProxyEnum::eParticles, 0>(data.Index, instanceIndex);
					}
					else if (o->Type == BITMAP_TORCH_FIRE)
					{
						constexpr mu_uint32 RenderCount = 3;
						if (instancesCount + RenderCount > MAX_EFFECTS_INSTANCES)
						{
							break;
						}
						instancesCount += RenderCount;

						const mu_uint32 instanceIndex = proxy->RequestInstanceIndex<std::memory_order_relaxed>(o->RenderType, 3);
						MURenderProxyEffects::SetInstanceIndex<EEffectProxyEnum::eParticles, 0>(data.Index, instanceIndex);
					}
					else
					{
						constexpr mu_uint32 RenderCount = 1;
						if (instancesCount + RenderCount > MAX_EFFECTS_INSTANCES)
						{
							break;
						}
						instancesCount += RenderCount;

						const mu_uint32 instanceIndex = proxy->RequestInstanceIndex<std::memory_order_relaxed>(o->RenderType, 1);
						MURenderProxyEffects::SetInstanceIndex<EEffectProxyEnum::eParticles, 0>(data.Index, instanceIndex);
					}

					const mu_uint32 renderFlag = 1 << o->RenderType;
					const mu_uint32 modesEnabled = proxy->RegisterInstancesMode(renderFlag);

					if (modesEnabled == 0)
					{
						g_TexturesCulledList[g_TexturesCulledCount++] = o->TexType;
					}

					if ((modesEnabled & renderFlag) == 0)
					{
						CULLED_DATA *cData = nullptr;

						if (o->RenderType >= BLEND::TXB_DEPTH_BEGIN &&
							o->RenderType < BLEND::TXB_DEPTH_END)
						{
							const mu_uint32 effectIndex = g_PreTexturesCulledCount++;
							cData = &g_PreTexturesCulledList[effectIndex];
						}
						else
						{
							const mu_uint32 effectIndex = g_PostTexturesCulledCount++;
							cData = &g_PostTexturesCulledList[effectIndex];
						}

						cData->Hash = RetrieveTextureHash(o->TexType, o->RenderType);
					}
				}
			}
			break;

		case EEffectProxyEnum::eBlurs:
			{
				const MUBlur *b = &g_Blur[data.Index];
				const mu_uint32 RenderCount = (mu_uint32)b->Number - 1;
				if (instancesCount + RenderCount > MAX_EFFECTS_INSTANCES)
				{
					break;
				}
				instancesCount += RenderCount;

				ETextureRenderProxy *proxy = &TEXTURES::Proxy[b->Texture];
				const mu_uint32 instanceIndex = proxy->RequestInstanceIndex<std::memory_order_relaxed>(b->RenderType, RenderCount);
				MURenderProxyEffects::SetInstanceIndex<EEffectProxyEnum::eBlurs, 0>(data.Index, instanceIndex);

				const mu_uint32 renderFlag = 1 << b->RenderType;
				const mu_uint32 modesEnabled = proxy->RegisterInstancesMode(renderFlag);

				if (modesEnabled == 0)
				{
					g_TexturesCulledList[g_TexturesCulledCount++] = b->Texture;
				}

				if ((modesEnabled & renderFlag) == 0)
				{
					CULLED_DATA *cData = nullptr;

					if (b->RenderType >= BLEND::TXB_DEPTH_BEGIN &&
						b->RenderType < BLEND::TXB_DEPTH_END)
					{
						const mu_uint32 effectIndex = g_PreTexturesCulledCount++;
						cData = &g_PreTexturesCulledList[effectIndex];
					}
					else
					{
						const mu_uint32 effectIndex = g_PostTexturesCulledCount++;
						cData = &g_PostTexturesCulledList[effectIndex];
					}

					cData->Hash = RetrieveTextureHash(b->Texture, b->RenderType);
				}
			}
			break;

		case EEffectProxyEnum::eObjectBlurs:
			{
				const MUObjectBlur *b = &g_ObjectBlur[data.Index];
				const mu_uint32 RenderCount = MUThreadQueues::PerObjectBlursRenderCount[data.Index];
				if (instancesCount + RenderCount > MAX_EFFECTS_INSTANCES)
				{
					break;
				}
				instancesCount += RenderCount;

				ETextureRenderProxy *proxy = &TEXTURES::Proxy[b->Texture];
				const mu_uint32 instanceIndex = proxy->RequestInstanceIndex<std::memory_order_relaxed>(b->RenderType, RenderCount);
				MURenderProxyEffects::SetInstanceIndex<EEffectProxyEnum::eObjectBlurs, 0>(data.Index, instanceIndex);

				const mu_uint32 renderFlag = 1 << b->RenderType;
				const mu_uint32 modesEnabled = proxy->RegisterInstancesMode(renderFlag);

				if (modesEnabled == 0)
				{
					g_TexturesCulledList[g_TexturesCulledCount++] = b->Texture;
				}

				if ((modesEnabled & renderFlag) == 0)
				{
					CULLED_DATA *cData = nullptr;

					if (b->RenderType >= BLEND::TXB_DEPTH_BEGIN &&
						b->RenderType < BLEND::TXB_DEPTH_END)
					{
						const mu_uint32 effectIndex = g_PreTexturesCulledCount++;
						cData = &g_PreTexturesCulledList[effectIndex];
					}
					else
					{
						const mu_uint32 effectIndex = g_PostTexturesCulledCount++;
						cData = &g_PostTexturesCulledList[effectIndex];
					}

					cData->Hash = RetrieveTextureHash(b->Texture, b->RenderType);
				}
			}
			break;

		case EEffectProxyEnum::ePointers:
			{
				const MUPointer *o = &g_Pointers[data.Index];
				const mu_int32 RenderCount = RenderTerrainBitmapCalculate(o->Scale, o->Scale);
				if (instancesCount + RenderCount > MAX_EFFECTS_INSTANCES)
				{
					break;
				}
				instancesCount += RenderCount;

				ETextureRenderProxy *proxy = &TEXTURES::Proxy[o->Type];
				const mu_uint32 instanceIndex = proxy->RequestInstanceIndex<std::memory_order_relaxed>(o->RenderType, RenderCount);
				MURenderProxyEffects::SetInstanceIndex<EEffectProxyEnum::ePointers, 0>(data.Index, instanceIndex);

				const mu_uint32 renderFlag = 1 << o->RenderType;
				const mu_uint32 modesEnabled = proxy->RegisterInstancesMode(renderFlag);

				if (modesEnabled == 0)
				{
					g_TexturesCulledList[g_TexturesCulledCount++] = o->Type;
				}

				if ((modesEnabled & renderFlag) == 0)
				{
					CULLED_DATA *cData = nullptr;

					if (o->RenderType >= BLEND::TXB_DEPTH_BEGIN &&
						o->RenderType < BLEND::TXB_DEPTH_END)
					{
						const mu_uint32 effectIndex = g_PreTexturesCulledCount++;
						cData = &g_PreTexturesCulledList[effectIndex];
					}
					else
					{
						const mu_uint32 effectIndex = g_PostTexturesCulledCount++;
						cData = &g_PostTexturesCulledList[effectIndex];
					}

					cData->Hash = RetrieveTextureHash(o->Type, o->RenderType);
				}
			}
			break;
		}
	}
	g_EffectsInstancesCount = instancesCount;
	g_EffectsCulledCount += n;
	g_JointsRenderCount = jointsCount;
}

FORCEINLINE void ConfigureSpritesInstancesIndex()
{
	mu_uint32 instancesCount = g_EffectsInstancesCount;

	const mu_uint32 guildSpritesRealCount = g_GuildSpritesCount.load(std::memory_order_relaxed);
	const mu_uint32 guildSpritesCount = instancesCount + guildSpritesRealCount <= MAX_EFFECTS_INSTANCES ? guildSpritesRealCount : MAX_EFFECTS_INSTANCES - instancesCount;
	g_GuildSpritesRenderCount = guildSpritesCount;

	for (mu_uint32 i = 0; i < guildSpritesCount; ++i)
	{
		MUGuildSprite *o = &g_GuildSprites[i];

		ETextureRenderProxy *proxy = &TEXTURES::Proxy[o->Type];
		const mu_uint32 instanceIndex = proxy->RequestInstanceIndex(GUILD_RENDERTYPE, 1);
		MURenderProxyEffects::SetInstanceIndex<EEffectProxyEnum::eGuildSprites, 0>(i, instanceIndex);

		constexpr mu_uint32 renderFlag = 1 << GUILD_RENDERTYPE;
		const mu_uint32 modesEnabled = proxy->RegisterInstancesMode(renderFlag);

		if (modesEnabled == 0)
		{
			g_TexturesCulledList[g_TexturesCulledCount++] = o->Type;
		}

		if ((modesEnabled & renderFlag) == 0)
		{
			CULLED_DATA *cData = nullptr;

			if (GUILD_RENDERTYPE >= BLEND::TXB_DEPTH_BEGIN &&
				GUILD_RENDERTYPE < BLEND::TXB_DEPTH_END)
			{
				const mu_uint32 effectIndex = g_PreTexturesCulledCount++;
				cData = &g_PreTexturesCulledList[effectIndex];
			}
			else
			{
				const mu_uint32 effectIndex = g_PostTexturesCulledCount++;
				cData = &g_PostTexturesCulledList[effectIndex];
			}

			cData->Hash = RetrieveTextureHash(o->Type, GUILD_RENDERTYPE);
		}
	}

	const mu_uint32 spritesRealCount = g_SpritesCount.load(std::memory_order_relaxed);
	const mu_uint32 spritesCount = instancesCount + spritesRealCount <= MAX_EFFECTS_INSTANCES ? spritesRealCount : MAX_EFFECTS_INSTANCES - instancesCount;
	g_SpritesRenderCount = spritesCount;
	
	for (mu_uint32 i = 0; i < spritesCount; ++i)
	{
		MUSprite *o = &g_Sprites[i];

		const mu_uint32 RenderType = GetSpriteRenderType(o->Type, o->SubType);
		ETextureRenderProxy *proxy = &TEXTURES::Proxy[o->Type];
		const mu_uint32 instanceIndex = proxy->RequestInstanceIndex(RenderType, 1);
		MURenderProxyEffects::SetInstanceIndex<EEffectProxyEnum::eSprites, 0>(i, instanceIndex);

		const mu_uint32 renderFlag = 1 << RenderType;
		const mu_uint32 modesEnabled = proxy->RegisterInstancesMode(renderFlag);

		if (modesEnabled == 0)
		{
			g_TexturesCulledList[g_TexturesCulledCount++] = o->Type;
		}

		if ((modesEnabled & renderFlag) == 0)
		{
			CULLED_DATA *cData = nullptr;

			if (RenderType >= BLEND::TXB_DEPTH_BEGIN &&
				RenderType < BLEND::TXB_DEPTH_END)
			{
				const mu_uint32 effectIndex = g_PreTexturesCulledCount++;
				cData = &g_PreTexturesCulledList[effectIndex];
			}
			else
			{
				const mu_uint32 effectIndex = g_PostTexturesCulledCount++;
				cData = &g_PostTexturesCulledList[effectIndex];
			}

			cData->Hash = RetrieveTextureHash(o->Type, RenderType);
		}
	}

	const mu_uint32 tileSpritesCount = g_TileSpritesCount.load(std::memory_order_relaxed);
	mu_uint32 i = 0;
	for (; i < tileSpritesCount; ++i)
	{
		MUTileSprite *o = &g_TileSprites[i];

		const mu_uint32 RenderCount = MUThreadQueues::PerTileSpritesRenderCount[i];
		if (instancesCount + RenderCount > MAX_EFFECTS_INSTANCES)
		{
			break;
		}

		ETextureRenderProxy *proxy = &TEXTURES::Proxy[o->Type];
		const mu_uint32 instanceIndex = proxy->RequestInstanceIndex(o->RenderType, RenderCount);
		MURenderProxyEffects::SetInstanceIndex<EEffectProxyEnum::eTileSprites, 0>(i, instanceIndex);

		const mu_uint32 renderFlag = 1 << o->RenderType;
		const mu_uint32 modesEnabled = proxy->RegisterInstancesMode(renderFlag);

		if (modesEnabled == 0)
		{
			g_TexturesCulledList[g_TexturesCulledCount++] = o->Type;
		}

		if ((modesEnabled & renderFlag) == 0)
		{
			CULLED_DATA *cData = nullptr;

			if (o->RenderType >= BLEND::TXB_DEPTH_BEGIN &&
				o->RenderType < BLEND::TXB_DEPTH_END)
			{
				const mu_uint32 effectIndex = g_PreTexturesCulledCount++;
				cData = &g_PreTexturesCulledList[effectIndex];
			}
			else
			{
				const mu_uint32 effectIndex = g_PostTexturesCulledCount++;
				cData = &g_PostTexturesCulledList[effectIndex];
			}

			cData->Hash = RetrieveTextureHash(o->Type, o->RenderType);
		}
	}
	g_TileSpritesRenderCount = i;
}

void MUTasking::RunGameVisual(const mu_uint32 ThreadIndex)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	CheckObjects(ThreadIndex);
	CheckCharacters(ThreadIndex);

	if (InChaosCastle() == false)
	{
		CheckPets(ThreadIndex);
	}

	CheckLeaves(ThreadIndex);
	CheckBoids(ThreadIndex);
	CheckFishs(ThreadIndex);
	CheckItems(ThreadIndex);
	CheckBugs(ThreadIndex);

	_ThreadBarrier->Sync();

	if (ThreadIndex == 0)
	{
		const mu_uint32 effectInstanceCount = g_EffectsCulledDataCount.load(std::memory_order_relaxed);
		std::sort(g_EffectsCulledData.begin(), g_EffectsCulledData.begin() + effectInstanceCount, EEffectCulledFastSort());
		ConfigureInstancesIndex(g_EffectsCulledData.data(), effectInstanceCount);
		g_EffectsCulledDataIndex = effectInstanceCount;
	}

	PrepareObjects(ThreadIndex);
	PrepareCharacters(ThreadIndex);

	if (InChaosCastle() == false)
	{
		PreparePets(ThreadIndex);
	}

	PrepareBoids(ThreadIndex);
	PrepareFishs(ThreadIndex);
	PrepareItems(ThreadIndex);
	PrepareBugs(ThreadIndex);

	TerrainCollision(ThreadIndex);

	_ThreadBarrier->Sync();

	if (ThreadIndex == 0)
	{
		CollisionDistance = FLT_MAX;

		for (mu_uint32 n = 0; n < _ThreadsCount; ++n)
		{
			MUThreadInfo &ti = g_ThreadInfo[n];

			if (ti.SelectFlag == true &&
				ti.CollisionDistance < CollisionDistance)
			{
				CollisionDistance = ti.CollisionDistance;
				SelectFlag = ti.SelectFlag;
				SelectXF = ti.SelectXF;
				SelectYF = ti.SelectYF;
				CollisionPosition = ti.CollisionPosition;
			}
		}
	}

	CheckEffects(ThreadIndex);

	_ThreadBarrier->Sync();

	PrepareEffects(ThreadIndex);

	_ThreadBarrier->Sync();

	CheckJoints(ThreadIndex);
	CheckParticles(ThreadIndex);
	CheckBlurs(ThreadIndex);
	CheckPointers(ThreadIndex);

	_ThreadBarrier->Sync();

	if (ThreadIndex == 0)
	{
		const mu_uint32 effectInstanceCount = g_EffectsCulledDataCount.load(std::memory_order_relaxed);
		std::sort(g_EffectsCulledData.begin() + g_EffectsCulledDataIndex, g_EffectsCulledData.begin() + effectInstanceCount, EEffectCulledFastSort());
		ConfigureInstancesIndex(g_EffectsCulledData.data() + g_EffectsCulledDataIndex, effectInstanceCount - g_EffectsCulledDataIndex);
		g_EffectsCulledDataIndex = effectInstanceCount;
	}

	_ThreadBarrier->Sync();

	PrepareJoints(ThreadIndex);

	_ThreadBarrier->Sync();

	if (ThreadIndex == 0)
	{
		ConfigureSpritesInstancesIndex();
	}

	_ThreadBarrier->Sync();

	// Warning! Draw3D functions modify MousePosition so need to be careful
	if (ThreadIndex == 0)
	{
		ThreadInfo.RenderGUI = true;
		ThreadInfo.GUIRenderIndex = 0;

		if (_RootUI != nullptr)
		{
			_RootUI->Draw3D();
		}

		g_pOptionRoot->Draw3D();
		g_pMessageRoot->Draw3D();
		g_pPickedItem.Draw3D();

		ThreadInfo.RenderGUI = false;
	}

	if (_ThreadsCount == 1)
	{
		std::sort(g_PreRenderSlotUsed, g_PreRenderSlotUsed + g_PreRenderSlotCount.load(std::memory_order_relaxed), MURenderSlotFastSort());
		std::sort(g_ClothRenderSlotUsed, g_ClothRenderSlotUsed + g_ClothRenderSlotCount.load(std::memory_order_relaxed), MUClothRenderSlotFastSort());
		std::sort(g_GUIRenderSlotUsed, g_GUIRenderSlotUsed + g_GUIRenderSlotCount, MUGUIRenderSlotFastSort());
		std::sort(g_PreTexturesCulledList, g_PreTexturesCulledList + g_PreTexturesCulledCount);
		std::sort(g_PostTexturesCulledList, g_PostTexturesCulledList + g_PostTexturesCulledCount);
	}
	else if (_ThreadsCount == 2)
	{
		switch (ThreadIndex)
		{
		case 0:
			{
				std::sort(g_PreRenderSlotUsed, g_PreRenderSlotUsed + g_PreRenderSlotCount.load(std::memory_order_relaxed), MURenderSlotFastSort());
				std::sort(g_GUIRenderSlotUsed, g_GUIRenderSlotUsed + g_GUIRenderSlotCount, MUGUIRenderSlotFastSort());
				std::sort(g_PreTexturesCulledList, g_PreTexturesCulledList + g_PreTexturesCulledCount);
			}
			break;

		case 1:
			{
				std::sort(g_ClothRenderSlotUsed, g_ClothRenderSlotUsed + g_ClothRenderSlotCount.load(std::memory_order_relaxed), MUClothRenderSlotFastSort());
				std::sort(g_PostTexturesCulledList, g_PostTexturesCulledList + g_PostTexturesCulledCount);
			}
			break;
		}
	}
	else if (_ThreadsCount == 3)
	{
		switch (ThreadIndex)
		{
		case 0:
			{
				std::sort(g_PreRenderSlotUsed, g_PreRenderSlotUsed + g_PreRenderSlotCount.load(std::memory_order_relaxed), MURenderSlotFastSort());
				std::sort(g_GUIRenderSlotUsed, g_GUIRenderSlotUsed + g_GUIRenderSlotCount, MUGUIRenderSlotFastSort());
			}
			break;

		case 1:
			{
				std::sort(g_PreTexturesCulledList, g_PreTexturesCulledList + g_PreTexturesCulledCount);
			}
			break;

		case 2:
			{
				std::sort(g_PostTexturesCulledList, g_PostTexturesCulledList + g_PostTexturesCulledCount);
				std::sort(g_ClothRenderSlotUsed, g_ClothRenderSlotUsed + g_ClothRenderSlotCount.load(std::memory_order_relaxed), MUClothRenderSlotFastSort());
			}
			break;
		}
	}
	else if (_ThreadsCount >= 4)
	{
		switch (ThreadIndex)
		{
		case 0:
			{
				std::sort(g_PreRenderSlotUsed, g_PreRenderSlotUsed + g_PreRenderSlotCount.load(std::memory_order_relaxed), MURenderSlotFastSort());
				std::sort(g_GUIRenderSlotUsed, g_GUIRenderSlotUsed + g_GUIRenderSlotCount, MUGUIRenderSlotFastSort());
			}
			break;

		case 1:
			{
				std::sort(g_PreTexturesCulledList, g_PreTexturesCulledList + g_PreTexturesCulledCount);
			}
			break;

		case 2:
			{
				std::sort(g_PostTexturesCulledList, g_PostTexturesCulledList + g_PostTexturesCulledCount);
			}
			break;

		case 3:
			{
				std::sort(g_ClothRenderSlotUsed, g_ClothRenderSlotUsed + g_ClothRenderSlotCount.load(std::memory_order_relaxed), MUClothRenderSlotFastSort());
			}
			break;
		}
	}

	_ThreadBarrier->Sync();

	if (ThreadIndex == 0)
	{
		if (battleCastle::InBattleCastle() ||
			GET_WORLD == WD_31HUNTING_GROUND)
		{
			battleCastle::InitEtcSetting();
		}
	}

	PreConfigureRenderSlots(ThreadIndex);

	_ThreadBarrier->Sync();

	if(_ThreadsCount < 2)
	{
		if (g_RenderProxySort == true)
		{
			g_RenderProxySort = false;
			std::sort(g_RenderProxyIndex, g_RenderProxyIndex + g_RenderProxyCount, g_RenderProxyFastSort);
		}

		ConfigureRenderProxy(ThreadIndex);
		MergeRenderProxy();
	}
	else
	{
		switch (ThreadIndex)
		{
		case 0:
			{
				if (g_RenderProxySort == true)
				{
					g_RenderProxySort = false;
					std::sort(g_RenderProxyIndex, g_RenderProxyIndex + g_RenderProxyCount, g_RenderProxyFastSort);
				}

				ConfigureRenderProxy(ThreadIndex);
			}
			break;

		case 1:
			{
				MergeRenderProxy();
			}
			break;
		}
	}
	
	PreConfigureTextures(ThreadIndex);

	_ThreadBarrier->Sync();

	PostConfigureRenderSlots(ThreadIndex);
	PostConfigureTextures(ThreadIndex, g_EffectsCulledCount);

	_ThreadBarrier->Sync();
}

void MUTasking::RenderTerrainVisual(const mu_uint32 ThreadIndex)
{
	if (_UpdateCounter == 0) return;

	switch (GET_WORLD)
	{
	case WD_34CRYWOLF_1ST:
		{
			EVector3 Light, Pos;
			Vector(0.30f, 0.40f, 0.20f, Light);

			mu_uint32 beginIndex, endIndex;
			GetIndexTasking(ThreadIndex, CPUCulling::BLOCK_COUNT, beginIndex, endIndex);

			for (mu_uint32 i = beginIndex; i < endIndex; ++i)
			{
				if (g_CulledBlockVisible[i] == true)
				{
					mu_uint32 minY = (i / CPUCulling::BLOCK_SIZE) * 4, maxY = ((i / CPUCulling::BLOCK_SIZE) + 1) * 4;
					mu_uint32 minX = (i % CPUCulling::BLOCK_SIZE) * 4, maxX = ((i % CPUCulling::BLOCK_SIZE) + 1) * 4;
					for (; minY < maxY; ++minY)
					{
						for (mu_uint32 x = minX; x < maxX; ++x)
						{
							mu_int32 TerrainIndex1 = TERRAIN_INDEX(x, minY);
							if ((TerrainWall[TerrainIndex1] & TW_NOGROUND) == TW_NOGROUND) continue;
							if(TerrainMappingLayer1[TerrainIndex1] != g_Terrain._WaterIndex[0] &&
								TerrainMappingLayer2[TerrainIndex1] != g_Terrain._WaterIndex[0]) continue;
							if (GetLargeRand(50) != 0) continue;

							mu_float sx = x*g_TerrainScale + (mu_float)((GetLargeRand(100) + 1)*1.0f);
							mu_float sy = minY*g_TerrainScale + (mu_float)((GetLargeRand(100) + 1)*1.0f);
							Vector(sx, sy, Hero->Object.Position[2] + 10.0f, Pos);
							CreateParticle(ThreadIndex, BITMAP_SPOT_WATER, Pos, Hero->Object.Angle, Light, 0);
						}
					}
				}
			}
		}
		break;
	}
}

void MUTasking::RunCleanCountersProcess(const mu_uint32 ThreadIndex)
{
	for (mu_uint32 i = ThreadIndex; i < g_RenderProxyCount; i += _ThreadsCount)
	{
		MURenderProxy &proxy = g_RenderProxy[i];

		proxy.InstanceCount.store(0, std::memory_order_relaxed);
		proxy.InstanceBase = 0;
		proxy.Enabled.store(false, std::memory_order_relaxed);
		proxy.ShadowEnabled.store(false, std::memory_order_relaxed);
	}

	for (mu_uint32 i = ThreadIndex; i < g_TexturesCulledCount; i += _ThreadsCount)
	{
		ETextureRenderProxy &proxy = TEXTURES::Proxy[g_TexturesCulledList[i]];

		proxy.ClearInstancesModesCount();
	}

	_ThreadBarrier->Sync();
}

#ifdef USE_DUAL_QUATERNION
mu_uint32 MUTasking::RegisterSkeletal(const EDualQuat *BoneMatrix, const mu_uint32 BoneCount)
{
	mu_uint32 skeletonIndex = _SkeletonCount.fetch_add(BoneCount);

	mu_memcpy(&_SkeletonBuffer[skeletonIndex], BoneMatrix, sizeof(EDualQuat) * BoneCount);

	return skeletonIndex;
}
#else
mu_uint32 MUTasking::RegisterSkeletal(const EMatrix4 *BoneMatrix, const mu_uint32 BoneCount)
{
	mu_uint32 skeletonIndex = _SkeletonCount.fetch_add(BoneCount);

	mu_memcpy(&_SkeletonBuffer[skeletonIndex], BoneMatrix, sizeof(EMatrix4) * BoneCount);

	return skeletonIndex;
}
#endif

void MUTasking::RenderSprite(SHEffectInstance *Instance, mu_uint32 TextureId, EVector3 Position, mu_float Width, mu_float Height, EVector3 Light, mu_float Rotation, mu_float U, mu_float V, mu_float uWidth, mu_float vHeight)
{
	MUTexture &texture = TEXTURES::Data[TextureId];

#ifdef USE_DUAL_QUATERNION
	EDualQuat billboardMatrix;
	billboardMatrix.Rotation = EQuat(EMath::ToRadians(Rotation + 180.0f) * 0.5f, Position[0], Position[1], Position[2]);
#else
	EMatrix4 billboardMatrix = MUVideoManager::_CurrentTextView;
	billboardMatrix[3] = EVector4(Position, billboardMatrix[3][3]);
	billboardMatrix = billboardMatrix * EMath::MakeMatrix(EQuat(EVector3(0.0f, 0.0f, EMath::ToRadians(Rotation + 180.0f))));
#endif

	if (texture.TextureInternal.Components == 3 ||
		TextureId == BITMAP_BLOOD + 1)
	{
		SetEffectInstance(Instance,
			billboardMatrix,
			EVector4(U, V, U + uWidth, V + vHeight),
			EVector2(Width, Height),
			MU::Float4ToARGB(Light, 1.0f),
			MU::MakeARGB(EffectSamplerMap[static_cast<mu_uint32>(texture.TextureInternal.Sampler)], TEXTURE_MODULATE, 0, 0));
	}
	else
	{
		SetEffectInstance(Instance,
			billboardMatrix,
			EVector4(U, V, U + uWidth, V + vHeight),
			EVector2(Width, Height),
			MU::Float4ToARGB(Light, Light[0]),
			MU::MakeARGB(EffectSamplerMap[static_cast<mu_uint32>(texture.TextureInternal.Sampler)], TEXTURE_MODULATE, 0, 0));
	}
}

void MUTasking::RenderSpriteADD(SHEffectInstance *Instance, mu_uint32 TextureId, EVector3 Position, mu_float Width, mu_float Height, EVector3 Light, mu_float Rotation, mu_float U, mu_float V, mu_float uWidth, mu_float vHeight)
{
	MUTexture &texture = TEXTURES::Data[TextureId];

#ifdef USE_DUAL_QUATERNION
	EDualQuat billboardMatrix;
	billboardMatrix.Rotation = EQuat(EMath::ToRadians(Rotation + 180.0f) * 0.5f, Position[0], Position[1], Position[2]);
#else
	EMatrix4 billboardMatrix = MUVideoManager::_CurrentTextView;
	billboardMatrix[3] = EVector4(Position, billboardMatrix[3][3]);
	billboardMatrix = billboardMatrix * EMath::MakeMatrix(EQuat(EVector3(0.0f, 0.0f, EMath::ToRadians(Rotation + 180.0f))));
#endif

	if (texture.TextureInternal.Components == 3 ||
		TextureId == BITMAP_BLOOD + 1)
	{
		SetEffectInstance(Instance,
			billboardMatrix,
			EVector4(U, V, U + uWidth, V + vHeight),
			EVector2(Width, Height),
			MU::Float4ToARGB(Light, 1.0f),
			MU::MakeARGB(EffectSamplerMap[static_cast<mu_uint32>(texture.TextureInternal.Sampler)], TEXTURE_ADD, 0, 0));
	}
	else
	{
		SetEffectInstance(Instance,
			billboardMatrix,
			EVector4(U, V, U + uWidth, V + vHeight),
			EVector2(Width, Height),
			MU::Float4ToARGB(Light, Light[0]),
			MU::MakeARGB(EffectSamplerMap[static_cast<mu_uint32>(texture.TextureInternal.Sampler)], TEXTURE_ADD, 0, 0));
	}
}

void MUTasking::RenderPlane3D(SHEffectInstance *Instance, mu_uint32 TextureId, EVector3 Position, mu_float Width, mu_float Height, EVector3 Light, EVector3 Angle, mu_float U, mu_float V, mu_float uWidth, mu_float vHeight)
{
	MUTexture &texture = TEXTURES::Data[TextureId];

#ifdef USE_DUAL_QUATERNION
	EDualQuat matrix(Angle, Position);
#else
	EMatrix4 matrix;
	EMath::MakeMatrix(matrix, Position, Angle);
#endif

	if (texture.TextureInternal.Components == 3 ||
		TextureId == BITMAP_BLOOD + 1)
	{
		SetEffectInstance(Instance,
			matrix,
			EVector4(U, V, U + uWidth, V + vHeight),
			EVector2(Width, Height),
			MU::Float4ToARGB(Light, 1.0f),
			MU::MakeARGB(EffectSamplerMap[static_cast<mu_uint32>(texture.TextureInternal.Sampler)], TEXTURE_MODULATE, 2, 0));
	}
	else
	{
		SetEffectInstance(Instance,
			matrix,
			EVector4(U, V, U + uWidth, V + vHeight),
			EVector2(Width, Height),
			MU::Float4ToARGB(Light, Light[0]),
			MU::MakeARGB(EffectSamplerMap[static_cast<mu_uint32>(texture.TextureInternal.Sampler)], TEXTURE_MODULATE, 2, 0));
	}
}

void MUTasking::RenderGuildSprite(SHEffectInstance *Instance, MUGuildSprite *GuildSprite)
{
	OBJECT *o = GuildSprite->Owner;

#ifdef USE_DUAL_QUATERNION
	EDualQuat Matrix(o->Angle + EVector3(80.0f, 45.0f, 135.0f));

	Matrix.Position[0] = 20.0f;
	Matrix.Position[1] = -5.0f;

	if (GuildSprite->ItemType == MODEL_ARMOR + 20)
	{
		Matrix.Position[2] = -18.0f;
	}
	else
	{
		Matrix.Position[2] = -10.0f;
	}

	if (o->BoneTransform != nullptr && o->Flags.Get(OBJECT::eOF_LIVE) == true)
	{
		GuildSprite->LatestMatrix = o->BoneTransform[MODELS::Data[o->Type].GetBoneIndex(26)];
	}

	Matrix.Mix(GuildSprite->LatestMatrix);
	Matrix.Position += o->Position;
#else
	EMatrix4 Matrix = EMath::EulerToMatrix(o->Angle + EVector3(80.0f, 45.0f, 135.0f));

	Matrix[3][0] = 20.0f;
	Matrix[3][1] = -5.0f;

	if (GuildSprite->ItemType == MODEL_ARMOR + 20)
	{
		Matrix[3][2] = -18.0f;
	}
	else
	{
		Matrix[3][2] = -10.0f;
	}

	if (o->BoneTransform != nullptr && o->Flags.Get(OBJECT::eOF_LIVE) == true)
	{
		GuildSprite->LatestMatrix = o->BoneTransform[b->GetBoneIndex(26)];
	}

	EMatrix4 ParentMatrix;
	R_ConcatTransforms(GuildSprite->LatestMatrix, Matrix, ParentMatrix);
	Matrix = EMath::Translate(o->Position) * ParentMatrix;
#endif

	SetEffectInstance(Instance,
		Matrix,
		EVector4(0.0f, 0.0f, 0.0f, 0.0f),
		EVector2(0.0f, 0.0f),
		MU::Float4ToARGB(1.0f, 1.0f, 1.0f, 1.0f),
		MU::MakeARGB(EffectSamplerMap[static_cast<mu_uint32>(ESampler::eUcVcWcMinNMagNMipN_Anisotropy)], TEXTURE_MODULATE, 6, 0));
}

void MUTasking::RenderTail(SHEffectInstance *Instance, const mu_uint32 TextureId, const EVector3 &LeftTop, const EVector3 &RightTop, const EVector3 &RightBottom, const EVector3 &LeftBottom, const EVector2 &UV, const mu_boolean UVType, const EVector4 &Light)
{
	MUTexture &texture = TEXTURES::Data[TextureId];

	SetEffectInstance(Instance,
		EMatrix4(
			EVector4(LeftTop, 0.0f),
			EVector4(LeftBottom, 0.0f),
			EVector4(RightBottom, 0.0f),
			EVector4(RightTop, 0.0f)
			),
		EVector4(UV[0], UV[0], UV[1], UV[1]),
		EVector2(0.0f, 0.0f),
		MU::Float4ToARGB(Light),
		MU::MakeARGB(EffectSamplerMap[static_cast<mu_uint32>(texture.TextureInternal.Sampler)], TEXTURE_MODULATE, 1, UVType));
}

void MUTasking::RenderBlur(SHEffectInstance *Instance, mu_uint32 TextureId, MUBlur *b, mu_uint32 c, mu_uint32 n, mu_uint32 j)
{
	MUTexture &texture = TEXTURES::Data[TextureId];

	mu_float TexU1 = j / (mu_float)b->Number;
	mu_float TexU2 = (j + 1) / (mu_float)b->Number;
	mu_float Light1 = b->Owner->Level == 0 ? (b->Number - j) / (mu_float)b->Number : 1.0f;
	mu_float Light2 = b->Owner->Level == 0 ? (b->Number - (j + 1)) / (mu_float)b->Number : 1.0f;

	SetEffectInstance(Instance,
		EMatrix4(
			EVector4(b->p1[c], TexU1),
			EVector4(b->p2[c], TexU1),
			EVector4(b->p2[n], TexU2),
			EVector4(b->p1[n], TexU2)
		),
		EVector4(0.0f, 0.0f, 0.0f, 0.0f),
		EVector2(Light1, Light2),
		MU::Float4ToARGB(b->Light, 1.0f),
		MU::MakeARGB(EffectSamplerMap[static_cast<mu_uint32>(texture.TextureInternal.Sampler)], TEXTURE_MODULATE, 3, 0));
}

void MUTasking::RenderObjectBlur(SHEffectInstance *Instance, mu_uint32 TextureId, MUObjectBlur *b, mu_uint32 c, mu_uint32 n, mu_uint32 j)
{
	MUTexture &texture = TEXTURES::Data[TextureId];

	mu_float TexU1 = j / (mu_float)b->Number;
	mu_float TexU2 = (j + 1) / (mu_float)b->Number;
	mu_float Light1 = (b->Number - j) / (mu_float)b->Number;
	mu_float Light2 = (b->Number - (j + 1)) / (mu_float)b->Number;

	SetEffectInstance(Instance,
		EMatrix4(
			EVector4(b->p1[c], TexU1),
			EVector4(b->p2[c], TexU1),
			EVector4(b->p2[n], TexU2),
			EVector4(b->p1[n], TexU2)
		),
		EVector4(0.0f, 0.0f, 0.0f, 0.0f),
		EVector2(Light1, Light2),
		MU::Float4ToARGB(b->Light, 1.0f),
		MU::MakeARGB(EffectSamplerMap[static_cast<mu_uint32>(texture.TextureInternal.Sampler)], TEXTURE_MODULATE, 3, 0));
}

FORCEINLINE void RenderTerrainBitmapTile(SHEffectInstance *Instance, const MUTexture &texture, const mu_float xf, const mu_float yf, const EVector3 c[4], const mu_boolean LightEnable, const mu_uint32 Color, const mu_float Height = 0.0f)
{
	mu_int32 xi = (mu_int32)xf;
	mu_int32 yi = (mu_int32)yf;

	if (xi < 0) xi = 0;
	else if (xi >= TERRAIN_SIZE_MASK) xi = TERRAIN_SIZE_MASK;

	if (yi < 0) yi = 0;
	else if (yi >= TERRAIN_SIZE_MASK) yi = TERRAIN_SIZE_MASK;

	mu_float sx = xf * g_TerrainScale;
	mu_float sy = yf * g_TerrainScale;

	mu_int32 TerrainIndex1 = TERRAIN_INDEX_REPEAT(xi, yi);
	mu_int32 TerrainIndex2 = TERRAIN_INDEX_REPEAT(xi + 1, yi);
	mu_int32 TerrainIndex3 = TERRAIN_INDEX_REPEAT(xi + 1, yi + 1);
	mu_int32 TerrainIndex4 = TERRAIN_INDEX_REPEAT(xi, yi + 1);

	if (LightEnable == true)
	{
		SetEffectInstance(Instance,
						  EMatrix4(
							  EVector4(sx, sy, BackTerrainHeight[TerrainIndex1] + Height, c[0][0]),
							  EVector4(0.0f, 0.0f, BackTerrainHeight[TerrainIndex2] + Height, c[1][0]),
							  EVector4(0.0f, 0.0f, BackTerrainHeight[TerrainIndex3] + Height, c[2][0]),
							  EVector4(0.0f, 0.0f, BackTerrainHeight[TerrainIndex4] + Height, c[3][0])
						  ),
						  EVector4(c[0][1], c[1][1], c[2][1], c[3][1]),
						  EVector2(xi, yi),
						  Color,
						  MU::MakeARGB(EffectSamplerMap[static_cast<mu_uint32>(texture.TextureInternal.Sampler)], TEXTURE_MODULATE, 4, 0));
	}
	else
	{
		SetEffectInstance(Instance,
						  EMatrix4(
							  EVector4(sx, sy, BackTerrainHeight[TerrainIndex1] + Height, c[0][0]),
							  EVector4(0.0f, 0.0f, BackTerrainHeight[TerrainIndex2] + Height, c[1][0]),
							  EVector4(0.0f, 0.0f, BackTerrainHeight[TerrainIndex3] + Height, c[2][0]),
							  EVector4(0.0f, 0.0f, BackTerrainHeight[TerrainIndex4] + Height, c[3][0])
						  ),
						  EVector4(c[0][1], c[1][1], c[2][1], c[3][1]),
						  EVector2(xi, yi),
						  Color,
						  MU::MakeARGB(EffectSamplerMap[static_cast<mu_uint32>(texture.TextureInternal.Sampler)], TEXTURE_MODULATE, 5, 0));
	}
}

void MUTasking::RenderTileSprite(SHEffectInstance *Instance, const mu_uint32 Texture, const mu_float xf, const mu_float yf, const mu_float SizeX, const mu_float SizeY, const mu_uint32 Light, const mu_float Rotation, const mu_float Height)
{
	MUTexture &texture = TEXTURES::Data[Texture];

	mu_float fSize = EMath::Max(SizeX, SizeY);
	mu_float Size = EMath::Floor(fSize) + 1.0f;

	EVector3 Angle;
	Vector(0.0f, 0.0f, Rotation, Angle);
	EBone Matrix;
	AngleMatrix(Angle, Matrix);

	mu_float mxf = (xf * g_TerrainScaleRev);
	mu_float myf = (yf * g_TerrainScaleRev);
	mu_int32 mxi = (mu_int32)(mxf);
	mu_int32 myi = (mu_int32)(myf);

	mu_float TexU = (((mu_float)mxi - mxf) + 0.5f*fSize);
	mu_float TexV = (((mu_float)myi - myf) + 0.5f*fSize);
	mu_float TexScaleU = 1.0f / fSize;
	mu_float TexScaleV = 1.0f / fSize;
	mu_float Aspect = SizeX / SizeY;

	for (mu_float y = -Size; y <= Size; y += 1.0f)
	{
		for (mu_float x = -Size; x <= Size; x += 1.0f)
		{
			EVector3 p1[4], p2[4];
			Vector((TexU + x)*TexScaleU, (TexV + y)*TexScaleV, 0.0f, p1[0]);
			Vector((TexU + x + 1.0f)*TexScaleU, (TexV + y)*TexScaleV, 0.0f, p1[1]);
			Vector((TexU + x + 1.0f)*TexScaleU, (TexV + y + 1.0f)*TexScaleV, 0.0f, p1[2]);
			Vector((TexU + x)*TexScaleU, (TexV + y + 1.0f)*TexScaleV, 0.0f, p1[3]);

			for (mu_int32 i = 0; i < 4; ++i)
			{
				p1[i][0] -= 0.5f;
				p1[i][1] -= 0.5f;
				VectorRotate(p1[i], Matrix, p2[i]);
				p2[i][0] *= Aspect;
				p2[i][0] += 0.5f;
				p2[i][1] += 0.5f;
			}

			RenderTerrainBitmapTile(Instance, texture, (mu_float)mxi + x, (mu_float)myi + y, p2, false, Light, Height);
			++Instance;
		}
	}
}