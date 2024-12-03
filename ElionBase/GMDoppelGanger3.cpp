#include "stdafx.h"
#include "GMDoppelGanger3.h"

CGMDoppelGanger3 g_DoppelGanger3;

CGMDoppelGanger3::CGMDoppelGanger3()
{

}

CGMDoppelGanger3::~CGMDoppelGanger3()
{

}

mu_boolean CGMDoppelGanger3::MoveObject(const mu_uint32 ThreadIndex, OBJECT* o)
{
	switch (o->Type)
	{
	case 22:
		o->HiddenMesh = -2;
		o->Timer += 0.1f;
		if (o->Timer > 10.0f)
			o->Timer = 0.0f;
		if (o->Timer > 5.0f)
			CreateParticle(ThreadIndex, BITMAP_BUBBLE, o->Position, o->Angle, o->Light);
		return true;
	case 23:
		o->BlendMesh = 0;
		o->BlendMeshLight = EMath::Sin(FWorldTime*0.002f)*0.3f + 0.5f;
		return true;
	case 32:
	case 34:
		o->BlendMesh = 1;
		o->BlendMeshLight = EMath::Sin(FWorldTime*0.004f)*0.5f + 0.5f;
		return true;
	case 38:
		o->BlendMesh = 0;
		return true;
	case 40:
		o->BlendMesh = 0;
		o->BlendMeshLight = EMath::Sin(FWorldTime*0.004f)*0.3f + 0.5f;
		o->Velocity = 0.05f;
		return true;
	case 47:
	case 48:
		o->HiddenMesh = -2;
		return true;
	}

	return false;
}

mu_boolean CGMDoppelGanger3::MoveMonsterVisual(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b)
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

void CGMDoppelGanger3::MoveBlurEffect(const mu_uint32 ThreadIndex, CHARACTER* pCharacter, OBJECT* pObject, MUModel* pModel)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	switch (pObject->Type)
	{
	case MODEL_MONSTER01 + 189:
	case MODEL_MONSTER01 + 190:
		{
			if (!(pObject->CurrentAction == MONSTER01_WALK || pObject->CurrentAction == MONSTER01_ATTACK1 || pObject->CurrentAction == MONSTER01_ATTACK2))
				break;

			EVector3  vLight;
			Vector(0.6f, 0.4f, 0.2f, vLight);

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
				pModel->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[pModel->GetBoneIndex(33)], StartRelative, StartPos, false);
				pModel->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[pModel->GetBoneIndex(34)], EndRelative, EndPos, false);
				CreateBlur(pCharacter, StartPos, EndPos, vLight, 0, false, 0);

				fAnimationFrame += fSpeedPerFrame;
			}
		}
		break;
	}
}

mu_boolean CGMDoppelGanger3::RenderObjectMesh(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b, mu_boolean ExtraMon)
{
	mu_float fBlendMeshLight = 0;

	switch (o->Type)
	{
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
	case 38:
		{
			b->BodyLight(ThreadIndex)[0] = EMath::Min(b->BodyLight(ThreadIndex)[0] * 2.0f, 1.0f);
			b->BodyLight(ThreadIndex)[1] = EMath::Min(b->BodyLight(ThreadIndex)[1] * 2.0f, 1.0f);
			b->BodyLight(ThreadIndex)[2] = EMath::Min(b->BodyLight(ThreadIndex)[2] * 2.0f, 1.0f);
			b->RenderBody(ThreadIndex, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		return true;
	case 43:
		{
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		return true;
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

mu_boolean CGMDoppelGanger3::RenderObjectVisual(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b)
{
	switch (o->Type)
	{
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

mu_boolean CGMDoppelGanger3::RenderMonsterVisual(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b)
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

	return true;
}