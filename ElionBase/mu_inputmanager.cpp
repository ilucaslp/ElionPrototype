#include "stdafx.h"

mu_uint32 CURSOR::Texture[CURSOR_MAX] = {
	INTERFACES::CURSOR_NORMAL,
	INTERFACES::CURSOR_PUSH,
	INTERFACES::CURSOR_ATTACK,
	INTERFACES::CURSOR_ATTACK2,
	INTERFACES::CURSOR_GET,
	INTERFACES::CURSOR_TALK,
	INTERFACES::CURSOR_REPAIR,
	INTERFACES::CURSOR_REPAIR,
	INTERFACES::CURSOR_LEANAGAINST,
	INTERFACES::CURSOR_SITDOWN,
	INTERFACES::CURSOR_DONTMOVE,
	INTERFACES::CURSOR_IDSELECT,
};

mu_uint8 g_MouseFastCmp[MOUSE_BUTTON_MAX] = { 0 };

MUInputManager::MUInputManager()
{
	_CaptureChanged = false;
	_ShowCursor = false;
	_Cursor = nullptr;
	_CursorIndex = CURSOR::CURSOR_NORMAL;

	_MouseOverInterface = false;
	_MouseWheel = 0.0f;
	mu_zeromem(g_MouseFastCmp, sizeof(g_MouseFastCmp));
	mu_zeromem(_MouseState, sizeof(_MouseState));
	mu_zeromem(_MouseUsed, sizeof(_MouseUsed));
	_Is3DMouse = false;

	mu_zeromem(_KeyPressed, sizeof(_KeyState));
	mu_zeromem(_KeyState, sizeof(_KeyState));
}

MUInputManager::~MUInputManager()
{
	Destroy();
}

void MUInputManager::Destroy()
{
	MU_SAFE_DELETE(_Cursor);
}

void MUInputManager::Update()
{
}

void MUInputManager::PostUpdate()
{
	_CaptureChanged = false;
	_MouseWheel = 0.0f;
}

void MUInputManager::RenderCursor(mu_uint64 CurrentTime)
{
	if (_ShowCursor == true)
	{
		if (_Cursor == nullptr)
		{
			_Cursor = new UISprite(nullptr, nullptr, nullptr, 640.0f, 480.0f, 0.0f, 0.0f, 24.0f, 24.0f);
			_Cursor->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
			_Cursor->SetDepthStencil(EDepthStencil::eNoZBuffer);
		}

		if(_Cursor != nullptr)
		{
			mu_float u = 0.0f;
			mu_float v = 0.0;
			mu_int32 Frame = (mu_int32)(FWorldTime * 0.01f) % 6;
			if (Frame == 1 || Frame == 3 || Frame == 5) u = 0.5f;
			if (Frame == 2 || Frame == 3 || Frame == 4) v = 0.5f;

			mu_float aspectWidth = MUVideoManager::_2DViewport.Width / 640.0f;
			mu_float aspectHeight = MUVideoManager::_2DViewport.Height / 480.0f;
			mu_float mouseX = (mu_float)_MouseX / aspectWidth;
			mu_float mouseY = (mu_float)_MouseY / aspectHeight;

			mu_uint32 cursorIndex = CURSOR::CURSOR_NORMAL;

			if (g_pMessageRoot->AnyVisible() == true ||
				g_pOptionRoot->AnyVisible() == true ||
				g_CurrentScene != MAIN_SCENE)
			{
				if (IsMousePressing(MOUSE_BUTTON_LEFT) == false)
				{
					cursorIndex = CURSOR::CURSOR_NORMAL;
				}
				else
				{
					cursorIndex = CURSOR::CURSOR_PUSH;
				}
			}
			else if (IsMouseOverInterface() == true)
			{
				if (g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_COMMAND))
				{
					if (g_pCommandWindow->GetCommandSelected() != -1)
					{
						cursorIndex = CURSOR::CURSOR_IDSELECT;
					}
				}
				else if ((g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_INVENTORY)
					&& g_pMyInventory->GetRepairMode() == true)
					|| (g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_NPCSHOP)
						&& g_pNpcShopInventory->GetShopState() == UIShopInventory::SHOP_REPAIR)
					)
				{
					cursorIndex = CURSOR::CURSOR_REPAIR;
				}
				else if (IsMousePressing(MOUSE_BUTTON_LEFT) == false)
				{
					cursorIndex = CURSOR::CURSOR_NORMAL;
				}
				else
				{
					cursorIndex = CURSOR::CURSOR_PUSH;
				}
			}
