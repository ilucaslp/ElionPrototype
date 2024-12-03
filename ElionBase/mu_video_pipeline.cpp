#include "stdafx.h"

namespace EVideoPipeline
{
#define TEMPLATE_INIT_HELPER(funcname, ...) \
template void funcname <EVideoBackendType::eDirectX>(__VA_ARGS__); \
template void funcname <EVideoBackendType::eOpenGL>(__VA_ARGS__); \
template void funcname <EVideoBackendType::eVulkan>(__VA_ARGS__); \
template void funcname <EVideoBackendType::eMetal>(__VA_ARGS__);

	TEMPLATE_INIT_HELPER(BindGraphicsBonesTexture, void);
	TEMPLATE_INIT_HELPER(BindGraphicsTerrainTexture, MUTexture *TilesTexture, const mu_uint32 WaterIndex);
	TEMPLATE_INIT_HELPER(BindGraphicsTerrainData, const EUniformLayout Layout, MUTexture *AlphaTexture, MUTexture *HeightTexture, MUTexture *AttributeTexture);
	TEMPLATE_INIT_HELPER(BindGraphicsTerrainBuffers, EBufferBackend *InstanceBuffer);
	TEMPLATE_INIT_HELPER(UnbindGraphicsTerrainBuffers, void);
	TEMPLATE_INIT_HELPER(BindGraphicsGrassTexture, MUTexture *GrassTexture);
	TEMPLATE_INIT_HELPER(BindGraphicsGrassBuffers, EBufferBackend *InstanceBuffer);
	TEMPLATE_INIT_HELPER(UnbindGraphicsGrassBuffers, void);
	TEMPLATE_INIT_HELPER(BindGraphicsShadowMap, const EUniformLayout Layout);
	TEMPLATE_INIT_HELPER(BindGraphicsModelTexture, MUTexture *ModelTexture);
	TEMPLATE_INIT_HELPER(BindGraphicsModelInstancesBuffer, void);
	TEMPLATE_INIT_HELPER(BindGraphicsModelTerrainData, const EUniformLayout Layout, MUTexture *AlphaTexture, MUTexture *HeightTexture, MUTexture *AttributeTexture);
	TEMPLATE_INIT_HELPER(BindGraphicsClothInstancesBuffer, void);
	TEMPLATE_INIT_HELPER(BindGraphicsModelBuffers, MUModel *Model);
	TEMPLATE_INIT_HELPER(BindGraphicsEffectTerrainData, void);
	TEMPLATE_INIT_HELPER(BindGraphicsEffectInstancesBuffer, void);
	TEMPLATE_INIT_HELPER(BindGraphicsEffectSamplers, void);
	TEMPLATE_INIT_HELPER(BindGraphicsEffectTexture, MUTexture *EffectTexture);

#undef TEMPLATE_INIT_HELPER

	void BindGraphicsUITextureAtRuntime(const ETextureBind Binds)
	{
		const EVideoBackendType backendType = EVideoBackend::GetBackend();

		switch (backendType)
		{
#if DIRECTX_BACKEND_ENABLE == 1
		case EVideoBackendType::eDirectX:
			{
				EDirectXPipeline::SetFragmentTexture(0, Binds.Texture);
				EDirectXPipeline::SetFragmentSampler(0, Binds.Sampler);
			}
			break;
#endif

#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
			{
				EOpenGLPipeline::SetFragmentTexture(0, Binds.Texture);
				EOpenGLPipeline::SetFragmentSampler(0, Binds.Sampler, false);
			}
			break;
#endif

#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
			{
				VkDescriptorSet DescriptorSets[2] = {};

				DescriptorSets[0] = Binds.Texture->VulkanInternal.DescriptorSet;
				DescriptorSets[1] = EVulkanDescriptor::GetSamplerDescriptorSet(Binds.Sampler);

				EVulkanPipeline::BindDescriptorSets(VK_PIPELINE_BIND_POINT_GRAPHICS, EVulkanPipeline::GetPipelineLayout(EUniformLayout::eUIDefault), EVulkan::VULKAN_DESCRIPTORSET_UITEXTURE, mu_countof(DescriptorSets), DescriptorSets);
			}
			break;
#endif

		default: break;
		}
	}

	template<EVideoBackendType backendType>
	void BindGraphicsBonesTexture()
	{
		if constexpr (backendType == EVideoBackendType::eDirectX)
		{
#if DIRECTX_BACKEND_ENABLE == 1
			const MUTexture *bonesTexture = EDirectXBuffer::GetCurrentBonesTexture();
			EDirectXPipeline::SetVertexTexture(0, bonesTexture);
			EDirectXPipeline::SetVertexSampler(0, bonesTexture->TextureInternal.Sampler);
#endif
		}
		else if constexpr (backendType == EVideoBackendType::eOpenGL)
		{
#if OPENGL_BACKEND_ENABLE == 1
			const MUTexture *bonesTexture = EOpenGLBuffer::GetCurrentBonesTexture();
			EOpenGLPipeline::SetVertexTexture(0, bonesTexture);
			EOpenGLPipeline::SetVertexSampler(0, bonesTexture->TextureInternal.Sampler, false);
#endif
		}
		else if constexpr (backendType == EVideoBackendType::eVulkan)
		{
#if VULKAN_BACKEND_ENABLE == 1
			VkDescriptorSet DescriptorSets[1] = { EVulkanSkeleton::GetBonesDescriptorSet() };
			EVulkanPipeline::BindDescriptorSets(VK_PIPELINE_BIND_POINT_GRAPHICS, EVulkanPipeline::GetPipelineLayout(EUniformLayout::eModel), EVulkan::VULKAN_DESCRIPTORSET_BONES, mu_countof(DescriptorSets), DescriptorSets);
#endif
		}
	}

