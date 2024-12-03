#ifndef __CLTOGS_EVENT_H__
#define __CLTOGS_EVENT_H__

#pragma once

namespace EVENTTIMER_TYPE
{
	enum
	{
		ETT_NONE = 0,

		ETT_DEVILSQUARE_ENTER_START,
		ETT_DEVILSQUARE_ENTER_CLOSE,
		ETT_DEVILSQUARE_CLOSE,

		ETT_BLOODCASTLE_ENTER_CLOSE,
		ETT_BLOODCASTLE_INFILTRATION,
		ETT_BLOODCASTLE_CLOSE,
		ETT_BLOODCASTLE_END,

		ETT_CHAOSCASTLE_ENTER_START,
		ETT_CHAOSCASTLE_INFILTRATION,
		ETT_CHAOSCASTLE_CLOSE,
		ETT_CHAOSCASTLE_END,

		ETT_CURSEDTEMPLE_ENTER_CLOSE,
		ETT_CURSEDTEMPLE_GAME_START,
		ETT_CURSEDTEMPLE_ICEWALKER,
		ETT_CURSEDTEMPLE_CLOSE,

		ETT_END,
	};
};

namespace EVENT_TYPE
{
	enum
	{
		ERT_BLOODCASTLE,
		ERT_DEVILSQUARE,
		ERT_CHAOSCASTLE,
	};
}

namespace EVENT_ENTER
{
	enum
	{
		EE_KILLERRESTRICTED_BC,
		EE_TIMEPASSED_BC,
		EE_ITEMINVALID_BC,
		EE_TOOWEAK_BC,
		EE_TOOSTRONG_BC,
		EE_FULL_BC,
	};
}

#pragma pack(push, 1)
class ARM_PACKED GS2CL_RECEIVE_EVENTENTER_DATA
{
public:
	mu_uint16 ResultCode;
};
#pragma pack(pop)

class GS2CL_RECEIVE_EVENTENTER : public MU_SERVER_TO_CLIENT_HEADEREX, public GS2CL_RECEIVE_EVENTENTER_DATA
{
public:
	GS2CL_RECEIVE_EVENTENTER()
	{
		Code = GS2CL_PROT_EVENTENTER;
	}

	virtual mu_uint32 Length() override
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_EVENTENTER_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_EVENTENTER_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_RECEIVE_EVENTENTER_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_RECEIVE_EVENTTIMER_DATA
{
public:
	mu_uint8 EventType;
	mu_uint8 TimerType;
};
#pragma pack(pop)

class GS2CL_RECEIVE_EVENTTIMER : public MU_SERVER_TO_CLIENT_HEADEREX, public GS2CL_RECEIVE_EVENTTIMER_DATA
{
public:
	GS2CL_RECEIVE_EVENTTIMER()
	{
		Code = GS2CL_PROT_EVENTTIMER;
	}

	virtual mu_uint32 Length() override
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_EVENTTIMER_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_EVENTTIMER_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_RECEIVE_EVENTTIMER_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_RECEIVE_EVENTRANK_DATA
{
public:
	mu_uint8 EventType;
	mu_uint8 MyRank;
	mu_uint8 IsSucceed;
};
#pragma pack(pop)

class GS2CL_RECEIVE_EVENTRANK_DATAEX
{
public:
#pragma pack(push, 1)
	class RankData
	{
	public:
		RankData()
		{
			TotalScore = 0;
			BonusExp = 0;
			BonusZen = 0;
		}

	public:
		mu_int32 TotalScore;
		mu_int64 BonusExp;
		mu_int64 BonusZen;
	};
#pragma pack(pop)

	class RankDataEx : public RankData
	{
	public:
		RankDataEx() : RankData()
		{

		}

	public:
		mu_string Name;
	};
public:
	std::vector<RankDataEx> List;
};

class GS2CL_RECEIVE_EVENTRANK : public MU_SERVER_TO_CLIENT_HEADEREX, public GS2CL_RECEIVE_EVENTRANK_DATA, public GS2CL_RECEIVE_EVENTRANK_DATAEX
{
public:
	GS2CL_RECEIVE_EVENTRANK()
	{
		Code = GS2CL_PROT_EVENTRANK;
	}

