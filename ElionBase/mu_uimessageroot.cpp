#include "stdafx.h"
#include "mu_uimessageroot.h"

#define SCREEN_WIDTH 640.0f
#define SCREEN_HEIGHT 480.0f

UIMessageRoot *g_pMessageRoot = nullptr;

UIMessageRoot::UIMessageRoot()
{
	_SceneManager = nullptr;

	UIBase::SetAspectRatio(SCREEN_WIDTH, SCREEN_HEIGHT);
	UIBase::SetPosition(0.0f, 0.0f);
	UIBase::SetDimensions(SCREEN_WIDTH, SCREEN_HEIGHT);

	_SelectedObject = nullptr;
	_FocusObject = nullptr;

	SetRoot(this);

	_Background = new UISpriteColor(this, this, MU::MakeARGB(0, 0, 0, 128), 640.0f, 480.0f, 0.0f, 0.0f, 640.0f, 480.0f);
	_Background->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

	_OkBox = new UIOkBox(this);
	AddObject(MESSAGEBOX::INTERFACE_OKBOX, _OkBox);

	_FailedConnect = new UIFailedConnect(this);
	AddObject(MESSAGEBOX::INTERFACE_FAILEDCONNECT, _FailedConnect);

	_ServerFull = new UIServerFull(this);
	AddObject(MESSAGEBOX::INTERFACE_SERVERISFULL, _ServerFull);

	_BannedFromServer = new UIBannedFromServer(this);
	AddObject(MESSAGEBOX::INTERFACE_BANNEDFROMSERVER, _BannedFromServer);

	_BlockedFromServer = new UIBlockedFromServer(this);
	AddObject(MESSAGEBOX::INTERFACE_BLOCKEDFROMSERVER, _BlockedFromServer);

	_WaitingBox = new UIWaitingBox(this);
	AddObject(MESSAGEBOX::INTERFACE_WAITINGBOX, _WaitingBox);

	_ConnectBox = new UIConnectBox(this);
	AddObject(MESSAGEBOX::INTERFACE_CONNECT, _ConnectBox);

	_DeleteCharacter = new UIDeleteCharacterWarning(this);
	AddObject(MESSAGEBOX::INTERFACE_DELETECHARACTER, _DeleteCharacter);

	_DisconnectedBox = new UIDisconnectedBox(this);
	AddObject(MESSAGEBOX::INTERFACE_DISCONNECTED, _DisconnectedBox);

	_ExitGameBox = new UIExitGameBox(this);
	AddObject(MESSAGEBOX::INTERFACE_EXITGAME, _ExitGameBox);

	_ChangeServerBox = new UIChangeServerBox(this);
	AddObject(MESSAGEBOX::INTERFACE_CHANGESERVER, _ChangeServerBox);

	_GuildLeaveBox = new UIGuildLeaveBox(this);
	AddObject(MESSAGEBOX::INTERFACE_GUILDLEAVE, _GuildLeaveBox);

	_GuildRequestBox = new UIGuildRequestBox(this);
	AddObject(MESSAGEBOX::INTERFACE_GUILDREQUEST, _GuildRequestBox);

	_GuildGiveStatusBox = new UIGuildGiveStatusBox(this);
	AddObject(MESSAGEBOX::INTERFACE_GUILDGIVESTATUS, _GuildGiveStatusBox);

	_GuildCancelStatusBox = new UIGuildCancelStatusBox(this);
	AddObject(MESSAGEBOX::INTERFACE_GUILDCANCELSTATUS, _GuildCancelStatusBox);

	_PartyRequestBox = new UIPartyRequestBox(this);
	AddObject(MESSAGEBOX::INTERFACE_PARTYREQUEST, _PartyRequestBox);

	_TradeRequestBox = new UITradeRequestBox(this);
	AddObject(MESSAGEBOX::INTERFACE_TRADEREQUEST, _TradeRequestBox);

	_TradeMoneyBox = new UITradeMoneyBox(this);
	AddObject(MESSAGEBOX::INTERFACE_TRADEMONEY, _TradeMoneyBox);

	_TradeAcceptBox = new UITradeAcceptBox(this);
	AddObject(MESSAGEBOX::INTERFACE_TRADEACCEPT, _TradeAcceptBox);

	_StorageDepositBox = new UIStorageDepositBox(this);
	AddObject(MESSAGEBOX::INTERFACE_STORAGEDEPOSITBOX, _StorageDepositBox);

	_StorageWithdrawBox = new UIStorageWithdrawBox(this);
	AddObject(MESSAGEBOX::INTERFACE_STORAGEWITHDRAWBOX, _StorageWithdrawBox);

	_StorageLockBox = new UIStorageLockBox(this);
	AddObject(MESSAGEBOX::INTERFACE_STORAGELOCKBOX, _StorageLockBox);

	_HighValueWarningBox = new UIHighValueWarningBox(this);
	AddObject(MESSAGEBOX::INTERFACE_HIGHVALUEWARNINGBOX, _HighValueWarningBox);

	_GambleBuyBox = new UIGambleBuyBox(this);
	AddObject(MESSAGEBOX::INTERFACE_GAMBLEBUY, _GambleBuyBox);

	_PCPointBuyBox = new UIPCPointBuyBox(this);
	AddObject(MESSAGEBOX::INTERFACE_PCPOINTBUY, _PCPointBuyBox);

	_ChaosMixMenuBox = new UIChaosMixMenu(this);
	AddObject(MESSAGEBOX::INTERFACE_CHAOSMIXMENU, _ChaosMixMenuBox);

	_MixCheckBox = new UIMixCheckBox(this);
	AddObject(MESSAGEBOX::INTERFACE_MIXCHECK, _MixCheckBox);

	_BloodCastleRank = new UIBloodCastleRank(this);
	AddObject(MESSAGEBOX::INTERFACE_BLOODCASTLE_RANK, _BloodCastleRank);

	_HelperSkillSetting = new UIHelperSkillSetting(this);
	AddObject(MESSAGEBOX::INTERFACE_HELPER_SKILLSETTING, _HelperSkillSetting);

	_HelperPotionSetting = new UIHelperPotionSetting(this);
	AddObject(MESSAGEBOX::INTERFACE_HELPER_POTIONSETTING, _HelperPotionSetting);

	_HelperElfPotionSetting = new UIHelperElfPotionSetting(this);
	AddObject(MESSAGEBOX::INTERFACE_HELPER_ELFPOTIONSETTING, _HelperElfPotionSetting);

	_HelperPartySetting = new UIHelperPartySetting(this);
	AddObject(MESSAGEBOX::INTERFACE_HELPER_PARTYSETTING, _HelperPartySetting);

	_HelperWizardPartySetting = new UIHelperWizardPartySetting(this);
	AddObject(MESSAGEBOX::INTERFACE_HELPER_WIZARDPARTYSETTING, _HelperWizardPartySetting);

	_HelperSkillSelect = new UIHelperSkillSelect(this);
	AddObject(MESSAGEBOX::INTERFACE_HELPER_SKILLSELECT, _HelperSkillSelect);

	_HelperItemAdd = new UIHelperItemAdd(this);
	AddObject(MESSAGEBOX::INTERFACE_HELPER_ITEMADD, _HelperItemAdd);

	_HelperStart = new UIHelperStart(this);
	AddObject(MESSAGEBOX::INTERFACE_HELPER_START, _HelperStart);

	_MouseState = 0;
}