	template<EVideoBackendType backendType>
	void BindGraphicsTerrainTexture(MUTexture *TilesTexture, const mu_uint32 WaterIndex)
	{
		if constexpr (backendType == EVideoBackendType::eDirectX)
		{
#if DIRECTX_BACKEND_ENABLE == 1
			const MUTexture *textures[2] = { TilesTexture, &TEXTURES::Data[BITMAP_WATER + WaterIndex] };
			const ESampler samplers[2] = { ESampler::eUrVrWrMinLMagLMipL_Anisotropy, ESampler::eUrVrWrMinLMagLMipL_Anisotropy };

			EDirectXPipeline::SetFragmentTexture(0, mu_countof(textures), textures);
			EDirectXPipeline::SetFragmentSampler(0, mu_countof(samplers), samplers);
#endif
		}
		else if constexpr (backendType == EVideoBackendType::eOpenGL)
		{
#if OPENGL_BACKEND_ENABLE == 1
			const MUTexture *textures[2] = { TilesTexture, &TEXTURES::Data[BITMAP_WATER + WaterIndex] };
			const ESampler samplers[2] = { ESampler::eUrVrWrMinLMagLMipL_Anisotropy, ESampler::eUrVrWrMinLMagLMipL_Anisotropy };
			const mu_boolean samplersMipmap[2] = { true, true };

			EOpenGLPipeline::SetFragmentTexture(0, mu_countof(textures), textures);
			EOpenGLPipeline::SetFragmentSampler(0, mu_countof(samplers), samplers, samplersMipmap);
#endif
		}
		else if constexpr (backendType == EVideoBackendType::eVulkan)
		{
#if VULKAN_BACKEND_ENABLE == 1
			VkDescriptorSet DescriptorSets[1] = { EVulkanDescriptor::GetTerrainTextureDescriptorSet(WaterIndex) };
			EVulkanPipeline::BindDescriptorSets(VK_PIPELINE_BIND_POINT_GRAPHICS, EVulkanPipeline::GetPipelineLayout(EUniformLayout::eTerrainCommon), EVulkan::VULKAN_DESCRIPTORSET_TERRAINTEXTURE, mu_countof(DescriptorSets), DescriptorSets);
			EVulkan::MarkBusyMask(TilesTexture->TextureInternal.BusyMask, EVulkan::CurrentCommandBufferIndex);
#endif
		}
	}

	template<EVideoBackendType backendType>
	void BindGraphicsTerrainData(const EUniformLayout Layout, MUTexture *AlphaTexture, MUTexture *HeightTexture, MUTexture *AttributeTexture)
	{
		if constexpr(backendType == EVideoBackendType::eDirectX)
		{
#if DIRECTX_BACKEND_ENABLE == 1
			const MUTexture *textures[4] =
			{
				EDirectXBuffer::GetTerrainLightTexture(EDirectX::CurrentCommandBufferIndex),
				AlphaTexture,
				HeightTexture,
				AttributeTexture
			};
			const ESampler samplers[4] =
			{
				textures[0]->TextureInternal.Sampler,
				textures[1]->TextureInternal.Sampler,
				textures[2]->TextureInternal.Sampler,
				textures[3]->TextureInternal.Sampler
			};

			EDirectXPipeline::SetVertexTexture(0, mu_countof(textures), textures);
			EDirectXPipeline::SetVertexSampler(0, mu_countof(samplers), samplers);
#endif
		}
		else if constexpr(backendType == EVideoBackendType::eOpenGL)
		{
#if OPENGL_BACKEND_ENABLE == 1
			const MUTexture *textures[4] =
			{
				EOpenGLBuffer::GetTerrainLightTexture(EOpenGL::CurrentCommandBufferIndex),
				AlphaTexture,
				HeightTexture,
				AttributeTexture
			};
			const ESampler samplers[4] =
			{
				textures[0]->TextureInternal.Sampler,
				textures[1]->TextureInternal.Sampler,
				textures[2]->TextureInternal.Sampler,
				textures[3]->TextureInternal.Sampler
			};
			const mu_boolean samplersMipmap[4] = { false, false, false, false };

			EOpenGLPipeline::SetVertexTexture(0, mu_countof(textures), textures);
			EOpenGLPipeline::SetVertexSampler(0, mu_countof(samplers), samplers, samplersMipmap);
#endif
		}
		else if constexpr (backendType == EVideoBackendType::eVulkan)
		{
#if VULKAN_BACKEND_ENABLE == 1
			VkDescriptorSet DescriptorSets[1] = { EVulkanDescriptor::GetTerrainDataDescriptorSet() };
			EVulkanPipeline::BindDescriptorSets(VK_PIPELINE_BIND_POINT_GRAPHICS, EVulkanPipeline::GetPipelineLayout(Layout), EVulkan::VULKAN_DESCRIPTORSET_TERRAINDATA, mu_countof(DescriptorSets), DescriptorSets);
			EVulkan::MarkBusyMask(AlphaTexture->TextureInternal.BusyMask, EVulkan::CurrentCommandBufferIndex);
			EVulkan::MarkBusyMask(HeightTexture->TextureInternal.BusyMask, EVulkan::CurrentCommandBufferIndex);
			EVulkan::MarkBusyMask(AttributeTexture->TextureInternal.BusyMask, EVulkan::CurrentCommandBufferIndex);
#endif
		}
	}

