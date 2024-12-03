#include "stdafx.h"
#include "GMAida.h"

mu_boolean M33Aida::MoveAidaObject(OBJECT* pObject)
{
	if (!IsInAida())
		return false;

	mu_float Luminosity;
	EVector3 Light;

	switch (pObject->Type)
	{
	case 25:
		{
			pObject->BlendMeshTexCoordV -= 0.015f;
		}
		break;
	case 28:
		{
			pObject->BlendMeshTexCoordV -= 0.015f;
		}
		break;
	case 30:
		{
			Luminosity = (mu_float)(GetLargeRand(5))*0.01f;
			Vector(Luminosity + 0.4f, Luminosity + 0.6f, Luminosity + 0.4f, Light);
			AddTerrainLight(pObject->Position[0], pObject->Position[1], Light, 2, PrimaryTerrainLight);
		}
		break;
	case 71:
		{
			Luminosity = (mu_float)(GetLargeRand(5))*0.01f;
			Vector(Luminosity + 0.9f, Luminosity + 0.2f, Luminosity + 0.2f, Light);
			AddTerrainLight(pObject->Position[0], pObject->Position[1], Light, 2, PrimaryTerrainLight);
		}
		break;
	case 41:
		{
			pObject->Alpha = 0.5f;
		}
		break;
	case    56:
	case    57:
	case    58:
	case	59:
	case	62:
	case	63:
	case	67:
	case	70:
		pObject->HiddenMesh = -2;
		break;
	case	64:
		pObject->Velocity = 0.05f;
		break;
	}

	PlayBackground(SOUND_AIDA_AMBIENT);

	return true;
}

mu_boolean M33Aida::RenderAidaObjectVisual(const mu_uint32 ThreadIndex, OBJECT* pObject, MUModel* pModel)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	EVector3 p, Position, Light;

	switch (pObject->Type)
	{
	case 30:
		{
			Vector(0.0f, -3.0f, 1.0f, p);
			pModel->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[pModel->GetBoneIndex(6)], p, Position, false);
			Vector(0.1f, 0.1f, 0.3f, Light);
			CreateSprite(BITMAP_SPARK + 1, Position, 5.5f, Light, pObject);
			Vector(0.15f, 0.15f, 0.15f, Light);
			CreateSprite(BITMAP_SPARK + 1, Position, 3.0f, Light, pObject);

			Vector(0.0f, -3.0f, 0.0f, p);
			pModel->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[pModel->GetBoneIndex(7)], p, Position, false);
			Vector(0.1f, 0.1f, 0.3f, Light);
			CreateSprite(BITMAP_SPARK + 1, Position, 4.5f, Light, pObject);
			Vector(0.15f, 0.15f, 0.15f, Light);
			CreateSprite(BITMAP_SPARK + 1, Position, 3.0f, Light, pObject);

			Vector(0.0f, -3.0f, 0.0f, p);
			pModel->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[pModel->GetBoneIndex(8)], p, Position, false);
			Vector(0.1f, 0.1f, 0.3f, Light);
			CreateSprite(BITMAP_SPARK + 1, Position, 4.0f, Light, pObject);
			Vector(0.15f, 0.15f, 0.15f, Light);
			CreateSprite(BITMAP_SPARK + 1, Position, 3.0f, Light, pObject);

			Vector(-3.0f, -1.0f, 0.0f, p);
			pModel->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[pModel->GetBoneIndex(12)], p, Position, false);
			Vector(0.1f, 0.1f, 0.3f, Light);
			CreateSprite(BITMAP_SPARK + 1, Position, 4.5f, Light, pObject);
			Vector(0.15f, 0.15f, 0.15f, Light);
			CreateSprite(BITMAP_SPARK + 1, Position, 3.0f, Light, pObject);

			Vector(-3.0f, -3.0f, 0.0f, p);
			pModel->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[pModel->GetBoneIndex(13)], p, Position, false);
			Vector(0.1f, 0.1f, 0.3f, Light);
			CreateSprite(BITMAP_SPARK + 1, Position, 4.0f, Light, pObject);
			Vector(0.15f, 0.15f, 0.15f, Light);
			CreateSprite(BITMAP_SPARK + 1, Position, 3.0f, Light, pObject);

			Vector(0.0f, 2.0f, -8.0f, p);
			pModel->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[pModel->GetBoneIndex(17)], p, Position, false);
			Vector(0.1f, 0.1f, 0.3f, Light);
			CreateSprite(BITMAP_SPARK + 1, Position, 4.0f, Light, pObject);
			Vector(0.15f, 0.15f, 0.15f, Light);
			CreateSprite(BITMAP_SPARK + 1, Position, 3.0f, Light, pObject);

		}
		break;
	case 56:
		Vector(1.0f, 1.0f, 1.0f, Light);
		if (GetLargeRand(2) == 0)
		{
			CreateParticle(ThreadIndex, BITMAP_WATERFALL_1, pObject->Position, pObject->Angle, Light, 2, pObject->Scale);
		}
		break;
	case 57:
		Vector(1.0f, 1.0f, 1.0f, Light);
		CreateParticle(ThreadIndex, BITMAP_WATERFALL_3, pObject->Position, pObject->Angle, Light, 4, pObject->Scale);
		break;
	case 58:
		Vector(1.0f, 1.0f, 1.0f, Light);
		if (GetLargeRand(3) == 0)
		{
			CreateParticle(ThreadIndex, BITMAP_WATERFALL_2, pObject->Position, pObject->Angle, Light, 2, pObject->Scale);
		}
		break;
	case 59:
		if (pObject->HiddenMesh != -2)
		{
			EVector3  Light;
			Vector(0.01f, 0.03f, 0.05f, Light);
			for (mu_int32 i = 0; i < 20; ++i)
			{
				CreateParticle(ThreadIndex, BITMAP_CLOUD, pObject->Position, pObject->Angle, Light, 1, pObject->Scale, pObject);
			}
		}
		break;
	case 60:
		{
			mu_int32 time = g_CurrentTime % 1024;
			if (time >= 0 && time < 10)
			{
				EVector3 Light;
				Vector(1.0f, 1.0f, 1.0f, Light);
				CreateEffect(ThreadIndex, MODEL_BUTTERFLY01, pObject->Position, pObject->Angle, Light, 3, pObject);
			}
			pObject->HiddenMesh = -2;
		}
		break;
	case 62:
		if (pObject->HiddenMesh != -2)
		{
			EVector3  Light;
			Vector(0.05f, 0.05f, 0.05f, Light);
			for (mu_int32 i = 0; i < 20; ++i)
			{
				CreateParticle(ThreadIndex, BITMAP_CLOUD, pObject->Position, pObject->Angle, Light, 1, pObject->Scale, pObject);
			}
		}
		break;
	case 63:
		if (pObject->HiddenMesh != -2)
		{
			EVector3  Light;
			Vector(0.05f, 0.02f, 0.02f, Light);
			for (mu_int32 i = 0; i < 20; ++i)
			{
				CreateParticle(ThreadIndex, BITMAP_CLOUD, pObject->Position, pObject->Angle, Light, 1, pObject->Scale, pObject);
			}
		}
		break;
	case	67:
		Vector(0.3f, 0.3f, 0.3f, pObject->Light);
		CreateParticle(ThreadIndex, BITMAP_FLAME, pObject->Position, pObject->Angle, pObject->Light, 7, pObject->Scale);
		break;
	case	70:
		{
			mu_int32 time = g_CurrentTime % 1024;
			if (GetLargeRand(5) == 1 && (time >= 0 && time < 30))
			{
				Vector(0.1f, 0.1f, 0.1f, Light);
				CreateEffect(ThreadIndex, MODEL_GHOST, pObject->Position, pObject->Angle, Light, 0, pObject, -1, 0, pObject->Scale);
			}
		}
		break;
	case	71:
		{
			Vector(0.0f, -3.0f, 1.0f, p);
			pModel->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[pModel->GetBoneIndex(6)], p, Position, false);
			Vector(0.5f, 0.0f, 0.0f, Light);
			CreateSprite(BITMAP_SPARK + 1, Position, 6.0f, Light, pObject);
			Vector(0.15f, 0.0f, 0.0f, Light);
			CreateSprite(BITMAP_SPARK + 1, Position, 3.0f, Light, pObject);

			Vector(0.0f, -3.0f, 0.0f, p);
			pModel->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[pModel->GetBoneIndex(7)], p, Position, false);
			Vector(0.25f, 0.0f, 0.0f, Light);
			CreateSprite(BITMAP_SPARK + 1, Position, 5.5f, Light, pObject);
			Vector(0.15f, 0.0f, 0.0f, Light);
			CreateSprite(BITMAP_SPARK + 1, Position, 3.0f, Light, pObject);

			Vector(0.0f, -3.0f, 0.0f, p);
			pModel->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[pModel->GetBoneIndex(8)], p, Position, false);
			Vector(0.25f, 0.0f, 0.0f, Light);
			CreateSprite(BITMAP_SPARK + 1, Position, 5.0f, Light, pObject);
			Vector(0.15f, 0.0f, 0.0f, Light);
			CreateSprite(BITMAP_SPARK + 1, Position, 3.0f, Light, pObject);

			Vector(-3.0f, -1.0f, 0.0f, p);
			pModel->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[pModel->GetBoneIndex(12)], p, Position, false);
			Vector(0.25f, 0.0f, 0.0f, Light);
			CreateSprite(BITMAP_SPARK + 1, Position, 5.5f, Light, pObject);
			Vector(0.15f, 0.0f, 0.0f, Light);
			CreateSprite(BITMAP_SPARK + 1, Position, 3.0f, Light, pObject);

			Vector(-3.0f, -3.0f, 0.0f, p);
			pModel->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[pModel->GetBoneIndex(13)], p, Position, false);
			Vector(0.25f, 0.0f, 0.0f, Light);
			CreateSprite(BITMAP_SPARK + 1, Position, 5.0f, Light, pObject);
			Vector(0.15f, 0.0f, 0.0f, Light);
			CreateSprite(BITMAP_SPARK + 1, Position, 3.0f, Light, pObject);

			Vector(0.0f, 2.0f, -8.0f, p);
			pModel->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[pModel->GetBoneIndex(17)], p, Position, false);
			Vector(0.25f, 0.0f, 0.0f, Light);
			CreateSprite(BITMAP_SPARK + 1, Position, 5.0f, Light, pObject);
			Vector(0.15f, 0.0f, 0.0f, Light);
			CreateSprite(BITMAP_SPARK + 1, Position, 3.0f, Light, pObject);
		}
		break;
	case 75:
		{
			Vector(0.0f, 0.0f, 0.0f, p);
			pModel->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[pModel->GetBoneIndex(4)], p, Position, false);
			mu_float fLumi = (EMath::Sin(FWorldTime*0.002f) + 1.0f) * 0.5f;
			Vector(fLumi, fLumi, fLumi, Light);
			Vector(1.0f, 1.0f, 1.0f, Light);
			CreateSprite(BITMAP_FLARE, Position, 3.0f, Light, pObject, (FWorldTime / 10.0f));
		}
		break;
	}

	return true;
}

