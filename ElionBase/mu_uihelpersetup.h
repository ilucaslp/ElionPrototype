#ifndef __MU_UIHELPERSETUP_H__
#define __MU_UIHELPERSETUP_H__

#pragma once

#define HELPERSETUP_POSX 450.0f
#define HELPERSETUP_POSY 0.0f
#define HELPERSETUP_WIDTH 190.0f
#define HELPERSETUP_HEIGHT 429.0f

#define MAX_HELPERSETUP_OBITEMSHOW (10)

extern mu_boolean g_IsHelperStarted;
extern HELPER_SETTING g_DefaultHelperSettings;
extern HELPER_SETTING g_HelperSettings;

class UIHelperSetup : public UIBase
{
public:
	enum
	{
		TAB_HUNTING,
		TAB_OBTAINING,
		TAB_MAX,
	};

public:
	UIHelperSetup(UISceneManager *manager);
	~UIHelperSetup();

	void ConfigureObtainScroll(mu_boolean keepScroll = false);
	void ConfigureObtainList();

	void AddObtainItem(HELPER_ITEMSETTING &itemSetting);
	void ModifyObtainItem(mu_int32 itemIndex, HELPER_ITEMSETTING &itemSetting);
	void RemoveObtainItem(mu_int32 itemIndex);

	virtual mu_uint32 OnPreWindowMessage(const SDL_Event *event, mu_size *result) override;

	virtual mu_boolean OnGFxMessage(mu_uint32 objectid, mu_uint32 message, mu_size message_data) override;

	virtual void Update(mu_uint64 CurrentTime) override;
	virtual void Draw() override;
	virtual void PostDraw() override;

	void DrawFrames();
	void DrawButtons();
	void DrawTexts();
	void PostDrawButtons();

	virtual UIBase *GetFocus(mu_int32 mouseX, mu_int32 mouseY) override;

	void SetCurrentTab(mu_uint32 tab);

	void Configure(HELPER_SETTING &helperSetting);
	void PrepareSetting();

public:
	EINLINE HELPER_SETTING & GetSetting()
	{
		return _InternalSetting;
	}

private:
	friend class UIGameRoot;
	friend class UIMessageRoot;
	UISprite * _BackFrame;
	UISprite *_TopFrame;
	UISprite *_BottomFrame;
	UISprite *_LeftFrame;
	UISprite *_RightFrame;

	UIButton *_XButton;
	UIButton *_CloseButton;
	UIButton *_ResetButton;
	UIButton *_SaveButton;

	UIRadioButton *_Tabs[TAB_MAX];

	mu_uint32 _CurrentTab;

	// Tab Frame
	UISprite *_DFBackground;
	UISprite *_DFTopFrame;
	UISprite *_DFBottomFrame;
	UISprite *_DFLeftFrame;
	UISprite *_DFRightFrame;

	// Hunting Tab
	UISprite *_HTDivs[5];

	UISprite *_HTRange;
	UIButton *_HTRangePlusBtn;
	UIButton *_HTRangeMinusBtn;

	UIToggleButton *_HTPotionCheckBox;
	UIButton *_HTPotionSettingBtn;
	UIToggleButton *_HTAutoHealCheckBox;
	UIToggleButton *_HTPartyCheckBox;
	UIButton *_HTPartySettingBtn;

	UIToggleButton *_HTRegularAttackCheckBox;
	UIToggleButton *_HTSkillsCloselyCheckBox;
	UIToggleButton *_HTLongDistCheckBox;
	UIToggleButton *_HTReturnPosCheckBox;

	UISprite *_HTReturnTimeBack;
	UIEditBox *_HTReturnTimeEditBox;

	UIButton *_HTSkillBtn[BUTTON_SKILLINDEX::BSI_MAX];
	UISprite *_HTSkillIcon[BUTTON_SKILLINDEX::BSI_MAX];

	UIToggleButton *_HTActSkill1DelayCheckBox;
	UISprite *_HTActSkill1DelayBack;
	UIEditBox *_HTActSkill1DelayEditBox;
	UIToggleButton *_HTActSkill1ConCheckBox;
	UIButton *_HTActSkill1ConSettingBtn;

	UIToggleButton *_HTActSkill2DelayCheckBox;
	UISprite *_HTActSkill2DelayBack;
	UIEditBox *_HTActSkill2DelayEditBox;
	UIToggleButton *_HTActSkill2ConCheckBox;
	UIButton *_HTActSkill2ConSettingBtn;

	UIToggleButton *_HTComboCheckBox;

	UIToggleButton *_HTBuffDurCheckBox;

	// Obtaining Tab
	UISprite *_OBDivs[5];

	UISprite *_OBRange;
	UIButton *_OBRangePlusBtn;
	UIButton *_OBRangeMinusBtn;

	UIToggleButton *_OBRepairCheckBox;
	UIToggleButton *_OBPickAllCheckBox;
	UIToggleButton *_OBPickSelCheckBox;
	UIToggleButton *_OBJewelCheckBox;
	UIToggleButton *_OBSetItemCheckBox;
	UIToggleButton *_OBZenCheckBox;
	UIToggleButton *_OBExcCheckBox;
	UIToggleButton *_OBExtraCheckBox;

	TGLAYOUT_VARS(OBItemsBox);
	UIScrollBar *_OBItemsScrollBar;
	UIRadioButton *_OBItemsListBtn[MAX_HELPERSETUP_OBITEMSHOW];
	mu_int32 _OBItemSelected;
	mu_int32 _OBItemListIndex;
	UIButton *_OBItemAddBtn;
	UIButton *_OBItemModBtn;
	UIButton *_OBItemDelBtn;

	HELPER_SETTING _InternalSetting;
};

#endif