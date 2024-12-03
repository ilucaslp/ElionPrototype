#include "stdafx.h"
#include "mu_skillprocessor.h"

#ifdef ENABLE_SKILLPROCESSORS
MUSkillProcessor g_SkillProcessors[MAX_SKILLPROCESSORS];
mu_uint16 g_SkillProcessorsFree[MAX_SKILLPROCESSORS];
mu_uint16 g_SkillProcessorsCurCount = 0;
#ifdef EXPERIMENTAL_MTSKILLPROCESSOR
mu_uint16 g_SkillProcessorsFreeCount = 0;
#else
mu_atomic_uint32_t g_SkillProcessorsFreeCount(0);
#endif
mu_boolean g_SkillProcessorsSort = false;

MUSkillProcessor::MUSkillProcessor()
{
	Live = false;
	Owner = nullptr;
}

void CreateSkill(CHARACTER *Owner, mu_int16 TargetIndex, mu_uint8 SkillX, mu_uint8 SkillY, mu_int16 SkillType, EVector3 Position, EVector3 Angle, mu_int16 SubType)
{
	mu_int32 i = -1;

	if (g_SkillProcessorsFreeCount > 0)
	{
		i = g_SkillProcessorsFree[--g_SkillProcessorsFreeCount];
		g_SkillProcessorsSort = true;
	}
	else if (g_SkillProcessorsCurCount < MAX_SKILLPROCESSORS)
	{
		i = g_SkillProcessorsCurCount++;
	}

	MUSkillProcessor *o = &g_SkillProcessors[i];

	o->Live = true;
	o->LifeTime = 1;
	o->Owner = Owner;
	o->SkillType = SkillType;
	o->SubType = SubType;
	o->Position = Position;
	o->TargetPosition = EVector3(0.0f, 0.0f, 0.0f);
	o->Angle = Angle;
	o->Direction = EVector3(0.0f, 0.0f, 0.0f);
	o->TargetIndex = TargetIndex;
	o->SkillX = SkillX;
	o->SkillY = SkillY;

	OBJECT *OwnerObject = &Owner->Object;

	switch (SkillType)
	{
	case AT_SKILL_ALICE_LIGHTNINGORB:
	case AT_SKILL_ALICE_DRAINLIFE:
		{
			mu_float PlaySpeed = GetPlaySpeed(OwnerObject, OwnerObject->Type, OwnerObject->CurrentAction);
			mu_float FrameCount = 5.0f;

			o->LifeTime = (mu_int32)EMath::Ceil(FrameCount / PlaySpeed);
		}
		break;
	}
}

void ProcessSkills(const mu_uint32 ThreadIndex, mu_uint32 ThreadCount)
{
	MUThreadInfo *ThreadInfo = &g_ThreadInfo[ThreadIndex];

	for (mu_uint32 n = ThreadIndex; n < MAX_SKILLPROCESSORS; n += ThreadCount)
	{
		MUSkillProcessor *o = &g_SkillProcessors[n];

		if (o->Live == true)
		{
			CHARACTER *Owner = o->Owner;
			OBJECT *OwnerObject = &Owner->Object;

			if (OwnerObject->Live == false)
			{
				goto DeleteSkill;
			}

			switch (o->SkillType)
			{
			case AT_SKILL_ALICE_LIGHTNINGORB:
				{
					if (o->LifeTime == 1)
					{
						CHARACTER *tc = &CharactersClient[Owner->TargetCharacter];
						OBJECT *to = &tc->Object;

						EVector3 vLight;
						Vector(1.0f, 1.0f, 1.0f, vLight);

						CreateEffect(ThreadIndex, MODEL_LIGHTNING_ORB, OwnerObject->Position, OwnerObject->Angle, vLight, 0, to);

						PlayBackground(SOUND_SUMMON_SKILL_LIGHTORB);
					}
				}
				break;

			case AT_SKILL_ALICE_DRAINLIFE:
				{
					if (o->LifeTime == 1)
					{
						CHARACTER *pTargetChar = &CharactersClient[Owner->TargetCharacter];
						OBJECT* pSourceObj = OwnerObject;
						pSourceObj->Owner = &(pTargetChar->Object);

						CreateEffect(ThreadIndex, MODEL_ALICE_DRAIN_LIFE, pSourceObj->Position, pSourceObj->Angle, pSourceObj->Light, 0, pSourceObj);
						PlayBackground(SOUND_SKILL_DRAIN_LIFE);
					}
				}
				break;
			}

			if (--o->LifeTime <= 0)
			{
DeleteSkill:
				o->Live = false;
#ifdef EXPERIMENTAL_MTSKILLPROCESSOR
				ThreadInfo->SkillsDeleted[ThreadInfo->SkillsDeletedCount++] = n;
#else
				g_SkillProcessorsFree[g_SkillProcessorsFreeCount++] = n;
#endif
			}
		}
	}
}

void PostProcessSkills(mu_uint32 ThreadCount)
{
#ifdef EXPERIMENTAL_MTSKILLPROCESSOR
	for (mu_uint32 n = 0; n < ThreadCount; ++n)
	{
		MUThreadInfo *ThreadInfo = &g_ThreadInfo[n];

		if (ThreadInfo->SkillsDeletedCount > 0)
		{
			mu_memcpy(&g_SkillProcessorsFree[g_SkillProcessorsFreeCount], ThreadInfo->SkillsDeleted, ThreadInfo->SkillsDeletedCount * sizeof(mu_uint16));
			g_SkillProcessorsFreeCount += ThreadInfo->SkillsDeletedCount;
			ThreadInfo->SkillsDeletedCount = 0;
		}
	}
#endif
}
#endif