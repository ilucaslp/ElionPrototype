#include "stdafx.h"
#include "mu_crywolf1stdirection.h"

MUCrywolf1stDirection::MUCrywolf1stDirection()
{
	Init();
}

MUCrywolf1stDirection::~MUCrywolf1stDirection()
{

}

void MUCrywolf1stDirection::Init()
{
	m_bTimerCheck = true;
	m_iCryWolfState = 0;
}

void MUCrywolf1stDirection::GetCryWolfState(mu_uint8 CryWolfState)
{
	m_iCryWolfState = CryWolfState;
}

mu_boolean MUCrywolf1stDirection::IsCryWolfDirection()
{
	if (!g_Direction.m_bOrderExit && !m_bTimerCheck && m_iCryWolfState == CRYWOLF_STATE_NOTIFY_2)
		return true;

	return false;
}

void MUCrywolf1stDirection::IsCryWolfDirectionTimer()
{
	if (m_iCryWolfState == CRYWOLF_STATE_NOTIFY_2 && m_bTimerCheck && GetTimeCheck(5000))
	{
		m_bTimerCheck = false;
	}

	if (m_iCryWolfState == CRYWOLF_STATE_READY)
	{
		g_Direction.DeleteMonster();
		Init();
	}
}

void MUCrywolf1stDirection::CryWolfDirection(const mu_uint32 ThreadIndex)
{
	if (GET_WORLD != WD_34CRYWOLF_1ST)
		return;

	IsCryWolfDirectionTimer();

	if (!IsCryWolfDirection())
		return;

	switch (m_iCryWolfState)
	{
	case CRYWOLF_STATE_NOTIFY_2:
		g_Direction.CloseAllWindows();
		MoveBeginDirection(ThreadIndex);
		break;

	default:
		g_Direction.DeleteMonster();
		Init();
		break;
	}
}

void MUCrywolf1stDirection::MoveBeginDirection(const mu_uint32 ThreadIndex)
{
	if (g_Direction.DirectionCameraMove()) return;

	switch (g_Direction.m_iTimeSchedule)
	{
	case 0:		BeginDirection0(ThreadIndex);
	case 1:		BeginDirection1(ThreadIndex);
	case 2:		BeginDirection2(ThreadIndex);
	case 3:		BeginDirection3(ThreadIndex);
	case 4:		BeginDirection4(ThreadIndex);
	case 5:		BeginDirection5(ThreadIndex);
	}
}

void MUCrywolf1stDirection::BeginDirection0(const mu_uint32 ThreadIndex)
{
	g_Direction.SetNextDirectionPosition(113, 232, 0, 300.0f);
	g_Direction.m_iTimeSchedule--;
}

