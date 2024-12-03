#include "stdafx.h"
#include "mu_petactionround.h"

MUPetActionRound g_PetActionRound;

MUPetActionRound::MUPetActionRound()
{

}

MUPetActionRound::~MUPetActionRound()
{

}

mu_boolean MUPetActionRound::Move(const mu_uint32 ThreadIndex, OBJECT* obj, CHARACTER *Owner, mu_int32 targetKey, mu_uint64 tick, mu_boolean bForceRender)
{
	mu_float fRadWidth = ((2.0f * Q_PI) / 5000.0f) * (mu_float)(tick % 5000);
	mu_float fRadHeight = ((2.0f * Q_PI) / 1000.0f) * (mu_float)(tick % 1000);
	EVector3 TargetPosition;

	VectorCopy(obj->Owner->Position, TargetPosition);
	VectorCopy(obj->Owner->HeadAngle, obj->HeadAngle);

	obj->Position[0] = TargetPosition[0] + (EMath::Sin(fRadWidth) * 150.0f);
	obj->Position[1] = TargetPosition[1] + (EMath::Cos(fRadWidth) * 150.0f);
	obj->Position[2] = TargetPosition[2] + 100.0f + (EMath::Sin(fRadHeight) * 30.0f);

	mu_float Angle = CreateAngle(obj->Position[0], obj->Position[1], TargetPosition[0], TargetPosition[1]);
	obj->Angle[2] = TurnAngle2(obj->Angle[2], Angle + 270.0f, 20.0f);

	return true;
}