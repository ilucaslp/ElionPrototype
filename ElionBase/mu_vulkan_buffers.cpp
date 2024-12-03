#include "stdafx.h"

namespace EVulkanBuffer
{
	VkDeviceSize GlobalConstantStagingSize;
	VkDeviceSize GlobalConstantUniformSize;
	std::vector<EBufferBackend> GlobalConstantBuffer;
	std::vector<EBufferBackend> ModelInstanceBuffer;
	std::vector<EBufferBackend> EffectInstanceBuffer;
	std::vector<EBufferBackend> ClothVertexBuffer;

	constexpr mu_size WaterMemorySize = sizeof(SHWaterVertex) * MAX_WATER_GRID * MAX_WATER_GRID * 4;
	constexpr mu_size WaterIndexMemorySize = sizeof(mu_uint16) * MAX_WATER_GRID * MAX_WATER_GRID * 6;
	std::vector<mu_boolean> WaterVertexUpdate;
	std::vector<EBufferBackend> WaterVertexBuffer;
	EBufferBackend WaterIndexBuffer;

	EBufferBackend DummyVertexBuffer;
	EBufferBackend DummyIndexBuffer;

	std::vector<MUTexture*> ShadowMapTexture;
	std::vector<VkDescriptorSet> ShadowDescriptorsSet;

	std::vector<EStagingBufferBackend> TerrainLightStagingBuffer;
	std::vector<MUTexture*> TerrainLightTexture;
	std::vector<mu_boolean> TerrainLightUpdate;

	EVulkanUtils::EDynamicBuffer<SHUIDefaultVertex, UI_VERTEX_INITIAL, UI_VERTEX_GROW> UIVertexBuffer;
	EVulkanUtils::EDynamicBufferExtended<SHFontInstance, FONT_INSTANCE_INITIAL, FONT_INSTANCE_GROW> FontVertexBuffer;

