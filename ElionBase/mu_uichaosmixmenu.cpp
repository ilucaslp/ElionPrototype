#include "stdafx.h"
#include "mu_uichaosmixmenu.h"

#define SCREEN_WIDTH 800.0f
#define SCREEN_HEIGHT 600.0f

#define BASE_WIDTH 230.0f
#define BASE_HEIGHT 132.0f

#define BOX_WIDTH 230.0f
#define BOX_HEIGHT (67.0f + 250.0f + 50.0f)
#define BOX_POSX (SCREEN_WIDTH * 0.5f - BOX_WIDTH * 0.5f)
#define BOX_POSY (SCREEN_HEIGHT * 0.5f - BOX_HEIGHT * 0.5f)

#define BACKTEXTURE_WIDTH 225.0f
#define BACKTEXTURE_HEIGHT 512.0f

#define BACK_POSX (BOX_POSX + BOX_WIDTH * (2.5f / BASE_WIDTH))
#define BACK_POSY (BOX_POSY + BOX_HEIGHT * (4.0f / BASE_HEIGHT))
#define BACK_WIDTH (BOX_WIDTH - BOX_WIDTH * (10.0f / BASE_WIDTH))
#define BACK_HEIGHT (BOX_HEIGHT - BOX_HEIGHT * (7.0f / BASE_HEIGHT))
#define BACK_UVBEGIN EVector2(0.0f, 0.0f)
#define BACK_UVEND EVector2(1.0f, BACK_HEIGHT / BACKTEXTURE_HEIGHT)

#define TOP_WIDTH BOX_WIDTH
#define TOP_HEIGHT (BOX_WIDTH * (67.0f / BASE_WIDTH))
#define BOTTOM_WIDTH BOX_WIDTH
#define BOTTOM_HEIGHT (BOX_WIDTH * (50.0f / BASE_WIDTH))
#define MIDDLE_WIDTH BOX_WIDTH
#define MIDDLE_HEIGHT (BOX_HEIGHT - TOP_HEIGHT - BOTTOM_HEIGHT)

#define TOP_POSX BOX_POSX
#define TOP_POSY BOX_POSY
#define MIDDLE_POSX BOX_POSX
#define MIDDLE_POSY (TOP_POSY + TOP_HEIGHT)
#define BOTTOM_POSX BOX_POSX
#define BOTTOM_POSY (MIDDLE_POSY + MIDDLE_HEIGHT)

#define BUTTON_WIDTH 128.0f
#define BUTTON_HEIGHT 29.0f

#define SMALLBUTTON_WIDTH 64.0f
#define SMALLBUTTON_HEIGHT 29.0f

#define MIXGENERAL_POSX (BOX_POSX + BOX_WIDTH * 0.5f - BUTTON_WIDTH * 0.5f)
#define MIXGENERAL_POSY (BOX_POSY + 85.0f + 25.0f)
#define MIXCHAOS_POSX (BOX_POSX + BOX_WIDTH * 0.5f - BUTTON_WIDTH * 0.5f)
#define MIXCHAOS_POSY (BOX_POSY + 155.0f + 25.0f)
#define MIX380_POSX (BOX_POSX + BOX_WIDTH * 0.5f - BUTTON_WIDTH * 0.5f)
#define MIX380_POSY (BOX_POSY + 225.0f + 25.0f)
#define CANCEL_POSX (BOX_POSX + BOX_WIDTH * 0.5f - SMALLBUTTON_WIDTH * 0.5f)
#define CANCEL_POSY (BOX_POSY + 225.0f + 29.0f + 40.0f)

