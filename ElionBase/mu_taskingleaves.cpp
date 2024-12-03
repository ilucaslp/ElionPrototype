#include "stdafx.h"
#include "mu_tasking.h"

mu_boolean CreateDevilSquareRain(const mu_uint32 ThreadIndex, PARTICLE* o, mu_int32 Index)
{
	if (InDevilSquare() == false
		&& GET_WORLD != WD_34CRYWOLF_1ST)
	{
		return false;
	}

	if (GET_WORLD == WD_34CRYWOLF_1ST && weather != 1)
		return false;

	if (GET_WORLD == WD_34CRYWOLF_1ST)
	{
		o->Type = BITMAP_RAIN2;
	}
	else
	{
		o->Type = BITMAP_RAIN;
	}
	if (Index < 300)
	{
		Vector(
			Hero->Object.Position[0] + (mu_float)(GetLargeRand(1600) - 800),
			Hero->Object.Position[1] + (mu_float)(GetLargeRand(1400) - 500),
			Hero->Object.Position[2] + (mu_float)(GetLargeRand(200) + 300),
			o->Position);
	}
	else
	{
		Vector(
			Hero->Object.Position[0] + (mu_float)(GetLargeRand(1600) - 800),
			Hero->Object.Position[1] + (mu_float)(GetLargeRand(300) + 1000) - RainPosition[ThreadIndex],
			Hero->Object.Position[2] + (mu_float)(GetLargeRand(200) + 300),
			o->Position);
	}
	if (GetLargeRand(2) == 0)
	{
		Vector((mu_float)(-(GetLargeRand(20) + 20)), 0.0f, 0.0f, o->Angle);
	}
	else
	{
		Vector((mu_float)(-(GetLargeRand(20) + 30 + RainAngle[ThreadIndex])), 0.0f, 0.0f, o->Angle);
	}
	EVector3 Velocity;
	Vector(0.0f, 0.0f, -(mu_float)(GetLargeRand(40) + RainSpeed[ThreadIndex]), Velocity);
	EMatrix4 Matrix = EMath::EulerToMatrix(o->Angle);
	o->Velocity = EMath::TransformNormal(Matrix, Velocity);

	return true;
}

mu_boolean CreateChaosCastleRain(const mu_uint32 ThreadIndex, PARTICLE* o, mu_int32 Index)
{
	if (InChaosCastle() == false) return false;

	o->Type = BITMAP_RAIN;
	o->TurningForce[0] = 1.0f;
	o->TurningForce[1] = 30.0f + GetLargeRand(10);
	if (Index < 300)
	{
		Vector(
			Hero->Object.Position[0] + (mu_float)(GetLargeRand(1600) - 800),
			Hero->Object.Position[1] + (mu_float)(GetLargeRand(1400) - 500),
			Hero->Object.Position[2] + (mu_float)(GetLargeRand(200) + 300),
			o->Position);
	}
	else
	{
		Vector(
			Hero->Object.Position[0] + (mu_float)(GetLargeRand(1600) - 800),
			Hero->Object.Position[1] + (mu_float)(GetLargeRand(300) + 1000) - RainPosition[ThreadIndex],
			Hero->Object.Position[2] + (mu_float)(GetLargeRand(200) + 300),
			o->Position);
	}
	if (GetLargeRand(2) == 0)
	{
		Vector((mu_float)(-(GetLargeRand(20) + 20)), 0.0f, 0.0f, o->Angle);
	}
	else
	{
		Vector((mu_float)(-(GetLargeRand(20) + 30 + RainAngle[ThreadIndex])), 0.0f, 0.0f, o->Angle);
	}
	EVector3 Velocity;
	Vector(0.0f, 0.0f, -(mu_float)(GetLargeRand(40) + RainSpeed[ThreadIndex] + 20), Velocity);
	EMatrix4 Matrix = EMath::EulerToMatrix(o->Angle);
	o->Velocity = EMath::TransformNormal(Matrix, Velocity);

	return true;
}

