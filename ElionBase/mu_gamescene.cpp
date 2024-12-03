#include "stdafx.h"
#include "mu_gamescene.h"

MUGameScene::MUGameScene()
{
	_Initialized = false;

	_OutsideWorld = -1;
	for (mu_uint32 i = 0; i < MAX_OUTSIDES; ++i)
	{
		_Outsides[i] = nullptr;
	}

	_SwitchScene = nullptr;
	_OutsideWorld = -1;
}

mu_boolean MUGameScene::IsLoaded()
{
	return _Initialized;
}

void MUGameScene::OnLoad()
{
	g_CurrentScene = MAIN_SCENE;

	MUVideoManager::_3DViewport.OffsetX = 0;
	MUVideoManager::_3DViewport.OffsetY = 0;
	MUVideoManager::_3DViewport.Width = (mu_float)MU_RESOURCE->GetWindowWidth();
	MUVideoManager::_3DViewport.Height = (mu_float)MU_RESOURCE->GetWindowHeight() * 0.9f;
	MUVideoManager::_3DViewport.MinDepth = 0.0f;
	MUVideoManager::_3DViewport.MaxDepth = 1.0f;

	_Initialized = true;

	g_pPickedItem.BackupPickedItem();

	g_Tasking.SettingBoids();

	MUVideoManager::Prepare2DCamera();

	SettingCamera();

	MU_CAMERA = &_Camera;

	MU_INPUT->ShowCursor(true);

	DontMove = false;
}

void MUGameScene::OnUnload()
{
	//EVideoBackend::WaitForDevice();

	// Erase Height-Map data
	MU_SAFE_DELETE(g_pCSWaterTerrain);
	g_Terrain.Unload();

	for (mu_uint32 i = 0; i < MAX_OUTSIDES; ++i)
	{
		MU_SAFE_DELETE(_Outsides[i]);
	}

	_Initialized = false;

	MU_CAMERA = nullptr;
}

void MUGameScene::OnUpdateGFX(mu_uint64 CurrentTime)
{
	g_Tasking.GameProcessGFX(CurrentTime);
}

void MUGameScene::OnUpdate(mu_double ElapsedTime, mu_uint64 CurrentTime)
{
	if (g_GameServerObject->IsConnected() == false)
	{
		SetNetworkState(NETWORKSTATE_DISCONNECTED);
	}

	g_Tasking.GameProcess(ElapsedTime, CurrentTime);
}

void MUGameScene::OnPostUpdate()
{
	if (_SwitchScene == nullptr)
	{
		g_GameServerObject->ProcessPackets();
	}
}

