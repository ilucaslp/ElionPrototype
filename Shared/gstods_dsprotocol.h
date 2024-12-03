#ifndef __GSTODS_DSPROTOCOL_H__
#define __GSTODS_DSPROTOCOL_H__

#pragma once

class GS2DS_HEARTBEAT : public MU_SERVER_TO_SERVER_HEADEREX
{
public:
	GS2DS_HEARTBEAT()
	{
		Code = GS2DS_PROT_HEARTBEAT;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, STS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));

		return true;
	}
};

class GS2DS_PACKET_RECEIVED : public MU_SERVER_TO_SERVER_HEADEREX
{
public:
	GS2DS_PACKET_RECEIVED()
	{
		Code = GS2DS_PROT_RECEIVED;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, STS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));

		return true;
	}
};

#define GSTODS_REGISTERCODE_NEW ((mu_uint32)-1)

#pragma pack(push, 1)
class GS2DS_REQUEST_REGISTER_DATA
{
public:
	mu_uint8 GroupIndex;
	mu_uint8 ServerIndex;
	mu_uint32 MaxUsers;
	mu_boolean NewInstance;
};
#pragma pack(pop)

class GS2DS_REQUEST_REGISTER_DATAEX
{
public:
	mu_string ServerName;
	mu_string Password;
};

class GS2DS_REGISTER : public MU_SERVER_TO_SERVER_HEADEREX, public GS2DS_REQUEST_REGISTER_DATA, public GS2DS_REQUEST_REGISTER_DATAEX
{
public:
	enum
	{
		MAX_NAME_LENGTH = 256,
		MAX_PASSWORD_LENGTH = 64,
	};

public:
	GS2DS_REGISTER()
	{
		Code = GS2DS_PROT_REGISTER;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2DS_REQUEST_REGISTER_DATA*>(this));
		PARSER_WRITESTRING(ServerName, MAX_NAME_LENGTH);
		PARSER_WRITESTRING(Password, MAX_PASSWORD_LENGTH);

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2DS_REQUEST_REGISTER_DATA*>(this));
		PARSER_WRITESTRING(ServerName, MAX_NAME_LENGTH);
		PARSER_WRITESTRING(Password, MAX_PASSWORD_LENGTH);

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<GS2DS_REQUEST_REGISTER_DATA*>(this));
		PARSER_READSTRING(ServerName, MAX_NAME_LENGTH);
		PARSER_READSTRING(Password, MAX_PASSWORD_LENGTH);

		return true;
	}
};

#pragma pack(push, 1)
class GS2DS_REQUEST_CHARACTERLIST_DATA
{
public:
	mu_uint64 Identifier;
	mu_uint32 Index;
	mu_int32 AccountIndex;
};
#pragma pack(pop)

