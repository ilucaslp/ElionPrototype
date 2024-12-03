#include "stdafx.h"
#include "mu_uichatinput.h"

#define SCREEN_WIDTH 640.0f
#define SCREEN_HEIGHT 480.0f

const mu_float FramePosX = 0.0f;
const mu_float FramePosY = 382.0f;
const mu_float FrameWidth = 254.0f;
const mu_float FrameHeight = 47.0f;

static mu_unicode *ChatBuffer = nullptr;
static mu_uint32 ChatBufferLength = 0;
static mu_unicode *WhisperBuffer = nullptr;
static mu_uint32 WhisperBufferLength = 0;

UIChatInput::UIChatInput(UISceneManager *manager) : UIBase()
{
	if (manager != nullptr)
	{
		manager->AddGFxObject(this);
	}

	_SceneManager = manager;

	UIBase::SetAspectRatio(SCREEN_WIDTH, SCREEN_HEIGHT);
	UIBase::SetPosition(FramePosX, FramePosY);
	UIBase::SetDimensions(FrameWidth, FrameHeight);

	SetGfxFlag(GFX_FLAG::GFX_HIDDEN | GFX_FLAG::GFX_FORCEMESSAGE);

	// Initialize
	{
		_Background = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::CHATFRAME_INPUT_BACK], SCREEN_WIDTH, SCREEN_HEIGHT, FramePosX, FramePosY, FrameWidth, FrameHeight);
		_Background->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

		_ChatTypeButton = new UIRadioButton(GAMEUI::MESSAGE_CHATINPUT_CHATTYPE_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, FramePosX, FramePosY, 27.0f, 26.0f);
		_ChatTypeButton->SetTooltip(FONT_NORMAL, GlobalText[1681], MU::MakeARGB(255, 255, 255, 255), MU::MakeARGB(0, 0, 0, 180), true);
		_ChatTypeButton->SetStateData(UIRadioButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::CHATFRAME_INPUT_NORMAL_ON], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_ChatTypeButton->SetStateData(UIRadioButton::UI_PRESSED_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::CHATFRAME_INPUT_NORMAL_ON], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_ChatTypeButton->ChangeState(UIRadioButton::UI_PRESSED);

		_PartyTypeButton = new UIRadioButton(GAMEUI::MESSAGE_CHATINPUT_PARTYTYPE_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, FramePosX + 27.0f, FramePosY, 27.0f, 26.0f);
		_PartyTypeButton->SetTooltip(FONT_NORMAL, GlobalText[1682], MU::MakeARGB(255, 255, 255, 255), MU::MakeARGB(0, 0, 0, 180), true);
		_PartyTypeButton->SetStateData(UIRadioButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::CHATFRAME_INPUT_PARTY_ON], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_PartyTypeButton->SetStateData(UIRadioButton::UI_PRESSED_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::CHATFRAME_INPUT_PARTY_ON], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));

		_GuildTypeButton = new UIRadioButton(GAMEUI::MESSAGE_CHATINPUT_GUILDTYPE_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, FramePosX + 54.0f, FramePosY, 27.0f, 26.0f);
		_GuildTypeButton->SetTooltip(FONT_NORMAL, GlobalText[1683], MU::MakeARGB(255, 255, 255, 255), MU::MakeARGB(0, 0, 0, 180), true);
		_GuildTypeButton->SetStateData(UIRadioButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::CHATFRAME_INPUT_GUILD_ON], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_GuildTypeButton->SetStateData(UIRadioButton::UI_PRESSED_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::CHATFRAME_INPUT_GUILD_ON], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));

		_WhisperBlockButton = new UIToggleButton(GAMEUI::MESSAGE_CHATINPUT_WHISPERBLOCK_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, FramePosX + 87.0f, FramePosY, 27.0f, 26.0f);
		_WhisperBlockButton->SetTooltip(FONT_NORMAL, GlobalText[1684], MU::MakeARGB(255, 255, 255, 255), MU::MakeARGB(0, 0, 0, 180), true);
		_WhisperBlockButton->SetStateData(UIToggleButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::CHATFRAME_INPUT_WHISPER_ON], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_WhisperBlockButton->SetStateData(UIToggleButton::UI_PRESSED_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::CHATFRAME_INPUT_WHISPER_ON], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));

		_ChatSystemButton = new UIToggleButton(GAMEUI::MESSAGE_CHATINPUT_SYSTEM_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, FramePosX + 114.0f, FramePosY, 27.0f, 26.0f);
		_ChatSystemButton->SetTooltip(FONT_NORMAL, GlobalText[1685], MU::MakeARGB(255, 255, 255, 255), MU::MakeARGB(0, 0, 0, 180), true);
		_ChatSystemButton->SetStateData(UIToggleButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::CHATFRAME_INPUT_SYSTEM_ON], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_ChatSystemButton->SetStateData(UIToggleButton::UI_PRESSED_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::CHATFRAME_INPUT_SYSTEM_ON], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));

		_ChatLogButton = new UIToggleButton(GAMEUI::MESSAGE_CHATINPUT_LOG_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, FramePosX + 141.0f, FramePosY, 27.0f, 26.0f);
		_ChatLogButton->SetTooltip(FONT_NORMAL, GlobalText[750], MU::MakeARGB(255, 255, 255, 255), MU::MakeARGB(0, 0, 0, 180), true);
		_ChatLogButton->SetStateData(UIToggleButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::CHATFRAME_INPUT_LOG_ON], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_ChatLogButton->SetStateData(UIToggleButton::UI_PRESSED_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::CHATFRAME_INPUT_LOG_ON], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_ChatLogButton->ChangeState(UIToggleButton::UI_PRESSED);

		_ChatFrameButton = new UIToggleButton(GAMEUI::MESSAGE_CHATINPUT_FRAME_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, FramePosX + 173.0f, FramePosY, 27.0f, 26.0f);
		_ChatFrameButton->SetTooltip(FONT_NORMAL, GlobalText[1686], MU::MakeARGB(255, 255, 255, 255), MU::MakeARGB(0, 0, 0, 180), true);
		_ChatFrameButton->SetStateData(UIToggleButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::CHATFRAME_INPUT_FRAME_ON], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_ChatFrameButton->SetStateData(UIToggleButton::UI_PRESSED_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::CHATFRAME_INPUT_FRAME_ON], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));

		_SizeButton = new UIButton(GAMEUI::MESSAGE_CHATINPUT_SIZE_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, FramePosX + 200.0f, FramePosY, 27.0f, 26.0f);
		_SizeButton->SetTooltip(FONT_NORMAL, GlobalText[751], MU::MakeARGB(255, 255, 255, 255), MU::MakeARGB(0, 0, 0, 180), true);
		_SizeButton->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::CHATFRAME_INPUT_BTN_SIZE], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_SizeButton->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::CHATFRAME_INPUT_BTN_SIZE], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_SizeButton->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::CHATFRAME_INPUT_BTN_SIZE], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_SizeButton->ChangeState(UIButton::UI_DISABLED);

		_TransparencyButton = new UIButton(GAMEUI::MESSAGE_CHATINPUT_TRANSPARENCY_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, FramePosX + 227.0f, FramePosY, 27.0f, 26.0f);
		_TransparencyButton->SetTooltip(FONT_NORMAL, GlobalText[752], MU::MakeARGB(255, 255, 255, 255), MU::MakeARGB(0, 0, 0, 180), true);
		_TransparencyButton->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::CHATFRAME_INPUT_BTN_ALPHA], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_TransparencyButton->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::CHATFRAME_INPUT_BTN_ALPHA], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_TransparencyButton->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::CHATFRAME_INPUT_BTN_ALPHA], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_TransparencyButton->ChangeState(UIButton::UI_DISABLED);

		_ChatInput = new UIEditBox(GAMEUI::MESSAGE_CHATINPUT_CHAT_TEXTBOX, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, FramePosX + 72.0f, FramePosY + 32.0f, 176.0f, 14.0f);
		_ChatInput->SetLimitText(MAX_CHAT_SIZE);
		_ChatInput->SetTextColor(255, 230, 210, 255);
		_ChatInput->SetBackColor(0, 0, 0, 0);
		_ChatInput->SetHighlightTextColor(0, 0, 0, 255);
		_ChatInput->SetHighlightColor(255, 230, 210, 255);

		_WhisperInput = new UIEditBox(GAMEUI::MESSAGE_CHATINPUT_CHAT_TEXTBOX, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, FramePosX + 5.0f, FramePosY + 32.0f, 50.0f, 14.0f);
		_WhisperInput->SetLimitText(ACCOUNT_MAXIMUM);
		_WhisperInput->SetTextColor(200, 200, 200, 255);
		_WhisperInput->SetBackColor(0, 0, 0, 0);
		_WhisperInput->SetHighlightTextColor(0, 0, 0, 255);
		_WhisperInput->SetHighlightColor(200, 200, 200, 255);

		_InputMessageType = INPUT_CHAT_MESSAGE;
		_ChatHistoryIndex = 0;
		_WhisperHistoryIndex = 0;
	}
}

