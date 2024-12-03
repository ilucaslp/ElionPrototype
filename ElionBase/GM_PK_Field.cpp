#include "stdafx.h"
#include "GM_PK_Field.h"

CGM_PK_Field g_PKField;

CHARACTER* CGM_PK_Field::CreateMonster(mu_int32 iType, mu_int32 PosX, mu_int32 PosY, mu_int32 Key)
{
	if(!IsPKField())
	{
		return nullptr;
	}

	CHARACTER* pCharacter = nullptr;
	switch (iType)
	{
	case 480:
		{
			pCharacter = CreateCharacter(Key, MODEL_MONSTER01+157, PosX, PosY);
			pCharacter->Object.Scale = 1.0f;
			pCharacter->Weapon[0].Type = -1;
			pCharacter->Weapon[1].Type = -1;
			pCharacter->Object.m_iAnimation = 0;
		}
		break;
	case 481:
		{
			pCharacter = CreateCharacter(Key, MODEL_MONSTER01+157, PosX, PosY);
			pCharacter->Object.Scale = 1.0f;
			pCharacter->Weapon[0].Type = -1;
			pCharacter->Weapon[1].Type = -1;
			pCharacter->Object.m_iAnimation = 0;
		}
		break;
	case 482:
		{
			pCharacter = CreateCharacter(Key, MODEL_MONSTER01+158, PosX, PosY);
			pCharacter->Object.Scale = 1.0f;
			pCharacter->Weapon[0].Type = -1;
			pCharacter->Weapon[1].Type = -1;
			pCharacter->Object.m_iAnimation = 0;
		}
		break;
	case 483:
		{
			pCharacter = CreateCharacter(Key, MODEL_MONSTER01+158, PosX, PosY);
			pCharacter->Object.Scale = 1.0f;
			pCharacter->Weapon[0].Type = -1;
			pCharacter->Weapon[1].Type = -1;
			pCharacter->Object.m_iAnimation = 0;
		}
		break;
	case 484:
		{
			pCharacter = CreateCharacter(Key, MODEL_MONSTER01+159, PosX, PosY);
			pCharacter->Object.Scale = 0.7f;
			pCharacter->Weapon[0].Type = -1;
			pCharacter->Weapon[1].Type = -1;
			pCharacter->Object.m_iAnimation = 0;
		}
		break;
	case 485:
		{
			pCharacter = CreateCharacter(Key, MODEL_MONSTER01+159, PosX, PosY);
			pCharacter->Object.Scale = 0.7f;
			pCharacter->Weapon[0].Type = -1;
			pCharacter->Weapon[1].Type = -1;
			pCharacter->Object.m_iAnimation = 0;
		}
		break;
	case 486:
		{
			pCharacter = CreateCharacter(Key, MODEL_MONSTER01+160, PosX, PosY);
			pCharacter->Object.Scale = 1.0f;
			pCharacter->Weapon[0].Type = -1;
			pCharacter->Weapon[1].Type = -1;
			pCharacter->Object.m_iAnimation = 0;
			pCharacter->Object.LifeTime = 100;
		}
		break;
	case 487:
		{
			pCharacter = CreateCharacter(Key, MODEL_MONSTER01+161, PosX, PosY);
			pCharacter->Object.Scale = 1.0f;
			pCharacter->Weapon[0].Type = -1;
			pCharacter->Weapon[1].Type = -1;
			pCharacter->Object.m_iAnimation = 0;
			pCharacter->Object.LifeTime = 100;
		}
		break;
	case 488:
		{
			pCharacter = CreateCharacter(Key, MODEL_MONSTER01+161, PosX, PosY);
			pCharacter->Object.Scale = 1.0f;
			pCharacter->Weapon[0].Type = -1;
			pCharacter->Weapon[1].Type = -1;
			pCharacter->Object.m_iAnimation = 0;
			pCharacter->Object.LifeTime = 100;
		}
		break;
	case 489:
		{
			pCharacter = CreateCharacter(Key, MODEL_MONSTER01+163, PosX, PosY);
			pCharacter->Object.Scale = 1.0f;
			pCharacter->Weapon[0].Type = -1;
			pCharacter->Weapon[1].Type = -1;
			pCharacter->Object.m_iAnimation = 0;
		}
		break;
	case 490:
		{
			pCharacter = CreateCharacter(Key, MODEL_MONSTER01+162, PosX, PosY);
			pCharacter->Object.Scale = 1.0f;
			pCharacter->Weapon[0].Type = -1;
			pCharacter->Weapon[1].Type = -1;
			pCharacter->Object.m_iAnimation = 0;
		}
		break;
	case 491:
		{
			pCharacter = CreateCharacter(Key, MODEL_MONSTER01+162, PosX, PosY);
			pCharacter->Object.Scale = 1.0f;
			pCharacter->Weapon[0].Type = -1;
			pCharacter->Weapon[1].Type = -1;
			pCharacter->Object.m_iAnimation = 0;
		}
		break;
	}
	return pCharacter;
}

