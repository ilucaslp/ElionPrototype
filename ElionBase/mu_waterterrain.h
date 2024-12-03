#ifndef __MU_WATERTERRAIN_H__
#define __MU_WATERTERRAIN_H__

class MUWaterTerrain
{
public:
	MUWaterTerrain();
	~MUWaterTerrain();

	mu_boolean Create();
	void Destroy();

	void Update();
	void UpdateBuffer();
	void Render();

	void addSineWave(mu_int32 x, mu_int32 y, mu_int32 radiusX, mu_int32 radiusY, mu_int32 height);
	mu_float GetWaterTerrain(mu_float xf, mu_float yf);
	void RenderWaterAlphaBitmap(mu_int32 Texture, mu_float xf, mu_float yf, mu_float SizeX, mu_float SizeY, EVector3 Light, mu_float Rotation, mu_float Alpha, mu_float Height);

private:
	void calcBaseWave(void);
	void calcWave(void);

	void CreateTerrain(mu_int32 x, mu_int32 y);

	void RenderWaterBitmapTile(mu_float xf, mu_float yf, mu_float lodf, mu_int32 lodi, EVector3 c[4], mu_boolean LightEnable, mu_float Alpha, mu_float Height = 0.0f);

public:
	FORCEINLINE const SHWaterVertex *GetWaterBuffer()
	{
		return m_WaterBuffer;
	}

private:
	SHWaterVertex m_WaterBuffer[MAX_WATER_GRID * MAX_WATER_GRID * 4];

	mu_int32 m_iWaveHeight[4][WATER_TERRAIN_SIZE*WATER_TERRAIN_SIZE];
	mu_int32 m_iWaterPage;

	EVector3 m_Vertices[MAX_WATER_GRID*MAX_WATER_GRID];
	EVector3 m_Normals[MAX_WATER_GRID*MAX_WATER_GRID];
	mu_int32 m_iTriangleList[MAX_WATER_GRID*MAX_WATER_GRID * 4];
	mu_int32 m_iTriangleListNum;

	MUCriticalSectionSpinLock m_Critical;
};

#endif