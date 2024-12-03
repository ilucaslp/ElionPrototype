#include "stdafx.h"
#include "mu_uicharacterinfo.h"

#define SCREEN_WIDTH 640.0f
#define SCREEN_HEIGHT 480.0f

#define CHARACTERINFO_POSX 450.0f
#define CHARACTERINFO_POSY 0.0f
#define CHARACTERINFO_WIDTH 190.0f
#define CHARACTERINFO_HEIGHT 429.0f

#define BACKTEXTURE_WIDTH 225.0f
#define BACKTEXTURE_HEIGHT 512.0f

#define BACK_POSX CHARACTERINFO_POSX
#define BACK_POSY CHARACTERINFO_POSY
#define BACK_WIDTH CHARACTERINFO_WIDTH
#define BACK_HEIGHT CHARACTERINFO_HEIGHT
#define BACK_UVBEGIN EVector2(0.0f, 0.0f)
#define BACK_UVEND EVector2(BACK_WIDTH / BACKTEXTURE_WIDTH, BACK_HEIGHT / BACKTEXTURE_HEIGHT)

#define TOP_WIDTH CHARACTERINFO_WIDTH
#define TOP_HEIGHT 64.0f
#define TOP_POSX CHARACTERINFO_POSX
#define TOP_POSY CHARACTERINFO_POSY

#define BOTTOM_WIDTH CHARACTERINFO_WIDTH
#define BOTTOM_HEIGHT 45.0f
#define BOTTOM_POSX CHARACTERINFO_POSX
#define BOTTOM_POSY (CHARACTERINFO_POSY + CHARACTERINFO_HEIGHT - BOTTOM_HEIGHT)

#define LEFT_WIDTH 21.0f
#define LEFT_HEIGHT (BOTTOM_POSY - (TOP_POSY + TOP_HEIGHT))
#define LEFT_POSX CHARACTERINFO_POSX
#define LEFT_POSY (TOP_POSY + TOP_HEIGHT)

#define RIGHT_WIDTH 21.0f
#define RIGHT_HEIGHT (BOTTOM_POSY - (TOP_POSY + TOP_HEIGHT))
#define RIGHT_POSX (CHARACTERINFO_POSX + CHARACTERINFO_WIDTH - RIGHT_WIDTH)
#define RIGHT_POSY (TOP_POSY + TOP_HEIGHT)

#define BACKCOLOR_POSX (CHARACTERINFO_POSX + 12.0f)
#define BACKCOLOR_POSY (CHARACTERINFO_POSY + 48.0f)
#define BACKCOLOR_WIDTH 160.0f
#define BACKCOLOR_HEIGHT 66.0f

#define XBUTTON_WIDTH 16.0f
#define XBUTTON_HEIGHT 16.0f
#define XBUTTON_POSX (TOP_POSX + TOP_WIDTH - 7.0f - XBUTTON_WIDTH)
#define XBUTTON_POSY (TOP_POSY + 7.0f)

const mu_float StatPosX = 160.0f;
const mu_float StatPosY[UICharacterInfo::STAT_MAX] = { 120.0f, 175.0f, 240.0f, 295.0f, 350.0f };

UICharacterInfo::UICharacterInfo(UISceneManager *manager, mu_float x, mu_float y, mu_float width, mu_float height) : UIBase()
{
	if (manager != nullptr)
	{
		manager->AddGFxObject(this);
	}

	_SceneManager = manager;

	UIBase::SetAspectRatio(SCREEN_WIDTH, SCREEN_HEIGHT);
	UIBase::SetPosition(x, y);
	UIBase::SetDimensions(width, height);

	SetGfxFlag(GFX_FLAG::GFX_HIDDEN);

	// Initialize
	{
		mu_text buffer[256];

		_FrameBack = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::MSGBOX_BACK], SCREEN_WIDTH, SCREEN_HEIGHT, BACK_POSX, BACK_POSY, BACK_WIDTH, BACK_HEIGHT, BACK_UVBEGIN, BACK_UVEND);
		_FrameBack->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_FrameTop = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::WINDOWFRAME_TOP_EXT], SCREEN_WIDTH, SCREEN_HEIGHT, TOP_POSX, TOP_POSY, TOP_WIDTH, TOP_HEIGHT);
		_FrameTop->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_FrameLeft = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::WINDOWFRAME_LEFT], SCREEN_WIDTH, SCREEN_HEIGHT, LEFT_POSX, LEFT_POSY, LEFT_WIDTH, LEFT_HEIGHT);
		_FrameLeft->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_FrameRight = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::WINDOWFRAME_RIGHT], SCREEN_WIDTH, SCREEN_HEIGHT, RIGHT_POSX, RIGHT_POSY, RIGHT_WIDTH, RIGHT_HEIGHT);
		_FrameRight->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_FrameBottom = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::WINDOWFRAME_BOTTOM], SCREEN_WIDTH, SCREEN_HEIGHT, BOTTOM_POSX, BOTTOM_POSY, BOTTOM_WIDTH, BOTTOM_HEIGHT);
		_FrameBottom->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_FrameBackColor = new UISpriteColor(manager, this, MU::MakeARGB(0, 0, 0, 76), SCREEN_WIDTH, SCREEN_HEIGHT, BACKCOLOR_POSX, BACKCOLOR_POSY, BACKCOLOR_WIDTH, BACKCOLOR_HEIGHT);
		_FrameBackColor->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

		_TableTopLeft = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::TABLE_TOP_LEFT], SCREEN_WIDTH, SCREEN_HEIGHT, CHARACTERINFO_POSX + 12.0f, CHARACTERINFO_POSY + 48.0f, 14.0f, 14.0f);
		_TableTopLeft->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_TableTopRight = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::TABLE_TOP_RIGHT], SCREEN_WIDTH, SCREEN_HEIGHT, CHARACTERINFO_POSX + 12.0f + 165.0f - 14.0f, CHARACTERINFO_POSY + 48.0f, 14.0f, 14.0f);
		_TableTopRight->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_TableBottomLeft = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::TABLE_BOTTOM_LEFT], SCREEN_WIDTH, SCREEN_HEIGHT, CHARACTERINFO_POSX + 12.0f, CHARACTERINFO_POSY + 119.0f - 14.0f, 14.0f, 14.0f);
		_TableBottomLeft->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_TableBottomRight = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::TABLE_BOTTOM_RIGHT], SCREEN_WIDTH, SCREEN_HEIGHT, CHARACTERINFO_POSX + 12.0f + 165.0f - 14.0f, CHARACTERINFO_POSY + 119.0f - 14.0f, 14.0f, 14.0f);
		_TableBottomRight->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

		_TableTopPixel = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::TABLE_TOP_LINE], SCREEN_WIDTH, SCREEN_HEIGHT, CHARACTERINFO_POSX + 12.0f + 14.0f, CHARACTERINFO_POSY + 48.0f, 137.0f, 14.0f);
		_TableTopPixel->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_TableBottomPixel1 = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::TABLE_BOTTOM_LINE], SCREEN_WIDTH, SCREEN_HEIGHT, CHARACTERINFO_POSX + 12.0f + 14.0f, CHARACTERINFO_POSY + 119.0f - 14.0f, 137.0f, 14.0f);
		_TableBottomPixel1->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_TableBottomPixel2 = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::TABLE_BOTTOM_LINE], SCREEN_WIDTH, SCREEN_HEIGHT, CHARACTERINFO_POSX + 14, CHARACTERINFO_POSY + 48.0f + 12.0f, 159.0f, 14.0f);
		_TableBottomPixel2->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_TableLeftPixel = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::TABLE_LEFT_LINE], SCREEN_WIDTH, SCREEN_HEIGHT, CHARACTERINFO_POSX + 12.0f, CHARACTERINFO_POSY + 48.0f + 14.0f, 14.0f, 43.0f);
		_TableLeftPixel->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_TableRightPixel = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::TABLE_RIGHT_LINE], SCREEN_WIDTH, SCREEN_HEIGHT, CHARACTERINFO_POSX + 12.0f + 165.0f - 14.0f, CHARACTERINFO_POSY + 48.0f + 14.0f, 14.0f, 43.0f);
		_TableRightPixel->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

		_StatTextBox[STAT_STRENGTH] = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::CHARFRAME_TEXTBOX], SCREEN_WIDTH, SCREEN_HEIGHT, CHARACTERINFO_POSX + 11.0f, CHARACTERINFO_POSY + StatPosY[STAT_STRENGTH], 170.0f, 21.0f);
		_StatTextBox[STAT_STRENGTH]->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_StatTextBox[STAT_DEXTERITY] = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::CHARFRAME_TEXTBOX], SCREEN_WIDTH, SCREEN_HEIGHT, CHARACTERINFO_POSX + 11.0f, CHARACTERINFO_POSY + StatPosY[STAT_DEXTERITY], 170.0f, 21.0f);
		_StatTextBox[STAT_DEXTERITY]->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_StatTextBox[STAT_VITALITY] = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::CHARFRAME_TEXTBOX], SCREEN_WIDTH, SCREEN_HEIGHT, CHARACTERINFO_POSX + 11.0f, CHARACTERINFO_POSY + StatPosY[STAT_VITALITY], 170.0f, 21.0f);
		_StatTextBox[STAT_VITALITY]->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_StatTextBox[STAT_ENERGY] = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::CHARFRAME_TEXTBOX], SCREEN_WIDTH, SCREEN_HEIGHT, CHARACTERINFO_POSX + 11.0f, CHARACTERINFO_POSY + StatPosY[STAT_ENERGY], 170.0f, 21.0f);
		_StatTextBox[STAT_ENERGY]->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_StatTextBox[STAT_COMMAND] = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::CHARFRAME_TEXTBOX], SCREEN_WIDTH, SCREEN_HEIGHT, CHARACTERINFO_POSX + 11.0f, CHARACTERINFO_POSY + StatPosY[STAT_COMMAND], 170.0f, 21.0f);
		_StatTextBox[STAT_COMMAND]->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

		_StatButton[STAT_STRENGTH] = new UIButton(GAMEUI::MESSAGE_CHARACTERINFO_STRENGTH_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, CHARACTERINFO_POSX + StatPosX, CHARACTERINFO_POSY + StatPosY[STAT_STRENGTH] + 2.0f, 16.0f, 15.0f);
		_StatButton[STAT_STRENGTH]->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_StatButton[STAT_STRENGTH]->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::CHARFRAME_BTN_STAT], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(100, 100, 100, 255)));
		_StatButton[STAT_STRENGTH]->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::CHARFRAME_BTN_STAT], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_StatButton[STAT_STRENGTH]->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::CHARFRAME_BTN_STAT], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_StatButton[STAT_STRENGTH]->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::CHARFRAME_BTN_STAT], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		
		_StatButton[STAT_DEXTERITY] = new UIButton(GAMEUI::MESSAGE_CHARACTERINFO_DEXTERITY_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, CHARACTERINFO_POSX + StatPosX, CHARACTERINFO_POSY + StatPosY[STAT_DEXTERITY] + 2.0f, 16.0f, 15.0f);
		_StatButton[STAT_DEXTERITY]->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_StatButton[STAT_DEXTERITY]->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::CHARFRAME_BTN_STAT], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(100, 100, 100, 255)));
		_StatButton[STAT_DEXTERITY]->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::CHARFRAME_BTN_STAT], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_StatButton[STAT_DEXTERITY]->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::CHARFRAME_BTN_STAT], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_StatButton[STAT_DEXTERITY]->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::CHARFRAME_BTN_STAT], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		
		_StatButton[STAT_VITALITY] = new UIButton(GAMEUI::MESSAGE_CHARACTERINFO_VITALITY_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, CHARACTERINFO_POSX + StatPosX, CHARACTERINFO_POSY + StatPosY[STAT_VITALITY] + 2.0f, 16.0f, 15.0f);
		_StatButton[STAT_VITALITY]->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_StatButton[STAT_VITALITY]->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::CHARFRAME_BTN_STAT], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(100, 100, 100, 255)));
		_StatButton[STAT_VITALITY]->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::CHARFRAME_BTN_STAT], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_StatButton[STAT_VITALITY]->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::CHARFRAME_BTN_STAT], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_StatButton[STAT_VITALITY]->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::CHARFRAME_BTN_STAT], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		
		_StatButton[STAT_ENERGY] = new UIButton(GAMEUI::MESSAGE_CHARACTERINFO_ENERGY_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, CHARACTERINFO_POSX + StatPosX, CHARACTERINFO_POSY + StatPosY[STAT_ENERGY] + 2.0f, 16.0f, 15.0f);
		_StatButton[STAT_ENERGY]->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_StatButton[STAT_ENERGY]->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::CHARFRAME_BTN_STAT], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(100, 100, 100, 255)));
		_StatButton[STAT_ENERGY]->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::CHARFRAME_BTN_STAT], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_StatButton[STAT_ENERGY]->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::CHARFRAME_BTN_STAT], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_StatButton[STAT_ENERGY]->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::CHARFRAME_BTN_STAT], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		
		_StatButton[STAT_COMMAND] = new UIButton(GAMEUI::MESSAGE_CHARACTERINFO_COMMAND_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, CHARACTERINFO_POSX + StatPosX, CHARACTERINFO_POSY + StatPosY[STAT_COMMAND] + 2.0f, 16.0f, 15.0f);
		_StatButton[STAT_COMMAND]->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_StatButton[STAT_COMMAND]->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::CHARFRAME_BTN_STAT], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(100, 100, 100, 255)));
		_StatButton[STAT_COMMAND]->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::CHARFRAME_BTN_STAT], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_StatButton[STAT_COMMAND]->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::CHARFRAME_BTN_STAT], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_StatButton[STAT_COMMAND]->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::CHARFRAME_BTN_STAT], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));

		_XButton = new UIButton(GAMEUI::MESSAGE_CHARACTERINFO_CLOSE_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, XBUTTON_POSX, XBUTTON_POSY, XBUTTON_WIDTH, XBUTTON_HEIGHT);

		mu_sprintf_s(buffer, mu_countof(buffer), GlobalText[927], _T("C"));
		_CloseButton = new UIButton(GAMEUI::MESSAGE_CHARACTERINFO_CLOSE_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, CHARACTERINFO_POSX + 13.0f, CHARACTERINFO_POSY + 392.0f, 36.0f, 29.0f);
		_CloseButton->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_CloseButton->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EXIT_BUTTON], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(100, 100, 100, 255)));
		_CloseButton->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EXIT_BUTTON], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_CloseButton->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EXIT_BUTTON], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_CloseButton->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EXIT_BUTTON], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_CloseButton->SetTooltip(FONT_NORMAL, buffer, MU::MakeARGB(255, 255, 255, 255), MU::MakeARGB(0, 0, 0, 180), true);

		mu_sprintf_s(buffer, mu_countof(buffer), _T("") SPF_STRING _T("(") SPF_STRING _T(")"), GlobalText[1140], _T("T"));
		_QuestButton = new UIButton(GAMEUI::MESSAGE_CHARACTERINFO_QUEST_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, CHARACTERINFO_POSX + 50.0f, CHARACTERINFO_POSY + 392.0f, 36.0f, 29.0f);
		_QuestButton->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_QuestButton->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::CHARFRAME_BTN_QUEST], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(100, 100, 100, 255)));
		_QuestButton->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::CHARFRAME_BTN_QUEST], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_QuestButton->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::CHARFRAME_BTN_QUEST], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_QuestButton->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::CHARFRAME_BTN_QUEST], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_QuestButton->SetTooltip(FONT_NORMAL, buffer, MU::MakeARGB(255, 255, 255, 255), MU::MakeARGB(0, 0, 0, 180), true);

		_PetButton = new UIButton(GAMEUI::MESSAGE_CHARACTERINFO_PET_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, CHARACTERINFO_POSX + 87.0f, CHARACTERINFO_POSY + 392.0f, 36.0f, 29.0f);
		_PetButton->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_PetButton->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::CHARFRAME_BTN_PET], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(100, 100, 100, 255)));
		_PetButton->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::CHARFRAME_BTN_PET], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_PetButton->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::CHARFRAME_BTN_PET], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_PetButton->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::CHARFRAME_BTN_PET], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_PetButton->SetTooltip(FONT_NORMAL, GlobalText[1217], MU::MakeARGB(255, 255, 255, 255), MU::MakeARGB(0, 0, 0, 180), true);

		_MasterButton = new UIButton(GAMEUI::MESSAGE_CHARACTERINFO_PET_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, CHARACTERINFO_POSX + 124.0f, CHARACTERINFO_POSY + 392.0f, 36.0f, 29.0f);
		_MasterButton->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_MasterButton->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::CHARFRAME_BTN_MASTER], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(100, 100, 100, 255)));
		_MasterButton->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::CHARFRAME_BTN_MASTER], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_MasterButton->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::CHARFRAME_BTN_MASTER], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_MasterButton->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::CHARFRAME_BTN_MASTER], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_MasterButton->SetTooltip(FONT_NORMAL, GlobalText[1749], MU::MakeARGB(255, 255, 255, 255), MU::MakeARGB(0, 0, 0, 180), true);
	}
}

