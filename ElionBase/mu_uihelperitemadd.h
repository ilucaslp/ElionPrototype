#ifndef __MU_UIHELPERITEMADD_H__
#define __MU_UIHELPERITEMADD_H__

#pragma once

#define MAX_ITEMADD_SHOWTYPE (8)
#define MAX_ITEMADD_SHOWITEM (8)

class UIHelperItemAdd : public UIBase
{
public:
	UIHelperItemAdd(UISceneManager *manager);
	~UIHelperItemAdd();

	void InitializeItemsData();

	void ConfigureTypeScroll();
	void ConfigureItemScroll();
	void CheckTypeList();
	void ConfigureTypeList();
	void ConfigureItemList();
	void ConfigureItemInfo();

	virtual mu_uint32 OnPreWindowMessage(const SDL_Event *event, mu_size *result) override;
	virtual mu_uint32 OnWindowMessage(const SDL_Event *event, mu_size *result) override;

	virtual mu_boolean OnGFxMessage(mu_uint32 objectid, mu_uint32 message, mu_size message_data) override;

	virtual void Update(mu_uint64 CurrentTime) override;
	virtual void Draw3D() override;
	virtual void PostDraw() override;

	void DrawFrame();
	void DrawText();
	void DrawButton();

	virtual mu_uint32 OnPreKeyPressed(const SDL_Keysym &key) override;

	virtual UIBase *GetFocus(mu_int32 mouseX, mu_int32 mouseY) override;

	void Configure(mu_int32 helperIndex, HELPER_ITEMSETTING *itemSetting);

public:
	EINLINE void DecreaseLevel()
	{
		if (_MinItemLevel > 0)
		{
			--_MinItemLevel;
		}
	}

	EINLINE void IncreaseLevel()
	{
		if (_MinItemLevel < MAX_ITEM_LEVEL)
		{
			++_MinItemLevel;
		}
	}

private:
	friend class UIMessageRoot;
	TMSGBOX_VARS;
	TGLAYOUT_VARS(MainBack);

	UISprite *_MainDiv;
	UISprite *_RenderDiv;

	TGLAYOUT_VARS(TypeBack);
	UIScrollBar *_TypeScrollBar;
	TGLAYOUT_VARS(ItemBack);
	UIScrollBar *_ItemScrollBar;

	UIRadioButton *_TypesListBtn[MAX_ITEMADD_SHOWTYPE];
	UIRadioButton *_ItemsListBtn[MAX_ITEMADD_SHOWITEM];

	UIButton *_DecLevelBtn;
	UIButton *_IncLevelBtn;
	mu_uint16 _MinItemLevel;

	UIToggleButton *_LuckCheckBox;
	UIToggleButton *_SkillCheckBox;
	UIToggleButton *_OptionCheckBox;
	UIToggleButton *_ExcellentCheckBox;
	UIToggleButton *_SetItemCheckBox;
	UIToggleButton *_SocketCheckBox;

	UIButton *_AcceptButton;
	UIButton *_CancelButton;

	mu_uint32 _TypeListIndex;
	mu_uint32 _ItemListIndex;
	mu_int32 _TypeSelected;
	mu_int32 _ItemSelected;
	mu_int32 _TypeIndex;
	mu_int32 _ItemIndex;

	mu_boolean _EnableItemInfo;
	ITEM _ItemInfo;

	mu_int32 _HelperIndex;

	mu_uint32 _SlotIndex;
	mu_uint32 _SlotCount;
};

#endif