UIMessageRoot::~UIMessageRoot()
{

}

void UIMessageRoot::AddObject(mu_uint32 index, UIBase *object)
{
	_ObjectsMap.insert(std::make_pair(index, object));
}

mu_uint32 UIMessageRoot::OnWindowMessage(const SDL_Event *event, mu_size *result)
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

mu_uint32 UIMessageRoot::OnPostWindowMessage(const SDL_Event *event, mu_size *result)
{
	switch (event->type)
	{
	case SDL_KEYDOWN:
		return OnPostKeyPressed(event->key.keysym);
	}

	return GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE;
}

void UIMessageRoot::OnWindowCancelled()
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

void UIMessageRoot::SystemConfigure()
{
	UIBase::SystemConfigure();

	_Background->SystemConfigure();

	for (auto it = _ObjectsMap.begin(); it != _ObjectsMap.end(); ++it)
	{
		it->second->SystemConfigure();
	}
}

void UIMessageRoot::InitializeGPU()
{
	UIBase::InitializeGPU();

	for (auto it = _ObjectsMap.begin(); it != _ObjectsMap.end(); ++it)
	{
		it->second->InitializeGPU();
	}
}

void UIMessageRoot::ReleaseGPU()
{
	UIBase::ReleaseGPU();

	for (auto it = _ObjectsMap.begin(); it != _ObjectsMap.end(); ++it)
	{
		it->second->ReleaseGPU();
	}
}

void UIMessageRoot::Update(mu_uint64 CurrentTime)
{
	for (auto it = _ObjectsMap.begin(); it != _ObjectsMap.end(); ++it)
	{
		if (it->second->IsGfxFlag(GFX_FLAG::GFX_HIDDEN) == false)
		{
			it->second->Update(CurrentTime);
		}
	}
}

void UIMessageRoot::Draw()
{
	if (_VisibleBitset.any() == false)
	{
		return;
	}

	for (auto it = _ObjectsMap.rbegin(); it != _ObjectsMap.rend(); ++it)
	{
		if (it->second->IsGfxFlag(GFX_FLAG::GFX_HIDDEN) == false)
		{
			it->second->Draw();
			return;
		}
	}
}

void UIMessageRoot::Draw3D()
{
	if (_VisibleBitset.any() == false)
	{
		return;
	}

	for (auto it = _ObjectsMap.rbegin(); it != _ObjectsMap.rend(); ++it)
	{
		if (it->second->IsGfxFlag(GFX_FLAG::GFX_HIDDEN) == false)
		{
			it->second->Draw3D();
			return;
		}
	}
}

void UIMessageRoot::PostDraw()
{
	if (_VisibleBitset.any() == false)
	{
		return;
	}

	_Background->Draw();

	for (auto it = _ObjectsMap.rbegin(); it != _ObjectsMap.rend(); ++it)
	{
		if (it->second->IsGfxFlag(GFX_FLAG::GFX_HIDDEN) == false)
		{
			it->second->PostDraw();
			return;
		}
	}
}

UIBase *UIMessageRoot::GetFocus(mu_int32 mouseX, mu_int32 mouseY)
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

mu_uint32 UIMessageRoot::OnPreKeyPressed(const SDL_Keysym &key)
{
	for (auto it = _ObjectsMap.rbegin(); it != _ObjectsMap.rend(); ++it)
	{
		if (it->second->IsGfxFlag(GFX_FLAG::GFX_HIDDEN) == false)
		{
			return it->second->OnPreKeyPressed(key);
		}
	}

	return GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE;
}

mu_uint32 UIMessageRoot::OnKeyPressed(const SDL_Keysym &key)
{
	for (auto it = _ObjectsMap.rbegin(); it != _ObjectsMap.rend(); ++it)
	{
		if (it->second->IsGfxFlag(GFX_FLAG::GFX_HIDDEN) == false)
		{
			return it->second->OnKeyPressed(key);
		}
	}

	return GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE;
}

