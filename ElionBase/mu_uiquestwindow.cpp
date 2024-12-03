#include "stdafx.h"
#include "mu_uiquestwindow.h"

#define SCREEN_WIDTH 640.0f
#define SCREEN_HEIGHT 480.0f

#define BACKTEXTURE_WIDTH 225.0f
#define BACKTEXTURE_HEIGHT 512.0f

#define BACK_POSX QUESTWINDOW_POSX
#define BACK_POSY QUESTWINDOW_POSY
#define BACK_WIDTH QUESTWINDOW_WIDTH
#define BACK_HEIGHT QUESTWINDOW_HEIGHT
#define BACK_UVBEGIN EVector2(0.0f, 0.0f)
#define BACK_UVEND EVector2(BACK_WIDTH / BACKTEXTURE_WIDTH, BACK_HEIGHT / BACKTEXTURE_HEIGHT)

#define TOP_WIDTH QUESTWINDOW_WIDTH
#define TOP_HEIGHT 64.0f
#define TOP_POSX QUESTWINDOW_POSX
#define TOP_POSY QUESTWINDOW_POSY

#define BOTTOM_WIDTH QUESTWINDOW_WIDTH
#define BOTTOM_HEIGHT 45.0f
#define BOTTOM_POSX QUESTWINDOW_POSX
#define BOTTOM_POSY (QUESTWINDOW_POSY + QUESTWINDOW_HEIGHT - BOTTOM_HEIGHT)

#define LEFT_WIDTH 21.0f
#define LEFT_HEIGHT (BOTTOM_POSY - (TOP_POSY + TOP_HEIGHT))
#define LEFT_POSX QUESTWINDOW_POSX
#define LEFT_POSY (TOP_POSY + TOP_HEIGHT)

#define RIGHT_WIDTH 21.0f
#define RIGHT_HEIGHT (BOTTOM_POSY - (TOP_POSY + TOP_HEIGHT))
#define RIGHT_POSX (QUESTWINDOW_POSX + QUESTWINDOW_WIDTH - RIGHT_WIDTH)
#define RIGHT_POSY (TOP_POSY + TOP_HEIGHT)

#define XBUTTON_WIDTH 16.0f
#define XBUTTON_HEIGHT 16.0f
#define XBUTTON_POSX (TOP_POSX + TOP_WIDTH - 7.0f - XBUTTON_WIDTH)
#define XBUTTON_POSY (TOP_POSY + 7.0f)

#define CLOSEBUTTON_POSX (QUESTWINDOW_POSX + 13.0f)
#define CLOSEBUTTON_POSY (QUESTWINDOW_POSY + 392.0f)
#define CLOSEBUTTON_WIDTH (36.0f)
#define CLOSEBUTTON_HEIGHT (29.0f)

#define TABBACK_POSX (QUESTWINDOW_POSX + 10.0f)
#define TABBACK_POSY (QUESTWINDOW_POSY + 27.0f)
#define TABBACK_WIDTH (166.0f)
#define TABBACK_HEIGHT (22.0f)

#define TABBUTTON_WIDTH (48.0f)
#define TABBUTTON_WIDTH2 (72.0f)
#define TABBUTTON_HEIGHT TABBACK_HEIGHT
#define TABBUTTON_POSX(n) (TABBACK_POSX + n * (TABBUTTON_WIDTH - 1.0f))
#define TABBUTTON_POSY TABBACK_POSY

// Quest Tab
#define QUESTTAB_START_POSX (QUESTWINDOW_POSX + 50.0f)
#define QUESTTAB_START_POSY (QUESTWINDOW_POSY + 392.0f)
#define QUESTTAB_START_WIDTH (36.0f)
#define QUESTTAB_START_HEIGHT (29.0f)
#define QUESTTAB_GIVEUP_POSX (QUESTWINDOW_POSX + 87.0f)
#define QUESTTAB_GIVEUP_POSY (QUESTWINDOW_POSY + 392.0f)
#define QUESTTAB_GIVEUP_WIDTH (36.0f)
#define QUESTTAB_GIVEUP_HEIGHT (29.0f)

