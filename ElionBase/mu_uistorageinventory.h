#ifndef __MU_UISTORAGEINVENTORY_H__
#define __MU_UISTORAGEINVENTORY_H__

#pragma once

#define STORAGE_POSX 260.0f
#define STORAGE_POSY 0.0f
#define STORAGE_WIDTH 190.0f
#define STORAGE_HEIGHT 429.0f

class UIStorageInventory : public UIBase
{
public:
	enum
	{
		WHAUTO_NONE,
		WHAUTO_DEPOSIT,
		WHAUTO_WITHDRAW,
		WHAUTO_MOVEITEM,
	};

public:
	UIStorageInventory(UISceneManager *manager);
	~UIStorageInventory();

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

	void SetStorageLock(mu_boolean lock);
	void SetStorageAllowed(mu_boolean allowed);

	mu_boolean InsertItem(mu_int32 ColumnX, mu_int32 RowY, SHARED::ITEM_DATA &Item);
	void DeleteItem(mu_int32 ColumnX, mu_int32 RowY);
	void DeleteAllItems();

	ITEM *FindItem(mu_int32 iLinealPos);
	ITEM *FindItem(mu_int32 ColumnX, mu_int32 RowY);
	ITEM *FindItemByKey(mu_uint32 dwKey);

	mu_int32 FindEmptySlot(ITEM* pItem);

	void ProcessAutoAction();

public:
	EINLINE UIInventoryControl *GetInventoryControl()
	{
		return _InventoryControl;
	}

	EINLINE mu_boolean IsStorageLocked()
	{
		return _IsLocked;
	}

	EINLINE mu_boolean IsStorageAllowed()
	{
		return _IsAllowed;
	}

	EINLINE void ClearAuto()
	{
		_CurrentAction = WHAUTO_NONE;
	}

	EINLINE void SetAutoDeposit(mu_int64 gold)
	{
		_CurrentAction = WHAUTO_DEPOSIT;
		_GoldAction = gold;
	}

	EINLINE void SetAutoWithdraw(mu_int64 gold)
	{
		_CurrentAction = WHAUTO_WITHDRAW;
		_GoldAction = gold;
	}

	EINLINE void SetAutoMoveItem(mu_uint8 SrcType, mu_uint8 SrcX, mu_uint8 SrcY, mu_uint8 DestType, mu_uint8 DestX, mu_uint8 DestY)
	{
		_CurrentAction = WHAUTO_MOVEITEM;
		_SrcType = SrcType;
		_SrcX = SrcX;
		_SrcY = SrcY;
		_DestType = DestType;
		_DestX = DestX;
		_DestY = DestY;
	}

	EINLINE mu_uint8 GetCurrentAction()
	{
		return _CurrentAction;
	}

private:
	friend class UIGameRoot;
	friend class UIMessageRoot;
	UISprite *_BackFrame;
	UISprite *_TopFrame;
	UISprite *_BottomFrame;
	UISprite *_LeftFrame;
	UISprite *_RightFrame;

	UIInventoryControl *_InventoryControl;

	UISprite *_MoneyIcon;

	UIButton *_XButton;
	UIButton *_DepositButton;
	UIButton *_WithdrawButton;
	UIButton *_LockButton;

	mu_boolean _IsLocked;
	mu_boolean _IsAllowed;

	mu_uint8 _CurrentAction;
	mu_int64 _GoldAction;
	mu_uint8 _SrcType, _SrcX, _SrcY;
	mu_uint8 _DestType, _DestX, _DestY;
};

#endif