#include "stdafx.h"

#define TEXTURE_POOL_SIZE (4096)
#define FONT_POOL_SIZE (2048)

namespace EVulkanDescriptor
{
	VkDescriptorPool FixedDescriptorPool = VK_NULL_HANDLE;
	std::vector<VkDescriptorPool> FontDescriptorPools;
	VkDescriptorSet TerrainTextureDescriptorSet[32] = {};
	std::vector<VkDescriptorSet> TerrainDataDescriptorSet;
	VkDescriptorSet GrassTextureDescriptorSet = VK_NULL_HANDLE;
	VkDescriptorSet EffectSamplersDescriptorSet = VK_NULL_HANDLE;

	MUCriticalSectionSpinLock TextureCritical;
	std::vector<VkDescriptorPool> TexturePools;
	mu_uint32 TextureDescriptorCount = 0;
	std::vector<VkDescriptorSet> FreeTextureSets;

	std::vector<VkDescriptorPool> FontPools;
	mu_uint32 FontDescriptorCount = 0;

	mu_uint32 SamplerAllocatedSets = 0;
	std::array<VkDescriptorSet, ESamplerMax> SamplerDescriptorSets = { { VK_NULL_HANDLE } };

	std::vector<std::array<VkDescriptorSet, EGlobalConstantMax>> GlobalDescriptorSets;

#define POOLSIZE_INSERT(x) for(mu_uint32 bi = 0;bi < mu_countof(x);++bi) poolsize.push_back(x[bi]);
	const mu_boolean Create()
	{
		// Fixed Descriptor Pool
		{
			VkDescriptorPoolSize GlobalConstantPoolSize[] =
			{
				{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, EGlobalConstantMax * EVulkan::SwapchainImagesCount },
			};

			VkDescriptorPoolSize TerrainConstantPoolSize[] =
			{
				{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, EVulkan::SwapchainImagesCount },
			};

			VkDescriptorPoolSize TerrainPoolSize[] =
			{
				{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, static_cast<mu_uint32>(mu_countof(EVulkanBinding::TerrainBinding)) * EVulkan::SwapchainImagesCount },
			};

			VkDescriptorPoolSize TerrainAttributePoolSize[] =
			{
				{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, EVulkan::SwapchainImagesCount },
			};

			VkDescriptorPoolSize BonesPoolSize[] =
			{
				{ EVulkan::SkeletonTexelBufferEnabled == true ? VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER : VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, EVulkan::SwapchainImagesCount },
			};

			VkDescriptorPoolSize ShadowPoolSize[] =
			{
				{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, static_cast<mu_uint32>(mu_countof(EVulkanBinding::ShadowBinding)) * EVulkan::SwapchainImagesCount },
			};

			VkDescriptorPoolSize TerrainTexturePoolSize[] =
			{
				{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, static_cast<mu_uint32>(mu_countof(EVulkanBinding::TerrainTextureBinding)) * static_cast<mu_uint32>(mu_countof(TerrainTextureDescriptorSet)) },
			};

			VkDescriptorPoolSize GrassTexturePoolSize[] =
			{
				{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, static_cast<mu_uint32>(mu_countof(EVulkanBinding::GrassTextureBinding)) },
			};

			VkDescriptorPoolSize WaterTexturePoolSize[] =
			{
				{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, static_cast<mu_uint32>(mu_countof(EVulkanBinding::TerrainTextureBinding)) },
			};

			VkDescriptorPoolSize SamplerPoolSize[] =
			{
				{ VK_DESCRIPTOR_TYPE_SAMPLER, ESamplerMax },
			};

			VkDescriptorPoolSize EffectSamplerPoolSize[] =
			{
				{ VK_DESCRIPTOR_TYPE_SAMPLER, EVulkanBinding::EffectSamplerBinding[0].descriptorCount },
			};

			std::vector<VkDescriptorPoolSize> poolsize;

			POOLSIZE_INSERT(GlobalConstantPoolSize);
			POOLSIZE_INSERT(TerrainConstantPoolSize);
			POOLSIZE_INSERT(TerrainPoolSize);
			POOLSIZE_INSERT(TerrainAttributePoolSize);
			POOLSIZE_INSERT(BonesPoolSize);
			POOLSIZE_INSERT(ShadowPoolSize);
			POOLSIZE_INSERT(TerrainTexturePoolSize);
			POOLSIZE_INSERT(GrassTexturePoolSize);
			POOLSIZE_INSERT(WaterTexturePoolSize);
			POOLSIZE_INSERT(SamplerPoolSize);
			POOLSIZE_INSERT(EffectSamplerPoolSize);

			VkDescriptorPoolCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
			createInfo.poolSizeCount = poolsize.size();
			createInfo.pPoolSizes = poolsize.data();
			createInfo.maxSets = EGlobalConstantMax * EVulkan::SwapchainImagesCount +
				1 * EVulkan::SwapchainImagesCount + // Terrain Global Constant
				1 * EVulkan::SwapchainImagesCount + // Terrain Data
				1 * EVulkan::SwapchainImagesCount + // Terrain Attribute Data
				1 * EVulkan::SwapchainImagesCount + // Bones Data
				1 * EVulkan::SwapchainImagesCount + // Shadow Data
				mu_countof(TerrainTextureDescriptorSet) +
				1 + 1 + // Terrain, Grass & Water Texture
				ESamplerMax + // All Samplers
				EVulkanBinding::EffectSamplerBinding[0].descriptorCount; // Effect Samplers

			VkResult result = vkCreateDescriptorPool(EVulkan::Device, &createInfo, nullptr, &FixedDescriptorPool);
			if (result != VK_SUCCESS)
			{
				return false;
			}
		}

		// Allocate Terrain Texture Descriptors Set
		{
			std::array<VkDescriptorSetLayout, mu_countof(TerrainTextureDescriptorSet)> layouts;
			std::fill(layouts.begin(), layouts.end(), EVulkanPipeline::GetSetLayout(EVulkan::VULKAN_SETLAYOUT_TERRAIN_TEXTURE));

			VkDescriptorSetAllocateInfo allocateInfo = {};
			allocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			allocateInfo.descriptorPool = FixedDescriptorPool;
			allocateInfo.descriptorSetCount = mu_countof(TerrainTextureDescriptorSet);
			allocateInfo.pSetLayouts = layouts.data();

			VkResult result = vkAllocateDescriptorSets(EVulkan::Device, &allocateInfo, TerrainTextureDescriptorSet);
			if (result != VK_SUCCESS)
			{
				return false;
			}
		}

		// Allocate Terrain Data Descriptors Set
		{
			TerrainDataDescriptorSet.resize(EVulkan::SwapchainImagesCount, VK_NULL_HANDLE);

			std::vector<VkDescriptorSetLayout> layouts(EVulkan::SwapchainImagesCount, EVulkanPipeline::GetSetLayout(EVulkan::VULKAN_SETLAYOUT_TERRAIN_DATA));

			VkDescriptorSetAllocateInfo allocateInfo = {};
			allocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			allocateInfo.descriptorPool = FixedDescriptorPool;
			allocateInfo.descriptorSetCount = EVulkan::SwapchainImagesCount;
			allocateInfo.pSetLayouts = layouts.data();

			VkResult result = vkAllocateDescriptorSets(EVulkan::Device, &allocateInfo, TerrainDataDescriptorSet.data());
			if (result != VK_SUCCESS)
			{
				return false;
			}
		}

		// Allocate Grass Texture Descriptors Set
		{
			VkDescriptorSetLayout setLayout = EVulkanPipeline::GetSetLayout(EVulkan::VULKAN_SETLAYOUT_GRASS_TEXTURE);

			VkDescriptorSetAllocateInfo allocateInfo = {};
			allocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			allocateInfo.descriptorPool = FixedDescriptorPool;
			allocateInfo.descriptorSetCount = 1;
			allocateInfo.pSetLayouts = &setLayout;

			VkResult result = vkAllocateDescriptorSets(EVulkan::Device, &allocateInfo, &GrassTextureDescriptorSet);
			if (result != VK_SUCCESS)
			{
				return false;
			}
		}

		// Allocate Effect Samplers Descriptors Set
		{
			VkDescriptorSetLayout setLayout = EVulkanPipeline::GetSetLayout(EVulkan::VULKAN_SETLAYOUT_EFFECT_SAMPLER);

			VkDescriptorSetAllocateInfo allocateInfo = {};
			allocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			allocateInfo.descriptorPool = FixedDescriptorPool;
			allocateInfo.descriptorSetCount = 1;
			allocateInfo.pSetLayouts = &setLayout;

			VkResult result = vkAllocateDescriptorSets(EVulkan::Device, &allocateInfo, &EffectSamplersDescriptorSet);
			if (result != VK_SUCCESS)
			{
				return false;
			}

			VkWriteDescriptorSet writeInfo[mu_countof(EffectSamplers)];
			VkDescriptorImageInfo imageInfo[mu_countof(EffectSamplers)];
			for(mu_uint32 n = 0;n < mu_countof(EffectSamplers);++n)
			{
				imageInfo[n].sampler = EVulkanSampler::Get(EffectSamplers[n]);
				imageInfo[n].imageView = VK_NULL_HANDLE;
				imageInfo[n].imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;

				writeInfo[n].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				writeInfo[n].pNext = nullptr;
				writeInfo[n].dstSet = EffectSamplersDescriptorSet;
				writeInfo[n].dstBinding = EVulkanBinding::EffectSamplerBinding[0].binding;
				writeInfo[n].dstArrayElement = 0;
				writeInfo[n].descriptorCount = 1;
				writeInfo[n].descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
				writeInfo[n].pImageInfo = &imageInfo[n];
			}

			vkUpdateDescriptorSets(EVulkan::Device, mu_countof(writeInfo), writeInfo, 0, nullptr);
		}

		return true;
	}

