#include "stdafx.h"
#include "mu_uishopinventory.h"

#define SCREEN_WIDTH 640.0f
#define SCREEN_HEIGHT 480.0f

#define BACKTEXTURE_WIDTH 225.0f
#define BACKTEXTURE_HEIGHT 512.0f

#define BACK_POSX SHOP_POSX
#define BACK_POSY SHOP_POSY
#define BACK_WIDTH SHOP_WIDTH
#define BACK_HEIGHT SHOP_HEIGHT
#define BACK_UVBEGIN EVector2(0.0f, 0.0f)
#define BACK_UVEND EVector2(BACK_WIDTH / BACKTEXTURE_WIDTH, BACK_HEIGHT / BACKTEXTURE_HEIGHT)

#define TOP_WIDTH SHOP_WIDTH
#define TOP_HEIGHT 64.0f
#define TOP_POSX SHOP_POSX
#define TOP_POSY SHOP_POSY

#define BOTTOM_WIDTH SHOP_WIDTH
#define BOTTOM_HEIGHT 45.0f
#define BOTTOM_POSX SHOP_POSX
#define BOTTOM_POSY (SHOP_POSY + SHOP_HEIGHT - BOTTOM_HEIGHT)

#define LEFT_WIDTH 21.0f
#define LEFT_HEIGHT (BOTTOM_POSY - (TOP_POSY + TOP_HEIGHT))
#define LEFT_POSX SHOP_POSX
#define LEFT_POSY (TOP_POSY + TOP_HEIGHT)

#define RIGHT_WIDTH 21.0f
#define RIGHT_HEIGHT (BOTTOM_POSY - (TOP_POSY + TOP_HEIGHT))
#define RIGHT_POSX (SHOP_POSX + SHOP_WIDTH - RIGHT_WIDTH)
#define RIGHT_POSY (TOP_POSY + TOP_HEIGHT)

#define XBUTTON_WIDTH 16.0f
#define XBUTTON_HEIGHT 16.0f
#define XBUTTON_POSX (TOP_POSX + TOP_WIDTH - 7.0f - XBUTTON_WIDTH)
#define XBUTTON_POSY (TOP_POSY + 7.0f)

UIShopInventory::UIShopInventory(UISceneManager *manager)
{
	if (manager != nullptr)
	{
		manager->AddGFxObject(this);
	}

	_SceneManager = manager;

	UIBase::SetAspectRatio(SCREEN_WIDTH, SCREEN_HEIGHT);
	UIBase::SetPosition(SHOP_POSX, SHOP_POSY);
	UIBase::SetDimensions(SHOP_WIDTH, SHOP_HEIGHT);

	SetGfxFlag(GFX_FLAG::GFX_HIDDEN);
	SetEvent(GFX_EVENT::GFX_MOUSE_EVENT | GFX_EVENT::GFX_MOUSE_DBLCLICK_AS_CLICK);

	// Initialize
	{
		_BackFrame = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::MSGBOX_BACK], SCREEN_WIDTH, SCREEN_HEIGHT, BACK_POSX, BACK_POSY, BACK_WIDTH, BACK_HEIGHT, BACK_UVBEGIN, BACK_UVEND);
		_TopFrame = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::WINDOWFRAME_TOP_EXT], SCREEN_WIDTH, SCREEN_HEIGHT, TOP_POSX, TOP_POSY, TOP_WIDTH, TOP_HEIGHT);
		_TopFrame->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_BottomFrame = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::WINDOWFRAME_BOTTOM], SCREEN_WIDTH, SCREEN_HEIGHT, BOTTOM_POSX, BOTTOM_POSY, BOTTOM_WIDTH, BOTTOM_HEIGHT);
		_BottomFrame->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_LeftFrame = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::WINDOWFRAME_LEFT], SCREEN_WIDTH, SCREEN_HEIGHT, LEFT_POSX, LEFT_POSY, LEFT_WIDTH, LEFT_HEIGHT);
		_LeftFrame->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_RightFrame = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::WINDOWFRAME_RIGHT], SCREEN_WIDTH, SCREEN_HEIGHT, RIGHT_POSX, RIGHT_POSY, RIGHT_WIDTH, RIGHT_HEIGHT);
		_RightFrame->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

		_InventoryControl = new UIInventoryControl(manager, this, TEXTINDEX::BEGIN_SHOP_ITEMNUMBER, SCREEN_WIDTH, SCREEN_HEIGHT, SHOP_POSX + 15.0f, SHOP_POSY + 50.0f, COLUMN_SHOP_INVENTORY, ROW_SHOP_INVENTORY, INVENTORY_TYPE::IT_SHOP);
		_InventoryControl->SetToolTipType(UIInventoryControl::TOOLTIP_TYPE_NPC_SHOP);

		_MoneyIcon = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::ICON_MONEY2], SCREEN_WIDTH, SCREEN_HEIGHT, SHOP_POSX + 10.0f, SHOP_POSY + 355.0f, 170.0f, 24.0f);
		_MoneyIcon->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

		_XButton = new UIButton(GAMEUI::MESSAGE_MYINVENTORY_CLOSE_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, XBUTTON_POSX, XBUTTON_POSY, XBUTTON_WIDTH, XBUTTON_HEIGHT);

		_RepairAllButton = new UIButton(GAMEUI::MESSAGE_NPCSHOP_REPAIRALL_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, SHOP_POSX + 98.0f, SHOP_POSY + 390.0f, 36.0f, 29.0f);
		_RepairAllButton->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_RepairAllButton->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::REPAIR_BUTTON], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(100, 100, 100, 255)));
		_RepairAllButton->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::REPAIR_BUTTON], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_RepairAllButton->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::REPAIR_BUTTON], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_RepairAllButton->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::REPAIR_BUTTON], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_RepairAllButton->SetTooltip(FONT_NORMAL, GlobalText[237], MU::MakeARGB(255, 255, 255, 255), MU::MakeARGB(0, 0, 0, 180), true);

		_RepairButton = new UIButton(GAMEUI::MESSAGE_NPCSHOP_REPAIR_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, SHOP_POSX + 54.0f, SHOP_POSY + 390.0f, 36.0f, 29.0f);
		_RepairButton->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_RepairButton->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::REPAIR_BUTTON], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(100, 100, 100, 255)));
		_RepairButton->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::REPAIR_BUTTON], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_RepairButton->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::REPAIR_BUTTON], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_RepairButton->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::REPAIR_BUTTON], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_RepairButton->SetTooltip(FONT_NORMAL, GlobalText[233], MU::MakeARGB(255, 255, 255, 255), MU::MakeARGB(0, 0, 0, 180), true);

		_TaxRate = 0;
		_RepairShop = false;
		_ShopState = SHOP_BUYSELL;
	}
}

