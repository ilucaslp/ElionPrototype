#ifndef __MU_VULKAN_PIPELINE_H__
#define __MU_VULKAN_PIPELINE_H__

#pragma once

namespace EVulkanPipeline
{
	constexpr mu_uint32 BlendStateBits = ComputeBitsNeeded(EBlendStateMax);
	constexpr mu_uint32 DepthStencilBits = ComputeBitsNeeded(EDepthStencilMax);
	constexpr mu_uint32 RasterizerBits = ComputeBitsNeeded(ERasterizerMax);
	constexpr mu_uint32 ProgramBits = ComputeBitsNeeded(ECombinedShaderMax);
	constexpr mu_uint32 TotalPipelineBits = ProgramBits + RasterizerBits + DepthStencilBits + BlendStateBits;

#if (TotalPipelineBits > 64) // Too big
#error "Too many bits for pipeline index"
#elif (TotalPipelineBits > 32)
	typedef mu_uint64 mu_pipelinetype;
#else
	typedef mu_uint32 mu_pipelinetype;
#endif

	constexpr mu_pipelinetype BlendStateLeft = 0;
	constexpr mu_pipelinetype DepthStencilLeft = BlendStateLeft + BlendStateBits;
	constexpr mu_pipelinetype RasterizerLeft = DepthStencilLeft + DepthStencilBits;
	constexpr mu_pipelinetype ProgramLeft = RasterizerLeft + RasterizerBits;

	constexpr mu_uint32 GRAPHICS_PIPELINE_VARIATIONS =
		ERasterizerMax *
		EDepthStencilMax *
		EBlendStateMax;
	constexpr mu_uint32 MAX_GRAPHICS_PIPELINE =
		ECombinedShaderMax *
		GRAPHICS_PIPELINE_VARIATIONS;

	EINLINE const mu_pipelinetype CreatePipelineIndex(const ECombinedShader Program,
		const ERasterizer Rasterizer,
		const EDepthStencil DepthStencil,
		const EBlendState BlendState)
	{
		constexpr mu_uint32 CountLayer1 = ERasterizerMax * EDepthStencilMax * EBlendStateMax;
		constexpr mu_uint32 CountLayer2 = EDepthStencilMax * EBlendStateMax;
		constexpr mu_uint32 CountLayer3 = EBlendStateMax;

		return ECastEnum(Program) * CountLayer1 +
			ECastEnum(Rasterizer) * CountLayer2 +
			ECastEnum(DepthStencil) * CountLayer3 +
			ECastEnum(BlendState);
	}

	extern std::array<VkDescriptorSetLayout, EVulkan::VULKAN_SETLAYOUT_MAX> DescriptorSetLayout;
	extern std::array<VkPipelineLayout, EUniformLayoutMax> PipelinesLayout;
	extern std::array<VkPipeline, MAX_GRAPHICS_PIPELINE> Pipelines;

	mu_boolean CreateRenderPasses();
	void DestroyRenderPasses();
	VkRenderPass GetRenderPass(const ERenderPass index);

	mu_boolean CreatePipelineCache();
	void DestroyPipelineCache();

	mu_boolean CreatePipelineLayouts();
	void DestroyPipelineLayouts();
	VkPipelineLayout GetPipelineLayout(const EUniformLayout index);
	VkDescriptorSetLayout GetSetLayout(const mu_uint32 index);

	mu_boolean CreatePipelines(const ECombinedShader begin, const ECombinedShader end);
	void DestroyPipelines();

	void ClearPipeline();
	void SetPipeline(const VkPipelineBindPoint Bindpoint,
		const ECombinedShader Program,
		const ERasterizer Rasterizer,
		const EDepthStencil DepthStencil,
		const EBlendState BlendState);

	void SetScissor(const mu_boolean Enable);

	void ClearBindBuffers();
	void BindIndexBuffer(const VkBuffer buffer, VkIndexType indexType);
	void BindVertexBuffers(const mu_uint32 index, const mu_uint32 count, const VkBuffer *buffers);
	void BindDescriptorSets(const VkPipelineBindPoint pipelineBindpoint, const VkPipelineLayout layout, const mu_uint32 index, const mu_uint32 count, const VkDescriptorSet *descriptors);
};

#endif