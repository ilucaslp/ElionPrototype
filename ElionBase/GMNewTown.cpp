#include "stdafx.h"
#include "GMNewTown.h"

void GMNewTown::CreateObject(mu_uint32 objectIndex, OBJECT* pObject)
{
	switch(pObject->Type)
	{
	case 103:
		{
			CreateOperate(objectIndex, pObject);
		}
		break;
	}

	if (pObject->Type==15 || pObject->Type==25 || pObject->Type==27 || pObject->Type==45 || pObject->Type==46 || pObject->Type==47 || pObject->Type==48
		|| pObject->Type==53 || pObject->Type==98 || pObject->Type==107 || pObject->Type==115 || pObject->Type==122 || pObject->Type==130)
	{
		pObject->CollisionRange = -300;
	}
}

mu_boolean GMNewTown::MoveObject(OBJECT* pObject)
{
	PlayObjectSound(pObject);

	mu_float Luminosity;
	EVector3 Light;

	switch (pObject->Type)
	{
	case 0:
		Luminosity = (mu_float)(GetLargeRand(4) + 3) * 0.1f;
		Vector(Luminosity, Luminosity * 0.6f, Luminosity * 0.2f, Light);
		AddTerrainLight(pObject->Position[0], pObject->Position[1], Light, 3, PrimaryTerrainLight);
		pObject->HiddenMesh = -2;
		break;
	case 2:
		{
			pObject->BlendMeshTexCoordV += 0.015f;
		}
		break;
	case 53:
		{
			pObject->BlendMeshTexCoordV += 0.015f;
		}
		break;
	case 54:
		pObject->HiddenMesh = -2;
		break;
	case 55:
		{
			pObject->BlendMeshTexCoordV += 0.015f;
		}
		break;
	case 56:
		pObject->BlendMesh = 0;
		pObject->BlendMeshLight = EMath::Sin(FWorldTime*0.003f)*0.3f + 0.5f;
		pObject->Velocity = 0.05f;
		break;
	case 60:
		pObject->HiddenMesh = -2;
		break;
	case 61:
		Luminosity = (mu_float)(GetLargeRand(4) + 3) * 0.1f;
		Vector(Luminosity * 0.2f, Luminosity * 0.6f, Luminosity, Light);
		AddTerrainLight(pObject->Position[0], pObject->Position[1], Light, 3, PrimaryTerrainLight);
		pObject->HiddenMesh = -2;
		break;
	case 58:
		pObject->HiddenMesh = -2;
		break;
	case 59:
		pObject->HiddenMesh = -2;
		break;
	case 89:
		{
			pObject->BlendMeshTexCoordV += 0.005f;
		}
		break;
	case 62:
		pObject->HiddenMesh = -2;
		{
			mu_int32 iEagleIndex = 1;
			OBJECT *pBoid = &Boids[iEagleIndex];
			if (pBoid->Flags.Get(OBJECT::eOF_LIVE) == false)
			{
				pBoid->Flags.Clear();
				pBoid->Flags.Set(OBJECT::eOF_LIVE | OBJECT::eOF_LIGHTENABLE | OBJECT::eOF_ALPHAENABLE);
				pBoid->Velocity = 0.0f;
				pBoid->LifeTime = 0;
				pBoid->SubType = 1;
				Vector(0.5f, 0.5f, 0.5f, pBoid->Light);
				pBoid->Alpha = 0.0f;
				pBoid->AlphaTarget = 1.0f;
				pBoid->Gravity = 10.0f * pObject->Scale;
				pBoid->Scale = 0.5f;

				if (pObject->Position[2] > 100)
					pBoid->ShadowScale = 15.0f;
				else
					pBoid->ShadowScale = 0.0f;

				pBoid->HiddenMesh = -1;
				pBoid->BlendMesh = -1;
				pBoid->Timer = (mu_float)(GetLargeRand(314))*0.01f;
				pBoid->Type = MODEL_EAGLE;
				Vector(0.0f, 0.0f, 0.0f, pBoid->Angle);
				VectorCopy(pObject->Position, pBoid->Position);
			}
		}
		break;
	}
	return true;
}

void GMNewTown::PlayObjectSound(OBJECT* pObject)
{
	mu_float fDis_x, fDis_y;
	fDis_x = (pObject->Position[0] - Hero->Object.Position[0]) * g_TerrainScaleRev;
	fDis_y = (pObject->Position[1] - Hero->Object.Position[1]) * g_TerrainScaleRev;
	mu_float fDistance = EMath::Sqrt(fDis_x*fDis_x + fDis_y*fDis_y);

	if (fDistance >= 30.0f)
		return;

	mu_int32 Index = TERRAIN_INDEX_REPEAT((Hero->PositionX), (Hero->PositionY));
	mu_boolean bSafeZone = false;
	if ((TerrainWall[Index] & TW_SAFEZONE) == TW_SAFEZONE)
		bSafeZone = true;

	switch (pObject->Type)
	{
	case 2:
		if (!bSafeZone)
			PlayObject(SOUND_ELBELAND_WATERSMALL01, pObject, false);
		break;
	case 53:
		if (!bSafeZone)
			PlayObject(SOUND_ELBELAND_RAVINE01, pObject, false);
		break;
	case 56:
		PlayObject(SOUND_ELBELAND_ENTERATLANCE01, pObject, false);
		break;
	case 59:
		if (!bSafeZone)
			PlayObject(SOUND_ELBELAND_WATERFALLSMALL01, pObject, false);
		break;
	case 85:
		PlayObject(SOUND_ELBELAND_ENTERDEVIAS01, pObject, false);
		break;
	case 89:
		if (!bSafeZone)
			PlayObject(SOUND_ELBELAND_WATERWAY01, pObject, false);
		break;
	case 110:
		PlayObject(SOUND_ELBELAND_VILLAGEPROTECTION01, pObject, false);
		break;
	}
}

