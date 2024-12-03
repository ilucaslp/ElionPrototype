#ifndef __MU_SOCKETSYSTEM_H__
#define __MU_SOCKETSYSTEM_H__

#pragma once

enum _SOCKET_OPTION_TYPE
{
	SOT_SOCKET_ITEM_OPTIONS,
	SOT_MIX_SET_BONUS_OPTIONS,
	SOT_EQUIP_SET_BONUS_OPTIONS,
	MAX_SOCKET_OPTION_TYPES,
};

const mu_int32 MAX_SOCKET_OPTION = 50;
const mu_int32 MAX_SOCKET_OPTION_NAME_LENGTH = 64;
const mu_int32 MAX_SPHERE_LEVEL = 5;
const mu_int32 MAX_SOCKET_TYPES = 6;

enum _SOCKET_OPTION_CATEGORY
{
	SOC_NULL,
	SOC_IMPROVE_ATTACK,
	SOC_IMPROVE_DEFENSE,
	SOC_IMPROVE_WEAPON,
	SOC_IMPROVE_ARMOR,
	SOC_IMPROVE_BATTLE,
	SOC_IMPROVE_STATUS,
	SOC_UNIQUE_OPTION,
	MAX_SOCKET_OPTION_CATEGORY,
};

enum _SOCKET_OPTIONS
{
	SOPT_ATTACK_N_MAGIC_DAMAGE_BONUS_BY_LEVEL = 0,
	SOPT_ATTACK_SPEED_BONUS,
	SOPT_ATTACT_N_MAGIC_DAMAGE_MAX_BONUS,
	SOPT_ATTACK_N_MAGIC_DAMAGE_MIN_BONUS,
	SOPT_ATTACK_N_MAGIC_DAMAGE_BONUS,
	SOPT_DECREASE_AG_USE,

	SOPT_DEFENCE_RATE_BONUS = 10,
	SOPT_DEFENCE_BONUS,
	SOPT_SHIELD_DEFENCE_BONUS,
	SOPT_DECREASE_DAMAGE,
	SOPT_REFLECT_DAMAGE,

	SOPT_MONSTER_DEATH_LIFE_BONUS = 16,
	SOPT_MONSTER_DEATH_MANA_BONUS,
	SOPT_SKILL_DAMAGE_BONUS,
	SOPT_ATTACK_RATE_BONUS,
	SOPT_INCREASE_ITEM_DURABILITY,

	SOPT_LIFE_REGENERATION_BONUS = 21,
	SOPT_MAX_LIFE_BONUS,
	SOPT_MAX_MANA_BONUS,
	SOPT_MANA_REGENERATION_BONUS,
	SOPT_MAX_AG_BONUS,
	SOPT_AG_REGENERATION_BONUS,

	SOPT_EXCELLENT_DAMAGE_BONUS = 29,
	SOPT_EXCELLENT_DAMAGE_RATE_BONUS,
	SOPT_CRITICAL_DAMAGE_BONUS,
	SOPT_CRITICAL_DAMAGE_RATE_BONUS,

	SOPT_STRENGTH_BONUS = 34,
	SOPT_DEXTERITY_BONUS,
	SOPT_VITALITY_BONUS,
	SOPT_ENERGY_BONUS,
	SOPT_REQUIRED_STENGTH_BONUS,
	SOPT_REQUIRED_DEXTERITY_BONUS,

	SOPT_UNIQUE01 = 41,
	SOPT_UNIQUE02,
};

enum _SOCKET_BONUS_OPTIONS
{
	SBOPT_ATTACK_DAMAGE_BONUS = 0,
	SBOPT_SKILL_DAMAGE_BONUS,
	SBOPT_MAGIC_POWER_BONUS,
	SBOPT_SKILL_DAMAGE_BONUS_2,
	SBOPT_DEFENCE_BONUS,
	SBOPT_MAX_LIFE_BONUS,
};

typedef struct _SOCKET_OPTION_INFO
{
	mu_text m_szOptionName[MAX_SOCKET_OPTION_NAME_LENGTH];

	mu_int32 m_iOptionID;
	mu_int32 m_iOptionCategory;
	mu_int8 m_bOptionType;
	mu_int32 m_iOptionValue[5];
	mu_uint8 m_bySocketCheckInfo[6];
} SOCKET_OPTION_INFO;

typedef struct _SOCKET_OPTION_STATUS_BONUS
{
	mu_int32 m_iAttackDamageMinBonus;
	mu_int32 m_iAttackDamageMaxBonus;
	mu_int32 m_iAttackRateBonus;
	mu_int32 m_iSkillAttackDamageBonus;
	mu_int32 m_iAttackSpeedBonus;
	mu_int32 m_iMagicPowerBonus;

	mu_int32 m_iDefenceBonus;
	mu_float m_fDefenceRateBonus;
	mu_int32 m_iShieldDefenceBonus;

	mu_int32 m_iStrengthBonus;
	mu_int32 m_iDexterityBonus;
	mu_int32 m_iVitalityBonus;
	mu_int32 m_iEnergyBonus;
} SOCKET_OPTION_STATUS_BONUS;

class OBJECT;

class CSocketItemMgr
{
public:
	CSocketItemMgr();
	~CSocketItemMgr();

	mu_boolean IsSocketItem(const ITEM* pItem);
	mu_boolean IsSocketItem(const OBJECT* pObject);
	mu_int32 GetSeedShpereSeedID(const ITEM* pItem);
	mu_int32 GetSocketCategory(mu_int32 iSeedID);

	mu_int32 AttachToolTipForSocketItem(const ITEM* pItem, mu_int32 iTextNum);
	mu_int32 AttachToolTipForSeedSphereItem(const ITEM* pItem, mu_int32 iTextNum);

	void CheckSocketSetOption();
	mu_boolean IsSocketSetOptionEnabled();
	void RenderToolTipForSocketSetOption(mu_float iPos_x, mu_float iPos_y);

	void CreateSocketOptionText(mu_text *pszOptionText, const mu_size OptionTextLength, mu_int32 iSeedID, mu_int32 iSphereLv);

	mu_int64 CalcSocketBonusItemValue(const ITEM * pItem, mu_int64 iOrgGold);

	mu_int32 GetSocketOptionValue(const ITEM * pItem, mu_int32 iSocketIndex);

	void CalcSocketStatusBonus();

	mu_boolean OpenSocketItemScript(const mu_text* szFileName);

protected:
	mu_boolean IsSocketItem(mu_int32 iItemType);
	void CalcSocketOptionValueText(mu_text *pszOptionValueText, const mu_size OptionValueTextLength, mu_int32 iOptionType, mu_float fOptionValue);
	mu_int32 CalcSocketOptionValue(mu_int32 iOptionType, mu_float fOptionValue);

public:
	SOCKET_OPTION_STATUS_BONUS m_StatusBonus;

protected:
	SOCKET_OPTION_INFO m_SocketOptionInfo[MAX_SOCKET_OPTION_TYPES][MAX_SOCKET_OPTION];
	mu_int32 m_iNumEquitSetBonusOptions;

	std::deque<mu_uint32> m_EquipSetBonusList;
};

extern CSocketItemMgr g_SocketItemMgr;

#endif