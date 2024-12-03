#ifndef __MU_UIOPTIONWINDOW_H__
#define __MU_UIOPTIONWINDOW_H__

#pragma once

class UIOptionData
{
public:
	mu_boolean IsAutoAttack;

	mu_uint32 Width;
	mu_uint32 Height;
	mu_boolean WindowMode;
	mu_boolean AntiAliasing;
	mu_boolean VerticalSync;
	mu_boolean FpsLimit;
	mu_uint8 ShadowType;

	mu_float MusicVolume;
	mu_float SoundVolume;
};

class UIResolutionData
{
public:
	union
	{
		struct 
		{
			mu_uint32 Width;
			mu_uint32 Height;
		};

		mu_uint64 uniqueident;
	};

	mu_string Name;
};

class UIOptionWindow : public UIBase
{
public:
	UIOptionWindow(UISceneManager *manager);
	~UIOptionWindow();

	virtual mu_uint32 OnPreWindowMessage(const SDL_Event *event, mu_size *result) override;

	virtual void Update(mu_uint64 CurrentTime) override;
	virtual void Draw() override;
	virtual void PostDraw() override;

	void DrawFrames();
	void DrawButtons();
	void DrawTexts();

	virtual UIBase *GetFocus(mu_int32 mouseX, mu_int32 mouseY) override;

	virtual mu_boolean OnGFxMessage(mu_uint32 objectid, mu_uint32 message, mu_size message_data) override;

	void PrepareCurrentSettings();

	mu_boolean OnApplyNewResolution(UIOptionData &newSettings);
	void OnApplySettings();

	void Prepare();

	void SetCurrentTab(mu_uint32 currentTab);

	void ClearResolutions();
	void ConfigureResolutionScroll();
	void ConfigureResolutionList();
	void ConfigureShadowButton(mu_uint8 shadowType);

	void SetWhisperSound(mu_boolean bSound);
	mu_boolean IsWhisperSound();
	void SetSlideHelp(mu_boolean bHelp);
	mu_boolean IsSlideHelp();
	void SetRenderLevel(mu_int32 iRender);
	mu_int32 GetRenderLevel();

public:
	enum
	{
		TAB_GENERAL,
		TAB_VIDEO,
		TAB_AUDIO,
		MAX_TAB,
	};

private:
	friend class UIRoot;
	friend class UIOptionRoot;
	UISprite *_Background;
	UISprite *_TopFrame;
	UISprite *_BottomFrame;
	UISprite *_LeftFrame;
	UISprite *_RightFrame;

	UIRadioButton *_TabButtons[MAX_TAB];
	mu_uint32 _CurrentTab;

	UIButton *_ApplyBtn;
	UIButton *_CloseBtn;

	UIToggleButton *_AutoAttackBtn;

	UIScrollBar *_ResolutionScrollBar;
	UIRadioButton *_ResolutionListBtn[MAX_RESOLUTION_RENDER];

	UIToggleButton *_WindowModeBtn;
	UIToggleButton *_AntiAliasingBtn;
	UIToggleButton *_VerticalSyncBtn;
	UIToggleButton *_FpsLimitBtn;
	UIButton *_ShadowTypeBtn;

	std::vector<UIResolutionData> _Resolutions;
	mu_int32 _ResolutionSelected;
	mu_uint32 _ResolutionListIndex;

	UISliderBar *_MusicVolumeSlider;
	UISliderBar *_SoundVolumeSlider;

	mu_boolean _WhisperSound;
	mu_boolean _SlideHelp;
	mu_int32 _RenderLevel;

	UIOptionData _CurrentSettings;
	mu_uint8 _CurrentShadowType;
};

#endif