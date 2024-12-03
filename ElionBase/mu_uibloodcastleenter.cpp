#include "stdafx.h"
#include "mu_uibloodcastleenter.h"

#define SCREEN_WIDTH 640.0f
#define SCREEN_HEIGHT 480.0f

#define BACKTEXTURE_WIDTH 225.0f
#define BACKTEXTURE_HEIGHT 512.0f

#define BACK_POSX BLOODCASTLEENTER_POSX
#define BACK_POSY BLOODCASTLEENTER_POSY
#define BACK_WIDTH BLOODCASTLEENTER_WIDTH
#define BACK_HEIGHT BLOODCASTLEENTER_HEIGHT
#define BACK_UVBEGIN EVector2(0.0f, 0.0f)
#define BACK_UVEND EVector2(BACK_WIDTH / BACKTEXTURE_WIDTH, BACK_HEIGHT / BACKTEXTURE_HEIGHT)

#define TOP_WIDTH BLOODCASTLEENTER_WIDTH
#define TOP_HEIGHT 64.0f
#define TOP_POSX BLOODCASTLEENTER_POSX
#define TOP_POSY BLOODCASTLEENTER_POSY

#define BOTTOM_WIDTH BLOODCASTLEENTER_WIDTH
#define BOTTOM_HEIGHT 45.0f
#define BOTTOM_POSX BLOODCASTLEENTER_POSX
#define BOTTOM_POSY (BLOODCASTLEENTER_POSY + BLOODCASTLEENTER_HEIGHT - BOTTOM_HEIGHT)

#define LEFT_WIDTH 21.0f
#define LEFT_HEIGHT (BOTTOM_POSY - (TOP_POSY + TOP_HEIGHT))
#define LEFT_POSX BLOODCASTLEENTER_POSX
#define LEFT_POSY (TOP_POSY + TOP_HEIGHT)

#define RIGHT_WIDTH 21.0f
#define RIGHT_HEIGHT (BOTTOM_POSY - (TOP_POSY + TOP_HEIGHT))
#define RIGHT_POSX (BLOODCASTLEENTER_POSX + BLOODCASTLEENTER_WIDTH - RIGHT_WIDTH)
#define RIGHT_POSY (TOP_POSY + TOP_HEIGHT)

#define XBUTTON_WIDTH 16.0f
#define XBUTTON_HEIGHT 16.0f
#define XBUTTON_POSX (TOP_POSX + TOP_WIDTH - 7.0f - XBUTTON_WIDTH)
#define XBUTTON_POSY (TOP_POSY + 7.0f)

#define CLOSEBUTTON_POSX (BLOODCASTLEENTER_POSX + 13.0f)
#define CLOSEBUTTON_POSY (BLOODCASTLEENTER_POSY + 392.0f)
#define CLOSEBUTTON_WIDTH (36.0f)
#define CLOSEBUTTON_HEIGHT (29.0f)

#define ENTERBUTTON_WIDTH (180.0f)
#define ENTERBUTTON_HEIGHT (29.0f)
#define ENTERBUTTON_POSX (BLOODCASTLEENTER_POSX + 6.0f)
#define ENTERBUTTON_POSY(n) (BLOODCASTLEENTER_POSY + 125.0f + (ENTERBUTTON_HEIGHT + 4.0f) * n)

typedef struct _BLOOODCASTLE_LEVEL
{
	mu_int32 iLOWER_BOUND;
	mu_int32 iUPPER_BOUND;
	mu_int32 iLOWER_BOUND_MAGUMSA;
	mu_int32 iUPPER_BOUND_MAGUMSA;
} BLOODCASTLE_LEVEL, *LPBLOODCASTLE_LEVEL;

