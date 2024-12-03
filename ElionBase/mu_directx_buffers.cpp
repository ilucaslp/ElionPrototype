#include "stdafx.h"

#if DIRECTX_BACKEND_ENABLE == 1
namespace EDirectXBuffer
{
	EBufferBackend GlobalConstantBuffer[DIRECTX_BACKEND_BUFFERCOUNT][EGlobalConstantMax];
	EBufferBackend ModelInstanceBuffer[DIRECTX_BACKEND_BUFFERCOUNT];
	EBufferBackend EffectInstanceBuffer[DIRECTX_BACKEND_BUFFERCOUNT];
	EBufferBackend ClothVertexBuffer[DIRECTX_BACKEND_BUFFERCOUNT];

	constexpr mu_size WaterMemorySize = sizeof(SHWaterVertex) * MAX_WATER_GRID * MAX_WATER_GRID * 4;
	constexpr mu_size WaterIndexMemorySize = sizeof(mu_uint16) * MAX_WATER_GRID * MAX_WATER_GRID * 6;
	std::array<mu_boolean, DIRECTX_BACKEND_BUFFERCOUNT> WaterVertexUpdate;
	std::array<EBufferBackend, DIRECTX_BACKEND_BUFFERCOUNT> WaterVertexBuffer;
	EBufferBackend WaterIndexBuffer;

	EBufferBackend DummyIndexBuffer;

	MUTexture BonesTexture[DIRECTX_BACKEND_BUFFERCOUNT];
	EStagingBufferBackend BonesTextureStagingBuffer;

	MUTexture TerrainLightTexture[DIRECTX_BACKEND_BUFFERCOUNT];
	EStagingBufferBackend TerrainLightStagingBuffer;
	std::array<mu_boolean, DIRECTX_BACKEND_BUFFERCOUNT> TerrainLightUpdate;

	MUTexture ShadowMapTexture[DIRECTX_BACKEND_BUFFERCOUNT];
	MUTexture MultisampleTexture[DIRECTX_BACKEND_BUFFERCOUNT];

	EDirectXUtils::EDynamicBuffer<SHUIDefaultVertex, D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER, UI_VERTEX_INITIAL, UI_VERTEX_GROW> UIVertexBuffer;
	EDirectXUtils::EDynamicBufferExtended<SHFontInstance, D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER, FONT_INSTANCE_INITIAL, FONT_INSTANCE_GROW> FontVertexBuffer;

