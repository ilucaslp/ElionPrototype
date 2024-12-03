#include "stdafx.h"
#include "mu_characterscene.h"

MUCharacterScene::MUCharacterScene()
{
	_Initialized = false;
	_SwitchScene = nullptr;
}

mu_boolean MUCharacterScene::IsLoaded()
{
	return _Initialized;
}

void MUCharacterScene::OnLoad()
{
	g_CurrentScene = CHARACTER_SCENE;

	mu_float aspectWidth = (mu_float)MU_RESOURCE->GetWindowWidth() / 640;
	mu_float aspectHeight = (mu_float)MU_RESOURCE->GetWindowHeight() / 480;

	MUVideoManager::_3DViewport.OffsetX = MU_TO_WINDOW(0, aspectWidth);
	MUVideoManager::_3DViewport.OffsetY = MU_TO_WINDOW(25, aspectHeight);
	MUVideoManager::_3DViewport.Width = MU_TO_WINDOW(640, aspectWidth);
	MUVideoManager::_3DViewport.Height = MU_TO_WINDOW(430, aspectHeight);
	MUVideoManager::_3DViewport.MinDepth = 0.0f;
	MUVideoManager::_3DViewport.MaxDepth = 1.0f;

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
	g_Tasking.ReleaseModelsMemory();

	g_pPickedItem.BackupPickedItem();

	// Event Clear
	{
		g_pBloodCastleTimer->Reset();
	}

	// Load Height-Map data & optimize
	if (g_Terrain.Load(WD_74NEW_CHARACTER_SCENE, 0) == false)
	{
		return;
	}

	MU_SAFE_DELETE(g_pCSWaterTerrain);

	MUVideoManager::Prepare2DCamera();

	_Camera.SetPosition(EVector3(9758.93f, 18913.11f, 675.5f));
	_Camera.SetAngle(EVector3(-84.5f, 0.0f, -75.0f));

	g_Tasking.SettingBoids();

#if 1
	EMusicBackend::Stop();
#else
	EMusicBackend::Play(MUSICS::Data[MUSIC_MUTHEME]);
#endif

	MU_CAMERA = &_Camera;

	MU_INPUT->ShowCursor(true);

	Hero = &CharactersClient[0];
	Hero->Object.Position = EVector3(8046.0f, 19400.0f, RequestTerrainHeight(8046.0f, 19400.0f));

	DontMove = false;
	SelectedHero = -1;
	PartyNumber = 0;
	for (mu_uint32 n = 0; n < MAX_PARTYS; ++n)
	{
		Party[n].Clear();
	}

	g_pSkillFrame->ClearSkills();

	g_pCharacterRoot->_ServerMessage->Clear();
}

void MUCharacterScene::OnUnload()
{
	//EVideoBackend::WaitForDevice();

	// Erase Height-Map data
	g_Terrain.Unload();

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

	_Initialized = false;

	MU_CAMERA = nullptr;
}

void MUCharacterScene::OnUpdateGFX(mu_uint64 CurrentTime)
{
	g_Tasking.CharacterProcessGFX(CurrentTime);
}

void MUCharacterScene::OnUpdate(mu_double ElapsedTime, mu_uint64 CurrentTime)
{
	if (g_GameServerObject->IsConnected() == false)
	{
		SetNetworkState(NETWORKSTATE_DISCONNECTED);
	}

	g_Tasking.CharacterProcess(ElapsedTime, CurrentTime);
}

void MUCharacterScene::OnPostUpdate()
{
	if (_SwitchScene == nullptr)
	{
		g_GameServerObject->ProcessPackets();
	}
}

mu_uint32 MUCharacterScene::OnWindowMessage(const SDL_Event *event, mu_size *result)
{
	UISceneManager *managers[] = { g_pMessageRoot, g_pOptionRoot, g_pCharacterRoot };

	mu_uint32 messageReturn = g_pMessageRoot->OnWindowMessage(event, result);
	if (messageReturn != GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE)
	{
		g_pOptionRoot->OnWindowCancelled();
		g_pCharacterRoot->OnWindowCancelled();
		CheckTextInput(mu_countof(managers), managers);
		return messageReturn;
	}

	if (g_pMessageRoot->AnyVisible() == false)
	{
		messageReturn = g_pOptionRoot->OnWindowMessage(event, result);
		if (messageReturn != GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE)
		{
			g_pCharacterRoot->OnWindowCancelled();
			CheckTextInput(mu_countof(managers), managers);
			return messageReturn;
		}

		if (g_pOptionRoot->AnyVisible() == false)
		{
			messageReturn = g_pCharacterRoot->OnWindowMessage(event, result);
			if (messageReturn != GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE)
			{
				CheckTextInput(mu_countof(managers), managers);
				return messageReturn;
			}
		}
		else
		{
			g_pCharacterRoot->OnWindowCancelled();
		}
	}
	else
	{
		g_pOptionRoot->OnWindowCancelled();
		g_pCharacterRoot->OnWindowCancelled();
	}

	CheckTextInput(mu_countof(managers), managers);

	messageReturn = g_pMessageRoot->OnPostWindowMessage(event, result);
	if (messageReturn != GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE)
	{
		return messageReturn;
	}

	if (g_pMessageRoot->AnyVisible() == false)
	{
		messageReturn = g_pOptionRoot->OnPostWindowMessage(event, result);
		if (messageReturn != GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE)
		{
			return messageReturn;
		}

		if (g_pOptionRoot->AnyVisible() == false)
		{
			messageReturn = g_pCharacterRoot->OnPostWindowMessage(event, result);
			if (messageReturn != GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE)
			{
				return messageReturn;
			}
		}
	}

	UIBase *selectedObject = g_pMessageRoot->GetInternalFocused();

	if (selectedObject == nullptr)
	{
		selectedObject = g_pOptionRoot->GetInternalFocused();
	}

	if (selectedObject == nullptr)
	{
		selectedObject = g_pCharacterRoot->GetInternalFocused();
	}

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

mu_boolean MUCharacterScene::RequireSceneSwitch()
{
	return _SwitchScene != nullptr;
}

MUSceneBase* MUCharacterScene::NextScene()
{
	MUSceneBase *newScene = _SwitchScene;
	_SwitchScene = nullptr;

	return newScene;
}