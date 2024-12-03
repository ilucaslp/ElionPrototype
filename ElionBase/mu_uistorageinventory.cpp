#include "stdafx.h"
#include "mu_uistorageinventory.h"

#define SCREEN_WIDTH 640.0f
#define SCREEN_HEIGHT 480.0f

#define BACKTEXTURE_WIDTH 225.0f
#define BACKTEXTURE_HEIGHT 512.0f

#define BACK_POSX STORAGE_POSX
#define BACK_POSY STORAGE_POSY
#define BACK_WIDTH STORAGE_WIDTH
#define BACK_HEIGHT STORAGE_HEIGHT
#define BACK_UVBEGIN EVector2(0.0f, 0.0f)
#define BACK_UVEND EVector2(BACK_WIDTH / BACKTEXTURE_WIDTH, BACK_HEIGHT / BACKTEXTURE_HEIGHT)

#define TOP_WIDTH STORAGE_WIDTH
#define TOP_HEIGHT 64.0f
#define TOP_POSX STORAGE_POSX
#define TOP_POSY STORAGE_POSY

#define BOTTOM_WIDTH STORAGE_WIDTH
#define BOTTOM_HEIGHT 45.0f
#define BOTTOM_POSX STORAGE_POSX
#define BOTTOM_POSY (STORAGE_POSY + STORAGE_HEIGHT - BOTTOM_HEIGHT)

#define LEFT_WIDTH 21.0f
#define LEFT_HEIGHT (BOTTOM_POSY - (TOP_POSY + TOP_HEIGHT))
#define LEFT_POSX STORAGE_POSX
#define LEFT_POSY (TOP_POSY + TOP_HEIGHT)

#define RIGHT_WIDTH 21.0f
#define RIGHT_HEIGHT (BOTTOM_POSY - (TOP_POSY + TOP_HEIGHT))
#define RIGHT_POSX (STORAGE_POSX + STORAGE_WIDTH - RIGHT_WIDTH)
#define RIGHT_POSY (TOP_POSY + TOP_HEIGHT)

#define XBUTTON_WIDTH 16.0f
#define XBUTTON_HEIGHT 16.0f
#define XBUTTON_POSX (TOP_POSX + TOP_WIDTH - 7.0f - XBUTTON_WIDTH)
#define XBUTTON_POSY (TOP_POSY + 7.0f)

