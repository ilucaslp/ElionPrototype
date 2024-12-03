#include "stdafx.h"
#include "mu_physicsmanager.h"

#define RATE_SHORT_SHOULDER		( 0.6f)

mu_float CPhysicsVertex::s_Gravity = 9.8f;
mu_float CPhysicsVertex::s_fMass = 0.0025f;
mu_float CPhysicsVertex::s_fInvOfMass = 400.0f;

mu_float CPhysicsManager::s_fWindBase = 0.0f;
mu_float CPhysicsManager::s_fWind[MAX_TASKING_THREADS] = { 0.0f };
EVector3 CPhysicsManager::s_vWind[MAX_TASKING_THREADS] = { { 0.0f, 0.0f, 0.0f } };

CPhysicsVertex::CPhysicsVertex()
{
	Clear();
}

CPhysicsVertex::~CPhysicsVertex()
{
}

void CPhysicsVertex::Clear()
{
	m_iCountOneTimeMove = 0;
	for (mu_int32 i = 0; i < 3; ++i)
	{
		m_vForce[i] = m_vVel[i] = m_vPos[i] = m_vOneTimeMove[i] = 0.0f;
	}
	m_byState = PVS_NORMAL;
}

void CPhysicsVertex::Init(EVector3 &vPos, mu_boolean bFixed)
{
	m_vPos = vPos;
	if (bFixed)
	{
		m_byState |= PVS_FIXEDPOS;
	}
}

void CPhysicsVertex::UpdateForce(const mu_uint32 ThreadIndex, mu_uint32 iKey, mu_uint32 dwType, mu_float fWind)
{
	if (PVS_FIXEDPOS & m_byState)
	{
		m_vForce[0] = m_vForce[1] = m_vForce[2] = 0.0f;
		return;
	}

	mu_float fGravityRate = 1.0f;

	mu_int32 iTemp = EMath::Min(EMath::Max(0u, 5u - iKey), 4u);
	mu_float fRand = (mu_float)(iTemp == 0 ? 0 : iTemp + 2);

	for (mu_int32 i = 0; i < 3; ++i)
	{
		m_vForce[i] = fRand * CPhysicsManager::s_vWind[ThreadIndex][i] - m_vVel[i] * 0.01f;
	}

	switch (PCT_MASK_ELASTIC & dwType)
	{
	case PCT_RUBBER:
		m_vForce[2] += fRand * (fWind + 0.1f) * 1.0f;
		break;
	case PCT_RUBBER2:
		m_vForce[2] += fRand * (fWind);
		break;
	}

	switch (PCT_MASK_ELASTIC_EXT & dwType)
	{
	case PCT_ELASTIC_HALLOWEEN:
		m_vForce[0] += -(fRand * fWind * 0.5f);
		m_vForce[2] += fRand * (fWind + 0.1f) * 0.5f * (mu_float)EMath::Sin(FWorldTime*0.003f) * 5.0f;
		m_vForce[2] -= s_Gravity * fGravityRate * s_fMass * 50.0f;
		break;
	case PCT_ELASTIC_RAGE_L:
		m_vForce[0] += -(fRand * fWind * 0.8f);
		break;
	case PCT_ELASTIC_RAGE_R:
		m_vForce[0] -= -(fRand * fWind * 0.8f);
		break;
	}

	switch (PCT_MASK_WEIGHT & dwType)
	{
	case PCT_HEAVY:
		m_vForce[2] -= s_Gravity * fGravityRate * s_fMass * 180.0f;
		break;
	default:
		m_vForce[2] -= s_Gravity * fGravityRate * s_fMass * 100.0f;
		break;
	}
}

void CPhysicsVertex::AddToForce(mu_float fXForce, mu_float fYForce, mu_float fZForce)
{
	m_vForce[0] += fXForce;
	m_vForce[1] += fYForce;
	m_vForce[2] += fZForce;
}

void CPhysicsVertex::Move(mu_float fTime)
{
	if (PVS_FIXEDPOS & m_byState)
	{
		return;
	}

	for (mu_int32 i = 0; i < 3; ++i)
	{
		m_vVel[i] += m_vForce[i] * s_fInvOfMass * fTime;
		m_vPos[i] += m_vVel[i] * fTime;
	}
}

void CPhysicsVertex::GetPosition(EVector3 *pPos)
{
	*pPos = m_vPos;
}

EVector3 &CPhysicsVertex::GetPosition()
{
	return m_vPos;
}

mu_float CPhysicsVertex::GetDistance(CPhysicsVertex *pVertex2, EVector3 *pDistance)
{
	*pDistance = m_vPos - pVertex2->m_vPos;

	return (VectorLength((*pDistance)));
}

mu_boolean CPhysicsVertex::KeepLength(CPhysicsVertex *pVertex2, mu_float *pfLength)
{
	if (PVS_FIXEDPOS & m_byState)
	{
		return true;
	}

	EVector3 vDistance;
	mu_float fDistance = EMath::Max(0.001f, GetDistance(pVertex2, &vDistance));

	if (fDistance > pfLength[1] * 20.0f)
	{
		return false;
	}

	if (fDistance > pfLength[1])
	{
		VectorScale(vDistance, (fDistance - pfLength[1]) / fDistance, vDistance);
		VectorSubtract(m_vPos, vDistance, m_vPos);
	}
	else if (fDistance < pfLength[0])
	{
		VectorScale(vDistance, (fDistance - pfLength[0]) / fDistance, vDistance);
		VectorSubtract(m_vPos, vDistance, m_vPos);
	}

	return true;
}

void CPhysicsVertex::AddOneTimeMoveToKeepLength(CPhysicsVertex *pVertex2, mu_float fLength)
{
	EVector3 vDistance;
	mu_float fDistance = EMath::Max(0.001f, GetDistance(pVertex2, &vDistance));
	VectorScale(vDistance, (fDistance - fLength) *0.5f / fDistance, vDistance);
	VectorSubtract(m_vOneTimeMove, vDistance, m_vOneTimeMove);
	VectorAdd(pVertex2->m_vOneTimeMove, vDistance, pVertex2->m_vOneTimeMove);
	++m_iCountOneTimeMove;
	++pVertex2->m_iCountOneTimeMove;
}

void CPhysicsVertex::DoOneTimeMove()
{
	if (PVS_FIXEDPOS & m_byState)
	{
		m_vOneTimeMove[0] = m_vOneTimeMove[1] = m_vOneTimeMove[2] = 0.0f;
		return;
	}

	if (m_iCountOneTimeMove > 0)
	{
		for (mu_int32 i = 0; i < 3; ++i)
		{
			m_vOneTimeMove[i] /= (mu_float)(m_iCountOneTimeMove);
		}
		VectorAdd(m_vPos, m_vOneTimeMove, m_vPos);
		m_vOneTimeMove[0] = m_vOneTimeMove[1] = m_vOneTimeMove[2] = 0.0f;
		m_iCountOneTimeMove = 0;
	}
}

void CPhysicsVertex::AddOneTimeMove(EVector3 &vMove)
{
	VectorAdd(m_vOneTimeMove, vMove, m_vOneTimeMove);
	++m_iCountOneTimeMove;
}

CPhysicsCollision::CPhysicsCollision()
{
	Clear();
}

