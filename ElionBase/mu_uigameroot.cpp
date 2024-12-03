#include "stdafx.h"
#include "mu_uigameroot.h"

#define SCREEN_WIDTH 640.0f
#define SCREEN_HEIGHT 480.0f

UIGameRoot *g_pNewUISystem = nullptr;

UIGameRoot::UIGameRoot()
{
	_SceneManager = nullptr;

	UIBase::SetAspectRatio(SCREEN_WIDTH, SCREEN_HEIGHT);
	UIBase::SetPosition(0.0f, 0.0f);
	UIBase::SetDimensions(SCREEN_WIDTH, SCREEN_HEIGHT);

	_SelectedObject = nullptr;
	_FocusObject = nullptr;

	SetRoot(this);

	_SimpleHpBar = new UISimpleHPBar(this);
	AddObject(GAMEUI::INTERFACE_SIMPLEHPBAR, _SimpleHpBar);

	_SimpleName = new UISimpleName(this);
	AddObject(GAMEUI::INTERFACE_SIMPLENAME, _SimpleName);

	_SimpleChat = new UISimpleChat(this);
	AddObject(GAMEUI::INTERFACE_SIMPLECHAT, _SimpleChat);

	_SimpleNotice = new UISimpleNotice(this);
	AddObject(GAMEUI::INTERFACE_SIMPLENOTICE, _SimpleNotice);

	_MainFrame = new UIMainFrame(this, 0.0f, 429.0f, 640.0f, 51.0f);
	AddObject(GAMEUI::INTERFACE_MAINFRAME, _MainFrame);

	_HotkeyFrame = new UIItemHotkey(this);
	AddObject(GAMEUI::INTERFACE_ITEMHOYKEY, _HotkeyFrame);

	_SkillFrame = new UISkillFrame(this);
	AddObject(GAMEUI::INTERFACE_SKILL_LIST, _SkillFrame);

	_CharacterInfo = new UICharacterInfo(this, 450.0f, 0.0f, 190.0f, 429.0f);
	AddObject(GAMEUI::INTERFACE_CHARACTER, _CharacterInfo);

	_MyQuestWindow = new UIQuestWindow(this);
	AddObject(GAMEUI::INTERFACE_MYQUEST, _MyQuestWindow);

	_NpcQuestWindow = new UINpcQuestWindow(this);
	AddObject(GAMEUI::INTERFACE_NPCQUEST, _NpcQuestWindow);

	_MyInventoryWindow = new UIMyInventory(this);
	AddObject(GAMEUI::INTERFACE_INVENTORY, _MyInventoryWindow);

	_MixInventoryWindow = new UIMixInventory(this);
	AddObject(GAMEUI::INTERFACE_MIXINVENTORY, _MixInventoryWindow);

	_StorageWindow = new UIStorageInventory(this);
	AddObject(GAMEUI::INTERFACE_STORAGE, _StorageWindow);

	_ShopWindow = new UIShopInventory(this);
	AddObject(GAMEUI::INTERFACE_NPCSHOP, _ShopWindow);

	_TradeWindow = new UITradeInventory(this);
	AddObject(GAMEUI::INTERFACE_TRADE, _TradeWindow);

	_PartyWindow = new UIPartyWindow(this);
	AddObject(GAMEUI::INTERFACE_PARTY, _PartyWindow);

	_PartyList = new UIPartyList(this);
	AddObject(GAMEUI::INTERFACE_PARTYLIST, _PartyList);

	_GuildWindow = new UIGuildWindow(this);
	AddObject(GAMEUI::INTERFACE_GUILDINFO, _GuildWindow);

	_GuildCreate = new UIGuildCreate(this);
	AddObject(GAMEUI::INTERFACE_NPCGUILDMASTER, _GuildCreate);

	_ChatInput = new UIChatInput(this);
	AddObject(GAMEUI::INTERFACE_CHATINPUTBOX, _ChatInput);

	_ChatWindow = new UIChatWindow(this, 0.0f, 283.0f, UIChatWindow::WND_WIDTH, UIChatWindow::WND_HEIGHT);
	AddObject(GAMEUI::INTERFACE_CHATLOGWINDOW, _ChatWindow);

	_MapName = new UIMapName(this);
	AddObject(GAMEUI::INTERFACE_MAPNAME, _MapName);

	_BuffWindow = new UIBuffStatus(this);
	AddObject(GAMEUI::INTERFACE_BUFF_WINDOW, _BuffWindow);

	_HeroPosition = new UIHeroPosition(this);
	AddObject(GAMEUI::INTERFACE_HERO_POSITION_INFO, _HeroPosition);

	_MoveWindow = new UIMoveWindow(this);
	AddObject(GAMEUI::INTERFACE_MOVEMAP, _MoveWindow);

	_BloodCastleTimer = new UIBloodCastleTimer(this);
	AddObject(GAMEUI::INTERFACE_BLOODCASTLE_TIME, _BloodCastleTimer);

	_BloodCastleEnter = new UIBloodCastleEnter(this);
	AddObject(GAMEUI::INTERFACE_BLOODCASTLE, _BloodCastleEnter);

	_CommandWindow = new UICommandWindow(this);
	AddObject(GAMEUI::INTERFACE_COMMAND, _CommandWindow);

	_HelperSetup = new UIHelperSetup(this);
	AddObject(GAMEUI::INTERFACE_HELPERSETUP, _HelperSetup);

	_MouseState = 0;
}

UIGameRoot::~UIGameRoot()
{

}

void UIGameRoot::AddObject(mu_uint32 index, UIBase *object)
{
	_ObjectsMap.insert(std::make_pair(index, object));
}

mu_uint32 UIGameRoot::OnWindowMessage(const SDL_Event *event, mu_size *result)
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
		if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT)
		{
			UIInventoryControl *ownerInventory = g_pPickedItem.GetOwnerInventory();
			ITEM *PickedItem = g_pPickedItem.GetItem();
			if (PickedItem != nullptr)
			{
				if (CheckMouseIn(0, 0, MU_TO_WINDOW(GetScreenWidth(), MU_RESOURCE->_640x480Rate[0]), MU_TO_WINDOW(429, MU_RESOURCE->_640x480Rate[1])))
				{
					selectedRet = GFX_MESSAGE_RETURN::GFX_MESSAGE_BYPASS;

					if (g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_NPCSHOP) == false &&
						g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_TRADE) == false &&
						g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_DEVILSQUARE) == false &&
						g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_BLOODCASTLE) == false &&
						g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_MIXINVENTORY) == false &&
						g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_STORAGE) == false &&
						g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_MYSHOP_INVENTORY) == false &&
						g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_PURCHASESHOP_INVENTORY) == false)
					{
						if (PickedItem->HarmonyOption != 0)
						{
							g_pChatListBox->AddText(_T(""), GlobalText[2217], TYPE_ERROR_MESSAGE);
							g_pPickedItem.BackupPickedItem();
						}
						else if (IsHighValueItem(PickedItem) == true)
						{
							g_pChatListBox->AddText(_T(""), GlobalText[269], TYPE_ERROR_MESSAGE);
							g_pPickedItem.BackupPickedItem();
						}
						else if (IsDropBan(PickedItem) == true)
						{
							g_pChatListBox->AddText(_T(""), GlobalText[1915], TYPE_ERROR_MESSAGE);
							g_pPickedItem.BackupPickedItem();
						}
						else if (PickedItem->Type == ITEM_POTION + 28 &&
							g_CursedTemple.IsCursedTemple() == true)
						{
							g_pPickedItem.BackupPickedItem();
						}
						else
						{
							mu_int32 tx = (mu_uint8)SelectXF;
							mu_int32 ty = (mu_uint8)SelectYF;

							if (ownerInventory == g_pMyInventory->GetInventoryControl())
							{
								if (Hero->Dead == 0)
								{
									g_GameServerObject->SendDropItem(INVENTORY_TYPE::IT_INVENTORY, PickedItem->x, PickedItem->y, tx, ty);
								}
							}
							else if (ownerInventory == nullptr &&
								PickedItem->ex_src_type == ITEM_EX_SRC_EQUIPMENT)
							{
								if (Hero->Dead == 0)
								{
									g_GameServerObject->SendDropItem(INVENTORY_TYPE::IT_EQUIPMENT, PickedItem->lineal_pos, 0, tx, ty);
								}
							}
						}
					}
				}
			}
		}

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

mu_uint32 UIGameRoot::OnPostWindowMessage(const SDL_Event *event, mu_size *result)
{
	switch (event->type)
	{
	case SDL_KEYDOWN:
		return OnPostKeyPressed(event->key.keysym);
	}

	return GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE;
}

void UIGameRoot::OnWindowCancelled()
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

void UIGameRoot::SystemConfigure()
{
	UIBase::SystemConfigure();

	for (auto it = _ObjectsMap.begin(); it != _ObjectsMap.end(); ++it)
	{
		it->second->SystemConfigure();
	}
}

void UIGameRoot::InitializeGPU()
{
	UIBase::InitializeGPU();

	for (auto it = _ObjectsMap.begin(); it != _ObjectsMap.end(); ++it)
	{
		it->second->InitializeGPU();
	}
}

void UIGameRoot::ReleaseGPU()
{
	UIBase::ReleaseGPU();

	for (auto it = _ObjectsMap.begin(); it != _ObjectsMap.end(); ++it)
	{
		it->second->ReleaseGPU();
	}
}

void UIGameRoot::Update(mu_uint64 CurrentTime)
{
	for (auto it = _ObjectsMap.begin(); it != _ObjectsMap.end(); ++it)
	{
		if (it->second->IsGfxFlag(GFX_FLAG::GFX_HIDDEN) == false)
		{
			it->second->Update(CurrentTime);
		}
	}
}

void UIGameRoot::Draw()
{
	for (auto it = _ObjectsMap.begin(); it != _ObjectsMap.end(); ++it)
	{
		if (it->second->IsGfxFlag(GFX_FLAG::GFX_HIDDEN) == false)
		{
			it->second->Draw();
		}
	}
}

void UIGameRoot::Draw3D()
{
	for (auto it = _ObjectsMap.begin(); it != _ObjectsMap.end(); ++it)
	{
		if (it->second->IsGfxFlag(GFX_FLAG::GFX_HIDDEN) == false)
		{
			it->second->Draw3D();
		}
	}
}

void UIGameRoot::PostDraw()
{
	for (auto it = _ObjectsMap.begin(); it != _ObjectsMap.end(); ++it)
	{
		if (it->second->IsGfxFlag(GFX_FLAG::GFX_HIDDEN) == false)
		{
			it->second->PostDraw();
		}
	}
}

UIBase *UIGameRoot::GetFocus(mu_int32 mouseX, mu_int32 mouseY)
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

mu_uint32 UIGameRoot::OnPreKeyPressed(const SDL_Keysym &key)
{
	return GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE;
}

