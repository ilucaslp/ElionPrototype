#ifndef __CLTOGS_HELPER_STRUCTS_H__
#define __CLTOGS_HELPER_STRUCTS_H__

#pragma once

#define MIN_HUNTING_RANGE 1
#define MAX_HUNTING_RANGE 8
#define MAX_HELPER_SKILLS 3
#define MAX_HELPER_BUFFSKILL 3

#define MAX_HELPERSETUP_OBITEMS (30)

namespace BUTTON_SKILLINDEX
{
	enum
	{
		BSI_BASIC,
		BSI_ACT1,
		BSI_ACT2,
		BSI_BUFF1,
		BSI_BUFF2,
		BSI_BUFF3,
		BSI_MAX,
	};
};

class HELPER_SKILLSETTING
{
public:
	enum
	{
		HSS_CON_BEGIN,
		HSS_CON_RANGE = HSS_CON_BEGIN,
		HSS_CON_ATTACKING,
	};
	enum
	{
		HSS_MOB_BEGIN,
		HSS_MOB_2PLUS = HSS_MOB_BEGIN,
		HSS_MOB_3PLUS,
		HSS_MOB_4PLUS,
		HSS_MOB_5PLUS,
	};

public:
	HELPER_SKILLSETTING()
	{
		Reset();
	}

	void Reset()
	{
		SkillIndex = -1;
		EnableCon = false;
		EnableDelay = false;
		DelayTime = 0;
		AttackType = HSS_CON_RANGE;
		MobsCountType = HSS_MOB_2PLUS;
	}

public:
	mu_int32 SkillIndex;
	mu_boolean EnableCon;
	mu_boolean EnableDelay;
	mu_uint16 DelayTime;
	mu_uint8 AttackType;
	mu_uint8 MobsCountType;
};

class HELPER_POTIONSETTING
{
public:
	HELPER_POTIONSETTING()
	{
		Reset();
	}

	void Reset()
	{
		EnablePotion = true;
		EnableAutoHeal = false;

		MinLifePercent = 0.5f;
		MinAutoHealPercent = 0.5f;
	}

public:
	mu_boolean EnablePotion;
	mu_boolean EnableAutoHeal;
	mu_float MinLifePercent;
	mu_float MinAutoHealPercent;
};

class HELPER_PARTYSETTING
{
public:
	HELPER_PARTYSETTING()
	{
		Reset();
	}

	void Reset()
	{
		EnableParty = false;
		EnableHealMin = false;
		HealMin = 0.5f;
	}

public:
	mu_boolean EnableParty;
	mu_boolean EnableHealMin;
	mu_float HealMin;
};

class HELPER_HUNTINGSETTING
{
public:
	HELPER_HUNTINGSETTING()
	{
		Reset();
	}

	void Reset()
	{
		Range = MAX_HUNTING_RANGE;

		EnableRegularAttack = false;
		EnableSkillsClosely = false;
		EnableLongDistCounterAttack = false;
		EnableReturnPosition = true;
		ReturnPositionTime = 10;

		EnableBuffs = false;
	}

public:
	mu_uint8 Range;

	HELPER_POTIONSETTING PotionSetting;
	HELPER_PARTYSETTING PartySetting;

	mu_boolean EnableRegularAttack;
	mu_boolean EnableSkillsClosely;
	mu_boolean EnableLongDistCounterAttack;
	mu_boolean EnableReturnPosition;
	mu_uint16 ReturnPositionTime;

	mu_boolean EnableBuffs;
	HELPER_SKILLSETTING Skills[BUTTON_SKILLINDEX::BSI_MAX];
};

class HELPER_ITEMSETTING
{
public:
	HELPER_ITEMSETTING()
	{
		Reset();
	}

	void Reset()
	{
		ItemIndex = -1;
		ItemLevel = 0;
		MinLevel = 0;
		IsLuck = false;
		IsSkill = false;
		IsOption = false;
		IsExcellent = false;
		IsSetItem = false;
		IsSocket = false;
	}

public:
	mu_int32 ItemIndex;
	mu_int16 ItemLevel;
	mu_int16 MinLevel;
	mu_boolean IsLuck;
	mu_boolean IsSkill;
	mu_boolean IsOption;
	mu_boolean IsExcellent;
	mu_boolean IsSetItem;
	mu_boolean IsSocket;
};

class HELPER_OBTAININGSETTING
{
public:
	HELPER_OBTAININGSETTING()
	{
		Reset();
	}

	void Reset()
	{
		Range = MAX_HUNTING_RANGE;
		EnableRepair = false;
		EnablePickAll = true;
		EnablePick = false;
		PickJewels = false;
		PickSetItems = false;
		PickZen = false;
		PickExcellent = false;
		PickExtra = false;
	}

public:
	mu_uint8 Range;
	mu_boolean EnableRepair;
	mu_boolean EnablePickAll;
	mu_boolean EnablePick;
	mu_boolean PickJewels;
	mu_boolean PickSetItems;
	mu_boolean PickZen;
	mu_boolean PickExcellent;
	mu_boolean PickExtra;
};

class HELPER_OBTAININGSETTINGEX : public HELPER_OBTAININGSETTING
{
public:
	HELPER_OBTAININGSETTINGEX()
	{
		Reset();
	}

	void Reset()
	{
		HELPER_OBTAININGSETTING::Reset();
		ExtraItems.clear();
	}

public:
	std::vector<HELPER_ITEMSETTING> ExtraItems;
};

class HELPER_SETTING
{
public:
	mu_boolean operator!=(const HELPER_SETTING &r)
	{
		if (mu_memcmp(&this->Hunting, &r.Hunting, sizeof(HELPER_HUNTINGSETTING)) != 0)
		{
			return true;
		}

		const HELPER_OBTAININGSETTING *lo = static_cast<const HELPER_OBTAININGSETTING*>(&this->Obtaining);
		const HELPER_OBTAININGSETTING *ro = static_cast<const HELPER_OBTAININGSETTING*>(&r.Obtaining);

		if (mu_memcmp(lo, ro, sizeof(HELPER_OBTAININGSETTING)) != 0)
		{
			return true;
		}

		if (this->Obtaining.ExtraItems.size() != r.Obtaining.ExtraItems.size() ||
			(this->Obtaining.ExtraItems.size() > 0 &&
				mu_memcmp(this->Obtaining.ExtraItems.data(), r.Obtaining.ExtraItems.data(), sizeof(HELPER_ITEMSETTING) * this->Obtaining.ExtraItems.size()) != 0))
		{
			return true;
		}

		return false;
	}

public:
	HELPER_HUNTINGSETTING Hunting;
	HELPER_OBTAININGSETTINGEX Obtaining;
};

#endif