#ifndef __MU_DIRECTION_H__
#define __MU_DIRECTION_H__

#pragma once

#include "mu_crywolf1stdirection.h"
#include "mu_kanturudirection.h"

struct DirectionMonster
{
	mu_int32		m_Index;
	mu_int32		m_iActionCheck;
	mu_boolean		m_bAngleCheck;
};

class MUDirection
{
private:
	EVector3	m_vCameraPosition;
	EVector3	m_v1stPosition;
	EVector3	m_v2ndPosition;
	EVector3	m_vResult;

	mu_boolean	m_bCameraCheck;
	mu_float	m_fCount;
	mu_float	m_fLength;
	mu_float	m_fCameraSpeed;

	mu_boolean	m_bTimeCheck;
	mu_int32		m_iBackupTime;
	mu_boolean	m_bStateCheck;

public:
	std::vector<DirectionMonster> stl_Monster;

	mu_boolean	m_bOrderExit;
	mu_int32	m_iTimeSchedule;
	mu_int32	m_CameraLevel;
	mu_float	m_fCameraViewFar;
	mu_uint64	m_iCheckTime;
	mu_boolean	m_bAction;
	mu_boolean	m_bDownHero;
	mu_float	m_AngleY;

	MUKanturuDirection m_CKanturu;
	MUCrywolf1stDirection m_CMVP;

	MUDirection();
	virtual ~MUDirection();

	void Init();
	void CloseAllWindows();
	mu_boolean IsDirection();
	void CheckDirection(const mu_uint32 ThreadIndex);
	void SetCameraPosition();
	mu_int32 GetCameraPosition(EVector3 GetPosition);
	mu_boolean DirectionCameraMove();
	void DeleteMonster();


	mu_float CalculateAngle(CHARACTER* c, mu_int32 x, mu_int32 y, mu_float Angle);
	void CameraLevelUp();
	void SetNextDirectionPosition(mu_int32 x, mu_int32 y, mu_int32 z, mu_float Speed);
	mu_boolean GetTimeCheck(mu_uint64 DelayTime);
	void HeroFallingDownDirection(const mu_uint32 ThreadIndex);
	void HeroFallingDownInit();
	void FaillingEffect(const mu_uint32 ThreadIndex);

	void SummonCreateMonster(const mu_uint32 ThreadIndex, mu_int32 Type, mu_int32 x, mu_int32 y, mu_float Angle, mu_boolean NextCheck = true, mu_boolean SummonAni = true, mu_float AniSpeed = -1.0f);
	mu_boolean MoveCreatedMonster(mu_int32 Type, mu_int32 x, mu_int32 y, mu_float Angle, mu_int32 Speed);
	mu_boolean ActionCreatedMonster(const mu_uint32 ThreadIndex, mu_int32 Type, mu_int32 Action, mu_int32 Count, mu_boolean TankerAttack = false, mu_boolean NextCheck = false);
};

extern MUDirection g_Direction;

#endif