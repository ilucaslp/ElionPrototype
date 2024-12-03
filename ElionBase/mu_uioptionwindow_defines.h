#ifndef __MU_UIOPTIONWINDOW_DEFINES_H__
#define __MU_UIOPTIONWINDOW_DEFINES_H__

#pragma once

#define SCREEN_WIDTH 1024.0f
#define SCREEN_HEIGHT 768.0f

#define OPTIONWINDOW_WIDTH (400.0f)
#define OPTIONWINDOW_HEIGHT (450.0f)
#define OPTIONWINDOW_POSX (SCREEN_WIDTH * 0.5f - OPTIONWINDOW_WIDTH * 0.5f)
#define OPTIONWINDOW_POSY (SCREEN_HEIGHT * 0.5f - OPTIONWINDOW_HEIGHT * 0.5f)

#define MESSAGE_EXHEIGHT (3.0f)

#define BACK_POSX OPTIONWINDOW_POSX
#define BACK_POSY OPTIONWINDOW_POSY
#define BACK_WIDTH OPTIONWINDOW_WIDTH
#define BACK_UVBEGIN EVector2(0.0f, 0.0f)
#define BACK_UVEND EVector2(1.0f, 1.0f)

#define TOP_WIDTH OPTIONWINDOW_WIDTH
#define TOP_HEIGHT 4.0f
#define TOP_POSX OPTIONWINDOW_POSX
#define TOP_POSY OPTIONWINDOW_POSY

#define LEFT_WIDTH ((OPTIONWINDOW_WIDTH / 512.0f) * 4.0f)
#define LEFT_HEIGHT (OPTIONWINDOW_HEIGHT - TOP_HEIGHT * 2.0f)
#define LEFT_POSX OPTIONWINDOW_POSX
#define LEFT_POSY (TOP_POSY + TOP_HEIGHT)

#define RIGHT_WIDTH ((OPTIONWINDOW_WIDTH / 512.0f) * 4.0f)
#define RIGHT_HEIGHT (OPTIONWINDOW_HEIGHT - TOP_HEIGHT * 2.0f)
#define RIGHT_POSX (OPTIONWINDOW_POSX + OPTIONWINDOW_WIDTH - RIGHT_WIDTH)
#define RIGHT_POSY (TOP_POSY + TOP_HEIGHT)

#define BOTTOM_WIDTH OPTIONWINDOW_WIDTH
#define BOTTOM_HEIGHT 4.0f
#define BOTTOM_POSX OPTIONWINDOW_POSX
#define BOTTOM_POSY (LEFT_POSY + LEFT_HEIGHT)

#define TABBUTTON_WIDTH (108.0f)
#define TABBUTTON_HEIGHT (26.0f)
#define TABBUTTON_POSX(n) (OPTIONWINDOW_POSX + OPTIONWINDOW_WIDTH * 0.5f - TABBUTTON_WIDTH * UIOptionWindow::MAX_TAB * 0.5f + n * TABBUTTON_WIDTH)
#define TABBUTTON_POSY (OPTIONWINDOW_POSY + 25.0f)

#define BUTTONNS_WIDTH (108.0f)
#define BUTTONNS_HEIGHT (26.0f)

#define BUTTONSHADOW_WIDTH (80.0f)
#define BUTTONSHADOW_HEIGHT (15.0f)

#define APPLYBUTTON_POSX (OPTIONWINDOW_POSX + OPTIONWINDOW_WIDTH - 15.0f - (BUTTONNS_WIDTH + 1.0f) * 2.0f)
#define APPLYBUTTON_POSY (OPTIONWINDOW_POSY + OPTIONWINDOW_HEIGHT - 15.0f - BUTTONNS_HEIGHT)
#define APPLYBUTTON_TEXT _T("Apply")

#define CLOSEBUTTON_POSX (OPTIONWINDOW_POSX + OPTIONWINDOW_WIDTH - 15.0f - (BUTTONNS_WIDTH + 1.0f) * 1.0f)
#define CLOSEBUTTON_POSY (OPTIONWINDOW_POSY + OPTIONWINDOW_HEIGHT - 15.0f - BUTTONNS_HEIGHT)
#define CLOSEBUTTON_TEXT _T("Close")

#define WARNING_POSX (OPTIONWINDOW_POSX + 20.0f)
#define WARNING_POSY (OPTIONWINDOW_POSY + OPTIONWINDOW_HEIGHT - 50.0f)

// General Tab
#define FEATURES1BOX_START_POSX (RESOLUTIONBOX_START_POSX + 20.0f)

#define FEATURES1BOX_ITEM_HEIGHT (15.0f)
#define FEATURES1BOX_ITEM_POSY(n) (OPTIONWINDOW_POSY + 75.0f + (FEATURES1BOX_ITEM_HEIGHT * n))

#define FEATURES1BOX_TITLE_POSX (FEATURES1BOX_START_POSX + 5.0f)
#define FEATURES1BOX_TITLE_POSY (VIDEOTAB_LINE_ONE_POSY)

