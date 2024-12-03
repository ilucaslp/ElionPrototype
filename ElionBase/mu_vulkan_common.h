#ifndef __MU_VULKAN_COMMON_H__
#define __MU_VULKAN_COMMON_H__

#pragma once

namespace EVulkan
{
	extern VkPipelineInputAssemblyStateCreateInfo TopologyList[ETopologyTypeMax];
	extern VkViewport SrcViewportList[VULKAN_VIEWPORT_MAX];
	extern VkViewport ViewportList[VULKAN_VIEWPORT_MAX];
	extern VkRect2D SrcScissorList[VULKAN_VIEWPORT_MAX];
	extern VkRect2D ScissorList[VULKAN_VIEWPORT_MAX];
	extern VkPipelineRasterizationStateCreateInfo RasterizerList[ERasterizerMax];
	extern VkPipelineColorBlendAttachmentState BlendStateAttachment[EBlendStateMax];
	extern VkPipelineColorBlendStateCreateInfo BlendStateList[EBlendStateMax];
	extern VkPipelineDepthStencilStateCreateInfo DepthStencilList[EDepthStencilMax];

	void AlignAtomSize(VkDeviceSize &size);
	void AlignUniformSize(VkDeviceSize &size);
	void AlignMappedRangeAtomSize(VkDeviceSize &offset, VkDeviceSize &size, const VkDeviceSize maxSize);

	EINLINE void MarkBusyMask(mu_uint8 &busyMask, const mu_uint32 bufferIndex)
	{
		busyMask |= (1 << bufferIndex);
	}
	EINLINE void UnmarkBusyMask(mu_uint8 &busyMask, const mu_uint32 bufferIndex)
	{
		busyMask &= ~(1 << bufferIndex);
	}
};

#endif