	VkDescriptorSet CreateBonesDescriptorSet()
	{
		VkDescriptorSetLayout setLayout = EVulkanPipeline::GetSetLayout(EVulkan::VULKAN_SETLAYOUT_BONES);
		VkDescriptorSetAllocateInfo allocateInfo = {};
		allocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocateInfo.descriptorPool = FixedDescriptorPool;
		allocateInfo.descriptorSetCount = 1;
		allocateInfo.pSetLayouts = &setLayout;

		VkDescriptorSet descriptorSet = VK_NULL_HANDLE;
		VkResult result = vkAllocateDescriptorSets(EVulkan::Device, &allocateInfo, &descriptorSet);
		if (result != VK_SUCCESS)
		{
			return VK_NULL_HANDLE;
		}

		return descriptorSet;
	}

	VkDescriptorSet CreateShadowMapDescriptorSet()
	{
		VkDescriptorSetLayout setLayout = EVulkanPipeline::GetSetLayout(EVulkan::VULKAN_SETLAYOUT_SHADOW);
		VkDescriptorSetAllocateInfo allocateInfo = {};
		allocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocateInfo.descriptorPool = FixedDescriptorPool;
		allocateInfo.descriptorSetCount = 1;
		allocateInfo.pSetLayouts = &setLayout;

		VkDescriptorSet descriptorSet = VK_NULL_HANDLE;
		VkResult result = vkAllocateDescriptorSets(EVulkan::Device, &allocateInfo, &descriptorSet);
		if (result != VK_SUCCESS)
		{
			return VK_NULL_HANDLE;
		}

		return descriptorSet;
	}