mu_boolean GMNewTown::RenderObjectVisual(const mu_uint32 ThreadIndex, OBJECT* pObject, MUModel* pModel)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	EVector3 p, Position, Light;

	switch (pObject->Type)
	{
	case 0:
		if (GetLargeRand(3) == 0)
		{
			Vector(1.0f, 1.0f, 1.0f, Light);
			CreateParticle(ThreadIndex, BITMAP_TRUE_FIRE, pObject->Position, pObject->Angle, Light, 0, pObject->Scale);
		}
		break;
	case 54:
		Vector(1.0f, 1.0f, 1.0f, Light);
		if (GetLargeRand(4) == 0)
		{
			CreateParticle(ThreadIndex, BITMAP_WATERFALL_2, pObject->Position, pObject->Angle, Light, 4, pObject->Scale);
		}
		break;
	case 58:
		CreateParticle(ThreadIndex, BITMAP_WATERFALL_5, pObject->Position, pObject->Angle, Light, 0);
		break;
	case 59:
		Vector(1.0f, 1.0f, 1.0f, Light);
		CreateParticle(ThreadIndex, BITMAP_WATERFALL_3, pObject->Position, pObject->Angle, Light, 8, pObject->Scale);
		break;
	case 60:
		if (pObject->HiddenMesh != -2)
		{
			EVector3  Light;
			Vector(0.06f, 0.07f, 0.08f, Light);
			for (mu_int32 i = 0; i < 10; ++i)
			{
				CreateParticle(ThreadIndex, BITMAP_CLOUD, pObject->Position, pObject->Angle, Light, 3, pObject->Scale, pObject);
			}
			pObject->HiddenMesh = -2;
		}
		break;
	case 61:
		if (GetLargeRand(3) == 0)
		{
			Vector(1.0f, 1.0f, 1.0f, Light);
			CreateParticle(ThreadIndex, BITMAP_TRUE_BLUE, pObject->Position, pObject->Angle, Light, 0, pObject->Scale);
		}
		break;
	case 63:
		{
			EVector3 vPos, vRelative;
			mu_float fLumi;
			fLumi = (EMath::Sin(FWorldTime*0.001f) + 1.0f) * 0.3f + 0.4f;
			Vector(fLumi*1.0f, fLumi*1.0f, fLumi*0.6f, Light);

			Vector(-40.0f, -10.0f, 0.0f, vRelative);
			pModel->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[pModel->GetBoneIndex(5)], vRelative, vPos, false);
			CreateSprite(BITMAP_LIGHT, vPos, pObject->Scale*6.0f, Light, pObject);
		}
		break;
	case 110:
		Vector(1.0f, 1.0f, 1.0f, Light);
		Vector(0.0f, 0.0f, 70.0f, p);
		if (GetLargeRand(2) == 0)
		{
			p[0] = (mu_float)EMath::Cos(FWorldTime*0.03f)*(30.0f + GetLargeRand(5));
			p[1] = (mu_float)EMath::Sin(FWorldTime*0.03f)*(30.0f + GetLargeRand(5));
			pModel->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[pModel->GetBoneIndex(0)], p, Position, false);
			CreateParticle(ThreadIndex, BITMAP_LIGHT, Position, pObject->Angle, Light, 11, 0.6f, pObject);
		}
		break;
	case 121:
		{
			EVector3 vPos, vRelative;
			mu_float fLumi, fScale;

			fLumi = (EMath::Sin(FWorldTime*0.002f) + 1.0f) * 0.5f + 0.5f;
			Vector(fLumi*1.0f, fLumi*0.5f, fLumi*0.3f, Light);
			fScale = fLumi / 1.0f;

			Vector(5.0f, -4.0f, -1.0f, vRelative);
			for (mu_int32 i = 3; i <= 8; ++i)
			{
				pModel->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[pModel->GetBoneIndex(i)], vRelative, vPos, false);
				CreateSprite(BITMAP_LIGHT, vPos, fScale, Light, pObject);
			}
		}
		break;
	case 133:	case 134:	case 135:	case 136:	case 137:	case 138:	case 139:
	case 140:	case 141:	case 142:	case 143:	case 144:	case 145:	case 146:	case 147:
		if (pObject->HiddenMesh != -2)
		{
			pObject->HiddenMesh = -2;
			MUAutoCriticalSpinLock lock(CharactersClientLock);
			mu_int32 icntIndex = TEMP_CHARACTERS_INDEX;
			for (; icntIndex < MAX_CHARACTERS_CLIENT; ++icntIndex)
			{
				if (CharactersClient[icntIndex].Object.Flags.Get(OBJECT::eOF_LIVE) == false) break;
			}

			CHARACTER * pCharacter = &CharactersClient[icntIndex];
			OBJECT * pNewObject = &CharactersClient[icntIndex].Object;

			pNewObject->Flags.Clear();
			pNewObject->Flags.Set(OBJECT::eOF_LIVE);

			CreateCharacterPointer(pCharacter, MODEL_PLAYER, 0, 0, 0);
			Vector(0.3f, 0.3f, 0.3f, pNewObject->Light);
			pCharacter->Key = icntIndex;
			CharactersKeys[icntIndex] = icntIndex;
			mu_int32 Level = 0;
			switch (pObject->Type)
			{
			case 133:
				pCharacter->Class = GAME::CLASS_KNIGHT;
				pCharacter->Skin = 0;
				Level = 9;
				pCharacter->BodyPart[BODYPART_HELM].Type = MODEL_HELM + 9;
				pCharacter->BodyPart[BODYPART_HELM].Level = Level;
				pCharacter->BodyPart[BODYPART_ARMOR].Type = MODEL_ARMOR + 9;
				pCharacter->BodyPart[BODYPART_ARMOR].Level = Level;
				pCharacter->BodyPart[BODYPART_PANTS].Type = MODEL_PANTS + 9;
				pCharacter->BodyPart[BODYPART_PANTS].Level = Level;
				pCharacter->BodyPart[BODYPART_GLOVES].Type = MODEL_GLOVES + 9;
				pCharacter->BodyPart[BODYPART_GLOVES].Level = Level;
				pCharacter->BodyPart[BODYPART_BOOTS].Type = MODEL_BOOTS + 9;
				pCharacter->BodyPart[BODYPART_BOOTS].Level = Level;
				pCharacter->Weapon[0].Type = MODEL_SWORD + 14;
				pCharacter->Weapon[0].Level = 9;
				pCharacter->Weapon[1].Type = MODEL_SHIELD + 9;
				pCharacter->Weapon[1].Level = 9;
				pCharacter->Wing.Type = -1;
				pCharacter->Helper.Type = -1;
				break;
			case 134:
				pCharacter->Class = GAME::CLASS_KNIGHT;
				pCharacter->Skin = 0;
				Level = 10;
				pCharacter->BodyPart[BODYPART_HELM].Type = MODEL_HELM + 1;
				pCharacter->BodyPart[BODYPART_HELM].Level = Level;
				pCharacter->BodyPart[BODYPART_ARMOR].Type = MODEL_ARMOR + 1;
				pCharacter->BodyPart[BODYPART_ARMOR].Level = Level;
				pCharacter->BodyPart[BODYPART_PANTS].Type = MODEL_PANTS + 1;
				pCharacter->BodyPart[BODYPART_PANTS].Level = Level;
				pCharacter->BodyPart[BODYPART_GLOVES].Type = MODEL_GLOVES + 1;
				pCharacter->BodyPart[BODYPART_GLOVES].Level = Level;
				pCharacter->BodyPart[BODYPART_BOOTS].Type = MODEL_BOOTS + 1;
				pCharacter->BodyPart[BODYPART_BOOTS].Level = Level;
				pCharacter->Weapon[0].Type = MODEL_SWORD + 16;
				pCharacter->Weapon[0].Level = 10;
				pCharacter->Weapon[1].Type = MODEL_SWORD + 16;
				pCharacter->Weapon[1].Level = 10;
				pCharacter->Wing.Type = MODEL_WING + 5;
				pCharacter->Helper.Type = -1;
				break;
			case 136:
				pCharacter->Class = GAME::CLASS_KNIGHT;
				pCharacter->Skin = 0;
				Level = 13;
				pCharacter->BodyPart[BODYPART_HELM].Type = MODEL_HELM + 29;
				pCharacter->BodyPart[BODYPART_HELM].Level = Level;
				pCharacter->BodyPart[BODYPART_ARMOR].Type = MODEL_ARMOR + 29;
				pCharacter->BodyPart[BODYPART_ARMOR].Level = Level;
				pCharacter->BodyPart[BODYPART_PANTS].Type = MODEL_PANTS + 29;
				pCharacter->BodyPart[BODYPART_PANTS].Level = Level;
				pCharacter->BodyPart[BODYPART_GLOVES].Type = MODEL_GLOVES + 29;
				pCharacter->BodyPart[BODYPART_GLOVES].Level = Level;
				pCharacter->BodyPart[BODYPART_BOOTS].Type = MODEL_BOOTS + 29;
				pCharacter->BodyPart[BODYPART_BOOTS].Level = Level;
				pCharacter->Weapon[0].Type = MODEL_SWORD + 17;
				pCharacter->Weapon[0].Level = 13;
				pCharacter->Weapon[1].Type = -1;
				pCharacter->Weapon[1].Level = 0;
				pCharacter->Wing.Type = MODEL_WING + 5;
				pCharacter->Helper.Type = MODEL_HELPER + 37;
				CreateBug(MODEL_FENRIR_BLUE, pNewObject->Position, pNewObject);
				break;
			case 138:
				pCharacter->Class = GAME::CLASS_WIZARD;
				pCharacter->Skin = 0;
				Level = 10;
				pCharacter->BodyPart[BODYPART_HELM].Type = MODEL_HELM + 3;
				pCharacter->BodyPart[BODYPART_HELM].Level = Level;
				pCharacter->BodyPart[BODYPART_ARMOR].Type = MODEL_ARMOR + 3;
				pCharacter->BodyPart[BODYPART_ARMOR].Level = Level;
				pCharacter->BodyPart[BODYPART_PANTS].Type = MODEL_PANTS + 3;
				pCharacter->BodyPart[BODYPART_PANTS].Level = Level;
				pCharacter->BodyPart[BODYPART_GLOVES].Type = MODEL_GLOVES + 3;
				pCharacter->BodyPart[BODYPART_GLOVES].Level = Level;
				pCharacter->BodyPart[BODYPART_BOOTS].Type = MODEL_BOOTS + 3;
				pCharacter->BodyPart[BODYPART_BOOTS].Level = Level;
				pCharacter->Weapon[0].Type = MODEL_STAFF + 5;
				pCharacter->Weapon[0].Level = 10;
				pCharacter->Weapon[1].Type = MODEL_SHIELD + 14;
				pCharacter->Weapon[1].Level = 10;
				pCharacter->Wing.Type = MODEL_WING + 4;
				pCharacter->Helper.Type = -1;
				break;
			case 139:
				pCharacter->Class = GAME::CLASS_WIZARD;
				pCharacter->Skin = 0;
				Level = 13;
				pCharacter->BodyPart[BODYPART_HELM].Type = MODEL_HELM + 30;
				pCharacter->BodyPart[BODYPART_HELM].Level = Level;
				pCharacter->BodyPart[BODYPART_ARMOR].Type = MODEL_ARMOR + 30;
				pCharacter->BodyPart[BODYPART_ARMOR].Level = Level;
				pCharacter->BodyPart[BODYPART_PANTS].Type = MODEL_PANTS + 30;
				pCharacter->BodyPart[BODYPART_PANTS].Level = Level;
				pCharacter->BodyPart[BODYPART_GLOVES].Type = MODEL_GLOVES + 30;
				pCharacter->BodyPart[BODYPART_GLOVES].Level = Level;
				pCharacter->BodyPart[BODYPART_BOOTS].Type = MODEL_BOOTS + 30;
				pCharacter->BodyPart[BODYPART_BOOTS].Level = Level;
				pCharacter->Weapon[0].Type = MODEL_STAFF + 13;
				pCharacter->Weapon[0].Level = 13;
				pCharacter->Weapon[1].Type = -1;
				pCharacter->Weapon[1].Level = 0;
				pCharacter->Wing.Type = MODEL_WING + 37;
				pCharacter->Helper.Type = -1;
				break;
			case 142:
				pCharacter->Class = GAME::CLASS_ELF;
				pCharacter->Skin = 0;
				Level = 13;
				pCharacter->BodyPart[BODYPART_HELM].Type = MODEL_HELM + 31;
				pCharacter->BodyPart[BODYPART_HELM].Level = Level;
				pCharacter->BodyPart[BODYPART_ARMOR].Type = MODEL_ARMOR + 31;
				pCharacter->BodyPart[BODYPART_ARMOR].Level = Level;
				pCharacter->BodyPart[BODYPART_PANTS].Type = MODEL_PANTS + 31;
				pCharacter->BodyPart[BODYPART_PANTS].Level = Level;
				pCharacter->BodyPart[BODYPART_GLOVES].Type = MODEL_GLOVES + 31;
				pCharacter->BodyPart[BODYPART_GLOVES].Level = Level;
				pCharacter->BodyPart[BODYPART_BOOTS].Type = MODEL_BOOTS + 31;
				pCharacter->BodyPart[BODYPART_BOOTS].Level = Level;
				pCharacter->Weapon[0].Type = -1;
				pCharacter->Weapon[0].Level = 0;
				pCharacter->Weapon[1].Type = MODEL_BOW + 22;
				pCharacter->Weapon[1].Level = 13;
				pCharacter->Wing.Type = MODEL_WING + 38;
				pCharacter->Helper.Type = -1;
				break;
			case 144:
				pCharacter->Class = GAME::CLASS_DARK;
				pCharacter->Skin = 0;
				Level = 13;
				pCharacter->BodyPart[BODYPART_HELM].Type = MODEL_HELM + 32;
				pCharacter->BodyPart[BODYPART_HELM].Level = Level;
				pCharacter->BodyPart[BODYPART_ARMOR].Type = MODEL_ARMOR + 32;
				pCharacter->BodyPart[BODYPART_ARMOR].Level = Level;
				pCharacter->BodyPart[BODYPART_PANTS].Type = MODEL_PANTS + 32;
				pCharacter->BodyPart[BODYPART_PANTS].Level = Level;
				pCharacter->BodyPart[BODYPART_GLOVES].Type = MODEL_GLOVES + 32;
				pCharacter->BodyPart[BODYPART_GLOVES].Level = Level;
				pCharacter->BodyPart[BODYPART_BOOTS].Type = MODEL_BOOTS + 32;
				pCharacter->BodyPart[BODYPART_BOOTS].Level = Level;
				pCharacter->Weapon[0].Type = MODEL_SWORD + 23;
				pCharacter->Weapon[0].Level = 13;
				pCharacter->Weapon[1].Type = -1;
				pCharacter->Weapon[1].Level = 0;
				pCharacter->Wing.Type = MODEL_WING + 39;
				pCharacter->Helper.Type = -1;
				break;
			case 146:
				pCharacter->Class = GAME::CLASS_DARK_LORD;
				pCharacter->Skin = 0;
				Level = 13;
				pCharacter->BodyPart[BODYPART_HELM].Type = MODEL_HELM + 28;
				pCharacter->BodyPart[BODYPART_HELM].Level = Level;
				pCharacter->BodyPart[BODYPART_ARMOR].Type = MODEL_ARMOR + 28;
				pCharacter->BodyPart[BODYPART_ARMOR].Level = Level;
				pCharacter->BodyPart[BODYPART_PANTS].Type = MODEL_PANTS + 28;
				pCharacter->BodyPart[BODYPART_PANTS].Level = Level;
				pCharacter->BodyPart[BODYPART_GLOVES].Type = MODEL_GLOVES + 28;
				pCharacter->BodyPart[BODYPART_GLOVES].Level = Level;
				pCharacter->BodyPart[BODYPART_BOOTS].Type = MODEL_BOOTS + 28;
				pCharacter->BodyPart[BODYPART_BOOTS].Level = Level;
				pCharacter->Weapon[0].Type = MODEL_MACE + 15;
				pCharacter->Weapon[0].Level = 10;
				pCharacter->Weapon[1].Type = -1;
				pCharacter->Weapon[1].Level = 0;
				pCharacter->Wing.Type = MODEL_HELPER + 30;
				pCharacter->Helper.Type = MODEL_HELPER + 4;
				CreateBug(MODEL_DARK_HORSE, pNewObject->Position, pNewObject);
				giPetManager::CreatePetDarkSpirit_Now(pCharacter);
				break;
			case 147:
				pCharacter->Class = GAME::CLASS_DARK_LORD;
				pCharacter->Skin = 0;
				Level = 13;
				pCharacter->BodyPart[BODYPART_HELM].Type = MODEL_HELM + 33;
				pCharacter->BodyPart[BODYPART_HELM].Level = Level;
				pCharacter->BodyPart[BODYPART_ARMOR].Type = MODEL_ARMOR + 33;
				pCharacter->BodyPart[BODYPART_ARMOR].Level = Level;
				pCharacter->BodyPart[BODYPART_PANTS].Type = MODEL_PANTS + 33;
				pCharacter->BodyPart[BODYPART_PANTS].Level = Level;
				pCharacter->BodyPart[BODYPART_GLOVES].Type = MODEL_GLOVES + 33;
				pCharacter->BodyPart[BODYPART_GLOVES].Level = Level;
				pCharacter->BodyPart[BODYPART_BOOTS].Type = MODEL_BOOTS + 33;
				pCharacter->BodyPart[BODYPART_BOOTS].Level = Level;
				pCharacter->Weapon[0].Type = MODEL_MACE + 14;
				pCharacter->Weapon[0].Level = 13;
				pCharacter->Weapon[1].Type = -1;
				pCharacter->Weapon[1].Level = 0;
				pCharacter->Wing.Type = MODEL_WING + 40;
				pCharacter->Helper.Type = -1;
				break;
			}
			VectorCopy(pObject->Position, pNewObject->Position);
			VectorCopy(pObject->Angle, pNewObject->Angle);
			SetCharacterScale(pCharacter);

			SetPlayerAttack(ThreadIndex, pCharacter);
			pNewObject->PriorAnimationFrame = pNewObject->AnimationFrame = GetLargeRand(MODELS::Data[pNewObject->Type]._AnimationHeader[pNewObject->CurrentAction].FrameCount);
			pObject->Owner = pNewObject;
			pObject->SubType = icntIndex;
		}
		break;
	case 149:		case 150:		case 151:		case 152:		case 153:		case 154:		case 155:
		if (pObject->HiddenMesh != -2)
		{
			pObject->HiddenMesh = -2;
			MUAutoCriticalSpinLock lock(CharactersClientLock);
			mu_int32 icntIndex = TEMP_CHARACTERS_INDEX;
			for (mu_int32 i = TEMP_CHARACTERS_INDEX; i < MAX_CHARACTERS_CLIENT; ++i)
			{
				icntIndex = i;
				if (CharactersClient[i].Object.Flags.Get(OBJECT::eOF_LIVE) == false)
				{
					break;
				}
			}

			OBJECT * pNewObject = &CharactersClient[icntIndex].Object;

			switch (pObject->Type)
			{
			case 149: CreateMonster(0, 419, 0, 0, icntIndex); break;
			case 150: CreateMonster(0, 420, 0, 0, icntIndex); break;
			case 151: CreateMonster(0, 421, 0, 0, icntIndex); break;
			case 153: CreateMonster(0, 423, 0, 0, icntIndex); break;
			case 155: CreateMonster(0, 425, 0, 0, icntIndex); break;
			}

			VectorCopy(pObject->Position, pNewObject->Position);
			VectorCopy(pObject->Angle, pNewObject->Angle);
			SetAction(pNewObject, MONSTER01_ATTACK1);
			if (MODELS::Data[pNewObject->Type]._AnimationHeader[MONSTER01_ATTACK1].FrameCount > 0)
			{
				pNewObject->PriorAnimationFrame = pNewObject->AnimationFrame =
					GetLargeRand(MODELS::Data[pNewObject->Type]._AnimationHeader[MONSTER01_ATTACK1].FrameCount);
			}
			pObject->Owner = pNewObject;
		}
		break;
	}

	if (pObject->Type >= 133 && pObject->Type <= 147)
	{
		if (pObject->Owner != nullptr)
		{
			CHARACTER * pCharacter = &CharactersClient[pObject->SubType];
			if (pObject->Owner->CurrentAction < PLAYER_WALK_MALE)
			{
				if (pObject->Type == 133)
				{
					if (GetLargeRand(2) == 0)
					{
						pCharacter->Helper.Type = -1;
						SetPlayerAttack(ThreadIndex, pCharacter);
					}
					else
					{
						pCharacter->Helper.Type = -1;
						SetAction(pObject->Owner, PLAYER_ATTACK_SKILL_WHEEL);
						pCharacter->Skill = AT_SKILL_WHEEL;
						pCharacter->AttackTime = 1;
						pCharacter->TargetCharacter = -1;
						pCharacter->AttackFlag = ATTACK_FAIL;
						pCharacter->SkillX = pCharacter->PositionX;
						pCharacter->SkillY = pCharacter->PositionY;
					}
				}
				else if (pObject->Type == 134)
				{
					if (GetLargeRand(2) == 0)
					{
						SetAction(pObject->Owner, PLAYER_ATTACK_SKILL_WHEEL);
						pCharacter->Skill = AT_SKILL_WHEEL;
						pCharacter->AttackTime = 1;
						pCharacter->TargetCharacter = -1;
						pCharacter->AttackFlag = ATTACK_FAIL;
						pCharacter->SkillX = pCharacter->PositionX;
						pCharacter->SkillY = pCharacter->PositionY;
					}
					else
					{
						SetAction(pObject->Owner, PLAYER_ATTACK_ONETOONE);
						pCharacter->Skill = AT_SKILL_ONETOONE;
						pCharacter->AttackTime = 1;
						pCharacter->TargetCharacter = -1;
						pCharacter->AttackFlag = ATTACK_FAIL;
						pCharacter->SkillX = pCharacter->PositionX;
						pCharacter->SkillY = pCharacter->PositionY;
					}
				}
				else if (pObject->Type == 138)
				{
					if (GetLargeRand(2) == 0)
					{
						SetPlayerAttack(ThreadIndex, pCharacter);
					}
					else
					{
						SetPlayerMagic(ThreadIndex, pCharacter);
						pCharacter->Skill = AT_SKILL_INFERNO;
						pCharacter->AttackTime = 1;
						pCharacter->TargetCharacter = -1;
						pCharacter->AttackFlag = ATTACK_FAIL;
						pCharacter->SkillX = pCharacter->PositionX;
						pCharacter->SkillY = pCharacter->PositionY;
					}
				}
				else if (pObject->Type == 139)
				{
					SetPlayerStop(ThreadIndex, pCharacter);
				}
				else if (pObject->Type == 142)
				{
					SetPlayerStop(ThreadIndex, pCharacter);
				}
				else if (pObject->Type == 144)
				{
					SetPlayerStop(ThreadIndex, pCharacter);
				}
				else if (pObject->Type == 147)
				{
					SetPlayerStop(ThreadIndex, pCharacter);
				}
			}
		}
	}
	else if (pObject->Type >= 149 && pObject->Type <= 155)
	{
		if (pObject->Owner != nullptr)
		{
			if (pObject->Owner->CurrentAction < MONSTER01_WALK)
			{
				if (pObject->Type == 153)
				{
					mu_int32 iRand = GetLargeRand(3);
					if (iRand == 0) SetAction(pObject->Owner, MONSTER01_ATTACK1);
					else if (iRand == 1)  SetAction(pObject->Owner, MONSTER01_ATTACK2);
					else SetAction(pObject->Owner, MONSTER01_SHOCK);
				}
				else if ((pObject->Type >= 152 && pObject->Type <= 155))
				{
					if (GetLargeRand(2) == 0) SetAction(pObject->Owner, MONSTER01_ATTACK1);
					else SetAction(pObject->Owner, MONSTER01_SHOCK);
				}
				else if ((pObject->Type >= 149 && pObject->Type <= 151))
				{
					SetAction(pObject->Owner, MONSTER01_STOP1);
				}
			}
		}
	}

	return true;
}

