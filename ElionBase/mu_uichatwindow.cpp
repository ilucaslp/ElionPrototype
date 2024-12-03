#include "stdafx.h"
#include "mu_uichatwindow.h"

#define SCREEN_WIDTH 640.0f
#define SCREEN_HEIGHT 480.0f

#define SCROLLBAR_BACKWIDTH 7.0f

UIChatWindow::UIChatWindow(UISceneManager *manager, mu_float x, mu_float y, mu_float width, mu_float height) : UIBase()
{
	if (manager != nullptr)
	{
		manager->AddGFxObject(this);
	}

	_SceneManager = manager;

	y -= RESIZING_BTN_HEIGHT;
	height += RESIZING_BTN_HEIGHT;

	UIBase::SetAspectRatio(SCREEN_WIDTH, SCREEN_HEIGHT);
	UIBase::SetPosition(x, y);
	UIBase::SetDimensions(width, height);

	// Initialize
	{
		// Configure Messages
		for (mu_uint32 i = 0; i < MAX_MESSAGES; ++i)
		{
			_Messages[i]._RenderIndex = i;
			_FreeMessages.push_back(i);
		}

		_ScrollButtonPos = EPoint(0, 0);
		_ShowingLines = DEFAULT_SHOWING_LINES;
		_CurrentEndLine = 0;
		_BackAlpha = 0.6f;
		_ShowFrame = false;
		_CurrentMessageType = TYPE_ALL_MESSAGE;
		_ShowChatLog = true;
		_PointedMessage = false;
		_PointedMessageIndex = 0;

		_Background = new UISpriteColor(manager, this, MU::MakeARGB(0, 0, 0, _BackAlpha), 640.0f, 480.0f, x, y, width, height);
		_Background->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

		_ResizeSlide = new UISlideSection(GAMEUI::MESSAGE_CHATWINDOW_RESIZE_SLIDE, manager, this, 640.0f, 480.0f, x, y, 254.0f, RESIZING_BTN_HEIGHT);
		_ResizeSlide->SetSectionInfo(MAX_SHOWING_LINES / SHOWING_LINES_DIVIDER, DEFAULT_SHOWING_LINES / SHOWING_LINES_DIVIDER, SCROLL_MIDDLE_PART_HEIGHT * 3, SCROLL_TOP_BOTTOM_PART_HEIGHT);
		_ResizeSlide->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_ResizeSlide->SetStateData(UISlideSection::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::SCROLLBAR_STRETCH], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(100, 100, 100, 255)));
		_ResizeSlide->SetStateData(UISlideSection::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::SCROLLBAR_STRETCH], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_ResizeSlide->SetStateData(UISlideSection::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::SCROLLBAR_STRETCH], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_ResizeSlide->SetStateData(UISlideSection::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::SCROLLBAR_STRETCH], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(178, 178, 178, 255)));

		_ScrollBar = new UIScrollBar(GAMEUI::MESSAGE_CHATWINDOW_SCROLLBAR, manager, this, 640.0f, 480.0f, x + width - SCROLL_BAR_WIDTH - WND_LEFT_RIGHT_EDGE - 4.0f, y + RESIZING_BTN_HEIGHT + WND_TOP_BOTTOM_EDGE, UIScrollBar::ORI_VERTICAL);
		_ScrollBar->ConfigureTop(&INTERFACES::Data[INTERFACES::SCROLLBAR_TOP], EBlendState::eAlphaSrc_InvAlphaSrc, SCROLLBAR_BACKWIDTH, SCROLL_TOP_BOTTOM_PART_HEIGHT);
		_ScrollBar->ConfigureMiddle(&INTERFACES::Data[INTERFACES::SCROLLBAR_MIDDLE], EBlendState::eAlphaSrc_InvAlphaSrc, SCROLLBAR_BACKWIDTH, GetNumberOfShowingLines() * SCROLL_MIDDLE_PART_HEIGHT);
		_ScrollBar->ConfigureBottom(&INTERFACES::Data[INTERFACES::SCROLLBAR_BOTTOM], EBlendState::eAlphaSrc_InvAlphaSrc, SCROLLBAR_BACKWIDTH, SCROLL_TOP_BOTTOM_PART_HEIGHT);
		_ScrollBar->ConfigureScroll(EBlendState::eAlphaSrc_InvAlphaSrc, SCROLL_BTN_WIDTH, SCROLL_BTN_HEIGHT, true);

		_ScrollBar->SetStateData(UIScrollBar::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::SCROLLBAR_SLIDE_OFF], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_ScrollBar->SetStateData(UIScrollBar::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::SCROLLBAR_SLIDE_ON], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_ScrollBar->SetStateData(UIScrollBar::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::SCROLLBAR_SLIDE_ON], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_ScrollBar->SetStateData(UIScrollBar::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::SCROLLBAR_SLIDE_ON], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(178, 178, 178, 255)));

		_ScrollBar->ConfigureScrollInfo(0.0f, 1.0f, 1.0f);
		_ScrollBar->ConfigureScrollDimension(GetNumberOfShowingLines() * SCROLL_MIDDLE_PART_HEIGHT);
		_ScrollBar->ChangeState(UIScrollBar::UI_DISABLED);

		_ScrollBar->ApplyChanges();
	}
}

