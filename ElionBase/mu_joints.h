#ifndef __MU_JOINTS_H__
#define __MU_JOINTS_H__

#pragma once

class JOINT_HEADER
{
public:
	MUFlagset<mu_uint8> Flags;
};

class JOINT_CHECK
{
public:
	mu_uint64 CreationTime;
};

class JOINT
{
public:
	enum JOINT_FLAGS
	{
		eJF_FREE = (1 << 0), // Should always be flag 1
		eJF_LIVE = (1 << 1), // Should always be flag 2
	};
	enum JOINT_FEATURES
	{
		eJFS_COLLISION = (1 << 0),
		eJFS_TILEMAPPING = (1 << 1),
		eJFS_CREATETAILS = (1 << 2),
		eJFS_FACEONE = (1 << 3),
		eJFS_FACETWO = (1 << 4),
		eJFS_ALTERNATIVE = (1 << 5),
	};

	MUFlagset<mu_uint8> Features;

	mu_uint8 RenderType;
	mu_uint8 Weapon;
	mu_uint8 ReverseUV;

	mu_int16 BeginTails;
	mu_int16 NumTails;
	mu_int16 MaxTails;

	mu_int16 PKKey;
	std::array<mu_int16, 5> TargetIndex;
	mu_int16 m_iChaIndex;
	mu_int16 m_sTargetIndex;

	mu_uint16 Skill;

	mu_uint16 Type;
	mu_uint16 TexType;
	mu_uint16 SubType;

	mu_int32 LifeTime;
	mu_int32 MultiUse;

	mu_float Scale;
	mu_float Velocity;

	EVector3 Position;
	EVector3 StartPosition;
	EVector3 TargetPosition;
	EVector3 Angle;
	EVector3 HeadAngle;
	EVector3 Light;
	EVector3 Direction;
	std::array<std::array<EVector3, 4>, MAX_TAILS> Tails;

	OBJECT *Target;
};

struct JOINT_WAIT
{
	mu_boolean EnablePriorColor;

	mu_int16 PKKey;
	mu_int16 sTargetIndex;

	mu_uint16 SkillIndex;
	mu_uint16 SkillSerialNum;

	mu_int32 iChaIndex;

	mu_uint32 Type;
	mu_uint32 SubType;

	mu_float Scale;

	EVector3 Position;
	EVector3 TargetPosition;
	EVector3 Angle;
	EVector3 vPriorColor;

	OBJECT *Target;
};

struct JOINT_DELETE
{
	mu_int32 Type;
	mu_int32 SubType;
	OBJECT *Target;
};

extern MUCriticalSectionSpinLock g_JointsLock;
extern std::array<JOINT_HEADER, MAX_JOINTS> g_JointsHeader;
extern std::array<JOINT_CHECK, MAX_JOINTS> g_JointsCheck;
extern std::array<JOINT, MAX_JOINTS> g_Joints;
extern std::array<mu_uint16, MAX_JOINTS + MAX_TASKING_THREADS> g_JointsQueue;
extern std::array<mu_uint16, MAX_JOINTS> g_JointsFree;
extern mu_uint16 g_JointsRenderCount;
extern mu_uint16 g_JointsCurCount;
extern mu_uint16 g_JointsFreeCount;
extern mu_atomic_int32_t g_JointsDeleteCount;
extern mu_boolean g_JointsLocked;

extern std::array<JOINT_WAIT, MAX_JOINTS_WAIT> g_JointsWait;
extern mu_atomic_uint32_t g_JointsWaitCount;
extern std::array<JOINT_DELETE, MAX_JOINTS_WAIT> g_JointsErase;
extern mu_uint16 g_JointsEraseCount;

void GetMagicScrew(mu_int32 iParam, EVector3 &vResult, mu_float fSpeedRate = 1.0f);

#ifdef USE_DUAL_QUATERNION
void CreateTailAxis(JOINT *o, const EDualQuat &Matrix, const mu_uint8 axis);
void CreateTailAxis(JOINT *o, const EDualQuat &Matrix, const mu_float ScaleX, const mu_float ScaleY, const mu_uint8 axis);
void CreateTail(JOINT *o, const EDualQuat &Matrix, const mu_boolean Blur = false);
void CreateTail(JOINT *o, const EDualQuat &Matrix, const mu_float ScaleX, const mu_float ScaleY);
#endif

void CreateTailAxis(JOINT *o, const EMatrix4 &Matrix, const mu_uint8 axis);
void CreateTailAxis(JOINT *o, const EMatrix4 &Matrix, const mu_float ScaleX, const mu_float ScaleY, const mu_uint8 axis);
void CreateTail(JOINT *o, const EMatrix4 &Matrix, const mu_boolean Blur = false);
void CreateTail(JOINT *o, const EMatrix4 &Matrix, const mu_float ScaleX, const mu_float ScaleY);

template<mu_boolean isLockedAllowed = true>
void CreateJoint(const mu_uint32 ThreadIndex, mu_uint32 Type, EVector3 Position, EVector3 TargetPosition, EVector3 Angle, mu_uint32 SubType = 0, OBJECT *Target = nullptr, mu_float Scale = 10.0f, mu_int16 PKKey = -1,
	mu_uint16 SkillIndex = 0, mu_uint16 SkillSerialNum = 0, mu_int32 iChaIndex = -1, const EVector3 *vPriorColor = nullptr, mu_int16 sTargetIndex = -1);
void DeleteJoint(mu_int32 Type, OBJECT *Target, mu_int32 SubType = -1);
mu_boolean SearchJoint(mu_int32 Type, OBJECT *Target, mu_int32 SubType = -1);

#endif