mu_boolean GMNewTown::RenderObject(const mu_uint32 ThreadIndex, OBJECT* pObject, MUModel* pModel, mu_boolean ExtraMon)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	mu_boolean Success = true;
	mu_boolean LightEnable = pModel->LightEnable(ThreadIndex);
	EVector3 BodyLight = pModel->BodyLight(ThreadIndex);

	if ((pObject->Type >= 5 && pObject->Type <= 14) || pObject->Type == 4 || pObject->Type == 129)
	{
		if (LightEnable == true)
		{
			pModel->LightEnable(ThreadIndex) = false;
			pModel->BodyLight(ThreadIndex) *= 2.0f;
		}
		Vector(1.0f, 1.0f, 1.0f, pObject->Light);
	}

	if (pObject->Type == 2 || pObject->Type == 53 || pObject->Type == 55 || pObject->Type == 89 || pObject->Type == 125 || pObject->Type == 128)
	{
		pModel->RenderBody(ThreadIndex, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, pObject->HiddenMesh);
	}
	else if (pObject->Type == 110)
	{
		pModel->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE | RENDER_BRIGHT | RENDER_CHROME2, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight);
		pModel->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight);
		pModel->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight);
	}
	else if (pObject->Type == 78)
	{
		pModel->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV);
		pModel->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV);
		
		if(MU_UPDATECOUNTER > 0)
		{
			EVector3 p, Position, Light;
			for (mu_int32 i = 1; i <= 4; ++i)
			{
				Vector(0.0f, 0.0f, 0.0f, p);
				pModel->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[pModel->GetBoneIndex(i)], p, Position, false);
				Vector(0.1f, 0.1f, 0.3f, Light);
				CreateSprite(BITMAP_SPARK + 1, Position, 5.5f, Light, pObject);
			}
		}
	}
	else if (pObject->Type == 79)
	{
		pModel->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV);
		pModel->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV);
		
		if(MU_UPDATECOUNTER > 0)
		{
			EVector3 p, Position, Light;
			for (mu_int32 i = 1; i <= 6; ++i)
			{
				Vector(0.0f, 0.0f, 0.0f, p);
				pModel->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[pModel->GetBoneIndex(i)], p, Position, false);
				Vector(0.1f, 0.1f, 0.3f, Light);
				CreateSprite(BITMAP_SPARK + 1, Position, 5.5f, Light, pObject);
			}
		}
	}
	else if (pObject->Type == 73)
	{
		pModel->RenderBody(ThreadIndex, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, pObject->HiddenMesh);
		pModel->RenderBody(ThreadIndex, RENDER_BRIGHT | RENDER_CHROME, 0.5f, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, pObject->HiddenMesh);
	}
	else if (pObject->Type == 104)
	{
		pModel->RenderBody(ThreadIndex, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, pObject->HiddenMesh);
		mu_float fLumi = (EMath::Sin(FWorldTime*0.002f) + 1.0f) * 0.5f;
		pModel->RenderMesh(ThreadIndex, 4, RENDER_BRIGHT, pObject->Alpha, 4, fLumi);
	}
	else if (pObject->Type == 113)
	{
		pModel->RenderBody(ThreadIndex, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, pObject->HiddenMesh);
		pModel->RenderMesh(ThreadIndex, 9, RENDER_BRIGHT | RENDER_CHROME, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight);
	}
	else if (pObject->Type == 114)
	{
		pModel->RenderBody(ThreadIndex, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, pObject->HiddenMesh);
		pModel->RenderMesh(ThreadIndex, 9, RENDER_BRIGHT | RENDER_CHROME, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight);
	}
	else if (pObject->Type == 121)
	{
		pModel->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight);
		pModel->RenderMesh(ThreadIndex, 3, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight);
		pModel->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight);
		mu_float fLumi = (EMath::Sin(FWorldTime*0.002f) + 1.0f) * 0.5f;
		pModel->RenderMesh(ThreadIndex, 1, RENDER_BRIGHT, pObject->Alpha, 1, fLumi);
		fLumi = (EMath::Sin(FWorldTime*0.0015f) + 1.0f) * 0.1f + 0.1f;
		pModel->RenderMesh(ThreadIndex, 0, RENDER_BRIGHT | RENDER_CHROME, fLumi, 0, fLumi);
	}
	else if (pObject->Type == MODEL_ELBELAND_MARCE)
	{
		pModel->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight);
		pModel->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight);
		pModel->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE | RENDER_BRIGHT, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight);
	}
	else if (pObject->Type == MODEL_MONSTER01 + 130)
	{
		pModel->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight);
		pModel->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight);
	}
	else if (pObject->Type == MODEL_MONSTER01 + 133 && pObject->CurrentAction == MONSTER01_DIE)
	{

	}
	else
	{
		Success = false;
	}

	if ((pObject->Type >= 5 && pObject->Type <= 14) || pObject->Type == 4 || pObject->Type == 129)
	{
		pModel->LightEnable(ThreadIndex) = LightEnable;
		pModel->BodyLight(ThreadIndex) = BodyLight;
	}

	return Success;
}

