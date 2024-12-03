#include "stdafx.h"

#include "mu_vulkan_texture_formats.h"

namespace EVulkanTexture
{
	const VkFormat RetrieveFormat(const ETextureFormat format)
	{
		return VulkanFormats[ECastEnum(format)];
	}

	const mu_size RetrieveFormatSize(const ETextureFormat format)
	{
		return VulkanFormatSizes[ECastEnum(format)];
	}

#if ELION_TEXTURETYPE_FORMAT == ELION_TEXTURETYPE_DDS
	EINLINE mu_boolean LoadFromBuffer(gli::texture &Texture,
		VkImage &Image,
		VkImageView &ImageView,
		VkImageLayout &ImageLayout,
		VmaAllocation &Memory,
		VmaAllocationInfo &MemoryInfo,
		VkExtent3D &ImageExtent,
		const EImageViewType ImageViewType = EImageViewType::eNone)
	{
		const mu_size memorySize = Texture.size();

		EStagingBufferBackend stagingBuffer;

		if (stagingBuffer.Allocate(memorySize, true) == false)
		{
			return false;
		}

		mu_memcpy(stagingBuffer.BufferInternal.MappedMemory, Texture.data(), memorySize);

		stagingBuffer.Flush(0, VK_WHOLE_SIZE);

		const mu_size layerCount = Texture.layers();
		const mu_size faceCount = Texture.faces();
		const mu_size levelCount = Texture.levels();

		VkImageType imageType = VK_IMAGE_TYPE_MAX_ENUM;
		switch (Texture.target())
		{
		case gli::TARGET_1D:
		case gli::TARGET_1D_ARRAY:
			{
				imageType = VK_IMAGE_TYPE_1D;
			}
			break;

		case gli::TARGET_2D:
		case gli::TARGET_2D_ARRAY:
		case gli::TARGET_CUBE:
		case gli::TARGET_CUBE_ARRAY:
			{
				imageType = VK_IMAGE_TYPE_2D;
			}
			break;

		case gli::TARGET_3D:
			{
				imageType = VK_IMAGE_TYPE_3D;
			}
			break;
}

		// Initialize ImageExtent
		{
			const gli::texture::extent_type TextureExtent(Texture.extent(0));
			ImageExtent = {
			static_cast<mu_uint32>(TextureExtent.x),
			static_cast<mu_uint32>(TextureExtent.y),
			static_cast<mu_uint32>(TextureExtent.z) };
		}

		if (EVulkanMemory::CreateImage(Image,
			Memory,
			MemoryInfo,
			(VkFormat)Texture.format(),
			levelCount,
			layerCount,
			imageType,
			ImageExtent,
			VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			VMA_MEMORY_USAGE_GPU_ONLY) == false)
		{
			mu_error("Failed to create image.");
			return false;
		}

		std::vector<VkBufferImageCopy> bufferCopyRegions(layerCount * faceCount * levelCount);
		mu_size offset = 0;

		auto copyIter = bufferCopyRegions.begin();
		for (mu_size layer = 0; layer < layerCount; ++layer)
		{
			for (mu_size face = 0; face < faceCount; ++face)
			{
				for (mu_size level = 0; level < levelCount; ++level, ++copyIter)
				{
					const glm::tvec3<GLsizei> Extent(Texture.extent(level));

					VkBufferImageCopy &bufferCopyRegion = *copyIter;

					mu_zeromem(&bufferCopyRegion, sizeof(VkBufferImageCopy));
					bufferCopyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
					bufferCopyRegion.imageSubresource.mipLevel = level;
					bufferCopyRegion.imageSubresource.baseArrayLayer = layer;
					bufferCopyRegion.imageSubresource.layerCount = 1;
					bufferCopyRegion.imageExtent.width = static_cast<uint32_t>(Extent.x);
					bufferCopyRegion.imageExtent.height = static_cast<uint32_t>(Extent.y);
					bufferCopyRegion.imageExtent.depth = 1;
					bufferCopyRegion.bufferOffset = offset;

					// Increase offset into staging buffer for next level / face
					offset += Texture.size(level);
				}
			}
		}

		// Create Command Buffer
		VkCommandBuffer command = EVulkan::CreateCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, EVulkan::ShortCommandPool, true);
		if (command == nullptr)
		{
			EVulkanMemory::DestroyImage(Image, Memory);
			mu_error("Failed to create Texture image.");
			return false;
		}

		VkImageSubresourceRange subresourceRange = {};
		subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		subresourceRange.baseMipLevel = 0;
		subresourceRange.levelCount = levelCount;
		subresourceRange.baseArrayLayer = 0;
		subresourceRange.layerCount = layerCount;

		EVulkan::SetImageLayout(command, Image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, subresourceRange);

