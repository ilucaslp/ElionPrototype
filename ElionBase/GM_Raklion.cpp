#include "stdafx.h"
#include "GM_Raklion.h"

SEASON4A::CGM_Raklion g_Raklion;

using namespace SEASON4A;

CGM_Raklion::CGM_Raklion() : m_bCanGoBossMap(true)
{
	m_byState = RAKLION_STATE_IDLE;
	m_byDetailState = BATTLE_OF_SELUPAN_NONE;
	m_Timer.SetTimer(0);
	m_bVisualEffect = false;
	m_bMusicBossMap = false;
	m_bBossHeightMove = false;
}

CGM_Raklion::~CGM_Raklion()
{
}

mu_boolean CGM_Raklion::CreateObject(const mu_uint32 ThreadIndex, mu_uint32 objectIndex, OBJECT* o)
{
	switch (o->Type)
	{
	case MODEL_WARP:
		{
		}
		return true;
	case MODEL_WARP4:
		{
			EVector3 Position;
			Vector(o->Position[0], o->Position[1] - 40.0f, o->Position[2] + 520.0f, Position);
			CreateEffect(ThreadIndex, MODEL_WARP4, Position, o->Angle, o->Light, 1);

			Vector(o->Position[0], o->Position[1] - 36.0f, o->Position[2] + 520.0f, Position);
			CreateEffect(ThreadIndex, MODEL_WARP5, Position, o->Angle, o->Light, 1);

			Vector(o->Position[0], o->Position[1] - 20.0f, o->Position[2] + 520.0f, Position);
			CreateEffect(ThreadIndex, MODEL_WARP6, Position, o->Angle, o->Light, 1);
		}
		return true;
	}

	return false;
}

CHARACTER* CGM_Raklion::CreateMonster(mu_int32 iType, mu_int32 PosX, mu_int32 PosY, mu_int32 Key)
{
	CHARACTER* pCharacter = nullptr;
	switch (iType)
	{
	case 454:
		pCharacter = CreateCharacter(Key, MODEL_MONSTER01 + 145, PosX, PosY);
		pCharacter->Object.Scale = 1.2f;
		pCharacter->Weapon[0].Type = -1;
		pCharacter->Weapon[1].Type = -1;
		break;

	case 455:
		pCharacter = CreateCharacter(Key, MODEL_MONSTER01 + 146, PosX, PosY);
		pCharacter->Object.Scale = 1.7f;
		pCharacter->Weapon[0].Type = -1;
		pCharacter->Weapon[1].Type = -1;
		break;

	case 456:
		pCharacter = CreateCharacter(Key, MODEL_MONSTER01 + 147, PosX, PosY);
		pCharacter->Object.Scale = 1.0f;
		pCharacter->Weapon[0].Type = -1;
		pCharacter->Weapon[1].Type = -1;
		pCharacter->Object.LifeTime = 100;
		break;

	case 457:
		pCharacter = CreateCharacter(Key, MODEL_MONSTER01 + 148, PosX, PosY);
		pCharacter->Object.Scale = 1.0f;
		pCharacter->Weapon[0].Type = -1;
		pCharacter->Weapon[1].Type = -1;
		break;

	case 458:
		pCharacter = CreateCharacter(Key, MODEL_MONSTER01 + 149, PosX, PosY);
		pCharacter->Object.Scale = 1.5f;
		pCharacter->Weapon[0].Type = -1;
		pCharacter->Weapon[1].Type = -1;
		break;

	case 459:
		{
			pCharacter = CreateCharacter(Key, MODEL_MONSTER01 + 150, PosX, PosY);
			pCharacter->Object.Scale = 2.0f;
			pCharacter->Weapon[0].Type = -1;
			pCharacter->Weapon[1].Type = -1;

			if (m_byState >= RAKLION_STATE_STANDBY && m_byState <= RAKLION_STATE_READY)
			{
				pCharacter->Object.Position[2] = 1000.0f;
				m_bBossHeightMove = true;
			}
		}
		break;

	case 460:
		{
			pCharacter = CreateCharacter(Key, MODEL_MONSTER01 + 151, PosX, PosY);
			pCharacter->Object.Scale = 0.3f;
			pCharacter->Weapon[0].Type = -1;
			pCharacter->Weapon[1].Type = -1;
		}
		break;

	case 461:
		{
			pCharacter = CreateCharacter(Key, MODEL_MONSTER01 + 152, PosX, PosY);
			pCharacter->Object.Scale = 0.3f;
			pCharacter->Weapon[0].Type = -1;
			pCharacter->Weapon[1].Type = -1;
		}
		break;

	case 462:
		{
			pCharacter = CreateCharacter(Key, MODEL_MONSTER01 + 153, PosX, PosY);
			pCharacter->Object.Scale = 0.3f;
			pCharacter->Weapon[0].Type = -1;
			pCharacter->Weapon[1].Type = -1;
		}
		break;

	case 562:
		{
			pCharacter = CreateCharacter(Key, MODEL_MONSTER01 + 205, PosX, PosY);
			pCharacter->Object.Scale = 1.9f;
			pCharacter->Weapon[0].Type = -1;
			pCharacter->Weapon[1].Type = -1;
		}
		break;

	case 563:
		{
			pCharacter = CreateCharacter(Key, MODEL_MONSTER01 + 206, PosX, PosY);
			pCharacter->Object.Scale = 1.1f;
			pCharacter->Weapon[0].Type = -1;
			pCharacter->Weapon[1].Type = -1;
			pCharacter->Object.LifeTime = 100;
		}
		break;

	case 564:
		{
			pCharacter = CreateCharacter(Key, MODEL_MONSTER01 + 207, PosX, PosY);
			pCharacter->Object.Scale = 1.3f;
			pCharacter->Weapon[0].Type = -1;
			pCharacter->Weapon[1].Type = -1;
		}
		break;

	case 565:
		{
			pCharacter = CreateCharacter(Key, MODEL_MONSTER01 + 208, PosX, PosY);
			pCharacter->Object.Scale = 1.8f;
			pCharacter->Weapon[0].Type = -1;
			pCharacter->Weapon[1].Type = -1;
		}
		break;
	}

	return pCharacter;
}

mu_boolean CGM_Raklion::MoveObject(const mu_uint32 ThreadIndex, OBJECT* o)
{
	switch (o->Type)
	{
	case 22:
		{
			o->BlendMeshLight = (mu_float)EMath::Sin(FWorldTime*0.001f) + 1.0f;
			return true;
		}
		break;
	case 70:
	case 80:
		{
			o->HiddenMesh = -2;
			return true;
		}
		break;
	}

	MoveEffect(ThreadIndex);

	return false;
}