mu_boolean M33Aida::RenderAidaObjectMesh(const mu_uint32 ThreadIndex, OBJECT* pObject, MUModel* pModel, mu_boolean ExtraMon)
{
	if (IsInAida())
	{
		switch (pObject->Type)
		{
		case 41:
			{
				pModel->BodyLight(ThreadIndex)[0] = 0.52f;
				pModel->BodyLight(ThreadIndex)[1] = 0.52f;
				pModel->BodyLight(ThreadIndex)[2] = 0.52f;

				pModel->StreamMesh(ThreadIndex) = 0;
				pModel->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV);
				pModel->StreamMesh(ThreadIndex) = -1;
			}
			return true;
		case 65:
		case 66:
			{
				pModel->StreamMesh(ThreadIndex) = 0;
				pModel->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, -IWorldTime % 100000 * 0.00005f, pObject->BlendMeshTexCoordV);
				pModel->StreamMesh(ThreadIndex) = -1;
			}
			return true;
		case 77:
		case 78:
			{
				pModel->StreamMesh(ThreadIndex) = 0;
				pModel->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, IWorldTime % 100000 * 0.0002f, pObject->BlendMeshTexCoordV);
				pModel->StreamMesh(ThreadIndex) = -1;
			}
			return true;
		}
	}

	return false;
}

CHARACTER* M33Aida::CreateAidaMonster(mu_int32 iType, mu_int32 PosX, mu_int32 PosY, mu_int32 Key)
{
	CHARACTER* pCharacter = nullptr;
	switch (iType)
	{
	case 304:
		{
			pCharacter = CreateCharacter(Key, MODEL_MONSTER01 + 100, PosX, PosY);
			pCharacter->Object.Scale = 1.4f;
			pCharacter->Weapon[0].Type = -1;
			pCharacter->Weapon[1].Type = -1;
		}
		break;
	case 305:
		{
			pCharacter = CreateCharacter(Key, MODEL_MONSTER01 + 101, PosX, PosY);
			pCharacter->Object.Scale = 1.35f;
			pCharacter->Weapon[0].Type = -1;
			pCharacter->Weapon[1].Type = -1;
		}
		break;
	case 306:
		{
			pCharacter = CreateCharacter(Key, MODEL_MONSTER01 + 102, PosX, PosY);
			pCharacter->Object.Scale = 1.1f;
			pCharacter->Weapon[0].Type = -1;
			pCharacter->Weapon[1].Type = -1;
		}
		break;
	case 307:
		{
			pCharacter = CreateCharacter(Key, MODEL_MONSTER01 + 103, PosX, PosY);
			pCharacter->Object.Scale = 1.15f;
			pCharacter->Weapon[0].Type = -1;
			pCharacter->Weapon[1].Type = -1;
		}
		break;
	case 308:
		{
			pCharacter = CreateCharacter(Key, MODEL_MONSTER01 + 104, PosX, PosY);
			pCharacter->Object.Scale = 1.2f;
			pCharacter->Weapon[0].Type = -1;
			pCharacter->Weapon[1].Type = -1;
		}
		break;
	case 309:
		{
			pCharacter = CreateCharacter(Key, MODEL_MONSTER01 + 105, PosX, PosY);
			pCharacter->Object.Scale = 1.8f;
			pCharacter->Weapon[0].Type = -1;
			pCharacter->Weapon[1].Type = -1;
		}
		break;
	case 549:
		{
			pCharacter = CreateCharacter(Key, MODEL_MONSTER01 + 193, PosX, PosY);
			pCharacter->Object.Scale = 1.35f;
			pCharacter->Weapon[0].Type = -1;
			pCharacter->Weapon[1].Type = -1;
		}
		break;
	case 550:
		{
			pCharacter = CreateCharacter(Key, MODEL_MONSTER01 + 194, PosX, PosY);
			pCharacter->Object.Scale = 1.2f;
			pCharacter->Weapon[0].Type = -1;
			pCharacter->Weapon[1].Type = -1;
		}
		break;
	case 551:
		{
			pCharacter = CreateCharacter(Key, MODEL_MONSTER01 + 195, PosX, PosY);
			pCharacter->Object.Scale = 1.40f;
			pCharacter->Weapon[0].Type = -1;
			pCharacter->Weapon[1].Type = -1;
		}
		break;
	case 552:
		{
			pCharacter = CreateCharacter(Key, MODEL_MONSTER01 + 196, PosX, PosY);
			pCharacter->Object.Scale = 1.75f;
			pCharacter->Weapon[0].Type = -1;
			pCharacter->Weapon[1].Type = -1;
		}
		break;
	}
	return pCharacter;
}

