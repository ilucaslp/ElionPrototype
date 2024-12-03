#include "stdafx.h"
#include "mu_uihelperskillsetting.h"
#include "mu_uihelperskillsetting_defines.h"

UIHelperSkillSetting::UIHelperSkillSetting(UISceneManager *manager) : UIBase()
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
		TGLAYOUT_PREPARE(PreConBack, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, PRECON_BACK_POSX, PRECON_BACK_POSY, PRECON_BACK_WIDTH, PRECON_BACK_HEIGHT);
		TGLAYOUT_PREPARE(SubConBack, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, SUBCON_BACK_POSX, SUBCON_BACK_POSY, SUBCON_BACK_WIDTH, SUBCON_BACK_HEIGHT);

		_PreConDiv = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::LOGIN_SERVER_EX01], SCREEN_WIDTH, SCREEN_HEIGHT, PRECON_DIV_POSX, PRECON_DIV_POSY, PRECON_DIV_WIDTH, PRECON_DIV_HEIGHT, EVector2(0.0f, 0.0f), EVector2(1.0f, 4.0f / 12.0f));
		_PreConDiv->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_SubConDiv = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::LOGIN_SERVER_EX01], SCREEN_WIDTH, SCREEN_HEIGHT, SUBCON_DIV_POSX, SUBCON_DIV_POSY, SUBCON_DIV_WIDTH, SUBCON_DIV_HEIGHT, EVector2(0.0f, 0.0f), EVector2(1.0f, 4.0f / 12.0f));
		_SubConDiv->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

		_HuntingRange = new UIRadioButton(MESSAGEBOX::MESSAGE_HELPERSKILL_RANGE_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, HTRANGE_POSX, HTRANGE_POSY, HTRANGE_WIDTH, HTRANGE_HEIGHT);
		_HuntingRange->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_HuntingRange->SetStateData(UIRadioButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MACROUI_OPTIONBUTTON], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(200, 200, 200, 255)));
		_HuntingRange->SetStateData(UIRadioButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MACROUI_OPTIONBUTTON], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_HuntingRange->SetStateData(UIRadioButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MACROUI_OPTIONBUTTON], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(200, 200, 200, 255)));
		_HuntingRange->SetStateData(UIRadioButton::UI_PRESSED_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MACROUI_OPTIONBUTTON], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_HuntingRange->ChangeState(UIRadioButton::UI_PRESSED);
		_HuntingRange->SetMessageInheritor(this);

		_HuntingAttacker = new UIRadioButton(MESSAGEBOX::MESSAGE_HELPERSKILL_ATTACKER_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, HTATTACKER_POSX, HTATTACKER_POSY, HTATTACKER_WIDTH, HTATTACKER_HEIGHT);
		_HuntingAttacker->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_HuntingAttacker->SetStateData(UIRadioButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MACROUI_OPTIONBUTTON], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(200, 200, 200, 255)));
		_HuntingAttacker->SetStateData(UIRadioButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MACROUI_OPTIONBUTTON], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_HuntingAttacker->SetStateData(UIRadioButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MACROUI_OPTIONBUTTON], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(200, 200, 200, 255)));
		_HuntingAttacker->SetStateData(UIRadioButton::UI_PRESSED_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MACROUI_OPTIONBUTTON], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_HuntingAttacker->SetMessageInheritor(this);

		_TwoPlus = new UIRadioButton(MESSAGEBOX::MESSAGE_HELPERSKILL_TWOPLUS_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, TWOPLUS_POSX, TWOPLUS_POSY, TWOPLUS_WIDTH, TWOPLUS_HEIGHT);
		_TwoPlus->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_TwoPlus->SetStateData(UIRadioButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MACROUI_OPTIONBUTTON], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(200, 200, 200, 255)));
		_TwoPlus->SetStateData(UIRadioButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MACROUI_OPTIONBUTTON], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_TwoPlus->SetStateData(UIRadioButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MACROUI_OPTIONBUTTON], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(200, 200, 200, 255)));
		_TwoPlus->SetStateData(UIRadioButton::UI_PRESSED_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MACROUI_OPTIONBUTTON], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_TwoPlus->ChangeState(UIRadioButton::UI_PRESSED);
		_TwoPlus->SetMessageInheritor(this);

		_ThreePlus = new UIRadioButton(MESSAGEBOX::MESSAGE_HELPERSKILL_THREEPLUS_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, THREEPLUS_POSX, THREEPLUS_POSY, THREEPLUS_WIDTH, THREEPLUS_HEIGHT);
		_ThreePlus->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_ThreePlus->SetStateData(UIRadioButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MACROUI_OPTIONBUTTON], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(200, 200, 200, 255)));
		_ThreePlus->SetStateData(UIRadioButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MACROUI_OPTIONBUTTON], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_ThreePlus->SetStateData(UIRadioButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MACROUI_OPTIONBUTTON], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(200, 200, 200, 255)));
		_ThreePlus->SetStateData(UIRadioButton::UI_PRESSED_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MACROUI_OPTIONBUTTON], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_ThreePlus->SetMessageInheritor(this);

		_FourPlus = new UIRadioButton(MESSAGEBOX::MESSAGE_HELPERSKILL_FOURPLUS_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, FOURPLUS_POSX, FOURPLUS_POSY, FOURPLUS_WIDTH, FOURPLUS_HEIGHT);
		_FourPlus->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_FourPlus->SetStateData(UIRadioButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MACROUI_OPTIONBUTTON], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(200, 200, 200, 255)));
		_FourPlus->SetStateData(UIRadioButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MACROUI_OPTIONBUTTON], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_FourPlus->SetStateData(UIRadioButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MACROUI_OPTIONBUTTON], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(200, 200, 200, 255)));
		_FourPlus->SetStateData(UIRadioButton::UI_PRESSED_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MACROUI_OPTIONBUTTON], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_FourPlus->SetMessageInheritor(this);

		_FivePlus = new UIRadioButton(MESSAGEBOX::MESSAGE_HELPERSKILL_FIVEPLUS_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, FIVEPLUS_POSX, FIVEPLUS_POSY, FIVEPLUS_WIDTH, FIVEPLUS_HEIGHT);
		_FivePlus->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_FivePlus->SetStateData(UIRadioButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MACROUI_OPTIONBUTTON], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(200, 200, 200, 255)));
		_FivePlus->SetStateData(UIRadioButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MACROUI_OPTIONBUTTON], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_FivePlus->SetStateData(UIRadioButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MACROUI_OPTIONBUTTON], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(200, 200, 200, 255)));
		_FivePlus->SetStateData(UIRadioButton::UI_PRESSED_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MACROUI_OPTIONBUTTON], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_FivePlus->SetMessageInheritor(this);

		_OkButton = new UIButton(MESSAGEBOX::MESSAGE_HELPERSKILL_OKBUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, OK_POSX, OK_POSY, BUTTON_WIDTH, BUTTON_HEIGHT);
		_OkButton->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_OkButton->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_OK], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(100, 100, 100, 255)));
		_OkButton->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_OK], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(255, 255, 255, 255)));
		_OkButton->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_OK], TUV_THREE_OVER_BEGIN, TUV_THREE_OVER_END, MU::MakeARGB(255, 255, 255, 255)));
		_OkButton->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_OK], TUV_THREE_PRESSED_BEGIN, TUV_THREE_PRESSED_END, MU::MakeARGB(255, 255, 255, 255)));
	}
}