#if 0 // FIX
			else if (g_iKeyPadEnable ||
				ErrorMessage)
			{
				cursorIndex = CURSOR::CURSOR_NORMAL;
			}
#endif
			else if (SelectedItem != -1)
			{
				cursorIndex = CURSOR::CURSOR_GET;
			}
			else if (SelectedNpc != -1)
			{
				if (M38Kanturu2nd::Is_Kanturu2nd())
				{
					cursorIndex = CURSOR::CURSOR_ATTACK2;
				}
				else
				{
					cursorIndex = CURSOR::CURSOR_TALK;
				}
			}
			else if (SelectedOperate != -1)
			{
				if ((GET_WORLD == WD_0LORENCIA && g_Operates[SelectedOperate].Owner->Type == MODEL_POSE_BOX) ||
					(GET_WORLD == WD_1DUNGEON && g_Operates[SelectedOperate].Owner->Type == 60) ||
					(GET_WORLD == WD_2DEVIAS && g_Operates[SelectedOperate].Owner->Type == 91) ||
					(GET_WORLD == WD_3NORIA && g_Operates[SelectedOperate].Owner->Type == 38))
				{
					cursorIndex = CURSOR::CURSOR_LEANAGAINST;
				}
				else
				{
					cursorIndex = CURSOR::CURSOR_SITDOWN;
				}
			}
			else if (g_CurrentScene == MAIN_SCENE &&
				!Hero->SafeZone &&
				SelectedCharacter != -1)
			{
				if (CheckAttack())
				{
					if (battleCastle::InBattleCastle())
					{
						cursorIndex = CURSOR::CURSOR_ATTACK2;
					}
					else
					{
						cursorIndex = CURSOR::CURSOR_ATTACK;
					}
				}
				else
				{
					cursorIndex = CURSOR::CURSOR_NORMAL;
				}
			}
			else if (g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_COMMAND))
			{
				if (g_pCommandWindow->GetCommandSelected() != -1)
				{
					cursorIndex = CURSOR::CURSOR_IDSELECT;
				}
			}
#if 0 // FIX
			else if (RepairEnable == 2)
			{
				if (EMath::Sin(FWorldTime*0.02f) > 0)
				{
					RenderBitmapRotate(BITMAP_CURSOR + 5, (mu_float)MouseX + 10.0f, (mu_float)MouseY + 10.0f, 24.0f, 24.0f, 0.0f);
				}
				else
				{
					RenderBitmapRotate(BITMAP_CURSOR + 5, (mu_float)MouseX + 5.0f, (mu_float)MouseY + 18.0f, 24.0f, 24.0f, 45.0f);
				}
			}
