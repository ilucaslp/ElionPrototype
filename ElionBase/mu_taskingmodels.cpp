#include "stdafx.h"
#include "mu_tasking.h"

#define TEMPLATE_INIT_HELPER(funcname, ...) \
template void MUTasking::funcname <false, EVideoBackendType::eDirectX>(__VA_ARGS__); \
template void MUTasking::funcname <false, EVideoBackendType::eOpenGL>(__VA_ARGS__); \
template void MUTasking::funcname <false, EVideoBackendType::eVulkan>(__VA_ARGS__); \
template void MUTasking::funcname <false, EVideoBackendType::eMetal>(__VA_ARGS__); \
template void MUTasking::funcname <true, EVideoBackendType::eDirectX>(__VA_ARGS__); \
template void MUTasking::funcname <true, EVideoBackendType::eOpenGL>(__VA_ARGS__); \
template void MUTasking::funcname <true, EVideoBackendType::eVulkan>(__VA_ARGS__); \
template void MUTasking::funcname <true, EVideoBackendType::eMetal>(__VA_ARGS__);

TEMPLATE_INIT_HELPER(RenderPreModels, const mu_boolean shadowPass);
TEMPLATE_INIT_HELPER(RenderPreAlphaModels, const mu_boolean shadowPass);
TEMPLATE_INIT_HELPER(RenderPostModels, const mu_boolean shadowPass);
TEMPLATE_INIT_HELPER(RenderGUIModels, const mu_uint32 slotStart, const mu_uint32 slotCount);
TEMPLATE_INIT_HELPER(RenderShadowModels, void);

#undef TEMPLATE_INIT_HELPER

void MUTasking::PreConfigureRenderSlots(const mu_uint32 ThreadIndex)
{
	mu_uint32 slotsCount;

	if(ThreadIndex == 0) // This prevent rendering order flickering, depth models shouldn't be listed by multiple threads
	{
		slotsCount = g_PreRenderSlotCount.load(std::memory_order_relaxed);
		for (mu_uint32 i = 0; i < slotsCount; ++i)
		{
			MURenderSlot &slot = g_PreRenderSlot[g_PreRenderSlotUsed[i]];
			mu_uint32 proxyIndex = FindRenderProxy(slot.RenderIndex, slot.ModelIndex, slot.RenderType, slot.BlendType, slot.ExtraFlag, slot.Texture, slot.MeshIndex);

			if (proxyIndex != WAITING_PROXY)
			{
				MURenderProxy &proxy = g_RenderProxy[proxyIndex];

				slot.ProxyIndex = proxyIndex;
				slot.InstanceIndex = proxy.InstanceCount.fetch_add(1);

				proxy.Enabled = true;

				if (slot.Flags.Get(MURenderSlot::eRSF_SHADOW) == true)
				{
					proxy.ShadowEnabled = true;
				}
			}
		}
	}

	slotsCount = g_PostRenderSlotCount.load(std::memory_order_relaxed);
	for (mu_uint32 i = ThreadIndex; i < slotsCount; i += _ThreadsCount)
	{
		MURenderSlot &slot = g_PostRenderSlot[i];
		mu_uint32 proxyIndex = FindRenderProxy(0, slot.ModelIndex, slot.RenderType, slot.BlendType, slot.ExtraFlag, slot.Texture, slot.MeshIndex);

		if (proxyIndex != WAITING_PROXY)
		{
			MURenderProxy &proxy = g_RenderProxy[proxyIndex];

			slot.ProxyIndex = proxyIndex;
			slot.InstanceIndex = proxy.InstanceCount.fetch_add(1);

			proxy.Enabled = true;

			if (slot.Flags.Get(MURenderSlot::eRSF_SHADOW) == true)
			{
				proxy.ShadowEnabled = true;
			}
		}
	}
}

void MUTasking::PostConfigureRenderSlots(const mu_uint32 ThreadIndex)
{
	const mu_uint32 preSlotsCount = g_PreRenderSlotCount.load(std::memory_order_relaxed);
	for (mu_uint32 i = ThreadIndex; i < preSlotsCount; i += _ThreadsCount)
	{
		MURenderSlot &slot = g_PreRenderSlot[i];
		MURenderProxy &proxy = g_RenderProxy[slot.ProxyIndex];
		SHModelInstance *instance = GetModelInstanceBuffer(proxy.InstanceBase + slot.InstanceIndex);

		mu_float boneScale = (slot.BoneScale - 1.0f);

		if (slot.RenderType == RT_COLOR)
		{
			if (boneScale < 0.0f) boneScale -= 1.0f;
			else if (boneScale > 0.0f) boneScale += 1.0f;
		}

		const mu_float isTranslate = static_cast<mu_float>(slot.Flags.Get(MURenderSlot::eRSF_TRANSLATE));

		SetModelInstance(instance,
			slot.BodyOrigin * isTranslate,
			EVector2((1.0f - isTranslate) + slot.BodyScale * isTranslate, boneScale),
			EVector2(slot.Flags.Get(MURenderSlot::eRSF_LIGHT), slot.MinAlpha),
			EVector4(slot.BlendMeshTexCoordU, slot.BlendMeshTexCoordV, slot.Flags.Get(MURenderSlot::eRSF_SHADOW), slot.ShadowHeight),
			slot.BodyLight,
			slot.ShadowLight,
			slot.SkeletalIndex);
	}

	const mu_uint32 postSlotsCount = g_PostRenderSlotCount.load(std::memory_order_relaxed);
	for (mu_uint32 i = ThreadIndex; i < postSlotsCount; i += _ThreadsCount)
	{
		MURenderSlot &slot = g_PostRenderSlot[i];
		MURenderProxy &proxy = g_RenderProxy[slot.ProxyIndex];
		SHModelInstance *instance = GetModelInstanceBuffer(proxy.InstanceBase + slot.InstanceIndex);

		mu_float boneScale = (slot.BoneScale - 1.0f);

		if (slot.RenderType == RT_COLOR)
		{
			if (boneScale < 0.0f) boneScale -= 1.0f;
			else if (boneScale > 0.0f) boneScale += 1.0f;
		}

		const mu_float isTranslate = static_cast<mu_float>(slot.Flags.Get(MURenderSlot::eRSF_TRANSLATE));

		SetModelInstance(instance,
			slot.BodyOrigin * isTranslate,
			EVector2((1.0f - isTranslate) + slot.BodyScale * isTranslate, boneScale),
			EVector2(slot.Flags.Get(MURenderSlot::eRSF_LIGHT), slot.MinAlpha),
			EVector4(slot.BlendMeshTexCoordU, slot.BlendMeshTexCoordV, slot.Flags.Get(MURenderSlot::eRSF_SHADOW), slot.ShadowHeight),
			slot.BodyLight,
			slot.ShadowLight,
			slot.SkeletalIndex);
	}
}

void MUTasking::ConfigureRenderProxy(const mu_uint32 ThreadIndex)
{
	// This is executed on thread 0, this prevent rendering order flickering, depth models shouldn't be listed by multiple threads
	for (mu_uint32 i = 0; i < g_RenderProxyCount; ++i)
	{
		MURenderProxy &proxy = g_RenderProxy[i];

		if (proxy.Enabled == true)
		{
			proxy.InstanceBase = _ModelInstanceCount.fetch_add(proxy.InstanceCount.load(std::memory_order_relaxed));
		}
	}
}

