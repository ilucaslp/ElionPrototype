#include "stdafx.h"
#include "GMEmpireGuardian4.h"

GMEmpireGuardian4 g_EmpireGuardian4;

GMEmpireGuardian4::GMEmpireGuardian4()
{

}

GMEmpireGuardian4::~GMEmpireGuardian4()
{

}

mu_boolean GMEmpireGuardian4::CreateObject(mu_uint32 objectIndex, OBJECT* o)
{
	switch (o->Type)
	{
	case 129:
	case 130:
	case 131:
	case 132:
		{
			o->Angle[2] = (mu_float)((mu_int32)o->Angle[2] % 360);
			VectorCopy(o->Angle, o->HeadAngle);
			VectorCopy(o->Position, o->HeadTargetAngle);
		}
		return true;

	case 115:
	case 117:
		{
			o->SubType = 100;
		}
		return true;
	case 10:
		{
			o->SubType = GetLargeRand(50);
		}
		return true;
	}

	return false;
}

CHARACTER* GMEmpireGuardian4::CreateMonster(const mu_uint32 ThreadIndex, mu_int32 iType, mu_int32 PosX, mu_int32 PosY, mu_int32 Key)
{
	CHARACTER* pCharacter = nullptr;

	switch (iType)
	{
	case 504:
		{
			pCharacter = CreateCharacter(Key, MODEL_MONSTER01 + 164, PosX, PosY);

			pCharacter->Object.Scale = 1.40f;

			m_bCurrentIsRage_BossGaion = false;
		}
		break;
	case 505:
	case 522:
		{
			pCharacter = CreateCharacter(Key, MODEL_MONSTER01 + 165, PosX, PosY);

			pCharacter->Object.Scale = 1.45f;

			m_bCurrentIsRage_Jerint = false;
		}
		break;
	case 527:
		{
			pCharacter = CreateCharacter(Key, MODEL_MONSTER01 + 186, PosX, PosY);

			pCharacter->Object.Scale = 1.25f;
		}
		break;

	case 528:
		{
			pCharacter = CreateCharacter(Key, MODEL_MONSTER01 + 187, PosX, PosY);

			pCharacter->Object.LifeTime = 100;
			pCharacter->Object.Scale = 1.25f;
		}
		break;
	}

	return pCharacter;
}

mu_boolean GMEmpireGuardian4::MoveObject(const mu_uint32 ThreadIndex, OBJECT* o)
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