#endif
			else
			{
				if (IsMousePressing(MOUSE_BUTTON_LEFT) == false)
				{
					cursorIndex = CURSOR::CURSOR_NORMAL;
				}
				else
				{
					if (DontMove)
					{
						cursorIndex = CURSOR::CURSOR_DONTMOVE;
					}
					else
					{
						cursorIndex = CURSOR::CURSOR_PUSH;
					}
				}
			}

			_Cursor->SetRotation(0.0f);
			_Cursor->SetTexture(&INTERFACES::Data[CURSOR::Texture[cursorIndex]]);

			mu_float widthRatio = MU_RESOURCE->_640x480Rate[0];
			mu_float heightRatio = MU_VIEWPORT_RATE(MU_RESOURCE->GetWindowWidth() * 0.75f, 480.0f);

			_Cursor->SetDimensions(MU_TO_WINDOW(18.0f, widthRatio), MU_TO_WINDOW(18.0f, heightRatio), false);

			switch (cursorIndex)
			{
			case CURSOR::CURSOR_NORMAL:
				{
					_Cursor->SetPosition(mouseX - 2.0f, mouseY - 2.0f);
					_Cursor->SetUV(EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f));
				}
				break;
			case CURSOR::CURSOR_PUSH:
				{
					_Cursor->SetPosition(mouseX - 2.0f, mouseY - 2.0f);
					_Cursor->SetUV(EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f));
				}
				break;
			case CURSOR::CURSOR_ATTACK:
				{
					_Cursor->SetPosition(mouseX - 2.0f, mouseY - 2.0f);
					_Cursor->SetUV(EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f));
				}
				break;
			case CURSOR::CURSOR_ATTACK2:
				{
					_Cursor->SetPosition(mouseX - 2.0f, mouseY - 2.0f);
					_Cursor->SetUV(EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f));
				}
				break;
			case CURSOR::CURSOR_GET:
				{
					_Cursor->SetPosition(mouseX - 2.0f, mouseY - 2.0f);
					_Cursor->SetUV(EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f));
				}
				break;
			case CURSOR::CURSOR_TALK:
				{
					_Cursor->SetPosition(mouseX - 2.0f, mouseY - 2.0f);
					_Cursor->SetUV(EVector2(u, v), EVector2(u + 0.5f, v + 0.5f));
				}
				break;
			case CURSOR::CURSOR_REPAIR:
				{
					if (IsMousePressing(MOUSE_BUTTON_LEFT) == false)
					{
						_Cursor->SetPosition(mouseX - 2.0f, mouseY - 2.0f);
						_Cursor->SetUV(EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f));
					}
					else
					{
						_Cursor->SetPosition(mouseX + 5.0f, mouseY + 18.0f);
						_Cursor->SetUV(EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f));
						_Cursor->SetRotation(45.0f);
					}
				}
				break;
			case CURSOR::CURSOR_REPAIR_ANIM:
				{
					if(EMath::Sin(FWorldTime * 0.02f) > 0.0f)
					{
						_Cursor->SetPosition(mouseX + 10.0f, mouseY + 10.0f);
						_Cursor->SetUV(EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f));
					}
					else
					{
						_Cursor->SetPosition(mouseX + 5.0f, mouseY + 18.0f);
						_Cursor->SetUV(EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f));
						_Cursor->SetRotation(45.0f);
					}
				}
				break;
			case CURSOR::CURSOR_LEANAGAINST:
				{
					_Cursor->SetPosition(mouseX - 2.0f, mouseY - 2.0f);
					_Cursor->SetUV(EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f));
				}
				break;
			case CURSOR::CURSOR_SITDOWN:
				{
					_Cursor->SetPosition(mouseX - 2.0f, mouseY - 2.0f);
					_Cursor->SetUV(EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f));
				}
				break;
			case CURSOR::CURSOR_DONTMOVE:
				{
					_Cursor->SetPosition(mouseX - 2.0f, mouseY - 2.0f);
					_Cursor->SetUV(EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f));
				}
				break;
			case CURSOR::CURSOR_IDSELECT:
				{
					_Cursor->SetPosition(mouseX - 2.0f, mouseY - 2.0f);
					_Cursor->SetUV(EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f));
				}
				break;
			}

			_Cursor->Update(CurrentTime);
			_Cursor->Draw();
		}
	}
}

void MUInputManager::ShowCursor(mu_boolean show)
{
	_ShowCursor = show;
}

void MUInputManager::SystemConfigure()
{
	if (_Cursor != nullptr)
	{
		_Cursor->SystemConfigure();
	}
}

mu_boolean MUInputManager::IsCaptureChanged()
{
	return _CaptureChanged;
}

void MUInputManager::SetMouseOverInterface(mu_boolean isOver)
{
	_MouseOverInterface = isOver;
}

mu_boolean MUInputManager::IsMouseOverInterface()
{
	return _MouseOverInterface;
}

void MUInputManager::ClearKeyState()
{
	mu_zeromem(_KeyPressed, sizeof(_KeyPressed));
	mu_zeromem(_KeyState, sizeof(_KeyState));
}

mu_boolean MUInputManager::GetKeyState(mu_uint32 Key)
{
	return _KeyPressed[Key];
}

void MUInputManager::SetKeyDown(mu_uint32 Key)
{
	_KeyPressed[Key] = true;
	_KeyState[Key] = 0;
}