template<const mu_boolean useBaseInstance, const EVideoBackendType backendType>
void MUTasking::RenderPreModels(const mu_boolean shadowPass)
{
#ifdef USE_RENDERING_DISABLE_FLAGS
	if (g_DisablePreModelRender == true)
		return;
#endif

	EVideoPipeline::BindGraphicsBonesTexture<backendType>();

	if (g_PreRenderSlotCount.load(std::memory_order_relaxed) > 0)
	{
		EVideoPipeline::BindGraphicsModelInstancesBuffer<backendType>();

		mu_uint32 lastModel = -1, lastTexture = -1;
		mu_uint32 instanceOffset = -1;

		ECombinedShader programShader;
		EBlendState blendState;
		EDepthStencil depthStencil;
		ERasterizer rasterizer;

		for (mu_uint32 index = 0; index < g_RenderProxyCount; ++index)
		{
			MURenderProxy &proxy = g_RenderProxy[g_RenderProxyIndex[index]];

			if (proxy.Enabled.load(std::memory_order_relaxed) == false ||
				(shadowPass == true &&
					proxy.ShadowEnabled.load(std::memory_order_relaxed) == false))
			{
				continue;
			}

			if (proxy.BlendType < BLEND::TXB_DEPTH_BEGIN ||
				proxy.BlendType >= BLEND::TXB_DEPTH_END ||
				proxy.BlendType == BLEND::TXB_ALPHATEST_NONDEPTH)
			{
				continue;
			}

			MUModel &model = MODELS::Data[proxy.ModelIndex];
			if (model.IsGPUAvailable() == false)
			{
				if (lastModel != proxy.ModelIndex)
				{
					lastModel = proxy.ModelIndex;
					model.RequestLoad();
				}
				continue;
			}

			MUTexture &texture = TEXTURES::Data[proxy.TextureIndex];
			if (texture.IsLoaded() == false)
			{
				if (proxy.TextureIndex >= BITMAP_BEGIN_RUNTIME &&
					lastTexture != proxy.TextureIndex)
				{
					lastTexture = proxy.TextureIndex;
					if (texture.RequestLoad() == false)
					{
						g_LoadingManager.AddRuntime(TEXTURES::LoadData[proxy.TextureIndex]);
					}
				}
				continue;
			}

			if (proxy.BlendType >= BLEND::TXB_ALPHATEST_BEGIN &&
				proxy.BlendType <= BLEND::TXB_ALPHATEST_END)
			{
				blendState = EBlendState::eAlphaSrc_InvAlphaSrc;
			}
			else
			{
				switch (proxy.BlendType)
				{
				case BLEND::TXB_ALPHATEST_NOMASK:
				case BLEND::TXB_ALPHATEST_NONDEPTH:
				{
					blendState = EBlendState::eAlphaSrc_InvAlphaSrc;
				}
				break;
				}
			}

			if ((proxy.ExtraFlag & REXF_CULL_FRONT) == REXF_CULL_FRONT)
			{
				rasterizer = ERasterizer::eFrontOnly;
			}
			else if ((proxy.ExtraFlag & REXF_CULL_BACK) == REXF_CULL_BACK)
			{
				rasterizer = ERasterizer::eBackOnly;
			}
			else
			{
				rasterizer = ERasterizer::eNoCull;
			}

			if ((proxy.ExtraFlag & REXF_DOPPELGANGER) == REXF_DOPPELGANGER)
			{
				depthStencil = EDepthStencil::eZBuffer;
			}
			else if ((proxy.ExtraFlag & REXF_ENABLED_DEPTH) == REXF_ENABLED_DEPTH)
			{
				depthStencil = EDepthStencil::eZBuffer;
			}
			else
			{
				depthStencil = EDepthStencil::eNoZBuffer;
			}

			if (shadowPass == false)
			{
				switch (proxy.RenderType)
				{
				case RT_TEXTURE:
					{
						programShader = depthStencil >= EDepthStencil::eEarlyZBegin ? ECombinedShader::eModelNormalEZ : ECombinedShader::eModelNormal;
					}
					break;

				case RT_COLOR:
					{
						programShader = depthStencil >= EDepthStencil::eEarlyZBegin ? ECombinedShader::eModelColorEZ : ECombinedShader::eModelColor;
					}
					break;

				case RT_CHROME:
					{
						programShader = depthStencil >= EDepthStencil::eEarlyZBegin ? ECombinedShader::eModelChrome01EZ : ECombinedShader::eModelChrome01;
					}
					break;

				case RT_CHROME2:
					{
						programShader = depthStencil >= EDepthStencil::eEarlyZBegin ? ECombinedShader::eModelChrome02EZ : ECombinedShader::eModelChrome02;
					}
					break;

				case RT_CHROME3:
					{
						programShader = depthStencil >= EDepthStencil::eEarlyZBegin ? ECombinedShader::eModelChrome03EZ : ECombinedShader::eModelChrome03;
					}
					break;

				case RT_CHROME4:
					{
						programShader = depthStencil >= EDepthStencil::eEarlyZBegin ? ECombinedShader::eModelChrome04EZ : ECombinedShader::eModelChrome04;
					}
					break;

				case RT_CHROME5:
					{
						programShader = depthStencil >= EDepthStencil::eEarlyZBegin ? ECombinedShader::eModelChrome05EZ : ECombinedShader::eModelChrome05;
					}
					break;

				case RT_CHROME6:
					{
						programShader = depthStencil >= EDepthStencil::eEarlyZBegin ? ECombinedShader::eModelChrome06EZ : ECombinedShader::eModelChrome06;
					}
					break;

				case RT_CHROME7:
					{
						programShader = depthStencil >= EDepthStencil::eEarlyZBegin ? ECombinedShader::eModelChrome07EZ : ECombinedShader::eModelChrome07;
					}
					break;

				case RT_METAL:
					{
						programShader = depthStencil >= EDepthStencil::eEarlyZBegin ? ECombinedShader::eModelMetalEZ : ECombinedShader::eModelMetal;
					}
					break;

				case RT_OIL:
					{
						programShader = depthStencil >= EDepthStencil::eEarlyZBegin ? ECombinedShader::eModelOilEZ : ECombinedShader::eModelOil;
					}
					break;

				default:
					{
						programShader = depthStencil >= EDepthStencil::eEarlyZBegin ? ECombinedShader::eModelNormalEZ : ECombinedShader::eModelNormal;
					}
					break;
				}
			}
			else
			{
				switch (proxy.RenderType)
				{
				case RT_TEXTURE:
					{
						programShader = ECombinedShader::eModelShadowNormalAlpha;
					}
					break;

				case RT_COLOR:
					continue;

				case RT_CHROME:
					{
						programShader = ECombinedShader::eModelShadowChrome01Alpha;
					}
					break;

				case RT_CHROME2:
					{
						programShader = ECombinedShader::eModelShadowChrome02Alpha;
					}
					break;

				case RT_CHROME3:
					{
						programShader = ECombinedShader::eModelShadowChrome03Alpha;
					}
					break;

				case RT_CHROME4:
					{
						programShader = ECombinedShader::eModelShadowChrome04Alpha;
					}
					break;

				case RT_CHROME5:
					{
						programShader = ECombinedShader::eModelShadowChrome05Alpha;
					}
					break;

				case RT_CHROME6:
					{
						programShader = ECombinedShader::eModelShadowChrome06Alpha;
					}
					break;

				case RT_CHROME7:
					{
						programShader = ECombinedShader::eModelShadowChrome07Alpha;
					}
					break;

				case RT_METAL:
					{
						programShader = ECombinedShader::eModelShadowMetalAlpha;
					}
					break;

				case RT_OIL:
					{
						programShader = ECombinedShader::eModelShadowOilAlpha;
					}
					break;

				default:
					{
						programShader = ECombinedShader::eModelShadowNormalAlpha;
					}
					break;
				}
			}

			EVideoPipeline::SetGraphicsPipeline<backendType>(programShader, rasterizer, depthStencil, blendState, false);

			if (proxy.ModelIndex != lastModel)
			{
				lastModel = proxy.ModelIndex;

				EVideoPipeline::BindGraphicsModelBuffers<backendType>(&model);
				EVideoAttributes::ConfigureAtRuntime<backendType>(EInputLayout::eModel);
			}

			if (proxy.TextureIndex != lastTexture)
			{
				lastTexture = proxy.TextureIndex;

				EVideoPipeline::BindGraphicsModelTexture<backendType>(&texture);
			}

			Model::MeshData1 &mesh = model._MeshData[proxy.MeshIndex];

			if (model._VertexHeader.IndexType == MUModel::INDEX_16BIT)
			{
				EVideoPipeline::DrawIndexedInstanced<useBaseInstance, backendType>(mesh.IndexStart, mesh.IndexCount, proxy.InstanceBase, proxy.InstanceCount.load(std::memory_order_relaxed), ETopologyType::eTriangleList, EIndexType::eIndex16, EInputLayout::eModel);
			}
			else
			{
				EVideoPipeline::DrawIndexedInstanced<useBaseInstance, backendType>(mesh.IndexStart, mesh.IndexCount, proxy.InstanceBase, proxy.InstanceCount.load(std::memory_order_relaxed), ETopologyType::eTriangleList, EIndexType::eIndex32, EInputLayout::eModel);
			}
		}
	}

	const mu_uint32 clothCount = g_ClothRenderSlotCount.load(std::memory_order_relaxed);
	if (clothCount > 0)
	{
		EVideoPipeline::BindGraphicsClothInstancesBuffer<backendType>();

		mu_uint32 lastTexture = -1;

		ECombinedShader programShader;
		EBlendState blendState;
		EDepthStencil depthStencil;
		ERasterizer rasterizer;

		for (mu_uint32 index = 0; index < clothCount; ++index)
		{
			MUClothRenderSlot &slot = g_ClothRenderSlot[g_ClothRenderSlotUsed[index]];

			if (slot.BlendType < BLEND::TXB_DEPTH_BEGIN ||
				slot.BlendType >= BLEND::TXB_DEPTH_END ||
				slot.BlendType == BLEND::TXB_ALPHATEST_NONDEPTH ||
				(shadowPass == true &&
					slot.EnableShadow == false))
			{
				continue;
			}

			MUTexture &texture = TEXTURES::Data[slot.Texture];
			if (texture.IsLoaded() == false)
			{
				if (slot.Texture >= BITMAP_BEGIN_RUNTIME &&
					lastTexture != slot.Texture)
				{
					lastTexture = slot.Texture;
					if (texture.RequestLoad() == false)
					{
						g_LoadingManager.AddRuntime(TEXTURES::LoadData[slot.Texture]);
					}
				}
				continue;
			}

			if (slot.BlendType >= BLEND::TXB_ALPHATEST_BEGIN &&
				slot.BlendType <= BLEND::TXB_ALPHATEST_END)
			{
				blendState = EBlendState::eAlphaSrc_InvAlphaSrc;
			}
			else
			{
				switch (slot.BlendType)
				{
				case BLEND::TXB_ALPHATEST_NOMASK:
				case BLEND::TXB_ALPHATEST_NONDEPTH:
				{
					blendState = EBlendState::eAlphaSrc_InvAlphaSrc;
				}
				break;
				}
			}

			if ((slot.ExtraFlag & REXF_CULL_FRONT) == REXF_CULL_FRONT)
			{
				rasterizer = ERasterizer::eFrontOnly;
			}
			else if ((slot.ExtraFlag & REXF_CULL_BACK) == REXF_CULL_BACK)
			{
				rasterizer = ERasterizer::eBackOnly;
			}
			else
			{
				rasterizer = ERasterizer::eNoCull;
			}

#if 1
			depthStencil = EDepthStencil::eNoZBuffer;
#else
			if ((slot.ExtraFlag & REXF_DOPPELGANGER) == REXF_DOPPELGANGER)
			{
				depthStencil = EDepthStencil::eZBuffer;
			}
			else if ((slot.ExtraFlag & REXF_ENABLED_DEPTH) == REXF_ENABLED_DEPTH)
			{
				depthStencil = EDepthStencil::eZBuffer;
			}
			else
			{
				depthStencil = EDepthStencil::eNoZBuffer;
			}
#endif

			if (shadowPass == false)
			{
				switch (slot.RenderType)
				{
				case RT_TEXTURE:
					{
						programShader = depthStencil >= EDepthStencil::eEarlyZBegin ? ECombinedShader::eClothNormalEZ : ECombinedShader::eClothNormal;
					}
					break;

				case RT_COLOR:
					{
						programShader = depthStencil >= EDepthStencil::eEarlyZBegin ? ECombinedShader::eClothColorEZ : ECombinedShader::eClothColor;
					}
					break;

				case RT_CHROME:
					{
						programShader = depthStencil >= EDepthStencil::eEarlyZBegin ? ECombinedShader::eClothChrome01EZ : ECombinedShader::eClothChrome01;
					}
					break;

				case RT_CHROME2:
					{
						programShader = depthStencil >= EDepthStencil::eEarlyZBegin ? ECombinedShader::eClothChrome02EZ : ECombinedShader::eClothChrome02;
					}
					break;

				case RT_CHROME3:
					{
						programShader = depthStencil >= EDepthStencil::eEarlyZBegin ? ECombinedShader::eClothChrome03EZ : ECombinedShader::eClothChrome03;
					}
					break;

				case RT_CHROME4:
					{
						programShader = depthStencil >= EDepthStencil::eEarlyZBegin ? ECombinedShader::eClothChrome04EZ : ECombinedShader::eClothChrome04;
					}
					break;

				case RT_CHROME5:
					{
						programShader = depthStencil >= EDepthStencil::eEarlyZBegin ? ECombinedShader::eClothChrome05EZ : ECombinedShader::eClothChrome05;
					}
					break;

				case RT_CHROME6:
					{
						programShader = depthStencil >= EDepthStencil::eEarlyZBegin ? ECombinedShader::eClothChrome06EZ : ECombinedShader::eClothChrome06;
					}
					break;

				case RT_CHROME7:
					{
						programShader = depthStencil >= EDepthStencil::eEarlyZBegin ? ECombinedShader::eClothChrome07EZ : ECombinedShader::eClothChrome07;
					}
					break;

				case RT_METAL:
					{
						programShader = depthStencil >= EDepthStencil::eEarlyZBegin ? ECombinedShader::eClothMetalEZ : ECombinedShader::eClothMetal;
					}
					break;

				case RT_OIL:
					{
						programShader = depthStencil >= EDepthStencil::eEarlyZBegin ? ECombinedShader::eClothOilEZ : ECombinedShader::eClothOil;
					}
					break;

				default:
					{
						programShader = depthStencil >= EDepthStencil::eEarlyZBegin ? ECombinedShader::eClothNormalEZ : ECombinedShader::eClothNormal;
					}
					break;
				}
			}
			else
			{
				switch (slot.RenderType)
				{
				case RT_TEXTURE:
					{
						programShader = ECombinedShader::eClothShadowNormalAlpha;
					}
					break;

				default:
					continue;
				}
			}

			EVideoPipeline::SetGraphicsPipeline<backendType>(programShader, rasterizer, depthStencil, blendState, false);

			if (slot.Texture != lastTexture)
			{
				lastTexture = slot.Texture;

				EVideoPipeline::BindGraphicsModelTexture<backendType>(&texture);
			}

			EVideoPipeline::DrawInstanced<useBaseInstance, backendType>(slot.BaseIndex, slot.IndexCount, slot.InstanceIndex, 1, ETopologyType::eTriangleList, EInputLayout::eCloth);
		}
	}

	EVideoPipeline::UnbindVAO();
}