class GS2DS_REQUEST_CHARACTERLIST : public MU_SERVER_TO_SERVER_HEADEREX, public GS2DS_REQUEST_CHARACTERLIST_DATA
{
public:
	GS2DS_REQUEST_CHARACTERLIST()
	{
		Code = GS2DS_PROT_CHARACTERLIST;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2DS_REQUEST_CHARACTERLIST_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2DS_REQUEST_CHARACTERLIST_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<GS2DS_REQUEST_CHARACTERLIST_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class DS2GS_RESPONSE_CHARACTERLIST_HEADER
{
public:
	mu_uint64 Identifier;
	mu_uint32 Index;
};
#pragma pack(pop)

class DS2GS_RESPONSE_CHARACTERLIST : public MU_SERVER_TO_SERVER_HEADEREX, public DS2GS_RESPONSE_CHARACTERLIST_HEADER
{
public:
	struct CHARACTER_DATA
	{
		mu_int32 Index;

		mu_uint8 SlotIndex;

		mu_uint8 Class;
		mu_uint8 SubClass;
		mu_uint8 ControlCode;
		mu_uint8 PkLevel;

		mu_int16 GuildStatus;
		mu_uint16 Level;
		mu_uint16 DarkRavenLevel;

		SHARED::CHARSET_PACKET Equipment[GAME::MAX_EQUIPMENT];

		mu_string Name;
	};

public:
	DS2GS_RESPONSE_CHARACTERLIST()
	{
		Code = GS2DS_PROT_CHARACTERLIST;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<DS2GS_RESPONSE_CHARACTERLIST_HEADER*>(this));
		PARSER_WRITE32(Characters.size());

		for (auto it = Characters.begin(); it != Characters.end(); it++)
		{
			CHARACTER_DATA &character = *it;

			PARSER_WRITE(character.Index);
			PARSER_WRITE(character.SlotIndex);
			PARSER_WRITE(character.Class);
			PARSER_WRITE(character.SubClass);
			PARSER_WRITE(character.ControlCode);
			PARSER_WRITE(character.PkLevel);
			PARSER_WRITE(character.GuildStatus);
			PARSER_WRITE(character.Level);
			PARSER_WRITE(character.DarkRavenLevel);

			for (mu_uint32 n = 0; n < GAME::MAX_EQUIPMENT; n++)
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

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<DS2GS_RESPONSE_CHARACTERLIST_HEADER*>(this));
		PARSER_WRITE32(Characters.size());

		for (auto it = Characters.begin(); it != Characters.end(); it++)
		{
			CHARACTER_DATA &character = *it;

			PARSER_WRITE(character.Index);
			PARSER_WRITE(character.SlotIndex);
			PARSER_WRITE(character.Class);
			PARSER_WRITE(character.SubClass);
			PARSER_WRITE(character.ControlCode);
			PARSER_WRITE(character.PkLevel);
			PARSER_WRITE(character.GuildStatus);
			PARSER_WRITE(character.Level);
			PARSER_WRITE(character.DarkRavenLevel);

			for (mu_uint32 n = 0; n < GAME::MAX_EQUIPMENT; n++)
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

		PARSER_READ(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<DS2GS_RESPONSE_CHARACTERLIST_HEADER*>(this));
		mu_uint32 charactersCount = 0;
		PARSER_READ(charactersCount);
		Characters.resize(charactersCount);

		for (auto it = Characters.begin(); it != Characters.end(); it++)
		{
			CHARACTER_DATA &character = *it;

			PARSER_READ(character.Index);
			PARSER_READ(character.SlotIndex);
			PARSER_READ(character.Class);
			PARSER_READ(character.SubClass);
			PARSER_READ(character.ControlCode);
			PARSER_READ(character.PkLevel);
			PARSER_READ(character.GuildStatus);
			PARSER_READ(character.Level);
			PARSER_READ(character.DarkRavenLevel);

			for (mu_uint32 n = 0; n < GAME::MAX_EQUIPMENT; n++)
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

#pragma pack(push, 1)
class GS2DS_REQUEST_CREATECHARACTER_HEADER
{
public:
	mu_uint64 Identifier;
	mu_uint32 Index;
	mu_int32 AccountIndex;
	mu_uint8 Class;
};
#pragma pack(pop)

class GS2DS_REQUEST_CREATECHARACTER_HEADEREX
{
public:
	mu_string CharacterName;
};

class GS2DS_REQUEST_CREATECHARACTER : public MU_SERVER_TO_SERVER_HEADEREX,
	public GS2DS_REQUEST_CREATECHARACTER_HEADER,
	public GS2DS_REQUEST_CREATECHARACTER_HEADEREX
{
public:
	GS2DS_REQUEST_CREATECHARACTER()
	{
		Code = GS2DS_PROT_CREATECHARACTER;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2DS_REQUEST_CREATECHARACTER_HEADER*>(this));
		PARSER_WRITESTRING(CharacterName, CHARACTER_MAXIMUM);

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2DS_REQUEST_CREATECHARACTER_HEADER*>(this));
		PARSER_WRITESTRING(CharacterName, CHARACTER_MAXIMUM);

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<GS2DS_REQUEST_CREATECHARACTER_HEADER*>(this));
		PARSER_READSTRING(CharacterName, CHARACTER_MAXIMUM);

		return true;
	}
};

#pragma pack(push, 1)
class DS2GS_RESPONSE_CREATECHARACTER_HEADER
{
public:
	mu_uint64 Identifier;
	mu_uint32 Index;
	mu_int32 CharacterIndex;

	mu_int16 GuildStatus;
	mu_uint16 Level;
	mu_uint16 DarkRavenLevel;

	mu_uint8 ResponseCode;
	mu_uint8 SlotIndex;
	mu_uint8 Class;
	mu_uint8 SubClass;
	mu_uint8 ControlCode;
	mu_uint8 PkLevel;
};
#pragma pack(pop)

class DS2GS_RESPONSE_CREATECHARACTER_HEADEREX
{
public:
	SHARED::CHARSET_PACKET Equipment[GAME::MAX_EQUIPMENT];
	mu_string Name;
};

class DS2GS_RESPONSE_CREATECHARACTER : public MU_SERVER_TO_SERVER_HEADEREX,
	public DS2GS_RESPONSE_CREATECHARACTER_HEADER,
	public DS2GS_RESPONSE_CREATECHARACTER_HEADEREX
{
public:
	DS2GS_RESPONSE_CREATECHARACTER()
	{
		Code = GS2DS_PROT_CREATECHARACTER;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<DS2GS_RESPONSE_CREATECHARACTER_HEADER*>(this));
		for (mu_uint32 n = 0; n < GAME::MAX_EQUIPMENT; n++)
		{
			PARSER_WRITECHARSET(Equipment[n]);
		}
		PARSER_WRITESTRING(Name, CHARACTER_MAXIMUM);

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<DS2GS_RESPONSE_CREATECHARACTER_HEADER*>(this));
		for (mu_uint32 n = 0; n < GAME::MAX_EQUIPMENT; n++)
		{
			PARSER_WRITECHARSET(Equipment[n]);
		}
		PARSER_WRITESTRING(Name, CHARACTER_MAXIMUM);

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<DS2GS_RESPONSE_CREATECHARACTER_HEADER*>(this));
		for (mu_uint32 n = 0; n < GAME::MAX_EQUIPMENT; n++)
		{
			PARSER_READCHARSET(Equipment[n]);
		}
		PARSER_READSTRING(Name, CHARACTER_MAXIMUM);

