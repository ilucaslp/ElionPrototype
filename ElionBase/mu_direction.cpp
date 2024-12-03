#include "stdafx.h"
#include "mu_direction.h"

MUDirection g_Direction;

MUDirection::MUDirection()
{
	Init();
}

MUDirection::~MUDirection()
{

}

void MUDirection::Init()
{
	Vector(0.0f, 0.0f, 0.0f, m_vCameraPosition);
	Vector(0.0f, 0.0f, 0.0f, m_v1stPosition);
	Vector(0.0f, 0.0f, 0.0f, m_v2ndPosition);
	Vector(0.0f, 0.0f, 0.0f, m_vResult);

	m_bStateCheck = true;
	m_bCameraCheck = false;
	m_bAction = true;
	m_bTimeCheck = true;
	m_bOrderExit = false;

	m_bDownHero = false;

	m_fCount = 0.0f;
	m_fLength = 0.0f;
	m_fCameraSpeed = 100.0f;
	m_fCameraViewFar = 1000.0f;

	m_iCheckTime = 0;
	m_iTimeSchedule = 0;
	m_iBackupTime = 0;
	m_CameraLevel = 0;

	m_AngleY = 0.0f;

	m_CMVP.Init();
	m_CKanturu.Init();
}

void MUDirection::CloseAllWindows()
{
	g_pPickedItem.BackupPickedItem();
	g_pNewUISystem->HideAll();
}

mu_boolean MUDirection::IsDirection()
{
	if (GET_WORLD == WD_34CRYWOLF_1ST)
		return m_CMVP.IsCryWolfDirection();
	else if (GET_WORLD == WD_39KANTURU_3RD)
		return m_CKanturu.IsKanturuDirection();

	return false;
}

void MUDirection::CheckDirection(const mu_uint32 ThreadIndex)
{
	m_CMVP.CryWolfDirection(ThreadIndex);
	m_CKanturu.KanturuAllDirection(ThreadIndex);
}

void MUDirection::SetCameraPosition()
{
	if (m_bStateCheck)
	{
		if (m_iTimeSchedule == 0)
		{
			VectorCopy(Hero->Object.Position, m_v1stPosition);
			++m_iTimeSchedule;
		}

		VectorSubtract(m_v2ndPosition, m_v1stPosition, m_vResult);
		m_fLength = VectorLength(m_vResult);
	}
}

mu_int32 MUDirection::GetCameraPosition(EVector3 GetPosition)
{
	VectorCopy(m_vCameraPosition, GetPosition);
	return m_CameraLevel;
}

mu_boolean MUDirection::DirectionCameraMove()
{
	if (m_bCameraCheck)
	{
		SetCameraPosition();
		m_bStateCheck = false;

		VectorNormalize(m_vResult);
		EVector3 vTemp;

		VectorScale(m_vResult, m_fCount, vTemp);
		VectorAdd(m_v1stPosition, vTemp, m_vCameraPosition);
		m_fCount += m_fCameraSpeed;

		if (m_fLength <= VectorLength(vTemp))
		{
			VectorCopy(m_v2ndPosition, m_vCameraPosition);
			m_bAction = true;
			m_bCameraCheck = false;
			m_fCount = 0.0f;
			VectorCopy(m_v2ndPosition, m_v1stPosition);
		}
		return true;
	}

	return false;
}

void MUDirection::DeleteMonster()
{
	mu_int32 Count = stl_Monster.size();

	if (Count == 0)
		return;

	for (mu_int32 i = 0; i < Count; ++i)
	{
		DeleteCharacter(i + NUMOFMON);
		stl_Monster.erase(stl_Monster.begin());
	}
}

