#include "stdafx.h"
#include "GMDuelArena.h"

CGMDuelArena g_DuelArena;

CGMDuelArena::CGMDuelArena()
{

}

CGMDuelArena::~CGMDuelArena()
{

}

mu_boolean CGMDuelArena::CreateObject(mu_uint32 objectIndex, OBJECT* o)
{
	switch (o->Type)
	{
	case 0: case 1: case 32:
		o->CollisionRange = -300;
		return true;
	}

	return false;
}

mu_boolean CGMDuelArena::MoveObject(OBJECT* o)
{
	switch (o->Type)
	{
	case 35:
	case 36:
		{
			o->HiddenMesh = -2;
		}
		break;
	case 34:
		{
			mu_float Luminosity = (mu_float)(GetLargeRand(3) + 5)*0.1f;
			EVector3 Light;
			Vector(Luminosity*0.9f, Luminosity*0.2f, Luminosity*0.1f, Light);
			AddTerrainLight(o->Position[0], o->Position[1], Light, 3, PrimaryTerrainLight);
			o->HiddenMesh = -2;
		}
		break;
	}

	return false;
}

mu_boolean CGMDuelArena::RenderObjectVisual(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b)
{
	switch (o->Type)
	{
	case 34:
		break;
	case 35:
		{
			EVector3 vLight;
			if (GetLargeRand(3) == 0)
			{
				Vector(1.0f, 1.0f, 1.0f, vLight);
				CreateParticle(ThreadIndex, BITMAP_SMOKE, o->Position, o->Angle, vLight, 14, o->Scale, o);
			}
		}
		break;
	case 36:
		{
			EVector3 vLightFire;
			Vector(1.0f, 0.2f, 0.0f, vLightFire);
			CreateSprite(BITMAP_LIGHT, o->Position, 2.0f * o->Scale, vLightFire, o);

			EVector3 vLight;
			Vector(1.0f, 1.0f, 1.0f, vLight);

			switch (GetLargeRand(3))
			{
			case 0:
				CreateParticle(ThreadIndex, BITMAP_FIRE_HIK1, o->Position, o->Angle, vLight, 0, o->Scale);
				break;
			case 1:
				CreateParticle(ThreadIndex, BITMAP_FIRE_CURSEDLICH, o->Position, o->Angle, vLight, 4, o->Scale);
				break;
			case 2:
				CreateParticle(ThreadIndex, BITMAP_FIRE_HIK3, o->Position, o->Angle, vLight, 0, o->Scale);
				break;
			}
		}
		break;
	}

	return false;
}