UIShopInventory::~UIShopInventory()
{

}

void UIShopInventory::Update(mu_uint64 CurrentTime)
{
	if (this->GetRepairShop() == true)
	{
		mu_text text[100];

		_AllRepairGold = 0;

		for (mu_int32 i = 0; i < GAME::MAX_EQUIPMENT; ++i)
		{
			if (CharacterMachine->Equipment[i].Type != -1)
			{
				ITEM* ip = &CharacterMachine->Equipment[i];
				ITEM_ATTRIBUTE* p = &ItemAttribute[ip->Type];

				mu_int32 Level = ip->Level;
				mu_int32 maxDurability = calcMaxDurability(ip, p, Level);

				if (IsRepairBan(ip) == true)
				{
					continue;
				}

				if (ip->Durability < maxDurability)
				{
					_AllRepairGold += ConvertRepairGold(ItemValue(ip, 2), ip->Durability, maxDurability, ip->Type, text, mu_countof(text));
				}
			}
		}

		ITEM * pItem = nullptr;
		UIInventoryControl *InventoryControl = g_pMyInventory->GetInventoryControl();
		auto &ItemsMap = InventoryControl->GetItems();

		for (auto it = ItemsMap.begin(); it != ItemsMap.end(); ++it)
		{
			pItem = it->second;

			if (IsRepairBan(pItem) == true)
			{
				continue;
			}

			ITEM_ATTRIBUTE* p = &ItemAttribute[pItem->Type];

			mu_int32 Level = pItem->Level;
			mu_int32 maxDurability = calcMaxDurability(pItem, p, Level);

			if (pItem->Durability < maxDurability)
			{
				_AllRepairGold += ConvertRepairGold(ItemValue(pItem, 2), pItem->Durability, maxDurability, pItem->Type, text, mu_countof(text));
			}
		}
	}
}

void UIShopInventory::Draw()
{
	DrawFrames();
	DrawDetails();
	DrawButtons();
	_InventoryControl->Draw();
}

void UIShopInventory::Draw3D()
{
	_InventoryControl->Draw3D();
}

void UIShopInventory::PostDraw()
{
	_InventoryControl->PostDraw();
	PostDrawButtons();
}

mu_uint32 UIShopInventory::OnPreKeyPressed(const SDL_Keysym &key)
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

UIBase *UIShopInventory::GetFocus(mu_int32 mouseX, mu_int32 mouseY)
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

		if (this->GetRepairShop() == true)
		{
			object = _RepairAllButton->GetFocus(mouseX, mouseY);

			if (object != nullptr)
			{
				return object;
			}

			object = _RepairButton->GetFocus(mouseX, mouseY);

			if (object != nullptr)
			{
				return object;
			}
		}
	}

	return focus;
}

