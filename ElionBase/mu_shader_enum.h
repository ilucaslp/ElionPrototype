#ifndef __MU_SHADER_ENUM_H__
#define __MU_SHADER_ENUM_H__

#pragma once

enum class EShaderType : mu_uint32
{
	eVertex,
	eFragment,
	eGeometry,
	eMax,
};

enum class EInputLayout : mu_uint32
{
	eUIDefault,
	eFont,
	eTerrainCommon,
	eGrass,
	eWater,
	eModel,
	eCloth,
	eEffect,
	eMax,

	eInvalid = 0xFFFFFFFF,
};

constexpr mu_uint32 EInputLayoutMax = static_cast<mu_uint32>(EInputLayout::eMax);

enum class EUniformLayout : mu_uint32
{
	eGlobalConstantOnly,
	eUIDefault,
	eFont,
	eTerrainCommon,
	eGrass,
	eWater,
	eModel,
	eShadow,
	eEffect,
	eMax,
};

constexpr mu_uint32 EUniformLayoutMax = static_cast<mu_uint32>(EUniformLayout::eMax);

enum class EVertexShader : mu_uint32
{
	eUIDefault,
	eUIColor,

	eFont,

	eTerrainCommon,
	eTerrainAtlans,
	eTerrainHeaven,
	eTerrainCastle,
	
	eGrass,
	eWater1,
	eWater2,

	eModelNormal,
	eModelColor,
	eModelChrome01,
	eModelChrome02,
	eModelChrome03,
	eModelChrome04,
	eModelChrome05,
	eModelChrome06,
	eModelChrome07,
	eModelMetal,
	eModelOil,

	eClothNormal,
	eClothColor,
	eClothChrome01,
	eClothChrome02,
	eClothChrome03,
	eClothChrome04,
	eClothChrome05,
	eClothChrome06,
	eClothChrome07,
	eClothMetal,
	eClothOil,

	eModelShadowNormal,
	eModelShadowChrome01,
	eModelShadowChrome02,
	eModelShadowChrome03,
	eModelShadowChrome04,
	eModelShadowChrome05,
	eModelShadowChrome06,
	eModelShadowChrome07,
	eModelShadowMetal,
	eModelShadowOil,

	eClothShadowNormal,

	eEffect,

	eMax,

	eInvalid = 0xFFFFFFFF,
};

enum class EFragmentShader : mu_uint32
{
	eUIDefault,
	eUIColor,
	eUILimitedAlpha,

	eFont,

	eTerrainCommon,
	
	eGrass,
	eWater1,
	eWater2,

	eModelNormal,
	eModelColor,
	eModelNormalEZ,
	eModelColorEZ,

	eShadowAlpha,
	eShadowBlend,

	eEffect,
	eEffectEZ,

	eMax,

	eInvalid = 0xFFFFFFFF,
};

#define GEOMETRY_SHADER_ENABLE (0) // Enable only when added any geometry shader

enum class EGeometryShader : mu_uint32
{
	eDummy,

	eMax,

	eInvalid = 0xFFFFFFFF,
};

enum class ECombinedShader : mu_uint32
{
	eBeginEarly,

	eUIDefault = eBeginEarly,
	eUIColor,
	eUILimitedAlpha,

	eFont,

	eEndEarly,
	eBeginLate = eEndEarly,

	eTerrainCommon = eBeginLate,
	eTerrainAtlans,
	eTerrainHeaven,
	eTerrainCastle,
	
	eGrass,
	eWater1,
	eWater2,

	eModelNormal,
	eModelColor,
	eModelChrome01,
	eModelChrome02,
	eModelChrome03,
	eModelChrome04,
	eModelChrome05,
	eModelChrome06,
	eModelChrome07,
	eModelMetal,
	eModelOil,

	eModelNormalEZ,
	eModelColorEZ,
	eModelChrome01EZ,
	eModelChrome02EZ,
	eModelChrome03EZ,
	eModelChrome04EZ,
	eModelChrome05EZ,
	eModelChrome06EZ,
	eModelChrome07EZ,
	eModelMetalEZ,
	eModelOilEZ,

	eClothNormal,
	eClothColor,
	eClothChrome01,
	eClothChrome02,
	eClothChrome03,
	eClothChrome04,
	eClothChrome05,
	eClothChrome06,
	eClothChrome07,
	eClothMetal,
	eClothOil,

	eClothNormalEZ,
	eClothColorEZ,
	eClothChrome01EZ,
	eClothChrome02EZ,
	eClothChrome03EZ,
	eClothChrome04EZ,
	eClothChrome05EZ,
	eClothChrome06EZ,
	eClothChrome07EZ,
	eClothMetalEZ,
	eClothOilEZ,

	eModelShadowNormalAlpha,
	eModelShadowChrome01Alpha,
	eModelShadowChrome02Alpha,
	eModelShadowChrome03Alpha,
	eModelShadowChrome04Alpha,
	eModelShadowChrome05Alpha,
	eModelShadowChrome06Alpha,
	eModelShadowChrome07Alpha,
	eModelShadowMetalAlpha,
	eModelShadowOilAlpha,

	eModelShadowNormalBlend,
	eModelShadowChrome01Blend,
	eModelShadowChrome02Blend,
	eModelShadowChrome03Blend,
	eModelShadowChrome04Blend,
	eModelShadowChrome05Blend,
	eModelShadowChrome06Blend,
	eModelShadowChrome07Blend,
	eModelShadowMetalBlend,
	eModelShadowOilBlend,

	eClothShadowNormalAlpha,
	eClothShadowNormalBlend,

	eEffect,
	eEffectEZ,

	eEndLate,
	eMax = eEndLate,

	eInvalid = 0xFFFFFFFF,
};

constexpr mu_uint32 EVertexShaderMax = static_cast<mu_uint32>(EVertexShader::eMax);
constexpr mu_uint32 EFragmentShaderMax = static_cast<mu_uint32>(EFragmentShader::eMax);
constexpr mu_uint32 EGeometryShaderMax = static_cast<mu_uint32>(EGeometryShader::eMax);
constexpr mu_uint32 ECombinedShaderMax = static_cast<mu_uint32>(ECombinedShader::eMax);

#endif