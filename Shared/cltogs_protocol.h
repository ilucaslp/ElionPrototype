#ifndef __CLTOGS_PROTOCOL_H__
#define __CLTOGS_PROTOCOL_H__

#pragma once

// Client<->GameServer
enum CL2GS_PROTOCOL : mu_uint16
{
	GS2CL_PROT_CONNECTED = 0x0000,
	GS2CL_PROT_CLIENTALLOW,
	GS2CL_PROT_CLIENTDENIED,

	GS2CL_PROT_LOGIN,
	GS2CL_PROT_CHARACTERLIST,
	GS2CL_PROT_CREATECHARACTER,
	GS2CL_PROT_DELETECHARACTER,
	GS2CL_PROT_SELECTCHARACTER,
	GS2CL_PROT_LOGOUT,

	// Actions
	GS2CL_PROT_HEROANGLE,
	GS2CL_PROT_MOVE,
	GS2CL_PROT_OPERATE,
	GS2CL_PROT_FOLLOW,
	GS2CL_PROT_ATTACK,
	GS2CL_PROT_MAGIC,
	GS2CL_PROT_TALK,
	GS2CL_PROT_CANCEL,

	GS2CL_PROT_WEAPONSTATE,
	GS2CL_PROT_DAMAGE,
	GS2CL_PROT_SIMPLEDAMAGE,
	GS2CL_PROT_KILLPLAYER,
	GS2CL_PROT_PICKITEM,
	GS2CL_PROT_PICKGOLD,
	GS2CL_PROT_MOVEITEM,
	GS2CL_PROT_AUTOMOVEITEM,
	GS2CL_PROT_USEITEM,
	GS2CL_PROT_DROPITEM,
	GS2CL_PROT_REPAIRITEM,
	GS2CL_PROT_BUYITEM,
	GS2CL_PROT_SELLITEM,

	GS2CL_PROT_RELOADARROW,
	GS2CL_PROT_SERVERCOMMAND,
	GS2CL_PROT_MOVECOMMAND,

	GS2CL_PROT_PARTYREQUEST,
	GS2CL_PROT_PARTYANSWER,
	GS2CL_PROT_PARTYRESULT,
	GS2CL_PROT_PARTYKICK,

	GS2CL_PROT_STORAGEGOLD,
	GS2CL_PROT_STORAGELOCK,
	GS2CL_PROT_STORAGESTATE,

	GS2CL_PROT_CLOSEWINDOW,

	GS2CL_PROT_ITEMINSERT,
	GS2CL_PROT_ITEMDELETE,
	GS2CL_PROT_ITEMMODIFY,
	GS2CL_PROT_ITEMDURABILITY,
	GS2CL_PROT_ITEMMOVERESULT,

	GS2CL_PROT_SHOPLIST,
	GS2CL_PROT_TAXRATE,

	GS2CL_PROT_MIX,

	GS2CL_PROT_TRADEREQUEST,
	GS2CL_PROT_TRADEANSWER,
	GS2CL_PROT_TRADERESULT,
	GS2CL_PROT_TRADEOK,
	GS2CL_PROT_TRADEMONEY,

	GS2CL_PROT_CREATEGUILD,
	GS2CL_PROT_LEAVEGUILD,
	GS2CL_PROT_GUILDINFO,
	GS2CL_PROT_GUILDJOIN_REQUEST,
	GS2CL_PROT_GUILDJOIN_PETITION,
	GS2CL_PROT_GUILDSTATUS_CANCEL,
	GS2CL_PROT_GUILDSTATUS_GIVE,

	GS2CL_PROT_QUESTANSWER,

	GS2CL_PROT_SETATTRIBUTE,

	GS2CL_PROT_VIEWPORT_PACKAGE = 0x0050,

	GS2CL_PROT_LIVE = 0x00F0,
	GS2CL_PROT_CHAT,
	GS2CL_PROT_CHATKEY,
	GS2CL_PROT_WHISPER,
	GS2CL_PROT_WHISPERFAIL,
	GS2CL_PROT_NOTICE,

	GS2CL_PROT_INCREASESTAT,

	GS2CL_PROT_EVENTENTER,
	GS2CL_PROT_EVENTTIMER,
	GS2CL_PROT_EVENTRANK,
	GS2CL_PROT_EVENTSTATE,

	GS2CL_PROT_BLOODCASTLE,

	GS2CL_PROT_HELPERSTART,
	GS2CL_PROT_HELPERSTOP,
	GS2CL_PROT_HELPERSETTING,
};

enum VIEWPORT_PROTOCOL : mu_uint16
{
	GS2CL_PROT_VIEWPORT_CREATECHARACTER,
	GS2CL_PROT_VIEWPORT_DELETECHARACTER,
	GS2CL_PROT_VIEWPORT_CREATEMONSTER,
	GS2CL_PROT_VIEWPORT_DELETEMONSTER,
	GS2CL_PROT_VIEWPORT_CREATESUMMON,

	GS2CL_PROT_VIEWPORT_CREATEMONEY,
	GS2CL_PROT_VIEWPORT_CREATEITEM,
	GS2CL_PROT_VIEWPORT_DELETEITEM,

	GS2CL_PROT_VIEWPORT_CHARSETCHANGE,
	GS2CL_PROT_VIEWPORT_FORCEPOSITION,

	GS2CL_PROT_VIEWPORT_REFRESH,
	GS2CL_PROT_VIEWPORT_STAND,
	GS2CL_PROT_VIEWPORT_ANIMATION,
	GS2CL_PROT_VIEWPORT_DIE,
	GS2CL_PROT_VIEWPORT_MOVE,
	GS2CL_PROT_VIEWPORT_REGEN,
	GS2CL_PROT_VIEWPORT_ATTACK,
	GS2CL_PROT_VIEWPORT_MAGIC,
	GS2CL_PROT_VIEWPORT_MAGIC2,
	GS2CL_PROT_VIEWPORT_WEATHER,
	GS2CL_PROT_VIEWPORT_EVENTMASK,
	GS2CL_PROT_VIEWPORT_LIFE,
	GS2CL_PROT_VIEWPORT_MANA,
	GS2CL_PROT_VIEWPORT_LEVELUP,
	GS2CL_PROT_VIEWPORT_SHIELDBROKE,
	GS2CL_PROT_VIEWPORT_POTIONSD,
	GS2CL_PROT_VIEWPORT_QUESTEFFECT,
	GS2CL_PROT_VIEWPORT_QUESTHISTORY,
	GS2CL_PROT_VIEWPORT_MAGICLIST,
	GS2CL_PROT_VIEWPORT_EXPERIENCE,
	GS2CL_PROT_VIEWPORT_LEVELUPPOINTS,
	GS2CL_PROT_VIEWPORT_STATS,
	GS2CL_PROT_VIEWPORT_MONEY,
	GS2CL_PROT_VIEWPORT_SPEED,
	GS2CL_PROT_VIEWPORT_BUFFEFFECT,
	GS2CL_PROT_VIEWPORT_PKLEVEL,
	GS2CL_PROT_VIEWPORT_TELEPORT,
	GS2CL_PROT_VIEWPORT_TELEPORTEND,
	GS2CL_PROT_VIEWPORT_INVENTORY,
	GS2CL_PROT_VIEWPORT_TRADEMONEY,

	GS2CL_PROT_VIEWPORT_PARTYLIST,
	GS2CL_PROT_VIEWPORT_PARTYINFO,

	GS2CL_PROT_VIEWPORT_GUILDINFO,
	GS2CL_PROT_VIEWPORT_GUILDLIST,
	GS2CL_PROT_VIEWPORT_GUILDUPDATE,

	GS2CL_PROT_VIEWPORT_PETUPDATE,
	GS2CL_PROT_VIEWPORT_PETEFFECT,
	GS2CL_PROT_VIEWPORT_PETCOMMAND,
	GS2CL_PROT_VIEWPORT_PETATTACK,
};

#include "cltogs_chat.h"
#include "cltogs_party.h"
#include "cltogs_guild.h"
#include "cltogs_storage.h"
#include "cltogs_mix.h"
#include "cltogs_trade.h"
#include "cltogs_event.h"
#include "cltogs_quest.h"
#include "cltogs_helper.h"

class CL2GS_LIVE : public MU_CLIENT_TO_SERVER_HEADEREX
{
public:
	CL2GS_LIVE()
	{
		Code = GS2CL_PROT_LIVE;
	}
};

class CL2GS_REQUEST_CLIENTALLOW_DATA
{
public:
	mu_string Version;
	mu_string Serial;
};