void UIShopInventory::DrawFrames()
{
	_BackFrame->Draw();
	_TopFrame->Draw();
	_BottomFrame->Draw();
	_LeftFrame->Draw();
	_RightFrame->Draw();

	if (this->GetRepairShop() == true)
	{
		_MoneyIcon->Draw();
	}
}

void UIShopInventory::DrawDetails()
{
	FONT_BOLD->DrawStringC2D(TEXTINDEX::SHOP_HEADER, _NativeX + MU_TO_WINDOW(SHOP_WIDTH * 0.5f, _AspectWidth), _NativeY + MU_TO_WINDOW(20.0f, _AspectHeight), 1.0f, ETA_CENTER, ETA_CENTER, 0.0f, MU::MakeARGB(216, 216, 216, 255), MU::MakeARGB(0, 0, 0, 0), GlobalText[230]);
	
	mu_text textBuffer[512];
	mu_sprintf_s(textBuffer, mu_countof(textBuffer), GlobalText[1623], _TaxRate);
	FONT_BOLD->DrawStringC2D(TEXTINDEX::SHOP_TAXHEADER, _NativeX + MU_TO_WINDOW(SHOP_WIDTH * 0.5f, _AspectWidth), _NativeY + MU_TO_WINDOW(32.0f, _AspectHeight), 1.0f, ETA_CENTER, ETA_CENTER, 0.0f, MU::MakeARGB(216, 216, 216, 255), MU::MakeARGB(0, 0, 0, 0), GlobalText[230]);

	if(this->GetRepairShop() == true)
	{
		FONT_BOLD->DrawStringC2D(TEXTINDEX::SHOP_GOLDHEADER, _MoneyIcon->_NativeX + MU_TO_WINDOW(28.0f, _AspectWidth), _MoneyIcon->_NativeY + _MoneyIcon->_Height * 0.45f, 1.0f, ETA_CENTER, ETA_CENTER, 0.0f, MU::MakeARGB(255, 220, 150, 255), MU::MakeARGB(0, 0, 0, 0), GlobalText[239]);

		mu_unicode Text[256];
		ConvertGold(_AllRepairGold, Text, mu_countof(Text));

		FONT_BOLD->DrawStringC2D(TEXTINDEX::SHOP_GOLD, _MoneyIcon->_NativeX + _MoneyIcon->_Width - MU_TO_WINDOW(12.5f, _AspectWidth), _MoneyIcon->_NativeY + _MoneyIcon->_Height * 0.45f, 1.0f, ETA_RIGHT, ETA_CENTER, 0.0f, getGoldColor(_AllRepairGold), MU::MakeARGB(0, 0, 0, 0), Text);
	}
}

void UIShopInventory::DrawButtons()
{
	if (this->GetRepairShop() == true)
	{
		_RepairAllButton->Draw();
		_RepairButton->Draw();
	}
}

void UIShopInventory::PostDrawButtons()
{
	if (this->GetRepairShop() == true)
	{
		_RepairAllButton->PostDraw();
		_RepairButton->PostDraw();
	}
}

mu_boolean UIShopInventory::InsertItem(mu_int32 ColumnX, mu_int32 RowY, SHARED::ITEM_DATA &Item)
{
	ITEM *NewItem = GenerateItem();

	ParseItem(Item, NewItem);

	if (_InventoryControl->AddItem(ColumnX, RowY, NewItem) == true)
		return true;

	DestroyItem(NewItem);

	return false;
}

void UIShopInventory::DeleteItem(mu_int32 ColumnX, mu_int32 RowY)
{
	ITEM* pItem = _InventoryControl->FindItem(ColumnX, RowY);
	if (pItem != nullptr)
	{
		_InventoryControl->RemoveItem(pItem);
	}
}

void UIShopInventory::DeleteAllItems()
{
	_InventoryControl->RemoveAllItems();
}

ITEM *UIShopInventory::FindItem(mu_int32 iLinealPos)
{
	return _InventoryControl->FindItem(iLinealPos);
}

ITEM *UIShopInventory::FindItem(mu_int32 ColumnX, mu_int32 RowY)
{
	return _InventoryControl->FindItem(ColumnX, RowY);
}

ITEM *UIShopInventory::FindItemByKey(mu_uint32 dwKey)
{
	return _InventoryControl->FindItemByKey(dwKey);
}

mu_int32 UIShopInventory::FindEmptySlot(ITEM* pItem)
{
	ITEM_ATTRIBUTE *pItemAttr = &ItemAttribute[pItem->Type];

	return _InventoryControl->FindEmptySlot(pItemAttr->Width, pItemAttr->Height);
}

void UIShopInventory::ToggleState()
{
	if (_ShopState == SHOP_BUYSELL)
	{
		SetShopState(SHOP_REPAIR);
		g_pMyInventory->SetRepairMode(true);
	}
	else
	{
		SetShopState(SHOP_BUYSELL);
		g_pMyInventory->SetRepairMode(false);
	}
}