#include "stdafx.h"
#include "mu_tasking.h"

enum
{
	BOID_FLY = 0,
	BOID_DOWN = 1,
	BOID_GROUND = 2,
	BOID_UP = 3,
};

mu_int32 CreateDragon(const mu_uint32 ThreadIndex, OBJECT* o, mu_int32 index)
{
	if (GET_WORLD != WD_10HEAVEN) return 0;

	if (index < 3)
	{
		o->Flags.Set(OBJECT::eOF_LIVE | OBJECT::eOF_LIGHTENABLE);
		o->Flags.Clear(OBJECT::eOF_ALPHAENABLE);
		
		o->Type = MODEL_MONSTER01 + 31;
		o->Scale = (mu_float)(GetLargeRand(3) + 6)*0.05f;
		o->Alpha = 1.0f;
		o->AlphaTarget = o->Alpha;
		o->Velocity = (mu_float)(GetLargeRand(10) + 10)*0.02f;
		o->Gravity = (mu_float)(GetLargeRand(10) + 10)*0.05f;
		o->SubType = 0;
		o->HiddenMesh = -1;
		o->BlendMesh = -1;

		o->CurrentAction = MONSTER01_DIE + 1;
		SetAction(o, o->CurrentAction);
		Vector(0.0f, 0.0f, (mu_float)(GetLargeRand(360)), o->Angle);
		Vector(Hero->Object.Position[0] + (mu_float)(GetLargeRand(4000) - 2000),
			Hero->Object.Position[1] + (mu_float)(GetLargeRand(4000) - 2000),
			Hero->Object.Position[2] - 600.0f, o->Position);
	}
	else
	{
		o->Flags.Set(OBJECT::eOF_LIVE | OBJECT::eOF_ALPHAENABLE);
		o->Flags.Clear(OBJECT::eOF_LIGHTENABLE);

		o->Type = MODEL_SPEARSKILL;
		o->Velocity = 2.2f;
		o->LifeTime = 240 * 40;
		o->AI = BOID_GROUND;
		Vector(1.0f, 1.0f, 1.0f, o->Light);

		o->Scale = 0.8f;
		o->ShadowScale = 10.0f;
		o->HiddenMesh = -1;
		o->BlendMesh = -1;
		o->Timer = (mu_float)(GetLargeRand(314))*0.01f;
		Vector(Hero->Object.Position[0] + (mu_float)(GetLargeRand(1024) - 512),
			Hero->Object.Position[1] + (mu_float)(GetLargeRand(1024) - 512),
			Hero->Object.Position[2], o->Position);

		o->Position[2] = Hero->Object.Position[2];
		if (InBloodCastle() == true)
		{
			CreateJoint(ThreadIndex, MODEL_SPEARSKILL, o->Position, o->Position, o->Angle, 3, o, 25.0f);
		}
		else
		{
			CreateJoint(ThreadIndex, MODEL_SPEARSKILL, o->Position, o->Position, o->Angle, 1, o, 25.0f);
		}
		Vector(0.0f, 0.0f, 0.0f, o->Angle);
		o->Angle[2] = (mu_float)(GetLargeRand(360));
	}
	return 1;
}

mu_int32 CreateAtlanseFish(OBJECT* o)
{
	if (GET_WORLD != WD_7ATLANSE && GET_WORLD != WD_67DOPPLEGANGER3) return 0;

	if (Hero->Object.Position[1] * 0.01f < 128)
	{
		o->Flags.Set(OBJECT::eOF_ALPHAENABLE);
		o->Flags.Clear(OBJECT::eOF_LIGHTENABLE);

		o->Type = MODEL_FISH01 + 1 + GetLargeRand(2);
		o->Gravity = 15;
		o->Scale = 0.8f;
		o->ShadowScale = 10.0f;
		o->HiddenMesh = -1;
		o->BlendMesh = -1;
		o->Timer = (mu_float)(GetLargeRand(314))*0.01f;
		if (GetLargeRand(100) < 90)
		{
			o->Velocity = 0.3f;
		}
		else
		{
			o->Velocity = 0.25f;
		}

		Vector(0.0f, 0.0f, 0.0f, o->Angle);
		Vector(1.0f, 1.0f, 1.0f, o->Light);
		Vector(Hero->Object.Position[0] + (mu_float)(GetLargeRand(1024) - 512),
			Hero->Object.Position[1] + (mu_float)(GetLargeRand(1024) - 512),
			Hero->Object.Position[2], o->Position);

		o->Position[2] = RequestTerrainHeight(o->Position[0], o->Position[1]) + (mu_float)(GetLargeRand(200) + 150);
	}
	else
	{
		o->Flags.Clear(OBJECT::eOF_LIVE);
	}
	return 1;
}

void MoveBat(OBJECT* o)
{
	o->Position[2] = RequestTerrainHeight(o->Position[0], o->Position[1]);
	o->Position[2] += -EMath::Abs(EMath::Sin(o->Timer))*150.0f + 350.0f;
	o->Timer += 0.2f;
}

void MoveButterFly(OBJECT* o)
{
	if (GetLargeRand(32) == 0)
	{
		o->Angle[2] = (mu_float)(GetLargeRand(360));
		o->Direction[2] = (mu_float)(GetLargeRand(15) - 7)*1.0f;
	}
	o->Direction[2] += (mu_float)(GetLargeRand(15) - 7)*0.2f;
	mu_float Height = RequestTerrainHeight(o->Position[0], o->Position[1]);
	if (o->Position[2] < Height + 50.0f)
	{
		o->Direction[2] *= 0.8f;
		o->Direction[2] += 1.0f;
	}
	if (o->Position[2] > Height + 300.0f)
	{
		o->Direction[2] *= 0.8f;
		o->Direction[2] -= 1.0f;
	}
	o->Position[2] += (mu_float)(GetLargeRand(15) - 7)*0.3f;
}

