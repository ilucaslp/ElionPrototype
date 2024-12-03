#ifndef __MU_VULKAN_TEXTURE_FORMATS_H__
#define __MU_VULKAN_TEXTURE_FORMATS_H__

#pragma once

static const VkFormat VulkanFormats[ETextureFormatMax] =
{
	VK_FORMAT_R8_UNORM,
	VK_FORMAT_R8_SNORM,
	VK_FORMAT_R8_UINT,
	VK_FORMAT_R8_SINT,

	VK_FORMAT_R8G8_UNORM,
	VK_FORMAT_R8G8_SNORM,
	VK_FORMAT_R8G8_UINT,
	VK_FORMAT_R8G8_SINT,

	VK_FORMAT_R8G8B8A8_UNORM,
	VK_FORMAT_R8G8B8A8_SNORM,
	VK_FORMAT_R8G8B8A8_UINT,
	VK_FORMAT_R8G8B8A8_SINT,

	VK_FORMAT_R16_UNORM,
	VK_FORMAT_R16_SNORM,
	VK_FORMAT_R16_UINT,
	VK_FORMAT_R16_SINT,
	VK_FORMAT_R16_SFLOAT,

	VK_FORMAT_R16G16_UNORM,
	VK_FORMAT_R16G16_SNORM,
	VK_FORMAT_R16G16_UINT,
	VK_FORMAT_R16G16_SINT,
	VK_FORMAT_R16G16_SFLOAT,

	VK_FORMAT_R16G16B16A16_UNORM,
	VK_FORMAT_R16G16B16A16_SNORM,
	VK_FORMAT_R16G16B16A16_UINT,
	VK_FORMAT_R16G16B16A16_SINT,
	VK_FORMAT_R16G16B16A16_SFLOAT,

	VK_FORMAT_R32_UINT,
	VK_FORMAT_R32_SINT,
	VK_FORMAT_R32_SFLOAT,

	VK_FORMAT_R32G32_UINT,
	VK_FORMAT_R32G32_SINT,
	VK_FORMAT_R32G32_SFLOAT,

	VK_FORMAT_R32G32B32_UINT,
	VK_FORMAT_R32G32B32_SINT,
	VK_FORMAT_R32G32B32_SFLOAT,

	VK_FORMAT_R32G32B32A32_UINT,
	VK_FORMAT_R32G32B32A32_SINT,
	VK_FORMAT_R32G32B32A32_SFLOAT,

	VK_FORMAT_D16_UNORM,
	VK_FORMAT_D32_SFLOAT,
};

static const mu_size VulkanFormatSizes[ETextureFormatMax] =
{
	sizeof(mu_uint8) * 1,
	sizeof(mu_uint8) * 1,
	sizeof(mu_uint8) * 1,
	sizeof(mu_uint8) * 1,

	sizeof(mu_uint8) * 2,
	sizeof(mu_uint8) * 2,
	sizeof(mu_uint8) * 2,
	sizeof(mu_uint8) * 2,

	sizeof(mu_uint8) * 4,
	sizeof(mu_uint8) * 4,
	sizeof(mu_uint8) * 4,
	sizeof(mu_uint8) * 4,

	sizeof(mu_uint16) * 1,
	sizeof(mu_uint16) * 1,
	sizeof(mu_uint16) * 1,
	sizeof(mu_uint16) * 1,
	sizeof(mu_uint16) * 1,

	sizeof(mu_uint16) * 2,
	sizeof(mu_uint16) * 2,
	sizeof(mu_uint16) * 2,
	sizeof(mu_uint16) * 2,
	sizeof(mu_uint16) * 2,

	sizeof(mu_uint16) * 4,
	sizeof(mu_uint16) * 4,
	sizeof(mu_uint16) * 4,
	sizeof(mu_uint16) * 4,
	sizeof(mu_uint16) * 4,

	sizeof(mu_uint32) * 1,
	sizeof(mu_uint32) * 1,
	sizeof(mu_float) * 1,

	sizeof(mu_uint32) * 2,
	sizeof(mu_uint32) * 2,
	sizeof(mu_float) * 2,

	sizeof(mu_uint32) * 3,
	sizeof(mu_uint32) * 3,
	sizeof(mu_float) * 3,

	sizeof(mu_uint32) * 4,
	sizeof(mu_uint32) * 4,
	sizeof(mu_float) * 4,

	sizeof(mu_uint16) * 1,
	sizeof(mu_float) * 1,
};

#endif