UIChatWindow::~UIChatWindow()
{

}

void UIChatWindow::Update(mu_uint64 CurrentTime)
{
	if (_ScrollBar->GetState() == UIScrollBar::UI_DISABLED)
	{
		if (GetNumberOfLines(GetCurrentMessageType()) > GetNumberOfShowingLines())
		{
			_ScrollBar->ChangeState(UIScrollBar::UI_NORMAL);
		}
	}
	else
	{
		if (GetNumberOfLines(GetCurrentMessageType()) <= GetNumberOfShowingLines())
		{
			_ScrollBar->ChangeState(UIScrollBar::UI_DISABLED);
			_ScrollBar->ConfigureScrollInfo(0.0f, 1.0f, 1.0f);
		}
	}
}

void UIChatWindow::Draw()
{
	DrawBackground();
	DrawMessages();
	DrawFrame();
}

void UIChatWindow::DrawBackground()
{
	if(_ShowFrame == true)
	{
		_Background->SetColor(MU::MakeARGB(0, 0, 0, 255.0f * GetBackAlpha()));
		_Background->Draw();
	}
}

void UIChatWindow::DrawMessages()
{
	if (_ShowChatLog == false) return;

	mu_float RenderPosX = _ViewX;
	mu_float RenderPosY = _ViewY + SCROLL_TOP_BOTTOM_PART_HEIGHT + RESIZING_BTN_HEIGHT + FONT_LEADING;
	messages_vector *messages = GetMessages(GetCurrentMessageType());

	mu_int32 RenderStartLine = 0;
	if (GetCurrentRenderEndLine() >= _ShowingLines)
	{
		RenderStartLine = GetCurrentRenderEndLine() - _ShowingLines;
	}
	else
	{
		RenderPosY += SCROLL_MIDDLE_PART_HEIGHT * (_ShowingLines - GetCurrentRenderEndLine());
	}

	mu_uint8 Alpha = _ShowFrame == true ? 100 : 150;

	for (mu_int32 i = RenderStartLine, s = 0; i < GetCurrentRenderEndLine(); ++i, ++s)
	{
		if (i < 0 && i >= (mu_int32)messages->size()) break;

		mu_boolean RenderMessage = true;
		mu_uint32 Color, BackColor;
		MUTextSimpleRender *font = FONT_NORMAL;

		UIChatMessageText *pMsgText = (*messages)[i];

		if (pMsgText->GetType() == TYPE_WHISPER_MESSAGE)
		{
			BackColor = MU::MakeARGB(255, 200, 50, 150);
			Color = MU::MakeARGB(0, 0, 0, 255);
		}
		else if (pMsgText->GetType() == TYPE_SYSTEM_MESSAGE)
		{
			BackColor = MU::MakeARGB(0, 0, 0, 150);
			Color = MU::MakeARGB(100, 150, 255, 255);
		}
		else if (pMsgText->GetType() == TYPE_ERROR_MESSAGE)
		{
			BackColor = MU::MakeARGB(0, 0, 0, 150);
			Color = MU::MakeARGB(255, 30, 0, 255);
		}
		else if (pMsgText->GetType() == TYPE_CHAT_MESSAGE)
		{
			BackColor = MU::MakeARGB(0, 0, 0, Alpha);
			Color = MU::MakeARGB(205, 220, 239, 255);
		}
		else if (pMsgText->GetType() == TYPE_PARTY_MESSAGE)
		{
			BackColor = MU::MakeARGB(0, 200, 255, 150);
			Color = MU::MakeARGB(0, 0, 0, 255);
		}
		else if (pMsgText->GetType() == TYPE_GUILD_MESSAGE)
		{
			BackColor = MU::MakeARGB(0, 255, 150, 200);
			Color = MU::MakeARGB(0, 0, 0, 255);
		}
		else if (pMsgText->GetType() == TYPE_GUILD_MESSAGE)
		{
			BackColor = MU::MakeARGB(0, 255, 150, 200);
			Color = MU::MakeARGB(0, 0, 0, 255);
		}
		else if (pMsgText->GetType() == TYPE_UNION_MESSAGE)
		{
			BackColor = MU::MakeARGB(200, 200, 0, 200);
			Color = MU::MakeARGB(0, 0, 0, 255);
		}
		else if (pMsgText->GetType() == TYPE_GM_MESSAGE)
		{
			BackColor = MU::MakeARGB(30, 30, 30, 200);
			Color = MU::MakeARGB(250, 200, 50, 255);
			font = FONT_BOLD;
		}
		else
		{
			RenderMessage = false;
		}

		if (RenderMessage && !pMsgText->GetText().empty())
		{
			EPoint ptRenderPos = EPoint(RenderPosX + WND_LEFT_RIGHT_EDGE, RenderPosY + (SCROLL_MIDDLE_PART_HEIGHT*s));

			if (!pMsgText->GetID().empty())
			{
				if (_PointedMessage == true && _PointedMessageIndex == i)
				{
					BackColor = MU::MakeARGB(30, 30, 30, 180);
					Color = MU::MakeARGB(255, 128, 255, 255);
				}

				mu_string strLine = pMsgText->GetID() + _T(" : ") + pMsgText->GetText();

				font->DrawStringC2D(TEXTINDEX::BEGIN_CHATMESSAGES + pMsgText->_RenderIndex, MU_TO_WINDOW(ptRenderPos.x, _AspectWidth), MU_TO_WINDOW(ptRenderPos.y, _AspectHeight), 1.0f, ETA_LEFT, ETA_BOTTOM, 2.0f, Color, BackColor, strLine.c_str());
			}
			else
			{
				font->DrawStringC2D(TEXTINDEX::BEGIN_CHATMESSAGES + pMsgText->_RenderIndex, MU_TO_WINDOW(ptRenderPos.x, _AspectWidth), MU_TO_WINDOW(ptRenderPos.y, _AspectHeight), 1.0f, ETA_LEFT, ETA_BOTTOM, 2.0f, Color, BackColor, pMsgText->GetText().c_str());
			}
		}
	}
}