		return true;
	}
};

#pragma pack(push, 1)
class GS2DS_REQUEST_DELETECHARACTER_DATA
{
public:
	mu_uint64 Identifier;
	mu_uint32 Index;
	mu_int32 AccountIndex;
	mu_int32 CharacterIndex;
};
#pragma pack(pop)

class GS2DS_REQUEST_DELETECHARACTER : public MU_SERVER_TO_SERVER_HEADEREX, public GS2DS_REQUEST_DELETECHARACTER_DATA
{
public:
	GS2DS_REQUEST_DELETECHARACTER()
	{
		Code = GS2DS_PROT_DELETECHARACTER;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2DS_REQUEST_DELETECHARACTER_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2DS_REQUEST_DELETECHARACTER_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<GS2DS_REQUEST_DELETECHARACTER_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class GS2DS_REQUEST_CHARACTER_DATA
{
public:
	mu_uint64 Identifier;
	mu_uint32 Index;
	mu_int32 AccountIndex;
	mu_int32 CharacterIndex;
};
#pragma pack(pop)

class GS2DS_REQUEST_CHARACTER : public MU_SERVER_TO_SERVER_HEADEREX,
	public GS2DS_REQUEST_CHARACTER_DATA
{
public:
	GS2DS_REQUEST_CHARACTER()
	{
		Code = GS2DS_PROT_CHARACTER;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2DS_REQUEST_CHARACTER_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2DS_REQUEST_CHARACTER_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<GS2DS_REQUEST_CHARACTER_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class DS2GS_RESPONSE_CHARACTER_HEADER
{
public:
	mu_uint64 Identifier;
	mu_uint32 Index;
	mu_int32 CharacterIndex;
	mu_int32 GuildIndex;

	mu_uint8 ResponseCode;
	mu_uint8 Class;
	mu_uint8 SubClass;
	mu_uint8 PkLevel;
	mu_uint8 Map;
	mu_uint8 PositionX;
	mu_uint8 PositionY;
	mu_uint8 Direction;
	mu_uint8 ControlCode;

	mu_int16 GuildStatus;
	mu_uint16 Level;
	mu_uint32 MasterLevel;

	mu_uint32 LevelUpPoint;
	mu_uint32 MasterPoints;
	mu_uint32 Strength;
	mu_uint32 Dexterity;
	mu_uint32 Vitality;
	mu_uint32 Energy;
	mu_uint32 Leadership;
	mu_uint32 PkCount;
	mu_uint32 PkTime;

	mu_float Life;
	mu_float Mana;
	mu_float BP;
	mu_float Shield;

	mu_int64 Experience;
	mu_int64 MasterExperience;
	mu_int64 Money;
	mu_int64 StorageMoney;

	mu_uint8 Quest[GAME::MAX_DBQUEST];

	SHARED::QUESTMONSTERKILL_DATA QuestMonsterKill;
};
#pragma pack(pop)

class DS2GS_RESPONSE_CHARACTER_HEADEREX
{
public:
	mu_string Name;
	mu_string StoragePassword;
	std::vector<SHARED::ITEM_DATAEX> Items;
	std::vector<SHARED::SKILL_DATA> Skills;
	HELPER_SETTING HelperSetting;
};

class DS2GS_RESPONSE_CHARACTER : public MU_SERVER_TO_SERVER_HEADEREX,
	public DS2GS_RESPONSE_CHARACTER_HEADER,
	public DS2GS_RESPONSE_CHARACTER_HEADEREX
{
public:
	DS2GS_RESPONSE_CHARACTER()
	{
		Code = GS2DS_PROT_CHARACTER;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<DS2GS_RESPONSE_CHARACTER_HEADER*>(this));

		PARSER_WRITEVECTOR(Items);
		PARSER_WRITEVECTOR(Skills);

		PARSER_WRITESTRING(Name, CHARACTER_MAXIMUM);
		PARSER_WRITESTRING(StoragePassword, STORAGE_MAXIMUM);

		PARSER_WRITE(HelperSetting.Hunting);
		PARSER_WRITE(*static_cast<HELPER_OBTAININGSETTING*>(&HelperSetting.Obtaining));
		PARSER_WRITEVECTOR(HelperSetting.Obtaining.ExtraItems);

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<DS2GS_RESPONSE_CHARACTER_HEADER*>(this));

		PARSER_WRITEVECTOR(Items);
		PARSER_WRITEVECTOR(Skills);

		PARSER_WRITESTRING(Name, CHARACTER_MAXIMUM);
		PARSER_WRITESTRING(StoragePassword, STORAGE_MAXIMUM);

		PARSER_WRITE(HelperSetting.Hunting);
		PARSER_WRITE(*static_cast<HELPER_OBTAININGSETTING*>(&HelperSetting.Obtaining));
		PARSER_WRITEVECTOR(HelperSetting.Obtaining.ExtraItems);

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<DS2GS_RESPONSE_CHARACTER_HEADER*>(this));

		PARSER_READVECTOR(Items);
		PARSER_READVECTOR(Skills);

		PARSER_READSTRING(Name, CHARACTER_MAXIMUM);
		PARSER_READSTRING(StoragePassword, STORAGE_MAXIMUM);

		PARSER_READ(HelperSetting.Hunting);
		PARSER_READ(*static_cast<HELPER_OBTAININGSETTING*>(&HelperSetting.Obtaining));
		PARSER_READVECTOR(HelperSetting.Obtaining.ExtraItems);

		return true;
	}
};

#pragma pack(push, 1)
class GS2DS_REQUEST_UPDATEITEM_DATA
{
public:
	mu_uint32 OwnerIndex;
};
#pragma pack(pop)

class GS2DS_REQUEST_UPDATEITEM_DATAEX
{
public:
	std::vector<SHARED::ITEM_DATAEX> Items;
};

class GS2DS_REQUEST_UPDATEITEM : public MU_SERVER_TO_SERVER_HEADEREX, public GS2DS_REQUEST_UPDATEITEM_DATA, public GS2DS_REQUEST_UPDATEITEM_DATAEX
{
public:
	GS2DS_REQUEST_UPDATEITEM()
	{
		Code = GS2DS_PROT_UPDATEITEM;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2DS_REQUEST_UPDATEITEM_DATA*>(this));

