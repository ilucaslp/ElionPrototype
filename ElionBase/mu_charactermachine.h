#ifndef __MU_CHARACTERMACHINE_H__
#define __MU_CHARACTERMACHINE_H__

#pragma once

typedef struct _MASTER_LEVEL_VALUE
{
	mu_int16 nMLevel;
	mu_int64 lMasterLevel_Experince;
	mu_int64 lNext_MasterLevel_Experince;

	mu_int16 nAddMPoint;
	mu_int16 nMLevelUpMPoint;
	mu_int16 nTotalMPoint;
	mu_int16 nMaxPoint;
} MASTER_LEVEL_VALUE;

class CHARACTER_MACHINE
{
public:
	CHARACTER_ATTRIBUTE Character;
	ITEM Equipment[GAME::MAX_EQUIPMENT];
	mu_int64 Gold;
	mu_int64 StorageGold;
	MONSTER Enemy;

	mu_uint16 AttackDamageRight;
	mu_uint16 AttackDamageLeft;
	mu_uint16 CriticalDamage;

	mu_uint16 FinalAttackDamageRight;
	mu_uint16 FinalAttackDamageLeft;
	mu_uint16 FinalHitPoint;
	mu_uint16 FinalAttackRating;
	mu_uint16 FinalDefenseRating;

	mu_uint8 InfinityArrowAdditionalMana;

	void Init();
	void InitAddValue();
	mu_uint8 GetSkillMasteryType(mu_int32 iType);
	void SetCharacter(mu_uint8 Class);
	void InputEnemyAttribute(MONSTER *Enemy);
	mu_boolean IsZeroDurability();
	void CalculateDamage();
	void CalculateCriticalDamage();
	void CalculateMagicDamage();
	void CalculateCurseDamage();
	void CalculateAttackRating();
	void CalculateAttackSpeed();
	void CalculateSuccessfulBlocking();
	void CalculateAttackRatingPK();
	void CalculateSuccessfulBlockingPK();
	void CalculateDefense();
	void CalculateMagicDefense();
	void CalculateWalkSpeed();
	void CalculateNextExperince();
	void CalulateMasterLevelNextExperience();
	void CalculateAll();

	void GetMagicSkillDamage(mu_int32 iType, mu_int32 *piMinDamage, mu_int32 *piMaxDamage);
	void GetCurseSkillDamage(mu_int32 iType, mu_int32 *piMinDamage, mu_int32 *piMaxDamage);
	void GetSkillDamage(mu_int32 iType, mu_int32 *piMinDamage, mu_int32 *piMaxDamage);

	void CalculateBasicState();

	void getAllAddStateOnlyExValues(mu_int32 &iAddStrengthExValues,
		mu_int32 &iAddDexterityExValues,
		mu_int32 &iAddVitalityExValues,
		mu_int32 &iAddEnergyExValues,
		mu_int32 &iAddCharismaExValues);
};

extern MASTER_LEVEL_VALUE Master_Level_Data;
extern CLASS_ATTRIBUTE ClassAttribute[GAME::MAX_CLASS];
extern CHARACTER_MACHINE *CharacterMachine;
extern CHARACTER_ATTRIBUTE *CharacterAttribute;

extern mu_boolean g_AddDefense;

#endif