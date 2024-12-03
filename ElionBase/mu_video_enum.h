#ifndef __MU_VIDEO_ENUM_H__
#define __MU_VIDEO_ENUM_H__

#pragma once

#define UI_VERTEX_INITIAL (10000 * 4)
#define UI_VERTEX_GROW (1000 * 4)
#define FONT_INSTANCE_INITIAL (50000)
#define FONT_INSTANCE_GROW (25000)
#define FONT_TEXT_GROW (32)

constexpr mu_float TextureMinLOD = 0.0f;
constexpr mu_float TextureMaxLOD = 3.402823466e+38f;

// Status Flags
enum EVideoStatusFlag : mu_uint32
{
	VSF_Active				= (1 << 0),
	VSF_Visible				= (VSF_Active << 1),
	VSF_Focused				= (VSF_Visible << 1),
	VSF_Available			= (VSF_Focused << 1),
	VSF_VerticalSync		= (VSF_Available << 1),
};

// Requests Flags
enum EVideoRequestFlag : mu_uint32
{
	VRF_ToggleVerticalSync	= (1 << 0),
	VRF_ResizeWindow		= (VRF_ToggleVerticalSync << 1),
	VRF_ToggleWindowMode	= (VRF_ResizeWindow << 1),
	VRF_ToggleMSAA			= (VRF_ToggleWindowMode << 1),
	VRF_ReloadShader		= (VRF_ToggleMSAA << 1),
	VRF_ToggleShadowMap		= (VRF_ReloadShader << 1),
};

enum class EVideoBackendType : mu_int32
{
	eNone = -1,
	eDirectX = 0,
	eOpenGL = 1,
	eVulkan = 2,
	eMetal = 3,
	eMax,
};

enum class EImageViewType : mu_uint32
{
	eNone,
	e1D,
	e1DArray,
	e2D,
	e2DArray,
	e3D,
	eCube,
	eMax,
};

constexpr mu_uint32 EImageViewTypeMax = static_cast<mu_uint32>(EImageViewType::eMax);

enum class ETextureFormat : mu_uint32
{
	eR8_Unorm,
	eR8_Snorm,
	eR8_Unsigned,
	eR8_Signed,

	eRG8_Unorm,
	eRG8_Snorm,
	eRG8_Unsigned,
	eRG8_Signed,

	eRGBA8_Unorm,
	eRGBA8_Snorm,
	eRGBA8_Unsigned,
	eRGBA8_Signed,

	eR16_Unorm,
	eR16_Snorm,
	eR16_Unsigned,
	eR16_Signed,
	eR16_Float,
	
	eRG16_Unorm,
	eRG16_Snorm,
	eRG16_Unsigned,
	eRG16_Signed,
	eRG16_Float,

	eRGBA16_Unorm,
	eRGBA16_Snorm,
	eRGBA16_Unsigned,
	eRGBA16_Signed,
	eRGBA16_Float,

	eR32_Unsigned,
	eR32_Signed,
	eR32_Float,

	eRG32_Unsigned,
	eRG32_Signed,
	eRG32_Float,

	eRGB32_Unsigned,
	eRGB32_Signed,
	eRGB32_Float,

	eRGBA32_Unsigned,
	eRGBA32_Signed,
	eRGBA32_Float,

	eDepth16_Unorm,
	eDepth32_Float,

	eMax,
};

constexpr mu_uint32 ETextureFormatMax = static_cast<mu_uint32>(ETextureFormat::eMax);
extern const mu_uint32 ETextureComponents[ETextureFormatMax];

enum class ETextureType : mu_uint8
{
	eTexture1D,
	eTexture2D,
	eTexture3D,
	eTextureCube,
	eMax,
};

constexpr mu_uint32 ETextureTypeMax = static_cast<mu_uint32>(ETextureType::eMax);

enum class ETextureUsage : mu_uint32
{
	eStatic,
	eDynamic,
	eStream,
	eMax,
};

