#ifndef __GSTODS_EXPROTOCOL_H__
#define __GSTODS_EXPROTOCOL_H__

#pragma once

#pragma pack(push, 1)
class GS2EX_REQUEST_GUILD_DATA
{
public:
	mu_uint64 Identifier;
	mu_uint32 Index;
	mu_int32 CharacterIndex;
};
#pragma pack(pop)

class GS2EX_REQUEST_GUILDMEMBER : public MU_SERVER_TO_SERVER_HEADEREX, public GS2EX_REQUEST_GUILD_DATA
{
public:
	GS2EX_REQUEST_GUILDMEMBER()
	{
		Code = GS2EX_PROT_GUILDMEMBERINFO;
	}

	virtual mu_uint32 Length() override
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2EX_REQUEST_GUILD_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, STS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2EX_REQUEST_GUILD_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<GS2EX_REQUEST_GUILD_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class EX2GS_RESPONSE_GUILD_HEADER
{
public:
	mu_int32 GuildIndex;
	mu_int32 MasterIndex;

	mu_int32 Score;

	mu_int32 GuildUnion;
	mu_int32 GuildRival;
	
	mu_uint8 Mark[MAX_GUILDMARKBUFFER];
	mu_int8 GuildType;
};
#pragma pack(pop)

#pragma pack(push, 1)
class EX2GS_GUILD_MEMBER_HEADER
{
public:
	mu_int32 CharacterIndex;
	mu_int16 GroupIndex;
	mu_int16 ServerIndex;
	mu_int16 Status;
};
#pragma pack(pop)

class EX2GS_GUILD_MEMBER : public EX2GS_GUILD_MEMBER_HEADER
{
public:
	mu_string Name;
};

class EX2GS_RESPONSE_GUILD_HEADEREX
{
public:
	mu_string GuildName;
	mu_string RivalName;
	mu_string MasterName;

	std::vector<EX2GS_GUILD_MEMBER> Members;
};

class EX2GS_RESPONSE_GUILD : public MU_SERVER_TO_SERVER_HEADEREX,
	public EX2GS_RESPONSE_GUILD_HEADER,
	public EX2GS_RESPONSE_GUILD_HEADEREX
{
public:
	EX2GS_RESPONSE_GUILD()
	{
		Code = GS2EX_PROT_GUILDINFO;
	}

	virtual mu_uint32 Length() override
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<EX2GS_RESPONSE_GUILD_HEADER*>(this));
		PARSER_WRITESTRING(GuildName, GUILDNAME_MAXIMUM);
		PARSER_WRITESTRING(RivalName, GUILDNAME_MAXIMUM);
		PARSER_WRITESTRING(MasterName, CHARACTER_MAXIMUM);
		PARSER_WRITE32(Members.size());

		for (auto it = Members.begin(); it != Members.end(); it++)
		{
			EX2GS_GUILD_MEMBER &Member = *it;

			PARSER_WRITE(*static_cast<EX2GS_GUILD_MEMBER_HEADER*>(&Member));
			PARSER_WRITESTRING(Member.Name, CHARACTER_MAXIMUM);
		}

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, STS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<EX2GS_RESPONSE_GUILD_HEADER*>(this));
		PARSER_WRITESTRING(GuildName, GUILDNAME_MAXIMUM);
		PARSER_WRITESTRING(RivalName, GUILDNAME_MAXIMUM);
		PARSER_WRITESTRING(MasterName, CHARACTER_MAXIMUM);
		PARSER_WRITE32(Members.size());

		for (auto it = Members.begin(); it != Members.end(); it++)
		{
			EX2GS_GUILD_MEMBER &Member = *it;

			PARSER_WRITE(*static_cast<EX2GS_GUILD_MEMBER_HEADER*>(&Member));
			PARSER_WRITESTRING(Member.Name, CHARACTER_MAXIMUM);
		}

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<EX2GS_RESPONSE_GUILD_HEADER*>(this));
		PARSER_READSTRING(GuildName, GUILDNAME_MAXIMUM);
		PARSER_READSTRING(RivalName, GUILDNAME_MAXIMUM);
		PARSER_READSTRING(MasterName, CHARACTER_MAXIMUM);

		mu_uint32 membersCount = 0;
		PARSER_READ(membersCount);
		Members.resize(membersCount);

		for (auto it = Members.begin(); it != Members.end(); it++)
		{
			EX2GS_GUILD_MEMBER &Member = *it;

			PARSER_READ(*static_cast<EX2GS_GUILD_MEMBER_HEADER*>(&Member));
			PARSER_READSTRING(Member.Name, CHARACTER_MAXIMUM);
		}

		return true;
	}
};

