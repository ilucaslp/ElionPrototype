#include "stdafx.h"
#include "GMEmpireGuardian2.h"

GMEmpireGuardian2 g_EmpireGuardian2;

GMEmpireGuardian2::GMEmpireGuardian2()
{

}

GMEmpireGuardian2::~GMEmpireGuardian2()
{

}

mu_boolean GMEmpireGuardian2::CreateObject(mu_uint32 objectIndex, OBJECT* o)
{
	switch(o->Type)
	{
	case 129:
	case 130:
	case 131:
	case 132:
		{
			o->Angle[2] = (mu_float)((mu_int32)o->Angle[2]%360);
			VectorCopy(o->Angle,o->HeadAngle);
			VectorCopy(o->Position,o->HeadTargetAngle);
		}
		return true;
		
	case 115:
	case 117:
		{
			o->SubType = 100;
		}
		return true;
	}
	
	return false;
}

CHARACTER* GMEmpireGuardian2::CreateMonster(const mu_uint32 ThreadIndex, mu_int32 iType, mu_int32 PosX, mu_int32 PosY, mu_int32 Key)
{
	CHARACTER* pCharacter = nullptr;

	switch (iType)
	{
	case 509:
		{
			pCharacter = CreateCharacter(Key, MODEL_MONSTER01 + 169, PosX, PosY);

			pCharacter->Object.Scale = 1.3f;

			m_bCurrentIsRage_Bermont = false;
		}
		break;
	case 514:
		{
			pCharacter = CreateCharacter(Key, MODEL_MONSTER01 + 174, PosX, PosY);

			pCharacter->Object.Scale = 1.35f;
		}
		break;
	case 515:
		{
			pCharacter = CreateCharacter(Key, MODEL_MONSTER01 + 175, PosX, PosY);

			OBJECT * pObject = &pCharacter->Object;
			pObject->Scale = 1.3f;

			//MoveEye(ThreadIndex, pObject, &MODELS::Data[pObject->Type], 79, 33);
			CreateJoint(ThreadIndex, BITMAP_JOINT_ENERGY, pObject->Position, pObject->Position, pObject->Angle, 22, pObject, 30.0f);
			CreateJoint(ThreadIndex, BITMAP_JOINT_ENERGY, pObject->Position, pObject->Position, pObject->Angle, 23, pObject, 30.0f);
		}
		break;
	}

	return pCharacter;
}

mu_boolean GMEmpireGuardian2::MoveObject(const mu_uint32 ThreadIndex, OBJECT* o)
{
	switch (o->Type)
	{
	case 20:
		{
			o->AnimationFrame = g_EmpireGuardianAnimationFrame[ThreadIndex];
			o->PriorAnimationFrame = g_EmpireGuardianPriorAnimationFrame[ThreadIndex];
			o->PriorAction = g_EmpireGuardianPriorAction[ThreadIndex];
		}
		return true;
	case 64:
		{
			o->Velocity = 0.64f;
		}
		return true;
	case 79:
	case 80:
	case 82:
	case 83:
	case 84:
	case 85:
	case 86:
	case 129:
	case 130:
	case 131:
	case 132:
		{
			o->HiddenMesh = -2;
		}
		return true;
	case 81:
		{
			o->BlendMeshTexCoordV += 0.015f;
		}
		return true;
	case 36:
		{
			o->Velocity = 0.02f;
		}
		return true;
	}

	return false;
}