mu_boolean GMEmpireGuardian4::MoveMonsterVisual(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b)
{
	if (true == g_EmpireGuardian1.MoveMonsterVisual(ThreadIndex, c, o, b))
	{
		return true;
	}

	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	switch (o->Type)
	{
	case MODEL_MONSTER01 + 164:
		{
			switch (o->CurrentAction)
			{
			case MONSTER01_ATTACK1:
				{
					CreateEffect(ThreadIndex, MODEL_SWORDLEFT01_EMPIREGUARDIAN_BOSS_GAION_,
						o->Position, o->Angle, o->Light, 0, o, -1, 0, 0, 0, o->Scale);
					CreateEffect(ThreadIndex, MODEL_SWORDRIGHT01_EMPIREGUARDIAN_BOSS_GAION_,
						o->Position, o->Angle, o->Light, 0, o, -1, 0, 0, 0, o->Scale);
					CreateEffect(ThreadIndex, MODEL_SWORDMAIN01_EMPIREGUARDIAN_BOSS_GAION_,
						o->Position, o->Angle, o->Light, 0, o, -1, 0, 0, 0, o->Scale);

					if (o->AnimationFrame >= 0.0f && o->AnimationFrame < 0.5f)
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

						b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(2)], v3RelativePos, v3StartPos, true);
						CreateEffect(ThreadIndex, MODEL_SWORDLEFT02_EMPIREGUARDIAN_BOSS_GAION_,
							v3StartPos, Angle, v3PosTo, 1, o, -1, 0, 0, 0, o->Scale);

						b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(10)], v3RelativePos, v3StartPos, true);
						CreateEffect(ThreadIndex, MODEL_SWORDRIGHT02_EMPIREGUARDIAN_BOSS_GAION_,
							v3StartPos, Angle, v3PosTo, 1, o, -1, 0, 0, 0, o->Scale);
					}
				}
				break;
			case MONSTER01_ATTACK2:
				{
					CreateEffect(ThreadIndex, MODEL_SWORDLEFT02_EMPIREGUARDIAN_BOSS_GAION_,
						o->Position, o->Angle, o->Light, 0, o, -1, 0, 0, 0, o->Scale);
					CreateEffect(ThreadIndex, MODEL_SWORDRIGHT02_EMPIREGUARDIAN_BOSS_GAION_,
						o->Position, o->Angle, o->Light, 0, o, -1, 0, 0, 0, o->Scale);
					CreateEffect(ThreadIndex, MODEL_SWORDMAIN01_EMPIREGUARDIAN_BOSS_GAION_,
						o->Position, o->Angle, o->Light, 0, o, -1, 0, 0, 0, o->Scale);

					if (o->AnimationFrame >= 0.0f && o->AnimationFrame < 0.7f)
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

						b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(4)], v3RelativePos, v3StartPos, true);
						CreateEffect(ThreadIndex, MODEL_SWORDLEFT01_EMPIREGUARDIAN_BOSS_GAION_,
							v3StartPos, Angle, v3PosTo, 1, o, -1, 0, 0, 0, o->Scale);

						b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(8)], v3RelativePos, v3StartPos, true);
						CreateEffect(ThreadIndex, MODEL_SWORDRIGHT01_EMPIREGUARDIAN_BOSS_GAION_,
							v3StartPos, Angle, v3PosTo, 1, o, -1, 0, 0, 0, o->Scale);
					}
				}
				break;
			case MONSTER01_ATTACK3:
				{
					if (o->AnimationFrame >= 0.0f && o->AnimationFrame < 0.5f)
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

						b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(4)], v3RelativePos, v3StartPos, true);
						CreateEffect(ThreadIndex, MODEL_SWORDLEFT01_EMPIREGUARDIAN_BOSS_GAION_,
							v3StartPos, Angle, v3PosTo, 3, o, -1, 0, 0, 0, o->Scale);

						b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(8)], v3RelativePos, v3StartPos, true);
						CreateEffect(ThreadIndex, MODEL_SWORDRIGHT01_EMPIREGUARDIAN_BOSS_GAION_,
							v3StartPos, Angle, v3PosTo, 3, o, -1, 0, 0, 0, o->Scale);

						b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(2)], v3RelativePos, v3StartPos, true);
						CreateEffect(ThreadIndex, MODEL_SWORDLEFT02_EMPIREGUARDIAN_BOSS_GAION_,
							v3StartPos, Angle, v3PosTo, 3, o, -1, 0, 0, 0, o->Scale);

						b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(10)], v3RelativePos, v3StartPos, true);
						CreateEffect(ThreadIndex, MODEL_SWORDRIGHT02_EMPIREGUARDIAN_BOSS_GAION_,
							v3StartPos, Angle, v3PosTo, 3, o, -1, 0, 0, 0, o->Scale);

						b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(6)], v3RelativePos, v3StartPos, true);
						CreateEffect(ThreadIndex, MODEL_SWORDMAIN01_EMPIREGUARDIAN_BOSS_GAION_,
							v3StartPos, Angle, v3PosTo, 3, o, -1, 0, 0, 0, o->Scale);
					}
				}
				break;
			case MONSTER01_ATTACK4:
				{
					CreateEffect(ThreadIndex, MODEL_SWORDLEFT01_EMPIREGUARDIAN_BOSS_GAION_,
						o->Position, o->Angle, o->Light, 0, o, -1, 0, 0, 0, o->Scale);
					CreateEffect(ThreadIndex, MODEL_SWORDLEFT02_EMPIREGUARDIAN_BOSS_GAION_,
						o->Position, o->Angle, o->Light, 0, o, -1, 0, 0, 0, o->Scale);
					CreateEffect(ThreadIndex, MODEL_SWORDRIGHT01_EMPIREGUARDIAN_BOSS_GAION_,
						o->Position, o->Angle, o->Light, 0, o, -1, 0, 0, 0, o->Scale);
					CreateEffect(ThreadIndex, MODEL_SWORDRIGHT02_EMPIREGUARDIAN_BOSS_GAION_,
						o->Position, o->Angle, o->Light, 0, o, -1, 0, 0, 0, o->Scale);

					if (o->AnimationFrame >= 0.0f && o->AnimationFrame < 0.5f)
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

						b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(6)], v3RelativePos, v3StartPos, true);
						CreateEffect(ThreadIndex, MODEL_SWORDMAIN01_EMPIREGUARDIAN_BOSS_GAION_,
							v3StartPos, Angle, v3PosTo, 1, o, -1, 0, 0, 0, o->Scale);
					}
				}
				break;
			case MONSTER01_APEAR:
				{
					CreateEffect(ThreadIndex, MODEL_SWORDLEFT01_EMPIREGUARDIAN_BOSS_GAION_,
						o->Position, o->Angle, o->Light, 0, o, -1, 0, 0, 0, o->Scale);
					CreateEffect(ThreadIndex, MODEL_SWORDLEFT02_EMPIREGUARDIAN_BOSS_GAION_,
						o->Position, o->Angle, o->Light, 0, o, -1, 0, 0, 0, o->Scale);
					CreateEffect(ThreadIndex, MODEL_SWORDRIGHT01_EMPIREGUARDIAN_BOSS_GAION_,
						o->Position, o->Angle, o->Light, 0, o, -1, 0, 0, 0, o->Scale);
					CreateEffect(ThreadIndex, MODEL_SWORDRIGHT02_EMPIREGUARDIAN_BOSS_GAION_,
						o->Position, o->Angle, o->Light, 0, o, -1, 0, 0, 0, o->Scale);
					CreateEffect(ThreadIndex, MODEL_SWORDMAIN01_EMPIREGUARDIAN_BOSS_GAION_,
						o->Position, o->Angle, o->Light, 0, o, -1, 0, 0, 0, o->Scale);

					EVector3 Light;

					if (m_bCurrentIsRage_BossGaion == true)
					{
						Vector(1.0f, 1.0f, 1.0f, Light);
						CreateInferno(ThreadIndex, o->Position);

						CreateEffect(ThreadIndex, MODEL_CIRCLE, o->Position, o->Angle, Light, 4, o);
						CreateEffect(ThreadIndex, MODEL_CIRCLE, o->Position, o->Angle, Light, 4, o);
						m_bCurrentIsRage_BossGaion = false;
					}
				}
				break;
			default:
				{
					CreateEffect(ThreadIndex, MODEL_SWORDLEFT01_EMPIREGUARDIAN_BOSS_GAION_,
						o->Position, o->Angle, o->Light, 0, o, -1, 0, 0, 0, o->Scale);
					CreateEffect(ThreadIndex, MODEL_SWORDLEFT02_EMPIREGUARDIAN_BOSS_GAION_,
						o->Position, o->Angle, o->Light, 0, o, -1, 0, 0, 0, o->Scale);
					CreateEffect(ThreadIndex, MODEL_SWORDRIGHT01_EMPIREGUARDIAN_BOSS_GAION_,
						o->Position, o->Angle, o->Light, 0, o, -1, 0, 0, 0, o->Scale);
					CreateEffect(ThreadIndex, MODEL_SWORDRIGHT02_EMPIREGUARDIAN_BOSS_GAION_,
						o->Position, o->Angle, o->Light, 0, o, -1, 0, 0, 0, o->Scale);
					CreateEffect(ThreadIndex, MODEL_SWORDMAIN01_EMPIREGUARDIAN_BOSS_GAION_,
						o->Position, o->Angle, o->Light, 0, o, -1, 0, 0, 0, o->Scale);
				}
				break;
			}
		}
		return true;
	case MODEL_MONSTER01 + 182:
	case MODEL_MONSTER01 + 165:
		{
			switch (o->CurrentAction)
			{
			case MONSTER01_STOP1:
			case MONSTER01_STOP2:
				break;
			case MONSTER01_WALK:
				break;
			case MONSTER01_DIE:
				{
				}
				break;
			case MONSTER01_ATTACK1:
				{
					if (7.5f <= o->AnimationFrame && o->AnimationFrame < 10.8f)
					{
						EVector3  Light;
						Vector(1.0f, 0.5f, 0.2f, Light);

						EVector3 vPosBlur01, vPosBlurRelative01;
						EVector3 vPosBlur02, vPosBlurRelative02;

						mu_float fActionSpeed = b->_AnimationHeader[o->CurrentAction].Speed;
						mu_float fSpeedPerFrame = fActionSpeed / 10.0;
						mu_float fAnimationFrame = o->AnimationFrame - fActionSpeed;

						mu_int32 iSwordForceType = 0;

						for (mu_int32 i = 0; i < 14; ++i)
						{
							b->Animation(ThreadIndex, ThreadInfo.BoneTransform, fAnimationFrame, o->PriorAnimationFrame, o->PriorAction, o->Angle, o->HeadAngle);

							Vector(0.0f, 0.0f, 0.0f, vPosBlurRelative01);
							Vector(0.0f, 0.0f, 0.0f, vPosBlurRelative02);

							b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(61)], vPosBlurRelative01, vPosBlur01, false);
							b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(51)], vPosBlurRelative02, vPosBlur02, false);

							CreateObjectBlur(o, vPosBlur01, vPosBlur02, Light, iSwordForceType, true, 1, 30);
							CreateObjectBlur(o, vPosBlur01, vPosBlur02, Light, iSwordForceType, true, 2, 30);

							b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(52)], vPosBlurRelative01, vPosBlur01, false);
							b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(60)], vPosBlurRelative02, vPosBlur02, false);

							CreateObjectBlur(o, vPosBlur01, vPosBlur02, Light, iSwordForceType, true, 11, 30);
							CreateObjectBlur(o, vPosBlur01, vPosBlur02, Light, iSwordForceType, true, 12, 30);

							fAnimationFrame += fSpeedPerFrame;
						}
					}
				}
				break;
			case MONSTER01_ATTACK2:
				{
					if (o->AnimationFrame >= 3.0f && o->AnimationFrame <= 5.0f)
					{
						CreateEffect(ThreadIndex, BITMAP_GATHERING, o->Position, o->Angle, o->Light, 1, o);
					}

					if (8.0f <= o->AnimationFrame && o->AnimationFrame < 10.1f)
					{
						mu_float fActionSpeed = b->_AnimationHeader[o->CurrentAction].Speed;
						mu_float fSpeedPerFrame = fActionSpeed / 10.0;
						mu_float fAnimationFrame = o->AnimationFrame - fActionSpeed;

						EVector3	vRelative, vPosition, vRelative2;

						EVector3	vAngle, vRandomDir, vRandomDirPosition, vResultRandomPosition;
						EBone	matRandomRotation;
						Vector(0.0f, 0.0f, 0.0f, vAngle);

						Vector(0.0f, 0.0f, 0.0f, vRandomDirPosition);

						Vector(0.0f, 0.0f, 0.0f, vRelative);
						Vector(0.0f, 0.0f, 0.0f, vRelative2);
						for (mu_int32 i = 0; i < 100; ++i)
						{
							mu_float	fRandDistance = (mu_float)(GetLargeRand(100)) + 100;
							Vector(0.0f, fRandDistance, 0.0f, vRandomDir);

							b->Animation(ThreadIndex, ThreadInfo.BoneTransform, fAnimationFrame, o->PriorAnimationFrame, o->PriorAction, o->Angle, o->HeadAngle);

							b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(61)], vRelative, vPosition, false);
							CreateParticle(ThreadIndex, BITMAP_FIRE, vPosition, o->Angle, o->Light, 0);

							Vector((mu_float)(GetLargeRand(360)), 0.0f, (mu_float)(GetLargeRand(360)), vAngle);
							AngleMatrix(vAngle, matRandomRotation);
							VectorRotate(vRandomDir, matRandomRotation, vRandomDirPosition);
							VectorAdd(vPosition, vRandomDirPosition, vResultRandomPosition);
							CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, vResultRandomPosition, vPosition, vAngle, 3, nullptr, 10.0f, 10, 10);

							fAnimationFrame += fSpeedPerFrame;
						}

					}

					if (6.0f <= o->AnimationFrame && o->AnimationFrame < 10.1f)
					{
						EVector3  Light;
						Vector(1.0f, 1.0f, 1.0f, Light);

						EVector3 vPosBlur01, vPosBlurRelative01;
						EVector3 vPosBlur02, vPosBlurRelative02;

						mu_float fActionSpeed = b->_AnimationHeader[o->CurrentAction].Speed;
						mu_float fSpeedPerFrame = fActionSpeed / 10.0;
						mu_float fAnimationFrame = o->AnimationFrame - fActionSpeed;


						for (mu_int32 i = 0; i < 40; ++i)
						{
							b->Animation(ThreadIndex, ThreadInfo.BoneTransform, fAnimationFrame, o->PriorAnimationFrame, o->PriorAction, o->Angle, o->HeadAngle);

							Vector(0.0f, 0.0f, 0.0f, vPosBlurRelative01);
							Vector(0.0f, 0.0f, 0.0f, vPosBlurRelative02);

							b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(61)], vPosBlurRelative01, vPosBlur01, false);
							b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(51)], vPosBlurRelative02, vPosBlur02, false);

							CreateBlur(c, vPosBlur01, vPosBlur02, Light, 2);

							fAnimationFrame += fSpeedPerFrame;
						}
					}
				}
				break;
			case MONSTER01_ATTACK3:
				{
					if (o->AnimationFrame >= 0.2f && o->AnimationFrame <= 1.0f)
					{
						CHARACTER	*chT = &CharactersClient[c->TargetCharacter];
						EVector3	&v3TargetPos = chT->Object.Position;

						Vector(v3TargetPos[0], v3TargetPos[1], v3TargetPos[2], o->Light);
						CreateEffect(ThreadIndex, MODEL_EMPIREGUARDIAN_BLOW_OF_DESTRUCTION,
							o->Position, o->Angle, o->Light, 0, o, -1, 0, 0, 0, 1.0f);
					}

					if (9.8f <= o->AnimationFrame && o->AnimationFrame < 14.0f)
					{
						EVector3  Light;
						Vector(1.0f, 1.0f, 1.0f, Light);

						EVector3 vPosBlur03, vPosBlurRelative03;
						EVector3 vPosBlur04, vPosBlurRelative04;

						mu_float fActionSpeed = b->_AnimationHeader[o->CurrentAction].Speed;
						mu_float fSpeedPerFrame = fActionSpeed / 10.0;

						mu_float fAnimationFrame = o->AnimationFrame - fActionSpeed;
						for (mu_int32 i = 0; i < 10; ++i)
						{
							b->Animation(ThreadIndex, ThreadInfo.BoneTransform, fAnimationFrame, o->PriorAnimationFrame, o->PriorAction, o->Angle, o->HeadAngle);

							Vector(0.0f, 0.0f, 0.0f, vPosBlurRelative03);
							Vector(0.0f, 0.0f, 0.0f, vPosBlurRelative04);

							b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(52)], vPosBlurRelative04, vPosBlur04, false);
							b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(58)], vPosBlurRelative04, vPosBlur03, false);

							CreateObjectBlur(o, vPosBlur03, vPosBlur04, Light, 10, false, 0);
							CreateObjectBlur(o, vPosBlur03, vPosBlur04, Light, 10, false, 1);
							CreateObjectBlur(o, vPosBlur03, vPosBlur04, Light, 10, false, 2);

							fAnimationFrame += fSpeedPerFrame;
						}
					}
				}
				break;
			case MONSTER01_APEAR:
				{
					EVector3 Light;

					if (m_bCurrentIsRage_Jerint == true)
					{
						Vector(1.0f, 1.0f, 1.0f, Light);
						CreateInferno(ThreadIndex, o->Position);

						CreateEffect(ThreadIndex, MODEL_CIRCLE, o->Position, o->Angle, Light, 4, o);
						CreateEffect(ThreadIndex, MODEL_CIRCLE, o->Position, o->Angle, Light, 4, o);
						m_bCurrentIsRage_Jerint = false;
					}
				}
				break;
			}
		}
		return true;
	case MODEL_MONSTER01 + 187:
		{
			if (o->CurrentAction == MONSTER01_DIE)
			{
				if (o->LifeTime == 100)
				{
					o->LifeTime = 90;

					EVector3 vPos, vRelativePos;
					Vector(200.0f, 0.0f, 0.0f, vRelativePos);
					b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(0)], vRelativePos, vPos, true);
					CreateEffect(ThreadIndex, MODEL_DOOR_CRUSH_EFFECT, vPos, o->Angle, o->Light, 1, o, 0, 0);
				}
			}
		}
		return true;
	}

	return false;
}

