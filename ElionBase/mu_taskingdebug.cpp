#include "stdafx.h"

void MUTasking::RenderDebugInformation(const mu_double elapsedTime, const mu_uint64 currentTime)
{
#define FPSLIMIT_CURVETIME 1000
#define FPSLIMIT_CURVETIMEHALF (FPSLIMIT_CURVETIME / 2)
	if (MU_GLOBALTIMER.GetLimitFPS() == false &&
		EVideoBackend::GetVideoStatus(EVideoStatusFlag::VSF_Active) == true)
	{
		mu_int64 lapseTime = (currentTime % (FPSLIMIT_CURVETIME * 2));

		if (lapseTime < FPSLIMIT_CURVETIME)
		{
			lapseTime -= FPSLIMIT_CURVETIMEHALF;
		}
		else
		{
			lapseTime = FPSLIMIT_CURVETIMEHALF - (lapseTime - FPSLIMIT_CURVETIME);
		}

		if (MU_RESOURCE->IsVerticalSync() == true)
		{
			FONT_BOLD->DrawStringC2D(TEXTINDEX::WARNING_FPSLIMIT,
				MU_TO_WINDOW(10.0f, MU_RESOURCE->_1024x768Rate[0]),
				MU_TO_WINDOW(48.0f, MU_RESOURCE->_1024x768Rate[1]),
				1.0f,
				ETA_LEFT, ETA_BOTTOM,
				0.0f,
				MU::MakeARGB(20, 255, 20, 200 + (mu_int32)(55.0f * (lapseTime / (mu_float)FPSLIMIT_CURVETIMEHALF))),
				MU::MakeARGB(0, 0, 0, 0),
				_T("Please disable Vertical Sync for disable FPS limit"));
		}
		else
		{
			FONT_BOLD->DrawStringC2D(TEXTINDEX::WARNING_FPSLIMIT,
				MU_TO_WINDOW(10.0f, MU_RESOURCE->_1024x768Rate[0]),
				MU_TO_WINDOW(48.0f, MU_RESOURCE->_1024x768Rate[1]),
				1.0f,
				ETA_LEFT, ETA_BOTTOM,
				0.0f,
				MU::MakeARGB(255, 0, 0, 200 + (mu_int32)(55.0f * (lapseTime / (mu_float)FPSLIMIT_CURVETIMEHALF))),
				MU::MakeARGB(0, 0, 0, 0),
				_T("WARNING ! FPS LIMIT IS DISABLED !\nPlease check CPU and GPU Temperature!\nBe careful, this could burn your CPU and GPU\nDoesn't recommend to play without FPS limit,\nuse it only for benchmarking"));
		}
	}
#if !defined(INTERNAL_SERVER) && !defined(__ANDROID__)
	else if (EVideoBackend::GetVideoStatus(EVideoStatusFlag::VSF_Focused) == false)
	{
		mu_int64 lapseTime = (currentTime % (FPSLIMIT_CURVETIME * 2));

		if (lapseTime < FPSLIMIT_CURVETIME)
		{
			lapseTime -= FPSLIMIT_CURVETIMEHALF;
		}
		else
		{
			lapseTime = FPSLIMIT_CURVETIMEHALF - (lapseTime - FPSLIMIT_CURVETIME);
		}

#if 1
		FONT_BOLD->DrawStringC2D(TEXTINDEX::WARNING_FPSLIMIT,
			MU_TO_WINDOW(10.0f, MU_RESOURCE->_1024x768Rate[0]),
			MU_TO_WINDOW(48.0f, MU_RESOURCE->_1024x768Rate[1]),
			1.0f,
			ETA_LEFT, ETA_BOTTOM,
			0.0f,
			MU::MakeARGB(20, 255, 20, 200 + (mu_int32)(55.0f * (lapseTime / (mu_float)FPSLIMIT_CURVETIMEHALF))),
			MU::MakeARGB(0, 0, 0, 0),
			_T("30 FPS Limited (Background Mode)"));
#endif
	}
#endif

#if 1
	static mu_double fpsCounterTime = 0.0;
	static mu_int32 fpsCounterCount = 0;
	static mu_double fpsCounterLastValue = 0.0;
	static mu_int32 fpsCounterLastCount = 0;
	static mu_text fpsCounter[256] = { 0 };

	fpsCounterTime += elapsedTime;
	++fpsCounterCount;

	if (fpsCounterTime >= 1000.0)
	{
		fpsCounterLastValue = (mu_int32)(fpsCounterTime / (mu_double)fpsCounterCount);
		fpsCounterLastCount = fpsCounterCount;
		fpsCounterCount = 0;
		fpsCounterTime = 0.0;
	}

	mu_sprintf_s(fpsCounter, mu_countof(fpsCounter), _T("Backend : ") SPF_STRING _T("\nFPS : %d (%.02f)\nFrame Time : (%.02f)"), EVideoBackend::GetBackendString(), fpsCounterLastCount, fpsCounterLastValue, elapsedTime);

#if 1
	FONT_BOLD->DrawStringC2D(TEXTINDEX::FPS_COUNTER,
		MU_TO_WINDOW(1020.0f, MU_RESOURCE->_1024x768Rate[0]),
		MU_TO_WINDOW(15.0f, MU_RESOURCE->_1024x768Rate[1]),
		1.0f,
		ETA_RIGHT, ETA_BOTTOM,
		0.0f,
		MU::MakeARGB(128, 128, 128, 255),
		MU::MakeARGB(0, 0, 0, 0),
		fpsCounter);
#endif
#endif

#if 1
	mu_text characterPos[512] = { 0 };

#if defined(DISABLE_AUDIO)
	mu_sprintf_s(characterPos,
		mu_countof(characterPos),
		_T("%d / %d\nRendering Requests : [%u|%u]\nProxies : %u\nTextures : %u\nEffects : %u\nParticles : %u\nJoints : %u\nSprites : %u\nActive Effects : %u\nRender Sprites : %u\nSkeleton Count : %u / %u\n\nMonster Type : %d\nMonster Count : %d\nSounds : %u"),
		(mu_int32)Hero->PositionX,
		(mu_int32)Hero->PositionY,
		g_PreRenderSlotCount.load(std::memory_order_relaxed),
		g_PostRenderSlotCount.load(std::memory_order_relaxed),
		g_RenderProxyCount,
		g_TexturesCulledCount,
		g_EffectsCurCount - g_EffectsFreeCount,
		g_ParticlesCurCount - g_ParticlesFreeCount,
		g_JointsCurCount - g_JointsFreeCount,
		g_SpritesCount.load(std::memory_order_relaxed),
		g_EffectsCulledCount,
		g_EffectsRenderCount.load(std::memory_order_relaxed),
		g_Tasking.GetSkeletonCount(),
		MAX_SKELETON, 0, 0, 0);
#else
	mu_sprintf_s(characterPos,
		mu_countof(characterPos),
		_T("%d / %d\nRendering Requests : [%u|%u]\nProxies : %u\nTextures : %u\nEffects : %u\nParticles : %u\nJoints : %u\nSprites : %u\nActive Effects : %u\nRender Sprites : %u\nSkeleton Count : %u / %u\n\nMonster Type : %d\nMonster Count : %d\nSounds : %u"),
		(mu_int32)Hero->PositionX,
		(mu_int32)Hero->PositionY,
		g_PreRenderSlotCount.load(std::memory_order_relaxed),
		g_PostRenderSlotCount.load(std::memory_order_relaxed),
		g_RenderProxyCount,
		g_TexturesCulledCount,
		g_EffectsCurCount - g_EffectsFreeCount,
		g_ParticlesCurCount - g_ParticlesFreeCount,
		g_JointsCurCount - g_JointsFreeCount,
		g_SpritesCount.load(std::memory_order_relaxed),
		g_EffectsCulledCount,
		g_EffectsRenderCount.load(std::memory_order_relaxed),
		g_Tasking.GetSkeletonCount(),
		MAX_SKELETON, 0, 0, g_SoundsCurCount);
#endif

#if 1
	FONT_NORMAL->DrawStringC2D(TEXTINDEX::DEBUG_GAMEINFO,
		MU_TO_WINDOW(0.0f, MU_RESOURCE->_1024x1024Rate[0]),
		MU_TO_WINDOW(0.0f, MU_RESOURCE->_1024x1024Rate[1]),
		1.0f,
		ETA_LEFT,
		ETA_BOTTOM,
		8.0f,
		MU::MakeARGB(255, 255, 255, 255),
		MU::MakeARGB(200, 0, 200, 76),
		characterPos);
#endif
#endif

#if 1
	if(MU_RESOURCE->GetShadowType() >= EShadowType::eShadowMappingBegin)
	{
		switch(EVideoBackend::GetBackend())
		{
#if DIRECTX_BACKEND_ENABLE == 1
		case EVideoBackendType::eDirectX:
			{
				static UISprite *Shadowmap = nullptr;
				if (Shadowmap == nullptr)
				{
					Shadowmap = new UISprite(nullptr, nullptr, EDirectXBuffer::GetShadowMapTexture(EDirectX::CurrentCommandBufferIndex), 640.0f, 480.0f, 0.0f, 0.0f, 128.0f, 128.0f);
				}
				if (Shadowmap != nullptr)
				{
					Shadowmap->SetTexture(EDirectXBuffer::GetShadowMapTexture(EDirectX::CurrentCommandBufferIndex));
					Shadowmap->Draw();
				}
			}
			break;
#endif
#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
			{
				static UISprite *Shadowmap = nullptr;
				if (Shadowmap == nullptr)
				{
					Shadowmap = new UISprite(nullptr, nullptr, EOpenGLBuffer::GetShadowMapTexture(EOpenGL::CurrentCommandBufferIndex), 640.0f, 480.0f, 0.0f, 0.0f, 128.0f, 128.0f);
				}
				if (Shadowmap != nullptr)
				{
					Shadowmap->SetTexture(EOpenGLBuffer::GetShadowMapTexture(EOpenGL::CurrentCommandBufferIndex));
					Shadowmap->Draw();
				}
			}
			break;
#endif
#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
			{
				static UISprite *Shadowmap = nullptr;
				if (Shadowmap == nullptr)
				{
					Shadowmap = new UISprite(nullptr, nullptr, EVulkanBuffer::GetShadowMapTexture(EVulkan::CurrentCommandBufferIndex), 640.0f, 480.0f, 0.0f, 0.0f, 128.0f, 128.0f);
				}
				if (Shadowmap != nullptr)
				{
					Shadowmap->SetTexture(EVulkanBuffer::GetShadowMapTexture(EVulkan::CurrentCommandBufferIndex));
					Shadowmap->Draw();
				}
			}
			break;
#endif
		}
	}
#endif
}