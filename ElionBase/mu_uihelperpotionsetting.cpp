#include "stdafx.h"
#include "mu_uihelperpotionsetting.h"
#include "mu_uihelperpotionsetting_defines.h"

UIHelperPotionSetting::UIHelperPotionSetting(UISceneManager *manager) : UIBase()
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
		TGLAYOUT_PREPARE(MainBack, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, MAIN_BACK_POSX, MAIN_BACK_POSY, MAIN_BACK_WIDTH, MAIN_BACK_HEIGHT);
		
		_MainDiv = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::LOGIN_SERVER_EX01], SCREEN_WIDTH, SCREEN_HEIGHT, MAIN_DIV_POSX, MAIN_DIV_POSY, MAIN_DIV_WIDTH, MAIN_DIV_HEIGHT, EVector2(0.0f, 0.0f), EVector2(1.0f, 4.0f / 12.0f));
		_MainDiv->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

		_MinLifeSlider = new UISliderBar(MESSAGEBOX::MESSAGE_HELPERPOTION_MINLIFE_SLIDER, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, MINLIFE_POSX, MINLIFE_POSY, MINLIFE_WIDTH, MINLIFE_HEIGHT, UISliderBar::ORI_HORIZONTAL);
		_MinLifeSlider->ConfigureBack(&INTERFACES::Data[INTERFACES::OPTION_VOLUME_BACK], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f));
		_MinLifeSlider->SetLockUV(false);
		_MinLifeSlider->SetStateData(UISliderBar::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::OPTION_VOLUME_COLOR], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(100, 100, 100, 255)));
		_MinLifeSlider->SetStateData(UISliderBar::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::OPTION_VOLUME_COLOR], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_MinLifeSlider->SetCurrentValue(1.0f);
		
		_OkButton = new UIButton(MESSAGEBOX::MESSAGE_HELPERPOTION_OKBUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, OK_POSX, OK_POSY, BUTTON_WIDTH, BUTTON_HEIGHT);
		_OkButton->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_OkButton->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_OK], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(100, 100, 100, 255)));
		_OkButton->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_OK], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(255, 255, 255, 255)));
		_OkButton->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_OK], TUV_THREE_OVER_BEGIN, TUV_THREE_OVER_END, MU::MakeARGB(255, 255, 255, 255)));
		_OkButton->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_OK], TUV_THREE_PRESSED_BEGIN, TUV_THREE_PRESSED_END, MU::MakeARGB(255, 255, 255, 255)));
	}
}

UIHelperPotionSetting::~UIHelperPotionSetting()
{

}

void UIHelperPotionSetting::Update(mu_uint64 CurrentTime)
{

}

void UIHelperPotionSetting::PostDraw()
{
	DrawFrame();
	DrawText();
	DrawButton();
}

void UIHelperPotionSetting::DrawFrame()
{
	TMSGBOX_RENDERBACK;
	TGLAYOUT_RENDERBACK(MainBack);
	_MainDiv->Draw();
	_MinLifeSlider->Draw();
	TGLAYOUT_RENDERFRONT(MainBack);
	TMSGBOX_RENDERFRONT;
}

void UIHelperPotionSetting::DrawText()
{
	mu_float screenX = _AspectWidth;
	mu_float screenY = _AspectHeight;
	mu_uint32 textColor = MU::MakeARGB(255, 255, 255, 255);
	mu_uint32 bgColor = MU::MakeARGB(0, 0, 0, 0);

	FONT_BOLD->DrawStringC2D(TEXTINDEX::HELPERPOTION_MAIN_TITLE,
		MU_TO_WINDOW(MAIN_TITLE_POSX, screenX),
		MU_TO_WINDOW(MAIN_TITLE_POSY, screenY),
		1.0f,
		ETA_CENTER,
		ETA_TOP,
		0.0f,
		textColor,
		bgColor,
		GlobalText[3545]);

	mu_text textBuffer[512];
	mu_sprintf_s(textBuffer, mu_countof(textBuffer), _T("%d%%"), (mu_int32)(_MinLifeSlider->GetCurrentValue() * 100.0f));

	FONT_NORMAL->DrawStringC2D(TEXTINDEX::HELPERPOTION_MINLIFE_LABEL,
		MU_TO_WINDOW(MINLIFE_LABEL_POSX, screenX),
		MU_TO_WINDOW(MINLIFE_LABEL_POSY, screenY),
		1.0f,
		ETA_LEFT,
		ETA_CENTER,
		0.0f,
		textColor,
		bgColor,
		textBuffer);

	FONT_NORMAL->DrawStringC2D(TEXTINDEX::HELPERPOTION_MINLIFE_LABEL,
		MU_TO_WINDOW(MINLIFE_LABEL2_POSX, screenX),
		MU_TO_WINDOW(MINLIFE_LABEL2_POSY, screenY),
		1.0f,
		ETA_CENTER,
		ETA_BOTTOM,
		0.0f,
		textColor,
		bgColor,
		GlobalText[3547]);
}

void UIHelperPotionSetting::DrawButton()
{
	_OkButton->Draw();
	_OkButton->PostDraw();
}

mu_uint32 UIHelperPotionSetting::OnPreKeyPressed(const SDL_Keysym &key)
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

UIBase *UIHelperPotionSetting::GetFocus(mu_int32 mouseX, mu_int32 mouseY)
{
	UIBase *focus = UIBase::GetFocus(mouseX, mouseY);

	if (focus != nullptr)
	{
		UIBase *object = _OkButton->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}

		object = _MinLifeSlider->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}
	}

	return focus;
}

void UIHelperPotionSetting::Configure(HELPER_POTIONSETTING &PotionSetting)
{
	_MinLifeSlider->SetCurrentValue(PotionSetting.MinLifePercent);
}