void UIMessageRoot::OnGFxSceneMessage(GFX_MESSAGE &objMsg)
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
		if (objMsg.ObjectID >= MESSAGEBOX::MESSAGE_HELPERITEMADD_TYPE_BEGIN &&
			objMsg.ObjectID <= MESSAGEBOX::MESSAGE_HELPERITEMADD_TYPE_END)
		{
			mu_int32 resIndex = objMsg.ObjectID - MESSAGEBOX::MESSAGE_HELPERITEMADD_TYPE_BEGIN;

			g_pHelperItemAdd->_TypeSelected = g_pHelperItemAdd->_TypeListIndex + resIndex;
			g_pHelperItemAdd->_ItemSelected = -1;
			g_pHelperItemAdd->CheckTypeList();
			g_pHelperItemAdd->ConfigureTypeList();
			g_pHelperItemAdd->ConfigureItemScroll();
			g_pHelperItemAdd->ConfigureItemList();
		}
		else if (objMsg.ObjectID >= MESSAGEBOX::MESSAGE_HELPERITEMADD_ITEM_BEGIN &&
			objMsg.ObjectID <= MESSAGEBOX::MESSAGE_HELPERITEMADD_ITEM_END)
		{
			mu_int32 resIndex = objMsg.ObjectID - MESSAGEBOX::MESSAGE_HELPERITEMADD_ITEM_BEGIN;

			g_pHelperItemAdd->_ItemSelected = g_pHelperItemAdd->_ItemListIndex + resIndex;
			g_pHelperItemAdd->ConfigureItemList();
			g_pHelperItemAdd->ConfigureItemInfo();
		}
	}
	else if (objMsg.Message == GFX_MESSAGE_TYPE::GFX_PRESSED)
	{
		if (objMsg.ObjectID >= MESSAGEBOX::MESSAGE_HELPERSKILLSELECT_SKILL_BEGIN &&
			objMsg.ObjectID <= MESSAGEBOX::MESSAGE_HELPERSKILLSELECT_SKILL_END)
		{
			mu_uint32 index = objMsg.ObjectID - MESSAGEBOX::MESSAGE_HELPERSKILLSELECT_SKILL_BEGIN;
			g_pHelperSkillSelect->SetSelectedSkill(g_pHelperSkillSelect->_SkillSelectIndex[index]);
			g_pHelperSkillSelect->ConfigureIcons();

			HELPER_HUNTINGSETTING &setting = g_pHelperSetup->_InternalSetting.Hunting;
			HELPER_SKILLSETTING &skillSetting = setting.Skills[g_pHelperSkillSelect->_SkillSlot];

			skillSetting.SkillIndex = g_pHelperSkillSelect->_SkillSelectIndex[index];
			g_pHelperSetup->_HTSkillIcon[g_pHelperSkillSelect->_SkillSlot]->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
			ConfigureSkill(g_pHelperSetup->_HTSkillIcon[g_pHelperSkillSelect->_SkillSlot], g_pHelperSkillSelect->_SkillSelectIndex[index]);
		}
		else
		{
			switch (objMsg.ObjectID)
			{
			case MESSAGEBOX::MESSAGE_OKBOX_OKBUTTON:
				{
					Hide(MESSAGEBOX::INTERFACE_OKBOX);
				}
				break;

			case MESSAGEBOX::MESSAGE_FAILEDCONNECT_OKBUTTON:
				{
					Hide(MESSAGEBOX::INTERFACE_FAILEDCONNECT);
				}
				break;

			case MESSAGEBOX::MESSAGE_SERVERISFULL_OKBUTTON:
				{
					Hide(MESSAGEBOX::INTERFACE_SERVERISFULL);
				}
				break;

			case MESSAGEBOX::MESSAGE_BANNEDFROMSERVER_OKBUTTON:
				{
					Hide(MESSAGEBOX::INTERFACE_BANNEDFROMSERVER);
				}
				break;

			case MESSAGEBOX::MESSAGE_BLOCKEDFROMSERVER_OKBUTTON:
				{
					Hide(MESSAGEBOX::INTERFACE_BLOCKEDFROMSERVER);
				}
				break;

			case MESSAGEBOX::MESSAGE_DELETECHARACTER_ACCEPTBUTTON:
				{
					if (SelectedHero == -1 ||
						SelectedHero < BEGIN_CHARACTERLIST_COUNT + 0 ||
						SelectedHero >= BEGIN_CHARACTERLIST_COUNT + MAX_CHARACTERLIST_COUNT)
						break;

					CL2GS_REQUEST_DELETECHARACTER RequestDeleteCharacter;

					RequestDeleteCharacter.SlotIndex = SelectedHero - BEGIN_CHARACTERLIST_COUNT;

					g_GameServerObject->Send(&RequestDeleteCharacter);

					this->Show(MESSAGEBOX::INTERFACE_WAITINGBOX);
					this->_WaitingBox->SetMessage(_T("Waiting for response"));
				}
				break;

			case MESSAGEBOX::MESSAGE_DELETECHARACTER_CANCELBUTTON:
				{
					Hide(MESSAGEBOX::INTERFACE_DELETECHARACTER);
				}
				break;

			case MESSAGEBOX::MESSAGE_DISCONNECTED_OKBUTTON:
				{
					SDL_Event event;
					SDL_memset(&event, 0, sizeof(event));
					event.type = SDL_QUIT;
					SDL_PushEvent(&event);
				}
				break;

			case MESSAGEBOX::MESSAGE_GUILDLEAVE_ACCEPTBUTTON:
				{
					Hide(MESSAGEBOX::INTERFACE_GUILDLEAVE);
					if (_GuildLeaveBox->_IsKickMember == true)
					{
						g_GameServerObject->SendGuildLeave(_GuildLeaveBox->_MemberToKick.CharacterIndex);
					}
					else
					{
						g_GameServerObject->SendGuildLeave(-1);
					}
				}
				break;

			case MESSAGEBOX::MESSAGE_GUILDLEAVE_CANCELBUTTON:
				{
					Hide(MESSAGEBOX::INTERFACE_GUILDLEAVE);
				}
				break;

			case MESSAGEBOX::MESSAGE_GUILDREQUEST_ACCEPTBUTTON:
				{
					Hide(MESSAGEBOX::INTERFACE_GUILDREQUEST);
					g_GameServerObject->SendGuildAnswer(true);
				}
				break;

			case MESSAGEBOX::MESSAGE_GUILDREQUEST_CANCELBUTTON:
				{
					Hide(MESSAGEBOX::INTERFACE_GUILDREQUEST);
					g_GameServerObject->SendGuildAnswer(false);
				}
				break;

			case MESSAGEBOX::MESSAGE_GUILDCANCELSTATUS_ACCEPTBUTTON:
				{
					Hide(MESSAGEBOX::INTERFACE_GUILDCANCELSTATUS);
					g_GameServerObject->SendGuildCancelStatus(_GuildCancelStatusBox->_Member.CharacterIndex);
				}
				break;

			case MESSAGEBOX::MESSAGE_GUILDCANCELSTATUS_CANCELBUTTON:
				{
					Hide(MESSAGEBOX::INTERFACE_GUILDCANCELSTATUS);
				}
				break;

			case MESSAGEBOX::MESSAGE_GUILDGIVESTATUS_BATTLEMASTER_BUTTON:
				{
					Hide(MESSAGEBOX::INTERFACE_GUILDGIVESTATUS);
					g_GameServerObject->SendGuildGiveStatus(_GuildGiveStatusBox->_Member.CharacterIndex, GAME::G_BATTLE_MASTER);
				}
				break;

			case MESSAGEBOX::MESSAGE_GUILDGIVESTATUS_SUBMASTER_BUTTON:
				{
					Hide(MESSAGEBOX::INTERFACE_GUILDGIVESTATUS);
					g_GameServerObject->SendGuildGiveStatus(_GuildGiveStatusBox->_Member.CharacterIndex, GAME::G_SUB_MASTER);
				}
				break;

			case MESSAGEBOX::MESSAGE_GUILDGIVESTATUS_CANCELBUTTON:
				{
					Hide(MESSAGEBOX::INTERFACE_GUILDGIVESTATUS);
				}
				break;

			case MESSAGEBOX::MESSAGE_PARTYREQUEST_ACCEPTBUTTON:
				{
					Hide(MESSAGEBOX::INTERFACE_PARTYREQUEST);
					g_GameServerObject->SendPartyAnswer(true);
				}
				break;

			case MESSAGEBOX::MESSAGE_PARTYREQUEST_CANCELBUTTON:
				{
					Hide(MESSAGEBOX::INTERFACE_PARTYREQUEST);
					g_GameServerObject->SendPartyAnswer(false);
				}
				break;

			case MESSAGEBOX::MESSAGE_TRADEREQUEST_ACCEPTBUTTON:
				{
					Hide(MESSAGEBOX::INTERFACE_TRADEREQUEST);
					g_GameServerObject->SendTradeAnswer(true);
				}
				break;

			case MESSAGEBOX::MESSAGE_TRADEREQUEST_CANCELBUTTON:
				{
					Hide(MESSAGEBOX::INTERFACE_TRADEREQUEST);
					g_GameServerObject->SendTradeAnswer(false);
				}
				break;

			case MESSAGEBOX::MESSAGE_TRADEMONEY_ACCEPTBUTTON:
				{
					mu_text textBuffer[32] = { 0 };
					_TradeMoneyBox->_GoldInput->GetText(textBuffer, mu_countof(textBuffer));

					if (mu_strlen(textBuffer) == 0)
					{
						break;
					}

					mu_int64 goldInput = boost::lexical_cast<mu_int64>(textBuffer);

					if (goldInput < 0)
					{
						break;
					}

					if (goldInput <= CharacterMachine->Gold)
					{
						g_GameServerObject->SendTradeMoney(goldInput);
					}
					else
					{
						this->Show(MESSAGEBOX::INTERFACE_OKBOX);
						g_pOkBox->ClearMessages();
						g_pOkBox->AddMessage(FONT_NORMAL, GlobalText[423]);
						g_pOkBox->Configure();
					}

					Hide(MESSAGEBOX::INTERFACE_TRADEMONEY);
				}
				break;

			case MESSAGEBOX::MESSAGE_TRADEMONEY_CANCELBUTTON:
				{
					Hide(MESSAGEBOX::INTERFACE_TRADEMONEY);
				}
				break;

			case MESSAGEBOX::MESSAGE_TRADEACCEPT_ACCEPTBUTTON:
				{
					g_pTrade->_OkButton->ChangeState(UIToggleButton::UI_PRESSED);
					g_GameServerObject->SendTradeOk(true);

					Hide(MESSAGEBOX::INTERFACE_TRADEACCEPT);
				}
				break;

			case MESSAGEBOX::MESSAGE_TRADEACCEPT_CANCELBUTTON:
				{
					Hide(MESSAGEBOX::INTERFACE_TRADEACCEPT);
				}
				break;

			case MESSAGEBOX::MESSAGE_STORAGEDEPOSIT_ACCEPTBUTTON:
				{
					mu_text textBuffer[32] = { 0 };
					_StorageDepositBox->_GoldInput->GetText(textBuffer, mu_countof(textBuffer));

					if (mu_strlen(textBuffer) == 0)
					{
						break;
					}

					mu_int64 goldInput = boost::lexical_cast<mu_int64>(textBuffer);

					if (goldInput <= 0)
					{
						break;
					}

					if (goldInput <= CharacterMachine->Gold)
					{
						g_GameServerObject->SendStorageDeposit(goldInput);
					}
					else
					{
						this->Show(MESSAGEBOX::INTERFACE_OKBOX);
						g_pOkBox->ClearMessages();
						g_pOkBox->AddMessage(FONT_NORMAL, GlobalText[423]);
						g_pOkBox->Configure();
					}

					Hide(MESSAGEBOX::INTERFACE_STORAGEDEPOSITBOX);
				}
				break;

			case MESSAGEBOX::MESSAGE_STORAGEDEPOSIT_CANCELBUTTON:
				{
					Hide(MESSAGEBOX::INTERFACE_STORAGEDEPOSITBOX);
				}
				break;

			case MESSAGEBOX::MESSAGE_STORAGEWITHDRAW_ACCEPTBUTTON:
				{
					mu_text textBuffer[32] = { 0 };
					_StorageWithdrawBox->_GoldInput->GetText(textBuffer, mu_countof(textBuffer));

					if (mu_strlen(textBuffer) == 0)
					{
						break;
					}

					mu_int64 goldInput = boost::lexical_cast<mu_int64>(textBuffer);

					if (goldInput <= 0)
					{
						break;
					}

					if (goldInput <= CharacterMachine->StorageGold)
					{
						if (CharacterMachine->Gold + goldInput >= MAX_ZEN)
						{
							goldInput = MAX_ZEN - CharacterMachine->Gold;
						}

						if (goldInput > 0)
						{
							if (g_pStorageInventory->IsStorageLocked() == true &&
								g_pStorageInventory->IsStorageAllowed() == false)
							{
								g_pStorageInventory->SetAutoWithdraw(goldInput);
								this->Show(MESSAGEBOX::INTERFACE_STORAGELOCKBOX);
								this->_StorageLockBox->SetRequestState(CL2GS_REQUEST_STORAGELOCK::REQUEST_ALLOW);
							}
							else
							{
								g_GameServerObject->SendStorageWithdraw(goldInput);
							}
						}
					}
					else
					{
						this->Show(MESSAGEBOX::INTERFACE_OKBOX);
						g_pOkBox->ClearMessages();
						g_pOkBox->AddMessage(FONT_NORMAL, GlobalText[423]);
						g_pOkBox->Configure();
					}

					Hide(MESSAGEBOX::INTERFACE_STORAGEWITHDRAWBOX);
				}
				break;

			case MESSAGEBOX::MESSAGE_STORAGEWITHDRAW_CANCELBUTTON:
				{
					Hide(MESSAGEBOX::INTERFACE_STORAGEWITHDRAWBOX);
				}
				break;

			case MESSAGEBOX::MESSAGE_STORAGELOCK_ACCEPTBUTTON:
				{
					mu_text textBuffer[32] = { 0 };
					_StorageLockBox->_PasswordInput->GetText(textBuffer, mu_countof(textBuffer));

					if (mu_strlen(textBuffer) == 0)
					{
						break;
					}

					mu_regex passwordRegex(STORAGE_REGEX);

					if (boost::regex_match(textBuffer, passwordRegex) == false)
					{
						_StorageLockBox->_PasswordInput->SelectAll();
						_StorageLockBox->_PasswordInput->GiveFocus();
						break;
					}

					CL2GS_REQUEST_STORAGELOCK RequestStorageLock;

					RequestStorageLock.RequestType = _StorageLockBox->_RequestState;
					RequestStorageLock.Password = textBuffer;

					g_GameServerObject->Send(&RequestStorageLock);

					Hide(MESSAGEBOX::INTERFACE_STORAGELOCKBOX);
				}
				break;

			case MESSAGEBOX::MESSAGE_STORAGELOCK_CANCELBUTTON:
				{
					if (g_pStorageInventory->GetCurrentAction() != UIStorageInventory::WHAUTO_NONE &&
						_StorageLockBox->_RequestState == CL2GS_REQUEST_STORAGELOCK::REQUEST_ALLOW)
					{
						g_pStorageInventory->ClearAuto();
					}

					Hide(MESSAGEBOX::INTERFACE_STORAGELOCKBOX);
				}
				break;

			case MESSAGEBOX::MESSAGE_HIGHVALUE_ACCEPTBUTTON:
				{
					ITEM *Item = g_pPickedItem.GetItem();

					if (g_pPickedItem.GetOwnerInventory() == g_pMyInventory->GetInventoryControl())
					{
						g_GameServerObject->SendRequestSell(INVENTORY_TYPE::IT_INVENTORY, Item->x, Item->y);
					}
					else if (g_pPickedItem.GetOwnerInventory() == nullptr)
					{
						g_GameServerObject->SendRequestSell(INVENTORY_TYPE::IT_EQUIPMENT, g_pPickedItem.GetSourceLinealPos(), 0);
					}

					g_pPickedItem.BackupPickedItem();
					Hide(MESSAGEBOX::INTERFACE_HIGHVALUEWARNINGBOX);
				}
				break;

			case MESSAGEBOX::MESSAGE_HIGHVALUE_CANCELBUTTON:
				{
					g_pPickedItem.BackupPickedItem();
					Hide(MESSAGEBOX::INTERFACE_HIGHVALUEWARNINGBOX);
				}
				break;

			case MESSAGEBOX::MESSAGE_GAMBLEBUY_ACCEPTBUTTON:
				{
					ITEM *Item = g_pPickedItem.GetItem();

					g_GameServerObject->SendRequestBuy(Item->x, Item->y);

					g_pPickedItem.BackupPickedItem();
					Hide(MESSAGEBOX::INTERFACE_GAMBLEBUY);
				}
				break;

			case MESSAGEBOX::MESSAGE_GAMBLEBUY_CANCELBUTTON:
				{
					g_pPickedItem.BackupPickedItem();
					Hide(MESSAGEBOX::INTERFACE_GAMBLEBUY);
				}
				break;

			case MESSAGEBOX::MESSAGE_PCPOINTBUY_ACCEPTBUTTON:
				{
					ITEM *Item = g_pPickedItem.GetItem();

					g_GameServerObject->SendRequestBuy(Item->x, Item->y);

					g_pPickedItem.BackupPickedItem();
					Hide(MESSAGEBOX::INTERFACE_PCPOINTBUY);
				}
				break;

			case MESSAGEBOX::MESSAGE_PCPOINTBUY_CANCELBUTTON:
				{
					g_pPickedItem.BackupPickedItem();
					Hide(MESSAGEBOX::INTERFACE_PCPOINTBUY);
				}
				break;

			case MESSAGEBOX::MESSAGE_MIXMENU_GENERALBUTTON:
				{
					g_MixManager.SetMixType(MIXTYPE_GOBLIN_NORMAL);
					g_pMixInventory->Configure();
					Hide(MESSAGEBOX::INTERFACE_CHAOSMIXMENU);
				}
				break;

			case MESSAGEBOX::MESSAGE_MIXMENU_CHAOSBUTTON:
				{
					g_MixManager.SetMixType(MIXTYPE_GOBLIN_CHAOSITEM);
					g_pMixInventory->Configure();
					Hide(MESSAGEBOX::INTERFACE_CHAOSMIXMENU);
				}
				break;

			case MESSAGEBOX::MESSAGE_MIXMENU_380BUTTON:
				{
					g_MixManager.SetMixType(MIXTYPE_GOBLIN_ADD380);
					g_pMixInventory->Configure();
					Hide(MESSAGEBOX::INTERFACE_CHAOSMIXMENU);
				}
				break;

			case MESSAGEBOX::MESSAGE_MIXMENU_CANCELBUTTON:
				{
					g_pNewUISystem->Hide(GAMEUI::INTERFACE_INVENTORY);

					Hide(MESSAGEBOX::INTERFACE_CHAOSMIXMENU);
				}
				break;

			case MESSAGEBOX::MESSAGE_MIXCHECK_ACCEPTBUTTON:
				{
					g_pMixInventory->SetMixState(UIMixInventory::MIX_PROCESSING);

					CL2GS_REQUEST_MIX RequestMix;

					RequestMix.Type = g_MixManager.GetCurMixID();
					RequestMix.SubType = g_MixManager.GetMixSubType();

					g_GameServerObject->Send(&RequestMix);

					Hide(MESSAGEBOX::INTERFACE_MIXCHECK);
				}
				break;

			case MESSAGEBOX::MESSAGE_MIXCHECK_CANCELBUTTON:
				{
					Hide(MESSAGEBOX::INTERFACE_MIXCHECK);
				}
				break;

			case MESSAGEBOX::MESSAGE_BCRANK_OKBUTTON:
				{
					Hide(MESSAGEBOX::INTERFACE_BLOODCASTLE_RANK);
				}
				break;

				// Helper Setting
			case MESSAGEBOX::MESSAGE_HELPERSKILL_OKBUTTON:
				{
					HELPER_HUNTINGSETTING &setting = g_pHelperSetup->_InternalSetting.Hunting;
					HELPER_SKILLSETTING &skillSetting = setting.Skills[g_pHelperSkillSetting->_ActIndex];

					UIRadioButton *huntingTypeRadios[] =
					{
						g_pHelperSkillSetting->_HuntingRange,
						g_pHelperSkillSetting->_HuntingAttacker,
					};

					for (mu_uint32 n = HELPER_SKILLSETTING::HSS_CON_BEGIN; n < mu_countof(huntingTypeRadios); ++n)
					{
						mu_uint32 currentState = huntingTypeRadios[n]->GetState();
						if (currentState == UIRadioButton::UI_PRESSED ||
							currentState == UIRadioButton::UI_PRESSED_OVER)
						{
							skillSetting.AttackType = n;
							break;
						}
					}

					UIRadioButton *mobsTypeRadios[] =
					{
						g_pHelperSkillSetting->_TwoPlus,
						g_pHelperSkillSetting->_ThreePlus,
						g_pHelperSkillSetting->_FourPlus,
						g_pHelperSkillSetting->_FivePlus,
					};

					for (mu_uint32 n = HELPER_SKILLSETTING::HSS_MOB_BEGIN; n < mu_countof(mobsTypeRadios); ++n)
					{
						mu_uint32 currentState = mobsTypeRadios[n]->GetState();
						if (currentState == UIRadioButton::UI_PRESSED ||
							currentState == UIRadioButton::UI_PRESSED_OVER)
						{
							skillSetting.MobsCountType = n;
							break;
						}
					}

					Hide(MESSAGEBOX::INTERFACE_HELPER_SKILLSETTING);
				}
				break;

			case MESSAGEBOX::MESSAGE_HELPERPOTION_OKBUTTON:
				{
					HELPER_HUNTINGSETTING &setting = g_pHelperSetup->_InternalSetting.Hunting;
					HELPER_POTIONSETTING &potionSetting = setting.PotionSetting;

					potionSetting.MinLifePercent = g_pHelperPotionSetting->_MinLifeSlider->GetCurrentValue();

					Hide(MESSAGEBOX::INTERFACE_HELPER_POTIONSETTING);
				}
				break;

			case MESSAGEBOX::MESSAGE_HELPERELFPOTION_OKBUTTON:
				{
					HELPER_HUNTINGSETTING &setting = g_pHelperSetup->_InternalSetting.Hunting;
					HELPER_POTIONSETTING &potionSetting = setting.PotionSetting;

					potionSetting.MinLifePercent = g_pHelperElfPotionSetting->_MinLifeSlider->GetCurrentValue();
					potionSetting.MinAutoHealPercent = g_pHelperElfPotionSetting->_MinAutoHealSlider->GetCurrentValue();

					Hide(MESSAGEBOX::INTERFACE_HELPER_ELFPOTIONSETTING);
				}
				break;

			case MESSAGEBOX::MESSAGE_HELPERPARTY_OKBUTTON:
				{
					HELPER_HUNTINGSETTING &setting = g_pHelperSetup->_InternalSetting.Hunting;
					HELPER_PARTYSETTING &partySetting = setting.PartySetting;

					switch (GetBaseClass(Hero->Class))
					{
					case GAME::CLASS_WIZARD:
					case GAME::CLASS_KNIGHT:
					case GAME::CLASS_DARK_LORD:
					case GAME::CLASS_SUMMONER:
						{
							Hide(MESSAGEBOX::INTERFACE_HELPER_WIZARDPARTYSETTING);
						}
						break;

					default:
						{
							partySetting.EnableHealMin = g_pHelperPartySetting->_EnableHealCheckBox->GetState() >= UIToggleButton::UI_PRESSED;
							partySetting.HealMin = g_pHelperPartySetting->_MinHealSlider->GetCurrentValue();
							Hide(MESSAGEBOX::INTERFACE_HELPER_PARTYSETTING);
						}
						break;
					}
				}
				break;

			case MESSAGEBOX::MESSAGE_HELPERSKILLSELECT_LEFT_BUTTON:
				{
					g_pHelperSkillSelect->DecreasePage();
					g_pHelperSkillSelect->ConfigureIcons();
				}
				break;

			case MESSAGEBOX::MESSAGE_HELPERSKILLSELECT_RIGHT_BUTTON:
				{
					g_pHelperSkillSelect->IncreasePage();
					g_pHelperSkillSelect->ConfigureIcons();
				}
				break;

			case MESSAGEBOX::MESSAGE_HELPERSKILLSELECT_OKBUTTON:
				{
					Hide(MESSAGEBOX::INTERFACE_HELPER_SKILLSELECT);
				}
				break;

			case MESSAGEBOX::MESSAGE_HELPERITEMADD_DECLEVEL_BUTTON:
				{
					g_pHelperItemAdd->DecreaseLevel();
				}
				break;

			case MESSAGEBOX::MESSAGE_HELPERITEMADD_INCLEVEL_BUTTON:
				{
					g_pHelperItemAdd->IncreaseLevel();
				}
				break;

			case MESSAGEBOX::MESSAGE_HELPERITEMADD_ACCEPT_BUTTON:
				{
					if (g_pHelperItemAdd->_TypeSelected >= 0 &&
						g_pHelperItemAdd->_ItemSelected >= 0)
					{
						MUItemCategoriesData &typeData = g_ItemCategoriesDatabase._Categories[g_pHelperItemAdd->_TypeSelected];
						MUItemCategoryItemData &itemData = typeData.Items[g_pHelperItemAdd->_ItemSelected];

						HELPER_ITEMSETTING itemSetting;

						itemSetting.ItemIndex = itemData.ItemIndex;
						itemSetting.ItemLevel = itemData.ItemLevel;
						itemSetting.MinLevel = g_pHelperItemAdd->_MinItemLevel;
						itemSetting.IsLuck = g_pHelperItemAdd->_LuckCheckBox->GetState() >= UIToggleButton::UI_PRESSED;
						itemSetting.IsSkill = g_pHelperItemAdd->_SkillCheckBox->GetState() >= UIToggleButton::UI_PRESSED;
						itemSetting.IsOption = g_pHelperItemAdd->_OptionCheckBox->GetState() >= UIToggleButton::UI_PRESSED;
						itemSetting.IsExcellent = g_pHelperItemAdd->_ExcellentCheckBox->GetState() >= UIToggleButton::UI_PRESSED;
						itemSetting.IsSetItem = g_pHelperItemAdd->_SetItemCheckBox->GetState() >= UIToggleButton::UI_PRESSED;
						itemSetting.IsSocket = g_pHelperItemAdd->_SocketCheckBox->GetState() >= UIToggleButton::UI_PRESSED;

						if (g_pHelperItemAdd->_HelperIndex >= 0)
						{
							g_pHelperSetup->ModifyObtainItem(g_pHelperItemAdd->_HelperIndex, itemSetting);
						}
						else
						{
							g_pHelperSetup->AddObtainItem(itemSetting);
						}

						Hide(MESSAGEBOX::INTERFACE_HELPER_ITEMADD);
					}
				}
				break;

			case MESSAGEBOX::MESSAGE_HELPERITEMADD_CANCEL_BUTTON:
				{
					Hide(MESSAGEBOX::INTERFACE_HELPER_ITEMADD);
				}
				break;

			case MESSAGEBOX::MESSAGE_HELPERSTART_ONLINE_BUTTON:
				{
					g_GameServerObject->SendHelperStart(CL2GS_REQUEST_HELPERSTART::ONLINE_MODE);
					Hide(MESSAGEBOX::INTERFACE_HELPER_START);
				}
				break;

			case MESSAGEBOX::MESSAGE_HELPERSTART_OFFLINE_BUTTON:
				{
					g_GameServerObject->SendHelperStart(CL2GS_REQUEST_HELPERSTART::OFFLINE_MODE);
					Hide(MESSAGEBOX::INTERFACE_HELPER_START);
				}
				break;

			case MESSAGEBOX::MESSAGE_HELPERSTART_CANCEL_BUTTON:
				{
					Hide(MESSAGEBOX::INTERFACE_HELPER_START);
				}
				break;
			}
		}
	}
}