	VkDescriptorSet CreateTextureDescriptorSet()
	{
		MUAutoCriticalSpinLock lock(TextureCritical);

		if(FreeTextureSets.empty() == false)
		{
			auto it = FreeTextureSets.end() - 1;
			VkDescriptorSet set = *it;
			FreeTextureSets.erase(it);
			return set;
		}

		const mu_uint32 needPoolCount = (TextureDescriptorCount / TEXTURE_POOL_SIZE) + 1;
		if(TexturePools.size() < needPoolCount)
		{
			VkDescriptorPoolSize TexturePoolSize[] =
			{
				{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, TEXTURE_POOL_SIZE },
			};

			std::vector<VkDescriptorPoolSize> poolsize;

			POOLSIZE_INSERT(TexturePoolSize);

			VkDescriptorPoolCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
			createInfo.poolSizeCount = poolsize.size();
			createInfo.pPoolSizes = poolsize.data();
			createInfo.maxSets = TEXTURE_POOL_SIZE;

			VkDescriptorPool pool = VK_NULL_HANDLE;
			VkResult result = vkCreateDescriptorPool(EVulkan::Device, &createInfo, nullptr, &pool);
			if (result != VK_SUCCESS)
			{
				return VK_NULL_HANDLE;
			}

			TexturePools.push_back(pool);
		}

		VkDescriptorSetLayout setLayout = EVulkanPipeline::GetSetLayout(EVulkan::VULKAN_SETLAYOUT_TEXTURE);
		VkDescriptorSetAllocateInfo allocateInfo = {};
		allocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocateInfo.descriptorPool = TexturePools[needPoolCount - 1];
		allocateInfo.descriptorSetCount = 1;
		allocateInfo.pSetLayouts = &setLayout;

		VkDescriptorSet descriptorSet = VK_NULL_HANDLE;
		VkResult result = vkAllocateDescriptorSets(EVulkan::Device, &allocateInfo, &descriptorSet);
		if(result != VK_SUCCESS)
		{
			return VK_NULL_HANDLE;
		}

		++TextureDescriptorCount;

		return descriptorSet;
	}

