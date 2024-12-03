#ifndef __MU_INTERFACE_LIST_H__
#define __MU_INTERFACE_LIST_H__

#pragma once

#define MESSAGE_UNKNOWN 0xFFFFFFFF

#define MAX_GUILD_COLORS (16)
#define MAX_GUILD_COLORS_HALF (MAX_GUILD_COLORS / 2)

#define MAX_RESOLUTION_RENDER (8)

namespace TEXTINDEX
{
	enum
	{
		// Add fixed text index here
		WARNING_FPSLIMIT,
		FPS_COUNTER,
		TEST_DEBUG,
		DEBUG_GAMEINFO,
		VERSION_INFO,
		LIFE_NUMBER,
		LIFE_TOOLTIP,
		MANA_NUMBER,
		MANA_TOOLTIP,
		SHIELD_NUMBER,
		SHIELD_TOOLTIP,
		AG_NUMBER,
		AG_TOOLTIP,
		EXPERIENCE_NUMBER,
		EXPERIENCE_TOOLTIP,
		HERO_POSITION,

		// MessageBox
		MESSAGEBOX_WAITINGLIST,
		MESSAGEBOX_DISCONNECTED,
		MESSAGEBOX_FAILEDCONNECT,
		MESSAGEBOX_SERVERISFULL,
		MESSAGEBOX_BANNEDFROMSERVER,
		MESSAGEBOX_BLOCKEDFROMSERVER,
		MESSAGEBOX_DELETECHARACTER,
		MESSAGEBOX_GUILDLEAVE,
		MESSAGEBOX_GUILDREQUEST,
		MESSAGEBOX_GUILDCANCELSTATUS,
		MESSAGEBOX_PARTYREQUEST,
		MESSAGEBOX_TRADEREQUEST,
		MESSAGEBOX_TRADEMONEY,
		MESSAGEBOX_TRADEACCEPT,
		MESSAGEBOX_TRADEACCEPT_BEGIN,
		MESSAGEBOX_TRADEACCEPT_END = MESSAGEBOX_TRADEACCEPT_BEGIN + 10 - 1,

		OKBOX_BEGIN,
		OKBOX_END = OKBOX_BEGIN + 20 - 1,

		// Options
		OPTIONWINDOW_TITLE,
		OPTIONWINDOW_WARNING,
		OPTIONWINDOW_AUTOATTACK_TITLE,
		OPTIONWINDOW_RESOLUTION_TITLE,
		OPTIONWINDOW_RESOLUTION_NAME_BEGIN,
		OPTIONWINDOW_RESOLUTION_NAME_END = OPTIONWINDOW_RESOLUTION_NAME_BEGIN + MAX_RESOLUTION_RENDER - 1,
		OPTIONWINDOW_ADVANCED_TITLE,
		OPTIONWINDOW_WINDOWMODE_TITLE,
		OPTIONWINDOW_ANTIALIASING_TITLE,
		OPTIONWINDOW_VERTICALSYNC_TITLE,
		OPTIONWINDOW_FPSLIMIT_TITLE,
		OPTIONWINDOW_SHADOWTYPE_TITLE,
		OPTIONWINDOW_MUSICVOLUME_TITLE,
		OPTIONWINDOW_MUSICVOLUME_LABEL,
		OPTIONWINDOW_SOUNDVOLUME_TITLE,
		OPTIONWINDOW_SOUNDVOLUME_LABEL,

		// Login
		LOGIN_SERVERNAME,
		LOGIN_ACCOUNTTIP,
		LOGIN_PASSWORDTIP,

		// Create Character
		CREATECHARACTER_STAT,
		CREATECHARACTER_STAT_END = CREATECHARACTER_STAT + 5 - 1,

		// Main Frame Window
		BEGIN_HOTKEY_ITEMNUMBER,
		END_HOTKEY_ITEMNUMBER = BEGIN_HOTKEY_ITEMNUMBER + 8 - 1,
		BEGIN_SKILL_HOTKEY_NUMBER,
		END_SKILL_HOTKEY_NUMBER = BEGIN_SKILL_HOTKEY_NUMBER + 10 - 1,

		// Character Info Window
		CHARACTERINFO_CHARNAME,
		CHARACTERINFO_CLASSNAME,
		CHARACTERINFO_SERVERNAME,
		CHARACTERINFO_LEVEL,
		CHARACTERINFO_STATPOINTS,
		CHARACTERINFO_EXPERIENCE,
		CHARACTERINFO_POINT,
		CHARACTERINFO_POINTPROB,

		CHARACTERINFO_STRENGTH_LABEL,
		CHARACTERINFO_STRENGTH,
		CHARACTERINFO_DEXTERITY_LABEL,
		CHARACTERINFO_DEXTERITY,
		CHARACTERINFO_VITALITY_LABEL,
		CHARACTERINFO_VITALITY,
		CHARACTERINFO_ENERGY_LABEL,
		CHARACTERINFO_ENERGY,
		CHARACTERINFO_COMMAND_LABEL,
		CHARACTERINFO_COMMAND,

		CHARACTERINFO_ATTACKDAMAGE,
		CHARACTERINFO_ATTACKRATE,
		CHARACTERINFO_BLOCKINGRATE,
		CHARACTERINFO_ATTACKSPEED,
		CHARACTERINFO_DEFENSERATE,
		CHARACTERINFO_LIFE,
		CHARACTERINFO_MANA,
		CHARACTERINFO_MAGICDAMAGE,
		CHARACTERINFO_MAGICDAMAGE2,
		CHARACTERINFO_CURSEDAMAGE,

		// My Inventory Window
		MYINVENTORY_HEADER,
		MYINVENTORY_GOLD,
		MYINVENTORY_PCROOM1,
		MYINVENTORY_PCROOM2,

		BEGIN_MYINVENTORY_ITEMNUMBER,
		END_MYINVENTORY_ITEMNUMBER = BEGIN_MYINVENTORY_ITEMNUMBER + 256 - 1,

		// Mix Inventory Window
		MIXINVENTORY_HEADER,
		MIXINVENTORY_HEADER2,
		MIXINVENTORY_INFO1,
		MIXINVENTORY_INFO2,

		BEGIN_MIXINVENTORY_DETAILS,
		END_MIXINVENTORY_DETAILS = BEGIN_MIXINVENTORY_DETAILS + 60 - 1,

		BEGIN_MIXINVENTORY_ITEMNUMBER,
		END_MIXINVENTORY_ITEMNUMBER = BEGIN_MIXINVENTORY_ITEMNUMBER + 256 - 1,

		// Mix Check Window
		MIXCHECK_BEGIN,
		MIXCHECK_END = MIXCHECK_BEGIN + 5 - 1,

		// Storage Window
		STORAGE_HEADER,
		STORAGE_GOLD,
		STORAGE_FEEHEADER,
		STORAGE_FEE,

		STORAGE_DEPOSITBOX,
		STORAGE_WITHDRAWBOX,
		STORAGE_LOCKBOX,

		BEGIN_STORAGE_ITEMNUMBER,
		END_STORAGE_ITEMNUMBER = BEGIN_STORAGE_ITEMNUMBER + 256 - 1,

		// Trade Window
		TRADE_HEADER,
		TRADE_MYNAME,
		TRADE_YOURNAME,
		TRADE_MYGOLD,
		TRADE_YOURGOLD,
		TRADE_YOURGUILD,
		TRADE_LEVEL,

		BEGIN_TRADE_EXTRA,
		END_TRADE_EXTRA = BEGIN_TRADE_EXTRA + 5 - 1,

		TRADE_WARNING_BEGIN,
		TRADE_WARNING_END = TRADE_WARNING_BEGIN + 128 - 1,

		// Shop Window
		SHOP_HEADER,
		SHOP_TAXHEADER,
		SHOP_GOLDHEADER,
		SHOP_GOLD,

		BEGIN_SHOP_ITEMNUMBER,
		END_SHOP_ITEMNUMBER = BEGIN_SHOP_ITEMNUMBER + 256 - 1,

		// Trade Window
		BEGIN_YOURTRADE_ITEMNUMBER,
		END_YOURTRADE_ITEMNUMBER = BEGIN_YOURTRADE_ITEMNUMBER + 256 - 1,

		BEGIN_MYTRADE_ITEMNUMBER,
		END_MYTRADE_ITEMNUMBER = BEGIN_MYTRADE_ITEMNUMBER + 256 - 1,

		// High Value Warning
		HIGHVALUE_LINE1,
		HIGHVALUE_LINE2,
		HIGHVALUE_LINE3,

		// Gamble Buy Warning
		GAMBLEBUY_LINE1,

		// PCPoint Buy Warning
		PCPOINTBUY_LINE1,

		SIMPLENAME_MONSTERNAME,
		SIMPLENAME_ITEMNAME,

		BEGIN_SERVERMESSAGES,
		END_SERVERMESSAGES = BEGIN_SERVERMESSAGES + 128 - 1,

		BEGIN_NOTICES,
		END_NOTICES = BEGIN_NOTICES + 128 - 1,

