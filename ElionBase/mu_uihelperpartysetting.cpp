#include "stdafx.h"
#include "mu_uihelperpartysetting.h"
#include "mu_uihelperpartysetting_defines.h"

UIHelperPartySetting::UIHelperPartySetting(UISceneManager *manager) : UIBase()
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
		TGLAYOUT_PREPARE(HealBack, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, HEAL_BACK_POSX, HEAL_BACK_POSY, HEAL_BACK_WIDTH, HEAL_BACK_HEIGHT);
		TGLAYOUT_PREPARE(BuffBack, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, BUFF_BACK_POSX, BUFF_BACK_POSY, BUFF_BACK_WIDTH, BUFF_BACK_HEIGHT);

		_HealDiv = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::LOGIN_SERVER_EX01], SCREEN_WIDTH, SCREEN_HEIGHT, HEAL_DIV_POSX, HEAL_DIV_POSY, HEAL_DIV_WIDTH, HEAL_DIV_HEIGHT, EVector2(0.0f, 0.0f), EVector2(1.0f, 4.0f / 12.0f));
		_HealDiv->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_BuffDiv = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::LOGIN_SERVER_EX01], SCREEN_WIDTH, SCREEN_HEIGHT, BUFF_DIV_POSX, BUFF_DIV_POSY, BUFF_DIV_WIDTH, BUFF_DIV_HEIGHT, EVector2(0.0f, 0.0f), EVector2(1.0f, 4.0f / 12.0f));
		_BuffDiv->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

		_EnableHealCheckBox = new UIToggleButton(MESSAGEBOX::MESSAGE_HELPERPARTY_ENABLEHEAL_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, ENABLEHEAL_POSX, ENABLEHEAL_POSY, ENABLEHEAL_WIDTH, ENABLEHEAL_HEIGHT);
		_EnableHealCheckBox->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_EnableHealCheckBox->SetStateData(UIToggleButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::OPTION_CHECK], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(200, 200, 200, 255)));
		_EnableHealCheckBox->SetStateData(UIToggleButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::OPTION_CHECK], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_EnableHealCheckBox->SetStateData(UIToggleButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::OPTION_CHECK], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(200, 200, 200, 255)));
		_EnableHealCheckBox->SetStateData(UIToggleButton::UI_PRESSED_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::OPTION_CHECK], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));

		_MinHealSlider = new UISliderBar(MESSAGEBOX::MESSAGE_HELPERPARTY_MINHEAL_SLIDER, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, MINHEAL_POSX, MINHEAL_POSY, MINHEAL_WIDTH, MINHEAL_HEIGHT, UISliderBar::ORI_HORIZONTAL);
		_MinHealSlider->ConfigureBack(&INTERFACES::Data[INTERFACES::OPTION_VOLUME_BACK], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f));
		_MinHealSlider->SetLockUV(false);
		_MinHealSlider->SetStateData(UISliderBar::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::OPTION_VOLUME_COLOR], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(100, 100, 100, 255)));
		_MinHealSlider->SetStateData(UISliderBar::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::OPTION_VOLUME_COLOR], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_MinHealSlider->SetCurrentValue(1.0f);

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

UIHelperPartySetting::~UIHelperPartySetting()
{

}

void UIHelperPartySetting::Update(mu_uint64 CurrentTime)
{
}

void UIHelperPartySetting::PostDraw()
{
	DrawFrame();
	DrawText();
	DrawButton();
}

void UIHelperPartySetting::DrawFrame()
{
	TMSGBOX_RENDERBACK;
	TGLAYOUT_RENDERBACK(HealBack);
	TGLAYOUT_RENDERBACK(BuffBack);
	_HealDiv->Draw();
	_BuffDiv->Draw();
	_EnableHealCheckBox->Draw();
	_MinHealSlider->Draw();
	_EnableBuffCheckBox->Draw();
	TGLAYOUT_RENDERFRONT(HealBack);
	TGLAYOUT_RENDERFRONT(BuffBack);
	TMSGBOX_RENDERFRONT;
}

void UIHelperPartySetting::DrawText()
{
	mu_float screenX = _AspectWidth;
	mu_float screenY = _AspectHeight;
	mu_uint32 textColor = MU::MakeARGB(255, 255, 255, 255);
	mu_uint32 bgColor = MU::MakeARGB(0, 0, 0, 0);

	FONT_BOLD->DrawStringC2D(TEXTINDEX::HELPERPARTY_HEAL_TITLE,
		MU_TO_WINDOW(HEAL_TITLE_POSX, screenX),
		MU_TO_WINDOW(HEAL_TITLE_POSY, screenY),
		1.0f,
		ETA_CENTER,
		ETA_TOP,
		0.0f,
		textColor,
		bgColor,
		GlobalText[3548]);

	FONT_NORMAL->DrawStringC2D(TEXTINDEX::HELPERPARTY_ENABLEHEAL_LABEL,
		MU_TO_WINDOW(ENABLEHEAL_LABEL_POSX, screenX),
		MU_TO_WINDOW(ENABLEHEAL_LABEL_POSY, screenY),
		1.0f,
		ETA_LEFT,
		ETA_CENTER,
		0.0f,
		textColor,
		bgColor,
		GlobalText[3539]);

	mu_text textBuffer[512];
	mu_sprintf_s(textBuffer, mu_countof(textBuffer), _T("%d%%"), (mu_int32)(_MinHealSlider->GetCurrentValue() * 100.0f));

	FONT_NORMAL->DrawStringC2D(TEXTINDEX::HELPERPARTY_MINHEAL_LABEL,
		MU_TO_WINDOW(MINHEAL_LABEL_POSX, screenX),
		MU_TO_WINDOW(MINHEAL_LABEL_POSY, screenY),
		1.0f,
		ETA_LEFT,
		ETA_CENTER,
		0.0f,
		textColor,
		bgColor,
		textBuffer);

	FONT_NORMAL->DrawStringC2D(TEXTINDEX::HELPERPARTY_MINHEAL_LABEL2,
		MU_TO_WINDOW(MINHEAL_LABEL2_POSX, screenX),
		MU_TO_WINDOW(MINHEAL_LABEL2_POSY, screenY),
		1.0f,
		ETA_CENTER,
		ETA_BOTTOM,
		0.0f,
		textColor,
		bgColor,
		GlobalText[3550]);

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

void UIHelperPartySetting::DrawButton()
{
	_OkButton->Draw();
	_OkButton->PostDraw();
}

mu_uint32 UIHelperPartySetting::OnPreKeyPressed(const SDL_Keysym &key)
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

UIBase *UIHelperPartySetting::GetFocus(mu_int32 mouseX, mu_int32 mouseY)
{
	UIBase *focus = UIBase::GetFocus(mouseX, mouseY);

	if (focus != nullptr)
	{
		UIBase *object = _OkButton->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}

		object = _EnableHealCheckBox->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}

		object = _MinHealSlider->GetFocus(mouseX, mouseY);

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

void UIHelperPartySetting::Configure(HELPER_PARTYSETTING &PartySetting)
{
	_EnableHealCheckBox->ChangeState(PartySetting.EnableHealMin == true ? UIToggleButton::UI_PRESSED : UIToggleButton::UI_NORMAL);
	_MinHealSlider->SetCurrentValue(PartySetting.HealMin);
	_EnableBuffCheckBox->ChangeState(UIToggleButton::UI_DISABLED);
}