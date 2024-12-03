#include "stdafx.h"
#include "mu_uinpcquestwindow.h"

#define SCREEN_WIDTH 640.0f
#define SCREEN_HEIGHT 480.0f

#define BACKTEXTURE_WIDTH 225.0f
#define BACKTEXTURE_HEIGHT 512.0f

#define BACK_POSX NPCQUESTWINDOW_POSX
#define BACK_POSY NPCQUESTWINDOW_POSY
#define BACK_WIDTH NPCQUESTWINDOW_WIDTH
#define BACK_HEIGHT NPCQUESTWINDOW_HEIGHT
#define BACK_UVBEGIN EVector2(0.0f, 0.0f)
#define BACK_UVEND EVector2(BACK_WIDTH / BACKTEXTURE_WIDTH, BACK_HEIGHT / BACKTEXTURE_HEIGHT)

#define TOP_WIDTH NPCQUESTWINDOW_WIDTH
#define TOP_HEIGHT 64.0f
#define TOP_POSX NPCQUESTWINDOW_POSX
#define TOP_POSY NPCQUESTWINDOW_POSY

#define BOTTOM_WIDTH NPCQUESTWINDOW_WIDTH
#define BOTTOM_HEIGHT 45.0f
#define BOTTOM_POSX NPCQUESTWINDOW_POSX
#define BOTTOM_POSY (NPCQUESTWINDOW_POSY + NPCQUESTWINDOW_HEIGHT - BOTTOM_HEIGHT)

#define LEFT_WIDTH 21.0f
#define LEFT_HEIGHT (BOTTOM_POSY - (TOP_POSY + TOP_HEIGHT))
#define LEFT_POSX NPCQUESTWINDOW_POSX
#define LEFT_POSY (TOP_POSY + TOP_HEIGHT)

#define RIGHT_WIDTH 21.0f
#define RIGHT_HEIGHT (BOTTOM_POSY - (TOP_POSY + TOP_HEIGHT))
#define RIGHT_POSX (NPCQUESTWINDOW_POSX + NPCQUESTWINDOW_WIDTH - RIGHT_WIDTH)
#define RIGHT_POSY (TOP_POSY + TOP_HEIGHT)

#define XBUTTON_WIDTH 16.0f
#define XBUTTON_HEIGHT 16.0f
#define XBUTTON_POSX (TOP_POSX + TOP_WIDTH - 7.0f - XBUTTON_WIDTH)
#define XBUTTON_POSY (TOP_POSY + 7.0f)

#define CLOSEBUTTON_POSX (NPCQUESTWINDOW_POSX + 13.0f)
#define CLOSEBUTTON_POSY (NPCQUESTWINDOW_POSY + 392.0f)
#define CLOSEBUTTON_WIDTH (36.0f)
#define CLOSEBUTTON_HEIGHT (29.0f)

#define COMPLETEBUTTON_WIDTH (108.0f)
#define COMPLETEBUTTON_HEIGHT (29.0f)
#define COMPLETEBUTTON_POSX (NPCQUESTWINDOW_POSX + 41.0f)
#define COMPLETEBUTTON_POSY (NPCQUESTWINDOW_POSY + 355.0f)

#define ANSWERBUTTON_WIDTH (182.0f)
#define ANSWERBUTTON_HEIGHT (15.0f)
#define ANSWERBUTTON_POSX (NPCQUESTWINDOW_POSX + 4.0f)
#define ANSWERBUTTON_POSY(n) (NPCQUESTWINDOW_POSY + 245.0f + (ANSWERBUTTON_HEIGHT + 3.0f) * n)

#define ITEM_WIDTH 20.0f
#define ITEM_HEIGHT 27.0f
#define ITEM_POSX (_ViewX + 30.0f - ITEM_WIDTH * 0.5f)
#define ITEM_POSY (_ViewY + 165.0f - ITEM_HEIGHT * 0.5f)
#define TEXT_POSX (ITEM_POSX)
#define TEXT_POSY (ITEM_POSY)