void CGM_PK_Field::MoveBlurEffect(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b)
{
	if (!IsPKField())
		return;

	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	if (o->Type >= MODEL_MONSTER01 + 158 && o->Type <= MODEL_MONSTER01 + 161)
	{
		mu_float Start_Frame = 0.0f;
		mu_float End_Frame = 0.0f;

		switch (o->Type)
		{
		case MODEL_MONSTER01 + 159:
			{
				Start_Frame = 4.0f;
				End_Frame = 10.0f;
			}
			break;
		case MODEL_MONSTER01 + 158:
			{
				Start_Frame = 3.0f;
				End_Frame = 7.0f;
			}
			break;
		case MODEL_MONSTER01 + 160:
		case MODEL_MONSTER01 + 161:
			{
				Start_Frame = 3.0f;
				End_Frame = 8.0f;
			}
			break;
		default:
			return;
		}

		if ((o->AnimationFrame >= Start_Frame && o->AnimationFrame <= End_Frame && o->CurrentAction == MONSTER01_ATTACK1)
			|| (o->AnimationFrame >= Start_Frame && o->AnimationFrame <= End_Frame && o->CurrentAction == MONSTER01_ATTACK2))
		{
			MUModel *b = &MODELS::Data[o->Type];
			EVector3  Light;

			EVector3 StartPos, StartRelative;
			EVector3 EndPos, EndRelative;

			mu_float fDelay = 5.0f;

			mu_float fActionSpeed = b->_AnimationHeader[o->CurrentAction].Speed;
			mu_float fSpeedPerFrame = fActionSpeed / fDelay;
			mu_float fAnimationFrame = o->AnimationFrame - fActionSpeed;
			for (mu_int32 i = 0; i < fDelay; ++i)
			{
				b->Animation(ThreadIndex, ThreadInfo.BoneTransform, fAnimationFrame, o->PriorAnimationFrame, o->PriorAction, o->Angle, o->HeadAngle);

				Vector(0.0f, 0.0f, 0.0f, StartRelative);
				Vector(0.0f, 0.0f, 0.0f, EndRelative);

				switch (o->Type)
				{
				case MODEL_MONSTER01 + 159:
					{
						Vector(0.3f, 0.3f, 0.3f, Light);
						b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(33)], StartRelative, StartPos, false);
						b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(34)], EndRelative, EndPos, false);
						CreateBlur(c, StartPos, EndPos, Light, 0);
					}
					break;
				case MODEL_MONSTER01 + 158:
					{
						Vector(0.0f, 0.3f, 0.2f, Light);
						b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(39)], StartRelative, StartPos, false);
						b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(40)], EndRelative, EndPos, false);
						CreateBlur(c, StartPos, EndPos, Light, 0);
					}
					break;
				case MODEL_MONSTER01 + 160:
				case MODEL_MONSTER01 + 161:
					{
						if (o->Type == MODEL_MONSTER01 + 160)
						{
							Vector(0.9f, 0.2f, 0.1f, Light);
						}
						else	//o->Type == MODEL_MONSTER01+161
						{
							Vector(0.2f, 0.9f, 0.1f, Light);
						}

						b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(40)], StartRelative, StartPos, false);

						b->TransformByBoneMatrix(ThreadIndex, EndPos, ThreadInfo.BoneTransform[b->GetBoneIndex(40)]);
						CreateBlur(c, StartPos, EndPos, Light, 5, false, 0);

						b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(40)], EndRelative, EndPos, false);
						CreateBlur(c, StartPos, EndPos, Light, 5, false, 1);
						b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(53)], EndRelative, EndPos, false);
						CreateBlur(c, StartPos, EndPos, Light, 5, false, 2);
						b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(14)], StartRelative, StartPos, false);

						b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(14)], EndRelative, EndPos, false);
						CreateBlur(c, StartPos, EndPos, Light, 5, false, 3);
						b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(27)], EndRelative, EndPos, false);
						CreateBlur(c, StartPos, EndPos, Light, 5, false, 4);
					}
					break;

				default:
					break;
				}
				fAnimationFrame += fSpeedPerFrame;
			}
		}
	}
}

