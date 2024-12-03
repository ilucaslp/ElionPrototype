#include "stdafx.h"

#if OPENGL_BACKEND_ENABLE == 1
namespace EOpenGLBuffer
{
	EBufferBackend GlobalConstantBuffer[OPENGL_BACKEND_BUFFERCOUNT][EGlobalConstantMax];
	EBufferBackend ModelInstanceBuffer[OPENGL_BACKEND_BUFFERCOUNT];
	EBufferBackend EffectInstanceBuffer[OPENGL_BACKEND_BUFFERCOUNT];
	EBufferBackend ClothVertexBuffer[OPENGL_BACKEND_BUFFERCOUNT];

	constexpr mu_size WaterMemorySize = sizeof(SHWaterVertex) * MAX_WATER_GRID * MAX_WATER_GRID * 4;
	constexpr mu_size WaterIndexMemorySize = sizeof(mu_uint16) * MAX_WATER_GRID * MAX_WATER_GRID * 6;
	std::array<mu_boolean, OPENGL_BACKEND_BUFFERCOUNT> WaterVertexUpdate;
	std::array<EBufferBackend, OPENGL_BACKEND_BUFFERCOUNT> WaterVertexBuffer;
	EBufferBackend WaterIndexBuffer;

	EBufferBackend DummyIndexBuffer;

	GLuint TerrainVAO = 0;
	GLuint GrassVAO = 0;
	GLuint EffectVAO[OPENGL_BACKEND_BUFFERCOUNT] = { 0 };
	GLuint ClothVAO[OPENGL_BACKEND_BUFFERCOUNT] = { 0 };
	GLuint WaterVAO[OPENGL_BACKEND_BUFFERCOUNT] = { 0 };

	MUTexture BonesTexture[OPENGL_BACKEND_BUFFERCOUNT];
	void *BonesBufferMap[OPENGL_BACKEND_BUFFERCOUNT] = { nullptr };

	MUTexture TerrainLightTexture[OPENGL_BACKEND_BUFFERCOUNT];
	EStagingBufferBackend TerrainLightStagingBuffer;
	std::array<mu_boolean, OPENGL_BACKEND_BUFFERCOUNT> TerrainLightUpdate;

	MUTexture ShadowMapTexture[OPENGL_BACKEND_BUFFERCOUNT];
	MUTexture MultisampleTexture[OPENGL_BACKEND_BUFFERCOUNT];

	EOpenGLUtils::EDynamicBuffer<SHUIDefaultVertex, GL_ARRAY_BUFFER, UI_VERTEX_INITIAL, UI_VERTEX_GROW> UIVertexBuffer;
	std::array<GLuint, OPENGL_BACKEND_BUFFERCOUNT> UIVertexArray = { { 0, 0 } };

	EOpenGLUtils::EDynamicBufferExtended<SHFontInstance, GL_ARRAY_BUFFER, FONT_INSTANCE_INITIAL, FONT_INSTANCE_GROW> FontVertexBuffer;
	std::array<GLuint, OPENGL_BACKEND_BUFFERCOUNT> FontVertexArray = { { 0, 0 } };

