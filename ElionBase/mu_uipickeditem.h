#ifndef __MU_UIPICKEDITEM_H__
#define __MU_UIPICKEDITEM_H__

#pragma once

class UIInventoryControl;

class UIPickedItem
{
public:
	UIPickedItem();
	~UIPickedItem();

	void Draw();
	void Draw3D();

	void GetRect(ERect &box);

	mu_int32 GetSourceLinealPos();
	mu_int32 GetTargetLinealPos(UIInventoryControl *Dest);
	mu_boolean GetTargetPos(UIInventoryControl *Dest, mu_int32 &TargetColumnX, mu_int32 &TargetRowY);

	mu_boolean CreatePickedItem(UIInventoryControl *Src, ITEM *Item);
	void BackupPickedItem();
	void DeletePickedItem();

public:
	EINLINE UIInventoryControl *GetOwnerInventory()
	{
		return _SrcInventory;
	}

	EINLINE ITEM *GetItem()
	{
		return _PickedItem;
	}

	EINLINE mu_boolean IsVisible()
	{
		return _Visible;
	}

	EINLINE void Show()
	{
		_Visible = true;
	}

	EINLINE void Hide()
	{
		_Visible = false;
	}

private:
	mu_boolean _Visible;

	UIInventoryControl *_SrcInventory;
	ITEM *_PickedItem;

	mu_uint32 _SlotIndex;
	mu_uint32 _SlotCount;
};

extern UIPickedItem g_pPickedItem;

#endif