mu_boolean CGM_PK_Field::CreateObject(mu_uint32 objectIndex, OBJECT* o)
{
	if(!IsPKField())
	{
		return false;
	}

	if(o->Type>=0 && o->Type<=6)
	{
		o->CollisionRange = -300;
		return true;
	}

	return false;
}

mu_boolean CGM_PK_Field::MoveObject(OBJECT* o)
{
	if(!IsPKField())
	{
		return false;
	}

	switch(o->Type)
	{
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
		o->HiddenMesh = -2;
		return true;

	default:
		break;
	}

	return false;
}

mu_boolean CGM_PK_Field::RenderObjectVisual(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b)
{
	if (!IsPKField())
	{
		return false;
	}

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
	}
	return false;
}

mu_boolean CGM_PK_Field::RenderObjectMesh(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b, mu_boolean ExtraMon)
{
	if(!IsPKField())
	{
		return false;
	}

	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];
	
	if(o->Type >= MODEL_MONSTER01+157 && o->Type <= MODEL_MONSTER01+163)
	{
		RenderMonster(ThreadIndex, o, b, ExtraMon);
		
		return true;
	}
	else if (o->Type == 16)
	{
		mu_float fLumi = (EMath::Sin(FWorldTime*0.002f) + 1.0f) * 0.5f;
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, 0, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);

		return true;
	}
 	else if(o->Type == 15)
 	{
 		b->StreamMesh(ThreadIndex) = 0;
 		b->RenderBody(ThreadIndex, RENDER_TEXTURE,o->Alpha,o->BlendMesh,o->BlendMeshLight,o->BlendMeshTexCoordU,-IWorldTime%10000*0.0001f);
 		b->StreamMesh(ThreadIndex) = -1;

		return true;
 	}
	else if(o->Type == 67)
	{
		b->StreamMesh(ThreadIndex) = 1;
		b->RenderBody(ThreadIndex, RENDER_TEXTURE,o->Alpha,o->BlendMesh,o->BlendMeshLight,o->BlendMeshTexCoordU, IWorldTime%10000*0.0001f);
		b->StreamMesh(ThreadIndex) = -1;
 		EVector3 light;
 		Vector(1.0f,0.0f,0.0f,light);
 		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha,o->BlendMesh,o->BlendMeshLight,o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
 		VectorCopy(light, b->BodyLight(ThreadIndex));

		b->RenderMesh(ThreadIndex, 0, RENDER_BRIGHT|RENDER_CHROME, 0.2f,0,0.2f,o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);

		EVector3 vLightFire, Position, vPos;
		Vector(1.0f, 0.0f, 0.0f, vLightFire);
		Vector(0.0f, 0.0f, 0.0f, vPos);

		if(MU_UPDATECOUNTER > 0)
		{
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
				CreateParticle(ThreadIndex, BITMAP_SMOKE, Pos, o->Angle, Light, 63, o->Scale*1.5f);
		}
		return true;
	}
	else if(o->Type == 68)
	{
		b->StreamMesh(ThreadIndex) = 1;
		b->RenderBody(ThreadIndex, RENDER_TEXTURE,o->Alpha,o->BlendMesh,o->BlendMeshLight,o->BlendMeshTexCoordU, IWorldTime%10000*0.0001f);
		b->StreamMesh(ThreadIndex) = -1;
		
		EVector3 light;
		Vector(1.0f,0.0f,0.0f,light);
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha,o->BlendMesh,o->BlendMeshLight,o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		VectorCopy(light, b->BodyLight(ThreadIndex));
		b->RenderMesh(ThreadIndex, 0, RENDER_BRIGHT|RENDER_CHROME, 0.2f,0,0.2f,o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);

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
	return false;
}

