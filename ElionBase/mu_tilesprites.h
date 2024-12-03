#ifndef __MU_TILE_SPRITES_H__
#define __MU_TILE_SPRITES_H__

#pragma once

class MUTileSprite
{
public:
	mu_boolean EnableLight;

	mu_uint16 Type;
	mu_uint16 RenderType;

	mu_float PositionX;
	mu_float PositionY;
	mu_float SizeX;
	mu_float SizeY;
	mu_float Rotation;
	mu_float Height;

	mu_uint32 Light;
};

extern MUTileSprite g_TileSprites[MAX_TILESPRITES];
extern mu_atomic_uint32_t g_TileSpritesCount;
extern mu_uint32 g_TileSpritesRenderCount;

void RenderTerrainAlphaBitmap(mu_uint32 Type, mu_uint32 RenderType, mu_float PositionX, mu_float PositionY, mu_float SizeX, mu_float SizeY, EVector3 &Light, mu_float Rotation = 0.0f, mu_float Alpha = 1.0f, mu_float Height = 5.0f);

#endif