UIChatInput::~UIChatInput()
{
	MU_SAFE_FREE(ChatBuffer);
	MU_SAFE_FREE(WhisperBuffer);
}

mu_uint32 UIChatInput::OnPreWindowMessage(const SDL_Event *event, mu_size *result)
{
	switch (event->type)
	{
	case SDL_KEYDOWN:
		{
			if (MU_INPUT->IsShiftPressing() == false &&
				MU_INPUT->IsCtrlPressing() == false)
			{
				switch (event->key.keysym.scancode)
				{
				case SDL_SCANCODE_ESCAPE:
					{
						if (g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_CHATINPUTBOX) == true)
						{
							g_pNewUISystem->Hide(GAMEUI::INTERFACE_CHATINPUTBOX);
							PlayBackground(SOUND_CLICK01);
							return GFX_MESSAGE_RETURN::GFX_MESSAGE_BYPASS;
						}
					}
					break;

				case SDL_SCANCODE_RETURN:
				case SDL_SCANCODE_KP_ENTER:
					{
						if (g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_CHATINPUTBOX) == false)
						{
							if (InChaosCastle() == false ||
								g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_CHAOSCASTLE_TIME) == false)
							{
								g_pNewUISystem->Show(GAMEUI::INTERFACE_CHATINPUTBOX);
							}
						}
						else
						{
							mu_uint32 chatLength = _ChatInput->GetLength();
							mu_uint32 whisperLength = _WhisperInput->GetLength();

							if (chatLength + 2 > ChatBufferLength)
							{
								ChatBufferLength = chatLength + 2;
								ChatBuffer = (mu_unicode*)mu_realloc(ChatBuffer, ChatBufferLength * sizeof(mu_unicode));
							}

							if (whisperLength + 2 > WhisperBufferLength)
							{
								WhisperBufferLength = whisperLength + 2;
								WhisperBuffer = (mu_unicode*)mu_realloc(WhisperBuffer, WhisperBufferLength * sizeof(mu_unicode));
							}

							if (_InputMessageType == INPUT_MESSAGE_TYPE::INPUT_PARTY_MESSAGE)
							{
								ChatBuffer[0] = _T('~');
								_ChatInput->GetText(ChatBuffer + 1, ChatBufferLength - 1);
							}
							else if (_InputMessageType == INPUT_MESSAGE_TYPE::INPUT_GUILD_MESSAGE)
							{
								ChatBuffer[0] = _T('@');
								_ChatInput->GetText(ChatBuffer + 1, ChatBufferLength - 1);
							}
							else
							{
								_ChatInput->GetText(ChatBuffer, ChatBufferLength);
							}

							_WhisperInput->GetText(WhisperBuffer, WhisperBufferLength);

							if (chatLength > 0)
							{
								if (CheckCommand(ChatBuffer) == false)
								{
									if (CheckAbuseFilter(ChatBuffer))
									{
										mu_strcpy_s(ChatBuffer, ChatBufferLength, GlobalText[570]);
									}

									if (_WhisperInput->IsReadOnly() == false &&
										chatLength > 0 &&
										whisperLength > 0)
									{
										g_GameServerObject->SendChatWhisper(WhisperBuffer, ChatBuffer);
										//g_pChatListBox->AddText(Hero->ID, ChatBuffer, TYPE_WHISPER_MESSAGE);
										AddWhisperHistory(WhisperBuffer);
									}
									else
									{
										if (Hero->SafeZone ||
											(Hero->Helper.Type != MODEL_HELPER + 2
												&& Hero->Helper.Type != MODEL_HELPER + 3
												&& Hero->Helper.Type != MODEL_HELPER + 4
												&& Hero->Helper.Type != MODEL_HELPER + 37))
										{
											CheckChatText(ChatBuffer);
										}

										g_GameServerObject->SendChat(ChatBuffer);
										AddChatHistory(ChatBuffer);
									}
								}
							}

							_ChatInput->SetText(_T(""));
							_ChatHistoryIndex = 0;
							_WhisperHistoryIndex = 0;

							g_pNewUISystem->Hide(GAMEUI::INTERFACE_CHATINPUTBOX);
						}
					}
					return GFX_MESSAGE_RETURN::GFX_MESSAGE_BYPASS;

				case SDL_SCANCODE_F2:
					{
						mu_uint32 messageType = g_pChatListBox->GetCurrentMessageType();
						g_pNewUISystem->ToggleChatMessageType(messageType != TYPE_WHISPER_MESSAGE ? TYPE_WHISPER_MESSAGE : TYPE_ALL_MESSAGE);
						PlayBackground(SOUND_CLICK01);
					}
					return GFX_MESSAGE_RETURN::GFX_MESSAGE_BYPASS;

				case SDL_SCANCODE_F3:
					{
						mu_boolean isReadOnly = _WhisperInput->IsReadOnly();

						if (isReadOnly)
						{
							_WhisperInput->SetReadOnly(false);
							_WhisperInput->SetTextColor(200, 200, 200, 255);
						}
						else
						{
							_WhisperInput->SetReadOnly(true);
							_WhisperInput->SetTextColor(255, 255, 255, 100);

							if (IsGfxFlag(GFX_FLAG::GFX_HIDDEN) == false)
							{
								_ChatInput->GiveFocus();
							}
						}
					}
					return GFX_MESSAGE_RETURN::GFX_MESSAGE_BYPASS;

				case SDL_SCANCODE_F4:
					{
						if (g_pChatListBox->IsShowFrame() == true)
						{
							g_pNewUISystem->UpdateChatWindowSize();
							PlayBackground(SOUND_CLICK01);
						}
					}
					return GFX_MESSAGE_RETURN::GFX_MESSAGE_BYPASS;

				case SDL_SCANCODE_F5:
					{
						if (g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_CHATINPUTBOX) == true)
						{
							g_pNewUISystem->ToggleChatFrame(_ChatFrameButton->IsPressed());
							PlayBackground(SOUND_CLICK01);
							return GFX_MESSAGE_RETURN::GFX_MESSAGE_BYPASS;
						}
					}
					break;

				case SDL_SCANCODE_PAGEUP:
					{
						if (g_pChatListBox->IsShowFrame())
						{
							g_pChatListBox->Scrolling(g_pChatListBox->GetCurrentRenderEndLine() - g_pChatListBox->GetNumberOfShowingLines());
							return GFX_MESSAGE_RETURN::GFX_MESSAGE_BYPASS;
						}
					}
					break;

				case SDL_SCANCODE_PAGEDOWN:
					{
						if (g_pChatListBox->IsShowFrame())
						{
							g_pChatListBox->Scrolling(g_pChatListBox->GetCurrentRenderEndLine() + g_pChatListBox->GetNumberOfShowingLines());
							return GFX_MESSAGE_RETURN::GFX_MESSAGE_BYPASS;
						}
					}
					break;

				case SDL_SCANCODE_UP:
					{
						UIBase *selected = _SceneManager->GetSelected();
						if (selected == _ChatInput)
						{
							if (_ChatHistory.empty() == false)
							{
								if (_ChatHistoryIndex > 0)
								{
									--_ChatHistoryIndex;
								}
								else
								{
									_ChatHistoryIndex = _ChatHistory.size() - 1;
								}

								_ChatInput->SetText(_ChatHistory[_ChatHistoryIndex].c_str());
							}
						}
						else if (selected == _WhisperInput)
						{
							if (_WhisperInput->IsReadOnly() == false &&
								_WhisperHistory.empty() == false)
							{
								if (_WhisperHistoryIndex > 0)
								{
									--_WhisperHistoryIndex;
								}
								else
								{
									_WhisperHistoryIndex = _WhisperHistory.size() - 1;
								}

								_WhisperInput->SetText(_WhisperHistory[_WhisperHistoryIndex].c_str());
							}
						}
					}
					return GFX_MESSAGE_RETURN::GFX_MESSAGE_BYPASS;

				case SDL_SCANCODE_DOWN:
					{
						UIBase *selected = _SceneManager->GetSelected();
						if (selected == _ChatInput)
						{
							if (_ChatHistory.empty() == false)
							{
								if (_ChatHistoryIndex < _ChatHistory.size() - 1)
								{
									++_ChatHistoryIndex;
								}
								else
								{
									_ChatHistoryIndex = 0;
								}

								_ChatInput->SetText(_ChatHistory[_ChatHistoryIndex].c_str());
							}
						}
						else if (selected == _WhisperInput)
						{
							if (_WhisperInput->IsReadOnly() == false &&
								_WhisperHistory.empty() == false)
							{
								if (_WhisperHistoryIndex < _WhisperHistory.size() - 1)
								{
									++_WhisperHistoryIndex;
								}
								else
								{
									_WhisperHistoryIndex = 0;
								}

								_WhisperInput->SetText(_WhisperHistory[_WhisperHistoryIndex].c_str());
							}
						}
					}
					return GFX_MESSAGE_RETURN::GFX_MESSAGE_BYPASS;

				case SDL_SCANCODE_TAB:
					{
						if (_SceneManager->GetSelected() == _WhisperInput)
						{
							_ChatInput->GiveFocus();
							_ChatInput->SelectAll();
						}
						else if (_SceneManager->GetSelected() == _ChatInput)
						{
							_WhisperInput->GiveFocus();
							_WhisperInput->SelectAll();
						}
						else
						{
							return GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE;
						}
					}
					return GFX_MESSAGE_RETURN::GFX_MESSAGE_BYPASS;
				}
			}
		}
		break;
	}

	return GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE;
}

