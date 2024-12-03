#ifndef __MU_OPENGL_BUFFER_H__
#define __MU_OPENGL_BUFFER_H__

#pragma once

class EStagingBufferBackend;
class EBufferBackend;
class EUniformBufferBackend;

namespace EOpenGLBuffer
{
	const mu_boolean Allocate(const mu_size Size, const EBufferUsage Usage, const EBufferType Type, const void *DataBuffer, EBufferBackend *Buffer);
	void Deallocate(EBufferBackend *Buffer);
	void Flush(const mu_size StagingOffset, const mu_size BufferOffset, const mu_size Size, EBufferBackend *Buffer);

	const mu_boolean AllocateStaging(const mu_size Size, const mu_boolean ForceAllocate, EStagingBufferBackend *StagingBuffer);
	void DeallocateStaging(EStagingBufferBackend *StagingBuffer);
};

#endif