UIChaosMixMenu::UIChaosMixMenu(UISceneManager *manager) : UIBase()
{
	if (manager != nullptr)
	{
		manager->AddGFxObject(this);
	}

	_SceneManager = manager;

	UIBase::SetAspectRatio(SCREEN_WIDTH, SCREEN_HEIGHT);
	UIBase::SetPosition(BOX_POSX, BOX_POSY);
	UIBase::SetDimensions(BOX_WIDTH, BOX_HEIGHT);

	SetGfxFlag(GFX_FLAG::GFX_NO_FOCUS | GFX_FLAG::GFX_HIDDEN);

	// Initialize
	{
		_Back = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::MSGBOX_BACK], SCREEN_WIDTH, SCREEN_HEIGHT, BACK_POSX, BACK_POSY, BACK_WIDTH, BACK_HEIGHT, BACK_UVBEGIN, BACK_UVEND);
		_Top = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::MSGBOX_TITLEBAR], SCREEN_WIDTH, SCREEN_HEIGHT, TOP_POSX, TOP_POSY, TOP_WIDTH, TOP_HEIGHT);
		_Middle = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::MSGBOX_MIDDLE], SCREEN_WIDTH, SCREEN_HEIGHT, MIDDLE_POSX, MIDDLE_POSY, MIDDLE_WIDTH, MIDDLE_HEIGHT);
		_Bottom = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::MSGBOX_BOTTOM], SCREEN_WIDTH, SCREEN_HEIGHT, BOTTOM_POSX, BOTTOM_POSY, BOTTOM_WIDTH, BOTTOM_HEIGHT);

		_Top->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_Middle->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_Bottom->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

		_GeneralButton = new UIButton(MESSAGEBOX::MESSAGE_MIXMENU_GENERALBUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, MIXGENERAL_POSX, MIXGENERAL_POSY, BUTTON_WIDTH, BUTTON_HEIGHT);
		_GeneralButton->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_GeneralButton->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTY_BUTTON], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(100, 100, 100, 255)));
		_GeneralButton->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTY_BUTTON], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(255, 255, 255, 255)));
		_GeneralButton->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTY_BUTTON], TUV_THREE_OVER_BEGIN, TUV_THREE_OVER_END, MU::MakeARGB(255, 255, 255, 255)));
		_GeneralButton->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTY_BUTTON], TUV_THREE_PRESSED_BEGIN, TUV_THREE_PRESSED_END, MU::MakeARGB(255, 255, 255, 255)));
		_GeneralButton->SetStateText(UIButton::UI_DISABLED, FONT_NORMAL, GlobalText[735], MU::MakeARGB(100, 100, 100, 255));
		_GeneralButton->SetStateText(UIButton::UI_NORMAL, FONT_NORMAL, GlobalText[735]);
		_GeneralButton->SetStateText(UIButton::UI_OVER, FONT_NORMAL, GlobalText[735]);
		_GeneralButton->SetStateText(UIButton::UI_PRESSED, FONT_NORMAL, GlobalText[735]);

		_ChaosButton = new UIButton(MESSAGEBOX::MESSAGE_MIXMENU_CHAOSBUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, MIXCHAOS_POSX, MIXCHAOS_POSY, BUTTON_WIDTH, BUTTON_HEIGHT);
		_ChaosButton->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_ChaosButton->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTY_BUTTON], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(100, 100, 100, 255)));
		_ChaosButton->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTY_BUTTON], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(255, 255, 255, 255)));
		_ChaosButton->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTY_BUTTON], TUV_THREE_OVER_BEGIN, TUV_THREE_OVER_END, MU::MakeARGB(255, 255, 255, 255)));
		_ChaosButton->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTY_BUTTON], TUV_THREE_PRESSED_BEGIN, TUV_THREE_PRESSED_END, MU::MakeARGB(255, 255, 255, 255)));
		_ChaosButton->SetStateText(UIButton::UI_DISABLED, FONT_NORMAL, GlobalText[736], MU::MakeARGB(100, 100, 100, 255));
		_ChaosButton->SetStateText(UIButton::UI_NORMAL, FONT_NORMAL, GlobalText[736]);
		_ChaosButton->SetStateText(UIButton::UI_OVER, FONT_NORMAL, GlobalText[736]);
		_ChaosButton->SetStateText(UIButton::UI_PRESSED, FONT_NORMAL, GlobalText[736]);

		_380Button = new UIButton(MESSAGEBOX::MESSAGE_MIXMENU_380BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, MIX380_POSX, MIX380_POSY, BUTTON_WIDTH, BUTTON_HEIGHT);
		_380Button->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_380Button->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTY_BUTTON], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(100, 100, 100, 255)));
		_380Button->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTY_BUTTON], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(255, 255, 255, 255)));
		_380Button->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTY_BUTTON], TUV_THREE_OVER_BEGIN, TUV_THREE_OVER_END, MU::MakeARGB(255, 255, 255, 255)));
		_380Button->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTY_BUTTON], TUV_THREE_PRESSED_BEGIN, TUV_THREE_PRESSED_END, MU::MakeARGB(255, 255, 255, 255)));
		_380Button->SetStateText(UIButton::UI_DISABLED, FONT_NORMAL, GlobalText[2193], MU::MakeARGB(100, 100, 100, 255));
		_380Button->SetStateText(UIButton::UI_NORMAL, FONT_NORMAL, GlobalText[2193]);
		_380Button->SetStateText(UIButton::UI_OVER, FONT_NORMAL, GlobalText[2193]);
		_380Button->SetStateText(UIButton::UI_PRESSED, FONT_NORMAL, GlobalText[2193]);

		_CancelButton = new UIButton(MESSAGEBOX::MESSAGE_MIXMENU_CANCELBUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, CANCEL_POSX, CANCEL_POSY, SMALLBUTTON_WIDTH, SMALLBUTTON_HEIGHT);
		_CancelButton->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_CancelButton->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTY_BUTTON_SMALL], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(100, 100, 100, 255)));
		_CancelButton->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTY_BUTTON_SMALL], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(255, 255, 255, 255)));
		_CancelButton->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTY_BUTTON_SMALL], TUV_THREE_OVER_BEGIN, TUV_THREE_OVER_END, MU::MakeARGB(255, 255, 255, 255)));
		_CancelButton->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTY_BUTTON_SMALL], TUV_THREE_PRESSED_BEGIN, TUV_THREE_PRESSED_END, MU::MakeARGB(255, 255, 255, 255)));
		_CancelButton->SetStateText(UIButton::UI_DISABLED, FONT_NORMAL, GlobalText[1002], MU::MakeARGB(100, 100, 100, 255));
		_CancelButton->SetStateText(UIButton::UI_NORMAL, FONT_NORMAL, GlobalText[1002]);
		_CancelButton->SetStateText(UIButton::UI_OVER, FONT_NORMAL, GlobalText[1002]);
		_CancelButton->SetStateText(UIButton::UI_PRESSED, FONT_NORMAL, GlobalText[1002]);
	}
}