mu_boolean CGM_Raklion::MoveMonsterVisual(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b)
{
	switch (o->Type)
	{
	case MODEL_MONSTER01 + 145:
		{
			switch (o->CurrentAction)
			{
			case MONSTER01_ATTACK2:
				{
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
			default:
				{

				}
				break;
			}
		}
		break;

	case MODEL_MONSTER01 + 146:
		{
			mu_float fActionSpeed = b->_AnimationHeader[o->CurrentAction].Speed;
			EVector3 Light;
			EVector3 EndPos, EndRelative;
			Vector(1.0f, 1.0f, 1.0f, Light);


			if (o->CurrentAction == MONSTER01_ATTACK2)
			{
				if (o->AnimationFrame >= 8.0f && o->AnimationFrame < (8.0f + fActionSpeed))
				{
					Vector(0.0f, 0.0f, 0.0f, EndRelative);
					b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(28)], EndRelative, EndPos, true);
					Vector(0.7f, 0.7f, 1.0f, Light);
					CreateParticle(ThreadIndex, BITMAP_CLUD64, EndPos, o->Angle, Light, 7, 2.0f);
					CreateParticle(ThreadIndex, BITMAP_CLUD64, EndPos, o->Angle, Light, 7, 2.0f);
					Vector(0.9f, 0.9f, 0.9f, Light);
					CreateEffect(ThreadIndex, BITMAP_SHOCK_WAVE, EndPos, o->Angle, Light, 8);
					CreateEffect(ThreadIndex, BITMAP_SHOCK_WAVE, EndPos, o->Angle, Light, 13);
					CreateEffect(ThreadIndex, BITMAP_SHOCK_WAVE, EndPos, o->Angle, Light, 13);

					for (mu_int32 iu = 0; iu < 60; ++iu)
					{
						Vector(0.4f, 0.8f, 0.9f, Light);
						CreateEffect(ThreadIndex, MODEL_EFFECT_BROKEN_ICE0 + GetLargeRand(3), EndPos, o->Angle, Light, 0);
					}
				}
			}
			else
			{
				EVector3 vPos, vRelative;
				Vector(0.0f, 0.0f, 0.0f, vRelative);
				if (o->CurrentAction == MONSTER01_DIE)
				{
					mu_float Scale = 3.5f;
					Vector(1.0f, 1.0f, 1.0f, o->Light);
					BoneManager::GetBonePosition(o, 45, vRelative, vPos);
					CreateParticle(ThreadIndex, BITMAP_SMOKE, vPos, o->Angle, o->Light, 53, Scale);
					BoneManager::GetBonePosition(o, 6, vRelative, vPos);
					CreateParticle(ThreadIndex, BITMAP_SMOKE, vPos, o->Angle, o->Light, 3, Scale);
					BoneManager::GetBonePosition(o, 7, vRelative, vPos);
					CreateParticle(ThreadIndex, BITMAP_SMOKE, vPos, o->Angle, o->Light, 53, Scale);
					BoneManager::GetBonePosition(o, 3, vRelative, vPos);
					CreateParticle(ThreadIndex, BITMAP_SMOKE, vPos, o->Angle, o->Light, 3, Scale);
					BoneManager::GetBonePosition(o, 4, vRelative, vPos);
					CreateParticle(ThreadIndex, BITMAP_SMOKE, vPos, o->Angle, o->Light, 53, Scale);
					BoneManager::GetBonePosition(o, 5, vRelative, vPos);
					CreateParticle(ThreadIndex, BITMAP_SMOKE, vPos, o->Angle, o->Light, 3, Scale);
				}
			}
		}
		break;

	case MODEL_MONSTER01 + 147:
		{

		}
		break;

	case MODEL_MONSTER01 + 148:
		{
			if (o->AnimationFrame >= 1.7f && o->AnimationFrame <= 2.0f)
			{
				switch (o->CurrentAction)
				{
				case MONSTER01_ATTACK1:
					{
						const mu_float fSize = 1.0f;
						EVector3 vLight, vPos;
						MUModel *b = &MODELS::Data[o->Type];

						Vector(0.4f, 0.6f, 1.0f, vLight);
						b->TransformByObjectBone(ThreadIndex, vPos, o, 19);
						CreateEffect(ThreadIndex, MODEL_1_STREAMBREATHFIRE,
							vPos,
							o->Angle,
							vLight,
							0, 0, -1, 0, 0, 0, fSize, -1);
					}
					break;
				case MONSTER01_ATTACK2:
					{
						const mu_float fSize = 1.0f;
						EVector3 vLight, vPos;
						MUModel *b = &MODELS::Data[o->Type];

						Vector(0.0f, 0.9f, 0.1f, vLight);
						b->TransformByObjectBone(ThreadIndex, vPos, o, 19);
						CreateEffect(ThreadIndex, MODEL_1_STREAMBREATHFIRE,
							vPos,
							o->Angle,
							vLight,
							0, 0, -1, 0, 0, 0, fSize, -1);
					}
					break;
				}
			}

			if (o->AnimationFrame <= 8.0f)
			{
				switch (o->CurrentAction)
				{
				case MONSTER01_DIE:
					{
						EVector3 vPos, vLight;

						if (GetLargeRand(3) != 0)
						{
							for (mu_int32 i = 0; i < b->_BoneNonDummyEnd; ++i)
							{
								b->TransformByObjectBoneDirect(ThreadIndex, vPos, o, i);
								if (GetLargeRand(5) == 0)
								{
									Vector(0.0f, 1.0f, 0.2f, vLight);
									CreateParticle(ThreadIndex, BITMAP_WATERFALL_5, vPos, o->Angle, vLight, 8, 2.0f);
								}
								if (GetLargeRand(5) == 0)
								{
									Vector(0.1f, 1.0f, 0.1f, vLight);
									CreateParticle(ThreadIndex, BITMAP_WATERFALL_3, vPos, o->Angle, vLight, 8, 2.5f);
								}
							}
						}

						if (GetLargeRand(3) == 0)
						{
							VectorCopy(o->Position, vPos);
							vPos[0] += (mu_float)(GetLargeRand(200) - 100);
							vPos[1] += (mu_float)(GetLargeRand(200) - 100);
							Vector(1.0f, 1.0f, 1.0f, vLight);
							CreateParticle(ThreadIndex, BITMAP_SMOKE, vPos, o->Angle, vLight, 1, 0.5f);
							CreateParticle(ThreadIndex, BITMAP_SMOKE, vPos, o->Angle, vLight, 24, 1.25f);

							VectorCopy(o->Position, vPos);
							vPos[0] += (mu_float)(GetLargeRand(250) - 125);
							vPos[1] += (mu_float)(GetLargeRand(250) - 125);
							Vector(0.1f, 1.0f, 0.1f, vLight);
							CreateEffect(ThreadIndex, BITMAP_CLOUD, vPos, o->Angle, vLight, 0, nullptr, -1, 0, 0, 0, 1.0f);

							Vector(1.0f, 1.0f, 1.0f, vLight);
							vPos[2] += 50.0f;
							CreateParticle(ThreadIndex, BITMAP_SPARK + 1, vPos, o->Angle, vLight, 5, 0.75f);
						}
					}
					break;
				}
			}
		}
		break;

	case MODEL_MONSTER01 + 149:
		{

		}
		break;

	case MODEL_MONSTER01 + 150:
		{
			if (o->CurrentAction == MONSTER01_ATTACK1)
			{
				EVector3 vLight, vPos;

				if (o->AnimationFrame >= 3.7f && o->AnimationFrame <= 4.0f)
				{
					Vector(0.2f, 0.4f, 1.0f, vLight);
					VectorCopy(o->Position, vPos);
					vPos[1] += 30.0f;
					CreateEffect(ThreadIndex, MODEL_RAKLION_BOSS_MAGIC, vPos, o->Angle, vLight, 0, o, -1, 0, 0, 0, 1.5f);

					Vector(0.2f, 0.4f, 1.0f, vLight);
					for (mu_int32 i = 0; i < 20; ++i)
					{
						vPos[0] = Hero->Object.Position[0] + (mu_float)(GetLargeRand(20) - 10)*80.0f + 500.0f;
						vPos[1] = Hero->Object.Position[1] + (mu_float)(GetLargeRand(20) - 10)*80.0f + 200.0f;
						vPos[2] = Hero->Object.Position[2] + 300.0f + (mu_float)(GetLargeRand(10))*100.0f;
						mu_float fScale = 1.0f + (GetLargeRand(10)) / 5.0f;
						mu_int32 iIndex = MODEL_EFFECT_BROKEN_ICE1;
						if (GetLargeRand(2) == 0)
						{
							iIndex = MODEL_EFFECT_BROKEN_ICE3;
						}
						CreateEffect(ThreadIndex, iIndex, vPos, o->Angle, vLight, 1, nullptr, -1, 0, 0, 0, fScale);
					}
				}
			}
			else if (o->CurrentAction == MONSTER01_ATTACK2)
			{
				EVector3 vLight, vPos, vAngle;

				Vector(0.0f, 0.9f, 0.1f, vLight);
				b->TransformByObjectBone(ThreadIndex, vPos, o, 0);
				CreateParticle(ThreadIndex, BITMAP_WATERFALL_3, vPos, o->Angle, vLight, 11, 2.0f);
				CreateParticle(ThreadIndex, BITMAP_SMOKE, vPos, o->Angle, vLight, 52, 2.0f);

				if (o->AnimationFrame >= 6.8f && o->AnimationFrame <= 7.0f)
				{
					EBone Matrix;
					EVector3 vDirection, vDirection2;
					VectorCopy(o->Position, vPos);
					vPos[2] += 100.0f;
					mu_float fAngle = 0.0f;
					Vector(0.0f, 0.0f, 0.0f, vAngle);
					for (mu_int32 i = 0; i < 5; ++i)
					{
						Vector(0.0f, 20.0f, 0.0f, vDirection);
						fAngle = o->Angle[2] + 150.0f + i*20.0f;
						Vector(0.0f, 0.0f, fAngle, vAngle);
						AngleMatrix(vAngle, Matrix);
						VectorRotate(vDirection, Matrix, vDirection2);
						CreateEffect(ThreadIndex, MODEL_MOONHARVEST_MOON, vPos, vDirection2, vLight, 1, nullptr, -1, 0, 0, 0, 0.4f);
					}
				}
			}
			else if (o->CurrentAction == MONSTER01_ATTACK3)
			{
				EVector3 vLight, vPos, vAngle, vPos2;
				EBone Matrix;

				Vector(0.3f, 0.5f, 1.0f, vLight);

				if (o->AnimationFrame >= 6.6f && o->AnimationFrame <= 7.0f)
				{
					CreateEffect(ThreadIndex, BITMAP_DAMAGE_01_MONO, o->Position, o->Angle, vLight, 0);

					for (mu_int32 i = 0; i < 3; ++i)
					{
						Vector(0.0f, 200.0f, 0.0f, vPos);
						Vector(0.0f, 0.0f, (i*120.0f), vAngle);
						AngleMatrix(vAngle, Matrix);
						VectorRotate(vPos, Matrix, vPos2);
						VectorAdd(vPos2, o->Position, vPos2);
						CreateEffect(ThreadIndex, BITMAP_FIRE_HIK2_MONO, vPos2, o->Angle, vLight, 0, o);
					}

					Vector(0.0f, 0.0f, 1.0f, vLight);
					CreateEffect(ThreadIndex, BITMAP_FIRE_HIK2_MONO, o->Position, o->Angle, vLight, 1, o);

					Vector(0.1f, 0.2f, 1.0f, vLight);
					CreateEffect(ThreadIndex, MODEL_RAKLION_BOSS_CRACKEFFECT, o->Position, o->Angle, vLight, 0, o, -1, 0, 0, 0, 2.0f);
				}
			}
			else if (o->CurrentAction == MONSTER01_APEAR)
			{
				EVector3 vLight, vPos, vAngle, vPos2;
				EBone Matrix;
				mu_float fScale = 1.0f;

				if (o->AnimationFrame <= 3.0f)
				{
					Vector(0.3f, 0.5f, 1.0f, vLight);

					for (mu_int32 i = 0; i < 1; ++i)
					{
						VectorCopy(o->Position, vPos);
						vPos[0] += (GetLargeRand(2000) - 1000.0f);
						vPos[1] += (GetLargeRand(2000) - 1000.0f);
						vPos[2] = 500.0f + GetLargeRand(100);

						fScale = 2.0f + (GetLargeRand(20)) / 5.0f;
						mu_int32 iIndex = MODEL_EFFECT_BROKEN_ICE1;

						CreateEffect(ThreadIndex, iIndex, vPos, o->Angle, vLight, 2, nullptr, -1, 0, 0, 0, fScale);

						VectorCopy(o->Position, vPos);
						vPos[0] += (GetLargeRand(2000) - 1000.0f);
						vPos[1] += (GetLargeRand(2000) - 1000.0f);
						vPos[2] = 500.0f + GetLargeRand(100);

						fScale = 0.5f + (GetLargeRand(10)) / 5.0f;
						iIndex = MODEL_EFFECT_BROKEN_ICE3;

						CreateEffect(ThreadIndex, iIndex, vPos, o->Angle, vLight, 2, nullptr, -1, 0, 0, 0, fScale);

						VectorCopy(o->Position, vPos);
						vPos[0] += (GetLargeRand(2000) - 1000.0f);
						vPos[1] += (GetLargeRand(2000) - 1000.0f);
						vPos[2] = 500.0f + GetLargeRand(100);

						Vector(1.0f, 0.8f, 0.8f, vLight);
						fScale = 0.05f + (GetLargeRand(10)) / 20.0f;
						CreateEffect(ThreadIndex, MODEL_FALL_STONE_EFFECT, vPos, o->Angle, vLight, 2, nullptr, -1, 0, 0, 0, fScale);

						Vector(0.7f, 0.7f, 0.8f, vLight);
						vPos[0] += (mu_float)(GetLargeRand(80) - 40);
						vPos[1] += (mu_float)(GetLargeRand(80) - 40);
						CreateParticle(ThreadIndex, BITMAP_WATERFALL_3 + (GetLargeRand(2)), vPos, o->Angle, vLight, 2);
					}
				}

				if (o->AnimationFrame >= 5.0f && o->AnimationFrame <= 7.0f)
				{
					EarthQuake.Store((mu_float)(GetLargeRand(4) - 2)*1.0f);

					if (o->AnimationFrame >= 5.6f && o->AnimationFrame <= 6.1f)
					{
						Vector(0.3f, 0.5f, 1.0f, vLight);

						CreateEffect(ThreadIndex, BITMAP_DAMAGE_01_MONO, o->Position, o->Angle, vLight, 0);

						for (mu_int32 i = 0; i < 3; ++i)
						{
							Vector(0.0f, 200.0f, 0.0f, vPos);
							Vector(0.0f, 0.0f, (i*120.0f), vAngle);
							AngleMatrix(vAngle, Matrix);
							VectorRotate(vPos, Matrix, vPos2);
							VectorAdd(vPos2, o->Position, vPos2);
							CreateEffect(ThreadIndex, BITMAP_FIRE_HIK2_MONO, vPos2, o->Angle, vLight, 0, o);
						}

						Vector(0.0f, 0.0f, 1.0f, vLight);
						CreateEffect(ThreadIndex, BITMAP_FIRE_HIK2_MONO, o->Position, o->Angle, vLight, 1, o);

						Vector(0.1f, 0.2f, 1.0f, vLight);
						CreateEffect(ThreadIndex, MODEL_RAKLION_BOSS_CRACKEFFECT, o->Position, o->Angle, vLight, 0, o, -1, 0, 0, 0, 2.0f);

						PlayBackground(SOUND_KANTURU_3RD_MAYAHAND_ATTACK2);
					}
				}
				else
				{
					EarthQuake.Store(0.0f);
				}
			}
			else if (o->CurrentAction == MONSTER01_DIE && o->AnimationFrame <= 8.0f)
			{
				EVector3 vPos, vLight;

				if (GetLargeRand(3) != 0)
				{
					for (mu_int32 i = 0; i < b->_BoneNonDummyEnd; ++i)
					{
						b->TransformByObjectBoneDirect(ThreadIndex, vPos, o, i);
						if (GetLargeRand(5) == 0)
						{
							Vector(0.0f, 1.0f, 0.2f, vLight);
							CreateParticle(ThreadIndex, BITMAP_WATERFALL_5, vPos, o->Angle, vLight, 8, 2.0f);
						}
						if (GetLargeRand(5) == 0)
						{
							Vector(0.1f, 1.0f, 0.1f, vLight);
							CreateParticle(ThreadIndex, BITMAP_WATERFALL_3, vPos, o->Angle, vLight, 8, 2.5f);
						}
					}
				}

				if (GetLargeRand(3) == 0)
				{
					VectorCopy(o->Position, vPos);
					vPos[0] += (mu_float)(GetLargeRand(400) - 200);
					vPos[1] += (mu_float)(GetLargeRand(400) - 200);
					Vector(1.0f, 1.0f, 1.0f, vLight);
					CreateParticle(ThreadIndex, BITMAP_SMOKE, vPos, o->Angle, vLight, 1, 1.0f);
					CreateParticle(ThreadIndex, BITMAP_SMOKE, vPos, o->Angle, vLight, 24, 2.5f);

					VectorCopy(o->Position, vPos);
					vPos[0] += (mu_float)(GetLargeRand(500) - 250);
					vPos[1] += (mu_float)(GetLargeRand(500) - 250);
					Vector(0.1f, 1.0f, 0.1f, vLight);
					CreateEffect(ThreadIndex, BITMAP_CLOUD, vPos, o->Angle, vLight, 0, nullptr, -1, 0, 0, 0, 2.0f);

					Vector(1.0f, 1.0f, 1.0f, vLight);
					vPos[2] += 50.0f;
					CreateParticle(ThreadIndex, BITMAP_SPARK + 1, vPos, o->Angle, vLight, 5, 1.5f);
				}
			}

			if (m_byDetailState >= BATTLE_OF_SELUPAN_PATTERN_3 && m_byDetailState <= BATTLE_OF_SELUPAN_PATTERN_7)
			{
				EVector3 vLight;
				mu_float fScale = 1.0f;
				if (m_byDetailState == BATTLE_OF_SELUPAN_PATTERN_3 || m_byDetailState == BATTLE_OF_SELUPAN_PATTERN_4)
				{
					fScale = 0.5f;
				}
				else if (m_byDetailState == BATTLE_OF_SELUPAN_PATTERN_5)
				{
					fScale = 1.0f;
				}
				else if (m_byDetailState == BATTLE_OF_SELUPAN_PATTERN_6 || m_byDetailState == BATTLE_OF_SELUPAN_PATTERN_7)
				{
					fScale = 1.5f;
				}

				Vector(1.0f, 0.1f, 0.1f, vLight);
				for (mu_int32 k = 0; k < 1; ++k)
				{
					if (GetLargeRand(2) == 0)
					{
						EVector3 vPos1, vPos2;
						b->TransformByObjectBone(ThreadIndex, vPos1, o, 34);
						CreateParticle(ThreadIndex, BITMAP_SMOKE, vPos1, o->Angle, vLight, 50, fScale);
						CreateParticle(ThreadIndex, BITMAP_SMOKELINE1 + GetLargeRand(3), vPos1, o->Angle, vLight, 0, fScale);

						b->TransformByObjectBone(ThreadIndex, vPos2, o, 52);
						CreateParticle(ThreadIndex, BITMAP_SMOKE, vPos2, o->Angle, vLight, 50, fScale);
						CreateParticle(ThreadIndex, BITMAP_SMOKELINE1 + GetLargeRand(3), vPos2, o->Angle, vLight, 0, fScale);

						if (m_byDetailState == BATTLE_OF_SELUPAN_PATTERN_6 || m_byDetailState == BATTLE_OF_SELUPAN_PATTERN_7)
						{
							CreateParticle(ThreadIndex, BITMAP_FIRE_HIK1_MONO, vPos1, o->Angle, vLight, 0, o->Scale);
							CreateParticle(ThreadIndex, BITMAP_FIRE_HIK1_MONO, vPos2, o->Angle, vLight, 0, o->Scale);
						}
					}
				}
			}
		}
		break;
	case MODEL_MONSTER01 + 151:
	case MODEL_MONSTER01 + 152:
	case MODEL_MONSTER01 + 153:
		{
			if (o->CurrentAction == MONSTER01_DIE && o->AnimationFrame <= 12.0f)
			{
				EVector3 vPos, vLight;

				for (mu_int32 i = 0; i < 1; ++i)
				{
					VectorCopy(o->Position, vPos);

					if (i == 0)
					{
						vPos[0] += 100.0f;
					}
					else if (i == 1)
					{
						vPos[0] += 100.0f;
						vPos[1] += 100.0f;
					}

					vPos[0] += GetLargeRand(60) - 30.0f;
					vPos[1] += GetLargeRand(60) - 30.0f;
					vPos[2] += GetLargeRand(20) - 10.0f;
					Vector(0.2f, 0.4f, 1.0f, vLight);
					CreateParticle(ThreadIndex, BITMAP_SMOKE, vPos, o->Angle, vLight, 11, 1.2f);

					if (GetLargeRand(2) == 0)
					{
						VectorCopy(o->Position, vPos);
						vPos[0] += (mu_float)(GetLargeRand(200) - 100);
						vPos[1] += (mu_float)(GetLargeRand(200) - 100);
						Vector(0.2f, 0.4f, 1.0f, vLight);
						CreateEffect(ThreadIndex, BITMAP_CLOUD, vPos, o->Angle, vLight, 0, nullptr, -1, 0, 0, 0, 1.0f);
					}
				}
			}
		}
		break;

	case MODEL_MONSTER01 + 205:
		{
			mu_float fActionSpeed = b->_AnimationHeader[o->CurrentAction].Speed;

			EVector3 Light;
			EVector3 EndPos, EndRelative;
			Vector(1.0f, 1.0f, 1.0f, Light);


			if (o->CurrentAction == MONSTER01_ATTACK2)
			{
				if (o->AnimationFrame >= 8.0f && o->AnimationFrame < (8.0f + fActionSpeed))
				{
					Vector(0.0f, 0.0f, 0.0f, EndRelative);
					b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(28)], EndRelative, EndPos, true);
					Vector(0.7f, 0.7f, 1.0f, Light);
					CreateParticle(ThreadIndex, BITMAP_CLUD64, EndPos, o->Angle, Light, 7, 2.0f);
					CreateParticle(ThreadIndex, BITMAP_CLUD64, EndPos, o->Angle, Light, 7, 2.0f);
					Vector(0.9f, 0.9f, 0.9f, Light);
					CreateEffect(ThreadIndex, BITMAP_SHOCK_WAVE, EndPos, o->Angle, Light, 8);
					CreateEffect(ThreadIndex, BITMAP_SHOCK_WAVE, EndPos, o->Angle, Light, 13);
					CreateEffect(ThreadIndex, BITMAP_SHOCK_WAVE, EndPos, o->Angle, Light, 13);

					for (mu_int32 iu = 0; iu < 60; ++iu)
					{
						Vector(0.4f, 0.8f, 0.9f, Light);
						CreateEffect(ThreadIndex, MODEL_EFFECT_BROKEN_ICE0 + GetLargeRand(3), EndPos, o->Angle, Light, 0);
					}
				}
			}
			else
			{
				EVector3 vPos, vRelative;
				Vector(0.0f, 0.0f, 0.0f, vRelative);
				if (o->CurrentAction == MONSTER01_DIE)
				{
					mu_float Scale = 3.5f;
					Vector(1.0f, 1.0f, 1.0f, o->Light);
					BoneManager::GetBonePosition(o, 45, vRelative, vPos);
					CreateParticle(ThreadIndex, BITMAP_SMOKE, vPos, o->Angle, o->Light, 53, Scale);
					BoneManager::GetBonePosition(o, 6, vRelative, vPos);
					CreateParticle(ThreadIndex, BITMAP_SMOKE, vPos, o->Angle, o->Light, 3, Scale);
					BoneManager::GetBonePosition(o, 7, vRelative, vPos);
					CreateParticle(ThreadIndex, BITMAP_SMOKE, vPos, o->Angle, o->Light, 53, Scale);
					BoneManager::GetBonePosition(o, 3, vRelative, vPos);
					CreateParticle(ThreadIndex, BITMAP_SMOKE, vPos, o->Angle, o->Light, 3, Scale);
					BoneManager::GetBonePosition(o, 4, vRelative, vPos);
					CreateParticle(ThreadIndex, BITMAP_SMOKE, vPos, o->Angle, o->Light, 53, Scale);
					BoneManager::GetBonePosition(o, 5, vRelative, vPos);
					CreateParticle(ThreadIndex, BITMAP_SMOKE, vPos, o->Angle, o->Light, 3, Scale);
				}
			}
		}
		break;
	case MODEL_MONSTER01 + 206:
		{

		}
		break;
	case MODEL_MONSTER01 + 207:
		{
			if (o->AnimationFrame >= 1.7f && o->AnimationFrame <= 2.0f)
			{
				switch (o->CurrentAction)
				{
				case MONSTER01_ATTACK1:
					{
						const mu_float fSize = 1.0f;
						EVector3 vLight, vPos;
						MUModel *b = &MODELS::Data[o->Type];

						Vector(0.4f, 0.6f, 1.0f, vLight);
						b->TransformByObjectBone(ThreadIndex, vPos, o, 19);
						CreateEffect(ThreadIndex, MODEL_1_STREAMBREATHFIRE,
							vPos,
							o->Angle,
							vLight,
							0, 0, -1, 0, 0, 0, fSize, -1);
					}
					break;
				case MONSTER01_ATTACK2:
					{
						const mu_float fSize = 1.0f;
						EVector3 vLight, vPos;
						MUModel *b = &MODELS::Data[o->Type];

						Vector(0.0f, 0.9f, 0.1f, vLight);
						b->TransformByObjectBone(ThreadIndex, vPos, o, 19);
						CreateEffect(ThreadIndex, MODEL_1_STREAMBREATHFIRE,
							vPos,
							o->Angle,
							vLight,
							0, 0, -1, 0, 0, 0, fSize, -1);
					}
					break;
				}
			}

			if (o->AnimationFrame <= 8.0f)
			{
				switch (o->CurrentAction)
				{
				case MONSTER01_DIE:
					{
						EVector3 vPos, vLight;

						if (GetLargeRand(3) != 0)
						{
							for (mu_int32 i = 0; i < b->_BoneNonDummyEnd; ++i)
							{
								b->TransformByObjectBoneDirect(ThreadIndex, vPos, o, i);
								if (GetLargeRand(5) == 0)
								{
									Vector(0.0f, 1.0f, 0.2f, vLight);
									CreateParticle(ThreadIndex, BITMAP_WATERFALL_5, vPos, o->Angle, vLight, 8, 2.0f);
								}
								if (GetLargeRand(5) == 0)
								{
									Vector(0.1f, 1.0f, 0.1f, vLight);
									CreateParticle(ThreadIndex, BITMAP_WATERFALL_3, vPos, o->Angle, vLight, 8, 2.5f);
								}
							}
						}

						if (GetLargeRand(3) == 0)
						{
							VectorCopy(o->Position, vPos);
							vPos[0] += (mu_float)(GetLargeRand(200) - 100);
							vPos[1] += (mu_float)(GetLargeRand(200) - 100);
							Vector(1.0f, 1.0f, 1.0f, vLight);
							CreateParticle(ThreadIndex, BITMAP_SMOKE, vPos, o->Angle, vLight, 1, 0.5f);
							CreateParticle(ThreadIndex, BITMAP_SMOKE, vPos, o->Angle, vLight, 24, 1.25f);

							VectorCopy(o->Position, vPos);
							vPos[0] += (mu_float)(GetLargeRand(250) - 125);
							vPos[1] += (mu_float)(GetLargeRand(250) - 125);
							Vector(0.1f, 1.0f, 0.1f, vLight);
							CreateEffect(ThreadIndex, BITMAP_CLOUD, vPos, o->Angle, vLight, 0, nullptr, -1, 0, 0, 0, 1.0f);

							Vector(1.0f, 1.0f, 1.0f, vLight);
							vPos[2] += 50.0f;
							CreateParticle(ThreadIndex, BITMAP_SPARK + 1, vPos, o->Angle, vLight, 5, 0.75f);
						}
					}
					break;
				}
			}
		}
		break;
	case MODEL_MONSTER01 + 208:
		{

		}
		break;
	}

	return false;
}