mu_boolean M33Aida::MoveAidaMonsterVisual(const mu_uint32 ThreadIndex, OBJECT* pObject, MUModel* pModel)
{
	switch (pObject->Type)
	{
	case MODEL_MONSTER01 + 100:
		{
			EVector3 Light;
			Vector(0.7f, 0.1f, 0.1f, Light);
			AddTerrainLight(pObject->Position[0], pObject->Position[1], Light, 3, PrimaryTerrainLight);
		}
		break;
	case MODEL_MONSTER01 + 101:
		{
			EVector3 Light;
			Vector(0.0f, 0.0f, 0.7f, Light);
			AddTerrainLight(pObject->Position[0], pObject->Position[1], Light, 3, PrimaryTerrainLight);
		}
		break;
	case MODEL_MONSTER01 + 105:
		{
			EVector3 Light;
			Vector(1.0f, 0.0f, 0.0f, Light);
			AddTerrainLight(pObject->Position[0], pObject->Position[1], Light, 3, PrimaryTerrainLight);
		}
		break;
	case MODEL_MONSTER01 + 195:
		{
			EVector3 Light;
			Vector(0.0f, 0.0f, 0.7f, Light);
			AddTerrainLight(pObject->Position[0], pObject->Position[1], Light, 3, PrimaryTerrainLight);
		}
		break;
	case MODEL_MONSTER01 + 196:
		{
			EVector3 Light;
			Vector(0.7f, 0.1f, 0.1f, Light);
			AddTerrainLight(pObject->Position[0], pObject->Position[1], Light, 3, PrimaryTerrainLight);
		}
		break;
	}
	return false;
}

void M33Aida::MoveAidaBlurEffect(const mu_uint32 ThreadIndex, CHARACTER* pCharacter, OBJECT* pObject, MUModel* pModel)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	switch (pObject->Type)
	{
	case MODEL_MONSTER01 + 102:
		{
			if (pObject->AnimationFrame <= 5.06f && (pObject->CurrentAction == MONSTER01_ATTACK1 || pObject->CurrentAction == MONSTER01_ATTACK2))
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

					Vector(20.0f, 0.0f, 0.0f, StartRelative);
					Vector(100.0f, 0.0f, 0.0f, EndRelative);

					pModel->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[pModel->GetBoneIndex(18)], StartRelative, StartPos, false);
					pModel->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[pModel->GetBoneIndex(20)], EndRelative, EndPos, false);
					CreateBlur(pCharacter, StartPos, EndPos, Light, 3, true, 23);

					pModel->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[pModel->GetBoneIndex(25)], StartRelative, StartPos, false);
					pModel->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[pModel->GetBoneIndex(26)], EndRelative, EndPos, false);
					CreateBlur(pCharacter, StartPos, EndPos, Light, 3, true, 24);

					fAnimationFrame += fSpeedPerFrame;
				}
			}
		}
		break;
	case MODEL_MONSTER01 + 103:
		{
			if (pObject->CurrentAction == MONSTER01_ATTACK1 || pObject->CurrentAction == MONSTER01_ATTACK2)
			{
				EVector3  Light;
				Vector(0.5f, 0.5f, 0.0f, Light);

				EVector3 StartPos, StartRelative;
				EVector3 EndPos, EndRelative;

				mu_float fActionSpeed = pModel->_AnimationHeader[pObject->CurrentAction].Speed;
				mu_float fSpeedPerFrame = fActionSpeed / 10.0;
				mu_float fAnimationFrame = pObject->AnimationFrame - fActionSpeed;
				for (mu_int32 i = 0; i < 10; ++i)
				{
					pModel->Animation(ThreadIndex, ThreadInfo.BoneTransform, fAnimationFrame, pObject->PriorAnimationFrame, pObject->PriorAction, pObject->Angle, pObject->HeadAngle);

					Vector(20.0f, 0.0f, 0.0f, StartRelative);
					Vector(60.0f, 0.0f, 0.0f, EndRelative);

					pModel->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[pModel->GetBoneIndex(35)], StartRelative, StartPos, false);
					pModel->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[pModel->GetBoneIndex(37)], EndRelative, EndPos, false);
					CreateBlur(pCharacter, StartPos, EndPos, Light, 3, true, 23);

					pModel->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[pModel->GetBoneIndex(39)], StartRelative, StartPos, false);
					pModel->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[pModel->GetBoneIndex(41)], EndRelative, EndPos, false);
					CreateBlur(pCharacter, StartPos, EndPos, Light, 3, true, 24);

					pModel->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[pModel->GetBoneIndex(43)], StartRelative, StartPos, false);
					pModel->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[pModel->GetBoneIndex(45)], EndRelative, EndPos, false);
					CreateBlur(pCharacter, StartPos, EndPos, Light, 3, true, 25);

					fAnimationFrame += fSpeedPerFrame;
				}
			}
		}
		break;
	case MODEL_MONSTER01 + 104:
		{
			if (pObject->CurrentAction == MONSTER01_ATTACK1)
			{
				EVector3  Light;
				Vector(0.3f, 0.3f, 0.3f, Light);

				EVector3 StartPos, StartRelative;
				EVector3 EndPos, EndRelative;

				mu_float fActionSpeed = pModel->_AnimationHeader[pObject->CurrentAction].Speed;
				mu_float fSpeedPerFrame = fActionSpeed / 10.0;
				mu_float fAnimationFrame = pObject->AnimationFrame - fActionSpeed;
				for (mu_int32 i = 0; i < 10; ++i)
				{
					pModel->Animation(ThreadIndex, ThreadInfo.BoneTransform, fAnimationFrame, pObject->PriorAnimationFrame, pObject->PriorAction, pObject->Angle, pObject->HeadAngle);

					Vector(0.0f, 0.0f, 20.0f, StartRelative);
					Vector(0.0f, 0.0f, 70.0f, EndRelative);

					pModel->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[pModel->GetBoneIndex(26)], StartRelative, StartPos, false);
					pModel->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[pModel->GetBoneIndex(27)], EndRelative, EndPos, false);
					CreateBlur(pCharacter, StartPos, EndPos, Light, 3, true, 24);

					fAnimationFrame += fSpeedPerFrame;
				}
			}
		}
		break;
	case MODEL_MONSTER01 + 193:
		{
			if (pObject->CurrentAction == MONSTER01_ATTACK1 || pObject->CurrentAction == MONSTER01_ATTACK2)
			{
				EVector3  Light;
				Vector(0.5f, 0.5f, 0.0f, Light);

				EVector3 StartPos, StartRelative;
				EVector3 EndPos, EndRelative;

				mu_float fActionSpeed = pModel->_AnimationHeader[pObject->CurrentAction].Speed;
				mu_float fSpeedPerFrame = fActionSpeed / 10.0;
				mu_float fAnimationFrame = pObject->AnimationFrame - fActionSpeed;
				for (mu_int32 i = 0; i < 10; ++i)
				{
					pModel->Animation(ThreadIndex, ThreadInfo.BoneTransform, fAnimationFrame, pObject->PriorAnimationFrame, pObject->PriorAction, pObject->Angle, pObject->HeadAngle);

					Vector(20.0f, 0.0f, 0.0f, StartRelative);
					Vector(60.0f, 0.0f, 0.0f, EndRelative);

					pModel->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[pModel->GetBoneIndex(35)], StartRelative, StartPos, false);
					pModel->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[pModel->GetBoneIndex(37)], EndRelative, EndPos, false);
					CreateBlur(pCharacter, StartPos, EndPos, Light, 3, true, 23);

					pModel->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[pModel->GetBoneIndex(39)], StartRelative, StartPos, false);
					pModel->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[pModel->GetBoneIndex(41)], EndRelative, EndPos, false);
					CreateBlur(pCharacter, StartPos, EndPos, Light, 3, true, 24);

					pModel->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[pModel->GetBoneIndex(43)], StartRelative, StartPos, false);
					pModel->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[pModel->GetBoneIndex(45)], EndRelative, EndPos, false);
					CreateBlur(pCharacter, StartPos, EndPos, Light, 3, true, 25);

					fAnimationFrame += fSpeedPerFrame;
				}
			}
		}
		break;
	case MODEL_MONSTER01 + 194:
		{
			if (pObject->AnimationFrame <= 5.06f && (pObject->CurrentAction == MONSTER01_ATTACK1 || pObject->CurrentAction == MONSTER01_ATTACK2))
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

					Vector(20.0f, 0.0f, 0.0f, StartRelative);
					Vector(100.0f, 0.0f, 0.0f, EndRelative);

					pModel->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[pModel->GetBoneIndex(18)], StartRelative, StartPos, false);
					pModel->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[pModel->GetBoneIndex(20)], EndRelative, EndPos, false);
					CreateBlur(pCharacter, StartPos, EndPos, Light, 3, true, 23);

					pModel->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[pModel->GetBoneIndex(25)], StartRelative, StartPos, false);
					pModel->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[pModel->GetBoneIndex(26)], EndRelative, EndPos, false);
					CreateBlur(pCharacter, StartPos, EndPos, Light, 3, true, 24);

					fAnimationFrame += fSpeedPerFrame;
				}
			}
		}
		break;
	}
}

