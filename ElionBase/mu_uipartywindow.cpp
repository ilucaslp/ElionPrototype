#include "stdafx.h"
#include "mu_uipartywindow.h"

#define SCREEN_WIDTH 640.0f
#define SCREEN_HEIGHT 480.0f

#define BACKTEXTURE_WIDTH 225.0f
#define BACKTEXTURE_HEIGHT 512.0f

#define BACK_POSX PARTYWINDOW_POSX
#define BACK_POSY PARTYWINDOW_POSY
#define BACK_WIDTH PARTYWINDOW_WIDTH
#define BACK_HEIGHT PARTYWINDOW_HEIGHT
#define BACK_UVBEGIN EVector2(0.0f, 0.0f)
#define BACK_UVEND EVector2(BACK_WIDTH / BACKTEXTURE_WIDTH, BACK_HEIGHT / BACKTEXTURE_HEIGHT)

#define TOP_WIDTH PARTYWINDOW_WIDTH
#define TOP_HEIGHT 64.0f
#define TOP_POSX PARTYWINDOW_POSX
#define TOP_POSY PARTYWINDOW_POSY

#define BOTTOM_WIDTH PARTYWINDOW_WIDTH
#define BOTTOM_HEIGHT 45.0f
#define BOTTOM_POSX PARTYWINDOW_POSX
#define BOTTOM_POSY (PARTYWINDOW_POSY + PARTYWINDOW_HEIGHT - BOTTOM_HEIGHT)

#define LEFT_WIDTH 21.0f
#define LEFT_HEIGHT (BOTTOM_POSY - (TOP_POSY + TOP_HEIGHT))
#define LEFT_POSX PARTYWINDOW_POSX
#define LEFT_POSY (TOP_POSY + TOP_HEIGHT)

#define RIGHT_WIDTH 21.0f
#define RIGHT_HEIGHT (BOTTOM_POSY - (TOP_POSY + TOP_HEIGHT))
#define RIGHT_POSX (PARTYWINDOW_POSX + PARTYWINDOW_WIDTH - RIGHT_WIDTH)
#define RIGHT_POSY (TOP_POSY + TOP_HEIGHT)

#define XBUTTON_WIDTH 16.0f
#define XBUTTON_HEIGHT 16.0f
#define XBUTTON_POSX (TOP_POSX + TOP_WIDTH - 7.0f - XBUTTON_WIDTH)
#define XBUTTON_POSY (TOP_POSY + 7.0f)

#define TITLE_WIDTH 70.0f
#define TITLE_HEIGHT 20.0f
#define GROUP_WIDTH 170.0f
#define GROUP_HEIGHT 50.0f

#define SQUARE_WIDTH GROUP_WIDTH
#define SQUARE_HEIGHT (TITLE_HEIGHT + GROUP_HEIGHT)

#define SPACE_HEIGHT (SQUARE_HEIGHT + 1.0f)

#define HPBACK_WIDTH 151.0f
#define HPBACK_HEIGHT 8.0f

#define HPBAR_WIDTH 147.0f
#define HPBAR_HEIGHT 4.0f

