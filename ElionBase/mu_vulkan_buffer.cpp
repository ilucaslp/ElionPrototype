#include "stdafx.h"

namespace EVulkanBuffer
{
	const mu_boolean Allocate(const mu_uint64 Size, const EBufferUsage Usage, const EBufferType Type, const void *DataBuffer, EBufferBackend *Buffer)
	{
		EStagingBufferBackend StagingBuffer;

		if (Usage == EBufferUsage::eStatic &&
			StagingBuffer.Allocate(Size) == false)
		{
			return false;
		}

		VkDeviceSize alignedSize = Size;
		VmaAllocationInfo memoryInfo = {};
		switch(Type)
		{
		case EBufferType::eUniform:
			{
				EVulkan::AlignUniformSize(alignedSize);

#if defined(__ANDROID__)
				if (EVulkanMemory::CreateBuffer(Buffer->VulkanInternal.Buffer,
					Buffer->VulkanInternal.Memory,
					memoryInfo,
					alignedSize,
					VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
					VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | (Usage != EBufferUsage::eStatic ? VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT : 0),
					VMA_MEMORY_USAGE_CPU_TO_GPU,
					0,
					VMA_ALLOCATION_CREATE_MAPPED_BIT) == false)
				{
					return false;
				}
#else
				if (EVulkanMemory::CreateBuffer(Buffer->VulkanInternal.Buffer,
					Buffer->VulkanInternal.Memory,
					memoryInfo,
					alignedSize,
					VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
					VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
					VMA_MEMORY_USAGE_GPU_ONLY) == false)
				{
					return false;
				}
#endif
			}
			break;

		case EBufferType::eVertex:
		case EBufferType::eInstance:
			{
#if defined(__ANDROID__)
				if (EVulkanMemory::CreateBuffer(Buffer->VulkanInternal.Buffer,
					Buffer->VulkanInternal.Memory,
					memoryInfo,
					alignedSize,
					VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
					VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | (Usage != EBufferUsage::eStatic ? VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT : 0),
					VMA_MEMORY_USAGE_CPU_TO_GPU,
					0,
					VMA_ALLOCATION_CREATE_MAPPED_BIT) == false)
				{
					return false;
				}
#else
				if (EVulkanMemory::CreateBuffer(Buffer->VulkanInternal.Buffer,
					Buffer->VulkanInternal.Memory,
					memoryInfo,
					alignedSize,
					VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
					VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
					VMA_MEMORY_USAGE_GPU_ONLY) == false)
				{
					return false;
				}
#endif
			}
			break;

		case EBufferType::eIndex:
			{
#if defined(__ANDROID__)
				if (EVulkanMemory::CreateBuffer(Buffer->VulkanInternal.Buffer,
					Buffer->VulkanInternal.Memory,
					memoryInfo,
					alignedSize,
					VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
					VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | (Usage != EBufferUsage::eStatic ? VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT : 0),
					VMA_MEMORY_USAGE_CPU_TO_GPU,
					0,
					VMA_ALLOCATION_CREATE_MAPPED_BIT) == false)
				{
					return false;
				}
#else
				if (EVulkanMemory::CreateBuffer(Buffer->VulkanInternal.Buffer,
					Buffer->VulkanInternal.Memory,
					memoryInfo,
					alignedSize,
					VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
					VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
					VMA_MEMORY_USAGE_GPU_ONLY) == false)
				{
					return false;
				}
#endif
			}
			break;

		default: break;
		}

#if defined(__ANDROID__)
		Buffer->VulkanInternal.MappedMemory = memoryInfo.pMappedData;

		if (Usage == EBufferUsage::eStatic)
		{
			mu_memcpy(Buffer->VulkanInternal.MappedMemory, DataBuffer, Size);
			Buffer->Flush(0, 0, VK_WHOLE_SIZE);
		}
#else
		if(Usage == EBufferUsage::eDynamic || Usage == EBufferUsage::eStream)
		{
			VmaAllocationInfo memoryInfo = {};
			if (EVulkanMemory::CreateBuffer(Buffer->VulkanInternal.StagingBuffer,
				Buffer->VulkanInternal.StagingMemory,
				memoryInfo,
				alignedSize,
				VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
				VMA_MEMORY_USAGE_CPU_ONLY,
				0,
				VMA_ALLOCATION_CREATE_MAPPED_BIT) == false)
			{
				return false;
			}

			Buffer->VulkanInternal.MappedMemory = memoryInfo.pMappedData;
		}

		if (Usage == EBufferUsage::eStatic)
		{
			mu_memcpy(StagingBuffer.BufferInternal.MappedMemory, DataBuffer, Size);
			StagingBuffer.Flush(0, VK_WHOLE_SIZE);

			VkCommandBuffer copyCmd = EVulkan::CreateCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, EVulkan::ShortCommandPool, true);

			VkBufferCopy copyRegion = {};

			copyRegion.size = Size;
			vkCmdCopyBuffer(copyCmd, StagingBuffer.VulkanInternal.StagingBuffer, Buffer->VulkanInternal.Buffer, 1, &copyRegion);

			EVulkan::FlushCommandBuffer<true>(copyCmd, EVulkan::ShortCommandPool, EVulkan::GraphicsQueue);
		}
#endif

