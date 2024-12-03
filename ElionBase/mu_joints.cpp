#include "stdafx.h"
#include "mu_joints.h"

MUCriticalSectionSpinLock g_JointsLock;
MUCriticalSectionSpinLock g_JointsEraseLock;
std::array<JOINT_HEADER, MAX_JOINTS> g_JointsHeader;
std::array<JOINT_CHECK, MAX_JOINTS> g_JointsCheck;
std::array<JOINT, MAX_JOINTS> g_Joints;
std::array<mu_uint16, MAX_JOINTS + MAX_TASKING_THREADS> g_JointsQueue;
std::array<mu_uint16, MAX_JOINTS> g_JointsFree;
mu_uint16 g_JointsRenderCount;
mu_uint16 g_JointsCurCount = 0;
mu_uint16 g_JointsFreeCount = 0;
mu_atomic_int32_t g_JointsDeleteCount(0);
mu_boolean g_JointsLocked = false;

std::array<JOINT_WAIT, MAX_JOINTS_WAIT> g_JointsWait;
mu_atomic_uint32_t g_JointsWaitCount(0);
std::array<JOINT_DELETE, MAX_JOINTS_WAIT> g_JointsErase;
mu_uint16 g_JointsEraseCount = 0;

void GetMagicScrew(mu_int32 iParam, EVector3 &vResult, mu_float fSpeedRate)
{
	iParam += MoveSceneFrame;

	EVector3 vDirTemp;

	mu_float fSpeed[3] = { 0.048f, 0.0613f, 0.1113f };
	fSpeed[0] *= fSpeedRate;
	fSpeed[1] *= fSpeedRate;
	fSpeed[2] *= fSpeedRate;
	vDirTemp[0] = EMath::Sin((mu_float)(iParam + 55555)*fSpeed[0]) * EMath::Cos((mu_float)iParam*fSpeed[1]);
	vDirTemp[1] = EMath::Sin((mu_float)(iParam + 55555)*fSpeed[0]) * EMath::Sin((mu_float)iParam*fSpeed[1]);
	vDirTemp[2] = EMath::Cos((mu_float)(iParam + 55555)*fSpeed[0]);

	mu_float fSinAdd = EMath::Sin((mu_float)(iParam + 11111)*fSpeed[2]);
	mu_float fCosAdd = EMath::Cos((mu_float)(iParam + 11111)*fSpeed[2]);
	vResult[2] = vDirTemp[0];
	vResult[1] = fSinAdd * vDirTemp[1] + fCosAdd * vDirTemp[2];
	vResult[0] = fCosAdd * vDirTemp[1] - fSinAdd * vDirTemp[2];
}

#ifdef USE_DUAL_QUATERNION
void CreateTailAxis(JOINT *o, const EDualQuat &Matrix, const mu_uint8 axis)
{
	if ((mu_int32)o->NumTails < (mu_int32)o->MaxTails - 1)
	{
		++o->NumTails;
	}

	if (o->NumTails >= 0)
	{
		if (o->BeginTails == 0)
		{
			o->BeginTails = o->MaxTails - 1;
		}
		else
		{
			--o->BeginTails;
		}
	}

	EVector3 Position, p;
	if (axis == 0)
	{
		Vector(-o->Scale*0.5f, 0.0f, 0.0f, Position);
		Matrix.TransformNormalOut(Position, p);
		VectorAdd(o->Position, p, o->Tails[o->BeginTails][0]);
		Vector(o->Scale*0.5f, 0.0f, 0.0f, Position);
		Matrix.TransformNormalOut(Position, p);
		VectorAdd(o->Position, p, o->Tails[o->BeginTails][1]);
		Vector(0.0f, 0.0f, -o->Scale*0.5f, Position);
		Matrix.TransformNormalOut(Position, p);
		VectorAdd(o->Position, p, o->Tails[o->BeginTails][2]);
		Vector(0.0f, 0.0f, o->Scale*0.5f, Position);
		Matrix.TransformNormalOut(Position, p);
		VectorAdd(o->Position, p, o->Tails[o->BeginTails][3]);
	}
	else
	{
		Vector(-o->Scale*0.5f, 0.0f, 0.0f, Position);
		Matrix.TransformNormalOut(Position, p);
		VectorAdd(o->Position, p, o->Tails[o->BeginTails][0]);
		Vector(o->Scale*0.5f, 0.0f, 0.0f, Position);
		Matrix.TransformNormalOut(Position, p);
		VectorAdd(o->Position, p, o->Tails[o->BeginTails][1]);
		Vector(0.0f, -o->Scale*0.5f, 0.0f, Position);
		Matrix.TransformNormalOut(Position, p);
		VectorAdd(o->Position, p, o->Tails[o->BeginTails][2]);
		Vector(0.0f, o->Scale*0.5f, 0.0f, Position);
		Matrix.TransformNormalOut(Position, p);
		VectorAdd(o->Position, p, o->Tails[o->BeginTails][3]);
	}
}

void CreateTailAxis(JOINT *o, const EDualQuat &Matrix, const mu_float ScaleX, const mu_float ScaleY, const mu_uint8 axis)
{
	if ((mu_int32)o->NumTails < (mu_int32)o->MaxTails - 1)
	{
		++o->NumTails;
	}

	if (o->NumTails >= 0)
	{
		if (o->BeginTails == 0)
		{
			o->BeginTails = o->MaxTails - 1;
		}
		else
		{
			--o->BeginTails;
		}
	}

	EVector3 Position, p;
	if (axis == 0)
	{
		Vector(-ScaleX * 0.5f, 0.0f, 0.0f, Position);
		Matrix.TransformNormalOut(Position, p);
		VectorAdd(o->Position, p, o->Tails[o->BeginTails][0]);
		Vector(ScaleX*0.5f, 0.0f, 0.0f, Position);
		Matrix.TransformNormalOut(Position, p);
		VectorAdd(o->Position, p, o->Tails[o->BeginTails][1]);
		Vector(0.0f, 0.0f, -ScaleY * 0.5f, Position);
		Matrix.TransformNormalOut(Position, p);
		VectorAdd(o->Position, p, o->Tails[o->BeginTails][2]);
		Vector(0.0f, 0.0f, ScaleY*0.5f, Position);
		Matrix.TransformNormalOut(Position, p);
		VectorAdd(o->Position, p, o->Tails[o->BeginTails][3]);
	}
	else
	{
		Vector(-ScaleX * 0.5f, 0.0f, 0.0f, Position);
		Matrix.TransformNormalOut(Position, p);
		VectorAdd(o->Position, p, o->Tails[o->BeginTails][0]);
		Vector(ScaleX*0.5f, 0.0f, 0.0f, Position);
		Matrix.TransformNormalOut(Position, p);
		VectorAdd(o->Position, p, o->Tails[o->BeginTails][1]);
		Vector(0.0f, -ScaleY * 0.5f, 0.0f, Position);
		Matrix.TransformNormalOut(Position, p);
		VectorAdd(o->Position, p, o->Tails[o->BeginTails][2]);
		Vector(0.0f, ScaleY*0.5f, 0.0f, Position);
		Matrix.TransformNormalOut(Position, p);
		VectorAdd(o->Position, p, o->Tails[o->BeginTails][3]);
	}
}

void CreateTail(JOINT *o, const EDualQuat &Matrix, const mu_boolean Blur)
{
	if (Blur == true)
	{
		for (mu_int32 i = 0; i < 2; ++i)
		{
			if ((mu_int32)o->NumTails < (mu_int32)o->MaxTails - 1)
			{
				++o->NumTails;
			}

			if (o->NumTails >= 0)
			{
				if (o->BeginTails == 0)
				{
					o->BeginTails = o->MaxTails - 1;
				}
				else
				{
					--o->BeginTails;
				}
			}

			mu_uint32 curTail = o->BeginTails, nextTail = (o->BeginTails + 1) % o->MaxTails;

			if (i == 0 && o->NumTails > 1)
			{
				EVector3 Position, p;
				Vector(-o->Scale*0.5f, 0.0f, 0.0f, Position);
				Matrix.TransformNormalOut(Position, p);
				VectorAdd(o->Position, p, o->Tails[curTail][0]);
				o->Tails[curTail][0] = (o->Tails[curTail][0] + o->Tails[nextTail][0]) * 0.5f;
				Vector(o->Scale*0.5f, 0.0f, 0.0f, Position);
				Matrix.TransformNormalOut(Position, p);
				VectorAdd(o->Position, p, o->Tails[curTail][1]);
				o->Tails[curTail][1] = (o->Tails[curTail][1] + o->Tails[nextTail][1]) * 0.5f;
				Vector(0.0f, 0.0f, -o->Scale*0.5f, Position);
				Matrix.TransformNormalOut(Position, p);
				VectorAdd(o->Position, p, o->Tails[curTail][2]);
				o->Tails[curTail][2] = (o->Tails[curTail][2] + o->Tails[nextTail][2]) * 0.5f;
				Vector(0.0f, 0.0f, o->Scale*0.5f, Position);
				Matrix.TransformNormalOut(Position, p);
				VectorAdd(o->Position, p, o->Tails[curTail][3]);
				o->Tails[curTail][3] = (o->Tails[curTail][3] + o->Tails[nextTail][3]) * 0.5f;
			}
			else
			{
				EVector3 Position, p;
				Vector(-o->Scale*0.5f, 0.0f, 0.0f, Position);
				Matrix.TransformNormalOut(Position, p);
				VectorAdd(o->Position, p, o->Tails[curTail][0]);
				Vector(o->Scale*0.5f, 0.0f, 0.0f, Position);
				Matrix.TransformNormalOut(Position, p);
				VectorAdd(o->Position, p, o->Tails[curTail][1]);
				Vector(0.0f, 0.0f, -o->Scale*0.5f, Position);
				Matrix.TransformNormalOut(Position, p);
				VectorAdd(o->Position, p, o->Tails[curTail][2]);
				Vector(0.0f, 0.0f, o->Scale*0.5f, Position);
				Matrix.TransformNormalOut(Position, p);
				VectorAdd(o->Position, p, o->Tails[curTail][3]);
			}
		}
	}
	else
	{
		if ((mu_int32)o->NumTails < (mu_int32)o->MaxTails - 1)
		{
			++o->NumTails;
		}

		if (o->NumTails >= 0)
		{
			if (o->BeginTails == 0)
			{
				o->BeginTails = o->MaxTails - 1;
			}
			else
			{
				--o->BeginTails;
			}
		}

		EVector3 Position, p;
		Vector(-o->Scale*0.5f, 0.0f, 0.0f, Position);
		Matrix.TransformNormalOut(Position, p);
		VectorAdd(o->Position, p, o->Tails[o->BeginTails][0]);
		Vector(o->Scale*0.5f, 0.0f, 0.0f, Position);
		Matrix.TransformNormalOut(Position, p);
		VectorAdd(o->Position, p, o->Tails[o->BeginTails][1]);
		Vector(0.0f, 0.0f, -o->Scale*0.5f, Position);
		Matrix.TransformNormalOut(Position, p);
		VectorAdd(o->Position, p, o->Tails[o->BeginTails][2]);
		Vector(0.0f, 0.0f, o->Scale*0.5f, Position);
		Matrix.TransformNormalOut(Position, p);
		VectorAdd(o->Position, p, o->Tails[o->BeginTails][3]);
	}
}

void CreateTail(JOINT *o, const EDualQuat &Matrix, const mu_float ScaleX, const mu_float ScaleY)
{
	if ((mu_int32)o->NumTails < (mu_int32)o->MaxTails - 1)
	{
		++o->NumTails;
	}

	if (o->NumTails >= 0)
	{
		if (o->BeginTails == 0)
		{
			o->BeginTails = o->MaxTails - 1;
		}
		else
		{
			--o->BeginTails;
		}
	}

	EVector3 Position, p;
	Vector(-ScaleX * 0.5f, 0.0f, 0.0f, Position);
	Matrix.TransformNormalOut(Position, p);
	VectorAdd(o->Position, p, o->Tails[o->BeginTails][0]);
	Vector(ScaleX*0.5f, 0.0f, 0.0f, Position);
	Matrix.TransformNormalOut(Position, p);
	VectorAdd(o->Position, p, o->Tails[o->BeginTails][1]);
	Vector(0.0f, 0.0f, -ScaleY * 0.5f, Position);
	Matrix.TransformNormalOut(Position, p);
	VectorAdd(o->Position, p, o->Tails[o->BeginTails][2]);
	Vector(0.0f, 0.0f, ScaleY*0.5f, Position);
	Matrix.TransformNormalOut(Position, p);
	VectorAdd(o->Position, p, o->Tails[o->BeginTails][3]);
}
#endif

void CreateTailAxis(JOINT *o, const EMatrix4 &Matrix, const mu_uint8 axis)
{
	if ((mu_int32)o->NumTails < (mu_int32)o->MaxTails - 1)
	{
		++o->NumTails;
	}

	if (o->NumTails >= 0)
	{
		if (o->BeginTails == 0)
		{
			o->BeginTails = o->MaxTails - 1;
		}
		else
		{
			--o->BeginTails;
		}
	}

	EVector3 Position, p;
	if (axis == 0)
	{
		Vector(-o->Scale*0.5f, 0.0f, 0.0f, Position);
		p = EMath::TransformNormal(Matrix, Position);
		VectorAdd(o->Position, p, o->Tails[o->BeginTails][0]);
		Vector(o->Scale*0.5f, 0.0f, 0.0f, Position);
		p = EMath::TransformNormal(Matrix, Position);
		VectorAdd(o->Position, p, o->Tails[o->BeginTails][1]);
		Vector(0.0f, 0.0f, -o->Scale*0.5f, Position);
		p = EMath::TransformNormal(Matrix, Position);
		VectorAdd(o->Position, p, o->Tails[o->BeginTails][2]);
		Vector(0.0f, 0.0f, o->Scale*0.5f, Position);
		p = EMath::TransformNormal(Matrix, Position);
		VectorAdd(o->Position, p, o->Tails[o->BeginTails][3]);
	}
	else
	{
		Vector(-o->Scale*0.5f, 0.0f, 0.0f, Position);
		p = EMath::TransformNormal(Matrix, Position);
		VectorAdd(o->Position, p, o->Tails[o->BeginTails][0]);
		Vector(o->Scale*0.5f, 0.0f, 0.0f, Position);
		p = EMath::TransformNormal(Matrix, Position);
		VectorAdd(o->Position, p, o->Tails[o->BeginTails][1]);
		Vector(0.0f, -o->Scale*0.5f, 0.0f, Position);
		p = EMath::TransformNormal(Matrix, Position);
		VectorAdd(o->Position, p, o->Tails[o->BeginTails][2]);
		Vector(0.0f, o->Scale*0.5f, 0.0f, Position);
		p = EMath::TransformNormal(Matrix, Position);
		VectorAdd(o->Position, p, o->Tails[o->BeginTails][3]);
	}
}

