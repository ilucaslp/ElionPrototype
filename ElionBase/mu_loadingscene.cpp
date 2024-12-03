#include "stdafx.h"
#include "mu_loadingscene.h"

MULoadingScene::MULoadingScene()
{
	_Initialized = false;
	_IsLastRender = false;

	mu_zeromem(_Sprites, sizeof(_Sprites));
	mu_zeromem(_Textures, sizeof(_Textures));
}

mu_boolean MULoadingScene::IsLoaded()
{
	return _Initialized;
}

#include <ctime>

void MULoadingScene::OnLoad()
{
	g_CurrentScene = STARTUP_SCENE;

	_Initialized = true;

	mu_assert(_Textures[UIM_TS_BACK0] = MU_RESOURCE->CreateTexture2D(_T("./data/interface/new_lo_back_01.jtex")));
	mu_assert(_Textures[UIM_TS_BACK1] = MU_RESOURCE->CreateTexture2D(_T("./data/interface/new_lo_back_02.jtex")));
	mu_assert(_Textures[UIM_TS_BAR] = MU_RESOURCE->CreateTexture2D(_T("./data/interface/lo_lo.jtex")));
	mu_assert(_Textures[UIM_TS_BACK2] = MU_RESOURCE->CreateTexture2D(_T("./data/interface/lo_back_s5_03.jtex")));
	mu_assert(_Textures[UIM_TS_BACK3] = MU_RESOURCE->CreateTexture2D(_T("./data/interface/lo_back_s5_04.jtex")));

	switch (GetRandom(0, 2))
	{
	case 0:
		{
			mu_assert(_Textures[UIM_TS_BACK4] = MU_RESOURCE->CreateTexture2D(_T("./data/interface/lo_back_im01.jtex")));
			mu_assert(_Textures[UIM_TS_BACK5] = MU_RESOURCE->CreateTexture2D(_T("./data/interface/lo_back_im02.jtex")));
			mu_assert(_Textures[UIM_TS_BACK6] = MU_RESOURCE->CreateTexture2D(_T("./data/interface/lo_back_im03.jtex")));
			mu_assert(_Textures[UIM_TS_BACK7] = MU_RESOURCE->CreateTexture2D(_T("./data/interface/lo_back_im04.jtex")));
			mu_assert(_Textures[UIM_TS_BACK8] = MU_RESOURCE->CreateTexture2D(_T("./data/interface/lo_back_im05.jtex")));
			mu_assert(_Textures[UIM_TS_BACK9] = MU_RESOURCE->CreateTexture2D(_T("./data/interface/lo_back_im06.jtex")));
		}
		break;

	case 1:
		{
			mu_assert(_Textures[UIM_TS_BACK4] = MU_RESOURCE->CreateTexture2D(_T("./data/interface/lo_back_im01_blue.jtex")));
			mu_assert(_Textures[UIM_TS_BACK5] = MU_RESOURCE->CreateTexture2D(_T("./data/interface/lo_back_im02_blue.jtex")));
			mu_assert(_Textures[UIM_TS_BACK6] = MU_RESOURCE->CreateTexture2D(_T("./data/interface/lo_back_im03_blue.jtex")));
			mu_assert(_Textures[UIM_TS_BACK7] = MU_RESOURCE->CreateTexture2D(_T("./data/interface/lo_back_im04_blue.jtex")));
			mu_assert(_Textures[UIM_TS_BACK8] = MU_RESOURCE->CreateTexture2D(_T("./data/interface/lo_back_im05_blue.jtex")));
			mu_assert(_Textures[UIM_TS_BACK9] = MU_RESOURCE->CreateTexture2D(_T("./data/interface/lo_back_im06_blue.jtex")));
		}
		break;

	case 2:
		{
			mu_assert(_Textures[UIM_TS_BACK4] = MU_RESOURCE->CreateTexture2D(_T("./data/interface/lo_back_s5_im01.jtex")));
			mu_assert(_Textures[UIM_TS_BACK5] = MU_RESOURCE->CreateTexture2D(_T("./data/interface/lo_back_s5_im02.jtex")));
			mu_assert(_Textures[UIM_TS_BACK6] = MU_RESOURCE->CreateTexture2D(_T("./data/interface/lo_back_s5_im03.jtex")));
			mu_assert(_Textures[UIM_TS_BACK7] = MU_RESOURCE->CreateTexture2D(_T("./data/interface/lo_back_s5_im04.jtex")));
			mu_assert(_Textures[UIM_TS_BACK8] = MU_RESOURCE->CreateTexture2D(_T("./data/interface/lo_back_s5_im05.jtex")));
			mu_assert(_Textures[UIM_TS_BACK9] = MU_RESOURCE->CreateTexture2D(_T("./data/interface/lo_back_s5_im06.jtex")));
		}
		break;
	}

	mu_float W1 = 800.0f, H1 = 600.0f;
	mu_float W2 = 1280.0f, H2 = 1024.0f;

	_Sprites[UIM_TS_BACK0] = new UISprite(nullptr, nullptr, _Textures[UIM_TS_BACK0], W1, H1, 0.0f, 0.0f, 400.0f, 69.0f);
	_Sprites[UIM_TS_BACK1] = new UISprite(nullptr, nullptr, _Textures[UIM_TS_BACK1], W1, H1, 400.0f, 0.0f, 400.0f, 69.0f);
	_Sprites[UIM_TS_BACK2] = new UISprite(nullptr, nullptr, _Textures[UIM_TS_BACK2], W1, H1, 0.0f, 500.0f, 400.0f, 100.0f);
	_Sprites[UIM_TS_BACK3] = new UISprite(nullptr, nullptr, _Textures[UIM_TS_BACK3], W1, H1, 400.0f, 500.0f, 400.0f, 100.0f);
	_Sprites[UIM_TS_BACK4] = new UISprite(nullptr, nullptr, _Textures[UIM_TS_BACK4], W2, H2, 0.0f, 119.0f, 512.0f, 512.0f);
	_Sprites[UIM_TS_BACK5] = new UISprite(nullptr, nullptr, _Textures[UIM_TS_BACK5], W2, H2, 512.0f, 119.0f, 512.0f, 512.0f);
	_Sprites[UIM_TS_BACK6] = new UISprite(nullptr, nullptr, _Textures[UIM_TS_BACK6], W2, H2, 1024.0f, 119.0f, 256.0f, 512.0f);
	_Sprites[UIM_TS_BACK7] = new UISprite(nullptr, nullptr, _Textures[UIM_TS_BACK7], W2, H2, 0.0f, 631.0f, 512.0f, 223.0f);
	_Sprites[UIM_TS_BACK8] = new UISprite(nullptr, nullptr, _Textures[UIM_TS_BACK8], W2, H2, 512.0f, 631.0f, 512.0f, 223.0f);
	_Sprites[UIM_TS_BACK9] = new UISprite(nullptr, nullptr, _Textures[UIM_TS_BACK9], W2, H2, 1024.0f, 631.0f, 256.0f, 223.0f);
	_Sprites[UIM_TS_BAR] = new UISprite(nullptr, nullptr, _Textures[UIM_TS_BAR], W1, H1, 72.0f, 540.0f, 656.0f, 15.0f);

	MUVideoManager::Prepare2DCamera();

	mu_assert(g_LoadingManager.AddOnce(GAMEDATA::Load, GAMEDATA::LoadCount) == true);
	mu_assert(g_LoadingManager.AddOnce(INTERFACES::Load, INTERFACES::LoadCount) == true);
	mu_assert(g_LoadingManager.AddOnce(TEXTURES::Load, TEXTURES::LoadCount) == true);
	mu_assert(g_LoadingManager.AddOnce(TEXTURES::LoadRuntime, TEXTURES::LoadRuntimeCount) == true);
	mu_assert(g_LoadingManager.AddOnce(MODELS::Load, MODELS::LoadCount) == true);
	mu_assert(g_LoadingManager.AddOnce(SOUNDS::Load, SOUNDS::LoadCount) == true);

	MU_INPUT->ShowCursor(false);

	MU_GLOBALTIMER.SetFPS(30);
	MU_GLOBALTIMER.SetLimitFPS(true);

	_IsVerticalSync = MU_RESOURCE->IsVerticalSync();
	MU_RESOURCE->SetVerticalSync(false);
}