		BEGIN_CHATMESSAGES,
		END_CHATMESSAGES = BEGIN_CHATMESSAGES + 5500 - 1,

		BEGIN_CHARACTERNAME,
		END_CHARACTERNAME = BEGIN_CHARACTERNAME + MAX_CHARACTERS_CLIENT - 1,

		BEGIN_ITEMNAME,
		END_ITEMNAME = BEGIN_ITEMNAME + MAX_ITEMS - 1,

		BEGIN_RENDERTOOLTIP,
		END_RENDERTOOLTIP = BEGIN_RENDERTOOLTIP + TEXTLIST_COUNT - 1,

		BEGIN_POINTS,
		END_POINTS = BEGIN_POINTS + MAX_POINTS - 1,

		// Party Window
		PARTYWINDOW_TITLE,
		PARTYWINDOW_INFO,

		PARTYWINDOW_MEMBER_NAME_BEGIN,
		PARTYWINDOW_MEMBER_NAME_END = PARTYWINDOW_MEMBER_NAME_BEGIN + MAX_PARTYS - 1,
		PARTYWINDOW_MEMBER_MAPNAME_BEGIN,
		PARTYWINDOW_MEMBER_MAPNAME_END = PARTYWINDOW_MEMBER_MAPNAME_BEGIN + MAX_PARTYS - 1,
		PARTYWINDOW_MEMBER_COORDS_BEGIN,
		PARTYWINDOW_MEMBER_COORDS_END = PARTYWINDOW_MEMBER_COORDS_BEGIN + MAX_PARTYS - 1,
		PARTYWINDOW_MEMBER_LIFE_BEGIN,
		PARTYWINDOW_MEMBER_LIFE_END = PARTYWINDOW_MEMBER_COORDS_BEGIN + MAX_PARTYS - 1,

		// Party List
		PARTYLIST_MEMBERNAME_BEGIN,
		PARTYLIST_MEMBERNAME_END = PARTYLIST_MEMBERNAME_BEGIN + MAX_PARTYS - 1,
		
		// Party Member HP
		SIMPLEHPBAR_LIFE_BEGIN,
		SIMPLEHPBAR_LIFE_END = SIMPLEHPBAR_LIFE_BEGIN + MAX_PARTYS - 1,

		// Guild Window
		GUILDWINDOW_TITLE,
		GUILDWINDOW_INFO,
		GUILDWINDOW_SCORE,
		GUILDWINDOW_MISC_BEGIN,
		GUILDWINDOW_MISC_END = GUILDWINDOW_MISC_BEGIN + 24 - 1,
		GUILDWINDOW_MEMBER_NAME_BEGIN,
		GUILDWINDOW_MEMBER_NAME_END = GUILDWINDOW_MEMBER_NAME_BEGIN + 20 - 1,
		GUILDWINDOW_MEMBER_POSITION_BEGIN,
		GUILDWINDOW_MEMBER_POSITION_END = GUILDWINDOW_MEMBER_POSITION_BEGIN + 20 - 1,
		GUILDWINDOW_MEMBER_SERVER_BEGIN,
		GUILDWINDOW_MEMBER_SERVER_END = GUILDWINDOW_MEMBER_SERVER_BEGIN + 20 - 1,
		GUILDWINDOW_ALLIANCE_NAME_BEGIN,
		GUILDWINDOW_ALLIANCE_NAME_END = GUILDWINDOW_ALLIANCE_NAME_BEGIN + 8 - 1,
		GUILDWINDOW_ALLIANCE_MEMBERCOUNT_BEGIN,
		GUILDWINDOW_ALLIANCE_POSITION_END = GUILDWINDOW_ALLIANCE_MEMBERCOUNT_BEGIN + 8 - 1,

		// Guild Create Window
		GUILDCREATE_TITLE,
		GUILDCREATE_MAKETITLE,
		GUILDCREATE_MARKTITLE,
		GUILDCREATE_MARKINFO,
		GUILDCREATE_RESULTTITLE,

		// Chaos Mix Menu
		CHAOSMIXMENU_BEGIN,
		CHAOSMIXMENU_END = CHAOSMIXMENU_BEGIN + 50 - 1,

		// Map Window
		MAPWINDOW_TITLE,
		MAPWINDOW_BATTLEZONE_COLUMN,
		MAPWINDOW_BATTLEZONE_BEGIN,
		MAPWINDOW_BATTLEZONE_END = MAPWINDOW_BATTLEZONE_BEGIN + 500 - 1,
		MAPWINDOW_MAPNAME_COLUMN,
		MAPWINDOW_MAPNAME_BEGIN,
		MAPWINDOW_MAPNAME_END = MAPWINDOW_MAPNAME_BEGIN + 500 - 1,
		MAPWINDOW_LEVEL_COLUMN,
		MAPWINDOW_LEVEL_BEGIN,
		MAPWINDOW_LEVEL_END = MAPWINDOW_LEVEL_BEGIN + 500 - 1,
		MAPWINDOW_COST_COLUMN,
		MAPWINDOW_COST_BEGIN,
		MAPWINDOW_COST_END = MAPWINDOW_COST_BEGIN + 500 - 1,

		// Quest Window
		QUESTWINDOW_TITLE,
		QUESTWINDOW_CHANGECLASS_TITLE,
		QUESTWINDOW_CASTLEINFO1,
		QUESTWINDOW_CASTLEINFO2,
		QUESTWINDOW_CASTLEINFO3,
		QUESTWINDOW_TEMPLEINFO1,
		QUESTWINDOW_TEMPLEINFO2,
		QUESTWINDOW_TEMPLEINFO3,

		// Npc Quest Window
		NPCQUESTWINDOW_TITLE,
		NPCQUESTWINDOW_NAME,
		NPCQUESTWINDOW_COST,
		NPCQUESTWINDOW_REQUIREDZEN,
		NPCQUESTWINDOW_ITEMNAME_BEGIN,
		NPCQUESTWINDOW_ITEMNAME_END = NPCQUESTWINDOW_ITEMNAME_BEGIN + 32 - 1,

		// Blood Castle Timer
		BCT_COUNTDOWN,
		BCT_COUNT,
		BCT_TITLE,
		BCT_TIME,

		// Blood Castle Rank
		BCR_MESSAGE,
		BCR_MESSAGE2,
		BCR_EXPREWARD,
		BCR_ZENREWARD,
		BCR_SCOREREWARD,

		// Blood Castle Enter
		BCE_TITLE,
		BCE_LINES_BEGIN,
		BCE_LINES_END = BCE_LINES_BEGIN + 10 - 1,

		// Command
		COMMANDWINDOW_TITLE,

		// Helper Setup
		HELPERSETUP_TITLE,
		// Hunting Tab
		HELPERSETUP_HTRANGE_LABEL,
		HELPERSETUP_HTPOTION_LABEL,
		HELPERSETUP_HTAUTOHEAL_LABEL,
		HELPERSETUP_HTPARTY_LABEL,
		HELPERSETUP_HTREGULARATTACK_LABEL,
		HELPERSETUP_HTSKILLSCLOSELY_LABEL,
		HELPERSETUP_HTLONGDIST_LABEL,
		HELPERSETUP_HTORIGPOS_LABEL,
		HELPERSETUP_HTRETURNTIME_LABEL1,
		HELPERSETUP_HTRETURNTIME_LABEL2,
		HELPERSETUP_HTBASICSKILL_LABEL,
		HELPERSETUP_HTACTSKILL1_LABEL,
		HELPERSETUP_HTACTSKILL1_DELAY_LABEL,
		HELPERSETUP_HTACTSKILL1_DELAYTIME_LABEL,
		HELPERSETUP_HTACTSKILL1_CON_LABEL,
		HELPERSETUP_HTACTSKILL2_LABEL,
		HELPERSETUP_HTACTSKILL2_DELAY_LABEL,
		HELPERSETUP_HTACTSKILL2_DELAYTIME_LABEL,
		HELPERSETUP_HTACTSKILL2_CON_LABEL,
		HELPERSETUP_HTCOMBO_LABEL,
		HELPERSETUP_HTBUFFDUR_LABEL,
		// Obtaining Tab
		HELPERSETUP_OBREPAIR_LABEL,
		HELPERSETUP_OBPICKALL_LABEL,
		HELPERSETUP_OBPICKSEL_LABEL,
		HELPERSETUP_OBJEWEL_LABEL,
		HELPERSETUP_OBSETITEM_LABEL,
		HELPERSETUP_OBZEN_LABEL,
		HELPERSETUP_OBEXC_LABEL,
		HELPERSETUP_OBEXTRA_LABEL,

		// Helper Skill Setting
		HELPERSKILL_PRECON_TITLE,
		HELPERSKILL_HTRANGE_LABEL,
		HELPERSKILL_HTATTACKER_LABEL,
		HELPERSKILL_SUBCON_TITLE,
		HELPERSKILL_TWOPLUS_LABEL,
		HELPERSKILL_THREEPLUS_LABEL,
		HELPERSKILL_FOURPLUS_LABEL,
		HELPERSKILL_FIVEPLUS_LABEL,

