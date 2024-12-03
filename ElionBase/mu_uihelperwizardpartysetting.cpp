#include "stdafx.h"
#include "mu_uihelperwizardpartysetting.h"
#include "mu_uihelperwizardpartysetting_defines.h"

UIHelperWizardPartySetting::UIHelperWizardPartySetting(UISceneManager *manager) : UIBase()
{
	if (manager != nullptr)
	{
		manager->AddGFxObject(this);
	}

	_SceneManager = manager;

	UIBase::SetAspectRatio(SCREEN_WIDTH, SCREEN_HEIGHT);
	UIBase::SetPosition(WINBOX_POSX, WINBOX_POSY);
	UIBase::SetDimensions(WINBOX_WIDTH, WINBOX_HEIGHT);

	SetGfxFlag(GFX_FLAG::GFX_NO_FOCUS | GFX_FLAG::GFX_HIDDEN);

	// Initialize
	{
		TMSGBOX_PREPARE(manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, WINBOX_POSX, WINBOX_POSY, WINBOX_WIDTH, WINBOX_HEIGHT);
		TGLAYOUT_PREPARE(BuffBack, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, BUFF_BACK_POSX, BUFF_BACK_POSY, BUFF_BACK_WIDTH, BUFF_BACK_HEIGHT);

		_BuffDiv = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::LOGIN_SERVER_EX01], SCREEN_WIDTH, SCREEN_HEIGHT, BUFF_DIV_POSX, BUFF_DIV_POSY, BUFF_DIV_WIDTH, BUFF_DIV_HEIGHT, EVector2(0.0f, 0.0f), EVector2(1.0f, 4.0f / 12.0f));
		_BuffDiv->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

		_EnableBuffCheckBox = new UIToggleButton(MESSAGEBOX::MESSAGE_HELPERPARTY_ENABLEBUFF_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, ENABLEBUFF_POSX, ENABLEBUFF_POSY, ENABLEBUFF_WIDTH, ENABLEBUFF_HEIGHT);
		_EnableBuffCheckBox->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_EnableBuffCheckBox->SetStateData(UIToggleButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::OPTION_CHECK], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(100, 100, 100, 255)));
		_EnableBuffCheckBox->SetStateData(UIToggleButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::OPTION_CHECK], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(200, 200, 200, 255)));
		_EnableBuffCheckBox->SetStateData(UIToggleButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::OPTION_CHECK], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_EnableBuffCheckBox->SetStateData(UIToggleButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::OPTION_CHECK], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(200, 200, 200, 255)));
		_EnableBuffCheckBox->SetStateData(UIToggleButton::UI_PRESSED_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::OPTION_CHECK], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));

		_OkButton = new UIButton(MESSAGEBOX::MESSAGE_HELPERPARTY_OKBUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, OK_POSX, OK_POSY, BUTTON_WIDTH, BUTTON_HEIGHT);
		_OkButton->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_OkButton->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_OK], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(100, 100, 100, 255)));
		_OkButton->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_OK], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(255, 255, 255, 255)));
		_OkButton->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_OK], TUV_THREE_OVER_BEGIN, TUV_THREE_OVER_END, MU::MakeARGB(255, 255, 255, 255)));
		_OkButton->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_OK], TUV_THREE_PRESSED_BEGIN, TUV_THREE_PRESSED_END, MU::MakeARGB(255, 255, 255, 255)));
	}
}

UIHelperWizardPartySetting::~UIHelperWizardPartySetting()
{

}

void UIHelperWizardPartySetting::Update(mu_uint64 CurrentTime)
{
}

void UIHelperWizardPartySetting::PostDraw()
{
	DrawFrame();
	DrawText();
	DrawButton();
}

void UIHelperWizardPartySetting::DrawFrame()
{
	TMSGBOX_RENDERBACK;
	TGLAYOUT_RENDERBACK(BuffBack);
	_BuffDiv->Draw();
	_EnableBuffCheckBox->Draw();
	TGLAYOUT_RENDERFRONT(BuffBack);
	TMSGBOX_RENDERFRONT;
}

void UIHelperWizardPartySetting::DrawText()
{
	mu_float screenX = _AspectWidth;
	mu_float screenY = _AspectHeight;
	mu_uint32 textColor = MU::MakeARGB(255, 255, 255, 255);
	mu_uint32 bgColor = MU::MakeARGB(0, 0, 0, 0);

	FONT_BOLD->DrawStringC2D(TEXTINDEX::HELPERPARTY_BUFF_TITLE,
		MU_TO_WINDOW(BUFF_TITLE_POSX, screenX),
		MU_TO_WINDOW(BUFF_TITLE_POSY, screenY),
		1.0f,
		ETA_CENTER,
		ETA_TOP,
		0.0f,
		textColor,
		bgColor,
		GlobalText[3549]);

	FONT_NORMAL->DrawStringC2D(TEXTINDEX::HELPERPARTY_ENABLEBUFF_LABEL,
		MU_TO_WINDOW(ENABLEBUFF_LABEL_POSX, screenX),
		MU_TO_WINDOW(ENABLEBUFF_LABEL_POSY, screenY),
		1.0f,
		ETA_LEFT,
		ETA_CENTER,
		0.0f,
		textColor,
		bgColor,
		GlobalText[3540]);
}

void UIHelperWizardPartySetting::DrawButton()
{
	_OkButton->Draw();
	_OkButton->PostDraw();
}

mu_uint32 UIHelperWizardPartySetting::OnPreKeyPressed(const SDL_Keysym &key)
{
	switch (key.scancode)
	{
	case SDL_SCANCODE_RETURN:
	case SDL_SCANCODE_KP_ENTER:
		{
			_SceneManager->OnGFxMessagePush(_OkButton->_ObjectID, GFX_MESSAGE_TYPE::GFX_PRESSED, 0);
		}
		return GFX_MESSAGE_RETURN::GFX_MESSAGE_ACCEPT;
	}

	return GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE;
}

UIBase *UIHelperWizardPartySetting::GetFocus(mu_int32 mouseX, mu_int32 mouseY)
{
	UIBase *focus = UIBase::GetFocus(mouseX, mouseY);

	if (focus != nullptr)
	{
		UIBase *object = _OkButton->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}

		object = _EnableBuffCheckBox->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}
	}

	return focus;
}

void UIHelperWizardPartySetting::Configure(HELPER_PARTYSETTING &PartySetting)
{
	_EnableBuffCheckBox->ChangeState(UIToggleButton::UI_DISABLED);
}