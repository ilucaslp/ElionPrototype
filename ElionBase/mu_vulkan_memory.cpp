#include "stdafx.h"

#define VMA_IMPLEMENTATION
#include <vk_mem_alloc.h>

namespace EVulkanMemory
{
	VmaAllocator MemoryAllocator = nullptr;

	const mu_boolean Initialize()
	{
		VmaVulkanFunctions vulkanFunctions = {};
		vulkanFunctions.vkGetPhysicalDeviceProperties = vkGetPhysicalDeviceProperties;
		vulkanFunctions.vkGetPhysicalDeviceMemoryProperties = vkGetPhysicalDeviceMemoryProperties;
		vulkanFunctions.vkAllocateMemory = vkAllocateMemory;
		vulkanFunctions.vkFreeMemory = vkFreeMemory;
		vulkanFunctions.vkMapMemory = vkMapMemory;
		vulkanFunctions.vkUnmapMemory = vkUnmapMemory;
		vulkanFunctions.vkFlushMappedMemoryRanges = vkFlushMappedMemoryRanges;
		vulkanFunctions.vkInvalidateMappedMemoryRanges = vkInvalidateMappedMemoryRanges;
		vulkanFunctions.vkBindBufferMemory = vkBindBufferMemory;
		vulkanFunctions.vkBindImageMemory = vkBindImageMemory;
		vulkanFunctions.vkGetBufferMemoryRequirements = vkGetBufferMemoryRequirements;
		vulkanFunctions.vkGetImageMemoryRequirements = vkGetImageMemoryRequirements;
		vulkanFunctions.vkCreateBuffer = vkCreateBuffer;
		vulkanFunctions.vkDestroyBuffer = vkDestroyBuffer;
		vulkanFunctions.vkCreateImage = vkCreateImage;
		vulkanFunctions.vkDestroyImage = vkDestroyImage;
#if VMA_DEDICATED_ALLOCATION
		vulkanFunctions.vkGetBufferMemoryRequirements2KHR = vkGetBufferMemoryRequirements2KHR;
		vulkanFunctions.vkGetImageMemoryRequirements2KHR = vkGetImageMemoryRequirements2KHR;
#endif

		VmaAllocatorCreateInfo allocatorInfo = {};
		allocatorInfo.physicalDevice = EVulkan::PhysicalDevice;
		allocatorInfo.device = EVulkan::Device;
		allocatorInfo.pVulkanFunctions = &vulkanFunctions;

		VkResult result = vmaCreateAllocator(&allocatorInfo, &MemoryAllocator);
		if(result != VK_SUCCESS)
		{
			return false;
		}

		return true;
	}

	void Destroy()
	{
		if(MemoryAllocator != VMA_NULL)
		{
			vmaDestroyAllocator(MemoryAllocator);
			MemoryAllocator = VMA_NULL;
		}
	}
};