		// Helper Potion Setting
		HELPERPOTION_MAIN_TITLE,
		HELPERPOTION_MINLIFE_LABEL,
		HELPERPOTION_MINLIFE_LABEL2,
		HELPERPOTION_AUTOHEAL_TITLE,
		HELPERPOTION_MINAUTOHEAL_LABEL,
		HELPERPOTION_MINAUTOHEAL_LABEL2,

		// Helper Party Setting
		HELPERPARTY_HEAL_TITLE,
		HELPERPARTY_ENABLEHEAL_LABEL,
		HELPERPARTY_MINHEAL_LABEL,
		HELPERPARTY_MINHEAL_LABEL2,
		HELPERPARTY_BUFF_TITLE,
		HELPERPARTY_ENABLEBUFF_LABEL,
		HELPERPARTY_BUFFINTERVAL_LABEL1,
		HELPERPARTY_BUFFINTERVAL_LABEL2,

		// Helper Skill Select
		HELPERSKILLSELECT_TITLE,

		// Helper Item Add
		HELPERITEMADD_MAIN_TITLE,
		HELPERITEMADD_TYPENAME_BEGIN,
		HELPERITEMADD_TYPENAME_END = HELPERITEMADD_TYPENAME_BEGIN + 32 - 1,
		HELPERITEMADD_ITEMNAME_BEGIN,
		HELPERITEMADD_ITEMNAME_END = HELPERITEMADD_ITEMNAME_BEGIN + 32 - 1,
		HELPERITEMADD_MINLEVEL,
		HELPERITEMADD_LEVEL_LABEL,
		HELPERITEMADD_LUCK_LABEL,
		HELPERITEMADD_SKILL_LABEL,
		HELPERITEMADD_OPTION_LABEL,
		HELPERITEMADD_EXC_LABEL,
		HELPERITEMADD_SETITEM_LABEL,
		HELPERITEMADD_SOCKET_LABEL,

		BEGIN_RANDOM,
		END_RANDOM = BEGIN_RANDOM + 5000 - 1,
	};
};

extern mu_atomic_uint32_t g_RandomTextIndex;

namespace MESSAGEBOX
{
	enum Interfaces
	{
		INTERFACE_DELETECHARACTER,

		INTERFACE_FAILEDCONNECT,
		INTERFACE_SERVERISFULL,
		INTERFACE_BANNEDFROMSERVER,
		INTERFACE_BLOCKEDFROMSERVER,
		INTERFACE_EXITGAME,
		INTERFACE_CHANGESERVER,
		INTERFACE_CHANGECHARACTER,
		INTERFACE_GUILDLEAVE,
		INTERFACE_GUILDREQUEST,
		INTERFACE_GUILDCANCELSTATUS,
		INTERFACE_GUILDGIVESTATUS,
		INTERFACE_PARTYREQUEST,
		INTERFACE_TRADEREQUEST,
		INTERFACE_TRADEMONEY,
		INTERFACE_TRADEACCEPT,
		INTERFACE_STORAGEDEPOSITBOX,
		INTERFACE_STORAGEWITHDRAWBOX,
		INTERFACE_STORAGELOCKBOX,
		INTERFACE_HIGHVALUEWARNINGBOX,
		INTERFACE_GAMBLEBUY,
		INTERFACE_PCPOINTBUY,
		INTERFACE_CHAOSMIXMENU,
		INTERFACE_MIXCHECK,
		INTERFACE_BLOODCASTLE_RANK,
		INTERFACE_HELPER_SKILLSETTING,
		INTERFACE_HELPER_POTIONSETTING,
		INTERFACE_HELPER_ELFPOTIONSETTING,
		INTERFACE_HELPER_PARTYSETTING,
		INTERFACE_HELPER_WIZARDPARTYSETTING,
		INTERFACE_HELPER_SKILLSELECT,
		INTERFACE_HELPER_ITEMADD,
		INTERFACE_HELPER_START,
		INTERFACE_WAITINGBOX,
		INTERFACE_CONNECT,
		INTERFACE_OKBOX,
		INTERFACE_DISCONNECTED,
	};

	enum Messages : mu_uint32
	{
		MESSAGE_OKBOX_OKBUTTON,
		MESSAGE_FAILEDCONNECT_OKBUTTON,
		MESSAGE_SERVERISFULL_OKBUTTON,
		MESSAGE_BANNEDFROMSERVER_OKBUTTON,
		MESSAGE_BLOCKEDFROMSERVER_OKBUTTON,

		MESSAGE_DELETECHARACTER_ACCEPTBUTTON,
		MESSAGE_DELETECHARACTER_CANCELBUTTON,

		MESSAGE_DISCONNECTED_OKBUTTON,

		MESSAGE_GUILDLEAVE_ACCEPTBUTTON,
		MESSAGE_GUILDLEAVE_CANCELBUTTON,
		MESSAGE_GUILDREQUEST_ACCEPTBUTTON,
		MESSAGE_GUILDREQUEST_CANCELBUTTON,
		MESSAGE_GUILDCANCELSTATUS_ACCEPTBUTTON,
		MESSAGE_GUILDCANCELSTATUS_CANCELBUTTON,
		MESSAGE_GUILDGIVESTATUS_BATTLEMASTER_BUTTON,
		MESSAGE_GUILDGIVESTATUS_SUBMASTER_BUTTON,
		MESSAGE_GUILDGIVESTATUS_CANCELBUTTON,

		MESSAGE_PARTYREQUEST_ACCEPTBUTTON,
		MESSAGE_PARTYREQUEST_CANCELBUTTON,

		MESSAGE_TRADEREQUEST_ACCEPTBUTTON,
		MESSAGE_TRADEREQUEST_CANCELBUTTON,

		MESSAGE_TRADEMONEY_GOLDINPUT,
		MESSAGE_TRADEMONEY_ACCEPTBUTTON,
		MESSAGE_TRADEMONEY_CANCELBUTTON,

		MESSAGE_TRADEACCEPT_ACCEPTBUTTON,
		MESSAGE_TRADEACCEPT_CANCELBUTTON,

		MESSAGE_STORAGEDEPOSIT_GOLDINPUT,
		MESSAGE_STORAGEDEPOSIT_ACCEPTBUTTON,
		MESSAGE_STORAGEDEPOSIT_CANCELBUTTON,

		MESSAGE_STORAGEWITHDRAW_GOLDINPUT,
		MESSAGE_STORAGEWITHDRAW_ACCEPTBUTTON,
		MESSAGE_STORAGEWITHDRAW_CANCELBUTTON,

		MESSAGE_STORAGELOCK_PASSWORDINPUT,
		MESSAGE_STORAGELOCK_ACCEPTBUTTON,
		MESSAGE_STORAGELOCK_CANCELBUTTON,

		MESSAGE_HIGHVALUE_ACCEPTBUTTON,
		MESSAGE_HIGHVALUE_CANCELBUTTON,

		MESSAGE_GAMBLEBUY_ACCEPTBUTTON,
		MESSAGE_GAMBLEBUY_CANCELBUTTON,

		MESSAGE_PCPOINTBUY_ACCEPTBUTTON,
		MESSAGE_PCPOINTBUY_CANCELBUTTON,

		MESSAGE_MIXMENU_GENERALBUTTON,
		MESSAGE_MIXMENU_CHAOSBUTTON,
		MESSAGE_MIXMENU_380BUTTON,
		MESSAGE_MIXMENU_CANCELBUTTON,

		MESSAGE_MIXCHECK_ACCEPTBUTTON,
		MESSAGE_MIXCHECK_CANCELBUTTON,

		MESSAGE_BCRANK_OKBUTTON,

		MESSAGE_HELPERSKILL_RANGE_BUTTON,
		MESSAGE_HELPERSKILL_ATTACKER_BUTTON,
		MESSAGE_HELPERSKILL_TWOPLUS_BUTTON,
		MESSAGE_HELPERSKILL_THREEPLUS_BUTTON,
		MESSAGE_HELPERSKILL_FOURPLUS_BUTTON,
		MESSAGE_HELPERSKILL_FIVEPLUS_BUTTON,
		MESSAGE_HELPERSKILL_OKBUTTON,

		MESSAGE_HELPERPOTION_MINLIFE_SLIDER,
		MESSAGE_HELPERPOTION_MINAUTOHEAL_SLIDER,
		MESSAGE_HELPERPOTION_OKBUTTON,
		MESSAGE_HELPERELFPOTION_OKBUTTON,

		MESSAGE_HELPERPARTY_ENABLEHEAL_BUTTON,
		MESSAGE_HELPERPARTY_MINHEAL_SLIDER,
		MESSAGE_HELPERPARTY_ENABLEBUFF_BUTTON,
		MESSAGE_HELPERPARTY_BUFFINTERVAL_EDITBOX,
		MESSAGE_HELPERPARTY_OKBUTTON,