		vkCmdCopyBufferToImage(command, stagingBuffer.VulkanInternal.StagingBuffer, Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, bufferCopyRegions.size(), bufferCopyRegions.data());

		ImageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		EVulkan::SetImageLayout(command, Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, subresourceRange);

		if (EVulkan::FlushCommandBuffer<true>(command, EVulkan::ShortCommandPool, EVulkan::GraphicsQueue) == false)
		{
			EVulkanMemory::DestroyImage(Image, Memory);
			mu_error("Failed to create Texture image.");
			return false;
		}

		VkImageViewType viewType = ImageViewTypeList[ECastEnum(ImageViewType)];

		if (ImageViewType == EImageViewType::eNone)
		{
			switch (Texture.target())
			{
			case gli::TARGET_1D: viewType = VK_IMAGE_VIEW_TYPE_1D; break;
			case gli::TARGET_1D_ARRAY: viewType = VK_IMAGE_VIEW_TYPE_1D_ARRAY; break;
			case gli::TARGET_2D: viewType = VK_IMAGE_VIEW_TYPE_2D; break;
			case gli::TARGET_2D_ARRAY: viewType = VK_IMAGE_VIEW_TYPE_2D_ARRAY; break;
			case gli::TARGET_CUBE: viewType = VK_IMAGE_VIEW_TYPE_CUBE; break;
			case gli::TARGET_CUBE_ARRAY: viewType = VK_IMAGE_VIEW_TYPE_CUBE_ARRAY; break;
			case gli::TARGET_3D: viewType = VK_IMAGE_VIEW_TYPE_3D; break;
			}
		}

		if (EVulkanMemory::CreateImageView(ImageView,
			Image,
			(VkFormat)Texture.format(),
			levelCount,
			layerCount,
			viewType) == false)
		{
			EVulkanMemory::DestroyImage(Image, Memory);
			mu_error("Failed to create image view.");
			return false;
		}

		return true;
	}
