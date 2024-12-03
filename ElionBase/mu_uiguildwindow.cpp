#include "stdafx.h"
#include "mu_uiguildwindow.h"
#include "mu_uiguildwindow_defines.h"

UIGuildWindow::UIGuildWindow(UISceneManager *manager) : UIBase()
{
	if (manager != nullptr)
	{
		manager->AddGFxObject(this);
	}

	_SceneManager = manager;

	UIBase::SetAspectRatio(SCREEN_WIDTH, SCREEN_HEIGHT);
	UIBase::SetPosition(GUILDWINDOW_POSX, GUILDWINDOW_POSY);
	UIBase::SetDimensions(GUILDWINDOW_WIDTH, GUILDWINDOW_HEIGHT);

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

		_XButton = new UIButton(GAMEUI::MESSAGE_GUILDWINDOW_CLOSE_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, XBUTTON_POSX, XBUTTON_POSY, XBUTTON_WIDTH, XBUTTON_HEIGHT);

		_CloseButton = new UIButton(GAMEUI::MESSAGE_GUILDWINDOW_CLOSE_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, GUILDWINDOW_POSX + 13.0f, GUILDWINDOW_POSY + 391.0f, 36.0f, 29.0f);
		_CloseButton->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_CloseButton->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EXIT_BUTTON], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(100, 100, 100, 255)));
		_CloseButton->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EXIT_BUTTON], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_CloseButton->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EXIT_BUTTON], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));
		_CloseButton->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EXIT_BUTTON], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		mu_text tipBuffer[256];
		mu_sprintf_s(tipBuffer, mu_countof(tipBuffer), GlobalText[927], _T("G")); 
		_CloseButton->SetTooltip(FONT_NORMAL, tipBuffer, MU::MakeARGB(255, 255, 255, 255), MU::MakeARGB(0, 0, 0, 180), true);

		_TabBackground = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::GUILD_BTN_TAB1], SCREEN_WIDTH, SCREEN_HEIGHT, TABBACK_POSX, TABBACK_POSY, TABBACK_WIDTH, TABBACK_HEIGHT);
		_TabBackground->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

		mu_uint32 tabsText[MAX_TAB] =
		{
			180,
			1330,
			1352,
		};

		for (mu_uint32 n = 0; n < MAX_TAB; ++n)
		{
			_TabButtons[n] = new UIRadioButton(GAMEUI::MESSAGE_GUILDWINDOW_TAB_BEGIN + n, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, TABBUTTON_POSX(n), TABBUTTON_POSY, TABBUTTON_WIDTH, TABBUTTON_HEIGHT);
			
			_TabButtons[n]->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
			_TabButtons[n]->SetStateData(UIRadioButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::GUILD_BTN_TAB2], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
			_TabButtons[n]->SetStateData(UIRadioButton::UI_PRESSED_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::GUILD_BTN_TAB2], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
			
			_TabButtons[n]->SetStateText(UIRadioButton::UI_DISABLED, FONT_NORMAL, GlobalText[tabsText[n]], MU::MakeARGB(100, 100, 100, 255));
			_TabButtons[n]->SetStateText(UIRadioButton::UI_NORMAL, FONT_NORMAL, GlobalText[tabsText[n]], MU::MakeARGB(150, 150, 150, 255));
			_TabButtons[n]->SetStateText(UIRadioButton::UI_OVER, FONT_NORMAL, GlobalText[tabsText[n]], MU::MakeARGB(150, 150, 150, 255));
			_TabButtons[n]->SetStateText(UIRadioButton::UI_PRESSED, FONT_NORMAL, GlobalText[tabsText[n]], MU::MakeARGB(255, 255, 255, 255));
			_TabButtons[n]->SetStateText(UIRadioButton::UI_PRESSED_OVER, FONT_NORMAL, GlobalText[tabsText[n]], MU::MakeARGB(255, 255, 255, 255));
		}

		// Guild Tab
		// Mark Box
		_MarkTopLine = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::TABLE_TOP_LINE], SCREEN_WIDTH, SCREEN_HEIGHT, MARKBOX_TLINE_POSX, MARKBOX_TLINE_POSY, MARKBOX_TLINE_WIDTH, MARKBOX_TLINE_HEIGHT);
		_MarkTopLine->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_MarkBottomLine = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::TABLE_BOTTOM_LINE], SCREEN_WIDTH, SCREEN_HEIGHT, MARKBOX_BLINE_POSX, MARKBOX_BLINE_POSY, MARKBOX_BLINE_WIDTH, MARKBOX_BLINE_HEIGHT);
		_MarkBottomLine->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_MarkLeftLine = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::TABLE_LEFT_LINE], SCREEN_WIDTH, SCREEN_HEIGHT, MARKBOX_LLINE_POSX, MARKBOX_LLINE_POSY, MARKBOX_LLINE_WIDTH, MARKBOX_LLINE_HEIGHT);
		_MarkLeftLine->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_MarkRightLine = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::TABLE_RIGHT_LINE], SCREEN_WIDTH, SCREEN_HEIGHT, MARKBOX_RLINE_POSX, MARKBOX_RLINE_POSY, MARKBOX_RLINE_WIDTH, MARKBOX_RLINE_HEIGHT);
		_MarkRightLine->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_MarkTopLeft = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::TABLE_TOP_LEFT], SCREEN_WIDTH, SCREEN_HEIGHT, MARKBOX_TL_POSX, MARKBOX_TL_POSY, MARKBOX_TL_WIDTH, MARKBOX_TL_HEIGHT);
		_MarkTopLeft->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_MarkTopRight = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::TABLE_TOP_RIGHT], SCREEN_WIDTH, SCREEN_HEIGHT, MARKBOX_TR_POSX, MARKBOX_TR_POSY, MARKBOX_TR_WIDTH, MARKBOX_TR_HEIGHT);
		_MarkTopRight->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_MarkBottomLeft = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::TABLE_BOTTOM_LEFT], SCREEN_WIDTH, SCREEN_HEIGHT, MARKBOX_BL_POSX, MARKBOX_BL_POSY, MARKBOX_BL_WIDTH, MARKBOX_BL_HEIGHT);
		_MarkBottomLeft->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_MarkBottomRight = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::TABLE_BOTTOM_RIGHT], SCREEN_WIDTH, SCREEN_HEIGHT, MARKBOX_BR_POSX, MARKBOX_BR_POSY, MARKBOX_BR_WIDTH, MARKBOX_BR_HEIGHT);
		_MarkBottomRight->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_MarkBox = new UISprite(manager, this, &TEXTURES::Data[BITMAP_GUILD_BEGIN], SCREEN_WIDTH, SCREEN_HEIGHT, MARKBOX_POSX, MARKBOX_POSY, MARKBOX_WIDTH, MARKBOX_HEIGHT);
		_MarkBox->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_MarkBox->SetSampler(ESampler::eUcVcWcMinNMagNMipN);

		// Info Box
		_InfoTopLine = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::TABLE_TOP_LINE], SCREEN_WIDTH, SCREEN_HEIGHT, INFOBOX_TLINE_POSX, INFOBOX_TLINE_POSY, INFOBOX_TLINE_WIDTH, INFOBOX_TLINE_HEIGHT);
		_InfoTopLine->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_InfoBottomLine = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::TABLE_BOTTOM_LINE], SCREEN_WIDTH, SCREEN_HEIGHT, INFOBOX_BLINE_POSX, INFOBOX_BLINE_POSY, INFOBOX_BLINE_WIDTH, INFOBOX_BLINE_HEIGHT);
		_InfoBottomLine->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_InfoLeftLine = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::TABLE_LEFT_LINE], SCREEN_WIDTH, SCREEN_HEIGHT, INFOBOX_LLINE_POSX, INFOBOX_LLINE_POSY, INFOBOX_LLINE_WIDTH, INFOBOX_LLINE_HEIGHT);
		_InfoLeftLine->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_InfoRightLine = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::TABLE_RIGHT_LINE], SCREEN_WIDTH, SCREEN_HEIGHT, INFOBOX_RLINE_POSX, INFOBOX_RLINE_POSY, INFOBOX_RLINE_WIDTH, INFOBOX_RLINE_HEIGHT);
		_InfoRightLine->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_InfoTopLeft = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::TABLE_TOP_LEFT], SCREEN_WIDTH, SCREEN_HEIGHT, INFOBOX_TL_POSX, INFOBOX_TL_POSY, INFOBOX_TL_WIDTH, INFOBOX_TL_HEIGHT);
		_InfoTopLeft->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_InfoTopRight = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::TABLE_TOP_RIGHT], SCREEN_WIDTH, SCREEN_HEIGHT, INFOBOX_TR_POSX, INFOBOX_TR_POSY, INFOBOX_TR_WIDTH, INFOBOX_TR_HEIGHT);
		_InfoTopRight->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_InfoBottomLeft = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::TABLE_BOTTOM_LEFT], SCREEN_WIDTH, SCREEN_HEIGHT, INFOBOX_BL_POSX, INFOBOX_BL_POSY, INFOBOX_BL_WIDTH, INFOBOX_BL_HEIGHT);
		_InfoBottomLeft->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_InfoBottomRight = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::TABLE_BOTTOM_RIGHT], SCREEN_WIDTH, SCREEN_HEIGHT, INFOBOX_BR_POSX, INFOBOX_BR_POSY, INFOBOX_BR_WIDTH, INFOBOX_BR_HEIGHT);
		_InfoBottomRight->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		
		// Announcement Box
		_AnnBack = new UISpriteColor(manager, this, MU::MakeARGB(0, 0, 0, 255), SCREEN_WIDTH, SCREEN_HEIGHT, ANNBOX_BACK_POSX, ANNBOX_BACK_POSY, ANNBOX_BACK_WIDTH, ANNBOX_BACK_HEIGHT);
		_AnnTopLine = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::TABLE_TOP_LINE], SCREEN_WIDTH, SCREEN_HEIGHT, ANNBOX_TLINE_POSX, ANNBOX_TLINE_POSY, ANNBOX_TLINE_WIDTH, ANNBOX_TLINE_HEIGHT);
		_AnnTopLine->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_AnnBottomLine = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::TABLE_BOTTOM_LINE], SCREEN_WIDTH, SCREEN_HEIGHT, ANNBOX_BLINE_POSX, ANNBOX_BLINE_POSY, ANNBOX_BLINE_WIDTH, ANNBOX_BLINE_HEIGHT);
		_AnnBottomLine->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_AnnLeftLine = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::TABLE_LEFT_LINE], SCREEN_WIDTH, SCREEN_HEIGHT, ANNBOX_LLINE_POSX, ANNBOX_LLINE_POSY, ANNBOX_LLINE_WIDTH, ANNBOX_LLINE_HEIGHT);
		_AnnLeftLine->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_AnnRightLine = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::TABLE_RIGHT_LINE], SCREEN_WIDTH, SCREEN_HEIGHT, ANNBOX_RLINE_POSX, ANNBOX_RLINE_POSY, ANNBOX_RLINE_WIDTH, ANNBOX_RLINE_HEIGHT);
		_AnnRightLine->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_AnnTab = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::GUILD_BTN_TAB3], SCREEN_WIDTH, SCREEN_HEIGHT, ANNBOX_TAB_POSX, ANNBOX_TAB_POSY, ANNBOX_TAB_WIDTH, ANNBOX_TAB_HEIGHT);
		_AnnTab->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_AnnTab->SetStateText(FONT_NORMAL, GlobalText[1323], MU::MakeARGB(255, 185, 1, 255), MU::MakeARGB(0, 0, 0, 0), -1.5f);
		_AnnTopRight = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::TABLE_TOP_RIGHT], SCREEN_WIDTH, SCREEN_HEIGHT, ANNBOX_TR_POSX, ANNBOX_TR_POSY, ANNBOX_TR_WIDTH, ANNBOX_TR_HEIGHT);
		_AnnTopRight->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_AnnBottomLeft = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::TABLE_BOTTOM_LEFT], SCREEN_WIDTH, SCREEN_HEIGHT, ANNBOX_BL_POSX, ANNBOX_BL_POSY, ANNBOX_BL_WIDTH, ANNBOX_BL_HEIGHT);
		_AnnBottomLeft->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_AnnBottomRight = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::TABLE_BOTTOM_RIGHT], SCREEN_WIDTH, SCREEN_HEIGHT, ANNBOX_BR_POSX, ANNBOX_BR_POSY, ANNBOX_BR_WIDTH, ANNBOX_BR_HEIGHT);
		_AnnBottomRight->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

		_AnnScrollBar = new UIScrollBar(GAMEUI::MESSAGE_GUILDWINDOW_ANN_SCROLLBAR, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, ANNSCROLLBAR_POSX, ANNSCROLLBAR_POSY, UIScrollBar::ORI_VERTICAL);
		_AnnScrollBar->ConfigureTop(&INTERFACES::Data[INTERFACES::SCROLLBAR_TOP], EBlendState::eAlphaSrc_InvAlphaSrc, ANNSCROLLBAR_BACKWIDTH, ANNSCROLLBAR_TOP_BOTTOM_PART_HEIGHT);
		_AnnScrollBar->ConfigureMiddle(&INTERFACES::Data[INTERFACES::SCROLLBAR_MIDDLE], EBlendState::eAlphaSrc_InvAlphaSrc, ANNSCROLLBAR_BACKWIDTH, ANNSCROLLBAR_LINESHEIGHT);
		_AnnScrollBar->ConfigureBottom(&INTERFACES::Data[INTERFACES::SCROLLBAR_BOTTOM], EBlendState::eAlphaSrc_InvAlphaSrc, ANNSCROLLBAR_BACKWIDTH, ANNSCROLLBAR_TOP_BOTTOM_PART_HEIGHT);
		_AnnScrollBar->ConfigureScroll(EBlendState::eAlphaSrc_InvAlphaSrc, ANNSCROLLBAR_BTN_WIDTH, ANNSCROLLBAR_BTN_HEIGHT, true);

		_AnnScrollBar->SetStateData(UIScrollBar::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::SCROLLBAR_SLIDE_OFF], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_AnnScrollBar->SetStateData(UIScrollBar::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::SCROLLBAR_SLIDE_ON], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_AnnScrollBar->SetStateData(UIScrollBar::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::SCROLLBAR_SLIDE_ON], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_AnnScrollBar->SetStateData(UIScrollBar::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::SCROLLBAR_SLIDE_ON], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(178, 178, 178, 255)));

		_AnnScrollBar->ConfigureScrollInfo(0.0f, 1.0f, 0.0f);
		_AnnScrollBar->ConfigureScrollDimension(ANNSCROLLBAR_LINESHEIGHT);
		_AnnScrollBar->ChangeState(UIScrollBar::UI_DISABLED);

		_AnnScrollBar->ApplyChanges();

		_AnnTextBox = new UIMultiLineEditBox(GAMEUI::MESSAGE_GUILDWINDOW_ANN_TEXTBOX, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, ANNTEXTBOX_POSX, ANNTEXTBOX_POSY, ANNTEXTBOX_WIDTH, ANNTEXTBOX_HEIGHT, _AnnScrollBar);
		_AnnTextBox->SetTextColor(230, 220, 200, 255);
		_AnnTextBox->SetBackColor(0, 0, 0, 0);
		_AnnTextBox->SetHighlightTextColor(0, 0, 0, 255);
		_AnnTextBox->SetHighlightColor(255, 230, 210, 255);
		//_AnnTextBox->SetReadOnly(true);
		_AnnTextBox->SetWrapMode(true);

		_LeaveGuildBtn = new UIButton(GAMEUI::MESSAGE_GUILDWINDOW_LEAVEGUILD_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, LEAVEGUILD_POSX, LEAVEGUILD_POSY, BUTTON_WIDTH, BUTTON_HEIGHT);
		_LeaveGuildBtn->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_LeaveGuildBtn->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTY_BUTTON_SMALL], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(100, 100, 100, 255)));
		_LeaveGuildBtn->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTY_BUTTON_SMALL], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(255, 255, 255, 255)));
		_LeaveGuildBtn->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTY_BUTTON_SMALL], TUV_THREE_OVER_BEGIN, TUV_THREE_OVER_END, MU::MakeARGB(255, 255, 255, 255)));
		_LeaveGuildBtn->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTY_BUTTON_SMALL], TUV_THREE_PRESSED_BEGIN, TUV_THREE_PRESSED_END, MU::MakeARGB(255, 255, 255, 255)));
		_LeaveGuildBtn->SetStateText(UIButton::UI_DISABLED, FONT_NORMAL, GlobalText[188], MU::MakeARGB(100, 100, 100, 255));
		_LeaveGuildBtn->SetStateText(UIButton::UI_NORMAL, FONT_NORMAL, GlobalText[188], MU::MakeARGB(200, 200, 200, 255));
		_LeaveGuildBtn->SetStateText(UIButton::UI_OVER, FONT_NORMAL, GlobalText[188], MU::MakeARGB(255, 255, 255, 255));
		_LeaveGuildBtn->SetStateText(UIButton::UI_PRESSED, FONT_NORMAL, GlobalText[188], MU::MakeARGB(150, 150, 150, 255));

		// Members Box
		_MemberColorABox = new UISpriteColor(manager, this, MU::MakeARGB(21, 21, 21, 150), SCREEN_WIDTH, SCREEN_HEIGHT, MEMBERBOX_COLORA_POSX, MEMBERBOX_COLORA_POSY, MEMBERBOX_COLORA_WIDTH, MEMBERBOX_COLORA_HEIGHT);
		_MemberColorABox->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_MemberColorBBox = new UISpriteColor(manager, this, MU::MakeARGB(59, 59, 59, 150), SCREEN_WIDTH, SCREEN_HEIGHT, MEMBERBOX_COLORB_POSX, MEMBERBOX_COLORB_POSY, MEMBERBOX_COLORB_WIDTH, MEMBERBOX_COLORB_HEIGHT);
		_MemberColorBBox->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_MemberTopLine = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::TABLE_TOP_LINE], SCREEN_WIDTH, SCREEN_HEIGHT, MEMBERBOX_TLINE_POSX, MEMBERBOX_TLINE_POSY, MEMBERBOX_TLINE_WIDTH, MEMBERBOX_TLINE_HEIGHT);
		_MemberTopLine->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_MemberBottomLine = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::TABLE_BOTTOM_LINE], SCREEN_WIDTH, SCREEN_HEIGHT, MEMBERBOX_BLINE_POSX, MEMBERBOX_BLINE_POSY, MEMBERBOX_BLINE_WIDTH, MEMBERBOX_BLINE_HEIGHT);
		_MemberBottomLine->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_MemberLeftLine = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::TABLE_LEFT_LINE], SCREEN_WIDTH, SCREEN_HEIGHT, MEMBERBOX_LLINE_POSX, MEMBERBOX_LLINE_POSY, MEMBERBOX_LLINE_WIDTH, MEMBERBOX_LLINE_HEIGHT);
		_MemberLeftLine->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_MemberRightLine = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::TABLE_RIGHT_LINE], SCREEN_WIDTH, SCREEN_HEIGHT, MEMBERBOX_RLINE_POSX, MEMBERBOX_RLINE_POSY, MEMBERBOX_RLINE_WIDTH, MEMBERBOX_RLINE_HEIGHT);
		_MemberRightLine->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_MemberTopLeft = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::TABLE_TOP_LEFT], SCREEN_WIDTH, SCREEN_HEIGHT, MEMBERBOX_TL_POSX, MEMBERBOX_TL_POSY, MEMBERBOX_TL_WIDTH, MEMBERBOX_TL_HEIGHT);
		_MemberTopLeft->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_MemberTopRight = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::TABLE_TOP_RIGHT], SCREEN_WIDTH, SCREEN_HEIGHT, MEMBERBOX_TR_POSX, MEMBERBOX_TR_POSY, MEMBERBOX_TR_WIDTH, MEMBERBOX_TR_HEIGHT);
		_MemberTopRight->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_MemberBottomLeft = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::TABLE_BOTTOM_LEFT], SCREEN_WIDTH, SCREEN_HEIGHT, MEMBERBOX_BL_POSX, MEMBERBOX_BL_POSY, MEMBERBOX_BL_WIDTH, MEMBERBOX_BL_HEIGHT);
		_MemberBottomLeft->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_MemberBottomRight = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::TABLE_BOTTOM_RIGHT], SCREEN_WIDTH, SCREEN_HEIGHT, MEMBERBOX_BR_POSX, MEMBERBOX_BR_POSY, MEMBERBOX_BR_WIDTH, MEMBERBOX_BR_HEIGHT);
		_MemberBottomRight->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

		_MemberScrollBar = new UIScrollBar(GAMEUI::MESSAGE_GUILDWINDOW_MEMBER_SCROLLBAR, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, MEMBERSCROLLBAR_POSX, MEMBERSCROLLBAR_POSY, UIScrollBar::ORI_VERTICAL);
		_MemberScrollBar->ConfigureTop(&INTERFACES::Data[INTERFACES::SCROLLBAR_TOP], EBlendState::eAlphaSrc_InvAlphaSrc, MEMBERSCROLLBAR_BACKWIDTH, MEMBERSCROLLBAR_TOP_BOTTOM_PART_HEIGHT);
		_MemberScrollBar->ConfigureMiddle(&INTERFACES::Data[INTERFACES::SCROLLBAR_MIDDLE], EBlendState::eAlphaSrc_InvAlphaSrc, MEMBERSCROLLBAR_BACKWIDTH, MEMBERSCROLLBAR_LINESHEIGHT);
		_MemberScrollBar->ConfigureBottom(&INTERFACES::Data[INTERFACES::SCROLLBAR_BOTTOM], EBlendState::eAlphaSrc_InvAlphaSrc, MEMBERSCROLLBAR_BACKWIDTH, MEMBERSCROLLBAR_TOP_BOTTOM_PART_HEIGHT);
		_MemberScrollBar->ConfigureScroll(EBlendState::eAlphaSrc_InvAlphaSrc, MEMBERSCROLLBAR_BTN_WIDTH, MEMBERSCROLLBAR_BTN_HEIGHT, true);

		_MemberScrollBar->SetStateData(UIScrollBar::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::SCROLLBAR_SLIDE_OFF], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_MemberScrollBar->SetStateData(UIScrollBar::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::SCROLLBAR_SLIDE_ON], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_MemberScrollBar->SetStateData(UIScrollBar::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::SCROLLBAR_SLIDE_ON], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_MemberScrollBar->SetStateData(UIScrollBar::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::SCROLLBAR_SLIDE_ON], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(178, 178, 178, 255)));

		_MemberScrollBar->ConfigureScrollInfo(0.0f, 1.0f, 1.0f);
		_MemberScrollBar->ConfigureScrollDimension(MEMBERSCROLLBAR_LINESHEIGHT);
		_MemberScrollBar->ChangeState(UIScrollBar::UI_DISABLED);
		_MemberScrollBar->SetMessageInheritor(this);

		_MemberScrollBar->ApplyChanges();

		for (mu_uint32 n = 0; n < MAX_MEMBER_RENDER; ++n)
		{
			_MembersListBtn[n] = new UIRadioButton(GAMEUI::MESSAGE_GUILDWINDOW_MEMBER_BUTTON_BEGIN + n, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, MEMBERBOX_ITEM_POSX, MEMBERBOX_ITEM_POSY(n), MEMBERBOX_ITEM_WIDTH, MEMBERBOX_ITEM_HEIGHT);
			_MembersListBtn[n]->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
			_MembersListBtn[n]->SetStateData(UIRadioButton::UI_NORMAL, UI_STATE_DATA(USE_COLOR_STATE, EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(0, 0, 0, 0)));
			_MembersListBtn[n]->SetStateData(UIRadioButton::UI_OVER, UI_STATE_DATA(USE_COLOR_STATE, EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(150, 150, 150, 220)));
			_MembersListBtn[n]->SetStateData(UIRadioButton::UI_PRESSED, UI_STATE_DATA(USE_COLOR_STATE, EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(150, 150, 150, 180)));
			_MembersListBtn[n]->SetStateData(UIRadioButton::UI_PRESSED_OVER, UI_STATE_DATA(USE_COLOR_STATE, EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(150, 150, 150, 220)));
		}

		_GivePositionBtn = new UIButton(GAMEUI::MESSAGE_GUILDWINDOW_GIVEPOSITION_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, GETPOSITION_POSX, GETPOSITION_POSY, BUTTON_WIDTH, BUTTON_HEIGHT);
		_GivePositionBtn->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_GivePositionBtn->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTY_BUTTON_SMALL], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(100, 100, 100, 255)));
		_GivePositionBtn->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTY_BUTTON_SMALL], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(255, 255, 255, 255)));
		_GivePositionBtn->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTY_BUTTON_SMALL], TUV_THREE_OVER_BEGIN, TUV_THREE_OVER_END, MU::MakeARGB(255, 255, 255, 255)));
		_GivePositionBtn->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTY_BUTTON_SMALL], TUV_THREE_PRESSED_BEGIN, TUV_THREE_PRESSED_END, MU::MakeARGB(255, 255, 255, 255)));
		_GivePositionBtn->SetStateText(UIButton::UI_DISABLED, FONT_NORMAL, GlobalText[1307], MU::MakeARGB(100, 100, 100, 255));
		_GivePositionBtn->SetStateText(UIButton::UI_NORMAL, FONT_NORMAL, GlobalText[1307], MU::MakeARGB(200, 200, 200, 255));
		_GivePositionBtn->SetStateText(UIButton::UI_OVER, FONT_NORMAL, GlobalText[1307], MU::MakeARGB(255, 255, 255, 255));
		_GivePositionBtn->SetStateText(UIButton::UI_PRESSED, FONT_NORMAL, GlobalText[1307], MU::MakeARGB(150, 150, 150, 255));

		_FreePositionBtn = new UIButton(GAMEUI::MESSAGE_GUILDWINDOW_FREEPOSITION_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, FREEPOSITION_POSX, FREEPOSITION_POSY, BUTTON_WIDTH, BUTTON_HEIGHT);
		_FreePositionBtn->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_FreePositionBtn->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTY_BUTTON_SMALL], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(100, 100, 100, 255)));
		_FreePositionBtn->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTY_BUTTON_SMALL], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(255, 255, 255, 255)));
		_FreePositionBtn->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTY_BUTTON_SMALL], TUV_THREE_OVER_BEGIN, TUV_THREE_OVER_END, MU::MakeARGB(255, 255, 255, 255)));
		_FreePositionBtn->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTY_BUTTON_SMALL], TUV_THREE_PRESSED_BEGIN, TUV_THREE_PRESSED_END, MU::MakeARGB(255, 255, 255, 255)));
		_FreePositionBtn->SetStateText(UIButton::UI_DISABLED, FONT_NORMAL, GlobalText[1308], MU::MakeARGB(100, 100, 100, 255));
		_FreePositionBtn->SetStateText(UIButton::UI_NORMAL, FONT_NORMAL, GlobalText[1308], MU::MakeARGB(200, 200, 200, 255));
		_FreePositionBtn->SetStateText(UIButton::UI_OVER, FONT_NORMAL, GlobalText[1308], MU::MakeARGB(255, 255, 255, 255));
		_FreePositionBtn->SetStateText(UIButton::UI_PRESSED, FONT_NORMAL, GlobalText[1308], MU::MakeARGB(150, 150, 150, 255));

		_KickPlayerBtn = new UIButton(GAMEUI::MESSAGE_GUILDWINDOW_KICKPLAYER_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, GETOUT_POSX, GETOUT_POSY, BUTTON_WIDTH, BUTTON_HEIGHT);
		_KickPlayerBtn->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_KickPlayerBtn->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTY_BUTTON_SMALL], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(100, 100, 100, 255)));
		_KickPlayerBtn->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTY_BUTTON_SMALL], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(255, 255, 255, 255)));
		_KickPlayerBtn->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTY_BUTTON_SMALL], TUV_THREE_OVER_BEGIN, TUV_THREE_OVER_END, MU::MakeARGB(255, 255, 255, 255)));
		_KickPlayerBtn->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTY_BUTTON_SMALL], TUV_THREE_PRESSED_BEGIN, TUV_THREE_PRESSED_END, MU::MakeARGB(255, 255, 255, 255)));
		_KickPlayerBtn->SetStateText(UIButton::UI_DISABLED, FONT_NORMAL, GlobalText[1309], MU::MakeARGB(100, 100, 100, 255));
		_KickPlayerBtn->SetStateText(UIButton::UI_NORMAL, FONT_NORMAL, GlobalText[1309], MU::MakeARGB(200, 200, 200, 255));
		_KickPlayerBtn->SetStateText(UIButton::UI_OVER, FONT_NORMAL, GlobalText[1309], MU::MakeARGB(255, 255, 255, 255));
		_KickPlayerBtn->SetStateText(UIButton::UI_PRESSED, FONT_NORMAL, GlobalText[1309], MU::MakeARGB(150, 150, 150, 255));

		// Alliance Box
		_AllianceColorABox = new UISpriteColor(manager, this, MU::MakeARGB(21, 21, 21, 150), SCREEN_WIDTH, SCREEN_HEIGHT, ALLIANCEBOX_COLORA_POSX, ALLIANCEBOX_COLORA_POSY, ALLIANCEBOX_COLORA_WIDTH, ALLIANCEBOX_COLORA_HEIGHT);
		_AllianceColorABox->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_AllianceColorBBox = new UISpriteColor(manager, this, MU::MakeARGB(59, 59, 59, 150), SCREEN_WIDTH, SCREEN_HEIGHT, ALLIANCEBOX_COLORB_POSX, ALLIANCEBOX_COLORB_POSY, ALLIANCEBOX_COLORB_WIDTH, ALLIANCEBOX_COLORB_HEIGHT);
		_AllianceColorBBox->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_AllianceTopLine = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::TABLE_TOP_LINE], SCREEN_WIDTH, SCREEN_HEIGHT, ALLIANCEBOX_TLINE_POSX, ALLIANCEBOX_TLINE_POSY, ALLIANCEBOX_TLINE_WIDTH, ALLIANCEBOX_TLINE_HEIGHT);
		_AllianceTopLine->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_AllianceBottomLine = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::TABLE_BOTTOM_LINE], SCREEN_WIDTH, SCREEN_HEIGHT, ALLIANCEBOX_BLINE_POSX, ALLIANCEBOX_BLINE_POSY, ALLIANCEBOX_BLINE_WIDTH, ALLIANCEBOX_BLINE_HEIGHT);
		_AllianceBottomLine->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_AllianceLeftLine = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::TABLE_LEFT_LINE], SCREEN_WIDTH, SCREEN_HEIGHT, ALLIANCEBOX_LLINE_POSX, ALLIANCEBOX_LLINE_POSY, ALLIANCEBOX_LLINE_WIDTH, ALLIANCEBOX_LLINE_HEIGHT);
		_AllianceLeftLine->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_AllianceRightLine = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::TABLE_RIGHT_LINE], SCREEN_WIDTH, SCREEN_HEIGHT, ALLIANCEBOX_RLINE_POSX, ALLIANCEBOX_RLINE_POSY, ALLIANCEBOX_RLINE_WIDTH, ALLIANCEBOX_RLINE_HEIGHT);
		_AllianceRightLine->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_AllianceTopLeft = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::TABLE_TOP_LEFT], SCREEN_WIDTH, SCREEN_HEIGHT, ALLIANCEBOX_TL_POSX, ALLIANCEBOX_TL_POSY, ALLIANCEBOX_TL_WIDTH, ALLIANCEBOX_TL_HEIGHT);
		_AllianceTopLeft->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_AllianceTopRight = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::TABLE_TOP_RIGHT], SCREEN_WIDTH, SCREEN_HEIGHT, ALLIANCEBOX_TR_POSX, ALLIANCEBOX_TR_POSY, ALLIANCEBOX_TR_WIDTH, ALLIANCEBOX_TR_HEIGHT);
		_AllianceTopRight->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_AllianceBottomLeft = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::TABLE_BOTTOM_LEFT], SCREEN_WIDTH, SCREEN_HEIGHT, ALLIANCEBOX_BL_POSX, ALLIANCEBOX_BL_POSY, ALLIANCEBOX_BL_WIDTH, ALLIANCEBOX_BL_HEIGHT);
		_AllianceBottomLeft->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_AllianceBottomRight = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::TABLE_BOTTOM_RIGHT], SCREEN_WIDTH, SCREEN_HEIGHT, ALLIANCEBOX_BR_POSX, ALLIANCEBOX_BR_POSY, ALLIANCEBOX_BR_WIDTH, ALLIANCEBOX_BR_HEIGHT);
		_AllianceBottomRight->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

		_AllianceScrollBar = new UIScrollBar(GAMEUI::MESSAGE_GUILDWINDOW_ALLIANCE_SCROLLBAR, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, ALLIANCESCROLLBAR_POSX, ALLIANCESCROLLBAR_POSY, UIScrollBar::ORI_VERTICAL);
		_AllianceScrollBar->ConfigureTop(&INTERFACES::Data[INTERFACES::SCROLLBAR_TOP], EBlendState::eAlphaSrc_InvAlphaSrc, ALLIANCESCROLLBAR_BACKWIDTH, ALLIANCESCROLLBAR_TOP_BOTTOM_PART_HEIGHT);
		_AllianceScrollBar->ConfigureMiddle(&INTERFACES::Data[INTERFACES::SCROLLBAR_MIDDLE], EBlendState::eAlphaSrc_InvAlphaSrc, ALLIANCESCROLLBAR_BACKWIDTH, ALLIANCESCROLLBAR_LINESHEIGHT);
		_AllianceScrollBar->ConfigureBottom(&INTERFACES::Data[INTERFACES::SCROLLBAR_BOTTOM], EBlendState::eAlphaSrc_InvAlphaSrc, ALLIANCESCROLLBAR_BACKWIDTH, ALLIANCESCROLLBAR_TOP_BOTTOM_PART_HEIGHT);
		_AllianceScrollBar->ConfigureScroll(EBlendState::eAlphaSrc_InvAlphaSrc, ALLIANCESCROLLBAR_BTN_WIDTH, ALLIANCESCROLLBAR_BTN_HEIGHT, true);

		_AllianceScrollBar->SetStateData(UIScrollBar::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::SCROLLBAR_SLIDE_OFF], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_AllianceScrollBar->SetStateData(UIScrollBar::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::SCROLLBAR_SLIDE_ON], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_AllianceScrollBar->SetStateData(UIScrollBar::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::SCROLLBAR_SLIDE_ON], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_AllianceScrollBar->SetStateData(UIScrollBar::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::SCROLLBAR_SLIDE_ON], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(178, 178, 178, 255)));

		_AllianceScrollBar->ConfigureScrollInfo(0.0f, 1.0f, 1.0f);
		_AllianceScrollBar->ConfigureScrollDimension(ALLIANCESCROLLBAR_LINESHEIGHT);
		_AllianceScrollBar->ChangeState(UIScrollBar::UI_DISABLED);
		_AllianceScrollBar->SetMessageInheritor(this);

		_AllianceScrollBar->ApplyChanges();

		for (mu_uint32 n = 0; n < MAX_ALLIANCE_RENDER; ++n)
		{
			_AlliancesListBtn[n] = new UIRadioButton(GAMEUI::MESSAGE_GUILDWINDOW_ALLIANCE_BUTTON_BEGIN + n, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, ALLIANCEBOX_ITEM_POSX, ALLIANCEBOX_ITEM_POSY(n), ALLIANCEBOX_ITEM_WIDTH, ALLIANCEBOX_ITEM_HEIGHT);
			_AlliancesListBtn[n]->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
			_AlliancesListBtn[n]->SetStateData(UIRadioButton::UI_NORMAL, UI_STATE_DATA(USE_COLOR_STATE, EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(0, 0, 0, 0)));
			_AlliancesListBtn[n]->SetStateData(UIRadioButton::UI_OVER, UI_STATE_DATA(USE_COLOR_STATE, EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(150, 150, 150, 220)));
			_AlliancesListBtn[n]->SetStateData(UIRadioButton::UI_PRESSED, UI_STATE_DATA(USE_COLOR_STATE, EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(150, 150, 150, 180)));
			_AlliancesListBtn[n]->SetStateData(UIRadioButton::UI_PRESSED_OVER, UI_STATE_DATA(USE_COLOR_STATE, EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(150, 150, 150, 220)));
		}

		_AllianceCreateBtn = new UIButton(GAMEUI::MESSAGE_GUILDWINDOW_ALLIANCECREATE_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, ALLIANCECREATE_POSX, ALLIANCECREATE_POSY, BUTTON_WIDTH, BUTTON_HEIGHT);
		_AllianceCreateBtn->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_AllianceCreateBtn->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTY_BUTTON_SMALL], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(100, 100, 100, 255)));
		_AllianceCreateBtn->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTY_BUTTON_SMALL], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(255, 255, 255, 255)));
		_AllianceCreateBtn->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTY_BUTTON_SMALL], TUV_THREE_OVER_BEGIN, TUV_THREE_OVER_END, MU::MakeARGB(255, 255, 255, 255)));
		_AllianceCreateBtn->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTY_BUTTON_SMALL], TUV_THREE_PRESSED_BEGIN, TUV_THREE_PRESSED_END, MU::MakeARGB(255, 255, 255, 255)));
		_AllianceCreateBtn->SetStateText(UIButton::UI_DISABLED, FONT_NORMAL, GlobalText[1422], MU::MakeARGB(100, 100, 100, 255));
		_AllianceCreateBtn->SetStateText(UIButton::UI_NORMAL, FONT_NORMAL, GlobalText[1422], MU::MakeARGB(200, 200, 200, 255));
		_AllianceCreateBtn->SetStateText(UIButton::UI_OVER, FONT_NORMAL, GlobalText[1422], MU::MakeARGB(255, 255, 255, 255));
		_AllianceCreateBtn->SetStateText(UIButton::UI_PRESSED, FONT_NORMAL, GlobalText[1422], MU::MakeARGB(150, 150, 150, 255));

		_AllianceOutBtn = new UIButton(GAMEUI::MESSAGE_GUILDWINDOW_ALLIANCEOUT_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, ALLIANCEOUT_POSX, ALLIANCEOUT_POSY, BUTTON_WIDTH, BUTTON_HEIGHT);
		_AllianceOutBtn->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_AllianceOutBtn->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTY_BUTTON_SMALL], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(100, 100, 100, 255)));
		_AllianceOutBtn->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTY_BUTTON_SMALL], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(255, 255, 255, 255)));
		_AllianceOutBtn->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTY_BUTTON_SMALL], TUV_THREE_OVER_BEGIN, TUV_THREE_OVER_END, MU::MakeARGB(255, 255, 255, 255)));
		_AllianceOutBtn->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTY_BUTTON_SMALL], TUV_THREE_PRESSED_BEGIN, TUV_THREE_PRESSED_END, MU::MakeARGB(255, 255, 255, 255)));
		_AllianceOutBtn->SetStateText(UIButton::UI_DISABLED, FONT_NORMAL, GlobalText[1324], MU::MakeARGB(100, 100, 100, 255));
		_AllianceOutBtn->SetStateText(UIButton::UI_NORMAL, FONT_NORMAL, GlobalText[1324], MU::MakeARGB(200, 200, 200, 255));
		_AllianceOutBtn->SetStateText(UIButton::UI_OVER, FONT_NORMAL, GlobalText[1324], MU::MakeARGB(255, 255, 255, 255));
		_AllianceOutBtn->SetStateText(UIButton::UI_PRESSED, FONT_NORMAL, GlobalText[1324], MU::MakeARGB(150, 150, 150, 255));

		for (mu_uint32 n = 0; n < MAX_ALLIANCE_RENDER; ++n)
		{
			_AlliancesMarksSprite[n] = new UISprite(manager, this, nullptr, SCREEN_WIDTH, SCREEN_HEIGHT, ALLIANCEBOX_MARK_POSX, ALLIANCEBOX_MARK_POSY(n), ALLIANCEBOX_MARK_WIDTH, ALLIANCEBOX_MARK_HEIGHT);
		}

		SetCurrentTab(TAB_GUILD);

		SetPosition(GUILDWINDOW_POSX, GUILDWINDOW_POSY);

		ClearMembers();
		ClearAlliances();

		InitializeGPU();
	}
}