		MESSAGE_HELPERSKILLSELECT_LEFT_BUTTON,
		MESSAGE_HELPERSKILLSELECT_RIGHT_BUTTON,
		MESSAGE_HELPERSKILLSELECT_SKILL_BEGIN,
		MESSAGE_HELPERSKILLSELECT_SKILL_END = MESSAGE_HELPERSKILLSELECT_SKILL_BEGIN + 8 - 1,
		MESSAGE_HELPERSKILLSELECT_OKBUTTON,

		MESSAGE_HELPERITEMADD_TYPE_SCROLL,
		MESSAGE_HELPERITEMADD_ITEM_SCROLL,
		MESSAGE_HELPERITEMADD_TYPE_BEGIN,
		MESSAGE_HELPERITEMADD_TYPE_END = MESSAGE_HELPERITEMADD_TYPE_BEGIN + 10 - 1,
		MESSAGE_HELPERITEMADD_ITEM_BEGIN,
		MESSAGE_HELPERITEMADD_ITEM_END = MESSAGE_HELPERITEMADD_ITEM_BEGIN + 10 -1,
		MESSAGE_HELPERITEMADD_DECLEVEL_BUTTON,
		MESSAGE_HELPERITEMADD_INCLEVEL_BUTTON,
		MESSAGE_HELPERITEMADD_LUCK_BUTTON,
		MESSAGE_HELPERITEMADD_SKILL_BUTTON,
		MESSAGE_HELPERITEMADD_OPTION_BUTTON,
		MESSAGE_HELPERITEMADD_EXC_BUTTON,
		MESSAGE_HELPERITEMADD_SETITEM_BUTTON,
		MESSAGE_HELPERITEMADD_SOCKET_BUTTON,
		MESSAGE_HELPERITEMADD_ACCEPT_BUTTON,
		MESSAGE_HELPERITEMADD_CANCEL_BUTTON,

		MESSAGE_HELPERSTART_ONLINE_BUTTON,
		MESSAGE_HELPERSTART_OFFLINE_BUTTON,
		MESSAGE_HELPERSTART_CANCEL_BUTTON,
	};
};

namespace OPTIONUI
{
	enum Interfaces
	{
		INTERFACE_LOGINMENU,
		INTERFACE_CHARACTERMENU,
		INTERFACE_GAMEMENU,

		INTERFACE_OPTION,
	};

	enum Messages : mu_uint32
	{
		// Login Menu
		MESSAGE_LOGIN_EXITBUTTON,
		MESSAGE_LOGIN_OPTIONBUTTON,
		MESSAGE_LOGIN_CLOSEBUTTON,

		// Character Menu
		MESSAGE_CHARACTER_EXITBUTTON,
		MESSAGE_CHARACTER_SERVERBUTTON,
		MESSAGE_CHARACTER_OPTIONBUTTON,
		MESSAGE_CHARACTER_CLOSEBUTTON,

		// Game Menu
		MESSAGE_GAME_EXITBUTTON,
		MESSAGE_GAME_SERVERBUTTON,
		MESSAGE_GAME_CHARACTERBUTTON,
		MESSAGE_GAME_OPTIONBUTTON,
		MESSAGE_GAME_CLOSEBUTTON,

		// Option Window
		MESSAGE_OPTION_TAB_BEGIN,
		MESSAGE_OPTION_TAB_END = MESSAGE_OPTION_TAB_BEGIN + 3 - 1,
		MESSAGE_OPTION_AUTOATTACK_BUTTON,
		MESSAGE_OPTION_RESOLUTION_SCROLLBAR,
		MESSAGE_OPTION_RESOLUTION_BEGIN,
		MESSAGE_OPTION_RESOLUTION_END = MESSAGE_OPTION_RESOLUTION_BEGIN + MAX_RESOLUTION_RENDER - 1,
		MESSAGE_OPTION_WINDOWMODE_BUTTON,
		MESSAGE_OPTION_ANTIALIASING_BUTTON,
		MESSAGE_OPTION_VERTICALSYNC_BUTTON,
		MESSAGE_OPTION_FPSLIMIT_BUTTON,
		MESSAGE_OPTION_SHADOWTYPE_BUTTON,
		MESSAGE_OPTION_MUSICVOLUME_SLIDER,
		MESSAGE_OPTION_SOUNDVOLUME_SLIDER,
		MESSAGE_OPTION_APPLY,
		MESSAGE_OPTION_CLOSE,
	};
};

namespace LOGINUI
{
	enum Interfaces
	{
		INTERFACE_LOGO,
		INTERFACE_LOGINFRAME,
		INTERFACE_SERVERLIST,
		INTERFACE_LOGINWINDOW,
		INTERFACE_SERVERMESSAGE,
	};

	enum Messages : mu_uint32
	{
		// Login Frame
		MESSAGE_MENU_BUTTON,
		MESSAGE_WEBSITE_BUTTON,
		MESSAGE_FORUM_BUTTON,
		MESSAGE_FACEBOOK_BUTTON,

		// Server List
		MESSAGE_SERVERGROUP_BUTTON,
		MESSAGE_SERVERGROUP_BUTTON_END = MESSAGE_SERVERGROUP_BUTTON + SERVERLIST::MAX_SERVER_GROUP,
		MESSAGE_SERVER_BUTTON,
		MESSAGE_SERVER_BUTTON_END = MESSAGE_SERVER_BUTTON + SERVERLIST::MAX_SERVER,

		// Login Window
		MESSAGE_LOGIN_ACCOUNTBOX,
		MESSAGE_LOGIN_PASSWORDBOX,
		MESSAGE_LOGIN_OKBUTTON,
		MESSAGE_LOGIN_CANCELBUTTON,
	};
};

namespace CHARACTERUI
{
	enum Interfaces
	{
		INTERFACE_FRAME,
		INTERFACE_CREATECHARACTER,
		INTERFACE_SERVERMESSAGE,
	};

	enum Messages : mu_uint32
	{
		MESSAGE_CREATE_BUTTON,
		MESSAGE_MENU_BUTTON,
		MESSAGE_CONNECT_BUTTON,
		MESSAGE_DELETE_BUTTON,

		MESSAGE_CREATECHARACTER_OKBUTTON,
		MESSAGE_CREATECHARACTER_CANCELBUTTON,
		MESSAGE_CREATECHARACTER_NAMEINPUT,

		MESSAGE_CREATECHARACTER_CLASS,
		MESSAGE_CREATECHARACTER_CLASS_END = MESSAGE_CREATECHARACTER_CLASS + GAME::MAX_CLASS - 1,
	};
}

namespace GAMEUI
{
	enum Interfaces
	{
		INTERFACE_SIMPLEHPBAR,
		INTERFACE_SIMPLECHAT,
		INTERFACE_SIMPLENAME,
		INTERFACE_FRIEND,
		INTERFACE_PARTY,
		INTERFACE_PARTYLIST,
		INTERFACE_MYQUEST,
		INTERFACE_NPCQUEST,
		INTERFACE_GUILDINFO,
		INTERFACE_TRADE,
		INTERFACE_STORAGE,
		INTERFACE_MIXINVENTORY,
		INTERFACE_COMMAND,
		INTERFACE_HELPERSETUP,
		INTERFACE_PET,
		INTERFACE_NPCSHOP,
		INTERFACE_INVENTORY,
		INTERFACE_MYSHOP_INVENTORY,
		INTERFACE_PURCHASESHOP_INVENTORY,
		INTERFACE_CHARACTER,
		INTERFACE_NPCBREEDER,
		INTERFACE_SERVERDIVISION,
		INTERFACE_DEVILSQUARE,
		INTERFACE_BLOODCASTLE,

		INTERFACE_NPCGUILDMASTER,
		INTERFACE_GUARDSMAN,
		INTERFACE_SENATUS,
		INTERFACE_GATEKEEPER,
		INTERFACE_GATESWITCH,
		INTERFACE_CATAPULT,
		INTERFACE_REFINERY,
		INTERFACE_REFINERYINFO,
		INTERFACE_KANTURU2ND_ENTERNPC,
		INTERFACE_CURSEDTEMPLE_NPC,
		INTERFACE_CURSEDTEMPLE_GAMESYSTEM,
		INTERFACE_CURSEDTEMPLE_RESULT,
		INTERFACE_CHATLOGWINDOW,
		INTERFACE_CHATINPUTBOX,
		INTERFACE_WINDOW_MENU,
		INTERFACE_OPTION,
		INTERFACE_HELP,
		INTERFACE_ITEM_EXPLANATION,
		INTERFACE_SETITEM_EXPLANATION,
		INTERFACE_QUICK_COMMAND,

