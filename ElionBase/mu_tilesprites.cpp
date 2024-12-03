#include "stdafx.h"
#include "mu_tilesprites.h"

MUTileSprite g_TileSprites[MAX_TILESPRITES];
mu_atomic_uint32_t g_TileSpritesCount(0);
mu_uint32 g_TileSpritesRenderCount;

void RenderTerrainAlphaBitmap(mu_uint32 Type, mu_uint32 RenderType, mu_float PositionX, mu_float PositionY, mu_float SizeX, mu_float SizeY, EVector3 &Light, mu_float Rotation, mu_float Alpha, mu_float Height)
{
	mu_uint32 i = g_TileSpritesCount.fetch_add(1);

	if (i >= MAX_TILESPRITES)
	{
		return;
	}

	MUTileSprite *o = &g_TileSprites[i];

	o->Type = Type;
	o->RenderType = RenderType;
	o->PositionX = PositionX;
	o->PositionY = PositionY;
	o->SizeX = SizeX;
	o->SizeY = SizeY;
	o->Rotation = Rotation;
	o->Height = Height;
	o->Light = MU::Float4ToARGB(Light, Alpha);

	MUThreadQueues::PerTileSpritesRenderCount[i] = RenderTerrainBitmapCalculate(o->SizeX, o->SizeY);
}