	const mu_boolean Create()
	{
		GlobalConstantBuffer.resize(EVulkan::SwapchainImagesCount);
		ModelInstanceBuffer.resize(EVulkan::SwapchainImagesCount);
		EffectInstanceBuffer.resize(EVulkan::SwapchainImagesCount);
		ClothVertexBuffer.resize(EVulkan::SwapchainImagesCount);
		WaterVertexUpdate.resize(EVulkan::SwapchainImagesCount, true);
		WaterVertexBuffer.resize(EVulkan::SwapchainImagesCount);

		GlobalConstantUniformSize = sizeof(SHGlobalConstantBuffer);
		EVulkan::AlignUniformSize(GlobalConstantUniformSize);
		GlobalConstantStagingSize = GlobalConstantUniformSize * EGlobalConstantMax;
		EVulkan::AlignAtomSize(GlobalConstantStagingSize);

		for(mu_uint32 n = 0;n < EVulkan::SwapchainImagesCount;++n)
		{
			if(GlobalConstantBuffer[n].Allocate(GlobalConstantStagingSize,
				EBufferUsage::eDynamic,
				EBufferType::eUniform) == false)
			{
				return false;
			}

			if (ModelInstanceBuffer[n].Allocate(sizeof(SHModelInstance) * MAX_MODEL_INSTANCES,
				EBufferUsage::eDynamic,
				EBufferType::eInstance) == false)
			{
				return false;
			}

			if (EffectInstanceBuffer[n].Allocate(sizeof(SHEffectInstance) * MAX_EFFECTS_INSTANCES,
				EBufferUsage::eDynamic,
				EBufferType::eInstance) == false)
			{
				return false;
			}

			if (ClothVertexBuffer[n].Allocate(sizeof(SHClothVertexData) * MAX_CLOTH_VERTICES,
				EBufferUsage::eDynamic,
				EBufferType::eInstance) == false)
			{
				return false;
			}

			if (WaterVertexBuffer[n].Allocate(WaterMemorySize, EBufferUsage::eDynamic, EBufferType::eInstance, nullptr) == false)
			{
				return false;
			}
		}

		// Water Index Setup
		{
			std::array<mu_uint16, MAX_WATER_GRID * MAX_WATER_GRID * 6> indexData;

			for (mu_uint32 y = 0; y < MAX_WATER_GRID; ++y)
			{
				for (mu_uint32 x = 0; x < MAX_WATER_GRID; ++x)
				{
					mu_uint16 *terrainIndex = &indexData[y * MAX_WATER_GRID * 6 + x * 6];
					terrainIndex[0] = y * MAX_WATER_GRID * 4 + x * 4 + 0;
					terrainIndex[1] = y * MAX_WATER_GRID * 4 + x * 4 + 1;
					terrainIndex[2] = y * MAX_WATER_GRID * 4 + x * 4 + 2;
					terrainIndex[3] = y * MAX_WATER_GRID * 4 + x * 4 + 0;
					terrainIndex[4] = y * MAX_WATER_GRID * 4 + x * 4 + 2;
					terrainIndex[5] = y * MAX_WATER_GRID * 4 + x * 4 + 3;
				}
			}

			if (WaterIndexBuffer.Allocate(WaterIndexMemorySize, EBufferUsage::eStatic, EBufferType::eIndex, indexData.data()) == false)
			{
				return false;
			}
		}

		// Dummy Vertex
		{
			const mu_uint16 dummyVertex[4] = { 0, 1, 2, 3 };
			const VkDeviceSize memorySize = sizeof(dummyVertex);

			if(DummyVertexBuffer.Allocate(memorySize,
				EBufferUsage::eStatic,
				EBufferType::eVertex,
				dummyVertex) == false)
			{
				return false;
			}
		}

		// Dummy Index
		{
			const mu_uint16 dummyIndex[6] = { 0, 1, 2, 0, 2, 3 };
			const VkDeviceSize memorySize = sizeof(dummyIndex);

			if (DummyIndexBuffer.Allocate(memorySize,
				EBufferUsage::eStatic,
				EBufferType::eIndex,
				dummyIndex) == false)
			{
				return false;
			}
		}

		// Terrain Light
		{
			// Terrain Size^2 * Components RGB * Component Type Size
			TerrainLightStagingBuffer.resize(EVulkan::SwapchainImagesCount);
			TerrainLightTexture.resize(EVulkan::SwapchainImagesCount, nullptr);
			TerrainLightUpdate.resize(EVulkan::SwapchainImagesCount, true);

			for(mu_uint32 n = 0;n < EVulkan::SwapchainImagesCount;++n)
			{
				TerrainLightTexture[n] = new MUTexture();

				if(TerrainLightTexture[n]->Create2D(TERRAIN_SIZE,
					TERRAIN_SIZE,
					TERRAIN_LIGHT_FORMAT,
					ETextureUsage::eDynamic,
					TERRAIN_LIGHT_SAMPLER,
					nullptr,
					0) == false)
				{
					return false;
				}

				if (TerrainLightStagingBuffer[n].Allocate(TerrainLightTexture[n]->VulkanInternal.MemorySize) == false)
				{
					return false;
				}
			}
		}

		if(UIVertexBuffer.Initialize(EVulkan::SwapchainImagesCount) == false)
		{
			return false;
		}

		if(FontVertexBuffer.Initialize(EVulkan::SwapchainImagesCount) == false)
		{
			return false;
		}

		return true;
	}

	void Destroy(const mu_boolean gpuOnly)
	{
		FontVertexBuffer.Destroy(gpuOnly);
		UIVertexBuffer.Destroy(gpuOnly);

		DestroyShadowMap();
		ShadowDescriptorsSet.clear();

		for (auto it = TerrainLightTexture.begin(); it != TerrainLightTexture.end(); ++it)
		{
			MUTexture *texture = *it;
			if (texture != nullptr)
			{
				texture->Release();
				delete texture;
			}
		}
		TerrainLightTexture.clear();

		for(auto it = TerrainLightStagingBuffer.begin();it != TerrainLightStagingBuffer.end();++it)
		{
			(*it).Deallocate();
		}
		TerrainLightStagingBuffer.clear();

		for (auto it = GlobalConstantBuffer.begin(); it != GlobalConstantBuffer.end(); ++it)
		{
			(*it).Deallocate();
		}
		GlobalConstantBuffer.clear();

		for (auto it = ModelInstanceBuffer.begin(); it != ModelInstanceBuffer.end(); ++it)
		{
			(*it).Deallocate();
		}
		ModelInstanceBuffer.clear();

		for (auto it = EffectInstanceBuffer.begin(); it != EffectInstanceBuffer.end(); ++it)
		{
			(*it).Deallocate();
		}
		EffectInstanceBuffer.clear();

		for (auto it = ClothVertexBuffer.begin(); it != ClothVertexBuffer.end(); ++it)
		{
			(*it).Deallocate();
		}
		ClothVertexBuffer.clear();

		for (auto it = WaterVertexBuffer.begin(); it != WaterVertexBuffer.end(); ++it)
		{
			(*it).Deallocate();
		}
		WaterVertexBuffer.clear();
		WaterIndexBuffer.Deallocate();
		WaterVertexUpdate.clear();

		DummyVertexBuffer.Deallocate();
		DummyIndexBuffer.Deallocate();
	}

