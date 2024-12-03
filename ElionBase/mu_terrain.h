#ifndef __MU_TERRAIN_H__
#define __MU_TERRAIN_H__

#pragma once

namespace Terrain
{
	/*
	Structure:
	World File(ewld):
		[Header Signature] (4 bytes) (uint)
		[Header Version] (2 bytes) (ushort)
		[CRC32] (4 bytes) (uint)
		[Encrypted Data] (aligned 8 bytes)
			[Compressed Size] (4 bytes) (uint)
			[Decompressed Size] (4 bytes) (uint)
			[Compressed Data]
			(Begin)
				[Clear Color] (4 bytes * 4] (mu_float[4] - RGBA)
				[Textures Count] (4 bytes) (uint)
				[Textures Information] (based on version)[TexturesCount] (TextureDataX)
				(Begin)
					[Index] (4 bytes) (mu_float)
					[UV] (8 bytes) (mu_float[2]) (Used to know how much should be expanded)
				(End)
				[Grass Textures Count] (4 bytes) (uint)
				[Grass Textures Information] (based on version)[GrassTexturesCount] (GrassTextureDataX)
				(Begin)
					[Index] (4 bytes) (mu_float)
					[UV] (8 bytes) (mu_float[2]) (Used to know how much should be expanded)
					[Height] (4 bytes) (mu_float) (Used to know how much should be moved by z-position)
				(End)
				[Height Array] (4 bytes * 256 * 256) (mu_float[256][256])
				[Light Array] (12 bytes * 256 * 256) (mu_float[256][256][3]) (Red, Green, Blue)
				[Mapping 1 Array] (1 bytes * 256 * 256) (int8[256][256])
				[Mapping 2 Array] (1 bytes * 256 * 256) (int8[256][256])
				[Grass Mapping Array] (1 bytes * 256 * 256) (int8[256][256])
				[Mapping Alpha Array] (4 bytes * 256 * 256) (mu_float[256][256])
				[Attributes Array] (2 bytes * 256 * 256) (uint16[256][256])
			(End)

	World Objects File(eobj):
		[Header Signature] (4 bytes) (uint)
		[Header Version] (2 bytes) (ushort)
		[CRC32] (4 bytes) (uint)
		[Encrypted Data] (aligned 8 bytes)
			[Compressed Size] (4 bytes) (uint)
			[Decompressed Size] (4 bytes) (uint)
			[Compressed Data]
			(Begin)
				[Models Count] (4 bytes) (uint)
				[Models Array] (based on version)[ModelsCount] (ModelDataX)
				(Begin)
					[Filename] (256 bytes) (unicode[128])
				(End)
				[Objects Count] (4 bytes) (uint)
				[Objects Array] (based on version)[ObjectsCount] (ObjectDataX)
				(Begin)
					[Index] (4 bytes) (uint)
					[Matrix] (64 bytes) (matrix)
				(End)
			(End)
	*/

#pragma pack(push,1)
	struct ARM_PACKED TextureData1
	{
		EVector2 UV;
	};

	struct ARM_PACKED GrassTextureData1
	{
		EVector2 UV;
		mu_float Height;
	};

	struct ARM_PACKED ObjectData1
	{
		mu_int32 Type;
		EVector3 Position;
		EVector3 Angle;
		EFloat Scale;
	};
#pragma pack(pop)
};

class MUTerrain
{
private:
	static const mu_uint8 CryptKey[];
	static MUBlowfish Crypt;

public:
	MUTerrain();
	~MUTerrain();

	const mu_boolean Load(mu_int32 WorldIndex, mu_int32 WorldState, mu_boolean ReloadObjects = true);
	void Unload(mu_boolean ReloadObjects = true);

	const mu_boolean InitializeGraphics(const mu_boolean processQueue = true);
	void DestroyGraphics();

	mu_int8 GetTerrainMapping(mu_float X, mu_float Y);

	mu_float GetMappingU(mu_int8 Index);
	mu_float GetGrassMappingU(mu_int8 Index);