UIGuildWindow::~UIGuildWindow()
{
	ReleaseGPU();
}

void UIGuildWindow::InitializeGPU()
{
	for (mu_uint32 n = 0; n < MAX_ALLIANCE_RENDER; ++n)
	{
		_AlliancesMarksTextures[n].Create2D(GUILDMARK_WIDTH, GUILDMARK_HEIGHT, ETextureFormat::eRGBA8_Unorm, ETextureUsage::eDynamic, ESampler::eUcVcWcMinNMagNMipN, nullptr, 0);
		_AlliancesMarksStagingBuffer[n].Allocate(sizeof(mu_uint8) * 4 * GUILDMARK_WIDTH * GUILDMARK_HEIGHT, true);
		_AlliancesMarksSprite[n]->SetTexture(&_AlliancesMarksTextures[n]);
		_AlliancesMarksSprite[n]->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_AlliancesMarksSprite[n]->SetSampler(ESampler::eUcVcWcMinNMagNMipN);
	}

	UIBase::InitializeGPU();
}

void UIGuildWindow::ReleaseGPU()
{
	for (mu_uint32 n = 0; n < MAX_ALLIANCE_RENDER; ++n)
	{
		_AlliancesMarksTextures[n].Release();
		_AlliancesMarksStagingBuffer[n].Deallocate();
	}

	UIBase::ReleaseGPU();
}

