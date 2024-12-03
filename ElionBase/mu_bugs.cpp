#include "stdafx.h"
#include "mu_objectbase.h"

MUCriticalSectionSpinLock g_ButterflesLock;
std::array<OBJECT, MAX_BUTTERFLES> g_Butterfles;
std::array<mu_uint16, MAX_BUTTERFLES> g_ButterflesFree;
std::array<mu_uint16, MAX_BUTTERFLES + MAX_TASKING_THREADS> g_ButterflesQueue;
mu_uint16 g_ButterflesCurCount = 0;
mu_uint16 g_ButterflesFreeCount = 0;
mu_uint16 g_ButterflesDeleteCount = 0;

void CreateBug(mu_int32 Type, EVector3 Position, OBJECT *Owner, mu_int32 SubType, mu_int32 LinkBone)
{
	if (InChaosCastle() == true)
	{
		return;
	}

	mu_uint32 i = 0;

	if (g_ButterflesFreeCount > 0)
	{
		i = g_ButterflesFree[--g_ButterflesFreeCount];
	}
	else if (g_ButterflesCurCount < MAX_BUTTERFLES)
	{
		i = g_ButterflesCurCount++;
	}
	else
	{
		return;
	}

	OBJECT *o = &g_Butterfles[i];

	o->Flags.Clear();
	o->Flags.Set(OBJECT::eOF_LIVE | OBJECT::eOF_LIGHTENABLE);

	o->Type = Type;
	o->Owner = Owner;
	o->SubType = SubType;
	o->HiddenMesh = -1;
	o->BlendMesh = -1;
	o->BlendMeshLight = 1.0f;
	o->Scale = 0.7f;
	o->LifeTime = 30;
	o->Alpha = 0.0f;
	o->AlphaTarget = 1.0f;
	VectorCopy(Position, o->Position);
	VectorCopy(Owner->Angle, o->Angle);
	Vector(3.0f, 3.0f, 3.0f, o->Light);

	o->BoneTransform = nullptr;

	o->PriorAnimationFrame = 0.0f;
	o->AnimationFrame = 0.0f;
	o->Velocity = 0.5f;
	switch (o->Type)
	{
	case MODEL_FENRIR_BLACK:
	case MODEL_FENRIR_BLUE:
	case MODEL_FENRIR_RED:
	case MODEL_FENRIR_GOLD:
		o->Scale = 0.9f;
		break;
	case MODEL_DARK_HORSE:
		o->Scale = 1.0f;
		break;
	case MODEL_PEGASUS:
	case MODEL_UNICON:
		o->Scale = 0.9f;
		break;
	case MODEL_HELPER:
		o->BlendMesh = 1;
		Vector(Owner->Position[0] + (mu_float)(GetLargeRand(512) - 256),
			Owner->Position[1] + (mu_float)(GetLargeRand(512) - 256),
			Owner->Position[2] + (mu_float)(GetLargeRand(128) + 128), o->Position);
		break;
	case MODEL_HELPER + 1:
		Vector(Owner->Position[0] + (mu_float)(GetLargeRand(128) - 64),
			Owner->Position[1] + (mu_float)(GetLargeRand(128) - 64),
			Owner->Position[2], o->Position);
		o->Position[2] = RequestTerrainHeight(o->Position[0], o->Position[1]) + (mu_float)(GetLargeRand(100));
		break;
	}
}

void DeleteBug(OBJECT *Owner)
{
	g_ButterflesLock.Lock();
	for (mu_uint32 i = 0; i < g_ButterflesCurCount; ++i)
	{
		OBJECT *o = &g_Butterfles[i];
		
		if (o->Flags.GetAny(OBJECT::eOF_LIVE) == true && o->Owner == Owner)
		{
			o->Flags.Clear(OBJECT::eOF_LIVE);
			g_ButterflesDeleteCount++;
		}
	}
	g_ButterflesLock.Unlock();
}

mu_boolean IsBug(ITEM* pItem)
{
	if (pItem == nullptr)
	{
		return false;
	}

	if (pItem->Type == ITEM_HELPER + 0
		|| pItem->Type == ITEM_HELPER + 1
		|| pItem->Type == ITEM_HELPER + 2
		|| pItem->Type == ITEM_HELPER + 3
		|| pItem->Type == ITEM_HELPER + 4
		|| pItem->Type == ITEM_HELPER + 5
		|| pItem->Type == ITEM_HELPER + 37)
	{
		return true;
	}

	return false;
}

