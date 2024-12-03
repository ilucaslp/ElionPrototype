#include "stdafx.h"

ECombinedShaderData CombinedShaderList[ECombinedShaderMax] =
{
	ECombinedShaderData(EVertexShader::eUIDefault, EFragmentShader::eUIDefault, EGeometryShader::eInvalid, EInputLayout::eUIDefault, EUniformLayout::eUIDefault, ETopologyType::eTriangleStrip, ERenderPass::eNormal, true),
	ECombinedShaderData(EVertexShader::eUIColor, EFragmentShader::eUIColor, EGeometryShader::eInvalid, EInputLayout::eUIDefault, EUniformLayout::eUIDefault, ETopologyType::eTriangleStrip, ERenderPass::eNormal, true),
	ECombinedShaderData(EVertexShader::eUIDefault, EFragmentShader::eUILimitedAlpha, EGeometryShader::eInvalid, EInputLayout::eUIDefault, EUniformLayout::eUIDefault, ETopologyType::eTriangleStrip, ERenderPass::eNormal, true),
	ECombinedShaderData(EVertexShader::eFont, EFragmentShader::eFont, EGeometryShader::eInvalid, EInputLayout::eFont, EUniformLayout::eFont, ETopologyType::eTriangleList, ERenderPass::eNormal, true),

	ECombinedShaderData(EVertexShader::eTerrainCommon, EFragmentShader::eTerrainCommon, EGeometryShader::eInvalid, EInputLayout::eTerrainCommon, EUniformLayout::eTerrainCommon, ETopologyType::eTriangleList, ERenderPass::eNormal, false),
	ECombinedShaderData(EVertexShader::eTerrainAtlans, EFragmentShader::eTerrainCommon, EGeometryShader::eInvalid, EInputLayout::eTerrainCommon, EUniformLayout::eTerrainCommon, ETopologyType::eTriangleList, ERenderPass::eNormal, false),
	ECombinedShaderData(EVertexShader::eTerrainHeaven, EFragmentShader::eTerrainCommon, EGeometryShader::eInvalid, EInputLayout::eTerrainCommon, EUniformLayout::eTerrainCommon, ETopologyType::eTriangleList, ERenderPass::eNormal, false),
	ECombinedShaderData(EVertexShader::eTerrainCastle, EFragmentShader::eTerrainCommon, EGeometryShader::eInvalid, EInputLayout::eTerrainCommon, EUniformLayout::eTerrainCommon, ETopologyType::eTriangleList, ERenderPass::eNormal, false),

	ECombinedShaderData(EVertexShader::eGrass, EFragmentShader::eGrass, EGeometryShader::eInvalid, EInputLayout::eGrass, EUniformLayout::eGrass, ETopologyType::eTriangleList, ERenderPass::eNormal, false),
	ECombinedShaderData(EVertexShader::eWater1, EFragmentShader::eWater1, EGeometryShader::eInvalid, EInputLayout::eWater, EUniformLayout::eWater, ETopologyType::eTriangleList, ERenderPass::eNormal, false),
	ECombinedShaderData(EVertexShader::eWater2, EFragmentShader::eWater2, EGeometryShader::eInvalid, EInputLayout::eWater, EUniformLayout::eWater, ETopologyType::eTriangleList, ERenderPass::eNormal, false),

	ECombinedShaderData(EVertexShader::eModelNormal, EFragmentShader::eModelNormal, EGeometryShader::eInvalid, EInputLayout::eModel, EUniformLayout::eModel, ETopologyType::eTriangleList, ERenderPass::eNormal, false),
	ECombinedShaderData(EVertexShader::eModelColor, EFragmentShader::eModelColor, EGeometryShader::eInvalid, EInputLayout::eModel, EUniformLayout::eModel, ETopologyType::eTriangleList, ERenderPass::eNormal, false),
	ECombinedShaderData(EVertexShader::eModelChrome01, EFragmentShader::eModelNormal, EGeometryShader::eInvalid, EInputLayout::eModel, EUniformLayout::eModel, ETopologyType::eTriangleList, ERenderPass::eNormal, false),
	ECombinedShaderData(EVertexShader::eModelChrome02, EFragmentShader::eModelNormal, EGeometryShader::eInvalid, EInputLayout::eModel, EUniformLayout::eModel, ETopologyType::eTriangleList, ERenderPass::eNormal, false),
	ECombinedShaderData(EVertexShader::eModelChrome03, EFragmentShader::eModelNormal, EGeometryShader::eInvalid, EInputLayout::eModel, EUniformLayout::eModel, ETopologyType::eTriangleList, ERenderPass::eNormal, false),
	ECombinedShaderData(EVertexShader::eModelChrome04, EFragmentShader::eModelNormal, EGeometryShader::eInvalid, EInputLayout::eModel, EUniformLayout::eModel, ETopologyType::eTriangleList, ERenderPass::eNormal, false),
	ECombinedShaderData(EVertexShader::eModelChrome05, EFragmentShader::eModelNormal, EGeometryShader::eInvalid, EInputLayout::eModel, EUniformLayout::eModel, ETopologyType::eTriangleList, ERenderPass::eNormal, false),
	ECombinedShaderData(EVertexShader::eModelChrome06, EFragmentShader::eModelNormal, EGeometryShader::eInvalid, EInputLayout::eModel, EUniformLayout::eModel, ETopologyType::eTriangleList, ERenderPass::eNormal, false),
	ECombinedShaderData(EVertexShader::eModelChrome07, EFragmentShader::eModelNormal, EGeometryShader::eInvalid, EInputLayout::eModel, EUniformLayout::eModel, ETopologyType::eTriangleList, ERenderPass::eNormal, false),
	ECombinedShaderData(EVertexShader::eModelMetal, EFragmentShader::eModelNormal, EGeometryShader::eInvalid, EInputLayout::eModel, EUniformLayout::eModel, ETopologyType::eTriangleList, ERenderPass::eNormal, false),
	ECombinedShaderData(EVertexShader::eModelOil, EFragmentShader::eModelNormal, EGeometryShader::eInvalid, EInputLayout::eModel, EUniformLayout::eModel, ETopologyType::eTriangleList, ERenderPass::eNormal, false),

	ECombinedShaderData(EVertexShader::eModelNormal, EFragmentShader::eModelNormalEZ, EGeometryShader::eInvalid, EInputLayout::eModel, EUniformLayout::eModel, ETopologyType::eTriangleList, ERenderPass::eNormal, false),
	ECombinedShaderData(EVertexShader::eModelColor, EFragmentShader::eModelColorEZ, EGeometryShader::eInvalid, EInputLayout::eModel, EUniformLayout::eModel, ETopologyType::eTriangleList, ERenderPass::eNormal, false),
	ECombinedShaderData(EVertexShader::eModelChrome01, EFragmentShader::eModelNormalEZ, EGeometryShader::eInvalid, EInputLayout::eModel, EUniformLayout::eModel, ETopologyType::eTriangleList, ERenderPass::eNormal, false),
	ECombinedShaderData(EVertexShader::eModelChrome02, EFragmentShader::eModelNormalEZ, EGeometryShader::eInvalid, EInputLayout::eModel, EUniformLayout::eModel, ETopologyType::eTriangleList, ERenderPass::eNormal, false),
	ECombinedShaderData(EVertexShader::eModelChrome03, EFragmentShader::eModelNormalEZ, EGeometryShader::eInvalid, EInputLayout::eModel, EUniformLayout::eModel, ETopologyType::eTriangleList, ERenderPass::eNormal, false),
	ECombinedShaderData(EVertexShader::eModelChrome04, EFragmentShader::eModelNormalEZ, EGeometryShader::eInvalid, EInputLayout::eModel, EUniformLayout::eModel, ETopologyType::eTriangleList, ERenderPass::eNormal, false),
	ECombinedShaderData(EVertexShader::eModelChrome05, EFragmentShader::eModelNormalEZ, EGeometryShader::eInvalid, EInputLayout::eModel, EUniformLayout::eModel, ETopologyType::eTriangleList, ERenderPass::eNormal, false),
	ECombinedShaderData(EVertexShader::eModelChrome06, EFragmentShader::eModelNormalEZ, EGeometryShader::eInvalid, EInputLayout::eModel, EUniformLayout::eModel, ETopologyType::eTriangleList, ERenderPass::eNormal, false),
	ECombinedShaderData(EVertexShader::eModelChrome07, EFragmentShader::eModelNormalEZ, EGeometryShader::eInvalid, EInputLayout::eModel, EUniformLayout::eModel, ETopologyType::eTriangleList, ERenderPass::eNormal, false),
	ECombinedShaderData(EVertexShader::eModelMetal, EFragmentShader::eModelNormalEZ, EGeometryShader::eInvalid, EInputLayout::eModel, EUniformLayout::eModel, ETopologyType::eTriangleList, ERenderPass::eNormal, false),
	ECombinedShaderData(EVertexShader::eModelOil, EFragmentShader::eModelNormalEZ, EGeometryShader::eInvalid, EInputLayout::eModel, EUniformLayout::eModel, ETopologyType::eTriangleList, ERenderPass::eNormal, false),

	ECombinedShaderData(EVertexShader::eClothNormal, EFragmentShader::eModelNormal, EGeometryShader::eInvalid, EInputLayout::eCloth, EUniformLayout::eModel, ETopologyType::eTriangleList, ERenderPass::eNormal, false),
	ECombinedShaderData(EVertexShader::eClothColor, EFragmentShader::eModelColor, EGeometryShader::eInvalid, EInputLayout::eCloth, EUniformLayout::eModel, ETopologyType::eTriangleList, ERenderPass::eNormal, false),
	ECombinedShaderData(EVertexShader::eClothChrome01, EFragmentShader::eModelNormal, EGeometryShader::eInvalid, EInputLayout::eCloth, EUniformLayout::eModel, ETopologyType::eTriangleList, ERenderPass::eNormal, false),
	ECombinedShaderData(EVertexShader::eClothChrome02, EFragmentShader::eModelNormal, EGeometryShader::eInvalid, EInputLayout::eCloth, EUniformLayout::eModel, ETopologyType::eTriangleList, ERenderPass::eNormal, false),
	ECombinedShaderData(EVertexShader::eClothChrome03, EFragmentShader::eModelNormal, EGeometryShader::eInvalid, EInputLayout::eCloth, EUniformLayout::eModel, ETopologyType::eTriangleList, ERenderPass::eNormal, false),
	ECombinedShaderData(EVertexShader::eClothChrome04, EFragmentShader::eModelNormal, EGeometryShader::eInvalid, EInputLayout::eCloth, EUniformLayout::eModel, ETopologyType::eTriangleList, ERenderPass::eNormal, false),
	ECombinedShaderData(EVertexShader::eClothChrome05, EFragmentShader::eModelNormal, EGeometryShader::eInvalid, EInputLayout::eCloth, EUniformLayout::eModel, ETopologyType::eTriangleList, ERenderPass::eNormal, false),
	ECombinedShaderData(EVertexShader::eClothChrome06, EFragmentShader::eModelNormal, EGeometryShader::eInvalid, EInputLayout::eCloth, EUniformLayout::eModel, ETopologyType::eTriangleList, ERenderPass::eNormal, false),
	ECombinedShaderData(EVertexShader::eClothChrome07, EFragmentShader::eModelNormal, EGeometryShader::eInvalid, EInputLayout::eCloth, EUniformLayout::eModel, ETopologyType::eTriangleList, ERenderPass::eNormal, false),
	ECombinedShaderData(EVertexShader::eClothMetal, EFragmentShader::eModelNormal, EGeometryShader::eInvalid, EInputLayout::eCloth, EUniformLayout::eModel, ETopologyType::eTriangleList, ERenderPass::eNormal, false),
	ECombinedShaderData(EVertexShader::eClothOil, EFragmentShader::eModelNormal, EGeometryShader::eInvalid, EInputLayout::eCloth, EUniformLayout::eModel, ETopologyType::eTriangleList, ERenderPass::eNormal, false),

	ECombinedShaderData(EVertexShader::eClothNormal, EFragmentShader::eModelNormalEZ, EGeometryShader::eInvalid, EInputLayout::eCloth, EUniformLayout::eModel, ETopologyType::eTriangleList, ERenderPass::eNormal, false),
	ECombinedShaderData(EVertexShader::eClothColor, EFragmentShader::eModelColorEZ, EGeometryShader::eInvalid, EInputLayout::eCloth, EUniformLayout::eModel, ETopologyType::eTriangleList, ERenderPass::eNormal, false),
	ECombinedShaderData(EVertexShader::eClothChrome01, EFragmentShader::eModelNormalEZ, EGeometryShader::eInvalid, EInputLayout::eCloth, EUniformLayout::eModel, ETopologyType::eTriangleList, ERenderPass::eNormal, false),
	ECombinedShaderData(EVertexShader::eClothChrome02, EFragmentShader::eModelNormalEZ, EGeometryShader::eInvalid, EInputLayout::eCloth, EUniformLayout::eModel, ETopologyType::eTriangleList, ERenderPass::eNormal, false),
	ECombinedShaderData(EVertexShader::eClothChrome03, EFragmentShader::eModelNormalEZ, EGeometryShader::eInvalid, EInputLayout::eCloth, EUniformLayout::eModel, ETopologyType::eTriangleList, ERenderPass::eNormal, false),
	ECombinedShaderData(EVertexShader::eClothChrome04, EFragmentShader::eModelNormalEZ, EGeometryShader::eInvalid, EInputLayout::eCloth, EUniformLayout::eModel, ETopologyType::eTriangleList, ERenderPass::eNormal, false),
	ECombinedShaderData(EVertexShader::eClothChrome05, EFragmentShader::eModelNormalEZ, EGeometryShader::eInvalid, EInputLayout::eCloth, EUniformLayout::eModel, ETopologyType::eTriangleList, ERenderPass::eNormal, false),
	ECombinedShaderData(EVertexShader::eClothChrome06, EFragmentShader::eModelNormalEZ, EGeometryShader::eInvalid, EInputLayout::eCloth, EUniformLayout::eModel, ETopologyType::eTriangleList, ERenderPass::eNormal, false),
	ECombinedShaderData(EVertexShader::eClothChrome07, EFragmentShader::eModelNormalEZ, EGeometryShader::eInvalid, EInputLayout::eCloth, EUniformLayout::eModel, ETopologyType::eTriangleList, ERenderPass::eNormal, false),
	ECombinedShaderData(EVertexShader::eClothMetal, EFragmentShader::eModelNormalEZ, EGeometryShader::eInvalid, EInputLayout::eCloth, EUniformLayout::eModel, ETopologyType::eTriangleList, ERenderPass::eNormal, false),
	ECombinedShaderData(EVertexShader::eClothOil, EFragmentShader::eModelNormalEZ, EGeometryShader::eInvalid, EInputLayout::eCloth, EUniformLayout::eModel, ETopologyType::eTriangleList, ERenderPass::eNormal, false),

	ECombinedShaderData(EVertexShader::eModelShadowNormal, EFragmentShader::eShadowAlpha, EGeometryShader::eInvalid, EInputLayout::eModel, EUniformLayout::eShadow, ETopologyType::eTriangleList, ERenderPass::eShadow, false),
	ECombinedShaderData(EVertexShader::eModelShadowChrome01, EFragmentShader::eShadowAlpha, EGeometryShader::eInvalid, EInputLayout::eModel, EUniformLayout::eShadow, ETopologyType::eTriangleList, ERenderPass::eShadow, false),
	ECombinedShaderData(EVertexShader::eModelShadowChrome02, EFragmentShader::eShadowAlpha, EGeometryShader::eInvalid, EInputLayout::eModel, EUniformLayout::eShadow, ETopologyType::eTriangleList, ERenderPass::eShadow, false),
	ECombinedShaderData(EVertexShader::eModelShadowChrome03, EFragmentShader::eShadowAlpha, EGeometryShader::eInvalid, EInputLayout::eModel, EUniformLayout::eShadow, ETopologyType::eTriangleList, ERenderPass::eShadow, false),
	ECombinedShaderData(EVertexShader::eModelShadowChrome04, EFragmentShader::eShadowAlpha, EGeometryShader::eInvalid, EInputLayout::eModel, EUniformLayout::eShadow, ETopologyType::eTriangleList, ERenderPass::eShadow, false),
	ECombinedShaderData(EVertexShader::eModelShadowChrome05, EFragmentShader::eShadowAlpha, EGeometryShader::eInvalid, EInputLayout::eModel, EUniformLayout::eShadow, ETopologyType::eTriangleList, ERenderPass::eShadow, false),
	ECombinedShaderData(EVertexShader::eModelShadowChrome06, EFragmentShader::eShadowAlpha, EGeometryShader::eInvalid, EInputLayout::eModel, EUniformLayout::eShadow, ETopologyType::eTriangleList, ERenderPass::eShadow, false),
	ECombinedShaderData(EVertexShader::eModelShadowChrome07, EFragmentShader::eShadowAlpha, EGeometryShader::eInvalid, EInputLayout::eModel, EUniformLayout::eShadow, ETopologyType::eTriangleList, ERenderPass::eShadow, false),
	ECombinedShaderData(EVertexShader::eModelShadowMetal, EFragmentShader::eShadowAlpha, EGeometryShader::eInvalid, EInputLayout::eModel, EUniformLayout::eShadow, ETopologyType::eTriangleList, ERenderPass::eShadow, false),
	ECombinedShaderData(EVertexShader::eModelShadowOil, EFragmentShader::eShadowAlpha, EGeometryShader::eInvalid, EInputLayout::eModel, EUniformLayout::eShadow, ETopologyType::eTriangleList, ERenderPass::eShadow, false),

	ECombinedShaderData(EVertexShader::eModelShadowNormal, EFragmentShader::eShadowBlend, EGeometryShader::eInvalid, EInputLayout::eModel, EUniformLayout::eShadow, ETopologyType::eTriangleList, ERenderPass::eShadow, false),
	ECombinedShaderData(EVertexShader::eModelShadowChrome01, EFragmentShader::eShadowBlend, EGeometryShader::eInvalid, EInputLayout::eModel, EUniformLayout::eShadow, ETopologyType::eTriangleList, ERenderPass::eShadow, false),
	ECombinedShaderData(EVertexShader::eModelShadowChrome02, EFragmentShader::eShadowBlend, EGeometryShader::eInvalid, EInputLayout::eModel, EUniformLayout::eShadow, ETopologyType::eTriangleList, ERenderPass::eShadow, false),
	ECombinedShaderData(EVertexShader::eModelShadowChrome03, EFragmentShader::eShadowBlend, EGeometryShader::eInvalid, EInputLayout::eModel, EUniformLayout::eShadow, ETopologyType::eTriangleList, ERenderPass::eShadow, false),
	ECombinedShaderData(EVertexShader::eModelShadowChrome04, EFragmentShader::eShadowBlend, EGeometryShader::eInvalid, EInputLayout::eModel, EUniformLayout::eShadow, ETopologyType::eTriangleList, ERenderPass::eShadow, false),
	ECombinedShaderData(EVertexShader::eModelShadowChrome05, EFragmentShader::eShadowBlend, EGeometryShader::eInvalid, EInputLayout::eModel, EUniformLayout::eShadow, ETopologyType::eTriangleList, ERenderPass::eShadow, false),
	ECombinedShaderData(EVertexShader::eModelShadowChrome06, EFragmentShader::eShadowBlend, EGeometryShader::eInvalid, EInputLayout::eModel, EUniformLayout::eShadow, ETopologyType::eTriangleList, ERenderPass::eShadow, false),
	ECombinedShaderData(EVertexShader::eModelShadowChrome07, EFragmentShader::eShadowBlend, EGeometryShader::eInvalid, EInputLayout::eModel, EUniformLayout::eShadow, ETopologyType::eTriangleList, ERenderPass::eShadow, false),
	ECombinedShaderData(EVertexShader::eModelShadowMetal, EFragmentShader::eShadowBlend, EGeometryShader::eInvalid, EInputLayout::eModel, EUniformLayout::eShadow, ETopologyType::eTriangleList, ERenderPass::eShadow, false),
	ECombinedShaderData(EVertexShader::eModelShadowOil, EFragmentShader::eShadowBlend, EGeometryShader::eInvalid, EInputLayout::eModel, EUniformLayout::eShadow, ETopologyType::eTriangleList, ERenderPass::eShadow, false),

	ECombinedShaderData(EVertexShader::eClothShadowNormal, EFragmentShader::eShadowAlpha, EGeometryShader::eInvalid, EInputLayout::eCloth, EUniformLayout::eShadow, ETopologyType::eTriangleList, ERenderPass::eShadow, false),
	ECombinedShaderData(EVertexShader::eClothShadowNormal, EFragmentShader::eShadowBlend, EGeometryShader::eInvalid, EInputLayout::eCloth, EUniformLayout::eShadow, ETopologyType::eTriangleList, ERenderPass::eShadow, false),

	ECombinedShaderData(EVertexShader::eEffect, EFragmentShader::eEffect, EGeometryShader::eInvalid, EInputLayout::eEffect, EUniformLayout::eEffect, ETopologyType::eTriangleList, ERenderPass::eNormal, false),
	ECombinedShaderData(EVertexShader::eEffect, EFragmentShader::eEffectEZ, EGeometryShader::eInvalid, EInputLayout::eEffect, EUniformLayout::eEffect, ETopologyType::eTriangleList, ERenderPass::eNormal, false),
};