void CGM_Raklion::MoveBlurEffect(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	switch (o->Type)
	{
	case MODEL_MONSTER01 + 145:
		{
		}
		break;

	case MODEL_MONSTER01 + 146:
		{
		}
		break;
	case MODEL_MONSTER01 + 147:
		{
			mu_float Start_Frame = 3.0f;
			mu_float End_Frame = 8.0f;
			if ((o->AnimationFrame >= Start_Frame && o->AnimationFrame <= End_Frame && o->CurrentAction == MONSTER01_ATTACK1)
				|| (o->AnimationFrame >= Start_Frame && o->AnimationFrame <= End_Frame && o->CurrentAction == MONSTER01_ATTACK2))
			{
				EVector3  Light;
				Vector(1.0f, 1.2f, 2.0f, Light);

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

					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(35)], StartRelative, StartPos, false);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(36)], EndRelative, EndPos, false);
					CreateBlur(c, StartPos, EndPos, Light, 0);

					fAnimationFrame += fSpeedPerFrame;
				}
			}
		}
		break;

	case MODEL_MONSTER01 + 148:
		{

		}
		break;

	case MODEL_MONSTER01 + 149:
		{

		}
		break;

	case MODEL_MONSTER01 + 150:
		{

		}
		break;

	case MODEL_MONSTER01 + 151:
	case MODEL_MONSTER01 + 152:
	case MODEL_MONSTER01 + 153:
		{

		}
		break;

	case MODEL_MONSTER01 + 205:
		{

		}
		break;

	case MODEL_MONSTER01 + 206:
		{
			mu_float Start_Frame = 3.0f;
			mu_float End_Frame = 8.0f;
			if ((o->AnimationFrame >= Start_Frame && o->AnimationFrame <= End_Frame && o->CurrentAction == MONSTER01_ATTACK1)
				|| (o->AnimationFrame >= Start_Frame && o->AnimationFrame <= End_Frame && o->CurrentAction == MONSTER01_ATTACK2))
			{
				EVector3  Light;
				Vector(1.0f, 1.2f, 2.0f, Light);

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

					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(35)], StartRelative, StartPos, false);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(36)], EndRelative, EndPos, false);
					CreateBlur(c, StartPos, EndPos, Light, 0);

					fAnimationFrame += fSpeedPerFrame;
				}
			}
		}
		break;

	case MODEL_MONSTER01 + 208:
		{

		}
		break;

	case MODEL_MONSTER01 + 207:
		{

		}
		break;
	}
}