	const mu_boolean Create()
	{
		FillArray(WaterVertexUpdate, 0, WaterVertexUpdate.size(), true);
		FillArray(TerrainLightUpdate, 0, TerrainLightUpdate.size(), true);

		for (mu_uint32 n = 0; n < DIRECTX_BACKEND_BUFFERCOUNT; ++n)
		{
			for (mu_uint32 j = 0; j < EGlobalConstantMax; ++j)
			{
				if (GlobalConstantBuffer[n][j].Allocate(sizeof(SHGlobalConstantBuffer),
					EBufferUsage::eDynamic, EBufferType::eUniform) == false)
				{
					return false;
				}
			}
		}

		for (mu_uint32 n = 0; n < DIRECTX_BACKEND_BUFFERCOUNT; ++n)
		{
			if (ModelInstanceBuffer[n].Allocate(sizeof(SHModelInstance) * MAX_MODEL_INSTANCES,
				EBufferUsage::eDynamic, EBufferType::eInstance) == false)
			{
				return false;
			}

			if (EffectInstanceBuffer[n].Allocate(sizeof(SHEffectInstance) * MAX_EFFECTS_INSTANCES,
				EBufferUsage::eDynamic, EBufferType::eInstance) == false)
			{
				return false;
			}

			if (ClothVertexBuffer[n].Allocate(sizeof(SHClothVertexData) * MAX_CLOTH_VERTICES,
				EBufferUsage::eDynamic, EBufferType::eVertex) == false)
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

		const mu_uint16 dummyIndexes[6] = { 0, 1, 2, 0, 2, 3 };
		if (DummyIndexBuffer.Allocate(sizeof(dummyIndexes), EBufferUsage::eStatic, EBufferType::eIndex, dummyIndexes) == false)
		{
			return false;
		}

		for (mu_uint32 n = 0; n < DIRECTX_BACKEND_BUFFERCOUNT; ++n)
		{
			if (EDirectXTexture::CreateBones(&BonesTexture[n]) == false)
			{
				return false;
			}
		}

		if (BonesTextureStagingBuffer.Allocate(SkeletonMemorySize) == false)
		{
			return false;
		}

		for (mu_uint32 n = 0; n < DIRECTX_BACKEND_BUFFERCOUNT; ++n)
		{
			if (TerrainLightTexture[n].Create2D(TERRAIN_SIZE, TERRAIN_SIZE,
				TERRAIN_LIGHT_FORMAT, ETextureUsage::eDynamic, TERRAIN_LIGHT_SAMPLER,
				nullptr, 0) == false)
			{
				return false;
			}
		}

		if (TerrainLightStagingBuffer.Allocate(TERRAIN_LIGHT_FORMAT_MEMORYSIZE, true) == false)
		{
			return false;
		}

		if (UIVertexBuffer.Initialize(DIRECTX_BACKEND_BUFFERCOUNT) == false)
		{
			return false;
		}

		if (FontVertexBuffer.Initialize(DIRECTX_BACKEND_BUFFERCOUNT) == false)
		{
			return false;
		}

		return true;
	}

	void Destroy(const mu_boolean gpuOnly)
	{
		UIVertexBuffer.Destroy(gpuOnly);
		FontVertexBuffer.Destroy(gpuOnly);

		for (mu_uint32 n = 0; n < DIRECTX_BACKEND_BUFFERCOUNT; ++n)
		{
			for (mu_uint32 j = 0; j < EGlobalConstantMax; ++j)
			{
				GlobalConstantBuffer[n][j].Deallocate();
			}
		}

		for (mu_uint32 n = 0; n < DIRECTX_BACKEND_BUFFERCOUNT; ++n)
		{
			WaterVertexBuffer[n].Deallocate();
			ModelInstanceBuffer[n].Deallocate();
			EffectInstanceBuffer[n].Deallocate();
			ClothVertexBuffer[n].Deallocate();
		}

		for (mu_uint32 n = 0; n < DIRECTX_BACKEND_BUFFERCOUNT; ++n)
		{
			TerrainLightTexture[n].Release();
		}
		TerrainLightStagingBuffer.Deallocate();

		WaterIndexBuffer.Deallocate();
		DummyIndexBuffer.Deallocate();
		BonesTextureStagingBuffer.Deallocate();

		for (mu_uint32 n = 0; n < DIRECTX_BACKEND_BUFFERCOUNT; ++n)
		{
			BonesTexture[n].Release();
		}
	}

	const mu_boolean CreateShadowMap()
	{
		if (MU_RESOURCE->GetShadowType() >= EShadowType::eShadowMappingBegin &&
			MU_RESOURCE->GetShadowType() <= EShadowType::eShadowMappingEnd)
		{
			for (mu_uint32 n = 0; n < mu_countof(ShadowMapTexture); ++n)
			{
				if (EDirectXTexture::CreateShadowMap(&ShadowMapTexture[n]) == false)
				{
					return false;
				}
			}
		}

		return true;
	}

	void DestroyShadowMap()
	{
		for (mu_uint32 n = 0; n < mu_countof(ShadowMapTexture); ++n)
		{
			ShadowMapTexture[n].Release();
		}
	}

	const mu_boolean CreateMultisample()
	{
		if (MU_RESOURCE->IsAntialiasing() == true)
		{
			const DXGI_FORMAT msaaFormat = EDirectXInit::SwapchainDesc.BufferDesc.Format;
			mu_uint32 msaaSampleCount = 16;
			mu_uint32 msaaQuality = 0;

			for (; msaaSampleCount >= 2; msaaSampleCount /= 2)
			{
				HRESULT result = EDirectXInit::Device->CheckMultisampleQualityLevels(msaaFormat, msaaSampleCount, &msaaQuality);
				if (SUCCEEDED(result) && msaaQuality > 0)
				{
					result = EDirectXInit::Device->CheckMultisampleQualityLevels(DXGI_FORMAT_D32_FLOAT, msaaSampleCount, &msaaQuality);
					if (SUCCEEDED(result) && msaaQuality > 0)
					{
						break;
					}
				}
			}

			if (msaaSampleCount < 2)
			{
				return true;
			}

			if (msaaQuality > 0)
			{
				--msaaQuality;
			}

			for (mu_uint32 n = 0; n < mu_countof(MultisampleTexture); ++n)
			{
				if (EDirectXTexture::CreateMultisample(&MultisampleTexture[n], msaaSampleCount, msaaQuality) == false)
				{
					return false;
				}
			}
		}

		return true;
	}

	void DestroyMultisample()
	{
		for (mu_uint32 n = 0; n < mu_countof(MultisampleTexture); ++n)
		{
			MultisampleTexture[n].Release();
		}
	}

	void UpdateGlobalOnly()
	{
		const mu_uint32 currentIndex = EDirectX::CurrentCommandBufferIndex;

		for (mu_uint32 j = 0; j < EGlobalConstantMax; ++j)
		{
			GlobalConstantBuffer[currentIndex][j].Flush(0, 0, sizeof(SHGlobalConstantBuffer));
		}
	}

	void Update(const mu_boolean IsLightUpload)
	{
		UpdateGlobalOnly();

		if (IsLightUpload == true)
		{
#if defined(USE_COMPRESSED_TERRAINLIGHT_RGB8)
			ConvertLightToRGB8(g_Terrain._PrimaryTerrainLight, reinterpret_cast<mu_uint32*>(TerrainLightStagingBuffer.BufferInternal.MappedMemory));
#else
			EVector4 *lightBuffer = reinterpret_cast<EVector4*>(TerrainLightStagingBuffer.BufferInternal.MappedMemory);
			const EVector3 *lightSource = g_Terrain._PrimaryTerrainLight.data();
			for (mu_uint32 n = 0; n < MAX_TERRAIN_SIZE; ++n)
			{
				mu_memcpy(&lightBuffer[n], &lightSource[n], sizeof(EVector3));
			}
#endif

			std::fill(TerrainLightUpdate.begin(), TerrainLightUpdate.end(), true);
		}

		const mu_uint32 currentIndex = EDirectX::CurrentCommandBufferIndex;

		ModelInstanceBuffer[currentIndex].Flush(0, 0, sizeof(SHModelInstance) * g_Tasking._ModelInstanceCount.load(std::memory_order_relaxed));
		EffectInstanceBuffer[currentIndex].Flush(0, 0, sizeof(SHEffectInstance) * g_Tasking._EffectInstanceCount.load(std::memory_order_relaxed));
		ClothVertexBuffer[currentIndex].Flush(0, 0, sizeof(SHClothVertexData) * g_Tasking._DynamicVerticesCount.load(std::memory_order_relaxed));

		if (InHellas() == true &&
			g_pCSWaterTerrain != nullptr)
		{
			if (MU_UPDATECOUNTER > 0)
			{
				g_pCSWaterTerrain->UpdateBuffer();
				FillArray(WaterVertexUpdate, 0, WaterVertexUpdate.size(), true);
			}

			if (WaterVertexUpdate[currentIndex] == true)
			{
				mu_memcpy(WaterVertexBuffer[currentIndex].DirectXInternal.MappedMemory, g_pCSWaterTerrain->GetWaterBuffer(), WaterMemorySize);
				WaterVertexBuffer[currentIndex].Flush(0, 0, WaterMemorySize);
				WaterVertexUpdate[currentIndex] = false;
			}
		}

		// Update Bones Texture
		{
			const mu_uint32 pitch = SKELETON_TEXTURE_TEXELSIZE * SKELETON_TEXTURE_WIDTH;
#ifdef USE_DUAL_QUATERNION
			const mu_uint32 skeletonCount = g_Tasking._SkeletonCount.load(std::memory_order_relaxed) * 2;
#else
			const mu_uint32 skeletonCount = g_Tasking._SkeletonCount.load(std::memory_order_relaxed) * 4;
#endif
			const mu_uint32 width = skeletonCount > SKELETON_TEXTURE_WIDTH ? SKELETON_TEXTURE_WIDTH : skeletonCount;
			const mu_uint32 height = (skeletonCount / SKELETON_TEXTURE_WIDTH) + (mu_uint32)(skeletonCount % SKELETON_TEXTURE_WIDTH > 0);
			BonesTexture[currentIndex].FlushAsTexture2D(&BonesTextureStagingBuffer, 0, 0, 0, width, height, SKELETON_TEXTURE_FORMAT, pitch);
		}

		if (TerrainLightUpdate[currentIndex] == true)
		{
			TerrainLightTexture[currentIndex].Flush(&TerrainLightStagingBuffer, 0, 0, TERRAIN_LIGHT_FORMAT_MEMORYSIZE, TERRAIN_LIGHT_FORMAT);
			TerrainLightUpdate[currentIndex] = false;
		}
	}

	void BindGlobalConstant(const EGlobalConstant globalConstant)
	{
		const mu_uint32 currentIndex = EDirectX::CurrentCommandBufferIndex;
		EDirectXPipeline::SetConstantBuffers(0, 1, &GlobalConstantBuffer[currentIndex][ECastEnum(globalConstant)].DirectXInternal.Resource, D3D_CONSTANT_MASK_VS | D3D_CONSTANT_MASK_PS | D3D_CONSTANT_MASK_GS);
	}

	ID3D11Buffer *GetModelInstancesResource()
	{
		return ModelInstanceBuffer[EDirectX::CurrentCommandBufferIndex].DirectXInternal.Resource;
	}

	ID3D11Buffer *GetClothInstancesResource()
	{
		return ClothVertexBuffer[EDirectX::CurrentCommandBufferIndex].DirectXInternal.Resource;
	}

	ID3D11Buffer *GetEffectInstancesResource()
	{
		return EffectInstanceBuffer[EDirectX::CurrentCommandBufferIndex].DirectXInternal.Resource;
	}

	ID3D11Buffer *GetWaterVertexResource()
	{
		return WaterVertexBuffer[EDirectX::CurrentCommandBufferIndex].DirectXInternal.Resource;
	}

	ID3D11Buffer *GetWaterIndexResource()
	{
		return WaterIndexBuffer.DirectXInternal.Resource;
	}

	ID3D11Buffer *GetDummyIndexResource()
	{
		return DummyIndexBuffer.DirectXInternal.Resource;
	}

	MUTexture *GetShadowMapTexture(const mu_uint32 index)
	{
		return &ShadowMapTexture[index];
	}

	MUTexture *GetMultisampleTexture(const mu_uint32 index)
	{
		return &MultisampleTexture[index];
	}

	const MUTexture *GetTerrainLightTexture(const mu_uint32 index)
	{
		return &TerrainLightTexture[index];
	}

	SHGlobalConstantBuffer *GetGlobalConstantBuffer(const EGlobalConstant globalConstant)
	{
		return reinterpret_cast<SHGlobalConstantBuffer*>(GlobalConstantBuffer[EDirectX::CurrentCommandBufferIndex][ECastEnum(globalConstant)].DirectXInternal.MappedMemory);
	}

	SHModelInstance *GetModelInstancesBuffer()
	{
		return reinterpret_cast<SHModelInstance*>(ModelInstanceBuffer[EDirectX::CurrentCommandBufferIndex].DirectXInternal.MappedMemory);
	}

	SHEffectInstance *GetEffectInstancesBuffer()
	{
		return reinterpret_cast<SHEffectInstance*>(EffectInstanceBuffer[EDirectX::CurrentCommandBufferIndex].DirectXInternal.MappedMemory);
	}

	SHClothVertexData *GetClothVerticesBuffer()
	{
		return reinterpret_cast<SHClothVertexData*>(ClothVertexBuffer[EDirectX::CurrentCommandBufferIndex].DirectXInternal.MappedMemory);
	}

	const MUTexture *GetCurrentBonesTexture()
	{
		return &BonesTexture[EDirectX::CurrentCommandBufferIndex];
	}

#ifdef USE_DUAL_QUATERNION
	EDualQuat *GetBonesBuffer()
	{
		return reinterpret_cast<EDualQuat*>(BonesTextureStagingBuffer.BufferInternal.MappedMemory);
	}
#else
	EMatrix4 *GetBonesBuffer()
	{
		return reinterpret_cast<EMatrix4*>(BonesTextureStagingBuffer.BufferInternal.MappedMemory);
	}
#endif

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

	ID3D11Buffer *UIRetrieveCurrentBuffer()
	{
		return UIVertexBuffer.RetrieveBuffer(EDirectX::CurrentCommandBufferIndex);
	}

	void FontUpdateBuffers()
	{
		FontVertexBuffer.UpdateBuffer();
	}

	void FontDefragmentBuffer()
	{
		FontVertexBuffer.DefragmentBuffer();
	}

	const mu_uint32 FontAllocateDataIndex(const mu_uint32 count)
	{
		return FontVertexBuffer.AllocateDataIndex(count);
	}

	void FontDeallocateDataIndex(const mu_uint32 index)
	{
		return FontVertexBuffer.DeallocateDataIndex(index);
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

	ID3D11Buffer *FontRetrieveCurrentBuffer()
	{
		return FontVertexBuffer.RetrieveBuffer(EDirectX::CurrentCommandBufferIndex);
	}
};
#endif