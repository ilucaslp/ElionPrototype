#ifndef __MU_ENUM_H__
#define __MU_ENUM_H__

#pragma once

enum RENDER_TYPE : mu_uint8
{
	// Texture
	RT_TEXTURE,
	RT_TEXTURE_SHADOWMAP,
	RT_TEXTURE_WAVE,

	// Chromes
	RT_CHROME,
	RT_CHROME2,
	RT_CHROME3,
	RT_CHROME4,
	RT_CHROME5,
	RT_CHROME6,
	RT_CHROME7,
	RT_METAL,
	RT_OIL,

	// Color
	RT_COLOR,
	RT_COLOR_SHADOWMAP,
	RT_COLOR_WAVE,

	RT_MAX,
};

enum RENDER_EXTRA_FLAG
{
	REXF_ENABLED_DEPTH = (1 << 0),
	REXF_DOPPELGANGER = (1 << 1),
	REXF_CULL_FRONT = (1 << 2),
	REXF_CULL_BACK = (1 << 3),
	REXF_MAX,
};

enum SAMPLER_TYPE
{
	SAMPLER_CLAMP,
	SAMPLER_WRAP,
};

enum eCursedTempleState
{
	eCursedTempleState_None = 0,
	eCursedTempleState_Wait,
	eCursedTempleState_Ready,
	eCursedTempleState_Play,
	eCursedTempleState_End,
};

enum eCursedTempleTeam
{
	eTeam_Allied = 0,
	eTeam_Illusion,
	eTeam_Count,
};

enum eTYPEBOW
{
	BOWTYPE_NONE = 0,
	BOWTYPE_BOW,
	BOWTYPE_CROSSBOW,
};

enum ITEM_EX_SRC_TYPE
{
	ITEM_EX_SRC_EQUIPMENT = 1,
};

enum ITEM_COLORSTATE
{
	ITEM_COLOR_NORMAL = 0,
	ITEM_COLOR_DURABILITY_50,
	ITEM_COLOR_DURABILITY_70,
	ITEM_COLOR_DURABILITY_80,
	ITEM_COLOR_DURABILITY_100,
	ITEM_COLOR_TRADE_WARNING,
};

enum
{
	SKILL_ABILITY_NONE = 0,
	SKILL_ABILITY_FIRE,
	SKILL_ABILITY_THUNDER,
	SKILL_ABILITY_ICE,
	SKILL_ABILITY_POSION,
	SKILL_ABILITY_EARTH,
	SKILL_ABILITY_WIND,
	SKILL_ABILITY_WATER
};

enum
{
	SKILL_USE_TYPE_NONE = 0,
	SKILL_USE_TYPE_MASTER,			// 1
	SKILL_USE_TYPE_BRAND,			// 2
	SKILL_USE_TYPE_MASTERLEVEL,		// 3
	SKILL_USE_TYPE_MASTERACTIVE,	// 4
};

