#include "stdafx.h"
#include "GMDoppelGanger2.h"

CGMDoppelGanger2 g_DoppelGanger2;

CGMDoppelGanger2::CGMDoppelGanger2()
{

}

CGMDoppelGanger2::~CGMDoppelGanger2()
{

}

mu_boolean CGMDoppelGanger2::CreateObject(mu_uint32 objectIndex, OBJECT* o)
{
	if(o->Type>=0 && o->Type<=6)
	{
		o->CollisionRange = -300;
		return true;
	}

	return false;
}

mu_boolean CGMDoppelGanger2::MoveObject(OBJECT* o)
{
	switch(o->Type)
	{
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 47:
	case 48:
		o->HiddenMesh = -2;
		return true;
	default:
		break;
	}

	return false;
}

mu_boolean CGMDoppelGanger2::MoveMonsterVisual(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b)
{
	switch (o->Type)
	{
	case MODEL_MONSTER01 + 145:
		{
			switch (o->CurrentAction)
			{
			case MONSTER01_ATTACK2:
				if (o->AnimationFrame > 4.4f && o->AnimationFrame < 4.7f)
				{
					const mu_float ANG_REVISION = 20.0f;

					EVector3 v3Pos, v3Ang_, v3BasisPos;

					b->TransformByObjectBone(ThreadIndex, v3BasisPos, o, 8);

					VectorCopy(v3BasisPos, v3Pos);

					VectorCopy(o->Angle, v3Ang_);
					v3Ang_[0] = v3Ang_[0] + ANG_REVISION;

					CreateEffect(ThreadIndex, MODEL_STREAMOFICEBREATH, v3Pos, o->Angle, o->Light, 0, 0, -1, 0, 0, 0, 0.2f, -1);
				}
				break;
			case MONSTER01_DIE:
				{
					EVector3 vPos, vRelative;
					Vector(0.0f, 0.0f, 0.0f, vRelative);

					mu_float Scale = 3.5f;
					Vector(1.0f, 1.0f, 1.0f, o->Light);
					b->TransformByObjectBone(ThreadIndex, vPos, o, 6);
					CreateParticle(ThreadIndex, BITMAP_SMOKE, vPos, o->Angle, o->Light, 3, Scale);
					b->TransformByObjectBone(ThreadIndex, vPos, o, 79);
					CreateParticle(ThreadIndex, BITMAP_SMOKE, vPos, o->Angle, o->Light, 53, Scale);
				}
				break;
			}
		}
		return true;
	case MODEL_MONSTER01 + 189:
	case MODEL_MONSTER01 + 190:
		{
			if (o->CurrentAction == MONSTER01_STOP1 || o->CurrentAction == MONSTER01_STOP2 || o->CurrentAction == MONSTER01_ATTACK1 || o->CurrentAction == MONSTER01_ATTACK2)
				o->CurrentAction = MONSTER01_WALK;
		}
		return true;
	case MODEL_MONSTER01 + 191:
		if (o->CurrentAction == MONSTER01_APEAR)
		{
			if (o->AnimationFrame > 18.0f)
			{
				if (o->Flags.Get(OBJECT::eOF_ACTIONSTART) == false)
				{
					o->Flags.Set(OBJECT::eOF_ACTIONSTART);

					EVector3 vPos, vLight;
					mu_int32 i;

					for (i = 0; i < 6; ++i)
					{
						VectorCopy(o->Position, vPos);
						vPos[0] += (mu_float)(GetLargeRand(140) - 70);
						vPos[1] += (mu_float)(GetLargeRand(140) - 70);
						Vector(0.2f, 1.0f, 0.3f, vLight);
						CreateEffect(ThreadIndex, BITMAP_CLOUD, vPos, o->Angle, vLight, 0, NULL, -1, 0, 0, 0, 2.0f);
					}

					Vector(0.4f, 1.0f, 0.6f, vLight);
					for (i = 0; i < 3; ++i)
					{
						vPos[0] = o->Position[0] + (GetLargeRand(100) - 50)*1.0f;
						vPos[1] = o->Position[1] + (GetLargeRand(100) - 50)*1.0f;
						vPos[2] = o->Position[2] + 10.0f + (GetLargeRand(20))*10.0f;
						CreateParticle(ThreadIndex, BITMAP_EXPLOTION_MONO, vPos, o->Angle, vLight, 0, (GetLargeRand(8) + 7)*0.1f);
					}

					Vector(0.0f, 0.5f, 0.0f, vLight);
					for (i = 0; i < 15; ++i)
					{
						vPos[0] = o->Position[0] + (GetLargeRand(200) - 100)*1.0f;
						vPos[1] = o->Position[1] + (GetLargeRand(200) - 100)*1.0f;
						vPos[2] = o->Position[2] + (GetLargeRand(10))*10.0f;
						CreateEffect(ThreadIndex, MODEL_DOPPELGANGER_SLIME_CHIP, vPos, o->Angle, vLight, 0, o, 0, 0);
					}

					Vector(0.2f, 0.9f, 0.3f, vLight);
					for (i = 0; i < 30; ++i)
					{
						vPos[0] = o->Position[0] + (GetLargeRand(300) - 150)*1.0f;
						vPos[1] = o->Position[1] + (GetLargeRand(300) - 150)*1.0f;
						vPos[2] = o->Position[2] + 20.0f + (GetLargeRand(10))*10.0f;
						CreateParticle(ThreadIndex, BITMAP_SPARK + 1, vPos, o->Angle, vLight, 31);
					}

					Vector(0.8f, 1.0f, 0.8f, vLight);
					CreateParticle(ThreadIndex, BITMAP_SMOKE, o->Position, o->Angle, vLight, 54, 2.8f);

					SetAction(o, MONSTER01_ATTACK1);
				}
			}
		}
		return true;
	}

	return false;
}

