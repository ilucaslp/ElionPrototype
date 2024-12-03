#include "stdafx.h"
#include "mu_objectbase.h"

std::array<OBJECT, MAX_BOIDS> Boids;
std::array<OBJECT, MAX_FISHS> Fishs;

std::array<mu_uint16, MAX_BOIDS + MAX_TASKING_THREADS> g_BoidsQueue;
std::array<mu_uint16, MAX_FISHS + MAX_TASKING_THREADS> g_FishsQueue;

void CInterpolateContainer::GetCurrentValue(EVector3& v3Out, mu_float fCurrentRate, VEC_INTERPOLATES& vecInterpolates)
{
	VEC_INTERPOLATES::iterator	iterBegin = vecInterpolates.begin();
	VEC_INTERPOLATES::iterator	iterEnd = vecInterpolates.end();
	VEC_INTERPOLATES::iterator	iter_;

	INTERPOLATE_FACTOR*	pCurFactor = nullptr;
	mu_boolean	bFindInterpolateFactor = false;

	for (iter_ = iterBegin; iter_ < iterEnd; ++iter_)
	{
		INTERPOLATE_FACTOR&	interpolateFactor = (*iter_);

		if (interpolateFactor.fRateStart <= fCurrentRate && interpolateFactor.fRateEnd > fCurrentRate)
		{
			bFindInterpolateFactor = true;
			pCurFactor = &interpolateFactor;
			break;
		}
	}

	if (bFindInterpolateFactor == true)
	{
		VectorInterpolation_F(v3Out,
			pCurFactor->v3Start,
			pCurFactor->v3End,
			pCurFactor->fRateEnd - pCurFactor->fRateStart,
			fCurrentRate - pCurFactor->fRateStart);
	}

}

void CInterpolateContainer::GetCurrentValueF(mu_float& fOut, mu_float fCurrentRate, VEC_INTERPOLATES_F& vecInterpolates)
{
	VEC_INTERPOLATES_F::iterator	iterBegin = vecInterpolates.begin();
	VEC_INTERPOLATES_F::iterator	iterEnd = vecInterpolates.end();
	VEC_INTERPOLATES_F::iterator	iter_;

	INTERPOLATE_FACTOR_F*	pCurFactor = nullptr;
	mu_boolean	bFindInterpolateFactor = false;

	for (iter_ = iterBegin; iter_ < iterEnd; ++iter_)
	{
		INTERPOLATE_FACTOR_F&	interpolateFactor = (*iter_);

		if (interpolateFactor.fRateStart <= fCurrentRate && interpolateFactor.fRateEnd > fCurrentRate)
		{
			bFindInterpolateFactor = true;
			pCurFactor = &interpolateFactor;
			break;
		}
	}

	if (bFindInterpolateFactor == true)
	{
		LInterpolationF(fOut,
			pCurFactor->fStart,
			pCurFactor->fEnd,
			(mu_float)(fCurrentRate - pCurFactor->fRateStart) / (mu_float)(pCurFactor->fRateEnd - pCurFactor->fRateStart));
	}

}

void CInterpolateContainer::ClearContainer()
{
	m_vecInterpolatesPos.clear();
	m_vecInterpolatesAngle.clear();
	m_vecInterpolatesScale.clear();
	m_vecInterpolatesAlpha.clear();
}

OBJECT::OBJECT()
{
	BoneTransform = nullptr;
	m_pCloth = nullptr;
	Initialize();
}

OBJECT::~OBJECT()
{
	if (this->m_pCloth != nullptr)
	{
		CPhysicsCloth *pCloth = this->m_pCloth;
		for (mu_int32 i = 0; i < this->m_byNumCloth; ++i)
		{
			pCloth[i].Destroy();
		}
		delete[] pCloth;
		this->m_pCloth = nullptr;
	}

	BoneTransform = nullptr;
}

void OBJECT::Initialize()
{
	Flags.Clear();

	AI = 0;
	CurrentAction = 0;
	PriorAction = 0;

	ExtState = 0;
	Teleport = 0;
	Kind = 0;
	Skill = 0;
	m_byNumCloth = 0;
	m_byHurtByOneToOne = 0;
	WeaponLevel = 0;
	DamageTime = 0;
	m_byBuildTime = 0;
	SpecialSkillTick = 0;
	m_bySkillSerialNum = 0;

	PKKey = 0;
	Weapon = 0;

	Type = 0;
	SubType = 0;
	m_iAnimation = 0;
	HiddenMesh = -1;
	LifeTime = 0;
	BlendMesh = -1;
	AttackPoint[0] = 0;
	AttackPoint[1] = 0;
	RenderType = 0;
	InitialSceneFrame = 0;
	LinkBone = -1;
	PlayingSound = -1;

	m_dwTime = 0;

	Scale = 0.0f;
	BlendMeshLight = 0.0f;
	BlendMeshTexCoordU = 0.0f;
	BlendMeshTexCoordV = 0.0f;
	Timer = 0.0f;
	m_fEdgeScale = 0.0f;
	Velocity = 0.0f;
	CollisionRange = 0.0f;
	ShadowScale = 0.0f;
	Gravity = 0.0f;
	Distance = 0.0f;
	AnimationFrame = 0.0f;
	PriorAnimationFrame = 0.0f;
	AlphaTarget = 0.0f;
	Alpha = 0.0f;
	DirectionSpeed = 0.0f;

	Light = EVector3();
	Direction = EVector3();
	m_vPosSword = EVector3();
	StartPosition = EVector3();
	BoundingBoxMin = EVector3();
	BoundingBoxMax = EVector3();
	m_vDownAngle = EVector3();
	m_vDeadPosition = EVector3();
	Position = EVector3();
	Angle = EVector3();
	HeadAngle = EVector3();
	HeadTargetAngle = EVector3();
	EyeLeft = EVector3();
	EyeRight = EVector3();
	EyeLeft2 = EVector3();
	EyeRight2 = EVector3();
	EyeLeft3 = EVector3();
	EyeRight3 = EVector3();

	OBB.StartPos = EVector3();
	OBB.XAxis = EVector3();
	OBB.YAxis = EVector3();
	OBB.ZAxis = EVector3();

	MU_SAFE_DELETE(m_pCloth);
	BoneTransform = nullptr;

	Owner = nullptr;
}

