#ifndef __MU_POINTS_H__
#define __MU_POINTS_H__

#pragma once

class MUPoint
{
public:
	enum
	{
		ePF_FREE = (1 << 0),
		ePF_LIVE = (1 << 1),
		ePF_MOVE = (1 << 2),
	};
	MUFlagset<mu_uint8> Flags;

	mu_int32 Type;

	mu_float Gravity;
	mu_float Scale;

	EVector3 Position;
	EVector3 Color;

	mu_string Text;
};

extern MUCriticalSectionSpinLock g_PointsLock;
extern MUPoint g_Points[MAX_POINTS];
extern mu_uint16 g_PointsFree[MAX_POINTS];
extern mu_uint16 g_PointsCurCount;
extern mu_uint16 g_PointsFreeCount;
extern mu_atomic_int32_t g_PointsDeleteCount;

void CreatePoint(EVector3 &Position, mu_int32 Value, EVector3 &Color, mu_float Scale = 15.0f, mu_boolean Move = true);

#endif