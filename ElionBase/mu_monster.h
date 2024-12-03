#ifndef __MU_MONSTER_H__
#define __MU_MONSTER_H__

#pragma once

typedef struct
{
	mu_uint16 Life;
	mu_uint16 MoveSpeed;
	mu_uint16 AttackSpeed;
	mu_uint16 AttackDamageMin;
	mu_uint16 AttackDamageMax;
	mu_uint16 Defense;
	mu_uint16 MagicDefense;
	mu_uint16 AttackRating;
	mu_uint16 SuccessfulBlocking;
} MONSTER_ATTRIBUTE;

struct MONSTER_SCRIPT
{
	mu_boolean Used;
	mu_text Name[64];
	mu_uint16 Level;
	MONSTER_ATTRIBUTE Attribute;

	MONSTER_SCRIPT()
	{
		Used = false;
	}
};

typedef struct
{
	mu_int16 Type;
	mu_uint8 Level;
	mu_int32 Experince;
	MONSTER_ATTRIBUTE Attribute;
} MONSTER;

#define MAX_MONSTERSKILL_NUM 10

typedef struct 
{
	mu_int32 Skill_Num[MAX_MONSTERSKILL_NUM];
	mu_int32 Slot;
} Script_Skill;

extern MONSTER_SCRIPT MonsterScript[MAX_MONSTER];
extern Script_Skill MonsterSkill[MAX_MONSTER];

mu_boolean OpenMonsterScript(const mu_text *filename);
mu_boolean OpenMonsterSkillScript(const mu_text *filename);

const mu_text *GetMonsterName(mu_int32 MonsterType);

#endif