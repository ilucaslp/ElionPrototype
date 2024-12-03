#ifndef __MU_BLURS_H__
#define __MU_BLURS_H__

#pragma once

namespace BLUR
{
	enum BLUR_FLAGS
	{
		eBF_FREE = (1 << 0),
		eBF_LIVE = (1 << 1),
	};
};

class MUBlur
{
public:
	MUFlagset<mu_uint8> Flags;

	mu_int16	BeginTails;

	mu_int32	Type;
	mu_int32	SubType;
	mu_int32	LifeTime;
	mu_int32	Number;

	mu_uint16	Texture;
	mu_uint16	RenderType;
	mu_int32	RenderCount;

	EVector3    Light;
	std::array<EVector3, MAX_BLUR_TAILS + 1> p1;
	std::array<EVector3, MAX_BLUR_TAILS + 1> p2;

	CHARACTER	*Owner;
};

class MUObjectBlur
{
public:
	MUFlagset<mu_uint8> Flags;

	mu_int16	BeginTails;

	mu_int32	Type;
	mu_int32	SubType;
	mu_int32	LifeTime;
	mu_int32	_iLimitLifeTime;
	mu_int32	Number;

	mu_uint16	Texture;
	mu_uint16	RenderType;

	EVector3    Light;
	std::array<EVector3, MAX_OBJECTBLUR_TAILS + 1> p1;
	std::array<EVector3, MAX_OBJECTBLUR_TAILS + 1> p2;

	void *Owner;
};

extern std::array<MUBlur, MAX_BLURS> g_Blur;
extern std::array<mu_uint16, MAX_BLURS + MAX_TASKING_THREADS> g_BlursQueue;
extern std::array<mu_uint16, MAX_BLURS> g_BlurFree;
extern mu_uint16 g_BlurCurCount;
extern mu_uint16 g_BlurFreeCount;
extern mu_atomic_uint32_t g_BlurDeleteCount;

extern std::array<MUObjectBlur, MAX_OBJECTBLURS> g_ObjectBlur;
extern std::array<mu_uint16, MAX_OBJECTBLURS + MAX_TASKING_THREADS> g_ObjectBlursQueue;
extern std::array<mu_uint16, MAX_OBJECTBLURS> g_ObjectBlurFree;
extern mu_uint16 g_ObjectBlurCurCount;
extern mu_uint16 g_ObjectBlurFreeCount;
extern mu_atomic_uint32_t g_ObjectBlurDeleteCount;

void CreateBlur(CHARACTER *Owner, EVector3 &p1, EVector3 &p2, EVector3 &Light, mu_int32 Type, mu_boolean Short = false, mu_int32 SubType = 0);
void CreateObjectBlur(void *Owner, EVector3 &p1, EVector3 &p2, EVector3 &Light, mu_int32 Type, mu_boolean Short = false, mu_int32 SubType = 0, mu_int32 iLimitLifeTime = -1);

void RemoveObjectBlurs(void *Owner, mu_int32 SubType = 0);

#endif