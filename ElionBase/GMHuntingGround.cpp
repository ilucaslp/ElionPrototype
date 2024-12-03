#include "stdafx.h"
#include "GMHuntingGround.h"

mu_uint64 g_MusicStartStamp = 0;

mu_boolean M31HuntingGround::CreateHuntingGroundObject(mu_uint32 objectIndex, OBJECT* pObject)
{
	switch(pObject->Type)
	{
	case 27:
	case 54:
		pObject->Timer = (mu_float)GetLargeRand(1000) * 0.01f;
		break;
	}

	return true;
}
mu_boolean M31HuntingGround::MoveHuntingGroundObject(const mu_uint32 ThreadIndex, OBJECT* pObject)
{
	mu_float Luminosity;
	EVector3 Light;

	switch (pObject->Type)
	{
	case 1:
		{
			mu_int32 time = g_CurrentTime % 1024;
			if (time >= 0 && time < 10)
			{
				EVector3 Light;
				Vector(1.0f, 1.0f, 1.0f, Light);
				CreateEffect(ThreadIndex, MODEL_BUTTERFLY01, pObject->Position, pObject->Angle, Light, 0, pObject);
			}
			pObject->HiddenMesh = -2;
		}
		break;
	case 44:
		{
			mu_int32 time = g_CurrentTime % 1024;
			if (time >= 0 && time < 10)
			{
				EVector3 Light;
				Vector(1.0f, 1.0f, 1.0f, Light);
				CreateEffect(ThreadIndex, MODEL_BUTTERFLY01, pObject->Position, pObject->Angle, Light, 1, pObject);
			}
			pObject->HiddenMesh = -2;
		}
		break;
	case 45:
		{
			mu_int32 time = g_CurrentTime % 1024;
			if (time >= 0 && time < 10)
			{
				EVector3 Light;
				Vector(1.0f, 1.0f, 1.0f, Light);
				CreateEffect(ThreadIndex, MODEL_BUTTERFLY01, pObject->Position, pObject->Angle, Light, 2, pObject);
			}
			pObject->HiddenMesh = -2;
		}
		break;
	case 3:
	case 53:
		pObject->HiddenMesh = -2;
		break;
	case 27:
		{
			mu_float vibration = EMath::Sin(pObject->Timer + FWorldTime*0.0024f)*0.3f;
			pObject->Position[2] += vibration;
		}
		break;
	case 42:
		{
			Luminosity = (mu_float)(GetLargeRand(4) + 3)*0.1f;
			Vector(Luminosity*0.9f, Luminosity*0.2f, Luminosity*0.1f, Light);
			AddTerrainLight(pObject->Position[0], pObject->Position[1], Light, 3, PrimaryTerrainLight);
			pObject->HiddenMesh = -2;
		}
		break;
	}

	if (g_CurrentTime - g_MusicStartStamp > 300000)
	{
		g_MusicStartStamp = g_CurrentTime;
		PlayBackground(SOUND_BC_HUNTINGGROUND_AMBIENT);
	}
	return true;
}

mu_boolean M31HuntingGround::RenderHuntingGroundObjectVisual(const mu_uint32 ThreadIndex, OBJECT* pObject, MUModel* pModel)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	EVector3 Light;

	switch (pObject->Type)
	{
	case 3:
		if (GetLargeRand(3) == 0)
		{
			Vector(1.0f, 1.0f, 1.0f, Light);
			CreateParticle(ThreadIndex, BITMAP_WATERFALL_3, pObject->Position, pObject->Angle, Light, 3, pObject->Scale);
		}
		break;
	case 53:
		if (GetLargeRand(3) == 0)
		{
			Vector(1.0f, 1.0f, 1.0f, Light);
			CreateParticle(ThreadIndex, BITMAP_SMOKE, pObject->Position, pObject->Angle, Light, 22, pObject->Scale);
		}
		break;
	case 49:
		{
			EVector3 Relative, Position;
			Vector(0.0f, -10.0f, 0.0f, Relative);
			pModel->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[pModel->GetBoneIndex(3)], Relative, Position, false);

			Vector(1.0f, 0.0f, 0.0f, Light);
			mu_float Luminosity = (mu_float)EMath::Sin((FWorldTime)*0.002f)*0.35f + 0.65f;
			CreateSprite(BITMAP_LIGHT, Position, Luminosity*1.2f + 0.3f, Light, pObject);
			CreateSprite(BITMAP_LIGHT, Position, 0.5f, Light, pObject);
			Vector(0.3f, 0.3f, 0.3f, Light);
			CreateSprite(BITMAP_SHINY + 1, Position, 1.2f, Light, nullptr, GetLargeRand(360));
		}
		break;
	}

	return true;
}