CPhysicsCollision::~CPhysicsCollision()
{
}

void CPhysicsCollision::Clear()
{
	Vector(0.0f, 0.0f, 0.0f, m_vCenterBeforeTransform);
	m_iBone = 0;
	Vector(0.0f, 0.0f, 0.0f, m_vCenter);
}

void CPhysicsCollision::SetPosition(mu_float fXPos, mu_float fYPos, mu_float fZPos)
{
	m_vCenter[0] = fXPos;
	m_vCenter[1] = fYPos;
	m_vCenter[2] = fZPos;
}

void CPhysicsCollision::GetCenter(EVector3 &vCenter)
{
	vCenter = m_vCenter;
}

void CPhysicsCollision::GetCenterBeforeTransform(EVector3 &vCenter)
{
	vCenter = m_vCenterBeforeTransform;
}

void CPhysicsCollision::ProcessCollision(CPhysicsVertex *pVertex)
{
}

CPhysicsColSphere::CPhysicsColSphere()
{
	Clear();
}

CPhysicsColSphere::~CPhysicsColSphere()
{
}

void CPhysicsColSphere::Clear()
{
	CPhysicsCollision::Clear();
	m_fRadius = 0.0f;
}

void CPhysicsColSphere::Init(mu_float fXPos, mu_float fYPos, mu_float fZPos, mu_float fRadius, mu_int32 iBone)
{
	m_vCenterBeforeTransform[0] = fXPos;
	m_vCenterBeforeTransform[1] = fYPos;
	m_vCenterBeforeTransform[2] = fZPos;
	m_fRadius = fRadius;
	m_iBone = iBone;
}

void CPhysicsColSphere::ProcessCollision(CPhysicsVertex *pVertex)
{
	EVector3 vPos;
	pVertex->GetPosition(&vPos);
	VectorSubtract(vPos, m_vCenter, vPos);

	mu_float fLength = VectorLength(vPos);
	if (fLength < 0.01f)
	{
		fLength = 0.01f;
		Vector(fLength, 0.0f, 0.0f, vPos);
	}
	if (fLength < m_fRadius)
	{
		VectorScale(vPos, (m_fRadius - fLength) / fLength, vPos);
		pVertex->AddOneTimeMove(vPos);
	}
}

CPhysicsCloth::CPhysicsCloth()
{
	Clear();
}

CPhysicsCloth::~CPhysicsCloth()
{
}

void CPhysicsCloth::Clear()
{
	m_oOwner = nullptr;
	m_iBone = 0;
	m_iTexFront = m_iTexBack = 0;
	m_dwType = 0;
	m_fxPos = m_fyPos = m_fzPos = 0.0f;
	m_fWidth = m_fHeight = 1.0f;
	m_fUnitWidth = m_fUnitHeight = 1.0f;

	m_iNumHor = 0;
	m_iNumVer = 0;
	m_iNumVertices = 0;
	m_pVertices = nullptr;
	m_iNumLink = 0;
	m_pLink = nullptr;

	m_byWindMax = 1;
	m_byWindMin = 1;
}

mu_boolean CPhysicsCloth::Create(const mu_uint32 ThreadIndex, OBJECT *o, mu_int32 iBone, mu_float fxPos, mu_float fyPos, mu_float fzPos, mu_int32 iNumHor, mu_int32 iNumVer, mu_float fWidth, mu_float fHeight, mu_int32 iTexFront, mu_int32 iTexBack, mu_uint32 dwType)
{
	assert(iNumHor > 1 && iNumVer > 1);

	m_oOwner = o;
	m_iBone = iBone;
	m_iTexFront = iTexFront;
	m_iTexBack = iTexBack;
	m_dwType |= dwType;
	m_fxPos = fxPos;
	m_fyPos = fyPos;
	m_fzPos = fzPos;

	MU_SAFE_DELETE_ARRAY(m_pVertices);
	MU_SAFE_DELETE_ARRAY(m_pLink);

	m_fWidth = fWidth;
	m_fHeight = fHeight;
	m_iNumHor = iNumHor;
	m_iNumVer = iNumVer;
	m_iNumVertices = m_iNumHor * m_iNumVer;
	m_pVertices = new CPhysicsVertex[m_iNumVertices];

	m_iNumLink = 2 * ((m_iNumHor - 1) * m_iNumVer + m_iNumHor * (m_iNumVer - 1));
	m_pLink = new St_PhysicsLink[m_iNumLink];

	m_fUnitWidth = m_fWidth / (mu_float)(m_iNumHor - 1);
	m_fUnitHeight = m_fHeight / (mu_float)(m_iNumVer - 1);

	MUModel *b = &MODELS::Data[m_oOwner->Type];
	mu_int32 iLink = 0;
#ifdef USE_DUAL_QUATERNION
	EDualQuat Matrix(m_oOwner->Angle, m_oOwner->BoneTransform != nullptr ? m_oOwner->BoneTransform[b->GetBoneIndex(m_iBone)].Position : EVector3());
#else
	EMatrix4 Matrix;
	AngleMatrix(m_oOwner->Angle, Matrix);
	if (m_oOwner->BoneTransform)
	{
		Matrix[3] = m_oOwner->BoneTransform[b->GetBoneIndex(m_iBone][3)];
	}
#endif

	mu_boolean bCylinder = false;

	for (mu_int32 j = 0; j < m_iNumVer; ++j)
	{
		mu_float fWidth = m_fWidth;
		mu_float fUnitWidth = m_fUnitWidth;
		mu_float fUnitHeight = m_fUnitHeight;

		switch (m_dwType & PCT_MASK_SHAPE_EXT)
		{
		case PCT_SHORT_SHOULDER:
			fWidth *= RATE_SHORT_SHOULDER + (1.0f - RATE_SHORT_SHOULDER) * (mu_float)j / (mu_float)(m_iNumVer - 1);
			fUnitWidth = fWidth / (mu_float)(m_iNumHor - 1);
			break;
		case PCT_CYLINDER:
			bCylinder = true;
			break;
		}

		for (mu_int32 i = 0; i < m_iNumHor; ++i)
		{
			mu_int32 iVertex = m_iNumHor * j + i;

			EVector3 vPos;
			if (bCylinder)
			{
				mu_float fPosY = EMath::Sin((mu_float)i / (mu_float)(m_iNumHor - 1)*3.14f)*100.0f;

				vPos[0] = (fUnitWidth * (mu_float)i);
				vPos[1] = fPosY - 30.0f;
				vPos[2] = (fUnitHeight * (mu_float)j);
			}
			else
			{
				vPos[0] = (fUnitWidth * (mu_float)i) - 0.5f * fWidth;
				vPos[1] = 20.0f;
				vPos[2] = -fUnitHeight * (mu_float)j;
			}

			switch (PCT_MASK_SHAPE & m_dwType)
			{
			case PCT_CURVED:
				{
					mu_float fMove = 2.0f * (mu_float)EMath::Abs((mu_float)i / (mu_float)(m_iNumHor - 1) - 0.5f);
					vPos[1] -= 10.0f * fMove * fMove;
				}
				break;
			case PCT_STICKED:
				vPos[1] = 0.0f;
				break;
			}

			NotifyVertexPos(iVertex, vPos);

			EVector3 vPos2;
			if (m_oOwner->BoneTransform)
			{
				b->TransformPosition(ThreadIndex, Matrix, vPos, vPos2, true);
			}
			else
			{
				VectorCopy(vPos, vPos2);
			}
			m_pVertices[iVertex].Init(vPos2);
		}
	}

	mu_uint8 byVerLinkStyle = 0;
	mu_uint8 byCrossLinkStyle = 0;
	switch (PCT_MASK_ELASTIC & m_dwType)
	{
	case PCT_RUBBER:
		break;
	default:
		byVerLinkStyle |= PLS_STRICTDISTANCE;
		byCrossLinkStyle |= PLS_LOOSEDISTANCE;
		break;
	}

	for (mu_int32 j = 0; j < m_iNumVer; ++j)
	{
		for (mu_int32 i = 0; i < m_iNumHor; ++i)
		{
			EVector3 vTemp;
			mu_int32 iVertex = m_iNumHor * j + i;

			if (j < m_iNumVer - 1)
			{
				mu_float fDist = m_pVertices[iVertex].GetDistance(&m_pVertices[iVertex + m_iNumHor], &vTemp);
				SetLink(iLink++, iVertex, iVertex + m_iNumHor, fDist*0.8f, fDist, PLS_SPRING | byVerLinkStyle);
			}
			if (i < m_iNumHor - 1)
			{
				mu_float fDist = m_pVertices[iVertex].GetDistance(&m_pVertices[iVertex + 1], &vTemp);
				SetLink(iLink++, iVertex, iVertex + 1, fDist*0.8f, fDist, PLS_SPRING | PLS_LOOSEDISTANCE);

				if (j < m_iNumVer - 1)
				{
					mu_float fDist = m_pVertices[iVertex].GetDistance(&m_pVertices[iVertex + 1 + m_iNumHor], &vTemp);
					SetLink(iLink++, iVertex, iVertex + 1 + m_iNumHor, fDist*0.8f, fDist, byCrossLinkStyle);
				}

				if (j > 1)
				{
					mu_float fDist = m_pVertices[iVertex].GetDistance(&m_pVertices[iVertex + 1 - m_iNumHor], &vTemp);
					SetLink(iLink++, iVertex, iVertex + 1 - m_iNumHor, fDist*0.8f, fDist, byCrossLinkStyle);
				}
			}
		}
	}
	m_iNumLink = iLink;
	SetFixedVertices(ThreadIndex, Matrix);

	return true;
}

