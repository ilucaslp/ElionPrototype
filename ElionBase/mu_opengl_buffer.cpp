#include "stdafx.h"

#if OPENGL_BACKEND_ENABLE == 1
namespace EOpenGLBuffer
{
	const mu_boolean Allocate(const mu_size Size, const EBufferUsage Usage, const EBufferType Type, const void *DataBuffer, EBufferBackend *Buffer)
	{
		EOpenGLHelpers::GenerateBuffers(1, &Buffer->OpenGLInternal.Resource);

		GLenum targetType = GL_INVALID_ENUM;
		switch(Type)
		{
		case EBufferType::eUniform:
			{
				targetType = GL_UNIFORM_BUFFER;
			}
			break;

		case EBufferType::eVertex:
		case EBufferType::eInstance:
			{
				targetType = GL_ARRAY_BUFFER;
			}
			break;

		case EBufferType::eIndex:
			{
				targetType = GL_ELEMENT_ARRAY_BUFFER;
			}
			break;

		default: break;
		}

		glBindBuffer(targetType, Buffer->OpenGLInternal.Resource);
		if (Usage != EBufferUsage::eStream &&
			EOpenGLExtensions::IsFeatureSupported(OpenGLFeatures::BufferStorage) == true)
		{
			if(Usage == EBufferUsage::eStatic)
			{
				glBufferStorage(targetType, Size, DataBuffer, 0);
			}
			else
			{
				GLenum usageType = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT;
				switch (Usage)
				{
				case EBufferUsage::eDynamic:
					{
						usageType |= GL_DYNAMIC_STORAGE_BIT;
					}
					break;

				default: break;
				}

				glBufferStorage(targetType, Size, nullptr, usageType);

				Buffer->OpenGLInternal.MappedMemory = glMapBufferRange(targetType, 0, Size, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_FLUSH_EXPLICIT_BIT);

				if (Buffer->OpenGLInternal.MappedMemory == nullptr)
				{
					return false;
				}
			}
		}
		else
		{
			GLenum usageType = GL_INVALID_ENUM;
			switch (Usage)
			{
			case EBufferUsage::eStatic:
				{
					usageType = GL_STATIC_DRAW;
				}
				break;

			case EBufferUsage::eDynamic:
				{
					usageType = GL_DYNAMIC_DRAW;
				}
				break;

			case EBufferUsage::eStream:
				{
					usageType = GL_STREAM_DRAW;
				}
				break;

			default: break;
			}

			glBufferData(targetType, Size, DataBuffer, usageType);

			if(Usage != EBufferUsage::eStatic)
			{
				Buffer->OpenGLInternal.MappedMemory = mu_malloc(Size);
			}
		}

		Buffer->OpenGLInternal.TargetType = targetType;

		if(EOpenGLError::ReportCheck() == false)
		{
			return false;
		}

		return true;
	}

	void Deallocate(EBufferBackend *Buffer)
	{
		if (Buffer->OpenGLInternal.MappedMemory != nullptr)
		{
			if (EOpenGLExtensions::IsFeatureSupported(OpenGLFeatures::BufferStorage) == true)
			{
				glBindBuffer(Buffer->OpenGLInternal.TargetType, Buffer->OpenGLInternal.Resource);
				glUnmapBuffer(Buffer->OpenGLInternal.TargetType);
				glBindBuffer(Buffer->OpenGLInternal.TargetType, 0);
			}
			else
			{
				mu_free(Buffer->OpenGLInternal.MappedMemory);
			}
			Buffer->OpenGLInternal.MappedMemory = nullptr;
		}

		EOpenGLHelpers::DeleteBuffers(1, &Buffer->OpenGLInternal.Resource);
	}

	void Flush(const mu_size StagingOffset, const mu_size BufferOffset, const mu_size Size, EBufferBackend *Buffer)
	{
		if (Size == 0) return;

		glBindBuffer(Buffer->OpenGLInternal.TargetType, Buffer->OpenGLInternal.Resource);
		if (EOpenGLExtensions::IsFeatureSupported(OpenGLFeatures::BufferStorage) == true)
		{
			glFlushMappedBufferRange(Buffer->OpenGLInternal.TargetType, BufferOffset, Size);
		}
		else
		{
			glBufferSubData(Buffer->OpenGLInternal.TargetType, BufferOffset, Size, reinterpret_cast<mu_uint8*>(Buffer->OpenGLInternal.MappedMemory) + StagingOffset);
		}
		glBindBuffer(Buffer->OpenGLInternal.TargetType, 0);
	}

	const mu_boolean AllocateStaging(const mu_size Size, const mu_boolean ForceAllocate, EStagingBufferBackend *StagingBuffer)
	{
		if(ForceAllocate == true ||
			EOpenGLExtensions::IsFeatureSupported(OpenGLFeatures::BufferStorage) == false)
		{
			StagingBuffer->BufferInternal.MappedMemory = mu_malloc(Size);
			if(StagingBuffer->BufferInternal.MappedMemory == nullptr)
			{
				return false;
			}
		}

		return true;
	}

	void DeallocateStaging(EStagingBufferBackend *StagingBuffer)
	{
		if(StagingBuffer->BufferInternal.MappedMemory != nullptr)
		{
			mu_free(StagingBuffer->BufferInternal.MappedMemory);
			StagingBuffer->BufferInternal.MappedMemory = nullptr;
		}
	}
};
#endif