	const mu_boolean Create()
	{
		FillArray(WaterVertexUpdate, 0, WaterVertexUpdate.size(), true);
		FillArray(TerrainLightUpdate, 0, TerrainLightUpdate.size(), true);

		for (mu_uint32 n = 0; n < OPENGL_BACKEND_BUFFERCOUNT; ++n)
		{
			for (mu_uint32 j = 0; j < EGlobalConstantMax; ++j)
			{
				if(GlobalConstantBuffer[n][j].Allocate(sizeof(SHGlobalConstantBuffer),
					EBufferUsage::eDynamic, EBufferType::eUniform) == false)
				{
					return false;
				}
			}
		}

		for (mu_uint32 n = 0; n < OPENGL_BACKEND_BUFFERCOUNT; ++n)
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
		if(DummyIndexBuffer.Allocate(sizeof(dummyIndexes), EBufferUsage::eStatic, EBufferType::eIndex, dummyIndexes) == false)
		{
			return false;
		}

		EOpenGLHelpers::GenerateVAOs(1, &TerrainVAO);
		EOpenGLHelpers::GenerateVAOs(1, &GrassVAO);
		EOpenGLHelpers::GenerateVAOs(mu_countof(EffectVAO), EffectVAO);
		EOpenGLHelpers::GenerateVAOs(mu_countof(ClothVAO), ClothVAO);
		EOpenGLHelpers::GenerateVAOs(mu_countof(WaterVAO), WaterVAO);

		for(mu_uint32 n = 0;n < OPENGL_BACKEND_BUFFERCOUNT;++n)
		{
			EOpenGL::CurrentCommandBufferIndex = n;
			EOpenGLPipeline::BindVAO(EffectVAO[n]);
			EVideoAttributes::ConfigureAtLoad(EInputLayout::eEffect);

			EOpenGLPipeline::BindVAO(ClothVAO[n]);
			EVideoAttributes::ConfigureAtLoad(EInputLayout::eCloth);

			EOpenGLPipeline::BindVAO(WaterVAO[n]);
			EVideoAttributes::ConfigureAtLoad(EInputLayout::eWater);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, WaterIndexBuffer.OpenGLInternal.Resource);
		}

		EOpenGL::CurrentCommandBufferIndex = 0;
		EOpenGLPipeline::BindVAO(EInvalidUInt32);

		for(mu_uint32 n = 0;n < OPENGL_BACKEND_BUFFERCOUNT;++n)
		{
			if (EOpenGLTexture::CreateBones(&BonesTexture[n]) == false)
			{
				return false;
			}
		}

		if (EOpenGLExtensions::IsFeatureSupported(OpenGLFeatures::BonesTextureBuffer) == true &&
			EOpenGLExtensions::IsFeatureSupported(OpenGLFeatures::BufferStorage) == true)
		{
			for (mu_uint32 n = 0; n < OPENGL_BACKEND_BUFFERCOUNT; ++n)
			{
				glBindBuffer(GL_TEXTURE_BUFFER, BonesTexture[n].OpenGLInternal.BufferResource);
				BonesBufferMap[n] = glMapBufferRange(GL_TEXTURE_BUFFER, 0, SkeletonMemorySize, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_FLUSH_EXPLICIT_BIT);
				if (BonesBufferMap[n] == nullptr)
				{
					glBindBuffer(GL_TEXTURE_BUFFER, 0);
					return false;
				}
			}

			glBindBuffer(GL_TEXTURE_BUFFER, 0);
		}
		else
		{
			BonesBufferMap[0] = mu_malloc(SkeletonMemorySize);
			for (mu_uint32 n = 1; n < OPENGL_BACKEND_BUFFERCOUNT; ++n)
			{
				BonesBufferMap[n] = BonesBufferMap[0];
			}
		}

		for(mu_uint32 n = 0;n < OPENGL_BACKEND_BUFFERCOUNT;++n)
		{
			if(TerrainLightTexture[n].Create2D(TERRAIN_SIZE, TERRAIN_SIZE,
				TERRAIN_LIGHT_FORMAT, ETextureUsage::eDynamic, TERRAIN_LIGHT_SAMPLER,
				nullptr, 0) == false)
			{
				return false;
			}
		}

		if(TerrainLightStagingBuffer.Allocate(TERRAIN_LIGHT_FORMAT_MEMORYSIZE, true) == false)
		{
			return false;
		}

		if(UIVertexBuffer.Initialize(OPENGL_BACKEND_BUFFERCOUNT) == false)
		{
			return false;
		}

		if(FontVertexBuffer.Initialize(OPENGL_BACKEND_BUFFERCOUNT) == false)
		{
			return false;
		}

