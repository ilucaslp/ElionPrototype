#ifndef __MU_UIHELPERPOTIONSETTING_H__
#define __MU_UIHELPERPOTIONSETTING_H__

#pragma once

class UIHelperPotionSetting : public UIBase
{
public:
	UIHelperPotionSetting(UISceneManager *manager);
	~UIHelperPotionSetting();

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

	UISprite *_MainDiv;

	UISliderBar *_MinLifeSlider;

	UIButton *_OkButton;
};

#endif