void GMEmpireGuardian4::MoveBlurEffect(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b)
{
	switch (o->Type)
	{
	case MODEL_MONSTER01 + 166:
	case MODEL_MONSTER01 + 178:
	case MODEL_MONSTER01 + 179:
	case MODEL_MONSTER01 + 180:
	case MODEL_MONSTER01 + 181:
		{
			g_EmpireGuardian1.MoveBlurEffect(ThreadIndex, c, o, b);
		}
		break;
	}
}

mu_boolean GMEmpireGuardian4::RenderObjectMesh(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b, mu_boolean ExtraMon)
{
	if (true == g_EmpireGuardian1.RenderObjectMesh(ThreadIndex, o, b, ExtraMon))
	{
		return true;
	}

	switch (o->Type)
	{
	case 96:
	case 97:
	case 100:
		{
			Vector(0.170382, 0.170382, 0.170382, b->BodyLight(ThreadIndex));
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		return true;
	case MODEL_MONSTER01 + 186:
		{
			mu_int32 tileX = mu_int32(o->Position[0] * g_TerrainScaleRev);
			mu_int32 tileY = mu_int32(o->Position[1] * g_TerrainScaleRev);

			if ((49 <= tileX && tileX <= 51) && (68 <= tileY && tileY <= 70))
			{
				o->Scale = 1.0f;
				o->Position[0] = 5080;	o->Position[1] = 6920;
			}
			else if ((51 <= tileX && tileX <= 53) && (190 <= tileY && tileY <= 192))
			{
				o->Scale = 1.0f;
				o->Position[0] = 5270;	o->Position[1] = 19120;
			}
			else if ((196 <= tileX && tileX <= 198) && (131 <= tileY && tileY <= 133))
			{
				o->Scale = 1.0f;
				o->Position[0] = 19750;	o->Position[1] = 13220;
			}

			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		return true;

	case MODEL_MONSTER01 + 187:
		{
			if (o->CurrentAction != MONSTER01_DIE)
			{
				mu_int32 tileX = mu_int32(o->Position[0] * g_TerrainScaleRev);
				mu_int32 tileY = mu_int32(o->Position[1] * g_TerrainScaleRev);

				if ((80 <= tileX && tileX <= 82) && (68 <= tileY && tileY <= 70))
				{
					o->Scale = 0.8f;
					o->Position[0] = 8115;	o->Position[1] = 6880;
				}
				else if ((31 <= tileX && tileX <= 33) && (89 <= tileY && tileY <= 91))
				{
					o->Scale = 0.9f;
					o->Position[0] = 3250;	o->Position[1] = 9000;
				}
				else if ((33 <= tileX && tileX <= 35) && (175 <= tileY && tileY <= 177))
				{
					o->Scale = 0.8f;
					o->Position[0] = 3470;	o->Position[1] = 17600;
				}
				else if ((68 <= tileX && tileX <= 70) && (165 <= tileY && tileY <= 167))
				{
					o->Scale = 0.9f;
					o->Position[0] = 6915;	o->Position[1] = 16650;
				}
				else if ((155 <= tileX && tileX <= 157) && (131 <= tileY && tileY <= 133))
				{
					o->Scale = 0.9f;
					o->Position[0] = 15710;	o->Position[1] = 13250;
				}
				else if ((223 <= tileX && tileX <= 225) && (158 <= tileY && tileY <= 160))
				{
					o->Scale = 0.8f;
					o->Position[0] = 22500;	o->Position[1] = 16000;
				}
				else if ((213 <= tileX && tileX <= 215) && (23 <= tileY && tileY <= 25))
				{
					o->Scale = 0.9f;
					o->Position[0] = 21480;	o->Position[1] = 2430;
				}

				b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			}
		}
		return true;
	}

	return false;
}

mu_boolean GMEmpireGuardian4::RenderObjectVisual(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b)
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

	case 157:
		{
			if (GMNewTown::IsNewMap73_74())
				return true;

			EVector3 vPos, vRelativePos, vLightFire01, vLightFire02, vLightFlareFire, vLightSmoke, vAngle;
			Vector(0.0f, 0.0f, 0.0f, vPos);

			Vector(4.0f, 0.0f, 0.0f, vRelativePos);
			Vector(0.0f, 0.0f, 0.0f, vAngle);
			Vector(0.9f, 0.5f, 0.0f, vLightFire01);
			Vector(0.75f, 0.3f, 0.0f, vLightFire02);


			mu_int32	arriCandleFire[] = { 22, 23, 25 };
			mu_int32	arriCandleSmoke[] = { 26, 24, 21 };

			Vector(0.65f, 0.45f, 0.02f, vLightFlareFire);

			{
				for (mu_int32 i = 0; i < 3; ++i)
				{
					mu_int32 iCurBoneIdx = arriCandleFire[i];
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(iCurBoneIdx)], vRelativePos, vPos);

					for (mu_int32 j = 0; j < 5; ++j)
					{
						CreateParticle(ThreadIndex, BITMAP_FIRE_HIK3_MONO, vPos, vAngle, vLightFire01, 3, o->Scale*0.2f);
						CreateParticle(ThreadIndex, BITMAP_FIRE_HIK3_MONO, vPos, vAngle, vLightFire02, 3, o->Scale*0.1f);
					}

				}
			}

			Vector(4.0f, 0.0f, 0.0f, vRelativePos);
			Vector(1.0f, 1.0f, 1.0f, vLightSmoke);

			{
				for (mu_int32 i = 0; i < 3; ++i)
				{
					mu_int32 iCurBoneIdx = arriCandleSmoke[i];
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(iCurBoneIdx)], vRelativePos, vPos);

					for (mu_int32 j = 0; j < 4; ++j)
					{
						CreateParticle(ThreadIndex, BITMAP_SMOKE, vPos, o->Angle, vLightSmoke, 65, o->Scale * 0.1, o);
					}
				}
			}
		}
		return true;
	case 158:
		{
			CreateParticle(ThreadIndex, BITMAP_SMOKE, o->Position, o->Angle, Light, 64, o->Scale, o);
		}
		return true;
	}

	return false;
}