void CreateTailAxis(JOINT *o, const EMatrix4 &Matrix, const mu_float ScaleX, const mu_float ScaleY, const mu_uint8 axis)
{
	if ((mu_int32)o->NumTails < (mu_int32)o->MaxTails - 1)
	{
		++o->NumTails;
	}

	if (o->NumTails >= 0)
	{
		if (o->BeginTails == 0)
		{
			o->BeginTails = o->MaxTails - 1;
		}
		else
		{
			--o->BeginTails;
		}
	}

	EVector3 Position, p;
	if (axis == 0)
	{
		Vector(-ScaleX*0.5f, 0.0f, 0.0f, Position);
		p = EMath::TransformNormal(Matrix, Position);
		VectorAdd(o->Position, p, o->Tails[o->BeginTails][0]);
		Vector(ScaleX*0.5f, 0.0f, 0.0f, Position);
		p = EMath::TransformNormal(Matrix, Position);
		VectorAdd(o->Position, p, o->Tails[o->BeginTails][1]);
		Vector(0.0f, 0.0f, -ScaleY*0.5f, Position);
		p = EMath::TransformNormal(Matrix, Position);
		VectorAdd(o->Position, p, o->Tails[o->BeginTails][2]);
		Vector(0.0f, 0.0f, ScaleY*0.5f, Position);
		p = EMath::TransformNormal(Matrix, Position);
		VectorAdd(o->Position, p, o->Tails[o->BeginTails][3]);
	}
	else
	{
		Vector(-ScaleX*0.5f, 0.0f, 0.0f, Position);
		p = EMath::TransformNormal(Matrix, Position);
		VectorAdd(o->Position, p, o->Tails[o->BeginTails][0]);
		Vector(ScaleX*0.5f, 0.0f, 0.0f, Position);
		p = EMath::TransformNormal(Matrix, Position);
		VectorAdd(o->Position, p, o->Tails[o->BeginTails][1]);
		Vector(0.0f, -ScaleY*0.5f, 0.0f, Position);
		p = EMath::TransformNormal(Matrix, Position);
		VectorAdd(o->Position, p, o->Tails[o->BeginTails][2]);
		Vector(0.0f, ScaleY*0.5f, 0.0f, Position);
		p = EMath::TransformNormal(Matrix, Position);
		VectorAdd(o->Position, p, o->Tails[o->BeginTails][3]);
	}
}

void CreateTail(JOINT *o, const EMatrix4 &Matrix, const mu_boolean Blur)
{
	if (Blur == true)
	{
		for (mu_int32 i = 0; i < 2; ++i)
		{
			if ((mu_int32)o->NumTails < (mu_int32)o->MaxTails - 1)
			{
				++o->NumTails;
			}

			if (o->NumTails >= 0)
			{
				if (o->BeginTails == 0)
				{
					o->BeginTails = o->MaxTails - 1;
				}
				else
				{
					--o->BeginTails;
				}
			}

			mu_uint32 curTail = o->BeginTails, nextTail = (o->BeginTails + 1) % o->MaxTails;

			if (i == 0 && o->NumTails > 1)
			{
				EVector3 Position, p;
				Vector(-o->Scale*0.5f, 0.0f, 0.0f, Position);
				p = EMath::TransformNormal(Matrix, Position);
				VectorAdd(o->Position, p, o->Tails[curTail][0]);
				o->Tails[curTail][0] = (o->Tails[curTail][0] + o->Tails[nextTail][0]) * 0.5f;
				Vector(o->Scale*0.5f, 0.0f, 0.0f, Position);
				p = EMath::TransformNormal(Matrix, Position);
				VectorAdd(o->Position, p, o->Tails[curTail][1]);
				o->Tails[curTail][1] = (o->Tails[curTail][1] + o->Tails[nextTail][1]) * 0.5f;
				Vector(0.0f, 0.0f, -o->Scale*0.5f, Position);
				p = EMath::TransformNormal(Matrix, Position);
				VectorAdd(o->Position, p, o->Tails[curTail][2]);
				o->Tails[curTail][2] = (o->Tails[curTail][2] + o->Tails[nextTail][2]) * 0.5f;
				Vector(0.0f, 0.0f, o->Scale*0.5f, Position);
				p = EMath::TransformNormal(Matrix, Position);
				VectorAdd(o->Position, p, o->Tails[curTail][3]);
				o->Tails[curTail][3] = (o->Tails[curTail][3] + o->Tails[nextTail][3]) * 0.5f;
			}
			else
			{
				EVector3 Position, p;
				Vector(-o->Scale*0.5f, 0.0f, 0.0f, Position);
				p = EMath::TransformNormal(Matrix, Position);
				VectorAdd(o->Position, p, o->Tails[curTail][0]);
				Vector(o->Scale*0.5f, 0.0f, 0.0f, Position);
				p = EMath::TransformNormal(Matrix, Position);
				VectorAdd(o->Position, p, o->Tails[curTail][1]);
				Vector(0.0f, 0.0f, -o->Scale*0.5f, Position);
				p = EMath::TransformNormal(Matrix, Position);
				VectorAdd(o->Position, p, o->Tails[curTail][2]);
				Vector(0.0f, 0.0f, o->Scale*0.5f, Position);
				p = EMath::TransformNormal(Matrix, Position);
				VectorAdd(o->Position, p, o->Tails[curTail][3]);
			}
		}
	}
	else
	{
		if ((mu_int32)o->NumTails < (mu_int32)o->MaxTails - 1)
		{
			++o->NumTails;
		}

		if (o->NumTails >= 0)
		{
			if (o->BeginTails == 0)
			{
				o->BeginTails = o->MaxTails - 1;
			}
			else
			{
				--o->BeginTails;
			}
		}

		EVector3 Position, p;
		Vector(-o->Scale*0.5f, 0.0f, 0.0f, Position);
		p = EMath::TransformNormal(Matrix, Position);
		VectorAdd(o->Position, p, o->Tails[o->BeginTails][0]);
		Vector(o->Scale*0.5f, 0.0f, 0.0f, Position);
		p = EMath::TransformNormal(Matrix, Position);
		VectorAdd(o->Position, p, o->Tails[o->BeginTails][1]);
		Vector(0.0f, 0.0f, -o->Scale*0.5f, Position);
		p = EMath::TransformNormal(Matrix, Position);
		VectorAdd(o->Position, p, o->Tails[o->BeginTails][2]);
		Vector(0.0f, 0.0f, o->Scale*0.5f, Position);
		p = EMath::TransformNormal(Matrix, Position);
		VectorAdd(o->Position, p, o->Tails[o->BeginTails][3]);
	}
}

void CreateTail(JOINT *o, const EMatrix4 &Matrix, const mu_float ScaleX, const mu_float ScaleY)
{
	if ((mu_int32)o->NumTails < (mu_int32)o->MaxTails - 1)
	{
		++o->NumTails;
	}

	if (o->NumTails >= 0)
	{
		if (o->BeginTails == 0)
		{
			o->BeginTails = o->MaxTails - 1;
		}
		else
		{
			--o->BeginTails;
		}
	}

	EVector3 Position, p;
	Vector(-ScaleX*0.5f, 0.0f, 0.0f, Position);
	p = EMath::TransformNormal(Matrix, Position);
	VectorAdd(o->Position, p, o->Tails[o->BeginTails][0]);
	Vector(ScaleX*0.5f, 0.0f, 0.0f, Position);
	p = EMath::TransformNormal(Matrix, Position);
	VectorAdd(o->Position, p, o->Tails[o->BeginTails][1]);
	Vector(0.0f, 0.0f, -ScaleY*0.5f, Position);
	p = EMath::TransformNormal(Matrix, Position);
	VectorAdd(o->Position, p, o->Tails[o->BeginTails][2]);
	Vector(0.0f, 0.0f, ScaleY*0.5f, Position);
	p = EMath::TransformNormal(Matrix, Position);
	VectorAdd(o->Position, p, o->Tails[o->BeginTails][3]);
}

