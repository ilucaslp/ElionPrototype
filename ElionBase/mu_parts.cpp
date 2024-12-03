#include "stdafx.h"
#include "mu_parts.h"

void CreatePartsFactory(CHARACTER* c)
{
	if (c->m_pParts == nullptr && c->EtcPart != 0)
	{
		switch (c->EtcPart)
		{
		case PARTS_ATTACK_TEAM_MARK:
			c->m_pParts = new CSParts2D(BITMAP_FORMATION_MARK, 0, 20, 120.0f, 0.0f, 0.0f);
			break;

		case PARTS_ATTACK_TEAM_MARK2:
			c->m_pParts = new CSParts2D(BITMAP_FORMATION_MARK, 1, 20, 120.0f, 0.0f, 0.0f);
			break;

		case PARTS_ATTACK_TEAM_MARK3:
			c->m_pParts = new CSParts2D(BITMAP_FORMATION_MARK, 2, 20, 120.0f, 0.0f, 0.0f);
			break;

		case PARTS_ATTACK_KING_TEAM_MARK:
			c->m_pParts = new CSParts2D(BITMAP_FORMATION_MARK, 3, 20, 120.0f, 0.0f, 0.0f);
			break;

		case PARTS_ATTACK_KING_TEAM_MARK2:
			c->m_pParts = new CSParts2D(BITMAP_FORMATION_MARK, 4, 20, 120.0f, 0.0f, 0.0f);
			break;

		case PARTS_ATTACK_KING_TEAM_MARK3:
			c->m_pParts = new CSParts2D(BITMAP_FORMATION_MARK, 5, 20, 120.0f, 0.0f, 0.0f);
			break;

		case PARTS_DEFENSE_TEAM_MARK:
			c->m_pParts = new CSParts2D(BITMAP_FORMATION_MARK, 6, 20, 120.0f, 0.0f, 0.0f);
			break;

		case PARTS_DEFENSE_KING_TEAM_MARK:
			c->m_pParts = new CSParts2D(BITMAP_FORMATION_MARK, 7, 20, 120.0f, 0.0f, 0.0f);
			break;
		default:
			break;
		}
	}
}

void RenderParts(const mu_uint32 ThreadIndex, CHARACTER* c)
{
	if (g_isCharacterBuff((&c->Object), eBuff_Cloaking))
	{
		return;
	}

	if (c->EtcPart >= PARTS_ATTACK_TEAM_MARK && c->EtcPart <= PARTS_DEFENSE_KING_TEAM_MARK && battleCastle::IsBattleCastleStart() == false)
	{
		return;
	}

	if (c->m_pTempParts != nullptr)
	{
		c->m_pTempParts->IRender(ThreadIndex, c);
	}

	if (c->m_pParts == nullptr)
	{
		return;
	}

	c->m_pParts->IRender(ThreadIndex, c);
}

void DeleteParts(CHARACTER* c)
{
	if (c->m_pParts != nullptr)
	{
		MU_SAFE_DELETE(c->m_pParts);
		c->EtcPart = 0;
	}

	MU_SAFE_DELETE(c->m_pTempParts);
}

CSParts::CSParts(mu_int32 Type, mu_int32 BoneNumber, mu_boolean bBillBoard, mu_float x, mu_float y, mu_float z, mu_float ax, mu_float ay, mu_float az)
{
	m_iBoneNumber = BoneNumber;

	m_vOffset[0] = x;
	m_vOffset[1] = y;
	m_vOffset[2] = z;

	m_pObj.Flags.Clear();
	if (bBillBoard)
	{
		m_pObj.Flags.Set(OBJECT::eOF_LIVE | OBJECT::eOF_LIGHTENABLE | OBJECT::eOF_BILLBOARD);
	}
	else
	{
		m_pObj.Flags.Set(OBJECT::eOF_LIVE | OBJECT::eOF_LIGHTENABLE);
	}

	m_pObj.Type = Type;
	m_pObj.Owner = nullptr;
	m_pObj.SubType = 0;
	m_pObj.HiddenMesh = -1;
	m_pObj.BlendMesh = -1;
	m_pObj.BlendMeshLight = 1.0f;
	m_pObj.Scale = 1.0f;
	m_pObj.LifeTime = 30;
	m_pObj.Alpha = 1.0f;
	m_pObj.AlphaTarget = 1.0f;

	g_CharacterClearBuff((&m_pObj));

	Vector(1.0f, 1.0f, 1.0f, m_pObj.Light);
	Vector(0.0f, 0.0f, 0.0f, m_pObj.HeadAngle);

	m_pObj.CurrentAction = 0;
	m_pObj.PriorAction = 0;
	m_pObj.PriorAnimationFrame = 0.0f;
	m_pObj.AnimationFrame = 0.0f;
	m_pObj.Velocity = 0.5f;

	Vector(ax, ay, az, m_pObj.Angle);
}

void CSParts::IRender(const mu_uint32 ThreadIndex, CHARACTER* c)
{
	if (c == nullptr) return;
	if (m_pObj.Alpha < 0.01f) return;

	OBJECT* o = &c->Object;
	MUModel* b = &MODELS::Data[o->Type];
	EVector3  Position;

	b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(m_iBoneNumber)], m_vOffset, Position, true);
	VectorCopy(Position, m_pObj.Position);
	if (m_pObj.Flags.Get(OBJECT::eOF_BILLBOARD) == false)
	{
		VectorCopy(o->Angle, m_pObj.Angle);
	}

	if (m_pObj.Type == MODEL_XMAS_EVENT_EARRING)
	{
		m_pObj.Angle[2] -= o->HeadAngle[0];
	}

	b = &MODELS::Data[m_pObj.Type];
	b->CurrentAction(ThreadIndex) = m_pObj.CurrentAction;

	if(MU_UPDATECOUNTER > 0)
	{
		mu_float fSpeed = m_pObj.Velocity;
		b->PlayAnimation(ThreadIndex, &m_pObj.AnimationFrame, &m_pObj.PriorAnimationFrame, &m_pObj.PriorAction, fSpeed);
	}

	Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
	RenderObject(ThreadIndex, &m_pObj, true);
}

