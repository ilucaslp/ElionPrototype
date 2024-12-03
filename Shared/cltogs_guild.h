#ifndef __CLTOGS_GUILD_H__
#define __CLTOGS_GUILD_H__

#pragma once

namespace CREATEGUILD_RESULT
{
	enum
	{
		RESULT_SUCCESS = 0,
		RESULT_FAILED,
		RESULT_ALREADYEXISTS,
		RESULT_TOOSHORT,
		RESULT_ALREADYINGUILD,
		RESULT_INVALIDNAME,
		RESULT_RESERVEDNAME,
	};
};

#pragma pack(push, 1)
class ARM_PACKED CL2GS_REQUEST_CREATEGUILD_DATA
{
public:
	mu_uint8 MarkBuffer[MAX_GUILDMARKBUFFER];
};
#pragma pack(pop)

class CL2GS_REQUEST_CREATEGUILD_DATAEX
{
public:
	mu_string GuildName;
};

class CL2GS_REQUEST_CREATEGUILD : public MU_CLIENT_TO_SERVER_HEADEREX,
	public CL2GS_REQUEST_CREATEGUILD_DATA,
	public CL2GS_REQUEST_CREATEGUILD_DATAEX
{
public:
	CL2GS_REQUEST_CREATEGUILD()
	{
		Code = GS2CL_PROT_CREATEGUILD;
	}

	virtual mu_uint32 Length() override
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_REQUEST_CREATEGUILD_DATA*>(this));
		PARSER_WRITESTRING(GuildName, GUILDNAME_MAXIMUM);

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_REQUEST_CREATEGUILD_DATA*>(this));
		PARSER_WRITESTRING(GuildName, GUILDNAME_MAXIMUM);

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<CL2GS_REQUEST_CREATEGUILD_DATA*>(this));
		PARSER_READSTRING(GuildName, GUILDNAME_MAXIMUM);

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_RECEIVE_CREATEGUILD_DATA
{
public:
	mu_uint8 Result;
};
#pragma pack(pop)

class GS2CL_RECEIVE_CREATEGUILD : public MU_SERVER_TO_CLIENT_HEADEREX, public GS2CL_RECEIVE_CREATEGUILD_DATA
{
public:
	GS2CL_RECEIVE_CREATEGUILD()
	{
		Code = GS2CL_PROT_CREATEGUILD;
	}

	virtual mu_uint32 Length() override
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_CREATEGUILD_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_CREATEGUILD_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_RECEIVE_CREATEGUILD_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED CL2GS_REQUEST_LEAVEGUILD_DATA
{
public:
	mu_int32 MemberIndex;
};
#pragma pack(pop)

class CL2GS_REQUEST_LEAVEGUILD : public MU_CLIENT_TO_SERVER_HEADEREX, public CL2GS_REQUEST_LEAVEGUILD_DATA
{
public:
	CL2GS_REQUEST_LEAVEGUILD()
	{
		Code = GS2CL_PROT_LEAVEGUILD;
	}

	virtual mu_uint32 Length() override
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_REQUEST_LEAVEGUILD_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_REQUEST_LEAVEGUILD_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<CL2GS_REQUEST_LEAVEGUILD_DATA*>(this));

		return true;
	}
};

namespace LEAVEGUILD_RESULT
{
	enum
	{
		RESULT_SUCCESS_DISBAND = 0,
		RESULT_SUCCESS_LEAVE,
		RESULT_FAILED,
	};
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_RECEIVE_LEAVEGUILD_DATA
{
public:
	mu_uint8 Result;
	mu_int16 Key;
	mu_int32 MemberIndex;
};
#pragma pack(pop)

class GS2CL_RECEIVE_LEAVEGUILD : public MU_SERVER_TO_CLIENT_HEADEREX, public GS2CL_RECEIVE_LEAVEGUILD_DATA
{
public:
	GS2CL_RECEIVE_LEAVEGUILD()
	{
		Code = GS2CL_PROT_LEAVEGUILD;
	}

