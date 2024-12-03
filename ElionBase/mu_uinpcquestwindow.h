#ifndef __MU_UINPCQUESTWINDOW_H__
#define __MU_UINPCQUESTWINDOW_H__

#pragma once

#define NPCQUESTWINDOW_POSX 450.0f
#define NPCQUESTWINDOW_POSY 0.0f
#define NPCQUESTWINDOW_WIDTH 190.0f
#define NPCQUESTWINDOW_HEIGHT 429.0f

class UINpcQuestWindow : public UIBase
{
public:
	UINpcQuestWindow(UISceneManager *manager);
	~UINpcQuestWindow();

	void UpdateWindow(mu_int32 screenWidth);
	void Configure();
	void ConfigureDialog(mu_int32 dialogIndex);

	virtual void Update(mu_uint64 CurrentTime) override;
	virtual void Draw() override;
	virtual void Draw3D() override;
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
	mu_uint8 _CurrentQuestState;

private:
	friend class UIGameRoot;
	UISprite *_BackFrame;
	UISprite *_TopFrame;
	UISprite *_BottomFrame;
	UISprite *_LeftFrame;
	UISprite *_RightFrame;

	UIButton *_XButton;
	UIButton *_CloseButton;

	UIButton *_CompleteButton;
	UIButton *_AnswersButton[10];

	UISprite *_LineSprite1;
	UISprite *_LineSprite2;
	UISprite *_MoneyIcon;

	// Change Class Tab
	UIMultiLineEditBox *_ChangeQuestPreviewBox;
	UIMultiLineEditBox *_ChangeQuestStatePreviewBox;

	const MUDialogData *_CurrentDialog;

	mu_uint32 _SlotIndex;
	mu_uint32 _SlotCount;
};

#endif