void CGMDoppelGanger2::MoveBlurEffect(const mu_uint32 ThreadIndex, CHARACTER* pCharacter, OBJECT* pObject, MUModel* pModel)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	switch(pObject->Type)
	{
	case MODEL_MONSTER01+189:
	case MODEL_MONSTER01+190:
		{
			if (!(pObject->CurrentAction == MONSTER01_WALK || pObject->CurrentAction == MONSTER01_ATTACK1 || pObject->CurrentAction == MONSTER01_ATTACK2))
				break;

			EVector3  vLight;
			Vector(0.6f, 0.4f, 0.2f, vLight);

			EVector3 StartPos, StartRelative;
			EVector3 EndPos, EndRelative;
			
			mu_float fActionSpeed = pModel->_AnimationHeader[pObject->CurrentAction].Speed;
			mu_float fSpeedPerFrame = fActionSpeed/10.0;
			mu_float fAnimationFrame = pObject->AnimationFrame - fActionSpeed;
			for(mu_int32 i=0; i<10; ++i) 
			{
				pModel->Animation(ThreadIndex, ThreadInfo.BoneTransform, fAnimationFrame, pObject->PriorAnimationFrame, pObject->PriorAction, pObject->Angle, pObject->HeadAngle);

				Vector(0.0f, 0.0f, 0.0f, StartRelative);
				Vector(0.0f, 0.0f, 0.0f, EndRelative);
				pModel->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[pModel->GetBoneIndex(33)], StartRelative, StartPos, false);
				pModel->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[pModel->GetBoneIndex(34)], EndRelative, EndPos, false);
				CreateBlur(pCharacter, StartPos, EndPos, vLight, 0, false, 0);
				
				fAnimationFrame += fSpeedPerFrame;
			}
		}
		break;
	}
}

