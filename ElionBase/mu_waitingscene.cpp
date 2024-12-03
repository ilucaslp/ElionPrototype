#include "stdafx.h"
#include "mu_waitingscene.h"

MUWaitingScene::MUWaitingScene()
{
	_Initialized = false;
	_SwitchScene = nullptr;

	mu_zeromem(_Sprites, sizeof(_Sprites));
	mu_zeromem(_Textures, sizeof(_Textures));
}

mu_boolean MUWaitingScene::IsLoaded()
{
	return _Initialized;
}

#include <ctime>

void MUWaitingScene::OnLoad()
{
	g_CurrentScene = WAITING_SCENE;

	_Initialized = true;

	EMusicBackend::Stop();
	g_Tasking.CleanCharacters();
	g_Tasking.CleanSounds();
	g_Tasking.CleanEffects();
	g_Tasking.CleanBoids();
	g_Tasking.CleanPets();
	g_Tasking.CleanLeaves();
	g_Tasking.CleanOperates();
	g_Tasking.CleanWorldObjects();
	g_Tasking.CleanProxy();

	g_pPickedItem.BackupPickedItem();

	mu_assert(_Textures[UIM_TS_BACK0] = MU_RESOURCE->CreateTexture2D(_T("./data/interface/lsbg01.jtex")));
	mu_assert(_Textures[UIM_TS_BACK1] = MU_RESOURCE->CreateTexture2D(_T("./data/interface/lsbg02.jtex")));
	mu_assert(_Textures[UIM_TS_BACK2] = MU_RESOURCE->CreateTexture2D(_T("./data/interface/lsbg03.jtex")));
	mu_assert(_Textures[UIM_TS_BACK3] = MU_RESOURCE->CreateTexture2D(_T("./data/interface/lsbg04.jtex")));

	mu_float W1 = 800.0f, H1 = 600.0f;

	_Sprites[UIM_TS_BACK0] = new UISprite(nullptr, nullptr, _Textures[UIM_TS_BACK0], W1, H1, 0.0f, 0.0f, 400.0f, 512.0f);
	_Sprites[UIM_TS_BACK1] = new UISprite(nullptr, nullptr, _Textures[UIM_TS_BACK1], W1, H1, 400.0f, 0.0f, 400.0f, 512.0f);
	_Sprites[UIM_TS_BACK2] = new UISprite(nullptr, nullptr, _Textures[UIM_TS_BACK2], W1, H1, 0.0f, 512.0f, 400.0f, 88.0f);
	_Sprites[UIM_TS_BACK3] = new UISprite(nullptr, nullptr, _Textures[UIM_TS_BACK3], W1, H1, 400.0f, 512.0f, 400.0f, 88.0f);
	
	MUVideoManager::Prepare2DCamera();

	MU_INPUT->ShowCursor(false);
}

void MUWaitingScene::OnUnload()
{
	//EVideoBackend::WaitForDevice();

	// Delete textures and models what won't be used on future
	for (mu_uint32 index = 0; index < UIM_TS_MAX; ++index)
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

void MUWaitingScene::OnUpdateGFX(mu_uint64 CurrentTime)
{
}

void MUWaitingScene::OnUpdate(mu_double ElapsedTime, mu_uint64 CurrentTime)
{
	if (g_GameServerObject->IsConnected() == false)
	{
		SetNetworkState(NETWORKSTATE_DISCONNECTED);
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

		g_pMessageRoot->Update(CurrentTime);
		g_pMessageRoot->UpdateGFX();
		g_pMessageRoot->Draw();
		g_pMessageRoot->PostDraw();

		EVideoBackend::EndRenderPass();
		EVideoPipeline::UnbindVAO();
	}
}

void MUWaitingScene::OnPostUpdate()
{
	if (_SwitchScene == nullptr)
	{
		g_GameServerObject->ProcessPackets();
	}
}

mu_uint32 MUWaitingScene::OnWindowMessage(const SDL_Event *event, mu_size *result)
{
	mu_uint32 messageReturn = g_pMessageRoot->OnWindowMessage(event, result);

	UISceneManager *managers[] = { g_pMessageRoot };
	CheckTextInput(mu_countof(managers), managers);

	if (messageReturn != GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE)
	{
		return messageReturn;
	}

	messageReturn = g_pMessageRoot->OnPostWindowMessage(event, result);
	if (messageReturn != GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE)
	{
		return messageReturn;
	}

	UIBase *selectedObject = g_pMessageRoot->GetInternalFocused();

	if (selectedObject != nullptr)
	{
		MU_INPUT->SetMouseOverInterface(true);
		switch (event->type)
		{
		case SDL_KEYDOWN:
		case SDL_KEYUP:
		case SDL_TEXTINPUT:
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			return GFX_MESSAGE_RETURN::GFX_MESSAGE_BYPASS;
		}
	}
	else
	{
		MU_INPUT->SetMouseOverInterface(false);
	}

	return GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE;
}

mu_boolean MUWaitingScene::RequireSceneSwitch()
{
	return _SwitchScene != nullptr;
}

MUSceneBase* MUWaitingScene::NextScene()
{
	MUSceneBase *newScene = _SwitchScene;
	_SwitchScene = nullptr;

	return newScene;
}