mu_float MUDirection::CalculateAngle(CHARACTER* c, mu_int32 x, mu_int32 y, mu_float Angle)
{
	EVector3 vTemp, vTemp2, vResult;
	mu_float fx = (mu_float)(x*g_TerrainScale) + 0.5f*g_TerrainScale;
	mu_float fy = (mu_float)(y*g_TerrainScale) + 0.5f*g_TerrainScale;

	Vector(fx, fy, 0.0f, vTemp);
	Vector(c->Object.Position[0], c->Object.Position[1], 0.0f, vTemp2);

	VectorSubtract(vTemp2, vTemp, vResult);
	Vector(0.0f, 1.0f, 0.0f, vTemp2);

	VectorNormalize(vResult);

	mu_float fAngle = EMath::Acos(DotProduct(vResult, vTemp2)) / Q_RAD;

	if (vResult[0] > 0) fAngle = 360 - fAngle;

	return fAngle;
}

void MUDirection::SummonCreateMonster(const mu_uint32 ThreadIndex, mu_int32 Type, mu_int32 x, mu_int32 y, mu_float Angle, mu_boolean NextCheck, mu_boolean SummonAni, mu_float AniSpeed)
{
	CHARACTER* c = nullptr;
	DirectionMonster DMonster = { 0, };

	DMonster.m_Index = stl_Monster.size();
	DMonster.m_bAngleCheck = true;
	DMonster.m_iActionCheck = 0;

	stl_Monster.push_back(DMonster);

	c = CreateMonster(ThreadIndex, Type, x, y, DMonster.m_Index + NUMOFMON);
	c->Key = NUMOFMON + DMonster.m_Index++;
	CharactersKeys[c->Key] = c->Index;
	c->Object.Angle[2] = Angle;
	c->Weapon[0].Type = -1;
	c->Weapon[1].Type = -1;
	c->Object.Alpha = 0.0f;

	mu_int32 Index = 0;

	switch (Type)
	{
	case 344: Index = MODEL_MONSTER01 + 91;
	case 341: Index = MODEL_MONSTER01 + 94;
	case 440:
	case 340: Index = MODEL_MONSTER01 + 92;
	case 345: Index = MODEL_MONSTER01 + 93;
	case 348: Index = MODEL_MONSTER01 + 99;
	case 349: Index = MODEL_MONSTER01 + 89;
	case 361: Index = MODEL_MONSTER01 + 121;
	}

	if (AniSpeed >= 0.0f)
	{
		c->Object.Flags.Set(OBJECT::eOF_DIRECTIONANIMATION);
		c->Object.DirectionSpeed = AniSpeed;
	}

	if (SummonAni)
	{
		if (Type == 361)
		{
			EVector3 Light, Angle;

			Vector(0.5f, 0.8f, 1.0f, Light);
			Vector(0.0f, 0.0f, 0.0f, Angle);
			CreateEffect(ThreadIndex, MODEL_STORM2, c->Object.Position, Angle, Light, 1, nullptr, -1, 0, 0, 0, 1.6f);
			CreateEffect(ThreadIndex, MODEL_STORM2, c->Object.Position, Angle, Light, 1, nullptr, -1, 0, 0, 0, 1.3f);
			CreateEffect(ThreadIndex, MODEL_STORM2, c->Object.Position, Angle, Light, 1, nullptr, -1, 0, 0, 0, 0.7f);

			PlayBackground(SOUND_KANTURU_3RD_MAYA_END);
		}
		else
		{
			PlayBackground(SOUND_CRY1ST_SUMMON);

			EVector3 vPos;
			Vector(c->Object.Position[0] + 20.0f, c->Object.Position[1] + 20.0f, c->Object.Position[2], vPos);

			CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER + 1, vPos, vPos, c->Object.Angle, 7, nullptr, 60.0f + GetLargeRand(10));
			CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER + 1, vPos, vPos, c->Object.Angle, 7, nullptr, 50.0f + GetLargeRand(10));
			CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER + 1, vPos, vPos, c->Object.Angle, 7, nullptr, 50.0f + GetLargeRand(10));
			CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER + 1, vPos, vPos, c->Object.Angle, 7, nullptr, 60.0f + GetLargeRand(10));

			CreateParticle(ThreadIndex, BITMAP_SMOKE + 4, c->Object.Position, c->Object.Angle, c->Object.Light, 1, 5.0f);
			CreateParticle(ThreadIndex, BITMAP_SMOKE + 4, c->Object.Position, c->Object.Angle, c->Object.Light, 1, 5.0f);
			CreateParticle(ThreadIndex, BITMAP_SMOKE + 4, c->Object.Position, c->Object.Angle, c->Object.Light, 1, 5.0f);

			Vector(c->Object.Position[0], c->Object.Position[1], c->Object.Position[2] + 120.0f, vPos);
			CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, c->Object.Position, vPos, c->Object.Angle, 17);
			CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, c->Object.Position, vPos, c->Object.Angle, 17);
			CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, c->Object.Position, vPos, c->Object.Angle, 17);
			CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, c->Object.Position, vPos, c->Object.Angle, 17);
		}

	}

	if (NextCheck) ++m_iCheckTime;
}

