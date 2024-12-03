#include "stdafx.h"
#include "mu_pointers.h"

MUCriticalSectionSpinLock g_PointersLock;
std::array<MUPointer, MAX_POINTERS> g_Pointers;
std::array<mu_uint16, MAX_POINTERS + MAX_TASKING_THREADS> g_PointersQueue;
std::array<mu_uint16, MAX_POINTERS> g_PointersFree;
mu_uint16 g_PointersCurCount = 0;
mu_uint16 g_PointersFreeCount = 0;
mu_atomic_int32_t g_PointersDeleteCount(0);

void CreatePointer(mu_uint32 Type, EVector3 &Position, mu_float Angle, EVector3 &Light, mu_float Scale)
{
	mu_uint32 i = 0;

	g_PointersLock.Lock();

	if (g_PointersFreeCount > 0)
	{
		i = g_PointersFree[--g_PointersFreeCount];
	}
	else if (g_PointersCurCount < MAX_PARTICLES)
	{
		i = g_PointersCurCount++;
	}
	else
	{
		g_PointersLock.Unlock();
		return;
	}

	MUPointer *o = &g_Pointers[i];

	o->Flags.Clear();
	o->Flags.Set(POINTER::ePF_LIVE);

	o->Type = Type;

	g_PointersLock.Unlock();

	VectorCopy(Position, o->Position);
	o->Light = MU::Float4ToARGB(Light, 1.0f);
	o->Rotation = Angle;
	o->Scale = Scale;

	MUTexture *texture = &TEXTURES::Data[o->Type];

	if (texture->TextureInternal.Components == 3)
	{
		o->RenderType = BLEND::TXB_BLEND01;
	}
	else
	{
		o->RenderType = BLEND::TXB_ALPHATEST_NOMASK;
	}

	switch (Type)
	{
	case BITMAP_BLOOD:
	case BITMAP_BLOOD + 1:
		o->LifeTime = 50 + GetLargeRand(32);
		break;
	}
}