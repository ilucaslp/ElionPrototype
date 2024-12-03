#include "stdafx.h"
#include "mu_tasking.h"

void MUTasking::CleanCharacters()
{
	for (mu_uint32 n = 0; n < MAX_CHARACTERS_CLIENT; ++n)
	{
		CHARACTER *c = &CharactersClient[n];
		OBJECT *o = &c->Object;
		DeleteCharacter(c, o);
	}

	mu_memset(CharactersKeys.data(), 0xFF, SizeOfArray(CharactersKeys));
}

void MUTasking::CleanSounds()
{
#ifndef DISABLE_AUDIO
	for (mu_uint32 i = 0; i < g_SoundsCurCount; ++i)
	{
		MUAudioSource *o = &g_Sounds[i];

		if (o->Live == true)
		{
			o->Delete();
		}
	}

	g_SoundsCurCount = 0;
	g_SoundsFreeCount = 0;
	g_SoundsDeleteCount = 0;
#endif
}

void MUTasking::CleanEffects()
{
	// Effects
	g_EffectsCurCount = 0;
	g_EffectsFreeCount = 0;
	g_EffectsWaitCount = 0;
	g_EffectsEraseCount = 0;
	g_EffectsDeleteCount = 0;

	for (mu_uint32 n = 0; n < MAX_EFFECTS; ++n)
	{
		g_Effects[n].Flags.Clear();
	}

	// Blurs
	g_BlurCurCount = 0;
	g_BlurFreeCount = 0;
	g_BlurDeleteCount = 0;
	
	g_ObjectBlurCurCount = 0;
	g_ObjectBlurFreeCount = 0;
	g_ObjectBlurDeleteCount = 0;

	// Joints
	g_JointsCurCount = 0;
	g_JointsFreeCount = 0;
	g_JointsWaitCount = 0;
	g_JointsEraseCount = 0;
	g_JointsDeleteCount = 0;

	for (mu_uint32 n = 0; n < MAX_JOINTS; ++n)
	{
		g_JointsHeader[n].Flags.Clear();
	}

	// Particles
	g_ParticlesCurCount = 0;
	g_ParticlesFreeCount = 0;
	g_ParticlesWaitCount = 0;
	g_ParticlesEraseCount = 0;
	g_ParticlesDeleteCount = 0;

	for (mu_uint32 n = 0; n < MAX_PARTICLES; ++n)
	{
		g_ParticlesHeader[n].Flags.Clear();
	}

	// Points
	g_PointsCurCount = 0;
	g_PointsFreeCount = 0;
	g_PointsDeleteCount = 0;

	// Pointers
	g_PointersCurCount = 0;
	g_PointersFreeCount = 0;
	g_PointersDeleteCount = 0;

	// Sprites
	g_SpritesCount = 0;
	g_TileSpritesCount = 0;
	g_GuildSpritesCount = 0;
}

void MUTasking::CleanBoids()
{
	for (mu_uint32 i = 0; i < MAX_BOIDS; ++i)
	{
		Boids[i].Flags.Clear();
	}

	for (mu_uint32 i = 0; i < MAX_FISHS; ++i)
	{
		Fishs[i].Flags.Clear();
	}

	// Bugs
	g_ButterflesCurCount = 0;
	g_ButterflesFreeCount = 0;
	g_ButterflesDeleteCount = 0;
}

void MUTasking::CleanPets()
{
	// Pets
	g_PetObjectsCurCount = 0;
	g_PetObjectsFreeCount = 0;
}

void MUTasking::CleanLeaves()
{
	for (mu_uint32 i = 0; i < MAX_LEAVES; ++i)
	{
		LeavesHeader[i].Flags.Clear();
	}
}

void MUTasking::CleanOperates()
{
	g_OperatesCount = 0;
}