UIHelperSkillSetting::~UIHelperSkillSetting()
{

}

mu_boolean UIHelperSkillSetting::OnGFxMessage(mu_uint32 objectid, mu_uint32 message, mu_size message_data)
{
	switch (message)
	{
	case GFX_MESSAGE_TYPE::GFX_TOGGLE:
		{
			switch (objectid)
			{
			case MESSAGEBOX::MESSAGE_HELPERSKILL_RANGE_BUTTON:
			case MESSAGEBOX::MESSAGE_HELPERSKILL_ATTACKER_BUTTON:
				{
					UIRadioButton *radios[] =
					{
						_HuntingRange,
						_HuntingAttacker,
					};

					for (mu_uint32 n = 0; n < mu_countof(radios); ++n)
					{
						if (radios[n]->GetObjectID() != objectid)
						{
							radios[n]->ChangeState(UIRadioButton::UI_NORMAL);
						}
					}
				}
				break;

			case MESSAGEBOX::MESSAGE_HELPERSKILL_TWOPLUS_BUTTON:
			case MESSAGEBOX::MESSAGE_HELPERSKILL_THREEPLUS_BUTTON:
			case MESSAGEBOX::MESSAGE_HELPERSKILL_FOURPLUS_BUTTON:
			case MESSAGEBOX::MESSAGE_HELPERSKILL_FIVEPLUS_BUTTON:
				{
					UIRadioButton *radios[] =
					{
						_TwoPlus,
						_ThreePlus,
						_FourPlus,
						_FivePlus,
					};

					for (mu_uint32 n = 0; n < mu_countof(radios); ++n)
					{
						if (radios[n]->GetObjectID() != objectid)
						{
							radios[n]->ChangeState(UIRadioButton::UI_NORMAL);
						}
					}
				}
				break;
			}
		}
		break;
	}

	return false;
}

