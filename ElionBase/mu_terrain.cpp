#include "stdafx.h"
#include "mu_terrain.h"

const mu_uint8 MUTerrain::CryptKey[] = { 0x13, 0xc9, 0x0b, 0xc0, 0xb0, 0x99, 0x18, 0xb3, 0x43, 0x07, 0xd2, 0xb1, 0x78, 0xac, 0xb7, 0xaf, 0x45, 0x92, 0xe2, 0xeb, 0x07, 0x44, 0x91, 0x16, 0x4c, 0x99, 0x02, 0x0b, 0x75, 0x7e, 0x92, 0x37, 0x51, 0x87, 0x2e, 0x43, 0x91, 0x29, 0xf2, 0x70, 0xa0, 0xaa, 0x5c, 0x9c, 0xa5, 0xa9, 0x43, 0x9f, 0x0c, 0x5d, 0x5a, 0xa4, 0x4c, 0xb5, 0x6c, 0xa4 };
MUBlowfish MUTerrain::Crypt(CryptKey, mu_countof(CryptKey));

const mu_float MUTerrain::GRASS_EXPAND = 0.25f;
const mu_float MUTerrain::HEIGHT_MULTIPLY = 1.5f;
const mu_float MUTerrain::TERRAIN_SPECIAL_HEIGHT = 1200.0f;

MUTerrain g_Terrain;
#ifdef USE_FAST_TERRAIN_COLLISION
MU_ALIGN(16, std::array<std::array<mu_int16, 4>, MAX_TERRAIN_SIZE>) g_TerrainQuadHeights;
#endif
MU_ALIGN(16, std::array<std::array<EVector3, 2>, MAX_TERRAIN_SIZE>) g_TerrainCollisionNormal;
MU_ALIGN(16, std::array<mu_uint16, MAX_TERRAIN_SIZE + MAX_TASKING_THREADS>) g_TerrainCollisionQueue;

mu_boolean g_LightEnabled = false;
MUCriticalSectionSpinLock g_LightLock;
MUCriticalSectionSpinLock g_HeightLock;
MUCriticalSectionSpinLock g_AttributeLock;

TerrainModificationRequest g_TerrainModification[MAX_TERRAIN_MODIFICATION];
mu_atomic_uint32_t g_TerrainModificationCount(0);

#define TEMPLATE_INIT_HELPER(funcname, ...) \
template void MUTerrain::funcname <false, EVideoBackendType::eDirectX>(__VA_ARGS__); \
template void MUTerrain::funcname <false, EVideoBackendType::eOpenGL>(__VA_ARGS__); \
template void MUTerrain::funcname <false, EVideoBackendType::eVulkan>(__VA_ARGS__); \
template void MUTerrain::funcname <false, EVideoBackendType::eMetal>(__VA_ARGS__); \
template void MUTerrain::funcname <true, EVideoBackendType::eDirectX>(__VA_ARGS__); \
template void MUTerrain::funcname <true, EVideoBackendType::eOpenGL>(__VA_ARGS__); \
template void MUTerrain::funcname <true, EVideoBackendType::eVulkan>(__VA_ARGS__); \
template void MUTerrain::funcname <true, EVideoBackendType::eMetal>(__VA_ARGS__);

TEMPLATE_INIT_HELPER(Render, void);
TEMPLATE_INIT_HELPER(PostRender, void);

#undef TEMPLATE_INIT_HELPER

MUTerrain::MUTerrain()
{
	_HeightUpdate = false;

	_TextureData = nullptr;
	_GrassTextureData = nullptr;

	_TilesTexture = nullptr;
	_GrassTilesTexture = nullptr;

	for (mu_uint32 index = 0;index < TERRAIN_SIZE; ++index)
	{
		_GrassInstanceData[index][0] = EMPTY_GRASS;
		_GrassInstanceData[index][1] = 0;
	}

	_ClearColor[0] = 0.0f;
	_ClearColor[1] = 0.0f;
	_ClearColor[2] = 0.0f;
	_ClearColor[3] = 1.0f;

	_WorldIndex = -1;
	_WorldState = -1;
}

MUTerrain::~MUTerrain()
{
	Unload();
}