mu_uint32 UIGameRoot::OnKeyPressed(const SDL_Keysym &key)
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
	else if (MU_INPUT->IsShiftPressing() == false &&
		MU_INPUT->IsCtrlPressing() == true)
	{
		switch (key.scancode)
		{
		case SDL_SCANCODE_Q:
		case SDL_SCANCODE_W:
		case SDL_SCANCODE_E:
		case SDL_SCANCODE_R:
			{
				if (g_pPickedItem.GetItem() != nullptr)
				{
					break;
				}

				if (IsVisible(GAMEUI::INTERFACE_INVENTORY) == true)
				{
					mu_int32 hotKey = -1;

					switch (key.scancode)
					{
					case SDL_SCANCODE_Q:
						{
							hotKey = UIItemHotkey::HOTKEY_Q;
						}
						break;

					case SDL_SCANCODE_W:
						{
							hotKey = UIItemHotkey::HOTKEY_W;
						}
						break;

					case SDL_SCANCODE_E:
						{
							hotKey = UIItemHotkey::HOTKEY_E;
						}
						break;

					case SDL_SCANCODE_R:
						{
							hotKey = UIItemHotkey::HOTKEY_R;
						}
						break;
					}

					if (hotKey != -1)
					{
						UIInventoryControl *control = g_pMyInventory->GetInventoryControl();
						mu_int32 pointedIndex = control->GetPointedSquareIndex();

						if (pointedIndex != -1)
						{
							ITEM *item = control->FindItem(pointedIndex);

							if (item != nullptr &&
								_HotkeyFrame->CanRegisterItemHotKey(item->Type) == true)
							{
								_HotkeyFrame->SetItemHotKey(hotKey, item->Type, item->Level);
							}
						}

						return GFX_MESSAGE_RETURN::GFX_MESSAGE_ACCEPT;
					}
				}
				else
				{
					mu_int32 Index = -1;

					switch (key.sym)
					{
					case SDL_SCANCODE_Q:
						{
							Index = _HotkeyFrame->GetHotkeyItemIndex(UIItemHotkey::HOTKEY_Q);
						}
						break;

					case SDL_SCANCODE_W:
						{
							Index = _HotkeyFrame->GetHotkeyItemIndex(UIItemHotkey::HOTKEY_W);
						}
						break;

					case SDL_SCANCODE_E:
						{
							Index = _HotkeyFrame->GetHotkeyItemIndex(UIItemHotkey::HOTKEY_E);
						}
						break;

					case SDL_SCANCODE_R:
						{
							Index = _HotkeyFrame->GetHotkeyItemIndex(UIItemHotkey::HOTKEY_R);
						}
						break;
					}

					if (Index != -1)
					{
						UIInventoryControl *control = g_pMyInventory->GetInventoryControl();
						ITEM *pItem = control->FindItem(Index);
						mu_int32 targetIndex = pItem->y * control->GetNumberOfColumn() + pItem->x;

						if ((pItem->Type >= ITEM_POTION + 78 && pItem->Type <= ITEM_POTION + 82))
						{
							std::list<eBuffState> secretPotionbufflist;
							secretPotionbufflist.push_back(eBuff_SecretPotion1);
							secretPotionbufflist.push_back(eBuff_SecretPotion2);
							secretPotionbufflist.push_back(eBuff_SecretPotion3);
							secretPotionbufflist.push_back(eBuff_SecretPotion4);
							secretPotionbufflist.push_back(eBuff_SecretPotion5);

							if (g_isCharacterBufflist((&Hero->Object), secretPotionbufflist) != eBuffNone)
							{
								g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_OKBOX);
								g_pOkBox->ClearMessages();
								g_pOkBox->AddMessage(FONT_NORMAL, GlobalText[2530], MU::MakeARGB(255, 30, 0, 255));
								g_pOkBox->Configure();
							}
							else
							{
								g_GameServerObject->SendUseItem(targetIndex);
							}
						}
						else
						{
							g_GameServerObject->SendUseItem(targetIndex);
						}
					}
				}
			}
			break;

		case SDL_SCANCODE_0:
		case SDL_SCANCODE_1:
		case SDL_SCANCODE_2:
		case SDL_SCANCODE_3:
		case SDL_SCANCODE_4:
		case SDL_SCANCODE_5:
		case SDL_SCANCODE_6:
		case SDL_SCANCODE_7:
		case SDL_SCANCODE_8:
		case SDL_SCANCODE_9:
			{
				if (g_pPickedItem.GetItem() != nullptr)
				{
					break;
				}

				UIBase *focusObject = GetFocused();
				if (IsVisible(GAMEUI::INTERFACE_SKILL_LIST) == true &&
					focusObject != nullptr &&
					focusObject->GetParent() == _SkillFrame)
				{
					mu_int32 hotkeyIndex = SDL_GetKeyFromScancode(key.scancode) - SDLK_0;

					if (focusObject->GetObjectID() >= GAMEUI::MESSAGE_SKILLFRAME_SKILLLIST_BEGIN &&
						focusObject->GetObjectID() <= GAMEUI::MESSAGE_SKILLFRAME_SKILLLIST_END)
					{
						mu_uint32 useboxIndex = focusObject->GetObjectID() - GAMEUI::MESSAGE_SKILLFRAME_SKILLLIST_BEGIN;

						_SkillFrame->SetHotkey(hotkeyIndex, CharacterAttribute->Skill[useboxIndex]);
						_SkillFrame->UpdateHotkeySkill();
					}
					else if (focusObject->GetObjectID() >= GAMEUI::MESSAGE_SKILLFRAME_COMMANDLIST_BEGIN &&
						focusObject->GetObjectID() <= GAMEUI::MESSAGE_SKILLFRAME_COMMANDLIST_END)
					{
						mu_uint32 useboxIndex = focusObject->GetObjectID() - GAMEUI::MESSAGE_SKILLFRAME_COMMANDLIST_BEGIN;

						_SkillFrame->SetHotkey(hotkeyIndex, CharacterAttribute->Skill[AT_PET_COMMAND_DEFAULT + useboxIndex]);
						_SkillFrame->UpdateHotkeySkill();
					}

					return GFX_MESSAGE_RETURN::GFX_MESSAGE_ACCEPT;
				}
				else
				{
					mu_int32 hotkeyIndex = SDL_GetKeyFromScancode(key.scancode) - SDLK_0;

					mu_int32 skillType = _SkillFrame->_HotkeyIndex[hotkeyIndex];

					if (skillType != -1)
					{
						if (skillType >= AT_PET_COMMAND_DEFAULT &&
							skillType < AT_PET_COMMAND_END)
						{
							Hero->CurrentSkill = skillType;
						}
						else
						{
							Hero->CurrentSkill = _SkillFrame->_HotkeyUseboxIndex[hotkeyIndex];
						}

						_SkillFrame->ShowList(false);
						_SkillFrame->UpdateSkills();
					}
				}
			}
			break;
		}
	}
	else if (MU_INPUT->IsShiftPressing() == false &&
		MU_INPUT->IsCtrlPressing() == false)
	{
		switch (key.scancode)
		{
		case SDL_SCANCODE_C:
			{
				Toggle(GAMEUI::INTERFACE_CHARACTER);
				PlayBackground(SOUND_CLICK01);
			}
			return GFX_MESSAGE_RETURN::GFX_MESSAGE_ACCEPT;

		case SDL_SCANCODE_T:
			{
				Toggle(GAMEUI::INTERFACE_MYQUEST);
				PlayBackground(SOUND_CLICK01);
			}
			return GFX_MESSAGE_RETURN::GFX_MESSAGE_ACCEPT;

		case SDL_SCANCODE_I:
		case SDL_SCANCODE_V:
			{
				Toggle(GAMEUI::INTERFACE_INVENTORY);
				PlayBackground(SOUND_CLICK01);
			}
			return GFX_MESSAGE_RETURN::GFX_MESSAGE_ACCEPT;

		case SDL_SCANCODE_P:
			{
				Toggle(GAMEUI::INTERFACE_PARTY);
				PlayBackground(SOUND_CLICK01);
			}
			return GFX_MESSAGE_RETURN::GFX_MESSAGE_ACCEPT;

		case SDL_SCANCODE_G:
			{
				Toggle(GAMEUI::INTERFACE_GUILDINFO);
				PlayBackground(SOUND_CLICK01);
			}
			return GFX_MESSAGE_RETURN::GFX_MESSAGE_ACCEPT;

		case SDL_SCANCODE_M:
			{
				Toggle(GAMEUI::INTERFACE_MOVEMAP);
				PlayBackground(SOUND_CLICK01);
			}
			return GFX_MESSAGE_RETURN::GFX_MESSAGE_ACCEPT;

		case SDL_SCANCODE_D:
			{
				Toggle(GAMEUI::INTERFACE_COMMAND);
				PlayBackground(SOUND_CLICK01);
			}
			return GFX_MESSAGE_RETURN::GFX_MESSAGE_ACCEPT;

		case SDL_SCANCODE_L:
			{
				if (this->IsVisible(GAMEUI::INTERFACE_NPCSHOP) == true)
				{
					if (MU_INPUT->IsShiftPressing() == true)
					{
						if (g_pNpcShopInventory->GetRepairShop() == true &&
							g_pPickedItem.GetItem() == nullptr)
						{
							g_GameServerObject->SendRequestRepairAll();
						}
					}
					else
					{
						if (g_pNpcShopInventory->GetRepairShop() == true &&
							g_pPickedItem.GetItem() == nullptr)
						{
							g_pNpcShopInventory->ToggleState();
						}
					}

					return GFX_MESSAGE_RETURN::GFX_MESSAGE_ACCEPT;
				}
			}
			break;

		case SDL_SCANCODE_Z:
			{
				Toggle(GAMEUI::INTERFACE_HELPERSETUP);
				PlayBackground(SOUND_CLICK01);
			}
			return GFX_MESSAGE_RETURN::GFX_MESSAGE_ACCEPT;

		case SDL_SCANCODE_Q:
		case SDL_SCANCODE_W:
		case SDL_SCANCODE_E:
		case SDL_SCANCODE_R:
			{
				mu_int32 Index = -1;

				switch (key.scancode)
				{
				case SDL_SCANCODE_Q:
					{
						Index = _HotkeyFrame->GetHotkeyItemIndex(UIItemHotkey::HOTKEY_Q);
					}
					break;

				case SDL_SCANCODE_W:
					{
						Index = _HotkeyFrame->GetHotkeyItemIndex(UIItemHotkey::HOTKEY_W);
					}
					break;

				case SDL_SCANCODE_E:
					{
						Index = _HotkeyFrame->GetHotkeyItemIndex(UIItemHotkey::HOTKEY_E);
					}
					break;

				case SDL_SCANCODE_R:
					{
						Index = _HotkeyFrame->GetHotkeyItemIndex(UIItemHotkey::HOTKEY_R);
					}
					break;
				}

				if (Index != -1)
				{
					UIInventoryControl *control = g_pMyInventory->GetInventoryControl();
					ITEM *pItem = control->FindItem(Index);
					mu_int32 targetIndex = pItem->y * control->GetNumberOfColumn() + pItem->x;

					if ((pItem->Type >= ITEM_POTION + 78 && pItem->Type <= ITEM_POTION + 82))
					{
						std::list<eBuffState> secretPotionbufflist;
						secretPotionbufflist.push_back(eBuff_SecretPotion1);
						secretPotionbufflist.push_back(eBuff_SecretPotion2);
						secretPotionbufflist.push_back(eBuff_SecretPotion3);
						secretPotionbufflist.push_back(eBuff_SecretPotion4);
						secretPotionbufflist.push_back(eBuff_SecretPotion5);

						if (g_isCharacterBufflist((&Hero->Object), secretPotionbufflist) != eBuffNone)
						{
							g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_OKBOX);
							g_pOkBox->ClearMessages();
							g_pOkBox->AddMessage(FONT_NORMAL, GlobalText[2530], MU::MakeARGB(255, 30, 0, 255));
							g_pOkBox->Configure();
						}
						else
						{
							g_GameServerObject->SendUseItem(targetIndex);
						}
					}
					else
					{
						g_GameServerObject->SendUseItem(targetIndex);
					}
				}
			}
			return GFX_MESSAGE_RETURN::GFX_MESSAGE_ACCEPT;

		case SDL_SCANCODE_0:
		case SDL_SCANCODE_1:
		case SDL_SCANCODE_2:
		case SDL_SCANCODE_3:
		case SDL_SCANCODE_4:
		case SDL_SCANCODE_5:
		case SDL_SCANCODE_6:
		case SDL_SCANCODE_7:
		case SDL_SCANCODE_8:
		case SDL_SCANCODE_9:
			{
				mu_int32 hotkeyIndex = SDL_GetKeyFromScancode(key.scancode) - SDLK_0;
				mu_int32 skillType = _SkillFrame->_HotkeyIndex[hotkeyIndex];

				if (skillType != -1)
				{
					if (skillType >= AT_PET_COMMAND_DEFAULT &&
						skillType < AT_PET_COMMAND_END)
					{
						Hero->CurrentSkill = skillType;
					}
					else
					{
						Hero->CurrentSkill = _SkillFrame->_HotkeyUseboxIndex[hotkeyIndex];
					}

					_SkillFrame->ShowList(false);
					_SkillFrame->UpdateSkills();
				}
			}
			return GFX_MESSAGE_RETURN::GFX_MESSAGE_ACCEPT;
		}
	}

	return GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE;
}