UICharacterInfo::~UICharacterInfo()
{

}

void UICharacterInfo::Update(mu_uint64 CurrentTime)
{

}

void UICharacterInfo::Draw()
{
	DrawFrame();
	DrawButtons();
	DrawTexts();
}

void UICharacterInfo::PostDraw()
{
	PostDrawButtons();
}

void UICharacterInfo::DrawFrame()
{
	_FrameBack->Draw();
	_FrameTop->Draw();
	_FrameLeft->Draw();
	_FrameRight->Draw();
	_FrameBottom->Draw();
	_FrameBackColor->Draw();

	_TableTopLeft->Draw();
	_TableTopRight->Draw();
	_TableBottomLeft->Draw();
	_TableBottomRight->Draw();

	_TableTopPixel->Draw();
	_TableBottomPixel1->Draw();
	_TableBottomPixel2->Draw();
	_TableLeftPixel->Draw();
	_TableRightPixel->Draw();

	mu_uint32 maxIndex = GetBaseClass(Hero->Class) == GAME::CLASS_DARK_LORD ? STAT_MAX : STAT_COMMAND;
	for (mu_uint32 index = 0; index < maxIndex; ++index)
	{
		_StatTextBox[index]->Draw();
	}
}

void UICharacterInfo::DrawButtons()
{
	if(CharacterAttribute->LevelUpPoint > 0)
	{
		mu_uint32 maxIndex = GetBaseClass(Hero->Class) == GAME::CLASS_DARK_LORD ? STAT_MAX : STAT_COMMAND;
		for (mu_uint32 index = 0; index < maxIndex; ++index)
		{
			_StatButton[index]->Draw();
		}
	}

	_CloseButton->Draw();
	_QuestButton->Draw();
	_PetButton->Draw();
	_MasterButton->Draw();
}

void UICharacterInfo::PostDrawButtons()
{
	if (CharacterAttribute->LevelUpPoint > 0)
	{
		mu_uint32 maxIndex = GetBaseClass(Hero->Class) == GAME::CLASS_DARK_LORD ? STAT_MAX : STAT_COMMAND;
		for (mu_uint32 index = 0; index < maxIndex; ++index)
		{
			_StatButton[index]->PostDraw();
		}
	}

	_CloseButton->PostDraw();
	_QuestButton->PostDraw();
	_PetButton->PostDraw();
	_MasterButton->PostDraw();
}

void UICharacterInfo::DrawTexts()
{
	DrawSubjectTexts();
	DrawTableTexts();
	DrawAttribute();
}

