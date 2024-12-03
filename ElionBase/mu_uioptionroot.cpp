#include "stdafx.h"
#include "mu_uioptionroot.h"

#define SCREEN_WIDTH 640.0f
#define SCREEN_HEIGHT 480.0f

UIOptionRoot *g_pOptionRoot = nullptr;

UIOptionRoot::UIOptionRoot()
{
	_SceneManager = nullptr;

	UIBase::SetAspectRatio(SCREEN_WIDTH, SCREEN_HEIGHT);
	UIBase::SetPosition(0.0f, 0.0f);
	UIBase::SetDimensions(SCREEN_WIDTH, SCREEN_HEIGHT);

	_SelectedObject = nullptr;
	_FocusObject = nullptr;

	SetRoot(this);

	_Background = new UISpriteColor(this, nullptr, MU::MakeARGB(0, 0, 0, 128), 640.0f, 480.0f, 0.0f, 0.0f, 640.0f, 480.0f);
	_Background->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

	_LoginMenu = new UILoginMenu(this);
	AddObject(OPTIONUI::INTERFACE_LOGINMENU, _LoginMenu);

	_CharacterMenu = new UICharacterMenu(this);
	AddObject(OPTIONUI::INTERFACE_CHARACTERMENU, _CharacterMenu);

	_GameMenu = new UIGameMenu(this);
	AddObject(OPTIONUI::INTERFACE_GAMEMENU, _GameMenu);

	_OptionWindow = new UIOptionWindow(this);
	AddObject(OPTIONUI::INTERFACE_OPTION, _OptionWindow);

	_MouseState = 0;
}

UIOptionRoot::~UIOptionRoot()
{

}

void UIOptionRoot::AddObject(mu_uint32 index, UIBase *object)
{
	_ObjectsMap.insert(std::make_pair(index, object));
}

