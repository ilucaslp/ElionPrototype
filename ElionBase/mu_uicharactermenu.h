#ifndef __MU_UICHARACTERMENU_H__
#define __MU_UICHARACTERMENU_H__

#pragma once

class UICharacterMenu : public UIBase
{
public:
	UICharacterMenu(UISceneManager *manager);
	~UICharacterMenu();

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
	UIButton *_OptionButton;
	UIButton *_CloseButton;
};

#endif