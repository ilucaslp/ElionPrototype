#include "stdafx.h"
#include "mu_event.h"

CXmasEvent g_XmasEvent;
CNewYearsDayEvent g_NewYearsDayEvent;
CBlueLuckyBagEvent g_pBlueLuckyBagEvent;
C09SummerEvent g_09SummerEvent;

CXmasEvent::CXmasEvent()
{
	m_iEffectID = XMAS_MINIMUM_INDEX;
}

CXmasEvent::~CXmasEvent()
{

}

void CXmasEvent::CreateXmasEventEffect(const mu_uint32 ThreadIndex, CHARACTER* pCha, OBJECT *pObj, mu_int32 iType)
{
	if (pCha->m_iTempKey >= 0)
	{
		DeleteCharacter(pCha->m_iTempKey);
	}

	GenID();

	CHARACTER* c = CreateCharacter(m_iEffectID, MODEL_PLAYER, pCha->PositionX, pCha->PositionY, pCha->Rot);

	pCha->m_iTempKey = m_iEffectID;
	c->Object.Scale = 0.30f;
	c->Object.SubType = iType + MODEL_XMAS_EVENT_CHA_SSANTA;

	switch (c->Object.SubType)
	{
	case MODEL_XMAS_EVENT_CHA_SSANTA:
		mu_strcpy(c->ID, mu_countof(c->ID), GlobalText[2245]);
		break;
	case MODEL_XMAS_EVENT_CHA_DEER:
		mu_strcpy(c->ID, mu_countof(c->ID), GlobalText[2246]);
		break;
	case MODEL_XMAS_EVENT_CHA_SNOWMAN:
		mu_strcpy(c->ID, mu_countof(c->ID), GlobalText[2247]);
		break;
	}

	c->Object.Owner = pObj;

	c->Object.m_dwTime = g_CurrentTime;

	OBJECT* o = &c->Object;

	VectorCopy(pObj->Position, o->Position);
	VectorCopy(pObj->Angle, o->Angle);
	o->PriorAction = pObj->PriorAction;
	o->PriorAnimationFrame = pObj->PriorAnimationFrame;
	o->CurrentAction = pObj->CurrentAction;
	o->AnimationFrame = pObj->AnimationFrame;

	EVector3 vPos, vLight;
	Vector(0.6f, 0.6f, 0.6f, vLight);

	VectorCopy(o->Position, vPos);
	vPos[0] += 50.0f;
	vPos[1] += 50.0f;
	vPos[2] += 50.0f;
	CreateParticle(ThreadIndex, BITMAP_EXPLOTION_MONO, vPos, o->Angle, vLight, 0, 0.6f);
	mu_int32 i;
	for (i = 0; i < 2; ++i)
	{
		CreateEffect(ThreadIndex, MODEL_ICE_SMALL, vPos, o->Angle, vLight, 0);
		CreateParticle(ThreadIndex, BITMAP_CLUD64, vPos, o->Angle, vLight, 3, 1.0f);
		CreateParticle(ThreadIndex, BITMAP_CLUD64, vPos, o->Angle, vLight, 3, 1.0f);
		CreateEffect(ThreadIndex, MODEL_HALLOWEEN_CANDY_STAR, vPos, o->Angle, vLight, 1);
		CreateParticle(ThreadIndex, BITMAP_SNOW_EFFECT_1, vPos, o->Angle, vLight, 0, 0.5f);
		CreateParticle(ThreadIndex, BITMAP_SNOW_EFFECT_1, vPos, o->Angle, vLight, 0, 0.5f);
	}

	VectorCopy(o->Position, vPos);
	vPos[0] -= 50.0f;
	vPos[1] -= 50.0f;
	vPos[2] += 50.0f;
	CreateParticle(ThreadIndex, BITMAP_EXPLOTION_MONO, vPos, o->Angle, vLight, 0, 0.6f);
	for (i = 0; i < 2; ++i)
	{
		CreateEffect(ThreadIndex, MODEL_ICE_SMALL, vPos, o->Angle, vLight, 0);
		CreateParticle(ThreadIndex, BITMAP_CLUD64, vPos, o->Angle, vLight, 3, 1.0f);
		CreateParticle(ThreadIndex, BITMAP_CLUD64, vPos, o->Angle, vLight, 3, 1.0f);
		CreateEffect(ThreadIndex, MODEL_HALLOWEEN_CANDY_STAR, vPos, o->Angle, vLight, 1);
		CreateParticle(ThreadIndex, BITMAP_SNOW_EFFECT_1, vPos, o->Angle, vLight, 0, 0.5f);
		CreateParticle(ThreadIndex, BITMAP_SNOW_EFFECT_1, vPos, o->Angle, vLight, 0, 0.5f);
	}

	VectorCopy(o->Position, vPos);
	vPos[0] += 50.0f;
	vPos[1] -= 50.0f;
	vPos[2] += 50.0f;
	CreateParticle(ThreadIndex, BITMAP_EXPLOTION_MONO, vPos, o->Angle, vLight, 0, 0.6f);
	for (i = 0; i < 2; ++i)
	{
		CreateEffect(ThreadIndex, MODEL_ICE_SMALL, vPos, o->Angle, vLight, 0);
		CreateParticle(ThreadIndex, BITMAP_CLUD64, vPos, o->Angle, vLight, 3, 1.0f);
		CreateParticle(ThreadIndex, BITMAP_CLUD64, vPos, o->Angle, vLight, 3, 1.0f);
		CreateEffect(ThreadIndex, MODEL_HALLOWEEN_CANDY_STAR, vPos, o->Angle, vLight, 1);
		CreateParticle(ThreadIndex, BITMAP_SNOW_EFFECT_1, vPos, o->Angle, vLight, 0, 0.5f);
		CreateParticle(ThreadIndex, BITMAP_SNOW_EFFECT_1, vPos, o->Angle, vLight, 0, 0.5f);
	}

	if (o->CurrentAction == PLAYER_SANTA_2)
	{
		EVector3 vPos, vAngle, vLight;
		VectorCopy(o->Position, vPos);
		vPos[2] += 230.0f;
		Vector(1.0f, 1.0f, 1.0f, vLight);
		Vector(0.0f, 0.0f, 40.0f, vAngle);
		CreateEffect(ThreadIndex, MODEL_XMAS_EVENT_ICEHEART, vPos, vAngle, vLight, 0, o);
		CreateParticle(ThreadIndex, BITMAP_DS_EFFECT, vPos, o->Angle, vLight, 0, 3.0f, o);
		Vector(1.0f, 0.0f, 0.0f, vLight);
		CreateParticle(ThreadIndex, BITMAP_LIGHT, vPos, o->Angle, vLight, 10, 3.0f, o);
	}
}