CHARACTER* GMNewTown::CreateNewTownMonster(mu_int32 iType, mu_int32 PosX, mu_int32 PosY, mu_int32 Key)
{
	CHARACTER* pCharacter = nullptr;

	switch (iType)
	{
	case 415:
		pCharacter = CreateCharacter(Key, MODEL_ELBELAND_SILVIA, PosX, PosY);
		pCharacter->Object.Scale = 1.0f;
		pCharacter->Object.m_fEdgeScale = 1.2f;
		break;
	case 416:
		pCharacter = CreateCharacter(Key, MODEL_ELBELAND_RHEA, PosX, PosY);
		pCharacter->Object.Scale = 1.0f;
		pCharacter->Object.m_fEdgeScale = 1.1f;
		break;
	case 417:
		pCharacter = CreateCharacter(Key, MODEL_ELBELAND_MARCE, PosX, PosY);
		pCharacter->Object.Scale = 1.05f;
		pCharacter->Object.m_fEdgeScale = 1.2f;
		break;
	case 418:
		pCharacter = CreateCharacter(Key, MODEL_MONSTER01 + 128, PosX, PosY);
		pCharacter->Object.Scale = 1.0f * 0.95f;
		pCharacter->Weapon[0].Type = -1;
		pCharacter->Weapon[1].Type = -1;
		break;
	case 419:
		pCharacter = CreateCharacter(Key, MODEL_MONSTER01 + 129, PosX, PosY);
		pCharacter->Object.Scale = 0.8f * 0.95f;
		pCharacter->Weapon[0].Type = -1;
		pCharacter->Weapon[1].Type = -1;
		break;
	case 420:
		pCharacter = CreateCharacter(Key, MODEL_MONSTER01 + 130, PosX, PosY);
		pCharacter->Object.Scale = 1.0f * 0.95f;
		pCharacter->Weapon[0].Type = -1;
		pCharacter->Weapon[1].Type = -1;
		break;
	case 421:
		pCharacter = CreateCharacter(Key, MODEL_MONSTER01 + 131, PosX, PosY);
		pCharacter->Object.Scale = 0.8f * 1.1f;
		pCharacter->Weapon[0].Type = -1;
		pCharacter->Weapon[1].Type = -1;
		break;
	case 422:
		pCharacter = CreateCharacter(Key, MODEL_MONSTER01 + 132, PosX, PosY);
		pCharacter->Object.Scale = 1.0f * 0.95f;
		pCharacter->Weapon[0].Type = -1;
		pCharacter->Weapon[1].Type = -1;
		break;
	case 423:
		pCharacter = CreateCharacter(Key, MODEL_MONSTER01 + 133, PosX, PosY);
		pCharacter->Object.Scale = 0.17f * 0.95f;
		pCharacter->Object.ShadowScale = 0.01f;
		pCharacter->Object.m_fEdgeScale = 1.05f;
		pCharacter->Object.LifeTime = 100;
		pCharacter->Weapon[0].Type = -1;
		pCharacter->Weapon[1].Type = -1;
		break;
	case 424:
		pCharacter = CreateCharacter(Key, MODEL_MONSTER01 + 134, PosX, PosY);
		pCharacter->Object.Scale = 1.2f * 0.95f;
		pCharacter->Weapon[0].Type = -1;
		pCharacter->Weapon[1].Type = -1;
		break;
	case 425:
		pCharacter = CreateCharacter(Key, MODEL_MONSTER01 + 135, PosX, PosY);
		pCharacter->Object.Scale = 1.3f * 0.95f;
		pCharacter->Weapon[0].Type = -1;
		pCharacter->Weapon[1].Type = -1;
		break;
	}

	return pCharacter;
}