void UIGuildWindow::ChangePosition(mu_float x, mu_float y)
{
#define SETPOSITION_HELPER(var) var->SetPosition(var->_ViewX - oldX + x, var->_ViewY - oldY + y)

	mu_float oldX = _ViewX, oldY = _ViewY;

	SETPOSITION_HELPER(this);

	// General
	SETPOSITION_HELPER(_BackFrame);
	SETPOSITION_HELPER(_TopFrame);
	SETPOSITION_HELPER(_BottomFrame);
	SETPOSITION_HELPER(_LeftFrame);
	SETPOSITION_HELPER(_RightFrame);

	SETPOSITION_HELPER(_XButton);
	SETPOSITION_HELPER(_CloseButton);

	SETPOSITION_HELPER(_TabBackground);

	for (mu_uint32 n = 0; n < MAX_TAB; ++n)
	{
		SETPOSITION_HELPER(_TabButtons[n]);
	}

	// Guild Tab
	// Mark Box
	SETPOSITION_HELPER(_MarkTopLine);
	SETPOSITION_HELPER(_MarkBottomLine);
	SETPOSITION_HELPER(_MarkLeftLine);
	SETPOSITION_HELPER(_MarkRightLine);
	SETPOSITION_HELPER(_MarkTopLeft);
	SETPOSITION_HELPER(_MarkTopRight);
	SETPOSITION_HELPER(_MarkBottomLeft);
	SETPOSITION_HELPER(_MarkBottomRight);
	SETPOSITION_HELPER(_MarkBox);

	// Info Box
	SETPOSITION_HELPER(_InfoTopLine);
	SETPOSITION_HELPER(_InfoBottomLine);
	SETPOSITION_HELPER(_InfoLeftLine);
	SETPOSITION_HELPER(_InfoRightLine);
	SETPOSITION_HELPER(_InfoTopLeft);
	SETPOSITION_HELPER(_InfoTopRight);
	SETPOSITION_HELPER(_InfoBottomLeft);
	SETPOSITION_HELPER(_InfoBottomRight);

	// Announcement Box
	SETPOSITION_HELPER(_AnnBack);
	SETPOSITION_HELPER(_AnnTopLine);
	SETPOSITION_HELPER(_AnnBottomLine);
	SETPOSITION_HELPER(_AnnLeftLine);
	SETPOSITION_HELPER(_AnnRightLine);
	SETPOSITION_HELPER(_AnnTab);
	SETPOSITION_HELPER(_AnnTopRight);
	SETPOSITION_HELPER(_AnnBottomLeft);
	SETPOSITION_HELPER(_AnnBottomRight);
	SETPOSITION_HELPER(_AnnScrollBar);
	SETPOSITION_HELPER(_AnnTextBox);

	SETPOSITION_HELPER(_LeaveGuildBtn);

	// Members Tab
	// Members Box
	SETPOSITION_HELPER(_MemberColorABox);
	SETPOSITION_HELPER(_MemberColorBBox);
	SETPOSITION_HELPER(_MemberTopLine);
	SETPOSITION_HELPER(_MemberBottomLine);
	SETPOSITION_HELPER(_MemberLeftLine);
	SETPOSITION_HELPER(_MemberRightLine);
	SETPOSITION_HELPER(_MemberTopLeft);
	SETPOSITION_HELPER(_MemberTopRight);
	SETPOSITION_HELPER(_MemberBottomLeft);
	SETPOSITION_HELPER(_MemberBottomRight);
	SETPOSITION_HELPER(_MemberScrollBar);

	SETPOSITION_HELPER(_GivePositionBtn);
	SETPOSITION_HELPER(_FreePositionBtn);
	SETPOSITION_HELPER(_KickPlayerBtn);

	for (mu_uint32 n = 0; n < MAX_MEMBER_RENDER; ++n)
	{
		SETPOSITION_HELPER(_MembersListBtn[n]);
	}

	// Alliances Tab
	// Alliances Box
	SETPOSITION_HELPER(_AllianceColorABox);
	SETPOSITION_HELPER(_AllianceColorBBox);
	SETPOSITION_HELPER(_AllianceTopLine);
	SETPOSITION_HELPER(_AllianceBottomLine);
	SETPOSITION_HELPER(_AllianceLeftLine);
	SETPOSITION_HELPER(_AllianceRightLine);
	SETPOSITION_HELPER(_AllianceTopLeft);
	SETPOSITION_HELPER(_AllianceTopRight);
	SETPOSITION_HELPER(_AllianceBottomLeft);
	SETPOSITION_HELPER(_AllianceBottomRight);
	SETPOSITION_HELPER(_AllianceScrollBar);

	SETPOSITION_HELPER(_AllianceCreateBtn);
	SETPOSITION_HELPER(_AllianceOutBtn);

	for (mu_uint32 n = 0; n < MAX_ALLIANCE_RENDER; ++n)
	{
		SETPOSITION_HELPER(_AlliancesListBtn[n]);
	}

#undef SETPOSITION_HELPER
}