mu_boolean CGM_Raklion::RenderObjectMesh(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b, mu_boolean ExtraMon)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	if (o->Type >= 6 && o->Type <= 12)
	{
		mu_boolean LightEnable = b->LightEnable(ThreadIndex);
		EVector3 BodyLight = b->BodyLight(ThreadIndex);

		if(LightEnable == true)
		{
			b->LightEnable(ThreadIndex) = false;
			b->BodyLight(ThreadIndex) *= 0.5f;
		}

		Vector(1.0f, 1.0f, 1.0f, o->Light);
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		
		b->LightEnable(ThreadIndex) = LightEnable;
		b->BodyLight(ThreadIndex) = BodyLight;

		return true;
	}
	else if (o->Type == 16)
	{
		if (o->AnimationFrame >= 19)
		{
			SetAction(o, GetLargeRand(2));
		}

		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);

		return true;
	}
	else if (o->Type == 17)
	{
#if 0 // FIX this if was bugged but fixed it, check if animation is good
		if (o->CurrentAction <= 1)
		{
			mu_int32 iAniIndex = GetLargeRand(100);

			if (iAniIndex < 90)
			{
				if (iAniIndex % 2 == 0)
					SetAction(o, 0);
				else
					SetAction(o, 1);
			}
			else
			{
				if (iAniIndex % 2 == 0)
					SetAction(o, 2);
				else
					SetAction(o, 3);
			}
		}
		else
#endif
		if (o->CurrentAction == 2 && o->AnimationFrame >= 97)
		{
			SetAction(o, GetLargeRand(2));
		}
		else if (o->CurrentAction == 3 && o->AnimationFrame >= 98)
		{
			SetAction(o, GetLargeRand(2));
		}

		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);

		return true;
	}
	else if (o->Type == 19)
	{
		if(MU_UPDATECOUNTER > 0)
		{
			if (o->CurrentAction == 0)
			{
				EVector3 vRelativePos, vWorldPos, vLight;
				Vector(0, 0, 0, vRelativePos);
				Vector(1.0f, 1.0f, 1.0f, vLight);

				if ((o->AnimationFrame >= 2 && o->AnimationFrame <= 5))
				{
					for (mu_int32 i = 0; i < b->_BoneNonDummyEnd; ++i)
					{
						b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[i], vRelativePos, vWorldPos, false);
						CreateParticle(ThreadIndex, BITMAP_WATERFALL_5, vWorldPos, o->Angle, vLight, 7);
					}
				}
				else if (o->AnimationFrame >= 11 && o->AnimationFrame <= 12)
				{
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(6)], vRelativePos, vWorldPos, false);
					CreateParticle(ThreadIndex, BITMAP_WATERFALL_5, vWorldPos, o->Angle, vLight, 7);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(7)], vRelativePos, vWorldPos, false);
					CreateParticle(ThreadIndex, BITMAP_WATERFALL_5, vWorldPos, o->Angle, vLight, 7);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(8)], vRelativePos, vWorldPos, false);
					CreateParticle(ThreadIndex, BITMAP_WATERFALL_5, vWorldPos, o->Angle, vLight, 7);
				}
				else if (o->AnimationFrame >= 13 && o->AnimationFrame <= 14)
				{
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(9)], vRelativePos, vWorldPos, false);
					CreateParticle(ThreadIndex, BITMAP_WATERFALL_5, vWorldPos, o->Angle, vLight, 7);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(10)], vRelativePos, vWorldPos, false);
					CreateParticle(ThreadIndex, BITMAP_WATERFALL_5, vWorldPos, o->Angle, vLight, 7);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(11)], vRelativePos, vWorldPos, false);
					CreateParticle(ThreadIndex, BITMAP_WATERFALL_5, vWorldPos, o->Angle, vLight, 7);
				}
				else if (o->AnimationFrame >= 15 && o->AnimationFrame <= 17)
				{
					for (mu_int32 i = 12; i < 20; ++i)
					{
						b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(i)], vRelativePos, vWorldPos, false);
						CreateParticle(ThreadIndex, BITMAP_WATERFALL_5, vWorldPos, o->Angle, vLight, 7);
					}
				}
				else if (o->AnimationFrame >= 19.5f)
				{
					o->CurrentAction = 1;
				}


				for (mu_int32 i = 0; i < b->_BoneNonDummyEnd; ++i)
				{
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[i], vRelativePos, vWorldPos, false);
					CreateParticle(ThreadIndex, BITMAP_WATERFALL_3, vWorldPos, o->Angle, vLight, 8, 1.5f);
				}
			}
			else if (o->CurrentAction == 1)
			{
				if (GetLargeRand(40) == 30)
				{
					o->CurrentAction = 0;
				}
			}
		}

		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);

		return true;
	}
	else if (o->Type == 20)
	{
		b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		b->RenderMesh(ThreadIndex, 1, RENDER_CHROME2 | RENDER_DARK, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_CHROME8);
		b->RenderMesh(ThreadIndex, 1, RENDER_CHROME | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_CHROME8);

		if(MU_UPDATECOUNTER > 0)
		{
			if (o->CurrentAction == 0)
			{
				EVector3 vRelativePos, vWorldPos, vLight;
				Vector(0, 0, 0, vRelativePos);
				Vector(1.0f, 1.0f, 1.0f, vLight);

				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(5)], vRelativePos, vWorldPos, false);
				CreateParticle(ThreadIndex, BITMAP_WATERFALL_3, vWorldPos, o->Angle, vLight, 8, 2.0f);

				if (o->AnimationFrame <= 8)
				{
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(6)], vRelativePos, vWorldPos, false);
					CreateParticle(ThreadIndex, BITMAP_WATERFALL_3, vWorldPos, o->Angle, vLight, 8, 1.5f);
					CreateParticle(ThreadIndex, BITMAP_WATERFALL_5, vWorldPos, o->Angle, vLight, 7, 0.1f);

				}

				if (o->AnimationFrame >= 12)
				{
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(6)], vRelativePos, vWorldPos, false);
					CreateParticle(ThreadIndex, BITMAP_WATERFALL_3, vWorldPos, o->Angle, vLight, 8, 2.0f);
					CreateParticle(ThreadIndex, BITMAP_WATERFALL_5, vWorldPos, o->Angle, vLight, 7, 0.1f);
				}

				if (o->AnimationFrame <= 15)
				{
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(8)], vRelativePos, vWorldPos, false);
					CreateParticle(ThreadIndex, BITMAP_WATERFALL_3, vWorldPos, o->Angle, vLight, 9, 2.0f);
					CreateParticle(ThreadIndex, BITMAP_WATERFALL_5, vWorldPos, o->Angle, vLight, 7, 0.1f);

					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(11)], vRelativePos, vWorldPos, false);
					CreateParticle(ThreadIndex, BITMAP_WATERFALL_3, vWorldPos, o->Angle, vLight, 9, 2.0f);
					CreateParticle(ThreadIndex, BITMAP_WATERFALL_5, vWorldPos, o->Angle, vLight, 7, 0.1f);
				}

				if (o->AnimationFrame >= 19)
				{
					o->CurrentAction = 1;
				}
			}
			else if (o->CurrentAction == 1)
			{
				if (GetLargeRand(40) == 25)
				{
					o->CurrentAction = 0;
				}
			}
		}

		return true;
	}
	else if (o->Type == 21)
	{
		if(MU_UPDATECOUNTER > 0)
		{
			if (o->CurrentAction == 0)
			{
				if (o->AnimationFrame >= 4 && o->AnimationFrame <= 8)
				{
					EVector3 vRelativePos, vWorldPos, vLight;
					Vector(0, 0, 0, vRelativePos);
					Vector(1.0f, 1.0f, 1.0f, vLight);

					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(7)], vRelativePos, vWorldPos, false);

					CreateParticle(ThreadIndex, BITMAP_WATERFALL_3, vWorldPos, o->Angle, vLight, 9, 0.5f);
					CreateParticle(ThreadIndex, BITMAP_WATERFALL_5, vWorldPos, o->Angle, vLight, 7);

					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(7)], vRelativePos, vWorldPos, false);
					CreateParticle(ThreadIndex, BITMAP_WATERFALL_2, vWorldPos, o->Angle, vLight, 5, 1.0f);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(16)], vRelativePos, vWorldPos, false);
					CreateParticle(ThreadIndex, BITMAP_WATERFALL_2, vWorldPos, o->Angle, vLight, 5, 0.5f);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(17)], vRelativePos, vWorldPos, false);
					CreateParticle(ThreadIndex, BITMAP_WATERFALL_2, vWorldPos, o->Angle, vLight, 5, 0.7f);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(20)], vRelativePos, vWorldPos, false);
					CreateParticle(ThreadIndex, BITMAP_WATERFALL_2, vWorldPos, o->Angle, vLight, 5, 0.3f);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(21)], vRelativePos, vWorldPos, false);
					CreateParticle(ThreadIndex, BITMAP_WATERFALL_2, vWorldPos, o->Angle, vLight, 5, 0.3f);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(22)], vRelativePos, vWorldPos, false);
					CreateParticle(ThreadIndex, BITMAP_WATERFALL_2, vWorldPos, o->Angle, vLight, 5, 0.5f);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(23)], vRelativePos, vWorldPos, false);
					CreateParticle(ThreadIndex, BITMAP_WATERFALL_2, vWorldPos, o->Angle, vLight, 5, 0.3f);
					b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(24)], vRelativePos, vWorldPos, false);
					CreateParticle(ThreadIndex, BITMAP_WATERFALL_2, vWorldPos, o->Angle, vLight, 5, 0.3f);
				}

				if (o->AnimationFrame >= 19.0f)
				{
					o->CurrentAction = 1;
				}
			}
			else if (o->CurrentAction == 1)
			{
				if (GetLargeRand(40) == 19)
				{
					o->CurrentAction = 0;
				}
			}
		}

		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);

		return true;
	}
	else if (o->Type == 46 || o->Type == 53)
	{
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);

		return true;
	}
	else if (o->Type == 76)
	{
		mu_float R, G, B;
		R = (mu_float)EMath::Sin(FWorldTime*0.002f) * 0.2f + 0.5f;
		G = (mu_float)EMath::Sin(FWorldTime*0.0015f) * 0.2f + 0.5f;
		B = (mu_float)EMath::Sin(FWorldTime*0.0014f) * 0.2f + 0.5f;
		Vector(R, G, B, o->Light);
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);

		return true;
	}
	else if (o->Type == 57)
	{
		EVector3 vRelativePos, vWorldPos;
		Vector(0, 0, 0, vRelativePos);
		b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(1)], vRelativePos, vWorldPos, false);
		EVector3 vLight;
		Vector(1.0f, 1.0f, 1.0f, vLight);

		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);

		return true;
	}
	else if (o->Type == 68 || o->Type == 69 || o->Type == 71)
	{
		mu_float fLumi = (EMath::Sin(FWorldTime*0.001f) + 1.0f) * 0.5f + 0.3f;
		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, 0, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);

		return true;
	}
	else if (o->Type == MODEL_WARP || o->Type == MODEL_WARP2 || o->Type == MODEL_WARP3)
	{
		b->BodyLight(ThreadIndex)[0] = 1.0f;
		b->BodyLight(ThreadIndex)[1] = 1.0f;
		b->BodyLight(ThreadIndex)[2] = 1.0f;
		o->BlendMeshLight = 1.0f;

		if (o->Type == MODEL_WARP)
		{
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		else if (o->Type == MODEL_WARP2)
		{
			b->RenderBody(ThreadIndex, RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		else if (o->Type == MODEL_WARP3)
		{
			b->RenderMesh(ThreadIndex, 0, RENDER_DARK, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}

		return true;
	}
	else if (o->Type == MODEL_WARP4 || o->Type == MODEL_WARP5 || o->Type == MODEL_WARP6)
	{
		if (m_bCanGoBossMap == false)
		{
			return true;
		}

		b->BodyLight(ThreadIndex)[0] = 0.5f;
		b->BodyLight(ThreadIndex)[1] = 0.6f;
		b->BodyLight(ThreadIndex)[2] = 1.0f;
		o->BlendMeshLight = 0.8f;

		if (o->Type == MODEL_WARP4)
		{
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		else if (o->Type == MODEL_WARP5)
		{
			b->RenderBody(ThreadIndex, RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}
		else if (o->Type == MODEL_WARP6)
		{
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
		}

		return true;
	}
	else if (o->Type == 82)
	{
		if (m_bCanGoBossMap == false)
		{
			o->AnimationFrame = 0;
			o->PriorAnimationFrame = 0;
		}

		b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);

		return true;
	}
	else if ((o->Type >= MODEL_MONSTER01 + 145 && o->Type <= MODEL_MONSTER01 + 153)
		|| o->Type == MODEL_MONSTER01 + 205
		|| o->Type == MODEL_MONSTER01 + 206
		|| o->Type == MODEL_MONSTER01 + 208
		|| o->Type == MODEL_MONSTER01 + 207
		)
	{
		RenderMonster(ThreadIndex, o, b, ExtraMon);

		return true;
	}

	return false;
}

mu_boolean CGM_Raklion::RenderMonster(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b, mu_boolean ExtraMon)
{
	switch (o->Type)
	{
	case MODEL_MONSTER01 + 145:
		{
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);

			return true;
		}
		break;

	case MODEL_MONSTER01 + 146:
		{
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			b->RenderMesh(ThreadIndex, 1, RENDER_CHROME6 | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);

			return true;
		}
		break;

	case MODEL_MONSTER01 + 147:
		if (o->CurrentAction != MONSTER01_DIE)
		{
			Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT | RENDER_CHROME7, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 3, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 3, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 3, RENDER_TEXTURE | RENDER_BRIGHT | RENDER_CHROME, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);

			return true;
		}
		break;

	case MODEL_MONSTER01 + 148:
		{
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);

			return true;
		}
		break;

	case MODEL_MONSTER01 + 149:
		if (o->CurrentAction == MONSTER01_DIE)
		{
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		else
		{
			mu_float fLumi = (EMath::Sin(FWorldTime*0.01f) + 1.0f) * 0.4f + 0.2f;
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_IRONKNIGHT_BODY_BRIGHT);
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, 0.4f, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 1, 0.7f, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			b->RenderMesh(ThreadIndex, 1, RENDER_CHROME | RENDER_BRIGHT, 0.5f, 1, 0.5f, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}

		return true;

	case MODEL_MONSTER01 + 150:
		{
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			b->RenderMesh(ThreadIndex, 3, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			b->RenderMesh(ThreadIndex, 4, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			b->RenderMesh(ThreadIndex, 5, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 5, o->BlendMeshLight, IWorldTime % 10000 * 0.0002f, IWorldTime % 10000 * 0.0002f, o->HiddenMesh);

			mu_float fLumi2 = (EMath::Sin(FWorldTime*0.002f) + 1.0f) * 0.5f;
			b->RenderMesh(ThreadIndex, 4, RENDER_TEXTURE | RENDER_CHROME4 | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, fLumi2, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_SERUFAN_ARM_R);
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, fLumi2, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_SERUFAN_ARM_R);
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, fLumi2, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_SERUFAN_ARM_R);
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, fLumi2, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_SERUFAN_ARM_R);

			if (m_byDetailState >= BATTLE_OF_SELUPAN_PATTERN_2)
			{
				b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_CHROME6 | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_MAGIC_EMBLEM);
				b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_CHROME4 | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
			}

			b->RenderMesh(ThreadIndex, 3, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);

			return true;
		}
		break;
	case MODEL_MONSTER01 + 151:
	case MODEL_MONSTER01 + 152:
	case MODEL_MONSTER01 + 153:
		{
			if (o->CurrentAction == MONSTER01_DIE)
			{
				mu_float fBlendLight = 20.0f - o->AnimationFrame;
				fBlendLight /= 15;
				EVector3 vOriginPos;
				VectorCopy(o->Position, vOriginPos);

				for (mu_int32 i = 0; i < 3; ++i)
				{
					if (i == 0)
					{
						if (o->Type == MODEL_MONSTER01 + 151)
							o->Position[0] += 60;
						else
							o->Position[0] += 100;
					}
					else if (i == 1)
					{
						if (o->Type == MODEL_MONSTER01 + 151)
							o->Position[1] += 60;
						else
							o->Position[1] += 100;
					}
					else
					{
						VectorCopy(vOriginPos, o->Position);
					}

					Calc_RenderObject(ThreadIndex, o, true, false, 0);
					b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
					b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
					b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
					b->RenderMesh(ThreadIndex, 3, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 3, fBlendLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
				}
			}
			else
			{
				EVector3 vOriginPos;
				VectorCopy(o->Position, vOriginPos);

				for (mu_int32 i = 0; i < 3; ++i)
				{
					if (i == 0)
					{
						if (o->Type == MODEL_MONSTER01 + 151)
							o->Position[0] += 60;
						else
							o->Position[0] += 100;
					}
					else if (i == 1)
					{
						if (o->Type == MODEL_MONSTER01 + 151)
							o->Position[1] += 60;
						else
							o->Position[1] += 100;
					}
					else
					{
						VectorCopy(vOriginPos, o->Position);
					}

					Calc_RenderObject(ThreadIndex, o, true, false, 0);
					b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);

					if(MU_UPDATECOUNTER > 0)
					{
						EVector3 vPos, vLight;
						mu_float fLumi = (EMath::Sin(FWorldTime*0.004f) + 1.2f) * 0.5f + 0.1f;
						Vector(0.1f*fLumi, 0.6f*fLumi, 0.7f*fLumi, vLight);
						if (o->Type == MODEL_MONSTER01 + 151)
						{
							b->TransformByObjectBone(ThreadIndex, vPos, o, 57);
							CreateSprite(BITMAP_LIGHT, vPos, 3.0f, vLight, o);
							if (GetLargeRand(100) == 0)
								CreateParticle(ThreadIndex, BITMAP_SMOKE, vPos, o->Angle, vLight, 33, 1.0f);
						}
						else if (o->Type == MODEL_MONSTER01 + 152)
						{
							b->TransformByObjectBone(ThreadIndex, vPos, o, 95);
							CreateSprite(BITMAP_LIGHT, vPos, 3.0f, vLight, o);
							if (GetLargeRand(100) == 0)
								CreateParticle(ThreadIndex, BITMAP_SMOKE, vPos, o->Angle, vLight, 33, 1.0f);
							b->TransformByObjectBone(ThreadIndex, vPos, o, 115);
							CreateSprite(BITMAP_LIGHT, vPos, 3.0f, vLight, o);
							if (GetLargeRand(100) == 0)
								CreateParticle(ThreadIndex, BITMAP_SMOKE, vPos, o->Angle, vLight, 33, 1.0f);
						}
						else
						{
							b->TransformByObjectBone(ThreadIndex, vPos, o, 95);
							CreateSprite(BITMAP_LIGHT, vPos, 3.0f, vLight, o);
							if (GetLargeRand(100) == 0)
								CreateParticle(ThreadIndex, BITMAP_SMOKE, vPos, o->Angle, vLight, 33, 1.0f);
							b->TransformByObjectBone(ThreadIndex, vPos, o, 115);
							CreateSprite(BITMAP_LIGHT, vPos, 3.0f, vLight, o);
							if (GetLargeRand(100) == 0)
								CreateParticle(ThreadIndex, BITMAP_SMOKE, vPos, o->Angle, vLight, 33, 1.0f);
							b->TransformByObjectBone(ThreadIndex, vPos, o, 173);
							CreateSprite(BITMAP_LIGHT, vPos, 3.0f, vLight, o);
							if (GetLargeRand(100) == 0)
								CreateParticle(ThreadIndex, BITMAP_SMOKE, vPos, o->Angle, vLight, 33, 1.0f);
						}
					}
				}
			}

			return true;
		}
		break;

	case MODEL_MONSTER01 + 205:
		{
			b->RenderBody(ThreadIndex, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			b->RenderMesh(ThreadIndex, 1, RENDER_CHROME6 | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);

			return true;
		}
		break;

	case MODEL_MONSTER01 + 206:
		{
			if (o->CurrentAction != MONSTER01_DIE)
			{
				Vector(1.0f, 1.0f, 1.0f, b->BodyLight(ThreadIndex));
				b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
				b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
				b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
				b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT | RENDER_CHROME7, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
				b->RenderMesh(ThreadIndex, 2, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
				b->RenderMesh(ThreadIndex, 3, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
				b->RenderMesh(ThreadIndex, 3, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);
				b->RenderMesh(ThreadIndex, 3, RENDER_TEXTURE | RENDER_BRIGHT | RENDER_CHROME, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV);

				return true;
			}
		}
		break;
	case MODEL_MONSTER01 + 207:
		{
			b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
		}
		break;
	case MODEL_MONSTER01 + 208:
		{
			if (o->CurrentAction == MONSTER01_DIE)
			{
				b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			}
			else
			{
				mu_float fLumi = (EMath::Sin(FWorldTime*0.01f) + 1.0f) * 0.4f + 0.2f;
				b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE, o->Alpha, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
				b->RenderMesh(ThreadIndex, 0, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 0, fLumi, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, BITMAP_IRONKNIGHT_BODY_BRIGHT);
				b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE, 0.4f, o->BlendMesh, o->BlendMeshLight, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
				b->RenderMesh(ThreadIndex, 1, RENDER_TEXTURE | RENDER_BRIGHT, o->Alpha, 1, 0.7f, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
				b->RenderMesh(ThreadIndex, 1, RENDER_CHROME | RENDER_BRIGHT, 0.5f, 1, 0.5f, o->BlendMeshTexCoordU, o->BlendMeshTexCoordV, o->HiddenMesh);
			}

			return true;
		}
		break;
	}

	return false;
}

mu_boolean CGM_Raklion::RenderObjectVisual(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b)
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

			return true;
		}
		break;
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

			return true;
		}
		break;
	}

	return false;
}

