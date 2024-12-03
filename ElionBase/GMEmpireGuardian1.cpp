#include "stdafx.h"
#include "GMEmpireGuardian1.h"

GMEmpireGuardian1 g_EmpireGuardian1;
std::array<mu_float, MAX_TASKING_THREADS> g_EmpireGuardianAnimationFrame;
std::array<mu_float, MAX_TASKING_THREADS> g_EmpireGuardianPriorAnimationFrame;
std::array<mu_uint16, MAX_TASKING_THREADS> g_EmpireGuardianPriorAction;

GMEmpireGuardian1::GMEmpireGuardian1()
{
	ZeroArray(g_EmpireGuardianAnimationFrame);
	ZeroArray(g_EmpireGuardianPriorAnimationFrame);
	ZeroArray(g_EmpireGuardianPriorAction);

	m_iWeather = WEATHER_TYPE::WEATHER_SUN;
}

GMEmpireGuardian1::~GMEmpireGuardian1()
{

}

mu_boolean GMEmpireGuardian1::CreateObject(mu_uint32 objectIndex, OBJECT* o)
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

CHARACTER* GMEmpireGuardian1::CreateMonster(const mu_uint32 ThreadIndex, mu_int32 iType, mu_int32 PosX, mu_int32 PosY, mu_int32 Key)
{
	CHARACTER* pCharacter = nullptr;

	switch (iType)
	{
	case 506:
		{
			pCharacter = CreateCharacter(Key, MODEL_MONSTER01 + 166, PosX, PosY);

			pCharacter->Object.Scale = 1.45f;

			m_bCurrentIsRage_Raymond = false;
		}
		break;
	case 507:
		{
			pCharacter = CreateCharacter(Key, MODEL_MONSTER01 + 167, PosX, PosY);

			pCharacter->Object.Scale = 1.25f;

			m_bCurrentIsRage_Ercanne = false;
		}
		break;
	case 508:
		{
			pCharacter = CreateCharacter(Key, MODEL_MONSTER01 + 168, PosX, PosY);
			pCharacter->Object.Scale = 1.55f;

			OBJECT* o = &pCharacter->Object;

			//MoveEye(ThreadIndex, o, b, 14, 15, 71, 72, 73, 74);
			EVector3 vColor = EVector3(0.7f, 0.7f, 1.0f);
			CreateJoint(ThreadIndex, BITMAP_JOINT_ENERGY, o->Position, o->Position, o->Angle, 24, o, 10.0f, -1, 0, 0, -1, &vColor);
			CreateJoint(ThreadIndex, BITMAP_JOINT_ENERGY, o->Position, o->Position, o->Angle, 25, o, 10.0f, -1, 0, 0, -1, &vColor);
			Vector(0.7f, 0.7f, 1.0f, vColor);
			mu_float Sca = 100.0f;
			CreateJoint(ThreadIndex, BITMAP_JOINT_ENERGY, o->Position, o->Position, o->Angle, 54, o, Sca, 0, 0, 0, -1, &vColor);
			CreateJoint(ThreadIndex, BITMAP_JOINT_ENERGY, o->Position, o->Position, o->Angle, 54, o, Sca, 1, 0, 0, -1, &vColor);
			CreateJoint(ThreadIndex, BITMAP_JOINT_ENERGY, o->Position, o->Position, o->Angle, 54, o, Sca, 2, 0, 0, -1, &vColor);
			CreateJoint(ThreadIndex, BITMAP_JOINT_ENERGY, o->Position, o->Position, o->Angle, 54, o, Sca, 3, 0, 0, -1, &vColor);

			m_bCurrentIsRage_Daesuler = false;
		}
		break;
	case 511:
		{
			pCharacter = CreateCharacter(Key, MODEL_MONSTER01 + 171, PosX, PosY);
			pCharacter->Object.Scale = 1.35f;

			m_bCurrentIsRage_Gallia = false;
		}
		break;
	case 512:
		{
			pCharacter = CreateCharacter(Key, MODEL_MONSTER01 + 172, PosX, PosY);
			pCharacter->Object.Scale = 1.27f;
		}
		break;
	case 513:
		{
			pCharacter = CreateCharacter(Key, MODEL_MONSTER01 + 173, PosX, PosY);
			pCharacter->Object.Scale = 1.25f;
		}
		break;
	case 518:
		{
			pCharacter = CreateCharacter(Key, MODEL_MONSTER01 + 178, PosX, PosY);
			pCharacter->Object.Scale = 1.2f;

			Vector(0.0f, 0.0f, 0.0f, pCharacter->Object.EyeRight3);
			Vector(0.0f, 0.0f, 0.0f, pCharacter->Object.EyeLeft3);
		}
		break;
	case 519:
		{
			pCharacter = CreateCharacter(Key, MODEL_MONSTER01 + 179, PosX, PosY);
			pCharacter->Object.Scale = 0.9f;
		}
		break;
	case 520:
		{
			pCharacter = CreateCharacter(Key, MODEL_MONSTER01 + 180, PosX, PosY);
			pCharacter->Object.Scale = 1.1f;
		}
		break;
	case 521:
		{
			pCharacter = CreateCharacter(Key, MODEL_MONSTER01 + 181, PosX, PosY);
			pCharacter->Object.Scale = 1.1f;
		}
		break;
	case 524:
		{
			pCharacter = CreateCharacter(Key, MODEL_MONSTER01 + 183, PosX, PosY);
			pCharacter->Object.Scale = 1.25f;
		}
		break;
	case 525:
		{
			pCharacter = CreateCharacter(Key, MODEL_MONSTER01 + 184, PosX, PosY);
			pCharacter->Object.LifeTime = 100;
			pCharacter->Object.Scale = 1.25f;
		}
		break;
	case 526:
		{
			pCharacter = CreateCharacter(Key, MODEL_MONSTER01 + 185, PosX, PosY);
			pCharacter->Object.Scale = 0.6f;
			pCharacter->Object.LifeTime = 100;
		}
		break;
	}

	return pCharacter;
}

mu_boolean GMEmpireGuardian1::MoveObject(const mu_uint32 ThreadIndex, OBJECT* o)
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