void CPhysicsCloth::Destroy()
{
	m_lstCollision.clear();

	MU_SAFE_DELETE_ARRAY(m_pLink);
	MU_SAFE_DELETE_ARRAY(m_pVertices);

	Clear();
}

#ifdef USE_DUAL_QUATERNION
void CPhysicsCloth::SetFixedVertices(const mu_uint32 ThreadIndex, const EDualQuat &Matrix)
#else
void CPhysicsCloth::SetFixedVertices(const mu_uint32 ThreadIndex, const EMatrix4 &Matrix)
#endif
{
	MUModel *b = &MODELS::Data[m_oOwner->Type];
	mu_boolean bCylinder = false;
	for (mu_int32 iVertex = 0; iVertex < m_iNumHor; ++iVertex)
	{
		mu_float fWidth = m_fWidth;
		mu_float fUnitWidth = m_fUnitWidth;
		switch (PCT_MASK_SHAPE_EXT & m_dwType)
		{
		case PCT_SHORT_SHOULDER:
			fWidth *= RATE_SHORT_SHOULDER;
			fUnitWidth = fWidth / (mu_float)(m_iNumHor - 1);
			break;
		case PCT_CYLINDER:
			bCylinder = true;
			break;
		}

		EVector3 vPos = { (fUnitWidth * (mu_float)iVertex) - 0.5f * fWidth, 10.0f, 0.0f };

		vPos[0] += m_fxPos;
		if (bCylinder)
		{
			mu_float fPosY = EMath::Sin((mu_float)iVertex / (mu_float)(m_iNumHor - 1)*3.14f)*130.0f;
			vPos[1] = fPosY - 110.0f;
		}
		else
		{
			vPos[1] = m_fyPos;
		}
		vPos[2] = m_fzPos;

		switch (PCT_MASK_SHAPE_EXT2 & m_dwType)
		{
		case PCT_SHAPE_HALLOWEEN:
			vPos[0] += 5.0f;
			vPos[1] += 10.0f;
			break;
		}

		switch (PCT_MASK_SHAPE & m_dwType)
		{
		case PCT_CURVED:
			mu_float fMove = 2.0f * (mu_float)EMath::Abs((mu_float)iVertex / (mu_float)(m_iNumHor - 1) - 0.5f);
			vPos[1] -= 10.0f * fMove * fMove;
			break;
		}

		vPos = EVector3(vPos[2], -vPos[1], vPos[0]);

		EVector3 vPos2;
		b->TransformPosition(ThreadIndex, Matrix, vPos, vPos2, true);
		m_pVertices[iVertex].Init(vPos2, true);
	}
}

void CPhysicsCloth::SetLink(mu_int32 iLink, mu_int32 iVertex1, mu_int32 iVertex2, mu_float fDistanceSmall, mu_float fDistanceLarge, mu_uint8 byStyle)
{
	m_pLink[iLink].m_nVertices[0] = iVertex1;
	m_pLink[iLink].m_nVertices[1] = iVertex2;
	m_pLink[iLink].m_fDistance[0] = fDistanceSmall;
	m_pLink[iLink].m_fDistance[1] = fDistanceLarge;
	m_pLink[iLink].m_byStyle = byStyle;
}

mu_boolean CPhysicsCloth::Move2(const mu_uint32 ThreadIndex, mu_float fTime, mu_int32 iCount)
{
	for (mu_int32 i = 0; i < iCount; ++i)
	{
		if (!Move(ThreadIndex, fTime))
		{
			return false;
		}
	}

	return true;
}

mu_boolean CPhysicsCloth::Move(const mu_uint32 ThreadIndex, mu_float fTime)
{
	switch (PCT_MASK_ELASTIC & m_dwType)
	{
	case PCT_RUBBER2:
		m_fWind = ((GetLargeRand((mu_int32)m_byWindMax) + m_byWindMin) / 100.0f);
		break;
	default:
		m_fWind = CPhysicsManager::s_fWind[ThreadIndex];
		break;
	}

	switch (PCT_MASK_SHAPE_EXT & m_dwType)
	{
	case PCT_CYLINDER:
		m_fWind = (mu_float)(GetLargeRand(10) + 25) / 50.0f;
		break;
	}

	if (m_oOwner == nullptr)
		return false;

	CPhysicsManager::s_vWind[ThreadIndex][0] = m_fWind * EMath::Sin((180.0f + m_oOwner->Angle[2])*Q_PI / 180.0f);
	CPhysicsManager::s_vWind[ThreadIndex][1] = -m_fWind * EMath::Cos((180.0f + m_oOwner->Angle[2])*Q_PI / 180.0f);

	MoveVertices(ThreadIndex, fTime);

	if (!PreventFromStretching(ThreadIndex))
	{
		return false;
	}

	return true;
}

