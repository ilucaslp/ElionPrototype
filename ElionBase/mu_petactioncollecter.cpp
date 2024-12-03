#include "stdafx.h"
#include "mu_petactioncollecter.h"

MUPetActionCollecter g_PetActionCollecter;

MUPetActionCollecter::MUPetActionCollecter()
{
	m_isRooting = false;

	m_dwSendDelayTime = 0;
	m_dwRootingTime = 0;
	m_state = eAction_Stand;

	m_fRadWidthStand = 0.0f;
	m_fRadWidthGet = 0.0f;
}

MUPetActionCollecter::~MUPetActionCollecter()
{

}

mu_boolean MUPetActionCollecter::Release(OBJECT* obj, CHARACTER *Owner)
{
	DeleteEffect(MODEL_NEWYEARSDAY_EVENT_MONEY, obj);

	return true;
}

mu_boolean MUPetActionCollecter::Move(const mu_uint32 ThreadIndex, OBJECT* obj, CHARACTER *Owner, mu_int32 targetKey, mu_uint64 tick, mu_boolean bForceRender)
{
	if (nullptr == obj || nullptr == Owner) return false;

	FindZen(obj);

	if (eAction_Stand == m_state && m_isRooting)
	{
		m_state = eAction_Move;
	}

	mu_float FlyRange = 10.0f;
	EVector3 targetPos, Range, Direction;
	m_fRadWidthStand = ((2.0f * Q_PI) / 4000.0f) * (mu_float)(tick % 4000);
	m_fRadWidthGet = ((2.0f * Q_PI) / 2000.0f) * (mu_float)(tick % 2000);

	obj->Position[2] = obj->Owner->Position[2] + 20.0f;

	VectorSubtract(obj->Position, obj->Owner->Position, Range);

	mu_float Distance = EMath::Sqrt(Range[0] * Range[0] + Range[1] * Range[1]);
	if (Distance > SEARCH_LENGTH * 3)
	{
		obj->Position[0] = obj->Owner->Position[0] + (EMath::Sin(m_fRadWidthStand) * CIRCLE_STAND_RADIAN);
		obj->Position[1] = obj->Owner->Position[1] + (EMath::Cos(m_fRadWidthStand) * CIRCLE_STAND_RADIAN);

		VectorCopy(obj->Owner->Angle, obj->Angle);

		m_state = eAction_Stand;
		m_isRooting = false;
	}

	switch (m_state)
	{
	default: break;

	case eAction_Stand:
		{
			targetPos[0] = obj->Owner->Position[0] + (EMath::Sin(m_fRadWidthStand) * CIRCLE_STAND_RADIAN);
			targetPos[1] = obj->Owner->Position[1] + (EMath::Cos(m_fRadWidthStand) * CIRCLE_STAND_RADIAN);
			targetPos[2] = obj->Owner->Position[2];

			VectorSubtract(targetPos, obj->Position, Range);

			Distance = EMath::Sqrt(Range[0] * Range[0] + Range[1] * Range[1]);

			if (80.0f >= FlyRange)
			{
				mu_float Angle = CreateAngle(obj->Position[0], obj->Position[1], targetPos[0], targetPos[1]);
				obj->Angle[2] = TurnAngle2(obj->Angle[2], Angle, 8.0f);
			}

			EMatrix4 matrix;
			AngleMatrix(obj->Angle, matrix);
			VectorRotate(obj->Direction, matrix, Direction);
			VectorAdd(obj->Position, Direction, obj->Position);

			mu_float Speed = (FlyRange >= Distance) ? 0 : (mu_float)EMath::Log(Distance) * 2.3f;

			obj->Direction[0] = 0.0f;
			obj->Direction[1] = -Speed;
			obj->Direction[2] = 0.0f;
		}
		break;

	case eAction_Move:
		{
			if (!m_isRooting)
			{
				m_isRooting = false;
				m_state = eAction_Return;
				break;
			}

			targetPos[0] = m_RootItem.position[0] + (EMath::Sin(m_fRadWidthGet) * CIRCLE_STAND_RADIAN);
			targetPos[1] = m_RootItem.position[1] + (EMath::Cos(m_fRadWidthGet) * CIRCLE_STAND_RADIAN);
			targetPos[2] = m_RootItem.position[2];

			VectorSubtract(targetPos, obj->Position, Range);

			Distance = EMath::Sqrt(Range[0] * Range[0] + Range[1] * Range[1]);
			if (Distance >= FlyRange)
			{
				mu_float Angle = CreateAngle(obj->Position[0], obj->Position[1], targetPos[0], targetPos[1]);
				obj->Angle[2] = TurnAngle2(obj->Angle[2], Angle, 20.0f);
			}

			EMatrix4 matrix;
			AngleMatrix(obj->Angle, matrix);
			VectorRotate(obj->Direction, matrix, Direction);
			VectorAdd(obj->Position, Direction, obj->Position);

			mu_float Speed = (20.0f >= Distance) ? 0 : (mu_float)EMath::Log(Distance) * 2.5f;

			obj->Direction[0] = 0.0f;
			obj->Direction[1] = -Speed;
			obj->Direction[2] = 0.0f;

			if (0 == Speed || CompTimeControl(100000, m_dwRootingTime))
			{
				m_dwSendDelayTime = g_CurrentTime;
				m_dwRootingTime = g_CurrentTime;
				m_state = eAction_Get;
			}
		}
		break;

	case eAction_Get:
		{
			if (!m_isRooting || SEARCH_LENGTH < Distance || CompTimeControl(3000, m_dwRootingTime))
			{
				m_isRooting = false;
				m_dwRootingTime = g_CurrentTime;
				m_state = eAction_Return;
				break;
			}

			VectorCopy(m_RootItem.position, targetPos);

			mu_float Angle = CreateAngle(obj->Position[0], obj->Position[1], targetPos[0], targetPos[1]);
			obj->Angle[2] = TurnAngle2(obj->Angle[2], Angle, 10.0f);

			if (CompTimeControl(1000, m_dwSendDelayTime))
			{
#if 0 // FIX
				if (&Hero->Object == obj->Owner)
					SendRequestGetItem(m_RootItem.itemIndex);
#endif
			}
		}
		break;

	case eAction_Return:
		{
			targetPos[0] = obj->Owner->Position[0] + (EMath::Sin(m_fRadWidthStand) * CIRCLE_STAND_RADIAN);
			targetPos[1] = obj->Owner->Position[1] + (EMath::Cos(m_fRadWidthStand) * CIRCLE_STAND_RADIAN);
			targetPos[2] = obj->Owner->Position[2];

			VectorSubtract(targetPos, obj->Position, Range);

			Distance = EMath::Sqrt(Range[0] * Range[0] + Range[1] * Range[1]);
			if (Distance >= FlyRange)
			{
				mu_float Angle = CreateAngle(obj->Position[0], obj->Position[1], targetPos[0], targetPos[1]);
				obj->Angle[2] = TurnAngle2(obj->Angle[2], Angle, 20.0f);
			}

			EMatrix4 matrix;
			AngleMatrix(obj->Angle, matrix);
			VectorRotate(obj->Direction, matrix, Direction);
			VectorAdd(obj->Position, Direction, obj->Position);

			mu_float Speed = (FlyRange >= Distance) ? 0 : (mu_float)EMath::Log(Distance) * 2.5f;

			obj->Direction[0] = 0.0f;
			obj->Direction[1] = -Speed;
			obj->Direction[2] = 0.0f;

			if (0 == Speed || CompTimeControl(3000, m_dwRootingTime))
			{
				m_state = eAction_Stand;
			}
		}
		break;
	}

	return true;
}