#pragma pack(push, 1)
class EX2GS_RESPONSE_GUILDMEMBER_DATA
{
public:
	EX2GS_RESPONSE_GUILDMEMBER_DATA()
	{
		CharacterIndex = -1;
		GuildIndex = -1;
		Level = 0;
		Status = 0;
	}

public:
	mu_uint64 Identifier;
	mu_uint32 Index;
	mu_int32 CharacterIndex;
	mu_int32 GuildIndex;
	mu_uint8 Level;
	mu_uint8 Status;
};
#pragma pack(pop)

class EX2GS_RESPONSE_GUILDMEMBER : public MU_SERVER_TO_SERVER_HEADEREX, public EX2GS_RESPONSE_GUILDMEMBER_DATA
{
public:
	EX2GS_RESPONSE_GUILDMEMBER()
	{
		Code = GS2EX_PROT_GUILDMEMBERINFO;
	}

	virtual mu_uint32 Length() override
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<EX2GS_RESPONSE_GUILDMEMBER_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, STS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<EX2GS_RESPONSE_GUILDMEMBER_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<EX2GS_RESPONSE_GUILDMEMBER_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class GS2EX_REQUEST_CREATEGUILD_DATA
{
public:
	mu_uint64 Identifier;
	mu_uint32 Index;
	mu_int32 GuildMasterIndex;
	mu_uint8 MarkBuffer[MAX_GUILDMARKBUFFER];
};
#pragma pack(pop)

class GS2EX_REQUEST_CREATEGUILD_DATAEX
{
public:
	mu_string GuildName;
};

class GS2EX_REQUEST_CREATEGUILD : public MU_SERVER_TO_SERVER_HEADEREX,
	public GS2EX_REQUEST_CREATEGUILD_DATA,
	public GS2EX_REQUEST_CREATEGUILD_DATAEX
{
public:
	GS2EX_REQUEST_CREATEGUILD()
	{
		Code = GS2EX_PROT_CREATEGUILD;
	}

	virtual mu_uint32 Length() override
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2EX_REQUEST_CREATEGUILD_DATA*>(this));
		PARSER_WRITESTRING(GuildName, GUILDNAME_MAXIMUM);

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2EX_REQUEST_CREATEGUILD_DATA*>(this));
		PARSER_WRITESTRING(GuildName, GUILDNAME_MAXIMUM);

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<GS2EX_REQUEST_CREATEGUILD_DATA*>(this));
		PARSER_READSTRING(GuildName, GUILDNAME_MAXIMUM);

		return true;
	}
};

enum EX2GS_CREATEGUILD_RESULT
{
	EX2GS_CREATEGUILD_SUCCESS = 0,
	EX2GS_CREATEGUILD_ALREADYEXISTS,
	EX2GS_CREATEGUILD_ALREADYHASGUILD,
	EX2GS_CREATEGUILD_UNEXPECTEDERROR,
};

#pragma pack(push, 1)
class EX2GS_RESPONSE_CREATEGUILD_DATA
{
public:
	mu_uint64 Identifier;
	mu_uint32 Index;
	mu_int32 CharacterIndex;
	mu_int32 GuildIndex;
	mu_uint8 Result;
};
#pragma pack(pop)