UINpcQuestWindow::UINpcQuestWindow(UISceneManager *manager) : UIBase()
{
	if (manager != nullptr)
	{
		manager->AddGFxObject(this);
	}

	_SceneManager = manager;

	UIBase::SetAspectRatio(SCREEN_WIDTH, SCREEN_HEIGHT);
	UIBase::SetPosition(NPCQUESTWINDOW_POSX, NPCQUESTWINDOW_POSY);
	UIBase::SetDimensions(NPCQUESTWINDOW_WIDTH, NPCQUESTWINDOW_HEIGHT);

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

		_XButton = new UIButton(GAMEUI::MESSAGE_NPCQUESTWINDOW_CLOSE_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, XBUTTON_POSX, XBUTTON_POSY, XBUTTON_WIDTH, XBUTTON_HEIGHT);

		_CloseButton = new UIButton(GAMEUI::MESSAGE_NPCQUESTWINDOW_CLOSE_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, CLOSEBUTTON_POSX, CLOSEBUTTON_POSY, CLOSEBUTTON_WIDTH, CLOSEBUTTON_HEIGHT);
		_CloseButton->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_CloseButton->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EXIT_BUTTON], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(100, 100, 100, 255)));
		_CloseButton->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EXIT_BUTTON], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_CloseButton->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EXIT_BUTTON], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_CloseButton->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EXIT_BUTTON], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_CloseButton->SetTooltip(FONT_NORMAL, GlobalText[1002], MU::MakeARGB(255, 255, 255, 255), MU::MakeARGB(0, 0, 0, 180), true);

		_CompleteButton = new UIButton(GAMEUI::MESSAGE_NPCQUESTWINDOW_COMPLETE_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, COMPLETEBUTTON_POSX, COMPLETEBUTTON_POSY, COMPLETEBUTTON_WIDTH, COMPLETEBUTTON_HEIGHT);
		_CompleteButton->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_CompleteButton->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTY_BUTTON], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(100, 100, 100, 255)));
		_CompleteButton->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTY_BUTTON], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(255, 255, 255, 255)));
		_CompleteButton->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTY_BUTTON], TUV_THREE_OVER_BEGIN, TUV_THREE_OVER_END, MU::MakeARGB(255, 255, 255, 255)));
		_CompleteButton->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTY_BUTTON], TUV_THREE_PRESSED_BEGIN, TUV_THREE_PRESSED_END, MU::MakeARGB(255, 255, 255, 255)));
		_CompleteButton->SetStateText(UIButton::UI_DISABLED, FONT_NORMAL, GlobalText[699], MU::MakeARGB(170, 170, 170, 255));
		_CompleteButton->SetStateText(UIButton::UI_NORMAL, FONT_NORMAL, GlobalText[699], MU::MakeARGB(255, 230, 210, 255));
		_CompleteButton->SetStateText(UIButton::UI_OVER, FONT_NORMAL, GlobalText[699], MU::MakeARGB(255, 230, 210, 255));
		_CompleteButton->SetStateText(UIButton::UI_PRESSED, FONT_NORMAL, GlobalText[699], MU::MakeARGB(255, 230, 210, 255));

		for (mu_uint32 n = 0; n < mu_countof(_AnswersButton); ++n)
		{
			_AnswersButton[n] = new UIButton(GAMEUI::MESSAGE_NPCQUESTWINDOW_ANSWER_BEGIN + n, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, ANSWERBUTTON_POSX, ANSWERBUTTON_POSY(n), ANSWERBUTTON_WIDTH, ANSWERBUTTON_HEIGHT);
			_AnswersButton[n]->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
			_AnswersButton[n]->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(USE_COLOR_STATE, EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(0, 0, 0, 0)));
			_AnswersButton[n]->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(USE_COLOR_STATE, EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(20, 20, 20, 50)));
			_AnswersButton[n]->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(USE_COLOR_STATE, EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(20, 20, 20, 150)));
			_AnswersButton[n]->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(USE_COLOR_STATE, EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(20, 20, 20, 25)));
			_AnswersButton[n]->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		}

		_LineSprite1 = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::MYQUEST_LINE], SCREEN_WIDTH, SCREEN_HEIGHT, NPCQUESTWINDOW_POSX + 0.0f, NPCQUESTWINDOW_POSY + 220.0f, 188.0f, 21.0f);
		_LineSprite1->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

		_LineSprite2 = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::MYQUEST_LINE], SCREEN_WIDTH, SCREEN_HEIGHT, NPCQUESTWINDOW_POSX + 0.0f, NPCQUESTWINDOW_POSY + 160.0f, 188.0f, 21.0f);
		_LineSprite2->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

		_MoneyIcon = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::ICON_MONEY2], SCREEN_WIDTH, SCREEN_HEIGHT, NPCQUESTWINDOW_POSX + 10.0f, NPCQUESTWINDOW_POSY + 361.0f, 170.0f, 24.0f);
		_MoneyIcon->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

		// Change Class Tab
		_ChangeQuestPreviewBox = new UIMultiLineEditBox(-1, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, NPCQUESTWINDOW_POSX + 20.0f, NPCQUESTWINDOW_POSY + 66.0f, NPCQUESTWINDOW_WIDTH - 40.0f, 96.0f);
		_ChangeQuestPreviewBox->MarkStyle(UIMultiLineEditBox::UES_CENTER);
		_ChangeQuestPreviewBox->SetWrapMode(true);
		_ChangeQuestPreviewBox->SetReadOnly(true);
		_ChangeQuestPreviewBox->SetTextColor(255, 255, 255, 255);
		_ChangeQuestPreviewBox->SetBackColor(0, 0, 0, 0);

		_ChangeQuestStatePreviewBox = new UIMultiLineEditBox(-1, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, NPCQUESTWINDOW_POSX + 23.0f, NPCQUESTWINDOW_POSY + 66.0f, 64.0f, 96.0f);
		_ChangeQuestStatePreviewBox->SetWrapMode(true);
		_ChangeQuestStatePreviewBox->SetReadOnly(true);
		_ChangeQuestStatePreviewBox->SetFont(FONT_BOLD);
		_ChangeQuestStatePreviewBox->SetTextColor(255, 255, 0, 255);
		_ChangeQuestStatePreviewBox->SetBackColor(0, 0, 0, 0);

		SetCurrentTab(TAB_QUEST);
	}
}