enum MonsterSkillType
{
	ATMON_SKILL_BIGIN		= 0,
	ATMON_SKILL_THUNDER		= 1,
	ATMON_SKILL_WIND		= 2,
	ATMON_SKILL_FIRELAY		= 3,
	ATMON_SKILL_CHARM		= 4,
	ATMON_SKILL_TOUCH		= 5,
	ATMON_SKILL_NUM1		= 6,
	ATMON_SKILL_NUM2		= 7,
	ATMON_SKILL_NUM3		= 8,
	ATMON_SKILL_NUM4		= 9,
	ATMON_SKILL_NUM5		= 10,
	ATMON_SKILL_NUM6		= 11,
	ATMON_SKILL_NUM7		= 12,
	ATMON_SKILL_NUM8		= 13,
	ATMON_SKILL_NUM9		= 14,
	ATMON_SKILL_NUM10		= 15,
	ATMON_SKILL_NUM11		= 16,
	ATMON_SKILL_NUM12		= 17,
	ATMON_SKILL_NORMAL		= 18,
	ATMON_SKILL_STORM		= 19,
	ATMON_SKILL_SUMMON		= 20,
	ATMON_SKILL_HELL		= 21,
	ATMON_SKILL_TELEPORT	= 22,
	ATMON_SKILL_SHOCK		= 23,
	ATMON_SKILL_CRYSTAL		= 24,
	ATMON_SKILL_MAYASTORM	= 25,
	ATMON_SKILL_MAYACRYSTAL = 26,
	ATMON_SKILL_COURSED_STUN   = 27,
	ATMON_SKILL_COURSED_POISON = 28,
	ATMON_SKILL_SERUFAN_POISONGEM = 34,
	ATMON_SKILL_SERUFAN_COOLSTORM = 35,
	ATMON_SKILL_SERUFAN_COOLSHOCK = 36,
	ATMON_SKILL_SERUFAN_FALLING = 37,
	ATMON_SKILL_SERUFAN_SUMMON = 38,
	ATMON_SKILL_SERUFAN_HEAL = 39,
	ATMON_SKILL_SERUFAN_FREEZE = 40,
	ATMON_SKILL_SERUFAN_TELEPORT = 41,
	ATMON_SKILL_SERUFAN_SUPERMAN = 42,
	ATMON_SKILL_EMPIREGUARDIAN_BERSERKER = 59,
	ATMON_SKILL_EMPIREGUARDIAN_GAION_02_BLOODATTACK = 64,
	ATMON_SKILL_EMPIREGUARDIAN_GAION_03_GIGANTIKSTORM = 65,
	ATMON_SKILL_EMPIREGUARDIAN_GAION_04_FLAMEATTACK = 66,
	ATMON_SKILL_EMPIREGUARDIAN_GAION_01_GENERALATTACK = 67,
	ATMON_SKILL_EX_BLOODYGOLUEM_ATTACKSKILL = 68,
	ATMON_SKILL_EX_BLOODYWITCHQUEEN_ATTACKSKILL = 69,
	ATMON_SKILL_EX_BERSERKERWARRIOR_ATTACKSKILL = 70,
	ATMON_SKILL_EX_KENTAURUSWARRIOR_ATTACKSKILL = 14,
	ATMON_SKILL_EX_GENOSIDEWARRIOR_ATTACKSKILL = 71,
	ATMON_SKILL_EX_SAPIQUEEN_ATTACKSKILL = 72,
	ATMON_SKILL_EX_ICENAPIN_ATTACKSKILL = 73,
	ATMON_SKILL_EX_SHADOWMASTER_ATTACKSKILL = 74,
	ATMON_SKILL_EX_DARKMEMUD_ATTACKSKILL = 75,
	ATMON_SKILL_EX_DARKGIANT_ATTACKSKILL = 76,
	ATMON_SKILL_EX_DARKAIONNIGHT_ATTACKSKILL = 32,
	ATMON_SKILL_EX_DARKCOOLERTIN_ATTACKSKILL = 77,
	ATMON_SKILL_END,
};

enum ActionSkillType
{
	AT_SKILL_POISON			= 1,
	AT_SKILL_METEO			= 2,
	AT_SKILL_THUNDER		= 3,
	AT_SKILL_FIREBALL		= 4,
	AT_SKILL_FLAME			= 5,
	AT_SKILL_TELEPORT		= 6,
	AT_SKILL_SLOW			= 7,
	AT_SKILL_STORM			= 8,
	AT_SKILL_EVIL			= 9,
	AT_SKILL_HELL			= 10,
	AT_SKILL_POWERWAVE		= 11,
	AT_SKILL_FLASH			= 12,
	AT_SKILL_BLAST			= 13,
	AT_SKILL_INFERNO		= 14,
	AT_SKILL_TELEPORT_B		= 15,
	AT_SKILL_WIZARDDEFENSE	= 16,
	AT_SKILL_ENERGYBALL		= 17,
	
	AT_SKILL_BLAST_POISON		= 38,
	AT_SKILL_BLAST_FREEZE		= 39,
	AT_SKILL_BLAST_HELL			= 40,
	AT_SKILL_BLAST_HELL_BEGIN	= 58,
	
	AT_SKILL_BLOCKING   = 18,
	AT_SKILL_SWORD1     = 19,
	AT_SKILL_SWORD2     = 20,
	AT_SKILL_SWORD3     = 21,
	AT_SKILL_SWORD4     = 22,
	AT_SKILL_SWORD5     = 23,
	AT_SKILL_SPEAR		= 47,
	AT_SKILL_VITALITY	= 48,
	AT_SKILL_RIDER      = 49,

