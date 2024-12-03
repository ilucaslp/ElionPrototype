#include "stdafx.h"
#include "mu_uibloodcastlerank.h"

#define SCREEN_WIDTH 1024.0f
#define SCREEN_HEIGHT 768.0f

#define BASE_WIDTH 230.0f
#define BASE_HEIGHT 132.0f

#define BOX_WIDTH 352.0f
#define BOX_HEIGHT (50.0f + BOX_WIDTH / (BASE_WIDTH / BASE_HEIGHT))
#define BOX_POSX ((SCREEN_WIDTH - BOX_WIDTH) * 0.5f)
#define BOX_POSY ((SCREEN_HEIGHT - BOX_HEIGHT) * 0.5f)

#define BACKTEXTURE_WIDTH 225.0f
#define BACKTEXTURE_HEIGHT 512.0f

#define BACK_POSX (BOX_POSX + BOX_WIDTH * (2.5f / BASE_WIDTH))
#define BACK_POSY (BOX_POSY + BOX_HEIGHT * (4.0f / BASE_HEIGHT))
#define BACK_WIDTH (BOX_WIDTH - BOX_WIDTH * (10.0f / BASE_WIDTH))
#define BACK_HEIGHT (BOX_HEIGHT - BOX_HEIGHT * (12.0f / BASE_HEIGHT))
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

#define BUTTON_WIDTH 54.0f
#define BUTTON_HEIGHT 30.0f

#define OK_POSX (BOX_POSX + (BOX_WIDTH - BUTTON_WIDTH) * 0.5f)
#define OK_POSY (BOX_POSY + BOX_HEIGHT - BUTTON_HEIGHT * 2.25f)

#define MAX_TEXTWIDTH (BOX_WIDTH * (180.0f / BASE_WIDTH))

#define TOP_TEXT 0.65f

UIBloodCastleRank::UIBloodCastleRank(UISceneManager *manager) : UIBase()
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
		_Top = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::MSGBOX_TOP], SCREEN_WIDTH, SCREEN_HEIGHT, TOP_POSX, TOP_POSY, TOP_WIDTH, TOP_HEIGHT);
		_Middle = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::MSGBOX_MIDDLE], SCREEN_WIDTH, SCREEN_HEIGHT, MIDDLE_POSX, MIDDLE_POSY, MIDDLE_WIDTH, MIDDLE_HEIGHT);
		_Bottom = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::MSGBOX_BOTTOM], SCREEN_WIDTH, SCREEN_HEIGHT, BOTTOM_POSX, BOTTOM_POSY, BOTTOM_WIDTH, BOTTOM_HEIGHT);

		_Top->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_Middle->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_Bottom->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

		_OkButton = new UIButton(MESSAGEBOX::MESSAGE_BCRANK_OKBUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, OK_POSX, OK_POSY, BUTTON_WIDTH, BUTTON_HEIGHT);
		_OkButton->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_OkButton->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_OK], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(100, 100, 100, 255)));
		_OkButton->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_OK], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(255, 255, 255, 255)));
		_OkButton->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_OK], TUV_THREE_OVER_BEGIN, TUV_THREE_OVER_END, MU::MakeARGB(255, 255, 255, 255)));
		_OkButton->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MSGBOX_OK], TUV_THREE_PRESSED_BEGIN, TUV_THREE_PRESSED_END, MU::MakeARGB(255, 255, 255, 255)));
	}
}

UIBloodCastleRank::~UIBloodCastleRank()
{
}

void UIBloodCastleRank::Update(mu_uint64 CurrentTime)
{

}

void UIBloodCastleRank::PostDraw()
{
	DrawFrame();
	DrawText();
	DrawButton();
}

void UIBloodCastleRank::DrawFrame()
{
	_Back->Draw();
	_Top->Draw();
	_Middle->Draw();
	_Bottom->Draw();
}

void UIBloodCastleRank::DrawText()
{
	MUTextSimpleRender *font = FONT_NORMAL;
	mu_uint32 textColor = MU::MakeARGB(128, 255, 128, 255);
	mu_uint32 backColor = MU::MakeARGB(0, 0, 0, 0);

	const mu_text *msg1 = nullptr;
	const mu_text *msg2 = nullptr;

	if (_IsSucceed == true)
	{
		msg1 = GlobalText[858];
		msg2 = GlobalText[857];
	}
	else
	{
		msg1 = GlobalText[860];
		msg2 = GlobalText[859];
	}

	mu_float messageX = _NativeX + _Width * 0.5f;
	mu_float messageY = _ViewY + 40.0f;
	font->DrawStringC2D(TEXTINDEX::BCR_MESSAGE,
		messageX,
		MU_TO_WINDOW(messageY, _AspectHeight),
		1.0f,
		ETA_CENTER,
		ETA_BOTTOM,
		0.0f,
		textColor,
		backColor,
		msg1);
	messageY += 20.0f;
	font->DrawStringC2D(TEXTINDEX::BCR_MESSAGE,
		messageX,
		MU_TO_WINDOW(messageY, _AspectHeight),
		1.0f,
		ETA_CENTER,
		ETA_BOTTOM,
		0.0f,
		textColor,
		backColor,
		msg2);
	messageY += 36.0f;

	mu_text textBuffer[256];

	font = FONT_BOLD;
	textColor = MU::MakeARGB(210, 255, 210, 255);
	mu_sprintf_s(textBuffer, mu_countof(textBuffer), GlobalText[861], (mu_int32)_RankData.BonusExp);
	font->DrawStringC2D(TEXTINDEX::BCR_EXPREWARD,
		messageX,
		MU_TO_WINDOW(messageY, _AspectHeight),
		1.0f,
		ETA_CENTER,
		ETA_BOTTOM,
		0.0f,
		textColor,
		backColor,
		textBuffer);
	messageY += 24.0f;

	if (_IsSucceed)
	{
		textColor = MU::MakeARGB(255, 210, 210, 255);
		mu_sprintf_s(textBuffer, mu_countof(textBuffer), GlobalText[862], (mu_int32)_RankData.BonusZen);
		font->DrawStringC2D(TEXTINDEX::BCR_ZENREWARD,
			messageX,
			MU_TO_WINDOW(messageY, _AspectHeight),
			1.0f,
			ETA_CENTER,
			ETA_BOTTOM,
			0.0f,
			textColor,
			backColor,
			textBuffer);
		messageY += 24.0f;
	}

	textColor = MU::MakeARGB(210, 210, 255, 255);
	mu_sprintf_s(textBuffer, mu_countof(textBuffer), GlobalText[863], (mu_int32)_RankData.TotalScore);
	font->DrawStringC2D(TEXTINDEX::BCR_SCOREREWARD,
		messageX,
		MU_TO_WINDOW(messageY, _AspectHeight),
		1.0f,
		ETA_CENTER,
		ETA_BOTTOM,
		0.0f,
		textColor,
		backColor,
		textBuffer);
}

void UIBloodCastleRank::DrawButton()
{
	_OkButton->Draw();
	_OkButton->PostDraw();
}

mu_uint32 UIBloodCastleRank::OnPreKeyPressed(const SDL_Keysym &key)
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

UIBase *UIBloodCastleRank::GetFocus(mu_int32 mouseX, mu_int32 mouseY)
{
	UIBase *focus = UIBase::GetFocus(mouseX, mouseY);

	if (focus != nullptr)
	{
		UIBase *object = _OkButton->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}
	}

	return focus;
}