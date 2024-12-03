#include "stdafx.h"
#include "mu_tasking.h"

void MUTasking::MoveBlurs(const mu_uint32 ThreadIndex)
{
#ifdef DIVIDE_TASKING
	mu_uint32 beginIndex, endIndex;
	GetIndexTasking(ThreadIndex, MAX_BLURS, beginIndex, endIndex);
	for (mu_uint32 i = beginIndex; i < endIndex; ++i)
#else
	for (mu_uint32 i = ThreadIndex; i < MAX_BLURS; i += _ThreadsCount)
#endif
	{
		MUBlur *b = &g_Blur[i];

		if (b->Flags.Get(BLUR::eBF_LIVE) == true)
		{
			--b->LifeTime;
			--b->Number;

			const mu_uint32 c = b->BeginTails;
			if(c == 0)
			{
				b->BeginTails = MAX_BLUR_TAILS - 1;
			}
			else
			{
				--b->BeginTails;
			}

			VectorCopy(b->p1[c], b->p1[b->BeginTails]);
			VectorCopy(b->p2[c], b->p2[b->BeginTails]);

			if (b->LifeTime <= 0)
			{
				if (--b->Number <= 0)
				{
					b->Flags.Clear(BLUR::eBF_LIVE);

					++g_BlurDeleteCount;
				}
			}
		}
	}

#ifdef DIVIDE_TASKING
	GetIndexTasking(ThreadIndex, MAX_OBJECTBLURS, beginIndex, endIndex);
#ifndef NDEBUG
	if (endIndex > MAX_OBJECTBLURS)
		DEBUG_BREAK();
#endif
	for (mu_uint32 i = beginIndex; i < endIndex; ++i)
#else
	for (mu_uint32 i = ThreadIndex; i < MAX_OBJECTBLURS; i += _ThreadsCount)
#endif
	{
		MUObjectBlur *b = &g_ObjectBlur[i];

		if (b->Flags.Get(BLUR::eBF_LIVE) == true)
		{
			--b->LifeTime;
			--b->Number;

			if (b->LifeTime <= 0)
			{
				b->Number = 0;
				b->Flags.Clear(BLUR::eBF_LIVE);
				++g_ObjectBlurDeleteCount;
				continue;
			}

			const mu_uint32 c = b->BeginTails;
			if (c == 0)
			{
				b->BeginTails = MAX_OBJECTBLUR_TAILS - 1;
			}
			else
			{
				--b->BeginTails;
			}

			VectorCopy(b->p1[c], b->p1[b->BeginTails]);
			VectorCopy(b->p2[c], b->p2[b->BeginTails]);
		}
	}
}

void MUTasking::CheckBlurs(const mu_uint32 ThreadIndex)
{
	mu_uint32 beginIndex, endIndex;

	// Blurs
	{
		const mu_uint32 queueIndex = MUThreadQueues::BlursQueueIndex[ThreadIndex];
		GetIndexTasking(ThreadIndex, MAX_BLURS, beginIndex, endIndex);

		for (mu_uint32 i = beginIndex; i < endIndex; ++i)
		{
			const MUBlur *b = &g_Blur[i];

			if (b->Flags.Get(BLUR::eBF_LIVE) == false)
			{
				continue;
			}

			if (b->Number < 2)
			{
				continue;
			}

			EEffectCulledData &instanceData = g_EffectsCulledData[g_EffectsCulledDataCount.fetch_add(1)];
			instanceData.Type = ECastEnum(EEffectProxyEnum::eBlurs);
			instanceData.Index = i;
		}
	}

	// Object Blurs
	{
		GetIndexTasking(ThreadIndex, MAX_OBJECTBLURS, beginIndex, endIndex);

		for (mu_uint32 i = beginIndex; i < endIndex; ++i)
		{
			const MUObjectBlur *b = &g_ObjectBlur[i];

			if (b->Flags.Get(BLUR::eBF_LIVE) == false)
			{
				continue;
			}

			if (b->Number < 2)
			{
				continue;
			}

			mu_int16 RenderCount = b->Number - 1;

			if (b->SubType == 113 || b->SubType == 114)
			{
				for (mu_uint32 j = 0, l = b->BeginTails; j < b->Number - 1; ++j, l = (l + 1) % MAX_OBJECTBLUR_TAILS)
				{
					static const mu_float Data = 300.0f;

					mu_uint32 l2 = (l + 1) % MAX_OBJECTBLUR_TAILS;

					if (EMath::Abs(b->p1[l][0] - b->p1[l2][0]) > Data ||
						EMath::Abs(b->p1[l][1] - b->p1[l2][1]) > Data ||
						EMath::Abs(b->p1[l][2] - b->p1[l2][2]) > Data ||
						EMath::Abs(b->p1[l][0] - b->p2[l2][0]) > Data ||
						EMath::Abs(b->p1[l][1] - b->p2[l2][1]) > Data ||
						EMath::Abs(b->p2[l][2] - b->p2[l2][2]) > Data)
					{
						--RenderCount;
					}
				}
			}

			if (RenderCount <= 0)
			{
				continue;
			}

			MUThreadQueues::PerObjectBlursRenderCount[i] = RenderCount;

			EEffectCulledData &instanceData = g_EffectsCulledData[g_EffectsCulledDataCount.fetch_add(1)];
			instanceData.Type = ECastEnum(EEffectProxyEnum::eObjectBlurs);
			instanceData.Index = i;
		}
	}
}