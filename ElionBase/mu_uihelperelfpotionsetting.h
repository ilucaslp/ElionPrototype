#ifndef __MU_UIHELPERELFPOTIONSETTING_H__
#define __MU_UIHELPERELFPOTIONSETTING_H__

#pragma once

class UIHelperElfPotionSetting : public UIBase
{
public:
	UIHelperElfPotionSetting(UISceneManager *manager);
	~UIHelperElfPotionSetting();

	virtual void Update(mu_uint64 CurrentTime) override;
	virtual void PostDraw() override;

	void DrawFrame();
	void DrawText();
	void DrawButton();

	virtual mu_uint32 OnPreKeyPressed(const SDL_Keysym &key) override;

	virtual UIBase *GetFocus(mu_int32 mouseX, mu_int32 mouseY) override;

	void Configure(HELPER_POTIONSETTING &PotionSetting);

private:
	friend class UIMessageRoot;
	TMSGBOX_VARS;
	TGLAYOUT_VARS(MainBack);
	TGLAYOUT_VARS(AutoHealBack);

	UISprite *_MainDiv;
	UISprite *_AutoHealDiv;

	UISliderBar *_MinLifeSlider;
	UISliderBar *_MinAutoHealSlider;

	UIButton *_OkButton;
};

#endif