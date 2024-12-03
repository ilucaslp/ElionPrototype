#include "stdafx.h"
#include "GM3rdChangeUp.h"

SEASON3A::CGM3rdChangeUp g_3rdChangeUp;

SEASON3A::CGM3rdChangeUp::CGM3rdChangeUp()	: m_nDarkElfAppearance(false)
{

}

SEASON3A::CGM3rdChangeUp::~CGM3rdChangeUp()
{

}

mu_boolean SEASON3A::CGM3rdChangeUp::MoveObject(OBJECT* pObject)
{
	PlayEffectSound(pObject);

	EVector3 Light;
	mu_float Luminosity;

	switch (pObject->Type)
	{
	case 2:
	case 5:
	case 58:
	case 59:
	case 60:
		pObject->HiddenMesh = -2;
		break;
	case 3:
		Luminosity = (mu_float)(GetLargeRand(4) + 3) * 0.1f;
		Vector(Luminosity, Luminosity * 0.6f, Luminosity * 0.2f, Light);
		AddTerrainLight(pObject->Position[0], pObject->Position[1], Light, 3, PrimaryTerrainLight);
		pObject->HiddenMesh = -2;
		break;
	case 57:
		pObject->BlendMeshTexCoordV = -IWorldTime % 10000 * 0.0002f;
		break;
	case 84:
		{
			pObject->Position[2] = RequestTerrainHeight(pObject->Position[0], pObject->Position[1])
				+ EMath::Sin(FWorldTime*0.0005f)*150.0f - 100.0f;
		}
		break;
	case 78:
		pObject->Alpha = 0.5f;
	case 85:
	case 86:
	case 87:
	case 88:
	case 89:
	case 90:
	case 91:
	case 92:
	case 93:
		pObject->HiddenMesh = -2;
		break;
	}

	return true;
}