	AT_SKILL_CROSSBOW   = 24,
	AT_SKILL_BOW        = 25,
	AT_SKILL_HEALING    = 26,
	AT_SKILL_DEFENSE    = 27,
	AT_SKILL_ATTACK     = 28,
	AT_SKILL_SUMMON     = 30,
	AT_SKILL_PARALYZE	= 51,
	AT_SKILL_PIERCING	= 52,
	AT_SKILL_IMPROVE_AG = 53,
	AT_SKILL_BLAST_CROSSBOW4 = 54,
	
	AT_SKILL_REDUCEDEFENSE	= 55,
	AT_SKILL_ICE_BLADE      = 56,

	AT_SKILL_WHEEL      = 41,
	AT_SKILL_FURY_STRIKE= 42,
	AT_SKILL_ONETOONE	= 43,
	AT_SKILL_RUSH       = 44,
	AT_SKILL_JAVELIN    = 45,
	AT_SKILL_DEEPIMPACT = 46,
	AT_SKILL_ONEFLASH   = 57,

	AT_SKILL_BOSS       = 50,

	AT_SKILL_COMBO		= 59,

	AT_SKILL_STRONG_PIER        = 60,
	AT_SKILL_LONGPIER_ATTACK,
	AT_SKILL_DARK_HORSE,
	AT_SKILL_PARTY_TELEPORT,
	AT_SKILL_ADD_CRITICAL,
	AT_SKILL_THUNDER_STRIKE,
	AT_SKILL_LONG_SPEAR,
	AT_SKILL_STUN               ,
	AT_SKILL_REMOVAL_STUN       ,
	AT_SKILL_MANA               ,
	AT_SKILL_INVISIBLE          ,
	AT_SKILL_REMOVAL_INVISIBLE  ,
	AT_SKILL_REMOVAL_BUFF       ,
	AT_SKILL_DEATH_CANNON       ,
	AT_SKILL_SPACE_SPLIT        ,
	AT_SKILL_BRAND_OF_SKILL     ,
	AT_SKILL_PLASMA_STORM_FENRIR	= 76	,
	AT_SKILL_INFINITY_ARROW		,
	AT_SKILL_DARK_SCREAM,
	AT_SKILL_EXPLODE,

	AT_IMPROVE_DAMAGE           ,
	AT_IMPROVE_MAGIC            ,
	AT_IMPROVE_CURSE            ,
	AT_IMPROVE_BLOCKING         ,
	AT_IMPROVE_DEFENSE          ,
	AT_LUCK                     ,
	AT_LIFE_REGENERATION        ,

	AT_IMPROVE_LIFE             ,
	AT_IMPROVE_MANA             ,
	AT_DECREASE_DAMAGE          ,
	AT_REFLECTION_DAMAGE        ,
	AT_IMPROVE_BLOCKING_PERCENT ,
	AT_IMPROVE_GAIN_GOLD        ,
	AT_EXCELLENT_DAMAGE         ,
	AT_IMPROVE_DAMAGE_LEVEL     ,
	AT_IMPROVE_DAMAGE_PERCENT   ,
	AT_IMPROVE_MAGIC_LEVEL      ,
	AT_IMPROVE_MAGIC_PERCENT    ,
	AT_IMPROVE_ATTACK_SPEED     ,
	AT_IMPROVE_GAIN_LIFE        ,
	AT_IMPROVE_GAIN_MANA        ,

	AT_IMPROVE_HP_MAX           ,
	AT_IMPROVE_MP_MAX           ,
	AT_ONE_PERCENT_DAMAGE       ,
	AT_IMPROVE_AG_MAX           ,
	AT_DAMAGE_ABSORB            ,

	AT_DAMAGE_REFLECTION     ,
	AT_RECOVER_FULL_LIFE     ,
	AT_RECOVER_FULL_MANA     ,

	AT_IMPROVE_CHARISMA         ,

	AT_IMPROVE_EVADE            = 110,
		