void GMNewTown::MoveBlurEffect(const mu_uint32 ThreadIndex, CHARACTER* pCharacter, OBJECT* pObject, MUModel* pModel)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	switch (pObject->Type)
	{
	case MODEL_MONSTER01 + 130:
		{
			mu_float Start_Frame = 0.0f;
			mu_float End_Frame = 6.0f;
			if ((pObject->AnimationFrame >= Start_Frame && pObject->AnimationFrame <= End_Frame && pObject->CurrentAction == MONSTER01_ATTACK1)
				|| (pObject->AnimationFrame >= Start_Frame && pObject->AnimationFrame <= End_Frame && pObject->CurrentAction == MONSTER01_ATTACK2))
			{
				EVector3  Light;
				Vector(1.0f, 1.2f, 2.0f, Light);

				EVector3 StartPos, StartRelative;
				EVector3 EndPos, EndRelative;

				mu_float fActionSpeed = pModel->_AnimationHeader[pObject->CurrentAction].Speed;
				mu_float fSpeedPerFrame = fActionSpeed / 10.0;
				mu_float fAnimationFrame = pObject->AnimationFrame - fActionSpeed;
				for (mu_int32 i = 0; i < 10; ++i)
				{
					pModel->Animation(ThreadIndex, ThreadInfo.BoneTransform, fAnimationFrame, pObject->PriorAnimationFrame, pObject->PriorAction, pObject->Angle, pObject->HeadAngle);

					Vector(0.0f, 0.0f, 0.0f, StartRelative);
					Vector(0.0f, 0.0f, 0.0f, EndRelative);

					pModel->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[pModel->GetBoneIndex(60)], StartRelative, StartPos, false);
					pModel->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[pModel->GetBoneIndex(61)], EndRelative, EndPos, false);
					CreateBlur(pCharacter, StartPos, EndPos, Light, 0);

					fAnimationFrame += fSpeedPerFrame;
				}
			}
		}
		break;
	}
}