void CPhysicsCloth::InitForces(const mu_uint32 ThreadIndex)
{
	mu_int32 iSeed = ((MoveSceneFrame / 10) * 101) % m_iNumVertices;

	for (mu_int32 iVertex = 0; iVertex < m_iNumVertices; ++iVertex)
	{
		m_pVertices[iVertex].UpdateForce(ThreadIndex, EMath::Abs(iSeed % m_iNumHor - iVertex % m_iNumHor) + EMath::Abs(iSeed / m_iNumHor - iVertex / m_iNumHor), m_dwType, m_fWind);
	}
}

void CPhysicsCloth::MoveVertices(const mu_uint32 ThreadIndex, mu_float fTime)
{
	InitForces(ThreadIndex);

	for (mu_int32 iLink = 0; iLink < m_iNumLink; ++iLink)
	{
		St_PhysicsLink *pLink = &m_pLink[iLink];
		if (pLink->m_byStyle & PLS_SPRING)
		{
			CPhysicsVertex *pVertex1 = &m_pVertices[pLink->m_nVertices[0]];
			CPhysicsVertex *pVertex2 = &m_pVertices[pLink->m_nVertices[1]];

			EVector3 vDistance;
			mu_float fDistance = EMath::Max(0.001f, pVertex1->GetDistance(pVertex2, &vDistance));
			if (fDistance > pLink->m_fDistance[1] + 0.01f)
			{
				EVector3 vForce;
				for (mu_int32 i = 0; i < 3; ++i)
				{
					vForce[i] = (fDistance - pLink->m_fDistance[1]) * vDistance[i] / fDistance;
					if (PCT_OPT_CORRECTEDFORCE & m_dwType)
					{
						vForce[i] *= (pLink->m_fDistance[1] / 32.0f);
					}
					switch (PCT_MASK_ELASTIC & m_dwType)
					{
					case PCT_RUBBER:
						vForce[i] *= 3.0f;
						break;
					}

					switch (PCT_MASK_ELASTIC_EXT & m_dwType)
					{
					case PCT_ELASTIC_HALLOWEEN:
						vForce[i] *= 2.0f;
						break;
					case PCT_ELASTIC_RAGE_L:
					case PCT_ELASTIC_RAGE_R:
						{
							vForce[i] *= 1.0f;
						}
						break;
					}

				}
				pVertex1->AddToForce(-vForce[0], -vForce[1], -vForce[2]);
				pVertex2->AddToForce(vForce[0], vForce[1], vForce[2]);
			}
		}
	}

	MUModel *b = &MODELS::Data[m_oOwner->Type];
	SetFixedVertices(ThreadIndex, m_oOwner->BoneTransform[b->GetBoneIndex(m_iBone)]);
	for (mu_int32 iVertex = 0; iVertex < m_iNumVertices; ++iVertex)
	{
		m_pVertices[iVertex].Move(fTime);
	}
}

mu_boolean CPhysicsCloth::PreventFromStretching(const mu_uint32 ThreadIndex)
{
	MUModel *b = &MODELS::Data[m_oOwner->Type];
	for (auto it = m_lstCollision.begin(); it != m_lstCollision.end(); ++it)
	{
		CPhysicsCollision *pCollision = *it;

		EVector3 vPos;
		pCollision->GetCenterBeforeTransform(vPos);
		vPos = EVector3(vPos[2], -vPos[1], vPos[0]);
		EVector3 vPos2;
		b->TransformPosition(ThreadIndex, m_oOwner->BoneTransform[b->GetBoneIndex(pCollision->GetBone())], vPos, vPos2, true);
		pCollision->SetPosition(vPos2[0], vPos2[1], vPos2[2]);
	}

	ProcessCollision();

	for (mu_int32 iLink = 0; iLink < m_iNumLink; ++iLink)
	{
		St_PhysicsLink *pLink = &m_pLink[iLink];
		if (pLink->m_byStyle & PLS_LOOSEDISTANCE)
		{
			CPhysicsVertex *pVertex1 = &m_pVertices[pLink->m_nVertices[0]];
			CPhysicsVertex *pVertex2 = &m_pVertices[pLink->m_nVertices[1]];

			pVertex1->AddOneTimeMoveToKeepLength(pVertex2, pLink->m_fDistance[1]);
		}
	}

	for (mu_int32 j = 0; j < m_iNumVer; ++j)
	{
		for (mu_int32 i = 0; i < m_iNumHor; ++i)
		{
			mu_int32 iVertex = m_iNumHor * j + i;
			m_pVertices[iVertex].DoOneTimeMove();
		}
	}

	for (mu_int32 iLink = 0; iLink < m_iNumLink; ++iLink)
	{
		St_PhysicsLink *pLink = &m_pLink[iLink];
		if (pLink->m_nVertices[1] >= m_iNumHor &&
			(pLink->m_byStyle & PLS_STRICTDISTANCE))
		{
			CPhysicsVertex *pVertex1 = &m_pVertices[pLink->m_nVertices[0]];
			CPhysicsVertex *pVertex2 = &m_pVertices[pLink->m_nVertices[1]];

			if (!pVertex2->KeepLength(pVertex1, pLink->m_fDistance))
			{
				return false;
			}
		}
	}

	return true;
}

