#ifndef __MU_VULKAN_LOADER_H__
#define __MU_VULKAN_LOADER_H__

#pragma once

#define VULKAN_DEBUG_INSTANCE_FUNCTIONS()                               \
	VULKAN_INSTANCE_FUNCTION(vkCreateDebugReportCallbackEXT)            \
	VULKAN_INSTANCE_FUNCTION(vkDestroyDebugReportCallbackEXT)           \
	VULKAN_INSTANCE_FUNCTION(vkDebugReportMessageEXT)

#define VULKAN_GLOBAL_FUNCTIONS()										\
    VULKAN_GLOBAL_FUNCTION(vkCreateInstance)                            \
    VULKAN_GLOBAL_FUNCTION(vkEnumerateInstanceExtensionProperties)      \
    VULKAN_GLOBAL_FUNCTION(vkEnumerateInstanceLayerProperties)          

#define VULKAN_INSTANCE_FUNCTIONS()										\
    VULKAN_INSTANCE_FUNCTION(vkDestroyInstance)                         \
    VULKAN_INSTANCE_FUNCTION(vkCreateDevice)                            \
    VULKAN_INSTANCE_FUNCTION(vkDestroyDevice)                           \
    VULKAN_INSTANCE_FUNCTION(vkDestroySurfaceKHR)                       \
    VULKAN_INSTANCE_FUNCTION(vkEnumerateDeviceExtensionProperties)      \
    VULKAN_INSTANCE_FUNCTION(vkEnumeratePhysicalDevices)                \
    VULKAN_INSTANCE_FUNCTION(vkGetDeviceProcAddr)                       \
    VULKAN_INSTANCE_FUNCTION(vkGetPhysicalDeviceFeatures)               \
    VULKAN_INSTANCE_FUNCTION(vkGetPhysicalDeviceProperties)             \
    VULKAN_INSTANCE_FUNCTION(vkGetPhysicalDeviceQueueFamilyProperties)  \
    VULKAN_INSTANCE_FUNCTION(vkGetPhysicalDeviceSurfaceCapabilitiesKHR) \
    VULKAN_INSTANCE_FUNCTION(vkGetPhysicalDeviceSurfaceFormatsKHR)      \
    VULKAN_INSTANCE_FUNCTION(vkGetPhysicalDeviceSurfacePresentModesKHR) \
    VULKAN_INSTANCE_FUNCTION(vkGetPhysicalDeviceSurfaceSupportKHR)      \
    VULKAN_INSTANCE_FUNCTION(vkCreateShaderModule)                      \
    VULKAN_INSTANCE_FUNCTION(vkDestroyShaderModule)                     \
	VULKAN_INSTANCE_FUNCTION(vkCreateRenderPass)						\
	VULKAN_INSTANCE_FUNCTION(vkDestroyRenderPass)						\
	VULKAN_INSTANCE_FUNCTION(vkGetPhysicalDeviceFormatProperties)		\
	VULKAN_INSTANCE_FUNCTION(vkCreateDescriptorSetLayout)				\
	VULKAN_INSTANCE_FUNCTION(vkDestroyDescriptorSetLayout)				\
	VULKAN_INSTANCE_FUNCTION(vkCreatePipelineCache)						\
	VULKAN_INSTANCE_FUNCTION(vkDestroyPipelineCache)					\
	VULKAN_INSTANCE_FUNCTION(vkGetPipelineCacheData)					\
	VULKAN_INSTANCE_FUNCTION(vkCreatePipelineLayout)					\
	VULKAN_INSTANCE_FUNCTION(vkDestroyPipelineLayout)					\
	VULKAN_INSTANCE_FUNCTION(vkCreateGraphicsPipelines)					\
	VULKAN_INSTANCE_FUNCTION(vkDestroyPipeline)							\
	VULKAN_INSTANCE_FUNCTION(vkGetPhysicalDeviceMemoryProperties)       \
	VULKAN_INSTANCE_FUNCTION(vkGetBufferMemoryRequirements)             \
	VULKAN_INSTANCE_FUNCTION(vkGetBufferMemoryRequirements2KHR)			\
	VULKAN_INSTANCE_FUNCTION(vkCreateBuffer)                            \
	VULKAN_INSTANCE_FUNCTION(vkDestroyBuffer)                           \
	VULKAN_INSTANCE_FUNCTION(vkCreateBufferView)                        \
	VULKAN_INSTANCE_FUNCTION(vkDestroyBufferView)                       \
	VULKAN_INSTANCE_FUNCTION(vkCmdCopyBuffer)                           \
	VULKAN_INSTANCE_FUNCTION(vkAllocateMemory)                          \
	VULKAN_INSTANCE_FUNCTION(vkFreeMemory)                              \
	VULKAN_INSTANCE_FUNCTION(vkBindBufferMemory)                        \
	VULKAN_INSTANCE_FUNCTION(vkMapMemory)                               \
	VULKAN_INSTANCE_FUNCTION(vkUnmapMemory)                             \
	VULKAN_INSTANCE_FUNCTION(vkFlushMappedMemoryRanges)                 \
	VULKAN_INSTANCE_FUNCTION(vkInvalidateMappedMemoryRanges)			\
	VULKAN_INSTANCE_FUNCTION(vkCreateImage)                             \
	VULKAN_INSTANCE_FUNCTION(vkDestroyImage)                            \
    VULKAN_INSTANCE_FUNCTION(vkCreateImageView)                         \
    VULKAN_INSTANCE_FUNCTION(vkDestroyImageView)                        \
	VULKAN_INSTANCE_FUNCTION(vkBindImageMemory)                         \
	VULKAN_INSTANCE_FUNCTION(vkGetImageMemoryRequirements)              \
	VULKAN_INSTANCE_FUNCTION(vkGetImageMemoryRequirements2KHR)			\
	VULKAN_INSTANCE_FUNCTION(vkCmdCopyBufferToImage)                    \
	VULKAN_INSTANCE_FUNCTION(vkCmdBlitImage)							\
	VULKAN_INSTANCE_FUNCTION(vkCmdCopyImage)							\
	VULKAN_INSTANCE_FUNCTION(vkGetImageSubresourceLayout)				\
	VULKAN_INSTANCE_FUNCTION(vkCreateSampler)                           \
	VULKAN_INSTANCE_FUNCTION(vkDestroySampler)                          \
	VULKAN_INSTANCE_FUNCTION(vkQueueWaitIdle)                           \
	VULKAN_INSTANCE_FUNCTION(vkCreateDescriptorPool)                    \
	VULKAN_INSTANCE_FUNCTION(vkDestroyDescriptorPool)                   \
	VULKAN_INSTANCE_FUNCTION(vkAllocateDescriptorSets)                  \
	VULKAN_INSTANCE_FUNCTION(vkFreeDescriptorSets)                      \
	VULKAN_INSTANCE_FUNCTION(vkUpdateDescriptorSets)                    \
	VULKAN_INSTANCE_FUNCTION(vkCreateFramebuffer)						\
	VULKAN_INSTANCE_FUNCTION(vkDestroyFramebuffer)						\
    VULKAN_INSTANCE_FUNCTION(vkCreateSwapchainKHR)                      \
    VULKAN_INSTANCE_FUNCTION(vkDestroySwapchainKHR)                     \
    VULKAN_INSTANCE_FUNCTION(vkGetSwapchainImagesKHR)                   \
    VULKAN_INSTANCE_FUNCTION(vkAcquireNextImageKHR)                     \
    VULKAN_INSTANCE_FUNCTION(vkCreateCommandPool)                       \
    VULKAN_INSTANCE_FUNCTION(vkDestroyCommandPool)                      \
    VULKAN_INSTANCE_FUNCTION(vkAllocateCommandBuffers)                  \
    VULKAN_INSTANCE_FUNCTION(vkFreeCommandBuffers)                      \
    VULKAN_INSTANCE_FUNCTION(vkBeginCommandBuffer)                      \
    VULKAN_INSTANCE_FUNCTION(vkEndCommandBuffer)                        \
    VULKAN_INSTANCE_FUNCTION(vkResetCommandBuffer)                      \
    VULKAN_INSTANCE_FUNCTION(vkCreateFence)                             \
    VULKAN_INSTANCE_FUNCTION(vkDestroyFence)                            \
    VULKAN_INSTANCE_FUNCTION(vkResetFences)                             \
    VULKAN_INSTANCE_FUNCTION(vkWaitForFences)                           \
    VULKAN_INSTANCE_FUNCTION(vkGetFenceStatus)                          \
    VULKAN_INSTANCE_FUNCTION(vkCreateSemaphore)                         \
    VULKAN_INSTANCE_FUNCTION(vkDestroySemaphore)                        

