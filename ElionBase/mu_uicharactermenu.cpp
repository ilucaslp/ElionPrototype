#include "stdafx.h"
#include "mu_uicharactermenu.h"

#define SCREEN_WIDTH 1024.0f
#define SCREEN_HEIGHT 768.0f
#define WINDOW_WIDTH 213.0f
#define WINDOW_HEIGHT 187.0f
#define WINDOW_POSX ((SCREEN_WIDTH - WINDOW_WIDTH) * 0.5f)
#define WINDOW_POSY ((SCREEN_HEIGHT - WINDOW_HEIGHT) * 0.5f)

#define TOP_WIDTH WINDOW_WIDTH
#define TOP_HEIGHT 64.0f
#define TOP_POSX WINDOW_POSX
#define TOP_POSY WINDOW_POSY

#define BOTTOM_WIDTH WINDOW_WIDTH
#define BOTTOM_HEIGHT 43.0f
#define BOTTOM_POSX WINDOW_POSX
#define BOTTOM_POSY (WINDOW_POSY + WINDOW_HEIGHT - BOTTOM_HEIGHT)

#define SIDE_WIDTH 5.0f
#define SIDE_HEIGHT (WINDOW_HEIGHT - TOP_HEIGHT - BOTTOM_HEIGHT)
#define LEFT_POSX TOP_POSX
#define LEFT_POSY (TOP_POSY + TOP_HEIGHT)
#define RIGHT_POSX (TOP_POSX + TOP_WIDTH - SIDE_WIDTH)
#define RIGHT_POSY (TOP_POSY + TOP_HEIGHT)

#define BACK_WIDTH (WINDOW_WIDTH - 6.0f)
#define BACK_HEIGHT (WINDOW_HEIGHT - 6.0f)
#define BACK_POSX (WINDOW_POSX + 3.0f)
#define BACK_POSY (WINDOW_POSY + 3.0f)

#define EXIT_WIDTH 108.0f
#define EXIT_HEIGHT 30.0f
#define EXIT_POSX (WINDOW_POSX + (WINDOW_WIDTH - EXIT_WIDTH) * 0.5f)
#define EXIT_POSY (WINDOW_POSY + 33.0f)

#define SERVER_WIDTH 108.0f
#define SERVER_HEIGHT 30.0f
#define SERVER_POSX (WINDOW_POSX + (WINDOW_WIDTH - EXIT_WIDTH) * 0.5f)
#define SERVER_POSY (WINDOW_POSY + 33.0f + (SERVER_HEIGHT + 4.0f) * 1.0f)

#define OPTION_WIDTH 108.0f
#define OPTION_HEIGHT 30.0f
#define OPTION_POSX (WINDOW_POSX + (WINDOW_WIDTH - EXIT_WIDTH) * 0.5f)
#define OPTION_POSY (WINDOW_POSY + 33.0f + (OPTION_HEIGHT + 4.0f) * 2.0f)

#define CLOSE_WIDTH 108.0f
#define CLOSE_HEIGHT 30.0f
#define CLOSE_POSX (WINDOW_POSX + (WINDOW_WIDTH - EXIT_WIDTH) * 0.5f)
#define CLOSE_POSY (WINDOW_POSY + 33.0f + (OPTION_HEIGHT + 4.0f) * 3.0f)

