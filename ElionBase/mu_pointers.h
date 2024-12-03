#ifndef __MU_POINTERS_H__
#define __MU_POINTERS_H__

#pragma once

namespace POINTER
{
	enum
	{
		ePF_FREE = (1 << 0),
		ePF_LIVE = (1 << 1),
	};
};

class MUPointer
{
public:
	MUFlagset<mu_uint8> Flags;

	mu_uint16 Type;
	mu_uint16 RenderType;
	
	mu_int32 LifeTime;
	mu_uint32 Light;
	
	mu_float Scale;
	mu_float Rotation;

	EVector3 Position;
};

extern MUCriticalSectionSpinLock g_PointersLock;
extern std::array<MUPointer, MAX_POINTERS> g_Pointers;
extern std::array<mu_uint16, MAX_POINTERS + MAX_TASKING_THREADS> g_PointersQueue;
extern std::array<mu_uint16, MAX_POINTERS> g_PointersFree;
extern mu_uint16 g_PointersCurCount;
extern mu_uint16 g_PointersFreeCount;
extern mu_atomic_int32_t g_PointersDeleteCount;
extern mu_boolean g_PointersLocked;

void CreatePointer(mu_uint32 Type, EVector3 &Position, mu_float Angle, EVector3 &Light, mu_float Scale = 1.0f);

#endif