mu_uint32 UIOptionRoot::OnWindowMessage(const SDL_Event *event, mu_size *result)
{
	mu_int32 mouseX, mouseY;

	switch (event->type)
	{
	case SDL_MOUSEBUTTONDOWN:
	case SDL_MOUSEBUTTONUP:
		{
			mouseX = event->button.x;
			mouseY = event->button.y;
		}
		break;

	default:
		{
			mouseX = MU_INPUT->GetMouseX();
			mouseY = MU_INPUT->GetMouseY();
		}
		break;
	}

	UIBase *focusObject = GetFocus(mouseX, mouseY);

	if (focusObject != nullptr)
	{
		switch (event->type)
		{
		case SDL_MOUSEBUTTONDOWN:
			{
				++_MouseState;
			}
			break;
		}
	}

	if (_FocusObject != focusObject)
	{
		if (IsValidFocus(_FocusObject) &&
			_FocusObject->IsEvent(GFX_EVENT::GFX_MOUSE_EVENT) == true)
		{
			_FocusObject->OnMouseLeave();
		}

		_FocusObject = focusObject;

		if (IsValidFocus(focusObject) &&
			focusObject->IsEvent(GFX_EVENT::GFX_MOUSE_EVENT) == true)
		{
			focusObject->OnMouseOver(mouseX, mouseY);
		}
	}

	mu_uint32 pressedClick = 0;
	mu_uint32 doubleClick = 0;
	mu_uint32 releasedClick = 0;
	mu_uint32 virtualKeys = 0;
	mu_float mouseWheel = 0.0f;

	switch (event->type)
	{
	case SDL_MOUSEBUTTONDOWN:
		{
			switch (event->button.button)
			{
			case SDL_BUTTON_LEFT:
				{
					if (event->button.clicks == 2)
					{
						doubleClick = MOUSE_FLAG_LEFT;
					}
					else
					{
						pressedClick = MOUSE_FLAG_LEFT;
					}
				}
				break;

			case SDL_BUTTON_MIDDLE:
				{
					if (event->button.clicks == 2)
					{
						doubleClick = MOUSE_FLAG_MIDDLE;
					}
					else
					{
						pressedClick = MOUSE_FLAG_MIDDLE;
					}
				}
				break;

			case SDL_BUTTON_RIGHT:
				{
					if (event->button.clicks == 2)
					{
						doubleClick = MOUSE_FLAG_RIGHT;
					}
					else
					{
						pressedClick = MOUSE_FLAG_RIGHT;
					}
				}
				break;
			}
		}
		break;

	case SDL_MOUSEBUTTONUP:
		{
			switch (event->button.button)
			{
			case SDL_BUTTON_LEFT:
				{
					if (_MouseState > 0)
					{
						releasedClick = MOUSE_FLAG_LEFT;
					}
				}
				break;

			case SDL_BUTTON_MIDDLE:
				{
					if (_MouseState > 0)
					{
						releasedClick = MOUSE_FLAG_MIDDLE;
					}
				}
				break;

			case SDL_BUTTON_RIGHT:
				{
					if (_MouseState > 0)
					{
						releasedClick = MOUSE_FLAG_RIGHT;
					}
				}
				break;
			}
		}
		break;

	case SDL_MOUSEWHEEL:
		mouseWheel = event->wheel.y;
		break;
	}

	switch (event->type)
	{
	case SDL_MOUSEBUTTONDOWN:
	case SDL_MOUSEBUTTONUP:
		{
			virtualKeys = 0;
			if (MU_INPUT->IsCtrlPressing() == true)
				virtualKeys |= VFLAG_CONTROL;
			if (MU_INPUT->IsShiftPressing() == true)
				virtualKeys |= VFLAG_SHIFT;
		}
		break;
	}

	mu_boolean forceSelect = false;
	mu_uint32 selectedRet = GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE;

	if (event->type == g_ELFocusEventID)
	{
		UISceneManager *sceneManager = (UISceneManager*)event->user.data2;
		if (static_cast<UISceneManager*>(this) == sceneManager)
		{
			focusObject = (UIBase*)event->user.data1;
			forceSelect = true;
		}
	}

	if (focusObject == GFX_BYPASS_FOCUS)
	{
		focusObject = nullptr;
	}

	if (_SelectedObject != nullptr &&
		_SelectedObject->GetParent() != nullptr &&
		_SelectedObject->GetParent()->IsGfxFlag(GFX_FLAG::GFX_HIDDEN) == true)
	{
		if (_SelectedObject->_MouseFlag)
		{
			_SelectedObject->OnMouseCancelled(_SelectedObject->_MouseFlag);
		}

		_SelectedObject->OnFocusCancelled();

		_SelectedObject = nullptr;
	}

	if (focusObject != _SelectedObject &&
		(forceSelect || pressedClick || doubleClick))
	{
		if (IsValidFocus(focusObject))
		{
			focusObject->OnFocus();
		}

		if (_SelectedObject != nullptr)
		{
			if (_SelectedObject->_MouseFlag)
			{
				_SelectedObject->OnMouseCancelled(_SelectedObject->_MouseFlag);
			}

			_SelectedObject->OnFocusCancelled();
		}

		_SelectedObject = focusObject;
	}

	// Pre-objects messages
	if (selectedRet == GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE)
	{

		if (IsValidFocus(_FocusObject) == true &&
			selectedRet == GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE &&
			_FocusObject->IsEvent(GFX_EVENT::GFX_PREMESSAGE_FIRST) == true)
		{
			selectedRet = _FocusObject->OnPreWindowMessage(event, result);
		}

		if (_SelectedObject != nullptr &&
			selectedRet == GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE &&
			_SelectedObject->IsEvent(GFX_EVENT::GFX_PREMESSAGE_FIRST) == true)
		{
			selectedRet = _SelectedObject->OnPreWindowMessage(event, result);
		}

		if (selectedRet == GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE)
		{
			for (auto it = _ObjectsMap.rbegin(); it != _ObjectsMap.rend(); ++it)
			{
				if (it->second->IsGfxFlag(GFX_FLAG::GFX_HIDDEN) == false ||
					it->second->IsGfxFlag(GFX_FLAG::GFX_FORCEMESSAGE) == true)
				{
					selectedRet = it->second->OnPreWindowMessage(event, result);

					if (selectedRet != GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE)
					{
						break;
					}
				}
			}
		}

		if (selectedRet == GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE)
		{
			switch (event->type)
			{
			case SDL_KEYDOWN:
				{
					selectedRet = OnPreKeyPressed(event->key.keysym);
				}
				break;
			}
		}
	}

	if (IsValidFocus(_FocusObject) &&
		_SelectedObject != _FocusObject)
	{
		if (_FocusObject->IsEvent(GFX_EVENT::GFX_MOUSE_EVENT))
		{
			_FocusObject->OnMouseMove(mouseX, mouseY);
		}
	}

	if (_SelectedObject != nullptr)
	{
		if (_SelectedObject->IsEvent(GFX_EVENT::GFX_MOUSE_EVENT))
		{
			mu_uint32 &mouseFlags = _SelectedObject->_MouseFlag;

			pressedClick &= ~mouseFlags;
			releasedClick &= mouseFlags;

			mu_uint32 doubleFilter = doubleClick & (~mouseFlags);

			_SelectedObject->OnMouseMove(mouseX, mouseY);

			if (mouseWheel != 0.0f)
			{
				_SelectedObject->OnMouseWheel(mouseWheel);
			}

			if (doubleClick != 0 && doubleFilter != doubleClick)
			{
				doubleClick &= ~doubleFilter;

				_SelectedObject->OnMouseCancelled(doubleClick);
			}

			if (mouseFlags)
			{
				if (releasedClick != 0)
				{
					if (_SelectedObject->IsInside(mouseX, mouseY) == false)
					{
						_SelectedObject->OnMouseCancelled(releasedClick);
					}
					else
					{
						_SelectedObject->OnMouseReleased(releasedClick, virtualKeys);
					}
				}

				mouseFlags &= ~releasedClick;
			}

			if (doubleClick != 0 ||
				pressedClick != 0)
			{
				if (_SelectedObject->IsEvent(GFX_EVENT::GFX_MOUSE_DBLCLICK_AS_CLICK))
				{
					pressedClick |= doubleClick;
					doubleClick = 0;
				}

				_SelectedObject->OnMousePressed(pressedClick, doubleClick, virtualKeys);

				// Add focus inside of containers for optimization
				if (_SelectedObject->IsEvent(GFX_EVENT::GFX_MOUSE_DBLCLICK_MARK) == true)
				{
					_SelectedObject->MarkMouseFlag(pressedClick | doubleClick);
				}
				else if (pressedClick != 0)
				{
					_SelectedObject->MarkMouseFlag(pressedClick);
				}
			}
		}

		if (selectedRet == GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE)
		{
			if (_SelectedObject->IsRouteToParent() == false ||
				_SelectedObject->GetParent() == nullptr ||
				(selectedRet = _SelectedObject->GetParent()->OnWindowMessage(event, result)) == GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE)
			{
				selectedRet = _SelectedObject->OnWindowMessage(event, result);
			}
		}
	}

	if (selectedRet == GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE)
	{
		for (auto it = _ObjectsMap.rbegin(); it != _ObjectsMap.rend(); ++it)
		{
			if (it->second->IsGfxFlag(GFX_FLAG::GFX_HIDDEN) == false ||
				it->second->IsGfxFlag(GFX_FLAG::GFX_FORCEMESSAGE) == true)
			{
				selectedRet = it->second->OnWindowMessage(event, result);

				if (selectedRet != GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE)
				{
					break;
				}
			}
		}

		if (selectedRet == GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE)
		{
			switch (event->type)
			{
			case SDL_KEYDOWN:
				{
					selectedRet = OnKeyPressed(event->key.keysym);
				}
				break;
			}
		}
	}

	switch (event->type)
	{
	case SDL_MOUSEBUTTONUP:
		{
			// This is for avoid any mix between the GUI and the 3D input
			if (_MouseState > 0)
			{
				--_MouseState;

				if (selectedRet == GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE)
				{
					selectedRet = GFX_MESSAGE_RETURN::GFX_MESSAGE_BYPASS;
				}
			}
		}
		break;
	}

	if (selectedRet == GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE &&
		IsValidFocus(_FocusObject) == true &&
		_FocusObject->BypassMessage() == true)
	{
		switch (event->type)
		{
		case SDL_KEYDOWN:
		case SDL_KEYUP:
		case SDL_TEXTINPUT:
		case SDL_MOUSEMOTION:
		case SDL_MOUSEWHEEL:
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			{
				selectedRet = GFX_MESSAGE_RETURN::GFX_MESSAGE_BYPASS;
			}
			break;
		}
	}

	return selectedRet;
}