	void FreeTextureDescriptorSet(VkDescriptorSet descriptorSet)
	{
		if(descriptorSet == VK_NULL_HANDLE) return;

		MUAutoCriticalSpinLock lock(TextureCritical);
		FreeTextureSets.push_back(descriptorSet);
	}

	void UpdateTextureDescriptorSet(ETextureBackend *Texture)
	{
		VkDescriptorImageInfo imageInfo[1];

		imageInfo[0].sampler = EVulkanSampler::Get(Texture->TextureInternal.Sampler);
		imageInfo[0].imageView = Texture->VulkanInternal.ImageView;
		imageInfo[0].imageLayout = Texture->VulkanInternal.ImageLayout;

		VkWriteDescriptorSet writeSet[1];

		writeSet[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		writeSet[0].pNext = nullptr;
		writeSet[0].dstSet = Texture->VulkanInternal.DescriptorSet;
		writeSet[0].dstBinding = EVulkanBinding::TextureBinding[0].binding;
		writeSet[0].dstArrayElement = 0;
		writeSet[0].descriptorCount = 1;
		writeSet[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		writeSet[0].pImageInfo = &imageInfo[0];

		vkUpdateDescriptorSets(EVulkan::Device, mu_countof(writeSet), writeSet, 0, nullptr);
	}

	const mu_boolean AllocateAllSamplerDescriptorSet()
	{
		ZeroArray(SamplerDescriptorSets);

		VkDescriptorSetLayout setLayout = EVulkanPipeline::GetSetLayout(EVulkan::VULKAN_SETLAYOUT_SAMPLER);
		std::array<VkDescriptorImageInfo, ESamplerMax> imageInfo;
		std::array<VkWriteDescriptorSet, ESamplerMax> writeInfo;

		for(mu_uint32 n = 0;n < ESamplerMax;++n, ++SamplerAllocatedSets)
		{
			VkDescriptorSetAllocateInfo allocateInfo = {};
			allocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			allocateInfo.descriptorPool = FixedDescriptorPool;
			allocateInfo.descriptorSetCount = 1;
			allocateInfo.pSetLayouts = &setLayout;

			VkResult result = vkAllocateDescriptorSets(EVulkan::Device, &allocateInfo, &SamplerDescriptorSets[n]);
			if (result != VK_SUCCESS)
			{
				return false;
			}

			// Update Vulkan Descriptor Set
			{
				imageInfo[n].sampler = EVulkanSampler::Get(static_cast<ESampler>(n));
				imageInfo[n].imageView = VK_NULL_HANDLE;
				imageInfo[n].imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;

				writeInfo[n].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				writeInfo[n].pNext = nullptr;
				writeInfo[n].dstSet = SamplerDescriptorSets[n];
				writeInfo[n].dstBinding = EVulkanBinding::SamplerBinding[0].binding;
				writeInfo[n].dstArrayElement = 0;
				writeInfo[n].descriptorCount = 1;
				writeInfo[n].descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
				writeInfo[n].pImageInfo = &imageInfo[n];
			}
		}

		vkUpdateDescriptorSets(EVulkan::Device, ESamplerMax, writeInfo.data(), 0, nullptr);

		return true;
	}

	const mu_boolean AllocateAllGlobalDescriptorSet()
	{
		GlobalDescriptorSets.resize(EVulkan::SwapchainImagesCount);

		VkDescriptorSetLayout setLayout = EVulkanPipeline::GetSetLayout(EVulkan::VULKAN_SETLAYOUT_GLOBAL_CONSTANT);
		std::vector<VkDescriptorSetLayout> descriptorSetLayouts(EGlobalConstantMax, setLayout);
		std::vector<VkDescriptorBufferInfo> bufferInfo(EGlobalConstantMax);
		std::vector<VkWriteDescriptorSet> writeInfo(EGlobalConstantMax);

		for (mu_uint32 n = 0;n < EVulkan::SwapchainImagesCount; ++n)
		{
			std::array<VkDescriptorSet, EGlobalConstantMax> &globalDescriptors = GlobalDescriptorSets[n];

			VkDescriptorSetAllocateInfo allocateInfo = {};
			allocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			allocateInfo.descriptorPool = FixedDescriptorPool;
			allocateInfo.descriptorSetCount = EGlobalConstantMax;
			allocateInfo.pSetLayouts = descriptorSetLayouts.data();

			VkResult result = vkAllocateDescriptorSets(EVulkan::Device, &allocateInfo, globalDescriptors.data());
			if (result != VK_SUCCESS)
			{
				return false;
			}

			// Update Vulkan Descriptor Set
			for (mu_uint32 j = 0; j < EGlobalConstantMax; ++j)
			{
				bufferInfo[j].buffer = EVulkanBuffer::GlobalConstantBuffer[n].VulkanInternal.Buffer;
				bufferInfo[j].offset = EVulkanBuffer::GlobalConstantUniformSize * j;
				bufferInfo[j].range = EVulkanBuffer::GlobalConstantUniformSize;

				writeInfo[j].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				writeInfo[j].pNext = nullptr;
				writeInfo[j].dstSet = globalDescriptors[j];
				writeInfo[j].dstBinding = EVulkanBinding::GlobalConstantBinding[0].binding;
				writeInfo[j].dstArrayElement = 0;
				writeInfo[j].descriptorCount = 1;
				writeInfo[j].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				writeInfo[j].pBufferInfo = &bufferInfo[j];
			}

			vkUpdateDescriptorSets(EVulkan::Device, EGlobalConstantMax, writeInfo.data(), 0, nullptr);
		}

		return true;
	}

	VkDescriptorSet CreateFontDescriptorSet(EBufferBackend *UniformBuffer)
	{
		const mu_uint32 needPoolCount = (FontDescriptorCount / FONT_POOL_SIZE) + 1;
		if (FontPools.size() < needPoolCount)
		{
			VkDescriptorPoolSize FontPoolSize[] =
			{
				{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, FONT_POOL_SIZE },
			};

			std::vector<VkDescriptorPoolSize> poolsize;

			POOLSIZE_INSERT(FontPoolSize);

			VkDescriptorPoolCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
			createInfo.poolSizeCount = poolsize.size();
			createInfo.pPoolSizes = poolsize.data();
			createInfo.maxSets = FONT_POOL_SIZE;

			VkDescriptorPool pool = VK_NULL_HANDLE;
			VkResult result = vkCreateDescriptorPool(EVulkan::Device, &createInfo, nullptr, &pool);
			if (result != VK_SUCCESS)
			{
				return VK_NULL_HANDLE;
			}

			FontPools.push_back(pool);
		}

		VkDescriptorSetLayout setLayout = EVulkanPipeline::GetSetLayout(EVulkan::VULKAN_SETLAYOUT_FONT_CONSTANT);
		VkDescriptorSetAllocateInfo allocateInfo = {};
		allocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocateInfo.descriptorPool = FontPools[needPoolCount - 1];
		allocateInfo.descriptorSetCount = 1;
		allocateInfo.pSetLayouts = &setLayout;

		VkDescriptorSet descriptorSet = VK_NULL_HANDLE;
		VkResult result = vkAllocateDescriptorSets(EVulkan::Device, &allocateInfo, &descriptorSet);
		if (result != VK_SUCCESS)
		{
			return VK_NULL_HANDLE;
		}

		++FontDescriptorCount;

		VkDescriptorBufferInfo bufferInfo[1] = { {} };

		bufferInfo[0].buffer = UniformBuffer->VulkanInternal.Buffer;
		bufferInfo[0].offset = 0;
		bufferInfo[0].range = sizeof(SHFontConstant);

		VkWriteDescriptorSet writeSet[1] = { {} };

		writeSet[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		writeSet[0].pNext = nullptr;
		writeSet[0].dstSet = descriptorSet;
		writeSet[0].dstBinding = EVulkanBinding::FontConstantBinding[0].binding;
		writeSet[0].dstArrayElement = 0;
		writeSet[0].descriptorCount = 1;
		writeSet[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		writeSet[0].pBufferInfo = bufferInfo;

		vkUpdateDescriptorSets(EVulkan::Device, mu_countof(writeSet), writeSet, 0, nullptr);

		return descriptorSet;
	}
#undef POOLSIZE_INSERT

	void Destroy()
	{
		for (auto it = FontPools.begin(); it != FontPools.end(); ++it)
		{
			VkDescriptorPool tmp = *it;
			if (tmp != VK_NULL_HANDLE)
			{
				vkDestroyDescriptorPool(EVulkan::Device, tmp, nullptr);
			}
		}
		FontPools.clear();
		FontDescriptorCount = 0;

		for (auto it = TexturePools.begin(); it != TexturePools.end(); ++it)
		{
			VkDescriptorPool tmp = *it;
			if (tmp != VK_NULL_HANDLE)
			{
				vkDestroyDescriptorPool(EVulkan::Device, tmp, nullptr);
			}
		}
		TexturePools.clear();
		FreeTextureSets.clear();
		TextureDescriptorCount = 0;

		if (FixedDescriptorPool != VK_NULL_HANDLE)
		{
			vkDestroyDescriptorPool(EVulkan::Device, FixedDescriptorPool, nullptr);
			FixedDescriptorPool = VK_NULL_HANDLE;
		}
	}

	VkDescriptorSet GetSamplerDescriptorSet(const ESampler index)
	{
		return SamplerDescriptorSets[ECastEnum(index)];
	}

	VkDescriptorSet GetGlobalDescriptorSet(const EGlobalConstant index)
	{
		return GlobalDescriptorSets[EVulkan::CurrentCommandBufferIndex][ECastEnum(index)];
	}

	void UpdateTerrainTextureDescriptorSet(const MUTexture *TilesTexture)
	{
		VkWriteDescriptorSet writeInfo[mu_countof(TerrainTextureDescriptorSet) * 2];
		VkDescriptorImageInfo imageInfo[mu_countof(TerrainTextureDescriptorSet) * 2];

		for(mu_uint32 n = 0, j = 0;n < mu_countof(TerrainTextureDescriptorSet) * 2;n += 2, ++j)
		{
			imageInfo[n + 0].sampler = EVulkanSampler::Get(ESampler::eUrVrWrMinLMagLMipL_Anisotropy);
			imageInfo[n + 0].imageView = TilesTexture->VulkanInternal.ImageView;
			imageInfo[n + 0].imageLayout = TilesTexture->VulkanInternal.ImageLayout;

			imageInfo[n + 1].sampler = EVulkanSampler::Get(ESampler::eUrVrWrMinLMagLMipL_Anisotropy);
			imageInfo[n + 1].imageView = TEXTURES::Data[BITMAP_WATER + j].VulkanInternal.ImageView;
			imageInfo[n + 1].imageLayout = TEXTURES::Data[BITMAP_WATER + j].VulkanInternal.ImageLayout;

			writeInfo[n + 0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			writeInfo[n + 0].pNext = nullptr;
			writeInfo[n + 0].dstSet = TerrainTextureDescriptorSet[j];
			writeInfo[n + 0].dstBinding = EVulkanBinding::TerrainTextureBinding[0].binding;
			writeInfo[n + 0].dstArrayElement = 0;
			writeInfo[n + 0].descriptorCount = 1;
			writeInfo[n + 0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			writeInfo[n + 0].pImageInfo = &imageInfo[n + 0];

			writeInfo[n + 1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			writeInfo[n + 1].pNext = nullptr;
			writeInfo[n + 1].dstSet = TerrainTextureDescriptorSet[j];
			writeInfo[n + 1].dstBinding = EVulkanBinding::TerrainTextureBinding[1].binding;
			writeInfo[n + 1].dstArrayElement = 0;
			writeInfo[n + 1].descriptorCount = 1;
			writeInfo[n + 1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			writeInfo[n + 1].pImageInfo = &imageInfo[n + 1];
		}

		vkUpdateDescriptorSets(EVulkan::Device, mu_countof(writeInfo), writeInfo, 0, nullptr);
	}

	VkDescriptorSet GetTerrainTextureDescriptorSet(const mu_uint32 WaterIndex)
	{
		return TerrainTextureDescriptorSet[WaterIndex];
	}

	void UpdateTerrainDataDescriptorSet(const MUTexture *AlphaTexture, const MUTexture *HeightTexture, const MUTexture *AttributeTexture)
	{
		for(mu_uint32 n = 0;n < EVulkan::SwapchainImagesCount;++n)
		{
			VkWriteDescriptorSet writeInfo[4];
			VkDescriptorImageInfo imageInfo[4];

			imageInfo[0].sampler = EVulkanSampler::Get(TERRAIN_LIGHT_SAMPLER);
			imageInfo[0].imageView = EVulkanBuffer::GetTerrainLightTexture(n)->VulkanInternal.ImageView;
			imageInfo[0].imageLayout = EVulkanBuffer::GetTerrainLightTexture(n)->VulkanInternal.ImageLayout;

			imageInfo[1].sampler = EVulkanSampler::Get(TERRAIN_ALPHA_SAMPLER);
			imageInfo[1].imageView = AlphaTexture->VulkanInternal.ImageView;
			imageInfo[1].imageLayout = AlphaTexture->VulkanInternal.ImageLayout;

			imageInfo[2].sampler = EVulkanSampler::Get(TERRAIN_HEIGHT_SAMPLER);
			imageInfo[2].imageView = HeightTexture->VulkanInternal.ImageView;
			imageInfo[2].imageLayout = HeightTexture->VulkanInternal.ImageLayout;

			imageInfo[3].sampler = EVulkanSampler::Get(TERRAIN_ATTRIBUTE_SAMPLER);
			imageInfo[3].imageView = AttributeTexture->VulkanInternal.ImageView;
			imageInfo[3].imageLayout = AttributeTexture->VulkanInternal.ImageLayout;

			writeInfo[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			writeInfo[0].pNext = nullptr;
			writeInfo[0].dstSet = TerrainDataDescriptorSet[n];
			writeInfo[0].dstBinding = EVulkanBinding::TerrainBinding[0].binding;
			writeInfo[0].dstArrayElement = 0;
			writeInfo[0].descriptorCount = 1;
			writeInfo[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			writeInfo[0].pImageInfo = &imageInfo[0];

			writeInfo[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			writeInfo[1].pNext = nullptr;
			writeInfo[1].dstSet = TerrainDataDescriptorSet[n];
			writeInfo[1].dstBinding = EVulkanBinding::TerrainBinding[1].binding;
			writeInfo[1].dstArrayElement = 0;
			writeInfo[1].descriptorCount = 1;
			writeInfo[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			writeInfo[1].pImageInfo = &imageInfo[1];

			writeInfo[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			writeInfo[2].pNext = nullptr;
			writeInfo[2].dstSet = TerrainDataDescriptorSet[n];
			writeInfo[2].dstBinding = EVulkanBinding::TerrainBinding[2].binding;
			writeInfo[2].dstArrayElement = 0;
			writeInfo[2].descriptorCount = 1;
			writeInfo[2].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			writeInfo[2].pImageInfo = &imageInfo[2];

			writeInfo[3].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			writeInfo[3].pNext = nullptr;
			writeInfo[3].dstSet = TerrainDataDescriptorSet[n];
			writeInfo[3].dstBinding = EVulkanBinding::TerrainBinding[3].binding;
			writeInfo[3].dstArrayElement = 0;
			writeInfo[3].descriptorCount = 1;
			writeInfo[3].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			writeInfo[3].pImageInfo = &imageInfo[3];

			vkUpdateDescriptorSets(EVulkan::Device, mu_countof(writeInfo), writeInfo, 0, nullptr);
		}
	}

	VkDescriptorSet GetTerrainDataDescriptorSet()
	{
		return TerrainDataDescriptorSet[EVulkan::CurrentCommandBufferIndex];
	}

	void UpdateGrassTextureDescriptorSet(const MUTexture *GrassTexture)
	{
		VkWriteDescriptorSet writeInfo[1];
		VkDescriptorImageInfo imageInfo[1];

		imageInfo[0].sampler = EVulkanSampler::Get(ESampler::eUrVrWrMinLMagLMipL_Anisotropy);
		imageInfo[0].imageView = GrassTexture->VulkanInternal.ImageView;
		imageInfo[0].imageLayout = GrassTexture->VulkanInternal.ImageLayout;

		writeInfo[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		writeInfo[0].pNext = nullptr;
		writeInfo[0].dstSet = GrassTextureDescriptorSet;
		writeInfo[0].dstBinding = EVulkanBinding::GrassTextureBinding[0].binding;
		writeInfo[0].dstArrayElement = 0;
		writeInfo[0].descriptorCount = 1;
		writeInfo[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		writeInfo[0].pImageInfo = &imageInfo[0];

		vkUpdateDescriptorSets(EVulkan::Device, mu_countof(writeInfo), writeInfo, 0, nullptr);
	}

	VkDescriptorSet GetGrassTextureDescriptorSet()
	{
		return GrassTextureDescriptorSet;
	}

	VkDescriptorSet GetEffectSamplersDescriptorSet()
	{
		return EffectSamplersDescriptorSet;
	}
};