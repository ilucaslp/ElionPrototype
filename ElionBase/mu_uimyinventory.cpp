#include "stdafx.h"
#include "mu_uimyinventory.h"

#define SCREEN_WIDTH 640.0f
#define SCREEN_HEIGHT 480.0f

#define BACKTEXTURE_WIDTH 225.0f
#define BACKTEXTURE_HEIGHT 512.0f

#define BACK_POSX MYINVENTORY_POSX
#define BACK_POSY MYINVENTORY_POSY
#define BACK_WIDTH MYINVENTORY_WIDTH
#define BACK_HEIGHT MYINVENTORY_HEIGHT
#define BACK_UVBEGIN EVector2(0.0f, 0.0f)
#define BACK_UVEND EVector2(BACK_WIDTH / BACKTEXTURE_WIDTH, BACK_HEIGHT / BACKTEXTURE_HEIGHT)

#define TOP_WIDTH MYINVENTORY_WIDTH
#define TOP_HEIGHT 64.0f
#define TOP_POSX MYINVENTORY_POSX
#define TOP_POSY MYINVENTORY_POSY

#define BOTTOM_WIDTH MYINVENTORY_WIDTH
#define BOTTOM_HEIGHT 45.0f
#define BOTTOM_POSX MYINVENTORY_POSX
#define BOTTOM_POSY (MYINVENTORY_POSY + MYINVENTORY_HEIGHT - BOTTOM_HEIGHT)

#define LEFT_WIDTH 21.0f
#define LEFT_HEIGHT (BOTTOM_POSY - (TOP_POSY + TOP_HEIGHT))
#define LEFT_POSX MYINVENTORY_POSX
#define LEFT_POSY (TOP_POSY + TOP_HEIGHT)

#define RIGHT_WIDTH 21.0f
#define RIGHT_HEIGHT (BOTTOM_POSY - (TOP_POSY + TOP_HEIGHT))
#define RIGHT_POSX (MYINVENTORY_POSX + MYINVENTORY_WIDTH - RIGHT_WIDTH)
#define RIGHT_POSY (TOP_POSY + TOP_HEIGHT)

#define XBUTTON_WIDTH 16.0f
#define XBUTTON_HEIGHT 16.0f
#define XBUTTON_POSX (TOP_POSX + TOP_WIDTH - 7.0f - XBUTTON_WIDTH)
#define XBUTTON_POSY (TOP_POSY + 7.0f)

EVector2 EquipmentPos[GAME::MAX_EQUIPMENT] = {
	EVector2(15.0f, 87.0f), // Weapon Right
	EVector2(135.0f, 87.0f), // Weapon Left
	EVector2(75.0f, 44.0f), // Helm
	EVector2(75.0f, 87.0f), // Armor
	EVector2(75.0f, 150.0f), // Pants
	EVector2(15.0f, 150.0f), // Gloves
	EVector2(135.0f, 150.0f), // Boots
	EVector2(120.0f, 44.0f), // Wings
	EVector2(15.0f, 44.0f), // Helper
	EVector2(54.0f, 87.0f), // Amulet
	EVector2(54.0f, 150.0f), // Ring Right
	EVector2(114.0f, 150.0f), // Ring Left
};

EVector2 EquipmentSize[GAME::MAX_EQUIPMENT] = {
	EVector2(46.0f, 66.0f), // Weapon Right
	EVector2(46.0f, 66.0f), // Weapon Left
	EVector2(46.0f, 46.0f), // Helm
	EVector2(46.0f, 66.0f), // Armor
	EVector2(46.0f, 46.0f), // Pants
	EVector2(46.0f, 46.0f), // Gloves
	EVector2(46.0f, 46.0f), // Boots
	EVector2(61.0f, 46.0f), // Wings
	EVector2(46.0f, 46.0f), // Helper
	EVector2(28.0f, 28.0f), // Amulet
	EVector2(28.0f, 28.0f), // Ring Right
	EVector2(28.0f, 28.0f), // Ring Left
};

MUTexture *EquipmentTextures[GAME::MAX_EQUIPMENT] = {
	&INTERFACES::Data[INTERFACES::ICON_WEAPON_RIGHT],
	&INTERFACES::Data[INTERFACES::ICON_WEAPON_LEFT],
	&INTERFACES::Data[INTERFACES::ICON_HELM],
	&INTERFACES::Data[INTERFACES::ICON_ARMOR],
	&INTERFACES::Data[INTERFACES::ICON_PANTS],
	&INTERFACES::Data[INTERFACES::ICON_GLOVES],
	&INTERFACES::Data[INTERFACES::ICON_BOOTS],
	&INTERFACES::Data[INTERFACES::ICON_WING],
	&INTERFACES::Data[INTERFACES::ICON_HELPER],
	&INTERFACES::Data[INTERFACES::ICON_NECKLACE],
	&INTERFACES::Data[INTERFACES::ICON_RING],
	&INTERFACES::Data[INTERFACES::ICON_RING],
};

