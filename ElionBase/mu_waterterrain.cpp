#include "stdafx.h"
#include "mu_waterterrain.h"

MUWaterTerrain::MUWaterTerrain() : m_iWaterPage(0), m_iTriangleListNum(0)
{
	mu_zeromem(m_WaterBuffer, sizeof(m_WaterBuffer));
	mu_zeromem(m_iWaveHeight, sizeof(m_iWaveHeight));
}

MUWaterTerrain::~MUWaterTerrain()
{
	Destroy();
}

mu_boolean MUWaterTerrain::Create()
{
	return true;
}

void MUWaterTerrain::Destroy()
{
}

void MUWaterTerrain::Update()
{
	mu_int32 WaveX;
	mu_int32 WaveY;

	if ((MoveSceneFrame % 40) == 0)
	{
		WaveX = ((Hero->PositionX) * 2) + (GetLargeRand(30)) - 15;
		WaveY = ((Hero->PositionY) * 2) + 25;
		addSineWave(WaveX, WaveY, 20, 2, 2000);
	}

	m_iWaterPage ^= 1;
	calcWave();
	calcBaseWave();
}

void MUWaterTerrain::UpdateBuffer()
{
	CreateTerrain((Hero->PositionX) * 2, (Hero->PositionY) * 2);

	for (mu_int32 j = 0; j < m_iTriangleListNum; ++j)
	{
		mu_int32 offset = m_iTriangleList[j];

		SetWaterTerrainVertex(&m_WaterBuffer[j], m_Vertices[offset], m_Normals[offset]);
	}
}

void MUWaterTerrain::Render()
{
	EVideoPipeline::BindGraphicsWaterVertexBuffer();
	EVideoPipeline::BindGraphicsWaterTexture();

	EVideoPipeline::SetGraphicsPipelineAtRuntime(ECombinedShader::eWater1, ERasterizer::eFrontOnly, EDepthStencil::eNoDepthMask, EBlendState::eOne_One, false);
	EVideoPipeline::DrawIndexedAtRuntime(0, (m_iTriangleListNum / 4) * 6, ETopologyType::eTriangleList, EIndexType::eIndex16, EInputLayout::eWater);
	EVideoPipeline::SetGraphicsPipelineAtRuntime(ECombinedShader::eWater2, ERasterizer::eFrontOnly, EDepthStencil::eNoDepthMask, EBlendState::eOne_One, false);
	EVideoPipeline::DrawIndexedAtRuntime(0, (m_iTriangleListNum / 4) * 6, ETopologyType::eTriangleList, EIndexType::eIndex16, EInputLayout::eWater);

	EVideoPipeline::UnbindVAO();
}