void UIChatWindow::DrawFrame()
{
	if (_ShowFrame == false) return;

	_ResizeSlide->Draw();

	_ScrollBar->Draw();
}

UIBase *UIChatWindow::GetFocus(mu_int32 mouseX, mu_int32 mouseY)
{
	UIBase *focus = UIBase::GetFocus(mouseX, mouseY);

	if (focus != nullptr)
	{
		if(_ShowFrame == true)
		{
			UIBase *object = _ResizeSlide->GetFocus(mouseX, mouseY);

			if (object != nullptr)
			{
				return object;
			}

			object = _ScrollBar->GetFocus(mouseX, mouseY);

			if (object != nullptr)
			{
				return object;
			}
		}

		if (_ShowFrame == true)
		{
			return focus;
		}
	}

	return nullptr;
}

mu_uint32 UIChatWindow::OnPreWindowMessage(const SDL_Event *event, mu_size *result)
{
	if (_ShowFrame == true)
	{
		UIBase *focusObject = _SceneManager->GetFocused();
		if (focusObject == this ||
			(focusObject != nullptr &&
			focusObject->_Parent == this))
		{
			mu_int32 mouseY = MU_INPUT->GetMouseY();

			if (mouseY >= _ScrollBar->_NativeY &&
				mouseY <= _ScrollBar->_NativeY + _ScrollBar->_Height)
			{
				switch (event->type)
				{
				case SDL_MOUSEWHEEL:
					{
						if (_ScrollBar->GetState() != UIScrollBar::UI_DISABLED)
						{
							mu_float mouseWheel = event->wheel.y;
							mu_float diffValue = mouseWheel * _ScrollBar->_DiffValue * -0.03f;
							_ScrollBar->ConfigureScrollInfo(_ScrollBar->_MinValue, _ScrollBar->_MaxValue, _ScrollBar->_CurrentValue + diffValue);
							this->OnScrolling();
						}
					}
					return GFX_MESSAGE_RETURN::GFX_MESSAGE_ACCEPT;
				}
			}
		}
	}

	return GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE;
}