UIMyInventory::UIMyInventory(UISceneManager *manager) : UIBase()
{
	if (manager != nullptr)
	{
		manager->AddGFxObject(this);
	}

	_SceneManager = manager;

	UIBase::SetAspectRatio(SCREEN_WIDTH, SCREEN_HEIGHT);
	UIBase::SetPosition(MYINVENTORY_POSX, MYINVENTORY_POSY);
	UIBase::SetDimensions(MYINVENTORY_WIDTH, MYINVENTORY_HEIGHT);

	SetGfxFlag(GFX_FLAG::GFX_HIDDEN);
	SetEvent(GFX_EVENT::GFX_MOUSE_EVENT | GFX_EVENT::GFX_MOUSE_DBLCLICK_AS_CLICK);

	// Initialize
	{
		_PointedSlot = -1;
		_SlotIndex = 0;
		_SlotCount = 0;

		_RepairMode = false;

		_BackFrame = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::MSGBOX_BACK], SCREEN_WIDTH, SCREEN_HEIGHT, BACK_POSX, BACK_POSY, BACK_WIDTH, BACK_HEIGHT, BACK_UVBEGIN, BACK_UVEND);
		_TopFrame = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::WINDOWFRAME_TOP_EXT], SCREEN_WIDTH, SCREEN_HEIGHT, TOP_POSX, TOP_POSY, TOP_WIDTH, TOP_HEIGHT);
		_TopFrame->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_BottomFrame = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::WINDOWFRAME_BOTTOM], SCREEN_WIDTH, SCREEN_HEIGHT, BOTTOM_POSX, BOTTOM_POSY, BOTTOM_WIDTH, BOTTOM_HEIGHT);
		_BottomFrame->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_LeftFrame = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::WINDOWFRAME_LEFT], SCREEN_WIDTH, SCREEN_HEIGHT, LEFT_POSX, LEFT_POSY, LEFT_WIDTH, LEFT_HEIGHT);
		_LeftFrame->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_RightFrame = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::WINDOWFRAME_RIGHT], SCREEN_WIDTH, SCREEN_HEIGHT, RIGHT_POSX, RIGHT_POSY, RIGHT_WIDTH, RIGHT_HEIGHT);
		_RightFrame->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

		for (mu_uint32 i = 0; i < GAME::MAX_EQUIPMENT; ++i)
		{
			_EquipmentFrame[i] = new UISprite(manager, this, EquipmentTextures[i], SCREEN_WIDTH, SCREEN_HEIGHT, MYINVENTORY_POSX + EquipmentPos[i].x, MYINVENTORY_POSY + EquipmentPos[i].y, EquipmentSize[i].x, EquipmentSize[i].y);
			_EquipmentFrame[i]->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
			
			_EquipmentColor[i] = new UISpriteColor(manager, this, MU::MakeARGB(255, 255, 255, 255), SCREEN_WIDTH, SCREEN_HEIGHT, MYINVENTORY_POSX + EquipmentPos[i].x + 1.0f, MYINVENTORY_POSY + EquipmentPos[i].y, EquipmentSize[i].x - 4.0f, EquipmentSize[i].y - 4.0f);
			_EquipmentColor[i]->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

			_EquipmentPointed[i] = new UISpriteColor(manager, this, MU::MakeARGB(229, 25, 25, 102), SCREEN_WIDTH, SCREEN_HEIGHT, MYINVENTORY_POSX + EquipmentPos[i].x + 1.0f, MYINVENTORY_POSY + EquipmentPos[i].y, EquipmentSize[i].x - 4.0f, EquipmentSize[i].y - 4.0f);
			_EquipmentPointed[i]->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		}

		_InventoryControl = new UIInventoryControl(manager, this, TEXTINDEX::BEGIN_MYINVENTORY_ITEMNUMBER, SCREEN_WIDTH, SCREEN_HEIGHT, MYINVENTORY_POSX + 15.0f, MYINVENTORY_POSY + 200.0f, COLUMN_INVENTORY, ROW_INVENTORY, INVENTORY_TYPE::IT_INVENTORY);

		_MoneyIcon = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::ICON_MONEY1], SCREEN_WIDTH, SCREEN_HEIGHT, MYINVENTORY_POSX + 11.0f, MYINVENTORY_POSY + 364.0f, 170.0f, 26.0f);
		_MoneyIcon->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

		_XButton = new UIButton(GAMEUI::MESSAGE_MYINVENTORY_CLOSE_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, XBUTTON_POSX, XBUTTON_POSY, XBUTTON_WIDTH, XBUTTON_HEIGHT);

		_CloseButton = new UIButton(GAMEUI::MESSAGE_MYINVENTORY_CLOSE_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, MYINVENTORY_POSX + 13.0f, MYINVENTORY_POSY + 391.0f, 36.0f, 29.0f);
		_CloseButton->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_CloseButton->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EXIT_BUTTON], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(100, 100, 100, 255)));
		_CloseButton->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EXIT_BUTTON], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_CloseButton->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EXIT_BUTTON], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_CloseButton->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EXIT_BUTTON], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_CloseButton->SetTooltip(FONT_NORMAL, GlobalText[225], MU::MakeARGB(255, 255, 255, 255), MU::MakeARGB(0, 0, 0, 180), true);

		_RepairButton = new UIButton(GAMEUI::MESSAGE_MYINVENTORY_REPAIR_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, MYINVENTORY_POSX + 50.0f, MYINVENTORY_POSY + 391.0f, 36.0f, 29.0f);
		_RepairButton->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_RepairButton->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::REPAIR_BUTTON], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_RepairButton->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::REPAIR_BUTTON], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_RepairButton->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::REPAIR_BUTTON], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_RepairButton->SetTooltip(FONT_NORMAL, GlobalText[233], MU::MakeARGB(255, 255, 255, 255), MU::MakeARGB(0, 0, 0, 180), true);

		_PShopButton = new UIButton(GAMEUI::MESSAGE_MYINVENTORY_PSHOP_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, MYINVENTORY_POSX + 87.0f, MYINVENTORY_POSY + 391.0f, 36.0f, 29.0f);
		_PShopButton->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_PShopButton->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::PSHOP_OPEN_BUTTON], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_PShopButton->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::PSHOP_OPEN_BUTTON], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_PShopButton->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::PSHOP_OPEN_BUTTON], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_PShopButton->SetTooltip(FONT_NORMAL, GlobalText[1125], MU::MakeARGB(255, 255, 255, 255), MU::MakeARGB(0, 0, 0, 180), true);

		SetPosition(MYINVENTORY_POSX, MYINVENTORY_POSY);
	}
}

UIMyInventory::~UIMyInventory()
{

}

