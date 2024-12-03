#ifndef __MU_UIPARTYWINDOW_H__
#define __MU_UIPARTYWINDOW_H__

#pragma once

#define PARTYWINDOW_POSX 450.0f
#define PARTYWINDOW_POSY 0.0f
#define PARTYWINDOW_WIDTH 190.0f
#define PARTYWINDOW_HEIGHT 429.0f

class UIPartyWindow : public UIBase
{
public:
	UIPartyWindow(UISceneManager *manager);
	~UIPartyWindow();

	void ChangePosition(mu_float x, mu_float y);

	virtual void Update(mu_uint64 CurrentTime) override;
	virtual void Draw() override;
	virtual void PostDraw() override;

	void DrawFrames();
	void DrawButtons();
	void PostDrawButtons();

	virtual UIBase *GetFocus(mu_int32 mouseX, mu_int32 mouseY) override;

private:
	UISprite *_BackFrame;
	UISprite *_TopFrame;
	UISprite *_BottomFrame;
	UISprite *_LeftFrame;
	UISprite *_RightFrame;

	UIButton *_XButton;
	UIButton *_CloseButton;

	struct UIPartyMember
	{
		UISpriteColor *Back1;
		UISpriteColor *Back2;

		UISprite *TopLeft;
		UISprite *TopRight1;
		UISprite *TopRight2;
		UISprite *BottomLeft;
		UISprite *BottomRight;
		UISprite *TopLine1;
		UISprite *TopLine2;
		UISprite *BottomLine;
		UISprite *LeftLine;
		UISprite *RightLine1;
		UISprite *RightLine2;

		UISprite *HPBack;
		UISprite *HPBar;

		UIButton *KickPlayer;
	};

	UISprite *_LeaderFlag;

	UIPartyMember _Members[MAX_PARTYS];
};

#endif