	template<EVideoBackendType backendType>
	void BindGraphicsTerrainBuffers(EBufferBackend *InstanceBuffer)
	{
		if constexpr (backendType == EVideoBackendType::eDirectX)
		{
#if DIRECTX_BACKEND_ENABLE == 1
			ID3D11Buffer *buffers[] = { InstanceBuffer->DirectXInternal.Resource };
			mu_uint32 buffersStride[] = { sizeof(SHTerrainInstance) };
			mu_uint32 buffersOffset[] = { 0 };

			EDirectXPipeline::SetVertexBuffers(0, mu_countof(buffers), buffers, buffersStride, buffersOffset);
			EDirectXPipeline::SetIndexBuffer(EDirectXBuffer::GetDummyIndexResource(), DXGI_FORMAT_R16_UINT, 0);
#endif
		}
		else if constexpr (backendType == EVideoBackendType::eOpenGL)
		{
#if OPENGL_BACKEND_ENABLE == 1
			EOpenGLPipeline::BindVAO(EOpenGLBuffer::RetrieveTerrainVAO());
#endif
		}
		else if constexpr (backendType == EVideoBackendType::eVulkan)
		{
#if VULKAN_BACKEND_ENABLE == 1
			EVulkanPipeline::BindIndexBuffer(EVulkanBuffer::DummyIndexBuffer.VulkanInternal.Buffer, VK_INDEX_TYPE_UINT16);
			EVulkanPipeline::BindVertexBuffers(0, 1, &InstanceBuffer->VulkanInternal.Buffer);
			EVulkan::MarkBusyMask(InstanceBuffer->BufferInternal.BusyMask, EVulkan::CurrentCommandBufferIndex);
#endif
		}
	}

	template<EVideoBackendType backendType>
	void UnbindGraphicsTerrainBuffers()
	{
		if constexpr (backendType == EVideoBackendType::eDirectX)
		{
#if DIRECTX_BACKEND_ENABLE == 1
#endif
		}
		else if constexpr (backendType == EVideoBackendType::eOpenGL)
		{
#if OPENGL_BACKEND_ENABLE == 1
			EOpenGLPipeline::BindVAO(EInvalidUInt32);
#endif
		}
		else if constexpr (backendType == EVideoBackendType::eVulkan)
		{
#if VULKAN_BACKEND_ENABLE == 1
#endif
		}
	}

	template<EVideoBackendType backendType>
	void BindGraphicsGrassTexture(MUTexture *GrassTexture)
	{
		if constexpr (backendType == EVideoBackendType::eDirectX)
		{
#if DIRECTX_BACKEND_ENABLE == 1
			const MUTexture *textures[1] = { GrassTexture };
			const ESampler samplers[1] = { ESampler::eUrVrWrMinLMagLMipL_Anisotropy };

			EDirectXPipeline::SetFragmentTexture(0, mu_countof(textures), textures);
			EDirectXPipeline::SetFragmentSampler(0, mu_countof(samplers), samplers);
#endif
		}
		else if constexpr (backendType == EVideoBackendType::eOpenGL)
		{
#if OPENGL_BACKEND_ENABLE == 1
			const MUTexture *textures[1] = { GrassTexture };
			const ESampler samplers[1] = { ESampler::eUrVrWrMinLMagLMipL_Anisotropy };
			const mu_boolean samplersMipmap[1] = { true };

			EOpenGLPipeline::SetFragmentTexture(0, mu_countof(textures), textures);
			EOpenGLPipeline::SetFragmentSampler(0, mu_countof(samplers), samplers, samplersMipmap);
#endif
		}
		else if constexpr (backendType == EVideoBackendType::eVulkan)
		{
#if VULKAN_BACKEND_ENABLE == 1
			VkDescriptorSet DescriptorSets[1] = { EVulkanDescriptor::GetGrassTextureDescriptorSet() };
			EVulkanPipeline::BindDescriptorSets(VK_PIPELINE_BIND_POINT_GRAPHICS, EVulkanPipeline::GetPipelineLayout(EUniformLayout::eGrass), EVulkan::VULKAN_DESCRIPTORSET_GRASSTEXTURE, mu_countof(DescriptorSets), DescriptorSets);
			EVulkan::MarkBusyMask(GrassTexture->TextureInternal.BusyMask, EVulkan::CurrentCommandBufferIndex);
#endif
		}
	}

