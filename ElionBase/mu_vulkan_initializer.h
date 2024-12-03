#ifndef __MU_VULKAN_INITIALIZER_H__
#define __MU_VULKAN_INITIALIZER_H__

#pragma once

namespace EVulkanInitializer
{
	EINLINE VkCommandBufferAllocateInfo CommandBufferAllocateInfo(VkCommandPool commandPool,
		VkCommandBufferLevel level,
		const mu_uint32 bufferCount)
	{
		VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
		commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		commandBufferAllocateInfo.commandPool = commandPool;
		commandBufferAllocateInfo.level = level;
		commandBufferAllocateInfo.commandBufferCount = bufferCount;
		return commandBufferAllocateInfo;
	}

	EINLINE VkCommandBufferBeginInfo CommandBufferBeginInfo()
	{
		VkCommandBufferBeginInfo cmdBufferBeginInfo = {};
		cmdBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		return cmdBufferBeginInfo;
	}

	EINLINE VkRenderPassBeginInfo RenderPassBeginInfo()
	{
		VkRenderPassBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		return beginInfo;
	}
};

#endif