void MUCrywolf1stDirection::BeginDirection1(const mu_uint32 ThreadIndex)
{
	if (g_Direction.m_bAction)
	{
		if (g_Direction.m_iCheckTime == 0) g_Direction.SummonCreateMonster(ThreadIndex, 349, 114, 238, 0, true, true, 0.18f);
		else if (g_Direction.m_iCheckTime == 1) g_Direction.SummonCreateMonster(ThreadIndex, 340, 113, 238, 0, true, true, 0.227f);
		else if (g_Direction.m_iCheckTime == 2) g_Direction.SummonCreateMonster(ThreadIndex, 340, 115, 238, 0, true, true, 0.227f);
		else if (g_Direction.m_iCheckTime == 3) g_Direction.GetTimeCheck(1000);
		else if (g_Direction.m_iCheckTime == 4)
		{
			mu_boolean bSuccess[3];
			bSuccess[0] = g_Direction.MoveCreatedMonster(0, 114, 234, 0, 9);
			bSuccess[1] = g_Direction.MoveCreatedMonster(1, 112, 232, 0, 12);
			bSuccess[2] = g_Direction.MoveCreatedMonster(2, 116, 232, 0, 12);

			if (bSuccess[0] && bSuccess[1] && bSuccess[2]) ++g_Direction.m_iCheckTime;
		}
		else if (g_Direction.m_iCheckTime == 5) g_Direction.GetTimeCheck(1000);
		else if (g_Direction.m_iCheckTime == 6)
		{
			if (g_Direction.ActionCreatedMonster(ThreadIndex, 1, MONSTER01_ATTACK3, 1))
				g_Direction.SummonCreateMonster(ThreadIndex, 345, 109, 229, 0);
		}
		else if (g_Direction.m_iCheckTime == 7) g_Direction.GetTimeCheck(660);
		else if (g_Direction.m_iCheckTime == 8) g_Direction.SummonCreateMonster(ThreadIndex, 345, 110, 230, 0);
		else if (g_Direction.m_iCheckTime == 9) g_Direction.GetTimeCheck(660);
		else if (g_Direction.m_iCheckTime == 10) g_Direction.SummonCreateMonster(ThreadIndex, 345, 112, 230, 0);
		else if (g_Direction.m_iCheckTime == 11) g_Direction.GetTimeCheck(660);
		else if (g_Direction.m_iCheckTime == 12) g_Direction.SummonCreateMonster(ThreadIndex, 345, 113, 229, 0);
		else if (g_Direction.m_iCheckTime == 13) g_Direction.GetTimeCheck(1000);
		else if (g_Direction.m_iCheckTime == 14)
		{
			if (g_Direction.ActionCreatedMonster(ThreadIndex, 2, MONSTER01_ATTACK3, 1))
				g_Direction.SummonCreateMonster(ThreadIndex, 344, 114, 229, 0);
		}
		else if (g_Direction.m_iCheckTime == 15) g_Direction.GetTimeCheck(660);
		else if (g_Direction.m_iCheckTime == 16)	g_Direction.SummonCreateMonster(ThreadIndex, 344, 115, 230, 0);
		else if (g_Direction.m_iCheckTime == 17) g_Direction.GetTimeCheck(660);
		else if (g_Direction.m_iCheckTime == 18)	g_Direction.SummonCreateMonster(ThreadIndex, 344, 117, 230, 0);
		else if (g_Direction.m_iCheckTime == 19) g_Direction.GetTimeCheck(660);
		else if (g_Direction.m_iCheckTime == 20)	g_Direction.SummonCreateMonster(ThreadIndex, 344, 118, 229, 0);
		else if (g_Direction.m_iCheckTime == 21) g_Direction.GetTimeCheck(1000);
		else if (g_Direction.m_iCheckTime == 22)
		{
			g_Direction.SummonCreateMonster(ThreadIndex, 341, 110, 227, 0);
			g_Direction.CameraLevelUp();
		}
		else if (g_Direction.m_iCheckTime == 23) g_Direction.GetTimeCheck(660);
		else if (g_Direction.m_iCheckTime == 24)
		{
			g_Direction.SummonCreateMonster(ThreadIndex, 341, 112, 227, 0);
			g_Direction.CameraLevelUp();
		}
		else if (g_Direction.m_iCheckTime == 25) g_Direction.GetTimeCheck(660);
		else if (g_Direction.m_iCheckTime == 26)
		{
			g_Direction.SummonCreateMonster(ThreadIndex, 341, 114, 227, 0);
			g_Direction.CameraLevelUp();
		}
		else if (g_Direction.m_iCheckTime == 27) g_Direction.GetTimeCheck(660);
		else if (g_Direction.m_iCheckTime == 28)
		{
			g_Direction.SummonCreateMonster(ThreadIndex, 341, 116, 227, 0);
			g_Direction.CameraLevelUp();
		}
		else if (g_Direction.m_iCheckTime == 29) g_Direction.GetTimeCheck(1000);
		else if (g_Direction.m_iCheckTime == 30)	g_Direction.ActionCreatedMonster(ThreadIndex, 0, MONSTER01_ATTACK3, 1, false, true);
		else if (g_Direction.m_iCheckTime == 31) g_Direction.GetTimeCheck(2000);
		else g_Direction.m_bAction = false;
	}
	else
	{
		g_Direction.SetNextDirectionPosition(114, 220, 0, 40.0f);
	}
}