#define FEATURES1BOX_AUTOATTACK_WIDTH (15.0f)
#define FEATURES1BOX_AUTOATTACK_HEIGHT (15.0f)
#define FEATURES1BOX_AUTOATTACK_POSX (FEATURES1BOX_START_POSX)
#define FEATURES1BOX_AUTOATTACK_POSY (FEATURES1BOX_ITEM_POSY(0) + FEATURES1BOX_ITEM_HEIGHT * 0.5f)
#define FEATURES1BOX_AUTOATTACK_TITLE_POSX (FEATURES1BOX_AUTOATTACK_POSX + FEATURES1BOX_AUTOATTACK_WIDTH + 5.0f)
#define FEATURES1BOX_AUTOATTACK_TITLE_POSY (FEATURES1BOX_AUTOATTACK_POSY + FEATURES1BOX_AUTOATTACK_HEIGHT * 0.5f)

// Video Tab
#define VIDEOTAB_LINE_ONE_POSY (OPTIONWINDOW_POSY + 80.0f - 13.0f)

#define RESOLUTIONSCROLLBAR_WIDTH (7.0f)
#define RESOLUTIONSCROLLBAR_BACKWIDTH (7.0f)
#define RESOLUTIONSCROLLBAR_BTN_WIDTH (15.0f)
#define RESOLUTIONSCROLLBAR_BTN_HEIGHT (30.0f)
#define RESOLUTIONSCROLLBAR_TOP_BOTTOM_PART_HEIGHT (3.0f)
#define RESOLUTIONSCROLLBAR_MIDDLE_PART_HEIGHT (14.8f)
#define RESOLUTIONSCROLLBAR_BAR_WIDTH (7.0f)
#define RESOLUTIONSCROLLBAR_POSX (OPTIONWINDOW_POSX + 168.5f - RESOLUTIONSCROLLBAR_WIDTH)
#define RESOLUTIONSCROLLBAR_POSY (OPTIONWINDOW_POSY + 80.0f)
#define RESOLUTIONSCROLLBAR_LINESHEIGHT ((MAX_RESOLUTION_RENDER - 1) * RESOLUTIONSCROLLBAR_MIDDLE_PART_HEIGHT)

#define RESOLUTIONBOX_START_POSX (OPTIONWINDOW_POSX + 13.0f)

#define RESOLUTIONBOX_ITEM_WIDTH (148.0f)
#define RESOLUTIONBOX_ITEM_HEIGHT (13.0f)
#define RESOLUTIONBOX_ITEM_POSX (RESOLUTIONBOX_START_POSX)
#define RESOLUTIONBOX_ITEM_POSY(n) (OPTIONWINDOW_POSY + 80.0f + (RESOLUTIONBOX_ITEM_HEIGHT * n))

#define RESOLUTIONBOX_TITLE_POSX (RESOLUTIONBOX_ITEM_POSX + 11.0f)
#define RESOLUTIONBOX_TITLE_POSY (VIDEOTAB_LINE_ONE_POSY)

#define ADVANCEDBOX_START_POSX (RESOLUTIONBOX_START_POSX + RESOLUTIONBOX_ITEM_WIDTH + 20.0f)

#define ADVANCEDBOX_ITEM_HEIGHT (15.0f)
#define ADVANCEDBOX_ITEM_POSY(n) (OPTIONWINDOW_POSY + 75.0f + (ADVANCEDBOX_ITEM_HEIGHT * n))

#define ADVANCEDBOX_TITLE_POSX (ADVANCEDBOX_START_POSX + 5.0f)
#define ADVANCEDBOX_TITLE_POSY (VIDEOTAB_LINE_ONE_POSY)

#define ADVANCEDBOX_WINDOWMODE_WIDTH (15.0f)
#define ADVANCEDBOX_WINDOWMODE_HEIGHT (15.0f)
#define ADVANCEDBOX_WINDOWMODE_POSX (ADVANCEDBOX_START_POSX)
#define ADVANCEDBOX_WINDOWMODE_POSY (ADVANCEDBOX_ITEM_POSY(0) + ADVANCEDBOX_ITEM_HEIGHT * 0.5f)
#define ADVANCEDBOX_WINDOWMODE_TITLE_POSX (ADVANCEDBOX_WINDOWMODE_POSX + ADVANCEDBOX_WINDOWMODE_WIDTH + 5.0f)
#define ADVANCEDBOX_WINDOWMODE_TITLE_POSY (ADVANCEDBOX_WINDOWMODE_POSY + ADVANCEDBOX_WINDOWMODE_HEIGHT * 0.5f)

#define ADVANCEDBOX_AA_WIDTH (15.0f)
#define ADVANCEDBOX_AA_HEIGHT (15.0f)
#define ADVANCEDBOX_AA_POSX (ADVANCEDBOX_START_POSX)
#define ADVANCEDBOX_AA_POSY (ADVANCEDBOX_ITEM_POSY(1) + ADVANCEDBOX_ITEM_HEIGHT * 0.5f)
#define ADVANCEDBOX_AA_TITLE_POSX (ADVANCEDBOX_AA_POSX + ADVANCEDBOX_AA_WIDTH + 5.0f)
#define ADVANCEDBOX_AA_TITLE_POSY (ADVANCEDBOX_AA_POSY + ADVANCEDBOX_AA_HEIGHT * 0.5f)

