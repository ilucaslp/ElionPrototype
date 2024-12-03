#ifndef __MU_CRYWOLF_1ST_DIRECTION_H__
#define __MU_CRYWOLF_1ST_DIRECTION_H__

#pragma once

enum CRYWOLF_OCCUPATION_STATE_TYPE
{
	CRYWOLF_OCCUPATION_STATE_PEACE = 0,
	CRYWOLF_OCCUPATION_STATE_OCCUPIED = 1,
	CRYWOLF_OCCUPATION_STATE_WAR = 2,
};

enum CRYWOLF_STATE_TYPE
{
	CRYWOLF_STATE_NONE = 0,
	CRYWOLF_STATE_NOTIFY_1 = 1,
	CRYWOLF_STATE_NOTIFY_2 = 2,
	CRYWOLF_STATE_READY = 3,
	CRYWOLF_STATE_START = 4,
	CRYWOLF_STATE_END = 5,
	CRYWOLF_STATE_ENDCYCLE = 6,
};

enum CRYWOLF_ALTAR_STATE
{
	CRYWOLF_ALTAR_STATE_NONE = 0,
	CRYWOLF_ALTAR_STATE_CONTRACTED = 1,
	CRYWOLF_ALTAR_STATE_APPLYING_CONTRACT = 2,
	CRYWOLF_ALTAR_STATE_EXCEEDING_CONTRACT_COUNT = 3,
	CRYWOLF_ALTAR_STATE_OCCUPYING = 4,
	CRYWOLF_ALTAR_STATE_OCCUPIED = 5,
};

class MUCrywolf1stDirection
{
private:
	mu_boolean m_bTimerCheck;

	void IsCryWolfDirectionTimer();

	void MoveBeginDirection(const mu_uint32 ThreadIndex);
	void BeginDirection0(const mu_uint32 ThreadIndex);
	void BeginDirection1(const mu_uint32 ThreadIndex);
	void BeginDirection2(const mu_uint32 ThreadIndex);
	void BeginDirection3(const mu_uint32 ThreadIndex);
	void BeginDirection4(const mu_uint32 ThreadIndex);
	void BeginDirection5(const mu_uint32 ThreadIndex);
public:
	mu_int32 m_iCryWolfState;

	MUCrywolf1stDirection();
	virtual ~MUCrywolf1stDirection();

	void Init();
	void GetCryWolfState(mu_uint8 CryWolfState);
	mu_boolean IsCryWolfDirection();
	void CryWolfDirection(const mu_uint32 ThreadIndex);
};

#endif