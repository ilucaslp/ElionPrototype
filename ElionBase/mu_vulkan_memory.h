#ifndef __MU_VULKAN_MEMORY_H__
#define __MU_VULKAN_MEMORY_H__

#pragma once

namespace EVulkanMemory
{
	extern VmaAllocator MemoryAllocator;

	const mu_boolean Initialize();
	void Destroy();

	EINLINE const mu_boolean CreateBuffer(VkBuffer &Buffer,
		VmaAllocation &Memory,
		VmaAllocationInfo &MemoryInfo,
		const VkDeviceSize BufferSize,
		const VkBufferUsageFlags UsageFlags,
		const VkMemoryPropertyFlags MemoryFlags,
		const VmaMemoryUsage MemoryUsageFlags,
		const VkMemoryPropertyFlags PreferredMemoryFlags = 0,
		const VmaAllocationCreateFlags MemoryAllocateFlags = 0)
	{
		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = BufferSize;
		bufferInfo.usage = UsageFlags;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VmaAllocationCreateInfo allocationInfo = {};
		allocationInfo.flags = MemoryAllocateFlags;
		allocationInfo.usage = MemoryUsageFlags;
		allocationInfo.requiredFlags = MemoryFlags;
		allocationInfo.preferredFlags = PreferredMemoryFlags;

		VkResult result = vmaCreateBuffer(MemoryAllocator, &bufferInfo, &allocationInfo, &Buffer, &Memory, &MemoryInfo);
		if (result != VK_SUCCESS)
		{
			return false;
		}

		return true;
	}

	EINLINE void DestroyBuffer(VkBuffer &Buffer,
		VmaAllocation &Memory)
	{
		if(Buffer != VK_NULL_HANDLE)
		{
			vmaDestroyBuffer(MemoryAllocator, Buffer, Memory);
			Buffer = VK_NULL_HANDLE;
			Memory = nullptr;
		}
	}

	EINLINE const mu_boolean CreateBufferView(VkBufferView &BufferView,
		VkBuffer Buffer,
		const VkFormat Format,
		const VkDeviceSize BufferSize,
		const VkDeviceSize BufferOffset = 0)
	{
		VkBufferViewCreateInfo viewCreateInfo = {};
		viewCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO;
		viewCreateInfo.pNext = nullptr;
		viewCreateInfo.buffer = Buffer;
		viewCreateInfo.format = Format;
		viewCreateInfo.offset = BufferOffset;
		viewCreateInfo.range = BufferSize;

		VkResult result = vkCreateBufferView(EVulkan::Device, &viewCreateInfo, nullptr, &BufferView);
		if (result != VK_SUCCESS)
		{
			return false;
		}

		return true;
	}

	EINLINE void DestroyBufferView(VkBufferView &buffer)
	{
		if (buffer != VK_NULL_HANDLE)
		{
			vkDestroyBufferView(EVulkan::Device, buffer, nullptr);
			buffer = VK_NULL_HANDLE;
		}
	}

	EINLINE const mu_boolean CreateImage(VkImage &Image,
		VmaAllocation &Memory,
		VmaAllocationInfo &MemoryInfo,
		const VkFormat Format,
		const mu_uint32 MipLevels,
		const mu_uint32 ArrayLayers,
		const VkImageType Type,
		const VkExtent3D Extent,
		const VkImageUsageFlags UsageFlags,
		const VkMemoryPropertyFlags MemoryFlags,
		const VmaMemoryUsage MemoryUsageFlags,
		const VkMemoryPropertyFlags PreferredMemoryFlags = 0,
		const VkImageCreateFlags CreateFlags = 0)
	{
		VkImageCreateInfo imageCreateInfo = {};
		imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageCreateInfo.flags = CreateFlags;
		imageCreateInfo.imageType = Type;
		imageCreateInfo.format = Format;
		imageCreateInfo.extent = Extent;
		imageCreateInfo.mipLevels = MipLevels;
		imageCreateInfo.arrayLayers = ArrayLayers;
		imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageCreateInfo.usage = UsageFlags;
		imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

		VmaAllocationCreateInfo allocationInfo = {};
		allocationInfo.usage = MemoryUsageFlags;
		allocationInfo.requiredFlags = MemoryFlags;
		allocationInfo.preferredFlags = MemoryFlags;

		VkResult result = vmaCreateImage(MemoryAllocator, &imageCreateInfo, &allocationInfo, &Image, &Memory, &MemoryInfo);
		if(result != VK_SUCCESS)
		{
			return false;
		}

		return true;
	}

