#include "stdafx.h"
#include "w_CursedTemple.h"

CursedTemple g_CursedTemple;

CursedTemple::CursedTemple() : m_IsTalkEnterNpc(false), m_HolyItemPlayerIndex(0xffff)
{
	Initialize();
}

CursedTemple::~CursedTemple()
{
	Destroy();
}

void CursedTemple::Initialize()
{
	ResetCursedTemple();
}

void CursedTemple::Destroy()
{
	if (m_TerrainWaterIndex.size() != 0) m_TerrainWaterIndex.clear();
}

void CursedTemple::ResetCursedTemple()
{
	m_CursedTempleState = eCursedTempleState_None;
	m_InterfaceState = false;
	m_AlliedPoint = 0;
	m_IllusionPoint = 0;
	m_ShowAlliedPointEffect = false;
	m_ShowIllusionPointEffect = false;
	m_bGaugebarEnabled = false;
	m_fGaugebarCloseTimer = 0;
}

void CursedTemple::SetInterfaceState(mu_boolean state, mu_int32 subtype)
{
	if (subtype == -1)
	{
		m_InterfaceState = state;
	}
	else if (subtype == 0)
	{
		if (m_CursedTempleState == eCursedTempleState_Wait
			|| m_CursedTempleState == eCursedTempleState_Ready)
		{
			m_InterfaceState = state;
		}
	}

	if (m_InterfaceState == false)
	{
		if (!g_pNewUISystem->IsVisible(GAMEUI::INTERFACE_CURSEDTEMPLE_RESULT))
		{
			g_pPickedItem.BackupPickedItem();
			g_pNewUISystem->HideAll();
		}
	}
}

mu_boolean CursedTemple::GetInterfaceState(mu_int32 type, mu_int32 subtype)
{
	if (!IsCursedTemple()) return true;

	mu_boolean result = m_InterfaceState;

	if (type == GAMEUI::INTERFACE_COMMAND)
	{
		if (subtype == UICommandWindow::UICMD_PARTY)
		{
			return false;
		}
		else if (subtype == UICommandWindow::UICMD_TRADE ||
			subtype == UICommandWindow::UICMD_BUY ||
			subtype == UICommandWindow::UICMD_DUEL)
		{
			return result;
		}
		else
		{
			return true;
		}
	}

	return result;
}

mu_boolean CursedTemple::IsPartyMember(mu_uint32 selectcharacterindex)
{
	if (PartyNumber == 0) return false;

	CHARACTER* c = &CharactersClient[selectcharacterindex];
	if (c == nullptr) return false;

	for (mu_int32 i = 0; i < PartyNumber; ++i)
	{
		PARTY_t* p = &Party[i];
		if (p->Key == c->Key) return true;
	}

	return false;
}

mu_boolean CursedTemple::SetCurrentActionMonster(CHARACTER* c, OBJECT* o)
{
	switch(c->MonsterIndex)
	{
	case 388:
	case 391:
	case 394:
	case 397:
	case 400:
	case 403:
		return CheckMonsterSkill(c, o);
	}

	return false;
}

CHARACTER* CursedTemple::CreateCharacters(mu_int32 iType, mu_int32 iPosX, mu_int32 iPosY, mu_int32 iKey)
{
	CHARACTER* pCharacter = nullptr;

	switch (iType)
	{
	case 380:
		{
			pCharacter = CreateCharacter(iKey, MODEL_CURSEDTEMPLE_STATUE, iPosX, iPosY);
			pCharacter->Object.m_fEdgeScale = 1.03f;
			pCharacter->Object.PKKey = 0;
		}
		break;
	case 381:
		{
			pCharacter = CreateCharacter(iKey, MODEL_CURSEDTEMPLE_ALLIED_NPC, iPosX, iPosY);
			pCharacter->Object.Scale = 1.2f;
		}
		break;
	case 382:
		{
			pCharacter = CreateCharacter(iKey, MODEL_CURSEDTEMPLE_ILLUSION_NPC, iPosX, iPosY);
			pCharacter->Object.Scale = 1.2f;
		}
		break;
	case 383:
		{
			pCharacter = CreateCharacter(iKey, MODEL_CURSEDTEMPLE_ALLIED_BASKET, iPosX, iPosY);
			pCharacter->Object.Scale = 1.8f;
			pCharacter->Object.m_fEdgeScale = 1.03f;
			m_ShowAlliedPointEffect = false;
		}
		break;
	case 384:
		{
			pCharacter = CreateCharacter(iKey, MODEL_CURSEDTEMPLE_ILLUSION__BASKET, iPosX, iPosY);
			pCharacter->Object.Scale = 1.5f;
			pCharacter->Object.m_fEdgeScale = 1.03f;
			m_ShowIllusionPointEffect = false;
		}
		break;
	case 385:
		{
			pCharacter = CreateCharacter(iKey, MODEL_CURSEDTEMPLE_ENTER_NPC, iPosX, iPosY);
			pCharacter->Object.Scale = 0.95f;
		}
		break;
	case 386:
	case 389:
	case 392:
	case 395:
	case 398:
	case 401:
		{
			pCharacter = CreateCharacter(iKey, MODEL_MONSTER01 + 123, iPosX, iPosY);
			pCharacter->Object.Scale = 1.0f;
			pCharacter->Weapon[0].Type = -1;
			pCharacter->Weapon[1].Type = -1;
		}
		break;
	case 387:
	case 390:
	case 393:
	case 396:
	case 399:
	case 402:
		{
			pCharacter = CreateCharacter(iKey, MODEL_MONSTER01 + 124, iPosX, iPosY);
			pCharacter->Object.Scale = 1.0f;
			pCharacter->Weapon[0].Type = -1;
			pCharacter->Weapon[1].Type = -1;
		}
		break;
	case 388:
	case 391:
	case 394:
	case 397:
	case 400:
	case 403:
		{
			pCharacter = CreateCharacter(iKey, MODEL_MONSTER01 + 125, iPosX, iPosY);
			pCharacter->Object.Scale = 1.0f;
			pCharacter->Weapon[0].Type = -1;
			pCharacter->Weapon[1].Type = -1;
		}
		break;
	case 404:
		{
			pCharacter = CreateCharacter(iKey, MODEL_PLAYER, iPosX, iPosY);
			pCharacter->Object.Scale = 1.0f;
			pCharacter->Object.SubType = MODEL_CURSEDTEMPLE_ALLIED_PLAYER;
		}
		break;
	case 405:
		{
			pCharacter = CreateCharacter(iKey, MODEL_PLAYER, iPosX, iPosY);
			pCharacter->Object.Scale = 1.0f;
			pCharacter->Object.SubType = MODEL_CURSEDTEMPLE_ILLUSION_PLAYER;
		}
		break;
	}

	return pCharacter;
}

