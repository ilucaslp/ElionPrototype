#ifndef __CLTOGS_QUEST_H__
#define __CLTOGS_QUEST_H__

#pragma once

#pragma pack(push, 1)
class ARM_PACKED CL2GS_REQUEST_QUESTANSWER_DATA
{
public:
	mu_uint8 QuestIndex;
	mu_uint8 State;
};
#pragma pack(pop)

class CL2GS_REQUEST_QUESTANSWER : public MU_CLIENT_TO_SERVER_HEADEREX, public CL2GS_REQUEST_QUESTANSWER_DATA
{
public:
	CL2GS_REQUEST_QUESTANSWER()
	{
		Code = GS2CL_PROT_QUESTANSWER;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_REQUEST_QUESTANSWER_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_REQUEST_QUESTANSWER_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<CL2GS_REQUEST_QUESTANSWER_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_RECEIVE_QUESTANSWER_DATA
{
public:
	mu_uint8 QuestIndex;
	mu_uint8 State;
	mu_uint8 Result;
};
#pragma pack(pop)

class GS2CL_RECEIVE_QUESTANSWER : public MU_SERVER_TO_CLIENT_HEADEREX, public GS2CL_RECEIVE_QUESTANSWER_DATA
{
public:
	GS2CL_RECEIVE_QUESTANSWER()
	{
		Code = GS2CL_PROT_QUESTANSWER;
	}

	virtual mu_uint32 Length()
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_QUESTANSWER_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_QUESTANSWER_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength)
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_RECEIVE_QUESTANSWER_DATA*>(this));

		return true;
	}
};

#endif