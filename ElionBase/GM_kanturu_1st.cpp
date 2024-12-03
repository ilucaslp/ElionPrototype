#include "stdafx.h"
#include "GM_Kanturu_1st.h"

using namespace M37Kanturu1st;

mu_boolean M37Kanturu1st::MoveKanturu1stObject(const mu_uint32 ThreadIndex, OBJECT* pObject)
{
	EVector3 Light;
	mu_float Luminosity;

	switch (pObject->Type)
	{
	case 59:
	case 62:
	case 81:
	case 82:
	case 83:
	case 107:
	case 108:
		pObject->HiddenMesh = -2;
		break;
	case 44:
		pObject->Velocity = 0.02f;
		break;
	case 46:
		pObject->Velocity = 0.01f;
		pObject->BlendMeshLight = (mu_float)EMath::Sin(FWorldTime * 0.0015f) * 0.8f + 1.0f;
		PlayBackground(SOUND_KANTURU_1ST_BG_WHEEL);
		break;
	case 60:
		Luminosity = (mu_float)(GetLargeRand(4) + 3) * 0.1f;
		Vector(Luminosity * 0.9f, Luminosity * 0.2f, Luminosity * 0.1f, Light);
		AddTerrainLight(pObject->Position[0], pObject->Position[1], Light, 3,
			PrimaryTerrainLight);
		pObject->HiddenMesh = -2;
		break;
	case 61:
		Luminosity = (mu_float)(GetLargeRand(4) + 3) * 0.1f;
		Vector(Luminosity, Luminosity * 0.6f, Luminosity * 0.2f, Light);
		AddTerrainLight(pObject->Position[0], pObject->Position[1], Light, 3,
			PrimaryTerrainLight);
		pObject->HiddenMesh = -2;
		break;
	case 70:
		pObject->Velocity = 0.04f;
		Luminosity = (mu_float)EMath::Sin(FWorldTime * 0.002f) * 0.45f + 0.55f;
		Vector(Luminosity * 1.4f, Luminosity * 0.7f, Luminosity * 0.4f, Light);
		AddTerrainLight(pObject->Position[0], pObject->Position[1], Light, 4,
			PrimaryTerrainLight);
		break;
	case 76:
		pObject->Alpha = 0.5f;
		break;
	case 77:
		pObject->BlendMeshTexCoordV = -IWorldTime % 10000 * 0.0002f;
		PlayBackground(SOUND_KANTURU_1ST_BG_WATERFALL);
		break;
	case 90:
		pObject->Velocity = 0.04f;
		break;
	case 92:
		PlayBackground(SOUND_KANTURU_1ST_BG_ELEC);
		break;
	case 96:
		pObject->Alpha = 0.5f;
		break;
	case 97:
		pObject->HiddenMesh = -2;
		pObject->Timer += 0.1f;
		if (pObject->Timer > 10.0f)
			pObject->Timer = 0.0f;
		if (pObject->Timer > 5.0f)
			CreateParticle(ThreadIndex, BITMAP_BUBBLE, pObject->Position, pObject->Angle,
				pObject->Light, 5);
		break;
	case 98:
		PlayBackground(SOUND_KANTURU_1ST_BG_PLANT);
		break;
	case 102:
		pObject->BlendMeshLight = (mu_float)EMath::Sin(FWorldTime * 0.0010f) + 1.0f;
		break;
	}

	PlayBackground(SOUND_KANTURU_1ST_BG_GLOBAL);

	return true;
}

mu_boolean M37Kanturu1st::RenderKanturu1stObjectVisual(const mu_uint32 ThreadIndex, OBJECT* pObject, MUModel* pModel)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	EVector3 Light, p, Position;

	switch (pObject->Type)
	{
	case 37:
		{
			mu_int32 time = g_CurrentTime % 1024;
			if (time >= 0 && time < 10)
			{
				Vector(1.0f, 1.0f, 1.0f, Light);
				CreateEffect(ThreadIndex, MODEL_BUTTERFLY01, pObject->Position,
					pObject->Angle, Light, 3, pObject);
			}
			pObject->HiddenMesh = -2;
		}
		break;
	case 59:
		if (GetLargeRand(3) == 0)
		{
			Vector(1.0f, 1.0f, 1.0f, Light);
			CreateParticle(ThreadIndex, BITMAP_SMOKE, pObject->Position, pObject->Angle,
				Light, 21, pObject->Scale);
		}
		break;
	case 61:
		if (GetLargeRand(3) == 0)
		{
			Vector(1.0f, 1.0f, 1.0f, Light);
			CreateParticle(ThreadIndex, BITMAP_TRUE_FIRE, pObject->Position, pObject->Angle,
				Light, 0, pObject->Scale);
		}
		break;
	case 62:
		if (pObject->HiddenMesh != -2)
		{
			Vector(0.04f, 0.04f, 0.04f, Light);
			for (mu_int32 i = 0; i < 20; ++i)
				CreateParticle(ThreadIndex, BITMAP_CLOUD, pObject->Position, pObject->Angle,
					Light, 20, pObject->Scale, pObject);
		}
		pObject->HiddenMesh = -2;
		break;
	case 70:
		mu_float Luminosity;
		Vector(0.0f, 0.0f, 0.0f, p);
		pModel->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[pModel->GetBoneIndex(6)], p, Position, false);
		Luminosity = (mu_float)EMath::Sin(FWorldTime * 0.002f) + 1.8f;
		Vector(0.8f, 0.4f, 0.2f, Light);
		CreateSprite(
			BITMAP_SPARK + 1, Position, Luminosity * 7.0f, Light, pObject);
		Vector(0.65f, 0.65f, 0.65f, Light);
		CreateSprite(
			BITMAP_SPARK + 1, Position, Luminosity * 4.0f, Light, pObject);
		break;
	case 81:
		Vector(1.0f, 1.0f, 1.0f, Light);
		if (GetLargeRand(2) == 0)
			CreateParticle(ThreadIndex, BITMAP_WATERFALL_1, pObject->Position,
				pObject->Angle, Light, 2, pObject->Scale);
		break;
	case 82:
		Vector(1.0f, 1.0f, 1.0f, Light);
		CreateParticle(ThreadIndex, BITMAP_WATERFALL_3, pObject->Position,
			pObject->Angle, Light, 4, pObject->Scale);
		break;
	case 83:
		Vector(1.0f, 1.0f, 1.0f, Light);
		if (GetLargeRand(3) == 0)
			CreateParticle(ThreadIndex, BITMAP_WATERFALL_2, pObject->Position,
				pObject->Angle, Light, 2, pObject->Scale);
		break;
	case 92:
		{
			EVector3 EndRelative, StartPos, EndPos;
			Vector(0.0f, 0.0f, 0.0f, EndRelative);
			pModel->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[pModel->GetBoneIndex(1)], EndRelative, StartPos, false);
			pModel->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[pModel->GetBoneIndex(2)], EndRelative, EndPos, false);
			if (GetLargeRand(30) == 1)
				CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, StartPos, EndPos,
					pObject->Angle, 18, nullptr, 50.0f);
		}
		break;
	case 98:
		if (GetLargeRand(3) == 0)
		{
			EVector3 vPos;
			Vector(0.0f, 0.0f, 0.0f, vPos);
			pModel->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[pModel->GetBoneIndex(1)], vPos, Position, false);
			Vector(0.5f, 0.6f, 0.1f, Light);
			CreateParticle(ThreadIndex, BITMAP_TWINTAIL_WATER, Position, pObject->Angle, Light, 2);
		}
		break;
	case 105:
		{
			mu_float fLumi = (EMath::Sin(FWorldTime * 0.001f) + 1.0f) * 0.5f;
			EVector3 vPos;
			Vector(0.0f, 0.0f, 0.0f, vPos);
			Vector(fLumi, fLumi, fLumi, Light);
			pModel->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[pModel->GetBoneIndex(4)], vPos, Position, false);
			CreateParticle(ThreadIndex, BITMAP_ENERGY, Position, pObject->Angle, Light, 0, 1.5f);
			CreateSprite(BITMAP_SPARK + 1, Position, 10.0f, Light, pObject);
			EVector3 StartPos, EndPos;
			VectorCopy(Position, StartPos);
			VectorCopy(Position, EndPos);
			StartPos[0] -= 50.0f;
			StartPos[0] -= GetLargeRand(100);
			EndPos[0] += GetLargeRand(80);
			StartPos[1] -= GetLargeRand(50);
			EndPos[1] += GetLargeRand(50);
			StartPos[2] += 10.0f;
			EndPos[2] += 10.0f;
			if (GetLargeRand(20) == 0)
				CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, StartPos, EndPos,
					pObject->Angle, 8, nullptr, 40.0f);
		}
		break;
	case 107:
		if (pObject->HiddenMesh != -2)
		{
			EVector3 Light;
			Vector(0.06f, 0.06f, 0.06f, Light);
			for (mu_int32 i = 0; i < 20; ++i)
				CreateParticle(ThreadIndex, BITMAP_CLOUD, pObject->Position, pObject->Angle,
					Light, 2, pObject->Scale, pObject);
		}
		pObject->HiddenMesh = -2;
		break;
	case 108:
		if (pObject->HiddenMesh != -2)
		{
			EVector3 Light;
			Vector(0.2f, 0.2f, 0.2f, Light);
			for (mu_int32 i = 0; i < 20; ++i)
				CreateParticle(ThreadIndex, BITMAP_CLOUD, pObject->Position, pObject->Angle,
					Light, 7, pObject->Scale, pObject);
		}
		pObject->HiddenMesh = -2;
		break;
	case 110:
		{
			mu_float fLumi = (EMath::Sin(FWorldTime * 0.002f) + 1.5f) * 0.5f;
			Vector(fLumi * 0.6f, fLumi * 1.0f, fLumi * 0.8f, Light);
			EVector3 vPos;
			Vector(0.0f, 0.0f, 0.0f, vPos);
			pModel->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[pModel->GetBoneIndex(1)], vPos, Position, false);
			CreateSprite(BITMAP_LIGHT, Position, 1.1f, Light, pObject);
		}
		break;
	}

	return true;
}