	virtual mu_uint32 Length() override
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_LEAVEGUILD_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_LEAVEGUILD_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_RECEIVE_LEAVEGUILD_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED CL2GS_REQUEST_GUILDINFO_DATA
{
public:
	mu_int32 GuildIndex;
};
#pragma pack(pop)

class CL2GS_REQUEST_GUILDINFO : public MU_CLIENT_TO_SERVER_HEADEREX,
	public CL2GS_REQUEST_GUILDINFO_DATA
{
public:
	CL2GS_REQUEST_GUILDINFO()
	{
		Code = GS2CL_PROT_GUILDINFO;
	}

	virtual mu_uint32 Length() override
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_REQUEST_GUILDINFO_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_REQUEST_GUILDINFO_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<CL2GS_REQUEST_GUILDINFO_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_RECEIVE_GUILDINFO_DATA
{
public:
	mu_int32 GuildIndex;
	mu_int8 GuildType;
	mu_uint8 MarkBuffer[MAX_GUILDMARKBUFFER];
};
#pragma pack(pop)

class GS2CL_RECEIVE_GUILDINFO_DATAEX
{
public:
	mu_string GuildName;
	mu_string GuildUnion;
};

class GS2CL_RECEIVE_GUILDINFO : public MU_SERVER_TO_CLIENT_HEADEREX,
	public GS2CL_RECEIVE_GUILDINFO_DATA,
	public GS2CL_RECEIVE_GUILDINFO_DATAEX
{
public:
	GS2CL_RECEIVE_GUILDINFO()
	{
		Code = GS2CL_PROT_GUILDINFO;
	}

	virtual mu_uint32 Length() override
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_GUILDINFO_DATA*>(this));
		PARSER_WRITESTRING(GuildName, GUILDNAME_MAXIMUM);
		PARSER_WRITESTRING(GuildUnion, GUILDNAME_MAXIMUM);

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_GUILDINFO_DATA*>(this));
		PARSER_WRITESTRING(GuildName, GUILDNAME_MAXIMUM);
		PARSER_WRITESTRING(GuildUnion, GUILDNAME_MAXIMUM);

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_RECEIVE_GUILDINFO_DATA*>(this));
		PARSER_READSTRING(GuildName, GUILDNAME_MAXIMUM);
		PARSER_READSTRING(GuildUnion, GUILDNAME_MAXIMUM);

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED CL2GS_REQUEST_GUILDJOIN_DATA
{
public:
	mu_int16 Key;
};
#pragma pack(pop)

class CL2GS_REQUEST_GUILDJOIN : public MU_CLIENT_TO_SERVER_HEADEREX,
	public CL2GS_REQUEST_GUILDJOIN_DATA
{
public:
	CL2GS_REQUEST_GUILDJOIN()
	{
		Code = GS2CL_PROT_GUILDJOIN_REQUEST;
	}

	virtual mu_uint32 Length() override
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_REQUEST_GUILDJOIN_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_REQUEST_GUILDJOIN_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<CL2GS_REQUEST_GUILDJOIN_DATA*>(this));

		return true;
	}
};

namespace GUILDJOIN_RESULT
{
	enum
	{
		GUILDJOIN_SUCCESS,
		GUILDJOIN_FAILED,
		GUILDJOIN_REFUSED,
		GUILDJOIN_NOGUILD,
		GUILDJOIN_NOTMASTER,
		GUILDJOIN_INWAR,
		GUILDJOIN_FULL,
		GUILDJOIN_DISCONNECTED,
		GUILDJOIN_BUSY,
		GUILDJOIN_LOWLEVEL,
		GUILDJOIN_ALREADYINGUILD,
		GUILDJOIN_MASTERNOTGENS,
		GUILDJOIN_REQUESTNOTGENS,
		GUILDJOIN_REQUESTDIFFGENS,
	};
}

#pragma pack(push, 1)
class ARM_PACKED GS2CL_RECEIVE_GUILDJOIN_DATA
{
public:
	mu_int16 Result;
};
#pragma pack(pop)