	AT_SKILL_MONSTER_SUMMON     = 200,
	AT_SKILL_MONSTER_MAGIC_DEF  ,
	AT_SKILL_MONSTER_PHY_DEF    ,
	AT_SKILL_HELLOWIN_EVENT_1	= 205,
	AT_SKILL_HELLOWIN_EVENT_2	= 206,
	AT_SKILL_HELLOWIN_EVENT_3	= 207,
	AT_SKILL_HELLOWIN_EVENT_4	= 208,
	AT_SKILL_HELLOWIN_EVENT_5	= 209,
	AT_SKILL_CURSED_TEMPLE_PRODECTION  = 210,
	AT_SKILL_CURSED_TEMPLE_RESTRAINT   = 211,
	AT_SKILL_CURSED_TEMPLE_TELEPORT    = 212,
	AT_SKILL_CURSED_TEMPLE_SUBLIMATION = 213,
	AT_SKILL_ALICE_DRAINLIFE = 214,
	AT_SKILL_ALICE_CHAINLIGHTNING = 215,
	AT_SKILL_ALICE_LIGHTNINGORB = 216,
	AT_SKILL_ALICE_THORNS = 217,
	AT_SKILL_ALICE_BERSERKER = 218,
	AT_SKILL_ALICE_SLEEP = 219,
	AT_SKILL_ALICE_BLIND = 220,
	AT_SKILL_ALICE_WEAKNESS = 221,
	AT_SKILL_ALICE_ENERVATION = 222,

	AT_SKILL_SUMMON_EXPLOSION = 223,
	AT_SKILL_SUMMON_REQUIEM,
	AT_SKILL_SUMMON_POLLUTION = 225,
	
	AT_SKILL_LIGHTNING_SHOCK = 230,
	AT_SKILL_BLOW_OF_DESTRUCTION = 232,
	
	AT_SKILL_SWELL_OF_MAGICPOWER = 233,

	AT_SKILL_FLAME_STRIKE = 236,
	AT_SKILL_GIGANTIC_STORM = 237,
	AT_SKILL_RECOVER = 234,
	AT_SKILL_MULTI_SHOT = 235,
	AT_SKILL_GAOTIC = 238,
	AT_SKILL_DOPPELGANGER_SELFDESTRUCTION = 239,

	AT_SKILL_ATTACK_RATEUP		= 300,
	AT_SKILL_TOMAN_ATTACKUP		= AT_SKILL_ATTACK_RATEUP + 5,
	AT_SKILL_TOMAN_DEFENCEUP	= AT_SKILL_TOMAN_ATTACKUP + 5,
	AT_SKILL_DURABLE_MINUS1		= AT_SKILL_TOMAN_DEFENCEUP + 5,
	AT_SKILL_DURABLE_MINUS2		= AT_SKILL_DURABLE_MINUS1 + 5,
	AT_SKILL_RESIST_UP_POISON	= AT_SKILL_DURABLE_MINUS2 + 5,
	AT_SKILL_RESIST_UP_LIGHT	= AT_SKILL_RESIST_UP_POISON + 5,
	AT_SKILL_RESIST_UP_ICE		= AT_SKILL_RESIST_UP_LIGHT + 5,
	AT_SKILL_AUTO_RECOVER_LIFE	= AT_SKILL_RESIST_UP_ICE + 5,
	AT_SKILL_GET_MONEY_UP		= AT_SKILL_AUTO_RECOVER_LIFE + 5,
	AT_SKILL_DEF_UP				= AT_SKILL_GET_MONEY_UP + 5,
	AT_SKILL_MAX_HP_UP			= AT_SKILL_DEF_UP + 5,
	AT_SKILL_MAX_AG_UP			= AT_SKILL_MAX_HP_UP + 5,
	AT_SKILL_MANA_RECOVER		= AT_SKILL_MAX_AG_UP + 5,
	AT_SKILL_HP_RECOVER			= AT_SKILL_MANA_RECOVER + 5,
	AT_SKILL_SD_RECOVER			= AT_SKILL_HP_RECOVER + 5,
	AT_SKILL_EXP_UP				= AT_SKILL_SD_RECOVER + 5,
	AT_SKILL_MAX_SD_UP			= AT_SKILL_EXP_UP + 5,
	AT_SKILL_SD_RECOVER_SPD_UP	= AT_SKILL_MAX_SD_UP + 5,
	AT_SKILL_MAX_ATTACKRATE_UP  = AT_SKILL_SD_RECOVER_SPD_UP + 5,
	AT_SKILL_MIN_ATTACKRATE_UP  = AT_SKILL_MAX_ATTACKRATE_UP + 5,
	AT_SKILL_MANA_MINUS_ADD     = AT_SKILL_MIN_ATTACKRATE_UP + 5,
	AT_SKILL_MAX_MANA_UP		= AT_SKILL_MANA_MINUS_ADD + 5,
	AT_SKILL_MIN_MANA_UP		= AT_SKILL_MAX_MANA_UP + 5,
	AT_SKILL_PET_DURABLE_SPD_DN = AT_SKILL_MIN_MANA_UP + 5,
	AT_SKILL_MAX_ATT_MAGIC_UP	= AT_SKILL_PET_DURABLE_SPD_DN + 5,
	AT_SKILL_MIN_ATT_MAGIC_UP   = AT_SKILL_MAX_ATT_MAGIC_UP	+ 5,