mu_boolean M33Aida::RenderAidaMonsterVisual(const mu_uint32 ThreadIndex, CHARACTER* pCharacter, OBJECT* pObject, MUModel* pModel)
{
	switch (pObject->Type)
	{
	case MODEL_MONSTER01 + 100:
		{
			EVector3 Position, Light, Angle;
			mu_float Random_Light = (mu_float)(GetLargeRand(30)) / 100.0f + 0.6f;
			Vector(Random_Light + 0.5f, Random_Light - 0.05f, Random_Light + 0.5f, Light);

			BoneManager::GetBonePosition(pObject, 76, Position);
			CreateSprite(BITMAP_LIGHT + 1, Position, 0.8f, Light, pObject);
			BoneManager::GetBonePosition(pObject, 107, Position);
			CreateSprite(BITMAP_LIGHT + 1, Position, 1.1f, Light, pObject);
			BoneManager::GetBonePosition(pObject, 108, Position);
			CreateSprite(BITMAP_LIGHT + 1, Position, 1.0f, Light, pObject);
			BoneManager::GetBonePosition(pObject, 109, Position);
			CreateSprite(BITMAP_LIGHT + 1, Position, 1.7f, Light, pObject);
			BoneManager::GetBonePosition(pObject, 110, Position);
			CreateSprite(BITMAP_LIGHT + 1, Position, 0.8f, Light, pObject);

			Vector(0.7f, 0.5f, 0.7f, Light);
			EVector3 Relative = { 0.0f, 0.0f, -65.0f };
			BoneManager::GetBonePosition(pObject, 0, Relative, Position);
			CreateParticle(ThreadIndex, BITMAP_LIGHT + 1, Position, Angle, Light, 4, 4.0f);

			if (pObject->CurrentAction == MONSTER01_WALK || pObject->CurrentAction == MONSTER01_RUN)
			{
				if (GetLargeRand(15) == 0)
					PlayBackground(SOUND_AIDA_WITCHQUEEN_MOVE1 + GetLargeRand(2));
			}
			else if (pObject->CurrentAction == MONSTER01_ATTACK1)
			{
				if (pObject->SubType == 0)
				{
					pObject->SubType = 1;
					PlayBackground(SOUND_AIDA_WITCHQUEEN_ATTACK2);
				}
			}
			else if (pObject->CurrentAction == MONSTER01_ATTACK2)
			{
				if (pObject->SubType == 0)
				{
					pObject->SubType = 1;
					PlayBackground(SOUND_CHAOS_THUNDER01 + GetLargeRand(2));
					PlayBackground(SOUND_AIDA_WITCHQUEEN_ATTACK1);
				}
			}
			else if (pObject->CurrentAction == MONSTER01_DIE)
			{
				if (pObject->SubType == 0)
				{
					pObject->SubType = 1;
					PlayBackground(SOUND_AIDA_WITCHQUEEN_DIE);
				}
			}

			if (pObject->CurrentAction == MONSTER01_ATTACK1 || pObject->CurrentAction == MONSTER01_ATTACK2)
			{
				if (pObject->AnimationFrame >= 4.0f)
				{
					EVector3 Relative = { 70.0f, 0.0f, 0.0f };
					BoneManager::GetBonePosition(pObject, 0, Relative, Position);

					Vector(0.5f, 0.2f, 0.5f, Light);
					CreateParticle(ThreadIndex, BITMAP_SMOKE, Position, Angle, Light, 27, 2.0f);
					Vector(1.0f, 1.0f, 1.0f, Light);
					CreateParticle(ThreadIndex, BITMAP_LIGHT + 1, Position, Angle, Light, 2, 1.0f);
				}
				if (pObject->AnimationFrame >= 6.0f && pObject->CurrentAction == MONSTER01_ATTACK1)
				{
					EVector3 Position, Light, Angle = { 0.0f, 0.0f, 0.0f };
					EVector3 Relative = { 70.0f, 0.0f, 0.0f };
					BoneManager::GetBonePosition(pObject, 0, Relative, Position);
					Vector(1.0f, 1.0f, 1.0f, Light);
					CreateParticle(ThreadIndex, BITMAP_LIGHT + 1, Position, Angle, Light, 3, 1.3f);
				}
				if (pCharacter->TargetCharacter >= 0 && pCharacter->TargetCharacter < MAX_CHARACTERS_CLIENT)
				{
					CHARACTER *tc = &CharactersClient[pCharacter->TargetCharacter];
					OBJECT *to = &tc->Object;

					EVector3 vTemp;
					VectorCopy(to->Position, vTemp);
					vTemp[2] += 100.0f;

					if (pObject->AnimationFrame >= 6.0f && pObject->CurrentAction == MONSTER01_ATTACK1)
					{
						CreateParticle(ThreadIndex, BITMAP_LIGHT + 1, vTemp, Angle, Light, 3, 1.3f);
					}
					if (pObject->AnimationFrame >= 6.0f && pObject->CurrentAction == MONSTER01_ATTACK2)
					{
						CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, to->Position, vTemp, pObject->Angle, 16);
					}
				}
			}

			if (pObject->CurrentAction == MONSTER01_STOP1 || pObject->CurrentAction == MONSTER01_STOP2)
				pObject->SubType = 0;
		}
		break;
	case MODEL_MONSTER01 + 101:
		{
			EVector3 Position, Light, Angle = { 0.0f, 0.0f, 0.0f };
			Vector(1.0f, 1.0f, 1.0f, Light);

			BoneManager::GetBonePosition(pObject, 12, Position);
			CreateParticle(ThreadIndex, BITMAP_WATERFALL_2, Position, Angle, Light, 1);
			BoneManager::GetBonePosition(pObject, 20, Position);
			CreateParticle(ThreadIndex, BITMAP_WATERFALL_2, Position, Angle, Light, 1);
			BoneManager::GetBonePosition(pObject, 6, Position);
			CreateParticle(ThreadIndex, BITMAP_WATERFALL_2, Position, Angle, Light, 1);

			if (pObject->CurrentAction == MONSTER01_WALK || pObject->CurrentAction == MONSTER01_RUN)
			{
				if (GetLargeRand(15) == 0)
					PlayBackground(SOUND_AIDA_BLUEGOLEM_MOVE1 + GetLargeRand(2));
			}
			else if (pObject->CurrentAction == MONSTER01_ATTACK1)
			{
				if (pObject->SubType == 0)
				{
					pObject->SubType = 1;
					PlayBackground(SOUND_AIDA_BLUEGOLEM_ATTACK1);
				}
			}
			else if (pObject->CurrentAction == MONSTER01_ATTACK2)
			{
				if (pObject->SubType == 0)
				{
					pObject->SubType = 1;
					PlayBackground(SOUND_AIDA_BLUEGOLEM_ATTACK2);
				}
			}
			else if (pObject->CurrentAction == MONSTER01_DIE)
			{
				if (pObject->SubType == 0)
				{
					pObject->SubType = 1;
					PlayBackground(SOUND_AIDA_BLUEGOLEM_DIE);
				}
			}

			if (pObject->CurrentAction == MONSTER01_ATTACK1)
			{
				EVector3 Position, Light, Angle = { 0.0f, 0.0f, 0.0f };
				Vector(1.0f, 1.0f, 1.0f, Light);
				if (pObject->AnimationFrame >= 4.0f)
				{
					Vector(0.0f, 45.0f, 45.0f, Angle);
					EVector3 Relative = { 30.0f, 0.0f, 0.0f };
					BoneManager::GetBonePosition(pObject, 12, Relative, Position);
					CreateParticle(ThreadIndex, BITMAP_SMOKE, Position, Angle, Light, 25);

					if (pObject->AnimationFrame >= 5.0f && pObject->AnimationFrame <= 5.5f)
					{
						Vector(5.0f, 5.0f, 5.0f, Light);
						BoneManager::GetBonePosition(pObject, 12, Relative, Position);
						CreateParticle(ThreadIndex, BITMAP_SHOCK_WAVE, Position, Angle, Light, 3, 0.5f);
					}
				}

				if (pCharacter->TargetCharacter >= 0 && pCharacter->TargetCharacter < MAX_CHARACTERS_CLIENT && pObject->AnimationFrame >= 6.9f)
				{
					CHARACTER *tc = &CharactersClient[pCharacter->TargetCharacter];
					OBJECT *to = &tc->Object;

					EVector3 vTemp;
					VectorCopy(to->Position, vTemp);
					vTemp[2] += 100.0f;

					CreateJoint(ThreadIndex, BITMAP_JOINT_ENERGY, vTemp, pObject->Position, to->Angle, 16, pObject, 20.0f);
				}
			}

			if (pObject->CurrentAction == MONSTER01_STOP1 || pObject->CurrentAction == MONSTER01_STOP2)
				pObject->SubType = 0;
		}
		break;
	case MODEL_MONSTER01 + 102:
		{
			EVector3 Relative, Position, Light, Angle = { 0.0f, 0.0f, 0.0f };

			mu_float Random_Light = (mu_float)(GetLargeRand(10)) / 100.0f + 0.2f;
			Vector(60.0f, -30.0f, 0.0f, Relative);
			Vector(0.0f, Random_Light, 0.0f, Light);

			BoneManager::GetBonePosition(pObject, 6, Relative, Position);
			CreateSprite(BITMAP_LIGHT + 1, Position, 3.0f, Light, pObject);
			CreateParticle(ThreadIndex, BITMAP_SPARK + 1, Position, Angle, Light, 7);

			if (pObject->CurrentAction == MONSTER01_WALK || pObject->CurrentAction == MONSTER01_RUN)
			{
				if (GetLargeRand(15) == 0)
					PlayBackground(SOUND_AIDA_DEATHRAIDER_MOVE1 + GetLargeRand(2));
			}
			else if (pObject->CurrentAction == MONSTER01_ATTACK1)
			{
				if (pObject->SubType == 0)
				{
					pObject->SubType = 1;
					PlayBackground(SOUND_AIDA_DEATHRAIDER_ATTACK1);
				}
			}
			else if (pObject->CurrentAction == MONSTER01_ATTACK2)
			{
				if (pObject->SubType == 0)
				{
					pObject->SubType = 1;
					PlayBackground(SOUND_AIDA_DEATHRAIDER_ATTACK2);
				}
			}
			else if (pObject->CurrentAction == MONSTER01_DIE)
			{
				if (pObject->SubType == 0)
				{
					pObject->SubType = 1;
					PlayBackground(SOUND_AIDA_DEATHRAIDER_DIE);
				}
			}

			if (pObject->CurrentAction == MONSTER01_STOP1 || pObject->CurrentAction == MONSTER01_STOP2)
				pObject->SubType = 0;
		}
		break;

	case MODEL_MONSTER01 + 103:
		{
			if (pObject->CurrentAction == MONSTER01_WALK || pObject->CurrentAction == MONSTER01_RUN)
			{
				if (GetLargeRand(15) == 0)
					PlayBackground(SOUND_AIDA_FORESTORC_MOVE1 + GetLargeRand(2));
			}
			else if (pObject->CurrentAction == MONSTER01_ATTACK1)
			{
				if (pObject->SubType == 0)
				{
					pObject->SubType = 1;
					PlayBackground(SOUND_AIDA_FORESTORC_ATTACK1);
				}
			}
			else if (pObject->CurrentAction == MONSTER01_ATTACK2)
			{
				if (pObject->SubType == 0)
				{
					pObject->SubType = 1;
					PlayBackground(SOUND_AIDA_FORESTORC_ATTACK2);
				}
			}
			else if (pObject->CurrentAction == MONSTER01_DIE)
			{
				if (pObject->SubType == 0)
				{
					pObject->SubType = 1;
					PlayBackground(SOUND_AIDA_FORESTORC_DIE);
				}
			}

			if (pObject->CurrentAction == MONSTER01_STOP1 || pObject->CurrentAction == MONSTER01_STOP2)
				pObject->SubType = 0;
		}
		break;
	case MODEL_MONSTER01 + 104:
		{
			if (pObject->CurrentAction == MONSTER01_WALK || pObject->CurrentAction == MONSTER01_RUN)
			{
				if (GetLargeRand(15) == 0)
					PlayBackground(SOUND_AIDA_DEATHTREE_MOVE1 + GetLargeRand(2));
			}
			else if (pObject->CurrentAction == MONSTER01_ATTACK1)
			{
				if (pObject->SubType == 0)
				{
					pObject->SubType = 1;
					PlayBackground(SOUND_AIDA_DEATHTREE_ATTACK1);
				}
			}
			else if (pObject->CurrentAction == MONSTER01_ATTACK2)
			{
				if (pObject->SubType == 0)
				{
					pObject->SubType = 1;
					PlayBackground(SOUND_AIDA_DEATHTREE_ATTACK2);
				}
			}
			else if (pObject->CurrentAction == MONSTER01_DIE)
			{
				if (pObject->SubType == 0)
				{
					pObject->SubType = 1;
					PlayBackground(SOUND_AIDA_DEATHTREE_DIE);
				}
			}

			EVector3 Position, Light, Angle = { 0.0f, 0.0f, 0.0f };
			mu_float Random_Light;

			static mu_uint32 hornBones[6] = {37, 38, 39, 40, 44, 45};
			for (mu_int32 i = 0; i < 6; ++i)
			{
				Random_Light = (mu_float)(GetLargeRand(10)) / 100.0f + 0.8f;
				Vector(0.4f, Random_Light, 0.5f, Light);
				BoneManager::GetBonePosition(pObject, hornBones[i], Position);
				CreateSprite(BITMAP_LIGHT + 1, Position, 0.4f, Light, pObject);
				CreateParticle(ThreadIndex, BITMAP_SPARK + 1, Position, Angle, Light, 6);
			}
			if (pObject->CurrentAction == MONSTER01_ATTACK2)
			{
				EVector3 Position, Relative = { 0.0f, -100.0f, 20.0f }, Light = { 0.5f, 0.7f, 0.5f };
				BoneManager::GetBonePosition(pObject, 1, Relative, Position);
				CreateParticle(ThreadIndex, BITMAP_SMOKE, Position, pObject->Angle, Light, 26);
			}

			if (pObject->CurrentAction == MONSTER01_STOP1 || pObject->CurrentAction == MONSTER01_STOP2)
				pObject->SubType = 0;
		}
		break;
	case MODEL_MONSTER01 + 105:
		{
			EVector3 Relative, Position, Light, Angle = { 0.0f, 0.0f, 0.0f };
			mu_float Random_Light;

			Random_Light = (mu_float)(GetLargeRand(8)) / 10.0f + 0.6f;
			Vector(Random_Light, 0.0f, 0.0f, Light);

			BoneManager::GetBonePosition(pObject, 9, Position);
			CreateSprite(BITMAP_LIGHT + 1, Position, 0.3f, Light, pObject);

			BoneManager::GetBonePosition(pObject, 10, Position);
			CreateSprite(BITMAP_LIGHT + 1, Position, 0.3f, Light, pObject);

			BoneManager::GetBonePosition(pObject, 33, Position);
			CreateSprite(BITMAP_LIGHT + 1, Position, 1.0f, Light, pObject);

			BoneManager::GetBonePosition(pObject, 34, Position);
			CreateSprite(BITMAP_LIGHT + 1, Position, 1.0f, Light, pObject);

			BoneManager::GetBonePosition(pObject,35, Position);
			CreateSprite(BITMAP_LIGHT + 1, Position, 1.0f, Light, pObject);

			if (pObject->CurrentAction == MONSTER01_WALK || pObject->CurrentAction == MONSTER01_RUN)
			{
				if (GetLargeRand(15) == 0)
					PlayBackground(SOUND_AIDA_HELL_MOVE1 + GetLargeRand(2));
			}
			else if (pObject->CurrentAction == MONSTER01_ATTACK1)
			{
				if (pObject->SubType == 0)
				{
					pObject->SubType = 1;
					PlayBackground(SOUND_AIDA_HELL_ATTACK3);
				}
			}
			else if (pObject->CurrentAction == MONSTER01_ATTACK2)
			{
				if (pObject->SubType == 0)
				{
					pObject->SubType = 1;
					PlayBackground(SOUND_AIDA_HELL_ATTACK2);
				}
			}
			else if (pObject->CurrentAction == MONSTER01_ATTACK3)
			{
				if (pObject->SubType == 0)
				{
					pObject->SubType = 1;
					PlayBackground(SOUND_AIDA_HELL_ATTACK1);
				}
			}
			else if (pObject->CurrentAction == MONSTER01_DIE)
			{
				if (pObject->SubType == 0)
				{
					pObject->SubType = 1;
					PlayBackground(SOUND_AIDA_HELL_DIE);
				}
			}

			if (pObject->CurrentAction == MONSTER01_ATTACK2)
			{
				BoneManager::GetBonePosition(pObject, 20, Position);
				CreateParticle(ThreadIndex, BITMAP_TRUE_FIRE, Position, Angle, Light, 6, 5.0f);

				BoneManager::GetBonePosition(pObject, 39, Position);
				CreateParticle(ThreadIndex, BITMAP_TRUE_FIRE, Position, Angle, Light, 6, 5.0f);
			}
			else
			{
				BoneManager::GetBonePosition(pObject, 20, Position);
				CreateParticle(ThreadIndex, BITMAP_TRUE_FIRE, Position, Angle, Light, 6, 2.5f);

				Vector(0.0f, 0.0f, -10.0f, Relative);
				BoneManager::GetBonePosition(pObject, 39, Relative, Position);
				CreateParticle(ThreadIndex, BITMAP_TRUE_FIRE, Position, Angle, Light, 6, 2.1f);
			}

			if ((pObject->AnimationFrame >= 9.0f && pObject->AnimationFrame <= 10.0f) && pObject->CurrentAction == MONSTER01_ATTACK2)
			{
				CreateEffect(ThreadIndex, BITMAP_BOSS_LASER, Position, pObject->Angle, Light, 1);
			}

			if (pObject->CurrentAction == MONSTER01_STOP1 || pObject->CurrentAction == MONSTER01_STOP2)
				pObject->SubType = 0;
		}
		break;
	case MODEL_MONSTER01 + 193:
		{
			if (pObject->CurrentAction == MONSTER01_WALK || pObject->CurrentAction == MONSTER01_RUN)
			{
				if (GetLargeRand(15) == 0)
					PlayBackground(SOUND_AIDA_FORESTORC_MOVE1 + GetLargeRand(2));
			}
			else if (pObject->CurrentAction == MONSTER01_ATTACK1)
			{
				if (pObject->SubType == 0)
				{
					pObject->SubType = 1;
					PlayBackground(SOUND_AIDA_FORESTORC_ATTACK1);
				}
			}
			else if (pObject->CurrentAction == MONSTER01_ATTACK2)
			{
				if (pObject->SubType == 0)
				{
					pObject->SubType = 1;
					PlayBackground(SOUND_AIDA_FORESTORC_ATTACK2);
				}
			}
			else if (pObject->CurrentAction == MONSTER01_DIE)
			{
				if (pObject->SubType == 0)
				{
					pObject->SubType = 1;
					PlayBackground(SOUND_AIDA_FORESTORC_DIE);
				}
			}

			if (pObject->CurrentAction == MONSTER01_STOP1 || pObject->CurrentAction == MONSTER01_STOP2)
				pObject->SubType = 0;
		}
		break;
	case MODEL_MONSTER01 + 194:
		{
			EVector3 Relative, Position, Light, Angle = { 0.0f, 0.0f, 0.0f };

			mu_float Random_Light = (mu_float)(GetLargeRand(10)) / 100.0f + 0.2f;
			Vector(60.0f, -30.0f, 0.0f, Relative);
			Vector(0.0f, Random_Light, 0.0f, Light);

			BoneManager::GetBonePosition(pObject, 6, Relative, Position);
			CreateSprite(BITMAP_LIGHT + 1, Position, 3.0f, Light, pObject);
			CreateParticle(ThreadIndex, BITMAP_SPARK + 1, Position, Angle, Light, 7);

			if (pObject->CurrentAction == MONSTER01_WALK || pObject->CurrentAction == MONSTER01_RUN)
			{
				if (GetLargeRand(15) == 0)
					PlayBackground(SOUND_AIDA_DEATHRAIDER_MOVE1 + GetLargeRand(2));
			}
			else if (pObject->CurrentAction == MONSTER01_ATTACK1)
			{
				if (pObject->SubType == 0)
				{
					pObject->SubType = 1;
					PlayBackground(SOUND_AIDA_DEATHRAIDER_ATTACK1);
				}
			}
			else if (pObject->CurrentAction == MONSTER01_ATTACK2)
			{
				if (pObject->SubType == 0)
				{
					pObject->SubType = 1;
					PlayBackground(SOUND_AIDA_DEATHRAIDER_ATTACK2);
				}
			}
			else if (pObject->CurrentAction == MONSTER01_DIE)
			{
				if (pObject->SubType == 0)
				{
					pObject->SubType = 1;
					PlayBackground(SOUND_AIDA_DEATHRAIDER_DIE);
				}
			}

			if (pObject->CurrentAction == MONSTER01_STOP1 || pObject->CurrentAction == MONSTER01_STOP2)
				pObject->SubType = 0;
		}
		break;

	case MODEL_MONSTER01 + 195:
		{
			EVector3 Position, Light, Angle = { 0.0f, 0.0f, 0.0f };
			Vector(1.0f, 1.0f, 1.0f, Light);

			BoneManager::GetBonePosition(pObject, 12, Position);
			CreateParticle(ThreadIndex, BITMAP_WATERFALL_2, Position, Angle, Light, 1);
			BoneManager::GetBonePosition(pObject, 20, Position);
			CreateParticle(ThreadIndex, BITMAP_WATERFALL_2, Position, Angle, Light, 1);
			BoneManager::GetBonePosition(pObject, 6, Position);
			CreateParticle(ThreadIndex, BITMAP_WATERFALL_2, Position, Angle, Light, 1);

			if (pObject->CurrentAction == MONSTER01_WALK || pObject->CurrentAction == MONSTER01_RUN)
			{
				if (GetLargeRand(15) == 0)
					PlayBackground(SOUND_AIDA_BLUEGOLEM_MOVE1 + GetLargeRand(2));
			}
			else if (pObject->CurrentAction == MONSTER01_ATTACK1)
			{
				if (pObject->SubType == 0)
				{
					pObject->SubType = 1;
					PlayBackground(SOUND_AIDA_BLUEGOLEM_ATTACK1);
				}
			}
			else if (pObject->CurrentAction == MONSTER01_ATTACK2)
			{
				if (pObject->SubType == 0)
				{
					pObject->SubType = 1;
					PlayBackground(SOUND_AIDA_BLUEGOLEM_ATTACK2);
				}
			}
			else if (pObject->CurrentAction == MONSTER01_DIE)
			{
				if (pObject->SubType == 0)
				{
					pObject->SubType = 1;
					PlayBackground(SOUND_AIDA_BLUEGOLEM_DIE);
				}
			}

			if (pObject->CurrentAction == MONSTER01_ATTACK1)
			{
				EVector3 Position, Light, Angle = { 0.0f, 0.0f, 0.0f };
				Vector(1.0f, 1.0f, 1.0f, Light);
				if (pObject->AnimationFrame >= 4.0f)
				{
					Vector(0.0f, 45.0f, 45.0f, Angle);
					EVector3 Relative = { 30.0f, 0.0f, 0.0f };
					BoneManager::GetBonePosition(pObject, 12, Relative, Position);
					CreateParticle(ThreadIndex, BITMAP_SMOKE, Position, Angle, Light, 25);

					if (pObject->AnimationFrame >= 5.0f && pObject->AnimationFrame <= 5.5f)
					{
						Vector(5.0f, 5.0f, 5.0f, Light);
						BoneManager::GetBonePosition(pObject, 12, Relative, Position);
						CreateParticle(ThreadIndex, BITMAP_SHOCK_WAVE, Position, Angle, Light, 3, 0.5f);
					}
				}

				if (pCharacter->TargetCharacter >= 0 && pCharacter->TargetCharacter < MAX_CHARACTERS_CLIENT && pObject->AnimationFrame >= 6.9f)
				{
					CHARACTER *tc = &CharactersClient[pCharacter->TargetCharacter];
					OBJECT *to = &tc->Object;

					EVector3 vTemp;
					VectorCopy(to->Position, vTemp);
					vTemp[2] += 100.0f;

					CreateJoint(ThreadIndex, BITMAP_JOINT_ENERGY, vTemp, pObject->Position, to->Angle, 16, pObject, 20.0f);
				}
			}

			if (pObject->CurrentAction == MONSTER01_STOP1 || pObject->CurrentAction == MONSTER01_STOP2)
				pObject->SubType = 0;
		}
		break;
	case MODEL_MONSTER01 + 196:
		{
			EVector3 Position, Light, Angle;
			mu_float Random_Light = (mu_float)(GetLargeRand(30)) / 100.0f + 0.6f;
			Vector(Random_Light + 0.5f, Random_Light - 0.05f, Random_Light + 0.5f, Light);

			BoneManager::GetBonePosition(pObject, 76, Position);
			CreateSprite(BITMAP_LIGHT + 1, Position, 0.8f, Light, pObject);
			BoneManager::GetBonePosition(pObject, 107, Position);
			CreateSprite(BITMAP_LIGHT + 1, Position, 1.1f, Light, pObject);
			BoneManager::GetBonePosition(pObject, 108, Position);
			CreateSprite(BITMAP_LIGHT + 1, Position, 1.0f, Light, pObject);
			BoneManager::GetBonePosition(pObject, 109, Position);
			CreateSprite(BITMAP_LIGHT + 1, Position, 1.7f, Light, pObject);
			BoneManager::GetBonePosition(pObject, 110, Position);
			CreateSprite(BITMAP_LIGHT + 1, Position, 0.8f, Light, pObject);

			Vector(0.7f, 0.5f, 0.7f, Light);
			EVector3 Relative = { 0.0f, 0.0f, -65.0f };
			BoneManager::GetBonePosition(pObject, 0, Relative, Position);
			CreateParticle(ThreadIndex, BITMAP_LIGHT + 1, Position, Angle, Light, 4, 4.0f);

			if (pObject->CurrentAction == MONSTER01_WALK || pObject->CurrentAction == MONSTER01_RUN)
			{
				if (GetLargeRand(15) == 0)
					PlayBackground(SOUND_AIDA_WITCHQUEEN_MOVE1 + GetLargeRand(2));
			}
			else if (pObject->CurrentAction == MONSTER01_ATTACK1)
			{
				if (pObject->SubType == 0)
				{
					pObject->SubType = 1;
					PlayBackground(SOUND_AIDA_WITCHQUEEN_ATTACK2);
				}
			}
			else if (pObject->CurrentAction == MONSTER01_ATTACK2)
			{
				if (pObject->SubType == 0)
				{
					pObject->SubType = 1;
					PlayBackground(SOUND_CHAOS_THUNDER01 + GetLargeRand(2));
					PlayBackground(SOUND_AIDA_WITCHQUEEN_ATTACK1);
				}
			}
			else if (pObject->CurrentAction == MONSTER01_DIE)
			{
				if (pObject->SubType == 0)
				{
					pObject->SubType = 1;
					PlayBackground(SOUND_AIDA_WITCHQUEEN_DIE);
				}
			}

			if (pObject->CurrentAction == MONSTER01_ATTACK1 || pObject->CurrentAction == MONSTER01_ATTACK2)
			{
				if (pObject->AnimationFrame >= 4.0f)
				{
					EVector3 Relative = { 70.0f, 0.0f, 0.0f };
					BoneManager::GetBonePosition(pObject, 0, Relative, Position);

					Vector(0.5f, 0.2f, 0.5f, Light);
					CreateParticle(ThreadIndex, BITMAP_SMOKE, Position, Angle, Light, 27, 2.0f);
					Vector(1.0f, 1.0f, 1.0f, Light);
					CreateParticle(ThreadIndex, BITMAP_LIGHT + 1, Position, Angle, Light, 2, 1.0f);
				}
				if (pObject->AnimationFrame >= 6.0f && pObject->CurrentAction == MONSTER01_ATTACK1)
				{
					EVector3 Position, Light, Angle = { 0.0f, 0.0f, 0.0f };
					EVector3 Relative = { 70.0f, 0.0f, 0.0f };
					BoneManager::GetBonePosition(pObject, 0, Relative, Position);
					Vector(1.0f, 1.0f, 1.0f, Light);
					CreateParticle(ThreadIndex, BITMAP_LIGHT + 1, Position, Angle, Light, 3, 1.3f);
				}
				if (pCharacter->TargetCharacter >= 0 && pCharacter->TargetCharacter < MAX_CHARACTERS_CLIENT)
				{
					CHARACTER *tc = &CharactersClient[pCharacter->TargetCharacter];
					OBJECT *to = &tc->Object;

					EVector3 vTemp;
					VectorCopy(to->Position, vTemp);
					vTemp[2] += 100.0f;

					if (pObject->AnimationFrame >= 6.0f && pObject->CurrentAction == MONSTER01_ATTACK1)
					{
						CreateParticle(ThreadIndex, BITMAP_LIGHT + 1, vTemp, Angle, Light, 3, 1.3f);
					}
					if (pObject->AnimationFrame >= 6.0f && pObject->CurrentAction == MONSTER01_ATTACK2)
					{
						CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, to->Position, vTemp, pObject->Angle, 16);
					}
				}
			}

			if (pObject->CurrentAction == MONSTER01_STOP1 || pObject->CurrentAction == MONSTER01_STOP2)
				pObject->SubType = 0;
		}
		break;
	}
	return false;
}