mu_uint32 UIGuildWindow::OnPreWindowMessage(const SDL_Event *event, mu_size *result)
{
	if (Hero->GuildStatus != GAME::G_NONE)
	{
		switch (_CurrentTab)
		{
		case TAB_MEMBERS:
			{
				UIBase *focusObject = _SceneManager->GetFocused();
				if (focusObject == this ||
					(focusObject != nullptr &&
					focusObject->_Parent == this))
				{
					mu_int32 mouseX = MU_INPUT->GetMouseX();
					mu_int32 mouseY = MU_INPUT->GetMouseY();

					if (mouseY >= _MemberScrollBar->_NativeY &&
						mouseY <= _MemberScrollBar->_NativeY + _MemberScrollBar->_Height &&
						mouseX >= _MembersListBtn[0]->_NativeX &&
						mouseX <= _MembersListBtn[0]->_NativeX + _MembersListBtn[0]->_Width)
					{
						switch (event->type)
						{
						case SDL_MOUSEWHEEL:
							{
								if (_MemberScrollBar->GetState() != UIScrollBar::UI_DISABLED)
								{
									mu_uint32 maxIndex = _Members.size() <= MAX_MEMBER_RENDER ? 0 : _Members.size() - MAX_MEMBER_RENDER;
									mu_float mouseWheel = event->wheel.y;
									mu_float diffValue = mouseWheel * _MemberScrollBar->_DiffValue * -0.03f;
									_MemberScrollBar->ConfigureScrollInfo(_MemberScrollBar->_MinValue, _MemberScrollBar->_MaxValue, _MemberScrollBar->_CurrentValue + diffValue);

									mu_uint32 newIndex = (mu_uint32)(_MemberScrollBar->GetCurrentValue() * (mu_float)maxIndex);
									if (newIndex != _MemberListIndex)
									{
										_MemberListIndex = newIndex;
										ConfigureMemberList();
									}
								}
							}
							return GFX_MESSAGE_RETURN::GFX_MESSAGE_ACCEPT;
						}
					}
				}
			}
			break;

		case TAB_ALLIANCE:
			{
				UIBase *focusObject = _SceneManager->GetFocused();
				if (focusObject == this ||
					(focusObject != nullptr &&
						focusObject->_Parent == this))
				{
					mu_int32 mouseX = MU_INPUT->GetMouseX();
					mu_int32 mouseY = MU_INPUT->GetMouseY();

					if (mouseY >= _AllianceScrollBar->_NativeY &&
						mouseY <= _AllianceScrollBar->_NativeY + _AllianceScrollBar->_Height &&
						mouseX >= _AlliancesListBtn[0]->_NativeX &&
						mouseX <= _AlliancesListBtn[0]->_NativeX + _AlliancesListBtn[0]->_Width)
					{
						switch (event->type)
						{
						case SDL_MOUSEWHEEL:
							{
								if (_AllianceScrollBar->GetState() != UIScrollBar::UI_DISABLED)
								{
									mu_uint32 maxIndex = _Alliances.size() <= MAX_ALLIANCE_RENDER ? 0 : _Alliances.size() - MAX_ALLIANCE_RENDER;
									mu_float mouseWheel = event->wheel.y;
									mu_float diffValue = mouseWheel * _AllianceScrollBar->_DiffValue * -0.03f;
									_AllianceScrollBar->ConfigureScrollInfo(_AllianceScrollBar->_MinValue, _AllianceScrollBar->_MaxValue, _AllianceScrollBar->_CurrentValue + diffValue);

									mu_uint32 newIndex = (mu_uint32)(_AllianceScrollBar->GetCurrentValue() * (mu_float)maxIndex);
									if (newIndex != _AllianceListIndex)
									{
										_AllianceListIndex = newIndex;
										ConfigureAllianceList();
									}
								}
							}
							return GFX_MESSAGE_RETURN::GFX_MESSAGE_ACCEPT;
						}
					}
				}
			}
			break;
		}
	}

	return GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE;
}