void UIChatWindow::SetBackAlphaAuto()
{
	_BackAlpha += 0.2f;

	if (_BackAlpha > 0.9f)
	{
		_BackAlpha = 0.2f;
	}
}

void UIChatWindow::SetBackAlpha(mu_float Alpha)
{
	_BackAlpha = Alpha;
}

mu_float UIChatWindow::GetBackAlpha() const
{
	return _BackAlpha;
}

UIChatWindow::messages_vector *UIChatWindow::GetMessages(MESSAGE_TYPE messageType)
{
	return &_MessagesList[messageType];
}

void UIChatWindow::ChangeMessage(MESSAGE_TYPE messageType)
{
	_CurrentMessageType = messageType;
	_CurrentEndLine = GetMessages(GetCurrentMessageType())->size();
}

MESSAGE_TYPE UIChatWindow::GetCurrentMessageType()
{
	return _CurrentMessageType;
}

mu_int32 UIChatWindow::GetCurrentRenderEndLine()
{
	return _CurrentEndLine;
}

void UIChatWindow::SetSizeAuto()
{
	SetNumberOfShowingLines(GetNumberOfShowingLines() + 3);
}

void UIChatWindow::SetNumberOfShowingLines(mu_int32 ShowingLines, EPoint *outBox)
{
	_ShowingLines = (ShowingLines / 3) * 3;

	if (_ShowingLines < 3 || _ShowingLines > 15)
		_ShowingLines = 3;

	if (_ShowingLines > GetCurrentRenderEndLine())
		Scrolling(_ShowingLines - 1);

	UpdateWindowSize();
	UpdateScrollPosition();

	if (outBox != nullptr)
	{
		outBox->x = WND_WIDTH;
		outBox->y = (SCROLL_MIDDLE_PART_HEIGHT * GetNumberOfShowingLines()) + (SCROLL_TOP_BOTTOM_PART_HEIGHT * 2) + (WND_TOP_BOTTOM_EDGE * 2);
	}
}

mu_int32 UIChatWindow::GetNumberOfShowingLines()
{
	return _ShowingLines;
}

mu_size UIChatWindow::GetNumberOfLines(MESSAGE_TYPE messageType)
{
	return _MessagesList[messageType].size();
}

void UIChatWindow::UpdateWindowSize()
{
	_ResizeSlide->SetCurrentSection(_ShowingLines / 3);

	SetPosition(_NativeX, _ResizeSlide->_NativeY, false);
	SetDimensions(_ViewWidth, (SCROLL_MIDDLE_PART_HEIGHT * _ShowingLines) + (SCROLL_TOP_BOTTOM_PART_HEIGHT * 2) + (WND_TOP_BOTTOM_EDGE * 2) + RESIZING_BTN_HEIGHT);

	mu_float RenderPosX = _ViewX;
	mu_float RenderPosY = _ViewY + RESIZING_BTN_HEIGHT;
	mu_float RenderWidth = _ViewWidth;
	mu_float RenderHeight = _ViewHeight - RESIZING_BTN_HEIGHT;

	_Background->SetPosition(_NativeX, _NativeY, false);
	_Background->SetDimensions(_Width, _Height, false);

	_ScrollBar->ChangePosition(RenderPosX + RenderWidth - SCROLL_BAR_WIDTH - WND_LEFT_RIGHT_EDGE - 4.0f, RenderPosY + WND_TOP_BOTTOM_EDGE);
	_ScrollBar->ConfigureMiddle(&INTERFACES::Data[INTERFACES::SCROLLBAR_MIDDLE], EBlendState::eAlphaSrc_InvAlphaSrc, SCROLLBAR_BACKWIDTH, _ShowingLines * SCROLL_MIDDLE_PART_HEIGHT);
	_ScrollBar->ConfigureScrollDimension(_ShowingLines * SCROLL_MIDDLE_PART_HEIGHT);
	_ScrollBar->ApplyChanges();
}