	void Update(SHGlobalConstantBuffer *setting);
	void RestoreLight();
	void RestoreHeight();
	template<const mu_boolean useBaseInstance, const EVideoBackendType backendType>
	void Render();
	template<const mu_boolean useBaseInstance, const EVideoBackendType backendType>
	void PostRender();

	mu_int32 GetWorld()
	{
		return _WorldIndex;
	}

	static mu_uint32 GetTerrainIndex(mu_int32 X, mu_int32 Y);
	static mu_uint32 GetTerrainIndexRepeat(mu_int32 X, mu_int32 Y);
	mu_float GetTerrainHeight(mu_float X, mu_float Y);
	void AddLight(mu_float X, mu_float Y, EVector3 Light, mu_int32 Radius, std::array<EVector3, MAX_TERRAIN_SIZE> &LightBuffer);
	void RequestLight(mu_float X, mu_float Y, EVector3 &Light);

public:
	enum : mu_uint32
	{
		MAX_GRASS = 3,
		EMPTY_GRASS = 0xFFFFFFFF,
		TERRAIN_RSIZE = TERRAIN_SIZE - 1,
		TERRAIN_BLOCK = 4,
		TERRAIN_BLOCK_SIZE = 64,
	};
	static const mu_float GRASS_EXPAND;
	static const mu_float HEIGHT_MULTIPLY;
	static const mu_float TERRAIN_SPECIAL_HEIGHT;

public:
	FORCEINLINE const EBufferBackend *GetTerrainInstanceBuffer()
	{
		return &_TerrainInstanceBuffer;
	}

	FORCEINLINE const EBufferBackend *GetGrassInstanceBuffer()
	{
		return &_GrassInstanceBuffer;
	}

private:
	friend class MUCPUCulling;
	friend class MUTasking;
	friend class MUGameScene;

	mu_string _BasePath;

	mu_int32 _WorldIndex;
	mu_int32 _WorldIndex2;
	mu_int32 _WorldState;

	mu_boolean _HeightUpdate;

	mu_uint32 _TextureCount;
	mu_uint32 _GrassTextureCount;
	Terrain::TextureData1 *_TextureData;
	Terrain::GrassTextureData1 *_GrassTextureData;

	EBufferBackend _TerrainInstanceBuffer;
	EBufferBackend _GrassInstanceBuffer;

	MUTexture *_TilesTexture;
	MUTexture *_GrassTilesTexture;
	MUTexture _AlphaTexture;
	MUTexture _HeightTexture;
	MUTexture _AttributeTexture;

public:
	MUCriticalSectionSpinLock _Lock;

	mu_uint32 _GrassInstanceCount;

	std::array<mu_float, 4> _ClearColor;
	std::array<mu_int8, 4> _WaterIndex;

	std::array<mu_int8, MAX_TERRAIN_SIZE> _TerrainMapping1;
	std::array<mu_int8, MAX_TERRAIN_SIZE> _TerrainMapping2;
	std::array<mu_int8, MAX_TERRAIN_SIZE> _GrassTerrainMapping;

	std::array<mu_uint16, MAX_TERRAIN_SIZE> _TerrainAttribute;

	std::array<mu_float, MAX_TERRAIN_SIZE> _TerrainHeight;
	std::array<mu_float, MAX_TERRAIN_SIZE> _PrimaryTerrainHeight;
	std::array<EVector3, MAX_TERRAIN_SIZE> _TerrainLight;
	std::array<EVector3, MAX_TERRAIN_SIZE> _BackTerrainLight;
	std::array<EVector3, MAX_TERRAIN_SIZE> _PrimaryTerrainLight;
	std::array<EVector3, MAX_TERRAIN_SIZE> _TerrainNormal;
	std::array<mu_float, MAX_TERRAIN_SIZE> _TerrainMappingAlpha;

	std::array<std::array<mu_uint32, 2>, TERRAIN_SIZE> _GrassInstanceData;
};

