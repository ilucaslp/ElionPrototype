#ifndef __MU_VULKAN_ATTRIBUTES_H__
#define __MU_VULKAN_ATTRIBUTES_H__

#pragma once

namespace EVulkanAttributes
{
	enum
	{
		VULKAN_UIDEFAULT_BINDING = 1,
		VULKAN_UIDEFAULT_ATTRIBUTES = 3,

		VULKAN_FONT_BINDING = 1,
		VULKAN_FONT_ATTRIBUTES = 4,

		VULKAN_TERRAIN_BINDING = 1,
		VULKAN_TERRAIN_ATTRIBUTES = 2,

		VULKAN_GRASS_BINDING = 1,
		VULKAN_GRASS_ATTRIBUTES = 2,

		VULKAN_WATER_BINDING = 1,
		VULKAN_WATER_ATTRIBUTES = 2,

		VULKAN_MODEL_BINDING = 2,
		VULKAN_MODEL_ATTRIBUTES = 11,

		VULKAN_CLOTH_BINDING = 2,
		VULKAN_CLOTH_ATTRIBUTES = 10,

		VULKAN_EFFECT_BINDING = 1,
		VULKAN_EFFECT_ATTRIBUTES = 8,

		VULKAN_BBOX_BINDING = 1,
		VULKAN_BBOX_ATTRIBUTES = 2,
	};

	void Retrieve(const EInputLayout layout,
		mu_uint32 &bindingCount,
		const VkVertexInputBindingDescription *&bindings,
		mu_uint32 &attributesCount,
		const VkVertexInputAttributeDescription *&attributes);
};

#endif