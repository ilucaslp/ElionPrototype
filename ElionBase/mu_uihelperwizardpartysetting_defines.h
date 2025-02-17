#ifndef __MU_UIHELPERWIZARDPARTYSETTING_DEFINES_H__
#define __MU_UIHELPERWIZARDPARTYSETTING_DEFINES_H__

#pragma once

#define SCREEN_WIDTH 1280.0f
#define SCREEN_HEIGHT 960.0f

#define WINBOX_WIDTH (340.0f)
#define WINBOX_HEIGHT (198.0f)
#define WINBOX_POSX (SCREEN_WIDTH * 0.5f - WINBOX_WIDTH * 0.5f)
#define WINBOX_POSY (SCREEN_HEIGHT * 0.5f - WINBOX_HEIGHT * 0.5f)

#define BUTTON_WIDTH 54.0f
#define BUTTON_HEIGHT 30.0f

#define OK_POSX (WINBOX_POSX + WINBOX_WIDTH * 0.5f - BUTTON_WIDTH * 0.5f - TPADDING_MSGBOX_GETLEFT(SCREEN_WIDTH))
#define OK_POSY (WINBOX_POSY + WINBOX_HEIGHT - BUTTON_HEIGHT - 18.0f)

#define BUFF_BACK_WIDTH (WINBOX_WIDTH - 42.0f)
#define BUFF_BACK_HEIGHT (93.0f)
#define BUFF_BACK_POSX (WINBOX_POSX + WINBOX_WIDTH * 0.5f - BUFF_BACK_WIDTH * 0.5f - TPADDING_MSGBOX_GETLEFT(SCREEN_WIDTH))
#define BUFF_BACK_POSY (WINBOX_POSY + TTEXT_MSGBOX_MOVEUP(SCREEN_HEIGHT) + 20.0f)
#define BUFF_TITLE_POSX (BUFF_BACK_POSX + BUFF_BACK_WIDTH * 0.5f)
#define BUFF_TITLE_POSY (BUFF_BACK_POSY + 25.0f)
#define BUFF_DIV_POSX (BUFF_BACK_POSX)
#define BUFF_DIV_POSY (BUFF_TITLE_POSY + 5.0f)
#define BUFF_DIV_WIDTH (BUFF_BACK_WIDTH)
#define BUFF_DIV_HEIGHT (3.0f)

#define ENABLEBUFF_WIDTH (22.5f)
#define ENABLEBUFF_HEIGHT (22.5f)
#define ENABLEBUFF_POSX (BUFF_BACK_POSX + 6.0f)
#define ENABLEBUFF_POSY (BUFF_DIV_POSY + BUFF_DIV_HEIGHT + 1.0f)
#define ENABLEBUFF_LABEL_POSX (ENABLEBUFF_POSX + ENABLEBUFF_WIDTH + 2.0f)
#define ENABLEBUFF_LABEL_POSY (ENABLEBUFF_POSY + ENABLEBUFF_HEIGHT * 0.5f)

#endif