mu_boolean CGM_PK_Field::MoveMonsterVisual(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b)
{
	if (!IsPKField())
		return false;

	switch (o->Type)
	{
	case MODEL_MONSTER01 + 160:
	case MODEL_MONSTER01 + 161:
		{
			if (o->CurrentAction == MONSTER01_DIE)
			{
				mu_int32 iBones[] = { 5, 6, 7 };
				EVector3 vLight, vPos, vRelative;
				Vector(1.0f, 1.0f, 1.0f, vLight);
				EVector3 vLightFire;
				if (o->Type == MODEL_MONSTER01 + 160)
				{
					Vector(1.0f, 0.2f, 0.0f, vLightFire);
				}
				else
				{
					Vector(0.2f, 1.0f, 0.0f, vLightFire);
				}
				for (mu_int32 i = 0; i < 3; ++i)
				{
					mu_float fScale = 1.2f;
					if (i >= 1)
					{
						b->TransformByObjectBone(ThreadIndex, vPos, o, iBones[i]);
						CreateSprite(BITMAP_LIGHT, vPos, 1.0f, vLightFire, o);

						fScale = 0.7f;
						Vector((GetLargeRand(10) - 5)*1.0f, (GetLargeRand(10) - 5)*1.0f, (GetLargeRand(10) - 5)*1.0f, vRelative);
						b->TransformByObjectBone(ThreadIndex, vPos, o, iBones[i], &vRelative);
					}
					else
					{
						b->TransformByObjectBone(ThreadIndex, vPos, o, iBones[i]);
						vPos[2] += 50.0f;
						CreateSprite(BITMAP_LIGHT, vPos, 2.5f, vLightFire, o);

						Vector((GetLargeRand(20) - 10)*1.0f, (GetLargeRand(20) - 10)*1.0f, (GetLargeRand(20) - 10)*1.0f, vRelative);
						b->TransformByObjectBone(ThreadIndex, vPos, o, iBones[i], &vRelative);
					}
					if (o->Type == MODEL_MONSTER01 + 160)
					{
						for (mu_int32 i = 0; i < 2; ++i)
						{
							mu_float fScale = (GetLargeRand(5) + 18)*0.03f;
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
					else		//o->Type == MODEL_MONSTER01+161
					{
						for (mu_int32 i = 0; i < 2; ++i)
						{
							mu_float fScale = (GetLargeRand(5) + 18)*0.03f;
							Vector(0.6f, 0.9f, 0.1f, o->Light);
							switch (GetLargeRand(3))
							{
							case 0:
								CreateParticle(ThreadIndex, BITMAP_FIRE_HIK1_MONO, vPos, o->Angle, o->Light, 0, fScale);
								break;
							case 1:
								CreateParticle(ThreadIndex, BITMAP_FIRE_HIK2_MONO, vPos, o->Angle, o->Light, 4, fScale);
								break;
							case 2:
								CreateParticle(ThreadIndex, BITMAP_FIRE_HIK3_MONO, vPos, o->Angle, o->Light, 0, fScale);
								break;
							}
						}
					}
				}
			}
		}
		return true;
	}
	return false;
}

mu_boolean CGM_PK_Field::RenderMonsterVisual(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b)
{
	if (!IsPKField())
	{
		return false;
	}

	EVector3 vPos, vLight;

	switch (o->Type)
	{
	case MODEL_MONSTER01 + 157:
		{

		}
		return true;
	case MODEL_MONSTER01 + 158:
		{

		}
		return true;
	case MODEL_MONSTER01 + 159:
		{
			EVector3 p, Position;
			if (GetLargeRand(4) == 0)
			{
				Vector(0.0f, 50.0f, 0.0f, p);
				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(6)], p, Position, true);

				Vector(1.0f, 1.0f, 1.0f, o->Light);
				CreateParticle(ThreadIndex, BITMAP_SMOKE, Position, o->Angle, o->Light, 61);
			}
		}
		return true;
	case MODEL_MONSTER01 + 160:
	case MODEL_MONSTER01 + 161:
		{
			mu_int32 iBones[] = { 37, 11, 70, 65, 6 };

			switch (o->Type)
			{
			case MODEL_MONSTER01 + 160:
				Vector(0.9f, 0.2f, 0.1f, vLight);
				break;
			case MODEL_MONSTER01 + 161:
				Vector(0.3f, 0.9f, 0.2f, vLight);
				break;
			}
			if (o->CurrentAction != MONSTER01_DIE)
			{
				for (mu_int32 i = 0; i < 4; ++i)
				{
					if (GetLargeRand(4) > 0) continue;

					b->TransformByObjectBone(ThreadIndex, vPos, o, iBones[i]);
					CreateParticle(ThreadIndex, BITMAP_SMOKE, vPos, o->Angle, vLight, 50, 1.0f);
					CreateParticle(ThreadIndex, BITMAP_SMOKELINE1 + GetLargeRand(3), vPos, o->Angle, vLight, 0, 0.01f);
				}

				if (o->CurrentAction == MONSTER01_ATTACK1)
				{
					EVector3 TempPos;
					Vector(0.0f, 0.0f, 0.0f, TempPos);

					b->TransformByObjectBone(ThreadIndex, vPos, o, iBones[4]);

					TempPos[1] = (Hero->Object.Position[1] - vPos[1])*0.5f;
					TempPos[0] = (Hero->Object.Position[0] - vPos[0])*0.5f;
					TempPos[2] = 0.0f;

					VectorNormalize(TempPos);
					VectorScale(TempPos, 50.0f, TempPos);

					VectorAdd(vPos, TempPos, vPos);

					CreateParticle(ThreadIndex, BITMAP_LIGHT + 2, vPos, o->Angle, vLight, 7, 0.5f);

					switch (o->Type)
					{
					case MODEL_MONSTER01 + 160:
						Vector(0.9f, 0.4f, 0.1f, vLight);
						break;
					case MODEL_MONSTER01 + 161:
						Vector(0.6f, 0.9f, 0.2f, vLight);
						break;
					}

					CreateParticle(ThreadIndex, BITMAP_SPARK + 1, vPos, o->Angle, vLight, 29, 1.0f);
				}
			}
			else
			{
				if (o->LifeTime == 100)
				{
					o->LifeTime = 90;

					EVector3 vRelativePos, vWorldPos, Light;
					Vector(1.0f, 1.0f, 1.0f, Light);

					Vector(0.0f, 0.0f, 0.0f, vRelativePos);

					b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(5)], vRelativePos, vWorldPos, true);
					switch (o->Type)
					{
					case MODEL_MONSTER01 + 160:
						CreateEffect(ThreadIndex, MODEL_PKFIELD_ASSASSIN_EFFECT_RED_HEAD, vWorldPos, o->Angle, Light, 0, o, 0, 0);
						break;

					case MODEL_MONSTER01 + 161:
						CreateEffect(ThreadIndex, MODEL_PKFIELD_ASSASSIN_EFFECT_GREEN_HEAD, vWorldPos, o->Angle, Light, 0, o, 0, 0);
						break;
					}
				}
			}
		}
		return true;
	case MODEL_MONSTER01 + 162:
	case MODEL_MONSTER01 + 163:
		{
			mu_float fRotation = (mu_float)((mu_int32)(FWorldTime*0.1f) % 360);
			mu_float fAngle = (EMath::Sin(FWorldTime*0.003f) + 1.0f)*0.4f + 1.5f;
			EVector3 vWorldPos, vLight;
			switch (o->Type)
			{
			case MODEL_MONSTER01 + 162:
				{
					Vector(0.5f, 0.1f, 0.9f, vLight);
				}
				break;
			case MODEL_MONSTER01 + 163:
				{
					Vector(0.9f, 0.4f, 0.1f, vLight);
				}
				break;
			}

			b->TransformByObjectBone(ThreadIndex, vWorldPos, o, 3);
			CreateSprite(BITMAP_LIGHTMARKS, vWorldPos, fAngle, vLight, o, fRotation);
			fAngle = (EMath::Sin(FWorldTime*0.003f) + 1.0f)*0.4f + 0.5f;
			b->TransformByObjectBone(ThreadIndex, vWorldPos, o, 37);
			CreateSprite(BITMAP_LIGHTMARKS, vWorldPos, fAngle, vLight, o, fRotation);
			b->TransformByObjectBone(ThreadIndex, vWorldPos, o, 43);
			CreateSprite(BITMAP_LIGHTMARKS, vWorldPos, fAngle, vLight, o, fRotation);

			EVector3 vRelative, vPos;
			Vector(1.0f, 1.0f, 1.0f, vLight);
			mu_int32 iModel = (o->Type == MODEL_MONSTER01 + 163) ? MODEL_LAVAGIANT_FOOTPRINT_R : MODEL_LAVAGIANT_FOOTPRINT_V;

			if (o->CurrentAction == MONSTER01_WALK)
			{
				if (o->AnimationFrame >= 2.0f && o->AnimationFrame <= 2.4f)
				{
					Vector(0.0f, 0.0f, 0.0f, vPos);
					b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(36)], vPos, vRelative, true);
					CreateEffect(ThreadIndex, iModel, vRelative, o->Angle, vLight, 0, o, -1, 0, 0, 0, 1.3f);
					CreateParticle(ThreadIndex, BITMAP_SMOKE, vRelative, o->Angle, vLight, 62, 1.0f);
				}
				else if (o->AnimationFrame >= 7.0f && o->AnimationFrame <= 7.4f)
				{
					Vector(0.0f, 0.0f, 0.0f, vPos);
					b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(42)], vPos, vRelative, true);
					CreateEffect(ThreadIndex, iModel, vRelative, o->Angle, vLight, 0, o, -1, 0, 0, 0, 1.3f);
					CreateParticle(ThreadIndex, BITMAP_SMOKE, vRelative, o->Angle, vLight, 62, 1.0f);
				}
			}
			else if (o->CurrentAction == MONSTER01_ATTACK2)
			{
				if (o->AnimationFrame >= 6.0f && o->AnimationFrame <= 6.4f)
				{
					Vector(0.0f, 0.0f, 0.0f, vPos);
					b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(42)], vPos, vRelative, true);
					CreateEffect(ThreadIndex, iModel, vRelative, o->Angle, vLight, 0, o, -1, 0, 0, 0, 1.6f);
					CreateParticle(ThreadIndex, BITMAP_SMOKE, vRelative, o->Angle, vLight, 62, 4.0f);
				}
			}

			EVector3 p, Position;
			if (GetLargeRand(4) == 0)
			{
				Vector(0.0f, 50.0f, 0.0f, p);
				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(7)], p, Position, true);
				switch (o->Type)
				{
				case MODEL_MONSTER01 + 162:
					Vector(0.5f, 0.1f, 0.9f, o->Light);
					break;
				case MODEL_MONSTER01 + 163:
					Vector(0.9f, 0.4f, 0.1f, o->Light);
					break;
				}
				CreateParticle(ThreadIndex, BITMAP_SMOKE, Position, o->Angle, o->Light, 61);
			}
		}
		return true;
	}
	return false;
}

