#ifndef __MU_VIDEO_BUFFER_H__
#define __MU_VIDEO_BUFFER_H__

#pragma once

/*
	Used as base for improve transfer to GPU
	Related to Vulkan, but due to structure simplification, it is also used for others backend
	At future will be used for Metal
*/
class EStagingBufferBackend
{
public:
	EStagingBufferBackend()
	{
		mu_zeromem(this, sizeof(EStagingBufferBackend));
	}
	~EStagingBufferBackend()
	{
		Deallocate();
	}

	EINLINE const mu_boolean Allocate(const mu_size Size, const mu_boolean ForceAllocate = false)
	{
		if(BufferInternal.Size != 0)
		{
			return false;
		}

		mu_boolean isAllocated = false;
		switch (EVideoBackend::GetBackend())
		{
#if DIRECTX_BACKEND_ENABLE == 1
		case EVideoBackendType::eDirectX:
			isAllocated = EDirectXBuffer::AllocateStaging(Size, ForceAllocate, this);
			break;
#endif

#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
			isAllocated = EOpenGLBuffer::AllocateStaging(Size, ForceAllocate, this);
			break;
#endif

#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
			isAllocated = EVulkanBuffer::AllocateStaging(Size, this);
			break;
#endif

		default: break;
		}

		if(isAllocated == true)
		{
			BufferInternal.Size = Size;
		}

		return isAllocated;
	}

	EINLINE void Deallocate()
	{
		if(BufferInternal.Size != 0)
		{
			switch (EVideoBackend::GetBackend())
			{
#if DIRECTX_BACKEND_ENABLE == 1
			case EVideoBackendType::eDirectX:
				{
					EDirectXBuffer::DeallocateStaging(this);
				}
				break;
#endif

#if OPENGL_BACKEND_ENABLE == 1
			case EVideoBackendType::eOpenGL:
				{
					EOpenGLBuffer::DeallocateStaging(this);
				}
				break;
#endif

#if VULKAN_BACKEND_ENABLE == 1
			case EVideoBackendType::eVulkan:
				{
					EVulkanBuffer::DeallocateStaging(this);
				}
				break;
#endif

			default: break;
			}

			BufferInternal.Size = 0;
		}
	}

	EINLINE void Flush(const mu_uint64 Offset, const mu_uint64 Size)
	{
		switch (EVideoBackend::GetBackend())
		{
#if DIRECTX_BACKEND_ENABLE == 1
		case EVideoBackendType::eDirectX:
			{
			}
			break;
#endif

#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
			{
			}
			break;
#endif

#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
			{
				EVulkanBuffer::FlushStaging(Offset, Size, this);
			}
			break;
#endif

		default: break;
		}
	}

public:
	struct 
	{
		mu_uint8 BusyMask;
		mu_size Size;
		void *MappedMemory;
	} BufferInternal;

	union
	{
#if DIRECTX_BACKEND_ENABLE == 1
		struct
		{
		} DirectXInternal;
#endif
#if OPENGL_BACKEND_ENABLE == 1
		struct
		{
		} OpenGLInternal;
#endif
#if VULKAN_BACKEND_ENABLE == 1
		struct
		{
			VkBuffer StagingBuffer;
			VmaAllocation StagingMemory;
		} VulkanInternal;
#endif
	};
};

class EBufferBackend
{
public:
	EBufferBackend()
	{
		mu_zeromem(this, sizeof(EBufferBackend));
	}
	~EBufferBackend()
	{
		Deallocate();
	}

	EINLINE const mu_boolean Allocate(const mu_size Size, const EBufferUsage Usage, const EBufferType Type, const void *DataBuffer = nullptr)
	{
		if(BufferInternal.Size != 0)
		{
			return false;
		}

		mu_boolean isAllocated = false;
		switch (EVideoBackend::GetBackend())
		{
#if DIRECTX_BACKEND_ENABLE == 1
		case EVideoBackendType::eDirectX:
			isAllocated = EDirectXBuffer::Allocate(Size, Usage, Type, DataBuffer, this);
			break;
#endif

#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
			isAllocated = EOpenGLBuffer::Allocate(Size, Usage, Type, DataBuffer, this);
			break;
#endif

#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
			isAllocated = EVulkanBuffer::Allocate(Size, Usage, Type, DataBuffer, this);
			break;
#endif

		default: break;
		}

		if(isAllocated == true)
		{
			BufferInternal.Size = Size;
		}

		return isAllocated;
	}

	EINLINE void Deallocate()
	{
		switch (EVideoBackend::GetBackend())
		{
#if DIRECTX_BACKEND_ENABLE == 1
		case EVideoBackendType::eDirectX:
			{
				EDirectXBuffer::Deallocate(this);
			}
			break;
#endif

#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
			{
				EOpenGLBuffer::Deallocate(this);
			}
			break;
#endif

#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
			{
				EVulkanBuffer::Deallocate(this);
			}
			break;
#endif

		default: break;
		}

		BufferInternal.Size = 0;
	}

	EINLINE void ConfigureUniform(const EUniformLayout Layout)
	{
		switch (EVideoBackend::GetBackend())
		{
#if DIRECTX_BACKEND_ENABLE == 1
		case EVideoBackendType::eDirectX:
			{
			}
			break;
#endif

#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
			{
			}
			break;
#endif

#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
			{
				switch (Layout)
				{
				case EUniformLayout::eFont:
					{
						VulkanInternal.DescriptorSet = EVulkanDescriptor::CreateFontDescriptorSet(this);
					}
					break;
				default: break;
				}
			}
			break;
#endif

		default: break;
		}
	}

	EINLINE void Flush(const mu_size StagingOffset, const mu_size BufferOffset, const mu_size Size)
	{
		switch (EVideoBackend::GetBackend())
		{
#if DIRECTX_BACKEND_ENABLE == 1
		case EVideoBackendType::eDirectX:
			{
				EDirectXBuffer::Flush(StagingOffset, BufferOffset, Size, this);
			}
			break;
#endif

#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
			{
				EOpenGLBuffer::Flush(StagingOffset, BufferOffset, Size, this);
			}
			break;
#endif

#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
			{
				EVulkanBuffer::Flush(StagingOffset, BufferOffset, Size, this);
			}
			break;
#endif

		default: break;
		}
	}

public:
	struct 
	{
		mu_uint8 BusyMask;
		mu_size Size;
	} BufferInternal;

	union
	{
#if DIRECTX_BACKEND_ENABLE == 1
		struct
		{
			D3D11_USAGE Usage;
			ID3D11Buffer *Resource;
			void *MappedMemory;
		} DirectXInternal;
#endif
#if OPENGL_BACKEND_ENABLE == 1
		struct
		{
			GLenum TargetType;
			GLuint Resource;
			void *MappedMemory;
		} OpenGLInternal;
#endif
#if VULKAN_BACKEND_ENABLE == 1
		struct
		{
			VkBuffer Buffer;
			VmaAllocation Memory;
			VkBuffer StagingBuffer;
			VmaAllocation StagingMemory;
			VkDescriptorSet DescriptorSet;
			void *MappedMemory;
		} VulkanInternal;
#endif
	};
};

#if OPENGL_BACKEND_ENABLE == 1
#include "mu_opengl_dynamicbuffer.h"
#endif
#if VULKAN_BACKEND_ENABLE == 1
#include "mu_vulkan_dynamicbuffer.h"
#endif

#endif