	AT_SKILL_SOUL_UP			= AT_SKILL_MIN_ATT_MAGIC_UP	+ 5,
	AT_SKILL_HELL_FIRE_UP		= AT_SKILL_SOUL_UP + 5,
	AT_SKILL_EVIL_SPIRIT_UP		= AT_SKILL_HELL_FIRE_UP + 5,
	AT_SKILL_ICE_UP				= AT_SKILL_EVIL_SPIRIT_UP + 5,
	AT_SKILL_TORNADO_SWORDA_UP	= AT_SKILL_ICE_UP + 5,
	AT_SKILL_BLOW_UP			= AT_SKILL_TORNADO_SWORDA_UP + 5,
	AT_SKILL_ANGER_SWORD_UP		= AT_SKILL_BLOW_UP + 5,
	AT_SKILL_LIFE_UP			= AT_SKILL_ANGER_SWORD_UP + 5,

	AT_SKILL_HEAL_UP			= AT_SKILL_LIFE_UP + 5,
	AT_SKILL_DEF_POWER_UP		= AT_SKILL_HEAL_UP + 5,
	AT_SKILL_ATT_POWER_UP		= AT_SKILL_DEF_POWER_UP + 5,
	AT_SKILL_MANY_ARROW_UP		= AT_SKILL_ATT_POWER_UP + 5,

	AT_SKILL_TORNADO_SWORDB_UP	= AT_SKILL_MANY_ARROW_UP + 5,
	AT_SKILL_BLOOD_ATT_UP		= AT_SKILL_TORNADO_SWORDB_UP + 5,
	AT_SKILL_POWER_SLASH_UP		= AT_SKILL_BLOOD_ATT_UP + 5,
	AT_SKILL_BLAST_UP			= AT_SKILL_POWER_SLASH_UP + 5,

	AT_SKILL_ASHAKE_UP			= AT_SKILL_BLAST_UP + 5,
	AT_SKILL_FIRE_BUST_UP		= AT_SKILL_ASHAKE_UP + 5,
	AT_SKILL_FIRE_SCREAM_UP		= AT_SKILL_FIRE_BUST_UP + 5,

	AT_SKILL_EVIL_SPIRIT_UP_M	= AT_SKILL_FIRE_SCREAM_UP + 5,
	AT_SKILL_ALICE_SLEEP_UP				= AT_SKILL_EVIL_SPIRIT_UP_M + 5,
	AT_SKILL_ALICE_CHAINLIGHTNING_UP	= AT_SKILL_ALICE_SLEEP_UP + 5,
	AT_SKILL_LIGHTNING_SHOCK_UP			= AT_SKILL_ALICE_CHAINLIGHTNING_UP + 5,
	AT_SKILL_ALICE_DRAINLIFE_UP			= AT_SKILL_LIGHTNING_SHOCK_UP + 5,
};

enum PetCommandType
{
	AT_PET_COMMAND_DEFAULT      = 120,
	AT_PET_COMMAND_RANDOM       ,
	AT_PET_COMMAND_OWNER        ,
	AT_PET_COMMAND_TARGET       ,
	AT_PET_COMMAND_END          ,
};

enum OptionType
{
	AT_ATTACK1          = 120,
	AT_ATTACK2,

