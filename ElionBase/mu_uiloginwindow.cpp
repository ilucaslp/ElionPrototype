#include "stdafx.h"
#include "mu_uiloginwindow.h"

#define SCREEN_WIDTH 1024.0f
#define SCREEN_HEIGHT 768.0f
#define WINDOW_WIDTH 329.0f
#define WINDOW_HEIGHT 245.0f
#define WINDOW_POSX ((SCREEN_WIDTH - WINDOW_WIDTH) * 0.5f)
#define WINDOW_POSY ((SCREEN_HEIGHT - WINDOW_HEIGHT) * 2.0f / 3.0f)

UILoginWindow::UILoginWindow(UISceneManager *manager) : UIBase()
{
	if (manager != nullptr)
	{
		manager->AddGFxObject(this);
	}

	_SceneManager = manager;

	UIBase::SetAspectRatio(SCREEN_WIDTH, SCREEN_HEIGHT);
	UIBase::SetPosition(WINDOW_POSX, WINDOW_POSY);
	UIBase::SetDimensions(WINDOW_WIDTH, WINDOW_HEIGHT);

	SetGfxFlag(GFX_FLAG::GFX_HIDDEN);

	// Initialize
	{
		_BackFrame = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::LOGIN_BACKGROUND], SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_POSX, WINDOW_POSY, WINDOW_WIDTH, WINDOW_HEIGHT);
		_BackFrame->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

		_AccountBack = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::LOGIN_TEXT_BACKGROUND], SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_POSX + 109.0f, WINDOW_POSY + 106.0f, 156.0f, 23.0f);
		_AccountBack->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_PasswordBack = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::LOGIN_TEXT_BACKGROUND], SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_POSX + 109.0f, WINDOW_POSY + 131.0f, 156.0f, 23.0f);
		_PasswordBack->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		
		_AccountBox = new UIEditBox(LOGINUI::MESSAGE_LOGIN_ACCOUNTBOX, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_POSX + 109.0f + 9.0f, WINDOW_POSY + 106.0f + 4.0f, 156.0f - 9.0f, 23.0f - 6.0f);
		_AccountBox->SetLimitText(ACCOUNT_MAXIMUM);
		_AccountBox->SetTextColor(255, 230, 210, 255);
		_AccountBox->SetBackColor(0, 0, 0, 0);
		_AccountBox->SetHighlightTextColor(0, 0, 0, 255);
		_AccountBox->SetHighlightColor(255, 230, 210, 255);

		_PasswordBox = new UIEditBox(LOGINUI::MESSAGE_LOGIN_PASSWORDBOX, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_POSX + 109.0f + 9.0f, WINDOW_POSY + 131.0f + 4.0f, 156.0f - 9.0f, 23.0f - 6.0f);
		_PasswordBox->SetLimitText(PASSWORD_MAXIMUM);
		_PasswordBox->SetPasswordChar(_T('*'));
		_PasswordBox->SetTextColor(255, 230, 210, 255);
		_PasswordBox->SetBackColor(0, 0, 0, 0);
		_PasswordBox->SetHighlightTextColor(0, 0, 0, 255);
		_PasswordBox->SetHighlightColor(255, 230, 210, 255);

		_OkButton = new UIButton(LOGINUI::MESSAGE_LOGIN_OKBUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_POSX + 150.0f, WINDOW_POSY + 178.0f, 54.0f, 30.0f);
		_OkButton->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_OK], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(100, 100, 100, 255)));
		_OkButton->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_OK], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(255, 255, 255, 255)));
		_OkButton->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_OK], TUV_THREE_OVER_BEGIN, TUV_THREE_OVER_END, MU::MakeARGB(255, 255, 255, 255)));
		_OkButton->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_OK], TUV_THREE_PRESSED_BEGIN, TUV_THREE_PRESSED_END, MU::MakeARGB(255, 255, 255, 255)));

		_CancelButton = new UIButton(LOGINUI::MESSAGE_LOGIN_CANCELBUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_POSX + 211.0f, WINDOW_POSY + 178.0f, 54.0f, 30.0f);
		_CancelButton->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_CANCEL], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(100, 100, 100, 255)));
		_CancelButton->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_CANCEL], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(255, 255, 255, 255)));
		_CancelButton->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_CANCEL], TUV_THREE_OVER_BEGIN, TUV_THREE_OVER_END, MU::MakeARGB(255, 255, 255, 255)));
		_CancelButton->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_CANCEL], TUV_THREE_PRESSED_BEGIN, TUV_THREE_PRESSED_END, MU::MakeARGB(255, 255, 255, 255)));
	}
}