UIStorageInventory::UIStorageInventory(UISceneManager *manager) : UIBase()
{
	if (manager != nullptr)
	{
		manager->AddGFxObject(this);
	}

	_SceneManager = manager;

	UIBase::SetAspectRatio(SCREEN_WIDTH, SCREEN_HEIGHT);
	UIBase::SetPosition(STORAGE_POSX, STORAGE_POSY);
	UIBase::SetDimensions(STORAGE_WIDTH, STORAGE_HEIGHT);

	SetGfxFlag(GFX_FLAG::GFX_HIDDEN);
	SetEvent(GFX_EVENT::GFX_MOUSE_EVENT | GFX_EVENT::GFX_MOUSE_DBLCLICK_AS_CLICK);

	// Initialize
	{
		_BackFrame = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::MSGBOX_BACK], SCREEN_WIDTH, SCREEN_HEIGHT, BACK_POSX, BACK_POSY, BACK_WIDTH, BACK_HEIGHT, BACK_UVBEGIN, BACK_UVEND);
		_TopFrame = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::WINDOWFRAME_TOP], SCREEN_WIDTH, SCREEN_HEIGHT, TOP_POSX, TOP_POSY, TOP_WIDTH, TOP_HEIGHT);
		_TopFrame->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_BottomFrame = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::WINDOWFRAME_BOTTOM], SCREEN_WIDTH, SCREEN_HEIGHT, BOTTOM_POSX, BOTTOM_POSY, BOTTOM_WIDTH, BOTTOM_HEIGHT);
		_BottomFrame->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_LeftFrame = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::WINDOWFRAME_LEFT], SCREEN_WIDTH, SCREEN_HEIGHT, LEFT_POSX, LEFT_POSY, LEFT_WIDTH, LEFT_HEIGHT);
		_LeftFrame->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_RightFrame = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::WINDOWFRAME_RIGHT], SCREEN_WIDTH, SCREEN_HEIGHT, RIGHT_POSX, RIGHT_POSY, RIGHT_WIDTH, RIGHT_HEIGHT);
		_RightFrame->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

		_InventoryControl = new UIInventoryControl(manager, this, TEXTINDEX::BEGIN_STORAGE_ITEMNUMBER, SCREEN_WIDTH, SCREEN_HEIGHT, STORAGE_POSX + 15.0f, STORAGE_POSY + 36.0f, COLUMN_STORAGE, ROW_STORAGE, INVENTORY_TYPE::IT_STORAGE);

		_MoneyIcon = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::ICON_MONEY3], SCREEN_WIDTH, SCREEN_HEIGHT, STORAGE_POSX + 10.0f, STORAGE_POSY + 342.0f, 170.0f, 46.0f);
		_MoneyIcon->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

		_XButton = new UIButton(GAMEUI::MESSAGE_MYINVENTORY_CLOSE_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, XBUTTON_POSX, XBUTTON_POSY, XBUTTON_WIDTH, XBUTTON_HEIGHT);

		_DepositButton = new UIButton(GAMEUI::MESSAGE_STORAGE_DEPOSIT_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, STORAGE_POSX + 38.0f, STORAGE_POSY + 390.0f, 36.0f, 29.0f);
		_DepositButton->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_DepositButton->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::STORAGE_BTN_DEPOSIT_MONEY], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(100, 100, 100, 255)));
		_DepositButton->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::STORAGE_BTN_DEPOSIT_MONEY], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_DepositButton->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::STORAGE_BTN_DEPOSIT_MONEY], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_DepositButton->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::STORAGE_BTN_DEPOSIT_MONEY], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_DepositButton->SetTooltip(FONT_NORMAL, GlobalText[235], MU::MakeARGB(255, 255, 255, 255), MU::MakeARGB(0, 0, 0, 180), true);

		_WithdrawButton = new UIButton(GAMEUI::MESSAGE_STORAGE_WITHDRAW_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, STORAGE_POSX + 78.0f, STORAGE_POSY + 390.0f, 36.0f, 29.0f);
		_WithdrawButton->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_WithdrawButton->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::STORAGE_BTN_WITHDRAW_MONEY], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(100, 100, 100, 255)));
		_WithdrawButton->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::STORAGE_BTN_WITHDRAW_MONEY], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_WithdrawButton->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::STORAGE_BTN_WITHDRAW_MONEY], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_WithdrawButton->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::STORAGE_BTN_WITHDRAW_MONEY], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_WithdrawButton->SetTooltip(FONT_NORMAL, GlobalText[236], MU::MakeARGB(255, 255, 255, 255), MU::MakeARGB(0, 0, 0, 180), true);

		_LockButton = new UIButton(GAMEUI::MESSAGE_STORAGE_LOCK_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, STORAGE_POSX + 118.0f, STORAGE_POSY + 390.0f, 36.0f, 29.0f);
		_LockButton->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_LockButton->SetTooltip(FONT_NORMAL, GlobalText[242], MU::MakeARGB(255, 255, 255, 255), MU::MakeARGB(0, 0, 0, 180), true);

		SetStorageLock(false);
		SetStorageAllowed(false);
		ClearAuto();
	}
}

UIStorageInventory::~UIStorageInventory()
{

}

void UIStorageInventory::Update(mu_uint64 CurrentTime)
{

}

void UIStorageInventory::Draw()
{
	DrawFrames();
	DrawDetails();
	DrawButtons();
	_InventoryControl->Draw();
}

void UIStorageInventory::Draw3D()
{
	_InventoryControl->Draw3D();
}

void UIStorageInventory::PostDraw()
{
	PostDrawButtons();
	_InventoryControl->PostDraw();
}

mu_uint32 UIStorageInventory::OnPreKeyPressed(const SDL_Keysym &key)
{
	switch (key.scancode)
	{
	case SDL_SCANCODE_ESCAPE:
		{
			g_pPickedItem.BackupPickedItem();
			g_pNewUISystem->Hide(GAMEUI::INTERFACE_INVENTORY);
		}
		return GFX_MESSAGE_RETURN::GFX_MESSAGE_ACCEPT;
	}

	return GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE;
}