#else
	typedef struct user_cbdata_optimal {
		VkBufferImageCopy* region; // specify destination region in final image.
		VkDeviceSize offset;       // Offset of current level in staging buffer
		ktx_uint32_t numFaces;
		ktx_uint32_t numLayers;
#if !defined(NDEBUG)
		VkBufferImageCopy* regionsArrayEnd;   //  "
#endif
	} user_cbdata_optimal;

	KTX_error_code KTXAPIENTRY
		optimalTilingCallback(int miplevel, int face,
			int width, int height, int depth,
			ktx_uint32_t faceLodSize,
			void* pixels, void* userdata)
	{
		user_cbdata_optimal* ud = (user_cbdata_optimal*)userdata;

		// Set up copy to destination region in final image
#if !defined(NDEBUG)
		mu_assert(ud->region < ud->regionsArrayEnd);
#endif
		ud->region->bufferOffset = ud->offset;
		ud->offset += faceLodSize;
		// XXX Handle row padding for uncompressed textures. KTX specifies
		// GL_UNPACK_ALIGNMENT of 4 so need to pad this from actual width.
		// That means I need the element size and group size for the format
		// to calculate bufferRowLength.
		ud->region->bufferRowLength = 0;
		ud->region->bufferImageHeight = 0;
		ud->region->imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		ud->region->imageSubresource.mipLevel = miplevel;
		ud->region->imageSubresource.baseArrayLayer = face;
		ud->region->imageSubresource.layerCount = ud->numLayers * ud->numFaces;
		ud->region->imageOffset.x = 0;
		ud->region->imageOffset.y = 0;
		ud->region->imageOffset.z = 0;
		ud->region->imageExtent.width = width;
		ud->region->imageExtent.height = height;
		ud->region->imageExtent.depth = depth;

		ud->region += 1; // XXX Probably need some check of the array length.

		return KTX_SUCCESS;
	}

	EINLINE mu_boolean LoadFromBuffer(ktxTexture *Texture,
		VkImage &Image,
		VkImageView &ImageView,
		VkImageLayout &ImageLayout,
		VmaAllocation &Memory,
		VmaAllocationInfo &MemoryInfo,
		VkExtent3D &ImageExtent,
		const EImageViewType ImageViewType = EImageViewType::eNone)
	{
		if (Texture == nullptr ||
			Texture->pData == nullptr)
		{
			return false;
		}

		const mu_size memorySize = ktxTexture_GetSize(Texture);

		EStagingBufferBackend stagingBuffer;

		if (stagingBuffer.Allocate(memorySize, true) == false)
		{
			return false;
		}

		mu_memcpy(stagingBuffer.BufferInternal.MappedMemory, ktxTexture_GetData(Texture), memorySize);

		stagingBuffer.Flush(0, VK_WHOLE_SIZE);

		const mu_size layerCount = Texture->numLayers;
		const mu_size faceCount = Texture->numFaces;
		const mu_size levelCount = Texture->numLevels;

		VkImageType imageType = VK_IMAGE_TYPE_MAX_ENUM;
		switch (Texture->numDimensions)
		{
		case 1:
			{
				imageType = VK_IMAGE_TYPE_1D;
			}
			break;

		case 2:
			{
				imageType = VK_IMAGE_TYPE_2D;
			}
			break;

		case 3:
			{
				imageType = VK_IMAGE_TYPE_3D;
			}
			break;
		}

		// Initialize ImageExtent
		{
			ImageExtent = {
			static_cast<mu_uint32>(Texture->baseWidth),
			static_cast<mu_uint32>(Texture->baseHeight),
			static_cast<mu_uint32>(Texture->baseDepth) };
		}

		const VkFormat textureFormat = ktxTexture_GetVkFormat(Texture);
		if (EVulkanMemory::CreateImage(Image,
			Memory,
			MemoryInfo,
			textureFormat,
			levelCount,
			layerCount,
			imageType,
			ImageExtent,
			VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			VMA_MEMORY_USAGE_GPU_ONLY) == false)
		{
			mu_error("Failed to create image.");
			return false;
		}

		std::vector<VkBufferImageCopy> bufferCopyRegions(levelCount);

		user_cbdata_optimal cbData;
		cbData.offset = 0;
		cbData.region = bufferCopyRegions.data();
		cbData.numFaces = faceCount;
		cbData.numLayers = layerCount;
#if !defined(NDEBUG)
		cbData.regionsArrayEnd = bufferCopyRegions.data() + levelCount;
#endif

		ktxTexture_IterateLevels(Texture, optimalTilingCallback, &cbData);

		// Create Command Buffer
		VkCommandBuffer command = EVulkan::CreateCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, EVulkan::ShortCommandPool, true);
		if (command == nullptr)
		{
			EVulkanMemory::DestroyImage(Image, Memory);
			mu_error("Failed to create Texture image.");
			return false;
		}

		VkImageSubresourceRange subresourceRange = {};
		subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		subresourceRange.baseMipLevel = 0;
		subresourceRange.levelCount = levelCount;
		subresourceRange.baseArrayLayer = 0;
		subresourceRange.layerCount = layerCount;

		EVulkan::SetImageLayout(command, Image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, subresourceRange);

		vkCmdCopyBufferToImage(command, stagingBuffer.VulkanInternal.StagingBuffer, Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, bufferCopyRegions.size(), bufferCopyRegions.data());

		ImageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		EVulkan::SetImageLayout(command, Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, subresourceRange);

		if (EVulkan::FlushCommandBuffer<true>(command, EVulkan::ShortCommandPool, EVulkan::GraphicsQueue) == false)
		{
			EVulkanMemory::DestroyImage(Image, Memory);
			mu_error("Failed to create Texture image.");
			return false;
		}

		VkImageViewType viewType = ImageViewTypeList[ECastEnum(ImageViewType)];

		if (ImageViewType == EImageViewType::eNone)
		{
			if (Texture->isArray)
			{
				if (Texture->isCubemap)
				{
					viewType = VK_IMAGE_VIEW_TYPE_CUBE_ARRAY;
				}
				else
				{
					switch (Texture->numDimensions)
					{
					case 1: viewType = VK_IMAGE_VIEW_TYPE_1D_ARRAY; break;
					case 2: viewType = VK_IMAGE_VIEW_TYPE_2D_ARRAY; break;
					}
				}
			}
			else
			{
				switch (Texture->numDimensions)
				{
				case 1: viewType = VK_IMAGE_VIEW_TYPE_1D; break;
				case 2: viewType = VK_IMAGE_VIEW_TYPE_2D; break;
				case 3: viewType = VK_IMAGE_VIEW_TYPE_3D; break;
				}
			}
		}

		if (EVulkanMemory::CreateImageView(ImageView,
			Image,
			textureFormat,
			levelCount,
			layerCount,
			viewType) == false)
		{
			EVulkanMemory::DestroyImage(Image, Memory);
			mu_error("Failed to create image view.");
			return false;
		}

		return true;
	}
