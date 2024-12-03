#ifndef __MU_UIINVENTORYCONTROL_H__
#define __MU_UIINVENTORYCONTROL_H__

#pragma once

enum
{
	INVENTORY_SQUARE_WIDTH = 20,
	INVENTORY_SQUARE_HEIGHT = 20,
};

class UIInventoryControl : public UIBase
{
public:
	enum TOOLTIP_TYPE
	{
		UNKNOWN_TOOLTIP_TYPE = 0,
		TOOLTIP_TYPE_INVENTORY,
		TOOLTIP_TYPE_REPAIR,
		TOOLTIP_TYPE_NPC_SHOP,
		TOOLTIP_TYPE_MY_SHOP,
		TOOLTIP_TYPE_PURCHASE_SHOP,
	};

	enum SQUARE_COLOR_STATE
	{
		UNKNOWN_COLOR_STATE = 0,
		COLOR_STATE_NORMAL,
		COLOR_STATE_WARNING,
	};

	enum
	{
		ITEM_SQUARE_WIDTH = 20,
		ITEM_SQUARE_HEIGHT = 20,
		WND_TOP_EDGE = 3,
		WND_LEFT_EDGE = 4,
		WND_BOTTOM_EDGE = 8,
		WND_RIGHT_EDGE = 9,

		RENDER_NUMBER_OF_ITEM = 1,
		RENDER_ITEM_TOOLTIP = 2,
	};

	typedef std::map<mu_uint32, ITEM*> item_map;

public:
	UIInventoryControl(UISceneManager *manager, UIBase *owner, mu_uint32 beginNumberIndex, mu_float aspectWidth, mu_float aspectHeight, mu_float x, mu_float y, mu_uint32 column, mu_uint32 row, mu_uint8 inventoryType);
	~UIInventoryControl();

	void SetPosition(mu_float x, mu_float y);

	virtual void Update(mu_uint64 CurrentTime) override;
	virtual void Draw() override;
	virtual void Draw3D() override;
	virtual void PostDraw() override;

	void RenderNumberOfItem();
	void RenderItemToolTip();

	virtual void OnMouseLeave() override;
	virtual void OnMouseMove(mu_int32 mouse_x, mu_int32 mouse_y) override;
	virtual void OnMousePressed(mu_uint32 mouse_flags, mu_uint32 double_flags, mu_uint32 virtual_keys) override;

	void GetRect(ERect &box);
	mu_boolean GetSquarePosAtPoint(mu_int32 x, mu_int32 y, mu_int32 &ColumnX, mu_int32 &RowY);
	mu_int32 GetSquareIndexAtPoint(mu_int32 x, mu_int32 y);
	mu_boolean CheckSlot(mu_int32 iLinealPos, mu_int32 cx, mu_int32 cy);
	mu_boolean CheckSlot(mu_int32 iColumnX, mu_int32 iRowY, mu_int32 cx, mu_int32 cy);
	mu_boolean CheckSlotIgnore(mu_int32 iLinealPos, mu_int32 cx, mu_int32 cy, mu_uint32 IgnoreKey);
	mu_boolean CheckPointInRect(mu_int32 x, mu_int32 y);
	mu_boolean CheckRectInRect(const ERect& rcBox);
	mu_boolean CanMove(mu_int32 iLinealPos, ITEM* pItem);
	mu_boolean CanMove(mu_int32 iColumnX, mu_int32 iRowY, ITEM* pItem);
	mu_boolean CanMove(mu_int32 iLinealPos, ITEM* pItem, mu_uint32 IgnoreKey);
	mu_boolean CanMoveToPt(mu_int32 x, mu_int32 y, ITEM* pItem);

	mu_boolean AddItem(mu_int32 ColumnX, mu_int32 RowY, ITEM *Item);

	ITEM *FindItemByKey(mu_uint32 Key);
	ITEM *FindItem(mu_int32 LinealPos);
	ITEM *FindItem(mu_int32 Column, mu_int32 Row);

	mu_int32 FindItemIndex(mu_int32 Type, mu_int32 Level = -1);
	mu_int32 FindItemReverseIndex(mu_int32 Type, mu_int32 Level = -1);
	mu_int32 FindEmptySlot(mu_int32 width, mu_int32 height);

	void RemoveItemByKey(mu_uint32 Key);
	void RemoveItem(ITEM *pItem);
	void RemoveAllItems();

	void SetItemColorState(ITEM *pItem);
	mu_boolean CanChangeItemColorState(ITEM *pItem);

	void SetRepairMode(mu_boolean bRepair);
	mu_boolean IsRepairMode();

	void SetToolTipType(TOOLTIP_TYPE ToolTipType);

	mu_boolean IsOverlayItem(ITEM *pSourceItem, ITEM *pTargetItem);
	mu_boolean CanUpgradeItem(ITEM *pSourceItem, ITEM *pTargetItem);

public:
	EINLINE mu_uint8 GetInventoryType()
	{
		return _InventoryType;
	}

	EINLINE void SetSquareColorNormal(mu_uint8 R, mu_uint8 G, mu_uint8 B, mu_uint8 A = 102)
	{
		_NormalColor = MU::MakeARGB(R, G, B, A);
	}

	EINLINE mu_uint32 GetSquareColorNormal()
	{
		return _NormalColor;
	}

	EINLINE void SetSquareColorWarning(mu_uint8 R, mu_uint8 G, mu_uint8 B, mu_uint8 A = 102)
	{
		_WarningColor = MU::MakeARGB(R, G, B, A);
	}

	EINLINE mu_uint32 GetSquareColorWarning()
	{
		return _WarningColor;
	}

	EINLINE mu_uint32 GetNumberOfColumn()
	{
		return _Column;
	}

	EINLINE mu_uint32 GetNumberOfRow()
	{
		return _Row;
	}

	EINLINE mu_size GetNumberOfItems()
	{
		return _Items.size();
	}

	EINLINE item_map &GetItems()
	{
		return _Items;
	}

	EINLINE mu_int32 GetPointedSquareIndex()
	{
		return _PointedSquareIndex;
	}

	EINLINE mu_boolean IsLocked()
	{
		return _Locked;
	}

	EINLINE void Lock()
	{
		_Locked = true;
	}

	EINLINE void Unlock()
	{
		_Locked = false;
	}

private:
	mu_uint32 _SlotIndex;
	mu_uint32 _SlotCount;

	mu_uint8 _InventoryType;

	mu_boolean _CanPush;
	mu_boolean _RepairMode;
	mu_boolean _Locked;
	mu_int32 _PointedSquareIndex;
	mu_uint32 _BeginNumberIndex;
	TOOLTIP_TYPE _ToolTipType;

	mu_uint32 _Column;
	mu_uint32 _Row;
	mu_uint32 *_ItemCheckBox;
	item_map _Items;

	mu_uint32 _NormalColor;
	mu_uint32 _WarningColor;

	std::vector<UISpriteColor*> _UseboxColor;
	std::vector<UISprite*> _UseboxImage;
	std::vector<UISpriteColor*> _PickedColor;
	UISpriteColor *_PickedWarningColor;

	UISprite *_TopLeft;
	UISprite *_TopRight;
	UISprite *_BottomLeft;
	UISprite *_BottomRight;
	UISprite *_TopLine;
	UISprite *_BottomLine;
	UISprite *_LeftLine;
	UISprite *_RightLine;
};

#endif