UIQuestWindow::UIQuestWindow(UISceneManager *manager) : UIBase()
{
	if (manager != nullptr)
	{
		manager->AddGFxObject(this);
	}

	_SceneManager = manager;

	UIBase::SetAspectRatio(SCREEN_WIDTH, SCREEN_HEIGHT);
	UIBase::SetPosition(QUESTWINDOW_POSX, QUESTWINDOW_POSY);
	UIBase::SetDimensions(QUESTWINDOW_WIDTH, QUESTWINDOW_HEIGHT);

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

		_XButton = new UIButton(GAMEUI::MESSAGE_QUESTWINDOW_CLOSE_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, XBUTTON_POSX, XBUTTON_POSY, XBUTTON_WIDTH, XBUTTON_HEIGHT);

		mu_text buffer[256];
		mu_sprintf_s(buffer, mu_countof(buffer), GlobalText[927], _T("T"));
		_CloseButton = new UIButton(GAMEUI::MESSAGE_QUESTWINDOW_CLOSE_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, CLOSEBUTTON_POSX, CLOSEBUTTON_POSY, CLOSEBUTTON_WIDTH, CLOSEBUTTON_HEIGHT);
		_CloseButton->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_CloseButton->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EXIT_BUTTON], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(100, 100, 100, 255)));
		_CloseButton->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EXIT_BUTTON], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_CloseButton->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EXIT_BUTTON], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_CloseButton->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EXIT_BUTTON], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_CloseButton->SetTooltip(FONT_NORMAL, buffer, MU::MakeARGB(255, 255, 255, 255), MU::MakeARGB(0, 0, 0, 180), true);

		_TabBack = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::MYQUEST_TAB1], SCREEN_WIDTH, SCREEN_HEIGHT, TABBACK_POSX, TABBACK_POSY, TABBACK_WIDTH, TABBACK_HEIGHT);
		_TabBack->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

		mu_uint32 tabsText[TAB_MAX] =
		{
			1140,
			2821,
			2824,
		};

		for (mu_uint32 n = 0; n < TAB_MAX; ++n)
		{
			if (n == TAB_CASTLETEMPLE)
			{
				_TabButtons[n] = new UIRadioButton(GAMEUI::MESSAGE_QUESTWINDOW_TAB_BEGIN + n, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, TABBUTTON_POSX(n), TABBUTTON_POSY, TABBUTTON_WIDTH2, TABBUTTON_HEIGHT);
				_TabButtons[n]->SetStateData(UIRadioButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MYQUEST_TAB3], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
				_TabButtons[n]->SetStateData(UIRadioButton::UI_PRESSED_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MYQUEST_TAB3], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
			}
			else
			{
				_TabButtons[n] = new UIRadioButton(GAMEUI::MESSAGE_QUESTWINDOW_TAB_BEGIN + n, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, TABBUTTON_POSX(n), TABBUTTON_POSY, TABBUTTON_WIDTH, TABBUTTON_HEIGHT);
				_TabButtons[n]->SetStateData(UIRadioButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MYQUEST_TAB2], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
				_TabButtons[n]->SetStateData(UIRadioButton::UI_PRESSED_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MYQUEST_TAB2], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
			}

			_TabButtons[n]->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

			_TabButtons[n]->SetStateText(UIRadioButton::UI_DISABLED, FONT_NORMAL, GlobalText[tabsText[n]], MU::MakeARGB(100, 100, 100, 255));
			_TabButtons[n]->SetStateText(UIRadioButton::UI_NORMAL, FONT_NORMAL, GlobalText[tabsText[n]], MU::MakeARGB(181, 181, 181, 255));
			_TabButtons[n]->SetStateText(UIRadioButton::UI_OVER, FONT_NORMAL, GlobalText[tabsText[n]], MU::MakeARGB(181, 181, 181, 255));
			_TabButtons[n]->SetStateText(UIRadioButton::UI_PRESSED, FONT_NORMAL, GlobalText[tabsText[n]], MU::MakeARGB(255, 255, 255, 255));
			_TabButtons[n]->SetStateText(UIRadioButton::UI_PRESSED_OVER, FONT_NORMAL, GlobalText[tabsText[n]], MU::MakeARGB(255, 255, 255, 255));
		}

		_LineSprite = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::MYQUEST_LINE], SCREEN_WIDTH, SCREEN_HEIGHT, QUESTWINDOW_POSX + 0.0f, QUESTWINDOW_POSY + 160.0f, 188.0f, 21.0f);
		_LineSprite->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

		// Quest Tab
		_QuestStartButton = new UIButton(GAMEUI::MESSAGE_QUESTWINDOW_QUEST_START_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, QUESTTAB_START_POSX, QUESTTAB_START_POSY, QUESTTAB_START_WIDTH, QUESTTAB_START_HEIGHT);
		_QuestStartButton->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_QuestStartButton->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MYQUEST_BTN_OPEN], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(100, 100, 100, 255)));
		_QuestStartButton->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MYQUEST_BTN_OPEN], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_QuestStartButton->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MYQUEST_BTN_OPEN], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_QuestStartButton->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MYQUEST_BTN_OPEN], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_QuestStartButton->SetTooltip(FONT_NORMAL, GlobalText[2822], MU::MakeARGB(255, 255, 255, 255), MU::MakeARGB(0, 0, 0, 180), true);
		_QuestStartButton->ChangeState(UIButton::UI_DISABLED);

		_QuestGiveUpButton = new UIButton(GAMEUI::MESSAGE_QUESTWINDOW_QUEST_GIVEUP_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, QUESTTAB_GIVEUP_POSX, QUESTTAB_GIVEUP_POSY, QUESTTAB_GIVEUP_WIDTH, QUESTTAB_GIVEUP_HEIGHT);
		_QuestGiveUpButton->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_QuestGiveUpButton->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MYQUEST_BTN_GIVEUP], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(100, 100, 100, 255)));
		_QuestGiveUpButton->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MYQUEST_BTN_GIVEUP], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_QuestGiveUpButton->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MYQUEST_BTN_GIVEUP], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_QuestGiveUpButton->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::MYQUEST_BTN_GIVEUP], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_QuestGiveUpButton->SetTooltip(FONT_NORMAL, GlobalText[2823], MU::MakeARGB(255, 255, 255, 255), MU::MakeARGB(0, 0, 0, 180), true);
		_QuestGiveUpButton->ChangeState(UIButton::UI_DISABLED);

		// Change Class Tab
		_ChangeQuestPreviewBox = new UIMultiLineEditBox(-1, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, QUESTWINDOW_POSX + 20.0f, QUESTWINDOW_POSY + 76.0f, QUESTWINDOW_WIDTH - 40.0f, 96.0f);
		_ChangeQuestPreviewBox->MarkStyle(UIMultiLineEditBox::UES_CENTER);
		_ChangeQuestPreviewBox->SetWrapMode(true);
		_ChangeQuestPreviewBox->SetReadOnly(true);
		_ChangeQuestPreviewBox->SetTextColor(255, 255, 255, 255);
		_ChangeQuestPreviewBox->SetBackColor(0, 0, 0, 0);

		_ChangeQuestStatePreviewBox = new UIMultiLineEditBox(-1, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, QUESTWINDOW_POSX + 23.0f, QUESTWINDOW_POSY + 283.0f, 64.0f, 96.0f);
		_ChangeQuestStatePreviewBox->SetWrapMode(true);
		_ChangeQuestStatePreviewBox->SetReadOnly(true);
		_ChangeQuestStatePreviewBox->SetFont(FONT_BOLD);
		_ChangeQuestStatePreviewBox->SetTextColor(255, 255, 0, 255);
		_ChangeQuestStatePreviewBox->SetBackColor(0, 0, 0, 0);

		SetCurrentTab(TAB_QUEST);
	}
}

