#ifndef __MU_UICHAOSMIXMENU_H__
#define __MU_UICHAOSMIXMENU_H__

#pragma once

class UIChaosMixMenu : public UIBase
{
public:
	UIChaosMixMenu(UISceneManager *manager);
	~UIChaosMixMenu();

	virtual void Update(mu_uint64 CurrentTime) override;
	virtual void PostDraw() override;

	void DrawFrame();
	void DrawText();
	void DrawButton();

	virtual UIBase *GetFocus(mu_int32 mouseX, mu_int32 mouseY) override;

private:
	UISprite *_Back;
	UISprite *_Top;
	UISprite *_Middle;
	UISprite *_Bottom;

	UIButton *_GeneralButton;
	UIButton *_ChaosButton;
	UIButton *_380Button;
	UIButton *_CancelButton;
};

#endif