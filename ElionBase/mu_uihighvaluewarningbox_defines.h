#ifndef __MU_UIHIGHVALUEWARNINGBOX_DEFINES_H__
#define __MU_UIHIGHVALUEWARNINGBOX_DEFINES_H__

#pragma once

#define SCREEN_WIDTH 1280.0f
#define SCREEN_HEIGHT 960.0f

#define WINBOX_WIDTH (450.0f)
#define WINBOX_HEIGHT (260.0f)
#define WINBOX_POSX (SCREEN_WIDTH * 0.5f - WINBOX_WIDTH * 0.5f)
#define WINBOX_POSY (SCREEN_HEIGHT * 0.5f - WINBOX_HEIGHT * 0.5f)

#define BUTTON_WIDTH 54.0f
#define BUTTON_HEIGHT 30.0f

#define ACCEPT_POSX (WINBOX_POSX + 70.0f - TPADDING_MSGBOX_GETLEFT(SCREEN_WIDTH))
#define ACCEPT_POSY (WINBOX_POSY + WINBOX_HEIGHT -  TPADDING_MSGBOX_GETBOTTOM(SCREEN_HEIGHT) - 50.0f - BUTTON_HEIGHT)
#define CANCEL_POSX (WINBOX_POSX + WINBOX_WIDTH - TPADDING_MSGBOX_GETLEFT(SCREEN_WIDTH) - 70.0f - BUTTON_WIDTH)
#define CANCEL_POSY (ACCEPT_POSY)

#define ITEM_WIDTH 200.0f
#define ITEM_HEIGHT 80.0f
#define ITEM_POSX (WINBOX_POSX + ITEM_WIDTH * 0.5f)
#define ITEM_POSY (WINBOX_POSY + (WINBOX_HEIGHT * (MSGBOX_TEXT_TOP_BLANK / 135.0f)) + ITEM_HEIGHT * 0.5f)
#define TEXT_POSX (WINBOX_POSX + ITEM_WIDTH - 20.0f)
#define TEXT_POSY (WINBOX_POSY + (WINBOX_HEIGHT * (MSGBOX_TEXT_TOP_BLANK / 135.0f)))

#endif