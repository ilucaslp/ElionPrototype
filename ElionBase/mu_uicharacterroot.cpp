#include "stdafx.h"
#include "mu_uicharacterroot.h"

#define SCREEN_WIDTH 640.0f
#define SCREEN_HEIGHT 480.0f

UICharacterRoot *g_pCharacterRoot = nullptr;

UICharacterRoot::UICharacterRoot()
{
	_SceneManager = nullptr;

	UIBase::SetAspectRatio(SCREEN_WIDTH, SCREEN_HEIGHT);
	UIBase::SetPosition(0.0f, 0.0f);
	UIBase::SetDimensions(SCREEN_WIDTH, SCREEN_HEIGHT);

	_SelectedObject = nullptr;
	_FocusObject = nullptr;

	SetRoot(this);

	_CharacterFrame = new UICharacterFrame(this);
	AddObject(CHARACTERUI::INTERFACE_FRAME, _CharacterFrame);

	_CreateCharacter = new UICreateCharacter(this);
	AddObject(CHARACTERUI::INTERFACE_CREATECHARACTER, _CreateCharacter);

	_ServerMessage = new UIServerMessage(this);
	AddObject(CHARACTERUI::INTERFACE_SERVERMESSAGE, _ServerMessage);

	_MouseState = 0;
}

UICharacterRoot::~UICharacterRoot()
{

}

void UICharacterRoot::AddObject(mu_uint32 index, UIBase *object)
{
	_ObjectsMap.insert(std::make_pair(index, object));
}

mu_uint32 UICharacterRoot::OnWindowMessage(const SDL_Event *event, mu_size *result)
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

mu_uint32 UICharacterRoot::OnPostWindowMessage(const SDL_Event *event, mu_size *result)
{
	switch (event->type)
	{
	case SDL_KEYDOWN:
		return OnPostKeyPressed(event->key.keysym);
	}

	return GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE;
}

void UICharacterRoot::OnWindowCancelled()
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

void UICharacterRoot::SystemConfigure()
{
	UIBase::SystemConfigure();

	for (auto it = _ObjectsMap.begin(); it != _ObjectsMap.end(); ++it)
	{
		it->second->SystemConfigure();
	}
}

void UICharacterRoot::InitializeGPU()
{
	UIBase::InitializeGPU();

	for (auto it = _ObjectsMap.begin(); it != _ObjectsMap.end(); ++it)
	{
		it->second->InitializeGPU();
	}
}

void UICharacterRoot::ReleaseGPU()
{
	UIBase::ReleaseGPU();

	for (auto it = _ObjectsMap.begin(); it != _ObjectsMap.end(); ++it)
	{
		it->second->ReleaseGPU();
	}
}

void UICharacterRoot::Update(mu_uint64 CurrentTime)
{
	for (auto it = _ObjectsMap.begin(); it != _ObjectsMap.end(); ++it)
	{
		it->second->Update(CurrentTime);
	}
}

void UICharacterRoot::Draw()
{
	for (auto it = _ObjectsMap.begin(); it != _ObjectsMap.end(); ++it)
	{
		if (it->second->IsGfxFlag(GFX_FLAG::GFX_HIDDEN) == false)
		{
			it->second->Draw();
		}
	}
}

void UICharacterRoot::Draw3D()
{
	for (auto it = _ObjectsMap.begin(); it != _ObjectsMap.end(); ++it)
	{
		if (it->second->IsGfxFlag(GFX_FLAG::GFX_HIDDEN) == false)
		{
			it->second->Draw3D();
		}
	}
}

void UICharacterRoot::PostDraw()
{
	for (auto it = _ObjectsMap.begin(); it != _ObjectsMap.end(); ++it)
	{
		if (it->second->IsGfxFlag(GFX_FLAG::GFX_HIDDEN) == false)
		{
			it->second->PostDraw();
		}
	}
}

UIBase *UICharacterRoot::GetFocus(mu_int32 mouseX, mu_int32 mouseY)
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

mu_uint32 UICharacterRoot::OnPreKeyPressed(const SDL_Keysym &key)
{
	return GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE;
}

mu_uint32 UICharacterRoot::OnKeyPressed(const SDL_Keysym &key)
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