	EINLINE void DestroyImage(VkImage &Image,
		VmaAllocation &Memory)
	{
		if(Image != VK_NULL_HANDLE)
		{
			vmaDestroyImage(MemoryAllocator, Image, Memory);
			Image = VK_NULL_HANDLE;
			Memory = nullptr;
		}
	}

	EINLINE const mu_boolean CreateImageView(VkImageView &ImageView,
		VkImage Image,
		const VkFormat Format,
		const mu_uint32 MipLevels,
		const mu_uint32 ArrayLayers,
		const VkImageViewType ViewType,
		const VkImageAspectFlags ImageAspect = VK_IMAGE_ASPECT_COLOR_BIT)
	{
		VkImageViewCreateInfo viewCreateInfo = {};
		viewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewCreateInfo.format = Format;
		viewCreateInfo.components = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
		viewCreateInfo.subresourceRange.aspectMask = ImageAspect;
		viewCreateInfo.subresourceRange.baseArrayLayer = 0;
		viewCreateInfo.subresourceRange.layerCount = ArrayLayers;
		viewCreateInfo.subresourceRange.baseMipLevel = 0;
		viewCreateInfo.subresourceRange.levelCount = MipLevels;
		viewCreateInfo.image = Image;
		viewCreateInfo.viewType = ViewType;

		VkResult result = vkCreateImageView(EVulkan::Device, &viewCreateInfo, nullptr, &ImageView);
		if (result != VK_SUCCESS)
		{
			return false;
		}

		return true;
	}

	EINLINE void DestroyImageView(VkImageView &image)
	{
		if (image != VK_NULL_HANDLE)
		{
			vkDestroyImageView(EVulkan::Device, image, nullptr);
			image = VK_NULL_HANDLE;
		}
	}

	EINLINE const mu_boolean CreateFramebuffer(VkFramebuffer &Framebuffer,
		const mu_uint32 Width,
		const mu_uint32 Height,
		const mu_uint32 Layers,
		const mu_uint32 AttachmentCount,
		const VkImageView *Attachments,
		const VkRenderPass RenderPass)
	{
		VkFramebufferCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		createInfo.renderPass = RenderPass;
		createInfo.attachmentCount = AttachmentCount;
		createInfo.pAttachments = Attachments;
		createInfo.width = Width;
		createInfo.height = Height;
		createInfo.layers = Layers;

		VkResult result = vkCreateFramebuffer(EVulkan::Device, &createInfo, nullptr, &Framebuffer);
		if(result != VK_SUCCESS)
		{
			return false;
		}

		return true;
	}

	EINLINE void DestroyFramebuffer(VkFramebuffer &framebuffer)
	{
		if (framebuffer != VK_NULL_HANDLE)
		{
			vkDestroyFramebuffer(EVulkan::Device, framebuffer, nullptr);
			framebuffer = VK_NULL_HANDLE;
		}
	}

	EINLINE VkResult MapMemory(VmaAllocation Memory, void **MappedPointer)
	{
		return vmaMapMemory(MemoryAllocator, Memory, MappedPointer);
	}

	EINLINE void UnmapMemory(VmaAllocation Memory)
	{
		vmaUnmapMemory(MemoryAllocator, Memory);
	}

	EINLINE void FlushMemory(VmaAllocation Memory, const VkDeviceSize Offset, const VkDeviceSize Size)
	{
		vmaFlushAllocation(MemoryAllocator, Memory, Offset, Size);
	}
};

#endif