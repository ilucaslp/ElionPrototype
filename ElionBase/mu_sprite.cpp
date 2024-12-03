#include "stdafx.h"
#include "mu_sprites.h"

MUSprite g_Sprites[MAX_SPRITES];
mu_atomic_uint32_t g_SpritesCount(0);
mu_uint32 g_SpritesRenderCount;
const mu_uint16 g_SpritesRenderType[4] =
{
	BLEND::TXB_BLEND01,
	BLEND::TXB_BLENDMINUS,
	BLEND::TXB_ALPHATEST_BEGIN,
	BLEND::TXB_BLEND02,
};

mu_uint32 CreateSprite(mu_uint32 Type, EVector3 Position, mu_float Scale, EVector3 Light, OBJECT *Owner, mu_float Rotation, mu_uint32 SubType)
{
	mu_uint32 i = g_SpritesCount.fetch_add(1);

	if (i > MAX_SPRITES - 1)
	{
		return INVALID_PARTICLE;
	}

	MUSprite *o = &g_Sprites[i];

	o->Type = Type;
	o->SubType = SubType;
	o->Scale = Scale;
	o->Rotation = Rotation;
	o->Position = Position;
	o->Light = Light;

#if 0
	if (o->Type == BITMAP_FORMATION_MARK)
	{
		o->RenderType = BLEND::TXB_ALPHATEST_BEGIN;
	}
	else
	{
		switch (o->SubType)
		{
		default:
		case 0:
			o->RenderType = BLEND::TXB_BLEND01;
			break;

		case 1:
			o->RenderType = BLEND::TXB_BLENDMINUS;
			break;

		case 2:
			o->RenderType = BLEND::TXB_ALPHATEST_BEGIN;
			break;

		case 3:
			o->RenderType = BLEND::TXB_BLEND02;
			break;
		}
	}
#endif

	return i;
}