void UIGameRoot::OnGFxSceneMessage(GFX_MESSAGE &objMsg)
{
	switch (objMsg.Message)
	{
	case GFX_MESSAGE_TYPE::GFX_PRESSED:
	case GFX_MESSAGE_TYPE::GFX_TOGGLE:
		{
			if (objMsg.ObjectID >= GAMEUI::MESSAGE_NPCQUESTWINDOW_ANSWER_BEGIN &&
				objMsg.ObjectID <= GAMEUI::MESSAGE_NPCQUESTWINDOW_ANSWER_END)
			{
				PlayBackground(SOUND_INTERFACE01);
			}
			else if (objMsg.ObjectID == GAMEUI::MESSAGE_NPCQUESTWINDOW_COMPLETE_BUTTON)
			{
				PlayBackground(SOUND_INTERFACE01);
			}
			else
			{
				PlayBackground(SOUND_CLICK01);
			}
		}
		break;
	}

	if (objMsg.ObjectID >= GAMEUI::MESSAGE_PARTYLIST_SELECT_BEGIN &&
		objMsg.ObjectID <= GAMEUI::MESSAGE_PARTYLIST_SELECT_END)
	{
		switch (objMsg.Message)
		{
		case GFX_MESSAGE_TYPE::GFX_MOUSE_PRESSED:
			{
				if (MUMisc::IsFlag(objMsg.MessageData, MOUSE_FLAG_RIGHT))
				{
					mu_uint32 memberIndex = objMsg.ObjectID - GAMEUI::MESSAGE_PARTYLIST_SELECT_BEGIN;

					mu_int16 CharacterTarget = Party[memberIndex].Key;
					mu_float SkillDirection;

					mu_int32 CharacterIndex = FindCharacterIndex(CharacterTarget);

					if (CharacterIndex == MAX_CHARACTERS_CLIENT)
					{
						SkillDirection = Hero->TargetAngle;
					}
					else
					{
						OBJECT *o = &Hero->Object;
						OBJECT *to = &CharactersClient[CharacterIndex].Object;
						SkillDirection = CreateAngle(o->Position[0], o->Position[1], to->Position[0], to->Position[1]);
					}

					if (g_pNewUISystem->AllowHeroAction() == true)
					{
						if (ActionRequest != ACTION_REQUEST::AR_MAGIC ||
							ActionRequestMagic != Hero->CurrentSkill ||
							ActionRequestTarget != CharacterTarget ||
							(CharacterTarget == -1 &&
							(ActionRequestX != SelectXF ||
								ActionRequestY != SelectYF ||
								ActionRequestDirection != SkillDirection)))
						{
							CL2GS_REQUEST_MAGIC RequestMagic;

							RequestMagic.Key = CharacterTarget;
							RequestMagic.Magic = Hero->CurrentSkill;
							RequestMagic.X = SelectXF;
							RequestMagic.Y = SelectYF;
							RequestMagic.Direction = SkillDirection;

							g_GameServerObject->Send(&RequestMagic);

							ActionRequest = ACTION_REQUEST::AR_MAGIC;
							ActionRequestTarget = CharacterTarget;
							ActionRequestMagic = Hero->CurrentSkill;
							ActionRequestX = SelectXF;
							ActionRequestY = SelectYF;
							ActionRequestDirection = SkillDirection;
						}
					}
				}
			}
			break;
		}
	}
	else if (objMsg.ObjectID >= GAMEUI::MESSAGE_SKILLFRAME_SKILLLIST_BEGIN &&
		objMsg.ObjectID <= GAMEUI::MESSAGE_SKILLFRAME_SKILLLIST_END)
	{
		switch (objMsg.Message)
		{
		case GFX_MESSAGE_TYPE::GFX_PRESSED:
			{
				mu_int32 skillIndex = objMsg.ObjectID - GAMEUI::MESSAGE_SKILLFRAME_SKILLLIST_BEGIN;

				Hero->CurrentSkill = skillIndex;

				_SkillFrame->ShowList(false);
				_SkillFrame->UpdateSkills();
			}
			break;
		}
	}
	else if (objMsg.ObjectID >= GAMEUI::MESSAGE_SKILLFRAME_COMMANDLIST_BEGIN &&
		objMsg.ObjectID <= GAMEUI::MESSAGE_SKILLFRAME_COMMANDLIST_END)
	{
		switch (objMsg.Message)
		{
		case GFX_MESSAGE_TYPE::GFX_PRESSED:
			{
				mu_int32 skillIndex = objMsg.ObjectID - GAMEUI::MESSAGE_SKILLFRAME_COMMANDLIST_BEGIN;

				Hero->CurrentSkill = AT_PET_COMMAND_DEFAULT + skillIndex;

				_SkillFrame->ShowList(false);
				_SkillFrame->UpdateSkills();
			}
			break;
		}
	}
	else if (objMsg.ObjectID >= GAMEUI::MESSAGE_SKILLFRAME_HOTKEYLIST_BEGIN &&
		objMsg.ObjectID <= GAMEUI::MESSAGE_SKILLFRAME_HOTKEYLIST_END)
	{
		switch (objMsg.Message)
		{
		case GFX_MESSAGE_TYPE::GFX_PRESSED:
			{
				mu_int32 hotkeyIndex = objMsg.ObjectID - GAMEUI::MESSAGE_SKILLFRAME_HOTKEYLIST_BEGIN;
				mu_int32 skillIndex = _SkillFrame->GetHotkey(hotkeyIndex);

				if (skillIndex != -1 &&
					CharacterAttribute->Skill[skillIndex] > 0)
				{
					Hero->CurrentSkill = skillIndex;

					_SkillFrame->ShowList(false);
					_SkillFrame->UpdateSkills();
				}
			}
			break;
		}
	}
	else if (objMsg.ObjectID >= GAMEUI::MESSAGE_CHARACTERINFO_STRENGTH_BUTTON &&
		objMsg.ObjectID <= GAMEUI::MESSAGE_CHARACTERINFO_COMMAND_BUTTON)
	{
		if (objMsg.ObjectID != GAMEUI::MESSAGE_CHARACTERINFO_COMMAND_BUTTON ||
			GetBaseClass(Hero->Class) == GAME::CLASS_DARK_LORD)
		{
			switch (objMsg.Message)
			{
			case GFX_MESSAGE_TYPE::GFX_PRESSED:
				{
					CL2GS_REQUEST_INCREASESTAT RequestIncreaseStat;
					RequestIncreaseStat.StatType = objMsg.ObjectID - GAMEUI::MESSAGE_CHARACTERINFO_STRENGTH_BUTTON;
					g_GameServerObject->Send(&RequestIncreaseStat);
				}
				break;
			}
		}
	}
	else if (objMsg.ObjectID >= GAMEUI::MESSAGE_PARTYWINDOW_KICK_BEGIN &&
		objMsg.ObjectID <= GAMEUI::MESSAGE_PARTYWINDOW_KICK_END)
	{
		switch (objMsg.Message)
		{
		case GFX_MESSAGE_TYPE::GFX_PRESSED:
			{
				mu_int32 kickIndex = objMsg.ObjectID - GAMEUI::MESSAGE_PARTYWINDOW_KICK_BEGIN;
				PARTY_t &member = Party[kickIndex];

				if (g_CursedTemple.IsCursedTemple() == false)
				{
					PlayBackground(SOUND_CLICK01);
					g_GameServerObject->SendPartyKick(member.Number);
				}
			}
			break;
		}
	}
	else if (objMsg.ObjectID >= GAMEUI::MESSAGE_PARTYLIST_LEAVE_BEGIN &&
		objMsg.ObjectID <= GAMEUI::MESSAGE_PARTYLIST_LEAVE_END)
	{
		switch (objMsg.Message)
		{
		case GFX_MESSAGE_TYPE::GFX_PRESSED:
			{
				mu_int32 kickIndex = objMsg.ObjectID - GAMEUI::MESSAGE_PARTYLIST_LEAVE_BEGIN;
				PARTY_t &member = Party[kickIndex];

				if (g_CursedTemple.IsCursedTemple() == false)
				{
					PlayBackground(SOUND_CLICK01);
					g_GameServerObject->SendPartyKick(member.Number);
				}
			}
			break;
		}
	}
	else if (objMsg.ObjectID >= GAMEUI::MESSAGE_MOVEWINDOW_BEGIN &&
		objMsg.ObjectID <= GAMEUI::MESSAGE_MOVEWINDOW_END)
	{
		switch (objMsg.Message)
		{
		case GFX_MESSAGE_TYPE::GFX_PRESSED:
			{
				mu_int32 moveIndex = objMsg.ObjectID - GAMEUI::MESSAGE_MOVEWINDOW_BEGIN;
				MoveRequirementDataEx &data = g_MoveManager.MoveData[g_pMoveWindow->_BeginIndex + moveIndex];

				CL2GS_REQUEST_MOVECOMMAND RequestMoveCommand;
				RequestMoveCommand.MoveIndex = data.Index;

				g_GameServerObject->Send(&RequestMoveCommand);
				Hide(GAMEUI::INTERFACE_MOVEMAP);
			}
			break;
		}
	}
	else if (objMsg.ObjectID >= GAMEUI::MESSAGE_GUILDWINDOW_MEMBER_BUTTON_BEGIN &&
		objMsg.ObjectID <= GAMEUI::MESSAGE_GUILDWINDOW_MEMBER_BUTTON_END)
	{
		switch (objMsg.Message)
		{
		case GFX_MESSAGE_TYPE::GFX_TOGGLE:
			{
				mu_int32 memberIndex = objMsg.ObjectID - GAMEUI::MESSAGE_GUILDWINDOW_MEMBER_BUTTON_BEGIN;

				auto it = _GuildWindow->_Members.begin() + _GuildWindow->_MemberListIndex + memberIndex;

				_GuildWindow->_MemberSelected = (*it).CharacterIndex;
				_GuildWindow->ConfigureMemberList();
			}
			break;
		}
	}
	else if (objMsg.ObjectID >= GAMEUI::MESSAGE_GUILDWINDOW_ALLIANCE_BUTTON_BEGIN &&
		objMsg.ObjectID <= GAMEUI::MESSAGE_GUILDWINDOW_ALLIANCE_BUTTON_END)
	{
		switch (objMsg.Message)
		{
		case GFX_MESSAGE_TYPE::GFX_TOGGLE:
			{
				mu_int32 allianceIndex = objMsg.ObjectID - GAMEUI::MESSAGE_GUILDWINDOW_ALLIANCE_BUTTON_BEGIN;

				auto it = _GuildWindow->_Alliances.begin();

				for (mu_uint32 n = _GuildWindow->_AllianceListIndex; n--; ++it);
				for (mu_uint32 n = allianceIndex; n--; ++it);

				_GuildWindow->_AllianceSelected = it->second.GuildIndex;
				_GuildWindow->ConfigureAllianceList();
			}
			break;
		}
	}
	else if (objMsg.ObjectID >= GAMEUI::MESSAGE_GUILDCREATE_MARKS_BUTTON_BEGIN &&
		objMsg.ObjectID <= GAMEUI::MESSAGE_GUILDCREATE_MARKS_BUTTON_END)
	{
		switch (objMsg.Message)
		{
		case GFX_MESSAGE_TYPE::GFX_PRESSED:
			{
				mu_uint32 index = objMsg.ObjectID - GAMEUI::MESSAGE_GUILDCREATE_MARKS_BUTTON_BEGIN;
				_GuildCreate->SetBufferColor(index);
			}
			break;
		}
	}
	else if (objMsg.ObjectID >= GAMEUI::MESSAGE_GUILDCREATE_COLORS_BUTTON_BEGIN &&
		objMsg.ObjectID <= GAMEUI::MESSAGE_GUILDCREATE_COLORS_BUTTON_END)
	{
		switch (objMsg.Message)
		{
		case GFX_MESSAGE_TYPE::GFX_PRESSED:
			{
				mu_uint32 index = objMsg.ObjectID - GAMEUI::MESSAGE_GUILDCREATE_COLORS_BUTTON_BEGIN;
				_GuildCreate->SetSelectedColor(index);
			}
			break;
		}
	}
	else if (objMsg.ObjectID >= GAMEUI::MESSAGE_BLOODCASTLEENTER_ENTER_BEGIN &&
		objMsg.ObjectID <= GAMEUI::MESSAGE_BLOODCASTLEENTER_ENTER_END)
	{
		switch (objMsg.Message)
		{
		case GFX_MESSAGE_TYPE::GFX_PRESSED:
			{
				mu_uint32 index = objMsg.ObjectID - GAMEUI::MESSAGE_BLOODCASTLEENTER_ENTER_BEGIN;

				mu_int32 ItemIndex = g_pMyInventory->FindItemIndex(ITEM_HELPER + 18, index + 1);

				if (ItemIndex == -1)
				{
					ItemIndex = g_pMyInventory->FindItemIndex(ITEM_HELPER + 47, -1);
				}

				if (ItemIndex != -1)
				{
					g_GameServerObject->SendRequestBloodCastleEnter(index + 1, GAME::MAX_EQUIPMENT + ItemIndex);
				}
				else
				{
					g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_OKBOX);
					g_pOkBox->ClearMessages();
					g_pOkBox->AddMessage(FONT_NORMAL, GlobalText[854]);
					g_pOkBox->Configure();
				}

				Hide(GAMEUI::INTERFACE_BLOODCASTLE);
			}
			break;
		}
	}
	else if (objMsg.ObjectID >= GAMEUI::MESSAGE_ITEMHOTKEY_BEGIN &&
		objMsg.ObjectID < GAMEUI::MESSAGE_ITEMHOTKEY_END)
	{
		switch (objMsg.Message)
		{
		case GFX_MESSAGE_TYPE::GFX_PRESSED:
			{
				mu_int32 Index = _HotkeyFrame->GetHotkeyItemIndex(objMsg.ObjectID - GAMEUI::MESSAGE_ITEMHOTKEY_BEGIN);

				if (Index != -1)
				{
					ITEM *pItem = g_pMyInventory->FindItem(Index);

					if ((pItem->Type >= ITEM_POTION + 78 && pItem->Type <= ITEM_POTION + 82))
					{
						std::list<eBuffState> secretPotionbufflist;
						secretPotionbufflist.push_back(eBuff_SecretPotion1);
						secretPotionbufflist.push_back(eBuff_SecretPotion2);
						secretPotionbufflist.push_back(eBuff_SecretPotion3);
						secretPotionbufflist.push_back(eBuff_SecretPotion4);
						secretPotionbufflist.push_back(eBuff_SecretPotion5);

						if (g_isCharacterBufflist((&Hero->Object), secretPotionbufflist) != eBuffNone)
						{
							g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_OKBOX);
							g_pOkBox->ClearMessages();
							g_pOkBox->AddMessage(FONT_NORMAL, GlobalText[2530], MU::MakeARGB(255, 30, 0, 255));
							g_pOkBox->Configure();
						}
						else
						{
							g_GameServerObject->SendUseItem(Index);
						}
					}
					else
					{
						g_GameServerObject->SendUseItem(Index);
					}
				}
			}
			break;
		}
	}
	else if (objMsg.ObjectID >= GAMEUI::MESSAGE_NPCQUESTWINDOW_ANSWER_BEGIN &&
		objMsg.ObjectID <= GAMEUI::MESSAGE_NPCQUESTWINDOW_ANSWER_END)
	{
		switch (objMsg.Message)
		{
		case GFX_MESSAGE_TYPE::GFX_PRESSED:
			{
				mu_uint32 index = objMsg.ObjectID - GAMEUI::MESSAGE_NPCQUESTWINDOW_ANSWER_BEGIN;

				if (_NpcQuestWindow->_AnswersButton[index]->IsGfxFlag(GFX_FLAG::GFX_HIDDEN) == false)
				{
					const MUDialogData *dialog = _NpcQuestWindow->_CurrentDialog;
					if (dialog != nullptr)
					{
						if (index < dialog->Answers.size())
						{
							const MUDialogAnswerData &answer = dialog->Answers[index];

							mu_boolean errorMessage = false;
							switch (answer.ReturnForAnswer)
							{
							case 1:
								{
									const MUQuestData *questData = g_QuestManager.GetQuest(g_QuestManager.GetCurrentQuestIndex());

									if (questData == nullptr)
									{
										errorMessage = true;
										break;
									}

									if (g_QuestManager.CheckRequestCondition(questData, true) == false)
									{
										errorMessage = true;
										break;
									}

									CL2GS_REQUEST_QUESTANSWER RequestQuestAnswer;

									RequestQuestAnswer.QuestIndex = g_QuestManager.GetCurrentQuestIndex();
									RequestQuestAnswer.State = 1;

									g_GameServerObject->Send(&RequestQuestAnswer);
								}
								break;

							case 2:
								{
									Hide(GAMEUI::INTERFACE_NPCQUEST);
								}
								break;

							case 3:
								{
									CL2GS_REQUEST_QUESTANSWER RequestQuestAnswer;

									RequestQuestAnswer.QuestIndex = g_QuestManager.GetCurrentQuestIndex();
									RequestQuestAnswer.State = 1;

									g_GameServerObject->Send(&RequestQuestAnswer);
								}
								break;
							}

							if (answer.LinkForAnswer > 0 &&
								errorMessage == false)
							{
								_NpcQuestWindow->ConfigureDialog(answer.LinkForAnswer);
							}
						}
						else if(dialog->Answers.size() == 0)
						{
							Hide(GAMEUI::INTERFACE_NPCQUEST);
						}
					}
				}
			}
			break;
		}
	}
	else if ((objMsg.ObjectID >= GAMEUI::MESSAGE_COMMANDWINDOW_BEGIN &&
		objMsg.ObjectID <= GAMEUI::MESSAGE_COMMANDWINDOW_END) ||
		objMsg.ObjectID == GAMEUI::MESSAGE_COMMANDWINDOW_PLAYER_BUTTON)
	{
		if (objMsg.ObjectID == GAMEUI::MESSAGE_COMMANDWINDOW_PLAYER_BUTTON)
		{
			switch (objMsg.Message)
			{
			case GFX_MESSAGE_TYPE::GFX_MOUSE_PRESSED:
				{
					if (MUMisc::IsFlag(objMsg.MessageData, MOUSE_FLAG_RIGHT))
					{
						switch (g_pCommandWindow->GetCommandSelected())
						{
						case UICommandWindow::UICMD_TRADE:
							{
								g_pCommandWindow->CommandTrade();
							}
							break;

						case UICommandWindow::UICMD_BUY:
							{
								g_pCommandWindow->CommandBuy();
							}
							break;

						case UICommandWindow::UICMD_PARTY:
							{
								g_pCommandWindow->CommandParty();
							}
							break;

						case UICommandWindow::UICMD_WHISPER:
							{
								g_pCommandWindow->CommandWhisper();
							}
							break;

						case UICommandWindow::UICMD_GUILD:
							{
								g_pCommandWindow->CommandGuild();
							}
							break;

						case UICommandWindow::UICMD_ALLIANCE:
							{
								g_pCommandWindow->CommandAlliance();
							}
							break;

						case UICommandWindow::UICMD_HOSTILITY:
							{
								g_pCommandWindow->CommandHostility();
							}
							break;

						case UICommandWindow::UICMD_SUSPENDHOSTILITY:
							{
								g_pCommandWindow->CommandSuspendHostility();
							}
							break;

						case UICommandWindow::UICMD_ADDFRIEND:
							{
								g_pCommandWindow->CommandAddFriend();
							}
							break;

						case UICommandWindow::UICMD_FOLLOW:
							{
								g_pCommandWindow->CommandFollow();
							}
							break;

						case UICommandWindow::UICMD_DUEL:
							{
								g_pCommandWindow->CommandDuel();
							}
							break;
						}
						g_pCommandWindow->SetCommandSelected(-1);
					}
				}
				break;
			}
		}
		else
		{
			switch (objMsg.Message)
			{
			case GFX_MESSAGE_TYPE::GFX_TOGGLE:
				{
					mu_uint32 index = objMsg.ObjectID - GAMEUI::MESSAGE_COMMANDWINDOW_BEGIN;

					switch (index)
					{
					case UICommandWindow::UICMD_BUY:
					case UICommandWindow::UICMD_ALLIANCE:
					case UICommandWindow::UICMD_HOSTILITY:
					case UICommandWindow::UICMD_SUSPENDHOSTILITY:
					case UICommandWindow::UICMD_DUEL:
						{
							g_pCommandWindow->_Commands[index]->ChangeState(UIToggleButton::UI_NORMAL);
							g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_OKBOX);
							g_pOkBox->ClearMessages();
							g_pOkBox->AddMessage(FONT_NORMAL, _T("This feature is under development"));
							g_pOkBox->Configure();
						}
						break;

					case UICommandWindow::UICMD_ADDFRIEND:
						{
							g_pCommandWindow->_Commands[index]->ChangeState(UIToggleButton::UI_NORMAL);
							g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_OKBOX);
							g_pOkBox->ClearMessages();
							g_pOkBox->AddMessage(FONT_NORMAL, _T("A new alternative better than original Friend-List coming soon!"));
							g_pOkBox->Configure();
						}
						break;

					default:
						{
							if (g_pCommandWindow->GetCommandSelected() == -1)
							{
								g_pCommandWindow->SetCommandSelected(index);
							}
							else if (index == g_pCommandWindow->GetCommandSelected())
							{
								g_pCommandWindow->SetCommandSelected(-1);
							}
							else
							{
								g_pCommandWindow->SetCommandSelected(index);
							}
						}
						break;
					}
				}
				break;
			}
		}
	}
	else if (objMsg.ObjectID >= GAMEUI::MESSAGE_HELPERSETUP_TAB_BEGIN &&
		objMsg.ObjectID <= GAMEUI::MESSAGE_HELPERSETUP_TAB_END)
	{
		switch (objMsg.Message)
		{
		case GFX_MESSAGE_TYPE::GFX_TOGGLE:
			{
				mu_uint32 tabIndex = objMsg.ObjectID - GAMEUI::MESSAGE_HELPERSETUP_TAB_BEGIN;
				g_pHelperSetup->SetCurrentTab(tabIndex);
			}
			break;
		}
	}
	else if (objMsg.ObjectID >= GAMEUI::MESSAGE_HELPERSETUP_OBITEMS_BEGIN &&
		objMsg.ObjectID <= GAMEUI::MESSAGE_HELPERSETUP_OBITEMS_END)
	{
		mu_int32 resIndex = objMsg.ObjectID - GAMEUI::MESSAGE_HELPERSETUP_OBITEMS_BEGIN;

		g_pHelperSetup->_OBItemSelected = g_pHelperSetup->_OBItemListIndex + resIndex;
		g_pHelperSetup->ConfigureObtainList();
	}
	else
	{
		switch (objMsg.ObjectID)
		{
			// Skill Frame
		case GAMEUI::MESSAGE_SKILLFRAME_CURRENTSKILL:
			{
				switch (objMsg.Message)
				{
				case GFX_MESSAGE_TYPE::GFX_PRESSED:
					{
						_SkillFrame->ShowList(!_SkillFrame->IsShowList());
						_SkillFrame->UpdateSkills();
					}
					break;
				}
			}
			break;

			// Character Window
		case GAMEUI::MESSAGE_CHARACTERINFO_BUTTON:
			{
				switch (objMsg.Message)
				{
				case GFX_MESSAGE_TYPE::GFX_TOGGLE:
					{
						Toggle(GAMEUI::INTERFACE_CHARACTER);
					}
					break;
				}
			}
			break;

		case GAMEUI::MESSAGE_CHARACTERINFO_CLOSE_BUTTON:
			{
				switch (objMsg.Message)
				{
				case GFX_MESSAGE_TYPE::GFX_PRESSED:
					{
						Hide(GAMEUI::INTERFACE_CHARACTER);
					}
					break;
				}
			}
			break;

			// Quest Window
		case GAMEUI::MESSAGE_CHARACTERINFO_QUEST_BUTTON:
			{
				switch (objMsg.Message)
				{
				case GFX_MESSAGE_TYPE::GFX_PRESSED:
					{
						Toggle(GAMEUI::INTERFACE_MYQUEST);
					}
					break;
				}
			}
			break;

		case GAMEUI::MESSAGE_QUESTWINDOW_CLOSE_BUTTON:
			{
				switch (objMsg.Message)
				{
				case GFX_MESSAGE_TYPE::GFX_PRESSED:
					{
						Hide(GAMEUI::INTERFACE_MYQUEST);
					}
					break;
				}
			}
			break;

		case GAMEUI::MESSAGE_QUESTWINDOW_TAB_BEGIN + 0:
		case GAMEUI::MESSAGE_QUESTWINDOW_TAB_BEGIN + 1:
		case GAMEUI::MESSAGE_QUESTWINDOW_TAB_BEGIN + 2:
			{
				switch (objMsg.Message)
				{
				case GFX_MESSAGE_TYPE::GFX_TOGGLE:
					{
						mu_uint32 tabIndex = objMsg.ObjectID - GAMEUI::MESSAGE_QUESTWINDOW_TAB_BEGIN;

						_MyQuestWindow->SetCurrentTab(tabIndex);
					}
					break;
				}
			}
			break;

			// Npc Quest Window
		case GAMEUI::MESSAGE_NPCQUESTWINDOW_CLOSE_BUTTON:
			{
				switch (objMsg.Message)
				{
				case GFX_MESSAGE_TYPE::GFX_PRESSED:
					{
						Hide(GAMEUI::INTERFACE_NPCQUEST);
					}
					break;
				}
			}
			break;

		case GAMEUI::MESSAGE_NPCQUESTWINDOW_COMPLETE_BUTTON:
			{
				switch (objMsg.Message)
				{
				case GFX_MESSAGE_TYPE::GFX_PRESSED:
					{
						CL2GS_REQUEST_QUESTANSWER RequestQuestAnswer;

						RequestQuestAnswer.QuestIndex = g_QuestManager.GetCurrentQuestIndex();
						RequestQuestAnswer.State = 1;

						g_GameServerObject->Send(&RequestQuestAnswer);
					}
					break;
				}
			}
			break;

			// Options
		case GAMEUI::MESSAGE_WINDOW_BUTTON:
			{
				switch (objMsg.Message)
				{
				case GFX_MESSAGE_TYPE::GFX_TOGGLE:
					{
						g_pOptionRoot->Show(OPTIONUI::INTERFACE_GAMEMENU);
					}
					break;
				}
			}
			break;

			// My Inventory
		case GAMEUI::MESSAGE_MYINVENTORY_BUTTON:
			{
				switch (objMsg.Message)
				{
				case GFX_MESSAGE_TYPE::GFX_TOGGLE:
					{
						Toggle(GAMEUI::INTERFACE_INVENTORY);
					}
					break;
				}
			}
			break;

		case GAMEUI::MESSAGE_MYINVENTORY_CLOSE_BUTTON:
			{
				switch (objMsg.Message)
				{
				case GFX_MESSAGE_TYPE::GFX_PRESSED:
					{
						Hide(GAMEUI::INTERFACE_INVENTORY);
					}
					break;
				}
			}
			break;

		case GAMEUI::MESSAGE_MYINVENTORY_REPAIR_BUTTON:
			{
				switch (objMsg.Message)
				{
				case GFX_MESSAGE_TYPE::GFX_PRESSED:
					{
						if (this->IsVisible(GAMEUI::INTERFACE_NPCSHOP) == true)
						{
							if (g_pNpcShopInventory->GetRepairShop() == true &&
								g_pPickedItem.GetItem() == nullptr)
							{
								g_pNpcShopInventory->ToggleState();
							}
						}
						else if(CharacterAttribute->Level >= REPAIR_MINLEVEL)
						{
							g_pMyInventory->SetRepairMode(!g_pMyInventory->GetRepairMode());
						}
					}
					break;
				}
			}
			break;

			// Party Window
		case GAMEUI::MESSAGE_PARTYWINDOW_CLOSE_BUTTON:
			{
				switch (objMsg.Message)
				{
				case GFX_MESSAGE_TYPE::GFX_PRESSED:
					{
						Hide(GAMEUI::INTERFACE_PARTY);
					}
					break;
				}
			}
			break;

			// Guild Window
		case GAMEUI::MESSAGE_GUILDWINDOW_CLOSE_BUTTON:
			{
				switch (objMsg.Message)
				{
				case GFX_MESSAGE_TYPE::GFX_PRESSED:
					{
						Hide(GAMEUI::INTERFACE_GUILDINFO);
					}
					break;
				}
			}
			break;

		case GAMEUI::MESSAGE_GUILDWINDOW_TAB_BEGIN + 0:
		case GAMEUI::MESSAGE_GUILDWINDOW_TAB_BEGIN + 1:
		case GAMEUI::MESSAGE_GUILDWINDOW_TAB_BEGIN + 2:
			{
				switch (objMsg.Message)
				{
				case GFX_MESSAGE_TYPE::GFX_TOGGLE:
					{
						mu_uint32 tabIndex = objMsg.ObjectID - GAMEUI::MESSAGE_GUILDWINDOW_TAB_BEGIN;

						_GuildWindow->SetCurrentTab(tabIndex);
					}
					break;
				}
			}
			break;

		case GAMEUI::MESSAGE_GUILDWINDOW_LEAVEGUILD_BUTTON:
			{
				switch (objMsg.Message)
				{
				case GFX_MESSAGE_TYPE::GFX_PRESSED:
					{
						g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_GUILDLEAVE);
						g_pGuildLeaveBox->Prepare();
					}
					break;
				}
			}
			break;

		case GAMEUI::MESSAGE_GUILDWINDOW_KICKPLAYER_BUTTON:
			{
				switch (objMsg.Message)
				{
				case GFX_MESSAGE_TYPE::GFX_PRESSED:
					{
						if (Hero->GuildStatus == GAME::G_MASTER)
						{
							GUILD_LIST_t *selectedMember = g_pGuildWindow->GetSelectedMember();

							if (selectedMember != nullptr &&
								selectedMember->GuildStatus != GAME::G_MASTER)
							{
								g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_GUILDLEAVE);
								g_pGuildLeaveBox->Prepare(selectedMember);
							}
						}
					}
					break;
				}
			}
			break;

		case GAMEUI::MESSAGE_GUILDWINDOW_GIVEPOSITION_BUTTON:
			{
				switch (objMsg.Message)
				{
				case GFX_MESSAGE_TYPE::GFX_PRESSED:
					{
						if (Hero->GuildStatus == GAME::G_MASTER)
						{
							GUILD_LIST_t *selectedMember = g_pGuildWindow->GetSelectedMember();

							if (selectedMember != nullptr &&
								selectedMember->GuildStatus != GAME::G_MASTER)
							{
								g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_GUILDGIVESTATUS);
								g_pGuildGiveStatus->Prepare(selectedMember);
							}
						}
					}
					break;
				}
			}
			break;

		case GAMEUI::MESSAGE_GUILDWINDOW_FREEPOSITION_BUTTON:
			{
				switch (objMsg.Message)
				{
				case GFX_MESSAGE_TYPE::GFX_PRESSED:
					{
						if (Hero->GuildStatus == GAME::G_MASTER)
						{
							GUILD_LIST_t *selectedMember = g_pGuildWindow->GetSelectedMember();

							if (selectedMember != nullptr &&
								selectedMember->GuildStatus != GAME::G_MASTER &&
								selectedMember->GuildStatus != GAME::G_PERSON)
							{
								g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_GUILDCANCELSTATUS);
								g_pGuildCancelStatus->Prepare(selectedMember);
							}
						}
					}
					break;
				}
			}
			break;

			// Guild Create Window
		case GAMEUI::MESSAGE_GUILDCREATE_CLOSE_BUTTON:
			{
				switch (objMsg.Message)
				{
				case GFX_MESSAGE_TYPE::GFX_PRESSED:
					{
						Hide(GAMEUI::INTERFACE_NPCGUILDMASTER);
					}
					break;
				}
			}
			break;

		case GAMEUI::MESSAGE_GUILDCREATE_BACK_BUTTON:
			{
				switch (objMsg.Message)
				{
				case GFX_MESSAGE_TYPE::GFX_PRESSED:
					{
						_GuildCreate->BackPage();
					}
					break;
				}
			}
			break;

		case GAMEUI::MESSAGE_GUILDCREATE_NEXT_BUTTON:
			{
				switch (objMsg.Message)
				{
				case GFX_MESSAGE_TYPE::GFX_PRESSED:
					{
						_GuildCreate->NextPage();
					}
					break;
				}
			}
			break;

			// Chat Window
		case GAMEUI::MESSAGE_CHATWINDOW_RESIZE_SLIDE:
			{
				switch (objMsg.Message)
				{
				case GFX_MESSAGE_TYPE::GFX_SLIDE_SECTION:
					{
						_ChatWindow->ResizeSection();
					}
					break;
				}
			}
			break;

		case GAMEUI::MESSAGE_CHATWINDOW_SCROLLBAR:
			{
				switch (objMsg.Message)
				{
				case GFX_MESSAGE_TYPE::GFX_SLIDE:
					{
						_ChatWindow->OnScrolling();
					}
					break;
				}
			}
			break;

			// Chat Input
		case GAMEUI::MESSAGE_CHATINPUT_CHATTYPE_BUTTON:
			{
				switch (objMsg.Message)
				{
				case GFX_MESSAGE_TYPE::GFX_TOGGLE:
					{
						ToggleChatInputType(INPUT_CHAT_MESSAGE);
					}
					break;
				}
			}
			break;

		case GAMEUI::MESSAGE_CHATINPUT_PARTYTYPE_BUTTON:
			{
				switch (objMsg.Message)
				{
				case GFX_MESSAGE_TYPE::GFX_TOGGLE:
					{
						ToggleChatInputType(INPUT_PARTY_MESSAGE);
					}
					break;
				}
			}
			break;

		case GAMEUI::MESSAGE_CHATINPUT_GUILDTYPE_BUTTON:
			{
				switch (objMsg.Message)
				{
				case GFX_MESSAGE_TYPE::GFX_TOGGLE:
					{
						ToggleChatInputType(INPUT_GUILD_MESSAGE);
					}
					break;
				}
			}
			break;

		case GAMEUI::MESSAGE_CHATINPUT_SYSTEM_BUTTON:
			{
				switch (objMsg.Message)
				{
				case GFX_MESSAGE_TYPE::GFX_TOGGLE:
					{
						mu_uint32 systemState = _ChatInput->_ChatSystemButton->GetState();
						mu_boolean useSystem = systemState == UIToggleButton::UI_PRESSED || systemState == UIToggleButton::UI_PRESSED_OVER ? true : false;
						ToggleChatMessageType(useSystem ? TYPE_SYSTEM_MESSAGE : TYPE_ALL_MESSAGE);
					}
					break;
				}
			}
			break;

		case GAMEUI::MESSAGE_CHATINPUT_LOG_BUTTON:
			{
				switch (objMsg.Message)
				{
				case GFX_MESSAGE_TYPE::GFX_TOGGLE:
					{
						ToggleChatLog();
					}
					break;
				}
			}
			break;

		case GAMEUI::MESSAGE_CHATINPUT_FRAME_BUTTON:
			{
				switch (objMsg.Message)
				{
				case GFX_MESSAGE_TYPE::GFX_TOGGLE:
					{
						ToggleChatFrame(!_ChatInput->_ChatFrameButton->IsPressed());
					}
					break;
				}
			}
			break;

		case GAMEUI::MESSAGE_CHATINPUT_SIZE_BUTTON:
			{
				switch (objMsg.Message)
				{
				case GFX_MESSAGE_TYPE::GFX_PRESSED:
					{
						UpdateChatWindowSize();
					}
					break;
				}
			}
			break;

		case GAMEUI::MESSAGE_CHATINPUT_TRANSPARENCY_BUTTON:
			{
				switch (objMsg.Message)
				{
				case GFX_MESSAGE_TYPE::GFX_PRESSED:
					{
						UpdateChatWindowAlpha();
					}
					break;
				}
			}
			break;

			// Storage Window
		case GAMEUI::MESSAGE_STORAGE_DEPOSIT_BUTTON:
			{
				switch (objMsg.Message)
				{
				case GFX_MESSAGE_TYPE::GFX_PRESSED:
					{
						g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_STORAGEDEPOSITBOX);
					}
					break;
				}
			}
			break;

		case GAMEUI::MESSAGE_STORAGE_WITHDRAW_BUTTON:
			{
				switch (objMsg.Message)
				{
				case GFX_MESSAGE_TYPE::GFX_PRESSED:
					{
						if (g_pStorageInventory->_CurrentAction == UIStorageInventory::WHAUTO_NONE)
						{
							g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_STORAGEWITHDRAWBOX);
						}
					}
					break;
				}
			}
			break;

		case GAMEUI::MESSAGE_STORAGE_LOCK_BUTTON:
			{
				switch (objMsg.Message)
				{
				case GFX_MESSAGE_TYPE::GFX_PRESSED:
					{
						if (g_pStorageInventory->_CurrentAction == UIStorageInventory::WHAUTO_NONE)
						{
							g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_STORAGELOCKBOX);
							if (g_pStorageInventory->IsStorageLocked() == true)
							{
								g_pStorageLockBox->SetRequestState(CL2GS_REQUEST_STORAGELOCK::REQUEST_UNLOCK);
							}
							else
							{
								g_pStorageLockBox->SetRequestState(CL2GS_REQUEST_STORAGELOCK::REQUEST_LOCK);
							}
						}
					}
					break;
				}
			}
			break;

			// NPC Shop Window
		case GAMEUI::MESSAGE_NPCSHOP_REPAIRALL_BUTTON:
			{
				switch (objMsg.Message)
				{
				case GFX_MESSAGE_TYPE::GFX_PRESSED:
					{
						if (g_pNpcShopInventory->GetRepairShop() == true &&
							g_pPickedItem.GetItem() == nullptr)
						{
							g_GameServerObject->SendRequestRepairAll();
						}
					}
					break;
				}
			}
			break;

		case GAMEUI::MESSAGE_NPCSHOP_REPAIR_BUTTON:
			{
				switch (objMsg.Message)
				{
				case GFX_MESSAGE_TYPE::GFX_PRESSED:
					{
						if (g_pNpcShopInventory->GetRepairShop() == true &&
							g_pPickedItem.GetItem() == nullptr)
						{
							g_pNpcShopInventory->ToggleState();
						}
					}
					break;
				}
			}
			break;

			// Trade Window
		case GAMEUI::MESSAGE_TRADEWINDOW_MONEY_BUTTON:
			{
				switch (objMsg.Message)
				{
				case GFX_MESSAGE_TYPE::GFX_PRESSED:
					{
						g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_TRADEMONEY);
					}
					break;
				}
			}
			break;

		case GAMEUI::MESSAGE_TRADEWINDOW_OK_BUTTON:
			{
				switch (objMsg.Message)
				{
				case GFX_MESSAGE_TYPE::GFX_TOGGLE:
					{
						if (_TradeWindow->_OkButton->GetState() >= UIToggleButton::UI_PRESSED)
						{
							if (_TradeWindow->IsAlertTrade() == true)
							{
								_TradeWindow->_OkButton->ChangeState(UIToggleButton::UI_NORMAL);
								g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_TRADEACCEPT);
							}
							else
							{
								_TradeWindow->SetAlertTrade(true);
								g_GameServerObject->SendTradeOk(true);
							}
						}
						else
						{
							g_GameServerObject->SendTradeOk(false);
						}
					}
					break;
				}
			}
			break;

			// Mix Window
		case GAMEUI::MESSAGE_MIXINVENTORY_MIX_BUTTON:
			{
				switch (objMsg.Message)
				{
				case GFX_MESSAGE_TYPE::GFX_PRESSED:
					{
						mu_int64 Gold = CharacterMachine->Gold;
						mu_int64 nMixZen = g_MixManager.GetReqiredZen();

						nMixZen = MixRequireZen(nMixZen);

						if (nMixZen > Gold)
						{
							g_pChatListBox->AddText(_T(""), GlobalText[596], TYPE_ERROR_MESSAGE);
							break;
						}

						if (!g_MixManager.IsReadyToMix())
						{
							mu_text szText[100];
							mu_sprintf_s(szText, mu_countof(szText), GlobalText[580], GlobalText[591]);
							g_pChatListBox->AddText(_T(""), szText, TYPE_ERROR_MESSAGE);
							break;
						}

						mu_int32 iLevel = CharacterAttribute->Level;
						if (iLevel < g_MixManager.GetCurRecipe()->RequiredLevel)
						{
							mu_text szText[100];
							mu_text szText2[100];
							g_MixManager.GetCurRecipeName(szText2, mu_countof(szText2), 1);
							mu_sprintf_s(szText, mu_countof(szText), GlobalText[2347], g_MixManager.GetCurRecipe()->RequiredLevel, szText2);
							g_pChatListBox->AddText(_T(""), szText, TYPE_ERROR_MESSAGE);
							break;
						}

						if (g_MixManager.GetCurRecipe()->Width != -1 &&
							g_pMyInventory->FindEmptySlot(g_MixManager.GetCurRecipe()->Width,
								g_MixManager.GetCurRecipe()->Height) == -1)
						{
							g_pChatListBox->AddText(_T(""), GlobalText[581], TYPE_ERROR_MESSAGE);
							break;
						}

#if 0 // FIX
						if (g_MixManager.GetMixType() == MIXTYPE_ATTACH_SOCKET)
						{
							mu_int32 iSelectedLine = m_SocketListBox.GetLineNum() - m_SocketListBox.SLGetSelectLineNum();

							for (mu_int32 i = 0; i < m_SocketListBox.GetLineNum(); ++i)
							{
								mu_uint8 bySocketSeedID = g_MixManager.GetFirstItemSocketSeedID(i);
								if (bySocketSeedID != SOCKET_EMPTY)
								{
									mu_uint8 bySeedSphereID = g_MixManager.GetSeedSphereID(0);
									if (bySocketSeedID == bySeedSphereID)
									{
										g_pChatListBox->AddText(_T(""), GlobalText[2683], TYPE_ERROR_MESSAGE);
										break;
									}
								}
							}

							if (m_SocketListBox.SLGetSelectLineNum() == 0)
							{
								g_pChatListBox->AddText(_T(""), GlobalText[2676], TYPE_ERROR_MESSAGE);
								break;
							}
							else if (iSelectedLine > g_MixManager.GetFirstItemSocketCount()
								|| g_MixManager.GetFirstItemSocketSeedID(iSelectedLine) != SOCKET_EMPTY)
							{
								g_pChatListBox->AddText(_T(""), GlobalText[2677], TYPE_ERROR_MESSAGE);
								break;
							}

							g_MixManager.SetMixSubType(iSelectedLine);
						}
						else if (g_MixManager.GetMixType() == MIXTYPE_DETACH_SOCKET)
						{
							mu_int32 iSelectedLine = m_SocketListBox.GetLineNum() - m_SocketListBox.SLGetSelectLineNum();
							if (m_SocketListBox.SLGetSelectLineNum() == 0)
							{
								g_pChatListBox->AddText(_T(""), GlobalText[2678], TYPE_ERROR_MESSAGE);
								break;
							}
							else if (iSelectedLine > g_MixManager.GetFirstItemSocketCount()
								|| g_MixManager.GetFirstItemSocketSeedID(iSelectedLine) == SOCKET_EMPTY)
							{
								g_pChatListBox->AddText(_T(""), GlobalText[2679], TYPE_ERROR_MESSAGE);
								break;
							}
							g_MixManager.SetMixSubType(iSelectedLine);
						}
#endif

						if (g_pPickedItem.GetItem() == nullptr)
						{
							g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_MIXCHECK);
						}
					}
					break;
				}
			}
			break;

			// Move Window
		case GAMEUI::MESSAGE_MOVEWINDOW_CLOSE:
			{
				switch (objMsg.Message)
				{
				case GFX_MESSAGE_TYPE::GFX_PRESSED:
					{
						Hide(GAMEUI::INTERFACE_MOVEMAP);
					}
					break;
				}
			}
			break;

		case GAMEUI::MESSAGE_MOVEWINDOW_SCROLLBAR:
			{
				switch (objMsg.Message)
				{
				case GFX_MESSAGE_TYPE::GFX_SLIDE:
					{
						_MoveWindow->OnScrolling();
					}
					break;
				}
			}
			break;

			// Command Window
		case GAMEUI::MESSAGE_COMMANDWINDOW_CLOSE_BUTTON:
			{
				switch (objMsg.Message)
				{
				case GFX_MESSAGE_TYPE::GFX_PRESSED:
					{
						Hide(GAMEUI::INTERFACE_COMMAND);
					}
					break;
				}
			}
			break;

			// Helper
		case GAMEUI::MESSAGE_HELPER_CONFIGURE_BUTTON:
			{
				switch (objMsg.Message)
				{
				case GFX_MESSAGE_TYPE::GFX_PRESSED:
					{
						Toggle(GAMEUI::INTERFACE_HELPERSETUP);
					}
					break;
				}
			}
			break;

		case GAMEUI::MESSAGE_HELPER_START_BUTTON:
			{
				switch (objMsg.Message)
				{
				case GFX_MESSAGE_TYPE::GFX_PRESSED:
					{
						if (g_IsHelperStarted == false)
						{
							g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_HELPER_START);
						}
						else
						{
							g_GameServerObject->SendHelperStop();
						}
					}
					break;
				}
			}
			break;

			// Helper Setup
		case GAMEUI::MESSAGE_HELPERSETUP_CLOSE_BUTTON:
			{
				switch (objMsg.Message)
				{
				case GFX_MESSAGE_TYPE::GFX_PRESSED:
					{
						Hide(GAMEUI::INTERFACE_HELPERSETUP);
					}
					break;
				}
			}
			break;

		case GAMEUI::MESSAGE_HELPERSETUP_RESET_BUTTON:
			{
				switch (objMsg.Message)
				{
				case GFX_MESSAGE_TYPE::GFX_PRESSED:
					{
						g_pHelperSetup->Configure(g_DefaultHelperSettings);
					}
					break;
				}
			}
			break;

		case GAMEUI::MESSAGE_HELPERSETUP_SAVE_BUTTON:
			{
				switch (objMsg.Message)
				{
				case GFX_MESSAGE_TYPE::GFX_PRESSED:
					{
						g_pHelperSetup->PrepareSetting();

						if (g_HelperSettings != g_pHelperSetup->_InternalSetting)
						{
							g_HelperSettings = g_pHelperSetup->_InternalSetting;

							CL2GS_RECEIVE_HELPERSETTING ReceiveHelperSetting;

							ReceiveHelperSetting.HelperSetting = g_pHelperSetup->_InternalSetting;

							g_GameServerObject->Send(&ReceiveHelperSetting);
						}
					}
					break;
				}
			}
			break;

		case GAMEUI::MESSAGE_HELPERSETUP_HTRANGEPLUS_BUTTON:
			{
				switch (objMsg.Message)
				{
				case GFX_MESSAGE_TYPE::GFX_PRESSED:
					{
						HELPER_HUNTINGSETTING &huntingSetting = g_pHelperSetup->_InternalSetting.Hunting;
						if (huntingSetting.Range < MAX_HUNTING_RANGE)
						{
							++huntingSetting.Range;
						}
					}
					break;
				}
			}
			break;

		case GAMEUI::MESSAGE_HELPERSETUP_HTRANGEMINUS_BUTTON:
			{
				switch (objMsg.Message)
				{
				case GFX_MESSAGE_TYPE::GFX_PRESSED:
					{
						HELPER_HUNTINGSETTING &huntingSetting = g_pHelperSetup->_InternalSetting.Hunting;
						if (huntingSetting.Range > MIN_HUNTING_RANGE)
						{
							--huntingSetting.Range;
						}
					}
					break;
				}
			}
			break;

		case GAMEUI::MESSAGE_HELPERSETUP_HTPOTION_SETTING_BUTTON:
			{
				switch (objMsg.Message)
				{
				case GFX_MESSAGE_TYPE::GFX_PRESSED:
					{
						mu_uint8 heroClass = GetBaseClass(Hero->Class);

						switch (heroClass)
						{
						case GAME::CLASS_ELF:
						case GAME::CLASS_SUMMONER:
							{
								HELPER_HUNTINGSETTING &huntingSetting = g_pHelperSetup->_InternalSetting.Hunting;
								g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_HELPER_ELFPOTIONSETTING);
								g_pHelperElfPotionSetting->Configure(huntingSetting.PotionSetting);
							}
							break;

						default:
							{
								HELPER_HUNTINGSETTING &huntingSetting = g_pHelperSetup->_InternalSetting.Hunting;
								g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_HELPER_POTIONSETTING);
								g_pHelperPotionSetting->Configure(huntingSetting.PotionSetting);
							}
							break;
						}
					}
					break;
				}
			}
			break;

		case GAMEUI::MESSAGE_HELPERSETUP_HTPARTY_SETTING_BUTTON:
			{
				switch (objMsg.Message)
				{
				case GFX_MESSAGE_TYPE::GFX_PRESSED:
					{
						HELPER_HUNTINGSETTING &huntingSetting = g_pHelperSetup->_InternalSetting.Hunting;

						switch (GetBaseClass(Hero->Class))
						{
						case GAME::CLASS_WIZARD:
						case GAME::CLASS_KNIGHT:
						case GAME::CLASS_DARK_LORD:
						case GAME::CLASS_SUMMONER:
							{
								g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_HELPER_WIZARDPARTYSETTING);
								g_pHelperWizardPartySetting->Configure(huntingSetting.PartySetting);
							}
							break;

						default:
							{
								g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_HELPER_PARTYSETTING);
								g_pHelperPartySetting->Configure(huntingSetting.PartySetting);
							}
							break;
						}
					}
					break;
				}
			}
			break;

		case GAMEUI::MESSAGE_HELPERSETUP_HTACTSKILL1_CON_SETTING_BUTTON:
			{
				switch (objMsg.Message)
				{
				case GFX_MESSAGE_TYPE::GFX_PRESSED:
					{
						HELPER_HUNTINGSETTING &huntingSetting = g_pHelperSetup->_InternalSetting.Hunting;
						g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_HELPER_SKILLSETTING);
						g_pHelperSkillSetting->Configure(BUTTON_SKILLINDEX::BSI_ACT1, huntingSetting.Skills[BUTTON_SKILLINDEX::BSI_ACT1]);
					}
					break;
				}
			}
			break;

		case GAMEUI::MESSAGE_HELPERSETUP_HTACTSKILL2_CON_SETTING_BUTTON:
			{
				switch (objMsg.Message)
				{
				case GFX_MESSAGE_TYPE::GFX_PRESSED:
					{
						HELPER_HUNTINGSETTING &huntingSetting = g_pHelperSetup->_InternalSetting.Hunting;
						g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_HELPER_SKILLSETTING);
						g_pHelperSkillSetting->Configure(BUTTON_SKILLINDEX::BSI_ACT2, huntingSetting.Skills[BUTTON_SKILLINDEX::BSI_ACT2]);
					}
					break;
				}
			}
			break;

		case GAMEUI::MESSAGE_HELPERSETUP_HTSKILLBTN_BEGIN + 0:
		case GAMEUI::MESSAGE_HELPERSETUP_HTSKILLBTN_BEGIN + 1:
		case GAMEUI::MESSAGE_HELPERSETUP_HTSKILLBTN_BEGIN + 2:
		case GAMEUI::MESSAGE_HELPERSETUP_HTSKILLBTN_BEGIN + 3:
		case GAMEUI::MESSAGE_HELPERSETUP_HTSKILLBTN_BEGIN + 4:
		case GAMEUI::MESSAGE_HELPERSETUP_HTSKILLBTN_BEGIN + 5:
			{
				switch (objMsg.Message)
				{
				case GFX_MESSAGE_TYPE::GFX_MOUSE_PRESSED:
					{
						if (MUMisc::IsFlag(objMsg.MessageData, MOUSE_FLAG_RIGHT))
						{
							HELPER_HUNTINGSETTING &huntingSetting = g_pHelperSetup->_InternalSetting.Hunting;
							mu_uint32 index = objMsg.ObjectID - GAMEUI::MESSAGE_HELPERSETUP_HTSKILLBTN_BEGIN;
							g_pHelperSetup->_HTSkillIcon[index]->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
							huntingSetting.Skills[index].SkillIndex = -1;
						}
					}
					break;

				case GFX_MESSAGE_TYPE::GFX_PRESSED:
					{
						HELPER_HUNTINGSETTING &huntingSetting = g_pHelperSetup->_InternalSetting.Hunting;
						mu_uint32 index = objMsg.ObjectID - GAMEUI::MESSAGE_HELPERSETUP_HTSKILLBTN_BEGIN;
						g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_HELPER_SKILLSELECT);
						if (index >= BUTTON_SKILLINDEX::BSI_BUFF1)
						{
							g_pHelperSkillSelect->Configure(index, huntingSetting.Skills[index].SkillIndex, EST_BUFF);
						}
						else
						{
							g_pHelperSkillSelect->Configure(index, huntingSetting.Skills[index].SkillIndex, EST_ATTACK);
						}
					}
					break;
				}
			}
			break;

		case GAMEUI::MESSAGE_HELPERSETUP_OBRANGEPLUS_BUTTON:
			{
				switch (objMsg.Message)
				{
				case GFX_MESSAGE_TYPE::GFX_PRESSED:
					{
						HELPER_OBTAININGSETTINGEX &obtainSetting = g_pHelperSetup->_InternalSetting.Obtaining;
						if (obtainSetting.Range < MAX_HUNTING_RANGE)
						{
							++obtainSetting.Range;
						}
					}
					break;
				}
			}
			break;

		case GAMEUI::MESSAGE_HELPERSETUP_OBRANGEMINUS_BUTTON:
			{
				switch (objMsg.Message)
				{
				case GFX_MESSAGE_TYPE::GFX_PRESSED:
					{
						HELPER_OBTAININGSETTINGEX &obtainSetting = g_pHelperSetup->_InternalSetting.Obtaining;
						if (obtainSetting.Range > MIN_HUNTING_RANGE)
						{
							--obtainSetting.Range;
						}
					}
					break;
				}
			}
			break;

		case GAMEUI::MESSAGE_HELPERSETUP_OBITEMADD_BUTTON:
			{
				switch (objMsg.Message)
				{
				case GFX_MESSAGE_TYPE::GFX_PRESSED:
					{
						if(g_pHelperSetup->_InternalSetting.Obtaining.ExtraItems.size() >= MAX_HELPERSETUP_OBITEMS)
						{
							mu_text textBuffer[512];
							mu_sprintf_s(textBuffer, mu_countof(textBuffer), GlobalText[3570], (mu_int32)MAX_HELPERSETUP_OBITEMS);
							g_pChatListBox->AddText(_T(""), textBuffer, TYPE_ERROR_MESSAGE);
							break;
						}

						g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_HELPER_ITEMADD);
						g_pHelperItemAdd->Configure(-1, nullptr);
					}
					break;
				}
			}
			break;

		case GAMEUI::MESSAGE_HELPERSETUP_OBITEMMOD_BUTTON:
			{
				switch (objMsg.Message)
				{
				case GFX_MESSAGE_TYPE::GFX_PRESSED:
					{
						if (g_pHelperSetup->_OBItemSelected >= 0)
						{
							HELPER_OBTAININGSETTINGEX &obtainSetting = g_pHelperSetup->_InternalSetting.Obtaining;
							g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_HELPER_ITEMADD);
							g_pHelperItemAdd->Configure(g_pHelperSetup->_OBItemSelected, &obtainSetting.ExtraItems[g_pHelperSetup->_OBItemSelected]);
						}
					}
					break;
				}
			}
			break;

		case GAMEUI::MESSAGE_HELPERSETUP_OBITEMDEL_BUTTON:
			{
				switch (objMsg.Message)
				{
				case GFX_MESSAGE_TYPE::GFX_PRESSED:
					{
						if (g_pHelperSetup->_OBItemSelected >= 0)
						{
							g_pHelperSetup->RemoveObtainItem(g_pHelperSetup->_OBItemSelected);
						}
					}
					break;
				}
			}
			break;
		}
	}
}