#define ADVANCEDBOX_VERTICALSYNC_WIDTH (15.0f)
#define ADVANCEDBOX_VERTICALSYNC_HEIGHT (15.0f)
#define ADVANCEDBOX_VERTICALSYNC_POSX (ADVANCEDBOX_START_POSX)
#define ADVANCEDBOX_VERTICALSYNC_POSY (ADVANCEDBOX_ITEM_POSY(2) + ADVANCEDBOX_ITEM_HEIGHT * 0.5f)
#define ADVANCEDBOX_VERTICALSYNC_TITLE_POSX (ADVANCEDBOX_VERTICALSYNC_POSX + ADVANCEDBOX_VERTICALSYNC_WIDTH + 5.0f)
#define ADVANCEDBOX_VERTICALSYNC_TITLE_POSY (ADVANCEDBOX_VERTICALSYNC_POSY + ADVANCEDBOX_VERTICALSYNC_HEIGHT * 0.5f)

#define ADVANCEDBOX_FPSLIMIT_WIDTH (15.0f)
#define ADVANCEDBOX_FPSLIMIT_HEIGHT (15.0f)
#define ADVANCEDBOX_FPSLIMIT_POSX (ADVANCEDBOX_START_POSX)
#define ADVANCEDBOX_FPSLIMIT_POSY (ADVANCEDBOX_ITEM_POSY(3) + ADVANCEDBOX_ITEM_HEIGHT * 0.5f)
#define ADVANCEDBOX_FPSLIMIT_TITLE_POSX (ADVANCEDBOX_FPSLIMIT_POSX + ADVANCEDBOX_FPSLIMIT_WIDTH + 5.0f)
#define ADVANCEDBOX_FPSLIMIT_TITLE_POSY (ADVANCEDBOX_FPSLIMIT_POSY + ADVANCEDBOX_FPSLIMIT_HEIGHT * 0.5f)

#define ADVANCEDBOX_SHADOWTYPE_POSX (ADVANCEDBOX_START_POSX)
#define ADVANCEDBOX_SHADOWTYPE_POSY (ADVANCEDBOX_ITEM_POSY(5) + ADVANCEDBOX_ITEM_HEIGHT * 0.5f)
#define ADVANCEDBOX_SHADOWTYPE_TITLE_POSX (ADVANCEDBOX_SHADOWTYPE_POSX)
#define ADVANCEDBOX_SHADOWTYPE_TITLE_POSY (ADVANCEDBOX_ITEM_POSY(4) + ADVANCEDBOX_ITEM_HEIGHT)

// Audio Tab
#define AUDIOTAB_LINE_ONE_POSY (OPTIONWINDOW_POSY + 80.0f - 13.0f)

#define MUSICBOX_START_POSX (OPTIONWINDOW_POSX + 13.0f)

#define MUSICBOX_TITLE_POSX (MUSICBOX_START_POSX + 11.0f)
#define MUSICBOX_TITLE_POSY (AUDIOTAB_LINE_ONE_POSY)

#define MUSICBOX_SLIDER_POSX (MUSICBOX_TITLE_POSX - 3.0f)
#define MUSICBOX_SLIDER_POSY (MUSICBOX_TITLE_POSY + 20.0f)
#define MUSICBOX_SLIDER_WIDTH (124.0f)
#define MUSICBOX_SLIDER_HEIGHT (16.0f)

#define MUSICBOX_LABEL_POSX (MUSICBOX_SLIDER_POSX + MUSICBOX_SLIDER_WIDTH + 5.0f)
#define MUSICBOX_LABEL_POSY (MUSICBOX_SLIDER_POSY + MUSICBOX_SLIDER_HEIGHT * 0.5f)

#define SOUNDBOX_START_POSX (OPTIONWINDOW_POSX + 13.0f)

#define SOUNDBOX_TITLE_POSX (SOUNDBOX_START_POSX + 11.0f)
#define SOUNDBOX_TITLE_POSY (MUSICBOX_SLIDER_POSY + MUSICBOX_SLIDER_HEIGHT + 15.0f)

#define SOUNDBOX_SLIDER_POSX (SOUNDBOX_TITLE_POSX - 3.0f)
#define SOUNDBOX_SLIDER_POSY (SOUNDBOX_TITLE_POSY + 20.0f)
#define SOUNDBOX_SLIDER_WIDTH (124.0f)
#define SOUNDBOX_SLIDER_HEIGHT (16.0f)

#define SOUNDBOX_LABEL_POSX (SOUNDBOX_SLIDER_POSX + SOUNDBOX_SLIDER_WIDTH + 5.0f)
#define SOUNDBOX_LABEL_POSY (SOUNDBOX_SLIDER_POSY + SOUNDBOX_SLIDER_HEIGHT * 0.5f)

#endif