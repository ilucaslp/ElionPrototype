#include "stdafx.h"
#include "mu_vulkan_garbagecollector.h"

namespace EVulkanGarbage
{
	std::vector<ETextureTrash> TrashTextures;
	std::vector<EBufferTrash> TrashBuffers;

	void MaskProcess(const mu_uint32 bufferIndex)
	{
		const mu_uint32 busyMask = ~(1 << bufferIndex);
		for (auto it = TrashTextures.begin(); it != TrashTextures.end(); ++it)
		{
			ETextureTrash &textureTrash = *it;
			textureTrash.BusyMask &= busyMask;
		}

		for (auto it = TrashBuffers.begin(); it != TrashBuffers.end(); ++it)
		{
			EBufferTrash &bufferTrash = *it;
			bufferTrash.BusyMask &= busyMask;
		}
	}

	void Empty()
	{
		for (auto it = TrashTextures.begin(); it != TrashTextures.end();)
		{
			ETextureTrash &textureTrash = *it;

			if (textureTrash.BusyMask == 0)
			{
				EVulkanMemory::DestroyFramebuffer(textureTrash.Framebuffer);
				EVulkanMemory::DestroyImageView(textureTrash.ImageView);
				EVulkanMemory::DestroyImage(textureTrash.Image, textureTrash.Memory);
				EVulkanDescriptor::FreeTextureDescriptorSet(textureTrash.DescriptorSet);
				it = TrashTextures.erase(it);
			}
			else
			{
				 ++it;
			}
		}

		for (auto it = TrashBuffers.begin(); it != TrashBuffers.end();)
		{
			EBufferTrash &bufferTrash = *it;

			if(bufferTrash.BusyMask == 0)
			{
				EVulkanMemory::DestroyBufferView(bufferTrash.BufferView);
				EVulkanMemory::DestroyBuffer(bufferTrash.Buffer, bufferTrash.Memory);
				it = TrashBuffers.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

	void ForceEmpty()
	{
		for (auto it = TrashTextures.begin(); it != TrashTextures.end();++it)
		{
			ETextureTrash &textureTrash = *it;

			EVulkanMemory::DestroyFramebuffer(textureTrash.Framebuffer);
			EVulkanMemory::DestroyImageView(textureTrash.ImageView);
			EVulkanMemory::DestroyImage(textureTrash.Image, textureTrash.Memory);
			EVulkanDescriptor::FreeTextureDescriptorSet(textureTrash.DescriptorSet);
		}
		TrashTextures.clear();

		for (auto it = TrashBuffers.begin(); it != TrashBuffers.end();++it)
		{
			EBufferTrash &bufferTrash = *it;

			EVulkanMemory::DestroyBufferView(bufferTrash.BufferView);
			EVulkanMemory::DestroyBuffer(bufferTrash.Buffer, bufferTrash.Memory);
		}
		TrashBuffers.clear();
	}

	void PushTexture(const mu_uint8 busyMask,
		VkImage image,
		VkImageView imageView,
		VmaAllocation memory,
		VkDescriptorSet descriptorSet,
		VkFramebuffer framebuffer)
	{
		TrashTextures.push_back(ETextureTrash(busyMask, image, imageView, memory, framebuffer, descriptorSet));
	}

	void PushBuffer(const mu_uint8 busyMask,
		VkBuffer buffer,
		VkBufferView bufferView,
		VmaAllocation memory)
	{
		TrashBuffers.push_back(EBufferTrash(busyMask, buffer, bufferView, memory));
	}

	void PushDynamicBuffers(const mu_uint32 count, VkBuffer *buffers, VmaAllocation *memories)
	{
		EBufferTrash bufferTrash = {};

		for(mu_uint32 n = 0;n < count;++n)
		{
			bufferTrash.Buffer = buffers[n];
			bufferTrash.Memory = memories[n];
			TrashBuffers.push_back(bufferTrash);
		}
	}
};