#define VULKAN_DEVICE_FUNCTIONS()                                       \
	VULKAN_DEVICE_FUNCTION(vkCmdBeginRenderPass)						\
	VULKAN_DEVICE_FUNCTION(vkCmdEndRenderPass)							\
	VULKAN_DEVICE_FUNCTION(vkCmdClearAttachments)						\
	VULKAN_DEVICE_FUNCTION(vkCmdBindPipeline)							\
	VULKAN_DEVICE_FUNCTION(vkCmdSetViewport)							\
	VULKAN_DEVICE_FUNCTION(vkCmdSetScissor)								\
    VULKAN_DEVICE_FUNCTION(vkCmdClearColorImage)                        \
    VULKAN_DEVICE_FUNCTION(vkCmdPipelineBarrier)                        \
	VULKAN_DEVICE_FUNCTION(vkCmdExecuteCommands)						\
	VULKAN_DEVICE_FUNCTION(vkCmdBindDescriptorSets)						\
	VULKAN_DEVICE_FUNCTION(vkCmdBindVertexBuffers)						\
	VULKAN_DEVICE_FUNCTION(vkCmdBindIndexBuffer)						\
	VULKAN_DEVICE_FUNCTION(vkCmdDraw)									\
	VULKAN_DEVICE_FUNCTION(vkCmdDrawIndexed)							\
	VULKAN_DEVICE_FUNCTION(vkCmdDrawIndirect)							\
    VULKAN_DEVICE_FUNCTION(vkDeviceWaitIdle)                            \
    VULKAN_DEVICE_FUNCTION(vkGetDeviceQueue)                            \
    VULKAN_DEVICE_FUNCTION(vkQueuePresentKHR)                           \
    VULKAN_DEVICE_FUNCTION(vkQueueSubmit)                               \

#define VULKAN_DEVICE_FUNCTION(name) extern PFN_##name name;
#define VULKAN_GLOBAL_FUNCTION(name) extern PFN_##name name;
#define VULKAN_INSTANCE_FUNCTION(name) extern PFN_##name name;
#if !defined(NDEBUG) || defined(BACKEND_FORCE_DEBUGREPORT)
VULKAN_DEBUG_INSTANCE_FUNCTIONS();
#endif
VULKAN_GLOBAL_FUNCTIONS();
VULKAN_INSTANCE_FUNCTIONS();
VULKAN_DEVICE_FUNCTIONS();
#undef VULKAN_DEVICE_FUNCTION
#undef VULKAN_GLOBAL_FUNCTION
#undef VULKAN_INSTANCE_FUNCTION
extern PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;

#endif