mu_boolean CreateLorenciaLeaf(PARTICLE* o)
{
	if (GET_WORLD != WD_0LORENCIA) return false;

	o->Type = BITMAP_LEAF05;
	EVector3 Position;
	Vector(Hero->Object.Position[0] + (mu_float)(GetLargeRand(1600) - 800), Hero->Object.Position[1] + (mu_float)(GetLargeRand(1400) - 500),
		Hero->Object.Position[2] + (mu_float)(GetLargeRand(300) + 50),
		Position);
	VectorCopy(Position, o->Position);
	VectorCopy(Position, o->StartPosition);
	o->Velocity[0] = -(mu_float)(GetLargeRand(64) + 64)*0.1f;
	if (Position[1] < MU_CAMERA->GetCameraPosition()[1] + 400.0f) o->Velocity[0] = -o->Velocity[0] + 3.2f;
	o->Velocity[1] = (mu_float)(GetLargeRand(32) - 16)*0.1f;
	o->Velocity[2] = (mu_float)(GetLargeRand(32) - 16)*0.1f;
	o->TurningForce[0] = (mu_float)(GetLargeRand(16) - 8)*0.1f;
	o->TurningForce[1] = (mu_float)(GetLargeRand(64) - 32)*0.1f;
	o->TurningForce[2] = (mu_float)(GetLargeRand(16) - 8)*0.1f;

	return true;
}

mu_boolean CreateHeavenRain(const mu_uint32 ThreadIndex, PARTICLE* o, mu_int32 index)
{
	if (GET_WORLD != WD_10HEAVEN) return false;

	mu_int32 Rainly = RainCurrent[ThreadIndex] * MAX_LEAVES / 100;
	if (index < Rainly)
	{
		o->Type = BITMAP_RAIN;
		Vector(Hero->Object.Position[0] + (mu_float)(GetLargeRand(1600) - 800), Hero->Object.Position[1] + (mu_float)(GetLargeRand(1400) - 500),
			Hero->Object.Position[2] + (mu_float)(GetLargeRand(200) + 200),
			o->Position);
		Vector(-30.0f, 0.0f, 0.0f, o->Angle);
		EVector3 Velocity;
		Vector(0.0f, 0.0f, -(mu_float)(GetLargeRand(24) + 20), Velocity);
		EMatrix4 Matrix = EMath::EulerToMatrix(o->Angle);
		o->Velocity = EMath::TransformNormal(Matrix, Velocity);
	}
	return true;
}

mu_boolean CreateDeviasSnow(PARTICLE* o)
{
	if (GET_WORLD != WD_2DEVIAS) return false;

	o->Type = BITMAP_LEAF01;
	o->Scale = 5.0f;
	if (GetLargeRand(5) == 0)
	{
		o->Type = BITMAP_LEAF08;
		o->Scale = 10.0f;
	}
	Vector(Hero->Object.Position[0] + (mu_float)(GetLargeRand(1600) - 800), Hero->Object.Position[1] + (mu_float)(GetLargeRand(1400) - 500),
		Hero->Object.Position[2] + (mu_float)(GetLargeRand(200) + 200),
		o->Position);
	Vector(-30.0f, 0.0f, 0.0f, o->Angle);
	EVector3 Velocity;
	Vector(0.0f, 0.0f, -(mu_float)(GetLargeRand(16) + 8), Velocity);
	EMatrix4 Matrix = EMath::EulerToMatrix(o->Angle);
	o->Velocity = EMath::TransformNormal(Matrix, Velocity);

	return true;
}

mu_boolean CreateAtlanseLeaf(PARTICLE*o)
{
	if (GET_WORLD != WD_3NORIA && GET_WORLD != WD_7ATLANSE) return false;

	if (GET_WORLD == WD_3NORIA)
		o->Type = BITMAP_LEAF04;
	else
		o->Type = BITMAP_LEAF02;

	EVector3 Position;
	Vector(Hero->Object.Position[0] + (mu_float)(GetLargeRand(1600) - 800), Hero->Object.Position[1] + (mu_float)(GetLargeRand(1400) - 500),
		Hero->Object.Position[2] + (mu_float)(GetLargeRand(300) + 50),
		Position);
	VectorCopy(Position, o->Position);
	VectorCopy(Position, o->StartPosition);
	o->Velocity[0] = -(mu_float)(GetLargeRand(64) + 64)*0.1f;
	if (Position[1] < MU_CAMERA->GetCameraPosition()[1] + 400.0f) o->Velocity[0] = -o->Velocity[0] + 3.2f;
	o->Velocity[1] = (mu_float)(GetLargeRand(32) - 16)*0.1f;
	o->Velocity[2] = (mu_float)(GetLargeRand(32) - 16)*0.1f;
	o->TurningForce[0] = (mu_float)(GetLargeRand(16) - 8)*0.1f;
	o->TurningForce[1] = (mu_float)(GetLargeRand(64) - 32)*0.1f;
	o->TurningForce[2] = (mu_float)(GetLargeRand(16) - 8)*0.1f;

	return true;
}