void UIGuildWindow::Update(mu_uint64 CurrentTime)
{
	if (Hero->GuildStatus != GAME::G_NONE)
	{
		switch (_CurrentTab)
		{
		case TAB_GUILD:
			{
				_AnnTextBox->Update(CurrentTime);
			}
			break;
		}
	}
}

void UIGuildWindow::Draw()
{
	DrawFrames();
	DrawButtons();
	DrawTexts();
}

void UIGuildWindow::PostDraw()
{
	PostDrawButtons();
}

void UIGuildWindow::DrawFrames()
{
	MARK_t &mark = GuildMark[Hero->GuildMarkIndex];

	_BackFrame->Draw();
	_TopFrame->Draw();
	_BottomFrame->Draw();
	_LeftFrame->Draw();
	_RightFrame->Draw();

	if (Hero->GuildStatus != GAME::G_NONE)
	{
		_TabBackground->Draw();

		switch (_CurrentTab)
		{
		case TAB_GUILD:
			{
				_MarkTopLine->Draw();
				_MarkBottomLine->Draw();
				_MarkLeftLine->Draw();
				_MarkRightLine->Draw();
				_MarkTopLeft->Draw();
				_MarkTopRight->Draw();
				_MarkBottomLeft->Draw();
				_MarkBottomRight->Draw();
				_MarkBox->Draw();

				_InfoTopLine->Draw();
				_InfoBottomLine->Draw();
				_InfoLeftLine->Draw();
				_InfoRightLine->Draw();
				_InfoTopLeft->Draw();
				_InfoTopRight->Draw();
				_InfoBottomLeft->Draw();
				_InfoBottomRight->Draw();

				_AnnBack->Draw();
				_AnnTopLine->Draw();
				_AnnBottomLine->Draw();
				_AnnLeftLine->Draw();
				_AnnRightLine->Draw();
				_AnnTopRight->Draw();
				_AnnBottomLeft->Draw();
				_AnnBottomRight->Draw();
				_AnnTab->Draw();
			}
			break;

		case TAB_MEMBERS:
			{
				_MemberColorABox->Draw();
				_MemberColorBBox->Draw();

				mu_uint32 maxIndex = _Members.size();
				if (maxIndex > MAX_MEMBER_RENDER) maxIndex = MAX_MEMBER_RENDER;
				for (mu_uint32 n = 0; n < maxIndex; ++n)
				{
					_MembersListBtn[n]->Draw();
				}

				_MemberTopLine->Draw();
				_MemberBottomLine->Draw();
				_MemberLeftLine->Draw();
				_MemberRightLine->Draw();
				_MemberTopLeft->Draw();
				_MemberTopRight->Draw();
				_MemberBottomLeft->Draw();
				_MemberBottomRight->Draw();
			}
			break;

		case TAB_ALLIANCE:
			{
				if (mark.UnionName.empty() == false)
				{
					_AllianceColorABox->Draw();
					_AllianceColorBBox->Draw();

					mu_uint32 maxIndex = _Alliances.size();
					if (maxIndex > MAX_ALLIANCE_RENDER) maxIndex = MAX_ALLIANCE_RENDER;
					for (mu_uint32 n = 0; n < maxIndex; ++n)
					{
						_AlliancesListBtn[n]->Draw();
						_AlliancesMarksSprite[n]->Draw();
					}

					_AllianceTopLine->Draw();
					_AllianceBottomLine->Draw();
					_AllianceLeftLine->Draw();
					_AllianceRightLine->Draw();
					_AllianceTopLeft->Draw();
					_AllianceTopRight->Draw();
					_AllianceBottomLeft->Draw();
					_AllianceBottomRight->Draw();
				}
			}
			break;
		}
	}
}

void UIGuildWindow::DrawButtons()
{
	MARK_t &mark = GuildMark[Hero->GuildMarkIndex];

	_CloseButton->Draw();

	if (Hero->GuildStatus != GAME::G_NONE)
	{
		for (mu_uint32 n = 0; n < MAX_TAB; ++n)
		{
			if (n == _CurrentTab) continue;

			_TabButtons[n]->Draw();
		}

		_TabButtons[_CurrentTab]->Draw();

		switch (_CurrentTab)
		{
		case TAB_GUILD:
			{
				_AnnScrollBar->Draw();

				_AnnTextBox->Draw();

				_LeaveGuildBtn->Draw();
			}
			break;

		case TAB_MEMBERS:
			{
				_MemberScrollBar->Draw();

				_GivePositionBtn->Draw();
				_FreePositionBtn->Draw();
				_KickPlayerBtn->Draw();
			}
			break;

		case TAB_ALLIANCE:
			{
				if (mark.UnionName.empty() == false)
				{
					_AllianceScrollBar->Draw();

					_AllianceCreateBtn->Draw();
					_AllianceOutBtn->Draw();
				}
			}
			break;
		}
	}
}

