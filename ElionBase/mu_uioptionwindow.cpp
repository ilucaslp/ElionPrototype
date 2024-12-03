#include "stdafx.h"
#include "mu_uioptionwindow.h"
#include "mu_uioptionwindow_defines.h"

UIOptionWindow::UIOptionWindow(UISceneManager *manager) : UIBase()
{
	if (manager != nullptr)
	{
		manager->AddGFxObject(this);
	}

	_SceneManager = manager;

 	UIBase::SetAspectRatio(SCREEN_WIDTH, SCREEN_HEIGHT);
 	UIBase::SetPosition(OPTIONWINDOW_POSX, OPTIONWINDOW_POSY);
	UIBase::SetDimensions(OPTIONWINDOW_WIDTH, OPTIONWINDOW_HEIGHT);

	SetGfxFlag(GFX_FLAG::GFX_HIDDEN);

	// Initialize
	{
		_Background = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::LOGIN_SERVER_EX03], SCREEN_WIDTH, SCREEN_HEIGHT, BACK_POSX, BACK_POSY, BACK_WIDTH, OPTIONWINDOW_HEIGHT, BACK_UVBEGIN, BACK_UVEND);
		_Background->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_TopFrame = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::LOGIN_SERVER_EX01], SCREEN_WIDTH, SCREEN_HEIGHT, TOP_POSX, TOP_POSY, TOP_WIDTH, TOP_HEIGHT, EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f));
		_TopFrame->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_BottomFrame = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::LOGIN_SERVER_EX01], SCREEN_WIDTH, SCREEN_HEIGHT, BOTTOM_POSX, BOTTOM_POSY, BOTTOM_WIDTH, BOTTOM_HEIGHT, EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f));
		_BottomFrame->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_LeftFrame = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::LOGIN_SERVER_EX02], SCREEN_WIDTH, SCREEN_HEIGHT, LEFT_POSX, LEFT_POSY, LEFT_WIDTH, LEFT_HEIGHT, EVector2(0.0f, 0.0f), EVector2(0.5f, 1.0f));
		_LeftFrame->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_RightFrame = new UISprite(manager, this, &INTERFACES::Data[INTERFACES::LOGIN_SERVER_EX02], SCREEN_WIDTH, SCREEN_HEIGHT, RIGHT_POSX, RIGHT_POSY, RIGHT_WIDTH, RIGHT_HEIGHT, EVector2(0.5f, 0.0f), EVector2(1.0f, 1.0f));
		_RightFrame->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);

		const mu_text *tabsText[MAX_TAB] =
		{
			_T("General"),
			_T("Video"),
			_T("Audio"),
		};

		for (mu_uint32 n = 0; n < MAX_TAB; ++n)
		{
			_TabButtons[n] = new UIRadioButton(OPTIONUI::MESSAGE_OPTION_TAB_BEGIN + n, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, TABBUTTON_POSX(n), TABBUTTON_POSY, TABBUTTON_WIDTH, TABBUTTON_HEIGHT);

			_TabButtons[n]->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
			_TabButtons[n]->SetStateData(UIRadioButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::LOGIN_SERVERLIST_BACKGROUND], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.25f), MU::MakeARGB(200, 200, 200, 255)));
			_TabButtons[n]->SetStateData(UIRadioButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::LOGIN_SERVERLIST_BACKGROUND], EVector2(0.0f, 0.25f), EVector2(1.0f, 0.50f), MU::MakeARGB(255, 255, 255, 255)));
			_TabButtons[n]->SetStateData(UIRadioButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::LOGIN_SERVERLIST_BACKGROUND], EVector2(0.0f, 0.50f), EVector2(1.0f, 0.75f), MU::MakeARGB(200, 200, 200, 255)));
			_TabButtons[n]->SetStateData(UIRadioButton::UI_PRESSED_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::LOGIN_SERVERLIST_BACKGROUND], EVector2(0.0f, 0.25f), EVector2(1.0f, 0.50f), MU::MakeARGB(255, 255, 255, 255)));

			_TabButtons[n]->SetStateText(UIRadioButton::UI_DISABLED, FONT_NORMAL, tabsText[n], MU::MakeARGB(100, 100, 100, 255));
			_TabButtons[n]->SetStateText(UIRadioButton::UI_NORMAL, FONT_NORMAL, tabsText[n], MU::MakeARGB(150, 150, 150, 255));
			_TabButtons[n]->SetStateText(UIRadioButton::UI_OVER, FONT_NORMAL, tabsText[n], MU::MakeARGB(255, 255, 255, 255));
			_TabButtons[n]->SetStateText(UIRadioButton::UI_PRESSED, FONT_NORMAL, tabsText[n], MU::MakeARGB(200, 200, 200, 255));
			_TabButtons[n]->SetStateText(UIRadioButton::UI_PRESSED_OVER, FONT_NORMAL, tabsText[n], MU::MakeARGB(255, 255, 255, 255));
		}

		_ApplyBtn = new UIButton(OPTIONUI::MESSAGE_OPTION_APPLY, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, APPLYBUTTON_POSX, APPLYBUTTON_POSY, BUTTONNS_WIDTH, BUTTONNS_HEIGHT);
		_ApplyBtn->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_ApplyBtn->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTYNS_BUTTON], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(100, 100, 100, 255)));
		_ApplyBtn->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTYNS_BUTTON], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(255, 255, 255, 255)));
		_ApplyBtn->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTYNS_BUTTON], TUV_THREE_OVER_BEGIN, TUV_THREE_OVER_END, MU::MakeARGB(255, 255, 255, 255)));
		_ApplyBtn->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTYNS_BUTTON], TUV_THREE_PRESSED_BEGIN, TUV_THREE_PRESSED_END, MU::MakeARGB(255, 255, 255, 255)));
		_ApplyBtn->SetStateText(UIButton::UI_DISABLED, FONT_NORMAL, APPLYBUTTON_TEXT, MU::MakeARGB(100, 100, 100, 255));
		_ApplyBtn->SetStateText(UIButton::UI_NORMAL, FONT_NORMAL, APPLYBUTTON_TEXT, MU::MakeARGB(200, 200, 200, 255));
		_ApplyBtn->SetStateText(UIButton::UI_OVER, FONT_NORMAL, APPLYBUTTON_TEXT, MU::MakeARGB(255, 255, 255, 255));
		_ApplyBtn->SetStateText(UIButton::UI_PRESSED, FONT_NORMAL, APPLYBUTTON_TEXT, MU::MakeARGB(150, 150, 150, 255));

		_CloseBtn = new UIButton(OPTIONUI::MESSAGE_OPTION_CLOSE, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, CLOSEBUTTON_POSX, CLOSEBUTTON_POSY, BUTTONNS_WIDTH, BUTTONNS_HEIGHT);
		_CloseBtn->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_CloseBtn->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTYNS_BUTTON], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(100, 100, 100, 255)));
		_CloseBtn->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTYNS_BUTTON], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(255, 255, 255, 255)));
		_CloseBtn->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTYNS_BUTTON], TUV_THREE_OVER_BEGIN, TUV_THREE_OVER_END, MU::MakeARGB(255, 255, 255, 255)));
		_CloseBtn->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTYNS_BUTTON], TUV_THREE_PRESSED_BEGIN, TUV_THREE_PRESSED_END, MU::MakeARGB(255, 255, 255, 255)));
		_CloseBtn->SetStateText(UIButton::UI_DISABLED, FONT_NORMAL, CLOSEBUTTON_TEXT, MU::MakeARGB(100, 100, 100, 255));
		_CloseBtn->SetStateText(UIButton::UI_NORMAL, FONT_NORMAL, CLOSEBUTTON_TEXT, MU::MakeARGB(200, 200, 200, 255));
		_CloseBtn->SetStateText(UIButton::UI_OVER, FONT_NORMAL, CLOSEBUTTON_TEXT, MU::MakeARGB(255, 255, 255, 255));
		_CloseBtn->SetStateText(UIButton::UI_PRESSED, FONT_NORMAL, CLOSEBUTTON_TEXT, MU::MakeARGB(150, 150, 150, 255));

		_AutoAttackBtn = new UIToggleButton(OPTIONUI::MESSAGE_OPTION_WINDOWMODE_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, ADVANCEDBOX_WINDOWMODE_POSX, ADVANCEDBOX_WINDOWMODE_POSY, ADVANCEDBOX_WINDOWMODE_WIDTH, ADVANCEDBOX_WINDOWMODE_HEIGHT);
		_AutoAttackBtn->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_AutoAttackBtn->SetStateData(UIToggleButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::OPTION_CHECK], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(200, 200, 200, 255)));
		_AutoAttackBtn->SetStateData(UIToggleButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::OPTION_CHECK], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_AutoAttackBtn->SetStateData(UIToggleButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::OPTION_CHECK], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(200, 200, 200, 255)));
		_AutoAttackBtn->SetStateData(UIToggleButton::UI_PRESSED_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::OPTION_CHECK], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));

		_ResolutionScrollBar = new UIScrollBar(OPTIONUI::MESSAGE_OPTION_RESOLUTION_SCROLLBAR, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, RESOLUTIONSCROLLBAR_POSX, RESOLUTIONSCROLLBAR_POSY, UIScrollBar::ORI_VERTICAL);
		_ResolutionScrollBar->ConfigureTop(&INTERFACES::Data[INTERFACES::SCROLLBAR_TOP], EBlendState::eAlphaSrc_InvAlphaSrc, RESOLUTIONSCROLLBAR_BACKWIDTH, RESOLUTIONSCROLLBAR_TOP_BOTTOM_PART_HEIGHT);
		_ResolutionScrollBar->ConfigureMiddle(&INTERFACES::Data[INTERFACES::SCROLLBAR_MIDDLE], EBlendState::eAlphaSrc_InvAlphaSrc, RESOLUTIONSCROLLBAR_BACKWIDTH, RESOLUTIONSCROLLBAR_LINESHEIGHT);
		_ResolutionScrollBar->ConfigureBottom(&INTERFACES::Data[INTERFACES::SCROLLBAR_BOTTOM], EBlendState::eAlphaSrc_InvAlphaSrc, RESOLUTIONSCROLLBAR_BACKWIDTH, RESOLUTIONSCROLLBAR_TOP_BOTTOM_PART_HEIGHT);
		_ResolutionScrollBar->ConfigureScroll(EBlendState::eAlphaSrc_InvAlphaSrc, RESOLUTIONSCROLLBAR_BTN_WIDTH, RESOLUTIONSCROLLBAR_BTN_HEIGHT, true);

		_ResolutionScrollBar->SetStateData(UIScrollBar::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::SCROLLBAR_SLIDE_OFF], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_ResolutionScrollBar->SetStateData(UIScrollBar::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::SCROLLBAR_SLIDE_ON], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_ResolutionScrollBar->SetStateData(UIScrollBar::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::SCROLLBAR_SLIDE_ON], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_ResolutionScrollBar->SetStateData(UIScrollBar::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::SCROLLBAR_SLIDE_ON], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(178, 178, 178, 255)));

		_ResolutionScrollBar->ConfigureScrollInfo(0.0f, 1.0f, 1.0f);
		_ResolutionScrollBar->ConfigureScrollDimension(RESOLUTIONSCROLLBAR_LINESHEIGHT);
		_ResolutionScrollBar->ChangeState(UIScrollBar::UI_DISABLED);
		_ResolutionScrollBar->SetMessageInheritor(this);

		_ResolutionScrollBar->ApplyChanges();

		for (mu_uint32 n = 0; n < MAX_RESOLUTION_RENDER; ++n)
		{
			_ResolutionListBtn[n] = new UIRadioButton(OPTIONUI::MESSAGE_OPTION_RESOLUTION_BEGIN + n, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, RESOLUTIONBOX_ITEM_POSX, RESOLUTIONBOX_ITEM_POSY(n), RESOLUTIONBOX_ITEM_WIDTH, RESOLUTIONBOX_ITEM_HEIGHT);
			_ResolutionListBtn[n]->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
			_ResolutionListBtn[n]->SetStateData(UIRadioButton::UI_NORMAL, UI_STATE_DATA(USE_COLOR_STATE, EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(0, 0, 0, 0)));
			_ResolutionListBtn[n]->SetStateData(UIRadioButton::UI_OVER, UI_STATE_DATA(USE_COLOR_STATE, EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(150, 150, 150, 220)));
			_ResolutionListBtn[n]->SetStateData(UIRadioButton::UI_PRESSED, UI_STATE_DATA(USE_COLOR_STATE, EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(150, 150, 150, 180)));
			_ResolutionListBtn[n]->SetStateData(UIRadioButton::UI_PRESSED_OVER, UI_STATE_DATA(USE_COLOR_STATE, EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(150, 150, 150, 220)));
		}

		_WindowModeBtn = new UIToggleButton(OPTIONUI::MESSAGE_OPTION_WINDOWMODE_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, ADVANCEDBOX_WINDOWMODE_POSX, ADVANCEDBOX_WINDOWMODE_POSY, ADVANCEDBOX_WINDOWMODE_WIDTH, ADVANCEDBOX_WINDOWMODE_HEIGHT);
		_WindowModeBtn->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_WindowModeBtn->SetStateData(UIToggleButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::OPTION_CHECK], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(200, 200, 200, 255)));
		_WindowModeBtn->SetStateData(UIToggleButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::OPTION_CHECK], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_WindowModeBtn->SetStateData(UIToggleButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::OPTION_CHECK], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(200, 200, 200, 255)));
		_WindowModeBtn->SetStateData(UIToggleButton::UI_PRESSED_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::OPTION_CHECK], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));

		_AntiAliasingBtn = new UIToggleButton(OPTIONUI::MESSAGE_OPTION_ANTIALIASING_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, ADVANCEDBOX_AA_POSX, ADVANCEDBOX_AA_POSY, ADVANCEDBOX_AA_WIDTH, ADVANCEDBOX_AA_HEIGHT);
		_AntiAliasingBtn->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_AntiAliasingBtn->SetStateData(UIToggleButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::OPTION_CHECK], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(200, 200, 200, 255)));
		_AntiAliasingBtn->SetStateData(UIToggleButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::OPTION_CHECK], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_AntiAliasingBtn->SetStateData(UIToggleButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::OPTION_CHECK], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(200, 200, 200, 255)));
		_AntiAliasingBtn->SetStateData(UIToggleButton::UI_PRESSED_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::OPTION_CHECK], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));

		_VerticalSyncBtn = new UIToggleButton(OPTIONUI::MESSAGE_OPTION_VERTICALSYNC_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, ADVANCEDBOX_VERTICALSYNC_POSX, ADVANCEDBOX_VERTICALSYNC_POSY, ADVANCEDBOX_VERTICALSYNC_WIDTH, ADVANCEDBOX_VERTICALSYNC_HEIGHT);
		_VerticalSyncBtn->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_VerticalSyncBtn->SetStateData(UIToggleButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::OPTION_CHECK], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(200, 200, 200, 255)));
		_VerticalSyncBtn->SetStateData(UIToggleButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::OPTION_CHECK], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_VerticalSyncBtn->SetStateData(UIToggleButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::OPTION_CHECK], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(200, 200, 200, 255)));
		_VerticalSyncBtn->SetStateData(UIToggleButton::UI_PRESSED_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::OPTION_CHECK], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));

		_FpsLimitBtn = new UIToggleButton(OPTIONUI::MESSAGE_OPTION_FPSLIMIT_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, ADVANCEDBOX_FPSLIMIT_POSX, ADVANCEDBOX_FPSLIMIT_POSY, ADVANCEDBOX_FPSLIMIT_WIDTH, ADVANCEDBOX_FPSLIMIT_HEIGHT);
		_FpsLimitBtn->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_FpsLimitBtn->SetStateData(UIToggleButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::OPTION_CHECK], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(200, 200, 200, 255)));
		_FpsLimitBtn->SetStateData(UIToggleButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::OPTION_CHECK], EVector2(0.0f, 0.5f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_FpsLimitBtn->SetStateData(UIToggleButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::OPTION_CHECK], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(200, 200, 200, 255)));
		_FpsLimitBtn->SetStateData(UIToggleButton::UI_PRESSED_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::OPTION_CHECK], EVector2(0.0f, 0.0f), EVector2(1.0f, 0.5f), MU::MakeARGB(255, 255, 255, 255)));

		_ShadowTypeBtn = new UIButton(OPTIONUI::MESSAGE_OPTION_SHADOWTYPE_BUTTON, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, ADVANCEDBOX_SHADOWTYPE_POSX, ADVANCEDBOX_SHADOWTYPE_POSY, BUTTONSHADOW_WIDTH, BUTTONSHADOW_HEIGHT);
		_ShadowTypeBtn->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc);
		_ShadowTypeBtn->SetStateData(UIButton::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTYNS_BUTTON], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(100, 100, 100, 255)));
		_ShadowTypeBtn->SetStateData(UIButton::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTYNS_BUTTON], TUV_THREE_NORMAL_BEGIN, TUV_THREE_NORMAL_END, MU::MakeARGB(255, 255, 255, 255)));
		_ShadowTypeBtn->SetStateData(UIButton::UI_OVER, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTYNS_BUTTON], TUV_THREE_OVER_BEGIN, TUV_THREE_OVER_END, MU::MakeARGB(255, 255, 255, 255)));
		_ShadowTypeBtn->SetStateData(UIButton::UI_PRESSED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::EMPTYNS_BUTTON], TUV_THREE_PRESSED_BEGIN, TUV_THREE_PRESSED_END, MU::MakeARGB(255, 255, 255, 255)));
		_ShadowTypeBtn->SetStateText(UIButton::UI_DISABLED, FONT_NORMAL, _T(""), MU::MakeARGB(100, 100, 100, 255));
		_ShadowTypeBtn->SetStateText(UIButton::UI_NORMAL, FONT_NORMAL, _T(""), MU::MakeARGB(200, 200, 200, 255));
		_ShadowTypeBtn->SetStateText(UIButton::UI_OVER, FONT_NORMAL, _T(""), MU::MakeARGB(255, 255, 255, 255));
		_ShadowTypeBtn->SetStateText(UIButton::UI_PRESSED, FONT_NORMAL, _T(""), MU::MakeARGB(150, 150, 150, 255));

		_MusicVolumeSlider = new UISliderBar(OPTIONUI::MESSAGE_OPTION_MUSICVOLUME_SLIDER, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, MUSICBOX_SLIDER_POSX, MUSICBOX_SLIDER_POSY, MUSICBOX_SLIDER_WIDTH, MUSICBOX_SLIDER_HEIGHT, UISliderBar::ORI_HORIZONTAL);
		_MusicVolumeSlider->ConfigureBack(&INTERFACES::Data[INTERFACES::OPTION_VOLUME_BACK], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f));
		_MusicVolumeSlider->SetLockUV(false);
		_MusicVolumeSlider->SetStateData(UISliderBar::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::OPTION_VOLUME_COLOR], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(100, 100, 100, 255)));
		_MusicVolumeSlider->SetStateData(UISliderBar::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::OPTION_VOLUME_COLOR], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_MusicVolumeSlider->SetCurrentValue(MU_RESOURCE->GetMusicVolume());
		_MusicVolumeSlider->SetMessageInheritor(this);

		_SoundVolumeSlider = new UISliderBar(OPTIONUI::MESSAGE_OPTION_SOUNDVOLUME_SLIDER, manager, this, SCREEN_WIDTH, SCREEN_HEIGHT, SOUNDBOX_SLIDER_POSX, SOUNDBOX_SLIDER_POSY, SOUNDBOX_SLIDER_WIDTH, SOUNDBOX_SLIDER_HEIGHT, UISliderBar::ORI_HORIZONTAL);
		_SoundVolumeSlider->ConfigureBack(&INTERFACES::Data[INTERFACES::OPTION_VOLUME_BACK], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f));
		_SoundVolumeSlider->SetLockUV(false);
		_SoundVolumeSlider->SetStateData(UISliderBar::UI_DISABLED, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::OPTION_VOLUME_COLOR], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(100, 100, 100, 255)));
		_SoundVolumeSlider->SetStateData(UISliderBar::UI_NORMAL, UI_STATE_DATA(&INTERFACES::Data[INTERFACES::OPTION_VOLUME_COLOR], EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(255, 255, 255, 255)));
		_SoundVolumeSlider->SetCurrentValue(MU_RESOURCE->GetSoundVolume());
		_SoundVolumeSlider->SetMessageInheritor(this);

		_WhisperSound = false;
		_SlideHelp = true;
		_RenderLevel = 4;

		SetCurrentTab(TAB_VIDEO);

		Prepare();
	}
}

