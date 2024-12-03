#include "stdafx.h"
#include "mu_uiheroposition.h"

#define SCREEN_WIDTH 640.0f
#define SCREEN_HEIGHT 480.0f

#define LEFT_WIDTH 22.0f
#define MIDDLE_WIDTH 96.0f
#define RIGHT_WIDTH 73.0f
#define RIGHT_HEIGHT 20.0f
#define BAR_WIDTH (LEFT_WIDTH + MIDDLE_WIDTH + RIGHT_WIDTH)
#define BAR_HEIGHT 25.0f

#define BTN_WIDTH (16.0f)
#define BTN_HEIGHT (12.0f)
#define CONFIGURE_POSX (LEFT_WIDTH + MIDDLE_WIDTH + 19.0f)
#define CONFIGURE_POSY (2.0f)
#define START_POSX (LEFT_WIDTH + MIDDLE_WIDTH + 37.0f)
#define START_POSY (2.0f)

#define BTN_U (18.0f)
#define BTN_V (40.0f)
#define BTN_NORMAL_UVSTART (EVector2(1.0f / BTN_U, 1.0f / BTN_V))
#define BTN_NORMAL_UVEND (EVector2(16.0f / BTN_U, 13.0f / BTN_V))
#define BTN_OVER_UVSTART (EVector2(1.0f / BTN_U, 14.0f / BTN_V))
#define BTN_OVER_UVEND (EVector2(16.0f / BTN_U, 26.0f / BTN_V))
#define BTN_PRESSED_UVSTART (EVector2(1.0f / BTN_U, 27.0f / BTN_V))
#define BTN_PRESSED_UVEND (EVector2(16.0f / BTN_U, 39.0f / BTN_V))

UIHeroPosition::UIHeroPosition(UISceneManager *manager) : UIBase()
{
	if (manager != nullptr)
	{
		manager->AddGFxObject(this);
	}

	_SceneManager = manager;

	// Initialize
	{
		_Left = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::HEROPOSITION_POSITIONA], SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, LEFT_WIDTH, BAR_HEIGHT);
		_Middle = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::HEROPOSITION_POSITIONB], SCREEN_WIDTH, SCREEN_HEIGHT, LEFT_WIDTH, 0.0f, MIDDLE_WIDTH, BAR_HEIGHT);
		_Right = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::HEROPOSITION_POSITIONC], SCREEN_WIDTH, SCREEN_HEIGHT, LEFT_WIDTH + MIDDLE_WIDTH, 0.0f, RIGHT_WIDTH, RIGHT_HEIGHT);

		_Left->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_Middle->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_Right->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

		_ConfigureHelper = new UIButton(GAMEUI::MESSAGE_HELPER_CONFIGURE_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, CONFIGURE_POSX, CONFIGURE_POSY, BTN_WIDTH, BTN_HEIGHT);
		_ConfigureHelper->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_ConfigureHelper->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MACROUI_SETUP], BTN_NORMAL_UVSTART, BTN_NORMAL_UVEND, MU::MakeARGB(100, 100, 100, 255)));
		_ConfigureHelper->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MACROUI_SETUP], BTN_NORMAL_UVSTART, BTN_NORMAL_UVEND, MU::MakeARGB(255, 255, 255, 255)));
		_ConfigureHelper->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MACROUI_SETUP], BTN_OVER_UVSTART, BTN_OVER_UVEND, MU::MakeARGB(255, 255, 255, 255)));
		_ConfigureHelper->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MACROUI_SETUP], BTN_PRESSED_UVSTART, BTN_PRESSED_UVEND, MU::MakeARGB(255, 255, 255, 255)));
		_ConfigureHelper->SetTooltip(FONT_NORMAL, GlobalText[3561], MU::MakeARGB(255, 255, 255, 255), MU::MakeARGB(0, 0, 0, 180), false);

		_StartHelper = new UIButton(GAMEUI::MESSAGE_HELPER_START_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, START_POSX, START_POSY, BTN_WIDTH, BTN_HEIGHT);
		_StartHelper->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_StartHelper->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MACROUI_START], BTN_NORMAL_UVSTART, BTN_NORMAL_UVEND, MU::MakeARGB(100, 100, 100, 255)));
		_StartHelper->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MACROUI_START], BTN_NORMAL_UVSTART, BTN_NORMAL_UVEND, MU::MakeARGB(255, 255, 255, 255)));
		_StartHelper->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MACROUI_START], BTN_OVER_UVSTART, BTN_OVER_UVEND, MU::MakeARGB(255, 255, 255, 255)));
		_StartHelper->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MACROUI_START], BTN_PRESSED_UVSTART, BTN_PRESSED_UVEND, MU::MakeARGB(255, 255, 255, 255)));
		_StartHelper->SetTooltip(FONT_NORMAL, GlobalText[3562], MU::MakeARGB(255, 255, 255, 255), MU::MakeARGB(0, 0, 0, 180), false);
	}
}

