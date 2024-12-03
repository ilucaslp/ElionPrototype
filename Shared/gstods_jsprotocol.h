#ifndef __GSTOJS_PROTOCOL_H__
#define __GSTOJS_PROTOCOL_H__

#pragma once

#pragma pack(push, 1)
class GS2JS_REQUEST_LOGIN_DATA
{
public:
	mu_uint64 Identifier;
	mu_uint32 Index;
	mu_uint8 PasswordHash[HASH_LENGTH];
};
#pragma pack(pop)

class GS2JS_REQUEST_LOGIN_DATAEX
{
public:
	mu_string Username;
};

class GS2JS_REQUEST_LOGIN : public MU_SERVER_TO_SERVER_HEADEREX,
							public GS2JS_REQUEST_LOGIN_DATA,
							public GS2JS_REQUEST_LOGIN_DATAEX
{
public:
	GS2JS_REQUEST_LOGIN()
	{
		Code = GS2JS_PROT_USERLOGIN;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2JS_REQUEST_LOGIN_DATA*>(this));
		PARSER_WRITESTRING(Username, ACCOUNT_MAXIMUM);

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2JS_REQUEST_LOGIN_DATA*>(this));
		PARSER_WRITESTRING(Username, ACCOUNT_MAXIMUM);

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<GS2JS_REQUEST_LOGIN_DATA*>(this));
		PARSER_READSTRING(Username, ACCOUNT_MAXIMUM);

		return true;
	}
};

#pragma pack(push, 1)
class JS2GS_RESPONSE_LOGIN_DATA
{
public:
	mu_uint64 Identifier;
	mu_uint32 Index;
	mu_uint32 JoinIndex;
	mu_int32 AccountIndex;
	mu_uint8 Response;
};
#pragma pack(pop)

class JS2GS_RESPONSE_LOGIN : public MU_SERVER_TO_SERVER_HEADEREX, public JS2GS_RESPONSE_LOGIN_DATA
{
public:
	JS2GS_RESPONSE_LOGIN()
	{
		Code = GS2JS_PROT_USERLOGIN;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<JS2GS_RESPONSE_LOGIN_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<JS2GS_RESPONSE_LOGIN_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<JS2GS_RESPONSE_LOGIN_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class GS2JS_DISCONNECT_ACCOUNT_DATA
{
public:
	mu_uint32 JoinIndex;
	mu_int32 AccountIndex;
};
#pragma pack(pop)

class GS2JS_DISCONNECT_ACCOUNT : public MU_SERVER_TO_SERVER_HEADEREX, public GS2JS_DISCONNECT_ACCOUNT_DATA
{
public:
	GS2JS_DISCONNECT_ACCOUNT()
	{
		Code = GS2JS_PROT_USERDISCONNECT;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2JS_DISCONNECT_ACCOUNT_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2JS_DISCONNECT_ACCOUNT_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<GS2JS_DISCONNECT_ACCOUNT_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class JS2GS_REQUEST_FORCEDISCONNECT_DATA
{
public:
	mu_uint32 JoinIndex;
	mu_int32 AccountIndex;
};
#pragma pack(pop)

class JS2GS_REQUEST_FORCEDISCONNECT_DATAEX
{
public:
	mu_string AccountName;
};

class JS2GS_REQUEST_FORCEDISCONNECT : public MU_SERVER_TO_SERVER_HEADEREX,
	public JS2GS_REQUEST_FORCEDISCONNECT_DATA,
	public JS2GS_REQUEST_FORCEDISCONNECT_DATAEX
{
public:
	JS2GS_REQUEST_FORCEDISCONNECT()
	{
		Code = GS2JS_PROT_USERDISCONNECT;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<JS2GS_REQUEST_FORCEDISCONNECT_DATA*>(this));
		PARSER_WRITESTRING(AccountName, ACCOUNT_MAXIMUM);

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<JS2GS_REQUEST_FORCEDISCONNECT_DATA*>(this));
		PARSER_WRITESTRING(AccountName, ACCOUNT_MAXIMUM);

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<JS2GS_REQUEST_FORCEDISCONNECT_DATA*>(this));
		PARSER_READSTRING(AccountName, ACCOUNT_MAXIMUM);

		return true;
	}
};

#pragma pack(push, 1)
class GS2JS_RECEIVE_HELPEROFFLINE_DATA
{
public:
	mu_uint32 JoinIndex;
	mu_int32 AccountIndex;
};
#pragma pack(pop)

class GS2JS_RECEIVE_HELPEROFFLINE : public MU_SERVER_TO_SERVER_HEADEREX, public GS2JS_RECEIVE_HELPEROFFLINE_DATA
{
public:
	GS2JS_RECEIVE_HELPEROFFLINE()
	{
		Code = GS2JS_PROT_USERHELPER;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2JS_RECEIVE_HELPEROFFLINE_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2JS_RECEIVE_HELPEROFFLINE_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<GS2JS_RECEIVE_HELPEROFFLINE_DATA*>(this));

		return true;
	}
};

#endif