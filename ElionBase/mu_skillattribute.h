#ifndef __MU_SKILLATTRIBUTE_H__
#define __MU_SKILLATTRIBUTE_H__

#pragma once

typedef struct
{
	mu_text Name[64];
	mu_uint16 Level;
	mu_uint16 Damage;
	mu_uint16 Mana;
	mu_uint16 AbilityGuage;
	mu_uint8 Distance;
	mu_int32 Delay;

	mu_uint16 Strength;
	mu_uint16 Dexterity;
	mu_uint16 Energy;
	mu_uint16 Charisma;

	mu_uint8 MasteryType;
	mu_uint8 SkillUseType;
	mu_uint8 SkillBrand;
	mu_uint8 KillCount;
	mu_uint8 RequireDutyClass[MAX_DUTY_CLASS];
	mu_uint8 RequireClass[GAME::MAX_CLASS];
	mu_uint16 Magic_Icon;

	mu_uint8 TypeSkill;
} SKILL_ATTRIBUTE;

extern SKILL_ATTRIBUTE SkillAttribute[MAX_SKILLS];

mu_boolean OpenSkillScript(const mu_text *filename);

#endif