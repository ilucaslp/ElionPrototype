#ifndef __MU_VIDEO_TEXTURE_H__
#define __MU_VIDEO_TEXTURE_H__

#pragma once

class ETextureBackendInfo
{
public:
	ETextureBackendInfo()
	{
		TextureInternal.Loaded = false;
		TextureInternal.Requested = false;
		TextureInternal.StaticDescriptorSet = false;
		TextureInternal.RenderTarget = false;
		TextureInternal.BusyMask = 0;
		TextureInternal.Sampler = ESampler::eInvalid;
		TextureInternal.Width = 0;
		TextureInternal.Height = 0;
		TextureInternal.Depth = 0;
		TextureInternal.Components = 0;
		TextureInternal.TextureType = ETextureClassType::eTexture;
	}

	~ETextureBackendInfo()
	{
	}

public:
	struct
	{
		mu_boolean Loaded;
		mu_atomic_bool Requested;
		mu_boolean StaticDescriptorSet;
		mu_boolean RenderTarget;
		mu_uint8 BusyMask;
		ESampler Sampler;
		mu_uint16 Width;
		mu_uint16 Height;
		mu_uint16 Depth;
		mu_uint8 Components;
		ETextureClassType TextureType; // vulkan only (might be used for metal too)
	} TextureInternal;
};

class ETextureBackendData
{
public:
	ETextureBackendData()
	{
		mu_zeromem(static_cast<ETextureBackendData*>(this), sizeof(ETextureBackendData));
	}

	~ETextureBackendData()
	{
	}

public:
	union
	{
#if DIRECTX_BACKEND_ENABLE == 1
		struct
		{
			D3D11_USAGE Usage;
			ID3D11Resource *TextureResource;
			union
			{
				struct
				{
					ID3D11ShaderResourceView *ShaderView;
					ID3D11DepthStencilView *DepthStencilView;
				};
				struct
				{
					ID3D11RenderTargetView *RenderTargetView;
				};
			};
		} DirectXInternal;
#endif
#if OPENGL_BACKEND_ENABLE == 1
		struct
		{
			GLenum TargetType;
			GLuint TextureResource;
			GLuint DepthResource;
			GLuint BufferResource;
			GLuint FramebufferResource;
		} OpenGLInternal;
#endif
#if VULKAN_BACKEND_ENABLE == 1
		struct
		{
			VkImage Image;
			VkImageView ImageView;
			VmaAllocation Memory;
			VkDeviceSize MemorySize;
			VkImageLayout ImageLayout;
			VkFramebuffer Framebuffer;
			VkDescriptorSet DescriptorSet;
		} VulkanInternal;
		struct
		{
			VkBuffer Buffer;
			VkBufferView BufferView;
			VmaAllocation Memory;
			VkBuffer StagingBuffer;
			VmaAllocation StagingMemory;
			VkDescriptorSet DescriptorSet;
			void *MappedMemory;
		} VulkanBufferInternal;
#endif
	};
};

class ETextureBackend : public ETextureBackendInfo, public ETextureBackendData
{
public:
	ETextureBackend() : ETextureBackendInfo(), ETextureBackendData()
	{
	}

