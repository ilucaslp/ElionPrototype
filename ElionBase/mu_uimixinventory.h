#ifndef __MU_UIMIXINVENTORY_H__
#define __MU_UIMIXINVENTORY_H__

#pragma once

extern mu_int32 g_ChaosTaxRate;

#define MIXINVENTORY_POSX 260.0f
#define MIXINVENTORY_POSY 0.0f
#define MIXINVENTORY_WIDTH 190.0f
#define MIXINVENTORY_HEIGHT 429.0f

class UIMixInventory : public UIBase
{
public:
	enum
	{
		MIX_WAITING,
		MIX_PROCESSING,
		MIX_FINISHED,
	};
public:
	UIMixInventory(UISceneManager *manager);
	~UIMixInventory();

	void Configure();
	void UpdateMix();

	virtual void Update(mu_uint64 CurrentTime) override;
	virtual void Draw() override;
	virtual void Draw3D() override;
	virtual void PostDraw() override;

	void DrawFrames();
	void DrawInventoryDetails();
	void DrawButtons();
	void PostDrawButtons();

	virtual UIBase *GetFocus(mu_int32 mouseX, mu_int32 mouseY) override;

	mu_boolean InsertItem(mu_int32 ColumnX, mu_int32 RowY, SHARED::ITEM_DATA &Item);
	void DeleteItem(mu_int32 ColumnX, mu_int32 RowY);
	void DeleteAllItems();

	ITEM *FindItem(mu_int32 iLinealPos);
	ITEM *FindItem(mu_int32 ColumnX, mu_int32 RowY);
	ITEM *FindItemByKey(mu_uint32 dwKey);

	mu_int32 FindEmptySlot(ITEM* pItem);

	void SetMixState(mu_uint8 mixState);

public:
	EINLINE UIInventoryControl *GetInventoryControl()
	{
		return _InventoryControl;
	}

	EINLINE mu_uint8 GetMixState()
	{
		return _MixState;
	}

private:
	UISprite *_BackFrame;
	UISprite *_TopFrame;
	UISprite *_BottomFrame;
	UISprite *_LeftFrame;
	UISprite *_RightFrame;

	UISprite *_ShineEffect[ROW_MIX_INVENTORY * COLUMN_MIX_INVENTORY][4];

	UIInventoryControl *_InventoryControl;

	UIButton *_XButton;
	UIButton *_MixButton;

	mu_uint8 _MixState;
	mu_uint64 _MixEffectTimer;
};

EINLINE mu_int64 MixRequireZen(mu_int64 MixZen)
{
	if (g_ChaosTaxRate > 0)
	{
		MixZen += (MixZen * g_ChaosTaxRate) / 100;
	}

	return MixZen;
}

#endif