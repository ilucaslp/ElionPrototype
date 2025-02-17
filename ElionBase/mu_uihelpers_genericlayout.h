#ifndef __MU_UIHELPERS_GENERICLAYOUT_H__
#define __MU_UIHELPERS_GENERICLAYOUT_H__

#pragma once

#ifndef __GNUC__
#pragma region Generic Layout Box
#endif
#define TUV_GLAYOUT_SCREENWIDTH (1024.0f)
#define TUV_GLAYOUT_SCREENHEIGHT (768.0f)

#define TUV_GLAYOUT_BACK_WIDTH (4.0f)
#define TUV_GLAYOUT_BACK_HEIGHT (4.0f)
#define TUV_GLAYOUT_FRAME_WIDTH (512.0f)
#define TUV_GLAYOUT_FRAME_HEIGHT (12.0f)
#define TUV_GLAYOUT_MIDDLE_WIDTH (6.0f)
#define TUV_GLAYOUT_MIDDLE_HEIGHT (4.0f)

#define TSIZE_GLAYOUT_MINWIDTH (TUV_GLAYOUT_FRAME_WIDTH)
#define TSIZE_GLAYOUT_MINHEIGHT (TUV_GLAYOUT_FRAME_HEIGHT)

#define TPADDING_GLAYOUT_BACK_LEFT (0.0f)
#define TPADDING_GLAYOUT_BACK_TOP (0.0f)
#define TPADDING_GLAYOUT_BACK_RIGHT (TPADDING_GLAYOUT_BACK_LEFT + 0.0f)
#define TPADDING_GLAYOUT_BACK_BOTTOM (TPADDING_GLAYOUT_BACK_TOP + 0.0f)

#define TSIZE_GLAYOUT_BACK_WIDTH(width) (width - TPADDING_GLAYOUT_BACK_RIGHT)
#define TSIZE_GLAYOUT_BACK_HEIGHT(height) (height - TPADDING_GLAYOUT_BACK_BOTTOM)

#define TSIZE_GLAYOUT_LEFTTOP_WIDTH (6.0f)
#define TSIZE_GLAYOUT_RIGHTTOP_WIDTH (6.0f)
#define TSIZE_GLAYOUT_MIDDLETOP_WIDTH(width) (width - TSIZE_GLAYOUT_LEFTTOP_WIDTH - TSIZE_GLAYOUT_RIGHTTOP_WIDTH)
#define TSIZE_GLAYOUT_TOP_HEIGHT (6.0f)

#define TSIZE_GLAYOUT_LEFTBOTTOM_WIDTH (6.0f)
#define TSIZE_GLAYOUT_RIGHTBOTTOM_WIDTH (6.0f)
#define TSIZE_GLAYOUT_MIDDLEBOTTOM_WIDTH(width) (width - TSIZE_GLAYOUT_LEFTBOTTOM_WIDTH - TSIZE_GLAYOUT_RIGHTBOTTOM_WIDTH)
#define TSIZE_GLAYOUT_BOTTOM_HEIGHT (6.0f)

#define TSIZE_GLAYOUT_LEFTMIDDLE_WIDTH (3.0f)
#define TSIZE_GLAYOUT_RIGHTMIDDLE_WIDTH (3.0f)
#define TSIZE_GLAYOUT_MIDDLE_HEIGHT(height) (height - TSIZE_GLAYOUT_TOP_HEIGHT - TSIZE_GLAYOUT_BOTTOM_HEIGHT)