template<const mu_boolean useBaseInstance, const EVideoBackendType backendType>
void MUTasking::RenderPreAlphaModels(const mu_boolean shadowPass)
{
#ifdef USE_RENDERING_DISABLE_FLAGS
	if (g_DisablePreAlphaModelRender == true)
		return;
#endif

	EVideoPipeline::BindGraphicsBonesTexture<backendType>();

	if (g_PostRenderSlotCount.load(std::memory_order_relaxed) > 0)
	{
		EVideoPipeline::BindGraphicsModelInstancesBuffer<backendType>();

		mu_uint32 lastModel = -1, lastTexture = -1;
		mu_uint32 instanceOffset = -1;

		ECombinedShader programShader;
		EBlendState blendState = EBlendState::eAlphaSrc_InvAlphaSrc;
		EDepthStencil depthStencil;
		ERasterizer rasterizer;

		for (mu_uint32 index = 0; index < g_RenderProxyCount; ++index)
		{
			MURenderProxy &proxy = g_RenderProxy[g_RenderProxyIndex[index]];

			if (proxy.Enabled.load(std::memory_order_relaxed) == false ||
				(shadowPass == true &&
					proxy.ShadowEnabled.load(std::memory_order_relaxed) == false))
			{
				continue;
			}

			if (proxy.BlendType != BLEND::TXB_ALPHATEST_NONDEPTH)
			{
				continue;
			}

			MUModel &model = MODELS::Data[proxy.ModelIndex];
			if (model.IsGPUAvailable() == false)
			{
				if (lastModel != proxy.ModelIndex)
				{
					lastModel = proxy.ModelIndex;
					model.RequestLoad();
				}
				continue;
			}

			MUTexture &texture = TEXTURES::Data[proxy.TextureIndex];
			if (texture.IsLoaded() == false)
			{
				if (proxy.TextureIndex >= BITMAP_BEGIN_RUNTIME &&
					lastTexture != proxy.TextureIndex)
				{
					lastTexture = proxy.TextureIndex;
					if (texture.RequestLoad() == false)
					{
						g_LoadingManager.AddRuntime(TEXTURES::LoadData[proxy.TextureIndex]);
					}
				}
				continue;
			}

			if ((proxy.ExtraFlag & REXF_CULL_FRONT) == REXF_CULL_FRONT)
			{
				rasterizer = ERasterizer::eFrontOnly;
			}
			else if ((proxy.ExtraFlag & REXF_CULL_BACK) == REXF_CULL_BACK)
			{
				rasterizer = ERasterizer::eBackOnly;
			}
			else
			{
				rasterizer = ERasterizer::eNoCull;
			}

			if ((proxy.ExtraFlag & REXF_DOPPELGANGER) == REXF_DOPPELGANGER)
			{
				depthStencil = EDepthStencil::eZBuffer;
			}
			else if ((proxy.ExtraFlag & REXF_ENABLED_DEPTH) == REXF_ENABLED_DEPTH)
			{
				depthStencil = EDepthStencil::eZBuffer;
			}
			else
			{
				depthStencil = EDepthStencil::eNoZBuffer;
			}

			if (shadowPass == false)
			{
				switch (proxy.RenderType)
				{
				case RT_TEXTURE:
					{
						programShader = depthStencil >= EDepthStencil::eEarlyZBegin ? ECombinedShader::eModelNormalEZ : ECombinedShader::eModelNormal;
					}
					break;

				case RT_COLOR:
					{
						programShader = depthStencil >= EDepthStencil::eEarlyZBegin ? ECombinedShader::eModelColorEZ : ECombinedShader::eModelColor;
					}
					break;

				case RT_CHROME:
					{
						programShader = depthStencil >= EDepthStencil::eEarlyZBegin ? ECombinedShader::eModelChrome01EZ : ECombinedShader::eModelChrome01;
					}
					break;

				case RT_CHROME2:
					{
						programShader = depthStencil >= EDepthStencil::eEarlyZBegin ? ECombinedShader::eModelChrome02EZ : ECombinedShader::eModelChrome02;
					}
					break;

				case RT_CHROME3:
					{
						programShader = depthStencil >= EDepthStencil::eEarlyZBegin ? ECombinedShader::eModelChrome03EZ : ECombinedShader::eModelChrome03;
					}
					break;

				case RT_CHROME4:
					{
						programShader = depthStencil >= EDepthStencil::eEarlyZBegin ? ECombinedShader::eModelChrome04EZ : ECombinedShader::eModelChrome04;
					}
					break;

				case RT_CHROME5:
					{
						programShader = depthStencil >= EDepthStencil::eEarlyZBegin ? ECombinedShader::eModelChrome05EZ : ECombinedShader::eModelChrome05;
					}
					break;

				case RT_CHROME6:
					{
						programShader = depthStencil >= EDepthStencil::eEarlyZBegin ? ECombinedShader::eModelChrome06EZ : ECombinedShader::eModelChrome06;
					}
					break;

				case RT_CHROME7:
					{
						programShader = depthStencil >= EDepthStencil::eEarlyZBegin ? ECombinedShader::eModelChrome07EZ : ECombinedShader::eModelChrome07;
					}
					break;

				case RT_METAL:
					{
						programShader = depthStencil >= EDepthStencil::eEarlyZBegin ? ECombinedShader::eModelMetalEZ : ECombinedShader::eModelMetal;
					}
					break;

				case RT_OIL:
					{
						programShader = depthStencil >= EDepthStencil::eEarlyZBegin ? ECombinedShader::eModelOilEZ : ECombinedShader::eModelOil;
					}
					break;

				default:
					{
						programShader = depthStencil >= EDepthStencil::eEarlyZBegin ? ECombinedShader::eModelNormalEZ : ECombinedShader::eModelNormal;
					}
					break;
				}
			}
			else
			{
				switch (proxy.RenderType)
				{
				case RT_TEXTURE:
					{
						programShader = ECombinedShader::eModelShadowNormalAlpha;
					}
					break;

				case RT_COLOR:
					continue;

				case RT_CHROME:
					{
						programShader = ECombinedShader::eModelShadowChrome01Alpha;
					}
					break;

				case RT_CHROME2:
					{
						programShader = ECombinedShader::eModelShadowChrome02Alpha;
					}
					break;

				case RT_CHROME3:
					{
						programShader = ECombinedShader::eModelShadowChrome03Alpha;
					}
					break;

				case RT_CHROME4:
					{
						programShader = ECombinedShader::eModelShadowChrome04Alpha;
					}
					break;

				case RT_CHROME5:
					{
						programShader = ECombinedShader::eModelShadowChrome05Alpha;
					}
					break;

				case RT_CHROME6:
					{
						programShader = ECombinedShader::eModelShadowChrome06Alpha;
					}
					break;

				case RT_CHROME7:
					{
						programShader = ECombinedShader::eModelShadowChrome07Alpha;
					}
					break;

				case RT_METAL:
					{
						programShader = ECombinedShader::eModelShadowMetalAlpha;
					}
					break;

				case RT_OIL:
					{
						programShader = ECombinedShader::eModelShadowOilAlpha;
					}
					break;

				default:
					{
						programShader = ECombinedShader::eModelShadowNormalAlpha;
					}
					break;
				}
			}

			if (proxy.ModelIndex != lastModel)
			{
				lastModel = proxy.ModelIndex;

				EVideoPipeline::BindGraphicsModelBuffers<backendType>(&model);
				EVideoAttributes::ConfigureAtRuntime<backendType>(EInputLayout::eModel);
			}

			if (proxy.TextureIndex != lastTexture)
			{
				lastTexture = proxy.TextureIndex;

				EVideoPipeline::BindGraphicsModelTexture<backendType>(&texture);
			}

			Model::MeshData1 &mesh = model._MeshData[proxy.MeshIndex];

			EVideoPipeline::SetGraphicsPipeline<backendType>(programShader, rasterizer, depthStencil, blendState, false);

			if (model._VertexHeader.IndexType == MUModel::INDEX_16BIT)
			{
				EVideoPipeline::DrawIndexedInstanced<useBaseInstance, backendType>(mesh.IndexStart, mesh.IndexCount, proxy.InstanceBase, proxy.InstanceCount.load(std::memory_order_relaxed), ETopologyType::eTriangleList, EIndexType::eIndex16, EInputLayout::eModel);
			}
			else
			{
				EVideoPipeline::DrawIndexedInstanced<useBaseInstance, backendType>(mesh.IndexStart, mesh.IndexCount, proxy.InstanceBase, proxy.InstanceCount.load(std::memory_order_relaxed), ETopologyType::eTriangleList, EIndexType::eIndex32, EInputLayout::eModel);
			}
		}
	}

	EVideoPipeline::UnbindVAO();
}

