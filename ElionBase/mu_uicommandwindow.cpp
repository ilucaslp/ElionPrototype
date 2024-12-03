#include "stdafx.h"
#include "mu_uicommandwindow.h"

#define SCREEN_WIDTH 640.0f
#define SCREEN_HEIGHT 480.0f

#define BACKTEXTURE_WIDTH 225.0f
#define BACKTEXTURE_HEIGHT 512.0f

#define BACK_POSX COMMANDWINDOW_POSX
#define BACK_POSY COMMANDWINDOW_POSY
#define BACK_WIDTH COMMANDWINDOW_WIDTH
#define BACK_HEIGHT COMMANDWINDOW_HEIGHT
#define BACK_UVBEGIN EVector2(0.0f, 0.0f)
#define BACK_UVEND EVector2(BACK_WIDTH / BACKTEXTURE_WIDTH, BACK_HEIGHT / BACKTEXTURE_HEIGHT)

#define TOP_WIDTH COMMANDWINDOW_WIDTH
#define TOP_HEIGHT 64.0f
#define TOP_POSX COMMANDWINDOW_POSX
#define TOP_POSY COMMANDWINDOW_POSY

#define BOTTOM_WIDTH COMMANDWINDOW_WIDTH
#define BOTTOM_HEIGHT 45.0f
#define BOTTOM_POSX COMMANDWINDOW_POSX
#define BOTTOM_POSY (COMMANDWINDOW_POSY + COMMANDWINDOW_HEIGHT - BOTTOM_HEIGHT)

#define LEFT_WIDTH 21.0f
#define LEFT_HEIGHT (BOTTOM_POSY - (TOP_POSY + TOP_HEIGHT))
#define LEFT_POSX COMMANDWINDOW_POSX
#define LEFT_POSY (TOP_POSY + TOP_HEIGHT)

#define RIGHT_WIDTH 21.0f
#define RIGHT_HEIGHT (BOTTOM_POSY - (TOP_POSY + TOP_HEIGHT))
#define RIGHT_POSX (COMMANDWINDOW_POSX + COMMANDWINDOW_WIDTH - RIGHT_WIDTH)
#define RIGHT_POSY (TOP_POSY + TOP_HEIGHT)

#define XBUTTON_WIDTH 16.0f
#define XBUTTON_HEIGHT 16.0f
#define XBUTTON_POSX (TOP_POSX + TOP_WIDTH - 7.0f - XBUTTON_WIDTH)
#define XBUTTON_POSY (TOP_POSY + 7.0f)

#define COMMAND_WIDTH (108.0f)
#define COMMAND_HEIGHT (29.0f)
#define COMMAND_POSX (COMMANDWINDOW_POSX + COMMANDWINDOW_WIDTH * 0.5f - COMMAND_WIDTH * 0.5f)
#define COMMAND_POSY(n) (COMMANDWINDOW_POSY + 33.0f + (n * (COMMAND_HEIGHT + 1.0f)))