UIBase *UIStorageInventory::GetFocus(mu_int32 mouseX, mu_int32 mouseY)
{
	UIBase *focus = UIBase::GetFocus(mouseX, mouseY);

	if (focus != nullptr)
	{
		UIBase *object = _InventoryControl->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}

		object = _XButton->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}

		object = _DepositButton->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}

		object = _WithdrawButton->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}

		object = _LockButton->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}
	}

	return focus;
}

void UIStorageInventory::DrawFrames()
{
	_BackFrame->Draw();
	_TopFrame->Draw();
	_BottomFrame->Draw();
	_LeftFrame->Draw();
	_RightFrame->Draw();
}

void UIStorageInventory::DrawDetails()
{
	mu_text textBuffer[512];

	if (_IsLocked)
	{
		mu_sprintf_s(textBuffer, mu_countof(textBuffer), _T("") SPF_STRING _T(" (") SPF_STRING _T(")"), GlobalText[234], GlobalText[241]);

		FONT_BOLD->DrawStringC2D(TEXTINDEX::STORAGE_HEADER, _NativeX + MU_TO_WINDOW(STORAGE_WIDTH * 0.5f, _AspectWidth), _NativeY + MU_TO_WINDOW(15.0f, _AspectHeight), 1.0f, ETA_CENTER, ETA_CENTER, 0.0f, MU::MakeARGB(240, 32, 32, 255), MU::MakeARGB(0, 0, 0, 0), textBuffer);
	}
	else
	{
		mu_sprintf_s(textBuffer, mu_countof(textBuffer), _T("") SPF_STRING _T(" (") SPF_STRING _T(")"), GlobalText[234], GlobalText[240]);

		FONT_BOLD->DrawStringC2D(TEXTINDEX::STORAGE_HEADER, _NativeX + MU_TO_WINDOW(STORAGE_WIDTH * 0.5f, _AspectWidth), _NativeY + MU_TO_WINDOW(15.0f, _AspectHeight), 1.0f, ETA_CENTER, ETA_CENTER, 0.0f, MU::MakeARGB(216, 216, 216, 255), MU::MakeARGB(0, 0, 0, 0), textBuffer);
	}

	_MoneyIcon->Draw();

	mu_int64 TempZen = CharacterMachine->StorageGold;

	mu_unicode Text[256];
	ConvertGold(TempZen, Text, mu_countof(Text));

	FONT_BOLD->DrawStringC2D(TEXTINDEX::STORAGE_GOLD, _NativeX + MU_TO_WINDOW(168.0f, _AspectWidth), _NativeY + MU_TO_WINDOW(353.0f, _AspectHeight), 1.0f, ETA_RIGHT, ETA_CENTER, 0.0f, getGoldColor(TempZen), MU::MakeARGB(0, 0, 0, 0), Text);
	
	FONT_BOLD->DrawStringC2D(TEXTINDEX::STORAGE_FEEHEADER, _NativeX + MU_TO_WINDOW(46.0f, _AspectWidth), _NativeY + MU_TO_WINDOW(374.5f, _AspectHeight), 1.0f, ETA_CENTER, ETA_CENTER, 0.0f, MU::MakeARGB(240, 64, 64, 255), MU::MakeARGB(0, 0, 0, 0), GlobalText[266]);
	
	mu_int64 TotalLevel = CharacterAttribute->Level + Master_Level_Data.nMLevel;
	TempZen = (mu_int64)((mu_double)TotalLevel * (mu_double)TotalLevel * 0.04);
	TempZen += _IsLocked ? (mu_int64)CharacterAttribute->Level * 2 : 0;
	TempZen = EMath::Max(1ll, TempZen);

	if (TempZen >= 1000)
		TempZen = TempZen / 100 * 100;
	else if (TempZen >= 100)
		TempZen = TempZen / 10 * 10;
	ConvertGold(TempZen, Text, mu_countof(Text));
	
	FONT_BOLD->DrawStringC2D(TEXTINDEX::STORAGE_FEE, _NativeX + MU_TO_WINDOW(168.0f, _AspectWidth), _NativeY + MU_TO_WINDOW(375.0f, _AspectHeight), 1.0f, ETA_RIGHT, ETA_CENTER, 0.0f, MU::MakeARGB(255, 220, 150, 255), MU::MakeARGB(0, 0, 0, 0), Text);
}