void UIMyInventory::ChangePosition(mu_float x, mu_float y)
{
#define SETPOSITION_HELPER(var) var->SetPosition(var->_ViewX - oldX + x, var->_ViewY - oldY + y)

	mu_float oldX = _ViewX, oldY = _ViewY;

	SETPOSITION_HELPER(this);

	SETPOSITION_HELPER(_BackFrame);
	SETPOSITION_HELPER(_TopFrame);
	SETPOSITION_HELPER(_BottomFrame);
	SETPOSITION_HELPER(_LeftFrame);
	SETPOSITION_HELPER(_RightFrame);

	for (mu_uint32 i = 0; i < GAME::MAX_EQUIPMENT; ++i)
	{
		SETPOSITION_HELPER(_EquipmentFrame[i]);
		SETPOSITION_HELPER(_EquipmentColor[i]);
		SETPOSITION_HELPER(_EquipmentPointed[i]);
	}

	SETPOSITION_HELPER(_InventoryControl);

	SETPOSITION_HELPER(_MoneyIcon);

	SETPOSITION_HELPER(_XButton);
	SETPOSITION_HELPER(_CloseButton);
	SETPOSITION_HELPER(_RepairButton);
	SETPOSITION_HELPER(_PShopButton);

#undef SETPOSITION_HELPER
}

void UIMyInventory::Update(mu_uint64 CurrentTime)
{

}

void UIMyInventory::Draw()
{
	DrawFrames();
	DrawInventoryDetails();
	DrawSetOption();
	DrawSocketOption();
	DrawButtons();
	_InventoryControl->Draw();
	DrawEquippedItem();
}

void UIMyInventory::Draw3D()
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[0];
	++ThreadInfo.GUIRenderIndex;

	_SlotIndex = g_GUIRenderSlotCount;

	ITEM *PickedItem = g_pPickedItem.GetItem();

	if (PickedItem == nullptr ||
		g_pPickedItem.IsVisible() == false)
	{
		for (mu_uint32 i = 0; i < MAX_EQUIPMENT_INDEX; ++i)
		{
			ITEM* pEquippedItem = &CharacterMachine->Equipment[i];
			if (pEquippedItem->Type >= 0)
			{
				mu_float y = 0.0f;
				if (i == GAME::EQUIPMENT_ARMOR)
				{
					y = _EquipmentFrame[i]->_ViewY - 10.0f;
				}
				else
				{
					y = _EquipmentFrame[i]->_ViewY;
				}

				ThreadInfo.RenderLight = MU::Float4ToARGB(1.0f, 1.0f, 1.0f, 1.0f);

				RenderItem3D(
					_EquipmentFrame[i]->_ViewX,
					y,
					_EquipmentFrame[i]->_ViewWidth - 4.0f,
					_EquipmentFrame[i]->_ViewHeight - 4.0f,
					pEquippedItem->Type,
					pEquippedItem->Level,
					pEquippedItem->ExcOption,
					pEquippedItem->SetOption,
					false);
			}
		}
	}
	else
	{
		for (mu_uint32 i = 0; i < MAX_EQUIPMENT_INDEX; ++i)
		{
			ITEM* pEquippedItem = &CharacterMachine->Equipment[i];
			if (pEquippedItem->Type >= 0)
			{
				mu_float y = 0.0f;
				if (i == GAME::EQUIPMENT_ARMOR)
				{
					y = _EquipmentFrame[i]->_ViewY - 10.0f;
				}
				else
				{
					y = _EquipmentFrame[i]->_ViewY;
				}

				if (pEquippedItem == PickedItem)
				{
					ThreadInfo.RenderLight = MU::Float4ToARGB(0.2f, 0.2f, 0.2f, 0.3f);
				}
				else
				{
					ThreadInfo.RenderLight = MU::Float4ToARGB(1.0f, 1.0f, 1.0f, 1.0f);
				}

				RenderItem3D(
					_EquipmentFrame[i]->_ViewX,
					y,
					_EquipmentFrame[i]->_ViewWidth - 4.0f,
					_EquipmentFrame[i]->_ViewHeight - 4.0f,
					pEquippedItem->Type,
					pEquippedItem->Level,
					pEquippedItem->ExcOption,
					pEquippedItem->SetOption,
					false);
			}
		}
	}

	_SlotCount = g_GUIRenderSlotCount - _SlotIndex;

	_InventoryControl->Draw3D();
}

void UIMyInventory::PostDraw()
{
	PostDrawSetOption();
	PostDrawSocketOption();
	_InventoryControl->PostDraw();
	PostDrawEquippedItem();
	PostDrawButtons();
}

void UIMyInventory::DrawFrames()
{
	_BackFrame->Draw();
	_TopFrame->Draw();
	_BottomFrame->Draw();
	_LeftFrame->Draw();
	_RightFrame->Draw();
}

void UIMyInventory::DrawInventoryDetails()
{
	FONT_BOLD->DrawStringC2D(TEXTINDEX::MYINVENTORY_HEADER, _NativeX + MU_TO_WINDOW(MYINVENTORY_WIDTH * 0.5f, _AspectWidth), _NativeY + MU_TO_WINDOW(20.0f, _AspectHeight), 1.0f, ETA_CENTER, ETA_CENTER, 0.0f, MU::MakeARGB(255, 255, 255, 255), MU::MakeARGB(0, 0, 0, 0), GlobalText[223]);
	
	_MoneyIcon->Draw();

#if 0 // FIX
	CPCRoomPtSys& rPCRoomPtSys = CPCRoomPtSys::Instance();
	if (rPCRoomPtSys.IsPCRoomPointShopMode())
	{
		FONT_BOLD->DrawStringC2D(TEXTINDEX::MYINVENTORY_PCROOM1, _NativeX + MU_TO_WINDOW(50.0f, _AspectWidth), _NativeY + MU_TO_WINDOW(375.0f, _AspectHeight), 1.0f, TEXT_ALIGN_LEFT, TEXT_ALIGN_CENTER, 2.0f, MU::MakeARGB(255, 220, 250, 255), MU::MakeARGB(40, 40, 40, 255), GlobalText[2326]);

		mu_unicode Text[256];
		mu_sprintf_s(Text, mu_countof(Text), _T("%d"), rPCRoomPtSys.GetNowPoint());
		FONT_BOLD->DrawStringC2D(TEXTINDEX::MYINVENTORY_PCROOM2, _NativeX + MU_TO_WINDOW(82.0f, _AspectWidth), _NativeY + MU_TO_WINDOW(375.0f, _AspectHeight), 1.0f, TEXT_ALIGN_LEFT, TEXT_ALIGN_CENTER, 2.0f, MU::MakeARGB(255, 220, 250, 255), MU::MakeARGB(40, 40, 40, 255), Text);
	}
	else
#endif
	{
		mu_int64 dwZen = CharacterMachine->Gold;

		mu_unicode Text[256];
		ConvertGold(dwZen, Text, mu_countof(Text));

		FONT_BOLD->DrawStringC2D(TEXTINDEX::MYINVENTORY_GOLD, _NativeX + MU_TO_WINDOW(50.0f, _AspectWidth), _NativeY + MU_TO_WINDOW(375.0f, _AspectHeight), 1.0f, ETA_LEFT, ETA_CENTER, 0.0f, getGoldColor(dwZen), MU::MakeARGB(0, 0, 0, 0), Text);
	}
}