const BLOODCASTLE_LEVEL g_sttBLOODCASTLE_LEVEL[MAX_BLOODCASTLE_BRIDGE_COUNT] =
{
	{ 15,		80,			10,		60 },
	{ 81,		130,		61,		110 },
	{ 131,	180,		111,	160 },
	{ 181,	230,		161,	210 },
	{ 231,	280,		211,	260 },
	{ 281,	330,		261,	310 },
	{ 331,	MAX_LEVEL,	311,	MAX_LEVEL },
	{ MAX_LEVEL,	(MAX_LEVEL + MAX_MASTER_LEVEL),	MAX_LEVEL,	(MAX_LEVEL + MAX_MASTER_LEVEL) },
};

UIBloodCastleEnter::UIBloodCastleEnter(UISceneManager *manager) : UIBase()
{
	if (manager != nullptr)
	{
		manager->AddGFxObject(this);
	}

	_SceneManager = manager;

	UIBase::SetAspectRatio(SCREEN_WIDTH, SCREEN_HEIGHT);
	UIBase::SetPosition(BLOODCASTLEENTER_POSX, BLOODCASTLEENTER_POSY);
	UIBase::SetDimensions(BLOODCASTLEENTER_WIDTH, BLOODCASTLEENTER_HEIGHT);

	SetGfxFlag(GFX_FLAG::GFX_HIDDEN);
	SetEvent(GFX_EVENT::GFX_MOUSE_EVENT | GFX_EVENT::GFX_MOUSE_DBLCLICK_AS_CLICK);

	// Initialize
	{
		_BackFrame = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::MSGBOX_BACK], SCREEN_WIDTH, SCREEN_HEIGHT, BACK_POSX, BACK_POSY, BACK_WIDTH, BACK_HEIGHT, BACK_UVBEGIN, BACK_UVEND);
		_TopFrame = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::WINDOWFRAME_TOP], SCREEN_WIDTH, SCREEN_HEIGHT, TOP_POSX, TOP_POSY, TOP_WIDTH, TOP_HEIGHT);
		_TopFrame->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_BottomFrame = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::WINDOWFRAME_BOTTOM], SCREEN_WIDTH, SCREEN_HEIGHT, BOTTOM_POSX, BOTTOM_POSY, BOTTOM_WIDTH, BOTTOM_HEIGHT);
		_BottomFrame->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_LeftFrame = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::WINDOWFRAME_LEFT], SCREEN_WIDTH, SCREEN_HEIGHT, LEFT_POSX, LEFT_POSY, LEFT_WIDTH, LEFT_HEIGHT);
		_LeftFrame->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_RightFrame = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::WINDOWFRAME_RIGHT], SCREEN_WIDTH, SCREEN_HEIGHT, RIGHT_POSX, RIGHT_POSY, RIGHT_WIDTH, RIGHT_HEIGHT);
		_RightFrame->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

		_XButton = new UIButton(GAMEUI::MESSAGE_BLOODCASTLEENTER_CLOSE_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, XBUTTON_POSX, XBUTTON_POSY, XBUTTON_WIDTH, XBUTTON_HEIGHT);

		mu_text buffer[256];
		mu_sprintf_s(buffer, mu_countof(buffer), GlobalText[927], _T("C"));
		_CloseButton = new UIButton(GAMEUI::MESSAGE_BLOODCASTLEENTER_CLOSE_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, CLOSEBUTTON_POSX, CLOSEBUTTON_POSY, CLOSEBUTTON_WIDTH, CLOSEBUTTON_HEIGHT);
		_CloseButton->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_CloseButton->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EXIT_BUTTON], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(100, 100, 100, 255)));
		_CloseButton->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EXIT_BUTTON], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_CloseButton->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EXIT_BUTTON], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_CloseButton->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EXIT_BUTTON], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_CloseButton->SetTooltip(FONT_NORMAL, buffer, MU::MakeARGB(255, 255, 255, 255), MU::MakeARGB(0, 0, 0, 180), true);

		for (mu_uint32 n = 0; n < MAX_BLOODCASTLE_BRIDGE_COUNT; ++n)
		{
			_EnterButton[n] = new UIButton(GAMEUI::MESSAGE_BLOODCASTLEENTER_ENTER_BEGIN + n, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, ENTERBUTTON_POSX, ENTERBUTTON_POSY(n), ENTERBUTTON_WIDTH, ENTERBUTTON_HEIGHT);
			_EnterButton[n]->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
			_EnterButton[n]->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTY_BUTTON_BIG], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(100, 100, 100, 255)));
			_EnterButton[n]->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTY_BUTTON_BIG], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(255, 255, 255, 255)));
			_EnterButton[n]->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTY_BUTTON_BIG], TUV_THREE_OVER_BEGIN, TUV_THREE_OVER_END, MU::MakeARGB(255, 255, 255, 255)));
			_EnterButton[n]->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTY_BUTTON_BIG], TUV_THREE_PRESSED_BEGIN, TUV_THREE_PRESSED_END, MU::MakeARGB(255, 255, 255, 255)));
		}
	}
}

