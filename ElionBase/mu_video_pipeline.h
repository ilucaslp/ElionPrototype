#ifndef __MU_VIDEO_PIPELINE_H__
#define __MU_VIDEO_PIPELINE_H__

#pragma once

class MUModel;

struct ETextureBind
{
	const MUTexture *Texture;
	const ESampler Sampler;
};

namespace EVideoPipeline
{
	EINLINE void UnbindVAO()
	{
		const EVideoBackendType backendType = EVideoBackend::GetBackend();

#if OPENGL_BACKEND_ENABLE == 1
		if (backendType == EVideoBackendType::eOpenGL)
		{
			EOpenGLPipeline::BindVAO(EInvalidUInt32);
		}
#endif
	}

	EINLINE void SetGraphicsPipelineAtRuntime(const ECombinedShader Program,
		const ERasterizer Rasterizer,
		const EDepthStencil DepthStencil,
		const EBlendState BlendState,
		const mu_boolean EnableScissor)
	{
		const EVideoBackendType backendType = EVideoBackend::GetBackend();

		switch(backendType)
		{
#if DIRECTX_BACKEND_ENABLE == 1
		case EVideoBackendType::eDirectX:
			{
				EDirectXShader::SetProgramShader(Program);
				EDirectXPipeline::SetRasterizer(Rasterizer, EnableScissor);
				EDirectXPipeline::SetDepthStencil(DepthStencil);
				EDirectXPipeline::SetBlendState(BlendState);
			}
			break;
#endif
		
#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
			{
				EOpenGLShader::SetProgramShader(Program);
				EOpenGLPipeline::SetRasterizer(Rasterizer, EnableScissor);
				EOpenGLPipeline::SetDepthStencil(DepthStencil);
				EOpenGLPipeline::SetBlendState(BlendState);
			}
			break;
#endif

#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
			{
				EVulkanPipeline::SetPipeline(VK_PIPELINE_BIND_POINT_GRAPHICS, Program, Rasterizer, DepthStencil, BlendState);
				EVulkanPipeline::SetScissor(EnableScissor);
			}
			break;
#endif

		default: break;
		}
	}

	template<EVideoBackendType backendType>
	EINLINE void SetGraphicsPipeline(const ECombinedShader Program,
		const ERasterizer Rasterizer,
		const EDepthStencil DepthStencil,
		const EBlendState BlendState,
		const mu_boolean EnableScissor)
	{
		if constexpr (backendType == EVideoBackendType::eDirectX)
		{
#if DIRECTX_BACKEND_ENABLE == 1
			EDirectXShader::SetProgramShader(Program);
			EDirectXPipeline::SetRasterizer(Rasterizer, EnableScissor);
			EDirectXPipeline::SetDepthStencil(DepthStencil);
			EDirectXPipeline::SetBlendState(BlendState);
#endif
		}
		else if constexpr (backendType == EVideoBackendType::eOpenGL)
		{
#if OPENGL_BACKEND_ENABLE == 1
			EOpenGLShader::SetProgramShader(Program);
			EOpenGLPipeline::SetRasterizer(Rasterizer, EnableScissor);
			EOpenGLPipeline::SetDepthStencil(DepthStencil);
			EOpenGLPipeline::SetBlendState(BlendState);
#endif
		}
		else if constexpr (backendType == EVideoBackendType::eVulkan)
		{
#if VULKAN_BACKEND_ENABLE == 1
			EVulkanPipeline::SetPipeline(VK_PIPELINE_BIND_POINT_GRAPHICS, Program, Rasterizer, DepthStencil, BlendState);
			EVulkanPipeline::SetScissor(EnableScissor);
#endif
		}
	}

	void BindGraphicsUITextureAtRuntime(const ETextureBind Binds);

	EINLINE void BindGraphicsUIAtRuntime()
	{
		const EVideoBackendType backendType = EVideoBackend::GetBackend();

		switch (backendType)
		{
#if DIRECTX_BACKEND_ENABLE == 1
		case EVideoBackendType::eDirectX:
			{
				mu_uint32 instanceStride[] = { sizeof(SHUIDefaultVertex) };
				mu_uint32 instanceOffset[] = { 0 };
				ID3D11Buffer *instanceBuffer[] = { EDirectXBuffer::UIRetrieveCurrentBuffer() };

				EDirectXPipeline::SetVertexBuffers(0, 1, instanceBuffer, instanceStride, instanceOffset);
			}
			break;
#endif

#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
			{
				EOpenGLPipeline::BindVAO(EOpenGLBuffer::UIRetrieveVAO());
			}
			break;
#endif

#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
			{
				VkBuffer vertexBuffer = EVulkanBuffer::UIRetrieveCurrentBuffer();
				EVulkanPipeline::BindVertexBuffers(0, 1, &vertexBuffer);
			}
			break;
#endif

		default: break;
		}
	}

	template<EVideoBackendType backendType>
	void BindGraphicsBonesTexture();

	template<EVideoBackendType backendType>
	void BindGraphicsTerrainTexture(MUTexture *TilesTexture, const mu_uint32 WaterIndex);