		INTERFACE_KANTURU_INFO,
		INTERFACE_PARTY_INFO_WINDOW,
		INTERFACE_BLOODCASTLE_TIME,
		INTERFACE_CHAOSCASTLE_TIME,
		INTERFACE_BATTLE_SOCCER_SCORE,
		INTERFACE_SLIDEWINDOW,
		INTERFACE_HERO_POSITION_INFO,
		INTERFACE_MOVEMAP,
		INTERFACE_MESSAGEBOX,
		INTERFACE_DUEL_WINDOW,
		INTERFACE_CRYWOLF,
		INTERFACE_NAME_WINDOW,
		INTERFACE_SIEGEWARFARE,
		INTERFACE_MAINFRAME,
		INTERFACE_ITEMHOYKEY,
		INTERFACE_SKILL_LIST,
		INTERFACE_ITEM_ENDURANCE_INFO,
		INTERFACE_BUFF_WINDOW,
		INTERFACE_PARTCHARGE_SHOP,
		INTERFACE_MASTER_LEVEL,
		INTERFACE_GOLD_BOWMAN,
		INTERFACE_GOLD_BOWMAN_LENA,
		INTERFACE_LUCKYCOIN_REGISTRATION,
		INTERFACE_EXCHANGE_LUCKYCOIN,
		INTERFACE_DUELWATCH,
		INTERFACE_DUELWATCH_MAINFRAME,
		INTERFACE_DUELWATCH_USERLIST,
		INTERFACE_DOPPELGANGER_NPC,
		INTERFACE_DOPPELGANGER_FRAME,
		INTERFACE_QUEST_PROGRESS,
		INTERFACE_QUEST_PROGRESS_ETC,
		INTERFACE_EMPIREGUARDIAN_NPC,
		INTERFACE_EMPIREGUARDIAN_TIMER,
		INTERFACE_MINI_MAP,
		INTERFACE_NPC_DIALOGUE,
		INTERFACE_GENSRANKING,
		INTERFACE_UNITEDMARKETPLACE_NPC_JULIA,

		INTERFACE_HOTKEY,
		INTERFACE_3DRENDERING_CAMERA_BEGIN,
		INTERFACE_3DRENDERING_CAMERA_END = INTERFACE_3DRENDERING_CAMERA_BEGIN + 24,
		INTERFACE_ITEM_TOOLTIP,
		INTERFACE_MAPNAME,
		INTERFACE_SIMPLENOTICE,
		INTERFACE_END,
		INTERFACE_COUNT = INTERFACE_END,
	};

	enum Messages : mu_uint32
	{
		// Main frame
		MESSAGE_CHARACTERINFO_BUTTON,
		MESSAGE_MYINVENTORY_BUTTON,
		MESSAGE_FRIENDLIST_BUTTON,
		MESSAGE_WINDOW_BUTTON,
		MESSAGE_CASHSHOP_BUTTON,

		// Helper
		MESSAGE_HELPER_CONFIGURE_BUTTON,
		MESSAGE_HELPER_START_BUTTON,

		// Helper Setup
		MESSAGE_HELPERSETUP_CLOSE_BUTTON,
		MESSAGE_HELPERSETUP_RESET_BUTTON,
		MESSAGE_HELPERSETUP_SAVE_BUTTON,
		// Hunting Tab
		MESSAGE_HELPERSETUP_HTRANGEPLUS_BUTTON,
		MESSAGE_HELPERSETUP_HTRANGEMINUS_BUTTON,
		MESSAGE_HELPERSETUP_HTPOTION_BUTTON,
		MESSAGE_HELPERSETUP_HTPOTION_SETTING_BUTTON,
		MESSAGE_HELPERSETUP_HTAUTOHEAL_BUTTON,
		MESSAGE_HELPERSETUP_HTPARTY_BUTTON,
		MESSAGE_HELPERSETUP_HTPARTY_SETTING_BUTTON,
		MESSAGE_HELPERSETUP_HTREGULARATTACK_BUTTON,
		MESSAGE_HELPERSETUP_HTSKILLSCLOSELY_BUTTON,
		MESSAGE_HELPERSETUP_HTLONGDIST_BUTTON,
		MESSAGE_HELPERSETUP_HTORIGPOS_BUTTON,
		MESSAGE_HELPERSETUP_HTRETURNTIME_EDITBOX,
		MESSAGE_HELPERSETUP_HTSKILLBTN_BEGIN,
		MESSAGE_HELPERSETUP_HTSKILLBTN_END = MESSAGE_HELPERSETUP_HTSKILLBTN_BEGIN + 6 - 1,
		MESSAGE_HELPERSETUP_HTACTSKILL1_DELAY_BUTTON,
		MESSAGE_HELPERSETUP_HTACTSKILL1_DELAYTIME_EDITBOX,
		MESSAGE_HELPERSETUP_HTACTSKILL1_CON_BUTTON,
		MESSAGE_HELPERSETUP_HTACTSKILL1_CON_SETTING_BUTTON,
		MESSAGE_HELPERSETUP_HTACTSKILL2_DELAY_BUTTON,
		MESSAGE_HELPERSETUP_HTACTSKILL2_DELAYTIME_EDITBOX,
		MESSAGE_HELPERSETUP_HTACTSKILL2_CON_BUTTON,
		MESSAGE_HELPERSETUP_HTACTSKILL2_CON_SETTING_BUTTON,
		MESSAGE_HELPERSETUP_HTCOMBO_BUTTON,
		MESSAGE_HELPERSETUP_HTBUFFDUR_BUTTON,
		// Obtaining Tab
		MESSAGE_HELPERSETUP_OBRANGEPLUS_BUTTON,
		MESSAGE_HELPERSETUP_OBRANGEMINUS_BUTTON,
		MESSAGE_HELPERSETUP_OBREPAIR_BUTTON,
		MESSAGE_HELPERSETUP_OBPICKALL_BUTTON,
		MESSAGE_HELPERSETUP_OBPICKSEL_BUTTON,
		MESSAGE_HELPERSETUP_OBJEWEL_BUTTON,
		MESSAGE_HELPERSETUP_OBSETITEM_BUTTON,
		MESSAGE_HELPERSETUP_OBZEN_BUTTON,
		MESSAGE_HELPERSETUP_OBEXC_BUTTON,
		MESSAGE_HELPERSETUP_OBEXTRA_BUTTON,
		MESSAGE_HELPERSETUP_OBITEMS_SCROLLBAR,
		MESSAGE_HELPERSETUP_OBITEMS_BEGIN,
		MESSAGE_HELPERSETUP_OBITEMS_END = MESSAGE_HELPERSETUP_OBITEMS_BEGIN + 16 - 1,
		MESSAGE_HELPERSETUP_OBITEMADD_BUTTON,
		MESSAGE_HELPERSETUP_OBITEMMOD_BUTTON,
		MESSAGE_HELPERSETUP_OBITEMDEL_BUTTON,

		// Item Hotkey
		MESSAGE_ITEMHOTKEY_BEGIN,
		MESSAGE_ITEMHOTKEY_END = MESSAGE_ITEMHOTKEY_BEGIN + 4 - 1,

		// Character frame
		MESSAGE_CHARACTERINFO_STRENGTH_BUTTON,
		MESSAGE_CHARACTERINFO_DEXTERITY_BUTTON,
		MESSAGE_CHARACTERINFO_VITALITY_BUTTON,
		MESSAGE_CHARACTERINFO_ENERGY_BUTTON,
		MESSAGE_CHARACTERINFO_COMMAND_BUTTON,
		MESSAGE_CHARACTERINFO_CLOSE_BUTTON,
		MESSAGE_CHARACTERINFO_QUEST_BUTTON,
		MESSAGE_CHARACTERINFO_PET_BUTTON,
		MESSAGE_CHARACTERINFO_MASTERLEVEL_BUTTON,

		// Quest Window
		MESSAGE_QUESTWINDOW_CLOSE_BUTTON,
		MESSAGE_QUESTWINDOW_TAB_BEGIN,
		MESSAGE_QUESTWINDOW_TAB_END = MESSAGE_QUESTWINDOW_TAB_BEGIN + 3 - 1,
		MESSAGE_QUESTWINDOW_QUEST_START_BUTTON,
		MESSAGE_QUESTWINDOW_QUEST_GIVEUP_BUTTON,

		// Npc Quest Window
		MESSAGE_NPCQUESTWINDOW_CLOSE_BUTTON,
		MESSAGE_NPCQUESTWINDOW_COMPLETE_BUTTON,
		MESSAGE_NPCQUESTWINDOW_ANSWER_BEGIN,
		MESSAGE_NPCQUESTWINDOW_ANSWER_END = MESSAGE_NPCQUESTWINDOW_ANSWER_BEGIN + 10 - 1,

		// Chat Window
		MESSAGE_CHATWINDOW_RESIZE_SLIDE,
		MESSAGE_CHATWINDOW_SCROLLBAR,

		// Chat Input
		MESSAGE_CHATINPUT_CHATTYPE_BUTTON,
		MESSAGE_CHATINPUT_PARTYTYPE_BUTTON,
		MESSAGE_CHATINPUT_GUILDTYPE_BUTTON,
		MESSAGE_CHATINPUT_WHISPERBLOCK_BUTTON,
		MESSAGE_CHATINPUT_SYSTEM_BUTTON,
		MESSAGE_CHATINPUT_LOG_BUTTON,
		MESSAGE_CHATINPUT_FRAME_BUTTON,
		MESSAGE_CHATINPUT_SIZE_BUTTON,
		MESSAGE_CHATINPUT_TRANSPARENCY_BUTTON,
		MESSAGE_CHATINPUT_CHAT_TEXTBOX,
		MESSAGE_CHATINPUT_WHISPER_TEXTBOX,