		return true;
	}

	void Deallocate(EBufferBackend *Buffer)
	{
		if(Buffer->VulkanInternal.StagingBuffer != VK_NULL_HANDLE)
		{
			EVulkanGarbage::PushBuffer(Buffer->BufferInternal.BusyMask, Buffer->VulkanInternal.StagingBuffer, VK_NULL_HANDLE, Buffer->VulkanInternal.StagingMemory);
			Buffer->VulkanInternal.StagingBuffer = VK_NULL_HANDLE;
			Buffer->VulkanInternal.StagingMemory = nullptr;
		}
		EVulkanGarbage::PushBuffer(Buffer->BufferInternal.BusyMask, Buffer->VulkanInternal.Buffer, VK_NULL_HANDLE, Buffer->VulkanInternal.Memory);
		Buffer->VulkanInternal.Buffer = VK_NULL_HANDLE;
		Buffer->VulkanInternal.Memory = nullptr;
		Buffer->VulkanInternal.MappedMemory = nullptr;
	}

	void Flush(const mu_uint64 StagingOffset, const mu_uint64 BufferOffset, const mu_uint64 Size, EBufferBackend *Buffer)
	{
		if (Size == 0) return;

		const mu_uint32 index = EVulkan::CurrentCommandBufferIndex;

#if !defined(__ANDROID__)
		EVulkanMemory::FlushMemory(Buffer->VulkanInternal.StagingMemory, StagingOffset, Size);

		VkBufferCopy copyRegion[1] = { {} };
		copyRegion[0].srcOffset = StagingOffset;
		copyRegion[0].dstOffset = BufferOffset;
		copyRegion[0].size = Size;

		vkCmdCopyBuffer(EVulkan::TransferBuffers[index], Buffer->VulkanInternal.StagingBuffer, Buffer->VulkanInternal.Buffer, mu_countof(copyRegion), copyRegion);
#else
		EVulkanMemory::FlushMemory(Buffer->VulkanInternal.Memory, StagingOffset, Size);
#endif

		EVulkan::MarkBusyMask(Buffer->BufferInternal.BusyMask, index);
	}

	const mu_boolean AllocateStaging(const mu_uint64 Size, EStagingBufferBackend *StagingBuffer)
	{
		VmaAllocationInfo memoryInfo = {};
		if (EVulkanMemory::CreateBuffer(StagingBuffer->VulkanInternal.StagingBuffer,
			StagingBuffer->VulkanInternal.StagingMemory,
			memoryInfo,
			Size,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			VMA_MEMORY_USAGE_CPU_ONLY,
			0,
			VMA_ALLOCATION_CREATE_MAPPED_BIT) == false)
		{
			return false;
		}

		StagingBuffer->BufferInternal.MappedMemory = memoryInfo.pMappedData;

		return true;
	}

	void DeallocateStaging(EStagingBufferBackend *StagingBuffer)
	{
		StagingBuffer->BufferInternal.MappedMemory = nullptr;
		EVulkanGarbage::PushBuffer(StagingBuffer->BufferInternal.BusyMask, StagingBuffer->VulkanInternal.StagingBuffer, VK_NULL_HANDLE, StagingBuffer->VulkanInternal.StagingMemory);
		StagingBuffer->VulkanInternal.StagingBuffer = VK_NULL_HANDLE;
		StagingBuffer->VulkanInternal.StagingMemory = nullptr;
	}

	void FlushStaging(const mu_uint64 Offset, const mu_uint64 Size, EStagingBufferBackend *StagingBuffer)
	{
		if (Size == 0) return;

		EVulkanMemory::FlushMemory(StagingBuffer->VulkanInternal.StagingMemory, Offset, Size);
	}
};