template<mu_boolean isLockedAllowed>
void CreateJoint(const mu_uint32 ThreadIndex, mu_uint32 Type, EVector3 Position, EVector3 TargetPosition, EVector3 Angle, mu_uint32 SubType, OBJECT *Target, mu_float Scale, mu_int16 PKKey,
	mu_uint16 SkillIndex, mu_uint16 SkillSerialNum, mu_int32 iChaIndex, const EVector3 *vPriorColor, mu_int16 sTargetIndex)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	if (ThreadInfo.RenderGUI == true)
	{
		return;
	}

	switch (Type)
	{
	case BITMAP_FLARE:
	case BITMAP_FLARE_BLUE:
		{
			if (SubType == 20 && Target == nullptr)
			{
				return;
			}
		}
		break;

	case BITMAP_FLARE + 1:
		{
			if (SubType >= 8 && SubType <= 11 && Target == nullptr)
			{
				return;
			}
		}
		break;

	case BITMAP_JOINT_FORCE:
		{
			if (SubType == 1 && Target == nullptr)
			{
				return;
			}
		}
		break;

	case BITMAP_FLASH:
		{
			if (SubType == 4 && Target == nullptr)
			{
				return;
			}
		}
		break;
	}

	if constexpr(isLockedAllowed)
	{
		if (ThreadInfo.NoLockedAllowed == false &&
			g_JointsLocked == true)
		{
			const mu_uint32 i = g_JointsWaitCount.fetch_add(1);

			if (i < MAX_JOINTS_WAIT)
			{
				JOINT_WAIT &o = g_JointsWait[i];

				o.Type = Type;
				o.Position = Position;
				o.TargetPosition = TargetPosition;
				o.Angle = Angle;
				o.SubType = SubType;
				o.Target = Target;
				o.Scale = Scale;
				o.PKKey = PKKey;
				o.SkillIndex = SkillIndex;
				o.SkillSerialNum = SkillSerialNum;
				o.iChaIndex = iChaIndex;

				if (vPriorColor != nullptr)
				{
					o.EnablePriorColor = true;
					o.vPriorColor = *vPriorColor;
				}
				else
				{
					o.EnablePriorColor = false;
				}

				o.sTargetIndex = sTargetIndex;
			}

			return;
		}
	}

	mu_uint32 i = 0;

	g_JointsLock.Lock();

	if (g_JointsFreeCount > 0)
	{
		i = g_JointsFree[--g_JointsFreeCount];
	}
	else if (g_JointsCurCount < MAX_JOINTS)
	{
		i = g_JointsCurCount++;
	}
	else
	{
		g_JointsLock.Unlock();
		return;
	}

	g_JointsLock.Unlock();

	JOINT_HEADER *oh = &g_JointsHeader[i];
	JOINT_CHECK *oc = &g_JointsCheck[i];
	JOINT *o = &g_Joints[i];

	oc->CreationTime = g_CreationTime;
	oh->Flags.SetDirect(JOINT::eJF_LIVE);
	o->Features.SetDirect(JOINT::eJFS_CREATETAILS | JOINT::eJFS_FACEONE | JOINT::eJFS_FACETWO);

	o->Type = Type;
	
	o->TexType = o->Type;
	o->SubType = SubType;
	o->RenderType = BLEND::TXB_BLEND01;
	o->PKKey = PKKey;
	o->Skill = SkillIndex;
	o->Velocity = 0.0f;
	o->Target = nullptr;
	o->ReverseUV = 0;
	o->Features.Set(JOINT::eJFS_ALTERNATIVE * (mu_int32)(SkillSerialNum > 0));
	o->m_sTargetIndex = sTargetIndex;
	VectorCopy(Position, o->Position);
	VectorCopy(Angle, o->Angle);
	if (vPriorColor)
	{
		VectorCopy(*vPriorColor, o->Light);
	}
	else
	{
		Vector(1.0f, 1.0f, 1.0f, o->Light);
	}
	Vector(0.0f, 0.0f, 0.0f, o->Direction);
	if (Target == nullptr)
	{
		VectorCopy(TargetPosition, o->TargetPosition);
	}
	else if (MODEL_SPEARSKILL == Type && o->SubType == 2)
	{
		VectorCopy(TargetPosition, o->TargetPosition);
		o->Target = Target;
	}
	else if (Type == MODEL_SPEARSKILL && (o->SubType == 4 || o->SubType == 9))
	{
		if (iChaIndex != -1)
			o->m_iChaIndex = iChaIndex;
		o->Target = Target;
	}
	else if (Type == MODEL_SPEARSKILL && (o->SubType == 10 || o->SubType == 11))
	{
		if (iChaIndex != -1)
		{
			o->m_iChaIndex = FindCharacterIndex(iChaIndex);
		}
		o->Target = Target;
	}
	else
	{
		o->Target = Target;
	}

	o->BeginTails = 0;
	o->NumTails = 0;
	EBone Matrix;
	EVector3 p;

	mu_boolean bCreateStartTail = true;
	if (Type == BITMAP_FLARE + 1 && o->SubType == 8)
	{
		bCreateStartTail = false;
		o->NumTails = -1;
	}
	else if (Type == BITMAP_SCOLPION_TAIL)
	{
		bCreateStartTail = false;
		o->NumTails = -1;
	}
	else if (Type == BITMAP_JOINT_ENERGY && (o->SubType == 10 || o->SubType == 11))
	{
		bCreateStartTail = false;
		o->NumTails = -1;
	}
	else if (Type == BITMAP_JOINT_ENERGY && (o->SubType == 14 || o->SubType == 15))
	{
		bCreateStartTail = false;
		o->NumTails = -1;
	}
	else if (Type == BITMAP_JOINT_ENERGY && (o->SubType == 55 || o->SubType == 56))
	{
		bCreateStartTail = false;
		o->NumTails = -1;
	}
	else if (Type == BITMAP_JOINT_ENERGY && o->SubType == 57)
	{
		bCreateStartTail = false;
		o->NumTails = -1;
	}

	if (Type == BITMAP_FLARE_FORCE && (o->SubType == 5 || o->SubType == 6 || o->SubType == 7))
	{
		bCreateStartTail = false;
	}

	if (Type == MODEL_SPEARSKILL && (o->SubType == 5 || o->SubType == 6 || o->SubType == 7))
	{
		bCreateStartTail = false;
	}

	if ((Type == BITMAP_FLASH) && (o->SubType == 6))
		bCreateStartTail = false;

	if (bCreateStartTail)
	{
		Matrix = EMath::EulerToMatrix(o->Angle);
		Vector(-o->Scale*0.5f, 0.0f, 0.0f, Position);
		p = EMath::TransformNormal(Matrix, Position);
		VectorAdd(o->Position, p, o->Tails[o->BeginTails][0]);
		Vector(o->Scale*0.5f, 0.0f, 0.0f, Position);
		p = EMath::TransformNormal(Matrix, Position);
		VectorAdd(o->Position, p, o->Tails[o->BeginTails][1]);
		Vector(0.0f, 0.0f, -o->Scale*0.5f, Position);
		p = EMath::TransformNormal(Matrix, Position);
		VectorAdd(o->Position, p, o->Tails[o->BeginTails][2]);
		Vector(0.0f, 0.0f, o->Scale*0.5f, Position);
		p = EMath::TransformNormal(Matrix, Position);
		VectorAdd(o->Position, p, o->Tails[o->BeginTails][3]);
	}

	EVector3 BitePosition;
	switch (Type)
	{
	case BITMAP_SCOLPION_TAIL:
		o->Scale = Scale;
		o->LifeTime = 120;
		o->MaxTails = 28;
		o->Velocity = 3.0f;
		break;
		
	case BITMAP_JOINT_ENERGY:
		o->Scale = Scale;
		switch (o->SubType)
		{
		case 0:
		case 1:
		case 6:
		case 7:
		case 12:
		case 13:
		case 16:
		case 44:
		case 45:
		case 46:
			o->LifeTime = 120;
			o->MaxTails = 8;
			o->Velocity = 3.0f;
			if (o->SubType == 44)
			{
				o->LifeTime = 45;
				o->MaxTails = 20;
			}
			if (o->SubType == 12 || o->SubType == 13 || o->SubType == 16
				|| o->SubType == 46
				)
			{
				if (GetLargeRand(2))
					o->MultiUse = 1;
				else
					o->MultiUse = -1;
				o->Velocity = 10.0f;
				o->Angle[0] = (mu_float)(GetLargeRand(45));
				o->Angle[1] = (mu_float)(GetLargeRand(45));
			}
			break;
		case 9:
			Vector(0.2f, 0.2f, 0.2f, o->Light);
			o->LifeTime = 120;
			o->MaxTails = 8;
			o->Velocity = 3.0f;
			break;
		case 17:
			o->Velocity = 0.0f;
			o->LifeTime = 12;
			VectorCopy(o->Target->EyeLeft2, o->Position);
			break;
		case 47:		
			{
				o->Velocity = 0.0f;
				o->LifeTime = 12;
				VectorCopy(o->Target->EyeRight, o->Position);
			}
			break;
		case 2:
		case 3:
		case 4:
		case 5:
		case 8:
		case 10:
		case 11:
		case 14:
		case 15:
		case 18:	
		case 19:
		case 20:
		case 21:
		case 26:
		case 27:
		case 28:	
		case 29:
		case 30:
		case 31:
		case 32:
		case 33:
			o->Velocity = 0.0f;
			o->LifeTime = 999999999;
			if (o->SubType == 5) o->MaxTails = 10;
			else if (o->SubType == 10 || o->SubType == 11) o->MaxTails = 5;
			else if (o->SubType == 14 || o->SubType == 15) o->MaxTails = 15;
			else o->MaxTails = 20;
			if (o->SubType == 3 || o->SubType == 11 || o->SubType == 15)
			{
				o->MaxTails = 20;
				VectorCopy(o->Target->EyeRight, o->Position);
			}
			else if (o->SubType == 18 || o->SubType == 28)
			{
				o->MaxTails = 20;
				VectorCopy(o->Target->EyeLeft, o->Position);
			}
			else if (o->SubType == 19 || o->SubType == 29)
			{
				o->MaxTails = 20;
				VectorCopy(o->Target->EyeRight, o->Position);
			}
			else if (o->SubType == 20 || o->SubType == 30)
			{
				o->MaxTails = 20;
				VectorCopy(o->Target->EyeLeft2, o->Position);
			}
			else if (o->SubType == 21 || o->SubType == 31)
			{
				o->MaxTails = 20;
				VectorCopy(o->Target->EyeRight2, o->Position);
			}
			else if (o->SubType == 26 || o->SubType == 32)
			{
				o->MaxTails = 20;
				VectorCopy(o->Target->EyeLeft3, o->Position);
			}
			else if (o->SubType == 27 || o->SubType == 33)
			{
				o->MaxTails = 20;
				VectorCopy(o->Target->EyeRight3, o->Position);
			}
			else
			{
				VectorCopy(o->Target->EyeLeft, o->Position);
			}
			o->TexType = BITMAP_JOINT_ENERGY;
			if ((o->SubType >= 28 && o->SubType <= 33)
				)
			{
				o->TexType = BITMAP_FLARE;
			}
			break;
		case 22:
			o->Velocity = 0.0f;
			o->LifeTime = 999999999;
			o->MaxTails = 10;
			VectorCopy(o->Target->EyeLeft, o->Position);
			break;
		case 23:
			o->Velocity = 0.0f;
			o->LifeTime = 999999999;
			o->MaxTails = 10;
			VectorCopy(o->Target->EyeRight, o->Position);
			break;
		case 24:
			o->Velocity = 0.0f;
			o->LifeTime = 999999999;
			o->MaxTails = 10;
			VectorCopy(o->Target->EyeLeft, o->Position);
			break;
		case 25:
			o->Velocity = 0.0f;
			o->LifeTime = 999999999;
			o->MaxTails = 10;
			VectorCopy(o->Target->EyeRight, o->Position);
			break;
		case 40:
		case 41:
			o->Velocity = 0.0f;
			o->LifeTime = 999999999;
			if (o->SubType == 5) o->MaxTails = 10;
			VectorCopy(TargetPosition, o->Position);
			break;
		case 42:
			{
				o->LifeTime = 100;
				o->MaxTails = 6;
				o->Velocity = 3.0f;
			}
			break;
		case 43:
			{
				o->LifeTime = 100;
				o->MaxTails = 6;
				o->Velocity = 3.0f;
				o->Scale = Scale;
			}
			break;
			
		case 48:	
		case 49:	
		case 50:	
		case 51:	
		case 52:	
		case 53:	
			o->LifeTime = 999999999;
			o->MaxTails = 3;
			Vector(0.5f, 0.5f, 0.9f, o->Light);
			break;
		case 54:
			o->Velocity = 0.0f;
			o->LifeTime = 999999999;
			o->MaxTails = 30;
			o->TexType = BITMAP_SPARK + 1;
			switch (o->PKKey)
			{
			case 0:
				VectorCopy(o->Target->EyeRight2, o->Position);
				break;
			case 1:
				VectorCopy(o->Target->EyeLeft2, o->Position);
				break;
			case 2:
				VectorCopy(o->Target->EyeRight3, o->Position);
				break;
			case 3:
				VectorCopy(o->Target->EyeLeft3, o->Position);
				break;
			}
			break;
		case 55:	
		case 56:	
			{
				o->Velocity = 0.0f;
				o->MaxTails = 8;
				o->LifeTime = 10;
				o->TexType = BITMAP_JOINT_ENERGY;

				switch (o->SubType)
				{
				case 55: VectorCopy(o->Target->EyeLeft, o->Position); break; 
				case 56: VectorCopy(o->Target->EyeRight, o->Position); break; 
				}
			}
			break;
		case 57:	
			{
				o->Velocity = 0.0f;
				o->MaxTails = iChaIndex;
				o->LifeTime = 10;
				o->TexType = BITMAP_JOINT_ENERGY;
			}
			break;
		}

		if (!vPriorColor)
		{
			switch (o->SubType)
			{
			case 0:Vector(0.4f, 0.3f, 0.2f, o->Light); break;
			case 1:Vector(0.1f, 0.1f, 0.5f, o->Light); break;
			case 2:
			case 3:Vector(0.5f, 0.1f, 1.0f, o->Light); break;
			case 4:Vector(0.3f, 0.15f, 0.1f, o->Light); break;
			case 5:Vector(0.5f, 0.1f, 1.0f, o->Light); break;
			case 6:Vector(0.4f, 0.3f, 0.2f, o->Light); break;
			case 8:Vector(1.0f, 0.0f, 0.5f, o->Light); break;
			case 10:
			case 11:Vector(1.0f, 0.3f, 0.1f, o->Light); break;
			case 12:
			case 13:Vector(0.7f, 0.3f, 1.0f, o->Light); break;
			case 14:
			case 15:Vector(1.0f, 0.1f, 0.1f, o->Light); break;
			case 16:Vector(0.4f, 0.2f, 0.4f, o->Light); break;
			case 17:Vector(0.8f, 0.2f, 1.0f, o->Light); break;
			case 18:
			case 19:
			case 20:
			case 21:
			case 26:
			case 27: Vector(0.8f, 0.5f, 1.0f, o->Light); break;
			case 42: Vector(0.0f, 0.0f, 0.0f, o->Light); break;
			case 43: Vector(2.5f, 0.0f, 0.0f, o->Light); break;	
			case 46: Vector(0.1f, 0.25f, 0.1f, o->Light); break;	
			case 47: Vector(0.9f, 0.0f, 0.0f, o->Light); break;	
			}
		}
		break;

	case BITMAP_JOINT_HEALING:
		o->LifeTime = 12;
		o->Scale = Scale;
		o->MaxTails = 2;
		o->Velocity = 0.0f;

		if (o->SubType == 4)
		{
			o->LifeTime = 30;
			Vector((mu_float)(GetLargeRand(64) - 32), -10.0f, 0.0f, o->TargetPosition);
		}
		else if (o->SubType == 5) 
		{
			o->Scale += GetLargeRand(10) - 5;
			o->Velocity = (mu_float)(GetLargeRand(20) + 6);
			o->LifeTime = GetLargeRand(8) + 8;
			o->MaxTails = 8;
			Vector(1.0f, 1.0f, 1.0f, o->Light);
		}
		else if (o->SubType == 6)
		{
			o->MaxTails = 4;
			Vector(1.0f, 1.0f, 0.5f, o->Light);
		}
		else if (o->SubType == 7)
		{
			o->MaxTails = 4;
			Vector(1.0f, 1.0f, 0.0f, o->Light);
		}
		else if (o->SubType == 8)
		{
			o->LifeTime = 17;
			o->MaxTails = 3;
			o->Light[0] = 0.5f;
			o->Light[1] = 0.5f;
			o->Light[2] = (GetLargeRand(128)) / 255.0f + 0.5f;
			VectorCopy(TargetPosition, o->TargetPosition);
			o->TargetPosition[2] += 100.0f;
		}
		else if (o->SubType == 9)
		{
			o->LifeTime = 90;
			o->MaxTails = 20;
			o->NumTails = 0;
			o->MultiUse = (mu_int32)o->Angle[2];
			o->Velocity = 0.0f;
			o->Direction[0] = 50.0f;
			o->Light[0] = 1.0f / (11.0f);
			o->Light[1] = 0.5f / (11.0f);
			o->Light[2] = 1.0f / (11.0f);
			VectorCopy(o->Target->Position, o->TargetPosition);
			VectorCopy(o->TargetPosition, o->Position);
		}
		else if (o->SubType == 10)
		{
			o->LifeTime = 80;
			o->MaxTails = 20;
			o->NumTails = 0;
			o->MultiUse = (mu_int32)o->Angle[2];
			o->Velocity = 0.0f;
			o->Direction[0] = 80.0f;
			if (o->MultiUse == 225 || o->MultiUse == 405)
			{
				o->Features.Set(JOINT::eJFS_COLLISION);
			}
			o->Light[0] = 1.0f / (11.0f);
			o->Light[1] = 0.5f / (11.0f);
			o->Light[2] = 1.0f / (11.0f);
			VectorCopy(o->Target->Position, o->TargetPosition);
			VectorCopy(o->TargetPosition, o->Position);
		}
		else if (o->SubType == 13)
		{
			o->LifeTime = 17;
			o->MaxTails = 10;
			o->Light[0] = 1.0f;
			o->Light[1] = 0.3f;
			o->Light[2] = 0.2f;
			VectorCopy(TargetPosition, o->TargetPosition);
			o->TargetPosition[2] += 200.0f;
		}
		else if (o->SubType == 14)
		{
			o->LifeTime = 10;
			o->MaxTails = 10;
			o->Light[0] = 0.8f;
			o->Light[1] = 1.0f;
			o->Light[2] = 0.8f;
			VectorCopy(TargetPosition, o->TargetPosition);
		}
		else if (o->SubType == 15)
		{
			o->LifeTime = 10;
			o->Scale = Scale;
			o->MaxTails = 2;
			o->Velocity = 0.0f;
		}
		else if (o->SubType == 16)
		{
			o->LifeTime = 10;
			o->Scale = Scale;
			o->MaxTails = 2;
			o->Velocity = 0.0f;
			o->RenderType = BLEND::TXB_BLENDMINUS;
		}
		else if (o->SubType == 17)
		{
			o->LifeTime = 17;
			o->MaxTails = 3;
			o->Light[0] = (GetLargeRand(128)) / 255.0f + 0.6f;
			o->Light[1] = 0.1f;
			o->Light[2] = 0.0f;
			VectorCopy(TargetPosition, o->TargetPosition);
			o->TargetPosition[2] += 100.0f;
		}
		break;

		
	case BITMAP_2LINE_GHOST:
		{
			if (o->SubType == 0)
			{
				o->RenderType = BLEND::TXB_BLENDMINUS;
				o->Velocity = 40.0f + GetLargeRand(20);
				if (GetLargeRand(2) == 0)
					o->LifeTime = 67;
				else
					o->LifeTime = 75;
				o->MaxTails = 26;
				o->Scale = Scale + (GetLargeRand(200) + 1);
				o->Direction[0] = 0;
				o->m_sTargetIndex = 2;
				
				if (GetLargeRand(3) < 2)
				{
					Vector(0.5f, 0.5f, 0.5f, o->Light);
					CreateEffect(ThreadIndex, MODEL_DESAIR, o->Position, o->Angle, o->Light, 0, nullptr, -1, 0, 0, 0, 1.0f, i);
				}
			}
			else if (o->SubType == 1)		
			{
				o->RenderType = BLEND::TXB_BLEND01;
				o->Scale = Scale;
				o->LifeTime = 25 + GetLargeRand(10);
				o->MaxTails = 15;
				o->Velocity = 20.0f + GetLargeRand(10);
				VectorCopy(o->Position, o->TargetPosition);

				EVector3 vAngle, vDir, vRandPos;
				Vector(0.0f, 0.0f, (mu_float)(GetLargeRand(360)), vAngle);
				AngleMatrix(vAngle, Matrix);
				Vector(0.0f, 300.0f, 0.0f, vDir);
				VectorRotate(vDir, Matrix, vRandPos);

				o->Position[0] += vRandPos[0];
				o->Position[1] += vRandPos[1];
				o->Position[2] = (GetLargeRand(100)) + 200;
				o->TargetPosition[2] = o->Position[2] - (GetLargeRand(100) - 100) * (GetLargeRand(2) == 0 ? 1 : -1);

				VectorSubtract(o->TargetPosition, o->Position, o->Direction);

				
				o->Angle[2] = CreateAngle(o->Position[0], o->Position[1], o->TargetPosition[0], o->TargetPosition[1]);
			}
		}
		break;
	case BITMAP_JOINT_SPIRIT:
	case BITMAP_JOINT_SPIRIT2:
		o->RenderType = BLEND::TXB_BLENDMINUS;
		switch (o->SubType)
		{
		case 0:
			o->Weapon = 0;
			o->Velocity = 70.0f;
			o->LifeTime = 49;
			o->Scale = Scale;
			o->MaxTails = 6;
			break;
		case 1:
			o->Velocity = 70.0f;
			o->LifeTime = 49;
			o->Scale = Scale;
			o->MaxTails = 6;
			PlayBackground(SOUND_BRANDISH_SWORD03);
			break;
		case 2:
		case 21:
			o->RenderType = BLEND::TXB_BLEND01;
			o->Features.Clear(JOINT::eJFS_FACEONE);
			o->Velocity = 50.0f;
			o->LifeTime = 20;
			o->Scale = Scale;
			o->MaxTails = 3;
			Vector(0.5f, 0.5f, 0.5f, o->Light);
			VectorCopy(o->Position, o->StartPosition);
			
			break;
		case 3:
			o->Velocity = 140.0f;
			o->LifeTime = 49;
			o->Scale = Scale;
			o->MaxTails = 10;
			if (o->Type == BITMAP_JOINT_SPIRIT2)
			{
				Vector(1.0f, 1.0f, 1.0f, o->Light);
			}
			else
			{
				o->RenderType = BLEND::TXB_BLEND01;
				Vector(1.0f, 0.5f, 0.1f, o->Light);
			}
			break;
		case 4:
			{
				o->RenderType = BLEND::TXB_BLEND01;
				o->LifeTime = 0;
				o->Scale = Scale;
				o->MaxTails = 10;
				Vector(0.0f, 0.0f, 0.0f, o->Angle);
				Vector(0.3f, 0.3f, 1.0f, o->Light);
				o->StartPosition[0] = (TargetPosition[0] - o->Position[0]) / o->MaxTails;
				o->StartPosition[1] = (TargetPosition[1] - o->Position[1]) / o->MaxTails;
				o->StartPosition[2] = (TargetPosition[2] - o->Position[2]) / o->MaxTails;
				VectorCopy(o->StartPosition, Position);

				AngleMatrix(o->Angle, Matrix);
				for (mu_int32 i = 0; i < (o->MaxTails - 1); ++i)
				{
					if (o->Target == nullptr)
					{
						Position[0] = o->StartPosition[0];
						Position[1] = o->StartPosition[1];
					}
					else
					{
						Position[0] = o->StartPosition[0];
						Position[1] = o->StartPosition[1];
					}
					VectorAdd(o->Position, Position, o->Position);
					CreateTail(o, Matrix);
					o->Position[0] -= Position[0];
					o->Position[0] += o->StartPosition[0];
					o->Position[1] -= Position[1];
					o->Position[1] += o->StartPosition[1];
				}
				VectorCopy(o->TargetPosition, o->Position);
			}
			break;
		case 5:
			o->RenderType = BLEND::TXB_BLEND01;
			o->Weapon = 0;
			o->Velocity = 30.0f;
			o->LifeTime = 49;
			o->Scale = Scale;
			o->MaxTails = 12;
			break;
		case 6:
			o->RenderType = BLEND::TXB_BLEND01;
			o->Features.Clear(JOINT::eJFS_FACEONE);
			o->Velocity = 50.0f;
			o->LifeTime = 20;
			o->Scale = Scale;
			o->MaxTails = 5;
			o->PKKey = GetLargeRand(5);

			if (o->PKKey)
			{
				o->Features.Clear(JOINT::eJFS_CREATETAILS);
			}

			switch (o->Skill)
			{
			case 0:
				Vector(0.3f, 0.3f, 1.0f, o->Light);
				break;
			case 1:
				o->Velocity = 10.0f;
				Vector(0.5f, 0.5f, 0.5f, o->Light);
				break;
			}
			VectorCopy(o->Position, o->StartPosition);
			
			break;
		case 7:
			o->Features.Clear(JOINT::eJFS_FACEONE | JOINT::eJFS_FACETWO);
			o->Velocity = 10.0f;
			o->LifeTime = 20;
			o->Scale = Scale;
			o->MaxTails = 5;
			switch (o->Skill)
			{
			case 0:
				Vector(0.3f, 0.3f, 1.0f, o->Light);
				break;
			case 1:
				o->Velocity = 10.0f;
				Vector(0.5f, 0.5f, 0.5f, o->Light);
				break;
			}
			VectorCopy(o->Position, o->StartPosition);
			
			break;
		case 8:
			o->LifeTime = 49;
			o->Scale = Scale;
			o->MaxTails = 15;
			break;
		case 9:
			o->Features.Clear(JOINT::eJFS_FACEONE);
			o->Velocity = 10.0f;
			o->LifeTime = 20;
			o->Scale = Scale;
			o->MaxTails = 15;
			Vector(1.0f, 1.0f, 1.0f, o->Light);
			Vector(-90.0f, 0.0f, 0.0f, o->Angle);
			Vector(0.0f, 0.0f, 0.0f, o->Direction);
			VectorCopy(o->Position, o->StartPosition);
			break;
		case 10:
			o->LifeTime = 20;
			o->Scale = Scale;
			o->MaxTails = 6;
			break;
		case 11:
			o->Velocity = 0;
			o->LifeTime = 50;
			o->Scale = Scale;
			o->MaxTails = 1;
			break;
		case 12:
			o->Velocity = 0;
			o->LifeTime = 30;
			o->Scale = Scale;
			o->MaxTails = 1;
			break;
		case 13:
			if (o->Type == BITMAP_JOINT_SPIRIT)
			{
				o->RenderType = BLEND::TXB_BLEND01;
			}
			o->Velocity = 40.0f;
			o->LifeTime = 20;
			o->Scale = Scale;
			o->MaxTails = 10;
			Vector(1.0f, 0.6f, 0.4f, o->Light);
			break;
		case 14:
			o->Velocity = 0;
			o->LifeTime = 10;
			o->Scale = Scale;
			o->MaxTails = 1;
			o->Angle[0] = 20;
			break;
		case 15:
			o->Velocity = 0;
			o->LifeTime = 20;
			o->Scale = Scale;
			o->MaxTails = 1;
			break;
		case 16:
			o->Velocity = 0;
			o->LifeTime = 50;
			o->Scale = Scale;
			o->MaxTails = 1;
			break;
		case 17:
			o->Velocity = 0;
			o->LifeTime = 100;
			o->Scale = Scale;
			o->MaxTails = 1;
			break;
		case 18:
			if (o->Type == BITMAP_JOINT_SPIRIT2)
			{
				o->RenderType = BLEND::TXB_BLEND01;
			}
			o->Velocity = 50.0f;
			o->LifeTime = 39;
			o->Scale = Scale;
			o->MaxTails = 15;
			Vector(0.7f, 0.7f, 0.7f, o->Light);
			break;
		case 19:
			if (o->Type == BITMAP_JOINT_SPIRIT)
			{
				o->RenderType = BLEND::TXB_BLEND01;
			}
			o->Velocity = 70.0f;
			o->LifeTime = 49;
			o->Scale = Scale;
			o->MaxTails = 9;
			Vector(0.1f, 0.5f, 0.2f, o->Light);
			break;
		case 20:
			if (o->Type == BITMAP_JOINT_SPIRIT2)
			{
				o->RenderType = BLEND::TXB_BLEND01;
			}
			o->LifeTime = 49;
			o->Scale = Scale;
			o->MaxTails = 15;
			break;
		case 22:
			o->RenderType = BLEND::TXB_BLEND01;
			o->Features.Clear(JOINT::eJFS_FACEONE);
			o->Velocity = 60.0f;
			o->LifeTime = 20;
			o->Scale = Scale;
			o->MaxTails = 0;
			VectorCopy(o->Position, o->StartPosition);
			break;
		case 23:
			o->Features.Clear(JOINT::eJFS_FACEONE | JOINT::eJFS_FACETWO);
			o->Velocity = 10.0f;
			o->LifeTime = 20;
			o->Scale = Scale;
			o->MaxTails = 0;
			VectorCopy(o->Position, o->StartPosition);
			break;
		case 24:	
			{
				o->RenderType = BLEND::TXB_BLEND01;
				o->Features.Clear(JOINT::eJFS_FACEONE);
				o->Velocity = 10.0f;
				o->LifeTime = 160;
				o->Scale = Scale;
				o->MaxTails = 40;
				Vector(0.0f, 0.0f, 0.0f, o->Light);

				o->Position[0] += (mu_float)(GetLargeRand(300) - 150);
				o->Position[1] += (mu_float)(GetLargeRand(300) - 150);

				
				mu_float fRargleScale = o->Scale / 70.0f;	
				if (0.9f <= fRargleScale)	
				{
					EVector3 vLight;
					Vector(0.0f, 0.0f, 0.1f, vLight);
					CreateEffect(ThreadIndex, MODEL_SUMMONER_SUMMON_LAGUL, o->Position, o->Angle,
						vLight, 1, (OBJECT*)o, -1, 0, 0, 0, fRargleScale);
				}
			}
			break;
		case 25:
			{
				o->RenderType = BLEND::TXB_BLEND01;
				o->TexType = BITMAP_SHINY;
				o->Velocity = 9.0f;
				o->LifeTime = 26;
				o->Scale = Scale;
				o->MaxTails = 30;
				o->Skill = SkillIndex;
				Vector(0.9f, 0.8f, 1.0f, o->Light);
				Vector(-90.0f, 0.0f, 0.0f, o->Angle);
				Vector(0.0f, 0.0f, 0.0f, o->Direction);
				VectorCopy(o->Position, o->StartPosition);
			}
			break;
		}
		break;
	case BITMAP_JOINT_LASER:
		o->Features.Set(JOINT::eJFS_TILEMAPPING);
		o->Velocity = 70.0f;
		o->LifeTime = 49;
		o->Scale = Scale;
		o->MaxTails = 6;
		break;
	case BITMAP_JOINT_SPARK:
		switch (o->SubType)
		{
		case 0:
			o->Scale = 2.0f;
			o->Velocity = (mu_float)(GetLargeRand(20) + 6);
			o->LifeTime = GetLargeRand(8) + 8;
			o->MaxTails = 2;
			Vector(1.0f, 1.0f, 1.0f, o->Light);
			break;
		case 1:
			o->Scale = 2.0f;
			o->Velocity = (mu_float)(GetLargeRand(20) + 16);
			o->LifeTime = GetLargeRand(4) + 4;
			o->MaxTails = 2;
			Vector(1.0f, 1.0f, 1.0f, o->Light);
			break;
		case 2:
			o->Scale = 4.0f;
			o->Velocity = 30.0f;
			o->LifeTime = GetLargeRand(4) + 4;
			o->MaxTails = 2;
			Vector(0.3f, 0.3f, 1.0f, o->Light);
			break;
		case 3:
			o->Scale = 2.0f;
			o->Velocity = (mu_float)(GetLargeRand(20) + 6);
			o->LifeTime = GetLargeRand(8) + 8;
			o->MaxTails = 2;
			Vector(1.0f, 1.0f, 1.0f, o->Light);
			break;
		case 4:
			o->Scale = Scale * 2 + 4.0f;
			o->Velocity = (mu_float)(GetLargeRand(5) + 4);
			o->LifeTime = GetLargeRand(10) + 8;
			o->MaxTails = 4;
			Vector(1.0f, 0.2f, 0.2f, o->Light);
			break;
		case 5:
			o->Scale = 2.0f;
			o->Velocity = (mu_float)(GetLargeRand(20) + 6);
			o->LifeTime = GetLargeRand(8) + 8;
			o->MaxTails = 2;
			Vector(0.7f, 0.1f, 0.1f, o->Light);
			break;
		}
		break;

	case BITMAP_SMOKE:
		o->Scale = Scale;
		Vector(1.0f, 1.0f, 1.0f, o->Light);
		if (o->SubType == 0)
		{
			o->MaxTails = 20;
			o->LifeTime = 20;
			o->Velocity = 0.0f;
			o->TexType = BITMAP_FLARE;
		}
		else if (o->SubType == 1)
		{
			o->MaxTails = 15;
			o->LifeTime = 15;
			o->Velocity = 0.0f;
		}
		else if (o->SubType == 2)
		{
			o->MaxTails = 25;
			o->LifeTime = 22;
			o->Velocity = 0.0f;
			o->TexType = BITMAP_FLARE;
			Vector(0.1f, 0.3f, 1.0f, o->Light);
		}
		break;

	case MODEL_FENRIR_SKILL_THUNDER:
		o->Scale = Scale;
		o->MaxTails = MAX_TAILS;
		o->Velocity = 50.0f;
		o->LifeTime = 20;
		o->Features.Set(JOINT::eJFS_TILEMAPPING);

		if (o->SubType == 0)	
		{
			o->TexType = BITMAP_JOINT_THUNDER;
			Vector(0.7f, 1.0f, 0.7f, o->Light);
		}
		else if (o->SubType == 1)	
		{
			o->TexType = BITMAP_JOINT_THUNDER;
			Vector(1.0f, 0.6f, 0.6f, o->Light);
		}
		else if (o->SubType == 2)	
		{
			o->TexType = BITMAP_JOINT_THUNDER;
			Vector(0.7f, 0.7f, 1.0f, o->Light);
		}
		else if (o->SubType == 3)	
		{
			o->TexType = BITMAP_JOINT_THUNDER;
			Vector(0.9f, 0.9f, 0.3f, o->Light);
		}
		else if (o->SubType == 4)	
		{
			o->TexType = BITMAP_FLASH;
			Vector(0.1f, 0.8f, 0.1f, o->Light);
		}
		else if (o->SubType == 5)	
		{
			o->TexType = BITMAP_FLASH;
			Vector(1.0f, 0.3f, 0.2f, o->Light);
		}
		else if (o->SubType == 6)	
		{
			o->TexType = BITMAP_FLASH;
			Vector(0.2f, 0.3f, 1.0f, o->Light);
		}
		else if (o->SubType == 7)	
		{
			o->TexType = BITMAP_FLASH;
			Vector(0.8f, 0.8f, 0.1f, o->Light);
		}
		break;

	case BITMAP_JOINT_LASER + 1:
		o->Features.Set(JOINT::eJFS_TILEMAPPING);
	case BITMAP_BLUR + 1:
		o->Scale = 60.0f;
		o->Velocity = 40.0f;
		o->MaxTails = MAX_TAILS;
		o->LifeTime = 2;
		if (o->SubType == 0)
		{
			Vector(1.0f, 1.0f, 1.0f, o->Light);
		}
		else if (o->SubType == 3)
		{
			o->LifeTime = 20;
			o->Velocity = 0.0f;
			Vector(1.0f, 1.0f, 1.0f, o->Light);
		}
		else
		{
			if (o->Type == BITMAP_JOINT_LASER + 1)
			{
				Vector(.35f, .1f, 1.0f, o->Light);
			}
			else
			{
				Vector(0.0f, 0.3f, 1.0f, o->Light);
			}
		}
		break;
		
	case BITMAP_JOINT_THUNDER:
		o->Scale = Scale;
		o->MaxTails = MAX_TAILS;
		o->Velocity = 50.0f;
		switch (o->SubType)
		{
		case 0:
		case 20:
			o->LifeTime = 5;
			break;

		case 1:
			o->Velocity = 20.0f + (mu_float)(GetLargeRand(10));
			o->LifeTime = GetLargeRand(8) + 8;
			break;

		case 2:
			{
				o->LifeTime = 2;
				Vector(1.0f, 0.1f, 0.0f, o->Light);
				Vector(0.0f, -150.0f, 0.0f, BitePosition);
				MUModel *b = &MODELS::Data[MODEL_PLAYER];
				b->TransformPosition(ThreadIndex, o->Target->BoneTransform[b->GetBoneIndex(33)], BitePosition, o->TargetPosition, true);
			}
			break;

		case 3:    
			o->Velocity = SkillIndex;
			o->PKKey = -1;
			o->LifeTime = PKKey;
			o->MaxTails = SkillIndex;
			Vector(0.5f, 0.5f, 1.0f, o->Light);
			VectorCopy(o->Position, o->StartPosition);
			break;

		case 4:    
			o->Velocity = 60.0f;
			o->LifeTime = 20;
			o->MaxTails = 10;
			Vector(0.0f, 0.0f, 0.0f, o->Direction);
			Vector(0.5f, 0.5f, 1.0f, o->Light);
			VectorCopy(TargetPosition, o->TargetPosition);
			VectorCopy(o->Position, o->StartPosition);
			break;

		case 5:    
			o->Velocity = -60.0f;
			o->LifeTime = 20;
			o->MaxTails = 10;
			Vector(0.0f, 0.0f, 0.0f, o->Direction);
			Vector(0.5f, 0.5f, 1.0f, o->Light);
			VectorCopy(TargetPosition, o->TargetPosition);
			VectorCopy(o->Position, o->StartPosition);
			o->TargetPosition[2] += 600.0f;
			break;

		case 6:    
			o->LifeTime = GetLargeRand(20) + 6;
			o->Features.Clear(JOINT::eJFS_CREATETAILS);
			o->Velocity = 15.0f + (mu_float)(GetLargeRand(10));

			o->Light[0] = (GetLargeRand(10)) / 15.0f + 0.1f;
			o->Light[1] = o->Light[0];
			o->Light[2] = o->Light[0];
			VectorCopy(o->Position, o->StartPosition);
			break;

		case 7:    
			o->Features.Set(JOINT::eJFS_TILEMAPPING);
			o->Velocity = 5.0f;
			o->MaxTails = 3;
			o->LifeTime = 1;
			Vector(0.5f, 0.5f, 1.0f, o->Light);
			VectorCopy(o->Position, o->StartPosition);

			MoveHumming(o->Position, o->Angle, o->TargetPosition, 360.0f);
			break;

		case 8:    
			o->Scale = 5.0f;
			o->Velocity = (mu_float)(GetLargeRand(10) + 6);
			o->LifeTime = GetLargeRand(8) + 8;
			o->MaxTails = 2;
			Vector(1.0f, 1.0f, 1.0f, o->Light);
			break;

		case 9:    
			{
				o->Velocity = 80.0f + (mu_float)(GetLargeRand(20));
				o->LifeTime = 30;

				o->Light[0] = (GetLargeRand(10)) / 15.0f + 0.1f;
				o->Light[1] = o->Light[0];
				o->Light[2] = o->Light[0];
				VectorCopy(o->Position, o->StartPosition);
			}
			break;

		case 10:   
			{
				o->Features.Set(JOINT::eJFS_TILEMAPPING);
				o->LifeTime = 0;
				o->Scale = Scale;
				o->MaxTails = 10;
				Vector(0.0f, 0.0f, 0.0f, o->Angle);
				Vector(0.3f, 0.3f, 1.0f, o->Light);
				o->StartPosition[0] = (TargetPosition[0] - o->Position[0]) / o->MaxTails;
				o->StartPosition[1] = (TargetPosition[1] - o->Position[1]) / o->MaxTails;
				o->StartPosition[2] = (TargetPosition[2] - o->Position[2]) / o->MaxTails;
				VectorCopy(o->StartPosition, Position);

				AngleMatrix(o->Angle, Matrix);
				for (mu_int32 i = 0; i < (o->MaxTails - 1); ++i)
				{
					if (o->Target == nullptr)
					{
						Position[0] = o->StartPosition[0] + GetLargeRand(8) - 4;
						Position[1] = o->StartPosition[1] + GetLargeRand(8) - 4;
					}
					else
					{
						Position[0] = o->StartPosition[0] + GetLargeRand(16) - 8;
						Position[1] = o->StartPosition[1] + GetLargeRand(16) - 8;
					}
					VectorAdd(o->Position, Position, o->Position);
					CreateTail(o, Matrix);
					o->Position[0] -= Position[0];
					o->Position[0] += o->StartPosition[0];
					o->Position[1] -= Position[1];
					o->Position[1] += o->StartPosition[1];
				}
				VectorCopy(o->TargetPosition, o->Position);
			}
			break;
		case 11:
			Vector(1.0f, 0.5f, 0.1f, o->Light);
			o->LifeTime = 2;
			break;
		case 12:
			Vector(1.0f, 0.1f, 0.1f, o->Light);
			o->LifeTime = 10;
			break;
		case 13:
			Vector(1.0f, 0.1f, 0.1f, o->Light);
			o->LifeTime = 2;
			o->SubType = 11;
			break;
		case 14:
			{
				o->LifeTime = 0;
				o->Scale = Scale;
				o->MaxTails = 10;
				Vector(0.0f, 0.0f, 0.0f, o->Angle);
				Vector(0.3f, 0.3f, 1.0f, o->Light);
				o->StartPosition[0] = (TargetPosition[0] - o->Position[0]) / o->MaxTails;
				o->StartPosition[1] = (TargetPosition[1] - o->Position[1]) / o->MaxTails;
				o->StartPosition[2] = (TargetPosition[2] - o->Position[2]) / o->MaxTails;
				VectorCopy(o->StartPosition, Position);

				AngleMatrix(o->Angle, Matrix);
				for (mu_int32 i = 0; i < (o->MaxTails - 1); ++i)
				{
					if (o->Target == nullptr)
					{
						Position[0] = o->StartPosition[0];
						Position[1] = o->StartPosition[1];
					}
					else
					{
						Position[0] = o->StartPosition[0];
						Position[1] = o->StartPosition[1];
					}
					VectorAdd(o->Position, Position, o->Position);
					CreateTail(o, Matrix);
					o->Position[0] -= Position[0];
					o->Position[0] += o->StartPosition[0];
					o->Position[1] -= Position[1];
					o->Position[1] += o->StartPosition[1];
				}
				VectorCopy(o->TargetPosition, o->Position);
				
			}
			break;
		case 15:
			o->Features.Clear(JOINT::eJFS_FACEONE | JOINT::eJFS_FACETWO);
			o->LifeTime = 80;
			o->MaxTails = 0;
			o->MultiUse = 0;
			for (mu_int32 j = 0; j < MAX_CHARACTERS_CLIENT; ++j)
			{
				CHARACTER *tc = &CharactersClient[j];
				OBJECT    *to = &tc->Object;
				if (to->Flags.Get(OBJECT::eOF_LIVE | OBJECT::eOF_VISIBLE) == false)
					continue;

				if (to->Kind != KIND_MONSTER ||
					tc->Dead != 0)
				{
					continue;
				}

				mu_float dx = o->Position[0] - to->Position[0];
				mu_float dy = o->Position[1] - to->Position[1];
				mu_float Distance = EMath::Sqrt(dx*dx + dy*dy);
				if (Distance <= 400 && to != Target)
				{
					o->TargetIndex[o->MultiUse++] = j;
				}

				if (o->MultiUse > 5) break;
			}
			o->Weapon = o->MultiUse * 15;
			o->MultiUse = 0;

			VectorCopy(TargetPosition, o->StartPosition);
			o->StartPosition[2] += 150.0f;
			break;
		case 16:
			o->Velocity = 20.0f + (mu_float)(GetLargeRand(10));
			o->LifeTime = GetLargeRand(2) + 2;
			break;
		case 17:
			o->Velocity = 20.0f + (mu_float)(GetLargeRand(10));
			o->LifeTime = 10;
			break;
		case 18:
			o->Velocity = 80.0f;
			o->MaxTails = 5;
			o->LifeTime = 10;
			o->Light[0] = o->Light[1] = o->Light[2] = 1.0f;
			VectorCopy(o->Position, o->StartPosition);
			break;
		case 19:
			{
				o->MaxTails = 30;
				o->LifeTime = 2;
				o->Velocity = o->Scale*3.0f;
				VectorCopy(o->Position, o->StartPosition);
			}
			break;
		case 21:
			{
				o->LifeTime = 2;
				Vector(1.0f, 0.5f, 0.4f, o->Light);
				Vector(0.0f, -150.0f, 0.0f, BitePosition);
				MUModel *b = &MODELS::Data[MODEL_PLAYER];
				b->TransformPosition(ThreadIndex, o->Target->BoneTransform[b->GetBoneIndex(33)], BitePosition, o->TargetPosition, true);
			}
			break;
			
		case 22:	
		case 23:
		case 24:
			o->LifeTime = 15;
			o->Scale = Scale + (GetLargeRand(50) + 50)*0.1f;
			o->MaxTails = 30;
			o->Velocity = 20.0f;
			break;
		case 25:		
			{
				o->LifeTime = 20;
				o->MaxTails = 8;
				o->Velocity = 20.0f + (mu_float)(GetLargeRand(10));
			}
			break;
		case 26:
			o->Velocity = 20.0f + (mu_float)(GetLargeRand(10));
			o->LifeTime = GetLargeRand(8) + 8;
			break;
		case 27:
		case 28:
			{
				o->LifeTime = 2;
				VectorCopy(*vPriorColor, o->Light);
			}
			break;
		case 33:			
			Vector(0.3f, 0.3f, 1.0f, o->Light);
			o->Velocity = 20.0f + (mu_float)(GetLargeRand(10));
			o->LifeTime = GetLargeRand(2) + 2;
			break;
			}
		break;

	case BITMAP_JOINT_THUNDER + 1:
		o->Scale = Scale;
		o->TexType = BITMAP_JOINT_THUNDER;

		if (o->SubType == 0)
		{
			o->MaxTails = MAX_TAILS;

			o->Velocity = 80.0f + (mu_float)(GetLargeRand(20));
			o->LifeTime = 20;

			o->Light[2] = (mu_float)o->MaxTails;
			VectorCopy(o->Position, o->StartPosition);
		}
		else if (o->SubType == 1 || o->SubType == 2 || o->SubType == 3 || o->SubType == 5 || o->SubType == 6 || o->SubType == 7) 
		{
			o->MaxTails = MAX_TAILS;
			o->LifeTime = 20;
			o->Position[0] += (mu_float)(GetLargeRand(10) - 5);
			o->Position[1] += GetLargeRand(10) - 5;

			if (o->SubType == 7)
			{
				o->Features.Clear(JOINT::eJFS_CREATETAILS);
				o->Position[2] += 1050.0f;
			}
			else
			{
				o->Position[2] += 800.0f;
			}
			Vector(0.0f, 0.0f, 0.0f, o->Angle);
			VectorCopy(TargetPosition, o->TargetPosition);

			if (o->SubType == 1)
			{
				o->Features.Clear(JOINT::eJFS_CREATETAILS);
			}
			VectorCopy(o->Position, o->StartPosition);

			if (o->SubType == 5)
			{
				Vector(1.0f, 0.5f, 0.2f, o->Light);
			}
			else if (o->SubType == 6)
			{
				o->Velocity = 0.3f;
				Vector(1.0f, 1.0f, 1.0f, o->Light);
				o->Position[2] += 700.0f;
			}
		}
		else if (o->SubType == 4)   
		{
			o->MaxTails = MAX_TAILS;
			o->LifeTime = 20;
			o->Features.Clear(JOINT::eJFS_CREATETAILS);
			o->TargetIndex[0] = (mu_int32)(o->MaxTails / 1.5f);        
			o->TargetIndex[1] = o->MaxTails - 1;
			o->StartPosition[0] = (mu_float)(1.0f / o->MaxTails);   
			o->StartPosition[1] = (mu_float)(1.0f / (o->MaxTails - o->TargetIndex[0] - 1));
			Vector(0.0f, 0.0f, 0.0f, o->Angle);
			Vector(1.0f, 0.8f, 1.0f, o->Light);
			VectorCopy(TargetPosition, o->TargetPosition);
			o->TargetPosition[2] += 100.0f;
			Angle[2] = CreateAngle(o->Position[0], o->Position[1], o->TargetPosition[0], o->TargetPosition[1]);
		}
		else if (o->SubType == 8)
		{
			o->MaxTails = MAX_TAILS;
			o->LifeTime = 20;
			o->Features.Clear(JOINT::eJFS_CREATETAILS);

			o->Position[0] += (mu_float)(GetLargeRand(10) - 5);
			o->Position[1] += GetLargeRand(10) - 5;
			o->Position[2] += 1100.0f;

			Vector(0.0f, 0.0f, 0.0f, o->Angle);
			VectorCopy(TargetPosition, o->TargetPosition);
			VectorCopy(o->Position, o->StartPosition);
		}
		else if (o->SubType == 9)
		{
			o->MaxTails = MAX_TAILS;
			o->LifeTime = 20;
			o->Features.Clear(JOINT::eJFS_CREATETAILS);

			o->Position[0] -= 50.0f;

			Vector(0.0f, 0.0f, 0.0f, o->Angle);
			VectorCopy(TargetPosition, o->TargetPosition);
			VectorCopy(o->Position, o->StartPosition);
		}
		else if (o->SubType == 10)
		{
			o->MaxTails = MAX_TAILS;
			o->LifeTime = 20;
			o->Features.Clear(JOINT::eJFS_CREATETAILS);

			o->Position[1] -= 350.0f;

			Vector(0.0f, 0.0f, 0.0f, o->Angle);
			VectorCopy(TargetPosition, o->TargetPosition);
			VectorCopy(o->Position, o->StartPosition);
		}
		else if (o->SubType == 11)
		{
			o->Velocity = 80.0f + (mu_float)(GetLargeRand(20));
			o->Light[2] += 0.2f;
			o->LifeTime = 5;
			o->MaxTails = 30;
			VectorCopy(o->Position, o->StartPosition);
			VectorCopy(TargetPosition, o->TargetPosition);
		}
		else if (o->SubType == 12)
		{
			o->Velocity = 80.0f + (mu_float)(GetLargeRand(20));
			o->Light[2] = (mu_float)o->MaxTails;
			o->MaxTails = 30;
			o->LifeTime = 4;
			o->Features.Clear(JOINT::eJFS_CREATETAILS);
			VectorCopy(o->Position, o->StartPosition);

			EVector3 p1, p2;
			Vector(0.0f, o->Scale / 1.0f, 0.0f, p1);
			AngleMatrix(o->Angle, Matrix);
			VectorRotate(p1, Matrix, p2);

			Vector(p2[0] * o->Scale * 0.5f, p2[1] * o->Scale * 0.5f, p2[2] * o->Scale * 0.5f, p2);
			VectorAdd(o->Position, p2, o->Position);
			VectorCopy(o->Position, o->StartPosition);

			mu_int32 iScale = 1;
			iScale = (mu_int32)(o->Scale / 4.0f);
			o->TargetPosition[0] = TargetPosition[0] + GetLargeRand(iScale * 2) - iScale;
			o->TargetPosition[1] = TargetPosition[1] + GetLargeRand(iScale * 2) - iScale;
			o->TargetPosition[2] = TargetPosition[2] + GetLargeRand(iScale * 2) - iScale;
		}
		break;
	case BITMAP_JOINT_FIRE:
		o->Scale = 70.0f;
		o->Velocity = 50.0f;
		o->MaxTails = 8;
		o->LifeTime = 20;
		Vector(0.0f, 0.0f, 130.0f, BitePosition);
		VectorAdd(o->TargetPosition, BitePosition, o->TargetPosition);
		break;
	case BITMAP_SPARK + 1:
		o->LifeTime = 100;
		o->MaxTails = 20;
		o->Scale = 10.0f;
		if (o->SubType == 0)
		{
			o->Direction[2] = (mu_float)(GetLargeRand(20) + 35);
			o->Scale = GetLargeRand(20) + 20.0f;
			o->LifeTime = 25;
		}
		else if (o->SubType == 1)
		{
			o->Scale = Scale;
			o->Velocity = (mu_float)(GetLargeRand(55) + 14);
			o->LifeTime = GetLargeRand(10) + 8;
			o->MaxTails = 4;
		}
		Vector(1.0f, 1.0f, 1.0f, o->Light);
		VectorCopy(o->Position, o->TargetPosition);
		break;
	case MODEL_SPEARSKILL:	
		VectorCopy(o->Target->Position, o->TargetPosition);
		switch (o->SubType)
		{
		case 0:
			Vector(1.0f, 1.0f, 1.0f, o->Light);
			o->LifeTime = 999999;
			o->MaxTails = 30;
			o->TexType = BITMAP_FLARE_BLUE;
			break;
		case 4:
		case 9:	
			Vector(.4f, .8f, .2f, o->Light);
			o->LifeTime = 10000;
			o->MaxTails = 30;
			o->TexType = BITMAP_FLARE_BLUE;
			break;
		case 10:
		case 11:
			Vector(1.0f, 0.6f, 0.6f, o->Light);
			o->LifeTime = 10000;
			o->MaxTails = 30;
			o->TexType = BITMAP_LUCKY_SEAL_EFFECT;
			break;
		case 1:
			Vector(.2f, .2f, .2f, o->Light);
			o->LifeTime = 999999;
			o->MaxTails = 30;
			o->TexType = BITMAP_JOINT_SPIRIT;
			break;
		case 2:
			VectorCopy(TargetPosition, o->TargetPosition);
			Vector(1.0f, .3f, .3f, o->Light);
			o->LifeTime = 20;
			o->MaxTails = 5;
			o->TexType = BITMAP_FLARE_FORCE;
			break;
		case 3:
			if (o->Target != nullptr)
			{
				VectorCopy(o->Target->Light, o->Light);
			}
			else
			{
				Vector(0.5f, 0.0f, 0.0f, o->Light);
			}
			Vector(-90.0f, 0.0f, 0.0f, o->Angle);
			Vector(0.0f, (mu_float)(GetLargeRand(500)), 0.0f, o->Direction);
			o->Velocity = GetLargeRand(5) + 5.0f;
			o->LifeTime = 999999;
			o->MaxTails = 30;
			o->TexType = BITMAP_JOINT_SPIRIT;
			break;
		case 5:     
		case 6:     
		case 7:     
			o->Features.Clear(JOINT::eJFS_FACETWO);
			o->LifeTime = 60;
			o->MaxTails = 30;
			o->Weapon = 0;
			o->Features.Clear(JOINT::eJFS_CREATETAILS);

			if (o->SubType == 5)        
			{
				Vector(1.0f, 1.0f, 0.8f, o->Light);
				o->TexType = BITMAP_FLARE + 1;
			}
			else if (o->SubType == 6)   
			{
				Vector(1.0f, 0.8f, 1.0f, o->Light);
				o->TexType = BITMAP_FLARE + 1;
			}
			else if (o->SubType == 7)   
			{
				Vector(0.8f, 1.0f, 1.0f, o->Light);
				o->TexType = BITMAP_FLARE + 1;
			}
			Vector(0.0f, 800.0f, 0.0f, o->Direction);
			VectorCopy(TargetPosition, o->StartPosition);

			AngleMatrix(o->Angle, Matrix);
			VectorRotate(o->Direction, Matrix, Position);
			VectorAdd(o->StartPosition, Position, o->Position);

			o->NumTails = -1;
			CreateTail(o, Matrix);
			break;
			
		case 8:
			o->Features.Clear(JOINT::eJFS_FACETWO);
			o->LifeTime = 40;
			o->MaxTails = 30;
			o->Features.Clear(JOINT::eJFS_CREATETAILS);

			Vector(0.5f, 0.5f, 0.5f, o->Light);
			o->TexType = BITMAP_LIGHT;
			Vector(0.0f, -40.0f, 0.0f, o->Direction);
			VectorCopy(TargetPosition, o->StartPosition);

			AngleMatrix(o->Angle, Matrix);
			VectorRotate(o->Direction, Matrix, Position);
			VectorAdd(o->StartPosition, Position, o->Position);
			break;
		case 14:	
			VectorCopy(*vPriorColor, o->Light);
			o->LifeTime = 100;
			o->MaxTails = 30;
			o->TexType = BITMAP_LIGHT;
			break;
		case 15:	
			Vector(1.0f, 1.0f, 1.0f, o->Light);
			o->LifeTime = 100;
			o->MaxTails = 20;
			o->TexType = BITMAP_JOINT_SPIRIT;
			VectorCopy(o->Target->Owner->Position, o->StartPosition);
			break;
		case 16:
			Vector(1.0f, 1.0f, 0.0f, o->Light);
			o->LifeTime = 100;
			o->MaxTails = 30;
			o->TexType = BITMAP_LIGHT;
			break;
		case 17:	
			Vector(0.7f, 0.2f, 1.0f, o->Light);
			o->LifeTime = 100;
			o->MaxTails = 20;
			o->TexType = BITMAP_JOINT_SPIRIT;
			VectorCopy(o->Target->Owner->Position, o->StartPosition);
			break;
		}
		o->Scale = Scale;
		break;
	case BITMAP_FLARE:
	case BITMAP_FLARE_BLUE:
		o->LifeTime = 100;
		o->MaxTails = 20;
		o->Scale = 10.0f;
		if (o->SubType == 0 || o->SubType == 18)     
		{
			if (o->Type == BITMAP_FLARE && o->SubType == 18)
			{
				o->RenderType = BLEND::TXB_BLEND02;
			}

			o->Scale = Scale;
			o->Direction[2] = (GetLargeRand(150)) / 100.0f;
			o->Direction[1] = (mu_float)(GetLargeRand(500) - 250);
			o->Velocity = 40.0f;

			if (o->Scale > 10.0f)
			{
				o->LifeTime = 50;
				o->Direction[2] = (GetLargeRand(250) + 200) / 100.0f;
			}

			VectorCopy(o->Target->Light, o->Light);
		}
		else if (o->SubType == 20)      
		{
			if (o->Target == nullptr)
			{
				oh->Flags.Clear(JOINT::eJF_LIVE);
				break;
			}
			o->LifeTime = 30;
			o->MaxTails = 10;
			o->Scale = Scale;
			o->TexType = BITMAP_FIRECRACKER;

			Vector(0.8f, 0.3f, 1.0f, o->Light);

			MUModel *b = &MODELS::Data[o->Target->Type];
			EVector3 p;
			Vector(0.0f, 0.0f, 0.0f, p);
			b->TransformPosition(ThreadIndex, o->Target->BoneTransform[b->GetBoneIndex(33)], p, o->Position, true);
		}
		else if (o->SubType == 10)     
		{
			o->Scale = Scale;
			o->Direction[2] = (GetLargeRand(150)) / 100.0f;
			o->Direction[1] = (mu_float)(GetLargeRand(500) - 250);
			if (o->SubType == 25)
				o->Velocity = 140.0f;
			else
				o->Velocity = 40.0f;

			if (o->Scale > 10.0f)
			{
				o->LifeTime = 50;
				o->Direction[2] = (GetLargeRand(250) + 200) / 100.0f;
			}

			VectorCopy(o->Target->Light, o->Light);
		}
		else if (o->SubType == 14 || o->SubType == 15)	
		{
			o->RenderType = BLEND::TXB_BLEND02;
			o->Scale = Scale;
			o->TexType = BITMAP_JOINT_SPIRIT;
			o->Direction[2] = (GetLargeRand(150)) / 100.0f;
			o->Direction[1] = (mu_float)(GetLargeRand(500) - 250);
			o->Velocity = 30.0f;

			
			Vector(0.2f, 0.4f, 0.8f, o->Light);

			if (o->Scale > 10.0f)
			{
				o->Direction[2] = (GetLargeRand(250) + 150) / 100.0f;
			}
			o->LifeTime = 20;

			o->Position[2] += GetLargeRand(25);
		}
		else if (o->SubType == 16)      
		{
			o->LifeTime = 60;
			o->Features.Clear(JOINT::eJFS_CREATETAILS);
			Vector(0.1f, 0.1f, 0.1f, o->Light);
			VectorCopy(TargetPosition, o->TargetPosition);
			o->Scale = Scale;
			for (mu_int32 j = 0; j < o->MaxTails; ++j)
			{
				o->Position[2] += 50.0f;

				CreateTail(o, Matrix);
			}
		}
		else if (o->SubType == 17)      
		{
			o->Position[0] += GetLargeRand(500) - 500;
			o->Position[1] += GetLargeRand(500) - 500;

			o->Direction[0] = 0.0f;
			o->Direction[1] = 0.0f;
			o->Direction[2] = 0.0f;
			o->Velocity = (GetLargeRand(200) + 10) / 25.0f;
			o->Scale = Scale;
			o->LifeTime = 20 + GetLargeRand(10);

			Vector(0.0f, 0.0f, 1.0f, o->Light);
		}
		else if (o->SubType == 2 || o->SubType == 24
			|| o->SubType == 50 || o->SubType == 51
			)  
		{
			if (o->SubType == 24)
				o->TexType = BITMAP_FLARE_RED;
			else
				if (o->SubType == 50 || o->SubType == 51)
					o->TexType = BITMAP_FLARE_BLUE;
			o->Direction[2] = (mu_float)(GetLargeRand(20) + 35);
			o->Scale = Scale;
			o->LifeTime = 25 + GetLargeRand(50);

			Vector(1.0f, 1.0f, 1.0f, o->Light);
		}
		else if (o->SubType == 41)
		{
			o->Scale = Scale;
			o->LifeTime = 40;
			o->MaxTails = MAX_TAILS;
			mu_float rbias = (mu_float)(GetLargeRand(300)) / 1000;
			mu_float gbias = (mu_float)(GetLargeRand(300)) / 1000;
			Vector(1.0f - rbias, 1.0f - gbias, 1.0f, o->Light);
			o->Direction[2] = (mu_float)(GetLargeRand(5) + 5);
		}
		else if (o->SubType == 42)
		{
			o->Direction[1] = -15.0f;
			AngleMatrix(o->Angle, Matrix);
			VectorRotate(o->Direction, Matrix, Position);
			VectorAdd(o->Position, Position, o->Position);
			VectorCopy(o->Position, o->StartPosition);
			o->Direction[1] = -50.0f;
			o->Features.Clear(JOINT::eJFS_CREATETAILS);
			o->Scale = Scale;
			Vector(1.0f, 1.0f, 1.0f, o->Light);
		}
		else if (o->SubType == 19)  
		{
			o->Direction[2] = -(mu_float)(GetLargeRand(20) + 35);
			o->Scale = Scale;
			o->LifeTime = 25 + GetLargeRand(50);

			Vector(1.0f, 1.0f, 1.0f, o->Light);
		}
		else if (o->SubType == 3)  
		{
			o->Velocity = 50.0f;
			o->Scale = Scale;
			o->LifeTime = 5;
			o->MaxTails = 10;

			EBone Matrix;
			Vector(0.0f, 45.0f, -90.0f, o->Angle);
			AngleMatrix(o->Angle, Matrix);
			Vector(0.0f, 100, 0.0f, p);
			VectorRotate(p, Matrix, Position);
			VectorAdd(o->Position, Position, o->Position);

			CreateSprite(BITMAP_SHINY + 1, o->Position, (mu_float)(GetLargeRand(8) + 8)*0.3f, o->Light, nullptr, (mu_float)(GetLargeRand(360)));

			Vector(1.0f, 1.0f, 1.0f, o->Light);
			o->Angle[0] *= -1.0f;
			o->Angle[1] *= -1.0f;
			o->Angle[2] *= -1.0f;
		}
		else if (o->SubType == 4 || o->SubType == 6 || o->SubType == 12)	
		{
			o->Scale = Scale;
			if (o->SubType == 12)
			{
				o->LifeTime = 70;
				o->MaxTails = 50;
				
				Vector(0.1f, 0.1f, 1.0f, o->Light);

				
				o->Direction[0] = (mu_float)(GetLargeRand(360));
				o->Direction[1] = -4.0f * (mu_float)EMath::Sin(-o->Angle[2] * Q_PI / 180.0f);
				o->Direction[2] = -4.0f * (mu_float)EMath::Cos(-o->Angle[2] * Q_PI / 180.0f);
			}
			else
			{
				if (o->SubType == 6)
				{
					o->LifeTime = 20;
					o->MaxTails = 30;
					if (o->Type == BITMAP_FLARE_BLUE)
					{
						o->LifeTime = 15;
						o->MaxTails = 15;
					}
					Vector(1.0f, 1.0f, 1.0f, o->Light);
				}
				else
				{
					o->LifeTime = 110;
					o->MaxTails = 200;

					if (o->Target != nullptr)
					{
						VectorCopy(o->Target->Light, o->Light);
					}
				}
				o->Direction[0] = (mu_float)(GetLargeRand(360));
				o->Direction[1] = -2.0f * (mu_float)EMath::Sin(-o->Angle[2] * Q_PI / 180.0f);
				o->Direction[2] = -2.0f * (mu_float)EMath::Cos(-o->Angle[2] * Q_PI / 180.0f);
			}
		}
		else if (o->SubType == 5)	
		{
			o->LifeTime = 2;
			o->MaxTails = 3;
			o->Direction[2] = -(mu_float)(GetLargeRand(3) + 40);

			Vector(1.0f, 1.0f, 1.0f, o->Light);
		}
		else if (o->SubType == 7)	
		{
			Vector(0.2f, 0.2f, 1.0f, o->Light);
			o->MultiUse = GetLargeRand(10);	
			o->LifeTime = 30 + o->MultiUse;	
			o->MaxTails = 15;
			o->Direction[0] = (mu_float)(GetLargeRand(3000));
			o->Scale = 30.0f;
		}
		else if (o->SubType == 8)
		{
			Vector(0.0f, 0.0f, 0.0f, o->Angle);
			Vector(0.0f, 0.0f, 0.0f, o->Direction);
			VectorCopy(o->Position, o->StartPosition);

			Vector(0.0f, -50.0f, 0.0f, p);
			AngleMatrix(o->TargetPosition, Matrix);
			VectorRotate(p, Matrix, Position);
			VectorAdd(o->StartPosition, Position, o->Position);
		}
		else if (o->SubType == 9)   
		{
			o->LifeTime = 0;
			o->Scale = Scale;
			o->MaxTails = 10;
			Vector(0.0f, 0.0f, 0.0f, o->Angle);
			Vector(0.3f, 0.3f, 1.0f, o->Light);
			o->StartPosition[0] = (TargetPosition[0] - o->Position[0]) / o->MaxTails;
			o->StartPosition[1] = (TargetPosition[1] - o->Position[1]) / o->MaxTails;
			o->StartPosition[2] = (TargetPosition[2] - o->Position[2]) / o->MaxTails;

			AngleMatrix(o->Angle, Matrix);
			for (mu_int32 i = 0; i < o->MaxTails; ++i)
			{
				VectorAdd(o->Position, o->StartPosition, o->Position);
				CreateTail(o, Matrix);
			}
		}
		else if (o->SubType == 11)	
		{
			Vector(0.2f, 0.2f, 1.0f, o->Light);
			o->MultiUse = 0;	
			o->LifeTime = 30;	
			o->MaxTails = 15;
			o->Direction[0] = (mu_float)(GetLargeRand(3000));
			o->Scale = 30.0f;
		}
		else if (o->SubType == 25)		
		{
			Vector(0.9f, 0.4f, 0.6f, o->Light);
			o->MultiUse = 0;
			o->LifeTime = 30;
			o->MaxTails = 15;
			o->Direction[0] = (mu_float)(GetLargeRand(3000));
			o->Scale = 30.0f;
		}
		else if (o->SubType == 13)         
		{
			o->Direction[2] = (mu_float)(GetLargeRand(20) + 35);
			o->Scale = Scale;
			o->LifeTime = 25 + GetLargeRand(50);

			Vector(0.5f, 0.5f, 0.5f, o->Light);
		}
		else if (o->SubType == 23)
		{
			o->LifeTime = 20 + (4 - PKKey);
			o->Scale = Scale;
			o->MaxTails = 15;
			o->NumTails = -1;
			o->Velocity = 0.0f;
			o->Direction[0] = 1.0f;
			o->Direction[1] = 5.0f;
			o->Direction[2] = 1.0f;
			o->Features.Clear(JOINT::eJFS_CREATETAILS);

			VectorCopy(o->Angle, o->HeadAngle);

			o->MultiUse = PKKey;
			switch (o->MultiUse)
			{
			case 0: o->HeadAngle[2] += 90.0f; o->Position[2] += 200.0f; break;
			case 1: o->HeadAngle[2] += 90.0f; o->Position[2] += 10.0f; break;
			case 2: o->HeadAngle[2] -= 90.0f; o->Position[2] += 200.0f; break;
			case 3: o->HeadAngle[2] -= 90.0f; o->Position[2] += 10.0f; break;
			case 4: o->HeadAngle[2] += 180.0f; o->Position[2] += 100.0f; break;
			case 5:
				o->HeadAngle[0] = 90.0f;
				o->Position[2] += 100.0f;
				o->LifeTime = 10;
				o->MaxTails = 20;
				o->Features.Clear(JOINT::eJFS_FACEONE);
				break;
			}
			VectorCopy(o->Position, o->StartPosition);
		}
		else if (o->SubType == 43)
		{
			o->LifeTime = 100;	
			o->MaxTails = 0;
			o->Features.Clear(JOINT::eJFS_CREATETAILS);
		}
		else if (o->SubType == 44)
		{
			o->Scale = Scale;
			o->LifeTime = 15;
			o->MaxTails = 30;
			o->Direction[2] = (mu_float)(GetLargeRand(2) + 2);
		}
		else if (o->SubType == 45 || o->SubType == 46)	
		{
			Vector(0.2f, 0.2f, 1.0f, o->Light);
			o->MultiUse = GetLargeRand(10);	
			o->LifeTime = 30 + o->MultiUse;	
			o->MaxTails = 15;
			o->Direction[0] = (mu_float)(GetLargeRand(3000));
			o->Scale = 30.0f;
		}
		else if (o->SubType == 47)
		{
			Vector(0.7f, 0.7f, 1.0f, o->Light);
			o->MultiUse = GetLargeRand(10);	
			o->LifeTime = 30 + o->MultiUse;	
			o->MaxTails = 15;
			o->Direction[0] = (mu_float)(GetLargeRand(3000));
			o->Scale = 30.0f;
		}
		else
		{
			o->Features.Clear(JOINT::eJFS_CREATETAILS);
		}
		VectorCopy(TargetPosition, o->TargetPosition);
		break;

	case BITMAP_FLARE + 1:    
		o->LifeTime = PKKey;
		o->PKKey = 0;
		o->MaxTails = MAX_TAILS;
		switch (o->Skill)
		{
		case 0:
			o->Scale = 20.0f;
			Vector(0.5f, 0.5f, 0.5f, o->Light);
			break;

		case 1:
			o->Scale = 40.0f;
			Vector(1.0f, 0.5f, 0.0f, o->Light);
			break;
		case 3:		
			o->Scale = 20.0f;
			VectorCopy(Target->Light, o->Light);
			break;
		}
		switch (o->SubType)
		{
		case 0:
			o->Velocity = 0.0f;
			Vector(Scale*1.5f, 0.0f, 0.0f, o->Direction);
			break;

		case 1:
			o->Velocity = 90.0f;
			Vector(Scale*1.5f, 0.0f, 0.0f, o->Direction);
			break;

		case 2:
			o->Velocity = 180.0f;
			Vector(Scale*1.5f, 0.0f, 0.0f, o->Direction);
			break;

		case 3:
			o->Velocity = 240.0f;
			Vector(Scale*1.5f, 0.0f, 0.0f, o->Direction);
			break;

		case 4: 
			{
				o->RenderType = BLEND::TXB_BLENDMINUS;
				o->LifeTime = 0;
				o->Scale = Scale;
				o->MaxTails = 10;

				Vector(0.0f, 0.0f, 0.0f, o->Angle);
				Vector(1.0f, 1.0f, 1.0f, o->Light);
				o->StartPosition[0] = (TargetPosition[0] - o->Position[0]) / o->MaxTails;
				o->StartPosition[1] = (TargetPosition[1] - o->Position[1]) / o->MaxTails;
				o->StartPosition[2] = (TargetPosition[2] - o->Position[2]) / o->MaxTails;

				AngleMatrix(o->Angle, Matrix);
				for (mu_int32 i = 0; i < o->MaxTails; ++i)
				{
					VectorAdd(o->Position, o->StartPosition, o->Position);
					CreateTail(o, Matrix);
				}
			}
			break;

		case 5:
			Vector(1.0f, 1.0f, 1.0f, o->Light);
			o->RenderType = BLEND::TXB_BLEND02;
			o->Features.Clear(JOINT::eJFS_FACEONE);
			o->LifeTime = 50;
			o->MaxTails = 8;
			o->Velocity = 3.0f;
			break;

		case 6:
			Vector(1.0f, 1.0f, 1.0f, o->Light);
			o->RenderType = BLEND::TXB_BLEND02;
			o->Scale = Scale;
			o->MaxTails = 16;
			o->Velocity = 5.0f;
			break;

		case 7:
			Vector(1.0f, 1.0f, 1.0f, o->Light);
			Vector(0.0f, 0.0f, 0.0f, o->Direction);
			o->RenderType = BLEND::TXB_BLEND02;
			o->Scale = Scale;
			o->MaxTails = 16;
			o->Velocity = 10.0f;
			o->TexType = BITMAP_FLARE;
			o->Direction[0] = 15.0f;

			o->Position[0] = TargetPosition[0] + (mu_float)EMath::Cos((mu_float)(GetLargeRand(360))) * 40;
			o->Position[1] = TargetPosition[1] - (mu_float)EMath::Sin((mu_float)(GetLargeRand(360))) * 40;
			VectorCopy(o->Position, o->StartPosition);
			break;

		case 8: 
		case 9: 
			o->Features.Clear(JOINT::eJFS_FACEONE);
		case 10:
		case 11:
			o->RenderType = BLEND::TXB_BLEND02;
			o->Scale = Scale;
			o->MaxTails = 20;
			o->LifeTime = 100;
			o->Velocity = 0.0f;
			o->MultiUse = SkillIndex;

			if (o->SubType == 10 || o->SubType == 11)
			{
				Vector(0.7f, 0.7f, 0.7f, o->Light);
				o->MaxTails = 10;
			}
			else
			{
				Vector(1.0f, 1.0f, 1.0f, o->Light);
			}
			Vector(0.0f, 0.0f, 0.0f, o->Direction);

			if (o->Target == nullptr)
			{
				oh->Flags.Clear(JOINT::eJF_LIVE);
			}
			VectorCopy(o->Target->Position, o->Position);
			VectorCopy(o->Target->Angle, o->Angle);
			if (o->SubType == 8)
			{
				o->Position[2] = 300.0f;
			}
			else if (o->SubType == 9)
			{
				o->Position[2] += o->MultiUse;
			}
			break;

		case 12:
			Vector(0.6f, 0.2f, 0.8f, o->Light);
			o->RenderType = BLEND::TXB_BLEND02;
			o->Scale = Scale;
			o->MaxTails = 16;
			o->Velocity = 70.0f;
			break;
		case 13:
			Vector(0.7f, 0.7f, 0.3f, o->Light);
			o->RenderType = BLEND::TXB_BLEND02;
			o->Scale = Scale;
			o->MaxTails = 16;
			o->Velocity = 70.0f;
			break;
		case 14:
			Vector(1.0f, 1.0f, 1.0f, o->Light);
			o->RenderType = BLEND::TXB_BLEND01;
			o->Scale = Scale;
			o->MaxTails = 16;
			o->Velocity = 70.0f;
			break;
		case 15:
			Vector(0.4f, 0.9f, 0.5f, o->Light);
			o->RenderType = BLEND::TXB_BLEND01;
			o->Scale = Scale;
			o->MaxTails = 16;
			o->Velocity = 70.0f;
			break;
		case 16:
			o->RenderType = BLEND::TXB_BLEND02;
			o->Features.Clear(JOINT::eJFS_FACEONE);
			o->LifeTime = 35;
			o->MaxTails = 8;
			o->Velocity = 7.0f;
			Vector(1.0f, 0.5f, 0.3f, o->Light);
			break;
		case 17:
			Vector(1.0f, 1.0f, 1.0f, o->Light);
			o->RenderType = BLEND::TXB_BLEND01;
			o->Scale = Scale;
			o->MaxTails = 16;
			o->Velocity = 70.0f;
			break;
		case 18:
			{
				if (vPriorColor)
				{
					VectorCopy(*vPriorColor, o->Light);
				}
				o->RenderType = BLEND::TXB_BLEND01;
				o->Scale = Scale;
				o->MaxTails = 7;
				o->Velocity = 70.0f;
			}
			break;
		case 19:
			{
				Vector(0.5f, 0.5f, 1.0f, o->Light);
				o->RenderType = BLEND::TXB_BLEND01;
				o->Scale = Scale;
				o->MaxTails = 14;
				o->Velocity = 30.0f;
				o->LifeTime = 50;
			}
			break;
		case 20:
			{
				Vector(0.5f, 0.5f, 1.0f, o->Light);
				o->RenderType = BLEND::TXB_BLEND01;
				o->Scale = Scale;
				o->MaxTails = 14;
				o->Velocity = 30.0f;
				o->LifeTime = 50;
			}
			break;
		}
		VectorCopy(TargetPosition, o->TargetPosition);
		break;
	case BITMAP_JOINT_FORCE:    
		if (o->SubType == 0
			|| o->SubType == 10
			)
		{
			o->LifeTime = 20;
			o->Features.Clear(JOINT::eJFS_CREATETAILS);
			o->Scale = Scale;
			o->Velocity = 0.0f;
			o->MaxTails = 18;
			o->NumTails = -1;

			Vector(0.0f, -180.0f, 0.0f, p);
			VectorCopy(o->Position, o->TargetPosition);
			o->Angle[2] += 30.0f;
			VectorCopy(o->Angle, o->Direction);

			AngleMatrix(o->Angle, Matrix);
			VectorRotate(p, Matrix, Position);
			VectorAdd(o->Position, Position, o->Position);
		}
		else if (o->SubType == 1)
		{
			o->Scale = Scale;
			o->MaxTails = 15;
			o->LifeTime = 30;
			o->Velocity = 0.0f;
			o->Weapon = 0;

			Vector(1.0f, 1.0f, 1.0f, o->Light);
			Vector(0.0f, 0.0f, 0.0f, o->Direction);

			if (o->Target == nullptr)
			{
				oh->Flags.Clear(JOINT::eJF_LIVE);
			}
			VectorCopy(o->Target->Position, o->Position);
			VectorCopy(o->Target->Angle, o->Angle);
		}
		else if (o->SubType >= 2 && o->SubType <= 6)
		{
			o->LifeTime = 20;
			o->Scale = Scale;
			o->Velocity = 8.0f;
			o->Direction[0] = 3.5f;
			o->Direction[2] = 1.0f;
			o->MaxTails = 12;
			o->NumTails = -1;
			o->MultiUse = 5;
			if (o->SubType == 3)
			{
				o->TexType = BITMAP_FLARE;
				o->MultiUse = 10;
				o->Velocity = 10.0f;
				o->Direction[2] = 2.0f;
				o->ReverseUV = 0;
				o->Direction[0] = 10.5f;
			}
			else if (o->SubType == 4)
			{
				o->LifeTime = 20;
				o->MaxTails = 13;
				o->TexType = BITMAP_HOLE;
				o->Direction[0] = 15.5f;
				o->Direction[2] = 3.0f;
				o->MultiUse = 10;
			}
			else if (o->SubType == 5)
			{
				o->TexType = BITMAP_LAVA;
			}
			else if (o->SubType == 6)
			{
				o->TexType = BITMAP_LAVA;
				o->Velocity = 20.0f;
				o->LifeTime = 16;
			}
			else
			{
				o->Position[2] += 10.0f;
				o->TexType = BITMAP_INFERNO;
				o->LifeTime = 15;
				o->MultiUse = 5;
				o->Direction[0] = 5.0f;
				o->Direction[2] = 5.0f;
			}
			Vector(0.0f, 0.0f, o->Angle[2], o->HeadAngle);
			VectorCopy(o->Position, o->StartPosition);
		}
		else if (o->SubType == 7)
		{
			o->Scale = Scale;
			o->Velocity = 8.0f;
			o->Direction[0] = 3.5f;
			o->Direction[2] = 1.0f;
			o->MaxTails = 13;
			o->NumTails = -1;
			o->MultiUse = 5;

			o->TexType = BITMAP_INFERNO;
			o->Velocity = 10.0f;
			o->LifeTime = 20;
		}
		else if (o->SubType == 8)	
		{
			o->LifeTime = 20;
			o->Features.Clear(JOINT::eJFS_CREATETAILS);
			o->Scale = Scale;
			o->Velocity = 0.0f;
			o->MaxTails = 18;
			o->NumTails = -1;

			Vector(0.0f, -180.0f, 0.0f, p);
			VectorCopy(o->Position, o->TargetPosition);
			o->Angle[2] += 30.0f;
			VectorCopy(o->Angle, o->Direction);

			AngleMatrix(o->Angle, Matrix);
			VectorRotate(p, Matrix, Position);
			VectorAdd(o->Position, Position, o->Position);
			
		}
		else if (o->SubType == 20)	
		{
			o->Scale = Scale;
			o->Velocity = 8.0f;
			o->Direction[0] = 3.5f;
			o->Direction[2] = 1.0f;
			o->MaxTails = 15;
			o->NumTails = -1;
			o->MultiUse = 5;

			o->TexType = BITMAP_INFERNO;
			o->Velocity = 10.0f;
			o->LifeTime = 20;
		}
		break;

	case BITMAP_LIGHT:
		o->Features.Clear(JOINT::eJFS_CREATETAILS);
		if (o->SubType == 0)
		{
			o->LifeTime = 20;
			o->MaxTails = 30;
			o->Scale = Scale;

			o->Velocity = GetLargeRand(10) + 10.0f;
			Vector(-(mu_float)(GetLargeRand(40)) + 30.0f, 0.0f, (mu_float)(GetLargeRand(360)), o->Angle);
			Vector(0.0f, 0.0f, 0.0f, o->Direction);
			Vector(0.1f, 0.5f, 1.0f, o->Light);

			VectorCopy(o->Position, o->StartPosition);
		}
		else if (o->SubType == 1)
		{
			o->LifeTime = 10;
			o->MaxTails = 20;
			o->Scale = Scale;
			o->Skill = GetLargeRand(5);

			o->Velocity = GetLargeRand(5) + 1.0f;
			Vector(30.0f, 0.0f, (mu_float)(GetLargeRand(360)), o->Angle);
			Vector(0.0f, 0.0f, 0.0f, o->Direction);
			Vector(1.0f, 0.8f, 0.6f, o->Light);

			VectorCopy(o->Position, o->StartPosition);
		}
		break;

	case BITMAP_PIERCING:
		o->Features.Clear(JOINT::eJFS_CREATETAILS);
		if (o->SubType == 0)       
		{
			o->LifeTime = 10;
			o->MaxTails = 30;
			o->Scale = Scale;

			o->Velocity = 20.0f;
			Vector(-90.0f, 0.0f, Angle[2], o->Angle);
			Vector(0.0f, 0.0f, 0.0f, o->Direction);
			if (GetLargeRand(2) == 0)
			{
				Vector(1.0f, 0.8f, 0.6f, o->Light);
			}
			else
			{
				Vector(1.0f, 1.0f, 1.0f, o->Light);
			}

			VectorCopy(o->Position, o->StartPosition);
		}
		else if (o->SubType == 1)       
		{
			o->LifeTime = 10;
			o->Features.Set(JOINT::eJFS_TILEMAPPING);
			o->MaxTails = 30;
			o->Scale = Scale;

			o->Velocity = 20.0f;
			Vector(-90.0f, 0.0f, Angle[2], o->Angle);
			Vector(0.0f, 0.0f, 0.0f, o->Direction);
			if (GetLargeRand(2) == 0)
			{
				Vector(1.0f, 0.8f, 0.6f, o->Light);
			}
			else
			{
				Vector(1.0f, 1.0f, 1.0f, o->Light);
			}

			VectorCopy(o->Position, o->StartPosition);
		}
		break;
		
	case BITMAP_FLARE_FORCE:
		o->LifeTime = 20;
		o->Features.Clear(JOINT::eJFS_CREATETAILS);
		o->MaxTails = 30;
		o->Scale = Scale;
		o->MultiUse = 1;
		o->Velocity = -3.0f;
		o->Position[2] += 150.0f;
		Vector(0.0f, 0.0f, 0.0f, o->Direction);
		Vector(0.0f, 0.0f, 0.0f, o->TargetPosition);
		Vector(1.0f, 0.8f, 1.0f, o->Light);
		VectorCopy(o->Position, o->StartPosition);
		o->Weapon = 0;  

		if ((o->SubType >= 1 && o->SubType <= 4) ||
			(o->SubType >= 11 && o->SubType <= 13)	)
		{
			o->TexType = BITMAP_JOINT_THUNDER;
			o->Velocity = -3.0f;
			o->TargetPosition[0] = 80.0f;
			o->TargetPosition[1] = 180.0f;
			o->Weapon = GetLargeRand(3) + 2;
			if (o->SubType == 2 || o->SubType == 4
				|| (o->SubType >= 11 && o->SubType <= 13)	
				)
			{
				
				switch (o->SubType)
				{
				case 11:	
					Vector(0.7f, 1.0f, 0.7f, o->Light);
					break;
				case 12:	
					Vector(1.0f, 0.6f, 0.6f, o->Light);
					break;
				case 13:	
					Vector(0.7f, 0.7f, 1.0f, o->Light);
					break;
				}

				o->TargetPosition[1] = -180.0f;
			}

			if (o->SubType == 3 || o->SubType == 4)
			{
				o->Weapon = 0;
			}

			o->LifeTime += o->Weapon;

			AngleMatrix(o->TargetPosition, Matrix);
			Vector(0.0f, 0.0f, o->TargetPosition[0], p);
			VectorRotate(p, Matrix, Position);
			VectorAdd(o->StartPosition, Position, o->Position);
		}
		else if (o->SubType != 0)
		{
			o->LifeTime = 15;
			o->Skill = 0;
			o->MaxTails = 1;
			o->Weapon = 30;
			o->MultiUse = SkillIndex;
			o->Velocity = 0.0f;
			o->TargetPosition[0] = 30.0f;
			o->TargetPosition[2] = (mu_float)(o->SubType * 90);
			Vector(1.0f, 0.8f, 1.0f, o->Light);
			Vector(0.0f, -4.0f, 0.0f, o->Direction);
			VectorCopy(o->Position, o->StartPosition);

			if (o->PKKey != -1)
				o->Direction[1] = 0.0f;
		}

		if (o->SubType >= 5 && o->SubType <= 7)
		{
			o->TexType = BITMAP_FIRECRACKER;
		}
		else if (o->SubType == 0)
		{
			o->TexType = BITMAP_JOINT_THUNDER;
		}
		break;
		
	case BITMAP_FLASH:
		if (o->SubType <= 3 || o->SubType == 5)
		{
			o->MaxTails = 10;
			o->LifeTime = 40;
			o->Velocity = 70.0f;
			o->Scale = Scale;
			o->ReverseUV = GetLargeRand(2);
			o->MultiUse = 150;
			o->PKKey = 0;
			if (o->SubType == 2)
			{
				o->Velocity = 30.0f;
				o->MultiUse = 300;
				o->TexType = BITMAP_FLARE_FORCE;
			}
			else if (o->SubType == 3)
			{
				o->Velocity = 30.0f;
				o->MultiUse = 300;
				o->TexType = BITMAP_FLARE_BLUE;
			}

			if (o->SubType == 5)
			{
				o->TexType = BITMAP_FLARE;
				o->MaxTails = 15;
				o->Velocity = GetLargeRand(20) + 10.0f;
				o->ReverseUV = 2;
			}
			else
			{
				Vector(90.0f, 0.0f, 0.0f, o->Angle);
			}
			VectorCopy(o->Angle, o->HeadAngle);
		}
		else if (o->SubType == 4)
		{
			o->Scale = Scale;
			o->MaxTails = 20;
			o->LifeTime = 30;
			o->Velocity = 0.0f;
			o->Weapon = 0;
			o->TexType = BITMAP_FLARE_BLUE;

			Vector(1.0f, 1.0f, 1.0f, o->Light);
			Vector(0.0f, 0.0f, 0.0f, o->Direction);

			if (o->Target == nullptr)
			{
				oh->Flags.Clear(JOINT::eJF_LIVE);
			}
			VectorCopy(o->Target->Position, o->Position);
			VectorCopy(o->Target->Angle, o->Angle);
		}
		else if (o->SubType == 6)
		{
			
			o->Scale = Scale;
			o->MaxTails = 30;
			o->LifeTime = 25;
			o->Weapon = o->LifeTime;
			o->Velocity = (mu_float)(70 + GetLargeRand(3)) * Q_PI / 180; 

			Vector(0.8f, 0.8f, 1.0f, o->Light);
			Vector(0.0f, -(40.0f + (mu_float)(GetLargeRand(4))), EMath::Sin(o->Velocity) * 10.0f, o->Direction);
			o->Position[1] -= 20.0f;
			o->Position[2] += 130.0f;
			VectorCopy(o->Position, o->StartPosition);
			VectorCopy(Angle, o->Angle);
			AngleMatrix(o->Angle, Matrix);
			o->Features.Clear(JOINT::eJFS_CREATETAILS);
			o->ReverseUV = 3;
			CreateTailAxis(o, Matrix, 1);
			o->NumTails = 0;
		}
		else if (o->SubType == 7)
		{
			
			o->Features.Clear(JOINT::eJFS_CREATETAILS);
			o->LifeTime = 0;
			for (mu_int32 i = 0; i < 3; ++i)
			{
				EVector3 Pos;
				AngleMatrix(o->Angle, Matrix);
				Vector((i - 1) * 100, -50.0f, 0.0f, Pos);
				VectorRotate(Pos, Matrix, Position);
				VectorAdd(Position, o->Position, Pos);
				CreateJoint(ThreadIndex, BITMAP_FLASH, Pos, Pos, o->Angle, 6, nullptr, 40.0f);
			}
		}
		break;
	case BITMAP_DRAIN_LIFE_GHOST:
		{
			switch (o->SubType)
			{
			case 0:
				{
					o->RenderType = BLEND::TXB_BLEND01;

					
					if (GetLargeRand(2) == 0)
					{
						o->Angle[2] += 90.0f;
					}
					else
					{
						o->Angle[2] -= 90.0f;
					}

					VectorCopy(TargetPosition, o->TargetPosition);
					o->Angle[0] += (mu_float)((GetLargeRand(100)) - 50);				
					o->Angle[1] += (mu_float)((GetLargeRand(100)) - 50);				
					o->Angle[2] += (mu_float)((GetLargeRand(100)) - 50);				
					o->Velocity = (mu_float)(1 + ((mu_float)(GetLargeRand(10))*0.2f));		
					o->LifeTime = (mu_float)(30 + (GetLargeRand(20) - 10));				
					o->Scale = Scale + (mu_float)((GetLargeRand(60) - 30));			
					o->MaxTails = (mu_float)(20 + (GetLargeRand(10) - 5));				
				}
				break;
			}
		}
		break;
	}

	if (o->MaxTails > MAX_TAILS)
		o->MaxTails = MAX_TAILS;
}