mu_boolean M37Kanturu1st::RenderKanturu1stObjectMesh(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b, mu_boolean ExtraMon)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	mu_float tmpAlpha = o->Alpha;
	o->Alpha = 1.0f;

	mu_boolean isSuccess = false;
	switch (o->Type)
	{
	case 103:
		{
			mu_float fLumi = (EMath::Sin(FWorldTime * 0.002f) + 1.0f) * 0.5f;
			o->HiddenMesh = 1;
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh,
				o->BlendMeshLight, o->BlendMeshTexCoordU,
				o->BlendMeshTexCoordV, o->HiddenMesh);
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, 1, fLumi,
				o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			isSuccess = true;
		}
		break;

	case 76:
		{
			b->BodyLight(ThreadIndex) = EVector3(0.52f, 0.52f, 0.52f);

			b->StreamMesh(ThreadIndex) = 0;
			b->RenderMesh(ThreadIndex,
				0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh,
				o->BlendMeshLight, -IWorldTime % 100000 * 0.00001f,
				o->BlendMeshTexCoordV);
			b->StreamMesh(ThreadIndex) = -1;
			isSuccess = true;
		}
		break;

	case 95:
		{
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha,
				o->BlendMesh, o->BlendMeshLight,
				o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 0, RENDER_CHROME7 | RENDER_DARK, o->Alpha,
				o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU,
				o->BlendMeshTexCoordV, BITMAP_CHROME);

			if(MU_UPDATECOUNTER > 0)
			{
				EVector3 Light, p, Position;
				for (mu_int32 i = 0; i < 10; ++i)
				{
					Vector(0.0f, 0.0f, 0.0f, p);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(i)], p, Position, false);
					Vector(0.1f, 0.1f, 0.3f, Light);
					CreateSprite(BITMAP_SPARK + 1, Position, 7.5f, Light, o);
				}
			}
			isSuccess = true;
		}
		break;

	case 105:
		{
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh,
				o->BlendMeshLight, o->BlendMeshTexCoordU,
				o->BlendMeshTexCoordV, o->HiddenMesh);
			b->RenderMesh(ThreadIndex, 1, RENDER_CHROME | RENDER_BRIGHT, o->Alpha,
				o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU,
				o->BlendMeshTexCoordV, BITMAP_CHROME);
			isSuccess = true;
		}
		break;

	case 77:
	case 91:
	case 92:
		{
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh,
				o->BlendMeshLight, o->BlendMeshTexCoordU,
				o->BlendMeshTexCoordV, o->HiddenMesh);
			isSuccess = true;
		}
		break;

	case 85:
		{
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			b->RenderMesh(ThreadIndex, 3, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha,
				o->BlendMesh, o->BlendMeshLight,
				o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 3, RENDER_CHROME | RENDER_DARK, o->Alpha,
				o->BlendMesh, o->BlendMeshLight,
				o->BlendMeshTexCoordU, o->BlendMeshTexCoordV,
				BITMAP_CHROME);
			isSuccess = true;
		}
		break;
	case 96:
		{
			b->StreamMesh(ThreadIndex) = 0;
			ThreadInfo.MinAlpha = 0.0f;
			b->RenderMesh(ThreadIndex,
				0, RENDER_TEXTURE, 1.0f, o->BlendMesh,
				o->BlendMeshLight, o->BlendMeshTexCoordU,
				-IWorldTime % 20000 * 0.00005f);
			ThreadInfo.MinAlpha = 0.25f;
			b->StreamMesh(ThreadIndex) = -1;
			isSuccess = true;
		}
		break;
	}

	o->Alpha = tmpAlpha;

	return isSuccess;
}

CHARACTER* M37Kanturu1st::CreateKanturu1stMonster(const mu_uint32 ThreadIndex, mu_int32 iType, mu_int32 PosX, mu_int32 PosY, mu_int32 Key)
{
	CHARACTER* pCharacter = nullptr;

	switch(iType)
	{
	case 435:
	case 350:
		{
		    pCharacter = CreateCharacter(Key, MODEL_MONSTER01+106, PosX, PosY);
		    pCharacter->Object.Scale = 0.95f;
		    pCharacter->Weapon[0].Type = -1;
		    pCharacter->Weapon[1].Type = -1;
		}
		break;
	case 434:
	case 356:
		{
		    pCharacter = CreateCharacter(Key, MODEL_MONSTER01+112, PosX, PosY);
		    pCharacter->Object.Scale = 1.2f;
		    pCharacter->Weapon[0].Type = -1;
		    pCharacter->Weapon[1].Type = -1;
		}
		break;
	case 357:
		{
		    pCharacter = CreateCharacter(Key, MODEL_MONSTER01+113, PosX, PosY);
		    pCharacter->Object.Scale = 1.2f;
		    pCharacter->Weapon[0].Type = -1;
		    pCharacter->Weapon[1].Type = -1;
		}
		break;

	case 351:
		{
			pCharacter = CreateCharacter(Key, MODEL_MONSTER01+107, PosX, PosY);
			pCharacter->Object.Scale = 0.8f;
			pCharacter->Weapon[0].Type = -1;
			pCharacter->Weapon[1].Type = -1;

			OBJECT* o = &pCharacter->Object;

			//MoveEye(ThreadIndex, o, b, 16, 17);
			EVector3 vColor = { 1.5f, 0.01f, 0.0f };
   			CreateJoint(ThreadIndex, BITMAP_JOINT_ENERGY,o->Position,o->Position,o->Angle,24,o,10.0f,-1, 0, 0, -1, &vColor);
   			CreateJoint(ThreadIndex, BITMAP_JOINT_ENERGY,o->Position,o->Position,o->Angle,25,o,10.0f,-1, 0, 0, -1, &vColor);
		}
		break;

	case 352:
		{
			pCharacter = CreateCharacter(Key, MODEL_MONSTER01+108, PosX, PosY);
			pCharacter->Object.Scale = 1.3f;
			pCharacter->Weapon[0].Type = -1;
			pCharacter->Weapon[1].Type = -1;
		}
		break;
	case 354:
		{
			pCharacter = CreateCharacter(Key, MODEL_MONSTER01+110, PosX, PosY);
			pCharacter->Object.Scale = 1.3f;
			pCharacter->Object.Gravity = 0.0f;
			pCharacter->Object.Distance = (mu_float)GetLargeRand(20)/10.0f;
			pCharacter->Object.Angle[0] = 0.0f;
			pCharacter->Weapon[0].Type = -1;
			pCharacter->Weapon[1].Type = -1;
		}
		break;
	case 353:
		{
			pCharacter = CreateCharacter(Key, MODEL_MONSTER01+109, PosX, PosY);
			pCharacter->Object.Scale = 1.3f;
			pCharacter->Weapon[0].Type = -1;
			pCharacter->Weapon[1].Type = -1;
		}
		break;
	case 355:
		{
			pCharacter = CreateCharacter(Key, MODEL_MONSTER01+111, PosX, PosY);
			pCharacter->Object.Scale = 1.1f;
			pCharacter->Weapon[0].Type = -1;
			pCharacter->Weapon[1].Type = -1;
		}
		break;
	case 553:
		{
			pCharacter = CreateCharacter(Key, MODEL_MONSTER01+197, PosX, PosY);
			pCharacter->Object.Scale = 1.15f;
			pCharacter->Weapon[0].Type = -1;
			pCharacter->Weapon[1].Type = -1;
		}
		break;
	case 554:
		{
			pCharacter = CreateCharacter(Key, MODEL_MONSTER01+198, PosX, PosY);
			pCharacter->Object.Scale = 1.3f;
			pCharacter->Weapon[0].Type = -1;
			pCharacter->Weapon[1].Type = -1;
		}
		break;
	case 555:
		{
			pCharacter = CreateCharacter(Key, MODEL_MONSTER01+199, PosX, PosY);
			pCharacter->Object.Scale = 1.5f;
			pCharacter->Weapon[0].Type = -1;
			pCharacter->Weapon[1].Type = -1;
		}
		break;
	case 556:
		{
			pCharacter = CreateCharacter(Key, MODEL_MONSTER01+200, PosX, PosY);
			pCharacter->Object.Scale = 1.35f;
			pCharacter->Weapon[0].Type = -1;
			pCharacter->Weapon[1].Type = -1;
		}
		break;
	}
	
	return pCharacter;
}