void UICharacterInfo::DrawSubjectTexts()
{
	mu_text strID[256];
	mu_sprintf_s(strID, mu_countof(strID), _T("") SPF_STRING _T(""), CharacterAttribute->Name.c_str());

	mu_text strClassName[256];
	mu_sprintf_s(strClassName, mu_countof(strClassName), _T("(") SPF_STRING _T(")"), GetCharacterClassText(CharacterAttribute->Class));

	mu_uint32 Color, BackColor;
	MUTextSimpleRender *font = FONT_BOLD;
	BackColor = MU::MakeARGB(20, 20, 20, 20);
	SetPlayerColor(Hero->PK, Color);
	font->DrawStringC2D(TEXTINDEX::CHARACTERINFO_CHARNAME, MU_TO_WINDOW(CHARACTERINFO_POSX + CHARACTERINFO_WIDTH * 0.5f, _AspectWidth), MU_TO_WINDOW(CHARACTERINFO_POSY + 20.0f, _AspectHeight), 1.0f, ETA_CENTER, ETA_CENTER, 0.0f, Color, BackColor, strID);

	mu_text strServerName[MAX_TEXT_LENGTH];
	const mu_text *sFormat;

	switch (g_pServerList->GetSelectedServerType())
	{
	default:
	case CS2CL::SERVER_TYPE::ST_NORMAL:
		{
			if ((g_pServerList->GetSelectedServerFlag() & CS2CL::SERVER_FLAG::SF_NONPVP) != 0)
			{
				sFormat = GlobalText[460];
			}
			else
			{
				sFormat = GlobalText[461];
			}
		}
		break;

	case CS2CL::SERVER_TYPE::ST_GOLD:
		{
			if ((g_pServerList->GetSelectedServerFlag() & CS2CL::SERVER_FLAG::SF_NONPVP) != 0)
			{
				sFormat = GlobalText[3131];
			}
			else
			{
				sFormat = GlobalText[3130];
			}
		}
		break;
	}

	mu_sprintf_s(strServerName, mu_countof(strServerName), sFormat, g_pServerList->GetSelectedGroupName().c_str(), g_pServerList->GetSelectedServerIndex() + 1);

	mu_float fAlpha = EMath::Sin(FWorldTime * 0.001f) + 1.0f;
	Color = MU::MakeARGB(255, 255, 255, 127 * (2.0f - fAlpha));
	font->DrawStringC2D(TEXTINDEX::CHARACTERINFO_CLASSNAME, MU_TO_WINDOW(CHARACTERINFO_POSX + CHARACTERINFO_WIDTH * 0.5f, _AspectWidth), MU_TO_WINDOW(CHARACTERINFO_POSY + 32.0f, _AspectHeight), 1.0f, ETA_CENTER, ETA_CENTER, 2.0f, Color, BackColor, strClassName);
	Color = MU::MakeARGB(255, 255, 255, 127 * fAlpha);
	font->DrawStringC2D(TEXTINDEX::CHARACTERINFO_SERVERNAME, MU_TO_WINDOW(CHARACTERINFO_POSX + CHARACTERINFO_WIDTH * 0.5f, _AspectWidth), MU_TO_WINDOW(CHARACTERINFO_POSY + 32.0f, _AspectHeight), 1.0f, ETA_CENTER, ETA_CENTER, 2.0f, Color, BackColor, strServerName);
}

void UICharacterInfo::DrawTableTexts()
{
	mu_text strLevel[32];
	mu_text strExp[128];
	mu_text strPoint[128];

	if (IsMasterLevel(CharacterAttribute->Class) == true)
	{
		mu_sprintf_s(strLevel, mu_countof(strLevel), GlobalText[1745]);
		mu_sprintf_s(strExp, mu_countof(strExp), _T("----------"));
	}
	else
	{
		mu_sprintf_s(strLevel, mu_countof(strLevel), GlobalText[200], CharacterAttribute->Level);
		mu_sprintf_s(strExp, mu_countof(strExp), GlobalText[201], CharacterAttribute->Experience, CharacterAttribute->NextExperince);
	}

	if (CharacterAttribute->Level > 9)
	{
		mu_int32 iMinus, iMaxMinus;
		if (CharacterAttribute->wMinusPoint == 0)
		{
			iMinus = 0;
		}
		else
		{
			iMinus = -CharacterAttribute->wMinusPoint;
		}

		iMaxMinus = -CharacterAttribute->wMaxMinusPoint;

		mu_sprintf_s(strPoint, mu_countof(strPoint), _T("") SPF_STRING _T(" %d/%d | ") SPF_STRING _T(" %d/%d"),
			GlobalText[1412], CharacterAttribute->AddPoint, CharacterAttribute->MaxAddPoint,
			GlobalText[1903], iMinus, iMaxMinus);
	}
	else
	{
		mu_sprintf_s(strPoint, mu_countof(strPoint), _T("") SPF_STRING _T(" %d/%d | ") SPF_STRING _T(" %d/%d"), GlobalText[1412], 0, 0, GlobalText[1903], 0, 0);
	}

	mu_uint32 Color, BackColor;
	MUTextSimpleRender *font = FONT_BOLD;
	Color = MU::MakeARGB(230, 230, 0, 255);
	BackColor = MU::MakeARGB(0, 0, 0, 0);
	font->DrawStringC2D(TEXTINDEX::CHARACTERINFO_LEVEL, MU_TO_WINDOW(CHARACTERINFO_POSX + 18.0f, _AspectWidth), MU_TO_WINDOW(CHARACTERINFO_POSY + 61.0f, _AspectHeight), 1.0f, ETA_LEFT, ETA_CENTER, 0.0f, Color, BackColor, strLevel);

	if (CharacterAttribute->LevelUpPoint > 0)
	{
		mu_text strLevelUpPoint[128];
		mu_sprintf_s(strLevelUpPoint, mu_countof(strLevelUpPoint), GlobalText[217], CharacterAttribute->LevelUpPoint);
		Color = MU::MakeARGB(255, 138, 0, 255);
		BackColor = MU::MakeARGB(0, 0, 0, 0);
		font->DrawStringC2D(TEXTINDEX::CHARACTERINFO_STATPOINTS, MU_TO_WINDOW(CHARACTERINFO_POSX + 110.0f, _AspectWidth), MU_TO_WINDOW(CHARACTERINFO_POSY + 61.0f, _AspectHeight), 1.0f, ETA_LEFT, ETA_CENTER, 0.0f, Color, BackColor, strLevelUpPoint);
	}

	font = FONT_NORMAL;
	Color = MU::MakeARGB(255, 255, 255, 255);
	BackColor = MU::MakeARGB(0, 0, 0, 0);
	font->DrawStringC2D(TEXTINDEX::CHARACTERINFO_STATPOINTS, MU_TO_WINDOW(CHARACTERINFO_POSX + 18.0f, _AspectWidth), MU_TO_WINDOW(CHARACTERINFO_POSY + 78.0f, _AspectHeight), 1.0f, ETA_LEFT, ETA_CENTER, 0.0f, Color, BackColor, strExp);
	
	mu_int32 iAddPoint, iMinusPoint;

	if (CharacterAttribute->AddPoint <= 10)
	{
		iAddPoint = 100;
	}
	else
	{
		mu_int32 iTemp = 0;
		if (CharacterAttribute->MaxAddPoint != 0)
		{
			iTemp = (CharacterAttribute->AddPoint * 100) / CharacterAttribute->MaxAddPoint;
		}

		if (iTemp <= 10)
		{
			iAddPoint = 70;
		}
		else if (iTemp <= 30)
		{
			iAddPoint = 60;
		}
		else if (iTemp <= 50)
		{
			iAddPoint = 50;
		}
		else
		{
			iAddPoint = 40;
		}
	}

	if (CharacterAttribute->wMinusPoint <= 10)
	{
		iMinusPoint = 100;
	}
	else
	{
		mu_int32 iTemp = 0;
		if (CharacterAttribute->wMaxMinusPoint != 0)
		{
			iTemp = (CharacterAttribute->wMinusPoint * 100) / CharacterAttribute->wMaxMinusPoint;
		}

		if (iTemp <= 10)
		{
			iMinusPoint = 70;
		}
		else if (iTemp <= 30)
		{
			iMinusPoint = 60;
		}
		else if (iTemp <= 50)
		{
			iMinusPoint = 50;
		}
		else
		{
			iMinusPoint = 40;
		}
	}

	mu_text strPointProbability[128];

	mu_sprintf_s(strPointProbability, mu_countof(strPointProbability), GlobalText[1907], iAddPoint, iMinusPoint);
	font = FONT_NORMAL;
	Color = MU::MakeARGB(76, 197, 254, 255);
	BackColor = MU::MakeARGB(0, 0, 0, 0);
	font->DrawStringC2D(TEXTINDEX::CHARACTERINFO_POINTPROB, MU_TO_WINDOW(CHARACTERINFO_POSX + 18.0f, _AspectWidth), MU_TO_WINDOW(CHARACTERINFO_POSY + 91.0f, _AspectHeight), 1.0f, ETA_LEFT, ETA_CENTER, 0.0f, Color, BackColor, strPointProbability);

	Color = MU::MakeARGB(76, 197, 254, 255);
	BackColor = MU::MakeARGB(0, 0, 0, 0);
	font->DrawStringC2D(TEXTINDEX::CHARACTERINFO_POINT, MU_TO_WINDOW(CHARACTERINFO_POSX + 18.0f, _AspectWidth), MU_TO_WINDOW(CHARACTERINFO_POSY + 104.0f, _AspectHeight), 1.0f, ETA_LEFT, ETA_CENTER, 0.0f, Color, BackColor, strPoint);
}