		// My Inventory Window
		MESSAGE_MYINVENTORY_CLOSE_BUTTON,
		MESSAGE_MYINVENTORY_REPAIR_BUTTON,
		MESSAGE_MYINVENTORY_PSHOP_BUTTON,

		// Mix Inventory Window
		MESSAGE_MIXINVENTORY_MIX_BUTTON,

		// Storage Window
		MESSAGE_STORAGE_DEPOSIT_BUTTON,
		MESSAGE_STORAGE_WITHDRAW_BUTTON,
		MESSAGE_STORAGE_LOCK_BUTTON,

		// NPC Shop Window
		MESSAGE_NPCSHOP_REPAIRALL_BUTTON,
		MESSAGE_NPCSHOP_REPAIR_BUTTON,

		// Party Window
		MESSAGE_PARTYWINDOW_CLOSE_BUTTON,
		MESSAGE_PARTYWINDOW_KICK_BEGIN,
		MESSAGE_PARTYWINDOW_KICK_END = MESSAGE_PARTYWINDOW_KICK_BEGIN + MAX_PARTYS - 1,

		// Party List
		MESSAGE_PARTYLIST_SELECT_BEGIN,
		MESSAGE_PARTYLIST_SELECT_END = MESSAGE_PARTYLIST_SELECT_BEGIN + MAX_PARTYS - 1,
		MESSAGE_PARTYLIST_LEAVE_BEGIN,
		MESSAGE_PARTYLIST_LEAVE_END = MESSAGE_PARTYLIST_LEAVE_BEGIN + MAX_PARTYS - 1,

		// Guild Window
		MESSAGE_GUILDWINDOW_CLOSE_BUTTON,
		MESSAGE_GUILDWINDOW_TAB_BEGIN,
		MESSAGE_GUILDWINDOW_TAB_END = MESSAGE_GUILDWINDOW_TAB_BEGIN + 3 - 1,
		MESSAGE_GUILDWINDOW_ANN_SCROLLBAR,
		MESSAGE_GUILDWINDOW_ANN_TEXTBOX,
		MESSAGE_GUILDWINDOW_MEMBER_SCROLLBAR,
		MESSAGE_GUILDWINDOW_MEMBER_BUTTON_BEGIN,
		MESSAGE_GUILDWINDOW_MEMBER_BUTTON_END = MESSAGE_GUILDWINDOW_MEMBER_BUTTON_BEGIN + 20 - 1,
		MESSAGE_GUILDWINDOW_LEAVEGUILD_BUTTON,
		MESSAGE_GUILDWINDOW_GIVEPOSITION_BUTTON,
		MESSAGE_GUILDWINDOW_FREEPOSITION_BUTTON,
		MESSAGE_GUILDWINDOW_KICKPLAYER_BUTTON,
		MESSAGE_GUILDWINDOW_ALLIANCE_SCROLLBAR,
		MESSAGE_GUILDWINDOW_ALLIANCE_BUTTON_BEGIN,
		MESSAGE_GUILDWINDOW_ALLIANCE_BUTTON_END = MESSAGE_GUILDWINDOW_ALLIANCE_BUTTON_BEGIN + 8 - 1,
		MESSAGE_GUILDWINDOW_ALLIANCECREATE_BUTTON,
		MESSAGE_GUILDWINDOW_ALLIANCEOUT_BUTTON,

		// Guild Create Window
		MESSAGE_GUILDCREATE_CLOSE_BUTTON,
		MESSAGE_GUILDCREATE_MARKS_BUTTON_BEGIN,
		MESSAGE_GUILDCREATE_MARKS_BUTTON_END = MESSAGE_GUILDCREATE_MARKS_BUTTON_BEGIN + MAX_GUILDMARKBUFFER - 1,
		MESSAGE_GUILDCREATE_COLORS_BUTTON_BEGIN,
		MESSAGE_GUILDCREATE_COLORS_BUTTON_END = MESSAGE_GUILDCREATE_COLORS_BUTTON_BEGIN + MAX_GUILD_COLORS - 1,
		MESSAGE_GUILDCREATE_NAMEINPUT,
		MESSAGE_GUILDCREATE_BACK_BUTTON,
		MESSAGE_GUILDCREATE_NEXT_BUTTON,

		// Skill Frame
		MESSAGE_SKILLFRAME_CURRENTSKILL,

		MESSAGE_SKILLFRAME_HOTKEYLIST_BEGIN,
		MESSAGE_SKILLFRAME_HOTKEYLIST_END = MESSAGE_SKILLFRAME_HOTKEYLIST_BEGIN + 5 - 1,

		MESSAGE_SKILLFRAME_SKILLLIST_BEGIN,
		MESSAGE_SKILLFRAME_SKILLLIST_END = MESSAGE_SKILLFRAME_SKILLLIST_BEGIN + MAX_MAGIC - 1,

		MESSAGE_SKILLFRAME_COMMANDLIST_BEGIN,
		MESSAGE_SKILLFRAME_COMMANDLIST_END = MESSAGE_SKILLFRAME_COMMANDLIST_BEGIN + 4 - 1,

		// Buff Status
		MESSAGE_BUFFSTATUS_BEGIN,
		MESSAGE_BUFFSTATUS_END = MESSAGE_BUFFSTATUS_BEGIN + 32 - 1,

		// Move Window
		MESSAGE_MOVEWINDOW_CLOSE,
		MESSAGE_MOVEWINDOW_SCROLLBAR,
		MESSAGE_MOVEWINDOW_BEGIN,
		MESSAGE_MOVEWINDOW_END = MESSAGE_MOVEWINDOW_BEGIN + 32 - 1,

		// Trade Window
		MESSAGE_TRADEWINDOW_MONEY_BUTTON,
		MESSAGE_TRADEWINDOW_OK_BUTTON,

		// Blood Castle
		MESSAGE_BLOODCASTLEENTER_CLOSE_BUTTON,
		MESSAGE_BLOODCASTLEENTER_ENTER_BEGIN,
		MESSAGE_BLOODCASTLEENTER_ENTER_END = MESSAGE_BLOODCASTLEENTER_ENTER_BEGIN + 10 - 1,

		// Command Window
		MESSAGE_COMMANDWINDOW_CLOSE_BUTTON,
		MESSAGE_COMMANDWINDOW_BEGIN,
		MESSAGE_COMMANDWINDOW_TRADE,
		MESSAGE_COMMANDWINDOW_BUY,
		MESSAGE_COMMANDWINDOW_PARTY,
		MESSAGE_COMMANDWINDOW_WHISPER,
		MESSAGE_COMMANDWINDOW_GUILD,
		MESSAGE_COMMANDWINDOW_ALLIANCE,
		MESSAGE_COMMANDWINDOW_HOSTILITY,
		MESSAGE_COMMANDWINDOW_SUSPENDHOSTILITY,
		MESSAGE_COMMANDWINDOW_ADDFRIEND,
		MESSAGE_COMMANDWINDOW_FOLLOW,
		MESSAGE_COMMANDWINDOW_DUEL,
		MESSAGE_COMMANDWINDOW_END = MESSAGE_COMMANDWINDOW_DUEL,
		MESSAGE_COMMANDWINDOW_PLAYER_BUTTON,

		// Helper Setup
		MESSAGE_HELPERSETUP_TAB_BEGIN,
		MESSAGE_HELPERSETUP_TAB_END = MESSAGE_HELPERSETUP_TAB_BEGIN + 4 - 1,
	};
};

namespace INTERFACES
{
	enum : mu_uint32
	{
		// General usage
		CURSOR_NORMAL,
		CURSOR_PUSH,
		CURSOR_ATTACK,
		CURSOR_ATTACK2,
		CURSOR_GET,
		CURSOR_TALK,
		CURSOR_REPAIR,
		CURSOR_LEANAGAINST,
		CURSOR_SITDOWN,
		CURSOR_DONTMOVE,
		CURSOR_IDSELECT,

		// Menu
		MENU_BACK,
		MENU_TOP,
		MENU_BOTTOM,
		MENU_RIGHT,
		MENU_LEFT,
		MENU_BUTTON,

		// Message Box
		MSGBOX_WINDOW,
		MSGBOX_TOP,
		MSGBOX_MIDDLE,
		MSGBOX_BOTTOM,
		MSGBOX_BACK,
		MSGBOX_LINE,
		MSGBOX_TITLEBAR,
		MSGBOX_SEPARATE_LINE,
		MSGBOX_OK,
		MSGBOX_CANCEL,
		MSGBOX_CLOSE,
		MSGBOX_PROGRESSBAR_BACK,
		MSGBOX_PROGRESSBAR_FRONT,
		MSGBOX_DUELWINDOW_BACK,

		// General Window Frame
		WINDOWFRAME_TOP,
		WINDOWFRAME_TOP_EXT,
		WINDOWFRAME_LEFT,
		WINDOWFRAME_RIGHT,
		WINDOWFRAME_BOTTOM,