#define TUV_GLAYOUT_BACK_BEGIN (EVector2(0.0f, 0.0f))
#define TUV_GLAYOUT_BACK_END(width, height) (EVector2(1.0f, 1.0f))
#define TUV_GLAYOUT_LEFTTOP_BEGIN (EVector2(UV_PADDING_LEFT(TUV_GLAYOUT_FRAME_WIDTH, 0.0f), UV_PADDING_TOP(TUV_GLAYOUT_FRAME_HEIGHT, 0.0f)))
#define TUV_GLAYOUT_LEFTTOP_END (EVector2(UV_PADDING_LEFT(TUV_GLAYOUT_FRAME_WIDTH, TSIZE_GLAYOUT_LEFTTOP_WIDTH), UV_PADDING_BOTTOM(TUV_GLAYOUT_FRAME_HEIGHT, TUV_GLAYOUT_FRAME_HEIGHT * 0.5f)))
#define TUV_GLAYOUT_RIGHTTOP_BEGIN (EVector2(UV_PADDING_RIGHT(TUV_GLAYOUT_FRAME_WIDTH, TSIZE_GLAYOUT_LEFTTOP_WIDTH), UV_PADDING_TOP(TUV_GLAYOUT_FRAME_HEIGHT, 0.0f)))
#define TUV_GLAYOUT_RIGHTTOP_END (EVector2(UV_PADDING_RIGHT(TUV_GLAYOUT_FRAME_WIDTH, 0.0f), UV_PADDING_BOTTOM(TUV_GLAYOUT_FRAME_HEIGHT, TUV_GLAYOUT_FRAME_HEIGHT * 0.5f)))
#define TUV_GLAYOUT_MIDDLETOP_BEGIN (EVector2(UV_PADDING_LEFT(TUV_GLAYOUT_FRAME_WIDTH, TSIZE_GLAYOUT_RIGHTTOP_WIDTH), UV_PADDING_TOP(TUV_GLAYOUT_FRAME_HEIGHT, 0.0f)))
#define TUV_GLAYOUT_MIDDLETOP_END (EVector2(UV_PADDING_RIGHT(TUV_GLAYOUT_FRAME_WIDTH, TSIZE_GLAYOUT_RIGHTTOP_WIDTH), UV_PADDING_BOTTOM(TUV_GLAYOUT_FRAME_HEIGHT, TUV_GLAYOUT_FRAME_HEIGHT * 0.5f)))

#define TUV_GLAYOUT_LEFTBOTTOM_BEGIN (EVector2(UV_PADDING_LEFT(TUV_GLAYOUT_FRAME_WIDTH, 0.0f), UV_PADDING_TOP(TUV_GLAYOUT_FRAME_HEIGHT, TUV_GLAYOUT_FRAME_HEIGHT * 0.5f)))
#define TUV_GLAYOUT_LEFTBOTTOM_END (EVector2(UV_PADDING_LEFT(TUV_GLAYOUT_FRAME_WIDTH, TSIZE_GLAYOUT_LEFTBOTTOM_WIDTH), UV_PADDING_BOTTOM(TUV_GLAYOUT_FRAME_HEIGHT, 0.0f)))
#define TUV_GLAYOUT_RIGHTBOTTOM_BEGIN (EVector2(UV_PADDING_RIGHT(TUV_GLAYOUT_FRAME_WIDTH, TSIZE_GLAYOUT_LEFTBOTTOM_WIDTH), UV_PADDING_TOP(TUV_GLAYOUT_FRAME_HEIGHT, TUV_GLAYOUT_FRAME_HEIGHT * 0.5f)))
#define TUV_GLAYOUT_RIGHTBOTTOM_END (EVector2(UV_PADDING_RIGHT(TUV_GLAYOUT_FRAME_WIDTH, 0.0f), UV_PADDING_BOTTOM(TUV_GLAYOUT_FRAME_HEIGHT, 0.0f)))
#define TUV_GLAYOUT_MIDDLEBOTTOM_BEGIN (EVector2(UV_PADDING_LEFT(TUV_GLAYOUT_FRAME_WIDTH, TSIZE_GLAYOUT_RIGHTBOTTOM_WIDTH), UV_PADDING_TOP(TUV_GLAYOUT_FRAME_HEIGHT, TUV_GLAYOUT_FRAME_HEIGHT * 0.5f)))
#define TUV_GLAYOUT_MIDDLEBOTTOM_END (EVector2(UV_PADDING_RIGHT(TUV_GLAYOUT_FRAME_WIDTH, TSIZE_GLAYOUT_RIGHTBOTTOM_WIDTH), UV_PADDING_BOTTOM(TUV_GLAYOUT_FRAME_HEIGHT, 0.0f)))

