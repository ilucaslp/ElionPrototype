#include "stdafx.h"
#include "mu_tasking.h"

void MUTasking::MoveBugs(const mu_uint32 ThreadIndex)
{
#ifdef DIVIDE_TASKING
	mu_uint32 beginIndex, endIndex;
	GetIndexTasking(ThreadIndex, MAX_BUTTERFLES, beginIndex, endIndex);

	for (mu_uint32 i = beginIndex; i < endIndex; ++i)
#else
	for (mu_uint32 i = ThreadIndex; i < MAX_BUTTERFLES; i += _ThreadsCount)
#endif
	{
		OBJECT *o = &g_Butterfles[i];
		if (o->Flags.Get(OBJECT::eOF_LIVE) == true)
		{
			MoveBug(ThreadIndex, o);
		}
	}
}

void MUTasking::CheckBugs(const mu_uint32 ThreadIndex)
{
	mu_uint32 queueCount = 0;
	const mu_uint32 queueIndex = MUThreadQueues::ButterflesQueueIndex[ThreadIndex];

	mu_uint32 beginIndex, endIndex;
	GetIndexTasking(ThreadIndex, MAX_BUTTERFLES, beginIndex, endIndex);

	for (mu_uint32 i = beginIndex; i < endIndex; ++i)
	{
		OBJECT *o = &g_Butterfles[i];

		if (o->Flags.Get(OBJECT::eOF_LIVE) == true)
		{
			const mu_boolean isVisible = TestFrustrum2D(o->Position, 100.0f);
			if (isVisible)
			{
				o->Flags.Set(OBJECT::eOF_VISIBLE);
			}
			else
			{
				o->Flags.Clear(OBJECT::eOF_VISIBLE);
			}

			if (isVisible)
			{
				if (o->Owner->Type != MODEL_PLAYER &&
					o->Type != MODEL_HELPER)
					continue;

				if (o->Type == MODEL_DARK_HORSE &&
					!(o->Owner->CurrentAction == PLAYER_STOP_RIDE_HORSE ||
						o->Owner->CurrentAction == PLAYER_RUN_RIDE_HORSE ||
						o->Owner->CurrentAction == PLAYER_ATTACK_RIDE_HORSE_SWORD ||
						o->Owner->CurrentAction == PLAYER_ATTACK_DARKHORSE))
					continue;

				MUThreadQueues::ButterflesQueue[queueIndex + queueCount++] = i;
			}
		}
	}

	MUThreadQueues::ButterflesQueueCount[ThreadIndex] = queueCount;
}

void MUTasking::PrepareBugs(const mu_uint32 ThreadIndex)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	mu_uint32 q = ThreadIndex;
	for (mu_uint32 t = 0; t < _ThreadsCount; ++t)
	{
		const mu_uint32 queueCount = MUThreadQueues::ButterflesQueueCount[t];
		const mu_uint16 *queue = MUThreadQueues::ButterflesQueue.data() + MUThreadQueues::ButterflesQueueIndex[t];

		for (; q < queueCount; q += _ThreadsCount)
		{
			const mu_uint32 butterfleIndex = queue[q];
			OBJECT *o = &g_Butterfles[butterfleIndex];

			if (g_CurrentScene == CHARACTER_SCENE)
				o->Scale = 1.2f;
			else if (o->Type != MODEL_FENRIR_BLACK &&
				o->Type != MODEL_FENRIR_BLUE &&
				o->Type != MODEL_FENRIR_RED &&
				o->Type != MODEL_FENRIR_GOLD)
				o->Scale = 1.0f;

			mu_int32 State = 0;
			if (g_isCharacterBuff(o->Owner, eBuff_Cloaking))
			{
				State = 10;
			}

			ThreadInfo.RenderIndex = 0;
			ThreadInfo.CulledType = CULLED_BUGS;
			ThreadInfo.ObjectIndex = butterfleIndex;

			RenderObject(ThreadIndex, o, false, 0, State);

			if (_UpdateCounter > 0)
			{
				EVector3 Light;
				mu_float Luminosity = (mu_float)(GetLargeRand(30) + 70)*0.01f;
				switch (o->Type)
				{
				case MODEL_HELPER:
					Vector(Luminosity*0.5f, Luminosity*0.8f, Luminosity*0.6f, Light);
					CreateSprite(BITMAP_LIGHT, o->Position, 1.0f, Light, o);
					break;
				}
			}
		}

		q -= queueCount;
	}
}