mu_boolean SEASON3A::CGM3rdChangeUp::RenderObjectVisual(const mu_uint32 ThreadIndex, OBJECT* pObject, MUModel* pModel)
{
	EVector3 Light;

	switch (pObject->Type)
	{
	case 2:
		if (GetLargeRand(3) == 0)
		{
			Vector(1.0f, 1.0f, 1.0f, Light);
			CreateParticle(ThreadIndex, BITMAP_SMOKE, pObject->Position, pObject->Angle, Light, 13, pObject->Scale);
		}
		break;
	case 3:
		if (GetLargeRand(3) == 0)
		{
			Vector(1.0f, 1.0f, 1.0f, Light);
			CreateParticle(ThreadIndex, BITMAP_TRUE_FIRE, pObject->Position, pObject->Angle, Light, 0, pObject->Scale);
		}
		break;
	case 5:
		{
			Vector(1.0f, 1.0f, 1.0f, Light);
			if (GetLargeRand(2) == 0)
			{
				if ((mu_int32)((pObject->Timer++) + 2) % 4 == 0)
				{
					CreateParticle(ThreadIndex, BITMAP_ADV_SMOKE + 1, pObject->Position, pObject->Angle, Light);
					CreateParticle(ThreadIndex, BITMAP_ADV_SMOKE, pObject->Position, pObject->Angle, Light, 0);
				}
			}
			if (GetLargeRand(2) == 0)
			{
				if ((mu_int32)(pObject->Timer++) % 4 == 0)
				{
					CreateParticle(ThreadIndex, BITMAP_CLOUD, pObject->Position, pObject->Angle, Light, 6);
					CreateParticle(ThreadIndex, BITMAP_ADV_SMOKE, pObject->Position, pObject->Angle, Light, 1);
				}
			}
		}
		break;
	case 6:
		{
			Vector(1.0f, 1.0f, 1.0f, Light);
			Vector(0.2f, 0.2f, 0.2f, Light);

			if (pObject->HiddenMesh != -2)
			{
				CreateParticle(ThreadIndex, BITMAP_CLOUD, pObject->Position, pObject->Angle, Light, 1, pObject->Scale, pObject);
				CreateParticle(ThreadIndex, BITMAP_CLOUD, pObject->Position, pObject->Angle, Light, 2, pObject->Scale, pObject);
				CreateParticle(ThreadIndex, BITMAP_CLOUD, pObject->Position, pObject->Angle, Light, 3, pObject->Scale, pObject);
				CreateParticle(ThreadIndex, BITMAP_CLOUD, pObject->Position, pObject->Angle, Light, 4, pObject->Scale, pObject);
			}
			pObject->HiddenMesh = -2;
		}
		break;
	case 58:
		Vector(1.0f, 1.0f, 1.0f, Light);
		if (GetLargeRand(2) == 0)
			CreateParticle(ThreadIndex, BITMAP_WATERFALL_1, pObject->Position, pObject->Angle, Light, 2, pObject->Scale);
		break;
	case 59:
		Vector(1.0f, 1.0f, 1.0f, Light);
		if (GetLargeRand(3) == 0)
			CreateParticle(ThreadIndex, BITMAP_WATERFALL_2, pObject->Position, pObject->Angle, Light, 1, pObject->Scale);
		break;
	case 60:
		Vector(1.0f, 1.0f, 1.0f, Light);
		CreateParticle(ThreadIndex, BITMAP_WATERFALL_3, pObject->Position, pObject->Angle, Light, 3, pObject->Scale);
		break;
	case 85:
		if (GetLargeRand(2) == 0)
		{
			if ((mu_int32)((pObject->Timer++) + 2) % 4 == 0)
			{
				if (GetLargeRand(2) == 0)
				{
					Vector(0.0f, 0.0f, 0.0f, Light);
					CreateParticle(ThreadIndex, BITMAP_ADV_SMOKE + 1, pObject->Position, pObject->Angle, Light, 1, pObject->Scale);
				}
				Vector(1.0f, 0.4f, 0.4f, Light);
				CreateParticle(ThreadIndex, BITMAP_ADV_SMOKE, pObject->Position, pObject->Angle, Light, 2, pObject->Scale);
			}
		}
		Vector(1.0f, 0.4f, 0.4f, Light);
		if (GetLargeRand(2) == 0)
		{
			if ((mu_int32)(pObject->Timer++) % 4 == 0)
			{
				CreateParticle(ThreadIndex, BITMAP_CLOUD, pObject->Position, pObject->Angle, Light, 14, pObject->Scale, 0);
				CreateParticle(ThreadIndex, BITMAP_ADV_SMOKE, pObject->Position, pObject->Angle, Light, 2, pObject->Scale * 2);
			}
		}
		break;
	case 88:
		Vector(1.0f, 1.0f, 1.0f, Light);
		if (GetLargeRand(4) == 0)
		{
			CreateParticle(ThreadIndex, BITMAP_CLOUD, pObject->Position, pObject->Angle, Light, 10, pObject->Scale, pObject);
		}
		break;
	case 89:
		if (GetLargeRand(4) == 0)
		{
			mu_float fRed = (GetLargeRand(3))*0.01f + 0.015f;
			Vector(fRed, 0.0f, 0.0f, Light);
			CreateParticle(ThreadIndex, BITMAP_CLOUD, pObject->Position, pObject->Angle, Light, 11, pObject->Scale, pObject);
		}
		break;
	case 90:
		{
			Vector(1.0f, 0.4f, 0.4f, Light);
			EVector3 vAngle;
			if (GetLargeRand(3) == 0)
			{
				Vector((mu_float)(GetLargeRand(40) + 120), 0.0f, (mu_float)(GetLargeRand(30)), vAngle);
				VectorAdd(vAngle, pObject->Angle, vAngle);
				CreateJoint(ThreadIndex, BITMAP_JOINT_SPARK, pObject->Position, pObject->Position, vAngle, 4, pObject, pObject->Scale);
				CreateParticle(ThreadIndex, BITMAP_SPARK, pObject->Position, vAngle, Light, 9, pObject->Scale);
			}
		}
		break;
	case 92:
		{
			Vector(1.0f, 0.4f, 0.4f, Light);
			mu_float fSin = (EMath::Sin(FWorldTime*0.0005f) + 1.0f) * 0.5f;
			if (fSin > 0.9f)
			{
				for (mu_int32 i = 0; i < 2; ++i)
				{
					CreateEffect(ThreadIndex, BITMAP_FIRE_RED, pObject->Position, pObject->Angle, Light, 0, nullptr, -1, 0, pObject->Scale);
				}
			}
		}
		break;
	}

	return true;
}