	~ETextureBackend()
	{
	}

protected:
	EINLINE void ReleaseTexture()
	{
		TextureInternal.Requested = false;
		TextureInternal.Loaded = false;

		switch(EVideoBackend::GetBackend())
		{
#if DIRECTX_BACKEND_ENABLE == 1
		case EVideoBackendType::eDirectX:
			{
				if (TextureInternal.RenderTarget == true)
				{
					EDirectXHelpers::Release(DirectXInternal.RenderTargetView);
				}
				else
				{
					EDirectXHelpers::Release(DirectXInternal.DepthStencilView);
					EDirectXHelpers::Release(DirectXInternal.ShaderView);
				}
				EDirectXHelpers::Release(DirectXInternal.TextureResource);

				TextureInternal.RenderTarget = false;
			}
			break;
#endif

#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
			{
				if (OpenGLInternal.TargetType == GL_RENDERBUFFER)
				{
					EOpenGLHelpers::DeleteRenderbuffers(1, &OpenGLInternal.TextureResource);
					EOpenGLHelpers::DeleteRenderbuffers(1, &OpenGLInternal.DepthResource);
				}
				else
				{
					EOpenGLHelpers::DeleteTextures(1, &OpenGLInternal.TextureResource);
					EOpenGLHelpers::DeleteTextures(1, &OpenGLInternal.DepthResource);
				}
				EOpenGLHelpers::DeleteBuffers(1, &OpenGLInternal.BufferResource);
				EOpenGLHelpers::DeleteFramebuffers(1, &OpenGLInternal.FramebufferResource);
			}
			break;
#endif

#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
			{
				switch(TextureInternal.TextureType)
				{
				case ETextureClassType::eTexture:
					{
						if(TextureInternal.StaticDescriptorSet == true)
						{
							EVulkanGarbage::PushTexture(TextureInternal.BusyMask, VulkanInternal.Image, VulkanInternal.ImageView, VulkanInternal.Memory, VK_NULL_HANDLE);
						}
						else
						{
							EVulkanGarbage::PushTexture(TextureInternal.BusyMask, VulkanInternal.Image, VulkanInternal.ImageView, VulkanInternal.Memory, VulkanInternal.DescriptorSet);
						}
						VulkanInternal.Image = VK_NULL_HANDLE;
						VulkanInternal.ImageView = VK_NULL_HANDLE;
						VulkanInternal.Memory = nullptr;
						VulkanInternal.DescriptorSet = VK_NULL_HANDLE;
					}
					break;

				case ETextureClassType::eTextureBuffer:
					{
						EVulkanGarbage::PushBuffer(TextureInternal.BusyMask, VulkanBufferInternal.Buffer, VulkanBufferInternal.BufferView, VulkanBufferInternal.Memory);
						EVulkanGarbage::PushBuffer(TextureInternal.BusyMask, VulkanBufferInternal.StagingBuffer, VK_NULL_HANDLE, VulkanBufferInternal.StagingMemory);
						VulkanBufferInternal.Buffer = VK_NULL_HANDLE;
						VulkanBufferInternal.BufferView = VK_NULL_HANDLE;
						VulkanBufferInternal.Memory = nullptr;
						VulkanBufferInternal.StagingBuffer = VK_NULL_HANDLE;
						VulkanBufferInternal.StagingMemory = nullptr;
						VulkanBufferInternal.MappedMemory = nullptr;
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

	EINLINE const mu_boolean LoadFromBuffer(const mu_uint8 *DataBuffer, const mu_uint32 DataSize, const EImageViewType ImageViewType = EImageViewType::eNone)
	{
		switch (EVideoBackend::GetBackend())
		{
#if DIRECTX_BACKEND_ENABLE == 1
		case EVideoBackendType::eDirectX:
			return EDirectXTexture::LoadFromBuffer(DataBuffer, DataSize, this);
#endif

#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
			return EOpenGLTexture::LoadFromBuffer(DataBuffer, DataSize, ImageViewType, this);
#endif

#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
			return EVulkanTexture::LoadFromBuffer(DataBuffer, DataSize, ImageViewType, this);
#endif

		default: break;
		}

		return false;
	}

public:
	EINLINE const mu_boolean Create1D(const mu_uint32 Width,
		const ETextureFormat Format,
		const ETextureUsage Usage,
		const ESampler Sampler,
		const mu_uint8 *DataBuffer)
	{
		switch (EVideoBackend::GetBackend())
		{
#if DIRECTX_BACKEND_ENABLE == 1
		case EVideoBackendType::eDirectX:
			return EDirectXTexture::Create1D(Width, Format, Usage, Sampler, DataBuffer, this);
#endif

#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
			return EOpenGLTexture::Create1D(Width, Format, Usage, Sampler, DataBuffer, this);
#endif

#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
			return EVulkanTexture::Create1D(Width, Format, Usage, Sampler, DataBuffer, this);
#endif

		default: break;
		}

		return false;
	}

	EINLINE const mu_boolean Create2D(const mu_uint32 Width,
		const mu_uint32 Height,
		const ETextureFormat Format,
		const ETextureUsage Usage,
		const ESampler Sampler,
		const mu_uint8 *DataBuffer,
		const mu_uint32 Pitch)
	{
		switch (EVideoBackend::GetBackend())
		{
#if DIRECTX_BACKEND_ENABLE == 1
		case EVideoBackendType::eDirectX:
			return EDirectXTexture::Create2D(Width, Height, Format, Usage, Sampler, DataBuffer, Pitch, this);
#endif

#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
			return EOpenGLTexture::Create2D(Width, Height, Format, Usage, Sampler, DataBuffer, Pitch, this);
#endif

#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
			return EVulkanTexture::Create2D(Width, Height, Format, Usage, Sampler, DataBuffer, Pitch, this);
#endif

		default: break;
		}

		return false;
	}

	EINLINE const mu_boolean Create2DArray(const mu_uint32 Width,
		const mu_uint32 Height,
		const mu_uint32 Layers,
		const ETextureFormat Format,
		const ETextureUsage Usage,
		const ESampler Sampler,
		const mu_uint8 *DataBuffer,
		const mu_uint32 Pitch)
	{
		switch (EVideoBackend::GetBackend())
		{
#if DIRECTX_BACKEND_ENABLE == 1
		case EVideoBackendType::eDirectX:
			return EDirectXTexture::Create2DArray(Width, Height, Layers, Format, Usage, Sampler, DataBuffer, Pitch, this);
#endif

#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
			return EOpenGLTexture::Create2DArray(Width, Height, Layers, Format, Usage, Sampler, DataBuffer, Pitch, this);
#endif

#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
			return EVulkanTexture::Create2DArray(Width, Height, Layers, Format, Usage, Sampler, DataBuffer, Pitch, this);
#endif

		default: break;
		}

		return false;
	}

	EINLINE void Flush(EStagingBufferBackend *StagingBuffer, const mu_size StagingOffset, const mu_size BufferOffset, const mu_size Size, const ETextureFormat Format = ETextureFormat::eR8_Signed)
	{
		switch (EVideoBackend::GetBackend())
		{
#if DIRECTX_BACKEND_ENABLE == 1
		case EVideoBackendType::eDirectX:
			{
				const mu_uint8 *InputBuffer = reinterpret_cast<mu_uint8*>(StagingBuffer->BufferInternal.MappedMemory);
				const mu_uint32 Width = TextureInternal.Width;
				const mu_uint32 Height =TextureInternal.Height;
				const mu_uint32 FlatSize = EDirectXTexture::RetrieveFormatSize(Format);
				const mu_uint32 Pitch = Width * FlatSize;
				const mu_uint32 resourceIndex = D3D11CalcSubresource(0, 0, 1);

				D3D11_MAPPED_SUBRESOURCE mappedResource = { 0 };

				if (SUCCEEDED(EDirectXInit::DeviceContext->Map(DirectXInternal.TextureResource, resourceIndex, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
				{
					if (mappedResource.RowPitch == Width * FlatSize)
					{
						mu_memcpy(mappedResource.pData, InputBuffer, Width * Height * FlatSize);
					}
					else
					{
						for (mu_uint32 n = 0; n < Height; ++n)
						{
							mu_memcpy((mu_uint8*)mappedResource.pData + n * mappedResource.RowPitch, InputBuffer + (n * Pitch), Width * FlatSize);
						}
					}

					EDirectXInit::DeviceContext->Unmap(DirectXInternal.TextureResource, resourceIndex);
				}
			}
			break;
#endif

#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
			{
				EOpenGLPipeline::SetUploadTexture(nullptr);
				glBindTexture(OpenGLInternal.TargetType, OpenGLInternal.TextureResource);
				glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

				switch(OpenGLInternal.TargetType)
				{
				case GL_TEXTURE_2D:
					{
						glTexSubImage2D(OpenGLInternal.TargetType, 0, 0, 0, TextureInternal.Width, TextureInternal.Height, EOpenGLTexture::RetrieveFlatFormat(Format), EOpenGLTexture::RetrieveScalarFormat(Format), StagingBuffer->BufferInternal.MappedMemory);
					}
					break;
				}
				EOpenGLError::ReportCheck();

				glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
				glBindTexture(OpenGLInternal.TargetType, 0);
			}
			break;
#endif

#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
			{
				switch (TextureInternal.TextureType)
				{
				case ETextureClassType::eTexture:
					{
						const mu_uint32 index = EVulkan::CurrentCommandBufferIndex;

						VkBufferImageCopy bufferRegion = {};
						bufferRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
						bufferRegion.imageSubresource.mipLevel = 0;
						bufferRegion.imageSubresource.baseArrayLayer = 0;
						bufferRegion.imageSubresource.layerCount = TextureInternal.Depth;
						bufferRegion.imageExtent.width = TextureInternal.Width;
						bufferRegion.imageExtent.height = TextureInternal.Height;
						bufferRegion.imageExtent.depth = TextureInternal.Depth;

						EVulkanTexture::TransferCPUtoGPU(EVulkan::TransferBuffers[index],
							StagingBuffer->VulkanInternal.StagingBuffer,
							VulkanInternal.Image,
							VulkanInternal.ImageLayout,
							VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
							1,
							&bufferRegion,
							0,
							1,
							0,
							1);

						EVulkan::MarkBusyMask(TextureInternal.BusyMask, index);
					}
					break;

				case ETextureClassType::eTextureBuffer:
					{
						EVulkanMemory::FlushMemory(VulkanBufferInternal.StagingMemory, 0, Size);

						const mu_uint32 index = EVulkan::CurrentCommandBufferIndex;
#if !defined(__ANDROID__)
						VkBufferCopy copyRegion[1] = { {} };
						copyRegion[0].srcOffset = StagingOffset;
						copyRegion[0].dstOffset = BufferOffset;
						copyRegion[0].size = Size;

						vkCmdCopyBuffer(EVulkan::TransferBuffers[index], VulkanBufferInternal.StagingBuffer, VulkanBufferInternal.Buffer, mu_countof(copyRegion), copyRegion);
#endif

						EVulkan::MarkBusyMask(TextureInternal.BusyMask, index);
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

	EINLINE void FlushAsTexture2D(EStagingBufferBackend *StagingBuffer, const mu_size StagingOffset,
		const mu_uint32 OffsetX, const mu_uint32 OffsetY,
		const mu_uint32 Width, const mu_uint32 Height,
		const ETextureFormat Format, const mu_uint32 Pitch)
	{
		if (Width == 0 || Height == 0) return;

		switch (EVideoBackend::GetBackend())
		{
#if DIRECTX_BACKEND_ENABLE == 1
		case EVideoBackendType::eDirectX:
		{
			const mu_uint8 *InputBuffer = reinterpret_cast<mu_uint8*>(StagingBuffer->BufferInternal.MappedMemory) + StagingOffset;
			const mu_uint32 FlatSize = EDirectXTexture::RetrieveFormatSize(Format);

			if (DirectXInternal.Usage == D3D11_USAGE_DEFAULT)
			{
				D3D11_BOX destBox = { OffsetX, OffsetY, 0, OffsetX + Width, OffsetY + Height, 1 };
				EDirectXInit::DeviceContext->UpdateSubresource(DirectXInternal.TextureResource, 0, &destBox, InputBuffer, Pitch, 0);
			}
			else
			{
				D3D11_MAPPED_SUBRESOURCE mappedResource = {};
				HRESULT result = EDirectXInit::DeviceContext->Map(DirectXInternal.TextureResource, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
				if (FAILED(result))
				{
					return;
				}

				if (mappedResource.RowPitch == Pitch &&
					OffsetX == 0 &&
					Width == TextureInternal.Width)
				{
					mu_uint8 *OutputBuffer = reinterpret_cast<mu_uint8*>(mappedResource.pData) + mappedResource.RowPitch * OffsetY;
					mu_memcpy(OutputBuffer, InputBuffer, Height * Pitch);
				}
				else
				{
					mu_uint8 *OutputBuffer = reinterpret_cast<mu_uint8*>(mappedResource.pData) + mappedResource.RowPitch * OffsetY + FlatSize * OffsetX;
					for (mu_uint32 y = 0; y < Height; ++y)
					{
						mu_memcpy(OutputBuffer, InputBuffer, Pitch);

						InputBuffer += Pitch;
						OutputBuffer += mappedResource.RowPitch;
					}
				}

				EDirectXInit::DeviceContext->Unmap(DirectXInternal.TextureResource, 0);
			}
		}
		break;
#endif

#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
		{
			EOpenGLPipeline::SetUploadTexture(nullptr);
			glBindTexture(GL_TEXTURE_2D, OpenGLInternal.TextureResource);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glTexSubImage2D(GL_TEXTURE_2D, 0, OffsetX, OffsetY, Width, Height, EOpenGLTexture::RetrieveFlatFormat(Format), EOpenGLTexture::RetrieveScalarFormat(Format), reinterpret_cast<mu_uint8*>(StagingBuffer->BufferInternal.MappedMemory) + StagingOffset);
			EOpenGLError::ReportCheck();
			glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		break;
#endif

#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
		{
			switch (TextureInternal.TextureType)
			{
			case ETextureClassType::eTexture:
			{
				const mu_uint32 index = EVulkan::CurrentCommandBufferIndex;

				VkBufferImageCopy bufferRegion = {};
				bufferRegion.bufferOffset = StagingOffset;
				bufferRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				bufferRegion.imageSubresource.mipLevel = 0;
				bufferRegion.imageSubresource.baseArrayLayer = 0;
				bufferRegion.imageSubresource.layerCount = 1;
				bufferRegion.imageOffset.x = OffsetX;
				bufferRegion.imageOffset.y = OffsetY;
				bufferRegion.imageOffset.z = 0;
				bufferRegion.imageExtent.width = Width;
				bufferRegion.imageExtent.height = Height;
				bufferRegion.imageExtent.depth = 1;

				EVulkanTexture::TransferCPUtoGPU(EVulkan::TransferBuffers[index],
					StagingBuffer->VulkanInternal.StagingBuffer,
					VulkanInternal.Image,
					VulkanInternal.ImageLayout,
					VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
					1,
					&bufferRegion,
					0,
					1,
					0,
					1);
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

	EINLINE void FlushAsTexture2DArray(EStagingBufferBackend *StagingBuffer, const mu_size StagingOffset,
		const mu_uint32 OffsetX, const mu_uint32 OffsetY, const mu_uint32 OffsetZ,
		const mu_uint32 Width, const mu_uint32 Height, const mu_uint32 Depth,
		const ETextureFormat Format, const mu_uint32 Pitch)
	{
		if (Width == 0 || Height == 0 || Depth == 0) return;

		switch (EVideoBackend::GetBackend())
		{
#if DIRECTX_BACKEND_ENABLE == 1
		case EVideoBackendType::eDirectX:
			{
				const mu_uint8 *InputBuffer = reinterpret_cast<mu_uint8*>(StagingBuffer->BufferInternal.MappedMemory) + StagingOffset;
				const mu_uint32 FlatSize = EDirectXTexture::RetrieveFormatSize(Format);

				if (DirectXInternal.Usage == D3D11_USAGE_DEFAULT)
				{
					for (mu_uint32 n = 0;n < Depth;++n)
					{
						const mu_uint32 resourceIndex = D3D11CalcSubresource(0, OffsetZ + n, 1);

						D3D11_BOX destBox = { OffsetX, OffsetY, 0, OffsetX + Width, OffsetY + Height, 1 };
						EDirectXInit::DeviceContext->UpdateSubresource(DirectXInternal.TextureResource, resourceIndex, &destBox, InputBuffer, Pitch, Height * Pitch);

						InputBuffer += Height * Pitch;
					}
				}
				else
				{
					for (mu_uint32 n = 0; n < Depth; ++n)
					{
						const mu_uint32 resourceIndex = D3D11CalcSubresource(0, OffsetZ + n, 1);

						D3D11_MAPPED_SUBRESOURCE mappedResource = {};
						HRESULT result = EDirectXInit::DeviceContext->Map(DirectXInternal.TextureResource, resourceIndex, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
						if (FAILED(result))
						{
							return;
						}

						if (mappedResource.RowPitch == Pitch &&
							OffsetX == 0 &&
							Width == TextureInternal.Width)
						{
							mu_uint8 *OutputBuffer = reinterpret_cast<mu_uint8*>(mappedResource.pData) + mappedResource.RowPitch * OffsetY;
							mu_memcpy(OutputBuffer, InputBuffer, Height * Pitch);
						}
						else
						{
							mu_uint8 *OutputBuffer = reinterpret_cast<mu_uint8*>(mappedResource.pData) + mappedResource.RowPitch * OffsetY + FlatSize * OffsetX;
							for (mu_uint32 y = 0; y < Height; ++y)
							{
								mu_memcpy(OutputBuffer, InputBuffer, Pitch);

								InputBuffer += Pitch;
								OutputBuffer += mappedResource.RowPitch;
							}
						}

						EDirectXInit::DeviceContext->Unmap(DirectXInternal.TextureResource, resourceIndex);
					}
				}
			}
			break;
#endif

#if OPENGL_BACKEND_ENABLE == 1
		case EVideoBackendType::eOpenGL:
			{
				EOpenGLPipeline::SetUploadTexture(nullptr);
				glBindTexture(GL_TEXTURE_2D_ARRAY, OpenGLInternal.TextureResource);
				glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
				glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, OffsetX, OffsetY, OffsetZ, Width, Height, Depth, EOpenGLTexture::RetrieveFlatFormat(Format), EOpenGLTexture::RetrieveScalarFormat(Format), reinterpret_cast<mu_uint8*>(StagingBuffer->BufferInternal.MappedMemory) + StagingOffset);
				EOpenGLError::ReportCheck();
				glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
				glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
			}
			break;
#endif

#if VULKAN_BACKEND_ENABLE == 1
		case EVideoBackendType::eVulkan:
			{
				switch (TextureInternal.TextureType)
				{
				case ETextureClassType::eTexture:
					{
						const mu_uint32 index = EVulkan::CurrentCommandBufferIndex;

						VkBufferImageCopy bufferRegion = {};
						bufferRegion.bufferOffset = StagingOffset;
						bufferRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
						bufferRegion.imageSubresource.mipLevel = 0;
						bufferRegion.imageSubresource.baseArrayLayer = OffsetZ;
						bufferRegion.imageSubresource.layerCount = Depth;
						bufferRegion.imageOffset.x = OffsetX;
						bufferRegion.imageOffset.y = OffsetY;
						bufferRegion.imageOffset.z = 0;
						bufferRegion.imageExtent.width = Width;
						bufferRegion.imageExtent.height = Height;
						bufferRegion.imageExtent.depth = 1;

						EVulkanTexture::TransferCPUtoGPU(EVulkan::TransferBuffers[index],
							StagingBuffer->VulkanInternal.StagingBuffer,
							VulkanInternal.Image,
							VulkanInternal.ImageLayout,
							VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
							1,
							&bufferRegion,
							0,
							1,
							0,
							1);
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

public:
	EINLINE const mu_boolean IsLoaded() const
	{
		return TextureInternal.Loaded;
	}

	EINLINE const mu_boolean IsRequested() const
	{
		return TextureInternal.Requested;
	}

	EINLINE const mu_boolean RequestLoad()
	{
		return TextureInternal.Requested.exchange(true);
	}

	EINLINE mu_uint32 GetWidth() const
	{
		return TextureInternal.Width;
	}

	EINLINE mu_uint32 GetHeight() const
	{
		return TextureInternal.Height;
	}

	EINLINE mu_float GetFloatWidth() const
	{
		return static_cast<mu_float>(TextureInternal.Width);
	}

	EINLINE mu_float GetFloatHeight() const
	{
		return static_cast<mu_float>(TextureInternal.Height);
	}
};

#endif