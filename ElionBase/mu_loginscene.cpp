 #include "stdafx.h"
#include "mu_loginscene.h"

MULoginScene::MULoginScene()
{
	_Initialized = false;
	_SwitchScene = nullptr;
}

mu_boolean MULoginScene::IsLoaded()
{
	return _Initialized;
}

void MULoginScene::OnLoad()
{
	g_CurrentScene = LOG_IN_SCENE;

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
	if (g_Terrain.Load(WD_73NEW_LOGIN_SCENE, 0) == false)
	{
		return;
	}

	MU_SAFE_DELETE(g_pCSWaterTerrain);

	MUVideoManager::Prepare2DCamera();

	mu_assert(_Camera.Load(_T("./data/loginwalkscript.cws")) == true);

	g_Tasking.SettingBoids();

	MU_CAMERA = &_Camera;

	MU_INPUT->ShowCursor(true);

	SetNetworkState(NETWORKSTATE_CONNECTING);

	if (g_NetworkClient->Connect(g_ConnectServerObject, SERVER_ADDRESS, SERVER_PORT) == false)
	{
		SetNetworkState(NETWORKSTATE_DISCONNECTED);
	}

	DontMove = false;
	SelectedCharacter = -1;
	PartyNumber = 0;
	for (mu_uint32 n = 0; n < MAX_PARTYS; ++n)
	{
		Party[n].Clear();
	}

	g_pSkillFrame->ClearSkills();

	g_pLoginRoot->_ServerMessage->Clear();
}

void MULoginScene::OnUnload()
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

	if (g_ConnectServerObject->IsConnected() == true)
	{
		g_NetworkClient->Disconnect(g_ConnectServerObject);
	}
}

#if defined(WIN32) || defined(WIN64)
#include <Psapi.h>
#endif

void MULoginScene::OnUpdateGFX(mu_uint64 CurrentTime)
{
	g_Tasking.LoginProcessGFX(CurrentTime);
}

void MULoginScene::OnUpdate(mu_double ElapsedTime, mu_uint64 CurrentTime)
{
	EMusicBackend::Play(MUSICS::Data[MUSIC_MUTHEME]);

	if (g_ConnectServerObject->IsConnected() == false &&
		g_GameServerObject->IsConnected() == false)
	{
		SetNetworkState(NETWORKSTATE_DISCONNECTED);
	}
	else if (g_pLoginRoot->IsVisible(LOGINUI::INTERFACE_SERVERLIST) == false &&
		g_GameServerObject->IsConnected() == false &&
		g_GameServerConnected == true)
	{
		g_pLoginRoot->Show(LOGINUI::INTERFACE_SERVERLIST);
		g_pLoginRoot->Hide(LOGINUI::INTERFACE_LOGINWINDOW);
		g_GameServerConnected = false;
	}

	g_Tasking.LoginProcess(ElapsedTime, CurrentTime);
}

void MULoginScene::OnPostUpdate()
{
	g_ConnectServerObject->ProcessPackets();

	if (_SwitchScene == nullptr)
	{
		g_GameServerObject->ProcessPackets();
	}
}

mu_uint32 MULoginScene::OnWindowMessage(const SDL_Event *event, mu_size *result)
{
	UISceneManager *managers[] = { g_pMessageRoot, g_pOptionRoot, g_pLoginRoot };

	mu_uint32 messageReturn = g_pMessageRoot->OnWindowMessage(event, result);
	if (messageReturn != GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE)
	{
		g_pOptionRoot->OnWindowCancelled();
		g_pLoginRoot->OnWindowCancelled();
		CheckTextInput(mu_countof(managers), managers);
		return messageReturn;
	}

	if (g_pMessageRoot->AnyVisible() == false)
	{
		messageReturn = g_pOptionRoot->OnWindowMessage(event, result);
		if (messageReturn != GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE)
		{
			g_pLoginRoot->OnWindowCancelled();
			CheckTextInput(mu_countof(managers), managers);
			return messageReturn;
		}

		if (g_pOptionRoot->AnyVisible() == false)
		{
			messageReturn = g_pLoginRoot->OnWindowMessage(event, result);
			if (messageReturn != GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE)
			{
				CheckTextInput(mu_countof(managers), managers);
				return messageReturn;
			}
		}
		else
		{
			g_pLoginRoot->OnWindowCancelled();
		}
	}
	else
	{
		g_pOptionRoot->OnWindowCancelled();
		g_pLoginRoot->OnWindowCancelled();
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
			messageReturn = g_pLoginRoot->OnPostWindowMessage(event, result);
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
		selectedObject = g_pLoginRoot->GetInternalFocused();
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

mu_boolean MULoginScene::RequireSceneSwitch()
{
	return _SwitchScene != nullptr;
}

MUSceneBase* MULoginScene::NextScene()
{
	MUSceneBase *newScene = _SwitchScene;
	_SwitchScene = nullptr;

	return newScene;
}