mu_boolean SEASON3A::CGM3rdChangeUp::RenderObjectMesh(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b, mu_boolean ExtraMon)
{
	switch (o->Type)
	{
	case 79:
		b->StreamMesh(ThreadIndex) = 0;
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, -IWorldTime % 10000 * 0.0001f);
		b->StreamMesh(ThreadIndex) = -1;
		return true;
	case 81:
		{
			mu_float fLumi = (EMath::Sin(FWorldTime*0.002f) + 1.0f) * 0.5f + 0.3f;
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, 0, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		return true;
	case 82:
	case 83:
		{
			mu_float fLumi = (EMath::Sin(FWorldTime*0.0005f)) * 10.0f;
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, 1, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		return true;
	case 57:
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh,
			o->BlendMeshLight, o->BlendMeshTexCoordU,
			o->BlendMeshTexCoordV, o->HiddenMesh);
		return true;
	case 78:
		b->BodyLight(ThreadIndex)[0] = 0.52f;
		b->BodyLight(ThreadIndex)[1] = 0.52f;
		b->BodyLight(ThreadIndex)[2] = 0.52f;

		b->StreamMesh(ThreadIndex) = 0;
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh,
			o->BlendMeshLight, -IWorldTime % 100000 * 0.00001f,
			o->BlendMeshTexCoordV);
		b->StreamMesh(ThreadIndex) = -1;
		return true;
	case 84:
		b->StreamMesh(ThreadIndex) = 0;
		mu_float fLumi = (EMath::Sin(FWorldTime*0.001f) + 1.0f) * 0.5f;
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, 0, fLumi, IWorldTime % 10000 * 0.0001f, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->StreamMesh(ThreadIndex) = -1;
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		return true;
	}

	return false;
}

mu_boolean SEASON3A::CGM3rdChangeUp::CreateFireSnuff(PARTICLE* o)
{
	if (IsBalgasRefugeMap() == true)
	{
		o->Type = BITMAP_FIRE_SNUFF;
		o->Scale = GetLargeRand(50) / 100.0f + 0.4f;
		EVector3 Position;
		Vector(Hero->Object.Position[0] + (mu_float)(GetLargeRand(1600) - 800), Hero->Object.Position[1] + (mu_float)(GetLargeRand(1400) - 500), Hero->Object.Position[2] + (mu_float)(GetLargeRand(300) + 50), Position);

		VectorCopy(Position, o->Position);
		VectorCopy(Position, o->StartPosition);
		o->Velocity[0] = -(mu_float)(GetLargeRand(64) + 64)*0.1f;
		if (Position[1] < MU_CAMERA->GetCameraPosition()[1] + 400.0f)
		{
			o->Velocity[0] = -o->Velocity[0] + 2.2f;
		}
		o->Velocity[1] = (mu_float)(GetLargeRand(32) - 16)*0.1f;
		o->Velocity[2] = (mu_float)(GetLargeRand(32) - 16)*0.1f;
		o->TurningForce[0] = (mu_float)(GetLargeRand(16) - 8)*0.1f;
		o->TurningForce[1] = (mu_float)(GetLargeRand(64) - 32)*0.1f;
		o->TurningForce[2] = (mu_float)(GetLargeRand(16) - 8)*0.1f;
		return true;
	}
	return false;
}

void SEASON3A::CGM3rdChangeUp::PlayEffectSound(OBJECT* o)
{
	switch (o->Type)
	{
	case 74:
	case 75:
		PlayBackground(SOUND_3RD_CHANGE_UP_BG_CAGE1 + GetLargeRand(2));
		break;
	case 79:
		PlayBackground(SOUND_3RD_CHANGE_UP_BG_VOLCANO);
		break;
	case 92:
		{
			mu_float fSin = (EMath::Sin(FWorldTime*0.0005f) + 1.0f) * 0.5f;
			if (fSin > 0.9f)
				PlayBackground(SOUND_3RD_CHANGE_UP_BG_FIREPILLAR);
		}
		break;
	}
}

void SEASON3A::CGM3rdChangeUp::PlayBGM()
{
	if (IsBalgasBarrackMap())
	{
		EMusicBackend::Play(MUSICS::Data[MUSIC_BALGAS_BARRACK]);
	}
	else if (IsBalgasRefugeMap())
	{
		EMusicBackend::Play(MUSICS::Data[MUSIC_BALGAS_REFUGE]);
	}
	else
	{
		EMusicBackend::Stop(MUSICS::Data[MUSIC_BALGAS_BARRACK]);
		EMusicBackend::Stop(MUSICS::Data[MUSIC_BALGAS_REFUGE]);
	}
}

