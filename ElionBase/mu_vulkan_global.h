#ifndef __MU_VULKAN_GLOBAL_H__
#define __MU_VULKAN_GLOBAL_H__

#pragma once

#define VK_INFINITE_FENCE ((uint64_t)-1)
#define VULKAN_INVALID_MEMORY_TYPE ((mu_uint32)-1)

namespace EVulkan
{
	// Global Variables
	extern SDL_vulkanInstance Instance;
	extern VkDebugReportCallbackEXT DebugCallback;
	extern SDL_vulkanSurface Surface;
	extern std::vector<VkSurfaceFormatKHR> SurfaceFormats;
	extern VkSurfaceFormatKHR SurfaceFormat;
	extern VkFormat DepthStencilFormat;
	extern VkSurfaceCapabilitiesKHR SurfaceCapabilities;
	extern mu_uint32 SurfaceFormatsCount;
	extern VkPhysicalDeviceMemoryProperties DeviceMemoryProperties;
	extern VkPhysicalDeviceProperties DeviceProperties;
	extern VkPhysicalDeviceFeatures DeviceFeatures;
	extern VkPhysicalDevice PhysicalDevice;
	extern VkDevice Device;
	extern VkQueue GraphicsQueue;
	extern VkQueue PresentQueue;
	extern mu_uint32 GraphicsQueueFamilyIndex;
	extern mu_uint32 PresentQueueFamilyIndex;
	extern mu_uint32 SwapchainDesiredImageCount;
	extern VkSampleCountFlags DesiredSamplesCount;
	extern VkExtent2D SwapchainSize;
	extern VkSwapchainKHR Swapchain;
	extern mu_uint32 SwapchainImagesCount;
	extern std::vector<VkImage> SwapchainImages;
	extern std::vector<VkImageView> SwapchainImagesView;
	extern VkImage ColorImage;
	extern VkImageView ColorImageView;
	extern VmaAllocation ColorMemory;
	extern VkImage DepthStencilImage;
	extern VkImageView DepthStencilImageView;
	extern VmaAllocation DepthStencilMemory;
	extern std::vector<VkFramebuffer> Framebuffers;
	extern VkCommandPool MainCommandPool;
	extern VkCommandPool SecondaryCommandPool;
	extern VkCommandPool ShortCommandPool;
	extern std::vector<VkCommandBuffer> CommandBuffers;
	extern std::vector<VkCommandBuffer> TransferBuffers;
	extern std::vector<VkCommandBuffer> ShadowMapBuffers;
	extern VkCommandBuffer CurrentCommandBuffer;
	extern std::vector<VkFence> CommandFences;
	extern VkSemaphore ImageAvailableSemaphore;
	extern VkSemaphore RenderingFinishedSemaphore;
	extern VkSemaphore TransferFinishedSemaphore;
	extern VkSemaphore ShadowMapFinishedSemaphore;
	extern mu_boolean SkeletonTexelBufferEnabled;
	
	extern mu_uint32 CurrentCommandBufferIndex;
	extern mu_uint32 CurrentTransferCommandBufferIndex;
	extern mu_uint32 CurrentSwapchainIndex;
	extern mu_uint32 LastSwapchainIndex;