class EX2GS_RESPONSE_CREATEGUILD : public MU_SERVER_TO_SERVER_HEADEREX,
	public EX2GS_RESPONSE_CREATEGUILD_DATA
{
public:
	EX2GS_RESPONSE_CREATEGUILD()
	{
		Code = GS2EX_PROT_CREATEGUILD;
	}

	virtual mu_uint32 Length() override
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<EX2GS_RESPONSE_CREATEGUILD_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<EX2GS_RESPONSE_CREATEGUILD_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<EX2GS_RESPONSE_CREATEGUILD_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class GS2EX_REQUEST_DESTROYGUILD_DATA
{
public:
	mu_uint64 Identifier;
	mu_uint32 Index;
	mu_int32 CharacterIndex;
	mu_int32 GuildIndex;
};
#pragma pack(pop)

class GS2EX_REQUEST_DESTROYGUILD : public MU_SERVER_TO_SERVER_HEADEREX, public GS2EX_REQUEST_DESTROYGUILD_DATA
{
public:
	GS2EX_REQUEST_DESTROYGUILD()
	{
		Code = GS2EX_PROT_DESTROYGUILD;
	}

	virtual mu_uint32 Length() override
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2EX_REQUEST_DESTROYGUILD_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2EX_REQUEST_DESTROYGUILD_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<GS2EX_REQUEST_DESTROYGUILD_DATA*>(this));

		return true;
	}
};

enum EX2GS_DESTROYGUILD_RESULT
{
	EX2GS_DESTROYGUILD_SUCCESS = 0,
	EX2GS_DESTROYGUILD_UNEXPECTEDERROR,
};

#pragma pack(push, 1)
class EX2GS_RESPONSE_DESTROYGUILD_DATA
{
public:
	mu_uint64 Identifier;
	mu_uint32 Index;
	mu_int32 CharacterIndex;
	mu_int32 GuildIndex;
	mu_uint8 Result;
};
#pragma pack(pop)

class EX2GS_RESPONSE_DESTROYGUILD : public MU_SERVER_TO_SERVER_HEADEREX,
	public EX2GS_RESPONSE_DESTROYGUILD_DATA
{
public:
	EX2GS_RESPONSE_DESTROYGUILD()
	{
		Code = GS2EX_PROT_DESTROYGUILD;
	}

	virtual mu_uint32 Length() override
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<EX2GS_RESPONSE_DESTROYGUILD_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<EX2GS_RESPONSE_DESTROYGUILD_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<EX2GS_RESPONSE_DESTROYGUILD_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class GS2EX_REQUEST_DELETEMEMBER_DATA
{
public:
	mu_uint64 Identifier;
	mu_uint32 Index;
	mu_int32 CharacterIndex;
	mu_int32 GuildIndex;
	mu_int32 MemberIndex;
};
#pragma pack(pop)

class GS2EX_REQUEST_DELETEMEMBER : public MU_SERVER_TO_SERVER_HEADEREX, public GS2EX_REQUEST_DELETEMEMBER_DATA
{
public:
	GS2EX_REQUEST_DELETEMEMBER()
	{
		Code = GS2EX_PROT_DELETEMEMBER;
	}

	virtual mu_uint32 Length() override
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2EX_REQUEST_DELETEMEMBER_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2EX_REQUEST_DELETEMEMBER_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<GS2EX_REQUEST_DELETEMEMBER_DATA*>(this));

		return true;
	}
};

enum EX2GS_DELETEMEMBER_RESULT
{
	EX2GS_DELETEMEMBER_SUCCESS = 0,
	EX2GS_DELETEMEMBER_UNEXPECTEDERROR,
};

#pragma pack(push, 1)
class EX2GS_RESPONSE_DELETEMEMBER_DATA
{
public:
	mu_uint64 Identifier;
	mu_uint32 Index;
	mu_int32 CharacterIndex;
	mu_int32 GuildIndex;
	mu_int32 MemberIndex;
	mu_uint8 Result;
};
#pragma pack(pop)

