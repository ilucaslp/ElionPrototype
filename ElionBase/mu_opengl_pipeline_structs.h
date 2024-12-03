#ifndef __MU_OPENGL_PIPELINE_STRUCTS_H__
#define __MU_OPENGL_PIPELINE_STRUCTS_H__

#pragma once

namespace EOpenGLPipeline
{
	struct EBlendStateData
	{
		EBlendStateData(const GLenum rgbSrc,
			const GLenum rgbDest,
			const GLenum rgbMode,
			const GLenum alphaSrc,
			const GLenum alphaDest,
			const GLenum alphaMode) :
			RGBSrc(rgbSrc),
			RGBDest(rgbDest),
			RGBMode(rgbMode),
			AlphaSrc(alphaSrc),
			AlphaDest(alphaDest),
			AlphaMode(alphaMode)
		{
		}

		const GLenum RGBSrc;
		const GLenum RGBDest;
		const GLenum RGBMode;
		const GLenum AlphaSrc;
		const GLenum AlphaDest;
		const GLenum AlphaMode;
	};

	struct EDepthStencilData
	{
		EDepthStencilData(const mu_boolean depthEnable,
			const mu_boolean depthMask,
			const GLenum depthFunc,
			const mu_boolean stencilEnable) :
			DepthEnable(depthEnable),
			DepthMask(depthMask),
			DepthFunc(depthFunc),
			StencilEnable(stencilEnable)
		{
		}

		const mu_boolean DepthEnable;
		const mu_boolean DepthMask;
		const GLenum DepthFunc;
		const mu_boolean StencilEnable;
	};

	struct ERasterizerData
	{
		ERasterizerData(const GLenum cullFace,
			const GLenum frontFace) :
			CullFace(cullFace),
			FrontFace(frontFace)
		{
		}

		const GLenum CullFace;
		const GLenum FrontFace;
	};

	struct ESamplerData
	{
		ESamplerData(const GLint minMapFilter,
			const GLint magFilter,
			const GLint addressU,
			const GLint addressV,
			const GLint addressW,
			const GLint compareMode,
			const GLint compareFunc) :
			MinMapFilter(minMapFilter),
			MagFilter(magFilter),
			AddressU(addressU),
			AddressV(addressV),
			AddressW(addressW),
			CompareMode(compareMode),
			CompareFunc(compareFunc)
		{
		}

		const GLint MinMapFilter;
		const GLint MagFilter;
		const GLint AddressU;
		const GLint AddressV;
		const GLint AddressW;
		const GLint CompareMode;
		const GLint CompareFunc;
	};

	extern EBlendStateData BlendStateData[EBlendStateMax];
	extern EDepthStencilData DepthStencilData[EDepthStencilMax];
	extern ERasterizerData RasterizerData[ERasterizerMax];
	extern ESamplerData SamplerData[ESamplerMax * 2]; // Multiplied by 2 due to Mipmap
};

#endif