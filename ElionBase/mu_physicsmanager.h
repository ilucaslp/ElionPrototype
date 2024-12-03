#ifndef __MU_PHYSICSMANAGER_H__
#define __MU_PHYSICSMANAGER_H__

#pragma once

#define PVS_NORMAL		( 0x00)
#define PVS_FIXEDPOS	( 0x01)

class CPhysicsVertex
{
public:
	CPhysicsVertex();
	virtual ~CPhysicsVertex();
	void Clear();

	static mu_float s_Gravity;
	static mu_float s_fMass;
	static mu_float s_fInvOfMass;

protected:
	EVector3	m_vForce;
	EVector3	m_vVel;
	EVector3	m_vPos;
	mu_uint8	m_byState;

public:
	void Init(EVector3 &vPos, mu_boolean bFixed = false);
	void UpdateForce(const mu_uint32 ThreadIndex, mu_uint32 iKey, mu_uint32 dwType = 0, mu_float fWind = 0.0f);
	void AddToForce(mu_float fXForce, mu_float fYForce, mu_float fZForce);
	void Move(mu_float fTime);

public:
	void GetPosition(EVector3 *pPos);
	EVector3 &GetPosition();
	mu_float GetDistance(CPhysicsVertex *pVertex2, EVector3 *pDistance);

protected:
	mu_int32 m_iCountOneTimeMove;
	EVector3 m_vOneTimeMove;

public:
	mu_boolean KeepLength(CPhysicsVertex *pVertex2, mu_float *pfLength);
	void AddOneTimeMoveToKeepLength(CPhysicsVertex *pVertex2, mu_float fLength);
	void DoOneTimeMove();
	void AddOneTimeMove(EVector3 &vMove);
};

enum ENUM_COLLISION_TYPE
{
	CLT_DEFAULT = 0,
	CLT_SPHERE,
	NUM_CLT
};

class CPhysicsCollision
{
public:
	CPhysicsCollision();
	virtual ~CPhysicsCollision();
	void Clear();

protected:
	EVector3 m_vCenterBeforeTransform;
	mu_int32 m_iBone;
	EVector3 m_vCenter;

public:
	virtual mu_int32 GetType() { return (CLT_DEFAULT); }

	mu_int32 GetBone() { return (m_iBone); }
	void SetPosition(mu_float fXPos, mu_float fYPos, mu_float fZPos);
	void GetCenter(EVector3 &vCenter);
	void GetCenterBeforeTransform(EVector3 &vCenter);
	virtual void ProcessCollision(CPhysicsVertex *pVertex);
};

class CPhysicsColSphere : public CPhysicsCollision
{
public:
	CPhysicsColSphere();
	virtual ~CPhysicsColSphere();
	void Clear();

protected:
	mu_float m_fRadius;

public:
	virtual mu_int32 GetType() { return (CLT_SPHERE); }

	void Init(mu_float fXPos, mu_float fYPos, mu_float fZPos, mu_float fRadius, mu_int32 iBone);
	virtual void ProcessCollision(CPhysicsVertex *pVertex);
	mu_float GetRadius() { return (m_fRadius); }
};

#define PCT_MASK_SHAPE			( 0x00000003)
#define PCT_FLAT				( 0x00000000)
#define PCT_CURVED				( 0x00000001)
#define PCT_STICKED				( 0x00000002)

#define PCT_MASK_SHAPE_EXT		( 0x0000000C)
#define PCT_SHAPE_NORMAL		( 0x00000000)
#define PCT_SHORT_SHOULDER		( 0x00000004)
#define PCT_CYLINDER			( 0x00000008)

#define PCT_MASK_SHAPE_EXT2		( 0x00000030)
#define PCT_SHAPE_HALLOWEEN		( 0x00000010)

#define PCT_MASK_ELASTIC		( 0x00000300)
#define PCT_COTTON				( 0x00000000)
#define PCT_RUBBER				( 0x00000100)
#define PCT_RUBBER2				( 0x00000200)

#define PCT_MASK_WEIGHT			( 0x00000C00)
#define PCT_NORMAL_THICKNESS	( 0x00000000)
#define PCT_HEAVY				( 0x00000400)

#define PCT_MASK_DRAW			( 0x00003000)
#define PCT_MASK_BLIT			( 0x00000000)
#define PCT_MASK_ALPHA			( 0x00001000)
#define PCT_MASK_BLEND          ( 0x00002000)

#define PCT_MASK_ELASTIC_EXT	( 0x0000E000)
#define PCT_ELASTIC_HALLOWEEN	( 0x00004000)
#define PCT_ELASTIC_RAGE_L		( 0x00008000)
#define PCT_ELASTIC_RAGE_R		( 0x0000C000)

#define PCT_OPT_MESHPROG		( 0x10000000)
#define PCT_OPT_CORRECTEDFORCE	( 0x20000000)
#define PCT_MASK_LIGHT          ( 0x40000000)
#define PCT_OPT_HAIR            ( 0x80000000)

#define PLS_NORMAL				( 0x00)
#define PLS_LOOSEDISTANCE		( 0x01)
#define PLS_SPRING				( 0x02)
#define PLS_STRICTDISTANCE		( 0x04)

typedef struct
{
	mu_int16	m_nVertices[2];
	mu_float	m_fDistance[2];
	mu_uint8	m_byStyle;
} St_PhysicsLink;