UINpcQuestWindow::~UINpcQuestWindow()
{

}

void UINpcQuestWindow::UpdateWindow(mu_int32 screenWidth)
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

	SETPOSITION_HELPER(_LineSprite1);
	SETPOSITION_HELPER(_LineSprite2);

#undef SETPOSITION_HELPER
}

void UINpcQuestWindow::Configure()
{
	g_QuestManager.ShowQuestNpcWindow(-1);

	const MUQuestData *questData = g_QuestManager.GetQuest(g_QuestManager.GetCurrentQuestIndex());
	mu_int32 heroClass = GetBaseClass(Hero->Class);

	mu_boolean isCompleted = true;
	for (auto it = questData->QuestActs.begin(); it != questData->QuestActs.end(); ++it)
	{
		const MUQuestClassAct &act = *it;
		if (act.RequestClass[heroClass] == 0)
		{
			continue;
		}

		switch (act.QuestType)
		{
		case QUEST_ITEM:
			{
				mu_int32 itemType = act.ItemType * MAX_ITEM_INDEX + act.ItemSubType;

				if (g_QuestManager.CheckQuestsItems(itemType, act.ItemNum, act.ItemLevel) > 0)
				{
					isCompleted = false;
					break;
				}
			}
			break;

		case QUEST_MONSTER:
			{
				mu_int32 curKillCount = g_QuestManager.GetKillMobCount(act.ItemType);

				if (curKillCount < (mu_int32)act.ItemNum)
				{
					isCompleted = false;
					break;
				}
			}
			break;
		}
	}

	if (isCompleted == true)
	{
		_CompleteButton->ChangeState(UIButton::UI_NORMAL);
	}
	else
	{
		_CompleteButton->ChangeState(UIButton::UI_DISABLED);
	}

	ConfigureDialog(g_QuestManager.GetCurrentPage());
}

