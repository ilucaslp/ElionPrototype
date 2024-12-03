#ifndef __MU_UIHELPERS_GENERAL_H__
#define __MU_UIHELPERS_GENERAL_H__

#pragma once

#define TSCREENBASE_WIDTH (800.0f)
#define TSCREENBASE_HEIGHT (600.0f)

#ifndef __GNUC__
#pragma region General Usage Defines
#endif
#define UV_PADDING_LEFT(width, pixel) (pixel / width)
#define UV_PADDING_RIGHT(width, pixel) ((width - pixel) / width)
#define UV_PADDING_TOP(height, pixel) (pixel / height)
#define UV_PADDING_BOTTOM(height, pixel) ((height - pixel) / height)

#define TUV_THREE_NORMAL_BEGIN EVector2(0.0f, (1.0f / 3.0f) * 0.0f)
#define TUV_THREE_NORMAL_END EVector2(1.0f, (1.0f / 3.0f) * 1.0f)
#define TUV_THREE_OVER_BEGIN EVector2(0.0f, (1.0f / 3.0f) * 1.0f)
#define TUV_THREE_OVER_END EVector2(1.0f, (1.0f / 3.0f) * 2.0f)
#define TUV_THREE_PRESSED_BEGIN EVector2(0.0f, (1.0f / 3.0f) * 2.0f)
#define TUV_THREE_PRESSED_END EVector2(1.0f, (1.0f / 3.0f) * 3.0f)

#define TUV_ONE_BEGIN (EVector2(0.0f, 0.0f))
#define TUV_ONE_END (EVector2(1.0f, 1.0f))

#define TSIZE_ROUND_WIDTH (77.0f)
#define TSIZE_ROUND_HEIGHT (46.0f)

#define TSIZEBASE_CHECKBOX_WIDTH (15.0f)
#define TSIZEBASE_CHECKBOX_HEIGHT (15.0f)
#define TSIZE_CHECKBOX_WIDTH(screenWidth) (TSIZEBASE_CHECKBOX_WIDTH * (screenWidth / TSCREENBASE_WIDTH))
#define TSIZE_CHECKBOX_HEIGHT(screenHeight) (TSIZEBASE_CHECKBOX_HEIGHT * (screenHeight / TSCREENBASE_HEIGHT))

#define TSIZEBASE_RADIOBUTTON_WIDTH (15.0f)
#define TSIZEBASE_RADIOBUTTON_HEIGHT (15.0f)
#define TSIZE_RADIOBUTTON_WIDTH(screenWidth) (TSIZEBASE_RADIOBUTTON_WIDTH * (screenWidth / TSCREENBASE_WIDTH))
#define TSIZE_RADIOBUTTON_HEIGHT(screenHeight) (TSIZEBASE_RADIOBUTTON_HEIGHT * (screenHeight / TSCREENBASE_HEIGHT))
#ifndef __GNUC__
#pragma endregion
#endif

#endif