mu_boolean MoveDevilSquareRain(const mu_uint32 ThreadIndex, PARTICLE_HEADER *oh, PARTICLE* o)
{
	if (InDevilSquare() == false
		&& GET_WORLD != WD_34CRYWOLF_1ST
		) return false;

	if (GET_WORLD == WD_34CRYWOLF_1ST && weather != 1)
		return false;

	VectorAdd(o->Position, o->Velocity, o->Position);
	mu_float Height = RequestTerrainHeight(o->Position[0], o->Position[1]);
	if (o->Position[2] < Height)
	{
		oh->Flags.Clear(PARTICLE::ePF_LIVE);
		o->Position[2] = Height + 10.0f;
		if (GetLargeRand(4) == 0)
			CreateParticle(ThreadIndex, BITMAP_RAIN_CIRCLE, o->Position, o->Angle, o->Light);
		else
			CreateParticle(ThreadIndex, BITMAP_RAIN_CIRCLE + 1, o->Position, o->Angle, o->Light);
	}
	return true;
}

mu_boolean MoveChaosCastleRain(const mu_uint32 ThreadIndex, PARTICLE_HEADER *oh, PARTICLE* o)
{
	if (InChaosCastle() == false) return false;

	VectorAdd(o->Position, o->Velocity, o->Position);
	mu_float Height = RequestTerrainHeight(o->Position[0], o->Position[1]);
	if (o->Position[2] < Height && (TERRAIN_ATTRIBUTE(o->Position[0], o->Position[1])&TW_NOGROUND) != TW_NOGROUND)
	{
		oh->Flags.Clear(PARTICLE::ePF_LIVE);
		o->Position[2] = Height + 10.0f;
		if (GetLargeRand(4) == 0)
			CreateParticle(ThreadIndex, BITMAP_RAIN_CIRCLE, o->Position, o->Angle, o->Light);
		else
			CreateParticle(ThreadIndex, BITMAP_RAIN_CIRCLE + 1, o->Position, o->Angle, o->Light);
	}
	return true;
}

mu_boolean MoveHeavenRain(const mu_uint32 ThreadIndex, PARTICLE_HEADER *oh, PARTICLE* o)
{
	if (GET_WORLD != WD_0LORENCIA && GET_WORLD != WD_10HEAVEN) return false;

	if (o->Type == BITMAP_RAIN)
	{
		VectorAdd(o->Position, o->Velocity, o->Position);
		mu_float Height = RequestTerrainHeight(o->Position[0], o->Position[1]);
		if (o->Position[2] < Height)
		{
			oh->Flags.Clear(PARTICLE::ePF_LIVE);
			o->Position[2] = Height + 10.0f;
			if (GET_WORLD != WD_10HEAVEN)
				CreateParticle(ThreadIndex, BITMAP_RAIN_CIRCLE, o->Position, o->Angle, o->Light);
		}
	}
	else
	{
		o->Velocity[0] += (mu_float)(GetLargeRand(16) - 8)*0.1f;
		o->Velocity[1] += (mu_float)(GetLargeRand(16) - 8)*0.1f;
		o->Velocity[2] += (mu_float)(GetLargeRand(16) - 8)*0.1f;
		VectorAdd(o->Position, o->Velocity, o->Position);

		o->TurningForce[0] += (mu_float)(GetLargeRand(8) - 4)*0.02f;
		o->TurningForce[1] += (mu_float)(GetLargeRand(16) - 8)*0.02f;
		o->TurningForce[2] += (mu_float)(GetLargeRand(8) - 4)*0.02f;
		VectorAdd(o->Angle, o->TurningForce, o->Angle);

		EVector3 Range;
		VectorSubtract(o->StartPosition, o->Position, Range);
		mu_float Length = Range[0] * Range[0] + Range[1] * Range[1] + Range[2] * Range[2];
		if (Length >= 200000.0f) oh->Flags.Clear(PARTICLE::ePF_LIVE);
	}

	return true;
}

void MoveEtcLeaf(PARTICLE_HEADER *oh, PARTICLE* o)
{
	mu_float Height = RequestTerrainHeight(o->Position[0], o->Position[1]);
	if (o->Position[2] <= Height)
	{
		o->Position[2] = Height;
		o->Light[0] -= 0.05f;
		o->Light[1] -= 0.05f;
		o->Light[2] -= 0.05f;
		if (o->Light[0] <= 0.0f)
			oh->Flags.Clear(PARTICLE::ePF_LIVE);
	}
	else
	{
		o->Velocity[0] += (mu_float)(GetLargeRand(16) - 8)*0.1f;
		o->Velocity[1] += (mu_float)(GetLargeRand(16) - 8)*0.1f;
		o->Velocity[2] += (mu_float)(GetLargeRand(16) - 8)*0.1f;
		VectorAdd(o->Position, o->Velocity, o->Position);
	}
}