mu_boolean GMNewTown::RenderMonsterVisual(const mu_uint32 ThreadIndex, CHARACTER* pCharacter, OBJECT* pObject, MUModel* pModel)
{
	EVector3 vPos, vRelative, Light;
	mu_float fLumi, fScale;

	switch (pObject->Type)
	{
	case MODEL_ELBELAND_RHEA:
		if (pObject->CurrentAction == 0 && GetLargeRand(5) == 0)
		{
			Vector((GetLargeRand(90) + 10)*0.01f, (GetLargeRand(90) + 10)*0.01f, (GetLargeRand(90) + 10)*0.01f, Light);
			fScale = (GetLargeRand(5) + 5)*0.1f;
			Vector(0.0f, 0.0f, 0.0f, vRelative);
			vRelative[0] = 20 + (GetLargeRand(1000) - 500)*0.1f;
			vRelative[1] = (GetLargeRand(300) - 150)*0.1f;
			pModel->TransformPosition(ThreadIndex, pObject->BoneTransform[pModel->GetBoneIndex(35)], vRelative, vPos, true);
			CreateParticle(ThreadIndex, BITMAP_LIGHT, vPos, pObject->Angle, Light, 13, fScale, pObject);
			CreateParticle(ThreadIndex, BITMAP_LIGHT, vPos, pObject->Angle, Light, 12, fScale, pObject);
		}
		return true;
	case MODEL_ELBELAND_MARCE:
		{
			Vector(10.0f, 0.0f, 0.0f, vRelative);
			pModel->TransformPosition(ThreadIndex, pObject->BoneTransform[pModel->GetBoneIndex(81)], vRelative, vPos, true);
			fScale = 1.5f;

			fLumi = (EMath::Sin(FWorldTime*0.001f) + 1.0f) * 0.3f + 0.4f;
			Vector(fLumi*0.5f, fLumi*0.5f, fLumi*0.5f, Light);
			CreateSprite(BITMAP_FLARE_BLUE, vPos, fScale, Light, pObject);

			Vector(0.5f, 0.5f, 0.5f, Light);
			CreateSprite(BITMAP_FLARE_BLUE, vPos, fScale*0.8f, Light, pObject, -FWorldTime*0.1f);
		}
		return true;
	case MODEL_MONSTER01 + 132:
		for (mu_int32 i = 0; i < 4; ++i)
		{
			Vector(0, (GetLargeRand(300) - 150)*0.1f, (GetLargeRand(200) - 100)*0.1f, vRelative);
			pModel->TransformPosition(ThreadIndex, pObject->BoneTransform[pModel->GetBoneIndex(30)], vRelative, vPos, true);
			CreateParticle(ThreadIndex, BITMAP_FIRE_CURSEDLICH, vPos, pObject->Angle, pObject->Light, 0, 1, pObject);
		}
		return true;
	case MODEL_MONSTER01 + 133:
		if (pObject->CurrentAction == MONSTER01_WALK)
		{
			EVector3 Position;
			Vector(pObject->Position[0] + GetLargeRand(200) - 100, pObject->Position[1] + GetLargeRand(200) - 100, pObject->Position[2], Position);
			CreateParticle(ThreadIndex, BITMAP_SMOKE + 1, Position, pObject->Angle, pObject->Light);
		}
		if (pObject->CurrentAction == MONSTER01_DIE)
		{
			if (pObject->LifeTime == 100)
			{
				pObject->LifeTime = 90;

				EVector3 vRelativePos, vWorldPos, Light;
				Vector(1.0f, 1.0f, 1.0f, Light);

				Vector(0.0f, 0.0f, 0.0f, vRelativePos);

				pModel->TransformPosition(ThreadIndex, pObject->BoneTransform[pModel->GetBoneIndex(7)], vRelativePos, vWorldPos, true);
				CreateEffect(ThreadIndex, MODEL_TOTEMGOLEM_PART1, vWorldPos, pObject->Angle, Light, 0, pObject, 0, 0);

				pModel->TransformPosition(ThreadIndex, pObject->BoneTransform[pModel->GetBoneIndex(5)], vRelativePos, vWorldPos, true);
				CreateEffect(ThreadIndex, MODEL_TOTEMGOLEM_PART2, vWorldPos, pObject->Angle, Light, 0, pObject, 0, 0);

				pModel->TransformPosition(ThreadIndex, pObject->BoneTransform[pModel->GetBoneIndex(29)], vRelativePos, vWorldPos, true);
				CreateEffect(ThreadIndex, MODEL_TOTEMGOLEM_PART3, vWorldPos, pObject->Angle, Light, 0, pObject, 0, 0);

				pModel->TransformPosition(ThreadIndex, pObject->BoneTransform[pModel->GetBoneIndex(64)], vRelativePos, vWorldPos, true);
				CreateEffect(ThreadIndex, MODEL_TOTEMGOLEM_PART4, vWorldPos, pObject->Angle, Light, 0, pObject, 0, 0);

				pModel->TransformPosition(ThreadIndex, pObject->BoneTransform[pModel->GetBoneIndex(93)], vRelativePos, vWorldPos, true);
				CreateEffect(ThreadIndex, MODEL_TOTEMGOLEM_PART5, vWorldPos, pObject->Angle, Light, 0, pObject, 0, 0);

				pModel->TransformPosition(ThreadIndex, pObject->BoneTransform[pModel->GetBoneIndex(98)], vRelativePos, vWorldPos, true);
				CreateEffect(ThreadIndex, MODEL_TOTEMGOLEM_PART6, vWorldPos, pObject->Angle, Light, 0, pObject, 0, 0);

				pModel->TransformPosition(ThreadIndex, pObject->BoneTransform[pModel->GetBoneIndex(5)], vRelativePos, vWorldPos, true);
				VectorCopy(vWorldPos, vRelativePos);

				for (mu_int32 i = 0; i < 6; ++i)
				{
					vWorldPos[0] = vRelativePos[0] + GetLargeRand(160) - 80;
					vWorldPos[1] = vRelativePos[1] + GetLargeRand(160) - 80;
					vWorldPos[2] = vRelativePos[2];
					CreateParticle(ThreadIndex, BITMAP_LEAF_TOTEMGOLEM, vWorldPos, pObject->Angle, Light, 0, 1.0f);
				}

				Vector(0.5f, 0.5f, 0.5f, Light);

				for (mu_int32 i = 0; i < 20; ++i)
				{
					vWorldPos[0] = vRelativePos[0] + GetLargeRand(160) - 80;
					vWorldPos[1] = vRelativePos[1] + GetLargeRand(160) - 80;
					vWorldPos[2] = vRelativePos[2] + (GetLargeRand(150)) - 50;
					CreateParticle(ThreadIndex, BITMAP_SMOKE, vWorldPos, pObject->Angle, Light, 48, 1.0f);
				}
			}
		}
		return true;
	}

	return false;
}