void UINpcQuestWindow::ConfigureDialog(mu_int32 dialogIndex)
{
	const mu_text *dialogText = _T("");
	const MUDialogData *dialog = g_DialogManager.GetDialog(dialogIndex);
	if (dialog != nullptr)
	{
		dialogText = dialog->Text.c_str();
	}

	_CurrentDialog = dialog;

	_ChangeQuestPreviewBox->SetText(dialogText);

	_LineSprite2->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);

	_CurrentQuestState = g_QuestManager.GetQuestState2(g_QuestManager.GetCurrentQuestIndex());
	switch (_CurrentQuestState)
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
			_LineSprite2->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
			_LineSprite2->SetPosition(_ViewX + 0.0f, _ViewY + 325.0f);
			_ChangeQuestStatePreviewBox->SetText(GlobalText[931]);
		}
		break;

	case QUEST_END:
		{
			_ChangeQuestStatePreviewBox->SetText(GlobalText[932]);
		}
		break;
	}

	for (mu_uint32 n = 0; n < mu_countof(_AnswersButton); ++n)
	{
		_AnswersButton[n]->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
	}

	mu_text textBuffer[1024];

	if (dialog->Answers.size() == 0)
	{
		mu_sprintf_s(textBuffer, mu_countof(textBuffer), _T("%u) ") SPF_STRING _T(""), 1, GlobalText[609]);
		_AnswersButton[0]->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
		_AnswersButton[0]->SetStateText(UIButton::UI_DISABLED, FONT_NORMAL, textBuffer, MU::MakeARGB(150, 150, 150, 200), MU::MakeARGB(0, 0, 0, 0), 5.0f, 0.0f, ETA_LEFT);
		_AnswersButton[0]->SetStateText(UIButton::UI_NORMAL, FONT_NORMAL, textBuffer, MU::MakeARGB(223, 191, 103, 255), MU::MakeARGB(0, 0, 0, 0), 5.0f, 0.0f, ETA_LEFT);
		_AnswersButton[0]->SetStateText(UIButton::UI_OVER, FONT_NORMAL, textBuffer, MU::MakeARGB(255, 0, 0, 255), MU::MakeARGB(0, 0, 0, 0), 5.0f, 0.0f, ETA_LEFT);
		_AnswersButton[0]->SetStateText(UIButton::UI_PRESSED, FONT_NORMAL, textBuffer, MU::MakeARGB(223, 191, 103, 255), MU::MakeARGB(0, 0, 0, 0), 5.0f, 0.0f, ETA_LEFT);

	}
	else
	{
		for (mu_uint32 n = 0; n < dialog->Answers.size(); ++n)
		{
			const MUDialogAnswerData &answer = dialog->Answers[n];

			mu_sprintf_s(textBuffer, mu_countof(textBuffer), _T("%u) ") SPF_STRING _T(""), n + 1, answer.Answer.c_str());
			_AnswersButton[n]->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
			_AnswersButton[n]->SetStateText(UIButton::UI_DISABLED, FONT_NORMAL, textBuffer, MU::MakeARGB(150, 150, 150, 200), MU::MakeARGB(0, 0, 0, 0), 5.0f, 0.0f, ETA_LEFT);
			_AnswersButton[n]->SetStateText(UIButton::UI_NORMAL, FONT_NORMAL, textBuffer, MU::MakeARGB(223, 191, 103, 255), MU::MakeARGB(0, 0, 0, 0), 5.0f, 0.0f, ETA_LEFT);
			_AnswersButton[n]->SetStateText(UIButton::UI_OVER, FONT_NORMAL, textBuffer, MU::MakeARGB(255, 0, 0, 255), MU::MakeARGB(0, 0, 0, 0), 5.0f, 0.0f, ETA_LEFT);
			_AnswersButton[n]->SetStateText(UIButton::UI_PRESSED, FONT_NORMAL, textBuffer, MU::MakeARGB(223, 191, 103, 255), MU::MakeARGB(0, 0, 0, 0), 5.0f, 0.0f, ETA_LEFT);
		}
	}
}