template<const mu_boolean useBaseInstance, const EVideoBackendType backendType>
void MUTasking::RenderPostModels(const mu_boolean shadowPass)
{
#ifdef USE_RENDERING_DISABLE_FLAGS
	if (g_DisablePostModelRender == true)
		return;
#endif

	EVideoPipeline::BindGraphicsBonesTexture<backendType>();

	if (g_PostRenderSlotCount.load(std::memory_order_relaxed) > 0)
	{
		EVideoPipeline::BindGraphicsModelInstancesBuffer<backendType>();

		mu_uint32 lastModel = -1, lastTexture = -1;
		mu_uint32 instanceOffset = -1;

		ECombinedShader programShader;
		EBlendState blendState;
		EDepthStencil depthStencil;
		ERasterizer rasterizer;

		for (mu_uint32 index = 0; index < g_RenderProxyCount; ++index)
		{
			MURenderProxy &proxy = g_RenderProxy[g_RenderProxyIndex[index]];

			if (proxy.Enabled.load(std::memory_order_relaxed) == false ||
				(shadowPass == true &&
					proxy.ShadowEnabled.load(std::memory_order_relaxed) == false))
			{
				continue;
			}

			if (proxy.BlendType < BLEND::TXB_NON_DEPTH_BEGIN ||
				proxy.BlendType >= BLEND::TXB_NON_DEPTH_END)
			{
				continue;
			}

			MUModel &model = MODELS::Data[proxy.ModelIndex];
			if (model.IsGPUAvailable() == false)
			{
				if (lastModel != proxy.ModelIndex)
				{
					lastModel = proxy.ModelIndex;
					model.RequestLoad();
				}
				continue;
			}

			MUTexture &texture = TEXTURES::Data[proxy.TextureIndex];
			if (texture.IsLoaded() == false)
			{
				if (proxy.TextureIndex >= BITMAP_BEGIN_RUNTIME &&
					lastTexture != proxy.TextureIndex)
				{
					lastTexture = proxy.TextureIndex;
					if (texture.RequestLoad() == false)
					{
						g_LoadingManager.AddRuntime(TEXTURES::LoadData[proxy.TextureIndex]);
					}
				}
				continue;
			}

			switch (proxy.BlendType)
			{
			case BLEND::TXB_BLEND01:
			case BLEND::TXB_PRE_BLEND01:
				{
					if (shadowPass == true)
					{
						blendState = EBlendState::eOne_One;
						//blendState = EBlendState::eAlphaSrc_InvAlphaSrc;
					}
					else
					{
						blendState = EBlendState::eOne_One;
					}
				}
				break;

			case BLEND::TXB_BLENDMINUS:
			case BLEND::TXB_PRE_BLENDMINUS:
				{
					blendState = EBlendState::eZero_InvColorSrc;
				}
				break;

			case BLEND::TXB_BLEND02:
			case BLEND::TXB_PRE_BLEND02:
				{
					blendState = EBlendState::eInvColorSrc_One;
				}
				break;

			case BLEND::TXB_BLEND03:
			case BLEND::TXB_PRE_BLEND03:
				{
					blendState = EBlendState::eAlphaSrc_InvAlphaSrc;
				}
				break;

			case BLEND::TXB_BLEND04:
			case BLEND::TXB_PRE_BLEND04:
				{
					blendState = EBlendState::eOne_InvColorSrc;
				}
				break;

			case BLEND::TXB_LIGHTMAP:
			case BLEND::TXB_PRE_LIGHTMAP:
				{
					blendState = EBlendState::eZero_ColorSrc;
				}
				break;
			}

			if ((proxy.ExtraFlag & REXF_CULL_FRONT) == REXF_CULL_FRONT)
			{
				rasterizer = ERasterizer::eFrontOnly;
			}
			else if ((proxy.ExtraFlag & REXF_CULL_BACK) == REXF_CULL_BACK)
			{
				rasterizer = ERasterizer::eBackOnly;
			}
			else
			{
				rasterizer = ERasterizer::eNoCull;
			}

			if (shadowPass == true)
			{
				depthStencil = EDepthStencil::eZBuffer;
			}
			else if ((proxy.ExtraFlag & REXF_DOPPELGANGER) == REXF_DOPPELGANGER)
			{
				depthStencil = EDepthStencil::eDepthMaskPlusEqual;
			}
			else if (proxy.BlendType >= BLEND::TXB_PRE_BLEND_BEGIN &&
					 proxy.BlendType < BLEND::TXB_PRE_BLEND_END)
			{
				depthStencil = EDepthStencil::eNoDepthMask;
			}
			else if ((proxy.ExtraFlag & REXF_ENABLED_DEPTH) == REXF_ENABLED_DEPTH)
			{
				depthStencil = EDepthStencil::eNoDepthMaskPlusEqual;
			}
			else
			{
				depthStencil = EDepthStencil::eNoZBuffer;
			}

			if (shadowPass == false)
			{
				switch (proxy.RenderType)
				{
				case RT_TEXTURE:
					{
						programShader = depthStencil >= EDepthStencil::eEarlyZBegin ? ECombinedShader::eModelNormalEZ : ECombinedShader::eModelNormal;
					}
					break;

				case RT_COLOR:
					{
						programShader = depthStencil >= EDepthStencil::eEarlyZBegin ? ECombinedShader::eModelColorEZ : ECombinedShader::eModelColor;
					}
					break;

				case RT_CHROME:
					{
						programShader = depthStencil >= EDepthStencil::eEarlyZBegin ? ECombinedShader::eModelChrome01EZ : ECombinedShader::eModelChrome01;
					}
					break;

				case RT_CHROME2:
					{
						programShader = depthStencil >= EDepthStencil::eEarlyZBegin ? ECombinedShader::eModelChrome02EZ : ECombinedShader::eModelChrome02;
					}
					break;

				case RT_CHROME3:
					{
						programShader = depthStencil >= EDepthStencil::eEarlyZBegin ? ECombinedShader::eModelChrome03EZ : ECombinedShader::eModelChrome03;
					}
					break;

				case RT_CHROME4:
					{
						programShader = depthStencil >= EDepthStencil::eEarlyZBegin ? ECombinedShader::eModelChrome04EZ : ECombinedShader::eModelChrome04;
					}
					break;

				case RT_CHROME5:
					{
						programShader = depthStencil >= EDepthStencil::eEarlyZBegin ? ECombinedShader::eModelChrome05EZ : ECombinedShader::eModelChrome05;
					}
					break;

				case RT_CHROME6:
					{
						programShader = depthStencil >= EDepthStencil::eEarlyZBegin ? ECombinedShader::eModelChrome06EZ : ECombinedShader::eModelChrome06;
					}
					break;

				case RT_CHROME7:
					{
						programShader = depthStencil >= EDepthStencil::eEarlyZBegin ? ECombinedShader::eModelChrome07EZ : ECombinedShader::eModelChrome07;
					}
					break;

				case RT_METAL:
					{
						programShader = depthStencil >= EDepthStencil::eEarlyZBegin ? ECombinedShader::eModelMetalEZ : ECombinedShader::eModelMetal;
					}
					break;

				case RT_OIL:
					{
						programShader = depthStencil >= EDepthStencil::eEarlyZBegin ? ECombinedShader::eModelOilEZ : ECombinedShader::eModelOil;
					}
					break;

				default:
					{
						programShader = depthStencil >= EDepthStencil::eEarlyZBegin ? ECombinedShader::eModelNormalEZ : ECombinedShader::eModelNormal;
					}
					break;
				}
			}
			else
			{
				switch (proxy.RenderType)
				{
				case RT_TEXTURE:
					{
						programShader = ECombinedShader::eModelShadowNormalBlend;
					}
					break;

				case RT_COLOR:
					continue;

				case RT_CHROME:
					{
						programShader = ECombinedShader::eModelShadowChrome01Blend;
					}
					break;

				case RT_CHROME2:
					{
						programShader = ECombinedShader::eModelShadowChrome02Blend;
					}
					break;

				case RT_CHROME3:
					{
						programShader = ECombinedShader::eModelShadowChrome03Blend;
					}
					break;

				case RT_CHROME4:
					{
						programShader = ECombinedShader::eModelShadowChrome04Blend;
					}
					break;

				case RT_CHROME5:
					{
						programShader = ECombinedShader::eModelShadowChrome05Blend;
					}
					break;

				case RT_CHROME6:
					{
						programShader = ECombinedShader::eModelShadowChrome06Blend;
					}
					break;

				case RT_CHROME7:
					{
						programShader = ECombinedShader::eModelShadowChrome07Blend;
					}
					break;

				case RT_METAL:
					{
						programShader = ECombinedShader::eModelShadowMetalBlend;
					}
					break;

				case RT_OIL:
					{
						programShader = ECombinedShader::eModelShadowOilBlend;
					}
					break;

				default:
					{
						programShader = ECombinedShader::eModelShadowNormalBlend;
					}
					break;
				}
			}

			EVideoPipeline::SetGraphicsPipeline<backendType>(programShader, rasterizer, depthStencil, blendState, false);

			if (proxy.ModelIndex != lastModel)
			{
				lastModel = proxy.ModelIndex;

				EVideoPipeline::BindGraphicsModelBuffers<backendType>(&model);
				EVideoAttributes::ConfigureAtRuntime<backendType>(EInputLayout::eModel);
			}

			if (proxy.TextureIndex != lastTexture)
			{
				lastTexture = proxy.TextureIndex;

				EVideoPipeline::BindGraphicsModelTexture<backendType>(&texture);
			}

			Model::MeshData1 &mesh = model._MeshData[proxy.MeshIndex];

			if (model._VertexHeader.IndexType == MUModel::INDEX_16BIT)
			{
				EVideoPipeline::DrawIndexedInstanced<useBaseInstance, backendType>(mesh.IndexStart, mesh.IndexCount, proxy.InstanceBase, proxy.InstanceCount.load(std::memory_order_relaxed), ETopologyType::eTriangleList, EIndexType::eIndex16, EInputLayout::eModel);
			}
			else
			{
				EVideoPipeline::DrawIndexedInstanced<useBaseInstance, backendType>(mesh.IndexStart, mesh.IndexCount, proxy.InstanceBase, proxy.InstanceCount.load(std::memory_order_relaxed), ETopologyType::eTriangleList, EIndexType::eIndex32, EInputLayout::eModel);
			}
		}
	}

	const mu_uint32 clothCount = g_ClothRenderSlotCount.load(std::memory_order_relaxed);
	if (clothCount > 0)
	{
		EVideoPipeline::BindGraphicsClothInstancesBuffer<backendType>();

		mu_uint32 lastTexture = -1;

		ECombinedShader programShader;
		EBlendState blendState;
		EDepthStencil depthStencil;
		ERasterizer rasterizer;

		for (mu_uint32 index = 0; index < clothCount; ++index)
		{
			MUClothRenderSlot &slot = g_ClothRenderSlot[g_ClothRenderSlotUsed[index]];

			if (slot.BlendType < BLEND::TXB_NON_DEPTH_BEGIN ||
				slot.BlendType >= BLEND::TXB_NON_DEPTH_END ||
				(shadowPass == true &&
					slot.EnableShadow == false))
			{
				continue;
			}

			MUTexture &texture = TEXTURES::Data[slot.Texture];
			if (texture.IsLoaded() == false)
			{
				if(slot.Texture >= BITMAP_BEGIN_RUNTIME &&
					lastTexture != slot.Texture)
				{
					lastTexture = slot.Texture;
					if(texture.RequestLoad() == false)
					{
						g_LoadingManager.AddRuntime(TEXTURES::LoadData[slot.Texture]);
					}
				}
				continue;
			}

			switch (slot.BlendType)
			{
			case BLEND::TXB_BLEND01:
			case BLEND::TXB_PRE_BLEND01:
				{
					blendState = EBlendState::eOne_One;
				}
				break;

			case BLEND::TXB_BLENDMINUS:
			case BLEND::TXB_PRE_BLENDMINUS:
				{
					blendState = EBlendState::eZero_InvColorSrc;
				}
				break;

			case BLEND::TXB_BLEND02:
			case BLEND::TXB_PRE_BLEND02:
				{
					blendState = EBlendState::eInvColorSrc_One;
				}
				break;

			case BLEND::TXB_BLEND03:
			case BLEND::TXB_PRE_BLEND03:
				{
					blendState = EBlendState::eAlphaSrc_InvAlphaSrc;
				}
				break;

			case BLEND::TXB_BLEND04:
			case BLEND::TXB_PRE_BLEND04:
				{
					blendState = EBlendState::eOne_InvColorSrc;
				}
				break;

			case BLEND::TXB_LIGHTMAP:
			case BLEND::TXB_PRE_LIGHTMAP:
				{
					blendState = EBlendState::eZero_ColorSrc;
				}
				break;
			}

			if ((slot.ExtraFlag & REXF_CULL_FRONT) == REXF_CULL_FRONT)
			{
				rasterizer = ERasterizer::eFrontOnly;
			}
			else if ((slot.ExtraFlag & REXF_CULL_BACK) == REXF_CULL_BACK)
			{
				rasterizer = ERasterizer::eBackOnly;
			}
			else
			{
				rasterizer = ERasterizer::eNoCull;
			}

			if ((slot.ExtraFlag & REXF_DOPPELGANGER) == REXF_DOPPELGANGER)
			{
				depthStencil = EDepthStencil::eNoDepthMask;
			}
			else if (slot.BlendType >= BLEND::TXB_PRE_BLEND_BEGIN &&
					 slot.BlendType < BLEND::TXB_PRE_BLEND_END)
			{
				depthStencil = EDepthStencil::eNoDepthMask;
			}
			else if ((slot.ExtraFlag & REXF_ENABLED_DEPTH) == REXF_ENABLED_DEPTH)
			{
				depthStencil = EDepthStencil::eNoDepthMaskPlusEqual;
			}
			else
			{
				depthStencil = EDepthStencil::eNoZBuffer;
			}

			if (shadowPass == false)
			{
				switch (slot.RenderType)
				{
				case RT_TEXTURE:
					{
						programShader = depthStencil >= EDepthStencil::eEarlyZBegin ? ECombinedShader::eClothNormalEZ : ECombinedShader::eClothNormal;
					}
					break;

				case RT_COLOR:
					{
						programShader = depthStencil >= EDepthStencil::eEarlyZBegin ? ECombinedShader::eClothColorEZ : ECombinedShader::eClothColor;
					}
					break;

				case RT_CHROME:
					{
						programShader = depthStencil >= EDepthStencil::eEarlyZBegin ? ECombinedShader::eClothChrome01EZ : ECombinedShader::eClothChrome01;
					}
					break;

				case RT_CHROME2:
					{
						programShader = depthStencil >= EDepthStencil::eEarlyZBegin ? ECombinedShader::eClothChrome02EZ : ECombinedShader::eClothChrome02;
					}
					break;

				case RT_CHROME3:
					{
						programShader = depthStencil >= EDepthStencil::eEarlyZBegin ? ECombinedShader::eClothChrome03EZ : ECombinedShader::eClothChrome03;
					}
					break;

				case RT_CHROME4:
					{
						programShader = depthStencil >= EDepthStencil::eEarlyZBegin ? ECombinedShader::eClothChrome04EZ : ECombinedShader::eClothChrome04;
					}
					break;

				case RT_CHROME5:
					{
						programShader = depthStencil >= EDepthStencil::eEarlyZBegin ? ECombinedShader::eClothChrome05EZ : ECombinedShader::eClothChrome05;
					}
					break;

				case RT_CHROME6:
					{
						programShader = depthStencil >= EDepthStencil::eEarlyZBegin ? ECombinedShader::eClothChrome06EZ : ECombinedShader::eClothChrome06;
					}
					break;

				case RT_CHROME7:
					{
						programShader = depthStencil >= EDepthStencil::eEarlyZBegin ? ECombinedShader::eClothChrome07EZ : ECombinedShader::eClothChrome07;
					}
					break;

				case RT_METAL:
					{
						programShader = depthStencil >= EDepthStencil::eEarlyZBegin ? ECombinedShader::eClothMetalEZ : ECombinedShader::eClothMetal;
					}
					break;

				case RT_OIL:
					{
						programShader = depthStencil >= EDepthStencil::eEarlyZBegin ? ECombinedShader::eClothOilEZ : ECombinedShader::eClothOil;
					}
					break;

				default:
					{
						programShader = depthStencil >= EDepthStencil::eEarlyZBegin ? ECombinedShader::eClothNormalEZ : ECombinedShader::eClothNormal;
					}
					break;
				}
			}
			else
			{
				switch (slot.RenderType)
				{
				case RT_TEXTURE:
					{
						programShader = ECombinedShader::eClothShadowNormalAlpha;
					}
					break;

				default:
					continue;
				}
			}

			EVideoPipeline::SetGraphicsPipeline<backendType>(programShader, rasterizer, depthStencil, blendState, false);

			if (slot.Texture != lastTexture)
			{
				lastTexture = slot.Texture;

				EVideoPipeline::BindGraphicsModelTexture<backendType>(&texture);
			}

			EVideoPipeline::DrawInstanced<useBaseInstance, backendType>(slot.BaseIndex, slot.IndexCount, slot.InstanceIndex, 1, ETopologyType::eTriangleList, EInputLayout::eCloth);
		}
	}

	EVideoPipeline::UnbindVAO();
}