void CPhysicsCloth::Render(const mu_uint32 ThreadIndex, MUModel *b, EVector3 *pvColor)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	mu_uint32 verticesCount;
	mu_uint32 verticesIndex;
	mu_uint32 verticesBufferCount;

	verticesCount = (m_iNumVer - 1) * (m_iNumHor - 1) * 6;
	if (m_iTexFront != m_iTexBack &&
		((PCT_MASK_DRAW&m_dwType) != PCT_MASK_BLEND || !(PCT_MASK_LIGHT&m_dwType)))
		verticesBufferCount = verticesCount * 2;
	else
		verticesBufferCount = verticesCount;

	verticesIndex = g_Tasking._DynamicVerticesCount.fetch_add(verticesBufferCount);

	SHClothVertexData *verticesBuffer = &g_Tasking._DynamicClothBuffer[verticesIndex];

	mu_uint32 instanceIndex = g_Tasking._ModelInstanceCount.fetch_add(1);
	SHModelInstance *instanceData = g_Tasking.GetModelInstanceBuffer(instanceIndex);

	mu_float boneScale = (ThreadInfo.BoneScale - 1.0f);

	SetModelInstance(instanceData,
		EVector3(0.0f, 0.0f, 0.0f),
		EVector2(1.0f, boneScale),
		EVector2(0.0f, ThreadInfo.MinAlpha),
		EVector4(0.0f, 0.0f, ThreadInfo.EnableShadow, ThreadInfo.ShadowHeight),
		ThreadInfo.RenderLight,
		ThreadInfo.ShadowLight,
		0);

	mu_uint32 blendType;

	switch (PCT_MASK_DRAW & m_dwType)
	{
	case PCT_MASK_BLIT:
	case PCT_MASK_ALPHA:
		blendType = BLEND::TXB_ALPHATEST_BEGIN + ThreadInfo.AlphaTestIndex;
		break;
	case PCT_MASK_BLEND:
	default:
		blendType = BLEND::TXB_BLEND01;
		break;
	}

	if (pvColor)
	{
		ThreadInfo.RenderLight = MU::Float4ToARGB(*pvColor, 1.0f);
	}
	else
	{
		ThreadInfo.RenderLight = MU::Float4ToARGB(1.0f, 1.0f, 1.0f, 1.0f);
	}

	if (PCT_MASK_LIGHT&m_dwType)
	{
		mu_float Lum = EMath::Sin(FWorldTime*0.001f)*0.1f + 0.4f;
		mu_float Lum2;
		EVector3 Light;
		mu_int32 iOffset = 0;
		mu_float fScale = 0.0f;

		if(MU_UPDATECOUNTER > 0)
		{
			for (mu_int32 i = 0; i < m_iNumHor; ++i)
			{
				for (mu_int32 j = 0; j < m_iNumVer; ++j)
				{
					fScale = i*0.2f + 0.4f;
					Lum2 = Lum*(m_iNumHor - i) / 5.0f;

					Vector(Lum2, Lum2, Lum2, Light);
					CreateSprite(BITMAP_LIGHT, m_pVertices[iOffset].GetPosition(), fScale, Light, nullptr);
					++iOffset;
				}
			}
		}

		ThreadInfo.RenderLight = MU::Float4ToARGB(Lum, Lum, Lum, 1.0f);
		blendType = BLEND::TXB_BLEND01;
	}

	if (m_iTexFront == m_iTexBack)
	{
		RenderFace(true, m_iTexFront, verticesBuffer);
		verticesBuffer += verticesCount;

		{
			mu_uint32 renderSlotIndex = g_ClothRenderSlotCount.fetch_add(1);
			MUClothRenderSlot *renderSlot = &g_ClothRenderSlot[renderSlotIndex];
			g_ClothRenderSlotUsed[renderSlotIndex] = renderSlotIndex;

			renderSlot->HighHash = RetrieveClothHighHash(0, 0, m_iTexFront);
			renderSlot->LowHash = RetrieveClothLowHash(RT_TEXTURE, blendType, 0, 0);

			renderSlot->EnableShadow = ThreadInfo.EnableShadow;

			renderSlot->CulledType = ThreadInfo.CulledType;
			renderSlot->RenderIndex = 0;
			renderSlot->MeshIndex = 0;
			renderSlot->RenderType = RT_TEXTURE;
			renderSlot->BlendType = blendType;
			renderSlot->ExtraFlag = REXF_ENABLED_DEPTH;

			renderSlot->ModelIndex = 0;
			renderSlot->Texture = m_iTexFront;

			renderSlot->BaseIndex = verticesIndex;
			renderSlot->IndexCount = verticesCount;
			renderSlot->InstanceIndex = instanceIndex;
		}
	}
	else
	{
		RenderFace(true, m_iTexFront, verticesBuffer);
		verticesBuffer += verticesCount;

		{
			mu_uint32 renderSlotIndex = g_ClothRenderSlotCount.fetch_add(1);
			MUClothRenderSlot *renderSlot = &g_ClothRenderSlot[renderSlotIndex];
			g_ClothRenderSlotUsed[renderSlotIndex] = renderSlotIndex;

			renderSlot->HighHash = RetrieveClothHighHash(0, 0, m_iTexFront);
			renderSlot->LowHash = RetrieveClothLowHash(RT_TEXTURE, blendType, 0, 0);

			renderSlot->EnableShadow = ThreadInfo.EnableShadow;

			renderSlot->CulledType = ThreadInfo.CulledType;
			renderSlot->RenderIndex = 0;
			renderSlot->MeshIndex = 0;
			renderSlot->RenderType = RT_TEXTURE;
			renderSlot->BlendType = blendType;
			renderSlot->ExtraFlag = REXF_ENABLED_DEPTH | REXF_CULL_FRONT;

			renderSlot->ModelIndex = 0;
			renderSlot->Texture = m_iTexFront;

			renderSlot->BaseIndex = verticesIndex;
			renderSlot->IndexCount = verticesCount;
			renderSlot->InstanceIndex = instanceIndex;
		}

		if ((PCT_MASK_DRAW&m_dwType) != PCT_MASK_BLEND || !(PCT_MASK_LIGHT&m_dwType))
		{
			RenderFace(false, m_iTexBack, verticesBuffer);
			verticesBuffer += verticesCount;

			{
				mu_uint32 renderSlotIndex = g_ClothRenderSlotCount.fetch_add(1);
				MUClothRenderSlot *renderSlot = &g_ClothRenderSlot[renderSlotIndex];
				g_ClothRenderSlotUsed[renderSlotIndex] = renderSlotIndex;

				renderSlot->HighHash = RetrieveClothHighHash(0, 0, m_iTexBack);
				renderSlot->LowHash = RetrieveClothLowHash(RT_TEXTURE, blendType, 0, 0);

				renderSlot->EnableShadow = ThreadInfo.EnableShadow;

				renderSlot->CulledType = ThreadInfo.CulledType;
				renderSlot->RenderIndex = 0;
				renderSlot->MeshIndex = 0;
				renderSlot->RenderType = RT_TEXTURE;
				renderSlot->BlendType = blendType;
				renderSlot->ExtraFlag = REXF_ENABLED_DEPTH | REXF_CULL_FRONT;

				renderSlot->ModelIndex = 0;
				renderSlot->Texture = m_iTexBack;

				renderSlot->BaseIndex = verticesIndex + verticesCount;
				renderSlot->IndexCount = verticesCount;
				renderSlot->InstanceIndex = instanceIndex;
			}
		}
	}
}

void CPhysicsCloth::RenderFace(mu_boolean bFront, mu_int32 iTexture, SHClothVertexData *verticesBuffer)
{
	SHClothVertexData *tmpBuffer = verticesBuffer;

	if (bFront)
	{
		for (mu_int32 j = 0; j < m_iNumVer - 1; ++j)
		{
			for (mu_int32 i = 0; i < m_iNumHor - 1; ++i)
			{
				RenderVertex(tmpBuffer++, i, j);
				RenderVertex(tmpBuffer++, i + 1, j);
				RenderVertex(tmpBuffer++, i + 1, j + 1);
				RenderVertex(tmpBuffer++, i, j);
				RenderVertex(tmpBuffer++, i + 1, j + 1);
				RenderVertex(tmpBuffer++, i, j + 1);
			}
		}
	}
	else
	{
		for (mu_int32 j = 0; j < m_iNumVer - 1; ++j)
		{
			for (mu_int32 i = 0; i < m_iNumHor - 1; ++i)
			{
				RenderVertex(tmpBuffer++, i, j);
				RenderVertex(tmpBuffer++, i, j + 1);
				RenderVertex(tmpBuffer++, i + 1, j + 1);
				RenderVertex(tmpBuffer++, i, j);
				RenderVertex(tmpBuffer++, i + 1, j + 1);
				RenderVertex(tmpBuffer++, i + 1, j);
			}
		}
	}
}