mu_uint32 UIOptionRoot::OnPostWindowMessage(const SDL_Event *event, mu_size *result)
{
	switch (event->type)
	{
	case SDL_KEYDOWN:
		return OnPostKeyPressed(event->key.keysym);
	}

	return GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE;
}

void UIOptionRoot::OnWindowCancelled()
{
	if (_SelectedObject != nullptr)
	{
		if (_SelectedObject->_MouseFlag)
		{
			_SelectedObject->OnMouseCancelled(_SelectedObject->_MouseFlag);
		}

		_SelectedObject->OnFocusCancelled();

		_SelectedObject = nullptr;
	}
}

void UIOptionRoot::SystemConfigure()
{
	UIBase::SystemConfigure();

	_Background->SystemConfigure();

	for (auto it = _ObjectsMap.begin(); it != _ObjectsMap.end(); ++it)
	{
		it->second->SystemConfigure();
	}
}

void UIOptionRoot::InitializeGPU()
{
	UIBase::InitializeGPU();

	for (auto it = _ObjectsMap.begin(); it != _ObjectsMap.end(); ++it)
	{
		it->second->InitializeGPU();
	}
}

void UIOptionRoot::ReleaseGPU()
{
	UIBase::ReleaseGPU();

	for (auto it = _ObjectsMap.begin(); it != _ObjectsMap.end(); ++it)
	{
		it->second->ReleaseGPU();
	}
}