mu_boolean M31HuntingGround::RenderHuntingGroundObjectMesh(const mu_uint32 ThreadIndex, OBJECT* pObject, MUModel* pModel,mu_boolean ExtraMon)
{
	if (pObject->Type == 27 || pObject->Type == 54)
	{
		EVector3 LightBackup;
		VectorCopy(pModel->BodyLight(ThreadIndex), LightBackup);
		mu_float Luminosity = EMath::Sin(pObject->Timer + FWorldTime*0.0012f)*0.5f + 0.9f;
		pModel->BodyLight(ThreadIndex)[0] *= Luminosity;
		pModel->BodyLight(ThreadIndex)[1] *= Luminosity;
		pModel->BodyLight(ThreadIndex)[2] *= Luminosity;
		pModel->RenderBody(ThreadIndex, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, pObject->HiddenMesh);

		VectorCopy(LightBackup, pModel->BodyLight(ThreadIndex));

		return true;
	}
	else if (pObject->Type == 10)
	{
		pModel->BodyLight(ThreadIndex)[0] = 0.56f;
		pModel->BodyLight(ThreadIndex)[1] = 0.80f;
		pModel->BodyLight(ThreadIndex)[2] = 0.81f;
		pModel->RenderBody(ThreadIndex, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, pObject->HiddenMesh);
		return true;
	}
	else if (pObject->Type == 52)
	{
		mu_float Luminosity = EMath::Sin(pObject->Timer + FWorldTime*0.0009f)*0.5f + 0.9f;
		pModel->BodyLight(ThreadIndex)[0] *= Luminosity;
		pModel->BodyLight(ThreadIndex)[1] *= Luminosity;
		pModel->BodyLight(ThreadIndex)[2] *= Luminosity;
		pModel->RenderBody(ThreadIndex, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, pObject->HiddenMesh);
	}

	return false;
}

CHARACTER* M31HuntingGround::CreateHuntingGroundMonster(mu_int32 iType, mu_int32 PosX, mu_int32 PosY, mu_int32 Key)
{
	CHARACTER* pCharacter = nullptr;
	switch (iType)
	{
	case 290:
		{
			pCharacter = CreateCharacter(Key, MODEL_MONSTER01 + 81, PosX, PosY);
			pCharacter->Object.Scale = 1.2f;
			pCharacter->Weapon[0].Type = -1;
			pCharacter->Weapon[1].Type = -1;
		}
		break;
	case 291:
		{
			pCharacter = CreateCharacter(Key, MODEL_MONSTER01 + 82, PosX, PosY);
			pCharacter->Object.Scale = 1.8f;
			pCharacter->Weapon[0].Type = -1;
			pCharacter->Weapon[1].Type = -1;
		}
		break;
	case 292:
		{
			pCharacter = CreateCharacter(Key, MODEL_MONSTER01 + 83, PosX, PosY);
			pCharacter->Object.Scale = 1.4f;
			pCharacter->Weapon[0].Type = -1;
			pCharacter->Weapon[1].Type = -1;
		}
		break;

	case 303:
	case 293:
		{
			pCharacter = CreateCharacter(Key, MODEL_MONSTER01 + 84, PosX, PosY);
			pCharacter->Object.Scale = 1.4f;
			pCharacter->Weapon[0].Type = -1;
			pCharacter->Weapon[1].Type = -1;
		}
		break;

	case 302:
	case 294:
		{
			pCharacter = CreateCharacter(Key, MODEL_MONSTER01 + 85, PosX, PosY);
			pCharacter->Object.Scale = 0.7f;
			pCharacter->Weapon[0].Type = -1;
			pCharacter->Weapon[1].Type = -1;
		}
		break;

	case 295:
		{
			pCharacter = CreateCharacter(Key, MODEL_MONSTER01 + 87, PosX, PosY);
			pCharacter->Object.Scale = 2.0f;
			pCharacter->Weapon[0].Type = -1;
			pCharacter->Weapon[1].Type = -1;

			PlayBackground(SOUND_BC_EROHIM_ENTER);
		}
		break;

	case 297:
		{
			pCharacter = CreateCharacter(Key, MODEL_PLAYER, PosX, PosY);
			pCharacter->Skin = 1;
			pCharacter->BodyPart[BODYPART_HELM].Type = MODEL_HELM + 1;
			pCharacter->BodyPart[BODYPART_ARMOR].Type = MODEL_ARMOR + 1;
			pCharacter->BodyPart[BODYPART_PANTS].Type = MODEL_PANTS + 1;
			pCharacter->BodyPart[BODYPART_GLOVES].Type = MODEL_GLOVES + 1;
			pCharacter->BodyPart[BODYPART_BOOTS].Type = MODEL_BOOTS + 1;
			pCharacter->Weapon[0].Type = MODEL_SWORD + 16;
			pCharacter->Weapon[1].Type = MODEL_SHIELD + 13;
			mu_int32 Level = 9;
			pCharacter->BodyPart[BODYPART_HELM].Level = Level;
			pCharacter->BodyPart[BODYPART_ARMOR].Level = Level;
			pCharacter->BodyPart[BODYPART_PANTS].Level = Level;
			pCharacter->BodyPart[BODYPART_GLOVES].Level = Level;
			pCharacter->BodyPart[BODYPART_BOOTS].Level = Level;
			pCharacter->PK = PVP_MURDERER2;
			SetCharacterScale(pCharacter);
		}
		break;
	}

	return pCharacter;
}