CHARACTER* SEASON3A::CGM3rdChangeUp::CreateBalgasBarrackMonster(mu_int32 iType, mu_int32 PosX, mu_int32 PosY, mu_int32 Key)
{
	if (!(IsBalgasBarrackMap() || IsBalgasRefugeMap() || InDevilSquare()))
		return nullptr;

	CHARACTER* c = nullptr;

	switch (iType)
	{
	case 436:
	case 409:
		{
			c = CreateCharacter(Key, MODEL_MONSTER01 + 91, PosX, PosY);
			c->Object.Scale = 1.25f;
			c->Weapon[0].Type = -1;
			c->Weapon[1].Type = -1;
		}
		break;
	case 410:
		{
			c = CreateCharacter(Key, MODEL_MONSTER01 + 93, PosX, PosY);
			c->Object.Scale = 1.25f;
			c->Weapon[0].Type = -1;
			c->Weapon[1].Type = -1;
		}
		break;
	case 437:
	case 411:
		{
			c = CreateCharacter(Key, MODEL_MONSTER01 + 94, PosX, PosY);
			c->Object.Scale = 1.3f;
			c->Weapon[0].Type = -1;
			c->Weapon[1].Type = -1;
		}
		break;
	case 412:
		{
			m_nDarkElfAppearance = true;

			c = CreateCharacter(Key, MODEL_MONSTER01 + 126, PosX, PosY);
			c->Object.Scale = 1.7f;
			c->Weapon[0].Type = -1;
			c->Weapon[1].Type = -1;
		}
		break;
	}
	return c;
}

mu_boolean SEASON3A::CGM3rdChangeUp::SetCurrentActionBalgasBarrackMonster(CHARACTER* c, OBJECT* o)
{
	if (!(IsBalgasBarrackMap() || IsBalgasRefugeMap()))
		return false;

	switch (c->MonsterIndex)
	{
	case 409:
	case 410:
	case 411:
	case 412:
		return CheckMonsterSkill(c, o);
	}
	return false;
}

mu_boolean SEASON3A::CGM3rdChangeUp::AttackEffectBalgasBarrackMonster(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b)
{
	if (!(IsBalgasBarrackMap() || IsBalgasRefugeMap()))
		return false;

	switch (o->Type)
	{
	case MODEL_MONSTER01 + 91:
		if (c->AttackTime == 14)
		{
			CreateEffect(ThreadIndex, MODEL_ARROW_HOLY, o->Position, o->Angle, o->Light, 1, o, o->PKKey);
			return true;
		}
		break;
	}

	return false;
}

mu_boolean SEASON3A::CGM3rdChangeUp::MoveBalgasBarrackMonsterVisual(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b)
{
	switch (o->Type)
	{
	case MODEL_MONSTER01 + 91:
		{
			EVector3 Light;
			Vector(0.9f, 0.2f, 0.1f, Light);
			if (o->CurrentAction == MONSTER01_WALK || o->CurrentAction == MONSTER01_RUN)
			{
				if (GetLargeRand(10) == 0)
				{
					CreateParticle(ThreadIndex, BITMAP_SMOKE + 1, o->Position, o->Angle, Light);
				}
			}
		}
		break;
	case MODEL_MONSTER01 + 93:
		{
			EVector3 Light;
			Vector(0.9f, 0.2f, 0.1f, Light);
			if (o->CurrentAction == MONSTER01_WALK || o->CurrentAction == MONSTER01_RUN)
			{
				if (GetLargeRand(10) == 0)
				{
					CreateParticle(ThreadIndex, BITMAP_SMOKE + 1, o->Position, o->Angle, Light);
				}
			}
		}
		break;
	case MODEL_MONSTER01 + 94:
		{
			mu_float fActionSpeed = b->_AnimationHeader[o->CurrentAction].Speed;
			EVector3 Light;
			EVector3 EndPos, EndRelative;
			Vector(1.0f, 1.0f, 1.0f, Light);


			if (o->CurrentAction == MONSTER01_ATTACK1)
			{
				if (o->AnimationFrame >= 6.5f && o->AnimationFrame < (6.5f + fActionSpeed))
				{
					Vector(0.0f, 0.0f, 0.0f, EndRelative);
					b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(27)], EndRelative, EndPos, true);
					CreateEffect(ThreadIndex, BITMAP_CRATER, EndPos, o->Angle, o->Light, 2);
					CreateParticle(ThreadIndex, BITMAP_BLUE_BLUR, EndPos, o->Angle, Light, 1);
					CreateParticle(ThreadIndex, BITMAP_BLUE_BLUR, EndPos, o->Angle, Light, 1);
					Vector(1.0f, 0.6f, 0.4f, Light);
					CreateEffect(ThreadIndex, BITMAP_SHOCK_WAVE, EndPos, o->Angle, Light, 8);
					CreateEffect(ThreadIndex, BITMAP_SHOCK_WAVE, EndPos, o->Angle, Light, 8);
					CreateEffect(ThreadIndex, BITMAP_SHOCK_WAVE, EndPos, o->Angle, Light, 8);
					CreateEffect(ThreadIndex, BITMAP_SHOCK_WAVE, EndPos, o->Angle, Light, 8);
					CreateEffect(ThreadIndex, BITMAP_SHOCK_WAVE, EndPos, o->Angle, Light, 8);

					for (mu_int32 iu = 0; iu < 4; ++iu)
					{
						CreateEffect(ThreadIndex, MODEL_BIG_STONE1, EndPos, o->Angle, o->Light, 10);
						CreateEffect(ThreadIndex, MODEL_STONE2, EndPos, o->Angle, o->Light);
					}
				}
			}
			else
				if (o->CurrentAction == MONSTER01_WALK || o->CurrentAction == MONSTER01_RUN)
				{
					if (GetLargeRand(10) == 0)
					{
						CreateParticle(ThreadIndex, BITMAP_SMOKE + 1, o->Position, o->Angle, Light);
					}
				}
		}
		break;
	case MODEL_MONSTER01 + 126:
		if (m_nDarkElfAppearance)
		{
			m_nDarkElfAppearance = false;

			EVector3 Light;
			EVector3 EndPos, EndRelative;

			Vector(1.0f, 0.2f, 0.2f, Light);
			Vector(0.0f, 0.0f, 0.0f, EndRelative);
			b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(27)], EndRelative, EndPos, true);
			CreateEffect(ThreadIndex, BITMAP_CRATER, EndPos, o->Angle, o->Light, 2);
			CreateParticle(ThreadIndex, BITMAP_BLUE_BLUR, EndPos, o->Angle, Light, 1);
			CreateParticle(ThreadIndex, BITMAP_BLUE_BLUR, EndPos, o->Angle, Light, 1);
			Vector(1.0f, 0.2f, 0.2f, Light);
			CreateEffect(ThreadIndex, BITMAP_SHOCK_WAVE, EndPos, o->Angle, Light, 8);
			CreateEffect(ThreadIndex, BITMAP_SHOCK_WAVE, EndPos, o->Angle, Light, 8);
			CreateEffect(ThreadIndex, BITMAP_SHOCK_WAVE, EndPos, o->Angle, Light, 8);
			CreateEffect(ThreadIndex, BITMAP_SHOCK_WAVE, EndPos, o->Angle, Light, 8);
			CreateEffect(ThreadIndex, BITMAP_SHOCK_WAVE, EndPos, o->Angle, Light, 8);
		}
		break;
	}
	return false;
}