mu_boolean CGMDoppelGanger2::RenderObjectMesh(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b, mu_boolean ExtraMon)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	mu_float fBlendMeshLight = 0;

	switch (o->Type)
	{
	case 10:
	case 19:
	case 20:
	case 31:
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		return true;
	case 33:
		b->StreamMesh(ThreadIndex) = 0;
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, IWorldTime % 10000 * 0.0001f);
		b->StreamMesh(ThreadIndex) = -1;
		return true;
	case 15:
		{
			b->StreamMesh(ThreadIndex) = 0;
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, -IWorldTime % 10000 * 0.0001f);
			b->StreamMesh(ThreadIndex) = -1;
			return true;
		}
	case 16:
		{
			mu_float fLumi = (EMath::Sin(FWorldTime*0.002f) + 1.0f) * 0.5f;
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, 0, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			return true;
		}
	case 67:
		{
			b->StreamMesh(ThreadIndex) = 1;
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, IWorldTime % 10000 * 0.0001f);
			b->StreamMesh(ThreadIndex) = -1;
			EVector3 light;
			Vector(1.0f, 0.0f, 0.0f, light);
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			VectorCopy(light, b->BodyLight(ThreadIndex));
			b->RenderMesh(ThreadIndex, 0, RENDER_BRIGHT | RENDER_CHROME, 0.5f, 0, 0.5f, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);

			if(MU_UPDATECOUNTER > 0)
			{
				EVector3 vLightFire, Position, vPos;
				Vector(1.0f, 0.0f, 0.0f, vLightFire);
				Vector(0.0f, 0.0f, 0.0f, vPos);

				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(6)], vPos, Position, false);
				CreateSprite(BITMAP_LIGHT, Position, o->Scale*5.0f, vLightFire, o);

				Vector(0.0f, 0.0f, -350.0f, vPos);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(6)], vPos, Position, false);
				CreateSprite(BITMAP_LIGHT, Position, o->Scale*5.0f, vLightFire, o);

				if (o->AnimationFrame >= 35 && o->AnimationFrame <= 37)
				{
					o->PKKey = -1;
				}

				if (o->AnimationFrame >= 1 && o->AnimationFrame <= 2 && o->PKKey != 1)
				{
					o->AnimationFrame = 1;

					mu_int32 test = GetLargeRand(1000);
					if (test >= 0 && test < 2)
					{
						o->PKKey = 1;
					}
					else
					{
						o->PKKey = -1;
					}
				}
				EVector3 p, Pos, Light;
				Vector(0.4f, 0.1f, 0.1f, Light);
				Vector(-150.0f, 0.0f, 0.0f, p);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(4)], p, Pos, false);
				if (o->AnimationFrame >= 35.0f && o->AnimationFrame < 50.0f)
					CreateParticle(ThreadIndex, BITMAP_SMOKE, Pos, o->Angle, Light, 24, o->Scale*1.5f);
			}
			return true;
		}
	case 68:
		{
			b->StreamMesh(ThreadIndex) = 1;
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, IWorldTime % 10000 * 0.0001f);
			b->StreamMesh(ThreadIndex) = -1;

			EVector3 light;
			Vector(1.0f, 0.0f, 0.0f, light);
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			VectorCopy(light, b->BodyLight(ThreadIndex));
			b->RenderMesh(ThreadIndex, 0, RENDER_BRIGHT | RENDER_CHROME, 0.5f, 0, 0.5f, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);

			if(MU_UPDATECOUNTER > 0)
			{
				EVector3 vLightFire, Position, vPos;
				Vector(1.0f, 0.0f, 0.0f, vLightFire);
				Vector(0.0f, 0.0f, 0.0f, vPos);

				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(6)], vPos, Position, false);
				CreateSprite(BITMAP_LIGHT, Position, o->Scale*5.0f, vLightFire, o);

				Vector(0.0f, 0.0f, -350.0f, vPos);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(6)], vPos, Position, false);
				CreateSprite(BITMAP_LIGHT, Position, o->Scale*5.0f, vLightFire, o);

				EVector3 p, Pos, Light;
				Vector(0.3f, 0.1f, 0.1f, Light);
				Vector(GetLargeRand(20) - 30.0f, GetLargeRand(20) - 30.0f, 0.0f, p);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(4)], p, Pos, false);
				if (o->AnimationFrame >= 7.0f && o->AnimationFrame < 13.0f)
					CreateParticle(ThreadIndex, BITMAP_SMOKE, Pos, o->Angle, Light, 18, o->Scale*1.5f);
			}
			return true;
		}
	case 72:
		{
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			mu_float fLumi = (EMath::Sin(FWorldTime*0.003f) + 1.0f) * 0.5f * 0.5f + 0.5f;
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, 1, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			return true;
		}
	case MODEL_MONSTER01 + 145:
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_DOPPELGANGER_ICEWALKER0);
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_DOPPELGANGER_ICEWALKER1);
		return true;
	case MODEL_MONSTER01 + 6:
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_DOPPELGANGER_SNAKE01);
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		return true;
	case MODEL_MONSTER01 + 189:
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		fBlendMeshLight = (EMath::Sin(FWorldTime*0.003f) + 1.0f)*0.5f*0.8f;
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, fBlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_BUGBEAR_R);
		return true;
	case MODEL_MONSTER01 + 190:
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		fBlendMeshLight = (EMath::Sin(FWorldTime*0.003f) + 1.0f)*0.5f*0.8f;
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, fBlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_BUGBEAR_R);
		return true;
	case MODEL_MONSTER01 + 191:
		if (o->Flags.Get(OBJECT::eOF_ACTIONSTART) == false)
		{
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 0, RENDER_CHROME | RENDER_BRIGHT, 0.3f, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		return true;
	}

	return false;
}