void UIHelperSkillSetting::Update(mu_uint64 CurrentTime)
{

}

void UIHelperSkillSetting::PostDraw()
{
	DrawFrame();
	DrawText();
	DrawButton();
}

void UIHelperSkillSetting::DrawFrame()
{
	TMSGBOX_RENDERBACK;
	TGLAYOUT_RENDERBACK(PreConBack);
	TGLAYOUT_RENDERBACK(SubConBack);
	_PreConDiv->Draw();
	_SubConDiv->Draw();
	TGLAYOUT_RENDERFRONT(PreConBack);
	TGLAYOUT_RENDERFRONT(SubConBack);
	TMSGBOX_RENDERFRONT;
}

void UIHelperSkillSetting::DrawText()
{
	mu_float screenX = _AspectWidth;
	mu_float screenY = _AspectHeight;
	mu_uint32 textColor = MU::MakeARGB(255, 255, 255, 255);
	mu_uint32 bgColor = MU::MakeARGB(0, 0, 0, 0);

	FONT_BOLD->DrawStringC2D(TEXTINDEX::HELPERSKILL_PRECON_TITLE,
		MU_TO_WINDOW(PRECON_TITLE_POSX, screenX),
		MU_TO_WINDOW(PRECON_TITLE_POSY, screenY),
		1.0f,
		ETA_CENTER,
		ETA_TOP,
		0.0f,
		textColor,
		bgColor,
		GlobalText[3543]);

	FONT_NORMAL->DrawStringC2D(TEXTINDEX::HELPERSKILL_HTRANGE_LABEL,
		MU_TO_WINDOW(HTRANGE_LABEL_POSX, screenX),
		MU_TO_WINDOW(HTRANGE_LABEL_POSY, screenY),
		1.0f,
		ETA_LEFT,
		ETA_CENTER,
		0.0f,
		textColor,
		bgColor,
		GlobalText[3555]);
	FONT_NORMAL->DrawStringC2D(TEXTINDEX::HELPERSKILL_HTATTACKER_LABEL,
		MU_TO_WINDOW(HTATTACKER_LABEL_POSX, screenX),
		MU_TO_WINDOW(HTATTACKER_LABEL_POSY, screenY),
		1.0f,
		ETA_LEFT,
		ETA_CENTER,
		0.0f,
		textColor,
		bgColor,
		GlobalText[3556]);

	FONT_BOLD->DrawStringC2D(TEXTINDEX::HELPERSKILL_SUBCON_TITLE,
		MU_TO_WINDOW(SUBCON_TITLE_POSX, screenX),
		MU_TO_WINDOW(SUBCON_TITLE_POSY, screenY),
		1.0f,
		ETA_CENTER,
		ETA_TOP,
		0.0f,
		textColor,
		bgColor,
		GlobalText[3544]);

	FONT_NORMAL->DrawStringC2D(TEXTINDEX::HELPERSKILL_TWOPLUS_LABEL,
		MU_TO_WINDOW(TWOPLUS_LABEL_POSX, screenX),
		MU_TO_WINDOW(TWOPLUS_LABEL_POSY, screenY),
		1.0f,
		ETA_LEFT,
		ETA_CENTER,
		0.0f,
		textColor,
		bgColor,
		GlobalText[3557]);
	FONT_NORMAL->DrawStringC2D(TEXTINDEX::HELPERSKILL_THREEPLUS_LABEL,
		MU_TO_WINDOW(THREEPLUS_LABEL_POSX, screenX),
		MU_TO_WINDOW(THREEPLUS_LABEL_POSY, screenY),
		1.0f,
		ETA_LEFT,
		ETA_CENTER,
		0.0f,
		textColor,
		bgColor,
		GlobalText[3558]);
	FONT_NORMAL->DrawStringC2D(TEXTINDEX::HELPERSKILL_FOURPLUS_LABEL,
		MU_TO_WINDOW(FOURPLUS_LABEL_POSX, screenX),
		MU_TO_WINDOW(FOURPLUS_LABEL_POSY, screenY),
		1.0f,
		ETA_LEFT,
		ETA_CENTER,
		0.0f,
		textColor,
		bgColor,
		GlobalText[3559]);
	FONT_NORMAL->DrawStringC2D(TEXTINDEX::HELPERSKILL_FIVEPLUS_LABEL,
		MU_TO_WINDOW(FIVEPLUS_LABEL_POSX, screenX),
		MU_TO_WINDOW(FIVEPLUS_LABEL_POSY, screenY),
		1.0f,
		ETA_LEFT,
		ETA_CENTER,
		0.0f,
		textColor,
		bgColor,
		GlobalText[3560]);
}