UILoginWindow::~UILoginWindow()
{

}

mu_uint32 UILoginWindow::OnPreWindowMessage(const SDL_Event *event, mu_size *result)
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
				case SDL_SCANCODE_RETURN:
				case SDL_SCANCODE_KP_ENTER:
					{
						if (_SceneManager->GetSelected() == _AccountBox)
						{
							_PasswordBox->GiveFocus();
							_PasswordBox->SelectAll();
						}
						else if (_SceneManager->GetSelected() == _PasswordBox)
						{
							_SceneManager->OnGFxMessagePush(_OkButton->_ObjectID, GFX_MESSAGE_TYPE::GFX_PRESSED, 0);
						}
						else
						{
							return GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE;
						}
					}
					return GFX_MESSAGE_RETURN::GFX_MESSAGE_ACCEPT;

				case SDL_SCANCODE_TAB:
					{
						if (_SceneManager->GetSelected() == _AccountBox)
						{
							_PasswordBox->GiveFocus();
							_PasswordBox->SelectAll();
						}
						else if (_SceneManager->GetSelected() == _PasswordBox)
						{
							_AccountBox->GiveFocus();
							_AccountBox->SelectAll();
						}
						else
						{
							return GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE;
						}
					}
					return GFX_MESSAGE_RETURN::GFX_MESSAGE_ACCEPT;
				}
			}
		}
		break;
	}

	return GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE;
}

void UILoginWindow::Update(mu_uint64 CurrentTime)
{
	_AccountBox->Update(CurrentTime);
	_PasswordBox->Update(CurrentTime);
}

void UILoginWindow::Draw()
{
	_BackFrame->Draw();
	_AccountBack->Draw();
	_PasswordBack->Draw();
	_OkButton->Draw();
	_CancelButton->Draw();
	_AccountBox->Draw();
	_PasswordBox->Draw();

	GROUP_INFO &group = g_pServerList->_ServerGroups[g_pServerList->_GroupSelected];
	SERVER_INFO &server = group.Servers[g_pServerList->_ServerSelected];

	mu_text textBuffer[512];
	mu_sprintf_s(textBuffer, mu_countof(textBuffer), _T("[") SPF_STRING _T("]"), server.ServerName.c_str());

	FONT_NORMAL->DrawStringC2D(TEXTINDEX::LOGIN_SERVERNAME,
		MU_TO_WINDOW(WINDOW_POSX + 111.0f, _AspectWidth),
		MU_TO_WINDOW(WINDOW_POSY + 80.0f, _AspectHeight),
		1.0f,
		ETA_LEFT, ETA_BOTTOM,
		0.0f,
		MU::MakeARGB(255, 255, 255, 255),
		MU::MakeARGB(0, 0, 0, 0),
		textBuffer);

	FONT_NORMAL->DrawStringC2D(TEXTINDEX::LOGIN_ACCOUNTTIP,
		MU_TO_WINDOW(WINDOW_POSX + 30.0f, _AspectWidth),
		MU_TO_WINDOW(WINDOW_POSY + 111.5f, _AspectHeight),
		1.0f,
		ETA_LEFT, ETA_BOTTOM,
		0.0f,
		MU::MakeARGB(255, 255, 255, 255),
		MU::MakeARGB(0, 0, 0, 0),
		GlobalText[450]);

	FONT_NORMAL->DrawStringC2D(TEXTINDEX::LOGIN_PASSWORDTIP,
		MU_TO_WINDOW(WINDOW_POSX + 30.0f, _AspectWidth),
		MU_TO_WINDOW(WINDOW_POSY + 137.5f, _AspectHeight),
		1.0f,
		ETA_LEFT, ETA_BOTTOM,
		0.0f,
		MU::MakeARGB(255, 255, 255, 255),
		MU::MakeARGB(0, 0, 0, 0),
		GlobalText[451]);
}

void UILoginWindow::PostDraw()
{
	_OkButton->PostDraw();
	_CancelButton->PostDraw();
}

UIBase *UILoginWindow::GetFocus(mu_int32 mouseX, mu_int32 mouseY)
{
	UIBase *focus = UIBase::GetFocus(mouseX, mouseY);

	if (focus != nullptr)
	{
		UIBase *object = _AccountBox->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}

		object = _PasswordBox->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}

		object = _OkButton->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}

		object = _CancelButton->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}
	}

	return focus;
}