mu_boolean MUPetActionCollecter::Effect(const mu_uint32 ThreadIndex, OBJECT* obj, CHARACTER *Owner, mu_int32 targetKey, mu_uint64 tick, mu_boolean bForceRender)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	if (nullptr == obj || nullptr == Owner) return false;

	MUModel* b = &MODELS::Data[obj->Type];
	EVector3 Position, vRelativePos, Light;

	VectorCopy(obj->Position, b->BodyOrigin(ThreadIndex));
	Vector(0.0f, 0.0f, 0.0f, vRelativePos);

	b->Animation(ThreadIndex, ThreadInfo.BoneTransform, obj->AnimationFrame, obj->PriorAnimationFrame, obj->PriorAction, obj->Angle, obj->HeadAngle);

	mu_float fRad1 = ((Q_PI / 3000.0f) * (mu_float)(tick % 3000));
	mu_float fSize = EMath::Sin(fRad1) * 0.2f;
	mu_float fSize2 = 1.0f;

	Vector(1.0f, 0.8f, 0.2f, Light);
	VectorCopy(obj->Position, Position);
	Position[2] += 30.0f;
	CreateParticle(ThreadIndex, BITMAP_SHINY, Position, obj->Angle, Light, 7);

	switch (m_state)
	{
	default: break;

	case eAction_Move:
		fSize = 0.8f;
		fSize2 = 3.0f;
		break;

	case eAction_Get:
		fSize = 0.8f;
		fSize2 = 3.0f;
		break;

	case eAction_Return:
		CreateEffect(ThreadIndex, MODEL_NEWYEARSDAY_EVENT_MONEY, Position, obj->Angle, Light);
		break;
	}

	b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(10)], vRelativePos, Position, false);
	Vector(1.0f, 0.8f, 0.2f, Light);
	CreateSprite(BITMAP_FLARE_RED, Position, (0.5f + fSize), Light, obj);
	Vector(1.0f, 0.1f, 0.2f, Light);
	CreateSprite(BITMAP_LIGHT, Position, (2.0f + fSize), Light, obj);

	mu_int32 temp[] = { 19, 32, 33, 34, 35 };
	for (mu_int32 i = 0; i<5; ++i)
	{
		b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(temp[i])], vRelativePos, Position, false);
		Vector(0.8f, 0.6f, 0.2f, Light);
		CreateSprite(BITMAP_LIGHT, Position, (0.6f * fSize2), Light, obj);
		Vector(0.8f, 0.8f, 0.2f, Light);
		CreateSprite(BITMAP_SHINY + 1, Position, (0.4f * fSize2), Light, obj);
	}

	return true;
}