		WINDOW_TOP,
		WINDOW_TOP_EXT,
		WINDOW_MIDDLE,
		WINDOW_BOTTOM,
		WINDOW_LINE,
		WINDOW_ARROW_LEFT,
		WINDOW_ARROW_RIGHT,

		// General Table
		TABLE_TOP_LEFT,
		TABLE_TOP_RIGHT,
		TABLE_BOTTOM_LEFT,
		TABLE_BOTTOM_RIGHT,
		TABLE_TOP_LINE,
		TABLE_BOTTOM_LINE,
		TABLE_LEFT_LINE,
		TABLE_RIGHT_LINE,

		// Game Shop UI
		INGAMESHOP_PAGE_LEFT,
		INGAMESHOP_PAGE_RIGHT,
		INGAMESHOP_PAGE_LEFT2,
		INGAMESHOP_PAGE_RIGHT2,
		
		// Lines
		ITEM_BACK01,
		INVENTORY_BOX02,
		TRADING_LINE,

		// General Usage Buttons
		EXIT_BUTTON,
		EMPTY_BUTTON,
		EMPTY_BUTTON_BIG,
		EMPTY_BUTTON_SMALL,
		EMPTY_BUTTON_VERYSMALL,

		EMPTYNS_BUTTON,
		EMPTYNS_BUTTON_BIG,

		EMPTY_ROUND,

		// Helper General
		MACROUI_INPUTNUMBER,
		MACROUI_INPUTSTRING,
		MACROUI_OPTIONBUTTON,
		MACROUI_RANGEMINUS,
		MACROUI_SETUP,
		MACROUI_START,
		MACROUI_STOP,

		// General Scrollbar
		SCROLLBAR_TOP,
		SCROLLBAR_MIDDLE,
		SCROLLBAR_BOTTOM,
		SCROLLBAR_SLIDE_ON,
		SCROLLBAR_SLIDE_OFF,
		SCROLLBAR_STRETCH,

		SCROLL_BTN_UP,
		SCROLL_BTN_DOWN,

		MYQUEST_LINE,

		GUILD_BTN_TAB1,
		GUILD_BTN_TAB2,
		GUILD_BTN_TAB3,
		GUILD_BTN_TAB4,
		GUILD_MAKE_EDITBOX,

		MIX_BUTTON,
		REPAIR_BUTTON,

		PSHOP_OPEN_BUTTON,
		PSHOP_CLOSE_BUTTON,
		PSHOP_EDITBOX_TITLE,

		ICON_WEAPON_RIGHT,
		ICON_WEAPON_LEFT,
		ICON_HELM,
		ICON_ARMOR,
		ICON_PANTS,
		ICON_GLOVES,
		ICON_BOOTS,
		ICON_WING,
		ICON_HELPER,
		ICON_NECKLACE,
		ICON_RING,

		ICON_MONEY1,
		ICON_MONEY2,
		ICON_MONEY3,

		QUEST_BUTTON_LEFT,
		QUEST_BUTTON_RIGHT,

		// Login interface
		LOGIN_LOGO,
		LOGIN_SERVERLIST_BACKGROUND,
		LOGIN_SERVER_BACKGROUND,
		LOGIN_SERVER_LOADBAR,
		LOGIN_DECORATION_ALL,
		LOGIN_MENU_BUTTON,
		LOGIN_CREDIT_BUTTON,
		LOGIN_DECORATION,
		LOGIN_BACKGROUND,
		LOGIN_TEXT_BACKGROUND,
		LOGIN_SERVER_EX01,
		LOGIN_SERVER_EX02,
		LOGIN_SERVER_EX03,

		// Character interface
		CHARACTER_CREATE_BUTTON,
		CHARACTER_CONNECT_BUTTON,
		CHARACTER_DELETE_BUTTON,
		CHARACTER_CREATE_NAMEINPUT,
		CHARACTER_BALLOON,

		// Mainframe interface
		MAINFRAME_MENU_1,
		MAINFRAME_MENU_2,
		MAINFRAME_MENU_2_1,
		MAINFRAME_MENU_3,
		MAINFRAME_GAUGE_BLUE,
		MAINFRAME_GAUGE_GREEN,
		MAINFRAME_GAUGE_RED,
		MAINFRAME_GAUGE_AG,
		MAINFRAME_GAUGE_SD,
		MAINFRAME_GAUGE_EXBAR,
		MAINFRAME_MASTER_GAUGE_BAR,
		MAINFRAME_MENU_BTN_CHARINFO,
		MAINFRAME_MENU_BTN_INVENTORY,
		MAINFRAME_MENU_BTN_FRIEND,
		MAINFRAME_MENU_BTN_OPTION,
		MAINFRAME_MENU_BTN_CASHSHOP,
		MAINFRAME_SKILL1,
		MAINFRAME_NON_SKILL1,
		MAINFRAME_SKILL2,
		MAINFRAME_NON_SKILL2,
		MAINFRAME_COMMAND,
		MAINFRAME_NON_COMMAND,
		MAINFRAME_SKILLBOX1,
		MAINFRAME_SKILLBOX2,

		// Options interface
		OPTION_TOP,
		OPTION_LEFT,
		OPTION_RIGHT,
		OPTION_LINE,
		OPTION_POINT,
		OPTION_CHECK,
		OPTION_EFFECT_BACK,
		OPTION_EFFECT_COLOR,
		OPTION_VOLUME_BACK,
		OPTION_VOLUME_COLOR,

		// Chat interface
		CHATFRAME_INPUT_BACK,
		CHATFRAME_INPUT_NORMAL_ON,
		CHATFRAME_INPUT_PARTY_ON,
		CHATFRAME_INPUT_GUILD_ON,
		CHATFRAME_INPUT_WHISPER_ON,
		CHATFRAME_INPUT_SYSTEM_ON,
		CHATFRAME_INPUT_LOG_ON,
		CHATFRAME_INPUT_FRAME_ON,
		CHATFRAME_INPUT_BTN_SIZE,
		CHATFRAME_INPUT_BTN_ALPHA,

		// Character interface
		CHARFRAME_TEXTBOX,
		CHARFRAME_BTN_STAT,
		CHARFRAME_BTN_QUEST,
		CHARFRAME_BTN_PET,
		CHARFRAME_BTN_MASTER,

		// Inventory interface
		INVENFRAME_ITEM_BOX,
		INVENFRAME_USEBOX01,
		INVENFRAME_USEBOX02,
		INVENFRAME_USEBOX03,

		// Storage interface
		STORAGE_BTN_DEPOSIT_MONEY,
		STORAGE_BTN_WITHDRAW_MONEY,
		STORAGE_BTN_UNLOCK,
		STORAGE_BTN_LOCK,

		// Trade interface
		TRADE_ACCOUNT_TITLE,
		TRADE_BTN_ACCEPT,

		// Party interface
		PARTY_LIST_BACK,
		PARTY_HPBAR_BACK,
		PARTY_HPBAR_COLOR,
		PARTYLIST_HPBAR,
		PARTY_FLAG,
		PARTY_EXIT,

		// Command interface
		COMMANDFRAME_CURSORID_WND,

		// Pet interface
		PET_LIFEBAR_BACK,
		PET_LIFEBAR_COLOR,

		// Buff interface
		BUFFWINDOW_STATUSICON1,
		BUFFWINDOW_STATUSICON2,

		// Catapult interface
		CATAPULTFRAME_BTN_SMALL,
		CATAPULTFRAME_BTN_BIG,

		// Master interface
		MASTERFRAME_BACK,
		MASTERFRAME_SKILLBOX,
		MASTERFRAME_ARROW_DOWN,
		MASTERFRAME_ARROW_RIGHT,

		// Duel Watch interface
		DUELWATCH_MAINFRAME_BACK1,
		DUELWATCH_MAINFRAME_BACK2,
		DUELWATCH_MAINFRAME_BACK3,
		DUELWATCH_MAINFRAME_SCORE,
		DUELWATCH_MAINFRAME_HP_GAUGE,
		DUELWATCH_MAINFRAME_SD_GAUGE,
		DUELWATCH_MAINFRAME_HP_GAUGE_FX,
		DUELWATCH_MAINFRAME_SD_GAUGE_FX,
		DUELWATCH_USERLIST_BOX,

		// Gens interface
		GENS_RANKBACK,
		GENS_POINT,

		// Hero position interface
		HEROPOSITION_POSITIONA,
		HEROPOSITION_POSITIONB,
		HEROPOSITION_POSITIONC,

		// Endurance interface
		ENDURANCE_PET_BACK,
		ENDURANCE_PET_HPBAR,
		ENDURANCE_BOOTS,
		ENDURANCE_CAP,
		ENDURANCE_GLOVES,
		ENDURANCE_PANTS,
		ENDURANCE_NECKLACE,
		ENDURANCE_RING,
		ENDURANCE_SHIELD,
		ENDURANCE_UPPER,
		ENDURANCE_WEAPON,
		ENDURANCE_WING,
		ENDURANCE_PCROOM,

