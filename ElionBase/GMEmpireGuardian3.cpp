#include "stdafx.h"
#include "GMEmpireGuardian3.h"

GMEmpireGuardian3 g_EmpireGuardian3;

GMEmpireGuardian3::GMEmpireGuardian3()
{

}

GMEmpireGuardian3::~GMEmpireGuardian3()
{

}

mu_boolean GMEmpireGuardian3::CreateObject(mu_uint32 objectIndex, OBJECT* o)
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

CHARACTER* GMEmpireGuardian3::CreateMonster(const mu_uint32 ThreadIndex, mu_int32 iType, mu_int32 PosX, mu_int32 PosY, mu_int32 Key)
{
	CHARACTER* pCharacter = nullptr;

 	switch (iType)
 	{
	case 510:
		{
			pCharacter = CreateCharacter(Key, MODEL_MONSTER01+170, PosX, PosY);
			pCharacter->Object.Scale = 1.35f;
			
			m_bCurrentIsRage_Kato = false;
		}
		break;
	case 516:
		{
			pCharacter = CreateCharacter(Key, MODEL_MONSTER01+176, PosX, PosY);
			pCharacter->Object.Scale = 1.55f;
		}
		break;
	case 517:
		{
			pCharacter = CreateCharacter(Key, MODEL_MONSTER01+177, PosX, PosY);
			pCharacter->Object.Scale = 1.25f;
		}
		break;
 	}

	return pCharacter;
}

mu_boolean GMEmpireGuardian3::MoveObject(const mu_uint32 ThreadIndex, OBJECT* o)
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
			o->Velocity = 0.44f;
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