class CL2GS_REQUEST_CLIENTALLOW : public MU_CLIENT_TO_SERVER_HEADEREX, public CL2GS_REQUEST_CLIENTALLOW_DATA
{
public:
	enum
	{
		MAX_VERSION_LENGTH = 10,
		MAX_SERIAL_LENGTH = 128,
	};

public:
	CL2GS_REQUEST_CLIENTALLOW()
	{
		Code = GS2CL_PROT_CLIENTALLOW;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITESTRING(Version, MAX_VERSION_LENGTH);
		PARSER_WRITESTRING(Serial, MAX_SERIAL_LENGTH);

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITESTRING(Version, MAX_VERSION_LENGTH);
		PARSER_WRITESTRING(Serial, MAX_SERIAL_LENGTH);

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_READSTRING(Version, MAX_VERSION_LENGTH);
		PARSER_READSTRING(Serial, MAX_SERIAL_LENGTH);

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_RECEIVE_CLIENTDENIED_DATA
{
public:
	mu_uint8 Reason;
};
#pragma pack(pop)

class GS2CL_RECEIVE_CLIENTDENIED : public MU_SERVER_TO_CLIENT_HEADEREX, public GS2CL_RECEIVE_CLIENTDENIED_DATA
{
public:
	GS2CL_RECEIVE_CLIENTDENIED()
	{
		Code = GS2CL_PROT_CLIENTDENIED;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_CLIENTDENIED_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_CLIENTDENIED_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_RECEIVE_CLIENTDENIED_DATA*>(this));

		return true;
	}
};

enum EREQUEST_LOGOUT
{
	ERL_EXITGAME,
	ERL_CHARACTER,
	ERL_SERVER,
};

#pragma pack(push, 1)
class ARM_PACKED CL2GS_REQUEST_LOGOUT_DATA
{
public:
	mu_uint8 Type;
};
#pragma pack(pop)

class CL2GS_REQUEST_LOGOUT : public MU_CLIENT_TO_SERVER_HEADEREX, public CL2GS_REQUEST_LOGOUT_DATA
{
public:
	CL2GS_REQUEST_LOGOUT()
	{
		Code = GS2CL_PROT_LOGOUT;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_REQUEST_LOGOUT_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_REQUEST_LOGOUT_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<CL2GS_REQUEST_LOGOUT_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_RECEIVE_LOGOUT_DATA
{
public:
	mu_uint8 Type;
};
#pragma pack(pop)

class GS2CL_RECEIVE_LOGOUT : public MU_SERVER_TO_CLIENT_HEADEREX, public GS2CL_RECEIVE_LOGOUT_DATA
{
public:
	GS2CL_RECEIVE_LOGOUT()
	{
		Code = GS2CL_PROT_LOGOUT;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_LOGOUT_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_LOGOUT_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_RECEIVE_LOGOUT_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED CL2GS_REQUEST_INCREASESTAT_DATA
{
public:
	mu_uint8 StatType;
};
#pragma pack(pop)

class CL2GS_REQUEST_INCREASESTAT : public MU_CLIENT_TO_SERVER_HEADEREX, public CL2GS_REQUEST_INCREASESTAT_DATA
{
public:
	CL2GS_REQUEST_INCREASESTAT()
	{
		Code = GS2CL_PROT_INCREASESTAT;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_REQUEST_INCREASESTAT_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_REQUEST_INCREASESTAT_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<CL2GS_REQUEST_INCREASESTAT_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED CL2GS_REQUEST_PICKITEM_DATA
{
public:
	mu_uint16 ItemKey;
};
#pragma pack(pop)

class CL2GS_REQUEST_PICKITEM : public MU_CLIENT_TO_SERVER_HEADEREX, public CL2GS_REQUEST_PICKITEM_DATA
{
public:
	CL2GS_REQUEST_PICKITEM()
	{
		Code = GS2CL_PROT_PICKITEM;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_REQUEST_PICKITEM_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_REQUEST_PICKITEM_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<CL2GS_REQUEST_PICKITEM_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_RESPONSE_PICKITEM_DATA
{
public:
	mu_boolean InsertItem;
};
#pragma pack(pop)

class GS2CL_RESPONSE_PICKITEM_DATAEX
{
public:
	std::vector<SHARED::ITEM_DATA> Items;
};

class GS2CL_RESPONSE_PICKITEM : public MU_SERVER_TO_CLIENT_HEADEREX, public GS2CL_RESPONSE_PICKITEM_DATA, public GS2CL_RESPONSE_PICKITEM_DATAEX
{
public:
	GS2CL_RESPONSE_PICKITEM()
	{
		Code = GS2CL_PROT_PICKITEM;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RESPONSE_PICKITEM_DATA*>(this));
		PARSER_WRITEVECTOR(Items);

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RESPONSE_PICKITEM_DATA*>(this));
		PARSER_WRITEVECTOR(Items);

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_RESPONSE_PICKITEM_DATA*>(this));
		PARSER_READVECTOR(Items);

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_RESPONSE_PICKGOLD_DATA
{
public:
	mu_int64 Money;
};
#pragma pack(pop)

class GS2CL_RESPONSE_PICKGOLD : public MU_SERVER_TO_CLIENT_HEADEREX, public GS2CL_RESPONSE_PICKGOLD_DATA
{
public:
	GS2CL_RESPONSE_PICKGOLD()
	{
		Code = GS2CL_PROT_PICKGOLD;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RESPONSE_PICKGOLD_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RESPONSE_PICKGOLD_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_RESPONSE_PICKGOLD_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED CL2GS_REQUEST_MOVEITEM_DATA
{
public:
	mu_uint8 SrcType;
	mu_uint8 SrcX;
	mu_uint8 SrcY;
	mu_uint8 DestType;
	mu_uint8 DestX;
	mu_uint8 DestY;
};
#pragma pack(pop)

class CL2GS_REQUEST_MOVEITEM : public MU_CLIENT_TO_SERVER_HEADEREX, public CL2GS_REQUEST_MOVEITEM_DATA
{
public:
	CL2GS_REQUEST_MOVEITEM()
	{
		Code = GS2CL_PROT_MOVEITEM;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_REQUEST_MOVEITEM_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_REQUEST_MOVEITEM_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<CL2GS_REQUEST_MOVEITEM_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED CL2GS_REQUEST_AUTOMOVEITEM_DATA
{
public:
	mu_uint8 SrcType;
	mu_uint8 SrcX;
	mu_uint8 SrcY;
	mu_uint8 DestType;
};
#pragma pack(pop)

class CL2GS_REQUEST_AUTOMOVEITEM : public MU_CLIENT_TO_SERVER_HEADEREX, public CL2GS_REQUEST_AUTOMOVEITEM_DATA
{
public:
	CL2GS_REQUEST_AUTOMOVEITEM()
	{
		Code = GS2CL_PROT_AUTOMOVEITEM;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_REQUEST_AUTOMOVEITEM_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_REQUEST_AUTOMOVEITEM_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<CL2GS_REQUEST_AUTOMOVEITEM_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED CL2GS_REQUEST_USEITEM_DATA
{
public:
	mu_uint8 SourceIndex;
	mu_uint8 TargetIndex;
	mu_uint8 ItemUseType;
};
#pragma pack(pop)

class CL2GS_REQUEST_USEITEM : public MU_CLIENT_TO_SERVER_HEADEREX, public CL2GS_REQUEST_USEITEM_DATA
{
public:
	CL2GS_REQUEST_USEITEM()
	{
		Code = GS2CL_PROT_USEITEM;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_REQUEST_USEITEM_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_REQUEST_USEITEM_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<CL2GS_REQUEST_USEITEM_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED CL2GS_REQUEST_DROPITEM_DATA
{
public:
	mu_uint8 InventoryType;
	mu_uint8 InventoryX;
	mu_uint8 InventoryY;
	mu_uint8 MapX;
	mu_uint8 MapY;
};
#pragma pack(pop)

class CL2GS_REQUEST_DROPITEM : public MU_CLIENT_TO_SERVER_HEADEREX, public CL2GS_REQUEST_DROPITEM_DATA
{
public:
	CL2GS_REQUEST_DROPITEM()
	{
		Code = GS2CL_PROT_DROPITEM;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_REQUEST_DROPITEM_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_REQUEST_DROPITEM_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<CL2GS_REQUEST_DROPITEM_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED CL2GS_REQUEST_REPAIRITEM_DATA
{
public:
	mu_uint8 InventoryType;
	mu_uint8 InventoryX;
	mu_uint8 InventoryY;
	mu_boolean RepairAll;
	mu_boolean ShopRepair;
};
#pragma pack(pop)

class CL2GS_REQUEST_REPAIRITEM : public MU_CLIENT_TO_SERVER_HEADEREX, public CL2GS_REQUEST_REPAIRITEM_DATA
{
public:
	CL2GS_REQUEST_REPAIRITEM()
	{
		Code = GS2CL_PROT_REPAIRITEM;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_REQUEST_REPAIRITEM_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_REQUEST_REPAIRITEM_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<CL2GS_REQUEST_REPAIRITEM_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED CL2GS_REQUEST_BUYITEM_DATA
{
public:
	mu_uint8 InventoryX;
	mu_uint8 InventoryY;
};
#pragma pack(pop)

class CL2GS_REQUEST_BUYITEM : public MU_CLIENT_TO_SERVER_HEADEREX, public CL2GS_REQUEST_BUYITEM_DATA
{
public:
	CL2GS_REQUEST_BUYITEM()
	{
		Code = GS2CL_PROT_BUYITEM;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_REQUEST_BUYITEM_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_REQUEST_BUYITEM_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<CL2GS_REQUEST_BUYITEM_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED CL2GS_REQUEST_SELLITEM_DATA
{
public:
	mu_uint8 InventoryType;
	mu_uint8 InventoryX;
	mu_uint8 InventoryY;
};
#pragma pack(pop)

class CL2GS_REQUEST_SELLITEM : public MU_CLIENT_TO_SERVER_HEADEREX, public CL2GS_REQUEST_SELLITEM_DATA
{
public:
	CL2GS_REQUEST_SELLITEM()
	{
		Code = GS2CL_PROT_SELLITEM;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_REQUEST_SELLITEM_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_REQUEST_SELLITEM_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<CL2GS_REQUEST_SELLITEM_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_RESPONSE_BUYITEM_DATA
{
public:
	mu_uint8 Result;
};
#pragma pack(pop)

class GS2CL_RESPONSE_BUYITEM : public MU_SERVER_TO_CLIENT_HEADEREX, public GS2CL_RESPONSE_BUYITEM_DATA
{
public:
	enum
	{
		BUY_FAILED,
		BUY_TIMEOUT,
		BUY_SUCCEED,
	};
public:
	GS2CL_RESPONSE_BUYITEM()
	{
		Code = GS2CL_PROT_BUYITEM;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RESPONSE_BUYITEM_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RESPONSE_BUYITEM_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_RESPONSE_BUYITEM_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_RESPONSE_SELLITEM_DATA
{
public:
	mu_uint8 Result;
};
#pragma pack(pop)

class GS2CL_RESPONSE_SELLITEM : public MU_SERVER_TO_CLIENT_HEADEREX, public GS2CL_RESPONSE_SELLITEM_DATA
{
public:
	enum
	{
		SELL_FAILED,
		SELL_TIMEOUT,
		SELL_SUCCEED,
	};
public:
	GS2CL_RESPONSE_SELLITEM()
	{
		Code = GS2CL_PROT_SELLITEM;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RESPONSE_SELLITEM_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RESPONSE_SELLITEM_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_RESPONSE_SELLITEM_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_RESPONSE_MOVEITEM_DATA
{
public:
	mu_uint8 SrcType;
	mu_uint8 SrcX;
	mu_uint8 SrcY;
	mu_uint8 DestType;
	mu_uint8 DestX;
	mu_uint8 DestY;
};
#pragma pack(pop)

class GS2CL_RESPONSE_MOVEITEM : public MU_SERVER_TO_CLIENT_HEADEREX, public GS2CL_RESPONSE_MOVEITEM_DATA
{
public:
	GS2CL_RESPONSE_MOVEITEM()
	{
		Code = GS2CL_PROT_MOVEITEM;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RESPONSE_MOVEITEM_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RESPONSE_MOVEITEM_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_RESPONSE_MOVEITEM_DATA*>(this));

		return true;
	}
};

#define PASSWORDSECRET_FORMAT "%08X"

#pragma pack(push, 1)
class ARM_PACKED GS2CL_RECEIVE_CONNECT_DATA
{
public:
	mu_uint16 Slot;
	mu_uint32 PasswordSecret;
};
#pragma pack(pop)

class GS2CL_RECEIVE_CONNECT : public MU_SERVER_TO_CLIENT_HEADEREX, public GS2CL_RECEIVE_CONNECT_DATA
{
public:
	GS2CL_RECEIVE_CONNECT()
	{
		Code = GS2CL_PROT_CONNECTED;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_CONNECT_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_CONNECT_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_RECEIVE_CONNECT_DATA*>(this));

		return true;
	}
};

class CL2GS_REQUEST_LOGIN : public MU_CLIENT_TO_SERVER_HEADEREX
{
public:
	CL2GS_REQUEST_LOGIN()
	{
		Code = GS2CL_PROT_LOGIN;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITESTRING(Username, ACCOUNT_MAXIMUM);
		PARSER_WRITEDATA(PasswordHash, HASH_LENGTH);

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITESTRING(Username, ACCOUNT_MAXIMUM);
		PARSER_WRITEDATA(PasswordHash, HASH_LENGTH);

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_READSTRING(Username, ACCOUNT_MAXIMUM);
		PARSER_READDATA(PasswordHash, HASH_LENGTH);

		return true;
	}

public:
	mu_string Username;
	mu_uint8 PasswordHash[HASH_LENGTH];
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_RESPONSE_LOGIN_DATA
{
public:
	mu_uint8 Response;
};
#pragma pack(pop)

class GS2CL_RESPONSE_LOGIN : public MU_SERVER_TO_CLIENT_HEADEREX, public GS2CL_RESPONSE_LOGIN_DATA
{
public:
	GS2CL_RESPONSE_LOGIN()
	{
		Code = GS2CL_PROT_LOGIN;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RESPONSE_LOGIN_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RESPONSE_LOGIN_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_RESPONSE_LOGIN_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_RESPONSE_CHARACTERLIST_HEADER
{
public:
	mu_boolean EnabledClasses[GAME::MAX_CLASS];
};
#pragma pack(pop)

class GS2CL_RESPONSE_CHARACTERLIST : public MU_SERVER_TO_CLIENT_HEADEREX, public GS2CL_RESPONSE_CHARACTERLIST_HEADER
{
public:
	class CHARACTER_DATA_HEADER
	{
	public:
		mu_uint8 SlotIndex;
		mu_uint8 Class;
		mu_uint8 SubClass;
		mu_uint8 ControlCode;
		mu_uint8 PkLevel;

		mu_int16 GuildStatus;
		mu_uint16 Level;
		mu_uint16 DarkRavenLevel;
	};

	class CHARACTER_DATA : public CHARACTER_DATA_HEADER
	{
	public:
		SHARED::CHARSET_PACKET Equipment[GAME::MAX_EQUIPMENT];
		mu_string Name;
	};

public:
	GS2CL_RESPONSE_CHARACTERLIST()
	{
		Code = GS2CL_PROT_CHARACTERLIST;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RESPONSE_CHARACTERLIST_HEADER*>(this));
		PARSER_WRITE32(Characters.size());

		for (auto it = Characters.begin(); it != Characters.end(); ++it)
		{
			CHARACTER_DATA &character = *it;

			PARSER_WRITE(*static_cast<CHARACTER_DATA_HEADER*>(&character));

			for (mu_uint32 n = 0; n < GAME::MAX_EQUIPMENT; ++n)
			{
				PARSER_WRITECHARSET(character.Equipment[n]);
			}

			PARSER_WRITESTRING(character.Name, CHARACTER_MAXIMUM);
		}

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RESPONSE_CHARACTERLIST_HEADER*>(this));
		PARSER_WRITE32(Characters.size());

		for (auto it = Characters.begin(); it != Characters.end(); ++it)
		{
			CHARACTER_DATA &character = *it;

			PARSER_WRITE(*static_cast<CHARACTER_DATA_HEADER*>(&character));

			for (mu_uint32 n = 0; n < GAME::MAX_EQUIPMENT; ++n)
			{
				PARSER_WRITECHARSET(character.Equipment[n]);
			}

			PARSER_WRITESTRING(character.Name, CHARACTER_MAXIMUM);
		}

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_RESPONSE_CHARACTERLIST_HEADER*>(this));
		mu_uint32 charactersCount = 0;
		PARSER_READ(charactersCount);
		Characters.resize(charactersCount);

		for (auto it = Characters.begin(); it != Characters.end(); ++it)
		{
			CHARACTER_DATA &character = *it;


			PARSER_READ(*static_cast<CHARACTER_DATA_HEADER*>(&character));

			for (mu_uint32 n = 0; n < GAME::MAX_EQUIPMENT; ++n)
			{
				PARSER_READCHARSET(character.Equipment[n]);
			}

			PARSER_READSTRING(character.Name, CHARACTER_MAXIMUM);
		}

		return true;
	}

public:
	std::vector<CHARACTER_DATA> Characters;
};

class CL2GS_REQUEST_CREATECHARACTER : public MU_CLIENT_TO_SERVER_HEADEREX
{
public:
	CL2GS_REQUEST_CREATECHARACTER()
	{
		Code = GS2CL_PROT_CREATECHARACTER;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(Class);
		PARSER_WRITESTRING(CharacterName, CHARACTER_MAXIMUM);

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(Class);
		PARSER_WRITESTRING(CharacterName, CHARACTER_MAXIMUM);

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_READ(Class);
		PARSER_READSTRING(CharacterName, CHARACTER_MAXIMUM);

		return true;
	}

public:
	mu_uint8 Class;
	mu_string CharacterName;
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_RESPONSE_CREATECHARACTER_HEADER
{
public:
	mu_uint8 ResponseCode;
	mu_uint8 SlotIndex;
	mu_uint8 Class;
	mu_uint8 SubClass;
	mu_uint8 ControlCode;
	mu_uint8 PkLevel;

	mu_int16 GuildStatus;
	mu_uint16 Level;
	mu_uint16 DarkRavenLevel;

	mu_boolean EnabledClasses[GAME::MAX_CLASS];
};
#pragma pack(pop)

class GS2CL_RESPONSE_CREATECHARACTER_HEADEREX
{
public:
	SHARED::CHARSET_PACKET Equipment[GAME::MAX_EQUIPMENT];
	mu_string Name;
};

class GS2CL_RESPONSE_CREATECHARACTER : public MU_SERVER_TO_CLIENT_HEADEREX,
	public GS2CL_RESPONSE_CREATECHARACTER_HEADER, 
	public GS2CL_RESPONSE_CREATECHARACTER_HEADEREX
{
public:
	GS2CL_RESPONSE_CREATECHARACTER()
	{
		Code = GS2CL_PROT_CREATECHARACTER;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RESPONSE_CREATECHARACTER_HEADER*>(this));

		for (mu_uint32 n = 0; n < GAME::MAX_EQUIPMENT; ++n)
		{
			PARSER_WRITECHARSET(Equipment[n]);
		}

		PARSER_WRITESTRING(Name, CHARACTER_MAXIMUM);

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RESPONSE_CREATECHARACTER_HEADER*>(this));

		for (mu_uint32 n = 0; n < GAME::MAX_EQUIPMENT; ++n)
		{
			PARSER_WRITECHARSET(Equipment[n]);
		}

		PARSER_WRITESTRING(Name, CHARACTER_MAXIMUM);

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_RESPONSE_CREATECHARACTER_HEADER*>(this));

		for (mu_uint32 n = 0; n < GAME::MAX_EQUIPMENT; ++n)
		{
			PARSER_READCHARSET(Equipment[n]);
		}

		PARSER_READSTRING(Name, CHARACTER_MAXIMUM);

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED CL2GS_REQUEST_DELETECHARACTER_DATA
{
public:
	mu_uint8 SlotIndex;
};
#pragma pack(pop)

class CL2GS_REQUEST_DELETECHARACTER : public MU_CLIENT_TO_SERVER_HEADEREX, public CL2GS_REQUEST_DELETECHARACTER_DATA
{
public:
	CL2GS_REQUEST_DELETECHARACTER()
	{
		Code = GS2CL_PROT_DELETECHARACTER;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_REQUEST_DELETECHARACTER_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_REQUEST_DELETECHARACTER_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<CL2GS_REQUEST_DELETECHARACTER_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_RESPONSE_DELETECHARACTER_DATA
{
public:
	mu_uint8 ResponseCode;
	mu_uint8 SlotIndex;
};
#pragma pack(pop)

class GS2CL_RESPONSE_DELETECHARACTER : public MU_SERVER_TO_CLIENT_HEADEREX, public GS2CL_RESPONSE_DELETECHARACTER_DATA
{
public:
	GS2CL_RESPONSE_DELETECHARACTER()
	{
		Code = GS2CL_PROT_DELETECHARACTER;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RESPONSE_DELETECHARACTER_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RESPONSE_DELETECHARACTER_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_RESPONSE_DELETECHARACTER_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED CL2GS_REQUEST_SELECTCHARACTER_DATA
{
public:
	mu_uint8 SlotIndex;
};
#pragma pack(pop)

class CL2GS_REQUEST_SELECTCHARACTER : public MU_CLIENT_TO_SERVER_HEADEREX, public CL2GS_REQUEST_SELECTCHARACTER_DATA
{
public:
	CL2GS_REQUEST_SELECTCHARACTER()
	{
		Code = GS2CL_PROT_SELECTCHARACTER;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_REQUEST_SELECTCHARACTER_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_REQUEST_SELECTCHARACTER_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<CL2GS_REQUEST_SELECTCHARACTER_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_RESPONSE_SELECTCHARACTER_HEADER
{
public:
	mu_uint32 Key;

	mu_float Direction;

	mu_int16 GuildStatus;

	mu_uint8 Class;
	mu_uint8 SubClass;
	mu_uint8 PkLevel;
	mu_uint8 Map;
	mu_uint8 PositionX;
	mu_uint8 PositionY;
	mu_uint8 ControlCode;
};
#pragma pack(pop)

class GS2CL_RESPONSE_SELECTCHARACTER_HEADEREX
{
public:
	mu_string Name;
	HELPER_SETTING HelperSetting;
};

class GS2CL_RESPONSE_SELECTCHARACTER : public MU_SERVER_TO_CLIENT_HEADEREX,
	public GS2CL_RESPONSE_SELECTCHARACTER_HEADER,
	public GS2CL_RESPONSE_SELECTCHARACTER_HEADEREX
{
public:
	GS2CL_RESPONSE_SELECTCHARACTER()
	{
		Code = GS2CL_PROT_SELECTCHARACTER;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RESPONSE_SELECTCHARACTER_HEADER*>(this));
		PARSER_WRITESTRING(Name, CHARACTER_MAXIMUM);
		PARSER_WRITE(HelperSetting.Hunting);
		PARSER_WRITE(*static_cast<HELPER_OBTAININGSETTING*>(&HelperSetting.Obtaining));
		PARSER_WRITEVECTOR(HelperSetting.Obtaining.ExtraItems);

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RESPONSE_SELECTCHARACTER_HEADER*>(this));
		PARSER_WRITESTRING(Name, CHARACTER_MAXIMUM);
		PARSER_WRITE(HelperSetting.Hunting);
		PARSER_WRITE(*static_cast<HELPER_OBTAININGSETTING*>(&HelperSetting.Obtaining));
		PARSER_WRITEVECTOR(HelperSetting.Obtaining.ExtraItems);

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_RESPONSE_SELECTCHARACTER_HEADER*>(this));
		PARSER_READSTRING(Name, CHARACTER_MAXIMUM);
		PARSER_READ(HelperSetting.Hunting);
		PARSER_READ(*static_cast<HELPER_OBTAININGSETTING*>(&HelperSetting.Obtaining));
		PARSER_READVECTOR(HelperSetting.Obtaining.ExtraItems);

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED CL2GS_REQUEST_HEROANGLE_DATA
{
public:
	mu_float Angle;
};
#pragma pack(pop)

class CL2GS_REQUEST_HEROANGLE : public MU_CLIENT_TO_SERVER_HEADEREX, public CL2GS_REQUEST_HEROANGLE_DATA
{
public:
	CL2GS_REQUEST_HEROANGLE()
	{
		Code = GS2CL_PROT_HEROANGLE;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_REQUEST_HEROANGLE_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_REQUEST_HEROANGLE_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<CL2GS_REQUEST_HEROANGLE_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED CL2GS_REQUEST_MOVE_DATA
{
public:
	mu_uint8 X;
	mu_uint8 Y;
};
#pragma pack(pop)

class CL2GS_REQUEST_MOVE : public MU_CLIENT_TO_SERVER_HEADEREX, public CL2GS_REQUEST_MOVE_DATA
{
public:
	CL2GS_REQUEST_MOVE()
	{
		Code = GS2CL_PROT_MOVE;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_REQUEST_MOVE_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_REQUEST_MOVE_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<CL2GS_REQUEST_MOVE_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED CL2GS_REQUEST_OPERATE_DATA
{
public:
	mu_uint8 X;
	mu_uint8 Y;
	mu_uint16 Type;
	mu_float Angle;
};
#pragma pack(pop)

class CL2GS_REQUEST_OPERATE : public MU_CLIENT_TO_SERVER_HEADEREX, public CL2GS_REQUEST_OPERATE_DATA
{
public:
	CL2GS_REQUEST_OPERATE()
	{
		Code = GS2CL_PROT_OPERATE;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_REQUEST_OPERATE_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_REQUEST_OPERATE_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<CL2GS_REQUEST_OPERATE_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED CL2GS_REQUEST_FOLLOW_DATA
{
public:
	mu_int16 Key;
};
#pragma pack(pop)

class CL2GS_REQUEST_FOLLOW : public MU_CLIENT_TO_SERVER_HEADEREX, public CL2GS_REQUEST_FOLLOW_DATA
{
public:
	CL2GS_REQUEST_FOLLOW()
	{
		Code = GS2CL_PROT_FOLLOW;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_REQUEST_FOLLOW_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_REQUEST_FOLLOW_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<CL2GS_REQUEST_FOLLOW_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED CL2GS_REQUEST_ATTACK_DATA
{
public:
	mu_int16 Key;
};
#pragma pack(pop)

class CL2GS_REQUEST_ATTACK : public MU_CLIENT_TO_SERVER_HEADEREX, public CL2GS_REQUEST_ATTACK_DATA
{
public:
	CL2GS_REQUEST_ATTACK()
	{
		Code = GS2CL_PROT_ATTACK;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_REQUEST_ATTACK_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_REQUEST_ATTACK_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<CL2GS_REQUEST_ATTACK_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED CL2GS_REQUEST_MAGIC_DATA
{
public:
	mu_int16 Key;
	mu_uint16 Magic;
	mu_uint8 X;
	mu_uint8 Y;
	mu_float Direction;
};
#pragma pack(pop)

class CL2GS_REQUEST_MAGIC : public MU_CLIENT_TO_SERVER_HEADEREX, public CL2GS_REQUEST_MAGIC_DATA
{
public:
	CL2GS_REQUEST_MAGIC()
	{
		Code = GS2CL_PROT_MAGIC;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_REQUEST_MAGIC_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_REQUEST_MAGIC_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<CL2GS_REQUEST_MAGIC_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED CL2GS_REQUEST_TALK_DATA
{
public:
	mu_int16 Key;
};
#pragma pack(pop)

class CL2GS_REQUEST_TALK : public MU_CLIENT_TO_SERVER_HEADEREX, public CL2GS_REQUEST_TALK_DATA
{
public:
	CL2GS_REQUEST_TALK()
	{
		Code = GS2CL_PROT_TALK;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_REQUEST_TALK_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_REQUEST_TALK_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<CL2GS_REQUEST_TALK_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_RESPONSE_TALK_DATA
{
public:
	mu_uint8 Response;
};
#pragma pack(pop)

class GS2CL_RESPONSE_TALK_DATAEX
{
public:
	std::vector<mu_uint8> TalkData;
};

class GS2CL_RESPONSE_TALK : public MU_SERVER_TO_CLIENT_HEADEREX, public GS2CL_RESPONSE_TALK_DATA, public GS2CL_RESPONSE_TALK_DATAEX
{
public:
	GS2CL_RESPONSE_TALK()
	{
		Code = GS2CL_PROT_TALK;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RESPONSE_TALK_DATA*>(this));
		PARSER_WRITEVECTOR(TalkData);

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RESPONSE_TALK_DATA*>(this));
		PARSER_WRITEVECTOR(TalkData);

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_RESPONSE_TALK_DATA*>(this));
		PARSER_READVECTOR(TalkData);

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_RESPONSE_WEAPONSTATE_DATA
{
public:
	mu_uint8 Result;
	mu_uint8 WeaponType;
	mu_int16 Key;
};
#pragma pack(pop)

class GS2CL_RESPONSE_WEAPONSTATE : public MU_SERVER_TO_CLIENT_HEADEREX, public GS2CL_RESPONSE_WEAPONSTATE_DATA
{
public:
	GS2CL_RESPONSE_WEAPONSTATE()
	{
		Code = GS2CL_PROT_WEAPONSTATE;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RESPONSE_WEAPONSTATE_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RESPONSE_WEAPONSTATE_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_RESPONSE_WEAPONSTATE_DATA*>(this));

		return true;
	}
};

class CL2GS_REQUEST_CANCEL : public MU_CLIENT_TO_SERVER_HEADEREX
{
public:
	CL2GS_REQUEST_CANCEL()
	{
		Code = GS2CL_PROT_CANCEL;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_RECEIVE_DAMAGE_DATA
{
public:
	mu_int16 Key;
	mu_int32 Damage;
	mu_int32 ShieldDamage;
	mu_boolean MSBFlag;
	mu_uint8 DamageType;
};
#pragma pack(pop)

class GS2CL_RECEIVE_DAMAGE : public MU_SERVER_TO_CLIENT_HEADEREX, public GS2CL_RECEIVE_DAMAGE_DATA
{
public:
	GS2CL_RECEIVE_DAMAGE()
	{
		Code = GS2CL_PROT_DAMAGE;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_DAMAGE_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_DAMAGE_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_RECEIVE_DAMAGE_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_RECEIVE_SIMPLEDAMAGE_DATA
{
public:
	mu_int32 Damage;
	mu_int32 ShieldDamage;
};
#pragma pack(pop)

class GS2CL_RECEIVE_SIMPLEDAMAGE : public MU_SERVER_TO_CLIENT_HEADEREX, public GS2CL_RECEIVE_SIMPLEDAMAGE_DATA
{
public:
	GS2CL_RECEIVE_SIMPLEDAMAGE()
	{
		Code = GS2CL_PROT_SIMPLEDAMAGE;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_SIMPLEDAMAGE_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_SIMPLEDAMAGE_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_RECEIVE_SIMPLEDAMAGE_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_RECEIVE_KILLPLAYER_DATA
{
public:
	mu_int16 Key;
	mu_int64 Experience;
	mu_int32 Damage;
	mu_boolean MSBFlag;
};
#pragma pack(pop)

class GS2CL_RECEIVE_KILLPLAYER : public MU_SERVER_TO_CLIENT_HEADEREX, public GS2CL_RECEIVE_KILLPLAYER_DATA
{
public:
	GS2CL_RECEIVE_KILLPLAYER()
	{
		Code = GS2CL_PROT_KILLPLAYER;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_KILLPLAYER_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_KILLPLAYER_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_RECEIVE_KILLPLAYER_DATA*>(this));

		return true;
	}
};

class CL2GS_REQUEST_CLOSEWINDOW : public MU_CLIENT_TO_SERVER_HEADEREX
{
public:
	CL2GS_REQUEST_CLOSEWINDOW()
	{
		Code = GS2CL_PROT_CLOSEWINDOW;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_RECEIVE_ITEMINSERT_DATA
{
public:
	SHARED::ITEM_DATA ItemData;
};
#pragma pack(pop)

class GS2CL_RECEIVE_ITEMINSERT : public MU_SERVER_TO_CLIENT_HEADEREX, public GS2CL_RECEIVE_ITEMINSERT_DATA
{
public:
	GS2CL_RECEIVE_ITEMINSERT()
	{
		Code = GS2CL_PROT_ITEMINSERT;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_ITEMINSERT_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_ITEMINSERT_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_RECEIVE_ITEMINSERT_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_RECEIVE_ITEMDELETE_DATA
{
public:
	mu_uint8 Type;
	mu_int16 Slot;
};
#pragma pack(pop)

class GS2CL_RECEIVE_ITEMDELETE : public MU_SERVER_TO_CLIENT_HEADEREX, public GS2CL_RECEIVE_ITEMDELETE_DATA
{
public:
	GS2CL_RECEIVE_ITEMDELETE()
	{
		Code = GS2CL_PROT_ITEMDELETE;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_ITEMDELETE_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_ITEMDELETE_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_RECEIVE_ITEMDELETE_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_RECEIVE_ITEMMODIFY_DATA
{
public:
	SHARED::ITEM_DATA ItemData;
};
#pragma pack(pop)

class GS2CL_RECEIVE_ITEMMODIFY : public MU_SERVER_TO_CLIENT_HEADEREX, public GS2CL_RECEIVE_ITEMMODIFY_DATA
{
public:
	GS2CL_RECEIVE_ITEMMODIFY()
	{
		Code = GS2CL_PROT_ITEMMODIFY;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_ITEMMODIFY_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_ITEMMODIFY_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_RECEIVE_ITEMMODIFY_DATA*>(this));

		return true;
	}
};

class GS2CL_RECEIVE_SHOPLIST_DATA
{
public:
	std::vector<SHARED::ITEM_DATA> Items;
};

class GS2CL_RECEIVE_SHOPLIST : public MU_SERVER_TO_CLIENT_HEADEREX, public GS2CL_RECEIVE_SHOPLIST_DATA
{
public:
	GS2CL_RECEIVE_SHOPLIST()
	{
		Code = GS2CL_PROT_SHOPLIST;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITEVECTOR(Items);

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITEVECTOR(Items);

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_READVECTOR(Items);

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_RECEIVE_TAXRATE_DATA
{
public:
	mu_uint8 Type;
	mu_int32 TaxRate;
};
#pragma pack(pop)

class GS2CL_RECEIVE_TAXRATE : public MU_SERVER_TO_CLIENT_HEADEREX, public GS2CL_RECEIVE_TAXRATE_DATA
{
public:
	enum
	{
		TAXRATE_CHAOS,
		TAXRATE_SHOP,
	};
public:
	GS2CL_RECEIVE_TAXRATE()
	{
		Code = GS2CL_PROT_TAXRATE;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_TAXRATE_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_TAXRATE_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_RECEIVE_TAXRATE_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_RECEIVE_ITEMDURABILITY_DATA
{
public:
	mu_uint8 Type;
	mu_int16 Slot;
	mu_uint8 Durability;
};
#pragma pack(pop)

class GS2CL_RECEIVE_ITEMDURABILITY : public MU_SERVER_TO_CLIENT_HEADEREX, public GS2CL_RECEIVE_ITEMDURABILITY_DATA
{
public:
	GS2CL_RECEIVE_ITEMDURABILITY()
	{
		Code = GS2CL_PROT_ITEMDURABILITY;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_ITEMDURABILITY_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_ITEMDURABILITY_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_RECEIVE_ITEMDURABILITY_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_RECEIVE_ITEMMOVERESULT_DATA
{
public:
	mu_uint8 Type;
	mu_uint8 X;
	mu_uint8 Y;
	mu_boolean Result;
};
#pragma pack(pop)

class GS2CL_RECEIVE_ITEMMOVERESULT : public MU_SERVER_TO_CLIENT_HEADEREX, public GS2CL_RECEIVE_ITEMMOVERESULT_DATA
{
public:
	GS2CL_RECEIVE_ITEMMOVERESULT()
	{
		Code = GS2CL_PROT_ITEMMOVERESULT;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_ITEMMOVERESULT_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_ITEMMOVERESULT_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_RECEIVE_ITEMMOVERESULT_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_RECEIVE_RELOADARROW_DATA
{
public:
	mu_boolean Success;
};
#pragma pack(pop)

class GS2CL_RECEIVE_RELOADARROW : public MU_SERVER_TO_CLIENT_HEADEREX, public GS2CL_RECEIVE_RELOADARROW_DATA
{
public:
	GS2CL_RECEIVE_RELOADARROW()
	{
		Code = GS2CL_PROT_RELOADARROW;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_RELOADARROW_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_RELOADARROW_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_RECEIVE_RELOADARROW_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_RECEIVE_SERVERCOMMAND_DATA
{
public:
	mu_uint8 CommandType;
	mu_uint8 CommandX;
	mu_uint8 CommandY;
	mu_int16 Key;
};
#pragma pack(pop)

class GS2CL_RECEIVE_SERVERCOMMAND : public MU_SERVER_TO_CLIENT_HEADEREX, public GS2CL_RECEIVE_SERVERCOMMAND_DATA
{
public:
	GS2CL_RECEIVE_SERVERCOMMAND()
	{
		Code = GS2CL_PROT_SERVERCOMMAND;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_SERVERCOMMAND_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_SERVERCOMMAND_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_RECEIVE_SERVERCOMMAND_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED CL2GS_REQUEST_MOVECOMMAND_DATA
{
public:
	mu_int32 MoveIndex;
};
#pragma pack(pop)

class CL2GS_REQUEST_MOVECOMMAND : public MU_CLIENT_TO_SERVER_HEADEREX, public CL2GS_REQUEST_MOVECOMMAND_DATA
{
public:
	CL2GS_REQUEST_MOVECOMMAND()
	{
		Code = GS2CL_PROT_MOVECOMMAND;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_REQUEST_MOVECOMMAND_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_REQUEST_MOVECOMMAND_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<CL2GS_REQUEST_MOVECOMMAND_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_RECEIVE_SETATTRIBUTE_DATA
{
public:
	mu_uint8 Type;
	mu_uint8 MapAttr;
	mu_uint8 MapSetType;
};
#pragma pack(pop)

class GS2CL_RECEIVE_SETATTRIBUTE_DATAEX
{
public:
#pragma pack(push, 1)
	struct ARM_PACKED PositionData
	{
		mu_uint8 x;
		mu_uint8 y;
	};
#pragma pack(pop)

public:
	std::vector<PositionData> Positions;
};

class GS2CL_RECEIVE_SETATTRIBUTE : public MU_SERVER_TO_CLIENT_HEADEREX, public GS2CL_RECEIVE_SETATTRIBUTE_DATA, public GS2CL_RECEIVE_SETATTRIBUTE_DATAEX
{
public:
	GS2CL_RECEIVE_SETATTRIBUTE()
	{
		Code = GS2CL_PROT_SETATTRIBUTE;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_SETATTRIBUTE_DATA*>(this));
		PARSER_WRITEVECTOR(Positions);

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_SETATTRIBUTE_DATA*>(this));
		PARSER_WRITEVECTOR(Positions);

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_RECEIVE_SETATTRIBUTE_DATA*>(this));
		PARSER_READVECTOR(Positions);

		return true;
	}
};

// Viewport Packets
#pragma pack(push, 1)
class ARM_PACKED GS2CL_VIEWPORT_PACKAGE_DATA
{
public:
	mu_boolean Compressed;
	mu_uint32 PackageSize;
	mu_uint32 CompressedSize;
};
#pragma pack(pop)

class GS2CL_VIEWPORT_PACKAGE : public MU_SERVER_TO_CLIENT_HEADEREX, public GS2CL_VIEWPORT_PACKAGE_DATA
{
public:
	GS2CL_VIEWPORT_PACKAGE()
	{
		Code = GS2CL_PROT_VIEWPORT_PACKAGE;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_PACKAGE_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_PACKAGE_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_VIEWPORT_PACKAGE_DATA*>(this));

		if (Compressed == true)
		{
			if (CompressedSize > bufferLength)
			{
				return false;
			}
		}
		else
		{
			if (PackageSize > bufferLength)
			{
				return false;
			}
		}

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_VIEWPORT_CREATECHARACTER_HEADER
{
public:
	mu_int16 Key;
	mu_uint8 Class;
	mu_uint8 SubClass;
	mu_uint8 PkLevel;
	mu_uint8 PositionX;
	mu_uint8 PositionY;
	mu_boolean Create;
	mu_uint16 DarkRavenLevel;
	mu_float Direction;
};
#pragma pack(pop)

class GS2CL_VIEWPORT_CREATECHARACTER_HEADEREX
{
public:
	SHARED::CHARSET_PACKET Equipment[GAME::MAX_EQUIPMENT];
	mu_string Name;
};

class GS2CL_VIEWPORT_CREATECHARACTER : public MU_VIEWPORT_HEADEREX,
	public GS2CL_VIEWPORT_CREATECHARACTER_HEADER,
	public GS2CL_VIEWPORT_CREATECHARACTER_HEADEREX
{
public:
	GS2CL_VIEWPORT_CREATECHARACTER()
	{
		Code = GS2CL_PROT_VIEWPORT_CREATECHARACTER;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_CREATECHARACTER_HEADER*>(this));

		for (mu_uint32 n = 0; n < GAME::MAX_EQUIPMENT; ++n)
		{
			PARSER_WRITECHARSET(Equipment[n]);
		}

		PARSER_WRITESTRING(Name, CHARACTER_MAXIMUM);

		return parser.ProcessedLength;
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		Size = bufferLength;
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_CREATECHARACTER_HEADER*>(this));

		for (mu_uint32 n = 0; n < GAME::MAX_EQUIPMENT; ++n)
		{
			PARSER_WRITECHARSET(Equipment[n]);
		}

		PARSER_WRITESTRING(Name, CHARACTER_MAXIMUM);

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_VIEWPORT_CREATECHARACTER_HEADER*>(this));

		for (mu_uint32 n = 0; n < GAME::MAX_EQUIPMENT; ++n)
		{
			PARSER_READCHARSET(Equipment[n]);
		}

		PARSER_READSTRING(Name, CHARACTER_MAXIMUM);

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_VIEWPORT_DELETECHARACTER_DATA
{
public:
	mu_int16 Key;
};
#pragma pack(pop)

class GS2CL_VIEWPORT_DELETECHARACTER : public MU_VIEWPORT_HEADEREX, public GS2CL_VIEWPORT_DELETECHARACTER_DATA
{
public:
	GS2CL_VIEWPORT_DELETECHARACTER()
	{
		Code = GS2CL_PROT_VIEWPORT_DELETECHARACTER;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_DELETECHARACTER_DATA*>(this));

		return parser.ProcessedLength;
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		Size = bufferLength;
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_DELETECHARACTER_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_VIEWPORT_DELETECHARACTER_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_VIEWPORT_CREATEMONEY_DATA
{
public:
	mu_int16 Key;
	mu_boolean Create;
	mu_uint8 X, Y;
	mu_int64 Money;
};
#pragma pack(pop)

class GS2CL_VIEWPORT_CREATEMONEY : public MU_VIEWPORT_HEADEREX, public GS2CL_VIEWPORT_CREATEMONEY_DATA
{
public:
	GS2CL_VIEWPORT_CREATEMONEY()
	{
		Code = GS2CL_PROT_VIEWPORT_CREATEMONEY;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_CREATEMONEY_DATA*>(this));

		return parser.ProcessedLength;
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		Size = bufferLength;
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_CREATEMONEY_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_VIEWPORT_CREATEMONEY_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_VIEWPORT_CREATEITEM_DATA
{
public:
	mu_int16 Key;
	mu_boolean Create;
	SHARED::ITEM_DATA ItemData;
};
#pragma pack(pop)

class GS2CL_VIEWPORT_CREATEITEM : public MU_VIEWPORT_HEADEREX, public GS2CL_VIEWPORT_CREATEITEM_DATA
{
public:
	GS2CL_VIEWPORT_CREATEITEM()
	{
		Code = GS2CL_PROT_VIEWPORT_CREATEITEM;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_CREATEITEM_DATA*>(this));

		return parser.ProcessedLength;
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		Size = bufferLength;
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_CREATEITEM_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_VIEWPORT_CREATEITEM_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_VIEWPORT_DELETEITEM_DATA
{
public:
	mu_int16 Key;
};
#pragma pack(pop)

class GS2CL_VIEWPORT_DELETEITEM : public MU_VIEWPORT_HEADEREX, public GS2CL_VIEWPORT_DELETEITEM_DATA
{
public:
	GS2CL_VIEWPORT_DELETEITEM()
	{
		Code = GS2CL_PROT_VIEWPORT_DELETEITEM;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_DELETEITEM_DATA*>(this));

		return parser.ProcessedLength;
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		Size = bufferLength;
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_DELETEITEM_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_VIEWPORT_DELETEITEM_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_VIEWPORT_CREATEMONSTER_DATA
{
public:
	mu_int16 Key;
	mu_int16 Class;
	mu_uint8 PositionX;
	mu_uint8 PositionY;
	mu_boolean Create;
	mu_boolean Teleport;
	mu_float Direction;
};
#pragma pack(pop)

class GS2CL_VIEWPORT_CREATEMONSTER : public MU_VIEWPORT_HEADEREX, public GS2CL_VIEWPORT_CREATEMONSTER_DATA
{
public:
	GS2CL_VIEWPORT_CREATEMONSTER()
	{
		Code = GS2CL_PROT_VIEWPORT_CREATEMONSTER;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_CREATEMONSTER_DATA*>(this));

		return parser.ProcessedLength;
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		Size = bufferLength;
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_CREATEMONSTER_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_VIEWPORT_CREATEMONSTER_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_VIEWPORT_DELETEMONSTER_DATA
{
public:
	mu_int16 Key;
};
#pragma pack(pop)

class GS2CL_VIEWPORT_DELETEMONSTER : public MU_VIEWPORT_HEADEREX, public GS2CL_VIEWPORT_DELETEMONSTER_DATA
{
public:
	GS2CL_VIEWPORT_DELETEMONSTER()
	{
		Code = GS2CL_PROT_VIEWPORT_DELETEMONSTER;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_DELETEMONSTER_DATA*>(this));

		return parser.ProcessedLength;
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		Size = bufferLength;
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_DELETEMONSTER_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_VIEWPORT_DELETEMONSTER_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_VIEWPORT_CREATESUMMON_DATA
{
public:
	mu_int16 Key;
	mu_int16 Class;
	mu_uint8 PositionX;
	mu_uint8 PositionY;
	mu_boolean Create;
	mu_boolean Teleport;
	mu_float Direction;
};
#pragma pack(pop)

class GS2CL_VIEWPORT_CREATESUMMON_DATAEX
{
public:
	mu_string Name;
};

class GS2CL_VIEWPORT_CREATESUMMON : public MU_VIEWPORT_HEADEREX, public GS2CL_VIEWPORT_CREATESUMMON_DATA, public GS2CL_VIEWPORT_CREATESUMMON_DATAEX
{
public:
	GS2CL_VIEWPORT_CREATESUMMON()
	{
		Code = GS2CL_PROT_VIEWPORT_CREATESUMMON;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_CREATESUMMON_DATA*>(this));
		PARSER_WRITESTRING(Name, CHARACTER_MAXIMUM);

		return parser.ProcessedLength;
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		Size = bufferLength;
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_CREATESUMMON_DATA*>(this));
		PARSER_WRITESTRING(Name, CHARACTER_MAXIMUM);

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_VIEWPORT_CREATESUMMON_DATA*>(this));
		PARSER_READSTRING(Name, CHARACTER_MAXIMUM);

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_VIEWPORT_CHARSETCHANGE_HEADER
{
public:
	mu_int16 Key;
	mu_uint8 Class;
	mu_uint8 SubClass;
	mu_uint8 PkLevel;
	mu_uint16 DarkRavenLevel;
};
#pragma pack(pop)

class GS2CL_VIEWPORT_CHARSETCHANGE_HEADEREX
{
public:
	SHARED::CHARSET_PACKET Equipment[GAME::MAX_EQUIPMENT];
};

class GS2CL_VIEWPORT_CHARSETCHANGE  : public MU_VIEWPORT_HEADEREX,
	public GS2CL_VIEWPORT_CHARSETCHANGE_HEADER,
	public GS2CL_VIEWPORT_CHARSETCHANGE_HEADEREX
{
public:
	GS2CL_VIEWPORT_CHARSETCHANGE()
	{
		Code = GS2CL_PROT_VIEWPORT_CHARSETCHANGE;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_CHARSETCHANGE_HEADER*>(this));

		for (mu_uint32 n = 0; n < GAME::MAX_EQUIPMENT; ++n)
		{
			PARSER_WRITECHARSET(Equipment[n]);
		}

		return parser.ProcessedLength;
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		Size = bufferLength;
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_CHARSETCHANGE_HEADER*>(this));

		for (mu_uint32 n = 0; n < GAME::MAX_EQUIPMENT; ++n)
		{
			PARSER_WRITECHARSET(Equipment[n]);
		}

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_VIEWPORT_CHARSETCHANGE_HEADER*>(this));

		for (mu_uint32 n = 0; n < GAME::MAX_EQUIPMENT; ++n)
		{
			PARSER_READCHARSET(Equipment[n]);
		}

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_VIEWPORT_FORCEPOSITION_DATA
{
public:
	mu_int16 Key;
	mu_uint8 X;
	mu_uint8 Y;
	mu_boolean IsTeleport;
};
#pragma pack(pop)

class GS2CL_VIEWPORT_FORCEPOSITION : public MU_VIEWPORT_HEADEREX, public GS2CL_VIEWPORT_FORCEPOSITION_DATA
{
public:
	GS2CL_VIEWPORT_FORCEPOSITION()
	{
		Code = GS2CL_PROT_VIEWPORT_FORCEPOSITION;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_FORCEPOSITION_DATA*>(this));

		return parser.ProcessedLength;
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		Size = bufferLength;
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_FORCEPOSITION_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_VIEWPORT_FORCEPOSITION_DATA*>(this));

		return true;
	}
};

class GS2CL_VIEWPORT_REFRESH : public MU_VIEWPORT_HEADEREX
{
public:
	GS2CL_VIEWPORT_REFRESH()
	{
		Code = GS2CL_PROT_VIEWPORT_REFRESH;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));

		return parser.ProcessedLength;
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		Size = bufferLength;
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_VIEWPORT_HEADER*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_VIEWPORT_STAND_DATA
{
public:
	mu_int16 Key;
	mu_float Dir;
	mu_uint8 StormTime;
};
#pragma pack(pop)

class GS2CL_VIEWPORT_STAND : public MU_VIEWPORT_HEADEREX, public GS2CL_VIEWPORT_STAND_DATA
{
public:
	GS2CL_VIEWPORT_STAND()
	{
		Code = GS2CL_PROT_VIEWPORT_STAND;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_STAND_DATA*>(this));

		return parser.ProcessedLength;
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		Size = bufferLength;
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_STAND_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_VIEWPORT_STAND_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_VIEWPORT_ANIMATION_DATA
{
public:
	mu_int16 Key;
	mu_uint16 Animation;
	mu_uint8 SubAnimation;
	mu_float Dir;
};
#pragma pack(pop)

class GS2CL_VIEWPORT_ANIMATION : public MU_VIEWPORT_HEADEREX, public GS2CL_VIEWPORT_ANIMATION_DATA
{
public:
	GS2CL_VIEWPORT_ANIMATION()
	{
		Code = GS2CL_PROT_VIEWPORT_ANIMATION;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_ANIMATION_DATA*>(this));

		return parser.ProcessedLength;
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		Size = bufferLength;
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_ANIMATION_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_VIEWPORT_ANIMATION_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_VIEWPORT_MOVE_DATA
{
public:
	mu_int16 Key;
	mu_uint8 X;
	mu_uint8 Y;
	mu_uint8 Run;
	mu_float Dir;
};
#pragma pack(pop)

class GS2CL_VIEWPORT_MOVE : public MU_VIEWPORT_HEADEREX, public GS2CL_VIEWPORT_MOVE_DATA
{
public:
	GS2CL_VIEWPORT_MOVE()
	{
		Code = GS2CL_PROT_VIEWPORT_MOVE;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_MOVE_DATA*>(this));

		return parser.ProcessedLength;
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		Size = bufferLength;
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_MOVE_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_VIEWPORT_MOVE_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_VIEWPORT_REGEN_DATA
{
public:
	mu_uint8 MapNumber;
	mu_uint8 X;
	mu_uint8 Y;
	mu_float Dir;
	mu_uint32 EventMask;
};
#pragma pack(pop)

class GS2CL_VIEWPORT_REGEN : public MU_VIEWPORT_HEADEREX, public GS2CL_VIEWPORT_REGEN_DATA
{
public:
	GS2CL_VIEWPORT_REGEN()
	{
		Code = GS2CL_PROT_VIEWPORT_REGEN;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_REGEN_DATA*>(this));

		return parser.ProcessedLength;
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		Size = bufferLength;
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_REGEN_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_VIEWPORT_REGEN_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_VIEWPORT_ATTACK_DATA
{
public:
	mu_int16 Key;
	mu_int16 TargetKey;
	mu_uint8 X;
	mu_uint8 Y;
	mu_float Dir;
};
#pragma pack(pop)

class GS2CL_VIEWPORT_ATTACK : public MU_VIEWPORT_HEADEREX, public GS2CL_VIEWPORT_ATTACK_DATA
{
public:
	GS2CL_VIEWPORT_ATTACK()
	{
		Code = GS2CL_PROT_VIEWPORT_ATTACK;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_ATTACK_DATA*>(this));

		return parser.ProcessedLength;
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		Size = bufferLength;
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_ATTACK_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_VIEWPORT_ATTACK_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_VIEWPORT_MAGIC_DATA
{
public:
	mu_int16 Key;
	mu_int16 TargetKey;
	mu_uint16 MagicNumber;
	mu_boolean SkillSuccess;
	mu_uint8 X;
	mu_uint8 Y;
	mu_float Dir;
};
#pragma pack(pop)

class GS2CL_VIEWPORT_MAGIC_DATAEX
{
public:
	std::vector<mu_int16> TargetsList;
};

class GS2CL_VIEWPORT_MAGIC : public MU_VIEWPORT_HEADEREX, public GS2CL_VIEWPORT_MAGIC_DATA, public GS2CL_VIEWPORT_MAGIC_DATAEX
{
public:
	GS2CL_VIEWPORT_MAGIC()
	{
		Code = GS2CL_PROT_VIEWPORT_MAGIC;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_MAGIC_DATA*>(this));
		PARSER_WRITEVECTOR(TargetsList);

		return parser.ProcessedLength;
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		Size = bufferLength;
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_MAGIC_DATA*>(this));
		PARSER_WRITEVECTOR(TargetsList);

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_VIEWPORT_MAGIC_DATA*>(this));
		PARSER_READVECTOR(TargetsList);

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_VIEWPORT_MAGIC2_DATA
{
public:
	mu_int16 Key;
	mu_int16 TargetKey;
	mu_uint16 MagicNumber;
	mu_uint8 PositionX;
	mu_uint8 PositionY;
	mu_uint8 X;
	mu_uint8 Y;
	mu_float Dir;
};
#pragma pack(pop)

class GS2CL_VIEWPORT_MAGIC2_DATAEX
{
public:
	std::vector<mu_int16> TargetsList;
};

class GS2CL_VIEWPORT_MAGIC2 : public MU_VIEWPORT_HEADEREX, public GS2CL_VIEWPORT_MAGIC2_DATA, public GS2CL_VIEWPORT_MAGIC2_DATAEX
{
public:
	GS2CL_VIEWPORT_MAGIC2()
	{
		Code = GS2CL_PROT_VIEWPORT_MAGIC2;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_MAGIC2_DATA*>(this));
		PARSER_WRITEVECTOR(TargetsList);

		return parser.ProcessedLength;
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		Size = bufferLength;
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_MAGIC2_DATA*>(this));
		PARSER_WRITEVECTOR(TargetsList);

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_VIEWPORT_MAGIC2_DATA*>(this));
		PARSER_READVECTOR(TargetsList);

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_VIEWPORT_DIE_DATA
{
public:
	mu_int16 Key;
	mu_int16 KillerKey;
	mu_uint16 Skill;
};
#pragma pack(pop)

class GS2CL_VIEWPORT_DIE : public MU_VIEWPORT_HEADEREX, public GS2CL_VIEWPORT_DIE_DATA
{
public:
	GS2CL_VIEWPORT_DIE()
	{
		Code = GS2CL_PROT_VIEWPORT_DIE;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_DIE_DATA*>(this));

		return parser.ProcessedLength;
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		Size = bufferLength;
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_DIE_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_VIEWPORT_DIE_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_VIEWPORT_TELEPORTEND_DATA
{
public:
	mu_int16 Key;
};
#pragma pack(pop)

class GS2CL_VIEWPORT_TELEPORTEND : public MU_VIEWPORT_HEADEREX, public GS2CL_VIEWPORT_TELEPORTEND_DATA
{
public:
	GS2CL_VIEWPORT_TELEPORTEND()
	{
		Code = GS2CL_PROT_VIEWPORT_TELEPORTEND;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_TELEPORTEND_DATA*>(this));

		return parser.ProcessedLength;
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		Size = bufferLength;
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_TELEPORTEND_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_VIEWPORT_TELEPORTEND_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_VIEWPORT_WEATHER_DATA
{
public:
	mu_uint8 Weather;
	mu_uint8 WeatherVariation;
};
#pragma pack(pop)

class GS2CL_VIEWPORT_WEATHER : public MU_VIEWPORT_HEADEREX, public GS2CL_VIEWPORT_WEATHER_DATA
{
public:
	GS2CL_VIEWPORT_WEATHER()
	{
		Code = GS2CL_PROT_VIEWPORT_WEATHER;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_WEATHER_DATA*>(this));

		return parser.ProcessedLength;
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		Size = bufferLength;
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_WEATHER_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_VIEWPORT_WEATHER_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_VIEWPORT_EVENTMASK_DATA
{
public:
	mu_uint32 EventMask;
};
#pragma pack(pop)

class GS2CL_VIEWPORT_EVENTMASK : public MU_VIEWPORT_HEADEREX, public GS2CL_VIEWPORT_EVENTMASK_DATA
{
public:
	GS2CL_VIEWPORT_EVENTMASK()
	{
		Code = GS2CL_PROT_VIEWPORT_EVENTMASK;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_EVENTMASK_DATA*>(this));

		return parser.ProcessedLength;
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		Size = bufferLength;
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_EVENTMASK_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_VIEWPORT_EVENTMASK_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_VIEWPORT_LIFE_DATA
{
public:
	mu_int16 Key;
	mu_float Life;
	mu_float MaxLife;
	mu_float Shield;
	mu_float MaxShield;
};
#pragma pack(pop)

class GS2CL_VIEWPORT_LIFE : public MU_VIEWPORT_HEADEREX, public GS2CL_VIEWPORT_LIFE_DATA
{
public:
	GS2CL_VIEWPORT_LIFE()
	{
		Code = GS2CL_PROT_VIEWPORT_LIFE;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_LIFE_DATA*>(this));

		return parser.ProcessedLength;
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		Size = bufferLength;
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_LIFE_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_VIEWPORT_LIFE_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_VIEWPORT_MANA_DATA
{
public:
	mu_int16 Key;
	mu_float Mana;
	mu_float MaxMana;
	mu_float BP;
	mu_float MaxBP;
};
#pragma pack(pop)

class GS2CL_VIEWPORT_MANA : public MU_VIEWPORT_HEADEREX, public GS2CL_VIEWPORT_MANA_DATA
{
public:
	GS2CL_VIEWPORT_MANA()
	{
		Code = GS2CL_PROT_VIEWPORT_MANA;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_MANA_DATA*>(this));

		return parser.ProcessedLength;
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		Size = bufferLength;
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_MANA_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_VIEWPORT_MANA_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_VIEWPORT_LEVELUP_DATA
{
public:
	mu_int16 Key;
};
#pragma pack(pop)

class GS2CL_VIEWPORT_LEVELUP : public MU_VIEWPORT_HEADEREX, public GS2CL_VIEWPORT_LEVELUP_DATA
{
public:
	GS2CL_VIEWPORT_LEVELUP()
	{
		Code = GS2CL_PROT_VIEWPORT_LEVELUP;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_LEVELUP_DATA*>(this));

		return parser.ProcessedLength;
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		Size = bufferLength;
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_LEVELUP_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_VIEWPORT_LEVELUP_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_VIEWPORT_SHIELDBROKE_DATA
{
public:
	mu_int16 Key;
};
#pragma pack(pop)

class GS2CL_VIEWPORT_SHIELDBROKE : public MU_VIEWPORT_HEADEREX, public GS2CL_VIEWPORT_SHIELDBROKE_DATA
{
public:
	GS2CL_VIEWPORT_SHIELDBROKE()
	{
		Code = GS2CL_PROT_VIEWPORT_SHIELDBROKE;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_SHIELDBROKE_DATA*>(this));

		return parser.ProcessedLength;
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		Size = bufferLength;
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_SHIELDBROKE_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_VIEWPORT_SHIELDBROKE_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_VIEWPORT_POTIONSD_DATA
{
public:
	mu_int16 Key;
};
#pragma pack(pop)

class GS2CL_VIEWPORT_POTIONSD : public MU_VIEWPORT_HEADEREX, public GS2CL_VIEWPORT_POTIONSD_DATA
{
public:
	GS2CL_VIEWPORT_POTIONSD()
	{
		Code = GS2CL_PROT_VIEWPORT_POTIONSD;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_POTIONSD_DATA*>(this));

		return parser.ProcessedLength;
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		Size = bufferLength;
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_POTIONSD_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_VIEWPORT_POTIONSD_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_VIEWPORT_QUESTEFFECT_DATA
{
public:
	mu_int16 Key;
	mu_uint16 EffectType;
};
#pragma pack(pop)

class GS2CL_VIEWPORT_QUESTEFFECT : public MU_VIEWPORT_HEADEREX, public GS2CL_VIEWPORT_QUESTEFFECT_DATA
{
public:
	GS2CL_VIEWPORT_QUESTEFFECT()
	{
		Code = GS2CL_PROT_VIEWPORT_QUESTEFFECT;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_QUESTEFFECT_DATA*>(this));

		return parser.ProcessedLength;
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		Size = bufferLength;
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_QUESTEFFECT_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_VIEWPORT_QUESTEFFECT_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_VIEWPORT_QUESTHISTORY_DATA
{
public:
	mu_uint8 Quest[GAME::MAX_DBQUEST];
};
#pragma pack(pop)

class GS2CL_VIEWPORT_QUESTHISTORY : public MU_VIEWPORT_HEADEREX, public GS2CL_VIEWPORT_QUESTHISTORY_DATA
{
public:
	GS2CL_VIEWPORT_QUESTHISTORY()
	{
		Code = GS2CL_PROT_VIEWPORT_QUESTHISTORY;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_QUESTHISTORY_DATA*>(this));

		return parser.ProcessedLength;
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		Size = bufferLength;
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_QUESTHISTORY_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_VIEWPORT_QUESTHISTORY_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_VIEWPORT_MAGICLIST_DATA
{
public:
	mu_uint8 MagicCount;
	struct MAGIC_DATA
	{
		mu_uint16 SkillNum;
		mu_uint8 Pos;
	} Magics[MAX_MAGIC];
};
#pragma pack(pop)

class GS2CL_VIEWPORT_MAGICLIST : public MU_VIEWPORT_HEADEREX, public GS2CL_VIEWPORT_MAGICLIST_DATA
{
public:
	GS2CL_VIEWPORT_MAGICLIST()
	{
		Code = GS2CL_PROT_VIEWPORT_MAGICLIST;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(MagicCount);
		PARSER_WRITEARRAY(Magics, MagicCount);

		return parser.ProcessedLength;
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		Size = bufferLength;
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(MagicCount);
		PARSER_WRITEARRAY(Magics, MagicCount);

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_READ(MagicCount);
		PARSER_READARRAY(Magics, MagicCount);

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_VIEWPORT_EXPERIENCE_DATA
{
public:
	mu_int16 Level;
	mu_int16 MasterLevel;
	mu_int64 Experience;
	mu_int64 NextExperience;
};
#pragma pack(pop)

class GS2CL_VIEWPORT_EXPERIENCE : public MU_VIEWPORT_HEADEREX, public GS2CL_VIEWPORT_EXPERIENCE_DATA
{
public:
	GS2CL_VIEWPORT_EXPERIENCE()
	{
		Code = GS2CL_PROT_VIEWPORT_EXPERIENCE;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_EXPERIENCE_DATA*>(this));

		return parser.ProcessedLength;
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		Size = bufferLength;
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_EXPERIENCE_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_VIEWPORT_EXPERIENCE_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_VIEWPORT_LEVELUPPOINTS_DATA
{
public:
	mu_int32 LevelUpPoints;
	mu_int32 MasterLevelUpPoints;
};
#pragma pack(pop)

class GS2CL_VIEWPORT_LEVELUPPOINTS : public MU_VIEWPORT_HEADEREX, public GS2CL_VIEWPORT_LEVELUPPOINTS_DATA
{
public:
	GS2CL_VIEWPORT_LEVELUPPOINTS()
	{
		Code = GS2CL_PROT_VIEWPORT_LEVELUPPOINTS;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_LEVELUPPOINTS_DATA*>(this));

		return parser.ProcessedLength;
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		Size = bufferLength;
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_LEVELUPPOINTS_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_VIEWPORT_LEVELUPPOINTS_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_VIEWPORT_STATS_DATA
{
public:
	mu_uint32 Strength;
	mu_uint32 Dexterity;
	mu_uint32 Vitality;
	mu_uint32 Energy;
	mu_uint32 Leadership;
};
#pragma pack(pop)

class GS2CL_VIEWPORT_STATS : public MU_VIEWPORT_HEADEREX, public GS2CL_VIEWPORT_STATS_DATA
{
public:
	GS2CL_VIEWPORT_STATS()
	{
		Code = GS2CL_PROT_VIEWPORT_STATS;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_STATS_DATA*>(this));

		return parser.ProcessedLength;
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		Size = bufferLength;
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_STATS_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_VIEWPORT_STATS_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_VIEWPORT_MONEY_DATA
{
public:
	mu_int64 Money;
	mu_int64 StorageMoney;
};
#pragma pack(pop)

class GS2CL_VIEWPORT_MONEY : public MU_VIEWPORT_HEADEREX, public GS2CL_VIEWPORT_MONEY_DATA
{
public:
	GS2CL_VIEWPORT_MONEY()
	{
		Code = GS2CL_PROT_VIEWPORT_MONEY;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_MONEY_DATA*>(this));

		return parser.ProcessedLength;
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		Size = bufferLength;
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_MONEY_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_VIEWPORT_MONEY_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_VIEWPORT_SPEED_DATA
{
public:
	mu_int16 Key;
	mu_int32 AttackSpeed;
	mu_int32 MagicSpeed;
};
#pragma pack(pop)

class GS2CL_VIEWPORT_SPEED : public MU_VIEWPORT_HEADEREX, public GS2CL_VIEWPORT_SPEED_DATA
{
public:
	GS2CL_VIEWPORT_SPEED()
	{
		Code = GS2CL_PROT_VIEWPORT_SPEED;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_SPEED_DATA*>(this));

		return parser.ProcessedLength;
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		Size = bufferLength;
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_SPEED_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_VIEWPORT_SPEED_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_VIEWPORT_BUFFEFFECT_DATA
{
public:
	struct ARM_PACKED BuffData
	{
		mu_int32 EffectTime;
		mu_uint16 OptionType;
		mu_uint16 EffectType;
		mu_uint8 EffectOption;
		mu_uint8 BuffType;
		mu_boolean AddEffect;
	};

	mu_int16 Key;
	BuffData BuffList[MAX_BUFF_SLOT_INDEX];
};
#pragma pack(pop)

class GS2CL_VIEWPORT_BUFFEFFECT : public MU_VIEWPORT_HEADEREX, public GS2CL_VIEWPORT_BUFFEFFECT_DATA
{
public:
	GS2CL_VIEWPORT_BUFFEFFECT()
	{
		Code = GS2CL_PROT_VIEWPORT_BUFFEFFECT;
		mu_zeromem(BuffList, sizeof(BuffList));
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_BUFFEFFECT_DATA*>(this));

		return parser.ProcessedLength;
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		Size = bufferLength;
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_BUFFEFFECT_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_VIEWPORT_BUFFEFFECT_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_VIEWPORT_PKLEVEL_DATA
{
public:
	mu_int16 Key;
	mu_uint8 PkLevel;
};
#pragma pack(pop)

class GS2CL_VIEWPORT_PKLEVEL : public MU_VIEWPORT_HEADEREX, public GS2CL_VIEWPORT_PKLEVEL_DATA
{
public:
	GS2CL_VIEWPORT_PKLEVEL()
	{
		Code = GS2CL_PROT_VIEWPORT_PKLEVEL;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_PKLEVEL_DATA*>(this));

		return parser.ProcessedLength;
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		Size = bufferLength;
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_PKLEVEL_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_VIEWPORT_PKLEVEL_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_VIEWPORT_TELEPORT_DATA
{
public:
	mu_uint8 Map;
	mu_uint8 X;
	mu_uint8 Y;
	mu_float Direction;
	mu_uint32 EventMask;
};
#pragma pack(pop)

class GS2CL_VIEWPORT_TELEPORT : public MU_VIEWPORT_HEADEREX, public GS2CL_VIEWPORT_TELEPORT_DATA
{
public:
	GS2CL_VIEWPORT_TELEPORT()
	{
		Code = GS2CL_PROT_VIEWPORT_TELEPORT;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_TELEPORT_DATA*>(this));

		return parser.ProcessedLength;
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		Size = bufferLength;
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_TELEPORT_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_VIEWPORT_TELEPORT_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_VIEWPORT_INVENTORY_DATA
{
public:
	enum
	{
		INVENTORY_DATA,
		WAREHOUSE_DATA,
		CHAOSMACHINE_DATA,
	};
	mu_uint8 Type;
};
#pragma pack(pop)

class GS2CL_VIEWPORT_INVENTORY_DATAEX
{
public:
	std::vector<SHARED::ITEM_DATA> Items;
};

class GS2CL_VIEWPORT_INVENTORY : public MU_VIEWPORT_HEADEREX, public GS2CL_VIEWPORT_INVENTORY_DATA, public GS2CL_VIEWPORT_INVENTORY_DATAEX
{
public:
	GS2CL_VIEWPORT_INVENTORY()
	{
		Code = GS2CL_PROT_VIEWPORT_INVENTORY;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_INVENTORY_DATA*>(this));
		PARSER_WRITEVECTOR(Items);

		return parser.ProcessedLength;
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		Size = bufferLength;
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_INVENTORY_DATA*>(this));
		PARSER_WRITEVECTOR(Items);

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_VIEWPORT_INVENTORY_DATA*>(this));
		PARSER_READVECTOR(Items);

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_VIEWPORT_TRADEMONEY_DATA
{
public:
	mu_boolean IsYourMoney;
	mu_int64 Money;
};
#pragma pack(pop)

class GS2CL_VIEWPORT_TRADEMONEY : public MU_VIEWPORT_HEADEREX, public GS2CL_VIEWPORT_TRADEMONEY_DATA
{
public:
	GS2CL_VIEWPORT_TRADEMONEY()
	{
		Code = GS2CL_PROT_VIEWPORT_TRADEMONEY;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_TRADEMONEY_DATA*>(this));

		return parser.ProcessedLength;
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		Size = bufferLength;
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_TRADEMONEY_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_VIEWPORT_TRADEMONEY_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_VIEWPORT_PETUPDATE_DATA
{
public:
	mu_uint8 PetType;
	mu_uint8 UpdateType;
	SHARED::ITEM_DATA ItemData;
};
#pragma pack(pop)

class GS2CL_VIEWPORT_PETUPDATE : public MU_VIEWPORT_HEADEREX, public GS2CL_VIEWPORT_PETUPDATE_DATA
{
public:
	enum
	{
		PET_UPDATE,
		PET_LEVELUP,
		PET_LEVELDOWN,
	};

	enum
	{
		TYPE_DARKHORSE,
		TYPE_DARKRAVEN,
	};
public:
	GS2CL_VIEWPORT_PETUPDATE()
	{
		Code = GS2CL_PROT_VIEWPORT_PETUPDATE;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_PETUPDATE_DATA*>(this));

		return parser.ProcessedLength;
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		Size = bufferLength;
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_PETUPDATE_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_VIEWPORT_PETUPDATE_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_VIEWPORT_PETEFFECT_DATA
{
public:
	mu_int16 Key;
	mu_uint16 Level;
	mu_uint8 PetType;
	mu_uint8 UpdateType;
};
#pragma pack(pop)

class GS2CL_VIEWPORT_PETEFFECT : public MU_VIEWPORT_HEADEREX, public GS2CL_VIEWPORT_PETEFFECT_DATA
{
public:
	GS2CL_VIEWPORT_PETEFFECT()
	{
		Code = GS2CL_PROT_VIEWPORT_PETEFFECT;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_PETEFFECT_DATA*>(this));

		return parser.ProcessedLength;
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		Size = bufferLength;
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_PETEFFECT_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_VIEWPORT_PETEFFECT_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_VIEWPORT_PETCOMMAND_DATA
{
public:
	mu_uint8 PetType;
	mu_uint8 CommandType;

	mu_int16 TargetKey;
};
#pragma pack(pop)

class GS2CL_VIEWPORT_PETCOMMAND : public MU_VIEWPORT_HEADEREX, public GS2CL_VIEWPORT_PETCOMMAND_DATA
{
public:
	GS2CL_VIEWPORT_PETCOMMAND()
	{
		Code = GS2CL_PROT_VIEWPORT_PETCOMMAND;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_PETCOMMAND_DATA*>(this));

		return parser.ProcessedLength;
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		Size = bufferLength;
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_PETCOMMAND_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_VIEWPORT_PETCOMMAND_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_VIEWPORT_PETATTACK_DATA
{
public:
	mu_uint8 PetType;
	mu_uint8 SkillType;

	mu_int16 Key;
	mu_int16 TargetKey;
};
#pragma pack(pop)

class GS2CL_VIEWPORT_PETATTACK : public MU_VIEWPORT_HEADEREX, public GS2CL_VIEWPORT_PETATTACK_DATA
{
public:
	GS2CL_VIEWPORT_PETATTACK()
	{
		Code = GS2CL_PROT_VIEWPORT_PETATTACK;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_PETATTACK_DATA*>(this));

		return parser.ProcessedLength;
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		Size = bufferLength;
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_PETATTACK_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_VIEWPORT_PETATTACK_DATA*>(this));

		return true;
	}
};

#endif