UICharacterMenu::UICharacterMenu(UISceneManager *manager) : UIBase()
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
		_BackFrame = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::MENU_BACK], SCREEN_WIDTH, SCREEN_HEIGHT, BACK_POSX, BACK_POSY, BACK_WIDTH, BACK_HEIGHT);
		_TopFrame = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::MENU_TOP], SCREEN_WIDTH, SCREEN_HEIGHT, TOP_POSX, TOP_POSY, TOP_WIDTH, TOP_HEIGHT);
		_TopFrame->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_BottomFrame = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::MENU_BOTTOM], SCREEN_WIDTH, SCREEN_HEIGHT, BOTTOM_POSX, BOTTOM_POSY, BOTTOM_WIDTH, BOTTOM_HEIGHT);
		_BottomFrame->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_LeftFrame = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::MENU_LEFT], SCREEN_WIDTH, SCREEN_HEIGHT, LEFT_POSX, LEFT_POSY, SIDE_WIDTH, SIDE_HEIGHT);
		_RightFrame = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::MENU_RIGHT], SCREEN_WIDTH, SCREEN_HEIGHT, RIGHT_POSX, RIGHT_POSY, SIDE_WIDTH, SIDE_HEIGHT);

		_ExitButton = new UIButton(OPTIONUI::MESSAGE_CHARACTER_EXITBUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, EXIT_POSX, EXIT_POSY, EXIT_WIDTH, EXIT_HEIGHT);
		_ExitButton->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_ExitButton->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MENU_BUTTON], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.25f), MU::MakeARGB(100, 100, 100, 255)));
		_ExitButton->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MENU_BUTTON], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.25f), MU::MakeARGB(255, 255, 255, 255)));
		_ExitButton->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MENU_BUTTON], EVector2(0.0f, 0.25f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_ExitButton->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MENU_BUTTON], EVector2(0.0f, 0.5f), EVector2(1.0f, 0.75f), MU::MakeARGB(255, 255, 255, 255)));
		_ExitButton->SetStateText(UIButton::UI_DISABLED, FONT_NORMAL, GlobalText[381], MU::MakeARGB(150, 150, 150, 200));
		_ExitButton->SetStateText(UIButton::UI_NORMAL, FONT_NORMAL, GlobalText[381], MU::MakeARGB(255, 255, 255, 255));
		_ExitButton->SetStateText(UIButton::UI_OVER, FONT_NORMAL, GlobalText[381], MU::MakeARGB(255, 255, 255, 255));
		_ExitButton->SetStateText(UIButton::UI_PRESSED, FONT_NORMAL, GlobalText[381], MU::MakeARGB(255, 255, 255, 255));

		_ServerButton = new UIButton(OPTIONUI::MESSAGE_CHARACTER_SERVERBUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, SERVER_POSX, SERVER_POSY, SERVER_WIDTH, SERVER_HEIGHT);
		_ServerButton->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_ServerButton->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MENU_BUTTON], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.25f), MU::MakeARGB(100, 100, 100, 255)));
		_ServerButton->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MENU_BUTTON], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.25f), MU::MakeARGB(255, 255, 255, 255)));
		_ServerButton->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MENU_BUTTON], EVector2(0.0f, 0.25f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_ServerButton->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MENU_BUTTON], EVector2(0.0f, 0.5f), EVector2(1.0f, 0.75f), MU::MakeARGB(255, 255, 255, 255)));
		_ServerButton->SetStateText(UIButton::UI_DISABLED, FONT_NORMAL, GlobalText[382], MU::MakeARGB(150, 150, 150, 200));
		_ServerButton->SetStateText(UIButton::UI_NORMAL, FONT_NORMAL, GlobalText[382], MU::MakeARGB(255, 255, 255, 255));
		_ServerButton->SetStateText(UIButton::UI_OVER, FONT_NORMAL, GlobalText[382], MU::MakeARGB(255, 255, 255, 255));
		_ServerButton->SetStateText(UIButton::UI_PRESSED, FONT_NORMAL, GlobalText[382], MU::MakeARGB(255, 255, 255, 255));

		_OptionButton = new UIButton(OPTIONUI::MESSAGE_CHARACTER_OPTIONBUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, OPTION_POSX, OPTION_POSY, OPTION_WIDTH, OPTION_HEIGHT);
		_OptionButton->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_OptionButton->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MENU_BUTTON], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.25f), MU::MakeARGB(100, 100, 100, 255)));
		_OptionButton->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MENU_BUTTON], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.25f), MU::MakeARGB(255, 255, 255, 255)));
		_OptionButton->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MENU_BUTTON], EVector2(0.0f, 0.25f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_OptionButton->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MENU_BUTTON], EVector2(0.0f, 0.5f), EVector2(1.0f, 0.75f), MU::MakeARGB(255, 255, 255, 255)));
		_OptionButton->SetStateText(UIButton::UI_DISABLED, FONT_NORMAL, GlobalText[385], MU::MakeARGB(150, 150, 150, 200));
		_OptionButton->SetStateText(UIButton::UI_NORMAL, FONT_NORMAL, GlobalText[385], MU::MakeARGB(255, 255, 255, 255));
		_OptionButton->SetStateText(UIButton::UI_OVER, FONT_NORMAL, GlobalText[385], MU::MakeARGB(255, 255, 255, 255));
		_OptionButton->SetStateText(UIButton::UI_PRESSED, FONT_NORMAL, GlobalText[385], MU::MakeARGB(255, 255, 255, 255));

		_CloseButton = new UIButton(OPTIONUI::MESSAGE_CHARACTER_CLOSEBUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, CLOSE_POSX, CLOSE_POSY, CLOSE_WIDTH, CLOSE_HEIGHT);
		_CloseButton->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_CloseButton->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MENU_BUTTON], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.25f), MU::MakeARGB(100, 100, 100, 255)));
		_CloseButton->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MENU_BUTTON], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.25f), MU::MakeARGB(255, 255, 255, 255)));
		_CloseButton->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MENU_BUTTON], EVector2(0.0f, 0.25f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_CloseButton->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MENU_BUTTON], EVector2(0.0f, 0.5f), EVector2(1.0f, 0.75f), MU::MakeARGB(255, 255, 255, 255)));
		_CloseButton->SetStateText(UIButton::UI_DISABLED, FONT_NORMAL, GlobalText[388], MU::MakeARGB(150, 150, 150, 200));
		_CloseButton->SetStateText(UIButton::UI_NORMAL, FONT_NORMAL, GlobalText[388], MU::MakeARGB(255, 255, 255, 255));
		_CloseButton->SetStateText(UIButton::UI_OVER, FONT_NORMAL, GlobalText[388], MU::MakeARGB(255, 255, 255, 255));
		_CloseButton->SetStateText(UIButton::UI_PRESSED, FONT_NORMAL, GlobalText[388], MU::MakeARGB(255, 255, 255, 255));
	}
}

UICharacterMenu::~UICharacterMenu()
{

}

void UICharacterMenu::Update(mu_uint64 CurrentTime)
{

}

void UICharacterMenu::Draw()
{
	_BackFrame->Draw();
	_TopFrame->Draw();
	_BottomFrame->Draw();
	_LeftFrame->Draw();
	_RightFrame->Draw();

	_ExitButton->Draw();
	_ServerButton->Draw();
	_OptionButton->Draw();
	_CloseButton->Draw();
}

void UICharacterMenu::PostDraw()
{
	_ExitButton->PostDraw();
	_ServerButton->PostDraw();
	_OptionButton->PostDraw();
	_CloseButton->PostDraw();
}

UIBase *UICharacterMenu::GetFocus(mu_int32 mouseX, mu_int32 mouseY)
{
	UIBase *focus = UIBase::GetFocus(mouseX, mouseY);

	if (focus != nullptr)
	{
		UIBase *object = _ExitButton->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}

		object = _ServerButton->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}

		object = _OptionButton->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}

		object = _CloseButton->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}
	}

	return focus;
}