void SEASON3A::CGM3rdChangeUp::MoveBalgasBarrackBlurEffect(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	EVector3 Angle, Position;
	EBone Matrix;
	EVector3 p, p2, EndPos;
	EVector3 TempAngle;

	switch (o->Type)
	{
	case MODEL_MONSTER01 + 93:
		{
			if ((o->CurrentAction == MONSTER01_ATTACK1 || o->CurrentAction == MONSTER01_ATTACK2))
			{
				EVector3  Light;
				Vector(0.2f, 1.0f, 0.4f, Light);

				EVector3 StartPos, StartRelative;
				EVector3 EndPos, EndRelative;

				mu_float fActionSpeed = b->_AnimationHeader[o->CurrentAction].Speed;
				mu_float fSpeedPerFrame = fActionSpeed / 10.0;
				mu_float fAnimationFrame = o->AnimationFrame - fActionSpeed;

				VectorCopy(o->Angle, TempAngle);
				for (mu_int32 i = 0; i < 10; ++i)
				{
					b->Animation(ThreadIndex, ThreadInfo.BoneTransform, fAnimationFrame, o->PriorAnimationFrame, o->PriorAction, o->Angle, o->HeadAngle);

					Vector(0.0f, 250.0f, 0.0f, StartRelative);
					Vector(0.0f, 0.0f, 0.0f, EndRelative);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(27)], StartRelative, StartPos, false);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(27)], EndRelative, EndPos, false);
					CreateBlur(c, StartPos, EndPos, Light, 3, true, 80);


					fAnimationFrame += fSpeedPerFrame;
				}
				VectorCopy(TempAngle, o->Angle);

				if (o->AnimationFrame > 4.5f && o->AnimationFrame < 5.0f)
				{
					CHARACTER *tc = &CharactersClient[c->TargetCharacter];
					OBJECT *to = &tc->Object;
					EVector3 Angle = { 0.0f, 0.0f, o->Angle[2] };
					EVector3 Pos = { 0.0f, 0.0f, (to->BoundingBoxMax[2] / 1.0f) };

					Vector(80.0f, 0.0f, 20.0f, p);
					b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(0)], p, Position, true);
					Position[2] += 50.0f;
					Angle[2] = o->Angle[2] + 90;
					CreateEffect(ThreadIndex, MODEL_DEATH_SPI_SKILL, Position, Angle, Pos, 0, to);
					CreateEffect(ThreadIndex, MODEL_DEATH_SPI_SKILL, Position, Angle, Pos, 1, to);
					Angle[2] = o->Angle[2] - 90;
					CreateEffect(ThreadIndex, MODEL_DEATH_SPI_SKILL, Position, Angle, Pos, 0, to);
					CreateEffect(ThreadIndex, MODEL_DEATH_SPI_SKILL, Position, Angle, Pos, 1, to);
				}
			}
		}
		break;
	case MODEL_MONSTER01 + 94:
		{
			EVector3  Light;
			Vector(1.0f, 1.0f, 1.0f, Light);

			if (o->CurrentAction == MONSTER01_ATTACK2)
			{
				EVector3 EndRelative;

				mu_float fActionSpeed = b->_AnimationHeader[o->CurrentAction].Speed;
				mu_float fSpeedPerFrame = fActionSpeed / 10.0;
				mu_float fAnimationFrame = o->AnimationFrame - fActionSpeed;
				VectorCopy(o->Angle, TempAngle);
				for (mu_int32 i = 0; i < 10; ++i)
				{
					b->Animation(ThreadIndex, ThreadInfo.BoneTransform, fAnimationFrame, o->PriorAnimationFrame, o->PriorAction, o->Angle, o->HeadAngle);

					Vector(0.0f, 100.0f, -150.0f, EndRelative);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(16)], EndRelative, EndPos, false);

					if (o->AnimationFrame > 5.0f && o->AnimationFrame < 7.0f)
					{
						CreateParticle(ThreadIndex, BITMAP_FIRE, EndPos, o->Angle, Light);
					}

					Vector(0.0f, -150.0f, 0.0f, p);
					AngleMatrix(o->Angle, Matrix);
					VectorRotate(p, Matrix, Position);
					VectorAdd(Position, o->Position, p2);
					o->Angle[2] -= 18;
					Vector((mu_float)(GetLargeRand(60) + 60 - 90), 0.0f, (mu_float)(GetLargeRand(30) + 90), Angle);
					VectorAdd(Angle, o->Angle, Angle);
					VectorCopy(p2, Position);

					fAnimationFrame += fSpeedPerFrame;
				}
				VectorCopy(TempAngle, o->Angle);
			}
		}
		break;
	case MODEL_MONSTER01 + 126:
		{
			if ((o->CurrentAction == MONSTER01_ATTACK1 || o->CurrentAction == MONSTER01_ATTACK2))
			{
				EVector3  Light;
				Vector(1.0f, 1.0f, 1.0f, Light);

				EVector3 StartPos, StartRelative;
				EVector3 EndPos, EndRelative;

				mu_float fActionSpeed = b->_AnimationHeader[o->CurrentAction].Speed;
				mu_float fSpeedPerFrame = fActionSpeed / 10.0;
				mu_float fAnimationFrame = o->AnimationFrame - fActionSpeed;

				if (o->CurrentAction == MONSTER01_ATTACK2 && (o->AnimationFrame > 4.5f && o->AnimationFrame < 5.0f))
					CreateEffect(ThreadIndex, MODEL_DARK_ELF_SKILL, o->Position, o->Angle, o->Light, 2, o);

				VectorCopy(o->Angle, TempAngle);
				for (mu_int32 i = 0; i < 10; ++i)
				{
					b->Animation(ThreadIndex, ThreadInfo.BoneTransform, fAnimationFrame, o->PriorAnimationFrame, o->PriorAction, o->Angle, o->HeadAngle);

					Vector(0.0f, 0.0f, -60.0f, StartRelative);
					Vector(0.0f, 0.0f, -150.0f, EndRelative);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[c->Weapon[0].LinkBone], StartRelative, StartPos, false);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[c->Weapon[0].LinkBone], EndRelative, EndPos, false);
					CreateBlur(c, StartPos, EndPos, Light, 3, true, 80);


					fAnimationFrame += fSpeedPerFrame;
				}
				VectorCopy(TempAngle, o->Angle);
			}
			else if (o->CurrentAction == MONSTER01_ATTACK3)
			{

				EVector3 Position, Light;
				BoneManager::GetBonePosition(o, 17, Position);

				Vector(0.2f, 0.2f, 0.7f, Light);
				CreateParticle(ThreadIndex, BITMAP_SMOKE, Position, o->Angle, Light, 27, 1.0f);
				Vector(0.3f, 0.3f, 0.4f, Light);
				CreateParticle(ThreadIndex, BITMAP_LIGHT + 1, Position, o->Angle, Light, 2, 0.8f);
				CreateParticle(ThreadIndex, BITMAP_LIGHT + 1, Position, o->Angle, Light, 2, 0.6f);
			}
		}
		break;
	}
}

