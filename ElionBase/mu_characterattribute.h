#ifndef __MU_CHARACTERATTRIBUTE_H__
#define __MU_CHARACTERATTRIBUTE_H__

#pragma once

typedef struct
{
	mu_string Name;
	mu_uint8 Class;
	mu_uint8 Skin;
	mu_uint16 Level;
	mu_int64 Experience;
	mu_int64 NextExperince;

	mu_uint32 Strength;
	mu_uint32 Dexterity;
	mu_uint32 Vitality;
	mu_uint32 Energy;
	mu_uint32 Charisma;

	mu_float Life;
	mu_float Mana;
	mu_float LifeMax;
	mu_float ManaMax;
	mu_float Shield;
	mu_float ShieldMax;
	mu_float SkillMana;
	mu_float SkillManaMax;

	mu_uint16 AttackRatingPK;
	mu_uint16 SuccessfulBlockingPK;
	mu_uint16 AddStrength;
	mu_uint16 AddDexterity;
	mu_uint16 AddVitality;
	mu_uint16 AddEnergy;
	mu_uint16 AddLifeMax;
	mu_uint16 AddManaMax;
	mu_uint16 AddCharisma;
	mu_uint8 Ability;
	mu_uint16 AbilityTime[3];

	mu_int16 AddPoint;
	mu_int16 MaxAddPoint;
	mu_uint16 wMinusPoint;
	mu_uint16 wMaxMinusPoint;
	mu_uint16 AttackSpeed;
	mu_uint16 AttackRating;
	mu_uint16 AttackDamageMinRight;
	mu_uint16 AttackDamageMaxRight;
	mu_uint16 AttackDamageMinLeft;
	mu_uint16 AttackDamageMaxLeft;
	mu_uint16 MagicSpeed;
	mu_uint16 MagicDamageMin;
	mu_uint16 MagicDamageMax;
	mu_uint16 CurseDamageMin;
	mu_uint16 CurseDamageMax;
	mu_uint16 CriticalDamage;
	mu_uint16 SuccessfulBlocking;
	mu_uint16 Defense;
	mu_uint16 MagicDefense;
	mu_uint16 WalkSpeed;
	mu_uint16 LevelUpPoint;
	mu_uint8 SkillNumber;
	mu_uint8 SkillMasterNumber;
	std::array<mu_uint16, MAX_SKILLS> Skill;
	std::array<mu_int32, MAX_SKILLS> SkillDelay;
} CHARACTER_ATTRIBUTE;

#endif