UIPartyWindow::UIPartyWindow(UISceneManager *manager) : UIBase()
{
	if (manager != nullptr)
	{
		manager->AddGFxObject(this);
	}

	_SceneManager = manager;

	UIBase::SetAspectRatio(SCREEN_WIDTH, SCREEN_HEIGHT);
	UIBase::SetPosition(PARTYWINDOW_POSX, PARTYWINDOW_POSY);
	UIBase::SetDimensions(PARTYWINDOW_WIDTH, PARTYWINDOW_HEIGHT);

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

		_XButton = new UIButton(GAMEUI::MESSAGE_PARTYWINDOW_CLOSE_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, XBUTTON_POSX, XBUTTON_POSY, XBUTTON_WIDTH, XBUTTON_HEIGHT);

		_CloseButton = new UIButton(GAMEUI::MESSAGE_PARTYWINDOW_CLOSE_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, PARTYWINDOW_POSX + 13.0f, PARTYWINDOW_POSY + 391.0f, 36.0f, 29.0f);
		_CloseButton->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_CloseButton->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EXIT_BUTTON], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(100, 100, 100, 255)));
		_CloseButton->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EXIT_BUTTON], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_CloseButton->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EXIT_BUTTON], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_CloseButton->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EXIT_BUTTON], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		mu_text tipBuffer[256];
		mu_sprintf_s(tipBuffer, mu_countof(tipBuffer), GlobalText[927], _T("P"));
		_CloseButton->SetTooltip(FONT_NORMAL, tipBuffer, MU::MakeARGB(255, 255, 255, 255), MU::MakeARGB(0, 0, 0, 180), true);

		for (mu_int32 n = 0; n < MAX_PARTYS; ++n)
		{
			UIPartyMember &member = _Members[n];

			member.Back1 = new UISpriteColor(manager, this, MU::MakeARGB(0, 0, 0, 229), SCREEN_WIDTH, SCREEN_HEIGHT, PARTYWINDOW_POSX + 13.0f, PARTYWINDOW_POSY + 40.0f + (SPACE_HEIGHT * n) + 2.0f, TITLE_WIDTH - 8.0f, TITLE_HEIGHT);
			member.Back2 = new UISpriteColor(manager, this, MU::MakeARGB(0, 0, 0, 153), SCREEN_WIDTH, SCREEN_HEIGHT, PARTYWINDOW_POSX + 13.0f, PARTYWINDOW_POSY + 40.0f + (SPACE_HEIGHT * n) + 2.0f + TITLE_HEIGHT, GROUP_WIDTH - 8.0f, GROUP_HEIGHT - 7.0f);

			member.TopLeft = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::TABLE_TOP_LEFT], SCREEN_WIDTH, SCREEN_HEIGHT, PARTYWINDOW_POSX + 10.0f, PARTYWINDOW_POSY + 40.0f + (SPACE_HEIGHT * n), 14.0f, 14.0f);
			member.TopRight1 = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::TABLE_TOP_RIGHT], SCREEN_WIDTH, SCREEN_HEIGHT, PARTYWINDOW_POSX + 10.0f + TITLE_WIDTH - 14.0f, PARTYWINDOW_POSY + 40.0f + (SPACE_HEIGHT * n), 14.0f, 14.0f);
			member.TopRight2 = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::TABLE_TOP_RIGHT], SCREEN_WIDTH, SCREEN_HEIGHT, PARTYWINDOW_POSX + 10.0f + GROUP_WIDTH - 14.0f, PARTYWINDOW_POSY + 40.0f + (SPACE_HEIGHT * n) + TITLE_HEIGHT, 14.0f, 14.0f);
			member.BottomLeft = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::TABLE_BOTTOM_LEFT], SCREEN_WIDTH, SCREEN_HEIGHT, PARTYWINDOW_POSX + 10.0f, PARTYWINDOW_POSY + 40.0f + SQUARE_HEIGHT - 14.0f + (SPACE_HEIGHT * n), 14.0f, 14.0f);
			member.BottomRight = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::TABLE_BOTTOM_RIGHT], SCREEN_WIDTH, SCREEN_HEIGHT, PARTYWINDOW_POSX + 10.0f + SQUARE_WIDTH - 14.0f, PARTYWINDOW_POSY + 40.0f + SQUARE_HEIGHT - 14.0f + (SPACE_HEIGHT * n), 14.0f, 14.0f);

			member.TopLine1 = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::TABLE_TOP_LINE], SCREEN_WIDTH, SCREEN_HEIGHT, PARTYWINDOW_POSX + 10.0f + 14.0f, PARTYWINDOW_POSY + 40.0f + (SPACE_HEIGHT * n), TITLE_WIDTH - 28.0f, 14.0f);
			member.TopLine2 = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::TABLE_TOP_LINE], SCREEN_WIDTH, SCREEN_HEIGHT, PARTYWINDOW_POSX + 10.0f + TITLE_WIDTH - 5.0f, PARTYWINDOW_POSY + 40.0f + (SPACE_HEIGHT * n) + TITLE_HEIGHT, GROUP_WIDTH - TITLE_WIDTH - 14.0f + 5.0f, 14.0f);
			member.RightLine1 = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::TABLE_RIGHT_LINE], SCREEN_WIDTH, SCREEN_HEIGHT, PARTYWINDOW_POSX + 10.0f + TITLE_WIDTH - 14.0f, PARTYWINDOW_POSY + 40.0f + (SPACE_HEIGHT * n) + 14.0f, 14.0f, TITLE_HEIGHT - 14.0f);
			member.RightLine2 = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::TABLE_RIGHT_LINE], SCREEN_WIDTH, SCREEN_HEIGHT, PARTYWINDOW_POSX + 10.0f + GROUP_WIDTH - 14.0f, PARTYWINDOW_POSY + 40.0f + (SPACE_HEIGHT * n) + TITLE_HEIGHT + 14.0f, 14.0f, GROUP_HEIGHT - 28.0f);
			member.LeftLine = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::TABLE_LEFT_LINE], SCREEN_WIDTH, SCREEN_HEIGHT, PARTYWINDOW_POSX + 10.0f, PARTYWINDOW_POSY + 40.0f + (SPACE_HEIGHT * n) + 14.0f, 14.0f, SQUARE_HEIGHT - 28.0f);
			member.BottomLine = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::TABLE_BOTTOM_LINE], SCREEN_WIDTH, SCREEN_HEIGHT, PARTYWINDOW_POSX + 10.0f + 14.0f, PARTYWINDOW_POSY + 40.0f + (SPACE_HEIGHT * n) + SQUARE_HEIGHT - 14.0f, GROUP_WIDTH - 28.0f, 14.0f);

			member.HPBack = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::PARTY_HPBAR_BACK], SCREEN_WIDTH, SCREEN_HEIGHT, PARTYWINDOW_POSX + 10.0f + 8.0f, PARTYWINDOW_POSY + 40.0f + (SPACE_HEIGHT * n) + 39.0f, HPBACK_WIDTH, HPBACK_HEIGHT);
			member.HPBar = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::PARTY_HPBAR_COLOR], SCREEN_WIDTH, SCREEN_HEIGHT, PARTYWINDOW_POSX + 10.0f + 10.0f, PARTYWINDOW_POSY + 40.0f + (SPACE_HEIGHT * n) + 41.0f, HPBAR_WIDTH, HPBAR_HEIGHT);

			member.Back1->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
			member.Back2->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

			member.TopLeft->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
			member.TopRight1->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
			member.TopRight2->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
			member.BottomLeft->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
			member.BottomRight->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

			member.TopLine1->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
			member.TopLine2->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
			member.RightLine1->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
			member.RightLine2->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
			member.LeftLine->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
			member.BottomLine->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

			member.KickPlayer = new UIButton(GAMEUI::MESSAGE_PARTYWINDOW_KICK_BEGIN + n,
				manager, this, SCREEN_WIDTH, SCREEN_HEIGHT,
				PARTYWINDOW_POSX + 159.0f, PARTYWINDOW_POSY + 63.0f + (SPACE_HEIGHT * n),
				13.0f, 13.0f);
			member.KickPlayer->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
			member.KickPlayer->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::PARTY_EXIT], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(100, 100, 100, 255)));
			member.KickPlayer->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::PARTY_EXIT], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(200, 200, 200, 255)));
			member.KickPlayer->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::PARTY_EXIT], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
			member.KickPlayer->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::PARTY_EXIT], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		}

		_LeaderFlag = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::PARTY_FLAG], SCREEN_WIDTH, SCREEN_HEIGHT, PARTYWINDOW_POSX + 10.0f + GROUP_WIDTH - 34.0f, PARTYWINDOW_POSY + 40.0f + TITLE_HEIGHT + 3.0f, 10.0f, 12.0f);
		_LeaderFlag->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

		SetPosition(PARTYWINDOW_POSX, PARTYWINDOW_POSY);
	}
}