template<const mu_boolean useBaseInstance, const EVideoBackendType backendType>
void MUTasking::RenderGUIModels(const mu_uint32 slotStart, const mu_uint32 slotCount)
{
#ifdef USE_RENDERING_DISABLE_FLAGS
	if (g_DisableGUIModelRender == true)
		return;
#endif

	MUThreadInfo &ThreadInfo = g_ThreadInfo[0];

	EVideoBackend::ClearDepthAndStencil();

	mu_uint32 usedCount = g_GUIRenderSlotCount;
	mu_uint32 endCount = slotStart + slotCount > usedCount ? usedCount : slotStart + slotCount;

	if (slotStart >= endCount) return;

	if (ThreadInfo.EnableScissor == true)
	{
		EVideoBackend::SetScissor(ThreadInfo.ScissorRect.left, ThreadInfo.ScissorRect.top, ThreadInfo.ScissorRect.right - ThreadInfo.ScissorRect.left, ThreadInfo.ScissorRect.bottom - ThreadInfo.ScissorRect.top);
	}

	EVideoPipeline::BindGraphicsModelInstancesBuffer<backendType>();
	EVideoPipeline::BindGraphicsBonesTexture<backendType>();
	
	mu_uint32 lastModel = -1, lastTexture = -1;

	ECombinedShader programShader;
	EBlendState blendState;
	EDepthStencil depthStencil;
	ERasterizer rasterizer;

	for (mu_uint32 n = slotStart; n < endCount; ++n)
	{
		MUGUIRenderSlot &renderSlot = g_GUIRenderSlot[n];

		if (renderSlot.BlendType >= BLEND::TXB_ALPHATEST_BEGIN &&
			renderSlot.BlendType <= BLEND::TXB_ALPHATEST_END)
		{
			blendState = EBlendState::eAlphaSrc_InvAlphaSrc;
		}
		else
		{
			switch (renderSlot.BlendType)
			{
			case BLEND::TXB_ALPHATEST_NOMASK:
			case BLEND::TXB_ALPHATEST_NONDEPTH:
				{
					blendState = EBlendState::eAlphaSrc_InvAlphaSrc;
				}
				break;

			case BLEND::TXB_BLEND01:
			case BLEND::TXB_PRE_BLEND01:
				{
					blendState = EBlendState::eOne_One;
				}
				break;

			case BLEND::TXB_BLENDMINUS:
			case BLEND::TXB_PRE_BLENDMINUS:
				{
					blendState = EBlendState::eZero_InvColorSrc;
				}
				break;

			case BLEND::TXB_BLEND02:
			case BLEND::TXB_PRE_BLEND02:
				{
					blendState = EBlendState::eInvColorSrc_One;
				}
				break;

			case BLEND::TXB_BLEND03:
			case BLEND::TXB_PRE_BLEND03:
				{
					blendState = EBlendState::eAlphaSrc_InvAlphaSrc;
				}
				break;

			case BLEND::TXB_BLEND04:
			case BLEND::TXB_PRE_BLEND04:
				{
					blendState = EBlendState::eOne_InvColorSrc;
				}
				break;

			case BLEND::TXB_LIGHTMAP:
			case BLEND::TXB_PRE_LIGHTMAP:
				{
					blendState = EBlendState::eZero_ColorSrc;
				}
				break;
			}
		}

		if (ThreadInfo.EnableScissor == true)
		{
			if ((renderSlot.ExtraFlag & REXF_CULL_FRONT) == REXF_CULL_FRONT)
			{
				rasterizer = ERasterizer::eFrontOnly;
			}
			else if ((renderSlot.ExtraFlag & REXF_CULL_BACK) == REXF_CULL_BACK)
			{
				rasterizer = ERasterizer::eBackOnly;
			}
			else
			{
				rasterizer = ERasterizer::eNoCull;
			}
		}
		else
		{
			if ((renderSlot.ExtraFlag & REXF_CULL_FRONT) == REXF_CULL_FRONT)
			{
				rasterizer = ERasterizer::eFrontOnly;
			}
			else if ((renderSlot.ExtraFlag & REXF_CULL_BACK) == REXF_CULL_BACK)
			{
				rasterizer = ERasterizer::eBackOnly;
			}
			else
			{
				rasterizer = ERasterizer::eNoCull;
			}
		}

		if ((renderSlot.ExtraFlag & REXF_DOPPELGANGER) == REXF_DOPPELGANGER)
		{
			depthStencil = EDepthStencil::eZBuffer;
		}
		else if (renderSlot.BlendType >= BLEND::TXB_PRE_BLEND_BEGIN &&
				 renderSlot.BlendType < BLEND::TXB_PRE_BLEND_END)
		{
			depthStencil = EDepthStencil::eNoDepthMask;
		}
		else if (renderSlot.BlendType >= BLEND::TXB_DEPTH_BEGIN &&
				 renderSlot.BlendType < BLEND::TXB_DEPTH_END &&
				 renderSlot.BlendType != BLEND::TXB_ALPHATEST_NONDEPTH)
		{
			if ((renderSlot.ExtraFlag & REXF_ENABLED_DEPTH) == REXF_ENABLED_DEPTH)
			{
				depthStencil = EDepthStencil::eZBuffer;
			}
			else
			{
				depthStencil = EDepthStencil::eNoZBuffer;
			}
		}
		else if (renderSlot.BlendType == BLEND::TXB_ALPHATEST_NONDEPTH)
		{
			if ((renderSlot.ExtraFlag & REXF_ENABLED_DEPTH) == REXF_ENABLED_DEPTH)
			{
				depthStencil = EDepthStencil::eZBuffer;
			}
			else
			{
				depthStencil = EDepthStencil::eNoZBuffer;
			}
		}
		else
		{
			if ((renderSlot.ExtraFlag & REXF_ENABLED_DEPTH) == REXF_ENABLED_DEPTH)
			{
				depthStencil = EDepthStencil::eNoDepthMaskPlusEqual;
			}
			else
			{
				depthStencil = EDepthStencil::eNoZBuffer;
			}
		}

		switch (renderSlot.RenderType)
		{
		case RT_TEXTURE:
			{
				programShader = depthStencil >= EDepthStencil::eEarlyZBegin ? ECombinedShader::eModelNormalEZ : ECombinedShader::eModelNormal;
			}
			break;

		case RT_COLOR:
			{
				programShader = depthStencil >= EDepthStencil::eEarlyZBegin ? ECombinedShader::eModelColorEZ : ECombinedShader::eModelColor;
			}
			break;

		case RT_CHROME:
			{
				programShader = depthStencil >= EDepthStencil::eEarlyZBegin ? ECombinedShader::eModelChrome01EZ : ECombinedShader::eModelChrome01;
			}
			break;

		case RT_CHROME2:
			{
				programShader = depthStencil >= EDepthStencil::eEarlyZBegin ? ECombinedShader::eModelChrome02EZ : ECombinedShader::eModelChrome02;
			}
			break;

		case RT_CHROME3:
			{
				programShader = depthStencil >= EDepthStencil::eEarlyZBegin ? ECombinedShader::eModelChrome03EZ : ECombinedShader::eModelChrome03;
			}
			break;

		case RT_CHROME4:
			{
				programShader = depthStencil >= EDepthStencil::eEarlyZBegin ? ECombinedShader::eModelChrome04EZ : ECombinedShader::eModelChrome04;
			}
			break;

		case RT_CHROME5:
			{
				programShader = depthStencil >= EDepthStencil::eEarlyZBegin ? ECombinedShader::eModelChrome05EZ : ECombinedShader::eModelChrome05;
			}
			break;

		case RT_CHROME6:
			{
				programShader = depthStencil >= EDepthStencil::eEarlyZBegin ? ECombinedShader::eModelChrome06EZ : ECombinedShader::eModelChrome06;
			}
			break;

		case RT_CHROME7:
			{
				programShader = depthStencil >= EDepthStencil::eEarlyZBegin ? ECombinedShader::eModelChrome07EZ : ECombinedShader::eModelChrome07;
			}
			break;

		case RT_METAL:
			{
				programShader = depthStencil >= EDepthStencil::eEarlyZBegin ? ECombinedShader::eModelMetalEZ : ECombinedShader::eModelMetal;
			}
			break;

		case RT_OIL:
			{
				programShader = depthStencil >= EDepthStencil::eEarlyZBegin ? ECombinedShader::eModelOilEZ : ECombinedShader::eModelOil;
			}
			break;

		default:
			{
				programShader = depthStencil >= EDepthStencil::eEarlyZBegin ? ECombinedShader::eModelNormalEZ : ECombinedShader::eModelNormal;
			}
			break;
		}

		EVideoPipeline::SetGraphicsPipeline<backendType>(programShader, rasterizer, depthStencil, blendState, ThreadInfo.EnableScissor);

		MUModel &model = MODELS::Data[renderSlot.ModelIndex];
		if (model.IsGPUAvailable() == false)
		{
			if (lastModel != renderSlot.ModelIndex)
			{
				lastModel = renderSlot.ModelIndex;
				model.RequestLoad();
			}
			continue;
		}

		MUTexture &texture = TEXTURES::Data[renderSlot.Texture];
		if(texture.IsLoaded() == false)
		{
			if (renderSlot.Texture >= BITMAP_BEGIN_RUNTIME &&
				lastTexture != renderSlot.Texture)
			{
				lastTexture = renderSlot.Texture;
				if (texture.RequestLoad() == false)
				{
					g_LoadingManager.AddRuntime(TEXTURES::LoadData[renderSlot.Texture]);
				}
			}
			continue;
		}

		if (renderSlot.ModelIndex != lastModel)
		{
			lastModel = renderSlot.ModelIndex;

			EVideoPipeline::BindGraphicsModelBuffers<backendType>(&model);
			EVideoAttributes::ConfigureAtRuntime<backendType>(EInputLayout::eModel);
		}

		if (renderSlot.Texture != lastTexture)
		{
			lastTexture = renderSlot.Texture;

			EVideoPipeline::BindGraphicsModelTexture<backendType>(&texture);
		}

		Model::MeshData1 &mesh = model._MeshData[renderSlot.MeshIndex];

		if (model._VertexHeader.IndexType == MUModel::INDEX_16BIT)
		{
			EVideoPipeline::DrawIndexedInstanced<useBaseInstance, backendType>(mesh.IndexStart, mesh.IndexCount, renderSlot.InstanceIndex, 1, ETopologyType::eTriangleList, EIndexType::eIndex16, EInputLayout::eModel);
		}
		else
		{
			EVideoPipeline::DrawIndexedInstanced<useBaseInstance, backendType>(mesh.IndexStart, mesh.IndexCount, renderSlot.InstanceIndex, 1, ETopologyType::eTriangleList, EIndexType::eIndex32, EInputLayout::eModel);
		}
	}

	EVideoPipeline::UnbindVAO();
}

