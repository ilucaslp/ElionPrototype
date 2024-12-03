#ifndef __MU_UIHELPERPARTYSETTING_H__
#define __MU_UIHELPERPARTYSETTING_H__

#pragma once

class UIHelperPartySetting : public UIBase
{
public:
	UIHelperPartySetting(UISceneManager *manager);
	~UIHelperPartySetting();

	virtual void Update(mu_uint64 CurrentTime) override;
	virtual void PostDraw() override;

	void DrawFrame();
	void DrawText();
	void DrawButton();

	virtual mu_uint32 OnPreKeyPressed(const SDL_Keysym &key) override;

	virtual UIBase *GetFocus(mu_int32 mouseX, mu_int32 mouseY) override;

	void Configure(HELPER_PARTYSETTING &PartySetting);

private:
	friend class UIMessageRoot;
	TMSGBOX_VARS;
	TGLAYOUT_VARS(HealBack);
	TGLAYOUT_VARS(BuffBack);

	UISprite *_HealDiv;
	UISprite *_BuffDiv;

	UIToggleButton *_EnableHealCheckBox;
	UISliderBar *_MinHealSlider;

	UIToggleButton *_EnableBuffCheckBox;

	UIButton *_OkButton;
};

#endif