void UIChatWindow::UpdateScrollPosition()
{
	mu_float PosRate = 1.0f;

	if (GetNumberOfLines(GetCurrentMessageType()) > GetNumberOfShowingLines())
	{
		if ((mu_int32)GetNumberOfShowingLines() > GetCurrentRenderEndLine())
		{
			PosRate = 0.0f;
		}
		else
		{
			PosRate = (mu_float)(GetCurrentRenderEndLine() - GetNumberOfShowingLines())
				/ (mu_float)(GetNumberOfLines(GetCurrentMessageType()) - GetNumberOfShowingLines());
		}
	}

	_ScrollBar->ConfigureScrollInfo(0.0f, 1.0f, PosRate);
}

void UIChatWindow::ResizeSection()
{
	SetNumberOfShowingLines(_ResizeSlide->GetCurrentSection() * 3);
}

void UIChatWindow::OnScrolling()
{
	mu_float scrollRate = _ScrollBar->GetCurrentValue();
	Scrolling(GetNumberOfShowingLines() + EMath::Floor((mu_float)(GetNumberOfLines(GetCurrentMessageType()) - GetNumberOfShowingLines()) * scrollRate));
}

void UIChatWindow::Scrolling(mu_int32 RenderEndLine)
{
	messages_vector *messages = GetMessages(GetCurrentMessageType());

	if ((mu_int32)messages->size() <= _ShowingLines)
	{
		_CurrentEndLine = (mu_int32)messages->size();
	}
	else
	{
		if (RenderEndLine < _ShowingLines)
			_CurrentEndLine = _ShowingLines;
		else if (RenderEndLine >= (mu_int32)messages->size())
			_CurrentEndLine = (mu_int32)messages->size();
		else
			_CurrentEndLine = RenderEndLine;
	}
}

void UIChatWindow::RemoveFrontLine(MESSAGE_TYPE messageType)
{
	messages_vector* pvecMsgs = GetMessages(messageType);

	auto vi = pvecMsgs->begin();
	if (vi != pvecMsgs->end())
	{
		mu_int32 referCount = --(*vi)->_ReferCount;

		if (referCount == 0)
		{
			_FreeMessages.push_back((*vi)->_RenderIndex);
		}

		vi = pvecMsgs->erase(vi);
	}

	if (messageType == GetCurrentMessageType())
	{
		Scrolling(GetCurrentRenderEndLine());
	}
}

void UIChatWindow::AddText(const mu_string& strID, const mu_string& strText, MESSAGE_TYPE MsgType, MESSAGE_TYPE ErrMsgType)
{
	if (strID.empty() && strText.empty())
	{
		return;
	}

	if (GetNumberOfLines(MsgType) >= MAX_NUMBER_OF_LINES)
	{
		RemoveFrontLine(MsgType);
	}

	if (GetNumberOfLines(TYPE_ALL_MESSAGE) >= MAX_NUMBER_OF_LINES)
	{
		RemoveFrontLine(TYPE_ALL_MESSAGE);
	}

	if (_Filters.empty() == true)
	{
		ProcessAddText(strID, strText, MsgType, ErrMsgType);
	}
	else
	{
		if (MsgType != TYPE_CHAT_MESSAGE)
		{
			ProcessAddText(strID, strText, MsgType, ErrMsgType);
		}
		else if (CheckFilterText(strID) || CheckFilterText(strText))
		{
			ProcessAddText(strID, strText, MsgType, ErrMsgType);

			if (g_pOption->IsWhisperSound())
			{
				PlayBackground(SOUND_WHISPER);
			}
		}
	}
}