class EX2GS_RESPONSE_DELETEMEMBER : public MU_SERVER_TO_SERVER_HEADEREX,
	public EX2GS_RESPONSE_DELETEMEMBER_DATA
{
public:
	EX2GS_RESPONSE_DELETEMEMBER()
	{
		Code = GS2EX_PROT_DELETEMEMBER;
	}

	virtual mu_uint32 Length() override
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<EX2GS_RESPONSE_DELETEMEMBER_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<EX2GS_RESPONSE_DELETEMEMBER_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<EX2GS_RESPONSE_DELETEMEMBER_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class GS2EX_REQUEST_ADDMEMBER_DATA
{
public:
	mu_uint64 Identifier;
	mu_uint32 Index;
	mu_int32 CharacterIndex;
	mu_int32 GuildIndex;
};
#pragma pack(pop)

class GS2EX_REQUEST_ADDMEMBER_DATAEX
{
public:
	mu_string CharacterName;
};

class GS2EX_REQUEST_ADDMEMBER : public MU_SERVER_TO_SERVER_HEADEREX,
	public GS2EX_REQUEST_ADDMEMBER_DATA,
	public GS2EX_REQUEST_ADDMEMBER_DATAEX
{
public:
	GS2EX_REQUEST_ADDMEMBER()
	{
		Code = GS2EX_PROT_ADDMEMBER;
	}

	virtual mu_uint32 Length() override
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2EX_REQUEST_ADDMEMBER_DATA*>(this));
		PARSER_WRITESTRING(CharacterName, CHARACTER_MAXIMUM);

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2EX_REQUEST_ADDMEMBER_DATA*>(this));
		PARSER_WRITESTRING(CharacterName, CHARACTER_MAXIMUM);

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<GS2EX_REQUEST_ADDMEMBER_DATA*>(this));
		PARSER_READSTRING(CharacterName, CHARACTER_MAXIMUM);

		return true;
	}
};

enum EX2GS_ADDMEMBER_RESULT
{
	EX2GS_ADDMEMBER_SUCCESS = 0,
	EX2GS_ADDMEMBER_UNEXPECTEDERROR,
};

#pragma pack(push, 1)
class EX2GS_RESPONSE_ADDMEMBER_DATA
{
public:
	mu_uint64 Identifier;
	mu_int16 GroupIndex;
	mu_int16 ServerIndex;
	mu_uint32 Index;
	mu_int32 CharacterIndex;
	mu_int32 GuildIndex;
	mu_uint8 Result;
};
#pragma pack(pop)

class EX2GS_RESPONSE_ADDMEMBER_DATAEX
{
public:
	mu_string CharacterName;
};

class EX2GS_RESPONSE_ADDMEMBER : public MU_SERVER_TO_SERVER_HEADEREX,
	public EX2GS_RESPONSE_ADDMEMBER_DATA,
	public EX2GS_RESPONSE_ADDMEMBER_DATAEX
{
public:
	EX2GS_RESPONSE_ADDMEMBER()
	{
		Code = GS2EX_PROT_ADDMEMBER;
	}

	virtual mu_uint32 Length() override
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<EX2GS_RESPONSE_ADDMEMBER_DATA*>(this));
		PARSER_WRITESTRING(CharacterName, CHARACTER_MAXIMUM);

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<EX2GS_RESPONSE_ADDMEMBER_DATA*>(this));
		PARSER_WRITESTRING(CharacterName, CHARACTER_MAXIMUM);

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<EX2GS_RESPONSE_ADDMEMBER_DATA*>(this));
		PARSER_READSTRING(CharacterName, CHARACTER_MAXIMUM);

		return true;
	}
};

#pragma pack(push, 1)
class GS2EX_RESPONSE_MEMBERCONNECTED_DATA
{
public:
	mu_uint64 Identifier;
	mu_uint32 Index;
	mu_int32 CharacterIndex;
	mu_int32 GuildIndex;
};
#pragma pack(pop)

class GS2EX_RESPONSE_MEMBERCONNECTED : public MU_SERVER_TO_SERVER_HEADEREX, public GS2EX_RESPONSE_MEMBERCONNECTED_DATA
{
public:
	GS2EX_RESPONSE_MEMBERCONNECTED()
	{
		Code = GS2EX_PROT_MEMBERCONNECTED;
	}