class GS2CL_RECEIVE_GUILDJOIN : public MU_SERVER_TO_CLIENT_HEADEREX,
	public GS2CL_RECEIVE_GUILDJOIN_DATA
{
public:
	GS2CL_RECEIVE_GUILDJOIN()
	{
		Code = GS2CL_PROT_GUILDJOIN_REQUEST;
	}

	virtual mu_uint32 Length() override
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_GUILDJOIN_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_GUILDJOIN_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_RECEIVE_GUILDJOIN_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_RECEIVE_GUILDJOIN_PETITION_DATA
{
public:
	mu_int16 Key;
};
#pragma pack(pop)

class GS2CL_RECEIVE_GUILDJOIN_PETITION : public MU_SERVER_TO_CLIENT_HEADEREX,
	public GS2CL_RECEIVE_GUILDJOIN_PETITION_DATA
{
public:
	GS2CL_RECEIVE_GUILDJOIN_PETITION()
	{
		Code = GS2CL_PROT_GUILDJOIN_PETITION;
	}

	virtual mu_uint32 Length() override
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_GUILDJOIN_PETITION_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_GUILDJOIN_PETITION_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_RECEIVE_GUILDJOIN_PETITION_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED CL2GS_RECEIVE_GUILDJOIN_PETITION_DATA
{
public:
	mu_boolean Answer;
};
#pragma pack(pop)

class CL2GS_RECEIVE_GUILDJOIN_PETITION : public MU_CLIENT_TO_SERVER_HEADEREX, public CL2GS_RECEIVE_GUILDJOIN_PETITION_DATA
{
public:
	CL2GS_RECEIVE_GUILDJOIN_PETITION()
	{
		Code = GS2CL_PROT_GUILDJOIN_PETITION;
	}

	virtual mu_uint32 Length() override
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_RECEIVE_GUILDJOIN_PETITION_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_RECEIVE_GUILDJOIN_PETITION_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<CL2GS_RECEIVE_GUILDJOIN_PETITION_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED CL2GS_REQUEST_GUILDSTATUS_CANCEL_DATA
{
public:
	mu_int32 CharacterIndex;
};
#pragma pack(pop)

class CL2GS_REQUEST_GUILDSTATUS_CANCEL : public MU_CLIENT_TO_SERVER_HEADEREX, public CL2GS_REQUEST_GUILDSTATUS_CANCEL_DATA
{
public:
	CL2GS_REQUEST_GUILDSTATUS_CANCEL()
	{
		Code = GS2CL_PROT_GUILDSTATUS_CANCEL;
	}

	virtual mu_uint32 Length() override
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_REQUEST_GUILDSTATUS_CANCEL_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_REQUEST_GUILDSTATUS_CANCEL_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<CL2GS_REQUEST_GUILDSTATUS_CANCEL_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED CL2GS_REQUEST_GUILDSTATUS_GIVE_DATA
{
public:
	mu_int32 CharacterIndex;
	mu_int16 GuildStatus;
};
#pragma pack(pop)

class CL2GS_REQUEST_GUILDSTATUS_GIVE : public MU_CLIENT_TO_SERVER_HEADEREX, public CL2GS_REQUEST_GUILDSTATUS_GIVE_DATA
{
public:
	CL2GS_REQUEST_GUILDSTATUS_GIVE()
	{
		Code = GS2CL_PROT_GUILDSTATUS_GIVE;
	}

	virtual mu_uint32 Length() override
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_REQUEST_GUILDSTATUS_GIVE_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_REQUEST_GUILDSTATUS_GIVE_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<CL2GS_REQUEST_GUILDSTATUS_GIVE_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_VIEWPORT_GUILDINFO_DATA
{
public:
	mu_int32 GuildIndex;

	mu_int16 Key;

	mu_int16 GuildStatus;
	mu_int8 GuildType;
	mu_uint8 GuildRelationship;

	mu_boolean CastleOwner;
};
#pragma pack(pop)

class GS2CL_VIEWPORT_GUILDINFO : public MU_VIEWPORT_HEADEREX, public GS2CL_VIEWPORT_GUILDINFO_DATA
{
public:
	GS2CL_VIEWPORT_GUILDINFO()
	{
		Code = GS2CL_PROT_VIEWPORT_GUILDINFO;
	}

	virtual mu_uint32 Length() override
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_GUILDINFO_DATA*>(this));

		return parser.ProcessedLength;
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		Size = bufferLength;
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_VIEWPORT_GUILDINFO_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_VIEWPORT_GUILDINFO_DATA*>(this));

		return true;
	}
};

class GS2CL_VIEWPORT_GUILDLIST_DATA
{
public:
#pragma pack(push, 1)
	class ARM_PACKED MEMBER_DATA
	{
	public:
		mu_int32 CharacterIndex;
		mu_int16 GroupIndex;
		mu_int16 ServerIndex;
		mu_int16 GuildStatus;
	};
#pragma pack(pop)

	class MEMBER_DATAEX : public MEMBER_DATA
	{
	public:
		mu_boolean Used;
		mu_string CharacterName;
	};

public:
	GS2CL_VIEWPORT_GUILDLIST_DATA()
	{
		Clear();
	}

	void Clear()
	{
		MembersCount = 0;
		for (mu_uint32 n = 0; n < MAX_GUILDMEMBER; n++)
		{
			Members[n].Used = false;
			Members[n].CharacterName.clear();
		}
	}

public:
	mu_uint32 MembersCount;
	MEMBER_DATAEX Members[MAX_GUILDMEMBER];
};

class GS2CL_VIEWPORT_GUILDLIST : public MU_VIEWPORT_HEADEREX, public GS2CL_VIEWPORT_GUILDLIST_DATA
{
public:
	GS2CL_VIEWPORT_GUILDLIST()
	{
		Code = GS2CL_PROT_VIEWPORT_GUILDLIST;
	}

	virtual mu_uint32 Length() override
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));

		PARSER_WRITE(MembersCount);
		if(MembersCount > 0)
		{
			for (mu_uint32 n = 0; n < MAX_GUILDMEMBER; n++)
			{
				MEMBER_DATAEX *member = &Members[n];

				if (member->Used == true)
				{
					PARSER_WRITE(*static_cast<MEMBER_DATA*>(member));
					PARSER_WRITESTRING(member->CharacterName, CHARACTER_MAXIMUM);
				}
			}
		}

		return parser.ProcessedLength;
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		Size = bufferLength;
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));

		PARSER_WRITE(MembersCount);
		if (MembersCount > 0)
		{
			for (mu_uint32 n = 0; n < MAX_GUILDMEMBER; n++)
			{
				MEMBER_DATAEX *member = &Members[n];

				if (member->Used == true)
				{
					PARSER_WRITE(*static_cast<MEMBER_DATA*>(member));
					PARSER_WRITESTRING(member->CharacterName, CHARACTER_MAXIMUM);
				}
			}
		}

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_VIEWPORT_HEADER*>(this));

		PARSER_READ(MembersCount);
		if (MembersCount > 0)
		{
			for (mu_uint32 n = 0; n < MembersCount; n++)
			{
				MEMBER_DATAEX *member = &Members[n];

				member->Used = true;
				PARSER_READ(*static_cast<MEMBER_DATA*>(member));
				PARSER_READSTRING(member->CharacterName, CHARACTER_MAXIMUM);
			}
		}

		return true;
	}
};