const mu_boolean MUTerrain::Load(mu_int32 WorldIndex, mu_int32 WorldState, mu_boolean ReloadObjects)
{
	mu_int32 world = WorldIndex;

	if ((world >= WD_11BLOODCASTLE1 && world <= WD_11BLOODCASTLE_END) ||
		world == WD_52BLOODCASTLE_MASTER_LEVEL)
	{
		world = WD_11BLOODCASTLE1;
	}
	else if ((world >= WD_24HELLAS && world <= WD_24HELLAS_END) ||
		(world == WD_24HELLAS_7))
	{
		world = WD_24HELLAS;
	}
	else if ((world >= WD_18CHAOS_CASTLE && world <= WD_18CHAOS_CASTLE_END) ||
		world == WD_53CAOSCASTLE_MASTER_LEVEL)
	{
		world = WD_18CHAOS_CASTLE;
	}
	else if (world >= WD_45CURSEDTEMPLE_LV1 && world <= WD_45CURSEDTEMPLE_LV6)
	{
		world = WD_45CURSEDTEMPLE_LV1 + 1;
	}

	if (_WorldIndex2 == world && WorldState == _WorldState)
	{
		return true;
	}

	Unload(ReloadObjects);

	mu_text TerrainDir[MAX_PATH] = { 0 };

	if (WorldState == 0)
	{
		mu_sprintf_s(TerrainDir, mu_countof(TerrainDir), _T("data/world%d/"), world + 1);
	}
	else
	{
		mu_sprintf_s(TerrainDir, mu_countof(TerrainDir), _T("data/world%d_%d/"), world + 1, WorldState);
	}

	_BasePath = TerrainDir;

	mu_string TerrainPath = _BasePath + _T("terrain.ewld");
	mu_string ObjectsPath = _BasePath + _T("terrain.eobj");

	SDL_RWops *file = nullptr;

	if (mu_rwfromfile(&file, TerrainPath.c_str(), "rb") == false)
	{
		Unload();
		return false;
	}

	static const mu_size HeaderSize = FileHeaders::HEADEREX_SIZE;
	static const mu_size CompressedHeaderSize = FileHeaders::COMPRESSED_HEADER_SIZE;

	mu_int64 fileLength = SDL_RWsize(file);
	mu_int64 encryptedLength = fileLength - HeaderSize;

	if (encryptedLength % MUBlowfish::BytesLength != 0)
	{
		Unload();
		return false;
	}

	std::vector<mu_uint8> dataBuffer(fileLength);
	SDL_RWread(file, (mu_char*)dataBuffer.data(), fileLength, 1);
	SDL_RWclose(file);

	FileHeaders::FileHeaderEx *header = (FileHeaders::FileHeaderEx*)dataBuffer.data();

	if (header->Signature != FILE_HEADERS::TERRAIN_HEADER)
	{
		Unload();
		return false;
	}

	if (GenerateCRC32(dataBuffer.data() + HeaderSize, encryptedLength) != header->CRC)
	{
		Unload();
		return false;
	}

	Crypt.Decode(dataBuffer.data() + HeaderSize, dataBuffer.data() + HeaderSize, encryptedLength);

	FileHeaders::CompressedHeader *compressedHeader = (FileHeaders::CompressedHeader*)(dataBuffer.data() + HeaderSize);

	std::vector<mu_uint8> decompressedBuffer(compressedHeader->DecompressedSize);

	if (FileHeaders::Decompress(dataBuffer.data() + HeaderSize + CompressedHeaderSize,
		decompressedBuffer.data(),
		compressedHeader->CompressedSize,
		compressedHeader->DecompressedSize) == false)
	{
		Unload();
		return false;
	}

	dataBuffer.clear();

	mu_uint8 *readBuffer = decompressedBuffer.data();

	FileHeaders::ReadBuffer(readBuffer, _ClearColor.data(), SizeOfArray(_ClearColor));

	FileHeaders::ReadBuffer(readBuffer, _TextureCount);
	_TextureData = new Terrain::TextureData1[_TextureCount];
	FileHeaders::ReadBuffer(readBuffer, _TextureData, sizeof(Terrain::TextureData1) * _TextureCount);

	FileHeaders::ReadBuffer(readBuffer, _GrassTextureCount);
	_GrassTextureData = new Terrain::GrassTextureData1[_GrassTextureCount];
	FileHeaders::ReadBuffer(readBuffer, _GrassTextureData, sizeof(Terrain::GrassTextureData1) * _GrassTextureCount);

	FileHeaders::ReadBuffer(readBuffer, _TerrainHeight.data(), SizeOfArray(_TerrainHeight));
	mu_memcpy(_PrimaryTerrainHeight.data(), _TerrainHeight.data(), SizeOfArray(_TerrainHeight));
	FileHeaders::ReadBuffer(readBuffer, _TerrainLight.data(), SizeOfArray(_TerrainLight));
	FileHeaders::ReadBuffer(readBuffer, _TerrainMapping1.data(), SizeOfArray(_TerrainMapping1));
	FileHeaders::ReadBuffer(readBuffer, _TerrainMapping2.data(), SizeOfArray(_TerrainMapping2));
	FileHeaders::ReadBuffer(readBuffer, _GrassTerrainMapping.data(), SizeOfArray(_GrassTerrainMapping));
	FileHeaders::ReadBuffer(readBuffer, _TerrainMappingAlpha.data(), SizeOfArray(_TerrainMappingAlpha));
	FileHeaders::ReadBuffer(readBuffer, _TerrainAttribute.data(), SizeOfArray(_TerrainAttribute));
	FileHeaders::ReadBuffer(readBuffer, _WaterIndex.data(), SizeOfArray(_WaterIndex));

	decompressedBuffer.clear();

	_WorldIndex = WorldIndex;
	_WorldIndex2 = world;
	_WorldState = WorldState;

	if (ReloadObjects == true &&
		mu_rwexists(ObjectsPath.c_str()) == true)
	{
		SDL_RWops *file = nullptr;

		if (mu_rwfromfile(&file, ObjectsPath.c_str(), "rb") == false)
		{
			return false;
		}

		static const mu_size HeaderSize = FileHeaders::HEADEREX_SIZE;
		static const mu_size CompressedHeaderSize = FileHeaders::COMPRESSED_HEADER_SIZE;

		mu_int64 fileLength = SDL_RWsize(file);
		mu_int64 encryptedLength = fileLength - HeaderSize;

		if (encryptedLength % MUBlowfish::BytesLength != 0)
		{
			return false;
		}

		mu_uint8 *dataBuffer = new mu_uint8[fileLength];
		SDL_RWread(file, dataBuffer, fileLength, 1);
		SDL_RWclose(file);

		FileHeaders::FileHeaderEx *header = (FileHeaders::FileHeaderEx*)dataBuffer;

		if (header->Signature != FILE_HEADERS::TERRAIN_HEADER)
		{
			delete[] dataBuffer;
			Unload();
			return false;
		}

		if (GenerateCRC32(dataBuffer + HeaderSize, encryptedLength) != header->CRC)
		{
			delete[] dataBuffer;
			Unload();
			return false;
		}

		Crypt.Decode(dataBuffer + HeaderSize, dataBuffer + HeaderSize, encryptedLength);

		FileHeaders::CompressedHeader *compressedHeader = (FileHeaders::CompressedHeader*)(dataBuffer + HeaderSize);

		mu_uint8 *decompressedBuffer = new mu_uint8[compressedHeader->DecompressedSize];

		mu_boolean decompressed = FileHeaders::Decompress(dataBuffer + HeaderSize + CompressedHeaderSize,
			decompressedBuffer,
			compressedHeader->CompressedSize,
			compressedHeader->DecompressedSize);
		delete[] dataBuffer;

		if (decompressed == false)
		{
			delete[] decompressedBuffer;
			Unload();
			return false;
		}

		mu_uint8 *readBuffer = decompressedBuffer;

		mu_uint32 dataCount = 0;
		FileHeaders::ReadBuffer(readBuffer, dataCount);

		mu_assert(dataCount <= MAX_OBJECTS);
		g_EnvironmentObjectsCount = dataCount;

		mu_uint32 *nodes = nullptr;
		mu_uint32 nodesCount = 0;

		for(mu_uint32 index = 0;index < dataCount; ++index)
		{
			Terrain::ObjectData1 objectData;
			FileHeaders::ReadBuffer(readBuffer, objectData.Type);
			FileHeaders::ReadBuffer(readBuffer, objectData.Position);
			FileHeaders::ReadBuffer(readBuffer, objectData.Angle);
			FileHeaders::ReadBuffer(readBuffer, objectData.Scale);

			EObjectCullingData culling;
			FileHeaders::ReadBuffer(readBuffer, culling);
			
			CreateObject(0, index, objectData.Type, objectData.Position, objectData.Angle, objectData.Scale, culling);
		}

		mu_free(nodes);

		switch (world)
		{
		case WD_2DEVIAS:
			{
				CreateObject(0, MODEL_NPC_SERBIS_DONKEY, EVector3(191.0f * g_TerrainScale, 16.0f * g_TerrainScale, 270.0f), EVector3(0.0f, 0.0f, 0.0f), 1.0f);
				CreateObject(0, MODEL_NPC_SERBIS_FLAG, EVector3(191.0f * g_TerrainScale, 17.0f * g_TerrainScale, 270.0f), EVector3(0.0f, 0.0f, 0.0f), 1.0f);
				CreateObject(0, MODEL_WARP, EVector3(53.0f * g_TerrainScale + 50.0f, 92.0f * g_TerrainScale + 20.0f, 0.0f), EVector3(0.0f, 0.0f, 10.0f), 1.0f);
			}
			break;

		case WD_3NORIA:
			{
				CreateObject(0, MODEL_WARP, EVector3(223.0f * g_TerrainScale, 30.0f * g_TerrainScale, 0.0f), EVector3(0.0f, 0.0f, 10.0f), 1.0f);
			}
			break;

		case WD_30BATTLECASTLE:
			{
				CreateObject(0, MODEL_MONSTER01 + 77, EVector3(65.0f * g_TerrainScale, 113.0f * g_TerrainScale, 270.0f), EVector3(0.0f, 0.0f, 0.0f), 1.0f);
				CreateObject(0, MODEL_MONSTER01 + 77, EVector3(71.0f * g_TerrainScale, 113.0f * g_TerrainScale, 270.0f), EVector3(0.0f, 0.0f, 0.0f), 1.0f);
				CreateObject(0, MODEL_MONSTER01 + 77, EVector3(91.0f * g_TerrainScale, 113.0f * g_TerrainScale, 270.0f), EVector3(0.0f, 0.0f, 0.0f), 1.0f);
				CreateObject(0, MODEL_MONSTER01 + 77, EVector3(118.0f * g_TerrainScale, 113.0f * g_TerrainScale, 270.0f), EVector3(0.0f, 0.0f, 0.0f), 1.0f);
				CreateObject(0, MODEL_MONSTER01 + 77, EVector3(123.0f * g_TerrainScale, 113.0f * g_TerrainScale, 270.0f), EVector3(0.0f, 0.0f, 0.0f), 1.0f);
			}
			break;

		case WD_57ICECITY:
			{
				CreateObject(0, MODEL_WARP4, EVector3(162.0f * g_TerrainScale, 83.0f * g_TerrainScale, 0.0f), EVector3(0.0f, 0.0f, 35.0f), 1.0f);
				CreateObject(0, MODEL_WARP4, EVector3(171.0f * g_TerrainScale, 24.0f * g_TerrainScale, 0.0f), EVector3(0.0f, 0.0f, 80.0f), 1.0f);
			}
			break;

		case WD_58ICECITY_BOSS:
			{
				CreateObject(0, MODEL_WARP4, EVector3(169.0f * g_TerrainScale, 24.0f * g_TerrainScale, 0.0f), EVector3(0.0f, 0.0f, 85.0f), 1.0f);
				CreateObject(0, MODEL_WARP4, EVector3(170.0f * g_TerrainScale, 24.0f * g_TerrainScale, 0.0f), EVector3(0.0f, 0.0f, 85.0f), 1.0f);
			}
			break;
		}

		delete[] decompressedBuffer;
	}

	CreateTerrainNormal();
	CreateTerrainLight();
	mu_memcpy(_PrimaryTerrainLight.data(), _BackTerrainLight.data(), SizeOfArray(_BackTerrainLight));

#ifdef USE_FAST_TERRAIN_COLLISION
	for (mu_uint32 y = 0; y < TERRAIN_SIZE; ++y)
	{
		for (mu_uint32 x = 0; x < TERRAIN_SIZE; ++x)
		{
			mu_uint32 TerrainIndex1 = TERRAIN_INDEX_REPEAT(x, y);
			mu_uint32 TerrainIndex2 = TERRAIN_INDEX_REPEAT(x + 1, y);
			mu_uint32 TerrainIndex3 = TERRAIN_INDEX_REPEAT(x + 1, y + 1);
			mu_uint32 TerrainIndex4 = TERRAIN_INDEX_REPEAT(x, y + 1);

			mu_float xf = x * g_TerrainScale;
			mu_float yf = y * g_TerrainScale;

			auto &TerrainVertex = g_TerrainQuadHeights[y * TERRAIN_SIZE + x];
			TerrainVertex[0] = static_cast<mu_int16>(BackTerrainHeight[TerrainIndex1] * 10.0f);
			TerrainVertex[1] = static_cast<mu_int16>(BackTerrainHeight[TerrainIndex2] * 10.0f);
			TerrainVertex[2] = static_cast<mu_int16>(BackTerrainHeight[TerrainIndex3] * 10.0f);
			TerrainVertex[3] = static_cast<mu_int16>(BackTerrainHeight[TerrainIndex4] * 10.0f);
		}
	}
#endif

	SCENES::GameScene.SettingOutsides();

	if (InitializeGraphics() == false)
	{
		return false;
	}

	return true;
}

