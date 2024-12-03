#ifndef __MU_UILOGINWINDOW_H__
#define __MU_UILOGINWINDOW_H__

#pragma once

class UILoginWindow : public UIBase
{
public:
	UILoginWindow(UISceneManager *manager);
	~UILoginWindow();

	virtual mu_uint32 OnPreWindowMessage(const SDL_Event *event, mu_size *result) override;

	virtual void Update(mu_uint64 CurrentTime) override;
	virtual void Draw() override;
	virtual void PostDraw() override;

	virtual UIBase *GetFocus(mu_int32 mouseX, mu_int32 mouseY) override;

public:
	UISprite *_BackFrame;

	UISprite *_AccountBack;
	UISprite *_PasswordBack;
	UIEditBox *_AccountBox;
	UIEditBox *_PasswordBox;

	UIButton *_OkButton;
	UIButton *_CancelButton;
};

#endif