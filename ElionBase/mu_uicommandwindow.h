#ifndef __MU_UICOMMANDWINDOW_H__
#define __MU_UICOMMANDWINDOW_H__

#pragma once

#define COMMANDWINDOW_POSX 450.0f
#define COMMANDWINDOW_POSY 0.0f
#define COMMANDWINDOW_WIDTH 190.0f
#define COMMANDWINDOW_HEIGHT 429.0f

class UICommandWindow : public UIBase
{
public:
	enum
	{
		UICMD_TRADE,
		UICMD_BUY,
		UICMD_PARTY,
		UICMD_WHISPER,
		UICMD_GUILD,
		UICMD_ALLIANCE,
		UICMD_HOSTILITY,
		UICMD_SUSPENDHOSTILITY,
		UICMD_ADDFRIEND,
		UICMD_FOLLOW,
		UICMD_DUEL,
		UICMD_MAX,
	};

public:
	UICommandWindow(UISceneManager *manager);
	~UICommandWindow();

	void ClosingWindow();

	virtual void Update(mu_uint64 CurrentTime) override;
	virtual void Draw() override;
	virtual void PostDraw() override;

	void DrawFrames();
	void DrawButtons();
	void PostDrawButtons();

	virtual UIBase *GetFocus(mu_int32 mouseX, mu_int32 mouseY) override;

	mu_boolean CommandTrade();
	mu_boolean CommandBuy();
	mu_boolean CommandParty();
	mu_boolean CommandWhisper();
	mu_boolean CommandGuild();
	mu_boolean CommandAlliance();
	mu_boolean CommandHostility();
	mu_boolean CommandSuspendHostility();
	mu_boolean CommandAddFriend();
	mu_boolean CommandFollow();
	mu_boolean CommandDuel();

public:
	EINLINE mu_int32 GetCommandSelected()
	{
		return _SelectedCommand;
	}

	EINLINE void SetCommandSelected(mu_int32 index)
	{
		if (_SelectedCommand != -1 &&
			_SelectedCommand != index)
		{
			_Commands[GetCommandSelected()]->ChangeState(UIToggleButton::UI_NORMAL);
		}
		_SelectedCommand = index;
	}

private:
	friend class UIGameRoot;
	UISprite * _BackFrame;
	UISprite *_TopFrame;
	UISprite *_BottomFrame;
	UISprite *_LeftFrame;
	UISprite *_RightFrame;

	UIButton *_XButton;
	UIButton *_CloseButton;
	UIButton *_PlayerButton;
	UISprite *_PlayerNameBack;

	UIToggleButton *_Commands[UICMD_MAX];
	mu_uint32 _SelectedCommand;
};

#endif