void MUTerrain::Unload(mu_boolean ReloadObjects)
{
	if(ReloadObjects == true)
	{
		g_RenderProxyCount = 0;
		g_RenderProxySort = false;
		g_RenderProxyMap.clear();
		g_RenderProxyFlyMap.clear();
	}

	DestroyGraphics();

	_HeightUpdate = false;

	MU_SAFE_DELETE_ARRAY(_TextureData);
	MU_SAFE_DELETE_ARRAY(_GrassTextureData);

	for (mu_uint32 index = 0;index < TERRAIN_SIZE; ++index)
	{
		_GrassInstanceData[index][0] = EMPTY_GRASS;
		_GrassInstanceData[index][1] = 0;
	}

	_ClearColor[0] = 0.0f;
	_ClearColor[1] = 0.0f;
	_ClearColor[2] = 0.0f;
	_ClearColor[3] = 1.0f;

	_WorldIndex = -1;
	_WorldIndex2 = -1;
	_WorldState = -1;

	if (ReloadObjects == true)
	{
		g_Tasking.CleanWorldObjects();
	}
}

const mu_boolean MUTerrain::InitializeGraphics(const mu_boolean processQueue)
{
	mu_string TexturePath = _BasePath + _T("terrain.etex");
	mu_string GrassTexturePath = _BasePath + _T("grassterrain.etex");

	if (_TextureCount > 0)
	{
		_TilesTexture = MU_RESOURCE->CreateTexture2D(TexturePath, EImageViewType::e2DArray);

		if (_TilesTexture == nullptr)
		{
			Unload();
			return false;
		}
	}

	mu_uint32 grassInstanceCount = 0;
	if (_GrassTextureCount > 0)
	{
		_GrassTilesTexture = MU_RESOURCE->CreateTexture2D(GrassTexturePath, EImageViewType::e2DArray);

		if (_GrassTilesTexture == nullptr)
		{
			Unload();
			return false;
		}

		for (mu_uint32 y = 0; y < TERRAIN_SIZE; ++y)
		{
			_GrassInstanceData[y][0] = grassInstanceCount;

			for (mu_uint32 x = 0; x < TERRAIN_SIZE; ++x)
			{
				mu_uint32 index1 = GetTerrainIndexRepeat(x, y);
				mu_uint32 index2 = GetTerrainIndexRepeat(x + 1, y);
				mu_uint32 index3 = GetTerrainIndexRepeat(x + 1, y + 1);
				mu_uint32 index4 = GetTerrainIndexRepeat(x, y + 1);

				if (_GrassTerrainMapping[index1] == -1 ||
					(_TerrainAttribute[index1] & TW_NOGROUND) != 0 ||
					_TerrainMappingAlpha[index1] > 0.0f ||
					_TerrainMappingAlpha[index2] > 0.0f ||
					_TerrainMappingAlpha[index3] > 0.0f ||
					_TerrainMappingAlpha[index4] > 0.0f)
				{
					continue;
				}

				++grassInstanceCount;
			}

			if (grassInstanceCount > _GrassInstanceData[y][0])
				_GrassInstanceData[y][1] = grassInstanceCount - _GrassInstanceData[y][0];
			else
				_GrassInstanceData[y][0] = EMPTY_GRASS;
		}
	}
	_GrassInstanceCount = grassInstanceCount;

	if (_TextureCount > 0)
	{
		std::unique_ptr<SHTerrainInstance[]> terrainData(new SHTerrainInstance[MAX_TERRAIN_SIZE]);

		SHTerrainInstance *instance = terrainData.get();
		for (mu_uint32 y = 0; y < TERRAIN_SIZE; ++y)
		{
			for (mu_uint32 x = 0; x < TERRAIN_SIZE; ++x, ++instance)
			{
				mu_uint32 index = GetTerrainIndex(x, y);

				mu_int8 map1 = _TerrainMapping1[index];
				mu_int8 map2 = _TerrainMapping2[index];

				SetTerrainInstance(instance,
								   MU::MakeARGB(x, y, map1, map2),
								   GetMappingU(map1), GetMappingU(map2));
			}
		}

		if (_TerrainInstanceBuffer.Allocate(sizeof(SHTerrainInstance) * MAX_TERRAIN_SIZE, EBufferUsage::eStatic, EBufferType::eInstance, terrainData.get()) == false)
		{
			return false;
		}

#if OPENGL_BACKEND_ENABLE == 1
		if (EVideoBackend::GetBackend() == EVideoBackendType::eOpenGL)
		{
			EOpenGLBuffer::UpdateTerrainVAO();
		}
#endif
	}

	if (_GrassInstanceCount > 0)
	{
		std::vector<SHGrassInstance> grassTerrainData(_GrassInstanceCount);

		mu_uint32 instanceCount = 0;
		for (mu_uint32 y = 0; y < TERRAIN_SIZE; ++y)
		{
			const mu_float grassUV = (mu_float)((GetLargeRand(4)) / 4.0f);
			for (mu_uint32 x = 0; x < TERRAIN_SIZE; ++x)
			{
				mu_uint32 index1 = GetTerrainIndexRepeat(x, y);
				mu_uint32 index2 = GetTerrainIndexRepeat(x + 1, y);
				mu_uint32 index3 = GetTerrainIndexRepeat(x + 1, y + 1);
				mu_uint32 index4 = GetTerrainIndexRepeat(x, y + 1);

				if (_GrassTerrainMapping[index1] == -1 ||
					(_TerrainAttribute[index1] & TW_NOGROUND) != 0 ||
					_TerrainMappingAlpha[index1] > 0.0f ||
					_TerrainMappingAlpha[index2] > 0.0f ||
					_TerrainMappingAlpha[index3] > 0.0f ||
					_TerrainMappingAlpha[index4] > 0.0f)
				{
					continue;
				}

				mu_float fx = (mu_float)x * g_TerrainScale, fy = (mu_float)y * g_TerrainScale;
				mu_float u1 = (mu_float)x * GRASS_EXPAND + grassUV;
				mu_float grassHeight = _GrassTextureData[_GrassTerrainMapping[index1]].Height;

				SHGrassInstance *instance = &grassTerrainData[instanceCount++];
				SetGrassInstance(instance,
								 EVector2(grassHeight, u1),
								 MU::MakeARGB(x, y, _GrassTerrainMapping[index1], 0));
			}
		}

		if (_GrassInstanceBuffer.Allocate(sizeof(SHGrassInstance) * _GrassInstanceCount, EBufferUsage::eStatic, EBufferType::eInstance, grassTerrainData.data()) == false)
		{
			return false;
		}

#if OPENGL_BACKEND_ENABLE == 1
		if (EVideoBackend::GetBackend() == EVideoBackendType::eOpenGL)
		{
			EOpenGLBuffer::UpdateGrassVAO();
		}
#endif
	}

#ifdef USE_COMPRESSED_TERRAINALPHA_R8
	static mu_uint8 terrainAlpha[MAX_TERRAIN_SIZE];
	for (mu_uint32 n = 0; n < MAX_TERRAIN_SIZE; ++n)
	{
		terrainAlpha[n] = (mu_uint8)(EMath::Clamp(_TerrainMappingAlpha[n], 0.0f, 1.0f) * 255.0f);
	}
	if (_AlphaTexture.Create2D(TERRAIN_SIZE, TERRAIN_SIZE, ETextureFormat::eR8_Unorm, ETextureUsage::eStatic, TERRAIN_ALPHA_SAMPLER, terrainAlpha, sizeof(mu_uint8) * TERRAIN_SIZE) == false)
#else
	if (_AlphaTexture.Create2D(TERRAIN_SIZE, TERRAIN_SIZE, ETextureFormat::eR32_Float, ETextureUsage::eStatic, TERRAIN_ALPHA_SAMPLER, reinterpret_cast<mu_uint8*>(_TerrainMappingAlpha.data()), sizeof(mu_float) * TERRAIN_SIZE) == false)
#endif
	{
		Unload();
		return false;
	}

	if (_HeightTexture.Create2D(TERRAIN_SIZE, TERRAIN_SIZE, ETextureFormat::eR32_Float, ETextureUsage::eStatic, TERRAIN_HEIGHT_SAMPLER, reinterpret_cast<mu_uint8*>(_TerrainHeight.data()), sizeof(mu_float) * TERRAIN_SIZE) == false)
	{
		Unload();
		return false;
	}

	if (_AttributeTexture.Create2D(TERRAIN_SIZE, TERRAIN_SIZE, ETextureFormat::eR16_Unsigned, ETextureUsage::eStatic, TERRAIN_ATTRIBUTE_SAMPLER, reinterpret_cast<mu_uint8*>(_TerrainAttribute.data()), sizeof(mu_uint16) * TERRAIN_SIZE) == false)
	{
		Unload();
		return false;
	}

#if VULKAN_BACKEND_ENABLE == 1
	if (EVideoBackend::GetBackend() == EVideoBackendType::eVulkan)
	{
		if (_TextureCount > 0)
		{
			EVulkanDescriptor::UpdateTerrainTextureDescriptorSet(_TilesTexture);
		}
		EVulkanDescriptor::UpdateTerrainDataDescriptorSet(&_AlphaTexture, &_HeightTexture, &_AttributeTexture);
		if (_GrassInstanceCount > 0)
		{
			EVulkanDescriptor::UpdateGrassTextureDescriptorSet(_GrassTilesTexture);
		}
	}
#endif

	if (processQueue == true)
	{
		g_LoadingManager.ProcessQueue();
		g_LoadingManager.RunLocked(LOADING_FLAGS::LF_MAPS);
	}

	return true;
}