mu_boolean CursedTemple::AttackEffectMonster(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b)
{
	EVector3 p, Light;
	Vector(0.0f, 0.0f, 0.0f, p);
	Vector(1.0f, 1.0f, 1.0f, Light);
	switch (c->MonsterIndex)
	{
	case 388:
	case 391:
	case 394:
	case 397:
	case 400:
	case 403:
		{
			if (o->CurrentAction == MONSTER01_ATTACK1)
			{
				if (c->TargetCharacter != -1)
				{
					CHARACTER *tc = &CharactersClient[c->TargetCharacter];
					OBJECT *to = &tc->Object;

					Vector(0.4f, 0.6f, 1.0f, Light);

					for (mu_int32 i = 0; i < 5; ++i)
						CreateParticle(ThreadIndex, BITMAP_SMOKE, to->Position, o->Angle, Light, 1);

					PlayBackground(SOUND_HEART);
				}
			}
		}
		return true;
	}

	return false;
}

mu_boolean CursedTemple::MoveObject(OBJECT* o)
{
	if (m_bGaugebarEnabled == true && FWorldTime - m_fGaugebarCloseTimer > 3000.0f)
	{
		SetGaugebarEnabled(false);
	}

	switch (GET_WORLD)
	{
	case WD_45CURSEDTEMPLE_LV1:
	case WD_45CURSEDTEMPLE_LV2:
	case WD_45CURSEDTEMPLE_LV3:
	case WD_45CURSEDTEMPLE_LV4:
	case WD_45CURSEDTEMPLE_LV5:
	case WD_45CURSEDTEMPLE_LV6:
		{
			switch (o->Type)
			{
			case 64:
			case 65:
			case 80:
				o->BlendMeshLight = (mu_float)EMath::Sin(FWorldTime*0.0010f)*0.5f + 0.5f;
				break;
			case 70:
			case 71:
			case 72:
			case 73:
			case 74:
			case 75:
			case 76:
			case 77:
			case 78:
			case 79:
				o->HiddenMesh = -2;
				break;
			}
		}
		return true;
	}

	return false;
}

void CursedTemple::MoveMonsterSoundVisual(OBJECT* o, MUModel* b)
{
	mu_float fActionSpeed = b->_AnimationHeader[o->CurrentAction].Speed;

	switch (o->Type)
	{
	case MODEL_MONSTER01 + 123:
	case MODEL_MONSTER01 + 124:
		{
			if (o->CurrentAction == MONSTER01_STOP1)
			{
				if (o->AnimationFrame > 0.5f && o->AnimationFrame < (0.7f + fActionSpeed))
				{
					PlayBackground(SOUND_CURSEDTEMPLE_MONSTER1_IDLE);
				}
			}
			else if (o->CurrentAction == MONSTER01_WALK)
			{
				if (o->AnimationFrame > 0.5f && o->AnimationFrame < (0.7f + fActionSpeed))
				{
					PlayBackground(SOUND_CURSEDTEMPLE_MONSTER_MOVE);
				}
			}
			else if (o->CurrentAction == MONSTER01_DIE)
			{
				if (o->AnimationFrame > 0.5f && o->AnimationFrame < (0.7f + fActionSpeed))
				{
					PlayBackground(SOUND_CURSEDTEMPLE_MONSTER1_DEATH);
				}
			}

			if (o->DamageTime == 14)
			{
				if (o->AnimationFrame > 0.5f && o->AnimationFrame < (0.7f + fActionSpeed))
				{
					PlayBackground(SOUND_CURSEDTEMPLE_MONSTER1_DAMAGE);
				}
			}
		}
		break;
	case MODEL_MONSTER01 + 125:
		{
			if (o->CurrentAction == MONSTER01_WALK)
			{
				if (o->AnimationFrame > 0.5f && o->AnimationFrame < (0.7f + fActionSpeed))
				{
					PlayBackground(SOUND_CURSEDTEMPLE_MONSTER_MOVE);
				}
			}
			else if (o->CurrentAction == MONSTER01_DIE)
			{
				if (o->AnimationFrame > 0.5f && o->AnimationFrame < (0.7f + fActionSpeed))
				{
					PlayBackground(SOUND_CURSEDTEMPLE_MONSTER2_DEATH);
				}
			}

			if (o->DamageTime == 14)
			{
				if (o->AnimationFrame > 0.5f && o->AnimationFrame < (0.7f + fActionSpeed))
				{
					PlayBackground(SOUND_CURSEDTEMPLE_MONSTER2_DAMAGE);
				}
			}
		}
		break;
	}
}

