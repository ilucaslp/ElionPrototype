#include "stdafx.h"
#include "mu_blurs.h"

MUCriticalSectionSpinLock g_BlurLock;
std::array<MUBlur, MAX_BLURS> g_Blur;
std::array<mu_uint16, MAX_BLURS + MAX_TASKING_THREADS> g_BlursQueue;
std::array<mu_uint16, MAX_BLURS> g_BlurFree;
mu_uint16 g_BlurCurCount = 0;
mu_uint16 g_BlurFreeCount = 0;
mu_atomic_uint32_t g_BlurDeleteCount(0);

MUCriticalSectionSpinLock g_ObjectBlurLock;
std::array<MUObjectBlur, MAX_OBJECTBLURS> g_ObjectBlur;
std::array<mu_uint16, MAX_OBJECTBLURS + MAX_TASKING_THREADS> g_ObjectBlursQueue;
std::array<mu_uint16, MAX_OBJECTBLURS> g_ObjectBlurFree;
mu_uint16 g_ObjectBlurCurCount = 0;
mu_uint16 g_ObjectBlurFreeCount = 0;
mu_atomic_uint32_t g_ObjectBlurDeleteCount = 0;

void AddBlur(MUBlur *b, EVector3 &p1, EVector3 &p2, EVector3 &Light, mu_int32 Type)
{
	b->Type = Type;
	VectorCopy(Light, b->Light);

	if (Type == 3)
	{
		b->Texture = BITMAP_BLUR2;
	}
	else if (Type == 4)
	{
		b->Texture = BITMAP_BLUR;
	}
	else if (Type == 5)
	{
		b->Texture = BITMAP_BLUR + 3;
	}
	else
	{
		b->Texture = BITMAP_BLUR + Type;
	}

	if (b->Owner->Level == 0
		&& (Type <= 3 || (Type >= 5 && Type <= 10)))
	{
		b->RenderType = BLEND::TXB_BLEND01;
	}
	else
	{
		b->RenderType = BLEND::TXB_BLENDMINUS;
	}

	if(b->BeginTails == 0)
	{
		b->BeginTails = MAX_BLUR_TAILS - 1;
	}
	else
	{
		--b->BeginTails;
	}

	VectorCopy(p1, b->p1[b->BeginTails]);
	VectorCopy(p2, b->p2[b->BeginTails]);

	if (++b->Number >= MAX_BLUR_TAILS - 1)
	{
		b->Number = MAX_BLUR_TAILS - 1;
	}
}

void CreateBlur(CHARACTER *Owner, EVector3 &p1, EVector3 &p2, EVector3 &Light, mu_int32 Type, mu_boolean Short, mu_int32 SubType)
{
	MUAutoCriticalSpinLock lock(g_BlurLock);

	for (mu_uint32 i = 0; i < g_BlurCurCount; ++i)
	{
		MUBlur *b = &g_Blur[i];

		if (b->Flags.Get(BLUR::eBF_LIVE) == true &&
			b->Owner == Owner &&
			(SubType == 0 || b->SubType == SubType))
		{
			AddBlur(b, p1, p2, Light, Type);
			return;
		}
	}

	mu_uint32 i;

	if (g_BlurFreeCount > 0)
	{
		i = g_BlurFree[--g_BlurFreeCount];
	}
	else if (g_BlurCurCount < MAX_BLURS)
	{
		i = g_BlurCurCount++;
	}
	else
	{
		return;
	}

	MUBlur *b = &g_Blur[i];

	b->Flags.Clear();
	b->Flags.Set(BLUR::eBF_LIVE);

	b->Owner = Owner;
	b->Number = 0;
	b->LifeTime = Short ? 15 : MAX_BLUR_LIFETIME;
	b->SubType = SubType;
	b->BeginTails = MAX_BLUR_TAILS;
	AddBlur(b, p1, p2, Light, Type);
}

void AddObjectBlur(MUObjectBlur *b, EVector3 &p1, EVector3 &p2, EVector3 &Light, mu_int32 Type)
{
	b->Type = Type;
	VectorCopy(Light, b->Light);

	if (Type == 3)
	{
		b->Texture = BITMAP_BLUR2;
	}
	else if (Type == 4)
	{
		b->Texture = BITMAP_BLUR;
	}
	else if (Type == 5)
	{
		b->Texture = BITMAP_LAVA;
	}
	else
	{
		b->Texture = BITMAP_BLUR + Type;
	}

	b->RenderType = BLEND::TXB_BLEND01;

	if (b->BeginTails == 0)
	{
		b->BeginTails = MAX_OBJECTBLUR_TAILS - 1;
	}
	else
	{
		--b->BeginTails;
	}

	VectorCopy(p1, b->p1[b->BeginTails]);
	VectorCopy(p2, b->p2[b->BeginTails]);

	if (++b->Number >= MAX_OBJECTBLUR_TAILS - 1)
	{
		b->Number = MAX_OBJECTBLUR_TAILS - 1;
	}
}

void CreateObjectBlur(void *Owner, EVector3 &p1, EVector3 &p2, EVector3 &Light, mu_int32 Type, mu_boolean Short, mu_int32 SubType, mu_int32 iLimitLifeTime)
{
	MUAutoCriticalSpinLock lock(g_ObjectBlurLock);

	for (mu_uint32 i = 0; i < g_ObjectBlurCurCount; ++i)
	{
		MUObjectBlur *b = &g_ObjectBlur[i];

		if (b->Flags.Get(BLUR::eBF_LIVE) == true &&
			b->Owner == Owner &&
			(SubType == 0 || b->SubType == SubType))
		{
			AddObjectBlur(b, p1, p2, Light, Type);
			return;
		}
	}

	mu_uint32 i;

	if (g_ObjectBlurFreeCount > 0)
	{
		i = g_ObjectBlurFree[--g_ObjectBlurFreeCount];
	}
	else if (g_ObjectBlurCurCount < MAX_OBJECTBLURS)
	{
		i = g_ObjectBlurCurCount++;
	}
	else
	{
		return;
	}

	MUObjectBlur *b = &g_ObjectBlur[i];

	b->Flags.Clear();
	b->Flags.Set(BLUR::eBF_LIVE);
	b->Owner = Owner;
	b->Number = 0;
	if (iLimitLifeTime > -1)
	{
		b->LifeTime = iLimitLifeTime;
		b->_iLimitLifeTime = iLimitLifeTime;
	}
	else
	{
		b->_iLimitLifeTime = Short ? 15 : MAX_OBJECTBLUR_LIFETIME;
		b->LifeTime = b->_iLimitLifeTime;
	}
	b->SubType = SubType;
	b->BeginTails = MAX_OBJECTBLUR_TAILS;
	AddObjectBlur(b, p1, p2, Light, Type);
}

void RemoveObjectBlurs(void *Owner, mu_int32 SubType)
{
	MUAutoCriticalSpinLock lock(g_ObjectBlurLock);

	if (g_ObjectBlurCurCount == 0)
	{
		return;
	}

	mu_uint16 j = -1, i = 0;

	for (i = 0; i < g_ObjectBlurCurCount; ++i)
	{
		MUObjectBlur *b = &g_ObjectBlur[i];

		if (b->Flags.Get(BLUR::eBF_LIVE) == true &&
			b->Owner == Owner)
		{
			j = i;
			break;
		}
	}

	if (j == (mu_uint16)-1) return;

	MUObjectBlur *b = &g_ObjectBlur[j];

	b->Flags.Clear(BLUR::eBF_LIVE);
	b->Flags.Set(BLUR::eBF_FREE);

	g_ObjectBlurFree[g_ObjectBlurFreeCount++] = j;
}