class GS2CL_VIEWPORT_GUILDUPDATE_DATA
{
public:
#pragma pack(push, 1)
	class ARM_PACKED ACTION_DATA
	{
	public:
		mu_int32 CharacterIndex;
		mu_int16 GroupIndex;
		mu_int16 ServerIndex;
		mu_int16 GuildStatus;
	};
#pragma pack(pop)

	class ACTION_DATAEX : public ACTION_DATA
	{
	public:
		mu_uint8 ActionType;
		mu_string CharacterName;
	};

public:
	GS2CL_VIEWPORT_GUILDUPDATE_DATA()
	{
		Clear();
	}

	void Clear()
	{
		Actions.clear();
	}

public:
	std::vector<ACTION_DATAEX> Actions;
};

class GS2CL_VIEWPORT_GUILDUPDATE : public MU_VIEWPORT_HEADEREX, public GS2CL_VIEWPORT_GUILDUPDATE_DATA
{
public:
	enum
	{
		ACTION_DELETE,
		ACTION_OFFLINE,
		ACTION_UPDATE,
	};

public:
	GS2CL_VIEWPORT_GUILDUPDATE()
	{
		Code = GS2CL_PROT_VIEWPORT_GUILDUPDATE;
	}

	virtual mu_uint32 Length() override
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));

		PARSER_WRITE32(Actions.size());
		if (Actions.empty() == false)
		{
			for (auto ait = Actions.begin(); ait != Actions.end(); ait++)
			{
				ACTION_DATAEX *action = &(*ait);

				PARSER_WRITE(action->ActionType);

				switch (action->ActionType)
				{
				case ACTION_DELETE:
				case ACTION_OFFLINE:
					{
						PARSER_WRITE(action->CharacterIndex);
					}
					break;

				case ACTION_UPDATE:
					{
						PARSER_WRITE(*static_cast<ACTION_DATA*>(action));
						PARSER_WRITESTRING(action->CharacterName, CHARACTER_MAXIMUM);
					}
					break;
				}
			}
		}

		return parser.ProcessedLength;
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		Size = bufferLength;
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));

		PARSER_WRITE32(Actions.size());
		if (Actions.empty() == false)
		{
			for (auto ait = Actions.begin(); ait != Actions.end(); ait++)
			{
				ACTION_DATAEX *action = &(*ait);

				PARSER_WRITE(action->ActionType);

				switch (action->ActionType)
				{
				case ACTION_DELETE:
				case ACTION_OFFLINE:
					{
						PARSER_WRITE(action->CharacterIndex);
					}
					break;

				case ACTION_UPDATE:
					{
						PARSER_WRITE(*static_cast<ACTION_DATA*>(action));
						PARSER_WRITESTRING(action->CharacterName, CHARACTER_MAXIMUM);
					}
					break;
				}
			}
		}

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_VIEWPORT_HEADER*>(this));

		mu_uint32 actionsCount = 0;
		PARSER_READ(actionsCount);
		if (actionsCount > 0)
		{
			Actions.resize(actionsCount);
			for (auto ait = Actions.begin(); ait != Actions.end(); ait++)
			{
				ACTION_DATAEX *action = &(*ait);

				PARSER_READ(action->ActionType);

				switch (action->ActionType)
				{
				case ACTION_DELETE:
				case ACTION_OFFLINE:
					{
						PARSER_READ(action->CharacterIndex);
					}
					break;

				case ACTION_UPDATE:
					{
						PARSER_READ(*static_cast<ACTION_DATA*>(action));
						PARSER_READSTRING(action->CharacterName, CHARACTER_MAXIMUM);
					}
					break;
				}
			}
		}

		return true;
	}
};

#endif