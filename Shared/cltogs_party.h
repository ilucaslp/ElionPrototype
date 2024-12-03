#ifndef __CLTOGS_PARTY_H__
#define __CLTOGS_PARTY_H__

#pragma once

namespace PARTY_RESULT
{
	enum
	{
		RESULT_FAILED = 0,
		RESULT_DENIED,
		RESULT_FULL,
		RESULT_DISCONNECTED,
		RESULT_ALREADYPARTY,
		RESULT_JUSTLEFT,
		RESULT_OPPOSEGENS,
		RESULT_BATTLEZONE1,
		RESULT_BATTLEZONE2,
	};
};

#pragma pack(push, 1)
class ARM_PACKED CL2GS_RECEIVE_PARTYREQUEST_DATA
{
public:
	mu_int16 Key;
};
#pragma pack(pop)

class CL2GS_RECEIVE_PARTYREQUEST : public MU_CLIENT_TO_SERVER_HEADEREX, public CL2GS_RECEIVE_PARTYREQUEST_DATA
{
public:
	CL2GS_RECEIVE_PARTYREQUEST()
	{
		Code = GS2CL_PROT_PARTYREQUEST;
	}

	virtual mu_uint32 Length() override
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_RECEIVE_PARTYREQUEST_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_RECEIVE_PARTYREQUEST_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<CL2GS_RECEIVE_PARTYREQUEST_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_RECEIVE_PARTYREQUEST_DATA
{
public:
	mu_int16 Key;
};
#pragma pack(pop)

class GS2CL_RECEIVE_PARTYREQUEST : public MU_SERVER_TO_CLIENT_HEADEREX, public GS2CL_RECEIVE_PARTYREQUEST_DATA
{
public:
	GS2CL_RECEIVE_PARTYREQUEST()
	{
		Code = GS2CL_PROT_PARTYREQUEST;
	}

	virtual mu_uint32 Length() override
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_PARTYREQUEST_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_PARTYREQUEST_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_RECEIVE_PARTYREQUEST_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED CL2GS_RECEIVE_PARTYANSWER_DATA
{
public:
	mu_boolean Answer;
};
#pragma pack(pop)

class CL2GS_RECEIVE_PARTYANSWER : public MU_CLIENT_TO_SERVER_HEADEREX, public CL2GS_RECEIVE_PARTYANSWER_DATA
{
public:
	CL2GS_RECEIVE_PARTYANSWER()
	{
		Code = GS2CL_PROT_PARTYANSWER;
	}

	virtual mu_uint32 Length() override
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_RECEIVE_PARTYANSWER_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_RECEIVE_PARTYANSWER_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<CL2GS_RECEIVE_PARTYANSWER_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_RECEIVE_PARTYRESULT_DATA
{
public:
	mu_uint8 Result;
};
#pragma pack(pop)

class GS2CL_RECEIVE_PARTYRESULT : public MU_SERVER_TO_CLIENT_HEADEREX, public GS2CL_RECEIVE_PARTYRESULT_DATA
{
public:
	GS2CL_RECEIVE_PARTYRESULT()
	{
		Code = GS2CL_PROT_PARTYRESULT;
	}

	virtual mu_uint32 Length() override
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_PARTYRESULT_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_PARTYRESULT_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_RECEIVE_PARTYRESULT_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED CL2GS_RECEIVE_PARTYKICK_DATA
{
public:
	mu_uint8 Number;
};
#pragma pack(pop)

class CL2GS_RECEIVE_PARTYKICK : public MU_CLIENT_TO_SERVER_HEADEREX, public CL2GS_RECEIVE_PARTYKICK_DATA
{
public:
	CL2GS_RECEIVE_PARTYKICK()
	{
		Code = GS2CL_PROT_PARTYKICK;
	}