mu_boolean CursedTemple::MoveMonsterVisual(OBJECT* o, MUModel* b)
{
	switch(o->Type)
	{
	case MODEL_CURSEDTEMPLE_ALLIED_NPC:
		{
			o->Position[2] = 225.0f;
		}
		return true;
	case MODEL_CURSEDTEMPLE_ILLUSION_NPC:
		{
			o->Position[2] = 250.0f;
		}
		return true;
	}

	return false;
}

void CursedTemple::MoveBlurEffect(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	switch (o->Type)
	{
	case MODEL_MONSTER01 + 125:
		{
			if (o->CurrentAction == MONSTER01_ATTACK1 || o->CurrentAction == MONSTER01_ATTACK2)
			{
				EVector3  Light;
				Vector(1.0f, 0.2f, 0.5f, Light);

				EVector3 StartPos, StartRelative;
				EVector3 EndPos, EndRelative;

				mu_float fActionSpeed = b->_AnimationHeader[o->CurrentAction].Speed;
				mu_float fSpeedPerFrame = fActionSpeed / 10.0;
				mu_float fAnimationFrame = o->AnimationFrame - fActionSpeed;
				for (mu_int32 i = 0; i < 10; ++i)
				{
					b->Animation(ThreadIndex, ThreadInfo.BoneTransform, fAnimationFrame, o->PriorAnimationFrame, o->PriorAction, o->Angle, o->HeadAngle);

					Vector(0.0f, 0.0f, 0.0f, StartRelative);
					Vector(0.0f, 0.0f, 0.0f, EndRelative);

					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(19)], StartRelative, StartPos, false);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(21)], EndRelative, EndPos, false);
					CreateBlur(c, StartPos, EndPos, Light, 1);

					Vector(0.0f, 0.0f, 0.0f, StartRelative);
					Vector(0.0f, 0.0f, 0.0f, EndRelative);

					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(25)], StartRelative, StartPos, false);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(27)], EndRelative, EndPos, false);
					CreateBlur(c, StartPos, EndPos, Light, 1);

					fAnimationFrame += fSpeedPerFrame;
				}

				for (mu_int32 j = 0; j < 10; ++j)
				{
					b->Animation(ThreadIndex, ThreadInfo.BoneTransform, fAnimationFrame, o->PriorAnimationFrame, o->PriorAction, o->Angle, o->HeadAngle);

					Vector(0.0f, 0.0f, 0.0f, StartRelative);
					Vector(0.0f, 0.0f, 0.0f, EndRelative);

					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(25)], StartRelative, StartPos, false);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(27)], EndRelative, EndPos, false);
					CreateBlur(c, StartPos, EndPos, Light, 1);

					fAnimationFrame += fSpeedPerFrame;
				}
			}
		}
		break;
	}
}