UICommandWindow::UICommandWindow(UISceneManager *manager) : UIBase()
{
	if (manager != nullptr)
	{
		manager->AddGFxObject(this);
	}

	_SceneManager = manager;

	UIBase::SetAspectRatio(SCREEN_WIDTH, SCREEN_HEIGHT);
	UIBase::SetPosition(COMMANDWINDOW_POSX, COMMANDWINDOW_POSY);
	UIBase::SetDimensions(COMMANDWINDOW_WIDTH, COMMANDWINDOW_HEIGHT);

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

		_XButton = new UIButton(GAMEUI::MESSAGE_COMMANDWINDOW_CLOSE_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, XBUTTON_POSX, XBUTTON_POSY, XBUTTON_WIDTH, XBUTTON_HEIGHT);

		_CloseButton = new UIButton(GAMEUI::MESSAGE_COMMANDWINDOW_CLOSE_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, COMMANDWINDOW_POSX + 13.0f, COMMANDWINDOW_POSY + 391.0f, 36.0f, 29.0f);
		_CloseButton->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_CloseButton->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EXIT_BUTTON], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(100, 100, 100, 255)));
		_CloseButton->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EXIT_BUTTON], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_CloseButton->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EXIT_BUTTON], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_CloseButton->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EXIT_BUTTON], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		mu_text tipBuffer[256];
		mu_sprintf_s(tipBuffer, mu_countof(tipBuffer), GlobalText[927], _T("D"));
		_CloseButton->SetTooltip(FONT_NORMAL, tipBuffer, MU::MakeARGB(255, 255, 255, 255), MU::MakeARGB(0, 0, 0, 180), true);

		mu_uint32 messages[UICMD_MAX] =
		{
			943,
			1124,
			944,
			945,
			946,
			1352,
			1321,
			1322,
			947,
			948,
			949,
		};
		for (mu_uint32 n = 0; n < UICMD_MAX; ++n)
		{
			mu_uint32 msgIndex = messages[n];
			_Commands[n] = new UIToggleButton(GAMEUI::MESSAGE_COMMANDWINDOW_BEGIN + n, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, COMMAND_POSX, COMMAND_POSY(n), COMMAND_WIDTH, COMMAND_HEIGHT);
			_Commands[n]->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
			_Commands[n]->SetStateData(UIToggleButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTY_BUTTON], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(100, 100, 100, 255)));
			_Commands[n]->SetStateData(UIToggleButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTY_BUTTON], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(255, 255, 255, 255)));
			_Commands[n]->SetStateData(UIToggleButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTY_BUTTON], TUV_THREE_OVER_BEGIN, TUV_THREE_OVER_END, MU::MakeARGB(255, 255, 255, 255)));
			_Commands[n]->SetStateData(UIToggleButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTY_BUTTON], TUV_THREE_PRESSED_BEGIN, TUV_THREE_PRESSED_END, MU::MakeARGB(200, 200, 200, 255)));
			_Commands[n]->SetStateData(UIToggleButton::UI_PRESSED_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTY_BUTTON], TUV_THREE_PRESSED_BEGIN, TUV_THREE_PRESSED_END, MU::MakeARGB(255, 255, 255, 255)));
			_Commands[n]->SetStateText(UIToggleButton::UI_DISABLED, FONT_NORMAL, GlobalText[msgIndex], MU::MakeARGB(170, 170, 170, 255));
			_Commands[n]->SetStateText(UIToggleButton::UI_NORMAL, FONT_NORMAL, GlobalText[msgIndex], MU::MakeARGB(200, 200, 200, 255));
			_Commands[n]->SetStateText(UIToggleButton::UI_OVER, FONT_NORMAL, GlobalText[msgIndex], MU::MakeARGB(255, 255, 255, 255));
			_Commands[n]->SetStateText(UIToggleButton::UI_PRESSED, FONT_NORMAL, GlobalText[msgIndex], MU::MakeARGB(200, 200, 200, 255));
			_Commands[n]->SetStateText(UIToggleButton::UI_PRESSED_OVER, FONT_NORMAL, GlobalText[msgIndex], MU::MakeARGB(255, 255, 255, 255));
		}

		_PlayerButton = new UIButton(GAMEUI::MESSAGE_COMMANDWINDOW_PLAYER_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 16.0f, 16.0f);

		_PlayerNameBack = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::COMMANDFRAME_CURSORID_WND], 800.0f, 600.0f, 0.0f, 0.0f, 128.0f, 32.0f);

		ClosingWindow();
	}
}

UICommandWindow::~UICommandWindow()
{

}

void UICommandWindow::ClosingWindow()
{
	for (mu_uint32 n = 0; n < UICMD_MAX; ++n)
	{
		_Commands[n]->ChangeState(UIToggleButton::UI_NORMAL);
	}

	_SelectedCommand = -1;
}

void UICommandWindow::Update(mu_uint64 CurrentTime)
{

}