mu_boolean GMEmpireGuardian1::MoveMonsterVisual(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	EVector3 vPos, vRelativePos, Light;

	switch (o->Type)
	{
	case MODEL_MONSTER01 + 166:
		{
			mu_float _fActSpdTemp = b->_AnimationHeader[o->CurrentAction].Speed;
			EVector3 EndPos, EndRelative, Light, vPos;
			Vector(1.0f, 1.0f, 1.0f, Light);

			switch (o->CurrentAction)
			{
			case MONSTER01_STOP1:
			case MONSTER01_STOP2:
			case MONSTER01_ATTACK1:
			case MONSTER01_APEAR:
				{
					EVector3 Light;

					if (m_bCurrentIsRage_Raymond == true)
					{
						Vector(1.0f, 1.0f, 1.0f, Light);
						CreateInferno(ThreadIndex, o->Position);

						CreateEffect(ThreadIndex, MODEL_CIRCLE, o->Position, o->Angle, Light, 4, o);
						CreateEffect(ThreadIndex, MODEL_CIRCLE, o->Position, o->Angle, Light, 4, o);
						m_bCurrentIsRage_Raymond = false;
					}
				}
				break;
			case MONSTER01_DIE:
				break;
			case MONSTER01_WALK:
				{
					Vector(0.9f, 0.2f, 0.1f, Light);
					if (7.5f <= o->AnimationFrame && o->AnimationFrame < 8.0f)
					{
						b->TransformByObjectBone(ThreadIndex, vPos, o, 54);
						CreateParticle(ThreadIndex, BITMAP_SMOKE + 1, vPos, o->Angle, Light, 0, 0.1f);
					}
					if (0.0f <= o->AnimationFrame && o->AnimationFrame < 1.0f)
					{
						b->TransformByObjectBone(ThreadIndex, vPos, o, 59);
						CreateParticle(ThreadIndex, BITMAP_SMOKE + 1, vPos, o->Angle, Light, 0, 0.1f);
					}
				}
				break;
			case MONSTER01_ATTACK2:
				{
					if (o->AnimationFrame >= 2.5f && o->AnimationFrame < (2.5f + _fActSpdTemp))
					{
						Vector(0.0f, 0.0f, 100.0f, EndRelative);
						b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(31)], EndRelative, EndPos, true);
						CreateEffect(ThreadIndex, MODEL_SKILL_FURY_STRIKE, EndPos, o->Angle, Light, 1, o, -1, 0, 0, 0);
					}
				}
				break;
			case MONSTER01_ATTACK3:
				{
					if (o->AnimationFrame >= 7.0f && o->AnimationFrame < (7.0f + _fActSpdTemp))
					{
						CreateEffect(ThreadIndex, MODEL_CIRCLE, o->Position, o->Angle, o->Light, 0, 0, -1, 0, 0, 0);
						CreateEffect(ThreadIndex, MODEL_CIRCLE_LIGHT, o->Position, o->Angle, o->Light, 0, 0, -1, 0, 0, 0);
					}

					Vector(o->Position[0] + GetLargeRand(1024) - 512, o->Position[1] + GetLargeRand(1024) - 512, o->Position[2], EndPos);
					CreateEffect(ThreadIndex, MODEL_FIRE, EndPos, o->Angle, o->Light);
				}
				break;
			}
		}
		return true;
	case MODEL_MONSTER01 + 167:
		{
			switch (o->CurrentAction)
			{
			case MONSTER01_STOP1:
			case MONSTER01_STOP2:
				break;
			case MONSTER01_WALK:
				break;
			case MONSTER01_DIE:
				break;
			case MONSTER01_ATTACK1:
				{
					if (2.0f <= o->AnimationFrame && o->AnimationFrame < 15.0f)
					{
						EVector3  Light;
						Vector(0.3f, 0.8f, 0.4f, Light);

						EVector3 StartPos, StartRelative;
						EVector3 EndPos, EndRelative;

						mu_float fActionSpeed = b->_AnimationHeader[o->CurrentAction].Speed;
						mu_float fSpeedPerFrame = fActionSpeed / 10.0;
						mu_float fAnimationFrame = o->AnimationFrame - fActionSpeed;
						for (mu_int32 i = 0; i < 25; ++i)
						{
							b->Animation(ThreadIndex, ThreadInfo.BoneTransform, fAnimationFrame, o->PriorAnimationFrame, o->PriorAction, o->Angle, o->HeadAngle);

							Vector(0.0f, 0.0f, 0.0f, StartRelative);
							Vector(0.0f, 0.0f, 0.0f, EndRelative);

							b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(39)], StartRelative, StartPos, false);
							b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(40)], EndRelative, EndPos, false);
							CreateBlur(c, StartPos, EndPos, Light, 0);

							fAnimationFrame += fSpeedPerFrame;
						}
					}
				}
				break;
			case MONSTER01_ATTACK2:
				{
					if (4.0f <= o->AnimationFrame && o->AnimationFrame < 4.4f)
					{
						mu_int32 SkillIndex = 9;
						OBJECT* pObj = o;
						EVector3 ap, P, dp;
						EMatrix4 matrix;

						VectorCopy(pObj->Position, ap);
						CreateEffect(ThreadIndex, MODEL_DARK_SCREAM, pObj->Position, pObj->Angle, pObj->Light, 1, pObj, pObj->PKKey, SkillIndex);
						CreateEffect(ThreadIndex, MODEL_DARK_SCREAM_FIRE, pObj->Position, pObj->Angle, pObj->Light, 1, pObj, pObj->PKKey, SkillIndex);

						Vector(80.0f, 0.0f, 0.0f, P);

						pObj->Angle[2] += 10.0f;

						AngleMatrix(pObj->Angle, matrix);
						VectorRotate(P, matrix, dp);
						VectorAdd(dp, pObj->Position, pObj->Position);
						CreateEffect(ThreadIndex, MODEL_DARK_SCREAM, pObj->Position, pObj->Angle, pObj->Light, 1, pObj, pObj->PKKey, SkillIndex);
						CreateEffect(ThreadIndex, MODEL_DARK_SCREAM_FIRE, pObj->Position, pObj->Angle, pObj->Light, 1, pObj, pObj->PKKey, SkillIndex);

						VectorCopy(ap, pObj->Position);
						VectorCopy(pObj->Position, ap);
						Vector(-80.0f, 0.0f, 0.0f, P);
						pObj->Angle[2] -= 20.0f;

						AngleMatrix(pObj->Angle, matrix);
						VectorRotate(P, matrix, dp);
						VectorAdd(dp, pObj->Position, pObj->Position);
						CreateEffect(ThreadIndex, MODEL_DARK_SCREAM, pObj->Position, pObj->Angle, pObj->Light, 1, pObj, pObj->PKKey, SkillIndex);
						CreateEffect(ThreadIndex, MODEL_DARK_SCREAM_FIRE, pObj->Position, pObj->Angle, pObj->Light, 1, pObj, pObj->PKKey, SkillIndex);
						VectorCopy(ap, pObj->Position);
					}

					if (2.0f <= o->AnimationFrame && o->AnimationFrame < 15.0f)
					{
						EVector3  Light;
						Vector(0.3f, 0.8f, 0.4f, Light);

						EVector3 StartPos, StartRelative;
						EVector3 EndPos, EndRelative;

						mu_float fActionSpeed = b->_AnimationHeader[o->CurrentAction].Speed;
						mu_float fSpeedPerFrame = fActionSpeed / 10.0;
						mu_float fAnimationFrame = o->AnimationFrame - fActionSpeed;
						for (mu_int32 i = 0; i < 25; ++i)
						{
							b->Animation(ThreadIndex, ThreadInfo.BoneTransform, fAnimationFrame, o->PriorAnimationFrame, o->PriorAction, o->Angle, o->HeadAngle);

							Vector(0.0f, 0.0f, 0.0f, StartRelative);
							Vector(0.0f, 0.0f, 0.0f, EndRelative);

							b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(39)], StartRelative, StartPos, false);
							b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(40)], EndRelative, EndPos, false);
							CreateBlur(c, StartPos, EndPos, Light, 0);

							fAnimationFrame += fSpeedPerFrame;
						}
					}
				}
				break;
			case MONSTER01_ATTACK3:
				{
					if (o->AnimationFrame >= 3.4f && o->AnimationFrame <= 9.0f)
					{
						RenderSkillEarthQuake(ThreadIndex, c, o, b, 12);
					}

					if (2.0f <= o->AnimationFrame && o->AnimationFrame < 15.0f)
					{
						EVector3  Light;
						Vector(0.3f, 0.8f, 0.4f, Light);

						EVector3 StartPos, StartRelative;
						EVector3 EndPos, EndRelative;

						mu_float fActionSpeed = b->_AnimationHeader[o->CurrentAction].Speed;
						mu_float fSpeedPerFrame = fActionSpeed / 10.0;
						mu_float fAnimationFrame = o->AnimationFrame - fActionSpeed;
						for (mu_int32 i = 0; i < 25; ++i)
						{
							b->Animation(ThreadIndex, ThreadInfo.BoneTransform, fAnimationFrame, o->PriorAnimationFrame, o->PriorAction, o->Angle, o->HeadAngle);

							Vector(0.0f, 0.0f, 0.0f, StartRelative);
							Vector(0.0f, 0.0f, 0.0f, EndRelative);

							b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(39)], StartRelative, StartPos, false);
							b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(40)], EndRelative, EndPos, false);
							CreateBlur(c, StartPos, EndPos, Light, 0);

							fAnimationFrame += fSpeedPerFrame;
						}
					}
				}
				break;
			case MONSTER01_APEAR:
				{
					EVector3 Light;

					if (m_bCurrentIsRage_Ercanne == true)
					{
						Vector(1.0f, 1.0f, 1.0f, Light);
						CreateInferno(ThreadIndex, o->Position);

						CreateEffect(ThreadIndex, MODEL_CIRCLE, o->Position, o->Angle, Light, 4, o);
						CreateEffect(ThreadIndex, MODEL_CIRCLE, o->Position, o->Angle, Light, 4, o);
						m_bCurrentIsRage_Ercanne = false;
					}
				}
				break;
			}
		}
		return true;
	case MODEL_MONSTER01 + 168:
		{
			switch (o->CurrentAction)
			{
			case MONSTER01_STOP1:
			case MONSTER01_STOP2:
				break;
			case MONSTER01_WALK:
				break;
			case MONSTER01_DIE:
				break;
			case MONSTER01_ATTACK1:
				break;
			case MONSTER01_ATTACK2:
				{
					mu_float _fActSpdTemp = b->_AnimationHeader[o->CurrentAction].Speed;
					if (o->AnimationFrame >= 4.5f && o->AnimationFrame < (4.5f + _fActSpdTemp))
					{
						CHARACTER *tc = &CharactersClient[c->TargetCharacter];
						OBJECT *to = &tc->Object;

						EVector3 Angle, AngleRelative = { 0.0f, 0.0f, 0.0f };
						EVector3 v3PosTo = { to->Position[0], to->Position[1], to->Position[2] };
						EVector3 v3RelativePos, v3StartPos;

						Vector(o->Angle[0] + AngleRelative[0],
							o->Angle[1] + AngleRelative[1],
							o->Angle[2] + AngleRelative[2],
							Angle);

						Vector(0.0f, 0.0f, 0.0f, v3RelativePos);
						b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(20)], v3RelativePos, v3StartPos, true);
						CreateEffect(ThreadIndex, MODEL_DEASULER, v3StartPos, Angle, v3PosTo, 0, o, -1, 0, 0, 0, 1.8f);
					}
				}
				break;
			case MONSTER01_ATTACK3:
				{
					EVector3 Angle, p, Position;
					EBone Matrix;
					Vector(0.0f, -500.0f, 0.0f, p);
					for (mu_int32 j = 0; j < 3; ++j)
					{
						Vector((mu_float)(GetLargeRand(90)), 0.0f, (mu_float)(GetLargeRand(360)), Angle);
						AngleMatrix(Angle, Matrix);
						VectorRotate(p, Matrix, Position);
						VectorSubtract(o->Position, Position, Position);
						Position[2] += 120.0f;
						CreateJoint(ThreadIndex, BITMAP_JOINT_HEALING, Position, o->Position, Angle, 17, o, 10.0f);
					}
					EVector3 Light;
					Vector(1.0f, 0.1f, 0.0f, Light);
					Vector(0.0f, 0.0f, 0.0f, p);
					for (mu_int32 i = 0; i < 10; ++i)
					{
						b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(GetLargeRand(62))], p, Position, true);
						CreateParticle(ThreadIndex, BITMAP_LIGHT, Position, o->Angle, Light, 5, 0.5f + (GetLargeRand(100)) / 50.0f);

						b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(50)], p, Position, true);
						CreateParticle(ThreadIndex, BITMAP_LIGHT, Position, o->Angle, Light, 5, 0.5f + (GetLargeRand(100)) / 50.0f);
					}

					mu_float _fActSpdTemp = b->_AnimationHeader[o->CurrentAction].Speed;
					if (o->AnimationFrame >= 4.5f && o->AnimationFrame < (4.5f + _fActSpdTemp))
					{
						EVector3 EndPos, EndRelative;

						Vector(0.0f, 0.0f, 0.0f, EndRelative);
						b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(11)], EndRelative, EndPos, true);
						CreateEffect(ThreadIndex, MODEL_SKILL_FURY_STRIKE, EndPos, o->Angle, Light, 1, o, -1, 0, 0);
					}

				}
				break;
			case MONSTER01_APEAR:
				{
					EVector3 Light;

					if (m_bCurrentIsRage_Daesuler == true)
					{
						Vector(1.0f, 1.0f, 1.0f, Light);
						CreateInferno(ThreadIndex, o->Position);

						CreateEffect(ThreadIndex, MODEL_CIRCLE, o->Position, o->Angle, Light, 4, o);
						CreateEffect(ThreadIndex, MODEL_CIRCLE, o->Position, o->Angle, Light, 4, o);
						m_bCurrentIsRage_Daesuler = false;
					}
				}
				break;
			}
		}
		break;
	case MODEL_MONSTER01 + 171:
		{
			Vector(1.0f, 0.8f, 0.2f, Light);
			b->TransformByObjectBone(ThreadIndex, vPos, o, 81);
			CreateSprite(BITMAP_LIGHT_RED, vPos, 0.5f, Light, o);
			b->TransformByObjectBone(ThreadIndex, vPos, o, 82);
			CreateSprite(BITMAP_LIGHT_RED, vPos, 1.3f, Light, o);
			b->TransformByObjectBone(ThreadIndex, vPos, o, 83);
			CreateSprite(BITMAP_LIGHT_RED, vPos, 2.0f, Light, o);

			Vector(1.0f, 0.8f, 0.2f, Light);
			b->TransformByObjectBone(ThreadIndex, vPos, o, 105);
			CreateSprite(BITMAP_LIGHT_RED, vPos, 0.5f, Light, o);
			b->TransformByObjectBone(ThreadIndex, vPos, o, 104);
			CreateSprite(BITMAP_LIGHT_RED, vPos, 1.3f, Light, o);
			b->TransformByObjectBone(ThreadIndex, vPos, o, 103);
			CreateSprite(BITMAP_LIGHT_RED, vPos, 2.0f, Light, o);

			Vector(1.0f, 0.8f, 0.2f, Light);
			b->TransformByObjectBone(ThreadIndex, vPos, o, 10);
			CreateSprite(BITMAP_LIGHT_RED, vPos, 1.5f, Light, o);

			Vector(1.0f, 0.8f, 0.2f, Light);
			b->TransformByObjectBone(ThreadIndex, vPos, o, 9);
			CreateSprite(BITMAP_LIGHT_RED, vPos, 2.0f, Light, o);

			Vector(1.0f, 0.8f, 0.2f, Light);
			b->TransformByObjectBone(ThreadIndex, vPos, o, 6);
			CreateSprite(BITMAP_LIGHT_RED, vPos, 4.0f, Light, o);

			Vector(1.0f, 0.8f, 0.2f, Light);
			b->TransformByObjectBone(ThreadIndex, vPos, o, 13);
			CreateSprite(BITMAP_LIGHT_RED, vPos, 2.0f, Light, o);
			b->TransformByObjectBone(ThreadIndex, vPos, o, 30);
			CreateSprite(BITMAP_LIGHT_RED, vPos, 2.0f, Light, o);

			Vector(1.0f, 0.8f, 0.2f, Light);
			b->TransformByObjectBone(ThreadIndex, vPos, o, 32);
			CreateSprite(BITMAP_LIGHT_RED, vPos, 2.0f, Light, o);
			b->TransformByObjectBone(ThreadIndex, vPos, o, 75);
			CreateSprite(BITMAP_LIGHT_RED, vPos, 2.0f, Light, o);

			mu_float Luminosity = EMath::Sin(FWorldTime*0.003f)*0.2f + 0.6f;
			Vector(1.0f*Luminosity, 0.5f*Luminosity, 0.4f*Luminosity, Light);

			mu_int32 temp[] = { 49, 50, 51, 52, 53, 54, 55, 56, 57, 58,
				59, 60, 61, 62, 63, 64, 65, 66, 67, 71, 72 };

			for (mu_int32 i = 0; i < 21; ++i)
			{
				b->TransformByObjectBone(ThreadIndex, vPos, o, temp[i]);
				CreateSprite(BITMAP_LIGHTMARKS, vPos, 0.6f, Light, o);
			}
			b->TransformByObjectBone(ThreadIndex, vPos, o, 68);
			CreateSprite(BITMAP_LIGHTMARKS, vPos, 0.3f, Light, o);
			b->TransformByObjectBone(ThreadIndex, vPos, o, 69);
			CreateSprite(BITMAP_LIGHTMARKS, vPos, 0.2f, Light, o);
			b->TransformByObjectBone(ThreadIndex, vPos, o, 70);
			CreateSprite(BITMAP_LIGHTMARKS, vPos, 0.3f, Light, o);

			mu_float _fActSpdTemp = b->_AnimationHeader[o->CurrentAction].Speed;

			switch (o->CurrentAction)
			{
			case MONSTER01_STOP1:
			case MONSTER01_STOP2:
				break;
			case MONSTER01_WALK:
				break;
			case MONSTER01_DIE:
				break;
			case MONSTER01_ATTACK1:
				break;
			case MONSTER01_ATTACK2:
				{
					if (o->AnimationFrame >= 2.5f && o->AnimationFrame < (2.5f + _fActSpdTemp))
					{
						EVector3 EndPos, EndRelative;

						Vector(0.0f, 0.0f, 100.0f, EndRelative);
						b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(11)], EndRelative, EndPos, true);
						CreateEffect(ThreadIndex, MODEL_SKILL_FURY_STRIKE, EndPos, o->Angle, Light, 1, o, -1, 0, 0);
					}
				}
				break;
			case MONSTER01_ATTACK3:
				{
					if (o->AnimationFrame >= 3.4f && o->AnimationFrame <= 9.0f)
					{
						RenderSkillEarthQuake(ThreadIndex, c, o, b, 12);
					}
				}
				break;
			case MONSTER01_APEAR:
				{
					EVector3 Light;

					if (m_bCurrentIsRage_Gallia == true)
					{
						Vector(1.0f, 1.0f, 1.0f, Light);
						CreateInferno(ThreadIndex, o->Position);

						CreateEffect(ThreadIndex, MODEL_CIRCLE, o->Position, o->Angle, Light, 4, o);
						CreateEffect(ThreadIndex, MODEL_CIRCLE, o->Position, o->Angle, Light, 4, o);
						m_bCurrentIsRage_Gallia = false;
					}
				}
				break;
			}

		}
		return true;

	case MODEL_MONSTER01 + 173:
		{
			switch (o->CurrentAction)
			{
			case MONSTER01_WALK:
				{
					Vector(0.5f, 0.2f, 0.1f, o->Light);

					if (7.0f <= o->AnimationFrame && o->AnimationFrame < 8.0f)
					{
						b->TransformByObjectBone(ThreadIndex, vPos, o, 53);
						vPos[2] += 25.0f;
						CreateParticle(ThreadIndex, BITMAP_SMOKE + 1, vPos, o->Angle, o->Light, 1);
					}
					if (1.0f <= o->AnimationFrame && o->AnimationFrame < 2.0f)
					{
						b->TransformByObjectBone(ThreadIndex, vPos, o, 48);
						vPos[2] += 25.0f;
						CreateParticle(ThreadIndex, BITMAP_SMOKE + 1, vPos, o->Angle, o->Light, 1);
					}
				}
				break;
			case MONSTER01_ATTACK1:
				{

				}
				break;
			case MONSTER01_ATTACK2:
				{

				}
				break;
			case MONSTER01_APEAR:
				{
					if (o->AnimationFrame >= 5.0f && o->AnimationFrame <= 5.8f)
					{
						CreateEffect(ThreadIndex, MODEL_WAVES, o->Position, o->Angle, o->Light, 1);
						CreateEffect(ThreadIndex, MODEL_WAVES, o->Position, o->Angle, o->Light, 1);
						CreateEffect(ThreadIndex, MODEL_PIERCING2, o->Position, o->Angle, o->Light);
						PlayBackground(SOUND_ATTACK_SPEAR);
					}
				}
				break;
			}
		}
		return true;
	case MODEL_MONSTER01 + 178:
		{
			switch (o->CurrentAction)
			{
			case MONSTER01_ATTACK1:
				{

				}
				break;
			case MONSTER01_ATTACK2:
				{
					if (4.0f <= o->AnimationFrame && o->AnimationFrame < 5.0f)
					{
						EVector3 vLook, vPosition, vLight;
						EBone matRotate;
						Vector(0.0f, -100.0f, 100.0f, vPosition);
						AngleMatrix(o->Angle, matRotate);
						VectorRotate(vPosition, matRotate, vLook);
						VectorAdd(o->Position, vLook, vPosition);

						Vector(0.8f, 0.8f, 1.0f, vLight);

						CreateEffect(ThreadIndex, MODEL_EFFECT_EG_GUARDIANDEFENDER_ATTACK2, vPosition, o->Angle, vLight, 0, o);
					}
				}
				break;
			case MONSTER01_APEAR:
				{
					VectorCopy(o->EyeRight3, o->Angle);
				}
				break;
			};

			VectorCopy(o->Angle, o->EyeRight3);
		}
		return true;
	case MODEL_MONSTER01 + 179:
		{
			EVector3 vRelative, vLight, vPos;
			switch (o->CurrentAction)
			{
			case MONSTER01_STOP1:
			case MONSTER01_STOP2:
			case MONSTER01_WALK:
				{
					Vector(10.0f, 0.0f, 0.0f, vRelative);
					b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(55)], vRelative, vPos, true);
					mu_float fLumi = (EMath::Sin(FWorldTime*0.001f) + 1.0f) * 0.3f + 0.4f;
					Vector(fLumi*0.5f, fLumi*0.5f, fLumi*0.5f, vLight);
					CreateSprite(BITMAP_FLARE_BLUE, vPos, 1.5f, vLight, o);
					Vector(0.5f, 0.5f, 0.5f, vLight);

					CreateSprite(BITMAP_FLARE_BLUE, vPos, 1.2f, vLight, o, -FWorldTime*0.1f);
				}
				break;
			case MONSTER01_DIE:
				break;
			case MONSTER01_ATTACK1:
				{
					Vector(10.0f, 0.0f, 0.0f, vRelative);
					b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(55)], vRelative, vPos, true);
					mu_float fLumi = (EMath::Sin(FWorldTime*0.001f) + 1.0f) * 0.3f + 0.4f;
					mu_float fScale = (EMath::Sin(FWorldTime*0.001f) + 1.0f)*0.5f + 1.0f;
					Vector(fLumi*0.5f, fLumi*0.5f, fLumi*0.5f, vLight);
					CreateSprite(BITMAP_FLARE_RED, vPos, 1.5f, vLight, o);
					Vector(0.5f, 0.5f, 0.5f, vLight);
					CreateSprite(BITMAP_FLARE_RED, vPos, 1.2f*fScale*o->AnimationFrame*0.15f, vLight, o, -FWorldTime*0.1f);

					CHARACTER *tc = &CharactersClient[c->TargetCharacter];
					OBJECT *to = &tc->Object;

					if (o->AnimationFrame > 10)
					{
						EVector3 p, Angle;
						Vector(0.0f, 0.0f, 0.0f, p);
						Vector(0.88f, 0.12f, 0.08f, vLight);
						b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(55)], p, vPos, true);
						Vector(-60.0f, 0.0f, o->Angle[2], Angle);
						Vector(-60.0f, 0.0f, o->Angle[2], Angle);
						CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, vPos, to->Position, Angle, 27, to, 50.0f, -1, 0, 0, 0, &vLight);
						CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, vPos, to->Position, Angle, 27, to, 10.0f, -1, 0, 0, 0, &vLight);
						CreateParticle(ThreadIndex, BITMAP_ENERGY, vPos, o->Angle, vLight);
					}
				}
				break;
			case MONSTER01_ATTACK2:
				{
					Vector(10.0f, 0.0f, 0.0f, vRelative);
					b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(55)], vRelative, vPos, true);
					mu_float fLumi = (EMath::Sin(FWorldTime*0.001f) + 1.0f) * 0.3f + 0.4f;
					mu_float fScale = (EMath::Sin(FWorldTime*0.001f) + 1.0f)*0.5f + 1.0f;
					Vector(fLumi*0.5f, fLumi*0.5f, fLumi*0.5f, vLight);
					CreateSprite(BITMAP_FLARE_BLUE, vPos, 1.5f, vLight, o);
					Vector(0.5f, 0.5f, 0.5f, vLight);
					CreateSprite(BITMAP_FLARE_BLUE, vPos, 1.2f*fScale*o->AnimationFrame*0.15f, vLight, o, -FWorldTime*0.1f);

					CHARACTER *tc = &CharactersClient[c->TargetCharacter];
					OBJECT *to = &tc->Object;

					if (o->AnimationFrame > 4.5f && o->AnimationFrame <= 4.8f)
						CreateEffect(ThreadIndex, BITMAP_MAGIC + 1, to->Position, to->Angle, to->Light, 1, to);
				}
				break;
			}
		}
		return true;
	case MODEL_MONSTER01 + 184:
		{
			if (o->CurrentAction == MONSTER01_DIE)
			{
				if (o->LifeTime == 100)
				{
					o->LifeTime = 90;

					Vector(0, 0, 200.0f, vRelativePos);
					b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(0)], vRelativePos, vPos, true);
					CreateEffect(ThreadIndex, MODEL_DOOR_CRUSH_EFFECT, vPos, o->Angle, o->Light, 0, o, 0, 0);
				}
			}
		}
		return true;
	case MODEL_MONSTER01 + 185:
		{
			if (o->CurrentAction == MONSTER01_DIE)
			{
				if (o->LifeTime == 100)
				{
					o->LifeTime = 90;

					Vector(0, 0, 0.0f, vRelativePos);
					b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(1)], vRelativePos, vPos, true);
					CreateEffect(ThreadIndex, MODEL_STATUE_CRUSH_EFFECT, vPos, o->Angle, o->Light, 0, o, 0, 0);

					CreateEffect(ThreadIndex, MODEL_STATUE_CRUSH_EFFECT_PIECE04, o->Position, o->Angle, o->Light, 0);

					EVector3 Angle;
					Vector(0.0f, 0.0f, (mu_float)(GetLargeRand(360)), Angle);

					CreateEffect(ThreadIndex, MODEL_STONE1 + GetLargeRand(2), vPos, Angle, o->Light, 0);
					CreateEffect(ThreadIndex, MODEL_STONE1 + GetLargeRand(2), vPos, Angle, o->Light, 0);
					CreateEffect(ThreadIndex, MODEL_STONE1 + GetLargeRand(2), vPos, Angle, o->Light, 0);
				}
			}
			else
			{
				mu_float Luminosity = EMath::Sin(FWorldTime*0.003f)*0.2f + 0.6f;
				Vector(1.0f*Luminosity, 0.8f*Luminosity, 0.2f*Luminosity, Light);

				b->TransformByObjectBone(ThreadIndex, vPos, o, 3);
				CreateSprite(BITMAP_LIGHT_RED, vPos, 1.0f, Light, o);
				b->TransformByObjectBone(ThreadIndex, vPos, o, 4);
				CreateSprite(BITMAP_LIGHT_RED, vPos, 1.0f, Light, o);
			}
		}
		return true;
	}

	return false;
}

