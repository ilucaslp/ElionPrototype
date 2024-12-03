#include "stdafx.h"
#include "mu_uiloginroot.h"

#define SCREEN_WIDTH 640.0f
#define SCREEN_HEIGHT 480.0f

UILoginRoot *g_pLoginRoot = nullptr;
mu_char g_PasswordSecret[8 + 1] = { 0 };

UILoginRoot::UILoginRoot()
{
	_SceneManager = nullptr;

	UIBase::SetAspectRatio(SCREEN_WIDTH, SCREEN_HEIGHT);
	UIBase::SetPosition(0.0f, 0.0f);
	UIBase::SetDimensions(SCREEN_WIDTH, SCREEN_HEIGHT);

	_SelectedObject = nullptr;
	_FocusObject = nullptr;

	SetRoot(this);

	_Logo = new UILogo(this);
	AddObject(LOGINUI::INTERFACE_LOGO, _Logo);

	_LoginFrame = new UILoginFrame(this);
	AddObject(LOGINUI::INTERFACE_LOGINFRAME, _LoginFrame);

	_ServerList = new UIServerList(this);
	AddObject(LOGINUI::INTERFACE_SERVERLIST, _ServerList);

	_LoginWindow = new UILoginWindow(this);
	AddObject(LOGINUI::INTERFACE_LOGINWINDOW, _LoginWindow);

	_ServerMessage = new UIServerMessage(this);
	AddObject(LOGINUI::INTERFACE_SERVERMESSAGE, _ServerMessage);

	_MouseState = 0;
}

UILoginRoot::~UILoginRoot()
{

}

void UILoginRoot::AddObject(mu_uint32 index, UIBase *object)
{
	_ObjectsMap.insert(std::make_pair(index, object));
}

mu_uint32 UILoginRoot::OnWindowMessage(const SDL_Event *event, mu_size *result)
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

mu_uint32 UILoginRoot::OnPostWindowMessage(const SDL_Event *event, mu_size *result)
{
	switch (event->type)
	{
	case SDL_KEYDOWN:
		return OnPostKeyPressed(event->key.keysym);
	}

	return GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE;
}

void UILoginRoot::OnWindowCancelled()
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

void UILoginRoot::SystemConfigure()
{
	UIBase::SystemConfigure();

	for (auto it = _ObjectsMap.begin(); it != _ObjectsMap.end(); ++it)
	{
		it->second->SystemConfigure();
	}
}

void UILoginRoot::InitializeGPU()
{
	UIBase::InitializeGPU();

	for (auto it = _ObjectsMap.begin(); it != _ObjectsMap.end(); ++it)
	{
		it->second->InitializeGPU();
	}
}

void UILoginRoot::ReleaseGPU()
{
	UIBase::ReleaseGPU();

	for (auto it = _ObjectsMap.begin(); it != _ObjectsMap.end(); ++it)
	{
		it->second->ReleaseGPU();
	}
}

void UILoginRoot::Update(mu_uint64 CurrentTime)
{
	for (auto it = _ObjectsMap.begin(); it != _ObjectsMap.end(); ++it)
	{
		if (it->second->IsGfxFlag(GFX_FLAG::GFX_HIDDEN) == false)
		{
			it->second->Update(CurrentTime);
		}
	}
}

void UILoginRoot::Draw()
{
	for (auto it = _ObjectsMap.begin(); it != _ObjectsMap.end(); ++it)
	{
		if (it->second->IsGfxFlag(GFX_FLAG::GFX_HIDDEN) == false)
		{
			it->second->Draw();
		}
	}

	FONT_NORMAL->DrawStringC2D(TEXTINDEX::VERSION_INFO,
		MU_TO_WINDOW(1020.0f, MU_RESOURCE->_1024x1024Rate[0]),
		MU_TO_WINDOW(1020.0f, MU_RESOURCE->_1024x1024Rate[1]),
		1.0f,
		ETA_RIGHT,
		ETA_TOP,
		0.0f,
		MU::MakeARGB(204, 178, 153, 255),
		MU::MakeARGB(0, 0, 0, 0),
		_T("Elion Test v") ELION_VERSION);
}