	virtual mu_uint32 Length() override
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_EVENTRANK_DATA*>(this));

		for (auto it = List.begin(); it != List.end(); it++)
		{
			RankDataEx &rankData = *it;
			PARSER_WRITE(*static_cast<RankData*>(&rankData));
			PARSER_WRITESTRING(rankData.Name, CHARACTER_MAXIMUM);
		}

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_EVENTRANK_DATA*>(this));

		for (auto it = List.begin(); it != List.end(); it++)
		{
			RankDataEx &rankData = *it;
			PARSER_WRITE(*static_cast<RankData*>(&rankData));
			PARSER_WRITESTRING(rankData.Name, CHARACTER_MAXIMUM);
		}

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_RECEIVE_EVENTRANK_DATA*>(this));

		for (auto it = List.begin(); it != List.end(); it++)
		{
			RankDataEx &rankData = *it;
			PARSER_READ(*static_cast<RankData*>(&rankData));
			PARSER_READSTRING(rankData.Name, CHARACTER_MAXIMUM);
		}

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED GS2CL_RECEIVE_EVENTSTATE_DATA
{
public:
	mu_uint8 EventType;
	mu_uint8 PlayState;
	mu_uint16 RemainSec;
	mu_uint16 MaxKillMonster;
	mu_uint16 CurKillMonster;
	mu_int16 UserHaveWeapon;
	mu_uint8 WeaponNum;
};
#pragma pack(pop)

class GS2CL_RECEIVE_EVENTSTATE : public MU_SERVER_TO_CLIENT_HEADEREX, public GS2CL_RECEIVE_EVENTSTATE_DATA
{
public:
	GS2CL_RECEIVE_EVENTSTATE()
	{
		Code = GS2CL_PROT_EVENTSTATE;
	}

	virtual mu_uint32 Length() override
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_EVENTSTATE_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_WRITE(*static_cast<GS2CL_RECEIVE_EVENTSTATE_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_SERVER_TO_CLIENT_HEADER*>(this));
		PARSER_READ(*static_cast<GS2CL_RECEIVE_EVENTSTATE_DATA*>(this));

		return true;
	}
};

#pragma pack(push, 1)
class ARM_PACKED CL2GS_REQUEST_BLOODCASTLE_ENTER_DATA
{
public:
	mu_uint8 Level;
	mu_int32 ItemIndex;
};
#pragma pack(pop)

class CL2GS_REQUEST_BLOODCASTLE_ENTER : public MU_CLIENT_TO_SERVER_HEADEREX, public CL2GS_REQUEST_BLOODCASTLE_ENTER_DATA
{
public:
	CL2GS_REQUEST_BLOODCASTLE_ENTER()
	{
		Code = GS2CL_PROT_BLOODCASTLE;
	}

	virtual mu_uint32 Length() override
	{
		DECLARE_PARSERHELPER(nullptr, 0, true);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_REQUEST_BLOODCASTLE_ENTER_DATA*>(this));

		return ParseHelper::RoundByBlockSize(parser.ProcessedLength, CTS_ENCRYPTION_BLOCKSIZE);
	}

	virtual mu_boolean ToBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_WRITE(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_WRITE(*static_cast<CL2GS_REQUEST_BLOODCASTLE_ENTER_DATA*>(this));

		return true;
	}

	virtual mu_boolean FromBuffer(mu_uint8 *buffer, mu_uint32 bufferLength) override
	{
		DECLARE_PARSERHELPER(buffer, bufferLength, false);

		PARSER_READ(*static_cast<MU_CLIENT_TO_SERVER_HEADER*>(this));
		PARSER_READ(*static_cast<CL2GS_REQUEST_BLOODCASTLE_ENTER_DATA*>(this));

		return true;
	}
};

#endif