		EOpenGLHelpers::GenerateVAOs(OPENGL_BACKEND_BUFFERCOUNT, UIVertexArray.data());
		EOpenGLHelpers::GenerateVAOs(OPENGL_BACKEND_BUFFERCOUNT, FontVertexArray.data());

		for(mu_uint32 n = 0;n < OPENGL_BACKEND_BUFFERCOUNT;++n)
		{
			EOpenGL::CurrentCommandBufferIndex = n;
			EOpenGLPipeline::BindVAO(UIVertexArray[n]);
			EVideoAttributes::ConfigureAtLoad(EInputLayout::eUIDefault);
		}

		for (mu_uint32 n = 0; n < OPENGL_BACKEND_BUFFERCOUNT; ++n)
		{
			EOpenGL::CurrentCommandBufferIndex = n;
			EOpenGLPipeline::BindVAO(FontVertexArray[n]);
			EVideoAttributes::ConfigureAtLoad(EInputLayout::eFont);
		}

		EOpenGL::CurrentCommandBufferIndex = 0;
		EOpenGLPipeline::BindVAO(EInvalidUInt32);

		return true;
	}

	void Destroy(const mu_boolean gpuOnly)
	{
		UIVertexBuffer.Destroy(gpuOnly);
		FontVertexBuffer.Destroy(gpuOnly);

		for (mu_uint32 n = 0; n < OPENGL_BACKEND_BUFFERCOUNT; ++n)
		{
			for (mu_uint32 j = 0; j < EGlobalConstantMax; ++j)
			{
				GlobalConstantBuffer[n][j].Deallocate();
			}
		}

		for (mu_uint32 n = 0; n < OPENGL_BACKEND_BUFFERCOUNT; ++n)
		{
			WaterVertexBuffer[n].Deallocate();
			ModelInstanceBuffer[n].Deallocate();
			EffectInstanceBuffer[n].Deallocate();
			ClothVertexBuffer[n].Deallocate();
		}

		WaterIndexBuffer.Deallocate();
		DummyIndexBuffer.Deallocate();

		if (EOpenGLExtensions::IsFeatureSupported(OpenGLFeatures::BufferStorage) == true)
		{
			for (mu_uint32 n = 0; n < OPENGL_BACKEND_BUFFERCOUNT; ++n)
			{
				if (BonesBufferMap[n] != nullptr)
				{
					glBindBuffer(GL_TEXTURE_BUFFER, BonesTexture[n].OpenGLInternal.BufferResource);
					glUnmapBuffer(GL_TEXTURE_BUFFER);
					BonesBufferMap[n] = nullptr;
				}
			}

			glBindBuffer(GL_TEXTURE_BUFFER, 0);
		}
		else
		{
			if (BonesBufferMap[0] != nullptr)
			{
				mu_free(BonesBufferMap[0]);
				mu_zeromem(BonesBufferMap, sizeof(BonesBufferMap));
			}
		}

		for(mu_uint32 n = 0;n < OPENGL_BACKEND_BUFFERCOUNT;++n)
		{
			BonesTexture[n].Release();
		}

		for (mu_uint32 n = 0; n < OPENGL_BACKEND_BUFFERCOUNT; ++n)
		{
			TerrainLightTexture[n].Release();
		}
		TerrainLightStagingBuffer.Deallocate();

		EOpenGLHelpers::DeleteVAOs(1, &TerrainVAO);
		EOpenGLHelpers::DeleteVAOs(1, &GrassVAO);
		EOpenGLHelpers::DeleteVAOs(mu_countof(EffectVAO), EffectVAO);
		EOpenGLHelpers::DeleteVAOs(mu_countof(ClothVAO), ClothVAO);
		EOpenGLHelpers::DeleteVAOs(mu_countof(WaterVAO), WaterVAO);
		EOpenGLHelpers::DeleteVAOs(OPENGL_BACKEND_BUFFERCOUNT, UIVertexArray.data());
		EOpenGLHelpers::DeleteVAOs(OPENGL_BACKEND_BUFFERCOUNT, FontVertexArray.data());
	}

	const mu_boolean CreateShadowMap()
	{
		if (MU_RESOURCE->GetShadowType() >= EShadowType::eShadowMappingBegin &&
			MU_RESOURCE->GetShadowType() <= EShadowType::eShadowMappingEnd)
		{
			for (mu_uint32 n = 0; n < mu_countof(ShadowMapTexture); ++n)
			{
				if (EOpenGLTexture::CreateShadowMap(&ShadowMapTexture[n]) == false)
				{
					return false;
				}
			}
		}

		return true;
	}

	void DestroyShadowMap()
	{
		for (mu_uint32 n = 0; n < OPENGL_BACKEND_BUFFERCOUNT; ++n)
		{
			ShadowMapTexture[n].Release();
		}
	}

	const mu_boolean CreateMultisample()
	{
		if (MU_RESOURCE->IsAntialiasing() == true)
		{
			GLint samples = 0;
			glGetIntegerv(GL_MAX_SAMPLES, &samples);
			if (samples < 4)
			{
				return true;
			}

			for (mu_uint32 n = 0; n < mu_countof(MultisampleTexture); ++n)
			{
				if (EOpenGLTexture::CreateMultisample(&MultisampleTexture[n], samples) == false)
				{
					return false;
				}
			}
		}

		return true;
	}

	void DestroyMultisample()
	{
		for (mu_uint32 n = 0; n < OPENGL_BACKEND_BUFFERCOUNT; ++n)
		{
			MultisampleTexture[n].Release();
		}
	}

	void UpdateGlobalOnly()
	{
		const mu_uint32 currentIndex = EOpenGL::CurrentCommandBufferIndex;

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
			for(mu_uint32 n = 0;n < MAX_TERRAIN_SIZE;++n)
			{
				mu_memcpy(&lightBuffer[n], &lightSource[n], sizeof(EVector3));
			}
#endif

			std::fill(TerrainLightUpdate.begin(), TerrainLightUpdate.end(), true);
		}

		const mu_uint32 currentIndex = EOpenGL::CurrentCommandBufferIndex;

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
				mu_memcpy(WaterVertexBuffer[currentIndex].OpenGLInternal.MappedMemory, g_pCSWaterTerrain->GetWaterBuffer(), WaterMemorySize);
				WaterVertexBuffer[currentIndex].Flush(0, 0, WaterMemorySize);
				WaterVertexUpdate[currentIndex] = false;
			}
		}

		if (EOpenGLExtensions::IsFeatureSupported(OpenGLFeatures::BonesTextureBuffer) == true)
		{
#ifdef USE_DUAL_QUATERNION
			const mu_size skeletonSize = g_Tasking._SkeletonCount.load(std::memory_order_relaxed) * sizeof(EDualQuat);
#else
			const mu_size skeletonSize = g_Tasking._SkeletonCount.load(std::memory_order_relaxed) * sizeof(EMatrix4);
#endif
			if(skeletonSize > 0)
			{
				if (EOpenGLExtensions::IsFeatureSupported(OpenGLFeatures::BufferStorage) == true)
				{
					glBindBuffer(GL_TEXTURE_BUFFER, BonesTexture[currentIndex].OpenGLInternal.BufferResource);
					glFlushMappedBufferRange(GL_TEXTURE_BUFFER, 0, skeletonSize);
				}
				else
				{
					glBindBuffer(GL_TEXTURE_BUFFER, BonesTexture[currentIndex].OpenGLInternal.BufferResource);
					glBufferSubData(GL_TEXTURE_BUFFER, 0, skeletonSize, BonesBufferMap[currentIndex]);
				}

				glBindBuffer(GL_TEXTURE_BUFFER, 0);
			}
		}
		else
		{
#ifdef USE_DUAL_QUATERNION
			const mu_uint32 skeletonCount = g_Tasking._SkeletonCount.load(std::memory_order_relaxed) * 2;
#else
			const mu_uint32 skeletonCount = g_Tasking._SkeletonCount.load(std::memory_order_relaxed) * 4;
#endif
			if(skeletonCount > 0)
			{
				EOpenGLPipeline::SetUploadTexture(nullptr); // bind texture 0 as active
				glBindTexture(GL_TEXTURE_2D, BonesTexture[currentIndex].OpenGLInternal.TextureResource);
				glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
				mu_uint32 width = skeletonCount > SKELETON_TEXTURE_WIDTH ? SKELETON_TEXTURE_WIDTH : skeletonCount;
				mu_uint32 height = (skeletonCount / SKELETON_TEXTURE_WIDTH) + (mu_uint32)(skeletonCount % SKELETON_TEXTURE_WIDTH > 0);
				glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, EOpenGLTexture::RetrieveFlatFormat(SKELETON_TEXTURE_FORMAT), EOpenGLTexture::RetrieveScalarFormat(SKELETON_TEXTURE_FORMAT), BonesBufferMap[currentIndex]);
				glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
				glBindTexture(GL_TEXTURE_2D, 0);
			}
		}

		if(TerrainLightUpdate[currentIndex] == true)
		{
			TerrainLightTexture[currentIndex].Flush(&TerrainLightStagingBuffer, 0, 0, TERRAIN_LIGHT_FORMAT_MEMORYSIZE, TERRAIN_LIGHT_FORMAT);
			TerrainLightUpdate[currentIndex] = false;
		}
	}

	void BindVertexBuffer(const EInputLayout inputLayout)
	{
		const mu_uint32 currentIndex = EOpenGL::CurrentCommandBufferIndex;

		switch (inputLayout)
		{
		case EInputLayout::eCloth:
			{
				glBindBuffer(GL_ARRAY_BUFFER, ClothVertexBuffer[currentIndex].OpenGLInternal.Resource);
			}
			break;

		case EInputLayout::eUIDefault:
			{
				glBindBuffer(GL_ARRAY_BUFFER, UIRetrieveCurrentBuffer());
			}
			break;

		case EInputLayout::eWater:
			{
				glBindBuffer(GL_ARRAY_BUFFER, WaterVertexBuffer[currentIndex].OpenGLInternal.Resource);
			}
			break;

		default: break;
		}
	}

	void BindInstanceBuffer(const EInputLayout inputLayout)
	{
		const mu_uint32 currentIndex = EOpenGL::CurrentCommandBufferIndex;

		switch(inputLayout)
		{
		case EInputLayout::eModel:
		case EInputLayout::eCloth:
			{
				glBindBuffer(GL_ARRAY_BUFFER, ModelInstanceBuffer[currentIndex].OpenGLInternal.Resource);
			}
			break;

		case EInputLayout::eEffect:
			{
				glBindBuffer(GL_ARRAY_BUFFER, EffectInstanceBuffer[currentIndex].OpenGLInternal.Resource);
			}
			break;

		case EInputLayout::eFont:
			{
				glBindBuffer(GL_ARRAY_BUFFER, FontRetrieveCurrentBuffer());
			}
			break;

		case EInputLayout::eTerrainCommon:
			{
				glBindBuffer(GL_ARRAY_BUFFER, g_Terrain.GetTerrainInstanceBuffer()->OpenGLInternal.Resource);
			}
			break;

		case EInputLayout::eGrass:
			{
				glBindBuffer(GL_ARRAY_BUFFER, g_Terrain.GetGrassInstanceBuffer()->OpenGLInternal.Resource);
			}
			break;

		default: break;
		}
	}

	void BindGlobalConstant(const EGlobalConstant globalConstant)
	{
		const mu_uint32 currentIndex = EOpenGL::CurrentCommandBufferIndex;

		glBindBufferBase(GL_UNIFORM_BUFFER, 0, GlobalConstantBuffer[currentIndex][ECastEnum(globalConstant)].OpenGLInternal.Resource);
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
		return reinterpret_cast<SHGlobalConstantBuffer*>(GlobalConstantBuffer[EOpenGL::CurrentCommandBufferIndex][ECastEnum(globalConstant)].OpenGLInternal.MappedMemory);
	}

	SHModelInstance *GetModelInstancesBuffer()
	{
		return reinterpret_cast<SHModelInstance*>(ModelInstanceBuffer[EOpenGL::CurrentCommandBufferIndex].OpenGLInternal.MappedMemory);
	}

	SHEffectInstance *GetEffectInstancesBuffer()
	{
		return reinterpret_cast<SHEffectInstance*>(EffectInstanceBuffer[EOpenGL::CurrentCommandBufferIndex].OpenGLInternal.MappedMemory);
	}

	SHClothVertexData *GetClothVerticesBuffer()
	{
		return reinterpret_cast<SHClothVertexData*>(ClothVertexBuffer[EOpenGL::CurrentCommandBufferIndex].OpenGLInternal.MappedMemory);
	}

	const MUTexture *GetCurrentBonesTexture()
	{
		return &BonesTexture[EOpenGL::CurrentCommandBufferIndex];
	}