void UIStorageInventory::DrawButtons()
{
	_DepositButton->Draw();
	_WithdrawButton->Draw();
	_LockButton->Draw();
}

void UIStorageInventory::PostDrawButtons()
{
	_DepositButton->PostDraw();
	_WithdrawButton->PostDraw();
	_LockButton->PostDraw();
}

void UIStorageInventory::SetStorageLock(mu_boolean lock)
{
	_IsLocked = lock;

	if (_IsLocked == true)
	{
		_LockButton->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::STORAGE_BTN_LOCK], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(100, 100, 100, 255)));
		_LockButton->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::STORAGE_BTN_LOCK], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_LockButton->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::STORAGE_BTN_LOCK], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_LockButton->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::STORAGE_BTN_LOCK], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
	}
	else
	{
		_LockButton->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::STORAGE_BTN_UNLOCK], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(100, 100, 100, 255)));
		_LockButton->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::STORAGE_BTN_UNLOCK], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_LockButton->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::STORAGE_BTN_UNLOCK], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_LockButton->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::STORAGE_BTN_UNLOCK], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
	}
}

void UIStorageInventory::SetStorageAllowed(mu_boolean allowed)
{
	_IsAllowed = allowed;
}

mu_boolean UIStorageInventory::InsertItem(mu_int32 ColumnX, mu_int32 RowY, SHARED::ITEM_DATA &Item)
{
	ITEM *NewItem = GenerateItem();

	ParseItem(Item, NewItem);

	if (_InventoryControl->AddItem(ColumnX, RowY, NewItem) == true)
		return true;

	DestroyItem(NewItem);

	return false;
}

void UIStorageInventory::DeleteItem(mu_int32 ColumnX, mu_int32 RowY)
{
	ITEM* pItem = _InventoryControl->FindItem(ColumnX, RowY);
	if (pItem != nullptr)
	{
		_InventoryControl->RemoveItem(pItem);
	}
}

void UIStorageInventory::DeleteAllItems()
{
	_InventoryControl->RemoveAllItems();
}

ITEM *UIStorageInventory::FindItem(mu_int32 iLinealPos)
{
	return _InventoryControl->FindItem(iLinealPos);
}

ITEM *UIStorageInventory::FindItem(mu_int32 ColumnX, mu_int32 RowY)
{
	return _InventoryControl->FindItem(ColumnX, RowY);
}

ITEM *UIStorageInventory::FindItemByKey(mu_uint32 dwKey)
{
	return _InventoryControl->FindItemByKey(dwKey);
}

mu_int32 UIStorageInventory::FindEmptySlot(ITEM* pItem)
{
	ITEM_ATTRIBUTE *pItemAttr = &ItemAttribute[pItem->Type];

	return _InventoryControl->FindEmptySlot(pItemAttr->Width, pItemAttr->Height);
}

void UIStorageInventory::ProcessAutoAction()
{
	switch (_CurrentAction)
	{
	case WHAUTO_DEPOSIT:
		{
			g_GameServerObject->SendStorageDeposit(_GoldAction);
		}
		break;

	case WHAUTO_WITHDRAW:
		{
			g_GameServerObject->SendStorageWithdraw(_GoldAction);
		}
		break;

	case WHAUTO_MOVEITEM:
		{
			CL2GS_REQUEST_MOVEITEM RequestMoveItem;

			RequestMoveItem.SrcType = _SrcType;
			RequestMoveItem.SrcX = _SrcX;
			RequestMoveItem.SrcY = _SrcY;

			RequestMoveItem.DestType = _DestType;
			RequestMoveItem.DestX = _DestX;
			RequestMoveItem.DestY = _DestY;

			g_GameServerObject->Send(&RequestMoveItem);
		}
		break;
	}

	this->ClearAuto();
}