mu_boolean SEASON3A::CGM3rdChangeUp::RenderMonsterObjectMesh(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b, mu_int32 ExtraMon)
{
	switch (o->Type)
	{
	case MODEL_MONSTER01 + 91:
		{
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, 5);
			return true;
		}
		break;
	case MODEL_MONSTER01 + 93:
		{
			for (mu_int32 i = 0; i < b->_MeshCount; ++i)
			{
				if (i == 2)
				{
					if (o->Alpha > 0.3f)
						o->Alpha = 0.3f;

					o->BlendMesh = -2;
					b->StreamMesh(ThreadIndex) = i;
				}
				else
					o->BlendMesh = -1;

				o->BlendMeshTexCoordU = IWorldTime % 10000 * 0.0005f;
				b->RenderMesh(ThreadIndex, i, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			}
			o->Alpha = 1.0f;
			return true;
		}
		break;
	case MODEL_MONSTER01 + 94:
		{
			Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, 5);
			return true;
		}
		break;
	case MODEL_MONSTER01 + 126:
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 0, RENDER_BRIGHT | RENDER_CHROME, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 0, RENDER_BRIGHT | RENDER_CHROME6, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 1, RENDER_BRIGHT | RENDER_CHROME, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 1, RENDER_BRIGHT | RENDER_CHROME6, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		return true;
	}
	return false;
}