void MUTerrain::DestroyGraphics()
{
	_TerrainInstanceBuffer.Deallocate();
	_GrassInstanceBuffer.Deallocate();

	if (_TilesTexture != nullptr)
	{
		MU_RESOURCE->DeleteTexture(_TilesTexture->_Index);
		_TilesTexture = nullptr;
	}

	if (_GrassTilesTexture != nullptr)
	{
		MU_RESOURCE->DeleteTexture(_GrassTilesTexture->_Index);
		_GrassTilesTexture = nullptr;
	}

	_AlphaTexture.Release();
	_HeightTexture.Release();
	_AttributeTexture.Release();
}

mu_int8 MUTerrain::GetTerrainMapping(mu_float X, mu_float Y)
{
	mu_int32 xi = (mu_int32)X;
	mu_int32 yi = (mu_int32)Y;

	return _TerrainMapping1[GetTerrainIndex(xi, yi)];
}

mu_float MUTerrain::GetMappingU(mu_int8 Index)
{
	if (Index == -1) return 0.0f;
	return _TextureData[Index].UV.x;
}

mu_float MUTerrain::GetGrassMappingU(mu_int8 Index)
{
	if (Index == -1) return 0.0f;
	return _GrassTextureData[Index].UV.x;
}

void MUTerrain::Update(SHGlobalConstantBuffer *setting)
{
	if (_WorldIndex == WD_8TARKAN)
	{
		setting->WaterMove = static_cast<mu_float>(IWorldTime % 40000) * 0.000025f;
	}
	else if (GET_WORLD == WD_42CHANGEUP3RD_2ND)
	{
		setting->WaterMove = static_cast<mu_float>(IWorldTime % 50000) * 0.00002f;
	}
	else
	{
		setting->WaterMove = static_cast<mu_float>(IWorldTime % 20000) * 0.00005f;
	}

	setting->WindScale = 10.0f;
	setting->WindSpeed = static_cast<mu_float>(IWorldTime % (360000 * 2)) * 0.002f;
	setting->WaterIndex = _WaterIndex[0];
}

