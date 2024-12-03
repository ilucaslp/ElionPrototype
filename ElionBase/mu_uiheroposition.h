#ifndef __MU_UIHEROPOSITION_H__
#define __MU_UIHEROPOSITION_H__

#pragma once

class UIHeroPosition : public UIBase
{
public:
	UIHeroPosition(UISceneManager *manager);
	~UIHeroPosition();

	virtual void Draw() override;
	virtual void PostDraw() override;

	virtual UIBase *GetFocus(mu_int32 mouseX, mu_int32 mouseY) override;

	void SetHelperButton(mu_boolean isStarted);

private:
	UISprite *_Left;
	UISprite *_Middle;
	UISprite *_Right;

	UIButton *_ConfigureHelper;
	UIButton *_StartHelper;
};

#endif