#ifndef __CLTOGS_HELPER_H__
#define __CLTOGS_HELPER_H__

#pragma once

#include "cltogs_helper_structs.h"

#pragma pack(push, 1)
class ARM_PACKED CL2GS_REQUEST_HELPERSTART_DATA
{
public:
	mu_uint8 StartType;
};
#pragma pack(pop)

class CL2GS_REQUEST_HELPERSTART : public MU_CLIENT_TO_SERVER_HEADEREX, public CL2GS_REQUEST_HELPERSTART_DATA
{
public:
	enum
	{
		ONLINE_MODE,
		OFFLINE_MODE,
	};

public:
	CL2GS_REQUEST_HELPERSTART()
	{
		Code = GS2CL_PROT_HELPERSTART;
	}

	virtual mu_uint32 Length() override
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_REQUEST_HELPERSTART_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_REQUEST_HELPERSTART_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<CL2GS_REQUEST_HELPERSTART_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_RECEIVE_HELPERSTART_DATA
{
public:
	mu_uint8 Result;
};
#pragma pack(pop)

class GS2CL_RECEIVE_HELPERSTART : public MU_SERVER_TO_CLIENT_HEADEREX, public GS2CL_RECEIVE_HELPERSTART_DATA
{
public:
	enum
	{
		RESULT_SUCCESS,
	};

public:
	GS2CL_RECEIVE_HELPERSTART()
	{
		Code = GS2CL_PROT_HELPERSTART;
	}

	virtual mu_uint32 Length() override
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_HELPERSTART_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_HELPERSTART_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_RECEIVE_HELPERSTART_DATA*>(this));

		return true;
	}
};

class CL2GS_REQUEST_HELPERSTOP : public MU_CLIENT_TO_SERVER_HEADEREX
{
public:
	CL2GS_REQUEST_HELPERSTOP()
	{
		Code = GS2CL_PROT_HELPERSTOP;
	}

	virtual mu_uint32 Length() override
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_RECEIVE_HELPERSTOP_DATA
{
public:
	mu_uint8 Result;
};
#pragma pack(pop)

class GS2CL_RECEIVE_HELPERSTOP : public MU_SERVER_TO_CLIENT_HEADEREX, public GS2CL_RECEIVE_HELPERSTOP_DATA
{
public:
	enum
	{
		RESULT_SUCCESS,
	};

public:
	GS2CL_RECEIVE_HELPERSTOP()
	{
		Code = GS2CL_PROT_HELPERSTOP;
	}

	virtual mu_uint32 Length() override
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_HELPERSTOP_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_HELPERSTOP_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_RECEIVE_HELPERSTOP_DATA*>(this));

		return true;
	}
};

class CL2GS_RECEIVE_HELPERSETTING_DATA
{
public:
	HELPER_SETTING HelperSetting;
};

class CL2GS_RECEIVE_HELPERSETTING : public MU_CLIENT_TO_SERVER_HEADEREX, public CL2GS_RECEIVE_HELPERSETTING_DATA
{
public:
	CL2GS_RECEIVE_HELPERSETTING()
	{
		Code = GS2CL_PROT_HELPERSETTING;
	}

	virtual mu_uint32 Length() override
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(HelperSetting.Hunting);
		PARSER_WRITE(*static_cast<HELPER_OBTAININGSETTING*>(&HelperSetting.Obtaining));
		PARSER_WRITEVECTOR(HelperSetting.Obtaining.ExtraItems);

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(HelperSetting.Hunting);
		PARSER_WRITE(*static_cast<HELPER_OBTAININGSETTING*>(&HelperSetting.Obtaining));
		PARSER_WRITEVECTOR(HelperSetting.Obtaining.ExtraItems);

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_READ(HelperSetting.Hunting);
		PARSER_READ(*static_cast<HELPER_OBTAININGSETTING*>(&HelperSetting.Obtaining));
		PARSER_READVECTOR(HelperSetting.Obtaining.ExtraItems);

		return true;
	}
};

#endif