constexpr mu_uint32 ETextureUsageMax = static_cast<mu_uint32>(ETextureUsage::eMax);

enum class ETextureClassType : mu_uint8
{
	eTexture,
	eTextureBuffer,
	eMax,
};

enum class EBufferUsage : mu_uint8
{
	eStatic,
	eDynamic,
	eStream,
	eMax,
};

enum class EBufferType : mu_uint8
{
	eUniform,
	eVertex,
	eIndex,
	eInstance,
	eMax,
};

enum class EGlobalConstant : mu_uint8
{
	eGame,
	eShadow,
	eUI,
	eMax,
};

constexpr mu_uint32 EGlobalConstantMax = static_cast<mu_uint32>(EGlobalConstant::eMax);

enum class EBlendState : mu_uint8
{
	eOne_Zero,
	eAlphaSrc_InvAlphaSrc,
	eOne_One,
	eZero_InvColorSrc,
	eInvColorSrc_One,
	eOne_InvColorSrc,
	eZero_ColorSrc,
	eMax,

	eInvalid = 0xFF,
};

constexpr mu_uint32 EBlendStateMax = static_cast<mu_uint32>(EBlendState::eMax);

enum class EDepthStencil : mu_uint8
{
	// Non Early-Z
	eZBuffer,
	eDepthMaskPlusEqual,

	// Early-Z
	eEarlyZBegin,
	eNoZBuffer = eEarlyZBegin,
	eNoDepthMask,
	eNoDepthMaskPlusEqual,
	eShadow,

	eMax,

	eInvalid = 0xFF,
};

constexpr mu_uint32 EDepthStencilMax = static_cast<mu_uint32>(EDepthStencil::eMax);

enum class ERasterizer : mu_uint8
{
	eFrontOnly,
	eBackOnly,
	eNoCull,
	eMax,

	eInvalid = 0xFF,
};

constexpr mu_uint32 ERasterizerMax = static_cast<mu_uint32>(ERasterizer::eMax);

/*
	UVW Address : R(Repeat), MR(Mirrored Repeat), C(Clamp To Edge), CB(Clamp To Border), MC(Mirror Clamp To Edge)
	Min-Mag-Mipmap : N(Nearest), L(Linear)
	Anisotropic : E(Enabled), D(Disabled)
*/
enum class ESampler : mu_uint8
{
	// Linears
	eUcVcWcMinLMagLMipL,
	eUcbVcbWcbMinLMagLMipL,
	eUrVrWrMinLMagLMipL,

	// Nearests
	eUcVcWcMinNMagNMipN,
	eUcbVcbWcbMinNMagNMipN,
	eUrVrWrMinNMagNMipN,

	eBeginAnisotropy,

	// Anisotropy Linears
	eUcVcWcMinLMagLMipL_Anisotropy = eBeginAnisotropy,
	eUcbVcbWcbMinLMagLMipL_Anisotropy,
	eUrVrWrMinLMagLMipL_Anisotropy,

	// Anisotropy Nearests
	eUcVcWcMinNMagNMipN_Anisotropy,
	eUcbVcbWcbMinNMagNMipN_Anisotropy,
	eUrVrWrMinNMagNMipN_Anisotropy,

	eEndAnisotropy = eUrVrWrMinNMagNMipN_Anisotropy,

	// Shadow Comparison
	eShadowPCF,
	eShadow,

	eMax,

	eInvalid = 0xFF,
};

constexpr mu_uint32 ESamplerMax = static_cast<mu_uint32>(ESampler::eMax);

enum class EEffectSampler : mu_uint8
{
	eUrVrWrMinLMagLMipL_Anisotropy, // Default
	eUcbVcbWcbMinLMagLMipL_Anisotropy,
	eUcVcWcMinNMagNMipN,
	eMax,
};