class CPhysicsCloth
{
public:
	CPhysicsCloth();
	virtual ~CPhysicsCloth();
	void Clear();

protected:
	OBJECT *m_oOwner;
	mu_int32 m_iBone;
	mu_int32 m_iTexFront;
	mu_int32 m_iTexBack;
	mu_uint32 m_dwType;

	mu_float m_fxPos, m_fyPos, m_fzPos;
	mu_float m_fWidth, m_fHeight;
	mu_int32 m_iNumHor, m_iNumVer;
	mu_int32 m_iNumVertices;
	CPhysicsVertex *m_pVertices;
	mu_int32 m_iNumLink;
	St_PhysicsLink *m_pLink;

	mu_float   m_fWind;
	mu_uint8    m_byWindMax;
	mu_uint8    m_byWindMin;

public:
	OBJECT* GetOwner() { return (m_oOwner); }

protected:
	mu_float m_fUnitWidth, m_fUnitHeight;
public:
	mu_boolean Create(const mu_uint32 ThreadIndex, OBJECT *o, mu_int32 iBone, mu_float fxPos, mu_float fyPos, mu_float fzPos, mu_int32 iNumHor, mu_int32 iNumVer, mu_float fWidth, mu_float fHeight, mu_int32 iTexFront, mu_int32 TexBack, mu_uint32 dwType = 0);
	virtual void Destroy();

protected:
#ifdef USE_DUAL_QUATERNION
	virtual void SetFixedVertices(const mu_uint32 ThreadIndex, const EDualQuat &Matrix);
#else
	virtual void SetFixedVertices(const mu_uint32 ThreadIndex, const EMatrix4 &Matrix);
#endif
	virtual void NotifyVertexPos(mu_int32 iVertex, EVector3 vPos) {}
	void SetLink(mu_int32 iLink, mu_int32 iVertex1, mu_int32 iVertex2, mu_float fDistanceSmall, mu_float fDistanceLarge, mu_uint8 byStyle);

public:
	mu_boolean Move2(const mu_uint32 ThreadIndex, mu_float fTime, mu_int32 iCount);
	mu_boolean Move(const mu_uint32 ThreadIndex, mu_float fTime);

protected:
	virtual void InitForces(const mu_uint32 ThreadIndex);
	void MoveVertices(const mu_uint32 ThreadIndex, mu_float fTime);
	mu_boolean PreventFromStretching(const mu_uint32 ThreadIndex);

public:
	virtual void Render(const mu_uint32 ThreadIndex, MUModel *b, EVector3 *pvColor = nullptr);

protected:
	void RenderFace(mu_boolean bFront, mu_int32 iTexture, SHClothVertexData *verticesBuffer);
	void RenderVertex(SHClothVertexData *verticesBuffer, mu_int32 xVertex, mu_int32 yVertex);

protected:
	std::vector<CPhysicsCollision*> m_lstCollision;

public:
	void SetWindMinMax(mu_uint8 Min, mu_uint8 Max) { m_byWindMin = Min; m_byWindMax = Max; }
	void AddCollisionSphere(mu_float fXPos, mu_float fYPos, mu_float fZPos, mu_float fRadius, mu_int32 iBone);
	void ProcessCollision();
};

class CPhysicsClothMesh : public CPhysicsCloth
{
public:
	CPhysicsClothMesh();
	virtual ~CPhysicsClothMesh();
	virtual void Clear();

protected:
	mu_int32 m_iMesh;
	mu_int32 m_iBoneForFixed;
	mu_int32 m_iBMDType;

public:
	mu_boolean Create(const mu_uint32 ThreadIndex, OBJECT *o, mu_int32 iMesh, mu_int32 iBone, mu_uint32 dwType = 0, mu_int32 iBMDType = -1);
	mu_boolean Create(const mu_uint32 ThreadIndex, OBJECT *o, mu_int32 iMesh, mu_int32 iBone, mu_float fxPos, mu_float fyPos, mu_float fzPos, mu_int32 iNumHor, mu_int32 iNumVer, mu_float fWidth, mu_float fHeight, mu_int32 iTexFront, mu_int32 TexBack, mu_uint32 dwType = 0, mu_int32 iBMDType = -1);

protected:
	mu_uint32 FindMatchVertex(Model::Mesh1 *pMesh, mu_int32 iV1, mu_int32 iV2, mu_int32 iV3);
	mu_boolean FindInLink(mu_int32 iCount, mu_int32 iV1, mu_int32 iV2);
#ifdef USE_DUAL_QUATERNION
	virtual void SetFixedVertices(const mu_uint32 ThreadIndex, const EDualQuat &Matrix);
#else
	virtual void SetFixedVertices(const mu_uint32 ThreadIndex, const EMatrix4 &Matrix);
#endif
	virtual void NotifyVertexPos(mu_int32 iVertex, EVector3 vPos);
	virtual void InitForces(const mu_uint32 ThreadIndex);

public:
	virtual void Render(const mu_uint32 ThreadIndex, MUModel *b, EVector3 *pvColor = nullptr);
};

namespace CPhysicsManager
{
	extern mu_float s_fWindBase;
	extern mu_float s_fWind[MAX_TASKING_THREADS];
	extern EVector3 s_vWind[MAX_TASKING_THREADS];
};

#endif