void MUTerrain::RestoreLight()
{
	mu_memcpy(_PrimaryTerrainLight.data(), _BackTerrainLight.data(), SizeOfArray(_BackTerrainLight));
}

void MUTerrain::RestoreHeight()
{
	if (_HeightUpdate == true)
	{
		_HeightUpdate = false;
		mu_memcpy(_PrimaryTerrainHeight.data(), _TerrainHeight.data(), SizeOfArray(_TerrainHeight));
	}
}

template<const mu_boolean useBaseInstance, const EVideoBackendType backendType>
void MUTerrain::Render()
{
#ifdef USE_RENDERING_DISABLE_FLAGS
	if(g_DisableTerrainRender == true)
		return;
#endif

	if (GET_WORLD == WD_39KANTURU_3RD &&
		g_Direction.m_CKanturu.IsMayaScene() == true)
		return;

	/*
		Terrain Rendering
	*/
	if (_TextureCount > 0)
	{
		ECombinedShader programShader;

		switch (_WorldIndex)
		{
		case WD_7ATLANSE:
		case WD_67DOPPLEGANGER3:
			{
				programShader = ECombinedShader::eTerrainAtlans;
			}
			break;

		case WD_10HEAVEN:
			{
				programShader = ECombinedShader::eTerrainHeaven;
			}
			break;

		case WD_30BATTLECASTLE:
			{
				programShader = ECombinedShader::eTerrainCastle;
			}
			break;

		default:
			{
				programShader = ECombinedShader::eTerrainCommon;
			}
			break;
		}

		if (MU_RESOURCE->GetShadowType() >= EShadowType::ePCF)
		{
			EVideoPipeline::BindGraphicsShadowMap<backendType>(EUniformLayout::eTerrainCommon);
		}

		EVideoPipeline::SetGraphicsPipeline<backendType>(programShader, ERasterizer::eNoCull, EDepthStencil::eZBuffer, EBlendState::eOne_Zero, false);
		EVideoPipeline::BindGraphicsTerrainData<backendType>(EUniformLayout::eTerrainCommon, &_AlphaTexture, &_HeightTexture, &_AttributeTexture);
		EVideoPipeline::BindGraphicsTerrainTexture<backendType>(_TilesTexture, WaterTextureNumber);
		EVideoPipeline::BindGraphicsTerrainBuffers<backendType>(&_TerrainInstanceBuffer);

#if 0 // Render all terrain
		EVideoPipeline::DrawIndexedInstanced<useBaseInstance, backendType>(0, 6, 0, MAX_TERRAIN_SIZE, ETopologyType::eTriangleList, EIndexType::eIndex16, EInputLayout::eTerrainCommon);
#else
		for (mu_int32 y = 0; y < TERRAIN_BLOCK_SIZE; y++)
		{
			mu_uint32 baseIndex = y * TERRAIN_BLOCK_SIZE;

			if (mu_memcmp(&g_CulledBlockVisible[baseIndex], g_FastCullingCheck.data(), sizeof(mu_boolean) * TERRAIN_BLOCK_SIZE) == 0)
			{
				continue;
			}

			mu_int32 minBlockX = 0;
			mu_int32 minX = 0;
			mu_int32 maxX = 0;

			for (mu_int32 x = 0; x < TERRAIN_BLOCK_SIZE; x += 16)
			{
				if (mu_memcmp(&g_CulledBlockVisible[baseIndex + x], g_FastCullingCheck.data(), sizeof(mu_boolean) * 16) != 0)
				{
					minBlockX = x;
					for (mu_int32 n = 0; n < 16; n++)
					{
						if (g_CulledBlockVisible[baseIndex + x + n] != false)
						{
							minX = x + n;
							break;
						}
					}
					break;
				}
			}

			for (mu_int32 x = TERRAIN_BLOCK_SIZE - 16; x >= minBlockX; x -= 16)
			{
				if (mu_memcmp(&g_CulledBlockVisible[baseIndex + x], g_FastCullingCheck.data(), sizeof(mu_boolean) * 16) != 0)
				{
					for (mu_int32 n = 15; n >= 0; n--)
					{
						if (g_CulledBlockVisible[baseIndex + x + n] != false)
						{
							maxX = x + n;
							break;
						}
					}
					break;
				}
			}

			mu_uint32 blockCount = (maxX - minX + 1) * TERRAIN_BLOCK;
			mu_uint32 maxY = y * TERRAIN_BLOCK + TERRAIN_BLOCK;
			for (mu_uint32 baseY = y * TERRAIN_BLOCK; baseY < maxY; baseY++)
			{
				EVideoPipeline::DrawIndexedInstanced<useBaseInstance, backendType>(0, 6, (baseY * TERRAIN_SIZE + minX * TERRAIN_BLOCK), blockCount, ETopologyType::eTriangleList, EIndexType::eIndex16, EInputLayout::eTerrainCommon);
			}
		}
#endif

		EVideoPipeline::UnbindGraphicsTerrainBuffers<backendType>();
	}
}