#define TUV_GLAYOUT_MIDDLELEFT_BEGIN (EVector2(UV_PADDING_LEFT(TUV_GLAYOUT_MIDDLE_WIDTH, 0.0f), UV_PADDING_TOP(TUV_GLAYOUT_MIDDLE_HEIGHT, 0.0f)))
#define TUV_GLAYOUT_MIDDLELEFT_END (EVector2(UV_PADDING_LEFT(TUV_GLAYOUT_MIDDLE_WIDTH, TSIZE_GLAYOUT_LEFTMIDDLE_WIDTH), UV_PADDING_BOTTOM(TUV_GLAYOUT_MIDDLE_HEIGHT, 0.0f)))
#define TUV_GLAYOUT_MIDDLERIGHT_BEGIN (EVector2(UV_PADDING_RIGHT(TUV_GLAYOUT_MIDDLE_WIDTH, TSIZE_GLAYOUT_RIGHTMIDDLE_WIDTH), UV_PADDING_TOP(TUV_GLAYOUT_MIDDLE_HEIGHT, 0.0f)))
#define TUV_GLAYOUT_MIDDLERIGHT_END (EVector2(UV_PADDING_RIGHT(TUV_GLAYOUT_MIDDLE_WIDTH, 0.0f), UV_PADDING_BOTTOM(TUV_GLAYOUT_MIDDLE_HEIGHT, 0.0f)))

#define TPOS_GLAYOUT_BACK_X(x) (x + TPADDING_GLAYOUT_BACK_LEFT)
#define TPOS_GLAYOUT_BACK_Y(y) (y + TPADDING_GLAYOUT_BACK_TOP)

#define TPOS_GLAYOUT_LEFTTOP_X(x) (x)
#define TPOS_GLAYOUT_RIGHTTOP_X(x, width) (x + (width - TSIZE_GLAYOUT_RIGHTTOP_WIDTH))
#define TPOS_GLAYOUT_MIDDLETOP_X(x) (x + TSIZE_GLAYOUT_LEFTTOP_WIDTH)
#define TPOS_GLAYOUT_TOP_Y(y) (y)

#define TPOS_GLAYOUT_LEFTBOTTOM_X(x) (x)
#define TPOS_GLAYOUT_RIGHTBOTTOM_X(x, width) (x + (width - TSIZE_GLAYOUT_RIGHTBOTTOM_WIDTH))
#define TPOS_GLAYOUT_MIDDLEBOTTOM_X(x) (x + TSIZE_GLAYOUT_LEFTBOTTOM_WIDTH)
#define TPOS_GLAYOUT_BOTTOM_Y(y, height) (y + height - TSIZE_GLAYOUT_BOTTOM_HEIGHT)

#define TPOS_GLAYOUT_LEFTMIDDLE_X(x) (x)
#define TPOS_GLAYOUT_RIGHTMIDDLE_X(x, width) (x + width - TSIZE_GLAYOUT_RIGHTMIDDLE_WIDTH)
#define TPOS_GLAYOUT_MIDDLE_Y(y, height) (y + TSIZE_GLAYOUT_TOP_HEIGHT)

#define TGLAYOUT_VARS(baseName) \
UISprite * baseName## _Int_gLayout_Back; \
UISprite * baseName## _Int_gLayout_TopLeft; \
UISprite * baseName## _Int_gLayout_TopMiddle; \
UISprite * baseName## _Int_gLayout_TopRight; \
UISprite * baseName## _Int_gLayout_MiddleLeft; \
UISprite * baseName## _Int_gLayout_MiddleRight; \
UISprite * baseName## _Int_gLayout_BottomLeft; \
UISprite * baseName## _Int_gLayout_BottomMiddle; \
UISprite * baseName## _Int_gLayout_BottomRight;

