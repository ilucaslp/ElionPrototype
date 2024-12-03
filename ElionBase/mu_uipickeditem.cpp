#include "stdafx.h"
#include "mu_uipickeditem.h"

UIPickedItem::UIPickedItem()
{
	_Visible = true;

	_SrcInventory = nullptr;
	_PickedItem = nullptr;
}

UIPickedItem::~UIPickedItem()
{
}

void UIPickedItem::Draw()
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[0];

	if (_SlotCount > 0)
	{
		ThreadInfo.EnableScissor = false;
		g_Tasking.RenderGUIModelsAtRuntime(_SlotIndex, _SlotCount);
	}
}

void UIPickedItem::Draw3D()
{
	_SlotCount = 0;

	if (g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_PARTCHARGE_SHOP) == true ||
		_Visible == false)
	{
		return;
	}

	if (_PickedItem != nullptr)
	{
		MUThreadInfo &ThreadInfo = g_ThreadInfo[0];
		++ThreadInfo.GUIRenderIndex;
		ThreadInfo.RenderLight = MU::Float4ToARGB(1.0f, 1.0f, 1.0f, 1.0f);

		_SlotIndex = g_GUIRenderSlotCount;

		ITEM_ATTRIBUTE* itemAttr = &ItemAttribute[_PickedItem->Type];
		mu_float width = itemAttr->Width * INVENTORY_SQUARE_WIDTH;
		mu_float height = itemAttr->Height * INVENTORY_SQUARE_HEIGHT;
		mu_float x = (mu_float)MouseX - width * 0.5f;
		mu_float y = (mu_float)MouseY - height * 0.5f;

		RenderItem3D(x, y, width, height, _PickedItem->Type, _PickedItem->Level, _PickedItem->ExcOption, _PickedItem->SetOption, true);

		_SlotCount = g_GUIRenderSlotCount - _SlotIndex;
	}
}

void UIPickedItem::GetRect(ERect &box)
{
	ITEM_ATTRIBUTE* itemAttr = &ItemAttribute[_PickedItem->Type];
	mu_float width = itemAttr->Width * INVENTORY_SQUARE_WIDTH;
	mu_float height = itemAttr->Height * INVENTORY_SQUARE_HEIGHT;
	mu_float x = (mu_float)MouseX - width * 0.5f;
	mu_float y = (mu_float)MouseY - height * 0.5f;
	box.left = x;
	box.right = x + width;
	box.top = y;
	box.bottom = y + height;
}

mu_int32 UIPickedItem::GetSourceLinealPos()
{
	if (_SrcInventory != nullptr)
	{
		return _PickedItem->y * _SrcInventory->GetNumberOfColumn() + _PickedItem->x;
	}
	else if (_PickedItem != nullptr && _PickedItem->ex_src_type > 0)
	{
		return _PickedItem->lineal_pos;
	}

	return -1;
}

mu_int32 UIPickedItem::GetTargetLinealPos(UIInventoryControl *Dest)
{
	mu_int32 iTargetColumnX, iTargetRowY;

	if (GetTargetPos(Dest, iTargetColumnX, iTargetRowY))
	{
		return iTargetRowY*Dest->GetNumberOfColumn() + iTargetColumnX;
	}

	return -1;
}

mu_boolean UIPickedItem::GetTargetPos(UIInventoryControl *Dest, mu_int32 &TargetColumnX, mu_int32 &TargetRowY)
{
	if (Dest != nullptr)
	{
		ERect rcInventory;
		Dest->GetRect(rcInventory);

		ITEM_ATTRIBUTE* pItemAttr = &ItemAttribute[_PickedItem->Type];
		mu_int32 PickedItemX = MouseX - ((pItemAttr->Width - 1) * INVENTORY_SQUARE_WIDTH / 2);
		mu_int32 PickedItemY = MouseY - ((pItemAttr->Height - 1) * INVENTORY_SQUARE_HEIGHT / 2);

		return Dest->GetSquarePosAtPoint(PickedItemX, PickedItemY, TargetColumnX, TargetRowY);
	}

	return false;
}

mu_boolean UIPickedItem::CreatePickedItem(UIInventoryControl *Src, ITEM *Item)
{
	if (_PickedItem != nullptr)
	{
		return false;
	}

	_SrcInventory = Src;
	_PickedItem = Item;
	Show();

	return true;
}

void UIPickedItem::BackupPickedItem()
{
	if (GetItem() != nullptr)
	{
		DeletePickedItem();
	}
}

void UIPickedItem::DeletePickedItem()
{
	_PickedItem = nullptr;
	_SrcInventory = nullptr;
}