void MUTasking::CleanWorldObjects()
{
	for (mu_uint32 i = 0; i < MAX_WORLD_OBJECTS; ++i)
	{
		MODELS::Data[i].Release();
	}

	ZeroArray(g_EnvironmentBlocks);
	ZeroArray(g_EnvironmentBlocksCount);

	g_EnvironmentObjectsCount = 0;
	ZeroArray(g_EmpireGuardianAnimationFrame);
	ZeroArray(g_EmpireGuardianPriorAnimationFrame);
	ZeroArray(g_EmpireGuardianPriorAction);
}

void MUTasking::CleanProxy()
{
	g_RenderProxyCount = 0;
	g_RenderProxyMap.clear();
	g_RenderProxyFlyMap.clear();
}

void MUTasking::SettingBoids()
{
	switch (GET_WORLD) // Boids
	{
	case WD_7ATLANSE:
	case WD_67DOPPLEGANGER3:
		MaxBoids = MAX_BOIDS;
		break;

	case WD_10HEAVEN:
		MaxBoids = 13;
		break;

	case WD_51HOME_6TH_CHAR:
		MaxBoids = 2;
		break;

	default:
		if (InHellas() == true)
		{
			MaxBoids = 2;
		}
		else
		{
			MaxBoids = 5;
		}
		break;
	}

	switch (GET_WORLD)
	{
	case WD_0LORENCIA:
	case WD_1DUNGEON:
	case WD_6STADIUM:
		MaxFishs = 3;
		break;

	case WD_7ATLANSE:
	case WD_67DOPPLEGANGER3:
	case WD_8TARKAN:
	case WD_34CRYWOLF_1ST:
		MaxFishs = MAX_FISHS;
		break;


	default:
		if (InHellas())
		{
			MaxFishs = MAX_FISHS;
		}
		else if (M33Aida::IsInAida())
		{
			MaxFishs = 3;
		}
		else
		{
			MaxFishs = 0;
		}
		break;
	}

	if (InDevilSquare() == true)
	{
		MaxLeaves = 20;// MAX_LEAVES;
	}
	else if (GET_WORLD == WD_10HEAVEN)
	{
		RainTarget = MAX_LEAVES / 2;
	}
	else if (InChaosCastle() == true)
	{
		RainTarget = MAX_LEAVES / 2;
		MaxLeaves = 80;
	}
	else if (GET_WORLD == WD_30BATTLECASTLE)
	{
		MaxLeaves = 40;
	}
	else if (M34CryWolf1st::IsCyrWolf1st())
	{
		if (weather == 1)
			MaxLeaves = 60;
		else if (weather == 2)
			MaxLeaves = 50;
	}
	else if (GET_WORLD == WD_2DEVIAS)
	{
		MaxLeaves = 20;
	}
	else
	{
		MaxLeaves = 10;
	}
}