extern MUTerrain g_Terrain;
#ifdef USE_FAST_TERRAIN_COLLISION
extern MU_ALIGN(16, std::array<std::array<mu_int16, 4>, MAX_TERRAIN_SIZE>) g_TerrainQuadHeights;
#endif
extern MU_ALIGN(16, std::array<std::array<EVector3, 2>, MAX_TERRAIN_SIZE>) g_TerrainCollisionNormal;
extern MU_ALIGN(16, std::array<mu_uint16, MAX_TERRAIN_SIZE + MAX_TASKING_THREADS>) g_TerrainCollisionQueue;

#define GET_WORLD (g_Terrain.GetWorld())

#define CHAOS_NUM 6
#define HELLAS_NUM 7

enum ENUM_WORLD
{
	WD_0LORENCIA = 0,
	WD_1DUNGEON,
	WD_2DEVIAS,
	WD_3NORIA,
	WD_4LOSTTOWER,
	WD_5UNKNOWN,
	WD_6STADIUM,
	WD_7ATLANSE,
	WD_8TARKAN,
	WD_9DEVILSQUARE,
	WD_10HEAVEN,
	WD_11BLOODCASTLE1,
	WD_11BLOODCASTLE_END = WD_11BLOODCASTLE1 + 6,
	WD_18CHAOS_CASTLE,
	WD_18CHAOS_CASTLE_END = WD_18CHAOS_CASTLE + (CHAOS_NUM - 1),
	WD_24HELLAS,
	WD_24HELLAS_END = WD_24HELLAS + (HELLAS_NUM - 2),
	WD_30BATTLECASTLE,
	WD_31HUNTING_GROUND = 31,
	WD_33AIDA = 33,
	WD_34CRYWOLF_1ST = 34,
	WD_24HELLAS_7 = 36,
	WD_37KANTURU_1ST = 37,
	WD_38KANTURU_2ND = 38,
	WD_39KANTURU_3RD = 39,
	WD_40AREA_FOR_GM = 40,
	WD_41CHANGEUP3RD_1ST = 41,
	WD_42CHANGEUP3RD_2ND = 42,
	WD_45CURSEDTEMPLE_LV1 = 45,
	WD_45CURSEDTEMPLE_LV2,
	WD_45CURSEDTEMPLE_LV3,
	WD_45CURSEDTEMPLE_LV4,
	WD_45CURSEDTEMPLE_LV5,
	WD_45CURSEDTEMPLE_LV6,
	WD_51HOME_6TH_CHAR = 51,
	WD_52BLOODCASTLE_MASTER_LEVEL = 52,
	WD_53CAOSCASTLE_MASTER_LEVEL = 53,
	WD_56MAP_SWAMP_OF_QUIET = 56,
	WD_57ICECITY = 57,
	WD_58ICECITY_BOSS = 58,
	WD_62SANTA_TOWN = 62,
	WD_63PK_FIELD = 63,
	WD_64DUELARENA = 64,
	WD_65DOPPLEGANGER1 = 65,
	WD_66DOPPLEGANGER2 = 66,
	WD_67DOPPLEGANGER3 = 67,
	WD_68DOPPLEGANGER4 = 68,
	WD_69EMPIREGUARDIAN1 = 69,
	WD_70EMPIREGUARDIAN2 = 70,
	WD_71EMPIREGUARDIAN3 = 71,
	WD_72EMPIREGUARDIAN4 = 72,
	WD_73NEW_LOGIN_SCENE = 73,
	WD_74NEW_CHARACTER_SCENE = 74,
	WD_79UNITEDMARKETPLACE = 79,
	NUM_WD,
};

extern mu_boolean g_LightEnabled;
extern MUCriticalSectionSpinLock g_LightLock;
extern MUCriticalSectionSpinLock g_HeightLock;
extern MUCriticalSectionSpinLock g_AttributeLock;

