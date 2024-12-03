#ifndef __CLTOGS_TRADE_H__
#define __CLTOGS_TRADE_H__

#pragma once

namespace TRADE_RESULT
{
	enum
	{
		RESULT_FAILED = 0,
		RESULT_DENIED,
		RESULT_CANCELLED,
		RESULT_INVENTORYFULL,
		RESULT_BLOCKEDITEM,
		RESULT_OK,
		RESULT_SUCCESS,
	};
};

#pragma pack(push, 1)
class ARM_PACKED CL2GS_REQUEST_TRADE_DATA
{
public:
	mu_int16 Key;
};
#pragma pack(pop)

class CL2GS_REQUEST_TRADE : public MU_CLIENT_TO_SERVER_HEADEREX, public CL2GS_REQUEST_TRADE_DATA
{
public:
	CL2GS_REQUEST_TRADE()
	{
		Code = GS2CL_PROT_TRADEREQUEST;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_REQUEST_TRADE_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_REQUEST_TRADE_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<CL2GS_REQUEST_TRADE_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_RECEIVE_TRADEREQUEST_DATA
{
public:
	mu_int16 Key;
};
#pragma pack(pop)

class GS2CL_RECEIVE_TRADEREQUEST : public MU_SERVER_TO_CLIENT_HEADEREX, public GS2CL_RECEIVE_TRADEREQUEST_DATA
{
public:
	GS2CL_RECEIVE_TRADEREQUEST()
	{
		Code = GS2CL_PROT_TRADEREQUEST;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_TRADEREQUEST_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_TRADEREQUEST_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_RECEIVE_TRADEREQUEST_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED CL2GS_RECEIVE_TRADEANSWER_DATA
{
public:
	mu_boolean Answer;
};
#pragma pack(pop)

class CL2GS_RECEIVE_TRADEANSWER : public MU_CLIENT_TO_SERVER_HEADEREX, public CL2GS_RECEIVE_TRADEANSWER_DATA
{
public:
	CL2GS_RECEIVE_TRADEANSWER()
	{
		Code = GS2CL_PROT_TRADEANSWER;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_RECEIVE_TRADEANSWER_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_RECEIVE_TRADEANSWER_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<CL2GS_RECEIVE_TRADEANSWER_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_RECEIVE_TRADEANSWER_DATA
{
public:
	mu_uint8 Result;
	mu_int16 Key;
	mu_uint16 Level;
	mu_uint32 GuildKey;
};
#pragma pack(pop)

class GS2CL_RECEIVE_TRADEANSWER : public MU_SERVER_TO_CLIENT_HEADEREX, public GS2CL_RECEIVE_TRADEANSWER_DATA
{
public:
	GS2CL_RECEIVE_TRADEANSWER()
	{
		Code = GS2CL_PROT_TRADEANSWER;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_TRADEANSWER_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_TRADEANSWER_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_RECEIVE_TRADEANSWER_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_RECEIVE_TRADERESULT_DATA
{
public:
	mu_uint8 Result;
};
#pragma pack(pop)

class GS2CL_RECEIVE_TRADERESULT : public MU_SERVER_TO_CLIENT_HEADEREX, public GS2CL_RECEIVE_TRADERESULT_DATA
{
public:
	GS2CL_RECEIVE_TRADERESULT()
	{
		Code = GS2CL_PROT_TRADERESULT;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_TRADERESULT_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_TRADERESULT_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_RECEIVE_TRADERESULT_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED CL2GS_RECEIVE_TRADEMONEY_DATA
{
public:
	mu_int64 Money;
};
#pragma pack(pop)

class CL2GS_RECEIVE_TRADEMONEY : public MU_CLIENT_TO_SERVER_HEADEREX, public CL2GS_RECEIVE_TRADEMONEY_DATA
{
public:
	CL2GS_RECEIVE_TRADEMONEY()
	{
		Code = GS2CL_PROT_TRADEMONEY;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_RECEIVE_TRADEMONEY_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_RECEIVE_TRADEMONEY_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<CL2GS_RECEIVE_TRADEMONEY_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED CL2GS_RECEIVE_TRADEOK_DATA
{
public:
	mu_boolean Answer;
};
#pragma pack(pop)

class CL2GS_RECEIVE_TRADEOK : public MU_CLIENT_TO_SERVER_HEADEREX, public CL2GS_RECEIVE_TRADEOK_DATA
{
public:
	CL2GS_RECEIVE_TRADEOK()
	{
		Code = GS2CL_PROT_TRADEOK;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_RECEIVE_TRADEOK_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_RECEIVE_TRADEOK_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<CL2GS_RECEIVE_TRADEOK_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_RECEIVE_TRADEOK_DATA
{
public:
	mu_boolean Result;
	mu_boolean WaitTime;
};
#pragma pack(pop)

class GS2CL_RECEIVE_TRADEOK : public MU_SERVER_TO_CLIENT_HEADEREX, public GS2CL_RECEIVE_TRADEOK_DATA
{
public:
	GS2CL_RECEIVE_TRADEOK()
	{
		Code = GS2CL_PROT_TRADEOK;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_TRADEOK_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_TRADEOK_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_RECEIVE_TRADEOK_DATA*>(this));

		return true;
	}
};

#endif