mu_boolean M31HuntingGround::MoveHuntingGroundMonsterVisual(const mu_uint32 ThreadIndex, OBJECT* pObject, MUModel* pModel)
{
	switch (pObject->Type)
	{
	case MODEL_MONSTER01 + 82:
		{
			EVector3 Light;
			Vector(1.0f, 0.2f, 0.1f, Light);
			AddTerrainLight(pObject->Position[0], pObject->Position[1], Light, 3, PrimaryTerrainLight);
		}
		return true;
	}

	return false;
}

void M31HuntingGround::MoveHuntingGroundBlurEffect(const mu_uint32 ThreadIndex, CHARACTER* pCharacter, OBJECT* pObject, MUModel* pModel)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	switch (pObject->Type)
	{
	case MODEL_MONSTER01 + 85:
		{
			if (pObject->CurrentAction == MONSTER01_ATTACK1 || pObject->CurrentAction == MONSTER01_ATTACK2)
			{
				EVector3  Light;
				Vector(1.0f, 1.0f, 1.0f, Light);

				EVector3 StartPos, StartRelative;
				EVector3 EndPos, EndRelative;

				mu_float fActionSpeed = pModel->_AnimationHeader[pObject->CurrentAction].Speed;
				mu_float fSpeedPerFrame = fActionSpeed / 10.0;
				mu_float fAnimationFrame = pObject->AnimationFrame - fActionSpeed;
				for (mu_int32 i = 0; i < 10; ++i)
				{
					pModel->Animation(ThreadIndex, ThreadInfo.BoneTransform, fAnimationFrame, pObject->PriorAnimationFrame, pObject->PriorAction, pObject->Angle, pObject->HeadAngle);

					Vector(0.0f, -10.0f, -80.0f, StartRelative);
					Vector(30.0f, -30.0f, -230.0f, EndRelative);
					pModel->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[pModel->GetBoneIndex(23)], StartRelative, StartPos, false);
					pModel->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[pModel->GetBoneIndex(23)], EndRelative, EndPos, false);

					CreateBlur(pCharacter, StartPos, EndPos, Light, 3, true, 23);

					Vector(30.0f, 10.0f, 80.0f, StartRelative);
					Vector(30.0f, -65.0f, 230, EndRelative);
					pModel->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[pModel->GetBoneIndex(34)], StartRelative, StartPos, false);
					pModel->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[pModel->GetBoneIndex(34)], EndRelative, EndPos, false);

					CreateBlur(pCharacter, StartPos, EndPos, Light, 3, true, 34);

					fAnimationFrame += fSpeedPerFrame;
				}
			}
		}
		break;
	}
}