void UICharacterInfo::DrawAttribute()
{
	mu_uint32 Color, BackColor;
	MUTextSimpleRender *font = FONT_BOLD;

	mu_uint32 wStrength = CharacterAttribute->Strength + CharacterAttribute->AddStrength;

	if (g_isCharacterBuff((&Hero->Object), eBuff_SecretPotion1))
	{
		Color = MU::MakeARGB(255, 120, 0, 255);
	}
	else if (CharacterAttribute->AddStrength)
	{
		Color = MU::MakeARGB(100, 150, 255, 255);
	}
	else
	{
		Color = MU::MakeARGB(230, 230, 0, 255);
	}

	mu_text strStrength[32];
	mu_sprintf_s(strStrength, mu_countof(strStrength), _T("%d"), wStrength);

	BackColor = MU::MakeARGB(0, 0, 0, 0);
	
	font->DrawStringC2D(TEXTINDEX::CHARACTERINFO_STRENGTH_LABEL, MU_TO_WINDOW(CHARACTERINFO_POSX + 12.0f + 37.0f, _AspectWidth), MU_TO_WINDOW(CHARACTERINFO_POSY + StatPosY[STAT_STRENGTH] + 9.0f, _AspectHeight), 1.0f, ETA_CENTER, ETA_CENTER, 0.0f, Color, BackColor, GlobalText[166]);
	font->DrawStringC2D(TEXTINDEX::CHARACTERINFO_STRENGTH, MU_TO_WINDOW(CHARACTERINFO_POSX + 86.0f + 43.0f, _AspectWidth), MU_TO_WINDOW(CHARACTERINFO_POSY + StatPosY[STAT_STRENGTH] + 9.0f, _AspectHeight), 1.0f, ETA_CENTER, ETA_CENTER, 0.0f, Color, BackColor, strStrength);

	mu_text strAttakMamage[256];
	mu_int32 iAttackDamageMin = 0;
	mu_int32 iAttackDamageMax = 0;

	mu_boolean bAttackDamage = GetAttackDamage(&iAttackDamageMin, &iAttackDamageMax);

	mu_int32 Add_Dex = 0;
	mu_int32 Add_Rat = 0;
	mu_int32 Add_Dfe = 0;
	mu_int32 Add_Att_Max = 0;
	mu_int32 Add_Att_Min = 0;
	mu_int32 Add_Ch_Dfe = 0;
	mu_int32 Add_Mana_Max = 0;
	mu_int32 Add_Mana_Min = 0;
	for (mu_int32 i = 0; i < MAX_MAGIC; ++i)
	{
		if (CharacterAttribute->Skill[i] >= AT_SKILL_TOMAN_ATTACKUP && CharacterAttribute->Skill[i] < AT_SKILL_TOMAN_ATTACKUP + 5)
		{
			Add_Dex = SkillAttribute[CharacterAttribute->Skill[i]].Damage;
		}
		else if (CharacterAttribute->Skill[i] >= AT_SKILL_ATTACK_RATEUP && CharacterAttribute->Skill[i] < AT_SKILL_ATTACK_RATEUP + 5)
		{
			Add_Rat = SkillAttribute[CharacterAttribute->Skill[i]].Damage;
		}
		else if (CharacterAttribute->Skill[i] >= AT_SKILL_TOMAN_DEFENCEUP && CharacterAttribute->Skill[i] < AT_SKILL_TOMAN_DEFENCEUP + 5)
		{
			Add_Dfe = SkillAttribute[CharacterAttribute->Skill[i]].Damage;
		}
		else if (CharacterAttribute->Skill[i] >= AT_SKILL_DEF_UP && CharacterAttribute->Skill[i] < AT_SKILL_DEF_UP + 5)
		{
			Add_Ch_Dfe = SkillAttribute[CharacterAttribute->Skill[i]].Damage;
		}
		else if (CharacterAttribute->Skill[i] >= AT_SKILL_MAX_ATTACKRATE_UP && CharacterAttribute->Skill[i] < AT_SKILL_MAX_ATTACKRATE_UP + 5)
		{
			Add_Att_Max = SkillAttribute[CharacterAttribute->Skill[i]].Damage;
		}
		else if (CharacterAttribute->Skill[i] >= AT_SKILL_MAX_ATT_MAGIC_UP && CharacterAttribute->Skill[i] < AT_SKILL_MAX_ATT_MAGIC_UP + 5)
		{
			Add_Mana_Max = Add_Att_Max = SkillAttribute[CharacterAttribute->Skill[i]].Damage;
		}
		else if (CharacterAttribute->Skill[i] >= AT_SKILL_MIN_ATT_MAGIC_UP && CharacterAttribute->Skill[i] < AT_SKILL_MIN_ATT_MAGIC_UP + 5)
		{
			Add_Mana_Min = Add_Att_Min = SkillAttribute[CharacterAttribute->Skill[i]].Damage;
		}
		else if (CharacterAttribute->Skill[i] >= AT_SKILL_MAX_MANA_UP && CharacterAttribute->Skill[i] < AT_SKILL_MAX_MANA_UP + 5)
		{
			Add_Mana_Max = SkillAttribute[CharacterAttribute->Skill[i]].Damage;
		}
		else if (CharacterAttribute->Skill[i] >= AT_SKILL_MIN_MANA_UP && CharacterAttribute->Skill[i] < AT_SKILL_MIN_MANA_UP + 5)
		{
			Add_Mana_Min = SkillAttribute[CharacterAttribute->Skill[i]].Damage;
		}
		else if (CharacterAttribute->Skill[i] >= AT_SKILL_MIN_ATTACKRATE_UP && CharacterAttribute->Skill[i] < AT_SKILL_MIN_ATTACKRATE_UP + 5)
		{
			Add_Att_Min = SkillAttribute[CharacterAttribute->Skill[i]].Damage;
		}
	}

	ITEM* pWeaponRight = &CharacterMachine->Equipment[GAME::EQUIPMENT_WEAPON_RIGHT];
	ITEM* pWeaponLeft = &CharacterMachine->Equipment[GAME::EQUIPMENT_WEAPON_LEFT];

	mu_int32 iAttackRating = CharacterAttribute->AttackRating + Add_Rat;
	mu_int32 iAttackRatingPK = CharacterAttribute->AttackRatingPK + Add_Dex;
	iAttackDamageMax += Add_Att_Max;
	iAttackDamageMin += Add_Att_Min;

	if (g_isCharacterBuff((&Hero->Object), eBuff_AddAG))
	{
		mu_uint32 wDexterity = CharacterAttribute->Dexterity + CharacterAttribute->AddDexterity;
		iAttackRating += wDexterity;
		iAttackRatingPK += wDexterity;
		if (PartyNumber >= 3)
		{
			mu_int32 iPlusRating = (wDexterity * ((PartyNumber - 2) * 0.01f));
			iAttackRating += iPlusRating;
			iAttackRatingPK = iPlusRating;
		}
	}
	if (g_isCharacterBuff((&Hero->Object), eBuff_HelpNpc))
	{
		mu_int32 iTemp = 0;

		if (CharacterAttribute->Level > 180)
		{
			iTemp = (180 / 3) + 45;
		}
		else
		{
			iTemp = (CharacterAttribute->Level / 3) + 45;
		}

		iAttackDamageMin += iTemp;
		iAttackDamageMax += iTemp;
	}

	if (g_isCharacterBuff((&Hero->Object), eBuff_Berserker))
	{
		mu_int32 nTemp = CharacterAttribute->Strength + CharacterAttribute->AddStrength
			+ CharacterAttribute->Dexterity + CharacterAttribute->AddDexterity;
		mu_float fTemp = mu_int32(CharacterAttribute->Energy / 30) / 100.0f;
		iAttackDamageMin += nTemp / 7 * fTemp;
		iAttackDamageMax += nTemp / 4 * fTemp;
	}

	mu_int32 iMinIndex = 0, iMaxIndex = 0, iMagicalIndex = 0;

	StrengthenCapability SC_r, SC_l;

	mu_int32 rlevel = pWeaponRight->Level;

	if (rlevel >= pWeaponRight->HarmonyOptionLevel)
	{
		g_pUIJewelHarmonyinfo.GetStrengthenCapability(&SC_r, pWeaponRight, 1);

		if (SC_r.SI_isSP)
		{
			iMinIndex = SC_r.SI_SP.SI_minattackpower;
			iMaxIndex = SC_r.SI_SP.SI_maxattackpower;
			iMagicalIndex = SC_r.SI_SP.SI_magicalpower;
		}
	}

	mu_int32 llevel = pWeaponLeft->Level;

	if (llevel >= pWeaponLeft->HarmonyOptionLevel)
	{
		g_pUIJewelHarmonyinfo.GetStrengthenCapability(&SC_l, pWeaponLeft, 1);

		if (SC_l.SI_isSP)
		{
			iMinIndex += SC_l.SI_SP.SI_minattackpower;
			iMaxIndex += SC_l.SI_SP.SI_maxattackpower;
			iMagicalIndex += SC_l.SI_SP.SI_magicalpower;
		}
	}

	mu_int32 iDefenseRate = 0, iAttackPowerRate = 0;

	StrengthenCapability rightinfo, leftinfo;

	mu_int32 iRightLevel = pWeaponRight->Level;

	if (iRightLevel >= pWeaponRight->HarmonyOptionLevel)
	{
		g_pUIJewelHarmonyinfo.GetStrengthenCapability(&rightinfo, pWeaponRight, 1);
	}

	mu_int32 iLeftLevel = pWeaponLeft->Level;

	if (iLeftLevel >= pWeaponLeft->HarmonyOptionLevel)
	{
		g_pUIJewelHarmonyinfo.GetStrengthenCapability(&leftinfo, pWeaponLeft, 1);
	}

	if (rightinfo.SI_isSP)
	{
		iAttackPowerRate += rightinfo.SI_SP.SI_attackpowerRate;
	}
	if (leftinfo.SI_isSP)
	{
		iAttackPowerRate += leftinfo.SI_SP.SI_attackpowerRate;
	}

	for (mu_int32 k = GAME::EQUIPMENT_WEAPON_LEFT; k < GAME::MAX_EQUIPMENT; ++k)
	{
		StrengthenCapability defenseinfo;

		ITEM* pItem = &CharacterMachine->Equipment[k];

		mu_int32 eqlevel = pItem->Level;

		if (eqlevel >= pItem->HarmonyOptionLevel)
		{
			g_pUIJewelHarmonyinfo.GetStrengthenCapability(&defenseinfo, pItem, 2);
		}

		if (defenseinfo.SI_isSD)
		{
			iDefenseRate += defenseinfo.SI_SD.SI_defenseRate;
		}
	}

	mu_int32 itemoption380Attack = 0;
	mu_int32 itemoption380Defense = 0;

	for (mu_int32 j = 0; j < GAME::MAX_EQUIPMENT; ++j)
	{
		mu_boolean is380item = CharacterMachine->Equipment[j].ItemOption;
		mu_int32 i380type = CharacterMachine->Equipment[j].Type;

		if (is380item && i380type > -1 && i380type < MAX_ITEM)
		{
			const ITEM_ADD_OPTION &item380option = g_pItemAddOptioninfo.GetItemAddOtioninfo(CharacterMachine->Equipment[j].Type);

			if (item380option.m_byOption1 == 1)
			{
				itemoption380Attack += item380option.m_byValue1;
			}

			if (item380option.m_byOption2 == 1)
			{
				itemoption380Attack += item380option.m_byValue2;
			}

			if (item380option.m_byOption1 == 3)
			{
				itemoption380Defense += item380option.m_byValue1;
			}

			if (item380option.m_byOption2 == 3)
			{
				itemoption380Defense += item380option.m_byValue2;
			}
		}
	}

	ITEM *pItemRingLeft, *pItemRingRight;

	pItemRingLeft = &CharacterMachine->Equipment[GAME::EQUIPMENT_RING_LEFT];
	pItemRingRight = &CharacterMachine->Equipment[GAME::EQUIPMENT_RING_RIGHT];
	if (pItemRingLeft && pItemRingRight)
	{
		mu_int32 iNonExpiredLRingType = -1;
		mu_int32 iNonExpiredRRingType = -1;
		iNonExpiredLRingType = pItemRingLeft->Type;
		iNonExpiredRRingType = pItemRingRight->Type;

		mu_int32 maxIAttackDamageMin = 0;
		mu_int32 maxIAttackDamageMax = 0;
		if (iNonExpiredLRingType == ITEM_HELPER + 41 || iNonExpiredRRingType == ITEM_HELPER + 41)
		{
			maxIAttackDamageMin = EMath::Max(maxIAttackDamageMin, 20);
			maxIAttackDamageMax = EMath::Max(maxIAttackDamageMax, 20);
		}
		if (iNonExpiredLRingType == ITEM_HELPER + 76 || iNonExpiredRRingType == ITEM_HELPER + 76)
		{
			maxIAttackDamageMin = EMath::Max(maxIAttackDamageMin, 30);
			maxIAttackDamageMax = EMath::Max(maxIAttackDamageMax, 30);
		}

		if (iNonExpiredLRingType == ITEM_HELPER + 122 || iNonExpiredRRingType == ITEM_HELPER + 122)
		{
			maxIAttackDamageMin = EMath::Max(maxIAttackDamageMin, 40);
			maxIAttackDamageMax = EMath::Max(maxIAttackDamageMax, 40);
		}

		iAttackDamageMin += maxIAttackDamageMin;
		iAttackDamageMax += maxIAttackDamageMax;
	}

	ITEM *pItemHelper = &CharacterMachine->Equipment[GAME::EQUIPMENT_HELPER];
	if (pItemHelper)
	{
		if (pItemHelper->Type == ITEM_HELPER + 37 && pItemHelper->ExcOption == 0x04)
		{
			mu_uint32 wLevel = CharacterAttribute->Level;
			iAttackDamageMin += (wLevel / 12);
			iAttackDamageMax += (wLevel / 12);
		}

		if (pItemHelper->Type == ITEM_HELPER + 64)
		{
			iAttackDamageMin += mu_int32(mu_float(iAttackDamageMin) * 0.4f);
			iAttackDamageMax += mu_int32(mu_float(iAttackDamageMax) * 0.4f);
		}

		if (pItemHelper->Type == ITEM_HELPER + 123)
		{
			iAttackDamageMin += mu_int32(mu_float(iAttackDamageMin) * 0.2f);
			iAttackDamageMax += mu_int32(mu_float(iAttackDamageMax) * 0.2f);
		}

		if (pItemHelper->Type == ITEM_HELPER + 1)
		{
			iAttackDamageMin += mu_int32(mu_float(iAttackDamageMin) * 0.3f);
			iAttackDamageMax += mu_int32(mu_float(iAttackDamageMax) * 0.3f);
		}
	}

	if (iAttackRating > 0)
	{
		if (iAttackDamageMin + iMinIndex >= iAttackDamageMax + iMaxIndex)
		{
			mu_sprintf_s(strAttakMamage, mu_countof(strAttakMamage), GlobalText[203], iAttackDamageMax + iMaxIndex, iAttackDamageMax + iMaxIndex, iAttackRating);
		}
		else
		{
			mu_sprintf_s(strAttakMamage, mu_countof(strAttakMamage), GlobalText[203], iAttackDamageMin + iMinIndex, iAttackDamageMax + iMaxIndex, iAttackRating);
		}
	}
	else
	{
		if (iAttackDamageMin + iMinIndex >= iAttackDamageMax + iMaxIndex)
		{
			mu_sprintf_s(strAttakMamage, mu_countof(strAttakMamage), GlobalText[204], iAttackDamageMax + iMaxIndex, iAttackDamageMax + iMaxIndex);
		}
		else
		{
			mu_sprintf_s(strAttakMamage, mu_countof(strAttakMamage), GlobalText[204], iAttackDamageMin + iMinIndex, iAttackDamageMax + iMaxIndex);
		}
	}

	font = FONT_NORMAL;
	BackColor = MU::MakeARGB(0, 0, 0, 0);

	if (bAttackDamage)
	{
		Color = MU::MakeARGB(100, 150, 255, 255);
	}
	else
	{
		Color = MU::MakeARGB(255, 255, 255, 255);
	}

	if (g_isCharacterBuff((&Hero->Object), eBuff_Hellowin2))
	{
		Color = MU::MakeARGB(255, 0, 240, 255);
	}

	if (g_isCharacterBuff((&Hero->Object), eBuff_EliteScroll3))
	{
		Color = MU::MakeARGB(255, 0, 240, 255);
	}

	if (g_isCharacterBuff((&Hero->Object), eBuff_CherryBlossom_Petal))
	{
		Color = MU::MakeARGB(255, 0, 240, 255);
	}

	mu_float iY = StatPosY[STAT_STRENGTH] + 27.0f;
	font->DrawStringC2D(TEXTINDEX::CHARACTERINFO_ATTACKDAMAGE, MU_TO_WINDOW(CHARACTERINFO_POSX + 20.0f, _AspectWidth), MU_TO_WINDOW(CHARACTERINFO_POSY + StatPosY[STAT_STRENGTH] + 28.0f, _AspectHeight), 1.0f, ETA_LEFT, ETA_CENTER, 0.0f, Color, BackColor, strAttakMamage);

	if (iAttackRatingPK > 0)
	{
		if (itemoption380Attack != 0 || iAttackPowerRate != 0)
		{
			mu_sprintf_s(strAttakMamage, mu_countof(strAttakMamage), GlobalText[2109], iAttackRatingPK, itemoption380Attack + iAttackPowerRate);
		}
		else
		{
			mu_sprintf_s(strAttakMamage, mu_countof(strAttakMamage), GlobalText[2044], iAttackRatingPK);
		}

		iY += 13.0f;
		font->DrawStringC2D(TEXTINDEX::CHARACTERINFO_ATTACKRATE, MU_TO_WINDOW(CHARACTERINFO_POSX + 20.0f, _AspectWidth), MU_TO_WINDOW(CHARACTERINFO_POSY + iY, _AspectHeight), 1.0f, ETA_LEFT, ETA_CENTER, 0.0f, Color, BackColor, strAttakMamage);
	}

	font = FONT_BOLD;
	BackColor = MU::MakeARGB(0, 0, 0, 0);

	if (g_isCharacterBuff((&Hero->Object), eBuff_SecretPotion2))
	{
		Color = MU::MakeARGB(255, 120, 0, 255);
	}
	else if (CharacterAttribute->AddDexterity)
	{
		Color = MU::MakeARGB(100, 150, 255, 255);
	}
	else
	{
		Color = MU::MakeARGB(230, 230, 0, 255);
	}

	mu_text strDexterity[32];
	mu_uint32 wDexterity = CharacterAttribute->Dexterity + CharacterAttribute->AddDexterity;
	mu_sprintf_s(strDexterity, mu_countof(strDexterity), _T("%d"), wDexterity);

	font->DrawStringC2D(TEXTINDEX::CHARACTERINFO_DEXTERITY_LABEL, MU_TO_WINDOW(CHARACTERINFO_POSX + 12.0f + 37.0f, _AspectWidth), MU_TO_WINDOW(CHARACTERINFO_POSY + StatPosY[STAT_DEXTERITY] + 9.0f, _AspectHeight), 1.0f, ETA_CENTER, ETA_CENTER, 0.0f, Color, BackColor, GlobalText[1702]);
	font->DrawStringC2D(TEXTINDEX::CHARACTERINFO_DEXTERITY, MU_TO_WINDOW(CHARACTERINFO_POSX + 86.0f + 43.0f, _AspectWidth), MU_TO_WINDOW(CHARACTERINFO_POSY + StatPosY[STAT_DEXTERITY] + 9.0f, _AspectHeight), 1.0f, ETA_CENTER, ETA_CENTER, 0.0f, Color, BackColor, strDexterity);

	mu_boolean bDexSuccess = true;
	mu_int32 iBaseClass = GetBaseClass(Hero->Class);

	for (mu_int32 i = GAME::EQUIPMENT_HELM; i <= GAME::EQUIPMENT_BOOTS; ++i)
	{
		if (iBaseClass == GAME::CLASS_DARK)
		{
			if ((CharacterMachine->Equipment[i].Type == -1 && (i != GAME::EQUIPMENT_HELM && iBaseClass == GAME::CLASS_DARK))
				|| (CharacterMachine->Equipment[i].Type != -1 && CharacterMachine->Equipment[i].Durability <= 0))
			{
				bDexSuccess = false;
				break;
			}
		}
		else
		{
			if ((CharacterMachine->Equipment[i].Type == -1) ||
				(CharacterMachine->Equipment[i].Type != -1 && CharacterMachine->Equipment[i].Durability <= 0))
			{
				bDexSuccess = false;
				break;
			}
		}
	}

	if (bDexSuccess)
	{
		mu_int32 iType;
		if (iBaseClass == GAME::CLASS_DARK)
		{
			iType = CharacterMachine->Equipment[GAME::EQUIPMENT_ARMOR].Type;

			if (
				(iType != ITEM_ARMOR + 15)
				&& (iType != ITEM_ARMOR + 20)
				&& (iType != ITEM_ARMOR + 23)
				&& (iType != ITEM_ARMOR + 32)
				&& (iType != ITEM_ARMOR + 37)
				&& (iType != ITEM_ARMOR + 47)
				&& (iType != ITEM_ARMOR + 48)
				)
			{
				bDexSuccess = false;
			}

			iType = iType % MAX_ITEM_INDEX;
		}
		else
		{
			iType = CharacterMachine->Equipment[GAME::EQUIPMENT_HELM].Type % MAX_ITEM_INDEX;
		}

		if (bDexSuccess)
		{
			for (mu_int32 i = GAME::EQUIPMENT_ARMOR; i <= GAME::EQUIPMENT_BOOTS; ++i)
			{
				if (iType != CharacterMachine->Equipment[i].Type % MAX_ITEM_INDEX)
				{
					bDexSuccess = false;
					break;
				}
			}
		}
	}

	mu_int32 t_adjdef = CharacterAttribute->Defense + Add_Ch_Dfe;

	if (g_isCharacterBuff((&Hero->Object), eBuff_HelpNpc))
	{
		if (CharacterAttribute->Level > 180)
		{
			t_adjdef += 180 / 5 + 50;
		}
		else
		{
			t_adjdef += (CharacterAttribute->Level / 5 + 50);
		}
	}

	if (g_isCharacterBuff((&Hero->Object), eBuff_Berserker))
	{
		mu_int32 nTemp = CharacterAttribute->Dexterity + CharacterAttribute->AddDexterity;
		mu_float fTemp = (40 - mu_int32(CharacterAttribute->Energy / 60)) / 100.0f;
		fTemp = EMath::Max(fTemp, 0.1f);
		t_adjdef -= nTemp / 3 * fTemp;
	}

	mu_int32 maxdefense = 0;

	for (mu_int32 j = 0; j < GAME::MAX_EQUIPMENT; ++j)
	{
		mu_int32 TempLevel = CharacterMachine->Equipment[j].Level;
		if (TempLevel >= CharacterMachine->Equipment[j].HarmonyOptionLevel)
		{
			StrengthenCapability SC;

			g_pUIJewelHarmonyinfo.GetStrengthenCapability(&SC, &CharacterMachine->Equipment[j], 2);

			if (SC.SI_isSD)
			{
				maxdefense += SC.SI_SD.SI_defense;
			}
		}
	}

	mu_int32 iChangeRingAddDefense = 0;

	pItemRingLeft = &CharacterMachine->Equipment[GAME::EQUIPMENT_RING_LEFT];
	pItemRingRight = &CharacterMachine->Equipment[GAME::EQUIPMENT_RING_RIGHT];
	if (pItemRingLeft->Type == ITEM_HELPER + 39 || pItemRingRight->Type == ITEM_HELPER + 39)
	{
		iChangeRingAddDefense = (t_adjdef + maxdefense) / 10;
	}

	if (Hero->Helper.Type == MODEL_HELPER + 80)
		iChangeRingAddDefense += 50;

	if (Hero->Helper.Type == MODEL_HELPER + 106)
		iChangeRingAddDefense += 50;

	mu_text strBlocking[256];

	if (bDexSuccess)
	{
		if (CharacterAttribute->SuccessfulBlocking > 0)
		{
			mu_sprintf_s(strBlocking, mu_countof(strBlocking), GlobalText[206],
				t_adjdef + maxdefense + iChangeRingAddDefense,
				CharacterAttribute->SuccessfulBlocking,
				(CharacterAttribute->SuccessfulBlocking) / 10
			);
		}
		else
		{
			mu_sprintf_s(strBlocking, mu_countof(strBlocking), GlobalText[207],
				t_adjdef + maxdefense + iChangeRingAddDefense,
				(t_adjdef + iChangeRingAddDefense) / 10
			);
		}
	}
	else
	{
		if (CharacterAttribute->SuccessfulBlocking > 0)
		{
			mu_sprintf_s(strBlocking, mu_countof(strBlocking), GlobalText[208],
				t_adjdef + maxdefense + iChangeRingAddDefense,
				CharacterAttribute->SuccessfulBlocking
			);
		}
		else
		{
			mu_sprintf_s(strBlocking, mu_countof(strBlocking), GlobalText[209],
				t_adjdef + maxdefense + iChangeRingAddDefense
			);
		}
	}

	iY = StatPosY[STAT_DEXTERITY] + 27.0f;
	font = FONT_NORMAL;
	Color = MU::MakeARGB(255, 255, 255, 255);

	if (g_isCharacterBuff((&Hero->Object), eBuff_Hellowin3))
	{
		Color = MU::MakeARGB(255, 0, 240, 255);
	}

	if (g_isCharacterBuff((&Hero->Object), eBuff_EliteScroll2))
	{
		Color = MU::MakeARGB(255, 0, 240, 255);
	}

	BackColor = MU::MakeARGB(0, 0, 0, 0);
	font->DrawStringC2D(TEXTINDEX::CHARACTERINFO_BLOCKINGRATE, MU_TO_WINDOW(CHARACTERINFO_POSX + 20.0f, _AspectWidth), MU_TO_WINDOW(CHARACTERINFO_POSY + iY, _AspectHeight), 1.0f, ETA_LEFT, ETA_CENTER, 0.0f, Color, BackColor, strBlocking);

	mu_uint32 wAttackSpeed = GAME::CLASS_WIZARD == iBaseClass || GAME::CLASS_SUMMONER == iBaseClass
		? CharacterAttribute->MagicSpeed : CharacterAttribute->AttackSpeed;

	mu_sprintf_s(strBlocking, mu_countof(strBlocking), GlobalText[64], wAttackSpeed);
	iY += 13.0f;

	font = FONT_NORMAL;
	Color = MU::MakeARGB(255, 255, 255, 255);

	if (g_isCharacterBuff((&Hero->Object), eBuff_Hellowin1))
	{
		Color = MU::MakeARGB(255, 0, 240, 255);
	}

	if (g_isCharacterBuff((&Hero->Object), eBuff_EliteScroll1))
	{
		Color = MU::MakeARGB(255, 0, 240, 255);
	}

	ITEM *phelper = &CharacterMachine->Equipment[GAME::EQUIPMENT_HELPER];
	if (phelper->Durability != 0 &&
		(phelper->Type == ITEM_HELPER + 64
			|| phelper->Type == ITEM_HELPER + 123
			))
	{
		if (IsRequireEquipItem(phelper))
		{
			Color = MU::MakeARGB(255, 0, 240, 255);
		}
	}

	BackColor = MU::MakeARGB(0, 0, 0, 0);
	font->DrawStringC2D(TEXTINDEX::CHARACTERINFO_ATTACKSPEED, MU_TO_WINDOW(CHARACTERINFO_POSX + 20.0f, _AspectWidth), MU_TO_WINDOW(CHARACTERINFO_POSY + iY, _AspectHeight), 1.0f, ETA_LEFT, ETA_CENTER, 0.0f, Color, BackColor, strBlocking);

	if (itemoption380Defense != 0 || iDefenseRate != 0)
	{
		mu_sprintf_s(strBlocking, mu_countof(strBlocking), GlobalText[2110], CharacterAttribute->SuccessfulBlockingPK + Add_Dfe, itemoption380Defense + iDefenseRate);
	}
	else
	{
		mu_sprintf_s(strBlocking, mu_countof(strBlocking), GlobalText[2045], CharacterAttribute->SuccessfulBlockingPK + Add_Dfe);
	}

	iY += 13.0f;
	font = FONT_NORMAL;
	Color = MU::MakeARGB(255, 255, 255, 255);
	BackColor = MU::MakeARGB(0, 0, 0, 0);
	font->DrawStringC2D(TEXTINDEX::CHARACTERINFO_DEFENSERATE, MU_TO_WINDOW(CHARACTERINFO_POSX + 20.0f, _AspectWidth), MU_TO_WINDOW(CHARACTERINFO_POSY + iY, _AspectHeight), 1.0f, ETA_LEFT, ETA_CENTER, 0.0f, Color, BackColor, strBlocking);

	font = FONT_BOLD;

	mu_uint32 wVitality = CharacterAttribute->Vitality + CharacterAttribute->AddVitality;

	if (g_isCharacterBuff((&Hero->Object), eBuff_SecretPotion3))
	{
		Color = MU::MakeARGB(255, 120, 0, 255);
	}
	else if (CharacterAttribute->AddVitality)
	{
		Color = MU::MakeARGB(100, 150, 255, 255);
	}
	else
	{
		Color = MU::MakeARGB(230, 230, 0, 255);
	}

	mu_text strVitality[256];
	mu_sprintf_s(strVitality, mu_countof(strVitality), _T("%d"), wVitality);
	BackColor = MU::MakeARGB(0, 0, 0, 0);

	font->DrawStringC2D(TEXTINDEX::CHARACTERINFO_VITALITY_LABEL, MU_TO_WINDOW(CHARACTERINFO_POSX + 12.0f + 37.0f, _AspectWidth), MU_TO_WINDOW(CHARACTERINFO_POSY + StatPosY[STAT_VITALITY] + 9.0f, _AspectHeight), 1.0f, ETA_CENTER, ETA_CENTER, 0.0f, Color, BackColor, GlobalText[169]);
	font->DrawStringC2D(TEXTINDEX::CHARACTERINFO_VITALITY, MU_TO_WINDOW(CHARACTERINFO_POSX + 86.0f + 43.0f, _AspectWidth), MU_TO_WINDOW(CHARACTERINFO_POSY + StatPosY[STAT_VITALITY] + 9.0f, _AspectHeight), 1.0f, ETA_CENTER, ETA_CENTER, 0.0f, Color, BackColor, strVitality);

	font = FONT_NORMAL;

	mu_sprintf_s(strVitality, mu_countof(strVitality), GlobalText[211], (mu_int32)CharacterAttribute->Life, (mu_int32)CharacterAttribute->LifeMax);

	Color = MU::MakeARGB(255, 255, 255, 255);

	if (g_isCharacterBuff((&Hero->Object), eBuff_Hellowin4))
	{
		Color = MU::MakeARGB(255, 0, 240, 255);
	}

	if (g_isCharacterBuff((&Hero->Object), eBuff_EliteScroll5))
	{
		Color = MU::MakeARGB(255, 0, 240, 255);
	}

	if (g_isCharacterBuff((&Hero->Object), eBuff_CherryBlossom_RiceCake))
	{
		Color = MU::MakeARGB(255, 0, 240, 255);
	}

	if (phelper->Durability != 0 && phelper->Type == ITEM_HELPER + 65)
	{
		if (IsRequireEquipItem(phelper))
		{
			Color = MU::MakeARGB(255, 0, 240, 255);
		}
	}

	BackColor = MU::MakeARGB(0, 0, 0, 0);
	iY = StatPosY[STAT_VITALITY] + 27.0f;
	font->DrawStringC2D(TEXTINDEX::CHARACTERINFO_LIFE, MU_TO_WINDOW(CHARACTERINFO_POSX + 20.0f, _AspectWidth), MU_TO_WINDOW(CHARACTERINFO_POSY + iY, _AspectHeight), 1.0f, ETA_LEFT, ETA_CENTER, 0.0f, Color, BackColor, strVitality);

	font = FONT_BOLD;

	mu_uint32 wEnergy = CharacterAttribute->Energy + CharacterAttribute->AddEnergy;

	if (g_isCharacterBuff((&Hero->Object), eBuff_SecretPotion4))
	{
		Color = MU::MakeARGB(255, 120, 0, 255);
	}
	else if (CharacterAttribute->AddEnergy)
	{
		Color = MU::MakeARGB(100, 150, 255, 255);
	}
	else
	{
		Color = MU::MakeARGB(230, 230, 0, 255);
	}

	mu_text strEnergy[256];
	mu_sprintf_s(strEnergy, mu_countof(strEnergy), _T("%d"), wEnergy);

	BackColor = MU::MakeARGB(0, 0, 0, 0);

	font->DrawStringC2D(TEXTINDEX::CHARACTERINFO_ENERGY_LABEL, MU_TO_WINDOW(CHARACTERINFO_POSX + 12.0f + 37.0f, _AspectWidth), MU_TO_WINDOW(CHARACTERINFO_POSY + StatPosY[STAT_ENERGY] + 9.0f, _AspectHeight), 1.0f, ETA_CENTER, ETA_CENTER, 0.0f, Color, BackColor, GlobalText[168]);
	font->DrawStringC2D(TEXTINDEX::CHARACTERINFO_ENERGY, MU_TO_WINDOW(CHARACTERINFO_POSX + 86.0f + 43.0f, _AspectWidth), MU_TO_WINDOW(CHARACTERINFO_POSY + StatPosY[STAT_ENERGY] + 9.0f, _AspectHeight), 1.0f, ETA_CENTER, ETA_CENTER, 0.0f, Color, BackColor, strEnergy);

	font = FONT_NORMAL;

	mu_sprintf_s(strEnergy, mu_countof(strEnergy), GlobalText[213], (mu_int32)CharacterAttribute->Mana, (mu_int32)CharacterAttribute->ManaMax);

	BackColor = MU::MakeARGB(0, 0, 0, 0);
	Color = MU::MakeARGB(255, 255, 255, 255);

	if (g_isCharacterBuff((&Hero->Object), eBuff_Hellowin5))
	{
		Color = MU::MakeARGB(255, 0, 240, 255);
	}

	if (g_isCharacterBuff((&Hero->Object), eBuff_EliteScroll6))
	{
		Color = MU::MakeARGB(255, 0, 240, 255);
	}

	if (g_isCharacterBuff((&Hero->Object), eBuff_CherryBlossom_Liguor))
	{
		Color = MU::MakeARGB(255, 0, 240, 255);
	}

	iY = StatPosY[STAT_ENERGY] + 27.0f;
	font->DrawStringC2D(TEXTINDEX::CHARACTERINFO_MANA, MU_TO_WINDOW(CHARACTERINFO_POSX + 18.0f, _AspectWidth), MU_TO_WINDOW(CHARACTERINFO_POSY + iY, _AspectHeight), 1.0f, ETA_LEFT, ETA_CENTER, 0.0f, Color, BackColor, strEnergy);

	if (iBaseClass == GAME::CLASS_WIZARD || iBaseClass == GAME::CLASS_DARK || iBaseClass == GAME::CLASS_SUMMONER)
	{
		mu_int32 iMagicDamageMin;
		mu_int32 iMagicDamageMax;

		CharacterMachine->GetMagicSkillDamage(CharacterAttribute->Skill[Hero->CurrentSkill], &iMagicDamageMin, &iMagicDamageMax);

		mu_int32 iMagicDamageMaxInitial = iMagicDamageMax;

		iMagicDamageMin += Add_Mana_Min;
		iMagicDamageMax += Add_Mana_Max;

		if (CharacterAttribute->Ability & ABILITY_PLUS_DAMAGE)
		{
			iMagicDamageMin += 10;
			iMagicDamageMax += 10;
		}

		mu_int32 maxMg = 0;

		for (mu_int32 j = 0; j < GAME::MAX_EQUIPMENT; ++j)
		{
			mu_int32 TempLevel = CharacterMachine->Equipment[j].Level;

			if (TempLevel >= CharacterMachine->Equipment[j].HarmonyOptionLevel)
			{
				StrengthenCapability SC;

				g_pUIJewelHarmonyinfo.GetStrengthenCapability(&SC, &CharacterMachine->Equipment[j], 1);

				if (SC.SI_isSP)
				{
					maxMg += SC.SI_SP.SI_magicalpower;
				}
			}
		}

		pItemRingLeft = &CharacterMachine->Equipment[GAME::EQUIPMENT_RING_LEFT];
		pItemRingRight = &CharacterMachine->Equipment[GAME::EQUIPMENT_RING_RIGHT];

		mu_int32 iNonExpiredLRingType = -1;
		mu_int32 iNonExpiredRRingType = -1;
		iNonExpiredLRingType = pItemRingLeft->Type;
		iNonExpiredRRingType = pItemRingRight->Type;

		mu_int32 maxIMagicDamageMin = 0;
		mu_int32 maxIMagicDamageMax = 0;

		if (iNonExpiredLRingType == ITEM_HELPER + 41 || iNonExpiredRRingType == ITEM_HELPER + 41)
		{
			maxIMagicDamageMin = EMath::Max(maxIMagicDamageMin, 20);
			maxIMagicDamageMax = EMath::Max(maxIMagicDamageMax, 20);
		}

		if (iNonExpiredLRingType == ITEM_HELPER + 76 || iNonExpiredRRingType == ITEM_HELPER + 76)
		{
			maxIMagicDamageMin = EMath::Max(maxIMagicDamageMin, 30);
			maxIMagicDamageMax = EMath::Max(maxIMagicDamageMax, 30);
		}

		if (iNonExpiredLRingType == ITEM_HELPER + 122 || iNonExpiredRRingType == ITEM_HELPER + 122)
		{
			maxIMagicDamageMin = EMath::Max(maxIMagicDamageMin, 40);
			maxIMagicDamageMax = EMath::Max(maxIMagicDamageMax, 40);
		}

		iMagicDamageMin += maxIMagicDamageMin;
		iMagicDamageMax += maxIMagicDamageMax;

		pItemHelper = &CharacterMachine->Equipment[GAME::EQUIPMENT_HELPER];
		if (pItemHelper)
		{
			if (pItemHelper->Type == ITEM_HELPER + 37 && pItemHelper->ExcOption == 0x04)
			{
				mu_uint32 wLevel = CharacterAttribute->Level;

				iMagicDamageMin += (wLevel / 25);
				iMagicDamageMax += (wLevel / 25);
			}

			if (pItemHelper->Type == ITEM_HELPER + 64)
			{
				iMagicDamageMin += mu_int32(mu_float(iMagicDamageMin) * 0.4f);
				iMagicDamageMax += mu_int32(mu_float(iMagicDamageMax) * 0.4f);
			}

			if (pItemHelper->Type == ITEM_HELPER + 123)	
			{
				iMagicDamageMin += mu_int32(mu_float(iMagicDamageMin) * 0.2f);
				iMagicDamageMax += mu_int32(mu_float(iMagicDamageMax) * 0.2f);
			}

			if (pItemHelper->Type == ITEM_HELPER + 1)
			{
				iMagicDamageMin += mu_int32(mu_float(iMagicDamageMin) * 0.3f);
				iMagicDamageMax += mu_int32(mu_float(iMagicDamageMax) * 0.3f);
			}
		}

		if (g_isCharacterBuff((&Hero->Object), eBuff_Berserker))
		{
			mu_int32 nTemp = CharacterAttribute->Energy + CharacterAttribute->AddEnergy;
			mu_float fTemp = mu_int32(CharacterAttribute->Energy / 30) / 100.0f;
			iMagicDamageMin += nTemp / 9 * fTemp;
			iMagicDamageMax += nTemp / 4 * fTemp;
		}

		if ((pWeaponRight->Type >= ITEM_STAFF
			&& pWeaponRight->Type < (ITEM_SHIELD))
			|| pWeaponRight->Type == (ITEM_SWORD + 31)
			|| pWeaponRight->Type == (ITEM_SWORD + 23)
			|| pWeaponRight->Type == (ITEM_SWORD + 25)
			|| pWeaponRight->Type == (ITEM_SWORD + 21)
			|| pWeaponRight->Type == (ITEM_SWORD + 28)
			)
		{
			mu_float magicPercent = (mu_float)(pWeaponRight->MagicPower) / 100;

			ITEM_ATTRIBUTE *p = &ItemAttribute[pWeaponRight->Type];
			mu_float   percent = CalcDurabilityPercent(pWeaponRight->Durability, p->MagicDur, pWeaponRight->Level, pWeaponRight->ExcOption, pWeaponRight->SetOption);

			magicPercent = magicPercent - magicPercent*percent;

			mu_sprintf_s(strEnergy, mu_countof(strEnergy), GlobalText[215],
				iMagicDamageMin + maxMg,
				iMagicDamageMax + maxMg,
				(mu_int32)((iMagicDamageMaxInitial + maxMg)*magicPercent));
		}
		else
		{
			mu_sprintf_s(strEnergy, mu_countof(strEnergy), GlobalText[216],
				iMagicDamageMin + maxMg,
				iMagicDamageMax + maxMg);
		}

		iY += 13.0f;
		BackColor = MU::MakeARGB(0, 0, 0, 0);
		Color = MU::MakeARGB(255, 255, 255, 255);

		if (g_isCharacterBuff((&Hero->Object), eBuff_Hellowin2))
		{
			Color = MU::MakeARGB(255, 0, 240, 255);
		}

		if (g_isCharacterBuff((&Hero->Object), eBuff_EliteScroll4))
		{
			Color = MU::MakeARGB(255, 0, 240, 255);
		}

		if (g_isCharacterBuff((&Hero->Object), eBuff_CherryBlossom_Petal))
		{
			Color = MU::MakeARGB(255, 0, 240, 255);
		}

		font->DrawStringC2D(TEXTINDEX::CHARACTERINFO_MAGICDAMAGE, MU_TO_WINDOW(CHARACTERINFO_POSX + 18.0f, _AspectWidth), MU_TO_WINDOW(CHARACTERINFO_POSY + iY, _AspectHeight), 1.0f, ETA_LEFT, ETA_CENTER, 0.0f, Color, BackColor, strEnergy);
	}

	if (iBaseClass == GAME::CLASS_SUMMONER)
	{
		mu_int32 iCurseDamageMin = 0;
		mu_int32 iCurseDamageMax = 0;

		CharacterMachine->GetCurseSkillDamage(CharacterAttribute->Skill[Hero->CurrentSkill], &iCurseDamageMin, &iCurseDamageMax);

		if (g_isCharacterBuff((&Hero->Object), eBuff_Berserker))
		{
			mu_int32 nTemp = CharacterAttribute->Energy + CharacterAttribute->AddEnergy;
			mu_float fTemp = mu_int32(CharacterAttribute->Energy / 30) / 100.0f;
			iCurseDamageMin += nTemp / 9 * fTemp;
			iCurseDamageMax += nTemp / 4 * fTemp;
		}

		mu_int32 iNonExpiredLRingType = -1;
		mu_int32 iNonExpiredRRingType = -1;
		iNonExpiredLRingType = pItemRingLeft->Type;
		iNonExpiredRRingType = pItemRingRight->Type;

		mu_int32 maxICurseDamageMin = 0;
		mu_int32 maxICurseDamageMax = 0;

		if (iNonExpiredLRingType == ITEM_HELPER + 76 || iNonExpiredRRingType == ITEM_HELPER + 76)
		{
			maxICurseDamageMin = EMath::Max(maxICurseDamageMin, 30);
			maxICurseDamageMax = EMath::Max(maxICurseDamageMax, 30);
		}

		if (iNonExpiredLRingType == ITEM_HELPER + 122 || iNonExpiredRRingType == ITEM_HELPER + 122)
		{
			maxICurseDamageMin = EMath::Max(maxICurseDamageMin, 40);
			maxICurseDamageMax = EMath::Max(maxICurseDamageMax, 40);
		}

		iCurseDamageMin += maxICurseDamageMin;
		iCurseDamageMax += maxICurseDamageMax;

		pItemHelper = &CharacterMachine->Equipment[GAME::EQUIPMENT_HELPER];
		if (pItemHelper)
		{
			if (pItemHelper->Type == ITEM_HELPER + 64)
			{
				iCurseDamageMin += mu_int32(mu_float(iCurseDamageMin) * 0.4f);
				iCurseDamageMax += mu_int32(mu_float(iCurseDamageMax) * 0.4f);
			}

			if (pItemHelper->Type == ITEM_HELPER + 123)
			{
				iCurseDamageMin += mu_int32(mu_float(iCurseDamageMin) * 0.2f);
				iCurseDamageMax += mu_int32(mu_float(iCurseDamageMax) * 0.2f);
			}
		}

		if (ITEM_STAFF + 21 <= pWeaponLeft->Type && pWeaponLeft->Type <= ITEM_STAFF + 29)
		{
			mu_float fCursePercent = (mu_float)(pWeaponLeft->MagicPower) / 100;

			ITEM_ATTRIBUTE *p = &ItemAttribute[pWeaponLeft->Type];
			mu_float fPercent = CalcDurabilityPercent(pWeaponLeft->Durability,
				p->MagicDur, pWeaponLeft->Level, pWeaponLeft->ExcOption,
				pWeaponLeft->SetOption);

			fCursePercent -= fCursePercent*fPercent;

			mu_sprintf_s(strEnergy, mu_countof(strEnergy), GlobalText[1693],
				iCurseDamageMin, iCurseDamageMax,
				(mu_int32)((iCurseDamageMax)*fCursePercent));
		}
		else
		{
			mu_sprintf_s(strEnergy, mu_countof(strEnergy), GlobalText[1694],
				iCurseDamageMin, iCurseDamageMax);
		}

		iY += 13.0f;
		font->DrawStringC2D(TEXTINDEX::CHARACTERINFO_CURSEDAMAGE, MU_TO_WINDOW(CHARACTERINFO_POSX + 18.0f, _AspectWidth), MU_TO_WINDOW(CHARACTERINFO_POSY + iY, _AspectHeight), 1.0f, ETA_LEFT, ETA_CENTER, 0.0f, Color, BackColor, strEnergy);
	}

	iY += 13.0f;
	if (iBaseClass == GAME::CLASS_KNIGHT)
	{
		mu_sprintf_s(strEnergy, mu_countof(strEnergy), GlobalText[582], 200 + (wEnergy / 10));
		font->DrawStringC2D(TEXTINDEX::CHARACTERINFO_MAGICDAMAGE2, MU_TO_WINDOW(CHARACTERINFO_POSX + 20.0f, _AspectWidth), MU_TO_WINDOW(CHARACTERINFO_POSY + iY, _AspectHeight), 1.0f, ETA_LEFT, ETA_CENTER, 0.0f, Color, BackColor, strEnergy);
	}

	if (iBaseClass == GAME::CLASS_DARK)
	{
		mu_sprintf_s(strEnergy, mu_countof(strEnergy), GlobalText[582], 200);
		font->DrawStringC2D(TEXTINDEX::CHARACTERINFO_MAGICDAMAGE2, MU_TO_WINDOW(CHARACTERINFO_POSX + 20.0f, _AspectWidth), MU_TO_WINDOW(CHARACTERINFO_POSY + iY, _AspectHeight), 1.0f, ETA_LEFT, ETA_CENTER, 0.0f, Color, BackColor, strEnergy);
	}
	if (iBaseClass == GAME::CLASS_DARK_LORD)
	{
		mu_sprintf_s(strEnergy, mu_countof(strEnergy), GlobalText[582], 200 + (wEnergy / 20));
		font->DrawStringC2D(TEXTINDEX::CHARACTERINFO_MAGICDAMAGE2, MU_TO_WINDOW(CHARACTERINFO_POSX + 20.0f, _AspectWidth), MU_TO_WINDOW(CHARACTERINFO_POSY + iY, _AspectHeight), 1.0f, ETA_LEFT, ETA_CENTER, 0.0f, Color, BackColor, strEnergy);
	}

	if (iBaseClass == GAME::CLASS_DARK_LORD)
	{
		font = FONT_BOLD;
		BackColor = MU::MakeARGB(0, 0, 0, 0);

		mu_uint32 wCharisma = CharacterAttribute->Charisma + CharacterAttribute->AddCharisma;

		if (g_isCharacterBuff((&Hero->Object), eBuff_SecretPotion5))
		{
			Color = MU::MakeARGB(255, 120, 0, 255);
		}
		else if (CharacterAttribute->AddCharisma)
		{
			Color = MU::MakeARGB(100, 150, 255, 255);
		}
		else
		{
			Color = MU::MakeARGB(230, 230, 0, 255);
		}

		mu_text strCharisma[256];
		mu_sprintf_s(strCharisma, mu_countof(strCharisma), _T("%d"), wCharisma);

		font->DrawStringC2D(TEXTINDEX::CHARACTERINFO_COMMAND_LABEL, MU_TO_WINDOW(CHARACTERINFO_POSX + 12.0f + 37.0f, _AspectWidth), MU_TO_WINDOW(CHARACTERINFO_POSY + StatPosY[STAT_COMMAND] + 9.0f, _AspectHeight), 1.0f, ETA_CENTER, ETA_CENTER, 0.0f, Color, BackColor, GlobalText[1900]);
		font->DrawStringC2D(TEXTINDEX::CHARACTERINFO_COMMAND, MU_TO_WINDOW(CHARACTERINFO_POSX + 86.0f + 43.0f, _AspectWidth), MU_TO_WINDOW(CHARACTERINFO_POSY + StatPosY[STAT_COMMAND] + 9.0f, _AspectHeight), 1.0f, ETA_CENTER, ETA_CENTER, 0.0f, Color, BackColor, strCharisma);
	}
}