void UIMyInventory::DrawSetOption()
{
	mu_uint32 backColor, textColor;

	backColor = MU::MakeARGB(0, 0, 0, 0);
	if (g_csItemOption.GetSetOptionANum() > 0 || g_csItemOption.GetSetOptionBNum() > 0)
	{
		textColor = MU::MakeARGB(255, 204, 25, 255);
	}
	else
	{
		textColor = MU::MakeARGB(100, 100, 100, 255);
	}

	mu_unicode strText[128];
	mu_sprintf_s(strText, mu_countof(strText), _T("[") SPF_STRING _T("]"), GlobalText[989]);
	FONT_BOLD->DrawStringC2D(TEXTINDEX::MYINVENTORY_HEADER, _NativeX + MU_TO_WINDOW(MYINVENTORY_WIDTH * 0.5f - 1.0f, _AspectWidth), _NativeY + MU_TO_WINDOW(32.0f, _AspectHeight), 1.0f, ETA_RIGHT, ETA_CENTER, 0.0f, textColor, backColor, strText);
}

void UIMyInventory::DrawSocketOption()
{
	mu_uint32 backColor, textColor;

	backColor = MU::MakeARGB(0, 0, 0, 0);

	if (g_SocketItemMgr.IsSocketSetOptionEnabled())
	{
		textColor = MU::MakeARGB(255, 204, 25, 255);
	}
	else
	{
		textColor = MU::MakeARGB(100, 100, 100, 255);
	}

	mu_unicode strText[128];
	mu_sprintf_s(strText, mu_countof(strText), _T("[") SPF_STRING _T("]"), GlobalText[2651]);
	FONT_BOLD->DrawStringC2D(TEXTINDEX::MYINVENTORY_HEADER, _NativeX + MU_TO_WINDOW(MYINVENTORY_WIDTH * 0.5f + 1.0f, _AspectWidth), _NativeY + MU_TO_WINDOW(32.0f, _AspectHeight), 1.0f, ETA_LEFT, ETA_CENTER, 0.0f, textColor, backColor, strText);
}

void UIMyInventory::DrawButtons()
{
	_CloseButton->Draw();
	_RepairButton->Draw();
	_PShopButton->Draw();
}

void UIMyInventory::PostDrawButtons()
{
	_CloseButton->PostDraw();
	_RepairButton->PostDraw();
	_PShopButton->PostDraw();
}

void UIMyInventory::DrawEquippedItem()
{
	ITEM *PickedItem = g_pPickedItem.GetItem();

	for (mu_int32 i = 0; i < MAX_EQUIPMENT_INDEX; ++i)
	{
		if (i == GAME::EQUIPMENT_HELM)
		{
			if (GetBaseClass(Hero->Class) == GAME::CLASS_DARK)
			{
				continue;
			}
		}

		_EquipmentFrame[i]->Draw();

		ITEM* pEquipmentItemSlot = &CharacterMachine->Equipment[i];

		if (PickedItem != nullptr &&
			PickedItem != pEquipmentItemSlot &&
			_PointedSlot == i)
		{
			if (pEquipmentItemSlot->Type != -1 || IsEquipable(_PointedSlot, PickedItem) == false)
			{
				_EquipmentPointed[_PointedSlot]->SetColor(MU::MakeARGB(229, 25, 25, 102));
			}
			else
			{
				_EquipmentPointed[_PointedSlot]->SetColor(MU::MakeARGB(25, 102, 204, 102));
			}

			_EquipmentPointed[_PointedSlot]->Draw();
			continue;
		}

		if (pEquipmentItemSlot->Type != -1)
		{
			ITEM_ATTRIBUTE* pItemAttr = &ItemAttribute[pEquipmentItemSlot->Type];
			mu_int32 iLevel = pEquipmentItemSlot->Level;
			mu_int32 iMaxDurability = calcMaxDurability(pEquipmentItemSlot, pItemAttr, iLevel);

			if (i == GAME::EQUIPMENT_RING_LEFT || i == GAME::EQUIPMENT_RING_RIGHT)
			{
				if (pEquipmentItemSlot->Type == ITEM_HELPER + 20 &&
					(iLevel == 1 || iLevel == 2))
				{
					continue;
				}
			}

			if (PickedItem != nullptr &&
				PickedItem == pEquipmentItemSlot)
				_EquipmentColor[i]->SetColor(MU::MakeARGB(183, 183, 255, 64));
			else if (pEquipmentItemSlot->Durability <= 0)
				_EquipmentColor[i]->SetColor(MU::MakeARGB(255, 0, 0, 64));
			else if (pEquipmentItemSlot->Durability <= (iMaxDurability*0.2f))
				_EquipmentColor[i]->SetColor(MU::MakeARGB(255, 38, 0, 64));
			else if (pEquipmentItemSlot->Durability <= (iMaxDurability*0.3f))
				_EquipmentColor[i]->SetColor(MU::MakeARGB(255, 127, 0, 64));
			else if (pEquipmentItemSlot->Durability <= (iMaxDurability*0.5f))
				_EquipmentColor[i]->SetColor(MU::MakeARGB(255, 255, 0, 64));
			else if (IsEquipable(i, pEquipmentItemSlot) == false)
				_EquipmentColor[i]->SetColor(MU::MakeARGB(255, 0, 0, 64));
			else
			{
				continue;
			}

			_EquipmentColor[i]->Draw();
		}
	}

	MUThreadInfo &ThreadInfo = g_ThreadInfo[0];

	if (_SlotCount > 0)
	{
		ThreadInfo.EnableScissor = false;
		g_Tasking.RenderGUIModelsAtRuntime(_SlotIndex, _SlotCount);
	}
}

