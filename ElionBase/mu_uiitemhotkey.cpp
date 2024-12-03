#include "stdafx.h"
#include "mu_uiitemhotkey.h"

#define SCREEN_WIDTH (640.0f)
#define SCREEN_HEIGHT (480.0f)

#define HOTKEY_WIDTH (20.0f)
#define HOTKEY_HEIGHT (20.0f)
#define HOTKEY_POSX(i) (10.0f + (i * (HOTKEY_WIDTH + 18.0f)))
#define HOTKEY_POSY (443.0f)

UIItemHotkey::UIItemHotkey(UISceneManager *manager) : UIBase()
{
	if (manager != nullptr)
	{
		manager->AddGFxObject(this);
	}

	_SceneManager = manager;

	// Initialize
	{
		for (mu_uint32 n = 0; n < HOTKEY_COUNT; ++n)
		{
			_ItemButton[n] = new UIButton(GAMEUI::MESSAGE_ITEMHOTKEY_BEGIN + n, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, HOTKEY_POSX(n), HOTKEY_POSY, HOTKEY_WIDTH, HOTKEY_HEIGHT);
		}

		mu_memset(_HotkeyItemType, -1, sizeof(_HotkeyItemType));
		mu_zeromem(_HotkeyItemLevel, sizeof(_HotkeyItemLevel));
	}
}

UIItemHotkey::~UIItemHotkey()
{

}

void UIItemHotkey::Draw()
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[0];

	if (_SlotCount > 0)
	{
		ThreadInfo.EnableScissor = false;
		g_Tasking.RenderGUIModelsAtRuntime(_SlotIndex, _SlotCount);
	}
}

void UIItemHotkey::PostDraw()
{
	mu_uint32 beginIndex = TEXTINDEX::BEGIN_HOTKEY_ITEMNUMBER;

	for (mu_int32 i = 0; i < HOTKEY_COUNT; ++i)
	{
		mu_int32 count = GetHotkeyItemCount(i);

		mu_float x = 30.0f + (i * 38.0f);
		mu_float y = 457.0f;
		mu_float width = 20.0f;
		mu_float height = 20.0f;

		mu_unicode textBuffer[256];
		mu_sprintf_s(textBuffer, mu_countof(textBuffer), _T("\uE000%08X\uE000\uE001%08X\uE001%d"), MU::MakeARGB(255, 255, 255, 255), 0, count);
		FONT_NUMBER->DrawStringOutline2D(beginIndex + i,
			MU_TO_WINDOW(x, MU_RESOURCE->_640x480Rate[0]),
			MU_TO_WINDOW(y, MU_RESOURCE->_640x480Rate[1]),
			0.8f,
			ETA_CENTER, ETA_BOTTOM,
			0.0f,
			MU::MakeARGB(0, 0, 0, 255),
			textBuffer);
	}
}

void UIItemHotkey::Draw3D()
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[0];
	++ThreadInfo.GUIRenderIndex;

	_SlotIndex = g_GUIRenderSlotCount;

	for (mu_int32 i = 0; i < HOTKEY_COUNT; ++i)
	{
		mu_int32 Index = GetHotkeyItemIndex(i);

		mu_int32 startIndex;
		mu_int32 endIndex;

		if(Index != -1)
		{
			ITEM* pItem = g_pMyInventory->FindItem(Index);

			if(pItem != nullptr)
			{
				mu_float x = 10.0f + (i * 38.0f);
				mu_float y = 443.0f;
				mu_float width = 20.0f;
				mu_float height = 20.0f;
				ThreadInfo.RenderLight = MU::Float4ToARGB(1.0f, 1.0f, 1.0f, 1.0f);

				RenderItem3D(x, y, width, height, pItem->Type, pItem->Level, pItem->ExcOption, pItem->SetOption, false);
			}
		}
		else
		{
			GetHotkeyInfo(i, startIndex, endIndex);

			mu_float x = 10.0f + (i * 38.0f);
			mu_float y = 443.0f;
			mu_float width = 20.0f;
			mu_float height = 20.0f;
			ThreadInfo.RenderLight = MU::Float4ToARGB(0.5f, 0.5f, 0.5f, 0.4f);

			RenderItem3D(x, y, width, height, endIndex, _HotkeyItemLevel[i], 0, 0, false);
		}
	}

	_SlotCount = g_GUIRenderSlotCount - _SlotIndex;
}

UIBase *UIItemHotkey::GetFocus(mu_int32 mouseX, mu_int32 mouseY)
{
	for (mu_uint32 n = 0; n < HOTKEY_COUNT; ++n)
	{
		UIBase *object = _ItemButton[n]->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}
	}

	return nullptr;
}