void MUCrywolf1stDirection::BeginDirection2(const mu_uint32 ThreadIndex)
{
	if (g_Direction.m_bAction)
	{
		if (g_Direction.m_iCheckTime == 0)
		{
			g_Direction.SummonCreateMonster(ThreadIndex, 348, 110, 240, 0, false, false);
			g_Direction.SummonCreateMonster(ThreadIndex, 348, 114, 240, 0, false, false);
			g_Direction.SummonCreateMonster(ThreadIndex, 348, 118, 240, 0, false, false);
			g_Direction.SummonCreateMonster(ThreadIndex, 341, 110, 242, 0, false, false);
			g_Direction.SummonCreateMonster(ThreadIndex, 341, 112, 242, 0, false, false);
			g_Direction.SummonCreateMonster(ThreadIndex, 341, 114, 242, 0, false, false);
			g_Direction.SummonCreateMonster(ThreadIndex, 341, 116, 242, 0, false, false);
			g_Direction.SummonCreateMonster(ThreadIndex, 341, 118, 242, 0, false, false);
			g_Direction.SummonCreateMonster(ThreadIndex, 341, 110, 244, 0, false, false);
			g_Direction.SummonCreateMonster(ThreadIndex, 341, 112, 244, 0, false, false);
			g_Direction.SummonCreateMonster(ThreadIndex, 341, 114, 244, 0, false, false);
			g_Direction.SummonCreateMonster(ThreadIndex, 341, 116, 244, 0, false, false);
			g_Direction.SummonCreateMonster(ThreadIndex, 341, 118, 244, 0);
		}
		else if (g_Direction.m_iCheckTime == 1)
		{
			g_Direction.GetTimeCheck(1000);
		}
		else if (g_Direction.m_iCheckTime == 2)
		{
			mu_boolean bSuccess[28];
			mu_int32 iMovingMonCount = sizeof(bSuccess);

			bSuccess[0] = g_Direction.MoveCreatedMonster(0, 114, 222, 0, 9);
			bSuccess[1] = g_Direction.MoveCreatedMonster(1, 110, 222, 0, 12);
			bSuccess[2] = g_Direction.MoveCreatedMonster(2, 118, 222, 0, 12);
			bSuccess[3] = g_Direction.MoveCreatedMonster(3, 107, 219, 0, 12);
			bSuccess[4] = g_Direction.MoveCreatedMonster(4, 108, 220, 0, 12);
			bSuccess[5] = g_Direction.MoveCreatedMonster(5, 110, 220, 0, 12);
			bSuccess[6] = g_Direction.MoveCreatedMonster(6, 111, 219, 0, 12);
			bSuccess[7] = g_Direction.MoveCreatedMonster(7, 116, 219, 0, 12);
			bSuccess[8] = g_Direction.MoveCreatedMonster(8, 117, 220, 0, 12);
			bSuccess[9] = g_Direction.MoveCreatedMonster(9, 119, 220, 0, 12);
			bSuccess[10] = g_Direction.MoveCreatedMonster(10, 120, 219, 0, 12);
			bSuccess[11] = g_Direction.MoveCreatedMonster(11, 110, 217, 0, 12);
			bSuccess[12] = g_Direction.MoveCreatedMonster(12, 112, 217, 0, 12);
			bSuccess[13] = g_Direction.MoveCreatedMonster(13, 114, 217, 0, 12);
			bSuccess[14] = g_Direction.MoveCreatedMonster(14, 116, 217, 0, 12);
			bSuccess[15] = g_Direction.MoveCreatedMonster(15, 110, 227, 0, 12);
			bSuccess[16] = g_Direction.MoveCreatedMonster(16, 114, 227, 0, 12);
			bSuccess[17] = g_Direction.MoveCreatedMonster(17, 118, 227, 0, 12);
			bSuccess[18] = g_Direction.MoveCreatedMonster(18, 110, 229, 0, 12);
			bSuccess[19] = g_Direction.MoveCreatedMonster(19, 112, 229, 0, 12);
			bSuccess[20] = g_Direction.MoveCreatedMonster(20, 114, 229, 0, 12);
			bSuccess[21] = g_Direction.MoveCreatedMonster(21, 116, 229, 0, 12);
			bSuccess[22] = g_Direction.MoveCreatedMonster(22, 118, 229, 0, 12);
			bSuccess[23] = g_Direction.MoveCreatedMonster(23, 110, 231, 0, 12);
			bSuccess[24] = g_Direction.MoveCreatedMonster(24, 112, 231, 0, 12);
			bSuccess[25] = g_Direction.MoveCreatedMonster(25, 114, 231, 0, 12);
			bSuccess[26] = g_Direction.MoveCreatedMonster(26, 116, 231, 0, 12);
			bSuccess[27] = g_Direction.MoveCreatedMonster(27, 118, 231, 0, 12);

			for (mu_int32 i = 0; i < iMovingMonCount; ++i)
			{
				if (!bSuccess[i]) break;
				if (i == iMovingMonCount - 1) ++g_Direction.m_iCheckTime;
			}
		}
		else if (g_Direction.m_iCheckTime == 3) g_Direction.GetTimeCheck(1000);
		else if (g_Direction.m_iCheckTime == 4) g_Direction.ActionCreatedMonster(ThreadIndex, 0, MONSTER01_ATTACK3, 1, false, true);
		else if (g_Direction.m_iCheckTime == 5) g_Direction.GetTimeCheck(2700);
		else if (g_Direction.m_iCheckTime == 6) g_Direction.ActionCreatedMonster(ThreadIndex, 0, MONSTER01_ATTACK4, 1, false, true);
		else if (g_Direction.m_iCheckTime == 7) g_Direction.GetTimeCheck(3000);
		else g_Direction.m_bAction = false;
	}
	else
		g_Direction.SetNextDirectionPosition(114, 160, 0, 300.0f);
}

