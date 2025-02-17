#ifndef __SHARED_DEFINE_H__
#define __SHARED_DEFINE_H__

#pragma once

#define	MAX_BLOODCASTLE_BRIDGE_COUNT	8
#define MAX_BLOODCASTLE_USER_COUNT	10

#define MAX_DEVILSQUARE 7

#define WIDESIZE(UnicodeSize) (UnicodeSize * 4)

#define MAX_ZEN						2000000000
#define MAX_SELLZEN					400000000
#define MAX_TRADEZEN				99999999
#define MAX_LEVEL					400
#define MAX_MASTER_LEVEL			200
#define REPAIR_MINLEVEL				50
#define PSHOP_MINLEVEL				6

#define MAX_GROUP_COUNT 21
#define MAX_SERVER_COUNT 16
#define MAX_GROUP_NAME 32
#define MAX_SERVER_NAME 64
#define MAX_SERVERIP_LENGTH 128

#define BEGIN_CHARACTERLIST_COUNT (1)
#define MAX_CHARACTERLIST_COUNT (5)

#define MAX_CHAT_SIZE 90

#define MAX_GUILDMARKBUFFER		64
#define MAX_GUILD_COLORS (16)
#define MAX_GUILD_COLORS_HALF (MAX_GUILD_COLORS / 2)
#define MAX_GUILDMEMBER			80

#define REGEX_SYMBOLS "\\.\\,\\;\\:<>!@#$%^&*()_+\\[\\]{}\\\\/"

#define HASH_LENGTH 64 // SHA2-512(64 bytes)
#define ACCOUNT_MINIMUM 4
#define ACCOUNT_MAXIMUM 16
#define PASSWORD_MINIMUM 8
#define PASSWORD_MAXIMUM 32
#define ACCOUNT_REGEX _T("^[a-zA-Z0-9_\\-\\.@+]+$")
#define PASSWORD_REGEX _T("^(?=.*[a-z])(?=.*[A-Z])(?=.*\\d)(?=.*[" REGEX_SYMBOLS "])[A-Za-z\\d" REGEX_SYMBOLS "]+$")
#define DISABLE_PASSWORD_REGEX

#define CHARACTER_MINIMUM 4
#define CHARACTER_MAXIMUM 16
#define CHARACTER_REGEX _T("^[a-zA-Z0-9" REGEX_SYMBOLS "]+$")

#define STORAGE_MINIMUM 1
#define STORAGE_MAXIMUM 16
#define STORAGE_REGEX _T("^[a-zA-Z0-9" REGEX_SYMBOLS "]+$")

#define GUILDNAME_MINIMUM 4
#define GUILDNAME_MAXIMUM 16
#define GUILDNAME_REGEX _T("^[a-zA-Z0-9" REGEX_SYMBOLS "]+$")

#define GUILDNOTICE_MAXIMUM 64

#define EFFECT_POTION_HP			0x01
#define EFFECT_POTION_MP			0x02
#define EFFECT_LEVELUP				0x10

#define EFFECT_POTION_SD			0x03
#define EFFECT_SHIELDBROKE			0x11

#define CHAOS_MIX_LEVEL 10

#define NPC_ROLENSIAGUARD_MSGBOX	0
#define NPC_RUSIPHER_MSGBOX			2
#define NPC_FASI_MSGBOX				6
#define NPC_CHAOSGOBLELIN_MSGBOX	9

#define NPC_POSIONGIRL_MSGBOX		5
#define NPC_DEVIASWIZARD_MSGBOX		8

#define NPC_DEVIASGUARD_MSGBOX		4
#define NPC_DEVIASWAREHOUSEMEN_MSGBOX		7
#define NPC_NORIARARA_MSGBOX		11
#define NPC_DEVIASMADAM_MSGBOX		12

#define NPC_SHADOW_PHANTOM_MSGBOX   13