mu_boolean GMEmpireGuardian3::MoveMonsterVisual(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b)
{
	if(true == g_EmpireGuardian1.MoveMonsterVisual(ThreadIndex, c, o, b))
	{
		return true;
	}

	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	EVector3 vPos;

	switch (o->Type)
	{
	case MODEL_MONSTER01 + 170:
		{
			switch (o->CurrentAction)
			{
			case MONSTER01_WALK:
			case MONSTER01_ATTACK1:
			case MONSTER01_ATTACK2:
			case MONSTER01_ATTACK3:
			case MONSTER01_DIE:
				break;
			case MONSTER01_APEAR:
				{
					EVector3		Light;

					if (m_bCurrentIsRage_Kato == true)
					{
						Vector(1.0f, 1.0f, 1.0f, Light);
						CreateInferno(ThreadIndex, o->Position);

						CreateEffect(ThreadIndex, MODEL_CIRCLE, o->Position, o->Angle, Light, 4, o);
						CreateEffect(ThreadIndex, MODEL_CIRCLE, o->Position, o->Angle, Light, 4, o);
						m_bCurrentIsRage_Kato = false;
					}
				}
				break;
			}
		}
		return true;
	case MODEL_MONSTER01 + 176:
		{
			switch (o->CurrentAction)
			{
			case MONSTER01_WALK:
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
						for (mu_int32 i = 0; i < 10; ++i)
						{
							b->Animation(ThreadIndex, ThreadInfo.BoneTransform, fAnimationFrame, o->PriorAnimationFrame, o->PriorAction, o->Angle, o->HeadAngle);

							Vector(0.0f, 0.0f, 0.0f, StartRelative);
							Vector(0.0f, 0.0f, 0.0f, EndRelative);

							b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(49)], StartRelative, StartPos, false);
							b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(51)], EndRelative, EndPos, false);
							CreateObjectBlur(o, StartPos, EndPos, Light, 3, false, o->m_iAnimation + 1);

							fAnimationFrame += fSpeedPerFrame;
						}
					}
				}
				break;
			case MONSTER01_ATTACK2:
				{
					if (c->AttackTime >= 1 && c->AttackTime <= 2)
					{
						EVector3 Angle;
						Vector(1.0f, 0.0f, 0.0f, Angle);
						CreateEffect(ThreadIndex, BITMAP_GATHERING, o->Position, o->Angle, o->Light, 1, o);
					}

					if (4.5f <= o->AnimationFrame && o->AnimationFrame < 4.6f)
					{
						EVector3  Light;
						Vector(1.0f, 0.5f, 0.2f, Light);

						EVector3 vPosBlur01, vPosBlurRelative01;
						EVector3 vPosBlur02, vPosBlurRelative02;

						mu_float fActionSpeed = b->_AnimationHeader[o->CurrentAction].Speed;
						mu_float fSpeedPerFrame = fActionSpeed / 6.0;
						mu_float fAnimationFrame = o->AnimationFrame - fActionSpeed;
						for (mu_int32 i = 0; i < 8; ++i)
						{
							b->Animation(ThreadIndex, ThreadInfo.BoneTransform, fAnimationFrame, o->PriorAnimationFrame, o->PriorAction, o->Angle, o->HeadAngle);

							Vector(0.0f, 0.0f, 0.0f, vPosBlurRelative01);
							Vector(0.0f, 0.0f, 0.0f, vPosBlurRelative02);

							b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(49)], vPosBlurRelative01, vPosBlur01, false);
							b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(51)], vPosBlurRelative02, vPosBlur02, false);

							CreateBlur(c, vPosBlur01, vPosBlur02, Light, 2);

							fAnimationFrame += fSpeedPerFrame;
						}
						Vector(0.1f, 0.6f, 0.3f, Light);
						CreateJoint(ThreadIndex, BITMAP_JOINT_FORCE, vPosBlur01, vPosBlur02, o->Angle, 10, o, 150.0f, o->PKKey, o->Skill, 1, -1, &Light);

						for (mu_int32 iLoop = 0; iLoop < 3; ++iLoop)
						{
							vPosBlur01[2] -= 30.0f;
							vPosBlur02[2] -= 30.0f;
							CreateJoint(ThreadIndex, BITMAP_JOINT_FORCE, vPosBlur01, vPosBlur02, o->Angle, 10, o, 150.0f, o->PKKey, o->Skill, 1, -1, &Light);
						}
					}

				}
				break;
			case MONSTER01_ATTACK3:
				{
					if (c->AttackTime >= 1 && c->AttackTime <= 2)
					{
						EVector3 Angle;
						Vector(1.0f, 0.0f, 0.0f, Angle);
						CreateEffect(ThreadIndex, BITMAP_GATHERING, o->Position, o->Angle, o->Light, 1, o);
					}

					if (4.5f <= o->AnimationFrame && o->AnimationFrame < 4.6f)
					{
						EVector3  Light;
						Vector(1.0f, 0.5f, 0.2f, Light);

						EVector3 vPosBlur01, vPosBlurRelative01;
						EVector3 vPosBlur02, vPosBlurRelative02;

						mu_float fActionSpeed = b->_AnimationHeader[o->CurrentAction].Speed;
						mu_float fSpeedPerFrame = fActionSpeed / 6.0;
						mu_float fAnimationFrame = o->AnimationFrame - fActionSpeed;
						for (mu_int32 i = 0; i < 8; ++i)
						{
							b->Animation(ThreadIndex, ThreadInfo.BoneTransform, fAnimationFrame, o->PriorAnimationFrame, o->PriorAction, o->Angle, o->HeadAngle);

							Vector(0.0f, 0.0f, 0.0f, vPosBlurRelative01);
							Vector(0.0f, 0.0f, 0.0f, vPosBlurRelative02);

							b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(49)], vPosBlurRelative01, vPosBlur01, false);
							b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(51)], vPosBlurRelative02, vPosBlur02, false);

							CreateBlur(c, vPosBlur01, vPosBlur02, Light, 2);

							fAnimationFrame += fSpeedPerFrame;
						}
						Vector(1.0f, 1.0f, 1.0f, Light);
						CreateJoint(ThreadIndex, BITMAP_JOINT_FORCE, vPosBlur01, vPosBlur02, o->Angle, 10, o, 150.0f, o->PKKey, o->Skill, 1, -1, &Light);

						for (mu_int32 iLoop = 0; iLoop < 3; ++iLoop)
						{
							vPosBlur01[2] -= 30.0f;
							vPosBlur02[2] -= 30.0f;
							CreateJoint(ThreadIndex, BITMAP_JOINT_FORCE, vPosBlur01, vPosBlur02, o->Angle, 10, o, 150.0f, o->PKKey, o->Skill, 1, -1, &Light);
						}
					}
				}
				break;
			case MONSTER01_DIE:
				break;
			case MONSTER01_APEAR:
				{
				}
				break;
			}
		}
		return true;
	case MODEL_MONSTER01 + 177:
		{
			switch (o->CurrentAction)
			{
			case MONSTER01_STOP1:
			case MONSTER01_STOP2:
				{
					if (4.5f <= o->AnimationFrame && o->AnimationFrame < 10.0f)
					{
						Vector(0.8f, 0.4f, 0.1f, o->Light);

						b->TransformByObjectBone(ThreadIndex, vPos, o, 15);
						CreateParticle(ThreadIndex, BITMAP_CLUD64, vPos, o->Angle, o->Light, 3, 0.2f);
						CreateParticle(ThreadIndex, BITMAP_WATERFALL_3, vPos, o->Angle, o->Light, 13, 1.0f);
					}
				}
				break;
			case MONSTER01_WALK:
				{
					Vector(0.5f, 0.2f, 0.1f, o->Light);

					if (7.0f <= o->AnimationFrame && o->AnimationFrame < 8.0f)
					{
						b->TransformByObjectBone(ThreadIndex, vPos, o, 32);
						vPos[2] += 25.0f;
						CreateParticle(ThreadIndex, BITMAP_ADV_SMOKE, vPos, o->Angle, o->Light, 3, 2.0f);
					}
					if (1.0f <= o->AnimationFrame && o->AnimationFrame < 2.0f)
					{
						b->TransformByObjectBone(ThreadIndex, vPos, o, 19);
						vPos[2] += 25.0f;
						CreateParticle(ThreadIndex, BITMAP_ADV_SMOKE, vPos, o->Angle, o->Light, 3, 2.0f);
					}
				}
				break;
			case MONSTER01_ATTACK1:
				{

				}
				break;
			case MONSTER01_ATTACK2:
				{
					if (o->AnimationFrame >= 3.8f && o->AnimationFrame <= 9.4f)
					{
						RenderSkillEarthQuake(ThreadIndex, c, o, b, 14);
					}
					else
					{
						o->WeaponLevel = 0;
					}
				}
				break;
			case MONSTER01_APEAR:
				{

				}
				break;
			case MONSTER01_ATTACK3:
				{

				}
				break;
			case MONSTER01_DIE:
				{
					mu_float Scale = 0.3f;
					b->TransformByObjectBone(ThreadIndex, vPos, o, 30);
					CreateParticle(ThreadIndex, BITMAP_SMOKE + 1, vPos, o->Angle, o->Light, 1, Scale);

					b->TransformByObjectBone(ThreadIndex, vPos, o, 17);
					CreateParticle(ThreadIndex, BITMAP_SMOKE + 1, vPos, o->Angle, o->Light, 1, Scale);

					b->TransformByObjectBone(ThreadIndex, vPos, o, 2);
					CreateParticle(ThreadIndex, BITMAP_SMOKE + 1, vPos, o->Angle, o->Light, 1, Scale);

					b->TransformByObjectBone(ThreadIndex, vPos, o, 6);
					CreateParticle(ThreadIndex, BITMAP_SMOKE + 1, vPos, o->Angle, o->Light, 1, Scale);

					b->TransformByObjectBone(ThreadIndex, vPos, o, 98);
					CreateParticle(ThreadIndex, BITMAP_SMOKE + 1, vPos, o->Angle, o->Light, 1, Scale);

					b->TransformByObjectBone(ThreadIndex, vPos, o, 91);
					CreateParticle(ThreadIndex, BITMAP_SMOKE + 1, vPos, o->Angle, o->Light, 1, Scale);

					b->TransformByObjectBone(ThreadIndex, vPos, o, 2);
					CreateParticle(ThreadIndex, BITMAP_SMOKE + 3, vPos, o->Angle, o->Light, 3, Scale);

					b->TransformByObjectBone(ThreadIndex, vPos, o, 3);
					CreateParticle(ThreadIndex, BITMAP_SMOKE + 3, vPos, o->Angle, o->Light, 3, Scale);
				}
				break;
			}
		}
		return true;
	}

	return false;
}