UIBase *UICharacterInfo::GetFocus(mu_int32 mouseX, mu_int32 mouseY)
{
	UIBase *focus = UIBase::GetFocus(mouseX, mouseY);

	if(focus != nullptr)
	{
		mu_uint32 maxIndex = GetBaseClass(Hero->Class) == GAME::CLASS_DARK_LORD ? STAT_MAX : STAT_COMMAND;
		for (mu_uint32 index = 0; index < maxIndex; ++index)
		{
			UIBase *object = _StatButton[index]->GetFocus(mouseX, mouseY);

			if (object != nullptr)
			{
				return object;
			}
		}

		UIButton *buttons[] = { _XButton, _CloseButton, _QuestButton, _PetButton, _MasterButton };

		for (mu_uint32 index = 0; index < mu_countof(buttons); ++index)
		{
			UIBase *object = buttons[index]->GetFocus(mouseX, mouseY);

			if (object != nullptr)
			{
				return object;
			}
		}
	}

	return focus;
}

void UICharacterInfo::Configure()
{
	ResetEquipmentLevel();

	if (IsMasterLevel(Hero->Class) == false)
	{
		_MasterButton->ChangeState(UIButton::UI_DISABLED);
	}
	else
	{
		_MasterButton->ChangeState(UIButton::UI_NORMAL);
	}

	g_csItemOption.init();

	if (CharacterMachine->IsZeroDurability())
	{
		CharacterMachine->CalculateAll();
	}

#if 0 // FIX
	if (g_QuestMng.IsIndexInCurQuestIndexList(0x10009))
	{
		if (g_QuestMng.IsEPRequestRewardState(0x10009))
		{
			g_pMyQuestInfoWindow->UnselectQuestList();
			SendSatisfyQuestRequestFromClient(0x10009);
			g_QuestMng.SetEPRequestRewardState(0x10009, false);
		}
	}
#endif
}

