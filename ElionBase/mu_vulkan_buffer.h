#ifndef __MU_VULKAN_BUFFER_H__
#define __MU_VULKAN_BUFFER_H__

#pragma once

class EStagingBufferBackend;
class EBufferBackend;
class EUniformBufferBackend;

namespace EVulkanBuffer
{
	const mu_boolean Allocate(const mu_uint64 Size, const EBufferUsage Usage, const EBufferType Type, const void *DataBuffer, EBufferBackend *Buffer);
	void Deallocate(EBufferBackend *Buffer);
	void Flush(const mu_uint64 StagingOffset, const mu_uint64 BufferOffset, const mu_uint64 Size, EBufferBackend *Buffer);

	const mu_boolean AllocateStaging(const mu_uint64 Size, EStagingBufferBackend *StagingBuffer);
	void DeallocateStaging(EStagingBufferBackend *StagingBuffer);
	void FlushStaging(const mu_uint64 Offset, const mu_uint64 Size, EStagingBufferBackend *StagingBuffer);
};

#endif