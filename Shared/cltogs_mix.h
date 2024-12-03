#ifndef __CLTOGS_MIX_H__
#define __CLTOGS_MIX_H__

#pragma once

namespace MIX
{
	enum MIX_RESULT
	{
		MIXR_FAILED = 0, // 0
		MIXR_DENIED, // 3, 5, 7, 8, 10, 32, others...
		MIXR_SUCCEED, // 1
		MIXR_SUCCEED2, // 100
		MIXR_BREAK,
		MIXR_BREAKPET,
		MIXR_GOLDERROR, // 2, 11
		MIXR_DSLEVELERROR, // 4
		MIXR_BSLEVELERROR, // 9
	};
};

#pragma pack(push, 1)
class ARM_PACKED CL2GS_REQUEST_MIX_DATA
{
public:
	mu_uint8 Type;
	mu_uint8 SubType;
};
#pragma pack(pop)

class CL2GS_REQUEST_MIX : public MU_CLIENT_TO_SERVER_HEADEREX, public CL2GS_REQUEST_MIX_DATA
{
public:
	CL2GS_REQUEST_MIX()
	{
		Code = GS2CL_PROT_MIX;
	}

	virtual mu_uint32 Length() override
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_REQUEST_MIX_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_REQUEST_MIX_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<CL2GS_REQUEST_MIX_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_RECEIVE_MIX_DATA
{
public:
	mu_uint8 Result;
};
#pragma pack(pop)

class GS2CL_RECEIVE_MIX : public MU_SERVER_TO_CLIENT_HEADEREX, public GS2CL_RECEIVE_MIX_DATA
{
public:
	GS2CL_RECEIVE_MIX()
	{
		Code = GS2CL_PROT_MIX;
	}

	virtual mu_uint32 Length() override
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_MIX_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_MIX_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_RECEIVE_MIX_DATA*>(this));

		return true;
	}
};

#endif