void MUWaterTerrain::CreateTerrain(mu_int32 x, mu_int32 y)
{
	mu_float fHeight, fHeight1;
	mu_int32 offset;
	mu_int32 grid = MAX_WATER_GRID / 2;

	m_iTriangleListNum = 0;
	for (mu_int32 offY = 0, i = y - grid + 6; offY < MAX_WATER_GRID; ++i, ++offY)
	{
		for (mu_int32 offX = 0, j = x - grid - 4; offX < MAX_WATER_GRID; ++j, ++offX)
		{
			if (i < 0 || j < 0) fHeight = 350.0f;
			else if (i >= WATER_TERRAIN_SIZE || j >= WATER_TERRAIN_SIZE) fHeight = 350.0f;
			else
			{
				offset = j + (i*WATER_TERRAIN_SIZE);

				fHeight = m_iWaveHeight[m_iWaterPage][offset] + 350.0f;

				fHeight1 = m_iWaveHeight[2][offset] + 350.0f;
				fHeight1 += m_iWaveHeight[3][offset] + 350.0f;

				fHeight = (fHeight + fHeight1 * 0.5f) * 0.5f;
			}

			offset = offX + (offY*MAX_WATER_GRID);
			Vector((mu_float)j*WAVE_SCALE, (mu_float)i*WAVE_SCALE, fHeight, m_Vertices[offset]);
			Vector(0.0f, 0.0f, 0.0f, m_Normals[offset]);
			if (offX >= MAX_WATER_GRID - 1 || offY >= MAX_WATER_GRID - 1)
			{
				VectorCopy(m_Normals[offset - 1], m_Normals[offset]);
				continue;
			}

			if (((offX % 2) == 0 && (offY % 2) == 0) || ((offX % 2) == 1 && (offY % 2) == 1))
			{
				m_iTriangleList[m_iTriangleListNum + 0] = offset;
				m_iTriangleList[m_iTriangleListNum + 1] = offset + 1;
				m_iTriangleList[m_iTriangleListNum + 2] = offset + 1 + MAX_WATER_GRID;
				m_iTriangleList[m_iTriangleListNum + 3] = offset + MAX_WATER_GRID;
			}
			else
			{
				m_iTriangleList[m_iTriangleListNum + 0] = offset + 1;
				m_iTriangleList[m_iTriangleListNum + 1] = offset + 1 + MAX_WATER_GRID;
				m_iTriangleList[m_iTriangleListNum + 2] = offset + MAX_WATER_GRID;
				m_iTriangleList[m_iTriangleListNum + 3] = offset;
			}

			m_iTriangleListNum += 4;
		}
	}

	mu_int32 v1, v2, v3;
	EVector3 normalV;
	static mu_int32 NormalNum[MAX_WATER_GRID*MAX_WATER_GRID];
	mu_zeromem(NormalNum, sizeof(NormalNum));
	for (mu_int32 j = 0; j < m_iTriangleListNum; j += 4)
	{
		v1 = m_iTriangleList[j + 0];
		v2 = m_iTriangleList[j + 1];
		v3 = m_iTriangleList[j + 2];

		FaceNormalize(m_Vertices[v1], m_Vertices[v2], m_Vertices[v3], normalV);

		VectorAdd(normalV, m_Normals[v1], m_Normals[v1]);
		VectorAdd(normalV, m_Normals[v2], m_Normals[v2]);
		VectorAdd(normalV, m_Normals[v3], m_Normals[v3]);

		++NormalNum[v1];
		++NormalNum[v2];
		++NormalNum[v3];

		v1 = m_iTriangleList[j + 0];
		v2 = m_iTriangleList[j + 2];
		v3 = m_iTriangleList[j + 3];

		FaceNormalize(m_Vertices[v1], m_Vertices[v2], m_Vertices[v3], normalV);

		VectorAdd(normalV, m_Normals[v1], m_Normals[v1]);
		VectorAdd(normalV, m_Normals[v2], m_Normals[v2]);
		VectorAdd(normalV, m_Normals[v3], m_Normals[v3]);

		++NormalNum[v1];
		++NormalNum[v2];
		++NormalNum[v3];
	}

	for (mu_int32 i = 0; i < MAX_WATER_GRID*MAX_WATER_GRID; ++i)
	{
		m_Normals[i][0] /= (mu_float)(NormalNum[i]);
		m_Normals[i][1] /= (mu_float)(NormalNum[i]);
		m_Normals[i][2] /= (mu_float)(NormalNum[i]);

		VectorNormalize(m_Normals[i]);
	}
}

void MUWaterTerrain::addSineWave(mu_int32 x, mu_int32 y, mu_int32 radiusX, mu_int32 radiusY, mu_int32 height)
{
	mu_int32* p = &m_iWaveHeight[m_iWaterPage][0];

	mu_int32 cx, cy;
	mu_int32 left, top, right, bottom;
	mu_int32 square;
	mu_int32 radsquare;
	mu_float length = (1024.0f / (mu_float)radiusX)*(1024.0f / (mu_float)radiusX);

	if (x < 0) x = 1 + radiusX + GetLargeRand(WATER_TERRAIN_SIZE - 2 * radiusX - 1);
	if (y < 0) y = 1 + radiusY + GetLargeRand(WATER_TERRAIN_SIZE - 2 * radiusY - 1);

	radsquare = (radiusX*radiusY);

	left = -radiusX; right = radiusX;
	top = -radiusY; bottom = radiusY;

	if ((x - radiusX) < 1) left -= (x - radiusX - 1);
	if ((y - radiusY) < 1) top -= (y - radiusY - 1);
	if ((x + radiusX) > WATER_TERRAIN_SIZE - 1) right -= (x + radiusX - WATER_TERRAIN_SIZE + 1);
	if ((y + radiusY) > WATER_TERRAIN_SIZE - 1) bottom -= (y + radiusY - WATER_TERRAIN_SIZE + 1);

	m_Critical.Lock();
	for (cy = top; cy < bottom; ++cy)
	{
		for (cx = left; cx < right; ++cx)
		{
			square = cy*cy + cx*cx;
			if (square < radsquare)
			{
				mu_float dist = EMath::Sqrt(square*length);
				mu_int32   sine = (mu_int32)((EMath::Cos(dist) + 0xffff)*height) >> 19;
				p[WATER_TERRAIN_SIZE*(cy + y) + cx + x] += sine;
			}
		}
	}
	m_Critical.Unlock();
}