mu_boolean CGM_PK_Field::RenderMonster(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b, mu_boolean ExtraMon)
{
	if (!IsPKField())
	{
		return false;
	}

	mu_float fRotation = (mu_float)((mu_int32)(FWorldTime*0.1f) % 360);
	mu_float fAngle = (EMath::Sin(FWorldTime*0.004f) + 1.0f)*0.4f + 0.2f;
	EVector3 vWorldPos, vLight;
	Vector(0.1f, 0.4f, 0.5f, vLight);

	switch (o->Type)
	{
	case MODEL_MONSTER01 + 157:
		{
			if(MU_UPDATECOUNTER > 0)
			{
				b->TransformByObjectBone(ThreadIndex, vWorldPos, o, 9);
				CreateSprite(BITMAP_LIGHTMARKS, vWorldPos, fAngle, vLight, o, fRotation);
			}

			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, o->BlendMeshLight*fAngle, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		return true;
	case MODEL_MONSTER01 + 158:
		{
			if(MU_UPDATECOUNTER > 0)
			{
				b->TransformByObjectBone(ThreadIndex, vWorldPos, o, 9);
				CreateSprite(BITMAP_LIGHTMARKS, vWorldPos, fAngle, vLight, o, fRotation);
			}

			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, o->BlendMeshLight*fAngle, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		return true;
	case MODEL_MONSTER01 + 159:
		{
			mu_float fBlendMeshLight = 0.0f;
			fBlendMeshLight = (EMath::Sin(FWorldTime*0.003f) + 1.0f)*0.5f;

			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);

			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, fBlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_BUGBEAR_R);
		}
		return true;
	case MODEL_MONSTER01 + 160:
	case MODEL_MONSTER01 + 161:
		{
			if (o->CurrentAction == MONSTER01_DIE)
			{
				if (o->LifeTime == 100 && MU_UPDATECOUNTER > 0)
				{
					switch (o->Type)
					{
					case MODEL_MONSTER01 + 160:
						{
							CreateEffect(ThreadIndex, MODEL_PKFIELD_ASSASSIN_EFFECT_RED_BODY, o->Position, o->Angle, o->Light, 0, o, 0, 0);
						}
						break;
					case MODEL_MONSTER01 + 161:
						{
							CreateEffect(ThreadIndex, MODEL_PKFIELD_ASSASSIN_EFFECT_GREEN_BODY, o->Position, o->Angle, o->Light, 0, o, 0, 0);
						}
						break;
					}
				}
			}
			else
			{
				mu_float fBlendMeshLight = 0.0f;
				fBlendMeshLight = (EMath::Sin(FWorldTime*0.005f) + 1.0f)*0.3f + 0.3f;

				b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);

				switch (o->Type)
				{
				case MODEL_MONSTER01 + 160:
					{
						b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
						b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, 1, fBlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_PKMON02);

						b->RenderMesh(ThreadIndex, 3, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
						b->RenderMesh(ThreadIndex, 3, RENDER_TEXTURE, o->Alpha, 3, fBlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_PKMON01);
					}
					break;
				case MODEL_MONSTER01 + 161:
					{
						b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
						b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, 1, fBlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_PKMON04);

						b->RenderMesh(ThreadIndex, 3, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
						b->RenderMesh(ThreadIndex, 3, RENDER_TEXTURE, o->Alpha, 3, fBlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_PKMON03);
					}
					break;
				}

				Vector(b->BodyLight(ThreadIndex)[0] * 0.65f, b->BodyLight(ThreadIndex)[0] * 0.65f, b->BodyLight(ThreadIndex)[0] * 0.65f, b->BodyLight(ThreadIndex));
				b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);

				Vector(b->BodyLight(ThreadIndex)[0] * 2.0f, b->BodyLight(ThreadIndex)[0] * 1.0f, b->BodyLight(ThreadIndex)[0] * 0.4f, b->BodyLight(ThreadIndex));
				b->RenderMesh(ThreadIndex, 2, RENDER_BRIGHT | RENDER_CHROME, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			}
		}
		return true;
	case MODEL_MONSTER01 + 162:
	case MODEL_MONSTER01 + 163:
		{
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);

			mu_float fBlendMeshLight = 0.0f;
			fBlendMeshLight = (EMath::Sin(FWorldTime*0.001f));

			mu_float fAlpha = 1.0f;
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);

			mu_int32 iTexture = (o->Type == MODEL_MONSTER01 + 162) ? BITMAP_PKMON06 : BITMAP_PKMON05;

			if (fBlendMeshLight < 0)
			{
				fBlendMeshLight = -(fBlendMeshLight);
				b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, fAlpha, 0, fBlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, iTexture);
			}
			else
			{
				for (mu_int32 i = 0; i < 3; ++i)
					b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, fAlpha, 0, fBlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, iTexture);
			}
		}
		return true;
	case MODEL_PKFIELD_ASSASSIN_EFFECT_GREEN_HEAD:
	case MODEL_PKFIELD_ASSASSIN_EFFECT_RED_HEAD:
		{
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		return true;
	case MODEL_PKFIELD_ASSASSIN_EFFECT_GREEN_BODY:
	case MODEL_PKFIELD_ASSASSIN_EFFECT_RED_BODY:
		{
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		return true;
	}
	return false;
}