void UIHelperSkillSetting::DrawButton()
{
	_HuntingRange->Draw();
	_HuntingAttacker->Draw();

	_TwoPlus->Draw();
	_ThreePlus->Draw();
	_FourPlus->Draw();
	_FivePlus->Draw();

	_OkButton->Draw();
	_OkButton->PostDraw();
}

mu_uint32 UIHelperSkillSetting::OnPreKeyPressed(const SDL_Keysym &key)
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

UIBase *UIHelperSkillSetting::GetFocus(mu_int32 mouseX, mu_int32 mouseY)
{
	UIBase *focus = UIBase::GetFocus(mouseX, mouseY);

	if (focus != nullptr)
	{
		UIBase *object = _OkButton->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}

		object = _HuntingRange->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}

		object = _HuntingAttacker->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}

		object = _TwoPlus->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}

		object = _ThreePlus->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}

		object = _FourPlus->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}

		object = _FivePlus->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}
	}

	return focus;
}

void UIHelperSkillSetting::Configure(mu_uint8 ActIndex, HELPER_SKILLSETTING &SkillSetting)
{
	_ActIndex = ActIndex;

	UIRadioButton *huntingTypeRadios[] =
	{
		g_pHelperSkillSetting->_HuntingRange,
		g_pHelperSkillSetting->_HuntingAttacker,
	};

	for (mu_uint32 n = HELPER_SKILLSETTING::HSS_CON_BEGIN; n < mu_countof(huntingTypeRadios); ++n)
	{
		if(n == SkillSetting.AttackType)
		{
			huntingTypeRadios[n]->ChangeState(UIRadioButton::UI_PRESSED);
		}
		else
		{
			huntingTypeRadios[n]->ChangeState(UIRadioButton::UI_NORMAL);
		}
	}

	UIRadioButton *mobsTypeRadios[] =
	{
		g_pHelperSkillSetting->_TwoPlus,
		g_pHelperSkillSetting->_ThreePlus,
		g_pHelperSkillSetting->_FourPlus,
		g_pHelperSkillSetting->_FivePlus,
	};

	for (mu_uint32 n = HELPER_SKILLSETTING::HSS_MOB_BEGIN; n < mu_countof(mobsTypeRadios); ++n)
	{
		if (n == SkillSetting.MobsCountType)
		{
			mobsTypeRadios[n]->ChangeState(UIRadioButton::UI_PRESSED);
		}
		else
		{
			mobsTypeRadios[n]->ChangeState(UIRadioButton::UI_NORMAL);
		}
	}
}