mu_boolean M37Kanturu1st::SetCurrentActionKanturu1stMonster(CHARACTER* c, OBJECT* o)
{
	if (!IsKanturu1st())
		return false;

	switch (c->MonsterIndex) 
	{
	case 350:
	case 356:
	case 357:
	case 351:
	case 352:
		{
			if (GetLargeRand(2) == 0)
				SetAction(o, MONSTER01_ATTACK1);
			else
				SetAction(o, MONSTER01_ATTACK2);

			return true;
		}
		break;
	case 355:
		{
			
			if(c->MonsterSkill == ATMON_SKILL_NUM9)
			{
				SetAction ( o, MONSTER01_ATTACK2 );
				c->MonsterSkill = -1;
			}
			else
				SetAction ( o, MONSTER01_ATTACK1 );

		}
		break;
	case 553:
		{
			if(c->MonsterSkill == ATMON_SKILL_EX_BERSERKERWARRIOR_ATTACKSKILL)
			{
				SetAction ( o, MONSTER01_ATTACK2 );
				c->MonsterSkill = -1;
			}
			else
				SetAction ( o, MONSTER01_ATTACK1 );
			
		}
		break;
	case 554:
		{
			if(c->MonsterSkill == ATMON_SKILL_EX_KENTAURUSWARRIOR_ATTACKSKILL)
			{
				SetAction ( o, MONSTER01_ATTACK2 );
				c->MonsterSkill = -1;
			}
			else
				SetAction ( o, MONSTER01_ATTACK1 );
			
		}
		break;
	case 555:
		{
			if (GetLargeRand(2) == 0)
				SetAction(o, MONSTER01_ATTACK1);
			else
				SetAction(o, MONSTER01_ATTACK2);
			
			return true;
		}
		break;
	case 556:
		{
			if(c->MonsterSkill == ATMON_SKILL_EX_GENOSIDEWARRIOR_ATTACKSKILL)
			{
				SetAction ( o, MONSTER01_ATTACK2 );
				c->MonsterSkill = -1;
			}
			else
				SetAction ( o, MONSTER01_ATTACK1 );
			
		}
		break;
	}

	return false;
}

mu_boolean M37Kanturu1st::AttackEffectKanturu1stMonster(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b)
{
	if (!IsKanturu1st())
		return false;
	
	switch(o->Type) 
	{
	case MODEL_MONSTER01+106:
		{
			return true;
		}
		break;
	case MODEL_MONSTER01+112:
		{
			return true;
		}
		break;
	case MODEL_MONSTER01+113:
		{
			return true;
		}
		break;
	case MODEL_MONSTER01+107:
		{
			return true;
		}
		break;
	case MODEL_MONSTER01+108:
		{
			if(o->CurrentAction == MONSTER01_ATTACK1 || o->CurrentAction == MONSTER01_ATTACK2)
			{
				EVector3 vPos, vRelative;
				EVector3 vLight = { 0.8f, 1.0f, 0.8f };
				Vector(0.0f, 0.0f, 0.0f, vRelative);
				vRelative[0] = (mu_float)(4 - GetLargeRand(5));
				vRelative[1] = (mu_float)(4 - GetLargeRand(5));
				vRelative[2] = (mu_float)(4 - GetLargeRand(5));
				BoneManager::GetBonePosition(o, 42, vRelative, vPos);
				CreateParticle(ThreadIndex, BITMAP_SPARK+1, vPos, o->Angle, vLight, 10, 4.0f);	
			}

			if(c->AttackTime == 10)
			{
				EVector3 vPos, vRelative;
				Vector(0.0f, 0.0f, 0.0f, vRelative);
				BoneManager::GetBonePosition(o, 42, vRelative, vPos);
				CreateEffect(ThreadIndex, MODEL_IRON_RIDER_ARROW, vPos, o->Angle, o->Light, 0);
			}
			
			return true;
		}
		break;	
	case MODEL_MONSTER01+110:
		{
			if(o->CurrentAction == MONSTER01_ATTACK1 || o->CurrentAction == MONSTER01_ATTACK2)
			{
				if(c->AttackTime == 14)
				{
					EVector3 vPos, vRelative,Light;
					Vector(140.0f, 0.0f, -30.0f, vRelative);
					Vector(0.2f, 0.2f, 1.0f, Light);
					BoneManager::GetBonePosition(o, 12, vRelative, vPos);
					
					CreateEffect(ThreadIndex, MODEL_BLADE_SKILL, vPos, o->Angle, Light, 0);

					CreateParticle(ThreadIndex, BITMAP_EXPLOTION+1,vPos,o->Angle,o->Light, 0, 1.3f);
					CreateParticle(ThreadIndex, BITMAP_EXPLOTION+1,vPos,o->Angle,o->Light, 0, 2.3f);
					CreateParticle(ThreadIndex, BITMAP_EXPLOTION+1,vPos,o->Angle,o->Light, 0, 1.8f);
					CreateParticle(ThreadIndex, BITMAP_EXPLOTION+1,vPos,o->Angle,o->Light, 0, 1.3f);
					CreateParticle(ThreadIndex, BITMAP_EXPLOTION+1,vPos,o->Angle,o->Light, 0, 2.3f);
					CreateParticle(ThreadIndex, BITMAP_EXPLOTION+1,vPos,o->Angle,o->Light, 0, 1.8f);
					
				}
			}

			return true;
		}
		break;
	case MODEL_MONSTER01+111:
		{
			if(o->CurrentAction == MONSTER01_ATTACK1 || o->CurrentAction == MONSTER01_ATTACK2)
			{
				EVector3 vLight = { 0.3f, 0.5f, 0.7f };
				EVector3 vPos, vRelative;
				Vector(0.0f, 0.0f, 0.0f, vRelative);
				mu_int32 index = 31;
				mu_float Width = 2.0f;
				BoneManager::GetBonePosition(o, 27, vRelative, vPos);
				CreateParticle(ThreadIndex, BITMAP_SMOKE, vPos, o->Angle, vLight, index, Width);
				BoneManager::GetBonePosition(o, 28, vRelative, vPos);
				CreateParticle(ThreadIndex, BITMAP_SMOKE, vPos, o->Angle, vLight, index, Width);
				BoneManager::GetBonePosition(o, 29, vRelative, vPos);
				CreateParticle(ThreadIndex, BITMAP_SMOKE, vPos, o->Angle, vLight, index, Width);
				BoneManager::GetBonePosition(o, 30, vRelative, vPos);
				CreateParticle(ThreadIndex, BITMAP_SMOKE, vPos, o->Angle, vLight, index, Width);
				BoneManager::GetBonePosition(o, 34, vRelative, vPos);
				CreateParticle(ThreadIndex, BITMAP_SMOKE, vPos, o->Angle, vLight, index, Width);
				BoneManager::GetBonePosition(o, 35, vRelative, vPos);
				CreateParticle(ThreadIndex, BITMAP_SMOKE, vPos, o->Angle, vLight, index, Width);
				BoneManager::GetBonePosition(o, 36, vRelative, vPos);
				CreateParticle(ThreadIndex, BITMAP_SMOKE, vPos, o->Angle, vLight, index, Width);
				BoneManager::GetBonePosition(o, 37, vRelative, vPos);
				CreateParticle(ThreadIndex, BITMAP_SMOKE, vPos, o->Angle, vLight, index, Width);
			}
			if(c->AttackTime == 14)
			{
				EVector3 vPos, vRelative;
				
				if(o->CurrentAction == MONSTER01_ATTACK1)
				{
					Vector(30.0f, -30.0f, 0.0f, vRelative);
					BoneManager::GetBonePosition(o, 29, vRelative, vPos);
					CreateEffect(ThreadIndex, MODEL_KENTAUROS_ARROW, vPos, o->Angle, o->Light, 0);
				}
				else
				{
					Vector(60.0f, -30.0f, 50.0f, vRelative);
					BoneManager::GetBonePosition(o, 27, vRelative, vPos);
					CreateEffect(ThreadIndex, MODEL_KENTAUROS_ARROW, vPos, o->Angle, o->Light, 0);
					o->Angle[2] += 15.0f;
					CreateEffect(ThreadIndex, MODEL_KENTAUROS_ARROW, vPos, o->Angle, o->Light, 0);
					o->Angle[2] -= 30.0f;
					CreateEffect(ThreadIndex, MODEL_KENTAUROS_ARROW, vPos, o->Angle, o->Light, 0);
					o->Angle[2] += 15.0f;
				}
			}
			
			return true;
		}
		break;
	case MODEL_MONSTER01+197:
		{
			
			return true;
		}
		break;
	case MODEL_MONSTER01+198:
		{
			if(o->CurrentAction == MONSTER01_ATTACK1 || o->CurrentAction == MONSTER01_ATTACK2)
			{
				EVector3 vLight = { 0.3f, 0.5f, 0.7f };
				EVector3 vPos, vRelative;
				Vector(0.0f, 0.0f, 0.0f, vRelative);
				mu_int32 index = 31;
				mu_float Width = 2.0f;
				BoneManager::GetBonePosition(o, 27, vRelative, vPos);
				CreateParticle(ThreadIndex, BITMAP_SMOKE, vPos, o->Angle, vLight, index, Width);
				BoneManager::GetBonePosition(o, 28, vRelative, vPos);
				CreateParticle(ThreadIndex, BITMAP_SMOKE, vPos, o->Angle, vLight, index, Width);
				BoneManager::GetBonePosition(o, 29, vRelative, vPos);
				CreateParticle(ThreadIndex, BITMAP_SMOKE, vPos, o->Angle, vLight, index, Width);
				BoneManager::GetBonePosition(o, 30, vRelative, vPos);
				CreateParticle(ThreadIndex, BITMAP_SMOKE, vPos, o->Angle, vLight, index, Width);
				BoneManager::GetBonePosition(o, 34, vRelative, vPos);
				CreateParticle(ThreadIndex, BITMAP_SMOKE, vPos, o->Angle, vLight, index, Width);
				BoneManager::GetBonePosition(o, 35, vRelative, vPos);
				CreateParticle(ThreadIndex, BITMAP_SMOKE, vPos, o->Angle, vLight, index, Width);
				BoneManager::GetBonePosition(o, 36, vRelative, vPos);
				CreateParticle(ThreadIndex, BITMAP_SMOKE, vPos, o->Angle, vLight, index, Width);
				BoneManager::GetBonePosition(o, 37, vRelative, vPos);
				CreateParticle(ThreadIndex, BITMAP_SMOKE, vPos, o->Angle, vLight, index, Width);
			}
			if(c->AttackTime == 14)
			{
				EVector3 vPos, vRelative;
				
				if(o->CurrentAction == MONSTER01_ATTACK1)
				{
					Vector(30.0f, -30.0f, 0.0f, vRelative);
					BoneManager::GetBonePosition(o, 29, vRelative, vPos);
					CreateEffect(ThreadIndex, MODEL_KENTAUROS_ARROW, vPos, o->Angle, o->Light, 0);
				}
				else
				{
					Vector(60.0f, -30.0f, 50.0f, vRelative);
					BoneManager::GetBonePosition(o, 27, vRelative, vPos);
					CreateEffect(ThreadIndex, MODEL_KENTAUROS_ARROW, vPos, o->Angle, o->Light, 0);
					o->Angle[2] += 15.0f;
					CreateEffect(ThreadIndex, MODEL_KENTAUROS_ARROW, vPos, o->Angle, o->Light, 0);
					o->Angle[2] -= 30.0f;
					CreateEffect(ThreadIndex, MODEL_KENTAUROS_ARROW, vPos, o->Angle, o->Light, 0);
					o->Angle[2] += 15.0f;
				}
			}
			
			return true;
		}
		break;
	case MODEL_MONSTER01+199:
		{
			return true;
		}
		break;
	case MODEL_MONSTER01+200:
		{
			return true;
		}
		break;
	}

	return false;
}