template<const mu_boolean useBaseInstance, const EVideoBackendType backendType>
void MUTerrain::PostRender()
{
#ifdef USE_RENDERING_DISABLE_FLAGS
	if (g_DisableGrassRender == true)
		return;
#endif

	if (InChaosCastle() == true ||
		battleCastle::InBattleCastle() == true ||
		GET_WORLD == WD_7ATLANSE ||
		IsDoppelGanger3() == true)
		return;

	/*
	Terrain Grass Rendering
	*/
	if (_GrassInstanceCount > 0)
	{
		EBlendState blendState;

		if (IsPKField() == true ||
			IsDoppelGanger2() == true)
		{
			blendState = EBlendState::eOne_One;
		}
		else
		{
			blendState = EBlendState::eAlphaSrc_InvAlphaSrc;
		}

		if (MU_RESOURCE->GetShadowType() >= EShadowType::ePCF)
		{
			EVideoPipeline::BindGraphicsShadowMap<backendType>(EUniformLayout::eGrass);
		}

		EVideoPipeline::SetGraphicsPipeline<backendType>(ECombinedShader::eGrass, ERasterizer::eNoCull, EDepthStencil::eNoDepthMask, blendState, false);
		EVideoPipeline::BindGraphicsTerrainData<backendType>(EUniformLayout::eGrass, &_AlphaTexture, &_HeightTexture, &_AttributeTexture);
		EVideoPipeline::BindGraphicsGrassTexture<backendType>(_GrassTilesTexture);
		EVideoPipeline::BindGraphicsGrassBuffers<backendType>(&_GrassInstanceBuffer);

		for (mu_int32 y = 0; y < TERRAIN_BLOCK_SIZE; y++)
		{
			mu_uint32 baseIndex = y * TERRAIN_BLOCK_SIZE;

			if (mu_memcmp(&g_CulledBlockVisible[baseIndex], g_FastCullingCheck.data(), sizeof(mu_boolean) * TERRAIN_BLOCK_SIZE) == 0)
			{
				continue;
			}

			mu_uint32 maxY = y * TERRAIN_BLOCK + TERRAIN_BLOCK;
			for (mu_uint32 baseY = y * TERRAIN_BLOCK; baseY < maxY; baseY++)
			{
				if (_GrassInstanceData[baseY][0] != EMPTY_GRASS)
				{
					EVideoPipeline::DrawIndexedInstanced<useBaseInstance, backendType>(0, 6, _GrassInstanceData[baseY][0], _GrassInstanceData[baseY][1], ETopologyType::eTriangleList, EIndexType::eIndex16, EInputLayout::eGrass);
				}
			}
		}

		EVideoPipeline::UnbindGraphicsGrassBuffers<backendType>();
	}
}