void GMEmpireGuardian3::MoveBlurEffect(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

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

	case MODEL_MONSTER01 + 170:
		{
			EVector3  Light;
			EVector3 StartPos, StartRelative;
			EVector3 EndPos, EndRelative;
			mu_float fActionSpeed, fSpeedPerFrame, fAnimationFrame;

			if (o->CurrentAction == MONSTER01_ATTACK1)
			{
				if (4.2f <= o->AnimationFrame && o->AnimationFrame < 6.6f)
				{
					Vector(0.9f, 0.5f, 0.4f, Light);

					fActionSpeed = b->_AnimationHeader[o->CurrentAction].Speed;
					fSpeedPerFrame = fActionSpeed / 5.0;
					fAnimationFrame = o->AnimationFrame - fActionSpeed;

					for (mu_int32 i = 0; i < 3; ++i)
					{
						b->Animation(ThreadIndex, ThreadInfo.BoneTransform, fAnimationFrame, o->PriorAnimationFrame, o->PriorAction, o->Angle, o->HeadAngle);

						Vector(0.0f, 0.0f, 0.0f, StartRelative);
						Vector(0.0f, 0.0f, 0.0f, EndRelative);

						b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(24)], StartRelative, StartPos, false);
						b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(25)], EndRelative, EndPos, false);
						CreateBlur(c, StartPos, EndPos, Light, 2, false, 1);

						fAnimationFrame += fSpeedPerFrame;
					}
				}
				else if (10.2f <= o->AnimationFrame && o->AnimationFrame < 12.6f)
				{
					Vector(0.9f, 0.5f, 0.4f, Light);

					fActionSpeed = b->_AnimationHeader[o->CurrentAction].Speed;
					fSpeedPerFrame = fActionSpeed / 5.0;
					fAnimationFrame = o->AnimationFrame - fActionSpeed;

					for (mu_int32 i = 0; i < 3; ++i)
					{
						b->Animation(ThreadIndex, ThreadInfo.BoneTransform, fAnimationFrame, o->PriorAnimationFrame, o->PriorAction, o->Angle, o->HeadAngle);

						Vector(0.0f, 0.0f, 0.0f, StartRelative);
						Vector(0.0f, 0.0f, 0.0f, EndRelative);

						b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(43)], StartRelative, StartPos, false);
						b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(44)], EndRelative, EndPos, false);
						CreateBlur(c, StartPos, EndPos, Light, 2, false, 0);

						fAnimationFrame += fSpeedPerFrame;
					}
				}
			}
			else if (o->CurrentAction == MONSTER01_ATTACK2)
			{
				if (6.2f <= o->AnimationFrame && o->AnimationFrame < 8.2f)
				{
					Vector(0.9f, 0.5f, 0.4f, Light);

					fActionSpeed = b->_AnimationHeader[o->CurrentAction].Speed;
					fSpeedPerFrame = fActionSpeed / 5.0;
					fAnimationFrame = o->AnimationFrame - fActionSpeed;
					for (mu_int32 i = 0; i < 3; ++i)
					{
						b->Animation(ThreadIndex, ThreadInfo.BoneTransform, fAnimationFrame, o->PriorAnimationFrame, o->PriorAction, o->Angle, o->HeadAngle);

						Vector(0.0f, 0.0f, 0.0f, StartRelative);
						Vector(0.0f, 0.0f, 0.0f, EndRelative);

						b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(43)], StartRelative, StartPos, false);
						b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(44)], EndRelative, EndPos, false);
						CreateBlur(c, StartPos, EndPos, Light, 2, false, 0);
						b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(24)], StartRelative, StartPos, false);
						b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(25)], EndRelative, EndPos, false);
						CreateBlur(c, StartPos, EndPos, Light, 2, false, 1);

						fAnimationFrame += fSpeedPerFrame;
					}
				}
			}
			else if (o->CurrentAction == MONSTER01_ATTACK3)
			{
				EVector3	vRelative, vRelative2, vPosition;
				Vector(0.0f, 0.0f, 0.0f, vRelative);
				Vector(0.0f, 0.0f, 0.0f, vRelative2);

				if (6.2f <= o->AnimationFrame && o->AnimationFrame < 8.2f)
				{
					Vector(0.9f, 0.5f, 0.4f, Light);

					fActionSpeed = b->_AnimationHeader[o->CurrentAction].Speed;
					fSpeedPerFrame = fActionSpeed / 10.0;
					fAnimationFrame = o->AnimationFrame - fActionSpeed;
					for (mu_int32 i = 0; i < 16; ++i)
					{
						b->Animation(ThreadIndex, ThreadInfo.BoneTransform, fAnimationFrame, o->PriorAnimationFrame, o->PriorAction, o->Angle, o->HeadAngle);

						Vector(0.0f, 0.0f, 0.0f, StartRelative);
						Vector(0.0f, 0.0f, 0.0f, EndRelative);

						b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(43)], StartRelative, StartPos, false);
						b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(44)], EndRelative, EndPos, false);
						CreateBlur(c, StartPos, EndPos, Light, 2, false, 10);
						b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(24)], StartRelative, StartPos, false);
						b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(25)], EndRelative, EndPos, false);
						CreateBlur(c, StartPos, EndPos, Light, 2, false, 21);

						fAnimationFrame += fSpeedPerFrame;

						if (7.4f <= o->AnimationFrame && o->AnimationFrame < 8.0f)
						{
							mu_int32 iOffset = 80;

							for (mu_int32 i_ = 0; i_ < 8; ++i_)
							{
								vRelative[0] = ((GetLargeRand(iOffset)) - iOffset * 0.5f);
								vRelative[1] = ((GetLargeRand(iOffset)) - iOffset * 0.5f);
								vRelative[2] = ((GetLargeRand(iOffset)) - iOffset * 0.5f);
								b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(24)], vRelative, vPosition, false);
								CreateParticle(ThreadIndex, BITMAP_FIRE, vPosition, o->Angle, o->Light, 0);

								vRelative[0] = ((GetLargeRand(iOffset)) - iOffset * 0.5f);
								vRelative[1] = ((GetLargeRand(iOffset)) - iOffset * 0.5f);
								vRelative[2] = ((GetLargeRand(iOffset)) - iOffset * 0.5f);
								b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(25)], vRelative, vPosition, false);
								CreateParticle(ThreadIndex, BITMAP_FIRE, vPosition, o->Angle, o->Light, 0);

								vRelative[0] = ((GetLargeRand(iOffset)) - iOffset * 0.5f);
								vRelative[1] = ((GetLargeRand(iOffset)) - iOffset * 0.5f);
								vRelative[2] = ((GetLargeRand(iOffset)) - iOffset * 0.5f);
								b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(43)], vRelative, vPosition, false);
								CreateParticle(ThreadIndex, BITMAP_FIRE, vPosition, o->Angle, o->Light, 0);

								vRelative[0] = ((GetLargeRand(iOffset)) - iOffset * 0.5f);
								vRelative[1] = ((GetLargeRand(iOffset)) - iOffset * 0.5f);
								vRelative[2] = ((GetLargeRand(iOffset)) - iOffset * 0.5f);
								b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(44)], vRelative, vPosition, false);
								CreateParticle(ThreadIndex, BITMAP_FIRE, vPosition, o->Angle, o->Light, 0);

								EVector3	vLight__;
								Vector(0.9f, 0.9f, 0.9f, vLight__);


								vRelative[0] = ((GetLargeRand(iOffset)) - iOffset * 0.5f);
								vRelative[1] = ((GetLargeRand(iOffset)) - iOffset * 0.5f);
								vRelative[2] = ((GetLargeRand(iOffset)) - iOffset * 0.5f);
								b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(24)], vRelative, vPosition, false);
								CreateParticle(ThreadIndex, BITMAP_SMOKELINE2, vPosition, o->Angle, vLight__, 3);

								vRelative[0] = ((GetLargeRand(iOffset)) - iOffset * 0.5f);
								vRelative[1] = ((GetLargeRand(iOffset)) - iOffset * 0.5f);
								vRelative[2] = ((GetLargeRand(iOffset)) - iOffset * 0.5f);
								b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(25)], vRelative, vPosition, false);
								CreateParticle(ThreadIndex, BITMAP_SMOKELINE2, vPosition, o->Angle, vLight__, 3);

								vRelative[0] = ((GetLargeRand(iOffset)) - iOffset * 0.5f);
								vRelative[1] = ((GetLargeRand(iOffset)) - iOffset * 0.5f);
								vRelative[2] = ((GetLargeRand(iOffset)) - iOffset * 0.5f);
								b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(43)], vRelative, vPosition, false);
								CreateParticle(ThreadIndex, BITMAP_SMOKELINE2, vPosition, o->Angle, vLight__, 3);

								vRelative[0] = ((GetLargeRand(iOffset)) - iOffset * 0.5f);
								vRelative[1] = ((GetLargeRand(iOffset)) - iOffset * 0.5f);
								vRelative[2] = ((GetLargeRand(iOffset)) - iOffset * 0.5f);
								b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(44)], vRelative, vPosition, false);
								CreateParticle(ThreadIndex, BITMAP_SMOKELINE2, vPosition, o->Angle, vLight__, 3);
							}
						}
					}
				}
			}
		}
		break;
	case MODEL_MONSTER01 + 177:
		{
			switch (o->CurrentAction)
			{
			case MONSTER01_ATTACK1:
				{
					if (4.2f <= o->AnimationFrame && o->AnimationFrame < 8.9f)
					{
						EVector3  Light;
						Vector(1.0f, 1.0f, 1.0f, Light);

						EVector3 StartPos, StartRelative;
						EVector3 EndPos, EndRelative;

						mu_float fActionSpeed = b->_AnimationHeader[o->CurrentAction].Speed;
						mu_float fSpeedPerFrame = fActionSpeed / 5.0;
						mu_float fAnimationFrame = o->AnimationFrame - fActionSpeed;
						for (mu_int32 i = 0; i < 5; ++i)
						{
							b->Animation(ThreadIndex, ThreadInfo.BoneTransform, fAnimationFrame, o->PriorAnimationFrame, o->PriorAction, o->Angle, o->HeadAngle);

							Vector(0.0f, 0.0f, 0.0f, StartRelative);
							Vector(0.0f, 0.0f, 0.0f, EndRelative);

							b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(35)], StartRelative, StartPos, false);
							b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(36)], EndRelative, EndPos, false);
							CreateBlur(c, EndPos, StartPos, Light, 6);

							fAnimationFrame += fSpeedPerFrame;
						}
					}
				}
				break;
			}
		}
		break;
	}
}

