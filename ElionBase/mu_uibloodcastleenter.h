#ifndef __MU_UIBLOODCASTLEENTER_H__
#define __MU_UIBLOODCASTLEENTER_H__

#pragma once

#define BLOODCASTLEENTER_POSX 450.0f
#define BLOODCASTLEENTER_POSY 0.0f
#define BLOODCASTLEENTER_WIDTH 190.0f
#define BLOODCASTLEENTER_HEIGHT 429.0f

class UIBloodCastleEnter : public UIBase
{
public:
	UIBloodCastleEnter(UISceneManager *manager);
	~UIBloodCastleEnter();

	void Configure();

	virtual void Update(mu_uint64 CurrentTime) override;
	virtual void Draw() override;
	virtual void PostDraw() override;

	void DrawFrames();
	void DrawButtons();
	void PostDrawButtons();
	void DrawTexts();

	virtual UIBase *GetFocus(mu_int32 mouseX, mu_int32 mouseY) override;

	mu_int32 CheckLimitLevel(mu_boolean isSpecialLevel);

private:
	UISprite *_BackFrame;
	UISprite *_TopFrame;
	UISprite *_BottomFrame;
	UISprite *_LeftFrame;
	UISprite *_RightFrame;

	UIButton *_XButton;
	UIButton *_CloseButton;
	UIButton *_EnterButton[MAX_BLOODCASTLE_BRIDGE_COUNT];
};

#endif