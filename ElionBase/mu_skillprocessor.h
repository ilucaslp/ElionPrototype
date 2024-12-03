#ifndef __MU_SKILLPROCESSOR_H__
#define __MU_SKILLPROCESSOR_H__

#pragma once

//#define ENABLE_SKILLPROCESSORS
#ifdef ENABLE_SKILLPROCESSORS
//#define EXPERIMENTAL_MTSKILLPROCESSOR

#define MAX_SKILLPROCESSORS (MAX_CHARACTERS_CLIENT * 300)

class MUSkillProcessor
{
public:
	MUSkillProcessor();

public:
	mu_boolean Live;

	mu_uint8 SkillX;
	mu_uint8 SkillY;

	mu_int16 SkillType;
	mu_int16 SubType;
	mu_int16 MapNumber;
	mu_int16 TargetIndex;

	mu_int32 LifeTime;

	EVector3 Position;
	EVector3 TargetPosition;
	EVector3 Angle;
	EVector3 Direction;
	mu_float Velocity;

	CHARACTER *Owner;
};

extern MUSkillProcessor g_SkillProcessors[MAX_SKILLPROCESSORS];
extern mu_uint16 g_SkillProcessorsFree[MAX_SKILLPROCESSORS];
extern mu_uint16 g_SkillProcessorsCurCount;
#ifdef EXPERIMENTAL_MTSKILLPROCESSOR
extern mu_uint16 g_SkillProcessorsFreeCount;
#else
extern mu_atomic_uint32_t g_SkillProcessorsFreeCount;
#endif

void CreateSkill(CHARACTER *Owner, mu_int16 TargetIndex, mu_uint8 SkillX, mu_uint8 SkillY, mu_int16 SkillType, EVector3 Position, EVector3 Angle, mu_int16 SubType = 0);
void ProcessSkills(const mu_uint32 ThreadIndex, mu_uint32 ThreadCount);
#endif

#endif