void CXmasEvent::GenID()
{
	if(m_iEffectID >= XMAS_MAXIMUM_INDEX)
	{
		m_iEffectID = XMAS_MAXIMUM_INDEX;
	}
	else
	{
		++m_iEffectID;
	}
}

CNewYearsDayEvent::CNewYearsDayEvent()
{

}

CNewYearsDayEvent::~CNewYearsDayEvent()
{

}

CHARACTER* CNewYearsDayEvent::CreateMonster(mu_int32 iType, mu_int32 iPosX, mu_int32 iPosY, mu_int32 iKey)
{
	CHARACTER* pCharacter = nullptr;

	switch(iType)
	{
	case 365:
		{
			pCharacter = CreateCharacter(iKey, MODEL_MONSTER01+122, iPosX, iPosY);
			pCharacter->Object.Scale = 1.5f;
		}
		break;
	}

	return pCharacter;
}

mu_boolean CNewYearsDayEvent::MoveMonsterVisual(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b)
{
	switch (o->Type)
	{
	case MODEL_MONSTER01 + 122:
		{
			EVector3 vRelativePos, vWorldPos;
			Vector(0.0f, 0.0f, 0.0f, vRelativePos);
			b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(81)], vRelativePos, vWorldPos, true);

			EVector3 vLight;
			Vector(1.0f, 1.0f, 1.0f, vLight);

			if (o->CurrentAction >= MONSTER01_STOP1 && o->CurrentAction <= MONSTER01_ATTACK2)
			{
				if (GetLargeRand(3) == 0)
				{
					Vector(1.0f, 0.4f, 0.0f, vLight);
					CreateParticle(ThreadIndex, BITMAP_LIGHT + 2, vWorldPos, o->Angle, vLight, 0, 0.28f);

					if (GetLargeRand(2) == 0)
					{
						Vector(0.8f, 0.6f, 0.1f, vLight);
					}
					else
					{
						mu_float fLumi;
						fLumi = (mu_float)(GetLargeRand(100)) * 0.01f;
						vLight[0] = fLumi;
						fLumi = (mu_float)(GetLargeRand(100)) * 0.01f;
						vLight[1] = fLumi;
						fLumi = (mu_float)(GetLargeRand(100)) * 0.01f;
						vLight[2] = fLumi;
					}
					CreateParticle(ThreadIndex, BITMAP_SHINY, vWorldPos, o->Angle, vLight, 4, 0.8f);
				}

			}
			else if (o->CurrentAction == MONSTER01_SHOCK || o->CurrentAction == MONSTER01_DIE)
			{
				if (GetLargeRand(3) == 0)
				{
					Vector(1.0f, 0.4f, 0.0f, vLight);
					mu_float fScale = 1.0f;
					if (o->CurrentAction == MONSTER01_SHOCK)	fScale = 0.4f;
					else if (o->CurrentAction == MONSTER01_DIE) fScale = 0.5f;

					CreateParticle(ThreadIndex, BITMAP_LIGHT + 2, vWorldPos, o->Angle, vLight, 0, fScale);

					for (mu_int32 i = 0; i < 5; ++i)
					{
						if (GetLargeRand(2) == 0)
						{
							Vector(0.8f, 0.6f, 0.1f, vLight);
						}
						else
						{
							mu_float fLumi;
							fLumi = (mu_float)(GetLargeRand(100)) * 0.01f;
							vLight[0] = fLumi;
							fLumi = (mu_float)(GetLargeRand(100)) * 0.01f;
							vLight[1] = fLumi;
							fLumi = (mu_float)(GetLargeRand(100)) * 0.01f;
							vLight[2] = fLumi;
						}
						CreateParticle(ThreadIndex, BITMAP_SHINY, vWorldPos, o->Angle, vLight, 4, 0.8f);
					}
				}
				if (GetLargeRand(2) == 0)
				{
					Vector(0.3f, 0.3f, 0.8f, vLight);
					CreateParticle(ThreadIndex, BITMAP_LIGHT + 2, vWorldPos, o->Angle, vLight, 1, 1.0f);
				}
			}

			if (o->CurrentAction == MONSTER01_DIE)
			{
				if (o->AnimationFrame <= 0.3f)
				{
					o->m_iAnimation = GetLargeRand(6) + MODEL_NEWYEARSDAY_EVENT_BEKSULKI;
					CreateParticle(ThreadIndex, BITMAP_EXPLOTION, vWorldPos, o->Angle, vLight, 0, 0.5f);
					if (GetLargeRand(4) == 0) o->m_iAnimation = MODEL_NEWYEARSDAY_EVENT_PIG;

					PlayBackground(SOUND_NEWYEARSDAY_DIE);
				}

				if (o->AnimationFrame >= 4.5f)
					CreateEffect(ThreadIndex, MODEL_NEWYEARSDAY_EVENT_MONEY, vWorldPos, o->Angle, vLight);

				if (o->m_iAnimation != 0 && GetLargeRand(3) == 0)
				{
					if (o->AnimationFrame >= 4.5f)
						CreateEffect(ThreadIndex, o->m_iAnimation, vWorldPos, o->Angle, vLight);
				}
			}
		}
		break;
	}
	return false;
}

