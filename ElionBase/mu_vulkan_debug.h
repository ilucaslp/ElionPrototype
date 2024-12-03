#ifndef __MU_VULKAN_DEBUG_H__
#define __MU_VULKAN_DEBUG_H__

#pragma once

namespace EVulkanDebug
{
	VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
		VkDebugReportFlagsEXT flags,
		VkDebugReportObjectTypeEXT objType,
		uint64_t srcObject,
		size_t location,
		int32_t msgCode,
		const char* pLayerPrefix,
		const char* pMsg,
		void* pUserData);
};

#endif