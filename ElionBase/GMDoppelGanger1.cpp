#include "stdafx.h"
#include "GMDoppelGanger1.h"

CGMDoppelGanger1 g_DoppelGanger1;

CGMDoppelGanger1::CGMDoppelGanger1()
{
	m_bIsMP3Playing = false;
}

CGMDoppelGanger1::~CGMDoppelGanger1()
{
}

CHARACTER* CGMDoppelGanger1::CreateMonster(mu_int32 iType, mu_int32 PosX, mu_int32 PosY, mu_int32 Key)
{
	CHARACTER* pCharacter = NULL;

	switch (iType)
	{
	case 529:
		pCharacter = CreateCharacter(Key, MODEL_MONSTER01+190, PosX, PosY);
		pCharacter->Object.Scale = 1.0f;
		break;
	case 530:
		pCharacter = CreateCharacter(Key, MODEL_MONSTER01+189, PosX, PosY);
		pCharacter->Object.Scale = 0.8f;
		break;
	case 531:
		pCharacter = CreateCharacter(Key, MODEL_MONSTER01+145, PosX, PosY);
		pCharacter->Object.Scale = 1.2f;
		pCharacter->Weapon[0].Type = -1;
		pCharacter->Weapon[1].Type = -1;
		break;
	case 532:
		pCharacter = CreateCharacter(Key, MODEL_MONSTER01+6, PosX, PosY);
		pCharacter->Object.Scale = 0.6f;
		break;
	case 533:
		pCharacter = CreateCharacter(Key, MODEL_MONSTER01+191, PosX, PosY);
		pCharacter->Object.Scale = 1.1f;
		break;
	case 534:
		pCharacter = CreateCharacter(Key,MODEL_PLAYER,PosX,PosY);
		pCharacter->Object.Scale = 1.0f;
		pCharacter->Class = GAME::CLASS_ELF;
		pCharacter->BodyPart[BODYPART_HELM  ].Type = MODEL_HELM  +13;
		pCharacter->BodyPart[BODYPART_ARMOR ].Type = MODEL_ARMOR +13;
		pCharacter->BodyPart[BODYPART_PANTS ].Type = MODEL_PANTS +13;
		pCharacter->BodyPart[BODYPART_GLOVES].Type = MODEL_GLOVES+13;
		pCharacter->BodyPart[BODYPART_BOOTS ].Type = MODEL_BOOTS +13;
		pCharacter->Weapon[0].Type = MODEL_BOW+15;
		pCharacter->Weapon[1].Type = MODEL_BOW+17;
		break;
	case 535:
		pCharacter = CreateCharacter(Key,MODEL_PLAYER,PosX,PosY);
		pCharacter->Object.Scale = 1.0f;
		pCharacter->Class = GAME::CLASS_KNIGHT;
		pCharacter->BodyPart[BODYPART_HELM  ].Type = MODEL_HELM  +1;
		pCharacter->BodyPart[BODYPART_ARMOR ].Type = MODEL_ARMOR +1;
		pCharacter->BodyPart[BODYPART_PANTS ].Type = MODEL_PANTS +1;
		pCharacter->BodyPart[BODYPART_GLOVES].Type = MODEL_GLOVES+1;
		pCharacter->BodyPart[BODYPART_BOOTS ].Type = MODEL_BOOTS +1;
		pCharacter->Weapon[0].Type = MODEL_SWORD+13;
		pCharacter->Weapon[1].Type = -1;
		break;
	case 536:
		pCharacter = CreateCharacter(Key,MODEL_PLAYER,PosX,PosY);
		pCharacter->Object.Scale = 1.0f;
		pCharacter->Class = GAME::CLASS_WIZARD;
		pCharacter->BodyPart[BODYPART_HELM  ].Type = MODEL_HELM  +18;
		pCharacter->BodyPart[BODYPART_ARMOR ].Type = MODEL_ARMOR +18;
		pCharacter->BodyPart[BODYPART_PANTS ].Type = MODEL_PANTS +18;
		pCharacter->BodyPart[BODYPART_GLOVES].Type = MODEL_GLOVES+18;
		pCharacter->BodyPart[BODYPART_BOOTS ].Type = MODEL_BOOTS +18;
		pCharacter->Weapon[0].Type = MODEL_STAFF+9;
		pCharacter->Weapon[1].Type = MODEL_SHIELD+15;
		break;
	case 537:
		pCharacter = CreateCharacter(Key,MODEL_PLAYER,PosX,PosY);
		pCharacter->Object.Scale = 1.0f;
		pCharacter->Class = GAME::CLASS_DARK;
		pCharacter->BodyPart[BODYPART_HELM  ].Type = MODEL_BODY_HELM+15;
		pCharacter->BodyPart[BODYPART_ARMOR ].Type = MODEL_ARMOR +15;
		pCharacter->BodyPart[BODYPART_PANTS ].Type = MODEL_PANTS +15;
		pCharacter->BodyPart[BODYPART_GLOVES].Type = MODEL_GLOVES+15;
		pCharacter->BodyPart[BODYPART_BOOTS ].Type = MODEL_BOOTS +15;
		pCharacter->Weapon[0].Type = MODEL_SWORD+31;
		pCharacter->Weapon[1].Type = -1;
		break;
	case 538:
		pCharacter = CreateCharacter(Key,MODEL_PLAYER,PosX,PosY);
		pCharacter->Object.Scale = 1.0f;
		pCharacter->Class = GAME::CLASS_DARK_LORD;
		pCharacter->BodyPart[BODYPART_HELM  ].Type = MODEL_HELM  +27;
		pCharacter->BodyPart[BODYPART_ARMOR ].Type = MODEL_ARMOR +27;
		pCharacter->BodyPart[BODYPART_PANTS ].Type = MODEL_PANTS +27;
		pCharacter->BodyPart[BODYPART_GLOVES].Type = MODEL_GLOVES+27;
		pCharacter->BodyPart[BODYPART_BOOTS ].Type = MODEL_BOOTS +27;
		pCharacter->Weapon[0].Type = MODEL_MACE+10;
		pCharacter->Weapon[1].Type = MODEL_SHIELD+7;
		pCharacter->Helper.Type    = MODEL_HELPER+4;
		CreateBug( MODEL_DARK_HORSE, pCharacter->Object.Position,&pCharacter->Object,1);
		break;
	case 539:
		pCharacter = CreateCharacter(Key,MODEL_PLAYER,PosX,PosY);
		pCharacter->Object.Scale = 1.0f;
		pCharacter->Class = GAME::CLASS_SUMMONER;
		pCharacter->BodyPart[BODYPART_HELM  ].Type = MODEL_HELM  +40;
		pCharacter->BodyPart[BODYPART_ARMOR ].Type = MODEL_ARMOR +40;
		pCharacter->BodyPart[BODYPART_PANTS ].Type = MODEL_PANTS +40;
		pCharacter->BodyPart[BODYPART_GLOVES].Type = MODEL_GLOVES+40;
		pCharacter->BodyPart[BODYPART_BOOTS ].Type = MODEL_BOOTS +40;
		pCharacter->Weapon[0].Type = MODEL_STAFF+18;
		break;
	}

	return pCharacter;
}

