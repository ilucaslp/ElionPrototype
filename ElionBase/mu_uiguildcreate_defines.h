#ifndef __MU_UIGUILDCREATE_DEFINES_H__
#define __MU_UIGUILDCREATE_DEFINES_H__

#pragma once

#define SCREEN_WIDTH 640.0f
#define SCREEN_HEIGHT 480.0f

#define BACKTEXTURE_WIDTH 225.0f
#define BACKTEXTURE_HEIGHT 512.0f

#define GUILDCREATE_POSX 450.0f
#define GUILDCREATE_POSY 0.0f
#define GUILDCREATE_WIDTH 190.0f
#define GUILDCREATE_HEIGHT 429.0f

#define BACK_POSX GUILDCREATE_POSX
#define BACK_POSY GUILDCREATE_POSY
#define BACK_WIDTH GUILDCREATE_WIDTH
#define BACK_HEIGHT GUILDCREATE_HEIGHT
#define BACK_UVBEGIN EVector2(0.0f, 0.0f)
#define BACK_UVEND EVector2(BACK_WIDTH / BACKTEXTURE_WIDTH, BACK_HEIGHT / BACKTEXTURE_HEIGHT)

#define TOP_WIDTH GUILDCREATE_WIDTH
#define TOP_HEIGHT 64.0f
#define TOP_POSX GUILDCREATE_POSX
#define TOP_POSY GUILDCREATE_POSY

#define BOTTOM_WIDTH GUILDCREATE_WIDTH
#define BOTTOM_HEIGHT 45.0f
#define BOTTOM_POSX GUILDCREATE_POSX
#define BOTTOM_POSY (GUILDCREATE_POSY + GUILDCREATE_HEIGHT - BOTTOM_HEIGHT)

#define LEFT_WIDTH 21.0f
#define LEFT_HEIGHT (BOTTOM_POSY - (TOP_POSY + TOP_HEIGHT))
#define LEFT_POSX GUILDCREATE_POSX
#define LEFT_POSY (TOP_POSY + TOP_HEIGHT)

#define RIGHT_WIDTH 21.0f
#define RIGHT_HEIGHT (BOTTOM_POSY - (TOP_POSY + TOP_HEIGHT))
#define RIGHT_POSX (GUILDCREATE_POSX + GUILDCREATE_WIDTH - RIGHT_WIDTH)
#define RIGHT_POSY (TOP_POSY + TOP_HEIGHT)

#define GOLDLINE_BIGHOR_WIDTH (130.0f)
#define GOLDLINE_BIGHOR_HEIGHT (2.0f)
#define GOLDLINE_BIGVER_WIDTH (2.0f)
#define GOLDLINE_BIGVER_HEIGHT (130.0f)
#define GOLDLINE_HOR_UV (EVector2(1.0f / 190.0f, 5.0f / 10.0f))
#define GOLDLINE_HOR_UVSIZE (EVector2(188.0f / 190.0f, 1.0f / 10.0f))
#define GOLDLINE_VER_UV (EVector2(1.0f / 190.0f, 80.0f / 256.0f))
#define GOLDLINE_VER_UVSIZE (EVector2(1.0f / 190.0f, 125.0f / 256.0f))

#define GOLDLINE_BIGTOP_POSX (GUILDCREATE_POSX + 45.0f)
#define GOLDLINE_BIGTOP_POSY (GUILDCREATE_POSY + 95.0f)
#define GOLDLINE_BIGBOTTOM_POSX (GOLDLINE_BIGTOP_POSX)
#define GOLDLINE_BIGBOTTOM_POSY (GOLDLINE_BIGTOP_POSY + GOLDLINE_BIGVER_HEIGHT - 2.0f)
#define GOLDLINE_BIGLEFT_POSX (GOLDLINE_BIGTOP_POSX)
#define GOLDLINE_BIGLEFT_POSY (GOLDLINE_BIGTOP_POSY)
#define GOLDLINE_BIGRIGHT_POSX (GOLDLINE_BIGLEFT_POSX + GOLDLINE_BIGHOR_WIDTH - 2.0f)
#define GOLDLINE_BIGRIGHT_POSY (GOLDLINE_BIGLEFT_POSY)