void UIMyInventory::PostDrawSetOption()
{
	if (g_csItemOption.IsViewOptionList() == true)
	{
		g_csItemOption.RenderSetOptionList(_ViewX, _ViewY);
	}
}

void UIMyInventory::PostDrawSocketOption()
{
	if (_SceneManager->GetFocused() == this &&
		CheckMouseIn(_NativeX + MU_TO_WINDOW(MYINVENTORY_WIDTH * 0.5f, _AspectWidth), _NativeY + MU_TO_WINDOW(20.0f, _AspectHeight), MU_TO_WINDOW(MYINVENTORY_WIDTH * 0.5f, _AspectWidth), MU_TO_WINDOW(15.0f, _AspectHeight)) == true)
	{
		g_SocketItemMgr.RenderToolTipForSocketSetOption(_ViewX, _ViewY);
	}
}

void UIMyInventory::PostDrawEquippedItem()
{
	if (_PointedSlot != -1)
	{
		RenderItemToolTip(_PointedSlot);
	}
}

void UIMyInventory::OnMouseLeave()
{
	_PointedSlot = -1;
}

void UIMyInventory::OnMouseMove(mu_int32 mouse_x, mu_int32 mouse_y)
{
	_PointedSlot = -1;

	for (mu_uint32 n = 0; n < MAX_EQUIPMENT_INDEX; ++n)
	{
		if (_EquipmentFrame[n]->IsInside(mouse_x, mouse_y))
		{
			_PointedSlot = n;
		}
	}
}