void UICommandWindow::Draw()
{
	DrawFrames();
	DrawButtons();

	mu_float screenX = MU_RESOURCE->_640x480Rate[0];
	mu_float screenY = MU_RESOURCE->_640x480Rate[1];
	mu_uint32 textColor = MU::MakeARGB(255, 255, 255, 255);
	mu_uint32 bgColor = MU::MakeARGB(0, 0, 0, 0);

	FONT_BOLD->DrawStringC2D(TEXTINDEX::COMMANDWINDOW_TITLE,
		MU_TO_WINDOW(_ViewX + COMMANDWINDOW_WIDTH * 0.5f, screenX),
		MU_TO_WINDOW(_ViewY + 15.0f, screenY),
		1.0f,
		ETA_CENTER,
		ETA_CENTER,
		0.0f,
		textColor,
		bgColor,
		GlobalText[938]);

	if (_SelectedCommand != -1 &&
		SelectedCharacter != -1)
	{
		CHARACTER *pChar = &CharactersClient[SelectedCharacter];

		EFloat dis = EMath::Distance(pChar->Object.Position, Hero->Object.Position);

		if (pChar->Object.Kind == KIND_PLAYER &&
			pChar != Hero &&
			(pChar->Object.Type == MODEL_PLAYER || pChar->Change == true) &&
			(_SelectedCommand == UICMD_WHISPER || dis <= 300.0f))
		{
			_PlayerNameBack->SetPosition(MU_INPUT->GetMouseX() + MU_INPUT->_Cursor->_Width * 0.5f,
				MU_INPUT->GetMouseY() + MU_INPUT->_Cursor->_Height * 0.5f, false);
			_PlayerNameBack->Draw();

			FONT_BOLD->DrawStringC2D(TEXTINDEX::COMMANDWINDOW_TITLE,
				_PlayerNameBack->_NativeX + _PlayerNameBack->_Width * 0.5f,
				_PlayerNameBack->_NativeY + _PlayerNameBack->_Height * 0.5f,
				1.0f,
				ETA_CENTER,
				ETA_CENTER,
				0.0f,
				textColor,
				bgColor,
				pChar->ID);
		}
	}
}

void UICommandWindow::PostDraw()
{
	PostDrawButtons();
}

void UICommandWindow::DrawFrames()
{
	_BackFrame->Draw();
	_TopFrame->Draw();
	_BottomFrame->Draw();
	_LeftFrame->Draw();
	_RightFrame->Draw();
}

void UICommandWindow::DrawButtons()
{
	_CloseButton->Draw();

	for (mu_uint32 n = 0; n < UICMD_MAX; ++n)
	{
		_Commands[n]->Draw();
	}
}

void UICommandWindow::PostDrawButtons()
{
	_CloseButton->PostDraw();

	for (mu_uint32 n = 0; n < UICMD_MAX; ++n)
	{
		_Commands[n]->PostDraw();
	}
}

UIBase *UICommandWindow::GetFocus(mu_int32 mouseX, mu_int32 mouseY)
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

		for (mu_uint32 n = 0; n < UICMD_MAX; ++n)
		{
			object = _Commands[n]->GetFocus(mouseX, mouseY);

			if (object != nullptr)
			{
				return object;
			}
		}
	}
	else
	{
		if (_SelectedCommand != -1 &&
			SelectedCharacter != -1)
		{
			_PlayerButton->SetPosition(mouseX - _PlayerButton->_Width * 0.5f, mouseY - _PlayerButton->_Height * 0.5f, false);

			UIBase *object = _PlayerButton->GetFocus(mouseX, mouseY);

			if (object != nullptr)
			{
				return object;
			}
		}
	}

	return focus;
}

mu_boolean UICommandWindow::CommandTrade()
{
	if (SelectedCharacter == -1)
	{
		return false;
	}

	CHARACTER *pChar = &CharactersClient[SelectedCharacter];
	OBJECT *pObj = &pChar->Object;

	if (pObj->Kind != KIND_PLAYER ||
		pChar == Hero ||
		(pObj->Type != MODEL_PLAYER && pChar->Change == false))
	{
		return false;
	}

	EFloat dis = EMath::Distance(pObj->Position, Hero->Object.Position);

	if (dis > 300.0f)
	{
		return false;
	}

	if (InChaosCastle() == true)
	{
		g_pChatListBox->AddText(_T(""), GlobalText[1150], TYPE_SYSTEM_MESSAGE);
		return false;
	}

	if (IsStrifeMap(GET_WORLD))
	{
		g_pChatListBox->AddText(_T(""), GlobalText[3147], TYPE_SYSTEM_MESSAGE);
		return false;
	}

	if (CharacterAttribute->Level < TRADELIMITLEVEL)
	{
		g_pChatListBox->AddText(_T(""), GlobalText[478], TYPE_SYSTEM_MESSAGE);
		return false;
	}

#if 0 // FIX
	if (IsShopInViewport(c))
	{
		g_pChatListBox->AddText(_T(""), GlobalText[493], TYPE_ERROR_MESSAGE);
		return true;
	}
#endif

	g_GameServerObject->SendRequestTrade(pChar->Key);

	return true;
}