mu_boolean M33Aida::RenderAidaMonsterObjectMesh(const mu_uint32 ThreadIndex, OBJECT* pObject, MUModel* pModel, mu_boolean ExtraMon)
{
	switch (pObject->Type)
	{
	case MODEL_MONSTER01 + 101:
		{
			pModel->BodyLight(ThreadIndex)[0] = 0.9f;
			pModel->BodyLight(ThreadIndex)[1] = 0.9f;
			pModel->BodyLight(ThreadIndex)[2] = 0.9f;

			pModel->StreamMesh(ThreadIndex) = 0;
			pModel->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, -IWorldTime % 10000 * 0.0003f, -IWorldTime % 10000 * 0.0003f);
			pModel->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT | RENDER_CHROME, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV);
			pModel->StreamMesh(ThreadIndex) = -1;
			pModel->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV);

			return true;
		}
		break;
	case MODEL_MONSTER01 + 102:
		{
			pModel->BodyLight(ThreadIndex)[0] = 0.9f;
			pModel->BodyLight(ThreadIndex)[1] = 0.9f;
			pModel->BodyLight(ThreadIndex)[2] = 0.9f;

			pModel->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV);
			pModel->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV);
			pModel->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV);
			pModel->StreamMesh(ThreadIndex) = 0;
			pModel->RenderMesh(ThreadIndex, 3, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, -IWorldTime % 10000 * 0.0006f, -IWorldTime % 10000 * 0.0006f);
			pModel->RenderMesh(ThreadIndex, 3, RENDER_TEXTURE | RENDER_BRIGHT | RENDER_CHROME, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV);
			pModel->StreamMesh(ThreadIndex) = -1;

			return true;
		}
		break;
	case MODEL_MONSTER01 + 105:
		{
			pModel->BodyLight(ThreadIndex)[0] = 1.0f;
			pModel->BodyLight(ThreadIndex)[1] = 1.0f;
			pModel->BodyLight(ThreadIndex)[2] = 1.0f;

			pModel->StreamMesh(ThreadIndex) = 0;
			pModel->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV);
			pModel->BodyLight(ThreadIndex)[0] = 1.0f;
			pModel->BodyLight(ThreadIndex)[1] = 0.0f;
			pModel->BodyLight(ThreadIndex)[2] = 0.0f;
			pModel->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT | RENDER_CHROME, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV);
			pModel->StreamMesh(ThreadIndex) = -1;

			pModel->BodyLight(ThreadIndex)[0] = 1.0f;
			pModel->BodyLight(ThreadIndex)[1] = 1.0f;
			pModel->BodyLight(ThreadIndex)[2] = 1.0f;
			pModel->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV);

			pModel->StreamMesh(ThreadIndex) = 1;
			pModel->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV);
			pModel->BodyLight(ThreadIndex)[0] = 0.5f;
			pModel->BodyLight(ThreadIndex)[1] = 0.0f;
			pModel->BodyLight(ThreadIndex)[2] = 0.0f;
			pModel->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE | RENDER_BRIGHT | RENDER_CHROME, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV);
			pModel->StreamMesh(ThreadIndex) = -1;

			return true;
		}
		break;
	case MODEL_MONSTER01 + 194:
		{
			pModel->BodyLight(ThreadIndex)[0] = 0.9f;
			pModel->BodyLight(ThreadIndex)[1] = 0.9f;
			pModel->BodyLight(ThreadIndex)[2] = 0.9f;

			pModel->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV);
			pModel->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV);
			pModel->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV);
			pModel->StreamMesh(ThreadIndex) = 0;
			pModel->RenderMesh(ThreadIndex, 3, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, -IWorldTime % 10000 * 0.0006f, -IWorldTime % 10000 * 0.0006f);
			pModel->RenderMesh(ThreadIndex, 3, RENDER_TEXTURE | RENDER_BRIGHT | RENDER_CHROME, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV);
			pModel->StreamMesh(ThreadIndex) = -1;

			return true;
		}
		break;
	case MODEL_MONSTER01 + 195:
		{
			pModel->BodyLight(ThreadIndex)[0] = 0.9f;
			pModel->BodyLight(ThreadIndex)[1] = 0.9f;
			pModel->BodyLight(ThreadIndex)[2] = 0.9f;

			pModel->StreamMesh(ThreadIndex) = 0;
			pModel->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, -IWorldTime % 10000 * 0.0003f, -IWorldTime % 10000 * 0.0003f);
			pModel->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT | RENDER_CHROME, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV);
			pModel->StreamMesh(ThreadIndex) = -1;
			pModel->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, pObject->Alpha, pObject->BlendMesh, pObject->BlendMeshLight, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV);

			return true;
		}
		break;
	}
	return false;
}