void UIMyInventory::OnMousePressed(mu_uint32 mouse_flags, mu_uint32 double_flags, mu_uint32 virtual_keys)
{
	if (_InventoryControl->IsLocked() == true)
	{
		if (g_pPickedItem.GetItem() != nullptr)
		{
			g_pPickedItem.BackupPickedItem();
		}
		return;
	}

	if (MUMisc::IsFlag(mouse_flags, MOUSE_FLAG_LEFT) && _PointedSlot != -1)
	{
		if (g_pPickedItem.GetItem() != nullptr)
		{
			ITEM *pItemObj = g_pPickedItem.GetItem();

			ITEM* pEquipmentItemSlot = &CharacterMachine->Equipment[_PointedSlot];
			if (pEquipmentItemSlot != pItemObj && pEquipmentItemSlot->Type != -1)
			{
				return;
			}

			if (pEquipmentItemSlot == pItemObj)
			{
				g_pPickedItem.BackupPickedItem();
				return;
			}

			if (g_ChangeRingMgr.CheckChangeRing(pItemObj->Type))
			{
				ITEM * pItemRingLeft = &CharacterMachine->Equipment[GAME::EQUIPMENT_RING_LEFT];
				ITEM * pItemRingRight = &CharacterMachine->Equipment[GAME::EQUIPMENT_RING_RIGHT];

				if (g_ChangeRingMgr.CheckChangeRing(pItemRingLeft->Type) ||
					g_ChangeRingMgr.CheckChangeRing(pItemRingRight->Type))
				{
					g_pChatListBox->AddText(_T(""), GlobalText[3089], TYPE_ERROR_MESSAGE);
					g_pPickedItem.BackupPickedItem();

					return;
				}
			}

			if (IsEquipable(_PointedSlot, pItemObj))
			{
				if(g_pPickedItem.GetOwnerInventory() == _InventoryControl)
				{
					CL2GS_REQUEST_MOVEITEM RequestMoveItem;

					RequestMoveItem.SrcType = INVENTORY_TYPE::IT_INVENTORY;
					RequestMoveItem.SrcX = pItemObj->x;
					RequestMoveItem.SrcY = pItemObj->y;

					RequestMoveItem.DestType = INVENTORY_TYPE::IT_EQUIPMENT;
					RequestMoveItem.DestX = _PointedSlot;
					RequestMoveItem.DestY = 0;

					g_GameServerObject->Send(&RequestMoveItem);
				}
				else if (g_pPickedItem.GetOwnerInventory() == g_pStorageInventory->GetInventoryControl())
				{
					if(g_pStorageInventory->GetCurrentAction() == UIStorageInventory::WHAUTO_NONE)
					{
						if (g_pStorageInventory->IsStorageLocked() == true &&
							g_pStorageInventory->IsStorageAllowed() == false)
						{
							g_pStorageInventory->SetAutoMoveItem(INVENTORY_TYPE::IT_STORAGE, pItemObj->x, pItemObj->y, INVENTORY_TYPE::IT_EQUIPMENT, _PointedSlot, 0);
							g_pMessageRoot->Show(MESSAGEBOX::INTERFACE_STORAGELOCKBOX);
							g_pStorageLockBox->SetRequestState(CL2GS_REQUEST_STORAGELOCK::REQUEST_ALLOW);
							g_pPickedItem.BackupPickedItem();
						}
						else
						{
							CL2GS_REQUEST_MOVEITEM RequestMoveItem;

							RequestMoveItem.SrcType = INVENTORY_TYPE::IT_STORAGE;
							RequestMoveItem.SrcX = pItemObj->x;
							RequestMoveItem.SrcY = pItemObj->y;

							RequestMoveItem.DestType = INVENTORY_TYPE::IT_EQUIPMENT;
							RequestMoveItem.DestX = _PointedSlot;
							RequestMoveItem.DestY = 0;

							g_GameServerObject->Send(&RequestMoveItem);
						}
					}
				}
				else if (g_pPickedItem.GetOwnerInventory() == g_pMixInventory->GetInventoryControl())
				{
					CL2GS_REQUEST_MOVEITEM RequestMoveItem;

					RequestMoveItem.SrcType = INVENTORY_TYPE::IT_MIX;
					RequestMoveItem.SrcX = pItemObj->x;
					RequestMoveItem.SrcY = pItemObj->y;

					RequestMoveItem.DestType = INVENTORY_TYPE::IT_EQUIPMENT;
					RequestMoveItem.DestX = _PointedSlot;
					RequestMoveItem.DestY = 0;

					g_GameServerObject->Send(&RequestMoveItem);
				}
#if 0 // FIX
				else if (pPickedItem->GetOwnerInventory() == g_pTrade->GetMyInvenCtrl())
				{
					mu_int32 iSourceIndex = pPickedItem->GetSourceLinealPos();
					mu_int32 iTargetIndex = m_iPointedSlot;

					g_pTrade->SendRequestItemToMyInven(
						pItemObj, iSourceIndex, iTargetIndex);

					return true;
				}
				else if (pPickedItem->GetOwnerInventory() == g_pMyShopInventory->GetInventoryCtrl())
				{
					mu_int32 iSourceIndex = pPickedItem->GetSourceLinealPos();
					mu_int32 iTargetIndex = m_iPointedSlot;

					SendRequestEquipmentItem(REQUEST_EQUIPMENT_MYSHOP, MAX_MY_INVENTORY_INDEX + iSourceIndex, pItemObj,
						REQUEST_EQUIPMENT_INVENTORY, iTargetIndex);

					return true;
				}
#endif
				else if (pItemObj->ex_src_type == ITEM_EX_SRC_EQUIPMENT)
				{
					if (g_pPickedItem.GetSourceLinealPos() == _PointedSlot)
					{
						g_pPickedItem.BackupPickedItem();
					}
					else
					{
						CL2GS_REQUEST_MOVEITEM RequestMoveItem;

						RequestMoveItem.SrcType = INVENTORY_TYPE::IT_EQUIPMENT;
						RequestMoveItem.SrcX = pItemObj->lineal_pos;
						RequestMoveItem.SrcY = 0;

						RequestMoveItem.DestType = INVENTORY_TYPE::IT_EQUIPMENT;
						RequestMoveItem.DestX = _PointedSlot;
						RequestMoveItem.DestY = 0;

						g_GameServerObject->Send(&RequestMoveItem);
						return;
					}
				}
			}
		}
		else if (GetRepairMode() == true)
		{
			ITEM* pEquippedItem = &CharacterMachine->Equipment[_PointedSlot];

			if (pEquippedItem != nullptr &&
				IsRepairBan(pEquippedItem) == false)
			{
				if (g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_NPCSHOP) &&
					g_pNpcShopInventory->GetShopState() == UIShopInventory::SHOP_REPAIR)
				{
					g_GameServerObject->SendRequestRepair(INVENTORY_TYPE::IT_EQUIPMENT, _PointedSlot, 0, true);
				}
				else if (CharacterAttribute->Level >= REPAIR_MINLEVEL)
				{
					g_GameServerObject->SendRequestRepair(INVENTORY_TYPE::IT_EQUIPMENT, _PointedSlot, 0, false);
				}
			}
		}
		else
		{
			ITEM* pEquippedItem = &CharacterMachine->Equipment[_PointedSlot];
			if (pEquippedItem->Type >= 0)
			{
				if (GET_WORLD == WD_10HEAVEN)
				{
					ITEM* pEquippedPetItem = &CharacterMachine->Equipment[GAME::EQUIPMENT_HELPER];
					mu_boolean bPicked = true;

					if (_PointedSlot == GAME::EQUIPMENT_HELPER || _PointedSlot == GAME::EQUIPMENT_WING)
					{
						if (_PointedSlot == GAME::EQUIPMENT_HELPER && !IsEquipedWing())
						{
							bPicked = false;
						}
						else if (_PointedSlot == GAME::EQUIPMENT_WING &&
							!(pEquippedPetItem->Type == ITEM_HELPER + 3
								|| pEquippedPetItem->Type == ITEM_HELPER + 4
								|| pEquippedPetItem->Type == ITEM_HELPER + 37))
						{
							bPicked = false;
						}
					}

					if (bPicked == true)
					{
						g_pPickedItem.CreatePickedItem(nullptr, pEquippedItem);
					}
				}
				else
				{
					g_pPickedItem.CreatePickedItem(nullptr, pEquippedItem);
				}
			}
		}
	}
}

UIBase *UIMyInventory::GetFocus(mu_int32 mouseX, mu_int32 mouseY)
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

		object = _CloseButton->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}

		object = _RepairButton->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}

		object = _PShopButton->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}
	}

	return focus;
}

void UIMyInventory::Configure()
{
	SetRepairMode(false);

	mu_uint16 wLevel = CharacterAttribute->Level;

	if (wLevel >= REPAIR_MINLEVEL)
	{
		_RepairButton->ChangeState(UIToggleButton::UI_NORMAL);
	}
	else
	{
		_RepairButton->ChangeState(UIToggleButton::UI_DISABLED);
	}

	if (wLevel >= PSHOP_MINLEVEL &&
		!g_CursedTemple.IsCursedTemple())
	{
		_PShopButton->ChangeState(UIToggleButton::UI_NORMAL);
	}
	else
	{
		_PShopButton->ChangeState(UIToggleButton::UI_DISABLED);
	}

#if 0 // FIX
	if (g_QuestMng.IsIndexInCurQuestIndexList(0x1000F))
	{
		if (g_QuestMng.IsEPRequestRewardState(0x1000F))
		{
			SendSatisfyQuestRequestFromClient(0x1000F);
			g_QuestMng.SetEPRequestRewardState(0x1000F, false);
		}
	}
#endif
}

