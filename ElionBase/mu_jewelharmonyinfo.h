#ifndef __MU_JEWELHARMONYINFO_H__
#define __MU_JEWELHARMONYINFO_H__

#pragma once

#define MAXHARMONYJEWELOPTIONTYPE 3
#define MAXHARMONYJEWELOPTIONINDEX 10

struct HarmonyJewelOption
{
	mu_int32 OptionType;
	mu_string Name;
	mu_int32 Minlevel;
	mu_int32 HarmonyJewelLevel[14];
	mu_int32 Zen[14];

	HarmonyJewelOption() : OptionType(-1), Minlevel(-1)
	{
		for (mu_int32 i = 0; i < 14; ++i)
		{
			HarmonyJewelLevel[i] = -1;
			Zen[i] = -1;
		}
	}
};

struct NaturalAbility
{
	mu_int32 SI_force;
	mu_int32 SI_activity;

	NaturalAbility() : SI_force(0), SI_activity(0) {}
};

struct StrikingPower
{
	mu_int32 SI_minattackpower;
	mu_int32 SI_maxattackpower;
	mu_int32 SI_magicalpower;
	mu_int32 SI_attackpowerRate;
	mu_int32 SI_skillattackpower;

	StrikingPower() : SI_minattackpower(0),
		SI_maxattackpower(0),
		SI_magicalpower(0),
		SI_attackpowerRate(0),
		SI_skillattackpower(0) {}
};

struct StrengthenDefense
{
	mu_int32 SI_defense;
	mu_int32 SI_AG;
	mu_int32 SI_HP;
	mu_int32 SI_defenseRate;

	StrengthenDefense() : SI_defense(0), SI_AG(0), SI_HP(0), SI_defenseRate(0) {}
};

struct StrengthenCapability
{
	NaturalAbility SI_NB;
	mu_boolean	   SI_isNB;

	StrikingPower  SI_SP;
	mu_boolean     SI_isSP;

	StrengthenDefense SI_SD;
	mu_boolean		  SI_isSD;

	StrengthenCapability() : SI_isNB(false), SI_isSP(false), SI_isSD(false) {}
};

enum StrengthenItem
{
	SI_Weapon = 0,
	SI_Staff,
	SI_Defense,
	SI_None,
};

typedef HarmonyJewelOption HARMONYJEWELOPTION;

class JewelHarmonyInfo
{
public:
	JewelHarmonyInfo();
	virtual ~JewelHarmonyInfo();

	const mu_boolean OpenJewelHarmonyInfoFile(const mu_text *filename);

public:
	const StrengthenItem		GetItemType(mu_int32 type);
	const HARMONYJEWELOPTION&   GetHarmonyJewelOptionInfo(mu_int32 type, mu_int32 option);
	void						GetStrengthenCapability(StrengthenCapability* pitemSC, const ITEM* pitem, const mu_int32 index);

public:
	const mu_boolean IsHarmonyJewelOption(mu_int32 type, mu_int32 option);

private:
	HARMONYJEWELOPTION m_OptionData[MAXHARMONYJEWELOPTIONTYPE][MAXHARMONYJEWELOPTIONINDEX];
};

inline
const HARMONYJEWELOPTION& JewelHarmonyInfo::GetHarmonyJewelOptionInfo(mu_int32 type, mu_int32 option)
{
	return m_OptionData[type][option - 1];
}

extern JewelHarmonyInfo g_pUIJewelHarmonyinfo;

#endif