mu_boolean M31HuntingGround::RenderHuntingGroundMonsterVisual(const mu_uint32 ThreadIndex, CHARACTER* pCharacter, OBJECT* pObject, MUModel* pModel)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	switch (pObject->Type)
	{
	case MODEL_MONSTER01 + 81:
		{
			EVector3 Position, Light;
			Vector(0.9f, 0.2f, 0.1f, Light);
			BoneManager::GetBonePosition(pObject, 29, Position);
			CreateSprite(BITMAP_LIGHT, Position, 0.5f, Light, pObject);
			BoneManager::GetBonePosition(pObject, 30, Position);
			CreateSprite(BITMAP_LIGHT, Position, 0.5f, Light, pObject);

			if (pObject->CurrentAction == MONSTER01_WALK || pObject->CurrentAction == MONSTER01_RUN)
			{
				if (GetLargeRand(15) == 0)
				{
					PlayBackground(SOUND_BC_LIZARDWARRIOR_MOVE1 + GetLargeRand(2));
				}
			}
			if (pObject->CurrentAction == MONSTER01_ATTACK1 || pObject->CurrentAction == MONSTER01_ATTACK2)
			{
				if (pObject->SubType == 0)
				{
					pObject->SubType = 1;
					PlayBackground(SOUND_BC_LIZARDWARRIOR_ATTACK1 + GetLargeRand(2));
				}
			}
			if (pObject->CurrentAction == MONSTER01_DIE)
			{
				if (pObject->SubType == 0)
				{
					pObject->SubType = 1;
					PlayBackground(SOUND_BC_LIZARDWARRIOR_DIE);
				}
			}
			if (pObject->CurrentAction == MONSTER01_STOP1 || pObject->CurrentAction == MONSTER01_STOP2)
				pObject->SubType = 0;
		}
		break;
	case MODEL_MONSTER01 + 82:
		{
			EVector3 Light;
			Vector(1.0f, 1.0f, 1.0f, Light);

			EVector3 Position, Relative;

			if (pObject->CurrentAction == MONSTER01_DIE)
			{
				pModel->RenderMeshEffect(ThreadIndex, pObject->BoneTransform, 0, MODEL_GOLEM_STONE);
				pObject->Flags.Clear(OBJECT::eOF_LIVE);
				PlayBackground(SOUND_BC_FIREGOLEM_DIE);
			}
			else
			{
				Vector(5.0f, 10.0f, 5.0f, Relative);
				BoneManager::GetBonePosition(pObject, 34, Relative, Position);
				CreateParticle(ThreadIndex, BITMAP_TRUE_FIRE, Position, pObject->Angle, Light, 3, 3.4f, pObject);
				CreateParticle(ThreadIndex, BITMAP_SMOKE, Position, pObject->Angle, Light, 21, 2.5f);

				BoneManager::GetBonePosition(pObject, 45, Position);
				CreateParticle(ThreadIndex, BITMAP_TRUE_FIRE, Position, pObject->Angle, Light, 4, 3.4f, pObject);
				CreateParticle(ThreadIndex, BITMAP_SMOKE, Position, pObject->Angle, Light, 21, 2.5f);

				Vector(-20.0f, -2.0f, -10.0f, Relative);
				BoneManager::GetBonePosition(pObject, 48, Relative, Position);
				CreateParticle(ThreadIndex, BITMAP_TRUE_FIRE, Position, pObject->Angle, Light, 0, 0.8f);
				CreateParticle(ThreadIndex, BITMAP_SMOKE, Position, pObject->Angle, Light, 21, 0.8f);

				BoneManager::GetBonePosition(pObject, 46, Position);
				CreateParticle(ThreadIndex, BITMAP_SMOKE, Position, pObject->Angle, Light, 21, 0.8f);

				if (GetLargeRand(20) == 0)
				{
					PlayBackground(SOUND_BC_FIREGOLEM_MOVE1 + GetLargeRand(2));
				}
			}

			if (pObject->CurrentAction == MONSTER01_ATTACK1 && pObject->AnimationFrame >= 8.4f && pObject->AnimationFrame <= 10.2f)
			{
				if (pObject->SubType == 0)
				{
					pObject->SubType = 1;

					BoneManager::GetBonePosition(pObject, 45, Position);
					Position[2] = pObject->Position[2];

					EVector3 Angle;
					Vector(-90.0f, 0.0f, pObject->Angle[2], Angle);

					CreateEffect(ThreadIndex, BITMAP_CRATER, Position, Angle, Light, 2);
					CreateJoint(ThreadIndex, BITMAP_JOINT_FORCE, Position, Position, Angle, 6, nullptr, 200.0f);
					CreateEffect(ThreadIndex, BITMAP_FLAME, Position, pObject->Angle, pObject->Light, 3);

					for (mu_int32 i = 0; i < MAX_CHARACTERS_CLIENT; ++i)
					{
						CHARACTER* tc = &CharactersClient[i];
						OBJECT *to = &tc->Object;
						if (to->Flags.Get(OBJECT::eOF_LIVE | OBJECT::eOF_VISIBLE) == true)
						{
							if (to->Kind == KIND_PLAYER && to->Type == MODEL_PLAYER)
							{
								mu_float Distance = VectorDistance2D(to->Position, Position);
								if (Distance > 200.0f && Distance < 1100.0f)
								{
									CreateEffect(ThreadIndex, BITMAP_CRATER, to->Position, Angle, Light, 2);
									CreateJoint(ThreadIndex, BITMAP_JOINT_FORCE, to->Position, to->Position, Angle, 6, nullptr, 200.0f);
									CreateEffect(ThreadIndex, BITMAP_FLAME, to->Position, to->Angle, Light, 3);
								}
							}
						}
					}

					PlayBackground(SOUND_BC_FIREGOLEM_ATTACK1);
				}
			}
			if (pObject->CurrentAction == MONSTER01_ATTACK2 && pObject->AnimationFrame >= 5.0f && pObject->AnimationFrame <= 5.7f)
			{
				if (pObject->SubType == 0)
				{
					pObject->SubType = 1;

					Vector(80.0f, 40.0f, -25.0f, Relative);
					BoneManager::GetBonePosition(pObject, 34, Relative, Position);
					CreateBomb(ThreadIndex, Position, true);

					PlayBackground(SOUND_BC_FIREGOLEM_ATTACK2);
				}
			}
			if (pObject->CurrentAction == MONSTER01_STOP1 || pObject->CurrentAction == MONSTER01_STOP2)
				pObject->SubType = 0;
		}
		break;
	case MODEL_MONSTER01 + 83:
		{
			if (pObject->CurrentAction == MONSTER01_WALK || pObject->CurrentAction == MONSTER01_RUN)
			{
				if (pObject->SubType == 0)
				{
					pObject->SubType = 1;
					PlayBackground(SOUND_BC_QUEENBEE_MOVE1 + GetLargeRand(2));
				}
			}
			if (pObject->CurrentAction == MONSTER01_ATTACK1 || pObject->CurrentAction == MONSTER01_ATTACK2)
			{
				if (pObject->SubType == 0)
				{
					pObject->SubType = 1;
					PlayBackground(SOUND_BC_QUEENBEE_ATTACK1 + GetLargeRand(2));
				}
			}
			if (pObject->CurrentAction == MONSTER01_DIE)
			{
				if (pObject->SubType == 0)
				{
					pObject->SubType = 1;
					PlayBackground(SOUND_BC_AXEWARRIOR_DIE);
				}
			}
			if (pObject->CurrentAction == MONSTER01_ATTACK2)
			{
				EVector3 Position, Relative;
				Vector(GetLargeRand(12) - 6, GetLargeRand(12) - 6, GetLargeRand(12) - 6, Relative);
				BoneManager::GetBonePosition(pObject, 62, Relative, Position);

				EVector3 Light;
				Vector(1.0f, 0.3f, 0.0f, Light);

				CreateSprite(BITMAP_SHINY + 1, Position, 2.5f, pObject->Light, pObject, 0.0f, 1);
				CreateSprite(BITMAP_MAGIC + 1, Position, 0.8f, pObject->Light, pObject, 0.0f);
				CreateParticle(ThreadIndex, BITMAP_ENERGY, Position, pObject->Angle, Light);
				CreateParticle(ThreadIndex, BITMAP_ENERGY, Position, pObject->Angle, Light);
			}
			if (pObject->CurrentAction == MONSTER01_STOP1 || pObject->CurrentAction == MONSTER01_STOP2)
				pObject->SubType = 0;
		}
		break;

	case MODEL_MONSTER01 + 84:
		{
			EVector3 Light, Position;

			Vector(0.4f, 1.0f, 0.7f, Light);

			if (pObject->CurrentAction == MONSTER01_WALK || pObject->CurrentAction == MONSTER01_RUN)
			{
				if (pObject->SubType == 0)
				{
					pObject->SubType = 1;
					PlayBackground(SOUND_BC_POISONGOLEM_MOVE1 + GetLargeRand(2));
				}
			}

			if (pObject->CurrentAction == MONSTER01_DIE)
			{
				pModel->RenderMeshEffect(ThreadIndex, pObject->BoneTransform, 0, MODEL_GOLEM_STONE, 1);
				pObject->Flags.Clear(OBJECT::eOF_LIVE);
				PlayBackground(SOUND_BC_POISONGOLEM_DIE);
			}

			if (pObject->CurrentAction == MONSTER01_ATTACK1)
			{
				if (pObject->SubType == 0)
				{
					pObject->SubType = 1;
					PlayBackground(SOUND_BC_POISONGOLEM_ATTACK3);
				}

				EVector3 Angle;
				Vector(-55.0f, EMath::Sin(FWorldTime*0.03f)*45.0f, pObject->Angle[2], Angle);
				if (GetLargeRand(2) == 0)
				{
					if (GetLargeRand(2) == 0)
						BoneManager::GetBonePosition(pObject, 54, Position);
					else
						BoneManager::GetBonePosition(pObject, 55, Position);
					CreateParticle(ThreadIndex, BITMAP_SMOKE, Position, pObject->Angle, Light, 11, (mu_float)(GetLargeRand(32) + 50)*0.025f);
				}
				CreateEffect(ThreadIndex, MODEL_BIG_STONE_PART2, Position, Angle, Light, 3);
			}
			if (pObject->CurrentAction == MONSTER01_ATTACK2 && pObject->AnimationFrame >= 3.5f && pObject->AnimationFrame <= 4.2f)
			{
				if (pObject->SubType == 0)
				{
					pObject->SubType = 1;
					PlayBackground(SOUND_BC_POISONGOLEM_ATTACK1 + GetLargeRand(2));
				}

				BoneManager::GetBonePosition(pObject, 50, Position);
				Position[2] = pObject->Position[2];
				CreateParticle(ThreadIndex, BITMAP_SMOKE, Position, pObject->Angle, Light, 11, (mu_float)(GetLargeRand(32) + 50)*0.05f);

				BoneManager::GetBonePosition(pObject, 37, Position);
				Position[2] = pObject->Position[2];
				CreateParticle(ThreadIndex, BITMAP_SMOKE, Position, pObject->Angle, Light, 11, (mu_float)(GetLargeRand(32) + 50)*0.05f);
			}

			if (GetLargeRand(10) == 0)
			{
				BoneManager::GetBonePosition(pObject, 53, Position);
				CreateParticle(ThreadIndex, BITMAP_SMOKE, Position, pObject->Angle, Light, 23);
			}
			if (GetLargeRand(10) == 0)
			{
				BoneManager::GetBonePosition(pObject, 54, Position);
				CreateParticle(ThreadIndex, BITMAP_SMOKE, Position, pObject->Angle, Light, 23);
			}
			if (GetLargeRand(10) == 0)
			{
				BoneManager::GetBonePosition(pObject, 55, Position);
				CreateParticle(ThreadIndex, BITMAP_SMOKE, Position, pObject->Angle, Light, 23);
			}
			if (pObject->CurrentAction == MONSTER01_STOP1 || pObject->CurrentAction == MONSTER01_STOP2)
				pObject->SubType = 0;
		}
		break;

	case MODEL_MONSTER01 + 85:
		{
			EVector3 Position, Light;
			Vector(0.9f, 0.2f, 0.1f, Light);
			BoneManager::GetBonePosition(pObject, 18, Position);
			CreateSprite(BITMAP_LIGHT, Position, 0.7f, Light, pObject);
			BoneManager::GetBonePosition(pObject, 19, Position);
			CreateSprite(BITMAP_LIGHT, Position, 0.7f, Light, pObject);

			if (pObject->CurrentAction == MONSTER01_WALK || pObject->CurrentAction == MONSTER01_RUN)
			{
				if (GetLargeRand(10) == 0)
				{
					CreateParticle(ThreadIndex, BITMAP_SMOKE + 1, pObject->Position, pObject->Angle, Light);
					PlayBackground(SOUND_BC_AXEWARRIOR_MOVE1 + GetLargeRand(2));
				}
			}
			if (pObject->CurrentAction == MONSTER01_ATTACK1 || pObject->CurrentAction == MONSTER01_ATTACK2)
			{
				if (pObject->SubType == 0)
				{
					pObject->SubType = 1;
					PlayBackground(SOUND_BC_AXEWARRIOR_ATTACK1 + GetLargeRand(2));
				}
			}
			if (pObject->CurrentAction == MONSTER01_DIE)
			{
				if (pObject->SubType == 0)
				{
					pObject->SubType = 1;
					PlayBackground(SOUND_BC_AXEWARRIOR_DIE);
				}
			}
			if (pObject->CurrentAction == MONSTER01_STOP1 || pObject->CurrentAction == MONSTER01_STOP2)
				pObject->SubType = 0;
		}
		break;
	case MODEL_MONSTER01 + 87:
		{
			EVector3 Position, Relative, Light;
			Vector(0.9f, 0.2f, 0.1f, Light);
			Vector(0.0f, -2.0f, 0.0f, Relative);
			BoneManager::GetBonePosition(pObject, 8, Relative, Position);
			CreateSprite(BITMAP_LIGHT, Position, 0.5f, Light, pObject);
			CreateSprite(BITMAP_LIGHT, Position, 0.5f, Light, pObject);
			BoneManager::GetBonePosition(pObject, 9, Relative, Position);
			CreateSprite(BITMAP_LIGHT, Position, 0.5f, Light, pObject);
			CreateSprite(BITMAP_LIGHT, Position, 0.5f, Light, pObject);

			if (pObject->CurrentAction == MONSTER01_ATTACK1)
			{
				if (pObject->SubType == 0)
				{
					pObject->SubType = 1;
					PlayBackground(SOUND_BC_EROHIM_ATTACK1 + GetLargeRand(2));
				}
			}
			if (pObject->CurrentAction == MONSTER01_DIE)
			{
				if (pObject->SubType == 0)
				{
					pObject->SubType = 1;
					PlayBackground(SOUND_BC_EROHIM_DIE);
				}
			}

			if (pObject->CurrentAction == MONSTER01_ATTACK2)
			{
				if (pObject->SubType == 0)
				{
					pObject->SubType = 1;
					CreateEffect(ThreadIndex, MODEL_SKILL_FISSURE, pObject->Position, pObject->Angle, pObject->Light, 0, pObject);
					PlayBackground(SOUND_BC_EROHIM_ATTACK3);
				}
				Vector(0.0f, 0.0f, -10.0f, Relative);
				BoneManager::GetBonePosition(pObject, 16, Relative, Position);
				CreateParticle(ThreadIndex, BITMAP_TRUE_FIRE, Position, pObject->Angle, Light, 3, 3.0f, pObject);
				CreateParticle(ThreadIndex, BITMAP_SMOKE, Position, pObject->Angle, Light, 21, 2.0f);
			}
			if (pObject->CurrentAction == MONSTER01_STOP1 || pObject->CurrentAction == MONSTER01_STOP2)
				pObject->SubType = 0;
		}
		break;
	}
	return false;
}