void UIGameRoot::Toggle(mu_uint32 Key)
{
	if (IsVisible(Key))
	{
		Hide(Key);
	}
	else
	{
		Show(Key);
	}
}

void UIGameRoot::Show(mu_uint32 Key)
{
	switch (Key)
	{
	case GAMEUI::INTERFACE_CHARACTER:
		{
			HideAllGroupC();
			ToggleCharacterInfo(true);
		}
		break;

	case GAMEUI::INTERFACE_MYQUEST:
		{
			HideAllGroupC();
			ToggleMyQuest(true);
			Hide(GAMEUI::INTERFACE_INVENTORY);
			Hide(GAMEUI::INTERFACE_PET);
		}
		break;

	case GAMEUI::INTERFACE_NPCQUEST:
		{
			HideAllGroupA();
			ToggleNpcQuest(true);
		}
		break;

	case GAMEUI::INTERFACE_INVENTORY:
		{
			HideAllGroupC();
			ToggleMyInventory(true);
			Hide(GAMEUI::INTERFACE_MYQUEST);
		}
		break;

	case GAMEUI::INTERFACE_MIXINVENTORY:
		{
			HideAllGroupA();

			g_MixManager.SetPlusChaosRate(0);
#if 0 // FIX
			SendRequestCrywolfBenefitPlusChaosRate();
#endif

			_MixInventoryWindow->SetMixState(UIMixInventory::MIX_WAITING);

			if (g_MixManager.GetMixType() == MIXTYPE_GOBLIN_NORMAL)
			{
				g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_CHAOSMIXMENU);
			}

			ToggleMyInventory(true);
			_MixInventoryWindow->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
			g_pMyInventory->SetRepairMode(false);

			_MixInventoryWindow->Configure();
		}
		break;

	case GAMEUI::INTERFACE_STORAGE:
		{
			HideAllGroupA();
			ToggleMyInventory(true);
			_StorageWindow->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
			g_pMyInventory->SetRepairMode(false);
		}
		break;

	case GAMEUI::INTERFACE_NPCSHOP:
		{
			HideAllGroupA();
			ToggleMyInventory(true);
			_ShopWindow->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
			_ShopWindow->SetShopState(UIShopInventory::SHOP_BUYSELL);
			g_pMyInventory->SetRepairMode(false);
		}
		break;

	case GAMEUI::INTERFACE_TRADE:
		{
			HideAllGroupA();
			ToggleMyInventory(true);
			_TradeWindow->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case GAMEUI::INTERFACE_PARTY:
		{
			HideAllGroupA();
			TogglePartyWindow(true);
		}
		break;

	case GAMEUI::INTERFACE_GUILDINFO:
		{
			HideAllGroupA();
			ToggleGuildWindow(true);
		}
		break;

	case GAMEUI::INTERFACE_NPCGUILDMASTER:
		{
			HideAllGroupA();
			ToggleGuildCreate(true);
		}
		break;

	case GAMEUI::INTERFACE_CHATINPUTBOX:
		{
			ToggleChatInput(true);
		}
		break;

	case GAMEUI::INTERFACE_HERO_POSITION_INFO:
		{
			_HeroPosition->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case GAMEUI::INTERFACE_MOVEMAP:
		{
			_MoveWindow->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case GAMEUI::INTERFACE_BLOODCASTLE:
		{
			HideAllGroupA();
			_BloodCastleEnter->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
			_BloodCastleEnter->Configure();
		}
		break;

	case GAMEUI::INTERFACE_COMMAND:
		{
			if (IsStrifeMap(GET_WORLD) == true)
			{
				if (g_pChatListBox->CheckChatRedundancy(GlobalText[2989]) == false)
				{
					g_pChatListBox->AddText(_T(""), GlobalText[2989], TYPE_SYSTEM_MESSAGE);
				}
			}
			else
			{
				HideAllGroupA();
				_CommandWindow->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
			}
		}
		break;

	case GAMEUI::INTERFACE_HELPERSETUP:
		{
			HideAllGroupA();
			_HelperSetup->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
			_HelperSetup->Configure(g_HelperSettings);
			_HelperSetup->SetCurrentTab(UIHelperSetup::TAB_HUNTING);
		}
		break;
	}

	UpdateWindowPosition();
}

void UIGameRoot::Hide(mu_uint32 Key)
{
	switch (Key)
	{
	case GAMEUI::INTERFACE_CHARACTER:
		{
			ToggleCharacterInfo(false);
		}
		break;

	case GAMEUI::INTERFACE_MYQUEST:
		{
			ToggleMyQuest(false);
		}
		break;

	case GAMEUI::INTERFACE_NPCQUEST:
		{
			ToggleNpcQuest(false);
		}
		break;

	case GAMEUI::INTERFACE_INVENTORY:
		{
			if (_StorageWindow->IsGfxFlag(GFX_FLAG::GFX_HIDDEN) == false)
			{
				Hide(GAMEUI::INTERFACE_STORAGE);
			}
			else if (_ShopWindow->IsGfxFlag(GFX_FLAG::GFX_HIDDEN) == false)
			{
				Hide(GAMEUI::INTERFACE_NPCSHOP);
			}
			else if (_TradeWindow->IsGfxFlag(GFX_FLAG::GFX_HIDDEN) == false)
			{
				Hide(GAMEUI::INTERFACE_TRADE);
			}
			else if (_MixInventoryWindow->IsGfxFlag(GFX_FLAG::GFX_HIDDEN) == false)
			{
				if (g_pMixInventory->GetInventoryControl()->GetNumberOfItems() > 0)
				{
					g_pChatListBox->AddText(_T(""), GlobalText[593], TYPE_ERROR_MESSAGE);
					return;
				}

				switch (g_MixManager.GetMixType())
				{
				case MIXTYPE_ATTACH_SOCKET:
				case MIXTYPE_DETACH_SOCKET:
#if 0 // FIX
					_MixInventoryWindow->_SocketListBox.Clear();
#endif
					break;
				}

				g_GameServerObject->SendCloseWindow();

				_MixInventoryWindow->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
				_MixInventoryWindow->DeleteAllItems();

				g_MixManager.ClearCheckRecipeResult();
			}

			ToggleMyInventory(false);
			g_pMyInventory->SetRepairMode(false);
		}
		break;

	case GAMEUI::INTERFACE_STORAGE:
		{
			g_GameServerObject->SendCloseWindow();
			_StorageWindow->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case GAMEUI::INTERFACE_NPCSHOP:
		{
			_ShopWindow->DeleteAllItems();
			g_GameServerObject->SendCloseWindow();
			_ShopWindow->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case GAMEUI::INTERFACE_TRADE:
		{
			g_pMessageRoot->Hide(MESSAGEBOX::INTERFACE_TRADEMONEY);
			g_pMessageRoot->Hide(MESSAGEBOX::INTERFACE_TRADEACCEPT);
			_TradeWindow->GetMyInventoryControl()->RemoveAllItems();
			_TradeWindow->GetYourInventoryControl()->RemoveAllItems();
			g_GameServerObject->SendCloseWindow();
			_TradeWindow->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case GAMEUI::INTERFACE_PARTY:
		{
			TogglePartyWindow(false);
		}
		break;

	case GAMEUI::INTERFACE_GUILDINFO:
		{
			ToggleGuildWindow(false);
		}
		break;

	case GAMEUI::INTERFACE_NPCGUILDMASTER:
		{
			ToggleGuildCreate(false);
		}
		break;

	case GAMEUI::INTERFACE_CHATINPUTBOX:
		{
			ToggleChatInput(false);
		}
		break;

	case GAMEUI::INTERFACE_HERO_POSITION_INFO:
		{
			_HeroPosition->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case GAMEUI::INTERFACE_MOVEMAP:
		{
			_MoveWindow->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;

	case GAMEUI::INTERFACE_BLOODCASTLE:
		{
			if(_BloodCastleEnter->IsGfxFlag(GFX_FLAG::GFX_HIDDEN) == false)
			{
				_BloodCastleEnter->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
				g_GameServerObject->SendCloseWindow();
			}
		}
		break;

	case GAMEUI::INTERFACE_COMMAND:
		{
			_CommandWindow->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
			_CommandWindow->ClosingWindow();
		}
		break;

	case GAMEUI::INTERFACE_HELPERSETUP:
		{
			_HelperSetup->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}
		break;
	}

	UpdateWindowPosition();
}

mu_boolean UIGameRoot::IsVisible(mu_uint32 Key)
{
	auto it = _ObjectsMap.find(Key);

	if (it != _ObjectsMap.end())
	{
		return it->second->IsGfxFlag(GFX_FLAG::GFX_HIDDEN) == false;
	}

	return false;
}

void UIGameRoot::UpdateWindowPosition()
{
	if (_MyInventoryWindow->IsGfxFlag(GFX_FLAG::GFX_HIDDEN) == false)
	{
		if (_CharacterInfo->IsGfxFlag(GFX_FLAG::GFX_HIDDEN) == false)
		{
			_MyInventoryWindow->ChangePosition(260.0f, 0.0f);
		}
		else
		{
			_MyInventoryWindow->ChangePosition(450.0f, 0.0f);
		}
	}

	mu_int32 screenWidth = GetScreenWidth();
	_BuffWindow->UpdateWindow(screenWidth);
	_PartyList->UpdateWindow(screenWidth);
	_BloodCastleTimer->UpdateWindow(screenWidth);
	_MyQuestWindow->UpdateWindow(screenWidth);
}

void UIGameRoot::ToggleCharacterInfo(mu_boolean isShow)
{
	mu_boolean isFocus = _FocusObject == _MainFrame->_CharInfoBtn;

	if (isShow)
	{
		_MainFrame->_CharInfoBtn->ChangeState(isFocus ? UIToggleButton::UI_PRESSED_OVER : UIToggleButton::UI_PRESSED);
		_CharacterInfo->Configure();
		_CharacterInfo->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
	}
	else
	{
		_MainFrame->_CharInfoBtn->ChangeState(isFocus ? UIToggleButton::UI_OVER : UIToggleButton::UI_NORMAL);
		_CharacterInfo->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
	}
}

void UIGameRoot::ToggleMyQuest(mu_boolean isShow)
{
	if (isShow)
	{
		_MyQuestWindow->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		_MyQuestWindow->SetCurrentTab(UIQuestWindow::TAB_QUEST);

		Hide(GAMEUI::INTERFACE_INVENTORY);
		Hide(GAMEUI::INTERFACE_PET);
	}
	else
	{
		_MyQuestWindow->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
	}
}

void UIGameRoot::ToggleNpcQuest(mu_boolean isShow)
{
	if (isShow)
	{
		_NpcQuestWindow->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		_NpcQuestWindow->Configure();
	}
	else
	{
		_NpcQuestWindow->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		g_GameServerObject->SendCloseWindow();
	}
}

void UIGameRoot::ToggleMyInventory(mu_boolean isShow)
{
	mu_boolean isFocus = _FocusObject == _MainFrame->_MyInvenBtn;
	g_pPickedItem.BackupPickedItem();

	if (isShow)
	{
		_MainFrame->_MyInvenBtn->ChangeState(isFocus ? UIToggleButton::UI_PRESSED_OVER : UIToggleButton::UI_PRESSED);
		_MyInventoryWindow->SetRepairMode(false); 
		_MyInventoryWindow->Configure();
		_MyInventoryWindow->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
	}
	else
	{
		_MainFrame->_MyInvenBtn->ChangeState(isFocus ? UIToggleButton::UI_OVER : UIToggleButton::UI_NORMAL);
		_MyInventoryWindow->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
	}
}

void UIGameRoot::TogglePartyWindow(mu_boolean isShow)
{
	if (isShow)
	{
		_PartyWindow->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
	}
	else
	{
		_PartyWindow->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
	}
}

void UIGameRoot::ToggleGuildWindow(mu_boolean isShow)
{
	if (isShow)
	{
		_GuildWindow->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
	}
	else
	{
		_GuildWindow->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
	}

	_GuildWindow->SetCurrentTab(UIGuildWindow::TAB_GUILD);
}

void UIGameRoot::ToggleGuildCreate(mu_boolean isShow)
{
	if (isShow)
	{
		_GuildCreate->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		_GuildCreate->InitializeWindow();
	}
	else
	{
		_GuildCreate->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		g_GameServerObject->SendCloseWindow();
	}
}

void UIGameRoot::ToggleChatInput(mu_boolean isShow)
{
	if (isShow)
	{
		_ChatInput->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		_ChatInput->_ChatInput->SetText(_T(""));
		_ChatInput->_ChatInput->GiveFocus();

		ToggleChatFrame(!_ChatInput->_ChatFrameButton->IsPressed(), false);
	}
	else
	{
		_ChatInput->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);

		ToggleChatFrame(true, false);
	}
}

void UIGameRoot::ToggleChatFrame(mu_boolean isShow, mu_boolean changeFrame)
{
	mu_boolean isFocus = _FocusObject == _ChatInput->_ChatFrameButton;

	if (isShow == false)
	{
		_ChatWindow->ShowFrame();
	}
	else
	{
		_ChatWindow->HideFrame();
	}

	if(changeFrame == true)
	{
		if (isShow == false)
		{
			_ChatInput->_ChatFrameButton->ChangeState(isFocus ? UIToggleButton::UI_PRESSED_OVER : UIToggleButton::UI_PRESSED);
			_ChatInput->_SizeButton->ChangeState(UIButton::UI_NORMAL);
			_ChatInput->_TransparencyButton->ChangeState(UIButton::UI_NORMAL);
		}
		else
		{
			_ChatInput->_ChatFrameButton->ChangeState(isFocus ? UIToggleButton::UI_OVER : UIToggleButton::UI_NORMAL);
			_ChatInput->_SizeButton->ChangeState(UIButton::UI_DISABLED);
			_ChatInput->_TransparencyButton->ChangeState(UIButton::UI_DISABLED);
		}
	}
}

void UIGameRoot::UpdateChatWindowSize()
{
	_ChatWindow->SetSizeAuto();
}

void UIGameRoot::UpdateChatWindowAlpha()
{
	_ChatWindow->SetBackAlphaAuto();
}

void UIGameRoot::ToggleChatInputType(INPUT_MESSAGE_TYPE inputType)
{
	mu_boolean isPressed[3] = { false, false, false };
	UIRadioButton *buttons[3] = { _ChatInput->_ChatTypeButton, _ChatInput->_PartyTypeButton, _ChatInput->_GuildTypeButton };

	switch (inputType)
	{
	case INPUT_CHAT_MESSAGE:
		{
			isPressed[0] = true;
		}
		break;

	case INPUT_PARTY_MESSAGE:
		{
			isPressed[1] = true;
		}
		break;

	case INPUT_GUILD_MESSAGE:
		{
			isPressed[2] = true;
		}
		break;

	default:
		return;
	}

	for (mu_uint32 i = 0; i < mu_countof(isPressed); ++i)
	{
		mu_boolean isFocus = _FocusObject == buttons[i];
		if (isPressed[i] == true)
		{
			buttons[i]->ChangeState(isFocus ? UIRadioButton::UI_PRESSED_OVER : UIRadioButton::UI_PRESSED);
		}
		else
		{
			buttons[i]->ChangeState(isFocus ? UIRadioButton::UI_OVER : UIRadioButton::UI_NORMAL);
		}
	}

	_ChatInput->SetInputMessageType(inputType);
}

void UIGameRoot::ToggleChatLog()
{
	mu_boolean isShow = !_ChatWindow->_ShowChatLog;
	mu_boolean isFocus = _FocusObject == _ChatInput->_ChatLogButton;

	if (isShow == true)
	{
		_ChatWindow->ShowChatLog();
		_ChatInput->_ChatLogButton->ChangeState(isFocus ? UIToggleButton::UI_PRESSED_OVER : UIToggleButton::UI_PRESSED);
	}
	else
	{
		_ChatWindow->HideChatLog();
		_ChatInput->_ChatLogButton->ChangeState(isFocus ? UIToggleButton::UI_OVER : UIToggleButton::UI_NORMAL);
	}
}

void UIGameRoot::ToggleChatMessageType(MESSAGE_TYPE messageType)
{
	switch (messageType)
	{
	case TYPE_SYSTEM_MESSAGE:
		{
			mu_boolean isFocus = _FocusObject == _ChatInput->_ChatSystemButton;
			_ChatInput->_ChatSystemButton->ChangeState(isFocus == true ? UIToggleButton::UI_PRESSED_OVER : UIToggleButton::UI_PRESSED);
		}
		break;

	default:
		{
			mu_boolean isFocus = _FocusObject == _ChatInput->_ChatSystemButton;
			_ChatInput->_ChatSystemButton->ChangeState(isFocus == true ? UIToggleButton::UI_OVER : UIToggleButton::UI_NORMAL);
		}
		break;
	}

	_ChatWindow->ChangeMessage(messageType);
}

void UIGameRoot::SetBlockWhisper(mu_boolean blockWhisper)
{
	mu_boolean isFocus = _FocusObject == _ChatInput->_ChatSystemButton;

	if (blockWhisper == true)
	{
		_ChatInput->_WhisperBlockButton->ChangeState(isFocus ? UIToggleButton::UI_PRESSED_OVER : UIToggleButton::UI_PRESSED);
	}
	else
	{
		_ChatInput->_WhisperBlockButton->ChangeState(isFocus ? UIToggleButton::UI_OVER : UIToggleButton::UI_NORMAL);
	}
}

mu_boolean UIGameRoot::IsAlwaysVisibleInterface(mu_uint32 Key)
{
	if (Key == GAMEUI::INTERFACE_MAINFRAME
		|| Key == GAMEUI::INTERFACE_SKILL_LIST
		|| Key == GAMEUI::INTERFACE_SLIDEWINDOW
		|| Key == GAMEUI::INTERFACE_MESSAGEBOX
		|| Key == GAMEUI::INTERFACE_CHATLOGWINDOW
		|| Key == GAMEUI::INTERFACE_PARTY_INFO_WINDOW
		|| Key == GAMEUI::INTERFACE_KANTURU_INFO
		|| Key == GAMEUI::INTERFACE_BLOODCASTLE_TIME
		|| Key == GAMEUI::INTERFACE_CHAOSCASTLE_TIME
		|| Key == GAMEUI::INTERFACE_BATTLE_SOCCER_SCORE
		|| Key == GAMEUI::INTERFACE_DUEL_WINDOW
		|| Key == GAMEUI::INTERFACE_CRYWOLF
		|| Key == GAMEUI::INTERFACE_HERO_POSITION_INFO
		|| Key == GAMEUI::INTERFACE_NAME_WINDOW
		|| Key == GAMEUI::INTERFACE_SIEGEWARFARE
		|| Key == GAMEUI::INTERFACE_ITEM_TOOLTIP
		|| Key == GAMEUI::INTERFACE_HOTKEY
		|| Key == GAMEUI::INTERFACE_CURSEDTEMPLE_GAMESYSTEM
		|| Key == GAMEUI::INTERFACE_ITEM_ENDURANCE_INFO
		|| Key == GAMEUI::INTERFACE_BUFF_WINDOW
		|| (Key >= GAMEUI::INTERFACE_3DRENDERING_CAMERA_BEGIN && Key <= GAMEUI::INTERFACE_3DRENDERING_CAMERA_END)
		|| Key == GAMEUI::INTERFACE_DUELWATCH_MAINFRAME
		|| Key == GAMEUI::INTERFACE_DUELWATCH_USERLIST
		|| Key == GAMEUI::INTERFACE_DOPPELGANGER_FRAME
		|| Key == GAMEUI::INTERFACE_GOLD_BOWMAN
		|| Key == GAMEUI::INTERFACE_GOLD_BOWMAN_LENA
		|| Key == GAMEUI::INTERFACE_EMPIREGUARDIAN_TIMER)
	{
		return true;
	}

	return false;
}

void UIGameRoot::HideAll()
{
	for (mu_uint32 n = 0; n < GAMEUI::INTERFACE_END; ++n)
	{
		if (IsAlwaysVisibleInterface(n) == false &&
			IsVisible(n) == true)
		{
			Hide(n);
		}
	}
}

void UIGameRoot::HideAllGroupA()
{
	mu_uint32 GroupA[] =
	{
		GAMEUI::INTERFACE_INVENTORY,
		GAMEUI::INTERFACE_CHARACTER,
		GAMEUI::INTERFACE_MIXINVENTORY,
		GAMEUI::INTERFACE_STORAGE,
		GAMEUI::INTERFACE_NPCSHOP,
		GAMEUI::INTERFACE_MYSHOP_INVENTORY,
		GAMEUI::INTERFACE_PURCHASESHOP_INVENTORY,
		GAMEUI::INTERFACE_PET,
		GAMEUI::INTERFACE_MYQUEST,
		GAMEUI::INTERFACE_NPCQUEST,
		GAMEUI::INTERFACE_PARTY,
		GAMEUI::INTERFACE_SENATUS,
		GAMEUI::INTERFACE_GUARDSMAN,
		GAMEUI::INTERFACE_COMMAND,
		GAMEUI::INTERFACE_HELPERSETUP,
		GAMEUI::INTERFACE_GUILDINFO,
		GAMEUI::INTERFACE_NPCGUILDMASTER,
		GAMEUI::INTERFACE_KANTURU2ND_ENTERNPC,
		GAMEUI::INTERFACE_DUELWATCH,
		GAMEUI::INTERFACE_DOPPELGANGER_NPC,
		GAMEUI::INTERFACE_GOLD_BOWMAN,
		GAMEUI::INTERFACE_GOLD_BOWMAN_LENA,
		GAMEUI::INTERFACE_NPC_DIALOGUE,
		GAMEUI::INTERFACE_QUEST_PROGRESS,
		GAMEUI::INTERFACE_QUEST_PROGRESS_ETC,
		GAMEUI::INTERFACE_EMPIREGUARDIAN_NPC,
		GAMEUI::INTERFACE_GENSRANKING,
		GAMEUI::INTERFACE_UNITEDMARKETPLACE_NPC_JULIA,
	};

	for (mu_uint32 n = 0;n < mu_countof(GroupA); ++n)
	{
		if (IsVisible(GroupA[n]) == true)
		{
			Hide(GroupA[n]);
		}
	}
}

void UIGameRoot::HideAllGroupB()
{
	mu_uint32 GroupB[] =
	{
		GAMEUI::INTERFACE_FRIEND,
		GAMEUI::INTERFACE_INVENTORY,
		GAMEUI::INTERFACE_CHARACTER,
		GAMEUI::INTERFACE_MIXINVENTORY,
		GAMEUI::INTERFACE_STORAGE,
		GAMEUI::INTERFACE_NPCSHOP,
		GAMEUI::INTERFACE_MYSHOP_INVENTORY,
		GAMEUI::INTERFACE_PURCHASESHOP_INVENTORY,
		GAMEUI::INTERFACE_PET,
		GAMEUI::INTERFACE_MYQUEST,
		GAMEUI::INTERFACE_NPCQUEST,
		GAMEUI::INTERFACE_PARTY,
		GAMEUI::INTERFACE_SENATUS,
		GAMEUI::INTERFACE_GUARDSMAN,
		GAMEUI::INTERFACE_COMMAND,
		GAMEUI::INTERFACE_HELPERSETUP,
		GAMEUI::INTERFACE_GUILDINFO,
		GAMEUI::INTERFACE_NPCGUILDMASTER,
		GAMEUI::INTERFACE_KANTURU2ND_ENTERNPC,
		GAMEUI::INTERFACE_CURSEDTEMPLE_NPC,
		GAMEUI::INTERFACE_DUELWATCH,
		GAMEUI::INTERFACE_DOPPELGANGER_NPC,
		GAMEUI::INTERFACE_GOLD_BOWMAN,
		GAMEUI::INTERFACE_GOLD_BOWMAN_LENA,
		GAMEUI::INTERFACE_NPC_DIALOGUE,
		GAMEUI::INTERFACE_QUEST_PROGRESS,
		GAMEUI::INTERFACE_QUEST_PROGRESS_ETC,
		GAMEUI::INTERFACE_EMPIREGUARDIAN_NPC,
		GAMEUI::INTERFACE_GENSRANKING,
		GAMEUI::INTERFACE_UNITEDMARKETPLACE_NPC_JULIA,
	};

	for (mu_uint32 n = 0; n < mu_countof(GroupB); ++n)
	{
		if (IsVisible(GroupB[n]) == true)
		{
			Hide(GroupB[n]);
		}
	}
}

void UIGameRoot::HideAllGroupC()
{
	mu_uint32 GroupC[] =
	{
		GAMEUI::INTERFACE_STORAGE,
		GAMEUI::INTERFACE_NPCSHOP,
		GAMEUI::INTERFACE_TRADE,
		GAMEUI::INTERFACE_PARTY,
		GAMEUI::INTERFACE_COMMAND,
		GAMEUI::INTERFACE_HELPERSETUP,
		GAMEUI::INTERFACE_GUILDINFO,
		GAMEUI::INTERFACE_NPCGUILDMASTER,
		GAMEUI::INTERFACE_GOLD_BOWMAN,
		GAMEUI::INTERFACE_GOLD_BOWMAN_LENA,
		GAMEUI::INTERFACE_GENSRANKING,
	};

	for (mu_uint32 n = 0; n < mu_countof(GroupC); ++n)
	{
		if (IsVisible(GroupC[n]) == true)
		{
			Hide(GroupC[n]);
		}
	}
}

mu_boolean UIGameRoot::AllowHeroAction()
{
	if (IsVisible(GAMEUI::INTERFACE_MIXINVENTORY) == true ||
		IsVisible(GAMEUI::INTERFACE_KANTURU2ND_ENTERNPC) == true)
	{
		return false;
	}

	if (IsVisible(GAMEUI::INTERFACE_TRADE))
	{
		Hide(GAMEUI::INTERFACE_INVENTORY);
	}
	if (IsVisible(GAMEUI::INTERFACE_STORAGE))
	{
		Hide(GAMEUI::INTERFACE_INVENTORY);
	}
	if (IsVisible(GAMEUI::INTERFACE_NPCGUILDMASTER))
	{
		Hide(GAMEUI::INTERFACE_NPCGUILDMASTER);
	}
// 	if (IsVisible(GAMEUI::INTERFACE_MYQUEST))
// 	{
// 		Hide(GAMEUI::INTERFACE_MYQUEST);
// 	}
	if (IsVisible(GAMEUI::INTERFACE_NPCQUEST))
	{
		Hide(GAMEUI::INTERFACE_NPCQUEST);
	}
	if (IsVisible(GAMEUI::INTERFACE_NPCSHOP))
	{
		Hide(GAMEUI::INTERFACE_INVENTORY);
	}
	if (IsVisible(GAMEUI::INTERFACE_GUARDSMAN))
	{
		Hide(GAMEUI::INTERFACE_GUARDSMAN);
	}
	if (IsVisible(GAMEUI::INTERFACE_GUARDSMAN))
	{
		Hide(GAMEUI::INTERFACE_GUARDSMAN);
	}
	if (IsVisible(GAMEUI::INTERFACE_DEVILSQUARE))
	{
		Hide(GAMEUI::INTERFACE_DEVILSQUARE);
	}
	if (IsVisible(GAMEUI::INTERFACE_BLOODCASTLE))
	{
		Hide(GAMEUI::INTERFACE_BLOODCASTLE);
	}
	if (IsVisible(GAMEUI::INTERFACE_CURSEDTEMPLE_NPC))
	{
		Hide(GAMEUI::INTERFACE_CURSEDTEMPLE_NPC);
	}
	if (IsVisible(GAMEUI::INTERFACE_MYSHOP_INVENTORY))
	{
		Hide(GAMEUI::INTERFACE_MYSHOP_INVENTORY);
	}
	if (IsVisible(GAMEUI::INTERFACE_PURCHASESHOP_INVENTORY))
	{
		Hide(GAMEUI::INTERFACE_PURCHASESHOP_INVENTORY);
	}
	if (IsVisible(GAMEUI::INTERFACE_DUELWATCH))
	{
		Hide(GAMEUI::INTERFACE_DUELWATCH);
	}
	if (IsVisible(GAMEUI::INTERFACE_DOPPELGANGER_NPC))
	{
		Hide(GAMEUI::INTERFACE_DOPPELGANGER_NPC);
	}
	if (IsVisible(GAMEUI::INTERFACE_NPC_DIALOGUE))
	{
		Hide(GAMEUI::INTERFACE_NPC_DIALOGUE);
	}
	if (IsVisible(GAMEUI::INTERFACE_QUEST_PROGRESS))
	{
		Hide(GAMEUI::INTERFACE_QUEST_PROGRESS);
	}
	if (IsVisible(GAMEUI::INTERFACE_QUEST_PROGRESS_ETC))
	{
		Hide(GAMEUI::INTERFACE_QUEST_PROGRESS_ETC);
	}
	if (IsVisible(GAMEUI::INTERFACE_EMPIREGUARDIAN_NPC))
	{
		Hide(GAMEUI::INTERFACE_EMPIREGUARDIAN_NPC);
	}
	if (IsVisible(GAMEUI::INTERFACE_LUCKYCOIN_REGISTRATION))
	{
		Hide(GAMEUI::INTERFACE_LUCKYCOIN_REGISTRATION);
	}
	if (IsVisible(GAMEUI::INTERFACE_EXCHANGE_LUCKYCOIN))
	{
		Hide(GAMEUI::INTERFACE_EXCHANGE_LUCKYCOIN);
	}
	if (IsVisible(GAMEUI::INTERFACE_UNITEDMARKETPLACE_NPC_JULIA))
	{
		Hide(GAMEUI::INTERFACE_UNITEDMARKETPLACE_NPC_JULIA);
	}

	return true;
}