void UICharacterRoot::OnGFxSceneMessage(GFX_MESSAGE &objMsg)
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
		if (objMsg.ObjectID >= CHARACTERUI::MESSAGE_CREATECHARACTER_CLASS &&
			objMsg.ObjectID <= CHARACTERUI::MESSAGE_CREATECHARACTER_CLASS_END)
		{
			mu_uint32 Class = objMsg.ObjectID - CHARACTERUI::MESSAGE_CREATECHARACTER_CLASS;

			_CreateCharacter->SetSelectedClass(Class);
		}
		else
		{
			switch (objMsg.ObjectID)
			{
			default:
				break;
			}
		}
	}
	else if (objMsg.Message == GFX_MESSAGE_TYPE::GFX_PRESSED)
	{
		switch (objMsg.ObjectID)
		{
		case CHARACTERUI::MESSAGE_MENU_BUTTON:
			{
				g_pOptionRoot->Show(OPTIONUI::INTERFACE_CHARACTERMENU);
			}
			break;

		case CHARACTERUI::MESSAGE_CREATE_BUTTON:
			{
				Show(CHARACTERUI::INTERFACE_CREATECHARACTER);
				g_pCreateCharacter->SetSelectedClass(GAME::CLASS_KNIGHT);
				g_pCreateCharacter->_InputBox->ClearAllText();
				g_pCreateCharacter->_InputBox->GiveFocus();
			}
			break;

		case CHARACTERUI::MESSAGE_CREATECHARACTER_OKBUTTON:
			{
				mu_text characterName[CHARACTER_MAXIMUM + 1] = { 0 };
				_CreateCharacter->_InputBox->GetText(characterName, mu_countof(characterName));

				mu_regex characterRegex(CHARACTER_REGEX);

				if (_CreateCharacter->_InputBox->GetLength() < CHARACTER_MINIMUM)
				{
					g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_OKBOX);
					g_pOkBox->ClearMessages();
					g_pOkBox->AddMessage(FONT_NORMAL, GlobalText[390]);
					g_pOkBox->Configure();
					_CreateCharacter->_InputBox->GiveFocus();
					_CreateCharacter->_InputBox->SelectAll();
				}
				else if (boost::regex_match(characterName, characterRegex) == false)
				{
					g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_OKBOX);
					g_pOkBox->ClearMessages();
					g_pOkBox->AddMessage(FONT_NORMAL, GlobalText[391]);
					g_pOkBox->Configure();
					_CreateCharacter->_InputBox->GiveFocus();
					_CreateCharacter->_InputBox->SelectAll();
				}
				else
				{
					g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_WAITINGBOX);
					g_pMessageRoot->_WaitingBox->SetMessage(GlobalText[471]);

					CL2GS_REQUEST_CREATECHARACTER RequestCreateCharacter;

					RequestCreateCharacter.Class = _CreateCharacter->_SelectedClass;
					RequestCreateCharacter.CharacterName = characterName;

					g_GameServerObject->Send(&RequestCreateCharacter);
				}
			}
			break;

		case CHARACTERUI::MESSAGE_CREATECHARACTER_CANCELBUTTON:
			{
				Hide(CHARACTERUI::INTERFACE_CREATECHARACTER);
			}
			break;

		case CHARACTERUI::MESSAGE_CONNECT_BUTTON:
			{
				if (SelectedHero >= BEGIN_CHARACTERLIST_COUNT + 0 && SelectedHero < MAX_CHARACTERLIST_COUNT)
				{
					CL2GS_REQUEST_SELECTCHARACTER RequestSelectCharacter;

					RequestSelectCharacter.SlotIndex = SelectedHero - BEGIN_CHARACTERLIST_COUNT;

					g_GameServerObject->Send(&RequestSelectCharacter);

					SCENES::CharacterScene.SetNextScene(&SCENES::WaitingScene);
					g_pChatListBox->Clear(TYPE_ALL_MESSAGE);
				}
			}
			break;

		case CHARACTERUI::MESSAGE_DELETE_BUTTON:
			{
				if (SelectedHero == -1 ||
					SelectedHero < BEGIN_CHARACTERLIST_COUNT + 0 ||
					SelectedHero >= BEGIN_CHARACTERLIST_COUNT + MAX_CHARACTERLIST_COUNT)
					break;

				if (CharactersClient[SelectedHero].GuildStatus != GAME::G_NONE)
				{
					g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_OKBOX);
					g_pOkBox->ClearMessages();
					g_pOkBox->AddMessage(FONT_NORMAL, GlobalText[1654]);
					g_pOkBox->Configure();
				}
				else
				{
					g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_DELETECHARACTER);

					mu_text buffer[512];
					mu_sprintf_s(buffer, mu_countof(buffer), GlobalText[1712], CharactersClient[SelectedHero].ID);
					g_pMessageRoot->_DeleteCharacter->SetMessage(buffer);
				}
			}
			break;
		}
	}
}

void UICharacterRoot::Show(mu_uint32 Key)
{
	switch (Key)
	{
	case CHARACTERUI::INTERFACE_FRAME:
		{
			_CharacterFrame->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case CHARACTERUI::INTERFACE_CREATECHARACTER:
		{
			_CreateCharacter->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;
	}

	//UpdateWindowPosition();
}

void UICharacterRoot::Hide(mu_uint32 Key)
{
	switch (Key)
	{
	case CHARACTERUI::INTERFACE_FRAME:
		{
			_CharacterFrame->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case CHARACTERUI::INTERFACE_CREATECHARACTER:
		{
			_CreateCharacter->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;
	}

	//UpdateWindowPosition();
}

mu_boolean UICharacterRoot::IsVisible(mu_uint32 Key)
{
	switch (Key)
	{
	case CHARACTERUI::INTERFACE_FRAME:
		return _CharacterFrame->IsGfxFlag(GFX_FLAG::GFX_HIDDEN) == false;
	case CHARACTERUI::INTERFACE_CREATECHARACTER:
		return _CreateCharacter->IsGfxFlag(GFX_FLAG::GFX_HIDDEN) == false;
	}

	return false;
}