mu_boolean CGM_PK_Field::CreateFireSpark(PARTICLE* o)
{
	if (!IsPKField())
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

mu_boolean CGM_PK_Field::PlayMonsterSound(OBJECT* o)
{
	if(!IsPKField())
		return false;
	
	mu_float fDis_x, fDis_y;
	fDis_x = o->Position[0] - Hero->Object.Position[0];
	fDis_y = o->Position[1] - Hero->Object.Position[1];
	mu_float fDistance = EMath::Sqrt(fDis_x*fDis_x+fDis_y*fDis_y);
	
	if(fDistance > 500.0f) 
		return true;

	switch(o->Type)
	{
	case MODEL_MONSTER01+157:
		{
			if(MONSTER01_ATTACK1 == o->CurrentAction || MONSTER01_ATTACK2 == o->CurrentAction)
			{
				PlayBackground(SOUND_PKFIELD_ZOMBIEWARRIOR_ATTACK);
			}
			else if(MONSTER01_SHOCK == o->CurrentAction)
			{
				PlayBackground(SOUND_PKFIELD_ZOMBIEWARRIOR_DAMAGE01);
			}
			else if(MONSTER01_WALK == o->CurrentAction)
			{
				if(GetLargeRand(20) == 0)
				{
					PlayBackground(SOUND_PKFIELD_ZOMBIEWARRIOR_MOVE01);
				}
			}
			else if(MONSTER01_DIE == o->CurrentAction)
			{
				PlayBackground(SOUND_PKFIELD_ZOMBIEWARRIOR_DEATH);
			}
		}
		return true;
	case MODEL_MONSTER01+158:
		{
			if(MONSTER01_ATTACK1 == o->CurrentAction || MONSTER01_ATTACK2 == o->CurrentAction)
			{
				PlayBackground(SOUND_PKFIELD_RAISEDGLADIATOR_ATTACK);
			}
			else if(MONSTER01_SHOCK == o->CurrentAction)
			{
				PlayBackground(SOUND_PKFIELD_RAISEDGLADIATOR_DAMAGE01);
			}
			else if(MONSTER01_WALK == o->CurrentAction)
			{
				if(GetLargeRand(20) == 0)
				{
					PlayBackground(SOUND_PKFIELD_RAISEDGLADIATOR_MOVE01);
				}
			}
			else if(MONSTER01_DIE == o->CurrentAction)
			{
				PlayBackground(SOUND_PKFIELD_RAISEDGLADIATOR_DEATH);
			}
		}
		return true;
	case MODEL_MONSTER01+159:
		{
			if(MONSTER01_ATTACK1 == o->CurrentAction || MONSTER01_ATTACK2 == o->CurrentAction)
			{
				PlayBackground(SOUND_PKFIELD_ASHESBUTCHER_ATTACK);
			}
			else if(MONSTER01_SHOCK == o->CurrentAction)
			{
				PlayBackground(SOUND_PKFIELD_ASHESBUTCHER_DAMAGE01);
			}
			else if(MONSTER01_WALK == o->CurrentAction)
			{
				if(GetLargeRand(20) == 0)
				{
					PlayBackground(SOUND_PKFIELD_ASHESBUTCHER_MOVE01);
				}
			}
			else if(MONSTER01_DIE == o->CurrentAction)
			{
				PlayBackground(SOUND_PKFIELD_ASHESBUTCHER_DEATH);
			}
		}
		return true;
	case MODEL_MONSTER01+160:
	case MODEL_MONSTER01+161:
		{
			if(MONSTER01_ATTACK1 == o->CurrentAction || MONSTER01_ATTACK2 == o->CurrentAction)
			{
				PlayBackground(SOUND_PKFIELD_BLOODASSASSIN_ATTACK);
			}
			else if(MONSTER01_SHOCK == o->CurrentAction)
			{
				PlayBackground(SOUND_PKFIELD_BLOODASSASSIN_DAMAGE01);
			}
			else if(MONSTER01_WALK == o->CurrentAction)
			{
				if(GetLargeRand(20) == 0)
				{
					PlayBackground(SOUND_PKFIELD_BLOODASSASSIN_MOVE01);
				}
			}
			else if(MONSTER01_DIE == o->CurrentAction)
			{
				PlayBackground(SOUND_PKFIELD_BLOODASSASSIN_DEDTH);
			}
		}
		return true;
	case MODEL_MONSTER01+162:
	case MODEL_MONSTER01+163:
		{		
			if(MONSTER01_ATTACK1 == o->CurrentAction)
			{
				PlayBackground(SOUND_PKFIELD_BURNINGLAVAGOLEM_ATTACK01);
			}
			else if(MONSTER01_ATTACK2 == o->CurrentAction)
			{
				PlayBackground(SOUND_PKFIELD_BURNINGLAVAGOLEM_ATTACK02);
			}
			else if(MONSTER01_SHOCK == o->CurrentAction)
			{
				PlayBackground(SOUND_PKFIELD_BURNINGLAVAGOLEM_DAMAGE01);
			}
			else if(MONSTER01_WALK == o->CurrentAction)
			{
				if(GetLargeRand(20) == 0)
				{
					PlayBackground(SOUND_PKFIELD_BURNINGLAVAGOLEM_MOVE01);
				}
			}
			else if(MONSTER01_DIE == o->CurrentAction)
			{
				PlayBackground(SOUND_PKFIELD_BURNINGLAVAGOLEM_DEATH);
			}
		}
		return true;
	}

	return false;
}

void CGM_PK_Field::PlayObjectSound(OBJECT* o)
{

}

void CGM_PK_Field::PlayBGM()
{
	if(IsPKField())
	{
		EMusicBackend::Play(MUSICS::Data[MUSIC_PKFIELD]);
	}
	else
	{
		EMusicBackend::Stop(MUSICS::Data[MUSIC_PKFIELD]);
	}
}