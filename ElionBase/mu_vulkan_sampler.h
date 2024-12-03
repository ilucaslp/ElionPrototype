#ifndef __MU_VULKAN_SAMPLER_H__
#define __MU_VULKAN_SAMPLER_H__

#pragma once

namespace EVulkanSampler
{
	mu_boolean Create();
	void Destroy();
	const VkSampler Get(const ESampler index);
};

#endif