void MoveBird(OBJECT* o)
{
	if (o->AI == BOID_FLY)
	{
		if (IWorldTime % 8192 < 2048)
		{
			EVector3 Range;
			VectorSubtract(o->Position, Hero->Object.Position, Range);
			mu_float Distance = EMath::Sqrt(Range[0] * Range[0] + Range[1] * Range[1]);
			if (Distance >= 200.0f && Distance <= 400.0f)
			{
				o->AI = BOID_DOWN;
			}
		}
		o->Velocity = 1.0f;
		o->Position[2] += (mu_float)(GetLargeRand(16) - 8);
		if (o->Position[2] < 200.0f) o->Direction[2] = 10.0f;
		else if (o->Position[2] > 600.0f) o->Direction[2] = -10.0f;
	}
	if (o->AI == BOID_DOWN)
	{
		o->Direction[2] = -20.0f;
		mu_float Height = RequestTerrainHeight(o->Position[0], o->Position[1]);
		if (o->Position[2] < Height)
		{
			o->AI = BOID_UP;
			o->Velocity = 1.1f;
			o->Direction[2] = 20.0f;
			o->CurrentAction = 0;
		}
	}
	if (o->AI == BOID_GROUND)
	{
		if (Hero->Object.CurrentAction >= PLAYER_WALK_MALE || GetLargeRand(256) == 0)
		{
			o->AI = BOID_UP;
			o->Velocity = 1.1f;
			o->Direction[2] = 20.0f;
			o->CurrentAction = 0;
		}
	}
	if (o->AI == BOID_UP)
	{
		o->Position[2] += (mu_float)(GetLargeRand(16) - 8);
		o->Velocity -= 0.005f;
		if (o->Velocity <= 1.0f)
		{
			o->AI = BOID_FLY;
		}
	}
}

void MoveHeavenBug(OBJECT* o, mu_int32 index)
{
	mu_int32 iFrame = MoveSceneFrame;

	o->Position[0] += o->Velocity * (mu_float)EMath::Sin(o->Angle[2]);
	o->Position[1] -= o->Velocity * (mu_float)EMath::Cos(o->Angle[2]);
	o->Angle[2] += 0.01f * EMath::Cos((mu_float)(34571 + iFrame + index * 41273)*0.0003f) * EMath::Sin((mu_float)(17732 + iFrame + index * 5161)*0.0003f);

	mu_float dx = o->Position[0] - Hero->Object.Position[0];
	mu_float dy = o->Position[1] - Hero->Object.Position[1];
	mu_float Range = EMath::Sqrt(dx*dx + dy*dy);
	if (Range >= 1500.0f)
		o->Flags.Clear(OBJECT::eOF_LIVE);
	if (GetLargeRand(5120) == 0)
		o->Flags.Clear(OBJECT::eOF_LIVE);

	if (InBloodCastle())
	{
		if (o->LifeTime <= 0)
		{
			o->Flags.Clear(OBJECT::eOF_LIVE);
		}
	}
}

void MoveEagle(OBJECT* o)
{
	if (o->SubType == 0 && GetLargeRand(120) == 0)
	{
		o->SubType = 1;
		o->AnimationFrame = 0;
	}
	else if (o->SubType == 1 && o->AnimationFrame == 24)
	{
		o->SubType = 2;
	}
	else if (o->SubType == 2 && o->AnimationFrame == 0)
	{
		o->SubType = 0;
	}
	else if (o->SubType == 0)
	{
		o->AnimationFrame = 0;
	}

	mu_float fSeedAngle = FWorldTime*0.001f;
	mu_float fFlyRange = o->Gravity;
	mu_float fAngle = 0;
	if (o->AI == BOID_FLY)
	{
		o->HeadAngle[0] = EMath::Cos(fSeedAngle) * fFlyRange;
		o->HeadAngle[1] = EMath::Sin(fSeedAngle) * fFlyRange;
		fAngle = CreateAngle(o->Position[0], o->Position[1], o->Position[0] + o->HeadAngle[0], o->Position[1] + o->HeadAngle[1]);

		if (o->HeadAngle[2] == 0 && o->HeadAngle[0] > o->HeadAngle[1])
		{
			o->HeadAngle[2] = 1;
		}
		else if (o->HeadAngle[2] == 1 && o->HeadAngle[0] < o->HeadAngle[1])
		{
			o->HeadAngle[2] = 2;
		}
		else if (o->HeadAngle[2] == 2 && o->HeadAngle[0] > o->HeadAngle[1])
		{
			o->AI = BOID_GROUND;
			o->HeadAngle[2] = 0;

			o->HeadAngle[0] = EMath::Sin(fSeedAngle) * fFlyRange;
			o->HeadAngle[1] = EMath::Cos(fSeedAngle) * fFlyRange;
			fAngle = CreateAngle(o->Position[0], o->Position[1], o->Position[0] + o->HeadAngle[0], o->Position[1] + o->HeadAngle[1]);
		}
	}
	else if (o->AI == BOID_GROUND)
	{
		o->HeadAngle[0] = EMath::Sin(fSeedAngle) * fFlyRange;
		o->HeadAngle[1] = EMath::Cos(fSeedAngle) * fFlyRange;
		fAngle = CreateAngle(o->Position[0], o->Position[1], o->Position[0] + o->HeadAngle[0], o->Position[1] + o->HeadAngle[1]);

		if (o->HeadAngle[2] == 0 && o->HeadAngle[0] < o->HeadAngle[1])
		{
			o->HeadAngle[2] = 1;
		}
		else if (o->HeadAngle[2] == 1 && o->HeadAngle[0] > o->HeadAngle[1])
		{
			o->HeadAngle[2] = 2;
		}
		else if (o->HeadAngle[2] == 2 && o->HeadAngle[0] < o->HeadAngle[1])
		{
			o->AI = BOID_FLY;
			o->HeadAngle[2] = 0;

			o->HeadAngle[0] = EMath::Cos(fSeedAngle) * fFlyRange;
			o->HeadAngle[1] = EMath::Sin(fSeedAngle) * fFlyRange;
			fAngle = CreateAngle(o->Position[0], o->Position[1], o->Position[0] + o->HeadAngle[0], o->Position[1] + o->HeadAngle[1]);
		}
	}

	o->Position[0] += o->HeadAngle[0];
	o->Position[1] += o->HeadAngle[1];
	o->Position[2] += EMath::Sin(FWorldTime*0.0005f) * 1.0f;
	o->Angle[1] += EMath::Sin(FWorldTime*0.001f) * 0.4f;
	o->Angle[2] = fAngle + 270;
}