const mu_uint64 CBlueLuckyBagEvent::m_dwBlueLuckyBagCheckTime = 600000;

CBlueLuckyBagEvent::CBlueLuckyBagEvent()
{
	m_bBlueLuckyBag = false;
	m_dwBlueLuckyBagTime = 0;
}

CBlueLuckyBagEvent::~CBlueLuckyBagEvent()
{

}

void CBlueLuckyBagEvent::StartBlueLuckyBag()
{
	m_bBlueLuckyBag = true;
	m_dwBlueLuckyBagTime = g_CurrentTime;
}

void CBlueLuckyBagEvent::CheckTime()
{
	if (m_bBlueLuckyBag == true)
	{
		if (g_CurrentTime - m_dwBlueLuckyBagTime > m_dwBlueLuckyBagCheckTime)
		{
			m_bBlueLuckyBag = false;
			m_dwBlueLuckyBagTime = 0;
		}
	}
}

mu_boolean CBlueLuckyBagEvent::IsEnableBlueLuckyBag()
{
	return m_bBlueLuckyBag;
}

C09SummerEvent::C09SummerEvent()
{

}

C09SummerEvent::~C09SummerEvent()
{

}

CHARACTER* C09SummerEvent::CreateMonster(mu_int32 iType, mu_int32 iPosX, mu_int32 iPosY, mu_int32 iKey)
{
	CHARACTER* pCharacter = nullptr;

	if (iType == 463)
	{
		pCharacter = CreateCharacter(iKey, MODEL_MONSTER01 + 154, iPosX, iPosY);
		pCharacter->Object.Scale = 0.8f;
		pCharacter->Object.HiddenMesh = 2;
		pCharacter->Object.m_iAnimation = 0;
	}

	return pCharacter;
}