mu_boolean CGM_Raklion::RenderMonsterVisual(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	EVector3 vPos, vRelative, vLight;

	switch (o->Type)
	{
	case MODEL_MONSTER01 + 145:
		{

		}
		break;

	case MODEL_MONSTER01 + 146:
		{

		}
		break;
	case MODEL_MONSTER01 + 147:
		if (o->CurrentAction == MONSTER01_ATTACK2 && o->AnimationFrame > 7.4f && o->AnimationFrame < 7.7f)
		{
			CreateInferno(ThreadIndex, o->Position, 5);
		}
		else if (o->CurrentAction == MONSTER01_DIE && o->LifeTime == 100)
		{
			const mu_int32 nBonesCount = 8;
			mu_int32 iBones[nBonesCount] = { 4, 7, 10, 22, 39, 44, 12, 24 };
			for (mu_int32 i = 0; i < nBonesCount; ++i)
			{
				Vector(0.3f, 0.6f, 1.0f, vLight);

				b->TransformByObjectBone(ThreadIndex, vPos, o, iBones[i]);
				for (mu_int32 j = 0; j < 12; ++j)
					CreateEffect(ThreadIndex, MODEL_EFFECT_BROKEN_ICE0 + GetLargeRand(3), vPos, o->Angle, vLight, 0);
			}

			o->LifeTime = 90;

			Vector(1.0f, 1.0f, 1.0f, vLight);

			b->TransformByObjectBone(ThreadIndex, vPos, o, 5);
			CreateEffect(ThreadIndex, MODEL_ICE_GIANT_PART1, vPos, o->Angle, vLight, 0, o, 0, 0);
			b->TransformByObjectBone(ThreadIndex, vPos, o, 3);
			CreateEffect(ThreadIndex, MODEL_ICE_GIANT_PART2, vPos, o->Angle, vLight, 0, o, 0, 0);
			b->TransformByObjectBone(ThreadIndex, vPos, o, 11);
			CreateEffect(ThreadIndex, MODEL_ICE_GIANT_PART3, vPos, o->Angle, vLight, 0, o, 0, 0);
			b->TransformByObjectBone(ThreadIndex, vPos, o, 23);
			CreateEffect(ThreadIndex, MODEL_ICE_GIANT_PART4, vPos, o->Angle, vLight, 0, o, 0, 0);
			b->TransformByObjectBone(ThreadIndex, vPos, o, 39);
			CreateEffect(ThreadIndex, MODEL_ICE_GIANT_PART5, vPos, o->Angle, vLight, 0, o, 0, 0);
			b->TransformByObjectBone(ThreadIndex, vPos, o, 45);
			CreateEffect(ThreadIndex, MODEL_ICE_GIANT_PART6, vPos, o->Angle, vLight, 0, o, 0, 0);
		}
		break;

	case MODEL_MONSTER01 + 148:
		{

		}
		break;

	case MODEL_MONSTER01 + 149:
		{
			EVector3  Light;
			Vector(1.0f, 1.2f, 2.0f, Light);

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

				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(35)], StartRelative, StartPos, false);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(36)], EndRelative, EndPos, false);
				CreateBlur(c, StartPos, EndPos, Light, 3);

				fAnimationFrame += fSpeedPerFrame;
			}

			mu_int32 iBones[] = { 20, 37, 45, 51 };
			for (mu_int32 i = 0; i < 4; ++i)
			{
				if (GetLargeRand(6) > 0) continue;

				if (GetLargeRand(3) > 0)
				{
					Vector(0.3f, 0.6f, 1.0f, vLight);
				}
				else
				{
					Vector(0.8f, 0.8f, 0.8f, vLight);
				}

				b->TransformByObjectBone(ThreadIndex, vPos, o, iBones[i]);
				CreateParticle(ThreadIndex, BITMAP_SMOKE, vPos, o->Angle, vLight, 50, 1.0f);
				CreateParticle(ThreadIndex, BITMAP_SMOKELINE1 + GetLargeRand(3), vPos, o->Angle, vLight, 2, 0.8f);
			}

			if (o->CurrentAction == MONSTER01_ATTACK2)
			{
				if (2 <= c->AttackTime && c->AttackTime <= 8)
				{
					for (mu_int32 j = 0; j < 3; ++j)
					{
						EVector3 CurPos;
						VectorCopy(o->Position, CurPos);
						CurPos[2] += 120.0f;
						EVector3 TempPos;
						GetNearRandomPos(CurPos, 300, TempPos);
						mu_float fDistance = 1400.0f;
						TempPos[0] += -fDistance * EMath::Sin(o->Angle[2] * Q_PI / 180.0f);
						TempPos[1] += fDistance * EMath::Cos(o->Angle[2] * Q_PI / 180.0f);
						CreateJoint(ThreadIndex, MODEL_SPEARSKILL, TempPos, TempPos, o->Angle, 2, o, 40.0f);
					}
				}
				if (c->AttackTime <= 8)
				{
					EVector3 Position2 = { 0.0f, 0.0f, 0.0f };
					b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(26)], Position2, o->m_vPosSword, true);

					mu_float fDistance = 300.0f;
					o->m_vPosSword[0] += fDistance * EMath::Sin(o->Angle[2] * Q_PI / 180.0f);
					o->m_vPosSword[1] += -fDistance * EMath::Cos(o->Angle[2] * Q_PI / 180.0f);
				}
				if (6 <= c->AttackTime && c->AttackTime <= 12)
				{
					EVector3 Position;
					EVector3 Position2 = { 0.0f, 0.0f, 0.0f };

					b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(26)], Position2, Position, true);

					mu_float fDistance = 100.0f + (mu_float)(c->AttackTime - 8) * 10.0f;
					Position[0] += fDistance * EMath::Sin(o->Angle[2] * Q_PI / 180.0f);
					Position[1] += -fDistance * EMath::Cos(o->Angle[2] * Q_PI / 180.0f);
					EVector3 Light = { 1.0f, 1.0f, 1.0f };
					CreateEffect(ThreadIndex, MODEL_SPEAR, Position, o->Angle, Light, 1, o);
					
					if (c->TargetCharacter != -1)
					{
						CHARACTER *tc = &CharactersClient[c->TargetCharacter];
						OBJECT *to = &tc->Object;
						if (10 <= c->AttackTime && to->Flags.Get(OBJECT::eOF_LIVE) == true)
						{
							to->m_byHurtByOneToOne = 35;
						}
					}
				}
				if (c->AttackTime >= 12)
				{
					c->AttackTime = ThreadInfo.LimitAttackTime;

					CreateEffect(ThreadIndex, MODEL_COMBO, o->Position, o->Angle, o->Light);
				}
			}
			else if (o->CurrentAction == MONSTER01_DIE && o->AnimationFrame < 1.0f)
			{
				mu_int32 iBones[] = { 20, 37, 45, 51 };
				for (mu_int32 i = 0; i < 4; ++i)
				{
					Vector(0.3f, 0.5f, 1.0f, vLight);

					b->TransformByObjectBone(ThreadIndex, vPos, o, iBones[i]);
					for (mu_int32 j = 0; j < 6; ++j)
						CreateEffect(ThreadIndex, MODEL_EFFECT_BROKEN_ICE0 + GetLargeRand(3), vPos, o->Angle, vLight, 0);
				}
			}
		}
		break;

	case MODEL_MONSTER01 + 150:
		{
			EVector3  Light;
			Vector(1.0f, 1.2f, 2.0f, Light);

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

				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(69)], StartRelative, StartPos, false);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(73)], EndRelative, EndPos, false);
				CreateBlur(c, StartPos, EndPos, Light, 0);

				fAnimationFrame += fSpeedPerFrame;
			}

			VectorCopy(o->Position, b->BodyOrigin(ThreadIndex));
			Vector(0.0f, 0.0f, 0.0f, vRelative);

			mu_float fLumi1 = (EMath::Sin(FWorldTime*0.004f) + 1.0f) * 0.25f;
			mu_float fLumi2 = (EMath::Sin(FWorldTime*0.004f) + 1.0f) * 0.2f;

			Vector(0.3f + fLumi1, 0.6f + fLumi1, 1.0f + fLumi1, vLight);
			Vector(0.0f, 10.0f, 0.0f, vRelative);
			b->TransformByObjectBone(ThreadIndex, vPos, o, 9, &vRelative);

			CreateSprite(BITMAP_LIGHT, vPos, 2.0f + fLumi2, vLight, o);
			CreateSprite(BITMAP_LIGHT, vPos, 2.0f + fLumi2, vLight, o);
			CreateSprite(BITMAP_LIGHT, vPos, 3.0f + fLumi2, vLight, o);

			Vector(0.0f, 0.0f, 0.0f, vRelative);
			fLumi1 = (EMath::Sin(FWorldTime*0.004f) + 1.0f) * 0.1f;
			Vector(0.1f + fLumi1, 0.2f + fLumi1, 1.0f + fLumi1, vLight);
			Vector(0.0f, 0.0f, 0.0f, vRelative);
			b->TransformByObjectBone(ThreadIndex, vPos, o, 70);

			CreateSprite(BITMAP_LIGHT, vPos, 2.1f + fLumi2, vLight, o, IWorldTime*0.08f);
			CreateSprite(BITMAP_SHINY + 6, vPos, 1.8f + fLumi2, vLight, o);

			Vector(0.0f, 0.0f, 0.0f, vRelative);
			b->TransformByObjectBone(ThreadIndex, vPos, o, 74);

			CreateSprite(BITMAP_LIGHT, vPos, 2.1f + fLumi2, vLight, o, IWorldTime*0.08f);
			CreateSprite(BITMAP_SHINY + 6, vPos, 1.8f + fLumi2, vLight, o);

			b->TransformByObjectBone(ThreadIndex, vPos, o, 71);

			CreateSprite(BITMAP_EVENT_CLOUD, vPos, 0.6f, vLight, o, -IWorldTime*0.1f);
			CreateSprite(BITMAP_EVENT_CLOUD, vPos, 0.40f, vLight, o, -IWorldTime*0.2f);
			CreateSprite(BITMAP_LIGHT, vPos, 2.3f + fLumi2, vLight, o);
			CreateSprite(BITMAP_SHINY + 1, vPos, 2.3f + fLumi2, vLight, o, IWorldTime*0.08f);
			CreateSprite(BITMAP_SHINY + 6, vPos, 2.3f + fLumi2, vLight, o, IWorldTime*0.15f);
			CreateParticle(ThreadIndex, BITMAP_SPARK + 1, vPos, o->Angle, vLight, 11, 1.5f);

			b->TransformByObjectBone(ThreadIndex, vPos, o, 72);

			CreateSprite(BITMAP_LIGHT, vPos, 1.8f + fLumi2, vLight, o);
			CreateSprite(BITMAP_SHINY + 6, vPos, 1.3f + fLumi2, vLight, o);

			return true;
		}
		break;

	case MODEL_MONSTER01 + 151:
	case MODEL_MONSTER01 + 152:
	case MODEL_MONSTER01 + 153:
		{
			return true;
		}
		break;

	case MODEL_MONSTER01 + 205:
		{

		}
		break;

	case MODEL_MONSTER01 + 206:
		{
			if (o->CurrentAction == MONSTER01_ATTACK2 && o->AnimationFrame > 7.4f && o->AnimationFrame < 7.7f)
			{
				CreateInferno(ThreadIndex, o->Position, 5);
			}
			else if (o->CurrentAction == MONSTER01_DIE && o->LifeTime == 100)
			{
				const mu_int32 nBonesCount = 8;
				mu_int32 iBones[nBonesCount] = { 4, 7, 10, 22, 39, 44, 12, 24 };
				for (mu_int32 i = 0; i < nBonesCount; ++i)
				{
					Vector(0.3f, 0.6f, 1.0f, vLight);

					b->TransformByObjectBone(ThreadIndex, vPos, o, iBones[i]);
					for (mu_int32 j = 0; j < 12; ++j)
						CreateEffect(ThreadIndex, MODEL_EFFECT_BROKEN_ICE0 + GetLargeRand(3), vPos, o->Angle, vLight, 0);
				}

				o->LifeTime = 90;

				Vector(1.0f, 1.0f, 1.0f, vLight);

				b->TransformByObjectBone(ThreadIndex, vPos, o, 5);
				CreateEffect(ThreadIndex, MODEL_ICE_GIANT_PART1, vPos, o->Angle, vLight, 0, o, 0, 0);
				b->TransformByObjectBone(ThreadIndex, vPos, o, 3);
				CreateEffect(ThreadIndex, MODEL_ICE_GIANT_PART2, vPos, o->Angle, vLight, 0, o, 0, 0);
				b->TransformByObjectBone(ThreadIndex, vPos, o, 11);
				CreateEffect(ThreadIndex, MODEL_ICE_GIANT_PART3, vPos, o->Angle, vLight, 0, o, 0, 0);
				b->TransformByObjectBone(ThreadIndex, vPos, o, 23);
				CreateEffect(ThreadIndex, MODEL_ICE_GIANT_PART4, vPos, o->Angle, vLight, 0, o, 0, 0);
				b->TransformByObjectBone(ThreadIndex, vPos, o, 39);
				CreateEffect(ThreadIndex, MODEL_ICE_GIANT_PART5, vPos, o->Angle, vLight, 0, o, 0, 0);
				b->TransformByObjectBone(ThreadIndex, vPos, o, 45);
				CreateEffect(ThreadIndex, MODEL_ICE_GIANT_PART6, vPos, o->Angle, vLight, 0, o, 0, 0);
			}
		}
		break;

	case MODEL_MONSTER01 + 208:
		{
			EVector3  Light;
			Vector(1.0f, 1.2f, 2.0f, Light);

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

				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(35)], StartRelative, StartPos, false);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(36)], EndRelative, EndPos, false);
				CreateBlur(c, StartPos, EndPos, Light, 3);

				fAnimationFrame += fSpeedPerFrame;
			}

			mu_int32 iBones[] = { 20, 37, 45, 51 };
			for (mu_int32 i = 0; i < 4; ++i)
			{
				if (GetLargeRand(6) > 0) continue;

				if (GetLargeRand(3) > 0)
				{
					Vector(0.3f, 0.6f, 1.0f, vLight);
				}
				else
				{
					Vector(0.8f, 0.8f, 0.8f, vLight);
				}

				b->TransformByObjectBone(ThreadIndex, vPos, o, iBones[i]);
				CreateParticle(ThreadIndex, BITMAP_SMOKE, vPos, o->Angle, vLight, 50, 1.0f);
				CreateParticle(ThreadIndex, BITMAP_SMOKELINE1 + GetLargeRand(3), vPos, o->Angle, vLight, 2, 0.8f);
			}

			if (o->CurrentAction == MONSTER01_ATTACK2)
			{
				if (2 <= c->AttackTime && c->AttackTime <= 8)
				{
					for (mu_int32 j = 0; j < 3; ++j)
					{
						EVector3 CurPos;
						VectorCopy(o->Position, CurPos);
						CurPos[2] += 120.0f;
						EVector3 TempPos;
						GetNearRandomPos(CurPos, 300, TempPos);
						mu_float fDistance = 1400.0f;
						TempPos[0] += -fDistance * EMath::Sin(o->Angle[2] * Q_PI / 180.0f);
						TempPos[1] += fDistance * EMath::Cos(o->Angle[2] * Q_PI / 180.0f);
						CreateJoint(ThreadIndex, MODEL_SPEARSKILL, TempPos, TempPos, o->Angle, 2, o, 40.0f);
					}
				}
				if (c->AttackTime <= 8)
				{
					EVector3 Position2 = { 0.0f, 0.0f, 0.0f };
					b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(26)], Position2, o->m_vPosSword, true);

					mu_float fDistance = 300.0f;
					o->m_vPosSword[0] += fDistance * EMath::Sin(o->Angle[2] * Q_PI / 180.0f);
					o->m_vPosSword[1] += -fDistance * EMath::Cos(o->Angle[2] * Q_PI / 180.0f);
				}
				if (6 <= c->AttackTime && c->AttackTime <= 12)
				{
					EVector3 Position;
					EVector3 Position2 = { 0.0f, 0.0f, 0.0f };

					b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(26)], Position2, Position, true);

					mu_float fDistance = 100.0f + (mu_float)(c->AttackTime - 8) * 10.0f;
					Position[0] += fDistance * EMath::Sin(o->Angle[2] * Q_PI / 180.0f);
					Position[1] += -fDistance * EMath::Cos(o->Angle[2] * Q_PI / 180.0f);
					EVector3 Light = { 1.0f, 1.0f, 1.0f };
					CreateEffect(ThreadIndex, MODEL_SPEAR, Position, o->Angle, Light, 1, o);
					
					if (c->TargetCharacter != -1)
					{
						CHARACTER *tc = &CharactersClient[c->TargetCharacter];
						OBJECT *to = &tc->Object;
						if (10 <= c->AttackTime && to->Flags.Get(OBJECT::eOF_LIVE) == true)
						{
							to->m_byHurtByOneToOne = 35;
						}
					}
				}
				if (c->AttackTime >= 12)
				{
					c->AttackTime = ThreadInfo.LimitAttackTime;

					CreateEffect(ThreadIndex, MODEL_COMBO, o->Position, o->Angle, o->Light);
				}
			}
			else if (o->CurrentAction == MONSTER01_DIE && o->AnimationFrame < 1.0f)
			{
				mu_int32 iBones[] = { 20, 37, 45, 51 };
				for (mu_int32 i = 0; i < 4; ++i)
				{
					Vector(0.3f, 0.5f, 1.0f, vLight);

					b->TransformByObjectBone(ThreadIndex, vPos, o, iBones[i]);
					for (mu_int32 j = 0; j < 6; ++j)
						CreateEffect(ThreadIndex, MODEL_EFFECT_BROKEN_ICE0 + GetLargeRand(3), vPos, o->Angle, vLight, 0);
				}
			}
		}
		break;

	case MODEL_MONSTER01 + 207:
		{

		}
		break;
	}

	return true;
}