void UILoginRoot::Draw3D()
{
	for (auto it = _ObjectsMap.begin(); it != _ObjectsMap.end(); ++it)
	{
		if (it->second->IsGfxFlag(GFX_FLAG::GFX_HIDDEN) == false)
		{
			it->second->Draw3D();
		}
	}
}

void UILoginRoot::PostDraw()
{
	for (auto it = _ObjectsMap.begin(); it != _ObjectsMap.end(); ++it)
	{
		if (it->second->IsGfxFlag(GFX_FLAG::GFX_HIDDEN) == false)
		{
			it->second->PostDraw();
		}
	}
}

UIBase *UILoginRoot::GetFocus(mu_int32 mouseX, mu_int32 mouseY)
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

	for (auto it = _ObjectsMap.rbegin(); it != _ObjectsMap.rend(); ++it)
	{
		if (it->second->IsGfxFlag(GFX_FLAG::GFX_HIDDEN) == true)
			continue;

		UIBase *object = it->second->GetFocus(mouseX, mouseY);

		if (object == GFX_BYPASS_FOCUS)
		{
			return GFX_BYPASS_FOCUS;
		}
		else if (object)
		{
			return object;
		}
	}

	return nullptr;
}

mu_uint32 UILoginRoot::OnPreKeyPressed(const SDL_Keysym &key)
{
	return GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE;
}

mu_uint32 UILoginRoot::OnKeyPressed(const SDL_Keysym &key)
{
	if (MU_INPUT->IsShiftPressing() == true &&
		MU_INPUT->IsCtrlPressing() == false)
	{
		switch (key.scancode)
		{
		case SDL_SCANCODE_F11:
			{
				MU_GLOBALTIMER.SetLimitFPS(!MU_GLOBALTIMER.GetLimitFPS());
			}
			break;
		}
	}

	return GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE;
}

