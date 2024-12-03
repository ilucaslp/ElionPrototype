#ifndef __CLTOGS_STORAGE_H__
#define __CLTOGS_STORAGE_H__

#pragma once

namespace STORAGE
{
	enum STATE_RESULT
	{
		SR_UNLOCKED,
		SR_LOCKED,
		SR_ALLOWED,
		SR_INVALIDPASSWORD,
	};
};

#pragma pack(push, 1)
class ARM_PACKED CL2GS_REQUEST_STORAGEGOLD_DATA
{
public:
	mu_boolean IsWithdraw;
	mu_int64 Gold;
};
#pragma pack(pop)

class CL2GS_REQUEST_STORAGEGOLD : public MU_CLIENT_TO_SERVER_HEADEREX, public CL2GS_REQUEST_STORAGEGOLD_DATA
{
public:
	CL2GS_REQUEST_STORAGEGOLD()
	{
		Code = GS2CL_PROT_STORAGEGOLD;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_REQUEST_STORAGEGOLD_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_REQUEST_STORAGEGOLD_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<CL2GS_REQUEST_STORAGEGOLD_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED CL2GS_REQUEST_STORAGELOCK_HEADER
{
public:
	mu_uint8 RequestType;
};
#pragma pack(pop)

class CL2GS_REQUEST_STORAGELOCK_HEADEREX
{
public:
	mu_string Password;
};

class CL2GS_REQUEST_STORAGELOCK : public MU_CLIENT_TO_SERVER_HEADEREX,
	public CL2GS_REQUEST_STORAGELOCK_HEADER,
	public CL2GS_REQUEST_STORAGELOCK_HEADEREX
{
public:
	enum
	{
		REQUEST_LOCK,
		REQUEST_UNLOCK,
		REQUEST_ALLOW,
	};

public:
	CL2GS_REQUEST_STORAGELOCK()
	{
		Code = GS2CL_PROT_STORAGELOCK;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_REQUEST_STORAGELOCK_HEADER*>(this));
		PARSER_WRITESTRING(Password, STORAGE_MAXIMUM);

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_REQUEST_STORAGELOCK_HEADER*>(this));
		PARSER_WRITESTRING(Password, STORAGE_MAXIMUM);

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<CL2GS_REQUEST_STORAGELOCK_HEADER*>(this));
		PARSER_READSTRING(Password, STORAGE_MAXIMUM);

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_RECEIVE_STORAGESTATE_DATA
{
public:
	mu_uint8 State;
};
#pragma pack(pop)

class GS2CL_RECEIVE_STORAGESTATE : public MU_SERVER_TO_CLIENT_HEADEREX, public GS2CL_RECEIVE_STORAGESTATE_DATA
{
public:
	GS2CL_RECEIVE_STORAGESTATE()
	{
		Code = GS2CL_PROT_STORAGESTATE;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_STORAGESTATE_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_STORAGESTATE_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_RECEIVE_STORAGESTATE_DATA*>(this));

		return true;
	}
};

#endif