mu_boolean MUDirection::MoveCreatedMonster(mu_int32 Index, mu_int32 x, mu_int32 y, mu_float Angle, mu_int32 Speed)
{
	CHARACTER *c = nullptr;
	mu_boolean bNext = false;

	mu_int16 tmpIndex = CharactersKeys[Index + NUMOFMON];

	if (tmpIndex == -1)
	{
		return false;
	}

	c = &CharactersClient[tmpIndex];

	if (c->Object.Flags.Get(OBJECT::eOF_LIVE) == false)
	{
		return false;
	}

	mu_int32 PresentX = (mu_int32)(c->Object.Position[0]) * g_TerrainScaleRev;
	mu_int32 PresentY = (mu_int32)(c->Object.Position[1]) * g_TerrainScaleRev;

	if (PresentX == x && PresentY == y) bNext = true;

	if (!bNext)
	{
		mu_int32 iResult = 0;

		if (stl_Monster[Index].m_bAngleCheck)
		{
			mu_int32	iAngle1 = (mu_int32)CalculateAngle(c, x, y, Angle);
			mu_int32 iAngle2 = (mu_int32)c->Object.Angle[2];

			if ((iAngle1 - Angle) > 180)
				iAngle1 = iAngle1 - 360;

			iResult = iAngle1 - iAngle2;
			c->Blood = false;
		}

		if (iResult <= 3 && iResult >= -3)
		{
			c->Blood = true;
			stl_Monster[Index].m_bAngleCheck = false;
		}

		if (c->Blood)
		{
			c->MoveSpeed = Speed;
			SetAction(&c->Object, MONSTER01_WALK);
			MoveCharacterPosition(c);
		}
		else
		{
			if (iResult > 3 && iResult <= 180)
				c->Object.Angle[2] += 3.0f;
			else
				c->Object.Angle[2] -= 3.0f;

			SetAction(&c->Object, MONSTER01_STOP1);
		}
	}
	else
	{
		c->Object.Position[0] = (mu_float)(x*g_TerrainScale) + 0.5f*g_TerrainScale;
		c->Object.Position[1] = (mu_float)(y*g_TerrainScale) + 0.5f*g_TerrainScale;

		stl_Monster[Index].m_bAngleCheck = true;
		SetAction(&c->Object, MONSTER01_STOP1);
		return true;
	}

	return false;
}

mu_boolean MUDirection::ActionCreatedMonster(const mu_uint32 ThreadIndex, mu_int32 Index, mu_int32 Action, mu_int32 Count, mu_boolean TankerAttack, mu_boolean NextCheck)
{
	CHARACTER *c = nullptr;
	mu_boolean bNext = false;

	mu_int16 tmpIndex = CharactersKeys[Index + NUMOFMON];

	if (tmpIndex == -1)
	{
		return false;
	}

	c = &CharactersClient[tmpIndex];

	if (c->Object.Flags.Get(OBJECT::eOF_LIVE) == false)
	{
		return false;
	}

	if (stl_Monster[Index].m_iActionCheck == Count) bNext = true;

	if (!bNext)
	{
		if (c->Object.CurrentAction != Action)
		{
			c->Object.CurrentAction = Action;
			c->Object.AnimationFrame = 0.0f;
			stl_Monster[Index].m_iActionCheck++;

			if (TankerAttack)
				CreateEffect(ThreadIndex, MODEL_ARROW_TANKER, c->Object.Position, c->Object.Angle, c->Object.Light, 1, &c->Object, c->Object.PKKey);
		}
	}
	else
	{
		if (NextCheck)
		{
			stl_Monster[Index].m_iActionCheck = 0;
			++m_iCheckTime;
		}

		if (c->Object.AnimationFrame >= 8.0f) return true;
	}

	return false;
}