void UILoginRoot::OnGFxSceneMessage(GFX_MESSAGE &objMsg)
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

	if (objMsg.Message == GFX_MESSAGE_TYPE::GFX_TOGGLE)
	{
		if (objMsg.ObjectID >= LOGINUI::MESSAGE_SERVERGROUP_BUTTON &&
			objMsg.ObjectID <= LOGINUI::MESSAGE_SERVERGROUP_BUTTON_END)
		{
			mu_uint32 groupIndex = objMsg.ObjectID - LOGINUI::MESSAGE_SERVERGROUP_BUTTON;
			mu_uint32 oldGroup = _ServerList->_GroupSelected;
			_ServerList->_GroupSelected = groupIndex;

			if (oldGroup != -1)
			{
				_ServerList->_GroupButton[oldGroup]->ChangeState(_FocusObject == _ServerList->_GroupButton[oldGroup] ? UIToggleButton::UI_OVER : UIToggleButton::UI_NORMAL);
			}

			_ServerList->_GroupButton[groupIndex]->ChangeState(_FocusObject == _ServerList->_GroupButton[groupIndex] ? UIToggleButton::UI_PRESSED_OVER : UIToggleButton::UI_PRESSED);

			CL2CS_REQUEST_SERVERLIST RequestServerList;

			RequestServerList.GroupIndex = groupIndex;

			g_ConnectServerObject->Send(&RequestServerList);
		}
	}
	else if (objMsg.Message == GFX_MESSAGE_TYPE::GFX_PRESSED)
	{
		if (objMsg.ObjectID >= LOGINUI::MESSAGE_SERVER_BUTTON &&
			objMsg.ObjectID <= LOGINUI::MESSAGE_SERVER_BUTTON_END)
		{
			mu_uint32 serverIndex = objMsg.ObjectID - LOGINUI::MESSAGE_SERVER_BUTTON;

			GROUP_INFO &group = _ServerList->_ServerGroups[_ServerList->_GroupSelected];
			SERVER_INFO &server = group.Servers[serverIndex];

			if (server.Percent == CS2CL::SERVER_OFFLINE)
			{
				// Do nothing
			}
			else if (server.Percent >= 100)
			{
				g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_SERVERISFULL);
			}
			else
			{
				_ServerList->_ServerSelected = serverIndex;

				Hide(LOGINUI::INTERFACE_SERVERLIST);

				CL2CS_REQUEST_CONNECT RequestConnect;

				RequestConnect.GroupIndex = _ServerList->_GroupSelected;
				RequestConnect.ServerIndex = _ServerList->_ServerSelected;

				g_ConnectServerObject->Send(&RequestConnect);
			}
		}
		else
		{
			switch (objMsg.ObjectID)
			{
			case LOGINUI::MESSAGE_MENU_BUTTON:
				{
					g_pOptionRoot->Show(OPTIONUI::INTERFACE_LOGINMENU);
				}
				break;

			case LOGINUI::MESSAGE_WEBSITE_BUTTON:
				{
#if defined(WIN32) || defined(WIN64) // FIX
					ShellExecute(nullptr, _T("open"), _T("https://elion.mu"), nullptr, nullptr, SW_SHOWNORMAL);
#endif
				}
				break;

			case LOGINUI::MESSAGE_FORUM_BUTTON:
				{
#if defined(WIN32) || defined(WIN64) // FIX
					ShellExecute(nullptr, _T("open"), _T("https://forum.elion.mu"), nullptr, nullptr, SW_SHOWNORMAL);
#endif
				}
				break;

			case LOGINUI::MESSAGE_FACEBOOK_BUTTON:
				{
#if defined(WIN32) || defined(WIN64) // FIX
					ShellExecute(nullptr, _T("open"), _T("https://www.facebook.com/eliononline/"), nullptr, nullptr, SW_SHOWNORMAL);
#endif
				}
				break;

			case LOGINUI::MESSAGE_LOGIN_OKBUTTON:
				{
					if (_LoginWindow->_AccountBox->GetLength() < ACCOUNT_MINIMUM ||
						_LoginWindow->_AccountBox->GetLength() > ACCOUNT_MAXIMUM)
					{
						g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_OKBOX);
						g_pOkBox->ClearMessages();
						g_pOkBox->AddMessage(FONT_NORMAL, GlobalText[403]);
						g_pOkBox->Configure();
						_LoginWindow->_AccountBox->GiveFocus();
						_LoginWindow->_AccountBox->SelectAll();
					}
					else if (_LoginWindow->_PasswordBox->GetLength() < PASSWORD_MINIMUM ||
						_LoginWindow->_PasswordBox->GetLength() > PASSWORD_MAXIMUM)
					{
						g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_OKBOX);
						g_pOkBox->ClearMessages();
						g_pOkBox->AddMessage(FONT_NORMAL, GlobalText[404]);
						g_pOkBox->Configure();
						_LoginWindow->_PasswordBox->GiveFocus();
						_LoginWindow->_PasswordBox->SelectAll();
					}
					else
					{
						mu_text accountBuffer[ACCOUNT_MAXIMUM + 1];
						mu_text passwordBuffer[PASSWORD_MAXIMUM + 1];

						_LoginWindow->_AccountBox->GetText(accountBuffer, mu_countof(accountBuffer));
						_LoginWindow->_PasswordBox->GetText(passwordBuffer, mu_countof(passwordBuffer));

						mu_regex accountRegex(ACCOUNT_REGEX);
#ifndef DISABLE_PASSWORD_REGEX
						mu_regex passwordRegex(PASSWORD_REGEX);
#endif

						if (boost::regex_match(accountBuffer, accountRegex) == false)
						{
							g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_OKBOX);
							g_pOkBox->ClearMessages();
							g_pOkBox->AddMessage(FONT_NORMAL, _T("Account contains prohibited characters"));
							g_pOkBox->Configure();
							_LoginWindow->_AccountBox->GiveFocus();
							_LoginWindow->_AccountBox->SelectAll();
						}
#ifndef DISABLE_PASSWORD_REGEX
						else if (boost::regex_match(passwordBuffer, passwordRegex) == false)
						{
							g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_OKBOX);
							g_pOkBox->ClearMessages();
							g_pOkBox->AddMessage(FONT_NORMAL, _T("Password contains prohibited characters"));
							g_pOkBox->Configure();
							_LoginWindow->_PasswordBox->GiveFocus();
							_LoginWindow->_PasswordBox->SelectAll();
						}