UIQuestWindow::~UIQuestWindow()
{

}

void UIQuestWindow::UpdateWindow(mu_int32 screenWidth)
{
	mu_float x = (mu_float)screenWidth;
	mu_float y = 0.0f;

#define SETPOSITION_HELPER(var) var->SetPosition(var->_ViewX - oldX + x, var->_ViewY - oldY + y)

	mu_float oldX = _ViewX, oldY = _ViewY;

	SETPOSITION_HELPER(this);

	SETPOSITION_HELPER(_BackFrame);
	SETPOSITION_HELPER(_TopFrame);
	SETPOSITION_HELPER(_BottomFrame);
	SETPOSITION_HELPER(_LeftFrame);
	SETPOSITION_HELPER(_RightFrame);

	SETPOSITION_HELPER(_XButton);
	SETPOSITION_HELPER(_CloseButton);

	for (mu_uint32 n = 0; n < TAB_MAX; ++n)
	{
		SETPOSITION_HELPER(_TabButtons[n]);
	}

	SETPOSITION_HELPER(_LineSprite);

	// Quest Tab
	SETPOSITION_HELPER(_QuestStartButton);
	SETPOSITION_HELPER(_QuestGiveUpButton);

	// Change Class Tab
	SETPOSITION_HELPER(_ChangeQuestPreviewBox);
	SETPOSITION_HELPER(_ChangeQuestStatePreviewBox);

#undef SETPOSITION_HELPER
}