	template<EVideoBackendType backendType>
	void BindGraphicsTerrainData(const EUniformLayout Layout, MUTexture *AlphaTexture, MUTexture *HeightTexture, MUTexture *AttributeTexture);

	template<EVideoBackendType backendType>
	void BindGraphicsTerrainBuffers(EBufferBackend *InstanceBuffer);

	template<EVideoBackendType backendType>
	void UnbindGraphicsTerrainBuffers();

	template<EVideoBackendType backendType>
	void BindGraphicsGrassTexture(MUTexture *GrassTexture);

	template<EVideoBackendType backendType>
	void BindGraphicsGrassBuffers(EBufferBackend *InstanceBuffer);

	template<EVideoBackendType backendType>
	void UnbindGraphicsGrassBuffers();

	template<EVideoBackendType backendType>
	void BindGraphicsShadowMap(const EUniformLayout Layout);

	template<EVideoBackendType backendType>
	void BindGraphicsModelTexture(MUTexture *ModelTexture);

	template<EVideoBackendType backendType>
	void BindGraphicsModelInstancesBuffer();

	template<EVideoBackendType backendType>
	void BindGraphicsModelTerrainData(const EUniformLayout Layout, MUTexture *AlphaTexture, MUTexture *HeightTexture, MUTexture *AttributeTexture);

	template<EVideoBackendType backendType>
	void BindGraphicsClothInstancesBuffer();

	template<EVideoBackendType backendType>
	void BindGraphicsModelBuffers(MUModel *Model);

	template<EVideoBackendType backendType>
	void BindGraphicsEffectTerrainData();

	template<EVideoBackendType backendType>
	void BindGraphicsEffectInstancesBuffer();

	template<EVideoBackendType backendType>
	void BindGraphicsEffectSamplers();

	template<EVideoBackendType backendType>
	void BindGraphicsEffectTexture(MUTexture *EffectTexture);

	void BindGraphicsFontBuffers(EBufferBackend *UniformBuffer);
	void BindGraphicsFontTexture(MUTexture *FontTexture);

	void BindGraphicsWaterVertexBuffer();
	void BindGraphicsWaterTexture();

	EINLINE void DrawAtRuntime(const mu_uint32 Index, const mu_uint32 Count, const ETopologyType Topology)
	{
		const EVideoBackendType backendType = EVideoBackend::GetBackend();

		switch (backendType)
		{
#if DIRECTX_BACKEND_ENABLE == 1
		case EVideoBackendType::eDirectX:
			{
				EDirectXInit::DeviceContext->Draw(Count, Index);
			}
			break;
#endif

#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
			{
				glDrawArrays(EOpenGL::RetrieveTopology(Topology), Index, Count);
			}
			break;
#endif

#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
			{
				vkCmdDraw(EVulkan::CurrentCommandBuffer, Count, 1, Index, 0);
			}
			break;
#endif

		default: break;
		}
	}

	template<const mu_boolean useBaseInstance, const EVideoBackendType backendType>
	EINLINE void DrawInstanced(const mu_uint32 BaseVertex, const mu_uint32 VertexCount, const mu_uint32 BaseInstance, const mu_uint32 InstanceCount, const ETopologyType Topology, const EInputLayout Layout)
	{
		if constexpr (backendType == EVideoBackendType::eDirectX)
		{
#if DIRECTX_BACKEND_ENABLE == 1
			EDirectXInit::DeviceContext->DrawInstanced(VertexCount, InstanceCount, BaseVertex, BaseInstance);
#endif
		}
		else if constexpr (backendType == EVideoBackendType::eOpenGL)
		{
#if OPENGL_BACKEND_ENABLE == 1
			if (useBaseInstance == true)
			{
				glDrawArraysInstancedBaseInstance(EOpenGL::RetrieveTopology(Topology), BaseVertex, VertexCount, InstanceCount, BaseInstance);
			}
			else
			{
				EVideoAttributes::ConfigureInstanceAtRuntime<EVideoBackendType::eOpenGL>(Layout, BaseInstance);
				glDrawArraysInstanced(EOpenGL::RetrieveTopology(Topology), BaseVertex, VertexCount, InstanceCount);
			}
#endif
		}
		else if constexpr (backendType == EVideoBackendType::eVulkan)
		{
#if VULKAN_BACKEND_ENABLE == 1
			vkCmdDraw(EVulkan::CurrentCommandBuffer, VertexCount, InstanceCount, BaseVertex, BaseInstance);
#endif
		}
	}