void UIOptionRoot::Update(mu_uint64 CurrentTime)
{
	for (auto it = _ObjectsMap.begin(); it != _ObjectsMap.end(); ++it)
	{
		if (it->second->IsGfxFlag(GFX_FLAG::GFX_HIDDEN) == false)
		{
			it->second->Update(CurrentTime);
		}
	}
}

void UIOptionRoot::Draw()
{
	if (_VisibleBitset.any() == false)
	{
		return;
	}

	_Background->Draw();

	for (auto it = _ObjectsMap.begin(); it != _ObjectsMap.end(); ++it)
	{
		if (it->second->IsGfxFlag(GFX_FLAG::GFX_HIDDEN) == false)
		{
			it->second->Draw();
		}
	}
}

void UIOptionRoot::Draw3D()
{
	for (auto it = _ObjectsMap.begin(); it != _ObjectsMap.end(); ++it)
	{
		if (it->second->IsGfxFlag(GFX_FLAG::GFX_HIDDEN) == false)
		{
			it->second->Draw3D();
		}
	}
}

void UIOptionRoot::PostDraw()
{
	for (auto it = _ObjectsMap.begin(); it != _ObjectsMap.end(); ++it)
	{
		if (it->second->IsGfxFlag(GFX_FLAG::GFX_HIDDEN) == false)
		{
			it->second->PostDraw();
		}
	}
}

UIBase *UIOptionRoot::GetFocus(mu_int32 mouseX, mu_int32 mouseY)
{
	if (IsGfxFlag(GFX_FLAG::GFX_DISABLED) ||
		IsGfxFlag(GFX_FLAG::GFX_HIDDEN))
	{
		return nullptr;
	}

	if (IsInside(mouseX, mouseY) == false)
	{
		return nullptr;
	}

	if (_VisibleBitset.count() == 0)
	{
		return nullptr;
	}

	UIBase *object = nullptr;

	for (auto it = _ObjectsMap.rbegin(); it != _ObjectsMap.rend(); ++it)
	{
		if (it->second->IsGfxFlag(GFX_FLAG::GFX_HIDDEN) == false)
		{
			object = it->second;
			break;
		}
	}

	if (object != nullptr)
	{
		object = object->GetFocus(mouseX, mouseY);
	}

	if (object == GFX_BYPASS_FOCUS)
	{
		return GFX_BYPASS_FOCUS;
	}
	else if (object != nullptr)
	{
		return object;
	}

	return GFX_BYPASS_FOCUS;
}