void UIGuildWindow::PostDrawButtons()
{
	MARK_t &mark = GuildMark[Hero->GuildMarkIndex];

	_CloseButton->PostDraw();

	if (Hero->GuildStatus != GAME::G_NONE)
	{
		for (mu_uint32 n = 0; n < MAX_TAB; ++n)
		{
			if (n == _CurrentTab) continue;

			_TabButtons[n]->PostDraw();
		}

		_TabButtons[_CurrentTab]->PostDraw();

		switch (_CurrentTab)
		{
		case TAB_GUILD:
			{
				_AnnScrollBar->PostDraw();

				_AnnTextBox->PostDraw();

				_LeaveGuildBtn->PostDraw();
			}
			break;

		case TAB_MEMBERS:
			{
				_MemberScrollBar->PostDraw();

				_GivePositionBtn->PostDraw();
				_FreePositionBtn->PostDraw();
				_KickPlayerBtn->PostDraw();
			}
			break;

		case TAB_ALLIANCE:
			{
				if (mark.UnionName.empty() == false)
				{
					_AllianceScrollBar->PostDraw();

					_AllianceCreateBtn->PostDraw();
					_AllianceOutBtn->PostDraw();
				}
			}
			break;
		}
	}
}

void UIGuildWindow::DrawTexts()
{
	mu_float screenX = MU_RESOURCE->_640x480Rate[0];
	mu_float screenY = MU_RESOURCE->_640x480Rate[1];
	mu_uint32 textColor = MU::MakeARGB(255, 255, 255, 255);
	mu_uint32 bgColor = MU::MakeARGB(0, 0, 0, 0);

	FONT_BOLD->DrawStringC2D(TEXTINDEX::GUILDWINDOW_TITLE,
		MU_TO_WINDOW(_ViewX + GUILDWINDOW_WIDTH * 0.5f, screenX),
		MU_TO_WINDOW(_ViewY + 15.0f, screenY),
		1.0f,
		ETA_CENTER,
		ETA_CENTER,
		0.0f,
		textColor,
		bgColor,
		GlobalText[180]);

	if (Hero->GuildStatus == GAME::G_NONE)
	{
		static mu_text guildInfo[512] = { 0 };
		static mu_boolean setGuildInfo = false;

		if (setGuildInfo == false)
		{
			setGuildInfo = true;
			mu_sprintf_s(guildInfo, mu_countof(guildInfo), _T("") SPF_STRING _T("\n\n") SPF_STRING _T("\n\n") SPF_STRING _T(""),
				GlobalText[185],
				GlobalText[186],
				GlobalText[187]);
		}

		FONT_NORMAL->DrawStringC2D(TEXTINDEX::GUILDWINDOW_INFO,
			MU_TO_WINDOW(_ViewX + 20.0f, screenX),
			MU_TO_WINDOW(_ViewY + 60.0f + 0.0f, screenY),
			1.0f,
			ETA_LEFT,
			ETA_BOTTOM,
			0.0f,
			textColor,
			bgColor,
			guildInfo);
	}
	else
	{
		MARK_t &mark = GuildMark[Hero->GuildMarkIndex];

		mu_text textBuffer[1024];
		mu_sprintf_s(textBuffer, mu_countof(textBuffer), _T("") SPF_STRING _T(" ( Score : %d )"), mark.GuildName.c_str(), GuildTotalScore);

		FONT_NORMAL->DrawStringC2D(TEXTINDEX::GUILDWINDOW_SCORE,
			MU_TO_WINDOW(_ViewX + _ViewWidth * 0.5f, screenX),
			MU_TO_WINDOW(_ViewY + 50.0f, screenY),
			1.0f,
			ETA_CENTER,
			ETA_CENTER,
			0.0f,
			MU::MakeARGB(200, 255, 100, 255),
			bgColor,
			textBuffer);

		switch (_CurrentTab)
		{
		case TAB_GUILD:
			{
				mu_float normalHeight = FONT_NORMAL->GetFont()->GetLineHeight();
				mu_float textY = _ViewY + 165.0f;

				mu_sprintf_s(textBuffer, mu_countof(textBuffer), _T("") SPF_STRING _T(" :"), GlobalText[1332]);
				FONT_NORMAL->DrawStringC2D(TEXTINDEX::GUILDWINDOW_MISC_BEGIN + 0,
					MU_TO_WINDOW(_ViewX + 22.0f, screenX),
					MU_TO_WINDOW(textY, screenY),
					1.0f,
					ETA_LEFT,
					ETA_BOTTOM,
					0.0f,
					textColor,
					bgColor,
					textBuffer);
				textY += normalHeight;

				mu_sprintf_s(textBuffer, mu_countof(textBuffer), GlobalText[1256], GuildTotalScore);
				FONT_NORMAL->DrawStringC2D(TEXTINDEX::GUILDWINDOW_MISC_BEGIN + 1,
					MU_TO_WINDOW(_ViewX + 22.0f, screenX),
					MU_TO_WINDOW(textY, screenY),
					1.0f,
					ETA_LEFT,
					ETA_BOTTOM,
					0.0f,
					textColor,
					bgColor,
					textBuffer);
				textY += normalHeight;

				if (IsGuildMaster())
				{
					mu_int32 baseClass = GetBaseClass(CharacterAttribute->Class);

					if (baseClass == GAME::CLASS_DARK_LORD)
					{
						mu_int32 maxMembers = CharacterAttribute->Level / 10 + CharacterAttribute->Charisma / 10;
						if (maxMembers > MAX_GUILDMEMBER)
							maxMembers = MAX_GUILDMEMBER;

						mu_sprintf_s(textBuffer, mu_countof(textBuffer), GlobalText[1362], static_cast<mu_uint32>(_Members.size()), maxMembers);
					}
					else
					{
						mu_int32 maxMembers = CharacterAttribute->Level / 10;
						if (maxMembers > MAX_GUILDMEMBER)
							maxMembers = MAX_GUILDMEMBER;

						mu_sprintf_s(textBuffer, mu_countof(textBuffer), GlobalText[1362], static_cast<mu_uint32>(_Members.size()), maxMembers);
					}
				}
				else
				{
					mu_sprintf_s(textBuffer, mu_countof(textBuffer), GlobalText[1310], static_cast<mu_uint32>(_Members.size()));
				}

				FONT_NORMAL->DrawStringC2D(TEXTINDEX::GUILDWINDOW_MISC_BEGIN + 2,
					MU_TO_WINDOW(_ViewX + 22.0f, screenX),
					MU_TO_WINDOW(textY, screenY),
					1.0f,
					ETA_LEFT,
					ETA_BOTTOM,
					0.0f,
					textColor,
					bgColor,
					textBuffer);
				textY += normalHeight;

				mu_sprintf_s(textBuffer, mu_countof(textBuffer), _T("") SPF_STRING _T(" : ") SPF_STRING _T(""), GlobalText[1321], mark.RivalName.empty() == false ? mark.RivalName.c_str() : GlobalText[1361]);
				FONT_NORMAL->DrawStringC2D(TEXTINDEX::GUILDWINDOW_MISC_BEGIN + 3,
					MU_TO_WINDOW(_ViewX + 22.0f, screenX),
					MU_TO_WINDOW(textY, screenY),
					1.0f,
					ETA_LEFT,
					ETA_BOTTOM,
					0.0f,
					textColor,
					bgColor,
					textBuffer);
				textY += normalHeight;
			}
			break;

		case TAB_MEMBERS:
			{
				FONT_NORMAL->DrawStringC2D(TEXTINDEX::GUILDWINDOW_MISC_BEGIN + 0,
					MU_TO_WINDOW(_ViewX + 18.0f, screenX),
					MU_TO_WINDOW(MEMBERBOX_COLORB_POSY + MEMBERBOX_COLORB_HEIGHT * 0.5f + 1.0f, screenY),
					1.0f,
					ETA_LEFT,
					ETA_CENTER,
					0.0f,
					textColor,
					bgColor,
					GlobalText[1389]);

				FONT_NORMAL->DrawStringC2D(TEXTINDEX::GUILDWINDOW_MISC_BEGIN + 1,
					MU_TO_WINDOW(_ViewX + 95.0f, screenX),
					MU_TO_WINDOW(MEMBERBOX_COLORB_POSY + MEMBERBOX_COLORB_HEIGHT * 0.5f + 1.0f, screenY),
					1.0f,
					ETA_RIGHT,
					ETA_CENTER,
					0.0f,
					textColor,
					bgColor,
					GlobalText[1307]);
			
				FONT_NORMAL->DrawStringC2D(TEXTINDEX::GUILDWINDOW_MISC_BEGIN + 2,
					MU_TO_WINDOW(_ViewX + 100.0f, screenX),
					MU_TO_WINDOW(MEMBERBOX_COLORB_POSY + MEMBERBOX_COLORB_HEIGHT * 0.5f + 1.0f, screenY),
					1.0f,
					ETA_LEFT,
					ETA_CENTER,
					0.0f,
					textColor,
					bgColor,
					GlobalText[1022]);

				UIBase *focused = _SceneManager->GetFocused();
				mu_uint32 maxIndex = _Members.size();
				if (maxIndex > MAX_MEMBER_RENDER) maxIndex = MAX_MEMBER_RENDER;
				auto it = _Members.begin();
				for (mu_uint32 n = _MemberListIndex; n--; ++it);
				for (mu_uint32 n = 0; n < maxIndex; ++n, ++it)
				{
					GUILD_LIST_t &member = *it;

					mu_boolean isSelected = member.CharacterIndex == _MemberSelected || _MembersListBtn[n] == focused;
					mu_uint32 memberColor = isSelected == true ? MU::MakeARGB(230, 220, 200, 255) : MU::MakeARGB(180, 170, 150, 255);

					FONT_NORMAL->DrawStringC2D(TEXTINDEX::GUILDWINDOW_MEMBER_NAME_BEGIN + n,
						MU_TO_WINDOW(MEMBERBOX_ITEM_POSX + 5.0f, screenX),
						MU_TO_WINDOW(MEMBERBOX_ITEM_POSY(n) + MEMBERBOX_ITEM_HEIGHT * 0.5f, screenY),
						1.0f,
						ETA_LEFT,
						ETA_CENTER,
						0.0f,
						memberColor,
						bgColor,
						member.Name.c_str());

					switch (member.GuildStatus)
					{
					case GAME::G_BATTLE_MASTER:
						{
							FONT_NORMAL->DrawStringC2D(TEXTINDEX::GUILDWINDOW_MEMBER_POSITION_BEGIN + n,
								MU_TO_WINDOW(MEMBERBOX_ITEM_POSX + 82.0f, screenX),
								MU_TO_WINDOW(MEMBERBOX_ITEM_POSY(n) + MEMBERBOX_ITEM_HEIGHT * 0.5f, screenY),
								1.0f,
								ETA_RIGHT,
								ETA_CENTER,
								0.0f,
								memberColor,
								bgColor,
								GlobalText[1302]);
						}
						break;

					case GAME::G_SUB_MASTER:
						{
							FONT_NORMAL->DrawStringC2D(TEXTINDEX::GUILDWINDOW_MEMBER_POSITION_BEGIN + n,
								MU_TO_WINDOW(MEMBERBOX_ITEM_POSX + 82.0f, screenX),
								MU_TO_WINDOW(MEMBERBOX_ITEM_POSY(n) + MEMBERBOX_ITEM_HEIGHT * 0.5f, screenY),
								1.0f,
								ETA_RIGHT,
								ETA_CENTER,
								0.0f,
								memberColor,
								bgColor,
								GlobalText[1301]);
						}
						break;

					case GAME::G_MASTER:
						{
							FONT_NORMAL->DrawStringC2D(TEXTINDEX::GUILDWINDOW_MEMBER_POSITION_BEGIN + n,
								MU_TO_WINDOW(MEMBERBOX_ITEM_POSX + 82.0f, screenX),
								MU_TO_WINDOW(MEMBERBOX_ITEM_POSY(n) + MEMBERBOX_ITEM_HEIGHT * 0.5f, screenY),
								1.0f,
								ETA_RIGHT,
								ETA_CENTER,
								0.0f,
								memberColor,
								bgColor,
								GlobalText[1300]);
						}
						break;
					}

					if (member.GroupIndex != -1 &&
						member.ServerIndex != -1)
					{
						mu_uint32 serverColor = isSelected == true ? MU::MakeARGB(255, 196, 0, 255) : MU::MakeARGB(205, 146, 0, 255);
						mu_text strServerName[MAX_TEXT_LENGTH];
						mu_sprintf_s(strServerName, mu_countof(strServerName), _T("[") SPF_STRING _T("-%d]"), g_pServerList->GetGroupName(member.GroupIndex).c_str(), member.ServerIndex + 1);
						
						FONT_NORMAL->DrawStringC2D(TEXTINDEX::GUILDWINDOW_MEMBER_SERVER_BEGIN + n,
							MU_TO_WINDOW(MEMBERBOX_ITEM_POSX + 87.0f, screenX),
							MU_TO_WINDOW(MEMBERBOX_ITEM_POSY(n) + MEMBERBOX_ITEM_HEIGHT * 0.5f, screenY),
							1.0f,
							ETA_LEFT,
							ETA_CENTER,
							0.0f,
							serverColor,
							bgColor,
							strServerName);
					}
					else
					{
						FONT_NORMAL->DrawStringC2D(TEXTINDEX::GUILDWINDOW_MEMBER_SERVER_BEGIN + n,
							MU_TO_WINDOW(MEMBERBOX_ITEM_POSX + 87.0f, screenX),
							MU_TO_WINDOW(MEMBERBOX_ITEM_POSY(n) + MEMBERBOX_ITEM_HEIGHT * 0.5f, screenY),
							1.0f,
							ETA_LEFT,
							ETA_CENTER,
							0.0f,
							memberColor,
							bgColor,
							_T("Offline"));
					}
				}
			}
			break;

		case TAB_ALLIANCE:
			{
				if (mark.UnionName.empty() == false)
				{
					FONT_NORMAL->DrawStringC2D(TEXTINDEX::GUILDWINDOW_MISC_BEGIN + 0,
						MU_TO_WINDOW(ALLIANCEBOX_ITEM_POSX + 12.0f, screenX),
						MU_TO_WINDOW(ALLIANCEBOX_COLORB_POSY + ALLIANCEBOX_COLORB_HEIGHT * 0.5f + 1.0f, screenY),
						1.0f,
						ETA_LEFT,
						ETA_CENTER,
						0.0f,
						textColor,
						bgColor,
						GlobalText[182]);

					FONT_NORMAL->DrawStringC2D(TEXTINDEX::GUILDWINDOW_MISC_BEGIN + 1,
						MU_TO_WINDOW(ALLIANCEBOX_ITEM_POSX + 125.0f, screenX),
						MU_TO_WINDOW(ALLIANCEBOX_COLORB_POSY + ALLIANCEBOX_COLORB_HEIGHT * 0.5f + 1.0f, screenY),
						1.0f,
						ETA_RIGHT,
						ETA_CENTER,
						0.0f,
						textColor,
						bgColor,
						GlobalText[1330]);

					UIBase *focused = _SceneManager->GetFocused();
					mu_uint32 maxIndex = _Alliances.size();
					if (maxIndex > MAX_ALLIANCE_RENDER) maxIndex = MAX_ALLIANCE_RENDER;
					auto it = _Alliances.begin();
					for (mu_uint32 n = _AllianceListIndex; n--; ++it);
					for (mu_uint32 n = 0; n < maxIndex; ++n, ++it)
					{
						ALLIANCE_LIST_t &member = it->second;

						mu_boolean isSelected = member.GuildIndex == _AllianceSelected || _AlliancesListBtn[n] == focused;
						mu_uint32 memberColor = isSelected == true ? MU::MakeARGB(230, 220, 200, 255) : MU::MakeARGB(180, 170, 150, 255);

						FONT_NORMAL->DrawStringC2D(TEXTINDEX::GUILDWINDOW_ALLIANCE_NAME_BEGIN + n,
							MU_TO_WINDOW(ALLIANCEBOX_ITEM_POSX + 12.0f, screenX),
							MU_TO_WINDOW(ALLIANCEBOX_ITEM_POSY(n) + ALLIANCEBOX_ITEM_HEIGHT * 0.5f, screenY),
							1.0f,
							ETA_LEFT,
							ETA_CENTER,
							0.0f,
							memberColor,
							bgColor,
							member.GuildName.c_str());

						mu_text countsBuffer[32];
						mu_sprintf_s(countsBuffer, mu_countof(countsBuffer), _T("%u"), member.MembersCount);

						FONT_NORMAL->DrawStringC2D(TEXTINDEX::GUILDWINDOW_ALLIANCE_MEMBERCOUNT_BEGIN + n,
							MU_TO_WINDOW(ALLIANCEBOX_ITEM_POSX + 125.0f, screenX),
							MU_TO_WINDOW(ALLIANCEBOX_ITEM_POSY(n) + ALLIANCEBOX_ITEM_HEIGHT * 0.5f, screenY),
							1.0f,
							ETA_RIGHT,
							ETA_CENTER,
							0.0f,
							memberColor,
							bgColor,
							countsBuffer);
					}
				}
				else
				{
					static mu_text allianceBuffer[2048] = { 0 };
					static mu_boolean setAllianceInfo = false;
					if (setAllianceInfo == false)
					{
						setAllianceInfo = true;

						mu_sprintf_s(allianceBuffer, mu_countof(allianceBuffer), _T("") SPF_STRING _T("\n\n") SPF_STRING _T("\n\n") SPF_STRING _T("\n\n") SPF_STRING _T("\n\n") SPF_STRING _T("\n\n\n") SPF_STRING _T("\n\n") SPF_STRING _T("\n\n") SPF_STRING _T("\n\n") SPF_STRING _T("\n\n") SPF_STRING _T("\n\n") SPF_STRING _T(""),
							GlobalText[1257],
							GlobalText[1258],
							GlobalText[1259],
							GlobalText[1260],
							GlobalText[1261],
							GlobalText[1262],
							GlobalText[1263],
							GlobalText[1264],
							GlobalText[1265],
							GlobalText[1266],
							GlobalText[1267]);
					}

					FONT_NORMAL->DrawStringC2D(TEXTINDEX::GUILDWINDOW_MISC_BEGIN + 0,
						MU_TO_WINDOW(_ViewX + 25.0f, screenX),
						MU_TO_WINDOW(_ViewY + 106.0f, screenY),
						1.0f,
						ETA_LEFT,
						ETA_BOTTOM,
						0.0f,
						textColor,
						bgColor,
						allianceBuffer);
				}
			}
			break;
		}
	}
}