void RenderDarkHorseSkill(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b)
{
	EBone Matrix;
	EVector3 Angle, p, Position;

	++o->WeaponLevel;
	if ((MoveSceneFrame % 10) == 0)
	{
		CreateEffect(ThreadIndex, BITMAP_SHOCK_WAVE, o->Position, o->Angle, o->Light);
	}

	if (o->AnimationFrame >= 8.0f && o->AnimationFrame <= 9.5f)
	{
		if ((o->WeaponLevel % 2) == 1)
		{
			Vector(0.0f, 150.0f*(o->WeaponLevel / 2), 0.0f, p);
			Vector(0.0f, 0.0f, (mu_float)(GetLargeRand(360)), Angle);
			for (mu_int32 i = 0; i < 6; ++i)
			{
				Angle[2] += 60.0f;
				AngleMatrix(Angle, Matrix);
				VectorRotate(p, Matrix, Position);
				VectorAdd(o->Position, Position, Position);

				CreateEffect(ThreadIndex, MODEL_GROUND_STONE + GetLargeRand(2), Position, o->Angle, o->Light);
			}
		}
		EarthQuake.Store((GetLargeRand(3) - 3)*0.7f);
	}
	else if (o->WeaponLevel == 19)
	{
		CreateEffect(ThreadIndex, MODEL_SKILL_FURY_STRIKE, o->Position, o->Angle, o->Light, 0, o, -1, 0, 2);
		o->WeaponLevel = -3;
	}
}