void UIChatWindow::ProcessAddText(const mu_string& strID, const mu_string& strText, MESSAGE_TYPE MsgType, MESSAGE_TYPE ErrMsgType)
{
	messages_vector* pvecMsgs = GetMessages(MsgType);

	mu_int32 nScrollLines = 0;
	if (strText.size() >= 20)
	{
		mu_string	strText1, strText2;
		SeparateText(strID, strText, strText1, strText2, MsgType);

		mu_int32 msgCount = 1;

		if (strText2.empty() == false)
		{
			msgCount = 2;
		}

		if (_FreeMessages.size() < msgCount)
		{
			return;
		}

		if (!strText1.empty())
		{
			mu_int32 messageIndex = _FreeMessages.front();
			UIChatMessageText* pMsgText = &_Messages[messageIndex];

			if (pMsgText->Create(strID, strText1, MsgType) == false)
			{
				return;
			}

			_FreeMessages.pop_front();
			pMsgText->_ReferCount += 2;
			pvecMsgs->push_back(pMsgText);
			_MessagesList[TYPE_ALL_MESSAGE].push_back(pMsgText);

			if ((MsgType == TYPE_ERROR_MESSAGE)
				&& (ErrMsgType != TYPE_ERROR_MESSAGE && ErrMsgType != TYPE_ALL_MESSAGE))
			{
				messages_vector* pErrvecMsgs = GetMessages(ErrMsgType);

				++pMsgText->_ReferCount;
				pErrvecMsgs->push_back(pMsgText);
			}

			if (GetCurrentMessageType() == TYPE_ALL_MESSAGE || GetCurrentMessageType() == MsgType)
			{
				++nScrollLines;
			}
		}

		if (!strText2.empty())
		{
			mu_int32 messageIndex = _FreeMessages.front();
			UIChatMessageText* pMsgText = &_Messages[messageIndex];

			if (pMsgText->Create(_T(""), strText2, MsgType) == true)
			{
				_FreeMessages.pop_front();
				pMsgText->_ReferCount += 2;
				pvecMsgs->push_back(pMsgText);
				_MessagesList[TYPE_ALL_MESSAGE].push_back(pMsgText);

				if ((MsgType == TYPE_ERROR_MESSAGE)
					&& (ErrMsgType != TYPE_ERROR_MESSAGE && ErrMsgType != TYPE_ALL_MESSAGE))
				{
					messages_vector* pErrvecMsgs = GetMessages(ErrMsgType);

					++pMsgText->_ReferCount;
					pErrvecMsgs->push_back(pMsgText);
				}

				if (GetCurrentMessageType() == TYPE_ALL_MESSAGE || GetCurrentMessageType() == MsgType)
				{
					++nScrollLines;
				}
			}
		}
	}
	else
	{
		mu_int32 messageIndex = _FreeMessages.front();
		UIChatMessageText* pMsgText = &_Messages[messageIndex];

		if (pMsgText->Create(strID, strText, MsgType) == false)
		{
			return;
		}

		_FreeMessages.pop_front();
		pMsgText->_ReferCount += 2;
		pvecMsgs->push_back(pMsgText);
		_MessagesList[TYPE_ALL_MESSAGE].push_back(pMsgText);

		if ((MsgType == TYPE_ERROR_MESSAGE)
			&& (ErrMsgType != TYPE_ERROR_MESSAGE && ErrMsgType != TYPE_ALL_MESSAGE))
		{
			messages_vector* pErrvecMsgs = GetMessages(ErrMsgType);

			++pMsgText->_ReferCount;
			pErrvecMsgs->push_back(pMsgText);
		}

		if (GetCurrentMessageType() == TYPE_ALL_MESSAGE || GetCurrentMessageType() == MsgType)
		{
			++nScrollLines;
		}
	}

	pvecMsgs = GetMessages(GetCurrentMessageType());

	if (nScrollLines > 0 && ((pvecMsgs->size() - _CurrentEndLine - nScrollLines) < 3))
		_CurrentEndLine = pvecMsgs->size();
	else if (!_ShowFrame)
		_CurrentEndLine = pvecMsgs->size();
}

mu_boolean UIChatWindow::CheckFilterText(const mu_string &text)
{
	mu_string tmp = text;
	std::transform(tmp.begin(), tmp.end(), tmp.begin(), mu_tolower);

	for (auto vi_filters = _Filters.begin(); vi_filters != _Filters.end(); ++vi_filters)
	{
		if (text.find(*vi_filters) != mu_string::npos)
		{
			return true;
		}
	}

	return false;
}

void UIChatWindow::AddFilterWord(const mu_string &word)
{
	if (_Filters.size() > 5)
		return;

	mu_string tmp = word;
	std::transform(tmp.begin(), tmp.end(), tmp.begin(), mu_tolower);

	for (auto vi_filters = _Filters.begin(); vi_filters != _Filters.end(); ++vi_filters)
	{
		if (0 == (*vi_filters).compare(tmp))
		{
			return;
		}
	}

	_Filters.push_back(tmp);
}

