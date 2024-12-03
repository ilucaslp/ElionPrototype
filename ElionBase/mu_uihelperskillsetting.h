#ifndef __MU_UIHELPERSKILLSETTING_H__
#define __MU_UIHELPERSKILLSETTING_H__

#pragma once

class UIHelperSkillSetting : public UIBase
{
public:
	UIHelperSkillSetting(UISceneManager *manager);
	~UIHelperSkillSetting();

	virtual mu_boolean OnGFxMessage(mu_uint32 objectid, mu_uint32 message, mu_size message_data) override;

	virtual void Update(mu_uint64 CurrentTime) override;
	virtual void PostDraw() override;

	void DrawFrame();
	void DrawText();
	void DrawButton();

	virtual mu_uint32 OnPreKeyPressed(const SDL_Keysym &key) override;

	virtual UIBase *GetFocus(mu_int32 mouseX, mu_int32 mouseY) override;

	void Configure(mu_uint8 ActIndex, HELPER_SKILLSETTING &SkillSetting);

private:
	friend class UIMessageRoot;
	TMSGBOX_VARS;
	TGLAYOUT_VARS(PreConBack);
	TGLAYOUT_VARS(SubConBack);

	UISprite *_PreConDiv;
	UISprite *_SubConDiv;

	UIRadioButton *_HuntingRange;
	UIRadioButton *_HuntingAttacker;

	UIRadioButton *_TwoPlus;
	UIRadioButton *_ThreePlus;
	UIRadioButton *_FourPlus;
	UIRadioButton *_FivePlus;

	UIButton *_OkButton;

	mu_uint8 _ActIndex;
};

#endif