mu_boolean UICommandWindow::CommandBuy()
{
	if (SelectedCharacter == -1)
	{
		return false;
	}

	CHARACTER *pChar = &CharactersClient[SelectedCharacter];
	OBJECT *pObj = &pChar->Object;

	if (pObj->Kind != KIND_PLAYER ||
		pChar == Hero ||
		(pObj->Type != MODEL_PLAYER && pChar->Change == false))
	{
		return false;
	}

	EFloat dis = EMath::Distance(pObj->Position, Hero->Object.Position);

	if (dis > 300.0f)
	{
		return false;
	}

	return true;
}

mu_boolean UICommandWindow::CommandParty()
{
	if (SelectedCharacter == -1)
	{
		return false;
	}

	CHARACTER *pChar = &CharactersClient[SelectedCharacter];
	OBJECT *pObj = &pChar->Object;

	if (pObj->Kind != KIND_PLAYER ||
		pChar == Hero ||
		(pObj->Type != MODEL_PLAYER && pChar->Change == false))
	{
		return false;
	}

	EFloat dis = EMath::Distance(pObj->Position, Hero->Object.Position);

	if (dis > 300.0f)
	{
		return false;
	}

	if (InChaosCastle() == true)
	{
		g_pChatListBox->AddText(_T(""), GlobalText[1150], TYPE_SYSTEM_MESSAGE);
		return false;
	}

	if (PartyNumber > 0 && Party[0].Key != HeroKey)
	{
		g_pChatListBox->AddText(_T(""), GlobalText[257], TYPE_SYSTEM_MESSAGE);
		return true;
	}

	g_GameServerObject->SendRequestParty(pChar->Key);

	return true;
}

mu_boolean UICommandWindow::CommandWhisper()
{
	if (SelectedCharacter == -1)
	{
		return false;
	}

	CHARACTER *pChar = &CharactersClient[SelectedCharacter];
	OBJECT *pObj = &pChar->Object;

	if (pObj->Kind != KIND_PLAYER ||
		pChar == Hero ||
		(pObj->Type != MODEL_PLAYER && pChar->Change == false))
	{
		return false;
	}

	g_pChatInputBox->SetWhisperID(pChar->ID);

	return true;
}

mu_boolean UICommandWindow::CommandGuild()
{
	if (SelectedCharacter == -1)
	{
		return false;
	}

	CHARACTER *pChar = &CharactersClient[SelectedCharacter];
	OBJECT *pObj = &pChar->Object;

	if (pObj->Kind != KIND_PLAYER ||
		pChar == Hero ||
		(pObj->Type != MODEL_PLAYER && pChar->Change == false))
	{
		return false;
	}

	EFloat dis = EMath::Distance(pObj->Position, Hero->Object.Position);

	if (dis > 300.0f)
	{
		return false;
	}

	if (InChaosCastle() == true)
	{
		g_pChatListBox->AddText(_T(""), GlobalText[1150], TYPE_SYSTEM_MESSAGE);
		return false;
	}

	if (Hero->GuildStatus != GAME::G_NONE)
	{
		g_pChatListBox->AddText(_T(""), GlobalText[255], TYPE_SYSTEM_MESSAGE);
		return true;
	}

	CL2GS_REQUEST_GUILDJOIN RequestGuildJoin;

	RequestGuildJoin.Key = pChar->Key;

	g_GameServerObject->Send(&RequestGuildJoin);

	return true;
}

mu_boolean UICommandWindow::CommandAlliance()
{
	if (SelectedCharacter == -1)
	{
		return false;
	}

	CHARACTER *pChar = &CharactersClient[SelectedCharacter];
	OBJECT *pObj = &pChar->Object;

	if (pObj->Kind != KIND_PLAYER ||
		pChar == Hero ||
		(pObj->Type != MODEL_PLAYER && pChar->Change == false))
	{
		return false;
	}

	EFloat dis = EMath::Distance(pObj->Position, Hero->Object.Position);

	if (dis > 300.0f)
	{
		return false;
	}

	return true;
}