mu_boolean M31HuntingGround::RenderHuntingGroundMonsterObjectMesh(const mu_uint32 ThreadIndex, OBJECT* pObject, MUModel* pModel, mu_boolean ExtraMon)
{
	switch (pObject->Type)
	{
	case MODEL_MONSTER01 + 81:
		{
			pModel->RenderBody(ThreadIndex, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, 1);
			EVector3 LightBackup;
			VectorCopy(pModel->BodyLight(ThreadIndex), LightBackup);
			Vector(0.6f, 0.4f, 0.4f, pModel->BodyLight(ThreadIndex));
			pModel->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV);
			VectorCopy(LightBackup, pModel->BodyLight(ThreadIndex));
			return true;
		}
		break;
	case MODEL_MONSTER01 + 82:
		{
			EVector3 LightBackup;
			VectorCopy(pModel->BodyLight(ThreadIndex), LightBackup);
			mu_float Luminosity = EMath::Sin(FWorldTime*0.0012f)*0.8f + 1.3f;
			if (Luminosity > 1.3f)
				Luminosity = 1.3f;

			pModel->BodyLight(ThreadIndex)[0] *= Luminosity;
			pModel->BodyLight(ThreadIndex)[1] *= Luminosity;
			pModel->BodyLight(ThreadIndex)[2] *= Luminosity;

			pModel->StreamMesh(ThreadIndex) = 0;
			pModel->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, IWorldTime % 10000 * 0.0002f, IWorldTime % 10000 * 0.0002f);
			pModel->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV);
			pModel->StreamMesh(ThreadIndex) = -1;

			VectorCopy(LightBackup, pModel->BodyLight(ThreadIndex));

			pModel->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV);
			return true;
		}
		break;
	case MODEL_MONSTER01 + 83:
		{
			pModel->RenderBody(ThreadIndex, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, 0);

			EVector3 LightBackup;
			VectorCopy(pModel->BodyLight(ThreadIndex), LightBackup);

			Vector(1.0f, 1.0f, 1.0f, pModel->BodyLight(ThreadIndex));
			pModel->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, pObject->Alpha, -1, 1.0f, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV);
			Vector(0.8f, 0.6f, 1.0f, pModel->BodyLight(ThreadIndex));
			pModel->RenderMesh(ThreadIndex, 0, RENDER_CHROME | RENDER_BRIGHT, pObject->Alpha, -1, 1.0f, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV);

			VectorCopy(LightBackup, pModel->BodyLight(ThreadIndex));

			return true;
		}
		break;
	case MODEL_MONSTER01 + 84:
		{
			EVector3 LightBackup;
			if (ExtraMon)
			{
				Vector(1.0f, 1.0f, 1.0f, pModel->BodyLight(ThreadIndex));
			}
			VectorCopy(pModel->BodyLight(ThreadIndex), LightBackup);
			mu_float Luminosity = EMath::Sin(FWorldTime*0.0012f)*0.8f + 1.3f;
			if (Luminosity > 1.3f)
				Luminosity = 1.3f;

			pModel->BodyLight(ThreadIndex)[0] *= (Luminosity*0.1f);
			pModel->BodyLight(ThreadIndex)[1] *= Luminosity;
			pModel->BodyLight(ThreadIndex)[2] *= (Luminosity*0.1f);

			pModel->StreamMesh(ThreadIndex) = 1;
			pModel->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, IWorldTime % 10000 * 0.0002f, IWorldTime % 10000 * 0.0002f);

			if (ExtraMon)
				pModel->RenderMesh(ThreadIndex, 1, RENDER_CHROME | RENDER_BRIGHT, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, BITMAP_CHROME);
			else
				pModel->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV);

			pModel->StreamMesh(ThreadIndex) = -1;

			VectorCopy(LightBackup, pModel->BodyLight(ThreadIndex));

			pModel->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, EMath::Sin(FWorldTime*0.0012f)*0.2f + 0.8f, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV);

			if (ExtraMon)
			{
				pModel->RenderMesh(ThreadIndex, 0, RENDER_CHROME | RENDER_BRIGHT, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, BITMAP_CHROME);
			}

			pModel->StreamMesh(ThreadIndex) = 2;
			pModel->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, pObject->Alpha, 2, 0.5f, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV);

			if (ExtraMon)
			{
				pModel->RenderMesh(ThreadIndex, 2, RENDER_CHROME | RENDER_BRIGHT, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, BITMAP_CHROME);
				pObject->Scale = pModel->BodyScale(ThreadIndex);
			}

			pModel->StreamMesh(ThreadIndex) = -1;
			return true;
		}
		break;
	case MODEL_MONSTER01 + 85:
		{
			EVector3 LightBackup;
			if (ExtraMon)
			{
				Vector(1.0f, 1.0f, 1.0f, pModel->BodyLight(ThreadIndex));
			}
			VectorCopy(pModel->BodyLight(ThreadIndex), LightBackup);
			pModel->BodyLight(ThreadIndex)[0] *= 0.5f;
			pModel->BodyLight(ThreadIndex)[1] *= 0.6f;
			pModel->BodyLight(ThreadIndex)[2] *= 0.8f;
			pModel->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV);
			if (ExtraMon)
				pModel->RenderMesh(ThreadIndex, 0, RENDER_CHROME | RENDER_BRIGHT, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, BITMAP_CHROME);
			Vector(0.5f, 0.8f, 0.6f, pModel->BodyLight(ThreadIndex));
			pModel->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV);
			if (ExtraMon)
				pModel->RenderMesh(ThreadIndex, 1, RENDER_CHROME | RENDER_BRIGHT, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, BITMAP_CHROME);
			VectorCopy(LightBackup, pModel->BodyLight(ThreadIndex));
			return true;
		}
		break;
	case MODEL_MONSTER01 + 87:
		{
			pModel->RenderBody(ThreadIndex, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, 5);

			pObject->Alpha = 1.0f;
			pModel->RenderMesh(ThreadIndex, 5, RENDER_TEXTURE, pObject->Alpha, -1, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, BITMAP_HGBOSS_WING);
			pModel->RenderMesh(ThreadIndex, 5, RENDER_TEXTURE, pObject->Alpha, -1, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV);

			EVector3 LightBackup;
			VectorCopy(pModel->BodyLight(ThreadIndex), LightBackup);

			Vector(1.0f, 0.5f, 0.0f, pModel->BodyLight(ThreadIndex));
			pModel->RenderMesh(ThreadIndex, 4, RENDER_CHROME | RENDER_BRIGHT, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV);

			Vector(0.75f, 0.65f, 0.5f, pModel->BodyLight(ThreadIndex));
			pModel->RenderMesh(ThreadIndex, 0, RENDER_CHROME | RENDER_BRIGHT, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV);
			pModel->RenderMesh(ThreadIndex, 1, RENDER_CHROME | RENDER_BRIGHT, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV);
			pModel->RenderMesh(ThreadIndex, 3, RENDER_CHROME | RENDER_BRIGHT, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV);

			mu_float Luminosity = EMath::Sin(FWorldTime*0.0012f)*0.3f + 0.6f;
			pModel->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, pObject->Alpha, 0, Luminosity, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, BITMAP_HGBOSS_PATTERN);

			VectorCopy(LightBackup, pModel->BodyLight(ThreadIndex));

			return true;
		}
		break;
	case MODEL_FISSURE:
		{
			Vector(1.0f, 1.0f, 1.0f, pModel->BodyLight(ThreadIndex));
			pModel->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, pObject->Alpha, -1, 1.0f, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV);
			pModel->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, pObject->Alpha, 0, 1.0f, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, BITMAP_FISSURE_FIRE);

			return true;
		}
		break;
	case MODEL_FISSURE_LIGHT:
		{
			Vector(1.0f, 1.0f, 1.0f, pModel->BodyLight(ThreadIndex));

			pObject->BlendMeshTexCoordU = EMath::Sin(FWorldTime*0.00008f)*2.5f;
			pModel->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, pObject->Alpha, 0, 1.0f, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV);
			pObject->BlendMeshTexCoordU = 1.0f;

			return true;
		}
		break;
	}
	return false;
}