#define GOLDLINE_SMALLHOR_WIDTH (56.0f)
#define GOLDLINE_SMALLHOR_HEIGHT (2.0f)
#define GOLDLINE_SMALLVER_WIDTH (2.0f)
#define GOLDLINE_SMALLVER_HEIGHT (56.0f)

#define GOLDLINE_SMALLTOP_POSX (GUILDCREATE_POSX + 68.0f)
#define GOLDLINE_SMALLTOP_POSY (GUILDCREATE_POSY + 70.0f)
#define GOLDLINE_SMALLBOTTOM_POSX (GOLDLINE_SMALLTOP_POSX)
#define GOLDLINE_SMALLBOTTOM_POSY (GOLDLINE_SMALLTOP_POSY + GOLDLINE_SMALLVER_HEIGHT - 2.0f)
#define GOLDLINE_SMALLLEFT_POSX (GOLDLINE_SMALLTOP_POSX)
#define GOLDLINE_SMALLLEFT_POSY (GOLDLINE_SMALLTOP_POSY)
#define GOLDLINE_SMALLRIGHT_POSX (GOLDLINE_SMALLLEFT_POSX + GOLDLINE_SMALLHOR_WIDTH - 2.0f)
#define GOLDLINE_SMALLRIGHT_POSY (GOLDLINE_SMALLLEFT_POSY)

#define INPUTBACK_POSX (GUILDCREATE_POSX + 45.0f)
#define INPUTBACK_POSY (GUILDCREATE_POSY + 60.0f)
#define INPUTBACK_WIDTH 108.0f
#define INPUTBACK_HEIGHT 23.0f

#define INPUT_POSX (INPUTBACK_POSX + 4.0f)
#define INPUT_POSY (INPUTBACK_POSY + 5.0f)
#define INPUT_WIDTH (INPUTBACK_WIDTH - 10.0f)
#define INPUT_HEIGHT (INPUTBACK_HEIGHT - 8.0f)

#define MARKS_WIDTH (13.0f)
#define MARKS_HEIGHT (13.0f)
#define MARKS_POSX(x) (GUILDCREATE_POSX + x * (MARKS_WIDTH + 2.0f) + 50.0f + 1.0f)
#define MARKS_POSY(y) (GUILDCREATE_POSY + y * (MARKS_HEIGHT + 2.0f) + 100.0f + 1.0f)

#define COLORS_WIDTH (18.0f)
#define COLORS_HEIGHT (18.0f)
#define COLORS_POSX(x) (GUILDCREATE_POSX + x * (COLORS_WIDTH + 2.0f) + 15.0f + 1.0f)
#define COLORS_POSY(y) (GUILDCREATE_POSY + y * (COLORS_HEIGHT + 2.0f) + 260.0f + 1.0f)

#define SELECTCOLOR_WIDTH (23.0f)
#define SELECTCOLOR_HEIGHT (23.0f)
#define SELECTCOLOR_POSX (GUILDCREATE_POSX + 16.0f)
#define SELECTCOLOR_POSY (GUILDCREATE_POSY + 231.0f)

#define MARK_WIDTH (48.0f)
#define MARK_HEIGHT (48.0f)
#define MARK_POSX (GUILDCREATE_POSX + 72.0f)
#define MARK_POSY (GUILDCREATE_POSY + 74.0f)

#define SMALLBTN_WIDTH (64.0f)
#define SMALLBTN_HEIGHT (29.0f)
#define MEDIUMBTN_WIDTH (108.0f)
#define MEDIUMBTN_HEIGHT (29.0f)

#define BACKBTN_POSX (GUILDCREATE_POSX + 15.0f)
#define BACKBTN_POSY (GUILDCREATE_POSY + 379.0f)
#define NEXTBTN_POSX (GUILDCREATE_POSX + 110.0f)
#define NEXTBTN_POSY (GUILDCREATE_POSY + 379.0f)
#define MAKEBTN_POSX (GUILDCREATE_POSX + GUILDCREATE_WIDTH * 0.5f - MEDIUMBTN_WIDTH * 0.5f)
#define MAKEBTN_POSY (GUILDCREATE_POSY + 100.0f)

#define XBUTTON_WIDTH 16.0f
#define XBUTTON_HEIGHT 16.0f
#define XBUTTON_POSX (TOP_POSX + TOP_WIDTH - 7.0f - XBUTTON_WIDTH)
#define XBUTTON_POSY (TOP_POSY + 7.0f)

#endif