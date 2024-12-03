#ifndef __MU_UICHATINPUT_H__
#define __MU_UICHATINPUT_H__

#pragma once

#define MAX_CHAT_HISTORY 12
#define MAX_WHISPER_HISTORY 5

enum INPUT_MESSAGE_TYPE
{
	INPUT_NOTHING = -1,
	INPUT_CHAT_MESSAGE,
	INPUT_PARTY_MESSAGE,
	INPUT_GUILD_MESSAGE,
};

class UIChatInput : public UIBase
{
public:
	UIChatInput(UISceneManager *manager);
	~UIChatInput();

	virtual mu_uint32 OnPreWindowMessage(const SDL_Event *event, mu_size *result) override;
	virtual mu_uint32 OnWindowMessage(const SDL_Event *event, mu_size *result) override;

	virtual void Update(mu_uint64 CurrentTime) override;
	virtual void Draw() override;
	virtual void PostDraw() override;

	void DrawBackground();
	void DrawButtons();
	void PostDrawButtons();

	virtual UIBase *GetFocus(mu_int32 mouseX, mu_int32 mouseY) override;

	void SetInputMessageType(mu_int32 inputMessageType);
	mu_int32 GetInputMessageType();

	mu_boolean IsBlockWhisper();

	void AddChatHistory(const mu_unicode *text);
	void RemoveChatHistory(mu_uint32 index);
	void RemoveAllChatHistory();

	void AddWhisperHistory(const mu_unicode *text);
	void RemoveWhisperHistory(mu_uint32 index);
	void RemoveAllWhisperHistory();

	void SetWhisperID(const mu_text *whisperID);

private:
	friend class UIGameRoot;
	mu_int32 _InputMessageType;
	mu_boolean _WhisperSend;
	std::vector<mu_string> _ChatHistory;
	mu_uint32 _ChatHistoryIndex;
	std::vector<mu_string> _WhisperHistory;
	mu_uint32 _WhisperHistoryIndex;

	UISprite *_Background;

	UIRadioButton *_ChatTypeButton;
	UIRadioButton *_PartyTypeButton;
	UIRadioButton *_GuildTypeButton;
	UIToggleButton *_WhisperBlockButton;
	UIToggleButton *_ChatSystemButton;
	UIToggleButton *_ChatLogButton;
	UIToggleButton *_ChatFrameButton;
	UIButton *_SizeButton;
	UIButton *_TransparencyButton;

	UIEditBox *_ChatInput;
	UIEditBox *_WhisperInput;
};

#endif