mu_boolean M33Aida::AttackEffectAidaMonster(const mu_uint32 ThreadIndex, CHARACTER* pCharacter, OBJECT* pObject, MUModel* pModel)
{
	switch (pCharacter->MonsterIndex)
	{
	case 304:
		{
			if (pCharacter->AttackTime == 10 && pObject->CurrentAction == MONSTER01_ATTACK2)
			{
				EVector3 Light;
				Vector(1.0f, 1.0f, 1.0f, Light);
				CreateEffect(ThreadIndex, BITMAP_JOINT_FORCE, pObject->Position, pObject->Angle, Light, 1);
			}
		}
		return true;
	case 308:
		{
			if (pCharacter->AttackTime == 10 && pObject->CurrentAction == MONSTER01_ATTACK2)
			{
				EVector3 Light;
				Vector(1.0f, 1.0f, 1.0f, Light);
				CreateEffect(ThreadIndex, MODEL_TREE_ATTACK, pObject->Position, pObject->Angle, Light);
			}
		}
		return true;
	case 309:
		{
			for (mu_int32 i = 0; i < 5; ++i)
			{
				if (pCharacter->AttackTime == 10 && pObject->CurrentAction == MONSTER01_ATTACK1)
				{
					EVector3 Light;
					Vector(1.0f, 1.0f, 1.0f, Light);
					CreateEffect(ThreadIndex, MODEL_STORM, pObject->Position, pObject->Angle, Light, 3 + i);
				}
			}
		}
		return true;
	case 552:
		{
			if (pCharacter->AttackTime == 10 && pObject->CurrentAction == MONSTER01_ATTACK2)
			{
				EVector3 Light;
				Vector(1.0f, 1.0f, 1.0f, Light);
				CreateEffect(ThreadIndex, BITMAP_JOINT_FORCE, pObject->Position, pObject->Angle, Light, 1);
			}
		}
		return true;
	}
	return false;
}