	EINLINE void DrawInstancedAtRuntime(const mu_uint32 BaseVertex, const mu_uint32 VertexCount, const mu_uint32 BaseInstance, const mu_uint32 InstanceCount, const ETopologyType Topology, const EInputLayout Layout)
	{
		const EVideoBackendType backendType = EVideoBackend::GetBackend();

		switch (backendType)
		{
#if DIRECTX_BACKEND_ENABLE == 1
		case EVideoBackendType::eDirectX:
			{
				EDirectXInit::DeviceContext->DrawInstanced(VertexCount, InstanceCount, BaseVertex, BaseInstance);
			}
			break;
#endif
#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
			{
				const mu_boolean useBaseInstance = EOpenGLExtensions::IsFeatureSupported(OpenGLFeatures::BaseInstance);
				if (useBaseInstance == true)
				{
					glDrawArraysInstancedBaseInstance(EOpenGL::RetrieveTopology(Topology), BaseVertex, VertexCount, InstanceCount, BaseInstance);
				}
				else
				{
					EVideoAttributes::ConfigureInstanceAtRuntime<EVideoBackendType::eOpenGL>(Layout, BaseInstance);
					glDrawArraysInstanced(EOpenGL::RetrieveTopology(Topology), BaseVertex, VertexCount, InstanceCount);
				}
			}
			break;
#endif
#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
			{
				vkCmdDraw(EVulkan::CurrentCommandBuffer, VertexCount, InstanceCount, BaseVertex, BaseInstance);
			}
			break;
#endif

		default: break;
		}
	}

	template<const EVideoBackendType backendType>
	EINLINE void DrawIndexed(const mu_uint32 BaseIndex, const mu_uint32 IndexCount, const ETopologyType Topology, const EIndexType IndexType, const EInputLayout Layout)
	{
		if constexpr (backendType == EVideoBackendType::eDirectX)
		{
#if DIRECTX_BACKEND_ENABLE == 1
			EDirectXInit::DeviceContext->DrawIndexed(IndexCount, BaseIndex, 0);
#endif
		}
		else if constexpr (backendType == EVideoBackendType::eOpenGL)
		{
#if OPENGL_BACKEND_ENABLE == 1
			const mu_size indexSize = EOpenGL::RetrieveIndexTypeSize(IndexType);

			glDrawElements(EOpenGL::RetrieveTopology(Topology), IndexCount, EOpenGL::RetrieveIndexType(IndexType), (void*)(BaseIndex * indexSize));
#endif
		}
		else if constexpr (backendType == EVideoBackendType::eVulkan)
		{
#if VULKAN_BACKEND_ENABLE == 1
			vkCmdDrawIndexed(EVulkan::CurrentCommandBuffer, IndexCount, 1, BaseIndex, 0, 0);
#endif
		}
	}

	EINLINE void DrawIndexedAtRuntime(const mu_uint32 BaseIndex, const mu_uint32 IndexCount, const ETopologyType Topology, const EIndexType IndexType, const EInputLayout Layout)
	{
		const EVideoBackendType backendType = EVideoBackend::GetBackend();

		switch (backendType)
		{
#if DIRECTX_BACKEND_ENABLE == 1
		case EVideoBackendType::eDirectX:
			{
				EDirectXInit::DeviceContext->DrawIndexed(IndexCount, BaseIndex, 0);
			}
			break;
#endif
#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
			{
				const mu_size indexSize = EOpenGL::RetrieveIndexTypeSize(IndexType);

				glDrawElements(EOpenGL::RetrieveTopology(Topology), IndexCount, EOpenGL::RetrieveIndexType(IndexType), (void*)(BaseIndex * indexSize));
			}
			break;
#endif
#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
			{
				vkCmdDrawIndexed(EVulkan::CurrentCommandBuffer, IndexCount, 1, BaseIndex, 0, 0);
			}
			break;
#endif
		default: break;
		}
	}

	template<const mu_boolean useBaseInstance, const EVideoBackendType backendType>
	EINLINE void DrawIndexedInstanced(const mu_uint32 BaseIndex, const mu_uint32 IndexCount, const mu_uint32 BaseInstance, const mu_uint32 InstanceCount, const ETopologyType Topology, const EIndexType IndexType, const EInputLayout Layout)
	{
		if constexpr (backendType == EVideoBackendType::eDirectX)
		{
#if DIRECTX_BACKEND_ENABLE == 1
			EDirectXInit::DeviceContext->DrawIndexedInstanced(IndexCount, InstanceCount, BaseIndex, 0, BaseInstance);
#endif
		}
		else if constexpr (backendType == EVideoBackendType::eOpenGL)
		{
#if OPENGL_BACKEND_ENABLE == 1
			const mu_size indexSize = EOpenGL::RetrieveIndexTypeSize(IndexType);

			if constexpr (useBaseInstance == true)
			{
				glDrawElementsInstancedBaseInstance(EOpenGL::RetrieveTopology(Topology), IndexCount, EOpenGL::RetrieveIndexType(IndexType), (void*)(BaseIndex * indexSize), InstanceCount, BaseInstance);
			}
			else
			{
				EVideoAttributes::ConfigureInstanceAtRuntime<EVideoBackendType::eOpenGL>(Layout, BaseInstance);
				glDrawElementsInstanced(EOpenGL::RetrieveTopology(Topology), IndexCount, EOpenGL::RetrieveIndexType(IndexType), (void*)(BaseIndex * indexSize), InstanceCount);
			}
#endif
		}
		else if constexpr (backendType == EVideoBackendType::eVulkan)
		{
#if VULKAN_BACKEND_ENABLE == 1
			vkCmdDrawIndexed(EVulkan::CurrentCommandBuffer, IndexCount, InstanceCount, BaseIndex, 0, BaseInstance);
#endif
		}
	}
};

#endif