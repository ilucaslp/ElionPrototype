#include "stdafx.h"
#include "mu_tasking.h"

mu_uint8 g_byUpperBoneLocation[7] = { 25, 26, 27, 20, 34, 35, 36 };

void MoveParticle(OBJECT *o, mu_boolean Turn)
{
	if (Turn)
	{
		EBone Matrix;
		EVector3 Angle;
		EVector3 Position;
		VectorCopy(o->Angle, Angle);
		AngleMatrix(Angle, Matrix);
		VectorRotate(o->Direction, Matrix, Position);
		VectorAdd(o->Position, Position, o->Position);
	}
	else
	{
		VectorAdd(o->Position, o->Direction, o->Position);
	}
}

void MoveParticle(OBJECT *o, EVector3 angle)
{
	EBone Matrix;
	EVector3 Angle;
	EVector3 Position;
	VectorCopy(angle, Angle);
	AngleMatrix(Angle, Matrix);
	VectorRotate(o->Direction, Matrix, Position);
	VectorAdd(o->Position, Position, o->Position);
}

void CheckTargetRange(const mu_uint32 ThreadIndex, OBJECT *o)
{
	OBJECT *to = o->Owner;
	if (to != nullptr && to->Flags.Get(OBJECT::eOF_LIVE | OBJECT::eOF_VISIBLE) == true)
	{
		mu_float dx = o->Position[0] - to->Position[0];
		mu_float dy = o->Position[1] - to->Position[1];
		mu_float Range = EMath::Sqrt(dx*dx + dy*dy);
		if (Range <= 100.0f)
		{
			o->LifeTime = 1;
			EVector3 Light;
			Vector(1.0f, 1.0f, 1.0f, Light);
			switch (o->Type)
			{
			case MODEL_ARROW:
			case MODEL_ARROW_STEEL:
			case MODEL_ARROW_THUNDER:
			case MODEL_ARROW_LASER:
			case MODEL_ARROW_V:
			case MODEL_ARROW_SAW:
			case MODEL_ARROW_NATURE:
			case MODEL_ARROW_WING:
			case MODEL_ARROW_BOMB:
			case MODEL_LACEARROW:
			case MODEL_DARK_SCREAM:
			case MODEL_DARK_SCREAM_FIRE:
			case MODEL_ARROW_SPARK:
			case MODEL_ARROW_RING:
			case MODEL_ARROW_TANKER:
			case MODEL_ARROW_DARKSTINGER:
			case MODEL_ARROW_GAMBLE:
				break;
			case MODEL_FIRE:
				if (o->SubType == 1)
				{
					for (mu_uint32 j = 0; j < 2; ++j)
						CreateEffect(ThreadIndex, MODEL_STONE1 + GetLargeRand(2), o->Position, o->Angle, o->Light);
				}
				break;

			case BITMAP_ENERGY:
				CreateParticle(ThreadIndex, BITMAP_SPARK + 1, o->Position, o->Angle, Light, 1, 6.0f);
				break;

			case MODEL_LIGHTNING_ORB:
				{
					CreateEffect(ThreadIndex, MODEL_LIGHTNING_ORB, o->Position, o->Angle, o->Light, 1);
				}
				break;

			case MODEL_SNOW1:
				{
					for (mu_uint32 j = 0; j < 2; ++j)
					{
						CreateEffect(ThreadIndex, MODEL_SNOW2 + GetLargeRand(2), o->Position, o->Angle, o->Light);
						CreateParticle(ThreadIndex, BITMAP_SMOKE, o->Position, o->Angle, o->Light);
					}
					PlayBackground(SOUND_BREAK01);
				}
				break;
			case MODEL_WOOSISTONE:
				if (Range <= 30.0f)
				{
					for (mu_uint32 j = 0; j < 20; ++j)
					{
						CreateEffect(ThreadIndex, MODEL_WOOSISTONE, o->Position, o->Angle, o->Light, 1);
						CreateParticle(ThreadIndex, BITMAP_FIRE, o->Position, o->Angle, o->Light, 0, 1, o);
					}
					PlayBackground(SOUND_BREAK01);
				}
				else
				{
					o->LifeTime = 100;
				}
				break;
			}
		}
	}
}

mu_boolean MoveJump(OBJECT *o)
{
	o->Position[2] += o->Gravity;
	o->Gravity -= 1.0f;
	mu_float Height = RequestTerrainHeight(o->Position[0], o->Position[1]);
	if (o->Position[2] < Height)
	{
		o->Position[2] = Height;
		o->Gravity = -o->Gravity*0.4f;
		o->Direction[1] = o->Direction[1] * 0.4f;
		return true;
	}
	return false;
}

void MUTasking::MoveEffect(const mu_uint32 ThreadIndex, OBJECT *o, mu_uint32 iIndex)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	EVector3 Light;
	EVector3 Angle;
	mu_uint32 Index;
	EVector3 p, Position;
	EBone Matrix;
	mu_float Height;
	mu_float Luminosity = (mu_float)(GetLargeRand(4) + 7)*0.1f;
	if (o->LifeTime < 5)
	{
		Luminosity -= (5 - o->LifeTime)*0.2f;
		if (Luminosity < 0.0f) Luminosity = 0.0f;
	}
	Vector(1.0f, 1.0f, 1.0f, Light);

	switch (o->Type)
	{
		// Skills Re-factoring
	case MODEL_EVILSPIRIT:
		{
			if (o->LifeTime == 1)
			{
				EVector3 Position;
				VectorCopy(o->Position, Position);
				Position[2] += 100.0f;

				for (mu_int32 i = 0; i < 4; ++i)
				{
					EVector3 Angle;
					Vector(0.0f, 0.0f, i*90.0f, Angle);

					mu_int32 SkillIndex = FindHotKey(o->Kind);
					CreateJoint(ThreadIndex, BITMAP_JOINT_SPIRIT, Position, o->Position, Angle, 0, o->Owner, 80.0f, o->Owner->PKKey, SkillIndex, o->Owner->m_bySkillSerialNum);
					CreateJoint(ThreadIndex, BITMAP_JOINT_SPIRIT, Position, o->Position, Angle, 0, o->Owner, 20.0f);
				}

				PlayBackground(SOUND_EVIL);
			}
		}
		break;

	case MODEL_DRAGON:
		{
			EVector3 vTemp;
			VectorSubtract(o->Position, o->StartPosition, vTemp);
			mu_float fTemp = VectorLength(vTemp);

			if (o->Angle[2] >= 360.0f) o->Angle[2] = 0.0f;
			if (o->Angle[2] <= -360.0f) o->Angle[2] = 0.0f;
			if (o->Angle[1] >= 360.0f) o->Angle[1] = 0.0f;
			if (o->Angle[1] <= -360.0f) o->Angle[1] = 0.0f;

			switch (o->Kind)
			{
			case 0:
				o->Timer = 30.0f;
				break;
			case 1:
				o->Timer = -30.0f;
				break;
			default:
				o->Timer = 0.0f;
				break;
			}
			if (o->Timer > o->Angle[1]) o->Angle[1] += 0.5f;
			if (o->Timer < o->Angle[1]) o->Angle[1] -= 0.5f;

			if (fTemp <= 800.0f && fTemp >= -800.0f)
			{
				o->Kind = 2;
				o->Distance = 0.0f;
				o->Angle[2] += 0.5f * o->Distance * 0.5f;
				o->CollisionRange = true;
			}
			else
			{
				if (o->CollisionRange)
				{
					o->Timer = o->Angle[2];
					o->Gravity = o->Gravity * -1.0f;
					o->CollisionRange = false;
				}
				if (o->Timer >= o->Angle[2] - 60.0f)
				{
					o->Angle[2] += 0.5f * o->Gravity * 3.0f;
					o->Kind = 1;
				}
				else if (o->Timer <= o->Angle[2] + 60.0f)
				{
					o->Angle[2] += 0.5f * o->Gravity * 3.0f;
					o->Kind = 0;
				}
				else
				{
					o->CollisionRange = true;
					o->Kind = 2;
				}
			}

			if (o->LifeTime <= 10)
				o->LifeTime = 1000;
			if (o->AnimationFrame >= 40.0f)
			{
				o->AnimationFrame = 0.0f;
				o->PriorAnimationFrame = 0.0f;
				o->CurrentAction = 0;
			}
		}
		break;

	case MODEL_ARROW_AUTOLOAD:
		{
			if (o->SubType == 0)
			{
				OBJECT* pOwn = o->Owner;
				if (o->LifeTime % 10 == 0)
				{
					CreateEffect(ThreadIndex, MODEL_ARROW_AUTOLOAD, pOwn->Position, pOwn->Angle, pOwn->Light, 1, pOwn);
				}
			}
			else if (o->SubType == 1)
			{
				EVector3 tmp = { -10.0f, 5.0f, 10.0f };

				if (o->LifeTime < 20)
				{
					mu_float flumi = (mu_float)(o->LifeTime - 20) / 20.0f;
					VectorScale(o->Direction, flumi, o->Light);
				}

				OBJECT* pOwner = o->Owner;
				MUModel *b = &MODELS::Data[pOwner->Type];
#ifdef USE_DUAL_QUATERNION
				pOwner->BoneTransform[b->GetBoneIndex(47)].TransformOut(tmp, o->Position);
#else
				VectorTransform(tmp, pOwner->BoneTransform[b->GetBoneIndex(47)], o->Position);
#endif
				VectorScale(o->Position, pOwner->Scale, o->Position);
				VectorAdd(o->Position, pOwner->Position, o->Position);
				VectorCopy(pOwner->Angle, o->Angle);
				o->Angle[0] += 85.0f; o->Angle[1] += -17.0f; o->Angle[2] += 20.0f;
			}
		}
		break;

	case MODEL_INFINITY_ARROW:
		{
			EVector3 tmp = { 0.0f, 0.0f, 0.0f };
			OBJECT* pOwner = o->Owner;
			if (o->SubType == 1)
			{
				o->Light[0] *= 0.95f;
				o->Light[1] *= 0.95f;
				o->Light[2] *= 0.95f;
			}
			MUModel *b = &MODELS::Data[pOwner->Type];
#ifdef USE_DUAL_QUATERNION
			pOwner->BoneTransform[b->GetBoneIndex(29)].TransformOut(tmp, o->Position);
#else
			VectorTransform(tmp, pOwner->BoneTransform[b->GetBoneIndex(29)], o->Position);
#endif
			VectorScale(o->Position, pOwner->Scale, o->Position);
			VectorAdd(o->Position, pOwner->Position, o->Position);
			VectorCopy(pOwner->Angle, o->Angle);
		}
		break;

	case MODEL_INFINITY_ARROW1:
	case MODEL_INFINITY_ARROW2:
	case MODEL_INFINITY_ARROW3:
		{
			if (o->SubType == 0)
			{
				o->Light[0] *= 0.95f;
				o->Light[1] *= 0.95f;
				o->Light[2] *= 0.95f;
				VectorCopy(o->Owner->Position, o->Position);
			}
			else if (o->SubType == 1)
			{
				o->Light[0] *= 0.98f;
				o->Light[1] *= 0.98f;
				o->Light[2] *= 0.98f;
				if (o->LifeTime == 40)
					CreateEffect(ThreadIndex, MODEL_INFINITY_ARROW3, o->Position, o->Angle, o->Light, 2, o);
				else
					if (o->LifeTime == 20)
						CreateEffect(ThreadIndex, MODEL_INFINITY_ARROW3, o->Position, o->Angle, o->Light, 3, o);
				VectorCopy(o->Owner->Position, o->Position);
			}
			else if (o->SubType == 2)
			{
				o->Light[0] *= 0.98f;
				o->Light[1] *= 0.98f;
				o->Light[2] *= 0.98f;
				VectorCopy(o->Owner->Position, o->Position);
			}
			else if (o->SubType == 3)
			{
				o->Light[0] *= 0.98f;
				o->Light[1] *= 0.98f;
				o->Light[2] *= 0.98f;
				VectorCopy(o->Owner->Position, o->Position);
			}
		}
		break;
	case MODEL_INFINITY_ARROW4:
		{
			EVector3 tmp = { 0.0f, 0.0f, 0.0f };
			OBJECT* pOwner = o->Owner;
			o->Scale *= 1.2f;
			o->Light[0] *= 0.9f;
			o->Light[1] *= 0.9f;
			o->Light[2] *= 0.9f;

			MUModel *b = &MODELS::Data[pOwner->Type];
#ifdef USE_DUAL_QUATERNION
			pOwner->BoneTransform[b->GetBoneIndex(29)].TransformOut(tmp, o->Position);
#else
			VectorTransform(tmp, pOwner->BoneTransform[b->GetBoneIndex(29)], o->Position);
#endif
			VectorScale(o->Position, pOwner->Scale, o->Position);
			VectorAdd(o->Position, pOwner->Position, o->Position);
			VectorCopy(pOwner->Angle, o->Angle);
		}
		break;

	case MODEL_SHIELD_CRASH:
		{
			mu_float ftmp = 0.0f;
			EVector3 nPos, nLight;

			if (o->LifeTime >= 0 && o->LifeTime < 8)
			{
				ftmp = (mu_float)o->LifeTime / 8;
				VectorScale(o->Direction, ftmp, o->Light);
			}

			if (o->LifeTime >= 8 && o->LifeTime < 24)
			{
				ftmp = 1.0f - ((mu_float)(o->LifeTime - 24) / 16);
				VectorScale(o->Direction, ftmp, o->Light);
			}

			if (o->LifeTime == 23)
			{
				if (o->SubType == 1)
				{
					Vector(0.3f, 0.3f, 0.8f, nLight);
					CreateEffect(ThreadIndex, BITMAP_SHOCK_WAVE, o->Owner->Position, o->Owner->Angle,
						nLight, 9, o->Owner, -1, 0, 0, 0);
				}
				else if (o->SubType == 2)
				{
					EVector3 vShockColor = { 0.8f, 0.3f, 0.3f };
					CreateEffect(ThreadIndex, BITMAP_SHOCK_WAVE, o->Owner->Position, o->Owner->Angle,
						vShockColor, 9, o->Owner, -1, 0, 0, 0);
				}
				else
				{
					EVector3 vShockColor = { 1.0f, 1.0f, 1.0f };
					CreateEffect(ThreadIndex, BITMAP_SHOCK_WAVE, o->Owner->Position, o->Owner->Angle,
						vShockColor, 9, o->Owner, -1, 0, 0, 0);
				}
			}

			VectorCopy(o->Owner->Position, o->Position);
			VectorCopy(o->Owner->Position, nPos);

			if (o->SubType == 1)
			{
				Vector(0.0f, 1.0f, 5.0f, nLight);
				CreateEffect(ThreadIndex, MODEL_SKILL_INFERNO, nPos, o->Angle, nLight, 10, o, 30, 0);
				nPos[2] += 120.0f;
				CreateParticle(ThreadIndex, BITMAP_SMOKE, nPos, o->Angle, o->Light, 11, 2.0f);
			}
			else if (o->SubType == 2)
			{
				Vector(0.8f, 0.3f, 0.3f, nLight);
				CreateEffect(ThreadIndex, MODEL_SKILL_INFERNO, nPos, o->Angle, nLight, 10, o, 30, 0);
			}
			else if (o->SubType == 0)
			{
				Vector(0.0f, 0.8f, 1.5f, nLight);
				CreateEffect(ThreadIndex, MODEL_SKILL_INFERNO, nPos, o->Angle, nLight, 2, o, 30, 0);
				nPos[2] += 120.0f;
				CreateParticle(ThreadIndex, BITMAP_SMOKE, nPos, o->Angle, o->Light, 11, 2.0f);
			}
		}
		break;

	case MODEL_SHIELD_CRASH2:
		{
			mu_float ftmp = 0.0f;
			if (o->LifeTime >= 0 && o->LifeTime < 8)
			{
				ftmp = (mu_float)o->LifeTime / 8;
				VectorScale(o->Direction, ftmp, o->Light);
			}

			if (o->LifeTime >= 8 && o->LifeTime < 24)
			{
				ftmp = 1.0f - ((mu_float)(o->LifeTime - 24) / 16);
				VectorScale(o->Direction, ftmp, o->Light);
			}


			VectorCopy(o->Owner->Position, o->Position);
		}
		break;

	case MODEL_IRON_RIDER_ARROW:
		{
			o->Scale *= 1.05f;
			if (o->LifeTime < 5)
			{
				o->Light[0] *= 0.7f;
				o->Light[1] *= 0.7f;
				o->Light[2] *= 0.7f;
			}
			EVector3 vPos;
			VectorScale(o->Direction, o->Velocity, vPos);
			VectorAdd(o->Position, vPos, o->Position);
			CreateSprite(BITMAP_LIGHT + 3, o->Position, 4.0f, o->Light, o);
			CreateSprite(BITMAP_DS_EFFECT, o->Position, 2.5f, o->Light, o);
			CreateParticle(ThreadIndex, BITMAP_SPARK + 1, o->Position, o->Angle, o->Light, 10, 3.0f);

			VectorCopy(o->Position, o->EyeLeft);
			CreateEffect(ThreadIndex, MODEL_WAVES, o->Position, o->Angle, o->Light, 3, nullptr, 0);
		}
		break;
	case MODEL_MULTI_SHOT3:
		o->Scale += 0.25f;
		o->BlendMeshLight = (mu_float)o->LifeTime / 18.0f;
		o->Alpha = o->BlendMeshLight;
		break;
	case MODEL_MULTI_SHOT1:
		o->Scale += 0.2f;
		o->BlendMeshLight = (mu_float)o->LifeTime / 18.0f;
		o->Alpha = o->BlendMeshLight;
		break;
	case MODEL_MULTI_SHOT2:
		o->Scale += 0.3f;
		o->BlendMeshLight = (mu_float)o->LifeTime / 18.0f;
		o->Alpha = o->BlendMeshLight;
		break;
	case MODEL_BLADE_SKILL:
		{
			if (o->SubType == 0)
			{
				o->Scale -= 0.1f;
				if (o->Scale < 0.8f)
					o->Scale = 0.8f;
			}
			else if (o->SubType == 1)
			{
				if (o->Owner == nullptr ||
					o->Owner->BoneTransform == nullptr)
				{
					o->Flags.Clear(OBJECT::eOF_LIVE);
					break;
				}

				o->BlendMesh = -2;
				o->BlendMeshLight = (mu_float)o->LifeTime / 14.0f;
				o->Alpha = o->BlendMeshLight;

				EVector3 p, Angle, Light;

				OBJECT *to = nullptr;

				EVector3 tmpPos;

				MUModel *b = &MODELS::Data[MODEL_PLAYER];
				b->TransformByObjectBone(0, tmpPos, o->Owner, 47);

				if (o->LifeTime >= 3 && o->LifeTime % 2 == 0)
				{
					VectorCopy(o->Owner->Angle, Angle);
					mu_int32 Ran = -1;
					CHARACTER *c = &CharactersClient[o->PKKey];

					Ran = GetLargeRand(6);
					if (o->m_sTargetIndex > -1)
					{
						while (true)
						{
							Ran = GetLargeRand(6);
							if (Ran != o->m_sTargetIndex)
							{
								break;
							}
						}
					}
					o->m_sTargetIndex = Ran;

					switch (Ran)
					{
					case 0:
						o->Owner->Angle[2] = Angle[2] + 7.0f;
						break;
					case 1:
						o->Owner->Angle[2] = Angle[2] + 21.0f;
						break;
					case 2:
						o->Owner->Angle[2] = Angle[2] + 35.0f;
						break;
					case 3:
						o->Owner->Angle[2] = Angle[2] - 7.0f;
						break;
					case 4:
						o->Owner->Angle[2] = Angle[2] - 21.0f;
						break;
					case 5:
						o->Owner->Angle[2] = Angle[2] - 35.0f;
						break;
					}

					for (mu_int32 i = 0; i < 3; ++i)
					{
						VectorCopy(o->Position, p);
						Vector(0.2f, 0.3f, 1.0f, Light);

						p[1] += GetLargeRand(200) - 100;
						p[2] += GetLargeRand(200) - 100;
						CreateSprite(BITMAP_SHINY + 1, p, (mu_float)(GetLargeRand(8) + 8)*0.2f, Light, o->Owner, (mu_float)(GetLargeRand(360)));
						Vector(1.0f, 1.0f, 1.0f, Light);
						CreateSprite(BITMAP_SHINY + 1, p, (mu_float)(GetLargeRand(8) + 8)*0.07f, Light, o->Owner, (mu_float)(GetLargeRand(360)));
						VectorCopy(tmpPos, p);
						p[2] += ((GetLargeRand(80) - 40) + 30);
						CreateJoint(ThreadIndex, BITMAP_SPARK + 1, p, p, o->Owner->Angle, 1, o->Owner, 18.7f);
					}

					CreateArrow(ThreadIndex, c, o->Owner, to, FindHotKey((o->Skill)), 1, 0);
					o->Owner->Angle[2] = Angle[2];
				}
			}
		}
		break;
	case MODEL_KENTAUROS_ARROW:
		{
			if (o->LifeTime == 24)
			{
				EVector3 vDir;
				EMatrix4 vMat;
				Vector(0.0f, -1.0f, 0.0f, vDir);
				AngleMatrix(o->Angle, vMat);
				VectorRotate(vDir, vMat, o->Direction);
				CreateJoint(ThreadIndex, BITMAP_FLARE + 1, o->Position, o->Position, o->Angle, 17, o, 15.0f, 40);
			}

			if (o->LifeTime <= 24)
			{
				o->Scale *= 1.05f;
				EVector3 vLight = { 0.3f, 0.5f, 1.0f };
				EVector3 vPos;
				VectorScale(o->Direction, o->Velocity, vPos);
				VectorAdd(o->Position, vPos, o->Position);
				CreateParticle(ThreadIndex, BITMAP_SMOKE, o->Position, o->Angle, vLight, 26, 0.2f);
				CreateParticle(ThreadIndex, BITMAP_SMOKE, o->Position, o->Angle, vLight, 26, 0.2f);
			}

			o->Alpha += 0.001f;
		}
		break;
	case MODEL_WARP3:
	case MODEL_WARP2:
	case MODEL_WARP:
	case MODEL_WARP6:
	case MODEL_WARP5:
	case MODEL_WARP4:
		{
			if (o->SubType == 0)
			{
				mu_float fTemp1 = EMath::Sin(FWorldTime*0.0011f)*0.2f;
				mu_float fTemp2 = EMath::Sin(FWorldTime*0.0017f)*0.2f;
				mu_float fTemp3 = EMath::Sin(FWorldTime*0.0013f)*0.2f;
				Vector(fTemp1 + 0.01f, fTemp2 + 0.01f, fTemp3 + 0.01f, o->Light);
				o->Angle[1] += 4.0f + o->Gravity;
			}
			else if (o->SubType == 1)
			{
				mu_float fTemp1 = EMath::Sin(FWorldTime*0.0011f)*0.05f;
				Vector(0.0f + fTemp1, 0.2f + fTemp1, 0.1f + fTemp1, o->Light);
				o->Angle[1] += 2.0f + o->Gravity;
			}
		}
		break;
	case MODEL_GHOST:
		{
			if (GetLargeRand(40) == 1)
			{
				o->Velocity = o->Velocity * -1.0f;
				o->Gravity = o->Velocity * ((mu_float)(GetLargeRand(3)) + 5.0f);
			}
			if (GetLargeRand(40) == 1)
			{
				o->Velocity = o->Velocity * -1.0f;
				o->Distance = o->Velocity * ((mu_float)(GetLargeRand(3)) + 1.0f);
			}

			EVector3 vTemp;
			VectorSubtract(o->Position, o->StartPosition, vTemp);
			mu_float fTemp = VectorLength(vTemp);

			if (fTemp >= 500.0f && o->Kind == 0)
			{
				o->LifeTime = 50;
				o->Kind = 1;
			}

			o->Angle[2] += EMath::Sin(IWorldTime % 10000 * 0.0001f) * o->Gravity;
			o->Angle[0] += EMath::Sin(IWorldTime % 10000 * 0.0001f) * o->Distance / 3.0f;

			Vector(o->BlendMeshLight / 10.0f, o->BlendMeshLight / 10.0f, o->BlendMeshLight / 4.0f, o->Light);
			CreateParticle(ThreadIndex, BITMAP_LIGHT, o->Position, o->Angle, o->Light, 9, o->Scale);
		}
		break;

	case MODEL_TREE_ATTACK:
		{
			Vector(1.0f, 1.0f, 1.0f, Light);
			o->Scale += 0.1f;
			if (o->Scale >= 1.0f)
			{
				o->Alpha -= 0.07f;
				o->Scale = 1.0f;
			}
		}
		break;
	case MODEL_BUTTERFLY01:
		{
			if (o->Kind > 0)
				o->Angle[2] += GetLargeRand(10);
			else
				o->Angle[2] -= GetLargeRand(10);

			if (GetLargeRand(32) == 0)
			{
				o->Direction[2] = (mu_float)(GetLargeRand(15) - 7)*1.0f;
			}
			o->Direction[2] += (mu_float)(GetLargeRand(15) - 7)*0.2f;
			mu_float Height = RequestTerrainHeight(o->Position[0], o->Position[1]);
			if (o->Position[2] < Height + 50.0f)
			{
				o->Direction[2] *= 0.8f;
				o->Direction[2] += 1.0f;
			}
			if (o->Position[2] > Height + 150.0f)
			{
				o->Direction[2] *= 0.8f;
				o->Direction[2] -= 1.0f;
			}
			o->Position[2] += (mu_float)(GetLargeRand(15) - 7)*0.3f;

			mu_float  Luminosity = (mu_float)(GetLargeRand(32) + 64)*0.01f;
			if (o->SubType == 0)
			{
				Vector(Luminosity*0.4f, Luminosity*0.8f, Luminosity*0.6f, Light);
			}
			else if (o->SubType == 1)
			{
				Vector(Luminosity*0.4f, Luminosity*0.6f, Luminosity*0.8f, Light);
			}
			else if (o->SubType == 2)
			{
				Vector(Luminosity*0.6f, Luminosity*0.8f, Luminosity*0.4f, Light);
			}
			else if (o->SubType == 3)
			{
				Vector(Luminosity*0.7f, Luminosity*0.9f, Luminosity*0.5f, Light);
				if (GetLargeRand(2) == 0)
				{
					EVector3 Position;
					Vector((mu_float)(GetLargeRand(16) - 8), (mu_float)(GetLargeRand(16) - 8), (mu_float)(GetLargeRand(16) - 8), Position);
					VectorAdd(Position, o->Position, Position);
					CreateParticle(ThreadIndex, BITMAP_SPARK, Position, o->Angle, o->Light, 7);
				}
			}
			CreateSprite(BITMAP_LIGHT, o->Position, 1.0f, Light, o);
		}
		break;
	case 9:
		VectorScale(o->Direction, o->Velocity, Position);
		VectorAdd(Position, o->Position, o->Position);

		o->Velocity += 3.0f;
		o->Angle[0] += 5.0f*o->Velocity;

		if (o->Position[2]<350.0f && o->PKKey == 0)
		{
			o->PKKey = 1;
			mu_int32 PositionX = (mu_int32)(o->Position[0] * g_TerrainScaleRev);
			mu_int32 PositionY = (mu_int32)(o->Position[1] * g_TerrainScaleRev);
			AddWaterWave(PositionX, PositionY, 2, -1000);
		}
		break;

	case BITMAP_SKULL:
		if (o->SubType == 4)
		{
			o->Direction[1] -= o->Velocity;
			o->Angle[0] -= 1.0f;
			CreateSprite(BITMAP_SKULL, o->Position, 10.0f, o->Light, nullptr);
		}
		else
		{
			if (!o->Owner->Flags.Get(OBJECT::eOF_LIVE))
			{
				o->LifeTime = 0;
				o->Flags.Clear(OBJECT::eOF_LIVE);
			}
			else
			{
				switch (o->SubType)
				{
				case 0:
					if (g_isCharacterBuff(o->Owner, eDeBuff_Defense))
					{
						o->LifeTime = 10;

						if (g_isCharacterBuff(o->Owner, eBuff_Cloaking))
						{
							o->Flags.Clear(OBJECT::eOF_VISIBLE);
							break;
						}
					}
				case 1:
				case 3:
					{
						for (mu_int32 i = 0; i < 3; ++i)
						{
							mu_float fParam = (mu_float)i * Q_PI * 2 / 3.0f + o->LifeTime * 0.17f;
							if (o->SubType == 3)
							{
								fParam = -fParam;
							}
							EVector3 Position;
							mu_float fDist = 50.0f + 20.0f * (mu_float)EMath::Sin(i * 15.37f + FWorldTime*0.0031f);
							Position[0] = o->Owner->Position[0] + fDist * (mu_float)EMath::Sin(fParam);
							Position[1] = o->Owner->Position[1] + fDist * (mu_float)EMath::Cos(fParam);
							Position[2] = o->Owner->Position[2] + ((o->SubType == 3) ? 250.0f : 200.0f) * o->Owner->Scale;
							CreateSprite(BITMAP_SKULL, Position, 1.0f, Light, o->Owner, 0.0f);
						}
					}
					break;
				case 5:
					{
						for (mu_int32 i = 0; i < 3; ++i)
						{
							mu_float fParam = (mu_float)i * Q_PI * 2 / 3.0f + o->LifeTime * 0.17f;
							EVector3 Position;
							Position[0] = o->Owner->Position[0] + 50.0f * (mu_float)EMath::Sin(fParam + FWorldTime*0.003f);
							Position[1] = o->Owner->Position[1] + 50.0f * (mu_float)EMath::Cos(fParam + FWorldTime*0.003f);
							Position[2] = o->Owner->Position[2] + (i + 1)* 50.0f * o->Owner->Scale;
							EVector3 LightFlame = { 0.6f, 0.6f, 1.0f };
							CreateParticle(ThreadIndex, BITMAP_LIGHT, Position, o->Angle, LightFlame, 5, 0.7f);
						}
					}
					break;
				case 2:
					{
						EVector3 Light;
						Light[0] = Light[1] = Light[2] = 0.1f * (mu_float)o->LifeTime;
						CreateSprite(BITMAP_SKULL, o->Position, 1.5f, Light, o->Owner, 0.0f);
					}
					break;
				}
			}
		}
		break;
	case MODEL_MAGIC_CAPSULE2:
		VectorCopy(o->Owner->Position, o->Position);
		if (o->LifeTime < 10)
		{
			o->BlendMeshLight = (mu_float)o->LifeTime * 0.1f;
		}
		break;
	case MODEL_SPEAR:
		if (1 == o->SubType)
		{
			CreateJoint(ThreadIndex, BITMAP_FLARE, o->Position, o->Position, o->Angle, 12, o, 100.0f);
		}
		else if (0 == o->SubType)
		{
			CreateJoint(ThreadIndex, BITMAP_FLARE, o->Position, o->Position, o->Angle, 4, o, 50.0f);
		}
		break;
	case MODEL_SPEAR + 1:
		{
			EVector3 Angle, p, Position;
			Vector(0.0f, 0.0f, 0.0f, Position);
			EBone Matrix;
			Vector(0.0f, -100.0f, 0.0f, p);
			for (mu_int32 j = 0; j<3; ++j)
			{
				Vector((mu_float)(GetLargeRand(90)), 0.0f, (mu_float)(GetLargeRand(360)), Angle);
				AngleMatrix(Angle, Matrix);
				VectorRotate(p, Matrix, Position);
				VectorSubtract(o->Position, Position, Position);

				CreateJoint(ThreadIndex, BITMAP_JOINT_HEALING, Position, o->Position, Angle, 6, nullptr, 5.0f);
			}
		}
		break;

	case MODEL_HALLOWEEN_CANDY_BLUE:
	case MODEL_HALLOWEEN_CANDY_ORANGE:
	case MODEL_HALLOWEEN_CANDY_YELLOW:
	case MODEL_HALLOWEEN_CANDY_RED:
	case MODEL_HALLOWEEN_CANDY_HOBAK:
	case MODEL_HALLOWEEN_CANDY_STAR:
		if (o->SubType == 0 || o->SubType == 1)
		{
			o->Angle[o->m_iAnimation] += 20.0f;
			o->Position[2] += (o->Gravity * 0.5f);
			o->Gravity -= 1.5f;

			Height = RequestTerrainHeight(o->Position[0], o->Position[1]);
			if (o->Position[2] < Height)
			{
				o->Position[2] = Height;
				o->Gravity = -o->Gravity*0.3f;
				o->LifeTime -= 2;
			}

			VectorAdd(o->Position, o->Direction, o->Position);

			if (o->Type == MODEL_HALLOWEEN_CANDY_HOBAK)
			{
				if (GetLargeRand(3) == 0)
				{
					CreateParticle(ThreadIndex, BITMAP_FIRE, o->Position, o->Angle, o->Light, 5, 0.4f + GetLargeRand(10) * 0.01f);
				}
			}
		}
		break;

	case MODEL_HALLOWEEN_EX:
		break;

	case MODEL_XMAS_EVENT_BOX:
	case MODEL_XMAS_EVENT_CANDY:
	case MODEL_XMAS_EVENT_TREE:
	case MODEL_XMAS_EVENT_SOCKS:
		o->Angle[o->m_iAnimation] += 20.0f;
		o->Position[2] += (o->Gravity * 0.5f);
		o->Gravity -= 1.5f;

		Height = RequestTerrainHeight(o->Position[0], o->Position[1]);
		if (o->Position[2] < Height + 3.0f)
		{
			o->Position[2] = Height + 3.0f;
			o->Gravity = -o->Gravity*0.3f;
			o->LifeTime -= 2;
		}

		VectorAdd(o->Position, o->Direction, o->Position);
		break;

	case MODEL_XMAS_EVENT_ICEHEART:
		o->Angle[2] += 10.0f;
		if (o->Owner != nullptr)
		{
			if (o->Owner->CurrentAction != PLAYER_SANTA_2)
			{
				o->Flags.Clear(OBJECT::eOF_LIVE);
			}
		}
		break;

	case MODEL_NEWYEARSDAY_EVENT_BEKSULKI:
	case MODEL_NEWYEARSDAY_EVENT_CANDY:
	case MODEL_NEWYEARSDAY_EVENT_MONEY:
	case MODEL_NEWYEARSDAY_EVENT_HOTPEPPER_GREEN:
	case MODEL_NEWYEARSDAY_EVENT_HOTPEPPER_RED:
	case MODEL_NEWYEARSDAY_EVENT_PIG:
	case MODEL_NEWYEARSDAY_EVENT_YUT:
		o->Angle[o->m_iAnimation] += 10.0f;
		o->Position[0] += (o->Direction[0] * 1.2f);
		o->Position[1] += (o->Direction[1] * 1.2f);
		o->Position[2] += (o->Gravity * 1.5f);
		o->Gravity -= 1.5f;

		Height = RequestTerrainHeight(o->Position[0], o->Position[1]);
		if (o->Position[2] < Height)
		{
			o->Position[2] = Height;
			o->Gravity = -o->Gravity*0.3f;
			o->LifeTime -= 2;
		}

		VectorAdd(o->Position, o->Direction, o->Position);
		break;

	case MODEL_MOONHARVEST_MOON:
		if (o->SubType == 0)
		{
			o->Angle[2] += 5.0f;
			if (o->LifeTime < 25)
			{
				o->Light[0] -= 0.05f;
				o->Light[1] -= 0.06f;
				o->Light[2] -= 0.05f;
			}
		}
		else if (o->SubType == 1)
		{
			VectorAdd(o->Position, o->Direction, o->Position);
			CreateParticle(ThreadIndex, BITMAP_SMOKELINE1 + GetLargeRand(3), o->Position, o->Angle, o->Light, 0, 1.0f);
			CreateParticle(ThreadIndex, BITMAP_SMOKE, o->Position, o->Angle, o->Light, 11, 1.0f);
			if (GetLargeRand(2) == 0)
			{
				CreateParticle(ThreadIndex, BITMAP_WATERFALL_3, o->Position, o->Angle, o->Light, 3, 3.0f);
			}
		}
		else if (o->SubType == 2)
		{
			o->Scale += 0.01f;
		}

		break;

	case MODEL_MOONHARVEST_GAM:
	case MODEL_MOONHARVEST_SONGPUEN1:
	case MODEL_MOONHARVEST_SONGPUEN2:
		o->Angle[0] += 10.0f;
		o->Position[0] += (o->Direction[0] * 2.2f);
		o->Position[1] += (o->Direction[1] * 2.2f);
		o->Position[2] += (o->Gravity * 1.5f);
		o->Gravity -= 1.5f;

		Height = RequestTerrainHeight(o->Position[0], o->Position[1]);
		if (o->Position[2] < Height)
		{
			o->Position[2] = Height;
			o->Gravity = -o->Gravity*0.3f;
			o->LifeTime -= 2;
		}
		break;

	case MODEL_SPEARSKILL:
		break;
	case BITMAP_FIRE_CURSEDLICH:
		if (o->SubType == 0)
		{
			OBJECT * pObject = o->Owner;
			if (pObject->Flags.Get(OBJECT::eOF_LIVE) == false)
			{
				o->Flags.Clear(OBJECT::eOF_LIVE);
				break;
			}
			MUModel * pModel = &MODELS::Data[pObject->Type];
			EVector3 vPos, vRelative;
			Vector(0.0f, 0.0f, 0.0f, vRelative);
			pModel->TransformPosition(ThreadIndex, pObject->BoneTransform[pModel->GetBoneIndex(o->Skill)], vRelative, vPos, false);
			VectorScale(vPos, pModel->BodyScale(ThreadIndex), vPos);
			VectorAdd(vPos, pObject->Position, o->Position);

			EVector3 vLight;
			Vector(o->Alpha * 0.3f, o->Alpha * 0.3f, o->Alpha * 0.3f, vLight);
			CreateParticle(ThreadIndex, BITMAP_FIRE_CURSEDLICH, o->Position, o->Angle, vLight, 3, 1, pObject);
		}
		else if (o->SubType == 1)
		{
			EVector3 vFirePosition;
			for (mu_int32 i = 0; i < 1; ++i)
			{
				Vector(o->Position[0] + (GetLargeRand(100) - 50)*1.0f,
					o->Position[1] + (GetLargeRand(100) - 50)*1.0f, o->Position[2] + (GetLargeRand(10) + 5)*1.0f, vFirePosition);
				mu_float fScale = (GetLargeRand(5) + 13)*0.1f;
				switch (GetLargeRand(3))
				{
				case 0:
					CreateParticle(ThreadIndex, BITMAP_FIRE_HIK1, vFirePosition, o->Angle, o->Light, 1, fScale);
					break;
				case 1:
					CreateParticle(ThreadIndex, BITMAP_FIRE_CURSEDLICH, vFirePosition, o->Angle, o->Light, 5, fScale);
					break;
				case 2:
					CreateParticle(ThreadIndex, BITMAP_FIRE_HIK3, vFirePosition, o->Angle, o->Light, 1, fScale);
					break;
				}
			}
		}
		else if (o->SubType == 12)
		{
			EVector3 vFirePosition;
			for (mu_int32 i = 0; i < 1; ++i)
			{
				Vector(o->Position[0] + (GetLargeRand(50) - 25)*1.0f,
					o->Position[1] + (GetLargeRand(50) - 25)*1.0f, o->Position[2] + (GetLargeRand(10) + 5)*1.0f, vFirePosition);
				mu_float fScale = ((GetLargeRand(5) + 13)*0.05f) * o->Scale;

				switch (GetLargeRand(3))
				{
				case 0:
					CreateParticle(ThreadIndex, BITMAP_FIRE_HIK1, vFirePosition, o->Angle, o->Light, 1, fScale);
					break;
				case 1:
					CreateParticle(ThreadIndex, BITMAP_FIRE_CURSEDLICH, vFirePosition, o->Angle, o->Light, 5, fScale);
					break;
				case 2:
					CreateParticle(ThreadIndex, BITMAP_FIRE_HIK3, vFirePosition, o->Angle, o->Light, 1, fScale);
					break;
				}
			}
		}
		else if (o->SubType == 2)
		{
			if (o->Owner == nullptr)
			{
				o->Flags.Clear(OBJECT::eOF_LIVE);
				break;
			}

			EVector3 vFirePosition;
			for (mu_int32 i = 0; i < 8; ++i)
			{
				mu_int32 iNumBones = MODELS::Data[o->Owner->Type]._BoneNonDummyEnd;
				MODELS::Data[o->Owner->Type].TransformByObjectBoneDirect(ThreadIndex, vFirePosition, o->Owner, GetLargeRand(iNumBones));

				EVector3 vLightFire;
				Vector(1.0f, 0.2f, 0.0f, vLightFire);
				CreateSprite(BITMAP_LIGHT, vFirePosition, 4.0f, vLightFire, o->Owner);

				EVector3 vLight;
				Vector(1.0f, 1.0f, 1.0f, vLight);
				mu_float fScale = (GetLargeRand(5) + 13)*0.1f;
				switch (GetLargeRand(3))
				{
				case 0:
					CreateParticle(ThreadIndex, BITMAP_FIRE_HIK1, vFirePosition, o->Angle, vLight, 0, fScale);
					break;
				case 1:
					CreateParticle(ThreadIndex, BITMAP_FIRE_CURSEDLICH, vFirePosition, o->Angle, vLight, 4, fScale);
					break;
				case 2:
					CreateParticle(ThreadIndex, BITMAP_FIRE_HIK3, vFirePosition, o->Angle, vLight, 0, fScale);
					break;
				}
			}
		}
		else if (o->SubType == 3)
		{
			for (mu_int32 i = 0; i < 2; ++i)
			{
				mu_float fScale = (GetLargeRand(5) + 18)*0.03f;

				switch (GetLargeRand(3))
				{
				case 0:
					CreateParticle(ThreadIndex, BITMAP_FIRE_HIK1, o->Position, o->Angle, o->Light, 0, fScale);
					break;
				case 1:
					CreateParticle(ThreadIndex, BITMAP_FIRE_CURSEDLICH, o->Position, o->Angle, o->Light, 4, fScale);
					break;
				case 2:
					CreateParticle(ThreadIndex, BITMAP_FIRE_HIK3, o->Position, o->Angle, o->Light, 0, fScale);
					break;
				}
			}
		}
		else if (o->SubType == 4)
		{
			for (mu_int32 i = 0; i < 2; ++i)
			{
				mu_float fScale = (GetLargeRand(5) + 18)*0.03f;
				Vector(0.6f, 0.9f, 0.1f, o->Light);
				switch (GetLargeRand(3))
				{
				case 0:
					CreateParticle(ThreadIndex, BITMAP_FIRE_HIK1_MONO, o->Position, o->Angle, o->Light, 0, fScale);
					break;
				case 1:
					CreateParticle(ThreadIndex, BITMAP_FIRE_HIK2_MONO, o->Position, o->Angle, o->Light, 4, fScale);
					break;
				case 2:
					CreateParticle(ThreadIndex, BITMAP_FIRE_HIK3_MONO, o->Position, o->Angle, o->Light, 0, fScale);
					break;
				}
			}
		}
		break;
	case MODEL_SUMMONER_WRISTRING_EFFECT:
		{
			OBJECT * pObject = o->Owner;
			MUModel * pModel = &MODELS::Data[pObject->Type];

			EVector3 vPos, vRelative;
			Vector(0.0f, 0.0f, 0.0f, vRelative);
			pModel->TransformPosition(ThreadIndex, pObject->BoneTransform[pModel->GetBoneIndex(37)], vRelative, vPos, false);
			VectorScale(vPos, pModel->BodyScale(ThreadIndex), vPos);
			VectorAdd(vPos, pObject->Position, o->Position);

			if (pObject->Flags.Get(OBJECT::eOF_LIVE))
				o->LifeTime = 100.0f;

			MUModel * b = &MODELS::Data[o->Type];
			b->PlayAnimation(ThreadIndex, &o->AnimationFrame, &o->PriorAnimationFrame, &o->PriorAction, o->Velocity / 5.0f);
		}
		break;
	case MODEL_SUMMONER_EQUIP_HEAD_SAHAMUTT:
		if (o->SubType == 0)
		{
			OBJECT * pObject = o->Owner;
			o->Position[0] = pObject->Position[0] + EMath::Cos(FWorldTime*0.003f + o->Skill*0.024f) * 60.0f;
			o->Position[1] = pObject->Position[1] + EMath::Sin(FWorldTime*0.003f + o->Skill*0.024f) * 60.0f;
			o->Position[2] = pObject->Position[2] + (EMath::Sin(FWorldTime*0.0010f + o->Skill*0.024f) + 2.0f) * 80.0f - 60.0f;

			if (o->StartPosition[0] != o->Position[0] - pObject->Position[0])
			{
				mu_float fAngle = CreateAngle(o->StartPosition[0], o->StartPosition[1],
					o->Position[0] - pObject->Position[0], o->Position[1] - pObject->Position[1]);
				o->Angle[2] = fAngle + 0;
			}
			VectorSubtract(o->Position, pObject->Position, o->StartPosition);

			if (o->Kind == 1)
			{
				if (o->Alpha > 0.0f) o->Alpha -= 0.03f;
				else DeleteEffect(MODEL_SUMMONER_EQUIP_HEAD_SAHAMUTT, o->Owner);
			}
			else
			{
				if (Hero->SafeZone == true || EMath::Sin(FWorldTime*0.0004f + o->Skill*0.024f) < 0.3f)
					o->Kind = 1;
				if (o->Alpha < 1.0f) o->Alpha += 0.03f;
			}

			if (pObject->Flags.Get(OBJECT::eOF_LIVE))
				o->LifeTime = 100.0f;

			EVector3 vLight;
			Vector(o->Alpha * 0.3f, o->Alpha * 0.3f, o->Alpha * 0.3f, vLight);
			CreateParticle(ThreadIndex, BITMAP_FIRE_CURSEDLICH, o->Position, o->Angle, vLight, 1, 1, pObject);
		}
		else if (o->SubType == 1)
		{
			if (GetLargeRand(2) == 0)
			{
				OBJECT * pObject = o->Owner;
				if (pObject->Flags.Get(OBJECT::eOF_LIVE) == false)
				{
					o->Flags.Clear(OBJECT::eOF_LIVE);
					break;
				}
				MUModel * pModel = &MODELS::Data[pObject->Type];
				const mu_int32 iNumBones = pModel->_BoneNonDummyEnd;
				CreateEffect(ThreadIndex, BITMAP_FIRE_CURSEDLICH, pObject->Position, pObject->Angle, pObject->Light, 0, pObject, -1, pModel->_ReverseBonesIndex[GetLargeRand(iNumBones)]);
			}
		}
		break;
	case MODEL_SUMMONER_EQUIP_HEAD_NEIL:
		if (o->SubType == 0)
		{
			OBJECT * pObject = o->Owner;
			o->Position[0] = pObject->Position[0] + EMath::Cos(FWorldTime*0.003f + o->Skill*0.024f) * 60.0f;
			o->Position[1] = pObject->Position[1] + EMath::Sin(FWorldTime*0.003f + o->Skill*0.024f) * 60.0f;
			o->Position[2] = pObject->Position[2] + (EMath::Sin(FWorldTime*0.0010f + o->Skill*0.024f) + 2.0f) * 80.0f - 60.0f;

			if (o->StartPosition[0] != o->Position[0] - pObject->Position[0])
			{
				mu_float fAngle = CreateAngle(o->StartPosition[0], o->StartPosition[1],
					o->Position[0] - pObject->Position[0], o->Position[1] - pObject->Position[1]);
				o->Angle[2] = fAngle + 0;
			}
			VectorSubtract(o->Position, pObject->Position, o->StartPosition);

			if (o->Kind == 1)
			{
				if (o->Alpha > 0.0f) o->Alpha -= 0.03f;
				else DeleteEffect(MODEL_SUMMONER_EQUIP_HEAD_NEIL, o->Owner);
			}
			else
			{
				if (Hero->SafeZone == true || EMath::Sin(FWorldTime*0.0004f + o->Skill*0.024f) < 0.3f)
					o->Kind = 1;
				if (o->Alpha < 1.0f) o->Alpha += 0.03f;
			}

			if (pObject->Flags.Get(OBJECT::eOF_LIVE))
				o->LifeTime = 100.0f;

			EVector3 vLight;
			Vector(o->Alpha, o->Alpha, o->Alpha, vLight);
			CreateParticle(ThreadIndex, BITMAP_LIGHT + 2, o->Position, o->Angle, o->Light, 3, 0.30f, pObject);
			if (GetLargeRand(2) == 0)
				CreateParticle(ThreadIndex, BITMAP_LIGHT + 2, o->Position, o->Angle, o->Light, 3, 0.30f, pObject);
		}
		else if (o->SubType == 1)
		{
			OBJECT * pObject = o->Owner;
			if (pObject->Flags.Get(OBJECT::eOF_LIVE) == false)
			{
				o->Flags.Clear(OBJECT::eOF_LIVE);
				break;
			}
			EVector3 vColor;
			Vector(1.0f, 1.0f, 1.0f, vColor);
			CreateParticle(ThreadIndex, BITMAP_LIGHT + 2, pObject->Position, pObject->Angle, vColor, 5, 0.1f, pObject);
		}
		break;
	case MODEL_SUMMONER_EQUIP_HEAD_LAGUL:
		{
			OBJECT* pObject = o->Owner;
			o->Position[0] = pObject->Position[0] + EMath::Cos(FWorldTime*0.003f + o->Skill*0.024f) * 60.0f;
			o->Position[1] = pObject->Position[1] + EMath::Sin(FWorldTime*0.003f + o->Skill*0.024f) * 60.0f;
			o->Position[2] = pObject->Position[2] + (EMath::Sin(FWorldTime*0.0010f + o->Skill*0.024f) + 2.0f) * 80.0f - 60.0f;

			if (o->StartPosition[0] != o->Position[0] - pObject->Position[0])
			{
				mu_float fAngle = CreateAngle(o->StartPosition[0], o->StartPosition[1],
					o->Position[0] - pObject->Position[0], o->Position[1] - pObject->Position[1]);
				o->Angle[2] = fAngle + 0;
			}
			VectorSubtract(o->Position, pObject->Position, o->StartPosition);

			if (o->Kind == 1)
			{
				if (o->Alpha > 0.0f) o->Alpha -= 0.03f;
				else DeleteEffect(MODEL_SUMMONER_EQUIP_HEAD_LAGUL, o->Owner);
			}
			else
			{
				if (Hero->SafeZone || EMath::Sin(FWorldTime*0.0004f + o->Skill*0.024f) < 0.3f)
					o->Kind = 1;
				if (o->Alpha < 1.0f) o->Alpha += 0.03f;
			}

			if (pObject->Flags.Get(OBJECT::eOF_LIVE))
				o->LifeTime = 100.0f;

			EVector3 vLight;
			Vector(o->Alpha * 0.7f, o->Alpha * 0.3f, o->Alpha * 1.0f, vLight);
			CreateParticle(ThreadIndex, BITMAP_CLUD64, o->Position, o->Angle, vLight, 10, 1.0f, pObject);
		}
	case MODEL_SUMMONER_CASTING_EFFECT1:
	case MODEL_SUMMONER_CASTING_EFFECT11:
	case MODEL_SUMMONER_CASTING_EFFECT111:
	case MODEL_SUMMONER_CASTING_EFFECT2:
	case MODEL_SUMMONER_CASTING_EFFECT22:
	case MODEL_SUMMONER_CASTING_EFFECT222:
	case MODEL_SUMMONER_CASTING_EFFECT4:
		{
			if (o->LifeTime < 20) o->BlendMeshLight -= 0.03f;
			else if (o->BlendMeshLight < 0.5f) o->BlendMeshLight += 0.05f;

			switch (o->Type)
			{
			case MODEL_SUMMONER_CASTING_EFFECT1:
				o->Angle[2] -= 3.0f;
				break;
			case MODEL_SUMMONER_CASTING_EFFECT11:
				o->Angle[2] += 3.0f;
				break;
			case MODEL_SUMMONER_CASTING_EFFECT111:
				o->Angle[2] -= 3.0f;
				break;
			case MODEL_SUMMONER_CASTING_EFFECT2:
				o->Angle[2] += 3.0f;
				break;
			case MODEL_SUMMONER_CASTING_EFFECT22:
				o->Angle[2] -= 3.0f;
				break;
			case MODEL_SUMMONER_CASTING_EFFECT222:
				o->Angle[2] += 3.0f;
				break;
			case MODEL_SUMMONER_CASTING_EFFECT4:
				o->Scale += 0.6f;
				break;
			}
		}
		break;
		{
			if (o->LifeTime < 20) o->BlendMeshLight -= 0.03f;
			else if (o->BlendMeshLight < 0.5f) o->BlendMeshLight += 0.05f;

			o->BlendMeshTexCoordV += 0.05f;
		}
		break;
	case MODEL_SUMMONER_SUMMON_SAHAMUTT:
		{
			EVector3 vTempPosition;
			VectorCopy(o->Position, vTempPosition);

			if (o->LifeTime < 20)
			{
				SetAction(o, 0);
				o->Alpha -= 0.05f;
			}
			else if (o->CurrentAction == 0 && o->Alpha < 0.3f)
			{
				o->Alpha += 0.05f;
				{
					o->Angle[2] = CreateAngle(o->Position[0], o->Position[1], o->HeadTargetAngle[0], o->HeadTargetAngle[1]);
					mu_float dx = o->HeadTargetAngle[0] - o->Position[0];
					mu_float dy = o->HeadTargetAngle[1] - o->Position[1];
					o->Distance = EMath::Sqrt(dx*dx + dy*dy);
				}
			}
			else
			{
				SetAction(o, 1);
				if (o->AnimationFrame >= 12.0f)
				{
					o->AnimationFrame = 12.0f;
				}

				if (o->AnimationFrame >= 11.0f)
				{
					if (o->Alpha > 0)
						o->Alpha -= 0.3f;
					else
						o->Alpha = 0;
				}
				else if (o->AnimationFrame < 3.0f && o->Alpha < 0.7f)
				{
					o->Alpha += 0.05f;
				}

				if (o->AnimationFrame > 4.0f && o->AnimationFrame < 12.0f)
				{
					AngleMatrix(o->Angle, Matrix);
					EVector3 vMoveDir, Position;
					if (o->AnimationFrame < 10.0f)
					{
						Vector(0, o->Distance / -13.0f, 0, vMoveDir);
					}
					else
					{
						Vector(0, o->Distance / -45.0f, 0, vMoveDir);
					}
					VectorRotate(vMoveDir, Matrix, Position);
					VectorAdd(o->Position, Position, o->Position);
				}
				if (o->AnimationFrame <= 4.0f)
				{
					o->Angle[2] = CreateAngle(o->Position[0], o->Position[1], o->HeadTargetAngle[0], o->HeadTargetAngle[1]);
					mu_float dx = o->HeadTargetAngle[0] - o->Position[0];
					mu_float dy = o->HeadTargetAngle[1] - o->Position[1];
					o->Distance = EMath::Sqrt(dx*dx + dy*dy);
				}
			}
			o->Position[2] = RequestTerrainHeight(o->Position[0], o->Position[1]);

			VectorSubtract(vTempPosition, o->Position, vTempPosition);

			if (o->AnimationFrame >= 11.0f)
			{
				CreateBomb3(ThreadIndex, o->Position, o->SubType);
				if (GetLargeRand(2) == 0)
					PlayBackground(SOUND_SUMMON_EXPLOSION);
			}

			MUModel * pModel = &MODELS::Data[o->Type];
			pModel->Animation(ThreadIndex, ThreadInfo.BoneTransform, o->AnimationFrame, o->PriorAnimationFrame, o->PriorAction, o->Angle, o->HeadAngle);
			EVector3 vPos, vRelative, vLight;
			Vector(0.0f, 0.0f, 0.0f, vRelative);
			mu_int32 iPositions[] = { 13, 23, 39, 49, 3, 4, 5, 61 };
			for (mu_int32 i = 0; i < 8; ++i)
			{
				pModel->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[pModel->GetBoneIndex(iPositions[i])], vRelative, vPos, false);
				VectorAdd(vPos, vTempPosition, vPos);
				Vector(o->Alpha * 0.3f, o->Alpha * 0.3f, o->Alpha * 0.3f, vLight);
				CreateParticle(ThreadIndex, BITMAP_FIRE_CURSEDLICH, vPos, o->Angle, vLight, 2, 5, o);
				CreateParticle(ThreadIndex, BITMAP_FIRE_CURSEDLICH, vPos, o->Angle, vLight, 2, 4, o);
				CreateParticle(ThreadIndex, BITMAP_FIRE_CURSEDLICH, vPos, o->Angle, vLight, 2, 3, o);
			}
		}
		break;
	case MODEL_SUMMONER_SUMMON_NEIL:
		{
			if (o->LifeTime < 20) o->Alpha -= 0.05f;
			else if (o->Alpha < 0.7f) o->Alpha += 0.04f;

			if (o->AnimationFrame > 8 && o->Skill == 0)
			{
				o->Skill = 1;
				CreateEffect(ThreadIndex, MODEL_SUMMONER_SUMMON_NEIL_NIFE1, o->HeadTargetAngle, o->Angle, o->Light, o->SubType);
				if (o->SubType >= 1)
					CreateEffect(ThreadIndex, MODEL_SUMMONER_SUMMON_NEIL_NIFE2, o->HeadTargetAngle, o->Angle, o->Light, o->SubType);
				if (o->SubType >= 2)
					CreateEffect(ThreadIndex, MODEL_SUMMONER_SUMMON_NEIL_NIFE3, o->HeadTargetAngle, o->Angle, o->Light, o->SubType);
			}
			if (o->AnimationFrame > 10 && o->Skill == 1)
			{
				o->Skill = 2;

				AngleMatrix(o->Angle, Matrix);
				EVector3 vMoveDir, vPosition;
				Vector(0, -60.0f, 0, vMoveDir);
				VectorRotate(vMoveDir, Matrix, vPosition);
				VectorAdd(o->Position, vPosition, vPosition);
				CreateEffect(ThreadIndex, MODEL_SUMMONER_SUMMON_NEIL_GROUND1, vPosition, o->Angle, o->Light, o->SubType, o);

				CreateEffect(ThreadIndex, MODEL_SUMMONER_SUMMON_NEIL_GROUND1, o->HeadTargetAngle, o->Angle, o->Light, o->SubType);
				if (o->SubType >= 1)
					CreateEffect(ThreadIndex, MODEL_SUMMONER_SUMMON_NEIL_GROUND2, o->HeadTargetAngle, o->Angle, o->Light, o->SubType);
				if (o->SubType >= 2)
					CreateEffect(ThreadIndex, MODEL_SUMMONER_SUMMON_NEIL_GROUND3, o->HeadTargetAngle, o->Angle, o->Light, o->SubType);

				PlayBackground(SOUND_SUMMON_REQUIEM);
			}
		}
		break;
	case MODEL_SUMMONER_SUMMON_NEIL_NIFE1:
	case MODEL_SUMMONER_SUMMON_NEIL_NIFE2:
	case MODEL_SUMMONER_SUMMON_NEIL_NIFE3:
		{
			if (o->LifeTime < 20) o->Alpha -= 0.05f;
			else if (o->Alpha < 1.0f) o->Alpha += 0.05f;
		}
		break;
	case MODEL_SUMMONER_SUMMON_NEIL_GROUND1:
	case MODEL_SUMMONER_SUMMON_NEIL_GROUND2:
	case MODEL_SUMMONER_SUMMON_NEIL_GROUND3:
		{
			if (o->LifeTime < 20) o->Alpha -= 0.05f;
			else if (o->Alpha < 1.0f) o->Alpha += 0.3f;
		}
		break;
	case MODEL_SUMMONER_SUMMON_LAGUL:
		{
			if (o->SubType == 0)
			{
				mu_int32 anEffectVolume[3] = { 5, 4, 3 };
				if (GetLargeRand(anEffectVolume[o->PKKey]) == 0)
				{
					EVector3 vPos, vLight;

					VectorCopy(o->HeadTargetAngle, vPos);
					vPos[0] += (mu_float)(GetLargeRand(500) - 250);
					vPos[1] += (mu_float)(GetLargeRand(500) - 250);
					Vector(1.0f, 1.0f, 1.0f, vLight);
					CreateParticle(ThreadIndex, BITMAP_SMOKE, vPos, o->Angle, vLight, 57, 3.5f);

					VectorCopy(o->HeadTargetAngle, vPos);
					vPos[0] += (mu_float)(GetLargeRand(400) - 200);
					vPos[1] += (mu_float)(GetLargeRand(400) - 200);
					Vector(0.6f, 0.1f, 1.0f, vLight);
					CreateEffect(ThreadIndex, BITMAP_CLOUD, vPos, o->Angle, vLight, 0, nullptr, -1, 0, 0, 0, 2.0f);

					Vector(0.6f, 0.1f, 1.0f, vLight);
					CreateParticle(ThreadIndex, BITMAP_TWINTAIL_WATER, vPos, o->Angle, vLight, 1);
				}
			}
			else if (o->SubType == 1)
			{
				SetAction(o, 0);

				if (o->LifeTime < 30)
					o->Alpha = o->LifeTime / 40.0f;
				else if (o->LifeTime > 144)
					o->Alpha = (160 - o->LifeTime) / 20.0f;
			}
		}
		break;
	case BITMAP_FIRE:
		CreateParticle(ThreadIndex, BITMAP_FIRE, o->Position, o->Angle, Light, 9, 1.0f, o->Owner);
		break;

	case BITMAP_FIRE + 1:
		Vector(1.0f, 1.0f, 1.0f, Light);
		CreateParticle(ThreadIndex, BITMAP_FIRE + 1, o->Position, o->Angle, Light, 1);
		Vector(Luminosity*1.0f, Luminosity*0.6f, Luminosity*0.3f, Light);
		AddTerrainLight(o->Position[0], o->Position[1], Light, 2, PrimaryTerrainLight);
		break;
	case BITMAP_MAGIC:
		if (o->SubType == 0)
		{
			CreateEffect(ThreadIndex, BITMAP_MAGIC, o->Position, o->Angle, o->Light, 1);
			if (o->LifeTime>5 && o->LifeTime<10)
			{
				CreateParticle(ThreadIndex, BITMAP_FLARE, o->Position, o->Angle, o->Light, 0, 0.19f, o);
			}
		}
		else if (o->SubType == 2 || o->SubType == 3 || o->SubType == 7)
		{
			if (o->LifeTime>5 && o->LifeTime<10)
			{
				if (o->SubType == 3)
					CreateParticle(ThreadIndex, BITMAP_FLARE, o->Position, o->Angle, o->Light, 10, 0.19f, o);
				else
					CreateParticle(ThreadIndex, BITMAP_FLARE, o->Position, o->Angle, o->Light, 0, 0.19f, o);
			}
		}
		else if (o->SubType == 4)
		{
			CreateParticle(ThreadIndex, BITMAP_FLARE, o->Position, o->Angle, o->Light, 12, 0.19f, o);
		}
		else if (o->SubType == 8)
		{
			o->Scale += 1.8f;
		}
		else if (o->SubType == 9)
		{
			if (o->LifeTime < 20) o->Alpha -= 0.05f;
			else if (o->Alpha < 1.0f) o->Alpha += 0.05f;
			
			o->HeadAngle[0] += 4.0f;
			o->HeadAngle[1] -= 8.0f;
			o->HeadAngle[2] += 4.0f;
		}
		else if (o->SubType == 10)
		{
			if (o->LifeTime < 20) o->Alpha -= 0.03f;
			else if (o->Alpha < 1.0f) o->Alpha += 0.05f;
		}
		else if (o->SubType == 11)
		{
			o->HeadAngle[0] += 2.0f;
			o->HeadAngle[1] -= 2.0f;
			o->HeadAngle[2] += 2.0f;

			if (o->LifeTime <= 10)
			{
				o->Alpha -= 0.05f;
				o->Light[0] *= o->Alpha;
				o->Light[1] *= o->Alpha;
				o->Light[2] *= o->Alpha;
			}
		}
		else if (o->SubType == 12)
		{
			if (o->Alpha > 0.0f)
			{
				o->Alpha -= 0.05f;
			}
			else
			{
				o->Alpha = 0.0f;
			}

			o->Scale += o->Alpha * 1.0f;
			if (o->Scale > 5.0f)
			{
				o->Light[0] *= 0.5f;
				o->Light[1] *= 0.5f;
				o->Light[2] *= 0.5f;
			}
		}
		break;
	case BITMAP_MAGIC + 1:
	case BITMAP_MAGIC + 2:
		if (o->SubType >= 1
			&& o->SubType != 4 && o->SubType != 6 && o->SubType != 7 && o->SubType != 8 && o->SubType != 9 && o->SubType != 10
			&& o->SubType != 11 && o->SubType != 12
			&& o->SubType != 13
			)
		{
			CreateHealing(ThreadIndex, o);
		}
		else if (o->SubType == 6)
		{
			MUModel *b = &MODELS::Data[o->Owner->Type];
			Vector(1, 1, 1, Light);
			VectorCopy(o->StartPosition, b->BodyOrigin(ThreadIndex));
			o->BlendMeshLight = (mu_float)(GetLargeRand(10))*0.1f;

			mu_int32 iIndex;
			for (mu_int32 i = 0; i < o->LifeTime; ++i)
			{
				iIndex = GetLargeRand(b->_BoneNonDummyEnd);
				Vector(-50 + GetLargeRand(100), -10 + GetLargeRand(20), -10 + GetLargeRand(20), p);
				b->TransformPosition(ThreadIndex, o->Owner->BoneTransform[iIndex], p, Position, true);
				CreateParticle(ThreadIndex, BITMAP_LIGHT, Position, o->Owner->Angle, Light, 8);
			}
			o->Owner->Alpha = o->LifeTime / 80.0f;
		}
		else if (o->SubType == 9)
		{
			o->Scale += 0.3f;
			o->Light[0] /= 1.1f;
			o->Light[1] /= 1.1f;
			o->Light[2] /= 1.1f;
		}
		break;
	case BITMAP_OUR_INFLUENCE_GROUND:
	case BITMAP_ENEMY_INFLUENCE_GROUND:
		if (o->SubType == 0)
		{
			if (o->Owner == nullptr || o->Owner->Flags.Get(OBJECT::eOF_LIVE) == false)
				o->Flags.Clear(OBJECT::eOF_LIVE);

			VectorCopy(o->Owner->Position, o->Position);

			o->Alpha -= 0.02f;
			o->Scale += 0.01f;
			if (o->Alpha < 0.0f)
			{
				o->Alpha = 1.0f;
				o->Scale = 0.6f;
			}

			if (o->LifeTime < 25)
				o->AlphaTarget -= 0.02f;
			else if (o->AlphaTarget < 1.0f)
				o->AlphaTarget += 0.02f;

			if (1 == o->LifeTime)
				o->LifeTime = 50;
		}
		break;

	case BITMAP_MAGIC_ZIN:
		switch (o->SubType)
		{
		case 0:
			if (o->LifeTime < 20)
				o->Alpha -= 0.05f;
			else if (o->Alpha < 1.0f)
				o->Alpha += 0.05f;
			break;
		case 1:
			if (o->LifeTime < 20)
				o->Alpha -= 0.03f;
			else if (o->Alpha < 0.7f)
				o->Alpha += 0.06f;
			break;
		case 2:
			if (o->Scale < 3.5f)
				o->Scale += 0.1f;
			if (o->LifeTime < 20)
				o->Alpha -= 0.05f;
			else if (o->Alpha < 1.0f)
				o->Alpha += 0.05f;
			break;
		}
		break;

	case BITMAP_SHINY + 6:
		switch (o->SubType)
		{
		case 0:
			Position[0] = o->Position[0] + (mu_float)(GetLargeRand(500) - 250);
			Position[1] = o->Position[1] + (mu_float)(GetLargeRand(500) - 250);
			Position[2] = o->Position[2] - (mu_float)(GetLargeRand(100)) + 150.0f;
			CreateParticle(ThreadIndex, o->Type, Position, o->Angle, o->Light, 0, o->Scale);
			break;
		case 1:
		case 2:
			if (o->Owner == nullptr || o->Owner->Flags.Get(OBJECT::eOF_LIVE) == false)
				o->Flags.Clear(OBJECT::eOF_LIVE);
			else
			{
				o->LifeTime = 100;
				MUModel* pModel = &MODELS::Data[o->Owner->Type];
				mu_int32 iBone = GetLargeRand(pModel->_BoneNonDummyEnd);
				EVector3 vRelativePos, vWorldPos;
				Vector(0.0f, 0.0f, 100.0f, vRelativePos);
				pModel->TransformPosition(ThreadIndex, o->Owner->BoneTransform[iBone], vRelativePos, vWorldPos, false);
				VectorScale(vWorldPos, pModel->BodyScale(ThreadIndex), vWorldPos);
				VectorAdd(vWorldPos, o->Owner->Position, vWorldPos);
				if (GetLargeRand(2) == 0)
				{
					vWorldPos[2] -= 20.0f;
					CreateParticle(ThreadIndex, o->Type, vWorldPos, o->Angle, o->Light, 0, o->Scale);
				}
			}
			break;
		case 3:
			{
				EVector3 Position, P, dp;
				EVector3 vFirePosition;

				EBone Matrix;
				mu_int32 iNumBones = MODELS::Data[o->Owner->Type]._BoneNonDummyEnd;
				MODELS::Data[o->Owner->Type].TransformByObjectBoneDirect(ThreadIndex, vFirePosition, o->Owner, GetLargeRand(iNumBones));
				Vector(0.0f, -20.0f, 0.0f, P);
				AngleMatrix(o->Owner->Angle, Matrix);
				VectorRotate(P, Matrix, dp);
				VectorAdd(dp, vFirePosition, Position);
				CreateSprite(BITMAP_SHINY + 6, Position, 2.0f, o->Light, o, 0.0f, 1);

			}
			break;
		}
		break;
	case MODEL_DESAIR:
		{
			JOINT_HEADER *ojh = &g_JointsHeader[o->m_sTargetIndex];
			if (ojh->Flags.Get(JOINT::eJF_LIVE) == false)
			{
				o->LifeTime = 1;
				break;
			}

			JOINT *oj = &g_Joints[o->m_sTargetIndex];
			VectorCopy(oj->Position, o->Position);
			VectorCopy(oj->Angle, o->Angle);
			if (o->LifeTime % 10 == 0)
			{
				CreateEffect(ThreadIndex, MODEL_FEATHER, o->Position, o->Angle, o->Light, 2, nullptr, -1, 0, 0, 0, 1.4f);
				CreateEffect(ThreadIndex, MODEL_FEATHER, o->Position, o->Angle, o->Light, 3, nullptr, -1, 0, 0, 0, 1.4f);
			}
		}
		break;
	case BITMAP_PIN_LIGHT:
		switch (o->SubType)
		{
		case 3:
		case 0:
			Position[0] = o->Position[0] + (mu_float)(GetLargeRand(500) - 250);
			Position[1] = o->Position[1] + (mu_float)(GetLargeRand(500) - 250);
			Position[2] = o->Position[2] - (mu_float)(GetLargeRand(100)) + 150.0f;
			if (GetLargeRand(2) == 0)
			{
				if (o->SubType == 3)
					CreateParticle(ThreadIndex, o->Type, Position, o->Angle, o->Light, 1, o->Scale);
				else
					CreateParticle(ThreadIndex, o->Type, Position, o->Angle, o->Light, 0, o->Scale);
			}
			break;
		case 1:
		case 2:
			if (o->Owner == nullptr || o->Owner->Flags.Get(OBJECT::eOF_LIVE) == false)
				o->Flags.Clear(OBJECT::eOF_LIVE);
			else
			{
				o->LifeTime = 100;
				MUModel* pModel = &MODELS::Data[o->Owner->Type];
				mu_int32 iBone = GetLargeRand(pModel->_BoneNonDummyEnd);
				EVector3 vRelativePos, vWorldPos;
				Vector(0.0f, 0.0f, 100.0f, vRelativePos);
				pModel->TransformPosition(ThreadIndex, o->Owner->BoneTransform[iBone], vRelativePos, vWorldPos, false);
				VectorScale(vWorldPos, pModel->BodyScale(ThreadIndex), vWorldPos);
				VectorAdd(vWorldPos, o->Owner->Position, vWorldPos);
				if (GetLargeRand(2) == 0)
				{
					vWorldPos[2] -= 20.0f;
					CreateParticle(ThreadIndex, o->Type, vWorldPos, o->Angle, o->Light, 0, o->Scale);
				}
			}
			break;
		case 4:
			if (o->Owner == nullptr || o->Owner->Flags.Get(OBJECT::eOF_LIVE) == false || o->Alpha <= 0.0f)
			{
				o->Flags.Clear(OBJECT::eOF_LIVE);
			}
			else
			{
				o->Scale -= 0.02f;
				o->Alpha -= 0.001f;

				OBJECT *Owner = o->Owner;
				MUModel* pModel = &MODELS::Data[o->Owner->Type];
				EVector3 vWorldPos, vRelativePos;
				Vector(0.0f, 0.0f, 0.0f, vRelativePos);

				if (!pModel->_BoneRef[11].Dummy)
				{
					pModel->BodyScale(ThreadIndex) = Owner->Scale;
					pModel->Animation(ThreadIndex, ThreadInfo.BoneTransform, Owner->AnimationFrame, Owner->PriorAnimationFrame, Owner->PriorAction, Owner->Angle, Owner->HeadAngle, false, false);
					pModel->TransformByObjectBone(ThreadIndex, vWorldPos, Owner, 11);
					VectorCopy(vWorldPos, o->Position);

					CreateSprite(BITMAP_PIN_LIGHT, vWorldPos, o->Scale, o->Light, Owner, o->Angle[1]);
				}
			}break;
		}
		break;

	case BITMAP_ORORA:
		if (o->Owner == nullptr || o->Owner->Flags.Get(OBJECT::eOF_LIVE) == false)
			o->Flags.Clear(OBJECT::eOF_LIVE);
		else
		{
			if (o->LifeTime <= 5)
			{
				o->Flags.Clear(OBJECT::eOF_LIVE);
				CreateEffect(ThreadIndex, o->Type, o->Position, o->Angle, o->Light, o->SubType, o->Owner);
			}
		}
		break;

	case BITMAP_SPARK + 2:
		if (o->Owner == nullptr || o->Owner->Flags.Get(OBJECT::eOF_LIVE) == false)
			o->Flags.Clear(OBJECT::eOF_LIVE);
		else
		{
			o->LifeTime = 100;

			if (GetLargeRand(60) == 0)
				CreateParticle(ThreadIndex, o->Type, o->Position, o->Angle, o->Light, o->SubType, 0.5f, o->Owner);
		}
		break;

	case BITMAP_LIGHT_MARKS:
		if (o->Owner == nullptr || o->Owner->Flags.Get(OBJECT::eOF_LIVE) == false)
			o->Flags.Clear(OBJECT::eOF_LIVE);
		else
		{
			if (o->LifeTime <= 5)
				o->LifeTime = 65;
		}
		break;

	case BITMAP_GATHERING:
		if (o->SubType == 1 || o->SubType == 2)
		{
			MUModel* b = &MODELS::Data[o->Owner->Type];
			Vector(0.0f, 0.0f, 10.0f, p);
			VectorCopy(o->StartPosition, b->BodyOrigin(ThreadIndex));
			b->TransformPosition(ThreadIndex, o->Owner->BoneTransform[b->GetBoneIndex(33)], p, o->Position, true);
		}
		for (mu_int32 j = 0; j<3; ++j)
		{
			Vector(0.0f, 120.0f, 0.0f, p);
			if (o->SubType == 3)
			{
				Vector(0.0f, 25.0f, 0.0f, p);
			}

			Vector((mu_float)(GetLargeRand(360)), 0.0f, (mu_float)(GetLargeRand(360)), Angle);
			AngleMatrix(Angle, Matrix);
			VectorRotate(p, Matrix, Position);
			VectorAdd(o->Position, Position, Position);

			if (o->SubType == 1)
			{
				if ((o->LifeTime % 2) == 0)
				{
					CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, Position, o->Position, Angle, 3, nullptr, 10.0f, 10, 10);
				}
				else
				{
					CreateParticle(ThreadIndex, BITMAP_SPARK + 1, Position, Angle, Light, 2, (GetLargeRand(50) + 10) / 100.0f, o);
				}
				CreateSprite(BITMAP_SHINY + 1, o->Position, (mu_float)(GetLargeRand(8) + 8)*0.2f, o->Light, o, (mu_float)(GetLargeRand(360)));
			}
			else if (o->SubType == 2)
			{
				if ((o->LifeTime % 2) == 0)
				{
					CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, Position, o->Position, Angle, 3, nullptr, 10.0f, 10, 10);
				}
				CreateSprite(BITMAP_SHINY + 1, o->Position, (mu_float)(GetLargeRand(8) + 8)*0.2f, o->Light, o, (mu_float)(GetLargeRand(360)));
			}
			else if (o->SubType == 3)
			{
				CreateParticle(ThreadIndex, BITMAP_SPARK + 1, Position, Angle, o->Light, 26, (GetLargeRand(10) + 5) / 25.0f, o);
			}
			else
			{
				CreateSprite(BITMAP_SHINY + 1, o->Position, (mu_float)(GetLargeRand(8) + 8)*0.3f, o->Light, o, (mu_float)(GetLargeRand(360)));
				CreateParticle(ThreadIndex, BITMAP_SPARK + 1, Position, Angle, Light, 2, (GetLargeRand(50) + 10) / 100.0f, o);
			}
		}
		break;

	case BITMAP_JOINT_THUNDER:
		{
			mu_float fScale = (GetLargeRand(400) + 100) / 10.0f;
			EVector3 vPosition, vStartPosition;
			VectorCopy(o->StartPosition, vStartPosition);
			VectorCopy(o->Position, vPosition);
			vStartPosition[0] += GetLargeRand(100) - 50;
			vStartPosition[1] += GetLargeRand(100) - 50;
			vPosition[0] += GetLargeRand(100) - 50;
			vPosition[1] += GetLargeRand(100) - 50;

			if (GetLargeRand(10) > 4)
			{
				if (o->SubType == 1)
				{
					CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, vStartPosition, vPosition, o->Angle, 33, nullptr, fScale);
				}
				else
				{
					CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, vStartPosition, vPosition, o->Angle, 16, nullptr, fScale);
				}
			}

			EVector3 vLight = { 0.45f, 0.45f, 0.7f };
			if (o->LifeTime > 10)
			{
				if (GetLargeRand(2) == 0)
				{
					CreateEffect(ThreadIndex, BITMAP_MAGIC + 1, vPosition, o->Angle, vLight, 11, o);
				}
			}

			if (GetLargeRand(4) == 0)
			{
				CreateParticle(ThreadIndex, BITMAP_SMOKE, vPosition, o->Angle, vLight, 54, 2.8f);
			}

			if (GetLargeRand(4) == 0)
			{
				CreateEffect(ThreadIndex, MODEL_STONE1 + GetLargeRand(2), o->Position, o->Angle, vLight, 13);
			}

			if (o->LifeTime > 5)
			{
				Vector(0.15f, 0.15f, 0.4f, vLight);
				if (GetLargeRand(5) == 0)
				{
					CreateEffect(ThreadIndex, BITMAP_CHROME_ENERGY2, vPosition, o->Angle, vLight, 0);
				}
			}

			if (o->Owner != nullptr && GetLargeRand(5) == 0
				&& o->Owner->BoneTransform != nullptr
				)
			{
				MUModel *pTargetModel = &MODELS::Data[o->Owner->Type];
				mu_int32 iNumBones = pTargetModel->_BoneNonDummyEnd;
				mu_float fRandom;
				EVector3 vLight, vRelativePos, vPos, vAngle;
				Vector(0.0f, 0.0f, 0.0f, vRelativePos);
				for (mu_int32 i = 0; i < iNumBones; ++i)
				{
					if (iNumBones > 100 && GetLargeRand(iNumBones) > iNumBones / 10) continue;
					else if (iNumBones > 50 && GetLargeRand(iNumBones) > iNumBones / 5) continue;
					else if (iNumBones > 20 && GetLargeRand(iNumBones) > iNumBones / 2) continue;
					VectorCopy(o->Owner->Position, pTargetModel->BodyOrigin(ThreadIndex));
					pTargetModel->TransformPosition(ThreadIndex, o->Owner->BoneTransform[i], vRelativePos, vPos, true);

					Vector(0.2f, 0.2f, 0.8f, vLight);
					fRandom = 3.0f + ((mu_float)(GetLargeRand(20) - 10)*0.1f);
					CreateParticle(ThreadIndex, BITMAP_LIGHT, vPos, vAngle, vLight, 5, fRandom);
				}
			}
		}
		break;
	case BITMAP_IMPACT:
		{
			mu_float ScaleBk = 0.0f;
			if (o->Scale < 2.0f)
			{
				o->Scale += 0.1f;
				ScaleBk = o->Scale;
			}
			else
			{
				if (o->Scale < 2.4f)
					o->Scale += 0.02f;
				else
					o->Scale = 2.0f;

				if (o->Scale >= 2.2f)
					ScaleBk = 2.2f - (o->Scale - 2.2f);
				else
					ScaleBk = o->Scale;
			}
			EVector3 Light, P, dp;

			if (o->LifeTime <= 40)
			{
				o->Alpha -= 0.1f;
				o->BlendMeshLight -= 0.1f;
			}
			EBone Matrix;
			Vector(-10.0f, -30.0f, 0.0f, P);
			Vector(o->Light[0] * o->Alpha, o->Light[1] * o->Alpha, o->Light[2] * o->Alpha, Light);
			AngleMatrix(o->Owner->Angle, Matrix);
			VectorRotate(P, Matrix, dp);
			VectorAdd(dp, o->Owner->Position, o->Position);
			o->Position[2] += 130;
			CreateSprite(BITMAP_IMPACT, o->Position, ScaleBk, Light, o);
			CreateSprite(BITMAP_SHINY + 1, o->Position, ScaleBk, Light, o, -FWorldTime*0.08f);
			if (o->Scale > 1.0f)
				CreateSprite(BITMAP_ORORA, o->Position, ScaleBk, Light, o, -FWorldTime*0.08f);
			if (o->Scale > 2.0f)
				CreateSprite(BITMAP_ORORA, o->Position, ScaleBk, Light, o, FWorldTime*0.08f);
		}
		break;
	case BITMAP_FLAME:
		if (o->SubType == 0)
		{
			for (mu_int32 j = 0; j<6; ++j)
			{
				Vector((mu_float)(GetLargeRand(50) - 25), (mu_float)(GetLargeRand(50) - 25), 0.0f, Position);
				VectorAdd(Position, o->Position, Position);
				CreateParticle(ThreadIndex, BITMAP_FLAME, Position, o->Angle, Light);
			}
			if (GetLargeRand(8) == 0)
				CreateEffect(ThreadIndex, MODEL_STONE1 + GetLargeRand(2), o->Position, o->Angle, o->Light);

			Vector(Luminosity*1.0f, Luminosity*0.4f, Luminosity*0.0f, Light);
			AddTerrainLight(o->Position[0], o->Position[1], Light, 3, PrimaryTerrainLight);
		}
		else if (o->SubType == 1 || o->SubType == 2)
		{
			for (mu_int32 j = 0; j<18; ++j)
			{
				Vector(0.0f, 250.0f, 0.0f, p);
				Vector(0.0f, 0.0f, j*20.0f, Angle);
				AngleMatrix(Angle, Matrix);
				VectorRotate(p, Matrix, Position);
				VectorAdd(Position, o->Position, Position);
				Position[0] += GetLargeRand(64) - 32;
				Position[1] += GetLargeRand(64) - 32;
				if (o->SubType == 1)
					CreateParticle(ThreadIndex, BITMAP_FLAME, Position, o->Angle, Light, 0, 1.2f);
				else if (o->SubType == 2)
					CreateParticle(ThreadIndex, BITMAP_FIRE + 3, Position, o->Angle, Light, 13, 2.5f);
			}
		}
		else if (o->SubType == 3)
		{
			for (mu_int32 j = 0; j<3; ++j)
			{
				CreateParticle(ThreadIndex, BITMAP_FLAME, o->Position, o->Angle, Light, 6);
				Vector((mu_float)(GetLargeRand(10) - 5), (mu_float)(GetLargeRand(10) - 5), 40.0f, Position);
				VectorAdd(Position, o->Position, Position);
				CreateParticle(ThreadIndex, BITMAP_TRUE_FIRE, Position, o->Angle, Light, 0, 2.8f);
			}
			Vector((mu_float)(GetLargeRand(10) - 5), (mu_float)(GetLargeRand(10) - 5), -40.0f, Position);
			VectorAdd(Position, o->Position, Position);
			CreateParticle(ThreadIndex, BITMAP_SMOKE, Position, o->Angle, Light, 21, 0.8f);

			CreateEffect(ThreadIndex, MODEL_STONE1 + GetLargeRand(2), o->Position, o->Angle, o->Light, 12);

			Vector(Luminosity*1.0f, Luminosity*0.4f, Luminosity*0.0f, Light);
			AddTerrainLight(o->Position[0], o->Position[1], Light, 3, PrimaryTerrainLight);
		}
		else if (o->SubType == 4)
		{
			o->Scale += 20.0f;
			for (mu_int32 j = 0; j<18; ++j)
			{
				Vector(0.0f, 150.0f - o->Scale, 0.0f, p);
				Vector(0.0f, 0.0f, j*20.0f, Angle);
				AngleMatrix(Angle, Matrix);
				VectorRotate(p, Matrix, Position);
				VectorAdd(Position, o->Position, Position);
				Position[0] += GetLargeRand(64) - 32;
				Position[1] += GetLargeRand(64) - 32;
				CreateParticle(ThreadIndex, BITMAP_FLAME, Position, o->Angle, Light, 0, 1.2f);
			}
			CreateEffect(ThreadIndex, MODEL_STONE1 + GetLargeRand(2), o->Position, o->Angle, o->Light, 12);
		}
		else if (o->SubType == 5)
		{
			Vector((mu_float)(GetLargeRand(32) - 16), (mu_float)(GetLargeRand(32) - 16), 0.0f, Position);
			VectorAdd(Position, o->Position, Position);
			CreateParticle(ThreadIndex, BITMAP_FLAME, Position, o->Angle, Light, 0, o->Scale);
		}
		else if (o->SubType == 6)
		{
			Vector((mu_float)(GetLargeRand(32) - 16), (mu_float)(GetLargeRand(32) - 16), 0.0f, Position);
			VectorAdd(Position, o->Position, Position);
			CreateParticle(ThreadIndex, BITMAP_FLAME, Position, o->Angle, o->Light, 12, o->Scale);
		}
		break;

	case MODEL_RAKLION_BOSS_CRACKEFFECT:
		if (o->SubType == 0)
		{
			o->Alpha -= 0.03f;
		}
		break;

	case MODEL_RAKLION_BOSS_MAGIC:
		if (o->SubType == 0)
		{
			o->Alpha -= 0.03f;
		}
		break;

	case BITMAP_FIRE_HIK2_MONO:
		if (o->SubType == 0)
		{
			for (mu_int32 i = 0; i<2; ++i)
			{
				Vector((mu_float)(GetLargeRand(30) - 15), (mu_float)(GetLargeRand(30) - 15), 0.0f, p);
				VectorAdd(p, o->Position, Position);
				if (GetLargeRand(3) != 0)
				{
					CreateParticle(ThreadIndex, BITMAP_FLAME, Position, o->Angle, o->Light, 11, 1.4f);
				}

				if (GetLargeRand(8) == 0)
				{
					CreateEffect(ThreadIndex, MODEL_ICE_SMALL, Position, o->Angle, o->Light, 0);
				}
			}
		}
		else if (o->SubType == 1)
		{
			for (mu_int32 j = 0; j<9; ++j)
			{
				Vector(0.0f, 250.0f, 0.0f, p);
				Vector(0.0f, 0.0f, j*40.0f, Angle);
				AngleMatrix(Angle, Matrix);
				VectorRotate(p, Matrix, Position);
				VectorAdd(Position, o->Position, Position);
				Position[0] += GetLargeRand(64) - 32;
				Position[1] += GetLargeRand(64) - 32;
				CreateParticle(ThreadIndex, BITMAP_FIRE + 3, Position, o->Angle, Light, 13, 2.5f);
			}
		}
		break;

	case BITMAP_CLOUD:
		if (o->SubType == 0)
		{
			o->Light[0] /= 1.05f;
			o->Light[1] /= 1.05f;
			o->Light[2] /= 1.05f;
			o->Scale += 0.03f;
		}
		break;

	case BITMAP_FIRE_RED:
		Vector((mu_float)(GetLargeRand(32) - 16), (mu_float)(GetLargeRand(32) - 16), 0.0f, Position);
		VectorAdd(Position, o->Position, Position);
		Vector(1.0f, 0.4f, 0.4f, Light);
		CreateParticle(ThreadIndex, BITMAP_FIRE_RED, Position, o->Angle, Light, 0, o->Scale);
		break;

	case BITMAP_SPARK + 1:
		{
			EVector3 Position;
			VectorCopy(o->Position, Position);
			mu_float Luminosity = o->LifeTime*0.1f;
			Vector(Luminosity, Luminosity, Luminosity, Light);
			mu_float Scale = 6.0f;
			for (mu_int32 j = 0; j<18; ++j)
			{
				Position[2] += Scale*4.0f;
				if (j == 0)
					CreateParticle(ThreadIndex, BITMAP_SPARK + 1, Position, o->Angle, Light, 1, Scale*2.0f);
				else
					CreateParticle(ThreadIndex, BITMAP_SPARK + 1, Position, o->Angle, Light, 1, Scale);
			}
			break;
		}

	case BITMAP_ENERGY:
		if (o->SubType == 0)
		{
			Luminosity = o->LifeTime*0.2f;
			Vector(Luminosity, Luminosity, Luminosity, Light);
			CreateParticle(ThreadIndex, BITMAP_ENERGY, o->Position, o->Angle, Light);
			CreateParticle(ThreadIndex, BITMAP_SPARK + 1, o->Position, o->Angle, Light, 0, 4.0f);
			Vector(Luminosity*0.2f, Luminosity*0.4f, Luminosity*1.0f, Light);
			AddTerrainLight(o->Position[0], o->Position[1], Light, 2, PrimaryTerrainLight);
			CheckTargetRange(ThreadIndex, o);
		}
		break;

	case MODEL_LIGHTNING_ORB:
		{
#ifndef ENABLE_SKILLPROCESSORS
			if (o->SubType == 0)
			{
				if (o->LifeTime > 20) break;
				if (o->LifeTime == 20)
				{
					PlayBackground(SOUND_SUMMON_SKILL_LIGHTORB);
				}
			}
#endif

			mu_float fRot = (FWorldTime*0.0006f) * 360.0f;
			EVector3 vLight;

			if (o->SubType == 0)
			{
				Vector(0.1f, 0.7f, 1.5f, vLight);
				CreateSprite(BITMAP_SHINY + 1, o->Position, 4.0f, vLight, o, fRot);
				CreateSprite(BITMAP_SHINY + 1, o->Position, 3.0f, vLight, o, -fRot);
				
				Vector(0.1f, 0.1f, 1.5f, vLight);
				CreateSprite(BITMAP_MAGIC, o->Position, 1.0f, vLight, o, fRot);
				CreateSprite(BITMAP_MAGIC, o->Position, 0.5f, vLight, o, -fRot);
				
				Vector(0.5f, 0.5f, 1.5f, vLight);
				CreateSprite(BITMAP_PIN_LIGHT, o->Position, 2.0f, vLight, o, (mu_float)(GetLargeRand(360)));
				CreateSprite(BITMAP_PIN_LIGHT, o->Position, 2.0f, vLight, o, (mu_float)(GetLargeRand(360)));

				Vector(0.4f, 0.4f, 1.5f, vLight);
				CreateParticle(ThreadIndex, BITMAP_MAGIC, o->Position, o->Angle, vLight, 0, 1.0f);
				for (mu_int32 i = 0; i<3; ++i)
				{
					CreateParticle(ThreadIndex, BITMAP_SPARK + 1, o->Position, o->Angle, o->Light, 13, 1.0f);
				}

				CheckTargetRange(ThreadIndex, o);
			}
			else if (o->SubType == 1)
			{
				EVector3 vLight;
				Vector(1.0f, 1.0f, 1.0f, vLight);
				if (o->LifeTime >= 5)
				{
					Vector(0.1f, 0.5f, 1.5f, vLight);
					CreateSprite(BITMAP_SHINY + 5, o->Position, 3.0f, vLight, o, fRot);
					CreateSprite(BITMAP_SHINY + 5, o->Position, 2.0f, vLight, o, -fRot);
					
					Vector(o->Light[0] * 0.3f, o->Light[1] * 0.3f, o->Light[2] * 1.0f, vLight);
					CreateSprite(BITMAP_PIN_LIGHT, o->Position, 4.0f, vLight, o, (mu_float)(GetLargeRand(360)));
					CreateSprite(BITMAP_PIN_LIGHT, o->Position, 4.0f, vLight, o, (mu_float)(GetLargeRand(360)));
					
					CreateSprite(BITMAP_ENERGY, o->Position, 4.0f, o->Light, o, fRot);
				}

				if (o->LifeTime >= 15)
				{
					for (mu_int32 i = 0; i<5; ++i)
					{
						CreateParticle(ThreadIndex, BITMAP_SPARK + 1, o->Position, o->Angle, o->Light, 20, 1.0f);
					}
				}

				if (o->LifeTime >= 14)
				{
					Vector(0.4f, 0.3f, 1.0f, vLight);
					CreateParticle(ThreadIndex, BITMAP_SHOCK_WAVE, o->Position, o->Angle, vLight, 0, 0.3f);
					CreateParticle(ThreadIndex, BITMAP_SHOCK_WAVE, o->Position, o->Angle, vLight, 0, 0.3f);
				}

				Vector(0.2f, 0.2f, 1.0f, vLight);
				for (mu_int32 i = 0; i<2; ++i)
				{
					CreateEffect(ThreadIndex, MODEL_FENRIR_THUNDER, o->Position, o->Angle, vLight, 3, o);

				}

				if (o->LifeTime <= 5)
				{
					for (mu_int32 i = 0; i<2; ++i)
					{
						CreateParticle(ThreadIndex, BITMAP_SMOKE, o->Position, o->Angle, o->Light, 40);
					}
				}

				o->Light[0] /= 1.08f;
				o->Light[1] /= 1.08f;
				o->Light[2] /= 1.08f;
			}
		}
		break;

	case MODEL_CHAIN_LIGHTNING:
		{
			switch (o->SubType)
			{
			case 0:
			case 1:
			case 2:
				{
					OBJECT* pSourceObj = o->Owner;
					CHARACTER* pTargetChar = &CharactersClient[FindCharacterIndex(o->m_sTargetIndex)];
					OBJECT* pTargetObj = &pTargetChar->Object;

					if (pSourceObj == nullptr || pSourceObj->Flags.Get(OBJECT::eOF_LIVE) == false
						|| pTargetObj == nullptr || pTargetObj->Flags.Get(OBJECT::eOF_LIVE) == false || pTargetChar->Dead > 0)
					{
						o->LifeTime = 0;
						o->Flags.Clear(OBJECT::eOF_LIVE);
						break;
					}

					MUModel *pSourceModel = &MODELS::Data[pSourceObj->Type];
					MUModel *pTargetModel = &MODELS::Data[pTargetObj->Type];

					EVector3 vLight, vRelativePos, vPos, vAngle;
					Vector(0.0f, 0.0f, 0.0f, vRelativePos);

					if (o->SubType == 1 || o->SubType == 2)
					{
						if (pSourceObj == pTargetObj)
							break;
					}

					if (o->SubType == 0)
					{
						VectorCopy(pSourceObj->Position, pSourceModel->BodyOrigin(ThreadIndex));

						Vector(0.4f, 0.4f, 1.0f, vLight);
						pSourceModel->TransformPosition(ThreadIndex, pSourceObj->BoneTransform[pSourceModel->GetBoneIndex(37)], vRelativePos, vPos, true);
						Vector(-60.0f, 0.0f, pSourceObj->Angle[2], vAngle);
						CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, vPos, pTargetObj->Position, vAngle, 0, pTargetObj, 50.0f, -1, 0, 0, -1, &vLight);
						CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, vPos, pTargetObj->Position, vAngle, 0, pTargetObj, 10.0f, -1, 0, 0, -1, &vLight);
						Vector(0.0f, 0.0f, (pSourceObj->Angle[2]) + 60.0f, vAngle);
						CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, vPos, pTargetObj->Position, vAngle, 0, pTargetObj, 50.0f, -1, 0, 0, -1, &vLight);
						CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, vPos, pTargetObj->Position, vAngle, 0, pTargetObj, 10.0f, -1, 0, 0, -1, &vLight);

						pSourceModel->TransformPosition(ThreadIndex, pSourceObj->BoneTransform[pSourceModel->GetBoneIndex(28)], vRelativePos, vPos, true);
						Vector(-60.0f, 0.0f, pSourceObj->Angle[2], vAngle);
						CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, vPos, pTargetObj->Position, vAngle, 0, pTargetObj, 50.0f, -1, 0, 0, -1, &vLight);
						CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, vPos, pTargetObj->Position, vAngle, 0, pTargetObj, 10.0f, -1, 0, 0, -1, &vLight);
						Vector(0.0f, 0.0f, (pSourceObj->Angle[2]) - 60.0f, vAngle);
						CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, vPos, pTargetObj->Position, vAngle, 0, pTargetObj, 50.0f, -1, 0, 0, -1, &vLight);
						CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, vPos, pTargetObj->Position, vAngle, 0, pTargetObj, 10.0f, -1, 0, 0, -1, &vLight);
					}
					else if (o->SubType == 1 || o->SubType == 2)
					{
						VectorCopy(pSourceObj->Position, vPos);
						vPos[2] += 80.0f;

						EVector3 vTargetPos;
						VectorCopy(pTargetObj->Position, vTargetPos);
						vTargetPos[2] += 80.0f;

						Vector(0.4f, 0.4f, 1.0f, vLight);
						CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, vPos, vTargetPos, pTargetObj->Angle, 0, pTargetObj, 50.0f, -1, 0, 0, -1, &vLight);
						CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, vPos, vTargetPos, pTargetObj->Angle, 0, pTargetObj, 10.0f, -1, 0, 0, -1, &vLight);
					}

					if (o->LifeTime == 15)
					{
						mu_int32 iNumBones = pTargetModel->_BoneNonDummyEnd;
						mu_float fRandom;
						for (mu_int32 i = 0; i<iNumBones; ++i)
						{
							VectorCopy(pTargetObj->Position, pTargetModel->BodyOrigin(ThreadIndex));
							pTargetModel->TransformPosition(ThreadIndex, pTargetObj->BoneTransform[i], vRelativePos, vPos, true);

							Vector(0.2f, 0.2f, 0.8f, vLight);
							fRandom = 3.0f + ((mu_float)(GetLargeRand(20) - 10)*0.1f);
							CreateParticle(ThreadIndex, BITMAP_LIGHT, vPos, vAngle, vLight, 5, fRandom);
						}
					}
				}
				break;
			}
		}
		break;


	case MODEL_ALICE_DRAIN_LIFE:
		{
#ifndef ENABLE_SKILLPROCESSORS
			if (o->LifeTime > 70) break;
			else if (o->LifeTime == 70)
			{
				PlayBackground(SOUND_SKILL_DRAIN_LIFE);
			}
#endif

			mu_int32 iNumBones = 0;

			EVector3 vSourcePos, vTargetPos, vLight, vRelativePos;
			Vector(0.0f, 0.0f, 0.0f, vRelativePos);

			OBJECT* pSourceObj = o->Owner;
			OBJECT* pTargetObj = pSourceObj->Owner;
			MUModel *pSourceModel = &MODELS::Data[pSourceObj->Type];
			MUModel *pTargetModel = &MODELS::Data[pTargetObj->Type];

			if (pSourceObj == nullptr || pTargetObj == nullptr ||
				pSourceObj->Flags.Get(OBJECT::eOF_LIVE) == false || pTargetObj->Flags.Get(OBJECT::eOF_LIVE) == false)
			{
				break;
			}

			mu_int32 iRandom = GetLargeRand(10);
			mu_int32 iCnt = 0;
			switch (iRandom)
			{
			case 0:
				iCnt = 0;
				break;
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
				iCnt = 1;
				break;
			case 8:
			case 9:
				iCnt = 2;
				break;
			}

			for (mu_int32 i = 0; i<iCnt; ++i)
			{
				VectorCopy(pSourceObj->Position, vSourcePos);
				vSourcePos[0] += ((mu_float)((GetLargeRand(80) - 40)));
				vSourcePos[1] += ((mu_float)((GetLargeRand(60) - 30)));
				vSourcePos[2] += (80.0f + ((mu_float)((GetLargeRand(180) - 100))));
				Vector(1.0f, 0.2f, 0.2f, vLight);
				CreateParticle(ThreadIndex, BITMAP_LIGHT + 2, vSourcePos, pSourceObj->Angle, vLight, 7, 1.8f);
			}

			if (o->LifeTime <= 60)
			{
				for (mu_int32 i = 0; i<iCnt; ++i)
				{
					VectorCopy(pTargetObj->Position, vTargetPos);
					vTargetPos[0] += ((mu_float)((GetLargeRand(80) - 40)));
					vTargetPos[1] += ((mu_float)((GetLargeRand(60) - 30)));
					vTargetPos[2] += (80.0f + ((mu_float)((GetLargeRand(180) - 100))));
					Vector(1.0f, 0.2f, 0.2f, vLight);
					CreateParticle(ThreadIndex, BITMAP_LIGHT + 2, vTargetPos, pTargetObj->Angle, vLight, 7, 1.8f);
				}
			}

			if (o->LifeTime <= 70 && o->LifeTime >= 66)
			{
				mu_int32 iRandom = GetLargeRand(10);
				mu_int32 iCnt2 = 0;
				switch (iRandom)
				{
				case 0:
					iCnt2 = 0;
					break;
				case 1:
				case 2:
				case 3:
					iCnt2 = 1;
					break;
				case 4:
				case 5:
					iCnt2 = 2;
				case 6:
				case 7:
				case 8:
					iCnt2 = 3;
					break;
				case 9:
					iCnt2 = 4;
					break;
				}

				EMatrix4 fMatrix;
				EVector3 vDir;

				for (mu_int32 i = 0; i<iCnt2; ++i)
				{
					VectorCopy(pSourceObj->Position, pSourceModel->BodyOrigin(ThreadIndex));
					pSourceModel->TransformPosition(ThreadIndex, pSourceObj->BoneTransform[pSourceModel->GetBoneIndex(18)], vRelativePos, vSourcePos, true);

					AngleMatrix(pSourceObj->Angle, fMatrix);
					vDir[0] = fMatrix[0][1];
					vDir[1] = fMatrix[1][1];
					vDir[2] = fMatrix[2][1];

					VectorNormalize(vDir);

					vSourcePos[0] = vSourcePos[0] + ((vDir[0])*100.0f) + (mu_float)((GetLargeRand(10)) * 5);
					vSourcePos[1] = vSourcePos[1] + ((vDir[1])*100.0f) + (mu_float)((GetLargeRand(10)) * 5);
					vSourcePos[2] += (mu_float)((GetLargeRand(10)) * 5);

					VectorCopy(pTargetObj->Position, vTargetPos);
					vTargetPos[2] += 100.0f + (mu_float)(((GetLargeRand(10) - 5)) * 4);

					Vector(0.8f, 0.1f, 0.2f, vLight);
					CreateJoint(ThreadIndex, BITMAP_DRAIN_LIFE_GHOST, vSourcePos, vTargetPos, o->Angle, 0, pSourceObj, 40.0f, 0, 0, 0, -1, &vLight);
				}
			}

			if (o->LifeTime == 64)
			{
				VectorCopy(pSourceObj->Position, pSourceModel->BodyOrigin(ThreadIndex));
				pSourceModel->TransformPosition(ThreadIndex, pSourceObj->BoneTransform[pSourceModel->GetBoneIndex(18)], vRelativePos, vSourcePos, true);

				iNumBones = pTargetModel->_BoneNonDummyEnd;

				for (mu_int32 i = 0; i<iNumBones; ++i)
				{
					VectorCopy(pTargetObj->Position, pTargetModel->BodyOrigin(ThreadIndex));
					pTargetModel->TransformPosition(ThreadIndex, pTargetObj->BoneTransform[i], vRelativePos, vTargetPos, true);

					Vector(0.4f, 0.4f, 0.8f, vLight);

					if (GetLargeRand(2) == 0)
					{
						VectorCopy(pSourceObj->Position, vSourcePos);
						vSourcePos[2] += 80.0f;
						Vector(1.0f, 0.0f, 0.1f, vLight);
						CreateJoint(ThreadIndex, BITMAP_JOINT_ENERGY, vTargetPos, vSourcePos, pTargetObj->Angle, 45, pSourceObj, 10.0f, -1, 0, 0, -1, &vLight);
					}
				}
			}
		}
		break;

	case MODEL_ALICE_BUFFSKILL_EFFECT:
	case MODEL_ALICE_BUFFSKILL_EFFECT2:
		{
			if (o->SubType == 0 || o->SubType == 1 || o->SubType == 2)
			{
				if (o->Owner == nullptr || o->Owner->Flags.Get(OBJECT::eOF_LIVE) == false)
				{
					o->Flags.Clear(OBJECT::eOF_LIVE);
				}

				VectorCopy(o->Owner->Position, o->Position);
				o->Position[2] += 100;
				if (o->LifeTime > 20)
				{
					o->Alpha += 0.05f;
					o->BlendMeshLight += 0.05f;
				}
				else
				{
					o->Alpha -= 0.05f;
					o->BlendMeshLight -= 0.05f;
					if (o->Alpha < 0.0f)
					{
						o->Flags.Clear(OBJECT::eOF_LIVE);
					}
				}

				if (o->Type == MODEL_ALICE_BUFFSKILL_EFFECT)
				{
					o->Angle[2] += 8.0f;
				}
				else if (o->Type == MODEL_ALICE_BUFFSKILL_EFFECT2)
				{
					o->Angle[2] -= 8.0f;
				}

				o->Scale += 0.035f;

				mu_float fRot = (FWorldTime*0.0006f) * 360.0f;
				EVector3 vLight;

				if (o->SubType == 0)
				{
					Vector(0.8f*o->Alpha, 0.1f*o->Alpha, 0.9f*o->Alpha, vLight);
				}
				else if (o->SubType == 1)
				{
					Vector(1.0f*o->Alpha, 1.0f*o->Alpha, 1.0f*o->Alpha, vLight);
				}
				else if (o->SubType == 2)
				{
					Vector(0.8f*o->Alpha, 0.5f*o->Alpha, 0.2f*o->Alpha, vLight);
				}

				if (o->SubType == 0 || o->SubType == 2)
				{
					CreateSprite(BITMAP_LIGHT, o->Position, 5.0f, vLight, o, 0.0f, 0);
					CreateSprite(BITMAP_LIGHT, o->Position, 5.0f, vLight, o, 0.0f, 0);
				}
				else if (o->SubType == 1)
				{
					CreateSprite(BITMAP_LIGHT, o->Position, 5.0f, vLight, o, 0.0f, 1);
					CreateSprite(BITMAP_LIGHT, o->Position, 5.0f, vLight, o, 0.0f, 1);
				}

				if (o->SubType == 0)
				{
					Vector(0.7f*o->Alpha, 0.6f*o->Alpha, 0.9f*o->Alpha, vLight);
				}
				else if (o->SubType == 1)
				{
					Vector(1.0f*o->Alpha, 1.0f*o->Alpha, 1.0f*o->Alpha, vLight);
				}
				else if (o->SubType == 2)
				{
					Vector(0.8f*o->Alpha, 0.5f*o->Alpha, 0.2f*o->Alpha, vLight);
				}

				if (o->SubType == 0 || o->SubType == 2)
				{
					CreateSprite(BITMAP_SHINY + 5, o->Position, 2.0f, vLight, o, fRot);
					CreateSprite(BITMAP_SHINY + 5, o->Position, 1.0f, vLight, o, -fRot);
				}
				else if (o->SubType == 1)
				{
					CreateSprite(BITMAP_SHINY + 5, o->Position, 2.0f, vLight, o, fRot, 1);
					CreateSprite(BITMAP_SHINY + 5, o->Position, 1.0f, vLight, o, -fRot, 1);
				}

				EVector3 vAngle, vPos, vWorldPos;
				EBone Matrix;
				Vector(0.0f, -200.0f, 0.0f, vPos);
				for (mu_int32 i = 0; i<3; ++i)
				{
					Vector((mu_float)(GetLargeRand(90)), 0.0f, (mu_float)(GetLargeRand(360)), vAngle);
					AngleMatrix(vAngle, Matrix);
					VectorRotate(vPos, Matrix, vWorldPos);
					VectorSubtract(o->Position, vWorldPos, vWorldPos);

					if (o->SubType == 0)
					{
						Vector(0.7f, 0.5f, 0.7f, vLight);
					}
					else if (o->SubType == 1)
					{
						Vector(1.0f, 1.0f, 1.0f, vLight);
					}
					else if (o->SubType == 2)
					{
						Vector(0.8f, 0.5f, 0.2f, vLight);
					}

					if (o->SubType == 0 || o->SubType == 2)
					{
						CreateJoint(ThreadIndex, BITMAP_JOINT_HEALING, vWorldPos, o->Position, vAngle, 15, o, 5.0f, 0, 0, 0, 0, &vLight);
					}
					else if (o->SubType == 1)
					{
						CreateJoint(ThreadIndex, BITMAP_JOINT_HEALING, vWorldPos, o->Position, vAngle, 16, o, 5.0f, 0, 0, 0, 0, &vLight);
					}
				}
			}
			else if (o->SubType == 3 || o->SubType == 4)
			{
				if (o->Owner == nullptr || o->Owner->Flags.Get(OBJECT::eOF_LIVE) == false)
				{
					o->Flags.Clear(OBJECT::eOF_LIVE);
				}
				else
				{
					o->LifeTime = 100;

					MUModel* pModel = &MODELS::Data[o->Owner->Type];
					mu_int32 iBone = GetLargeRand(pModel->_BoneNonDummyEnd);

					EVector3 vRelativePos, vWorldPos;
					Vector(0.0f, 0.0f, 0.0f, vRelativePos);

					pModel->TransformPosition(ThreadIndex, o->Owner->BoneTransform[iBone], vRelativePos, vWorldPos, false);
					VectorScale(vWorldPos, pModel->BodyScale(ThreadIndex), vWorldPos);
					VectorAdd(vWorldPos, o->Owner->Position, vWorldPos);

					if (o->SubType == 3)
					{
						CreateParticle(ThreadIndex, BITMAP_LIGHT + 2, vWorldPos, o->Angle, o->Light, 6, o->Scale);
						iBone = GetLargeRand(pModel->_BoneNonDummyEnd);
						pModel->TransformPosition(ThreadIndex, o->Owner->BoneTransform[iBone], vRelativePos, vWorldPos, false);
						VectorScale(vWorldPos, pModel->BodyScale(ThreadIndex), vWorldPos);
						VectorAdd(vWorldPos, o->Owner->Position, vWorldPos);
						CreateParticle(ThreadIndex, BITMAP_LIGHT + 2, vWorldPos, o->Angle, o->Light, 6, o->Scale);

					}
					else if (o->SubType == 4)
					{
						if (GetLargeRand(2) == 0)
						{
							vWorldPos[2] -= 20.0f;
							CreateParticle(ThreadIndex, BITMAP_TWINTAIL_WATER, vWorldPos, o->Angle, o->Light, 2);
						}
					}
				}
			}
		}
		break;

	case MODEL_LIGHTNING_SHOCK:
		{
			EVector3 vLight;

			if (o->SubType == 0)
			{
				if (o->Owner->AnimationFrame > 6.0f || o->LifeTime < 15)
				{
					Vector(0.0f, -20.0f, -75.0f - o->Velocity, o->Direction);
					o->Gravity += 0.1f;
					o->Velocity += o->Gravity;
				}

				OBJECT * pObject = o;
				OBJECT * pOwner = pObject->Owner;
				MUModel * pOwnerModel = &MODELS::Data[pOwner->Type];

				mu_float fRot = (FWorldTime*0.0006f) * 360.0f;
				mu_float fScale = 0.8f;

				Vector(1.0f, 0.4f, 0.4f, vLight);
				CreateSprite(BITMAP_SHINY + 1, o->Position, 4.0f * fScale, vLight, o, fRot);
				CreateSprite(BITMAP_SHINY + 1, o->Position, 3.0f * fScale, vLight, o, -fRot);
				
				Vector(1.0f, 0.2f, 0.2f, vLight);
				CreateSprite(BITMAP_MAGIC, o->Position, 1.0f * fScale, vLight, o, fRot);
				CreateSprite(BITMAP_MAGIC, o->Position, 0.5f * fScale, vLight, o, -fRot);
				
				Vector(1.0f, 0.4f, 0.4f, vLight);
				CreateSprite(BITMAP_PIN_LIGHT, o->Position, 2.0f * fScale, vLight, o, (mu_float)(GetLargeRand(360)));
				CreateSprite(BITMAP_PIN_LIGHT, o->Position, 2.0f * fScale, vLight, o, (mu_float)(GetLargeRand(360)));

				Vector(1.0f, 0.3f, 0.3f, vLight);
				CreateParticle(ThreadIndex, BITMAP_MAGIC, o->Position, o->Angle, vLight, 0, 1.0f * fScale);

				EVector3 vPos, vLightFlare, vRelative;
				Vector(1.0f, 0.7f, 0.4f, vLight);
				Vector(1.0f, 0.2f, 0.1f, vLightFlare);
				for (mu_int32 i = 0; i < 11; ++i)
				{
					fScale = (mu_float)(GetLargeRand(80) + 32)*0.01f*1.0f;
					Vector(o->Position[0] + (GetLargeRand(70) - 35)*1.0f, o->Position[1] + (GetLargeRand(70) - 35)*1.0f,
						o->Position[2] + (GetLargeRand(70) - 35)*1.0f, vPos);
					CreateSprite(BITMAP_LIGHT, vPos, 2.2f, vLightFlare, pObject);
					if (GetLargeRand(3) > 0) continue;
					CreateParticle(ThreadIndex, BITMAP_LIGHTNING_MEGA1 + GetLargeRand(3), vPos, pObject->Angle, vLight, 0, fScale);
				}

				Vector(1.0f, 0.5f, 0.4f, vLight);
				for (mu_int32 i = 0; i < 2; ++i)
				{
					fScale = (mu_float)(GetLargeRand(60) + 22)*0.01f*1.0f;
					mu_int32 iBone = GetLargeRand(41);
					Vector((GetLargeRand(30) - 15)*1.0f, (GetLargeRand(30) - 15)*1.0f, (GetLargeRand(30) - 15)*1.0f, vRelative);
					pOwnerModel->TransformByObjectBone(ThreadIndex, vPos, pOwner, iBone, &vRelative);
					CreateParticle(ThreadIndex, BITMAP_LIGHTNING_MEGA1 + GetLargeRand(3), vPos, pObject->Angle, vLight, 0, fScale);
				}

				mu_float Height = RequestTerrainHeight(o->Position[0], o->Position[1]);
				if (o->Position[2] < Height)
				{
					CreateEffect(ThreadIndex, MODEL_LIGHTNING_SHOCK, o->Position, o->Angle, o->Light, 1, o);
					o->Flags.Clear(OBJECT::eOF_LIVE);
				}
			}
			else if (o->SubType == 1)
			{
				OBJECT * pObject = o;
				OBJECT * pOwner = pObject->Owner;

				mu_float fScale = 0.8f;
				EVector3 vPos, vLightFlare;
				Vector(1.0f, 0.7f, 0.4f, vLight);
				Vector(1.0f, 0.2f, 0.1f, vLightFlare);
				for (mu_int32 i = 0; i < 11; ++i)
				{
					fScale = (mu_float)(GetLargeRand(80) + 32)*0.01f*1.0f;
					Vector(o->Position[0] + (GetLargeRand(70) - 35)*1.0f, o->Position[1] + (GetLargeRand(70) - 35)*1.0f,
						o->Position[2] + (GetLargeRand(70) - 35)*1.0f, vPos);
					CreateParticle(ThreadIndex, BITMAP_LIGHTNING_MEGA1 + GetLargeRand(3), vPos, pObject->Angle, vLight, 0, fScale);
				}

				EBone Matrix;
				EVector3 vAngle, vDirection, vPosition;
				mu_float fAngle;
				Vector(1.0f, 0.0f, 0.0f, vLight);
				for (mu_int32 i = 0; i < 6; ++i)
				{
					fScale = (mu_float)(GetLargeRand(60) + 22)*0.01f*1.0f;
					Vector(0.0f, GetLargeRand(400), 0.0f, vDirection);
					fAngle = o->Angle[2] + GetLargeRand(360);
					Vector(0.0f, 0.0f, fAngle, vAngle);
					AngleMatrix(vAngle, Matrix);
					VectorRotate(vDirection, Matrix, vPosition);
					VectorAdd(vPosition, o->Position, vPosition);
					vPosition[2] = RequestTerrainHeight(vPosition[0], vPosition[1]) + 20;

					CreateParticle(ThreadIndex, BITMAP_LIGHTNING_MEGA1 + GetLargeRand(3), vPosition, pObject->Angle, vLight, 0, fScale);
				}

				VectorCopy(o->Position, vPosition);
				vPosition[2] = RequestTerrainHeight(vPosition[0], vPosition[1]) + 10;

				Vector(1.0f, 0.0f, 0.0f, vLight);
				for (mu_int32 i = 0; i < 2; ++i)
				{
					CreateParticle(ThreadIndex, BITMAP_SMOKE, vPosition, o->Angle, vLight, 58);
				}

				if (GetLargeRand(2) == 0)
				{
					Vector(1.0f, 0.0f, 0.0f, vLight);
					CreateParticle(ThreadIndex, BITMAP_SMOKE, vPosition, o->Angle, vLight, 54, 2.8f);
				}

				CreateEffect(ThreadIndex, MODEL_STONE1 + GetLargeRand(2), vPosition, o->Angle, vLight, 13, o);
			}
			else if (o->SubType == 2)
			{
				if (o->Owner != nullptr && o->Owner->BoneTransform != nullptr)
				{
					MUModel *pTargetModel = &MODELS::Data[o->Owner->Type];
					mu_int32 iNumBones = pTargetModel->_BoneNonDummyEnd;
					mu_float fRandom;
					EVector3 vLight, vRelativePos, vPos, vAngle;
					Vector(0.0f, 0.0f, 0.0f, vRelativePos);
					for (mu_int32 i = 0; i<iNumBones; ++i)
					{
						VectorCopy(o->Owner->Position, pTargetModel->BodyOrigin(ThreadIndex));
						pTargetModel->TransformPosition(ThreadIndex, o->Owner->BoneTransform[i], vRelativePos, vPos, true);

						Vector(0.8f, 0.0f, 0.0f, vLight);
						if (GetLargeRand(60) == 0)
						{
							fRandom = 5.0f + ((mu_float)(GetLargeRand(20) - 10)*0.1f);
							CreateParticle(ThreadIndex, BITMAP_LIGHT, vPos, vAngle, vLight, 5, fRandom);
						}
						Vector(1.0f, 0.8f, 0.4f, vLight);
						if (GetLargeRand(5) == 0)
						{
							fRandom = (mu_float)(GetLargeRand(70) + 22)*0.01f*1.0f;
							CreateParticle(ThreadIndex, BITMAP_LIGHTNING_MEGA1 + GetLargeRand(3), vPos, vAngle, vLight, 0, fRandom);
						}
					}

					Vector(1.0f, 0.0f, 0.0f, vLight);
					CreateParticle(ThreadIndex, BITMAP_SMOKE, o->Position, o->Angle, vLight, 58);

					if (GetLargeRand(4) == 0)
					{
						Vector(1.0f, 0.0f, 0.0f, vLight);
						CreateParticle(ThreadIndex, BITMAP_SMOKE, o->Position, o->Angle, vLight, 54, 2.8f);
					}
				}
			}
		}
		break;

	case BITMAP_LIGHTNING + 1:
		if (o->SubType == 0)
		{
			Luminosity = o->LifeTime*0.2f;
			Vector(Luminosity*0.2f, Luminosity*0.5f, Luminosity*1.0f, Light);
			AddTerrainLight(o->Position[0], o->Position[1], Light, 2, PrimaryTerrainLight);
		}
		else if (o->SubType == 1)
		{
			o->Alpha += 0.01f;
			if (o->Alpha >= 1.0f)
			{
				o->Alpha = 1.0f;
			}
		}
		break;
	case BITMAP_LIGHT:
		if (o->SubType == 0)
		{
			VectorAdd(o->Position, o->Direction, o->Position);
			o->Direction[2] -= .01f;
			CreateParticle(ThreadIndex, BITMAP_LIGHT, o->Position, o->Angle, o->Light, 1, o->Scale);
			if (o->Direction[2] < -2.0f)
			{
				o->LifeTime = 0;
			}
		}
		else if (o->SubType == 1)
		{
			if (o->Owner != nullptr && o->Owner->Flags.Get(OBJECT::eOF_LIVE) == true && g_isCharacterBuff(o->Owner, eBuff_HpRecovery))
				o->LifeTime = 10;
			else
				o->LifeTime = 0;

			if (g_isCharacterBuff(o->Owner, eBuff_Cloaking)) break;

			Vector(1.0f, 0.5f, 0.1f, Light);
			Vector(0.0f, 0.0f, 0.0f, Angle);

			Index = GetLargeRand(7);
			Luminosity = g_byUpperBoneLocation[Index];
			CreateParticle(ThreadIndex, BITMAP_LIGHT, o->Position, Angle, Light, 4, Luminosity, o->Owner);

			Luminosity = g_byUpperBoneLocation[6 - Index];
			CreateParticle(ThreadIndex, BITMAP_LIGHT, o->Position, Angle, Light, 4, Luminosity, o->Owner);
		}
		else if (o->SubType == 2)
		{
			if (o->Owner != nullptr && o->Owner->Flags.Get(OBJECT::eOF_LIVE) == true && g_isCharacterBuff(o->Owner, eBuff_AddAG))
				o->LifeTime = 10;
			else
				o->LifeTime = 0;

			if (g_isCharacterBuff(o->Owner, eBuff_Cloaking)) break;

			++o->Velocity;

			if (((mu_int32)(o->Velocity) % 50) == 0)
			{
				if (o->Owner != nullptr)
				{
					EVector3 Position;
					o->Angle[2] += 50.0f;
					VectorCopy(o->Owner->Position, Position);

					o->Position[0] = o->Owner->Position[0] + EMath::Sin(o->Angle[2] * 0.1f)*80.0f;
					o->Position[1] = o->Owner->Position[1] + EMath::Cos(o->Angle[2] * 0.1f)*80.0f;
					o->Position[2] = o->Owner->Position[2] + 50;

					CreateJoint(ThreadIndex, BITMAP_JOINT_HEALING, o->Position, o->Position, o->Angle, 9, o->Owner, 15.0f);
				}
			}
		}
		else if (o->SubType == 3)
		{
			++o->Velocity;

			if ((o->LifeTime % 15) == 0)
			{
				if (o->Owner != nullptr)
				{
					EVector3 Position;
					o->Angle[2] += 180;
					VectorCopy(o->Owner->Position, Position);

					o->Position[0] = o->Owner->Position[0] + EMath::Sin(o->Angle[2] * 0.1f)*50.0f;
					o->Position[1] = o->Owner->Position[1] + EMath::Cos(o->Angle[2] * 0.1f)*50.0f;
					o->Position[2] = o->Owner->Position[2] + 20;

					CreateJoint(ThreadIndex, BITMAP_JOINT_HEALING, o->Position, o->Position, o->Angle, 9, o->Owner, 10.0f);
				}
			}
		}
		break;
	case MODEL_SKILL_BLAST:
		Height = RequestTerrainHeight(o->Position[0], o->Position[1]);
		if (o->Position[2] < Height)
		{
			o->Position[2] = Height;
			Vector(0.0f, 0.0f, 0.0f, o->Direction);
			EVector3 Position;
			Vector(o->Position[0], o->Position[1], o->Position[2] + 80.0f, Position);
			for (mu_int32 j = 0; j<6; ++j)
			{
				CreateEffect(ThreadIndex, MODEL_STONE1 + GetLargeRand(2), o->Position, o->Angle, o->Light);
			}
			CreateParticle(ThreadIndex, BITMAP_SHINY + 4, Position, o->Angle, Light);
			CreateParticle(ThreadIndex, BITMAP_EXPLOTION, Position, o->Angle, Light);
			o->Flags.Clear(OBJECT::eOF_LIVE);
		}
		VectorCopy(o->Position, o->EyeLeft);
		Vector(Luminosity*0.2f, Luminosity*0.4f, Luminosity*1.0f, Light);
		AddTerrainLight(o->Position[0], o->Position[1], Light, 2, PrimaryTerrainLight);
		break;
	case MODEL_WAVE:
		if (o->Scale>2.0f)
		{
			o->Scale += 0.1f;
			o->Position[0] -= 1.0f;
			o->Position[2] -= 1.5f;
			o->BlendMeshLight = o->LifeTime / 30.0f;
		}
		else
		{
			o->Scale += 1.2f;
			o->Position[0] -= 1.2f;
			o->Position[2] -= 1.8f;
		}

		Vector(-Luminosity*0.5f, -Luminosity*0.5f, -Luminosity*0.5f, Light);
		AddTerrainLight(o->Position[0], o->Position[1], Light, 5, PrimaryTerrainLight);
		break;
	case MODEL_TAIL:
		o->Position[2] -= o->Gravity;
		o->Gravity += 60.0f;
		o->BlendMeshLight = o->LifeTime / 20.0f;
		break;
	case MODEL_WAVE_FORCE:
		o->BlendMeshLight = o->LifeTime / 20.0f;
		Vector(-Luminosity*0.5f, -Luminosity*0.5f, -Luminosity*0.5f, Light);
		AddTerrainLight(o->Position[0], o->Position[1], Light, 5, PrimaryTerrainLight);
		break;
	case MODEL_SKILL_INFERNO:
		if (o->SubType == 2)
		{
			o->Scale += 0.04f;
		}
		else if (o->SubType == 8)
		{
			o->Scale += 0.04f;
			if (o->LifeTime < 10)
			{
				o->Light[0] *= 0.8f;
				o->Light[1] *= 0.8f;
				o->Light[2] *= 0.8f;
			}
		}
		else if (o->SubType == 3)
		{
			VectorCopy(o->Owner->Position, o->Position);
			o->Position[0] = o->Owner->Owner->Position[0] + 150.0f;
		}
		else if (o->SubType == 4)
		{
			VectorCopy(o->Owner->Position, o->Position);

			o->Scale += 0.003f;
			o->Gravity += 0.8f*o->Scale*30.0f;
			o->Position[2] += o->Gravity;
		}
		else if (o->SubType == 5)
		{
			o->Position[2] += 2.0f;
			o->Angle[2] += 20.0f;
			o->BlendMeshLight = o->LifeTime / 20.0f;
			Vector(Luminosity*0.1f, Luminosity*0.3f, Luminosity*0.8f, Light);
			AddTerrainLight(o->Position[0], o->Position[1], Light, 5, PrimaryTerrainLight);
		}
		else if (o->SubType == 6)
		{
			o->Scale += 0.01f;
			o->BlendMeshLight = o->LifeTime / 5.0f*0.1f;
			Vector(Luminosity*0.8f, Luminosity*0.3f, Luminosity*0.1f, Light);
			AddTerrainLight(o->Position[0], o->Position[1], Light, 2, PrimaryTerrainLight);
		}

		if (o->SubType<4)
		{
			o->BlendMeshLight = o->LifeTime / 20.0f;
			if (o->SubType != 2)
			{
				Vector(-Luminosity*0.5f, -Luminosity*0.5f, -Luminosity*0.5f, Light);
				AddTerrainLight(o->Position[0], o->Position[1], Light, 5, PrimaryTerrainLight);
			}
		}
		else if (o->SubType == 8)
		{
			o->BlendMeshLight = o->LifeTime / 20.0f;
		}
		else if (o->SubType == 9)
		{
			o->BlendMeshLight = o->LifeTime / 80.0f;
			o->Position[2] += o->Gravity;
			o->Light[0] /= 1.04f;
			o->Light[2] /= 1.04f;
			o->Alpha -= 0.01f;
		}
		else if (o->SubType == 10)
		{
			o->Scale += 0.04f;
			MUModel *b = &MODELS::Data[o->Type];
			VectorCopy(o->Light, b->BodyLight(ThreadIndex));
		}
		break;
	case MODEL_MAGIC_CIRCLE1:
		if (o->SubType == 2)
		{
			o->Scale += 0.015f;
			VectorCopy(o->Owner->Position, o->Position);
			Vector(0.1f, 0.0f, 0.0f, o->Light);
		}
		else if (o->SubType == 1)
		{
			o->Scale += 0.01f;
			Vector(Luminosity*1.0f, Luminosity*0.0f, Luminosity*0.0f, Light);
			AddTerrainLight(o->Position[0], o->Position[1], Light, 3, PrimaryTerrainLight);
		}
		else
		{
			VectorCopy(o->Owner->Position, o->Position);
			o->Scale += 0.01f;
		}
		break;
	case MODEL_BIG_METEO1:
	case MODEL_BIG_METEO2:
	case MODEL_BIG_METEO3:
		Index = TERRAIN_INDEX_REPEAT((mu_int32)o->Position[0] / 100, (mu_int32)o->Position[1] / 100);
		if ((TerrainWall[Index] & TW_NOGROUND) != TW_NOGROUND)
		{
			Height = RequestTerrainHeight(o->Position[0], o->Position[1]);
			if (o->Position[2] < Height)
			{
				o->Position[2] = Height;
				o->Flags.Clear(OBJECT::eOF_LIVE);
				EarthQuake.Store((mu_float)(GetLargeRand(8) - 8)*0.1f);
				for (mu_int32 j = 0; j<5; ++j)
					CreateEffect(ThreadIndex, MODEL_METEO1 + GetLargeRand(2), o->Position, o->Angle, o->Light, 0);
			}
		}
		VectorAdd(o->Position, o->Direction, o->Position);
		o->Angle[0] += 10.0f / o->Scale;
		CreateParticle(ThreadIndex, BITMAP_SMOKE, o->Position, o->Angle, o->Light, 3);
		break;
	case MODEL_PROTECT:
		o->BlendMeshLight = 1.3f;

		VectorCopy(o->Owner->Position, o->Position);
		o->Angle[2] += 10.0f;
		if (o->Owner->Flags.Get(OBJECT::eOF_LIVE | OBJECT::eOF_VISIBLE) == false)
			o->Flags.Clear(OBJECT::eOF_LIVE);
		break;
	case MODEL_POISON:
		o->BlendMeshLight = o->LifeTime*0.1f;
		o->Alpha = o->LifeTime*0.1f;
		Vector(Luminosity*0.3f, Luminosity*1.0f, Luminosity*0.6f, Light);
		AddTerrainLight(o->Position[0], o->Position[1], Light, 2, PrimaryTerrainLight);
		break;
	case MODEL_SAW:
		o->Angle[2] -= 30.0f;
		break;
	case MODEL_LASER:
		if (o->SubType != 0 && o->SubType != 3)
		{
			o->Direction[1] -= o->Velocity;
			o->Velocity += 1.0f;
		}
		break;
	case MODEL_MAGIC1:
		VectorCopy(o->Owner->Position, o->Position);
		o->Position[2] += 100.0f;
		o->Angle[1] += 20.0f;
		o->BlendMeshLight = o->LifeTime*0.1f;
		break;
	case MODEL_SKILL_WHEEL1:
		{
			if (o->LifeTime == 5)
			{
				if (g_CurrentScene != LOG_IN_SCENE)
					PlayBackground(SOUND_SKILL_SWORD4);
			}

			if (o->LifeTime <= 5)
			{
				CreateEffect(ThreadIndex, MODEL_SKILL_WHEEL2, o->Position, o->Angle, o->Light, 4 - o->LifeTime, o->Owner, o->PKKey, o->Skill, o->Kind);
			}
		}
		break;
	case MODEL_SKILL_WHEEL2:
		switch (o->SubType)
		{
		case 1:o->Alpha = 0.6f; break;
		case 2:o->Alpha = 0.5f; break;
		case 3:o->Alpha = 0.4f; break;
		case 4:o->Alpha = 0.3f; break;
		}
		if (o->Owner->Weapon >= MODEL_SPEAR - MODEL_SWORD && o->Owner->Weapon<MODEL_SPEAR - MODEL_SWORD + MAX_ITEM_INDEX)
		{
			Vector(0.0f, -180.0f, 0.0f, p);
		}
		else
		{
			Vector(0.0f, -150.0f, 0.0f, p);
		}
		AngleMatrix(o->Angle, Matrix);
		VectorRotate(p, Matrix, Position);
		VectorAdd(Position, o->Owner->Position, o->Position);
		o->Angle[2] -= 18;
		CreateParticle(ThreadIndex, BITMAP_SMOKE, o->Position, o->Angle, o->Light, 3);
		Vector(Luminosity*0.3f, Luminosity*0.3f, Luminosity*0.3f, Light);
		AddTerrainLight(o->Position[0], o->Position[1], Light, 3, PrimaryTerrainLight);
		Vector(1.0f, 1.0f, 1.0f, Light);

		Height = 20.0f;
		if (InHellas())
		{
			Height = 60.0f;
			Vector((mu_float)(GetLargeRand(60) + 60 - 90), 0.0f, (mu_float)(GetLargeRand(30) + 90), Angle);
			VectorAdd(Angle, o->Angle, Angle);
			VectorCopy(o->Position, Position);
			Position[0] += GetLargeRand(20) - 10;
			Position[1] += GetLargeRand(20) - 10;
			Position[2] += 120.0f;
			Vector(0.5f, 0.5f, 0.5f, Light);
			CreateParticle(ThreadIndex, BITMAP_WATERFALL_5, Position, Angle, Light, 3);
		}
		else
		{
			for (mu_int32 j = 0; j<4; ++j)
			{
				Vector((mu_float)(GetLargeRand(60) + 60 - 90), 0.0f, (mu_float)(GetLargeRand(30) + 90), Angle);
				VectorAdd(Angle, o->Angle, Angle);
				VectorCopy(o->Position, Position);
				Position[0] += GetLargeRand(20) - 10;
				Position[1] += GetLargeRand(20) - 10;
				CreateJoint(ThreadIndex, BITMAP_JOINT_SPARK, Position, Position, Angle);
				if (j == 0) CreateParticle(ThreadIndex, BITMAP_SPARK, Position, Angle, Light);
			}
		}
		VectorCopy(o->Position, Position);
		Position[2] += Height;
		Vector(1.0f, 0.8f, 0.6f, Light);
		CreateSprite(BITMAP_LIGHT, Position, 2.0f, Light, o);

		if (InHellas() && (o->LifeTime % 4) == 0)
		{
			mu_int32 PositionX = (mu_int32)(o->Position[0] * g_TerrainScaleRev);
			mu_int32 PositionY = (mu_int32)(o->Position[1] * g_TerrainScaleRev);
			AddWaterWave(PositionX, PositionY, 2, -150);
		}
		break;
	case MODEL_SKILL_FISSURE:
		{
			if (o->LifeTime == 8)
			{
				for (mu_int32 i = 0; i < 16; ++i)
				{
					Angle[0] = -10.0f;
					Angle[1] = 0.0f;
					Angle[2] = i*10.0f;
					EVector3 Position;
					VectorCopy(o->Position, Position);
					Position[2] += 100.0f;

					Position[0] += GetLargeRand(600) - 200;
					Position[1] += GetLargeRand(600) - 400;
					CreateJoint(ThreadIndex, BITMAP_FLARE, Position, Position, Angle, 24, nullptr, 90);
				}
			}
			else if (o->LifeTime == 2)
			{
				Vector(0.0f, 0.0f, GetLargeRand(360), Angle);
				CreateEffect(ThreadIndex, MODEL_FISSURE, o->Position, Angle, o->Light, 0, o);
				CreateEffect(ThreadIndex, MODEL_FISSURE_LIGHT, o->Position, Angle, o->Light, 0, o);
				o->Flags.Clear(OBJECT::eOF_LIVE);
			}
		}
		break;
	case MODEL_FISSURE:
		{
			double fLevel = EMath::Sin(Q_PI * double(o->LifeTime) / 120.0f) * -0.5f;
			EVector3 Light;
			Vector(fLevel, fLevel, fLevel + 0.1f, Light);
			AddTerrainLight(o->Position[0], o->Position[1], Light, 10, PrimaryTerrainLight);

			EarthQuake.Store((mu_float)(GetLargeRand(4) - 2)*0.1f);

			if (GetLargeRand(2) == 0) {
				Vector(0.0f, (mu_float)(GetLargeRand(150)) + 300.0f, 0.0f, p);
				Vector(0.0f, 0.0f, (mu_float)(GetLargeRand(360)), Angle);
				AngleMatrix(Angle, Matrix);
				VectorRotate(p, Matrix, Position);
				VectorAdd(Position, o->Position, Position);

				CreateEffect(ThreadIndex, MODEL_STONE1 + GetLargeRand(2), Position, o->Angle, o->Light, 0);
			}

			MUModel* pModel = &MODELS::Data[o->Type];
			pModel->Animation(ThreadIndex, ThreadInfo.BoneTransform, o->AnimationFrame, o->PriorAnimationFrame, o->PriorAction, o->Angle, o->HeadAngle, false, false);
			Vector(1.0f, 1.0f, 1.0f, Light);
			for (mu_int32 i = 0; i<pModel->_BoneNonDummyEnd; ++i) {
				if (GetLargeRand(12) == 0)
				{
					Vector(0.0f, 0.0f, 0.0f, p);
					pModel->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[i], p, Position, true);
					CreateParticle(ThreadIndex, BITMAP_TRUE_FIRE, Position, o->Angle, Light, 3, 4.0f, o);
					CreateParticle(ThreadIndex, BITMAP_SMOKE, Position, o->Angle, Light, 21, 1.0f);
				}
			}
		}
		break;
	case MODEL_SKILL_FURY_STRIKE:
		{
			EVector3 p, Position, Pos[5];
			EVector3 Angle;
			mu_int16  scale = 150;
			EMatrix4  Matrix;
			mu_float ang[5];
			mu_int32    TargetX, TargetY;

			Vector(0.0f, 0.0f, 0.0f, Angle);
			Vector(0.0f, 0.0f, 0.0f, p);

			if (o->LifeTime == 11)
			{
				EVector3  light;
				Vector(1.0f, 1.0f, 1.0f, light);
				Vector(0.0f, 0.0f, 0.0f, Angle);

				if (o->Kind != 3)
				{
					Vector(-25.0f, -80.0f, 0.0f, p);
					AngleMatrix(o->Owner->Angle, Matrix);
					VectorRotate(p, Matrix, Position);
					VectorAdd(Position, o->Position, o->StartPosition);
				}

				mu_float AddHeight = 25.0f;

				if (InHellas() == true)
				{
					if (o->Kind == 0 || o->Kind == 2)
					{
						mu_int32 PositionX = (mu_int32)(o->StartPosition[0] * g_TerrainScaleRev);
						mu_int32 PositionY = (mu_int32)(o->StartPosition[1] * g_TerrainScaleRev);
						AddWaterWave(PositionX, PositionY, 2, -1000);
					}
					else
					{
						AddHeight = 100.0f;
					}
				}

				o->StartPosition[2] = RequestTerrainHeight(o->StartPosition[0], o->StartPosition[1]) + AddHeight;
				AddHeight = 3.0f;

				CreateParticle(ThreadIndex, BITMAP_EXPLOTION, o->StartPosition, Angle, light, 0, 0.5f);

				if (o->Kind == 0)
				{
					for (mu_int32 j = 0; j < 8; ++j)
					{
						Vector((mu_float)(GetLargeRand(60) - 60.0f), 0.0f, (mu_float)(GetLargeRand(30) + 90), Angle);
						VectorAdd(Angle, o->Angle, Angle);
						VectorCopy(o->StartPosition, Position);
						Position[0] += GetLargeRand(20) - 10;
						Position[1] += GetLargeRand(20) - 10;
						CreateJoint(ThreadIndex, BITMAP_JOINT_SPARK, Position, Position, Angle);
						if (j == 0) CreateParticle(ThreadIndex, BITMAP_SPARK, Position, Angle, Light);
					}
				}
				Vector(0.0f, 0.0f, 0.0f, Angle);

				if (o->Kind == 0)
					CreateEffect(ThreadIndex, MODEL_WAVE, o->StartPosition, Angle, o->Light);

				o->StartPosition[2] -= 27.0f;

				if (o->Owner != nullptr)
				{
					if (o->Owner->Type != MODEL_MONSTER01 + 95)
					{
						CreateEffect(ThreadIndex, MODEL_SKILL_FURY_STRIKE + 3, o->StartPosition, Angle, o->Light, 0, o, scale);
						CreateEffect(ThreadIndex, MODEL_SKILL_FURY_STRIKE + 1, o->StartPosition, Angle, o->Light, 0, o, scale);
						CreateEffect(ThreadIndex, MODEL_SKILL_FURY_STRIKE + 2, o->StartPosition, Angle, o->Light, 0, o, scale);
					}
				}

				mu_int32 subType = o->SubType == 100 ? 0 : o->SubType;

				for (mu_int32 i = 0; i < 5; ++i)
				{
					Vector(0.0f, (mu_float)(GetLargeRand(150) + 100), 0.0f, p);
					Vector(0.0f, 0.0f, (mu_float)(subType + (i*72.0f)), Angle);
					AngleMatrix(Angle, Matrix);
					VectorRotate(p, Matrix, Position);
					VectorAdd(Position, o->StartPosition, Position);

					Position[2] += 3;
					scale = GetLargeRand(50) + 40;

					Vector(0.0f, 0.0f, 45 + (mu_float)(GetLargeRand(30) - 15), Angle);

					TargetX = (mu_int32)(Position[0] * g_TerrainScaleRev);
					TargetY = (mu_int32)(Position[1] * g_TerrainScaleRev);

					mu_uint16 wall = TerrainWall[TERRAIN_INDEX(TargetX, TargetY)];

					if ((wall&TW_NOMOVE) != TW_NOMOVE && (wall&TW_NOGROUND) != TW_NOGROUND && (wall&TW_WATER) != TW_WATER)
					{
						if (InHellas() == true)
						{
							AddHeight = 100.0f;
						}
						Position[2] = RequestTerrainHeight(Position[0], Position[1]) + AddHeight;

						CreateEffect(ThreadIndex, MODEL_SKILL_FURY_STRIKE + 4, Position, Angle, o->Light, 0, o->Owner, scale);
						CreateEffect(ThreadIndex, MODEL_SKILL_FURY_STRIKE + 5, Position, Angle, o->Light, 0, o->Owner, scale);
					}
				}
			}
			else if (o->LifeTime == 10)
			{
				for (mu_int32 j = 0; j < 5; ++j)
				{
					VectorCopy(o->StartPosition, Pos[j]);
					ang[j] = 0.0f;
				}

				mu_int32 count = 0;
				mu_int32 random;
				for (mu_int32 j = 0; j < 4; ++j)
				{
					Vector(0.0f, GetLargeRand(15) + 85.0f, 0.0f, p);

					if (j >= 3) count = GetLargeRand(10000000);

					for (mu_int32 i = 0; i < 5; ++i)
					{
						if ((count % 2) == 0) random = GetLargeRand(30) + 50;
						else                random = -(GetLargeRand(30) + 50);

						ang[i] += random;
						Angle[2] = ang[i] + (i*(GetLargeRand(10) + 62));

						AngleMatrix(Angle, Matrix);
						VectorRotate(p, Matrix, Position);
						VectorAdd(Position, Pos[i], Pos[i]);

						TargetX = (mu_int32)(Pos[i][0] * g_TerrainScaleRev);
						TargetY = (mu_int32)(Pos[i][1] * g_TerrainScaleRev);

						mu_uint16 wall = TerrainWall[TERRAIN_INDEX(TargetX, TargetY)];

						if ((wall&TW_NOMOVE) != TW_NOMOVE || (wall&TW_NOGROUND) != TW_NOGROUND || (wall&TW_WATER) != TW_WATER)
						{
							if (InHellas() == true)
								Pos[i][2] = RequestTerrainHeight(Pos[i][0], Pos[i][1]) + 100.0f;
							else
								Pos[i][2] = RequestTerrainHeight(Pos[i][0], Pos[i][1]) + 3;
							Angle[2] += 270.0f;

							CreateEffect(ThreadIndex, MODEL_SKILL_FURY_STRIKE + 7, Pos[i], Angle, o->Light, 0, o->Owner, 100);
							CreateEffect(ThreadIndex, MODEL_SKILL_FURY_STRIKE + 8, Pos[i], Angle, o->Light, 0, o->Owner, 100);
						}
					}

					++count;
				}
				o->LifeTime = 0;

				PlayBackground(SOUND_FURY_STRIKE3);
			}
			else
			{
				mu_float   count = (mu_float)o->LifeTime;
				mu_float	addAngle = 15.0f;
				if (o->LifeTime > 9 && o->LifeTime < 16)
				{
					count = 12.5;
					addAngle = 18.0f;
					if (o->LifeTime == 15) o->Gravity *= -1;
				}

				if (o->Kind == 0)
				{
					if (o->LifeTime == 13)
					{
						EVector3  pos;
						Vector(0.0f, 0.0f, 0.0f, Angle);
						Vector(-25.0f, -40.0f, 0.0f, p);
						AngleMatrix(o->Owner->Angle, Matrix);
						VectorRotate(p, Matrix, Position);
						VectorAdd(Position, o->Position, pos);

						EVector3  position;

						for (mu_int32 i = 0; i < 4; ++i)
						{
							position[0] = pos[0]; position[1] = pos[1];
							position[2] = pos[2] - (i * 50);
							CreateEffect(ThreadIndex, MODEL_TAIL, position, Angle, o->Light);
						}
						pos[0] += GetLargeRand(30) + 20;
						pos[2] += GetLargeRand(500) - 250;
						for (mu_int32 i = 0; i < 4; ++i)
						{
							position[0] = pos[0]; position[1] = pos[1];
							position[2] = pos[2] - (i * 30);
							CreateEffect(ThreadIndex, MODEL_TAIL, position, Angle, o->Light);
						}
						PlayBackground(SOUND_FURY_STRIKE2);
					}
				}

				mu_float   angle = (20 - count)*addAngle;

				o->Angle[0] += 80;
				o->Direction[1] = EMath::Sin((mu_float)((angle*3.141592) / 180.0f)) * 260;
				VectorCopy(o->StartPosition, o->Position);
				if (count<12.5 || count>12.5)
				{
					o->Gravity += 8.0f;
				}
				else
				{
					o->Gravity -= 8.0f;
				}

				AngleMatrix(o->HeadAngle, Matrix);
				VectorRotate(o->Direction, Matrix, Position);
				VectorAdd(Position, o->StartPosition, o->Position);

				o->Position[2] += o->Gravity;
				o->Position[2] += 200;
			}
		}
		break;
	case MODEL_SKILL_FURY_STRIKE + 1:
		o->BlendMeshLight = (o->LifeTime*0.1f) / 3.0f;
		if (o->LifeTime<10)
		{
			o->Position[2] -= 0.5f;
		}
		if (o->LifeTime>15 && (o->LifeTime % 3) == 0)
		{
			EarthQuake.Store((mu_float)(GetLargeRand(8) - 4)*0.1f);
		}
		break;
	case MODEL_SKILL_FURY_STRIKE + 3:
		o->BlendMeshLight = (o->LifeTime*0.1f) / 10.0f;
		if (o->LifeTime<13)
		{
			o->Position[2] -= 0.5f;
		}
		break;
	case MODEL_SKILL_FURY_STRIKE + 2:
		if (o->Scale>50)o->LifeTime = 0;
		if (o->LifeTime >= 10)
			o->BlendMeshLight = ((20 - o->LifeTime)*0.1f);
		else
		{
			if (o->LifeTime<5)
			{
				o->Position[2] -= 0.5f;
			}
			o->BlendMeshLight = (o->LifeTime*0.1f);

			if (o->LifeTime >= 5 && GetLargeRand(10) == 0)
			{
				EVector3 p, Position;
				EVector3 Angle;
				EBone Matrix;
				Vector(0.0f, (mu_float)(GetLargeRand(150)), 0.0f, p);
				Vector(0.0f, 0.0f, (mu_float)(GetLargeRand(360)), Angle);
				AngleMatrix(Angle, Matrix);
				VectorRotate(p, Matrix, Position);
				VectorAdd(Position, o->Position, Position);

				CreateEffect(ThreadIndex, MODEL_STONE1 + GetLargeRand(2), Position, o->Angle, o->Light, 0);
			}
		}

		o->BlendMeshTexCoordU = -(mu_float)o->LifeTime*0.01f;

		if (o->Owner != nullptr)
		{
			if (o->Owner->Type == MODEL_MONSTER01 + 95)
			{
				Vector(Luminosity*0.0f, Luminosity*0.0f, Luminosity*1.0f, Light);
			}
			else
			{
				Vector(Luminosity*1.0f, Luminosity*0.0f, Luminosity*0.0f, Light);
			}
		}
		AddTerrainLight(o->Position[0], o->Position[1], Light, 1, PrimaryTerrainLight);
		break;
	case MODEL_SKILL_FURY_STRIKE + 4:
		o->BlendMeshLight = (o->LifeTime*0.1f) / 3.0f;
		if (o->LifeTime<10)
		{
			o->Position[2] -= 0.5f;
		}
		break;
	case MODEL_SKILL_FURY_STRIKE + 6:
		o->BlendMeshLight = (o->LifeTime*0.1f) / 10.0f;
		if (o->LifeTime<13)
		{
			o->Position[2] -= 0.5f;
		}
		break;
	case MODEL_SKILL_FURY_STRIKE + 5:
		if (o->LifeTime >= 30)
			o->BlendMeshLight = ((40 - o->LifeTime)*0.1f);
		else
		{
			if (o->LifeTime<15)
			{
				o->Position[2] -= 0.5f;
			}

			o->BlendMeshLight = (o->LifeTime*0.1f);

			if (o->LifeTime >= 5 && GetLargeRand(15) == 0)
			{
				EVector3 p, Position;
				EVector3 Angle;
				EBone Matrix;
				Vector(0.0f, (mu_float)(GetLargeRand(150)), 0.0f, p);
				Vector(0.0f, 0.0f, (mu_float)(GetLargeRand(360)), Angle);
				AngleMatrix(Angle, Matrix);
				VectorRotate(p, Matrix, Position);
				VectorAdd(Position, o->Position, Position);

				CreateEffect(ThreadIndex, MODEL_STONE1 + GetLargeRand(2), Position, o->Angle, o->Light, 0);
			}
		}

		o->BlendMeshTexCoordU = -(mu_float)o->LifeTime*0.01f;

		if (o->Owner != nullptr)
		{
			if (o->Owner->Type == MODEL_MONSTER01 + 95)
			{
				Vector(Luminosity*0.0f, Luminosity*0.0f, Luminosity*1.0f, Light);
			}
			else
			{
				Vector(Luminosity*1.0f, Luminosity*0.0f, Luminosity*0.0f, Light);
			}
			AddTerrainLight(o->Position[0], o->Position[1], Light, 1, PrimaryTerrainLight);
		}
		break;
	case MODEL_SKILL_FURY_STRIKE + 7:
		o->BlendMeshLight = (o->LifeTime*0.1f) / 3.0f;
		if (o->LifeTime<10)
		{
			o->Position[2] -= 0.5f;
		}
		break;
	case MODEL_SKILL_FURY_STRIKE + 8:
		if (o->LifeTime >= 30)
			o->BlendMeshLight = ((40 - o->LifeTime)*0.1f);
		else
		{
			o->BlendMeshLight = (o->LifeTime*0.1f);
		}
		if (o->LifeTime<15)
		{
			o->Position[2] -= 0.5f;
		}

		if (o->LifeTime >= 13 && GetLargeRand(5) == 0)
		{
			EVector3 Position;

			Position[0] = o->Position[0];
			Position[1] = o->Position[1];
			Position[2] = o->Position[2] + 10;
		}

		o->BlendMeshTexCoordU = -(mu_float)o->LifeTime*0.01f;

		if (o->Owner != nullptr)
		{
			if (o->Owner->Type == MODEL_MONSTER01 + 95)
			{
				Vector(Luminosity*0.0f, Luminosity*0.0f, Luminosity*1.0f, Light);
			}
			else
			{
				Vector(Luminosity*1.0f, Luminosity*0.0f, Luminosity*0.0f, Light);
			}
			AddTerrainLight(o->Position[0], o->Position[1], Light, 1, PrimaryTerrainLight);
		}
		break;

	case MODEL_BALGAS_SKILL:
		o->BlendMeshLight = o->LifeTime*0.1f;
		Vector(Luminosity*0.3f, Luminosity*0.6f, Luminosity, Light);
		{
			MUModel *b = &MODELS::Data[o->Type];
			b->CurrentAction(ThreadIndex) = o->CurrentAction;
			b->PlayAnimation(ThreadIndex, &o->AnimationFrame, &o->PriorAnimationFrame, &o->PriorAction, o->Velocity*2.0f);
			AddTerrainLight(o->Position[0], o->Position[1], Light, 3, PrimaryTerrainLight);
		}
		break;
	case MODEL_CHANGE_UP_EFF:
		{
			EVector3  Loc;
			mu_int32 STwo = 2;
			if (GetLargeRand(STwo) == 0)
			{
				if (o->SubType == 2)
				{
					VectorCopy(o->Owner->Position, o->Position);
					o->Position[2] = RequestTerrainHeight(o->Position[0], o->Position[1]) + 22.0f;
				}

				Loc[0] = o->Position[0] + GetLargeRand(200) - 100;
				Loc[1] = o->Position[1] + GetLargeRand(200) - 100;
				Loc[2] = o->Position[2] - 200;
				if (o->SubType == 0
					|| o->SubType == 2)
				{
					CreateJoint(ThreadIndex, BITMAP_FLARE, Loc, Loc, o->Angle, 50, nullptr, 40);
				}

				if (o->LifeTime > 40)
					CreateEffect(ThreadIndex, BITMAP_MAGIC, o->Position, o->Angle, Light, 4, o, 4.0f);

			}
			if (o->LifeTime <= 1 && o->SubType == 0)
				SetPlayerStop(ThreadIndex, Hero);

			if (o->SubType == 0)
			{
				MUModel *b = &MODELS::Data[o->Type];
				b->PlayAnimation(ThreadIndex, &o->AnimationFrame, &o->PriorAnimationFrame, &o->PriorAction, o->Velocity / 3.0f);
			}
			else if (o->SubType == 2)
			{
				MUModel *b = &MODELS::Data[o->Type];
				b->PlayAnimation(ThreadIndex, &o->AnimationFrame, &o->PriorAnimationFrame, &o->PriorAction, o->Velocity / 3.0f);
			}
		}
		break;
	case MODEL_CHANGE_UP_NASA:
		{
			if (o->SubType == 0)
			{
				if (o->LifeTime == 100)
				{
					CreateEffect(ThreadIndex, MODEL_CHANGE_UP_NASA, o->Position, o->Angle, o->Light, 1, o);
				}
				else
					if (o->LifeTime == 70)
					{
						CreateEffect(ThreadIndex, MODEL_CHANGE_UP_NASA, o->Position, o->Angle, o->Light, 2, o);
					}
				if (o->LifeTime == 40)
				{
					CreateEffect(ThreadIndex, MODEL_CHANGE_UP_NASA, o->Position, o->Angle, o->Light, 3, o);
				}
			}
			if (o->SubType >= 1 && o->SubType <= 3)
			{
				o->Scale += 0.02f;
				o->BlendMeshLight = o->LifeTime*0.005f;
			}
			else
				if (o->SubType == 0)
					o->BlendMeshLight = o->LifeTime*0.01f;
		}
		break;
	case MODEL_CHANGE_UP_CYLINDER:
		{
			if (o->SubType == 2)
			{
				VectorCopy(o->Owner->Position, o->Position);
				o->Position[2] = RequestTerrainHeight(o->Position[0], o->Position[1]);
			}

			EVector3  Go;
			AngleMatrix(Angle, Matrix);
			VectorRotate(Go, Matrix, Position);
			VectorAdd(Position, o->Position, Position);

			if (o->SubType == 1)
			{
				o->BlendMeshLight = o->LifeTime*0.015f;
				o->Scale += 0.08f;
			}
		}
		break;
	case MODEL_DARK_ELF_SKILL:
		{
			EVector3  Go;
			if (o->Scale < 0.8f)
				o->Scale += 0.2f;
			else
				o->Scale -= 0.4f;
			o->BlendMeshLight = o->LifeTime*0.1f;
			o->BlendMeshTexCoordU = -(mu_float)o->LifeTime*0.2f;
			Vector(0.0f, -500.0f, 0.0f, Go);
			EVector3	 	  Angle;
			VectorCopy(o->Angle, Angle);
			Angle[2] += 7;

			AngleMatrix(Angle, Matrix);
			VectorRotate(Go, Matrix, Position);
			VectorAdd(Position, o->Position, Position);

			for (mu_int32 j = 0; j<6; ++j)
			{
				CreateParticle(ThreadIndex, BITMAP_SMOKE, Position, Angle, o->Light, 25);
			}
			Vector(Luminosity*0.3f, Luminosity*0.6f, Luminosity, Light);
			AddTerrainLight(Position[0], Position[1], Light, 3, PrimaryTerrainLight);
		}
		break;

	case MODEL_MAGIC2:
		o->BlendMeshLight = o->LifeTime*0.1f;
		o->BlendMeshTexCoordU = -(mu_float)o->LifeTime*0.2f;
		for (mu_int32 j = 0; j<4; ++j)
			CreateParticle(ThreadIndex, BITMAP_SMOKE, o->Position, o->Angle, o->Light, 3);
		Vector(Luminosity*0.3f, Luminosity*0.6f, Luminosity, Light);
		AddTerrainLight(o->Position[0], o->Position[1], Light, 3, PrimaryTerrainLight);

		if (o->SubType == 2)
		{
			o->HiddenMesh = 0;
			EVector3 Light2;
			VectorCopy(o->Angle, Angle);
			VectorCopy(o->Position, Position);
			Angle[2] += GetLargeRand(10) - 5;
			CreateParticle(ThreadIndex, BITMAP_SMOKE, o->Position, o->Angle, Light, 11, (mu_float)(GetLargeRand(32) + 80)*0.015f);

			Vector(1.0f, 1.0f, 1.0f, Light2);
			Position[2] += 50.0f;
			CreateSprite(BITMAP_SHINY + 1, Position, 1.5f, Light2, nullptr, (mu_float)(GetLargeRand(360)));
			CreateSprite(BITMAP_SHINY + 1, Position, 1.5f, Light2, nullptr, (mu_float)(GetLargeRand(360)));

			CreateSprite(BITMAP_LIGHT, Position, 3.5f, Light, nullptr, (mu_float)(GetLargeRand(360)));
		}
		break;
	case MODEL_STORM:
		switch (o->SubType)
		{
		case 0:
			o->BlendMeshLight = o->LifeTime*0.1f;
			o->BlendMeshTexCoordU = -(mu_float)o->LifeTime*0.1f;
			CreateParticle(ThreadIndex, BITMAP_SMOKE, o->Position, o->Angle, o->Light, 3);
			Vector(90.0f, 0.0f, o->Angle[2], Angle);
			if (GetLargeRand(2) == 0)
			{
				Vector(o->Position[0] - 200.0f, o->Position[1], o->Position[2] + 700.0f, Position);
				CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, Position, o->Position, Angle, 0, o, 10.0f);
			}
			if (GetLargeRand(2) == 0)
			{
				Vector(o->Position[0] + 200.0f, o->Position[1], o->Position[2] + 700.0f, Position);
				CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, Position, o->Position, Angle, 0, o, 10.0f);
			}
			o->Position[2] = RequestTerrainHeight(o->Position[0], o->Position[1]);
			if (GetLargeRand(4) == 0)
				CreateEffect(ThreadIndex, MODEL_STONE1 + GetLargeRand(2), o->Position, o->Angle, o->Light, 2);
			Vector(-Luminosity*0.4f, -Luminosity*0.3f, -Luminosity*0.2f, Light);
			AddTerrainLight(o->Position[0], o->Position[1], Light, 5, PrimaryTerrainLight);

			if (battleCastle::IsBattleCastleStart())
			{
				mu_uint32 att = TERRAIN_ATTRIBUTE(o->Position[0], o->Position[1]);
				if ((att&TW_NOATTACKZONE) == TW_NOATTACKZONE)
				{
					o->Velocity = 0.0f;
					Vector(0.0f, 0.0f, 0.0f, o->Direction);
					o->LifeTime /= 5.0f;
					break;
				}
			}
			break;

		case 1:
			o->BlendMeshLight = o->LifeTime*0.01f;
			o->BlendMeshTexCoordU = -(mu_float)o->LifeTime*0.1f;
			o->Angle[2] += GetLargeRand(30) + 30.0f;

			VectorCopy(o->Position, Position);
			Position[2] += 100.0f;
			CreateParticle(ThreadIndex, BITMAP_SMOKE, Position, o->Angle, o->Light, 3);
			CreateParticle(ThreadIndex, BITMAP_BUBBLE, Position, o->Angle, o->Light, 3, 0.1f);

			Vector(-Luminosity*0.1f, -Luminosity*0.3f, -Luminosity*1.0f, Light);
			AddTerrainLight(o->Position[0], o->Position[1], o->Light, 5, PrimaryTerrainLight);
			break;

		case 2:
			o->BlendMeshLight = o->LifeTime*0.1f;
			o->BlendMeshTexCoordU = -(mu_float)o->LifeTime*0.1f;

			o->Gravity = (mu_float)(GetLargeRand(360));

			VectorCopy(o->Position, Position);
			Position[2] += 100.0f;
			CreateParticle(ThreadIndex, BITMAP_SMOKE, Position, o->Angle, o->Light, 3);
			CreateParticle(ThreadIndex, BITMAP_BUBBLE, Position, o->Angle, o->Light, 3, 0.1f);

			Vector(-Luminosity*0.1f, -Luminosity*0.3f, -Luminosity*1.0f, Light);
			AddTerrainLight(o->Position[0], o->Position[1], o->Light, 5, PrimaryTerrainLight);
			break;
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
			EarthQuake.Store((mu_float)(GetLargeRand(8) - 4)*0.1f);
			CreateParticle(ThreadIndex, BITMAP_SMOKE, o->Position, o->Angle, o->Light, 28);
			CreateParticle(ThreadIndex, BITMAP_SMOKE, o->Position, o->Angle, o->Light, 29);
			CreateParticle(ThreadIndex, BITMAP_SMOKE, o->Position, o->Angle, o->Light, 30);
			if (GetLargeRand(2) == 0)
				CreateEffect(ThreadIndex, MODEL_STONE1 + GetLargeRand(2), o->Position, o->Angle, o->Light, 2);
			break;
		case 8:
			{
				o->BlendMeshLight = o->LifeTime*0.1f;
				o->BlendMeshTexCoordU = -(mu_float)o->LifeTime*0.1f;

				Vector(-Luminosity*0.1f, -Luminosity*0.3f, -Luminosity*1.0f, Light);
				AddTerrainLight(o->Position[0], o->Position[1], o->Light, 5, PrimaryTerrainLight);
			}
			break;
		}
		break;
	case MODEL_SUMMON:
		{
			if (o->LifeTime >= 30)
			{
				o->Light[0] *= 1.03f;
				o->Light[1] *= 1.03f;
				o->Light[2] *= 1.03f;
			}
			else
			{
				o->Light[0] /= 1.08f;
				o->Light[1] /= 1.08f;
				o->Light[2] /= 1.08f;
			}
			o->Angle[2] += 1.0f;
		}
		break;
	case MODEL_STORM2:
		{
			if (o->SubType == 0)
			{
				o->Angle[1] += 40.0f;

				if (o->LifeTime <= 5)
				{
					o->Light[0] /= 1.01f;
					o->Light[1] /= 1.01f;
					o->Light[2] /= 1.01f;
				}
			}
			else if (o->SubType == 1)
			{
				EVector3 Pos1, Pos2, Direction;

				EBone Matrix;

				for (mu_int32 i = 0; i < 2; ++i)
				{
					Angle[0] = o->Angle[0];
					Angle[1] = o->Angle[1];
					Angle[2] = o->Angle[2] / 3.0f + 180.0f*i;

					Vector(0.0f, 200.0f, 0.0f, Direction);
					AngleMatrix(Angle, Matrix);
					VectorRotate(Direction, Matrix, Position);
					VectorAdd(Position, o->Position, Position);

					Vector(0.3f, 0.5f, 0.6f, Light);
					CreateParticle(ThreadIndex, BITMAP_CLUD64, Position, o->Angle, Light, 1, 1.0f);
				}

				Vector(90.0f, 0.0f, 0.0f, Angle);

				Pos1[0] = o->Position[0] + GetLargeRand(600) - 300.0f;
				Pos1[1] = o->Position[1] + GetLargeRand(600) - 300.0f;
				Pos1[2] = o->Position[2];

				Pos2[0] = Pos1[0] + GetLargeRand(100) - 50.0f;
				Pos2[1] = Pos1[1] + GetLargeRand(100) - 50.0f;
				Pos2[2] = Pos1[2];

				CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, Pos1, Pos2, Angle, 20, o, 20.0f);

				Pos2[0] = Pos1[0] + GetLargeRand(100) - 50.0f;
				Pos2[1] = Pos1[1] + GetLargeRand(100) - 50.0f;
				Pos2[2] = Pos1[2];
				CreateEffect(ThreadIndex, MODEL_STONE1 + GetLargeRand(2), Pos2, o->Angle, Light, 2);

				o->Angle[2] += o->Gravity;
				o->Light[0] /= 1.02f;
				o->Light[1] /= 1.02f;
				o->Light[2] /= 1.02f;

				o->Alpha += 0.01f;

				if (o->Light[0] <= 0.05f)
					o->Flags.Clear(OBJECT::eOF_LIVE);
			}
			else if (o->SubType == 2)
			{
				o->Angle[1] += 40.0f;

				if (o->LifeTime <= 5)
				{
					o->Light[0] /= 1.01f;
					o->Light[1] /= 1.01f;
					o->Light[2] /= 1.01f;
				}

				CreateEffect(ThreadIndex, MODEL_STONE1 + GetLargeRand(2), o->Position, o->Angle, o->Light, 2);
			}
		}
		break;
	case MODEL_STORM3:
		{
			if (o->LifeTime <= 30)
			{
				o->Light[0] /= 1.1f;
				o->Light[1] /= 1.1f;
				o->Light[2] /= 1.1f;
			}
			o->Angle[1] += 30.0f;

			Vector(90.0f, 0.0f, o->Angle[2], Angle);

			for (mu_int32 i = 0; i < 5; ++i)
			{
				if (GetLargeRand(2) == 0)
				{
					EVector3 Pos1, Pos2;

					Pos1[0] = o->StartPosition[0] + GetLargeRand(2000) - 1000.0f;
					Pos1[1] = o->StartPosition[1] + GetLargeRand(2000) - 1000.0f;
					Pos1[2] = o->StartPosition[2];

					Pos2[0] = Pos1[1] + GetLargeRand(200) - 100.0f;
					Pos2[1] = Pos1[1] + GetLargeRand(200) - 100.0f;

					CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, Pos1, Pos2, Angle, 20, o, 20.0f);
				}
			}

		}
		break;
	case MODEL_MAYASTAR:
		{
			if (o->LifeTime <= 20)
			{
				o->Light[0] /= 1.2f;
				o->Light[1] /= 1.2f;
				o->Light[2] /= 1.2f;
			}
			o->Scale += EMath::Sin(FWorldTime*0.005f)*2.0f;
			o->Angle[1] += 10.0f;
			EarthQuake.Store((mu_float)(GetLargeRand(6) - 6)*0.5f);
		}
		break;
	case MODEL_MAYASTONE1:
	case MODEL_MAYASTONE2:
	case MODEL_MAYASTONE3:
		{

			mu_boolean success = false;

			VectorCopy(o->HeadAngle, o->Angle);

			mu_float Height = RequestTerrainHeight(o->Position[0], o->Position[1]);
			if (o->Position[2] < Height)
				success = true;

			if (success)
			{
				o->Position[2] = Height;
				Vector(0.0f, 0.0f, 0.0f, o->Direction);
				EVector3 Position;
				Vector(o->Position[0], o->Position[1], o->Position[2] + 80.0f, Position);

				mu_uint8 smokeNum = 15;

				Vector(0.0f, 0.6f, 1.0f, Light);
				Vector(0.0f, 0.0f, 0.0f, Angle);
				CreateEffect(ThreadIndex, MODEL_SKILL_INFERNO, o->Position, Angle, Light, 2, o, 30, 0);

				for (mu_int32 i = 0; i<smokeNum; ++i)
				{
					Position[0] = o->Position[0] + (GetLargeRand(160) - 80);
					Position[1] = o->Position[1] + (GetLargeRand(160) - 100);
					Position[2] = o->Position[2] + 50;

					Vector(0.3f, 0.5f, 1.0f, Light);
					CreateParticle(ThreadIndex, BITMAP_SMOKE, Position, o->Angle, Light, 11, (mu_float)(GetLargeRand(32) + 80)*0.025f);
				}

				for (mu_int32 j = 0; j<6; ++j)
					CreateEffect(ThreadIndex, MODEL_MAYASTONE4 + GetLargeRand(2), o->Position, o->Angle, Light);

				CreateParticle(ThreadIndex, BITMAP_EXPLOTION, Position, o->Angle, Light, 0, 4.0f);

				o->Flags.Clear(OBJECT::eOF_LIVE);
			}
		}
		break;
	case MODEL_MAYASTONE4:
	case MODEL_MAYASTONE5:
		{
			VectorAdd(o->Position, o->Direction, o->Position);
			VectorScale(o->Direction, 0.9f, o->Direction);
			o->Position[2] += o->Gravity;
			o->Gravity -= 3.0f;
			Height = RequestTerrainHeight(o->Position[0], o->Position[1]);
			if (o->Position[2] < Height)
			{
				o->Position[2] = Height;
				o->Gravity = -o->Gravity*0.5f;
				o->LifeTime -= 4;
				o->Angle[0] -= o->Scale*128.0f;
			}
			else
				o->Angle[0] -= o->Scale*32.0f;

			if (GetLargeRand(10) == 0)
				CreateParticle(ThreadIndex, BITMAP_FIRE, o->Position, o->Angle, o->Light, 15);
		}
		break;
	case MODEL_MAYASTONEFIRE:
		{
			VectorCopy(o->HeadAngle, o->Angle);

			mu_float Height = RequestTerrainHeight(o->Position[0], o->Position[1]);
			if (o->Position[2] < Height)
				o->Flags.Clear(OBJECT::eOF_LIVE);
		}
		break;
	case MODEL_MAYAHANDSKILL:
		{
			if (o->SubType == 0)
			{
				o->Light[0] /= 1.17f;
				o->Light[1] /= 1.17f;
				o->Light[2] /= 1.17f;

				EVector3 vPos;
				VectorCopy(o->Position, vPos);
				vPos[2] += 100.0f;
				CreateParticle(ThreadIndex, BITMAP_SMOKE, vPos, o->Angle, o->StartPosition, 17, 10.0f);

				EVector3 Direction;
				Vector(0.0f, (-80.0f - GetLargeRand(30))*o->Scale, 0.0f, Direction);
				AngleMatrix(o->Angle, Matrix);
				VectorRotate(Direction, Matrix, Position);
				VectorAdd(Position, o->Position, o->Position);
				CreateEffect(ThreadIndex, MODEL_MAYAHANDSKILL, o->Position, o->Angle, o->StartPosition, 1, nullptr, -1, 0, 0, 0, o->Scale + 0.2f);

				if (o->Light[0] <= 0.05f)
					o->Flags.Clear(OBJECT::eOF_LIVE);
			}
			else if (o->SubType == 1)
			{
				o->Light[0] /= 1.17f;
				o->Light[1] /= 1.17f;
				o->Light[2] /= 1.17f;

				EVector3 vPos;
				VectorCopy(o->Position, vPos);
				vPos[2] += 100.0f;
				CreateParticle(ThreadIndex, BITMAP_SMOKE, vPos, o->Angle, o->StartPosition, 17, 10.0f);
			}
		}
		break;

	case MODEL_CIRCLE:
		o->BlendMeshLight = o->LifeTime*0.1f;
		if (o->SubType == 1)
		{
			mu_int32 skillCount = (mu_int32)((g_CurrentTime - o->Owner->SpecialSkillTick) / 500ull);
			if (skillCount > 12)
			{
				skillCount = 12;
			}
			if (o->LifeTime>(44 - skillCount))
			{
				EVector3 Angle = { 0.0f, 0.0f, 0.0f };
				mu_int32 iCount = 36;
				for (mu_int32 i = 0; i < iCount; ++i)
				{
					Angle[0] = -10.0f;
					Angle[1] = 0.0f;
					Angle[2] = i*(10.0f + GetLargeRand(10));
					EVector3 Position;
					VectorCopy(o->Position, Position);
					Position[2] += 100.0f;
					CreateJoint(ThreadIndex, BITMAP_JOINT_SPIRIT, Position, Position, Angle, 6, o, 60.0f, 0, 0);

					if (o->LifeTime == (44 - skillCount + 1))
					{
						Angle[2] = i*10.0f;
						CreateJoint(ThreadIndex, BITMAP_JOINT_SPIRIT, Position, Position, Angle, 7, o, 60.0f, 0, 0);
					}
				}
			}
		}
		else if (o->SubType == 2)
		{
			if (o->LifeTime>240)
			{
				o->BlendMeshLight = (250 - o->LifeTime)*0.1f;
			}
			else
			{
				o->BlendMeshLight = o->LifeTime*0.1f;
			}
		}
		else if (o->SubType == 3)
		{
			if (o->LifeTime>10)
			{
				o->BlendMeshLight = (30 - o->LifeTime)*0.1f;
			}
			else
			{
				o->BlendMeshLight = o->LifeTime*0.1f;
			}
		}
		else if (o->SubType == 4)
		{
			mu_int32 skillCount = (mu_int32)((g_CurrentTime - o->Owner->SpecialSkillTick) / 500ull);
			if (skillCount > 12)
			{
				skillCount = 12;
			}
			if (o->LifeTime>(44 - skillCount))
			{
				EVector3 Angle, Position;
				for (mu_int32 i = 0; i < 36; ++i)
				{
					Angle[0] = -10.0f;
					Angle[1] = 0.0f;
					Angle[2] = i*10.0f;

					VectorCopy(o->Position, Position);
					Position[2] += 100.0f;
					CreateJoint(ThreadIndex, BITMAP_JOINT_SPIRIT, Position, Position, Angle, 22, o, 2.0f, 0, 0);
					CreateJoint(ThreadIndex, BITMAP_JOINT_SPIRIT, Position, Position, Angle, 23, o, 1.75f, 0, 0);
				}
			}
		}
		break;
		
	case MODEL_CIRCLE_LIGHT:
		if (o->SubType != 3 && o->SubType != 4)
		{
			mu_int32 value = 4;

			if (o->LifeTime >= 30)
				o->BlendMeshLight = (40 - o->LifeTime)*0.1f;
			else
				o->BlendMeshLight = o->LifeTime*0.1f;
			o->BlendMeshTexCoordU = -(mu_float)o->LifeTime*0.01f;
			EarthQuake.Store((mu_float)(GetLargeRand(6) - 6)*0.1f);

			if (GetLargeRand(value) == 0)
			{
				EVector3 p, Position;
				EVector3 Angle;
				EBone Matrix;
				Vector(0.0f, (mu_float)(GetLargeRand(300)), 0.0f, p);
				Vector(0.0f, 0.0f, (mu_float)(GetLargeRand(360)), Angle);
				AngleMatrix(Angle, Matrix);
				VectorRotate(p, Matrix, Position);
				VectorAdd(Position, o->Position, Position);
				CreateEffect(ThreadIndex, MODEL_STONE1 + GetLargeRand(2), Position, o->Angle, o->Light, 1);
			}
			Vector(Luminosity*1.0f, Luminosity*0.8f, Luminosity*0.2f, Light);
			AddTerrainLight(o->Position[0], o->Position[1], Light, 4, PrimaryTerrainLight);
		}
		else if (o->SubType == 3)
		{
			mu_int32 value = 2;

			if (o->LifeTime >= 240)
				o->BlendMeshLight = (250 - o->LifeTime)*0.1f;
			else
				o->BlendMeshLight = o->LifeTime*0.1f;

			o->BlendMeshLight = EMath::Min(0.5f, o->BlendMeshLight);
			o->BlendMeshTexCoordU = -(mu_float)o->LifeTime*0.01f;
			if (o->LifeTime>30 && GetLargeRand(value) == 0)
			{
				EVector3 p, Position;
				EVector3 Angle;
				EBone Matrix;
				Vector(0.0f, (mu_float)(GetLargeRand(200)), 0.0f, p);
				Vector(0.0f, 0.0f, (mu_float)(GetLargeRand(360)), Angle);
				AngleMatrix(Angle, Matrix);
				VectorRotate(p, Matrix, Position);
				VectorAdd(Position, o->Position, Position);

				CreateParticle(ThreadIndex, BITMAP_FLARE_BLUE, Position, o->Angle, o->Light, 0);
				if (o->LifeTime>40)
				{
					Position[2] += 600.0f;
					Angle[2] = 45.0f;
					CreateJoint(ThreadIndex, BITMAP_FLARE_BLUE, Position, Position, Angle, 19, nullptr, 40);
				}
			}
			Vector(o->BlendMeshLight, o->BlendMeshLight, o->BlendMeshLight, Light);
			AddTerrainLight(o->Position[0], o->Position[1], Light, 4, PrimaryTerrainLight);
		}
		else if (o->SubType == 4)
		{
			mu_int32 value = 5;

			if (o->LifeTime >= 10)
				o->BlendMeshLight = (20 - o->LifeTime)*0.1f;
			else
				o->BlendMeshLight = o->LifeTime*0.1f;

			o->BlendMeshLight = EMath::Min(0.5f, o->BlendMeshLight);
			o->BlendMeshTexCoordU = -(mu_float)o->LifeTime*0.01f;
			if (GetLargeRand(value) == 0 && o->LifeTime>5)
			{
				EVector3 p, Position;
				EVector3 Angle;
				EBone Matrix;
				Vector(0.0f, (mu_float)(GetLargeRand(100)), 0.0f, p);
				Vector(0.0f, 0.0f, (mu_float)(GetLargeRand(360)), Angle);
				AngleMatrix(Angle, Matrix);
				VectorRotate(p, Matrix, Position);
				VectorAdd(Position, o->Position, Position);

				CreateParticle(ThreadIndex, BITMAP_FLARE_BLUE, Position, o->Angle, o->Light, 0);
				Position[2] += 600.0f;
				Angle[2] = 45.0f;
				CreateJoint(ThreadIndex, BITMAP_FLARE_BLUE, Position, Position, Angle, 19, nullptr, 40);
			}
			Vector(o->BlendMeshLight, o->BlendMeshLight, o->BlendMeshLight, Light);
			AddTerrainLight(o->Position[0], o->Position[1], Light, 4, PrimaryTerrainLight);
		}
		break;
	case MODEL_SNOW2:
	case MODEL_SNOW3:
	case MODEL_STONE1:
	case MODEL_STONE2:
		if (o->SubType == 5)
		{
			VectorAdd(o->Owner->Position, o->StartPosition, o->Position);
			break;
		}
		else if (o->SubType == 11)
		{
			if (o->LifeTime<10)
			{
				o->Alpha *= 0.8f;
			}
			break;
		}
		else if (o->SubType == 13)
		{
			o->HeadAngle[2] -= o->Gravity;

			o->Position[0] += o->HeadAngle[0];
			o->Position[1] += o->HeadAngle[1];
			o->Position[2] += o->HeadAngle[2];

			Height = RequestTerrainHeight(o->Position[0], o->Position[1]);
			o->Angle[0] += 0.5f * o->LifeTime;
			o->Angle[1] += 0.5f * o->LifeTime;
			if (o->Position[2] + o->Direction[2] <= Height)
			{
				o->Position[2] = Height;
				o->HeadAngle[0] *= 0.6f;
				o->HeadAngle[1] *= 0.6f;
				o->HeadAngle[2] += 1.0f*o->LifeTime;
				if (o->HeadAngle[2] < 0.5f)
					o->HeadAngle[2] = 0;

				o->Alpha -= 0.1f;
			}
			break;
		}
	case MODEL_BONE1:
	case MODEL_BONE2:
	case MODEL_BIG_STONE1:
	case MODEL_BIG_STONE2:
		if (o->SubType == 5)
		{
			if (o->Owner->Flags.Get(OBJECT::eOF_LIVE) == false) o->Flags.Clear(OBJECT::eOF_LIVE);
			VectorAdd(o->Owner->Position, o->StartPosition, o->Position);
			break;
		}
	case MODEL_ICE_SMALL:
	case MODEL_METEO1:
	case MODEL_METEO2:
	case MODEL_BOSS_ATTACK:
	case MODEL_EFFECT_SAPITRES_ATTACK_2:
		if (o->SubType == 0 || o->SubType == 10 || o->SubType == 12)
		{
			VectorAdd(o->Position, o->Direction, o->Position);
		}
		else if (o->Type == MODEL_EFFECT_SAPITRES_ATTACK_2 && o->SubType == 14)
		{
			mu_float fMoveSpeed = (mu_float)(GetLargeRand(5) + 25);
			o->Position[0] -= (o->Direction[0] * fMoveSpeed);
			o->Position[1] -= (o->Direction[1] * fMoveSpeed);
			o->Position[2] -= (o->Direction[2] * fMoveSpeed);

			if (o->LifeTime <= 10)
			{
				o->BlendMeshLight -= 0.1f;
			}
		}
		else
		{
			AngleMatrix(o->Angle, Matrix);
			VectorRotate(o->Direction, Matrix, Position);
			VectorAdd(o->Position, Position, o->Position);
		}
		VectorScale(o->Direction, 0.9f, o->Direction);
		o->Position[2] += o->Gravity;
		if (o->SubType == 0 || o->SubType == 10 || o->SubType == 12
			|| o->SubType == 13)
		{
			o->Gravity -= 3.0f;
			Height = RequestTerrainHeight(o->Position[0], o->Position[1]);
			if (o->Position[2] < Height)
			{
				o->Position[2] = Height;
				o->Gravity = -o->Gravity*0.5f;
				o->LifeTime -= 4;
				o->Angle[0] -= o->Scale*128.0f;
			}
			else
				o->Angle[0] -= o->Scale*32.0f;

			if ((o->SubType == 0 || o->SubType == 12
				|| o->SubType == 13) && GetLargeRand(10) == 0)
			{
				if (o->Type == MODEL_ICE_SMALL || o->Type == MODEL_METEO1 || o->Type == MODEL_METEO2)
					CreateParticle(ThreadIndex, BITMAP_SMOKE, o->Position, o->Angle, o->Light);
				else if (o->Type == MODEL_STONE1 || o->Type == MODEL_STONE2)
				{
					CreateParticle(ThreadIndex, BITMAP_FIRE, o->Position, o->Angle, o->Light, 1 + GetLargeRand(3));
				}
			}
		}
		else if (o->SubType == 1)
			o->Gravity += 0.5f;
		else if (o->Type != MODEL_EFFECT_SAPITRES_ATTACK_2)
		{
			o->Angle[2] += 20.0f;
			o->Gravity += 0.5f;
		}
		break;
	case MODEL_EFFECT_BROKEN_ICE0:
	case MODEL_EFFECT_BROKEN_ICE1:
	case MODEL_EFFECT_BROKEN_ICE2:
	case MODEL_EFFECT_BROKEN_ICE3:
		if (o->SubType == 0)
		{
			o->HeadAngle[2] -= o->Gravity;

			o->Position[0] += o->HeadAngle[0];
			o->Position[1] += o->HeadAngle[1];
			o->Position[2] += o->HeadAngle[2];

			Height = RequestTerrainHeight(o->Position[0], o->Position[1]);
			o->Angle[0] += 0.5f * o->LifeTime;
			o->Angle[1] += 0.5f * o->LifeTime;
			if (o->Position[2] + o->Direction[2] <= Height)
			{
				o->Position[2] = Height;
				o->HeadAngle[0] *= 0.6f;
				o->HeadAngle[1] *= 0.6f;
				o->HeadAngle[2] += 1.0f*o->LifeTime;
				if (o->HeadAngle[2] < 0.5f)
					o->HeadAngle[2] = 0;

				o->Alpha -= 0.1f;
			}

			if (GetLargeRand(30) == 0)
				CreateParticle(ThreadIndex, BITMAP_SMOKE, o->Position, o->Angle, o->Light);
		}
		else if (o->SubType == 1)
		{
			mu_boolean bSuccess = false;

			VectorCopy(o->HeadAngle, o->Angle);

			mu_float fHeight = RequestTerrainHeight(o->Position[0], o->Position[1]);
			if (o->Position[2] < fHeight)
			{
				bSuccess = true;
			}

			if (bSuccess)
			{
				o->Position[2] = fHeight;
				Vector(0.0f, 0.0f, 0.0f, o->Direction);
				EVector3 vPos;
				Vector(o->Position[0], o->Position[1], o->Position[2] + 80.0f, vPos);

				Vector(0.0f, 0.6f, 1.0f, Light);
				Vector(0.0f, 0.0f, 0.0f, Angle);

				CreateEffect(ThreadIndex, MODEL_SKILL_INFERNO, o->Position, Angle, Light, 2, o);

				Vector(0.2f, 0.4f, 0.8f, Light);
				for (mu_int32 i = 0; i<8; ++i)
				{
					vPos[0] = o->Position[0] + (GetLargeRand(80) - 40);
					vPos[1] = o->Position[1] + (GetLargeRand(80) - 40);
					vPos[2] = o->Position[2] + 50;

					CreateParticle(ThreadIndex, BITMAP_SMOKE, vPos, o->Angle, Light, 11, (mu_float)(GetLargeRand(40) + 60)*0.025f);
				}

				for (mu_int32 j = 0; j<6; ++j)
				{
					CreateEffect(ThreadIndex, MODEL_EFFECT_BROKEN_ICE0 + GetLargeRand(3), vPos, o->Angle, Light, 0);
				}

				Vector(0.6f, 0.6f, 1.0f, Light);
				vPos[0] = o->Position[0];
				vPos[1] = o->Position[1];
				vPos[2] = o->Position[2] + 100;
				CreateParticle(ThreadIndex, BITMAP_EXPLOTION_MONO, vPos, o->Angle, Light, 1, 1.5f);

				o->Flags.Clear(OBJECT::eOF_LIVE);
			}
		}
		else if (o->SubType == 2)
		{
			o->Position[2] -= o->Gravity;

			mu_float fHeight = RequestTerrainHeight(o->Position[0], o->Position[1]);
			if (o->Position[2] < fHeight)
			{
				CreateEffect(ThreadIndex, o->Type, o->Position, o->Angle, o->Light, 0);

				o->Flags.Clear(OBJECT::eOF_LIVE);
			}
		}
		break;
	case MODEL_SNOW1:
		o->Direction[2] -= 2.5f;
		CheckTargetRange(ThreadIndex, o);
		break;
	case MODEL_WOOSISTONE:
		if (o->SubType == 1)
		{
			o->HeadAngle[2] -= o->Gravity;

			o->Position[0] += o->HeadAngle[0];
			o->Position[1] += o->HeadAngle[1];
			o->Position[2] += o->HeadAngle[2];

			Height = RequestTerrainHeight(o->Position[0], o->Position[1]);
			o->Angle[0] += 0.5f * o->LifeTime;
			o->Angle[1] += 0.5f * o->LifeTime;
			if (o->Position[2] + o->Direction[2] <= Height)
			{
				o->Position[2] = Height;
				o->HeadAngle[0] *= 0.6f;
				o->HeadAngle[1] *= 0.6f;
				o->HeadAngle[2] += 1.0f*o->LifeTime;
				if (o->HeadAngle[2] < 0.5f)
					o->HeadAngle[2] = 0;

				o->Alpha -= 0.1f;
			}
		}
		else
		{
			o->Direction[2] -= 2.0f;
			CheckTargetRange(ThreadIndex, o);
		}
		break;
	case MODEL_SKULL:
		if (o->SubType == 1 && o->Owner != nullptr)
		{
			mu_float dx = o->Position[0] - o->StartPosition[0];
			mu_float dy = o->Position[1] - o->StartPosition[1];

			if ((o->LifeTime % 10) == 0)
			{
				VectorCopy(o->Position, o->m_vDeadPosition);
			}

			VectorCopy(o->Owner->Position, o->StartPosition);
			o->StartPosition[2] += 200.0f;

			MoveHumming(o->Position, o->Angle, o->StartPosition, 10.0f);

			o->HeadAngle[0] += (mu_float)(GetLargeRand(32) - 16)*0.2f;
			o->HeadAngle[2] += (mu_float)(GetLargeRand(32) - 16)*0.8f;
			o->Angle[0] += o->HeadAngle[0];
			o->Angle[2] += o->HeadAngle[2];
			o->HeadAngle[0] *= 0.6f;
			o->HeadAngle[2] *= 0.8f;

			if (o->LifeTime<10)
			{
				o->Alpha /= 1.1f;
				o->BlendMeshLight /= 1.2f;

				if (o->Angle[0] <= 80)
				{
					o->Angle[0] += 5.0f;
				}
			}
			else
			{
				Height = RequestTerrainHeight(o->Position[0], o->Position[1]);
				if (o->Position[2] < Height + 100.0f)
				{
					o->HeadAngle[0] = 0.0f;
					o->Angle[0] = -5.0f;
				}
				if (o->Position[2] > Height + 400.0f)
				{
					o->HeadAngle[0] = 0.0f;
					o->Angle[0] = 5.0f;
				}
			}

			CreateParticle(ThreadIndex, BITMAP_FIRE, o->Position, o->Angle, Light, 5, 1.2f);

			Vector(Luminosity, Luminosity*0.3f, Luminosity*0.1f, Light);
			AddTerrainLight(o->Position[0], o->Position[1], Light, 4, PrimaryTerrainLight);
		}
		break;
	case MODEL_CUNDUN_PART1:
	case MODEL_CUNDUN_PART2:
	case MODEL_CUNDUN_PART3:
	case MODEL_CUNDUN_PART4:
	case MODEL_CUNDUN_PART5:
	case MODEL_CUNDUN_PART6:
	case MODEL_CUNDUN_PART7:
	case MODEL_CUNDUN_PART8:
	case MODEL_MONSTER01 + 64:
		switch (o->SubType)
		{
		case 1:
			o->Alpha -= 0.01f;
			break;
		case 2:
		case 3:
			if (o->LifeTime < 250 + 50 + (34 - o->Skill) * 5)
			{
				if (o->Position[2] > 350.0f)
				{
					if (o->Direction[0] == 0 && o->Direction[1] == 0 && o->Direction[2] == 0)
					{
						o->Direction[0] = (mu_float)((-1 + (GetLargeRand(2))) * 2);
						o->Direction[1] = (mu_float)((-1 + (GetLargeRand(2))) * 2);
						o->Direction[2] = 0;
					}
					o->Direction[2] -= 6.0f;
				}
				else
				{
					mu_boolean bUseEarthQuake = false;
					if (o->Direction[2] != -3.0f) bUseEarthQuake = true;

					if (bUseEarthQuake == true)
					{
						mu_int32 PositionX = (mu_int32)(o->Position[0] * g_TerrainScaleRev);
						mu_int32 PositionY = (mu_int32)(o->Position[1] * g_TerrainScaleRev);
						AddWaterWave(PositionX, PositionY, 2, -1000);

						EVector3 Light = { 0.3f, 0.3f, 0.3f };
						EVector3 Angle;
						for (mu_int32 i = 0; i < 100; ++i)
						{
							VectorCopy(o->Position, Position);
							mu_float fAngle = (mu_float)(GetLargeRand(360));
							mu_float fLength = (mu_float)(GetLargeRand(30) + 30);
							Position[0] += EMath::Sin(fAngle) * fLength;
							Position[1] += EMath::Cos(fAngle) * fLength;
							Position[2] -= 30.0f;
							VectorCopy(o->Angle, Angle);
							Angle[2] += GetLargeRand(180);
							CreateParticle(ThreadIndex, BITMAP_WATERFALL_5, Position, Angle, Light, 2);
						}
					}
					VectorCopy(o->Position, Position);
					Position[0] += GetLargeRand(40) - 20;
					Position[1] += GetLargeRand(40) - 20;
					Position[2] -= 50.0f;
					CreateParticle(ThreadIndex, BITMAP_BUBBLE, Position, o->Angle, o->Light, 4);

					o->Direction[0] = 0;
					o->Direction[1] = 0;
					o->Direction[2] = -3.0f;
					
					if (o->HeadAngle[0] == 0)
					{
						if (o->Angle[0] > -30)
						{
							Vector(o->Angle[0] - (GetLargeRand(3) + 1), o->Angle[1] - 0.0f, o->Angle[2] - 0.0f, o->Angle);
						}
						else
						{
							o->HeadAngle[0] = 1;

						}
					}
					else
					{
						if (o->Angle[0] < 30)
						{
							Vector(o->Angle[0] + (GetLargeRand(3) + 1), o->Angle[1] - 0.0f, o->Angle[2] - 0.0f, o->Angle);
						}
						else
						{
							o->HeadAngle[0] = 0;

						}
					}
					if (o->LifeTime < 100)
					{
						o->Alpha -= 0.1f;
					}
				}
			}
			else
			{
				if (o->SubType != 2 && o->LifeTime > 200 + 50 + 135)
				{
					EVector3 Angle = { 0.0f, 0.0f, 0.0f };
					if (GetLargeRand(2) == 0)
					{
						Angle[0] = 0.0f;
						Angle[1] = 0.0f;
						Angle[2] = (mu_float)(GetLargeRand(360));
						EVector3 Position;
						VectorCopy(o->Position, Position);
						if (GetLargeRand(2) == 0)
							CreateJoint(ThreadIndex, BITMAP_JOINT_SPIRIT2, Position, Position, Angle, 8, nullptr, 50.0f, 0, 0);
					}

					if (GetLargeRand(3) == 0)
					{
						EVector3 Light;
						Vector(0.5f, 0.5f, 0.5f, Light);
						CreateParticle(ThreadIndex, BITMAP_SMOKE + 3, o->Position, o->Angle, Light, 2, (mu_float)(GetLargeRand(32) + 48)*0.02f);
					}
				}
			}
			break;
		case 4:
			if (o->LifeTime < 140)
			{
				o->Alpha -= 0.01f;
				if (o->Position[2] > o->StartPosition[2])
				{
					o->Direction[2] -= 3.5f;
				}
				else
				{
					o->Direction[0] = 0;
					o->Direction[1] = 0;
					o->Direction[2] = 0;
				}
			}
			else
			{
				o->AnimationFrame = 0;
			}
			break;
		case 5:
			if (o->LifeTime < 150)
			{
				o->Alpha -= 0.01f;
			}
			else
			{
				o->AnimationFrame = 0;
			}
			break;
		}
		break;
	case MODEL_CURSEDTEMPLE_STATUE_PART1:
	case MODEL_CURSEDTEMPLE_STATUE_PART2:
		VectorCopy(o->Light, Light);
		if (o->Position[2] > 290.0f)
		{
			o->Direction[2] -= o->Gravity;
			o->Angle[2] -= (mu_float)(GetLargeRand(10));
		}
		else if (o->Position[2] < 290)
		{
			o->Position[2] = 290;
			o->Direction[0] = 0;
			o->Direction[1] = 0;
			o->Direction[2] = 0;
			EarthQuake.Store((mu_float)(GetLargeRand(4) + 1)*0.1f);
		}
		else
		{
			o->Alpha -= 0.01f;
		}
		break;
	case MODEL_XMAS2008_SNOWMAN_HEAD:
		{
			if (o->LifeTime < 45)
			{
				o->Position[0] += (o->Direction[0] * 1.2f);
				o->Position[1] += (o->Direction[1] * 1.2f);
				o->Position[2] += (o->Gravity * 1.5f);
				o->Gravity -= 1.5f;

				Height = RequestTerrainHeight(o->Position[0], o->Position[1]);
				if (o->Position[2] < Height)
				{
					o->Position[2] = Height;
					o->Gravity = -o->Gravity*0.3f;
					o->LifeTime -= 2;
				}

				VectorAdd(o->Position, o->Direction, o->Position);
			}
		}
		break;
	case MODEL_XMAS2008_SNOWMAN_BODY:
		{
			MUModel * b = &MODELS::Data[o->Type];
			b->PlayAnimation(ThreadIndex, &o->AnimationFrame, &o->PriorAnimationFrame, &o->PriorAction, o->Velocity / 5.0f);
		}
		break;
	case MODEL_TOTEMGOLEM_PART1:
	case MODEL_TOTEMGOLEM_PART2:
	case MODEL_TOTEMGOLEM_PART3:
	case MODEL_TOTEMGOLEM_PART4:
	case MODEL_TOTEMGOLEM_PART5:
	case MODEL_TOTEMGOLEM_PART6:
		o->HeadAngle[2] -= o->Gravity;
		VectorCopy(o->Light, Light);

		o->Position[0] += o->HeadAngle[0];
		o->Position[1] += o->HeadAngle[1];
		o->Position[2] += o->HeadAngle[2];

		Height = RequestTerrainHeight(o->Position[0], o->Position[1]) + 20;

		if (o->Position[2] + o->Direction[2] <= Height)
		{
			o->Position[2] = Height;
			o->HeadAngle[0] *= 0.8f;
			o->HeadAngle[1] *= 0.8f;
			o->HeadAngle[2] += 0.6f*o->LifeTime;
			if (o->HeadAngle[2] < 5.0f)
				o->HeadAngle[2] = 0;

			o->Alpha -= 0.05f;
		}
		else
		{
			if (o->SubType == 0)
			{
				o->Angle[0] += 0.15f * o->LifeTime;
				o->Angle[1] += 0.15f * o->LifeTime;
			}
			else
			{
				o->Angle[0] -= 0.15f * o->LifeTime;
				o->Angle[1] -= 0.15f * o->LifeTime;
			}
		}
		break;
	case MODEL_DOPPELGANGER_SLIME_CHIP:
		o->HeadAngle[2] -= o->Gravity;
		VectorCopy(o->Light, Light);

		o->Position[0] += o->HeadAngle[0];
		o->Position[1] += o->HeadAngle[1];
		o->Position[2] += o->HeadAngle[2];

		Height = RequestTerrainHeight(o->Position[0], o->Position[1]) + 20;

		if (o->LifeTime < 10)
		{
			o->Light[0] *= 0.8f;
			o->Light[1] *= 0.8f;
			o->Light[2] *= 0.8f;
		}

		if (o->Position[2] + o->Direction[2] <= Height)
		{
			o->Position[2] = Height;
			o->HeadAngle[0] *= 0.8f;
			o->HeadAngle[1] *= 0.8f;
			o->HeadAngle[2] += 1.6f*o->LifeTime;
			if (o->HeadAngle[2] < 5.0f)
				o->HeadAngle[2] = 0;
		}
		else
		{
			o->Scale += EMath::Sin(FWorldTime*0.015f)*0.1f;
		}

		if (o->SubType == 0)
		{
			o->Angle[0] += 0.35f * o->LifeTime;
			o->Angle[1] += 0.35f * o->LifeTime;
		}
		else
		{
			o->Angle[0] -= 0.35f * o->LifeTime;
			o->Angle[1] -= 0.35f * o->LifeTime;
		}
		break;

	case MODEL_SHADOW_PAWN_ANKLE_LEFT:		case MODEL_SHADOW_PAWN_ANKLE_RIGHT:
	case MODEL_SHADOW_PAWN_BELT:			case MODEL_SHADOW_PAWN_CHEST:
	case MODEL_SHADOW_PAWN_HELMET:
	case MODEL_SHADOW_PAWN_KNEE_LEFT:		case MODEL_SHADOW_PAWN_KNEE_RIGHT:
	case MODEL_SHADOW_PAWN_WRIST_LEFT:		case MODEL_SHADOW_PAWN_WRIST_RIGHT:

	case MODEL_SHADOW_KNIGHT_ANKLE_LEFT:	case MODEL_SHADOW_KNIGHT_ANKLE_RIGHT:
	case MODEL_SHADOW_KNIGHT_BELT:			case MODEL_SHADOW_KNIGHT_CHEST:
	case MODEL_SHADOW_KNIGHT_HELMET:
	case MODEL_SHADOW_KNIGHT_KNEE_LEFT:		case MODEL_SHADOW_KNIGHT_KNEE_RIGHT:
	case MODEL_SHADOW_KNIGHT_WRIST_LEFT:	case MODEL_SHADOW_KNIGHT_WRIST_RIGHT:

	case MODEL_SHADOW_ROOK_ANKLE_LEFT:		case MODEL_SHADOW_ROOK_ANKLE_RIGHT:
	case MODEL_SHADOW_ROOK_BELT:			case MODEL_SHADOW_ROOK_CHEST:
	case MODEL_SHADOW_ROOK_HELMET:
	case MODEL_SHADOW_ROOK_KNEE_LEFT:		case MODEL_SHADOW_ROOK_KNEE_RIGHT:
	case MODEL_SHADOW_ROOK_WRIST_LEFT:		case MODEL_SHADOW_ROOK_WRIST_RIGHT:

	case MODEL_ICE_GIANT_PART1:				case MODEL_ICE_GIANT_PART2:
	case MODEL_ICE_GIANT_PART3:				case MODEL_ICE_GIANT_PART4:
	case MODEL_ICE_GIANT_PART5:				case MODEL_ICE_GIANT_PART6:
		o->HeadAngle[2] -= o->Gravity;
		VectorCopy(o->Light, Light);

		o->Position[0] += o->HeadAngle[0];
		o->Position[1] += o->HeadAngle[1];
		o->Position[2] += o->HeadAngle[2];

		Height = RequestTerrainHeight(o->Position[0], o->Position[1]) + 20;

		if (o->Position[2] + o->Direction[2] <= Height)
		{
			o->Position[2] = Height;
			o->HeadAngle[0] *= 0.5f;
			o->HeadAngle[1] *= 0.5f;
			o->HeadAngle[2] += 0.6f*o->LifeTime;
			if (o->HeadAngle[2] < 5.0f)
				o->HeadAngle[2] = 0;

			o->Alpha -= 0.05f;
		}
		else
		{
			if (o->SubType == 0)
			{
				o->Angle[0] += 0.15f * o->LifeTime;
				o->Angle[1] += 0.15f * o->LifeTime;
			}
			else
			{
				o->Angle[0] -= 0.15f * o->LifeTime;
				o->Angle[1] -= 0.15f * o->LifeTime;
			}
		}
		break;

	case MODEL_WATER_WAVE:
		o->Scale += 0.02f;
		o->Position[2] = o->StartPosition[2] + o->Gravity;

		o->Velocity -= 10.0f;
		o->Gravity -= 20.0f;
		if (o->Gravity<60) o->Gravity = 60;

		Vector(0.0f, o->Velocity, 0.0f, o->Direction);
		Vector(Luminosity*0.1f, Luminosity*0.3f, Luminosity*0.6f, Light);
		for (mu_int32 j = 0; j<4; ++j)
		{
			CreateParticle(ThreadIndex, BITMAP_WATERFALL_5, o->Position, o->Angle, Light, 1);
		}
		AddTerrainLight(o->Position[0], o->Position[1], Light, 3, PrimaryTerrainLight);
		break;

	case MODEL_STAFF + 8:
		Height = RequestTerrainHeight(o->Position[0], o->Position[1]);
		if (o->Position[2] < Height)
		{
			VectorCopy(o->Position, Position);
			Position[2] += 80;
			CreateParticle(ThreadIndex, BITMAP_EXPLOTION, Position, o->Angle, Light);
			for (mu_int32 j = 0; j<6; ++j)
			{
				CreateEffect(ThreadIndex, MODEL_STONE1 + GetLargeRand(2), o->Position, o->Angle, o->Light);
			}
			o->Flags.Clear(OBJECT::eOF_LIVE);
		}
		Vector(Luminosity*0.2f, Luminosity*0.4f, Luminosity*1.0f, Light);
		AddTerrainLight(o->Position[0], o->Position[1], Light, 2, PrimaryTerrainLight);
		break;

	case MODEL_PIERCING:
		if (o->Owner->Flags.Get(OBJECT::eOF_LIVE) == false)
		{
			o->LifeTime = -1;
			break;
		}
		
		o->Gravity += 90.0f;
		VectorCopy(o->Owner->Angle, o->Angle);
		VectorCopy(o->Owner->Angle, o->HeadAngle);
		VectorCopy(o->Position, o->StartPosition);
		VectorCopy(o->Owner->Position, o->Position);
		if (o->SubType == 1)
		{
			Vector(1.0f, 1.0f, 1.0f, Light);

			CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, o->Position, o->Position, o->Angle, 3, nullptr, 20.0f, 7);
			CreateSprite(BITMAP_SHINY + 1, o->Position, (mu_float)(GetLargeRand(8) + 8)*0.2f, Light, o, (mu_float)(GetLargeRand(360)));
		}

		if (InHellas())
		{
			if (o->Owner != nullptr && o->Owner->Owner != nullptr && o->Owner->Owner == (&Hero->Object))
			{
				mu_int32 PositionX = (mu_int32)(o->Position[0] * g_TerrainScaleRev);
				mu_int32 PositionY = (mu_int32)(o->Position[1] * g_TerrainScaleRev);
				AddWaterWave(PositionX, PositionY, 2, -200);
			}
		}

		o->HeadAngle[1] = o->Gravity;
		break;

	case MODEL_PIERCING + 1:
		if ((GetLargeRand(2)) == 0)
		{
			CreateParticle(ThreadIndex, BITMAP_SMOKE, o->Position, o->Angle, Light, 0);
		}
		CreateParticle(ThreadIndex, BITMAP_FIRE, o->Position, o->Angle, Light, 5);
		CreateParticle(ThreadIndex, BITMAP_FIRE, o->Position, o->Angle, Light, 5);

		Vector(Luminosity*0.6f, Luminosity*0.8f, Luminosity*0.8f, Light);
		AddTerrainLight(o->Position[0], o->Position[1], Light, 2, PrimaryTerrainLight);
		break;

	case MODEL_ARROW_BEST_CROSSBOW:
		if (o->LifeTime>25)
		{
			o->BlendMeshLight = (30 - o->LifeTime) / 40.0f;
		}
		else
		{
			o->BlendMeshLight = 1.0f;
		}
		o->Angle[1] += GetLargeRand(60) + 30.0f;
		VectorCopy(o->Position, o->EyeLeft);

		VectorCopy(o->Angle, Angle);
		AngleMatrix(Angle, Matrix);
		VectorRotate(o->Direction, Matrix, Position);
		VectorAdd(o->EyeLeft, Position, o->EyeLeft);

		Vector(Luminosity*1.0f, Luminosity*0.4f, Luminosity*0.2f, Light);
		AddTerrainLight(o->Position[0], o->Position[1], Light, 2, PrimaryTerrainLight);
		CheckClientArrow(ThreadIndex, o);
		break;

	case MODEL_ARROW_DOUBLE:
		o->Angle[1] += 30.0f;
		VectorCopy(o->Position, o->EyeLeft);
		Vector(Luminosity*0.2f, Luminosity*0.4f, Luminosity*1.0f, Light);
		AddTerrainLight(o->Position[0], o->Position[1], Light, 2, PrimaryTerrainLight);
		CheckClientArrow(ThreadIndex, o);
		break;
	case MODEL_ARROW_HOLY:

		if (o->SubType == 1)
		{
			o->Angle[1] += 60.0f;
			
			if (o->LifeTime == 13)
				CreateEffect(ThreadIndex, MODEL_PIERCING, o->Position, o->Angle, o->Light, 3, o);
			CheckClientArrow(ThreadIndex, o);

			Vector(Luminosity*0.9f, Luminosity*0.4f, Luminosity*0.6f, Light);
			AddTerrainLight(o->Position[0], o->Position[1], Light, 2, PrimaryTerrainLight);
		}
		else
		{
			o->Angle[1] += 30.0f;

			Vector(Luminosity*0.2f, Luminosity*0.4f, Luminosity*1.0f, Light);
			AddTerrainLight(o->StartPosition[0], o->StartPosition[1], Light, 2, PrimaryTerrainLight);

			if (o->SubType != 0)
			{
				if (o->LifeTime == 13)
				{
					CreateEffect(ThreadIndex, MODEL_PIERCING, o->Position, o->Angle, o->Light, 0, o);
				}
				else if (o->LifeTime == 30)
				{
					o->AttackPoint[0] = 0;
					o->Kind = 1;
				}
			}
		}
		break;
	case MODEL_ARROW:
		Vector(Luminosity*0.8f, Luminosity*0.5f, Luminosity*0.2f, Light);
		AddTerrainLight(o->Position[0], o->Position[1], Light, 2, PrimaryTerrainLight);

		if (o->SubType == 3 || o->SubType == 4)
		{
			if (o->SubType == 3)
			{
				o->Angle[0] += o->Gravity;
			}
			if (o->Angle[0]>50)
			{
				o->Angle[0] = 50.0f;
			}

			mu_int32 PositionX = (mu_int32)(o->Position[0] * g_TerrainScaleRev);
			mu_int32 PositionY = (mu_int32)(o->Position[1] * g_TerrainScaleRev);
			mu_int32 WallIndex = TERRAIN_INDEX_REPEAT(PositionX, PositionY);
			mu_int32 Wall = TerrainWall[WallIndex] & TW_NOGROUND;

			if (Wall != TW_NOGROUND)
			{
				mu_float Height = RequestTerrainHeight(o->Position[0], o->Position[1]);
				if (o->Position[2] < Height)
				{
					o->Position[2] = Height + 10;
					Vector(0.0f, 0.0f, 0.0f, o->Direction);
				}
			}
			CreateParticle(ThreadIndex, BITMAP_FIRE, o->Position, o->Angle, Light, 5);
		}
		else if (o->SubType == 5)
		{
			Vector(Luminosity*0.1f, Luminosity*0.6f, Luminosity*0.3f, Light);
			CreateParticle(ThreadIndex, BITMAP_FIRE, o->Position, o->Angle, Light);
			CheckClientArrow(ThreadIndex, o);
		}
		else
		{
			CreateParticle(ThreadIndex, BITMAP_FIRE, o->Position, o->Angle, Light);
			CheckClientArrow(ThreadIndex, o);
		}
		break;
	case MODEL_ARROW_STEEL:
	case MODEL_ARROW_THUNDER:
	case MODEL_ARROW_LASER:
	case MODEL_ARROW_V:
	case MODEL_ARROW_SAW:
	case MODEL_ARROW_NATURE:
	case MODEL_ARROW_WING:
	case MODEL_LACEARROW:
		if (o->Type == MODEL_ARROW_NATURE)
		{
			if (o->SubType == 1)
			{
				Vector(0.1f, 0.4f, 0.1f, Light);
				CreateSprite(BITMAP_LIGHTNING + 1, o->Position, 0.3f, Light, o, IWorldTime*0.1f);
				CreateSprite(BITMAP_LIGHTNING + 1, o->Position, 0.7f, Light, o, -IWorldTime*0.1f);

				Vector(Luminosity*0.2f, Luminosity*0.8f, Luminosity*0.2f, Light);
				AddTerrainLight(o->Position[0], o->Position[1], Light, 2, PrimaryTerrainLight);

				o->Angle[1] += 60.0f;

				if (o->LifeTime > 25 && (o->LifeTime % 2) == 0)
				{
					for (mu_int32 j = 0; j<2; ++j)
					{
						Vector((mu_float)(GetLargeRand(32) - 16), (mu_float)(GetLargeRand(64) - 32), (mu_float)(GetLargeRand(32) - 16), Position);
						VectorAdd(Position, o->Position, Position);

						Vector(0.4f, 1.0f, 0.2f, Light);
						CreateParticle(ThreadIndex, BITMAP_FLARE, Position, o->Angle, Light, 5, 0.2f);
					}
				}
			}
			else
			{
				CreateSprite(BITMAP_LIGHTNING + 1, o->Position, 0.5f, o->Light, o, IWorldTime*0.1f);
				CreateSprite(BITMAP_LIGHTNING + 1, o->Position, 1.0f, o->Light, o, -IWorldTime*0.1f);
				for (mu_int32 j = 0; j<4; ++j)
				{
					Vector((mu_float)(GetLargeRand(32) - 16), (mu_float)(GetLargeRand(64) - 32), (mu_float)(GetLargeRand(32) - 16), Position);
					VectorAdd(Position, o->Position, Position);
					CreateParticle(ThreadIndex, BITMAP_FLOWER01 + GetLargeRand(3), Position, o->Angle, o->Light);
				}
				o->Angle[1] += 30.0f;

				Vector(Luminosity*0.6f, Luminosity*0.8f, Luminosity*0.8f, Light);
				AddTerrainLight(o->Position[0], o->Position[1], Light, 2, PrimaryTerrainLight);
			}
		}
		else if (o->Type == MODEL_LACEARROW)
		{
			o->Angle[1] += 60.0f;

			if ((o->LifeTime % 2) == 0)
			{
				for (mu_int32 j = 0; j<3; ++j)
				{
					Vector((mu_float)(GetLargeRand(32) - 16), (mu_float)(GetLargeRand(64) - 32), (mu_float)(GetLargeRand(32) - 16), Position);
					VectorAdd(Position, o->Position, Position);

					Vector(0.4f, 0.2f, 1.0f, Light);
					CreateParticle(ThreadIndex, BITMAP_FLARE, Position, o->Angle, Light, 5, 0.2f);
				}
			}
			VectorCopy(o->Position, o->EyeLeft);
			CreateEffect(ThreadIndex, MODEL_WAVES, o->Position, o->Angle, o->Light, 3, nullptr, 0);

			Vector(Luminosity*0.6f, Luminosity*0.2f, Luminosity*0.8f, Light);
			AddTerrainLight(o->Position[0], o->Position[1], Light, 2, PrimaryTerrainLight);
		}
		else if (o->Type == MODEL_ARROW_WING)
		{
			CreateSprite(BITMAP_LIGHTNING + 1, o->Position, 0.5f, o->Light, o, IWorldTime*0.1f);
			CreateSprite(BITMAP_LIGHTNING + 1, o->Position, 1.0f, o->Light, o, -IWorldTime*0.1f);
			for (mu_int32 j = 0; j<4; ++j)
			{
				Vector((mu_float)(GetLargeRand(16) - 8), (mu_float)(GetLargeRand(16) - 8), (mu_float)(GetLargeRand(16) - 8), Position);
				VectorAdd(Position, o->Position, Position);
				CreateParticle(ThreadIndex, BITMAP_BUBBLE, Position, o->Angle, o->Light, 1);
			}
			CreateParticle(ThreadIndex, BITMAP_SMOKE, o->Position, o->Angle, o->Light, 0);

			Vector(Luminosity*0.6f, Luminosity*0.8f, Luminosity*0.8f, Light);
			AddTerrainLight(o->Position[0], o->Position[1], Light, 2, PrimaryTerrainLight);
		}
		CheckClientArrow(ThreadIndex, o);
		break;
	case MODEL_DARK_SCREAM_FIRE:
	case MODEL_DARK_SCREAM:
		{
			if (o->Type == MODEL_DARK_SCREAM_FIRE)
			{
				o->Scale -= 0.14f;
			}
			if (o->Type == MODEL_DARK_SCREAM)
			{
				o->Scale -= 0.04f;
			}
			if (o->Scale < 0.1f)
			{
				o->Scale = 0.0f;
			}

			o->Position[2] = RequestTerrainHeight(o->Position[0], o->Position[1]) + 3.0f;

			CreateParticle(ThreadIndex, BITMAP_FLAME, o->Position, o->Angle, o->Light, 8, (o->Scale - 0.4f)*3.5f);
			if (o->Type == MODEL_DARK_SCREAM)
			{
				CheckClientArrow(ThreadIndex, o);
			}
		}
		break;
	case MODEL_CURSEDTEMPLE_HOLYITEM:
		{
			if (!o->Owner->Flags.Get(OBJECT::eOF_LIVE))
			{
				o->Flags.Clear(OBJECT::eOF_LIVE);
				break;
			}

			if (o->LifeTime < 10) o->LifeTime = 999999;

			MUModel *b = &MODELS::Data[o->Owner->Type];
			EVector3 vRelativePos, vWorldPos;
			Vector(70.0f, 0.0f, 0.0f, vRelativePos);

			VectorCopy(o->Owner->Position, b->BodyOrigin(ThreadIndex));
			b->TransformPosition(ThreadIndex, o->Owner->BoneTransform[b->GetBoneIndex(20)], vRelativePos, vWorldPos, true);

			Vector(1.0f, 1.0f, 1.0f, o->Light);
			VectorCopy(vWorldPos, o->Position);

			b->PlayAnimation(ThreadIndex, &o->AnimationFrame, &o->PriorAnimationFrame, &o->PriorAction, o->Velocity / 3.0f);
		}
		break;
	case MODEL_CURSEDTEMPLE_PRODECTION_SKILL:
		{
			if (!o->Owner->Flags.Get(OBJECT::eOF_LIVE))
			{
				o->Flags.Clear(OBJECT::eOF_LIVE);
				break;
			}

			if (o->LifeTime < 10) o->LifeTime = 999999;

			EVector3 RelativePos, Position;
			MUModel *herobmd = &MODELS::Data[o->Owner->Type];
			VectorCopy(o->Owner->Position, herobmd->BodyOrigin(ThreadIndex));
			Vector(-23.0f, 0.0f, 0.0f, RelativePos);
			herobmd->TransformPosition(ThreadIndex, o->Owner->BoneTransform[herobmd->GetBoneIndex(2)], RelativePos, Position, true);

			Vector(1.0f, 1.0f, 1.0f, o->Light);
			VectorCopy(Position, o->Position);
			VectorCopy(o->Owner->Angle, o->Angle);

			o->Alpha = 0.3f;
			o->Scale = 1.0f;

			MUModel *effbmd = &MODELS::Data[o->Type];

			effbmd->BodyLight(ThreadIndex) = o->Light;
			effbmd->PlayAnimation(ThreadIndex, &o->AnimationFrame, &o->PriorAnimationFrame, &o->PriorAction, o->Velocity / 3.0f);

			CreateEffect(ThreadIndex, BITMAP_SHOCK_WAVE, o->Owner->Position, o->Angle, o->Light, 10, o->Owner);
		}
		break;
	case MODEL_CURSEDTEMPLE_RESTRAINT_SKILL:
		{
			if (!o->Owner->Flags.Get(OBJECT::eOF_LIVE))
			{
				o->Flags.Clear(OBJECT::eOF_LIVE);
				break;
			}

			if (o->LifeTime < 10) o->LifeTime = 999999;

			Vector(1.0f, 1.0f, 1.0f, o->Light);
			VectorCopy(o->Owner->Position, o->Position);
			VectorCopy(o->Owner->Angle, o->Angle);
			o->Alpha = 0.6f;
			o->Scale = 1.0f;

			MUModel *b = &MODELS::Data[o->Type];
			b->PlayAnimation(ThreadIndex, &o->AnimationFrame, &o->PriorAnimationFrame, &o->PriorAction, o->Velocity / 3.0f);

			EVector3 vRelativePos, Light;
			Vector(0.4f, 0.4f, 0.8f, Light);
			Vector(0.0f, 0.0f, 0.0f, vRelativePos);
		}
		break;
	case MODEL_ARROW_SPARK:
		{
			o->Angle[1] += 35.0f;
			CheckClientArrow(ThreadIndex, o);
		}
		break;
	case MODEL_ARROW_RING:
		{
			VectorCopy(o->Position, o->EyeLeft);
			Vector(0.0f, 1.0f, 0.1f, o->Light);
			CreateEffect(ThreadIndex, MODEL_WAVES, o->Position, o->Angle, o->Light, 4, nullptr, 0);

			Vector(Luminosity*0.6f, Luminosity*0.2f, Luminosity*0.8f, Light);
			AddTerrainLight(o->Position[0], o->Position[1], Light, 2, PrimaryTerrainLight);

			CheckClientArrow(ThreadIndex, o);
		}
		break;
	case MODEL_ARROW_TANKER:
		{
			Vector(1.0f, 0.0f, 0.0f, o->Light);
			AddTerrainLight(o->Position[0], o->Position[1], o->Light, 2, PrimaryTerrainLight);
		}
		break;
	case MODEL_ARROW_BOMB:
		{
			Vector(1.0f, 0.6f, 0.4f, Light);
			CreateSprite(BITMAP_LIGHT, o->Position, 1.0f, Light, o, IWorldTime*0.1f);
			CreateSprite(BITMAP_LIGHT, o->Position, 2.0f, Light, o, -IWorldTime*0.1f);

			for (mu_int32 j = 0; j<4; ++j)
			{
				Vector((mu_float)(GetLargeRand(16) - 8), (mu_float)(GetLargeRand(16) - 8), (mu_float)(GetLargeRand(16) - 8), Position);
				VectorAdd(Position, o->Position, Position);
				CreateParticle(ThreadIndex, BITMAP_BUBBLE, Position, o->Angle, o->Light, 1);
			}
			MoveJump(o);
			if (o->LifeTime == 1)
			{
				CreateBomb(ThreadIndex, o->Position, true);
			}

			Vector(Luminosity*0.6f, Luminosity*0.8f, Luminosity*0.8f, Light);
			AddTerrainLight(o->Position[0], o->Position[1], Light, 2, PrimaryTerrainLight);
			CheckClientArrow(ThreadIndex, o);
		}
		break;
	case MODEL_ARROW_DARKSTINGER:
		{
			CheckClientArrow(ThreadIndex, o);

			MUModel *pModel = &MODELS::Data[o->Type];
			EVector3 vPos;

			Vector(0.6f, 0.7f, 0.9f, Light);
			pModel->Animation(ThreadIndex, ThreadInfo.BoneTransform, o->AnimationFrame, o->PriorAnimationFrame, o->PriorAction, o->Angle, o->HeadAngle, false, false);
			pModel->TransformByObjectBone(ThreadIndex, vPos, o, 0);

			EBone Matrix;
			EVector3 Angle;
			EVector3 Position;
			VectorCopy(o->Angle, Angle);
			AngleMatrix(Angle, Matrix);
			VectorRotate(o->Direction, Matrix, Position);
			VectorAdd(vPos, Position, vPos);

			CreateSprite(BITMAP_LIGHT, vPos, 3.0f, Light, o);
			CreateSprite(BITMAP_LIGHT, vPos, 2.0f, Light, o);


			if (o->LifeTime == 30 || o->LifeTime == 28 || o->LifeTime == 26 || o->LifeTime == 24)
			{
				mu_int32 iNumCreateFeather = GetLargeRand(3);
				Vector(0.6f, 0.7f, 0.9f, Light);
				pModel->TransformByObjectBone(ThreadIndex, vPos, o, 1);
				for (mu_int32 i = 0; i<iNumCreateFeather; ++i)
				{
					CreateEffect(ThreadIndex, MODEL_FEATHER, vPos, o->Angle, Light, 0, nullptr, -1, 0, 0, 0, 0.6f);
					CreateEffect(ThreadIndex, MODEL_FEATHER, vPos, o->Angle, Light, 1, nullptr, -1, 0, 0, 0, 0.6f);
				}
			}

			if (o->LifeTime == 30)
			{
				pModel->TransformByObjectBone(ThreadIndex, vPos, o, 1);
				Vector(0.4f, 0.4f, 0.9f, Light);
				CreateJoint(ThreadIndex, BITMAP_FLARE + 1, vPos, vPos, o->Angle, 18, o, 90.0f, 40, 0, 0, -1, &Light);
			}

		}break;
	case MODEL_ARROW_GAMBLE:
		{
			EVector3 vLight;
			Vector(0.2f, 0.8f, 0.5f, vLight);

			o->Angle[1] += 60.0f;

			for (mu_int32 j = 0; j<2; ++j)
			{
				Vector((mu_float)(GetLargeRand(32) - 16), (mu_float)(GetLargeRand(64) - 32), (mu_float)(GetLargeRand(32) - 16), Position);
				VectorAdd(Position, o->Position, Position);

				CreateParticle(ThreadIndex, BITMAP_SPARK + 1, Position, o->Angle, vLight, 30, 1.0f);
			}

			VectorCopy(o->Position, o->EyeLeft);
			CreateParticle(ThreadIndex, BITMAP_SMOKE, o->Position, o->Angle, vLight, 24, 1.0f);

			Vector(Luminosity*0.2f, Luminosity*0.8f, Luminosity*0.5f, vLight);
			AddTerrainLight(o->Position[0], o->Position[1], vLight, 2, PrimaryTerrainLight);

			CheckClientArrow(ThreadIndex, o);
		}break;
	case MODEL_FEATHER_FOREIGN:
		{
			switch (o->SubType)
			{
			case 4:
				{
					o->Light[0] *= 0.97f;
					o->Light[1] *= 0.97f;
					o->Light[2] *= 0.97f;

					o->Alpha *= 0.97f;

					VectorAdd(o->EyeRight, o->Angle, o->Angle);
				}
				break;
			}
		}break;
	case MODEL_DUNGEON_STONE01:
		o->Position[2] += o->Gravity;
		o->Gravity -= 1.0f;
		Height = RequestTerrainHeight(o->Position[0], o->Position[1]);
		if (o->Position[2] < Height)
		{
			o->Position[2] = Height;
			o->Gravity = -o->Gravity*0.4f;
			o->LifeTime -= 4;
			o->Direction[1] -= 2.0f;
		}
		break;
	case MODEL_ICE:
		switch (o->SubType)
		{
		case 0:
			if (o->AnimationFrame >= 5.0f)
			{
				o->Velocity = 0.0f;
				if (GetLargeRand(2) == 0)
				{
					EVector3 Position;
					Vector(o->Position[0] + (mu_float)(GetLargeRand(64) - 32),
						o->Position[1] + (mu_float)(GetLargeRand(64) - 32),
						o->Position[2] + (mu_float)(GetLargeRand(128) + 32), Position);
					CreateParticle(ThreadIndex, BITMAP_SMOKE, Position, o->Angle, o->Light);
				}
				o->Alpha -= 0.05f;
				if (o->Alpha < 0.0f)
					o->Flags.Clear(OBJECT::eOF_LIVE);
			}
			Vector(-Luminosity*0.4f, -Luminosity*0.3f, -Luminosity*0.2f, Light);
			AddTerrainLight(o->Position[0], o->Position[1], Light, 2, PrimaryTerrainLight);
			break;

		case 1:
		case 2:
			if (o->Owner != nullptr)
			{
				if (o->Owner->Flags.Get(OBJECT::eOF_LIVE) == true && g_isCharacterBuff(o->Owner, eDeBuff_Harden))
				{
					if (o->Owner->Type == MODEL_PLAYER)
					{
						if ((o->Owner->CurrentAction >= PLAYER_WALK_MALE && o->Owner->CurrentAction <= PLAYER_RUN_RIDE_WEAPON) ||
							(o->Owner->CurrentAction >= PLAYER_FLY_RIDE  && o->Owner->CurrentAction <= PLAYER_FLY_RIDE_WEAPON))
						{
							o->AnimationFrame = 0.0f;
							o->PriorAnimationFrame = 0.0f;
							o->HiddenMesh = -2;
						}
						else
						{
							o->HiddenMesh = -1;
						}
					}
					else
					{
						if (o->Owner->CurrentAction != 2)
						{
							o->HiddenMesh = -1;
						}
						else
						{
							o->PriorAnimationFrame = 0.0f;
							o->AnimationFrame = 0.0f;
							o->HiddenMesh = -2;
						}
					}
					VectorCopy(o->Owner->Position, o->Position);
					if (o->AnimationFrame >= 4.0f) o->AnimationFrame = 4.0f;
					o->LifeTime = 10;
				}
				else
				{
					o->Alpha -= 0.05f;
				}

				if (o->SubType == 1)
				{
					o->HeadAngle[2] += 20.0f;
					o->Gravity = EMath::Sin(FWorldTime*0.01f)*20.0f + 30;

					Vector(60.0f, 0.0f, 0.0f, p);
					AngleMatrix(o->HeadAngle, Matrix);
					VectorRotate(p, Matrix, Position);
					VectorAdd(o->Owner->Position, Position, Position);

					Position[2] += o->Gravity;
					Vector(1.0f, 1.0f, 1.0f, Light);
					CreateParticle(ThreadIndex, BITMAP_FIRE + 2, Position, o->Angle, Light, 10, 2.0f);
				}
			}
			else
			{
				o->Flags.Clear(OBJECT::eOF_LIVE);
			}
			break;
		}
		break;
	case MODEL_FIRE:
		if (o->SubType == 0 || o->SubType == 2 || o->SubType == 4 || o->SubType == 6 || o->SubType == 7 || o->SubType == 8)
		{
			mu_boolean success = false;
			mu_float Height;
			if (o->SubType == 4)
			{
				mu_int32 PositionX = (mu_int32)(o->Position[0] * g_TerrainScaleRev);
				mu_int32 PositionY = (mu_int32)(o->Position[1] * g_TerrainScaleRev);
				mu_int32 WallIndex = TERRAIN_INDEX_REPEAT(PositionX, PositionY);
				mu_int32 Wall = TerrainWall[WallIndex] & TW_NOGROUND;

				if (Wall != TW_NOGROUND)
				{
					Height = RequestTerrainHeight(o->Position[0], o->Position[1]);
					if (o->Position[2] < Height)
					{
						success = true;
						EarthQuake.Store((mu_float)(GetLargeRand(4) - 4)*0.1f);

						for (mu_int32 i = 0; i<MAX_CHARACTERS_CLIENT; ++i)
						{
							CHARACTER *tc = &CharactersClient[i];
							OBJECT    *to = &tc->Object;

							if (to->Flags.Get(OBJECT::eOF_LIVE | OBJECT::eOF_VISIBLE) == false ||
								tc == Hero ||
								tc->Dead != 0)
							{
								continue;
							}

							mu_float dx = o->Position[0] - to->Position[0];
							mu_float dy = o->Position[1] - to->Position[1];
							mu_float Distance = EMath::Sqrt(dx*dx + dy*dy);
							if (Distance <= 200)
							{
								if (to->Type == MODEL_PLAYER)
								{
									if (tc->Helper.Type == MODEL_HELPER + 37)
										SetAction_Fenrir_Damage(tc, to);
									else
										SetAction(to, PLAYER_SHOCK);
								}
								else
								{
									SetAction(to, MONSTER01_SHOCK);
								}
							}
						}
					}
				}
				else
				{
					if (o->Position[2]<-200)
					{
						success = true;
						Height = o->Position[2];
					}
				}
			}
			else
			{
				if (o->SubType == 6 || o->SubType == 7 || o->SubType == 8)
				{
					Vector(0.5f, 0.5f, 1.0f, Light);
					CreateSprite(BITMAP_LIGHT, o->Position, 3.0f, Light, o);
					Vector(1.0f, 1.0f, 1.0f, Light);
					CreateSprite(BITMAP_SHINY + 1, o->Position, 3.0f, Light, o, (mu_float)(GetLargeRand(360)), 1);
					CreateSprite(BITMAP_SHINY + 1, o->Position, 4.0f, o->Light, o, (mu_float)(GetLargeRand(360)));

					VectorCopy(o->HeadAngle, o->Angle);
				}
				mu_float AddHeight = 0.0f;
				if (InHellas())
				{
					AddHeight = 50.0f;
				}
				Height = RequestTerrainHeight(o->Position[0], o->Position[1]) + AddHeight;
				if (o->Position[2] < Height)
				{
					success = true;
				}
			}

			if (success)
			{
				o->Position[2] = Height;
				Vector(0.0f, 0.0f, 0.0f, o->Direction);
				EVector3 Position;
				Vector(o->Position[0], o->Position[1], o->Position[2] + 80.0f, Position);
				if (o->SubType == 6 || o->SubType == 7 || o->SubType == 8)
				{
					mu_uint8 smokeNum = 15;

					Vector(0.0f, 0.5f, 0.0f, Light);
					Vector(0.0f, 0.0f, 0.0f, Angle);
					CreateEffect(ThreadIndex, MODEL_SKILL_INFERNO, o->Position, Angle, Light, 2, o, 30, 0);

					if (o->SubType == 7)
					{
						smokeNum = 7;
					}
					else if (o->SubType == 8)
					{
						smokeNum = 5;
					}

					for (mu_int32 i = 0; i<smokeNum; ++i)
					{
						Position[0] = o->Position[0] + (GetLargeRand(160) - 80);
						Position[1] = o->Position[1] + (GetLargeRand(160) - 100);
						Position[2] = o->Position[2] + 50;

						Vector(0.1f, 0.5f, 0.1f, Light);
						CreateParticle(ThreadIndex, BITMAP_SMOKE, Position, o->Angle, Light, 11, (mu_float)(GetLargeRand(32) + 80)*0.025f);
					}

					if (o->SubType != 7 && o->SubType != 8)
					{
						for (mu_int32 j = 0; j<6; ++j)
						{
							CreateEffect(ThreadIndex, MODEL_STONE1 + GetLargeRand(2), o->Position, o->Angle, Light);
						}
					}
					PlayBackground(SOUND_DEATH_POISON2);
				}
				else
				{
					for (mu_int32 j = 0; j<6; ++j)
					{
						CreateEffect(ThreadIndex, MODEL_STONE1 + GetLargeRand(2), o->Position, o->Angle, o->Light);
					}
				}
				CreateParticle(ThreadIndex, BITMAP_EXPLOTION, Position, o->Angle, Light);

				o->Flags.Clear(OBJECT::eOF_LIVE);
			}
		}
		else if (o->SubType == 5)
		{
			o->Position[2] += o->Gravity;
			o->Gravity -= 2.0f;

			Height = RequestTerrainHeight(o->Position[0], o->Position[1]);
			if (o->Position[2] < Height)
			{
				o->Gravity = 10.0f;
				o->Position[2] += 10.0f;
				o->Direction[1] *= 0.5f;
				o->Scale *= 1.1f;
			}

			mu_int32 PositionX = (mu_int32)(o->Position[0] * g_TerrainScaleRev);
			mu_int32 PositionY = (mu_int32)(o->Position[1] * g_TerrainScaleRev);
			mu_int32 WallIndex = TERRAIN_INDEX_REPEAT(PositionX, PositionY);
			mu_int32 Wall = TerrainWall[WallIndex] & TW_NOGROUND;

			if (o->LifeTime == 1 || Wall == TW_NOGROUND)
			{
				o->Position[2] = Height;

				Vector(0.0f, 0.0f, 0.0f, o->Direction);
				EVector3 Position;
				Vector(o->Position[0], o->Position[1], o->Position[2] + 80.0f, Position);
				CreateParticle(ThreadIndex, BITMAP_EXPLOTION, Position, o->Angle, Light);
				for (mu_int32 j = 0; j<6; ++j)
				{
					CreateEffect(ThreadIndex, MODEL_STONE1 + GetLargeRand(2), o->Position, o->Angle, o->Light);
				}
				o->Flags.Clear(OBJECT::eOF_LIVE);
			}
		}
		else if (o->SubType == 9)
		{
			CreateParticle(ThreadIndex, BITMAP_SMOKE, o->Position, o->Angle, o->Direction, 36, 1.0f + o->Scale, o);
			CreateParticle(ThreadIndex, BITMAP_SMOKE, o->Position, o->Angle, o->Direction, 37, 2.0f + o->Scale, o);
			Vector(1.0f, 0.2f, 0.2f, o->Light);
			AddTerrainLight(o->Position[0], o->Position[1], o->Light, 4, PrimaryTerrainLight);

			mu_float Height = RequestTerrainHeight(o->Position[0], o->Position[1]);
			if (o->Position[2] < Height)
			{
				o->Flags.Clear(OBJECT::eOF_LIVE);
				break;
			}
		}

		if (o->SubType == 5)
		{
			o->HiddenMesh = 0;
			o->BlendMeshLight = 0.0f;

			Vector(Luminosity*1.0f, Luminosity*0.1f, Luminosity*0.0f, Light);
			AddTerrainLight(o->Position[0], o->Position[1], Light, 2, PrimaryTerrainLight);
			if ((GetLargeRand(5)) != 0)
			{
				CreateParticle(ThreadIndex, BITMAP_POUNDING_BALL, o->Position, o->Angle, Light);
			}
		}
		else
		{
			if (o->SubType == 3)
			{
				o->HiddenMesh = 0;
				o->BlendMeshLight = 0.0f;
			}
			else if (o->SubType == 0)
				o->BlendMeshLight = (mu_float)(GetLargeRand(4) + 4)*0.1f;
			else if (o->SubType == 6 || o->SubType == 7)
				o->BlendMeshLight = (mu_float)(GetLargeRand(4) + 4)*0.1f;
			else
				o->BlendMeshLight = 0.0f;

			if (o->SubType == 6 || o->SubType == 7 || o->SubType == 8)
			{
				Vector(Luminosity*0.1f, Luminosity*1.0f, Luminosity*0.0f, Light);
			}
			else
			{
				Vector(Luminosity*1.0f, Luminosity*0.1f, Luminosity*0.0f, Light);
			}
			AddTerrainLight(o->Position[0], o->Position[1], Light, 2, PrimaryTerrainLight);
			if (o->SubType == 6 || o->SubType == 7)
			{
				for (mu_int32 j = 0; j<2; ++j)
				{
					Position[0] = o->Position[0] + GetLargeRand(50) - 25;
					Position[1] = o->Position[1];
					Position[2] = o->Position[2];
					CreateParticle(ThreadIndex, BITMAP_FIRE, Position, o->Angle, Light, 5);
				}
			}
			else if (o->SubType != 3)
			{
				CreateParticle(ThreadIndex, BITMAP_FIRE, o->Position, o->Angle, Light, 5);
			}
			else
			{
				EVector3 Pos;
				EVector3 vRot;
				GetMagicScrew(iIndex * 5371, vRot);
				VectorScale(vRot, 50.0f, vRot);
				VectorCopy(o->Position, Pos);
				VectorAdd(Pos, vRot, Pos);

				Vector(0.0f, 0.0f, 0.0f, Angle);
				Vector(1.0f, 1.0f, 1.0f, Light);
				CreateParticle(ThreadIndex, BITMAP_FIRE, Pos, Angle, Light, 8, o->Scale*2.5f);
				CreateSprite(BITMAP_SHINY + 1, Pos, o->Scale*3.0f, Light, nullptr, (mu_float)(GetLargeRand(360)));
			}

			if (o->SubType == 1)
			{
				CheckTargetRange(ThreadIndex, o);
			}
		}
		break;
	case MODEL_WARCRAFT:
		VectorCopy(o->Owner->Position, o->Position);
		break;
	case BITMAP_FIRECRACKERRISE:
		if (0 == (o->LifeTime % 5) && (0 == (GetLargeRand(3))))
		{
			CreateEffect(ThreadIndex, BITMAP_FIRECRACKER, o->Position, o->Angle, o->Light);
		}
		break;
	case BITMAP_FIRECRACKER:
		if (1 == o->LifeTime)
		{
			mu_int32 iSubType = GetLargeRand(30);
			Vector((GetLargeRand(3))*.3f + .4f, (GetLargeRand(4))*.1f, .0f, Light);

			for (mu_int32 j = 0; j < 80; ++j)
			{
				CreateParticle(ThreadIndex, BITMAP_FIRECRACKER, o->Position, o->Angle, Light, iSubType);
			}
			PlayBackground(SOUND_FIRECRACKER2, o);
		}
		CreateParticle(ThreadIndex, BITMAP_FIRECRACKER, o->Position, o->Angle, o->Light, -1);
		Vector(Luminosity*.4f, Luminosity*0.3f, Luminosity*0.2f, Light);
		AddTerrainLight(o->Position[0], o->Position[1], Light, 1, PrimaryTerrainLight);
		break;
	case BITMAP_FIRECRACKER0001:
		{
			if (o->LifeTime == 1 || o->LifeTime == 9 || o->LifeTime == 17
				|| o->LifeTime == 24 || o->LifeTime == 31)
			{
				Vector(o->Position[0] + (GetLargeRand(200) - 100), o->Position[1] + (GetLargeRand(200) - 100),
					o->Position[2], Position);
				CreateJoint(ThreadIndex, BITMAP_JOINT_SPIRIT, Position, Position, o->Angle, 25, o, 1.0f, -1, o->SubType);
			}
		}
		break;
	case BITMAP_FIRECRACKER0002:
		{
			if (GetLargeRand(5) == 0)
			{
				EVector3 vLight;
				Vector(0.3f + (GetLargeRand(700))*0.001f, 0.3f + (GetLargeRand(700))*0.001f, 0.3f + (GetLargeRand(700))*0.001f, vLight);
				Vector(o->Position[0] + (GetLargeRand(300) - 150), o->Position[1] + (GetLargeRand(300) - 150),
					o->Position[2], Position);
				mu_float fScale = 0.5f + (GetLargeRand(5))*0.1f;
				CreateEffect(ThreadIndex, BITMAP_FIRECRACKER0003, Position, o->Angle, vLight, 0, o, -1, 0, 0, 0, fScale);
			}
		}
		break;
	case BITMAP_FIRECRACKER0003:
		{
			mu_int32 iFrame;
			if (o->LifeTime > 15 - 8)
			{
				iFrame = (15 - o->LifeTime) / 8.0f * 7;
				if (iFrame >= 7) iFrame = 6;
			}
			else
			{
				iFrame = 6;
				o->Position[2] -= 1.0f;
				o->Light[0] /= 1.05f;
				o->Light[1] /= 1.05f;
				o->Light[2] /= 1.05f;
				o->Scale *= 1.02f;
			}
			CreateSprite(BITMAP_FIRECRACKER0001 + iFrame, o->Position, o->Scale, o->Light, o, o->Angle[2]);
		}
		break;
	case BITMAP_SWORD_FORCE:
		if (o->LifeTime == 30)
		{
			VectorCopy(o->Position, Position);
			Position[2] += 100.0f;
			if (o->SubType == 1)
			{
				EVector3 Light;
				Vector(1.0f, 1.0f, 1.0f, Light);
				CreateJoint(ThreadIndex, BITMAP_JOINT_FORCE, Position, Position, o->HeadAngle, 10, o->Owner, 150.0f, o->PKKey, o->Skill, 0, -1, &Light);
			}
			else if (o->SubType == 0)
				CreateJoint(ThreadIndex, BITMAP_JOINT_FORCE, Position, Position, o->HeadAngle, 0, o->Owner, 150.0f, o->PKKey, o->Skill);
			else
				CreateJoint(ThreadIndex, BITMAP_JOINT_FORCE, Position, Position, o->HeadAngle, 8, o->Owner, 150.0f, o->PKKey, o->Skill);
		}
		break;

	case BITMAP_BLIZZARD:
		if (o->LifeTime <= 15)
		{
			if (o->SubType == 0)
			{
				o->Position[0] = o->StartPosition[0] + EMath::Sin((GetLargeRand(1000))*0.01f)*10.0f;
				o->Position[1] = o->StartPosition[1] + EMath::Sin((GetLargeRand(1000))*0.01f)*10.0f;
				o->Position[2] += o->Gravity;
				o->Gravity -= 2.0f;

				o->StartPosition[0] -= 10.0f;

				CreateParticle(ThreadIndex, BITMAP_FIRE + 2, o->Position, o->Angle, Light, 7, o->Scale);

				o->Light[0] += 0.1f;
				o->Light[1] = o->Light[0];
				o->Light[2] = o->Light[0];
				CreateSprite(BITMAP_SHINY + 1, o->Position, (mu_float)(GetLargeRand(4) + 4)*0.2f, o->Light, o, (mu_float)(GetLargeRand(360)));
				CreateSprite(BITMAP_LIGHT, o->Position, 1.0f, o->Light, o, (mu_float)(GetLargeRand(360)));
			}
			else if (o->SubType == 1)
			{
				o->Position[0] = o->StartPosition[0];
				o->Position[1] = o->StartPosition[1];
				o->Position[2] += o->Gravity;
				o->Gravity -= 2.0f;

				o->StartPosition[0] -= 10.0f;

				CreateParticle(ThreadIndex, BITMAP_FIRE + 2, o->Position, o->Angle, Light, 11, o->Scale);

				o->Light[0] += 0.1f;
				o->Light[1] = o->Light[0];
				o->Light[2] = o->Light[0];
				CreateSprite(BITMAP_SHINY + 1, o->Position, (mu_float)(GetLargeRand(4) + 4)*0.2f, o->Light, o, (mu_float)(GetLargeRand(360)));
				CreateSprite(BITMAP_LIGHT, o->Position, 1.0f, o->Light, o, (mu_float)(GetLargeRand(360)));

				Height = RequestTerrainHeight(o->Position[0], o->Position[1]);
				if (o->Position[2] < Height)
				{
					Vector(0.24f, 0.28f, 0.8f, Light);
					VectorCopy(o->Position, Position);
					Position[2] += 50.0f;
					CreateParticle(ThreadIndex, BITMAP_SMOKE, Position, o->Angle, Light, 11, (mu_float)(GetLargeRand(32) + 80)*0.025f);

					if ((GetLargeRand(5)) == 0)
						CreateEffect(ThreadIndex, MODEL_ICE_SMALL, Position, o->Angle, o->Light);
				}
			}
		}
		break;

	case BITMAP_SHOTGUN:
		{
			Vector(Luminosity*0.5f, Luminosity*0.5f, Luminosity*0.8f, Light);
			AddTerrainLight(o->Position[0], o->Position[1], Light, 2, PrimaryTerrainLight);

			Vector(1.0f, 1.0f, 1.0f, Light);
			VectorCopy(o->Angle, Angle);
			Angle[2] += 90.0f;

			o->Scale = ((15 - o->LifeTime) / 20.0f)*(GetLargeRand(3) + 2);

			AngleMatrix(Angle, Matrix);
			Vector(0.0f, 20.0f, 0.0f, p);
			VectorRotate(p, Matrix, Position);
			VectorAdd(o->Position, Position, o->StartPosition);
			CreateParticle(ThreadIndex, BITMAP_FIRE + 2, o->StartPosition, o->Angle, Light, 10, o->Scale);

			Vector(0.0f, -20.0f, 0.0f, p);
			VectorRotate(p, Matrix, Position);
			VectorAdd(o->Position, Position, o->StartPosition);
			CreateParticle(ThreadIndex, BITMAP_FIRE + 2, o->StartPosition, o->Angle, Light, 10, o->Scale);

			if (o->LifeTime == 1)
			{
				CreateBomb2(ThreadIndex, o->Position, false);
			}
		}
		break;

	case MODEL_GATE:
	case MODEL_GATE + 1:
		VectorAdd(o->Position, o->Direction, o->Position);
		VectorScale(o->Direction, 0.9f, o->Direction);
		o->Position[2] += o->Gravity;

		if (o->SubType == 1)
		{
			VectorScale(o->Direction, 1.1f, o->Direction);
		}

		o->Gravity -= 4.0f;
		Height = RequestTerrainHeight(o->Position[0], o->Position[1]);
		if (o->Position[2] < Height)
		{
			o->Position[2] = Height;
			o->Gravity = -o->Gravity*0.5f;
			o->LifeTime -= 5;
			o->Angle[0] -= o->Scale*128.0f;
		}
		else
			o->Angle[0] -= o->Scale*16.0f;

		if ((GetLargeRand(10)) == 0)
		{
			CreateParticle(ThreadIndex, BITMAP_SMOKE + 1, Position, o->Angle, Light);
		}
		break;

	case MODEL_STONE_COFFIN:
	case MODEL_STONE_COFFIN + 1:
		VectorAdd(o->Position, o->Direction, o->Position);
		VectorScale(o->Direction, 0.9f, o->Direction);
		o->Position[2] += o->Gravity;

		if (o->SubType == 1)
		{
			VectorScale(o->Direction, 1.1f, o->Direction);
		}

		o->Gravity -= 3.0f;
		Height = RequestTerrainHeight(o->Position[0], o->Position[1]);
		if (o->Position[2] < Height)
		{
			o->Position[2] = Height;
			o->Gravity = -o->Gravity*0.2f;
			o->LifeTime -= 5;
			o->Angle[0] -= o->Scale*128.0f;
		}
		else
			o->Angle[0] -= o->Scale*16.0f;

		o->Alpha = o->LifeTime / 10.0f;
		if ((GetLargeRand(10)) == 0)
		{
			CreateParticle(ThreadIndex, BITMAP_SMOKE + 1, Position, o->Angle, Light);
		}
		break;
	case MODEL_SHINE:
		if (o->SubType == 0 && o->Owner != nullptr && o->LifeTime>10)
		{
			Position[0] = (mu_float)(GetLargeRand(128) - 64) - 50.0f;
			Position[1] = Position[0] + 100.0f;
			Position[2] = 0.0f;
			VectorAdd(Position, o->Owner->Position, Position);
			Position[2] += 360.0f;

			mu_float Scale = (GetLargeRand(30) + 50) / 100.0f;
			CreateParticle(ThreadIndex, BITMAP_SHINY, Position, o->Angle, o->Light, 2, Scale);
		}
		break;
	case MODEL_BLIZZARD:
		if (o->SubType == 0)
		{
			o->Position[0] = o->StartPosition[0] + EMath::Sin((GetLargeRand(1000))*0.01f)*10.0f;
			o->Position[1] = o->StartPosition[1] + EMath::Sin((GetLargeRand(1000))*0.01f)*10.0f;
			o->Position[2] += o->Gravity;
			o->Gravity -= GetLargeRand(5);

			o->StartPosition[0] -= 10.0f;

			CreateParticle(ThreadIndex, BITMAP_SMOKE, o->Position, o->Angle, Light, 0, 1.5f);
			if ((GetLargeRand(2)) == 0)
			{
				CreateParticle(ThreadIndex, BITMAP_ENERGY, o->Position, o->Angle, Light, 1, 0.5f);
			}
			else
			{
				CreateParticle(ThreadIndex, BITMAP_FIRE + 2, o->Position, o->Angle, Light, 7, o->Scale);
			}

			o->Light[0] += 0.1f;
			o->Light[1] = o->Light[0];
			o->Light[2] = o->Light[0];
			CreateSprite(BITMAP_SHINY + 1, o->Position, (mu_float)(GetLargeRand(4) + 4)*0.2f, o->Light, o, (mu_float)(GetLargeRand(360)));
			CreateSprite(BITMAP_LIGHT, o->Position, 1.0f, o->Light, o, (mu_float)(GetLargeRand(360)));

			Height = RequestTerrainHeight(o->Position[0], o->Position[1]);
			if (o->Position[2] < Height)
			{
				Vector(0.24f, 0.28f, 0.8f, Light);
				VectorCopy(o->Position, Position);
				Position[2] += 50.0f;
				CreateParticle(ThreadIndex, BITMAP_SMOKE, Position, o->Angle, Light, 11, (mu_float)(GetLargeRand(32) + 80)*0.025f);

				if ((GetLargeRand(5)) == 0)
				{
					CreateEffect(ThreadIndex, MODEL_ICE_SMALL, Position, o->Angle, o->Light);
				}

				CreateEffect(ThreadIndex, MODEL_BLIZZARD, Position, o->Angle, o->Light, 1, nullptr, o->PKKey);

				o->Flags.Clear(OBJECT::eOF_LIVE);
			}
		}
		else if (o->SubType == 1)
		{
			o->BlendMeshLight /= 1.1f;

			if (o->LifeTime == 18 && o->PKKey == 1)
				PlayBackground(SOUND_SUDDEN_ICE2);
		}
		else if (o->SubType == 2)
		{
			o->Position[0] = o->StartPosition[0] + EMath::Sin((GetLargeRand(1000))*0.01f)*50.0f;
			o->Position[1] = o->StartPosition[1] + EMath::Sin((GetLargeRand(1000))*0.01f)*50.0f;
			o->Position[2] += o->Gravity;
			o->Gravity -= GetLargeRand(5);

			o->StartPosition[0] -= 10.0f;

			Height = RequestTerrainHeight(o->Position[0], o->Position[1]);
			if (o->Position[2]>Height)
			{
				if ((GetLargeRand(2)) == 0)
				{
					CreateParticle(ThreadIndex, BITMAP_FIRE + 2, o->Position, o->Angle, Light, 7, o->Scale);
				}

				o->Light[0] += 0.1f;
				o->Light[1] = o->Light[0];
				o->Light[2] = o->Light[0];
				CreateSprite(BITMAP_SHINY + 1, o->Position, (mu_float)(GetLargeRand(4) + 4)*0.2f, o->Light, o, (mu_float)(GetLargeRand(360)));
				CreateSprite(BITMAP_LIGHT, o->Position, 1.0f, o->Light, o, (mu_float)(GetLargeRand(360)));
			}
		}
		break;

	case MODEL_ARROW_DRILL:
		if (o->SubType == 0 || o->SubType == 2)
		{
			o->Angle[1] += 30.0f;

			CreateParticle(ThreadIndex, BITMAP_SMOKE, o->Position, o->Angle, Light, 13);
			CreateParticle(ThreadIndex, BITMAP_SMOKE, o->Position, o->Angle, Light, 13);
			CreateParticle(ThreadIndex, BITMAP_SMOKE, o->Position, o->Angle, Light, 13);

			if (o->LifeTime == 1)
			{
				CreateBomb(ThreadIndex, o->Position, true);
			}

			VectorCopy(o->Position, o->EyeLeft);
			VectorCopy(o->Angle, Angle);
			AngleMatrix(Angle, Matrix);
			VectorRotate(o->Direction, Matrix, Position);
			VectorAdd(o->EyeLeft, Position, o->EyeLeft);

			Vector(Luminosity*1.0f, Luminosity*0.4f, Luminosity*0.2f, Light);
			AddTerrainLight(o->Position[0], o->Position[1], Light, 2, PrimaryTerrainLight);
			CheckClientArrow(ThreadIndex, o);
		}
		break;

	case MODEL_COMBO:
		if (o->SubType == 0)
		{
			if (o->LifeTime>4)
			{
				o->Scale += o->Gravity;
				o->Gravity += 0.1f;
			}
			o->BlendMeshLight /= 1.4f;
		}
		break;
	case MODEL_WAVES:
		if (o->SubType == 0)
		{
			if (o->LifeTime>4)
			{
				o->Scale += o->Gravity;
				o->Gravity += 0.1f;
			}
			o->BlendMeshLight /= 1.4f;
		}
		else if (o->SubType == 1)
		{
			o->Scale += o->Gravity;
			o->Gravity += 0.07f;
			if (o->Scale>2.0f) o->Scale = 2.0f;
			o->BlendMeshLight /= 1.5f;
		}
		else if (o->SubType == 2)
		{
			o->Scale += o->Gravity;
			o->Gravity += 0.01f;
			if (o->Scale>1.5f) o->Scale = 1.5f;
			o->BlendMeshLight /= 1.5f;
		}
		else if (o->SubType == 3)
		{
			o->Scale += o->Gravity;
			o->Gravity += 0.005f;
			if (o->Scale>1.5f) o->Scale = 1.5f;
			o->BlendMeshLight /= 1.3f;
			o->Angle[1] += 45.0f;
		}
		else if (o->SubType == 4)
		{
			o->Scale += o->Gravity;
			o->Gravity += 0.002f;
			if (o->Scale>2.5f) o->Scale = 2.5f;
			o->BlendMeshLight /= 1.2f;
			o->Angle[1] += 45.0f;
		}
		else if (o->SubType == 5)
		{
			o->Scale += o->Gravity;
			o->Gravity += 0.015f;
			if (o->Scale>2.5f) o->Scale = 2.5f;
			o->BlendMeshLight /= 1.3f;
			o->Angle[1] += 45.0f;
		}
		else if (o->SubType == 6)
		{
			o->Scale += o->Gravity;
			o->Gravity += 0.015f;
			if (o->Scale>2.5f) o->Scale = 2.5f;
			o->BlendMeshLight /= 1.3f;
			o->Angle[1] += 45.0f;
		}
		break;
	case MODEL_AIR_FORCE:
		if (o->Owner == nullptr)
		{
			o->Flags.Clear(OBJECT::eOF_LIVE);
			break;
		}
		if (o->SubType == 0)
		{
			o->BlendMeshLight /= 1.5f;
			o->Scale += 0.2f;
		}

		if (o->SubType == 1)
		{
			o->BlendMeshLight = o->LifeTime / 10.0f;

			VectorCopy(o->Owner->Position, o->Position);
			VectorCopy(o->Owner->Angle, o->Angle);
			Vector(0.0f, -70.0f, 0.0f, p);
			AngleMatrix(o->Angle, Matrix);
			VectorRotate(p, Matrix, Position);
			Position[2] += 100.0f;
			VectorAdd(o->Position, Position, o->Position);
		}
		break;
	case MODEL_PIERCING2:
		AngleMatrix(o->Angle, Matrix);
		VectorRotate(o->Direction, Matrix, Position);

		o->Direction[1] += 12.0f;
		if (o->Direction[1] >= 0) o->Direction[1] = 0;

		if (o->SubType == 1)
		{
			if (o->LifeTime>1)
			{
				o->BlendMeshLight /= 1.6f;
				CreateEffect(ThreadIndex, MODEL_WAVES, o->Position, o->Angle, o->Light, 2, nullptr, o->LifeTime);
			}
		}
		else if (o->SubType == 2)
		{
			if (o->LifeTime>1)
			{
				o->BlendMeshLight /= 1.6f;
				CreateEffect(ThreadIndex, MODEL_WAVES, o->Position, o->Angle, o->Light, 2, nullptr, o->LifeTime);
			}
		}
		else
		{
			if (o->LifeTime>5)
			{
				o->BlendMeshLight /= 1.6f;
				CreateEffect(ThreadIndex, MODEL_WAVES, o->Position, o->Angle, o->Light, 2, nullptr, o->LifeTime);
			}
		}
		break;
	case MODEL_DEASULER:
		{
			MUObjectExtra *ox = &g_EffectsExtra[iIndex];
			EVector3	v3RotateAngleRelative;
			o->Flags.Set(OBJECT::eOF_VISIBLE);

			mu_float	fCurrentRate = 1.0f - ((mu_float)o->LifeTime / (mu_float)o->ExtState);

			if (ox->m_Interpolates.m_vecInterpolatesAngle.empty() == false)
			{
				ox->m_Interpolates.GetAngleCurrent(v3RotateAngleRelative, fCurrentRate);

				o->Angle[0] = v3RotateAngleRelative[0] + o->HeadAngle[0];
				o->Angle[1] = v3RotateAngleRelative[1] + o->HeadAngle[1];
				o->Angle[2] = v3RotateAngleRelative[2] + o->HeadAngle[2];
			}

			if (ox->m_Interpolates.m_vecInterpolatesPos.empty() == false)
			{
				ox->m_Interpolates.GetPosCurrent(o->Position, fCurrentRate);
			}

			if (ox->m_Interpolates.m_vecInterpolatesScale.empty() == false)
			{
				ox->m_Interpolates.GetScaleCurrent(o->Scale, fCurrentRate);
			}

			if (ox->m_Interpolates.m_vecInterpolatesScale.empty() == false)
			{
				ox->m_Interpolates.GetAlphaCurrent(o->Alpha, fCurrentRate);
			}

			mu_float fRateBlurStart, fRateBlurEnd, fRateShadowStart, fRateShadowEnd, fRateJointStart, fRateJointEnd;
			fRateBlurStart = fRateBlurEnd = 0.0f;
			fRateShadowStart = fRateShadowEnd = 0.0f;
			fRateJointStart = fRateJointEnd = 0.0f;
			fRateBlurStart = 0.0f; fRateBlurEnd = 0.90f;

			mu_int32 iTYPESWORDFORCE = 0;
			mu_int32 iTYPESWORDSHADOW = 0;

			iTYPESWORDFORCE = 1;
			iTYPESWORDSHADOW = 0;

			if (iTYPESWORDFORCE == 1)
			{
				if (fCurrentRate > fRateBlurStart && fCurrentRate < fRateBlurEnd)
				{
					MUModel*	b = &MODELS::Data[o->Type];
					EVector3  vLightBlur; Vector(1.0f, 1.0f, 1.0f, vLightBlur);
					mu_float	fPreRate = 1.0f - (mu_float)((o->LifeTime) + 1) / (mu_float)(o->ExtState);
					SETLIMITS(fPreRate, 1.0f, 0.0f);
					if (fPreRate < fCurrentRate)
					{
						for (mu_int32 Loop_bk = 0; Loop_bk<2; ++Loop_bk)
						{
							mu_float	fStartRate, fEndRate;

							fStartRate = 1.0f - (mu_float)((o->LifeTime) + 2) / (mu_float)(o->ExtState);
							fEndRate = 1.0f - (mu_float)((o->LifeTime) + 1) / (mu_float)(o->ExtState);


							SETLIMITS(fStartRate, 1.0f, 0.0f);
							SETLIMITS(fEndRate, 1.0f, 0.0f);

							EVector3 *arrEachBonePos = ThreadInfo.BonePosition;

							EVector3 v3CurBlurAngle, v3CurBlurPos;
							mu_int32	iAccess = 20;
							mu_int32 iBone01, iBone02;
							mu_int32 iBlurIdentity, iTypeBlur;
							mu_float fUnit;
							mu_float fCurrentRateUnit = fStartRate;
							mu_float fScale = o->Scale;

							if (Loop_bk == 0)
							{
								iBone01 = 4;
								iBone02 = 2;
								iBlurIdentity = 113;
							}
							else
							{
								iBone01 = 5;
								iBone02 = 1;
								iBlurIdentity = 114;
							}
							iTypeBlur = 10;


							fUnit = (fEndRate - fStartRate) / (mu_float)iAccess;


							for (mu_int32 i = 0; i<iAccess; ++i)
							{
								fCurrentRateUnit += fUnit;

								ox->m_Interpolates.GetAngleCurrent(v3CurBlurAngle, fCurrentRateUnit);
								ox->m_Interpolates.GetPosCurrent(v3CurBlurPos, fCurrentRateUnit);

								VectorAdd(v3CurBlurAngle, o->HeadAngle, v3CurBlurAngle);

								b->AnimationTransformOnlySelf(ThreadIndex, arrEachBonePos, v3CurBlurAngle, v3CurBlurPos, fScale);

								CreateObjectBlur(o,
									arrEachBonePos[iBone01],
									arrEachBonePos[iBone02],
									vLightBlur, iTypeBlur, false, iBlurIdentity, 20);

								if (GetLargeRand(10) == 0)
								{
									EVector3	vAngle, vRandomDir, vRandomDirPosition, vResultRandomPosition;
									EMatrix4	matRandomRotation;
									EVector3	vPosition;

									VectorCopy(arrEachBonePos[1], vPosition);

									mu_float	fRandDistance = (mu_float)(GetLargeRand(100)) + 100;
									Vector(0.0f, fRandDistance, 0.0f, vRandomDir);

									CreateParticle(ThreadIndex, BITMAP_FIRE + 2, vPosition, o->Angle, o->Light, 17, 1.35f);

									Vector((mu_float)(GetLargeRand(360)), 0.0f, (mu_float)(GetLargeRand(360)), vAngle);
									AngleMatrix(vAngle, matRandomRotation);
									VectorRotate(vRandomDir, matRandomRotation, vRandomDirPosition);
									VectorAdd(vPosition, vRandomDirPosition, vResultRandomPosition);
									CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, vResultRandomPosition, vPosition, vAngle, 3, nullptr, 10.0f, 10, 10);
								}

							}
						}
					}
				}
			}
		}
		break;
	case MODEL_DEATH_SPI_SKILL:
		{
			if (o->SubType == 0)
			{
				if (o->Owner != nullptr)
				{
					VectorCopy(o->Owner->Position, p);
					VectorAdd(p, o->StartPosition, p);

					mu_float Distance;
					for (mu_int32 i = 1; i<o->Gravity; ++i)
					{
						if (GetLargeRand(2) == 0)
						{
							if (o->Angle[0]<-90)
								o->Angle[0] += 20.0f;
							else
								o->Angle[0] -= 20.0f;
						}
						Distance = MoveHumming(o->Position, o->Angle, p, o->Velocity);
						o->Velocity += 0.4f;

						if (o->LifeTime<10)
						{
							o->Velocity += 0.1f;
							CreateEffect(ThreadIndex, BITMAP_MAGIC + 1, Position, o->Angle, o->Light, 1, o);
						}

						AngleMatrix(o->Angle, Matrix);
						VectorRotate(o->Direction, Matrix, Position);
						VectorAdd(o->Position, Position, o->Position);

						CreateEffect(ThreadIndex, MODEL_TAIL, o->Position, o->Angle, o->Light, 0, o);

					}
					if (Distance<40 && o->LifeTime == 5)
					{
						VectorCopy(o->Position, Position);
						Position[2] = RequestTerrainHeight(o->Position[0], o->Position[1]);
					}
					o->Gravity += 0.1f;

					PlayBackground(SOUND_ATTACK_FIRE_BUST_EXP);
				}
			}
			else if (o->SubType == 1)
			{
				if (o->LifeTime<5)
				{
					o->Alpha /= 1.3f;
				}
			}
		}
		break;
	case MODEL_PIER_PART:
		if (o->SubType == 0)
		{
			if (o->Owner != nullptr)
			{
				VectorCopy(o->Owner->Position, p);
				VectorAdd(p, o->StartPosition, p);

				mu_float Distance;
				for (mu_int32 i = 1; i<o->Gravity; ++i)
				{
					if (GetLargeRand(2) == 0)
					{
						if (o->Angle[0]<-90)
							o->Angle[0] += 20.0f;
						else
							o->Angle[0] -= 20.0f;
					}
					Distance = MoveHumming(o->Position, o->Angle, p, o->Velocity);
					o->Velocity += 0.4f;

					if (o->LifeTime<10)
					{
						o->Velocity += 0.1f;
					}

					AngleMatrix(o->Angle, Matrix);
					VectorRotate(o->Direction, Matrix, Position);
					VectorAdd(o->Position, Position, o->Position);

					CreateEffect(ThreadIndex, MODEL_PIER_PART, o->Position, o->Angle, o->Light, 1, o);

				}
				if (Distance<40 && o->LifeTime == 5)
				{
					VectorCopy(o->Position, Position);
					Position[2] = RequestTerrainHeight(o->Position[0], o->Position[1]);
				}
				o->Gravity += 0.1f;

				PlayBackground(SOUND_ATTACK_FIRE_BUST_EXP);
			}
		}
		else if (o->SubType == 2)
		{
			if (o->Owner != nullptr)
			{
				VectorCopy(o->Owner->Position, p);

				MoveHumming(o->Position, o->Angle, p, o->Velocity);
				o->Velocity += 2.4f;

				AngleMatrix(o->Angle, Matrix);
				VectorRotate(o->Direction, Matrix, Position);
				VectorAdd(o->Position, Position, o->Position);

				if (o->LifeTime % 3 == 0)
				{
					Vector(-90.0f, 0.0f, o->Angle[2], Angle);
					CreateJoint(ThreadIndex, BITMAP_JOINT_FORCE, o->Position, o->Position, Angle, 2, nullptr, 150.0f);
				}
			}
		}
		else if (o->SubType == 1)
		{
			if (o->LifeTime<5)
			{
				o->Alpha /= 1.3f;
			}
		}
		break;

	case BITMAP_FLARE_FORCE:
		break;

	case MODEL_DARKLORD_SKILL:
		if (o->SubType <= 1)
		{
			o->Scale += o->Velocity;
			o->Velocity += 0.02f;
			if (o->LifeTime<7)
			{
				o->BlendMeshLight /= 1.8f;
			}
		}
		else if (o->SubType == 2)
		{
			o->Scale += o->Velocity;
			o->Velocity -= 0.06f;
			if (o->LifeTime<8)
			{
				o->BlendMeshLight /= 1.5f;
			}
		}
		else
		{
			o->Scale += o->Velocity;
			o->Velocity += 0.04f;
			if (o->LifeTime<8)
			{
				o->BlendMeshLight /= 1.8f;
			}
		}
		break;
	case MODEL_GROUND_STONE:
	case MODEL_GROUND_STONE2:
		if (o->LifeTime>32 && o->LifeTime<37)
		{
			Position[0] = o->Position[0] + 60;
			Position[1] = o->Position[1] - 60;
			Position[2] = o->Position[2] + 50.0f;

			Vector(1.0f, 0.8f, 0.6f, Light);
			CreateParticle(ThreadIndex, BITMAP_SMOKE, Position, o->Angle, Light, 11, 2.0f, o);
			CreateEffect(ThreadIndex, MODEL_STONE1 + GetLargeRand(2), Position, o->Angle, o->Light, 10);
		}

		if (o->LifeTime<8)
		{
			o->Alpha /= 1.3f;
			o->BlendMeshLight /= 2.0f;
		}

		Vector(0.79f, 0.72f, 0.49f, Light);
		AddTerrainLight(o->Position[0], o->Position[1], Light, 2, PrimaryTerrainLight);
		break;

	case BITMAP_TWLIGHT:
		{
			if (o->SubType == 0)
			{
				o->Scale -= 0.1f;
				o->Angle[2] += 10.0f;
			}
			else
				if (o->SubType == 1)
				{
					o->Scale -= 0.1f;
					o->Angle[2] += 5.0f;
				}
				else
					if (o->SubType == 2)
					{
						o->Scale -= 0.1f;
						o->Angle[2] += 15.0f;
					}
					else if (o->SubType == 3)
					{
						VectorCopy(o->Owner->Position, o->Position);
						o->Scale -= 0.15f;
						o->Angle[2] += 10.0f;
						if (o->LifeTime >= 20)
						{
							o->Alpha += 0.1f;
							++o->PKKey;
							o->Light[0] = o->EyeRight[0] * (o->PKKey*0.1f);
							o->Light[1] = o->EyeRight[1] * (o->PKKey*0.1f);
							o->Light[2] = o->EyeRight[2] * (o->PKKey*0.1f);

						}
						else if (o->LifeTime <= 10)
						{
							--o->PKKey;
							o->Alpha -= 0.1f;
							o->Light[0] = o->EyeRight[0] * (o->PKKey*0.1f);
							o->Light[1] = o->EyeRight[1] * (o->PKKey*0.1f);
							o->Light[2] = o->EyeRight[2] * (o->PKKey*0.1f);

						}
					}
		}
		break;
	case BITMAP_SHOCK_WAVE:
		if (o->SubType == 0)
		{
			o->Scale -= 1.0f;
		}
		else if (o->SubType == 1)
		{
			o->Scale += (GetLargeRand(5)) / 10.0f;

			o->Position[0] += GetLargeRand(8) - 4.0f;
			o->Position[1] += GetLargeRand(8) - 4.0f;
		}
		else if (o->SubType == 2)
		{
			o->Scale += (GetLargeRand(5)) / 40.0f;

			o->Position[0] += GetLargeRand(8) - 4.0f;
			o->Position[1] += GetLargeRand(8) - 4.0f;
		}
		else if (o->SubType == 3)
		{
			o->Scale += 2.0f;
		}
		else if (o->SubType == 4)
		{
			o->Scale += 0.3f;
		}
		else if (o->SubType == 5)
		{
			o->Scale -= 0.4f;
		}
		else if (o->SubType == 6)
		{
			if ((o->LifeTime % 8) == 0)
			{
				CreateEffect(ThreadIndex, BITMAP_SHOCK_WAVE, o->Position, o->Angle, o->Light, 5);
			}
			break;
		}
		else if (o->SubType == 7)
		{
			o->Scale += 2.5f;
		}
		else if (o->SubType == 8)
		{
			o->Scale += 1.0f;
		}
		else if (o->SubType == 9)
		{
			o->Scale += 0.8f;
			VectorCopy(o->Owner->Position, o->Position);
		}
		else if (o->SubType == 10)
		{
			o->Scale -= 0.02f;
			VectorCopy(o->Owner->Position, o->Position);
		}
		else if (o->SubType == 11)
		{
			o->Scale += (GetLargeRand(5)) / 10.0f;

			o->Position[0] += GetLargeRand(8) - 4.0f;
			o->Position[1] += GetLargeRand(8) - 4.0f;
		}
		else if (o->SubType == 12)
		{
			if (o->LifeTime > 4.0f)
				o->Scale += (o->LifeTime - 4.0f) * 0.25f;
		}
		else if (o->SubType == 13)
		{
			o->Scale += 0.08f;
		}
		else if (o->SubType == 14)
		{
			VectorCopy(o->Owner->Position, o->Position);
			o->Scale -= 0.15f;
			if (o->LifeTime >= 20)
			{
				o->Alpha += 0.1f;
				++o->PKKey;
				o->Light[0] = o->EyeRight[0] * (o->PKKey*0.1f);
				o->Light[1] = o->EyeRight[1] * (o->PKKey*0.1f);
				o->Light[2] = o->EyeRight[2] * (o->PKKey*0.1f);

			}
			else if (o->LifeTime <= 10)
			{
				--o->PKKey;
				o->Alpha -= 0.1f;
				o->Light[0] = o->EyeRight[0] * (o->PKKey*0.1f);
				o->Light[1] = o->EyeRight[1] * (o->PKKey*0.1f);
				o->Light[2] = o->EyeRight[2] * (o->PKKey*0.1f);

			}
			break;
		}

		if (o->Scale<0)
		{
			o->Scale = 0;
		}
		if (o->SubType >= 0 && o->SubType <= 3)
		{
			if (o->LifeTime <= 20)
			{
				Luminosity = o->LifeTime / 20.0f;
				Vector(Luminosity, Luminosity, Luminosity, o->Light);
			}
			else
			{
				Luminosity = (40 - o->LifeTime) / 20.0f;
				Vector(Luminosity, Luminosity, Luminosity, o->Light);
			}
		}
		else
		{
			if (o->LifeTime<6)
			{
				o->Light[0] /= 1.3f;
				o->Light[1] /= 1.3f;
				o->Light[2] /= 1.3f;
			}
		}
		break;

	case BITMAP_DAMAGE_01_MONO:
		if (o->SubType == 0)
		{
			o->Scale += 5.0f;
		}
		else if (o->SubType == 1)
		{
			o->Scale += 0.5f;
			if (o->Scale > 3.5f)
			{
				o->Light[0] *= 0.5f;
				o->Light[1] *= 0.5f;
				o->Light[2] *= 0.5f;
			}

		}
		break;

	case BITMAP_FLARE:
		if (o->SubType == 1)
		{
			CreateParticle(ThreadIndex, BITMAP_FLARE, o->Position, o->Angle, Light, 11, 1.0f);
		}
		else if (o->SubType == 2)
		{
			CreateParticle(ThreadIndex, BITMAP_FLARE_BLUE, o->Position, o->Angle, Light, 1, 1.0f);
		}
		else if (o->SubType == 3)
		{
			Vector(0.9f, 0.4f, 0.1f, Light);
			mu_float fRandom = 3.5f + ((mu_float)(GetLargeRand(20) - 10)*0.1f);
			CreateParticle(ThreadIndex, BITMAP_LIGHT, o->Position, o->Angle, Light, 5, fRandom);
		}
		else if (o->LifeTime % 2 == 0)
		{
			VectorCopy(o->Position, Position);
			Position[2] += 100.0f;
			CreateJoint(ThreadIndex, BITMAP_FLARE_BLUE, Position, Position, o->Angle, 14, o, 40.0f);
			CreateJoint(ThreadIndex, BITMAP_FLARE_BLUE, Position, Position, o->Angle, 15, o, 40.0f);

			if ((GetLargeRand(2)) == 0 && o->LifeTime>5 && o->LifeTime<15)
			{
				Vector(o->Position[0] - 200.0f, o->Position[1], o->Position[2] + 700.0f, Position);
				CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, Position, o->Position, Angle, 0, o, 20.0f);
			}
		}
		break;

	case BITMAP_JOINT_THUNDER + 1:
		if (o->LifeTime<5 && o->LifeTime % 2 == 0 && o->SubType == 0)
		{
			VectorCopy(o->Position, Position);

			Position[0] += GetLargeRand(64) - 32.0f;
			Position[1] += GetLargeRand(64) - 32.0f;

			CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER + 1, Position, Position, o->Angle, 5, nullptr, 50.0f + GetLargeRand(10));
		}
		break;

	case MODEL_CUNDUN_DRAGON_HEAD:
		o->Angle[2] += 10 + GetLargeRand(10);

		if (o->Position[2] > 300 && o->Position[2] < 600)
		{
			EVector3 Angle;
			Angle[0] = Angle[1] = 0;
			Position[0] = o->Position[0];
			Position[1] = o->Position[1];
			Position[2] = 350;
			if (GetLargeRand(2) == 0)
			{
				Angle[2] = (mu_float)(GetLargeRand(24) * 30);
				CreateJoint(ThreadIndex, BITMAP_JOINT_SPIRIT2, Position, Position, Angle, 14, nullptr, 100.0f, 0, 0);
			}
		}
		break;
	case MODEL_CUNDUN_PHOENIX:
		if (o->LifeTime < 5)
		{
			o->Alpha = o->LifeTime * 0.2f;
		}
		break;
	case MODEL_CUNDUN_GHOST:
		if (o->Owner != nullptr)
		{
			if (o->Owner->PKKey == 0)
			{
				o->AnimationFrame = 0;
			}
			else
			{
				o->Owner = nullptr;
			}
		}
		else if (o->AnimationFrame > 6.0f)
		{
			o->PKKey += 1;
			o->Position[2] += (o->PKKey * 0.8f);

			VectorCopy(o->Position, Position);
			Position[0] += GetLargeRand(120) - 60;
			Position[1] += GetLargeRand(120) - 60;
			Position[2] += GetLargeRand(60);
			Vector(1, 1, 1, Light);
			for (mu_int32 i = 0; i < 3; ++i)
				CreateParticle(ThreadIndex, BITMAP_SMOKE, Position, o->Angle, Light, 20, 10.0f);
		}
		else
		{
			o->Skill += 1;
			if (o->Angle[2] < 45.0f) o->Angle[2] += o->Skill * 0.3f;
			if (o->Angle[2] > 45.0f) o->Angle[2] -= o->Skill * 0.3f;

			if (o->Position[0] < Hero->Object.Position[0] + 300.0f) o->Position[0] += o->Skill * 0.2f;
			if (o->Position[0] > Hero->Object.Position[0] + 300.0f) o->Position[0] -= o->Skill * 0.2f;
			if (o->Position[1] < Hero->Object.Position[1] - 300.0f) o->Position[1] += o->Skill * 0.2f;
			if (o->Position[1] > Hero->Object.Position[1] - 300.0f) o->Position[1] -= o->Skill * 0.2f;

			Vector(1, 1, 1, Light);
			VectorCopy(o->Position, Position);
			Position[0] += GetLargeRand(1200) - 600;
			Position[1] += GetLargeRand(1200) - 600;
			EVector3 Angle;
			Vector(0.0f, 0.0f, GetLargeRand(10) * 20.0f, Angle);
			CreateEffect(ThreadIndex, MODEL_FIRE, Position, Angle, Light, 0, nullptr, 0);

			o->Scale += 0.02f;
			EarthQuake.Store((mu_float)(GetLargeRand(8) - 8)*0.1f);
		}
		break;
	case MODEL_CUNDUN_SKILL:
		if (o->SubType == 0)
		{
			if (o->LifeTime == 30)
			{
				for (mu_int32 i = 0; i < 10; ++i)
				{
					EVector3 Angle;
					Vector(0, 0, o->Angle[2] + 320 + i * 8, Angle);
					CreateEffect(ThreadIndex, MODEL_CUNDUN_PHOENIX, o->Position, Angle, o->Light, 0);
				}
			}
		}
		else if (o->SubType == 1)
		{
			EVector3 Angle;
			Angle[0] = 0;
			Angle[1] = 0;
			if (o->LifeTime == 30)
			{
				for (mu_int32 i = 0; i < 20; ++i)
				{
					Angle[2] = (mu_float)(o->PKKey * 30);
					CreateEffect(ThreadIndex, MODEL_CUNDUN_DRAGON_HEAD, o->Position, Angle, o->Light);
				}
			}
			if (o->PKKey > 5 && o->PKKey < 10)
			{
				Angle[2] = (mu_float)(o->PKKey * 30);
				CreateEffect(ThreadIndex, MODEL_CUNDUN_DRAGON_HEAD, o->Position, Angle, o->Light);
			}
			++o->PKKey;
		}
		else if (o->SubType == 2)
		{
			if (o->PKKey == 0 && o->LifeTime < 10)
			{
				o->PKKey = 1;
			}
		}
		break;

	case BATTLE_CASTLE_WALL1:
	case BATTLE_CASTLE_WALL2:
	case BATTLE_CASTLE_WALL3:
	case BATTLE_CASTLE_WALL4:
		break;

	case MODEL_MONSTER01 + 77:
		if (o->SubType == 0)
		{
			if (o->LifeTime % 5 == 0)
			{
				VectorCopy(o->Position, Position);

				Position[0] += GetLargeRand(100) - 50.0f;
				Position[1] -= GetLargeRand(30) + 15.0f;
				CreateEffect(ThreadIndex, MODEL_FLY_BIG_STONE1, Position, o->Angle, o->Light, 2);
			}
		}
		if (o->LifeTime<5)
		{
			o->Alpha /= 1.3f;
		}
		break;
	case MODEL_ARROW_TANKER_HIT:
		{
			mu_float Height = RequestTerrainHeight(o->Position[0], o->Position[1]);

			if (o->Position[2] < Height + 80.0f)
			{
				if (o->Alpha <= 0.1f) o->LifeTime = 0;
				o->Alpha /= 1.05f;

				Vector(0.0f, 0.0f, -30.0f, o->Direction);
				Vector(1.0f, 0.6f, 0.2f, Light);
				CreateParticle(ThreadIndex, BITMAP_ADV_SMOKE + 1, o->Position, o->Angle, Light);

				if (o->HiddenMesh != 99)
				{
					CreateInferno(ThreadIndex, o->Position, 1);
					VectorCopy(o->Position, o->StartPosition);
					CreateEffect(ThreadIndex, BITMAP_CRATER, o->Position, o->Angle, o->Light);
				}

				EarthQuake.Store((mu_float)(GetLargeRand(8) - 8)*0.05f);

				o->HiddenMesh = 99;
			}

			if (o->HiddenMesh == 99)
			{
				o->Position[0] += (mu_float)(GetLargeRand(100) - 50);
				o->Position[1] += (mu_float)(GetLargeRand(100) - 50);
				o->Position[2] += (mu_float)(GetLargeRand(100) - 50);
				o->Scale = 2.2f;
			}

			if (o->Flags.Get(OBJECT::eOF_VISIBLE) == true)
			{
				EVector3 p1, p2;
				MUModel* b = &MODELS::Data[o->Type];

				Vector(1.0f, 1.0f, 1.0f, o->Light);
				Vector(0.0f, 1.5f, 0.0f, p1);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(2)], p1, p2);
				CreateParticle(ThreadIndex, BITMAP_FIRE + 1, o->Position, o->Angle, o->Light, 8, o->Scale - 0.4f, o);
				CreateParticle(ThreadIndex, BITMAP_SMOKE, o->Position, o->Angle, o->Light, 38, o->Scale, o);
				Vector(1.0f, 0.4f, 0.0f, o->Light);
				CreateParticle(ThreadIndex, BITMAP_FIRE + 1, o->Position, o->Angle, o->Light, 9, o->Scale - 0.4f, o);
			}

			if (o->HiddenMesh == 99)
			{
				o->Scale = 1.0f;
				VectorCopy(o->StartPosition, o->Position);
			}

			Vector(1.0f, 0.0f, 0.0f, Light);
			AddTerrainLight(o->Position[0], o->Position[1], Light, 4, PrimaryTerrainLight);
		}
		break;
	case MODEL_FLY_BIG_STONE1:
		{
			if (o->SubType == 2)
			{
				o->Position[2] -= o->Gravity;
				o->Gravity += 1.9f;

				o->Angle[0] += GetLargeRand(20) + 20.0f;
				o->Angle[1] += GetLargeRand(5);

				mu_float Height = RequestTerrainHeight(o->Position[0], o->Position[1]);
				if (o->Position[2]<Height)
				{
					o->Position[2] = Height + 50;
					o->Gravity = -o->Gravity*0.2f;
					o->LifeTime -= 5;
					o->Angle[0] -= o->Scale*128.0f;

					Vector(0.0f, 5.0f, 0.0f, o->Direction);
				}
				else
					o->Angle[0] -= o->Scale*16.0f;

				AngleMatrix(o->HeadAngle, Matrix);
				VectorRotate(o->Direction, Matrix, Position);
				VectorAdd(o->Position, Position, o->Position);

				if (o->LifeTime<10)
				{
					o->Alpha /= 1.5f;
				}

				if (o->LifeTime % 3 == 0)
				{
					Vector(1.0f, 0.6f, 0.2f, Light);
					CreateParticle(ThreadIndex, BITMAP_ADV_SMOKE + 1, o->Position, o->Angle, Light, 1, 0.2f);
				}
			}
			else
			{
				if (o->SubType <= 1)
				{
					o->Position[0] += o->Direction[0] * o->Velocity;
					o->Position[1] += o->Direction[1] * o->Velocity;
					o->StartPosition[2] += o->Direction[2];

					if (o->Position[2]>1000.0f)
					{
						o->Position[2] = 1000.0f;
						o->HiddenMesh = -2;
					}
					else
					{
						o->HiddenMesh = -1;
						Vector(Luminosity*1.0f, Luminosity*0.5f, Luminosity*0.1f, Light);
						CreateParticle(ThreadIndex, BITMAP_FIRE, o->Position, o->Angle, Light, 5, 2.0f);
					}

					o->Velocity -= 0.45f;
					if (o->Velocity<5.0f) o->Velocity = 5.0f;

					o->Direction[2] += o->Gravity;
					o->Gravity -= 0.1f;

					o->Angle[0] += 5.0f;
					o->Angle[1] += 5.0f;

					if (o->SubType == 0)
					{
						o->Position[2] = o->StartPosition[2];
					}
					else if (o->Owner == (&Hero->Object))
					{
						o->Position[2] = o->StartPosition[2];
						if (o->Position[2]>800.0f)
						{
							o->Position[2] = 800.0f;
						}
#if 0 // FIX
						g_pCatapultWindow->SetCameraPos(o->Position[0], o->Position[1], o->Position[2]);
#endif
					}
				}

				if (o->SubType == 88 || o->SubType == 99)
				{
					if (o->SubType == 99 && o->Owner == (&Hero->Object))
					{
#if 0 // FIX
						g_pCatapultWindow->SetCameraPos(o->Position[0], o->Position[1], o->Position[2]);
#endif
					}

					if (o->LifeTime>o->DamageTime)
					{
						Vector(1.0f, 0.6f, 0.2f, Light);
						CreateParticle(ThreadIndex, BITMAP_ADV_SMOKE + 1, o->Position, o->Angle, Light);
						CreateParticle(ThreadIndex, BITMAP_FIRE, o->Position, o->Angle, Light, 2, 2.0f);

						if (o->SubType == 88 && o->Owner != nullptr)
						{
							battleCastle::CollisionHeroCharacter(o->Position, ((15 - o->LifeTime) * 20) + 350.0f, PLAYER_HIGH_SHOCK);
						}
						else if (o->SubType == 99)
						{
							battleCastle::CollisionTempCharacter(o->Position, ((40 - o->LifeTime) * 7) + 350.0f, PLAYER_HIGH_SHOCK);
						}
					}
				}
				else
				{
					mu_float Height = RequestTerrainHeight(o->Position[0], o->Position[1]);
					if (o->LifeTime<250 && o->Position[2]<Height + 300.0f && o->SubType == 1 && o->Kind != 0 && o->Skill != 0)
					{
#if 0 // FIX
						SendWeaponExplosion(o->Kind, o->Skill);
#endif
						o->Kind = 0;
						o->Skill = 0;
					}

					if (o->Position[2]<Height)
					{
						o->Position[2] = Height - 5.0f;
						if (o->SubType == 0)
						{
							o->LifeTime = 15;
							o->DamageTime = 0;
							o->SubType = 88;
						}
						else if (o->SubType == 1)
						{
							o->LifeTime = 40;
							o->DamageTime = 25;
							o->SubType = 99;
						}
						if (o->HiddenMesh != 99 && o->Flags.Get(OBJECT::eOF_VISIBLE) == true)
						{
							CreateEffect(ThreadIndex, BITMAP_CRATER, o->Position, o->Angle, o->Light);
							PlayObject(SOUND_BC_CATAPULT_HIT, o);

							battleCastle::CollisionEffectToObject(o, 350.0f, 250.0f, true);

							if (o->SubType == 88 && o->Owner == nullptr)
							{
								battleCastle::CollisionHeroCharacter(o->Position, 350.0f, PLAYER_HIGH_SHOCK);
							}
							else if (o->SubType == 99 || (o->SubType == 88 && o->Owner != nullptr))
							{
								Vector(1.0f, 0.3f, 0.1f, Light);
								CreateEffect(ThreadIndex, BITMAP_SHOCK_WAVE, o->Position, o->Angle, Light, 7);
							}
						}
						o->HiddenMesh = 99;

						if (Height>150.0f)
						{
							mu_int32 CollisionX = (mu_int32)(o->Position[0] * g_TerrainScaleRev);
							mu_int32 CollisionY = (mu_int32)(o->Position[1] * g_TerrainScaleRev);

							mu_float earthquake = (mu_float)(GetLargeRand(8) - 8)*0.1f;
							if (CollisionY<104 && o->Flags.Get(OBJECT::eOF_VISIBLE) == true)
							{
								if (g_LightEnabled)
								{
									mu_uint32 modIndex = g_TerrainModificationCount.fetch_add(1);
									if(modIndex < MAX_TERRAIN_MODIFICATION)
									{
										TerrainModificationRequest &request = g_TerrainModification[modIndex];

										request.CreateNormal = true;
										request.CreateLight = true;
										request.PositionX = o->Position[0];
										request.PositionY = o->Position[1];
										request.Height = -30.0f;
										request.Range = 2;
										request.CollisionX = CollisionX;
										request.CollisionY = CollisionY;
									}
								}

								earthquake /= 2.0f;
							}
							EarthQuake.Store(earthquake);
						}
					}
					else
					{
						mu_boolean collision = false;

						if (o->SubType == 1)
						{
							collision = battleCastle::CollisionEffectToObject(o, 200.0f, 250.0f, false, true);
						}
						else
						{
							collision = battleCastle::CollisionEffectToObject(o, 200.0f, 250.0f, false);
						}

						if (collision && o->Flags.Get(OBJECT::eOF_VISIBLE) == true)
						{
							PlayObject(SOUND_BC_CATAPULT_HIT, o);
						}
					}
				}
				Vector(-0.5f, -0.5f, -0.5f, Light);
				AddTerrainLight(o->Position[0], o->Position[1], Light, 4, PrimaryTerrainLight);
			}
		}
		break;

	case MODEL_FLY_BIG_STONE2:
		{
			if (o->SubType <= 1)
			{
				o->Position[0] += o->Direction[0] * o->Velocity;
				o->Position[1] += o->Direction[1] * o->Velocity;
				o->StartPosition[2] += o->Direction[2];

				if (o->Position[2]>1000.0f)
				{
					o->Position[2] = 1000.0f;
					o->HiddenMesh = -2;
				}
				else
				{
					o->HiddenMesh = -1;
					Vector(1.0f, 1.0f, 1.0f, Light);

					if ((GetLargeRand(2)) == 0)
					{
						CreateParticle(ThreadIndex, BITMAP_ADV_SMOKE + 1, o->Position, o->Angle, Light, 1, 2.0f);
					}
				}

				o->Velocity -= 0.45f;
				if (o->Velocity<5.0f) o->Velocity = 5.0f;

				o->Direction[2] += o->Gravity;
				o->Gravity -= 0.1f;

				o->Angle[0] += 5.0f;
				o->Angle[1] += 5.0f;

				if (o->SubType == 0)
				{
					o->Position[2] = o->StartPosition[2];
				}
				else if (o->Owner == (&Hero->Object))
				{
					o->Position[2] = o->StartPosition[2];
					if (o->Position[2]>800.0f)
					{
						o->Position[2] = 800.0f;
					}
#if 0 // FIX
					g_pCatapultWindow->SetCameraPos(o->Position[0], o->Position[1], o->Position[2]);
#endif
				}
			}

			Vector(-0.5f, -0.5f, -0.5f, Light);
			if (o->SubType == 88 || o->SubType == 99)
			{
				if (o->SubType == 99 && o->Owner == (&Hero->Object))
				{
#if 0 // FIX
					g_pCatapultWindow->SetCameraPos(o->Position[0], o->Position[1], o->Position[2]);
#endif
				}

				if (o->LifeTime>o->DamageTime)
				{
					CreateParticle(ThreadIndex, BITMAP_ADV_SMOKE + 1, o->Position, o->Angle, o->Light);

					if (o->SubType == 88 && o->Owner != nullptr)
					{
						battleCastle::CollisionHeroCharacter(o->Position, ((15 - o->LifeTime) * 20) + 350.0f, PLAYER_HIGH_SHOCK);
					}
					else if (o->SubType == 99)
					{
						battleCastle::CollisionTempCharacter(o->Position, ((40 - o->LifeTime) * 7) + 350.0f, PLAYER_HIGH_SHOCK);
					}
				}
			}
			else
			{
				mu_float Height = RequestTerrainHeight(o->Position[0], o->Position[1]);
				if (o->Position[2]<Height + 300.0f && o->SubType == 1 && o->Kind != 0 && o->Skill != 0)
				{
#if 0 // FIX
					SendWeaponExplosion(o->Kind, o->Skill);
#endif
					o->Kind = 0;
					o->Skill = 0;
				}

				if (o->Position[2]<Height)
				{
					o->Position[2] = Height - 5.0f;
					if (o->SubType == 0)
					{
						o->LifeTime = 15;
						o->DamageTime = 0;
						o->SubType = 88;
					}
					else if (o->SubType == 1)
					{
						o->LifeTime = 40;
						o->DamageTime = 25;
						o->SubType = 99;
					}
					if (o->HiddenMesh != 99 && o->Flags.Get(OBJECT::eOF_VISIBLE) == true)
					{
						CreateEffect(ThreadIndex, BITMAP_CRATER, o->Position, o->Angle, o->Light);
						PlayObject(SOUND_BC_CATAPULT_HIT, o);

						if (o->SubType == 88 && o->Owner == nullptr)
						{
							battleCastle::CollisionHeroCharacter(o->Position, 350.0f, PLAYER_HIGH_SHOCK);
						}
						else if (o->SubType == 99 || (o->SubType == 88 && o->Owner != nullptr))
						{
							Vector(1.0f, 0.3f, 0.1f, Light);
							CreateEffect(ThreadIndex, BITMAP_SHOCK_WAVE, o->Position, o->Angle, Light, 7);
						}
					}
					o->HiddenMesh = 99;

					if (Height>150.0f)
					{
						mu_int32 CollisionX = (mu_int32)(o->Position[0] * g_TerrainScaleRev);
						mu_int32 CollisionY = (mu_int32)(o->Position[1] * g_TerrainScaleRev);

						mu_float earthquake = (mu_float)(GetLargeRand(8) - 8)*0.1f;
						if (CollisionY<104 && o->Flags.Get(OBJECT::eOF_VISIBLE) == true)
						{
							if(g_LightEnabled)
							{
								mu_uint32 modIndex = g_TerrainModificationCount.fetch_add(1);
								if(modIndex < MAX_TERRAIN_MODIFICATION)
								{
									TerrainModificationRequest &request = g_TerrainModification[modIndex];

									request.CreateNormal = true;
									request.CreateLight = true;
									request.PositionX = o->Position[0];
									request.PositionY = o->Position[1];
									request.Height = -30.0f;
									request.Range = 2;
									request.CollisionX = CollisionX;
									request.CollisionY = CollisionY;
								}
							}

							earthquake /= 2.0f;
						}
						EarthQuake.Store(earthquake);
					}
				}
			}
			AddTerrainLight(o->Position[0], o->Position[1], Light, 4, PrimaryTerrainLight);
		}
		break;

	case MODEL_BIG_STONE_PART1:
	case MODEL_BIG_STONE_PART2:
	case MODEL_WALL_PART1:
	case MODEL_WALL_PART2:
	case MODEL_GOLEM_STONE:
		if (o->Type == MODEL_BIG_STONE_PART2 && o->SubType == 3)
		{
			o->Direction[2] -= o->Velocity;
			o->Velocity += 0.3f;

			Height = RequestTerrainHeight(o->Position[0], o->Position[1]);
			if (o->Position[2]<Height)
			{
				o->Position[2] = Height;
				Vector(0.0f, 0.0f, 0.0f, o->Direction);
				Vector(0.0f, 0.0f, 0.0f, o->Angle);
				o->HiddenMesh = 0;

				if (GetLargeRand(10) == 0)
					CreateParticle(ThreadIndex, BITMAP_SMOKE, o->Position, o->Angle, o->Light, 24, 1.25f*o->Scale);
			}
			else
			{
				if (GetLargeRand(2) == 0)
					CreateParticle(ThreadIndex, BITMAP_SMOKE, o->Position, o->Angle, o->Light, 24, 0.2f);
			}
			break;
		}
		VectorAdd(o->Position, o->Direction, o->Position);
		if (o->Type >= MODEL_WALL_PART1 && o->Type <= MODEL_WALL_PART2)
		{
			VectorScale(o->Direction, 0.9f, o->Direction);
			o->Gravity -= 6.0f;
		}
		else
		{
			VectorScale(o->Direction, 0.9f, o->Direction);
			o->Gravity -= 3.0f;
		}
		o->Position[2] += o->Gravity;

		Height = RequestTerrainHeight(o->Position[0], o->Position[1]);
		if (o->Position[2]<Height)
		{
			o->Position[2] = Height;
			o->Gravity = -o->Gravity*0.2f;
			o->LifeTime -= 5;
			o->Angle[0] -= o->Scale*128.0f;
		}
		else
		{
			o->Angle[0] -= o->Scale*16.0f;
		}

		o->Alpha = o->LifeTime / 10.0f;
		if (o->Type == MODEL_GOLEM_STONE)
		{	
			if (GetLargeRand(4) == 0)
			{
				CreateParticle(ThreadIndex, BITMAP_TRUE_FIRE, o->Position, o->Angle, Light, 5, 2.8f);
				CreateParticle(ThreadIndex, BITMAP_SMOKE, o->Position, o->Angle, Light, 21, 1.8f);
			}
			if ((GetLargeRand(10)) == 0)
			{
				CreateParticle(ThreadIndex, BITMAP_SMOKE + 1, o->Position, o->Angle, Light);
			}
		}
		else if (o->Type == MODEL_BIG_STONE_PART1 && o->SubType == 2)
		{
			if ((GetLargeRand(10)) == 0)
			{
				Vector(0.2f, 0.5f, 0.35f, Light);
				o->Position[2] = Height;
				CreateParticle(ThreadIndex, BITMAP_SMOKE, o->Position, o->Angle, Light, 11, 2.0f);
			}
		}
		break;

	case MODEL_GATE_PART1:
	case MODEL_GATE_PART2:
	case MODEL_GATE_PART3:
		VectorAdd(o->Position, o->Direction, o->Position);
		VectorScale(o->Direction, 0.9f, o->Direction);
		o->Position[2] += o->Gravity;

		if (o->SubType == 1)
		{
			VectorScale(o->Direction, 1.1f, o->Direction);
		}

		o->Gravity -= 4.0f;
		Height = RequestTerrainHeight(o->Position[0], o->Position[1]);
		if (o->Position[2] < Height)
		{
			o->Position[2] = Height;
			o->Gravity = -o->Gravity*0.5f;
			o->LifeTime -= 5;
			o->Angle[0] -= o->Scale*128.0f;
		}
		else
			o->Angle[0] -= o->Scale*16.0f;

		if ((GetLargeRand(10)) == 0)
		{
			CreateParticle(ThreadIndex, BITMAP_SMOKE + 1, Position, o->Angle, Light);
		}
		break;

	case MODEL_AURORA:
		if (o->Owner != nullptr && o->Owner->Flags.Get(OBJECT::eOF_LIVE) == true)
		{
			if (battleCastle::IsBattleCastleStart() == false)
			{
				o->HiddenMesh = -2;
			}
			else
			{
				o->HiddenMesh = -1;
			}
			o->LifeTime = 2;
			o->BlendMeshLight = EMath::Sin(FWorldTime*0.001f)*0.1f + 0.2f;
		}
		else
		{
			o->Flags.Clear(OBJECT::eOF_LIVE);
		}
		o->BlendMeshTexCoordU = FWorldTime*0.0005f;
		break;

	case MODEL_FENRIR_THUNDER:
		if (o->SubType == 0)
		{
			if (o->Flags.Get(OBJECT::eOF_LIVE))
			{
				if (o->m_iAnimation == 0)
				{
					o->Alpha += 0.3f;
					if (o->Alpha >= 1.0f)
					{
						o->m_iAnimation = 1;
						o->Alpha = 1.0f;
					}

					if (o->Alpha < 0.0f)
					{
						o->Alpha = 1.0f;
					}
				}
				else if (o->m_iAnimation == 1)
				{
					o->Alpha -= 0.3f;
					if (o->Alpha <= 0.0f)
					{
						o->Alpha = 0.0f;
						o->Flags.Clear(OBJECT::eOF_LIVE);
					}
				}
				o->Angle[0] += 0.15f;
				o->Angle[1] += 0.15f;
				o->Angle[2] += 0.15f;
			}
		}
		else if (o->SubType == 1)
		{
			if (o->Flags.Get(OBJECT::eOF_LIVE))
			{
				if (o->m_iAnimation == 0)
				{
					o->Alpha += 0.3f;
					if (o->Alpha >= 1.0f)
					{
						o->m_iAnimation = 1;
						o->Alpha = 1.0f;
					}

					if (o->Alpha < 0.0f)
					{
						o->Alpha = 1.0f;
					}
				}
				else if (o->m_iAnimation == 1)
				{
					o->Alpha -= 0.3f;
					if (o->Alpha <= 0.0f)
					{
						o->Alpha = 0.0f;
						o->Flags.Clear(OBJECT::eOF_LIVE);
					}
				}
				o->Angle[0] += 0.15f;
				o->Angle[1] += 0.15f;
				o->Angle[2] += 0.15f;
			}
		}
		break;
	case MODEL_FALL_STONE_EFFECT:
		{
			if (o->SubType == 0 || o->SubType == 1)
			{
				o->Position[2] -= o->Gravity;
				o->Gravity += 0.1f;
				o->Angle[0] += 0.5f;
				o->Angle[1] += 0.5f;
				o->Angle[2] += 0.5f;
			}
			else if (o->SubType == 2)
			{
				o->Position[2] -= o->Gravity;
				o->Gravity += 0.1f;

				mu_float fHeight = RequestTerrainHeight(o->Position[0], o->Position[1]);
				if (o->Position[2] < fHeight)
				{
					EVector3 vLight;
					Vector(0.5f, 0.5f, 0.5f, vLight);
					CreateParticle(ThreadIndex, BITMAP_SMOKE, o->Position, o->Angle, vLight, 11, (mu_float)(GetLargeRand(20) + 30)*0.020f);

					mu_int32 iRand = GetLargeRand(2) + 2;
					for (mu_int32 i = 0; i<iRand; ++i)
					{
						mu_float fScale = 0.03f + (GetLargeRand(10)) / 40.0f + o->Scale * 0.3f;
						CreateEffect(ThreadIndex, MODEL_FALL_STONE_EFFECT, o->Position, o->Angle, o->Light, 3, nullptr, -1, 0, 0, 0, fScale);
					}

					o->Flags.Clear(OBJECT::eOF_LIVE);
				}
			}
			else if (o->SubType == 3)
			{
				o->HeadAngle[2] -= o->Gravity;

				o->Position[0] += o->HeadAngle[0];
				o->Position[1] += o->HeadAngle[1];
				o->Position[2] += o->HeadAngle[2];

				mu_float fHeight = RequestTerrainHeight(o->Position[0], o->Position[1]);
				o->Angle[0] += 0.5f * o->LifeTime;
				o->Angle[1] += 0.5f * o->LifeTime;
				if (o->Position[2] + o->Direction[2] <= fHeight)
				{
					o->Position[2] = fHeight;
					o->HeadAngle[0] *= 0.6f;
					o->HeadAngle[1] *= 0.6f;
					o->HeadAngle[2] += 1.0f*o->LifeTime;
					if (o->HeadAngle[2] < 0.5f)
						o->HeadAngle[2] = 0;

					o->Alpha -= 0.15f;

					o->Light[0] /= 1.08f;
					o->Light[1] /= 1.08f;
					o->Light[2] /= 1.08f;
				}
			}
		}
		break;
	case MODEL_FENRIR_FOOT_THUNDER:
		if (o->Flags.Get(OBJECT::eOF_LIVE) == true)
		{
			o->Angle[0] += 0.1f;

			if (o->SubType == 1)
			{
				o->Light[1] -= 0.05f;
				o->Light[2] -= 0.05f;
			}
			else if (o->SubType == 2)
			{
				o->Light[0] -= 0.05f;
				o->Light[1] -= 0.05f;
			}
			else if (o->SubType == 3)
			{
				o->Light[0] -= 0.05f;
				o->Light[2] -= 0.05f;
			}
			else if (o->SubType == 4)
			{
				o->Light[2] -= 0.05f;
			}
			o->Alpha -= 0.05f;
			if (o->Alpha <= 0.0f)
				o->Flags.Clear(OBJECT::eOF_LIVE);

			if (g_CurrentTime - o->m_dwTime > 200)
			{
				++o->m_iAnimation;
				if (o->m_iAnimation > 4)
					o->Flags.Clear(OBJECT::eOF_LIVE);
				o->m_dwTime = g_CurrentTime;
			}
		}
		break;
	case MODEL_TWINTAIL_EFFECT:
		{
			if (o->SubType == 0)
			{
				o->Alpha -= 0.01f;
				if (o->Alpha <= 0.0f)
				{
					o->Flags.Clear(OBJECT::eOF_LIVE);
				}

				o->Light[0] /= 1.02f;
				o->Light[1] /= 1.02f;
				o->Light[2] /= 1.02f;

				if (g_CurrentTime - o->m_dwTime > 1000)
				{
					if (o->m_iAnimation == 0)
					{
						o->m_iAnimation = 1;
					}
					else
					{
						o->m_iAnimation = 0;
					}

					o->m_dwTime = g_CurrentTime;
				}

				if (o->m_iAnimation == 0)
				{
					o->Scale -= 0.015f;
					if (o->Scale <= 0.0f)
					{
						o->Scale = 0.0f;
					}
				}
				else
				{
					o->Scale += 0.02f;
					if (o->Scale >= 1.2f)
					{
						o->Scale = 1.2f;
					}
				}
			}
			else if (o->SubType == 1 || o->SubType == 2)
			{
				o->Alpha -= 0.01f;
				if (o->Alpha <= 0.0f)
				{
					o->Flags.Clear(OBJECT::eOF_LIVE);
				}

				o->Light[0] /= 1.04f;
				o->Light[1] /= 1.04f;
				o->Light[2] /= 1.04f;

				if (o->SubType == 1)
					o->Angle[0] = -(FWorldTime * 0.3f);
				else if (o->SubType == 2)
					o->Angle[0] = -(FWorldTime * 0.1f);

				o->Scale -= 0.02f;
			}
		}
		break;

	case MODEL_TOWER_GATE_PLANE:
		o->LifeTime = 100;
		if (o->Owner != nullptr && o->Flags.Get(OBJECT::eOF_LIVE) == true)
		{
			o->Position[2] = o->StartPosition[2] + EMath::Sin(FWorldTime*0.001f)*200.0f + 200.0f;
		}
		break;

	case BITMAP_CRATER:
		if (o->LifeTime<10)
		{
			o->Light[0] = o->LifeTime / 10.0f;
			o->Light[1] = o->Light[0];
			o->Light[2] = o->Light[0];
			o->Alpha = o->Light[0];
		}
		Vector(-0.5f, -0.5f, -0.5f, Light);
		AddTerrainLight(o->Position[0], o->Position[1], Light, (mu_int32)(o->StartPosition[0] - 1), PrimaryTerrainLight);
		break;

	case BITMAP_CHROME_ENERGY2:
		if (o->LifeTime<10)
		{
			o->Light[0] *= 0.8f;
			o->Light[1] *= 0.8f;
			o->Light[2] *= 0.8f;
		}
		break;

	case MODEL_STUN_STONE:
		if (o->SubType == 0)
		{
			o->Position[2] += o->Gravity;
			o->Gravity -= 15.0f;

			Height = RequestTerrainHeight(o->Position[0], o->Position[1]) + 50;
			if (o->Position[2] <= Height)
			{
				o->Position[2] = Height;
				if (o->ExtState == 0)
				{
					CreateEffect(ThreadIndex, BITMAP_CRATER, o->Position, o->Angle, o->Light, 1);
					o->ExtState = 1;
				}
			}
			else
			{
				VectorAdd(o->Position, o->StartPosition, o->Position);
				VectorScale(o->StartPosition, 0.9f, o->StartPosition);

				o->HeadAngle[0] -= o->Scale*32.0f;

				if (o->LifeTime % 3 == 0)
				{
					CreateParticle(ThreadIndex, BITMAP_ADV_SMOKE + 1, o->Position, o->Angle, o->Light, 1, 1.0f);
				}
			}
			o->Light[0] = o->LifeTime / 10.0f;
			o->Light[1] = o->Light[0];
			o->Light[2] = o->Light[0];
			o->Alpha = o->Light[0];
		}
		else if (o->SubType == 1)
		{
			if (o->LifeTime % 3 == 0)
			{
				CreateEffect(ThreadIndex, MODEL_STUN_STONE, o->Position, o->Angle, o->Light);
			}
			AddTerrainLight(o->Position[0], o->Position[1], o->Light, 1, PrimaryTerrainLight);
		}
		break;

	case MODEL_SKIN_SHELL:
		VectorAdd(o->Position, o->Direction, o->Position);
		VectorScale(o->Direction, 0.9f, o->Direction);
		o->Position[2] += o->Gravity;

		VectorScale(o->Direction, 1.1f, o->Direction);

		o->Gravity -= 3.0f;
		Height = RequestTerrainHeight(o->Position[0], o->Position[1]);
		if (o->Position[2] < Height)
		{
			o->Position[2] = Height;
			o->Gravity = -o->Gravity*0.2f;
			o->LifeTime -= 5;
			o->Angle[0] -= o->Scale*128.0f;
		}
		else
			o->Angle[0] -= o->Scale*16.0f;

		o->Alpha = o->LifeTime / 10.0f;
		break;

	case MODEL_MANA_RUNE:
		if (o->SubType == 0)
		{
			if (o->LifeTime>43)
			{
				o->HiddenMesh = -2;
			}
			else if (o->LifeTime>40)
			{
				o->HiddenMesh = 0;
			}
			else if (o->LifeTime>30)
			{
				o->Scale += o->Gravity;
				o->Gravity += 0.15f;
				if (o->Scale>1.0f)
				{
					o->Scale = 1.0f;
					o->Gravity = 0.01f;

					CreateEffect(ThreadIndex, MODEL_MANA_RUNE, o->Position, o->Angle, o->Light, 1);
				}
			}
			else if (o->LifeTime<15)
			{
				o->Scale -= o->Gravity;
				o->Gravity += 0.2f;
				if (o->Scale<0.0f)
				{
					o->Scale = 0.0f;
				}
				o->Alpha = (o->LifeTime / 20.0f);
				o->Position[2] -= 10.0f;
			}
		}
		else if (o->SubType == 1)
		{
			o->Scale -= 0.02f;
			if (o->Scale<1.0f)
			{
				o->Scale = 1.0f;
			}
			o->Alpha = (o->LifeTime / 20.0f);
		}
		break;

	case MODEL_SKILL_JAVELIN:
		if (o->Owner != nullptr)
		{
			VectorCopy(o->Owner->Position, o->StartPosition);
			o->StartPosition[2] += 150.0f;
		}

		o->Scale += 0.015f;
		if (o->LifeTime<25)
		{
			mu_float Distance;
			if (o->LifeTime<20)
			{
				Distance = MoveHumming(o->Position, o->HeadAngle, o->StartPosition, o->Velocity);
				if (Distance<100.0f)
				{
					VectorCopy(o->StartPosition, o->Position);
					o->Scale += 0.04f;

					if (o->LifeTime % 3 == 0)
					{
						CreateParticle(ThreadIndex, BITMAP_POUNDING_BALL, o->Position, o->Angle, o->Light, 1);
					}
				}
				else
				{
					CreateParticle(ThreadIndex, BITMAP_POUNDING_BALL, o->Position, o->Angle, o->Light, 1);
				}
			}
			o->Velocity += 1.5f;

			AngleMatrix(o->HeadAngle, Matrix);
			VectorRotate(o->Direction, Matrix, Position);
			VectorAdd(o->Position, Position, o->Position);

			o->Gravity = GetLargeRand(30) + 30.0f;

			if (o->Direction[1]>-50.0f)
			{
				o->Direction[1] -= 8.0f;
			}
		}
		else
		{
			o->Gravity += 10.0f;
			AngleMatrix(o->HeadAngle, Matrix);
			VectorRotate(o->Direction, Matrix, Position);
			VectorAdd(o->Position, Position, o->Position);
		}
		o->Angle[2] += o->Gravity;
		o->BlendMeshLight = o->LifeTime / 10.0f;
		o->Alpha = o->BlendMeshLight;

		Height = EMath::Sin(o->LifeTime*0.1f)*30.0f;
		if (o->SubType == 1)
		{
			o->Position[2] = o->StartPosition[2] + Height;
		}
		else if (o->SubType == 2)
		{
			o->Position[2] = o->StartPosition[2] - Height;
		}

		Vector(1.0f, 0.6f, 0.3f, Light);
		AddTerrainLight(o->Position[0], o->Position[1], Light, 2, PrimaryTerrainLight);
		break;

	case MODEL_ARROW_IMPACT:
		o->Angle[0] -= 5.0f;
		o->Direction[1] -= 8.0f;

		if (o->LifeTime<2 && o->SubType == 0 && o->Owner != nullptr)
		{
			o->Angle[0] = 90.0f;
			o->SubType = 1;

			VectorCopy(o->Owner->Position, Position);
			Position[0] += GetLargeRand(100) - 50.0f;
			Position[1] += GetLargeRand(100) - 50.0f;
			Position[2] += 1200.0f;
			CreateJoint(ThreadIndex, BITMAP_FLASH, Position, Position, o->Angle, 2, o, 50.0f);
			CreateJoint(ThreadIndex, BITMAP_FLASH, Position, Position, o->Angle, 3, o, 50.0f);
		}
		else if (o->SubType == 1)
		{
			o->Flags.Clear(OBJECT::eOF_LIVE);
		}
		break;

	case BITMAP_JOINT_FORCE:
		if (o->SubType == 0)
		{
			if (o->LifeTime<11 && o->LifeTime % 2 == 0)
			{
				Vector(90.0f, 0.0f, 0.0f, o->Angle);
				o->HeadAngle[2] += 72.0f;
				AngleMatrix(o->HeadAngle, Matrix);
				VectorRotate(o->Direction, Matrix, Position);
				VectorAdd(o->StartPosition, Position, Position);

				Position[2] += GetLargeRand(400) + 700.0f;
				CreateJoint(ThreadIndex, BITMAP_FLASH, Position, Position, o->Angle, 5, o, 110.0f);
			}
		}
		else if (o->SubType == 1)
		{
			if (o->LifeTime<11 && o->LifeTime % 2 == 0)
			{
				Vector(90.0f, 0.0f, 0.0f, o->Angle);
				o->HeadAngle[2] += 72.0f;
				AngleMatrix(o->HeadAngle, Matrix);
				VectorRotate(o->Direction, Matrix, Position);
				VectorAdd(o->StartPosition, Position, Position);

				Position[2] += 100.0f;
				CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER + 1, Position, Position, o->Angle, 6, o, 80.0f);
				CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER + 1, Position, Position, o->Angle, 6, o, 80.0f);
			}
		}

	case MODEL_SWORD_FORCE:
		if (o->SubType == 0)
		{
			if (o->LifeTime>12)
			{
				o->Scale += 0.9f;

				o->Direction[1] -= 2.0f;
				CreateEffect(ThreadIndex, MODEL_SWORD_FORCE, o->Position, o->Angle, o->Light, 1, o);
			}
			else
			{
				o->Scale -= 0.05f;
				o->BlendMeshLight = (mu_float)o->LifeTime / 18.0f;
				o->Alpha = o->BlendMeshLight;
				o->Light[0] = o->Alpha;
				o->Light[1] = o->Alpha;
				o->Light[2] = o->Alpha;

				o->Direction[1] -= 2.0f;

				VectorCopy(o->Position, Position);
				Position[0] += GetLargeRand(30) - 15.0f;
				Position[1] += GetLargeRand(30) - 15.0f;
				Position[2] -= 100.0f;
				for (mu_int32 i = 0; i<4; ++i)
				{
					Vector((mu_float)(GetLargeRand(60) + 60 + 90), 0.0f, o->Angle[2], Angle);
					CreateJoint(ThreadIndex, BITMAP_JOINT_SPARK, Position, Position, Angle);
					CreateParticle(ThreadIndex, BITMAP_FIRE, Position, Angle, o->Light, 2, 1.5f);
				}
			}
			Vector(1.0f, 0.8f, 0.6f, Light);
			AddTerrainLight(o->Position[0], o->Position[1], Light, 1, PrimaryTerrainLight);
		}
		else if (o->SubType == 1)
		{
			o->BlendMeshLight = (mu_float)o->LifeTime / 10.0f;
			o->Alpha = o->BlendMeshLight;
			o->Light[0] = o->Alpha;
			o->Light[1] = o->Alpha;
			o->Light[2] = o->Alpha;
		}
		break;

	case MODEL_PROTECTGUILD:
		{
			if (o->Owner == nullptr)
			{
				o->LifeTime = 0;
				break;
			}

			if (o->Alpha >= 1.0f) o->Angle[2] += 5.0f;

			if (o->LifeTime > 120)
			{
				if (o->LifeTime < 125 && o->LifeTime > 120)
				{
					for (mu_int32 i = 0; i < 5; ++i)
						CreateParticle(ThreadIndex, BITMAP_SPARK, o->Position, o->Angle, o->Light, 6, 1.5f, o);
				}
			}
			else if (o->LifeTime > 95)
			{
				if (o->Alpha > 1.0f) o->Alpha = 1.0f;
				else o->Alpha += 0.4f;
			}
			else if (o->LifeTime < 50)
			{
				if (o->Alpha < 0) o->Alpha = 0;
				else o->Alpha -= 0.1f;
			}

			MUModel *b = &MODELS::Data[o->Owner->Type];
			EVector3 tempPosition;
			Vector(0.0f, 0.0f, 0.0f, p);
			b->TransformPosition(ThreadIndex, o->Owner->BoneTransform[b->GetBoneIndex(20)], p, tempPosition, true);
			o->Position[0] = tempPosition[0] + (o->Owner->Position[0] - Hero->Object.Position[0]);
			o->Position[1] = tempPosition[1] + (o->Owner->Position[1] - Hero->Object.Position[1]);

			mu_float fHeight = o->Owner->Position[2] + tempPosition[2] - o->Owner->Position[2] + 60;
			if (fHeight < o->Position[2])
				o->Position[2] += (fHeight - o->Position[2]) * 0.1f;
			else
				o->Position[2] += (fHeight - o->Position[2]) * 0.5f;

			if (o->LifeTime < 60 && o->LifeTime > 39)
			{
				EVector3 p;
				for (mu_int32 i = 0; i < 1; ++i)
				{
					mu_float fAngle = GetLargeRand(360);
					Vector(o->Position[0] + (GetLargeRand(26) - 13) * EMath::Sin(fAngle),
						o->Position[1] + (GetLargeRand(26) - 13) * EMath::Cos(fAngle),
						o->Position[2] + 48 - o->LifeTime + (GetLargeRand(5)), p);
					CreateParticle(ThreadIndex, BITMAP_SPARK, p, o->Angle, o->Light, 5, 2.0f, o);
				}
			}
		}
		break;
	case MODEL_MOVE_TARGETPOSITION_EFFECT:
		{
			if (o->SubType == 0)
			{
				EVector3 vLight, vPos, vAngle, vRelativePos;
				Vector(1.0f, 0.7f, 0.3f, vLight);
				Vector(0.0f, 0.0f, 0.0f, vAngle);
				Vector(0.0f, 0.0f, 0.0f, vRelativePos);
				VectorCopy(o->Position, vPos);
				vPos[2] += 110.0f;

				{
					CreateParticle(ThreadIndex, BITMAP_SPARK + 1, vPos, o->Angle, vLight, 24, 1.0f, o);
				}


				if (o->LifeTime % 15 == 0)
				{
					CreateEffect(ThreadIndex, BITMAP_TARGET_POSITION_EFFECT1, o->Position, vAngle, vLight, 0);
				}

				if (o->LifeTime <= 10)
				{
					o->BlendMeshLight -= 0.05f;
					o->Light[0] *= o->BlendMeshLight;
					o->Light[1] *= o->BlendMeshLight;
					o->Light[2] *= o->BlendMeshLight;
				}
			}
		}
		break;
	case BITMAP_TARGET_POSITION_EFFECT1:
		{
			if (o->SubType == 0)
			{
				o->Scale -= 0.04f;
				if (o->Scale <= 0.2f)
				{
					o->Flags.Clear(OBJECT::eOF_LIVE);
				}
				if (o->LifeTime <= 10)
				{
					o->Alpha -= 0.05f;
					o->Light[0] *= o->Alpha;
					o->Light[1] *= o->Alpha;
					o->Light[2] *= o->Alpha;
				}

			}
		}
		break;
	case BITMAP_TARGET_POSITION_EFFECT2:
		{
			if (o->SubType == 0)
			{
				if (o->Scale >= 1.8f)
				{
					o->m_iAnimation = 0;
				}
				else if (o->Scale <= 0.8f)
				{
					o->m_iAnimation = 1;
				}

				if (o->m_iAnimation == 0)
				{
					o->Scale -= 0.15f;
				}
				else if (o->m_iAnimation == 1)
				{
					o->Scale += 0.15f;
				}

				if (o->LifeTime <= 10)
				{
					o->Alpha -= 0.05f;
					o->Light[0] *= o->Alpha;
					o->Light[1] *= o->Alpha;
					o->Light[2] *= o->Alpha;
				}
			}
		}
		break;
	case MODEL_EFFECT_SAPITRES_ATTACK:
		{
			if (o->LifeTime % 6 == 0)
			{
				o->Position[0] += (mu_float)((GetLargeRand(120)) - 60);
				o->Position[1] += (mu_float)((GetLargeRand(120)) - 60);

				CreateEffect(ThreadIndex, MODEL_EFFECT_SAPITRES_ATTACK_1, o->Position, o->Angle, o->Light, 0, o->Owner);
			}
		}
		break;
	case MODEL_EFFECT_SAPITRES_ATTACK_1:
		{
			if (o->SubType == 0)
			{
				mu_float fMoveSpeed = 40.0f;
				o->Position[0] += (o->Direction[0] * fMoveSpeed);
				o->Position[1] += (o->Direction[1] * fMoveSpeed);

				EVector3 vLight;
				Vector(0.1f, 0.3f, 1.0f, vLight);
				CreateSprite(BITMAP_LIGHT, o->Position, 3.0f, vLight, o);

				CheckTargetRange(ThreadIndex, o);

				if (o->LifeTime == 1)
				{
					for (mu_int32 i = 0; i<3; ++i)
					{
						CreateEffect(ThreadIndex, MODEL_EFFECT_SAPITRES_ATTACK_2, o->Position, o->Angle, vLight, 0);
					}
				}
			}
		}
		break;
	case MODEL_EFFECT_THUNDER_NAPIN_ATTACK_1:
		{
			if (o->SubType == 0)
			{
				EVector3 vLight, vLightFlare, vPos;
				Vector(0.4f, 0.7f, 1.0f, vLight);
				Vector(0.1f, 0.2f, 0.8f, vLightFlare);
				VectorCopy(o->Position, vPos);
				if (o->LifeTime >= 4)
				{
					o->Scale = 0.8f + ((mu_float)(GetLargeRand(10))*0.1f);
					vPos[0] += (3.0f*((mu_float)(GetLargeRand(40)) - 20));
					vPos[1] += (3.0f*((mu_float)(GetLargeRand(40)) - 20));
					vPos[2] += (3.0f*((mu_float)(GetLargeRand(40)) - 20));
					CreateParticle(ThreadIndex, BITMAP_LIGHTNING_MEGA1 + GetLargeRand(3), vPos, o->Angle, vLight, 0, o->Scale);
				}
				CreateSprite(BITMAP_LIGHT, o->Position, 8.0f, vLightFlare, o);
				CreateSprite(BITMAP_LIGHT, o->Position, 8.0f, vLightFlare, o);
			}
			else if (o->SubType == 1)
			{
				const mu_int32 iOffsetPos = 200;
				EVector3 vLight, vLightFlare, vPos;

				Vector(0.4f, 0.7f, 1.0f, vLight);
				Vector(0.1f, 0.2f, 0.8f, vLightFlare);
				VectorCopy(o->Position, vPos);

				vPos[0] = vPos[0] + (GetLargeRand(iOffsetPos) - (iOffsetPos*0.5f));
				vPos[1] = vPos[1] + (GetLargeRand(iOffsetPos) - (iOffsetPos*0.5f));
				vPos[2] = vPos[2] + (GetLargeRand(iOffsetPos / 2) - ((iOffsetPos*0.5f) * 0.5f));
				if (o->LifeTime >= 4)
				{
					o->Scale = (0.8f + ((mu_float)(GetLargeRand(10))*0.1f));
					vPos[0] += (3.0f*((mu_float)(GetLargeRand(40)) - 20));
					vPos[1] += (3.0f*((mu_float)(GetLargeRand(40)) - 20));
					vPos[2] += (3.0f*((mu_float)(GetLargeRand(40)) - 20));
					CreateParticle(ThreadIndex, BITMAP_LIGHTNING_MEGA1 + GetLargeRand(3), vPos, o->Angle, vLight, 0, o->Scale);
				}

				VectorCopy(o->Position, vPos);

				vPos[0] = vPos[0] + (GetLargeRand(iOffsetPos) - (iOffsetPos*0.5f));
				vPos[1] = vPos[1] + (GetLargeRand(iOffsetPos) - (iOffsetPos*0.5f));
				vPos[2] = vPos[2] + (GetLargeRand(iOffsetPos / 2) - ((iOffsetPos*0.5f) * 0.5f));
				CreateSprite(BITMAP_LIGHT, vPos, 8.0f, vLightFlare, o);
				CreateSprite(BITMAP_LIGHT, vPos, 8.0f, vLightFlare, o);
			}
		}
		break;
	case MODEL_EFFECT_SKURA_ITEM:
		{
			if ((o->SubType == 0) || (o->SubType == 1))
			{
				if (o->Owner->Flags.Get(OBJECT::eOF_LIVE) == true && o->Flags.Get(OBJECT::eOF_LIVE))
				{
					EVector3 vRelativePos, vtaWorldPos, vLight1, vLight2;
					MUModel *b = &MODELS::Data[MODEL_EFFECT_SKURA_ITEM];

					VectorCopy(o->Owner->Position, o->Position);
					VectorCopy(o->Position, b->BodyOrigin(ThreadIndex));
					Vector(0.0f, 0.0f, 0.0f, vRelativePos);
					Vector(1.0f, 0.6f, 0.8f, o->Light);
					Vector(0.3f, 0.3f, 0.3f, vLight1);
					Vector(1.0f, 0.6f, 0.8f, vLight2);

					b->Animation(ThreadIndex, ThreadInfo.BoneTransform, o->AnimationFrame, o->PriorAnimationFrame, o->PriorAction, o->Angle, o->HeadAngle);

					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(1)], vRelativePos, vtaWorldPos, false);
					CreateSprite(BITMAP_SHINY + 1, vtaWorldPos, 1.8f, o->Light, o, +FWorldTime*0.08f);
					for (mu_int32 i = 0; i<7; ++i)
					{
						CreateParticle(ThreadIndex, BITMAP_SHINY + 1, vtaWorldPos, o->Angle, o->Light, 5, 0.8f, o);
						CreateParticle(ThreadIndex, BITMAP_CHERRYBLOSSOM_EVENT_PETAL, vtaWorldPos, o->Angle, GetLargeRand(7) == 3 ? vLight2 : vLight1, 0, 0.5f);
					}

					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(2)], vRelativePos, vtaWorldPos, false);
					CreateSprite(BITMAP_SHINY + 1, vtaWorldPos, 1.8f, o->Light, o, -FWorldTime*0.08f);

					for (mu_int32 i = 0; i<7; ++i)
					{
						CreateParticle(ThreadIndex, BITMAP_SHINY + 1, vtaWorldPos, o->Angle, o->Light, 5, 0.8f, o);
						CreateParticle(ThreadIndex, BITMAP_CHERRYBLOSSOM_EVENT_PETAL, vtaWorldPos, o->Angle, GetLargeRand(7) == 3 ? vLight2 : vLight1, 0, 0.5f);
					}

					if (o->LifeTime == 30 || o->LifeTime == 15 || o->LifeTime == 4)
					{
						EVector3 vAngle;

						MUModel *characterBMD = &MODELS::Data[o->Owner->Type];
						VectorCopy(o->Position, characterBMD->BodyOrigin(ThreadIndex));

						characterBMD->Animation(ThreadIndex, o->Owner->BoneTransform, o->Owner->AnimationFrame,
							o->Owner->PriorAnimationFrame, o->Owner->PriorAction, o->Owner->Angle, o->Owner->HeadAngle);
						characterBMD->TransformPosition(ThreadIndex, o->Owner->BoneTransform[b->GetBoneIndex(20)], vRelativePos, vtaWorldPos, false);

						if (GetLargeRand(2) == 0) {
							vtaWorldPos[0] += GetLargeRand(40) + 10;
						}
						else {
							vtaWorldPos[0] -= GetLargeRand(30) + 10;
						}

						vtaWorldPos[2] += GetLargeRand(110) + 50;

						Vector(0.7f, 0.71f, 1.0f, vLight1);
						Vector(0.8f, 0.85f, 1.0f, vLight2);

						for (mu_int32 k = 0; k<70; ++k)
						{
							if (GetLargeRand(3) == 0) {
								CreateParticle(ThreadIndex, BITMAP_CHERRYBLOSSOM_EVENT_PETAL, vtaWorldPos, o->Angle, GetLargeRand(7) == 3 ? vLight2 : vLight1, 0, 0.4f);
							}
							CreateParticle(ThreadIndex, BITMAP_CHERRYBLOSSOM_EVENT_FLOWER, vtaWorldPos, o->Angle, GetLargeRand(4) == 3 ? vLight2 : vLight1, 0, 0.4f);
						}

						Vector(0.0f, 1.0f, 0.0f, vAngle);
						Vector(1.0f, 0.6f, 0.8f, vLight2);
						CreateParticle(ThreadIndex, BITMAP_SHOCK_WAVE, vtaWorldPos, vAngle, vLight2, 4, 0.005f);

						characterBMD->Animation(ThreadIndex, o->Owner->BoneTransform, o->Owner->AnimationFrame,
							o->Owner->PriorAnimationFrame, o->Owner->PriorAction, o->Owner->Angle, o->Owner->HeadAngle);
						characterBMD->TransformPosition(ThreadIndex, o->Owner->BoneTransform[b->GetBoneIndex(20)], vRelativePos, vtaWorldPos, false);

						vtaWorldPos[2] += 70;
						Vector(1.0f, 0.3f, 0.6f, vLight2);
						CreateSprite(BITMAP_SHINY + 1, vtaWorldPos, 1.8f, o->Light, o, -FWorldTime*0.08f);

						StopBackground(SOUND_CHERRYBLOSSOM_EFFECT1);
						PlayBackground(SOUND_CHERRYBLOSSOM_EFFECT1);
					}

					b->PlayAnimation(ThreadIndex, &o->AnimationFrame, &o->PriorAnimationFrame, &o->PriorAction, o->Velocity / 3.0f);
				}
			}
		}
		break;

	case MODEL_BLOW_OF_DESTRUCTION:
		{
			if (o->SubType == 0)
			{
				EVector3 vLight;
				if (o->LifeTime <= 24)
				{
					mu_float earthquake = 0.0f;
					if (o->LifeTime >= 15.0f)
					{
						earthquake = (mu_float)(GetLargeRand(8) - 4)*0.1f;
					}
					EarthQuake.Store(earthquake);

					if (o->LifeTime == 23)
					{
						Vector(0.3f, 0.3f, 1.0f, vLight);

						CreateEffect(ThreadIndex, MODEL_NIGHTWATER_01, o->Position, o->Angle, vLight, 0, o);
						CreateEffect(ThreadIndex, MODEL_NIGHTWATER_01, o->Position, o->Angle, vLight, 0, o);

						CreateEffect(ThreadIndex, MODEL_KNIGHT_PLANCRACK_A, o->Position, o->Angle, vLight, 0, o, 0, 0, 0, 0, 1.2f);

						EVector3 vDir, vPos, vAngle;
						VectorSubtract(o->StartPosition, o->Position, vDir);
						mu_float fLength = VectorLength(vDir);
						VectorNormalize(vDir);
						mu_int32 iNum = (mu_int32)(fLength / 100) + 1;
						for (mu_int32 i = 0; i<iNum; ++i)
						{
							VectorScale(vDir, 55.0f * i, vPos);
							VectorAdd(o->Position, vPos, vPos);
							VectorCopy(o->Owner->Angle, vAngle);
							if (i % 2 == 0)
							{
								vAngle[2] += (GetLargeRand(20) + 10);
							}
							else
							{
								vAngle[2] -= (GetLargeRand(20) + 10);
							}
							CreateEffect(ThreadIndex, MODEL_KNIGHT_PLANCRACK_B, vPos, vAngle, vLight, 0, o, 0, 0, 0, 0, 1.0f);
						}
					}

					o->Light[0] /= 1.05f;
					o->Light[1] /= 1.05f;
					o->Light[2] /= 1.05f;
				}
			}
			else if (o->SubType == 1)
			{
				if (o->LifeTime <= 24)
				{
					if (o->LifeTime >= 15)
					{
						EVector3 vPos, vLight;
						for (mu_int32 i = 0; i<15; ++i)
						{
							VectorCopy(o->Position, vPos);
							vPos[0] += GetLargeRand(300) - 150;
							vPos[1] += GetLargeRand(300) - 150;
							vPos[2] += GetLargeRand(300) - 150;
							mu_float fScale = 1.6f + GetLargeRand(10) * 0.1f;
							Vector(0.5f, 0.5f, 1.0f, vLight);
							mu_int32 index = (GetLargeRand(2) == 0) ? BITMAP_WATERFALL_5 : BITMAP_WATERFALL_3;
							CreateParticle(ThreadIndex, index, vPos, o->Angle, vLight, 8, fScale);
							Vector(1.0f, 1.0f, 1.0f, vLight);
							if (GetLargeRand(2) == 0)
							{
								CreateParticle(ThreadIndex, BITMAP_SMOKE, vPos, o->Angle, vLight, 55, 1.0f);
							}
						}
					}

					if (o->LifeTime == 23)
					{
						EVector3 vLight;
						Vector(0.3f, 0.3f, 1.0f, vLight);

						CreateEffect(ThreadIndex, MODEL_NIGHTWATER_01, o->Position, o->Angle, vLight, 0, o, -1, 0, 0, 0, 2.0f);
						CreateEffect(ThreadIndex, MODEL_NIGHTWATER_01, o->Position, o->Angle, vLight, 0, o, -1, 0, 0, 0, 1.0f);

						CreateEffect(ThreadIndex, MODEL_RAKLION_BOSS_CRACKEFFECT, o->Position, o->Angle, vLight, 0, o, -1, 0, 0, 0, 0.2f);

						EVector3 vPos, vResult;
						EVector3 vAngle;
						EBone Matrix;
						for (mu_int32 i = 0; i<(5 + GetLargeRand(3)); ++i)
						{
							Vector(0.0f, (mu_float)(GetLargeRand(150)), 0.0f, vPos);
							Vector(0.0f, 0.0f, (mu_float)(GetLargeRand(360)), vAngle);
							AngleMatrix(vAngle, Matrix);
							VectorRotate(vPos, Matrix, vResult);
							VectorAdd(vResult, o->Position, vResult);

							CreateEffect(ThreadIndex, MODEL_STONE1 + GetLargeRand(2), vResult, o->Angle, o->Light, 13);
						}
					}

					o->Light[0] /= 1.05f;
					o->Light[1] /= 1.05f;
					o->Light[2] /= 1.05f;
				}
			}
		}
		break;

	case MODEL_NIGHTWATER_01:
		{
			o->Alpha -= 0.04f;
		}
		break;

	case MODEL_KNIGHT_PLANCRACK_A:
		if (o->SubType == 0)
		{
			o->Alpha -= 0.04f;
		}
		else if (o->SubType == 1)
		{
			o->Alpha *= 0.9f;
		}
		break;

	case MODEL_KNIGHT_PLANCRACK_B:
		{
			o->Alpha -= 0.04f;
		}
		break;

	case MODEL_EFFECT_FLAME_STRIKE:
		{
			if (o->SubType == 0)
			{
				if ((o->LifeTime < 20 && o->Alpha < 0.1f) || o->Owner == nullptr)
				{
					o->Flags.Clear(OBJECT::eOF_LIVE);
					break;
				}
				if (o->LifeTime < 20) o->Alpha -= 0.1f;
				else if (o->Alpha < 1.0f) o->Alpha += 0.1f;

				OBJECT * pObject = o;
				MUModel * pModel = &MODELS::Data[pObject->Type];
				OBJECT * pOwner = pObject->Owner;
				MUModel * pOwnerModel = &MODELS::Data[pOwner->Type];

				mu_float Start_Frame = 1.0f;
				mu_float End_Frame = 14.0f;

				if (pOwner->AnimationFrame > End_Frame || pObject->AI == 1)
				{
					pObject->AI = 1;
				}
				else if (pOwner->CurrentAction == PLAYER_SKILL_FLAMESTRIKE)
				{
					pObject->AnimationFrame = pOwner->AnimationFrame;

					pOwnerModel->BodyScale(ThreadIndex) = pOwner->Scale;
					pOwnerModel->CurrentAction(ThreadIndex) = pOwner->CurrentAction;
					VectorCopy(pOwner->Angle, pOwnerModel->BodyAngle(ThreadIndex));
					VectorCopy(pOwner->Position, pOwnerModel->BodyOrigin(ThreadIndex));

					pModel->BodyScale(ThreadIndex) = pObject->Scale;
					pModel->CurrentAction(ThreadIndex) = pObject->CurrentAction;
					VectorCopy(pObject->Angle, pModel->BodyAngle(ThreadIndex));
					VectorCopy(pObject->Position, pModel->BodyOrigin(ThreadIndex));
					pModel->CurrentAnimation(ThreadIndex) = pObject->AnimationFrame;
					pModel->CurrentAnimationFrame(ThreadIndex) = (mu_int32)pObject->AnimationFrame;

					EVector3  vLight;
					Vector(1.0f, 1.0f, 1.0f, vLight);

					EVector3 StartPos, StartRelative;
					EVector3 EndPos, EndRelative;

					mu_float fOwnerActionSpeed = GetPlaySpeed(pOwner, pOwner->Type, pOwner->CurrentAction);
					mu_float fOwnerSpeedPerFrame = fOwnerActionSpeed / 10.0;
					mu_float fOwnerAnimationFrame = pOwner->AnimationFrame - fOwnerActionSpeed;

					mu_float fActionSpeed = pObject->Velocity;
					mu_float fSpeedPerFrame = fActionSpeed / 10.0;
					mu_float fAnimationFrame = pObject->AnimationFrame - fActionSpeed;

					for (mu_int32 i = 0; i<10; ++i)
					{
						pOwnerModel->Animation(ThreadIndex, ThreadInfo.BoneTransform,
							fOwnerAnimationFrame, (mu_int32)fOwnerAnimationFrame - 1,
							pOwner->PriorAction, pOwner->Angle, pOwner->HeadAngle, false, false);
						pOwnerModel->RotationPosition(ThreadIndex, ThreadInfo.BoneTransform[pOwnerModel->GetBoneIndex(33)], p, p);

						pModel->Animation(ThreadIndex, ThreadInfo.BoneTransform, fAnimationFrame,
							(mu_int32)fAnimationFrame - 1, pObject->PriorAction,
							pObject->Angle, pObject->HeadAngle, true, true);

						if (fOwnerAnimationFrame >= Start_Frame && fOwnerAnimationFrame <= End_Frame)
						{
							Vector(0.0f, 0.0f, 0.0f, StartRelative);
							Vector(0.0f, 0.0f, 0.0f, EndRelative);
							pModel->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[pModel->GetBoneIndex(9)], StartRelative, StartPos, true);
							pModel->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[pModel->GetBoneIndex(6)], EndRelative, EndPos, true);
							CreateObjectBlur(pObject, StartPos, EndPos, vLight, 2, false, o->m_iAnimation + 1);
							CreateObjectBlur(pObject, StartPos, EndPos, vLight, 2, false, o->m_iAnimation + 2);

							pModel->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[pModel->GetBoneIndex(8)], StartRelative, StartPos, true);
							pModel->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[pModel->GetBoneIndex(5)], EndRelative, EndPos, true);
							CreateObjectBlur(pObject, StartPos, EndPos, vLight, 5, false, o->m_iAnimation + 3);
						}

						fOwnerAnimationFrame += fOwnerSpeedPerFrame;
						fAnimationFrame += fSpeedPerFrame;
					}
				}
			}
		}
		break;

	case MODEL_SWELL_OF_MAGICPOWER:
		{
			if (o->SubType == 0)
			{
				if (o->Owner->Type != MODEL_PLAYER)
					break;

				MUModel* pModel = &MODELS::Data[o->Owner->Type];
				VectorCopy(o->Owner->Position, o->Position);
				pModel->BodyScale(ThreadIndex) = o->Owner->Scale;
				EVector3 vLight, vPos;

				Vector(0.4f, 0.3f, 0.9f, vLight);
				if (o->LifeTime == 45 || o->LifeTime == 35 || o->LifeTime == 25)
				{
					CreateEffect(ThreadIndex, BITMAP_SHOCK_WAVE, o->Position, o->Angle, vLight, 14, o, -1, 0, 0, 0, 5.0f);
					CreateEffect(ThreadIndex, BITMAP_SHOCK_WAVE, o->Position, o->Angle, vLight, 14, o, -1, 0, 0, 0, 5.0f);
					CreateEffect(ThreadIndex, BITMAP_TWLIGHT, o->Position, o->Angle, vLight, 3, o, -1, 0, 0, 0, 6.0f);
				}

				Vector(0.7f, 0.3f, 0.9f, vLight);
				if (o->LifeTime <= 20)
				{
					EVector3 vDLight;
					mu_float fDynamicLightVal = o->LifeTime*0.05f;
					Vector(fDynamicLightVal*vLight[0], fDynamicLightVal*vLight[1], fDynamicLightVal*vLight[2], vDLight);
					for (mu_int32 i = 0; i<pModel->_BoneNonDummyEnd; ++i)
					{
						pModel->TransformByObjectBoneDirect(ThreadIndex, vPos, o->Owner, i);

						CreateSprite(BITMAP_LIGHT, vPos, 1.5f, vDLight, o);
					}
				}

				Vector(0.2f, 0.2f, 0.9f, vLight);
				if (o->LifeTime == 45)
				{
					pModel->TransformByObjectBone(ThreadIndex, vPos, o->Owner, 28);
					CreateEffect(ThreadIndex, MODEL_ARROWSRE06, vPos, o->Angle, vLight, 1, o->Owner, 28);
					pModel->TransformByObjectBone(ThreadIndex, vPos, o->Owner, 37);
					CreateEffect(ThreadIndex, MODEL_ARROWSRE06, vPos, o->Angle, vLight, 1, o->Owner, 37);
				}

				if (o->LifeTime >= 30)
				{
					Vector(0.3f, 0.2f, 0.9f, vLight);
					for (mu_int32 i = 0; i<2; ++i)
					{
						CreateJoint(ThreadIndex, BITMAP_2LINE_GHOST, o->Position, o->Position, o->Angle, 1, o, 20.0f + GetLargeRand(10), -1, 0, 0, -1, &vLight);
					}
				}
			}
		}break;
	case MODEL_ARROWSRE06:
		{
			if (o->SubType == 0)
			{
				if (o->Owner->Type != MODEL_PLAYER)
					break;

				MUModel* pModel = &MODELS::Data[o->Owner->Type];
				EVector3 vPos;
				pModel->TransformByObjectBone(ThreadIndex, vPos, o->Owner, o->PKKey);
				VectorCopy(vPos, o->Position);
				o->Scale -= 1.0f;
				if (o->LifeTime <= 15)
				{
					o->Scale += 0.5f;
				}
			}
			else if (o->SubType == 1)
			{
				if (o->Owner->Type != MODEL_PLAYER)
					break;

				MUModel* pModel = &MODELS::Data[o->Owner->Type];
				EVector3 vPos;
				pModel->TransformByObjectBone(ThreadIndex, vPos, o->Owner, o->PKKey);
				VectorCopy(vPos, o->Position);

				if (o->LifeTime >= 15)
				{
					o->Scale *= 1.05f;
				}
				else
				{
					o->Scale *= 0.95f;
				}

				CreateSprite(BITMAP_LIGHT, o->Position, o->Scale, o->Light, o->Owner);
				CreateSprite(BITMAP_LIGHT, o->Position, o->Scale*0.8f, o->Light, o->Owner);

				if (o->LifeTime <= 10)
				{
					o->Alpha *= 0.95f;
				}
			}
			else if (o->SubType == 2)
			{
				EVector3 vRelativePos;
				MUModel* b = &MODELS::Data[o->Owner->Type];

				if (o->Owner->Type != MODEL_PLAYER)
					break;

				if (o->LifeTime > 15)
				{
					o->Scale *= 0.9f;
				}
				else if (o->LifeTime == 15)
				{
					o->Scale = 2.0f;
				}
				else
				{
					o->Scale *= 0.65f;
				}


				if (o->PKKey == 0)
				{
					Vector(13.0f, 14.0f, -10.0f, vRelativePos);
					b->TransformPosition(ThreadIndex, o->Owner->BoneTransform[b->GetBoneIndex(20)], vRelativePos, o->Position);
					VectorAdd(o->Position, o->Owner->Position, o->Position);
				}
				else
				{
					Vector(13.0f, 14.0f, 10.0f, vRelativePos);
					b->TransformPosition(ThreadIndex, o->Owner->BoneTransform[b->GetBoneIndex(20)], vRelativePos, o->Position);
					VectorAdd(o->Position, o->Owner->Position, o->Position);
				}

				CreateSprite(BITMAP_LIGHT, o->Position, o->Scale, o->Light, o->Owner);
				CreateSprite(BITMAP_LIGHT, o->Position, o->Scale*0.8f, o->Light, o->Owner);

				if (o->LifeTime <= 10)
				{
					o->Alpha *= 0.95f;
				}
			}
		}break;
	case MODEL_SWELL_OF_MAGICPOWER_BUFF_EFF:
		{
			if (o->SubType == 0)
			{
				VectorCopy(o->Owner->Position, o->Position);

				mu_int32 iPastTime = FWorldTime - o->Timer;

				if (iPastTime >= 6000)
				{
					EVector3 vLight;
					Vector(0.2f, 0.2f, 0.9f, vLight);

					CreateEffect(ThreadIndex, MODEL_ARROWSRE06, o->Position, o->Angle, vLight, 1, o->Owner, 28);

					CreateEffect(ThreadIndex, MODEL_ARROWSRE06, o->Position, o->Angle, vLight, 1, o->Owner, 37);

					o->Timer = FWorldTime;

				}

				o->LifeTime = 999;
			}
		}break;

	case MODEL_STAR_SHINE:
		{
			if (o->SubType == 0)
			{
				if (o->LifeTime <= 10)
				{
					o->Scale *= 0.9f;
					o->Alpha *= 0.9f;
					o->Light[0] *= 0.9f;
					o->Light[1] *= 0.9f;
					o->Light[2] *= 0.9f;
				}
				else if (o->LifeTime <= 20)
				{
					CreateSprite(BITMAP_SHINY, o->Position, o->Scale, o->Light, o, o->Angle[0]);
				}
				else if (o->LifeTime <= 30)
				{
					o->Scale *= 1.1f;
					o->Alpha *= 1.1f;
					o->Light[0] *= 1.1f;
					o->Light[1] *= 1.1f;
					o->Light[2] *= 1.1f;
				}

				CreateSprite(BITMAP_SHINY, o->Position, o->Scale, o->Light, o, o->Angle[0]);
			}
		}break;
	case MODEL_FEATHER:
		{
			switch (o->SubType)
			{
			case 2:
				{
					o->Light[0] *= 0.97f;
					o->Light[1] *= 0.97f;
					o->Light[2] *= 0.97f;

					o->Scale *= 0.97f;
					o->Alpha *= 0.97f;
					VectorAdd(o->EyeRight, o->Angle, o->Angle);
				}
				break;
			case 0:
				{
					o->Light[0] *= 0.97f;
					o->Light[1] *= 0.97f;
					o->Light[2] *= 0.97f;

					o->Alpha *= 0.97f;

					VectorAdd(o->EyeRight, o->Angle, o->Angle);
				}
				break;
			case 3:
				{
					o->Light[0] *= 0.97f;
					o->Light[1] *= 0.97f;
					o->Light[2] *= 0.97f;

					o->Alpha *= 0.97f;

					o->Scale *= 0.97f;

					VectorAdd(o->EyeRight, o->Angle, o->Angle);
				}break;
			case 1:
				{
					o->Light[0] *= 0.97f;
					o->Light[1] *= 0.97f;
					o->Light[2] *= 0.97f;

					o->Alpha *= 0.97f;

					o->Scale *= 0.99f;

					VectorAdd(o->EyeRight, o->Angle, o->Angle);
				}break;
			}
		}break;

	case MODEL_STREAMOFICEBREATH:
		{
			if (0 == o->SubType)
			{
				if (o->LifeTime > 1)
				{
					for (mu_int32 i_ = 0; i_ < 4; ++i_)
					{

						const	mu_float RANDOMOFFSET_ANGLE = 15.0f;
						const	mu_float RANDOMOFFSET_SCALE = 0.01f;
						const	mu_float RANDOMOFFSET_POSITION = 20.0f;
						const	mu_float RANDNUM = ((mu_float)((GetLargeRand(2000)) - 1000) * 0.001f);

						EVector3		v3ResultAngle, v3ResultPos, v3Light_;
						mu_float		fResultScale;
						mu_float		fCurrentOffsetScale, fCurrentOffsetPos, fCurrentOffsetAngle;

						fCurrentOffsetScale = RANDOMOFFSET_SCALE * RANDNUM;
						fCurrentOffsetPos = RANDOMOFFSET_POSITION * RANDNUM;
						fCurrentOffsetAngle = RANDOMOFFSET_ANGLE * RANDNUM;

						v3ResultPos[0] = o->StartPosition[0] + fCurrentOffsetPos;
						v3ResultPos[1] = o->StartPosition[1] + fCurrentOffsetPos;
						v3ResultPos[2] = o->StartPosition[2] + fCurrentOffsetPos;

						v3ResultAngle[0] = o->Angle[0] + fCurrentOffsetAngle;
						v3ResultAngle[1] = o->Angle[1] + fCurrentOffsetAngle;
						v3ResultAngle[2] = o->Angle[2] + fCurrentOffsetAngle;

						fResultScale = o->Scale + fCurrentOffsetScale;

						v3Light_[0] = 0.5f; v3Light_[1] = 0.6f; v3Light_[2] = 0.94f;

						CreateParticle(ThreadIndex, BITMAP_RAKLION_CLOUDS, v3ResultPos, v3ResultAngle, v3Light_, 1, fResultScale);
					}
				}
			}
		}break;

	case MODEL_1_STREAMBREATHFIRE:
		{
			EVector3	v3Pos;
			VectorCopy(o->Position, v3Pos);

			CreateParticle(ThreadIndex, BITMAP_WATERFALL_3, v3Pos, o->Angle, o->Light, 11, 0.6f);
			CreateParticle(ThreadIndex, BITMAP_SMOKE, v3Pos, o->Angle, o->Light, 52, 0.6f);

			if (15 == o->LifeTime)
			{
				const mu_float fForce = 20.0f;

				EMatrix4 matRotation;
				EVector3 vPos;
				EVector3 vDir, vDir_;

				Vector(0.0f, -fForce, 0.0f, vDir);
				AngleMatrix(o->Angle, matRotation);

				VectorRotate(vDir, matRotation, vDir_);

				VectorCopy(o->Position, vPos);

				CreateEffect(ThreadIndex, MODEL_MOONHARVEST_MOON, vPos, vDir_, o->Light,
					1,
					nullptr, -1, 0, 0, 0, 0.3f);
			}

		}
		break;

	case MODEL_PKFIELD_ASSASSIN_EFFECT_GREEN_HEAD:
	case MODEL_PKFIELD_ASSASSIN_EFFECT_RED_HEAD:
		{
			o->HeadAngle[2] -= o->Gravity;
			VectorCopy(o->Light, Light);

			o->Position[0] += o->HeadAngle[0];
			o->Position[1] += o->HeadAngle[1];
			o->Position[2] += o->HeadAngle[2];

			Height = RequestTerrainHeight(o->Position[0], o->Position[1]) + 20;

			if (o->Position[2] + o->Direction[2] <= Height)
			{
				o->Position[2] = Height;
				o->HeadAngle[0] *= 0.8f;
				o->HeadAngle[1] *= 0.8f;
				o->HeadAngle[2] += 0.6f*o->LifeTime;
				if (o->HeadAngle[2] < 5.0f)
					o->HeadAngle[2] = 0;

				o->Alpha -= 0.05f;
			}
			else
			{
				if (o->SubType == 0)
				{
					o->Angle[0] += 0.15f * o->LifeTime;
					o->Angle[1] += 0.15f * o->LifeTime;
				}
				else
				{
					o->Angle[0] -= 0.15f * o->LifeTime;
					o->Angle[1] -= 0.15f * o->LifeTime;
				}
			}

			EVector3 vLook, vPosition, vLight;
			EBone matRotate;
			Vector(1.0f, 1.0f, 1.0f, vLight);
			Vector(0.0f, 0.0f, 0.0f, vPosition);
			AngleMatrix(o->Angle, matRotate);
			VectorRotate(vPosition, matRotate, vLook);
			VectorAdd(o->Position, vLook, vPosition);
			if (o->Type == MODEL_PKFIELD_ASSASSIN_EFFECT_GREEN_HEAD)
			{
				CreateEffect(ThreadIndex, BITMAP_FIRE_CURSEDLICH, vPosition, o->Angle, vLight, 4, o);
			}
			else
			{
				CreateEffect(ThreadIndex, BITMAP_FIRE_CURSEDLICH, vPosition, o->Angle, vLight, 3, o);
			}
		}
		break;
	case MODEL_PKFIELD_ASSASSIN_EFFECT_GREEN_BODY:
	case MODEL_PKFIELD_ASSASSIN_EFFECT_RED_BODY:
		{
			++o->m_iAnimation;

			if (o->LifeTime < 20)
			{
				o->Alpha = 0.1f * o->LifeTime;
			}
		}
		break;

	case MODEL_LAVAGIANT_FOOTPRINT_R:
	case MODEL_LAVAGIANT_FOOTPRINT_V:
		{
			o->Angle[0] += 0.1f;

			o->Alpha -= 0.01f;
			if (o->Alpha <= 0.0f)
				o->Flags.Clear(OBJECT::eOF_LIVE);

			o->Light[0] *= 0.93f;
			o->Light[1] *= 0.93f;
			o->Light[2] *= 0.93f;
		}
		break;

	case MODEL_PROJECTILE:
		{
			if (o->LifeTime > 38)
			{
				MUModel *b = &MODELS::Data[o->Owner->Type];
				b->Animation(ThreadIndex, ThreadInfo.BoneTransform, o->Owner->AnimationFrame, o->Owner->PriorAnimationFrame, o->Owner->PriorAction, o->Owner->Angle, o->Owner->HeadAngle, false, false);
				EVector3 vRelativePos, vWorldPos;
				Vector(0, 0, 0, vRelativePos);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(9)], vRelativePos, vWorldPos, false);

				o->Position[0] = vWorldPos[0] + o->Owner->Position[0];
				o->Position[1] = vWorldPos[1] + o->Owner->Position[1];
				o->Position[2] = vWorldPos[2] + o->Owner->Position[2];
			}
			else
			{
				if (o->Gravity == 0.0f)
				{
					o->Gravity = 0.1f;

					o->Direction[0] = o->Position[0] - o->StartPosition[0];
					o->Direction[1] = o->Position[1] - o->StartPosition[1];
					o->Direction[2] = o->Position[2] - o->StartPosition[2];
					VectorNormalize(o->Direction);
				}
				o->Angle[0] += 0.5f;
				o->Velocity += 0.2f;

				o->Position[0] += o->Direction[0] * o->Velocity;
				o->Position[1] += o->Direction[1] * o->Velocity;
				o->Position[2] += o->Direction[2];
			}
		}
		break;
	case MODEL_DOOR_CRUSH_EFFECT_PIECE01:
	case MODEL_DOOR_CRUSH_EFFECT_PIECE02:
	case MODEL_DOOR_CRUSH_EFFECT_PIECE03:
	case MODEL_DOOR_CRUSH_EFFECT_PIECE04:
	case MODEL_DOOR_CRUSH_EFFECT_PIECE05:
	case MODEL_DOOR_CRUSH_EFFECT_PIECE06:
	case MODEL_DOOR_CRUSH_EFFECT_PIECE07:
	case MODEL_DOOR_CRUSH_EFFECT_PIECE08:
	case MODEL_DOOR_CRUSH_EFFECT_PIECE09:
	case MODEL_DOOR_CRUSH_EFFECT_PIECE11:
	case MODEL_DOOR_CRUSH_EFFECT_PIECE12:
	case MODEL_DOOR_CRUSH_EFFECT_PIECE13:
	case MODEL_STATUE_CRUSH_EFFECT_PIECE01:
	case MODEL_STATUE_CRUSH_EFFECT_PIECE02:
	case MODEL_STATUE_CRUSH_EFFECT_PIECE03:
		{
			o->HeadAngle[2] -= o->Gravity;
			VectorCopy(o->Light, Light);

			o->Position[0] += o->HeadAngle[0];
			o->Position[1] += o->HeadAngle[1];
			o->Position[2] += o->HeadAngle[2];

			Height = RequestTerrainHeight(o->Position[0], o->Position[1]) + 20;

			if (o->Position[2] + o->Direction[2] <= Height)
			{
				o->Position[2] = Height;
				o->HeadAngle[0] *= 0.8f;
				o->HeadAngle[1] *= 0.8f;
				o->HeadAngle[2] += 0.6f*o->LifeTime;
				if (o->HeadAngle[2] < 5.0f)
					o->HeadAngle[2] = 0;

				o->Alpha -= 0.05f;

				if (GetLargeRand(4) == 0)
				{
					Vector(0.0f, (mu_float)(GetLargeRand(80)) + 50.0f, 0.0f, p);
					Vector(0.0f, 0.0f, (mu_float)(GetLargeRand(360)), Angle);
					AngleMatrix(Angle, Matrix);
					VectorRotate(p, Matrix, Position);
					VectorAdd(Position, o->Position, Position);

					CreateEffect(ThreadIndex, MODEL_STONE1 + GetLargeRand(2), Position, o->Angle, o->Light, 0);
				}
			}
			else
			{
				if (o->SubType == 0)
				{
					o->Angle[0] += 0.15f * o->LifeTime;
					o->Angle[1] += 0.15f * o->LifeTime;
				}
				else
				{
					o->Angle[0] -= 0.15f * o->LifeTime;
					o->Angle[1] -= 0.15f * o->LifeTime;
				}
			}

			if (GetLargeRand(3) == 0)
			{
				CreateParticle(ThreadIndex, BITMAP_SMOKE + 1, o->Position, o->Angle, o->Light, 6);
			}
		}
		break;
	case MODEL_STATUE_CRUSH_EFFECT_PIECE04:
	case MODEL_DOOR_CRUSH_EFFECT_PIECE10:
		{
			o->LifeTime -= 1;

			if (o->LifeTime < 40)
			{
				o->Alpha -= 0.1f;
			}
		}
		break;

	case MODEL_SWORDLEFT01_EMPIREGUARDIAN_BOSS_GAION_:
	case MODEL_SWORDRIGHT01_EMPIREGUARDIAN_BOSS_GAION_:
	case MODEL_SWORDLEFT02_EMPIREGUARDIAN_BOSS_GAION_:
	case MODEL_SWORDRIGHT02_EMPIREGUARDIAN_BOSS_GAION_:
	case MODEL_SWORDMAIN01_EMPIREGUARDIAN_BOSS_GAION_:
	case MODEL_EMPIREGUARDIANBOSS_FRAMESTRIKE:
		{
			MUObjectExtra *ox = &g_EffectsExtra[iIndex];
			if (o->SubType == 0)
			{
			}
			else if (o->SubType == 1)
			{
				EVector3	v3RotateAngleRelative;
				o->Flags.Set(OBJECT::eOF_VISIBLE);

				mu_float	fCurrentRate = 1.0f - ((mu_float)o->LifeTime / (mu_float)o->ExtState);

				if (ox->m_Interpolates.m_vecInterpolatesAngle.empty() == false)
				{
					ox->m_Interpolates.GetAngleCurrent(v3RotateAngleRelative, fCurrentRate);

					o->Angle[0] = v3RotateAngleRelative[0] + o->HeadAngle[0];
					o->Angle[1] = v3RotateAngleRelative[1] + o->HeadAngle[1];
					o->Angle[2] = v3RotateAngleRelative[2] + o->HeadAngle[2];
				}

				if (ox->m_Interpolates.m_vecInterpolatesPos.empty() == false)
				{
					ox->m_Interpolates.GetPosCurrent(o->Position, fCurrentRate);
				}

				if (ox->m_Interpolates.m_vecInterpolatesScale.empty() == false)
				{
					ox->m_Interpolates.GetScaleCurrent(o->Scale, fCurrentRate);
				}

				if (ox->m_Interpolates.m_vecInterpolatesScale.empty() == false)
				{
					ox->m_Interpolates.GetAlphaCurrent(o->Alpha, fCurrentRate);
				}

				mu_float fRateBlurStart, fRateBlurEnd, fRateShadowStart, fRateShadowEnd, fRateJointStart, fRateJointEnd;
				fRateBlurStart = fRateBlurEnd = 0.0f;
				fRateShadowStart = fRateShadowEnd = 0.0f;
				fRateJointStart = fRateJointEnd = 0.0f;
				switch (o->Type)
				{
				case MODEL_SWORDRIGHT01_EMPIREGUARDIAN_BOSS_GAION_:
					{
						fRateBlurStart = 0.1f; fRateBlurEnd = 0.90f;
					}
					break;
				case MODEL_SWORDLEFT01_EMPIREGUARDIAN_BOSS_GAION_:
					{
						fRateBlurStart = 0.1f; fRateBlurEnd = 0.90f;
					}
					break;
				case MODEL_SWORDRIGHT02_EMPIREGUARDIAN_BOSS_GAION_:
					{
						fRateBlurStart = 0.3f; fRateBlurEnd = 1.01f;

						fRateShadowStart = 0.3f; fRateShadowEnd = 1.01f;
						fRateJointStart = 0.25f; fRateJointEnd = 0.29f;
					}
					break;
				case MODEL_SWORDLEFT02_EMPIREGUARDIAN_BOSS_GAION_:
					{
						fRateBlurStart = 0.3f; fRateBlurEnd = 1.01f;

						fRateShadowStart = 0.3f; fRateShadowEnd = 1.01f;
						fRateJointStart = 0.25f; fRateJointEnd = 0.29f;
					}
					break;
				case MODEL_SWORDMAIN01_EMPIREGUARDIAN_BOSS_GAION_:
					{
						fRateBlurStart = 0.0f; fRateBlurEnd = 0.0f;
					}
					break;
				case MODEL_EMPIREGUARDIANBOSS_FRAMESTRIKE:
					{
						fRateBlurStart = 0.10f; fRateBlurEnd = 1.01f;
					}
					break;
				}

				mu_int32 iTYPESWORDFORCE = 0;
				mu_int32 iTYPESWORDSHADOW = 0;
				mu_int32 iTYPESWORDJOINT = 0;

				switch (o->Type)
				{
				case MODEL_EMPIREGUARDIANBOSS_FRAMESTRIKE:
					{
						iTYPESWORDFORCE = 1;
						iTYPESWORDSHADOW = 0;
					}
					break;
				case MODEL_SWORDMAIN01_EMPIREGUARDIAN_BOSS_GAION_:
				case MODEL_SWORDLEFT01_EMPIREGUARDIAN_BOSS_GAION_:
				case MODEL_SWORDRIGHT01_EMPIREGUARDIAN_BOSS_GAION_:
					{
						iTYPESWORDFORCE = 1;
						iTYPESWORDSHADOW = 0;
					}
					break;
				case MODEL_SWORDLEFT02_EMPIREGUARDIAN_BOSS_GAION_:
				case MODEL_SWORDRIGHT02_EMPIREGUARDIAN_BOSS_GAION_:
					{
						iTYPESWORDFORCE = 0;
						iTYPESWORDSHADOW = 1;
						iTYPESWORDJOINT = 0;
					}
					break;
				}

				if (iTYPESWORDFORCE == 1)
				{
					if (fCurrentRate > fRateBlurStart && fCurrentRate < fRateBlurEnd)
					{
						MUModel*	b = &MODELS::Data[o->Type];
						EVector3  vLightBlur; Vector(1.0f, 1.0f, 1.0f, vLightBlur);
						mu_float	fPreRate = 1.0f - (mu_float)((o->LifeTime) + 1) / (mu_float)(o->ExtState);
						SETLIMITS(fPreRate, 1.0f, 0.0f);
						if (fPreRate < fCurrentRate)
						{
							mu_float	fStartRate, fEndRate;

							fStartRate = 1.0f - (mu_float)((o->LifeTime) + 2) / (mu_float)(o->ExtState);
							fEndRate = 1.0f - (mu_float)((o->LifeTime) + 1) / (mu_float)(o->ExtState);


							SETLIMITS(fStartRate, 1.0f, 0.0f);
							SETLIMITS(fEndRate, 1.0f, 0.0f);

							EVector3 *arrEachBonePos = ThreadInfo.BonePosition;

							EVector3 v3CurBlurAngle, v3CurBlurPos;
							mu_int32	iAccess = 10;
							mu_int32 iBone01, iBone02;
							mu_int32 iBlurIdentity, iTypeBlur;
							mu_int32 iBlurAccessTimeAttk01,
								iBlurAccessTimeAttk02,
								iBlurAccessTimeAttk03,
								iBlurAccessTimeAttk04;
							mu_float fUnit;
							mu_float fCurrentRateUnit = fStartRate;
							mu_float fScale = o->Scale;

							iBlurAccessTimeAttk01 = 3;
							iBlurAccessTimeAttk02 = 20;
							iBlurAccessTimeAttk03 = 5;
							iBlurAccessTimeAttk04 = 6;

							switch (o->Type)
							{
							case MODEL_SWORDRIGHT01_EMPIREGUARDIAN_BOSS_GAION_:
								{
									iAccess = iBlurAccessTimeAttk02;
									iBone01 = 3, iBone02 = 12;
									iBlurIdentity = 113;
									iTypeBlur = 13;
								}
								break;
							case MODEL_SWORDLEFT01_EMPIREGUARDIAN_BOSS_GAION_:
								{
									iAccess = iBlurAccessTimeAttk02;
									iBone01 = 13, iBone02 = 1;
									iBlurIdentity = 119;
									iTypeBlur = 13;
								}
								break;
							case MODEL_SWORDLEFT02_EMPIREGUARDIAN_BOSS_GAION_:
								{
									iAccess = iBlurAccessTimeAttk01;
									iBone01 = 4, iBone02 = 6;
									iBlurIdentity = 133;
									iTypeBlur = 10;
								}
								break;
							case MODEL_SWORDRIGHT02_EMPIREGUARDIAN_BOSS_GAION_:
								{
									iAccess = iBlurAccessTimeAttk01;
									iBone01 = 4, iBone02 = 6;
									iBlurIdentity = 122;
									iTypeBlur = 10;
								}
								break;
							case MODEL_EMPIREGUARDIANBOSS_FRAMESTRIKE:
								{
									iAccess = iBlurAccessTimeAttk04;
									iBone01 = 4, iBone02 = 9;
									iBlurIdentity = 155;
									iTypeBlur = 2;
								}
								break;
							}

							fUnit = (fEndRate - fStartRate) / (mu_float)iAccess;

							switch (o->Type)
							{
							case MODEL_SWORDRIGHT01_EMPIREGUARDIAN_BOSS_GAION_:
							case MODEL_SWORDLEFT01_EMPIREGUARDIAN_BOSS_GAION_:
								{
									for (mu_int32 i = 0; i<iAccess; ++i)
									{
										fCurrentRateUnit += fUnit;

										ox->m_Interpolates.GetAngleCurrent(v3CurBlurAngle, fCurrentRateUnit);
										ox->m_Interpolates.GetPosCurrent(v3CurBlurPos, fCurrentRateUnit);

										VectorAdd(v3CurBlurAngle, o->HeadAngle, v3CurBlurAngle);

										b->AnimationTransformOnlySelf(ThreadIndex, arrEachBonePos, v3CurBlurAngle, v3CurBlurPos, fScale);

										CreateObjectBlur(o,
											arrEachBonePos[iBone01],
											arrEachBonePos[iBone02],
											vLightBlur, iTypeBlur, false, iBlurIdentity, 25);

										if (GetLargeRand(2) == 0)
										{
											EVector3	vAngle, vRandomDir, vRandomDirPosition, vResultRandomPosition;
											EMatrix4	matRandomRotation;
											EVector3	vPosition;

											if (o->Type == MODEL_SWORDRIGHT01_EMPIREGUARDIAN_BOSS_GAION_)
											{
												VectorCopy(arrEachBonePos[12], vPosition);
											}
											else
											{
												VectorCopy(arrEachBonePos[1], vPosition);
											}


											mu_float	fRandDistance = (mu_float)(GetLargeRand(100)) + 100;
											Vector(0.0f, fRandDistance, 0.0f, vRandomDir);

											CreateParticle(ThreadIndex, BITMAP_FIRE, vPosition, o->Angle, o->Light, 5, 1.35f);

											Vector((mu_float)(GetLargeRand(360)), 0.0f, (mu_float)(GetLargeRand(360)), vAngle);
											AngleMatrix(vAngle, matRandomRotation);
											VectorRotate(vRandomDir, matRandomRotation, vRandomDirPosition);
											VectorAdd(vPosition, vRandomDirPosition, vResultRandomPosition);
											CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, vResultRandomPosition, vPosition, vAngle, 3, nullptr, 10.0f, 10, 10);
										}
									}
								}
								break;
							case MODEL_SWORDRIGHT02_EMPIREGUARDIAN_BOSS_GAION_:
							case MODEL_SWORDLEFT02_EMPIREGUARDIAN_BOSS_GAION_:
								{
									for (mu_int32 i = 0; i<iAccess; ++i)
									{
										fCurrentRateUnit += fUnit;

										ox->m_Interpolates.GetAngleCurrent(v3CurBlurAngle, fCurrentRateUnit);
										ox->m_Interpolates.GetPosCurrent(v3CurBlurPos, fCurrentRateUnit);

										VectorAdd(v3CurBlurAngle, o->HeadAngle, v3CurBlurAngle);

										b->AnimationTransformOnlySelf(ThreadIndex, arrEachBonePos, v3CurBlurAngle, v3CurBlurPos, fScale);

										CreateObjectBlur(o,
											arrEachBonePos[iBone01],
											arrEachBonePos[iBone02],
											vLightBlur, iTypeBlur, false, iBlurIdentity, 3);

										if (o->Type == MODEL_SWORDRIGHT01_EMPIREGUARDIAN_BOSS_GAION_ ||
											o->Type == MODEL_SWORDLEFT01_EMPIREGUARDIAN_BOSS_GAION_)
										{
											EVector3	vAngle, vRandomDir, vRandomDirPosition, vResultRandomPosition;
											EMatrix4	matRandomRotation;
											EVector3	vPosition;

											if (o->Type == MODEL_SWORDRIGHT01_EMPIREGUARDIAN_BOSS_GAION_)
											{
												VectorCopy(arrEachBonePos[12], vPosition);
											}
											else
											{
												VectorCopy(arrEachBonePos[1], vPosition);
											}


											mu_float	fRandDistance = (mu_float)(GetLargeRand(100)) + 100;
											Vector(0.0f, fRandDistance, 0.0f, vRandomDir);

											CreateParticle(ThreadIndex, BITMAP_FIRE, vPosition, o->Angle, o->Light, 0);

											Vector((mu_float)(GetLargeRand(360)), 0.0f, (mu_float)(GetLargeRand(360)), vAngle);
											AngleMatrix(vAngle, matRandomRotation);
											VectorRotate(vRandomDir, matRandomRotation, vRandomDirPosition);
											VectorAdd(vPosition, vRandomDirPosition, vResultRandomPosition);
											CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, vResultRandomPosition, vPosition, vAngle, 3, nullptr, 10.0f, 10, 10);
										}
									}
								}
								break;
							case MODEL_EMPIREGUARDIANBOSS_FRAMESTRIKE:
								{
									mu_float fRateBlur = 0.0f;

									for (mu_int32 i = 0; i<iAccess; ++i)
									{
										fCurrentRateUnit += fUnit;

										ox->m_Interpolates.GetAngleCurrent(v3CurBlurAngle, fCurrentRateUnit);
										ox->m_Interpolates.GetPosCurrent(v3CurBlurPos, fCurrentRateUnit);

										VectorAdd(v3CurBlurAngle, o->HeadAngle, v3CurBlurAngle);

										fRateBlur = (mu_float)i / (mu_float)iAccess;
										b->AnimationTransformOnlySelf(ThreadIndex, arrEachBonePos, v3CurBlurAngle, v3CurBlurPos, o->Scale, o, o->Velocity, fRateBlur);

										CreateObjectBlur(o, arrEachBonePos[iBone01],
											arrEachBonePos[iBone02], vLightBlur, 13, false, iBlurIdentity + 2, 11);
									}
								}
								break;
							}
						}
					}
				}

				if (iTYPESWORDSHADOW == 1)
				{
					if (fCurrentRate > fRateShadowStart && fCurrentRate < fRateShadowEnd)
					{
						CreateEffect(ThreadIndex, o->Type, o->Position, o->Angle, o->Light, 20, o, -1, 0, 0, 0, o->Scale);
					}
				}

				if (iTYPESWORDJOINT == 1)
				{
					if (fCurrentRate > fRateJointStart && fCurrentRate < fRateJointEnd)
					{
						CreateJoint(ThreadIndex, BITMAP_FLARE + 1, o->Position, o->Position, o->Angle, 20, o, 160.0f, 40);
					}
				}

				switch (o->Type)
				{
				case MODEL_SWORDRIGHT01_EMPIREGUARDIAN_BOSS_GAION_:
				case MODEL_SWORDLEFT01_EMPIREGUARDIAN_BOSS_GAION_:
				case MODEL_SWORDRIGHT02_EMPIREGUARDIAN_BOSS_GAION_:
				case MODEL_SWORDLEFT02_EMPIREGUARDIAN_BOSS_GAION_:
					{
						EVector3	v3LightTerrain;
						switch (o->Type)
						{
						case MODEL_SWORDRIGHT01_EMPIREGUARDIAN_BOSS_GAION_:
							{
								Vector(0.0f, 0.2f, 0.9f, v3LightTerrain);
								AddTerrainLight(o->Position[0], o->Position[1], v3LightTerrain, 2, PrimaryTerrainLight);
							}
							break;
						case MODEL_SWORDLEFT01_EMPIREGUARDIAN_BOSS_GAION_:
							{
								Vector(0.0f, 0.2f, 0.9f, v3LightTerrain);
								AddTerrainLight(o->Position[0], o->Position[1], v3LightTerrain, 2, PrimaryTerrainLight);
							}
							break;
						case MODEL_SWORDRIGHT02_EMPIREGUARDIAN_BOSS_GAION_:
							{
								Vector(0.0f, 0.2f, 0.9f, v3LightTerrain);
								AddTerrainLight(o->Position[0], o->Position[1], v3LightTerrain, 2, PrimaryTerrainLight);
							}
							break;
						case MODEL_SWORDLEFT02_EMPIREGUARDIAN_BOSS_GAION_:
							{
								Vector(0.0f, 0.2f, 0.9f, v3LightTerrain);
								AddTerrainLight(o->Position[0], o->Position[1], v3LightTerrain, 2, PrimaryTerrainLight);
							}
							break;
						}
					}
					break;
				case MODEL_SWORDMAIN01_EMPIREGUARDIAN_BOSS_GAION_:
					{
						if (fCurrentRate >= 0.0f && fCurrentRate <= 0.6f)
						{
							o->Flags.Set(OBJECT::eOF_VISIBLE);
							MUModel*	b = &MODELS::Data[o->Type];
							EVector3	*arrEachBonePos = ThreadInfo.BonePosition;
							EVector3	v3LightModify;

							EVector3	vRelativePos, vAngle;
							EVector3	v3CurrentHighHierarchyNodePos;

							Vector(4.0f, 0.0f, 0.0f, vRelativePos);
							VectorCopy(o->Angle, vAngle);

							MUModel *pBMDSwordModel = &MODELS::Data[o->Type];
							MUModel *pOwnerModel = &MODELS::Data[o->Owner->Type];

							mu_int32 arrBoneIdxs[] = { 4, 2, 8, 10, 6 };
							mu_int32 iBoneIdx = arrBoneIdxs[o->Type - MODEL_SWORDLEFT01_EMPIREGUARDIAN_BOSS_GAION_];

							pBMDSwordModel->AnimationTransformWithAttachHighModel(ThreadIndex, o->Owner, pOwnerModel, iBoneIdx,
								&v3CurrentHighHierarchyNodePos, arrEachBonePos);

							EVector3	v3LightTerrain;
							Vector(0.9f, 0.4f, 0.1f, v3LightTerrain);

							AddTerrainLight(o->Position[0], o->Position[1], v3LightTerrain, 2, PrimaryTerrainLight);

							{
								Vector(1.0f, 1.0f, 1.0f, v3LightModify);

								for (mu_int32 j = 0; j < 11; ++j)
								{
									if (GetLargeRand(20) == 0)
									{
										CreateEffect(ThreadIndex, BITMAP_FIRE_CURSEDLICH, arrEachBonePos[j], vAngle, v3LightModify, 12, o, -1, 0, 0, 0, o->Scale);
									}
								}
							}
						}
					}
					break;
				case MODEL_EMPIREGUARDIANBOSS_FRAMESTRIKE:
					{
						if (fCurrentRate >= 0.0f && fCurrentRate <= 0.5f)
						{
							EVector3	vAngle;
							EVector3	v3LightModify, v3PosModify;

							MUModel	*b = &MODELS::Data[o->Type];
							EVector3* arrEachBonePos = ThreadInfo.BonePosition;

							b->AnimationTransformOnlySelf(ThreadIndex, arrEachBonePos, o->Angle, o->Position, o->Scale, o);

							VectorCopy(o->Angle, vAngle);
							Vector(1.0f, 1.0f, 1.0f, v3LightModify);

							for (mu_int32 i = 0; i < b->_BoneNonDummyEnd; ++i)
							{
								v3PosModify[0] = arrEachBonePos[i][0] + (mu_float)((GetLargeRand(80)) - 40);
								v3PosModify[1] = arrEachBonePos[i][1] + (mu_float)((GetLargeRand(80)) - 40);
								v3PosModify[2] = arrEachBonePos[i][2] + (mu_float)((GetLargeRand(80)) - 40);
								if (GetLargeRand(10) == 0)
								{
									CreateEffect(ThreadIndex, BITMAP_FIRE_CURSEDLICH, v3PosModify, vAngle, v3LightModify, 12, o, -1, 0, 0, 0, o->Scale * 1.5f);
								}
							}
						}


						EVector3	v3LightTerrain;
						Vector(0.9f, 0.4f, 0.1f, v3LightTerrain);

						AddTerrainLight(o->Position[0], o->Position[1], v3LightTerrain, 2, PrimaryTerrainLight);
					}
					break;
				}

				switch (o->Type)
				{
				case MODEL_SWORDLEFT01_EMPIREGUARDIAN_BOSS_GAION_:
				case MODEL_SWORDRIGHT01_EMPIREGUARDIAN_BOSS_GAION_:
					{
						if (o->LifeTime == 5)
						{
							CreateEffect(ThreadIndex, o->Type, o->Position, o->Angle, o->Light, 11, o->Owner, -1, 0, 0, 0, o->Scale);
						}
					}
					break;
				case MODEL_SWORDRIGHT02_EMPIREGUARDIAN_BOSS_GAION_:
				case MODEL_SWORDLEFT02_EMPIREGUARDIAN_BOSS_GAION_:
					{
						if (o->LifeTime == 3)
						{
							CreateEffect(ThreadIndex, o->Type, o->Position, o->Angle, o->Light, 11, o->Owner, -1, 0, 0, 0, o->Scale);
						}
					}
					break;
				case MODEL_EMPIREGUARDIANBOSS_FRAMESTRIKE:
					{
						if (o->LifeTime == 10)
						{
							CreateEffect(ThreadIndex, MODEL_SWORDMAIN01_EMPIREGUARDIAN_BOSS_GAION_,
								o->Position, o->Angle, o->Light, 11, o->Owner, -1, 0, 0, 0, o->Scale);
						}
					}
					break;
				case MODEL_SWORDMAIN01_EMPIREGUARDIAN_BOSS_GAION_:
					{
						if (o->LifeTime == 1)
						{
							CreateEffect(ThreadIndex, MODEL_EMPIREGUARDIANBOSS_FRAMESTRIKE,
								o->Position, o->Angle, o->Light, 1,
								o->Owner, -1, 0, 0, 0, o->Scale);
						}
					}
					break;
				}

				switch (o->Type)
				{
				case MODEL_SWORDRIGHT01_EMPIREGUARDIAN_BOSS_GAION_:
				case MODEL_SWORDLEFT01_EMPIREGUARDIAN_BOSS_GAION_:
					{
						if (o->LifeTime == o->ExtState - 1)
						{
							PlayBackground(SOUND_BLOODATTACK);
						}
					}
					break;
				case MODEL_SWORDRIGHT02_EMPIREGUARDIAN_BOSS_GAION_:
				case MODEL_SWORDLEFT02_EMPIREGUARDIAN_BOSS_GAION_:
					{
						if (o->LifeTime == o->ExtState - 1)
						{
							PlayBackground(SOUND_ATTACK01 + 4);
						}
					}
					break;
				case MODEL_EMPIREGUARDIANBOSS_FRAMESTRIKE:
					{
						if (o->LifeTime == o->ExtState - 5)
						{
							PlayBackground(SOUND_SKILL_FLAME_STRIKE);
						}
					}
					break;
				}

			}
			else if (o->SubType == 3)
			{
				o->Flags.Set(OBJECT::eOF_VISIBLE);

				EVector3 v3RotateAngleRelative;

				mu_float fCurrentRate = 1.0f - ((mu_float)o->LifeTime / (mu_float)o->ExtState);

				if (ox->m_Interpolates.m_vecInterpolatesAngle.empty() == false)
				{
					ox->m_Interpolates.GetAngleCurrent(v3RotateAngleRelative, fCurrentRate);

					o->Angle[0] = v3RotateAngleRelative[0] + o->HeadAngle[0];
					o->Angle[1] = v3RotateAngleRelative[1] + o->HeadAngle[1];
					o->Angle[2] = v3RotateAngleRelative[2] + o->HeadAngle[2];
				}

				if (ox->m_Interpolates.m_vecInterpolatesPos.empty() == false)
				{
					ox->m_Interpolates.GetPosCurrent(o->Position, fCurrentRate);
				}

				if (ox->m_Interpolates.m_vecInterpolatesScale.empty() == false)
				{
					ox->m_Interpolates.GetScaleCurrent(o->Scale, fCurrentRate);
				}

				if (ox->m_Interpolates.m_vecInterpolatesAlpha.empty() == false)
				{
					ox->m_Interpolates.GetAlphaCurrent(o->Alpha, fCurrentRate);
				}

				switch (o->Type)
				{
				case MODEL_SWORDRIGHT01_EMPIREGUARDIAN_BOSS_GAION_:
				case MODEL_SWORDLEFT01_EMPIREGUARDIAN_BOSS_GAION_:
				case MODEL_SWORDRIGHT02_EMPIREGUARDIAN_BOSS_GAION_:
				case MODEL_SWORDLEFT02_EMPIREGUARDIAN_BOSS_GAION_:
					{
						if (o->LifeTime == 0)
						{
							OBJECT	*oHighHier = o->Owner;

							EVector3	v3RelativePos, v3OwnerBonePos;
							Vector(0.0f, 0.0f, 0.0f, v3RelativePos);
							Vector(0.0f, 0.0f, 0.0f, v3OwnerBonePos);

							CreateEffect(ThreadIndex, o->Type, oHighHier->Position, oHighHier->Angle, oHighHier->Light,
								11, o->Owner, -1, 0, 0, 0, o->Scale);
						}

						EVector3	v3LightTerrain;
						Vector(0.0f, 0.2f, 0.9f, v3LightTerrain);
						AddTerrainLight(o->Position[0], o->Position[1], v3LightTerrain, 2, PrimaryTerrainLight);
					}
					break;
				case MODEL_SWORDMAIN01_EMPIREGUARDIAN_BOSS_GAION_:
					{
						if (o->LifeTime == 40)
						{
							EBone Matrix;
							EVector3 vAngle, vDirection, vPosition, vLight;
							mu_float fAngle;

							Vector(1.0f, 1.0f, 1.0f, vLight);
							for (mu_int32 i = 0; i < 5; ++i)
							{
								Vector(0.0f, 200.0f, 0.0f, vDirection);
								fAngle = o->Angle[2] + i * 72.0f;
								Vector(0.0f, 0.0f, fAngle, vAngle);
								AngleMatrix(vAngle, Matrix);
								VectorRotate(vDirection, Matrix, vPosition);
								VectorAdd(vPosition, o->Light, vPosition);

								CreateEffect(ThreadIndex, BITMAP_JOINT_THUNDER, vPosition, o->Angle, vLight, 1);
							}

							Vector(1.0f, 1.0f, 1.0f, vLight);
							VectorCopy(o->Light, vPosition);

							CreateEffect(ThreadIndex, BITMAP_CRATER, vPosition, o->Angle, vLight, 2, nullptr, -1, 0, 0, 0, 1.5f);
							for (mu_int32 iu = 0; iu < 20; ++iu)
							{
								CreateEffect(ThreadIndex, MODEL_STONE2, vPosition, o->Angle, vLight);
							}

							EVector3	v3Pos;
							mu_int32		iLimitPos = 200;

							Vector(0.7f, 0.7f, 1.0f, vLight);

							v3Pos[0] = vPosition[0] + ((GetLargeRand(iLimitPos)) - (iLimitPos*0.5f));
							v3Pos[1] = vPosition[1] + ((GetLargeRand(iLimitPos)) - (iLimitPos*0.5f));
							v3Pos[2] = vPosition[2] + ((GetLargeRand(iLimitPos)) - (iLimitPos*0.5f));
							CreateParticle(ThreadIndex, BITMAP_CLUD64, v3Pos, o->Angle, vLight, 7, 2.0f);

							v3Pos[0] = vPosition[0] + ((GetLargeRand(iLimitPos)) - (iLimitPos*0.5f));
							v3Pos[1] = vPosition[1] + ((GetLargeRand(iLimitPos)) - (iLimitPos*0.5f));
							v3Pos[2] = vPosition[2] + ((GetLargeRand(iLimitPos)) - (iLimitPos*0.5f));
							CreateParticle(ThreadIndex, BITMAP_CLUD64, v3Pos, o->Angle, vLight, 7, 2.0f);

							v3Pos[0] = vPosition[0] + ((GetLargeRand(iLimitPos)) - (iLimitPos*0.5f));
							v3Pos[1] = vPosition[1] + ((GetLargeRand(iLimitPos)) - (iLimitPos*0.5f));
							v3Pos[2] = vPosition[2] + ((GetLargeRand(iLimitPos)) - (iLimitPos*0.5f));
							CreateParticle(ThreadIndex, BITMAP_CLUD64, v3Pos, o->Angle, vLight, 7, 2.0f);

							v3Pos[0] = vPosition[0] + ((GetLargeRand(iLimitPos)) - (iLimitPos*0.5f));
							v3Pos[1] = vPosition[1] + ((GetLargeRand(iLimitPos)) - (iLimitPos*0.5f));
							v3Pos[2] = vPosition[2] + ((GetLargeRand(iLimitPos)) - (iLimitPos*0.5f));
							CreateParticle(ThreadIndex, BITMAP_CLUD64, v3Pos, o->Angle, vLight, 7, 2.0f);

							v3Pos[0] = vPosition[0] + ((GetLargeRand(iLimitPos)) - (iLimitPos*0.5f));
							v3Pos[1] = vPosition[1] + ((GetLargeRand(iLimitPos)) - (iLimitPos*0.5f));
							v3Pos[2] = vPosition[2] + ((GetLargeRand(iLimitPos)) - (iLimitPos*0.5f));
							CreateParticle(ThreadIndex, BITMAP_CLUD64, v3Pos, o->Angle, vLight, 7, 2.0f);

							v3Pos[0] = vPosition[0] + ((GetLargeRand(iLimitPos)) - (iLimitPos*0.5f));
							v3Pos[1] = vPosition[1] + ((GetLargeRand(iLimitPos)) - (iLimitPos*0.5f));
							v3Pos[2] = vPosition[2] + ((GetLargeRand(iLimitPos)) - (iLimitPos*0.5f));
							CreateParticle(ThreadIndex, BITMAP_CLUD64, v3Pos, o->Angle, vLight, 7, 2.0f);

							Vector(0.3f, 0.2f, 1.0f, vLight);
							CreateEffect(ThreadIndex, BITMAP_SHOCK_WAVE, vPosition, o->Angle, vLight, 11);
							CreateEffect(ThreadIndex, BITMAP_SHOCK_WAVE, vPosition, o->Angle, vLight, 11);

							vPosition[2] += 100.0f;
							Vector(0.0f, 0.2f, 1.0f, vLight);
							CreateEffect(ThreadIndex, MODEL_EFFECT_THUNDER_NAPIN_ATTACK_1, vPosition, o->Angle, vLight, 1, nullptr, -1, 0, 0, 0, 1.0f);
						}
					}
					break;
				}

				if (o->LifeTime == 15)
				{
					CreateEffect(ThreadIndex, o->Type, o->Position, o->Angle, o->Light, 12, o->Owner, -1, 0, 0, 0, o->Scale);
				}

				mu_int32	iSwordOnTheLand = (o->ExtState / 2) + 5;
				if (o->LifeTime <= iSwordOnTheLand && o->LifeTime > 10)
				{
					EarthQuake.Store((mu_float)(GetLargeRand(2) - 2)*0.5f);
				}

				if (o->LifeTime == o->ExtState)
				{
					PlayBackground(SOUND_ASSASSIN);	
				}

				if (o->LifeTime == o->ExtState / 2 + 2)
				{
					PlayBackground(SOUND_SKILL_GIGANTIC_STORM);
				}

				if (o->LifeTime == (o->ExtState / 2) + 5)
				{
					PlayBackground(SOUND_FURY_STRIKE2);
				}
			}
			else if (o->SubType == 11)
			{
				mu_float	fCurrentRate = 1.0f - ((mu_float)o->LifeTime / (mu_float)o->ExtState);

				if (ox->m_Interpolates.m_vecInterpolatesScale.empty() == false)
				{
					ox->m_Interpolates.GetAlphaCurrent(o->Alpha, fCurrentRate);
				}

				if (fCurrentRate >= 0.0f && fCurrentRate <= 0.5f)
				{
					o->Flags.Set(OBJECT::eOF_VISIBLE);

					MUModel*	b = &MODELS::Data[o->Type];
					EVector3	*arrEachBonePos = ThreadInfo.BonePosition;
					EVector3	v3LightModify;

					EVector3	vRelativePos, vAngle;
					EVector3	v3CurrentHighHierarchyNodePos;

					Vector(4.0f, 0.0f, 0.0f, vRelativePos);
					VectorCopy(o->Angle, vAngle);

					MUModel *pBMDSwordModel = &MODELS::Data[o->Type];
					MUModel *pOwnerModel = &MODELS::Data[o->Owner->Type];

					mu_int32 arrBoneIdxs[] = { 4, 2, 8, 10, 6 };
					mu_int32 iBoneIdx = arrBoneIdxs[o->Type - MODEL_SWORDLEFT01_EMPIREGUARDIAN_BOSS_GAION_];

					pBMDSwordModel->AnimationTransformWithAttachHighModel(ThreadIndex, o->Owner, pOwnerModel, iBoneIdx,
						&v3CurrentHighHierarchyNodePos, arrEachBonePos);

					EVector3	v3LightTerrain;
					Vector(0.9f, 0.4f, 0.1f, v3LightTerrain);

					AddTerrainLight(o->Position[0], o->Position[1], v3LightTerrain, 2, PrimaryTerrainLight);

					switch (o->Type)
					{
					case MODEL_SWORDLEFT01_EMPIREGUARDIAN_BOSS_GAION_:
					case MODEL_SWORDRIGHT01_EMPIREGUARDIAN_BOSS_GAION_:
						{
							for (mu_int32 j = 0; j < 11; ++j)
							{
								Vector(1.0f, 1.0f, 1.0f, v3LightModify);
								if (GetLargeRand(2) == 0)
								{
									CreateParticle(ThreadIndex, BITMAP_POUNDING_BALL, arrEachBonePos[j], o->Angle, v3LightModify, 3, 1.3f);
								}
							}
						}
						break;
					case MODEL_SWORDLEFT02_EMPIREGUARDIAN_BOSS_GAION_:
					case MODEL_SWORDRIGHT02_EMPIREGUARDIAN_BOSS_GAION_:
						{
							Vector(0.3f, 0.4f, 1.0f, v3LightModify);

							for (mu_int32 j = 0; j < 11; ++j)
							{
								if (GetLargeRand(2) == 0)
								{
									CreateParticle(ThreadIndex, BITMAP_FIRE_HIK1_MONO, arrEachBonePos[j], o->Angle, v3LightModify, 10, o->Scale);
								}
							}
						}
						break;
					case MODEL_SWORDMAIN01_EMPIREGUARDIAN_BOSS_GAION_:
						{
							Vector(1.0f, 1.0f, 1.0f, v3LightModify);

							for (mu_int32 j = 0; j < 11; ++j)
							{
								if (GetLargeRand(20) == 0)
								{
									CreateEffect(ThreadIndex, BITMAP_FIRE_CURSEDLICH, arrEachBonePos[j], vAngle, v3LightModify, 12, o, -1, 0, 0, 0, o->Scale);
								}
							}
						}
						break;
					}
				}

				if (ox->m_Interpolates.m_vecInterpolatesAlpha.empty() == false)
				{
					ox->m_Interpolates.GetAlphaCurrent(o->Alpha, fCurrentRate);
				}
			}
			else if (o->SubType == 12)
			{
				mu_float	fCurrentRate = 1.0f - ((mu_float)o->LifeTime / (mu_float)o->ExtState);

				if (ox->m_Interpolates.m_vecInterpolatesScale.empty() == false)
				{
					ox->m_Interpolates.GetAlphaCurrent(o->Alpha, fCurrentRate);
				}

				if (fCurrentRate >= 0.0f && fCurrentRate <= 0.6f)
				{
					o->Flags.Set(OBJECT::eOF_VISIBLE);

					MUModel*	b = &MODELS::Data[o->Type];
					EVector3	*arrEachBonePos = ThreadInfo.BonePosition;
					EVector3	v3LightModify;

					EVector3	vRelativePos, vAngle;
					EVector3	v3CurrentHighHierarchyNodePos;

					Vector(4.0f, 0.0f, 0.0f, vRelativePos);
					VectorCopy(o->Angle, vAngle);

					MUModel *pBMDSwordModel = &MODELS::Data[o->Type];
					MUModel *pOwnerModel = &MODELS::Data[o->Owner->Type];

					mu_int32 arrBoneIdxs[] = { 4, 2, 8, 10, 6 };
					mu_int32 iBoneIdx = arrBoneIdxs[o->Type - MODEL_SWORDLEFT01_EMPIREGUARDIAN_BOSS_GAION_];

					pBMDSwordModel->AnimationTransformWithAttachHighModel(ThreadIndex, o->Owner, pOwnerModel, iBoneIdx,
						&v3CurrentHighHierarchyNodePos, arrEachBonePos);

					EVector3	v3LightTerrain;
					Vector(0.9f, 0.4f, 0.1f, v3LightTerrain);

					AddTerrainLight(o->Position[0], o->Position[1], v3LightTerrain, 2, PrimaryTerrainLight);
	
					Vector(0.3f, 0.4f, 1.0f, v3LightModify);
					for (mu_int32 j = 0; j < 11; ++j)
					{
						if (GetLargeRand(2) == 0)
						{
							CreateParticle(ThreadIndex, BITMAP_FIRE_HIK1_MONO, arrEachBonePos[j], o->Angle, v3LightModify, 10, o->Scale);
						}
					}
				}

				if (ox->m_Interpolates.m_vecInterpolatesAlpha.empty() == false)
				{
					ox->m_Interpolates.GetAlphaCurrent(o->Alpha, fCurrentRate);
				}
			}
			else if (o->SubType == 20)
			{
				o->Flags.Set(OBJECT::eOF_VISIBLE);

				mu_float	fCurrentRate = (mu_float)(o->LifeTime) / (mu_float)(o->ExtState);
				EVector3	v3RotateAngleRelative;

				if (ox->m_Interpolates.m_vecInterpolatesAngle.empty() == false)
				{
					ox->m_Interpolates.GetAngleCurrent(v3RotateAngleRelative, fCurrentRate);

					o->Angle[0] = v3RotateAngleRelative[0];
					o->Angle[1] = v3RotateAngleRelative[1];
					o->Angle[2] = v3RotateAngleRelative[2];
				}

				if (ox->m_Interpolates.m_vecInterpolatesPos.empty() == false)
				{
					ox->m_Interpolates.GetPosCurrent(o->Position, fCurrentRate);
				}

				if (ox->m_Interpolates.m_vecInterpolatesScale.empty() == false)
				{
					ox->m_Interpolates.GetScaleCurrent(o->Scale, fCurrentRate);
				}

				if (ox->m_Interpolates.m_vecInterpolatesAlpha.empty() == false)
				{
					ox->m_Interpolates.GetAlphaCurrent(o->Alpha, fCurrentRate);
				}
			}
		}
		break;
	case MODEL_EMPIREGUARDIAN_BLOW_OF_DESTRUCTION:
		{
			if (o->SubType == 0)
			{
				EVector3 vLight;
				if (o->LifeTime <= 24)
				{
					mu_float earthquake = 0.0f;
					if (o->LifeTime >= 15.0f)
					{
						earthquake = (mu_float)(GetLargeRand(8) - 4)*0.1f;
					}
					EarthQuake.Store(earthquake);

					if (o->LifeTime == 23)
					{
						Vector(0.3f, 0.3f, 1.0f, vLight);

						CreateEffect(ThreadIndex, MODEL_NIGHTWATER_01, o->Position, o->Angle, vLight, 0, o, o->PKKey, o->Kind, o->Skill, o->m_bySkillSerialNum, o->Scale * 1.0f);
						CreateEffect(ThreadIndex, MODEL_NIGHTWATER_01, o->Position, o->Angle, vLight, 0, o, o->PKKey, o->Kind, o->Skill, o->m_bySkillSerialNum, o->Scale * 1.0f);

						CreateEffect(ThreadIndex, MODEL_KNIGHT_PLANCRACK_A, o->Position, o->Angle, vLight, 0, o, 0, 0, 0, 0, o->Scale*0.2f);

						EVector3 vDir, vPos, vAngle;
						VectorSubtract(o->StartPosition, o->Position, vDir);
						mu_float fLength = VectorLength(vDir);
						VectorNormalize(vDir);
						mu_int32 iNum = (mu_int32)(fLength / 100) + 1;
						for (mu_int32 i = 0; i<iNum; ++i)
						{
							VectorScale(vDir, 55.0f * i, vPos);
							VectorAdd(o->Position, vPos, vPos);
							VectorCopy(o->Owner->Angle, vAngle);
							if (i % 2 == 0)
							{
								vAngle[2] += (GetLargeRand(20) + 10);
							}
							else
							{
								vAngle[2] -= (GetLargeRand(20) + 10);
							}
							CreateEffect(ThreadIndex, MODEL_KNIGHT_PLANCRACK_B,
								vPos, vAngle, vLight, 0, o, 0, 0, 0, 0, o->Scale);
						}
					}

					o->Light[0] /= 1.05f;
					o->Light[1] /= 1.05f;
					o->Light[2] /= 1.05f;
				}
			}
			else if (o->SubType == 1)
			{
				if (o->LifeTime <= 24)
				{
					if (o->LifeTime >= 15)
					{
						EVector3 vPos, vLight;
						for (mu_int32 i = 0; i<15; ++i)
						{
							VectorCopy(o->Position, vPos);
							vPos[0] += GetLargeRand(400) - 200;
							vPos[1] += GetLargeRand(400) - 200;
							vPos[2] += GetLargeRand(400) - 200;
							mu_float fScale = (o->Scale * 1.6f) + GetLargeRand(10) * 0.1f;
							Vector(0.3f, 0.3f, 1.0f, vLight);
							mu_int32 index = (GetLargeRand(2) == 0) ? BITMAP_WATERFALL_5 : BITMAP_WATERFALL_3;
							CreateParticle(ThreadIndex, index, vPos, o->Angle, vLight, 8, fScale);
							Vector(1.0f, 1.0f, 1.0f, vLight);
							if (GetLargeRand(2) == 0)
							{
								CreateParticle(ThreadIndex, BITMAP_SMOKE, vPos, o->Angle, vLight, 55, o->Scale * 0.9f);
							}
						}
					}

					if (o->LifeTime == 23)
					{
						EVector3 vLight;
						Vector(0.3f, 0.3f, 1.0f, vLight);

						CreateEffect(ThreadIndex, MODEL_NIGHTWATER_01, o->Position, o->Angle, vLight, 0, o, -1, 0, 0, 0, o->Scale * 1.1f);
						CreateEffect(ThreadIndex, MODEL_NIGHTWATER_01, o->Position, o->Angle, vLight, 0, o, -1, 0, 0, 0, o->Scale * 1.2f);

						CreateEffect(ThreadIndex, MODEL_RAKLION_BOSS_CRACKEFFECT, o->Position, o->Angle, vLight,
							0, o, -1, 0, 0, 0, o->Scale * 0.4f);

						EVector3 vPos, vResult;
						EVector3 vAngle;
						EBone Matrix;
						for (mu_int32 i = 0; i<(5 + GetLargeRand(3)); ++i)
						{
							Vector(0.0f, (mu_float)(GetLargeRand(150)), 0.0f, vPos);
							Vector(0.0f, 0.0f, (mu_float)(GetLargeRand(360)), vAngle);
							AngleMatrix(vAngle, Matrix);
							VectorRotate(vPos, Matrix, vResult);
							VectorAdd(vResult, o->Position, vResult);

							CreateEffect(ThreadIndex, MODEL_STONE1 + GetLargeRand(2), vResult, o->Angle, o->Light, 13);
						}
					}

					o->Light[0] /= 1.05f;
					o->Light[1] /= 1.05f;
					o->Light[2] /= 1.05f;
				}
			}
		}
		break;

	case BITMAP_RING_OF_GRADATION:
		{
			if (o->SubType == 0)
			{
				if (o->LifeTime == 20)
					break;

				o->Scale += 0.1f;

				o->Light[0] /= 1.1f;
				o->Light[1] /= 1.1f;
				o->Light[2] /= 1.1f;

				o->Alpha /= 1.1f;
			}
		}break;
	case MODEL_EFFECT_UMBRELLA_DIE:
		{
			if (o->SubType == 0)
			{
				if (o->LifeTime == 28 || o->LifeTime == 18 || o->LifeTime == 8)
				{
					EVector3 vLight;
					Vector(1.0f, 0.2f, 0.5f, vLight);
					CreateEffect(ThreadIndex, BITMAP_RING_OF_GRADATION, o->Owner->Position, o->Angle, vLight, 0, o->Owner, 0, 0, 0, 0, 1.2f);
				}
			}
		}break;
	case MODEL_EFFECT_UMBRELLA_GOLD:
		{
			o->Angle[0] += 10.0f;
			o->Position[0] += (o->Direction[0] * 2.2f);
			o->Position[1] += (o->Direction[1] * 2.2f);
			o->Position[2] += (o->Gravity * 1.5f);
			o->Gravity -= 1.5f;

			Height = RequestTerrainHeight(o->Position[0], o->Position[1]);
			if (o->Position[2] < Height)
			{
				o->Position[2] = Height;
				o->Gravity = -o->Gravity*0.3f;
				o->LifeTime -= 2;
			}
		}break;
	case MODEL_EFFECT_EG_GUARDIANDEFENDER_ATTACK2:
		{
			if (o->LifeTime <= 18)
			{
				o->Light[0] /= 1.4f;
				o->Light[1] /= 1.4f;
				o->Light[2] /= 1.4f;
				o->Scale *= 1.1f;
			}
			CreateSprite(BITMAP_SHOCK_WAVE, o->Position, o->Scale, o->Light, o->Owner);
			CreateParticle(ThreadIndex, BITMAP_FIRE + 2, o->Position, o->Angle, o->Light, 16, 2.5f);

		}break;
	case MODEL_EFFECT_SD_AURA:
		{
			EVector3 Temp_Pos;
			MUModel* b = &MODELS::Data[o->Owner->Type];
			b->TransformByObjectBone(ThreadIndex, Temp_Pos, o->Owner, 18);
			Temp_Pos[2] -= 30.0f;
			VectorCopy(Temp_Pos, o->Position);
			o->LifeTime = 100;
		}
		break;
	case BITMAP_WATERFALL_4:
		{
			EVector3 Temp_Pos;
			MUModel* b = &MODELS::Data[o->Owner->Type];
			b->TransformByObjectBone(ThreadIndex, Temp_Pos, o->Owner, o->PKKey);

			o->Timer += 0.1f;
			o->Distance += 2.1f;
			o->Angle[0] += 0.7f;

			o->Light[0] *= 0.95f;
			o->Light[1] *= 0.95f;
			o->Light[2] *= 0.95f;

			EVector3 vPos;
			for (mu_int32 i = 0; i<3; ++i)
			{
				switch (i)
				{
				case 0:
					vPos[0] = EMath::Cos(o->Timer) * o->Distance + Temp_Pos[0];
					vPos[1] = Temp_Pos[1];
					vPos[2] = EMath::Sin(o->Timer) * o->Distance + Temp_Pos[2];
					break;
				case 1:
					vPos[0] = EMath::Sin(o->Timer) * o->Distance + Temp_Pos[0];
					vPos[1] = EMath::Cos(o->Timer) * o->Distance + Temp_Pos[1];
					vPos[2] = Temp_Pos[2];
					break;
				case 2:
					vPos[0] = Temp_Pos[0];
					vPos[1] = EMath::Sin(o->Timer) * o->Distance + Temp_Pos[1];
					vPos[2] = EMath::Cos(o->Timer) * o->Distance + Temp_Pos[2];
					break;
				}

				CreateSprite(BITMAP_WATERFALL_4, vPos, o->Scale, o->Light, o, o->Angle[0]);
			}
		}
		break;
	}

	if (o->Flags.Get(OBJECT::eOF_LIVE) == false)
	{
		g_EffectsDeleteCount.fetch_add(1);
	}
	else if (o->Type >= MODEL_BIG_METEO1 && o->Type <= MODEL_BIG_METEO3)
	{
	}
	else
	{
		if (
			o->Type == MODEL_SKILL_WHEEL1
			|| o->Type == MODEL_SKILL_WHEEL2
			|| o->Type == MODEL_SKILL_FURY_STRIKE
			|| ((o->Type == MODEL_STONE1 || o->Type == MODEL_STONE2) && o->SubType == 5)
			|| (o->Type == MODEL_ARROW_DRILL && o->SubType == 3)
			|| o->Type == MODEL_PIER_PART
			|| o->Type == MODEL_DEATH_SPI_SKILL
			|| o->Type == MODEL_CHANGE_UP_EFF
			)
		{
		}
		else
		{
			if (o->Type >= MODEL_BIRD01 && o->Type<MODEL_SKILL_END)
			{
				MUModel *b = &MODELS::Data[o->Type];
				b->CurrentAction(ThreadIndex) = o->CurrentAction;
				if (o->CurrentAction != 255 && b->PlayAnimation(ThreadIndex, &o->AnimationFrame, &o->PriorAnimationFrame, &o->PriorAction, o->Velocity) == false)
				{
					o->AnimationFrame = b->_AnimationHeader[o->CurrentAction].FrameCount;
					o->CurrentAction = 255;
				}
			}

			if (o->Type < BITMAP_BOSS_LASER || o->Type > BITMAP_BOSS_LASER + 2)
			{
				switch (o->Type)
				{
				case MODEL_SAW:
				case MODEL_SPEARSKILL:
					MoveParticle(o, false);
					break;
				case MODEL_ARROW_HOLY:
					{
						EBone Matrix;
						EVector3 Angle;
						EVector3 Position;

						VectorCopy(o->Angle, Angle);
						AngleMatrix(Angle, Matrix);
						VectorRotate(o->Direction, Matrix, Position);
						VectorAdd(o->StartPosition, Position, o->StartPosition);
					}
					break;
				case MODEL_FLY_BIG_STONE1:
				case MODEL_FLY_BIG_STONE2:
					break;

				case MODEL_SKILL_JAVELIN:
					break;
				case MODEL_SWORD_FORCE:
					{
						MUModel *b = &MODELS::Data[o->Type];
						b->CurrentAction(ThreadIndex) = o->CurrentAction;
						b->PlayAnimation(ThreadIndex, &o->AnimationFrame, &o->PriorAnimationFrame, &o->PriorAction, o->Velocity);
						MoveParticle(o, true);
					}
					break;
				default:
					MoveParticle(o, true);
					break;
				}
			}
		}
		--o->LifeTime;
		if (o->LifeTime <= 0 || o->Flags.Get(OBJECT::eOF_LIVE) == false)
		{
			g_EffectsDeleteCount.fetch_add(1);

			o->Flags.Clear(OBJECT::eOF_LIVE);
		}
		else
		{
			switch (o->Type)
			{
			case BITMAP_LIGHT:
				{
					if (o->SubType == 0 && 0 != (o->LifeTime % 2))
						MoveEffect(ThreadIndex, o, iIndex);
				}
				break;
			case MODEL_FIRE:
				if (o->SubType == 3)
				{
					if (0 != (o->LifeTime % 2))
						MoveEffect(ThreadIndex, o, iIndex);
				}
				break;

			case MODEL_SUMMONER_SUMMON_LAGUL:
				if (o->SubType == 1)
				{
					MUModel* pModel = &MODELS::Data[o->Type];
					EVector3 vPos, vLight;

					const mu_int32 nBoneCount = 6;
					mu_int32 nBone[nBoneCount] = { 54, 55, 56, 57, 58, 59 };
					Vector(0.2f, 0.3f, 1.0f, vLight);

					pModel->Animation(ThreadIndex, ThreadInfo.BoneTransform, o->AnimationFrame, o->PriorAnimationFrame,
						o->PriorAction, o->Angle, o->HeadAngle, false, false);
					for (mu_int32 i = 0; i < nBoneCount; ++i)
					{
						pModel->TransformByObjectBone(ThreadIndex, vPos, o, nBone[i]);
						CreateSprite(BITMAP_SHINY + 6, vPos, o->Scale * 0.3f, vLight, nullptr);
					}

					if (GetLargeRand(5) == 0)
					{
						mu_int32 nBones = pModel->_BoneNonDummyEnd;
						pModel->TransformByObjectBoneDirect(ThreadIndex, vPos, o, GetLargeRand(nBones));
						Vector(0.6f, 0.5f, 0.9f, vLight);
						CreateParticle(ThreadIndex, BITMAP_SMOKE, vPos, o->Angle, vLight, 57, 2.0f);
						Vector(0.7f, 0.7f, 1.0f, vLight);
						CreateParticle(ThreadIndex, BITMAP_TWINTAIL_WATER, vPos, o->Angle, vLight, 1);
					}
				}
				break;

			case BITMAP_BOSS_LASER:
			case BITMAP_BOSS_LASER + 1:
			case BITMAP_BOSS_LASER + 2:
				{
					EVector3 Position;
					VectorCopy(o->Position, Position);
					for (mu_int32 j = 0; j < 20; ++j)
					{
						if (o->SubType == 1)
						{
							o->Angle[2] -= 0.1f;
							Vector(1.0f, 1.0f, 1.0f, o->Light);
							CreateSprite(BITMAP_FIRE + 1, Position, o->Scale, o->Light, o, o->Angle[2]);
							Vector(0.2f, 0.0f, 0.0f, o->Light);
							RenderTerrainAlphaBitmap(BITMAP_SMOKE, BLEND::TXB_BLEND01, Position[0], Position[1], 4.0f, 4.0f, o->Light, -o->Angle[2], 1.0f, 5.0f);
						}
						else if (o->SubType == 2)
						{
							Vector(0.0f, 0.2f, 1.0f, o->Light);
							o->Angle[2] -= 0.1f;
							CreateSprite(BITMAP_FIRE + 1, Position, o->Scale, o->Light, o, o->Angle[2]);
						}
						else
							CreateSprite(BITMAP_SPARK + 1, Position, o->Scale, o->Light, o);

						VectorAdd(Position, o->Direction, Position);

						if (battleCastle::IsBattleCastleStart())
						{
							mu_uint16 att = TERRAIN_ATTRIBUTE(Position[0], Position[1]);
							if ((att&TW_NOATTACKZONE) == TW_NOATTACKZONE)
							{
								break;
							}
						}

						AddTerrainLight(Position[0], Position[1], o->Light, 2, PrimaryTerrainLight);
					}
				}
				break;

			case BITMAP_MAGIC + 1:
				{
					if (o->SubType == 13)
					{
						o->Light[0] /= 1.05f;
						o->Light[1] /= 1.05f;
						o->Light[2] /= 1.05f;
					}

					if (o->SubType == 7)
					{
						VectorCopy(o->Owner->Position, o->Position);
						o->Angle[2] += 10.0f;
					}
					else if (o->SubType == 6 && o->LifeTime % 25 == 0)
					{
						CreateEffect(ThreadIndex, BITMAP_MAGIC + 1, o->Position, o->Angle, o->Light, 7, o->Owner);
					}
					else if (o->SubType == 8 && o->LifeTime % 2 == 0)
					{
						CreateEffect(ThreadIndex, BITMAP_MAGIC + 1, o->Position, o->Angle, o->Light, 9, o->Owner);
					}
				}
				break;
			}
		}
	}
}

void MUTasking::CheckMoveEffects(const mu_uint32 ThreadIndex)
{
	mu_uint32 queueCount = 0;
	const mu_uint32 queueIndex = MUThreadQueues::EffectsQueueIndex[ThreadIndex];

	mu_uint32 beginIndex, endIndex;
	GetIndexTasking(ThreadIndex, MAX_EFFECTS, beginIndex, endIndex);

	for (mu_uint32 i = beginIndex; i < endIndex; ++i)
	{
		const OBJECT *o = &g_Effects[i];
		if (o->Flags.Get(OBJECT::eOF_LIVE) == true)
		{
			MUThreadQueues::EffectsQueue[queueIndex + queueCount++] = i;
		}
	}

	MUThreadQueues::EffectsQueueCount[ThreadIndex] = queueCount;
}

void MUTasking::MoveEffects(const mu_uint32 ThreadIndex)
{
	mu_uint32 q = ThreadIndex;
	for (mu_uint32 t = 0; t < _ThreadsCount; ++t)
	{
		const mu_uint32 queueCount = MUThreadQueues::EffectsQueueCount[t];
		const mu_uint16 *queue = MUThreadQueues::EffectsQueue.data() + MUThreadQueues::EffectsQueueIndex[t];

		for (; q < queueCount; q += _ThreadsCount)
		{
			const mu_uint32 i = queue[q];
			OBJECT *o = &g_Effects[i];
			MoveEffect(ThreadIndex, o, i);
		}

		q -= queueCount;
	}
}

void MUTasking::CheckEffects(const mu_uint32 ThreadIndex)
{
	mu_uint32 queueCount = 0;
	const mu_uint32 queueIndex = MUThreadQueues::EffectsQueueIndex[ThreadIndex];

	mu_uint32 beginIndex, endIndex;
	GetIndexTasking(ThreadIndex, MAX_EFFECTS, beginIndex, endIndex);

	for (mu_uint32 i = beginIndex; i < endIndex; ++i)
	{
		const OBJECT *o = &g_Effects[i];
		if (o->Flags.Get(OBJECT::eOF_LIVE) == true)
		{
			MUThreadQueues::EffectsQueue[queueIndex + queueCount++] = i;
		}
	}

	MUThreadQueues::EffectsQueueCount[ThreadIndex] = queueCount;
}

void MUTasking::PrepareEffects(const mu_uint32 ThreadIndex)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	EVector3 Light;
	mu_float Scale;
	mu_float Luminosity;

	mu_uint32 q = ThreadIndex;
	for (mu_uint32 t = 0; t < _ThreadsCount; ++t)
	{
		const mu_uint32 queueCount = MUThreadQueues::EffectsQueueCount[t];
		const mu_uint16 *queue = MUThreadQueues::EffectsQueue.data() + MUThreadQueues::EffectsQueueIndex[t];

		for (; q < queueCount; q += _ThreadsCount)
		{
			const mu_uint32 realIndex = queue[q];
			OBJECT *o = &g_Effects[realIndex];
			
			RENDER_STATUS renderStatus = RENDER_SUCCEED;

			ThreadInfo.RenderIndex = 0;
			ThreadInfo.CulledType = CULLED_EFFECTS;
			ThreadInfo.ObjectIndex = realIndex;

			switch (o->Type)
			{
			case MODEL_DRAGON:
			case MODEL_WARP3:
			case MODEL_WARP2:
			case MODEL_WARP:
			case MODEL_WARP6:
			case MODEL_WARP5:
			case MODEL_WARP4:
			case MODEL_DESAIR:
			case MODEL_GHOST:
			case MODEL_TREE_ATTACK:
				renderStatus = RenderObject(ThreadIndex, o);
				break;
			case MODEL_MULTI_SHOT1:
			case MODEL_MULTI_SHOT2:
			case MODEL_MULTI_SHOT3:
				{
					MUModel *b = &MODELS::Data[o->Type];
					Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
					b->PlayAnimation(ThreadIndex, &o->AnimationFrame, &o->PriorAnimationFrame, &o->PriorAction, o->Velocity / 6.0f);
					renderStatus = RenderObject(ThreadIndex, o);
				}
				break;
			case MODEL_STORM:
				if (o->SubType == 3 || o->SubType == 4 || o->SubType == 5 || o->SubType == 6 || o->SubType == 7)
				{
					if (MU_UPDATECOUNTER > 0)
					{
						EVector3 Light;
						Vector(0.3f, 0.3f, 0.3f, Light);
						mu_float Rotation = (mu_float)(FWorldTime);
						RenderTerrainAlphaBitmap(BITMAP_POUNDING_BALL, BLEND::TXB_BLEND01, o->Position[0], o->Position[1], 2.0f, 2.0f, Light, Rotation, 1.0f, 5.0f);
					}
				}
				else
					renderStatus = RenderObject(ThreadIndex, o);
				break;
			case MODEL_SUMMON:
			case MODEL_STORM2:
			case MODEL_STORM3:
			case MODEL_MAYASTAR:
			case MODEL_MAYAHANDSKILL:
			case MODEL_MAYASTONE1:
			case MODEL_MAYASTONE2:
			case MODEL_MAYASTONE3:
			case MODEL_MAYASTONE4:
			case MODEL_MAYASTONE5:
			case MODEL_MAYASTONEFIRE:
			case MODEL_BUTTERFLY01:
				renderStatus = RenderObject(ThreadIndex, o);
				break;
			case 9:
				renderStatus = RenderObject(ThreadIndex, o);
				break;
			case MODEL_HALLOWEEN_CANDY_BLUE:
			case MODEL_HALLOWEEN_CANDY_ORANGE:
			case MODEL_HALLOWEEN_CANDY_YELLOW:
			case MODEL_HALLOWEEN_CANDY_RED:
			case MODEL_HALLOWEEN_CANDY_HOBAK:
			case MODEL_HALLOWEEN_CANDY_STAR:
				renderStatus = RenderObject(ThreadIndex, o);
				break;
			case MODEL_HALLOWEEN:
			case MODEL_HALLOWEEN_EX:
				break;

			case MODEL_XMAS_EVENT_BOX:
			case MODEL_XMAS_EVENT_CANDY:
			case MODEL_XMAS_EVENT_TREE:
			case MODEL_XMAS_EVENT_SOCKS:
				renderStatus = RenderObject(ThreadIndex, o);
				break;
			case MODEL_XMAS_EVENT_ICEHEART:
				renderStatus = RenderObject(ThreadIndex, o);
				break;

			case MODEL_NEWYEARSDAY_EVENT_BEKSULKI:
			case MODEL_NEWYEARSDAY_EVENT_CANDY:
			case MODEL_NEWYEARSDAY_EVENT_MONEY:
			case MODEL_NEWYEARSDAY_EVENT_HOTPEPPER_GREEN:
			case MODEL_NEWYEARSDAY_EVENT_HOTPEPPER_RED:
			case MODEL_NEWYEARSDAY_EVENT_PIG:
			case MODEL_NEWYEARSDAY_EVENT_YUT:
				renderStatus = RenderObject(ThreadIndex, o);
				break;

			case MODEL_MOONHARVEST_MOON:
				{
					if (o->SubType == 0)
					{
						if (MU_UPDATECOUNTER > 0)
						{
							EVector3 vLight;

							Vector(0.6f, 0.8f, 0.6f, vLight);
							++o->m_iAnimation;
							CreateSprite(BITMAP_SHOCK_WAVE, o->Position, 0.8f, vLight, o, -(o->m_iAnimation*3.0f));

							Vector(0.8f, 0.6f, 0.0f, vLight);
							CreateSprite(BITMAP_LIGHT, o->Position, 5.0f, vLight, o, 0.0f);
						}
						renderStatus = RenderObject(ThreadIndex, o);
					}
					else if (o->SubType == 1)
					{
						if (MU_UPDATECOUNTER > 0)
						{
							CreateSprite(BITMAP_LIGHT, o->Position, 5.0f, o->Light, o, 0.0f);
							CreateSprite(BITMAP_LIGHT, o->Position, 3.0f, o->Light, o, 0.0f);
							CreateSprite(BITMAP_LIGHT, o->Position, 3.0f, o->Light, o, 0.0f);
							CreateSprite(BITMAP_SHINY + 6, o->Position, 2.0f, o->Light, o, 0.0f);
						}

						renderStatus = RenderObject(ThreadIndex, o);
					}
					else if (o->SubType == 2)
					{
						renderStatus = RenderObject(ThreadIndex, o);
					}
				}
				break;

			case MODEL_MOONHARVEST_GAM:
			case MODEL_MOONHARVEST_SONGPUEN1:
			case MODEL_MOONHARVEST_SONGPUEN2:
				renderStatus = RenderObject(ThreadIndex, o);
				break;

			case MODEL_CHANGE_UP_EFF:
				renderStatus = RenderObject(ThreadIndex, o);
				break;
			case MODEL_CURSEDTEMPLE_HOLYITEM:
			case MODEL_CURSEDTEMPLE_PRODECTION_SKILL:
				renderStatus = RenderObject(ThreadIndex, o);
				break;

			case MODEL_CURSEDTEMPLE_RESTRAINT_SKILL:
				renderStatus = RenderObject(ThreadIndex, o);
				break;

			case MODEL_SPEARSKILL:
				RenderSkillSpear(ThreadIndex, o);
				break;
			case BITMAP_FIRE_CURSEDLICH:
				break;

			case MODEL_SUMMONER_WRISTRING_EFFECT:
				renderStatus = RenderObject(ThreadIndex, o);
				break;

			case MODEL_SUMMONER_EQUIP_HEAD_SAHAMUTT:
				if (o->SubType == 0)
				{
					renderStatus = RenderObject(ThreadIndex, o);
				}
				break;
			case MODEL_SUMMONER_EQUIP_HEAD_NEIL:
				if (o->SubType == 0)
				{
					renderStatus = RenderObject(ThreadIndex, o);
				}
				break;
			case MODEL_SUMMONER_CASTING_EFFECT1:
			case MODEL_SUMMONER_CASTING_EFFECT11:
			case MODEL_SUMMONER_CASTING_EFFECT111:
			case MODEL_SUMMONER_CASTING_EFFECT2:
			case MODEL_SUMMONER_CASTING_EFFECT22:
			case MODEL_SUMMONER_CASTING_EFFECT222:
			case MODEL_SUMMONER_CASTING_EFFECT4:
				renderStatus = RenderObject(ThreadIndex, o);
				break;
			case MODEL_SUMMONER_SUMMON_SAHAMUTT:
				renderStatus = RenderObject(ThreadIndex, o);
				break;
			case MODEL_SUMMONER_SUMMON_NEIL:
				renderStatus = RenderObject(ThreadIndex, o);
				break;
			case MODEL_SUMMONER_SUMMON_NEIL_NIFE1:
			case MODEL_SUMMONER_SUMMON_NEIL_NIFE2:
			case MODEL_SUMMONER_SUMMON_NEIL_NIFE3:
				renderStatus = RenderObject(ThreadIndex, o);
				break;
			case MODEL_SUMMONER_SUMMON_NEIL_GROUND1:
			case MODEL_SUMMONER_SUMMON_NEIL_GROUND2:
			case MODEL_SUMMONER_SUMMON_NEIL_GROUND3:
				renderStatus = RenderObject(ThreadIndex, o);
				break;
			case MODEL_SUMMONER_SUMMON_LAGUL:
				if (o->SubType == 1)
				{
					renderStatus = RenderObject(ThreadIndex, o);
				}
				break;
			case MODEL_EFFECT_BROKEN_ICE0:
			case MODEL_EFFECT_BROKEN_ICE1:
			case MODEL_EFFECT_BROKEN_ICE2:
			case MODEL_EFFECT_BROKEN_ICE3:
				renderStatus = RenderObject(ThreadIndex, o);

				if (MU_UPDATECOUNTER > 0)
				{
					if (o->SubType == 1)
					{
						Luminosity = (mu_float)GetLargeRand(4, 8) * 0.1f;
						Vector(Luminosity*o->Light[0], Luminosity*o->Light[0], Luminosity*o->Light[0], Light);
						RenderTerrainAlphaBitmap(BITMAP_LIGHT, BLEND::TXB_BLEND01, o->Position[0], o->Position[1], 5.0f, 5.0f, Light, -o->Angle[2]);
					}
				}
				break;
			case MODEL_SKILL_WHEEL1:
				break;
			case MODEL_SKILL_WHEEL2:
				RenderWheelWeapon(ThreadIndex, o);
				break;
			case MODEL_SKILL_FURY_STRIKE:
				RenderFuryStrike(ThreadIndex, o);
				break;
			case MODEL_ARROW_HOLY:
				break;
			case BITMAP_BOSS_LASER:
			case BITMAP_BOSS_LASER + 1:
			case BITMAP_BOSS_LASER + 2:
				{
				}
				break;
			case MODEL_GATE:
			case MODEL_GATE + 1:
			case MODEL_STONE_COFFIN:
			case MODEL_STONE_COFFIN + 1:
			case MODEL_STAFF + 8:
			case MODEL_CLOUD:
				renderStatus = RenderObject(ThreadIndex, o);
				break;
			case MODEL_SHINE:
				break;
			case MODEL_WAVE_FORCE:
				renderStatus = RenderObject(ThreadIndex, o);
				break;
			case MODEL_MAGIC_CAPSULE2:
				renderStatus = RenderObject(ThreadIndex, o);
				break;
			case MODEL_AIR_FORCE:
			case MODEL_PIER_PART:
				renderStatus = RenderObject(ThreadIndex, o);
				break;
			case MODEL_PIERCING2:
				if (o->SubType == 1 || o->SubType == 2)
					break;
				renderStatus = RenderObject(ThreadIndex, o);
				break;
			case MODEL_PIERCING:
				if (o->SubType == 3)
					break;
				renderStatus = RenderObject(ThreadIndex, o);
				break;
			case MODEL_TOWER_GATE_PLANE:
				renderStatus = RenderObject(ThreadIndex, o);
				o->Position[2] = o->StartPosition[2] + 400.0f - (o->Position[2] - o->StartPosition[2]);
				renderStatus = RenderObject(ThreadIndex, o);
				break;

			case BATTLE_CASTLE_WALL1:
			case BATTLE_CASTLE_WALL2:
			case BATTLE_CASTLE_WALL3:
			case BATTLE_CASTLE_WALL4:
				renderStatus = RenderObject(ThreadIndex, o);
				break;
			case MODEL_FENRIR_THUNDER:
				renderStatus = RenderObject(ThreadIndex, o);
				break;
			case MODEL_FALL_STONE_EFFECT:
				renderStatus = RenderObject(ThreadIndex, o);
				break;
			case MODEL_FENRIR_FOOT_THUNDER:
				{
					if (MU_UPDATECOUNTER > 0)
					{
						RenderTerrainAlphaBitmap(BITMAP_FENRIR_FOOT_THUNDER1 + (o->m_iAnimation % 5), BLEND::TXB_BLEND01, o->Position[0], o->Position[1], 0.6f, 0.6f, o->Light, 0.0f, 1.0f, 5.0f);
					}
				}
				break;
			case MODEL_TWINTAIL_EFFECT:
				{
					if (MU_UPDATECOUNTER > 0)
					{
						if (o->SubType == 0)
						{
							RenderTerrainAlphaBitmap(BITMAP_SPARK + 1, BLEND::TXB_BLEND01, o->Position[0], o->Position[1], o->Scale, o->Scale, o->Light, 0.0f, o->Alpha, 5.0f);
						}
						else if (o->SubType == 1 || o->SubType == 2)
						{
							RenderTerrainAlphaBitmap(BITMAP_CLOUD, BLEND::TXB_BLEND01, o->Position[0], o->Position[1], o->Scale, o->Scale, o->Light, o->Angle[0], o->Alpha, 5.0f);
						}
					}
				}
				break;
			case MODEL_CUNDUN_PART1:
			case MODEL_CUNDUN_PART2:
			case MODEL_CUNDUN_PART3:
			case MODEL_CUNDUN_PART4:
			case MODEL_CUNDUN_PART5:
			case MODEL_CUNDUN_PART6:
			case MODEL_CUNDUN_PART7:
			case MODEL_CUNDUN_PART8:
			case MODEL_MONSTER01 + 64:
			case MODEL_CUNDUN_DRAGON_HEAD:
			case MODEL_CUNDUN_PHOENIX:
			case MODEL_CUNDUN_GHOST:
				renderStatus = RenderObject(ThreadIndex, o);
				break;
			case MODEL_CURSEDTEMPLE_STATUE_PART1:
			case MODEL_CURSEDTEMPLE_STATUE_PART2:
				renderStatus = RenderObject(ThreadIndex, o);
				break;
			case MODEL_XMAS2008_SNOWMAN_HEAD:
			case MODEL_XMAS2008_SNOWMAN_BODY:
				renderStatus = RenderObject(ThreadIndex, o);
				break;
			case MODEL_PANDA:
				renderStatus = RenderObject(ThreadIndex, o);
				break;
			case MODEL_TOTEMGOLEM_PART1:
			case MODEL_TOTEMGOLEM_PART2:
			case MODEL_TOTEMGOLEM_PART3:
			case MODEL_TOTEMGOLEM_PART4:
			case MODEL_TOTEMGOLEM_PART5:
			case MODEL_TOTEMGOLEM_PART6:
				renderStatus = RenderObject(ThreadIndex, o);
				break;
			case MODEL_SHADOW_PAWN_ANKLE_LEFT:		case MODEL_SHADOW_PAWN_ANKLE_RIGHT:
			case MODEL_SHADOW_PAWN_BELT:			case MODEL_SHADOW_PAWN_CHEST:
			case MODEL_SHADOW_PAWN_HELMET:
			case MODEL_SHADOW_PAWN_KNEE_LEFT:		case MODEL_SHADOW_PAWN_KNEE_RIGHT:
			case MODEL_SHADOW_PAWN_WRIST_LEFT:		case MODEL_SHADOW_PAWN_WRIST_RIGHT:

			case MODEL_SHADOW_KNIGHT_ANKLE_LEFT:	case MODEL_SHADOW_KNIGHT_ANKLE_RIGHT:
			case MODEL_SHADOW_KNIGHT_BELT:			case MODEL_SHADOW_KNIGHT_CHEST:
			case MODEL_SHADOW_KNIGHT_HELMET:
			case MODEL_SHADOW_KNIGHT_KNEE_LEFT:		case MODEL_SHADOW_KNIGHT_KNEE_RIGHT:
			case MODEL_SHADOW_KNIGHT_WRIST_LEFT:	case MODEL_SHADOW_KNIGHT_WRIST_RIGHT:

			case MODEL_SHADOW_ROOK_ANKLE_LEFT:		case MODEL_SHADOW_ROOK_ANKLE_RIGHT:
			case MODEL_SHADOW_ROOK_BELT:			case MODEL_SHADOW_ROOK_CHEST:
			case MODEL_SHADOW_ROOK_HELMET:
			case MODEL_SHADOW_ROOK_KNEE_LEFT:		case MODEL_SHADOW_ROOK_KNEE_RIGHT:
			case MODEL_SHADOW_ROOK_WRIST_LEFT:		case MODEL_SHADOW_ROOK_WRIST_RIGHT:

			case MODEL_ICE_GIANT_PART1:				case MODEL_ICE_GIANT_PART2:
			case MODEL_ICE_GIANT_PART3:				case MODEL_ICE_GIANT_PART4:
			case MODEL_ICE_GIANT_PART5:				case MODEL_ICE_GIANT_PART6:
				renderStatus = RenderObject(ThreadIndex, o);
				break;

			case MODEL_PROTECTGUILD:
				renderStatus = RenderObject(ThreadIndex, o);
				break;
			case MODEL_ARROW_AUTOLOAD:
				if (o->SubType == 1)
					renderStatus = RenderObject(ThreadIndex, o);
				break;
			case MODEL_INFINITY_ARROW:
				if (o->SubType == 1)
					renderStatus = RenderObject(ThreadIndex, o);
				break;

			case MODEL_INFINITY_ARROW1:
			case MODEL_INFINITY_ARROW2:
			case MODEL_INFINITY_ARROW3:
			case MODEL_INFINITY_ARROW4:
				renderStatus = RenderObject(ThreadIndex, o);
				break;

			case MODEL_ARROW_BEST_CROSSBOW:
				renderStatus = RenderObject(ThreadIndex, o);
				break;

			case MODEL_ALICE_BUFFSKILL_EFFECT:
			case MODEL_ALICE_BUFFSKILL_EFFECT2:
				{
					if (o->SubType == 0 || o->SubType == 1 || o->SubType == 2)
					{
						renderStatus = RenderObject(ThreadIndex, o);
					}
				}
				break;

			case MODEL_RAKLION_BOSS_CRACKEFFECT:
				{
					renderStatus = RenderObject(ThreadIndex, o);
				}
				break;

			case MODEL_RAKLION_BOSS_MAGIC:
				{
					renderStatus = RenderObject(ThreadIndex, o);

					if (MU_UPDATECOUNTER > 0)
					{
						if (o->SubType == 0)
						{
							Luminosity = (mu_float)GetLargeRand(4, 8) * 0.1f;
							Vector(Luminosity*0.4f, Luminosity*0.4f, Luminosity*1.0f, Light);
							RenderTerrainAlphaBitmap(BITMAP_LIGHT, BLEND::TXB_BLEND01, o->Position[0], o->Position[1], 15.0f, 15.0f, Light, -o->Angle[2]);
							Vector(Luminosity*1.0f, Luminosity*1.0f, Luminosity*1.0f, Light);
							RenderTerrainAlphaBitmap(BITMAP_LIGHT, BLEND::TXB_BLEND01, o->Position[0], o->Position[1], 5.0f, 5.0f, Light, -o->Angle[2]);
						}
					}
				}
				break;

			case MODEL_LAVAGIANT_FOOTPRINT_R:
			case MODEL_LAVAGIANT_FOOTPRINT_V:
				{
					if (MU_UPDATECOUNTER > 0)
					{
						if (o->Type == MODEL_LAVAGIANT_FOOTPRINT_R)
						{
							RenderTerrainAlphaBitmap(BITMAP_LAVAGIANT_FOOTPRINT_R, BLEND::TXB_BLEND01, o->Position[0], o->Position[1], o->Scale, o->Scale, o->Light, 0.0f, 1.0f, 5.0f);
						}
						else
						{
							RenderTerrainAlphaBitmap(BITMAP_LAVAGIANT_FOOTPRINT_V, BLEND::TXB_BLEND01, o->Position[0], o->Position[1], o->Scale, o->Scale, o->Light, 0.0f, 1.0f, 5.0f);
						}
					}
				}
				break;
			case MODEL_MOVE_TARGETPOSITION_EFFECT:
				{
					renderStatus = RenderObject(ThreadIndex, o);
				}
				break;
			case MODEL_EFFECT_SAPITRES_ATTACK_1:
			case MODEL_EFFECT_SAPITRES_ATTACK_2:
				{
					renderStatus = RenderObject(ThreadIndex, o);
				}
				break;
			case MODEL_BLOW_OF_DESTRUCTION:
				{
					if (MU_UPDATECOUNTER > 0)
					{
						if (o->SubType == 0)
						{
							if (o->LifeTime <= 24)
							{
								EVector3 vLight, vPos;
								Vector(0.5f, 0.5f, 1.0f, vLight);
								VectorCopy(o->Position, vPos);
								vPos[2] += 65.0f;
								mu_float fScale = (mu_float)(GetLargeRand(10) * 0.011f) + 3.0f;
								CreateSprite(BITMAP_SWORD_EFFECT_MONO, vPos, fScale, vLight, nullptr);
							}
						}
						else if (o->SubType == 1)
						{
							if (o->LifeTime <= 24)
							{
								EVector3 vLight, vPos;
								Vector(o->Light[0] * 0.5f, o->Light[1] * 0.5f, o->Light[2] * 1.0f, vLight);
								VectorCopy(o->Position, vPos);
								vPos[2] += 100.0f;
								CreateSprite(BITMAP_LIGHT, vPos, o->Scale, vLight, nullptr);
							}
						}

						if (o->SubType == 0)
						{
							if (o->LifeTime <= 24)
							{
								RenderTerrainAlphaBitmap(BITMAP_FLARE_BLUE, BLEND::TXB_BLEND01, o->Position[0], o->Position[1], 4.0f, 4.0f, o->Light, -o->Angle[2]);
							}
						}
						else if (o->SubType == 1)
						{
							if (o->LifeTime <= 24)
							{
								RenderTerrainAlphaBitmap(BITMAP_FLARE_BLUE, BLEND::TXB_BLEND01, o->Position[0], o->Position[1], 6.0f, 6.0f, o->Light, -o->Angle[2]);
							}
						}
					}
				}
				break;
			case MODEL_NIGHTWATER_01:
			case MODEL_KNIGHT_PLANCRACK_A:
			case MODEL_KNIGHT_PLANCRACK_B:
				{
					renderStatus = RenderObject(ThreadIndex, o);
				}
				break;
			case MODEL_EFFECT_FLAME_STRIKE:
				{
					if (o->SubType == 0)
					{
						OBJECT * pObject = o;
						MUModel * pModel = &MODELS::Data[pObject->Type];
						OBJECT * pOwner = pObject->Owner;
						MUModel * pOwnerModel = &MODELS::Data[pOwner->Type];

						EVector3 vPos, p;
						Vector(0.0f, 0.0f, 0.0f, p);
						pOwnerModel->RotationPosition(ThreadIndex, pOwner->BoneTransform[pOwnerModel->GetBoneIndex(33)], p, vPos);
						VectorAdd(pOwner->Position, vPos, pModel->BodyOrigin(ThreadIndex));
						Vector(0.0f, 0.0f, 0.0f, pObject->Angle);

						OBB_t OBB;
						EVector3 Temp;
						pModel->Animation(ThreadIndex, ThreadInfo.BoneTransform, pObject->AnimationFrame, pObject->PriorAnimationFrame, pObject->PriorAction, pObject->Angle, pObject->Angle, true);
						ThreadInfo.SkeletalIndex = g_Tasking.RegisterSkeletal(ThreadInfo.BoneTransform, pModel->_BoneNonDummyEnd);
						pObject->BoneTransform = &g_Tasking._SkeletonBuffer[ThreadInfo.SkeletalIndex];
						ThreadInfo.Translate = true;
						pModel->Transform(ThreadIndex, Temp, Temp, &OBB, true);

						BodyLight(ThreadIndex, pObject, pModel);
						pModel->BodyScale(ThreadIndex) = pObject->Scale;

						pModel->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, pObject->Alpha, 0, pObject->Alpha, pObject->BlendMeshTexCoordU, pObject->BlendMeshTexCoordV, -1);
						mu_float fV = (((mu_int32)(FWorldTime*0.05) % 16) / 4)*0.25f;
						pModel->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, pObject->Alpha, 1, pObject->Alpha, pObject->BlendMeshTexCoordU, fV);
					}
				}
				break;
			case BITMAP_LIGHT_MARKS:
				{
					if (MU_UPDATECOUNTER > 0)
					{
						mu_float fLight = 1.035f;
						if (o->LifeTime >= 35)
						{
							o->Light[0] /= fLight;
							o->Light[1] /= fLight;
							o->Light[2] /= fLight;
						}
						else
						{
							o->Light[0] *= fLight;
							o->Light[1] *= fLight;
							o->Light[2] *= fLight;
						}

						MUModel* pModel = &MODELS::Data[o->Owner->Type];
						EVector3 vPos(0.0f, 0.0f, 0.0f);
						const mu_int32 nBoneCount = 14;
						mu_int32 nBone[nBoneCount]
							= { 20, 20, 19, 18, 17, 2, 35, 26, 36, 27, 37, 28, 39, 30 };
						mu_float fScale[nBoneCount]
							= { 1.5f, 1.5f, 0.6f, 1.1f, 0.9f, 0.8f, 0.6f, 0.6f,
							0.8f, 0.8f, 0.8f, 0.8f, 0.7f, 0.7f };
						for (mu_int32 i = 0; i < nBoneCount; ++i)
						{
							pModel->TransformByObjectBone(ThreadIndex, vPos, o->Owner, nBone[i]);
							CreateSprite(BITMAP_LIGHT_MARKS, vPos, o->Scale * fScale[i], o->Light, o->Owner);
						}
					}
				}
				break;
			case MODEL_SWELL_OF_MAGICPOWER_BUFF_EFF:
				{
					if (MU_UPDATECOUNTER > 0)
					{
						if (o->SubType == 0)
						{
							if (o->Owner->Type != MODEL_PLAYER)
								break;

							MUModel* pModel = &MODELS::Data[o->Owner->Type];
							EVector3 vPos, vDLight;

							mu_float fLumi = (EMath::Abs((EMath::Sin(FWorldTime*0.001f))) + 0.2f)*0.5f;
							Vector(fLumi*0.7f, fLumi*0.3f, fLumi*0.9f, vDLight);

							for (mu_int32 i = 0; i < pModel->_BoneNonDummyEnd; ++i)
							{
								pModel->TransformByObjectBoneDirect(ThreadIndex, vPos, o->Owner, i);

								CreateSprite(BITMAP_LIGHT, vPos, 1.8f, vDLight, o);
							}
						}
					}
				}
				break;
			case MODEL_PROJECTILE:
				{
					renderStatus = RenderObject(ThreadIndex, o);
				}
				break;

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
					renderStatus = RenderObject(ThreadIndex, o);
				}
				break;
			case MODEL_SWORDLEFT01_EMPIREGUARDIAN_BOSS_GAION_:
			case MODEL_SWORDLEFT02_EMPIREGUARDIAN_BOSS_GAION_:
			case MODEL_SWORDRIGHT01_EMPIREGUARDIAN_BOSS_GAION_:
			case MODEL_SWORDRIGHT02_EMPIREGUARDIAN_BOSS_GAION_:
			case MODEL_SWORDMAIN01_EMPIREGUARDIAN_BOSS_GAION_:
				{
					MUModel *pBMDSwordModel = &MODELS::Data[o->Type];

					EVector3 *arrEachBoneTranslations = ThreadInfo.BonePosition;
					EVector3 v3CurrentHighHierarchyNodePos;

					if (o->Flags.Get(OBJECT::eOF_CHROMEENABLE) == true)
					{
						MUModel *pOwnerModel = &MODELS::Data[o->Owner->Type];

						mu_int32 arrBoneIdxs[] = { 4, 2, 8, 10, 6 };
						mu_int32 iBoneIdx = arrBoneIdxs[o->Type - MODEL_SWORDLEFT01_EMPIREGUARDIAN_BOSS_GAION_];

						pBMDSwordModel->AnimationTransformWithAttachHighModel_usingGlobalTM(ThreadIndex, o->Owner, pOwnerModel, iBoneIdx,
							&v3CurrentHighHierarchyNodePos, arrEachBoneTranslations, true);
					}
					else
					{
						pBMDSwordModel->BodyHeight(ThreadIndex) = 0.0f;
						BodyLight(ThreadIndex, o, pBMDSwordModel);
						pBMDSwordModel->BodyScale(ThreadIndex) = o->Scale;
						pBMDSwordModel->CurrentAction(ThreadIndex) = o->CurrentAction;
						VectorCopy(o->Position, pBMDSwordModel->BodyOrigin(ThreadIndex));

						pBMDSwordModel->Animation(ThreadIndex, ThreadInfo.BoneTransform, o->AnimationFrame, o->PriorAnimationFrame, o->PriorAction, o->Angle, o->HeadAngle, false, true);
						ThreadInfo.SkeletalIndex = g_Tasking.RegisterSkeletal(ThreadInfo.BoneTransform, pBMDSwordModel->_BoneNonDummyEnd);
						o->BoneTransform = &g_Tasking._SkeletonBuffer[ThreadInfo.SkeletalIndex];
						ThreadInfo.Translate = false;
						pBMDSwordModel->Transform(ThreadIndex, o->BoundingBoxMin, o->BoundingBoxMax, &o->OBB, false);

						pBMDSwordModel->AnimationTransformOnlySelf(ThreadIndex, arrEachBoneTranslations, o);
					}

					pBMDSwordModel->LightEnable(ThreadIndex) = true;

					Vector(1.0f, 1.0f, 1.0f, pBMDSwordModel->BodyLight(ThreadIndex));
					pBMDSwordModel->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);

					Vector(1.0f, 1.0f, 1.0f, pBMDSwordModel->BodyLight(ThreadIndex));
					pBMDSwordModel->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);

					Vector(0.2f, 0.2f, 1.0f, pBMDSwordModel->BodyLight(ThreadIndex));
					o->Alpha = 1.0f;
					pBMDSwordModel->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
					o->Alpha = 1.0f;

					if (MU_UPDATECOUNTER > 0 && o->SubType != 20)
					{
						EVector3	vRelative, vLight;
						EVector3	vPos_SwordEffectRed01,
							vPos_SwordEffectRed02,
							vPos_SwordEffectEdge01,
							vPos_SwordEffectEdge02,
							vPos_SwordEffectEdge03,
							vPos_SwordEffectEdge04,
							vPos_SwordEffectEdge05,
							vPos_SwordEffectEdge06,
							vPos_SwordEffectEdge07,
							vPos_SwordEffectEdge08,
							vPos_SwordEffectEdge09;
						mu_float	fLumi1, fLumi2;
						mu_int32		arrBoneIdxs_SwordEffectRed01[] = { 2,	9,	1,	3,	10 };
						mu_int32		arrBoneIdxs_SwordEffectRed02[] = { 3,	10, 2,	10, 11 };
						mu_int32		arrBoneIdxs_SwordEffectEdge01[] = { 13, 13, 3,	4,	1 };
						mu_int32		arrBoneIdxs_SwordEffectEdge02[] = { 12, 6,	4,	12, 2 };
						mu_int32		arrBoneIdxs_SwordEffectEdge03[] = { 5,	1,	5,	2,	3 };
						mu_int32		arrBoneIdxs_SwordEffectEdge04[] = { 6,	7,	6,	9,	4 };
						mu_int32		arrBoneIdxs_SwordEffectEdge05[] = { 1,	2,	12,	5,	5 };
						mu_int32		arrBoneIdxs_SwordEffectEdge06[] = { 8,	4,	8,	7,	6 };
						mu_int32		arrBoneIdxs_SwordEffectEdge07[] = { 9,	3,	9,	6,	7 };
						mu_int32		arrBoneIdxs_SwordEffectEdge08[] = { 10, 11, 10, 8,	8 };
						mu_int32		arrBoneIdxs_SwordEffectEdge09[] = { 4,	8,	7,	1,	9 };

						mu_int32		iBoneIdx_SwordEffectMain01 = arrBoneIdxs_SwordEffectRed01[o->Type - MODEL_SWORDLEFT01_EMPIREGUARDIAN_BOSS_GAION_];
						mu_int32		iBoneIdx_SwordEffectMain02 = arrBoneIdxs_SwordEffectRed02[o->Type - MODEL_SWORDLEFT01_EMPIREGUARDIAN_BOSS_GAION_];

						mu_int32		iBoneIdx_SwordEffectEdge01 = arrBoneIdxs_SwordEffectEdge01[o->Type - MODEL_SWORDLEFT01_EMPIREGUARDIAN_BOSS_GAION_];
						mu_int32		iBoneIdx_SwordEffectEdge02 = arrBoneIdxs_SwordEffectEdge02[o->Type - MODEL_SWORDLEFT01_EMPIREGUARDIAN_BOSS_GAION_];
						mu_int32		iBoneIdx_SwordEffectEdge03 = arrBoneIdxs_SwordEffectEdge03[o->Type - MODEL_SWORDLEFT01_EMPIREGUARDIAN_BOSS_GAION_];
						mu_int32		iBoneIdx_SwordEffectEdge04 = arrBoneIdxs_SwordEffectEdge04[o->Type - MODEL_SWORDLEFT01_EMPIREGUARDIAN_BOSS_GAION_];
						mu_int32		iBoneIdx_SwordEffectEdge05 = arrBoneIdxs_SwordEffectEdge05[o->Type - MODEL_SWORDLEFT01_EMPIREGUARDIAN_BOSS_GAION_];
						mu_int32		iBoneIdx_SwordEffectEdge06 = arrBoneIdxs_SwordEffectEdge06[o->Type - MODEL_SWORDLEFT01_EMPIREGUARDIAN_BOSS_GAION_];
						mu_int32		iBoneIdx_SwordEffectEdge07 = arrBoneIdxs_SwordEffectEdge07[o->Type - MODEL_SWORDLEFT01_EMPIREGUARDIAN_BOSS_GAION_];
						mu_int32		iBoneIdx_SwordEffectEdge08 = arrBoneIdxs_SwordEffectEdge08[o->Type - MODEL_SWORDLEFT01_EMPIREGUARDIAN_BOSS_GAION_];
						mu_int32		iBoneIdx_SwordEffectEdge09 = arrBoneIdxs_SwordEffectEdge09[o->Type - MODEL_SWORDLEFT01_EMPIREGUARDIAN_BOSS_GAION_];

						fLumi1 = 1.0f; fLumi2 = 1.0f;
						Vector(0.0f, 0.0f, 0.0f, vRelative);

						Vector(0.0f, 0.0f, 0.0f, vLight);

						Vector(fLumi1 * 1.0f, fLumi1 * 0.4f, fLumi1 * 0.1f, vLight);

						{
							VectorCopy(arrEachBoneTranslations[iBoneIdx_SwordEffectMain01], vPos_SwordEffectRed01);
							VectorCopy(arrEachBoneTranslations[iBoneIdx_SwordEffectMain02], vPos_SwordEffectRed02);
						}

						CreateSprite(BITMAP_LIGHT_RED, vPos_SwordEffectRed01, 1.3f, vLight, o);
						CreateSprite(BITMAP_LIGHT_RED, vPos_SwordEffectRed02, 1.3f, vLight, o);

						{
							VectorCopy(arrEachBoneTranslations[iBoneIdx_SwordEffectEdge01], vPos_SwordEffectEdge01);
							VectorCopy(arrEachBoneTranslations[iBoneIdx_SwordEffectEdge02], vPos_SwordEffectEdge02);
							VectorCopy(arrEachBoneTranslations[iBoneIdx_SwordEffectEdge03], vPos_SwordEffectEdge03);
							VectorCopy(arrEachBoneTranslations[iBoneIdx_SwordEffectEdge04], vPos_SwordEffectEdge04);
							VectorCopy(arrEachBoneTranslations[iBoneIdx_SwordEffectEdge05], vPos_SwordEffectEdge05);
							VectorCopy(arrEachBoneTranslations[iBoneIdx_SwordEffectEdge06], vPos_SwordEffectEdge06);
							VectorCopy(arrEachBoneTranslations[iBoneIdx_SwordEffectEdge07], vPos_SwordEffectEdge07);

							VectorCopy(arrEachBoneTranslations[iBoneIdx_SwordEffectEdge08], vPos_SwordEffectEdge08);
							VectorCopy(arrEachBoneTranslations[iBoneIdx_SwordEffectEdge09], vPos_SwordEffectEdge09);
						}

						Vector(fLumi1 * 0.3f, fLumi1 * 0.3f, fLumi1 * 1.0f, vLight);
						CreateSprite(BITMAP_LIGHT, vPos_SwordEffectEdge01, 1.5f, vLight, o);
						CreateSprite(BITMAP_LIGHT, vPos_SwordEffectEdge02, 1.5f, vLight, o);
						CreateSprite(BITMAP_LIGHT, vPos_SwordEffectEdge03, 1.5f, vLight, o);
						CreateSprite(BITMAP_LIGHT, vPos_SwordEffectEdge04, 1.5f, vLight, o);
						CreateSprite(BITMAP_LIGHT, vPos_SwordEffectEdge05, 1.5f, vLight, o);
						CreateSprite(BITMAP_LIGHT, vPos_SwordEffectEdge06, 1.5f, vLight, o);
						CreateSprite(BITMAP_LIGHT, vPos_SwordEffectEdge07, 1.5f, vLight, o);

						CreateSprite(BITMAP_LIGHT, vPos_SwordEffectEdge08, 1.0f, vLight, o);
						CreateSprite(BITMAP_LIGHT, vPos_SwordEffectEdge09, 1.0f, vLight, o);
					}
				}
				break;
			case MODEL_EMPIREGUARDIANBOSS_FRAMESTRIKE:
				{
					renderStatus = RenderObject(ThreadIndex, o);
				}
				break;
			case MODEL_EMPIREGUARDIAN_BLOW_OF_DESTRUCTION:
				{
					if (_UpdateCounter > 0)
					{
						if (o->SubType == 0)
						{
							if (o->LifeTime <= 24)
							{
								EVector3 vLight, vPos;
								Vector(0.5f, 0.5f, 1.0f, vLight);
								VectorCopy(o->Position, vPos);
								vPos[2] += 65.0f;
								mu_float fScale = (mu_float)(GetLargeRand(10) * 0.011f) + 3.0f;
								CreateSprite(BITMAP_SWORD_EFFECT_MONO, vPos, fScale, vLight, nullptr);
							}
						}
						else if (o->SubType == 1)
						{
							if (o->LifeTime <= 24)
							{
								EVector3 vLight, vPos;
								Vector(o->Light[0] * 0.5f, o->Light[1] * 0.5f, o->Light[2] * 1.0f, vLight);
								VectorCopy(o->Position, vPos);
								vPos[2] += 100.0f;
								CreateSprite(BITMAP_LIGHT, vPos, o->Scale, vLight, nullptr);
							}
						}
					}
				}
				break;

			case BITMAP_WATERFALL_4:
				{
				}
				break;

			case BITMAP_MAGIC:
				if (MU_UPDATECOUNTER > 0)
				{
					if (o->SubType == 1)
					{
						RenderTerrainAlphaBitmap(BITMAP_MAGIC, BLEND::TXB_BLEND01, o->Position[0], o->Position[1], o->Scale, o->Scale, o->Light, -o->Angle[2]);

						o->Light[0] /= 1.1f;
						o->Light[1] /= 1.1f;
						o->Light[2] /= 1.1f;
						o->Scale += 0.05f;
					}
					else if (o->SubType == 8)
					{
						RenderTerrainAlphaBitmap(o->Type, BLEND::TXB_BLEND01, o->Position[0], o->Position[1], o->Scale, o->Scale, o->Light, -o->Angle[2]);
						RenderTerrainAlphaBitmap(o->Type, BLEND::TXB_BLEND01, o->Position[0], o->Position[1], o->Scale*0.8f, o->Scale*0.8f, o->Light, -o->Angle[2]);
						RenderTerrainAlphaBitmap(o->Type, BLEND::TXB_BLEND01, o->Position[0], o->Position[1], o->Scale*1.2f, o->Scale*1.2f, o->Light, -o->Angle[2]);
					}
					else if (o->SubType == 9)
					{
						EVector3 vLight;
						Vector(o->Light[0] * o->Alpha, o->Light[1] * o->Alpha, o->Light[2] * o->Alpha, vLight);
						RenderTerrainAlphaBitmap(BITMAP_SUMMON_IMPACT, BLEND::TXB_BLEND01, o->Position[0], o->Position[1], o->Scale, o->Scale, vLight, o->HeadAngle[1]);
						RenderTerrainAlphaBitmap(BITMAP_SUMMON_IMPACT, BLEND::TXB_BLEND01, o->Position[0], o->Position[1], o->Scale*1.2f, o->Scale*1.2f, vLight, o->HeadAngle[2]);
					}
					else if (o->SubType == 10)
					{
						EVector3 vLight;
						Vector(o->Light[0] * o->Alpha, o->Light[1] * o->Alpha, o->Light[2] * o->Alpha, vLight);
						for (mu_int32 i = 0; i < 5; ++i)
						{
							if (o->LifeTime < 3 && i > 0) continue;
							else if (o->LifeTime < 6 && i > 1) continue;
							else if (o->LifeTime < 9 && i > 2) continue;
							else if (o->LifeTime < 12 && i > 3) continue;
							else if (o->LifeTime < 16 && i > 4) continue;
							RenderTerrainAlphaBitmap(BITMAP_LIGHT, BLEND::TXB_BLENDMINUS, o->Position[0], o->Position[1], o->Scale, o->Scale, vLight, -o->Angle[2]);
						}
					}
					else if (o->SubType == 11)
					{
						EVector3 vLight;
						Vector(o->Light[0] * o->Alpha, o->Light[1] * o->Alpha, o->Light[2] * o->Alpha, vLight);
						RenderTerrainAlphaBitmap(BITMAP_SUMMON_IMPACT, BLEND::TXB_BLEND01, o->Position[0], o->Position[1], o->Scale, o->Scale, vLight, o->HeadAngle[1]);
						RenderTerrainAlphaBitmap(BITMAP_SUMMON_IMPACT, BLEND::TXB_BLEND01, o->Position[0], o->Position[1], o->Scale*1.2f, o->Scale*1.2f, vLight, o->HeadAngle[2]);
					}
					else if (o->SubType == 12)
					{
						RenderTerrainAlphaBitmap(o->Type, BLEND::TXB_BLEND01, o->Position[0], o->Position[1], o->Scale, o->Scale, o->Light, -o->Angle[2]);
						RenderTerrainAlphaBitmap(o->Type, BLEND::TXB_BLEND01, o->Position[0], o->Position[1], o->Scale, o->Scale, o->Light, -o->Angle[2]);
					}
				}
				break;

			case BITMAP_MAGIC + 1:
				if (MU_UPDATECOUNTER > 0)
				{
					Luminosity = 1.0f;
					if (o->LifeTime < 5)
					{
						Luminosity -= (mu_float)(5 - o->LifeTime)*0.2f;
					}
					else if (o->SubType == 7)
					{
						if (o->LifeTime > 30)
							Luminosity = (mu_float)(40 - o->LifeTime)*0.1f;
					}

					if (o->SubType == 4 || o->SubType == 10)
					{
						Scale = o->Scale;
						if (Luminosity == 1.0f)
						{
							Luminosity = EMath::Sin((60 - o->LifeTime)*0.05f)*1.0f + 0.5f;
						}
					}
					else if (o->SubType == 6)
					{
						Scale = (80 - o->LifeTime)*0.05f;
					}
					else if (o->SubType == 7)
					{
						Scale = (o->LifeTime)*0.07f;
					}
					else if (o->SubType == 9)
					{
						Scale = o->Scale;
					}
					else if (o->SubType == 11)
					{
						Scale = (20 - o->LifeTime)*0.15f;
					}
					else if (o->SubType == 12)
					{
						Scale = (20 - o->LifeTime)*0.15f;
					}
					else if (o->SubType == 13)
					{
						Scale = o->Scale;
					}
					else
					{
						Scale = (20 - o->LifeTime)*0.15f;
					}

					if (o->SubType != 5 && o->SubType != 6 && o->SubType != 8)
					{
						mu_uint32 renderType = BLEND::TXB_BLEND01;
						switch (o->SubType)
						{
						case 0:
							Vector(Luminosity*0.4f, Luminosity*0.6f, Luminosity*1.0f, Light);
							break;
						case 1:
							Vector(Luminosity*0.4f, Luminosity*0.6f, Luminosity*1.0f, Light);
							break;
						case 2:
							Vector(Luminosity*0.4f, Luminosity*1.0f, Luminosity*0.6f, Light);
							break;
						case 3:
							Vector(Luminosity*1.0f, Luminosity*0.6f, Luminosity*0.4f, Light);
							break;
						case 4:
							Vector(Luminosity*1.0f, Luminosity*0.5f, Luminosity*0.1f, Light);
							break;
						case 7:
							Vector(Luminosity*0.9f, Luminosity*0.4f, Luminosity*0.2f, Light);
							break;
						case 9:
							Vector(Luminosity*0.9f, Luminosity*0.4f, Luminosity*0.2f, Light);
							break;
						case 10:
							Vector(Luminosity*0.1f, Luminosity*0.5f, Luminosity*1.0f, Light);
							break;
						case 11:
							Vector(Luminosity*o->Light[0], Luminosity*o->Light[1], Luminosity*o->Light[2], Light);
							break;
						case 12:
							renderType = BLEND::TXB_BLENDMINUS;
							Vector(Luminosity*o->Light[0], Luminosity*o->Light[1], Luminosity*o->Light[2], Light);
							break;
						case 13:
							Vector(o->Light[0], o->Light[1], o->Light[2], Light);
							break;
						}
						RenderTerrainAlphaBitmap(BITMAP_MAGIC + 1, renderType, o->Position[0], o->Position[1], Scale, Scale, Light, -o->Angle[2]);
					}
				}
				break;

			case BITMAP_FIRE_HIK2_MONO:
				if (MU_UPDATECOUNTER > 0)
				{
					if (o->SubType == 0)
					{
						Luminosity = (mu_float)GetLargeRand(4, 8) * 0.1f;
						Vector(Luminosity*1.0f, Luminosity*1.0f, Luminosity*1.0f, Light);
						RenderTerrainAlphaBitmap(BITMAP_LIGHTNING + 1, BLEND::TXB_BLEND01, o->Position[0], o->Position[1], 2.0f, 2.0f, Light, -o->Angle[2]);
					}
				}
				break;

			case BITMAP_CLOUD:
				if (MU_UPDATECOUNTER > 0)
				{
					if (o->SubType == 0)
					{
						Luminosity = (mu_float)GetLargeRand(4, 8) * 0.1f;
						Vector(Luminosity*o->Light[0], Luminosity*o->Light[1], Luminosity*o->Light[2], Light);
						RenderTerrainAlphaBitmap(BITMAP_CLOUD, BLEND::TXB_BLEND01, o->Position[0], o->Position[1], o->Scale, o->Scale, Light, -o->Angle[2]);
					}
				}
				break;

			case BITMAP_MAGIC + 2:
				if (MU_UPDATECOUNTER > 0)
				{
					Luminosity = 1.0f;
					if (o->SubType != 2)
					{
#if 0 // FIX
						mu_float Rotation = IWorldTime % 3600 / (mu_float)10.0f;
						RenderCircle(BITMAP_MAGIC + 2, o->Position, 90.0f, 130.0f, 200.0f, Rotation, 0.0f, 0.0f);
						RenderCircle(BITMAP_MAGIC + 2, o->Position, 90.0f, 130.0f, 200.0f, -Rotation, 0.0f, 0.0f);
#endif

						if (o->LifeTime < 5) Luminosity -= (mu_float)(5 - o->LifeTime)*0.2f;
						Scale = (20 - o->LifeTime)*0.15f;
					}
					else if (o->SubType == 2)
					{
						if (o->LifeTime > 10)
						{
							Scale = (20 - o->LifeTime)*0.55f;
						}
						else
						{
							Luminosity -= (mu_float)(10 - o->LifeTime)*0.1f;
						}
					}
					Vector(Luminosity*1.0f, Luminosity*0.4f, Luminosity*0.2f, Light);
					RenderTerrainAlphaBitmap(BITMAP_MAGIC + 1, BLEND::TXB_BLEND01, o->Position[0], o->Position[1], Scale, Scale, Light, -o->Angle[2]);
				}
				break;

			case BITMAP_MAGIC_ZIN:
				if (MU_UPDATECOUNTER > 0)
				{
					EVector3 vLight;
					switch (o->SubType)
					{
					case 0:
						Vector(o->Light[0] * o->Alpha * 2.0f, o->Light[1] * o->Alpha * 2.0f, o->Light[2] * o->Alpha * 2.0f, vLight);
						break;
					case 1:
						Vector(o->Light[0] * o->Alpha / 2.5f, o->Light[1] * o->Alpha / 2.5f, o->Light[2] * o->Alpha / 2.5f, vLight);
						break;
					case 2:
						Vector(o->Light[0] * o->Alpha, o->Light[1] * o->Alpha, o->Light[2] * o->Alpha, vLight);
						break;
					}
					RenderTerrainAlphaBitmap(o->Type, BLEND::TXB_BLEND01, o->Position[0], o->Position[1], o->Scale, o->Scale, vLight, o->HeadAngle[1]);
				}
				break;

			case BITMAP_OUR_INFLUENCE_GROUND:
				if (MU_UPDATECOUNTER > 0)
				{
					if (o->SubType == 0)
					{
						EVector3 vLight;

						Vector(0.6f*o->Alpha, 0.9f*o->Alpha, 1.0f*o->Alpha, vLight);
						RenderTerrainAlphaBitmap(o->Type, BLEND::TXB_BLEND01, o->Position[0], o->Position[1], o->Scale, o->Scale, vLight, 45.0f);

						Vector(0.6f*o->AlphaTarget, 0.9f*o->AlphaTarget, 1.0f*o->AlphaTarget, vLight);
						RenderTerrainAlphaBitmap(o->Type, BLEND::TXB_BLEND01, o->Position[0], o->Position[1], 0.8f, 0.8f, vLight, 45.0f);

						Vector(0.2f*o->AlphaTarget, 0.8f*o->AlphaTarget, 1.0f*o->AlphaTarget, vLight);
						RenderTerrainAlphaBitmap(BITMAP_LIGHT, BLEND::TXB_BLEND01, o->Position[0], o->Position[1], 2.0f, 2.0f, vLight);
						RenderTerrainAlphaBitmap(BITMAP_LIGHT, BLEND::TXB_BLEND01, o->Position[0], o->Position[1], 2.0f, 2.0f, vLight);
					}
				}
				break;

			case BITMAP_ENEMY_INFLUENCE_GROUND:
				if (MU_UPDATECOUNTER > 0)
				{
					if (o->SubType == 0)
					{
						EVector3 vLight;

						Vector(1.0f*o->Alpha, 0.3f*o->Alpha, 0.2f*o->Alpha, vLight);
						RenderTerrainAlphaBitmap(o->Type, BLEND::TXB_BLEND01, o->Position[0], o->Position[1], o->Scale*1.6f, o->Scale*1.6f, vLight);

						Vector(1.0f*o->AlphaTarget, 0.3f*o->AlphaTarget, 0.2f*o->AlphaTarget, vLight);
						RenderTerrainAlphaBitmap(o->Type, BLEND::TXB_BLEND01, o->Position[0], o->Position[1], 1.15f, 1.15f, vLight);

						Vector(1.0f*o->AlphaTarget, 1.0f*o->AlphaTarget, 1.0f*o->AlphaTarget, vLight);
						RenderTerrainAlphaBitmap(BITMAP_LIGHT_RED, BLEND::TXB_BLEND01, o->Position[0], o->Position[1], 1.8f, 1.8f, vLight);
						Vector(1.0f*o->AlphaTarget, 0.0f*o->AlphaTarget, 0.0f*o->AlphaTarget, vLight);
						RenderTerrainAlphaBitmap(BITMAP_LIGHT, BLEND::TXB_BLEND01, o->Position[0], o->Position[1], 2.0f, 2.0f, vLight);
					}
				}
				break;

			case BITMAP_FLAME:
				if (MU_UPDATECOUNTER > 0)
				{
					if (o->SubType != 3 && o->SubType != 6)
					{
						Luminosity = (mu_float)(GetLargeRand(4) + 8)*0.1f;
						Vector(Luminosity, Luminosity, Luminosity, Light);
						RenderTerrainAlphaBitmap(BITMAP_FLAME, BLEND::TXB_BLEND01, o->Position[0], o->Position[1], 2.0f, 2.0f, Light, -o->Angle[2]);
					}
				}
				break;

			case BITMAP_LIGHTNING + 1:
				if (MU_UPDATECOUNTER > 0)
				{
					o->Scale += 0.2f;
					Luminosity = (mu_float)(o->LifeTime)*0.1f;
					Vector(Luminosity, Luminosity, Luminosity, Light);
					RenderTerrainAlphaBitmap(BITMAP_LIGHTNING + 1, BLEND::TXB_BLEND01, o->Position[0], o->Position[1], o->Scale, o->Scale, Light, -o->Angle[2]);
				}
				break;

			case BITMAP_TWLIGHT:
			case BITMAP_SHOCK_WAVE:
				if (MU_UPDATECOUNTER > 0)
				{
					if (o->Type == BITMAP_SHOCK_WAVE && InHellas() && o->SubType != 6)
					{
#if 0 // FIX
						DisableDepthMask();
						RenderWaterTerrain(o->Type, BLEND::TXB_BLEND01, o->Position[0], o->Position[1], o->Scale, o->Scale, o->Light, -o->Angle[2]);
						EnableDepthMask();
#endif
					}
					else
					{
						RenderTerrainAlphaBitmap(o->Type, BLEND::TXB_BLEND01, o->Position[0], o->Position[1], o->Scale, o->Scale, o->Light, -o->Angle[2]);
					}
				}
				break;

			case BITMAP_DAMAGE_01_MONO:
				if (MU_UPDATECOUNTER > 0)
				{
					if (o->SubType == 0)
					{
						RenderTerrainAlphaBitmap(o->Type, BLEND::TXB_BLEND01, o->Position[0], o->Position[1], o->Scale, o->Scale, o->Light, -o->Angle[2]);
					}
					else if (o->SubType == 1)
					{
						RenderTerrainAlphaBitmap(o->Type, BLEND::TXB_BLEND01, o->Position[0], o->Position[1], o->Scale, o->Scale, o->Light, -o->Angle[2]);
					}
				}
				break;

			case BITMAP_CRATER:
				if (MU_UPDATECOUNTER > 0)
				{
					RenderTerrainAlphaBitmap(o->Type, BLEND::TXB_ALPHATEST_BEGIN, o->Position[0], o->Position[1], o->StartPosition[0], o->StartPosition[1], o->Light);
				}
				break;

			case BITMAP_CHROME_ENERGY2:
				if (MU_UPDATECOUNTER > 0)
				{
					RenderTerrainAlphaBitmap(o->Type, BLEND::TXB_BLEND01, o->Position[0], o->Position[1], o->StartPosition[0], o->StartPosition[1], o->Light);
				}
				break;

			case BITMAP_TARGET_POSITION_EFFECT1:
				if (MU_UPDATECOUNTER > 0)
				{
					if (o->SubType == 0)
					{
						RenderTerrainAlphaBitmap(o->Type, BLEND::TXB_BLEND01, o->Position[0], o->Position[1], o->Scale, o->Scale, o->Light);
					}
				}
				break;

			case BITMAP_TARGET_POSITION_EFFECT2:
				if (MU_UPDATECOUNTER > 0)
				{
					if (o->SubType == 0)
					{
						RenderTerrainAlphaBitmap(o->Type, BLEND::TXB_BLEND01, o->Position[0], o->Position[1], o->Scale, o->Scale, o->Light);
					}
				}
				break;

			case BITMAP_RING_OF_GRADATION:
				if (MU_UPDATECOUNTER > 0)
				{
					if (o->SubType == 0)
					{
						RenderTerrainAlphaBitmap(o->Type, BLEND::TXB_BLEND01, o->Position[0], o->Position[1], o->Scale, o->Scale, o->Light);
					}
				}
				break;

			default:
				if (o->Type >= MODEL_SKILL_BEGIN && o->Type < MODEL_SKILL_END)
				{
					renderStatus = RenderObject(ThreadIndex, o);
				}
				break;
			}

			if (renderStatus == RENDER_NOSLOT)
			{
				_DisableObjectRender = true;
			}
		}

		q -= queueCount;
	}
}