void UIGuildWindow::SetCurrentTab(mu_uint32 currentTab)
{
	for (mu_uint32 n = 0; n < MAX_TAB; ++n)
	{
		if (n == currentTab) continue;

		_TabButtons[n]->ChangeState(UIRadioButton::UI_NORMAL);
	}

	_CurrentTab = currentTab;
	_TabButtons[currentTab]->ChangeState(UIRadioButton::UI_PRESSED);
}

UIBase *UIGuildWindow::GetFocus(mu_int32 mouseX, mu_int32 mouseY)
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

		if (Hero->GuildStatus != GAME::G_NONE)
		{
			MARK_t &mark = GuildMark[Hero->GuildMarkIndex];

			object = _TabButtons[_CurrentTab]->GetFocus(mouseX, mouseY);

			if (object != nullptr)
			{
				return object;
			}

			for (mu_uint32 n = 0; n < MAX_TAB; ++n)
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
			case TAB_GUILD:
				{
					object = _AnnScrollBar->GetFocus(mouseX, mouseY);

					if (object != nullptr)
					{
						return object;
					}

					object = _AnnTextBox->GetFocus(mouseX, mouseY);

					if (object != nullptr)
					{
						return object;
					}

					object = _LeaveGuildBtn->GetFocus(mouseX, mouseY);

					if (object != nullptr)
					{
						return object;
					}
				}
				break;

			case TAB_MEMBERS:
				{
					object = _MemberScrollBar->GetFocus(mouseX, mouseY);

					if (object != nullptr)
					{
						return object;
					}

					mu_uint32 maxIndex = _Members.size();
					if (maxIndex > MAX_MEMBER_RENDER) maxIndex = MAX_MEMBER_RENDER;
					for (mu_uint32 n = 0; n < maxIndex; ++n)
					{
						object = _MembersListBtn[n]->GetFocus(mouseX, mouseY);

						if (object != nullptr)
						{
							return object;
						}
					}

					object = _GivePositionBtn->GetFocus(mouseX, mouseY);

					if (object != nullptr)
					{
						return object;
					}

					object = _FreePositionBtn->GetFocus(mouseX, mouseY);

					if (object != nullptr)
					{
						return object;
					}

					object = _KickPlayerBtn->GetFocus(mouseX, mouseY);

					if (object != nullptr)
					{
						return object;
					}
				}
				break;

			case TAB_ALLIANCE:
				{
					if (mark.UnionName.empty() == false)
					{
						object = _AllianceScrollBar->GetFocus(mouseX, mouseY);

						if (object != nullptr)
						{
							return object;
						}

						mu_uint32 maxIndex = _Alliances.size();
						if (maxIndex > MAX_ALLIANCE_RENDER) maxIndex = MAX_ALLIANCE_RENDER;
						for (mu_uint32 n = 0; n < maxIndex; ++n)
						{
							object = _AlliancesListBtn[n]->GetFocus(mouseX, mouseY);

							if (object != nullptr)
							{
								return object;
							}
						}

						object = _AllianceCreateBtn->GetFocus(mouseX, mouseY);

						if (object != nullptr)
						{
							return object;
						}

						object = _AllianceOutBtn->GetFocus(mouseX, mouseY);

						if (object != nullptr)
						{
							return object;
						}
					}
				}
				break;
			}
		}
	}

	return focus;
}

void UIGuildWindow::ClearMembers()
{
	_Members.clear();
	_MemberSelected = -1;
	_MemberListIndex = 0;
	_MemberScrollBar->ConfigureScrollInfo(0.0f, 1.0f, 0.0f);
}

