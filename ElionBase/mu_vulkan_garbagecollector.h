#ifndef __MU_VULKAN_GARBAGECOLLECTOR_H__
#define __MU_VULKAN_GARBAGECOLLECTOR_H__

#pragma once

namespace EVulkanGarbage
{
	class ETextureTrash
	{
	public:
		ETextureTrash() {}
		ETextureTrash(const mu_uint8 busyMask,
			VkImage image,
			VkImageView imageView,
			VmaAllocation memory,
			VkFramebuffer framebuffer,
			VkDescriptorSet descriptorSet) :
			BusyMask(busyMask),
			Image(image),
			ImageView(imageView),
			Memory(memory),
			Framebuffer(framebuffer),
			DescriptorSet(descriptorSet)
		{
		}

		mu_uint8 BusyMask;
		VkImage Image;
		VkImageView ImageView;
		VmaAllocation Memory;
		VkFramebuffer Framebuffer;
		VkDescriptorSet DescriptorSet;
	};

	class EBufferTrash
	{
	public:
		EBufferTrash() {}
		EBufferTrash(const mu_uint8 busyMask,
			VkBuffer buffer,
			VkBufferView bufferView,
			VmaAllocation memory) :
			BusyMask(busyMask),
			Buffer(buffer),
			BufferView(bufferView),
			Memory(memory)
		{
		}

		mu_uint8 BusyMask;
		VkBuffer Buffer;
		VkBufferView BufferView;
		VmaAllocation Memory;
	};

	void MaskProcess(const mu_uint32 bufferIndex);
	void Empty();
	void ForceEmpty();

	void PushTexture(const mu_uint8 busyMask,
		VkImage image,
		VkImageView imageView,
		VmaAllocation memory,
		VkDescriptorSet descriptorSet,
		VkFramebuffer framebuffer = VK_NULL_HANDLE);
	void PushBuffer(const mu_uint8 busyMask,
		VkBuffer buffer,
		VkBufferView bufferView,
		VmaAllocation memory);
	void PushDynamicBuffers(const mu_uint32 count, VkBuffer *buffers, VmaAllocation *memories);
};

#endif