mu_boolean GMEmpireGuardian2::MoveMonsterVisual(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b)
{
	if (true == g_EmpireGuardian1.MoveMonsterVisual(ThreadIndex, c, o, b))
	{
		return true;
	}

	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	switch (o->Type)
	{
	case MODEL_MONSTER01 + 169:
		{
			EVector3 vPos;
			switch (o->CurrentAction)
			{
			case MONSTER01_STOP1:
			case MONSTER01_STOP2:
				break;
			case MONSTER01_WALK:
				{
					mu_int32 iTypeSubType = 0;

					Vector(0.4f, 0.4f, 0.4f, o->Light);

					if (6.8f <= o->AnimationFrame && o->AnimationFrame < 7.5f)
					{
						b->TransformByObjectBone(ThreadIndex, vPos, o, 42);
						vPos[2] += 25.0f;
						vPos[1] += 0.0f;
						CreateParticle(ThreadIndex, BITMAP_SMOKE, vPos, o->Angle, o->Light, iTypeSubType, 1.0f);
					}
					if (0.8f <= o->AnimationFrame && o->AnimationFrame < 1.5f)
					{
						b->TransformByObjectBone(ThreadIndex, vPos, o, 49);
						vPos[2] += 25.0f;
						vPos[1] += 0.0f;
						CreateParticle(ThreadIndex, BITMAP_SMOKE, vPos, o->Angle, o->Light, iTypeSubType, 1.0f);
					}
				}
				break;
			case MONSTER01_DIE:
				{
				}
				break;
			case MONSTER01_ATTACK1:
				{
					if (6.0f <= o->AnimationFrame && o->AnimationFrame < 12.0f)
					{
						EVector3  Light;
						Vector(1.0f, 1.0f, 1.0f, Light);

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

							b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(34)], StartRelative, StartPos, false);
							b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(35)], EndRelative, EndPos, false);
							CreateBlur(c, StartPos, EndPos, Light, 0);

							fAnimationFrame += fSpeedPerFrame;
						}
					}
				}
				break;
			case MONSTER01_ATTACK2:
				{
					if (o->AnimationFrame >= 5.4f && o->AnimationFrame <= 11.0f)
					{
						RenderSkillEarthQuake(ThreadIndex, c, o, b, 14);
					}

					if (6.0f <= o->AnimationFrame && o->AnimationFrame < 10.0f)
					{
						EVector3  Light;
						Vector(1.0f, 1.0f, 1.0f, Light);

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

							b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(34)], StartRelative, StartPos, false);
							b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(35)], EndRelative, EndPos, false);
							CreateBlur(c, StartPos, EndPos, Light, 0);

							fAnimationFrame += fSpeedPerFrame;
						}
					}
				}
				break;
			case MONSTER01_ATTACK3:
				{
					if (o->AnimationFrame >= 1.7f && o->AnimationFrame <= 2.3f)
					{
						CHARACTER	*chT = &CharactersClient[c->TargetCharacter];
						EVector3	&v3TargetPos = chT->Object.Position;

						Vector(v3TargetPos[0], v3TargetPos[1], v3TargetPos[2], o->Light);
						CreateEffect(ThreadIndex, MODEL_EMPIREGUARDIAN_BLOW_OF_DESTRUCTION,
							o->Position, o->Angle, o->Light, 0, o, -1, 0, 0, 0, 1.0f);
					}

					if (6.0f <= o->AnimationFrame && o->AnimationFrame < 12.0f)
					{
						EVector3  Light;
						Vector(1.0f, 1.0f, 1.0f, Light);

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

							b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(34)], StartRelative, StartPos, false);
							b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(35)], EndRelative, EndPos, false);
							CreateBlur(c, StartPos, EndPos, Light, 0);

							fAnimationFrame += fSpeedPerFrame;
						}
					}
				}
				break;
			case MONSTER01_APEAR:
				{
					EVector3 Light;

					if (m_bCurrentIsRage_Bermont == true)
					{
						Vector(1.0f, 1.0f, 1.0f, Light);
						CreateInferno(ThreadIndex, o->Position);

						CreateEffect(ThreadIndex, MODEL_CIRCLE, o->Position, o->Angle, Light, 4, o);
						CreateEffect(ThreadIndex, MODEL_CIRCLE, o->Position, o->Angle, Light, 4, o);
						m_bCurrentIsRage_Bermont = false;
					}
				}
				break;
			}
		}
		return true;
	case MODEL_MONSTER01 + 174:
		{
			EVector3 vPos, vRelative, vLight, v3Temp(0.0f);

			mu_float fLumi1 = (EMath::Sin(FWorldTime*0.004f) + 1.0f) * 0.25f;

			Vector(0.7f + fLumi1, 0.7f + fLumi1, 0.7f + fLumi1, vLight);
			Vector(0.0f, 0.0f, 0.0f, vRelative);
			b->TransformByObjectBone(ThreadIndex, vPos, o, 21, &vRelative);
			CreateSprite(BITMAP_LIGHT, vPos, 2.0f + fLumi1, vLight, o);

			b->TransformByObjectBone(ThreadIndex, vPos, o, 11, &vRelative);
			CreateSprite(BITMAP_LIGHT, vPos, 2.0f + fLumi1, vLight, o);

			Vector(0.4f, 0.5f, 0.7f, vLight);
			b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(8)], v3Temp, vPos, true);
			CreateSprite(BITMAP_LIGHTMARKS, vPos, 2.0f, vLight, o);

			switch (o->CurrentAction)
			{
			case MONSTER01_STOP1:
			case MONSTER01_STOP2:
				{
				}
				break;
			case MONSTER01_WALK:
				{
					mu_int32		iTypeSubType = 0;

					Vector(0.7f, 0.7f, 0.7f, o->Light);

					if (7.0f <= o->AnimationFrame && o->AnimationFrame < 8.0f)
					{
						b->TransformByObjectBone(ThreadIndex, vPos, o, 42);
						vPos[2] += 25.0f;
						vPos[1] += 0.0f;
						CreateParticle(ThreadIndex, BITMAP_SMOKE, vPos, o->Angle, o->Light, iTypeSubType, 2.0f);
					}
					if (1.0f <= o->AnimationFrame && o->AnimationFrame < 2.0f)
					{
						b->TransformByObjectBone(ThreadIndex, vPos, o, 47);
						vPos[2] += 25.0f;
						vPos[1] += 0.0f;
						CreateParticle(ThreadIndex, BITMAP_SMOKE, vPos, o->Angle, o->Light, iTypeSubType, 2.0f);
					}
				}
				break;
			case MONSTER01_DIE:
				{
				}
				break;
			case MONSTER01_ATTACK1:
				{
				}
				break;
			case MONSTER01_ATTACK2:
				{
					EVector3	v3PosPiercing, v3AnglePiercing;

					if (o->AnimationFrame >= 6.6f && o->AnimationFrame <= 7.4f)
					{
						b->TransformByObjectBone(ThreadIndex, v3PosPiercing, o, 23);
						v3PosPiercing[2] = v3PosPiercing[2] - 150.0f;

						VectorCopy(o->Angle, v3AnglePiercing);
						v3AnglePiercing[2] = v3AnglePiercing[2] + 5.0f;

						CreateEffect(ThreadIndex, MODEL_PIERCING2, v3PosPiercing, v3AnglePiercing, o->Light, 0);

						PlayBackground(SOUND_ATTACK_SPEAR);
					}
				}
				break;
			case MONSTER01_APEAR:
				{
					EVector3	v3PosPiercing, v3AnglePiercing;
					if (o->AnimationFrame >= 6.6f && o->AnimationFrame <= 7.4f)
					{
						b->TransformByObjectBone(ThreadIndex, v3PosPiercing, o, 23);
						v3PosPiercing[2] = v3PosPiercing[2] - 150.0f;

						VectorCopy(o->Angle, v3AnglePiercing);
						v3AnglePiercing[2] = v3AnglePiercing[2] + 5.0f;

						CreateEffect(ThreadIndex, MODEL_WAVES, v3PosPiercing, v3AnglePiercing, o->Light, 1);
						CreateEffect(ThreadIndex, MODEL_WAVES, v3PosPiercing, v3AnglePiercing, o->Light, 1);
						CreateEffect(ThreadIndex, MODEL_WAVES, v3PosPiercing, v3AnglePiercing, o->Light, 1);
						CreateEffect(ThreadIndex, MODEL_WAVES, v3PosPiercing, v3AnglePiercing, o->Light, 1);
						CreateEffect(ThreadIndex, MODEL_WAVES, v3PosPiercing, v3AnglePiercing, o->Light, 1);
						CreateEffect(ThreadIndex, MODEL_WAVES, v3PosPiercing, v3AnglePiercing, o->Light, 1);

						CreateEffect(ThreadIndex, MODEL_PIERCING2, v3PosPiercing, v3AnglePiercing, o->Light);
						CreateEffect(ThreadIndex, MODEL_PIERCING2, v3PosPiercing, v3AnglePiercing, o->Light);
						CreateEffect(ThreadIndex, MODEL_PIERCING2, v3PosPiercing, v3AnglePiercing, o->Light);
						CreateEffect(ThreadIndex, MODEL_PIERCING2, v3PosPiercing, v3AnglePiercing, o->Light);
						CreateEffect(ThreadIndex, MODEL_PIERCING2, v3PosPiercing, v3AnglePiercing, o->Light);

						PlayBackground(SOUND_ATTACK_SPEAR);
					}
				}
				break;
			}
		}
		return true;
	}

	return false;
}

