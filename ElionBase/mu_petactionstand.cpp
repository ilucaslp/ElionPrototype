#include "stdafx.h"
#include "mu_petactionstand.h"

MUPetActionStand g_PetActionStand;

MUPetActionStand::MUPetActionStand()
{

}

MUPetActionStand::~MUPetActionStand()
{

}

mu_boolean MUPetActionStand::Release(OBJECT* obj, CHARACTER *Owner)
{
	DeleteEffect(MODEL_FEATHER_FOREIGN, obj);

	return true;
}

mu_boolean MUPetActionStand::Move(const mu_uint32 ThreadIndex, OBJECT* obj, CHARACTER *Owner, mu_int32 targetKey, mu_uint64 tick, mu_boolean bForceRender)
{
	mu_float FlyRange = 50.0f;
	EVector3 Range, Direction;

	obj->Position[2] = obj->Owner->Position[2] + (230.0f * obj->Owner->Scale);
	VectorSubtract(obj->Owner->Position, obj->Position, Range);

	mu_float Distance = Range[0] * Range[0] + Range[1] * Range[1];
	if (Distance >= FlyRange*FlyRange*FlyRange*FlyRange)
	{
		VectorCopy(obj->Owner->Position, obj->Position);
		obj->Position[2] = obj->Owner->Position[2] + 200.0f;
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

	mu_float Speed = (FlyRange*FlyRange >= Distance) ? 0 : (mu_float)EMath::Log(Distance) * 1.8f;

	obj->Direction[0] = 0.0f;
	obj->Direction[1] = -Speed;
	obj->Direction[2] = 0.0f;

	return true;
}

mu_boolean MUPetActionStand::Effect(const mu_uint32 ThreadIndex, OBJECT* obj, CHARACTER *Owner, mu_int32 targetKey, mu_uint64 tick, mu_boolean bForceRender)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	MUModel* b = &MODELS::Data[obj->Type];
	EVector3 Position, vRelativePos;
	EVector3 Light;

	mu_float fRad1 = ((Q_PI / 3000.0f) * (mu_float)(tick % 3000));
	mu_float fRad2 = ((Q_PI / 3000.0f) * (mu_float)((tick + 1500) % 3000));
	mu_float tempLight;

	VectorCopy(obj->Position, b->BodyOrigin(ThreadIndex));
	Vector(0.0f, 0.0f, 0.0f, vRelativePos);

	b->Animation(ThreadIndex, ThreadInfo.BoneTransform, obj->AnimationFrame, obj->PriorAnimationFrame, obj->PriorAction, obj->Angle, obj->HeadAngle);

	Vector(0.7f, 0.2f, 0.6f, Light);
	b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(3)], vRelativePos, Position, false);
	CreateSprite(BITMAP_LIGHTMARKS_FOREIGN, Position, 1.5f, Light, obj);
	CreateSprite(BITMAP_LIGHTMARKS_FOREIGN, Position, 0.8f, Light, obj);

	Vector(0.3f, 0.3f, 0.6f, Light);
	b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(5)], vRelativePos, Position, false);
	Position[2] -= 25.0f;
	CreateEffect(ThreadIndex, MODEL_FEATHER_FOREIGN, Position, obj->Angle, Light, 4, nullptr, -1, 0, 0, 0, 0.3f);

	mu_int32 temp[] = { 45, 42, 48, 54, 51, 57, 25, 26, 27, 38, 32 };

	for (mu_int32 i = 0; i < 11; ++i)
	{
		b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(temp[i])], vRelativePos, Position, false);

		switch (i)
		{
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
			Vector(0.5f, 0.5f, 0.8f, Light);
			CreateSprite(BITMAP_LIGHTMARKS_FOREIGN, Position, 0.3f, Light, obj);
			break;

		case 6:
		case 7:
		case 8:
			Vector(0.6f, 0.2f, 0.8f, Light);
			CreateSprite(BITMAP_LIGHT, Position, 0.2f, Light, obj);
			break;

		case 9:
		case 10:
			tempLight = (0 == i % 2) ? EMath::Sin(fRad1) * 2.0f : EMath::Sin(fRad2) * 2.0f;
			Vector(tempLight, tempLight, tempLight, Light);
			CreateSprite(BITMAP_FLARE, Position, 0.5f, Light, obj);
			break;
		}
	}

	return true;
}