mu_uint32 MUTerrain::GetTerrainIndex(mu_int32 X, mu_int32 Y)
{
	if (X < 0 || X > TERRAIN_SIZE ||
		Y < 0 || Y > TERRAIN_SIZE)
		return MAX_TERRAIN_SIZE + TERRAIN_SIZE;

	return Y * TERRAIN_SIZE + X;
}

mu_uint32 MUTerrain::GetTerrainIndexRepeat(mu_int32 X, mu_int32 Y)
{
	return (Y & TERRAIN_SIZE_MASK) * TERRAIN_SIZE + (X & TERRAIN_SIZE_MASK);
}

mu_float MUTerrain::GetTerrainHeight(mu_float X, mu_float Y)
{
	if (g_CurrentScene == STARTUP_SCENE || _WorldIndex == -1)
	{
		return 0.0f;
	}

	if (X < 0.0f || Y < 0.0f)
	{
		return 0.0f;
	}

	mu_int32 xi = (mu_int32)X;
	mu_int32 yi = (mu_int32)Y;
	mu_float xd = X - (mu_float)xi;
	mu_float yd = Y - (mu_float)yi;

	mu_uint32 Index = GetTerrainIndex(xi, yi);

	if (Index >= TERRAIN_SIZE*TERRAIN_SIZE)
		return TERRAIN_SPECIAL_HEIGHT;

	if ((_TerrainAttribute[Index] & TW_HEIGHT) == TW_HEIGHT)
	{
		return TERRAIN_SPECIAL_HEIGHT;
	}

	mu_uint32 index1 = GetTerrainIndexRepeat(xi, yi);
	mu_uint32 index2 = GetTerrainIndexRepeat(xi, yi + 1);
	mu_uint32 index3 = GetTerrainIndexRepeat(xi + 1, yi);
	mu_uint32 index4 = GetTerrainIndexRepeat(xi + 1, yi + 1);

	if (index1 >= TERRAIN_SIZE*TERRAIN_SIZE || index2 >= TERRAIN_SIZE*TERRAIN_SIZE ||
		index3 >= TERRAIN_SIZE*TERRAIN_SIZE || index4 >= TERRAIN_SIZE*TERRAIN_SIZE)
		return TERRAIN_SPECIAL_HEIGHT;

	//g_HeightLock.Lock();
	mu_float left = _PrimaryTerrainHeight[index1] + (_PrimaryTerrainHeight[index2] - _PrimaryTerrainHeight[index1]) * yd;
	mu_float right = _PrimaryTerrainHeight[index3] + (_PrimaryTerrainHeight[index4] - _PrimaryTerrainHeight[index3]) * yd;
	//g_HeightLock.Unlock();

	return left + (right - left) * xd;
}

void MUTerrain::AddLight(mu_float X, mu_float Y, EVector3 Light, mu_int32 Radius, std::array<EVector3, MAX_TERRAIN_SIZE> &LightBuffer)
{
	mu_float rf = (mu_float)Radius;

	mu_int32 xi = (mu_int32)X;
	mu_int32 yi = (mu_int32)Y;
	mu_int32 syi = yi - Radius;
	mu_int32 eyi = yi + Radius;
	mu_float syf = (mu_float)(syi);
	for (; syi <= eyi; ++syi, syf += 1.0f)
	{
		mu_int32   sxi = xi - Radius;
		mu_int32   exi = xi + Radius;
		mu_float sxf = (mu_float)(sxi);
		for (; sxi <= exi; ++sxi, sxf += 1.0f)
		{
			mu_float xd = X - sxf;
			mu_float yd = Y - syf;
			mu_float lf = (rf - EMath::Sqrt(xd*xd + yd*yd)) / rf;
			if (lf > 0.0f)
			{
				EVector3 &b = LightBuffer[TERRAIN_INDEX_REPEAT(sxi, syi)];

				b += Light * lf;
				if (b.x < 0.0f) b.x = 0.0f;
				if (b.y < 0.0f) b.y = 0.0f;
				if (b.z < 0.0f) b.z = 0.0f;
			}
		}
	}
}

void MUTerrain::RequestLight(mu_float X, mu_float Y, EVector3 &Light)
{
	mu_int32 x = (mu_int32)X;
	mu_int32 y = (mu_int32)Y;

	if (x < 0 || y < 0 ||
		x > TERRAIN_SIZE_MASK - 1 || y > TERRAIN_SIZE_MASK - 1)
	{
		Light = EVector3(0.0f, 0.0f, 0.0f);
		return;
	}

	mu_int32 Index1 = GetTerrainIndex(x, y);
	mu_int32 Index2 = GetTerrainIndex(x + 1, y);
	mu_int32 Index3 = GetTerrainIndex(x + 1, y + 1);
	mu_int32 Index4 = GetTerrainIndex(x, y + 1);

	mu_float xd = X - (mu_float)x;
	mu_float yd = Y - (mu_float)y;

	//g_LightLock.Lock();
	EVector3 left = _PrimaryTerrainLight[Index1] + (_PrimaryTerrainLight[Index4] - _PrimaryTerrainLight[Index1]) * yd;
	EVector3 right = _PrimaryTerrainLight[Index2] + (_PrimaryTerrainLight[Index3] - _PrimaryTerrainLight[Index2]) * yd;
	//g_LightLock.Unlock();
	Light = (left + (right - left) * xd);
}

void RequestTerrainLight(mu_float xf, mu_float yf, EVector3 &Light)
{
	if (g_CurrentScene == STARTUP_SCENE
		|| g_CurrentScene == WAITING_SCENE)
	{
		Light = EVector3(0.0f, 0.0f, 0.0f);
		return;
	}

	xf = xf * g_TerrainScaleRev;
	yf = yf * g_TerrainScaleRev;

	g_Terrain.RequestLight(xf, yf, Light);
}