void MoveTornado(OBJECT* o)
{
	o->Scale = 1.0f;
	if (GetLargeRand(500) == 0)
	{
		o->HeadAngle[0] = (GetLargeRand(314)) / 100.0f;
	}
	o->Position[0] += EMath::Sin(o->HeadAngle[0])*2.0f;
	o->Position[1] += EMath::Cos(o->HeadAngle[0])*2.0f;
	o->Angle[2] = 0;
	if (o->BlendMeshLight < 1.0f) o->BlendMeshLight += 0.1f;
}

template<mu_size N>
void MoveBoid(OBJECT *o, mu_int32 i, std::array<OBJECT, N> &Boids, mu_int32 MAX)
{
	mu_int32 NumBirds = 0;
	mu_float TargetX = 0.0f;
	mu_float TargetY = 0.0f;

	for (mu_int32 j = 0; j < MAX; ++j)
	{
		OBJECT *t = &Boids[j];
		if (t->Flags.Get(OBJECT::eOF_LIVE) == true &&
			j != i)
		{
			EVector3 Range;
			VectorSubtract(o->Position, t->Position, Range);
			mu_float Distance = EMath::Sqrt(Range[0] * Range[0] + Range[1] * Range[1]);
			if (Distance < 400.0f)
			{
				mu_float xdist = t->Direction[0] - t->Position[0];
				mu_float ydist = t->Direction[1] - t->Position[1];
				if (Distance < 80.0f)
				{
					xdist -= t->Direction[0] - o->Position[0];
					ydist -= t->Direction[1] - o->Position[1];
				}
				else
				{
					xdist += t->Direction[0] - o->Position[0];
					ydist += t->Direction[1] - o->Position[1];
				}
				mu_float pdist = EMath::Sqrt(xdist*xdist + ydist*ydist);
				TargetX += xdist / pdist;
				TargetY += ydist / pdist;
				++NumBirds;
			}
		}
	}

	if (NumBirds > 0)
	{
		TargetX = o->Position[0] + TargetX / NumBirds;
		TargetY = o->Position[1] + TargetY / NumBirds;

		o->Angle[2] = (mu_float)TurnAngle((mu_int32)o->Angle[2], CalcAngle(o->Position[0], o->Position[1], TargetX, TargetY), (mu_int32)o->Gravity);
	}
}

void MoveBoidGroup(OBJECT* o, mu_int32 index)
{
	if (o->AI != BOID_GROUND)
	{
		if (o->Type != MODEL_BUTTERFLY01 || GetLargeRand(4) == 0)
			MoveBoid(o, index, Boids, MaxBoids);

		EVector3 p, Direction;
		if (GET_WORLD == WD_7ATLANSE || GET_WORLD == WD_67DOPPLEGANGER3)
		{
			if (o->Timer < 5.0f)
			{
				if (index < 35)
				{
					Vector(o->Velocity*(mu_float)(GetLargeRand(16) + 8), 0.0f, o->Direction[2], Direction);
				}
				else
				{
					Vector(o->Velocity*(mu_float)(GetLargeRand(16) + 16), 0.0f, o->Direction[2], Direction);
				}
				o->Gravity = 15;
			}
			else
			{
				Vector(o->Velocity*(mu_float)(GetLargeRand(32) + 32), 0.0f, o->Direction[2], Direction);
				o->Gravity = 5;
			}
			o->Timer += 0.1f;
			if (o->Timer >= 10)
			{
				o->Timer = 0.0f;
			}
		}
		else
		{
			Vector(o->Velocity*25.0f, 0.0f, o->Direction[2], Direction);
		}

		EMatrix4 matrix;
		AngleMatrix(o->Angle, matrix);
		VectorRotate(Direction, matrix, p);
		VectorAdd(o->Position, p, o->Position);
		o->Direction[0] = o->Position[0] + 3.0f*p[0];
		o->Direction[1] = o->Position[1] + 3.0f*p[1];

		mu_float dx = o->Position[0] - Hero->Object.Position[0];
		mu_float dy = o->Position[1] - Hero->Object.Position[1];
		mu_float Range = EMath::Sqrt(dx*dx + dy*dy);
		mu_float FlyDistance = 1500.0f;
		if (o->Type == MODEL_MONSTER01 + 31)
		{
			FlyDistance = 4000.0f;
		}
		else if (o->Type == MODEL_MONSTER01 + 33)
		{
			FlyDistance = 3000.0f;
		}
		else if (GET_WORLD == WD_51HOME_6TH_CHAR)
		{
		}
		else
		{
			if (GetLargeRand(512) == 0)
				o->Flags.Clear(OBJECT::eOF_LIVE);
		}

		if (Range >= FlyDistance)
			o->Flags.Clear(OBJECT::eOF_LIVE);
	}
}

