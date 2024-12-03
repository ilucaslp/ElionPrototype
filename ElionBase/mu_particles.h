#ifndef __MU_PARTICLES_H__
#define __MU_PARTICLES_H__

#pragma once

struct PARTICLE_WAIT
{
	mu_uint32 Type;
	mu_uint32 SubType;
	mu_float Scale;
	EVector3 Position;
	EVector3 Angle;
	EVector3 Light;
	OBJECT *Owner;
};

class PARTICLE_HEADER // Only flags data for fast check
{
public:
	MUFlagset<mu_uint8> Flags;
};

class PARTICLE_CHECK
{
public:
	mu_uint64 CreationTime;
};

class PARTICLE
{
public:
	enum PARTICLE_FLAGS
	{
		ePF_FREE = (1 << 0),
		ePF_LIVE = (1 << 1),
	};

	mu_uint16 Type;
	mu_uint16 SubType;
	mu_uint16 TexType;
	mu_uint16 RenderType;

	mu_int16 Frame;

	mu_int32 LifeTime;

	mu_float Scale;
	mu_float Alpha;
	mu_float Rotation;
	mu_float Gravity;

	EVector3 Position;
	EVector3 Angle;
	EVector3 Light;
	EVector3 Velocity;
	EVector3 TurningForce;
	EVector3 StartPosition;

	OBJECT *Target;
};

struct PARTICLE_DELETE
{
	mu_uint32 Type;
};

extern MUCriticalSectionSpinLock g_ParticlesLock;
extern std::array<PARTICLE_HEADER, MAX_PARTICLES> g_ParticlesHeader;
extern std::array<PARTICLE_CHECK, MAX_PARTICLES> g_ParticlesCheck;
extern std::array<PARTICLE, MAX_PARTICLES> g_Particles;
extern std::array<mu_uint16, MAX_PARTICLES + MAX_TASKING_THREADS> g_ParticlesQueue;
extern std::array<mu_uint16, MAX_PARTICLES> g_ParticlesFree;
extern mu_uint16 g_ParticlesCurCount;
extern mu_uint16 g_ParticlesFreeCount;
extern mu_atomic_int32_t g_ParticlesDeleteCount;
extern mu_boolean g_ParticlesLocked;

extern std::array<PARTICLE_WAIT, MAX_PARTICLES_WAIT> g_ParticlesWait;
extern mu_atomic_uint32_t g_ParticlesWaitCount;
extern std::array<PARTICLE_DELETE, MAX_PARTICLES_WAIT> g_ParticlesErase;
extern mu_uint16 g_ParticlesEraseCount;

void CreateBomb(const mu_uint32 ThreadIndex, EVector3 p, mu_boolean Exp, mu_uint32 SubType = 0);
void CreateBomb2(const mu_uint32 ThreadIndex, EVector3 p, mu_boolean Exp, mu_uint32 SubType = 0, mu_float Scale = 0.0f);
void CreateBomb3(const mu_uint32 ThreadIndex, EVector3 vPos, mu_uint32 iSubType);

void HandPosition(const mu_uint32 ThreadIndex, PARTICLE *o);

template<mu_boolean isLockedAllowed = true>
void CreateParticle(const mu_uint32 ThreadIndex, mu_uint32 Type, EVector3 &Position, EVector3 &Angle, EVector3 &Light, mu_uint32 SubType = 0, mu_float Scale = 1.0f, OBJECT *Owner = nullptr);
void DeleteParticle(mu_uint32 Type);

#endif