UIPartyWindow::~UIPartyWindow()
{

}

void UIPartyWindow::ChangePosition(mu_float x, mu_float y)
{
#define SETPOSITION_HELPER(var) var->SetPosition(var->_ViewX - oldX + x, var->_ViewY - oldY + y)

	mu_float oldX = _ViewX, oldY = _ViewY;

	SETPOSITION_HELPER(this);

	SETPOSITION_HELPER(_BackFrame);
	SETPOSITION_HELPER(_TopFrame);
	SETPOSITION_HELPER(_BottomFrame);
	SETPOSITION_HELPER(_LeftFrame);
	SETPOSITION_HELPER(_RightFrame);

	SETPOSITION_HELPER(_CloseButton);

	for (mu_int32 n = 0; n < MAX_PARTYS; ++n)
	{
		UIPartyMember &member = _Members[n];

		SETPOSITION_HELPER(member.Back1);
		SETPOSITION_HELPER(member.Back2);
		
		SETPOSITION_HELPER(member.TopLeft);
		SETPOSITION_HELPER(member.TopRight1);
		SETPOSITION_HELPER(member.TopRight2);
		SETPOSITION_HELPER(member.BottomLeft);
		SETPOSITION_HELPER(member.BottomRight);
		
		SETPOSITION_HELPER(member.TopLine1);
		SETPOSITION_HELPER(member.TopLine2);
		SETPOSITION_HELPER(member.RightLine1);
		SETPOSITION_HELPER(member.RightLine2);
		SETPOSITION_HELPER(member.LeftLine);
		SETPOSITION_HELPER(member.BottomLine);

		SETPOSITION_HELPER(member.KickPlayer);
	}

	SETPOSITION_HELPER(_LeaderFlag);

#undef SETPOSITION_HELPER
}