void UINpcQuestWindow::Update(mu_uint64 CurrentTime)
{
	_ChangeQuestPreviewBox->Update(CurrentTime);
	_ChangeQuestStatePreviewBox->Update(CurrentTime);
}

void UINpcQuestWindow::Draw()
{
	DrawFrames();
	DrawButtons();
	DrawTexts();
}

void UINpcQuestWindow::Draw3D()
{
	_SlotCount = 0;

	if (g_QuestManager.GetQuestState2(g_QuestManager.GetCurrentQuestIndex()) != QUEST_ING)
	{
		return;
	}

	const MUQuestData *questData = g_QuestManager.GetQuest(g_QuestManager.GetCurrentQuestIndex());
	mu_int32 heroClass = GetBaseClass(Hero->Class);

	if (questData->QuestActs.empty() == false)
	{
		mu_uint32 itemsCount = 0;
		for (auto it = questData->QuestActs.begin(); it != questData->QuestActs.end(); ++it)
		{
			const MUQuestClassAct &act = *it;
			if (act.RequestClass[heroClass] == 0)
			{
				continue;
			}

			if (act.QuestType == QUEST_ITEM)
			{
				++itemsCount;
			}
		}

		if (itemsCount > 0)
		{
			MUThreadInfo &ThreadInfo = g_ThreadInfo[0];
			++ThreadInfo.GUIRenderIndex;
			ThreadInfo.RenderLight = MU::Float4ToARGB(1.0f, 1.0f, 1.0f, 1.0f);

			_SlotIndex = g_GUIRenderSlotCount;

			mu_float aspectWidth = SCREEN_WIDTH / 640.0f;
			mu_float aspectHeight = SCREEN_HEIGHT / 480.0f;

			itemsCount = 0;
			for (auto it = questData->QuestActs.begin(); it != questData->QuestActs.end(); ++it)
			{
				const MUQuestClassAct &act = *it;
				if (act.RequestClass[heroClass] == 0)
				{
					continue;
				}

				if (act.QuestType == QUEST_ITEM)
				{
					mu_int32 itemType = act.ItemType * MAX_ITEM_INDEX + act.ItemSubType;

					ITEM_ATTRIBUTE* itemAttr = &ItemAttribute[itemType];
					mu_float width = itemAttr->Width * INVENTORY_SQUARE_WIDTH;
					mu_float height = itemAttr->Height * INVENTORY_SQUARE_HEIGHT;
					mu_float x = MU_TO_VIEWPORT(ITEM_POSX, aspectWidth) - width * 0.5f;
					mu_float y = MU_TO_VIEWPORT(ITEM_POSY + (itemsCount * (ITEM_HEIGHT + 5.0f)), aspectHeight) - height * 0.5f;
					++itemsCount;

					RenderItem3D(x, y, ITEM_WIDTH, ITEM_HEIGHT, itemType, act.ItemLevel, 0, 0, false);
				}
			}

			_SlotCount = g_GUIRenderSlotCount - _SlotIndex;
		}
	}
}

void UINpcQuestWindow::PostDraw()
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[0];

	if (_SlotCount > 0)
	{
		ThreadInfo.EnableScissor = false;
		g_Tasking.RenderGUIModelsAtRuntime(_SlotIndex, _SlotCount);
	}

	PostDrawButtons();
}

void UINpcQuestWindow::DrawFrames()
{
	_BackFrame->Draw();
	_TopFrame->Draw();
	_BottomFrame->Draw();
	_LeftFrame->Draw();
	_RightFrame->Draw();

	_LineSprite1->Draw();
	_LineSprite2->Draw();

	switch (_CurrentQuestState)
	{
	case QUEST_NO:
		{
			_MoneyIcon->Draw();
		}
		break;
	}
}

void UINpcQuestWindow::DrawButtons()
{
	switch (_CurrentQuestState)
	{
	case QUEST_ING:
		{
			_CompleteButton->Draw();
		}
		break;
	}

	for (mu_uint32 n = 0; n < mu_countof(_AnswersButton); ++n)
	{
		_AnswersButton[n]->Draw();
	}

	_CloseButton->Draw();
}