#endif

	const mu_boolean LoadFromBuffer(const mu_uint8 *DataBuffer, const mu_uint32 DataSize, const EImageViewType ImageViewType, ETextureBackend *Texture)
	{
#if ELION_TEXTURETYPE_FORMAT == ELION_TEXTURETYPE_DDS
		gli::texture &texture = gli::load_dds((char*)DataBuffer, (mu_size)DataSize);
		if (texture.empty() == true)
		{
			return false;
		}

		VkExtent3D ImageExtent = {};
		VmaAllocationInfo MemoryInfo = {};
		if (EVulkanTexture::LoadFromBuffer(texture,
			Texture->VulkanInternal.Image,
			Texture->VulkanInternal.ImageView,
			Texture->VulkanInternal.ImageLayout,
			Texture->VulkanInternal.Memory,
			MemoryInfo,
			ImageExtent,
			ImageViewType) == false)
		{
			return false;
		}
#else
		ktxTexture *texture = nullptr;
		KTX_error_code_t error = ktxTexture_CreateFromMemory(DataBuffer, DataSize, KTX_TEXTURE_CREATE_LOAD_IMAGE_DATA_BIT, &texture);
		if(error != KTX_SUCCESS)
		{
			return false;
		}

		VkExtent3D ImageExtent = {};
		VmaAllocationInfo MemoryInfo = {};
		const mu_boolean isUploaded = EVulkanTexture::LoadFromBuffer(texture,
			Texture->VulkanInternal.Image,
			Texture->VulkanInternal.ImageView,
			Texture->VulkanInternal.ImageLayout,
			Texture->VulkanInternal.Memory,
			MemoryInfo,
			ImageExtent,
			ImageViewType);
		ktxTexture_Destroy(texture);

		if(isUploaded == false)
		{
			return false;
		}
#endif

		Texture->TextureInternal.StaticDescriptorSet = false;

		Texture->VulkanInternal.DescriptorSet = EVulkanDescriptor::CreateTextureDescriptorSet();
		if (Texture->VulkanInternal.DescriptorSet == VK_NULL_HANDLE)
		{
			return false;
		}

		EVulkanDescriptor::UpdateTextureDescriptorSet(Texture);

		return true;
	}

	const mu_boolean CreateBones(ETextureBackend *Texture)
	{
		VmaAllocationInfo memoryInfo = {};

		if (EVulkan::SkeletonTexelBufferEnabled == true)
		{
			Texture->TextureInternal.TextureType = ETextureClassType::eTextureBuffer;

#if defined(__ANDROID__)
			if (EVulkanMemory::CreateBuffer(Texture->VulkanBufferInternal.StagingBuffer,
				Texture->VulkanBufferInternal.StagingMemory,
				memoryInfo,
				SkeletonMemorySize,
				VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT,
				VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
				VMA_MEMORY_USAGE_CPU_TO_GPU,
				0,
				VMA_ALLOCATION_CREATE_MAPPED_BIT) == false)
			{
				return false;
			}

			Texture->VulkanBufferInternal.MappedMemory = memoryInfo.pMappedData;

			if (EVulkanMemory::CreateBufferView(Texture->VulkanBufferInternal.BufferView,
				Texture->VulkanBufferInternal.StagingBuffer,
				RetrieveFormat(SKELETON_TEXTURE_FORMAT),
				SkeletonMemorySize) == false)
			{
				return false;
		}
#else
			if (EVulkanMemory::CreateBuffer(Texture->VulkanBufferInternal.Buffer,
				Texture->VulkanBufferInternal.Memory,
				memoryInfo,
				SkeletonMemorySize,
				VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
				VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
				VMA_MEMORY_USAGE_GPU_ONLY) == false)
			{
				return false;
			}

			if (EVulkanMemory::CreateBuffer(Texture->VulkanBufferInternal.StagingBuffer,
				Texture->VulkanBufferInternal.StagingMemory,
				memoryInfo,
				SkeletonMemorySize,
				VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
				VMA_MEMORY_USAGE_CPU_ONLY,
				0,
				VMA_ALLOCATION_CREATE_MAPPED_BIT) == false)
			{
				return false;
			}

			Texture->VulkanBufferInternal.MappedMemory = memoryInfo.pMappedData;

			if (EVulkanMemory::CreateBufferView(Texture->VulkanBufferInternal.BufferView,
				Texture->VulkanBufferInternal.Buffer,
				RetrieveFormat(SKELETON_TEXTURE_FORMAT),
				SkeletonMemorySize) == false)
			{
				return false;
			}
#endif

			Texture->TextureInternal.StaticDescriptorSet = true;
			Texture->TextureInternal.Width = SKELETON_TEXTURE_WIDTH;
			Texture->TextureInternal.Height = SKELETON_TEXTURE_HEIGHT;
			Texture->TextureInternal.Depth = 1;
			Texture->TextureInternal.Components = GetTextureComponents(SKELETON_TEXTURE_FORMAT);
			Texture->TextureInternal.Sampler = ESampler::eUcVcWcMinNMagNMipN_Anisotropy;

			Texture->VulkanBufferInternal.DescriptorSet = EVulkanDescriptor::CreateBonesDescriptorSet();
			if (Texture->VulkanBufferInternal.DescriptorSet == VK_NULL_HANDLE)
			{
				return false;
			}

			VkWriteDescriptorSet writeSet[1];

			writeSet[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			writeSet[0].pNext = nullptr;
			writeSet[0].dstSet = Texture->VulkanBufferInternal.DescriptorSet;
			writeSet[0].dstBinding = EVulkanBinding::BonesBinding[0].binding;
			writeSet[0].dstArrayElement = 0;
			writeSet[0].descriptorCount = 1;
			writeSet[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER;
			writeSet[0].pTexelBufferView = &Texture->VulkanBufferInternal.BufferView;

			vkUpdateDescriptorSets(EVulkan::Device, mu_countof(writeSet), writeSet, 0, nullptr);
		}
		else
		{
			Texture->TextureInternal.TextureType = ETextureClassType::eTexture;

			VmaAllocationInfo memoryInfo = {};

			if (CreateImageDynamic(Texture->VulkanInternal.Image,
				Texture->VulkanInternal.ImageView,
				Texture->VulkanInternal.ImageLayout,
				Texture->VulkanInternal.Memory,
				memoryInfo,
				Texture->VulkanInternal.MemorySize,
				SKELETON_TEXTURE_WIDTH, SKELETON_TEXTURE_HEIGHT, 1,
				RetrieveFormat(SKELETON_TEXTURE_FORMAT),
				VK_IMAGE_TYPE_2D,
				VK_IMAGE_VIEW_TYPE_2D) == false)
			{
				return false;
			}

			Texture->TextureInternal.StaticDescriptorSet = false;
			Texture->TextureInternal.Width = SKELETON_TEXTURE_WIDTH;
			Texture->TextureInternal.Height = SKELETON_TEXTURE_HEIGHT;
			Texture->TextureInternal.Depth = 1;
			Texture->TextureInternal.Components = GetTextureComponents(SKELETON_TEXTURE_FORMAT);
			Texture->TextureInternal.Sampler = ESampler::eUcVcWcMinNMagNMipN_Anisotropy;

			Texture->VulkanInternal.DescriptorSet = EVulkanDescriptor::CreateBonesDescriptorSet();
			if (Texture->VulkanInternal.DescriptorSet == VK_NULL_HANDLE)
			{
				return false;
			}

			VkDescriptorImageInfo imageInfo[1];

			imageInfo[0].sampler = EVulkanSampler::Get(Texture->TextureInternal.Sampler);
			imageInfo[0].imageView = Texture->VulkanInternal.ImageView;
			imageInfo[0].imageLayout = Texture->VulkanInternal.ImageLayout;

			VkWriteDescriptorSet writeSet[1];

			writeSet[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			writeSet[0].pNext = nullptr;
			writeSet[0].dstSet = Texture->VulkanInternal.DescriptorSet;
			writeSet[0].dstBinding = EVulkanBinding::BonesBinding[0].binding;
			writeSet[0].dstArrayElement = 0;
			writeSet[0].descriptorCount = 1;
			writeSet[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			writeSet[0].pImageInfo = &imageInfo[0];

			vkUpdateDescriptorSets(EVulkan::Device, mu_countof(writeSet), writeSet, 0, nullptr);

			VkCommandBuffer command = EVulkan::CreateCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, EVulkan::ShortCommandPool, true);
			if (command == nullptr)
			{
				return false;
			}

			VkImageSubresourceRange subresourceRange = {};
			subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			subresourceRange.baseMipLevel = 0;
			subresourceRange.levelCount = 1;
			subresourceRange.baseArrayLayer = 0;
			subresourceRange.layerCount = 1;

			EVulkan::SetImageLayout(command, Texture->VulkanInternal.Image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, subresourceRange);
			Texture->VulkanInternal.ImageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

			if (EVulkan::FlushCommandBuffer<true>(command, EVulkan::ShortCommandPool, EVulkan::GraphicsQueue) == false)
			{
				return false;
			}
		}

		Texture->TextureInternal.Loaded = true;

		return true;
	}

	const mu_boolean CreateShadowMap(ETextureBackend *Texture, VkDescriptorSet DescriptorSet)
	{
		constexpr ETextureFormat Format = SHADOWMAP_FORMAT;
		constexpr mu_uint32 Width = SHADOWMAP_RESOLUTION;
		constexpr mu_uint32 Height = SHADOWMAP_RESOLUTION;

		VmaAllocationInfo memoryInfo = {};

		if (CreateImageDynamic(Texture->VulkanInternal.Image,
			Texture->VulkanInternal.ImageView,
			Texture->VulkanInternal.ImageLayout,
			Texture->VulkanInternal.Memory,
			memoryInfo,
			Texture->VulkanInternal.MemorySize,
			Width, Height, 1,
			RetrieveFormat(Format),
			VK_IMAGE_TYPE_2D,
			VK_IMAGE_VIEW_TYPE_2D,
			VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
			VK_IMAGE_ASPECT_DEPTH_BIT) == false)
		{
			return false;
		}

		Texture->TextureInternal.StaticDescriptorSet = false;
		Texture->TextureInternal.Width = Width;
		Texture->TextureInternal.Height = Height;
		Texture->TextureInternal.Depth = 1;
		Texture->TextureInternal.Components = GetTextureComponents(Format);
		Texture->TextureInternal.Sampler = SHADOWMAP_SAMPLER;

		Texture->VulkanInternal.DescriptorSet = EVulkanDescriptor::CreateTextureDescriptorSet();
		if (Texture->VulkanInternal.DescriptorSet == VK_NULL_HANDLE)
		{
			return false;
		}

		EVulkanDescriptor::UpdateTextureDescriptorSet(Texture);

		VkDescriptorImageInfo imageInfo[1];
		imageInfo[0].sampler = EVulkanSampler::Get(Texture->TextureInternal.Sampler);
		imageInfo[0].imageView = Texture->VulkanInternal.ImageView;
		imageInfo[0].imageLayout = Texture->VulkanInternal.ImageLayout;

		VkWriteDescriptorSet writeSet[1];

		writeSet[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		writeSet[0].pNext = nullptr;
		writeSet[0].dstSet = DescriptorSet;
		writeSet[0].dstBinding = EVulkanBinding::ShadowBinding[0].binding;
		writeSet[0].dstArrayElement = 0;
		writeSet[0].descriptorCount = 1;
		writeSet[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		writeSet[0].pImageInfo = imageInfo;

		vkUpdateDescriptorSets(EVulkan::Device, mu_countof(writeSet), writeSet, 0, nullptr);

		VkCommandBuffer command = EVulkan::CreateCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, EVulkan::ShortCommandPool, true);
		if (command == nullptr)
		{
			return false;
		}

		VkImageSubresourceRange subresourceRange = {};
		subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
		subresourceRange.baseMipLevel = 0;
		subresourceRange.levelCount = 1;
		subresourceRange.baseArrayLayer = 0;
		subresourceRange.layerCount = 1;

		EVulkan::SetImageLayout(command, Texture->VulkanInternal.Image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL, subresourceRange);
		Texture->VulkanInternal.ImageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;

		if (EVulkan::FlushCommandBuffer<true>(command, EVulkan::ShortCommandPool, EVulkan::GraphicsQueue) == false)
		{
			return false;
		}

		if (EVulkanMemory::CreateFramebuffer(Texture->VulkanInternal.Framebuffer, Width, Height, 1, 1, &Texture->VulkanInternal.ImageView, EVulkanPipeline::GetRenderPass(ERenderPass::eShadow)) == false)
		{
			return false;
		}

		Texture->TextureInternal.Loaded = true;

		return true;
	}

	const mu_boolean Create1D(const mu_uint32 Width,
		const ETextureFormat Format,
		const ETextureUsage Usage,
		const ESampler Sampler,
		const mu_uint8 *DataBuffer,
		ETextureBackend *Texture)
	{
		VmaAllocationInfo memoryInfo = {};

		if(CreateImageDynamic(Texture->VulkanInternal.Image,
			Texture->VulkanInternal.ImageView,
			Texture->VulkanInternal.ImageLayout,
			Texture->VulkanInternal.Memory,
			memoryInfo,
			Texture->VulkanInternal.MemorySize,
			Width, 1, 1,
			RetrieveFormat(Format),
			VK_IMAGE_TYPE_1D,
			VK_IMAGE_VIEW_TYPE_1D) == false)
		{
			return false;
		}

		Texture->TextureInternal.StaticDescriptorSet = false;
		Texture->TextureInternal.Width = Width;
		Texture->TextureInternal.Height = 1;
		Texture->TextureInternal.Depth = 1;
		Texture->TextureInternal.Components = GetTextureComponents(Format);
		Texture->TextureInternal.Sampler = Sampler;

		Texture->VulkanInternal.DescriptorSet = EVulkanDescriptor::CreateTextureDescriptorSet();
		if (Texture->VulkanInternal.DescriptorSet == VK_NULL_HANDLE)
		{
			return false;
		}

		EVulkanDescriptor::UpdateTextureDescriptorSet(Texture);

		VkCommandBuffer command = EVulkan::CreateCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, EVulkan::ShortCommandPool, true);
		if (command == nullptr)
		{
			return false;
		}

		VkImageSubresourceRange subresourceRange = {};
		subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		subresourceRange.baseMipLevel = 0;
		subresourceRange.levelCount = 1;
		subresourceRange.baseArrayLayer = 0;
		subresourceRange.layerCount = 1;

		EStagingBufferBackend stagingBuffer;

		if ((Usage == ETextureUsage::eDynamic && DataBuffer != nullptr) ||
			Usage == ETextureUsage::eStatic)
		{
			const mu_size memorySize = Width * RetrieveFormatSize(Format);
			mu_assert(DataBuffer != nullptr);

			if (stagingBuffer.Allocate(memorySize) == false)
			{
				return false;
			}

			mu_memcpy(stagingBuffer.BufferInternal.MappedMemory, DataBuffer, memorySize);

			stagingBuffer.Flush(0, VK_WHOLE_SIZE);

			VkBufferImageCopy bufferCopy = {};
			bufferCopy.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			bufferCopy.imageSubresource.mipLevel = 0;
			bufferCopy.imageSubresource.baseArrayLayer = 0;
			bufferCopy.imageSubresource.layerCount = 1;
			bufferCopy.imageExtent.width = Width;
			bufferCopy.imageExtent.height = 1;
			bufferCopy.imageExtent.depth = 1;
			bufferCopy.bufferOffset = 0;

			EVulkan::SetImageLayout(command, Texture->VulkanInternal.Image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, subresourceRange);

			vkCmdCopyBufferToImage(command, stagingBuffer.VulkanInternal.StagingBuffer, Texture->VulkanInternal.Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &bufferCopy);

			EVulkan::SetImageLayout(command, Texture->VulkanInternal.Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, subresourceRange);
		}
		else
		{
			EVulkan::SetImageLayout(command, Texture->VulkanInternal.Image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, subresourceRange);
		}

		Texture->VulkanInternal.ImageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

		if (EVulkan::FlushCommandBuffer<true>(command, EVulkan::ShortCommandPool, EVulkan::GraphicsQueue) == false)
		{
			return false;
		}

		stagingBuffer.Deallocate();
		Texture->TextureInternal.Loaded = true;

		return true;
	}

	const mu_boolean Create2D(const mu_uint32 Width,
		const mu_uint32 Height,
		const ETextureFormat Format,
		const ETextureUsage Usage,
		const ESampler Sampler,
		const mu_uint8 *DataBuffer,
		const mu_uint32 Pitch,
		ETextureBackend *Texture)
	{
		VmaAllocationInfo memoryInfo = {};

		if (CreateImageDynamic(Texture->VulkanInternal.Image,
			Texture->VulkanInternal.ImageView,
			Texture->VulkanInternal.ImageLayout,
			Texture->VulkanInternal.Memory,
			memoryInfo,
			Texture->VulkanInternal.MemorySize,
			Width, Height, 1,
			RetrieveFormat(Format),
			VK_IMAGE_TYPE_2D,
			VK_IMAGE_VIEW_TYPE_2D) == false)
		{
			return false;
		}

		Texture->TextureInternal.StaticDescriptorSet = false;
		Texture->TextureInternal.Width = Width;
		Texture->TextureInternal.Height = Height;
		Texture->TextureInternal.Depth = 1;
		Texture->TextureInternal.Components = GetTextureComponents(Format);
		Texture->TextureInternal.Sampler = Sampler;

		Texture->VulkanInternal.DescriptorSet = EVulkanDescriptor::CreateTextureDescriptorSet();
		if (Texture->VulkanInternal.DescriptorSet == VK_NULL_HANDLE)
		{
			return false;
		}

		EVulkanDescriptor::UpdateTextureDescriptorSet(Texture);

		VkCommandBuffer command = EVulkan::CreateCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, EVulkan::ShortCommandPool, true);
		if (command == nullptr)
		{
			return false;
		}

		VkImageSubresourceRange subresourceRange = {};
		subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		subresourceRange.baseMipLevel = 0;
		subresourceRange.levelCount = 1;
		subresourceRange.baseArrayLayer = 0;
		subresourceRange.layerCount = 1;

		EStagingBufferBackend stagingBuffer;

		if((Usage == ETextureUsage::eDynamic && DataBuffer != nullptr) ||
			Usage == ETextureUsage::eStatic)
		{
			const mu_size memorySize = Pitch * Height;
			mu_assert(DataBuffer != nullptr);

			if(stagingBuffer.Allocate(memorySize) == false)
			{
				return false;
			}

			mu_memcpy(stagingBuffer.BufferInternal.MappedMemory, DataBuffer, memorySize);

			stagingBuffer.Flush(0, VK_WHOLE_SIZE);

			VkBufferImageCopy bufferCopy = {};
			bufferCopy.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			bufferCopy.imageSubresource.mipLevel = 0;
			bufferCopy.imageSubresource.baseArrayLayer = 0;
			bufferCopy.imageSubresource.layerCount = 1;
			bufferCopy.imageExtent.width = Width;
			bufferCopy.imageExtent.height = Height;
			bufferCopy.imageExtent.depth = 1;
			bufferCopy.bufferOffset = 0;

			EVulkan::SetImageLayout(command, Texture->VulkanInternal.Image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, subresourceRange);

			vkCmdCopyBufferToImage(command, stagingBuffer.VulkanInternal.StagingBuffer, Texture->VulkanInternal.Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &bufferCopy);

			EVulkan::SetImageLayout(command, Texture->VulkanInternal.Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, subresourceRange);
		}
		else
		{
			EVulkan::SetImageLayout(command, Texture->VulkanInternal.Image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, subresourceRange);
		}

		Texture->VulkanInternal.ImageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

		if (EVulkan::FlushCommandBuffer<true>(command, EVulkan::ShortCommandPool, EVulkan::GraphicsQueue) == false)
		{
			return false;
		}

		stagingBuffer.Deallocate();
		Texture->TextureInternal.Loaded = true;

		return true;
	}

	const mu_boolean Create2DArray(const mu_uint32 Width,
		const mu_uint32 Height,
		const mu_uint32 Layers,
		const ETextureFormat Format,
		const ETextureUsage Usage,
		const ESampler Sampler,
		const mu_uint8 *DataBuffer,
		const mu_uint32 Pitch,
		ETextureBackend *Texture)
	{
		VmaAllocationInfo memoryInfo = {};

		if (CreateImageDynamic(Texture->VulkanInternal.Image,
			Texture->VulkanInternal.ImageView,
			Texture->VulkanInternal.ImageLayout,
			Texture->VulkanInternal.Memory,
			memoryInfo,
			Texture->VulkanInternal.MemorySize,
			Width, Height, Layers,
			RetrieveFormat(Format),
			VK_IMAGE_TYPE_2D,
			VK_IMAGE_VIEW_TYPE_2D_ARRAY) == false)
		{
			return false;
		}

		Texture->TextureInternal.StaticDescriptorSet = false;
		Texture->TextureInternal.Width = Width;
		Texture->TextureInternal.Height = Height;
		Texture->TextureInternal.Depth = Layers;
		Texture->TextureInternal.Components = GetTextureComponents(Format);
		Texture->TextureInternal.Sampler = Sampler;

		Texture->VulkanInternal.DescriptorSet = EVulkanDescriptor::CreateTextureDescriptorSet();
		if (Texture->VulkanInternal.DescriptorSet == VK_NULL_HANDLE)
		{
			return false;
		}

		EVulkanDescriptor::UpdateTextureDescriptorSet(Texture);

		VkCommandBuffer command = EVulkan::CreateCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, EVulkan::ShortCommandPool, true);
		if (command == nullptr)
		{
			return false;
		}

		VkImageSubresourceRange subresourceRange = {};
		subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		subresourceRange.baseMipLevel = 0;
		subresourceRange.levelCount = 1;
		subresourceRange.baseArrayLayer = 0;
		subresourceRange.layerCount = Layers;

		EStagingBufferBackend stagingBuffer;

		if ((Usage == ETextureUsage::eDynamic && DataBuffer != nullptr) ||
			Usage == ETextureUsage::eStatic)
		{
			const mu_size memorySize = Pitch * Height * Layers;
			mu_assert(DataBuffer != nullptr);

			if (stagingBuffer.Allocate(memorySize) == false)
			{
				return false;
			}

			mu_memcpy(stagingBuffer.BufferInternal.MappedMemory, DataBuffer, memorySize);

			stagingBuffer.Flush(0, VK_WHOLE_SIZE);

			VkBufferImageCopy bufferCopy = {};
			bufferCopy.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			bufferCopy.imageSubresource.mipLevel = 0;
			bufferCopy.imageSubresource.baseArrayLayer = 0;
			bufferCopy.imageSubresource.layerCount = Layers;
			bufferCopy.imageExtent.width = Width;
			bufferCopy.imageExtent.height = Height;
			bufferCopy.imageExtent.depth = 1;
			bufferCopy.bufferOffset = 0;
			EVulkan::SetImageLayout(command, Texture->VulkanInternal.Image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, subresourceRange);

			vkCmdCopyBufferToImage(command, stagingBuffer.VulkanInternal.StagingBuffer, Texture->VulkanInternal.Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &bufferCopy);

			EVulkan::SetImageLayout(command, Texture->VulkanInternal.Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, subresourceRange);
		}
		else
		{
			EVulkan::SetImageLayout(command, Texture->VulkanInternal.Image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, subresourceRange);
		}

		Texture->VulkanInternal.ImageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

		if (EVulkan::FlushCommandBuffer<true>(command, EVulkan::ShortCommandPool, EVulkan::GraphicsQueue) == false)
		{
			return false;
		}

		stagingBuffer.Deallocate();
		Texture->TextureInternal.Loaded = true;

		return true;
	}
};