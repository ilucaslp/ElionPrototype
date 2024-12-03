#include "stdafx.h"

EViewport MUVideoManager::_2DViewport = { 0 };
EViewport MUVideoManager::_3DViewport = { 0 };

// View/Projection Matrices
MUCamera *MUVideoManager::_CurrentCamera = nullptr;

EMatrix4 MUVideoManager::_CurrentShadowView;
EMatrix4 MUVideoManager::_CurrentShadowProjection;

EMatrix4 MUVideoManager::_CurrentView;
EMatrix4 MUVideoManager::_CurrentInverseView;
EMatrix4 MUVideoManager::_CurrentTextView;
EDualQuat MUVideoManager::_CurrentTextViewDQ;
EMatrix4 MUVideoManager::_CurrentProjection;
EMatrix4 MUVideoManager::_CurrentRayProjection;

EMatrix4 MUVideoManager::_CurrentView2D;
EMatrix4 MUVideoManager::_CurrentProjection2D;
EMatrix4 MUVideoManager::_CurrentProjectionUI;

mu_float MUVideoManager::_LastWorldTime = -1000.0f;
mu_int32 MUVideoManager::_LastRandom = 0;

MUVideoManager::MUVideoManager()
{
}

MUVideoManager::~MUVideoManager()
{
	Shutdown();
}

mu_boolean MUVideoManager::Initialize(SDL_Window *SDLWindow)
{
	_CurrentView = EMath::Matrix4Identity();
	_CurrentInverseView = EMath::Inverse(_CurrentView);
	_CurrentProjection = EMath::Matrix4Identity();
	_CurrentRayProjection = EMath::Matrix4Identity();

	_CurrentView2D = EMath::Matrix4Identity();
	_CurrentProjection2D = EMath::Matrix4Identity();

	switch(EVideoBackend::GetBackend())
	{
#if DIRECTX_BACKEND_ENABLE == 1
	case EVideoBackendType::eDirectX:
		{
			if (EDirectXInit::Initialize(SDLWindow) == false)
			{
				return false;
			}
		}
		break;
#endif

#if OPENGL_BACKEND_ENABLE == 1
	case EVideoBackendType::eOpenGL:
		{
			if (EOpenGLInit::Initialize(SDLWindow) == false)
			{
				return false;
			}
		}
		break;
#endif

#if VULKAN_BACKEND_ENABLE == 1
	case EVideoBackendType::eVulkan:
		{
			if (EVulkanInit::Initialize(SDLWindow) == false)
			{
				return false;
			}
		}
		break;
#endif

	default: break;
	}

	ConfigureViewport2D();

	return true;
}

void MUVideoManager::Shutdown(const mu_boolean gpuOnly)
{
	switch(EVideoBackend::GetBackend())
	{
#if DIRECTX_BACKEND_ENABLE == 1
	case EVideoBackendType::eDirectX:
		{
			EDirectXInit::Destroy(gpuOnly);
		}
		break;
#endif

#if OPENGL_BACKEND_ENABLE == 1
	case EVideoBackendType::eOpenGL:
		{
			EOpenGLInit::Destroy(gpuOnly);
		}
		break;
#endif

#if VULKAN_BACKEND_ENABLE == 1
	case EVideoBackendType::eVulkan:
		{
			EVulkanInit::Destroy(gpuOnly);
		}
		break;
#endif

#if METAL_BACKEND_ENABLE == 1
	case EVideoBackendType::eMetal:
		{
		}
		break;
#endif

	default: break;
	}
}

void MUVideoManager::ConfigureViewport2D()
{
	_2DViewport.OffsetX = 0;
	_2DViewport.OffsetY = 0;
	_2DViewport.Width = MU_RESOURCE->GetWindowWidth();
	_2DViewport.Height = MU_RESOURCE->GetWindowHeight();
	_2DViewport.MinDepth = 0.0f;
	_2DViewport.MaxDepth = 1.0f;
}

void MUVideoManager::Prepare2DCamera()
{
	switch(EVideoBackend::GetBackend())
	{
#if DIRECTX_BACKEND_ENABLE == 1
	case EVideoBackendType::eDirectX:
		{
			_CurrentView2D = EMatrix4();
			_CurrentProjection2D = EMathDX::Orthographic(MU_RESOURCE->GetWindowWidth(), MU_RESOURCE->GetWindowHeight(), 0.0f, 1000.0f);
		}
		break;
#endif
#if OPENGL_BACKEND_ENABLE == 1
	case EVideoBackendType::eOpenGL:
		{
			_CurrentView2D = EMatrix4();
			_CurrentProjection2D = EMathOGL::Orthographic(MU_RESOURCE->GetWindowWidth(), MU_RESOURCE->GetWindowHeight(), 0.0f, 1000.0f);
		}
		break;
#endif
#if VULKAN_BACKEND_ENABLE == 1
	case EVideoBackendType::eVulkan:
		{
			_CurrentView2D = EMatrix4();
			_CurrentProjection2D = EMathVK::Orthographic(MU_RESOURCE->GetWindowWidth(), MU_RESOURCE->GetWindowHeight(), 0.0f, 1000.0f);
		}
		break;
#endif

	default: break;
	}
}

