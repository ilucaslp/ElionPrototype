#ifndef __MU_OBJECT_BASE_H__
#define __MU_OBJECT_BASE_H__

#pragma once

class CPhysicsCloth;

#include "mu_buff.h"

#define TELEPORT_NONE   0
#define TELEPORT_BEGIN  1
#define TELEPORT        2
#define TELEPORT_END    3

enum
{
	KIND_PLAYER		= (1 << 0),
	KIND_MONSTER	= (1 << 1),
	KIND_NPC		= (1 << 2),
	KIND_TRAP		= (1 << 3),
	KIND_OPERATE	= (1 << 4),
	KIND_EDIT		= (1 << 5),
	KIND_PET		= (1 << 6),
	KIND_TMP		= (1 << 7),
};

#define ATTACK_FAIL    0
#define ATTACK_SUCCESS 1
#define ATTACK_DIE     2

class CInterpolateContainer
{
public:
	struct INTERPOLATE_FACTOR
	{
		mu_float	fRateStart;
		mu_float	fRateEnd;

		EVector3	v3Start;
		EVector3	v3End;
		INTERPOLATE_FACTOR(mu_float fRateStart_, mu_float fRateEnd_,
			EVector3& v3Start_, EVector3& v3End_)
		{
			fRateStart = fRateStart_;
			fRateEnd = fRateEnd_;

			VectorCopy(v3Start_, v3Start);
			VectorCopy(v3End_, v3End);
		};

		INTERPOLATE_FACTOR()
		{

		};
	};

	struct INTERPOLATE_FACTOR_F
	{
		mu_float	fRateStart;
		mu_float	fRateEnd;

		mu_float	fStart;
		mu_float	fEnd;
		INTERPOLATE_FACTOR_F(mu_float fRateStart_, mu_float fRateEnd_,
			mu_float& fStart_, mu_float& fEnd_)
		{
			fRateStart = fRateStart_;
			fRateEnd = fRateEnd_;

			fStart = fStart_;
			fEnd = fEnd_;
		};

		INTERPOLATE_FACTOR_F()
		{

		};
	};


public:
	typedef std::vector<INTERPOLATE_FACTOR>		VEC_INTERPOLATES;
	typedef std::vector<INTERPOLATE_FACTOR_F>	VEC_INTERPOLATES_F;
	VEC_INTERPOLATES						m_vecInterpolatesAngle;
	VEC_INTERPOLATES						m_vecInterpolatesPos;
	VEC_INTERPOLATES_F						m_vecInterpolatesScale;
	VEC_INTERPOLATES_F						m_vecInterpolatesAlpha;

public:
	void GetCurrentValue(EVector3& v3Out, mu_float fCurrentRate, VEC_INTERPOLATES& vecInterpolates);
	void GetAngleCurrent(EVector3& v3Out, mu_float fCurrentRate)
	{
		GetCurrentValue(v3Out, fCurrentRate, m_vecInterpolatesAngle);
	}
	void GetPosCurrent(EVector3& v3Out, mu_float fCurrentRate)
	{
		GetCurrentValue(v3Out, fCurrentRate, m_vecInterpolatesPos);
	}

	void GetCurrentValueF(mu_float& fOut, mu_float fCurrentRate, VEC_INTERPOLATES_F& vecInterpolates);
	void GetScaleCurrent(mu_float& fOut, mu_float fCurrentRate)
	{
		GetCurrentValueF(fOut, fCurrentRate, m_vecInterpolatesScale);
	}

	void GetAlphaCurrent(mu_float& fOut, mu_float fCurrentRate)
	{
		GetCurrentValueF(fOut, fCurrentRate, m_vecInterpolatesAlpha);
	}

public:
	void ClearContainer();

	CInterpolateContainer() {};
	~CInterpolateContainer() {};
};

struct EObjectCullingData
{
	mu_uint8 SX, SY;
	mu_uint8 EX, EY;
};

class OBJECT
{
public:
	OBJECT();
	~OBJECT();