mu_boolean GMNewTown::PlayMonsterSound(OBJECT* pObject)
{
	mu_float fDis_x, fDis_y;
	fDis_x = pObject->Position[0] - Hero->Object.Position[0];
	fDis_y = pObject->Position[1] - Hero->Object.Position[1];
	mu_float fDistance = EMath::Sqrt(fDis_x*fDis_x + fDis_y*fDis_y);

	if (fDistance > 500.0f) return true;

	switch (pObject->Type)
	{
	case MODEL_MONSTER01 + 128:
		if (pObject->CurrentAction == MONSTER01_ATTACK1 || pObject->CurrentAction == MONSTER01_ATTACK2)
		{
			PlayBackground(SOUND_ELBELAND_RABBITSTRANGE_ATTACK01);
		}
		else if (pObject->CurrentAction == MONSTER01_DIE)
		{
			PlayBackground(SOUND_ELBELAND_RABBITSTRANGE_DEATH01);
		}
		return true;
	case MODEL_MONSTER01 + 129:
		if (pObject->CurrentAction == MONSTER01_WALK)
		{
			if (GetLargeRand(100) == 0)
			{
				PlayBackground(SOUND_ELBELAND_RABBITUGLY_BREATH01);
			}
		}
		else if (pObject->CurrentAction == MONSTER01_ATTACK1 || pObject->CurrentAction == MONSTER01_ATTACK2)
		{
			PlayBackground(SOUND_ELBELAND_RABBITUGLY_ATTACK01);
		}
		else if (pObject->CurrentAction == MONSTER01_DIE)
		{
			PlayBackground(SOUND_ELBELAND_RABBITUGLY_DEATH01);
		}
		return true;
	case MODEL_MONSTER01 + 130:
		if (pObject->CurrentAction == MONSTER01_STOP1 || pObject->CurrentAction == MONSTER01_STOP2
			|| pObject->CurrentAction == MONSTER01_WALK)
		{
			if (GetLargeRand(30) == 0)
			{
				PlayBackground(SOUND_ELBELAND_WOLFHUMAN_MOVE02);
			}
		}
		else if (pObject->CurrentAction == MONSTER01_ATTACK1 || pObject->CurrentAction == MONSTER01_ATTACK2)
		{
			PlayBackground(SOUND_ELBELAND_WOLFHUMAN_ATTACK01);
		}
		else if (pObject->CurrentAction == MONSTER01_DIE)
		{
			PlayBackground(SOUND_ELBELAND_WOLFHUMAN_DEATH01);
		}
		return true;
	case MODEL_MONSTER01 + 131:
		if (pObject->CurrentAction == MONSTER01_WALK)
		{
			if (GetLargeRand(100) == 0)
			{
				PlayBackground(SOUND_ELBELAND_BUTTERFLYPOLLUTION_MOVE01);
			}
		}
		else if (pObject->CurrentAction == MONSTER01_DIE)
		{
			PlayBackground(SOUND_ELBELAND_BUTTERFLYPOLLUTION_DEATH01);
		}
		return true;
	case MODEL_MONSTER01 + 132:
		if (pObject->CurrentAction == MONSTER01_WALK)
		{
			if (GetLargeRand(100) == 0)
			{
				PlayBackground(SOUND_ELBELAND_CURSERICH_MOVE01);
			}
		}
		else if (pObject->CurrentAction == MONSTER01_ATTACK1 || pObject->CurrentAction == MONSTER01_ATTACK2)
		{
			PlayBackground(SOUND_ELBELAND_CURSERICH_ATTACK01);
		}
		else if (pObject->CurrentAction == MONSTER01_DIE)
		{
			PlayBackground(SOUND_ELBELAND_CURSERICH_DEATH01);
		}
		return true;
	case MODEL_MONSTER01 + 133:
		if (pObject->CurrentAction == MONSTER01_WALK)
		{
			if (GetLargeRand(2) == 0)
				PlayBackground(SOUND_ELBELAND_TOTEMGOLEM_MOVE01);
			else
				PlayBackground(SOUND_ELBELAND_TOTEMGOLEM_MOVE02);
		}
		else if (pObject->CurrentAction == MONSTER01_ATTACK1)
		{
			PlayBackground(SOUND_ELBELAND_TOTEMGOLEM_ATTACK01);
		}
		else if (pObject->CurrentAction == MONSTER01_ATTACK2)
		{
			PlayBackground(SOUND_ELBELAND_TOTEMGOLEM_ATTACK02);
		}
		else if (pObject->CurrentAction == MONSTER01_DIE)
		{
			PlayBackground(SOUND_ELBELAND_TOTEMGOLEM_DEATH01);
		}
		return true;
	case MODEL_MONSTER01 + 134:
		if (pObject->CurrentAction == MONSTER01_WALK)
		{
			if (GetLargeRand(100) == 0)
			{
				PlayBackground(SOUND_ELBELAND_BEASTWOO_MOVE01);
			}
		}
		else if (pObject->CurrentAction == MONSTER01_ATTACK1 || pObject->CurrentAction == MONSTER01_ATTACK2)
		{
			PlayBackground(SOUND_ELBELAND_BEASTWOO_ATTACK01);
		}
		else if (pObject->CurrentAction == MONSTER01_DIE)
		{
			PlayBackground(SOUND_ELBELAND_BEASTWOO_DEATH01);
		}
		return true;
	case MODEL_MONSTER01 + 135:
		if (pObject->CurrentAction == MONSTER01_WALK)
		{
			if (GetLargeRand(100) == 0)
			{
				PlayBackground(SOUND_ELBELAND_BEASTWOOLEADER_MOVE01);
			}
		}
		else if (pObject->CurrentAction == MONSTER01_ATTACK1 || pObject->CurrentAction == MONSTER01_ATTACK2)
		{
			PlayBackground(SOUND_ELBELAND_BEASTWOOLEADER_ATTACK01);
		}
		else if (pObject->CurrentAction == MONSTER01_DIE)
		{
			PlayBackground(SOUND_ELBELAND_BEASTWOO_DEATH01);
		}
		return true;
	}

	return false;
}