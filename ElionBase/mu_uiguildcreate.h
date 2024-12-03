#ifndef __MU_UIGUILDCREATE_H__
#define __MU_UIGUILDCREATE_H__

#pragma once

class UIGuildCreate : public UIBase
{
public:
	UIGuildCreate(UISceneManager *manager);
	~UIGuildCreate();

	void InitializeWindow();

	virtual void InitializeGPU() override;
	virtual void ReleaseGPU() override;

	virtual void Update(mu_uint64 CurrentTime) override;
	virtual void Draw() override;
	virtual void PostDraw() override;

	void DrawFrames();
	void DrawButtons();
	void PostDrawButtons();
	void DrawTexts();

	virtual UIBase *GetFocus(mu_int32 mouseX, mu_int32 mouseY) override;

	void SetSelectedColor(mu_uint32 index);
	void SetBufferColor(mu_uint32 index);

	void BackPage();
	void NextPage();

public:
	enum PAGE_ENUM
	{
		PAGE_INFO,
		PAGE_MARK,
		PAGE_RESULT,
	};

private:
	friend class UIGameRoot;
	UISprite *_BackFrame;
	UISprite *_TopFrame;
	UISprite *_BottomFrame;
	UISprite *_LeftFrame;
	UISprite *_RightFrame;

	// Golden Lines
	UISprite *_BigGoldTop;
	UISprite *_BigGoldBottom;
	UISprite *_BigGoldLeft;
	UISprite *_BigGoldRight;

	UISprite *_SmallGoldTop;
	UISprite *_SmallGoldBottom;
	UISprite *_SmallGoldLeft;
	UISprite *_SmallGoldRight;

	// Marks & Colors Buttons
	UISprite *_MarksBack[GUILDMARK_HEIGHT][GUILDMARK_WIDTH];
	UIOverButton *_MarksBtn[GUILDMARK_HEIGHT][GUILDMARK_WIDTH];
	UISprite *_ColorsBack[2][MAX_GUILD_COLORS_HALF];
	UIOverButton *_ColorsBtn[2][MAX_GUILD_COLORS_HALF];
	UISprite *_SelectedBack;
	UISprite *_SelectedColor;
	UISprite *_MarkSprite;
	MUTexture _MarkTexture;
	EStagingBufferBackend _MarkStagingBuffer;
	MUTexture _ColorsTexture[MAX_GUILD_COLORS];
	EStagingBufferBackend _ColorsStagingBuffer[MAX_GUILD_COLORS];

	mu_uint32 _ColorIndex;
	mu_uint8 _ColorBuffer[MAX_GUILDMARKBUFFER];
	mu_string _GuildName;

	UISprite *_InputBack;
	UIEditBox *_InputBox;

	UIButton *_BackBtn;
	UIButton *_NextBtn;
	UIButton *_MakeBtn;

	UIButton *_XButton;
	UIButton *_CloseButton;

	mu_uint32 _CurrentPage;
};

#endif