void UIItemHotkey::GetHotkeyInfo(mu_int32 Type, mu_int32 &iStartItemType, mu_int32 &iEndItemType)
{
	switch (Type)
	{
	case HOTKEY_Q:
		if (GetHotkeyCommonItem(Type, iStartItemType, iEndItemType) == false)
		{
			if (_HotkeyItemType[Type] >= ITEM_POTION + 4 && _HotkeyItemType[Type] <= ITEM_POTION + 6)
			{
				iStartItemType = ITEM_POTION + 6; iEndItemType = ITEM_POTION + 4;
			}
			else
			{
				iStartItemType = ITEM_POTION + 3; iEndItemType = ITEM_POTION + 0;
			}
		}
		break;
	case HOTKEY_W:
		if (GetHotkeyCommonItem(Type, iStartItemType, iEndItemType) == false)
		{
			if (_HotkeyItemType[Type] >= ITEM_POTION + 0 && _HotkeyItemType[Type] <= ITEM_POTION + 3)
			{
				iStartItemType = ITEM_POTION + 3; iEndItemType = ITEM_POTION + 0;
			}
			else
			{
				iStartItemType = ITEM_POTION + 6; iEndItemType = ITEM_POTION + 4;
			}
		}
		break;
	case HOTKEY_E:
		if (GetHotkeyCommonItem(Type, iStartItemType, iEndItemType) == false)
		{
			if (_HotkeyItemType[Type] >= ITEM_POTION + 0 && _HotkeyItemType[Type] <= ITEM_POTION + 3)
			{
				iStartItemType = ITEM_POTION + 3; iEndItemType = ITEM_POTION + 0;
			}
			else if (_HotkeyItemType[Type] >= ITEM_POTION + 4 && _HotkeyItemType[Type] <= ITEM_POTION + 6)
			{
				iStartItemType = ITEM_POTION + 6; iEndItemType = ITEM_POTION + 4;
			}
			else
			{
				iStartItemType = ITEM_POTION + 8; iEndItemType = ITEM_POTION + 8;
			}
		}
		break;
	case HOTKEY_R:
		if (GetHotkeyCommonItem(Type, iStartItemType, iEndItemType) == false)
		{
			if (_HotkeyItemType[Type] >= ITEM_POTION + 0 && _HotkeyItemType[Type] <= ITEM_POTION + 3)
			{
				iStartItemType = ITEM_POTION + 3; iEndItemType = ITEM_POTION + 0;
			}
			else if (_HotkeyItemType[Type] >= ITEM_POTION + 4 && _HotkeyItemType[Type] <= ITEM_POTION + 6)
			{
				iStartItemType = ITEM_POTION + 6; iEndItemType = ITEM_POTION + 4;
			}
			else
			{
				iStartItemType = ITEM_POTION + 37; iEndItemType = ITEM_POTION + 35;
			}
		}
		break;
	}
}

mu_int32 UIItemHotkey::GetHotkeyItemIndex(mu_int32 Type)
{
	mu_int32 iStartItemType = 0, iEndItemType = 0;
	mu_int32 i;

	GetHotkeyInfo(Type, iStartItemType, iEndItemType);

	ITEM *pItem = nullptr;

	UIInventoryControl *inventoryControl = g_pMyInventory->GetInventoryControl();

	for (i = iStartItemType; i >= iEndItemType; --i)
	{
		mu_int32 iIndex = -1;

		if (i >= ITEM_POTION + 0 && i <= ITEM_POTION + 3)
		{
			iIndex = inventoryControl->FindItemReverseIndex(i);
		}
		else
		{
			iIndex = inventoryControl->FindItemReverseIndex(i, _HotkeyItemLevel[Type]);
		}

		if (iIndex != -1)
		{
			pItem = g_pMyInventory->FindItem(iIndex);
			if ((pItem->Type != ITEM_POTION + 7 &&
				pItem->Type != ITEM_POTION + 10 &&
				pItem->Type != ITEM_POTION + 20)
				|| pItem->Level == _HotkeyItemLevel[Type])
			{
				return iIndex;
			}
		}
	}

	return -1;
}