void CPhysicsCloth::RenderVertex(SHClothVertexData *verticesBuffer, mu_int32 xVertex, mu_int32 yVertex)
{
	mu_int32 iVertex = m_iNumHor * yVertex + xVertex;
#ifdef USE_COMPRESSED_VERTICES
	const EVector3 &p = m_pVertices[iVertex].GetPosition();
	verticesBuffer->Position[0] = ESimdConvertFloatToHalf(p[0]);
	verticesBuffer->Position[1] = ESimdConvertFloatToHalf(p[1]);
	verticesBuffer->Position[2] = ESimdConvertFloatToHalf(p[2]);
	verticesBuffer->UV[0] = ESimdConvertFloatToHalf((mu_float)xVertex / (mu_float)(m_iNumHor - 1));
	verticesBuffer->UV[1] = ESimdConvertFloatToHalf(EMath::Min(0.99f, (mu_float)yVertex / (mu_float)(m_iNumVer - 1)));
	mu_zeromem(verticesBuffer->Normal, sizeof(verticesBuffer->Normal));
#else
	verticesBuffer->Position = m_pVertices[iVertex].GetPosition();
	verticesBuffer->UV = EVector2((mu_float)xVertex / (mu_float)(m_iNumHor - 1), EMath::Min(0.99f, (mu_float)yVertex / (mu_float)(m_iNumVer - 1)));
	mu_zeromem(&verticesBuffer->Normal, sizeof(verticesBuffer->Normal));
#endif
}

void CPhysicsCloth::AddCollisionSphere(mu_float fXPos, mu_float fYPos, mu_float fZPos, mu_float fRadius, mu_int32 iBone)
{
	CPhysicsColSphere *pCol = new CPhysicsColSphere;
	pCol->Init(fXPos, fYPos, fZPos, fRadius, iBone);
	m_lstCollision.push_back(pCol);
}

void CPhysicsCloth::ProcessCollision()
{
	if (m_lstCollision.empty() == false)
	{
		for (auto it = m_lstCollision.begin(); it != m_lstCollision.end(); ++it)
		{
			CPhysicsCollision *pCollision = *it;
			for (mu_int32 i = 0; i < m_iNumVertices; ++i)
			{
				pCollision->ProcessCollision(&m_pVertices[i]);
			}
		}

		for (mu_int32 i = 0; i < m_iNumVertices; ++i)
		{
			m_pVertices[i].DoOneTimeMove();
		}
	}
}

CPhysicsClothMesh::CPhysicsClothMesh()
{
}

CPhysicsClothMesh::~CPhysicsClothMesh()
{
	Clear();
}

void CPhysicsClothMesh::Clear()
{
	CPhysicsCloth::Destroy();
}

mu_boolean CPhysicsClothMesh::Create(const mu_uint32 ThreadIndex, OBJECT *o, mu_int32 iMesh, mu_int32 iBone, mu_uint32 dwType, mu_int32 iBMDType)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	m_oOwner = o;
	m_iMesh = iMesh;
	m_iBone = iBone;
	m_dwType |= dwType;

	m_iBMDType = (iBMDType == -1) ? m_oOwner->Type : iBMDType;
	MUModel *b = &MODELS::Data[m_iBMDType];
	assert(iMesh < b->_MeshCount);
	Model::Mesh1 *pMesh = &b->_Mesh[m_iMesh];
	Model::MeshData1 *pMeshData = &b->_MeshData[m_iMesh];

	MU_SAFE_DELETE_ARRAY(m_pVertices);
	MU_SAFE_DELETE_ARRAY(m_pLink);

	m_iNumVertices = pMesh->NumVertices;
	m_pVertices = new CPhysicsVertex[m_iNumVertices];

	m_iNumLink = pMesh->NumTriangles * 3 * 2;
	m_pLink = new St_PhysicsLink[m_iNumLink];

#ifdef USE_DUAL_QUATERNION
	const EDualQuat *BoneMatrix = m_oOwner->BoneTransform;
#else
	const EMatrix4 *BoneMatrix = m_oOwner->BoneTransform;
#endif

	b->TransformVertices(ThreadIndex, m_iMesh, BoneMatrix);

	EVector3 *vertexTransform = &ThreadInfo.VertexTransform[pMeshData->VertexStart];

	for (mu_int32 iVertex = 0; iVertex < m_iNumVertices; ++iVertex)
	{
		Model::MeshVertex1 *v = &pMesh->Vertices[iVertex];
		m_pVertices[iVertex].Init(vertexTransform[iVertex], (v->Node == m_iBone));
	}

	mu_int32 iLink = 0;
	EVector3 vTemp;
	for (mu_int32 iTri = 0; iTri < pMesh->NumTriangles; ++iTri)
	{
		Model::MeshTriangle1 *tp = &pMesh->Triangles[iTri];
		for (mu_int32 i = 0; i < 3; ++i)
		{
			mu_uint32 iV1 = tp->VertexIndex[i];
			mu_uint32 iV2 = tp->VertexIndex[(i + 1) % 3];
			mu_float fDist = m_pVertices[iV1].GetDistance(&m_pVertices[iV2], &vTemp);

			mu_uint8 byLinkType = PLS_STRICTDISTANCE;

			EVector3 vPos1, vPos2;
			m_pVertices[iV1].GetPosition(&vPos1);
			m_pVertices[iV2].GetPosition(&vPos2);
			if (EMath::Abs(vPos1[0] - vPos2[0]) > 10.0f)
			{
				byLinkType = PLS_LOOSEDISTANCE;
			}

			SetLink(iLink++, iV1, iV2, fDist * 0.5f, fDist, PLS_SPRING | byLinkType);

			mu_uint32 iV3 = tp->VertexIndex[(i + 2) % 3];
			mu_uint32 iMatch = FindMatchVertex(pMesh, iV1, iV2, iV3);
			if (iMatch != -1)
			{
				mu_float fDist2 = m_pVertices[iV3].GetDistance(&m_pVertices[iMatch], &vTemp);
				if (fDist2 < fDist * 1.2f && !FindInLink(iLink, iV3, iMatch))
				{
					SetLink(iLink++, iV3, iMatch, fDist2 * 0.5f, fDist2, PLS_SPRING | byLinkType);
				}
			}
		}
	}
	m_iNumLink = iLink;

	return true;
}

mu_boolean CPhysicsClothMesh::Create(const mu_uint32 ThreadIndex, OBJECT *o, mu_int32 iMesh, mu_int32 iBone, mu_float fxPos, mu_float fyPos, mu_float fzPos, mu_int32 iNumHor, mu_int32 iNumVer, mu_float fWidth, mu_float fHeight, mu_int32 iTexFront, mu_int32 TexBack, mu_uint32 dwType, mu_int32 iBMDType)
{
	m_iBMDType = (iBMDType == -1) ? m_oOwner->Type : iBMDType;

	m_iMesh = iMesh;

	m_dwType |= PCT_OPT_MESHPROG;
	if (!CPhysicsCloth::Create(ThreadIndex, o, iBone, fxPos, fyPos, fzPos, iNumHor, iNumVer, fWidth, fHeight, iTexFront, TexBack, dwType))
	{
		return false;
	}

	return true;
}

mu_uint32 CPhysicsClothMesh::FindMatchVertex(Model::Mesh1 *pMesh, mu_int32 iV1, mu_int32 iV2, mu_int32 iV3)
{
	for (mu_int32 iTri = 0; iTri < pMesh->NumTriangles; ++iTri)
	{
		Model::MeshTriangle1 *tp = &pMesh->Triangles[iTri];
		for (mu_int32 i = 0; i < 3; ++i)
		{
			mu_uint32 iVcompare1 = tp->VertexIndex[i];
			mu_uint32 iVcompare2 = tp->VertexIndex[(i + 1) % 3];
			mu_uint32 iVcompare3 = tp->VertexIndex[(i + 2) % 3];
			if (iV1 == iVcompare2 && iV2 == iVcompare1 && iV3 != iVcompare3)
			{
				return (iVcompare3);
			}
		}
	}

	return (-1);
}