mu_boolean MUPetActionCollecter::Sound(const mu_uint32 ThreadIndex, OBJECT* obj, CHARACTER *Owner, mu_int32 targetKey, mu_uint64 tick, mu_boolean bForceRender)
{
	if (nullptr == obj || nullptr == Owner) return false;

	switch (m_state)
	{
	default: break;

	case eAction_Return:
		PlayBackground(SOUND_DROP_GOLD01);
		break;
	}

	return true;
}

void MUPetActionCollecter::FindZen(OBJECT* obj)
{
	if (nullptr == obj || true == m_isRooting) return;

	mu_float dx, dy, dl;

	for (mu_int32 i = 0; i<MAX_ITEMS; ++i)
	{
		OBJECT *_item = &Items[i].Object;

		if (_item->Flags.Get(OBJECT::eOF_LIVE | OBJECT::eOF_VISIBLE) == false)
		{
			continue;
		}

		dx = obj->Owner->Position[0] - _item->Position[0];
		dy = obj->Owner->Position[1] - _item->Position[1];

		dl = EMath::Sqrt(dx*dx + dy*dy);

		if (SEARCH_LENGTH > dl)
		{
			if (Items[i].Item.Type != ITEM_POTION + 15)
			{
				continue;
			}

			if (!m_isRooting)
			{
				m_isRooting = true;
				m_RootItem.itemIndex = i;
				VectorCopy(_item->Position, m_RootItem.position);
			}
		}
	}
}

mu_boolean MUPetActionCollecter::CompTimeControl(const mu_uint64& dwCompTime, mu_uint64& dwTime)
{
	if ((g_CurrentTime - dwTime) > dwCompTime)
	{
		dwTime = g_CurrentTime;
		return true;
	}

	return false;
}