mu_boolean UICommandWindow::CommandHostility()
{
	if (SelectedCharacter == -1)
	{
		return false;
	}

	CHARACTER *pChar = &CharactersClient[SelectedCharacter];
	OBJECT *pObj = &pChar->Object;

	if (pObj->Kind != KIND_PLAYER ||
		pChar == Hero ||
		(pObj->Type != MODEL_PLAYER && pChar->Change == false))
	{
		return false;
	}

	EFloat dis = EMath::Distance(pObj->Position, Hero->Object.Position);

	if (dis > 300.0f)
	{
		return false;
	}

	return true;
}

mu_boolean UICommandWindow::CommandSuspendHostility()
{
	if (SelectedCharacter == -1)
	{
		return false;
	}

	CHARACTER *pChar = &CharactersClient[SelectedCharacter];
	OBJECT *pObj = &pChar->Object;

	if (pObj->Kind != KIND_PLAYER ||
		pChar == Hero ||
		(pObj->Type != MODEL_PLAYER && pChar->Change == false))
	{
		return false;
	}

	EFloat dis = EMath::Distance(pObj->Position, Hero->Object.Position);

	if (dis > 300.0f)
	{
		return false;
	}

	return true;
}

mu_boolean UICommandWindow::CommandAddFriend()
{
	if (SelectedCharacter == -1)
	{
		return false;
	}

	CHARACTER *pChar = &CharactersClient[SelectedCharacter];
	OBJECT *pObj = &pChar->Object;

	if (pObj->Kind != KIND_PLAYER ||
		pChar == Hero ||
		(pObj->Type != MODEL_PLAYER && pChar->Change == false))
	{
		return false;
	}

	EFloat dis = EMath::Distance(pObj->Position, Hero->Object.Position);

	if (dis > 300.0f)
	{
		return false;
	}

	return true;
}

mu_boolean UICommandWindow::CommandFollow()
{
	if (SelectedCharacter == -1)
	{
		return false;
	}

	CHARACTER *pChar = &CharactersClient[SelectedCharacter];
	OBJECT *pObj = &pChar->Object;

	if (pObj->Kind != KIND_PLAYER ||
		pChar == Hero ||
		(pObj->Type != MODEL_PLAYER && pChar->Change == false))
	{
		return false;
	}

	EFloat dis = EMath::Distance(pObj->Position, Hero->Object.Position);

	if (dis > 300.0f)
	{
		return false;
	}

	g_iFollowCharacter = SelectedCharacter;

	return true;
}

mu_boolean UICommandWindow::CommandDuel()
{
	if (g_DuelMgr.IsDuelEnabled() == true)
	{
		g_pChatListBox->AddText(_T(""), GlobalText[915], TYPE_SYSTEM_MESSAGE);
		return false;
	}

	if (SelectedCharacter == -1)
	{
		return false;
	}

	CHARACTER *pChar = &CharactersClient[SelectedCharacter];
	OBJECT *pObj = &pChar->Object;

	if (pObj->Kind != KIND_PLAYER ||
		pChar == Hero ||
		(pObj->Type != MODEL_PLAYER && pChar->Change == false))
	{
		return false;
	}

	EFloat dis = EMath::Distance(pObj->Position, Hero->Object.Position);

	if (dis > 300.0f)
	{
		return false;
	}

	if (InChaosCastle() == true)
	{
		g_pChatListBox->AddText(_T(""), GlobalText[1150], TYPE_SYSTEM_MESSAGE);
		return false;
	}

	mu_int32 iLevel = CharacterAttribute->Level;
	if (iLevel < 30)
	{
		mu_unicode szError[48];
		mu_sprintf_s(szError, mu_countof(szError), GlobalText[2704], 30);
		g_pChatListBox->AddText(_T(""), szError, TYPE_ERROR_MESSAGE);
		return false;
	}
	else if (SelectedCharacter != -1)
	{
		CHARACTER *c = &CharactersClient[SelectedCharacter];
		OBJECT *o = &c->Object;
		EFloat dis = EMath::Distance(o->Position, Hero->Object.Position);
		if (o->Kind == KIND_PLAYER && c != Hero && (o->Type == MODEL_PLAYER || c->Change) &&
			dis <= 300.0f)
		{
#if 0 // FIX
			SendRequestDuelStart(c->Key, c->ID);
#endif
		}
	}

	return true;
}