mu_boolean GMEmpireGuardian3::RenderObjectMesh(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b, mu_boolean ExtraMon)
{	
	return g_EmpireGuardian1.RenderObjectMesh(ThreadIndex, o, b, ExtraMon);
}

mu_boolean GMEmpireGuardian3::RenderObjectVisual(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b)
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

mu_boolean GMEmpireGuardian3::RenderMonsterVisual(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b)
{
	if (g_EmpireGuardian1.RenderMonsterVisual(ThreadIndex, c, o, b))
	{
		return true;
	}

	switch (o->Type)
	{
	case MODEL_MONSTER01 + 170:
		{
			EVector3 Light, Position;

			Vector(0.4f, 0.4f, 0.4f, Light);
			b->TransformByObjectBone(ThreadIndex, Position, o, 9);
			CreateSprite(BITMAP_LIGHT_RED, Position, 0.4f, Light, o);
			CreateSprite(BITMAP_LIGHT_RED, Position, 1.2f, Light, o);
			b->TransformByObjectBone(ThreadIndex, Position, o, 10);
			CreateSprite(BITMAP_LIGHT_RED, Position, 0.4f, Light, o);
			CreateSprite(BITMAP_LIGHT_RED, Position, 1.2f, Light, o);

			b->TransformByObjectBone(ThreadIndex, Position, o, 28);
			CreateSprite(BITMAP_LIGHT_RED, Position, 0.4f, Light, o);
			CreateSprite(BITMAP_LIGHT_RED, Position, 1.2f, Light, o);
			b->TransformByObjectBone(ThreadIndex, Position, o, 29);
			CreateSprite(BITMAP_LIGHT_RED, Position, 0.4f, Light, o);
			CreateSprite(BITMAP_LIGHT_RED, Position, 1.2f, Light, o);
			b->TransformByObjectBone(ThreadIndex, Position, o, 47);
			CreateSprite(BITMAP_LIGHT_RED, Position, 0.4f, Light, o);
			CreateSprite(BITMAP_LIGHT_RED, Position, 1.2f, Light, o);
			b->TransformByObjectBone(ThreadIndex, Position, o, 48);
			CreateSprite(BITMAP_LIGHT_RED, Position, 0.4f, Light, o);
			CreateSprite(BITMAP_LIGHT_RED, Position, 1.2f, Light, o);

			b->TransformByObjectBone(ThreadIndex, Position, o, 123);
			CreateSprite(BITMAP_LIGHT_RED, Position, 0.4f, Light, o);
			CreateSprite(BITMAP_LIGHT_RED, Position, 1.2f, Light, o);
			b->TransformByObjectBone(ThreadIndex, Position, o, 124);
			CreateSprite(BITMAP_LIGHT_RED, Position, 0.4f, Light, o);
			CreateSprite(BITMAP_LIGHT_RED, Position, 1.2f, Light, o);
			b->TransformByObjectBone(ThreadIndex, Position, o, 119);
			CreateSprite(BITMAP_LIGHT_RED, Position, 0.4f, Light, o);
			CreateSprite(BITMAP_LIGHT_RED, Position, 1.2f, Light, o);
			b->TransformByObjectBone(ThreadIndex, Position, o, 120);
			CreateSprite(BITMAP_LIGHT_RED, Position, 0.4f, Light, o);
			CreateSprite(BITMAP_LIGHT_RED, Position, 1.2f, Light, o);

			b->TransformByObjectBone(ThreadIndex, Position, o, 125);
			CreateSprite(BITMAP_LIGHT_RED, Position, 0.4f, Light, o);
			CreateSprite(BITMAP_LIGHT_RED, Position, 1.2f, Light, o);
			b->TransformByObjectBone(ThreadIndex, Position, o, 126);
			CreateSprite(BITMAP_LIGHT_RED, Position, 0.4f, Light, o);
			CreateSprite(BITMAP_LIGHT_RED, Position, 1.2f, Light, o);

			Vector(0.9f, 0.9f, 0.9f, Light);
			if (GetLargeRand(3) == 0)
			{
				b->TransformByObjectBone(ThreadIndex, Position, o, 54);
				CreateParticle(ThreadIndex, BITMAP_SMOKELINE2, Position, o->Angle, Light, 3, 0.2f);
				b->TransformByObjectBone(ThreadIndex, Position, o, 58);
				CreateParticle(ThreadIndex, BITMAP_SMOKELINE2, Position, o->Angle, Light, 3, 0.2f);
				b->TransformByObjectBone(ThreadIndex, Position, o, 64);
				CreateParticle(ThreadIndex, BITMAP_SMOKELINE2, Position, o->Angle, Light, 3, 0.2f);
				b->TransformByObjectBone(ThreadIndex, Position, o, 65);
				CreateParticle(ThreadIndex, BITMAP_SMOKELINE2, Position, o->Angle, Light, 3, 0.2f);
				b->TransformByObjectBone(ThreadIndex, Position, o, 69);
				CreateParticle(ThreadIndex, BITMAP_SMOKELINE2, Position, o->Angle, Light, 3, 0.2f);
				b->TransformByObjectBone(ThreadIndex, Position, o, 70);
				CreateParticle(ThreadIndex, BITMAP_SMOKELINE2, Position, o->Angle, Light, 3, 0.2f);
				b->TransformByObjectBone(ThreadIndex, Position, o, 77);
				CreateParticle(ThreadIndex, BITMAP_SMOKELINE2, Position, o->Angle, Light, 3, 0.2f);
				b->TransformByObjectBone(ThreadIndex, Position, o, 78);
				CreateParticle(ThreadIndex, BITMAP_SMOKELINE2, Position, o->Angle, Light, 3, 0.2f);
				b->TransformByObjectBone(ThreadIndex, Position, o, 83);
				CreateParticle(ThreadIndex, BITMAP_SMOKELINE2, Position, o->Angle, Light, 3, 0.2f);
				b->TransformByObjectBone(ThreadIndex, Position, o, 87);
				CreateParticle(ThreadIndex, BITMAP_SMOKELINE2, Position, o->Angle, Light, 3, 0.2f);
				b->TransformByObjectBone(ThreadIndex, Position, o, 91);
				CreateParticle(ThreadIndex, BITMAP_SMOKELINE2, Position, o->Angle, Light, 3, 0.2f);
				b->TransformByObjectBone(ThreadIndex, Position, o, 92);
				CreateParticle(ThreadIndex, BITMAP_SMOKELINE2, Position, o->Angle, Light, 3, 0.2f);
			}

			b->RenderMesh(ThreadIndex, 1, RENDER_CHROME | RENDER_BRIGHT, 0.5f, 1, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);

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
	case MODEL_MONSTER01 + 176:
		{
		}
		return true;
	}

	return false;
}

mu_boolean GMEmpireGuardian3::SetCurrentActionMonster(CHARACTER* c, OBJECT* o)
{
	if( true == g_EmpireGuardian1.SetCurrentActionMonster(c, o) )
	{
		return true;
	}
	
 	switch(c->MonsterIndex) 
 	{
	case 510:
		{
			if( m_bCurrentIsRage_Kato == true )
			{
				SetAction(o, MONSTER01_APEAR);
				c->MonsterSkill = -1;
				return true;
			}
			
			switch(c->MonsterSkill)
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
					
					m_bCurrentIsRage_Kato = true;
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
	case 517:
		{
			switch(c->MonsterSkill)
			{
			case 54:
				{
					SetAction(o, MONSTER01_ATTACK2);
					c->MonsterSkill = -1;
				}
				break;
			case 55:
				{
					SetAction(o, MONSTER01_APEAR);
					c->MonsterSkill = -1;
				}
				break;
			case 56:
				{
					SetAction(o, MONSTER01_ATTACK3);
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
			switch(c->MonsterSkill)
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
	case 516:
		{
			switch(c->MonsterSkill)
			{
			case 47:
				{
					SetAction(o, MONSTER01_ATTACK2);
					c->MonsterSkill = -1;
				}
				break;
			case 53:
				{
					SetAction(o, MONSTER01_ATTACK3);
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

mu_boolean GMEmpireGuardian3::PlayMonsterSound(OBJECT* o)
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
	case MODEL_MONSTER01 + 170:
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
					PlayBackground(SOUND_EMPIREGUARDIAN_COMBATMASTER_ATTACK01);
				}
				break;
			case MONSTER01_ATTACK2:
			case MONSTER01_ATTACK3:
				{
					PlayBackground(SOUND_EMPIREGUARDIAN_3CORP_CATO_ATTACK02);				
				}
				break;
			case MONSTER01_APEAR:
				{
					PlayBackground(SOUND_EMPIREGUARDIAN_RAYMOND_MONSTER_RAGE);
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

	case MODEL_MONSTER01 + 176:
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
					PlayBackground(SOUND_EMPIREGUARDIAN_COMBATMASTER_ATTACK01);
				}
				break;
			case MONSTER01_ATTACK2:
			case MONSTER01_ATTACK3:
				{
					PlayBackground(SOUND_EMPIREGUARDIAN_COMBATMASTER_ATTACK02);
				}
				break;
			case MONSTER01_DIE:
				{
					PlayBackground(SOUND_EMPIREGUARDIAN_ASSASSINMASTER_DEATH);
				}
				break;
			}
		}
		return true;

	case MODEL_MONSTER01 + 177:
		{
			switch (o->CurrentAction)
			{
			case MONSTER01_WALK:
				{
					if (7.0f <= o->AnimationFrame && o->AnimationFrame < 8.0f)
					{
						PlayBackground(SOUND_EMPIREGUARDIAN_CAVALRYLEADER_MOVE01);
					}
					if (1.0f <= o->AnimationFrame && o->AnimationFrame < 2.0f)
					{
						PlayBackground(SOUND_EMPIREGUARDIAN_CAVALRYLEADER_MOVE02);
					}
				}
				break;
			case MONSTER01_ATTACK1:
			case MONSTER01_ATTACK3:
				{
					PlayBackground(SOUND_EMPIREGUARDIAN_CAVALRYLEADER_ATTACK01);
				}
				break;
			case MONSTER01_ATTACK2:
			case MONSTER01_ATTACK4:
				{
					PlayBackground(SOUND_EMPIREGUARDIAN_CAVALRYLEADER_ATTACK02);
				}
				break;
			case MONSTER01_DIE:
				{
					PlayBackground(SOUND_RAKLION_GIANT_MAMUD_DEATH);
				}
				break;
			}
		}
		return true;
	}

	return false;
}