void CreateObject(const mu_uint32 ThreadIndex, mu_uint32 objectIndex, mu_uint32 type, EVector3 position, EVector3 angle, EFloat scale, EObjectCullingData &culling)
{
	mu_memcpy(&g_EnvironmentObjectsNodes[objectIndex], &culling, sizeof(culling));

	for (mu_uint32 y = culling.SY; y < culling.EY; ++y)
	{
		mu_uint32 blockIndex = MUCPUCulling::GetBlockIndex(y, culling.SX);
		for (mu_uint32 x = culling.SX; x < culling.EX; ++x, ++blockIndex)
		{
			g_EnvironmentBlocks[blockIndex][g_EnvironmentBlocksCount[blockIndex]++] = objectIndex;
		}
	}

	OBJECT *o = &g_EnvironmentObjects[objectIndex];

	o->Initialize();

	o->Flags.Set(OBJECT::eOF_LIVE | OBJECT::eOF_LIGHTENABLE);

	o->BoneTransform = nullptr;

	o->Type = type;
	o->Scale = scale;
	o->Alpha = 1.0f;
	o->AlphaTarget = 1.0f;
	o->Velocity = 0.16f;
	o->ShadowScale = 50.0f;
	o->HiddenMesh = -1;
	o->CurrentAction = 0;
	o->PriorAction = 0;
	o->AnimationFrame = 0.0f;
	o->PriorAnimationFrame = 0.0f;
	o->BlendMesh = -1;
	o->BlendMeshLight = 1.0f;
	o->BlendMeshTexCoordU = 0.0f;
	o->BlendMeshTexCoordV = 0.0f;
	g_CharacterClearBuff(o);
	o->CollisionRange = -30.0f;
	o->Timer = 0.0f;

	VectorCopy(position, o->Position);
	VectorCopy(position, o->StartPosition);
	VectorCopy(angle, o->Angle);
	Vector(0.0f, 0.0f, 0.0f, o->Light);
	Vector(0.0f, 0.0f, 0.0f, o->HeadAngle);
	Vector(0.0f, 0.0f, 0.0f, o->Direction);
	Vector(-40.0f, -40.0f, 0.0f, o->BoundingBoxMin);
	Vector(40.0f, 40.0f, 80.0f, o->BoundingBoxMax);

	if (g_CurrentScene == LOG_IN_SCENE || g_CurrentScene == CHARACTER_SCENE)
	{
		switch (o->Type)
		{
		case MODEL_SHIP:
			o->Scale = 0.8f;
			Vector(0.2f, 0.2f, 0.2f, o->Light);
			break;
		case MODEL_WAVEBYSHIP:
			o->Scale = 0.8f;
			o->BlendMesh = 0;
			o->BlendMeshLight = 1.0f;
			Vector(1.0f, 1.0f, 1.0f, o->Light);
			break;
		case MODEL_MUGAME:
			o->Scale = 2.2f;
			o->BlendMesh = 1;
			o->Flags.Clear(OBJECT::eOF_LIGHTENABLE);
			break;
		case MODEL_LOGO:
			o->BlendMesh = 1;
			o->BlendMeshLight = 1.0f;
			o->Scale = 5.0f;
			o->Scale = 0.044f;
			Vector(1.0f, 1.0f, 1.0f, o->Light);
			o->Flags.Clear(OBJECT::eOF_LIGHTENABLE);
			break;
		case MODEL_LOGOSUN:
			o->Scale = 3.0f;
			o->BlendMesh = 0;
			Vector(0.5f, 0.5f, 0.5f, o->Light);
			break;
		case MODEL_LOGO + 4:
			o->BlendMesh = 10;
			Vector(1.0f, 1.0f, 1.0f, o->Light);
			break;
		case MODEL_DRAGON:
			o->HiddenMesh = -2;
			Vector(1.0f, 1.0f, 1.0f, o->Light);
			CreateEffect(ThreadIndex, MODEL_DRAGON, o->Position, o->Angle, o->Light, 0, o, -1, 0, 0, 0, 1.6f);
			break;
		}
	}

	TheMapProcess().CreateObject(ThreadIndex, objectIndex, o);
}

void Alpha(OBJECT *o)
{
	if (o->Flags.Get(OBJECT::eOF_ALPHAENABLE) == true)
	{
		if (o->AlphaTarget > o->Alpha)
		{
			o->Alpha += 0.05f;
			if (o->Alpha > 1.0f) o->Alpha = 1.0f;
		}
		else if (o->AlphaTarget < o->Alpha)
		{
			o->Alpha -= 0.05f;
			if (o->Alpha < 0.0f) o->Alpha = 0.0f;
		}
	}
	else
	{
		mu_float alphaInc = (o->AlphaTarget - o->Alpha)*0.1f;
		o->Alpha += alphaInc;

		if(EMath::Abs(alphaInc) < 0.01f && o->Alpha != o->AlphaTarget)
			o->Alpha = o->AlphaTarget;
	}
		
	if (o->BlendMeshLight > o->Alpha)
		o->BlendMeshLight = o->Alpha;
}