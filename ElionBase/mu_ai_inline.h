#ifndef __MU_AI_INLINE_H__
#define __MU_AI_INLINE_H__

#pragma once

FORCEINLINE void MovePosition(EVector3 &Position, const EVector3 &Angle, const EVector3 &Speed)
{
	EBone Matrix;
	AngleMatrix(Angle, Matrix);
	EVector3 Velocity;
	VectorRotate(Speed, Matrix, Velocity);
	VectorAdd(Position, Velocity, Position);
}

#endif