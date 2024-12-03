#ifndef __MU_UIQUESTWINDOW_H__
#define __MU_UIQUESTWINDOW_H__

#pragma once

#define QUESTWINDOW_POSX 450.0f
#define QUESTWINDOW_POSY 0.0f
#define QUESTWINDOW_WIDTH 190.0f
#define QUESTWINDOW_HEIGHT 429.0f

class UIQuestWindow : public UIBase
{
public:
	UIQuestWindow(UISceneManager *manager);
	~UIQuestWindow();

	void UpdateWindow(mu_int32 screenWidth);
	void Configure();

	virtual void Update(mu_uint64 CurrentTime) override;
	virtual void Draw() override;
	virtual void PostDraw() override;

	void DrawFrames();
	void DrawButtons();
	void PostDrawButtons();
	void DrawTexts();

	void SetCurrentTab(mu_uint32 currentTab);

	virtual UIBase *GetFocus(mu_int32 mouseX, mu_int32 mouseY) override;

public:
	enum
	{
		TAB_QUEST,
		TAB_CHANGECLASS,
		TAB_CASTLETEMPLE,
		TAB_MAX,
	};

private:
	mu_uint8 _CurrentTab;

private:
	UISprite *_BackFrame;
	UISprite *_TopFrame;
	UISprite *_BottomFrame;
	UISprite *_LeftFrame;
	UISprite *_RightFrame;

	UIButton *_XButton;
	UIButton *_CloseButton;

	UISprite *_TabBack;
	UIRadioButton *_TabButtons[TAB_MAX];

	UISprite *_LineSprite;
	
	// Quest Tab
	UIButton *_QuestStartButton;
	UIButton *_QuestGiveUpButton;

	// Change Class Tab
	UIMultiLineEditBox *_ChangeQuestPreviewBox;
	UIMultiLineEditBox *_ChangeQuestStatePreviewBox;
};

#endif