EShaderData VertexShaderData[EVertexShaderMax] =
{
	EShaderData(_T("interface/uidefault.vs"), true, EInputLayout::eUIDefault),
	EShaderData(_T("interface/uicolor.vs"), true, EInputLayout::eUIDefault),

	EShaderData(_T("fonts/normalfont.vs"), true, EInputLayout::eFont),

	EShaderData(_T("terrains/terrain.vs"), false, EInputLayout::eTerrainCommon),
	EShaderData(_T("terrains/terrainatlans.vs"), false, EInputLayout::eTerrainCommon),
	EShaderData(_T("terrains/terrainheaven.vs"), false, EInputLayout::eTerrainCommon),
	EShaderData(_T("terrains/terraincastle.vs"), false, EInputLayout::eTerrainCommon),

	EShaderData(_T("terrains/terraingrass.vs"), false, EInputLayout::eGrass),
	EShaderData(_T("terrains/kalimawater.vs"), false, EInputLayout::eWater),
	EShaderData(_T("terrains/kalimawater2.vs"), false, EInputLayout::eWater),

	EShaderData(_T("models/texture.vs"), false, EInputLayout::eModel),
	EShaderData(_T("models/color.vs"), false, EInputLayout::eModel),
	EShaderData(_T("models/chrome01.vs"), false, EInputLayout::eModel),
	EShaderData(_T("models/chrome02.vs"), false, EInputLayout::eModel),
	EShaderData(_T("models/chrome03.vs"), false, EInputLayout::eModel),
	EShaderData(_T("models/chrome04.vs"), false, EInputLayout::eModel),
	EShaderData(_T("models/chrome05.vs"), false, EInputLayout::eModel),
	EShaderData(_T("models/chrome06.vs"), false, EInputLayout::eModel),
	EShaderData(_T("models/chrome07.vs"), false, EInputLayout::eModel),
	EShaderData(_T("models/metal.vs"), false, EInputLayout::eModel),
	EShaderData(_T("models/oil.vs"), false, EInputLayout::eModel),

	EShaderData(_T("cloth/texture.vs"), false, EInputLayout::eCloth),
	EShaderData(_T("cloth/color.vs"), false, EInputLayout::eCloth),
	EShaderData(_T("cloth/chrome01.vs"), false, EInputLayout::eCloth),
	EShaderData(_T("cloth/chrome02.vs"), false, EInputLayout::eCloth),
	EShaderData(_T("cloth/chrome03.vs"), false, EInputLayout::eCloth),
	EShaderData(_T("cloth/chrome04.vs"), false, EInputLayout::eCloth),
	EShaderData(_T("cloth/chrome05.vs"), false, EInputLayout::eCloth),
	EShaderData(_T("cloth/chrome06.vs"), false, EInputLayout::eCloth),
	EShaderData(_T("cloth/chrome07.vs"), false, EInputLayout::eCloth),
	EShaderData(_T("cloth/metal.vs"), false, EInputLayout::eCloth),
	EShaderData(_T("cloth/oil.vs"), false, EInputLayout::eCloth),

	EShaderData(_T("shadows/shadow.vs"), false, EInputLayout::eModel),
	EShaderData(_T("shadows/shadowchrome01.vs"), false, EInputLayout::eModel),
	EShaderData(_T("shadows/shadowchrome02.vs"), false, EInputLayout::eModel),
	EShaderData(_T("shadows/shadowchrome03.vs"), false, EInputLayout::eModel),
	EShaderData(_T("shadows/shadowchrome04.vs"), false, EInputLayout::eModel),
	EShaderData(_T("shadows/shadowchrome05.vs"), false, EInputLayout::eModel),
	EShaderData(_T("shadows/shadowchrome06.vs"), false, EInputLayout::eModel),
	EShaderData(_T("shadows/shadowchrome07.vs"), false, EInputLayout::eModel),
	EShaderData(_T("shadows/shadowmetal.vs"), false, EInputLayout::eModel),
	EShaderData(_T("shadows/shadowoil.vs"), false, EInputLayout::eModel),

	EShaderData(_T("shadowscloth/shadow.vs"), false, EInputLayout::eCloth),

	EShaderData(_T("effects/effects.vs"), false, EInputLayout::eEffect),
};

EShaderData FragmentShaderData[EFragmentShaderMax] =
{
	EShaderData(_T("interface/uidefault.ps"), true),
	EShaderData(_T("interface/uicolor.ps"), true),
	EShaderData(_T("interface/uilimitedalpha.ps"), true),

	EShaderData(_T("fonts/normalfont.ps"), true),

	EShaderData(_T("terrains/terrain.ps"), false),

	EShaderData(_T("terrains/terraingrass.ps"), false),
	EShaderData(_T("terrains/kalimawater.ps"), false),
	EShaderData(_T("terrains/kalimawater2.ps"), false),

	EShaderData(_T("models/texture.ps"), false),
	EShaderData(_T("models/color.ps"), false),
	EShaderData(_T("models/texture_ez.ps"), false),
	EShaderData(_T("models/color_ez.ps"), false),

	EShaderData(_T("shadows/shadow.ps"), false),
	EShaderData(_T("shadows/shadow2.ps"), false),

	EShaderData(_T("effects/effects.ps"), false),
	EShaderData(_T("effects/effects_ez.ps"), false),
};

EShaderData GeometryShaderData[EGeometryShaderMax] =
{
	EShaderData(_T("dummy.gs"), false),
};