mu_boolean SEASON3A::CGM3rdChangeUp::RenderBalgasBarrackMonsterVisual(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b)
{
	switch (o->Type)
	{
	case MODEL_MONSTER01 + 91:
		if (o->CurrentAction == MONSTER01_WALK || o->CurrentAction == MONSTER01_RUN)
		{
			if (GetLargeRand(15) == 0)
				PlayBackground(SOUND_CRY1ST_BALRAM_MOVE1 + GetLargeRand(2));
		}
		else if (o->CurrentAction == MONSTER01_ATTACK1)
		{
			if (o->SubType == 0)
			{
				o->SubType = 1;
				PlayBackground(SOUND_CRY1ST_BALRAM_ATTACK1);
			}
		}
		else if (o->CurrentAction == MONSTER01_ATTACK2)
		{
			if (o->SubType == 0)
			{
				o->SubType = 1;
				PlayBackground(SOUND_CRY1ST_BALRAM_ATTACK2);
			}
		}
		else if (o->CurrentAction == MONSTER01_DIE)
		{
			if (o->SubType == 0)
			{
				o->SubType = 1;
				PlayBackground(SOUND_CRY1ST_BALRAM_DIE);
			}
		}
		if (o->CurrentAction == MONSTER01_STOP1 || o->CurrentAction == MONSTER01_STOP2)
			o->SubType = 0;
		return true;

	case MODEL_MONSTER01 + 93:
		{
			if (o->CurrentAction == MONSTER01_WALK || o->CurrentAction == MONSTER01_RUN)
			{
				if (GetLargeRand(15) == 0)
					PlayBackground(SOUND_CRY1ST_DEATHSPIRIT_MOVE1 + GetLargeRand(2));
			}
			else if (o->CurrentAction == MONSTER01_ATTACK1)
			{
				if (o->SubType == 0)
				{
					o->SubType = 1;
					PlayBackground(SOUND_CRY1ST_DEATHSPIRIT_ATTACK1);
				}
			}
			else if (o->CurrentAction == MONSTER01_ATTACK2)
			{
				if (o->SubType == 0)
				{
					o->SubType = 1;
					PlayBackground(SOUND_CRY1ST_DEATHSPIRIT_ATTACK2);
				}
			}
			else if (o->CurrentAction == MONSTER01_DIE)
			{
				if (o->SubType == 0)
				{
					o->SubType = 1;
					PlayBackground(SOUND_CRY1ST_DEATHSPIRIT_DIE);
				}
			}

			if (o->CurrentAction == MONSTER01_STOP1 || o->CurrentAction == MONSTER01_STOP2)
				o->SubType = 0;

			EVector3 Position, Light;
			mu_int32 dummy = GetLargeRand(14);
			mu_float Data = (mu_float)((mu_float)dummy / (mu_float)100);
			mu_float Rot = (mu_float)(GetLargeRand(360));
			Vector(1.0f, 1.0f, 1.0f, Light);
			BoneManager::GetBonePosition(o, 27, Position);
			CreateSprite(BITMAP_DS_EFFECT, Position, 1.5f, Light, o);
			Vector(0.3f, 0.3f, 0.7f, Light);
			CreateSprite(BITMAP_LIGHT, Position, 3.5f + (Data * 5), Light, o);
			CreateSprite(BITMAP_LIGHT, Position, 3.5f + (Data * 5), Light, o);
			CreateSprite(BITMAP_LIGHT, Position, 3.5f + (Data * 5), Light, o);
			CreateSprite(BITMAP_DS_SHOCK, Position, 0.8f + Data, Light, o, Rot);

			BoneManager::GetBonePosition(o, 28, Position);
			Vector(0.1f, 0.0f, 0.6f, Light);
			CreateSprite(BITMAP_SHINY + 1, Position, 0.8f, Light, o, Rot);
			if (GetLargeRand(2) == 0)
			{
				Vector(0.7f, 0.7f, 1.0f, Light);
				CreateSprite(BITMAP_SHINY + 1, Position, 0.8f, Light, o, 360.0f - Rot);
			}
			Vector(0.3f, 0.3f, 0.7f, Light);
			CreateSprite(BITMAP_LIGHT, Position, 1.5f, Light, o);
		}
		return true;
	case MODEL_MONSTER01 + 94:
		if (o->CurrentAction == MONSTER01_WALK || o->CurrentAction == MONSTER01_RUN)
		{
			if (GetLargeRand(15) == 0)
				PlayBackground(SOUND_CRY1ST_SORAM_MOVE1 + GetLargeRand(2));
		}
		else if (o->CurrentAction == MONSTER01_ATTACK1)
		{
			if (o->SubType == 0)
			{
				o->SubType = 1;
				PlayBackground(SOUND_CRY1ST_SORAM_ATTACK1);
			}
		}
		else if (o->CurrentAction == MONSTER01_ATTACK2)
		{
			if (o->SubType == 0)
			{
				o->SubType = 1;
				PlayBackground(SOUND_CRY1ST_SORAM_ATTACK2);
			}
		}
		else if (o->CurrentAction == MONSTER01_DIE)
		{
			if (o->SubType == 0)
			{
				o->SubType = 1;
				PlayBackground(SOUND_CRY1ST_SORAM_DIE);
			}
		}
		if (o->CurrentAction == MONSTER01_STOP1 || o->CurrentAction == MONSTER01_STOP2)
			o->SubType = 0;
		return true;
	case MODEL_MONSTER01 + 126:
		{
			if (o->CurrentAction == MONSTER01_WALK || o->CurrentAction == MONSTER01_RUN)
			{
				if (GetLargeRand(15) == 0)
					PlayBackground(SOUND_CRY1ST_DARKELF_MOVE1 + GetLargeRand(2));
			}
			else if (o->CurrentAction == MONSTER01_ATTACK1)
			{
				if (o->SubType == 0)
				{
					o->SubType = 1;
					PlayBackground(SOUND_CRY1ST_DARKELF_ATTACK1);
				}
			}
			else if (o->CurrentAction == MONSTER01_ATTACK2)
			{
				if (o->SubType == 0)
				{
					o->SubType = 1;
					PlayBackground(SOUND_CRY1ST_DARKELF_ATTACK2);
				}
			}
			else if (o->CurrentAction == MONSTER01_ATTACK3)
			{
				if (o->SubType == 0)
				{
					o->SubType = 1;
					PlayBackground(SOUND_CRY1ST_DARKELF_SKILL1);
				}
			}
			else if (o->CurrentAction == MONSTER01_ATTACK4)
			{
				if (o->SubType == 0)
				{
					o->SubType = 1;
					PlayBackground(SOUND_CRY1ST_DARKELF_SKILL2);
				}
			}
			else if (o->CurrentAction == MONSTER01_DIE)
			{
				if (o->SubType == 0)
				{
					o->SubType = 1;
					PlayBackground(SOUND_CRY1ST_DARKELF_DIE);
				}
			}

			if (o->CurrentAction == MONSTER01_STOP1 || o->CurrentAction == MONSTER01_STOP2)
				o->SubType = 0;

			EVector3 vRelativePos, vPos, vLight;
			Vector(0.0f, 0.0f, 0.0f, vRelativePos);
			Vector(0.0f, 0.0f, 0.0f, vPos);
			Vector(0.6f, 0.6f, 0.9f, vLight);
			mu_int32 iBoneThunder[] = { 6, 15, 27, 17, 29, 3, 34, 44, 45, 40 };

			if (GetLargeRand(2) == 0)
			{
				for (mu_int32 i = 0; i < 10; ++i)
				{
					b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(iBoneThunder[i])], vRelativePos, vPos, true);
					if (GetLargeRand(2) == 0)
					{
						CreateEffect(ThreadIndex, MODEL_FENRIR_THUNDER, vPos, o->Angle, vLight, 1, o);
					}
				}
			}
		}
		return true;
	}
	return false;
}