	template<class T>
	EINLINE T GetGlobalProcessAddress(const mu_char *name)
	{
		T addr = (T)vkGetInstanceProcAddr(nullptr, name);
		if (addr == nullptr)
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "GetGlobalProcessAddress(\"%s\") failed\n", name);
		}
		return addr;
	}

	template<class T>
	EINLINE T GetInstanceProcessAddress(const mu_char *name)
	{
		T addr = (T)vkGetInstanceProcAddr(Instance, name);
		if(addr == nullptr)
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "GetInstanceProcessAddress(\"%s\") failed\n", name);
		}
		return addr;
	}

	template<class T>
	EINLINE T GetDeviceProcessAddress(const mu_char *name)
	{
		T addr = (T)vkGetDeviceProcAddr(Device, name);
		if (addr == nullptr)
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "GetDeviceProcessAddress(\"%s\") failed\n", name);
		}
		return addr;
	}

	EINLINE mu_boolean CreateFence(VkFence &Fence)
	{
		VkFenceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;

		VkResult result = vkCreateFence(Device, &createInfo, nullptr, &Fence);
		if(result != VK_SUCCESS)
		{
			return false;
		}

		return true;
	}

	EINLINE VkCommandBuffer CreateCommandBuffer(VkCommandBufferLevel level, VkCommandPool commandPool, const mu_boolean begin = false)
	{
		VkCommandBufferAllocateInfo cmdBufAllocateInfo = EVulkanInitializer::CommandBufferAllocateInfo(commandPool, level, 1);

		VkCommandBuffer cmdBuffer;
		VkResult result = vkAllocateCommandBuffers(Device, &cmdBufAllocateInfo, &cmdBuffer);
		if (result != VK_SUCCESS)
		{
			return nullptr;
		}

		// If requested, also start recording for the new command buffer
		if (begin)
		{
			VkCommandBufferBeginInfo cmdBufInfo = EVulkanInitializer::CommandBufferBeginInfo();
			VkResult result = vkBeginCommandBuffer(cmdBuffer, &cmdBufInfo);
			if (result != VK_SUCCESS)
			{
				vkFreeCommandBuffers(Device, commandPool, 1, &cmdBuffer);
				cmdBuffer = nullptr;
			}
		}

		return cmdBuffer;
	}

	template<mu_boolean FreeCommandBuffer>
	EINLINE mu_boolean FlushCommandBuffer(VkCommandBuffer commandBuffer, VkCommandPool commandPool, VkQueue queue)
	{
		if (commandBuffer == VK_NULL_HANDLE)
		{
			return false;
		}

		VkResult result = vkEndCommandBuffer(commandBuffer);
		if (result != VK_SUCCESS)
		{
			if constexpr (FreeCommandBuffer)
			{
				vkFreeCommandBuffers(Device, commandPool, 1, &commandBuffer);
			}
			return false;
		}

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		VkFence copyFence = VK_NULL_HANDLE;
		if(CreateFence(copyFence) == false)
		{
			if constexpr (FreeCommandBuffer)
			{
				vkFreeCommandBuffers(Device, commandPool, 1, &commandBuffer);
			}
			return false;
		}

		result = vkQueueSubmit(queue, 1, &submitInfo, copyFence);
		if (result != VK_SUCCESS)
		{
			vkDestroyFence(Device, copyFence, nullptr);

			if constexpr (FreeCommandBuffer)
			{
				vkFreeCommandBuffers(Device, commandPool, 1, &commandBuffer);
			}
			return false;
		}

		result = vkWaitForFences(Device, 1, &copyFence, VK_TRUE, VK_INFINITE_FENCE);
		vkDestroyFence(Device, copyFence, nullptr);

		if (result != VK_SUCCESS &&
			result != VK_TIMEOUT)
		{
			if constexpr (FreeCommandBuffer)
			{
				vkFreeCommandBuffers(Device, commandPool, 1, &commandBuffer);
			}
			return false;
		}

		if constexpr (FreeCommandBuffer)
		{
			vkFreeCommandBuffers(Device, commandPool, 1, &commandBuffer);
		}

		return true;
	}

	EINLINE mu_uint32 GetMemoryType(const mu_uint32 typeBits, const VkMemoryPropertyFlags properties)
	{
		for (mu_uint32 n = 0; n < DeviceMemoryProperties.memoryTypeCount; ++n)
		{
			if ((typeBits >> n) & 1)
			{
				if ((DeviceMemoryProperties.memoryTypes[n].propertyFlags & properties) == properties)
				{
					return n;
				}
			}
		}

		return VULKAN_INVALID_MEMORY_TYPE;
	}

	EINLINE void SetImageLayout(VkCommandBuffer cmdbuffer,
		VkImage image,
		VkImageLayout oldImageLayout,
		VkImageLayout newImageLayout,
		VkImageSubresourceRange subresourceRange,
		VkPipelineStageFlags srcStageMask = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
		VkPipelineStageFlags dstStageMask = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT)
	{
		// Create an image barrier object
		VkImageMemoryBarrier imageMemoryBarrier = {};
		imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		imageMemoryBarrier.oldLayout = oldImageLayout;
		imageMemoryBarrier.newLayout = newImageLayout;
		imageMemoryBarrier.image = image;
		imageMemoryBarrier.subresourceRange = subresourceRange;

		// Source layouts (old)
		// Source access mask controls actions that have to be finished on the old layout
		// before it will be transitioned to the new layout
		switch (oldImageLayout)
		{
		case VK_IMAGE_LAYOUT_UNDEFINED:
			// Image layout is undefined (or does not matter)
			// Only valid as initial layout
			// No flags required, listed only for completeness
			imageMemoryBarrier.srcAccessMask = 0;
			break;

		case VK_IMAGE_LAYOUT_PREINITIALIZED:
			// Image is preinitialized
			// Only valid as initial layout for linear images, preserves memory contents
			// Make sure host writes have been finished
			imageMemoryBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
			break;

		case VK_IMAGE_LAYOUT_PRESENT_SRC_KHR:
			imageMemoryBarrier.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
			break;

		case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
			// Image is a color attachment
			// Make sure any writes to the color buffer have been finished
			imageMemoryBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			break;

		case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
			// Image is a depth/stencil attachment
			// Make sure any writes to the depth/stencil buffer have been finished
			imageMemoryBarrier.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
			break;

		case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
			// Image is a transfer source 
			// Make sure any reads from the image have been finished
			imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
			break;

		case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
			// Image is a transfer destination
			// Make sure any writes to the image have been finished
			imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			break;

		case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
			// Image is read by a shader
			// Make sure any shader reads from the image have been finished
			imageMemoryBarrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
			break;
		default:
			// Other source layouts aren't handled (yet)
			break;
		}

		// Target layouts (new)
		// Destination access mask controls the dependency for the new image layout
		switch (newImageLayout)
		{
		case VK_IMAGE_LAYOUT_GENERAL:
			imageMemoryBarrier.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
			break;

		case VK_IMAGE_LAYOUT_PRESENT_SRC_KHR:
			imageMemoryBarrier.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
			break;

		case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
			// Image will be used as a transfer destination
			// Make sure any writes to the image have been finished
			imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			break;

		case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
			// Image will be used as a transfer source
			// Make sure any reads from the image have been finished
			imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
			break;

		case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
			// Image will be used as a color attachment
			// Make sure any writes to the color buffer have been finished
			imageMemoryBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			break;

		case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
			// Image layout will be used as a depth/stencil attachment
			// Make sure any writes to depth/stencil buffer have been finished
			imageMemoryBarrier.dstAccessMask = imageMemoryBarrier.dstAccessMask | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
			break;

		case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
			// Image will be read in a shader (sampler, input attachment)
			// Make sure any writes to the image have been finished
			if (imageMemoryBarrier.srcAccessMask == 0)
			{
				imageMemoryBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT | VK_ACCESS_TRANSFER_WRITE_BIT;
			}
			imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
			break;
		default:
			// Other source layouts aren't handled (yet)
			break;
		}

		// Put barrier inside setup command buffer
		vkCmdPipelineBarrier(
			cmdbuffer,
			srcStageMask,
			dstStageMask,
			0,
			0, nullptr,
			0, nullptr,
			1, &imageMemoryBarrier);
	}

	EINLINE VkResult WaitAndResetFence(VkFence Fence, const mu_uint64 Timeout = VK_INFINITE_FENCE)
	{
		VkResult fenceStatus = vkGetFenceStatus(EVulkan::Device, Fence);
		if (fenceStatus == VK_NOT_READY)
		{
			fenceStatus = vkWaitForFences(EVulkan::Device, 1, &Fence, VK_TRUE, Timeout);
		}

		if (fenceStatus == VK_SUCCESS)
		{
			fenceStatus = vkResetFences(EVulkan::Device, 1, &Fence);
		}
		else
		{
			DEBUG_BREAK();
		}

		return fenceStatus;
	}

	EINLINE void DestroyDescriptorPool(VkDescriptorPool &pool)
	{
		if(pool != VK_NULL_HANDLE)
		{
			vkDestroyDescriptorPool(Device, pool, nullptr);
			pool = VK_NULL_HANDLE;
		}
	}
};

#endif
