#include "stdafx.h"
#include "mu_tasking.h"

void MUTasking::MovePointers(const mu_uint32 ThreadIndex)
{
#ifdef DIVIDE_TASKING
	mu_uint32 beginIndex, endIndex;
	GetIndexTasking(ThreadIndex, MAX_POINTERS, beginIndex, endIndex);

	for (mu_uint32 i = beginIndex; i < endIndex; ++i)
#else
	for (mu_uint32 i = ThreadIndex; i < MAX_POINTERS; i += _ThreadsCount)
#endif
	{
		MUPointer *o = &g_Pointers[i];

		if (o->Flags.Get(POINTER::ePF_LIVE))
		{
			--o->LifeTime;

			switch (o->Type)
			{
			case BITMAP_BLOOD:
			case BITMAP_BLOOD + 1:
				if (o->Type == BITMAP_BLOOD)
				{
					o->Scale += 0.004f;
				}

				EVector3 Light;
				RequestTerrainLight(o->Position[0], o->Position[1], Light);
				if (o->LifeTime < 50)
				{
					o->Light = MU::Float4ToARGB(Light, o->LifeTime*0.02f);
				}
				else
				{
					o->Light = MU::Float4ToARGB(Light, 1.0f);
				}

				if (o->LifeTime <= 0)
				{
					o->Flags.Clear(POINTER::ePF_LIVE);
				}
				break;
			}

			if (o->Flags.Get(POINTER::ePF_LIVE) == false)
			{
				g_PointersDeleteCount.fetch_add(1);
			}
		}
	}
}

void MUTasking::CheckPointers(const mu_uint32 ThreadIndex)
{
	mu_uint32 beginIndex, endIndex;
	GetIndexTasking(ThreadIndex, MAX_POINTERS, beginIndex, endIndex);

	for (mu_uint32 i = beginIndex; i < endIndex; ++i)
	{
		const MUPointer *o = &g_Pointers[i];

		if (o->Flags.Get(POINTER::ePF_LIVE) == false)
		{
			continue;
		}

		EEffectCulledData &instanceData = g_EffectsCulledData[g_EffectsCulledDataCount.fetch_add(1)];
		instanceData.Type = ECastEnum(EEffectProxyEnum::ePointers);
		instanceData.Index = i;
	}
}