		// Minimap interface
		MINIMAP_CORNER,
		MINIMAP_LINE,
		MINIMAP_CHARACTER,
		MINIMAP_PORTAL,
		MINIMAP_NPC,
		MINIMAP_CANCEL,

		// My Quest interface
		MYQUEST_BTN_OPEN,
		MYQUEST_BTN_GIVEUP,
		MYQUEST_TAB1,
		MYQUEST_TAB2,
		MYQUEST_TAB3,

		// Credits interface
		CREDITS_LEFT1,
		CREDITS_RIGHT1,
		CREDITS_LEFT2,
		CREDITS_RIGHT2,
		CREDITS_LEFT3,
		CREDITS_RIGHT3,
		CREDITS_LEFT4,
		CREDITS_RIGHT4,
		CREDITS_LEFT5,
		CREDITS_RIGHT5,
		CREDITS_LEFT6,
		CREDITS_RIGHT6,
		CREDITS_LEFT7,
		CREDITS_RIGHT7,
		CREDITS_LEFT8,
		CREDITS_RIGHT8,

		// Blood Castle
		BLOODCASTLE_TIMER,

		// Battle Soccer
		BATTLESOCCER_TIMER,

		// Castle Siege
		CASTLESIEGE_MINIMAP,
		CASTLESIEGE_MINIMAP_FRAME,
		CASTLESIEGE_TIMER_FRAME,
		CASTLESIEGE_COMMAND_ATTACK,
		CASTLESIEGE_COMMAND_DEFEND,
		CASTLEISEGE_COMMAND_WAIT,
		CASTLESIEGE_BATTLESKILL_FRAME,
		CASTLESIEGE_SCROLL_BTN_UP,
		CASTLESIEGE_SCROLL_BTN_DOWN,
		CASTLESIEGE_MINIMAP_BTN_ALPHA,
		CASTLESIEGE_MINIMAP_BTN_GROUP,
		CASTLESIEGE_MINIMAP_BTN_COMMAND,

		// Crywolf
		CRYWOLF_BAR,
		CRYWOLF_BAR2,
		CRYWOLF_DECO,
		CRYWOLF_MAIN1,
		CRYWOLF_MAIN2,
		CRYWOLF_TMAIN,
		CRYWOLF_BAL1,
		CRYWOLF_DARKELF1,
		CRYWOLF_DARKELF2,
		CRYWOLF_NUMBER0,
		CRYWOLF_NUMBER1_OFF,
		CRYWOLF_NUMBER1_ON,
		CRYWOLF_NUMBER2_OFF,
		CRYWOLF_NUMBER2_ON,
		CRYWOLF_FAILURE,
		CRYWOLF_SUCCESS,
		CRYWOLF_BTN_NO1,
		CRYWOLF_BTN_NO2,
		CRYWOLF_BTN_NO3,
		CRYWOLF_BTN_YES1,
		CRYWOLF_BTN_YES2,
		CRYWOLF_BTN_YES3,
		CRYWOLF_BTN_OK1,
		CRYWOLF_BTN_OK2,
		CRYWOLF_BTN_OK3,
		CRYWOLF_M_MAIN,
		CRYWOLF_M_MAIN_RANK,
		CRYWOLF_RANK_TABLE,
		CRYWOLF_RANK_RANK,
		CRYWOLF_RANK_EXP,
		CRYWOLF_RANK_S,
		CRYWOLF_RANK_A,
		CRYWOLF_RANK_B,
		CRYWOLF_RANK_C,
		CRYWOLF_RANK_D,
		CRYWOLF_RANK_0,
		CRYWOLF_RANK_1,
		CRYWOLF_RANK_2,
		CRYWOLF_RANK_3,
		CRYWOLF_RANK_4,
		CRYWOLF_RANK_5,
		CRYWOLF_RANK_6,
		CRYWOLF_RANK_7,
		CRYWOLF_RANK_8,
		CRYWOLF_RANK_9,

		// Kanturu
		KANTURU_WINDOW,

		// Illusion Temple
		ILLUSIONTEMPLE_FAILURE,
		ILLUSIONTEMPLE_SUCCESS,
		ILLUSIONTEMPLE_MINIMAP,
		ILLUSIONTEMPLE_MINIMAP_FRAME,
		ILLUSIONTEMPLE_MINIMAP_BTN_ALPHA,
		ILLUSIONTEMPLE_MINIMAP_RELIC,
		ILLUSIONTEMPLE_MINIMAP_TEAMA_BOX,
		ILLUSIONTEMPLE_MINIMAP_TEAMA_MEMBER,
		ILLUSIONTEMPLE_MINIMAP_TEAMA_NPC,
		ILLUSIONTEMPLE_MINIMAP_TEAMB_BOX,
		ILLUSIONTEMPLE_MINIMAP_TEAMB_MEMBER,
		ILLUSIONTEMPLE_MINIMAP_TEAMB_NPC,
		ILLUSIONTEMPLE_MINIMAP_HERO,
		ILLUSIONTEMPLE_SKILLFRAME,
		ILLUSIONTEMPLE_SKILLUP,
		ILLUSIONTEMPLE_SKILLDOWN,
		ILLUSIONTEMPLE_TIMER,
		ILLUSIONTEMPLE_SCORE_VS0,
		ILLUSIONTEMPLE_SCORE_VS1,
		ILLUSIONTEMPLE_SCORE_LEFT,
		ILLUSIONTEMPLE_SCORE_RIGHT,
		ILLUSIONTEMPLE_SCORE_ALLIED_GAAIL,
		ILLUSIONTEMPLE_SCORE_ALLIED_NUM0,
		ILLUSIONTEMPLE_SCORE_ALLIED_NUM1,
		ILLUSIONTEMPLE_SCORE_ALLIED_NUM2,
		ILLUSIONTEMPLE_SCORE_ALLIED_NUM3,
		ILLUSIONTEMPLE_SCORE_ALLIED_NUM4,
		ILLUSIONTEMPLE_SCORE_ALLIED_NUM5,
		ILLUSIONTEMPLE_SCORE_ALLIED_NUM6,
		ILLUSIONTEMPLE_SCORE_ALLIED_NUM7,
		ILLUSIONTEMPLE_SCORE_ALLIED_NUM8,
		ILLUSIONTEMPLE_SCORE_ALLIED_NUM9,
		ILLUSIONTEMPLE_SCORE_ILLUSION_GAAIL,
		ILLUSIONTEMPLE_SCORE_ILLUSION_NUM0,
		ILLUSIONTEMPLE_SCORE_ILLUSION_NUM1,
		ILLUSIONTEMPLE_SCORE_ILLUSION_NUM2,
		ILLUSIONTEMPLE_SCORE_ILLUSION_NUM3,
		ILLUSIONTEMPLE_SCORE_ILLUSION_NUM4,
		ILLUSIONTEMPLE_SCORE_ILLUSION_NUM5,
		ILLUSIONTEMPLE_SCORE_ILLUSION_NUM6,
		ILLUSIONTEMPLE_SCORE_ILLUSION_NUM7,
		ILLUSIONTEMPLE_SCORE_ILLUSION_NUM8,
		ILLUSIONTEMPLE_SCORE_ILLUSION_NUM9,

		// Doppelganger
		DOPPELGANGER_WINDOWFRAME,
		DOPPELGANGER_GAUGE_RED,
		DOPPELGANGER_GAUGE_ORANGE,
		DOPPELGANGER_GAUGE_YELLOW,
		DOPPELGANGER_GAUGE_PLAYER,
		DOPPELGANGER_GAUGE_PARTY_MEMBER,
		DOPPELGANGER_GAUGE_ICEWALKER,

		// Map Welcome interface
		WELCOME_STRIFE,
		WELCOME_LORENCIA,
		WELCOME_DUNGEON,
		WELCOME_DEVIAS,
		WELCOME_NORIA,
		WELCOME_LOSTTOWER,
		WELCOME_STADIUM,
		WELCOME_ATLANS,
		WELCOME_TARKAN,
		WELCOME_DEVILSQUARE,
		WELCOME_ICARUS,
		WELCOME_BLOODCASTLE,
		WELCOME_CHAOSCASTLE,
		WELCOME_KALIMA,
		WELCOME_VALLEYOFLOREN,
		WELCOME_LANDOFTRIAL,
		WELCOME_AIDA,
		WELCOME_CRYWOLF,
		WELCOME_KANTURU,
		WELCOME_BALGASBARRACK,
		WELCOME_BALGASREFUGE,
		WELCOME_ILLUSIONTEMPLE,
		WELCOME_ELBELAND,
		WELCOME_SWAMPOFCALMNESS,
		WELCOME_RAKLION,
		WELCOME_RAKLIONBOSS,
		WELCOME_SANTATOWN,
		WELCOME_PKFIELD,
		WELCOME_DUELARENA,
		WELCOME_DOPPELGANGER,
		WELCOME_EMPIREGUARDIAN,
		WELCOME_LORENMARKET,

		MAX,
	};

	extern MUTexture Data[MAX];
	extern MULoadingData Load[];
	extern const mu_uint32 LoadCount;
};

#endif