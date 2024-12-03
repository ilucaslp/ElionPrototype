#ifndef __GSTOCS_PROTOCOL_H__
#define __GSTOCS_PROTOCOL_H__

#pragma once

// GameServer<->ConnectServer
enum GS2CS_PROTOCOL : mu_uint16
{
	GS2CS_PROT_REGISTER = 0x0000,
	GS2CS_PROT_UPDATE = 0x0001,
	GS2CS_PROT_CONNECT = 0x0002,

	GS2CS_PROT_HEARTBEAT = 0xFFFF,
};

class GS2CS_HEARTBEAT : public MU_SERVER_TO_SERVER_HEADEREX
{
public:
	GS2CS_HEARTBEAT()
	{
		Code = GS2CS_PROT_HEARTBEAT;
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

#pragma pack(push, 1)
class GS2CS_REGISTER_DATA
{
public:
	mu_uint8 GroupIndex;
	mu_uint8 ServerIndex;
	mu_uint8 ServerType;
	mu_uint8 ServerFlag;
	mu_uint32 MaxUsers;
};
#pragma pack(pop)

class GS2CS_REGISTER_DATAEX
{
public:
	mu_string Password;
};

class GS2CS_REGISTER : public MU_SERVER_TO_SERVER_HEADEREX, public GS2CS_REGISTER_DATA, public GS2CS_REGISTER_DATAEX
{
public:
	enum
	{
		MAX_PASSWORD_LENGTH = 64,
	};

public:
	GS2CS_REGISTER()
	{
		Code = GS2CS_PROT_REGISTER;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CS_REGISTER_DATA*>(this));
		PARSER_WRITESTRING(Password, MAX_PASSWORD_LENGTH);

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CS_REGISTER_DATA*>(this));
		PARSER_WRITESTRING(Password, MAX_PASSWORD_LENGTH);

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CS_REGISTER_DATA*>(this));
		PARSER_READSTRING(Password, MAX_PASSWORD_LENGTH);

		return true;
	}
};

#pragma pack(push, 1)
class GS2CS_UPDATE_DATA
{
public:
	mu_uint32 CurrentUsers;
	mu_uint32 MaxUsers;
	mu_uint8 ServerType;
	mu_uint8 ServerFlag;
};
#pragma pack(pop)

class GS2CS_UPDATE : public MU_SERVER_TO_SERVER_HEADEREX, public GS2CS_UPDATE_DATA
{
public:
	GS2CS_UPDATE()
	{
		Code = GS2CS_PROT_UPDATE;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CS_UPDATE_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CS_UPDATE_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CS_UPDATE_DATA*>(this));

		return true;
	}
};

#endif