void MUTasking::MoveBoidsProcess(const mu_uint32 ThreadIndex)
{
	mu_int32 Index = TERRAIN_INDEX_REPEAT((mu_int32)(Hero->Object.Position[0] * g_TerrainScaleRev), (mu_int32)(Hero->Object.Position[1] * g_TerrainScaleRev));

	if (GET_WORLD == WD_0LORENCIA
		|| GET_WORLD == WD_1DUNGEON
		|| GET_WORLD == WD_3NORIA
		|| GET_WORLD == WD_4LOSTTOWER
		|| GET_WORLD == WD_10HEAVEN
		|| GET_WORLD == WD_7ATLANSE
		|| GET_WORLD == WD_67DOPPLEGANGER3
		|| InBloodCastle()
		|| InHellas()
		|| GET_WORLD == WD_51HOME_6TH_CHAR)
	{
	}
	else if (EventMask == 0)
	{
		return;
	}

	if (EventMask != 0)
	{
		OBJECT* o = &Hero->Object;
		EVector3 Position, Angle, Light;

		if (GetLargeRand(40) == 0)
		{
			Vector(Hero->Object.Position[0] + (mu_float)(GetLargeRand(600) - 200),
				Hero->Object.Position[1] + (mu_float)(GetLargeRand(400) + 200),
				Hero->Object.Position[2] + 300.0f, Position);
			Vector(0.0f, 0.0f, 0.0f, Angle);
			Vector(1.0f, 1.0f, 1.0f, Light);
			CreateEffect(ThreadIndex, MODEL_FIRE, Position, Angle, Light, 3);
			PlayBackground(SOUND_METEORITE01);
		}

		Vector(-0.3f, -0.3f, -0.2f, Light);
		AddTerrainLight(o->Position[0], o->Position[1], Light, 16, PrimaryTerrainLight);
	}

	for (mu_uint32 i = 0; i < MaxBoids; ++i)
	{
		mu_boolean bCreate = true;

		switch (GET_WORLD)
		{
		default:
			if (InHellas() == true)
			{
				if ((GetLargeRand(10)))
				{
					bCreate = false;
				}
				break;
			}
			break;
		}

		OBJECT* o = &Boids[i];
		if (!o->Flags.Get(OBJECT::eOF_LIVE) && bCreate)
		{
			if (EventMask != 0)
			{
				if (GetLargeRand(300) == 0)
				{
					o->Initialize();

					o->Flags.Set(OBJECT::eOF_LIVE | OBJECT::eOF_LIGHTENABLE);

					o->Type = MODEL_MONSTER01 + 31;
					o->Scale = (mu_float)(GetLargeRand(3) + 6)*0.1f;
					o->Alpha = 1.0f;
					o->AlphaTarget = 1.0f;
					o->Velocity = 0.5f;

					o->SubType = 0;
					o->HiddenMesh = -1;
					o->BlendMesh = -1;
					o->LifeTime = 128 + GetLargeRand(128);
					o->Timer = (mu_float)(GetLargeRand(10))*0.1f;
					Vector(0.0f, 0.0f, -90.0f, o->Angle);

					if (IsEventMark(EVENT_MASK::GOLDEN_INVASION) == true)
					{
						o->SubType = 1;
					}

					Vector(Hero->Object.Position[0] + (mu_float)(GetLargeRand(600) - 100),
						Hero->Object.Position[1] + (mu_float)(GetLargeRand(400) + 200),
						Hero->Object.Position[2] + 300.0f, o->Position);

					o->BoneTransform = nullptr;
				}
			}
			else if (GET_WORLD == WD_0LORENCIA
				|| GET_WORLD == WD_1DUNGEON
				|| GET_WORLD == WD_3NORIA
				|| GET_WORLD == WD_4LOSTTOWER
				|| GET_WORLD == WD_10HEAVEN
				|| ((GET_WORLD == WD_7ATLANSE ||
					GET_WORLD == WD_67DOPPLEGANGER3) && (TerrainWall[Index] == 0 || TerrainWall[Index] == TW_CHARACTER))
				|| InBloodCastle()
				|| InHellas()
				|| (GET_WORLD == WD_51HOME_6TH_CHAR && i < 1 && GetLargeRand(500) == 0 && Hero->SafeZone != true)
				)
			{
				mu_int32 iCreateBoid = 0;

				o->Initialize();

				o->Flags.Set(OBJECT::eOF_LIVE | OBJECT::eOF_LIGHTENABLE);
				o->Velocity = 1.0f;
				o->LifeTime = 0;
				o->SubType = 0;
				Vector(0.5f, 0.5f, 0.5f, o->Light);
				o->Alpha = 0.0f;
				o->AlphaTarget = 1.0f;
				o->Gravity = 13;
				o->AI = 0;
				o->CurrentAction = 0;

				if (GET_WORLD == WD_0LORENCIA)
					o->Type = MODEL_BIRD01;
				else if (GET_WORLD == WD_1DUNGEON || GET_WORLD == WD_4LOSTTOWER)
					o->Type = MODEL_BAT01;
				else if (GET_WORLD == WD_3NORIA)
				{
					o->Type = MODEL_BUTTERFLY01;
					o->Velocity = 0.3f;
					o->Flags.Clear(OBJECT::eOF_LIGHTENABLE);
					Vector(1.0f, 1.0f, 1.0f, o->Light);
				}
				else if (InBloodCastle() == true)
				{
					o->Type = MODEL_CROW;
				}
				else if (GET_WORLD == WD_51HOME_6TH_CHAR)
				{
					o->Type = MODEL_MAP_TORNADO;
					o->Velocity = 0.0f;
					o->BlendMeshLight = 0.0f;
					o->HeadAngle[0] = (GetLargeRand(314)) / 100.0f;
					o->Position[1] += 5.0f;
				}
				else
				{
					if (iCreateBoid == 0)
					{
						iCreateBoid = CreateDragon(ThreadIndex, o, i);
					}
					if (iCreateBoid == 0)
					{
						iCreateBoid = CreateAtlanseFish(o);
					}
					if (iCreateBoid == 0)
					{
						iCreateBoid = CreateBigMon(o);
					}
				}
				if (iCreateBoid != 1)
				{
					o->Flags.Set(OBJECT::eOF_ALPHAENABLE);

					o->Scale = 0.8f;
					o->ShadowScale = 10.0f;
					o->HiddenMesh = -1;
					o->BlendMesh = -1;
					o->Timer = (mu_float)(GetLargeRand(314))*0.01f;
					Vector(Hero->Object.Position[0] + (mu_float)(GetLargeRand(1024) - 512),
						Hero->Object.Position[1] + (mu_float)(GetLargeRand(1024) - 512),
						Hero->Object.Position[2], o->Position);
					o->Position[2] = RequestTerrainHeight(o->Position[0], o->Position[1]) + (mu_float)(GetLargeRand(200) + 150);
					Vector(0.0f, 0.0f, 0.0f, o->Angle);
				}

				o->BoneTransform = nullptr;
			}
		}

		if (o->Flags.Get(OBJECT::eOF_LIVE))
		{
			MUModel* b = &MODELS::Data[o->Type];
			mu_float PlaySpeed = 1.0f;

			if (o->Type == MODEL_MONSTER01 + 31 || o->Type == MODEL_MONSTER01 + 33)
			{
				PlaySpeed = 0.5f;
			}

			if (EventMask != 0 && o->Type == MODEL_MONSTER01 + 31)
			{
				SetAction(o, MONSTER01_APEAR);
				b->CurrentAction(ThreadIndex) = o->CurrentAction;
				b->PlayAnimation(ThreadIndex, &o->AnimationFrame, &o->PriorAnimationFrame, &o->PriorAction, PlaySpeed);
				EVector3 Position, Direction;
				Vector(o->Scale*40.0f, 0.0f, 0.0f, Position);
				EMatrix4 matrix;
				AngleMatrix(o->Angle, matrix);
				VectorRotate(Position, matrix, Direction);
				VectorAdd(o->Position, Direction, o->Position);
				o->Position[2] = RequestTerrainHeight(o->Position[0], o->Position[1]) + 300.0f;
				o->Position[2] += -EMath::Abs(EMath::Sin(o->Timer))*100.0f + 100.0f;
				o->Timer += o->Scale*0.05f;
				--o->LifeTime;
				if (o->LifeTime <= 0)
					o->Flags.Clear(OBJECT::eOF_LIVE);

				if (GetLargeRand(128) == 0)
					PlayObject(SOUND_BULL_ATTACK1, o);
			}
			else
			{
				b->CurrentAction(ThreadIndex) = o->CurrentAction;

				if (GET_WORLD == WD_51HOME_6TH_CHAR)
				{
					PlaySpeed = b->_AnimationHeader[o->CurrentAction].Speed;
				}

				b->PlayAnimation(ThreadIndex, &o->AnimationFrame, &o->PriorAnimationFrame, &o->PriorAction, PlaySpeed);

				switch (o->Type)
				{
				case MODEL_BAT01:
					MoveBat(o);
					break;

				case MODEL_BUTTERFLY01:
					MoveButterFly(o);
					break;

				case MODEL_BIRD01:
				case MODEL_CROW:
					MoveBird(o);
					break;

				case MODEL_SPEARSKILL:
					MoveHeavenBug(o, i);
					break;

				case MODEL_MONSTER01 + 33:
					MoveBigMon(o);
					break;

				case MODEL_EAGLE:
					MoveEagle(o);
					break;
				case MODEL_MAP_TORNADO:
					MoveTornado(o);
					break;
				}

				MoveBoidGroup(o, i);

				if (o->LifeTime <= 0 && (GET_WORLD == WD_7ATLANSE || GET_WORLD == WD_67DOPPLEGANGER3) && TerrainWall[Index] == TW_SAFEZONE)
				{
					o->Angle[2] += 180.0f;
					if (o->Angle[2] >= 360.0f) o->Angle[2] -= 360.0f;
					o->LifeTime = 10;
					++o->SubType;
				}

				if (o->Type == MODEL_EAGLE || o->Type == MODEL_MAP_TORNADO)
				{
				}
				else if (o->SubType >= 2) o->Flags.Clear(OBJECT::eOF_LIVE);
				--o->LifeTime;

				mu_float dx = o->Position[0] - Hero->Object.Position[0];
				mu_float dy = o->Position[1] - Hero->Object.Position[1];
				mu_float Range = EMath::Sqrt(dx*dx + dy * dy);
				if (Range < 600)
				{
					if (o->Type == MODEL_BIRD01)
					{
						if (GetLargeRand(512) == 0)
							PlayObject(SOUND_BIRD01, o);
						if (GetLargeRand(512) == 0)
							PlayObject(SOUND_BIRD02, o);
					}
					else if (o->Type == MODEL_BAT01)
					{
						if (GetLargeRand(256) == 0)
							PlayObject(SOUND_BAT01, o);
					}
					else if (o->Type == MODEL_CROW)
					{
						if (TerrainWall[Index] == TW_SAFEZONE)
						{
							if (GetLargeRand(128) == 0)
								PlayObject(SOUND_CROW, o);
						}
					}
				}
			}

			Alpha(o);
		}
	}
}