mu_boolean C09SummerEvent::MoveMonsterVisual(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b)
{
	if (o->Type != MODEL_MONSTER01 + 154)
		return false;

	EVector3 vRelativePos, vWorldPos;
	Vector(0.0f, 0.0f, 0.0f, vRelativePos);
	b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(39)], vRelativePos, vWorldPos, true);

	EVector3 vLight;
	mu_float fLumi = (EMath::Sin(FWorldTime*0.0015f) + 1.0f) * 0.4f + 0.2f;

	Vector(0.8f, 0.4f, 0.2f, vLight);

	CreateSprite(BITMAP_LIGHT, vWorldPos, 2.0f, vLight, o);
	Vector(fLumi*0.8f, fLumi*0.4f, fLumi*0.2f, vLight);
	CreateSprite(BITMAP_LIGHT, vWorldPos, 3.0f, vLight, o);

	switch (o->CurrentAction)
	{
	case MONSTER01_WALK:
		{
			if (o->AnimationFrame >= 1.0f && o->AnimationFrame <= 1.3f)
			{
				if (o->m_iAnimation == 0)
				{
					PlayBackground(SOUND_UMBRELLA_MONSTER_WALK1);
					o->m_iAnimation = 1;
				}
				else
				{
					PlayBackground(SOUND_UMBRELLA_MONSTER_WALK2);
					o->m_iAnimation = 0;
				}
			}
		}
		break;
	case MONSTER01_SHOCK:
		{
			if (o->AnimationFrame >= 0.5f && o->AnimationFrame <= 0.8f)
				PlayBackground(SOUND_UMBRELLA_MONSTER_DAMAGE);
		}
		break;
	case MONSTER01_DIE:
		{
			if (o->AnimationFrame > 0.0f && o->AnimationFrame <= 0.3f)
			{
				CreateEffect(ThreadIndex, MODEL_EFFECT_SKURA_ITEM, o->Position, o->Angle, o->Light, 1, o);
				EVector3 Position, Angle(0.0f), Light(1.0f);
				Position[0] = o->Position[0];
				Position[1] = o->Position[1];
				Position[2] = RequestTerrainHeight(Position[0], Position[1]);
				CreateEffect(ThreadIndex, BITMAP_FIRECRACKER0001, Position, Angle, Light, 0);
				CreateEffect(ThreadIndex, MODEL_EFFECT_UMBRELLA_DIE, o->Position, o->Angle, o->Light, 0, o);
				for (mu_int32 i = 0; i < 40; ++i)
				{
					CreateEffect(ThreadIndex, MODEL_EFFECT_UMBRELLA_GOLD, o->Position, o->Angle, o->Light, 0, o);
				}
			}
			if (o->AnimationFrame >= 0.5f && o->AnimationFrame <= 0.8f)
			{
				PlayBackground(SOUND_UMBRELLA_MONSTER_DEAD);
			}
		}
		break;
	}

	return true;
}