void AddTerrainHeight(mu_float xf, mu_float yf, mu_float Height, mu_int32 Range, mu_float *Buffer)
{
	mu_float rf = (mu_float)Range;

	xf = xf * g_TerrainScaleRev;
	yf = yf * g_TerrainScaleRev;
	mu_int32   xi = (mu_int32)xf;
	mu_int32   yi = (mu_int32)yf;
	mu_int32   syi = yi - Range;
	mu_int32   eyi = yi + Range;
	mu_float syf = (mu_float)(syi);
	for (; syi <= eyi; ++syi, syf += 1.0f)
	{
		mu_int32   sxi = xi - Range;
		mu_int32   exi = xi + Range;
		mu_float sxf = (mu_float)(sxi);
		for (; sxi <= exi; ++sxi, sxf += 1.0f)
		{
			mu_float xd = xf - sxf;
			mu_float yd = yf - syf;
			mu_float lf = (rf - EMath::Sqrt(xd*xd + yd*yd)) / rf;
			if (lf > 0.0f)
			{
				Buffer[TERRAIN_INDEX_REPEAT(sxi, syi)] += Height * lf;
			}
		}
	}
}

void CreateTerrainNormal()
{
	for (mu_int32 y = 0; y < TERRAIN_SIZE; ++y)
	{
		for (mu_int32 x = 0; x < TERRAIN_SIZE; ++x)
		{
			mu_int32 Index = TERRAIN_INDEX(x, y);
			EVector3 v1, v2, v3, v4;
			Vector((x)*g_TerrainScale, (y)*g_TerrainScale, BackTerrainHeight[TERRAIN_INDEX_REPEAT(x, y)], v4);
			Vector((x + 1)*g_TerrainScale, (y)*g_TerrainScale, BackTerrainHeight[TERRAIN_INDEX_REPEAT(x + 1, y)], v1);
			Vector((x + 1)*g_TerrainScale, (y + 1)*g_TerrainScale, BackTerrainHeight[TERRAIN_INDEX_REPEAT(x + 1, y + 1)], v2);
			Vector((x)*g_TerrainScale, (y + 1)*g_TerrainScale, BackTerrainHeight[TERRAIN_INDEX_REPEAT(x, y + 1)], v3);
			FaceNormalize(v1, v2, v3, TerrainNormal[Index]);
			g_TerrainCollisionNormal[Index][0] = TerrainNormal[Index];
			FaceNormalize(v1, v3, v4, g_TerrainCollisionNormal[Index][1]);
		}
	}
}

void CreateTerrainNormal_Part(mu_int32 xi, mu_int32 yi)
{
	if (xi > TERRAIN_SIZE - 4) xi = TERRAIN_SIZE - 4;
	else if (xi < 4)         xi = 4;

	if (yi > TERRAIN_SIZE - 4) yi = TERRAIN_SIZE - 4;
	else if (yi < 4)         yi = 4;

	for (mu_int32 y = yi - 4; y < yi + 4; ++y)
	{
		for (mu_int32 x = xi - 4; x < xi + 4; ++x)
		{
			mu_int32 Index = TERRAIN_INDEX(x, y);
			EVector3 v1, v2, v3, v4;
			Vector((x)*g_TerrainScale, (y)*g_TerrainScale, BackTerrainHeight[TERRAIN_INDEX_REPEAT(x, y)], v4);
			Vector((x + 1)*g_TerrainScale, (y)*g_TerrainScale, BackTerrainHeight[TERRAIN_INDEX_REPEAT(x + 1, y)], v1);
			Vector((x + 1)*g_TerrainScale, (y + 1)*g_TerrainScale, BackTerrainHeight[TERRAIN_INDEX_REPEAT(x + 1, y + 1)], v2);
			Vector((x)*g_TerrainScale, (y + 1)*g_TerrainScale, BackTerrainHeight[TERRAIN_INDEX_REPEAT(x, y + 1)], v3);
			FaceNormalize(v1, v2, v3, TerrainNormal[Index]);
			g_TerrainCollisionNormal[Index][0] = TerrainNormal[Index];
			FaceNormalize(v1, v3, v4, g_TerrainCollisionNormal[Index][1]);
		}
	}
}

void CreateTerrainLight()
{
	EVector3 Light;

	if (battleCastle::InBattleCastle())
	{
		Light = EVector3(0.5f, -1.0f, 1.0f);
	}
	else
	{
		Light = EVector3(0.5f, -0.5f, 0.5f);
	}

	for (mu_int32 y = 0; y < TERRAIN_SIZE; ++y)
	{
		for (mu_int32 x = 0; x < TERRAIN_SIZE; ++x)
		{
			mu_int32 Index = TERRAIN_INDEX(x, y);
			mu_float Luminosity = DotProduct(TerrainNormal[Index], Light) + 0.5f;
			if (Luminosity < 0.0f) Luminosity = 0.0f;
			else if (Luminosity > 1.0f) Luminosity = 1.0f;
			BackTerrainLight[Index] = TerrainLight[Index] * Luminosity;
		}
	}
}

void CreateTerrainLight_Part(mu_int32 xi, mu_int32 yi)
{
	if (xi > TERRAIN_SIZE - 4) xi = TERRAIN_SIZE - 4;
	else if (xi < 4)         xi = 4;

	if (yi > TERRAIN_SIZE - 4) yi = TERRAIN_SIZE - 4;
	else if (yi < 4)         yi = 4;

	EVector3 Light;
	Vector(0.5f, -0.5f, 0.5f, Light);
	for (mu_int32 y = yi - 4; y < yi + 4; ++y)
	{
		for (mu_int32 x = xi - 4; x < xi + 4; ++x)
		{
			mu_int32 Index = TERRAIN_INDEX(x, y);
			mu_float Luminosity = DotProduct(TerrainNormal[Index], Light) + 0.5f;
			if (Luminosity < 0.0f) Luminosity = 0.0f;
			else if (Luminosity > 1.0f) Luminosity = 1.0f;
			BackTerrainLight[Index] = TerrainLight[Index] * Luminosity;
		}
	}
}