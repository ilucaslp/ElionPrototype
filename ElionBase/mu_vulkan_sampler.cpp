#include "stdafx.h"

namespace EVulkanSampler
{
	VkSamplerCreateInfo SamplerInfo[ESamplerMax] =
	{
		// Linears
		{ VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO, nullptr, 0, VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE, 0.0f, VK_FALSE, 0.0f, VK_FALSE, VK_COMPARE_OP_NEVER, TextureMinLOD, TextureMaxLOD, VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK, VK_FALSE },
		{ VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO, nullptr, 0, VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER, 0.0f, VK_FALSE, 0.0f, VK_FALSE, VK_COMPARE_OP_NEVER, TextureMinLOD, TextureMaxLOD, VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK, VK_FALSE },
		{ VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO, nullptr, 0, VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_REPEAT, VK_SAMPLER_ADDRESS_MODE_REPEAT, VK_SAMPLER_ADDRESS_MODE_REPEAT, 0.0f, VK_FALSE, 0.0f, VK_FALSE, VK_COMPARE_OP_NEVER, TextureMinLOD, TextureMaxLOD, VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK, VK_FALSE },

		// Nearests
		{ VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO, nullptr, 0, VK_FILTER_NEAREST, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE, 0.0f, VK_FALSE, 0.0f, VK_FALSE, VK_COMPARE_OP_NEVER, TextureMinLOD, TextureMaxLOD, VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK, VK_FALSE },
		{ VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO, nullptr, 0, VK_FILTER_NEAREST, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER, 0.0f, VK_FALSE, 0.0f, VK_FALSE, VK_COMPARE_OP_NEVER, TextureMinLOD, TextureMaxLOD, VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK, VK_FALSE },
		{ VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO, nullptr, 0, VK_FILTER_NEAREST, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_REPEAT, VK_SAMPLER_ADDRESS_MODE_REPEAT, VK_SAMPLER_ADDRESS_MODE_REPEAT, 0.0f, VK_FALSE, 0.0f, VK_FALSE, VK_COMPARE_OP_NEVER, TextureMinLOD, TextureMaxLOD, VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK, VK_FALSE },

		// Anisotropy Linears
		{ VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO, nullptr, 0, VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE, 0.0f, VK_TRUE, 16.0f, VK_FALSE, VK_COMPARE_OP_NEVER, TextureMinLOD, TextureMaxLOD, VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK, VK_FALSE },
		{ VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO, nullptr, 0, VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER, 0.0f, VK_TRUE, 16.0f, VK_FALSE, VK_COMPARE_OP_NEVER, TextureMinLOD, TextureMaxLOD, VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK, VK_FALSE },
		{ VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO, nullptr, 0, VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_REPEAT, VK_SAMPLER_ADDRESS_MODE_REPEAT, VK_SAMPLER_ADDRESS_MODE_REPEAT, 0.0f, VK_TRUE, 16.0f, VK_FALSE, VK_COMPARE_OP_NEVER, TextureMinLOD, TextureMaxLOD, VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK, VK_FALSE },

		// Anisotropy Nearests
		{ VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO, nullptr, 0, VK_FILTER_NEAREST, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE, 0.0f, VK_TRUE, 16.0f, VK_FALSE, VK_COMPARE_OP_NEVER, TextureMinLOD, TextureMaxLOD, VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK, VK_FALSE },
		{ VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO, nullptr, 0, VK_FILTER_NEAREST, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER, 0.0f, VK_TRUE, 16.0f, VK_FALSE, VK_COMPARE_OP_NEVER, TextureMinLOD, TextureMaxLOD, VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK, VK_FALSE },
		{ VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO, nullptr, 0, VK_FILTER_NEAREST, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_REPEAT, VK_SAMPLER_ADDRESS_MODE_REPEAT, VK_SAMPLER_ADDRESS_MODE_REPEAT, 0.0f, VK_TRUE, 16.0f, VK_FALSE, VK_COMPARE_OP_NEVER, TextureMinLOD, TextureMaxLOD, VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK, VK_FALSE },

		// Shadow Comparison
#if VULKAN_REVERSED_DEPTH == 1
		{ VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO, nullptr, 0, VK_FILTER_NEAREST, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER, 0.0f, VK_FALSE, 0.0f, VK_TRUE, VK_COMPARE_OP_GREATER_OR_EQUAL, TextureMinLOD, TextureMaxLOD, VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK, VK_FALSE },
		{ VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO, nullptr, 0, VK_FILTER_NEAREST, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER, 0.0f, VK_FALSE, 0.0f, VK_TRUE, VK_COMPARE_OP_GREATER, TextureMinLOD, TextureMaxLOD, VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK, VK_FALSE },
#else
		{ VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO, nullptr, 0, VK_FILTER_NEAREST, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER, 0.0f, VK_FALSE, 0.0f, VK_TRUE, VK_COMPARE_OP_LESS_OR_EQUAL, TextureMinLOD, TextureMaxLOD, VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK, VK_FALSE },
		{ VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO, nullptr, 0, VK_FILTER_NEAREST, VK_FILTER_NEAREST, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER, 0.0f, VK_FALSE, 0.0f, VK_TRUE, VK_COMPARE_OP_LESS, TextureMinLOD, TextureMaxLOD, VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK, VK_FALSE },
#endif
	};

	std::array<VkSampler, ESamplerMax> Sampler;

	mu_boolean Create()
	{
		VkResult result;

		for(mu_uint32 n = 0;n < ESamplerMax; ++n)
		{
			VkSamplerCreateInfo &createInfo = SamplerInfo[n];
			if(createInfo.anisotropyEnable == VK_TRUE &&
				createInfo.maxAnisotropy > EVulkan::DeviceProperties.limits.maxSamplerAnisotropy)
			{
				createInfo.maxAnisotropy = EVulkan::DeviceProperties.limits.maxSamplerAnisotropy;
			}

			result = vkCreateSampler(EVulkan::Device, &SamplerInfo[n], nullptr, &Sampler[n]);
			if(result != VK_SUCCESS)
			{
				return false;
			}
		}

		return true;
	}

	void Destroy()
	{
		for (mu_uint32 n = 0; n < ESamplerMax; ++n)
		{
			if (Sampler[n] != VK_NULL_HANDLE)
			{
				vkDestroySampler(EVulkan::Device, Sampler[n], nullptr);
				Sampler[n] = VK_NULL_HANDLE;
			}
		}
	}

	const VkSampler Get(const ESampler index)
	{
		return Sampler[static_cast<mu_uint32>(index)];
	}
};