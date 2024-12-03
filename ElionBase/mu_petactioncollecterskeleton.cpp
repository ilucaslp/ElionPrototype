#include "stdafx.h"
#include "mu_petactioncollecterskeleton.h"

MUPetActionCollecterSkeleton g_PetActionCollecterSkeleton;

MUPetActionCollecterSkeleton::MUPetActionCollecterSkeleton()
{
	m_isRooting = false;
	m_bIsMoving = false;

	m_dwSendDelayTime = 0;
	m_dwRootingTime = 0;
	m_state = eAction_Stand;

	m_fRadWidthStand = 0.0f;
	m_fRadWidthGet = 0.0f;
}

MUPetActionCollecterSkeleton::~MUPetActionCollecterSkeleton()
{

}

mu_boolean MUPetActionCollecterSkeleton::Release(OBJECT* obj, CHARACTER *Owner)
{
	return true;
}

mu_boolean MUPetActionCollecterSkeleton::Move(const mu_uint32 ThreadIndex, OBJECT* obj, CHARACTER *Owner, mu_int32 targetKey, mu_uint64 tick, mu_boolean bForceRender)
{
	if (nullptr == obj || nullptr == Owner) return false;

	FindZen(obj);

	if (eAction_Stand == m_state && m_isRooting)
	{
		m_state = eAction_Move;
	}

	mu_float FlyRange = 12.0f;
	EVector3 targetPos, Range, Direction;
	m_fRadWidthStand = ((2.0f * Q_PI) / 4000.0f) * (mu_float)(tick % 4000);
	m_fRadWidthGet = ((2.0f * Q_PI) / 2000.0f) * (mu_float)(tick % 2000);

	obj->Position[2] = obj->Owner->Position[2] + (50.0f * obj->Owner->Scale);

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
				mu_float Angle = CreateAngle(obj->Position[0], obj->Position[1], targetPos[0], targetPos[1]);
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

mu_boolean MUPetActionCollecterSkeleton::Effect(const mu_uint32 ThreadIndex, OBJECT* obj, CHARACTER *Owner, mu_int32 targetKey, mu_uint64 tick, mu_boolean bForceRender)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	if (nullptr == obj || nullptr == Owner) return false;

	MUModel* b = &MODELS::Data[obj->Type];
	EVector3 vPosition, vLight;

	b->BodyScale(ThreadIndex) = obj->Scale;
	b->Animation(ThreadIndex, ThreadInfo.BoneTransform, obj->AnimationFrame, obj->PriorAnimationFrame, obj->PriorAction, obj->Angle, obj->HeadAngle, false, false);

	mu_float fLumi = (EMath::Sin(FWorldTime*0.003f) + 1.0f)*0.5f + 0.5f;
	Vector(0.0f*fLumi, 1.0f*fLumi, 0.5f*fLumi, vLight);

	b->TransformByBoneMatrix(ThreadIndex, vPosition, ThreadInfo.BoneTransform[b->GetBoneIndex(12)], &obj->Position);
	CreateSprite(BITMAP_LIGHTNING + 1, vPosition, 0.05f, vLight, obj);
	b->TransformByBoneMatrix(ThreadIndex, vPosition, ThreadInfo.BoneTransform[b->GetBoneIndex(11)], &obj->Position);
	CreateSprite(BITMAP_LIGHTNING + 1, vPosition, 0.05f, vLight, obj);

	b->TransformByBoneMatrix(ThreadIndex, vPosition, ThreadInfo.BoneTransform[b->GetBoneIndex(44)], &obj->Position);
	CreateSprite(BITMAP_LIGHT, vPosition, 0.3f, vLight, obj);
	b->TransformByBoneMatrix(ThreadIndex, vPosition, ThreadInfo.BoneTransform[b->GetBoneIndex(46)], &obj->Position);
	CreateSprite(BITMAP_LIGHT, vPosition, 0.6f, vLight, obj);
	b->TransformByBoneMatrix(ThreadIndex, vPosition, ThreadInfo.BoneTransform[b->GetBoneIndex(45)], &obj->Position);
	CreateSprite(BITMAP_LIGHT, vPosition, 0.4f, vLight, obj);
	b->TransformByBoneMatrix(ThreadIndex, vPosition, ThreadInfo.BoneTransform[b->GetBoneIndex(62)], &obj->Position);
	CreateSprite(BITMAP_LIGHT, vPosition, 0.3f, vLight, obj);

	m_bIsMoving = !(
		(Owner->Object.CurrentAction >= PLAYER_STOP_MALE && Owner->Object.CurrentAction <= PLAYER_STOP_RIDE_WEAPON)
		|| Owner->Object.CurrentAction == PLAYER_STOP_RIDE_HORSE
		|| Owner->Object.CurrentAction == PLAYER_STOP_TWO_HAND_SWORD_TWO
		|| Owner->Object.CurrentAction == PLAYER_DARKLORD_STAND
		|| (Owner->Object.CurrentAction >= PLAYER_FENRIR_DAMAGE && Owner->Object.CurrentAction <= PLAYER_FENRIR_DAMAGE_ONE_LEFT)
		|| (Owner->Object.CurrentAction >= PLAYER_FENRIR_STAND && Owner->Object.CurrentAction <= PLAYER_FENRIR_STAND_ONE_LEFT)
		|| (Owner->Object.CurrentAction >= PLAYER_DEFENSE1 && Owner->Object.CurrentAction <= PLAYER_CHANGE_UP)
		);

	if (m_bIsMoving == true)
	{
		Vector(0.0f, 1.0f, 0.5f, vLight);
		b->TransformByBoneMatrix(ThreadIndex, vPosition, ThreadInfo.BoneTransform[b->GetBoneIndex(13)], &obj->Position);

		EVector3 vAngle;
		VectorCopy(obj->Angle, vAngle);
		vAngle[0] += 35.0f;

		for (mu_int32 i = 0; i < 2; ++i)
		{
			if (i == 1 && GetLargeRand(2) == 0) continue;

			switch (GetLargeRand(3))
			{
			case 0:
				CreateParticle(ThreadIndex, BITMAP_FIRE_HIK1_MONO, vPosition, vAngle, vLight, 4, obj->Scale, obj);
				break;
			case 1:
				CreateParticle(ThreadIndex, BITMAP_FIRE_HIK2_MONO, vPosition, vAngle, vLight, 8, obj->Scale, obj);
				break;
			case 2:
				CreateParticle(ThreadIndex, BITMAP_FIRE_HIK3_MONO, vPosition, vAngle, vLight, 5, obj->Scale, obj);
				break;
			}
		}

		Vector(1.0f, 1.0f, 1.0f, vLight);
		CreateSprite(BITMAP_HOLE, vPosition, (EMath::Sin(FWorldTime*0.005f) + 1.0f)*0.1f + 0.1f, vLight, obj);
	}

	return true;
}

mu_boolean MUPetActionCollecterSkeleton::Sound(const mu_uint32 ThreadIndex, OBJECT* obj, CHARACTER *Owner, mu_int32 targetKey, mu_uint64 tick, mu_boolean bForceRender)
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

void MUPetActionCollecterSkeleton::FindZen(OBJECT* obj)
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

mu_boolean MUPetActionCollecterSkeleton::CompTimeControl(const mu_uint64& dwCompTime, mu_uint64& dwTime)
{
	if ((g_CurrentTime - dwTime) > dwCompTime)
	{
		dwTime = g_CurrentTime;
		return true;
	}

	return false;
}