#define MSGBOX_ENTERLIMIT_PKUSER_DEVILSQURE		55
#define MSGBOX_ENTERLIMIT_PKUSER_BLOODCASTLE	56
#define MSGBOX_ENTERLIMIT_PKUSER_CHAOSCASTLE	57

#define MAX_PARTYS 5

#define MAX_LOTTERY_ITEM_WIDTH		2
#define	MAX_LOTTERY_ITEM_HEIGHT		4

namespace CS2CL
{
	namespace SERVER_TYPE
	{
		enum : mu_uint8
		{
			ST_NORMAL = 0,
			ST_GOLD = 1,
		};
	};

	namespace SERVER_FLAG
	{
		enum : mu_uint8
		{
			SF_NONPVP = (1 << 0),
		};
	};

	enum : mu_uint8
	{
		SERVER_OFFLINE = 128,
	};
};

namespace CS2GS
{
	namespace CONNECT_RESPONSE
	{
		enum : mu_uint8
		{
			CR_ALLOWED = 0,
			CR_BLOCKED = 1,
			CR_BANNED = 2,
			CR_FULL = 3,
		};
	};
};

namespace GS2CL
{
	namespace LOGIN_RESPONSE
	{
		enum : mu_uint8
		{
			LR_SUCCESS = 0,
			LR_WRONG_LOGIN = 1,
			LR_ALREADY_CONNECTED = 2,
			LR_WRONG_VERSION = 3,
			LR_UNEXPECTED_ERROR = 4,
			LR_ATTEMPT_FAILED = 5,
			LR_HELPER_DISCONNECTING = 6,
		};
	};

	namespace CREATECHARACTER_RESPONSE
	{
		enum : mu_uint8
		{
			CCR_SUCCESS = 0,
			CCR_WRONG_NAME = 1,
			CCR_NO_FREESLOT = 2,
			CCR_ALREADYEXISTS = 3,
			CCR_UNEXPECTED_ERROR = 4,
		};
	};

	namespace DELETECHARACTER_RESPONSE
	{
		enum : mu_uint8
		{
			DCR_SUCCESS = 0,
			DCR_FAILED = 1,
		};
	};
};

namespace GS2DS
{
	namespace CHARACTER_RESPONSE
	{
		enum : mu_uint8
		{
			CR_SUCCESS = 0,
			CR_UNEXPECTED_ERROR = 1,
		};
	};
};

namespace GAME
{
	enum
	{
		MAX_DBQUEST = 50,
	};

	enum
	{
		LIVECHECK_TIME = (30 * 1000),
	};

	enum
	{
		EQUIPMENT_WEAPON_RIGHT = 0,
		EQUIPMENT_WEAPON_LEFT = 1,
		EQUIPMENT_HELM = 2,
		EQUIPMENT_ARMOR = 3,
		EQUIPMENT_PANTS = 4,
		EQUIPMENT_GLOVES = 5,
		EQUIPMENT_BOOTS = 6,
		EQUIPMENT_WING = 7,
		EQUIPMENT_HELPER = 8,
		EQUIPMENT_AMULET = 9,
		EQUIPMENT_RING_RIGHT = 10,
		EQUIPMENT_RING_LEFT = 11,

		MAX_EQUIPMENT,
	};

	enum CLASS_TYPE
	{
		CLASS_WIZARD = 0,
		CLASS_KNIGHT,
		CLASS_ELF,
		CLASS_DARK,
		CLASS_DARK_LORD,
		CLASS_SUMMONER,
		MAX_CLASS,

		CLASS_SOULMASTER = MAX_CLASS,
		CLASS_BLADEKNIGHT,
		CLASS_MUSEELF,
		CLASS_BLOODYSUMMONER,

		CLASS_GRANDMASTER,
		CLASS_BLADEMASTER,
		CLASS_HIGHELF,
		CLASS_DUELMASTER,
		CLASS_LORDEMPEROR,
		CLASS_DIMENSIONMASTER,
	};

	enum CLASSES_MINIMUMLEVEL
	{
		CML_MAGIC_GLADIATOR = 220,
		CML_DARK_LORD = 250,
	};