void GMEmpireGuardian2::MoveBlurEffect(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b)
{
	switch(o->Type)
	{
	case MODEL_MONSTER01+167:
	case MODEL_MONSTER01+178:
	case MODEL_MONSTER01+179:
	case MODEL_MONSTER01+180:
	case MODEL_MONSTER01+181:
		{
			g_EmpireGuardian1.MoveBlurEffect(ThreadIndex, c, o, b);
		}
		break;
	}
}

mu_boolean GMEmpireGuardian2::RenderObjectMesh(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b, mu_boolean ExtraMon)
{
	if (true == g_EmpireGuardian1.RenderObjectMesh(ThreadIndex, o, b, ExtraMon))
	{
		return true;
	}

	switch (o->Type)
	{
	case MODEL_MONSTER01 + 167:
	case MODEL_MONSTER01 + 169:
	case MODEL_MONSTER01 + 174:
	case MODEL_MONSTER01 + 175:
		{
			RenderMonster(ThreadIndex, o, b, ExtraMon);

			return true;
		}
	}

	return false;
}

mu_boolean GMEmpireGuardian2::RenderObjectVisual(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	EVector3 p, Position, Light;
	Vector(0.0f, 30.0f, 0.0f, Position);
	Vector(0.0f, 0.0f, 0.0f, p);

	switch (o->Type)
	{
	case 12:
		{
			EVector3 vPos, vRelativePos, vLight1, vLight2;
			mu_float flumi = EMath::Abs(EMath::Sin(FWorldTime*0.0008))*0.9f + 0.1f;
			mu_float fScale = o->Scale*0.3f*flumi;
			Vector(0.0f, 0.0f, 0.0f, vPos);
			Vector(8.0f, -3.0f, -3.0f, vRelativePos);
			Vector(flumi, flumi, flumi, vLight1);
			Vector(0.9f, 0.1f, 0.1f, vLight2);
			b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(2)], vRelativePos, vPos);
			CreateSprite(BITMAP_SHINY + 6, vPos, 0.5f, vLight2, o);
			CreateSprite(BITMAP_SHINY + 6, vPos, fScale, vLight1, o);
			Vector(3.0f, -3.0f, -3.5f, vRelativePos);
			b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(3)], vRelativePos, vPos);
			CreateSprite(BITMAP_SHINY + 6, vPos, 0.5f, vLight2, o);
			CreateSprite(BITMAP_SHINY + 6, vPos, fScale, vLight1, o);
		}
		return true;

	case 20:
		{
			if (o->AnimationFrame > 5.4f && o->AnimationFrame < 6.5f)
			{
				EVector3	Angle;
				for (mu_int32 i = 0; i < 4; ++i)
				{
					Vector((mu_float)(GetLargeRand(60) + 60 + 90), 0.0f, o->Angle[2] + 180, Angle);
					CreateJoint(ThreadIndex, BITMAP_JOINT_SPARK, o->Position, o->Position, Angle, 5, o);
				}
				CreateParticle(ThreadIndex, BITMAP_SPARK, o->Position, Angle, o->Light, 11);
			}
			else if (o->AnimationFrame > 15.4f && o->AnimationFrame < 16.5f)
			{
				EVector3	Angle;
				for (mu_int32 i = 0; i < 4; ++i)
				{
					Vector((mu_float)(GetLargeRand(60) + 60 + 90), 0.0f, o->Angle[2], Angle);
					CreateJoint(ThreadIndex, BITMAP_JOINT_SPARK, o->Position, o->Position, Angle, 5, o);
				}
				CreateParticle(ThreadIndex, BITMAP_SPARK, o->Position, Angle, o->Light, 11);
			}
		}
		return true;

	case 37:
		{
			Vector(0.0f, 0.0f, 0.0f, p);
			b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(1)], p, Position);

			mu_float fLumi;
			fLumi = (EMath::Sin(FWorldTime*0.039f) + 1.0f) * 0.2f + 0.6f;
			EVector3 vLightFire;
			Vector(fLumi*0.7f, fLumi*0.7f, fLumi*0.7f, vLightFire);
			CreateSprite(BITMAP_FLARE, Position, 4.0f * o->Scale, vLightFire, o);
		}
		return true;

	case 50:
		{
			EVector3 vPos, vRelativePos, vLight1, vLight2, vAngle;
			Vector(0.0f, 0.0f, 0.0f, vPos);
			Vector(0.0f, 0.0f, 5.0f, vRelativePos);
			Vector(0.0f, -1.0f, 0.0f, vAngle);
			Vector(0.05f, 0.1f, 0.3f, vLight1);
			Vector(1.0f, 1.0f, 1.0f, vLight2);

			for (mu_int32 i = 2; i <= 7; ++i)
			{
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(i)], vRelativePos, vPos);
				CreateParticle(ThreadIndex, BITMAP_FIRE_HIK3_MONO, vPos, vAngle, vLight1, 4, o->Scale*0.6f);
				CreateParticle(ThreadIndex, BITMAP_FIRE_HIK3_MONO, vPos, vAngle, vLight2, 4, o->Scale*0.3f);
			}
		}
		return true;

	case 51:
		{
			if (o->AnimationFrame > 5.4f && o->AnimationFrame < 6.5f)
			{
				EVector3	Angle;
				for (mu_int32 i = 0; i < 4; ++i)
				{
					Vector((mu_float)(GetLargeRand(60) + 60 + 90), 0.0f, o->Angle[2] + 180, Angle);
					CreateJoint(ThreadIndex, BITMAP_JOINT_SPARK, o->Position, o->Position, Angle, 5, o);
				}
				CreateParticle(ThreadIndex, BITMAP_SPARK, o->Position, Angle, o->Light, 11);
			}
		}
		return true;

	case 64:
		{
			if ((o->AnimationFrame > 9.5f && o->AnimationFrame < 11.5f) ||
				(o->AnimationFrame > 23.5f && o->AnimationFrame < 25.5f))
			{
				EBone Matrix;
				EVector3 vAngle, vDirection, vPosition;
				Vector(0.0f, 0.0f, o->Angle[2] + 90, vAngle);
				AngleMatrix(vAngle, Matrix);
				Vector(0.0f, 30.0f, 0.0f, vDirection);
				VectorRotate(vDirection, Matrix, vPosition);
				VectorAdd(vPosition, o->Position, Position);

				Vector(0.04f, 0.03f, 0.02f, Light);
				for (mu_int32 i = 0; i < 3; ++i)
				{
					CreateParticle(ThreadIndex, BITMAP_CLOUD, Position, o->Angle, Light, 22, o->Scale, o);
				}
			}
		}
		return true;

	case 79:
		{
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

	case 80:
		{
			mu_float fLumi;
			fLumi = (EMath::Sin(FWorldTime*0.04f) + 1.0f) * 0.3f + 0.4f;
			EVector3 vLightFire;
			Vector(fLumi*0.1f, fLumi*0.1f, fLumi*0.5f, vLightFire);
			CreateSprite(BITMAP_LIGHT, o->Position, 8.0f * o->Scale, vLightFire, o);
		}
		return true;

	case 82:
		{
			CreateParticle(ThreadIndex, BITMAP_WATERFALL_5, o->Position, o->Angle, Light, 9, o->Scale);
		}
		return true;

	case 83:
		{
			Vector(1.0f, 1.0f, 1.0f, Light);
			CreateParticle(ThreadIndex, BITMAP_WATERFALL_3, o->Position, o->Angle, Light, 14, o->Scale);
		}
		return true;

	case 84:
		{
			Vector(1.0f, 1.0f, 1.0f, Light);
			if (GetLargeRand(8) == 0)
			{
				CreateParticle(ThreadIndex, BITMAP_WATERFALL_2, o->Position, o->Angle, Light, 4, o->Scale);
			}
		}
		return true;

	case 85:
		{
			EVector3 vLight;
			Vector(0.5f, 0.5f, 0.5f, vLight);
			mu_int32 iRemainder = IWorldTime % 200;

			if (iRemainder >= 0 && iRemainder <= 1)
			{
				mu_int32 iRand = GetLargeRand(4) + 4;
				for (mu_int32 i = 0; i < iRand; ++i)
				{
					CreateEffect(ThreadIndex, BITMAP_FLAME, o->Position, o->Angle, vLight, 6, nullptr, -1, 0, o->Scale);
				}
			}
		}
		return true;

	case 86:
		{
			if (GetLargeRand(6) == 0)
			{
				Vector(0.05f, 0.02f, 0.01f, Light);
				CreateParticle(ThreadIndex, BITMAP_CLOUD, o->Position, o->Angle, Light, 21, o->Scale, o);
			}
		}
		return true;

	case 129:
		{
			if (GetLargeRand(6) == 0)
			{
				Vector(0.01f, 0.02f, 0.05f, Light);
				CreateParticle(ThreadIndex, BITMAP_CLOUD, o->Position, o->Angle, Light, 21, o->Scale, o);
			}
		}
		return true;

	case 130:
		{
			if (GetLargeRand(6) == 0)
			{
				Vector(0.01f, 0.05f, 0.02f, Light);
				CreateParticle(ThreadIndex, BITMAP_CLOUD, o->Position, o->Angle, Light, 21, o->Scale, o);
			}
		}
		return true;

	case 131:
		{
			if (GetLargeRand(3) == 0)
			{
				Vector(1.0f, 1.0f, 1.0f, Light);
				CreateParticle(ThreadIndex, BITMAP_SMOKE, o->Position, o->Angle, Light, 22, o->Scale);

				Vector(1.0f, 1.0f, 1.0f, Light);
				CreateParticle(ThreadIndex, BITMAP_SMOKE, o->Position, o->Angle, Light, 21, o->Scale*2.0f, o);
			}
		}
		return true;

	case 132:
		{
			if (GetLargeRand(3) == 0)
			{
				Vector(1.0f, 1.0f, 1.0f, Light);
				CreateParticle(ThreadIndex, BITMAP_SMOKE, o->Position, o->Angle, Light, 60, o->Scale, o);

				Vector(1.0f, 1.0f, 1.0f, Light);
				CreateParticle(ThreadIndex, BITMAP_SMOKE, o->Position, o->Angle, Light, 60, o->Scale, o);

				Vector(1.0f, 1.0f, 1.0f, Light);
				CreateParticle(ThreadIndex, BITMAP_SMOKE, o->Position, o->Angle, Light, 21, o->Scale*2.0f, o);
			}
		}
		return true;
	}

	return false;
}