UIChaosMixMenu::~UIChaosMixMenu()
{

}

void UIChaosMixMenu::Update(mu_uint64 CurrentTime)
{

}

void UIChaosMixMenu::PostDraw()
{
	DrawFrame();
	DrawText();
	DrawButton();
}

void UIChaosMixMenu::DrawFrame()
{
	_Back->Draw();
	_Top->Draw();
	_Middle->Draw();
	_Bottom->Draw();
}

void UIChaosMixMenu::DrawText()
{
	mu_float PosX = BOX_POSX + BOX_WIDTH * 0.5f;
	mu_float PosY = BOX_POSY + 10.0f;

	FONT_BOLD->DrawStringC2D(TEXTINDEX::CHAOSMIXMENU_BEGIN,
		MU_TO_WINDOW(PosX, _AspectWidth),
		MU_TO_WINDOW(PosY, _AspectHeight),
		1.0f, ETA_CENTER, ETA_BOTTOM, 0.0f,
		MU::MakeARGB(255, 128, 0, 255), MU::MakeARGB(0, 0, 0, 0),
		GlobalText[734]);

	PosY += 40.0f;

	FONT_NORMAL->DrawStringC2D(TEXTINDEX::CHAOSMIXMENU_BEGIN,
		MU_TO_WINDOW(PosX, _AspectWidth),
		MU_TO_WINDOW(PosY + 15.0f, _AspectHeight),
		1.0f, ETA_CENTER, ETA_BOTTOM, 0.0f,
		MU::MakeARGB(255, 128, 0, 255), MU::MakeARGB(0, 0, 0, 0),
		GlobalText[872]);
	FONT_NORMAL->DrawStringC2D(TEXTINDEX::CHAOSMIXMENU_BEGIN,
		MU_TO_WINDOW(PosX, _AspectWidth),
		MU_TO_WINDOW(PosY + 30.0f, _AspectHeight),
		1.0f, ETA_CENTER, ETA_BOTTOM, 0.0f,
		MU::MakeARGB(255, 128, 0, 255), MU::MakeARGB(0, 0, 0, 0),
		GlobalText[873]);
	FONT_NORMAL->DrawStringC2D(TEXTINDEX::CHAOSMIXMENU_BEGIN,
		MU_TO_WINDOW(PosX, _AspectWidth),
		MU_TO_WINDOW(PosY + 45.0f, _AspectHeight),
		1.0f, ETA_CENTER, ETA_BOTTOM, 0.0f,
		MU::MakeARGB(255, 128, 0, 255), MU::MakeARGB(0, 0, 0, 0),
		GlobalText[1680]);

	PosY += 100.0f;

	FONT_NORMAL->DrawStringC2D(TEXTINDEX::CHAOSMIXMENU_BEGIN,
		MU_TO_WINDOW(PosX, _AspectWidth),
		MU_TO_WINDOW(PosY, _AspectHeight),
		1.0f, ETA_CENTER, ETA_BOTTOM, 0.0f,
		MU::MakeARGB(255, 128, 0, 255), MU::MakeARGB(0, 0, 0, 0),
		GlobalText[870]);
	FONT_NORMAL->DrawStringC2D(TEXTINDEX::CHAOSMIXMENU_BEGIN,
		MU_TO_WINDOW(PosX, _AspectWidth),
		MU_TO_WINDOW(PosY + 15.0f, _AspectHeight),
		1.0f, ETA_CENTER, ETA_BOTTOM, 0.0f,
		MU::MakeARGB(255, 128, 0, 255), MU::MakeARGB(0, 0, 0, 0),
		GlobalText[871]);

	PosY += 85.0f;

	FONT_NORMAL->DrawStringC2D(TEXTINDEX::CHAOSMIXMENU_BEGIN,
		MU_TO_WINDOW(PosX, _AspectWidth),
		MU_TO_WINDOW(PosY, _AspectHeight),
		1.0f, ETA_CENTER, ETA_BOTTOM, 0.0f,
		MU::MakeARGB(255, 128, 0, 255), MU::MakeARGB(0, 0, 0, 0),
		GlobalText[2194]);
}

void UIChaosMixMenu::DrawButton()
{
	_GeneralButton->Draw();
	_ChaosButton->Draw();
	_380Button->Draw();
	_CancelButton->Draw();
	_GeneralButton->PostDraw();
	_ChaosButton->PostDraw();
	_380Button->PostDraw();
	_CancelButton->PostDraw();
}

UIBase *UIChaosMixMenu::GetFocus(mu_int32 mouseX, mu_int32 mouseY)
{
	UIBase *focus = UIBase::GetFocus(mouseX, mouseY);

	if (focus != nullptr)
	{
		UIBase *object = _GeneralButton->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}

		object = _ChaosButton->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}

		object = _380Button->GetFocus(mouseX, mouseY);

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