void MULoadingScene::OnUnload()
{
	//EVideoBackend::WaitForDevice();

	MU_GLOBALTIMER.SetFPS(60);
#if defined(__ANDROID__)
	MU_GLOBALTIMER.SetLimitFPS(false);
	MU_RESOURCE->SetVerticalSync(false);
#else
	MU_RESOURCE->SetVerticalSync(_IsVerticalSync);
#endif

	// Delete textures and models what won't be used on future
	for (mu_uint32 index = 0;index < UIM_TS_MAX; ++index)
	{
		if (_Textures[index] != nullptr)
		{
			MU_RESOURCE->DeleteTexture(_Textures[index]->_Index);
			_Textures[index] = nullptr;
		}

		MU_SAFE_DELETE(_Sprites[index]);
	}

	_Initialized = false;
}

void MULoadingScene::OnUpdateGFX(mu_uint64 CurrentTime)
{
}

void MULoadingScene::OnUpdate(mu_double ElapsedTime, mu_uint64 CurrentTime)
{
	if (g_LoadingManager.RequiredDestroy() == true &&
		g_LoadingManager.IsDestroyed() == false)
	{
		g_LoadingManager.Destroy();
		SDL_Event event;
		SDL_memset(&event, 0, sizeof(event));
		event.type = SDL_QUIT;
		SDL_PushEvent(&event);
		return;
	}

	mu_double InterfaceProgress = (mu_double)LoadingData::Counter[LOADING_COUNTER::CT_INTERFACE].load();
	mu_double TexturesProgress = (mu_double)LoadingData::Counter[LOADING_COUNTER::CT_TEXTURES].load();
	mu_double ModelsProgress = (mu_double)LoadingData::Counter[LOADING_COUNTER::CT_MODELS].load();
	mu_double SoundsProgress = (mu_double)LoadingData::Counter[LOADING_COUNTER::CT_SOUNDS].load();
	mu_double GameDataProgress = (mu_double)LoadingData::Counter[LOADING_COUNTER::CT_DATA].load();
	mu_double Progress = (InterfaceProgress + TexturesProgress + ModelsProgress + SoundsProgress + GameDataProgress) / (mu_double)g_LoadingManager.GetTotalRequest();

	_Sprites[UIM_TS_BAR]->SetDimensions(656.0f * Progress, 15.0f);

	if (Progress == 1.0)
	{
		_IsLastRender = true;

		g_Tasking.CreateInterface();
		SetMonstersSounds();
	}

	MUVideoManager::OnUpdateBuffer();

	EVideoBackend::UpdateGlobalOnly();
	EVideoBackend::EmptyTrash();

	if (EVideoBackend::GetVideoStatus(EVideoStatusFlag::VSF_Visible | EVideoStatusFlag::VSF_Available) == true)
	{
		EVideoBackend::BeginRenderPass({ { 0.0f, 0.0f, 0.0f, 1.0f } });

		EVideoBackend::SetViewport(0.0f,
			0.0f,
			MU_RESOURCE->GetWindowWidth(),
			MU_RESOURCE->GetWindowHeight());
		EVideoBackend::CleanScissor();

		EVideoBackend::BindGlobalConstant(EGlobalConstant::eUI);

		for (mu_uint32 index = 0; index < UIM_TS_MAX; ++index)
		{
			_Sprites[index]->Draw();
		}

		EVideoBackend::RenderMouse(ElapsedTime, CurrentTime);
		EVideoBackend::EndRenderPass();

		EVideoPipeline::UnbindVAO();
	}
}

mu_boolean MULoadingScene::RequireSceneSwitch()
{
	return _IsLastRender;
}

MUSceneBase* MULoadingScene::NextScene()
{
	//return &SCENES::GameScene;
	return &SCENES::LoginScene;
}