void UIMessageRoot::Show(mu_uint32 Key)
{
	switch (Key)
	{
	case MESSAGEBOX::INTERFACE_OKBOX:
		{
			_OkBox->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case MESSAGEBOX::INTERFACE_FAILEDCONNECT:
		{
			_FailedConnect->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case MESSAGEBOX::INTERFACE_SERVERISFULL:
		{
			_ServerFull->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case MESSAGEBOX::INTERFACE_BANNEDFROMSERVER:
		{
			_BannedFromServer->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case MESSAGEBOX::INTERFACE_BLOCKEDFROMSERVER:
		{
			_BlockedFromServer->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case MESSAGEBOX::INTERFACE_WAITINGBOX:
		{
			_WaitingBox->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case MESSAGEBOX::INTERFACE_CONNECT:
		{
			_ConnectBox->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case MESSAGEBOX::INTERFACE_DELETECHARACTER:
		{
			_DeleteCharacter->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case MESSAGEBOX::INTERFACE_DISCONNECTED:
		{
			_DisconnectedBox->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case MESSAGEBOX::INTERFACE_EXITGAME:
		{
			_ExitGameBox->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case MESSAGEBOX::INTERFACE_CHANGESERVER:
		{
			_ChangeServerBox->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case MESSAGEBOX::INTERFACE_GUILDLEAVE:
		{
			_GuildLeaveBox->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case MESSAGEBOX::INTERFACE_GUILDREQUEST:
		{
			_GuildRequestBox->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case MESSAGEBOX::INTERFACE_GUILDGIVESTATUS:
		{
			_GuildGiveStatusBox->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case MESSAGEBOX::INTERFACE_GUILDCANCELSTATUS:
		{
			_GuildCancelStatusBox->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case MESSAGEBOX::INTERFACE_PARTYREQUEST:
		{
			_PartyRequestBox->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case MESSAGEBOX::INTERFACE_TRADEREQUEST:
		{
			_TradeRequestBox->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case MESSAGEBOX::INTERFACE_TRADEMONEY:
		{
			_TradeMoneyBox->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
			_TradeMoneyBox->_GoldInput->ClearAllText();
			_TradeMoneyBox->_GoldInput->GiveFocus();
		}
		break;

	case MESSAGEBOX::INTERFACE_TRADEACCEPT:
		{
			_TradeAcceptBox->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case MESSAGEBOX::INTERFACE_STORAGEDEPOSITBOX:
		{
			_StorageDepositBox->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
			_StorageDepositBox->_GoldInput->ClearAllText();
			_StorageDepositBox->_GoldInput->GiveFocus();
		}
		break;

	case MESSAGEBOX::INTERFACE_STORAGEWITHDRAWBOX:
		{
			_StorageWithdrawBox->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
			_StorageWithdrawBox->_GoldInput->ClearAllText();
			_StorageWithdrawBox->_GoldInput->GiveFocus();
		}
		break;

	case MESSAGEBOX::INTERFACE_STORAGELOCKBOX:
		{
			_StorageLockBox->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
			_StorageLockBox->_PasswordInput->ClearAllText();
			_StorageLockBox->_PasswordInput->GiveFocus();
		}
		break;

	case MESSAGEBOX::INTERFACE_HIGHVALUEWARNINGBOX:
		{
			_HighValueWarningBox->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case MESSAGEBOX::INTERFACE_GAMBLEBUY:
		{
			_GambleBuyBox->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case MESSAGEBOX::INTERFACE_PCPOINTBUY:
		{
			_PCPointBuyBox->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case MESSAGEBOX::INTERFACE_CHAOSMIXMENU:
		{
			_ChaosMixMenuBox->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case MESSAGEBOX::INTERFACE_MIXCHECK:
		{
			_MixCheckBox->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case MESSAGEBOX::INTERFACE_BLOODCASTLE_RANK:
		{
			_BloodCastleRank->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case MESSAGEBOX::INTERFACE_HELPER_SKILLSETTING:
		{
			_HelperSkillSetting->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case MESSAGEBOX::INTERFACE_HELPER_POTIONSETTING:
		{
			_HelperPotionSetting->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case MESSAGEBOX::INTERFACE_HELPER_ELFPOTIONSETTING:
		{
			_HelperElfPotionSetting->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case MESSAGEBOX::INTERFACE_HELPER_PARTYSETTING:
		{
			_HelperPartySetting->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case MESSAGEBOX::INTERFACE_HELPER_WIZARDPARTYSETTING:
		{
			_HelperWizardPartySetting->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case MESSAGEBOX::INTERFACE_HELPER_SKILLSELECT:
		{
			_HelperSkillSelect->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case MESSAGEBOX::INTERFACE_HELPER_ITEMADD:
		{
			_HelperItemAdd->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case MESSAGEBOX::INTERFACE_HELPER_START:
		{
			_HelperStart->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;
	}

	_VisibleBitset.set(Key, true);

	//UpdateWindowPosition();
}

void UIMessageRoot::Hide(mu_uint32 Key)
{
	switch (Key)
	{
	case MESSAGEBOX::INTERFACE_OKBOX:
		{
			_OkBox->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case MESSAGEBOX::INTERFACE_FAILEDCONNECT:
		{
			_FailedConnect->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case MESSAGEBOX::INTERFACE_SERVERISFULL:
		{
			_ServerFull->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case MESSAGEBOX::INTERFACE_BANNEDFROMSERVER:
		{
			_BannedFromServer->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case MESSAGEBOX::INTERFACE_BLOCKEDFROMSERVER:
		{
			_BlockedFromServer->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case MESSAGEBOX::INTERFACE_WAITINGBOX:
		{
			_WaitingBox->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case MESSAGEBOX::INTERFACE_CONNECT:
		{
			_ConnectBox->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case MESSAGEBOX::INTERFACE_DELETECHARACTER:
		{
			_DeleteCharacter->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case MESSAGEBOX::INTERFACE_DISCONNECTED:
		{
			_DisconnectedBox->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case MESSAGEBOX::INTERFACE_EXITGAME:
		{
			_ExitGameBox->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case MESSAGEBOX::INTERFACE_CHANGESERVER:
		{
			_ChangeServerBox->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case MESSAGEBOX::INTERFACE_GUILDLEAVE:
		{
			_GuildLeaveBox->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case MESSAGEBOX::INTERFACE_GUILDREQUEST:
		{
			_GuildRequestBox->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case MESSAGEBOX::INTERFACE_GUILDGIVESTATUS:
		{
			_GuildGiveStatusBox->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case MESSAGEBOX::INTERFACE_GUILDCANCELSTATUS:
		{
			_GuildCancelStatusBox->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case MESSAGEBOX::INTERFACE_PARTYREQUEST:
		{
			_PartyRequestBox->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case MESSAGEBOX::INTERFACE_TRADEREQUEST:
		{
			_TradeRequestBox->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case MESSAGEBOX::INTERFACE_TRADEMONEY:
		{
			_TradeMoneyBox->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case MESSAGEBOX::INTERFACE_TRADEACCEPT:
		{
			_TradeAcceptBox->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case MESSAGEBOX::INTERFACE_STORAGEDEPOSITBOX:
		{
			_StorageDepositBox->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case MESSAGEBOX::INTERFACE_STORAGEWITHDRAWBOX:
		{
			_StorageWithdrawBox->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case MESSAGEBOX::INTERFACE_STORAGELOCKBOX:
		{
			_StorageLockBox->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case MESSAGEBOX::INTERFACE_HIGHVALUEWARNINGBOX:
		{
			_HighValueWarningBox->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case MESSAGEBOX::INTERFACE_GAMBLEBUY:
		{
			_GambleBuyBox->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case MESSAGEBOX::INTERFACE_PCPOINTBUY:
		{
			_PCPointBuyBox->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case MESSAGEBOX::INTERFACE_CHAOSMIXMENU:
		{
			_ChaosMixMenuBox->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case MESSAGEBOX::INTERFACE_MIXCHECK:
		{
			_MixCheckBox->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case MESSAGEBOX::INTERFACE_BLOODCASTLE_RANK:
		{
			_BloodCastleRank->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case MESSAGEBOX::INTERFACE_HELPER_SKILLSETTING:
		{
			_HelperSkillSetting->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case MESSAGEBOX::INTERFACE_HELPER_POTIONSETTING:
		{
			_HelperPotionSetting->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case MESSAGEBOX::INTERFACE_HELPER_ELFPOTIONSETTING:
		{
			_HelperElfPotionSetting->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case MESSAGEBOX::INTERFACE_HELPER_PARTYSETTING:
		{
			_HelperPartySetting->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case MESSAGEBOX::INTERFACE_HELPER_WIZARDPARTYSETTING:
		{
			_HelperWizardPartySetting->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case MESSAGEBOX::INTERFACE_HELPER_SKILLSELECT:
		{
			_HelperSkillSelect->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case MESSAGEBOX::INTERFACE_HELPER_ITEMADD:
		{
			_HelperItemAdd->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case MESSAGEBOX::INTERFACE_HELPER_START:
		{
			_HelperStart->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;
	}

	_VisibleBitset.set(Key, false);

	//UpdateWindowPosition();
}

mu_boolean UIMessageRoot::IsVisible(mu_uint32 Key)
{
	return _VisibleBitset.test(Key);
}