	virtual mu_uint32 Length() override
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2EX_RESPONSE_MEMBERCONNECTED_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, STS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2EX_RESPONSE_MEMBERCONNECTED_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<GS2EX_RESPONSE_MEMBERCONNECTED_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class EX2GS_RESPONSE_MEMBERCONNECTED_DATA
{
public:
	mu_int16 GroupIndex;
	mu_int16 ServerIndex;
	mu_int32 CharacterIndex;
	mu_int32 GuildIndex;
};
#pragma pack(pop)

class EX2GS_RESPONSE_MEMBERCONNECTED : public MU_SERVER_TO_SERVER_HEADEREX, public EX2GS_RESPONSE_MEMBERCONNECTED_DATA
{
public:
	EX2GS_RESPONSE_MEMBERCONNECTED()
	{
		Code = GS2EX_PROT_MEMBERCONNECTED;
	}

	virtual mu_uint32 Length() override
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<EX2GS_RESPONSE_MEMBERCONNECTED_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, STS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<EX2GS_RESPONSE_MEMBERCONNECTED_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<EX2GS_RESPONSE_MEMBERCONNECTED_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class GS2EX_RESPONSE_MEMBERDISCONNECTED_DATA
{
public:
	mu_int32 CharacterIndex;
	mu_int32 GuildIndex;
};
#pragma pack(pop)

class GS2EX_RESPONSE_MEMBERDISCONNECTED : public MU_SERVER_TO_SERVER_HEADEREX, public GS2EX_RESPONSE_MEMBERDISCONNECTED_DATA
{
public:
	GS2EX_RESPONSE_MEMBERDISCONNECTED()
	{
		Code = GS2EX_PROT_MEMBERDISCONNECTED;
	}

	virtual mu_uint32 Length() override
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2EX_RESPONSE_MEMBERDISCONNECTED_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, STS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2EX_RESPONSE_MEMBERDISCONNECTED_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<GS2EX_RESPONSE_MEMBERDISCONNECTED_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class EX2GS_RESPONSE_MEMBERDISCONNECTED_DATA
{
public:
	mu_int32 CharacterIndex;
	mu_int32 GuildIndex;
};
#pragma pack(pop)

class EX2GS_RESPONSE_MEMBERDISCONNECTED : public MU_SERVER_TO_SERVER_HEADEREX, public EX2GS_RESPONSE_MEMBERDISCONNECTED_DATA
{
public:
	EX2GS_RESPONSE_MEMBERDISCONNECTED()
	{
		Code = GS2EX_PROT_MEMBERDISCONNECTED;
	}

	virtual mu_uint32 Length() override
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<EX2GS_RESPONSE_MEMBERDISCONNECTED_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, STS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<EX2GS_RESPONSE_MEMBERDISCONNECTED_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<EX2GS_RESPONSE_MEMBERDISCONNECTED_DATA*>(this));

		return true;
	}
};

class GS2EX_RESPONSE_MEMBERCONNECTED_BATCH_DATA
{
public:
	struct GUILD_DATA
	{
		mu_int32 GuildIndex;
		std::vector<mu_int32> Members;
	};

public:
	std::vector<GUILD_DATA> Guilds;
};

class GS2EX_RESPONSE_MEMBERCONNECTED_BATCH : public MU_SERVER_TO_SERVER_HEADEREX, public GS2EX_RESPONSE_MEMBERCONNECTED_BATCH_DATA
{
public:
	GS2EX_RESPONSE_MEMBERCONNECTED_BATCH()
	{
		Code = GS2EX_PROT_MEMBERCONNECTED_BATCH;
	}

	virtual mu_uint32 Length() override
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));

		PARSER_WRITE32(Guilds.size());

		for (auto it = Guilds.begin(); it != Guilds.end(); it++)
		{
			GUILD_DATA &guildData = (*it);
			PARSER_WRITE(guildData.GuildIndex);
			PARSER_WRITEVECTOR(guildData.Members);
		}

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, STS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));

		PARSER_WRITE32(Guilds.size());

		for (auto it = Guilds.begin(); it != Guilds.end(); it++)
		{
			GUILD_DATA &guildData = (*it);
			PARSER_WRITE(guildData.GuildIndex);
			PARSER_WRITEVECTOR(guildData.Members);
		}

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));

		mu_uint32 guildsCount = 0;
		PARSER_READ(guildsCount);
		Guilds.resize(guildsCount);

		for (auto it = Guilds.begin(); it != Guilds.end(); it++)
		{
			GUILD_DATA &guildData = (*it);
			PARSER_READ(guildData.GuildIndex);
			PARSER_READVECTOR(guildData.Members);
		}

		return true;
	}
};