mu_boolean UIMyInventory::IsEquipable(mu_int32 iIndex, ITEM* pItem)
{
	if (pItem == nullptr)
		return false;

	ITEM_ATTRIBUTE* pItemAttr = &ItemAttribute[pItem->Type];
	mu_boolean bEquipable = false;
	if (pItemAttr->RequireClass[GetBaseClass(Hero->Class)])
		bEquipable = true;
	else if (GetBaseClass(Hero->Class) == GAME::CLASS_DARK && pItemAttr->RequireClass[GAME::CLASS_WIZARD]
		&& pItemAttr->RequireClass[GAME::CLASS_KNIGHT])
		bEquipable = true; 

	mu_uint8 byFirstClass = GetBaseClass(Hero->Class);
	mu_uint8 byStepClass = GetStepClass(Hero->Class);
	if (pItemAttr->RequireClass[byFirstClass] > byStepClass)
	{
		return false;
	}

	if (bEquipable == false)
		return false;

	bEquipable = false;
	if (pItemAttr->m_byItemSlot == iIndex)
		bEquipable = true;
	else if (pItemAttr->m_byItemSlot == GAME::EQUIPMENT_WEAPON_RIGHT && iIndex == GAME::EQUIPMENT_WEAPON_LEFT)
	{
		if (GetBaseClass(Hero->Class) == GAME::CLASS_KNIGHT || GetBaseClass(Hero->Class) == GAME::CLASS_DARK)
		{
			if (!pItemAttr->TwoHand)
				bEquipable = true;
			else
			{
				bEquipable = false;
				return false;
			}
		}
		else if (GetBaseClass(Hero->Class) == GAME::CLASS_SUMMONER &&
			!(pItem->Type >= ITEM_STAFF && pItem->Type <= ITEM_STAFF + MAX_ITEM_INDEX))
			bEquipable = true;
	}
	else if (pItemAttr->m_byItemSlot == GAME::EQUIPMENT_RING_RIGHT && iIndex == GAME::EQUIPMENT_RING_LEFT)
		bEquipable = true;

	if (GetBaseClass(Hero->Class) == GAME::CLASS_ELF)
	{
		ITEM *l = &CharacterMachine->Equipment[GAME::EQUIPMENT_WEAPON_LEFT];
		if (iIndex == GAME::EQUIPMENT_WEAPON_RIGHT && l->Type != ITEM_BOW + 7
			&& (l->Type >= ITEM_BOW && l->Type < ITEM_BOW + MAX_ITEM_INDEX))
		{
			if (pItem->Type != ITEM_BOW + 15)
				bEquipable = false;
		}
	}

	if (bEquipable == false)
		return false;

	mu_uint16 wStrength = CharacterAttribute->Strength + CharacterAttribute->AddStrength;
	mu_uint16 wDexterity = CharacterAttribute->Dexterity + CharacterAttribute->AddDexterity;
	mu_uint16 wEnergy = CharacterAttribute->Energy + CharacterAttribute->AddEnergy;
	mu_uint16 wVitality = CharacterAttribute->Vitality + CharacterAttribute->AddVitality;
	mu_uint16 wCharisma = CharacterAttribute->Charisma + CharacterAttribute->AddCharisma;
	mu_uint16 wLevel = CharacterAttribute->Level;

	mu_int32 iItemLevel = pItem->Level;

	mu_int32 iDecNeedStrength = 0, iDecNeedDex = 0;

	if (iItemLevel >= pItem->HarmonyOptionLevel)
	{
		StrengthenCapability SC;
		g_pUIJewelHarmonyinfo.GetStrengthenCapability(&SC, pItem, 0);

		if (SC.SI_isNB)
		{
			iDecNeedStrength = SC.SI_NB.SI_force;
			iDecNeedDex = SC.SI_NB.SI_activity;
		}
	}

	if (pItem->SocketCount > 0)
	{
		for (mu_int32 i = 0; i < pItem->SocketCount; ++i)
		{
			if (pItem->SocketSeedID[i] == 38)
			{
				mu_int32 iReqStrengthDown = g_SocketItemMgr.GetSocketOptionValue(pItem, i);
				iDecNeedStrength += iReqStrengthDown;
			}
			else if (pItem->SocketSeedID[i] == 39)
			{
				mu_int32 iReqDexterityDown = g_SocketItemMgr.GetSocketOptionValue(pItem, i);
				iDecNeedDex += iReqDexterityDown;
			}
		}
	}

	if (pItem->RequireStrength - iDecNeedStrength > wStrength)
		return false;
	if (pItem->RequireDexterity - iDecNeedDex > wDexterity)
		return false;
	if (pItem->RequireEnergy > wEnergy)
		return false;
	if (pItem->RequireVitality > wVitality)
		return false;
	if (pItem->RequireCharisma > wCharisma)
		return false;
	if (pItem->RequireLevel > wLevel)
		return false;

	if (pItem->Type == ITEM_HELPER + 5)	
	{
		mu_uint16 wRequireCharisma = (185 + (pItem->PetLevel * 15));
		if (wRequireCharisma > wCharisma)
			return false;
	}

	if (GET_WORLD == WD_7ATLANSE && (pItem->Type >= ITEM_HELPER + 2 && pItem->Type <= ITEM_HELPER + 3))
	{
		return false;
	}
	else if (pItem->Type == ITEM_HELPER + 2 && GET_WORLD == WD_10HEAVEN)
	{
		return false;
	}
	else if (pItem->Type == ITEM_HELPER + 2 && g_Direction.m_CKanturu.IsMayaScene())
	{
		return false;
	}
	else if (InChaosCastle() || (M34CryWolf1st::Get_State_Only_Elf()
		&& g_isCharacterBuff((&Hero->Object), eBuff_CrywolfHeroContracted)))
	{
		if ((pItem->Type >= ITEM_HELPER + 2 && pItem->Type <= ITEM_HELPER + 5) || pItem->Type == ITEM_HELPER + 37)
			return false;
	}
	else if (((pItem->Type >= ITEM_HELPER + 2 && pItem->Type <= ITEM_HELPER + 4) || pItem->Type == ITEM_HELPER + 37)
		&& Hero->Object.CurrentAction >= PLAYER_SIT1 && Hero->Object.CurrentAction <= PLAYER_SIT_FEMALE2)
	{
		return false;
	}

	return bEquipable;
}