mu_boolean CGM_Raklion::CreateSnow(PARTICLE* o)
{
	if (IsIceCity() == false)
		return false;

	if (GET_WORLD == WD_57ICECITY || GET_WORLD == WD_58ICECITY_BOSS)
		o->Type = BITMAP_LEAF04;
	else
		o->Type = BITMAP_LEAF03;

	o->Scale = (mu_float)(GetLargeRand(10) + 3);
	if (GetLargeRand(10) == 0)
	{
		o->Scale = (mu_float)(GetLargeRand(3) + 10);
	}

	Vector(Hero->Object.Position[0] + (mu_float)(GetLargeRand(1600) - 800),
		Hero->Object.Position[1] + (mu_float)(GetLargeRand(1400) - 500),
		Hero->Object.Position[2] + (mu_float)(GetLargeRand(200) + 200),
		o->Position);
	Vector(-(mu_float)(GetLargeRand(30) + 50), 0.0f, 0.0f, o->Angle);
	EVector3 Velocity;
	Vector(0.0f, 0.0f, -(mu_float)(GetLargeRand(20) + 30), Velocity);
	EBone Matrix;
	AngleMatrix(o->Angle, Matrix);
	VectorRotate(Velocity, Matrix, o->Velocity);

	return true;
}

mu_boolean CGM_Raklion::SetCurrentActionMonster(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o)
{
	switch (c->MonsterIndex)
	{
	case 454:
		{
			if (c->MonsterSkill == 29)
			{
				SetAction(o, MONSTER01_ATTACK2);
				c->MonsterSkill = -1;
			}
			else
			{
				SetAction(o, MONSTER01_ATTACK1);
			}
			return true;
		}
		break;
	case 455:
		{
			if (c->MonsterSkill == 30)
			{
				SetAction(o, MONSTER01_ATTACK2);
				c->MonsterSkill = -1;
			}
			else
			{
				SetAction(o, MONSTER01_ATTACK1);
			}
			return true;
		}
		break;
	case 456:
		{
			if (c->MonsterSkill == 31)
			{
				SetAction(o, MONSTER01_ATTACK2);
				c->MonsterSkill = -1;
			}
			else
			{
				SetAction(o, MONSTER01_ATTACK1);
			}
			return true;
		}
		break;
	case 457:
		{
			if (c->MonsterSkill == 32)
			{
				SetAction(o, MONSTER01_ATTACK2);
				c->MonsterSkill = -1;
			}
			else
			{
				SetAction(o, MONSTER01_ATTACK1);
			}
			return true;
		}
	case 458:
		{
			if (c->MonsterSkill == 33)
			{
				SetAction(o, MONSTER01_ATTACK2);
				c->MonsterSkill = -1;
			}
			else
			{
				SetAction(o, MONSTER01_ATTACK1);
			}
			return true;
		}
		break;
	case 459:
		{
			SetBossMonsterAction(ThreadIndex, c, o);
			return true;
		}
		break;
	case 460:
	case 461:
	case 462:
		{
			return false;
		}
		break;
	case 562:
		{
			if (c->MonsterSkill == ATMON_SKILL_EX_DARKMEMUD_ATTACKSKILL)
			{
				SetAction(o, MONSTER01_ATTACK2);
				c->MonsterSkill = -1;
			}
			else
			{
				SetAction(o, MONSTER01_ATTACK1);
				c->MonsterSkill = -1;
			}
			return true;
		}
		break;
	case 563:
		{
			if (c->MonsterSkill == ATMON_SKILL_EX_DARKGIANT_ATTACKSKILL)
			{
				SetAction(o, MONSTER01_ATTACK2);
				c->MonsterSkill = -1;
			}
			else
			{
				SetAction(o, MONSTER01_ATTACK1);
				c->MonsterSkill = -1;
			}
			return true;
		}
		break;
	case 565:
		{
			if (c->MonsterSkill == ATMON_SKILL_EX_DARKAIONNIGHT_ATTACKSKILL)
			{
				SetAction(o, MONSTER01_ATTACK2);
				c->MonsterSkill = -1;
			}
			else
			{
				SetAction(o, MONSTER01_ATTACK1);
				c->MonsterSkill = -1;
			}
			return true;
		}
		break;
	case 564:
		{
			if (c->MonsterSkill == ATMON_SKILL_EX_DARKCOOLERTIN_ATTACKSKILL)
			{
				SetAction(o, MONSTER01_ATTACK2);
				c->MonsterSkill = -1;
			}
			else
			{
				SetAction(o, MONSTER01_ATTACK1);
				c->MonsterSkill = -1;
			}
			return true;
		}
		break;
	}

	return false;
}