mu_boolean M37Kanturu1st::MoveKanturu1stMonsterVisual(CHARACTER* c,OBJECT* o, MUModel* b)
{
	switch (o->Type)
	{
	case MODEL_MONSTER01+106:
		break;
	case MODEL_MONSTER01+112:
		break;
	case MODEL_MONSTER01+113:
		break;
	case MODEL_MONSTER01+111:
		break;
	case MODEL_MONSTER01+197:
		{
			
		}
		break;
	case MODEL_MONSTER01+198:
		{
			
		}
		break;
	case MODEL_MONSTER01+199:
		{	
			
		}
		break;
	case MODEL_MONSTER01+200:
		{
			
		}
		break;
	}
	
	return false;
}

mu_boolean M37Kanturu1st::RenderKanturu1stMonsterObjectMesh(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b,mu_int32 ExtraMon)
{
	switch (o->Type)
	{
	case MODEL_MONSTER01+106:
		{
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh,
				o->BlendMeshLight, o->BlendMeshTexCoordU,
				o->BlendMeshTexCoordV, o->HiddenMesh);

			if (o->CurrentAction == MONSTER01_DIE)
				return true;

			mu_float fLumi = EMath::Sin(FWorldTime * 0.002f) + 1.0f;
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0,
				fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV,
				BITMAP_BERSERK_EFFECT);
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 1,
				fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV,
				BITMAP_BERSERK_WP_EFFECT);

			return true;
		}
		break;
	case MODEL_MONSTER01+112:
		{
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh,
				o->BlendMeshLight, o->BlendMeshTexCoordU,
				o->BlendMeshTexCoordV);

			if (o->CurrentAction != MONSTER01_DIE)
				b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha,
					o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU,
					o->BlendMeshTexCoordV);
			
			b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, o->Alpha, o->BlendMesh,
				o->BlendMeshLight, o->BlendMeshTexCoordU,
				o->BlendMeshTexCoordV);

			return true;
		}
		break;
	case MODEL_MONSTER01+113:
		{
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh,
				o->BlendMeshLight, o->BlendMeshTexCoordU,
				o->BlendMeshTexCoordV, o->HiddenMesh);
			return true;	
		}
		break;

	case MODEL_MONSTER01+107:
		{
			mu_float fLumi = (EMath::Sin(FWorldTime*0.002f) + 1.0f) * 0.5f;
			Vector(1.0f,1.0f,1.0f,b->BodyLight(ThreadIndex));
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE);
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE);
			b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE|RENDER_BRIGHT, o->Alpha, 2, fLumi);
			b->RenderMesh(ThreadIndex, 3, RENDER_TEXTURE|RENDER_BRIGHT, o->Alpha, 3, fLumi);
			b->RenderMesh(ThreadIndex, 4, RENDER_TEXTURE);
			return true;
		}
		break;

	case MODEL_MONSTER01+108:
		{
			b->RenderBody(ThreadIndex, RENDER_TEXTURE,o->Alpha,o->BlendMesh,o->BlendMeshLight,o->BlendMeshTexCoordU,o->BlendMeshTexCoordV,o->HiddenMesh);
			return true;
		}
		break;
	case MODEL_MONSTER01+110:
		{
			if(o->CurrentAction != MONSTER01_DIE)
			{
				mu_float fLumi = EMath::Sin(FWorldTime*0.002f) + 1.0f;
				b->RenderBody(ThreadIndex, RENDER_TEXTURE,o->Alpha,o->BlendMesh,o->BlendMeshLight,o->BlendMeshTexCoordU,o->BlendMeshTexCoordV,o->HiddenMesh);
				b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE|RENDER_BRIGHT, o->Alpha, 0, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_BLADEHUNTER_EFFECT);
			}
			else
			{
				if (o->Alpha < 0.0f)
					o->Alpha = 0.0f;

				if (o->Alpha > 0.8f)
					b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
				else
					b->RenderBody(ThreadIndex, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_CHROME);

				b->RenderMesh(ThreadIndex, 0, RENDER_CHROME | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_CHROME);
			}
			return true;
		}
		break;
	case MODEL_MONSTER01+109:
		{
			EVector3 Light;
			VectorCopy(b->BodyLight(ThreadIndex), Light);
			b->RenderBody(ThreadIndex, RENDER_TEXTURE,o->Alpha,o->BlendMesh,o->BlendMeshLight,o->BlendMeshTexCoordU,o->BlendMeshTexCoordV,o->HiddenMesh);
			b->StreamMesh(ThreadIndex) = 1;
			b->RenderMesh(ThreadIndex, 1,RENDER_TEXTURE|RENDER_BRIGHT,1.0f,0,o->BlendMeshLight,o->BlendMeshTexCoordU,FWorldTime*0.0005f);
			b->RenderMesh(ThreadIndex, 3,RENDER_CHROME|RENDER_BRIGHT, 1.0f, 0,o->BlendMeshLight,o->BlendMeshTexCoordU,FWorldTime*0.001f,BITMAP_CHROME);

			mu_float Luminosity = EMath::Sin( FWorldTime*0.001f )*0.5f+0.5f;
	        Vector(Light[0]*Luminosity,Light[0]*Luminosity,Light[0]*Luminosity,b->BodyLight(ThreadIndex));
			b->RenderMesh(ThreadIndex, 4,RENDER_TEXTURE|RENDER_BRIGHT,1.0f,2,o->BlendMeshLight,FWorldTime*0.0001f,-FWorldTime*0.0005f);
			return true;
		}
		break;
	case MODEL_MONSTER01+111:
		{
			mu_float fLumi = (EMath::Sin(FWorldTime*0.002f) + 1.0f) * 0.5f;
			Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 1, fLumi);
			b->RenderMesh(ThreadIndex, 1, RENDER_CHROME2 | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			return true;
		}
		break;
	case MODEL_MONSTER01+197:
		{
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh,
				o->BlendMeshLight, o->BlendMeshTexCoordU,
				o->BlendMeshTexCoordV, o->HiddenMesh);
			
			if (o->CurrentAction == MONSTER01_DIE)
				return true;
			
			mu_float fLumi = EMath::Sin(FWorldTime * 0.002f) + 1.0f;
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0,
				fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV,
				BITMAP_BERSERK_EFFECT);
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 1,
				fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV,
				BITMAP_BERSERK_WP_EFFECT);
			
			return true;
		}
		break;
	case MODEL_MONSTER01+198:
		{
			mu_float fLumi = (EMath::Sin(FWorldTime*0.002f) + 1.0f) * 0.5f;
			Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 1, fLumi);
			b->RenderMesh(ThreadIndex, 1, RENDER_CHROME2 | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			return true;
		}
		break;
	case MODEL_MONSTER01+199:
		{
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh,
				o->BlendMeshLight, o->BlendMeshTexCoordU,
				o->BlendMeshTexCoordV);
			
			if (o->CurrentAction != MONSTER01_DIE)
				b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha,
				o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU,
				o->BlendMeshTexCoordV);
			
			b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, o->Alpha, o->BlendMesh,
				o->BlendMeshLight, o->BlendMeshTexCoordU,
				o->BlendMeshTexCoordV);
			
			return true;
		}
		break;
	case MODEL_MONSTER01+200:
		{
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh,
				o->BlendMeshLight, o->BlendMeshTexCoordU,
				o->BlendMeshTexCoordV, o->HiddenMesh);
			return true;	
		}
		break;
	}

	return false;
}