#ifdef USE_DUAL_QUATERNION
	EDualQuat *GetBonesBuffer()
	{
		return reinterpret_cast<EDualQuat*>(BonesBufferMap[EOpenGL::CurrentCommandBufferIndex]);
	}
#else
	EMatrix4 *GetBonesBuffer()
	{
		return reinterpret_cast<EMatrix4*>(BonesBufferMap[EOpenGL::CurrentCommandBufferIndex]);
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

	GLuint UIRetrieveCurrentBuffer()
	{
		return UIVertexBuffer.RetrieveBuffer(EOpenGL::CurrentCommandBufferIndex);
	}

	GLuint UIRetrieveVAO()
	{
		return UIVertexArray[EOpenGL::CurrentCommandBufferIndex];
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

	GLuint FontRetrieveCurrentBuffer()
	{
		return FontVertexBuffer.RetrieveBuffer(EOpenGL::CurrentCommandBufferIndex);
	}

	GLuint FontRetrieveVAO()
	{
		return FontVertexArray[EOpenGL::CurrentCommandBufferIndex];
	}

	void UpdateTerrainVAO()
	{
		EOpenGLPipeline::BindVAO(TerrainVAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, DummyIndexBuffer.OpenGLInternal.Resource);
		EVideoAttributes::ConfigureAtRuntime<EVideoBackendType::eOpenGL>(EInputLayout::eTerrainCommon);
		EOpenGLPipeline::BindVAO(EInvalidUInt32);
	}

	void UpdateGrassVAO()
	{
		EOpenGLPipeline::BindVAO(GrassVAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, DummyIndexBuffer.OpenGLInternal.Resource);
		EVideoAttributes::ConfigureAtRuntime<EVideoBackendType::eOpenGL>(EInputLayout::eGrass);
		EOpenGLPipeline::BindVAO(EInvalidUInt32);
	}

	GLuint RetrieveTerrainVAO()
	{
		return TerrainVAO;
	}

	GLuint RetrieveGrassVAO()
	{
		return GrassVAO;
	}

	GLuint RetrieveClothVAO()
	{
		return ClothVAO[EOpenGL::CurrentCommandBufferIndex];
	}

	GLuint RetrieveEffectVAO()
	{
		return EffectVAO[EOpenGL::CurrentCommandBufferIndex];
	}

	GLuint WaterRetrieveVAO()
	{
		return WaterVAO[EOpenGL::CurrentCommandBufferIndex];
	}
};
#endif