mu_boolean CGMDoppelGanger2::RenderObjectVisual(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b)
{
	EVector3 Light;

	switch (o->Type)
	{
	case 0:
		{
			o->HiddenMesh = -2;
			mu_float fLumi = ((EMath::Sin(FWorldTime*0.001f) + 1.0f) * 0.5f) * 100.0f;

			mu_int32 nRanDelay = o->Position[0];
			nRanDelay = nRanDelay % 3 + 1;
			mu_int32 nRanTemp = 30;
			nRanTemp = nRanTemp * nRanDelay;
			mu_int32 nRanGap = 10;
			if (nRanTemp != 90.0f)
			{
				nRanGap = 40;
			}

			if (fLumi >= nRanTemp && fLumi <= nRanTemp + nRanGap)
			{
				Vector(1.0f, 1.0f, 1.0f, Light);
				for (mu_int32 i = 0; i < 20; ++i)
				{
					CreateParticle(ThreadIndex, BITMAP_WATERFALL_2, o->Position, o->Angle, Light, 6, o->Scale, o);
				}
			}
		}
		return true;
	case 1:
		{
			o->HiddenMesh = -2;
			if (GetLargeRand(3) == 0)
			{
				Vector(1.0f, 1.0f, 1.0f, Light);
				CreateParticle(ThreadIndex, BITMAP_SMOKE, o->Position, o->Angle, Light, 60, o->Scale, o);
			}
		}
		return true;
	case 2:
		{
			o->HiddenMesh = -2;
			EVector3  Light;
			if (GetLargeRand(3) == 0)
			{
				Vector(0.0f, 0.0f, 0.0f, Light);
				CreateParticle(ThreadIndex, BITMAP_CLOUD, o->Position, o->Angle, Light, 16, o->Scale, o);
			}
		}
		return true;
	case 3:
		{
			o->HiddenMesh = -2;
			if (GetLargeRand(4) == 0)
			{
				mu_float fRed = (GetLargeRand(3))*0.01f + 0.015f;
				Vector(fRed, 0.0f, 0.0f, Light);
				CreateParticle(ThreadIndex, BITMAP_CLOUD, o->Position, o->Angle, Light, 11, o->Scale, o);
			}
		}
		return true;
	case 4:
		{
			o->HiddenMesh = -2;
			Vector(1.0f, 0.4f, 0.4f, Light);
			EVector3 vAngle;
			if (GetLargeRand(3) == 0)
			{
				Vector((mu_float)(GetLargeRand(40) + 120), 0.0f, (mu_float)(GetLargeRand(30)), vAngle);
				VectorAdd(vAngle, o->Angle, vAngle);
				CreateJoint(ThreadIndex, BITMAP_JOINT_SPARK, o->Position, o->Position, vAngle, 4, o, o->Scale);
				CreateParticle(ThreadIndex, BITMAP_SPARK, o->Position, vAngle, Light, 9, o->Scale);
			}
		}
		return true;
	case 5:
		{
			o->HiddenMesh = -2;
			if (GetLargeRand(3) == 0)
			{
				Vector(0.3f, 0.3f, 0.3f, o->Light);
				CreateParticle(ThreadIndex, BITMAP_SMOKE, o->Position, o->Angle, o->Light, 21, o->Scale);
			}
		}
		return true;
	case 6:
		{
			o->HiddenMesh = -2;

			EVector3 vLightFire;
			Vector(1.0f, 0.2f, 0.0f, vLightFire);
			CreateSprite(BITMAP_LIGHT, o->Position, 2.0f * o->Scale, vLightFire, o);

			EVector3 vLight;
			Vector(1.0f, 1.0f, 1.0f, vLight);

			switch (GetLargeRand(3))
			{
			case 0:
				CreateParticle(ThreadIndex, BITMAP_FIRE_HIK1, o->Position, o->Angle, vLight, 0, o->Scale);
				break;
			case 1:
				CreateParticle(ThreadIndex, BITMAP_FIRE_CURSEDLICH, o->Position, o->Angle, vLight, 4, o->Scale);
				break;
			case 2:
				CreateParticle(ThreadIndex, BITMAP_FIRE_HIK3, o->Position, o->Angle, vLight, 0, o->Scale);
				break;
			}
		}
		return true;
	case 47:
		{
			EVector3 vLight;
			Vector(0.1f, 0.4f, 1.0f, vLight);

			switch (GetLargeRand(3))
			{
			case 0:
				CreateParticle(ThreadIndex, BITMAP_FIRE_HIK1_MONO, o->Position, o->Angle, vLight, 2, o->Scale);
				break;
			case 1:
				CreateParticle(ThreadIndex, BITMAP_FIRE_HIK2_MONO, o->Position, o->Angle, vLight, 6, o->Scale);
				break;
			case 2:
				CreateParticle(ThreadIndex, BITMAP_FIRE_HIK3_MONO, o->Position, o->Angle, vLight, 2, o->Scale);
				break;
			}

			CreateParticle(ThreadIndex, BITMAP_FIRE_HIK1_MONO, o->Position, o->Angle, vLight, 2, o->Scale);
		}
		return true;
	case 48:
		if (GetLargeRand(3) == 0)
		{
			EVector3 Light, vPos;
			Vector(0.6f, 0.8f, 1.0f, Light);
			VectorCopy(o->Position, vPos);
			mu_int32 iScale = o->Scale * 60;
			vPos[0] += GetLargeRand(iScale) - iScale / 2;
			vPos[1] += GetLargeRand(iScale) - iScale / 2;
			CreateParticle(ThreadIndex, BITMAP_LIGHT, vPos, o->Angle, Light, 15, o->Scale, o);
		}
		return true;
	}

	return false;
}