#define TGLAYOUT_PREPARE(baseName, manager, owner, screenWidth, screenHeight, x, y, width, height) \
{ \
mu_float int_aspectWidth = TUV_GLAYOUT_SCREENWIDTH / screenWidth; \
mu_float int_aspectHeight = TUV_GLAYOUT_SCREENHEIGHT / screenHeight; \
mu_float int_x = MU_TO_WINDOW(x, int_aspectWidth); \
mu_float int_y = MU_TO_WINDOW(y, int_aspectHeight); \
mu_float int_width = MU_TO_WINDOW(width, int_aspectWidth); \
mu_float int_height = MU_TO_WINDOW(height, int_aspectHeight); \
baseName## _Int_gLayout_Back = new UISprite(manager, owner, &INTERFACES::Data[INTERFACES::LOGIN_SERVER_EX03], TUV_GLAYOUT_SCREENWIDTH, TUV_GLAYOUT_SCREENHEIGHT, TPOS_GLAYOUT_BACK_X(int_x), TPOS_GLAYOUT_BACK_Y(int_y), TSIZE_GLAYOUT_BACK_WIDTH(int_width), TSIZE_GLAYOUT_BACK_HEIGHT(int_height), TUV_GLAYOUT_BACK_BEGIN, TUV_GLAYOUT_BACK_END(int_width, int_height)); \
baseName## _Int_gLayout_Back->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc); \
baseName## _Int_gLayout_Back->SetSampler(ESampler::eUrVrWrMinLMagLMipL_Anisotropy); \
baseName## _Int_gLayout_TopLeft = new UISprite(manager, owner, &INTERFACES::Data[INTERFACES::LOGIN_SERVER_EX01], TUV_GLAYOUT_SCREENWIDTH, TUV_GLAYOUT_SCREENHEIGHT, TPOS_GLAYOUT_LEFTTOP_X(int_x), TPOS_GLAYOUT_TOP_Y(int_y), TSIZE_GLAYOUT_LEFTTOP_WIDTH, TSIZE_GLAYOUT_TOP_HEIGHT, TUV_GLAYOUT_LEFTTOP_BEGIN, TUV_GLAYOUT_LEFTTOP_END); \
baseName## _Int_gLayout_TopLeft->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc); \
baseName## _Int_gLayout_TopMiddle = new UISprite(manager, owner, &INTERFACES::Data[INTERFACES::LOGIN_SERVER_EX01], TUV_GLAYOUT_SCREENWIDTH, TUV_GLAYOUT_SCREENHEIGHT, TPOS_GLAYOUT_MIDDLETOP_X(int_x), TPOS_GLAYOUT_TOP_Y(int_y), TSIZE_GLAYOUT_MIDDLETOP_WIDTH(int_width), TSIZE_GLAYOUT_TOP_HEIGHT, TUV_GLAYOUT_MIDDLETOP_BEGIN, TUV_GLAYOUT_MIDDLETOP_END); \
baseName## _Int_gLayout_TopMiddle->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc); \
baseName## _Int_gLayout_TopRight = new UISprite(manager, owner, &INTERFACES::Data[INTERFACES::LOGIN_SERVER_EX01], TUV_GLAYOUT_SCREENWIDTH, TUV_GLAYOUT_SCREENHEIGHT, TPOS_GLAYOUT_RIGHTTOP_X(int_x, int_width), TPOS_GLAYOUT_TOP_Y(int_y), TSIZE_GLAYOUT_RIGHTTOP_WIDTH, TSIZE_GLAYOUT_TOP_HEIGHT, TUV_GLAYOUT_RIGHTTOP_BEGIN, TUV_GLAYOUT_RIGHTTOP_END); \
baseName## _Int_gLayout_TopRight->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc); \
baseName## _Int_gLayout_MiddleLeft = new UISprite(manager, owner, &INTERFACES::Data[INTERFACES::LOGIN_SERVER_EX02], TUV_GLAYOUT_SCREENWIDTH, TUV_GLAYOUT_SCREENHEIGHT, TPOS_GLAYOUT_LEFTMIDDLE_X(int_x), TPOS_GLAYOUT_MIDDLE_Y(int_y, int_height), TSIZE_GLAYOUT_LEFTMIDDLE_WIDTH, TSIZE_GLAYOUT_MIDDLE_HEIGHT(int_height), TUV_GLAYOUT_MIDDLELEFT_BEGIN, TUV_GLAYOUT_MIDDLELEFT_END); \
baseName## _Int_gLayout_MiddleRight = new UISprite(manager, owner, &INTERFACES::Data[INTERFACES::LOGIN_SERVER_EX02], TUV_GLAYOUT_SCREENWIDTH, TUV_GLAYOUT_SCREENHEIGHT, TPOS_GLAYOUT_RIGHTMIDDLE_X(int_x, int_width), TPOS_GLAYOUT_MIDDLE_Y(int_y, int_height), TSIZE_GLAYOUT_RIGHTMIDDLE_WIDTH, TSIZE_GLAYOUT_MIDDLE_HEIGHT(int_height), TUV_GLAYOUT_MIDDLERIGHT_BEGIN, TUV_GLAYOUT_MIDDLERIGHT_END); \
baseName## _Int_gLayout_BottomLeft = new UISprite(manager, owner, &INTERFACES::Data[INTERFACES::LOGIN_SERVER_EX01], TUV_GLAYOUT_SCREENWIDTH, TUV_GLAYOUT_SCREENHEIGHT, TPOS_GLAYOUT_LEFTBOTTOM_X(int_x), TPOS_GLAYOUT_BOTTOM_Y(int_y, int_height), TSIZE_GLAYOUT_LEFTBOTTOM_WIDTH, TSIZE_GLAYOUT_BOTTOM_HEIGHT, TUV_GLAYOUT_LEFTBOTTOM_BEGIN, TUV_GLAYOUT_LEFTBOTTOM_END); \
baseName## _Int_gLayout_BottomLeft->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc); \
baseName## _Int_gLayout_BottomMiddle = new UISprite(manager, owner, &INTERFACES::Data[INTERFACES::LOGIN_SERVER_EX01], TUV_GLAYOUT_SCREENWIDTH, TUV_GLAYOUT_SCREENHEIGHT, TPOS_GLAYOUT_MIDDLEBOTTOM_X(int_x), TPOS_GLAYOUT_BOTTOM_Y(int_y, int_height), TSIZE_GLAYOUT_MIDDLEBOTTOM_WIDTH(int_width), TSIZE_GLAYOUT_BOTTOM_HEIGHT, TUV_GLAYOUT_MIDDLEBOTTOM_BEGIN, TUV_GLAYOUT_MIDDLEBOTTOM_END); \
baseName## _Int_gLayout_BottomMiddle->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc); \
baseName## _Int_gLayout_BottomRight = new UISprite(manager, owner, &INTERFACES::Data[INTERFACES::LOGIN_SERVER_EX01], TUV_GLAYOUT_SCREENWIDTH, TUV_GLAYOUT_SCREENHEIGHT, TPOS_GLAYOUT_RIGHTBOTTOM_X(int_x, int_width), TPOS_GLAYOUT_BOTTOM_Y(int_y, int_height), TSIZE_GLAYOUT_RIGHTBOTTOM_WIDTH, TSIZE_GLAYOUT_BOTTOM_HEIGHT, TUV_GLAYOUT_RIGHTBOTTOM_BEGIN, TUV_GLAYOUT_RIGHTBOTTOM_END); \
baseName## _Int_gLayout_BottomRight->SetBlendState(EBlendState::eAlphaSrc_InvAlphaSrc); \
}

#define TGLAYOUT_RENDERBACK(baseName) \
{ \
baseName## _Int_gLayout_Back->Draw(); \
}

#define TGLAYOUT_RENDERFRONT(baseName) \
{ \
baseName## _Int_gLayout_TopMiddle->Draw(); \
baseName## _Int_gLayout_TopLeft->Draw(); \
baseName## _Int_gLayout_TopRight->Draw(); \
baseName## _Int_gLayout_MiddleLeft->Draw(); \
baseName## _Int_gLayout_MiddleRight->Draw(); \
baseName## _Int_gLayout_BottomMiddle->Draw(); \
baseName## _Int_gLayout_BottomLeft->Draw(); \
baseName## _Int_gLayout_BottomRight->Draw(); \
}
#ifndef __GNUC__
#pragma endregion
#endif

#endif