void MUTasking::RenderGUIModelsAtRuntime(const mu_uint32 slotStart, const mu_uint32 slotCount)
{
	switch (EVideoBackend::GetBackend())
	{
#if DIRECTX_BACKEND_ENABLE == 1
	case EVideoBackendType::eDirectX:
		{
			RenderGUIModels<false, EVideoBackendType::eDirectX>(slotStart, slotCount);
		}
		break;
#endif

#if OPENGL_BACKEND_ENABLE == 1
	case EVideoBackendType::eOpenGL:
		{
			if (EOpenGLExtensions::IsFeatureSupported(OpenGLFeatures::BaseInstance) == true)
			{
				RenderGUIModels<true, EVideoBackendType::eOpenGL>(slotStart, slotCount);
			}
			else
			{
				RenderGUIModels<false, EVideoBackendType::eOpenGL>(slotStart, slotCount);
			}
		}
		break;
#endif

#if VULKAN_BACKEND_ENABLE == 1
	case EVideoBackendType::eVulkan:
		{
			RenderGUIModels<false, EVideoBackendType::eVulkan>(slotStart, slotCount);
		}
		break;
#endif

#if METAL_BACKEND_ENABLE == 1
	case EVideoBackendType::eMetal:
		{
			RenderGUIModels<false, EVideoBackendType::eMetal>(slotStart, slotCount);
		}
		break;
#endif

	default: break;
	}
}