void CGM_Raklion::SetBossMonsterAction(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o)
{
	if (c->MonsterSkill == 37
		|| !(c->MonsterSkill >= 34 && c->MonsterSkill <= 42)
		|| m_bBossHeightMove == true)
	{
		c->Object.Position[2] = RequestTerrainHeight(o->Position[0], o->Position[1]);
		m_bBossHeightMove = false;
	}

	if (c->MonsterSkill == 34)
	{
		SetAction(o, MONSTER01_ATTACK2);
	}
	else if (c->MonsterSkill == 35)
	{
		SetAction(o, MONSTER01_ATTACK1);
	}
	else if (c->MonsterSkill == 36)
	{
		SetAction(o, MONSTER01_ATTACK3);
	}
	else if (c->MonsterSkill == 37)
	{
		SetAction(o, MONSTER01_APEAR);
	}
	else if (c->MonsterSkill == 38)
	{
		SetAction(o, MONSTER01_ATTACK4);
		PlayBackground(SOUND_RAKLION_SERUFAN_WORD4);
	}
	else if (c->MonsterSkill == 39)
	{
		SetAction(o, MONSTER01_ATTACK4);
		PlayBackground(SOUND_RAKLION_SERUFAN_CURE);

		EVector3 vLight, vPos, vAngle;
		Vector(1.0f, 1.0f, 1.0f, vLight);
		for (mu_int32 i = 0; i < 20; ++i)
		{
			Vector(0.0f, 0.0f, GetLargeRand(360), vAngle);
			VectorCopy(o->Position, vPos);
			vPos[0] += GetLargeRand(400) - 200;
			vPos[1] += GetLargeRand(400) - 200;
			vPos[2] -= 100.0f;
			CreateJoint(ThreadIndex, BITMAP_FLARE, vPos, vPos, vAngle, 2, nullptr, 40);
		}

		Vector(1.0f, 0.5f, 0.1f, vLight);
		CreateEffect(ThreadIndex, BITMAP_MAGIC + 1, o->Position, o->Angle, vLight, 13, o, -1, 0, 0, 0, 8.0f);
		CreateEffect(ThreadIndex, BITMAP_MAGIC + 1, o->Position, o->Angle, vLight, 13, o, -1, 0, 0, 0, 8.0f);
	}
	else if (c->MonsterSkill == 40)
	{
		SetAction(o, MONSTER01_ATTACK4);
		PlayBackground(SOUND_RAKLION_SERUFAN_WORD3);
	}
	else if (c->MonsterSkill == 41)
	{
	}
	else if (c->MonsterSkill == 42)
	{
		SetAction(o, MONSTER01_ATTACK4);

		EVector3 vLight;
		Vector(0.3f, 0.2f, 0.1f, vLight);
		CreateEffect(ThreadIndex, MODEL_STORM2, o->Position, o->Angle, vLight, 1, nullptr, -1, 0, 0, 0, 1.6f);
		CreateEffect(ThreadIndex, MODEL_SUMMON, o->Position, o->Angle, vLight, 3);
	}
	else
	{
		SetAction(o, MONSTER01_ATTACK4);
	}
}

mu_boolean CGM_Raklion::AttackEffectMonster(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b)
{
	switch (c->MonsterIndex)
	{
	case 454:
	case 455:
	case 456:
	case 457:
	case 458:
	case 459:
	case 460:
	case 461:
	case 462:
		break;
	}

	return false;
}