void MUVideoManager::OnUpdateBuffer()
{
	mu_float Random;
	if (FWorldTime - _LastWorldTime >= GAME_FPS)
	{
		Random = (mu_float)GetLargeRand(100000000);
		_LastWorldTime = FWorldTime;
		_LastRandom = Random;
	}
	else
	{
		Random = _LastRandom;
	}

	EQuat ShadowAngle = EQuat(EVector3(0.0f, 0.0f, 0.0f));

	EVector3 LightPosition = EVector3(EMath::MakeMatrix(ShadowAngle) * EVector4(1.3f, 0.0f, 2.0f, 1.0f));
	EVector3 ChromeWave = EVector3(EMath::Cos(FWorldTime * 0.001f), EMath::Sin(FWorldTime * 0.002f), 1.0f);
	EVector2 Wave = EVector2((mu_float)(IWorldTime % 10000) * 0.0001f, (mu_float)(IWorldTime % 5000) * 0.00024f - 0.4f);

	EVector4 ShadowColor;
	EVector4 ShadowPosition;
	if (GET_WORLD == WD_7ATLANSE)
	{
		ShadowColor = EVector4(0.0f, 0.0f, 0.0f, 0.2f);
	}
	else
	{
		ShadowColor = EVector4(0.0f, 0.0f, 0.0f, 1.0f);
	}

	if (GET_WORLD == WD_30BATTLECASTLE)
	{
		ShadowPosition = EVector4(2000.0f, 4000.0f, 0.0f, 0.0f);
	}
	else
	{
		ShadowPosition = EVector4(2500.0f, 4000.0f, 0.0f, 0.0f);
	}

	EMatrix4 Shadow = MUVideoManager::_CurrentShadowProjection *
					  MUVideoManager::_CurrentShadowView;
	EMatrix4 Game2D = MUVideoManager::_CurrentProjection2D *
		MUVideoManager::_CurrentView2D;
	EMatrix4 Game3D = MUVideoManager::_CurrentProjection *
		MUVideoManager::_CurrentView;

	if (EVideoBackend::GetVideoStatus(EVideoStatusFlag::VSF_Available) == true)
	{
		// Update Normal Constant Data
		{
			SHGlobalConstantBuffer *setting = EVideoBuffer::GetGlobalConstantBuffer(EGlobalConstant::eGame);

			setting->ShadowColor = ShadowColor;
			setting->ShadowPosition = ShadowPosition;

			switch (EVideoBackend::GetBackend())
			{
#if DIRECTX_BACKEND_ENABLE == 1
			case EVideoBackendType::eDirectX:
				{
					static EMatrix4 clip2Tex(
						0.5f, 0.0f, 0.0f, 0.0f,
						0.0f, -0.5f, 0.0f, 0.0f,
						0.0f, 0.0f, 1.0f, 0.0f,
						0.5f, 0.5f, 0.0f, 1.0f);
					setting->Shadow = clip2Tex * Shadow;
				}
				break;
#endif

#if OPENGL_BACKEND_ENABLE == 1
			case EVideoBackendType::eOpenGL:
				{
#if OPENGL_REVERSED_DEPTH == 1
					static EMatrix4 clip2Tex(
						0.5f, 0.0f, 0.0f, 0.0f,
						0.0f, 0.5f, 0.0f, 0.0f,
						0.0f, 0.0f, 1.0f, 0.0f,
						0.5f, 0.5f, 0.0f, 1.0f);
#else
					static EMatrix4 clip2Tex(
						0.5f, 0.0f, 0.0f, 0.0f,
						0.0f, 0.5f, 0.0f, 0.0f,
						0.0f, 0.0f, 0.5f, 0.0f,
						0.5f, 0.5f, 0.5f, 1.0f);
#endif
					setting->Shadow = clip2Tex * Shadow;
				}
				break;
#endif

#if VULKAN_BACKEND_ENABLE == 1
			case EVideoBackendType::eVulkan:
				{
					static EMatrix4 clip2Tex(
						0.5f, 0.0f, 0.0f, 0.0f,
						0.0f, 0.5f, 0.0f, 0.0f,
						0.0f, 0.0f, 1.0f, 0.0f,
						0.5f, 0.5f, 0.0f, 1.0f);
					setting->Shadow = clip2Tex * Shadow;
				}
				break;
#endif

			default: break;
			}

			setting->Game = Game3D;
			setting->UI = Game2D;
			setting->BillboardQ = MUVideoManager::_CurrentTextViewDQ.Rotation;

			setting->WorldTime = FWorldTime;
			setting->Random = Random;

			setting->LightPosition = LightPosition;
			setting->ChromeWave = ChromeWave;
			setting->Wave = Wave;

			g_Terrain.Update(setting);
		}

		// Update Shadow Constant Data
		{
			SHGlobalConstantBuffer *setting = EVideoBuffer::GetGlobalConstantBuffer(EGlobalConstant::eShadow);

			setting->ShadowColor = ShadowColor;
			setting->ShadowPosition = ShadowPosition;

			setting->Shadow = Shadow;
			setting->Game = Shadow;
			setting->UI = Game2D;
			setting->BillboardQ = MUVideoManager::_CurrentTextViewDQ.Rotation;

			setting->WorldTime = FWorldTime;
			setting->Random = Random;

			setting->LightPosition = LightPosition;
			setting->ChromeWave = ChromeWave;
			setting->Wave = Wave;

			g_Terrain.Update(setting);
		}

		// Update UI Constant Data
		{
			SHGlobalConstantBuffer *setting = EVideoBuffer::GetGlobalConstantBuffer(EGlobalConstant::eUI);

			setting->ShadowColor = ShadowColor;
			setting->ShadowPosition = ShadowPosition;

			setting->Shadow = Shadow;
			setting->Game = MUVideoManager::_CurrentProjectionUI;
			setting->UI = Game2D;
			setting->BillboardQ = MUVideoManager::_CurrentTextViewDQ.Rotation;

			setting->WorldTime = FWorldTime;
			setting->Random = Random;

			setting->LightPosition = LightPosition;
			setting->ChromeWave = ChromeWave;
			setting->Wave = Wave;

			g_Terrain.Update(setting);
		}
	}
}