	AT_STAND1,
	AT_STAND2,
	AT_MOVE1,
	AT_MOVE2,

	AT_DAMAGE1,
	AT_DIE1,
	AT_SIT1,
	AT_POSE1,
	AT_HEALING1,
	AT_GREETING1,
	AT_GOODBYE1,
	AT_CLAP1,
	AT_GESTURE1,
	AT_DIRECTION1,
	AT_UNKNOWN1,
	AT_CRY1,
	AT_CHEER1,
	AT_AWKWARD1,
	AT_SEE1,
	AT_WIN1,
	AT_SMILE1,
	AT_SLEEP1,
	AT_COLD1,
	AT_AGAIN1,
	AT_RESPECT1,
	AT_SALUTE1,
	AT_RUSH1,
	AT_SCISSORS,
	AT_ROCK,
	AT_PAPER,
	AT_HUSTLE,
	AT_PROVOCATION,
	AT_LOOK_AROUND,
	AT_CHEERS,
	AT_HANDCLAP,
	AT_POINTDANCE,
	AT_JACK1,
	AT_JACK2,
	AT_SANTA1_1,
	AT_SANTA1_2,
	AT_SANTA1_3,
	AT_SANTA2_1,
	AT_SANTA2_2,
	AT_SANTA2_3,

	AT_SET_OPTION_IMPROVE_STRENGTH              = 140,
	AT_SET_OPTION_IMPROVE_DEXTERITY,
	AT_SET_OPTION_IMPROVE_ENERGY,
	AT_SET_OPTION_IMPROVE_VITALITY,
	AT_SET_OPTION_IMPROVE_CHARISMA,
	AT_SET_OPTION_IMPROVE_ATTACK_MIN,
	AT_SET_OPTION_IMPROVE_ATTACK_MAX,
	AT_SET_OPTION_IMPROVE_MAGIC_POWER,
	AT_SET_OPTION_IMPROVE_DAMAGE,
	AT_SET_OPTION_IMPROVE_ATTACKING_PERCENT,
	AT_SET_OPTION_IMPROVE_DEFENCE,
	AT_SET_OPTION_IMPROVE_MAX_LIFE,
	AT_SET_OPTION_IMPROVE_MAX_MANA,
	AT_SET_OPTION_IMPROVE_MAX_AG,
	AT_SET_OPTION_IMPROVE_ADD_AG,
	AT_SET_OPTION_IMPROVE_CRITICAL_DAMAGE_PERCENT,
	AT_SET_OPTION_IMPROVE_CRITICAL_DAMAGE,
	AT_SET_OPTION_IMPROVE_EXCELLENT_DAMAGE_PERCENT,
	AT_SET_OPTION_IMPROVE_EXCELLENT_DAMAGE,
	AT_SET_OPTION_IMPROVE_SKILL_ATTACK,
	AT_SET_OPTION_DOUBLE_DAMAGE,
	AT_SET_OPTION_DISABLE_DEFENCE,
	AT_SET_OPTION_IMPROVE_SHIELD_DEFENCE,
	AT_SET_OPTION_TWO_HAND_SWORD_IMPROVE_DAMAGE,
	
	AT_SET_OPTION_IMPROVE_ATTACK_2,
	AT_SET_OPTION_IMPROVE_ATTACK_1,
	AT_SET_OPTION_IMPROVE_DEFENCE_3,
	AT_SET_OPTION_IMPROVE_DEFENCE_4,
	AT_SET_OPTION_IMPROVE_MAGIC,
	AT_SET_OPTION_ICE_MASTERY,
	AT_SET_OPTION_POSION_MASTERY,
	AT_SET_OPTION_THUNDER_MASTERY,
	AT_SET_OPTION_FIRE_MASTERY,
	AT_SET_OPTION_EARTH_MASTERY,
	AT_SET_OPTION_WIND_MASTERY,
	AT_SET_OPTION_WATER_MASTERY,

	AT_IMPROVE_MAX_MANA,
	AT_IMPROVE_MAX_AG,
};

enum eTypeSkill
{
	eTypeSkill_None = -1,
	eTypeSkill_CommonAttack,
	eTypeSkill_Buff,
	eTypeSkill_DeBuff,
	eTypeSkill_FrendlySkill,
	eTypeSkill_End,
};