mu_int32 UIItemHotkey::GetHotkeyItemCount(mu_int32 Type)
{
	mu_int32 iStartItemType = 0, iEndItemType = 0;

	GetHotkeyInfo(Type, iStartItemType, iEndItemType);

	mu_int32 iItemCount = 0;
	UIInventoryControl::item_map &items = g_pMyInventory->GetInventoryControl()->GetItems();

	for (auto it = items.begin(); it != items.end(); ++it)
	{
		ITEM *Item = it->second;

		if (Item->Type >= iEndItemType &&
			Item->Type <= iStartItemType &&
			(Item->Level == _HotkeyItemLevel[Type] ||
			(Item->Type >= ITEM_POTION + 0 &&
				Item->Type <= ITEM_POTION + 3)))
		{
			if (Item->Type == ITEM_POTION + 9
				|| Item->Type == ITEM_POTION + 10
				|| Item->Type == ITEM_POTION + 20)
			{
				++iItemCount;
			}
			else
			{
				iItemCount += Item->Durability;
			}
		}
	}

	return iItemCount;
}

mu_boolean UIItemHotkey::GetHotkeyCommonItem(mu_int32 Hotkey, mu_int32 &Start, mu_int32 &End)
{
	switch (_HotkeyItemType[Hotkey])
	{
	case ITEM_POTION + 7:
	case ITEM_POTION + 8:
	case ITEM_POTION + 9:
	case ITEM_POTION + 10:
	case ITEM_POTION + 20:
	case ITEM_POTION + 46:
	case ITEM_POTION + 47:
	case ITEM_POTION + 48:
	case ITEM_POTION + 49:
	case ITEM_POTION + 50:
	case ITEM_POTION + 70:
	case ITEM_POTION + 71:
	case ITEM_POTION + 78:
	case ITEM_POTION + 79:
	case ITEM_POTION + 80:
	case ITEM_POTION + 81:
	case ITEM_POTION + 82:
	case ITEM_POTION + 94:
	case ITEM_POTION + 85:
	case ITEM_POTION + 86:
	case ITEM_POTION + 87:
	case ITEM_POTION + 133:
		if (_HotkeyItemType[Hotkey] != ITEM_POTION + 20 || _HotkeyItemLevel[Hotkey] == 0)
		{
			Start = End = _HotkeyItemType[Hotkey];
			return true;
		}
		break;
	default:
		if (_HotkeyItemType[Hotkey] >= ITEM_POTION + 35 && _HotkeyItemType[Hotkey] <= ITEM_POTION + 37)
		{
			Start = ITEM_POTION + 37; End = ITEM_POTION + 35;
			return true;
		}
		else if (_HotkeyItemType[Hotkey] >= ITEM_POTION + 38 && _HotkeyItemType[Hotkey] <= ITEM_POTION + 40)
		{
			Start = ITEM_POTION + 40; End = ITEM_POTION + 38;
			return true;
		}
		break;
	}

	return false;
}

mu_boolean UIItemHotkey::CanRegisterItemHotKey(mu_int32 Type)
{
	switch (Type)
	{
	case ITEM_POTION + 0:
	case ITEM_POTION + 1:
	case ITEM_POTION + 2:
	case ITEM_POTION + 3:
	case ITEM_POTION + 4:
	case ITEM_POTION + 5:
	case ITEM_POTION + 6:
	case ITEM_POTION + 7:
	case ITEM_POTION + 8:
	case ITEM_POTION + 9:
	case ITEM_POTION + 10:
	case ITEM_POTION + 20:
	case ITEM_POTION + 35:
	case ITEM_POTION + 36:
	case ITEM_POTION + 37:
	case ITEM_POTION + 38:
	case ITEM_POTION + 39:
	case ITEM_POTION + 40:
	case ITEM_POTION + 46:
	case ITEM_POTION + 47:
	case ITEM_POTION + 48:
	case ITEM_POTION + 49:
	case ITEM_POTION + 50:
	case ITEM_POTION + 70:
	case ITEM_POTION + 71:
	case ITEM_POTION + 78:
	case ITEM_POTION + 79:
	case ITEM_POTION + 80:
	case ITEM_POTION + 81:
	case ITEM_POTION + 82:
	case ITEM_POTION + 94:
	case ITEM_POTION + 85:
	case ITEM_POTION + 86:
	case ITEM_POTION + 87:
	case ITEM_POTION + 133:
		return true;
	}

	return false;
}

void UIItemHotkey::SetItemHotKey(mu_int32 hotKey, mu_int32 type, mu_int32 level)
{
	if (hotKey < HOTKEY_Q ||
		hotKey >= HOTKEY_COUNT)
		return;

	if (CanRegisterItemHotKey(type) == true)
	{
		_HotkeyItemType[hotKey] = type;
		_HotkeyItemLevel[hotKey] = level;
	}
	else
	{
		_HotkeyItemType[hotKey] = -1;
		_HotkeyItemLevel[hotKey] = 0;
	}
}