mu_uint32 UIOptionRoot::OnPreKeyPressed(const SDL_Keysym &key)
{
	return GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE;
}

mu_uint32 UIOptionRoot::OnPostKeyPressed(const SDL_Keysym &key)
{
	if (MU_INPUT->IsShiftPressing() == false &&
		MU_INPUT->IsCtrlPressing() == false)
	{
		switch (key.scancode)
		{
		case SDL_SCANCODE_ESCAPE:
			{
				mu_int32 optionKey = -1;
				switch (g_CurrentScene)
				{
				case LOG_IN_SCENE:
					optionKey = OPTIONUI::INTERFACE_LOGINMENU;
					break;
				case CHARACTER_SCENE:
					optionKey = OPTIONUI::INTERFACE_CHARACTERMENU;
					break;
				case MAIN_SCENE:
					optionKey = OPTIONUI::INTERFACE_GAMEMENU;
					break;
				}

				if (optionKey != -1)
				{
					if (IsVisible(optionKey) == false && _VisibleBitset.any() == false)
						Show(optionKey);
					else
						Hide(optionKey);
				}
			}
			break;
		}
	}

	return GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE;
}

mu_uint32 UIOptionRoot::OnKeyPressed(const SDL_Keysym &key)
{
	return GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE;
}

void UIOptionRoot::OnGFxSceneMessage(GFX_MESSAGE &objMsg)
{
	switch (objMsg.Message)
	{
	case GFX_MESSAGE_TYPE::GFX_PRESSED:
	case GFX_MESSAGE_TYPE::GFX_TOGGLE:
		{
			PlayBackground(SOUND_CLICK01);
		}
		break;
	}

	if (objMsg.ObjectID >= OPTIONUI::MESSAGE_OPTION_TAB_BEGIN &&
		objMsg.ObjectID <= OPTIONUI::MESSAGE_OPTION_TAB_END)
	{
		switch (objMsg.Message)
		{
		case GFX_MESSAGE_TYPE::GFX_TOGGLE:
			{
				mu_uint32 tabIndex = objMsg.ObjectID - OPTIONUI::MESSAGE_OPTION_TAB_BEGIN;

				_OptionWindow->SetCurrentTab(tabIndex);
			}
			break;
		}
	}
	else if (objMsg.ObjectID >= OPTIONUI::MESSAGE_OPTION_RESOLUTION_BEGIN &&
		objMsg.ObjectID <= OPTIONUI::MESSAGE_OPTION_RESOLUTION_END)
	{
		switch (objMsg.Message)
		{
		case GFX_MESSAGE_TYPE::GFX_TOGGLE:
			{
				mu_int32 resIndex = objMsg.ObjectID - OPTIONUI::MESSAGE_OPTION_RESOLUTION_BEGIN;

				_OptionWindow->_ResolutionSelected = _OptionWindow->_ResolutionListIndex + resIndex;
				_OptionWindow->ConfigureResolutionList();
			}
			break;
		}
	}
	else if (objMsg.Message == GFX_MESSAGE_TYPE::GFX_PRESSED)
	{
		switch (objMsg.ObjectID)
		{
		case OPTIONUI::MESSAGE_LOGIN_EXITBUTTON:
		case OPTIONUI::MESSAGE_CHARACTER_EXITBUTTON:
			{
				SDL_Event event;
				SDL_memset(&event, 0, sizeof(event));
				event.type = SDL_QUIT;
				SDL_PushEvent(&event);

				HideAll();
			}
			break;

		case OPTIONUI::MESSAGE_CHARACTER_SERVERBUTTON:
			{
				g_GameServerObject->Disconnect();

				SCENES::CharacterScene.SetNextScene(&SCENES::LoginScene);
				g_pLoginListBox->Clear();
				
				HideAll();
			}
			break;

		case OPTIONUI::MESSAGE_GAME_EXITBUTTON:
			{
				g_GameServerObject->SendRequestLogout(EREQUEST_LOGOUT::ERL_EXITGAME);
				HideAll();
			}
			break;

		case OPTIONUI::MESSAGE_GAME_SERVERBUTTON:
			{
				g_GameServerObject->SendRequestLogout(EREQUEST_LOGOUT::ERL_SERVER);
				HideAll();
			}
			break;

		case OPTIONUI::MESSAGE_GAME_CHARACTERBUTTON:
			{
				g_GameServerObject->SendRequestLogout(EREQUEST_LOGOUT::ERL_CHARACTER);
				HideAll();
			}
			break;

		case OPTIONUI::MESSAGE_LOGIN_OPTIONBUTTON:
		case OPTIONUI::MESSAGE_CHARACTER_OPTIONBUTTON:
		case OPTIONUI::MESSAGE_GAME_OPTIONBUTTON:
			{
				HideAll();
				Show(OPTIONUI::INTERFACE_OPTION);
			}
			break;

		case OPTIONUI::MESSAGE_LOGIN_CLOSEBUTTON:
		case OPTIONUI::MESSAGE_CHARACTER_CLOSEBUTTON:
		case OPTIONUI::MESSAGE_GAME_CLOSEBUTTON:
			{
				HideAll();
			}
			break;

		case OPTIONUI::MESSAGE_OPTION_APPLY:
			{
				_OptionWindow->OnApplySettings();
				MU_RESOURCE->SaveSettings();
			}
			break;

		case OPTIONUI::MESSAGE_OPTION_CLOSE:
			{
				MU_RESOURCE->SetAutoAttack(_OptionWindow->_CurrentSettings.IsAutoAttack);
				MU_RESOURCE->SetMusicVolume(_OptionWindow->_CurrentSettings.MusicVolume);
				MU_RESOURCE->SetSoundVolume(_OptionWindow->_CurrentSettings.SoundVolume);
				HideAll();
			}
			break;

		case OPTIONUI::MESSAGE_OPTION_SHADOWTYPE_BUTTON:
			{
				_OptionWindow->_CurrentShadowType = (_OptionWindow->_CurrentShadowType + 1) % EShadowTypeMax;
				_OptionWindow->ConfigureShadowButton(_OptionWindow->_CurrentShadowType);
			}
			break;
		}
	}
}