	virtual mu_uint32 Length() override
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_RECEIVE_PARTYKICK_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_RECEIVE_PARTYKICK_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<CL2GS_RECEIVE_PARTYKICK_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_RECEIVE_PARTYKICK_DATA
{
public:
	mu_boolean Success;
};
#pragma pack(pop)

class GS2CL_RECEIVE_PARTYKICK : public MU_SERVER_TO_CLIENT_HEADEREX, public GS2CL_RECEIVE_PARTYKICK_DATA
{
public:
	GS2CL_RECEIVE_PARTYKICK()
	{
		Code = GS2CL_PROT_PARTYKICK;
	}

	virtual mu_uint32 Length() override
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_PARTYKICK_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_PARTYKICK_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_RECEIVE_PARTYKICK_DATA*>(this));

		return true;
	}
};

class GS2CL_VIEWPORT_PARTYLIST_DATA
{
public:
#pragma pack(push, 1)
	class ARM_PACKED MEMBER_DATA
	{
	public:
		mu_int16 Key;
		mu_uint8 Number;
		mu_uint8 Map;
		mu_uint8 X;
		mu_uint8 Y;
		mu_float Life;
		mu_float MaxLife;
	};
#pragma pack(pop)

	class MEMBER_DATA_EX : public MEMBER_DATA
	{
	public:
		mu_string Name;
	};

	std::vector<MEMBER_DATA_EX> Members;
};

class GS2CL_VIEWPORT_PARTYLIST : public MU_VIEWPORT_HEADEREX, public GS2CL_VIEWPORT_PARTYLIST_DATA
{
public:
	GS2CL_VIEWPORT_PARTYLIST()
	{
		Code = GS2CL_PROT_VIEWPORT_PARTYLIST;
	}

	virtual mu_uint32 Length() override
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE32(Members.size());

		for (auto it = Members.begin(); it != Members.end(); it++)
		{
			MEMBER_DATA_EX &member = *it;

			PARSER_WRITESTRING(member.Name, CHARACTER_MAXIMUM);
			PARSER_WRITE(*static_cast<MEMBER_DATA*>(&member));
		}

		return parser.ProcessedLength;
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		Size = bufferLength;
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITE32(Members.size());

		for (auto it = Members.begin(); it != Members.end(); it++)
		{
			MEMBER_DATA_EX &member = *it;

			PARSER_WRITESTRING(member.Name, CHARACTER_MAXIMUM);
			PARSER_WRITE(*static_cast<MEMBER_DATA*>(&member));
		}

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_VIEWPORT_HEADER*>(this));

		mu_uint32 membersCount = 0;
		PARSER_READ(membersCount);
		Members.resize(membersCount);

		for (auto it = Members.begin(); it != Members.end(); it++)
		{
			MEMBER_DATA_EX &member = *it;

			PARSER_READSTRING(member.Name, CHARACTER_MAXIMUM);
			PARSER_READ(*static_cast<MEMBER_DATA*>(&member));
		}

		return true;
	}
};

class GS2CL_VIEWPORT_PARTYINFO_DATA
{
public:
#pragma pack(push, 1)
	class ARM_PACKED MEMBER_DATA
	{
	public:
		mu_int16 Index;
		mu_uint8 Map;
		mu_uint8 X;
		mu_uint8 Y;
		mu_float Life;
		mu_float MaxLife;
	};
#pragma pack(pop)

	std::vector<MEMBER_DATA> Members;
};

class GS2CL_VIEWPORT_PARTYINFO : public MU_VIEWPORT_HEADEREX, public GS2CL_VIEWPORT_PARTYINFO_DATA
{
public:
	GS2CL_VIEWPORT_PARTYINFO()
	{
		Code = GS2CL_PROT_VIEWPORT_PARTYINFO;
	}

	virtual mu_uint32 Length() override
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITEVECTOR(Members);

		return parser.ProcessedLength;
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		Size = bufferLength;
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_WRITEVECTOR(Members);

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_VIEWPORT_HEADER*>(this));
		PARSER_READVECTOR(Members);

		return true;
	}
};

#endif