void MUTasking::MoveLeaves(const mu_uint32 ThreadIndex)
{
	const mu_int32 rainTarget = RainTarget;

	if (RainCurrent[ThreadIndex] > rainTarget)
		--RainCurrent[ThreadIndex];
	else if (RainCurrent[ThreadIndex] < rainTarget)
		++RainCurrent[ThreadIndex];

	RainSpeed[ThreadIndex] = (mu_int32)EMath::Sin(FWorldTime*0.001f) * 10 + 30;
	RainAngle[ThreadIndex] = (mu_int32)EMath::Sin(FWorldTime*0.0005f + 50.0f) * 20;
	RainPosition[ThreadIndex] += 20;
	RainPosition[ThreadIndex] %= 2000;

	if (IsEmpireGuardian() == false ||
		(g_EmpireGuardian1.GetWeather() == GMEmpireGuardian1::WEATHER_TYPE::WEATHER_RAIN ||
			g_EmpireGuardian1.GetWeather() == GMEmpireGuardian1::WEATHER_TYPE::WEATHER_STORM))
	{
		for (mu_int32 i = ThreadIndex; i < MaxLeaves; i += _ThreadsCount)
		{
			PARTICLE_HEADER *oh = &LeavesHeader[i];
			PARTICLE *o = &Leaves[i];

			if (!oh->Flags.Get(PARTICLE::ePF_LIVE))
			{
				Vector(1.0f, 1.0f, 1.0f, o->Light);
				
				if (CreateDevilSquareRain(ThreadIndex, o, i) ||
					CreateChaosCastleRain(ThreadIndex, o, i) ||
					CreateLorenciaLeaf(o) ||
					CreateHeavenRain(ThreadIndex, o, i) ||
					CreateDeviasSnow(o) ||
					CreateAtlanseLeaf(o) ||
					battleCastle::CreateFireSnuff(o) ||
					M31HuntingGround::CreateMist(ThreadIndex, oh, o) ||
					(weather == 2 && M34CryWolf1st::CreateMist(ThreadIndex, oh, o)) ||
					g_3rdChangeUp.CreateFireSnuff(o) ||
					g_Raklion.CreateSnow(o) ||
					g_SantaTown.CreateSnow(o) ||
					g_PKField.CreateFireSpark(o) ||
					g_DoppelGanger2.CreateFireSpark(o) ||
					g_EmpireGuardian1.CreateRain(ThreadIndex, oh, o) ||
					g_UnitedMarketPlace.CreateRain(o))
				{
					if(o->Type != -1)
					{
						oh->Flags.Set(PARTICLE::ePF_LIVE);

						MUTexture *texture = &TEXTURES::Data[o->Type];

						if (texture->TextureInternal.Components == 3)
						{
							o->RenderType = BLEND::TXB_BLEND01;
						}
						else
						{
							o->RenderType = BLEND::TXB_ALPHATEST_NOMASK;
						}
					}
				}
			}
			else
			{
				if (MoveDevilSquareRain(ThreadIndex, oh, o)) continue;
				if (MoveChaosCastleRain(ThreadIndex, oh, o)) continue;
				if (MoveHeavenRain(ThreadIndex, oh, o))      continue;
				if (g_UnitedMarketPlace.MoveRain(ThreadIndex, oh, o)) continue;
				MoveEtcLeaf(oh, o);
			}
		}
	}
}

void MUTasking::CheckLeaves(const mu_uint32 ThreadIndex)
{
	if (GET_WORLD == WD_34CRYWOLF_1ST &&
		weather != 1)
	{
		return;
	}

	mu_uint32 beginIndex, endIndex;
	GetIndexTasking(ThreadIndex, MaxLeaves, beginIndex, endIndex);

	for (mu_uint32 i = beginIndex; i < endIndex; ++i)
	{
		const PARTICLE_HEADER* o = &LeavesHeader[i];

		if (o->Flags.Get(PARTICLE::ePF_LIVE) == true)
		{
			EEffectCulledData &instanceData = g_EffectsCulledData[g_EffectsCulledDataCount.fetch_add(1)];
			instanceData.Type = ECastEnum(EEffectProxyEnum::eLeaves);
			instanceData.Index = i;
		}
	}
}