mu_boolean CursedTemple::RenderObjectVisual(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	EVector3 Light;

	switch (o->Type)
	{
	case 54:
		{
			EVector3 position;
			VectorCopy(o->Position, position);
			position[2] -= 100.0f;
			Vector(0.15f, 0.15f, 0.15f, o->Light);
			CreateParticle(ThreadIndex, BITMAP_EFFECT, position, o->Angle, o->Light);
			CreateParticle(ThreadIndex, BITMAP_EFFECT, position, o->Angle, o->Light, 3);
		}
		break;
	case 62:
		{
			// FIX check why this is using ThreadInfo.BoneTransform
			EVector3 vPos, vRelativePos, vLight;
			Vector(0.0f, 0.0f, 0.0f, vPos);
			Vector(6.0f, 5.0f, 2.0f, vRelativePos);
			Vector(1.0f, 1.0f, 1.0f, vLight);
			mu_float fLumi = EMath::Sin(FWorldTime*0.001f)*0.5f + 0.5f;
			b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(22)], vRelativePos, vPos);
			Vector(1.0f, 0.5f, 0.5f, vLight);
			CreateSprite(BITMAP_SHINY + 1, vPos, 1.5f + fLumi * 0.5f, vLight, nullptr);
			Vector(6.0f, -5.0f, 2.0f, vRelativePos);
			b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(23)], vRelativePos, vPos);
			Vector(1.0f, 0.5f, 0.5f, vLight);
			CreateSprite(BITMAP_SHINY + 1, vPos, 1.5f + fLumi * 0.5f, vLight, nullptr);

			if (GetLargeRand(5) == 0)
			{
				Vector(0.0f, 0.0f, 0.0f, vRelativePos);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(20)], vRelativePos, vPos);
				Vector(1.0f, 1.0f, 1.0f, vLight);
				CreateParticle(ThreadIndex, BITMAP_SMOKE, vPos, o->Angle, vLight);
			}
		}
		break;
	case 70:
		{
			if (GetLargeRand(3) == 0)
			{
				mu_float fLumi = (GetLargeRand(10)) * 0.007f + 0.03f;
				Vector(54.0f / 256.0f*fLumi, 177.0f / 256.0f*fLumi, 150.0f / 256.0f*fLumi, Light);
				CreateParticle(ThreadIndex, BITMAP_CLOUD, o->Position, o->Angle, Light, 15, o->Scale, o);
			}
		}
		return true;
	case 71:
		{
			if (GetLargeRand(3) == 0)
			{
				mu_float fLumi = (GetLargeRand(10)) * 0.007f + 0.03f;
				Vector(221.0f / 256.0f*fLumi, 121.0f / 256.0f*fLumi, 201.0f / 256.0f*fLumi, Light);
				CreateParticle(ThreadIndex, BITMAP_CLOUD, o->Position, o->Angle, Light, 15, o->Scale, o);
			}
		}
		return true;
	case 72:
		{
			if (GetLargeRand(3) == 0)
			{
				mu_float fLumi = (GetLargeRand(10)) * 0.007f + 0.03f;
				Vector(54.0f / 256.0f*fLumi, 177.0f / 256.0f*fLumi, 150.0f / 256.0f*fLumi, Light);
				CreateParticle(ThreadIndex, BITMAP_CLOUD, o->Position, o->Angle, Light, 15, o->Scale, o);
				Vector(221.0f / 256.0f*fLumi, 121.0f / 256.0f*fLumi, 201.0f / 256.0f*fLumi, Light);
				CreateParticle(ThreadIndex, BITMAP_CLOUD, o->Position, o->Angle, Light, 15, o->Scale, o);
			}
		}
		return true;
	case 73:
		{
			if (GetLargeRand(3) == 0)
			{
				mu_float fLumi = (GetLargeRand(10)) * 0.002f + 0.03f;
				Vector(1.2f*fLumi, 1.2f*fLumi, 1.2f*fLumi, Light);
				CreateParticle(ThreadIndex, BITMAP_CLOUD, o->Position, o->Angle, Light, 15, o->Scale, o);
			}
		}
		return true;
	case 74:
		{
			if (GetLargeRand(2) == 0)
			{
				Vector(0.0f, 0.0f, 0.0f, Light);
				CreateParticle(ThreadIndex, BITMAP_CLOUD, o->Position, o->Angle, Light, 16, o->Scale, o);
			}
		}
		return true;
	case 75:
		{
			if (GetLargeRand(35) == 5)
			{
				mu_float fLumi = (GetLargeRand(10)) * 0.05f + 0.03f;
				Vector(256.0f / 256.0f*fLumi, 256.0f / 256.0f*fLumi, 256.0f / 256.0f*fLumi, Light);
				CreateParticle(ThreadIndex, BITMAP_GHOST_CLOUD1, o->Position, o->Angle, Light, 0, o->Scale, o);
			}
		}
		return true;
	case 76:
		{
			mu_float fLumi = (GetLargeRand(100))*0.01;
			Vector(180.0f / 255.0f + fLumi, 71.0f / 255.0f, 55.0f / 255.0f, Light);
			EVector3 vPos;
			VectorCopy(o->Position, vPos);
			for (mu_int32 i = 0; i < 1; ++i)
			{
				CreateParticle(ThreadIndex, BITMAP_TORCH_FIRE, vPos, o->Angle, Light, 0, o->Scale, o);
			}
			VectorCopy(o->Position, vPos);
			vPos[2] += 20.0f;
			CreateSprite(BITMAP_LIGHT, vPos, o->Scale*6.0f, Light, o);
		}
		return true;
	case 77:
		{
			mu_float fLumi = (GetLargeRand(100))*0.005;
			Vector(55.0f / 256.0f, 71.0f / 256.0f, 180.0f / 256.0f + fLumi, Light);
			EVector3 vPos;
			VectorCopy(o->Position, vPos);
			for (mu_int32 i = 0; i < 1; ++i)
			{
				CreateParticle(ThreadIndex, BITMAP_TORCH_FIRE, vPos, o->Angle, Light, 0, o->Scale, o);
			}
			VectorCopy(o->Position, vPos);
			vPos[2] += 20.0f;
			CreateSprite(BITMAP_LIGHT, vPos, o->Scale*6.0f, Light, o);
		}
		return true;
	case 78:
		{
			mu_int32 iTime = g_CurrentTime % 500;
			mu_int32 iRand = GetLargeRand(485);
			if (iTime >= iRand + (GetLargeRand(5)) && iTime < iRand + (GetLargeRand(10) + 5))
			{
				Vector(1.0f, 0.8f, 0.8f, Light);
				for (mu_int32 i = 0; i < 4; ++i)
				{
					CreateEffect(ThreadIndex, MODEL_FALL_STONE_EFFECT, o->Position, o->Angle, Light);
				}
				CreateEffect(ThreadIndex, MODEL_FALL_STONE_EFFECT, o->Position, o->Angle, Light, 1);
				Vector(0.7f, 0.7f, 0.8f, Light);
				EVector3 vPos;
				VectorCopy(o->Position, vPos);
				vPos[0] += (mu_float)(GetLargeRand(80) - 40);
				vPos[1] += (mu_float)(GetLargeRand(80) - 40);
				CreateParticle(ThreadIndex, BITMAP_WATERFALL_3 + (GetLargeRand(2)), vPos, o->Angle, Light, 2);
				Vector(0.9f, 0.0f, 0.0f, Light);
				CreateParticle(ThreadIndex, BITMAP_SMOKE, o->Position, o->Angle, Light, 0, 1.5f);
			}
		}
		return true;
	case 79:
		{
			if (GetLargeRand(1) == 0)
			{
				for (mu_int32 i = 0; i < 5; ++i)
				{
					mu_float fLumi = (GetLargeRand(10)) * 0.03f + 0.008f;
					Vector(100.0f / 256.0f*fLumi, 110.0f / 256.0f*fLumi, 160.0f / 256.0f*fLumi, Light);
					CreateParticle(ThreadIndex, BITMAP_CLOUD, o->Position, o->Angle, Light, 17, o->Scale, o);
				}
			}
		}
		return true;
	}

	return true;
}