void UIGuildWindow::AddMember(GUILD_LIST_t &member)
{
	mu_boolean isFound = false;
	for (auto it = _Members.begin(); it != _Members.end(); ++it)
	{
		GUILD_LIST_t &m = *it;

		if (m.CharacterIndex == member.CharacterIndex)
		{
			m = member;
			isFound = true;
			break;
		}
	}

	if (isFound == false)
	{
		_Members.push_back(member);
	}

	ConfigureMemberScroll();
	ConfigureMemberList();
}

void UIGuildWindow::DeleteMember(mu_int32 number)
{
	if (_MemberSelected == number)
	{
		_MemberSelected = -1;
	}

	for (auto it = _Members.begin(); it != _Members.end(); ++it)
	{
		GUILD_LIST_t &member = *it;

		if (member.CharacterIndex == number)
		{
			_Members.erase(it);
			break;
		}
	}

	ConfigureMemberScroll();
	ConfigureMemberList();
}

void UIGuildWindow::SetMemberOffline(mu_int32 number)
{
	for (auto it = _Members.begin(); it != _Members.end(); ++it)
	{
		GUILD_LIST_t &member = *it;

		if (member.CharacterIndex == number)
		{
			member.GroupIndex = -1;
			member.ServerIndex = -1;
			break;
		}
	}
}

void UIGuildWindow::ConfigureMemberScroll()
{
	mu_boolean isEnabledScroll = _MemberScrollBar->GetState() != UIScrollBar::UI_DISABLED;

	if (_Members.size() > MAX_MEMBER_RENDER &&
		isEnabledScroll == false)
	{
		_MemberListIndex = 0;
		_MemberScrollBar->ChangeState(UIScrollBar::UI_NORMAL);
		_MemberScrollBar->ConfigureScrollInfo(0.0f, 1.0f, 0.0f);
	}
	else if (_Members.size() <= MAX_MEMBER_RENDER &&
		isEnabledScroll == true)
	{
		_MemberListIndex = 0;
		_MemberScrollBar->ChangeState(UIScrollBar::UI_DISABLED);
		_MemberScrollBar->ConfigureScrollInfo(0.0f, 1.0f, 0.0f);
	}

	if (_MemberScrollBar->GetState() != UIScrollBar::UI_DISABLED)
	{
		mu_uint32 maxIndex = _Members.size() - MAX_MEMBER_RENDER;
		if (_MemberListIndex > maxIndex)
		{
			_MemberListIndex = maxIndex;
			_MemberScrollBar->ConfigureScrollInfo(0.0f, 1.0f, 1.0f);
		}
	}
}

void UIGuildWindow::ConfigureMemberList()
{
	UIBase *focused = _SceneManager->GetFocused();
	auto it = _Members.begin();

	for (mu_uint32 n = _MemberListIndex; n--; ++it);

	mu_uint32 maxIndex = _Members.size() > MAX_MEMBER_RENDER ? MAX_MEMBER_RENDER : _Members.size();
	for (mu_uint32 n = 0; n < maxIndex; ++n, ++it)
	{
		GUILD_LIST_t &member = *it;
		UIRadioButton *btn = _MembersListBtn[n];

		if (_MemberSelected == member.CharacterIndex)
		{
			btn->ChangeState(focused == btn ? UIRadioButton::UI_PRESSED_OVER : UIRadioButton::UI_PRESSED);
		}
		else
		{
			btn->ChangeState(focused == btn ? UIRadioButton::UI_OVER : UIRadioButton::UI_NORMAL);
		}

		switch (member.GuildStatus)
		{
		case GAME::G_PERSON:
			{
				btn->SetStateData(UIRadioButton::UI_NORMAL, UI_STATE_DATA(USE_COLOR_STATE, EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(0, 0, 0, 0)));
				btn->SetStateData(UIRadioButton::UI_OVER, UI_STATE_DATA(USE_COLOR_STATE, EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(150, 150, 150, 220)));
				btn->SetStateData(UIRadioButton::UI_PRESSED, UI_STATE_DATA(USE_COLOR_STATE, EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(150, 150, 150, 180)));
				btn->SetStateData(UIRadioButton::UI_PRESSED_OVER, UI_STATE_DATA(USE_COLOR_STATE, EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(150, 150, 150, 220)));
			}
			break;

		case GAME::G_BATTLE_MASTER:
			{
				btn->SetStateData(UIRadioButton::UI_NORMAL, UI_STATE_DATA(USE_COLOR_STATE, EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 200, 200, 100)));
				btn->SetStateData(UIRadioButton::UI_OVER, UI_STATE_DATA(USE_COLOR_STATE, EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 200, 200, 220)));
				btn->SetStateData(UIRadioButton::UI_PRESSED, UI_STATE_DATA(USE_COLOR_STATE, EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 200, 200, 180)));
				btn->SetStateData(UIRadioButton::UI_PRESSED_OVER, UI_STATE_DATA(USE_COLOR_STATE, EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 200, 200, 220)));
			}
			break;

		case GAME::G_SUB_MASTER:
			{
				btn->SetStateData(UIRadioButton::UI_NORMAL, UI_STATE_DATA(USE_COLOR_STATE, EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 150, 80, 100)));
				btn->SetStateData(UIRadioButton::UI_OVER, UI_STATE_DATA(USE_COLOR_STATE, EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 150, 80, 220)));
				btn->SetStateData(UIRadioButton::UI_PRESSED, UI_STATE_DATA(USE_COLOR_STATE, EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 150, 80, 180)));
				btn->SetStateData(UIRadioButton::UI_PRESSED_OVER, UI_STATE_DATA(USE_COLOR_STATE, EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 150, 80, 220)));
			}
			break;

		case GAME::G_MASTER:
			{
				btn->SetStateData(UIRadioButton::UI_NORMAL, UI_STATE_DATA(USE_COLOR_STATE, EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 100, 50, 100)));
				btn->SetStateData(UIRadioButton::UI_OVER, UI_STATE_DATA(USE_COLOR_STATE, EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 100, 50, 220)));
				btn->SetStateData(UIRadioButton::UI_PRESSED, UI_STATE_DATA(USE_COLOR_STATE, EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 100, 50, 180)));
				btn->SetStateData(UIRadioButton::UI_PRESSED_OVER, UI_STATE_DATA(USE_COLOR_STATE, EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 100, 50, 220)));
			}
			break;
		}
	}
}

void UIGuildWindow::ClearAlliances()
{
	_Alliances.clear();
	_AllianceSelected = -1;
	_AllianceListIndex = 0;
	_AllianceScrollBar->ConfigureScrollInfo(0.0f, 1.0f, 0.0f);
}

void UIGuildWindow::AddAlliance(ALLIANCE_LIST_t &alliance)
{
	_Alliances.insert(std::make_pair(alliance.GuildIndex, alliance));

	ConfigureAllianceScroll();
	ConfigureAllianceList();
}

void UIGuildWindow::DeleteAlliance(mu_uint32 number)
{
	if (_AllianceSelected == number)
	{
		_AllianceSelected = -1;
	}

	_Alliances.erase(number);

	ConfigureAllianceScroll();
	ConfigureAllianceList();
}

void UIGuildWindow::ConfigureAllianceScroll()
{
	mu_boolean isEnabledScroll = _AllianceScrollBar->GetState() != UIScrollBar::UI_DISABLED;

	if (_Alliances.size() > MAX_ALLIANCE_RENDER &&
		isEnabledScroll == false)
	{
		_AllianceListIndex = 0;
		_AllianceScrollBar->ChangeState(UIScrollBar::UI_NORMAL);
		_AllianceScrollBar->ConfigureScrollInfo(0.0f, 1.0f, 0.0f);
	}
	else if (_Alliances.size() <= MAX_ALLIANCE_RENDER &&
		isEnabledScroll == true)
	{
		_AllianceListIndex = 0;
		_AllianceScrollBar->ChangeState(UIScrollBar::UI_DISABLED);
		_AllianceScrollBar->ConfigureScrollInfo(0.0f, 1.0f, 0.0f);
	}

	if (_AllianceScrollBar->GetState() != UIScrollBar::UI_DISABLED)
	{
		mu_uint32 maxIndex = _Alliances.size() - MAX_ALLIANCE_RENDER;
		if (_AllianceListIndex > maxIndex)
		{
			_AllianceListIndex = maxIndex;
			_AllianceScrollBar->ConfigureScrollInfo(0.0f, 1.0f, 1.0f);
		}
	}
}

void UIGuildWindow::ConfigureAllianceList()
{
	UIBase *focused = _SceneManager->GetFocused();
	auto it = _Alliances.begin();

	for (mu_uint32 n = _AllianceListIndex; n--; ++it);

	mu_uint32 maxIndex = _Alliances.size() > MAX_ALLIANCE_RENDER ? MAX_ALLIANCE_RENDER : _Alliances.size();
	for (mu_uint32 n = 0; n < maxIndex; ++n, ++it)
	{
		ALLIANCE_LIST_t &member = it->second;
		UIRadioButton *btn = _AlliancesListBtn[n];

		if (_AllianceSelected == member.GuildIndex)
		{
			btn->ChangeState(focused == btn ? UIRadioButton::UI_PRESSED_OVER : UIRadioButton::UI_PRESSED);
		}
		else
		{
			btn->ChangeState(focused == btn ? UIRadioButton::UI_OVER : UIRadioButton::UI_NORMAL);
		}

		CreateGuildMark(member.MarkBuffer, false, &_AlliancesMarksTextures[n], &_AlliancesMarksStagingBuffer[n]);
	}
}

mu_boolean UIGuildWindow::OnGFxMessage(mu_uint32 objectid, mu_uint32 message, mu_size message_data)
{
	if (objectid == GAMEUI::MESSAGE_GUILDWINDOW_MEMBER_SCROLLBAR)
	{
		switch (message)
		{
		case GFX_MESSAGE_TYPE::GFX_SLIDE:
			{
				mu_uint32 maxIndex = _Members.size() <= MAX_MEMBER_RENDER ? 0 : _Members.size() - MAX_MEMBER_RENDER;
				mu_uint32 newIndex = (mu_uint32)(_MemberScrollBar->GetCurrentValue() * (mu_float)maxIndex);
				
				if (newIndex != _MemberListIndex)
				{
					_MemberListIndex = newIndex;
					ConfigureMemberList();
				}
			}
			return true;
		}
	}
	else if (objectid == GAMEUI::MESSAGE_GUILDWINDOW_ALLIANCE_SCROLLBAR)
	{
		switch (message)
		{
		case GFX_MESSAGE_TYPE::GFX_SLIDE:
			{
				mu_uint32 maxIndex = _Alliances.size() <= MAX_ALLIANCE_RENDER ? 0 : _Members.size() - MAX_ALLIANCE_RENDER;
				mu_uint32 newIndex = (mu_uint32)(_AllianceScrollBar->GetCurrentValue() * (mu_float)maxIndex);

				if (newIndex != _AllianceListIndex)
				{
					_AllianceListIndex = newIndex;
					ConfigureAllianceList();
				}
			}
			return true;
		}
	}

	return false;
}

void UIGuildWindow::SetMarkTexture(MUTexture *texture)
{
	_MarkBox->SetTexture(texture);
}

void UIGuildWindow::SetGuildNotice(const mu_string &notice)
{
	_AnnTextBox->ClearAllText();
	_AnnTextBox->SetText(notice.c_str());
}