void UIPartyWindow::Update(mu_uint64 CurrentTime)
{

}

void UIPartyWindow::Draw()
{
	DrawFrames();
	DrawButtons();

	mu_float screenX = MU_RESOURCE->_640x480Rate[0];
	mu_float screenY = MU_RESOURCE->_640x480Rate[1];
	mu_uint32 textColor = MU::MakeARGB(255, 255, 255, 255);
	mu_uint32 bgColor = MU::MakeARGB(0, 0, 0, 0);

	FONT_BOLD->DrawStringC2D(TEXTINDEX::PARTYWINDOW_TITLE,
		MU_TO_WINDOW(_ViewX + PARTYWINDOW_WIDTH * 0.5f, screenX),
		MU_TO_WINDOW(_ViewY + 15.0f, screenY),
		1.0f,
		ETA_CENTER,
		ETA_CENTER,
		0.0f,
		textColor,
		bgColor,
		GlobalText[190]);

	if (PartyNumber > 0)
	{
		for (mu_uint32 n = 0; n < PartyNumber; ++n)
		{
			UIPartyMember &member = _Members[n];
			PARTY_t &info = Party[n];

			mu_float x = _ViewX + 10.0f;
			mu_float y = _ViewY + 40.0f + SPACE_HEIGHT * n;

			member.Back1->Draw();
			member.Back2->Draw();

			member.TopLeft->Draw(); 
			member.TopRight1->Draw();
			member.TopRight2->Draw();
			member.BottomLeft->Draw();
			member.BottomRight->Draw();

			member.TopLine1->Draw(); 
			member.TopLine2->Draw();
			member.RightLine1->Draw();
			member.RightLine2->Draw();
			member.LeftLine->Draw();
			member.BottomLine->Draw();

			if (Party[0].Key == HeroKey ||
				info.Key == HeroKey)
			{
				member.KickPlayer->Draw();
			}

			if (n == 0)
			{
				FONT_BOLD->DrawStringC2D(TEXTINDEX::PARTYWINDOW_MEMBER_NAME_BEGIN + n,
					MU_TO_WINDOW(x + TITLE_WIDTH * 0.5f, screenX),
					MU_TO_WINDOW(y + TITLE_HEIGHT * 0.5f, screenY),
					1.0f, ETA_CENTER, ETA_CENTER,
					0.0f, MU::MakeARGB(0, 255, 0, 255), bgColor,
					info.Name.c_str());

				_LeaderFlag->Draw();
			}
			else
			{
				FONT_BOLD->DrawStringC2D(TEXTINDEX::PARTYWINDOW_MEMBER_NAME_BEGIN + n,
					MU_TO_WINDOW(x + TITLE_WIDTH * 0.5f, screenX),
					MU_TO_WINDOW(y + TITLE_HEIGHT * 0.5f, screenY),
					1.0f, ETA_CENTER, ETA_CENTER,
					0.0f, MU::MakeARGB(255, 255, 255, 255), bgColor,
					info.Name.c_str());
			}

			mu_float hpPercent = info.maxHP > 0.0f ? info.currHP / info.maxHP : 0.0f;
			if (hpPercent > 1.0f) hpPercent = 1.0f;

			member.HPBack->Draw();
			if(hpPercent > 0.0f)
			{
				member.HPBar->SetDimensions(HPBAR_WIDTH * hpPercent, HPBAR_HEIGHT);
				member.HPBar->Draw();
			}

			FONT_BOLD->DrawStringC2D(TEXTINDEX::PARTYWINDOW_MEMBER_MAPNAME_BEGIN + n,
				MU_TO_WINDOW(x + 10.0f, screenX),
				MU_TO_WINDOW(y + 26.0f, screenY),
				1.0f, ETA_LEFT, ETA_BOTTOM,
				0.0f, MU::MakeARGB(194, 194, 194, 255), bgColor,
				GetMapName(info.Map));

			mu_text buffer[256] = { 0 };
			mu_sprintf_s(buffer, mu_countof(buffer), _T("(%d,%d)"), info.x, info.y);

			FONT_BOLD->DrawStringC2D(TEXTINDEX::PARTYWINDOW_MEMBER_MAPNAME_BEGIN + n,
				MU_TO_WINDOW(x + 85.0f, screenX),
				MU_TO_WINDOW(y + 26.0f, screenY),
				1.0f, ETA_LEFT, ETA_BOTTOM,
				0.0f, MU::MakeARGB(194, 194, 194, 255), bgColor,
				buffer);

			mu_sprintf_s(buffer, mu_countof(buffer), _T("%d ") SPF_STRING _T(" %d"), (mu_int32)info.currHP, GlobalText[2374], (mu_int32)info.maxHP);

			FONT_BOLD->DrawStringC2D(TEXTINDEX::PARTYWINDOW_MEMBER_MAPNAME_BEGIN + n,
				MU_TO_WINDOW(x + GROUP_WIDTH - 11.0f, screenX),
				MU_TO_WINDOW(y + 51.0f, screenY),
				1.0f, ETA_RIGHT, ETA_BOTTOM,
				0.0f, MU::MakeARGB(194, 194, 194, 255), bgColor,
				buffer);
		}
	}
	else
	{
		static mu_text partyInfo[512] = { 0 };
		static mu_boolean setPartyInfo = false;

		if (setPartyInfo == false)
		{
			setPartyInfo = true;
			mu_sprintf_s(partyInfo, mu_countof(partyInfo), _T("") SPF_STRING _T("\n\n") SPF_STRING _T("\n\n") SPF_STRING _T("\n\n") SPF_STRING _T("\n\n") SPF_STRING _T("\n\n") SPF_STRING _T("\n\n") SPF_STRING _T(""),
				GlobalText[191],
				GlobalText[192],
				GlobalText[193],
				GlobalText[194],
				GlobalText[195],
				GlobalText[196],
				GlobalText[197]);
		}

		FONT_NORMAL->DrawStringC2D(TEXTINDEX::PARTYWINDOW_INFO,
			MU_TO_WINDOW(_ViewX + 20.0f, screenX),
			MU_TO_WINDOW(_ViewY + 60.0f + 0.0f, screenY),
			1.0f,
			ETA_LEFT,
			ETA_BOTTOM,
			0.0f,
			textColor,
			bgColor,
			partyInfo);
	}
}