void MUCrywolf1stDirection::BeginDirection3(const mu_uint32 ThreadIndex)
{
	if (g_Direction.m_bAction)
	{
		if (g_Direction.m_iCheckTime == 0)
		{
			g_Direction.DeleteMonster();
			++g_Direction.m_iCheckTime;
		}
		else if (g_Direction.m_iCheckTime == 1)
		{
			g_Direction.SummonCreateMonster(ThreadIndex, 340, 110, 77, 0, false, false);
			g_Direction.SummonCreateMonster(ThreadIndex, 340, 125, 77, 0, false, false);

			g_Direction.SummonCreateMonster(ThreadIndex, 341, 90, 37, 0, false, false);
			g_Direction.SummonCreateMonster(ThreadIndex, 341, 108, 73, 0, false, false);
			g_Direction.SummonCreateMonster(ThreadIndex, 341, 109, 75, 0, false, false);
			g_Direction.SummonCreateMonster(ThreadIndex, 341, 110, 73, 0, false, false);
			g_Direction.SummonCreateMonster(ThreadIndex, 341, 111, 75, 0, false, false);
			g_Direction.SummonCreateMonster(ThreadIndex, 341, 112, 73, 0, false, false);
			g_Direction.SummonCreateMonster(ThreadIndex, 341, 123, 73, 0, false, false);
			g_Direction.SummonCreateMonster(ThreadIndex, 341, 124, 75, 0, false, false);
			g_Direction.SummonCreateMonster(ThreadIndex, 341, 125, 73, 0, false, false);
			g_Direction.SummonCreateMonster(ThreadIndex, 341, 126, 75, 0, false, false);
			g_Direction.SummonCreateMonster(ThreadIndex, 341, 127, 73, 0, false, false);
			g_Direction.SummonCreateMonster(ThreadIndex, 341, 176, 20, 0, false, false);

			g_Direction.SummonCreateMonster(ThreadIndex, 341, 117, 77, 0, false, false);
			g_Direction.SummonCreateMonster(ThreadIndex, 341, 119, 77, 0, false, false);
			g_Direction.SummonCreateMonster(ThreadIndex, 341, 121, 77, 0, true, false);

			g_Direction.SummonCreateMonster(ThreadIndex, 340, 119, 83, 0, false, false);

			g_Direction.SummonCreateMonster(ThreadIndex, 344, 118, 79, 0, false, false);
			g_Direction.SummonCreateMonster(ThreadIndex, 344, 120, 79, 0, false, false);

			g_Direction.SummonCreateMonster(ThreadIndex, 344, 119, 90, 0, false, false);

			g_Direction.SummonCreateMonster(ThreadIndex, 345, 116, 81, 0, false, false);
			g_Direction.SummonCreateMonster(ThreadIndex, 345, 119, 81, 0, false, false);
			g_Direction.SummonCreateMonster(ThreadIndex, 345, 122, 81, 0, false, false);

			g_Direction.SummonCreateMonster(ThreadIndex, 340, 119, 87, 0, false, false);

			g_Direction.SummonCreateMonster(ThreadIndex, 348, 116, 90, 0, false, false);
			g_Direction.SummonCreateMonster(ThreadIndex, 348, 122, 90, 0, true, false);
		}
		else g_Direction.m_bAction = false;
	}
	else
		g_Direction.SetNextDirectionPosition(121, 75, 0, 300.0f);
}

void MUCrywolf1stDirection::BeginDirection4(const mu_uint32 ThreadIndex)
{
	if (g_Direction.m_bAction)
	{
		if (g_Direction.m_iCheckTime == 0) g_Direction.GetTimeCheck(3000);
		else g_Direction.m_bAction = false;
	}
	else
		g_Direction.SetNextDirectionPosition(121, 87, 0, 40.0f);
}

void MUCrywolf1stDirection::BeginDirection5(const mu_uint32 ThreadIndex)
{
	if (g_Direction.m_bAction)
	{
		if (g_Direction.m_iCheckTime == 0) g_Direction.GetTimeCheck(2000);
		else if (g_Direction.m_iCheckTime == 1)
		{
			g_Direction.ActionCreatedMonster(ThreadIndex, 25, MONSTER01_ATTACK1, 1, true);
			g_Direction.ActionCreatedMonster(ThreadIndex, 26, MONSTER01_ATTACK1, 1, true, true);
		}
		else if (g_Direction.m_iCheckTime == 2) g_Direction.GetTimeCheck(4000);
		else g_Direction.m_bAction = false;
	}
	else
	{
		g_Direction.DeleteMonster();
		g_Direction.m_bOrderExit = true;
	}
}