UIHeroPosition::~UIHeroPosition()
{

}

void UIHeroPosition::Draw()
{
	_Left->Draw();
	_Middle->Draw();
	_Right->Draw();

	_ConfigureHelper->Draw();
	_StartHelper->Draw();

	mu_text buffer[256];
	mu_sprintf_s(buffer, mu_countof(buffer), _T("") SPF_STRING _T(" (%d , %d)"), GetMapName(GET_WORLD), Hero->PositionX, Hero->PositionY);

	FONT_NORMAL->DrawStringC2D(TEXTINDEX::HERO_POSITION,
		MU_TO_WINDOW((BAR_WIDTH - 51.0f) * 0.5f, MU_RESOURCE->_640x480Rate[0]),
		MU_TO_WINDOW(BAR_HEIGHT * 0.35f, MU_RESOURCE->_640x480Rate[1]),
		1.0f, ETA_CENTER, ETA_CENTER, 0.0f,
		MU::MakeARGB(255, 255, 255, 255),
		MU::MakeARGB(0, 0, 0, 0),
		buffer);
}

void UIHeroPosition::PostDraw()
{
	_ConfigureHelper->PostDraw();
	_StartHelper->PostDraw();
}

UIBase *UIHeroPosition::GetFocus(mu_int32 mouseX, mu_int32 mouseY)
{
	UIBase *object = _ConfigureHelper->GetFocus(mouseX, mouseY);

	if (object != nullptr)
	{
		return object;
	}

	object = _StartHelper->GetFocus(mouseX, mouseY);

	if (object != nullptr)
	{
		return object;
	}

	return nullptr;
}

void UIHeroPosition::SetHelperButton(mu_boolean isStarted)
{
	if (isStarted)
	{
		_StartHelper->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MACROUI_STOP], BTN_NORMAL_UVSTART, BTN_NORMAL_UVEND, MU::MakeARGB(100, 100, 100, 255)));
		_StartHelper->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MACROUI_STOP], BTN_NORMAL_UVSTART, BTN_NORMAL_UVEND, MU::MakeARGB(255, 255, 255, 255)));
		_StartHelper->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MACROUI_STOP], BTN_OVER_UVSTART, BTN_OVER_UVEND, MU::MakeARGB(255, 255, 255, 255)));
		_StartHelper->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MACROUI_STOP], BTN_PRESSED_UVSTART, BTN_PRESSED_UVEND, MU::MakeARGB(255, 255, 255, 255)));
		_StartHelper->SetTooltip(FONT_NORMAL, GlobalText[3562], MU::MakeARGB(255, 255, 255, 255), MU::MakeARGB(0, 0, 0, 180), false);
	}
	else
	{
		_StartHelper->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MACROUI_START], BTN_NORMAL_UVSTART, BTN_NORMAL_UVEND, MU::MakeARGB(100, 100, 100, 255)));
		_StartHelper->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MACROUI_START], BTN_NORMAL_UVSTART, BTN_NORMAL_UVEND, MU::MakeARGB(255, 255, 255, 255)));
		_StartHelper->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MACROUI_START], BTN_OVER_UVSTART, BTN_OVER_UVEND, MU::MakeARGB(255, 255, 255, 255)));
		_StartHelper->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MACROUI_START], BTN_PRESSED_UVSTART, BTN_PRESSED_UVEND, MU::MakeARGB(255, 255, 255, 255)));
		_StartHelper->SetTooltip(FONT_NORMAL, GlobalText[3563], MU::MakeARGB(255, 255, 255, 255), MU::MakeARGB(0, 0, 0, 180), false);
	}
}