void UICharacterInfo::ResetEquipmentLevel()
{
	ITEM *pItem = CharacterMachine->Equipment;
	Hero->Weapon[0].Level = pItem[GAME::EQUIPMENT_WEAPON_RIGHT].Level;
	Hero->Weapon[1].Level = pItem[GAME::EQUIPMENT_WEAPON_LEFT].Level;
	Hero->BodyPart[BODYPART_HELM].Level = pItem[GAME::EQUIPMENT_HELM].Level;
	Hero->BodyPart[BODYPART_ARMOR].Level = pItem[GAME::EQUIPMENT_ARMOR].Level;
	Hero->BodyPart[BODYPART_PANTS].Level = pItem[GAME::EQUIPMENT_PANTS].Level;
	Hero->BodyPart[BODYPART_GLOVES].Level = pItem[GAME::EQUIPMENT_GLOVES].Level;
	Hero->BodyPart[BODYPART_BOOTS].Level = pItem[GAME::EQUIPMENT_BOOTS].Level;

	CheckFullSet(0, Hero);
}

void UICharacterInfo::EnableStatButtons()
{
	mu_uint32 maxIndex = GetBaseClass(Hero->Class) == GAME::CLASS_DARK_LORD ? STAT_MAX : STAT_COMMAND;
	for (mu_uint32 index = 0; index < maxIndex; ++index)
	{
		_StatButton[index]->UnmarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
	}
}

void UICharacterInfo::DisableStatButtons()
{
	mu_uint32 maxIndex = GetBaseClass(Hero->Class) == GAME::CLASS_DARK_LORD ? STAT_MAX : STAT_COMMAND;
	for (mu_uint32 index = 0; index < maxIndex; ++index)
	{
		_StatButton[index]->MarkGfxFlag(GFX_FLAG::GFX_HIDDEN);
	}
}