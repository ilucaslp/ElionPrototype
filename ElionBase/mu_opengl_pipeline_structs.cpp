#include "stdafx.h"

#if OPENGL_BACKEND_ENABLE == 1
#include "mu_opengl_pipeline_structs.h"

namespace EOpenGLPipeline
{
	EBlendStateData BlendStateData[EBlendStateMax] =
	{
		// One / Zero
		EBlendStateData(GL_ONE, GL_ZERO, GL_FUNC_ADD, GL_ONE, GL_ZERO, GL_FUNC_ADD),
		// Alpha Src / Inverse Alpha Src
		EBlendStateData(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_FUNC_ADD, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_FUNC_ADD),
		// One / One
		EBlendStateData(GL_ONE, GL_ONE, GL_FUNC_ADD, GL_ONE, GL_ONE, GL_FUNC_ADD),
		// Zero / Inverse Color Src
		EBlendStateData(GL_ZERO, GL_ONE_MINUS_SRC_COLOR, GL_FUNC_ADD, GL_ONE, GL_ONE, GL_FUNC_ADD),
		// Inverse Color Src / One
		EBlendStateData(GL_ONE_MINUS_SRC_COLOR, GL_ONE, GL_FUNC_ADD, GL_ONE, GL_ONE, GL_FUNC_ADD),
		// One / Inverse Color Src
		EBlendStateData(GL_ONE, GL_ONE_MINUS_SRC_COLOR, GL_FUNC_ADD, GL_ONE, GL_ONE, GL_FUNC_ADD),
		// Zero / One
		EBlendStateData(GL_ZERO, GL_ONE, GL_FUNC_ADD, GL_ZERO, GL_ONE, GL_FUNC_ADD),
	};

	EDepthStencilData DepthStencilData[EDepthStencilMax] =
	{
#if OPENGL_REVERSED_DEPTH == 1
		// Enable Depth Test / Enable Depth Mask / Disable Stencil
		EDepthStencilData(true, true, GL_GREATER, false),
		// Enable Depth Test / Enable Depth Mask / Disable Stencil (Greater-or-Equal)
		EDepthStencilData(true, true, GL_GEQUAL, false),
		// Early-Z
		// Disable Depth Test / Disable Depth Mask / Disable Stencil
		EDepthStencilData(true, false, GL_GREATER, false),
		// Enable Depth Test / Disable Depth Mask / Disable Stencil
		EDepthStencilData(true, false, GL_GREATER, false),
		// Enable Depth Test / Disable Depth Mask / Disable Stencil (Greater-or-Equal)
		EDepthStencilData(true, false, GL_GEQUAL, false),
		// Enable Depth Test / Disable Depth Mask / Enable Stencil (Used for Shadows)
		EDepthStencilData(true, false, GL_GEQUAL, true),
#else
		// Enable Depth Test / Enable Depth Mask / Disable Stencil
		EDepthStencilData(true, true, GL_LESS, false),
		// Enable Depth Test / Enable Depth Mask / Disable Stencil (Greater-or-Equal)
		EDepthStencilData(true, true, GL_LEQUAL, false),
		// Early-Z
		// Disable Depth Test / Disable Depth Mask / Disable Stencil
		EDepthStencilData(true, false, GL_LESS, false),
		// Enable Depth Test / Disable Depth Mask / Disable Stencil
		EDepthStencilData(true, false, GL_LESS, false),
		// Enable Depth Test / Disable Depth Mask / Disable Stencil (Greater-or-Equal)
		EDepthStencilData(true, false, GL_LEQUAL, false),
		// Enable Depth Test / Disable Depth Mask / Enable Stencil (Used for Shadows)
		EDepthStencilData(true, false, GL_LEQUAL, true),
#endif
	};

	ERasterizerData RasterizerData[ERasterizerMax] =
	{
		ERasterizerData(GL_FRONT, GL_CW),
		ERasterizerData(GL_BACK, GL_CW),
		ERasterizerData(GL_NONE, GL_CW),
	};

	ESamplerData SamplerData[ESamplerMax * 2] =
	{
		// Linears
		ESamplerData(GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_NONE, GL_NEVER),
		ESamplerData(GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, GL_NONE, GL_NEVER),
		ESamplerData(GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT, GL_REPEAT, GL_NONE, GL_NEVER),

		// Nearests
		ESamplerData(GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_NONE, GL_NEVER),
		ESamplerData(GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, GL_NONE, GL_NEVER),
		ESamplerData(GL_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT, GL_REPEAT, GL_NONE, GL_NEVER),

		// Anisotropy Linears
		ESamplerData(GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_NONE, GL_NEVER),
		ESamplerData(GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, GL_NONE, GL_NEVER),
		ESamplerData(GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT, GL_REPEAT, GL_NONE, GL_NEVER),

		// Anisotropy Nearests
		ESamplerData(GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_NONE, GL_NEVER),
		ESamplerData(GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, GL_NONE, GL_NEVER),
		ESamplerData(GL_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT, GL_REPEAT, GL_NONE, GL_NEVER),

		// Shadow Comparison
#if OPENGL_REVERSED_DEPTH == 1
		ESamplerData(GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_COMPARE_REF_TO_TEXTURE, GL_GEQUAL),
		ESamplerData(GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_COMPARE_REF_TO_TEXTURE, GL_GREATER),
#else
		ESamplerData(GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_COMPARE_REF_TO_TEXTURE, GL_LEQUAL),
		ESamplerData(GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_COMPARE_REF_TO_TEXTURE, GL_LESS),
#endif

		// Linears
		ESamplerData(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_NONE, GL_NEVER),
		ESamplerData(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, GL_NONE, GL_NEVER),
		ESamplerData(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT, GL_REPEAT, GL_NONE, GL_NEVER),

		// Nearests
		ESamplerData(GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_NONE, GL_NEVER),
		ESamplerData(GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, GL_NONE, GL_NEVER),
		ESamplerData(GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT, GL_REPEAT, GL_NONE, GL_NEVER),

		// Anisotropy Linears
		ESamplerData(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_NONE, GL_NEVER),
		ESamplerData(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, GL_NONE, GL_NEVER),
		ESamplerData(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT, GL_REPEAT, GL_NONE, GL_NEVER),

		// Anisotropy Nearests
		ESamplerData(GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_NONE, GL_NEVER),
		ESamplerData(GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, GL_NONE, GL_NEVER),
		ESamplerData(GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT, GL_REPEAT, GL_NONE, GL_NEVER),

		// Shadow Comparison
#if OPENGL_REVERSED_DEPTH == 1
		ESamplerData(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT, GL_REPEAT, GL_COMPARE_REF_TO_TEXTURE, GL_GEQUAL),
		ESamplerData(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT, GL_REPEAT, GL_COMPARE_REF_TO_TEXTURE, GL_GREATER),
#else
		ESamplerData(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT, GL_REPEAT, GL_COMPARE_REF_TO_TEXTURE, GL_LEQUAL),
		ESamplerData(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT, GL_REPEAT, GL_COMPARE_REF_TO_TEXTURE, GL_LESS),
#endif
	};
};
#endif