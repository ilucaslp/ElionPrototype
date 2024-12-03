#ifndef __MU_UIHELPERSKILLSELECT_DEFINES_H__
#define __MU_UIHELPERSKILLSELECT_DEFINES_H__

#pragma once

#define SCREEN_WIDTH 1280.0f
#define SCREEN_HEIGHT 960.0f

#define WINBOX_WIDTH (460.0f)
#define WINBOX_HEIGHT (218.0f)
#define WINBOX_POSX (SCREEN_WIDTH * 0.5f - WINBOX_WIDTH * 0.5f)
#define WINBOX_POSY (SCREEN_HEIGHT * 0.5f - WINBOX_HEIGHT * 0.5f)

#define BUTTON_WIDTH 54.0f
#define BUTTON_HEIGHT 30.0f

#define OK_POSX (WINBOX_POSX + WINBOX_WIDTH * 0.5f - BUTTON_WIDTH * 0.5f - TPADDING_MSGBOX_GETLEFT(SCREEN_WIDTH))
#define OK_POSY (WINBOX_POSY + WINBOX_HEIGHT - BUTTON_HEIGHT - 18.0f)

#define SKILL_BACK_WIDTH (WINBOX_WIDTH - 42.0f)
#define SKILL_BACK_HEIGHT (113.0f)
#define SKILL_BACK_POSX (WINBOX_POSX + WINBOX_WIDTH * 0.5f - SKILL_BACK_WIDTH * 0.5f - TPADDING_MSGBOX_GETLEFT(SCREEN_WIDTH))
#define SKILL_BACK_POSY (WINBOX_POSY + TTEXT_MSGBOX_MOVEUP(SCREEN_HEIGHT) + 20.0f)
#define SKILL_TITLE_POSX (SKILL_BACK_POSX + SKILL_BACK_WIDTH * 0.5f)
#define SKILL_TITLE_POSY (SKILL_BACK_POSY + 25.0f)
#define SKILL_DIV_POSX (SKILL_BACK_POSX)
#define SKILL_DIV_POSY (SKILL_TITLE_POSY + 5.0f)
#define SKILL_DIV_WIDTH (SKILL_BACK_WIDTH)
#define SKILL_DIV_HEIGHT (3.0f)

#define LEFTARROW_WIDTH (25.0f)
#define LEFTARROW_HEIGHT (28.75f)
#define LEFTARROW_POSX (SKILL_BACK_POSX + 10.0f)
#define LEFTARROW_POSY (SKILL_DIV_POSY + (SKILL_BACK_HEIGHT - (SKILL_DIV_POSY - SKILL_BACK_POSY)) * 0.5f - LEFTARROW_HEIGHT * 0.5f)

#define RIGHTARROW_WIDTH (25.0f)
#define RIGHTARROW_HEIGHT (28.75f)
#define RIGHTARROW_POSX (SKILL_BACK_POSX + SKILL_BACK_WIDTH - 10.0f - RIGHTARROW_WIDTH)
#define RIGHTARROW_POSY (SKILL_DIV_POSY + (SKILL_BACK_HEIGHT - (SKILL_DIV_POSY - SKILL_BACK_POSY)) * 0.5f - RIGHTARROW_HEIGHT * 0.5f)

#define SKILL_ICON_FREEWIDTH (RIGHTARROW_POSX - (LEFTARROW_POSX + LEFTARROW_WIDTH))

#define SKILL_ICON_WIDTH (40.0f)
#define SKILL_ICON_HEIGHT (56.0f)
#define SKILL_ICON_SPACEX (2.0f)
#define SKILL_ICON_POSX(n) (LEFTARROW_POSX + LEFTARROW_WIDTH + SKILL_ICON_FREEWIDTH * 0.5f - (SKILL_ICON_WIDTH + SKILL_ICON_SPACEX) * MAX_SKILL_SHOW * 0.5f + (SKILL_ICON_WIDTH + SKILL_ICON_SPACEX) * n)
#define SKILL_ICON_POSY (SKILL_DIV_POSY + (SKILL_BACK_HEIGHT - (SKILL_DIV_POSY - SKILL_BACK_POSY)) * 0.5f - SKILL_ICON_HEIGHT * 0.5f)

#endif