UIBloodCastleEnter::~UIBloodCastleEnter()
{

}

void UIBloodCastleEnter::Configure()
{
	mu_boolean isSpecialLevel = false;
	if (GetBaseClass(Hero->Class) == GAME::CLASS_DARK ||
		GetBaseClass(Hero->Class) == GAME::CLASS_DARK_LORD)
	{
		isSpecialLevel = true;
	}

	mu_int32 bridgeIndex = CheckLimitLevel(isSpecialLevel);

	for (mu_uint32 n = 0; n < MAX_BLOODCASTLE_BRIDGE_COUNT; ++n)
	{
		if (bridgeIndex == n)
		{
			_EnterButton[n]->ChangeState(UIButton::UI_NORMAL);
		}
		else
		{
			_EnterButton[n]->ChangeState(UIButton::UI_DISABLED);
		}

		mu_text textBuffer[256];
		if (n == MAX_BLOODCASTLE_BRIDGE_COUNT - 1)
		{
			mu_sprintf_s(textBuffer, mu_countof(textBuffer), GlobalText[1779], n + 1);
		}
		else if (isSpecialLevel == true)
		{
			mu_sprintf_s(textBuffer, mu_countof(textBuffer), GlobalText[847], n + 1, g_sttBLOODCASTLE_LEVEL[n].iLOWER_BOUND_MAGUMSA, g_sttBLOODCASTLE_LEVEL[n].iUPPER_BOUND_MAGUMSA);
		}
		else
		{
			mu_sprintf_s(textBuffer, mu_countof(textBuffer), GlobalText[847], n + 1, g_sttBLOODCASTLE_LEVEL[n].iLOWER_BOUND, g_sttBLOODCASTLE_LEVEL[n].iUPPER_BOUND);
		}

		_EnterButton[n]->SetStateText(UIButton::UI_DISABLED, FONT_BOLD, textBuffer, MU::MakeARGB(100, 100, 100, 255));
		_EnterButton[n]->SetStateText(UIButton::UI_NORMAL, FONT_BOLD, textBuffer, MU::MakeARGB(200, 200, 200, 255));
		_EnterButton[n]->SetStateText(UIButton::UI_OVER, FONT_BOLD, textBuffer, MU::MakeARGB(255, 255, 255, 255));
		_EnterButton[n]->SetStateText(UIButton::UI_PRESSED, FONT_BOLD, textBuffer, MU::MakeARGB(150, 150, 150, 255));
	}
}

void UIBloodCastleEnter::Update(mu_uint64 CurrentTime)
{

}

void UIBloodCastleEnter::Draw()
{
	DrawFrames();
	DrawButtons();
	DrawTexts();
}

void UIBloodCastleEnter::PostDraw()
{
	PostDrawButtons();
}

void UIBloodCastleEnter::DrawFrames()
{
	_BackFrame->Draw();
	_TopFrame->Draw();
	_BottomFrame->Draw();
	_LeftFrame->Draw();
	_RightFrame->Draw();
}