mu_boolean CPhysicsClothMesh::FindInLink(mu_int32 iCount, mu_int32 iV1, mu_int32 iV2)
{
	for (mu_int32 iLink = 0; iLink < iCount; ++iLink)
	{
		if (m_pLink[iLink].m_nVertices[0] == iV1 && m_pLink[iLink].m_nVertices[1] == iV2)
		{
			return true;
		}
	}

	return false;
}

#ifdef USE_DUAL_QUATERNION
void CPhysicsClothMesh::SetFixedVertices(const mu_uint32 ThreadIndex, const EDualQuat &Matrix)
#else
void CPhysicsClothMesh::SetFixedVertices(const mu_uint32 ThreadIndex, const EMatrix4 &Matrix)
#endif
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	if (m_dwType & PCT_OPT_MESHPROG)
	{
		CPhysicsCloth::SetFixedVertices(ThreadIndex, Matrix);
		return;
	}

	MUModel *b = &MODELS::Data[m_iBMDType];
	Model::Mesh1 *pMesh = &b->_Mesh[m_iMesh];
	Model::MeshData1 *pMeshData = &b->_MeshData[m_iMesh];

#ifdef USE_DUAL_QUATERNION
	const EDualQuat *BoneMatrix = m_oOwner->BoneTransform;
#else
	const EMatrix4 *BoneMatrix = m_oOwner->BoneTransform;
#endif
	b->TransformVertices(ThreadIndex, m_iMesh, BoneMatrix);

	EVector3 *vertexTransform = &ThreadInfo.VertexTransform[pMeshData->VertexStart];

	for (mu_int32 iVertex = 0; iVertex < m_iNumVertices; ++iVertex)
	{
		Model::MeshVertex1 *v = &pMesh->Vertices[iVertex];
		if (v->Node == m_iBone)
		{
			m_pVertices[iVertex].Init(vertexTransform[iVertex], true);
		}
	}
}

void CPhysicsClothMesh::NotifyVertexPos(mu_int32 iVertex, EVector3 vPos)
{
#if 0 // FIX this won't be used
	if (m_dwType & PCT_OPT_MESHPROG)
	{
		MUModel *b = &MODELS::Data[m_iBMDType];
		Mesh_t *pMesh = &b->Meshs[m_iMesh];

		Vertex_t *v = &pMesh->Vertices[iVertex];
		v->Position[0] = vPos[2];
		v->Position[1] = -vPos[1];
		v->Position[2] = vPos[0];

		if (v->Node != m_iBone)
		{
			v->Node = m_iBone;
		}
	}
#endif
}

void CPhysicsClothMesh::InitForces(const mu_uint32 ThreadIndex)
{
	if (m_dwType & PCT_OPT_MESHPROG)
	{
		CPhysicsCloth::InitForces(ThreadIndex);
		return;
	}

	mu_int32 iSeed = ((MoveSceneFrame / 10) * 101) % m_iNumVertices;

	for (mu_int32 iVertex = 0; iVertex < m_iNumVertices; ++iVertex)
	{
		m_pVertices[iVertex].UpdateForce(ThreadIndex, EMath::Abs(iSeed % 10), m_dwType, m_fWind);
	}
}

