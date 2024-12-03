#include "stdafx.h"
#include "mu_petactionunicorn.h"

MUPetActionUnicorn g_PetActionUnicorn;

MUPetActionUnicorn::MUPetActionUnicorn()
{
	m_isRooting = false;

	m_dwSendDelayTime = 0;
	m_dwRootingTime = 0;
	m_state = eAction_Stand;

	m_fRadWidthStand = 0.0f;
	m_fRadWidthGet = 0.0f;

	m_speed = 0;
}

MUPetActionUnicorn::~MUPetActionUnicorn()
{

}

mu_boolean MUPetActionUnicorn::Release(OBJECT* obj, CHARACTER *Owner)
{
	DeleteEffect(BITMAP_PIN_LIGHT, obj);

	return true;
}

mu_boolean MUPetActionUnicorn::Move(const mu_uint32 ThreadIndex, OBJECT* obj, CHARACTER *Owner, mu_int32 targetKey, mu_uint64 tick, mu_boolean bForceRender)
{
	if (nullptr == obj || nullptr == Owner) return false;

	FindZen(obj);

	if (eAction_Stand == m_state && m_isRooting)
	{
		m_state = eAction_Move;
	}

	if (m_speed == 0)
	{
		m_speed = obj->Velocity;
	}

	mu_float FlyRange = 10.0f;
	EVector3 targetPos, Range, Direction;

	m_fRadWidthStand = ((2.0f * Q_PI) / 4000.0f) * (mu_float)(tick % 4000);
	m_fRadWidthGet = ((2.0f * Q_PI) / 2000.0f) * (mu_float)(tick % 2000);

	obj->Position[2] = obj->Owner->Position[2] + (200.0f * obj->Owner->Scale);

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
			targetPos[0] = obj->Owner->Position[0];
			targetPos[1] = obj->Owner->Position[1];
			targetPos[2] = obj->Owner->Position[2];

			VectorSubtract(targetPos, obj->Position, Range);

			Distance = EMath::Sqrt(Range[0] * Range[0] + Range[1] * Range[1]);

			if (80.0f >= FlyRange)
			{
				mu_float Angle = CreateAngle(obj->Position[0], obj->Position[1], targetPos[0], targetPos[1]); //test
				obj->Angle[2] = TurnAngle2(obj->Angle[2], Angle, 8.0f);
			}

			EMatrix4 matrix;
			AngleMatrix(obj->Angle, matrix);
			VectorRotate(obj->Direction, matrix, Direction);
			VectorAdd(obj->Position, Direction, obj->Position);

			mu_float Speed = (FlyRange*FlyRange >= Distance) ? 0 : (mu_float)EMath::Log(Distance) * 2.3f;

			obj->Direction[0] = 0.0f;
			obj->Direction[1] = -Speed;
			obj->Direction[2] = 0.0f;

			if (Speed == 0)
			{
				obj->Velocity = m_speed * 0.35f;
			}
			else
			{
				obj->Velocity = m_speed * 1.2f;
			}
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
				mu_float Angle = CreateAngle(obj->Position[0], obj->Position[1], targetPos[0], targetPos[1]); //test
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

			obj->Velocity = m_speed;
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
			obj->Velocity = m_speed;
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
				mu_float Angle = CreateAngle(obj->Position[0], obj->Position[1], targetPos[0], targetPos[1]); //test
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
			obj->Velocity = m_speed;
		}
		break;
	}

	return true;
}

mu_boolean MUPetActionUnicorn::Effect(const mu_uint32 ThreadIndex, OBJECT* obj, CHARACTER *Owner, mu_int32 targetKey, mu_uint64 tick, mu_boolean bForceRender)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	if (nullptr == obj || nullptr == Owner) return false;

	MUModel* b = &MODELS::Data[obj->Type];
	EVector3 Position, vRelativePos, Light;

	VectorCopy(obj->Position, b->BodyOrigin(ThreadIndex));
	Vector(0.0f, 0.0f, 0.0f, vRelativePos);

	b->Animation(ThreadIndex, ThreadInfo.BoneTransform, obj->AnimationFrame, obj->PriorAnimationFrame, obj->PriorAction, obj->Angle, obj->HeadAngle);

	Vector(0.0f, 0.0f, 0.0f, vRelativePos);
	b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(11)], vRelativePos, Position, false);
	Vector(1.0f, 0.7f, 0.0f, Light);
	CreateSprite(BITMAP_MAGIC, Position, 0.15f, Light, obj);

	Vector(1.0f, 0.7f, 0.3f, Light);
	if (GetLargeRand(3) == 0)
		CreateEffect(ThreadIndex, BITMAP_PIN_LIGHT, Position, obj->Angle, Light, 4, obj, -1, 0, 0, 0, 0.45f);

	b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(4)], vRelativePos, Position, false);
	Vector(0.5f, 0.5f, 1.0f, Light);

	CreateSprite(BITMAP_SMOKE, Position, 1.2f, Light, obj);
	CreateSprite(BITMAP_LIGHT, Position, 4.0f, Light, obj);

	if (GetLargeRand(2) == 0)
		CreateParticle(ThreadIndex, BITMAP_SMOKE, Position, obj->Angle, Light, 67, 1.0f);

	Vector(0.7f, 0.7f, 1.0f, Light);
	b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(4)], vRelativePos, Position, false);
	CreateParticle(ThreadIndex, BITMAP_SMOKELINE1, Position, obj->Angle, Light, 4, 0.6f, obj);
	CreateParticle(ThreadIndex, BITMAP_SMOKELINE2, Position, obj->Angle, Light, 4, 0.6f, obj);
	CreateParticle(ThreadIndex, BITMAP_SMOKELINE3, Position, obj->Angle, Light, 4, 0.6f, obj);

	return true;
}

mu_boolean MUPetActionUnicorn::Sound(const mu_uint32 ThreadIndex, OBJECT* obj, CHARACTER *Owner, mu_int32 targetKey, mu_uint64 tick, mu_boolean bForceRender)
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

void MUPetActionUnicorn::FindZen(OBJECT* obj)
{
	if (nullptr == obj || true == m_isRooting) return;

	mu_float dx, dy, dl;

	for (mu_int32 i = 0; i < MAX_ITEMS; ++i)
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

mu_boolean MUPetActionUnicorn::CompTimeControl(const mu_uint64& dwCompTime, mu_uint64& dwTime)
{
	if ((g_CurrentTime - dwTime) > dwCompTime)
	{
		dwTime = g_CurrentTime;
		return true;
	}
	return false;
}