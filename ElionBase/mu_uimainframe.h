#ifndef __MU_UI_MAINFRAME_H__
#define __MU_UI_MAINFRAME_H__

#pragma once

class UIMainFrame : public UIBase
{
public:
	UIMainFrame(UISceneManager *manager, mu_float x, mu_float y, mu_float width, mu_float height);
	~UIMainFrame();

	virtual void Update(mu_uint64 CurrentTime) override;
	virtual void Draw() override;
	virtual void PostDraw() override;

	void DrawFrames();
	void DrawButtons();
	void PostDrawButtons();
	void DrawLifeMana();
	void DrawGaugeSD();
	void DrawGaugeAG();
	void DrawExperience();

	virtual UIBase *GetFocus(mu_int32 mouseX, mu_int32 mouseY) override;

	void SetPreMasterExperience(mu_int64 PreExp);
	void SetGetMasterExperience(mu_int64 GetExp);
	void SetPreExperience(mu_int64 PreExp);
	void SetGetExperience(mu_int64 GetExp);

	void ChangeOptionButton();

public:
	mu_boolean _ExperienceEffect;
	mu_uint64 _EffectTime;
	mu_int64 _PreExperience;
	mu_int64 _GetExperience;
	mu_int64 _PreMasterExperience;
	mu_int64 _GetMasterExperience;

private:
	friend class UIGameRoot;
	friend class UIOptionRoot;
	UISprite *_Frame1;
	UISprite *_Frame2;
	UISprite *_Frame3;
	UISprite *_FrameSkill;
	UISprite *_LifeBar;
	UISprite *_ManaBar;
	UISprite *_ShieldBar;
	UISprite *_AgBar;
	UISprite *_ExperienceBar;
	UISprite *_ExperienceBar2;
	UISpriteColor *_ExperienceColor;
	UIToggleButton *_CharInfoBtn;
	UIToggleButton *_MyInvenBtn;
	UIToggleButton *_FriendBtn;
	UIToggleButton *_OptionsBtn;
	UIToggleButton *_CashShopBtn;
};

#endif