	template<EVideoBackendType backendType>
	void BindGraphicsGrassBuffers(EBufferBackend *InstanceBuffer)
	{
		if constexpr (backendType == EVideoBackendType::eDirectX)
		{
#if DIRECTX_BACKEND_ENABLE == 1
			ID3D11Buffer *buffers[] = { InstanceBuffer->DirectXInternal.Resource };
			mu_uint32 buffersStride[] = { sizeof(SHGrassInstance) };
			mu_uint32 buffersOffset[] = { 0 };

			EDirectXPipeline::SetVertexBuffers(0, mu_countof(buffers), buffers, buffersStride, buffersOffset);
			EDirectXPipeline::SetIndexBuffer(EDirectXBuffer::GetDummyIndexResource(), DXGI_FORMAT_R16_UINT, 0);
#endif
		}
		else if constexpr (backendType == EVideoBackendType::eOpenGL)
		{
#if OPENGL_BACKEND_ENABLE == 1
			EOpenGLPipeline::BindVAO(EOpenGLBuffer::RetrieveGrassVAO());
#endif
		}
		else if constexpr (backendType == EVideoBackendType::eVulkan)
		{
#if VULKAN_BACKEND_ENABLE == 1
			EVulkanPipeline::BindIndexBuffer(EVulkanBuffer::DummyIndexBuffer.VulkanInternal.Buffer, VK_INDEX_TYPE_UINT16);
			EVulkanPipeline::BindVertexBuffers(0, 1, &InstanceBuffer->VulkanInternal.Buffer);
			EVulkan::MarkBusyMask(InstanceBuffer->BufferInternal.BusyMask, EVulkan::CurrentCommandBufferIndex);
#endif
		}
	}

	template<EVideoBackendType backendType>
	void UnbindGraphicsGrassBuffers()
	{
		if constexpr (backendType == EVideoBackendType::eDirectX)
		{
#if DIRECTX_BACKEND_ENABLE == 1
#endif
		}
		else if constexpr (backendType == EVideoBackendType::eOpenGL)
		{
#if OPENGL_BACKEND_ENABLE == 1
			EOpenGLPipeline::BindVAO(EInvalidUInt32);
#endif
		}
		else if constexpr (backendType == EVideoBackendType::eVulkan)
		{
#if VULKAN_BACKEND_ENABLE == 1
#endif
		}
	}

	template<EVideoBackendType backendType>
	void BindGraphicsShadowMap(const EUniformLayout Layout)
	{
		if constexpr (backendType == EVideoBackendType::eDirectX)
		{
#if DIRECTX_BACKEND_ENABLE == 1
			MUTexture *ShadowTexture = EDirectXBuffer::GetShadowMapTexture(EDirectX::CurrentCommandBufferIndex);
			const MUTexture *textures[] = { ShadowTexture };
			const ESampler samplers[] = { ESampler::eShadowPCF };

			EDirectXPipeline::SetFragmentTexture(2, mu_countof(textures), textures);
			EDirectXPipeline::SetFragmentSampler(2, mu_countof(samplers), samplers);
#endif
		}
		else if constexpr (backendType == EVideoBackendType::eOpenGL)
		{
#if OPENGL_BACKEND_ENABLE == 1
			const MUTexture *ShadowTexture = EOpenGLBuffer::GetShadowMapTexture(EOpenGL::CurrentCommandBufferIndex);
			const MUTexture *textures[] = { ShadowTexture };
			const ESampler samplers[] = { ESampler::eShadowPCF };
			const mu_boolean samplersMipmap[] = { false };

			EOpenGLPipeline::SetFragmentTexture(2, mu_countof(textures), textures);
			EOpenGLPipeline::SetFragmentSampler(2, mu_countof(samplers), samplers, samplersMipmap);
#endif
		}
		else if constexpr (backendType == EVideoBackendType::eVulkan)
		{
#if VULKAN_BACKEND_ENABLE == 1
			VkDescriptorSet DescriptorSets[1] = { EVulkanBuffer::GetShadowMapDescriptorSet(EVulkan::CurrentCommandBufferIndex) };
			EVulkanPipeline::BindDescriptorSets(VK_PIPELINE_BIND_POINT_GRAPHICS, EVulkanPipeline::GetPipelineLayout(Layout), EVulkan::VULKAN_DESCRIPTORSET_SHADOWMAP, mu_countof(DescriptorSets), DescriptorSets);
#endif
		}
	}

	template<EVideoBackendType backendType>
	void BindGraphicsModelTexture(MUTexture *ModelTexture)
	{
		if constexpr (backendType == EVideoBackendType::eDirectX)
		{
#if DIRECTX_BACKEND_ENABLE == 1
			const MUTexture *textures[1] = { ModelTexture };
			const ESampler samplers[1] = { ModelTexture->TextureInternal.Sampler };

			EDirectXPipeline::SetFragmentTexture(0, mu_countof(textures), textures);
			EDirectXPipeline::SetFragmentSampler(0, mu_countof(samplers), samplers);
#endif
		}
		else if constexpr (backendType == EVideoBackendType::eOpenGL)
		{
#if OPENGL_BACKEND_ENABLE == 1
			const MUTexture *textures[1] = { ModelTexture };
			const ESampler samplers[1] = { ModelTexture->TextureInternal.Sampler };
			const mu_boolean samplersMipmap[1] = { true };

			EOpenGLPipeline::SetFragmentTexture(0, mu_countof(textures), textures);
			EOpenGLPipeline::SetFragmentSampler(0, mu_countof(samplers), samplers, samplersMipmap);
#endif
		}
		else if constexpr (backendType == EVideoBackendType::eVulkan)
		{
#if VULKAN_BACKEND_ENABLE == 1
			VkDescriptorSet DescriptorSets[1] = { ModelTexture->VulkanInternal.DescriptorSet };
			EVulkanPipeline::BindDescriptorSets(VK_PIPELINE_BIND_POINT_GRAPHICS, EVulkanPipeline::GetPipelineLayout(EUniformLayout::eModel), EVulkan::VULKAN_DESCRIPTORSET_MODELTEXTURE, mu_countof(DescriptorSets), DescriptorSets);
			EVulkan::MarkBusyMask(ModelTexture->TextureInternal.BusyMask, EVulkan::CurrentCommandBufferIndex);
#endif
		}
	}