void MUWaterTerrain::calcBaseWave()
{
	mu_int32 MaxHeight;
	mu_int32 offset;
	mu_int32 HeroX = (Hero->PositionX) * 2;
	mu_int32 HeroY = (Hero->PositionY) * 2;

	mu_int32 StartX = EMath::Max(0, HeroX - (VIEW_WATER_GRID / 2));
	mu_int32 StartY = EMath::Max(0, HeroY - (VIEW_WATER_GRID / 2));
	mu_int32 EndX = EMath::Min(WATER_TERRAIN_SIZE, HeroX + (VIEW_WATER_GRID / 2));
	mu_int32 EndY = EMath::Min(WATER_TERRAIN_SIZE, HeroY + (VIEW_WATER_GRID / 2));
	for (mu_int32 i = StartY; i < EndY; ++i)        //  y
	{
		for (mu_int32 j = StartX; j < EndX; ++j)    //  x
		{
			offset = j + (i*WATER_TERRAIN_SIZE);

			mu_float alpha = 0.0f;

			MaxHeight = (mu_int32)(EMath::Sin((FWorldTime*0.005f) + (i*0.1f) + (j*0.1f)) * 50 * (1 + alpha));
			m_iWaveHeight[2][offset] = (mu_int32)(MaxHeight - EMath::Sin((FWorldTime*0.003f) + (j*0.1f) + (i*0.5f)) * 50 * (1 + alpha));

			MaxHeight = (mu_int32)(EMath::Sin((FWorldTime*0.001f) + (i*0.5f) + (j*0.5f)) * 25 * (1 + alpha));
			m_iWaveHeight[3][offset] = (mu_int32)(MaxHeight - EMath::Sin((FWorldTime*0.002f) + (j*1.0f) + (i*0.3f)) * 25 * (1 + alpha));
		}
	}
}

void MUWaterTerrain::calcWave()
{
	mu_int32 newh;

	mu_int32* newptr = &m_iWaveHeight[m_iWaterPage][0];
	mu_int32* oldptr = &m_iWaveHeight[m_iWaterPage ^ 1][0];

	mu_int32 x;
	mu_int32 y = (WATER_TERRAIN_SIZE - 1)*WATER_TERRAIN_SIZE;
	for (mu_int32 count = WATER_TERRAIN_SIZE + 1; count < y; count += 2)
	{
		for (x = count + WATER_TERRAIN_SIZE - 2; count < x; ++count)
		{
			newh = ((oldptr[count + WATER_TERRAIN_SIZE]
				+ oldptr[count - WATER_TERRAIN_SIZE]
				+ oldptr[count + 1]
				+ oldptr[count - 1]
				) >> 1)
				- newptr[count];
			newptr[count] = newh - (newh >> 4);
		}
	}
}

mu_float MUWaterTerrain::GetWaterTerrain(mu_float xf, mu_float yf)
{
	mu_int32 x = (mu_int32)(xf * g_TerrainScaleRev * 2);
	mu_int32 y = (mu_int32)(yf * g_TerrainScaleRev * 2);

	m_Critical.Lock();
	mu_float fHeight;
	if (m_iWaterPage)
	{
		fHeight = m_iWaveHeight[1][x + (y*WATER_TERRAIN_SIZE)] + 350.0f;
	}
	else
	{
		fHeight = m_iWaveHeight[0][x + (y*WATER_TERRAIN_SIZE)] + 350.0f;
	}

	mu_float fHeight1 = m_iWaveHeight[2][x + (y*WATER_TERRAIN_SIZE)] + 350.0f;
	fHeight1 += m_iWaveHeight[3][x + (y*WATER_TERRAIN_SIZE)] + 350.0f;
	fHeight = (fHeight + fHeight1 * 0.5f) / 4.0f;
	m_Critical.Unlock();

	return fHeight;
}

