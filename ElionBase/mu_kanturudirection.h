#ifndef __MU_KANTURU_DIRECTION_H__
#define __MU_KANTURU_DIRECTION_H__

#pragma once

enum KANTURU_STATE_TYPE
{
	KANTURU_STATE_NONE				= 0,
	KANTURU_STATE_STANDBY			= 1,
	KANTURU_STATE_MAYA_BATTLE		= 2,
	KANTURU_STATE_NIGHTMARE_BATTLE	= 3,
	KANTURU_STATE_TOWER				= 4,
	KANTURU_STATE_END				= 5,

};

enum KANTURU_MAYA_DIRECTION_TYPE
{
	KANTURU_MAYA_DIRECTION_NONE				= 0,
	KANTURU_MAYA_DIRECTION_STANBY1			= 1,
	KANTURU_MAYA_DIRECTION_NOTIFY			= 2,
	KANTURU_MAYA_DIRECTION_MONSTER1			= 3,
	KANTURU_MAYA_DIRECTION_MAYA1			= 4,
	KANTURU_MAYA_DIRECTION_END_MAYA1		= 5,
	KANTURU_MAYA_DIRECTION_ENDCYCLE_MAYA1	= 6,
	KANTURU_MAYA_DIRECTION_STANBY2			= 7,
	KANTURU_MAYA_DIRECTION_MONSTER2			= 8,
	KANTURU_MAYA_DIRECTION_MAYA2			= 9,
	KANTURU_MAYA_DIRECTION_END_MAYA2		= 10,
	KANTURU_MAYA_DIRECTION_ENDCYCLE_MAYA2	= 11,
	KANTURU_MAYA_DIRECTION_STANBY3			= 12,
	KANTURU_MAYA_DIRECTION_MONSTER3			= 13,
	KANTURU_MAYA_DIRECTION_MAYA3			= 14,
	KANTURU_MAYA_DIRECTION_END_MAYA3		= 15,
	KANTURU_MAYA_DIRECTION_ENDCYCLE_MAYA3	= 16,
	KANTURU_MAYA_DIRECTION_END				= 17,
	KANTURU_MAYA_DIRECTION_ENDCYCLE			= 18,
};

enum KANTURU_NIGHTMARE_DIRECTION_TYPE
{
	KANTURU_NIGHTMARE_DIRECTION_NONE		= 0,
	KANTURU_NIGHTMARE_DIRECTION_IDLE		= 1,
	KANTURU_NIGHTMARE_DIRECTION_NIGHTMARE	= 2,
	KANTURU_NIGHTMARE_DIRECTION_BATTLE		= 3,
	KANTURU_NIGHTMARE_DIRECTION_END			= 4,
	KANTURU_NIGHTMARE_DIRECTION_ENDCYCLE	= 5,
};

enum KANTURU_TOWER_STATE_TYPE
{
	KANTURU_TOWER_NONE						= 0,
	KANTURU_TOWER_REVITALIXATION			= 1,
	KANTURU_TOWER_NOTIFY					= 2,
	KANTURU_TOWER_CLOSE						= 3,
	KANTURU_TOWER_NOTIFY2					= 4,
	KANTURU_TOWER_END						= 5,
	KANTURU_TOWER_ENDCYCLE					= 6,
};

class MUKanturuDirection
{
public:
	mu_int32		m_iKanturuState;
	mu_int32		m_iMayaState;
	mu_int32		m_iNightmareState;
	mu_boolean		m_bKanturuDirection;

	MUKanturuDirection();
	virtual ~MUKanturuDirection();

	void Init();
	mu_boolean IsKanturuDirection();
	mu_boolean IsKanturu3rdTimer();
	mu_boolean IsMayaScene();
	void GetKanturuAllState(mu_uint8 State, mu_uint8 DetailState);
	void KanturuAllDirection(const mu_uint32 ThreadIndex);
	mu_boolean GetMayaExplotion();
	void SetMayaExplotion(mu_boolean MayaDie);
	mu_boolean GetMayaAppear();
	void SetMayaAppear(mu_boolean MayaDie);

private:
	mu_boolean		m_bMayaDie;
	mu_boolean		m_bMayaAppear;
	mu_boolean		m_bDirectionEnd;

	void GetKanturuMayaState(mu_uint8 DetailState);
	void GetKanturuNightmareState(mu_uint8 DetailState);

	void KanturuMayaDirection();
	void Move1stDirection();
	void Direction1st0();
	void Direction1st1();
	void Move2ndDirection();
	void Direction2nd0();
	void Direction2nd1();
	void Direction2nd2();

	void KanturuNightmareDirection(const mu_uint32 ThreadIndex);
	void Move3rdDirection(const mu_uint32 ThreadIndex);
	void Direction3rd0(const mu_uint32 ThreadIndex);
	void Direction3rd1(const mu_uint32 ThreadIndex);
	void Move4thDirection(const mu_uint32 ThreadIndex);
	void Direction4th0(const mu_uint32 ThreadIndex);
	void Direction4th1(const mu_uint32 ThreadIndex);
};

#endif