void MUTasking::MoveFishsProcess(const mu_uint32 ThreadIndex)
{
	for (mu_int32 i = 0; i < MaxFishs; ++i)
	{
		OBJECT *o = &Fishs[i];

		if (!o->Flags.Get(OBJECT::eOF_LIVE))
		{
			Vector(Hero->Object.Position[0] + (mu_float)(GetLargeRand(1024) - 512),
				Hero->Object.Position[1] + (mu_float)(GetLargeRand(1024) - 512),
				Hero->Object.Position[2], o->Position);
			mu_int32 Index = TERRAIN_INDEX_REPEAT((mu_int32)(o->Position[0] * g_TerrainScaleRev), (mu_int32)(o->Position[1] * g_TerrainScaleRev));

			if ((GET_WORLD == WD_0LORENCIA && TerrainMappingLayer1[Index] == 5) ||
				(GET_WORLD == WD_1DUNGEON && TerrainWall[Index] < TW_NOGROUND) ||
				(GET_WORLD == WD_6STADIUM && TerrainWall[Index] < TW_NOGROUND) ||
				((GET_WORLD == WD_7ATLANSE || GET_WORLD == WD_67DOPPLEGANGER3) && (TerrainWall[Index] == 0 || TerrainWall[Index] == TW_CHARACTER)) ||
				(InHellas() && (TerrainWall[Index] == 0 || TerrainWall[Index] == TW_CHARACTER)) ||
				(M33Aida::IsInAida() && (TerrainWall[Index] == 0 || TerrainWall[Index] == TW_CHARACTER)) ||
				((GET_WORLD == WD_8TARKAN || GET_WORLD == WD_34CRYWOLF_1ST) && (TerrainWall[Index] == 0 || TerrainWall[Index] == TW_CHARACTER))
				)
			{
				o->Flags.Set(OBJECT::eOF_LIVE | OBJECT::eOF_LIGHTENABLE | OBJECT::eOF_ALPHAENABLE | OBJECT::eOF_BILLBOARD);
				o->Alpha = 0.0f;
				o->AlphaTarget = 1.0f;
				o->BlendMesh = -1;
				Vector(0.5f, 0.5f, 0.5f, o->Light);
				Vector(0.0f, 0.0f, 0.0f, o->Angle);
				o->SubType = 0;
				o->HiddenMesh = -1;
				o->LifeTime = GetLargeRand(128);
				o->Scale = (mu_float)(GetLargeRand(4) + 4)*0.1f;
				o->Gravity = 13;

				switch (GET_WORLD)
				{
				case WD_0LORENCIA:
					o->Type = MODEL_FISH01;
					o->AlphaTarget = (mu_float)(GetLargeRand(2) + 2)*0.1f;
					o->Velocity = 0.6f / o->Scale;
					break;
				case WD_1DUNGEON:
					o->Type = MODEL_RAT01;
					o->Velocity = 0.6f / o->Scale;
					break;
				case WD_6STADIUM:
					o->Type = MODEL_BUG01;
					o->Velocity = 0.6f / o->Scale;
					break;
				case WD_33AIDA:
				case WD_8TARKAN:
					o->Type = MODEL_BUG01 + 1;
					o->Scale = (mu_float)(GetLargeRand(4) + 8)*0.1f;
					o->Velocity = 2.5f / o->Scale;
					o->Gravity = 9;
					o->LifeTime = 100;
					VectorCopy(o->Position, o->EyeLeft);
					CreateJoint(ThreadIndex, BITMAP_JOINT_ENERGY, o->Position, o->Position, o->Angle, 4, o, 30.0f);
					break;
				case WD_34CRYWOLF_1ST:
					if (Hero->SafeZone != true)
					{
						o->Type = MODEL_SCOLPION;
						o->Scale = (mu_float)(GetLargeRand(4) + 8)*0.1f;
						o->Velocity = (((GetLargeRand(8) + 1))*0.1f) / o->Scale;
						VectorCopy(o->Position, o->EyeLeft);
						o->Gravity = 1;
						o->LifeTime = 100;
						CreateJoint(ThreadIndex, BITMAP_SCOLPION_TAIL, o->Position, o->Position, o->Angle, 0, o, 30.0f);
					}
					else
						o->Flags.Clear(OBJECT::eOF_LIVE);
					break;
				case WD_7ATLANSE:
				case WD_67DOPPLEGANGER3:
					o->Scale = (mu_float)(GetLargeRand(2) + 8)*0.1f;
					if (Hero->Object.Position[1] * 0.01f < 128)
					{
						o->Type = MODEL_FISH01 + 3 + GetLargeRand(4);
						o->Velocity = 1.0f / o->Scale;
						if (GetLargeRand(2) == 0)
							o->Gravity = 2;
						else
							o->Gravity = 3;
					}
					else
					{
						o->Type = MODEL_FISH01 + 7 + GetLargeRand(2);
						if (o->Type == MODEL_FISH01 + 7 || o->Type == MODEL_FISH01 + 8)
						{
							o->BlendMesh = 0;
							o->BlendMeshLight = 1.0f;
						}
						o->Velocity = 0.5f / o->Scale;
						if (GetLargeRand(2) == 0)
							o->Gravity = 1;
						else
							o->Gravity = 2;
					}
					o->Timer = (mu_float)(GetLargeRand(32))*0.1f;
					o->Position[2] = RequestTerrainHeight(o->Position[0], o->Position[1]) + (mu_float)(GetLargeRand(150) + 50);
					break;
				}

				if (InHellas())
				{
					o->Type = -1;
					o->Scale = (mu_float)(GetLargeRand(4) + 8)*0.1f;
					o->Velocity = 2.5f / o->Scale;
					o->Gravity = 9;
					o->LifeTime = 70;
					CreateJoint(ThreadIndex, BITMAP_FLARE + 1, o->Position, o->Position, o->Angle, 8, o, 50.0f);
				}

				o->BoneTransform = nullptr;
			}
		}

		if (o->Flags.Get(OBJECT::eOF_LIVE))
		{
			if (o->Type != -1)
			{
				MUModel *b = &MODELS::Data[o->Type];

				b->PlayAnimation(ThreadIndex, &o->AnimationFrame, &o->PriorAnimationFrame, &o->PriorAction, o->Velocity*0.5f);
			}

			if (o->Type == MODEL_FISH01 + 7 || o->Type == MODEL_FISH01 + 8)
			{
				o->BlendMeshLight = EMath::Sin(o->Timer)*0.4f + 0.5f;
				o->Timer += 0.1f;
			}

			if ((o->Type >= MODEL_FISH01 && o->Type <= MODEL_FISH01 + 10) ||
				o->LifeTime > 0)
			{
				if (o->Type == MODEL_BUG01
					|| o->Type == MODEL_SCOLPION
					)
					SetAction(o, 1);
				else
					SetAction(o, 0);

				MoveBoid(o, i, Fishs, MAX_FISHS);
				EMatrix4 matrix;
				AngleMatrix(o->Angle, matrix);
				EVector3 Position, Direction;
				Vector(o->Velocity*(mu_float)(GetLargeRand(4) + 6), 0.0f, 0.0f, Position);
				VectorRotate(Position, matrix, Direction);
				VectorAdd(o->Position, Direction, o->Position);
				if (GET_WORLD != 7 ||
					InHellas() == false ||
					GET_WORLD != WD_67DOPPLEGANGER3)
				{
					o->Position[2] = RequestTerrainHeight(o->Position[0], o->Position[1]);
				}
				o->Direction[0] = o->Position[0] + 3.0f*Direction[0];
				o->Direction[1] = o->Position[1] + 3.0f*Direction[1];
				mu_int32 Index = TERRAIN_INDEX_REPEAT((mu_int32)(o->Position[0] * g_TerrainScaleRev), (mu_int32)(o->Position[1] * g_TerrainScaleRev));


				if (GET_WORLD == WD_8TARKAN || InHellas() || M33Aida::IsInAida())
				{
					if (TerrainWall[Index] == 1 || TerrainWall[Index] >= TW_NOGROUND)
					{
						o->Angle[2] += 180.0f;
						if (o->Angle[2] >= 360.0f) o->Angle[2] -= 360.0f;
						++o->SubType;
					}
					else if (o->SubType > 0)
					{
						--o->SubType;
					}

					if (InHellas() && o->LifeTime <= 1)
					{
						o->Flags.Clear(OBJECT::eOF_LIVE);
					}
				}
				else if ((o->Type == MODEL_FISH01 && TerrainMappingLayer1[Index] != 5) ||
					(o->Type == MODEL_RAT01 && TerrainWall[Index] >= TW_NOGROUND) ||
					((GET_WORLD == WD_7ATLANSE || GET_WORLD == WD_67DOPPLEGANGER3) && (TerrainWall[Index] == 1 || TerrainWall[Index] >= TW_NOGROUND))
					)
				{
					o->Angle[2] += 180.0f;
					if (o->Angle[2] >= 360.0f) o->Angle[2] -= 360.0f;
					++o->SubType;
				}
				else
				{
					if (o->SubType > 0) --o->SubType;
				}

				if (o->SubType >= 2)
				{
					o->Flags.Clear(OBJECT::eOF_LIVE);
				}

				if (o->Type == MODEL_BUG01 + 1
					|| o->Type == MODEL_SCOLPION
					)
				{
					VectorCopy(o->Position, o->EyeLeft);
				}

				mu_float dx = o->Position[0] - Hero->Object.Position[0];
				mu_float dy = o->Position[1] - Hero->Object.Position[1];
				mu_float Range = EMath::Sqrt(dx*dx + dy * dy);
				if (Range >= 1500.0f)
					o->Flags.Clear(OBJECT::eOF_LIVE);

				if (Range < 600.0f)
				{
					if (o->Type == MODEL_RAT01 && GetLargeRand(256) == 0)
						PlayObject(SOUND_RAT01, o);
				}
			}
			else
			{
				SetAction(o, 0);
			}

			--o->LifeTime;
			if (o->LifeTime <= 0)
			{
				if (o->Type == MODEL_BUG01)
				{
				}
				else
				{
					if (GetLargeRand(64) == 0)
						o->LifeTime = GetLargeRand(128);
				}
			}

			Alpha(o);
		}
	}
}