	template<EVideoBackendType backendType>
	void BindGraphicsModelInstancesBuffer()
	{
		if constexpr (backendType == EVideoBackendType::eDirectX)
		{
#if DIRECTX_BACKEND_ENABLE == 1
			mu_uint32 instanceStride[] = { sizeof(SHModelInstance) };
			mu_uint32 instanceOffset[] = { 0 };
			ID3D11Buffer *instanceBuffer[] = { EDirectXBuffer::GetModelInstancesResource() };

			EDirectXPipeline::SetVertexBuffers(1, 1, instanceBuffer, instanceStride, instanceOffset);
#endif
		}
		else if constexpr (backendType == EVideoBackendType::eOpenGL)
		{
#if OPENGL_BACKEND_ENABLE == 1
#endif
		}
		else if constexpr (backendType == EVideoBackendType::eVulkan)
		{
#if VULKAN_BACKEND_ENABLE == 1
			const mu_uint32 index = EVulkan::CurrentCommandBufferIndex;
			EVulkanPipeline::BindVertexBuffers(1, 1, &EVulkanBuffer::ModelInstanceBuffer[index].VulkanInternal.Buffer);
#endif
		}
	}

	template<EVideoBackendType backendType>
	void BindGraphicsModelTerrainData(const EUniformLayout Layout, MUTexture *AlphaTexture, MUTexture *HeightTexture, MUTexture *AttributeTexture)
	{
		if constexpr (backendType == EVideoBackendType::eDirectX)
		{
#if DIRECTX_BACKEND_ENABLE == 1
			const MUTexture *textures[] =
			{
				HeightTexture
			};
			const ESampler samplers[] =
			{
				textures[0]->TextureInternal.Sampler
			};

			EDirectXPipeline::SetVertexTexture(1, mu_countof(textures), textures);
			EDirectXPipeline::SetVertexSampler(1, mu_countof(samplers), samplers);
#endif
		}
		else if constexpr (backendType == EVideoBackendType::eOpenGL)
		{
#if OPENGL_BACKEND_ENABLE == 1
			const MUTexture *textures[] =
			{
				HeightTexture
			};
			const ESampler samplers[] =
			{
				textures[0]->TextureInternal.Sampler
			};
			const mu_boolean samplersMipmap[] = { false };

			EOpenGLPipeline::SetVertexTexture(1, mu_countof(textures), textures);
			EOpenGLPipeline::SetVertexSampler(1, mu_countof(samplers), samplers, samplersMipmap);
#endif
		}
		else if constexpr (backendType == EVideoBackendType::eVulkan)
		{
#if VULKAN_BACKEND_ENABLE == 1
			VkDescriptorSet DescriptorSets[1] = { EVulkanDescriptor::GetTerrainDataDescriptorSet() };
			EVulkanPipeline::BindDescriptorSets(VK_PIPELINE_BIND_POINT_GRAPHICS, EVulkanPipeline::GetPipelineLayout(Layout), EVulkan::VULKAN_DESCRIPTORSET_TERRAINDATA, mu_countof(DescriptorSets), DescriptorSets);
			EVulkan::MarkBusyMask(AlphaTexture->TextureInternal.BusyMask, EVulkan::CurrentCommandBufferIndex);
			EVulkan::MarkBusyMask(HeightTexture->TextureInternal.BusyMask, EVulkan::CurrentCommandBufferIndex);
			EVulkan::MarkBusyMask(AttributeTexture->TextureInternal.BusyMask, EVulkan::CurrentCommandBufferIndex);
#endif
		}
	}

	template<EVideoBackendType backendType>
	void BindGraphicsClothInstancesBuffer()
	{
		if constexpr (backendType == EVideoBackendType::eDirectX)
		{
#if DIRECTX_BACKEND_ENABLE == 1
			mu_uint32 instanceStride[] = { sizeof(SHClothVertexData), sizeof(SHModelInstance) };
			mu_uint32 instanceOffset[] = { 0, 0 };
			ID3D11Buffer *instanceBuffer[] = { EDirectXBuffer::GetClothInstancesResource(), EDirectXBuffer::GetModelInstancesResource() };

			EDirectXPipeline::SetVertexBuffers(0, mu_countof(instanceBuffer), instanceBuffer, instanceStride, instanceOffset);
#endif
		}
		else if constexpr (backendType == EVideoBackendType::eOpenGL)
		{
#if OPENGL_BACKEND_ENABLE == 1
			EOpenGLPipeline::BindVAO(EOpenGLBuffer::RetrieveClothVAO());
#endif
		}
		else if constexpr (backendType == EVideoBackendType::eVulkan)
		{
#if VULKAN_BACKEND_ENABLE == 1
			const mu_uint32 index = EVulkan::CurrentCommandBufferIndex;
			VkBuffer buffers[] =
			{
				EVulkanBuffer::ClothVertexBuffer[index].VulkanInternal.Buffer,
				EVulkanBuffer::ModelInstanceBuffer[index].VulkanInternal.Buffer
			};
			EVulkanPipeline::BindVertexBuffers(0, mu_countof(buffers), buffers);
#endif
		}
	}