void MUDirection::HeroFallingDownDirection(const mu_uint32 ThreadIndex)
{
	if (!m_bDownHero)
		return;

	Hero->Object.Gravity += 1.5f;
	Hero->Object.Angle[0] -= 2.0f;
	Hero->Object.Flags.Set(OBJECT::eOF_ACTIONSTART);
	Hero->Object.Direction[1] += Hero->Object.Direction[0];

	if (Hero->Object.Gravity <= 2.0f)
		m_AngleY = Hero->Object.Angle[2];

	if (Hero->Object.Angle[0] <= -90.0f)
		Hero->Object.Angle[0] = -90.0f;

	Hero->Object.Angle[2] = m_AngleY;

	if (Hero->Object.Gravity >= 150.0f)
		Hero->Object.Gravity = 150.0f;

	Hero->Object.Position[2] -= Hero->Object.Gravity;

	FaillingEffect(ThreadIndex);
}

void MUDirection::FaillingEffect(const mu_uint32 ThreadIndex)
{
	EVector3 Pos, Light;
	mu_float Scale = 1.3f + GetLargeRand(10) / 30.0f;
	Vector(0.05f, 0.05f, 0.08f, Light);

	Pos[0] = Hero->Object.Position[0] + (mu_float)(GetLargeRand(20) - 10)*70.0f;
	Pos[1] = Hero->Object.Position[1] + (mu_float)(GetLargeRand(20) - 10)*70.0f;
	Pos[2] = Hero->Object.Position[2] - GetLargeRand(200) - 500.0f;

	CreateParticle(ThreadIndex, BITMAP_CLOUD, Pos, Hero->Object.Angle, Light, 13, Scale);

	Pos[0] = Hero->Object.Position[0] + (mu_float)(GetLargeRand(20) - 10)*70.0f;
	Pos[1] = Hero->Object.Position[1] + (mu_float)(GetLargeRand(20) - 10)*70.0f;
	Pos[2] = Hero->Object.Position[2] - GetLargeRand(200) - 500.0f;

	Vector(0.05f, 0.05f, 0.05f, Light);
	CreateParticle(ThreadIndex, BITMAP_CLOUD, Pos, Hero->Object.Angle, Light, 13, Scale);
}

void MUDirection::HeroFallingDownInit()
{
	if (!m_bDownHero)
	{
		Hero->Object.Flags.Clear(OBJECT::eOF_ACTIONSTART);
		Hero->Object.Gravity = 0.0f;
		Hero->Object.Angle[0] = 0.0f;
	}
}

void MUDirection::CameraLevelUp()
{
	if (m_CameraLevel < 4) ++m_CameraLevel;
}

void MUDirection::SetNextDirectionPosition(mu_int32 x, mu_int32 y, mu_int32 z, mu_float Speed)
{
	m_iCheckTime = 0;
	m_bCameraCheck = true;
	m_bStateCheck = true;
	m_fCameraSpeed = Speed;

	mu_float fx = (mu_float)(x*g_TerrainScale) + 0.5f*g_TerrainScale;
	mu_float fy = (mu_float)(y*g_TerrainScale) + 0.5f*g_TerrainScale;
	mu_float fz = (mu_float)(z*g_TerrainScale) + 0.5f*g_TerrainScale;

	Vector(fx, fy, fz, m_v2ndPosition);
	++m_iTimeSchedule;
}

mu_boolean MUDirection::GetTimeCheck(mu_uint64 DelayTime)
{
	mu_uint32 PresentTime = g_CurrentTime;

	if (m_bTimeCheck)
	{
		m_iBackupTime = PresentTime;
		m_bTimeCheck = false;
	}

	if (m_iBackupTime + DelayTime <= PresentTime)
	{
		m_bTimeCheck = true;
		++m_iCheckTime;
		return true;
	}
	return false;
}