void UIBloodCastleEnter::DrawButtons()
{
	_CloseButton->Draw();

	for (mu_uint32 n = 0; n < MAX_BLOODCASTLE_BRIDGE_COUNT; ++n)
	{
		_EnterButton[n]->Draw();
	}
}

void UIBloodCastleEnter::PostDrawButtons()
{
	_CloseButton->Draw();
}

void UIBloodCastleEnter::DrawTexts()
{
	mu_float screenX = MU_RESOURCE->_640x480Rate[0];
	mu_float screenY = MU_RESOURCE->_640x480Rate[1];
	mu_uint32 textColor = MU::MakeARGB(255, 255, 255, 255);
	mu_uint32 bgColor = MU::MakeARGB(0, 0, 0, 0);

	FONT_BOLD->DrawStringC2D(TEXTINDEX::BCE_TITLE,
		MU_TO_WINDOW(_ViewX + BLOODCASTLEENTER_WIDTH * 0.5f, screenX),
		MU_TO_WINDOW(_ViewY + 15.0f, screenY),
		1.0f,
		ETA_CENTER,
		ETA_CENTER,
		0.0f,
		textColor,
		bgColor,
		GlobalText[846]);

	mu_text txtline[7][38];
	mu_int32 tl = SeparateTextIntoLines(GlobalText[832], txtline[0], 7, 38);
	for (mu_int32 j = 0; j < tl; ++j)
	{
		FONT_NORMAL->DrawStringC2D(TEXTINDEX::BCE_LINES_BEGIN + j,
			MU_TO_WINDOW(_ViewX + 98.0f, screenX),
			MU_TO_WINDOW(_ViewY + 45.0f + j * 20.0f, screenY),
			1.0f,
			ETA_CENTER,
			ETA_BOTTOM,
			0.0f,
			textColor,
			bgColor,
			txtline[j]);
	}
}

UIBase *UIBloodCastleEnter::GetFocus(mu_int32 mouseX, mu_int32 mouseY)
{
	UIBase *focus = UIBase::GetFocus(mouseX, mouseY);

	if (focus != nullptr)
	{
		UIBase *object = _XButton->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}

		object = _CloseButton->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}

		for (mu_uint32 n = 0; n < MAX_BLOODCASTLE_BRIDGE_COUNT; ++n)
		{
			object = _EnterButton[n]->GetFocus(mouseX, mouseY);

			if (object != nullptr)
			{
				return object;
			}
		}
	}

	return focus;
}

mu_int32 UIBloodCastleEnter::CheckLimitLevel(mu_boolean isSpecialLevel)
{
	mu_int32 bridgeIndex = -1;

	if (IsMasterLevel(CharacterAttribute->Class) == true)
	{
		bridgeIndex = MAX_BLOODCASTLE_BRIDGE_COUNT - 1;
	}
	else
	{
		mu_int32 Level = CharacterAttribute->Level;
		if (isSpecialLevel == true)
		{
			for (mu_uint32 n = 0; n < MAX_BLOODCASTLE_BRIDGE_COUNT; ++n)
			{
				if (Level >= g_sttBLOODCASTLE_LEVEL[n].iLOWER_BOUND_MAGUMSA &&
					Level <= g_sttBLOODCASTLE_LEVEL[n].iUPPER_BOUND_MAGUMSA)
				{
					bridgeIndex = n;
					break;
				}
			}
		}
		else
		{
			for (mu_uint32 n = 0; n < MAX_BLOODCASTLE_BRIDGE_COUNT; ++n)
			{
				if (Level >= g_sttBLOODCASTLE_LEVEL[n].iLOWER_BOUND &&
					Level <= g_sttBLOODCASTLE_LEVEL[n].iUPPER_BOUND)
				{
					bridgeIndex = n;
					break;
				}
			}
		}
	}

	return bridgeIndex;
}