void UIOptionRoot::Show(mu_uint32 Key)
{
	_VisibleBitset.set(Key, true);

	switch (Key)
	{
	case OPTIONUI::INTERFACE_LOGINMENU:
		{
			_LoginMenu->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case OPTIONUI::INTERFACE_CHARACTERMENU:
		{
			_CharacterMenu->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case OPTIONUI::INTERFACE_GAMEMENU:
		{
			_GameMenu->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
			g_pMainFrame->ChangeOptionButton();
		}
		break;

	case OPTIONUI::INTERFACE_OPTION:
		{
			_OptionWindow->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
			_OptionWindow->PrepareCurrentSettings();
			_OptionWindow->SetCurrentTab(UIOptionWindow::TAB_VIDEO);
		}
		break;
	}

	//UpdateWindowPosition();
}

void UIOptionRoot::Hide(mu_uint32 Key)
{
	_VisibleBitset.set(Key, false);

	switch (Key)
	{
	case OPTIONUI::INTERFACE_LOGINMENU:
		{
			_LoginMenu->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case OPTIONUI::INTERFACE_CHARACTERMENU:
		{
			_CharacterMenu->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case OPTIONUI::INTERFACE_GAMEMENU:
		{
			_GameMenu->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
			g_pMainFrame->ChangeOptionButton();
		}
		break;

	case OPTIONUI::INTERFACE_OPTION:
		{
			_OptionWindow->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;
	}

	//UpdateWindowPosition();
}

mu_boolean UIOptionRoot::IsVisible(mu_uint32 Key)
{
	return _VisibleBitset.test(Key);
}

void UIOptionRoot::HideAll()
{
	if (_VisibleBitset.count() == 0)
	{
		return;
	}

	for (mu_size n = 0;n < _VisibleBitset.size(); ++n)
	{
		if (_VisibleBitset[n] == true)
		{
			Hide(n);
		}
	}
}