mu_boolean CGM_Raklion::PlayMonsterSound(OBJECT* o)
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

	case MODEL_MONSTER01 + 146:
		if (MONSTER01_ATTACK1 == o->CurrentAction || MONSTER01_ATTACK2 == o->CurrentAction)
		{
			PlayBackground(SOUND_RAKLION_GIANT_MAMUD_ATTACK);
		}
		else
			if (o->CurrentAction == MONSTER01_WALK)
			{
				if (GetLargeRand(100) == 0)
				{
					PlayBackground(SOUND_RAKLION_GIANT_MAMUD_MOVE);
				}
			}
			else
				if (o->CurrentAction == MONSTER01_DIE)
				{
					PlayBackground(SOUND_RAKLION_GIANT_MAMUD_DEATH);
				}
		return true;

	case MODEL_MONSTER01 + 147:
		if (o->CurrentAction == MONSTER01_WALK)
		{
			if (GetLargeRand(100) == 0)
			{
				PlayBackground(SOUND_RAKLION_ICEGIANT_MOVE);
			}
		}
		else if (o->CurrentAction == MONSTER01_DIE)
		{
			PlayBackground(SOUND_RAKLION_ICEGIANT_DEATH);
		}
		return true;

	case MODEL_MONSTER01 + 148:
		if (MONSTER01_ATTACK1 == o->CurrentAction || MONSTER01_ATTACK2 == o->CurrentAction)
		{
			PlayBackground(SOUND_RAKLION_COOLERTIN_ATTACK);
		}
		else if (MONSTER01_WALK == o->CurrentAction)
		{
			if (GetLargeRand(20) == 0)
			{
				PlayBackground(SOUND_RAKLION_COOLERTIN_MOVE);
			}
		}
		else if (MONSTER01_DIE == o->CurrentAction)
		{
			{
				PlayBackground(SOUND_HELL_SPIDER_DIE);
			}
		}

		return true;

	case MODEL_MONSTER01 + 149:
		if (o->CurrentAction == MONSTER01_ATTACK1 || o->CurrentAction == MONSTER01_ATTACK2)
		{
			PlayBackground(SOUND_RAKLION_IRON_KNIGHT_ATTACK);
		}
		else if (o->CurrentAction == MONSTER01_STOP1 || o->CurrentAction == MONSTER01_STOP2 || o->CurrentAction == MONSTER01_WALK)
		{
			PlayBackground(SOUND_RAKLION_IRON_KNIGHT_MOVE);
		}
		else if (o->CurrentAction == MONSTER01_DIE)
		{
			PlayBackground(SOUND_BEAM_KNIGHT_ACTION1);
		}
		return true;

	case MODEL_MONSTER01 + 150:
		if (o->CurrentAction == MONSTER01_ATTACK1 || o->CurrentAction == MONSTER01_ATTACK2)
		{
			PlayBackground(SOUND_RAKLION_SERUFAN_ATTACK1);
		}
		else if (o->CurrentAction == MONSTER01_ATTACK3)
		{
			PlayBackground(SOUND_RAKLION_SERUFAN_ATTACK2);
		}
		else if (o->CurrentAction == MONSTER01_DIE)
		{
			PlayBackground(SOUND_RAKLION_SERUFAN_WORD2);
		}
		return true;
	case MODEL_MONSTER01 + 205:
		if (MONSTER01_ATTACK1 == o->CurrentAction || MONSTER01_ATTACK2 == o->CurrentAction)
		{
			PlayBackground(SOUND_RAKLION_GIANT_MAMUD_ATTACK);
		}
		else if (o->CurrentAction == MONSTER01_WALK)
		{
			if (GetLargeRand(100) == 0)
			{
				PlayBackground(SOUND_RAKLION_GIANT_MAMUD_MOVE);
			}
		}
		else if (o->CurrentAction == MONSTER01_DIE)
		{
			PlayBackground(SOUND_RAKLION_GIANT_MAMUD_DEATH);
		}
		return true;
	case MODEL_MONSTER01 + 206:
		if (o->CurrentAction == MONSTER01_WALK)
		{
			if (GetLargeRand(100) == 0)
			{
				PlayBackground(SOUND_RAKLION_ICEGIANT_MOVE);
			}
		}
		else if (o->CurrentAction == MONSTER01_DIE)
		{
			PlayBackground(SOUND_RAKLION_ICEGIANT_DEATH);
		}
		return true;
	case MODEL_MONSTER01 + 208:
		if (o->CurrentAction == MONSTER01_ATTACK1 || o->CurrentAction == MONSTER01_ATTACK2)
		{
			PlayBackground(SOUND_RAKLION_IRON_KNIGHT_ATTACK);
		}
		else if (o->CurrentAction == MONSTER01_STOP1 || o->CurrentAction == MONSTER01_STOP2 || o->CurrentAction == MONSTER01_WALK)
		{
			PlayBackground(SOUND_RAKLION_IRON_KNIGHT_MOVE);
		}
		else if (o->CurrentAction == MONSTER01_DIE)
		{
			PlayBackground(SOUND_BEAM_KNIGHT_ACTION1);
		}
		return true;
	case MODEL_MONSTER01 + 207:
		if (MONSTER01_ATTACK1 == o->CurrentAction || MONSTER01_ATTACK2 == o->CurrentAction)
		{
			PlayBackground(SOUND_RAKLION_COOLERTIN_ATTACK);
		}
		else if (MONSTER01_WALK == o->CurrentAction)
		{
			if (GetLargeRand(20) == 0)
			{
				PlayBackground(SOUND_RAKLION_COOLERTIN_MOVE);
			}
		}
		else if (MONSTER01_DIE == o->CurrentAction)
		{
			{
				PlayBackground(SOUND_HELL_SPIDER_DIE);
			}
		}

		return true;
	}

	return false;
}

void CGM_Raklion::PlayObjectSound(OBJECT* o)
{
	
}

mu_boolean CGM_Raklion::CanGoBossMap()
{
	return m_bCanGoBossMap;
}

void CGM_Raklion::SetCanGoBossMap()
{
	if (m_byState <= RAKLION_STATE_NOTIFY_3)
	{
		m_bCanGoBossMap = true;
	}
	else
	{
		m_bCanGoBossMap = false;
	}
}

void CGM_Raklion::SetState(mu_uint8 byState, mu_uint8 byDetailState)
{
	if (byState == RAKLION_STATE_DETAIL_STATE)
	{
		m_byDetailState = byDetailState;

		if (m_byDetailState == BATTLE_OF_SELUPAN_PATTERN_2 || m_byDetailState == BATTLE_OF_SELUPAN_PATTERN_3
			|| m_byDetailState == BATTLE_OF_SELUPAN_PATTERN_5 || m_byDetailState == BATTLE_OF_SELUPAN_PATTERN_6)
		{
			PlayBackground(SOUND_RAKLION_SERUFAN_RAGE);
		}
	}
	else
	{
		m_byState = byState;

		if (m_byState == RAKLION_STATE_NOTIFY_1 || m_byState == RAKLION_STATE_STANDBY)
		{
			PlayBackground(SOUND_WIND01);
		}
		else if (m_byState == RAKLION_STATE_READY)
		{
			PlayBackground(SOUND_RAKLION_SERUFAN_WORD1);

			m_bMusicBossMap = true;
		}
		else if (m_byState == RAKLION_STATE_END)
		{
			m_bMusicBossMap = false;
		}

		SetCanGoBossMap();

		SetEffect();
	}
}

void CGM_Raklion::SetEffect()
{
	if (m_byState == RAKLION_STATE_NOTIFY_1)
	{
		m_Timer.SetTimer(500);
		m_bVisualEffect = true;
	}
	else if (m_byState == RAKLION_STATE_STANDBY)
	{
		m_Timer.SetTimer(1000);
		m_bVisualEffect = true;
	}
}

void CGM_Raklion::MoveEffect(const mu_uint32 ThreadIndex)
{
	MUAutoCriticalSpinLock lock(m_Critical);

	if (m_bVisualEffect == false)
	{
		return;
	}

	if (m_byState <= RAKLION_STATE_STANDBY)
	{
		m_Timer.UpdateTime();

		if (m_Timer.IsTime() == true)
		{
			m_Timer.ResetTimer();
			m_bVisualEffect = false;

			if (m_byState == RAKLION_STATE_STANDBY)
			{
				EarthQuake.Store(0.0f);
			}
		}
		else
		{
			if (GetLargeRand(2) == 0)
			{
				CreateMapEffect(ThreadIndex);
			}
		}
	}
	else
	{
		m_Timer.ResetTimer();
		m_bVisualEffect = false;
		EarthQuake.Store(0.0f);
	}
}

void CGM_Raklion::CreateMapEffect(const mu_uint32 ThreadIndex)
{
	if (m_byState <= RAKLION_STATE_NOTIFY_1)
	{
		mu_float fScale = 1.0f;
		EVector3 vPos, vLight;
		OBJECT* pObject = &Hero->Object;

		for (mu_int32 i = 0; i < 5; ++i)
		{
			Vector(1.0f, 1.0f, 1.0f, vLight);
			VectorCopy(pObject->Position, vPos);
			vPos[0] += (GetLargeRand(1600) - 800.0f);
			vPos[1] += (GetLargeRand(1600) - 800.0f);
			vPos[2] = 600.0f + GetLargeRand(100);

			fScale = 2.0f + (GetLargeRand(20)) / 5.0f;
			mu_int32 iIndex = MODEL_EFFECT_BROKEN_ICE0;

			CreateEffect(ThreadIndex, iIndex, vPos, pObject->Angle, vLight, 2, nullptr, -1, 0, 0, 0, fScale);

			VectorCopy(pObject->Position, vPos);
			vPos[0] += (GetLargeRand(1600) - 800.0f);
			vPos[1] += (GetLargeRand(1600) - 800.0f);
			vPos[2] = 600.0f + GetLargeRand(100);

			fScale = 0.5f + (GetLargeRand(10)) / 5.0f;
			iIndex = MODEL_EFFECT_BROKEN_ICE2;

			CreateEffect(ThreadIndex, iIndex, vPos, pObject->Angle, vLight, 2, nullptr, -1, 0, 0, 0, fScale);

			VectorCopy(pObject->Position, vPos);
			vPos[0] += (GetLargeRand(1600) - 800.0f);
			vPos[1] += (GetLargeRand(1600) - 800.0f);
			vPos[2] = 600.0f + GetLargeRand(100);

			Vector(1.0f, 0.8f, 0.8f, vLight);
			fScale = 0.005f + (GetLargeRand(10)) / 200.0f;
			CreateEffect(ThreadIndex, MODEL_FALL_STONE_EFFECT, vPos, pObject->Angle, vLight, 2, nullptr, -1, 0, 0, 0, fScale);

			Vector(0.4f, 0.4f, 0.5f, vLight);
			fScale = 0.5f + (GetLargeRand(10)) / 20.0f;
			for (mu_int32 k = 0; k < 3; ++k)
			{
				VectorCopy(pObject->Position, vPos);
				vPos[0] += (GetLargeRand(1000) - 500.0f);
				vPos[1] += (GetLargeRand(1000) - 500.0f);
				vPos[2] = 760.0f + GetLargeRand(150);
				fScale = 0.2f + (GetLargeRand(20)) / 40.0f;
				CreateParticle(ThreadIndex, BITMAP_WATERFALL_3 + (GetLargeRand(2)), vPos, pObject->Angle, vLight, 13, fScale);
			}
		}
	}
	else if (m_byState == RAKLION_STATE_STANDBY)
	{
		EVector3 vPos, vLight;
		mu_float fScale = 1.0f;
		OBJECT* pObject = &Hero->Object;
		Vector(0.3f, 0.5f, 1.0f, vLight);

		EarthQuake.Store((mu_float)(GetLargeRand(2) - 2)*0.5f);

		VectorCopy(pObject->Position, vPos);
		vPos[0] += (GetLargeRand(2000) - 1000.0f);
		vPos[1] += (GetLargeRand(2000) - 1000.0f);
		vPos[2] = 500.0f + GetLargeRand(100);

		fScale = 2.0f + (GetLargeRand(20)) / 5.0f;
		mu_int32 iIndex = MODEL_EFFECT_BROKEN_ICE1;

		CreateEffect(ThreadIndex, iIndex, vPos, pObject->Angle, vLight, 2, nullptr, -1, 0, 0, 0, fScale);

		VectorCopy(pObject->Position, vPos);
		vPos[0] += (GetLargeRand(2000) - 1000.0f);
		vPos[1] += (GetLargeRand(2000) - 1000.0f);
		vPos[2] = 500.0f + GetLargeRand(100);

		fScale = 0.5f + (GetLargeRand(10)) / 5.0f;
		iIndex = MODEL_EFFECT_BROKEN_ICE3;

		CreateEffect(ThreadIndex, iIndex, vPos, pObject->Angle, vLight, 2, nullptr, -1, 0, 0, 0, fScale);

		VectorCopy(pObject->Position, vPos);
		vPos[0] += (GetLargeRand(2000) - 1000.0f);
		vPos[1] += (GetLargeRand(2000) - 1000.0f);
		vPos[2] = 500.0f + GetLargeRand(100);

		Vector(1.0f, 0.8f, 0.8f, vLight);
		fScale = 0.05f + (GetLargeRand(10)) / 20.0f;
		CreateEffect(ThreadIndex, MODEL_FALL_STONE_EFFECT, vPos, pObject->Angle, vLight, 2, nullptr, -1, 0, 0, 0, fScale);
	}
}

void CGM_Raklion::PlayBGM()
{
	if (GET_WORLD == WD_57ICECITY)
	{
		EMusicBackend::Play(MUSICS::Data[MUSIC_RAKLION]);
	}
	else
	{
		EMusicBackend::Stop(MUSICS::Data[MUSIC_RAKLION]);
	}

	if (GET_WORLD == WD_58ICECITY_BOSS && m_bMusicBossMap == true)
	{
		EMusicBackend::Play(MUSICS::Data[MUSIC_RAKLION_BOSS]);
	}
	else
	{
		EMusicBackend::Stop(MUSICS::Data[MUSIC_RAKLION_BOSS]);
	}
}