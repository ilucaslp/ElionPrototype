#ifndef __MU_UILOGINFRAME_H__
#define __MU_UILOGINFRAME_H__

#pragma once

class UILoginFrame : public UIBase
{
public:
	UILoginFrame(UISceneManager *manager);
	~UILoginFrame();

	virtual void Update(mu_uint64 CurrentTime) override;
	virtual void Draw() override;
	virtual void PostDraw() override;

	virtual UIBase *GetFocus(mu_int32 mouseX, mu_int32 mouseY) override;

public:
	UISprite *_DecoRight;

	UIButton *_MenuButton;
	UIButton *_WebsiteButton;
	UIButton *_ForumButton;
	UIButton *_FacebookButton;
};

#endif