	template<EVideoBackendType backendType>
	void BindGraphicsModelBuffers(MUModel *Model)
	{
		if constexpr(backendType == EVideoBackendType::eDirectX)
		{
#if DIRECTX_BACKEND_ENABLE == 1
			mu_uint32 bufferStride = sizeof(Model::VertexData1);
			mu_uint32 bufferOffset = 0;
			EDirectXPipeline::SetVertexBuffers(0, 1, &Model->ModelInternal.VertexBuffer.DirectXInternal.Resource, &bufferStride, &bufferOffset);

			if (Model->_VertexHeader.IndexType == MUModel::INDEX_16BIT)
				EDirectXPipeline::SetIndexBuffer(Model->ModelInternal.IndexBuffer.DirectXInternal.Resource, DXGI_FORMAT_R16_UINT, 0);
			else
				EDirectXPipeline::SetIndexBuffer(Model->ModelInternal.IndexBuffer.DirectXInternal.Resource, DXGI_FORMAT_R32_UINT, 0);
#endif
		}
		else if constexpr(backendType == EVideoBackendType::eOpenGL)
		{
#if OPENGL_BACKEND_ENABLE == 1
			EOpenGLPipeline::BindVAO(Model->OpenGLInternal.VAO);
#endif
		}
		else if constexpr(backendType == EVideoBackendType::eVulkan)
		{
#if VULKAN_BACKEND_ENABLE == 1
			EVulkanPipeline::BindVertexBuffers(0, 1, &Model->ModelInternal.VertexBuffer.VulkanInternal.Buffer);
			if (Model->_VertexHeader.IndexType == MUModel::INDEX_16BIT)
			{
				EVulkanPipeline::BindIndexBuffer(Model->ModelInternal.IndexBuffer.VulkanInternal.Buffer, VK_INDEX_TYPE_UINT16);
			}
			else
			{
				EVulkanPipeline::BindIndexBuffer(Model->ModelInternal.IndexBuffer.VulkanInternal.Buffer, VK_INDEX_TYPE_UINT32);
			}

			EVulkan::MarkBusyMask(Model->ModelInternal.VertexBuffer.BufferInternal.BusyMask, EVulkan::CurrentCommandBufferIndex);
			EVulkan::MarkBusyMask(Model->ModelInternal.IndexBuffer.BufferInternal.BusyMask, EVulkan::CurrentCommandBufferIndex);
#endif
		}
	}

	template<EVideoBackendType backendType>
	void BindGraphicsEffectTerrainData()
	{
		if constexpr (backendType == EVideoBackendType::eDirectX)
		{
#if DIRECTX_BACKEND_ENABLE == 1
			const MUTexture *textures[1] =
			{
				EDirectXBuffer::GetTerrainLightTexture(EDirectX::CurrentCommandBufferIndex)
			};
			const ESampler samplers[1] =
			{
				ESampler::eUcVcWcMinNMagNMipN
			};

			EDirectXPipeline::SetVertexTexture(0, mu_countof(textures), textures);
			EDirectXPipeline::SetVertexSampler(0, mu_countof(samplers), samplers);
#endif
		}
		else if constexpr (backendType == EVideoBackendType::eOpenGL)
		{
#if OPENGL_BACKEND_ENABLE == 1
			const MUTexture *textures[1] =
			{
				EOpenGLBuffer::GetTerrainLightTexture(EOpenGL::CurrentCommandBufferIndex)
			};
			const ESampler samplers[1] =
			{
				ESampler::eUcVcWcMinNMagNMipN
			};
			const mu_boolean samplersMipmap[4] = { false };

			EOpenGLPipeline::SetVertexTexture(0, mu_countof(textures), textures);
			EOpenGLPipeline::SetVertexSampler(0, mu_countof(samplers), samplers, samplersMipmap);
#endif
		}
		else if constexpr (backendType == EVideoBackendType::eVulkan)
		{
#if VULKAN_BACKEND_ENABLE == 1
			VkDescriptorSet DescriptorSets[1] = { EVulkanDescriptor::GetTerrainDataDescriptorSet() };
			EVulkanPipeline::BindDescriptorSets(VK_PIPELINE_BIND_POINT_GRAPHICS, EVulkanPipeline::GetPipelineLayout(EUniformLayout::eEffect), EVulkan::VULKAN_DESCRIPTORSET_TERRAINDATA, mu_countof(DescriptorSets), DescriptorSets);
#endif
		}
	}

	template<EVideoBackendType backendType>
	void BindGraphicsEffectInstancesBuffer()
	{
		if constexpr (backendType == EVideoBackendType::eDirectX)
		{
#if DIRECTX_BACKEND_ENABLE == 1
			mu_uint32 instanceStride[] = { sizeof(SHEffectInstance) };
			mu_uint32 instanceOffset[] = { 0 };
			ID3D11Buffer *instanceBuffer[] = { EDirectXBuffer::GetEffectInstancesResource() };

			EDirectXPipeline::SetVertexBuffers(0, mu_countof(instanceBuffer), instanceBuffer, instanceStride, instanceOffset);
#endif
		}
		else if constexpr (backendType == EVideoBackendType::eOpenGL)
		{
#if OPENGL_BACKEND_ENABLE == 1
			EOpenGLPipeline::BindVAO(EOpenGLBuffer::RetrieveEffectVAO());
#endif
		}
		else if constexpr (backendType == EVideoBackendType::eVulkan)
		{
#if VULKAN_BACKEND_ENABLE == 1
			const mu_uint32 index = EVulkan::CurrentCommandBufferIndex;
			EVulkanPipeline::BindVertexBuffers(0, 1, &EVulkanBuffer::EffectInstanceBuffer[index].VulkanInternal.Buffer);
#endif
		}
	}