	enum GUILD_STATUS
	{
		G_NONE = -1,
		G_PERSON = 0,
		G_MASTER = 128,
		G_SUB_MASTER = 64,
		G_BATTLE_MASTER = 32
	};

	enum GUILD_TYPE
	{
		G_TYPE_NONE = -1,
		G_TYPE_COMMON = 0,
		G_TYPE_GUARD = 1,
	};

	enum GUILD_RELATIONSHIP
	{
		GR_NONE = 0x00,
		GR_UNION = 0x01,
		GR_UNIONMASTER = 0x04,
		GR_RIVAL = 0x02,
		GR_RIVALUNION = 0x08,
	};

	enum STAT_ENUM
	{
		STAT_STRENGTH,
		STAT_DEXTERITY,
		STAT_VITALITY,
		STAT_ENERGY,
		STAT_COMMAND,
		STAT_MAX,
	};
};

#define STORAGE_COUNT 10

namespace INVENTORY_TYPE
{
	enum : mu_uint8
	{
		IT_PLAYER_INVENTORYTYPE_BEGIN,

		IT_EQUIPMENT = IT_PLAYER_INVENTORYTYPE_BEGIN,
		IT_INVENTORY = 1,
		IT_PERSONALSHOP = IT_INVENTORY + 30 - 1,

		IT_TRADE,
		IT_YOURTRADE,

		IT_MIX,
		IT_PETMIX,

		IT_HARMONY_PURITY,
		IT_HARMONY_SMELT,
		IT_HARMONY_RESTORE,

		IT_LOTTERY_MIX,

		IT_SOCKET_EXTRACT,
		IT_SOCKET_COMPOSIT,
		IT_SOCKET_EQUIP,
		IT_SOCKET_REMOVE,

		IT_CHERRYBLOSSOM,

		IT_PLAYER_INVENTORYTYPE_END = 80,

		IT_ACCOUNT_INVENTORYTYPE_BEGIN = 100,

		IT_STORAGE = IT_ACCOUNT_INVENTORYTYPE_BEGIN,
		IT_STORAGE_BEGIN = IT_STORAGE,
		IT_STORAGE_END = IT_STORAGE_BEGIN + STORAGE_COUNT - 1,

		IT_ACCOUNT_INVENTORYTYPE_END,

		IT_SHOP = 200,

		IT_MAPGROUND = 254,
		IT_FREESLOT = 255,
	};
};

// Quest
#define MAX_QUEST_MONSTER_KILL_INFO		5
#define MAX_QUEST_MONSTER_KILL_INFO_CL	( MAX_QUEST_MONSTER_KILL_INFO * 2 )
#define MAX_QUEST_MONSTER_KILL_COUNT	50

enum QuestEffectType
{
	QET_STATUP = 0,
	QET_CHANGEUP = 1,
	QET_PLUSSTAT = 2,
	QET_USINGCOMBOSKILL = 3,
	QET_THIRD_CHANGEUP = 4,
};

namespace ACTION_REQUEST
{
	enum
	{
		AR_NONE,
		AR_ANGLE,
		AR_MOVE,
		AR_TALK,
		AR_OPERATE,
		AR_FOLLOW,
		AR_ATTACK,
		AR_MAGIC,
		AR_PICKITEM,
	};
};

enum FRUIT_ITEM_TYPE
{
	FRUIT_ENERGY = 0,
	FRUIT_VITALITY = 1,
	FRUIT_DEXTERITY = 2,
	FRUIT_STRENGTH = 3,
	FRUIT_LEADERSHIP = 4
};

namespace EVENT_MASK
{
	enum
	{
		DRAGON_INVASION,
		GOLDEN_INVASION,
		DEATHKING_INVASION,
		MAX_COUNT,
	};
};

// Skills
#define MAX_MAGIC			60

#define MAX_BUFF_SLOT_INDEX		16

#include "shared_talk.h"
#include "shared_castlesiege.h"
#include "shared_mix.h"

#endif