#ifndef __MU_UIFAILEDCONNECT_H__
#define __MU_UIFAILEDCONNECT_H__

#pragma once

class UIFailedConnect : public UIBase
{
public:
	UIFailedConnect(UISceneManager *manager);
	~UIFailedConnect();

	virtual void Update(mu_uint64 CurrentTime) override;
	virtual void PostDraw() override;

	void DrawFrame();
	void DrawText();
	void DrawButton();

	virtual mu_uint32 OnPreKeyPressed(const SDL_Keysym &key) override;

	virtual UIBase *GetFocus(mu_int32 mouseX, mu_int32 mouseY) override;

private:
	UISprite *_BackFrame;
	UIButton *_OkButton;
};

#endif