void MUInputManager::SetKeyUp(mu_uint32 Key)
{
	_KeyPressed[Key] = false;
}

void MUInputManager::ProcessKeys()
{
	mu_memcpy(_KeyState, _KeyPressed, sizeof(_KeyState));
}

mu_boolean MUInputManager::IsKeyPressed(mu_int32 Key)
{
	return (_KeyPressed[Key] == true && _KeyState[Key] == 0);
}

mu_boolean MUInputManager::IsKeyPressing(mu_int32 Key)
{
	return (_KeyPressed[Key] == true);
}

mu_boolean MUInputManager::IsKeyHolding(mu_int32 Key)
{
	return (_KeyPressed[Key] == true && _KeyState[Key] == 1);
}

mu_boolean MUInputManager::IsKeyReleased(mu_int32 Key)
{
	return (_KeyPressed[Key] == false && _KeyState[Key] == 1);
}

void MUInputManager::ClearMouseButton()
{
	for (mu_uint32 n = 0; n < MOUSE_BUTTON_MAX; ++n)
	{
		_MouseState[n] = MOUSE_STATE_NONE;
		_MouseUsed[n] = MOUSE_USED_NONE;
	}
}

void MUInputManager::SetMouse(mu_int32 mouse_x, mu_int32 mouse_y)
{
	_MouseX = mouse_x, _MouseY = mouse_y;
}

void MUInputManager::SetMouseButton(mu_uint32 index, mu_uint8 state)
{
	_MouseState[index] = state;
	_MouseUsed[index] = MOUSE_USED_NONE;
}

void MUInputManager::SetMouseUsed(mu_uint32 index, mu_uint8 state)
{
	if (_MouseState[index] > MOUSE_STATE_NONE)
	{
		_MouseUsed[index] = state;
	}
}

void MUInputManager::SetMouseWheel(mu_int32 mouse_wheel)
{
	_MouseWheel += (mu_float)mouse_wheel;
}

void MUInputManager::Set3DMouse(mu_boolean is3D)
{
	_Is3DMouse = is3D;
}

mu_boolean MUInputManager::IsAnyMousePressed()
{
	return mu_memcmp(g_MouseFastCmp, _MouseState, sizeof(g_MouseFastCmp)) != 0;
}

mu_boolean MUInputManager::IsMousePressed(mu_int32 index)
{
	return _MouseState[index] == MOUSE_STATE_CLICK;
}

mu_boolean MUInputManager::IsMouseDoublePressed(mu_int32 index)
{
	return _MouseState[index] == MOUSE_STATE_DOUBLECLICK;
}

mu_boolean MUInputManager::IsMousePressing(mu_uint32 index)
{
	return _MouseState[index] != MOUSE_STATE_NONE;
}

mu_boolean MUInputManager::Is3DMouse()
{
	return _Is3DMouse;
}

mu_boolean MUInputManager::Is3DMousePressed(mu_int32 index)
{
	return _Is3DMouse == true && _MouseState[index] == MOUSE_STATE_CLICK && _MouseUsed[index] < MOUSE_USED_PRESSED;
}

mu_boolean MUInputManager::Is3DMouseDoublePressed(mu_int32 index)
{
	return _Is3DMouse == true && _MouseState[index] == MOUSE_STATE_DOUBLECLICK && _MouseUsed[index] < MOUSE_USED_PRESSED;
}

mu_boolean MUInputManager::Is3DMousePressedAny(mu_int32 index)
{
	return _Is3DMouse == true && _MouseState[index] != MOUSE_STATE_NONE && _MouseUsed[index] < MOUSE_USED_PRESSED;
}

mu_boolean MUInputManager::Is3DMousePressing(mu_uint32 index)
{
	return _Is3DMouse == true && _MouseState[index] != MOUSE_STATE_NONE && _MouseUsed[index] < MOUSE_USED_ALL;
}

mu_int32 MUInputManager::GetMouseX()
{
	return _MouseX;
}

mu_int32 MUInputManager::GetMouseY()
{
	return _MouseY;
}

mu_float MUInputManager::GetMouseWheel()
{
	return _MouseWheel;
}