		PARSER_WRITEVECTOR(Items);

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2DS_REQUEST_UPDATEITEM_DATA*>(this));

		PARSER_WRITEVECTOR(Items);

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<GS2DS_REQUEST_UPDATEITEM_DATA*>(this));

		PARSER_READVECTOR(Items);

		return true;
	}
};

#pragma pack(push, 1)
class GS2DS_REQUEST_DELETEITEM_DATA
{
public:
	mu_int64 DbIndex;
	mu_int64 ServerKey;
	mu_uint16 ServerIndex;
	mu_boolean ResetSlot;
};
#pragma pack(pop)

class GS2DS_REQUEST_DELETEITEM : public MU_SERVER_TO_SERVER_HEADEREX, public GS2DS_REQUEST_DELETEITEM_DATA
{
public:
	GS2DS_REQUEST_DELETEITEM()
	{
		Code = GS2DS_PROT_DELETEITEM;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2DS_REQUEST_DELETEITEM_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2DS_REQUEST_DELETEITEM_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<GS2DS_REQUEST_DELETEITEM_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class GS2DS_REQUEST_SAVECHARACTER_DATA
{
public:
	mu_int32 AccountIndex;
	mu_int32 CharacterIndex;

	mu_uint8 Class;
	mu_uint8 SubClass;
	mu_uint8 PkLevel;
	mu_uint8 Map;
	mu_uint8 PositionX;
	mu_uint8 PositionY;
	mu_uint8 Direction;

	mu_uint16 Level;
	mu_uint32 MasterLevel;

	mu_uint32 LevelUpPoint;
	mu_uint32 MasterPoints;
	mu_uint32 Strength;
	mu_uint32 Dexterity;
	mu_uint32 Vitality;
	mu_uint32 Energy;
	mu_uint32 Leadership;
	mu_uint32 PkCount;
	mu_uint32 PkTime;

	mu_float Life;
	mu_float Mana;
	mu_float BP;
	mu_float Shield;

	mu_int64 Experience;
	mu_int64 MasterExperience;
	mu_int64 Money;
	mu_int64 StorageMoney;

	mu_uint8 Quest[GAME::MAX_DBQUEST];

	SHARED::QUESTMONSTERKILL_DATA QuestMonsterKill;
};
#pragma pack(pop)

class GS2DS_REQUEST_SAVECHARACTER_DATAEX
{
public:
	mu_string StoragePassword;
};

class GS2DS_REQUEST_SAVECHARACTER : public MU_SERVER_TO_SERVER_HEADEREX, public GS2DS_REQUEST_SAVECHARACTER_DATA, public GS2DS_REQUEST_SAVECHARACTER_DATAEX
{
public:
	GS2DS_REQUEST_SAVECHARACTER()
	{
		Code = GS2DS_PROT_SAVECHARACTER;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2DS_REQUEST_SAVECHARACTER_DATA*>(this));
		PARSER_WRITESTRING(StoragePassword, STORAGE_MAXIMUM);

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2DS_REQUEST_SAVECHARACTER_DATA*>(this));
		PARSER_WRITESTRING(StoragePassword, STORAGE_MAXIMUM);

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<GS2DS_REQUEST_SAVECHARACTER_DATA*>(this));
		PARSER_READSTRING(StoragePassword, STORAGE_MAXIMUM);