mu_boolean GMEmpireGuardian2::RenderMonster(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b, mu_boolean ExtraMon)
{
	switch (o->Type)
	{
	case MODEL_MONSTER01 + 169:
		{
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		return true;
	case MODEL_MONSTER01 + 174:
		{
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 2, RENDER_BRIGHT | RENDER_CHROME, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		return true;
	case MODEL_MONSTER01 + 175:
		{
			if (o->CurrentAction == MONSTER01_DIE)
			{
				Vector(0.3f, 1.0f, 0.2f, b->BodyLight(ThreadIndex));
			}
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 3, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 3, 0.5f, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		return true;
	}

	return false;
}


mu_boolean GMEmpireGuardian2::RenderMonsterVisual(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b)
{
	if (g_EmpireGuardian1.RenderMonsterVisual(ThreadIndex, c, o, b))
	{
		return true;
	}

	EVector3 vPos, vLight;

	switch (o->Type)
	{
	case MODEL_MONSTER01 + 169:
		{
			EVector3 vPos, vRelative, vLight;

			mu_float fLumi1 = (EMath::Sin(FWorldTime*0.004f) + 1.0f) * 0.25f;

			Vector(0.1f + fLumi1, 0.4f + fLumi1, 0.8f + fLumi1, vLight);
			Vector(0.0f, 0.0f, 0.0f, vRelative);
			b->TransformByObjectBone(ThreadIndex, vPos, o, 21, &vRelative);
			CreateSprite(BITMAP_LIGHT, vPos, 1.5f + fLumi1, vLight, o);

			b->TransformByObjectBone(ThreadIndex, vPos, o, 38, &vRelative);
			CreateSprite(BITMAP_LIGHT, vPos, 1.5f + fLumi1, vLight, o);

			if (g_isNotCharacterBuff(o) == true && g_isCharacterBuff(o, eBuff_Berserker) == true)
			{
				EVector3 vLightRage, vPosRage;

				mu_int32 iBoneCount = b->_BoneNonDummyEnd;
				Vector(1.0f, 0.1f, 0.1f, vLightRage);
				for (mu_int32 i = 0; i < iBoneCount; ++i)
				{
					if (GetLargeRand(6) > 0) continue;
					if (b->_ReverseBonesIndex[i] % 5 == 0)
					{
						b->TransformByObjectBoneDirect(ThreadIndex, vPosRage, o, i);
						CreateParticle(ThreadIndex, BITMAP_SMOKE, vPosRage, o->Angle, vLightRage, 50, 1.0f);
						CreateParticle(ThreadIndex, BITMAP_SMOKELINE1 + GetLargeRand(3), vPosRage, o->Angle, vLightRage, 0, 1.0f);
					}
				}
			}
		}
		return true;
	case MODEL_MONSTER01 + 175:
		{
			mu_int32 i;

			mu_float fLumi = (EMath::Sin(FWorldTime*0.08f) + 1.0f) * 0.5f * 0.3f + 0.7f;
			Vector(0.1f * fLumi, 0.6f * fLumi, 1.0f * fLumi, vLight);

			mu_int32 iBlueLights[] = { 93, 100, 47, 54 };
			for (i = 0; i < 4; ++i)
			{
				b->TransformByObjectBone(ThreadIndex, vPos, o, iBlueLights[i]);
				CreateSprite(BITMAP_LIGHT, vPos, 3.0f, vLight, o);
			}

			Vector(0.1f * fLumi, 1.0f * fLumi, 0.2f * fLumi, vLight);
			mu_int32 iGreenLights[] = { 92, 99, 46, 53, 95, 88, 42, 49 };
			for (i = 0; i < 8; ++i)
			{
				b->TransformByObjectBone(ThreadIndex, vPos, o, iGreenLights[i]);
				CreateSprite(BITMAP_LIGHT, vPos, 1.0f, vLight, o);
			}

			mu_int32 iBigGreenLights[] = { 80, 34 };
			for (i = 0; i < 2; ++i)
			{
				b->TransformByObjectBone(ThreadIndex, vPos, o, iBigGreenLights[i]);
				Vector(0.1f * fLumi, 1.0f * fLumi, 0.3f * fLumi, vLight);
				CreateSprite(BITMAP_LIGHT, vPos, 3.0f, vLight, o);

				Vector(0.5f, 0.5f, 0.5f, vLight);
				CreateParticle(ThreadIndex, BITMAP_CHROME2, vPos, o->Angle, vLight, 0, 0.9f, o);
			}

			MoveEye(ThreadIndex, o, b, 80, 34);

			if (o->CurrentAction == MONSTER01_ATTACK1 || o->CurrentAction == MONSTER01_ATTACK2)
			{
				for (i = 0; i < 2; ++i)
				{
					b->TransformByObjectBone(ThreadIndex, vPos, o, iBigGreenLights[i]);
					Vector(0.3f, 1.0f, 0.8f, vLight);
					CreateParticle(ThreadIndex, BITMAP_WATERFALL_4, vPos, o->Angle, vLight, 15, 2.0f);

					Vector(0.0f, 0.4f, 0.0f, vLight);
					CreateParticle(ThreadIndex, BITMAP_SPARK + 1, vPos, o->Angle, vLight, 13, 1.0f, o);
					CreateParticle(ThreadIndex, BITMAP_SPARK + 1, vPos, o->Angle, vLight, 13, 1.0f, o);
				}
			}
			else if (o->CurrentAction == MONSTER01_DIE)
			{
				o->BlendMesh = -2;

				if (o->AnimationFrame <= 3.0f)
				{
					Vector(0.1f, 1.0f, 0.2f, vLight);
					for (mu_int32 i = 0; i < 5; ++i)
					{
						CreateParticle(ThreadIndex, BITMAP_SMOKE, o->Position, o->Angle, vLight, 39);
					}

					CreateEffect(ThreadIndex, MODEL_SKILL_INFERNO, o->Position, o->Angle, o->Light, 9, o);

					if (o->AnimationFrame <= 0.2f)
					{
						Vector(0.4f, 1.0f, 0.6f, vLight);
						CreateEffect(ThreadIndex, MODEL_TWINTAIL_EFFECT, o->Position, o->Angle, vLight, 1, o);
						CreateEffect(ThreadIndex, MODEL_TWINTAIL_EFFECT, o->Position, o->Angle, vLight, 2, o);
					}
				}
			}
		}
		return true;
	}

	return true;
}

mu_boolean GMEmpireGuardian2::SetCurrentActionMonster(CHARACTER* c, OBJECT* o)
{
	if (IsEmpireGuardian2() == false)
		return false;

	if (true == g_EmpireGuardian1.SetCurrentActionMonster(c, o))
	{
		return true;
	}

	switch (c->MonsterIndex)
	{
	case 509:
		{
			if (m_bCurrentIsRage_Bermont == true)
			{
				SetAction(o, MONSTER01_APEAR);
				c->MonsterSkill = -1;
				return true;
			}

			switch (c->MonsterSkill)
			{
			case 60:
				{
					SetAction(o, MONSTER01_ATTACK2);
					c->MonsterSkill = -1;
				}
				break;
			case 61:
				{
					SetAction(o, MONSTER01_ATTACK3);
					c->MonsterSkill = -1;
				}
				break;
			case ATMON_SKILL_EMPIREGUARDIAN_BERSERKER:
				{
					SetAction(o, MONSTER01_APEAR);
					c->MonsterSkill = -1;

					m_bCurrentIsRage_Bermont = true;
				}
				break;
			default:
				{
					SetAction(o, MONSTER01_ATTACK1);
					c->MonsterSkill = -1;
				}
				break;
			}

		}
		return true;
	case 514:
		{
			switch (c->MonsterSkill)
			{
			case 47:
				{
					SetAction(o, MONSTER01_ATTACK2);
					c->MonsterSkill = -1;
				}
				break;
			case 50:
				{
					SetAction(o, MONSTER01_APEAR);
					c->MonsterSkill = -1;
				}
				break;
			default:
				{
					SetAction(o, MONSTER01_ATTACK1);
					c->MonsterSkill = -1;
				}
				break;
			}
		}
		return true;
	case 515:
		{
			switch (c->MonsterSkill)
			{
			case 51:
				{
					SetAction(o, MONSTER01_ATTACK2);
				}
				break;
			case 52:
				{
					SetAction(o, MONSTER01_ATTACK2);
				}
				break;
			default:
				{
					SetAction(o, MONSTER01_ATTACK1);
					c->MonsterSkill = -1;
				}
				break;
			}
		}
		return true;
	case 518:
		{

		}
		return true;
	case 519:
		{
			switch (c->MonsterSkill)
			{
			case 46:
				{
					SetAction(o, MONSTER01_ATTACK2);
					c->MonsterSkill = -1;
				}
				break;
			default:
				{
					SetAction(o, MONSTER01_ATTACK1);
					c->MonsterSkill = -1;
				}
				break;
			}
		}
		return true;
	}

	return false;
}

mu_boolean GMEmpireGuardian2::AttackEffectMonster(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b)
{
	if (IsEmpireGuardian2() == false)
		return false;

	switch (c->MonsterIndex)
	{
	case 515:
		{
			EVector3 vPos;

			if (o->CurrentAction == MONSTER01_ATTACK1)
			{
				if (c->TargetCharacter != -1)
				{
					CHARACTER * pTargetCharacter = &CharactersClient[c->TargetCharacter];
					CreateEffect(ThreadIndex, MODEL_FIRE, pTargetCharacter->Object.Position, o->Angle, o->Light);
					PlayBackground(SOUND_METEORITE01);
					c->TargetCharacter = -1;
					c->MonsterSkill = -1;
				}
			}
			else if (c->MonsterSkill == 51)
			{
				if (c->TargetCharacter != -1)
				{
					CHARACTER * pTargetCharacter = &CharactersClient[c->TargetCharacter];
					OBJECT * pTargetObject = &pTargetCharacter->Object;
					CreateEffect(ThreadIndex, BITMAP_FLAME, pTargetObject->Position, pTargetObject->Angle, pTargetObject->Light, 0, pTargetObject);
					PlayBackground(SOUND_FLAME);
					c->MonsterSkill = -1;
				}
			}
			else if (c->MonsterSkill == 52)
			{
				Vector(o->Position[0] + GetLargeRand(1024) - 512, o->Position[1] + GetLargeRand(1024) - 512, o->Position[2], vPos);
				CreateEffect(ThreadIndex, MODEL_FIRE, vPos, o->Angle, o->Light);
				PlayBackground(SOUND_METEORITE01);
			}
		}
		return true;
	}

	return false;
}

mu_boolean GMEmpireGuardian2::PlayMonsterSound(OBJECT* o)
{
	if (IsEmpireGuardian2() == false)
		return false;

	if (true == g_EmpireGuardian1.PlayMonsterSound(o))
	{
		return true;
	}

	mu_float fDis_x, fDis_y;
	fDis_x = o->Position[0] - Hero->Object.Position[0];
	fDis_y = o->Position[1] - Hero->Object.Position[1];
	mu_float fDistance = EMath::Sqrt(fDis_x*fDis_x + fDis_y*fDis_y);

	if (fDistance > 500.0f)
		return true;

	switch (o->Type)
	{
	case MODEL_MONSTER01 + 169:
		{
			switch (o->CurrentAction)
			{
			case MONSTER01_WALK:
				{
					if (GetLargeRand(2) == 0)
					{
						PlayBackground(SOUND_EMPIREGUARDIAN_JERINT_MONSTER_MOVE01);
					}
					else
					{
						PlayBackground(SOUND_EMPIREGUARDIAN_JERINT_MONSTER_MOVE02);
					}
				}
				break;
			case MONSTER01_ATTACK1:
				{
					PlayBackground(SOUND_EMPIREGUARDIAN_2CORP_VERMONT_MONSTER_ATTACK01);
				}
				break;
			case MONSTER01_ATTACK2:
				{
					PlayBackground(SOUND_EMPIREGUARDIAN_2CORP_VERMONT_MONSTER_ATTACK02);
				}
				break;
			case MONSTER01_ATTACK3:
				{
					PlayBackground(SOUND_EMPIREGUARDIAN_2CORP_VERMONT_MONSTER_ATTACK01);
				}
				break;
			case MONSTER01_APEAR:
				{
					PlayBackground(SOUND_EMPIREGUARDIAN_JERINT_MONSTER_RAGE);
				}
				break;
			case MONSTER01_DIE:
				{
					PlayBackground(SOUND_EMPIREGUARDIAN_2CORP_VERMONT_MONSTER_DEATH);
				}
				break;
			}
		}
		return true;
	case MODEL_MONSTER01 + 174:
		{
			switch (o->CurrentAction)
			{
			case MONSTER01_WALK:
				{
					if (GetLargeRand(2) == 0)
					{
						PlayBackground(SOUND_EMPIREGUARDIAN_JERINT_MONSTER_MOVE01);
					}
					else
					{
						PlayBackground(SOUND_EMPIREGUARDIAN_JERINT_MONSTER_MOVE02);
					}
				}
				break;
			case MONSTER01_ATTACK1:
				{
					PlayBackground(SOUND_EMPIREGUARDIAN_COMBATMASTER_ATTACK03);
				}
				break;
			case MONSTER01_ATTACK2:
				{
					PlayBackground(SOUND_EMPIREGUARDIAN_COMBATMASTER_ATTACK03);
				}
				break;
			case MONSTER01_ATTACK3:
				{
					PlayBackground(SOUND_EMPIREGUARDIAN_COMBATMASTER_ATTACK03);
				}
				break;
			case MONSTER01_DIE:
				{
					PlayBackground(SOUND_EMPIREGUARDIAN_JERINT_MONSTER_DEATH);
				}
				break;
			}
		}
		return true;
	case MODEL_MONSTER01 + 175:
		{
			switch (o->CurrentAction)
			{
			case MONSTER01_WALK:
				{
					PlayBackground(SOUND_EMPIREGUARDIAN_BOSS_GAION_MONSTER_MOVE);
				}
				break;
			case MONSTER01_ATTACK1:
				{
					PlayBackground(SOUND_METEORITE01);
					PlayBackground(SOUND_EXPLOTION01);
				}
				break;
			case MONSTER01_ATTACK2:
				{
					PlayBackground(SOUND_3RD_CHANGE_UP_BG_FIREPILLAR);
				}
				break;
			case MONSTER01_ATTACK3:
				{
					PlayBackground(SOUND_EXPLOTION01);
				}
				break;
			case MONSTER01_DIE:
				{
					PlayBackground(SOUND_EMPIREGUARDIAN_GRANDWIZARD_DEATH);
				}
				break;
			}
		}
		return true;
	}

	return false;
}