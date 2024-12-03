#ifndef __MU_EFFECTS_H__
#define __MU_EFFECTS_H__

#pragma once

struct EFFECT_WAIT
{
	mu_int16 PKKey;
	mu_uint16 SkillIndex;
	mu_uint16 Skill;
	mu_uint16 SkillSerialNum;
	mu_int16 sTargetIndex;

	mu_int32 Type;
	mu_int32 SubType;

	mu_float Scale;

	EVector3 Position;
	EVector3 Angle;
	EVector3 Light;

	OBJECT *Owner;
};

struct EFFECT_DELETE
{
	mu_int32 Type;
	mu_int32 SubType;
	OBJECT *Owner;
};

extern MUCriticalSectionSpinLock g_EffectsLock;
extern std::array<OBJECT, MAX_EFFECTS> g_Effects;
extern std::array<MUObjectExtra, MAX_EFFECTS> g_EffectsExtra;
extern std::array<mu_uint16, MAX_EFFECTS + MAX_TASKING_THREADS> g_EffectsQueue;
extern std::array<mu_uint16, MAX_EFFECTS> g_EffectsFree;
extern mu_uint16 g_EffectsCurCount;
extern mu_uint16 g_EffectsFreeCount;
extern mu_atomic_int32_t g_EffectsDeleteCount;
extern mu_boolean g_EffectsLocked;

extern std::array<EFFECT_WAIT, MAX_EFFECTS_WAIT> g_EffectsWait;
extern mu_atomic_uint32_t g_EffectsWaitCount;
extern std::array<EFFECT_DELETE, MAX_EFFECTS_WAIT> g_EffectsErase;
extern mu_uint16 g_EffectsEraseCount;

void CreateInferno(const mu_uint32 ThreadIndex, EVector3 Position, mu_int32 SubType = 0);

template<mu_boolean isLockedAllowed = true>
void CreateEffect(const mu_uint32 ThreadIndex, mu_int32 Type, EVector3 Position, EVector3 Angle,
	EVector3 Light, mu_int32 SubType = 0, OBJECT *Owner = nullptr, mu_int16 PKKey = -1, mu_uint16 SkillIndex = 0, mu_uint16 Skill = 0, mu_uint16 SkillSerialNum = 0,
	mu_float Scale = 0.0f, mu_int16 sTargetIndex = -1);
void DeleteEffect(mu_int32 Type, OBJECT *Owner, mu_int32 SubType = -1);
mu_boolean SearchEffect(mu_int32 iType, OBJECT* pOwner, mu_int32 iSubType = -1);

// mu_effectsmagicskill
void CreateMagicShiny(const mu_uint32 ThreadIndex, CHARACTER *c, mu_int32 Hand = 0);
void CreateTeleportBegin(const mu_uint32 ThreadIndex, OBJECT *o);
void CreateTeleportEnd(const mu_uint32 ThreadIndex, OBJECT *o);
void CreateArrow(const mu_uint32 ThreadIndex, CHARACTER *c, OBJECT *o, OBJECT *to, mu_uint16 SkillIndex, mu_uint16 Skill, mu_uint16 SKKey);
void CreateArrows(const mu_uint32 ThreadIndex, CHARACTER *c, OBJECT *o, OBJECT *to, mu_uint16 SkillIndex = 0, mu_uint16 Skill = 1, mu_uint16 SKKey = 0);
void CreateSpark(const mu_uint32 ThreadIndex, mu_int32 Type, CHARACTER *tc, EVector3 Position, EVector3 Angle);
void CreateBlood(const mu_uint32 ThreadIndex, OBJECT *o);
void CreateForce(const mu_uint32 ThreadIndex, OBJECT* o, EVector3 &Pos);
void CreateHealing(const mu_uint32 ThreadIndex, OBJECT *o);

#endif