template void CreateJoint<false>(const mu_uint32 ThreadIndex, mu_uint32 Type, EVector3 Position, EVector3 TargetPosition, EVector3 Angle, mu_uint32 SubType, OBJECT *Target, mu_float Scale, mu_int16 PKKey,
	mu_uint16 SkillIndex, mu_uint16 SkillSerialNum, mu_int32 iChaIndex, const EVector3 *vPriorColor, mu_int16 sTargetIndex);
template void CreateJoint<true>(const mu_uint32 ThreadIndex, mu_uint32 Type, EVector3 Position, EVector3 TargetPosition, EVector3 Angle, mu_uint32 SubType, OBJECT *Target, mu_float Scale, mu_int16 PKKey,
	mu_uint16 SkillIndex, mu_uint16 SkillSerialNum, mu_int32 iChaIndex, const EVector3 *vPriorColor, mu_int16 sTargetIndex);

void DeleteJoint(mu_int32 Type, OBJECT *Target, mu_int32 SubType)
{
	mu_uint32 i = (mu_uint32)-1;

	g_JointsEraseLock.Lock();

	if (g_JointsEraseCount < MAX_JOINTS_WAIT)
	{
		i = g_JointsEraseCount++;
	}

	g_JointsEraseLock.Unlock();

	if (i != (mu_uint32)-1)
	{
		JOINT_DELETE &joint = g_JointsErase[i];
		joint.Type = Type;
		joint.SubType = SubType;
		joint.Target = Target;
	}
}

mu_boolean SearchJoint(mu_int32 Type, OBJECT *Target, mu_int32 SubType)
{
	MUAutoCriticalSpinLock lock(g_JointsLock);

	for (mu_int32 i = 0; i < g_JointsCurCount; ++i)
	{
		JOINT_HEADER *oh = &g_JointsHeader[i];
		if (oh->Flags.Get(JOINT::eJF_LIVE) == false)
		{
			continue;
		}

		JOINT *o = &g_Joints[i];
		if (o->Type == Type &&
			o->Target == Target)
		{
			if (SubType == -1 || o->SubType == SubType)
				return true;
		}
	}

	return false;
}