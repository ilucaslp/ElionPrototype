#ifndef __MU_UIGAMEMENU_H__
#define __MU_UIGAMEMENU_H__

#pragma once

class UIGameMenu : public UIBase
{
public:
	UIGameMenu(UISceneManager *manager);
	~UIGameMenu();

	virtual void Update(mu_uint64 CurrentTime) override;
	virtual void Draw() override;
	virtual void PostDraw() override;

	virtual UIBase *GetFocus(mu_int32 mouseX, mu_int32 mouseY) override;

private:
	UISprite *_BackFrame;
	UISprite *_TopFrame;
	UISprite *_BottomFrame;
	UISprite *_LeftFrame;
	UISprite *_RightFrame;

	UIButton *_ExitButton;
	UIButton *_ServerButton;
	UIButton *_CharacterButton;
	UIButton *_OptionButton;
	UIButton *_CloseButton;
};

#endif