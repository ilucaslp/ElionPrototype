#include "stdafx.h"
#include "mu_points.h"

MUCriticalSectionSpinLock g_PointsLock;
MUPoint g_Points[MAX_POINTS];
mu_uint16 g_PointsFree[MAX_POINTS];
mu_uint16 g_PointsCurCount = 0;
mu_uint16 g_PointsFreeCount = 0;
mu_atomic_int32_t g_PointsDeleteCount(0);

void CreatePoint(EVector3 &Position, mu_int32 Value, EVector3 &Color, mu_float Scale, mu_boolean Move)
{
	mu_uint32 i = 0;

	g_PointsLock.Lock();

	if (g_PointsFreeCount > 0)
	{
		i = g_PointsFree[--g_PointsFreeCount];
	}
	else if (g_PointsCurCount < MAX_POINTS)
	{
		i = g_PointsCurCount++;
	}
	else
	{
		g_PointsLock.Unlock();
		return;
	}

	MUPoint *o = &g_Points[i];

	o->Flags.Clear();
	o->Flags.Set(MUPoint::ePF_LIVE | (MUPoint::ePF_MOVE * (mu_uint32)Move));

	o->Type = Value;

	g_PointsLock.Unlock();

	VectorCopy(Position, o->Position);
	o->Position[2] += 140.0f;
	VectorCopy(Color, o->Color);
	o->Gravity = 10.0f;
	o->Scale = Scale;

	switch (Value)
	{
	default:
		{
			o->Text = boost::str(boost::wformat(_T("%1%")) % Value);
		}
		break;

	case -1: o->Text = _T("Miss"); break;
	case -2: o->Text = _T("Hit"); break;
	}
}