void UIPartyWindow::PostDraw()
{
	PostDrawButtons();

	if (PartyNumber > 0)
	{
		for (mu_uint32 n = 0; n < PartyNumber; ++n)
		{
			UIPartyMember &member = _Members[n];
			PARTY_t &info = Party[n];

			if (Party[0].Key == HeroKey ||
				info.Key == HeroKey)
			{
				member.KickPlayer->PostDraw();
			}
		}
	}
}

void UIPartyWindow::DrawFrames()
{
	_BackFrame->Draw();
	_TopFrame->Draw();
	_BottomFrame->Draw();
	_LeftFrame->Draw();
	_RightFrame->Draw();
}

void UIPartyWindow::DrawButtons()
{
	_CloseButton->Draw();
}

void UIPartyWindow::PostDrawButtons()
{
	_CloseButton->PostDraw();
}

UIBase *UIPartyWindow::GetFocus(mu_int32 mouseX, mu_int32 mouseY)
{
	UIBase *focus = UIBase::GetFocus(mouseX, mouseY);

	if (focus != nullptr)
	{
		UIBase *object = _CloseButton->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}

		object = _XButton->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}

		if (PartyNumber > 0)
		{
			for (mu_uint32 n = 0; n < PartyNumber; ++n)
			{
				if (Party[0].Key != HeroKey &&
					Party[n].Key != HeroKey)
					continue;

				UIPartyMember &member = _Members[n];
				object = member.KickPlayer->GetFocus(mouseX, mouseY);

				if (object != nullptr)
				{
					return object;
				}
			}
		}
	}

	return focus;
}