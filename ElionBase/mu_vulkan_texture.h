#ifndef __MU_VULKAN_TEXTURE_H__
#define __MU_VULKAN_TEXTURE_H__

#pragma once

#if ELION_TEXTURETYPE_FORMAT == ELION_TEXTURETYPE_DDS
#ifdef DDPF_FOURCC
#undef DDPF_FOURCC
#endif
#include <gli/gli.hpp>
#include <gli/load_dds.hpp>
#else
#include <..\lib\vk_format.h>

EINLINE const VkFormat ktxTexture_GetVkFormat(ktxTexture* This)
{
	VkFormat vkFormat;

	vkFormat = vkGetFormatFromOpenGLInternalFormat(This->glInternalformat);
	if (vkFormat == VK_FORMAT_UNDEFINED)
		vkFormat = vkGetFormatFromOpenGLFormat(This->glFormat, This->glType);
	return vkFormat;
}
#endif

class ETextureBackend;

namespace EVulkanTexture
{
	static VkImageViewType ImageViewTypeList[EImageViewTypeMax] =
	{
		VK_IMAGE_VIEW_TYPE_MAX_ENUM,
		VK_IMAGE_VIEW_TYPE_1D,
		VK_IMAGE_VIEW_TYPE_1D_ARRAY,
		VK_IMAGE_VIEW_TYPE_2D,
		VK_IMAGE_VIEW_TYPE_2D_ARRAY,
		VK_IMAGE_VIEW_TYPE_3D,
		VK_IMAGE_VIEW_TYPE_CUBE,
	};

	const VkFormat RetrieveFormat(const ETextureFormat format);
	const mu_size RetrieveFormatSize(const ETextureFormat format);

	EINLINE mu_boolean CreateImageDynamic(VkImage &Image,
		VkImageView &ImageView,
		VkImageLayout &ImageLayout,
		VmaAllocation &Memory,
		VmaAllocationInfo &MemoryInfo,
		VkDeviceSize &MemorySize,
		const mu_uint32 Width,
		const mu_uint32 Height,
		const mu_uint32 Depth,
		const VkFormat ImageFormat,
		const VkImageType ImageType,
		const VkImageViewType ImageViewType,
		const VkImageUsageFlags ImageUsage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
		const VkImageAspectFlags ImageAspect = VK_IMAGE_ASPECT_COLOR_BIT)
	{
		const VkExtent3D ImageExtent =
		{
			Width,
			Height,
			Depth
		};

		if (EVulkanMemory::CreateImage(Image,
			Memory,
			MemoryInfo,
			ImageFormat,
			1,
			Depth,
			ImageType,
			ImageExtent,
			ImageUsage,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			VMA_MEMORY_USAGE_GPU_ONLY) == false)
		{
			mu_error("Failed to create image.");
			return false;
		}

		ImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;

		if (EVulkanMemory::CreateImageView(ImageView,
			Image,
			ImageFormat,
			1,
			Depth,
			ImageViewType,
			ImageAspect) == false)
		{
			EVulkanMemory::DestroyImage(Image, Memory);
			mu_error("Failed to create image view.");
			return false;
		}

		VkMemoryRequirements memoryRequirements = {};
		vkGetImageMemoryRequirements(EVulkan::Device, Image, &memoryRequirements);
		MemorySize = memoryRequirements.size;

		return true;
	}

	EINLINE void TransferCPUtoGPU(VkCommandBuffer &CommandBuffer,
		VkBuffer &StagingBuffer,
		VkImage &Image,
		VkImageLayout &ImageLayout,
		const VkImageLayout DstImageLayout,
		const mu_uint32 BufferRegionCount,
		const VkBufferImageCopy *BufferRegions,
		const mu_uint32 MipsBase,
		const mu_uint32 MipsLevel,
		const mu_uint32 LayerBase,
		const mu_uint32 LayerCount)
	{
		VkImageSubresourceRange subresourceRange = {};
		subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		subresourceRange.baseMipLevel = MipsBase;
		subresourceRange.levelCount = MipsLevel;
		subresourceRange.baseArrayLayer = LayerBase;
		subresourceRange.layerCount = LayerCount;

		EVulkan::SetImageLayout(CommandBuffer, Image, ImageLayout, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, subresourceRange);

		vkCmdCopyBufferToImage(CommandBuffer, StagingBuffer, Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, BufferRegionCount, BufferRegions);

		EVulkan::SetImageLayout(CommandBuffer, Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, DstImageLayout, subresourceRange);
		ImageLayout = DstImageLayout;
	}

	const mu_boolean LoadFromBuffer(const mu_uint8 *DataBuffer, const mu_uint32 DataSize, const EImageViewType ImageViewType, ETextureBackend *Texture);
	
	const mu_boolean CreateBones(ETextureBackend *Texture);
	const mu_boolean CreateShadowMap(ETextureBackend *Texture, VkDescriptorSet DescriptorSet);

	const mu_boolean Create1D(const mu_uint32 Width,
		const ETextureFormat Format,
		const ETextureUsage Usage,
		const ESampler Sampler,
		const mu_uint8 *DataBuffer,
		ETextureBackend *Texture);
	const mu_boolean Create2D(const mu_uint32 Width,
		const mu_uint32 Height,
		const ETextureFormat Format,
		const ETextureUsage Usage,
		const ESampler Sampler,
		const mu_uint8 *DataBuffer,
		const mu_uint32 Pitch,
		ETextureBackend *Texture);
	const mu_boolean Create2DArray(const mu_uint32 Width,
		const mu_uint32 Height,
		const mu_uint32 Layers,
		const ETextureFormat Format,
		const ETextureUsage Usage,
		const ESampler Sampler,
		const mu_uint8 *DataBuffer,
		const mu_uint32 Pitch,
		ETextureBackend *Texture);
};

#endif