mu_boolean GMEmpireGuardian4::RenderMonster(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b, mu_boolean ExtraMon)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	switch (o->Type)
	{
	case MODEL_MONSTER01 + 164:
		{
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			ThreadInfo.ExtraFlag = REXF_CULL_FRONT;
			b->RenderMesh(ThreadIndex, 3, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			ThreadInfo.ExtraFlag = 0;
			b->RenderMesh(ThreadIndex, 4, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		return true;
	case MODEL_MONSTER01 + 165:
	case MODEL_MONSTER01 + 182:
		{
			EVector3 v3LightBackup = b->BodyLight(ThreadIndex);

			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);

			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);

			b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);

			b->RenderMesh(ThreadIndex, 3, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);

			Vector(0.3f, 0.3f, 0.3f, b->BodyLight(ThreadIndex));

			b->RenderMesh(ThreadIndex, 3, RENDER_BRIGHT | RENDER_CHROME, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);

			VectorCopy(v3LightBackup, b->BodyLight(ThreadIndex));
		}
		return true;
	}

	return false;
}

mu_boolean GMEmpireGuardian4::RenderMonsterVisual(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b)
{
	if (g_EmpireGuardian1.RenderMonsterVisual(ThreadIndex, c, o, b))
	{
		return true;
	}

	EVector3 vPos, vRelative, vLight;

	switch (o->Type)
	{
	case MODEL_MONSTER01 + 164:
		{
			VectorCopy(o->Position, b->BodyOrigin(ThreadIndex));
			Vector(0.0f, 0.0f, 0.0f, vRelative);

			mu_float fLumi1 = (EMath::Sin(FWorldTime*0.004f) + 1.0f) * 0.25f;
			mu_float fLumi2 = (EMath::Sin(FWorldTime*0.004f) + 1.0f) * 0.2f;

			Vector(0.9f + fLumi1, 0.3f + fLumi1, 0.2f + fLumi1, vLight);
			Vector(0.0f, 5.0f, 0.0f, vRelative);
			b->TransformByObjectBone(ThreadIndex, vPos, o, 57, &vRelative);

			CreateSprite(BITMAP_LIGHT_RED, vPos, 1.5f + fLumi2, vLight, o);
			CreateSprite(BITMAP_LIGHT_RED, vPos, 2.0f + fLumi2, vLight, o);

			Vector(0.0f, -10.0f, 0.0f, vRelative);
			b->TransformByObjectBone(ThreadIndex, vPos, o, 57, &vRelative);
			Vector(1.0f, 0.0f, 0.0f, vLight);
			CreateParticle(ThreadIndex, BITMAP_SPARK + 1, vPos, o->Angle, vLight, 10, 2.0f);

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
		break;
	case MODEL_MONSTER01 + 165:
	case MODEL_MONSTER01 + 182:
		{
			VectorCopy(o->Position, b->BodyOrigin(ThreadIndex));
			Vector(0.0f, 0.0f, 0.0f, vRelative);
			mu_float fLumi1 = (EMath::Sin(FWorldTime*0.004f) + 1.0f) * 0.05f;
			mu_float fLumi2 = (EMath::Sin(FWorldTime*0.004f) + 1.0f) * 0.2f;

			mu_float fSize = 1.6f;
			Vector(0.6f + fLumi1, 0.2f + fLumi1, 0.1f + fLumi1, vLight);
			Vector(0.0f, 0.0f, 0.0f, vRelative);

			b->TransformByObjectBone(ThreadIndex, vPos, o, 50, &vRelative);
			CreateSprite(BITMAP_LIGHT, vPos, fSize + fLumi2, vLight, o);

			b->TransformByObjectBone(ThreadIndex, vPos, o, 54, &vRelative);
			CreateSprite(BITMAP_LIGHT, vPos, fSize + fLumi2, vLight, o);

			b->TransformByObjectBone(ThreadIndex, vPos, o, 59, &vRelative);
			CreateSprite(BITMAP_LIGHT, vPos, fSize + fLumi2, vLight, o);

			b->TransformByObjectBone(ThreadIndex, vPos, o, 60, &vRelative);
			CreateSprite(BITMAP_LIGHT, vPos, fSize + fLumi2, vLight, o);

			b->TransformByObjectBone(ThreadIndex, vPos, o, 53, &vRelative);
			CreateSprite(BITMAP_LIGHT, vPos, fSize + fLumi2, vLight, o);

			b->TransformByObjectBone(ThreadIndex, vPos, o, 55, &vRelative);
			CreateSprite(BITMAP_LIGHT, vPos, fSize + fLumi2, vLight, o);

			b->TransformByObjectBone(ThreadIndex, vPos, o, 56, &vRelative);
			CreateSprite(BITMAP_LIGHT, vPos, fSize + fLumi2, vLight, o);

			b->TransformByObjectBone(ThreadIndex, vPos, o, 57, &vRelative);
			CreateSprite(BITMAP_LIGHT, vPos, fSize + fLumi2, vLight, o);

			mu_float fLumi3 = (EMath::Cos(FWorldTime*0.004f) + 1.0f) * 0.1f;
			mu_float fLumi4 = (EMath::Cos(FWorldTime*0.004f) + 1.0f) * 0.2f;

			fSize = 1.6f;

			Vector(0.9f + fLumi3, 0.1f + fLumi3, 0.6f + fLumi3, vLight);
			Vector(0.0f, 0.0f, 0.0f, vRelative);

			b->TransformByObjectBone(ThreadIndex, vPos, o, 29, &vRelative);
			CreateSprite(BITMAP_LIGHT, vPos, fSize + fLumi4, vLight, o);

			b->TransformByObjectBone(ThreadIndex, vPos, o, 30, &vRelative);
			CreateSprite(BITMAP_LIGHT, vPos, fSize + fLumi4, vLight, o);

			b->TransformByObjectBone(ThreadIndex, vPos, o, 31, &vRelative);
			CreateSprite(BITMAP_LIGHT, vPos, (fSize * 0.6f) + fLumi4, vLight, o);

			b->TransformByObjectBone(ThreadIndex, vPos, o, 67, &vRelative);
			CreateSprite(BITMAP_LIGHT, vPos, (fSize * 0.6f) + fLumi4, vLight, o);

			b->TransformByObjectBone(ThreadIndex, vPos, o, 66, &vRelative);
			CreateSprite(BITMAP_LIGHT, vPos, fSize + fLumi4, vLight, o);

			b->TransformByObjectBone(ThreadIndex, vPos, o, 65, &vRelative);
			CreateSprite(BITMAP_LIGHT, vPos, fSize + fLumi4, vLight, o);

			b->TransformByObjectBone(ThreadIndex, vPos, o, 9, &vRelative);
			CreateSprite(BITMAP_LIGHT, vPos, fSize + fLumi4, vLight, o);

			b->TransformByObjectBone(ThreadIndex, vPos, o, 28, &vRelative);
			CreateSprite(BITMAP_LIGHT, vPos, fSize + fLumi4, vLight, o);

			b->TransformByObjectBone(ThreadIndex, vPos, o, 64, &vRelative);
			CreateSprite(BITMAP_LIGHT, vPos, fSize + fLumi4, vLight, o);

			fLumi3 = (EMath::Cos(FWorldTime*0.004f) + 1.0f) * 0.25f;
			fLumi4 = (EMath::Cos(FWorldTime*0.004f) + 1.0f) * 0.2f;

			fSize = 1.3f;

			Vector(0.9f + fLumi3, 0.1f + fLumi3, 0.4f + fLumi3, vLight);
			Vector(0.0f, 0.0f, 0.0f, vRelative);

			b->TransformByObjectBone(ThreadIndex, vPos, o, 7, &vRelative);
			CreateSprite(BITMAP_LIGHT, vPos, fSize + fLumi4, vLight, o);

			b->TransformByObjectBone(ThreadIndex, vPos, o, 11, &vRelative);
			CreateSprite(BITMAP_LIGHT, vPos, fSize + fLumi4, vLight, o);

			b->TransformByObjectBone(ThreadIndex, vPos, o, 33, &vRelative);
			CreateSprite(BITMAP_LIGHT, vPos, fSize + fLumi4, vLight, o);

			fSize = 1.3f;

			Vector(0.9f + fLumi3, 0.1f + fLumi3, 0.4f + fLumi3, vLight);
			Vector(0.0f, 0.0f, 0.0f, vRelative);

			b->TransformByObjectBone(ThreadIndex, vPos, o, 7, &vRelative);
			CreateSprite(BITMAP_LIGHT, vPos, fSize + fLumi4, vLight, o);

			b->TransformByObjectBone(ThreadIndex, vPos, o, 11, &vRelative);
			CreateSprite(BITMAP_LIGHT, vPos, fSize + fLumi4, vLight, o);

			b->TransformByObjectBone(ThreadIndex, vPos, o, 33, &vRelative);
			CreateSprite(BITMAP_LIGHT, vPos, fSize + fLumi4, vLight, o);

			fSize = 2.3f;

			Vector(0.9f + fLumi3, 0.1f + fLumi3, 0.4f + fLumi3, vLight);
			Vector(0.0f, 0.0f, 0.0f, vRelative);

			b->TransformByObjectBone(ThreadIndex, vPos, o, 5, &vRelative);
			CreateSprite(BITMAP_LIGHT, vPos, fSize + fLumi4, vLight, o);

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
	}

	return false;
}

mu_boolean GMEmpireGuardian4::SetCurrentActionMonster(CHARACTER* c, OBJECT* o)
{
	if (true == g_EmpireGuardian1.SetCurrentActionMonster(c, o))
	{
		return true;
	}

	switch (c->MonsterIndex)
	{
	case 504:
		{
			if (m_bCurrentIsRage_BossGaion == true)
			{
				SetAction(o, MONSTER01_APEAR);
				c->MonsterSkill = -1;
				return true;
			}

			switch (c->MonsterSkill)
			{
			case ATMON_SKILL_EMPIREGUARDIAN_GAION_01_GENERALATTACK:
				{
					SetAction(o, MONSTER01_ATTACK1);
					c->MonsterSkill = -1;
				}
				break;
			case ATMON_SKILL_EMPIREGUARDIAN_GAION_02_BLOODATTACK:
				{
					SetAction(o, MONSTER01_ATTACK2);
					c->MonsterSkill = -1;
				}
				break;
			case ATMON_SKILL_EMPIREGUARDIAN_GAION_03_GIGANTIKSTORM:
				{
					SetAction(o, MONSTER01_ATTACK3);
					c->MonsterSkill = -1;
				}
				break;
			case ATMON_SKILL_EMPIREGUARDIAN_GAION_04_FLAMEATTACK:
				{
					SetAction(o, MONSTER01_ATTACK4);
					c->MonsterSkill = -1;
				}
				break;
			case ATMON_SKILL_EMPIREGUARDIAN_BERSERKER:
				{
					SetAction(o, MONSTER01_APEAR);
					c->MonsterSkill = -1;

					m_bCurrentIsRage_BossGaion = true;
				}
				break;
			default:
				{
					SetAction(o, MONSTER01_ATTACK1);
					c->MonsterSkill = -1;
				}
				break;
			}

			return true;
		}
		return true;
	case 505:
		{
			if (m_bCurrentIsRage_Jerint == true)
			{
				SetAction(o, MONSTER01_APEAR);
				c->MonsterSkill = -1;
				return true;
			}

			switch (c->MonsterSkill)
			{
			case 55:
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

					m_bCurrentIsRage_Jerint = true;
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

mu_boolean GMEmpireGuardian4::PlayMonsterSound(OBJECT* o)
{
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
	case MODEL_MONSTER01 + 164:
		{
			switch (o->CurrentAction)
			{
			case MONSTER01_WALK:
				{
					PlayBackground(SOUND_EMPIREGUARDIAN_BOSS_GAION_MONSTER_MOVE);
				}
				return true;
			case MONSTER01_DIE:
				{
					PlayBackground(SOUND_EMPIREGUARDIAN_BOSS_GAION_MONSTER_DEATH);
				}
				return true;
			case MONSTER01_APEAR:
				{
					PlayBackground(SOUND_EMPIREGUARDIAN_BOSS_GAION_MONSTER_RAGE);
				}
				return true;
			}
		}
		return true;
	case MODEL_MONSTER01 + 165:
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
				return true;
			case MONSTER01_ATTACK1:
				{
					PlayBackground(SOUND_EMPIREGUARDIAN_JERINT_MONSTER_ATTACK01);
				}
				return true;
			case MONSTER01_ATTACK2:
				{
					PlayBackground(SOUND_BLOODATTACK);

				}
				return true;
			case MONSTER01_ATTACK3:
				{
					PlayBackground(SOUND_EMPIREGUARDIAN_JERINT_MONSTER_ATTACK03);
					PlayBackground(SOUND_SKILL_BLOWOFDESTRUCTION);
				}
				return true;
			case MONSTER01_DIE:
				{
					PlayBackground(SOUND_EMPIREGUARDIAN_JERINT_MONSTER_DEATH);
				}
				return true;
			case MONSTER01_APEAR:
				{
					PlayBackground(SOUND_EMPIREGUARDIAN_JERINT_MONSTER_RAGE);
				}
				return true;
			}
		}
		return true;
	}

	return false;
}