		return true;
	}
};

#pragma pack(push, 1)
class GS2DS_REQUEST_UPDATEMAGIC_DATA
{
public:
	mu_int32 CharacterIndex;
};
#pragma pack(pop)

class GS2DS_REQUEST_UPDATEMAGIC_DATAEX
{
public:
	std::vector<SHARED::SKILL_DATA> Magics;
};

class GS2DS_REQUEST_UPDATEMAGIC : public MU_SERVER_TO_SERVER_HEADEREX, public GS2DS_REQUEST_UPDATEMAGIC_DATA, public GS2DS_REQUEST_UPDATEMAGIC_DATAEX
{
public:
	GS2DS_REQUEST_UPDATEMAGIC()
	{
		Code = GS2DS_PROT_UPDATEMAGIC;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2DS_REQUEST_UPDATEMAGIC_DATA*>(this));
		PARSER_WRITEVECTOR(Magics);

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2DS_REQUEST_UPDATEMAGIC_DATA*>(this));
		PARSER_WRITEVECTOR(Magics);

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<GS2DS_REQUEST_UPDATEMAGIC_DATA*>(this));
		PARSER_READVECTOR(Magics);

		return true;
	}
};

#pragma pack(push, 1)
class GS2DS_REQUEST_DELETEMAGIC_DATA
{
public:
	mu_int32 CharacterIndex;
	mu_int32 SkillIndex;
};
#pragma pack(pop)