	template<EVideoBackendType backendType>
	void BindGraphicsEffectSamplers()
	{
		if constexpr (backendType == EVideoBackendType::eDirectX)
		{
#if DIRECTX_BACKEND_ENABLE == 1
			EDirectXPipeline::SetFragmentSampler(0, mu_countof(EffectSamplers), EffectSamplers);
#endif
		}
		else if constexpr (backendType == EVideoBackendType::eOpenGL)
		{
#if OPENGL_BACKEND_ENABLE == 1
			EOpenGLPipeline::SetFragmentSampler(0, mu_countof(EffectSamplers), EffectSamplers, EffectSamplersMipmap);
#endif
		}
		else if constexpr (backendType == EVideoBackendType::eVulkan)
		{
#if VULKAN_BACKEND_ENABLE == 1
			VkDescriptorSet DescriptorSets[1] = { EVulkanDescriptor::GetEffectSamplersDescriptorSet() };
			EVulkanPipeline::BindDescriptorSets(VK_PIPELINE_BIND_POINT_GRAPHICS, EVulkanPipeline::GetPipelineLayout(EUniformLayout::eEffect), EVulkan::VULKAN_DESCRIPTORSET_EFFECTSAMPLER, mu_countof(DescriptorSets), DescriptorSets);
#endif
		}
	}

	template<EVideoBackendType backendType>
	void BindGraphicsEffectTexture(MUTexture *EffectTexture)
	{
		if constexpr (backendType == EVideoBackendType::eDirectX)
		{
#if DIRECTX_BACKEND_ENABLE == 1
			std::array<const MUTexture*, mu_countof(EffectSamplers)> textures;
			std::fill(textures.begin(), textures.end(), EffectTexture);
			EDirectXPipeline::SetFragmentTexture(0, textures.size(), textures.data());
#endif
		}
		else if constexpr (backendType == EVideoBackendType::eOpenGL)
		{
#if OPENGL_BACKEND_ENABLE == 1
			std::array<const MUTexture*, mu_countof(EffectSamplers)> textures;
			std::fill(textures.begin(), textures.end(), EffectTexture);
			EOpenGLPipeline::SetFragmentTexture(0, textures.size(), textures.data());
#endif
		}
		else if constexpr (backendType == EVideoBackendType::eVulkan)
		{
#if VULKAN_BACKEND_ENABLE == 1
			VkDescriptorSet DescriptorSets[1] = { EffectTexture->VulkanInternal.DescriptorSet };
			EVulkanPipeline::BindDescriptorSets(VK_PIPELINE_BIND_POINT_GRAPHICS, EVulkanPipeline::GetPipelineLayout(EUniformLayout::eEffect), EVulkan::VULKAN_DESCRIPTORSET_EFFECTTEXTURE, mu_countof(DescriptorSets), DescriptorSets);
			EVulkan::MarkBusyMask(EffectTexture->TextureInternal.BusyMask, EVulkan::CurrentCommandBufferIndex);
#endif
		}
	}

	void BindGraphicsFontBuffers(EBufferBackend *UniformBuffer)
	{
		const EVideoBackendType backendType = EVideoBackend::GetBackend();

		switch (backendType)
		{
#if DIRECTX_BACKEND_ENABLE == 1
		case EVideoBackendType::eDirectX:
			{
				EDirectXPipeline::SetConstantBuffers(1, 1, &UniformBuffer->DirectXInternal.Resource, D3D_CONSTANT_MASK_VS | D3D_CONSTANT_MASK_PS);
				ID3D11Buffer *buffers[] = { EDirectXBuffer::FontRetrieveCurrentBuffer() };
				mu_uint32 buffersStride[] = { sizeof(SHFontInstance) };
				mu_uint32 buffersOffset[] = { 0 };

				EDirectXPipeline::SetVertexBuffers(0, mu_countof(buffers), buffers, buffersStride, buffersOffset);
			}
			break;
#endif

#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
			{
				EOpenGLPipeline::BindVAO(EOpenGLBuffer::FontRetrieveVAO());
				glBindBufferBase(GL_UNIFORM_BUFFER, 1, UniformBuffer->OpenGLInternal.Resource);
			}
			break;
#endif

#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
			{
				VkDescriptorSet DescriptorSets[1] = { UniformBuffer->VulkanInternal.DescriptorSet };
				EVulkanPipeline::BindDescriptorSets(VK_PIPELINE_BIND_POINT_GRAPHICS, EVulkanPipeline::GetPipelineLayout(EUniformLayout::eFont), EVulkan::VULKAN_DESCRIPTORSET_FONTCONSTANT, mu_countof(DescriptorSets), DescriptorSets);
				EVulkan::MarkBusyMask(UniformBuffer->BufferInternal.BusyMask, EVulkan::CurrentCommandBufferIndex);

				VkBuffer vertexBuffer = EVulkanBuffer::FontRetrieveCurrentBuffer();
				EVulkanPipeline::BindVertexBuffers(0, 1, &vertexBuffer);
			}
			break;
#endif

		default: break;
		}
	}