void UIChatWindow::SetFilterText(const mu_string &word)
{
	mu_boolean bPrevFilter = false;

	if (!_Filters.empty())
	{
		bPrevFilter = true;
		ResetFilter();
	}

	mu_tokenizer tokens(word, mu_separator(_T(" ")));

	if (tokens.begin() == tokens.end())
	{
		ResetFilter();
		AddText(_T(""), GlobalText[756], TYPE_SYSTEM_MESSAGE);
	}
	else
	{
		mu_uint32 count = 0;
		for (auto tok_iter = tokens.begin(); count < 5 && tok_iter != tokens.end(); ++tok_iter, ++count)
		{
			AddFilterWord(*tok_iter);
		}

		AddText(_T(""), GlobalText[755], TYPE_SYSTEM_MESSAGE);
	}
}

void UIChatWindow::ResetFilter()
{
	_Filters.clear();
}

void UIChatWindow::Clear(MESSAGE_TYPE messageType)
{
	messages_vector *messages = GetMessages(messageType);

	for (auto it = messages->begin(); it != messages->end(); it = messages->erase(it))
	{
		if (--(*it)->_ReferCount == 0)
		{
			_FreeMessages.push_back((*it)->_RenderIndex);
		}
	}

	if (messageType == GetCurrentMessageType())
	{
		_CurrentEndLine = 0;
	}
}

void UIChatWindow::ClearAll()
{
	for (mu_uint32 i = TYPE_ALL_MESSAGE; i < NUMBER_OF_TYPES; ++i)
	{
		Clear((MESSAGE_TYPE)i);
	}

	_CurrentEndLine = 0;
}

void UIChatWindow::ShowFrame()
{
	_ShowFrame = true;
}

void UIChatWindow::HideFrame()
{
	_ShowFrame = false;
}

mu_boolean UIChatWindow::IsShowFrame()
{
	return _ShowFrame;
}

void UIChatWindow::ShowChatLog()
{
	_ShowChatLog = true;

	messages_vector* messages = GetMessages(GetCurrentMessageType());

	_CurrentEndLine = messages->size();
}

void UIChatWindow::HideChatLog()
{
	_ShowChatLog = false;
}

mu_boolean UIChatWindow::CheckChatRedundancy(const mu_string& message, mu_int32 searchLine)
{
	messages_vector* messages = GetMessages(TYPE_ALL_MESSAGE);

	if (messages->empty()) return false;

	auto vri_msgs = messages->rbegin();
	for (mu_int32 i = 0; (i < searchLine) || (vri_msgs != messages->rend()); ++vri_msgs, ++i)
	{
		if (0 == (*vri_msgs)->GetText().compare(message)) return true;
	}

	return false;
}

void UIChatWindow::SeparateText(const mu_string &id, const mu_string &message, mu_string &text1, mu_string &text2, MESSAGE_TYPE messageType)
{
	EVector2 TextSize;
	mu_string idPart = id + _T(" : ");

	MUTextSimpleRender *font = messageType == TYPE_GM_MESSAGE ? FONT_BOLD : FONT_NORMAL;

	font->GetTextExtentPoint(idPart.c_str(), idPart.size(), ETA_LEFT, 1.0f, &TextSize);
	mu_float TextExtentWidth = MU_TO_VIEWPORT(TextSize.x, _AspectWidth);
	mu_float MaxFirstLineWidth = (mu_float)CLIENT_WIDTH - TextExtentWidth;

	font->GetTextExtentPoint(message.c_str(), message.size(), ETA_LEFT, 1.0f, &TextSize);
	TextExtentWidth = MU_TO_VIEWPORT(TextSize.x, _AspectWidth);

	if (TextExtentWidth > MaxFirstLineWidth)
	{
		mu_int32 prev_offset = 0;
		for (mu_int32 cur_offset = 0; cur_offset < (mu_int32)message.size(); )
		{
			prev_offset = cur_offset;
			cur_offset += 1;

			mu_string strTemp(message, 0, cur_offset + 1);
			font->GetTextExtentPoint(strTemp.c_str(), strTemp.size(), ETA_LEFT, 1.0f, &TextSize);
			TextExtentWidth = MU_TO_VIEWPORT(TextSize.x, _AspectWidth);

			if (TextExtentWidth > MaxFirstLineWidth && cur_offset != 0)
			{
				text1 = mu_string(message, 0, prev_offset);
				text2 = mu_string(message, prev_offset, message.size() - prev_offset);
				return;
			}
		}
	}
	else
	{
		text1 = message;
	}
}