mu_boolean M37Kanturu1st::RenderKanturu1stMonsterVisual(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b)
{
	switch (o->Type)
	{
	case MODEL_MONSTER01 + 106:
		{
			if (o->CurrentAction == MONSTER01_WALK)
			{
				if (GetLargeRand(15) == 0)
					PlayBackground(SOUND_KANTURU_1ST_BER_MOVE1 + GetLargeRand(2));
			}
			else if (o->CurrentAction == MONSTER01_ATTACK1 || o->CurrentAction == MONSTER01_ATTACK2)
			{
				if (o->SubType == 0)
				{
					o->SubType = 1;
					PlayBackground(SOUND_KANTURU_1ST_BER_ATTACK1 + GetLargeRand(2));
				}
			}
			else if (o->CurrentAction == MONSTER01_DIE)
			{
				if (o->SubType == 0)
				{
					o->SubType = 1;
					PlayBackground(SOUND_KANTURU_1ST_BER_DIE);
				}
			}

			if (o->CurrentAction == MONSTER01_STOP1 || o->CurrentAction == MONSTER01_STOP2)
				o->SubType = 0;

			if (o->CurrentAction == MONSTER01_DIE
				|| o->CurrentAction == MONSTER01_ATTACK1
				|| o->CurrentAction == MONSTER01_ATTACK2)
				return true;

			EVector3 vPos, vRelative;
			Vector(0.0f, 0.0f, 0.0f, vRelative);
			BoneManager::GetBonePosition(o, 9, vRelative, vPos);
			CreateParticle(ThreadIndex,
				BITMAP_SMOKE, vPos, o->Angle, o->Light, 42, o->Scale);

			return true;
		}
		break;

	case MODEL_MONSTER01 + 112:
		{
			if (o->CurrentAction == MONSTER01_WALK)
			{
				if (GetLargeRand(15) == 0)
					PlayBackground(SOUND_KANTURU_1ST_GIGAN_MOVE1);
			}
			else if (o->CurrentAction == MONSTER01_ATTACK1 || o->CurrentAction == MONSTER01_ATTACK2)
			{
				if (o->SubType == 0)
				{
					o->SubType = 1;
					PlayBackground(SOUND_KANTURU_1ST_GIGAN_ATTACK1 + GetLargeRand(2));
				}
			}
			else if (o->CurrentAction == MONSTER01_DIE)
			{
				if (o->SubType == 0)
				{
					o->SubType = 1;
					PlayBackground(SOUND_KANTURU_1ST_GIGAN_DIE);
				}
			}

			if (o->CurrentAction == MONSTER01_STOP1 || o->CurrentAction == MONSTER01_STOP2)
				o->SubType = 0;

			if (o->CurrentAction == MONSTER01_DIE)
				return true;

			EVector3 EndRelative, EndPos;
			Vector(19.0f, -2.0f, 0.0f, EndRelative);

			b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(7)], EndRelative, EndPos, true);

			Vector(0.4f, 0.6f, 0.8f, o->Light);
			CreateSprite(BITMAP_LIGHT, EndPos, 3.0f, o->Light, o, 0.5f);

			mu_float Luminosity;
			Luminosity = EMath::Sin(FWorldTime * 0.05f) * 0.4f + 0.9f;
			Vector(Luminosity * 0.3f, Luminosity * 0.5f, Luminosity * 0.8f,
				o->Light);
			CreateSprite(BITMAP_LIGHT, EndPos, 1.0f, o->Light, o);

			return true;
		}
		break;

	case MODEL_MONSTER01 + 113:
		{
			if (o->CurrentAction == MONSTER01_WALK)
			{
				if (GetLargeRand(15) == 0)
					PlayBackground(SOUND_KANTURU_1ST_GENO_MOVE1 + GetLargeRand(2));
			}
			else if (o->CurrentAction == MONSTER01_ATTACK1 || o->CurrentAction == MONSTER01_ATTACK2)
			{
				if (o->SubType == 0)
				{
					o->SubType = 1;
					PlayBackground(SOUND_KANTURU_1ST_GENO_ATTACK1 + GetLargeRand(2));
				}
			}
			else if (o->CurrentAction == MONSTER01_DIE)
			{
				if (o->SubType == 0)
				{
					o->SubType = 1;
					PlayBackground(SOUND_KANTURU_1ST_GENO_DIE);
				}
			}

			if (o->CurrentAction == MONSTER01_STOP1 || o->CurrentAction == MONSTER01_STOP2)
				o->SubType = 0;

			if (o->CurrentAction == MONSTER01_WALK)
			{
				EVector3 vPos, vRelative;
				Vector(0.0f, 0.0f, 0.0f, vRelative);
				BoneManager::GetBonePosition(o, 47, vRelative, vPos);
				CreateParticle(ThreadIndex,
					BITMAP_SMOKE + 1, vPos, o->Angle, o->Light, 1, o->Scale);

				CreateEffect(ThreadIndex, MODEL_STONE1 + GetLargeRand(2), vPos, o->Angle, o->Light);
			}
			return true;
		}
		break;

	case MODEL_MONSTER01 + 107:
		{
			MoveEye(ThreadIndex, o, b, 16, 17);

			if (o->CurrentAction == MONSTER01_WALK)
			{
				if (GetLargeRand(15) == 0)
					PlayBackground(SOUND_KANTURU_1ST_SWOLF_MOVE1 + GetLargeRand(2));
			}
			else if (o->CurrentAction == MONSTER01_ATTACK1 || o->CurrentAction == MONSTER01_ATTACK2)
			{
				if (o->SubType == 0)
				{
					o->SubType = 1;
					PlayBackground(SOUND_KANTURU_1ST_SWOLF_ATTACK1 + GetLargeRand(2));
				}
			}
			else if (o->CurrentAction == MONSTER01_DIE)
			{
				if (o->SubType == 0)
				{
					o->SubType = 1;
					PlayBackground(SOUND_KANTURU_1ST_SWOLF_DIE);
				}
			}
			return true;
		}
		break;
	case MODEL_MONSTER01 + 108:
		{
			if (o->CurrentAction != MONSTER01_DIE)
			{
				EVector3 vPos, vRelative;
				EVector3 vLight = { 0.8f, 1.0f, 0.8f };
				Vector(0.0f, 0.0f, 0.0f, vRelative);
				vRelative[0] = (mu_float)(4 - GetLargeRand(5));
				vRelative[1] = (mu_float)(4 - GetLargeRand(5));
				vRelative[2] = (mu_float)(4 - GetLargeRand(5));
				BoneManager::GetBonePosition(o, 52, vRelative, vPos);
				CreateParticle(ThreadIndex, BITMAP_SPARK + 1, vPos, o->Angle, vLight, 10, 4.0f);
				BoneManager::GetBonePosition(o, 47, vRelative, vPos);
				CreateParticle(ThreadIndex, BITMAP_SPARK + 1, vPos, o->Angle, vLight, 10, 4.0f);

				if (o->CurrentAction == MONSTER01_WALK)
				{
					if (GetLargeRand(15) == 0)
						PlayBackground(SOUND_KANTURU_1ST_IR_MOVE1 + GetLargeRand(2));
				}
				else if (o->CurrentAction == MONSTER01_ATTACK1 || o->CurrentAction == MONSTER01_ATTACK2)
				{
					if (o->SubType == 0)
					{
						o->SubType = 1;
						PlayBackground(SOUND_KANTURU_1ST_IR_ATTACK1 + GetLargeRand(2));
					}
				}
			}
			else
			{
				EVector3 vLight = { 1.0f, 1.0f, 1.0f };
				EVector3 vPos, vRelative;
				Vector(0.0f, 0.0f, 0.0f, vRelative);
				BoneManager::GetBonePosition(o, 2, vRelative, vPos);
				CreateParticle(ThreadIndex, BITMAP_SMOKE + 3, vPos, o->Angle, vLight, 3, 2.0f);
				CreateParticle(ThreadIndex, BITMAP_SMOKE + 3, vPos, o->Angle, vLight, 4, 1.0f);
				if (o->SubType == 0)
				{
					o->SubType = 1;
					PlayBackground(SOUND_KANTURU_1ST_IR_DIE);
				}
			}
			return true;
		}
		break;
	case MODEL_MONSTER01 + 109:
		{
			if (o->CurrentAction == MONSTER01_WALK)
			{
				if (GetLargeRand(15) == 0)
					PlayBackground(SOUND_KANTURU_1ST_SATI_MOVE1 + GetLargeRand(2));
			}
			else if (o->CurrentAction == MONSTER01_ATTACK1 || o->CurrentAction == MONSTER01_ATTACK2)
			{
				if (o->SubType == 0)
				{
					o->SubType = 1;
					PlayBackground(SOUND_KANTURU_1ST_SATI_ATTACK1 + GetLargeRand(2));
				}
			}
			else if (o->CurrentAction == MONSTER01_DIE)
			{
				if (o->SubType == 0)
				{
					o->SubType = 1;
					PlayBackground(SOUND_KANTURU_1ST_SATI_DIE);
				}
			}
		}
		break;
	case MODEL_MONSTER01 + 110:
		{
			if (GET_WORLD == WD_39KANTURU_3RD && g_Direction.m_CKanturu.m_iKanturuState == KANTURU_STATE_MAYA_BATTLE)
			{
				EVector3 Light;
				mu_float Scale, Angle;

				o->Distance += 0.05f;
				Scale = o->Distance;

				if (Scale >= 3.0f)
					o->Distance = 0.1f;

				Angle = (g_CurrentTime % 9000) / 10.0f;

				Light[0] = 0.6f - (Scale / 5.0f);
				Light[1] = 0.6f - (Scale / 5.0f);
				Light[2] = 0.6f - (Scale / 5.0f);

				RenderTerrainAlphaBitmap(BITMAP_ENERGY_RING, BLEND::TXB_BLEND01, o->Position[0], o->Position[1], Scale, Scale, Light, Angle);

				Vector(0.5f, 0.5f, 0.5f, Light);
				RenderTerrainAlphaBitmap(BITMAP_ENERGY_FIELD, BLEND::TXB_BLEND01, o->Position[0], o->Position[1], 2.0f, 2.0f, Light, Angle);
			}
			if (o->CurrentAction == MONSTER01_WALK)
			{
				if (GetLargeRand(10) == 0)
				{
					if (GET_WORLD != WD_39KANTURU_3RD)
						CreateParticle(ThreadIndex, BITMAP_SMOKE + 1, o->Position, o->Angle, o->Light);
					PlayBackground(SOUND_KANTURU_1ST_BLADE_MOVE1 + GetLargeRand(2));
				}
			}
			else if (o->CurrentAction == MONSTER01_ATTACK1 || o->CurrentAction == MONSTER01_ATTACK2)
			{
				if (o->SubType == 0)
				{
					o->SubType = 1;
					PlayBackground(SOUND_KANTURU_1ST_BLADE_ATTACK1 + GetLargeRand(2));
				}
			}
			else if (o->CurrentAction == MONSTER01_DIE)
			{
				if (o->SubType == 0)
				{
					o->SubType = 1;
					PlayBackground(SOUND_KANTURU_1ST_BLADE_DIE);
				}
			}
		}
		break;
	case MODEL_MONSTER01 + 111:
		{
			if (o->CurrentAction == MONSTER01_WALK)
			{
				if (GetLargeRand(15) == 0)
					PlayBackground(SOUND_KANTURU_1ST_KENTA_MOVE1 + GetLargeRand(2));
			}
			else if (o->CurrentAction == MONSTER01_ATTACK1 || o->CurrentAction == MONSTER01_ATTACK2)
			{
				if (o->SubType == 0)
				{
					o->SubType = 1;
					PlayBackground(SOUND_KANTURU_1ST_KENTA_ATTACK1 + GetLargeRand(2));
				}
			}
			else if (o->CurrentAction == MONSTER01_DIE)
			{
				if (o->SubType == 0)
				{
					o->SubType = 1;
					PlayBackground(SOUND_KANTURU_1ST_KENTA_DIE);
				}
			}
			EVector3 vPos, vRelative;
			Vector(0.0f, 0.0f, 0.0f, vRelative);
			if (o->CurrentAction == MONSTER01_DIE)
			{
				mu_float Scale = 0.3f;
				BoneManager::GetBonePosition(o, 81, vRelative, vPos);
				CreateParticle(ThreadIndex, BITMAP_SMOKE + 3, vPos, o->Angle, o->Light, 3, Scale);
				BoneManager::GetBonePosition(o, 82, vRelative, vPos);
				CreateParticle(ThreadIndex, BITMAP_SMOKE + 1, vPos, o->Angle, o->Light, 1, Scale);
				BoneManager::GetBonePosition(o, 83, vRelative, vPos);
				CreateParticle(ThreadIndex, BITMAP_SMOKE + 3, vPos, o->Angle, o->Light, 3, Scale);
				BoneManager::GetBonePosition(o, 4, vRelative, vPos);
				CreateParticle(ThreadIndex, BITMAP_SMOKE + 1, vPos, o->Angle, o->Light, 1, Scale);
				BoneManager::GetBonePosition(o, 5, vRelative, vPos);
				CreateParticle(ThreadIndex, BITMAP_SMOKE + 3, vPos, o->Angle, o->Light, 3, Scale);
				BoneManager::GetBonePosition(o, 6, vRelative, vPos);
				CreateParticle(ThreadIndex, BITMAP_SMOKE + 1, vPos, o->Angle, o->Light, 1, Scale);
			}
			else
				if (o->CurrentAction == MONSTER01_WALK || o->CurrentAction == MONSTER01_RUN)
				{
					if (GetLargeRand(10) == 0) {
						CreateParticle(ThreadIndex, BITMAP_SMOKE + 1, o->Position, o->Angle, o->Light);
					}
				}
		}
		break;
	case MODEL_MONSTER01 + 197:
		{
			if (o->CurrentAction == MONSTER01_WALK)
			{
				if (GetLargeRand(15) == 0)
					PlayBackground(SOUND_KANTURU_1ST_BER_MOVE1 + GetLargeRand(2));
			}
			else if (o->CurrentAction == MONSTER01_ATTACK1 || o->CurrentAction == MONSTER01_ATTACK2)
			{
				if (o->SubType == 0)
				{
					o->SubType = 1;
					PlayBackground(SOUND_KANTURU_1ST_BER_ATTACK1 + GetLargeRand(2));
				}
			}
			else if (o->CurrentAction == MONSTER01_DIE)
			{
				if (o->SubType == 0)
				{
					o->SubType = 1;
					PlayBackground(SOUND_KANTURU_1ST_BER_DIE);
				}
			}

			if (o->CurrentAction == MONSTER01_STOP1 || o->CurrentAction == MONSTER01_STOP2)
				o->SubType = 0;

			if (o->CurrentAction == MONSTER01_DIE
				|| o->CurrentAction == MONSTER01_ATTACK1
				|| o->CurrentAction == MONSTER01_ATTACK2)
				return true;

			EVector3 vPos, vRelative;
			Vector(0.0f, 0.0f, 0.0f, vRelative);
			BoneManager::GetBonePosition(o, 9, vRelative, vPos);
			CreateParticle(ThreadIndex,
				BITMAP_SMOKE, vPos, o->Angle, o->Light, 42, o->Scale);
		}
		return true;
	case MODEL_MONSTER01 + 198:
		{
			if (o->CurrentAction == MONSTER01_WALK)
			{
				if (GetLargeRand(15) == 0)
					PlayBackground(SOUND_KANTURU_1ST_KENTA_MOVE1 + GetLargeRand(2));
			}
			else if (o->CurrentAction == MONSTER01_ATTACK1 || o->CurrentAction == MONSTER01_ATTACK2)
			{
				if (o->SubType == 0)
				{
					o->SubType = 1;
					PlayBackground(SOUND_KANTURU_1ST_KENTA_ATTACK1 + GetLargeRand(2));
				}
			}
			else if (o->CurrentAction == MONSTER01_DIE)
			{
				if (o->SubType == 0)
				{
					o->SubType = 1;
					PlayBackground(SOUND_KANTURU_1ST_KENTA_DIE);
				}
			}
			EVector3 vPos, vRelative;
			Vector(0.0f, 0.0f, 0.0f, vRelative);
			if (o->CurrentAction == MONSTER01_DIE)
			{
				mu_float Scale = 0.3f;
				BoneManager::GetBonePosition(o, 81, vRelative, vPos);
				CreateParticle(ThreadIndex, BITMAP_SMOKE + 3, vPos, o->Angle, o->Light, 3, Scale);
				BoneManager::GetBonePosition(o, 82, vRelative, vPos);
				CreateParticle(ThreadIndex, BITMAP_SMOKE + 1, vPos, o->Angle, o->Light, 1, Scale);
				BoneManager::GetBonePosition(o, 83, vRelative, vPos);
				CreateParticle(ThreadIndex, BITMAP_SMOKE + 3, vPos, o->Angle, o->Light, 3, Scale);
				BoneManager::GetBonePosition(o, 4, vRelative, vPos);
				CreateParticle(ThreadIndex, BITMAP_SMOKE + 1, vPos, o->Angle, o->Light, 1, Scale);
				BoneManager::GetBonePosition(o, 5, vRelative, vPos);
				CreateParticle(ThreadIndex, BITMAP_SMOKE + 3, vPos, o->Angle, o->Light, 3, Scale);
				BoneManager::GetBonePosition(o, 6, vRelative, vPos);
				CreateParticle(ThreadIndex, BITMAP_SMOKE + 1, vPos, o->Angle, o->Light, 1, Scale);
			}
			else
			{
				if (o->CurrentAction == MONSTER01_WALK || o->CurrentAction == MONSTER01_RUN)
				{
					if (GetLargeRand(10) == 0) {
						CreateParticle(ThreadIndex, BITMAP_SMOKE + 1, o->Position, o->Angle, o->Light);
					}
				}
			}
		}
		return true;
	case MODEL_MONSTER01 + 199:
		{
			if (o->CurrentAction == MONSTER01_WALK)
			{
				if (GetLargeRand(15) == 0)
					PlayBackground(SOUND_KANTURU_1ST_GIGAN_MOVE1);
			}
			else if (o->CurrentAction == MONSTER01_ATTACK1 || o->CurrentAction == MONSTER01_ATTACK2)
			{
				if (o->SubType == 0)
				{
					o->SubType = 1;
					PlayBackground(SOUND_KANTURU_1ST_GIGAN_ATTACK1 + GetLargeRand(2));
				}
			}
			else if (o->CurrentAction == MONSTER01_DIE)
			{
				if (o->SubType == 0)
				{
					o->SubType = 1;
					PlayBackground(SOUND_KANTURU_1ST_GIGAN_DIE);
				}
			}

			if (o->CurrentAction == MONSTER01_STOP1 || o->CurrentAction == MONSTER01_STOP2)
				o->SubType = 0;

			if (o->CurrentAction == MONSTER01_DIE)
				return true;

			EVector3 EndRelative, EndPos;
			Vector(19.0f, -2.0f, 0.0f, EndRelative);

			b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(7)], EndRelative, EndPos, true);

			Vector(0.4f, 0.6f, 0.8f, o->Light);
			CreateSprite(BITMAP_LIGHT, EndPos, 3.0f, o->Light, o, 0.5f);

			mu_float Luminosity;
			Luminosity = EMath::Sin(FWorldTime * 0.05f) * 0.4f + 0.9f;
			Vector(Luminosity * 0.3f, Luminosity * 0.5f, Luminosity * 0.8f,
				o->Light);
			CreateSprite(BITMAP_LIGHT, EndPos, 1.0f, o->Light, o);
		}
		return true;
	case MODEL_MONSTER01 + 200:
		{
			if (o->CurrentAction == MONSTER01_WALK)
			{
				if (GetLargeRand(15) == 0)
					PlayBackground(SOUND_KANTURU_1ST_GENO_MOVE1 + GetLargeRand(2));
			}
			else if (o->CurrentAction == MONSTER01_ATTACK1 || o->CurrentAction == MONSTER01_ATTACK2)
			{
				if (o->SubType == 0)
				{
					o->SubType = 1;
					PlayBackground(SOUND_KANTURU_1ST_GENO_ATTACK1 + GetLargeRand(2));
				}
			}
			else if (o->CurrentAction == MONSTER01_DIE)
			{
				if (o->SubType == 0)
				{
					o->SubType = 1;
					PlayBackground(SOUND_KANTURU_1ST_GENO_DIE);
				}
			}

			if (o->CurrentAction == MONSTER01_STOP1 || o->CurrentAction == MONSTER01_STOP2)
				o->SubType = 0;

			if (o->CurrentAction == MONSTER01_WALK)
			{
				EVector3 vPos, vRelative;
				Vector(0.0f, 0.0f, 0.0f, vRelative);
				BoneManager::GetBonePosition(o, 47, vRelative, vPos);
				CreateParticle(ThreadIndex,
					BITMAP_SMOKE + 1, vPos, o->Angle, o->Light, 1, o->Scale);

				CreateEffect(ThreadIndex, MODEL_STONE1 + GetLargeRand(2), vPos, o->Angle, o->Light);
			}
		}
		return true;
	}
	return false;
}