void MoveBug(const mu_uint32 ThreadIndex, OBJECT *o, mu_boolean bForceRender)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	if (o->Flags.GetAny(OBJECT::eOF_LIVE) == true)
	{
		if (g_CurrentScene == MAIN_SCENE)
		{
			if (GET_WORLD >= WD_65DOPPLEGANGER1 && GET_WORLD <= WD_68DOPPLEGANGER4)
			{
			}
			else if (o->Owner->Flags.GetAny(OBJECT::eOF_LIVE) == false || o->Owner->Kind != KIND_PLAYER)
			{
				o->Flags.Clear(OBJECT::eOF_LIVE);
				return;
			}
		}

		Alpha(o);

		mu_int32 j;
		mu_float FlyRange = 0.0f;
		EVector3 Light, Position;
		EVector3 TargetPosition;
		MUModel* b = &MODELS::Data[o->Type];

		VectorCopy(o->Owner->Position, TargetPosition);
		switch (o->Type)
		{
		case MODEL_FENRIR_BLACK:
		case MODEL_FENRIR_BLUE:
		case MODEL_FENRIR_RED:
		case MODEL_FENRIR_GOLD:
			if ((TerrainWall[TERRAIN_INDEX_REPEAT((mu_int32)(o->Owner->Position[0] * g_TerrainScaleRev), (mu_int32)(o->Owner->Position[1] * g_TerrainScaleRev))] & TW_SAFEZONE) == TW_SAFEZONE && bForceRender == false)
			{
				o->Alpha = 0.0f;
				break;
			}

			if (o->Owner->Teleport == TELEPORT_BEGIN || o->Owner->Teleport == TELEPORT)
			{
				o->Alpha -= 0.1f;
				if (o->Alpha < 0) o->Alpha = 0.0f;
			}
			else
			{
				o->Alpha = 1.0f;
			}

			VectorCopy(o->Owner->HeadAngle, o->HeadAngle);
			VectorCopy(o->Owner->Position, o->Position);
			VectorCopy(o->Owner->Angle, o->Angle);

			if ((o->Owner->CurrentAction >= PLAYER_FENRIR_ATTACK && o->Owner->CurrentAction <= PLAYER_FENRIR_ATTACK_BOW)
				|| IsAliceRideAction_Fenrir(o->Owner->CurrentAction) == true)
			{
				SetAction(o, FENRIR_ATTACK);
				o->Velocity = 0.4f;
			}
			else if (o->Owner->CurrentAction >= PLAYER_FENRIR_SKILL && o->Owner->CurrentAction <= PLAYER_FENRIR_SKILL_ONE_LEFT)
			{
				SetAction(o, FENRIR_ATTACK_SKILL);
				o->Velocity = 0.4f;
			}
			else if (o->Owner->CurrentAction >= PLAYER_FENRIR_DAMAGE && o->Owner->CurrentAction <= PLAYER_FENRIR_DAMAGE_ONE_LEFT)
			{
				SetAction(o, FENRIR_DAMAGE);
				o->Velocity = 0.4f;
			}
			else if (o->Owner->CurrentAction >= PLAYER_FENRIR_STAND && o->Owner->CurrentAction <= PLAYER_FENRIR_STAND_ONE_LEFT)
			{
				SetAction(o, FENRIR_STAND);
				o->Velocity = 0.4f;
			}
			else if (o->Owner->CurrentAction == PLAYER_DIE1)
			{
				SetAction(o, FENRIR_STAND);
				o->Velocity = 0.4f;
			}
			else
			{
				if (o->Owner->CurrentAction >= PLAYER_FENRIR_WALK && o->Owner->CurrentAction <= PLAYER_FENRIR_WALK_ONE_LEFT)
				{
					SetAction(o, FENRIR_WALK);
					o->Velocity = 1.0f;
				}
				else if (o->Owner->CurrentAction >= PLAYER_FENRIR_RUN && o->Owner->CurrentAction <= PLAYER_FENRIR_RUN_ONE_LEFT_ELF)
				{
					SetAction(o, FENRIR_RUN);
					o->Velocity = 0.6f;
				}

				Vector(1.0f, 1.0f, 1.0f, Light);
				if (GET_WORLD == WD_10HEAVEN)
				{
					mu_boolean   bWave = false;
					EVector3 p = { 120.0f, 0.0f, 32.0f };

					if (o->AnimationFrame > 1.0f && o->AnimationFrame < 1.2f)
					{
						b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(22)], p, Position);
						Position[0] += 40.0f;
						bWave = true;
					}
					else if (o->AnimationFrame > 4.8f && o->AnimationFrame <= 5.0f)
					{
						b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(44)], p, Position);
						Position[0] += 40.0f;
						Position[2] += 700.0f;
						bWave = true;
					}

					if (bWave)
					{
						Vector(Position[0], Position[1], Position[2], p);
						CreateEffect(ThreadIndex, BITMAP_SHOCK_WAVE, p, o->Angle, Light, 1);

						for (mu_int32 i = 0; i < GetLargeRand(5) + 5; ++i)
						{
							Vector(Position[0] + (mu_float)(GetLargeRand(50) - 25),
								Position[1] + (mu_float)(GetLargeRand(50) - 25),
								Position[2] + (mu_float)(GetLargeRand(16) - 8) - 10, p);
							CreateParticle(ThreadIndex, BITMAP_SMOKE, p, o->Angle, Light);
						}
					}
				}
				else if (GetLargeRand(3) == 0 && !InHellas())
				{
					if (o->Owner && !g_isCharacterBuff(o->Owner, eBuff_Cloaking))
					{
						Vector(o->Position[0] + (mu_float)(GetLargeRand(64) - 32),
							o->Position[1] + (mu_float)(GetLargeRand(64) - 32),
							o->Position[2] + (mu_float)(GetLargeRand(32) - 16), Position);
						if (GET_WORLD == WD_2DEVIAS)
							CreateParticle(ThreadIndex, BITMAP_SMOKE, Position, o->Angle, Light);
						else if (GET_WORLD != WD_10HEAVEN)
							if (!g_Direction.m_CKanturu.IsMayaScene())
								CreateParticle(ThreadIndex, BITMAP_SMOKE + 1, Position, o->Angle, Light);
					}
				}
			}
			break;
		case MODEL_DARK_HORSE:
			if ((TerrainWall[TERRAIN_INDEX_REPEAT((mu_int32)(o->Owner->Position[0] * g_TerrainScaleRev), (mu_int32)(o->Owner->Position[1] * g_TerrainScaleRev))] & TW_SAFEZONE) == TW_SAFEZONE
				&& bForceRender == false)
			{
				o->Alpha = 0.0f;
				break;
			}

			VectorCopy(o->Owner->HeadAngle, o->HeadAngle);
			VectorCopy(o->Owner->Position, o->Position);

			VectorCopy(o->Owner->Angle, o->Angle);
			if (o->Owner->CurrentAction == PLAYER_ATTACK_DARKHORSE)
			{
				SetAction(o, 3);
				o->Velocity = 0.34f;
			}
			else if (o->Owner->CurrentAction == PLAYER_RUN_RIDE_HORSE)
			{
				Vector(1.0f, 1.0f, 1.0f, Light);

				SetAction(o, 1);
				if (GET_WORLD == WD_10HEAVEN || g_Direction.m_CKanturu.IsMayaScene())
				{
					mu_boolean   bWave = false;
					EVector3 p = { 120.0f, 0.0f, GetLargeRand(64) - 32.0f };

					if (o->AnimationFrame > 1.0f && o->AnimationFrame < 1.2f)
					{
						b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(19)], p, Position);
						bWave = true;
					}
					else if (o->AnimationFrame > 1.1f && o->AnimationFrame < 1.4f)
					{
						b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(25)], p, Position);
						bWave = true;
					}
					else if (o->AnimationFrame > 1.3f && o->AnimationFrame < 1.6f)
					{
						b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(32)], p, Position);
						bWave = true;
					}
					else if (o->AnimationFrame > 1.5f && o->AnimationFrame <= 1.8f)
					{
						b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(38)], p, Position);
						bWave = true;
					}

					if (bWave)
					{
						Vector(Position[0] + (mu_float)(GetLargeRand(16) - 8),
							Position[1] + (mu_float)(GetLargeRand(16) - 8),
							Position[2], p);
						CreateEffect(ThreadIndex, BITMAP_SHOCK_WAVE, p, o->Angle, Light, 1);

						for (mu_int32 i = 0; i < GetLargeRand(5) + 5; ++i)
						{
							Vector(Position[0] + (mu_float)(GetLargeRand(50) - 25),
								Position[1] + (mu_float)(GetLargeRand(50) - 25),
								Position[2] + (mu_float)(GetLargeRand(16) - 8) - 10, p);
							CreateParticle(ThreadIndex, BITMAP_SMOKE, p, o->Angle, Light);
						}
					}
				}
				else if (GetLargeRand(2) == 0 && !InHellas())
				{
					if (o->Owner && !g_isCharacterBuff(o->Owner, eBuff_Cloaking))
					{
						Vector(o->Position[0] + (mu_float)(GetLargeRand(64) - 32),
							o->Position[1] + (mu_float)(GetLargeRand(64) - 32),
							o->Position[2] + (mu_float)(GetLargeRand(32) - 16), Position);

						if (GET_WORLD == WD_2DEVIAS)
							CreateParticle(ThreadIndex, BITMAP_SMOKE, Position, o->Angle, Light);
						else if (GET_WORLD != WD_10HEAVEN)
							if (!g_Direction.m_CKanturu.IsMayaScene())
								CreateParticle(ThreadIndex, BITMAP_SMOKE + 1, Position, o->Angle, Light);
					}
				}

				o->Velocity = 0.34f;
			}
			else if (o->Owner->CurrentAction >= PLAYER_ATTACK_RIDE_STRIKE && o->Owner->CurrentAction <= PLAYER_ATTACK_RIDE_ATTACK_MAGIC)
			{
				SetAction(o, 2);
				o->Velocity = 0.34f;
			}
			else if (o->Owner->CurrentAction == PLAYER_IDLE1_DARKHORSE)
			{
				SetAction(o, 5);
				o->Velocity = 1.0f;
			}
			else if (o->Owner->CurrentAction == PLAYER_IDLE2_DARKHORSE)
			{
				SetAction(o, 6);
				o->Velocity = 1.0f;
			}
			else
			{
				SetAction(o, 0);
				o->WeaponLevel = 0;
				o->Velocity = 0.3f;
			}

			if (o->CurrentAction != 1)
			{
				if (o->Owner && !g_isCharacterBuff(o->Owner, eBuff_Cloaking))
				{
					if (GetLargeRand(3) == 0)
					{
						EVector3 p = { 50.0f, -4.0f, 0.0f };
						b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(27)], p, Position);

						if (GET_WORLD == WD_7ATLANSE
							|| GET_WORLD == WD_67DOPPLEGANGER3
							)
						{
							CreateParticle(ThreadIndex, BITMAP_BUBBLE, Position, o->Angle, o->Light);
						}
						else
						{
							CreateParticle(ThreadIndex, BITMAP_SMOKE, Position, o->Angle, o->Light);
						}
					}
				}
			}

			if (o->CurrentAction == 3)
			{
				EVector3 Pos1, Pos2, p, p2;

				Vector(60.0f, 0.0f, 0.0f, Pos1);
				Vector(0.0f, 0.0f, 0.0f, Pos2);
				Vector(1.0f, 1.0f, 1.0f, Light);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(9)], Pos1, p);
				b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(9)], Pos2, p2);
				CreateBlur(Hero, p, p2, Light, 0);
			}

			if (o->CurrentAction == 3)
			{
				RenderDarkHorseSkill(ThreadIndex, o, b);
			}

			if (o->Owner->ExtState == 1)
			{
				EVector3 p;
				EVector3 Angle = { 0.0f, 0.0f, 0.0f };
				EBone Matrix;

				Vector(0.0f, 50, 0.0f, p);
				for (mu_int32 i = 0; i < 4; ++i)
				{
					Angle[2] += 90.0f;
					AngleMatrix(Angle, Matrix);
					VectorRotate(p, Matrix, Position);
					VectorAdd(Position, o->Position, Position);

					for (mu_int32 j = 0; j < 3; ++j)
					{
						CreateJoint(ThreadIndex, BITMAP_FLARE, Position, Position, Angle, 0, o, 40, 2);
					}
				}
			}
			else if (o->Owner->ExtState == 2)
			{
			}
			o->Owner->ExtState = 0;
			o->Flags.Set(o->Owner->Flags.GetDirect(OBJECT::eOF_LIVE));
			break;
		case MODEL_PEGASUS:
		case MODEL_UNICON:
			if ((TerrainWall[TERRAIN_INDEX_REPEAT((mu_int32)(o->Owner->Position[0] * g_TerrainScaleRev), (mu_int32)(o->Owner->Position[1] * g_TerrainScaleRev))] & TW_SAFEZONE) == TW_SAFEZONE
				&& bForceRender == false)
			{
				o->Alpha = 0.0f;
				break;
			}

			if (o->Owner->Teleport == TELEPORT_BEGIN || o->Owner->Teleport == TELEPORT)
			{
				o->Alpha -= 0.1f;
				if (o->Alpha < 0) o->Alpha = 0.0f;
			}
			else
			{
				o->Alpha = 1.0f;
			}

			VectorCopy(o->Owner->Position, o->Position);

			if (o->Type == MODEL_PEGASUS)
			{
				if (GET_WORLD == WD_8TARKAN || GET_WORLD == WD_10HEAVEN || g_Direction.m_CKanturu.IsMayaScene())
					o->Position[2] -= 10.0f;
				else if (GET_WORLD != -1)
					o->Position[2] -= 30.0f;
			}
			VectorCopy(o->Owner->Angle, o->Angle);
			if ((o->Owner->CurrentAction >= PLAYER_WALK_MALE && o->Owner->CurrentAction <= PLAYER_RUN_RIDE_WEAPON)
				|| o->Owner->CurrentAction == PLAYER_FLY_RIDE || o->Owner->CurrentAction == PLAYER_FLY_RIDE_WEAPON)
			{
				if (o->Type == MODEL_PEGASUS)
				{
					if (GET_WORLD == WD_8TARKAN || GET_WORLD == WD_10HEAVEN || g_Direction.m_CKanturu.IsMayaScene())
						SetAction(o, 3);
					else
						SetAction(o, 2);
				}
				else
				{
					SetAction(o, 2);
				}

				if (GetLargeRand(2) == 0 && GET_WORLD != WD_10HEAVEN)
				{
					if (!g_Direction.m_CKanturu.IsMayaScene())

						if (o->Owner && !g_isCharacterBuff(o->Owner, eBuff_Cloaking))
						{
							Vector(1.0f, 1.0f, 1.0f, Light);
							Vector(o->Position[0] + (mu_float)(GetLargeRand(64) - 32),
								o->Position[1] + (mu_float)(GetLargeRand(64) - 32),
								o->Position[2] + (mu_float)(GetLargeRand(32) - 16), Position);
							if (GET_WORLD == 2)
								CreateParticle(ThreadIndex, BITMAP_SMOKE, Position, o->Angle, Light);
							else
								CreateParticle(ThreadIndex, BITMAP_SMOKE + 1, Position, o->Angle, Light);
						}

				}
			}
			else if (o->Owner->CurrentAction == PLAYER_SKILL_RIDER || o->Owner->CurrentAction == PLAYER_SKILL_RIDER_FLY)
			{
				if (GET_WORLD == WD_8TARKAN || GET_WORLD == WD_10HEAVEN || g_Direction.m_CKanturu.IsMayaScene())
					SetAction(o, 7);
				else
					SetAction(o, 6);
			}
			else if ((o->Owner->CurrentAction >= PLAYER_ATTACK_FIST && o->Owner->CurrentAction <= PLAYER_ATTACK_RIDE_CROSSBOW)
				|| IsAliceRideAction_UniDino(o->Owner->CurrentAction) == true)
			{
				if (o->Type == MODEL_PEGASUS)
				{
					if (GET_WORLD == WD_8TARKAN || GET_WORLD == WD_10HEAVEN || g_Direction.m_CKanturu.IsMayaScene())
						SetAction(o, 5);
					else
						SetAction(o, 4);
				}
				else
				{
					SetAction(o, 3);
				}
			}
			else
			{
				if (o->Type == MODEL_PEGASUS)
				{
					if (GET_WORLD == WD_8TARKAN || GET_WORLD == WD_10HEAVEN || g_Direction.m_CKanturu.IsMayaScene())
						SetAction(o, 1);
					else
						SetAction(o, 0);
				}
				else
				{
					SetAction(o, 0);
				}
			}
			o->Velocity = 0.34f;
			o->Flags.Set(o->Owner->Flags.GetDirect(OBJECT::eOF_LIVE));
			break;
		case MODEL_BUTTERFLY01:
			FlyRange = 100.0f;
			Vector(0.4f, 0.6f, 1.0f, Light);
			if (GetLargeRand(2) == 0)
				CreateParticle(ThreadIndex, BITMAP_SMOKE, o->Position, o->Angle, Light, 1);
			break;
		case MODEL_HELPER:
			if (o->Owner && !g_isCharacterBuff(o->Owner, eBuff_Cloaking))
			{
				FlyRange = 150.0f;
				EVector3 Position, Light;
				Vector(0.4f, 0.4f, 0.4f, Light);
				for (j = 0; j < 4; ++j)
				{
					Vector((mu_float)(GetLargeRand(16) - 8), (mu_float)(GetLargeRand(16) - 8), (mu_float)(GetLargeRand(16) - 8), Position);
					VectorAdd(Position, o->Position, Position);
					CreateParticle(ThreadIndex, BITMAP_SPARK, Position, o->Angle, Light, 1);
				}
			}
		case MODEL_HELPER + 1:
			FlyRange = 150.0f;
			break;
		}

		b->CurrentAction(ThreadIndex) = o->CurrentAction;
		b->PlayAnimation(ThreadIndex, &o->AnimationFrame, &o->PriorAnimationFrame, &o->PriorAction, o->Velocity);

		if (o->Type == MODEL_HELPER || o->Type == MODEL_HELPER + 1)
		{
			EVector3 Range;
			VectorSubtract(TargetPosition, o->Position, Range);
			mu_float Distance = Range[0] * Range[0] + Range[1] * Range[1];
			if (Distance >= FlyRange*FlyRange)
			{
				mu_float Angle = CreateAngle(o->Position[0], o->Position[1], TargetPosition[0], TargetPosition[1]);
				o->Angle[2] = TurnAngle2(o->Angle[2], Angle, 20.0f);
			}
			EMatrix4 Matrix;
			AngleMatrix(o->Angle, Matrix);
			EVector3 Direction;
			VectorRotate(o->Direction, Matrix, Direction);
			VectorAdd(o->Position, Direction, o->Position);
			o->Position[2] += (mu_float)(GetLargeRand(16) - 8);
			if (GetLargeRand(32) == 0)
			{
				mu_float Speed = 0;
				if (Distance >= FlyRange*FlyRange)
					Speed = -(mu_float)(GetLargeRand(64) + 128)*0.1f;
				else
				{
					Speed = -(mu_float)(GetLargeRand(64) + 16)*0.1f;
					o->Angle[2] = (mu_float)(GetLargeRand(360));
				}
				o->Direction[0] = 0.0f;
				o->Direction[1] = Speed;
				o->Direction[2] = (mu_float)(GetLargeRand(64) - 32)*0.1f;
			}
			if (o->Position[2] < o->Owner->Position[2] + 100.0f) o->Direction[2] += 1.5f;
			if (o->Position[2] > o->Owner->Position[2] + 200.0f) o->Direction[2] -= 1.5f;
		}
	}
}