	void BindGraphicsFontTexture(MUTexture *FontTexture)
	{
		const EVideoBackendType backendType = EVideoBackend::GetBackend();

		switch (backendType)
		{
#if DIRECTX_BACKEND_ENABLE == 1
		case EVideoBackendType::eDirectX:
			{
				std::array<const MUTexture*, 1> textures;
				std::fill(textures.begin(), textures.end(), FontTexture);
				EDirectXPipeline::SetFragmentTexture(0, textures.size(), textures.data());
				EDirectXPipeline::SetFragmentSampler(0, FontTexture->TextureInternal.Sampler);
			}
			break;
#endif

#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
			{
				std::array<const MUTexture*, 1> textures;
				std::fill(textures.begin(), textures.end(), FontTexture);
				EOpenGLPipeline::SetFragmentTexture(0, textures.size(), textures.data());
				EOpenGLPipeline::SetFragmentSampler(0, FontTexture->TextureInternal.Sampler, false);
			}
			break;
#endif

#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
			{
				VkDescriptorSet DescriptorSets[1] = { FontTexture->VulkanInternal.DescriptorSet };
				EVulkanPipeline::BindDescriptorSets(VK_PIPELINE_BIND_POINT_GRAPHICS, EVulkanPipeline::GetPipelineLayout(EUniformLayout::eFont), EVulkan::VULKAN_DESCRIPTORSET_FONTTEXTURE, mu_countof(DescriptorSets), DescriptorSets);
				EVulkan::MarkBusyMask(FontTexture->TextureInternal.BusyMask, EVulkan::CurrentCommandBufferIndex);
			}
			break;
#endif

		default: break;
		}
	}

	void BindGraphicsWaterVertexBuffer()
	{
		const EVideoBackendType backendType = EVideoBackend::GetBackend();

		switch (backendType)
		{
#if DIRECTX_BACKEND_ENABLE == 1
		case EVideoBackendType::eDirectX:
			{
				mu_uint32 vertexStride[] = { sizeof(SHWaterVertex) };
				mu_uint32 vertexOffset[] = { 0 };
				ID3D11Buffer *vertexBuffer[] = { EDirectXBuffer::GetWaterVertexResource() };

				EDirectXPipeline::SetVertexBuffers(0, 1, vertexBuffer, vertexStride, vertexOffset);
				EDirectXPipeline::SetIndexBuffer(EDirectXBuffer::GetWaterIndexResource(), DXGI_FORMAT_R16_UINT, 0);
			}
			break;
#endif

#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
			{
				EOpenGLPipeline::BindVAO(EOpenGLBuffer::WaterRetrieveVAO());
			}
			break;
#endif

#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
			{
				const mu_uint32 index = EVulkan::CurrentCommandBufferIndex;
				EVulkanPipeline::BindVertexBuffers(0, 1, &EVulkanBuffer::WaterVertexBuffer[index].VulkanInternal.Buffer);
				EVulkanPipeline::BindIndexBuffer(EVulkanBuffer::WaterIndexBuffer.VulkanInternal.Buffer, VK_INDEX_TYPE_UINT16);
			}
			break;
#endif

		default: break;
		}
	}

	void BindGraphicsWaterTexture()
	{
		const EVideoBackendType backendType = EVideoBackend::GetBackend();

		switch (backendType)
		{
#if DIRECTX_BACKEND_ENABLE == 1
		case EVideoBackendType::eDirectX:
			{
				const MUTexture *textures[] = { &TEXTURES::Data[BITMAP_WATER + 0], &TEXTURES::Data[BITMAP_WATER + 1] };
				const ESampler samplers[] = { ESampler::eUcVcWcMinLMagLMipL, ESampler::eUcVcWcMinLMagLMipL };
				
				EDirectXPipeline::SetFragmentTexture(0, mu_countof(textures), textures);
				EDirectXPipeline::SetFragmentSampler(0, mu_countof(samplers), samplers);
			}
			break;
#endif

#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
			{
				const MUTexture *textures[] = { &TEXTURES::Data[BITMAP_WATER + 0], &TEXTURES::Data[BITMAP_WATER + 1] };
				const ESampler samplers[] = { ESampler::eUcVcWcMinLMagLMipL, ESampler::eUcVcWcMinLMagLMipL };
				const mu_boolean samplersMipmap[] = { true, true };

				EOpenGLPipeline::SetFragmentTexture(0, mu_countof(textures), textures);
				EOpenGLPipeline::SetFragmentSampler(0, mu_countof(samplers), samplers, samplersMipmap);
			}
			break;
#endif

#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
			{
				VkDescriptorSet DescriptorSets[] = { TEXTURES::Data[BITMAP_WATER + 0].VulkanInternal.DescriptorSet, TEXTURES::Data[BITMAP_WATER + 1].VulkanInternal.DescriptorSet };
				EVulkanPipeline::BindDescriptorSets(VK_PIPELINE_BIND_POINT_GRAPHICS, EVulkanPipeline::GetPipelineLayout(EUniformLayout::eWater), EVulkan::VULKAN_DESCRIPTORSET_WATERTEXTURE, mu_countof(DescriptorSets), DescriptorSets);
				EVulkan::MarkBusyMask(TEXTURES::Data[BITMAP_WATER + 0].TextureInternal.BusyMask, EVulkan::CurrentCommandBufferIndex);
				EVulkan::MarkBusyMask(TEXTURES::Data[BITMAP_WATER + 1].TextureInternal.BusyMask, EVulkan::CurrentCommandBufferIndex);
			}
			break;
#endif

		default: break;
		}
	}
};