mu_uint32 UIChatInput::OnWindowMessage(const SDL_Event *event, mu_size *result)
{
	return GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE;
}

void UIChatInput::Update(mu_uint64 CurrentTime)
{
	_ChatInput->Update(CurrentTime);
	_WhisperInput->Update(CurrentTime);
}

void UIChatInput::Draw()
{
	DrawBackground();
	DrawButtons();
}

void UIChatInput::PostDraw()
{
	PostDrawButtons();
}

void UIChatInput::DrawBackground()
{
	_Background->Draw();
}

void UIChatInput::DrawButtons()
{
	_ChatTypeButton->Draw();
	_PartyTypeButton->Draw();
	_GuildTypeButton->Draw();
	_WhisperBlockButton->Draw();
	_ChatSystemButton->Draw();
	_ChatLogButton->Draw();
	_ChatFrameButton->Draw();
	_SizeButton->Draw();
	_TransparencyButton->Draw();
	_ChatInput->Draw();
	_WhisperInput->Draw();
}

void UIChatInput::PostDrawButtons()
{
	_ChatTypeButton->PostDraw();
	_PartyTypeButton->PostDraw();
	_GuildTypeButton->PostDraw();
	_WhisperBlockButton->PostDraw();
	_ChatSystemButton->PostDraw();
	_ChatLogButton->PostDraw();
	_ChatFrameButton->PostDraw();
	_SizeButton->PostDraw();
	_TransparencyButton->PostDraw();
}