mu_boolean CursedTemple::RenderMonsterVisual(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b)
{
	EVector3 Light;

	MoveMonsterSoundVisual(o, b);

	switch (o->Type)
	{
	case MODEL_CURSEDTEMPLE_ILLUSION_NPC:
		{
			EVector3 vRelativePos, vWorldPos;
			mu_int32 boneindex[11] = { 34, 48, 49, 20, 45, 19, 44, 22, 21, 47, 46 };
			for (mu_int32 i = 0; i < 11; ++i)
			{
				Vector(0.0f, 0.0f, 0.0f, vRelativePos);
				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(boneindex[i])], vRelativePos, vWorldPos, true);
				Vector(0.4f, 0.4f, 1.0f, Light);
				CreateSprite(BITMAP_LIGHT, vWorldPos, 0.7f, Light, o);
			}

			Vector(0.0f, 0.0f, 0.0f, Light);

			for (mu_int32 j = 0; j < 3; ++j)
			{
				mu_int32 randtemp = GetLargeRand(4);
				if (randtemp == 0)
				{
					Vector(static_cast<mu_float>(GetLargeRand(120)), static_cast<mu_float>(GetLargeRand(70)), static_cast<mu_float>(GetLargeRand(50)), vRelativePos);
				}
				else if (randtemp == 1)
				{
					Vector(-static_cast<mu_float>(GetLargeRand(120)), -static_cast<mu_float>(GetLargeRand(70)), static_cast<mu_float>(GetLargeRand(50)), vRelativePos);
				}
				else if (randtemp == 2)
				{
					Vector(static_cast<mu_float>(GetLargeRand(120)), -static_cast<mu_float>(GetLargeRand(70)), static_cast<mu_float>(GetLargeRand(50)), vRelativePos);
				}
				else if (randtemp == 3)
				{
					Vector(-static_cast<mu_float>(GetLargeRand(120)), static_cast<mu_float>(GetLargeRand(70)), static_cast<mu_float>(GetLargeRand(50)), vRelativePos);
				}

				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(1)], vRelativePos, vWorldPos, true);
				CreateParticle(ThreadIndex, BITMAP_CLUD64, vWorldPos, o->Angle, Light, 0, 0.7f);
			}
		}
		break;
	case MODEL_CURSEDTEMPLE_ENTER_NPC:
		{
			EVector3 vRelativePos, vWorldPos;

			if (o->CurrentAction == MONSTER01_STOP2)
			{
				mu_float fActionSpeed = b->_AnimationHeader[o->CurrentAction].Speed;

				if (o->AnimationFrame > 0.5f && o->AnimationFrame < (8.5f + fActionSpeed))
				{
					Vector(0.0f, 0.0f, 0.0f, vRelativePos);
					b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(22)], vRelativePos, vWorldPos, true);

					Vector(0.8f, 0.8f, 1.0f, Light);
					CreateParticle(ThreadIndex, BITMAP_LIGHT + 2, vWorldPos, o->Angle, Light, 2, 0.7f);

					Light[0] = static_cast<mu_float>((GetLargeRand(100)) * 0.01f);
					Light[1] = static_cast<mu_float>((GetLargeRand(100)) * 0.01f);
					Light[2] = static_cast<mu_float>((GetLargeRand(100)) * 0.01f);

					CreateParticle(ThreadIndex, BITMAP_SHINY, vWorldPos, o->Angle, Light, 3, 0.8f);
				}

				if (o->AnimationFrame > 7.3f && o->AnimationFrame < (7.5f + fActionSpeed))
				{
					Vector(0.7f, 0.7f, 1.0f, Light);
					CreateParticle(ThreadIndex, BITMAP_GM_AURORA, vWorldPos, o->Angle, Light, 3, 0.7f);
				}
			}
		}
		return true;

	case MODEL_MONSTER01 + 125:
		{
			EVector3 vRelativePos, vWorldPos;
			Vector(0.0f, 0.0f, 0.0f, vRelativePos);
			if (o->CurrentAction == MONSTER01_DIE)
			{
				mu_int32 boneindex[6] = { 6, 2, 19, 25, 35, 40 };

				for (mu_int32 i = 0; i < 6; ++i)
				{
					b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(boneindex[i])], vRelativePos, vWorldPos, true);
					CreateParticle(ThreadIndex, BITMAP_SMOKE + 3, vWorldPos, o->Angle, Light, 3, 0.5f);
				}
			}
		}
		return true;
	case MODEL_MONSTER01 + 124:
		{
			EVector3 vRelativePos, vWorldPos;
			mu_float fLuminosity = (mu_float)EMath::Sin((FWorldTime)*0.002f)*0.2f;

			Vector(0.4f, 0.4f, 1.0f, Light);
			Vector(-2.0f, 14.0f, 0.0f, vRelativePos);
			b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(8)], vRelativePos, vWorldPos, true);
			CreateSprite(BITMAP_SHINY + 1, vWorldPos, 0.9f, Light, o, -FWorldTime*0.08f);
			CreateSprite(BITMAP_LIGHT + 3, vWorldPos, 0.8f, Light, o, FWorldTime*0.3f);

			Vector(0.0f, 0.0f, 0.0f, vRelativePos);

			if (o->CurrentAction == MONSTER01_ATTACK1 || o->CurrentAction == MONSTER01_ATTACK2)
			{
				Vector(0.4f, 0.4f, 1.0f, Light);
				Vector(0.0f, 0.0f, 0.0f, vRelativePos);
				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(17)], vRelativePos, vWorldPos, true);
				CreateSprite(BITMAP_LIGHT, vWorldPos, 3.0f, Light, o, 0.0f);
				CreateParticle(ThreadIndex, BITMAP_CLUD64, vWorldPos, o->Angle, Light, 4, 0.8f);
				CreateParticle(ThreadIndex, BITMAP_SPARK + 1, vWorldPos, o->Angle, Light, 15, 0.7f + (fLuminosity*0.05f));

				Vector(0.0f, 0.0f, 0.0f, vRelativePos);
				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(41)], vRelativePos, vWorldPos, true);
				CreateSprite(BITMAP_LIGHT, vWorldPos, 3.0f, Light, o, 0.0f);
				CreateParticle(ThreadIndex, BITMAP_CLUD64, vWorldPos, o->Angle, Light, 4, 0.8f);
				CreateParticle(ThreadIndex, BITMAP_SPARK + 1, vWorldPos, o->Angle, Light, 15, 0.7f + (fLuminosity*0.05f));
			}
			else if (o->CurrentAction == MONSTER01_DIE)
			{
				mu_int32 boneindex[6] = { 7, 2, 14, 38, 73, 78 };

				for (mu_int32 i = 0; i < 4; ++i)
				{
					b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(boneindex[i])], vRelativePos, vWorldPos, true);
					CreateParticle(ThreadIndex, BITMAP_SMOKE + 3, vWorldPos, o->Angle, Light, 3, 0.5f);
				}
			}
		}
		return true;
	case MODEL_MONSTER01 + 123:
		{
			EVector3 vRelativePos, vWorldPos;

			Vector(0.6f, 0.0f, 0.0f, Light);
			Vector(-2.0f, 14.0f, 0.0f, vRelativePos);
			b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(8)], vRelativePos, vWorldPos, true);
			CreateSprite(BITMAP_SHINY + 1, vWorldPos, 0.5f, Light, o, -FWorldTime*0.08f);
			CreateSprite(BITMAP_LIGHT + 3, vWorldPos, 0.6f, Light, o, FWorldTime*0.3f);

			Vector(0.0f, 0.0f, 0.0f, vRelativePos);

			if (o->CurrentAction == MONSTER01_ATTACK1 || o->CurrentAction == MONSTER01_ATTACK2)
			{
				Vector(0.6f, 0.0f, 0.0f, Light);
				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(17)], vRelativePos, vWorldPos, true);
				CreateEffect(ThreadIndex, MODEL_FENRIR_THUNDER, vWorldPos, o->Angle, Light, 2, o);
				CreateEffect(ThreadIndex, MODEL_FENRIR_THUNDER, vWorldPos, o->Angle, Light, 2, o);
				CreateEffect(ThreadIndex, MODEL_FENRIR_THUNDER, vWorldPos, o->Angle, Light, 2, o);
				CreateParticle(ThreadIndex, BITMAP_CLUD64, vWorldPos, o->Angle, Light, 4, 1.0f);
				CreateSprite(BITMAP_LIGHT, vWorldPos, 3.0f, Light, o, 0.0f);

				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(41)], vRelativePos, vWorldPos, true);
				CreateEffect(ThreadIndex, MODEL_FENRIR_THUNDER, vWorldPos, o->Angle, Light, 2, o);
				CreateEffect(ThreadIndex, MODEL_FENRIR_THUNDER, vWorldPos, o->Angle, Light, 2, o);
				CreateEffect(ThreadIndex, MODEL_FENRIR_THUNDER, vWorldPos, o->Angle, Light, 2, o);
				CreateEffect(ThreadIndex, MODEL_FENRIR_THUNDER, vWorldPos, o->Angle, Light, 2, o);
				CreateParticle(ThreadIndex, BITMAP_CLUD64, vWorldPos, o->Angle, Light, 4, 1.0f);
				CreateSprite(BITMAP_LIGHT, vWorldPos, 3.0f, Light, o, 0.0f);
			}
			else if (o->CurrentAction == MONSTER01_DIE)
			{
				mu_int32 boneindex[6] = { 7, 2, 14, 38, 73, 78 };

				for (mu_int32 i = 0; i < 6; ++i)
				{
					b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(boneindex[i])], vRelativePos, vWorldPos, true);
					CreateParticle(ThreadIndex, BITMAP_SMOKE + 3, vWorldPos, o->Angle, Light, 3, 0.5f);
				}
			}
		}
		return true;
	case MODEL_CURSEDTEMPLE_STATUE:
		if (o->CurrentAction == MONSTER01_DIE)
		{
			EVector3 vRelativePos, vWorldPos, Light;
			Vector(1.0f, 1.0f, 1.0f, Light);
			Vector(0.0f, 0.0f, 0.0f, vRelativePos);
			b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(13)], vRelativePos, vWorldPos, true);
			vRelativePos[0] = vWorldPos[0];
			vRelativePos[1] = vWorldPos[1];
			vRelativePos[2] = vWorldPos[2];
			if (o->PKKey == 0)
			{
				o->PKKey = 1;
				EarthQuake.Store((mu_float)(GetLargeRand(16) - 8)*0.1f);
				vWorldPos[2] = vRelativePos[2] + 250;
				CreateEffect(ThreadIndex, MODEL_CURSEDTEMPLE_STATUE_PART2, vWorldPos, o->Angle, Light, 0, o, 0, 0);
				for (mu_int32 i = 0; i < 60; ++i)
				{
					vWorldPos[0] = vRelativePos[0] + GetLargeRand(80) - 40;
					vWorldPos[1] = vRelativePos[1] + GetLargeRand(80) - 40;
					vWorldPos[2] = vRelativePos[2] + (GetLargeRand(250));
					CreateEffect(ThreadIndex, MODEL_CURSEDTEMPLE_STATUE_PART1, vWorldPos, o->Angle, Light, 0, o, 0, 0);
				}
				Vector(0.5f, 0.5f, 0.5f, Light);
				for (mu_int32 i = 0; i < 160; ++i)
				{
					vWorldPos[0] = vRelativePos[0] + GetLargeRand(140) - 70;
					vWorldPos[1] = vRelativePos[1] + GetLargeRand(140) - 70;
					vWorldPos[2] = vRelativePos[2] + (GetLargeRand(400)) - 100;
					CreateParticle(ThreadIndex, BITMAP_SMOKE, vWorldPos, o->Angle, Light, 48, 1.0f);
				}
			}

		}
		else
		{
			EVector3 vRelativePos, vWorldPos, Light;
			Vector(0.2f, 0.3f, 0.4f + (GetLargeRand(3))*0.1f, Light);
			Vector(0.0f, 0.0f, 0.0f, vRelativePos);
			b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(13)], vRelativePos, vWorldPos, true);
			CreateParticle(ThreadIndex, BITMAP_FLARE + 1, vWorldPos, o->Angle, Light, 0, 0.15f);
			vWorldPos[2] += 30;
			CreateParticle(ThreadIndex, BITMAP_LIGHT, vWorldPos, o->Angle, Light, 1, 8.0f);

			vWorldPos[2] += 160;
			Vector(0.2f, 0.1f, 0.0f, Light);
			b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(14)], vRelativePos, vWorldPos, true);
			CreateParticle(ThreadIndex, BITMAP_LIGHT, vWorldPos, o->Angle, Light, 1, 4.0f);
		}
		return true;
	case MODEL_CURSEDTEMPLE_ALLIED_BASKET:
		{
			EVector3 vRelativePos, vWorldPos, Light;
			if (m_ShowAlliedPointEffect)
			{
				Vector(1.0f, 1.0f, 0.5f, Light);
				Vector(0.0f, 0.0f, 0.0f, vRelativePos);
				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(5)], vRelativePos, vWorldPos, true);
				CreateEffect(ThreadIndex, BITMAP_FLARE, vWorldPos, o->Angle, Light, 1);

				Vector(0.9f, 0.7f, 0.4f, Light);
				CreateEffect(ThreadIndex, BITMAP_MAGIC, o->Position, o->Angle, Light, 8);

				m_ShowAlliedPointEffect = false;
			}
		}
		return true;
	case MODEL_CURSEDTEMPLE_ILLUSION__BASKET:
		{
			EVector3 vRelativePos, vWorldPos, Light;
			if (m_ShowIllusionPointEffect)
			{
				Vector(0.5f, 1.0f, 1.0f, Light);
				Vector(0.0f, 0.0f, 0.0f, vRelativePos);
				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(3)], vRelativePos, vWorldPos, true);
				CreateEffect(ThreadIndex, BITMAP_FLARE, vWorldPos, o->Angle, Light, 2);

				Vector(0.7f, 0.8f, 0.9f, Light);
				CreateEffect(ThreadIndex, BITMAP_MAGIC, o->Position, o->Angle, Light, 8);

				m_ShowIllusionPointEffect = false;
			}
		}
		return true;
	}
	return false;
}