mu_boolean M31HuntingGround::AttackEffectHuntingGroundMonster(const mu_uint32 ThreadIndex, CHARACTER* pCharacter, OBJECT* pObject, MUModel* pModel)
{
	switch (pCharacter->MonsterIndex)
	{
	case 290:
		break;
	case 292:
	case 303:
	case 293:
	case 291:
	case 295:
		return true;

	case 302:
	case 294:
		break;
	}
	return false;
}

mu_boolean M31HuntingGround::SetCurrentActionHuntingGroundMonster(CHARACTER* pCharacter, OBJECT* pObject)
{
	switch (pCharacter->MonsterIndex)
	{
	case 291:
		if (pCharacter->Skill == AT_SKILL_BOSS)
		{
			SetAction(pObject, MONSTER01_ATTACK1);
		}
		else
		{
			SetAction(pObject, MONSTER01_ATTACK2);
		}
		return true;
	}

	return false;
}

mu_boolean M31HuntingGround::CreateMist(const mu_uint32 ThreadIndex, PARTICLE_HEADER *pParticleHeader, PARTICLE* pParticleObj)
{
	if (!IsInHuntingGround())
		return false;
	if (!IsInHuntingGroundSection2(Hero->Object.Position))
		return false;

	if (GetLargeRand(30) == 0)
	{
		EVector3 Light;
		Vector(0.05f, 0.05f, 0.1f, Light);

		EVector3 TargetPosition = { 0.0f, 0.0f, 0.0f }, TargetAngle = { 0.0f, 0.0f, 0.0f };
		switch (GetLargeRand(8))
		{
		case 0:
			TargetPosition[0] = Hero->Object.Position[0] + (300 + GetLargeRand(250));
			TargetPosition[1] = Hero->Object.Position[1] + (300 + GetLargeRand(250));
			break;
		case 1:
			TargetPosition[0] = Hero->Object.Position[0] + (250 + GetLargeRand(250));
			TargetPosition[1] = Hero->Object.Position[1] - (250 + GetLargeRand(250));
			break;
		case 2:
			TargetPosition[0] = Hero->Object.Position[0] - (200 + GetLargeRand(250));
			TargetPosition[1] = Hero->Object.Position[1] + (200 + GetLargeRand(250));
			break;
		case 3:
			TargetPosition[0] = Hero->Object.Position[0] - (300 + GetLargeRand(250));
			TargetPosition[1] = Hero->Object.Position[1] - (300 + GetLargeRand(250));
			break;
		case 4:
			TargetPosition[0] = Hero->Object.Position[0] + (400 + GetLargeRand(250));
			TargetPosition[1] = Hero->Object.Position[1];
			break;
		case 5:
			TargetPosition[0] = Hero->Object.Position[0] - (400 + GetLargeRand(250));
			TargetPosition[1] = Hero->Object.Position[1];
			break;
		case 6:
			TargetPosition[0] = Hero->Object.Position[0];
			TargetPosition[1] = Hero->Object.Position[1] + (400 + GetLargeRand(250));
			break;
		case 7:
			TargetPosition[0] = Hero->Object.Position[0];
			TargetPosition[1] = Hero->Object.Position[1] - (400 + GetLargeRand(250));
			break;
		}

		if (Hero->Movement)
		{
			EBone Matrix;
			AngleMatrix(Hero->Object.Angle, Matrix);
			EVector3 Velocity, Direction;
			Vector(0.0f, -45.0f*CharacterMoveSpeed(Hero), 0.0f, Velocity);
			VectorRotate(Velocity, Matrix, Direction);
			VectorAdd(TargetPosition, Direction, TargetPosition);
		}

		if (Hero->Movement || (GetLargeRand(2) == 0))
		{
			TargetPosition[2] = (GetLargeRand(20)) + RequestTerrainHeight(TargetPosition[0], TargetPosition[1]);
			CreateParticle(ThreadIndex, BITMAP_CLOUD, TargetPosition, TargetAngle, Light, 8, 0.4f);
		}
		pParticleHeader->Flags.Clear(PARTICLE::ePF_LIVE);

		return true;
	}

	return false;
}