void UIMyInventory::RenderItemToolTip(mu_int32 iSlotIndex)
{
	if (g_pPickedItem.GetItem() == nullptr &&
		_PointedSlot != -1)
	{
		ITEM* pEquipmentItemSlot = &CharacterMachine->Equipment[iSlotIndex];
		if (pEquipmentItemSlot->Type != -1)
		{
			mu_float iTargetX = _ViewX + EquipmentPos[iSlotIndex].x + EquipmentSize[iSlotIndex].x * 0.5f;
			mu_float iTargetY = _ViewY + EquipmentPos[iSlotIndex].y + EquipmentSize[iSlotIndex].y;

			pEquipmentItemSlot->bySelectedSlotIndex = iSlotIndex;

			if (_RepairMode == false)
			{
				RenderItemInfo(iTargetX, iTargetY, pEquipmentItemSlot, false, true, 0, false, true);
			}
			else
			{
				RenderRepairInfo(iTargetX, iTargetY, pEquipmentItemSlot, false, true);
			}
		}
	}
}

mu_boolean UIMyInventory::GetRepairMode()
{
	return _InventoryControl->IsRepairMode();
}

void UIMyInventory::SetRepairMode(mu_boolean bRepair)
{
	_InventoryControl->SetRepairMode(bRepair);
}

void UIMyInventory::ToggleRepairMode()
{
	SetRepairMode(!GetRepairMode());
}

void UIMyInventory::EquipItem(mu_int32 iIndex, SHARED::ITEM_DATA &Item)
{
	ITEM *pTargetItemSlot = &CharacterMachine->Equipment[iIndex];

	if (pTargetItemSlot->Type > 0)
	{
		UnequipItem(iIndex);
	}

	pTargetItemSlot->lineal_pos = iIndex;
	pTargetItemSlot->ex_src_type = ITEM_EX_SRC_EQUIPMENT;

	ParseItem(Item, pTargetItemSlot);

	CharacterMachine->CalculateAll();

	if (g_PlayItemSoundEnable == true)
	{
		PlayBackground(SOUND_GET_ITEM01);
	}
}

void UIMyInventory::UnequipItem(mu_int32 iIndex)
{
	if (iIndex >= 0 && iIndex < MAX_EQUIPMENT_INDEX && CharacterMachine)
	{
		if (g_pPickedItem.GetItem() != nullptr &&
			g_pPickedItem.GetItem()->ex_src_type == ITEM_EX_SRC_EQUIPMENT &&
			g_pPickedItem.GetItem()->lineal_pos == iIndex)
		{
			g_pPickedItem.BackupPickedItem();
		}

		ITEM* pEquippedItem = &CharacterMachine->Equipment[iIndex];

		if (pEquippedItem->Type != -1)
		{
			pEquippedItem->Type = -1;
			pEquippedItem->Level = 0;
			pEquippedItem->Number = -1;
			pEquippedItem->ExcOption = 0;
			pEquippedItem->SetOption = 0;
			pEquippedItem->SocketCount = 0;

			for (mu_int32 i = 0; i < MAX_SOCKETS; ++i)
			{
				pEquippedItem->SocketSeedID[i] = SOCKET_EMPTY;
				pEquippedItem->SocketSphereLv[i] = 0;
			}

			pEquippedItem->SocketSeedSetOption = 0;

			CharacterMachine->CalculateAll();
		}
	}
}

void UIMyInventory::UnequipAllItems()
{
	if (CharacterMachine)
	{
		if (g_pPickedItem.GetItem() != nullptr &&
			g_pPickedItem.GetItem()->ex_src_type == ITEM_EX_SRC_EQUIPMENT)
		{
			g_pPickedItem.BackupPickedItem();
		}

		for (mu_int32 i = 0; i < MAX_EQUIPMENT_INDEX; ++i)
		{
			UnequipItem(i);
		}
	}
}

mu_boolean UIMyInventory::InsertItem(mu_int32 ColumnX, mu_int32 RowY, SHARED::ITEM_DATA &Item)
{
	ITEM *NewItem = GenerateItem();

	ParseItem(Item, NewItem);

	if (_InventoryControl->AddItem(ColumnX, RowY, NewItem) == true)
		return true;

	DestroyItem(NewItem);

	return false;
}

void UIMyInventory::DeleteItem(mu_int32 ColumnX, mu_int32 RowY)
{
	ITEM* pItem = _InventoryControl->FindItem(ColumnX, RowY);
	if (pItem != nullptr)
	{
		_InventoryControl->RemoveItem(pItem);
	}
}

void UIMyInventory::DeleteAllItems()
{
	_InventoryControl->RemoveAllItems();
}

ITEM *UIMyInventory::FindItem(mu_int32 iLinealPos)
{
	return _InventoryControl->FindItem(iLinealPos);
}

ITEM *UIMyInventory::FindItem(mu_int32 ColumnX, mu_int32 RowY)
{
	return _InventoryControl->FindItem(ColumnX, RowY);
}

ITEM *UIMyInventory::FindItemByKey(mu_uint32 dwKey)
{
	return _InventoryControl->FindItemByKey(dwKey);
}

mu_int32 UIMyInventory::FindItemIndex(mu_int32 Type, mu_int32 Level)
{
	return _InventoryControl->FindItemIndex(Type, Level);
}

mu_int32 UIMyInventory::FindEmptySlot(ITEM* pItem)
{
	ITEM_ATTRIBUTE *pItemAttr = &ItemAttribute[pItem->Type];
	
	return _InventoryControl->FindEmptySlot(pItemAttr->Width, pItemAttr->Height);
}

mu_int32 UIMyInventory::FindEmptySlot(mu_int32 Width, mu_int32 Height)
{
	return _InventoryControl->FindEmptySlot(Width, Height);
}