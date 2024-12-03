#include "stdafx.h"
#include "mu_particles.h"

MUCriticalSectionSpinLock g_ParticlesLock;
MUCriticalSectionSpinLock g_ParticlesEraseLock;
std::array<PARTICLE_HEADER, MAX_PARTICLES> g_ParticlesHeader;
std::array<PARTICLE_CHECK, MAX_PARTICLES> g_ParticlesCheck;
std::array<PARTICLE, MAX_PARTICLES> g_Particles;
std::array<mu_uint16, MAX_PARTICLES + MAX_TASKING_THREADS> g_ParticlesQueue;
std::array<mu_uint16, MAX_PARTICLES> g_ParticlesFree;
mu_uint16 g_ParticlesCurCount = 0;
mu_uint16 g_ParticlesFreeCount = 0;
mu_atomic_int32_t g_ParticlesDeleteCount(0);
mu_boolean g_ParticlesLocked = false;

std::array<PARTICLE_WAIT, MAX_PARTICLES_WAIT> g_ParticlesWait;
mu_atomic_uint32_t g_ParticlesWaitCount(0);
std::array<PARTICLE_DELETE, MAX_PARTICLES_WAIT> g_ParticlesErase;
mu_uint16 g_ParticlesEraseCount = 0;

void CreateBomb(const mu_uint32 ThreadIndex, EVector3 p, mu_boolean Exp, mu_uint32 SubType)
{
	EVector3 Position, Light;
	VectorCopy(p, Position);
	Position[2] += 80.0f;
	EVector3 Angle;

	Vector(1.0f, 1.0f, 1.0f, Light);

	if (SubType != 5)
	{
		for (mu_uint32 j = 0; j < 20; ++j)
		{
			Vector((mu_float)(GetLargeRand(60) + 60 + 90), 0.0f, (mu_float)(GetLargeRand(30)), Angle);
			if (SubType == 2)
			{
				Vector(0.7f, 0.7f, 1.0f, Light);
				CreateParticle(ThreadIndex, BITMAP_SPARK, Position, Angle, Light, 8);
			}
			else if (SubType == 3)
			{
				Vector(1.0f, 0.7f, 0.4f, Light);
				CreateParticle(ThreadIndex, BITMAP_SPARK, Position, Angle, Light, 8);
			}
			else if (SubType == 4)
			{
				if (j == 5) break;
				Vector(1.0f, 0.5f, 0.3f, Light);
				CreateParticle(ThreadIndex, BITMAP_SPARK, Position, Angle, Light, 8);
			}
			else if (SubType == 6)
			{
				Vector(0.3f, 0.3f, 0.3f, Light);
				CreateParticle(ThreadIndex, BITMAP_SPARK, Position, Angle, Light, 10);
			}
			else
			{
				CreateParticle(ThreadIndex, BITMAP_SPARK, Position, Angle, Light, 2);
			}
		}
	}

	Vector(0.7f, 0.7f, 0.7f, Light);

	if (Exp)
	{
		if (SubType == 2
			|| SubType == 5
			|| SubType == 6
			)
		{
			Vector(0.3f, 0.6, 1.0f, Light);
			if (SubType == 6)
			{
				Vector(0.3f, 0.3f, 0.3f, Light);
			}

			CreateParticle(ThreadIndex, BITMAP_EXPLOTION_MONO, Position, Angle, Light);
		}
		else if (SubType == 3)
		{
			Vector(1.0f, 0.6, 0.3f, Light);
			CreateParticle(ThreadIndex, BITMAP_EXPLOTION_MONO, Position, Angle, Light);
		}
		else if (SubType == 4)
		{
			Vector(1.0f, 0.4, 0.2f, Light);
			CreateParticle(ThreadIndex, BITMAP_EXPLOTION_MONO, Position, Angle, Light, 0, 0.5f);
			Vector(1.0f, 0.6, 0.2f, Light);
			CreateParticle(ThreadIndex, BITMAP_EXPLOTION_MONO, Position, Angle, Light, 0, 0.3f);
		}
		else
		{
			CreateParticle(ThreadIndex, BITMAP_EXPLOTION, Position, Angle, Light);
		}
	}
}

void CreateBomb2(const mu_uint32 ThreadIndex, EVector3 p, mu_boolean Exp, mu_uint32 SubType, mu_float Scale)
{
	EVector3 Position, Light;
	VectorCopy(p, Position);
	Position[2] += 30.0f;
	EVector3 Angle;
	Vector(1.0f, 1.0f, 1.0f, Light);
	for (mu_uint32 j = 0; j < 20; ++j)
	{
		Vector((mu_float)(GetLargeRand(60) + 60 + 90), 0.0f, (mu_float)(GetLargeRand(30)), Angle);
		CreateParticle(ThreadIndex, BITMAP_SPARK, Position, Angle, Light, 2);
	}
	Vector(1.0f, 1.0f, 1.0f, Light);
	Vector(0.0f, 0.0f, 0.0f, Angle);
	if (Exp)
	{
		CreateParticle(ThreadIndex, BITMAP_EXPLOTION_MONO, Position, Angle, Light, 0, 4.0f + Scale);
	}
	else
	{
		CreateParticle(ThreadIndex, BITMAP_EXPLOTION + 1, Position, Angle, Light, 0, 4.0f);
	}
}

void CreateBomb3(const mu_uint32 ThreadIndex, EVector3 vPos, mu_uint32 iSubType)
{
	EVector3 vBombPos, vAngle, vLight;
	Vector((mu_float)(GetLargeRand(60) + 60 + 90), 0.0f, (mu_float)(GetLargeRand(30)), vAngle);
	Vector(1.0f, 1.0f, 1.0f, vLight);
	for (mu_uint32 i = 0; i < 2; ++i)
	{
		if (iSubType == 2)
		{
			if (GetLargeRand(10) == 0) break;
			Vector(vPos[0] + GetLargeRand(150) - 75, vPos[1] + GetLargeRand(150) - 75, vPos[2] + GetLargeRand(130) + 30, vBombPos);
		}
		else if (iSubType == 1)
		{
			if (i == 1) break;
			else if (GetLargeRand(5) == 0) break;
			Vector(vPos[0] + GetLargeRand(80) - 40, vPos[1] + GetLargeRand(80) - 40, vPos[2] + GetLargeRand(120) + 30, vBombPos);
		}
		else
		{
			if (i == 1) break;
			else if (GetLargeRand(3) != 0) break;
			Vector(vPos[0] + GetLargeRand(30) - 15, vPos[1] + GetLargeRand(30) - 15, vPos[2] + GetLargeRand(80) + 30, vBombPos);
		}

		CreateParticle(ThreadIndex, BITMAP_SUMMON_SAHAMUTT_EXPLOSION, vBombPos, vAngle, vLight, 0, 0.15f*(GetLargeRand(10)));
	}

	Vector(1.0f, 0.5f, 0.2f, vLight);
	CreateParticle(ThreadIndex, BITMAP_MAGIC + 1, vBombPos, vAngle, vLight, 0, 1.0f);
	Vector(1.0f, 0.6f, 0.2f, vLight);
	for (mu_uint32 i = 0; i < 3; ++i)
	{
		CreateEffect(ThreadIndex, MODEL_STONE1 + GetLargeRand(2), vBombPos, vAngle, vLight, 13);
	}
	Vector(1.0f, 1.0f, 1.0f, vLight);

	for (mu_uint32 i = 0; i < 5; ++i)
	{
		CreateParticle(ThreadIndex, BITMAP_SPARK, vBombPos, vAngle, vLight, 2);
	}
}

void HandPosition(const mu_uint32 ThreadIndex, PARTICLE *o)
{
	OBJECT *Owner = o->Target;
	MUModel *b = &MODELS::Data[Owner->Type];

	EVector3 p;
	switch (o->Type)
	{
	case BITMAP_FLARE_RED:
	case BITMAP_SHINY + 2:
		Vector(0.0f, -120.0f, 0.0f, p);
		break;
	default:
		Vector(0.0f, 0.0f, 0.0f, p);
		break;
	}

	VectorCopy(Owner->Position, b->BodyOrigin(ThreadIndex));
	b->TransformPosition(ThreadIndex, Owner->BoneTransform[Hero->Weapon[o->SubType % 2].LinkBone], p, o->Position, true);
}