#define TerrainMappingLayer1 (g_Terrain._TerrainMapping1)
#define TerrainMappingLayer2 (g_Terrain._TerrainMapping2)
#define TerrainWall (g_Terrain._TerrainAttribute)
#define TerrainLight (g_Terrain._TerrainLight)
#define BackTerrainLight (g_Terrain._BackTerrainLight)
#define PrimaryTerrainLight (g_Terrain._PrimaryTerrainLight)
#define BackTerrainHeight (g_Terrain._PrimaryTerrainHeight)
#define TerrainNormal (g_Terrain._TerrainNormal)
#define TERRAIN_INDEX MUTerrain::GetTerrainIndex
#define TERRAIN_INDEX_REPEAT MUTerrain::GetTerrainIndexRepeat

EINLINE mu_uint16 TERRAIN_ATTRIBUTE(mu_float x, mu_float y)
{
	mu_int32 xi = (mu_int32)(x * g_TerrainScaleRev);
	mu_int32 yi = (mu_int32)(y * g_TerrainScaleRev);
	return TerrainWall[TERRAIN_INDEX(xi, yi)];
}

EINLINE void AddTerrainAttribute(mu_int32 index, mu_uint16 att)
{
	g_AttributeLock.Lock();
	TerrainWall[index] |= att;
	g_AttributeLock.Unlock();
}

EINLINE void AddTerrainAttribute(mu_int32 x, mu_int32 y, mu_uint16 att)
{
	mu_int32 iIndex = TERRAIN_INDEX(x, y);

	TerrainWall[iIndex] |= att;
}

EINLINE void SubTerrainAttribute(mu_int32 x, mu_int32 y, mu_uint16 att)
{
	mu_int32 iIndex = TERRAIN_INDEX(x, y);

	TerrainWall[iIndex] &= ~att;
}

EINLINE void AddTerrainAttributeRange(mu_int32 x, mu_int32 y, mu_int32 dx, mu_int32 dy, mu_uint16 att, mu_boolean Add)
{
	g_AttributeLock.Lock();
	for (mu_int32 j = 0; j < dy; ++j)
	{
		for (mu_int32 i = 0; i < dx; ++i)
		{
			if (Add)
			{
				AddTerrainAttribute(x + i, y + j, att);
			}
			else
			{
				SubTerrainAttribute(x + i, y + j, att);
			}
		}
	}
	g_AttributeLock.Unlock();
}

EINLINE mu_float RequestTerrainHeight(mu_float xf, mu_float yf)
{
	return g_Terrain.GetTerrainHeight(xf * g_TerrainScaleRev, yf * g_TerrainScaleRev);
}

EINLINE void AddTerrainLight(mu_float xf, mu_float yf, EVector3 Light, mu_int32 Range, std::array<EVector3, MAX_TERRAIN_SIZE> &Buffer)
{
	if (g_LightEnabled == false)
	{
		return;
	}

	g_LightLock.Lock();
	g_Terrain.AddLight(xf * g_TerrainScaleRev, yf * g_TerrainScaleRev, Light, Range, Buffer);
	g_LightLock.Unlock();
}

extern mu_uint32 g_CurrentScene;

EINLINE mu_boolean IsGmArea()
{
	return (GET_WORLD == WD_40AREA_FOR_GM);
}

struct TerrainModificationRequest
{
	mu_boolean CreateNormal;
	mu_boolean CreateLight;
	mu_float PositionX;
	mu_float PositionY;
	mu_int32 CollisionX;
	mu_int32 CollisionY;
	mu_float Height;
	mu_int32 Range;
	mu_float *Buffer;
};

#define MAX_TERRAIN_MODIFICATION 5000
extern TerrainModificationRequest g_TerrainModification[MAX_TERRAIN_MODIFICATION];
extern mu_atomic_uint32_t g_TerrainModificationCount;

void RequestTerrainLight(mu_float xf, mu_float yf, EVector3 &Light);
void AddTerrainHeight(mu_float xf, mu_float yf, mu_float Height, mu_int32 Range, mu_float *Buffer);
void CreateTerrainNormal();
void CreateTerrainNormal_Part(mu_int32 xi, mu_int32 yi);
void CreateTerrainLight();
void CreateTerrainLight_Part(mu_int32 xi, mu_int32 yi);

#endif