enum eBuffClass
{
	eBuffClass_Buff = 0,
	eBuffClass_DeBuff,

	eBuffClass_Count,
};

enum eBuffState
{
	eBuffNone = 0,

	// Buff
	eBuff_Attack,
	eBuff_Defense,
	eBuff_HelpNpc,

	eBuff_PhysDefense,
	eBuff_AddCriticalDamage,
	eBuff_InfinityArrow,
	eBuff_AddAG,
	eBuff_HpRecovery,
	eBuff_AddMana,
	eBuff_BlessPotion,
	eBuff_SoulPotion,
	eBuff_RemovalMagic,
	eBuff_CastleGateIsOpen,
	eBuff_CastleRegimentDefense,
	eBuff_CastleRegimentAttack1,
	eBuff_CastleRegimentAttack2,
	eBuff_CastleRegimentAttack3,
	eBuff_Cloaking,
	eBuff_AddSkill,
	eBuff_CastleCrown,
	eBuff_CrywolfAltarEnable,
	eBuff_CrywolfAltarDisable,
	eBuff_CrywolfAltarContracted,
	eBuff_CrywolfAltarAttempt,
	eBuff_CrywolfAltarOccufied,
	eBuff_CrywolfHeroContracted,
	eBuff_CrywolfNPCHide,
	eBuff_GMEffect,
	eBuff_PcRoomSeal1,
	eBuff_PcRoomSeal2,
	eBuff_PcRoomSeal3,
	eBuff_CursedTempleQuickness,
	eBuff_CursedTempleSublimation,
	eBuff_CursedTempleProdection,
	eBuff_Hellowin1,
	eBuff_Hellowin2,
	eBuff_Hellowin3,
	eBuff_Hellowin4,
	eBuff_Hellowin5,
	eBuff_Seal1,
	eBuff_Seal2,
	eBuff_Seal3,
	eBuff_Seal4,
	eBuff_EliteScroll1,
	eBuff_EliteScroll2,
	eBuff_EliteScroll3,
	eBuff_EliteScroll4,
	eBuff_EliteScroll5,
	eBuff_EliteScroll6,
	eBuff_SecretPotion1,
	eBuff_SecretPotion2,
	eBuff_SecretPotion3,
	eBuff_SecretPotion4,
	eBuff_SecretPotion5,

	// DeBuff
	eDeBuff_Poison,
	eDeBuff_Freeze,
	eDeBuff_Harden,
	eDeBuff_Defense,
	eDeBuff_Attack,
	eDeBuff_MagicPower,
	eDeBuff_Stun,
	eDeBuff_InvincibleMagic,
	eDeBuff_InvincibleMagicAttack,
	eDeBuff_InvinciblePhysAttack,
	eDeBuff_CursedTempleRestraint,

	eBuff_CrywolfProdection1,
	eBuff_CrywolfProdection2,
	eBuff_CrywolfProdection3,
	eBuff_CrywolfProdection4,
	eBuff_CrywolfProdection5,

	eBuff_Thorns = 71,
	eDeBuff_Sleep = 72,
	eDeBuff_Blind = 73,
	eDeBuff_NeilDOT = 74,
	eDeBuff_SahamuttDOT = 75,
	eDeBuff_AttackDown = 76,
	eDeBuff_DefenseDown = 77,
	eBuff_CherryBlossom_Liguor,
	eBuff_CherryBlossom_RiceCake,
	eBuff_CherryBlossom_Petal,
	eBuff_Berserker = 81,
	eBuff_SwellOfMagicPower = 82,
	eDeBuff_FlameStrikeDamage = 83,
	eDeBuff_GiganticStormDamage = 84,
	eDeBuff_LightningShockDamage = 85,
	eDeBuff_BlowOfDestruction = 86,
	eBuff_Seal_HpRecovery = 87,
	eBuff_Seal_MpRecovery = 88,
	eBuff_Scroll_Battle = 89,
	eBuff_Scroll_Strengthen = 90,
	eBuff_BlessingOfXmax = 91,
	eBuff_CureOfSanta,
	eBuff_SafeGuardOfSanta,
	eBuff_StrengthOfSanta,
	eBuff_DefenseOfSanta,
	eBuff_QuickOfSanta,
	eBuff_LuckOfSanta,
	eBuff_DuelWatch = 98,
	eBuff_GuardCharm = 99,
	eBuff_ItemGuardCharm = 100,
	eBuff_AscensionSealMaster = 101,
	eBuff_WealthSealMaster = 102,
	eBuff_HonorOfGladiator = 103,
	eBuff_Doppelganger_Ascension = 105,
	eBuff_PartyExpBonus = 112,
	eBuff_AG_Addition = 113,
	eBuff_SD_Addition = 114,
	eBuff_NewWealthSeal = 119,
	eDeBuff_Discharge_Stamina = 120,
	eBuff_Scroll_Healing = 121,
	eBuff_Count,
};