void MUTasking::MoveBoids(const mu_uint32 ThreadIndex)
{
	if (_ThreadsCount < 2)
	{
		MoveBoidsProcess(ThreadIndex);
		MoveFishsProcess(ThreadIndex);
	}
	else
	{
		if (ThreadIndex == 0) // Boids
		{
			MoveBoidsProcess(ThreadIndex);
		}
		else if (ThreadIndex == 1) // Fishs
		{
			MoveFishsProcess(ThreadIndex);
		}
	}
}

void MUTasking::CheckBoids(const mu_uint32 ThreadIndex)
{
	mu_uint32 queueCount = 0;
	const mu_uint32 queueIndex = MUThreadQueues::BoidsQueueIndex[ThreadIndex];

	mu_uint32 beginIndex, endIndex;
	GetIndexTasking(ThreadIndex, MaxBoids, beginIndex, endIndex);

	for (mu_uint32 i = beginIndex; i < endIndex; ++i)
	{
		const OBJECT* o = &Boids[i];

		if (o->Flags.Get(OBJECT::eOF_LIVE) == true &&
			o->Type != MODEL_SPEARSKILL &&
			o->BoneTransform != nullptr)
		{
			MUThreadQueues::BoidsQueue[queueIndex + queueCount++] = i;
		}
	}

	MUThreadQueues::BoidsQueueCount[ThreadIndex] = queueCount;
}

