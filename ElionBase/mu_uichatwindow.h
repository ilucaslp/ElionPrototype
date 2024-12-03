#ifndef __MU_UICHATWINDOW_H__
#define __MU_UICHATWINDOW_H__

#pragma once

enum MESSAGE_TYPE
{
	TYPE_ALL_MESSAGE = 0,
	TYPE_CHAT_MESSAGE,
	TYPE_WHISPER_MESSAGE,
	TYPE_SYSTEM_MESSAGE,
	TYPE_ERROR_MESSAGE,
	TYPE_PARTY_MESSAGE,
	TYPE_GUILD_MESSAGE,
	TYPE_UNION_MESSAGE,
	TYPE_GM_MESSAGE,

	NUMBER_OF_TYPES,
	TYPE_UNKNOWN = 0xFFFFFFFF,
};

class UIChatMessageText
{
public:
	UIChatMessageText() : _MessageType(TYPE_UNKNOWN), _ReferCount(0)
	{

	}

	~UIChatMessageText()
	{
		Release();
	}

	mu_boolean Create(const mu_string &strID, const mu_string &strText, MESSAGE_TYPE MsgType)
	{
		if (MsgType >= NUMBER_OF_TYPES)
			return false;

		_ID = strID;
		_Message = strText;
		_MessageType = MsgType;
		_ReferCount = 0;

		return true;
	}

	void Release()
	{
		_ID.clear();
		_Message.clear();
		_MessageType = TYPE_UNKNOWN;
	}

	const mu_string &GetID() { return _ID; }
	const mu_string &GetText() { return _Message; }
	MESSAGE_TYPE GetType() { return _MessageType; }

private:
	friend class UIChatWindow;
	mu_string _ID, _Message;
	MESSAGE_TYPE _MessageType;
	mu_uint32 _RenderIndex;
	mu_uint32 _ReferCount;
};

class UIChatWindow : public UIBase
{
public:
	enum
	{
		MAX_CHAT_BUFFER_SIZE = 60,
		MAX_NUMBER_OF_LINES = 200,
		MAX_MESSAGES = MAX_NUMBER_OF_LINES * NUMBER_OF_TYPES * 3,
		WND_WIDTH = 254,
		FONT_LEADING = 4,
		WND_TOP_BOTTOM_EDGE = 2,
		WND_LEFT_RIGHT_EDGE = 4,
		RESIZING_BTN_WIDTH = WND_WIDTH,
		RESIZING_BTN_HEIGHT = 10,
		SCROLL_BAR_WIDTH = 7,
		SCROLL_TOP_BOTTOM_PART_HEIGHT = 3,
		SCROLL_MIDDLE_PART_HEIGHT = 15,
		SCROLL_BTN_WIDTH = 15,
		SCROLL_BTN_HEIGHT = 30,
		CLIENT_WIDTH = WND_WIDTH - SCROLL_BAR_WIDTH - (WND_LEFT_RIGHT_EDGE * 2),
		DEFAULT_SHOWING_LINES = 6,
		MAX_SHOWING_LINES = 15,
		SHOWING_LINES_DIVIDER = 3,
		WND_HEIGHT = (SCROLL_MIDDLE_PART_HEIGHT*DEFAULT_SHOWING_LINES) + (SCROLL_TOP_BOTTOM_PART_HEIGHT * 2) + (WND_TOP_BOTTOM_EDGE * 2),
	};

	typedef std::vector<UIChatMessageText*> messages_vector;
	typedef std::list<mu_uint32> index_list;
	typedef std::vector<mu_string> filters_vector;

public:
	UIChatWindow(UISceneManager *manager, mu_float x, mu_float y, mu_float width, mu_float height);
	~UIChatWindow();

	virtual void Update(mu_uint64 CurrentTime) override;
	virtual void Draw() override;

	void DrawBackground();
	void DrawMessages();
	void DrawFrame();

	virtual UIBase *GetFocus(mu_int32 mouseX, mu_int32 mouseY) override;

	virtual mu_uint32 OnPreWindowMessage(const SDL_Event *event, mu_size *result) override;

	void SetBackAlphaAuto();
	void SetBackAlpha(mu_float Alpha);
	mu_float GetBackAlpha() const;

	messages_vector *GetMessages(MESSAGE_TYPE messageType);
	void ChangeMessage(MESSAGE_TYPE messageType);
	MESSAGE_TYPE GetCurrentMessageType();
	mu_int32 GetCurrentRenderEndLine();

	void SetSizeAuto();
	void SetNumberOfShowingLines(mu_int32 ShowingLines, EPoint *outBox = nullptr);
	mu_int32 GetNumberOfShowingLines();
	mu_size GetNumberOfLines(MESSAGE_TYPE messageType);

	void UpdateWindowSize();
	void UpdateScrollPosition();

	void ResizeSection();
	void OnScrolling();
	void Scrolling(mu_int32 RenderEndLine);

	void RemoveFrontLine(MESSAGE_TYPE messageType);

	void AddText(const mu_string& strID, const mu_string& strText, MESSAGE_TYPE MsgType, MESSAGE_TYPE ErrMsgType = TYPE_ALL_MESSAGE);
	void ProcessAddText(const mu_string& strID, const mu_string& strText, MESSAGE_TYPE MsgType, MESSAGE_TYPE ErrMsgType);

	mu_boolean CheckFilterText(const mu_string &text);
	void AddFilterWord(const mu_string &word);
	void SetFilterText(const mu_string &word);
	void ResetFilter();

	void Clear(MESSAGE_TYPE messageType);
	void ClearAll();

	void ShowFrame();
	void HideFrame();
	mu_boolean IsShowFrame();

	void ShowChatLog();
	void HideChatLog();

	mu_boolean CheckChatRedundancy(const mu_string& message, mu_int32 searchLine = 1);

	void SeparateText(const mu_string &id, const mu_string &message, mu_string &text1, mu_string &text2, MESSAGE_TYPE messageType);

private:
	friend class UIGameRoot;
	UIChatMessageText _Messages[MAX_MESSAGES];
	index_list _FreeMessages;
	messages_vector _MessagesList[NUMBER_OF_TYPES];
	filters_vector _Filters;

	mu_boolean _ShowFrame;
	mu_boolean _ShowChatLog;

	EPoint _ScrollButtonPos;
	mu_int32 _ShowingLines;
	MESSAGE_TYPE _CurrentMessageType;
	mu_int32 _CurrentEndLine;
	mu_int32 _GrapRelativePosY;
	mu_float _BackAlpha;

	mu_boolean _PointedMessage;
	mu_int32 _PointedMessageIndex;

private:
	UISpriteColor *_Background;

	UISlideSection *_ResizeSlide;
	UIScrollBar *_ScrollBar;
};

#endif