template<const mu_boolean useBaseInstance, const EVideoBackendType backendType>
void MUTasking::RenderShadowModels()
{
	EVideoPipeline::BindGraphicsBonesTexture<backendType>();

	if (g_PreRenderSlotCount.load(std::memory_order_relaxed) > 0 ||
		g_PostRenderSlotCount.load(std::memory_order_relaxed) > 0)
	{
		EVideoPipeline::BindGraphicsModelInstancesBuffer<backendType>();
		EVideoPipeline::BindGraphicsModelTerrainData<backendType>(EUniformLayout::eShadow, &g_Terrain._AlphaTexture, &g_Terrain._HeightTexture, &g_Terrain._AttributeTexture);

		ECombinedShader programShader;
		EBlendState blendState = EBlendState::eAlphaSrc_InvAlphaSrc;
		EDepthStencil depthStencil = EDepthStencil::eShadow;
		ERasterizer rasterizer = ERasterizer::eNoCull;

		mu_uint32 lastModel = -1, lastTexture = -1;
		mu_uint32 instanceOffset = -1;

		for (mu_uint32 index = 0; index < g_RenderProxyCount; ++index)
		{
			MURenderProxy &proxy = g_RenderProxy[g_RenderProxyIndex[index]];

			if (proxy.Enabled.load(std::memory_order_relaxed) == false ||
				proxy.ShadowEnabled.load(std::memory_order_relaxed) == false)
			{
				continue;
			}

			if (proxy.BlendType < BLEND::TXB_DEPTH_BEGIN ||
				proxy.BlendType >= BLEND::TXB_DEPTH_END)
			{
				continue;
			}

			switch (proxy.RenderType)
			{
			case RT_TEXTURE:
				{
					programShader = ECombinedShader::eModelShadowNormalAlpha;
				}
				break;

			case RT_COLOR:
				continue;

			case RT_CHROME:
				{
					programShader = ECombinedShader::eModelShadowChrome01Alpha;
				}
				break;

			case RT_CHROME2:
				{
					programShader = ECombinedShader::eModelShadowChrome02Alpha;
				}
				break;

			case RT_CHROME3:
				{
					programShader = ECombinedShader::eModelShadowChrome03Alpha;
				}
				break;

			case RT_CHROME4:
				{
					programShader = ECombinedShader::eModelShadowChrome04Alpha;
				}
				break;

			case RT_CHROME5:
				{
					programShader = ECombinedShader::eModelShadowChrome05Alpha;
				}
				break;

			case RT_CHROME6:
				{
					programShader = ECombinedShader::eModelShadowChrome06Alpha;
				}
				break;

			case RT_CHROME7:
				{
					programShader = ECombinedShader::eModelShadowChrome07Alpha;
				}
				break;

			case RT_METAL:
				{
					programShader = ECombinedShader::eModelShadowMetalAlpha;
				}
				break;

			case RT_OIL:
				{
					programShader = ECombinedShader::eModelShadowOilAlpha;
				}
				break;

			default:
				{
					programShader = ECombinedShader::eModelShadowNormalAlpha;
				}
				break;
			}

			EVideoPipeline::SetGraphicsPipeline<backendType>(programShader, rasterizer, depthStencil, blendState, false);

			MUModel &model = MODELS::Data[proxy.ModelIndex];
			if (model.IsGPUAvailable() == false)
			{
				if (lastModel != proxy.ModelIndex)
				{
					lastModel = proxy.ModelIndex;
					model.RequestLoad();
				}
				continue;
			}

			MUTexture &texture = TEXTURES::Data[proxy.TextureIndex];
			if (texture.IsLoaded() == false)
			{
				if (proxy.TextureIndex >= BITMAP_BEGIN_RUNTIME &&
					lastTexture != proxy.TextureIndex)
				{
					lastTexture = proxy.TextureIndex;
					if (texture.RequestLoad() == false)
					{
						g_LoadingManager.AddRuntime(TEXTURES::LoadData[proxy.TextureIndex]);
					}
				}
				continue;
			}

			if (proxy.ModelIndex != lastModel)
			{
				lastModel = proxy.ModelIndex;

				EVideoPipeline::BindGraphicsModelBuffers<backendType>(&model);
				EVideoAttributes::ConfigureAtRuntime<backendType>(EInputLayout::eModel);
			}

			if (proxy.TextureIndex != lastTexture)
			{
				lastTexture = proxy.TextureIndex;

				EVideoPipeline::BindGraphicsModelTexture<backendType>(&texture);
			}

			Model::MeshData1 &mesh = model._MeshData[proxy.MeshIndex];

			if (model._VertexHeader.IndexType == MUModel::INDEX_16BIT)
			{
				EVideoPipeline::DrawIndexedInstanced<useBaseInstance, backendType>(mesh.IndexStart, mesh.IndexCount, proxy.InstanceBase, proxy.InstanceCount.load(std::memory_order_relaxed), ETopologyType::eTriangleList, EIndexType::eIndex16, EInputLayout::eModel);
			}
			else
			{
				EVideoPipeline::DrawIndexedInstanced<useBaseInstance, backendType>(mesh.IndexStart, mesh.IndexCount, proxy.InstanceBase, proxy.InstanceCount.load(std::memory_order_relaxed), ETopologyType::eTriangleList, EIndexType::eIndex32, EInputLayout::eModel);
			}
		}

		for (mu_uint32 index = 0; index < g_RenderProxyCount; ++index)
		{
			MURenderProxy &proxy = g_RenderProxy[g_RenderProxyIndex[index]];

			if (proxy.Enabled.load(std::memory_order_relaxed) == false ||
				proxy.ShadowEnabled.load(std::memory_order_relaxed) == false)
			{
				continue;
			}

			if (proxy.BlendType < BLEND::TXB_NON_DEPTH_BEGIN ||
				proxy.BlendType >= BLEND::TXB_NON_DEPTH_END)
			{
				continue;
			}

			switch (proxy.RenderType)
			{
			case RT_TEXTURE:
				{
					programShader = ECombinedShader::eModelShadowNormalBlend;
				}
				break;

			case RT_COLOR:
				continue;

			case RT_CHROME:
				{
					programShader = ECombinedShader::eModelShadowChrome01Blend;
				}
				break;

			case RT_CHROME2:
				{
					programShader = ECombinedShader::eModelShadowChrome02Blend;
				}
				break;

			case RT_CHROME3:
				{
					programShader = ECombinedShader::eModelShadowChrome03Blend;
				}
				break;

			case RT_CHROME4:
				{
					programShader = ECombinedShader::eModelShadowChrome04Blend;
				}
				break;

			case RT_CHROME5:
				{
					programShader = ECombinedShader::eModelShadowChrome05Blend;
				}
				break;

			case RT_CHROME6:
				{
					programShader = ECombinedShader::eModelShadowChrome06Blend;
				}
				break;

			case RT_CHROME7:
				{
					programShader = ECombinedShader::eModelShadowChrome07Blend;
				}
				break;

			case RT_METAL:
				{
					programShader = ECombinedShader::eModelShadowMetalBlend;
				}
				break;

			case RT_OIL:
				{
					programShader = ECombinedShader::eModelShadowOilBlend;
				}
				break;

			default:
				{
					programShader = ECombinedShader::eModelShadowNormalBlend;
				}
				break;
			}

			EVideoPipeline::SetGraphicsPipeline<backendType>(programShader, rasterizer, depthStencil, blendState, false);

			MUModel &model = MODELS::Data[proxy.ModelIndex];
			if (model.IsGPUAvailable() == false)
			{
				if (lastModel != proxy.ModelIndex)
				{
					lastModel = proxy.ModelIndex;
					model.RequestLoad();
				}
				continue;
			}

			MUTexture &texture = TEXTURES::Data[proxy.TextureIndex];
			if (texture.IsLoaded() == false)
			{
				if (proxy.TextureIndex >= BITMAP_BEGIN_RUNTIME &&
					lastTexture != proxy.TextureIndex)
				{
					lastTexture = proxy.TextureIndex;
					if (texture.RequestLoad() == false)
					{
						g_LoadingManager.AddRuntime(TEXTURES::LoadData[proxy.TextureIndex]);
					}
				}
				continue;
			}

			if (proxy.ModelIndex != lastModel)
			{
				lastModel = proxy.ModelIndex;

				EVideoPipeline::BindGraphicsModelBuffers<backendType>(&model);
				EVideoAttributes::ConfigureAtRuntime<backendType>(EInputLayout::eModel);
			}

			if (proxy.TextureIndex != lastTexture)
			{
				lastTexture = proxy.TextureIndex;

				EVideoPipeline::BindGraphicsModelTexture<backendType>(&texture);
			}

			Model::MeshData1 &mesh = model._MeshData[proxy.MeshIndex];

			if (model._VertexHeader.IndexType == MUModel::INDEX_16BIT)
			{
				EVideoPipeline::DrawIndexedInstanced<useBaseInstance, backendType>(mesh.IndexStart, mesh.IndexCount, proxy.InstanceBase, proxy.InstanceCount.load(std::memory_order_relaxed), ETopologyType::eTriangleList, EIndexType::eIndex16, EInputLayout::eModel);
			}
			else
			{
				EVideoPipeline::DrawIndexedInstanced<useBaseInstance, backendType>(mesh.IndexStart, mesh.IndexCount, proxy.InstanceBase, proxy.InstanceCount.load(std::memory_order_relaxed), ETopologyType::eTriangleList, EIndexType::eIndex32, EInputLayout::eModel);
			}
		}
	}

	const mu_uint32 clothCount = g_ClothRenderSlotCount.load(std::memory_order_relaxed);
	if (clothCount > 0)
	{
		EVideoPipeline::BindGraphicsClothInstancesBuffer<backendType>();

		mu_uint32 lastTexture = -1;

		ECombinedShader programShader;
		EBlendState blendState = EBlendState::eAlphaSrc_InvAlphaSrc;
		EDepthStencil depthStencil = EDepthStencil::eShadow;
		ERasterizer rasterizer = ERasterizer::eNoCull;

		for (mu_uint32 index = 0; index < clothCount; ++index)
		{
			MUClothRenderSlot &slot = g_ClothRenderSlot[g_ClothRenderSlotUsed[index]];

			if (slot.EnableShadow == false ||
				slot.BlendType < BLEND::TXB_DEPTH_BEGIN ||
				slot.BlendType >= BLEND::TXB_DEPTH_END)
			{
				continue;
			}

			MUTexture &texture = TEXTURES::Data[slot.Texture];
			if (texture.IsLoaded() == false)
			{
				if (slot.Texture >= BITMAP_BEGIN_RUNTIME &&
					lastTexture != slot.Texture)
				{
					lastTexture = slot.Texture;
					if (texture.RequestLoad() == false)
					{
						g_LoadingManager.AddRuntime(TEXTURES::LoadData[slot.Texture]);
					}
				}
				continue;
			}

			switch (slot.RenderType)
			{
			case RT_TEXTURE:
				{
					programShader = ECombinedShader::eClothShadowNormalAlpha;
				}
				break;
			}

			EVideoPipeline::SetGraphicsPipeline<backendType>(programShader, rasterizer, depthStencil, blendState, false);

			if (slot.Texture != lastTexture)
			{
				lastTexture = slot.Texture;

				EVideoPipeline::BindGraphicsModelTexture<backendType>(&texture);
			}

			EVideoPipeline::DrawInstanced<useBaseInstance, backendType>(slot.BaseIndex, slot.IndexCount, slot.InstanceIndex, 1, ETopologyType::eTriangleList, EInputLayout::eCloth);
		}

		for (mu_uint32 index = 0; index < clothCount; ++index)
		{
			MUClothRenderSlot &slot = g_ClothRenderSlot[g_ClothRenderSlotUsed[index]];

			if (slot.EnableShadow == false ||
				slot.BlendType < BLEND::TXB_NON_DEPTH_BEGIN ||
				slot.BlendType >= BLEND::TXB_NON_DEPTH_END)
			{
				continue;
			}

			MUTexture &texture = TEXTURES::Data[slot.Texture];
			if (texture.IsLoaded() == false)
			{
				if (slot.Texture >= BITMAP_BEGIN_RUNTIME &&
					lastTexture != slot.Texture)
				{
					lastTexture = slot.Texture;
					if (texture.RequestLoad() == false)
					{
						g_LoadingManager.AddRuntime(TEXTURES::LoadData[slot.Texture]);
					}
				}
				continue;
			}

			switch (slot.RenderType)
			{
			case RT_TEXTURE:
				{
					programShader = ECombinedShader::eClothShadowNormalBlend;
				}
				break;

			default:
				continue;
			}

			EVideoPipeline::SetGraphicsPipeline<backendType>(programShader, rasterizer, depthStencil, blendState, false);

			if (slot.Texture != lastTexture)
			{
				lastTexture = slot.Texture;

				EVideoPipeline::BindGraphicsModelTexture<backendType>(&texture);
			}

			EVideoPipeline::DrawInstanced<useBaseInstance, backendType>(slot.BaseIndex, slot.IndexCount, slot.InstanceIndex, 1, ETopologyType::eTriangleList, EInputLayout::eCloth);
		}
	}

	EVideoPipeline::UnbindVAO();
}

