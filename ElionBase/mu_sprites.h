#ifndef __MU_SPRITES_H__
#define __MU_SPRITES_H__

#pragma once

class MUSprite
{
public:
	mu_uint16 Type;
	mu_uint16 SubType;

	mu_float Scale;
	mu_float Rotation;

	EVector3 Position;
	EVector3 Light;
};

extern MUSprite g_Sprites[MAX_SPRITES];
extern mu_atomic_uint32_t g_SpritesCount;
extern mu_uint32 g_SpritesRenderCount;
extern const mu_uint16 g_SpritesRenderType[4];

mu_uint32 CreateSprite(mu_uint32 Type, EVector3 Position, mu_float Scale, EVector3 Light, OBJECT *Owner, mu_float Rotation = 0.0f, mu_uint32 SubType = 0);

FORCEINLINE const mu_uint16 GetSpriteRenderType(const mu_uint32 Type, const mu_uint32 SubType)
{
	if (Type == BITMAP_FORMATION_MARK) return BLEND::TXB_ALPHATEST_BEGIN;
	return g_SpritesRenderType[SubType > mu_countof(g_SpritesRenderType) ? 0 : SubType];
}

#endif