UIOptionWindow::~UIOptionWindow()
{

}

mu_uint32 UIOptionWindow::OnPreWindowMessage(const SDL_Event *event, mu_size *result)
{
	switch (_CurrentTab)
	{
	case TAB_VIDEO:
		{
			UIBase *focusObject = _SceneManager->GetFocused();
			if (focusObject == this ||
				(focusObject != nullptr &&
					focusObject->_Parent == this))
			{
				mu_int32 mouseX = MU_INPUT->GetMouseX();
				mu_int32 mouseY = MU_INPUT->GetMouseY();

				if (mouseY >= _ResolutionScrollBar->_NativeY &&
					mouseY <= _ResolutionScrollBar->_NativeY + _ResolutionScrollBar->_Height &&
					mouseX >= _ResolutionListBtn[0]->_NativeX &&
					mouseX <= _ResolutionListBtn[0]->_NativeX + _ResolutionListBtn[0]->_Width)
				{
					switch (event->type)
					{
					case SDL_MOUSEWHEEL:
						{
							if (_ResolutionScrollBar->GetState() != UIScrollBar::UI_DISABLED)
							{
								mu_uint32 maxIndex = _Resolutions.size() <= MAX_RESOLUTION_RENDER ? 0 : _Resolutions.size() - MAX_RESOLUTION_RENDER;
								mu_float mouseWheel = event->wheel.y;
								mu_float diffValue = mouseWheel * _ResolutionScrollBar->_DiffValue * -0.03f;
								_ResolutionScrollBar->ConfigureScrollInfo(_ResolutionScrollBar->_MinValue, _ResolutionScrollBar->_MaxValue, _ResolutionScrollBar->_CurrentValue + diffValue);

								mu_uint32 newIndex = (mu_uint32)(_ResolutionScrollBar->GetCurrentValue() * (mu_float)maxIndex);
								if (newIndex != _ResolutionListIndex)
								{
									_ResolutionListIndex = newIndex;
									ConfigureResolutionList();
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

	return GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE;
}

void UIOptionWindow::Update(mu_uint64 CurrentTime)
{

}

void UIOptionWindow::Draw()
{
	DrawFrames();
	DrawButtons();
	DrawTexts();
}

void UIOptionWindow::PostDraw()
{
}

void UIOptionWindow::DrawFrames()
{
	_Background->Draw();
	_LeftFrame->Draw();
	_RightFrame->Draw();
	_TopFrame->Draw();
	_BottomFrame->Draw();
}

void UIOptionWindow::DrawButtons()
{
	for (mu_uint32 n = 0; n < MAX_TAB; ++n)
	{
		_TabButtons[n]->Draw();
	}

	_ApplyBtn->Draw();
	_CloseBtn->Draw();

	switch (_CurrentTab)
	{
	case TAB_GENERAL:
		{
			_AutoAttackBtn->Draw();
		}
		break;

	case TAB_VIDEO:
		{
			mu_uint32 maxIndex = _Resolutions.size();
			if (maxIndex > MAX_RESOLUTION_RENDER) maxIndex = MAX_RESOLUTION_RENDER;
			for (mu_uint32 n = 0; n < maxIndex; ++n)
			{
				_ResolutionListBtn[n]->Draw();
			}

			_ResolutionScrollBar->Draw();

			_WindowModeBtn->Draw();
			_AntiAliasingBtn->Draw();
			_VerticalSyncBtn->Draw();
			_FpsLimitBtn->Draw();
			_ShadowTypeBtn->Draw();
		}
		break;

	case TAB_AUDIO:
		{
			_MusicVolumeSlider->Draw();
			_SoundVolumeSlider->Draw();
		}
		break;
	}
}

void UIOptionWindow::DrawTexts()
{
	mu_float screenX = _AspectWidth;
	mu_float screenY = _AspectHeight;
	mu_uint32 textColor = MU::MakeARGB(255, 255, 255, 255);
	mu_uint32 bgColor = MU::MakeARGB(0, 0, 0, 0);

	FONT_BOLD->DrawStringC2D(TEXTINDEX::OPTIONWINDOW_TITLE,
		MU_TO_WINDOW(_ViewX + OPTIONWINDOW_WIDTH * 0.5f, screenX),
		MU_TO_WINDOW(_ViewY + 15.0f, screenY),
		1.0f,
		ETA_CENTER,
		ETA_CENTER,
		0.0f,
		textColor,
		bgColor,
		_T("Options"));

	switch (_CurrentTab)
	{
	case TAB_GENERAL:
		{
			FONT_NORMAL->DrawStringC2D(TEXTINDEX::OPTIONWINDOW_AUTOATTACK_TITLE,
				MU_TO_WINDOW(ADVANCEDBOX_WINDOWMODE_TITLE_POSX, screenX),
				MU_TO_WINDOW(ADVANCEDBOX_WINDOWMODE_TITLE_POSY, screenY),
				1.0f,
				ETA_LEFT,
				ETA_CENTER,
				0.0f,
				textColor,
				bgColor,
				_T("Auto Attack"));
		}
		break;

	case TAB_VIDEO:
		{
			FONT_NORMAL->DrawStringC2D(TEXTINDEX::OPTIONWINDOW_RESOLUTION_TITLE,
				MU_TO_WINDOW(RESOLUTIONBOX_TITLE_POSX, screenX),
				MU_TO_WINDOW(RESOLUTIONBOX_TITLE_POSY, screenY),
				1.0f,
				ETA_LEFT,
				ETA_CENTER,
				0.0f,
				textColor,
				bgColor,
				_T("Resolution"));

			UIBase *focused = _SceneManager->GetFocused();
			mu_uint32 maxIndex = _Resolutions.size();
			if (maxIndex > MAX_RESOLUTION_RENDER) maxIndex = MAX_RESOLUTION_RENDER;
			auto it = _Resolutions.begin();
			for (mu_uint32 n = _ResolutionListIndex; n--; ++it);
			for (mu_uint32 n = 0; n < maxIndex; ++n, ++it)
			{
				UIResolutionData &res = *it;

				mu_boolean isSelected = (n + _ResolutionListIndex) == _ResolutionSelected || _ResolutionListBtn[n] == focused;
				mu_uint32 resColor = isSelected == true ? MU::MakeARGB(250, 240, 220, 255) : MU::MakeARGB(160, 150, 140, 255);

				FONT_NORMAL->DrawStringC2D(TEXTINDEX::OPTIONWINDOW_RESOLUTION_NAME_BEGIN + n,
					MU_TO_WINDOW(RESOLUTIONBOX_ITEM_POSX + 11.0f, screenX),
					MU_TO_WINDOW(RESOLUTIONBOX_ITEM_POSY(n) + RESOLUTIONBOX_ITEM_HEIGHT * 0.5f, screenY),
					1.0f,
					ETA_LEFT,
					ETA_CENTER,
					0.0f,
					resColor,
					bgColor,
					res.Name.c_str());
			}

			FONT_NORMAL->DrawStringC2D(TEXTINDEX::OPTIONWINDOW_ADVANCED_TITLE,
				MU_TO_WINDOW(ADVANCEDBOX_TITLE_POSX, screenX),
				MU_TO_WINDOW(ADVANCEDBOX_TITLE_POSY, screenY),
				1.0f,
				ETA_LEFT,
				ETA_CENTER,
				0.0f,
				textColor,
				bgColor,
				_T("Advanced"));

			FONT_NORMAL->DrawStringC2D(TEXTINDEX::OPTIONWINDOW_WINDOWMODE_TITLE,
				MU_TO_WINDOW(ADVANCEDBOX_WINDOWMODE_TITLE_POSX, screenX),
				MU_TO_WINDOW(ADVANCEDBOX_WINDOWMODE_TITLE_POSY, screenY),
				1.0f,
				ETA_LEFT,
				ETA_CENTER,
				0.0f,
				textColor,
				bgColor,
				_T("Window Mode"));

			FONT_NORMAL->DrawStringC2D(TEXTINDEX::OPTIONWINDOW_ANTIALIASING_TITLE,
				MU_TO_WINDOW(ADVANCEDBOX_AA_TITLE_POSX, screenX),
				MU_TO_WINDOW(ADVANCEDBOX_AA_TITLE_POSY, screenY),
				1.0f,
				ETA_LEFT,
				ETA_CENTER,
				0.0f,
				textColor,
				bgColor,
				_T("Enable Anti-Aliasing"));

			FONT_NORMAL->DrawStringC2D(TEXTINDEX::OPTIONWINDOW_VERTICALSYNC_TITLE,
				MU_TO_WINDOW(ADVANCEDBOX_VERTICALSYNC_TITLE_POSX, screenX),
				MU_TO_WINDOW(ADVANCEDBOX_VERTICALSYNC_TITLE_POSY, screenY),
				1.0f,
				ETA_LEFT,
				ETA_CENTER,
				0.0f,
				textColor,
				bgColor,
				_T("Enable Vertical-Sync"));

			FONT_NORMAL->DrawStringC2D(TEXTINDEX::OPTIONWINDOW_FPSLIMIT_TITLE,
				MU_TO_WINDOW(ADVANCEDBOX_FPSLIMIT_TITLE_POSX, screenX),
				MU_TO_WINDOW(ADVANCEDBOX_FPSLIMIT_TITLE_POSY, screenY),
				1.0f,
				ETA_LEFT,
				ETA_CENTER,
				0.0f,
				textColor,
				bgColor,
				_T("Enable FPS Limit"));

			FONT_NORMAL->DrawStringC2D(TEXTINDEX::OPTIONWINDOW_SHADOWTYPE_TITLE,
				MU_TO_WINDOW(ADVANCEDBOX_SHADOWTYPE_TITLE_POSX, screenX),
				MU_TO_WINDOW(ADVANCEDBOX_SHADOWTYPE_TITLE_POSY, screenY),
				1.0f,
				ETA_LEFT,
				ETA_CENTER,
				0.0f,
				textColor,
				bgColor,
				_T("Select shadow type"));
		}
		break;

	case TAB_AUDIO:
		{
			FONT_NORMAL->DrawStringC2D(TEXTINDEX::OPTIONWINDOW_MUSICVOLUME_TITLE,
				MU_TO_WINDOW(MUSICBOX_TITLE_POSX, screenX),
				MU_TO_WINDOW(MUSICBOX_TITLE_POSY, screenY),
				1.0f,
				ETA_LEFT,
				ETA_CENTER,
				0.0f,
				textColor,
				bgColor,
				_T("Music"));

			mu_text textBuffer[256];

			mu_sprintf_s(textBuffer, mu_countof(textBuffer), _T("%d%%"), (mu_int32)(_MusicVolumeSlider->GetCurrentValue() * 100.0f));

			FONT_NORMAL->DrawStringC2D(TEXTINDEX::OPTIONWINDOW_MUSICVOLUME_LABEL,
				MU_TO_WINDOW(MUSICBOX_LABEL_POSX, screenX),
				MU_TO_WINDOW(MUSICBOX_LABEL_POSY, screenY),
				1.0f,
				ETA_LEFT,
				ETA_CENTER,
				0.0f,
				textColor,
				bgColor,
				textBuffer);

			FONT_NORMAL->DrawStringC2D(TEXTINDEX::OPTIONWINDOW_SOUNDVOLUME_TITLE,
				MU_TO_WINDOW(SOUNDBOX_TITLE_POSX, screenX),
				MU_TO_WINDOW(SOUNDBOX_TITLE_POSY, screenY),
				1.0f,
				ETA_LEFT,
				ETA_CENTER,
				0.0f,
				textColor,
				bgColor,
				_T("Sound"));

			mu_sprintf_s(textBuffer, mu_countof(textBuffer), _T("%d%%"), (mu_int32)(_SoundVolumeSlider->GetCurrentValue() * 100.0f));

			FONT_NORMAL->DrawStringC2D(TEXTINDEX::OPTIONWINDOW_SOUNDVOLUME_LABEL,
				MU_TO_WINDOW(SOUNDBOX_LABEL_POSX, screenX),
				MU_TO_WINDOW(SOUNDBOX_LABEL_POSY, screenY),
				1.0f,
				ETA_LEFT,
				ETA_CENTER,
				0.0f,
				textColor,
				bgColor,
				textBuffer);
		}
		break;
	}

	FONT_NORMAL->DrawStringC2D(TEXTINDEX::OPTIONWINDOW_WARNING,
		MU_TO_WINDOW(WARNING_POSX, screenX),
		MU_TO_WINDOW(WARNING_POSY, screenY),
		1.0f,
		ETA_LEFT,
		ETA_TOP,
		0.0f,
		textColor,
		bgColor,
		_T("- Press [Apply] button for keep changes\n- If close options without [Apply] changes\n   sound and music volume will back to original configuration"));
}

UIBase *UIOptionWindow::GetFocus(mu_int32 mouseX, mu_int32 mouseY)
{
	UIBase *focus = UIBase::GetFocus(mouseX, mouseY);

	if (focus != nullptr)
	{
		UIBase *object = nullptr;

		object = _ApplyBtn->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}

		object = _CloseBtn->GetFocus(mouseX, mouseY);

		if (object != nullptr)
		{
			return object;
		}

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
		case TAB_GENERAL:
			{
				object = _AutoAttackBtn->GetFocus(mouseX, mouseY);

				if (object != nullptr)
				{
					return object;
				}
			}
			break;

		case TAB_VIDEO:
			{
				object = _ResolutionScrollBar->GetFocus(mouseX, mouseY);

				if (object != nullptr)
				{
					return object;
				}

				mu_uint32 maxIndex = _Resolutions.size();
				if (maxIndex > MAX_RESOLUTION_RENDER) maxIndex = MAX_RESOLUTION_RENDER;
				for (mu_uint32 n = 0; n < maxIndex; ++n)
				{
					object = _ResolutionListBtn[n]->GetFocus(mouseX, mouseY);

					if (object != nullptr)
					{
						return object;
					}
				}

				object = _WindowModeBtn->GetFocus(mouseX, mouseY);

				if (object != nullptr)
				{
					return object;
				}
				
				object = _AntiAliasingBtn->GetFocus(mouseX, mouseY);

				if (object != nullptr)
				{
					return object;
				}

				object = _VerticalSyncBtn->GetFocus(mouseX, mouseY);

				if (object != nullptr)
				{
					return object;
				}

				object = _FpsLimitBtn->GetFocus(mouseX, mouseY);

				if (object != nullptr)
				{
					return object;
				}

				object = _ShadowTypeBtn->GetFocus(mouseX, mouseY);

				if (object != nullptr)
				{
					return object;
				}
			}
			break;

		case TAB_AUDIO:
			{
				object = _MusicVolumeSlider->GetFocus(mouseX, mouseY);

				if (object != nullptr)
				{
					return object;
				}

				object = _SoundVolumeSlider->GetFocus(mouseX, mouseY);

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

mu_boolean UIOptionWindow::OnGFxMessage(mu_uint32 objectid, mu_uint32 message, mu_size message_data)
{
	if (objectid == OPTIONUI::MESSAGE_OPTION_RESOLUTION_SCROLLBAR)
	{
		switch (message)
		{
		case GFX_MESSAGE_TYPE::GFX_SLIDE:
			{
				mu_uint32 maxIndex = _Resolutions.size() <= MAX_RESOLUTION_RENDER ? 0 : _Resolutions.size() - MAX_RESOLUTION_RENDER;
				mu_uint32 newIndex = (mu_uint32)(_ResolutionScrollBar->GetCurrentValue() * (mu_float)maxIndex);

				if (newIndex != _ResolutionListIndex)
				{
					_ResolutionListIndex = newIndex;
					ConfigureResolutionList();
				}
			}
			return true;
		}
	}
	else if (objectid == OPTIONUI::MESSAGE_OPTION_MUSICVOLUME_SLIDER)
	{
		switch (message)
		{
		case GFX_MESSAGE_TYPE::GFX_SLIDE:
			{
				MU_RESOURCE->SetMusicVolume(_MusicVolumeSlider->GetCurrentValue());
			}
			return true;
		}
	}
	else if (objectid == OPTIONUI::MESSAGE_OPTION_SOUNDVOLUME_SLIDER)
	{
		switch (message)
		{
		case GFX_MESSAGE_TYPE::GFX_SLIDE:
			{
				MU_RESOURCE->SetSoundVolume(_SoundVolumeSlider->GetCurrentValue());
			}
			return true;
		}
	}

	return false;
}

void UIOptionWindow::PrepareCurrentSettings()
{
	_CurrentSettings.IsAutoAttack = MU_RESOURCE->IsAutoAttack();

	_CurrentSettings.Width = MU_RESOURCE->GetWindowWidth();
	_CurrentSettings.Height = MU_RESOURCE->GetWindowHeight();
	_CurrentSettings.WindowMode = MU_RESOURCE->IsWindowMode();
	_CurrentSettings.AntiAliasing = MU_RESOURCE->IsAntialiasing();
	_CurrentSettings.VerticalSync = MU_RESOURCE->IsVerticalSync();
	_CurrentSettings.FpsLimit = MU_GLOBALTIMER.GetLimitFPS();
	_CurrentShadowType = _CurrentSettings.ShadowType = ECastEnum(MU_RESOURCE->GetShadowType());

	_CurrentSettings.MusicVolume = MU_RESOURCE->GetMusicVolume();
	_CurrentSettings.SoundVolume = MU_RESOURCE->GetSoundVolume();

	for (mu_int32 n = 0; n < _Resolutions.size(); ++n)
	{
		UIResolutionData &res = _Resolutions[n];

		if (_CurrentSettings.Width == res.Width &&
			_CurrentSettings.Height == res.Height)
		{
			_ResolutionSelected = n;
			break;
		}
	}

	_AutoAttackBtn->ChangeState(_CurrentSettings.IsAutoAttack == true ? UIToggleButton::UI_PRESSED : UIToggleButton::UI_NORMAL);

	_WindowModeBtn->ChangeState(_CurrentSettings.WindowMode == true ? UIToggleButton::UI_PRESSED : UIToggleButton::UI_NORMAL);
	_AntiAliasingBtn->ChangeState(_CurrentSettings.AntiAliasing == true ? UIToggleButton::UI_PRESSED : UIToggleButton::UI_NORMAL);
	_VerticalSyncBtn->ChangeState(_CurrentSettings.VerticalSync == true ? UIToggleButton::UI_PRESSED : UIToggleButton::UI_NORMAL);
	_FpsLimitBtn->ChangeState(_CurrentSettings.FpsLimit == true ? UIToggleButton::UI_PRESSED : UIToggleButton::UI_NORMAL);

	ConfigureShadowButton(_CurrentShadowType);

	ConfigureResolutionList();

	_MusicVolumeSlider->SetCurrentValue(MU_RESOURCE->GetMusicVolume());
	_SoundVolumeSlider->SetCurrentValue(MU_RESOURCE->GetSoundVolume());
}

mu_boolean UIOptionWindow::OnApplyNewResolution(UIOptionData &newSettings)
{
	if (newSettings.Width != MU_RESOURCE->GetWindowWidth() ||
		newSettings.Height != MU_RESOURCE->GetWindowHeight())
	{
		mu_int32 displayIndex = SDL_GetWindowDisplayIndex(MU_WINDOW->GetSDLWindow());
		if (displayIndex == -1)
		{
			return false;
		}

		SDL_DisplayMode newRes = { 0 };
		if (MU_RESOURCE->IsWindowMode() == false)
		{
			SDL_DisplayMode mode = { 0 };
			for (mu_int32 modelIndex = 0; SDL_GetDisplayMode(displayIndex, modelIndex, &mode) == 0; ++modelIndex)
			{
				if (mode.w == newSettings.Width &&
					mode.h == newSettings.Height)
				{
					if (newRes.w == 0 ||
						newRes.refresh_rate < mode.refresh_rate)
					{
						mu_memcpy(&newRes, &mode, sizeof(newRes));
					}
				}
			}

			if (mode.w == 0)
			{
				return false;
			}
		}
		else
		{
			newRes.w = newSettings.Width;
			newRes.h = newSettings.Height;
		}

		if (newRes.w != MU_RESOURCE->GetWindowWidth() ||
			newRes.h != MU_RESOURCE->GetWindowHeight())
		{
			MU_RESOURCE->SetWindowWidth(newRes.w);
			MU_RESOURCE->SetWindowHeight(newRes.h);

			EVideoBackend::RequestResizeWindow();
		}
	}

	return true;
}

void UIOptionWindow::OnApplySettings()
{
	UIOptionData newSettings;

	UIResolutionData &res = _Resolutions[_ResolutionSelected];

#if defined(__ANDROID__)
	newSettings.Width = _CurrentSettings.Width;
	newSettings.Height = _CurrentSettings.Height;
	newSettings.WindowMode = false;
#else
	if (_WindowModeBtn->GetState() >= UIToggleButton::UI_PRESSED)
	{
		newSettings.Width = res.Width;
		newSettings.Height = res.Height;
		newSettings.WindowMode = true;
	}
	else
	{
		SDL_DisplayMode dm = {};
		if (SDL_GetDesktopDisplayMode(0, &dm) != 0)
		{
			_WindowModeBtn->ChangeState(UIToggleButton::UI_PRESSED);
			newSettings.Width = res.Width;
			newSettings.Height = res.Height;
			newSettings.WindowMode = true;
		}
		else
		{
			newSettings.Width = dm.w;
			newSettings.Height = dm.h;
			newSettings.WindowMode = false;
		}
	}
#endif

	newSettings.IsAutoAttack = _AutoAttackBtn->GetState() >= UIToggleButton::UI_PRESSED;
	newSettings.AntiAliasing = _AntiAliasingBtn->GetState() >= UIToggleButton::UI_PRESSED;
	newSettings.VerticalSync = _VerticalSyncBtn->GetState() >= UIToggleButton::UI_PRESSED;
	newSettings.FpsLimit = _FpsLimitBtn->GetState() >= UIToggleButton::UI_PRESSED;
	newSettings.ShadowType = _CurrentShadowType;
	
	if (mu_memcmp(&newSettings, &_CurrentSettings, sizeof(_CurrentSettings)) != 0)
	{
		// Process what to do
#if defined(WIN32) || defined(WIN64)
		if (newSettings.VerticalSync != _CurrentSettings.VerticalSync)
		{
			MU_RESOURCE->SetVerticalSync(newSettings.VerticalSync);
			EVideoBackend::RequestReloadVerticalSync();
		}
#endif

		if (newSettings.IsAutoAttack != _CurrentSettings.IsAutoAttack)
		{
			MU_RESOURCE->SetAutoAttack(newSettings.IsAutoAttack);
		}

#if defined(WIN32) || defined(WIN64)
		if (newSettings.WindowMode != _CurrentSettings.WindowMode)
		{
			MU_RESOURCE->SetWindowMode(newSettings.WindowMode);
			EVideoBackend::RequestChangeWindowMode();
		}
#endif

		if (newSettings.ShadowType != _CurrentSettings.ShadowType)
		{
			if (_CurrentShadowType < static_cast<mu_uint8>(EShadowType::eMax))
			{
				if (_CurrentSettings.ShadowType != _CurrentShadowType)
				{
					EVideoBackend::RequestReloadShaders();
					const mu_uint32 oldShadowCategory = static_cast<EShadowType>(_CurrentSettings.ShadowType) >= EShadowType::eShadowMappingBegin;
					const mu_uint32 newShadowCategory = static_cast<EShadowType>(_CurrentShadowType) >= EShadowType::eShadowMappingBegin;
					if (oldShadowCategory != newShadowCategory)
					{
						EVideoBackend::RequestReloadShadowMap();
					}
				}

				MU_RESOURCE->SetShadowType(static_cast<EShadowType>(_CurrentShadowType));
			}
		}

		if (MU_RESOURCE->IsAntialiasing() != newSettings.AntiAliasing)
		{
			MU_RESOURCE->SetAntialiasing(newSettings.AntiAliasing);
			EVideoBackend::RequestReloadMSAA();
		}

		if (newSettings.Width != _CurrentSettings.Width ||
			newSettings.Height != _CurrentSettings.Height)
		{
			OnApplyNewResolution(newSettings);
		}

		if (newSettings.FpsLimit != _CurrentSettings.FpsLimit)
		{
			MU_GLOBALTIMER.SetLimitFPS(newSettings.FpsLimit);
		}

		PrepareCurrentSettings();
	}
}

void UIOptionWindow::Prepare()
{
	ClearResolutions();

	mu_int32 displayIndex = SDL_GetWindowDisplayIndex(MU_WINDOW->GetSDLWindow());
	if (displayIndex != -1)
	{
		std::set<mu_uint64> insertedRes;

		SDL_DisplayMode mode = { 0 };
		for (mu_int32 modelIndex = 0; SDL_GetDisplayMode(displayIndex, modelIndex, &mode) == 0; ++modelIndex)
		{
			UIResolutionData Res;

			Res.Width = mode.w;
			Res.Height = mode.h;

			if (insertedRes.insert(Res.uniqueident).second == true)
			{
				mu_text nameBuffer[256];

				mu_sprintf_s(nameBuffer, mu_countof(nameBuffer), _T("%ux%u"), Res.Width, Res.Height);

				Res.Name = nameBuffer;
				_Resolutions.push_back(Res);
			}
		}
	}

	if (_Resolutions.size() == 0)
	{
		UIResolutionData DefRes;

		DefRes.Width = MU_RESOURCE->GetWindowWidth();
		DefRes.Height = MU_RESOURCE->GetWindowHeight();

		_Resolutions.push_back(DefRes);
	}

	for (mu_int32 n = 0; n < _Resolutions.size(); ++n)
	{
		UIResolutionData &res = _Resolutions[n];

		if (res.Width == MU_RESOURCE->GetWindowWidth() &&
			res.Height == MU_RESOURCE->GetWindowHeight())
		{
			_ResolutionSelected = n;
			break;
		}
	}

	ConfigureResolutionScroll();
	ConfigureResolutionList();
}

void UIOptionWindow::SetCurrentTab(mu_uint32 currentTab)
{
	for (mu_uint32 n = 0; n < MAX_TAB; ++n)
	{
		if (n == currentTab) continue;

		_TabButtons[n]->ChangeState(UIRadioButton::UI_NORMAL);
	}

	_CurrentTab = currentTab;
	_TabButtons[currentTab]->ChangeState(UIRadioButton::UI_PRESSED);
}

void UIOptionWindow::ClearResolutions()
{
	_Resolutions.clear();
	_ResolutionSelected = -1;
	_ResolutionListIndex = 0;
	_ResolutionScrollBar->ConfigureScrollInfo(0.0f, 1.0f, 0.0f);
}

void UIOptionWindow::ConfigureResolutionScroll()
{
	mu_boolean isEnabledScroll = _ResolutionScrollBar->GetState() != UIScrollBar::UI_DISABLED;

	if (_Resolutions.size() > MAX_RESOLUTION_RENDER &&
		isEnabledScroll == false)
	{
		_ResolutionListIndex = 0;
		_ResolutionScrollBar->ChangeState(UIScrollBar::UI_NORMAL);
		_ResolutionScrollBar->ConfigureScrollInfo(0.0f, 1.0f, 0.0f);
	}
	else if (_Resolutions.size() <= MAX_RESOLUTION_RENDER &&
		isEnabledScroll == true)
	{
		_ResolutionListIndex = 0;
		_ResolutionScrollBar->ChangeState(UIScrollBar::UI_DISABLED);
		_ResolutionScrollBar->ConfigureScrollInfo(0.0f, 1.0f, 0.0f);
	}

	if (_ResolutionScrollBar->GetState() != UIScrollBar::UI_DISABLED)
	{
		mu_uint32 maxIndex = _Resolutions.size() - MAX_RESOLUTION_RENDER;
		if (_ResolutionListIndex > maxIndex)
		{
			_ResolutionListIndex = maxIndex;
			_ResolutionScrollBar->ConfigureScrollInfo(0.0f, 1.0f, 1.0f);
		}
	}
}

void UIOptionWindow::ConfigureResolutionList()
{
	UIBase *focused = _SceneManager->GetFocused();

	mu_uint32 maxIndex = _Resolutions.size() > MAX_RESOLUTION_RENDER ? MAX_RESOLUTION_RENDER : _Resolutions.size();
	for (mu_uint32 n = 0; n < maxIndex; ++n)
	{
		UIRadioButton *btn = _ResolutionListBtn[n];

		if (_ResolutionSelected == _ResolutionListIndex + n)
		{
			btn->ChangeState(focused == btn ? UIRadioButton::UI_PRESSED_OVER : UIRadioButton::UI_PRESSED);
		}
		else
		{
			btn->ChangeState(focused == btn ? UIRadioButton::UI_OVER : UIRadioButton::UI_NORMAL);
		}

		btn->SetStateData(UIRadioButton::UI_NORMAL, UI_STATE_DATA(USE_COLOR_STATE, EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(0, 0, 0, 0)));
		btn->SetStateData(UIRadioButton::UI_OVER, UI_STATE_DATA(USE_COLOR_STATE, EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(150, 150, 150, 220)));
		btn->SetStateData(UIRadioButton::UI_PRESSED, UI_STATE_DATA(USE_COLOR_STATE, EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(150, 150, 150, 180)));
		btn->SetStateData(UIRadioButton::UI_PRESSED_OVER, UI_STATE_DATA(USE_COLOR_STATE, EVector2(0.0f, 0.0f), EVector2(1.0f, 1.0f), MU::MakeARGB(150, 150, 150, 220)));
	}
}

void UIOptionWindow::ConfigureShadowButton(mu_uint8 shadowType)
{
	const mu_text *ShadowTypes[] =
	{
		_T("None"),
		_T("Original"),
		_T("Original+"),
		_T("Advanced"),
	};

	_ShadowTypeBtn->SetStateText(UIButton::UI_DISABLED, FONT_NORMAL, ShadowTypes[shadowType], MU::MakeARGB(100, 100, 100, 255));
	_ShadowTypeBtn->SetStateText(UIButton::UI_NORMAL, FONT_NORMAL, ShadowTypes[shadowType], MU::MakeARGB(200, 200, 200, 255));
	_ShadowTypeBtn->SetStateText(UIButton::UI_OVER, FONT_NORMAL, ShadowTypes[shadowType], MU::MakeARGB(255, 255, 255, 255));
	_ShadowTypeBtn->SetStateText(UIButton::UI_PRESSED, FONT_NORMAL, ShadowTypes[shadowType], MU::MakeARGB(150, 150, 150, 255));
}

void UIOptionWindow::SetWhisperSound(mu_boolean bSound)
{
	_WhisperSound = bSound;
}

mu_boolean UIOptionWindow::IsWhisperSound()
{
	return _WhisperSound;
}

void UIOptionWindow::SetSlideHelp(mu_boolean bHelp)
{
	_SlideHelp = bHelp;
}

mu_boolean UIOptionWindow::IsSlideHelp()
{
	return _SlideHelp;
}

void UIOptionWindow::SetRenderLevel(mu_int32 iRender)
{
	_RenderLevel = iRender;
}

mu_int32 UIOptionWindow::GetRenderLevel()
{
	return _RenderLevel;
}