void MUTasking::DeleteEffects()
{
	mu_int32 deletedCount = g_EffectsDeleteCount.exchange(0);
	if (g_EffectsEraseCount > 0)
	{
		for (mu_uint32 n = 0; n < g_EffectsEraseCount; ++n)
		{
			EFFECT_DELETE &effect = g_EffectsErase[n];

			for (mu_uint32 i = 0; i < g_EffectsCurCount; ++i)
			{
				OBJECT *o = &g_Effects[i];

				if (o->Flags.Get(OBJECT::eOF_LIVE) == true &&
					o->CreationTime < g_CreationTime &&
					(effect.Owner == nullptr || o->Owner == effect.Owner) &&
					o->Type == effect.Type &&
					(effect.SubType == -1 || o->SubType == effect.SubType))
				{
					++deletedCount;
					o->Flags.Clear(OBJECT::eOF_LIVE);
				}
			}
		}

		g_EffectsEraseCount = 0;
	}

	if (deletedCount > 0)
	{
		for (mu_uint32 i = 0; i < g_EffectsCurCount; ++i)
		{
			OBJECT *effect = &g_Effects[i];

			if (effect->Flags.GetAny(OBJECT::eOF_LIVE | OBJECT::eOF_FREE) == false)
			{
				switch (effect->Type)
				{
				case MODEL_EFFECT_FLAME_STRIKE:
					RemoveObjectBlurs(effect, 1);
					RemoveObjectBlurs(effect, 2);
					RemoveObjectBlurs(effect, 3);
					break;
				case MODEL_SUMMONER_SUMMON_LAGUL:
					for (mu_int32 i = 48; i <= 53; ++i)
					{
						DeleteJoint(BITMAP_JOINT_ENERGY, effect, i);
					}
					break;
				}

				effect->Owner = nullptr;
				effect->Flags.Set(OBJECT::eOF_FREE);

				g_EffectsFree[g_EffectsFreeCount++] = i;
			}
		}
	}

	deletedCount = g_JointsDeleteCount.exchange(0);
	if (g_JointsEraseCount > 0)
	{
		for (mu_uint32 n = 0; n < g_JointsEraseCount; ++n)
		{
			JOINT_DELETE &joint = g_JointsErase[n];

			for (mu_uint32 i = 0; i < g_JointsCurCount; ++i)
			{
				JOINT_HEADER *oh = &g_JointsHeader[i];
				if (oh->Flags.Get(JOINT::eJF_LIVE) == false)
				{
					continue;
				}

				JOINT_CHECK *oc = &g_JointsCheck[i];
				if (oc->CreationTime >= g_CreationTime)
				{
					continue;
				}

				JOINT *o = &g_Joints[i];

				if ((joint.Target == nullptr || o->Target == joint.Target) &&
					o->Type == joint.Type &&
					(joint.SubType == -1 || o->SubType == joint.SubType))
				{
					++deletedCount;
					oh->Flags.Clear();
				}
			}
		}

		g_JointsEraseCount = 0;
	}

	if (deletedCount > 0)
	{
		for (mu_uint32 i = 0; i < g_JointsCurCount; ++i)
		{
			JOINT_HEADER *joint = &g_JointsHeader[i];

			if (joint->Flags.GetAny(JOINT::eJF_LIVE | JOINT::eJF_FREE) == false)
			{
				joint->Flags.Set(JOINT::eJF_FREE);

				g_JointsFree[g_JointsFreeCount++] = i;
			}
		}
	}
}

void MUTasking::DeleteParticles()
{
	mu_int32 deletedCount = g_ParticlesDeleteCount.exchange(0);

	if (g_ParticlesEraseCount > 0)
	{
		for (mu_uint32 n = 0; n < g_ParticlesEraseCount; ++n)
		{
			PARTICLE_DELETE &particle = g_ParticlesErase[n];

			for (mu_uint32 i = 0; i < g_ParticlesCurCount; ++i)
			{
				PARTICLE_HEADER *oh = &g_ParticlesHeader[i];
				if (oh->Flags.Get(PARTICLE::ePF_LIVE) == false)
				{
					continue;
				}

				PARTICLE_CHECK *oc = &g_ParticlesCheck[i];
				if (oc->CreationTime >= g_CreationTime)
				{
					continue;
				}

				PARTICLE *o = &g_Particles[i];
				if (o->Type == particle.Type)
				{
					++deletedCount;
					oh->Flags.Clear(PARTICLE::ePF_LIVE);
				}
			}
		}

		g_ParticlesEraseCount = 0;
	}

	if (deletedCount > 0)
	{
		for (mu_uint32 i = 0; i < g_ParticlesCurCount; ++i)
		{
			PARTICLE_HEADER *particle = &g_ParticlesHeader[i];

			if (particle->Flags.GetAny(PARTICLE::ePF_LIVE | PARTICLE::ePF_FREE) == false)
			{
				particle->Flags.Set(PARTICLE::ePF_FREE);

				g_ParticlesFree[g_ParticlesFreeCount++] = i;
			}
		}
	}
}