	const mu_boolean CreateShadowMap()
	{
		if (ShadowDescriptorsSet.size() == 0)
		{
			ShadowDescriptorsSet.resize(EVulkan::SwapchainImagesCount, VK_NULL_HANDLE);

			for (auto it = ShadowDescriptorsSet.begin(); it != ShadowDescriptorsSet.end(); ++it)
			{
				*it = EVulkanDescriptor::CreateShadowMapDescriptorSet();
				if (*it == VK_NULL_HANDLE)
				{
					return false;
				}
			}
		}

		if (MU_RESOURCE->GetShadowType() >= EShadowType::eShadowMappingBegin)
		{
			ShadowMapTexture.resize(EVulkan::SwapchainImagesCount, nullptr);
			for (mu_uint32 n = 0; n < EVulkan::SwapchainImagesCount; ++n)
			{
				ShadowMapTexture[n] = new MUTexture();

				if (EVulkanTexture::CreateShadowMap(ShadowMapTexture[n], ShadowDescriptorsSet[n]) == false)
				{
					return false;
				}
			}
		}

		return true;
	}

	void DestroyShadowMap()
	{
		for (auto it = ShadowMapTexture.begin(); it != ShadowMapTexture.end(); ++it)
		{
			MUTexture *texture = *it;
			if (texture != nullptr)
			{
				texture->Release();
				delete texture;
			}
		}
		ShadowMapTexture.clear();
		ShadowDescriptorsSet.clear();
	}

	void UpdateGlobalOnly()
	{
		const mu_uint32 index = EVulkan::CurrentCommandBufferIndex;

		GlobalConstantBuffer[index].Flush(0, 0, GlobalConstantStagingSize);
	}

#if defined(USE_COMPRESSED_TERRAINLIGHT_RGB8)
	mu_uint32 TerrainLightBuffer[MAX_TERRAIN_SIZE];
#else
	EVector4 TerrainLightBuffer[MAX_TERRAIN_SIZE];
#endif

	void Update(const mu_boolean IsLightUpload)
	{
		const mu_uint32 index = EVulkan::CurrentCommandBufferIndex;
		const mu_uint32 modelInstancesCount = g_Tasking._ModelInstanceCount.load(std::memory_order_relaxed);
		const mu_uint32 effectInstancesCount = g_Tasking._EffectInstanceCount.load(std::memory_order_relaxed);
		const mu_uint32 clothVerticesCount = g_Tasking._DynamicVerticesCount.load(std::memory_order_relaxed);

		if(IsLightUpload == true)
		{
#if defined(USE_COMPRESSED_TERRAINLIGHT_RGB8)
			ConvertLightToRGB8(g_Terrain._PrimaryTerrainLight, TerrainLightBuffer);
#else
			EVector4 *lightBuffer = TerrainLightBuffer;
			const EVector3 *lightSource = g_Terrain._PrimaryTerrainLight.data();
			for (mu_uint32 n = 0; n < MAX_TERRAIN_SIZE; ++n)
			{
				mu_memcpy(&lightBuffer[n], &lightSource[n], sizeof(EVector3));
			}
#endif
			
			std::fill(TerrainLightUpdate.begin(), TerrainLightUpdate.end(), true);
		}

		UpdateGlobalOnly();

		ModelInstanceBuffer[index].Flush(0, 0, sizeof(SHModelInstance) * modelInstancesCount);
		EffectInstanceBuffer[index].Flush(0, 0, sizeof(SHEffectInstance) * effectInstancesCount);
		ClothVertexBuffer[index].Flush(0, 0, sizeof(SHClothVertexData) * clothVerticesCount);
		EVulkanSkeleton::FlushBuffer();

		if (InHellas() == true &&
			g_pCSWaterTerrain != nullptr)
		{
			if (MU_UPDATECOUNTER > 0)
			{
				g_pCSWaterTerrain->UpdateBuffer();
				std::fill(WaterVertexUpdate.begin(), WaterVertexUpdate.end(), true);
			}

			if (WaterVertexUpdate[index] == true)
			{
				mu_memcpy(WaterVertexBuffer[index].VulkanInternal.MappedMemory, g_pCSWaterTerrain->GetWaterBuffer(), WaterMemorySize);
				WaterVertexBuffer[index].Flush(0, 0, WaterMemorySize);
				WaterVertexUpdate[index] = false;
			}
		}

		if(TerrainLightUpdate[index] == true)
		{
			EStagingBufferBackend &stagingBuffer = TerrainLightStagingBuffer[index];
			mu_memcpy(stagingBuffer.BufferInternal.MappedMemory, TerrainLightBuffer, sizeof(TerrainLightBuffer));
			stagingBuffer.Flush(0, VK_WHOLE_SIZE);
			TerrainLightTexture[index]->Flush(&stagingBuffer, 0, 0, 0);
			TerrainLightUpdate[index] = false;
		}
	}