void UIQuestWindow::Configure()
{
	g_QuestManager.ShowQuestPreviewWindow(-1);

	const mu_text *dialogText = _T("");
	mu_int32 currentDialog = g_QuestManager.GetCurrentPage();
	const MUDialogData *dialog = g_DialogManager.GetDialog(currentDialog);
	if (dialog != nullptr)
	{
		dialogText = dialog->Text.c_str();
	}

	_ChangeQuestPreviewBox->SetText(dialogText);

	switch (g_QuestManager.GetCurrentQuestState())
	{
	case QUEST_NONE:
	case QUEST_NO:
	case QUEST_ERROR:
		{
			_ChangeQuestStatePreviewBox->SetText(GlobalText[930]);
		}
		break;

	case QUEST_ING:
		{
			_ChangeQuestStatePreviewBox->SetText(GlobalText[931]);
		}
		break;

	case QUEST_END:
		{
			_ChangeQuestStatePreviewBox->SetText(GlobalText[932]);
		}
		break;
	}
}

void UIQuestWindow::Update(mu_uint64 CurrentTime)
{
	_ChangeQuestPreviewBox->Update(CurrentTime);
	_ChangeQuestStatePreviewBox->Update(CurrentTime);
}

void UIQuestWindow::Draw()
{
	DrawFrames();
	DrawButtons();
	DrawTexts();
}

void UIQuestWindow::PostDraw()
{
	PostDrawButtons();
}

void UIQuestWindow::DrawFrames()
{
	_BackFrame->Draw();
	_TopFrame->Draw();
	_BottomFrame->Draw();
	_LeftFrame->Draw();
	_RightFrame->Draw();

	_TabBack->Draw();

	_LineSprite->Draw();
}

void UIQuestWindow::DrawButtons()
{
	for (mu_uint32 n = 0; n < TAB_MAX; ++n)
	{
		if (n == _CurrentTab) continue;

		_TabButtons[n]->Draw();
	}

	_TabButtons[_CurrentTab]->Draw();

	switch (_CurrentTab)
	{
	case TAB_QUEST:
		{
			_QuestStartButton->Draw();
			_QuestGiveUpButton->Draw();
		}
		break;
	}

	_CloseButton->Draw();
}

void UIQuestWindow::PostDrawButtons()
{
	for (mu_uint32 n = 0; n < TAB_MAX; ++n)
	{
		if (n == _CurrentTab) continue;

		_TabButtons[n]->PostDraw();
	}

	_TabButtons[_CurrentTab]->PostDraw();

	switch (_CurrentTab)
	{
	case TAB_QUEST:
		{
			_QuestStartButton->PostDraw();
			_QuestGiveUpButton->PostDraw();
		}
		break;
	}

	_CloseButton->PostDraw();
}