class GS2DS_REQUEST_DELETEMAGIC : public MU_SERVER_TO_SERVER_HEADEREX, public GS2DS_REQUEST_DELETEMAGIC_DATA
{
public:
	GS2DS_REQUEST_DELETEMAGIC()
	{
		Code = GS2DS_PROT_DELETEMAGIC;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2DS_REQUEST_DELETEMAGIC_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2DS_REQUEST_DELETEMAGIC_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<GS2DS_REQUEST_DELETEMAGIC_DATA*>(this));

		return true;
	}
};

class GS2DS_REQUEST_SAVEHELPERSETTING_DATA
{
public:
	mu_int32 CharacterIndex;
	HELPER_SETTING HelperSetting;
};

class GS2DS_REQUEST_SAVEHELPERSETTING : public MU_SERVER_TO_SERVER_HEADEREX, public GS2DS_REQUEST_SAVEHELPERSETTING_DATA
{
public:
	GS2DS_REQUEST_SAVEHELPERSETTING()
	{
		Code = GS2DS_PROT_HELPER;
	}

	virtual mu_uint32 Length() override
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(CharacterIndex);
		PARSER_WRITE(HelperSetting.Hunting);
		PARSER_WRITE(*static_cast<HELPER_OBTAININGSETTING*>(&HelperSetting.Obtaining));
		PARSER_WRITEVECTOR(HelperSetting.Obtaining.ExtraItems);

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(CharacterIndex);
		PARSER_WRITE(HelperSetting.Hunting);
		PARSER_WRITE(*static_cast<HELPER_OBTAININGSETTING*>(&HelperSetting.Obtaining));
		PARSER_WRITEVECTOR(HelperSetting.Obtaining.ExtraItems);

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_READ(CharacterIndex);
		PARSER_READ(HelperSetting.Hunting);
		PARSER_READ(*static_cast<HELPER_OBTAININGSETTING*>(&HelperSetting.Obtaining));
		PARSER_READVECTOR(HelperSetting.Obtaining.ExtraItems);

		return true;
	}
};

#endif