mu_boolean CGMDoppelGanger2::RenderMonsterVisual(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b)
{
	EVector3 vPos, vLight;

	switch (o->Type)
	{
	case MODEL_MONSTER01 + 189:
	case MODEL_MONSTER01 + 190:
		if (GetLargeRand(4) == 0)
		{
			b->TransformByObjectBone(ThreadIndex, vPos, o, 6);
			vPos[1] += 50.0f;

			Vector(1.0f, 1.0f, 1.0f, vLight);
			CreateParticle(ThreadIndex, BITMAP_SMOKE, vPos, o->Angle, vLight, 61);
		}
		if (c->Dead == 0 && GetLargeRand(4) == 0)
		{
			Vector(o->Position[0] + (mu_float)(GetLargeRand(64) - 32),
				o->Position[1] + (mu_float)(GetLargeRand(64) - 32),
				o->Position[2] + (mu_float)(GetLargeRand(32) - 16), vPos);
			CreateParticle(ThreadIndex, BITMAP_SMOKE + 1, vPos, o->Angle, vLight, 0);
		}

		if (o->Type == MODEL_MONSTER01 + 190)
		{
			Vector(1.0f, 0.2f, 0.1f, vLight);
			for (mu_int32 j = 0; j < 50; ++j)
			{
				if (j >= 0 && j <= 1) continue;
				if (j >= 12 && j <= 20) continue;
				if (j >= 24 && j <= 32) continue;
				if (j >= 35 && j <= 50) continue;
				b->TransformByObjectBone(ThreadIndex, vPos, o, j);
				CreateSprite(BITMAP_LIGHT, vPos, 3.1f, vLight, o);
			}

			mu_float fScale = 0.8f;
			Vector(1.0f, 1.0f, 1.0f, vLight);
			for (mu_int32 i = 0; i < 30; ++i)
			{
				b->TransformByObjectBone(ThreadIndex, vPos, o, GetLargeRand(50));
				vPos[0] += GetLargeRand(10) - 20;
				vPos[1] += GetLargeRand(10) - 20;
				vPos[2] += GetLargeRand(10) - 20;

				switch (GetLargeRand(3))
				{
				case 0:
					CreateParticle(ThreadIndex, BITMAP_FIRE_HIK1, vPos, o->Angle, vLight, 0, fScale);
					break;
				case 1:
					CreateParticle(ThreadIndex, BITMAP_FIRE_CURSEDLICH, vPos, o->Angle, vLight, 4, fScale);
					break;
				case 2:
					CreateParticle(ThreadIndex, BITMAP_FIRE_HIK3, vPos, o->Angle, vLight, 0, fScale);
					break;
				}
			}
		}
		return true;
	}

	return false;
}

mu_boolean CGMDoppelGanger2::CreateFireSpark(PARTICLE* o)
{
	if (!IsDoppelGanger2())
	{
		return false;
	}

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

	Vector(1.0f, 0.0f, 0.0f, o->Light);

	return true;
}