void CursedTemple::UpdateTempleSystemMsg(mu_int32 _Value)
{
	mu_text szText[256] = { 0, };

	switch (_Value)
	{
	case 0:
		break;
	case 1:
		break;
	case 2:
		break;
	case 3:
		g_pChatListBox->AddText(_T(""), GlobalText[2367], TYPE_ERROR_MESSAGE);
		break;
	case 4:
		g_pChatListBox->AddText(_T(""), GlobalText[2368], TYPE_ERROR_MESSAGE);
		break;
	case 5:
		mu_sprintf_s(szText, mu_countof(szText), GlobalText[829], 6);
		g_pChatListBox->AddText(_T(""), szText, TYPE_ERROR_MESSAGE);
		break;
	case 6:
		break;
	case 7:
		g_pChatListBox->AddText(_T(""), GlobalText[2865], TYPE_ERROR_MESSAGE);
		break;
	case 8:
		g_pChatListBox->AddText(_T(""), GlobalText[2175], TYPE_ERROR_MESSAGE);
		break;
	}
}

void CursedTemple::SetGaugebarEnabled(mu_boolean bFlag)
{
	m_bGaugebarEnabled = bFlag;
	m_fGaugebarCloseTimer = FWorldTime + 10000000.0f;
}

void CursedTemple::SetGaugebarCloseTimer()
{
	m_fGaugebarCloseTimer = FWorldTime;
}