enum eBuffTimeType
{
	eBuffTime_None = 0,
	eBuffTime_Hellowin = 1006,
	eBuffTime_PcRoomSeal,
	eBuffTime_Seal,
	eBuffTime_Scroll,
	eBuffTime_Secret,
	eBuffTime_CherryBlossom,
	eBuffTime_SwellOfMP,
	eBuffTime_Christmax,
	eBuffTime_HonorOfGladiator,
	eBuffTime_GuardCharm,
	eBuffTime_ItemGuardCharm,
	eBuffTime_AG_Addition,
	eBuffTime_SD_Addition,
	eBuffTime_PartyExpBonus,
	eBuffTime_Count,
};

enum eBuffValueLoadType
{
	eBuffValueLoad_None = 0,
	eBuffValueLoad_Skill,
	eBuffValueLoad_Item,
	eBuffValueLoad_Text,
	eBuffValueLoad_ItemAddOption,

	eBuffValueLoad_Count,
};

#define BUFFINDEX( buff )				static_cast<eBuffState>(buff)
#define BUFFTIMEINDEX( timetype )		static_cast<eBuffTimeType>(timetype)
#define ITEMINDEX( type, index )        static_cast<mu_uint32>((type*MAX_ITEM_INDEX)+index)

enum
{
	CHARACTER_NONE = 0,
	CHARACTER_RENDER_OBJ,
	CHARACTER_ANIMATION
};

enum  MATCH_TYPE
{
	TYPE_MATCH_NONE = 0,
	TYPE_MATCH_DEVIL_ENTER_START,
	TYPE_MATCH_DEVIL_ENTER_CLOSE,
	TYPE_MATCH_DEVIL_CLOSE,
	TYPE_MATCH_CASTLE_ENTER_CLOSE,
	TYPE_MATCH_CASTLE_INFILTRATION,
	TYPE_MATCH_CASTLE_CLOSE,
	TYPE_MATCH_CASTLE_END,
	TYPE_MATCH_CHAOS_ENTER_START = 11,
	TYPE_MATCH_CHAOS_EINFILTRATION,
	TYPE_MATCH_CHAOS_CLOSE,
	TYPE_MATCH_CHAOS_END,
	TYPE_MATCH_CURSEDTEMPLE_ENTER_CLOSE,
	TYPE_MATCH_CURSEDTEMPLE_GAME_START,
	TYPE_MATCH_DOPPELGANGER_ENTER_CLOSE,
	TYPE_MATCH_DOPPELGANGER_GAME_START,
	TYPE_MATCH_DOPPELGANGER_ICEWALKER,
	TYPE_MATCH_DOPPELGANGER_CLOSE,
	TYPE_MATCH_END
};

enum
{
	MATCH_TYPE_NONE_EVENT = 0,
	MATCH_TYPE_DEVIL_SQUARE,
	MATCH_TYPE_BLOOD_CASTLE,
	MATCH_TYPE_END
};

enum SKILL_TOOLTIP_RENDER_POINT
{
	STRP_NONE = 0,
	STRP_TOPLEFT,
	STRP_TOPCENTER,
	STRP_TOPRIGHT,
	STRP_LEFTCENTER,
	STRP_CENTER,
	STRP_RIGHTCENTER,
	STRP_BOTTOMLEFT,
	STRP_BOTTOMCENTER,
	STRP_BOOTOMRIGHT,

	STRP_ITEMINFO,
	STRP_ITEMINFO2,
};

#endif