mu_boolean CGMDoppelGanger1::MoveObject(OBJECT* o)
{
	switch (o->Type)
	{
	case 22:
		{
			o->BlendMeshLight = (mu_float)EMath::Sin(FWorldTime*0.001f) + 1.0f;
		}
		return true;
	case 70:
	case 80:
	case 99:
	case 101:
		{
			o->HiddenMesh = -2;
		}
		return true;
	}

	return false;
}

mu_boolean CGMDoppelGanger1::MoveMonsterVisual(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b)
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

void CGMDoppelGanger1::MoveBlurEffect(const mu_uint32 ThreadIndex, CHARACTER* pCharacter, OBJECT* pObject, MUModel* pModel)
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

mu_boolean CGMDoppelGanger1::RenderObjectMesh(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b, mu_boolean ExtraMon)
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
	case 76:
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		return true;
	case 98:
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 0, RENDER_CHROME | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		return true;
	case 102:
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		b->StreamMesh(ThreadIndex) = 0;
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, -IWorldTime % 4000 * 0.00025f);
		b->StreamMesh(ThreadIndex) = -1;
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

mu_boolean CGMDoppelGanger1::RenderObjectVisual(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b)
{
	switch (o->Type)
	{
	case 70:
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
	case 80:
		{
			EVector3 vLight;
			Vector(0.7f, 0.2f, 0.1f, vLight);

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
	case 99:
		if (o->HiddenMesh != -2)
		{
			EVector3 Light;
			Vector(0.04f, 0.06f, 0.08f, Light);
			for (mu_int32 i = 0; i < 10; ++i)
				CreateParticle(ThreadIndex, BITMAP_CLOUD, o->Position, o->Angle, Light, 3, o->Scale, o);
		}
		o->HiddenMesh = -2;
		return true;
	case 101:
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

mu_boolean CGMDoppelGanger1::RenderMonsterVisual(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b)
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

mu_boolean CGMDoppelGanger1::PlayMonsterSound(OBJECT* o)
{
	mu_float fDis_x, fDis_y;
	fDis_x = o->Position[0] - Hero->Object.Position[0];
	fDis_y = o->Position[1] - Hero->Object.Position[1];
	mu_float fDistance = EMath::Sqrt(fDis_x*fDis_x + fDis_y*fDis_y);

	if (fDistance > 500.0f)
		return true;

	switch (o->Type)
	{
	case MODEL_MONSTER01 + 145:
		if (MONSTER01_ATTACK1 == o->CurrentAction || MONSTER01_ATTACK2 == o->CurrentAction)
		{
			PlayBackground(SOUND_RAKLION_ICEWALKER_ATTACK);
		}
		else if (MONSTER01_WALK == o->CurrentAction)
		{
			if (GetLargeRand(20) == 0)
			{
				PlayBackground(SOUND_RAKLION_ICEWALKER_MOVE);
			}
		}
		else if (MONSTER01_DIE == o->CurrentAction)
		{
			{
				PlayBackground(SOUND_ELBELAND_WOLFHUMAN_DEATH01);
			}
		}
		return true;
	case MODEL_MONSTER01 + 190:
		{
			if (MONSTER01_ATTACK1 == o->CurrentAction || MONSTER01_ATTACK2 == o->CurrentAction)
			{
				PlayBackground(SOUND_DOPPELGANGER_RED_BUGBEAR_ATTACK);
			}
			else if (MONSTER01_DIE == o->CurrentAction)
			{
				PlayBackground(SOUND_DOPPELGANGER_RED_BUGBEAR_DEATH);
			}
		}
		return true;
	case MODEL_MONSTER01 + 189:
		{
			if (MONSTER01_ATTACK1 == o->CurrentAction || MONSTER01_ATTACK2 == o->CurrentAction)
			{
				PlayBackground(SOUND_DOPPELGANGER_BUGBEAR_ATTACK);
			}
			else if (MONSTER01_DIE == o->CurrentAction)
			{
				PlayBackground(SOUND_DOPPELGANGER_BUGBEAR_DEATH);
			}
		}
		return true;
	case MODEL_MONSTER01 + 191:
		{
			if (MONSTER01_APEAR == o->CurrentAction)
			{
				PlayBackground(SOUND_DOPPELGANGER_SLIME_ATTACK);
			}
			else if (MONSTER01_DIE == o->CurrentAction)
			{
				PlayBackground(SOUND_DOPPELGANGER_SLIME_DEATH);
			}
		}
		return true;
	}

	return false;
}

void CGMDoppelGanger1::PlayBGM()
{
	if (IsDoppelGanger1() || IsDoppelGanger2() || IsDoppelGanger3() || IsDoppelGanger4())
	{
#if 0 // FIX
		if (!g_pDoppelGangerFrame->IsDoppelGangerEnabled())
		{
			EMusicBackend::Stop(MUSICS::Data[MUSIC_DOPPELGANGER]);
		}
		else
#endif
		{
			EMusicBackend::Play(MUSICS::Data[MUSIC_DOPPELGANGER]);
		}
	}
}