	MUTexture *GetShadowMapTexture(const mu_uint32 index)
	{
		return ShadowMapTexture[index];
	}

	VkDescriptorSet GetShadowMapDescriptorSet(const mu_uint32 index)
	{
		return ShadowDescriptorsSet[index];
	}

	const MUTexture *GetTerrainLightTexture(const mu_uint32 index)
	{
		return TerrainLightTexture[index];
	}

	SHGlobalConstantBuffer *GetGlobalConstantBuffer(const EGlobalConstant globalConstant)
	{
		return reinterpret_cast<SHGlobalConstantBuffer*>(reinterpret_cast<mu_uint8*>(GlobalConstantBuffer[EVulkan::CurrentCommandBufferIndex].VulkanInternal.MappedMemory) + GlobalConstantUniformSize * ECastEnum(globalConstant));
	}

	void UIUpdateBuffers()
	{
		UIVertexBuffer.UpdateBuffer();
	}

	const mu_uint32 UIAllocateDataIndex(const mu_uint32 count)
	{
		return UIVertexBuffer.AllocateDataIndex(count);
	}

	SHUIDefaultVertex *UIRetrieveDataBuffer(const mu_uint32 index)
	{
		return UIVertexBuffer.RetrieveDataBuffer(index);
	}

	void UIRequestUpdateData(const mu_uint32 index, const mu_uint32 count)
	{
		UIVertexBuffer.RequestUpdateData(index, count);
	}

	void UIEmptyTrash()
	{
		UIVertexBuffer.EmptyCurrentTrash();
	}

	VkBuffer UIRetrieveCurrentBuffer()
	{
		return UIVertexBuffer.RetrieveBuffer(EVulkan::CurrentCommandBufferIndex);
	}

	void FontUpdateBuffers()
	{
		FontVertexBuffer.UpdateBuffer();
	}

	const mu_uint32 FontAllocateDataIndex(const mu_uint32 count)
	{
		return FontVertexBuffer.AllocateDataIndex(count);
	}

	void FontDeallocateDataIndex(const mu_uint32 index)
	{
		FontVertexBuffer.DeallocateDataIndex(index);
	}

	SHFontInstance *FontRetrieveDataBuffer(const mu_uint32 index)
	{
		return FontVertexBuffer.RetrieveDataBuffer(index);
	}

	const mu_uint32 FontRetrieveBufferBaseIndex(const mu_uint32 index)
	{
		return FontVertexBuffer.RetrieveBufferBaseIndex(index);
	}

	void FontRequestUpdateData(const mu_uint32 index, const mu_uint32 count)
	{
		FontVertexBuffer.RequestUpdateData(index, count);
	}

	void FontEmptyTrash()
	{
		FontVertexBuffer.EmptyCurrentTrash();
	}

	void FontDefragmentBuffer()
	{
		FontVertexBuffer.DefragmentBuffer();
	}

	VkBuffer FontRetrieveCurrentBuffer()
	{
		return FontVertexBuffer.RetrieveBuffer(EVulkan::CurrentCommandBufferIndex);
	}

	SHModelInstance *GetModelInstancesBuffer()
	{
		return reinterpret_cast<SHModelInstance*>(ModelInstanceBuffer[EVulkan::CurrentCommandBufferIndex].VulkanInternal.MappedMemory);
	}

	SHEffectInstance *GetEffectInstancesBuffer()
	{
		return reinterpret_cast<SHEffectInstance*>(EffectInstanceBuffer[EVulkan::CurrentCommandBufferIndex].VulkanInternal.MappedMemory);
	}

	SHClothVertexData *GetClothVerticesBuffer()
	{
		return reinterpret_cast<SHClothVertexData*>(ClothVertexBuffer[EVulkan::CurrentCommandBufferIndex].VulkanInternal.MappedMemory);
	}
};