CSAnimationParts::CSAnimationParts(mu_int32 Type, mu_int32 BoneNumber, mu_boolean bBillBoard, mu_float x, mu_float y, mu_float z, mu_float ax, mu_float ay, mu_float az)
{
	m_iBoneNumber = BoneNumber;

	m_vOffset[0] = x;
	m_vOffset[1] = y;
	m_vOffset[2] = z;

	m_pObj.Flags.Clear();
	if (bBillBoard)
	{
		m_pObj.Flags.Set(OBJECT::eOF_LIVE | OBJECT::eOF_LIGHTENABLE | OBJECT::eOF_BILLBOARD);
	}
	else
	{
		m_pObj.Flags.Set(OBJECT::eOF_LIVE | OBJECT::eOF_LIGHTENABLE);
	}

	m_pObj.Type = Type;
	m_pObj.Owner = nullptr;
	m_pObj.SubType = 0;
	m_pObj.HiddenMesh = -1;
	m_pObj.BlendMesh = -1;
	m_pObj.BlendMeshLight = 1.0f;
	m_pObj.Scale = 1.0f;
	m_pObj.LifeTime = 30;
	m_pObj.Alpha = 1.0f;
	m_pObj.AlphaTarget = 1.0f;

	g_CharacterClearBuff((&m_pObj));

	Vector(1.0f, 1.0f, 1.0f, m_pObj.Light);
	Vector(0.0f, 0.0f, 0.0f, m_pObj.HeadAngle);

	m_pObj.CurrentAction = 0;
	m_pObj.PriorAction = 0;
	m_pObj.PriorAnimationFrame = 0.0f;
	m_pObj.AnimationFrame = 0.0f;
	m_pObj.Velocity = 0.5f;

	Vector(ax, ay, az, m_pObj.Angle);
}

void CSAnimationParts::Animation(const mu_uint32 ThreadIndex, CHARACTER* c)
{
	MUModel* b = &MODELS::Data[m_pObj.Type];
	b->CurrentAction(ThreadIndex) = m_pObj.CurrentAction;

	mu_float fSpeed = m_pObj.Velocity;
	b->PlayAnimation(ThreadIndex, &m_pObj.AnimationFrame, &m_pObj.PriorAnimationFrame, &m_pObj.PriorAction, fSpeed);
}

void CSAnimationParts::IRender(const mu_uint32 ThreadIndex, CHARACTER* c)
{
	if (c == nullptr) return;
	if (m_pObj.Alpha < 0.01f) return;

	OBJECT* o = &c->Object;
	MUModel* b = &MODELS::Data[o->Type];
	EVector3  Position;

	b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(m_iBoneNumber)], m_vOffset, Position, true);
	VectorAdd(o->Position, Position, m_pObj.Position);
	if (m_pObj.Flags.Get(OBJECT::eOF_BILLBOARD) == false)
	{
		VectorCopy(o->Angle, m_pObj.Angle);
	}

	if (MU_UPDATECOUNTER > 0)
	{
		Animation(ThreadIndex, c);
	}

	RenderObject(ThreadIndex, &m_pObj, true);
}

CSParts2D::CSParts2D(mu_int32 Type, mu_int32 SubType, mu_int32 BoneNumber, mu_float x, mu_float y, mu_float z)
{
	m_iBoneNumber = BoneNumber;

	m_vOffset[0] = x;
	m_vOffset[1] = y;
	m_vOffset[2] = z;

	m_pObj.Type = Type;
	m_pObj.SubType = SubType;
	Vector(1.0f, 1.0f, 1.0f, m_pObj.Light);
}

void CSParts2D::IRender(const mu_uint32 ThreadIndex, CHARACTER* c)
{
	if (c == nullptr) return;

	OBJECT* o = &c->Object;
	MUModel* b = &MODELS::Data[o->Type];
	EVector3  Position;
	mu_uint8 bSubType = m_pObj.SubType;

	b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(m_iBoneNumber)], m_vOffset, Position, true);
	VectorCopy(o->Position, m_pObj.Position);
	m_pObj.Position[2] = Position[2];

	if ((c->EtcPart == PARTS_ATTACK_TEAM_MARK
		|| c->EtcPart == PARTS_ATTACK_TEAM_MARK2
		|| c->EtcPart == PARTS_ATTACK_TEAM_MARK3
		|| c->EtcPart == PARTS_DEFENSE_TEAM_MARK)
		&& c->GuildStatus == GAME::G_MASTER)
	{
		if (mu_strcmp(GuildMark[c->GuildMarkIndex].UnionName.c_str(), GuildMark[c->GuildMarkIndex].GuildName.c_str()) == 0 ||
			mu_strcmp(GuildMark[c->GuildMarkIndex].UnionName.c_str(), _T("")) == 0)
		{
			if (c->EtcPart == PARTS_DEFENSE_TEAM_MARK)
				bSubType += 1;
			else
				bSubType += 3;
		}
	}
	
	if (MU_UPDATECOUNTER > 0)
	{
		CreateSprite(m_pObj.Type, m_pObj.Position, 1.0f, m_pObj.Light, nullptr, 0, bSubType);
	}
}