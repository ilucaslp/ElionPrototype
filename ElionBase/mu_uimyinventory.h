#ifndef __MU_UIMYINVENTORY_H__
#define __MU_UIMYINVENTORY_H__

#pragma once

#define MYINVENTORY_POSX 450.0f
#define MYINVENTORY_POSY 0.0f
#define MYINVENTORY_WIDTH 190.0f
#define MYINVENTORY_HEIGHT 429.0f

enum TOOLTIP_TYPE
{
	UNKNOWN_TOOLTIP_TYPE = 0,
	TOOLTIP_TYPE_INVENTORY,
	TOOLTIP_TYPE_REPAIR,
	TOOLTIP_TYPE_NPC_SHOP,
	TOOLTIP_TYPE_MY_SHOP,
	TOOLTIP_TYPE_PURCHASE_SHOP,
};

class UIMyInventory : public UIBase
{
public:
	UIMyInventory(UISceneManager *manager);
	~UIMyInventory();

	void ChangePosition(mu_float x, mu_float y);

	virtual void Update(mu_uint64 CurrentTime) override;
	virtual void Draw() override;
	virtual void Draw3D() override;
	virtual void PostDraw() override;

	void DrawFrames();
	void DrawInventoryDetails();
	void DrawSetOption();
	void DrawSocketOption();
	void DrawButtons();
	void PostDrawButtons();
	void DrawEquippedItem();

	void PostDrawSetOption();
	void PostDrawSocketOption();
	void PostDrawEquippedItem();

	virtual void OnMouseLeave() override;
	virtual void OnMouseMove(mu_int32 mouse_x, mu_int32 mouse_y) override;
	virtual void OnMousePressed(mu_uint32 mouse_flags, mu_uint32 double_flags, mu_uint32 virtual_keys) override;

	virtual UIBase *GetFocus(mu_int32 mouseX, mu_int32 mouseY) override;

	void Configure();

	mu_boolean IsEquipable(mu_int32 iIndex, ITEM *pItem);

	void RenderItemToolTip(mu_int32 iSlotIndex);

	mu_boolean GetRepairMode();
	void SetRepairMode(mu_boolean bRepair);
	void ToggleRepairMode();

	void EquipItem(mu_int32 iIndex, SHARED::ITEM_DATA &Item);
	void UnequipItem(mu_int32 iIndex);
	void UnequipAllItems();

	mu_boolean InsertItem(mu_int32 ColumnX, mu_int32 RowY, SHARED::ITEM_DATA &Item);
	void DeleteItem(mu_int32 ColumnX, mu_int32 RowY);
	void DeleteAllItems();

	ITEM *FindItem(mu_int32 iLinealPos);
	ITEM *FindItem(mu_int32 ColumnX, mu_int32 RowY);
	ITEM *FindItemByKey(mu_uint32 dwKey);
	mu_int32 FindItemIndex(mu_int32 Type, mu_int32 Level);

	mu_int32 FindEmptySlot(ITEM* pItem);
	mu_int32 FindEmptySlot(mu_int32 Width, mu_int32 Height);

public:
	EINLINE UIInventoryControl *GetInventoryControl()
	{
		return _InventoryControl;
	}

private:
	mu_int32 _PointedSlot;
	mu_uint32 _SlotIndex;
	mu_uint32 _SlotCount;

	mu_boolean _RepairMode;
	
	UISprite *_BackFrame;
	UISprite *_TopFrame;
	UISprite *_BottomFrame;
	UISprite *_LeftFrame;
	UISprite *_RightFrame;

	UISprite *_EquipmentFrame[GAME::MAX_EQUIPMENT];
	UISpriteColor *_EquipmentColor[GAME::MAX_EQUIPMENT];
	UISpriteColor *_EquipmentPointed[GAME::MAX_EQUIPMENT];

	UIInventoryControl *_InventoryControl;

	UISprite *_MoneyIcon;

	UIButton *_XButton;
	UIButton *_CloseButton;
	UIButton *_RepairButton;
	UIButton *_PShopButton;
};

#endif