void UINpcQuestWindow::PostDrawButtons()
{
	switch (_CurrentQuestState)
	{
	case QUEST_ING:
		{
			_CompleteButton->PostDraw();
		}
		break;
	}

	_CloseButton->PostDraw();
}

void UINpcQuestWindow::DrawTexts()
{
	mu_float screenX = MU_RESOURCE->_640x480Rate[0];
	mu_float screenY = MU_RESOURCE->_640x480Rate[1];
	mu_uint32 bgColor = MU::MakeARGB(0, 0, 0, 0);

	mu_uint32 nameColor = MU::MakeARGB(150, 255, 240, 255);

	if ((Hero->Class == GAME::CLASS_DARK ||
		Hero->Class == GAME::CLASS_DARK_LORD) &&
		g_QuestManager.GetCheckNpc() == true)
	{
		FONT_BOLD->DrawStringC2D(TEXTINDEX::NPCQUESTWINDOW_TITLE,
			MU_TO_WINDOW(_ViewX + QUESTWINDOW_WIDTH * 0.5f, screenX),
			MU_TO_WINDOW(_ViewY + 15.0f, screenY),
			1.0f,
			ETA_CENTER,
			ETA_CENTER,
			0.0f,
			nameColor,
			bgColor,
			g_QuestManager.GetNpcName(2));
	}
	else
	{
		FONT_BOLD->DrawStringC2D(TEXTINDEX::NPCQUESTWINDOW_TITLE,
			MU_TO_WINDOW(_ViewX + QUESTWINDOW_WIDTH * 0.5f, screenX),
			MU_TO_WINDOW(_ViewY + 15.0f, screenY),
			1.0f,
			ETA_CENTER,
			ETA_CENTER,
			0.0f,
			nameColor,
			bgColor,
			g_QuestManager.GetNpcName(g_QuestManager.GetCurrentQuestIndex()));
	}

	if ((Hero->Class != GAME::CLASS_DARK &&
		Hero->Class != GAME::CLASS_DARK_LORD) ||
		g_QuestManager.GetCheckNpc() == false)
	{
		FONT_BOLD->DrawStringC2D(TEXTINDEX::NPCQUESTWINDOW_NAME,
			MU_TO_WINDOW(_ViewX + QUESTWINDOW_WIDTH * 0.5f, screenX),
			MU_TO_WINDOW(_ViewY + 45.0f, screenY),
			1.0f,
			ETA_CENTER,
			ETA_BOTTOM,
			0.0f,
			MU::MakeARGB(200, 220, 255, 255),
			bgColor,
			g_QuestManager.GetQuestTitle());
	}

	switch (_CurrentQuestState)
	{
	case QUEST_NO:
		{
			mu_text textBuffer[512];

			FONT_BOLD->DrawStringC2D(TEXTINDEX::NPCQUESTWINDOW_COST,
				MU_TO_WINDOW(_ViewX + 20.0f, screenX),
				MU_TO_WINDOW(_ViewY + 368.0f, screenY),
				1.0f,
				ETA_LEFT,
				ETA_BOTTOM,
				0.0f,
				MU::MakeARGB(255, 220, 150, 255),
				bgColor,
				GlobalText[198]);

			mu_uint32 goldColor = getGoldColor(g_QuestManager.GetNeedZen());
			ConvertGold(g_QuestManager.GetNeedZen(), textBuffer, mu_countof(textBuffer));
			FONT_BOLD->DrawStringC2D(TEXTINDEX::NPCQUESTWINDOW_REQUIREDZEN,
				MU_TO_WINDOW(_ViewX + 170.0f, screenX),
				MU_TO_WINDOW(_ViewY + 368.0f, screenY),
				1.0f,
				ETA_RIGHT,
				ETA_BOTTOM,
				0.0f,
				goldColor,
				bgColor,
				textBuffer);
		}
		break;

	case QUEST_ING:
		{
			const MUQuestData *questData = g_QuestManager.GetQuest(g_QuestManager.GetCurrentQuestIndex());
			mu_int32 heroClass = GetBaseClass(Hero->Class);

			mu_uint32 itemColor = MU::MakeARGB(223, 191, 103, 255);
			mu_uint32 itemNotColor = MU::MakeARGB(255, 30, 30, 255);
			mu_uint32 itemBgColor = MU::MakeARGB(20, 20, 20, 150);

			mu_uint32 itemsCount = 0;
			for (auto it = questData->QuestActs.begin(); it != questData->QuestActs.end(); ++it)
			{
				const MUQuestClassAct &act = *it;
				if (act.RequestClass[heroClass] == 0)
				{
					continue;
				}

				switch (act.QuestType)
				{
				case QUEST_ITEM:
					{
						mu_int32 itemType = act.ItemType * MAX_ITEM_INDEX + act.ItemSubType;

						mu_float x = MU_TO_WINDOW(ITEM_POSX + 80.0f, screenX);
						mu_float y = MU_TO_WINDOW(ITEM_POSY + (((mu_float)itemsCount) * (ITEM_HEIGHT + 5.0f)) + 8.5f, screenY);
						++itemsCount;

						mu_uint32 curColor;
						if (g_QuestManager.CheckQuestsItems(itemType, act.ItemNum, act.ItemLevel) <= 0)
						{
							curColor = itemColor;
						}
						else
						{
							curColor = itemNotColor;
						}

						mu_text textBuffer[256], nameBuffer[256];
						nameBuffer[0] = _T('\0');
						GetItemName(itemType, act.ItemLevel, nameBuffer, mu_countof(nameBuffer));
						mu_sprintf_s(textBuffer, mu_countof(textBuffer), _T("") SPF_STRING _T(" x %d"), nameBuffer, act.ItemNum);
						FONT_BOLD->DrawStringC2D(TEXTINDEX::NPCQUESTWINDOW_ITEMNAME_BEGIN + itemsCount,
							x,
							y,
							1.0f,
							ETA_CENTER,
							ETA_CENTER,
							0.0f,
							curColor,
							itemBgColor,
							textBuffer,
							MU_TO_WINDOW(130.0f, screenX),
							MU_TO_WINDOW(13.0f, screenY));
					}
					break;

				case QUEST_MONSTER:
					{
						mu_float x = MU_TO_WINDOW(ITEM_POSX + 80.0f, screenX);
						mu_float y = MU_TO_WINDOW(ITEM_POSY + (((mu_float)itemsCount) * (ITEM_HEIGHT + 5.0f)) + 8.5f, screenY);
						++itemsCount;

						mu_int32 curKillCount = g_QuestManager.GetKillMobCount(act.ItemType);

						mu_uint32 curColor;
						if (curKillCount >= (mu_int32)act.ItemNum)
						{
							curColor = itemColor;
						}
						else
						{
							curColor = itemNotColor;
						}

						mu_text textBuffer[256];
						mu_sprintf_s(textBuffer, mu_countof(textBuffer), _T("") SPF_STRING _T(" x %d/%d"), GetMonsterName(act.ItemType), curKillCount, act.ItemNum);
						FONT_BOLD->DrawStringC2D(TEXTINDEX::NPCQUESTWINDOW_ITEMNAME_BEGIN + itemsCount,
							x,
							y,
							1.0f,
							ETA_CENTER,
							ETA_CENTER,
							0.0f,
							curColor,
							itemBgColor,
							textBuffer,
							MU_TO_WINDOW(130.0f, screenX),
							MU_TO_WINDOW(13.0f, screenY));
					}
					break;
				}
			}
		}
		break;
	}

	_ChangeQuestPreviewBox->Draw();
}

void UINpcQuestWindow::SetCurrentTab(mu_uint32 currentTab)
{
}

UIBase *UINpcQuestWindow::GetFocus(mu_int32 mouseX, mu_int32 mouseY)
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

		for (mu_uint32 n = 0; n < mu_countof(_AnswersButton); ++n)
		{
			object = _AnswersButton[n]->GetFocus(mouseX, mouseY);

			if (object != nullptr)
			{
				return object;
			}
		}

		switch (_CurrentQuestState)
		{
		case QUEST_ING:
			{
				object = _CompleteButton->GetFocus(mouseX, mouseY);

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