constexpr mu_uint32 EEffectSamplerMax = static_cast<mu_uint32>(EEffectSampler::eMax);
extern const mu_uint8 EffectSamplerMap[ESamplerMax];
extern const ESampler EffectSamplers[EEffectSamplerMax];
extern const mu_boolean EffectSamplersMipmap[EEffectSamplerMax];

enum class ETopologyType : mu_uint8
{
	eTriangleList,
	eTriangleStrip,
	eMax,

	eInvalid = 0xFF,
};

constexpr mu_uint32 ETopologyTypeMax = static_cast<mu_uint32>(ETopologyType::eMax);

enum class EIndexType : mu_uint8
{
	eIndex16,
	eIndex32,
	eMax,
};

constexpr mu_uint32 EIndexTypeMax = static_cast<mu_uint32>(EIndexType::eMax);

enum class ERenderPass : mu_uint8
{
	eNormal,
	eShadow,
	eMax,
};

constexpr mu_uint32 ERenderPassMax = static_cast<mu_uint32>(ERenderPass::eMax);

enum class EShadowType : mu_uint8
{
	eNonShadowMappingBegin = 0,
	eNone = eNonShadowMappingBegin,
	eOriginal,
	eOriginalPlus,
	eNonShadowMappingEnd = eOriginalPlus,

	eShadowMappingBegin,
	ePCF = eShadowMappingBegin,
	eShadowMappingEnd = ePCF,

	eMax,
};

constexpr mu_uint32 EShadowTypeMax = static_cast<mu_uint32>(EShadowType::eMax);

namespace BLEND
{
	enum RENDER_TYPE
	{
		TXB_ALPHATEST_COUNT = 3,

		TXB_DEPTH_BEGIN = 0,
		TXB_ALPHATEST_BEGIN = TXB_DEPTH_BEGIN,
		TXB_ALPHATEST_END = TXB_ALPHATEST_BEGIN + TXB_ALPHATEST_COUNT - 1,
		TXB_ALPHATEST_NOMASK,
		TXB_ALPHATEST_NONDEPTH,
		TXB_DEPTH_END,

		TXB_NON_DEPTH_BEGIN = TXB_DEPTH_END,
		TXB_BLEND01 = TXB_NON_DEPTH_BEGIN,
		TXB_BLENDMINUS,
		TXB_BLEND02,
		TXB_BLEND03,
		TXB_BLEND04,
		TXB_LIGHTMAP,
		TXB_PRE_BLEND_BEGIN,
		TXB_PRE_BLEND01 = TXB_PRE_BLEND_BEGIN,
		TXB_PRE_BLENDMINUS,
		TXB_PRE_BLEND02,
		TXB_PRE_BLEND03,
		TXB_PRE_BLEND04,
		TXB_PRE_LIGHTMAP,
		TXB_PRE_BLEND_END,
		TXB_NON_DEPTH_END = TXB_PRE_BLEND_END,

		TXB_MAX = TXB_NON_DEPTH_END,
	};
};

#define TEXTURE_MODULATE 0.0f
#define TEXTURE_ADD 1.0f

constexpr mu_uint8 EInvalidUint32 = 0xFF;
constexpr mu_uint32 EInvalidUInt32 = 0xFFFFFFFF;

#define ELION_TEXTURETYPE_DDS (0)
#define ELION_TEXTURETYPE_KTX (1)

#if defined(__ANDROID__) || defined(ELION_FORCE_KTX)
#define ELION_TEXTURETYPE_FORMAT (ELION_TEXTURETYPE_KTX)
#else
#define ELION_TEXTURETYPE_FORMAT (ELION_TEXTURETYPE_DDS)
#endif

template<typename T>
constexpr mu_uint32 ECastEnum(const T value)
{
	return static_cast<mu_uint32>(value);
}

constexpr mu_uint32 GetTextureComponents(const ETextureFormat format)
{
	return ETextureComponents[ECastEnum(format)];
}

#endif