void CPhysicsClothMesh::Render(const mu_uint32 ThreadIndex, MUModel *b, EVector3 *pvColor)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	b = &MODELS::Data[m_iBMDType];
	Model::Mesh1 *pMesh = &b->_Mesh[m_iMesh];

	if ((m_dwType & PCT_OPT_MESHPROG) != 0)
	{
#if 1
		mu_uint32 verticesCount = pMesh->NumTriangles * 3;
		mu_uint32 verticesIndex = g_Tasking._DynamicVerticesCount.fetch_add(verticesCount);
		SHClothVertexData *verticesBuffer = &g_Tasking._DynamicClothBuffer[verticesIndex];

		b->ClothMesh(ThreadIndex) = m_iMesh;
		b->ClothVerticesIndex(ThreadIndex) = verticesIndex;
		b->ClothVerticesCount(ThreadIndex) = verticesCount;

#ifdef USE_DUAL_QUATERNION
		const EDualQuat *BoneMatrix = m_oOwner->BoneTransform;
#else
		const EMatrix4 *BoneMatrix = m_oOwner->BoneTransform;
#endif

		for (mu_uint32 n = 0; n < pMesh->NumNormals; ++n)
		{
			Model::MeshNormal1 &v = pMesh->Normals[n];
#ifdef USE_DUAL_QUATERNION
			BoneMatrix[v.Node].TransformNormalOut(v.Normal, ThreadInfo.NormalTransform[n]);
#else
			VectorRotate(v.Normal, BoneMatrix[v.Node], ThreadInfo.NormalTransform[n]);
#endif
		}

		for (mu_uint32 n = 0; n < pMesh->NumTriangles; ++n)
		{
			Model::MeshTriangle1 *tp = &pMesh->Triangles[n];

			for (mu_uint32 j = 0; j < 3; ++j)
			{
				mu_uint16 &vindex = tp->VertexIndex[j];
				mu_uint16 &nindex = tp->NormalIndex[j];
				mu_uint16 &tindex = tp->TexCoordIndex[j];

#ifdef USE_COMPRESSED_VERTICES
				const EVector3 &p = m_pVertices[vindex].GetPosition();
				const EVector2 &t = pMesh->TexCoords[tindex];
				const EVector3 &n = ThreadInfo.NormalTransform[nindex];
				verticesBuffer->Position[0] = ESimdConvertFloatToHalf(p[0]);
				verticesBuffer->Position[1] = ESimdConvertFloatToHalf(p[1]);
				verticesBuffer->Position[2] = ESimdConvertFloatToHalf(p[2]);
				verticesBuffer->UV[0] = ESimdConvertFloatToHalf(t[0]);
				verticesBuffer->UV[1] = ESimdConvertFloatToHalf(t[1]);
				verticesBuffer->Normal[0] = ESimdConvertFloatToHalf(n[0]);
				verticesBuffer->Normal[1] = ESimdConvertFloatToHalf(n[1]);
				verticesBuffer->Normal[2] = ESimdConvertFloatToHalf(n[2]);
#else
				verticesBuffer->Position = m_pVertices[vindex].GetPosition();
				verticesBuffer->UV = pMesh->TexCoords[tindex];
				verticesBuffer->Normal = ThreadInfo.NormalTransform[nindex];
#endif
				++verticesBuffer;
			}
		}
#else
		mu_uint32 verticesCount = (m_iNumVer - 1) * (m_iNumHor - 1) * 6;
		mu_uint32 verticesIndex = g_Tasking._DynamicVerticesCount.fetch_add(verticesCount);
		SHClothVertexData *verticesBuffer = &g_Tasking._DynamicClothBuffer[verticesIndex];

		b->ClothMesh(ThreadIndex) = m_iMesh;
		b->ClothVerticesIndex(ThreadIndex) = verticesIndex;
		b->ClothVerticesCount(ThreadIndex) = verticesCount;

		EMatrix4 *BoneMatrix = m_oOwner->BoneTransform;

		for (mu_uint32 n = 0; n < pMesh->NumNormals; ++n)
		{
			Model::MeshNormal1 &v = pMesh->Normals[n];
			VectorRotate(v.Normal, BoneMatrix[v.Node], ThreadInfo.NormalTransform[n]);
		}

		for (mu_int32 j = 0; j < m_iNumVer - 1; ++j)
		{
			for (mu_int32 i = 0; i < m_iNumHor - 1; ++i)
			{
				mu_uint32 j1 = j, i1 = i;
				mu_uint32 j2 = j, i2 = i + 1;
				mu_uint32 j3 = j + 1, i3 = i + 1;
				mu_uint32 j4 = j, i4 = i;
				mu_uint32 j5 = j + 1, i5 = i + 1;
				mu_uint32 j6 = j + 1, i6 = i;

				mu_int32 iVertex1 = m_iNumHor * j1 + i1;
				mu_int32 iVertex2 = m_iNumHor * j2 + i2;
				mu_int32 iVertex3 = m_iNumHor * j3 + i3;
				mu_int32 iVertex4 = m_iNumHor * j4 + i4;
				mu_int32 iVertex5 = m_iNumHor * j5 + i5;
				mu_int32 iVertex6 = m_iNumHor * j6 + i6;

				mu_int32 nVertex1 = j1 + i1 * m_iNumVer;
				mu_int32 nVertex2 = j2 + i2 * m_iNumVer;
				mu_int32 nVertex3 = j3 + i3 * m_iNumVer;
				mu_int32 nVertex4 = j4 + i4 * m_iNumVer;
				mu_int32 nVertex5 = j5 + i5 * m_iNumVer;
				mu_int32 nVertex6 = j6 + i6 * m_iNumVer;

				verticesBuffer[0].Position = m_pVertices[iVertex1].GetPosition();
				verticesBuffer[0].Normal = ThreadInfo.NormalTransform[nVertex1];
				verticesBuffer[0].UV = EVector2((mu_float)i1 / (mu_float)(m_iNumHor - 1), EMath::Min(0.99f, (mu_float)j1 / (mu_float)(m_iNumVer - 1)));
				verticesBuffer[1].Position = m_pVertices[iVertex2].GetPosition();
				verticesBuffer[1].Normal = ThreadInfo.NormalTransform[nVertex2];
				verticesBuffer[1].UV = EVector2((mu_float)i2 / (mu_float)(m_iNumHor - 1), EMath::Min(0.99f, (mu_float)j2 / (mu_float)(m_iNumVer - 1)));
				verticesBuffer[2].Position = m_pVertices[iVertex3].GetPosition();
				verticesBuffer[2].Normal = ThreadInfo.NormalTransform[nVertex3];
				verticesBuffer[2].UV = EVector2((mu_float)i3 / (mu_float)(m_iNumHor - 1), EMath::Min(0.99f, (mu_float)j3 / (mu_float)(m_iNumVer - 1)));
				verticesBuffer[3].Position = m_pVertices[iVertex4].GetPosition();
				verticesBuffer[3].Normal = ThreadInfo.NormalTransform[nVertex4];
				verticesBuffer[3].UV = EVector2((mu_float)i4 / (mu_float)(m_iNumHor - 1), EMath::Min(0.99f, (mu_float)j4 / (mu_float)(m_iNumVer - 1)));
				verticesBuffer[4].Position = m_pVertices[iVertex5].GetPosition();
				verticesBuffer[4].Normal = ThreadInfo.NormalTransform[nVertex5];
				verticesBuffer[4].UV = EVector2((mu_float)i5 / (mu_float)(m_iNumHor - 1), EMath::Min(0.99f, (mu_float)j5 / (mu_float)(m_iNumVer - 1)));
				verticesBuffer[5].Position = m_pVertices[iVertex6].GetPosition();
				verticesBuffer[5].Normal = ThreadInfo.NormalTransform[nVertex6];
				verticesBuffer[5].UV = EVector2((mu_float)i6 / (mu_float)(m_iNumHor - 1), EMath::Min(0.99f, (mu_float)j6 / (mu_float)(m_iNumVer - 1)));

				verticesBuffer += 6;
			}
		}
#endif
	}
	else
	{
		mu_uint32 verticesCount = pMesh->NumTriangles * 3;
		mu_uint32 verticesIndex = g_Tasking._DynamicVerticesCount.fetch_add(verticesCount);
		SHClothVertexData *verticesBuffer = &g_Tasking._DynamicClothBuffer[verticesIndex];

		b->ClothMesh(ThreadIndex) = m_iMesh;
		b->ClothVerticesIndex(ThreadIndex) = verticesIndex;
		b->ClothVerticesCount(ThreadIndex) = verticesCount;

#ifdef USE_DUAL_QUATERNION
		const EDualQuat *BoneMatrix = m_oOwner->BoneTransform;
#else
		const EMatrix4 *BoneMatrix = m_oOwner->BoneTransform;
#endif

		for (mu_uint32 n = 0; n < pMesh->NumNormals; ++n)
		{
			Model::MeshNormal1 &v = pMesh->Normals[n];
#ifdef USE_DUAL_QUATERNION
			BoneMatrix[v.Node].TransformNormalOut(v.Normal, ThreadInfo.NormalTransform[n]);
#else
			VectorRotate(v.Normal, BoneMatrix[v.Node], ThreadInfo.NormalTransform[n]);
#endif
		}

		for (mu_uint32 n = 0; n < pMesh->NumTriangles; ++n)
		{
			Model::MeshTriangle1 *tp = &pMesh->Triangles[n];

			for (mu_uint32 j = 0; j < 3; ++j)
			{
				mu_uint16 &vindex = tp->VertexIndex[j];
				mu_uint16 &nindex = tp->NormalIndex[j];
				mu_uint16 &tindex = tp->TexCoordIndex[j];

#ifdef USE_COMPRESSED_VERTICES
				const EVector3 &p = m_pVertices[vindex].GetPosition();
				const EVector2 &t = pMesh->TexCoords[tindex];
				const EVector3 &n = ThreadInfo.NormalTransform[nindex];
				verticesBuffer->Position[0] = ESimdConvertFloatToHalf(p[0]);
				verticesBuffer->Position[1] = ESimdConvertFloatToHalf(p[1]);
				verticesBuffer->Position[2] = ESimdConvertFloatToHalf(p[2]);
				verticesBuffer->UV[0] = ESimdConvertFloatToHalf(t[0]);
				verticesBuffer->UV[1] = ESimdConvertFloatToHalf(t[1]);
				verticesBuffer->Normal[0] = ESimdConvertFloatToHalf(n[0]);
				verticesBuffer->Normal[1] = ESimdConvertFloatToHalf(n[1]);
				verticesBuffer->Normal[2] = ESimdConvertFloatToHalf(n[2]);
#else
				verticesBuffer->Position = m_pVertices[vindex].GetPosition();
				verticesBuffer->UV = pMesh->TexCoords[tindex];
				verticesBuffer->Normal = ThreadInfo.NormalTransform[nindex];
#endif
				++verticesBuffer;
			}
		}
	}
}