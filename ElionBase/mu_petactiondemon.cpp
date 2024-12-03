#include "stdafx.h"
#include "mu_petactiondemon.h"

MUPetActionDemon g_PetActionDemon;

MUPetActionDemon::MUPetActionDemon()
{

}

MUPetActionDemon::~MUPetActionDemon()
{

}

mu_boolean MUPetActionDemon::Release(OBJECT* obj, CHARACTER *Owner)
{
	DeleteEffect(BITMAP_CLUD64, obj);
	DeleteEffect(BITMAP_FIRE_HIK3, obj);

	return true;
}

mu_boolean MUPetActionDemon::Move(const mu_uint32 ThreadIndex, OBJECT* obj, CHARACTER *Owner, mu_int32 targetKey, mu_uint64 tick, mu_boolean bForceRender)
{
	mu_float fRad = (Q_PI / 10000.0f) * (mu_float)(tick % 10000);
	mu_float temp = EMath::Sin(fRad);

	Vector(temp, temp, temp, obj->Light);

	mu_float FlyRange = 50.0f;
	EVector3 Range, Direction;

	obj->Position[2] = obj->Owner->Position[2] + (230.0f * obj->Owner->Scale);

	VectorSubtract(obj->Owner->Position, obj->Position, Range);

	mu_float Distance = Range[0] * Range[0] + Range[1] * Range[1];
	if (Distance >= FlyRange*FlyRange*FlyRange*FlyRange)
	{
		VectorCopy(obj->Owner->Position, obj->Position);
		VectorCopy(obj->Owner->Angle, obj->Angle);
	}
	else if (Distance >= FlyRange*FlyRange)
	{
		mu_float Angle = CreateAngle(obj->Position[0], obj->Position[1], obj->Owner->Position[0], obj->Owner->Position[1]);
		obj->Angle[2] = TurnAngle2(obj->Angle[2], Angle, 10.0f);
	}

	EMatrix4 matrix;
	AngleMatrix(obj->Angle, matrix);
	VectorRotate(obj->Direction, matrix, Direction);
	VectorAdd(obj->Position, Direction, obj->Position);

	mu_float Speed = (FlyRange*FlyRange >= Distance) ? 0 : (mu_float)EMath::Log(Distance) + 5.0f;

	obj->Direction[0] = 0.0f;
	obj->Direction[1] = -Speed;
	obj->Direction[2] = 0.0f;

	return true;
}

mu_boolean MUPetActionDemon::Effect(const mu_uint32 ThreadIndex, OBJECT* obj, CHARACTER *Owner, mu_int32 targetKey, mu_uint64 tick, mu_boolean bForceRender)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	MUModel* b = &MODELS::Data[obj->Type];
	EVector3 Position, vRelativePos;
	EVector3 Light, Light2;
	mu_float fRad = ((Q_PI / 2500.0f) * (mu_float)(tick % 25000));
	mu_float temp = EMath::Sin(fRad) + 0.4f;

	Vector(0.0f, 0.0f, 0.0f, vRelativePos);
	Vector(temp*0.7f, temp*0.5f, temp*0.6f, Light);
	Vector(0.7f, 0.3f, 0.3f, Light2);

	mu_int32 itemp[] = { 8, 34, 51, 61, 56, 66, 52, 58, 44 };

	for (mu_int32 i = 0; i < 9; ++i)
	{
		b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(itemp[i])], vRelativePos, Position, false);

		switch (i)
		{

		case 0:
			CreateSprite(BITMAP_LIGHTMARKS_FOREIGN, Position, 1.0f, Light, obj);
			CreateSprite(BITMAP_FLARE, Position, 0.5f, Light, obj);
			break;

		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
			CreateParticle(ThreadIndex, BITMAP_CLUD64, Position, obj->Angle, Light, 11, 0.5f);
			break;

		case 6:
		case 7:
			CreateSprite(BITMAP_LIGHTMARKS_FOREIGN, Position, 0.5f, Light, obj);
			break;

		case 8:
			CreateParticle(ThreadIndex, BITMAP_FIRE_HIK3, Position, obj->Angle, Light, 1, 0.4f);
			CreateSprite(BITMAP_FLARE, Position, 1.5f, Light, obj);
			CreateSprite(BITMAP_FLARE, Position, 0.5f, Light2, obj);
			break;
		}
	}

	return true;
}