mu_boolean CursedTemple::IsGaugebarEnabled()
{
	return m_bGaugebarEnabled; 
}

mu_boolean CursedTemple::RenderObjectMesh(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b, mu_boolean ExtraMon)
{
	switch (o->Type)
	{
	case MODEL_CURSEDTEMPLE_STATUE:
		if (o->CurrentAction != MONSTER01_DIE)
		{
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			b->RenderMesh(ThreadIndex, 3, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			b->RenderMesh(ThreadIndex, 3, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			b->RenderMesh(ThreadIndex, 4, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			b->RenderMesh(ThreadIndex, 5, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		return true;
	case MODEL_CURSEDTEMPLE_ALLIED_BASKET:
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		return true;
	case MODEL_CURSEDTEMPLE_ILLUSION__BASKET:
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		return true;
	case MODEL_CURSEDTEMPLE_ILLUSION_NPC:
		{
			mu_float fLumi = (EMath::Sin(FWorldTime*0.002f) + 1.0f) * 0.5f;
			mu_float fLumi2 = (EMath::Sin(FWorldTime*0.002f) + 1.0f);

			b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 5, fLumi2, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_CURSEDTEMPLE_NPC_MESH_EFFECT);
			b->RenderMesh(ThreadIndex, 3, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 2, fLumi);
		}
		break;
	case MODEL_CURSEDTEMPLE_ENTER_NPC:
		{
			Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
			b->RenderMesh(ThreadIndex, 0, RENDER_CHROME, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);

			Vector(10.0f, 10.0f, 10.0f, b->BodyLight(ThreadIndex));
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 3, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 3, o->BlendMeshLight, o->BlendMeshTexCoordU, IWorldTime % 2000 * 0.001f);
		}
		return true;
	case MODEL_MONSTER01 + 123:
	case MODEL_MONSTER01 + 124:
		{
			b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			mu_float fLumi = (EMath::Sin(FWorldTime*0.002f) + 1.0f) * 0.8f;
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, FWorldTime*0.02f, 0, fLumi*4.0f, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			fLumi = (EMath::Sin(FWorldTime*0.006f) + 1.0f) * 0.9f;
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, FWorldTime*0.02f, 0, fLumi*3.0f, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 3, RENDER_TEXTURE | RENDER_BRIGHT, FWorldTime*0.002f, 0, o->BlendMeshLight*2.0f, -FWorldTime*0.002f, FWorldTime*0.02f);
		}
		return true;
	case 54:
		{
			mu_float fLumi = (EMath::Sin(FWorldTime*0.002f) + 1.0f) * 0.4f;
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, fLumi + 0.3f, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, 2);
			b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		return true;
	case 64:
	case 65:
	case 66:
	case 80:
		{
			if (o->Type == 64 || o->Type == 65 || o->Type == 66 || o->Type == 80)
			{
				b->RenderBody(ThreadIndex, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			}
		}
		return true;
	}
	return false;
}

void CursedTemple::PlayBGM()
{
	if (m_CursedTempleState == eCursedTempleState_Wait)
	{
		EMusicBackend::Play(MUSICS::Data[MUSIC_CURSEDTEMPLE_WAIT]);
	}
	else if( m_CursedTempleState == eCursedTempleState_Ready
		  || m_CursedTempleState == eCursedTempleState_Play )
	{
		EMusicBackend::Stop(MUSICS::Data[MUSIC_CURSEDTEMPLE_WAIT]);
		EMusicBackend::Play(MUSICS::Data[MUSIC_CURSEDTEMPLE_GAME]);
	}
	else if (m_CursedTempleState == eCursedTempleState_None )
	{
		EMusicBackend::Stop(MUSICS::Data[MUSIC_CURSEDTEMPLE_WAIT]);
		EMusicBackend::Stop(MUSICS::Data[MUSIC_CURSEDTEMPLE_GAME]);
	}
}

mu_boolean CursedTemple::IsHolyItemPickState()
{
	if( !IsCursedTemple() ) return false;

	if( m_CursedTempleState != eCursedTempleState_Play )
	{
		return false;
	}

	return true;
}

void CursedTemple::ReceiveCursedTempleState( const eCursedTempleState state )
{
	m_CursedTempleState = state;

	if( m_CursedTempleState == eCursedTempleState_Wait )
	{
		SetInterfaceState(false);
	}
	else if( m_CursedTempleState == eCursedTempleState_Ready )
	{
		SetInterfaceState(false);
	}
	else if( m_CursedTempleState == eCursedTempleState_Play )
	{
		m_TerrainWaterIndex.clear();
#if 0 // FIX
		SetTerrainWaterState( m_TerrainWaterIndex, 0 );
#endif
		SetInterfaceState(false);
	}
	else if( m_CursedTempleState == eCursedTempleState_End )
	{
#if 0 // FIX
		SetTerrainWaterState( m_TerrainWaterIndex, 1 );
#endif
		m_TerrainWaterIndex.clear();
		SetInterfaceState(false);
		g_pPickedItem.BackupPickedItem();
	}
}

void CursedTemple::ReceiveCursedTempleInfo( mu_uint8* ReceiveBuffer )
{
#if 0 // FIX
	LPPMSG_CURSED_TAMPLE_STATE data =(LPPMSG_CURSED_TAMPLE_STATE)ReceiveBuffer;
	
	if( m_AlliedPoint != data->btAlliedPoint )
	{
		m_ShowAlliedPointEffect = true;
	}
	else if( m_IllusionPoint != data->btIllusionPoint )
	{
		m_ShowIllusionPointEffect = true;
	}
	m_AlliedPoint   = data->btAlliedPoint;
	m_IllusionPoint = data->btIllusionPoint;

	if( m_HolyItemPlayerIndex != 0xffff && data->btUserIndex == 0xffff )
	{
		DeleteEffect( MODEL_CURSEDTEMPLE_HOLYITEM );
	}
	
	if( data->btUserIndex != 0xffff )
	{
		mu_uint16 holyitemkey = data->btUserIndex;
 		mu_uint16 holyitemcharacterindex = FindCharacterIndex( holyitemkey );

		if( holyitemcharacterindex != MAX_CHARACTERS_CLIENT )
		{
			CHARACTER* c = &CharactersClient[holyitemcharacterindex];
			OBJECT* o    = &c->Object;
		
			if( o->Live && !SearchEffect( MODEL_CURSEDTEMPLE_HOLYITEM, o ) )
			{
				PlayBackground( SOUND_CURSEDTEMPLE_GAMESYSTEM3 );

				EVector3 tempPosition, p;
				Vector(70.0f,0.0f,0.0f,p);
				MUModel *b = &Models[o->Type];
				VectorCopy(o->Position,b->BodyOrigin);

				mu_float fActionSpeed = b->_AnimationHeader[o->CurrentAction].Speed;
				mu_float fAnimationFrame = o->AnimationFrame - fActionSpeed;
				
				b->Animation(ThreadIndex, ThreadInfo.BoneTransform,fAnimationFrame,o->PriorAnimationFrame,o->PriorAction, o->Angle, o->HeadAngle);

				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(20)],p,tempPosition,true);
 				CreateEffect(ThreadIndex,  MODEL_CURSEDTEMPLE_HOLYITEM, tempPosition, o->Angle, o->Light, 0, o );
			}
		}
	}

	m_HolyItemPlayerIndex = data->btUserIndex;
#endif
}