void GMEmpireGuardian1::MoveBlurEffect(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	EVector3  Light;
	EVector3 StartPos, StartRelative;
	EVector3 EndPos, EndRelative;
	Vector(1.0f, 1.0f, 1.0f, Light);
	Vector(0.0f, 0.0f, 0.0f, StartRelative);
	Vector(0.0f, 0.0f, 0.0f, EndRelative);

	mu_float fActionSpeed = b->_AnimationHeader[o->CurrentAction].Speed;
	mu_float fSpeedPerFrame = fActionSpeed / 10.0;
	mu_float fAnimationFrame = o->AnimationFrame - fActionSpeed;

	switch (o->Type)
	{
	case MODEL_MONSTER01 + 166:
		{
			mu_float Start_Frame = 0.0f;
			mu_float End_Frame = 0.0f;

			switch (o->CurrentAction)
			{
			case MONSTER01_STOP1:
			case MONSTER01_STOP2:
			case MONSTER01_WALK:
			case MONSTER01_DIE:
				break;
			case MONSTER01_ATTACK1:
				{
					Start_Frame = 4.0f;
					End_Frame = 11.0f;
				}
				break;
			case MONSTER01_ATTACK2:
			case MONSTER01_ATTACK3:
				{
					Start_Frame = 5.0f;
					End_Frame = 10.0f;
				}
				break;
			}

			if ((o->AnimationFrame >= Start_Frame && o->AnimationFrame <= End_Frame)
				&& ((o->CurrentAction == MONSTER01_ATTACK1) || (o->CurrentAction == MONSTER01_ATTACK2) || (o->CurrentAction == MONSTER01_ATTACK3))
				)
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

					Vector(0.9f, 0.2f, 0.1f, Light);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(32)], StartRelative, StartPos, false);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(33)], EndRelative, EndPos, false);
					CreateBlur(c, StartPos, EndPos, Light, 0);

					fAnimationFrame += fSpeedPerFrame;
				}
			}
		}
		break;
	case MODEL_MONSTER01 + 171:
		{
			switch (o->CurrentAction)
			{
			case MONSTER01_ATTACK1:
				{
					if (4.0f <= o->AnimationFrame && o->AnimationFrame < 6.0f)
					{
						Vector(1.0f, 0.2f, 0.0f, Light);
						Vector(0.0f, 0.0f, 0.0f, StartRelative);
						Vector(0.0f, 0.0f, 0.0f, EndRelative);

						for (mu_int32 i = 0; i < 28; ++i)
						{
							b->Animation(ThreadIndex, ThreadInfo.BoneTransform, fAnimationFrame, o->PriorAnimationFrame, o->PriorAction, o->Angle, o->HeadAngle);

							b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(69)], StartRelative, StartPos, false);
							b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(49)], EndRelative, EndPos, false);
							CreateBlur(c, StartPos, EndPos, Light, 7);

							fAnimationFrame += fSpeedPerFrame;
						}
					}
				}
				break;
			case MONSTER01_ATTACK2:
				{
					if (4.0f <= o->AnimationFrame && o->AnimationFrame < 7.5f)
					{
						Vector(1.0f, 1.0f, 1.0f, Light);
						Vector(0.0f, 0.0f, 0.0f, StartRelative);
						Vector(0.0f, 0.0f, 0.0f, EndRelative);

						for (mu_int32 i = 0; i < 28; ++i)
						{
							b->Animation(ThreadIndex, ThreadInfo.BoneTransform, fAnimationFrame, o->PriorAnimationFrame, o->PriorAction, o->Angle, o->HeadAngle);

							b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(69)], StartRelative, StartPos, false);
							b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(49)], EndRelative, EndPos, false);
							CreateBlur(c, StartPos, EndPos, Light, 2);

							fAnimationFrame += fSpeedPerFrame;
						}
					}
				}
				break;
			case MONSTER01_ATTACK3:
				{
					if (4.5f <= o->AnimationFrame && o->AnimationFrame < 7.8f)
					{
						Vector(1.0f, 0.4f, 0.0f, Light);
						Vector(0.0f, 0.0f, 0.0f, StartRelative);
						Vector(0.0f, 0.0f, 0.0f, EndRelative);

						for (mu_int32 i = 0; i < 28; ++i)
						{
							b->Animation(ThreadIndex, ThreadInfo.BoneTransform, fAnimationFrame, o->PriorAnimationFrame, o->PriorAction, o->Angle, o->HeadAngle);

							b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(69)], StartRelative, StartPos, false);
							b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(49)], EndRelative, EndPos, false);
							CreateBlur(c, StartPos, EndPos, Light, 8);

							fAnimationFrame += fSpeedPerFrame;
						}
					}
				}
				break;
			}
		}
		break;
	case MODEL_MONSTER01 + 172:
		{
			if (o->CurrentAction == MONSTER01_ATTACK1 || o->CurrentAction == MONSTER01_ATTACK2)
			{
				EVector3  vLight;
				Vector(0.5f, 0.5f, 0.7f, vLight);

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
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(33)], StartRelative, StartPos, false);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(34)], EndRelative, EndPos, false);
					CreateBlur(c, StartPos, EndPos, vLight, 0, false, 0);

					fAnimationFrame += fSpeedPerFrame;
				}
			}
		}
		break;
	case MODEL_MONSTER01 + 173:
		{
			switch (o->CurrentAction)
			{
			case MONSTER01_ATTACK1:
				{
					if (4.0f <= o->AnimationFrame && o->AnimationFrame < 11.7f)
					{
						Vector(1.0f, 0.2f, 0.0f, Light);
						Vector(0.0f, 0.0f, 0.0f, StartRelative);
						Vector(0.0f, 0.0f, 0.0f, EndRelative);

						for (mu_int32 i = 0; i < 18; ++i)
						{
							b->Animation(ThreadIndex, ThreadInfo.BoneTransform, fAnimationFrame, o->PriorAnimationFrame, o->PriorAction, o->Angle, o->HeadAngle);

							b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(36)], StartRelative, StartPos, false);
							b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(37)], EndRelative, EndPos, false);
							CreateBlur(c, StartPos, EndPos, Light, 7);

							fAnimationFrame += fSpeedPerFrame;
						}
					}
				}
				break;
			case MONSTER01_ATTACK2:
				{
					if (4.0f <= o->AnimationFrame && o->AnimationFrame < 6.840f)
					{
						Vector(1.0f, 1.0f, 1.0f, Light);
						Vector(0.0f, 0.0f, 0.0f, StartRelative);
						Vector(0.0f, 0.0f, 0.0f, EndRelative);

						for (mu_int32 i = 0; i < 16; ++i)
						{
							b->Animation(ThreadIndex, ThreadInfo.BoneTransform, fAnimationFrame, o->PriorAnimationFrame, o->PriorAction, o->Angle, o->HeadAngle);

							b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(36)], StartRelative, StartPos, false);
							b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(37)], EndRelative, EndPos, false);
							CreateBlur(c, StartPos, EndPos, Light, 1);

							fAnimationFrame += fSpeedPerFrame;
						}
					}
				}
				break;
			case MONSTER01_APEAR:
				{
					if (4.0f <= o->AnimationFrame && o->AnimationFrame < 6.0f)
					{
						Vector(1.0f, 0.2f, 0.0f, Light);
						Vector(0.0f, 0.0f, 0.0f, StartRelative);
						Vector(0.0f, 0.0f, 0.0f, EndRelative);

						for (mu_int32 i = 0; i < 28; ++i)
						{
							b->Animation(ThreadIndex, ThreadInfo.BoneTransform, fAnimationFrame, o->PriorAnimationFrame, o->PriorAction, o->Angle, o->HeadAngle);

							b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(36)], StartRelative, StartPos, false);
							b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(37)], EndRelative, EndPos, false);
							CreateBlur(c, StartPos, EndPos, Light, 7);

							fAnimationFrame += fSpeedPerFrame;
						}
					}
				}
				break;
			}
		}
		break;
	case MODEL_MONSTER01 + 180:
		{
			if (o->CurrentAction == MONSTER01_ATTACK1 || o->CurrentAction == MONSTER01_ATTACK2)
			{
				EVector3  vLight;
				Vector(0.5f, 0.5f, 0.5f, vLight);

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
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(28)], StartRelative, StartPos, false);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(29)], EndRelative, EndPos, false);
					CreateBlur(c, StartPos, EndPos, vLight, 0, false, 0);

					fAnimationFrame += fSpeedPerFrame;
				}
			}
		}
		break;
	case MODEL_MONSTER01 + 181:
		{
			mu_float Start_Frame = 0.0f;
			mu_float End_Frame = 10.0f;
			if ((o->AnimationFrame >= Start_Frame && o->AnimationFrame <= End_Frame && o->CurrentAction == MONSTER01_ATTACK1)
				|| (o->AnimationFrame >= Start_Frame && o->AnimationFrame <= End_Frame && o->CurrentAction == MONSTER01_ATTACK2))
			{
				EVector3  Light;
				Vector(0.5f, 0.5f, 0.5f, Light);

				EVector3 StartPos, StartRelative;
				EVector3 EndPos, EndRelative;

				mu_float fActionSpeed = b->_AnimationHeader[o->CurrentAction].Speed;
				mu_float fSpeedPerFrame = fActionSpeed / 20.0;
				mu_float fAnimationFrame = o->AnimationFrame - fActionSpeed;
				for (mu_int32 i = 0; i < 20; ++i)
				{
					b->Animation(ThreadIndex, ThreadInfo.BoneTransform, fAnimationFrame, o->PriorAnimationFrame, o->PriorAction, o->Angle, o->HeadAngle);

					Vector(0.0f, 0.0f, 0.0f, StartRelative);
					Vector(0.0f, 0.0f, 0.0f, EndRelative);

					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(37)], StartRelative, StartPos, false);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(38)], EndRelative, EndPos, false);
					CreateBlur(c, StartPos, EndPos, Light, 0);

					fAnimationFrame += fSpeedPerFrame;
				}
			}
		}
		break;
	}
}

