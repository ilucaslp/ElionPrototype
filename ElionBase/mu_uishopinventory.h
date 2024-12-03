#ifndef __MU_SHOPINVENTORY_H__
#define __MU_SHOPINVENTORY_H__

#pragma once

#define SHOP_POSX 260.0f
#define SHOP_POSY 0.0f
#define SHOP_WIDTH 190.0f
#define SHOP_HEIGHT 429.0f

class UIShopInventory : public UIBase
{
public:
	enum
	{
		SHOP_BUYSELL,
		SHOP_REPAIR,
	};

	enum
	{
		SHOPTYPE_NORMAL,
		SHOPTYPE_GAMBLE,
		SHOPTYPE_PCPOINT,
	};
public:
	UIShopInventory(UISceneManager *manager);
	~UIShopInventory();

	virtual void Update(mu_uint64 CurrentTime) override;
	virtual void Draw() override;
	virtual void Draw3D() override;
	virtual void PostDraw() override;

	virtual mu_uint32 OnPreKeyPressed(const SDL_Keysym &key) override;

	virtual UIBase *GetFocus(mu_int32 mouseX, mu_int32 mouseY) override;

	void DrawFrames();
	void DrawDetails();
	void DrawButtons();
	void PostDrawButtons();

	mu_boolean InsertItem(mu_int32 ColumnX, mu_int32 RowY, SHARED::ITEM_DATA &Item);
	void DeleteItem(mu_int32 ColumnX, mu_int32 RowY);
	void DeleteAllItems();

	ITEM *FindItem(mu_int32 iLinealPos);
	ITEM *FindItem(mu_int32 ColumnX, mu_int32 RowY);
	ITEM *FindItemByKey(mu_uint32 dwKey);

	mu_int32 FindEmptySlot(ITEM* pItem);

	void ToggleState();

public:
	EINLINE UIInventoryControl *GetInventoryControl()
	{
		return _InventoryControl;
	}

	EINLINE mu_int32 GetTaxRate()
	{
		return _TaxRate;
	}

	EINLINE void SetTaxRate(mu_int32 taxRate)
	{
		_TaxRate = taxRate;
	}

	EINLINE mu_boolean GetRepairShop()
	{
		return _RepairShop;
	}

	EINLINE void SetRepairShop(mu_boolean isRepairEnabled)
	{
		_RepairShop = isRepairEnabled;
	}

	EINLINE mu_uint8 GetShopType()
	{
		return _ShopType;
	}

	EINLINE void SetShopType(mu_uint8 shopType)
	{
		_ShopType = shopType;
	}

	EINLINE mu_uint8 GetShopState()
	{
		return _ShopState;
	}

	EINLINE void SetShopState(mu_uint8 state)
	{
		_ShopState = state;
	}

private:
	friend class UIGameRoot;
	friend class UIMessageRoot;
	friend class UIInventoryControl;
	UISprite *_BackFrame;
	UISprite *_TopFrame;
	UISprite *_BottomFrame;
	UISprite *_LeftFrame;
	UISprite *_RightFrame;

	UIInventoryControl *_InventoryControl;

	UISprite *_MoneyIcon;

	UIButton *_XButton;
	UIButton *_RepairButton;
	UIButton *_RepairAllButton;

	mu_int32 _TaxRate;
	mu_boolean _RepairShop;
	mu_uint8 _ShopType;
	mu_uint8 _ShopState;
	mu_int64 _AllRepairGold;
};

#endif