template<mu_boolean isLockedAllowed>
void CreateParticle(const mu_uint32 ThreadIndex, mu_uint32 Type, EVector3 &Position, EVector3 &Angle, EVector3 &Light, mu_uint32 SubType, mu_float Scale, OBJECT *Owner)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	if (ThreadInfo.RenderGUI == true)
	{
		return;
	}

	if constexpr(isLockedAllowed)
	{
		if (ThreadInfo.NoLockedAllowed == false &&
			g_ParticlesLocked == true)
		{
			const mu_uint32 i = g_ParticlesWaitCount.fetch_add(1);

			if (i < MAX_PARTICLES_WAIT)
			{
				PARTICLE_WAIT &o = g_ParticlesWait[i];

				o.Type = Type;
				o.SubType = SubType;
				o.Scale = Scale;
				o.Position = Position;
				o.Angle = Angle;
				o.Light = Light;
				o.Owner = Owner;
			}

			return;
		}
	}

	mu_uint32 i = 0;

	g_ParticlesLock.Lock();

	if (g_ParticlesFreeCount > 0)
	{
		i = g_ParticlesFree[--g_ParticlesFreeCount];
	}
	else if (g_ParticlesCurCount < MAX_PARTICLES)
	{
		i = g_ParticlesCurCount++;
	}
	else
	{
		g_ParticlesLock.Unlock();
		return;
	}

	g_ParticlesLock.Unlock();

	PARTICLE_HEADER *oh = &g_ParticlesHeader[i];
	PARTICLE_CHECK *oc = &g_ParticlesCheck[i];
	PARTICLE *o = &g_Particles[i];

	oh->Flags.Clear();
	oh->Flags.Set(PARTICLE::ePF_LIVE);

	oc->CreationTime = g_CreationTime;

	o->Type = Type;
	o->TexType = Type;
	o->SubType = SubType;

	VectorCopy(Position, o->Position);
	VectorCopy(Position, o->StartPosition);
	VectorCopy(Light, o->Light);
	o->Scale = Scale;
	o->Gravity = 0.0f;
	o->LifeTime = 2;
	o->Frame = 0;
	o->Target = Owner;
	o->Rotation = 0.0f;
	VectorCopy(Angle, o->Angle);
	Vector(0.0f, 0.0f, 0.0f, o->Velocity);

	EVector3 p;
	EBone Matrix;

	switch (o->Type)
	{
	case BITMAP_EFFECT:
		if (o->SubType == 1)
		{
			o->TexType = BITMAP_EXT_LOG_IN + 2;
			o->Scale = (mu_float)(GetLargeRand(10) + 20)*0.01f;
			o->Gravity = (mu_float)(GetLargeRand(10) + 20)*0.05f;
		}
		else if (o->SubType == 2)
		{
			o->Gravity = 0.0f;
			o->Scale = (mu_float)(GetLargeRand(5) + 12)*0.1f;
		}
		else if (o->SubType == 3)
		{
			o->TexType = BITMAP_CLUD64;
			o->Scale = (mu_float)(GetLargeRand(5) + 10)*0.01f;
			o->Gravity = (mu_float)(GetLargeRand(10) + 20)*0.05f;
		}
		else if (o->SubType == 0)
		{
			o->Gravity = 0.0f;
			o->Scale = (mu_float)(GetLargeRand(5) + 20)*0.1f;
		}

		o->LifeTime = 20 + GetLargeRand(3);
		if (o->SubType == 2)
		{
			o->LifeTime = 40 + GetLargeRand(3);
		}

		o->Position[0] += (mu_float)(GetLargeRand(50) - 25);
		o->Position[1] += (mu_float)(GetLargeRand(50) - 25);
		o->Position[2] += (mu_float)(GetLargeRand(200) - 100) + 250.0f;

		if (o->SubType == 4)
		{
			o->Gravity = 0.0f;
			o->Scale = (mu_float)(GetLargeRand(5) + 20)*0.1f*1.3f;
			o->Position[0] = Position[0] + (mu_float)(GetLargeRand(80) - 40)*1.3f;
			o->Position[2] -= 100.0f;
		}
		else if (o->SubType == 5)
		{
			o->TexType = BITMAP_EXT_LOG_IN + 2;
			o->Scale = (mu_float)(GetLargeRand(10) + 20)*0.01f*1.3f;
			o->Gravity = (mu_float)(GetLargeRand(10) + 20)*0.05f*1.3f;
			o->Position[2] -= 100.0f;
		}
		else if (o->SubType == 6)
		{
			o->Gravity = (mu_float)(GetLargeRand(20) + 80)*0.1f;
			o->Scale = (mu_float)(GetLargeRand(5) + 20)*0.1f;

			o->Position[2] -= 200.0f;
		}
		else if (o->SubType == 7)
		{
			o->TexType = BITMAP_EXT_LOG_IN + 2;
			o->Scale = (mu_float)(GetLargeRand(10) + 20)*0.01f;
			o->Gravity = (mu_float)(GetLargeRand(20) + 80)*0.1f;

			o->Position[2] -= 100.0f;
		}
		break;

	case BITMAP_FLOWER01:
	case BITMAP_FLOWER01 + 1:
	case BITMAP_FLOWER01 + 2:
		o->LifeTime = 15 + GetLargeRand(10);
		o->Scale = (mu_float)(GetLargeRand(8) + 4)*0.03f;
		Vector(1.0f, 1.0f, 1.0f, o->Light);
		o->Velocity[0] = (mu_float)(GetLargeRand(32) - 16)*0.1f;
		o->Velocity[1] = (mu_float)(GetLargeRand(32) - 16)*0.1f;
		o->Velocity[2] = (mu_float)(GetLargeRand(32) - 32)*0.1f;
		break;
	case BITMAP_FLARE_BLUE:
		if (o->SubType == 0)
		{
			o->LifeTime = 30 + GetLargeRand(10);
			Vector(1.0f, 1.0f, 1.0f, o->Light);
			Vector(0.0f, 0.0f, (GetLargeRand(100)) / 50.0f, o->Velocity);
		}
		else if (o->SubType == 1)
		{
			o->LifeTime = 26 + (GetLargeRand(2));
			o->Gravity = 0;
			o->Velocity[0] = 0;
			o->Scale = Scale + (mu_float)(GetLargeRand(6))*0.1f;
			o->Rotation = (mu_float)(GetLargeRand(360));
		}
		break;
	case BITMAP_FLARE + 1:
		if (o->SubType == 0)
		{
			o->LifeTime = 110 + GetLargeRand(10);
			o->Gravity = ((GetLargeRand(100)) / 100.0f)*2.0f + 1.0f;
			o->Velocity[0] = (mu_float)(GetLargeRand(100) - 50);
			o->Scale = Scale + (mu_float)(GetLargeRand(2))*0.01f;
			o->Rotation = (mu_float)(GetLargeRand(360));

			Vector(0.0f, 0.0f, 0.0f, o->Angle);

			VectorCopy(o->Position, o->StartPosition);
		}
		break;
	case BITMAP_BUBBLE:
		switch (o->SubType)
		{
		case 0:
		case 1:
			o->LifeTime = 30 + GetLargeRand(10);
			o->Scale = (mu_float)(GetLargeRand(6) + 4)*0.03f;
			Vector(1.0f, 1.0f, 1.0f, o->Light);
			break;
		case 2:
			o->LifeTime = 30 + GetLargeRand(10);
			o->Scale = (mu_float)(GetLargeRand(6) + 4)*0.03f;
			Vector(1.0f, 1.0f, 1.0f, o->Light);
			PlayBackground(SOUND_DEATH_BUBBLE);
			break;
		case 3:
			o->LifeTime = 30 + GetLargeRand(10);
			o->Scale = (mu_float)(GetLargeRand(6) + 4)*0.03f + Scale;
			o->Gravity = (mu_float)(GetLargeRand(6) + 4)*0.03f;
			Vector(1.0f, 1.0f, 1.0f, o->Light);
			break;
		case 4:
			o->LifeTime = 30 + GetLargeRand(10);
			o->Scale = (mu_float)(GetLargeRand(6) + 4)*0.03f;
			Vector(1.0f, 1.0f, 1.0f, o->Light);
			break;
		case 5:
			o->LifeTime = 30 + GetLargeRand(10);
			o->Scale = (mu_float)(GetLargeRand(6) + 4)*0.04f;
			Vector(1.0f, 1.0f, 1.0f, o->Light);
			break;
		}
		break;
	case BITMAP_LIGHTNING + 1:
		switch (o->SubType)
		{
		case 0:
			o->Scale = 0.15f;
			break;
		case 4:
			o->Scale = 0.25f;
			break;
		case 1:
			o->LifeTime = 10;
			o->Scale = 0.0f;
			break;
		case 2:
			o->LifeTime = 20;
			o->Scale = 1.0f;
			break;
		case 3:
			o->LifeTime = 1;
			o->Scale = Scale;
			break;
		case 5:
			o->Scale = Scale;
			break;
		}

		if (o->SubType == 4)
		{
			VectorCopy(Light, o->Light);
		}
		else if (o->SubType == 5)
		{
			VectorCopy(Light, o->Light);
		}
		else
		{
			Vector(Light[0] + 0.5f, Light[1] + 0.5f, Light[2] + 0.5f, o->Light);
		}
		break;
	case BITMAP_LIGHTNING:
		o->LifeTime = 10;
		o->Scale = 1.8f;
		o->Angle[0] = 30.0f;
		Vector((mu_float)(GetLargeRand(4) + 6)*0.1f, (mu_float)(GetLargeRand(4) + 6)*0.1f, (mu_float)(GetLargeRand(4) + 6)*0.1f, o->Light);
		o->Position[2] += 260.0f;
		break;
	case BITMAP_CHROME_ENERGY2:
		o->LifeTime = 8;
		Vector(0.0f, 0.0f, 0.0f, o->Velocity);
		o->Scale = Scale*(mu_float)(GetLargeRand(64) + 128)*0.01f;
		o->Rotation = (mu_float)(GetLargeRand(360));
		break;
	case BITMAP_FIRE_CURSEDLICH:
	case BITMAP_FIRE_HIK2_MONO:
		if (o->SubType == 0)
		{
			o->LifeTime = (GetLargeRand(12) + 8);
			Vector(0.0f, 0.0f, 0.0f, o->Velocity);
			o->Scale = (mu_float)(GetLargeRand(30) + 20)*0.01f;
			o->Rotation = (mu_float)(GetLargeRand(360));
			o->Gravity = (GetLargeRand(15) + 15)*0.01f;
		}
		else if (o->SubType == 1)
		{
			o->Position[0] += (GetLargeRand(10) - 5)*0.2f;
			o->Position[1] += (GetLargeRand(10) - 5)*0.2f;
			o->Position[2] += (GetLargeRand(10) - 5)*0.2f;
			o->LifeTime = (GetLargeRand(28) + 4);
			Vector(0.0f, 0.0f, 0.0f, o->Velocity);
			o->Scale = (mu_float)(GetLargeRand(5) + 50)*0.016f;
			o->Rotation = (mu_float)(GetLargeRand(360));
			o->Gravity = (GetLargeRand(5) + 10)*0.01f;
			VectorCopy(o->Target->Position, o->StartPosition);
		}
		else if (o->SubType == 2)
		{
			o->LifeTime = (GetLargeRand(12) + 8);
			Vector(0.0f, 0.0f, 0.0f, o->Velocity);
			o->Scale = (mu_float)(GetLargeRand(30) + 20)*0.01f*Scale;
			o->Rotation = (mu_float)(GetLargeRand(360));
			o->Gravity = (GetLargeRand(15) + 15)*0.01f;
		}
		else if (o->SubType == 3)
		{
			o->LifeTime = (GetLargeRand(12) + 16);
			Vector(0.0f, 0.0f, 0.0f, o->Velocity);
			o->Scale = (mu_float)(GetLargeRand(30) + 20)*0.02f;
			o->Rotation = (mu_float)(GetLargeRand(360));
			o->Gravity = (GetLargeRand(15) + 15)*0.02f;
			VectorCopy(o->Target->Position, o->StartPosition);
		}
		else if (o->SubType == 4)
		{
			o->LifeTime = GetLargeRand(5) + 12;
			o->Scale = (mu_float)(GetLargeRand(72) + 72)*0.01f*Scale;
			o->Rotation = (mu_float)(GetLargeRand(360));
			o->Gravity = (mu_float)(GetLargeRand(24) + 64)*0.1f;
			o->Alpha = 0;
			VectorCopy(Light, o->TurningForce);
			Vector(0, 0, 0, o->Light);
		}
		else if (o->SubType == 5)
		{
			o->LifeTime = GetLargeRand(5) + 24;
			o->Scale = (mu_float)(GetLargeRand(72) + 72)*0.01f*Scale;
			o->Rotation = (mu_float)(GetLargeRand(360));
			o->Gravity = (mu_float)(GetLargeRand(24) + 74)*0.1f;
			o->Alpha = 0;
			VectorCopy(Light, o->TurningForce);
			Vector(0, 0, 0, o->Light);
		}
		else if (o->SubType == 6)
		{
			o->LifeTime = GetLargeRand(5) + 24;
			o->Scale = (mu_float)(GetLargeRand(72) + 72)*0.01f*Scale;
			o->Rotation = (mu_float)(GetLargeRand(360));
			o->Gravity = (mu_float)(GetLargeRand(24) + 100)*0.1f;
			VectorCopy(o->Angle, o->Velocity);
			mu_float fRand = (mu_float)(GetLargeRand(50)) * 0.03f;
			Vector(2.5f + fRand, -5.0f - fRand, 0.0f, o->Velocity);
			o->Alpha = 0;
			VectorCopy(Light, o->TurningForce);
			Vector(0, 0, 0, o->Light);
		}
		else if (o->SubType == 7)
		{
			o->LifeTime = GetLargeRand(5) + 12;
			o->Scale = (mu_float)(GetLargeRand(72) + 52)*0.01f*Scale;
			o->Rotation = (mu_float)(GetLargeRand(360));
			o->Gravity = (mu_float)(GetLargeRand(14) + 44)*0.1f;
			o->Alpha = 0;
			VectorCopy(Light, o->TurningForce);
			Vector(0, 0, 0, o->Light);
		}
		else if (o->SubType == 8)
		{
			o->LifeTime = 8;
			o->Scale = (mu_float)(GetLargeRand(42) + 12)*0.01f*Scale;
			o->Rotation = (mu_float)(GetLargeRand(360));
			o->Gravity = (mu_float)(GetLargeRand(14) + 44)*0.1f;
			o->Alpha = 0;
			VectorCopy(Light, o->TurningForce);
			Vector(0, 0, 0, o->Light);
			VectorCopy(o->Target->Position, o->StartPosition);
		}
		break;
	case BITMAP_LEAF_TOTEMGOLEM:
		o->LifeTime = GetLargeRand(10) + 40;
		Vector(0.0f, 0.0f, 0.0f, o->Velocity);
		o->Scale = (mu_float)(GetLargeRand(20) + 10)*0.04f;
		o->Rotation = (mu_float)(GetLargeRand(360));
		o->Gravity = (GetLargeRand(40) + 30)*-0.05f;
		o->Velocity[0] = GetLargeRand(10) - 5;
		o->Velocity[1] = GetLargeRand(10) - 5;
		o->Velocity[2] = GetLargeRand(5) + 10;
		o->Alpha = 1.0f;
		break;
	case BITMAP_FIRE:
	case BITMAP_FIRE + 2:
	case BITMAP_FIRE + 3:
		{
			switch (o->SubType)
			{
			case 0:
				{
					o->LifeTime = 24;
					Vector(0.0f, -(mu_float)(GetLargeRand(16) + 32)*0.1f, 0.0f, o->Velocity);
					o->Scale = (mu_float)(GetLargeRand(64) + 128)*0.01f;
					o->Rotation = (mu_float)(GetLargeRand(360));
				}break;
			case 1:
				{
					o->LifeTime = 24;
					Vector(0.0f, -(mu_float)(GetLargeRand(16) + 32)*0.1f, 0.0f, o->Velocity);
					o->Scale = (mu_float)(GetLargeRand(4) + 10)*0.01f;
					o->Rotation = (mu_float)(GetLargeRand(360));
				}break;
			case 17:
			case 5:
				{
					if (o->SubType == 17)
						o->LifeTime = 10;
					else
						o->LifeTime = 24;
					Vector(0.0f, -(mu_float)(GetLargeRand(16) + 32)*0.1f, 0.0f, o->Velocity);
					o->Scale = Scale*(mu_float)(GetLargeRand(64) + 128)*0.01f;
					o->Rotation = (mu_float)(GetLargeRand(360));
				}break;
			case 7:
				{
					o->LifeTime = 24;
					Vector(0.0f, -(mu_float)(GetLargeRand(16) + 32)*0.1f, 0.0f, o->Velocity);
					Vector(0.0f, -(mu_float)(GetLargeRand(32) - 16)*0.1f, 0.0f, o->Velocity);
					o->Scale = (mu_float)(GetLargeRand(64) + 128)*0.008f + Scale;
					o->Rotation = (mu_float)(GetLargeRand(360));
				}break;
			case 9:
				{
					o->LifeTime = 24;
					Vector(0.0f, -(mu_float)(GetLargeRand(16) + 32)*0.1f, 0.0f, o->Velocity);
					mu_int32 range = GetLargeRand(60) - 30;
					o->StartPosition[0] = (mu_float)range;
					o->StartPosition[1] = (mu_float)range;
					o->StartPosition[2] = 190.0f - abs(range)*1.5f;
					o->Rotation = (mu_float)(GetLargeRand(360));
				}break;
			case 10:
				{
					o->LifeTime = 24;
					Vector(0.0f, -(mu_float)(GetLargeRand(16) + 32)*0.1f, 0.0f, o->Velocity);
					o->Rotation = (mu_float)(GetLargeRand(360));
				}break;
			case 11:
				{
					o->LifeTime = 24;
					Vector(0.0f, -(mu_float)(GetLargeRand(32) - 16)*0.1f, 0.0f, o->Velocity);
					o->Scale = (mu_float)(GetLargeRand(64) + 128)*0.008f + Scale;
					o->Rotation = (mu_float)(GetLargeRand(360));
				}break;
			case 12:
				{
					o->LifeTime = 24;
					Vector(0.0f, -(mu_float)(GetLargeRand(16) + 32)*0.1f, 0.0f, o->Velocity);
					o->Rotation = (mu_float)(GetLargeRand(360));
					o->Scale = Scale*(mu_float)(GetLargeRand(16) + 150)*0.012f;
				}break;
			case 13:
				{
					o->LifeTime = 20;
					Vector(0.0f, 0.0f, (mu_float)(GetLargeRand(128) + 128)*0.15f, o->Velocity);
					o->Rotation = (mu_float)(GetLargeRand(360));
					o->Scale = (mu_float)(GetLargeRand(16) + 150)*0.012f;
				}break;
			case 14:
				{
					o->LifeTime = 24;
					o->Scale = 1.5f;
					o->TexType = BITMAP_CLUD64;
					Vector(0.0f, 0.0f, 0.0f, o->Velocity);
					o->Rotation = (mu_float)(GetLargeRand(360));
				}break;
			case 15:
				{
					o->LifeTime = 10;
					o->Scale = (mu_float)(GetLargeRand(64) + 128)*0.01f;
					o->TexType = BITMAP_CLUD64;
					Vector(0.0f, 0.0f, 0.0f, o->Velocity);
					o->Rotation = (mu_float)(GetLargeRand(360));
				}break;
			case 16:
				{
					o->LifeTime = 4;
				}break;
			default:
				{
					o->LifeTime = 24;
					Vector(0.0f, -(mu_float)(GetLargeRand(32) - 16)*0.1f, 0.0f, o->Velocity);
					o->Rotation = (mu_float)(GetLargeRand(360));
				}
			}
		}
		break;
	case BITMAP_FIRE + 1:
		if (o->SubType == 1)
		{
			o->LifeTime = 3;
			Vector(0.0f, -(mu_float)(GetLargeRand(8) + 32)*0.3f, 0.0f, o->Velocity);
			//o->Scale = 0.8f;
		}
		else if (o->SubType == 2)
		{
			o->LifeTime = 5;
			Vector(0.0f, -(mu_float)(GetLargeRand(8) + 32)*0.1f, 0.0f, o->Velocity);
			//o->Scale = 0.8f;
		}
		else if (o->SubType == 3)
		{
			o->LifeTime = 7;
			Vector(0.0f, -(mu_float)(GetLargeRand(8) + 32)*0.1f, 0.0f, o->Velocity);
			//o->Scale = 0.8f;
		}
		else if (o->SubType == 4)
		{
			o->LifeTime = 100;
			o->Gravity = ((GetLargeRand(100)) / 100.0f)*4.0f + 1.0f;
			o->Velocity[0] = (mu_float)(GetLargeRand(300) - 150);
			o->Scale = Scale + (mu_float)(GetLargeRand(6))*0.15f;
			o->Rotation = (mu_float)(GetLargeRand(360));

			Vector(0.0f, 0.0f, 0.0f, o->Angle);
			VectorCopy(o->Position, o->StartPosition);
		}
		else if (o->SubType == 5)
		{
			o->LifeTime = 6;
			o->Gravity = ((GetLargeRand(100)) / 100.0f)*4.0f + o->Angle[0] * 1.2f;//45.0f;
			o->Scale = Scale + (mu_float)(GetLargeRand(6))*0.20f;
			o->Rotation = (mu_float)(GetLargeRand(360));

			VectorCopy(o->Position, o->StartPosition);
		}
		else if (o->SubType == 6)
		{
			o->LifeTime = 6;
			o->Gravity = ((GetLargeRand(100)) / 100.0f)*4.0f + 5.0f;
			o->Scale = Scale + (mu_float)(GetLargeRand(6))*0.10f;
			o->Rotation = (mu_float)(GetLargeRand(360));

			VectorCopy(o->Position, o->StartPosition);
		}
		else if (o->SubType == 7)
		{
			o->LifeTime = 10;
		}
		else if (o->SubType == 8)
		{
			o->Position[0] += (mu_float)(GetLargeRand(20) - 10);
			o->Position[1] += (mu_float)(GetLargeRand(20) - 10);
			o->Position[2] += (mu_float)(GetLargeRand(20) - 10);
			o->LifeTime = 32;
			o->Rotation = (mu_float)(GetLargeRand(360));
			o->Gravity = (mu_float)(GetLargeRand(5) + 5) / 5.0f;
		}
		else if (o->SubType == 9)
		{
			o->TexType = BITMAP_LIGHT + 2;
			o->LifeTime = 32;
			o->Rotation = (mu_float)(GetLargeRand(360));
			o->Gravity = (mu_float)(GetLargeRand(5) + 5) / 10.0f;
		}
		else if (o->SubType == 0)
		{
			o->LifeTime = 12;
			Vector(0.0f, -(mu_float)(GetLargeRand(8) + 32)*0.3f, 0.0f, o->Velocity);
			o->Scale = 0.8f;
		}
		o->Rotation = (mu_float)(GetLargeRand(360));
		break;

	case BITMAP_FLAME:
		switch (o->SubType)
		{
		case 0:
			o->LifeTime = 20;
			Vector(0.0f, 0.0f, (mu_float)(GetLargeRand(128) + 128)*0.15f, o->Velocity);
			o->Scale = Scale*(mu_float)(GetLargeRand(64) + 64)*0.01f;
			break;
		case 6:
			o->LifeTime = 10;
			Vector(0.0f, 0.0f, (mu_float)(GetLargeRand(128) + 256)*0.12f, o->Velocity);
			o->Scale = Scale*(mu_float)(GetLargeRand(64) + 64)*0.01f;
			break;
		case 8:
		case 7:
			{
				o->LifeTime = 33;
				o->Scale = -(mu_float)(GetLargeRand(20)) / 100.0f + o->Scale;
				o->Rotation = (mu_float)(GetLargeRand(360)) - 180.0f;
				o->StartPosition[0] = o->Position[0];
				o->Position[0] += ((mu_float)(GetLargeRand(2)) * 0.5f - 0.0f) * o->Scale;
				o->Gravity = ((mu_float)(GetLargeRand(100)) / 100.0f + 1.8f) * o->Scale;
				mu_float fTemp = 0.0f;
				if (GetLargeRand(10) >= 3)
					fTemp = ((mu_float)(GetLargeRand(20)) / 10.0f - 1.0f) * o->Scale;
				Vector(0.0f, fTemp, 0.0f, o->Velocity);
				o->Position[1] += o->Position[0] - o->StartPosition[0];
			}
			break;
		case 1:
			o->LifeTime = 15;
			o->Scale += (mu_float)(GetLargeRand(32) + 32)*0.01f;
			Vector(0.0f, (mu_float)(GetLargeRand(4) + 4)*0.15f, 0.0f, o->Velocity);
			break;
		case 5:
			o->LifeTime = 4;
			Vector(0.0f, (mu_float)(GetLargeRand(4) + 4)*0.15f, 0.0f, o->Velocity);
			break;
		case 2:
			{
				mu_float Luminosity;

				o->LifeTime = 10;
				o->Scale += (mu_float)(GetLargeRand(32) + 32)*0.01f;

				mu_float inter = Light[0] * (GetLargeRand(80)) / 100.0f;
				Vector(0.0f, inter, 0.0f, o->Velocity);
				MovePosition(o->Position, o->Angle, o->Velocity);

				inter = (Light[0] - inter) / 15.0f;
				Vector(0.0f, inter, 0.0f, o->Velocity);

				Luminosity = (mu_float)EMath::Sin(FWorldTime*0.002f)*0.3f + 0.7f;
				Vector(Luminosity, Luminosity*0.5f, Luminosity*0.5f, o->Light);
			}
			break;

		case 3:
			{
				mu_float Luminosity;

				o->LifeTime = 10;
				o->Scale += (mu_float)(GetLargeRand(32) + 32)*0.01f;
				Vector(0.0f, 0.0f, 0.0f, o->Velocity);

				Luminosity = (mu_float)EMath::Sin(FWorldTime*0.002f)*0.3f + 0.7f;
				Vector(Luminosity, Luminosity*0.5f, Luminosity*0.5f, o->Light);
			}
			break;
		case 4:
			{
				mu_float Luminosity;

				o->LifeTime = 5;
				o->Scale += (mu_float)(GetLargeRand(32) + 32)*0.01f;
				o->Gravity = 10.0f;
				VectorCopy(o->Target->Position, o->StartPosition);

				Luminosity = (mu_float)EMath::Sin(FWorldTime*0.002f)*0.3f + 0.7f;
				Vector(Luminosity, Luminosity*0.5f, Luminosity*0.5f, o->Light);
			}
			break;
		case 9:
			{
				o->LifeTime = 40;
				o->Scale = -(mu_float)(GetLargeRand(20)) / 100.0f + o->Scale;
				o->Rotation = (mu_float)(GetLargeRand(360)) - 180.0f;
				o->StartPosition[0] = o->Position[0];
				o->Position[0] += ((mu_float)(GetLargeRand(2)) * 0.5f - 0.0f) * o->Scale;
				o->Gravity = ((mu_float)(GetLargeRand(100)) / 100.0f + 1.8f) * o->Scale + 2.0f;
				mu_float fTemp = 0.0f;
				if (GetLargeRand(10) >= 3)
				{
					fTemp = ((mu_float)(GetLargeRand(20)) / 10.0f - 1.0f) * o->Scale;
				}
				Vector(0.0f, fTemp*2.0f, 0.0f, o->Velocity);
				o->Position[1] += o->Position[0] - o->StartPosition[0];
			}
			break;
		case 10:
			o->LifeTime = 20 + GetLargeRand(5);
			o->Rotation = 0.0f;
			o->Scale = Scale*0.5f + (mu_float)(GetLargeRand(10))*0.02f;
			o->Velocity[0] = (mu_float)(GetLargeRand(10) + 5)*0.4f;
			o->Velocity[1] = (mu_float)(GetLargeRand(10) - 5)*0.4f;
			o->Velocity[2] = (mu_float)(GetLargeRand(10) + 5)*0.2f;
			break;
		case 11:
			o->LifeTime = 20;
			o->Rotation = (mu_float)(GetLargeRand(360));
			o->TexType = BITMAP_FIRE_HIK2_MONO;
			Vector(0.0f, 0.0f, (mu_float)(GetLargeRand(128) + 128)*0.15f, o->Velocity);
			o->Scale = Scale*(mu_float)(GetLargeRand(64) + 64)*0.01f;
			break;
		case 12:
			o->LifeTime = 10;
			Vector(0.0f, 0.0f, (mu_float)(GetLargeRand(128) + 128)*0.15f, o->Velocity);
			o->Scale = Scale*(mu_float)(GetLargeRand(64) + 64)*0.01f;
			break;
		}
		break;

	case BITMAP_FIRE_RED:
		o->LifeTime = 20;
		Vector(0.0f, 0.0f, (mu_float)(GetLargeRand(100) + 100)*0.15f, o->Velocity);
		o->Scale = Scale*(mu_float)(GetLargeRand(64) + 64)*0.01f;
		break;

	case BITMAP_RAIN_CIRCLE:
	case BITMAP_RAIN_CIRCLE + 1:
		o->LifeTime = 20;
		o->Scale = (GetLargeRand(6) + 8)*0.1f;
		if (o->Type == BITMAP_RAIN_CIRCLE + 1 || o->SubType == 1)
		{
			o->Position[0] += (GetLargeRand(10) - 5)*1.0f;
			o->Position[1] += (GetLargeRand(10) - 5)*1.0f;
			o->Position[2] += (GetLargeRand(10) - 5)*1.0f;
		}
		if (o->SubType == 1)
		{
			o->Scale = Scale;
			o->Gravity = GetLargeRand(100) / 1000.0f;
			o->Velocity[1] = 1.0f;
		}
		break;

	case BITMAP_ENERGY:
		o->Scale = Scale*(mu_float)(GetLargeRand(8) + 6)*0.1f;
		o->Rotation = (mu_float)(GetLargeRand(360));
		o->Gravity = 20.0f;

		if (o->SubType == 1)
		{
			o->LifeTime = 10;
			Vector(0.5f, 0.5f, 0.5f, o->Light);
		}
		else if (o->SubType == 2)
		{
			o->TexType = BITMAP_MAGIC + 1;
			o->LifeTime = 10;
			o->Scale = 0.3f + Scale*(mu_float)(GetLargeRand(8) + 6)*0.1f;
			VectorCopy(o->Light, Light);
			o->Rotation = 0;
			o->Gravity = 0;
		}
		else if (o->SubType == 3 || o->SubType == 4 || o->SubType == 5)
		{
			o->LifeTime = 25;
		}
		else if (o->SubType == 6)
		{
			o->LifeTime = 20;
		}
		break;

	case BITMAP_MAGIC:
		{
			if (o->SubType == 0)
			{
				o->LifeTime = 10;
				o->Scale = Scale;
			}
		}
		break;

	case BITMAP_FLARE:
		o->LifeTime = 60;
		if (o->SubType == 0 || o->SubType == 3 || o->SubType == 6
			|| o->SubType == 10
			)
		{
			o->Gravity = ((GetLargeRand(100)) / 100.0f)*4.0f + 1.0f;
		}
		else if (o->SubType == 2)
		{
			o->Gravity = ((GetLargeRand(100)) / 100.0f)*5.0f + 5.0f;
		}
		else if (o->SubType == 4)
		{
			o->LifeTime = 40;
			o->Gravity = ((GetLargeRand(100)) / 100.0f)*5.0f + 1.0f;
		}
		else if (o->SubType == 5)
		{
			o->LifeTime = 40;
			o->Gravity = ((GetLargeRand(100)) / 100.0f)*5.0f + 1.0f;
		}
		o->Velocity[0] = (mu_float)(GetLargeRand(300) - 150);
		o->Scale = Scale + (mu_float)(GetLargeRand(6))*0.01f;
		o->Rotation = (mu_float)(GetLargeRand(360));

		Vector(0.0f, 0.0f, 0.0f, o->Angle);

		VectorCopy(o->Position, o->StartPosition);
		if (o->SubType == 10)
		{
			mu_float count = (o->Velocity[0] + o->LifeTime)*0.1f;
			o->StartPosition[0] = o->StartPosition[0] + EMath::Sin(count)*40.0f;
			o->StartPosition[1] = o->StartPosition[1] - EMath::Cos(count)*40.0f;
		}
		else if (o->SubType == 11)
		{
			o->LifeTime = 26 + (GetLargeRand(2));
			o->Gravity = 0;
			o->Velocity[0] = 0;
			o->Scale = Scale + (mu_float)(GetLargeRand(6))*0.1f;
			o->Rotation = (mu_float)(GetLargeRand(360));
		}
		else
			if (o->SubType == 12)
			{
				o->LifeTime = 80;
				o->Gravity = ((GetLargeRand(100)) / 100.0f)*4.0f + 1.0f;
				Vector(0.2f, 0.6f, 1.0f, o->Light);
			}
		break;

	case BITMAP_LIGHT + 2:
		if (o->SubType == 0)
		{
			o->LifeTime = 16;
			o->Scale += (mu_float)(GetLargeRand(32) + 48)*0.01f;
			o->Angle[0] = (mu_float)(GetLargeRand(360));
			o->Rotation = (mu_float)(IWorldTime % 360);
		}
		else if (o->SubType == 1)
		{
			o->TexType = BITMAP_SMOKE;
			o->LifeTime = 20;
			o->Scale += (mu_float)(GetLargeRand(10) + 10)*0.01f;
			o->Angle[0] = (mu_float)(GetLargeRand(360));
			o->Rotation = (mu_float)(IWorldTime % 360);
			o->Gravity = (mu_float)(GetLargeRand(10) + 40)*0.04f;
			o->Position[1] += 10.0f;
		}
		else if (o->SubType == 2)
		{
			o->LifeTime = 10;
			o->Scale -= (mu_float)(GetLargeRand(40) + 48)*0.008f;
			o->Angle[0] = (mu_float)(GetLargeRand(360));
			o->Rotation = (mu_float)(IWorldTime % 360);
			o->Gravity = 0.0f;
		}
		else if (o->SubType == 3)
		{
			o->LifeTime = 20;
			o->Scale += (mu_float)(GetLargeRand(32) + 48)*0.008f;
			o->Angle[0] = (mu_float)(GetLargeRand(360));
			o->Rotation = (mu_float)(IWorldTime % 360);
			VectorCopy(o->Target->Position, o->StartPosition);
		}
		else if (o->SubType == 4)
		{
			o->LifeTime = 16;
			o->Scale += (mu_float)(GetLargeRand(32) + 48)*0.01f;
			o->Angle[0] = (mu_float)(GetLargeRand(360));
			o->Rotation = (mu_float)(IWorldTime % 360);
		}
		else if (o->SubType == 5)
		{
			o->TexType = BITMAP_ADV_SMOKE;

			MUModel * pModel = &MODELS::Data[o->Target->Type];
			mu_int32 iNumBones = pModel->_BoneNonDummyEnd;

			EVector3 vRelative, p1;
			Vector(0.0f, 0.0f, 0.0f, vRelative);
			pModel->TransformPosition(ThreadIndex, o->Target->BoneTransform[GetLargeRand(iNumBones)], vRelative, o->Position, false);
			VectorScale(o->Position, pModel->BodyScale(ThreadIndex), o->Position);
			VectorAdd(o->Position, o->Target->Position, o->Position);

			o->Velocity[0] = 0;
			o->Velocity[1] = 0;
			o->Velocity[2] = 0;

			o->LifeTime = 21;
			o->Scale = (mu_float)(GetLargeRand(4) + 8)*0.01f;
			o->Gravity = (mu_float)(GetLargeRand(1) + 4)*0.1f;

			o->Angle[2] = (mu_float)(GetLargeRand(360));
			AngleMatrix(o->Angle, Matrix);
			Vector(0.0f, (mu_float)(GetLargeRand(2) + 60)*0.1f, 0.0f, p1);
			VectorRotate(p1, Matrix, o->TurningForce);

			o->TurningForce[2] += 4.0f;
			o->Rotation = (mu_float)(IWorldTime % 360);
			o->Alpha = 1.0f;
		}
		else if (o->SubType == 6)
		{
			o->LifeTime = 21;
			o->Scale = Scale + (mu_float)(GetLargeRand(4) + 8)*0.01f;
			o->Gravity = (mu_float)(GetLargeRand(1) + 1)*0.1f;

			o->Angle[2] = (mu_float)(GetLargeRand(360));
			AngleMatrix(o->Angle, Matrix);
			EVector3 p1;
			Vector(0.0f, (mu_float)(GetLargeRand(2) + 60)*0.1f, 0.0f, p1);
			VectorRotate(p1, Matrix, o->TurningForce);

			o->TurningForce[2] += 2.0f;
			o->Rotation = (mu_float)(IWorldTime % 360);
			o->Alpha = 1.0f;

			o->Velocity[0] = 0;
			o->Velocity[1] = 0;
			o->Velocity[2] = 0;
		}
		else if (o->SubType == 7)
		{
			o->LifeTime = 18;
			o->Scale = Scale + (mu_float)(GetLargeRand(4) + 8)*0.01f;
			o->Gravity = (mu_float)(GetLargeRand(1) + 1)*0.1f;

			o->Angle[2] = (mu_float)(GetLargeRand(360));
			AngleMatrix(o->Angle, Matrix);
			EVector3 p1;
			Vector(0.0f, (mu_float)(GetLargeRand(2) + 60)*0.1f, 0.0f, p1);
			VectorRotate(p1, Matrix, o->TurningForce);

			o->TurningForce[2] += 2.0f;
			o->Rotation = (mu_float)(IWorldTime % 360);
			o->Alpha = 1.0f;

			o->Velocity[0] = 0;
			o->Velocity[1] = 0;
			o->Velocity[2] = 0;
		}
		break;

	case BITMAP_MAGIC + 1:
		o->LifeTime = 10;
		o->Scale += (mu_float)(GetLargeRand(32) + 48)*0.001f;
		o->Angle[0] = (mu_float)(GetLargeRand(360));
		o->Rotation = (mu_float)(IWorldTime % 360);
		break;
	case BITMAP_BLUE_BLUR:
		switch (SubType)
		{
		case 0:
			o->LifeTime = 30;
			o->Scale = (mu_float)(GetLargeRand(64) + 64)*0.01f;
			o->Rotation = (mu_float)(GetLargeRand(360));
			o->Gravity = (mu_float)(GetLargeRand(32) + 60)*0.1f;
			break;
		case 1:
			o->LifeTime = 30;
			o->Scale = (mu_float)(GetLargeRand(64) + 64)*0.01f;
			o->Rotation = (mu_float)(GetLargeRand(360));
			o->Gravity = (mu_float)(GetLargeRand(32) + 60)*0.1f;
			o->Position[2] -= 45.00f;
			o->TexType = BITMAP_POUNDING_BALL;
			break;
		}
		break;

	case BITMAP_CLUD64:
		{
			if (o->SubType == 0 || o->SubType == 2)
			{
				if (GetLargeRand(4) != 0)
					o->TexType = BITMAP_SMOKE;

				o->LifeTime = 40;
				o->Scale = (mu_float)(GetLargeRand(20) + 250)*0.01f;
				o->Rotation = (mu_float)(GetLargeRand(360));
				o->Position[0] += (mu_float)(GetLargeRand(20) - 10);
				o->Position[1] += (mu_float)(GetLargeRand(20) - 10);
				o->Position[2] -= 20.0f;
				o->Gravity = (mu_float)(GetLargeRand(10) + 5)*0.1f;
			}
			else if (o->SubType == 1)
			{
				o->LifeTime = 12;
				o->Scale += (mu_float)(GetLargeRand(30))*0.01f;
				o->Rotation = (mu_float)(GetLargeRand(360));
				o->Position[2] += 30.0f;
			}
			else if (o->SubType == 3)
			{
				if (GetLargeRand(2) != 0)
					o->TexType = BITMAP_SMOKE;
				o->LifeTime = 50;
				o->Scale = Scale + (mu_float)(GetLargeRand(10) + 10)*0.01f;
				o->Rotation = (mu_float)(GetLargeRand(360));
				o->Position[0] += (mu_float)(GetLargeRand(20) - 10);
				o->Position[1] += (mu_float)(GetLargeRand(20) - 10);
				o->Position[2] -= 20.0f;
				o->Gravity = (mu_float)(GetLargeRand(10) + 5)*0.1f;
			}
			else if (o->SubType == 4)
			{
				if (GetLargeRand(2) != 0)
					o->TexType = BITMAP_SMOKE;
				o->LifeTime = 4;
				o->Scale = Scale + (mu_float)(GetLargeRand(10) + 10)*0.01f;
				o->Rotation = (mu_float)(GetLargeRand(360));
				o->Position[0] += (mu_float)(GetLargeRand(20) - 10);
				o->Position[1] += (mu_float)(GetLargeRand(20) - 10);
				o->Position[2] -= 20.0f;
				o->Gravity = (mu_float)(GetLargeRand(10) + 5)*0.1f;
			}
			else if (o->SubType == 5)
			{
				if (GetLargeRand(2) != 0)
					o->TexType = BITMAP_SMOKE;
				o->LifeTime = 50;
				o->Scale = Scale + (mu_float)(GetLargeRand(10) + 10)*0.01f;
				o->Rotation = (mu_float)(GetLargeRand(360));
				o->Position[0] += (mu_float)(GetLargeRand(20) - 10);
				o->Position[1] += (mu_float)(GetLargeRand(20) - 10);
				o->Position[2] -= 20.0f;
				o->Gravity = (mu_float)(GetLargeRand(10) + 5)*0.1f;
			}
			else if (o->SubType == 6)
			{
				o->LifeTime = 25;
				o->Scale = (mu_float)(GetLargeRand(8) + 50)*0.01f*Scale;
				o->Rotation = (mu_float)(GetLargeRand(360));
				o->Gravity = (mu_float)(GetLargeRand(10) + 35)*0.1f;
				o->Alpha = 0;
				VectorCopy(Light, o->TurningForce);
				Vector(0, 0, 0, o->Light);
				VectorCopy(o->Target->Position, o->StartPosition);
			}
			else if (o->SubType == 7)
			{
				o->LifeTime = 40;
				o->Velocity[0] = (mu_float)(GetLargeRand(20) - 10)*0.1f;
				o->Velocity[1] = (mu_float)(GetLargeRand(20) - 10)*0.1f;
				o->Position[2] += 80.0f;
				o->Gravity = 0;
				o->Alpha = 0.5f;
			}
			else if (o->SubType == 8)
			{
				o->LifeTime = 20;
				o->Velocity[0] = (mu_float)(GetLargeRand(20) - 10)*0.1f;
				o->Velocity[1] = (mu_float)(GetLargeRand(20) - 10)*0.1f;
				o->Position[2] += 80.0f;
				o->Gravity = 0;
				o->Alpha = 0.5f;
			}
			else if (o->SubType == 9)
			{
				if (GetLargeRand(2) != 0)
					o->TexType = BITMAP_SMOKE;
				o->LifeTime = 40;
				o->Alpha = 0.5f;
				mu_float fIntervalScale = Scale*0.1f;
				o->Scale += ((mu_float((GetLargeRand(20)) - 10) * 0.5f)*((fIntervalScale * 0.5f)));
				o->Rotation = (mu_float)(GetLargeRand(360));
				o->Velocity[0] = (mu_float)(GetLargeRand(20) - 10)*0.03f;
				o->Velocity[1] = (mu_float)(GetLargeRand(20) - 10)*0.03f;
				o->Velocity[2] = -((1.2f) + ((mu_float)(GetLargeRand(20) - 10)*0.025f));
				o->Gravity = 2.0f + ((mu_float)(GetLargeRand(20) - 10)*0.05f);
			}
			else if (o->SubType == 10)
			{
				o->Position[0] += (GetLargeRand(10) - 5)*0.2f;
				o->Position[1] += (GetLargeRand(10) - 5)*0.2f;
				o->Position[2] += (GetLargeRand(10) - 5)*0.2f;
				o->LifeTime = (GetLargeRand(28) + 4);
				Vector(0.0f, 0.0f, 0.0f, o->Velocity);
				o->Scale = (mu_float)(GetLargeRand(5) + 70)*0.016f;
				o->Rotation = (mu_float)(GetLargeRand(360));
				o->Gravity = (GetLargeRand(5) + 10)*0.01f;
				VectorCopy(o->Target->Position, o->StartPosition);
			}
			else if (o->SubType == 11)
			{
				if (GetLargeRand(4) != 0)
					o->TexType = BITMAP_SMOKE;

				o->LifeTime = 30;
				o->Scale = Scale;
				o->Rotation = (mu_float)(GetLargeRand(360));
				o->Position[0] += (mu_float)(GetLargeRand(20) - 10);
				o->Position[1] += (mu_float)(GetLargeRand(20) - 10);
				o->Position[2] += (mu_float)(GetLargeRand(20) - 10);
				o->Gravity = -1.5f;
			}
		}
		break;

	case BITMAP_LIGHT + 3:
		{
			if (o->SubType == 0)
			{
				o->LifeTime = GetLargeRand(10) + 140;
				o->Gravity = GetLargeRand(10) + 5.0f;
				o->Scale = (GetLargeRand(20) + 20.0f) / 100.0f*Scale;
				o->Position[0] += (mu_float)(GetLargeRand(100) - 50);
				o->Position[1] += (mu_float)(GetLargeRand(100) - 50);
			}
			else if (o->SubType == 1)
			{
				o->Scale = 0.1f;
				o->StartPosition[0] = o->Scale;
				o->LifeTime = GetLargeRand(10) + 20;
				o->Position[0] += (mu_float)(GetLargeRand(50) - 25);
				o->Position[1] += (mu_float)(GetLargeRand(50) - 25);
				o->Position[2] += (mu_float)(GetLargeRand(50) - 25);

				o->Gravity = 1.0f;

				o->Angle[0] = (mu_float)(GetLargeRand(360));
				o->Angle[1] = (mu_float)(GetLargeRand(360));
				o->Angle[2] = (mu_float)(GetLargeRand(360));
				AngleMatrix(o->Angle, Matrix);
				Vector(0.5f, 0.5f, 0.5f, p);
				VectorScale(p, 0.8f, p);
				VectorRotate(p, Matrix, o->Velocity);

				o->Rotation = GetLargeRand(20) - 10;
			}
		}
		break;

	case BITMAP_TWINTAIL_WATER:
		{
			if (o->SubType == 0)
			{
				o->LifeTime = 50 + GetLargeRand(10);
				o->Scale = (mu_float)(GetLargeRand(32) + 140)*0.01f;
				o->Position[0] += (mu_float)(GetLargeRand(120) - 60);
				o->Position[1] += (mu_float)(GetLargeRand(100) - 50);
				o->Position[2] += (mu_float)(GetLargeRand(100));
			}
			else if (o->SubType == 1)
			{
				o->LifeTime = 20 + GetLargeRand(10);
				o->Scale = (mu_float)(GetLargeRand(32) + 50)*0.01f;
				o->Position[0] += (mu_float)(GetLargeRand(80) - 40);
				o->Position[1] += (mu_float)(GetLargeRand(80) - 40);
				o->Position[2] += (mu_float)(GetLargeRand(80));
			}
			else if (o->SubType == 2)
			{
				o->LifeTime = 60 + GetLargeRand(10);
				o->Scale = (mu_float)(GetLargeRand(32) + 50)*0.02f;
				o->Position[0] += (mu_float)(GetLargeRand(40) - 20);
				o->Position[1] += (mu_float)(GetLargeRand(40) - 20);
				o->Position[2] += (mu_float)(GetLargeRand(40));
			}
			o->Angle[0] = (mu_float)(GetLargeRand(360));
			o->Angle[2] = (mu_float)(GetLargeRand(360));
			o->Gravity = (mu_float)(GetLargeRand(10) + 20)*0.1f;
		}
		break;
	case BITMAP_SMOKE:
		switch (SubType)
		{
		case 0:
		case 61:
		case 4:
		case 9:
		case 23:
			o->LifeTime = 16;
			if (o->Type == MODEL_MONSTER01 + 159)
				o->Scale = (mu_float)(GetLargeRand(3) + 28)*0.01f;
			else
				o->Scale = (mu_float)(GetLargeRand(32) + 48)*0.01f;
			o->Angle[0] = (mu_float)(GetLargeRand(360));
			o->Rotation = (mu_float)(IWorldTime % 360);
			break;
		case 42:
			o->LifeTime = 16;
			o->Scale = (mu_float)(GetLargeRand(32) + 48)*0.006f;
			o->Angle[0] = (mu_float)(GetLargeRand(360));
			o->Rotation = (mu_float)(IWorldTime % 360);
			break;
		case 17:
			VectorCopy(o->Light, o->TurningForce);
			o->LifeTime = 12;
			o->Scale = Scale*(mu_float)(GetLargeRand(32) + 8)*0.01f;
			o->Angle[0] = (mu_float)(GetLargeRand(360));
			o->Rotation = (mu_float)(IWorldTime % 360);
			break;
		case 36:
		case 37:
			if (o->SubType == 37)
			{
				VectorCopy(o->Light, o->TurningForce);
				o->Position[2] -= 15.0f * o->TurningForce[2];
				o->Position[1] -= 15.0f * o->TurningForce[1];
				Vector(1.0f, 1.0f, 1.0f, o->Light);
			}
			else
				Vector(1.0f, 0.3f, 0.0f, o->Light);
			o->LifeTime = 25;
			o->Rotation = (mu_float)(GetLargeRand(360));
			o->Angle[0] = (mu_float)(GetLargeRand(360));
			Vector((mu_float)(GetLargeRand(50) - 25), (mu_float)(GetLargeRand(50) - 25), -(mu_float)(GetLargeRand(50)), o->StartPosition);
			break;
		case 38:
			o->TexType = BITMAP_WATERFALL_2;
			o->LifeTime = 35;
			o->Rotation = (mu_float)(GetLargeRand(360));
			break;
		case 34:
		case 35:
			Vector(1.0f, 0.2f, 0.2f, o->Light);
			o->LifeTime = 30;
			o->Rotation = (mu_float)(GetLargeRand(360));
			if (SubType == 35)
			{
				Vector(0.0f, -5.0f, 0.0f, o->Velocity);
			}
			else
			{
				Vector(0.0f, 0.0f, 0.0f, o->Velocity);
			}
			break;
		case 33:
			{
				o->LifeTime = GetLargeRand(150);
				o->Scale = Scale + 0.1f;
				o->Angle[0] = (mu_float)(GetLargeRand(90) - 45);
				o->Angle[2] = (mu_float)(GetLargeRand(360));
				Vector(0.0f, -(mu_float)(GetLargeRand(8)), 0.0f, o->Velocity);
			}
			break;
		case 32:
			{
				o->LifeTime = 10;
				o->Scale = (mu_float)(GetLargeRand(32) + 80)*0.01f;
				o->Angle[0] = (mu_float)(GetLargeRand(90) - 45);
				o->Angle[2] = (mu_float)(GetLargeRand(360));
				Vector(0.0f, -(mu_float)(GetLargeRand(8) + 40), 0.0f, o->Velocity);
			}
			break;
		case 10:
			o->LifeTime = 16;
			break;
		case 12:
			o->LifeTime = 20;
			o->Scale = (mu_float)(GetLargeRand(64) + 64)*0.01f;
			o->Rotation = (mu_float)(GetLargeRand(360));
			o->Gravity = (mu_float)(GetLargeRand(32) + 60)*0.1f;
			break;
		case 13:
			o->LifeTime = 20;
			o->Scale = (mu_float)(GetLargeRand(64) + 64)*0.01f;
			o->Rotation = (mu_float)(GetLargeRand(360));
			o->Gravity = (mu_float)(GetLargeRand(32) + 60)*0.1f;
			break;
		case 18:
			o->LifeTime = 20;
			o->Scale = (mu_float)(GetLargeRand(64) + 64)*0.01f;
			o->Rotation = (mu_float)(GetLargeRand(360));
			o->Gravity = (mu_float)(GetLargeRand(32) + 60)*0.1f;
			break;
		case 3:
			o->LifeTime = 10;
			o->Scale = (mu_float)(GetLargeRand(32) + 80)*0.01f;
			o->Angle[0] = (mu_float)(GetLargeRand(90) - 45);
			o->Angle[2] = (mu_float)(GetLargeRand(360));
			Vector(0.0f, -(mu_float)(GetLargeRand(8) + 40), 0.0f, o->Velocity);
			break;
		case 11:
		case 14:
			o->LifeTime = 50;
			o->Position[0] += (mu_float)(GetLargeRand(64) - 32);
			o->Position[1] += (mu_float)(GetLargeRand(64) - 32);
			o->Position[2] += (mu_float)(GetLargeRand(64) + 32);
			o->Angle[0] = (mu_float)(GetLargeRand(90) - 45);
			o->Angle[2] = (mu_float)(GetLargeRand(360));
			o->Rotation = (mu_float)(GetLargeRand(360));
			VectorCopy(o->Light, o->TurningForce);
			Vector(0.0f, -(mu_float)(GetLargeRand(8) + 40), 0.0f, o->Velocity);
			break;
		case 15:
			o->LifeTime = 80;
			o->Angle[0] = (mu_float)(GetLargeRand(360));
			o->Rotation = (mu_float)(IWorldTime % 360);
			break;
		case 53:
		case 56:
		case 1:
			o->LifeTime = 50;
			o->Scale = (mu_float)(GetLargeRand(32) + 80)*0.01f;
			o->Position[0] += (mu_float)(GetLargeRand(64) - 32);
			o->Position[1] += (mu_float)(GetLargeRand(64) - 32);
			o->Position[2] += (mu_float)(GetLargeRand(64) + 32);
			o->Angle[0] = (mu_float)(GetLargeRand(90) - 45);
			o->Angle[2] = (mu_float)(GetLargeRand(360));
			Vector(0.0f, -(mu_float)(GetLargeRand(8) + 40), 0.0f, o->Velocity);
			break;
		case 2:
			o->LifeTime = 50;
			o->Scale = (mu_float)(GetLargeRand(64) + 64)*0.01f;
			o->Rotation = (mu_float)(GetLargeRand(360));
			o->Gravity = (mu_float)(GetLargeRand(32) + 60)*0.1f;
			break;
		case 16:
			o->LifeTime = 50;
			o->Scale = (mu_float)(GetLargeRand(64) + 64)*0.01f;
			o->Rotation = (mu_float)(GetLargeRand(360));
			o->Gravity = (mu_float)(GetLargeRand(32) + 60)*0.1f;
			break;
		case 5:
			o->LifeTime = 20;
			o->Scale = (mu_float)(GetLargeRand(64) + 98)*0.01f;
			o->Rotation = (mu_float)(GetLargeRand(360));
			o->Gravity = (mu_float)(GetLargeRand(32) + 60)*0.1f;
			break;
		case 6:
			o->LifeTime = 30;
			o->Gravity = (mu_float)(GetLargeRand(1000));
			o->Scale = (mu_float)(GetLargeRand(20) + 180)*0.01f;
			o->Position[0] += (mu_float)(GetLargeRand(200) - 100)*Scale;
			o->Position[1] += (mu_float)(GetLargeRand(200) - 100)*Scale;
			o->Position[2] += (mu_float)(GetLargeRand(20) + 20);
			o->Rotation = o->Position[2];
			Vector(0.0f, 0.0f, 0.0f, o->Velocity);
			break;
		case 7:
			o->LifeTime = 30;
			Vector(0.0f, (mu_float)(GetLargeRand(4) + 6)*o->Scale, 0.0f, o->Velocity);

			o->Gravity = (mu_float)(GetLargeRand(200) / 200.0f);
			o->Scale *= (mu_float)(GetLargeRand(20) + 120)*0.01f;
			o->Rotation = (mu_float)(GetLargeRand(360));
			break;
		case 8:
			o->LifeTime = 24;
			Vector(0.0f, -(mu_float)(GetLargeRand(8) + 32)*0.3f, 0.0f, o->Velocity);
			o->Scale *= 0.8f;
			o->Rotation = (mu_float)(GetLargeRand(360));
			break;
		case 19:
			VectorCopy(o->Light, o->TurningForce);
			o->LifeTime = 40;
			o->Scale = Scale*(mu_float)(GetLargeRand(32) + 8)*0.01f;
			o->Angle[0] = (mu_float)(GetLargeRand(360));
			o->Rotation = (mu_float)(IWorldTime % 360);
			break;
		case 20:
			o->LifeTime = 40;
			o->Scale = Scale*(mu_float)(GetLargeRand(32) + 8)*0.01f;
			o->Angle[0] = (mu_float)(GetLargeRand(360));
			o->Rotation = (mu_float)(IWorldTime % 360);
			break;
		case 21:
			o->LifeTime = 80;
			o->Scale = o->Scale * (mu_float)(GetLargeRand(64) + 64)*0.005f;
			o->Rotation = (mu_float)(GetLargeRand(360));
			o->Gravity = (mu_float)(GetLargeRand(32) + 60)*0.1f;
			break;
		case 22:
			o->LifeTime = 60;
			o->Scale = o->Scale * (mu_float)(GetLargeRand(64) + 64)*0.005f;
			o->Rotation = (mu_float)(GetLargeRand(360));
			o->Gravity = (mu_float)(GetLargeRand(32) + 60)*0.1f;
			break;
		case 24:
		case 57:
			o->LifeTime = 32;
			o->Scale = Scale + (mu_float)(GetLargeRand(32) + 48)*0.01f*Scale;
			o->Angle[0] = (mu_float)(GetLargeRand(360));
			o->Rotation = (mu_float)(IWorldTime % 360);
			break;
		case 25:
			o->LifeTime = 25;
			o->Scale = (mu_float)(GetLargeRand(64) + 64)*0.01f;
			o->Rotation = (mu_float)(GetLargeRand(360));
			o->Gravity = (mu_float)(GetLargeRand(32) + 60)*0.1f;
			break;
		case 26:
			o->LifeTime = 25;
			o->Scale = (mu_float)(GetLargeRand(64) + 64)*0.01f;
			o->Rotation = (mu_float)(GetLargeRand(360));
			o->Gravity = (mu_float)(GetLargeRand(32) + 60)*0.1f;
			break;
		case 27:
			o->LifeTime = 30;
			o->Rotation = (mu_float)(GetLargeRand(360));
			o->TexType = BITMAP_CHROME3;
			break;
		case 28:
			o->LifeTime = 12;
			o->Scale = 2.3f;
			o->Rotation = (mu_float)(GetLargeRand(360));
			o->Gravity = 33.0f;
			o->Position[2] = RequestTerrainHeight(o->Position[0], o->Position[1]) + 15;
			o->TexType = BITMAP_POUNDING_BALL;
			break;
		case 29:
			o->LifeTime = 12;
			o->Scale = 2.3f;
			o->Rotation = (mu_float)(GetLargeRand(360));
			o->Gravity = 33.0f;
			o->Position[2] = RequestTerrainHeight(o->Position[0], o->Position[1]) + 15;
			break;
		case 30:
			o->LifeTime = 4;
			o->Scale = 1.8f;
			o->Gravity = 15.0f;
			o->Rotation = (mu_float)(GetLargeRand(360));
			o->Position[2] = RequestTerrainHeight(o->Position[0], o->Position[1]) + 15;
			o->Position[0] += (mu_float)EMath::Sin(FWorldTime)*5.0f;
			o->Position[1] += (mu_float)EMath::Sin(FWorldTime)*5.0f;
			break;
		case 31:
			o->LifeTime = 15;
			o->Scale = Scale;
			o->Rotation = (mu_float)(GetLargeRand(360));
			o->Gravity = (mu_float)(GetLargeRand(32) + 60)*0.1f;
			break;
		case 40:
			{
				o->LifeTime = 50;
				o->Scale = (mu_float)(GetLargeRand(32) + 80)*0.01f;
				o->Position[0] += (mu_float)(GetLargeRand(64) - 32);
				o->Position[1] += (mu_float)(GetLargeRand(64) - 32);
				o->Position[2] += (mu_float)(GetLargeRand(64) + 32);
				o->Angle[0] = (mu_float)(GetLargeRand(90) - 45);
				o->Angle[2] = (mu_float)(GetLargeRand(360));
				Vector(0.0f, -(mu_float)(GetLargeRand(8) + 40), 0.0f, o->Velocity);
			}
			break;
		case 41:
			{
				o->LifeTime = 50;
				o->Scale = (mu_float)(GetLargeRand(32) + 80)*0.01f;
				o->Position[0] += (mu_float)(GetLargeRand(64) - 32);
				o->Position[1] += (mu_float)(GetLargeRand(64) - 32);
				o->Position[2] += (mu_float)(GetLargeRand(64) + 32);
				o->Angle[0] = (mu_float)(GetLargeRand(90) - 45);
				o->Angle[2] = (mu_float)(GetLargeRand(360));
				Vector(0.0f, -(mu_float)(GetLargeRand(8) + 40), 0.0f, o->Velocity);
				o->Gravity = (mu_float)(GetLargeRand(100) - 50)*0.1f;
			}
			break;
		case 43:
			{
				o->TexType = BITMAP_CLUD64;
				o->LifeTime = 40;
				o->Rotation = (mu_float)(GetLargeRand(360));
				o->Angle[0] = 90.0f;
				Vector(GetLargeRand(20) / 10.0f - 1.0f, 2.0f, 15.0f, o->Velocity);
			}
			break;
		case 44:
			{
				o->LifeTime = 60;
				o->Scale += (mu_float)(GetLargeRand(10)) * 0.5f;
				o->Position[0] += (mu_float)(GetLargeRand(500)) - 250.0f;
				o->Position[1] += (mu_float)(GetLargeRand(700)) - 350.0f;
				o->Position[2] += (mu_float)(GetLargeRand(100)) + 150.0f;
				o->Rotation = (mu_float)(GetLargeRand(360));
				o->Gravity = (mu_float)(GetLargeRand(50)) - 25.0f;
				Vector(0.1f, 0.2f, 0.3f, o->Light);
			}
			break;
		case 45:
			{
				o->TexType = BITMAP_LIGHT + 2;
				o->LifeTime = 15;
				o->Scale += (mu_float)(GetLargeRand(10)) / 20.0f;
				o->Position[0] += (mu_float)(GetLargeRand(40)) - 20.0f;
				o->Position[1] += (mu_float)(GetLargeRand(40)) - 20.0f;
				o->Position[2] += (mu_float)(GetLargeRand(40)) - 20.0f;
			}
			break;
		case 46:
			{
				o->LifeTime = 60 + (mu_int32)(o->Scale*10.0f);
				o->Scale += (mu_float)(GetLargeRand(50))*0.01f;
				o->Rotation = (mu_float)(GetLargeRand(360));
				o->Gravity = (mu_float)(GetLargeRand(30) + 50)*0.05f;
			}
			break;
		case 47:
			{
				o->TexType = BITMAP_MAGIC + 1;
				o->LifeTime = 5;
				o->Scale += (mu_float)(GetLargeRand(50))*0.01f;
				o->Rotation = (mu_float)(GetLargeRand(360));
			}
			break;
		case 59:
		case 48:
			o->LifeTime = 40;
			o->Scale = (mu_float)(GetLargeRand(64) + 64)*0.01f;
			o->Rotation = (mu_float)(GetLargeRand(360));
			o->Gravity = (mu_float)(GetLargeRand(32) + 60)*0.1f;
			break;
		case 49:
			{
				o->TexType = BITMAP_CLOUD;
				o->LifeTime = GetLargeRand(50) + 100;
				o->Rotation = (mu_float)(GetLargeRand(360));
				o->Scale = Scale * 0.4f;
				if (GetLargeRand(2) == 0)
					o->Angle[0] = 1.0f;
				else
					o->Angle[0] = -1.0f;
			}
			break;
		case 50:
			o->LifeTime = 20;
			o->Scale = (mu_float)(GetLargeRand(32) + 32)*0.01f*Scale;
			o->Rotation = (mu_float)(GetLargeRand(360));
			o->Gravity = (mu_float)(GetLargeRand(16) + 16)*0.1f;
			o->Alpha = 0;
			VectorCopy(Light, o->TurningForce);
			Vector(0, 0, 0, o->Light);
			break;
		case 51:
			o->LifeTime = 25;
			o->Scale = (mu_float)(GetLargeRand(64) + 64)*0.01f;
			o->Rotation = (mu_float)(GetLargeRand(360));
			o->Gravity = (mu_float)(GetLargeRand(32) + 60)*0.1f;
			VectorCopy(Light, o->TurningForce);
			break;
		case 52:
			{
				o->LifeTime = 40;
				o->Scale = Scale + (mu_float)(GetLargeRand(10) + 10)*0.05f;
				o->Rotation = (mu_float)(GetLargeRand(360));

				EVector3 vAngle, vPos, vPos2;
				Vector(0.0f, 10.0f, 0.0f, vPos);
				mu_float fAngle = o->Angle[2] + (mu_float)(GetLargeRand(100) - 50) + 150;
				Vector(0.0f, 0.0f, fAngle, vAngle);
				AngleMatrix(vAngle, Matrix);
				VectorRotate(vPos, Matrix, vPos2);
				VectorCopy(vPos2, o->Velocity);
			}
			break;
		case 54:
			{
				o->LifeTime = (mu_int32)(o->Scale*8.0f);
				o->Scale += (mu_float)(GetLargeRand(50))*0.01f;
				o->Rotation = (mu_float)(GetLargeRand(360));
				o->Gravity = (mu_float)(GetLargeRand(30) + 50)*0.05f;
			}
			break;
		case 55:
			{
				o->LifeTime = 30;
				o->Scale = Scale + (mu_float)(GetLargeRand(32) + 48)*0.01f*Scale;
				o->Angle[0] = (mu_float)(GetLargeRand(360));
				o->Rotation = (mu_float)(IWorldTime % 360);
			}
			break;
		case 58:
			o->LifeTime = 50;
			o->Scale = (mu_float)(GetLargeRand(32) + 80)*0.01f;
			o->Position[0] += (mu_float)(GetLargeRand(64) - 32);
			o->Position[1] += (mu_float)(GetLargeRand(64) - 32);
			o->Position[2] += (mu_float)(GetLargeRand(64) + 32);
			o->Angle[0] = (mu_float)(GetLargeRand(90) - 45);
			o->Angle[2] = (mu_float)(GetLargeRand(360));
			Vector(0.0f, -(mu_float)(GetLargeRand(8) + 40), 0.0f, o->Velocity);
			VectorCopy(o->Light, o->StartPosition);
			break;
		case 60:
			{
				Vector(0.4f, 0.4f, 0.4f, o->Light);
				o->LifeTime = 60;
				o->Scale = o->Scale * (mu_float)(GetLargeRand(64) + 64)*0.005f;
				o->Rotation = (mu_float)(GetLargeRand(360));
				o->Gravity = (mu_float)(GetLargeRand(32) + 60)*0.1f;
			}
			break;
		case 62:
			{
				o->LifeTime = 50;
				o->Scale = o->Scale;
				o->Position[0] += (mu_float)(GetLargeRand(64) - 32);
				o->Position[1] += (mu_float)(GetLargeRand(64) - 32);
				o->Angle[0] = (mu_float)(GetLargeRand(90) - 45);
				o->Angle[2] = (mu_float)(GetLargeRand(360));
				Vector(0.0f, -(mu_float)(GetLargeRand(8)), 0.0f, o->Velocity);
			}
			break;
		case 63:
			{
				o->TexType = BITMAP_CLUD64;
				o->LifeTime = 10;
				o->Rotation = (mu_float)(GetLargeRand(360));
				o->Angle[0] = 90.0f;
				Vector(0.0f, 30.0f, 15.0f, o->Velocity);
			}
			break;
		case 64:
			{
				o->LifeTime = 30;
				o->Scale = o->Scale * 0.3f;

				o->Velocity[0] = 0.0f;
				o->Velocity[1] = 10.0f;
				o->Velocity[2] = 0.0f;

				const mu_float ANG_REVISION = 20.0f;
				mu_float fAng;
				fAng = (mu_float)(GetLargeRand((mu_int32)ANG_REVISION) + 10);

				o->Angle[0] = o->Angle[0] + fAng;
			}
			break;
		case 65:
			{
				o->LifeTime = 45;
				o->Scale = o->Scale * (mu_float)(GetLargeRand(64) + 64)*0.005f;
				o->Rotation = (mu_float)(GetLargeRand(360));
				o->Gravity = (mu_float)(GetLargeRand(10) + 18)*0.1f;
			}
			break;
		case 66:
			{
				o->LifeTime = (mu_int32)(o->Scale*30.0f);
				o->Scale += (mu_float)(GetLargeRand(50))*0.01f;
				o->Rotation = (mu_float)(GetLargeRand(360));
				o->Gravity = (mu_float)(GetLargeRand(30) + 50)*0.01f;
				mu_int32 iAngle = GetLargeRand(360);
				o->TurningForce[0] = EMath::Sin(iAngle / 3.14f * 180);
				o->TurningForce[1] = EMath::Cos(iAngle / 3.14f * 180);
			}
			break;
		case 67:
			{
				o->LifeTime = 45;
				o->Alpha = 1.0f;
				o->Rotation = (mu_float)(GetLargeRand(360));
				o->Gravity = (mu_float)(GetLargeRand(32) + 60)*0.1f;
			}
			break;
		case 68:
			{
				o->LifeTime = 45;
				o->Scale = o->Scale * (mu_float)(GetLargeRand(64) + 64)*0.005f;
				o->Rotation = (mu_float)(GetLargeRand(360));
				o->Gravity = (mu_float)(GetLargeRand(10) + 18)*0.1f;
				o->Alpha = 1.0f;

				EVector3	v3BasisDir, v3Dir;
				EMatrix4 matAngle;

				Vector(0.0f, 0.0f, 1.0f, v3BasisDir);
				AngleMatrix(o->Angle, matAngle);
				VectorRotate(v3BasisDir, matAngle, v3Dir);

				o->Velocity[0] = v3Dir[0] * 0.6f;
				o->Velocity[1] = v3Dir[1] * 0.6f;
				o->Velocity[2] = v3Dir[2] * 0.6f;
			}
			break;
		}
		break;
	case BITMAP_SMOKE + 1:
	case BITMAP_SMOKE + 4:
		o->LifeTime = 32;
		o->Position[0] += (mu_float)(GetLargeRand(16) - 8);
		o->Position[1] += (mu_float)(GetLargeRand(16) - 8);

		if (o->Type == BITMAP_SMOKE + 4)
		{
			o->Scale = (mu_float)(GetLargeRand(32) + 32)*0.01f;
			o->Scale *= Scale;
		}
		else
		{

			o->Scale = (mu_float)(GetLargeRand(32) + 32)*0.01f*Scale;
		}

		if (SubType == 0)
		{
			AngleMatrix(Angle, Matrix);
			Vector(0.0f, 3.0f, 0.0f, p);
		}
		else
		{
			o->Angle[2] = (mu_float)(GetLargeRand(360));
			AngleMatrix(o->Angle, Matrix);
			Vector(0.0f, 15.0f, 0.0f, p);
		}
		VectorRotate(p, Matrix, o->Velocity);
		o->Angle[0] = (mu_float)(GetLargeRand(360));

		if (SubType == 1)
		{
			o->LifeTime = 40;
			Vector(0.0f, 0.0f, 0.0f, o->Velocity);
			o->Position[2] += (mu_float)(GetLargeRand(16) - 8);
		}
		else if (SubType == 6)
		{
			o->LifeTime = 40;
			Vector(0.0f, 0.0f, 0.0f, o->Velocity);
			o->Position[2] += (mu_float)(GetLargeRand(16) - 8);
		}
		break;
	case BITMAP_SMOKE + 2:
		o->LifeTime = 50;
		o->Scale = (mu_float)(GetLargeRand(32) + 64)*0.01f;
		Vector(0.0f, 0.0f, (mu_float)(GetLargeRand(360)), o->Angle);
		AngleMatrix(o->Angle, Matrix);
		Vector(0.0f, (mu_float)(GetLargeRand(64)), (mu_float)(GetLargeRand(16) + 16), p);
		VectorRotate(p, Matrix, o->StartPosition);
		Vector(0.0f, 0.0f, 0.0f, o->Angle);
		break;
	case BITMAP_SMOKE + 3:
		switch (SubType)
		{
		case 0:
			VectorCopy(o->Light, o->TurningForce);
			o->LifeTime = 55;
			o->Angle[0] = -2 + (mu_float)(GetLargeRand(4));
			o->Rotation = (mu_float)(IWorldTime % 360);
			break;
		case 1:
			VectorCopy(o->Light, o->TurningForce);
			o->LifeTime = 30;
			o->Angle[0] = -2 + (mu_float)(GetLargeRand(4));
			o->Rotation = (mu_float)(IWorldTime % 360);
			break;
		case 2:
			VectorCopy(o->Light, o->TurningForce);
			o->LifeTime = 55;
			o->Angle[0] = -2 + (mu_float)(GetLargeRand(4));
			o->Rotation = (mu_float)(IWorldTime % 360);
			break;
		case 3:
			o->TexType = BITMAP_CLUD64;
			o->LifeTime = 60;
			o->Scale = o->Scale * (mu_float)(GetLargeRand(64) + 64)*0.02f;
			o->Rotation = (mu_float)(GetLargeRand(360));
			o->Position[0] += (mu_float)(GetLargeRand(250) - 125);
			o->Position[1] += (mu_float)(GetLargeRand(250) - 125);
			o->Position[2] -= (mu_float)(GetLargeRand(45));
			o->Gravity = (mu_float)(GetLargeRand(100) - 50)*0.1f;
			Vector(1.0f, 1.0f, 1.0f, o->Light);
			break;
		case 4:
			o->TexType = BITMAP_WATERFALL_3;
			o->LifeTime = 30;
			o->Scale = o->Scale * (mu_float)(GetLargeRand(64) + 64)*0.02f;
			o->Rotation = (mu_float)(GetLargeRand(360));
			o->Position[0] += (mu_float)(GetLargeRand(250) - 125);
			o->Position[1] += (mu_float)(GetLargeRand(250) - 125);
			o->Position[2] -= (mu_float)(GetLargeRand(45));
			o->Gravity = (mu_float)(GetLargeRand(100) - 50)*0.1f;
			Vector(1.0f, 1.0f, 1.0f, o->Light);
			break;
		}
		break;
	case BITMAP_SMOKELINE1:
	case BITMAP_SMOKELINE2:
	case BITMAP_SMOKELINE3:
		if (o->SubType == 0)
		{
			if (GET_WORLD == WD_63PK_FIELD)
			{
				o->LifeTime = 25;
			}
			else
			{
				o->LifeTime = 35;
			}
			o->Scale = (mu_float)(GetLargeRand(96) + 96)*0.01f*Scale;
			o->Rotation = (mu_float)(GetLargeRand(360));
			o->Gravity = (mu_float)(GetLargeRand(16) + 12)*0.1f;
			o->Alpha = 0;
			VectorCopy(Light, o->TurningForce);
			Vector(0, 0, 0, o->Light);
		}
		else if (o->SubType == 1)
		{
			o->LifeTime = 25;
			o->Scale = (mu_float)(GetLargeRand(50) + 55)*0.01f*Scale;
			o->Rotation = (mu_float)(GetLargeRand(360));
			o->Gravity = (mu_float)(GetLargeRand(4) + 35)*0.1f;
			o->Alpha = 0;
			VectorCopy(Light, o->TurningForce);
			Vector(0, 0, 0, o->Light);
			VectorCopy(o->Target->Position, o->StartPosition);
		}
		else if ((o->SubType == 2) || (o->SubType == 3))
		{
			o->LifeTime = 45;
			o->Scale = (mu_float)(GetLargeRand(96) + 96)*0.01f*Scale;
			o->Rotation = (mu_float)(GetLargeRand(360));
			o->Gravity = (mu_float)(GetLargeRand(16) + 12)*0.1f;
			o->Alpha = 0;
			VectorCopy(Light, o->TurningForce);
			Vector(0, 0, 0, o->Light);
		}
		else if (o->SubType == 4)
		{
			o->Position[0] += (mu_float)(GetLargeRand(20) - 10)*2.0f;
			o->Position[1] += (mu_float)(GetLargeRand(20) - 10)*2.0f;
			o->Position[2] += (mu_float)(GetLargeRand(20) - 10)*2.0f;

			o->Scale = o->Scale + ((mu_float)(GetLargeRand(20) - 10)*(o->Scale*0.03f));
			o->LifeTime = 45;
			o->Alpha = 1.0f;

			o->Gravity = 0.2f;
			o->Rotation = (mu_float)(GetLargeRand(360));
		}
		break;
	case BITMAP_LIGHTNING_MEGA1:
	case BITMAP_LIGHTNING_MEGA2:
	case BITMAP_LIGHTNING_MEGA3:
		{
			switch (o->SubType)
			{
			case 0:
				{
					o->LifeTime = 5;
					o->Rotation = (mu_float)(GetLargeRand(360));
					o->Gravity = 0;
					o->Alpha = 1.0f;
					VectorCopy(Light, o->TurningForce);
					Vector(0, 0, 0, o->Light);
				}
				break;
			}
		}
		break;
	case BITMAP_FIRE_HIK1:
	case BITMAP_FIRE_HIK1_MONO:
		if (o->SubType == 0)
		{
			o->LifeTime = GetLargeRand(5) + 27;
			o->Scale = (mu_float)(GetLargeRand(72) + 72)*0.01f*Scale;
			o->Rotation = (mu_float)(GetLargeRand(360));
			o->Gravity = (mu_float)(GetLargeRand(24) + 64)*0.1f;
			o->Alpha = 0;
			VectorCopy(Light, o->TurningForce);
			Vector(0, 0, 0, o->Light);
		}
		else if (o->SubType == 1)
		{
			o->LifeTime = GetLargeRand(5) + 54;
			o->Scale = (mu_float)(GetLargeRand(72) + 72)*0.01f*Scale;
			o->Rotation = (mu_float)(GetLargeRand(360));
			o->Gravity = (mu_float)(GetLargeRand(24) + 74)*0.1f;
			o->Alpha = 0;
			VectorCopy(Light, o->TurningForce);
			Vector(0, 0, 0, o->Light);
		}
		else if (o->SubType == 2)
		{
			o->LifeTime = GetLargeRand(5) + 27;
			o->Scale = (mu_float)(GetLargeRand(72) + 72)*0.01f*Scale;
			o->Rotation = (mu_float)(GetLargeRand(360));
			o->Gravity = (mu_float)(GetLargeRand(24) + 100)*0.1f;
			mu_float fRand = (mu_float)(GetLargeRand(50)) * 0.03f;
			Vector(2.5f + fRand, -7.0f - fRand, 0.0f, o->Velocity);
			o->Alpha = 0;
			VectorCopy(Light, o->TurningForce);
			Vector(0, 0, 0, o->Light);
		}
		else if (o->SubType == 3)
		{
			o->LifeTime = GetLargeRand(5) + 27;
			o->Scale = (mu_float)(GetLargeRand(72) + 52)*0.01f*Scale;
			o->Rotation = (mu_float)(GetLargeRand(360));
			o->Gravity = (mu_float)(GetLargeRand(14) + 44)*0.1f;
			o->Alpha = 0;
			VectorCopy(Light, o->TurningForce);
			Vector(0, 0, 0, o->Light);
		}
		else if (o->SubType == 4)
		{
			o->LifeTime = 8;
			o->Scale = (mu_float)(GetLargeRand(42) + 12)*0.01f*Scale;
			o->Rotation = (mu_float)(GetLargeRand(360));
			o->Gravity = (mu_float)(GetLargeRand(14) + 44)*0.1f;
			o->Alpha = 0;
			VectorCopy(Light, o->TurningForce);
			Vector(0, 0, 0, o->Light);
			VectorCopy(o->Target->Position, o->StartPosition);
		}
		if (o->SubType == 10)
		{
			o->LifeTime = GetLargeRand(5) + 47;
			o->Scale = (mu_float)(GetLargeRand(72) + 72)*0.01f*Scale;
			o->Rotation = (mu_float)(GetLargeRand(360));
			o->Gravity = (mu_float)(GetLargeRand(24) + 64)*0.1f;
			o->Alpha = 0;
			VectorCopy(Light, o->TurningForce);
			Vector(0, 0, 0, o->Light);
		}
		break;

	case BITMAP_FIRE_HIK3:
	case BITMAP_FIRE_HIK3_MONO:
		if (o->SubType == 0)
		{
			o->LifeTime = GetLargeRand(5) + 17;
			o->Scale = (mu_float)(GetLargeRand(72) + 72)*0.01f*Scale;
			o->Rotation = (mu_float)(GetLargeRand(360));
			o->Gravity = (mu_float)(GetLargeRand(24) + 64)*0.1f;
			o->Alpha = 0;
			VectorCopy(Light, o->TurningForce);
			Vector(0, 0, 0, o->Light);
		}
		else if (o->SubType == 1)
		{
			o->LifeTime = GetLargeRand(5) + 34;
			o->Scale = (mu_float)(GetLargeRand(72) + 72)*0.01f*Scale;
			o->Rotation = (mu_float)(GetLargeRand(360));
			o->Gravity = (mu_float)(GetLargeRand(24) + 74)*0.1f;
			o->Alpha = 0;
			VectorCopy(Light, o->TurningForce);
			Vector(0, 0, 0, o->Light);
		}
		else if (o->SubType == 2)
		{
			o->LifeTime = GetLargeRand(5) + 17;
			o->Scale = (mu_float)(GetLargeRand(72) + 72)*0.01f*Scale;
			o->Rotation = (mu_float)(GetLargeRand(360));
			o->Gravity = (mu_float)(GetLargeRand(24) + 100)*0.1f;
			mu_float fRand = (mu_float)(GetLargeRand(50)) * 0.03f;
			Vector(2.5f + fRand, -5.0f - fRand, 0.0f, o->Velocity);
			o->Alpha = 0;
			VectorCopy(Light, o->TurningForce);
			Vector(0, 0, 0, o->Light);
		}
		else if (o->SubType == 3)
		{
			o->LifeTime = GetLargeRand(5) + 17;
			o->Scale = (mu_float)(GetLargeRand(72) + 52)*0.01f*Scale;
			o->Rotation = (mu_float)(GetLargeRand(360));
			o->Gravity = (mu_float)(GetLargeRand(14) + 44)*0.1f;
			o->Alpha = 0;
			VectorCopy(Light, o->TurningForce);
			Vector(0, 0, 0, o->Light);
		}
		else if (o->SubType == 4)
		{
			o->LifeTime = GetLargeRand(5) + 34;
			o->Scale = (mu_float)(GetLargeRand(72) + 72)*0.01f*Scale;
			o->Rotation = (mu_float)(GetLargeRand(360));
			o->Gravity = (mu_float)(GetLargeRand(10) + 40)*0.1f;
			o->Alpha = 0;
			VectorCopy(Light, o->TurningForce);
			Vector(0, 0, 0, o->Light);
		}
		else if (o->SubType == 5)
		{
			o->LifeTime = 8;
			o->Scale = (mu_float)(GetLargeRand(42) + 12)*0.01f*Scale;
			o->Rotation = (mu_float)(GetLargeRand(360));
			o->Gravity = (mu_float)(GetLargeRand(14) + 44)*0.1f;
			o->Alpha = 0;
			VectorCopy(Light, o->TurningForce);
			Vector(0, 0, 0, o->Light);
			VectorCopy(o->Target->Position, o->StartPosition);
		}
		break;

	case BITMAP_LIGHT + 1:
		o->LifeTime = 20 + GetLargeRand(8);
		if (o->SubType == 1)
		{
			o->LifeTime = 5;
			o->Angle[0] = -2 + (mu_float)(GetLargeRand(4));
		}
		else if (o->SubType == 2)
		{
			o->LifeTime = 19;
			o->Position[1] += -30.0f + (mu_float)(GetLargeRand(60));
		}
		else if (o->SubType == 3)
		{
			o->LifeTime = 7;
			o->TexType = BITMAP_SHINY + 1;
		}
		else if (o->SubType == 4)
		{
			o->LifeTime = 4;
			o->Rotation = -360.0f + (mu_float)(GetLargeRand(180));
			o->TexType = BITMAP_SHINY + 1;
		}
		else if (o->SubType == 5)
		{
			o->LifeTime = 3;
		}
		break;
	case BITMAP_SPARK:
		o->Scale = (mu_float)(GetLargeRand(4) + 4)*0.1f;
		if (o->SubType == 0 || o->SubType == 2 || o->SubType == 3 || o->SubType == 4 || o->SubType == 6 || o->SubType == 11)
		{
			o->LifeTime = GetLargeRand(16) + 24;
			o->Angle[2] = (mu_float)(GetLargeRand(360));
			AngleMatrix(o->Angle, Matrix);
			o->Gravity = (mu_float)(GetLargeRand(16) + 6);
			Vector(0.0f, (mu_float)(GetLargeRand(20) + 20)*0.1f, 0.0f, p);

			if (o->SubType == 2)
			{
				o->Scale *= 2.0f;
				VectorScale(p, 3.0f, p);
			}
			else if (o->SubType == 4)
			{
				o->Scale *= 3.0f;
				VectorScale(p, 10.0f, p);
			}
			else if (o->SubType == 6)
			{
				o->LifeTime = GetLargeRand(4) + 4;
				o->Scale = Scale;
				o->Gravity = 0.0f;
				o->TexType = BITMAP_SPARK + 1;
				Vector(0.0f, (mu_float)(GetLargeRand(20) + 20)*0.2f, 0.0f, p);
			}
			else if (o->SubType == 11)
			{
				Vector(1.0f, 0.3f, 0.3f, o->Light);
			}

			VectorRotate(p, Matrix, o->Velocity);
		}
		else if (o->SubType == 7)
		{
			o->LifeTime = GetLargeRand(12) + 12;
		}
		else if (o->SubType == 8
			|| o->SubType == 10
			)
		{
			if (o->SubType == 10)
			{
				o->TexType = BITMAP_CLUD64;
			}
			else
				o->TexType = BITMAP_SPARK + 1;
			o->LifeTime = GetLargeRand(16) + 24;
			o->Angle[2] = (mu_float)(GetLargeRand(360));
			AngleMatrix(o->Angle, Matrix);
			o->Gravity = (mu_float)(GetLargeRand(16) + 6);
			Vector(0.0f, (mu_float)(GetLargeRand(60) - 30)*0.1f, 0.0f, p);
			o->Scale *= 1.5f;
			VectorScale(p, 3.0f, p);
			VectorRotate(p, Matrix, o->Velocity);
		}
		else if (o->SubType == 9)
		{
			o->Scale = Scale*1.2f + (mu_float)(GetLargeRand(4) + 4)*0.1f;
			o->LifeTime = GetLargeRand(16) + 50;
			o->Angle[2] = (mu_float)(GetLargeRand(360));
			AngleMatrix(o->Angle, Matrix);
			o->Gravity = (mu_float)(GetLargeRand(2));
			Vector(0.0f, (mu_float)(GetLargeRand(10) + 10)*0.05f, 0.0f, p);
			VectorRotate(p, Matrix, o->Velocity);
		}
		else
		{
			if (o->SubType == 5)
			{
				VectorCopy(o->Target->Position, o->StartPosition);
				o->Scale = Scale;
				o->LifeTime = GetLargeRand(4) + 12;
				o->Gravity = 0;
				o->TexType = BITMAP_SPARK + 1;
			}
			else
			{
				o->LifeTime = GetLargeRand(8) + 8;
			}

			if (o->SubType == 7)
			{
				o->Scale += Scale;
			}
		}
		break;

	case BITMAP_SPARK + 1:
		switch (o->SubType)
		{
		case 1:
			Vector((mu_float)(GetLargeRand(360)), (mu_float)(GetLargeRand(360)), (mu_float)(GetLargeRand(360)), o->Angle);
			AngleMatrix(o->Angle, Matrix);
			Vector(0.0f, -50.0f, 0.0f, p);
			VectorRotate(p, Matrix, o->Velocity);
			break;
		case 2:
			o->LifeTime = 15;
			Vector(0.0f, 0.0f, 0.0f, o->Angle);
			VectorCopy(o->Position, o->StartPosition);
			o->Velocity[0] = (o->Target->Position[0] - o->Position[0]) / 10.0f;
			o->Velocity[1] = (o->Target->Position[1] - o->Position[1]) / 10.0f;
			o->Velocity[2] = (o->Target->Position[2] - o->Position[2]) / 10.0f;
			break;
		case 3:
			o->LifeTime = 5;
			break;
		case 4:
			o->LifeTime = 10;
			o->Alpha = 0.1f;
			Vector(0.0f, 0.0f, 0.0f, o->Angle);
			VectorCopy(o->Position, o->StartPosition);
			o->Velocity[0] = (o->Target->Position[0] - o->Position[0]) / 6.0f;
			o->Velocity[1] = (o->Target->Position[1] - o->Position[1]) / 6.0f;
			o->Velocity[2] = (o->Target->Position[2] - o->Position[2]) / 6.0f;
			break;
		case 5:
			o->LifeTime = 10;
			Vector(0.0f, 0.0f, Angle[2], o->Angle);
			AngleMatrix(o->Angle, Matrix);
			Vector(-(GetLargeRand(2) + 2.0f), 0.0f, 0.0f, p);
			VectorRotate(p, Matrix, o->Velocity);
			break;
		case 6:
			if (GetLargeRand(50) == 0)
			{
				o->LifeTime = 50;
				o->Gravity = 1.0f + (mu_float)(GetLargeRand(20)) / 5.0f;
				o->Scale = (mu_float)(GetLargeRand(5)) / 10.0f + 1.0f;
			}
			else
				o->LifeTime = 0;
			break;
		case 7:
			o->Alpha = 1.0f;
			o->Position[0] += -30.0f + (mu_float)(GetLargeRand(60));
			o->Position[1] += -30.0f + (mu_float)(GetLargeRand(60));
			o->Position[2] += -30.0f + (mu_float)(GetLargeRand(60));
			o->LifeTime = 30;
			o->Gravity = 1.3f;
			o->Scale = (mu_float)(GetLargeRand(5)) / 10.0f + 1.0f;
			break;
		case 8:
			o->LifeTime = 10;
			break;
		case 9:
			{
				o->LifeTime = 30;
				Vector(0.0f, (mu_float)(GetLargeRand(360)), 0.0f, o->Angle);
				AngleMatrix(o->Angle, Matrix);
				Vector(0.0f, 0.0f, (mu_float)(GetLargeRand(10)) + 160.0f, p);
				VectorRotate(p, Matrix, o->Velocity);
				VectorAdd(o->Velocity, o->Position, o->Position);
				Vector(12.0f, 0.0f, -2.0f, o->Velocity);
			}
			break;
		case 10:
			o->Alpha = 1.0f;
			o->Velocity[0] = -4.0f + (mu_float)(GetLargeRand(8));
			o->Velocity[1] = 0.0f;
			o->Velocity[2] = -4.0f + (mu_float)(GetLargeRand(8));
			o->LifeTime = 20;
			o->Scale = (mu_float)(GetLargeRand(5)) / 10.0f + 1.0f;
			break;
		case 13:
		case 11:
			o->Alpha = 1.0f;
			o->Velocity[0] = -4.0f + (mu_float)(GetLargeRand(8));
			o->Velocity[1] = (mu_float)(GetLargeRand(4));
			o->Velocity[2] = (mu_float)(GetLargeRand(4));
			o->LifeTime = 30;
			o->Scale = (mu_float)(GetLargeRand(5)) / 10.0f + 1.0f;
			break;
		case 12:
			{
				o->TexType = BITMAP_SHINY;
				o->Alpha = 1.0f;

				EVector3 p1, p2;
				Vector(0.0f, -100.0f, 0.0f, p1);
				AngleMatrix(o->Angle, Matrix);
				VectorRotate(p1, Matrix, p2);
				VectorAdd(o->Position, p2, o->Position);

				o->Position[0] += -8.0f + (mu_float)(GetLargeRand(16));
				o->Position[1] += -8.0f + (mu_float)(GetLargeRand(16));
				o->Position[2] += 150.0f;

				o->Velocity[0] = -1.0f + (mu_float)(GetLargeRand(20)) / 10.0f;
				o->Velocity[1] = -1.0f + (mu_float)(GetLargeRand(20)) / 10.0f;
				o->Velocity[2] = -(mu_float)(GetLargeRand(3)) - 3.0f;
				o->LifeTime = 40;
				o->Scale = -(mu_float)(GetLargeRand(15)) / 10.0f + Scale;
			}
			break;
		case 14:
			if (GetLargeRand(2) == 0)
			{
				o->Alpha = 1.0f;
				o->Velocity[0] = -4.0f + (mu_float)(GetLargeRand(4));
				o->Velocity[1] = (mu_float)(GetLargeRand(4));
				o->Velocity[2] = (mu_float)(GetLargeRand(4));
				o->LifeTime = 30;
				o->Scale = (mu_float)(GetLargeRand(5)) / 10.0f + 0.4f;
			}
			break;
		case 15:
			if (GetLargeRand(3) == 0)
			{
				o->Alpha = 1.0f;
				o->LifeTime = 35;
				o->Velocity[2] = -((mu_float)(GetLargeRand(5)) + 5)*0.5f;
			}
			break;
		case 16:
		case 18:
			{
				o->Alpha = 1.0f;
				o->LifeTime = 100;
				if (o->SubType == 18)
				{
					o->LifeTime = 80;
				}
				o->Frame = 77;
				o->Scale = -(mu_float)(GetLargeRand(3)) / 3.0f + Scale + 0.01f;

				o->Velocity[0] = (mu_float)(GetLargeRand(50)) / 10.0f - 2.5f;
				o->Velocity[1] = (mu_float)(GetLargeRand(4)) / 10.0f - 0.2f;
				if (o->SubType == 18)
				{
					o->Velocity[1] = (mu_float)(GetLargeRand(50)) / 10.0f - 2.5f;
				}
				o->Velocity[2] = (mu_float)(GetLargeRand(50)) / 10.0f - 2.5f;

				EVector3 p1, p2;
				Vector(o->Velocity[0] * 10.0f, o->Velocity[1] * 10.0f, o->Velocity[2] * 10.0f + 100.0f, p1);
				AngleMatrix(o->Angle, Matrix);
				VectorRotate(p1, Matrix, p2);
				VectorAdd(o->Position, p2, o->Position);

				o->Gravity = 3.5f;
			}
			break;
		case 17:
			{
				o->LifeTime = 200;
				o->Scale += (mu_float)(GetLargeRand(3))*0.1f;
				o->Gravity = (mu_float)(GetLargeRand(5))*0.3f + 2.0f;
				o->Rotation = (mu_float)(GetLargeRand(20))*0.0001f + 0.002f;
				o->Alpha = (mu_float)(GetLargeRand(10))*0.1f + 0.5f;
				VectorCopy(o->Position, o->StartPosition);
			}
			break;
		case 19:
			{
				o->LifeTime = 50;
				o->Gravity = 1.0f + (mu_float)(GetLargeRand(20)) * 0.1;
				o->Position[2] = RequestTerrainHeight(o->Position[0], o->Position[1]) - 5.0f;
				o->Scale = (mu_float)(GetLargeRand(10)) * 0.08f + 0.8f;
				VectorCopy(Light, o->Light);
			}
			break;
		case 20:
			{
				o->Scale = Scale + (GetLargeRand(10))*0.02f;
				o->LifeTime = GetLargeRand(10) + 60;
				o->Angle[2] = (mu_float)(GetLargeRand(360));
				AngleMatrix(o->Angle, Matrix);
				o->Gravity = (mu_float)(GetLargeRand(10) + 15);
				Vector((mu_float)(GetLargeRand(40) - 20)*0.1f, (mu_float)(GetLargeRand(40) - 20)*0.1f, 0.0f, p);
				VectorScale(p, 2.5f, p);
				VectorRotate(p, Matrix, o->Velocity);
			}
			break;
		case 21:
			{
				EVector3 vSpeed;
				vSpeed[0] = -8.0f + (mu_float)(GetLargeRand(16));
				vSpeed[1] = -8.0f + (mu_float)(GetLargeRand(16));
				vSpeed[2] = -8.0f + (mu_float)(GetLargeRand(16));

				VectorNormalize(vSpeed);
				VectorScale(vSpeed, 12.0f, vSpeed);
				VectorCopy(vSpeed, o->Velocity);

				o->LifeTime = GetLargeRand(10) + 20;
				o->Scale = (mu_float)(GetLargeRand(20)) / 20.0f + 1.0f;
			}
			break;
		case 22:
			{
				EVector3 vSpeed;
				vSpeed[0] = -8.0f + (mu_float)(GetLargeRand(16));
				vSpeed[1] = -8.0f + (mu_float)(GetLargeRand(16));
				vSpeed[2] = -8.0f + (mu_float)(GetLargeRand(16));

				VectorNormalize(vSpeed);
				VectorScale(vSpeed, 14.0f, vSpeed);
				VectorCopy(vSpeed, o->Velocity);

				o->Scale = (mu_float)(GetLargeRand(20)) / 20.0f + 1.0f;
				o->LifeTime = GetLargeRand(10) + 20;
				o->Angle[2] = (mu_float)(GetLargeRand(360));
				AngleMatrix(o->Angle, Matrix);
				o->Gravity = 20.0f;
				Vector((mu_float)(GetLargeRand(40) - 20)*0.1f, (mu_float)(GetLargeRand(40) - 20)*0.1f, 0.0f, p);
				VectorScale(p, 2.5f, p);
				VectorRotate(p, Matrix, o->Velocity);
			}
			break;
		case 23:
			o->Alpha = 1.0f;
			o->Velocity[0] = (mu_float)(-1 + GetLargeRand(3))*1.5f;
			o->Velocity[1] = (mu_float)(-1 + GetLargeRand(3))*1.5f;
			o->Velocity[2] = (mu_float)(-1 + GetLargeRand(3))*1.5f;
			o->LifeTime = 20;
			o->Rotation = 0;
			o->Scale = (mu_float)(GetLargeRand(5)) / 10.0f + 0.8f;
			break;
		case 24:
			o->Alpha = 1.0f;
			o->Velocity[0] = (mu_float)(-1 + GetLargeRand(3))*2.0f;
			o->Velocity[1] = (mu_float)(-1 + GetLargeRand(3))*2.0f;
			o->Velocity[2] = (mu_float)(-1 + GetLargeRand(3))*2.0f;
			o->LifeTime = 16;
			o->Rotation = 0;
			o->Scale = (mu_float)(GetLargeRand(5)) / 10.0f + 0.4f;
			break;
		case 25:
			o->Alpha = 1.0f;
			o->Velocity[0] = -2.0f + (mu_float)(GetLargeRand(6));
			o->Velocity[1] = (mu_float)(GetLargeRand(4));
			o->Velocity[2] = (mu_float)(GetLargeRand(4));
			o->LifeTime = 20;
			break;
		case 26:
			o->LifeTime = 20;
			Vector(0.0f, 0.0f, 0.0f, o->Angle);
			VectorCopy(o->Position, o->StartPosition);
			o->Velocity[0] = (o->Target->Position[0] - o->Position[0]) / 20.0f;
			o->Velocity[1] = (o->Target->Position[1] - o->Position[1]) / 20.0f;
			o->Velocity[2] = (o->Target->Position[2] - o->Position[2]) / 20.0f;
			//Vector(1.0f, 0.0f, 0.0f, o->Light);
			break;
		case 27:
			{
				EVector3 vSpeed;
				vSpeed[0] = -8.0f + (mu_float)(GetLargeRand(16));
				vSpeed[1] = -8.0f + (mu_float)(GetLargeRand(16));
				vSpeed[2] = -8.0f + (mu_float)(GetLargeRand(16));

				VectorNormalize(vSpeed);
				VectorScale(vSpeed, 12.0f, vSpeed);
				VectorCopy(vSpeed, o->Velocity);

				o->LifeTime = GetLargeRand(10) + 10;
				o->Scale = (mu_float)(GetLargeRand(20)) / 20.0f + 1.0f;
			}
			break;
		case 28:
			{
				o->Scale = (mu_float)(GetLargeRand(20)) / 20.0f + 1.0f;
				o->LifeTime = GetLargeRand(10) + 20;
				o->Angle[2] = (mu_float)(GetLargeRand(360));
				AngleMatrix(o->Angle, Matrix);
				o->Gravity = 20.0f;
				Vector((mu_float)(GetLargeRand(40) - 20)*0.1f, (mu_float)(GetLargeRand(40) - 20)*0.1f, 0.0f, p);
				VectorScale(p, 2.5f, p);
				VectorRotate(p, Matrix, o->Velocity);
			}
			break;
		case 29:
			{
				o->Scale = (mu_float)(GetLargeRand(20)) / 20.0f + 0.5f;
				o->LifeTime = GetLargeRand(10) + 10;
				o->Angle[2] = (mu_float)(GetLargeRand(360));
				AngleMatrix(o->Angle, Matrix);
				o->Gravity = -20.0f;
				Vector((mu_float)(GetLargeRand(40) - 20)*0.1f, (mu_float)(GetLargeRand(40) - 20)*0.1f, 0.0f, p);
				VectorScale(p, 2.5f, p);
				VectorRotate(p, Matrix, o->Velocity);
			}
			break;
		case 30:
			{
				o->LifeTime = GetLargeRand(5) + 50;

				o->Scale = (mu_float)(GetLargeRand(10)) / 10.0f + 0.2f;

				o->Velocity[0] = -2.0f + (mu_float)(GetLargeRand(5));
				o->Velocity[1] = -2.0f + (mu_float)(GetLargeRand(5));
				o->Velocity[2] = -2.0f + (mu_float)(GetLargeRand(5));

				o->Gravity = 0.5f;
			}
			break;
		case 31:
			{
				o->Scale = (mu_float)(GetLargeRand(4) + 4)*0.1f;
				o->LifeTime = GetLargeRand(16) + 24;
				o->Angle[2] = (mu_float)(GetLargeRand(360));
				AngleMatrix(o->Angle, Matrix);
				o->Gravity = (mu_float)(GetLargeRand(16) + 6);
				Vector(0.0f, (mu_float)(GetLargeRand(20) + 20)*0.1f, 0.0f, p);

				VectorScale(p, 3.0f, p);

				VectorRotate(p, Matrix, o->Velocity);
			}
			break;
		}
		break;

	case BITMAP_SPARK + 2:
		if (o->SubType == 0)
		{
			o->LifeTime = 16;
			o->Scale = Scale + (mu_float)(GetLargeRand(10) - 5)*0.03f;
			o->Position[0] += (mu_float)(GetLargeRand(10) - 5);
			o->Position[1] += (mu_float)(GetLargeRand(10) - 5);
			o->Position[2] += (mu_float)(GetLargeRand(10) - 5);
		}
		else if (o->SubType == 1)
		{
			o->LifeTime = 12;
		}
		else if (o->SubType == 2 || o->SubType == 3)
		{
			o->Scale = 0.4f;
			o->LifeTime = 16;
		}
		break;

	case BITMAP_EXPLOTION_MONO:
	case BITMAP_EXPLOTION:
		o->LifeTime = 20;
		o->Scale = Scale;
		if (o->SubType == 2)
		{
			o->LifeTime = 30;
		}

		//if ( o->SubType != 1)
		{
			if (o->SubType == 10)
			{
				PlayBackground(SOUND_MOONRABBIT_EXPLOSION);
			}
			else
			{
				if (g_CurrentScene != LOG_IN_SCENE)
					PlayBackground(SOUND_EXPLOTION01);
			}
		}
		/*else
		{
		PlayBuffer(SOUND_METEORITE01);
		}*/
		break;
	case BITMAP_SUMMON_SAHAMUTT_EXPLOSION:
		o->LifeTime = 16;
		o->Scale = Scale;
		break;
	case BITMAP_SPOT_WATER:
		o->LifeTime = 32;
		o->Scale = (mu_float)((GetLargeRand(170) + 1)*0.01f);
		break;
	case BITMAP_FLARE_RED:
		o->LifeTime = 18;
		HandPosition(ThreadIndex, o);
		break;

	case BITMAP_EXPLOTION + 1:
		o->LifeTime = 12;
		o->Scale = Scale;
		PlayBackground(SOUND_EXPLOTION01);
		break;

	case BITMAP_SHINY:
		o->LifeTime = 18;
		o->Angle[0] = 45.0f;
		if (o->SubType == 2)
		{
			o->LifeTime = 25;
			o->Rotation = (mu_float)(GetLargeRand(360));

			o->Velocity[0] = -0.1f;
			o->Velocity[1] = -0.5f;
			o->Velocity[2] = -1.0f;
		}
		else if (o->SubType == 3)
		{
			o->Alpha = 1.0f;
			o->Velocity[0] = -4.0f + (mu_float)(GetLargeRand(8));
			o->Velocity[1] = (mu_float)(GetLargeRand(4));
			o->Velocity[2] = (mu_float)(GetLargeRand(4));
			o->LifeTime = 30;
			o->Scale = (mu_float)(GetLargeRand(10)) / 20.0f + 0.5f;
		}
		else if (o->SubType == 4)
		{
			o->Scale = Scale + (GetLargeRand(10))*0.02f;
			o->LifeTime = GetLargeRand(10) + 15;
			o->Angle[2] = (mu_float)(GetLargeRand(360));
			AngleMatrix(o->Angle, Matrix);
			o->Gravity = (mu_float)(GetLargeRand(10) + 10);
			Vector((mu_float)(GetLargeRand(40) - 20)*0.1f, (mu_float)(GetLargeRand(40) - 20)*0.1f, 0.0f, p);
			VectorScale(p, 2.0f, p);
			VectorRotate(p, Matrix, o->Velocity);
		}
		else if (o->SubType == 5)
		{
			o->LifeTime = GetLargeRand(10) + 10;
			o->Scale = Scale + (GetLargeRand(10))*0.02f;
			o->Position[0] += (mu_float)(GetLargeRand(10) - 5);
			o->Position[1] += (mu_float)(GetLargeRand(10) - 5);
			o->Gravity = -(mu_float)(GetLargeRand(10))*0.3f;
		}
		else if (o->SubType == 6)
		{
			o->Scale = (mu_float)(GetLargeRand(5)) / 10.0f + 0.5f;
			o->StartPosition[0] = o->Scale;
			o->LifeTime = GetLargeRand(10) + 60;
			o->Gravity = 20.0f;

			o->Angle[0] = (mu_float)(GetLargeRand(360));
			o->Angle[1] = (mu_float)(GetLargeRand(360));
			o->Angle[2] = (mu_float)(GetLargeRand(360));
			AngleMatrix(o->Angle, Matrix);
			Vector(1, 1, 1, p);
			VectorScale(p, 8.0f, p);
			VectorRotate(p, Matrix, o->Velocity);

			o->Rotation = GetLargeRand(20) - 10;
		}
		else if (o->SubType == 7)
		{
			o->Scale = 0.1f;
			o->StartPosition[0] = o->Scale;
			o->LifeTime = GetLargeRand(10) + 20;
			o->Position[0] += (mu_float)(GetLargeRand(50) - 25);
			o->Position[1] += (mu_float)(GetLargeRand(50) - 25);
			o->Position[2] += (mu_float)(GetLargeRand(50) - 25);

			o->Gravity = 1.0f;

			o->Angle[0] = (mu_float)(GetLargeRand(360));
			o->Angle[1] = (mu_float)(GetLargeRand(360));
			o->Angle[2] = (mu_float)(GetLargeRand(360));
			AngleMatrix(o->Angle, Matrix);
			Vector(0.5f, 0.5f, 0.5f, p);
			VectorScale(p, 0.8f, p);
			VectorRotate(p, Matrix, o->Velocity);

			o->Rotation = GetLargeRand(20) - 10;
		}
		else if (o->SubType == 8)
		{
			o->Scale = (mu_float)(GetLargeRand(5)) / 10.0f + 0.5f;
			o->StartPosition[0] = o->Scale;
			o->LifeTime = GetLargeRand(10) + 40;
			o->Gravity = 20.0f;
			Vector(0, 0, 1.0f, o->Velocity);

			o->Rotation = GetLargeRand(20) - 10;
		}
		else if (o->SubType == 9)
		{
			o->Scale = (mu_float)(GetLargeRand(5)) / 10.0f + 0.5f;
			o->StartPosition[0] = o->Scale;
			o->LifeTime = GetLargeRand(10) + 60;
			o->Gravity = 20.0f;

			o->Angle[0] = (mu_float)(GetLargeRand(360));
			o->Angle[1] = (mu_float)(GetLargeRand(360));
			o->Angle[2] = (mu_float)(GetLargeRand(360));
			AngleMatrix(o->Angle, Matrix);
			Vector(1, 1, 1, p);
			VectorScale(p, 8.0f, p);
			VectorRotate(p, Matrix, o->Velocity);

			o->Rotation = GetLargeRand(20) - 10;
		}
		break;

	case BITMAP_CHERRYBLOSSOM_EVENT_PETAL:
		{
			if (o->SubType == 0)
			{
				o->Alpha = 1.0f;
				o->LifeTime = 30;
				o->Scale = (mu_float)(GetLargeRand(10)) / 20.0f + 0.5f;
				o->Rotation = (mu_float)(GetLargeRand(360));
				o->Gravity = -(mu_float)(GetLargeRand(10) + 10);
				Vector(0.0f, 0.0f, 0.0f, o->Angle);
				o->Velocity[2] = -4.0f + (mu_float)(GetLargeRand(8));
				o->Velocity[1] = (mu_float)(GetLargeRand(4));
				o->Velocity[0] = (mu_float)(GetLargeRand(4));
			}
			else if (o->SubType == 1)
			{
				o->Alpha = 1.0f;
				o->LifeTime = 70;
				o->Alpha = 1.0f;

				o->Velocity[0] = -2.0f + (mu_float)(GetLargeRand(6));
				o->Velocity[1] = (mu_float)(GetLargeRand(4));
				o->Velocity[2] = (mu_float)(GetLargeRand(4));

				o->Rotation = (mu_float)(GetLargeRand(360));
			}
		}
		break;

	case BITMAP_CHERRYBLOSSOM_EVENT_FLOWER:
		{
			if (o->SubType == 0)
			{
				EVector3 vSpeed;
				vSpeed[0] = -8.0f + (mu_float)(GetLargeRand(16));
				vSpeed[1] = -8.0f + (mu_float)(GetLargeRand(16));
				vSpeed[2] = -8.0f + (mu_float)(GetLargeRand(16));

				VectorNormalize(vSpeed);
				VectorScale(vSpeed, 14.0f, vSpeed);
				VectorCopy(vSpeed, o->Velocity);

				o->Alpha = 1.0f;
				o->LifeTime = GetLargeRand(30) + 20;
				o->Angle[2] = (mu_float)(GetLargeRand(360));
				o->Rotation = (mu_float)(GetLargeRand(360));
				AngleMatrix(o->Angle, Matrix);
				o->Gravity = 20.0f;
				Vector((mu_float)(GetLargeRand(40) - 20)*0.1f, (mu_float)(GetLargeRand(40) - 20)*0.1f, 0.0f, p);
				VectorScale(p, 2.5f, p);
				VectorRotate(p, Matrix, o->Velocity);
			}
		}
		break;	
	case BITMAP_SHINY + 1:
		if (o->SubType == 5)
		{
			o->LifeTime = 14;
			o->Rotation = (mu_float)(GetLargeRand(360));
			o->Gravity = 0.0f;
			Vector(0.0f, 0.0f, 0.0f, o->Velocity);
		}
		else
		{
			o->LifeTime = 36;
			o->Angle[0] = 45.0f;
			if (o->SubType != 99)
			{

				HandPosition(ThreadIndex, o);
			}
		}
		break;
	case BITMAP_SHINY + 2:
		o->LifeTime = 18;
		HandPosition(ThreadIndex, o);
		break;
	case BITMAP_SHINY + 4:
		o->LifeTime = 20;
		if (o->SubType == 1)
		{
			o->LifeTime = 15;
			o->Gravity = 0.0f;
			o->Rotation = (mu_float)(GetLargeRand(360));
		}
		break;
	case BITMAP_SHINY + 6:
		if (o->SubType == 0)
		{
			o->Alpha = 1.0f;
			o->Position[0] += -5.0f + (mu_float)(GetLargeRand(10));
			o->Position[1] += -5.0f + (mu_float)(GetLargeRand(10));
			o->Position[2] += -5.0f + (mu_float)(GetLargeRand(10));
			o->LifeTime = 30;
			o->Gravity = 1.3f;
			o->Scale += (mu_float)(GetLargeRand(3)) / 10.0f;
		}
		else if (o->SubType == 1)
		{
			o->Alpha = 1.0f;
			o->LifeTime = GetLargeRand(3) + 10;
			o->Scale = 0.3f;

			o->Position[0] += (mu_float)(GetLargeRand(50) - 25);
			o->Position[1] += (mu_float)(GetLargeRand(50) - 25);
			o->Position[2] += (mu_float)(GetLargeRand(50) - 25);
			o->Rotation = GetLargeRand(20) - 10;
		}
		break;
	case BITMAP_PIN_LIGHT:
		o->Alpha = 1.0f;

		{
			o->Position[0] += -5.0f + (mu_float)(GetLargeRand(10));
			o->Position[1] += -5.0f + (mu_float)(GetLargeRand(10));
			o->Position[2] += -5.0f + (mu_float)(GetLargeRand(10));
		}
		o->LifeTime = 30;
		o->Gravity = 10.0f;
		o->Scale += (mu_float)(GetLargeRand(5)) / 10.0f;
		if (o->SubType == 2)
		{
			o->Gravity = -5.0f;
		}
		else if (o->SubType == 3)
		{
			o->Gravity = -5.0f;
			o->TexType = BITMAP_SHINY + 6;
			o->Rotation = GetLargeRand(360);
		}
		break;
	case BITMAP_ORORA:
		o->Scale = 0.2f;
		switch (o->SubType)
		{
		case 0:
		case 1:	o->LifeTime = 100;	break;
		case 2:
		case 3:	o->LifeTime = 25;	break;
		}
		break;
	case BITMAP_SNOW_EFFECT_1:
	case BITMAP_SNOW_EFFECT_2:
		o->Scale = Scale + (GetLargeRand(10))*0.02f;
		o->LifeTime = GetLargeRand(10) + 30;
		o->Angle[2] = (mu_float)(GetLargeRand(360));
		AngleMatrix(o->Angle, Matrix);
		o->Gravity = (mu_float)(GetLargeRand(10) + 20);
		Vector((mu_float)(GetLargeRand(40) - 20)*0.1f, (mu_float)(GetLargeRand(40) - 20)*0.1f, 0.0f, p);
		VectorScale(p, 1.5f, p);
		VectorRotate(p, Matrix, o->Velocity);
		break;

	case BITMAP_DS_EFFECT:
		o->Scale = Scale + (GetLargeRand(10))*0.02f;
		o->LifeTime = 100;
		break;
	case BITMAP_BLOOD:
	case BITMAP_BLOOD + 1:
		o->LifeTime = 12;
		o->Scale = (mu_float)(GetLargeRand(4) + 8)*0.05f;
		o->Angle[0] = (mu_float)(GetLargeRand(360));
		AngleMatrix(Angle, Matrix);
		Vector(0.0f, -(mu_float)(GetLargeRand(16) + 8), (mu_float)(GetLargeRand(6) - 3), p);
		VectorRotate(p, Matrix, o->Velocity);
		break;
	case BITMAP_FIRECRACKER:
		o->Angle[0] = o->Angle[1] = o->Angle[2] = 0.0f;
		o->Rotation = (mu_float)(GetLargeRand(360));
		if (SubType == -1)
		{
			o->LifeTime = 2;
			o->Scale = 0.25f;
		}
		else
		{
			mu_int32 iSize = 15 + SubType;
			o->Velocity[0] = (mu_float)(GetLargeRand(iSize) - iSize / 2)*0.15f;
			o->Velocity[1] = (mu_float)(GetLargeRand(iSize) - iSize / 2)*0.15f;
			o->Velocity[2] = (mu_float)(GetLargeRand(iSize) - iSize / 2)*0.15f + 10.0f;
			o->LifeTime = 50;
			o->Scale = 0.10f;
		}
		break;
	case BITMAP_SWORD_FORCE:
		o->LifeTime = 10;
		o->Gravity = 0.1f;
		o->Rotation = o->Angle[2] + 135.0f;
		break;
	case BITMAP_CLOUD:
			if (o->SubType == 6)
			{
				o->LifeTime = 25 + GetLargeRand(5);
				o->Rotation = (mu_float)(GetLargeRand(360));
				o->Scale = 0.2f;
				o->Velocity[0] = (mu_float)(GetLargeRand(10) + 5)*0.4f;
				o->Velocity[1] = 0.0f;
				o->Velocity[2] = (mu_float)(GetLargeRand(10) + 5)*0.2f;
			}
			else if (o->SubType == 8)
			{
				o->LifeTime = 300;
				o->Gravity = (mu_float)(GetLargeRand(1000));
				o->Alpha = 0.1f;

				o->Position[0] += (mu_float)(GetLargeRand(500) - 250);
				o->Position[1] += (mu_float)(GetLargeRand(500) - 250);
				o->Position[2] += (mu_float)(GetLargeRand(20) - 20);

				o->StartPosition[1] = (GetLargeRand(100)) / 100.0f;
				o->StartPosition[2] = o->Position[2];

				o->Scale = (mu_float)(GetLargeRand(20) + 180)*0.01f;
				o->TurningForce[0] = Scale + GetLargeRand(30) / 100.0f;
				Vector(0.0f, 0.0f, 0.0f, o->Velocity);
			}
			else if (o->SubType == 9)
			{
				o->LifeTime = 300;
				o->Gravity = (mu_float)(GetLargeRand(1000));
				o->Alpha = 0.1f;

				o->Position[0] += (mu_float)(GetLargeRand(500) - 250);
				o->Position[1] += (mu_float)(GetLargeRand(500) - 250);
				o->Position[2] += (mu_float)(GetLargeRand(20) - 20);

				o->StartPosition[1] = (GetLargeRand(100)) / 100.0f;
				o->StartPosition[2] = o->Position[2];

				o->Scale = (((mu_float)(GetLargeRand(20) + 180)*0.01f) * o->Scale);
				o->TurningForce[0] = GetLargeRand(10) / 100.0f;
				Vector(0.0f, 0.0f, 0.0f, o->Velocity);
			}
			else if (o->SubType == 10)
			{
				Vector(0.0f, 0.0f, 0.0f, o->Light);
				o->LifeTime = 500;
				o->Position[0] += (mu_float)(GetLargeRand(400) - 200);
				o->Position[1] += (mu_float)(GetLargeRand(400) - 200);
				o->Position[2] += (mu_float)(GetLargeRand(400) - 200);
				VectorCopy(o->Position, o->StartPosition);
				o->Scale = (mu_float)(GetLargeRand(15) + 10) / 15.0f + Scale;
				mu_float fTemp = (mu_float)(GetLargeRand(10) + 5)*0.12f;
				o->Velocity[0] = fTemp;
				o->Velocity[1] = fTemp;
				o->Velocity[2] = 0.0f;
			}
			else if (o->SubType == 11)
			{
				o->LifeTime = 500;
				o->Position[0] += (mu_float)(GetLargeRand(400) - 200);
				o->Position[1] += (mu_float)(GetLargeRand(400) - 200);
				o->Position[2] += (mu_float)(GetLargeRand(400) - 200);
				VectorCopy(o->Position, o->StartPosition);
				o->Scale = (mu_float)(GetLargeRand(15) + 15) / 30.0f + Scale;
				mu_float fTemp = (mu_float)(GetLargeRand(10) + 5)*0.12f;
				o->Velocity[0] = fTemp;
				o->Velocity[1] = fTemp;
				o->Velocity[2] = 0.0f;
			}
			else if (o->SubType == 12)
			{
				o->LifeTime = 30;
				o->Gravity = (mu_float)(GetLargeRand(1000));

				o->Position[0] += (mu_float)(GetLargeRand(500) - 250);//*Scale;
				o->Position[1] += (mu_float)(GetLargeRand(500) - 250);//*Scale;
				o->Position[2] += (mu_float)(GetLargeRand(20) + 20);

				o->StartPosition[1] = (GetLargeRand(100)) / 100.0f;
				o->StartPosition[2] = o->Position[2];

				o->Scale = (mu_float)(GetLargeRand(20) + 180)*0.01f;
				o->TurningForce[0] = Scale + GetLargeRand(30) / 100.0f;
				Vector(0.0f, 0.0f, 0.0f, o->Velocity);
			}
			else if (o->SubType == 13)
			{
				o->LifeTime = 30;
				o->Rotation = GetLargeRand(360);
				o->Gravity = (mu_float)(GetLargeRand(50) - 25) / 10.0f;
			}
			else if (o->SubType == 14)
			{
				Vector(0.0f, 0.0f, 0.0f, Light);
				o->LifeTime = 25 + GetLargeRand(5);
				o->Rotation = mu_float(GetLargeRand(360));
				o->Scale = Scale*0.2f;
				o->Velocity[0] = (mu_float)(GetLargeRand(10) + 5)*0.4f;
				o->Velocity[1] = 0.0f;
				o->Velocity[2] = (mu_float)(GetLargeRand(10) + 5)*0.2f;
			}
			else if (o->SubType == 15)
			{
				o->LifeTime = 500;

				o->Position[0] += (mu_float)(GetLargeRand(400) - 200);
				o->Position[1] += (mu_float)(GetLargeRand(400) - 200);
				o->Position[2] += (mu_float)(GetLargeRand(400) - 200);
				VectorCopy(o->Position, o->StartPosition);

				o->Rotation = GetLargeRand(360);

				o->Scale = (mu_float)(GetLargeRand(15) + 15) / 30.0f + Scale;

				mu_float fTemp = (mu_float)(GetLargeRand(10) + 5)*0.08f;
				o->Velocity[0] = fTemp;
				o->Velocity[1] = fTemp;
				o->Velocity[2] = 0.0f;

				VectorCopy(o->Light, o->TurningForce);
				o->Light[0] = 0.0f;
				o->Light[1] = 0.0f;
				o->Light[2] = 0.0f;
			}
			else if (o->SubType == 16)
			{
				Vector(0.0f, 0.0f, 0.0f, o->Light);

				o->LifeTime = 500;

				o->Position[0] += (mu_float)(GetLargeRand(400) - 200);
				o->Position[1] += (mu_float)(GetLargeRand(400) - 200);
				o->Position[2] += (mu_float)(GetLargeRand(400) - 200);
				VectorCopy(o->Position, o->StartPosition);

				o->Rotation = GetLargeRand(360);

				o->Scale = (mu_float)(GetLargeRand(15) + 10) / 15.0f + Scale;

				mu_float fTemp = (mu_float)(GetLargeRand(10) + 5)*0.05f;
				o->Velocity[0] = fTemp;
				o->Velocity[1] = fTemp;
				o->Velocity[2] = 0.0f;
			}
			else if (o->SubType == 17)
			{
				o->TexType = BITMAP_EVENT_CLOUD;
				o->LifeTime = 500;

				o->Position[0] += (mu_float)(GetLargeRand(900) - 450);
				o->Position[1] += (mu_float)(GetLargeRand(900) - 450);
				o->Position[2] += (mu_float)(GetLargeRand(20) + 50);
				VectorCopy(o->Position, o->StartPosition);

				o->Rotation = GetLargeRand(360);
				o->Scale = (mu_float)(GetLargeRand(20) + 20) / 80.0f + Scale / 3.0f;

				mu_float fTemp = (mu_float)(GetLargeRand(10) + 5)*0.006f;
				o->Velocity[0] = fTemp;
				o->Velocity[1] = fTemp;
				o->Velocity[2] = 0.0f;

				VectorCopy(o->Light, o->TurningForce);
				o->Light[0] = 0.0f;
				o->Light[1] = 0.0f;
				o->Light[2] = 0.0f;
			}
			else if (o->SubType == 18)
			{
				o->TexType = BITMAP_CHROME + 2;

				o->LifeTime = 160;
				o->Light[0] = 0.0f;
				o->Light[1] = 0.0f;
				o->Light[2] = 0.0f;
				o->Gravity = (mu_float)(GetLargeRand(1000));

				VectorCopy(Position, o->Position);
				o->Position[2] += (mu_float)(GetLargeRand(80));
				o->Rotation = GetLargeRand(360);

				o->StartPosition[0] = (GetLargeRand(200) - 10) / 10.0f;
				o->StartPosition[1] = (GetLargeRand(200) - 10) / 10.0f;

				o->Scale = (mu_float)(GetLargeRand(70) + 5)*0.02f;
				o->TurningForce[0] = mu_float(GetLargeRand(40) + 10) / 10000.0f;
				o->TurningForce[1] = GetLargeRand(120) + 80;
				Vector(0.0f, 0.0f, 0.0f, o->Velocity);
			}
			else if (o->SubType == 19)
			{
				o->TexType = BITMAP_CHROME + 2;

				o->LifeTime = 60;
				o->Light[0] = 0.0f;
				o->Light[1] = 0.0f;
				o->Light[2] = 0.0f;
				o->Gravity = (mu_float)(GetLargeRand(1000));

				o->Position[0] += (mu_float)(GetLargeRand(500) - 250);//*Scale;
				o->Position[1] += (mu_float)(GetLargeRand(500) - 250);//*Scale;
				o->Position[2] += (mu_float)(GetLargeRand(20) + 20);
				o->Rotation = GetLargeRand(360);

				o->StartPosition[1] = (GetLargeRand(100)) / 100.0f;
				o->StartPosition[2] = o->Position[2];

				o->Scale = (mu_float)(GetLargeRand(90) + 220)*0.02f;
				o->TurningForce[0] = Scale + GetLargeRand(20) / 100.0f;
				Vector(0.0f, 0.0f, 0.0f, o->Velocity);
			}
			else if (o->SubType == 20)
			{
				o->LifeTime = 300;
				o->Gravity = (mu_float)(GetLargeRand(1000));
				o->Alpha = 0.1f;

				o->Position[0] += (mu_float)(GetLargeRand(500) - 250);
				o->Position[1] += (mu_float)(GetLargeRand(500) - 250);
				o->Position[2] += (mu_float)(GetLargeRand(20) - 20);

				o->StartPosition[1] = (GetLargeRand(100)) / 100.0f;
				o->StartPosition[2] = o->Position[2];

				o->Scale = (((mu_float)(GetLargeRand(20) + 180)*0.01f) * o->Scale);
				o->TurningForce[0] = GetLargeRand(10) / 100.0f;
				Vector(0.0f, 0.0f, 0.0f, o->Velocity);
			}
			else if (o->SubType == 21)
			{
				o->LifeTime = 100;
				o->Gravity = (mu_float)(GetLargeRand(1000));
				o->Alpha = 0.6f;

				o->Position[0] += (mu_float)(GetLargeRand(200) - 100);
				o->Position[1] += (mu_float)(GetLargeRand(200) - 100);
				o->Position[2] += (mu_float)(GetLargeRand(20) - 20);

				o->StartPosition[1] = (GetLargeRand(100)) / 100.0f;
				o->StartPosition[2] = o->Position[2];

				o->Scale = (((mu_float)(GetLargeRand(20) + 180)*0.01f) * o->Scale);
				o->TurningForce[0] = GetLargeRand(10) / 100.0f;
				Vector(0.0f, 0.0f, 0.0f, o->Velocity);
			}
			else if (o->SubType == 22)
			{
				o->LifeTime = 80;
				o->Gravity = (mu_float)(GetLargeRand(1000));
				o->Alpha = 0.1f;

				o->Position[0] += (mu_float)(GetLargeRand(20) - 10);
				o->Position[1] += (mu_float)(GetLargeRand(20) - 10);
				o->Position[2] += (mu_float)(GetLargeRand(20) - 20);

				o->Scale = (((mu_float)(GetLargeRand(20) + 50)*0.003f) * o->Scale);
				Vector(0.0f, 0.0f, 0.0f, o->TurningForce);

				mu_float fAngle = o->Angle[2] + GetLargeRand(360);
				EVector3 vAngle, vDirection;
				Vector(0.0f, 0.0f, fAngle, vAngle);
				AngleMatrix(vAngle, Matrix);
				Vector(0.0f, (GetLargeRand(10))*0.1f + 0.2f, 0.0f, vDirection);
				VectorRotate(vDirection, Matrix, o->Velocity);
				o->Velocity[2] = 0;
			}
			else if (o->SubType == 23)
			{
				o->LifeTime = 80;
				o->Gravity = (mu_float)(GetLargeRand(1000));
				o->Alpha = 0.1f;

				o->Position[0] += (mu_float)(GetLargeRand(6) - 3);
				o->Position[1] += (mu_float)(GetLargeRand(6) - 3);
				o->Position[2] += (mu_float)(GetLargeRand(6) - 3);

				o->Scale = 0;
				Vector(0.0f, 0.0f, 0.0f, o->TurningForce);
				o->TurningForce[0] = (GetLargeRand(2) == 0 ? 1.0f : -1.0f);

				mu_float fAngle = o->Angle[2] + 90 + (GetLargeRand(40)) - 20;
				EVector3 vAngle, vDirection;
				Vector(0.0f, 0.0f, fAngle, vAngle);
				AngleMatrix(vAngle, Matrix);
				Vector(0.0f, (GetLargeRand(10))*0.2f + 1.0f, 0.0f, vDirection);
				VectorRotate(vDirection, Matrix, o->Velocity);
				o->Velocity[2] = 0;
			}
			else
			{
				o->LifeTime = 30;
				o->Gravity = (mu_float)(GetLargeRand(1000));

				o->Position[0] += (mu_float)(GetLargeRand(500) - 250);//*Scale;
				o->Position[1] += (mu_float)(GetLargeRand(500) - 250);//*Scale;
				o->Position[2] += (mu_float)(GetLargeRand(20) + 20);

				o->StartPosition[1] = (GetLargeRand(100)) / 100.0f;
				o->StartPosition[2] = o->Position[2];

				o->Scale = (mu_float)(GetLargeRand(20) + 180)*0.01f;
				o->TurningForce[0] = Scale + GetLargeRand(30) / 100.0f;
				Vector(0.0f, 0.0f, 0.0f, o->Velocity);
			}
		break;
	case BITMAP_TORCH_FIRE:
		{
			o->LifeTime = GetLargeRand(5) + 60;
			o->Scale = (mu_float)(GetLargeRand(30) + 30) * 0.02f + Scale / 3.0f;

			o->Position[0] += (mu_float)(GetLargeRand(4) - 2) * 0.25f;
			o->Position[1] += (mu_float)(GetLargeRand(4) - 2) * 0.25f;
			o->Position[2] += (mu_float)(GetLargeRand(10) - 5);

			o->Gravity = (mu_float)(GetLargeRand(2) + 10)*0.5f;
		}
		break;

	case BITMAP_GHOST_CLOUD1:
	case BITMAP_GHOST_CLOUD2:
		{
			if (o->SubType == 0)
			{
				o->LifeTime = 500;
				o->Position[0] += (mu_float)(GetLargeRand(400) - 200);


				o->Position[1] += (mu_float)(GetLargeRand(400) - 200);
				o->Position[2] += (mu_float)(GetLargeRand(400) - 200);
				VectorCopy(o->Position, o->StartPosition);
				o->Scale = (mu_float)(GetLargeRand(15) + 15) / 30.0f + Scale;
				mu_float fTemp = (mu_float)(GetLargeRand(10) + 5)*0.08f;
				o->Velocity[0] = fTemp;
				o->Velocity[1] = fTemp;
				o->Velocity[2] = 0.0f;

				VectorCopy(o->Light, o->TurningForce);
				o->Light[0] = 0.0f;
				o->Light[1] = 0.0f;
				o->Light[2] = 0.0f;
			}
		}
		break;
	case BITMAP_LIGHT:
		if (0 == o->SubType || 8 == o->SubType)
		{
			o->LifeTime = GetLargeRand(10) + 10;
			o->Gravity = GetLargeRand(10) + 10.0f;
			o->Scale = (GetLargeRand(50) + 50.0f) / 100.0f*Scale;
		}
		else if (o->SubType == 9)
		{
			o->LifeTime = 40 * o->Scale;
		}
		else if (o->SubType == 7)
		{
			o->LifeTime = GetLargeRand(10) + 30;
			o->Gravity = GetLargeRand(10) + 10.0f;
			o->Scale = (GetLargeRand(15) + 30.0f) / 100.0f*Scale;
		}
		else if (6 == o->SubType)
		{
			o->LifeTime = GetLargeRand(10) + 10;
			o->Gravity = GetLargeRand(10) + 10.0f;
			o->Scale = (GetLargeRand(50) + 50.0f) / 100.0f*Scale;
		}
		else if (1 == o->SubType)
		{
			o->LifeTime = 50;
		}
		else if (2 == o->SubType)
		{
			o->LifeTime = GetLargeRand(10) + 10;
			o->Gravity = GetLargeRand(10) + 10.0f;
			o->Scale = (GetLargeRand(50) + 50.0f) / 100.0f*Scale;
		}
		else if (3 == o->SubType)
		{
			o->LifeTime = 10;
		}
		else if (o->SubType == 4)
		{
			o->LifeTime = 10;
			o->Gravity = 0.0f;
			o->Rotation = Scale;
			o->Scale = 2.0f;
		}
		else if (o->SubType == 5)
		{
			o->LifeTime = 50;
		}
		else if (o->SubType == 6)
		{
			o->LifeTime = 2;
		}
		else if (o->SubType == 10)
		{
			o->LifeTime = 100;
			o->Scale = Scale + (GetLargeRand(10))*0.02f;
		}
		else if (o->SubType == 11)
		{
			o->LifeTime = GetLargeRand(10) + 130;
			o->Gravity = GetLargeRand(2) + 2.0f;
			o->Scale = (GetLargeRand(20) + 20.0f) / 100.0f*Scale;
		}
		else if (o->SubType == 12)
		{
			o->LifeTime = GetLargeRand(10) + 100;
			o->Gravity = GetLargeRand(2) + 2.0f;
			o->Scale = (GetLargeRand(20) + 20.0f) / 100.0f*Scale;
		}
		else if (o->SubType == 13)
		{
			o->LifeTime = GetLargeRand(10) + 100;
			o->Gravity = GetLargeRand(2) + 2.0f;
			o->Scale = (GetLargeRand(20) + 20.0f) / 100.0f*Scale;
		}
		else if (o->SubType == 14)
		{
			o->LifeTime = 50;
			o->Scale = Scale + (GetLargeRand(10))*0.02f;
		}
		else if (o->SubType == 15)
		{
			o->LifeTime = GetLargeRand(10) + 100;
			o->Gravity = GetLargeRand(3) + 2.0f;
			o->Scale = (GetLargeRand(20) + 20.0f) / 100.0f*Scale;
			VectorCopy(Position, o->StartPosition);
			o->StartPosition[2] = 0;
			VectorCopy(Light, o->TurningForce);
			o->Alpha = 0;
		}
		break;
	case BITMAP_POUNDING_BALL:
		if (o->SubType == 0)
		{
			o->LifeTime = 24;
			Vector(0.0f, -(mu_float)(GetLargeRand(16) + 32)*0.1f, 0.0f, o->Velocity);
			o->Scale = (mu_float)(GetLargeRand(128) + 128)*0.01f;
			o->Rotation = (mu_float)(GetLargeRand(360));

			Vector(0.5f, 0.5f, 0.5f, o->Light);
		}
		else if (o->SubType == 1)
		{
			o->LifeTime = 15;
			Vector(0.0f, -(mu_float)(GetLargeRand(16) + 32)*0.1f, 0.0f, o->Velocity);
			o->Scale = (mu_float)(GetLargeRand(128) + 128)*0.01f;
			o->Rotation = (mu_float)(GetLargeRand(360));

			Vector(0.5f, 0.5f, 0.5f, o->Light);
		}
		else if (o->SubType == 2)
		{
			o->LifeTime = 20;
			o->Scale = (mu_float)(GetLargeRand(128) + 80)*0.009f;
			o->Rotation = (mu_float)(GetLargeRand(360));
			o->Gravity = 0.0f;
			Vector(0.0f, 0.0f, 0.0f, o->Velocity);
			Vector(0.5f, 0.5f, 0.5f, o->Light);
		}
		else if (o->SubType == 3)
		{
			o->LifeTime = GetLargeRand(5) + 47;
			o->Scale = (mu_float)(GetLargeRand(72) + 72)*0.01f*Scale;
			o->Rotation = (mu_float)(GetLargeRand(360));
			o->Gravity = (mu_float)(GetLargeRand(24) + 64)*0.1f;
			o->Alpha = 0;
			VectorCopy(Light, o->TurningForce);
			Vector(0, 0, 0, o->Light);
		}
		break;
	case BITMAP_ADV_SMOKE:
		o->LifeTime = 20 + GetLargeRand(5);
		o->Rotation = 0.0f;
		if (o->SubType == 0)
		{
			o->Scale = 0.5f + (mu_float)(GetLargeRand(10))*0.02f;
			o->Velocity[0] = (mu_float)(GetLargeRand(10) + 5)*0.4f;
			o->Velocity[1] = (mu_float)(GetLargeRand(10) - 5)*0.4f;
			o->Velocity[2] = (mu_float)(GetLargeRand(10) + 5)*0.2f;
		}
		else if (o->SubType == 2)
		{
			o->Scale = Scale*0.5f + (mu_float)(GetLargeRand(10))*0.02f;
			o->Velocity[0] = (mu_float)(GetLargeRand(10) + 5)*0.4f;
			o->Velocity[1] = (mu_float)(GetLargeRand(10) - 5)*0.4f;
			o->Velocity[2] = (mu_float)(GetLargeRand(10) + 5)*0.2f;
		}
		else if (o->SubType == 3)
		{
			o->Scale = Scale*0.5f + (mu_float)(GetLargeRand(10))*0.02f;
			o->Alpha = 0.5f;
			o->Velocity[0] = 0;
			o->Velocity[1] = (mu_float)(GetLargeRand(10) - 5)*0.4f;
			o->Velocity[2] = 0;
		}
		else
		{
			o->Scale = 1.0f + (mu_float)(GetLargeRand(10))*0.1f;
			o->Velocity[0] = (mu_float)(GetLargeRand(10) + 5)*0.2f;
			o->Velocity[1] = (mu_float)(GetLargeRand(10) - 5)*0.2f;
			o->Velocity[2] = (mu_float)(GetLargeRand(10) + 5)*0.1f;
		}
		break;
	case BITMAP_ADV_SMOKE + 1:
		o->LifeTime = 25 + GetLargeRand(5);
		o->Rotation = (mu_float)(GetLargeRand(360));
		o->Scale = 0.5f;
		if (o->SubType == 1)
		{
			o->Scale *= Scale;
		}
		o->Velocity[0] = (mu_float)(GetLargeRand(10) + 5)*0.4f;
		o->Velocity[1] = 0.0f;
		o->Velocity[2] = (mu_float)(GetLargeRand(10) + 5)*0.2f;

	case BITMAP_TRUE_FIRE:
	case BITMAP_TRUE_BLUE:
		if (o->SubType == 7)
		{
			o->Scale = Scale + (mu_float)(GetLargeRand(30)) / 100.0f;
			o->LifeTime = 15;
			o->Position[0] += (mu_float)(GetLargeRand(10)) / 10.0f - 0.5f;
			o->Position[1] += (mu_float)(GetLargeRand(10)) / 10.0f - 0.5f;
			o->Position[2] += (mu_float)(GetLargeRand(10)) / 10.0f - 0.5f;
		}
		else if (o->SubType == 5)
			o->LifeTime = 20;
		else if (o->SubType == 6)
			o->LifeTime = 32;
		else if (o->SubType == 8)
		{
			o->LifeTime = 15;
		}
		else if (o->SubType == 9)
		{
			o->LifeTime = 20;
		}
		else
		{
			o->LifeTime = 24;
		}

		if (o->SubType == 8 || o->SubType == 9)
		{
			o->Velocity[0] = (mu_float)(GetLargeRand(4) - 2)*0.4f;
			o->Velocity[1] = 0.0f;
			o->Velocity[2] = (mu_float)(GetLargeRand(4) + 2)*0.2f;
		}
		else
		{
			o->Velocity[0] = (mu_float)(GetLargeRand(10) - 5)*0.4f;
			o->Velocity[1] = 0.0f;
			o->Velocity[2] = (mu_float)(GetLargeRand(10) + 5)*0.2f;
		}

		VectorCopy(o->Position, o->StartPosition);
		break;

	case BITMAP_HOLE:
		o->LifeTime = 30;
		o->Rotation = (mu_float)(GetLargeRand(360));
		Vector(0.1f, 0.1f, 0.1f, o->Light);
		break;
	case BITMAP_WATERFALL_1:
		o->LifeTime = 30;
		o->Rotation = (mu_float)(GetLargeRand(360));
		o->Velocity[2] = (mu_float)(-(GetLargeRand(5) + 7));

		Vector(0.2f, 0.2f, 0.2f, o->Light);

		if (o->SubType == 0)
		{
			o->Scale = 1.6f;
		}
		else if (o->SubType == 1)
		{
			o->Scale = (mu_float)(GetLargeRand(20) + 80)*0.01f;
			o->TexType = BITMAP_CLOUD + 2;
		}
		else if (o->SubType == 2)
		{
			o->Scale = 1.6f + Scale;
		}
		break;

	case BITMAP_WATERFALL_5:
		if (o->SubType == 0)
		{
			o->LifeTime = 30;
			o->Rotation = (mu_float)(GetLargeRand(360));
			o->Scale = 1.6f;
			o->Velocity[2] = (mu_float)(-(GetLargeRand(5) + 7));

			Vector(0.2f, 0.2f, 0.2f, o->Light);
		}
		else if (o->SubType == 1)
		{
			o->LifeTime = 20;
			o->Rotation = (mu_float)(GetLargeRand(360));
			o->Scale = 1.0f;
			o->Position[2] += 50.0f;
		}
		else if (o->SubType == 2)
		{
			o->LifeTime = 20;
			o->Rotation = (mu_float)(GetLargeRand(360));
			o->Scale = 0.5f;
			o->Position[2] += 50.0f;
			o->Velocity[2] = (mu_float)((GetLargeRand(5) + 10));
		}
		else if (o->SubType == 3)
		{
			o->LifeTime = 20;
			o->Rotation = (mu_float)(GetLargeRand(360));
			o->Scale = 1.6f;
			o->Velocity[2] = (mu_float)(-(GetLargeRand(5) + 7));
		}
		else if (o->SubType == 4)
		{
			o->LifeTime = 6;
			o->Gravity = ((GetLargeRand(100)) / 100.0f)*4.0f + o->Angle[0] * 1.2f;//45.0f;
			o->Scale = Scale + (mu_float)(GetLargeRand(6))*0.20f;
			o->Rotation = (mu_float)(GetLargeRand(360));

			VectorCopy(o->Position, o->StartPosition);
		}
		else if (o->SubType == 5)
		{
			o->LifeTime = 30;
			o->Rotation = (mu_float)(GetLargeRand(360));
			o->Scale = 1.6f*Scale;
			o->Velocity[2] = (mu_float)(-(GetLargeRand(5) + 10));

			Vector(0.2f, 0.2f, 0.2f, o->Light);
		}
		else if (o->SubType == 6)
		{
			o->LifeTime = 30;
			o->Rotation = (mu_float)(GetLargeRand(360));
			o->Scale = Scale + 0.6f;
			o->Velocity[2] = -(GetLargeRand(5) + 12);

			Vector(0.2f, 0.2f, 0.2f, o->Light);
		}
		else if (o->SubType == 7)
		{
			o->TexType = GetLargeRand(2) == 0 ? BITMAP_WATERFALL_4 : BITMAP_WATERFALL_5;
			o->LifeTime = 30;
			o->Rotation = (mu_float)(GetLargeRand(360));
			o->Scale = (mu_float)(GetLargeRand(50))*0.05f + Scale;
			o->Velocity[0] = -(GetLargeRand(2) + 1);
			o->Velocity[1] = -(GetLargeRand(2) + 3);
			o->Velocity[2] = (GetLargeRand(3) + 3);
		}
		else if (o->SubType == 8)
		{
			o->LifeTime = 30;
			o->Velocity[2] = GetLargeRand(3) + 1;
			o->Scale = (GetLargeRand(5) + 5)*0.05f + Scale;
		}
		else if (o->SubType == 9)
		{
			o->LifeTime = 30;
			o->Rotation = (mu_float)(GetLargeRand(360));
			o->Scale = 0.6f + Scale;
			o->Velocity[2] = (mu_float)(-(GetLargeRand(5) + 7));

			Vector(0.2f, 0.2f, 0.2f, o->Light);
		}
		break;

	case BITMAP_PLUS:
		o->LifeTime = 20;
		o->Scale = Scale;
		o->Position[0] += GetLargeRand(30) - 15.0f;
		o->Position[1] += GetLargeRand(30) - 15.0f;
		o->Position[2] += 240.0f;
		break;

	case BITMAP_WATERFALL_2:
		o->LifeTime = 30;
		o->Rotation = (mu_float)(GetLargeRand(360));

		if (o->SubType == 2)
		{
			o->LifeTime = 40;
			o->Scale = (GetLargeRand(6) + 6)*0.1f + Scale;
		}
		else if (o->SubType == 6)
		{
			o->LifeTime = GetLargeRand(50) + 20;
			o->Gravity = (mu_float)(GetLargeRand(20) + 10);
			o->Scale *= 0.2f;
			VectorCopy(o->Position, o->StartPosition);
		}
		else
			o->Scale = (GetLargeRand(6) + 6)*0.1f;

		o->Velocity[2] = (mu_float)(-(GetLargeRand(3) + 3));

		if (g_CurrentScene == CHARACTER_SCENE)
		{
			Vector(0.25f, 0.25f, 0.25f, o->Light);
		}
		else
		{
			Vector(0.4f, 0.4f, 0.4f, o->Light);
		}

		o->Position[0] += GetLargeRand(20) - 10.0f;
		o->Position[1] += GetLargeRand(20) - 10.0f;
		o->Position[2] += GetLargeRand(40) - 20.0f;

		if (o->SubType == 1)
		{
			Vector(0.0f, 0.4f, 0.4f, o->Light);
			o->LifeTime = 50;
			o->Velocity[2] = (mu_float)(-(GetLargeRand(3) + 3));
		}
		if (o->SubType == 3)
		{
			o->TexType = BITMAP_LIGHT + 2;
			VectorCopy(Light, o->Light);
			o->LifeTime = 12;
			o->Rotation = (mu_float)(GetLargeRand(360));
			o->Scale = (GetLargeRand(3) + 3)*0.18f;
			o->Velocity[2] = 2.0f;

			o->Position[0] += GetLargeRand(20) - 10.0f;
			o->Position[1] += GetLargeRand(20) - 10.0f;
			o->Position[2] += GetLargeRand(40) - 20.0f;
		}
		if (o->SubType == 4)
		{
			o->LifeTime = 70;
			o->Scale = (GetLargeRand(6) + 6)*0.1f + Scale;
		}
		if (o->SubType == 5)
		{
			o->LifeTime = 40;
			o->Rotation = (mu_float)(GetLargeRand(360));
			o->Gravity = (GetLargeRand(2) + 2);
			o->Scale = (GetLargeRand(6) + 6)*0.1f + Scale;
			o->Velocity[0] = -(GetLargeRand(2) + 2);
			o->Velocity[1] = -(GetLargeRand(2) + 2);
			o->Velocity[2] = (GetLargeRand(2) + 1);
			o->Position[0] += GetLargeRand(60) - 30.0f;
			o->Position[1] += GetLargeRand(60) - 30.0f;
			o->Position[2] += GetLargeRand(10);
			break;
		}
		if (o->SubType == 11)
		{
			o->LifeTime = 30;
			o->Velocity[2] = GetLargeRand(5) + 5;
			o->Scale = (GetLargeRand(10) + 10)*0.05f * Scale;
		}
		break;

	case BITMAP_WATERFALL_3:
	case BITMAP_WATERFALL_4:
		if (o->SubType == 0)
		{
			o->LifeTime = 20;
			o->Velocity[2] = (mu_float)(GetLargeRand(5) + 2);
		}
		else if (o->SubType == 1)
		{
			o->LifeTime = 10;
			o->Velocity[2] = (mu_float)(GetLargeRand(2) + 2);
		}
		else if (o->SubType == 2)
		{
			o->LifeTime = 6;
			o->Gravity = ((GetLargeRand(100)) / 100.0f)*4.0f + o->Angle[0] * 1.2f;//45.0f;
			o->Scale = Scale + (mu_float)(GetLargeRand(6))*0.10f;
			o->Rotation = (mu_float)(GetLargeRand(360));

			VectorCopy(o->Position, o->StartPosition);
			break;
		}
		else if (o->SubType == 3)
		{
			o->LifeTime = 60;
			o->Velocity[2] = -1.0f;
			o->Scale = o->Scale*(GetLargeRand(10) + 15)*0.02f;
			o->Rotation = (mu_float)(GetLargeRand(360));
			o->Position[0] += GetLargeRand(40) - 20.0f;
			o->Position[2] += GetLargeRand(25) - 10.0f;
			break;
		}
		else if (o->SubType == 12)
		{
			o->Rotation = (mu_float)(GetLargeRand(360));
			o->LifeTime = (GetLargeRand(2) - 1) + 5;
			mu_float fIntervalScale = Scale*0.3f;
			o->Scale += ((mu_float((GetLargeRand(20)) - 10) * 0.5f)*((fIntervalScale * 0.5f)));
			o->Position[0] += (mu_float)(GetLargeRand(20)) - 10.0f;
			o->Position[1] += (mu_float)(GetLargeRand(20)) - 10.0f;
			o->Position[2] += (mu_float)(GetLargeRand(20)) - 10.0f;

			break;
		}

		o->Rotation = (mu_float)(GetLargeRand(360));
		o->Scale = (GetLargeRand(10) + 10)*0.02f;

		if (o->SubType == 4)
		{
			o->LifeTime = 20;
			o->Velocity[2] = GetLargeRand(5) + 2;
			o->Scale = (GetLargeRand(10) + 10)*0.02f + Scale;
		}
		else if (o->SubType == 5 || o->SubType == 6)
		{
			o->LifeTime = 20;
			o->Gravity = ((GetLargeRand(100)) / 100.0f)*2.0f;
			o->Scale = (GetLargeRand(10) + 10)*0.02f + Scale;
			o->Rotation = (mu_float)(GetLargeRand(360));
			o->Position[0] += GetLargeRand(40) - 20.0f;
			o->Position[1] += GetLargeRand(40) - 20.0f;
			o->Position[2] -= 50.0f;
			break;
		}
		else if (o->SubType == 7)
		{
			o->LifeTime = 60;
			o->Velocity[2] = (GetLargeRand(4))*0.1f + 0.8f;
			o->Scale = o->Scale*(GetLargeRand(10) + 15)*0.02f;
			o->Rotation = (mu_float)(GetLargeRand(360));
			o->Position[0] += GetLargeRand(18) - 9.0f;
			o->Position[2] += GetLargeRand(18) - 9.0f;
			break;
		}
		else if (o->SubType == 8)
		{
			o->LifeTime = 30;
			o->Velocity[2] = GetLargeRand(5) + 5;
			o->Scale = (GetLargeRand(10) + 10)*0.05f;
		}
		else if (o->SubType == 9)
		{
			o->LifeTime = 30;
			o->Velocity[2] = GetLargeRand(5) + 2;
			o->Scale = (GetLargeRand(5) + 5)*0.05f + Scale;
		}
		else if (o->SubType == 10)
		{
			o->LifeTime = 20;
			o->Gravity = ((GetLargeRand(200)) / 100.0f) + 2.0f;
			o->Scale = (GetLargeRand(10) + 10)*0.02f + Scale;
			o->Rotation = (mu_float)(GetLargeRand(360));
			o->Velocity[0] = -(GetLargeRand(2) + 1);
			o->Velocity[1] = -(GetLargeRand(2) + 2);
		}
		else if (o->SubType == 11)
		{
			o->LifeTime = 20;
			o->Scale = Scale + (GetLargeRand(10) - 5)*0.05f;
			o->Rotation = (mu_float)(GetLargeRand(360));

			EVector3 vAngle, vPos, vPos2;
			Vector(0.0f, 5.0f, 0.0f, vPos);
			mu_float fAngle = o->Angle[2] + (mu_float)(GetLargeRand(90) - 45) + 150;
			Vector(0.0f, 0.0f, fAngle, vAngle);
			AngleMatrix(vAngle, Matrix);
			VectorRotate(vPos, Matrix, vPos2);
			VectorCopy(vPos2, o->Velocity);

			break;
		}
		else if (o->SubType == 13)
		{
			o->LifeTime = 30;
			o->Scale = Scale;
			o->Rotation = (mu_float)(GetLargeRand(360));
			o->Gravity = 2.5f + (mu_float)(GetLargeRand(10)) * 0.1f;
			break;
		}
		else if (o->SubType == 14)
		{
			o->LifeTime = 30;
			o->Velocity[2] = GetLargeRand(5) + 5;
			o->Scale = (GetLargeRand(10) + 10)*0.05f * Scale;
		}
		else if (o->SubType == 15)
		{
			o->LifeTime = 20;
			o->Gravity = ((GetLargeRand(100)) / 100.0f)*2.0f;
			o->Scale = (GetLargeRand(10) + 10)*0.02f + Scale;
			o->Rotation = (mu_float)(GetLargeRand(360));
			o->Position[0] += GetLargeRand(40) - 20.0f;
			o->Position[1] += GetLargeRand(40) - 20.0f;
			o->Position[2] -= 50.0f;
			break;
		}

		o->Position[0] += GetLargeRand(40) - 20.0f;
		o->Position[1] += GetLargeRand(40) - 20.0f;
		o->Position[2] += GetLargeRand(20) - 10.0f;
		break;
	case BITMAP_SHOCK_WAVE:
		if (o->SubType == 3)
		{
			o->LifeTime = 7;
		}
		else if (o->SubType == 0)
		{
			o->LifeTime = 7;
			o->Scale = Scale;
			VectorCopy(Light, o->Light);
		}

		if (o->SubType == 4)
		{
			o->Alpha = 1.0f;
			o->LifeTime = 7;
			o->Scale = Scale;
			o->Gravity = 6.0f;
			VectorCopy(Light, o->Light);
		}
		break;
	case BITMAP_GM_AURORA:
		o->LifeTime = 20;
		break;
	case BITMAP_CURSEDTEMPLE_EFFECT_MASKER:
		{
			o->LifeTime = 30;
		}
		break;
	case BITMAP_RAKLION_CLOUDS:
		{
			o->Alpha = 1.0f;
			o->LifeTime = 32;
			o->Rotation = (mu_float)(GetLargeRand(360));
		}
		break;
	case BITMAP_CHROME2:
		{
			o->LifeTime = GetLargeRand(5) + 5;
			o->Rotation = GetLargeRand(360);
			VectorCopy(Position, o->StartPosition);
			o->Scale *= 1.0f + (GetLargeRand(10))*0.03f;
		}
		break;
	case BITMAP_AG_ADDITION_EFFECT:
		{
			mu_float _Scale = 1.0f;
			if (o->SubType == 0)
			{
				o->LifeTime = 33 + GetLargeRand(5);
				o->Rotation = (mu_float)(GetLargeRand(90)) + 270;
				_Scale = (GetLargeRand(20) + 20.0f) / 50.0f*0.5f;
			}
			else if (o->SubType == 1)
			{
				o->LifeTime = 27 + GetLargeRand(5);
				o->Rotation = (mu_float)(GetLargeRand(90));
				_Scale = (GetLargeRand(20) + 20.0f) / 50.0f*1.5f;
			}
			else if (o->SubType == 2)
			{
				o->LifeTime = 38 + GetLargeRand(5);
				o->Rotation = (mu_float)(GetLargeRand(90)) + 135;
				_Scale = (GetLargeRand(20) + 20.0f) / 50.0f*1.0f;
			}
			o->Scale = _Scale;
			o->Gravity = (mu_float)(GetLargeRand(16) + 12)*0.1f;
			o->Alpha = 0;
			Vector(1.0f, 0.0f, 0.6f, Light);
			VectorCopy(Light, o->TurningForce);
			Vector(0, 0, 0, o->Light);
		}
		break;
	}

	MUTexture *texture = &TEXTURES::Data[o->TexType];

	if (o->Type == BITMAP_EXPLOTION &&
		o->SubType == 5)
	{
		// FIX check if works same, it doesn't use depth test however i think it should use for fix image glitches
		o->RenderType = BLEND::TXB_BLEND01;
	}
	else if (o->Type == BITMAP_CLUD64 &&
		(o->SubType == 0 ||
			o->SubType == 5 ||
			o->SubType == 11))
	{
		o->RenderType = BLEND::TXB_BLENDMINUS;
	}
	else if (o->Type == BITMAP_SMOKE &&
		(o->SubType == 2 ||
			o->SubType == 5 ||
			o->SubType == 12 ||
			o->SubType == 14 ||
			o->SubType == 15 ||
			o->SubType == 20 ||
			o->SubType == 21 ||
			o->SubType == 29 ||
			o->SubType == 37 ||
			o->SubType == 38 ||
			o->SubType == 59))
	{
		o->RenderType = BLEND::TXB_BLENDMINUS;
	}
	else if (o->Type == BITMAP_SMOKE + 1 ||
		o->Type == BITMAP_SMOKE + 4)
	{
		o->RenderType = BLEND::TXB_ALPHATEST_NOMASK;
	}
	else if (o->Type == BITMAP_ADV_SMOKE + 1)
	{
		o->RenderType = BLEND::TXB_BLEND03;
	}
	else if (o->Type == BITMAP_TWINTAIL_WATER)
	{
		o->RenderType = BLEND::TXB_BLEND01;
	}
	else if (o->Type == BITMAP_CLUD64 &&
		(o->SubType == 0 ||
			o->SubType == 5 ||
			o->SubType == 11))
	{
		o->RenderType = BLEND::TXB_BLENDMINUS;
	}
	else if (o->Type == BITMAP_SMOKE + 3 &&
		(o->SubType == 3 ||
			o->SubType == 4))
	{
		o->RenderType = BLEND::TXB_BLENDMINUS;
	}
	else if (o->Type == BITMAP_SMOKE + 3)
	{
		o->RenderType = BLEND::TXB_BLEND03;
	}
	else if (o->Type == BITMAP_LIGHT + 2 &&
		(o->SubType == 3 ||
			o->SubType == 4 ||
			o->SubType == 6))
	{
		o->RenderType = BLEND::TXB_BLENDMINUS;
	}
	else if (o->Type == BITMAP_CLOUD &&
		(o->SubType == 10 ||
			o->SubType == 12 ||
			o->SubType == 7 ||
			o->SubType == 14 ||
			o->SubType == 16))
	{
		o->RenderType = BLEND::TXB_BLENDMINUS;
	}
	else if (o->Type == BITMAP_SPARK &&
		o->SubType == 10)
	{
		o->RenderType = BLEND::TXB_BLENDMINUS;
	}
	else if (o->Type == BITMAP_SMOKELINE2 &&
		o->SubType == 3)
	{
		o->RenderType = BLEND::TXB_BLENDMINUS;
	}
	else if (texture->TextureInternal.Components == 3)
	{
		o->RenderType = BLEND::TXB_BLEND01;
	}
	else
	{
		o->RenderType = BLEND::TXB_ALPHATEST_NOMASK;
	}
}

template void CreateParticle<false>(const mu_uint32 ThreadIndex, mu_uint32 Type, EVector3 &Position, EVector3 &Angle, EVector3 &Light, mu_uint32 SubType, mu_float Scale, OBJECT *Owner);
template void CreateParticle<true>(const mu_uint32 ThreadIndex, mu_uint32 Type, EVector3 &Position, EVector3 &Angle, EVector3 &Light, mu_uint32 SubType, mu_float Scale, OBJECT *Owner);

void DeleteParticle(mu_uint32 Type)
{
	mu_uint32 i = (mu_uint32)-1;

	g_ParticlesEraseLock.Lock();

	if (g_ParticlesEraseCount < MAX_JOINTS_WAIT)
	{
		i = g_ParticlesEraseCount++;
	}

	g_ParticlesEraseLock.Unlock();

	if (i != (mu_uint32)-1)
	{
		PARTICLE_DELETE &particle = g_ParticlesErase[i];
		particle.Type = Type;
	}
}