mu_boolean GMEmpireGuardian1::RenderObjectMesh(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b, mu_boolean ExtraMon)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	switch (o->Type)
	{
	case 0:
	case 1:
	case 3:
	case 44:
		{
			mu_float fLumi;
			fLumi = (EMath::Sin(FWorldTime*0.0015f) + 1.0f) * 0.4f + 0.2f;

			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 2, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		return true;

	case 81:
		{
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		return true;

	case 37:
		{
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		return true;

	case 41:
		{
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			mu_float Luminosity = EMath::Sin(FWorldTime*0.002f)*0.5f + 0.6f;
			Vector(Luminosity*b->BodyLight(ThreadIndex)[0], Luminosity*b->BodyLight(ThreadIndex)[1], Luminosity*b->BodyLight(ThreadIndex)[2], b->BodyLight(ThreadIndex));
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		return true;

	case 47:
		{
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			Vector(1.2f, 1.2f, 1.2f, b->BodyLight(ThreadIndex));
			b->RenderMesh(ThreadIndex, 3, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		return true;

	case 48:
		{
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			Vector(1.1f, 1.1f, 1.1f, b->BodyLight(ThreadIndex));
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		return true;

	case 49:
		{
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		return true;

	case 55:
		{
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		return true;

	case 64:
		{
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 3, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			Vector(1.0f, 0.0f, 0.0f, b->BodyLight(ThreadIndex));
			b->RenderMesh(ThreadIndex, 1, RENDER_BRIGHT | RENDER_CHROME, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 1, RENDER_BRIGHT | RENDER_CHROME, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		return true;

	case 70:
		{
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		return true;

	case 115:
		{
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);

			o->BlendMeshTexCoordV = IWorldTime % 25000 * 0.0004f;
			b->StreamMesh(ThreadIndex) = 1;
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);

			o->BlendMeshTexCoordV = IWorldTime % 25000 * -0.0004f;
			b->StreamMesh(ThreadIndex) = 3;
			b->RenderMesh(ThreadIndex, 3, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->StreamMesh(ThreadIndex) = -1;

			if(MU_UPDATECOUNTER > 0)
			{
				EVector3 vRelativePos, vWorldPos, Light, Angle;
				Vector(0, 0, 0, vRelativePos);
				Vector(1.0f, 1.0f, 1.0f, Light);
				Vector(0.0f, 0.0f, 0.0f, Angle);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(9)], vRelativePos, vWorldPos, false);

				if (0.0f < o->AnimationFrame && o->AnimationFrame < 0.4f)
				{
					if (o->SubType == 100)
					{
						CreateEffect(ThreadIndex, MODEL_PROJECTILE, vWorldPos, Angle, Light, 0, o);
						o->SubType = 101;
					}
				}
				else if (7.0f < o->AnimationFrame && o->AnimationFrame < 7.5f)
				{
					if (o->SubType == 101)
					{
						o->SubType = 100;
					}
				}
			}
		}
		return true;

	case 117:
		{
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 3, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);

			o->BlendMeshTexCoordV = IWorldTime % 25000 * 0.0004f;
			b->StreamMesh(ThreadIndex) = 1;
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);

			o->BlendMeshTexCoordV = IWorldTime % 25000 * -0.0004f;
			b->StreamMesh(ThreadIndex) = 4;
			b->RenderMesh(ThreadIndex, 4, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->StreamMesh(ThreadIndex) = -1;

			if(MU_UPDATECOUNTER > 0)
			{
				EVector3 vRelativePos, vWorldPos, Light, Angle;
				Vector(0, 0, 0, vRelativePos);
				Vector(1.0f, 1.0f, 1.0f, Light);
				Vector(0.0f, 0.0f, 0.0f, Angle);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(9)], vRelativePos, vWorldPos, false);

				if (0.0f < o->AnimationFrame && o->AnimationFrame < 0.4f)
				{
					if (o->SubType == 100)
					{
						CreateEffect(ThreadIndex, MODEL_PROJECTILE, vWorldPos, Angle, Light, 0, o);
						o->SubType = 101;
					}
				}
				else if (7.0f < o->AnimationFrame && o->AnimationFrame < 7.5f)
				{
					if (o->SubType == 101)
					{
						o->SubType = 100;
					}
				}
			}
		}
		return true;

	case MODEL_PROJECTILE:
		{
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		return true;
	case MODEL_DOOR_CRUSH_EFFECT_PIECE01:
	case MODEL_DOOR_CRUSH_EFFECT_PIECE02:
	case MODEL_DOOR_CRUSH_EFFECT_PIECE03:
	case MODEL_DOOR_CRUSH_EFFECT_PIECE04:
	case MODEL_DOOR_CRUSH_EFFECT_PIECE05:
	case MODEL_DOOR_CRUSH_EFFECT_PIECE06:
	case MODEL_DOOR_CRUSH_EFFECT_PIECE07:
	case MODEL_DOOR_CRUSH_EFFECT_PIECE08:
	case MODEL_DOOR_CRUSH_EFFECT_PIECE09:
	case MODEL_DOOR_CRUSH_EFFECT_PIECE10:
	case MODEL_DOOR_CRUSH_EFFECT_PIECE11:
	case MODEL_DOOR_CRUSH_EFFECT_PIECE12:
	case MODEL_DOOR_CRUSH_EFFECT_PIECE13:
	case MODEL_STATUE_CRUSH_EFFECT_PIECE01:
	case MODEL_STATUE_CRUSH_EFFECT_PIECE02:
	case MODEL_STATUE_CRUSH_EFFECT_PIECE03:
	case MODEL_STATUE_CRUSH_EFFECT_PIECE04:
		{
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		return true;

	case MODEL_MONSTER01 + 166:
	case MODEL_MONSTER01 + 167:
	case MODEL_MONSTER01 + 183:
	case MODEL_MONSTER01 + 184:
	case MODEL_MONSTER01 + 185:
	case MODEL_MONSTER01 + 171:
	case MODEL_MONSTER01 + 172:
	case MODEL_MONSTER01 + 178:
		{
			RenderMonster(ThreadIndex, o, b, ExtraMon);
		}
		return true;
	}

	return false;
}

mu_boolean GMEmpireGuardian1::RenderObjectVisual(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b)
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

mu_boolean GMEmpireGuardian1::RenderMonster(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b, mu_boolean ExtraMon)
{
	switch (o->Type)
	{
	case MODEL_MONSTER01 + 166:
		{
			mu_float fBlendMeshLight = (EMath::Sin(FWorldTime*0.003f) + 1.0f)*0.5f;
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			EVector3 _temp;
			VectorCopy(b->BodyLight(ThreadIndex), _temp);
			Vector(b->BodyLight(ThreadIndex)[0] * 3.0f, b->BodyLight(ThreadIndex)[0] * 3.0f, b->BodyLight(ThreadIndex)[0] * 3.0f, b->BodyLight(ThreadIndex));
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 1, fBlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_RAYMOND_SWORD);
			VectorCopy(_temp, b->BodyLight(ThreadIndex));
			b->RenderMesh(ThreadIndex, 3, RENDER_BRIGHT | RENDER_CHROME, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		return true;
	case MODEL_MONSTER01 + 167:
		{
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);

			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);

			b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);

			b->RenderMesh(ThreadIndex, 3, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 3, RENDER_BRIGHT | RENDER_CHROME, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);

			b->RenderMesh(ThreadIndex, 4, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		return true;
	case MODEL_MONSTER01 + 171:
		{
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 3, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);

			b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE | RENDER_CHROME4, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		return true;
	case MODEL_MONSTER01 + 172:
		{
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 2, RENDER_CHROME | RENDER_BRIGHT, 0.3f, 2, 0.3f, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		return true;
	case MODEL_MONSTER01 + 178:
		{
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		return true;
	case MODEL_MONSTER01 + 183:
		{
			mu_int32 tileX = mu_int32(o->Position[0] * g_TerrainScaleRev);
			mu_int32 tileY = mu_int32(o->Position[1] * g_TerrainScaleRev);

			switch (GET_WORLD)
			{
			case WD_69EMPIREGUARDIAN1:
				{
					if (tileX == 233 && tileY == 55)
					{
						o->Position[0] = 23350;	o->Position[1] = 5520;
					}
					else if ((165 <= tileX && tileX <= 167) && (25 <= tileY && tileY <= 27))
					{
						o->Position[0] = 16710;	o->Position[1] = 2620;
					}
				}break;
			case WD_70EMPIREGUARDIAN2:
				{
					if ((49 <= tileX && tileX <= 51) && (64 <= tileY && tileY <= 66))
					{
						o->Position[0] = 5075;	o->Position[1] = 6490;
					}
					else if ((40 <= tileX && tileX <= 42) && (116 <= tileY && tileY <= 118))
					{
						o->Position[0] = 4200;	o->Position[1] = 11680;
					}
				}break;
			case WD_71EMPIREGUARDIAN3:
				{
					if ((118 <= tileX && tileX <= 120) && (191 <= tileY && tileY <= 193))
					{
						o->Scale = 0.9f;
						o->Position[0] = 11985;	o->Position[1] = 19250;
					}
					else if ((221 <= tileX && tileX <= 223) && (159 <= tileY && tileY <= 161))
					{
						o->Scale = 1.08f;
						o->Position[0] = 22300;	o->Position[1] = 16000;
					}
				}break;
			}

			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		return true;

	case MODEL_MONSTER01 + 184:
		{
			mu_int32 tileX = mu_int32(o->Position[0] * g_TerrainScaleRev);
			mu_int32 tileY = mu_int32(o->Position[1] * g_TerrainScaleRev);

			switch (GET_WORLD)
			{
			case WD_69EMPIREGUARDIAN1:
				{
					if (tileX == 234 && tileY == 28)
					{
						o->Position[0] = 23450;	o->Position[1] = 2820;
					}
					else if (tileX == 216 && tileY == 80)
					{
						o->Position[0] = 21650;	o->Position[1] = 8000;
					}
					else if (tileX == 194 && tileY == 25)
					{
						o->Position[0] = 19450;	o->Position[1] = 2530;
					}
					else if ((153 <= tileX && tileX <= 155) && (52 <= tileY && tileY <= 54))
					{
						o->Scale = 1.15f;
						o->Position[0] = 15510;	o->Position[1] = 5360;
					}
					else if (tileX == 180 && tileY == 79)
					{
						o->Position[0] = 18070;	o->Position[1] = 7950;
					}
				}break;
			case WD_70EMPIREGUARDIAN2:
				{
					if ((74 <= tileX && tileX <= 76) && (66 <= tileY && tileY <= 68))
					{
						o->Scale = 1.17f;
						o->Position[0] = 7620;	o->Position[1] = 6740;
					}
					else if ((18 <= tileX && tileX <= 20) && (64 <= tileY && tileY <= 66))
					{
						o->Position[0] = 1950;	o->Position[1] = 6500;
					}
					else if ((36 <= tileX && tileX <= 38) && (92 <= tileY && tileY <= 94))
					{
						o->Scale = 1.1f;
						o->Position[0] = 3770;	o->Position[1] = 9250;
					}
					else if ((54 <= tileX && tileX <= 56) && (153 <= tileY && tileY <= 155))
					{
						o->Scale = 1.15f;
						o->Position[0] = 5515;	o->Position[1] = 15350;
					}
					else if ((106 <= tileX && tileX <= 108) && (111 <= tileY && tileY <= 113))
					{
						o->Scale = 1.05f;
						o->Position[0] = 10830;	o->Position[1] = 11180;
					}
				}break;
			case WD_71EMPIREGUARDIAN3:
				{
					if ((145 <= tileX && tileX <= 147) && (190 <= tileY && tileY <= 192))
					{
						o->Scale = 1.28f;
						o->Position[0] = 14700;	o->Position[1] = 19140;
					}
					else if ((88 <= tileX && tileX <= 90) && (194 <= tileY && tileY <= 196))
					{
						o->Scale = 1.10f;
						o->Position[0] = 9010;	o->Position[1] = 19580;
					}
					else if ((221 <= tileX && tileX <= 223) && (133 <= tileY && tileY <= 135))
					{
						o->Scale = 1.1f;
						o->Position[0] = 22300;	o->Position[1] = 13360;
					}
					else if ((222 <= tileX && tileX <= 224) && (192 <= tileY && tileY <= 194))
					{
						o->Scale = 1.1f;
						o->Position[0] = 22305;	o->Position[1] = 19280;
					}
					else if ((166 <= tileX && tileX <= 168) && (216 <= tileY && tileY <= 218))
					{
						o->Scale = 1.23f;
						o->Position[0] = 16720;	o->Position[1] = 21750;
					}
				}break;
			}

			if (o->CurrentAction != MONSTER01_DIE)
			{
				b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			}
		}
		return true;
	case MODEL_MONSTER01 + 185:
		{
			if (o->CurrentAction != MONSTER01_DIE)
			{
				b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			}
		}
		return true;
	}

	return false;
}

mu_boolean GMEmpireGuardian1::RenderMonsterVisual(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b)
{
	EVector3 vLight, vPos;

	switch (o->Type)
	{
	case MODEL_MONSTER01 + 166:
		{
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
	case MODEL_MONSTER01 + 167:
		{
			EVector3 vRelative;
			Vector(0.0f, 0.0f, 0.0f, vRelative);
			mu_float fLumi1 = (EMath::Sin(FWorldTime*0.004f) + 0.9f) * 0.25f;

			Vector(0.05f + fLumi1, 0.75f + fLumi1, 0.35f + fLumi1, vLight);

			Vector(0.0f, 0.0f, -10.0f, vRelative);
			b->TransformByObjectBone(ThreadIndex, vPos, o, 41, &vRelative);
			CreateSprite(BITMAP_LIGHT, vPos, 1.0f + fLumi1, vLight, o);

			Vector(0.0f, 0.0f, 10.0f, vRelative);
			b->TransformByObjectBone(ThreadIndex, vPos, o, 41, &vRelative);
			CreateSprite(BITMAP_LIGHT, vPos, 1.0f + fLumi1, vLight, o);

			Vector(0.0f, 0.0f, -10.0f, vRelative);
			b->TransformByObjectBone(ThreadIndex, vPos, o, 42, &vRelative);
			CreateSprite(BITMAP_LIGHT, vPos, 1.0f + fLumi1, vLight, o);

			Vector(0.0f, 0.0f, 10.0f, vRelative);
			b->TransformByObjectBone(ThreadIndex, vPos, o, 42, &vRelative);
			CreateSprite(BITMAP_LIGHT, vPos, 1.0f + fLumi1, vLight, o);

			mu_float fScale = 1.2f;
			for (mu_int32 i = 50; i <= 55; ++i)
			{
				mu_int32 iCurrentBoneIndex = i;

				Vector(0.0f, 6.0f, 0.0f, vRelative);
				b->TransformByObjectBone(ThreadIndex, vPos, o, iCurrentBoneIndex, &vRelative);
				CreateSprite(BITMAP_LIGHT, vPos, fScale + fLumi1, vLight, o);

				fScale = fScale - 0.15f;
			}

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
	case MODEL_MONSTER01 + 168:
		{
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
	case MODEL_MONSTER01 + 171:
		{
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
	case MODEL_MONSTER01 + 172:
		{
			mu_int32 i;
			Vector(0.5f, 0.5f, 0.8f, vLight);
			mu_int32 iBoneNumbers[] = { 10, 8, 12, 25 };
			for (i = 1; i < 4; ++i)
			{
				b->TransformByObjectBone(ThreadIndex, vPos, o, iBoneNumbers[i]);
				CreateSprite(BITMAP_LIGHT, vPos, 3.1f, vLight, o);
			}

			if (o->AnimationFrame > 2 && o->AnimationFrame < 3)
			{
				o->m_dwTime = 0;
				b->TransformByObjectBone(ThreadIndex, vPos, o, 11);

				Vector(1.0f, 1.0f, 1.0f, vLight);
				CreateParticle(ThreadIndex, BITMAP_SMOKE, vPos, o->Angle, vLight, 4, 1.0f);
			}
		}
		return true;
	case MODEL_MONSTER01 + 178:
		{
		}
		return true;
	case MODEL_MONSTER01 + 179:
		{
			if (o->CurrentAction != MONSTER01_ATTACK1 && o->CurrentAction != MONSTER01_ATTACK2)
			{
				EVector3 vRelative;
				Vector(10.0f, 0.0f, 0.0f, vRelative);
				b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(55)], vRelative, vPos, true);
				mu_float fLumi = (EMath::Sin(FWorldTime*0.001f) + 1.0f) * 0.3f + 0.4f;
				Vector(fLumi*0.5f, fLumi*0.5f, fLumi*0.5f, vLight);
				CreateSprite(BITMAP_FLARE_BLUE, vPos, 1.5f, vLight, o);
				Vector(0.5f, 0.5f, 0.5f, vLight);
				CreateSprite(BITMAP_FLARE_BLUE, vPos, 1.2f, vLight, o, -FWorldTime*0.1f);
			}
		}
		return true;
	}

	return false;
}

mu_boolean GMEmpireGuardian1::CreateRain(const mu_uint32 ThreadIndex, PARTICLE_HEADER *oh, PARTICLE* o)
{
	if (IsEmpireGuardian1() == false &&
		IsEmpireGuardian2() == false &&
		IsEmpireGuardian3() == false &&
		IsEmpireGuardian4() == false)
	{
		return false;
	}

	if (m_iWeather == WEATHER_TYPE::WEATHER_RAIN || m_iWeather == WEATHER_TYPE::WEATHER_STORM)
	{
		o->Type = BITMAP_RAIN;
		Vector(Hero->Object.Position[0] + (mu_float)(GetLargeRand(800) - 300),
			Hero->Object.Position[1] + (mu_float)(GetLargeRand(800) - 400),
			Hero->Object.Position[2] + (mu_float)(GetLargeRand(200) + 400),
			o->Position);
		Vector(-30.0f, 0.0f, 0.0f, o->Angle);
		EVector3 Velocity;
		Vector(0.0f, 0.0f, -(mu_float)(GetLargeRand(24) + 30), Velocity);
		EBone Matrix;
		AngleMatrix(o->Angle, Matrix);
		VectorRotate(Velocity, Matrix, o->Velocity);

		VectorAdd(o->Position, o->Velocity, o->Position);
		mu_float Height = RequestTerrainHeight(o->Position[0], o->Position[1]);
		if (GetLargeRand(2) == 0)
		{
			oh->Flags.Clear(PARTICLE::ePF_LIVE);
			o->Position[2] = Height + 10.0f;
			if (GetLargeRand(4) == 0)
				CreateParticle(ThreadIndex, BITMAP_RAIN_CIRCLE, o->Position, o->Angle, o->Light);
			else
				CreateParticle(ThreadIndex, BITMAP_RAIN_CIRCLE + 1, o->Position, o->Angle, o->Light);
		}
	}

	return true;
}

mu_boolean GMEmpireGuardian1::SetCurrentActionMonster(CHARACTER* c, OBJECT* o)
{
	if (IsEmpireGuardian1() == false &&
		IsEmpireGuardian2() == false &&
		IsEmpireGuardian3() == false &&
		IsEmpireGuardian4() == false)
	{
		return false;
	}

	switch (c->MonsterIndex)
	{
	case 506:
		{
			if (m_bCurrentIsRage_Raymond == true)
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
			case 52:
				{
					SetAction(o, MONSTER01_ATTACK3);
					c->MonsterSkill = -1;
				}
				break;
			case ATMON_SKILL_EMPIREGUARDIAN_BERSERKER:
				{
					SetAction(o, MONSTER01_APEAR);
					c->MonsterSkill = -1;

					m_bCurrentIsRage_Raymond = true;
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
	case 507:
		{
			if (m_bCurrentIsRage_Ercanne == true)
			{
				SetAction(o, MONSTER01_APEAR);
				c->MonsterSkill = -1;
				return true;
			}

			switch (c->MonsterSkill)
			{
			case 62:
				{
					SetAction(o, MONSTER01_ATTACK2);
					c->MonsterSkill = -1;
				}
				break;
			case 63:
				{
					SetAction(o, MONSTER01_ATTACK3);
					c->MonsterSkill = -1;
				}
				break;
			case ATMON_SKILL_EMPIREGUARDIAN_BERSERKER:
				{
					SetAction(o, MONSTER01_APEAR);
					c->MonsterSkill = -1;

					m_bCurrentIsRage_Ercanne = true;
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
	case 508:
		{
			if (m_bCurrentIsRage_Daesuler == true)
			{
				SetAction(o, MONSTER01_APEAR);
				c->MonsterSkill = -1;
				return true;
			}

			switch (c->MonsterSkill)
			{
			case 57:
				{
					SetAction(o, MONSTER01_ATTACK2);
					c->MonsterSkill = -1;
				}
				break;
			case 58:
				{
					SetAction(o, MONSTER01_ATTACK3);
					c->MonsterSkill = -1;
				}
				break;
			case ATMON_SKILL_EMPIREGUARDIAN_BERSERKER:
				{
					SetAction(o, MONSTER01_APEAR);
					c->MonsterSkill = -1;

					m_bCurrentIsRage_Daesuler = true;
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
	case 511:
		{
			if (m_bCurrentIsRage_Gallia == true)
			{
				SetAction(o, MONSTER01_APEAR);
				c->MonsterSkill = -1;
				return true;
			}

			switch (c->MonsterSkill)
			{
			case 58:
				{
					SetAction(o, MONSTER01_ATTACK2);
					c->MonsterSkill = -1;
				}
				break;
			case 60:
				{
					SetAction(o, MONSTER01_ATTACK3);
					c->MonsterSkill = -1;
				}
				break;
			case ATMON_SKILL_EMPIREGUARDIAN_BERSERKER:
				{
					SetAction(o, MONSTER01_APEAR);
					c->MonsterSkill = -1;

					m_bCurrentIsRage_Gallia = true;
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
	case 513:
		{
			switch (c->MonsterSkill)
			{
			case 47:
				{
					SetAction(o, MONSTER01_ATTACK2);
					c->MonsterSkill = -1;
				}
				break;
			case 49:
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
	case 518:
		{
			switch (c->MonsterSkill)
			{
			case 44:
				{
					SetAction(o, MONSTER01_APEAR);
					c->MonsterSkill = -1;
				}
				break;
			case 45:
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

mu_boolean GMEmpireGuardian1::PlayMonsterSound(OBJECT* o)
{
	if (IsEmpireGuardian1() == false &&
		IsEmpireGuardian2() == false &&
		IsEmpireGuardian3() == false &&
		IsEmpireGuardian4() == false)
	{
		return false;
	}

	mu_float fDis_x, fDis_y;
	fDis_x = o->Position[0] - Hero->Object.Position[0];
	fDis_y = o->Position[1] - Hero->Object.Position[1];
	mu_float fDistance = EMath::Sqrt(fDis_x*fDis_x + fDis_y*fDis_y);

	if (fDistance > 500.0f)
		return true;

	switch (o->Type)
	{
	case MODEL_MONSTER01 + 166:
		{
			switch (o->CurrentAction)
			{
			case MONSTER01_WALK:
				{
					if (7.0f <= o->AnimationFrame && o->AnimationFrame < 8.0f)
					{
						PlayBackground(SOUND_EMPIREGUARDIAN_JERINT_MONSTER_MOVE01);
					}
					if (1.0f <= o->AnimationFrame && o->AnimationFrame < 2.0f)
					{
						PlayBackground(SOUND_EMPIREGUARDIAN_JERINT_MONSTER_MOVE02);
					}
				}
				break;
			case MONSTER01_ATTACK1:
				{
					PlayBackground(SOUND_EMPIREGUARDIAN_JERINT_MONSTER_ATTACK01);
				}
				break;
			case MONSTER01_ATTACK2:
				{
					PlayBackground(SOUND_EMPIREGUARDIAN_RAYMOND_MONSTER_ATTACK02);
				}
				break;
			case MONSTER01_ATTACK3:
				{
					PlayBackground(SOUND_EMPIREGUARDIAN_RAYMOND_MONSTER_ATTACK02);
				}
				break;
			case MONSTER01_APEAR:
				{
					PlayBackground(SOUND_EMPIREGUARDIAN_RAYMOND_MONSTER_RAGE);
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
	case MODEL_MONSTER01 + 167:
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
					PlayBackground(SOUND_EMPIREGUARDIAN_JERINT_MONSTER_ATTACK01);
				}
				break;
			case MONSTER01_ATTACK2:
				{
					PlayBackground(SOUND_EMPIREGUARDIAN_JERINT_MONSTER_ATTACK03);
				}
				break;
			case MONSTER01_ATTACK3:
				{
					PlayBackground(SOUND_EMPIREGUARDIAN_ERCANNE_MONSTER_ATTACK03);
				}
				break;
			case MONSTER01_APEAR:
				{
					PlayBackground(SOUND_EMPIREGUARDIAN_RAYMOND_MONSTER_RAGE);
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
	case MODEL_MONSTER01 + 168:
		{
			switch (o->CurrentAction)
			{
			case MONSTER01_WALK:
				{
					if (7.0f <= o->AnimationFrame && o->AnimationFrame < 8.0f)
					{
						PlayBackground(SOUND_EMPIREGUARDIAN_JERINT_MONSTER_MOVE01);
					}
					if (1.0f <= o->AnimationFrame && o->AnimationFrame < 2.0f)
					{
						PlayBackground(SOUND_EMPIREGUARDIAN_JERINT_MONSTER_MOVE02);
					}
				}
				break;
			case MONSTER01_ATTACK1:
				{
					PlayBackground(SOUND_EMPIREGUARDIAN_COMBATMASTER_ATTACK01);
				}
				break;
			case MONSTER01_ATTACK2:
				{
					PlayBackground(SOUND_EMPIREGUARDIAN_1CORP_DEASULER_MONSTER_ATTACK02);
				}
				break;
			case MONSTER01_ATTACK3:
				{
					PlayBackground(SOUND_EMPIREGUARDIAN_1CORP_DEASULER_MONSTER_ATTACK03);
				}
				break;
			case MONSTER01_APEAR:
				{
					PlayBackground(SOUND_EMPIREGUARDIAN_JERINT_MONSTER_RAGE);
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

	case MODEL_MONSTER01 + 171:
		{
			switch (o->CurrentAction)
			{
			case MONSTER01_WALK:
				{
					PlayBackground(SOUND_EMPIREGUARDIAN_3CORP_CATO_MOVE);
				}
				break;
			case MONSTER01_ATTACK1:
				{
					PlayBackground(SOUND_EMPIREGUARDIAN_JERINT_MONSTER_ATTACK01);
				}
				break;
			case MONSTER01_ATTACK2:
			case MONSTER01_ATTACK3:
				{
					PlayBackground(SOUND_EMPIREGUARDIAN_4CORP_GALLIA_ATTACK02);
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

	case MODEL_MONSTER01 + 172:
		{
			switch (o->CurrentAction)
			{
			case MONSTER01_WALK:
				{
					if (7.0f <= o->AnimationFrame && o->AnimationFrame < 8.0f)
					{
						PlayBackground(SOUND_EMPIREGUARDIAN_JERINT_MONSTER_MOVE01);
					}
					if (1.0f <= o->AnimationFrame && o->AnimationFrame < 2.0f)
					{
						PlayBackground(SOUND_EMPIREGUARDIAN_JERINT_MONSTER_MOVE02);
					}
				}
				break;
			case MONSTER01_ATTACK1:
				{
					PlayBackground(SOUND_EMPIREGUARDIAN_JERINT_MONSTER_ATTACK01);
				}
				break;
			case MONSTER01_ATTACK2:
			case MONSTER01_ATTACK3:
				{
					PlayBackground(SOUND_EMPIREGUARDIAN_QUATERMASTER_ATTACK02);
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

	case MODEL_MONSTER01 + 173:
		{
			switch (o->CurrentAction)
			{
			case MONSTER01_WALK:
				{
					if (7.0f <= o->AnimationFrame && o->AnimationFrame < 8.0f)
					{
						PlayBackground(SOUND_EMPIREGUARDIAN_JERINT_MONSTER_MOVE01);
					}
					if (1.0f <= o->AnimationFrame && o->AnimationFrame < 2.0f)
					{
						PlayBackground(SOUND_EMPIREGUARDIAN_JERINT_MONSTER_MOVE02);
					}
				}
				break;
			case MONSTER01_ATTACK1:
				{
					PlayBackground(SOUND_EMPIREGUARDIAN_COMBATMASTER_ATTACK01);
				}break;

			case MONSTER01_ATTACK2:
				{
					PlayBackground(SOUND_EMPIREGUARDIAN_COMBATMASTER_ATTACK02);
				}break;

			case MONSTER01_ATTACK3:
				{
					PlayBackground(SOUND_EMPIREGUARDIAN_COMBATMASTER_ATTACK03);
				}break;
			case MONSTER01_DIE:
				{
					PlayBackground(SOUND_EMPIREGUARDIAN_JERINT_MONSTER_DEATH);
				}
				break;
			}
		}
		return true;
	case MODEL_MONSTER01 + 178:
		{
			switch (o->CurrentAction)
			{
			case MONSTER01_WALK:
				{
					if (7.0f <= o->AnimationFrame && o->AnimationFrame < 8.0f)
					{
						PlayBackground(SOUND_EMPIREGUARDIAN_JERINT_MONSTER_MOVE01);
					}
					if (1.0f <= o->AnimationFrame && o->AnimationFrame < 2.0f)
					{
						PlayBackground(SOUND_EMPIREGUARDIAN_JERINT_MONSTER_MOVE02);
					}
				}
				break;
			case MONSTER01_ATTACK1:
				{
					PlayBackground(SOUND_EMPIREGUARDIAN_COMBATMASTER_ATTACK01);
				}break;

			case MONSTER01_ATTACK2:
				{
					PlayBackground(SOUND_EMPIREGUARDIAN_COMBATMASTER_ATTACK02);
				}break;
			case MONSTER01_DIE:
				{
					PlayBackground(SOUND_EMPIREGUARDIAN_DEFENDER_ATTACK02);
				}
				break;
			}
		}
		return true;

	case MODEL_MONSTER01 + 179:
		{
			switch (o->CurrentAction)
			{
			case MONSTER01_STOP1:
			case MONSTER01_STOP2:
				{
					PlayBackground(SOUND_EMPIREGUARDIAN_PRIEST_STOP);
				}break;
			case MONSTER01_ATTACK1:
				{
					PlayBackground(SOUND_THUNDERS01);
				}break;

			case MONSTER01_ATTACK2:
				{
					PlayBackground(SOUND_RAKLION_SERUFAN_CURE);
				}break;
			case MONSTER01_DIE:
				{
					PlayBackground(SOUND_DARKLORD_DEAD);
				}
				break;
			}
		}
		return true;
	}

	return false;
}

void GMEmpireGuardian1::PlayBGM()
{
	if (IsEmpireGuardian())
	{
		EMusicBackend::Play(MUSICS::Data[MUSIC_EMPIREGUARDIAN]);
	}
	else
	{
		EMusicBackend::Stop(MUSICS::Data[MUSIC_EMPIREGUARDIAN]);
	}
}