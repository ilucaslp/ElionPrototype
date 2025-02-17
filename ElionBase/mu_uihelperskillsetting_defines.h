#ifndef __MU_UIHELPERSKILLSETTING_DEFINES_H__
#define __MU_UIHELPERSKILLSETTING_DEFINES_H__

#pragma once

#define SCREEN_WIDTH 1280.0f
#define SCREEN_HEIGHT 960.0f

#define WINBOX_WIDTH (330.0f)
#define WINBOX_HEIGHT (345.0f)
#define WINBOX_POSX (SCREEN_WIDTH * 0.5f - WINBOX_WIDTH * 0.5f)
#define WINBOX_POSY (SCREEN_HEIGHT * 0.5f - WINBOX_HEIGHT * 0.5f)

#define BUTTON_WIDTH 54.0f
#define BUTTON_HEIGHT 30.0f

#define OK_POSX (WINBOX_POSX + WINBOX_WIDTH * 0.5f - BUTTON_WIDTH * 0.5f - TPADDING_MSGBOX_GETLEFT(SCREEN_WIDTH))
#define OK_POSY (WINBOX_POSY + WINBOX_HEIGHT - BUTTON_HEIGHT - 18.0f)

#define PRECON_BACK_WIDTH (WINBOX_WIDTH - 42.0f)
#define PRECON_BACK_HEIGHT (93.0f)
#define PRECON_BACK_POSX (WINBOX_POSX + WINBOX_WIDTH * 0.5f - PRECON_BACK_WIDTH * 0.5f - TPADDING_MSGBOX_GETLEFT(SCREEN_WIDTH))
#define PRECON_BACK_POSY (WINBOX_POSY + TTEXT_MSGBOX_MOVEUP(SCREEN_HEIGHT) + 20.0f)
#define PRECON_TITLE_POSX (PRECON_BACK_POSX + PRECON_BACK_WIDTH * 0.5f)
#define PRECON_TITLE_POSY (PRECON_BACK_POSY + 25.0f)
#define PRECON_DIV_POSX (PRECON_BACK_POSX)
#define PRECON_DIV_POSY (PRECON_TITLE_POSY + 5.0f)
#define PRECON_DIV_WIDTH (PRECON_BACK_WIDTH)
#define PRECON_DIV_HEIGHT (3.0f)

#define HTRANGE_POSX (PRECON_BACK_POSX + 6.0f)
#define HTRANGE_POSY (PRECON_DIV_POSY + 4.0f)
#define HTRANGE_WIDTH (TSIZE_RADIOBUTTON_WIDTH(SCREEN_WIDTH))
#define HTRANGE_HEIGHT (TSIZE_RADIOBUTTON_HEIGHT(SCREEN_HEIGHT))
#define HTRANGE_LABEL_POSX (HTRANGE_POSX + HTRANGE_WIDTH + 2.0f)
#define HTRANGE_LABEL_POSY (HTRANGE_POSY + HTRANGE_HEIGHT * 0.5f)

#define HTATTACKER_POSX (PRECON_BACK_POSX + 6.0f)
#define HTATTACKER_POSY (HTRANGE_POSY + HTRANGE_HEIGHT + 1.0f)
#define HTATTACKER_WIDTH (TSIZE_RADIOBUTTON_WIDTH(SCREEN_WIDTH))
#define HTATTACKER_HEIGHT (TSIZE_RADIOBUTTON_HEIGHT(SCREEN_HEIGHT))
#define HTATTACKER_LABEL_POSX (HTATTACKER_POSX + HTATTACKER_WIDTH + 2.0f)
#define HTATTACKER_LABEL_POSY (HTATTACKER_POSY + HTATTACKER_HEIGHT * 0.5f)

#define SUBCON_BACK_WIDTH (PRECON_BACK_WIDTH)
#define SUBCON_BACK_HEIGHT (143.0f)
#define SUBCON_BACK_POSX (PRECON_BACK_POSX)
#define SUBCON_BACK_POSY (PRECON_BACK_POSY + PRECON_BACK_HEIGHT + 4.0f)
#define SUBCON_TITLE_POSX (SUBCON_BACK_POSX + SUBCON_BACK_WIDTH * 0.5f)
#define SUBCON_TITLE_POSY (SUBCON_BACK_POSY + 25.0f)
#define SUBCON_DIV_POSX (SUBCON_BACK_POSX)
#define SUBCON_DIV_POSY (SUBCON_TITLE_POSY + 5.0f)
#define SUBCON_DIV_WIDTH (SUBCON_BACK_WIDTH)
#define SUBCON_DIV_HEIGHT (3.0f)

#define TWOPLUS_POSX (SUBCON_BACK_POSX + 6.0f)
#define TWOPLUS_POSY (SUBCON_DIV_POSY + 4.0f)
#define TWOPLUS_WIDTH (TSIZE_RADIOBUTTON_WIDTH(SCREEN_WIDTH))
#define TWOPLUS_HEIGHT (TSIZE_RADIOBUTTON_HEIGHT(SCREEN_HEIGHT))
#define TWOPLUS_LABEL_POSX (TWOPLUS_POSX + TWOPLUS_WIDTH + 2.0f)
#define TWOPLUS_LABEL_POSY (TWOPLUS_POSY + TWOPLUS_HEIGHT * 0.5f)

#define THREEPLUS_POSX (SUBCON_BACK_POSX + 6.0f)
#define THREEPLUS_POSY (TWOPLUS_POSY + TWOPLUS_HEIGHT + 1.0f)
#define THREEPLUS_WIDTH (TSIZE_RADIOBUTTON_WIDTH(SCREEN_WIDTH))
#define THREEPLUS_HEIGHT (TSIZE_RADIOBUTTON_HEIGHT(SCREEN_HEIGHT))
#define THREEPLUS_LABEL_POSX (THREEPLUS_POSX + THREEPLUS_WIDTH + 2.0f)
#define THREEPLUS_LABEL_POSY (THREEPLUS_POSY + THREEPLUS_HEIGHT * 0.5f)

#define FOURPLUS_POSX (SUBCON_BACK_POSX + 6.0f)
#define FOURPLUS_POSY (THREEPLUS_POSY + THREEPLUS_HEIGHT + 1.0f)
#define FOURPLUS_WIDTH (TSIZE_RADIOBUTTON_WIDTH(SCREEN_WIDTH))
#define FOURPLUS_HEIGHT (TSIZE_RADIOBUTTON_HEIGHT(SCREEN_HEIGHT))
#define FOURPLUS_LABEL_POSX (FOURPLUS_POSX + FOURPLUS_WIDTH + 2.0f)
#define FOURPLUS_LABEL_POSY (FOURPLUS_POSY + FOURPLUS_HEIGHT * 0.5f)

#define FIVEPLUS_POSX (SUBCON_BACK_POSX + 6.0f)
#define FIVEPLUS_POSY (FOURPLUS_POSY + FOURPLUS_HEIGHT + 1.0f)
#define FIVEPLUS_WIDTH (TSIZE_RADIOBUTTON_WIDTH(SCREEN_WIDTH))
#define FIVEPLUS_HEIGHT (TSIZE_RADIOBUTTON_HEIGHT(SCREEN_HEIGHT))
#define FIVEPLUS_LABEL_POSX (FIVEPLUS_POSX + FIVEPLUS_WIDTH + 2.0f)
#define FIVEPLUS_LABEL_POSY (FIVEPLUS_POSY + FIVEPLUS_HEIGHT * 0.5f)

#endif