class EX2GS_RESPONSE_MEMBERDISCONNECTED_BATCH_DATA
{
public:
	struct GUILD_DATA 
	{
		mu_int32 GuildIndex;
		std::vector<mu_int32> Members;
	};

public:
	std::vector<GUILD_DATA> Guilds;
};

class EX2GS_RESPONSE_MEMBERDISCONNECTED_BATCH : public MU_SERVER_TO_SERVER_HEADEREX, public EX2GS_RESPONSE_MEMBERDISCONNECTED_BATCH_DATA
{
public:
	EX2GS_RESPONSE_MEMBERDISCONNECTED_BATCH()
	{
		Code = GS2EX_PROT_MEMBERDISCONNECTED_BATCH;
	}

	virtual mu_uint32 Length() override
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));

		PARSER_WRITE32(Guilds.size());

		for (auto it = Guilds.begin(); it != Guilds.end(); it++)
		{
			GUILD_DATA &guildData = (*it);
			PARSER_WRITE(guildData.GuildIndex);
			PARSER_WRITEVECTOR(guildData.Members);
		}

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, STS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));

		PARSER_WRITE32(Guilds.size());

		for (auto it = Guilds.begin(); it != Guilds.end(); it++)
		{
			GUILD_DATA &guildData = (*it);
			PARSER_WRITE(guildData.GuildIndex);
			PARSER_WRITEVECTOR(guildData.Members);
		}

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));

		mu_uint32 guildsCount = 0;
		PARSER_READ(guildsCount);
		Guilds.resize(guildsCount);

		for (auto it = Guilds.begin(); it != Guilds.end(); it++)
		{
			GUILD_DATA &guildData = (*it);
			PARSER_READ(guildData.GuildIndex);
			PARSER_READVECTOR(guildData.Members);
		}

		return true;
	}
};

#pragma pack(push, 1)
class GS2EX_REQUEST_MEMBERSTATUS_CHANGE_DATA
{
public:
	mu_int32 GuildIndex;
	mu_int32 CharacterIndex;
	mu_int16 GuildStatus;
	mu_uint16 CharacterLevel;
};
#pragma pack(pop)

class GS2EX_REQUEST_MEMBERSTATUS_CHANGE : public MU_SERVER_TO_SERVER_HEADEREX, public GS2EX_REQUEST_MEMBERSTATUS_CHANGE_DATA
{
public:
	GS2EX_REQUEST_MEMBERSTATUS_CHANGE()
	{
		Code = GS2EX_PROT_MEMBERSTATUS_CHANGE;
	}

	virtual mu_uint32 Length() override
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2EX_REQUEST_MEMBERSTATUS_CHANGE_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2EX_REQUEST_MEMBERSTATUS_CHANGE_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<GS2EX_REQUEST_MEMBERSTATUS_CHANGE_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class EX2GS_RESPONSE_MEMBERSTATUS_CHANGE_DATA
{
public:
	mu_int32 GuildIndex;
	mu_int32 CharacterIndex;
	mu_int16 GuildStatus;
};
#pragma pack(pop)

class EX2GS_RESPONSE_MEMBERSTATUS_CHANGED : public MU_SERVER_TO_SERVER_HEADEREX,
	public EX2GS_RESPONSE_MEMBERSTATUS_CHANGE_DATA
{
public:
	EX2GS_RESPONSE_MEMBERSTATUS_CHANGED()
	{
		Code = GS2EX_PROT_MEMBERSTATUS_CHANGE;
	}

	virtual mu_uint32 Length() override
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<EX2GS_RESPONSE_MEMBERSTATUS_CHANGE_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<EX2GS_RESPONSE_MEMBERSTATUS_CHANGE_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<EX2GS_RESPONSE_MEMBERSTATUS_CHANGE_DATA*>(this));

		return true;
	}
};

#endif