void MUTasking::RenderShadowMapping()
{
	switch(EVideoBackend::GetBackend())
	{
#if DIRECTX_BACKEND_ENABLE == 1
	case EVideoBackendType::eDirectX:
		{
			RenderPostModels<false, EVideoBackendType::eDirectX>(true);
			RenderPreModels<false, EVideoBackendType::eDirectX>(true);
			RenderPreAlphaModels<false, EVideoBackendType::eDirectX>(true);
		}
		break;
#endif

#if OPENGL_BACKEND_ENABLE == 1
	case EVideoBackendType::eOpenGL:
		{
			if (EOpenGLExtensions::IsFeatureSupported(OpenGLFeatures::BaseInstance) == true)
			{
				RenderPostModels<true, EVideoBackendType::eOpenGL>(true);
				RenderPreModels<true, EVideoBackendType::eOpenGL>(true);
				RenderPreAlphaModels<true, EVideoBackendType::eOpenGL>(true);
			}
			else
			{
				RenderPostModels<false, EVideoBackendType::eOpenGL>(true);
				RenderPreModels<false, EVideoBackendType::eOpenGL>(true);
				RenderPreAlphaModels<false, EVideoBackendType::eOpenGL>(true);
			}
		}
		break;
#endif

#if VULKAN_BACKEND_ENABLE == 1
	case EVideoBackendType::eVulkan:
		{
			RenderPostModels<false, EVideoBackendType::eVulkan>(true);
			RenderPreModels<false, EVideoBackendType::eVulkan>(true);
			RenderPreAlphaModels<false, EVideoBackendType::eVulkan>(true);
		}
		break;
#endif

#if METAL_BACKEND_ENABLE == 1
	case EVideoBackendType::eMetal:
		{
			RenderPostModels<false, EVideoBackendType::eMetal>(true);
			RenderPreModels<false, EVideoBackendType::eMetal>(true);
			RenderPreAlphaModels<false, EVideoBackendType::eMetal>(true);
		}
		break;
#endif

	default: break;
	}
}