	void Initialize();

public:
	enum OBJECT_FLAGS
	{
		eOF_FREE = (1 << 0), // Should always be flag 1
		eOF_LIVE = (1 << 1), // Should always be flag 2
		eOF_BILLBOARD = (1 << 2),
		eOF_COLLISION = (1 << 3),
		eOF_LIGHTENABLE = (1 << 4),
		eOF_ACTIONSTART = (1 << 5),
		eOF_VISIBLE = (1 << 6),
		eOF_ALPHAENABLE = (1 << 7),
		eOF_CHROMEENABLE = (1 << 8),
		eOF_PCROOM = (1 << 9),
		eOF_SECONDANIMATION = (1 << 10),
		eOF_DIRECTIONANIMATION = (1 << 11),
	};
	MUFlagset<mu_uint16>	Flags;

public:
	mu_uint8			AI;
	mu_uint8			ExtState;
	mu_uint8			Teleport;
	mu_uint8			Kind;
	mu_uint8			m_byNumCloth;
	mu_uint8			m_byHurtByOneToOne;
	mu_uint8			WeaponExcOption;
	mu_uint8			WeaponSetOption;
	mu_uint8			DamageTime;
	mu_uint8			m_byBuildTime;
	mu_uint8			m_bySkillSerialNum;

public:
	mu_int8				BlendMesh;
	mu_int8				HiddenMesh;

public:
	mu_uint16			Type;
	mu_uint16			SubType;
	mu_uint16			CurrentAction;
	mu_uint16			PriorAction;
	mu_uint16			Skill;
	mu_uint16			WeaponLevel;

public:
	mu_int16			PKKey;
	mu_int16			Weapon;
	mu_int16			m_sTargetIndex;
	mu_int16			AttackPoint[2];
	mu_int16			LinkBone;
	mu_int16			PlayingSound;

public:
	mu_int32			m_iAnimation;
	mu_int32			LifeTime;
	mu_int32			RenderType;
	mu_int32			InitialSceneFrame;
	mu_int32			AttackSpeed;
	mu_int32			MagicSpeed;

public:
	mu_uint64			m_dwTime;
	mu_uint64			SpecialSkillTick;

public:
	mu_float			Scale;
	mu_float			BlendMeshLight;
	mu_float			BlendMeshTexCoordU;
	mu_float			BlendMeshTexCoordV;
	mu_float			Timer;
	mu_float			m_fEdgeScale;
	mu_float			Velocity;
	mu_float			CollisionRange;
	mu_float			ShadowScale;
	mu_float			Gravity;
	mu_float			Distance;
	mu_float			AlphaTarget;
	mu_float			Alpha;
	mu_float			DirectionSpeed;

public:
	mu_float			AnimationFrame;
	mu_float			PriorAnimationFrame;

public:
	EVector3			Light;
	EVector3			Direction;
	EVector3			m_vPosSword;
	EVector3			StartPosition;
	EVector3			BoundingBoxMin;
	EVector3			BoundingBoxMax;
	EVector3			m_vDownAngle;
	EVector3			m_vDeadPosition;
	EVector3			Position;
	EVector3	 		Angle;
	EVector3	 		HeadAngle;
	EVector3	   		HeadTargetAngle;
	EVector3  			EyeLeft;
	EVector3  			EyeRight;
	EVector3			EyeLeft2;
	EVector3			EyeRight2;
	EVector3			EyeLeft3;
	EVector3			EyeRight3;

public:
	mu_uint64			CreationTime;

public:
	OBB_t				OBB;

public:
	MUBuff					m_BuffMap;

public:
#ifdef USE_DUAL_QUATERNION
	EDualQuat				*BoneTransform;
#else
	EMatrix4				*BoneTransform;
#endif
	CPhysicsCloth			*m_pCloth;
	OBJECT	    			*Owner;
};

class MUObjectExtra
{
public:
	CInterpolateContainer	m_Interpolates;
};

void CreateObject(const mu_uint32 ThreadIndex, mu_uint32 objectIndex, mu_uint32 type, EVector3 position, EVector3 angle, EFloat scale, EObjectCullingData &culling);

extern std::array<OBJECT, MAX_BOIDS> Boids;
extern std::array<OBJECT, MAX_FISHS> Fishs;

extern std::array<mu_uint16, MAX_BOIDS + MAX_TASKING_THREADS> g_BoidsQueue;
extern std::array<mu_uint16, MAX_FISHS + MAX_TASKING_THREADS> g_FishsQueue;

void Alpha(OBJECT *o);

struct BUTTERFLE_DELETE
{
	OBJECT *Owner;
};

extern MUCriticalSectionSpinLock g_ButterflesLock;
extern std::array<OBJECT, MAX_BUTTERFLES> g_Butterfles;
extern std::array<mu_uint16, MAX_BUTTERFLES> g_ButterflesFree;
extern std::array<mu_uint16, MAX_BUTTERFLES + MAX_TASKING_THREADS> g_ButterflesQueue;
extern mu_uint16 g_ButterflesCurCount;
extern mu_uint16 g_ButterflesFreeCount;
extern mu_uint16 g_ButterflesDeleteCount;

void CreateBug(mu_int32 Type, EVector3 Position, OBJECT *Owner, mu_int32 SubType = 0, mu_int32 LinkBone = 0);
void DeleteBug(OBJECT *Owner);
mu_boolean IsBug(ITEM* pItem);
void MoveBug(const mu_uint32 ThreadIndex, OBJECT *o, mu_boolean bForceRender = false);

#endif