mu_boolean M33Aida::SetCurrentActionAidaMonster(CHARACTER* pCharacter, OBJECT* pObject)
{
	switch (pCharacter->MonsterIndex)
	{
	case 304:
	case 305:
	case 309:
		return CheckMonsterSkill(pCharacter, pObject);
	case 549:
	case 550:
		return CheckMonsterSkill(pCharacter, pObject);
	case 551:
		{
			if (pCharacter->MonsterSkill == ATMON_SKILL_EX_BLOODYGOLUEM_ATTACKSKILL)
			{
				SetAction(pObject, MONSTER01_ATTACK2);
				pCharacter->MonsterSkill = -1;
			}
			else
			{
				SetAction(pObject, MONSTER01_ATTACK1);
				pCharacter->MonsterSkill = -1;
			}
			return true;
		}
		return true;
	case 552:
		{
			if (pCharacter->MonsterSkill == ATMON_SKILL_EX_BLOODYWITCHQUEEN_ATTACKSKILL)
			{
				SetAction(pObject, MONSTER01_ATTACK2);
				pCharacter->MonsterSkill = -1;
			}
			else
			{
				SetAction(pObject, MONSTER01_ATTACK1);
				pCharacter->MonsterSkill = -1;
			}
			return true;
		}
		return true;
	}

	return false;
}

void M33Aida::PlayBGM()
{
	if (GET_WORLD == WD_33AIDA)
	{
		EMusicBackend::Play(MUSICS::Data[MUSIC_BC_ADIA]);
	}
	else
	{
		EMusicBackend::Stop(MUSICS::Data[MUSIC_BC_ADIA]);
	}
}