UIBase *UIChatInput::GetFocus(mu_int32 mouseX, mu_int32 mouseY)
{
	UIBase *focus = UIBase::GetFocus(mouseX, mouseY);

	if (focus != nullptr)
	{
		UIBase *object = _ChatTypeButton->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}

		object = _PartyTypeButton->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}

		object = _GuildTypeButton->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}

		object = _WhisperBlockButton->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}

		object = _ChatSystemButton->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}

		object = _ChatLogButton->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}

		object = _ChatFrameButton->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}

		object = _SizeButton->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}

		object = _TransparencyButton->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}

		object = _ChatInput->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}

		object = _WhisperInput->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}
	}

	return focus;
}

void UIChatInput::SetInputMessageType(mu_int32 inputMessageType)
{
	_InputMessageType = inputMessageType;
}

mu_int32 UIChatInput::GetInputMessageType()
{
	return _InputMessageType;
}

mu_boolean UIChatInput::IsBlockWhisper()
{
	mu_uint32 whisperState = _WhisperBlockButton->GetState();
	return (whisperState == UIToggleButton::UI_PRESSED ||
			whisperState == UIToggleButton::UI_PRESSED_OVER);
}

void UIChatInput::AddChatHistory(const mu_unicode *text)
{
	auto vi = std::find(_ChatHistory.begin(), _ChatHistory.end(), text);
	
	if (vi != _ChatHistory.end())
	{
		_ChatHistory.erase(vi);
	}
	else if (_ChatHistory.size() > MAX_CHAT_HISTORY)
	{
		_ChatHistory.erase(_ChatHistory.begin());
	}
	
	_ChatHistory.push_back(text);
}

void UIChatInput::RemoveChatHistory(mu_uint32 index)
{
	if (index < _ChatHistory.size())
	{
		_ChatHistory.erase(_ChatHistory.begin() + index);
	}
}

void UIChatInput::RemoveAllChatHistory()
{
	_ChatHistory.clear();
}

void UIChatInput::AddWhisperHistory(const mu_unicode *text)
{
	auto vi = std::find(_WhisperHistory.begin(), _WhisperHistory.end(), text);

	if (vi != _WhisperHistory.end())
	{
		_WhisperHistory.erase(vi);
	}
	else if (_WhisperHistory.size() > MAX_WHISPER_HISTORY)
	{
		_WhisperHistory.erase(_WhisperHistory.begin());
	}

	_WhisperHistory.push_back(text);
}

void UIChatInput::RemoveWhisperHistory(mu_uint32 index)
{
	if (index < _WhisperHistory.size())
	{
		_WhisperHistory.erase(_WhisperHistory.begin() + index);
	}
}

void UIChatInput::RemoveAllWhisperHistory()
{
	_WhisperHistory.clear();
}

void UIChatInput::SetWhisperID(const mu_text *whisperID)
{
	_WhisperInput->SetText(whisperID);
}