void MUTasking::DeletePoints()
{
	mu_int32 deletedCount = g_PointsDeleteCount.exchange(0);
	if (deletedCount > 0)
	{
		for (mu_uint32 i = 0; i < g_PointsCurCount; ++i)
		{
			MUPoint *o = &g_Points[i];

			if (o->Flags.GetAny(MUPoint::ePF_LIVE | MUPoint::ePF_FREE) == false)
			{
				o->Flags.Set(MUPoint::ePF_FREE);

				g_PointsFree[g_PointsFreeCount++] = i;
			}
		}
	}
}

void MUTasking::DeletePointers()
{
	mu_int32 deletedCount = g_PointersDeleteCount.exchange(0);
	if (deletedCount > 0)
	{
		for (mu_uint32 i = 0; i < g_PointersCurCount; ++i)
		{
			MUPointer *o = &g_Pointers[i];

			if (o->Flags.GetAny(POINTER::ePF_LIVE | POINTER::ePF_FREE) == false)
			{
				o->Flags.Set(POINTER::ePF_FREE);

				g_PointersFree[g_PointersFreeCount++] = i;
			}
		}
	}
}

void MUTasking::DeleteButterfles()
{
	if (g_ButterflesDeleteCount > 0)
	{
		for (mu_uint32 i = 0; i < g_ButterflesCurCount; ++i)
		{
			OBJECT *o = &g_Butterfles[i];

			if (o->Flags.GetAny(OBJECT::eOF_LIVE | OBJECT::eOF_FREE) == false)
			{
				o->Flags.Set(OBJECT::eOF_FREE);

				g_ButterflesFree[g_ButterflesFreeCount++] = i;
			}
		}

		g_ButterflesDeleteCount = 0;
	}
}

void MUTasking::DeleteBlurs()
{
	if (g_BlurDeleteCount > 0)
	{
		for (mu_uint32 i = 0; i < g_BlurCurCount; ++i)
		{
			MUBlur *b = &g_Blur[i];

			if (b->Flags.GetAny(BLUR::eBF_LIVE | BLUR::eBF_FREE) == false)
			{
				b->Flags.Set(BLUR::eBF_FREE);

				g_BlurFree[g_BlurFreeCount++] = i;
			}
		}

		g_BlurDeleteCount = 0;
	}

	if (g_ObjectBlurDeleteCount > 0)
	{
		for (mu_uint32 i = 0; i < g_ObjectBlurCurCount; ++i)
		{
			MUObjectBlur *b = &g_ObjectBlur[i];

			if (b->Flags.GetAny(BLUR::eBF_LIVE | BLUR::eBF_FREE) == false)
			{
				b->Flags.Set(BLUR::eBF_FREE);

				g_ObjectBlurFree[g_ObjectBlurFreeCount++] = i;
			}
		}

		g_ObjectBlurDeleteCount = 0;
	}
}

void MUTasking::EraseEffects(const mu_uint32 ThreadIndex)
{
	switch (_ThreadsCount)
	{
	case 1:
		{
			DeleteEffects();
			DeleteBlurs();
			DeletePoints();
			DeletePointers();
			DeleteParticles();
			DeleteButterfles();
		}
		break;

	case 2:
		{
			if (ThreadIndex == 0)
			{
				DeleteEffects();
				DeleteBlurs();
				DeletePoints();
				DeletePointers();
			}
			else if (ThreadIndex == 1)
			{
				DeleteParticles();
				DeleteButterfles();
			}
		}
		break;

	case 3:
		{
			if (ThreadIndex == 0)
			{
				DeleteEffects();
				DeleteBlurs();
			}
			else if (ThreadIndex == 1)
			{
				DeleteParticles();
			}
			else if (ThreadIndex == 2)
			{
				DeleteButterfles();
				DeletePoints();
				DeletePointers();
			}
		}
		break;

	case 4:
	default:
		{
			if (ThreadIndex == 0)
			{
				DeleteEffects();
				DeleteBlurs();
			}
			else if (ThreadIndex == 1)
			{
				DeleteParticles();
			}
			else if (ThreadIndex == 2)
			{
				DeleteButterfles();
			}
			else if (ThreadIndex == 3)
			{
				DeletePoints();
				DeletePointers();
			}
		}
		break;
	}
}