void UIQuestWindow::DrawTexts()
{
	mu_float screenX = MU_RESOURCE->_640x480Rate[0];
	mu_float screenY = MU_RESOURCE->_640x480Rate[1];
	mu_uint32 textColor = MU::MakeARGB(255, 255, 255, 255);
	mu_uint32 bgColor = MU::MakeARGB(0, 0, 0, 0);

	FONT_BOLD->DrawStringC2D(TEXTINDEX::QUESTWINDOW_TITLE,
		MU_TO_WINDOW(_ViewX + QUESTWINDOW_WIDTH * 0.5f, screenX),
		MU_TO_WINDOW(_ViewY + 15.0f, screenY),
		1.0f,
		ETA_CENTER,
		ETA_CENTER,
		0.0f,
		textColor,
		bgColor,
		GlobalText[1140]);

	switch (_CurrentTab)
	{
	case TAB_CHANGECLASS:
		{
			FONT_BOLD->DrawStringC2D(TEXTINDEX::QUESTWINDOW_CHANGECLASS_TITLE,
				MU_TO_WINDOW(_ViewX + QUESTWINDOW_WIDTH * 0.5f, screenX),
				MU_TO_WINDOW(_ViewY + 58.0f, screenY),
				1.0f,
				ETA_CENTER,
				ETA_BOTTOM,
				0.0f,
				MU::MakeARGB(36, 232, 252, 255),
				bgColor,
				g_QuestManager.GetQuestTitleWindow());

			_ChangeQuestPreviewBox->Draw();
			_ChangeQuestStatePreviewBox->Draw();
		}
		break;

	case TAB_CASTLETEMPLE:
		{
			// Castle Info
			FONT_BOLD->DrawStringC2D(TEXTINDEX::QUESTWINDOW_CASTLEINFO1,
				MU_TO_WINDOW(_ViewX + QUESTWINDOW_WIDTH * 0.5f, screenX),
				MU_TO_WINDOW(_ViewY + 105.0f, screenY),
				1.0f,
				ETA_CENTER,
				ETA_BOTTOM,
				0.0f,
				MU::MakeARGB(255, 255, 0, 255),
				bgColor,
				GlobalText[56]);

			mu_text textBuffer[512];

			mu_sprintf_s(textBuffer, mu_countof(textBuffer), GlobalText[868], g_QuestManager.GetEventCount(TYPE_BLOODCASTLE));
			FONT_NORMAL->DrawStringC2D(TEXTINDEX::QUESTWINDOW_CASTLEINFO2,
				MU_TO_WINDOW(_ViewX + QUESTWINDOW_WIDTH * 0.5f, screenX),
				MU_TO_WINDOW(_ViewY + 125.0f, screenY),
				1.0f,
				ETA_CENTER,
				ETA_BOTTOM,
				0.0f,
				textColor,
				bgColor,
				textBuffer);

			mu_sprintf_s(textBuffer, mu_countof(textBuffer), GlobalText[829], 6);
			FONT_NORMAL->DrawStringC2D(TEXTINDEX::QUESTWINDOW_CASTLEINFO3,
				MU_TO_WINDOW(_ViewX + QUESTWINDOW_WIDTH * 0.5f, screenX),
				MU_TO_WINDOW(_ViewY + 145.0f, screenY),
				1.0f,
				ETA_CENTER,
				ETA_BOTTOM,
				0.0f,
				textColor,
				bgColor,
				textBuffer);

			// Temple Info
			FONT_BOLD->DrawStringC2D(TEXTINDEX::QUESTWINDOW_TEMPLEINFO1,
				MU_TO_WINDOW(_ViewX + QUESTWINDOW_WIDTH * 0.5f, screenX),
				MU_TO_WINDOW(_ViewY + 285.0f, screenY),
				1.0f,
				ETA_CENTER,
				ETA_BOTTOM,
				0.0f,
				MU::MakeARGB(255, 255, 0, 255),
				bgColor,
				GlobalText[2369]);

			mu_sprintf_s(textBuffer, mu_countof(textBuffer), GlobalText[868], g_QuestManager.GetEventCount(TYPE_CURSEDTEMPLE));
			FONT_NORMAL->DrawStringC2D(TEXTINDEX::QUESTWINDOW_TEMPLEINFO2,
				MU_TO_WINDOW(_ViewX + QUESTWINDOW_WIDTH * 0.5f, screenX),
				MU_TO_WINDOW(_ViewY + 305.0f, screenY),
				1.0f,
				ETA_CENTER,
				ETA_BOTTOM,
				0.0f,
				textColor,
				bgColor,
				textBuffer);

			mu_sprintf_s(textBuffer, mu_countof(textBuffer), GlobalText[829], 6);
			FONT_NORMAL->DrawStringC2D(TEXTINDEX::QUESTWINDOW_TEMPLEINFO3,
				MU_TO_WINDOW(_ViewX + QUESTWINDOW_WIDTH * 0.5f, screenX),
				MU_TO_WINDOW(_ViewY + 325.0f, screenY),
				1.0f,
				ETA_CENTER,
				ETA_BOTTOM,
				0.0f,
				textColor,
				bgColor,
				textBuffer);
		}
		break;
	}
}

void UIQuestWindow::SetCurrentTab(mu_uint32 currentTab)
{
	for (mu_uint32 n = 0; n < TAB_MAX; ++n)
	{
		if (n == currentTab) continue;

		_TabButtons[n]->ChangeState(UIRadioButton::UI_NORMAL);
	}

	_CurrentTab = currentTab;
	_TabButtons[currentTab]->ChangeState(UIRadioButton::UI_PRESSED);

	switch (_CurrentTab)
	{
	case TAB_QUEST:
		{
			_LineSprite->SetPosition(_ViewX + 0.0f, _ViewY + 160.0f);
		}
		break;

	case TAB_CHANGECLASS:
		{
			_LineSprite->SetPosition(_ViewX + 0.0f, _ViewY + 182.0f);
		}
		break;

	case TAB_CASTLETEMPLE:
		{
			_LineSprite->SetPosition(_ViewX + 0.0f, _ViewY + 210.0f);
		}
		break;
	}
}

UIBase *UIQuestWindow::GetFocus(mu_int32 mouseX, mu_int32 mouseY)
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

		object = _TabButtons[_CurrentTab]->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}

		for (mu_uint32 n = 0; n < TAB_MAX; ++n)
		{
			if (n == _CurrentTab) continue;

			object = _TabButtons[n]->GetFocus(mouseX, mouseY);

			if (object != nullptr)
			{
				return object;
			}
		}

		switch (_CurrentTab)
		{
		case TAB_QUEST:
			{
				object = _QuestStartButton->GetFocus(mouseX, mouseY);

				if (object != nullptr)
				{
					return object;
				}

				object = _QuestGiveUpButton->GetFocus(mouseX, mouseY);

				if (object != nullptr)
				{
					return object;
				}
			}
			break;
		}
	}

	return focus;
}