void M37Kanturu1st::MoveKanturu1stBlurEffect(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	switch (o->Type)
	{
	case MODEL_MONSTER01 + 109:
		{
			if ((o->AnimationFrame >= 3.6f && o->AnimationFrame <= 6.0f
				&& o->CurrentAction == MONSTER01_ATTACK1)
				|| (o->AnimationFrame >= 3.6f && o->AnimationFrame <= 6.0f
					&& o->CurrentAction == MONSTER01_ATTACK2))
			{
				EVector3  Light;
				Vector(0.3f, 1.0f, 3.5f, Light);

				EVector3 StartPos, StartRelative;
				EVector3 EndPos, EndRelative;

				mu_float fActionSpeed = b->_AnimationHeader[o->CurrentAction].Speed;
				mu_float fSpeedPerFrame = fActionSpeed / 10.0;
				mu_float fAnimationFrame = o->AnimationFrame - fActionSpeed;
				for (mu_int32 i = 0; i < 10; ++i)
				{
					b->Animation(ThreadIndex, ThreadInfo.BoneTransform, fAnimationFrame,
						o->PriorAnimationFrame, o->PriorAction, o->Angle,
						o->HeadAngle);

					Vector(0.0f, 0.0f, 0.0f, StartRelative);
					Vector(0.0f, 0.0f, -200.0f, EndRelative);

					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(44)], StartRelative, StartPos, false);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(44)], EndRelative, EndPos, false);
					CreateBlur(c, StartPos, EndPos, Light, 1);

					fAnimationFrame += fSpeedPerFrame;
				}
			}
		}
		break;
	case MODEL_MONSTER01 + 106:
		{
			if ((o->AnimationFrame >= 3.5f && o->AnimationFrame <= 6.7f
				&& o->CurrentAction == MONSTER01_ATTACK1)
				|| (o->AnimationFrame >= 3.5f && o->AnimationFrame <= 6.7f
					&& o->CurrentAction == MONSTER01_ATTACK2))
			{
				EVector3  Light;
				Vector(0.3f, 2.0f, 0.5f, Light);

				EVector3 StartPos, StartRelative;
				EVector3 EndPos, EndRelative;

				mu_float fActionSpeed = b->_AnimationHeader[o->CurrentAction].Speed;
				mu_float fSpeedPerFrame = fActionSpeed / 10.0;
				mu_float fAnimationFrame = o->AnimationFrame - fActionSpeed;
				for (mu_int32 i = 0; i < 10; ++i)
				{
					b->Animation(ThreadIndex, ThreadInfo.BoneTransform, fAnimationFrame,
						o->PriorAnimationFrame, o->PriorAction, o->Angle,
						o->HeadAngle);

					Vector(-20.0f, 0.0f, 0.0f, StartRelative);
					Vector(100.0f, 0.0f, 0.0f, EndRelative);

					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(33)], StartRelative, StartPos, false);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(34)], EndRelative, EndPos, false);
					CreateBlur(c, StartPos, EndPos, Light, 1);

					fAnimationFrame += fSpeedPerFrame;
				}
			}
		}
		break;
	case MODEL_MONSTER01 + 110:
		{
			mu_float Start_Frame = 5.9f;
			mu_float End_Frame = 7.55f;
			if ((o->AnimationFrame >= Start_Frame && o->AnimationFrame <= End_Frame && o->CurrentAction == MONSTER01_ATTACK1)
				|| (o->AnimationFrame >= Start_Frame && o->AnimationFrame <= End_Frame && o->CurrentAction == MONSTER01_ATTACK2))
			{
				EVector3  Light;
				Vector(1.0f, 1.2f, 2.0f, Light);

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

					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(55)], StartRelative, StartPos, false);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(54)], EndRelative, EndPos, false);
					CreateBlur(c, StartPos, EndPos, Light, 0);

					fAnimationFrame += fSpeedPerFrame;
				}
			}
		}
		break;
	case MODEL_MONSTER01 + 112:
		{
			if ((o->AnimationFrame >= 3.5f && o->AnimationFrame <= 5.9f
				&& o->CurrentAction == MONSTER01_ATTACK1)
				|| (o->AnimationFrame >= 3.5f && o->AnimationFrame <= 5.9f
					&& o->CurrentAction == MONSTER01_ATTACK2))
			{
				EVector3  Light;
				Vector(1.0f, 1.2f, 2.0f, Light);

				EVector3 StartPos, StartRelative;
				EVector3 EndPos, EndRelative;

				mu_float fActionSpeed = b->_AnimationHeader[o->CurrentAction].Speed;
				mu_float fSpeedPerFrame = fActionSpeed / 10.0;
				mu_float fAnimationFrame = o->AnimationFrame - fActionSpeed;
				for (mu_int32 i = 0; i < 10; ++i)
				{
					b->Animation(ThreadIndex, ThreadInfo.BoneTransform, fAnimationFrame,
						o->PriorAnimationFrame, o->PriorAction, o->Angle,
						o->HeadAngle);

					Vector(0.0f, 0.0f, 0.0f, StartRelative);
					Vector(0.0f, 0.0f, 0.0f, EndRelative);

					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(34)], StartRelative, StartPos, false);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(33)], EndRelative, EndPos, false);
					CreateBlur(c, StartPos, EndPos, Light, 0);

					fAnimationFrame += fSpeedPerFrame;
				}
			}
		}
		break;
	case MODEL_MONSTER01 + 113:
		{
			if ((o->AnimationFrame >= 5.5f && o->AnimationFrame <= 6.9f
				&& o->CurrentAction == MONSTER01_ATTACK1)
				|| (o->AnimationFrame >= 5.5f && o->AnimationFrame <= 6.9f
					&& o->CurrentAction == MONSTER01_ATTACK2))
			{
				EVector3  Light;
				Vector(1.0f, 0.2f, 0.0f, Light);

				EVector3 StartPos, StartRelative;
				EVector3 EndPos, EndRelative;

				mu_float fActionSpeed = b->_AnimationHeader[o->CurrentAction].Speed;
				mu_float fSpeedPerFrame = fActionSpeed / 10.0;
				mu_float fAnimationFrame = o->AnimationFrame - fActionSpeed;
				for (mu_int32 i = 0; i < 18; ++i)
				{
					b->Animation(ThreadIndex, ThreadInfo.BoneTransform, fAnimationFrame,
						o->PriorAnimationFrame, o->PriorAction, o->Angle,
						o->HeadAngle);

					Vector(-40.0f, 0.0f, 0.0f, StartRelative);
					Vector(10.0f, 0.0f, 0.0f, EndRelative);

					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(49)], StartRelative, StartPos, false);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(49)], EndRelative, EndPos, false);
					CreateBlur(c, StartPos, EndPos, Light, 0, true, 0);

					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(51)], StartRelative, StartPos, false);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(51)], EndRelative, EndPos, false);
					CreateBlur(c, StartPos, EndPos, Light, 0, true, 1);

					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(50)], StartRelative, StartPos, false);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(50)], EndRelative, EndPos, false);
					CreateBlur(c, StartPos, EndPos, Light, 0, true, 2);

					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(52)], StartRelative, StartPos, false);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(52)], EndRelative, EndPos, false);
					CreateBlur(c, StartPos, EndPos, Light, 0, true, 3);

					fAnimationFrame += fSpeedPerFrame;
				}
			}
		}
		break;
	case MODEL_MONSTER01 + 197:
		{
			if ((o->AnimationFrame >= 3.5f && o->AnimationFrame <= 6.7f
				&& o->CurrentAction == MONSTER01_ATTACK1)
				|| (o->AnimationFrame >= 3.5f && o->AnimationFrame <= 6.7f
					&& o->CurrentAction == MONSTER01_ATTACK2))
			{
				EVector3  Light;
				Vector(0.3f, 2.0f, 0.5f, Light);

				EVector3 StartPos, StartRelative;
				EVector3 EndPos, EndRelative;

				mu_float fActionSpeed = b->_AnimationHeader[o->CurrentAction].Speed;
				mu_float fSpeedPerFrame = fActionSpeed / 10.0;
				mu_float fAnimationFrame = o->AnimationFrame - fActionSpeed;
				for (mu_int32 i = 0; i < 10; ++i)
				{
					b->Animation(ThreadIndex, ThreadInfo.BoneTransform, fAnimationFrame,
						o->PriorAnimationFrame, o->PriorAction, o->Angle,
						o->HeadAngle);

					Vector(-20.0f, 0.0f, 0.0f, StartRelative);
					Vector(100.0f, 0.0f, 0.0f, EndRelative);

					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(33)], StartRelative, StartPos, false);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(34)], EndRelative, EndPos, false);
					CreateBlur(c, StartPos, EndPos, Light, 1);

					fAnimationFrame += fSpeedPerFrame;
				}
			}
		}
		break;
	case MODEL_MONSTER01 + 198:
		{
			if ((o->AnimationFrame >= 3.5f && o->AnimationFrame <= 5.9f
				&& o->CurrentAction == MONSTER01_ATTACK1)
				|| (o->AnimationFrame >= 3.5f && o->AnimationFrame <= 5.9f
					&& o->CurrentAction == MONSTER01_ATTACK2))
			{
				EVector3  Light;
				Vector(1.0f, 1.2f, 2.0f, Light);

				EVector3 StartPos, StartRelative;
				EVector3 EndPos, EndRelative;

				mu_float fActionSpeed = b->_AnimationHeader[o->CurrentAction].Speed;
				mu_float fSpeedPerFrame = fActionSpeed / 10.0;
				mu_float fAnimationFrame = o->AnimationFrame - fActionSpeed;
				for (mu_int32 i = 0; i < 10; ++i)
				{
					b->Animation(ThreadIndex, ThreadInfo.BoneTransform, fAnimationFrame,
						o->PriorAnimationFrame, o->PriorAction, o->Angle,
						o->HeadAngle);

					Vector(0.0f, 0.0f, 0.0f, StartRelative);
					Vector(0.0f, 0.0f, 0.0f, EndRelative);

					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(34)], StartRelative, StartPos, false);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(33)], EndRelative, EndPos, false);
					CreateBlur(c, StartPos, EndPos, Light, 0);

					fAnimationFrame += fSpeedPerFrame;
				}
			}
		}
		break;
	case MODEL_MONSTER01 + 199:
		{
			if ((o->AnimationFrame >= 5.5f && o->AnimationFrame <= 6.9f
				&& o->CurrentAction == MONSTER01_ATTACK1)
				|| (o->AnimationFrame >= 5.5f && o->AnimationFrame <= 6.9f
					&& o->CurrentAction == MONSTER01_ATTACK2))
			{
				EVector3  Light;
				Vector(1.0f, 0.2f, 0.0f, Light);

				EVector3 StartPos, StartRelative;
				EVector3 EndPos, EndRelative;

				mu_float fActionSpeed = b->_AnimationHeader[o->CurrentAction].Speed;
				mu_float fSpeedPerFrame = fActionSpeed / 10.0;
				mu_float fAnimationFrame = o->AnimationFrame - fActionSpeed;
				for (mu_int32 i = 0; i < 18; ++i)
				{
					b->Animation(ThreadIndex, ThreadInfo.BoneTransform, fAnimationFrame,
						o->PriorAnimationFrame, o->PriorAction, o->Angle,
						o->HeadAngle);

					Vector(-40.0f, 0.0f, 0.0f, StartRelative);
					Vector(10.0f, 0.0f, 0.0f, EndRelative);

					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(49)], StartRelative, StartPos, false);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(49)], EndRelative, EndPos, false);
					CreateBlur(c, StartPos, EndPos, Light, 0, true, 0);

					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(51)], StartRelative, StartPos, false);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(51)], EndRelative, EndPos, false);
					CreateBlur(c, StartPos, EndPos, Light, 0, true, 1);

					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(50)], StartRelative, StartPos, false);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(50)], EndRelative, EndPos, false);
					CreateBlur(c, StartPos, EndPos, Light, 0, true, 2);

					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(52)], StartRelative, StartPos, false);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(52)], EndRelative, EndPos, false);
					CreateBlur(c, StartPos, EndPos, Light, 0, true, 3);

					fAnimationFrame += fSpeedPerFrame;
				}
			}
		}
		break;
	}
}

void M37Kanturu1st::PlayBGM()
{
	if (GET_WORLD == WD_37KANTURU_1ST)
		EMusicBackend::Play(MUSICS::Data[MUSIC_KANTURU_1ST]);
	else
		EMusicBackend::Stop(MUSICS::Data[MUSIC_KANTURU_1ST]);
}