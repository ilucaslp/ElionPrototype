#ifndef __MU_UIITEMHOTKEY_H__
#define __MU_UIITEMHOTKEY_H__

#pragma once

class UIItemHotkey : public UIBase
{
public:
	enum
	{
		HOTKEY_Q = 0,
		HOTKEY_W,
		HOTKEY_E,
		HOTKEY_R,
		HOTKEY_COUNT
	};

public:
	UIItemHotkey(UISceneManager *manager);
	~UIItemHotkey();

	virtual void Draw() override;
	virtual void PostDraw() override;
	virtual void Draw3D() override;

	virtual UIBase *GetFocus(mu_int32 mouseX, mu_int32 mouseY) override;

	void GetHotkeyInfo(mu_int32 Type, mu_int32 &iStartItemType, mu_int32 &iEndItemType);
	mu_int32 GetHotkeyItemIndex(mu_int32 Type);
	mu_int32 GetHotkeyItemCount(mu_int32 Type);
	mu_boolean GetHotkeyCommonItem(mu_int32 Hotkey, mu_int32 &Start, mu_int32 &End);

	mu_boolean CanRegisterItemHotKey(mu_int32 Type);

	void SetItemHotKey(mu_int32 hotKey, mu_int32 type, mu_int32 level);

private:
	UIButton *_ItemButton[HOTKEY_COUNT];

private:
	mu_uint32 _SlotIndex;
	mu_uint32 _SlotCount;

	mu_int32 _HotkeyItemType[HOTKEY_COUNT];
	mu_int32 _HotkeyItemLevel[HOTKEY_COUNT];
};

#endif