#endif
						else
						{
							_LoginWindow->_PasswordBox->ClearAllText();

							Hide(LOGINUI::INTERFACE_LOGINWINDOW);

							CL2GS_REQUEST_LOGIN RequestLogin;
							MUSha512 sha512;

							mu_char hashText[128 + 1];
							hashText[128] = '\0';

							RequestLogin.Username = accountBuffer;

							mu_asciistr accountUTF8 = boost::locale::conv::utf_to_utf<mu_char>(RequestLogin.Username);
							std::transform(accountUTF8.begin(), accountUTF8.end(), accountUTF8.begin(), ::toupper);
							mu_asciistr passwordUTF8 = boost::locale::conv::utf_to_utf<mu_char>(passwordBuffer);
							accountUTF8 += passwordUTF8;

							sha512.Init();
							sha512.Process(accountUTF8.c_str(), accountUTF8.size());
							sha512.Output(RequestLogin.PasswordHash);

							MUBlowfish passwordFish((mu_uint8*)g_PasswordSecret, sizeof(GS2CL_RECEIVE_CONNECT_DATA::PasswordSecret) * 2);

							passwordFish.Encode(RequestLogin.PasswordHash, RequestLogin.PasswordHash, sizeof(RequestLogin.PasswordHash));

							g_GameServerObject->Send(&RequestLogin);
						}
					}
				}
				break;

			case LOGINUI::MESSAGE_LOGIN_CANCELBUTTON:
				{
					_LoginWindow->_PasswordBox->ClearAllText();

					Show(LOGINUI::INTERFACE_SERVERLIST);
					Hide(LOGINUI::INTERFACE_LOGINWINDOW);

					g_NetworkClient->Disconnect(g_GameServerObject);
					g_GameServerConnected = false;

					CL2CS_REQUEST_SERVERLIST RequestServerList;

					RequestServerList.GroupIndex = _ServerList->_GroupSelected;

					g_ConnectServerObject->Send(&RequestServerList);
				}
				break;
			}
		}
	}
}

void UILoginRoot::Show(mu_uint32 Key)
{
	switch (Key)
	{
	case LOGINUI::INTERFACE_LOGO:
		{
			_Logo->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case LOGINUI::INTERFACE_LOGINFRAME:
		{
			_LoginFrame->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case LOGINUI::INTERFACE_SERVERLIST:
		{
			_ServerList->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case LOGINUI::INTERFACE_LOGINWINDOW:
		{
			_LoginWindow->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;
	}

	//UpdateWindowPosition();
}

void UILoginRoot::Hide(mu_uint32 Key)
{
	switch (Key)
	{
	case LOGINUI::INTERFACE_LOGO:
		{
			_Logo->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case LOGINUI::INTERFACE_LOGINFRAME:
		{
			_LoginFrame->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case LOGINUI::INTERFACE_SERVERLIST:
		{
			_ServerList->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case LOGINUI::INTERFACE_LOGINWINDOW:
		{
			_LoginWindow->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;
	}

	//UpdateWindowPosition();
}

mu_boolean UILoginRoot::IsVisible(mu_uint32 Key)
{
	switch (Key)
	{
	case LOGINUI::INTERFACE_LOGO:
		return _Logo->IsGfxFlag(GFX_FLAG::GFX_HIDDEN) == false;
	case LOGINUI::INTERFACE_LOGINFRAME:
		return _LoginFrame->IsGfxFlag(GFX_FLAG::GFX_HIDDEN) == false;
	case LOGINUI::INTERFACE_SERVERLIST:
		return _ServerList->IsGfxFlag(GFX_FLAG::GFX_HIDDEN) == false;
	case LOGINUI::INTERFACE_LOGINWINDOW:
		return _LoginWindow->IsGfxFlag(GFX_FLAG::GFX_HIDDEN) == false;
	}

	return false;
}