void MUTasking::CheckFishs(const mu_uint32 ThreadIndex)
{
	mu_uint32 queueCount = 0;
	const mu_uint32 queueIndex = MUThreadQueues::FishsQueueIndex[ThreadIndex];

	mu_uint32 beginIndex, endIndex;
	GetIndexTasking(ThreadIndex, MaxFishs, beginIndex, endIndex);

	for (mu_uint32 i = beginIndex; i < endIndex; ++i)
	{
		const OBJECT* o = &Fishs[i];

		if (o->Flags.Get(OBJECT::eOF_LIVE) == true &&
			o->Type != -1 &&
			o->BoneTransform != nullptr)
		{
			MUThreadQueues::FishsQueue[queueIndex + queueCount++] = i;
		}
	}

	MUThreadQueues::FishsQueueCount[ThreadIndex] = queueCount;
}

void MUTasking::PrepareBoids(const mu_uint32 ThreadIndex)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	mu_uint32 q = ThreadIndex;
	for (mu_uint32 t = 0; t < _ThreadsCount; ++t)
	{
		const mu_uint32 queueCount = MUThreadQueues::BoidsQueueCount[t];
		const mu_uint16 *queue = MUThreadQueues::BoidsQueue.data() + MUThreadQueues::BoidsQueueIndex[t];

		for (; q < queueCount; q += _ThreadsCount)
		{
			const mu_uint32 boidIndex = queue[q];
			OBJECT* o = &Boids[boidIndex];

			o->Angle[2] += 90.0f;

			ThreadInfo.RenderIndex = 0;
			ThreadInfo.CulledType = CULLED_BOIDS;
			ThreadInfo.ObjectIndex = boidIndex;

			RenderObject(ThreadIndex, o, true);

			switch (o->Type)
			{
			case MODEL_MONSTER01 + 31:
				if (o->SubType == 1)
				{
					MUModel* b = &MODELS::Data[o->Type];

					mu_float Bright = 1.0f;
					RenderPartObjectBodyColor(ThreadIndex, b, o, o->Type, o->Alpha, RENDER_METAL | RENDER_BRIGHT, Bright);
					RenderPartObjectBodyColor(ThreadIndex, b, o, o->Type, o->Alpha, RENDER_CHROME | RENDER_BRIGHT, Bright);
				}
				break;
			}

			if (_UpdateCounter > 0)
			{
				MUModel* b = &MODELS::Data[o->Type];
				EVector3 p, Position, Light;

				switch (o->Type)
				{
				case MODEL_MONSTER01 + 31:
					if (EventMask != 0)
					{
						Vector(0.0f, -50.0f, 0.0f, p);
						b->TransformPosition(ThreadIndex, ThreadInfo.BoneTransform[b->GetBoneIndex(11)], p, Position);
						Vector(1.0f, 0.0f, 0.0f, Light);
						CreateSprite(BITMAP_LIGHTNING + 1, Position, 1.0f, Light, o);
						Vector(1.0f, 1.0f, 1.0f, Light);
						CreateParticle(ThreadIndex, BITMAP_FIRE, Position, o->Angle, Light);
					}
					break;

				case MODEL_BUTTERFLY01:
				{
					mu_float  Luminosity = (mu_float)(GetLargeRand(32) + 64)*0.01f;

					Vector(Luminosity*0.2f, Luminosity*0.4f, Luminosity*0.4f, Light);
					CreateSprite(BITMAP_LIGHT, o->Position, 1.0f, Light, o);
				}
				break;

				case MODEL_MAP_TORNADO:
				{
					CreateParticle(ThreadIndex, BITMAP_CLOUD, o->Position, o->Angle, o->Light, 18, o->Scale, o);
				}
				break;

				case MODEL_CROW:
					Vector(-5.0f, 0.0f, 0.0f, p);
					b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(1)], p, Position, true);

					mu_float Luminosity = (mu_float)(GetLargeRand(32) + 128)*0.01f;
					Vector(Luminosity*1.0f, Luminosity*0.2f, 0.0f, Light);
					CreateSprite(BITMAP_LIGHT, Position, 0.1f, Light, o);

					Vector(5.0f, 0.0f, 0.0f, p);
					b->TransformPosition(ThreadIndex, o->BoneTransform[b->GetBoneIndex(1)], p, Position, true);
					CreateSprite(BITMAP_LIGHT, Position, 0.1f, Light, o);
					break;
				}
			}

			o->Angle[2] -= 90.0f;
		}

		q -= queueCount;
	}
}

void MUTasking::PrepareFishs(const mu_uint32 ThreadIndex)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	mu_uint32 q = ThreadIndex;
	for (mu_uint32 t = 0; t < _ThreadsCount; ++t)
	{
		const mu_uint32 queueCount = MUThreadQueues::FishsQueueCount[t];
		const mu_uint16 *queue = MUThreadQueues::FishsQueue.data() + MUThreadQueues::FishsQueueIndex[t];

		for (; q < queueCount; q += _ThreadsCount)
		{
			const mu_uint32 fishIndex = queue[q];
			OBJECT* o = &Fishs[fishIndex];

			o->Angle[2] += 90.0f;

			ThreadInfo.RenderIndex = 0;
			ThreadInfo.CulledType = CULLED_FISHS;
			ThreadInfo.ObjectIndex = fishIndex;

			RenderObject(ThreadIndex, o);

			o->Angle[2] -= 90.0f;
		}

		q -= queueCount;
	}
}