#if 0 // FIX
void MUWaterTerrain::RenderWaterAlphaBitmap(mu_int32 Texture, mu_float xf, mu_float yf, mu_float SizeX, mu_float SizeY, EVector3 Light, mu_float Rotation, mu_float Alpha, mu_float Height)
{
	if (Alpha == 1.0f)
		glColor3fv(Light);
	else
		glColor4f(Light[0], Light[1], Light[2], Alpha);

	EVector3 Angle;
	Vector(0.0f, 0.0f, Rotation, Angle);
	mu_float Matrix[3][4];
	AngleMatrix(Angle, Matrix);

	BindTexture(Texture);
	mu_float mxf = (xf * TerrainScaleRev * 2);
	mu_float myf = (yf * TerrainScaleRev * 2);
	mu_int32   mxi = (mu_int32)(mxf);
	mu_int32   myi = (mu_int32)(myf);

	mu_float Size;
	if (SizeX >= SizeY)
		Size = SizeX;
	else
		Size = SizeY;
	mu_float TexU = (((mu_float)mxi - mxf) + 0.5f*Size);
	mu_float TexV = (((mu_float)myi - myf) + 0.5f*Size);
	mu_float TexScaleU = 1.0f / Size;
	mu_float TexScaleV = 1.0f / Size;
	Size = (mu_float)((mu_int32)Size + 1);
	mu_float Aspect = SizeX / SizeY;
	for (mu_float y = -Size; y <= Size; y += 1.0f)
	{
		for (mu_float x = -Size; x <= Size; x += 1.0f)
		{
			EVector3 p1[4], p2[4];
			Vector((TexU + x)*TexScaleU, (TexV + y)*TexScaleV, 0.0f, p1[0]);
			Vector((TexU + x + 1.0f)*TexScaleU, (TexV + y)*TexScaleV, 0.0f, p1[1]);
			Vector((TexU + x + 1.0f)*TexScaleU, (TexV + y + 1.0f)*TexScaleV, 0.0f, p1[2]);
			Vector((TexU + x)*TexScaleU, (TexV + y + 1.0f)*TexScaleV, 0.0f, p1[3]);
			//mu_boolean Clip = false;
			for (mu_int32 i = 0; i < 4; ++i)
			{
				p1[i][0] -= 0.5f;
				p1[i][1] -= 0.5f;
				VectorRotate(p1[i], Matrix, p2[i]);
				p2[i][0] *= Aspect;
				p2[i][0] += 0.5f;
				p2[i][1] += 0.5f;
				//if((p2[i][0]>=0.0f && p2[i][0]<=1.0f) || (p2[i][1]>=0.0f && p2[i][1]<=1.0f)) Clip = true;
			}
			//if(Clip==true)
			RenderWaterBitmapTile((mu_float)mxi + x, (mu_float)myi + y, 1.0f, 1, p2, false, Alpha, Height);
		}
	}
}

void MUWaterTerrain::RenderWaterBitmapTile(mu_float xf, mu_float yf, mu_float lodf, mu_int32 lodi, EVector3 c[4], mu_boolean LightEnable, mu_float Alpha, mu_float Height)
{
	EVector3 TerrainVertex[4];
	mu_int32 xi = (mu_int32)xf;
	mu_int32 yi = (mu_int32)yf;
	if (xi < 0 || yi < 0 || xi >= TERRAIN_SIZE_MASK || yi >= TERRAIN_SIZE_MASK) return;
	mu_float TileScale = WAVE_SCALE;
	mu_float sx = xf*WAVE_SCALE;
	mu_float sy = yf*WAVE_SCALE;
	mu_int32 TerrainIndex1 = xi + (yi*WATER_TERRAIN_SIZE);
	mu_int32 TerrainIndex2 = xi + lodi + (yi*WATER_TERRAIN_SIZE);
	mu_int32 TerrainIndex3 = xi + lodi + ((yi + lodi)*WATER_TERRAIN_SIZE);
	mu_int32 TerrainIndex4 = xi + ((yi + lodi)*WATER_TERRAIN_SIZE);
	Vector(sx, sy, m_iWaveHeight[0][TerrainIndex1] + 400.0f + Height, TerrainVertex[0]);
	Vector(sx + TileScale, sy, m_iWaveHeight[0][TerrainIndex2] + 400.0f + Height, TerrainVertex[1]);
	Vector(sx + TileScale, sy + TileScale, m_iWaveHeight[0][TerrainIndex3] + 400.0f + Height, TerrainVertex[2]);
	Vector(sx, sy + TileScale, m_iWaveHeight[0][TerrainIndex4] + 400.0f + Height, TerrainVertex[3]);

	EVector3 Light[4];
	if (LightEnable)
	{
		VectorCopy(PrimaryTerrainLight[TerrainIndex1], Light[0]);
		VectorCopy(PrimaryTerrainLight[TerrainIndex2], Light[1]);
		VectorCopy(PrimaryTerrainLight[TerrainIndex3], Light[2]);
		VectorCopy(PrimaryTerrainLight[TerrainIndex4], Light[3]);
	}

	glBegin(GL_TRIANGLE_FAN);
	for (mu_int32 i = 0; i < 4; ++i)
	{
		if (LightEnable)
		{
			if (Alpha == 1.0f)
				glColor3fv(Light[i]);
			else
				glColor4f(Light[i][0], Light[i][1], Light[i][2], Alpha);
		}
		glTexCoord2f(c[i][0], c[i][1]);
		glVertex3fv(TerrainVertex[i]);
	}
	glEnd();
}
#endif