mu_uint32 MUGameScene::OnWindowMessage(const SDL_Event *event, mu_size *result)
{
	UISceneManager *managers[] = { g_pMessageRoot, g_pOptionRoot, g_pNewUISystem };

	mu_uint32 messageReturn = g_pMessageRoot->OnWindowMessage(event, result);
	if (messageReturn != GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE)
	{
		g_pOptionRoot->OnWindowCancelled();
		g_pNewUISystem->OnWindowCancelled();
		CheckTextInput(mu_countof(managers), managers);
		return messageReturn;
	}

	if (g_pMessageRoot->AnyVisible() == false)
	{
		messageReturn = g_pOptionRoot->OnWindowMessage(event, result);
		if (messageReturn != GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE)
		{
			g_pNewUISystem->OnWindowCancelled();
			CheckTextInput(mu_countof(managers), managers);
			return messageReturn;
		}

		if (g_pOptionRoot->AnyVisible() == false)
		{
			messageReturn = g_pNewUISystem->OnWindowMessage(event, result);
			if (messageReturn != GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE)
			{
				CheckTextInput(mu_countof(managers), managers);
				return messageReturn;
			}
		}
		else
		{
			g_pNewUISystem->OnWindowCancelled();
		}
	}
	else
	{
		g_pOptionRoot->OnWindowCancelled();
		g_pNewUISystem->OnWindowCancelled();
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
			messageReturn = g_pNewUISystem->OnPostWindowMessage(event, result);
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
		selectedObject = g_pNewUISystem->GetInternalFocused();
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

mu_boolean MUGameScene::RequireSceneSwitch()
{
	return _SwitchScene != nullptr;
}

MUSceneBase* MUGameScene::NextScene()
{
	MUSceneBase *newScene = _SwitchScene;
	_SwitchScene = nullptr;

	return newScene;
}

void MUGameScene::SettingOutsides()
{
	if(GET_WORLD != _OutsideWorld)
	{
		_OutsideWorld = GET_WORLD;

		for (mu_uint32 i = 0; i < MAX_OUTSIDES; ++i)
		{
			MU_SAFE_DELETE(_Outsides[i]);
		}

		switch (GET_WORLD)
		{
		case WD_8TARKAN:
			{
				_Outsides[0] = new UISpriteUV(nullptr, nullptr, &TEXTURES::Data[BITMAP_OUTSIDE3], 640.0f, 480.0f, 0.0f, 0.0f, 640.0f, 480.0f - 45.0f, EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(76, 76, 64, 255));
				_Outsides[0]->SetBlendState(EBlendState::eOne_One);
				_Outsides[0]->SetSampler(ESampler::eUrVrWrMinLMagLMipL);
				_Outsides[1] = new UISpriteUV(nullptr, nullptr, &TEXTURES::Data[BITMAP_OUTSIDE4], 640.0f, 480.0f, 0.0f, 0.0f, 640.0f, 480.0f - 45.0f, EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(76, 76, 64, 255));
				_Outsides[1]->SetBlendState(EBlendState::eOne_One);
				_Outsides[1]->SetSampler(ESampler::eUrVrWrMinLMagLMipL);
			}
			break;

		case WD_30BATTLECASTLE:
			{
				_Outsides[0] = new UISpriteUV(nullptr, nullptr, &TEXTURES::Data[BITMAP_OUTSIDE2], 640.0f, 480.0f, 0.0f, 0.0f, 640.0f, 480.0f - 45.0f, EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(76, 76, 64, 255));
				_Outsides[0]->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
				_Outsides[0]->SetSampler(ESampler::eUrVrWrMinLMagLMipL);
				_Outsides[1] = new UISpriteUV(nullptr, nullptr, &TEXTURES::Data[BITMAP_OUTSIDE1], 640.0f, 480.0f, 0.0f, 0.0f, 640.0f, 480.0f - 45.0f, EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(76, 76, 64, 255));
				_Outsides[1]->SetBlendState(EBlendState::eOne_One);
				_Outsides[1]->SetSampler(ESampler::eUrVrWrMinLMagLMipL);
			}
			break;

		case WD_34CRYWOLF_1ST:
			{
				_Outsides[0] = new UISpriteUV(nullptr, nullptr, &TEXTURES::Data[BITMAP_OUTSIDE2], 640.0f, 480.0f, 0.0f, 0.0f, 640.0f, 480.0f - 45.0f, EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(102, 102, 115, 255));
				_Outsides[0]->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
				_Outsides[0]->SetSampler(ESampler::eUrVrWrMinLMagLMipL);
				_Outsides[1] = new UISpriteUV(nullptr, nullptr, &TEXTURES::Data[BITMAP_OUTSIDE1], 640.0f, 480.0f, 0.0f, 0.0f, 640.0f, 480.0f - 45.0f, EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(102, 102, 115, 255));
				_Outsides[1]->SetBlendState(EBlendState::eOne_One);
				_Outsides[1]->SetSampler(ESampler::eUrVrWrMinLMagLMipL);
			}
			break;

		case WD_56MAP_SWAMP_OF_QUIET:
			{
				_Outsides[0] = new UISpriteUV(nullptr, nullptr, &TEXTURES::Data[BITMAP_OUTSIDE2], 640.0f, 480.0f, 0.0f, 0.0f, 640.0f, 480.0f - 45.0f, EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(102, 102, 115, 255));
				_Outsides[0]->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
				_Outsides[0]->SetSampler(ESampler::eUrVrWrMinLMagLMipL);
				_Outsides[1] = new UISpriteUV(nullptr, nullptr, &TEXTURES::Data[BITMAP_OUTSIDE1], 640.0f, 480.0f, 0.0f, 0.0f, 640.0f, 480.0f - 45.0f, EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(102, 102, 115, 255));
				_Outsides[1]->SetBlendState(EBlendState::eOne_One);
				_Outsides[1]->SetSampler(ESampler::eUrVrWrMinLMagLMipL);
			}
			break;

		case WD_57ICECITY:
		case WD_58ICECITY_BOSS:
		case WD_65DOPPLEGANGER1:
			{
				_Outsides[0] = new UISpriteUV(nullptr, nullptr, &TEXTURES::Data[BITMAP_OUTSIDE2], 640.0f, 480.0f, 0.0f, 0.0f, 640.0f, 480.0f - 45.0f, EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(102, 102, 115, 255));
				_Outsides[0]->SetBlendState(EBlendState::eOne_One);
				_Outsides[0]->SetSampler(ESampler::eUrVrWrMinLMagLMipL);
				_Outsides[1] = new UISpriteUV(nullptr, nullptr, &TEXTURES::Data[BITMAP_OUTSIDE1], 640.0f, 480.0f, 0.0f, 0.0f, 640.0f, 480.0f - 45.0f, EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(102, 102, 115, 255));
				_Outsides[1]->SetBlendState(EBlendState::eOne_One);
				_Outsides[1]->SetSampler(ESampler::eUrVrWrMinLMagLMipL);
			}
			break;

		case WD_69EMPIREGUARDIAN1:
		case WD_70EMPIREGUARDIAN2:
		case WD_71EMPIREGUARDIAN3:
		case WD_72EMPIREGUARDIAN4:
			{
				_Outsides[0] = new UISpriteUV(nullptr, nullptr, &TEXTURES::Data[BITMAP_OUTSIDE0], 640.0f, 480.0f, 0.0f, 0.0f, 640.0f, 480.0f - 45.0f, EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(153, 153, 229, 255));
				_Outsides[0]->SetBlendState(EBlendState::eOne_One);
				_Outsides[0]->SetSampler(ESampler::eUrVrWrMinLMagLMipL);
				_Outsides[1] = new UISpriteUV(nullptr, nullptr, &TEXTURES::Data[BITMAP_OUTSIDE1], 640.0f, 480.0f, 0.0f, 0.0f, 640.0f, 480.0f - 45.0f, EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(153, 153, 229, 255));
				_Outsides[1]->SetBlendState(EBlendState::eOne_One);
				_Outsides[1]->SetSampler(ESampler::eUrVrWrMinLMagLMipL);
			}
			break;
		}
	}
}

void MUGameScene::RenderOutsides()
{
	if (GET_WORLD != _OutsideWorld) return;

	switch (GET_WORLD)
	{
	case WD_34CRYWOLF_1ST:
		{
			if(g_Ashies == true)
			{
				mu_float WindX = (mu_float)(IWorldTime % 100000)*0.0005f;
				_Outsides[0]->SetUV(EVector2(WindX, 0.0f), EVector2(3.0f, 2.0f));
				WindX = (mu_float)(IWorldTime % 100000)*0.0002f;
				_Outsides[1]->SetUV(EVector2(WindX, 0.0f), EVector2(0.3f, 0.3f));

				for (mu_uint32 i = 0; i < MAX_OUTSIDES; ++i)
				{
					if (_Outsides[i] != nullptr)
					{
						_Outsides[i]->Draw();
					}
				}

				g_Ashies = false;
			}
		}
		break;

	case WD_30BATTLECASTLE:
		{
			if (battleCastle::IsBattleCastleStart() == true)
			{
				mu_float WindX = (mu_float)(IWorldTime % 100000)*0.0005f;
				_Outsides[0]->SetUV(EVector2(WindX, 0.0f), EVector2(3.0f, 2.0f));
				WindX = (mu_float)(IWorldTime % 100000)*0.0002f;
				_Outsides[1]->SetUV(EVector2(WindX, 0.0f), EVector2(0.3f, 0.3f));

				for (mu_uint32 i = 0; i < MAX_OUTSIDES; ++i)
				{
					if (_Outsides[i] != nullptr)
					{
						_Outsides[i]->Draw();
					}
				}
			}
		}
		break;

	case WD_69EMPIREGUARDIAN1:
	case WD_70EMPIREGUARDIAN2:
	case WD_71EMPIREGUARDIAN3:
	case WD_72EMPIREGUARDIAN4:
		{
			mu_int32 weather = g_EmpireGuardian1.GetWeather();
			switch (weather)
			{
			case GMEmpireGuardian1::WEATHER_TYPE::WEATHER_FOG:
				{
					mu_float WindX2 = (mu_float)(IWorldTime % 100000)*0.00005f;
					mu_float WindY2 = (mu_float)(IWorldTime % 100000)*0.00008f;
					_Outsides[0]->SetUV(EVector2(WindX2, WindY2), EVector2(2.0f, 2.0f));
					mu_float WindX = -(mu_float)(IWorldTime % 100000)*0.00005f;
					_Outsides[1]->SetUV(EVector2(WindX, 0.0f), EVector2(0.3f, 0.3f));
				}
				break;

			case GMEmpireGuardian1::WEATHER_TYPE::WEATHER_STORM:
				{
					if (GetLargeRand(20) == 0)
					{
						mu_float WindX2 = (mu_float)(IWorldTime % 100000)*0.0006f;
						mu_float WindY2 = -(mu_float)(IWorldTime % 100000)*0.0006f;
						_Outsides[0]->SetUV(EVector2(WindX2, WindY2), EVector2(3.0f, 2.0f));
						WindX2 = -(mu_float)(IWorldTime % 100000)*0.0006f;
						WindY2 = (mu_float)(IWorldTime % 100000)*0.0006f;
						_Outsides[1]->SetUV(EVector2(WindX2, WindY2), EVector2(3.0f, 2.0f));
					}
				}
				break;
			}

			if(weather == GMEmpireGuardian1::WEATHER_TYPE::WEATHER_FOG ||
				weather == GMEmpireGuardian1::WEATHER_TYPE::WEATHER_STORM)
			{
				for (mu_uint32 i = 0; i < MAX_OUTSIDES; ++i)
				{
					if (_Outsides[i] != nullptr)
					{
						_Outsides[i]->Draw();
					}
				}
			}
		}
		break;

	default:
		{
			switch (GET_WORLD)
			{
			case WD_8TARKAN:
				{
					mu_float WindX = (mu_float)(IWorldTime % 100000)*0.0002f;
					_Outsides[0]->SetUV(EVector2(WindX, 0.0f), EVector2(0.3f, 0.3f));
					WindX = (mu_float)(IWorldTime % 100000)*0.001f;
					_Outsides[1]->SetUV(EVector2(WindX, 0.0f), EVector2(3.0f, 2.0f));
				}
				break;

			case WD_56MAP_SWAMP_OF_QUIET:
				{
					mu_float WindX = (mu_float)(IWorldTime % 100000)*0.0005f;
					_Outsides[0]->SetUV(EVector2(WindX, 0.0f), EVector2(3.0f, 2.0f));
					WindX = (mu_float)(IWorldTime % 100000)*0.0002f;
					_Outsides[1]->SetUV(EVector2(WindX, 0.0f), EVector2(0.3f, 0.3f));
				}
				break;

			case WD_57ICECITY:
			case WD_58ICECITY_BOSS:
			case WD_65DOPPLEGANGER1:
				{
					mu_float WindX2 = (mu_float)(IWorldTime % 100000)*0.0006f;
					mu_float WindY2 = -(mu_float)(IWorldTime % 100000)*0.0006f;
					_Outsides[0]->SetUV(EVector2(WindX2, WindY2), EVector2(3.0f, 2.0f));
					mu_float WindX = (mu_float)(IWorldTime % 100000)*0.0001f;
					_Outsides[1]->SetUV(EVector2(WindX, 0.0f), EVector2(0.3f, 0.3f));
				}
				break;
			}

			for (mu_uint32 i = 0; i < MAX_OUTSIDES; ++i)
			{
				if (_Outsides[i] != nullptr)
				{
					_Outsides[i]->Draw();
				}
			}
		}
		break;
	}
}

void MUGameScene::SettingCamera()
{
	_Camera.ForceUpdate();

	switch (GET_WORLD)
	{
	case WD_30BATTLECASTLE:
		{
			_Camera.SetAngles(225.0f, 139.61f);
			_Camera.SetRadius(1517.02026f);
		}
		break;

	default:
		{
			_Camera.SetAngles(225.0f, 139.61f);
			_Camera.SetRadius(1313.43750f);
		}
		break;
	}
}