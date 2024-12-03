#include "stdafx.h"
#include "mu_tasking.h"

EVector3 g_vParticleWind = EVector3(0.0f, 0.0f, 0.0f);
EVector3 g_vParticleWindVelo = EVector3(0.0f, 0.0f, 0.0f);

void MUTasking::CheckMoveParticles(const mu_uint32 ThreadIndex)
{
	mu_uint32 queueCount = 0;
	const mu_uint32 queueIndex = MUThreadQueues::ParticlesQueueIndex[ThreadIndex];

	mu_uint32 beginIndex, endIndex;
	GetIndexTasking(ThreadIndex, MAX_PARTICLES, beginIndex, endIndex);

#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	mu_uint32 i = beginIndex;

#define RETRIEVE_HEADERFLAG(n) g_ParticlesHeader[i + n].Flags.GetFlags()
#if defined(_MSC_VER)
	if(cpuinfo_has_x86_avx2())
	{
#define CHECK_FLAG(n) if((value & (1 << n)) != 0) MUThreadQueues::ParticlesQueue[queueIndex + queueCount++] = i + n;
		const __m256i AVXFlags = _mm256_set1_epi8(PARTICLE::ePF_LIVE);
		constexpr mu_uint32 AVXBytes = 256 / 8;
		const mu_uint32 endIndexAVX = beginIndex + ((endIndex - beginIndex) / AVXBytes) * AVXBytes;
		for (; i < endIndexAVX; i += AVXBytes)
		{
			__m256i simdReg = _mm256_setr_epi8
			(
				RETRIEVE_HEADERFLAG(0),
				RETRIEVE_HEADERFLAG(1),
				RETRIEVE_HEADERFLAG(2),
				RETRIEVE_HEADERFLAG(3),
				RETRIEVE_HEADERFLAG(4),
				RETRIEVE_HEADERFLAG(5),
				RETRIEVE_HEADERFLAG(6),
				RETRIEVE_HEADERFLAG(7),
				RETRIEVE_HEADERFLAG(8),
				RETRIEVE_HEADERFLAG(9),
				RETRIEVE_HEADERFLAG(10),
				RETRIEVE_HEADERFLAG(11),
				RETRIEVE_HEADERFLAG(12),
				RETRIEVE_HEADERFLAG(13),
				RETRIEVE_HEADERFLAG(14),
				RETRIEVE_HEADERFLAG(15),
				RETRIEVE_HEADERFLAG(16),
				RETRIEVE_HEADERFLAG(17),
				RETRIEVE_HEADERFLAG(18),
				RETRIEVE_HEADERFLAG(19),
				RETRIEVE_HEADERFLAG(20),
				RETRIEVE_HEADERFLAG(21),
				RETRIEVE_HEADERFLAG(22),
				RETRIEVE_HEADERFLAG(23),
				RETRIEVE_HEADERFLAG(24),
				RETRIEVE_HEADERFLAG(25),
				RETRIEVE_HEADERFLAG(26),
				RETRIEVE_HEADERFLAG(27),
				RETRIEVE_HEADERFLAG(28),
				RETRIEVE_HEADERFLAG(29),
				RETRIEVE_HEADERFLAG(30),
				RETRIEVE_HEADERFLAG(31)
			);
			simdReg = _mm256_cmpeq_epi8(_mm256_and_si256(simdReg, AVXFlags), AVXFlags);

			const mu_uint32 value = static_cast<mu_uint32>(_mm256_movemask_epi8(simdReg));

			CHECK_FLAG(0);
			CHECK_FLAG(1);
			CHECK_FLAG(2);
			CHECK_FLAG(3);
			CHECK_FLAG(4);
			CHECK_FLAG(5);
			CHECK_FLAG(6);
			CHECK_FLAG(7);
			CHECK_FLAG(8);
			CHECK_FLAG(9);
			CHECK_FLAG(10);
			CHECK_FLAG(11);
			CHECK_FLAG(12);
			CHECK_FLAG(13);
			CHECK_FLAG(14);
			CHECK_FLAG(15);
			CHECK_FLAG(16);
			CHECK_FLAG(17);
			CHECK_FLAG(18);
			CHECK_FLAG(19);
			CHECK_FLAG(20);
			CHECK_FLAG(21);
			CHECK_FLAG(22);
			CHECK_FLAG(23);
			CHECK_FLAG(24);
			CHECK_FLAG(25);
			CHECK_FLAG(26);
			CHECK_FLAG(27);
			CHECK_FLAG(28);
			CHECK_FLAG(29);
			CHECK_FLAG(30);
			CHECK_FLAG(31);
		}

		const __m128i SSE2Flags = _mm_set1_epi8(PARTICLE::ePF_LIVE);
		constexpr mu_uint32 SSE2Bytes = 128 / 8;
		const mu_uint32 endIndexSSE2 = beginIndex + ((endIndex - beginIndex) / SSE2Bytes) * SSE2Bytes;
		for (; i < endIndexSSE2; i += SSE2Bytes)
		{
			__m128i simdReg = _mm_setr_epi8
			(
				RETRIEVE_HEADERFLAG(0),
				RETRIEVE_HEADERFLAG(1),
				RETRIEVE_HEADERFLAG(2),
				RETRIEVE_HEADERFLAG(3),
				RETRIEVE_HEADERFLAG(4),
				RETRIEVE_HEADERFLAG(5),
				RETRIEVE_HEADERFLAG(6),
				RETRIEVE_HEADERFLAG(7),
				RETRIEVE_HEADERFLAG(8),
				RETRIEVE_HEADERFLAG(9),
				RETRIEVE_HEADERFLAG(10),
				RETRIEVE_HEADERFLAG(11),
				RETRIEVE_HEADERFLAG(12),
				RETRIEVE_HEADERFLAG(13),
				RETRIEVE_HEADERFLAG(14),
				RETRIEVE_HEADERFLAG(15)
			);
			simdReg = _mm_cmpeq_epi8(_mm_and_si128(simdReg, SSE2Flags), SSE2Flags);

			const mu_uint32 value = static_cast<mu_uint32>(_mm_movemask_epi8(simdReg));

			CHECK_FLAG(0);
			CHECK_FLAG(1);
			CHECK_FLAG(2);
			CHECK_FLAG(3);
			CHECK_FLAG(4);
			CHECK_FLAG(5);
			CHECK_FLAG(6);
			CHECK_FLAG(7);
			CHECK_FLAG(8);
			CHECK_FLAG(9);
			CHECK_FLAG(10);
			CHECK_FLAG(11);
			CHECK_FLAG(12);
			CHECK_FLAG(13);
			CHECK_FLAG(14);
			CHECK_FLAG(15);
		}
#undef CHECK_FLAG
	}
	else
#endif
	{
#define CHECK_FLAG(n) if((value & (1 << n)) != 0) MUThreadQueues::ParticlesQueue[queueIndex + queueCount++] = i + n;
		const __m128i SSE2Flags = _mm_set1_epi8(PARTICLE::ePF_LIVE);
		constexpr mu_uint32 SSE2Bytes = 128 / 8;
		const mu_uint32 endIndexSSE2 = beginIndex + ((endIndex - beginIndex) / SSE2Bytes) * SSE2Bytes;
		for (; i < endIndexSSE2; i += SSE2Bytes)
		{
			__m128i simdReg = _mm_setr_epi8
			(
				RETRIEVE_HEADERFLAG(0),
				RETRIEVE_HEADERFLAG(1),
				RETRIEVE_HEADERFLAG(2),
				RETRIEVE_HEADERFLAG(3),
				RETRIEVE_HEADERFLAG(4),
				RETRIEVE_HEADERFLAG(5),
				RETRIEVE_HEADERFLAG(6),
				RETRIEVE_HEADERFLAG(7),
				RETRIEVE_HEADERFLAG(8),
				RETRIEVE_HEADERFLAG(9),
				RETRIEVE_HEADERFLAG(10),
				RETRIEVE_HEADERFLAG(11),
				RETRIEVE_HEADERFLAG(12),
				RETRIEVE_HEADERFLAG(13),
				RETRIEVE_HEADERFLAG(14),
				RETRIEVE_HEADERFLAG(15)
			);
			simdReg = _mm_cmpeq_epi8(_mm_and_si128(simdReg, SSE2Flags), SSE2Flags);

			const mu_uint32 value = static_cast<mu_uint32>(_mm_movemask_epi8(simdReg));
			
			CHECK_FLAG(0);
			CHECK_FLAG(1);
			CHECK_FLAG(2);
			CHECK_FLAG(3);
			CHECK_FLAG(4);
			CHECK_FLAG(5);
			CHECK_FLAG(6);
			CHECK_FLAG(7);
			CHECK_FLAG(8);
			CHECK_FLAG(9);
			CHECK_FLAG(10);
			CHECK_FLAG(11);
			CHECK_FLAG(12);
			CHECK_FLAG(13);
			CHECK_FLAG(14);
			CHECK_FLAG(15);
		}
#undef CHECK_FLAG
	}
#undef RETRIEVE_HEADERFLAG

	for (; i < endIndex; ++i)
	{
		const PARTICLE_HEADER *oh = &g_ParticlesHeader[i];

		if (oh->Flags.Get(PARTICLE::ePF_LIVE) == true)
		{
			MUThreadQueues::ParticlesQueue[queueIndex + queueCount++] = i;
		}
	}
#elif CPUINFO_ARCH_ARM || CPUINFO_ARCH_ARM64
	mu_uint32 i = beginIndex;

#define RETRIEVE_HEADERFLAG(n) g_ParticlesHeader[i + n].Flags.GetFlags()
#define CHECK_FLAG(n) if((value & (1 << n)) != 0) MUThreadQueues::ParticlesQueue[queueIndex + queueCount++] = i + n;
	const uint8x16_t NEONFlags = vdupq_n_u8(PARTICLE::ePF_LIVE);
	constexpr mu_uint32 NEONBytes = 128 / 8;
	const mu_uint32 endIndexNEON = beginIndex + ((endIndex - beginIndex) / NEONBytes) * NEONBytes;
	for (; i < endIndexNEON; i += NEONBytes)
	{
		const mu_uint8 flagsData[16] =
		{
			RETRIEVE_HEADERFLAG(0),
			RETRIEVE_HEADERFLAG(1),
			RETRIEVE_HEADERFLAG(2),
			RETRIEVE_HEADERFLAG(3),
			RETRIEVE_HEADERFLAG(4),
			RETRIEVE_HEADERFLAG(5),
			RETRIEVE_HEADERFLAG(6),
			RETRIEVE_HEADERFLAG(7),
			RETRIEVE_HEADERFLAG(8),
			RETRIEVE_HEADERFLAG(9),
			RETRIEVE_HEADERFLAG(10),
			RETRIEVE_HEADERFLAG(11),
			RETRIEVE_HEADERFLAG(12),
			RETRIEVE_HEADERFLAG(13),
			RETRIEVE_HEADERFLAG(14),
			RETRIEVE_HEADERFLAG(15)
		};
		uint8x16_t simdReg = vld1q_u8(flagsData);
		simdReg = vceqq_u8(vandq_u8(simdReg, NEONFlags), NEONFlags);

		const mu_uint32 value = static_cast<mu_uint32>(ESimdMoveMask8(simdReg));
			
		CHECK_FLAG(0);
		CHECK_FLAG(1);
		CHECK_FLAG(2);
		CHECK_FLAG(3);
		CHECK_FLAG(4);
		CHECK_FLAG(5);
		CHECK_FLAG(6);
		CHECK_FLAG(7);
		CHECK_FLAG(8);
		CHECK_FLAG(9);
		CHECK_FLAG(10);
		CHECK_FLAG(11);
		CHECK_FLAG(12);
		CHECK_FLAG(13);
		CHECK_FLAG(14);
		CHECK_FLAG(15);
	}
#undef RETRIEVE_HEADERFLAG
#undef CHECK_FLAG

	for (; i < endIndex; ++i)
	{
		const PARTICLE_HEADER *oh = &g_ParticlesHeader[i];

		if (oh->Flags.Get(PARTICLE::ePF_LIVE) == true)
		{
			MUThreadQueues::ParticlesQueue[queueIndex + queueCount++] = i;
		}
	}
#else
	for (mu_uint32 i = beginIndex; i < endIndex; ++i)
	{
		const PARTICLE_HEADER *oh = &g_ParticlesHeader[i];

		if (oh->Flags.Get(PARTICLE::ePF_LIVE) == true)
		{
			MUThreadQueues::ParticlesQueue[queueIndex + queueCount++] = i;
		}
	}
#endif

	MUThreadQueues::ParticlesQueueCount[ThreadIndex] = queueCount;
}

void MUTasking::MoveParticles(const mu_uint32 ThreadIndex)
{
	mu_uint32 q = ThreadIndex;
	for (mu_uint32 t = 0; t < _ThreadsCount; ++t)
	{
		const mu_uint32 queueCount = MUThreadQueues::ParticlesQueueCount[t];
		const mu_uint16 *queue = MUThreadQueues::ParticlesQueue.data() + MUThreadQueues::ParticlesQueueIndex[t];

		for (; q < queueCount; q += _ThreadsCount)
		{
			const mu_uint32 i = queue[q];

			PARTICLE_HEADER *oh = &g_ParticlesHeader[i];
			PARTICLE *o = &g_Particles[i];

			--o->LifeTime;

			EVector3 Position;
			EVector3 TargetPosition;
			EVector3 Light;
			mu_float Luminosity;
			mu_float Height;
			EBone Matrix;

			MovePosition(o->Position, o->Angle, o->Velocity);

			switch (o->Type)
			{
			case BITMAP_EFFECT:
				if (o->LifeTime >= 10)
				{
					if (o->SubType == 2)
					{
						o->Light[0] /= 1.03f;
						o->Light[1] /= 1.03f;
						o->Light[2] /= 1.03f;
					}
					else
					{
						o->Light[0] *= 1.16f;
						o->Light[1] *= 1.16f;
						o->Light[2] *= 1.16f;
					}
				}
				else
				{
					if (o->SubType == 2)
					{
						o->Light[0] /= 1.03f;
						o->Light[1] /= 1.03f;
						o->Light[2] /= 1.03f;
					}
					else
					{
						o->Light[0] /= 1.16f;
						o->Light[1] /= 1.16f;
						o->Light[2] /= 1.16f;
					}
				}
				o->Position[2] += o->Gravity;
				break;

			case BITMAP_FLOWER01:
			case BITMAP_FLOWER01 + 1:
			case BITMAP_FLOWER01 + 2:
				if (o->Frame == 0)
				{
					if (o->SubType == 0)
					{
						o->Velocity[0] += (mu_float)(GetLargeRand(32) - 16)*0.1f;
						o->Velocity[1] += (mu_float)(GetLargeRand(32) - 16)*0.1f;
						o->Velocity[2] += (mu_float)(GetLargeRand(32) - 16)*0.1f;
					}
					else if (o->SubType == 1)
					{
						o->Velocity[0] += (mu_float)(GetLargeRand(8) - 4)*0.05f;
						o->Velocity[1] += (mu_float)(GetLargeRand(8) - 4)*0.05f;
						o->Velocity[2] -= (mu_float)(GetLargeRand(8))*0.05f;
					}
					VectorAdd(o->Position, o->Velocity, o->Position);
				}
				Height = RequestTerrainHeight(o->Position[0], o->Position[1]);
				if (o->Position[2] < Height)
				{
					o->Position[2] = Height;
					Vector(0.0f, 0.0f, 0.0f, o->Velocity);
					o->Frame = 1;
				}
				o->Rotation += GetLargeRand(16);
				break;
			case BITMAP_FLARE_BLUE:
				if (o->SubType == 0)
				{
					o->Scale = 0.2f;//EMath::Sin( FWorldTime*0.001f )*0.2f+0.35f;

					o->Velocity[2] += 0.4f;
					o->Velocity[2] = EMath::Min(8.0f, o->Velocity[2]);

					if (o->LifeTime < 5)
					{
						o->Light[0] /= 1.2f;
						o->Light[1] /= 1.2f;
						o->Light[2] /= 1.2f;
					}
				}
				else if (o->SubType == 1)
				{
					o->Light[0] /= 1.1f;
					o->Light[1] /= 1.1f;
					o->Light[2] /= 1.1f;
					o->Scale += 1.5f;
				}
				break;
			case BITMAP_FLARE + 1:
				if (o->SubType == 0)
				{
					mu_float count = (o->Velocity[0] + o->LifeTime)*0.05f;
					o->Position[0] = o->StartPosition[0] + EMath::Sin(count)*(105.0f + o->Scale*-250);
					o->Position[1] = o->StartPosition[1] - EMath::Cos(count)*(105.0f + o->Scale*-250);
					o->Position[2] += o->Gravity;

					o->Scale -= 0.0008f;
				}
				break;
			case BITMAP_BLUE_BLUR:
				switch (o->SubType)
				{
				case 1:
				case 0:
					Luminosity = (mu_float)(o->LifeTime) / 20.0f;
					Vector(Luminosity*1.0f, Luminosity*1.0f, Luminosity*1.0f, o->Light);
					if (o->SubType == 1)
					{
						o->Scale += 0.19f;
						o->Position[2] += 5.00f;
					}
					else
						o->Scale += 0.05f;

					break;
				}
				break;
			case BITMAP_LIGHT + 2:
				if (o->SubType == 3)
				{
					o->Gravity += 0.2f;
					//o->Position[2] += o->Gravity;
					o->Scale -= 0.03f;

					if (o->Scale < 0 || !o->Target->Flags.Get(OBJECT::eOF_LIVE))
						oh->Flags.Clear(PARTICLE::ePF_LIVE);
					VectorSubtract(o->Position, o->StartPosition, o->Position);
					VectorCopy(o->Target->Position, o->StartPosition);
					VectorAdd(o->Position, o->StartPosition, o->Position);
				}
				else if (o->SubType == 5)
				{
					o->TurningForce[2] -= o->Gravity;

					o->Position[0] += o->TurningForce[0];
					o->Position[1] += o->TurningForce[1];
					o->Position[2] += o->TurningForce[2];

					o->Angle[0] += 0.5f * o->LifeTime;
					o->Angle[1] += 0.5f * o->LifeTime;
					o->Scale += 0.04f;
					if (o->LifeTime < 10)
					{
						o->Alpha -= 0.1f;
					}
					if (o->LifeTime < 20)
					{
						o->TurningForce[0] *= 0.6f;
						o->TurningForce[1] *= 0.6f;
					}
					Vector(o->Alpha * 1.0f, o->Alpha * 0.0f, o->Alpha * 0.6f, o->Light);

					if (o->Alpha < 0 || !o->Target->Flags.Get(OBJECT::eOF_LIVE))
						oh->Flags.Clear(PARTICLE::ePF_LIVE);
					VectorSubtract(o->Position, o->StartPosition, o->Position);
					VectorCopy(o->Target->Position, o->StartPosition);
					VectorAdd(o->Position, o->StartPosition, o->Position);
					o->Velocity[0] += 0.05f;
					o->Velocity[1] += 0.05f;

					o->Rotation += 1.0f;
				}
				else if (o->SubType == 6)
				{
					o->TurningForce[2] -= o->Gravity;

					o->Position[0] += o->TurningForce[0];
					o->Position[1] += o->TurningForce[1];
					o->Position[2] += o->TurningForce[2];

					o->Angle[0] += 0.5f * o->LifeTime;
					o->Angle[1] += 0.5f * o->LifeTime;
					o->Scale += 0.04f;
					if (o->LifeTime < 10)
					{
						o->Alpha -= 0.1f;
					}
					if (o->LifeTime < 20)
					{
						o->TurningForce[0] *= 0.6f;
						o->TurningForce[1] *= 0.6f;
					}
					Vector(o->Alpha * 1.0f, o->Alpha * 1.0f, o->Alpha * 1.0f, o->Light);

					if (o->Alpha < 0)
					{
						oh->Flags.Clear(PARTICLE::ePF_LIVE);
					}

					o->Velocity[0] += 0.05f;
					o->Velocity[1] += 0.05f;

					o->Rotation += 2.0f;
				}
				else if (o->SubType == 7)
				{
					o->TurningForce[2] -= o->Gravity;

					o->Position[0] += o->TurningForce[0];
					o->Position[1] += o->TurningForce[1];
					o->Position[2] += o->TurningForce[2];

					o->Angle[0] += 0.5f * o->LifeTime;
					o->Angle[1] += 0.5f * o->LifeTime;
					o->Scale += 0.04f;
					o->TexType = BITMAP_LIGHT;
					if (o->LifeTime < 10)
					{
						o->Alpha -= 0.1f;
					}
					if (o->LifeTime < 20)
					{
						o->TurningForce[0] *= 0.6f;
						o->TurningForce[1] *= 0.6f;
					}
					Vector(o->Alpha * o->Light[0], o->Alpha * o->Light[1], o->Alpha * o->Light[2], o->Light);

					if (o->Alpha < 0)
					{
						oh->Flags.Clear(PARTICLE::ePF_LIVE);
					}

					o->Velocity[0] += 0.05f;
					o->Velocity[1] += 0.05f;

					o->Rotation += 2.0f;

					Luminosity = (mu_float)(o->LifeTime) / 8.0f * 0.02f;
					o->Light[0] += Luminosity;
					o->Light[1] += Luminosity;
					o->Light[2] += Luminosity;
				}
				else
				{
					Luminosity = (mu_float)(o->LifeTime) / 8.0f * 0.02f;
					o->Light[0] += Luminosity;
					o->Light[1] += Luminosity;
					o->Light[2] += Luminosity;
					o->Gravity += 0.2f;
					o->Position[2] += o->Gravity;
					o->Scale -= 0.1f;
				}
				break;
			case BITMAP_GM_AURORA:
				{
					Luminosity = (mu_float)(o->LifeTime) / 8.0f * 0.04f;
					o->Light[0] -= Luminosity;
					o->Light[1] -= Luminosity;
					o->Light[2] -= Luminosity;
				}
				break;
			case BITMAP_MAGIC + 1:
				{
					o->Light[0] *= 0.9f;
					o->Light[1] *= 0.9f;
					o->Light[2] *= 0.9f;
					o->Scale += 0.1f;
				}
				break;
			case BITMAP_BUBBLE:
				switch (o->SubType)
				{
				case 0:
					++o->Frame;
					o->Position[0] += (mu_float)(GetLargeRand(20) - 10)*2.5f*o->Scale;
					o->Position[1] += (mu_float)(GetLargeRand(20) - 10)*2.5f*o->Scale;
					o->Position[2] += (mu_float)(GetLargeRand(20) + 10)*2.5f*o->Scale;
					break;

				case 1:
					++o->Frame;
					o->Position[0] += (mu_float)(GetLargeRand(30) - 15)*1.0f*o->Scale;
					o->Position[1] += (mu_float)(GetLargeRand(30) - 15)*1.0f*o->Scale;
					o->Position[2] += (mu_float)(GetLargeRand(20) + 10)*0.3f*o->Scale;
					o->Scale += 0.002f;
					break;

				case 2:
					o->Position[2] += (mu_float)(GetLargeRand(20) + 10)*0.3f;
					o->Scale += 0.005f;
					break;

				case 3:
					++o->Frame;
					o->Position[0] += (mu_float)(GetLargeRand(20) - 10)*2.5f*o->Gravity;
					o->Position[1] += (mu_float)(GetLargeRand(20) - 10)*2.5f*o->Gravity;
					o->Position[2] += (mu_float)(GetLargeRand(20) + 10)*2.5f*o->Gravity;
					break;
				case 4:
					o->Position[2] += (mu_float)(GetLargeRand(20) + 10)*0.3f;
					o->Scale += 0.005f;
					if (o->Position[2] > 350)
					{
						o->Position[2] = 350;
						o->LifeTime = 0;
					}
					break;
				case 5:
					++o->Frame;
					o->Position[0] += (mu_float)(GetLargeRand(20) - 10)*2.5f*o->Scale;
					o->Position[1] += (mu_float)(GetLargeRand(20) - 10)*2.5f*o->Scale;
					o->Position[2] += (mu_float)(GetLargeRand(20) + 10)*2.5f*o->Scale;
					if (o->Position[2] > 550)
						oh->Flags.Clear(PARTICLE::ePF_LIVE);
					break;
				}
				break;



			case BITMAP_EXPLOTION_MONO:
			case BITMAP_EXPLOTION:
				o->Frame = (20 - o->LifeTime) / 2;
				if (o->SubType == 2)
				{
					mu_int32 Life = 0;
					if (o->LifeTime > 10)
						Life = o->LifeTime - 10;
					else
						Life = 1;
					o->Frame = (20 - Life) / 2;
					EVector3  p, Angle;
					if (o->LifeTime == 20)
					{
						for (mu_int32 j = 0; j < 18; ++j)
						{
							Vector(0.0f, 200.0f, 0.0f, p);
							Vector(0.0f, 0.0f, j*20.0f, Angle);
							AngleMatrix(Angle, Matrix);
							VectorRotate(p, Matrix, Position);
							VectorAdd(Position, o->Position, Position);
							Vector(1.0f, 1.0f, 1.0f, Light);
							CreateParticle(ThreadIndex, BITMAP_SMOKE, Position, o->Angle, Light, 35, 2.5f);

							Vector(1.0f, 1.0f, 1.0f, Light);
							CreateParticle(ThreadIndex, BITMAP_EXPLOTION, Position, o->Angle, Light, 1);
							//							Vector(0.3f,0.3f,0.3f,Light);
							//							CreateParticle ( BITMAP_SMOKE+1, Position, o->Angle, Light );
						}
					}
					else
						if (o->LifeTime == 10)
						{
							for (mu_int32 j = 0; j < 18; ++j)
							{
								Vector(0.0f, 400.0f, 0.0f, p);
								Vector(0.0f, 0.0f, j*20.0f, Angle);
								AngleMatrix(Angle, Matrix);
								VectorRotate(p, Matrix, Position);
								VectorAdd(Position, o->Position, Position);
								Vector(0.6f, 0.6f, 0.6f, Light);
								CreateParticle(ThreadIndex, BITMAP_SMOKE, Position, o->Angle, Light, 35, 2.5f);
								CreateParticle(ThreadIndex, BITMAP_EXPLOTION, Position, o->Angle, Light, 1);
								//							Vector(0.3f,0.3f,0.3f,Light);
								//							CreateParticle ( BITMAP_SMOKE+1, Position, o->Angle, Light );

							}
						}
						else
							if (o->LifeTime == 1)
							{
								for (mu_int32 j = 0; j < 18; ++j)
								{
									Vector(0.0f, 600.0f, 0.0f, p);
									Vector(0.0f, 0.0f, j*20.0f, Angle);
									AngleMatrix(Angle, Matrix);
									VectorRotate(p, Matrix, Position);
									VectorAdd(Position, o->Position, Position);
									Vector(0.3f, 0.3f, 0.3f, Light);
									CreateParticle(ThreadIndex, BITMAP_SMOKE, Position, o->Angle, Light, 35, 2.5f);
									CreateParticle(ThreadIndex, BITMAP_EXPLOTION, Position, o->Angle, Light, 1);
								}
							}
				}
				if (o->SubType != 1)
				{
					Luminosity = (mu_float)(o->LifeTime) / 20.0f;
					Vector(Luminosity*0.5f, Luminosity*0.3f, Luminosity*0.1f, Light);
					AddTerrainLight(o->Position[0], o->Position[1], Light, 4, PrimaryTerrainLight);
				}
				break;
			case BITMAP_SUMMON_SAHAMUTT_EXPLOSION:
				o->Frame = (16 - o->LifeTime);
				break;
			case BITMAP_SPOT_WATER:
				o->Frame = (32 - o->LifeTime) / 4;
				break;
			case BITMAP_EXPLOTION + 1:
				o->Frame = (12 - o->LifeTime) / 3;
				break;
			case BITMAP_LIGHTNING + 1:
				o->Rotation = (mu_float)(IWorldTime % 1000)*0.001f;
				Luminosity = (mu_float)(o->LifeTime) / 10.0f;
				Vector(Luminosity*0.5f, Luminosity*1.0f, Luminosity*0.8f, Light);
				AddTerrainLight(o->Position[0], o->Position[1], Light, 3, PrimaryTerrainLight);
				if (o->SubType == 2)
				{
					o->Scale += 0.1f;
					Vector(Luminosity*0.5f, Luminosity*1.0f, Luminosity*0.8f, o->Light);
					VectorCopy(o->Target->Position, o->Position);
					o->Position[2] += 80.0f;
				}
				break;
			case BITMAP_LIGHTNING:
				o->Frame = GetLargeRand(4);
				Luminosity = (mu_float)(o->LifeTime) / 5.0f;
				Vector(Luminosity*1.0f, Luminosity*1.0f, Luminosity*1.0f, o->Light);
				Vector(-Luminosity * 0.6f, -Luminosity * 0.6f, -Luminosity * 0.6f, Light);
				AddTerrainLight(o->Position[0], o->Position[1], Light, 6, PrimaryTerrainLight);
				Vector(Luminosity*0.2f, Luminosity*0.4f, Luminosity*1.0f, Light);
				AddTerrainLight(o->Position[0], o->Position[1], Light, 4, PrimaryTerrainLight);
				break;
			case BITMAP_CHROME_ENERGY2:
				o->Gravity = 0.0f;
				Luminosity = (mu_float)(o->LifeTime) / 24.0f;
				o->Scale -= 0.04f;
				o->Rotation += 5;
				o->Frame = (23 - o->LifeTime) / 6;
				break;
			case BITMAP_FIRE_CURSEDLICH:
			case BITMAP_FIRE_HIK2_MONO:
				Luminosity = o->Alpha;

				if (o->SubType == 0)
				{
					o->Scale -= (GetLargeRand(20) + 10)*0.001f;
					Luminosity -= 0.05f;
					o->Position[2] += o->Gravity*10.0f;
				}
				else if (o->SubType == 1)
				{
					o->Scale -= (GetLargeRand(5) + 15)*0.0016f;
					o->Position[2] += o->Gravity*10.0f;

					if (o->Scale < 0 || !o->Target->Flags.Get(OBJECT::eOF_LIVE))
						oh->Flags.Clear(PARTICLE::ePF_LIVE);
					VectorSubtract(o->Position, o->StartPosition, o->Position);
					VectorCopy(o->Target->Position, o->StartPosition);
					VectorAdd(o->Position, o->StartPosition, o->Position);
				}
				else if (o->SubType == 2)
				{
					o->Scale -= (GetLargeRand(20) + 10)*0.004f;
					Luminosity -= 0.05f;
					o->Position[2] += o->Gravity*25.0f;
				}
				else if (o->SubType == 3)
				{
					o->Scale -= 0.03f;
					Luminosity -= 0.05f;
					o->Position[2] += o->Gravity*10.0f;

					if (o->Scale < 0)
						oh->Flags.Clear(PARTICLE::ePF_LIVE);

					if (o->Target->Flags.Get(OBJECT::eOF_LIVE))
					{
						VectorSubtract(o->Position, o->StartPosition, o->Position);
						VectorCopy(o->Target->Position, o->StartPosition);
						VectorAdd(o->Position, o->StartPosition, o->Position);
					}
				}
				else if (o->SubType == 4)
				{
					if (o->LifeTime < 10)
					{
						o->Alpha -= 0.2f;
					}
					else if (o->Alpha < 1.0f)
					{
						o->Alpha += (GetLargeRand(2) + 2)*0.1f;
					}
					else
					{
						o->Alpha = 1.0f;
					}

					if (o->Alpha < 0.1f)
						oh->Flags.Clear(PARTICLE::ePF_LIVE);

					Vector(o->TurningForce[0] * Luminosity, o->TurningForce[1] * Luminosity, o->TurningForce[2] * Luminosity, o->Light);
					if (o->Scale > 0)
					{
						o->Scale -= (GetLargeRand(3) + 6)*0.01f;
					}
					else
					{
						oh->Flags.Clear(PARTICLE::ePF_LIVE);
					}
					o->Position[2] += o->Gravity;
					o->Rotation += 3.0f;
				}
				else if (o->SubType == 5)
				{
					if (o->LifeTime < 15)
					{
						o->Alpha -= 0.2f;
					}
					else if (o->Alpha < 1.0f)
					{
						o->Alpha += (GetLargeRand(2) + 2)*0.1f;
					}
					else
					{
						o->Alpha = 1.0f;
					}

					if (o->Alpha < 0.1f)
						oh->Flags.Clear(PARTICLE::ePF_LIVE);

					Vector(o->TurningForce[0] * Luminosity, o->TurningForce[1] * Luminosity, o->TurningForce[2] * Luminosity, o->Light);
					if (o->Scale > 0)
					{
						o->Scale -= (GetLargeRand(2) + 5)*0.01f;
					}
					else
					{
						oh->Flags.Clear(PARTICLE::ePF_LIVE);
					}
					o->Position[2] += o->Gravity;
					o->Rotation += 3.0f;
				}
				else if (o->SubType == 6)
				{
					if (o->LifeTime < 10)
					{
						o->Alpha -= 0.2f;
					}
					else if (o->Alpha < 1.0f)
					{
						o->Alpha += (GetLargeRand(2) + 2)*0.1f;
					}
					else
					{
						o->Alpha = 1.0f;
					}

					if (o->Alpha < 0.1f)
						oh->Flags.Clear(PARTICLE::ePF_LIVE);

					Vector(o->TurningForce[0] * Luminosity, o->TurningForce[1] * Luminosity, o->TurningForce[2] * Luminosity, o->Light);
					if (o->Scale > 0)
					{
						o->Scale -= (GetLargeRand(3) + 6)*0.01f;
					}
					else
					{
						oh->Flags.Clear(PARTICLE::ePF_LIVE);
					}
					o->Position[0] += o->Velocity[0];
					o->Position[1] += o->Velocity[1];
					o->Position[2] += o->Gravity;
					o->Rotation += 3.0f;
				}
				else if (o->SubType == 7)
				{
					if (o->LifeTime < 10)
					{
						o->Alpha -= 0.2f;
					}
					else if (o->Alpha < 1.0f)
					{
						o->Alpha += (GetLargeRand(2) + 2)*0.1f;
					}
					else
					{
						o->Alpha = 1.0f;
					}

					if (o->Alpha < 0.1f)
						oh->Flags.Clear(PARTICLE::ePF_LIVE);

					Vector(o->TurningForce[0] * Luminosity, o->TurningForce[1] * Luminosity, o->TurningForce[2] * Luminosity, o->Light);
					if (o->Scale > 0)
					{
						o->Scale -= (GetLargeRand(3) + 3)*0.01f;
					}
					else
					{
						oh->Flags.Clear(PARTICLE::ePF_LIVE);
					}
					o->Position[2] += o->Gravity;
					o->Rotation += 3.0f;
				}
				else if (o->SubType == 8)
				{
					if (o->LifeTime < 5)
					{
						o->Alpha -= 0.1f;
					}
					else if (o->Alpha < 1.0f)
					{
						o->Alpha += (GetLargeRand(2) + 2)*0.1f;
					}
					else
					{
						o->Alpha = 1.0f;
					}

					if (o->Alpha < 0.1f)
						oh->Flags.Clear(PARTICLE::ePF_LIVE);

					Vector(o->TurningForce[0] * Luminosity, o->TurningForce[1] * Luminosity, o->TurningForce[2] * Luminosity, o->Light);

					o->Scale += (GetLargeRand(3) + 8)*0.01f;
					if (o->LifeTime < 3)
					{
						o->Scale += (10 - o->LifeTime)*0.02f;
					}

					AngleMatrix(o->Angle, Matrix);
					EVector3 vMoveDir, vMoveVec;
					Vector(0.0f, -1.0f, 0.0f, vMoveDir);
					VectorRotate(vMoveDir, Matrix, vMoveVec);
					VectorScale(vMoveVec, 8.0f, vMoveVec);
					VectorAdd(o->Position, vMoveVec, o->Position);

					VectorSubtract(o->Position, o->StartPosition, o->Position);
					VectorCopy(o->Target->Position, o->StartPosition);
					VectorAdd(o->Position, o->Target->Position, o->Position);

					o->Rotation += 5.0f;
				}
				break;
			case BITMAP_LEAF_TOTEMGOLEM:
				{
					o->Velocity[2] += o->Gravity;

					Height = RequestTerrainHeight(o->Position[0], o->Position[1]) + 20;
					if (o->Position[2] <= Height)
					{
						Vector(0, 0, 0, o->Velocity);
						o->Position[2] = Height;
						o->Alpha -= 0.05f;
						Vector(o->Alpha, o->Alpha, o->Alpha, o->Light);
					}
					else
					{
						o->Scale += 0.01f;
					}
				}
				break;
			case BITMAP_FIRE:
			case BITMAP_FIRE + 2:
			case BITMAP_FIRE + 3:
				switch (o->SubType)
				{
				case    0:
					{
						o->Gravity += 0.004f;
						Luminosity = (mu_float)(o->LifeTime) / 24.0f;
						o->Scale -= 0.04f;
						o->Frame = (23 - o->LifeTime) / 6;
						o->Position[2] += o->Gravity*10.0f;
					}
					break;
				case	17:
				case    5:
				case    6:
					{
						o->Gravity += 0.004f;
						Luminosity = (mu_float)(o->LifeTime) / 24.0f;
						o->Scale -= 0.04f;
						o->Rotation += 5;
						o->Frame = (23 - o->LifeTime) / 6;
						o->Position[2] += o->Gravity*10.0f;
					}
					break;
				case    7:
					{
						o->Gravity += 0.004f;
						Luminosity = (mu_float)(o->LifeTime) / 24.0f;
						o->Frame = (15 - o->LifeTime) / 6;
						o->Scale -= 0.04f;
						o->Frame = (23 - o->LifeTime) / 6;
						o->Position[2] += o->Gravity*10.0f;
					}
					break;
				case    8:
					{
						o->Gravity += 0.004f;
						Luminosity = (mu_float)(o->LifeTime) / 24.0f;
						o->Scale *= 0.95f;
						o->Rotation += 5;
						o->Frame = (23 - o->LifeTime) / 6;
						o->Position[2] += o->Gravity*10.0f;
					}
					break;
				case    9:
					{
						o->Gravity += 0.004f;
						Luminosity = (mu_float)(o->LifeTime) / 24.0f;
						o->Position[0] = o->Target->Position[0] + o->StartPosition[0];
						o->Position[1] = o->Target->Position[1] + o->StartPosition[1];
						o->Position[2] = o->Target->Position[2] + o->StartPosition[2];
						o->Gravity += ((GetLargeRand(60)) + 60) / 100;
						o->Scale -= o->Gravity / 90.0f;
						o->Frame = (23 - o->LifeTime) / 6;
						o->Position[2] += o->Gravity*10.0f;
					}
					break;
				case    11:
					{
						o->Gravity += 0.004f;
						Luminosity = (mu_float)(o->LifeTime) / 24.0f;
						if (o->LifeTime > 12)
						{
							o->Frame = (24 - o->LifeTime) / 3;
						}
						o->Scale += 0.04f;
						o->Frame = (23 - o->LifeTime) / 6;
						o->Position[2] += o->Gravity*10.0f;
					}
					break;
				case	10:
					{
						o->Gravity += 0.004f;
						Luminosity = (mu_float)(o->LifeTime) / 24.0f;
						o->Scale *= 0.95f;
						o->Frame = (23 - o->LifeTime) / 6;
						o->Position[2] += o->Gravity*10.0f;
					}
					break;
				case    12:
					{
						o->Gravity += 0.004f;
						Luminosity = (mu_float)(o->LifeTime) / 24.0f;
						o->Rotation += (mu_float)(GetLargeRand(10) + 10.0f);
						o->Scale -= 0.04f;
						Vector(Luminosity, Luminosity, Luminosity, o->Light);
						o->Frame = (23 - o->LifeTime) / 6;
						o->Position[2] += o->Gravity*10.0f;
					}
					break;
				case    13:
					{
						o->Gravity += 0.004f;
						Luminosity = (mu_float)(o->LifeTime) / 24.0f;
						o->Rotation += (mu_float)(GetLargeRand(10) + 10.0f);
						o->Scale -= 0.04f;
						Vector(Luminosity, Luminosity, Luminosity, o->Light);
						o->Frame = (23 - o->LifeTime) / 6;
						o->Position[2] += o->Gravity*10.0f;
					}
					break;
				case 14:
					{
						o->Gravity += 0.004f;
						Luminosity = (mu_float)(o->LifeTime) / 24.0f;
						o->Rotation += o->Gravity;
						o->Scale += 0.03f;
						o->Light[0] /= 1.1f;
						o->Light[1] /= 1.1f;
						o->Light[2] /= 1.1f;
						if (o->Light[2] <= 0.05f || o->Scale <= 0.0f)
							oh->Flags.Clear(PARTICLE::ePF_LIVE);
						o->Frame = (23 - o->LifeTime) / 6;
						o->Position[2] += o->Gravity*10.0f;
					}
					break;
				case 15:
					{
						o->Gravity += 0.004f;
						Luminosity = (mu_float)(o->LifeTime) / 24.0f;
						o->Scale -= 0.04f;
						o->Light[0] /= 1.1f;
						o->Light[1] /= 1.1f;
						o->Light[2] /= 1.1f;
						if (o->Light[2] <= 0.05f || o->Scale <= 0.0f)
							oh->Flags.Clear(PARTICLE::ePF_LIVE);
						o->Frame = (23 - o->LifeTime) / 6;
						o->Position[2] += o->Gravity*10.0f;
					}
					break;
				case 16:
					{
						o->Frame = (3 - o->LifeTime);
						o->Light[0] /= 1.7f;
						o->Light[1] /= 1.7f;
						o->Light[2] /= 1.7f;
						o->Scale *= 1.1f;
					}break;
				default:
					{
						o->Gravity += 0.004f;
						Luminosity = (mu_float)(o->LifeTime) / 24.0f;
						o->Scale += o->Gravity;
						VectorScale(o->Velocity, 0.98f, o->Velocity);
						o->Frame = (23 - o->LifeTime) / 6;
						o->Position[2] += o->Gravity*10.0f;
					}
					break;
				}
				break;
			case BITMAP_FIRE + 1:
				if (o->SubType == 1)
				{
					//					o->Gravity += 0.02f;
					//					o->Scale += o->Gravity;
					VectorScale(o->Velocity, 1.05f, o->Velocity);
					o->Position[2] += o->Gravity*20.0f;
					Luminosity = (mu_float)(o->LifeTime)*0.2f;
					Vector(Luminosity, Luminosity, Luminosity, o->Light);
				}
				else if (o->SubType == 2)
				{
					//					o->Gravity += 0.02f;
					//					o->Scale += o->Gravity;
					//     				VectorScale(o->Velocity,1.05f,o->Velocity);
					//					o->Position[2] += o->Gravity*20.0f;
					Luminosity = (mu_float)(o->LifeTime)*0.2f;
					Vector(Luminosity, Luminosity, Luminosity, o->Light);
				}
				else if (o->SubType == 3)
				{
					o->Gravity += 0.02f;
					o->Scale += o->Gravity;
					VectorScale(o->Velocity, 1.05f, o->Velocity);
					o->Position[2] += o->Gravity*20.0f;
					Luminosity = (mu_float)(o->LifeTime)*0.2f;
					Vector(Luminosity, Luminosity, Luminosity, o->Light);
				}
				else if (o->SubType == 4)
				{
					//     				Luminosity = (mu_float)(o->LifeTime)/5.0f;
					//					Vector(o->TurningForce[0]*Luminosity,o->TurningForce[1]*Luminosity,o->TurningForce[2]*Luminosity,o->Light);
					mu_float count = (o->Velocity[0] + o->LifeTime)*0.1f;
					o->Position[0] = o->StartPosition[0] + EMath::Sin(count)*120.0f;
					o->Position[1] = o->StartPosition[1] - EMath::Cos(count)*120.0f;
					o->Position[2] += o->Gravity;
					o->Scale -= 0.002f;
				}
				else if (o->SubType == 5)
				{
					//					Luminosity = (mu_float)(o->LifeTime)*0.2f;
					//					Vector(o->TurningForce[0]*Luminosity,o->TurningForce[1]*Luminosity,o->TurningForce[2]*Luminosity,o->Light);
					o->Position[0] += (mu_float)(EMath::Cos(o->Angle[2]) * 20.0f);
					o->Position[1] += (mu_float)(EMath::Sin(o->Angle[2]) * 20.0f);
					o->Position[2] += o->Gravity;
					o->Rotation += 1;
					//o->Scale += 0.003f;
				}
				else if (o->SubType == 6)
				{
					o->Position[2] += o->Gravity;
					o->Rotation += 4;
				}
				else if (o->SubType == 7)
				{
					VectorScale(o->Velocity, 1.05f, o->Velocity);
					Luminosity = (mu_float)(o->LifeTime)*0.2f;
					Vector(Luminosity, Luminosity, Luminosity, o->Light);
				}
				else if (o->SubType == 8 || o->SubType == 9)
				{
					o->Scale += 0.13f;
					o->Rotation += o->Gravity;
					o->Light[0] /= 1.13f;
					o->Light[1] /= 1.13f;
					o->Light[2] /= 1.13f;
				}
				else if (o->SubType == 0)
				{
					o->Gravity += 0.02f;
					o->Scale += o->Gravity;
					VectorScale(o->Velocity, 1.05f, o->Velocity);
					o->Position[2] += o->Gravity*20.0f;
					Luminosity = (mu_float)(o->LifeTime)*0.2f;
					Vector(Luminosity, Luminosity, Luminosity, o->Light);
				}
				break;

			case BITMAP_FLAME:
				if (o->LifeTime <= 0) oh->Flags.Clear(PARTICLE::ePF_LIVE);

				switch (o->SubType)
				{
				case 1:
					if (o->LifeTime == 10)
					{
						o->Velocity[1] += 32 * 0.2f;
						o->Scale -= 0.15f;
					}
					o->Rotation = (mu_float)(GetLargeRand(360));
					o->Light[0] -= 0.05f;
					o->Light[1] -= 0.05f;
					o->Light[2] -= 0.05f;
					break;
				case 5:
					if (o->LifeTime == 10)
					{
						o->Velocity[1] += 32 * 0.2f;
						o->Scale *= 0.8f;
					}
					o->Rotation = (mu_float)(GetLargeRand(360));
					o->Light[0] -= 0.05f;
					o->Light[1] -= 0.05f;
					o->Light[2] -= 0.05f;
					break;
				case 2:
					if (o->LifeTime < 10)
					{
						//                        o->Light[0] /= (15-o->LifeTime);
						//                        o->Light[1] /= (15-o->LifeTime);
						//                        o->Light[2] /= (15-o->LifeTime);
					}
					o->Rotation = (mu_float)(GetLargeRand(360));
					o->Light[0] -= 0.05f;
					o->Light[1] -= 0.05f;
					o->Light[2] -= 0.05f;
					break;

				case 3:
					o->Rotation = (mu_float)(GetLargeRand(360));
					break;

				case 4:
					o->Position[0] += o->Target->Position[0] - o->StartPosition[0];
					o->Position[1] += o->Target->Position[1] - o->StartPosition[1];
					o->Position[2] += o->Target->Position[2] - o->StartPosition[2] + o->Gravity;
					VectorCopy(o->Target->Position, o->StartPosition);
					o->Gravity += 0.1f;
					o->Rotation = (mu_float)(GetLargeRand(360));
					o->Light[0] -= 0.05f;
					o->Light[1] -= 0.05f;
					o->Light[2] -= 0.05f;
					break;
				case 8:
					{
						if (o->StartPosition[0] < o->Position[0])
							o->Rotation += 2.0f;
						else
							o->Rotation -= 2.0f;

						o->Position[2] += o->Gravity * 0.5f;
						o->Scale -= o->Gravity / 95.0f;
						if (o->Scale <= 0.0f)
							oh->Flags.Clear(PARTICLE::ePF_LIVE);

						o->Light[0] /= 1.007f;
						o->Light[1] /= 1.007f;
						o->Light[2] /= 1.007f;
					}
					break;
				case 7:
					{
						if (o->StartPosition[0] < o->Position[0])
							o->Rotation += 2.0f;
						else
							o->Rotation -= 2.0f;

						o->Position[2] += o->Gravity;
						o->Scale -= o->Gravity / 95.0f;
						if (o->Scale <= 0.0f)
							oh->Flags.Clear(PARTICLE::ePF_LIVE);

						o->Light[0] /= 1.007f;
						o->Light[1] /= 1.007f;
						o->Light[2] /= 1.007f;
					}
					break;
				case 9:
					{
						if (o->StartPosition[0] < o->Position[0])
							o->Rotation += 0.5f;
						else
							o->Rotation -= 0.5f;

						o->Position[2] += o->Gravity*1.2f;
						o->Scale -= o->Gravity / 98.0f;
						if (o->Scale <= 0.0f)
							oh->Flags.Clear(PARTICLE::ePF_LIVE);

						o->Light[0] /= 1.008f;
						o->Light[1] /= 1.008f;
						o->Light[2] /= 1.008f;
					}
					break;
				case 10:
					VectorAdd(o->Position, o->Velocity, o->Position);
					o->Velocity[0] *= 0.95f;
					o->Velocity[1] *= 0.95f;
					o->Light[0] = (mu_float)(o->LifeTime) / 10.0f;
					o->Light[1] = o->Light[0];
					o->Light[2] = o->Light[0];
					o->Velocity[2] += 0.3f;
					o->Scale += 0.07f;
					break;
				case 11:
					o->Light[0] /= 1.008f;
					o->Light[1] /= 1.008f;
					o->Light[2] /= 1.008f;
					break;
				}
				break;

			case BITMAP_FIRE_RED:
				if (o->LifeTime <= 0)
					oh->Flags.Clear(PARTICLE::ePF_LIVE);
				break;
			case BITMAP_RAIN_CIRCLE:
			case BITMAP_RAIN_CIRCLE + 1:
				if (o->LifeTime <= 0) oh->Flags.Clear(PARTICLE::ePF_LIVE);
				if (o->SubType == 1)
				{
					o->Scale += o->Gravity;
				}
				else
				{
					o->Scale += 0.03f;
				}
				if (M34CryWolf1st::IsCyrWolf1st() == true)
				{
					Vector(1.0f, 1.0f, 0.7f, o->Light);
				}
				else
				{
					if (o->SubType == 2)
					{
						Vector(0.03f, 0.03f, 0.03f, Light);
						VectorSubtract(o->Light, Light, o->Light);
					}
					else
					{
						Vector(0.05f, 0.05f, 0.05f, Light);
						VectorSubtract(o->Light, Light, o->Light);
					}
				}
				break;

			case BITMAP_ENERGY:
				{
					o->Rotation += o->Gravity;

					if (o->SubType == 1)
					{
						o->Light[1] = o->Light[2] = o->Light[0] = o->LifeTime / 15.0f;
						o->Scale += 0.1f;
					}
					else if (o->SubType == 2)
					{
						o->Light[0] -= 0.01f;
						o->Light[1] -= 0.01f;
						o->Light[2] -= 0.01f;
					}
					else if (o->SubType == 3 || o->SubType == 4 || o->SubType == 5)
					{
						MUModel* pModel = &MODELS::Data[o->Target->Type];
						EVector3 vRelativePos, vPos;
						Vector(0.0f, 0.0f, 0.0f, vRelativePos);

						if (o->SubType == 3)
						{
							pModel->TransformPosition(ThreadIndex, o->Target->BoneTransform[pModel->GetBoneIndex(37)], vRelativePos, vPos, true);
							VectorCopy(vPos, o->Position);
						}
						else if (o->SubType == 4)
						{
							pModel->TransformPosition(ThreadIndex, o->Target->BoneTransform[pModel->GetBoneIndex(28)], vRelativePos, vPos, true);
							VectorCopy(vPos, o->Position);
						}
						else if (o->SubType == 5)
						{
							VectorCopy(o->Target->Position, o->Position);
							o->Position[2] += 80.0f;
						}

						o->Light[0] /= 1.01f;
						o->Light[1] /= 1.01f;
						o->Light[2] /= 1.01f;
					}
				}
				break;

			case BITMAP_MAGIC:
				if (o->SubType == 0)
				{
					o->Scale -= 0.05f;

					o->Light[0] -= 0.01f;
					o->Light[1] -= 0.01f;
					o->Light[2] -= 0.01f;
				}
				break;

			case BITMAP_FLARE:
				if (o->SubType == 0 || o->SubType == 3 || o->SubType == 6
					|| o->SubType == 10
					)
				{
					mu_float count = (o->Velocity[0] + o->LifeTime)*0.1f;
					if (o->SubType == 10)
					{
						o->Position[0] = o->StartPosition[0];
						o->Position[1] = o->StartPosition[1];
					}
					else
					{
						o->Position[0] = o->StartPosition[0] + EMath::Sin(count)*40.0f;
						o->Position[1] = o->StartPosition[1] - EMath::Cos(count)*40.0f;
					}
					o->Position[2] += o->Gravity;

					o->Scale -= 0.002f;
				}
				else if (o->SubType == 2)
				{
					o->Position[0] = o->StartPosition[0];
					o->Position[1] = o->StartPosition[1];
					o->Position[2] += o->Gravity*((60 - o->LifeTime) / 10);
					o->Scale -= 0.002f;
				}
				else if (o->SubType == 5)
				{
					mu_float count = (o->Velocity[0] + o->LifeTime)*0.1f;
					o->Position[0] = o->StartPosition[0] + EMath::Sin(count)*40.0f;
					o->Position[1] = o->StartPosition[1] - EMath::Cos(count)*40.0f;
					o->Position[2] -= o->Gravity;
					o->Scale -= 0.002f;

					o->StartPosition[0] += 2.5f;
				}

				if (o->SubType == 4)
				{
					mu_float count = (o->Velocity[0] + o->LifeTime)*0.1f;
					o->Position[0] = o->StartPosition[0] + EMath::Sin(count)*40.0f;
					o->Position[1] = o->StartPosition[1] - EMath::Cos(count)*40.0f;
					o->Position[2] += o->Gravity;
					o->Scale -= 0.004f;

					if (o->LifeTime <= 30)
					{
						o->Light[0] /= 1.1f;
						o->Light[1] /= 1.1f;
						o->Light[2] /= 1.1f;
					}
				}
				else if (o->LifeTime <= 20)
				{
					o->Light[0] /= 1.1f;
					o->Light[1] /= 1.1f;
					o->Light[2] /= 1.1f;
				}
				else if (o->SubType == 11)
				{
					o->Light[0] /= 1.1f;
					o->Light[1] /= 1.1f;
					o->Light[2] /= 1.1f;
					o->Scale += 1.5f;
				}
				else if (o->SubType == 12)
				{
					mu_float count = (o->Velocity[0] + o->LifeTime)*0.1f;

					{
						o->Position[0] = o->StartPosition[0] + EMath::Sin(count)*110.0f;
						o->Position[1] = o->StartPosition[1] - EMath::Cos(count)*110.0f;
					}
					o->Position[2] += (o->Gravity + 0.1f);

					o->Scale -= 0.002f;
					if (o->LifeTime <= 35)
					{
						o->Light[0] /= 1.1f;
						o->Light[1] /= 1.1f;
						o->Light[2] /= 1.1f;
					}
				}

				if (o->SubType == 0)
				{
					if ((o->Target->CurrentAction >= PLAYER_WALK_MALE && o->Target->CurrentAction <= PLAYER_RUN_RIDE_WEAPON) ||
						(o->Target->CurrentAction >= PLAYER_ATTACK_SKILL_SWORD1 && o->Target->CurrentAction <= PLAYER_ATTACK_SKILL_SWORD5))
					{
						o->SubType = 1;
						o->LifeTime = EMath::Min(20, o->LifeTime);
						Vector(0.0f, 0.0f, 0.0f, o->Velocity);
						VectorCopy(o->Position, o->StartPosition);
					}
				}
				break;
			case BITMAP_FLARE_RED:
				o->Scale = EMath::Sin(o->LifeTime*10.0f*(Q_PI / 180.0f))*3.0f;
				HandPosition(ThreadIndex, o);
				break;
			case BITMAP_CLUD64:
				{
					if (o->SubType == 0)
					{
						Luminosity = (mu_float)(o->LifeTime) / 10.0f;
						Vector(Luminosity, Luminosity, Luminosity, o->Light);
						o->Gravity -= 0.01f;
						o->Position[2] -= o->Gravity;
						o->Scale -= 0.03f;
						o->Alpha -= 0.05f;
					}
					else if (o->SubType == 1 || o->SubType == 2)
					{
						o->Scale += 0.5f;

						o->Light[0] /= 1.1f;
						o->Light[1] /= 1.1f;
						o->Light[2] /= 1.1f;

						if (o->Light[0] <= 0.05f)
							oh->Flags.Clear(PARTICLE::ePF_LIVE);
					}
					else if (o->SubType == 3)
					{
						o->Scale += 0.08f;

						o->Light[0] /= 1.1f;
						o->Light[1] /= 1.1f;
						o->Light[2] /= 1.1f;

						if (o->Light[0] <= 0.05f)
							oh->Flags.Clear(PARTICLE::ePF_LIVE);
					}
					else if (o->SubType == 5)
					{
						o->Scale += 0.08f;

						o->Light[0] /= 1.1f;
						o->Light[1] /= 1.1f;
						o->Light[2] /= 1.1f;

						if (o->Light[0] <= 0.05f)
							oh->Flags.Clear(PARTICLE::ePF_LIVE);
					}
					else if (o->SubType == 6)
					{
						if (o->LifeTime < 10)
						{
							o->Alpha -= 0.2f;
						}
						else if (o->Alpha < 1.0f)
						{
							o->Alpha += (GetLargeRand(2) + 3)*0.1f;
						}
						else
						{
							o->Alpha = 1.0f;
						}

						if (o->Alpha < 0.1f)
							oh->Flags.Clear(PARTICLE::ePF_LIVE);
						Luminosity = o->Alpha;
						Vector(o->TurningForce[0] * Luminosity, o->TurningForce[1] * Luminosity, o->TurningForce[2] * Luminosity, o->Light);
						if (o->Scale > 0)
						{
							o->Scale -= (GetLargeRand(5) + 55)*0.001f;
						}
						else if (o->Scale < 0.1f)
						{
							oh->Flags.Clear(PARTICLE::ePF_LIVE);
						}
						o->Position[2] += o->Gravity;
						VectorSubtract(o->Position, o->StartPosition, o->Position);
						VectorCopy(o->Target->Position, o->StartPosition);
						VectorAdd(o->Position, o->StartPosition, o->Position);
					}
					else if (o->SubType == 7)
					{
						if (o->LifeTime < 10)
						{
							o->Alpha -= 0.1f;
							VectorScale(o->Light, 0.9f, o->Light);
						}
						else if (o->Alpha < 1.0f)
						{
							o->Alpha += (mu_float)(GetLargeRand(20))*0.005f;
						}

						VectorAdd(o->Position, o->Velocity, o->Position);
						o->Scale += ((mu_float)(GetLargeRand(10))*0.01f);
					}
					else if (o->SubType == 8)
					{
						if (o->LifeTime < 10)
						{
							o->Alpha -= 0.1f;
							VectorScale(o->Light, 0.9f, o->Light);
						}
						else if (o->Alpha < 1.0f)
						{
							o->Alpha += (mu_float)(GetLargeRand(20))*0.005f;
						}

						VectorAdd(o->Position, o->Velocity, o->Position);
						o->Scale += ((mu_float)(GetLargeRand(10))*0.01f);
					}
					else if (o->SubType == 9)
					{
						o->Scale *= 0.98f + ((mu_float)(GetLargeRand(20) - 10)*0.002f);

						o->Light[0] *= 0.95f + ((mu_float)(GetLargeRand(20) - 10)*0.002f);
						o->Light[1] *= 0.95f + ((mu_float)(GetLargeRand(20) - 10)*0.002f);
						o->Light[2] *= 0.95f + ((mu_float)(GetLargeRand(20) - 10)*0.002f);
						o->Alpha *= 0.95f + ((mu_float)(GetLargeRand(20) - 10)*0.002f);
					}
					else if (o->SubType == 10)
					{
						o->Scale -= (GetLargeRand(5) + 15)*0.0016f;
						o->Position[2] += o->Gravity*10.0f;

						if (o->Scale < 0 || !o->Target->Flags.Get(OBJECT::eOF_LIVE))
							oh->Flags.Clear(PARTICLE::ePF_LIVE);
						VectorSubtract(o->Position, o->StartPosition, o->Position);
						VectorCopy(o->Target->Position, o->StartPosition);
						VectorAdd(o->Position, o->StartPosition, o->Position);
					}
					if (o->SubType == 11)
					{
						Luminosity = (mu_float)(o->LifeTime) / 10.0f;
						Vector(Luminosity, Luminosity, Luminosity, o->Light);
						o->Gravity -= 0.01f;
						o->Position[2] -= o->Gravity;
						o->Scale -= 0.03f;
						o->Alpha -= 0.05f;
					}
				}
				break;

				//////////////////////////////////////////////////////////////////////////

			case BITMAP_LIGHT + 3:
				{
					if (o->SubType == 0)
					{
						mu_float _Angle = (o->Velocity[0] + o->LifeTime)*0.1f;
						o->Position[0] = o->StartPosition[0] + EMath::Sin(_Angle)*35.0f;
						o->Position[1] = o->StartPosition[1] - EMath::Cos(_Angle)*35.0f;
						o->Position[2] += o->Gravity*0.1f;
						o->Scale -= 0.001f;
					}
					else if (o->SubType == 1)
					{
						o->Scale = EMath::Sin(o->LifeTime*2.0f*(Q_PI / 180.0f));
					}
				}
				break;

			case BITMAP_TWINTAIL_WATER:
				{
					if (o->SubType == 0 || o->SubType == 1)
					{
						o->Scale -= 0.013f;
					}
					else if (o->SubType == 2)
					{
						o->Scale -= 0.026f;
					}

					o->Position[2] += o->Gravity;
					o->Alpha -= 0.05f;
					o->Light[0] /= 1.02f;
					o->Light[1] /= 1.02f;
					o->Light[2] /= 1.02f;
				}
				break;
			case BITMAP_SMOKE:
				switch (o->SubType)
				{
				case 0:
					Luminosity = (mu_float)(o->LifeTime) / 8.0f;
					Vector(Luminosity, Luminosity, Luminosity, o->Light);
					o->Gravity += 0.2f;
					o->Position[2] += o->Gravity;
					o->Scale += 0.05f;
					break;
				case 36:
					if (o->LifeTime < 23)
					{
						o->Gravity += (0.1f*o->Scale);
						o->Position[2] += o->Gravity;
						o->Position[1] += o->Gravity;
						o->Scale -= 0.08f;
					}
					else
						VectorAdd(o->Target->Position, o->StartPosition, o->Position);
					o->Rotation += 0.01f;
					break;
				case 37:
					o->Position[2] -= o->TurningForce[2] * 0.8f;
					o->Position[1] -= o->TurningForce[1] * 0.8f;
					o->Scale += 0.08f;
					o->Rotation += 0.01f;
					o->Light[0] /= 1.03f;
					o->Light[1] /= 1.03f;
					o->Light[2] /= 1.03f;
					break;
				case 38:
					o->Scale += 0.09f;
					o->Light[0] /= 1.04f;
					o->Light[1] /= 1.04f;
					o->Light[2] /= 1.04f;
					break;
				case 33:
					Luminosity = (mu_float)(o->LifeTime) / 8.0f;
					Vector(Luminosity*0.4f, Luminosity*0.4f, Luminosity, o->Light);
					VectorScale(o->Velocity, 0.001f, o->Velocity);
					o->Scale += (0.01f * 2);
					o->Position[2] += o->Scale;
					break;
				case 32:
					Luminosity = (mu_float)(o->LifeTime) / 8.0f;
					Vector(Luminosity*0.8f, Luminosity*0.8f, Luminosity, o->Light);
					VectorScale(o->Velocity, 0.4f, o->Velocity);
					o->Scale += 0.1f;
					break;
				case 23:	//. 일반(녹색)
					Luminosity = (mu_float)(o->LifeTime) / 8.0f;
					Vector(Luminosity*0.1f, Luminosity, Luminosity*0.6f, o->Light);
					o->Gravity += 0.2f;
					o->Position[2] += o->Gravity;
					o->Scale += 0.05f;
					break;
				case 3:
					Luminosity = (mu_float)(o->LifeTime) / 8.0f;
					Vector(Luminosity*0.8f, Luminosity*0.8f, Luminosity, o->Light);
					VectorScale(o->Velocity, 0.4f, o->Velocity);
					o->Scale += 0.1f;
					break;
				case 11:
				case 14:
					Luminosity = (mu_float)(o->LifeTime) / 50.0f;
					Vector(Luminosity*o->TurningForce[0], Luminosity*o->TurningForce[1], Luminosity*o->TurningForce[2], o->Light);
					VectorScale(o->Velocity, 0.4f, o->Velocity);
					o->Scale += 0.05f;
					o->Position[2] -= 1.0f;
					break;
				case 17:
					Luminosity = (mu_float)(o->LifeTime) / 8.0f;
					Luminosity = (Luminosity > 1.0f ? 1.0f - Luminosity : Luminosity);
					Vector(Luminosity*o->TurningForce[0], Luminosity*o->TurningForce[1], Luminosity*o->TurningForce[2], o->Light);
					o->Gravity += 0.2f;
					o->Position[2] += o->Gravity;
					o->Scale += 0.05f;
					break;
				case 15:
					Luminosity = (mu_float)(o->LifeTime) / 40.0f;
					Vector(Luminosity*0.5f, Luminosity*0.5f, Luminosity*0.5f, o->Light);
					o->Gravity += (GetLargeRand(20)) / 500.0f;
					o->Position[0] += (GetLargeRand(100) - 20) / 100.0f;
					o->Position[1] += (GetLargeRand(100) - 20) / 100.0f;
					o->Position[2] += o->Gravity;
					o->Scale += (GetLargeRand(10)) / 1000.0f;
					o->Rotation = o->Scale;
					break;
				case 1:
					Luminosity = (mu_float)(o->LifeTime) / 50.0f;
					Vector(Luminosity*0.5f, Luminosity*1.0f, Luminosity*0.8f, o->Light);
					VectorScale(o->Velocity, 0.4f, o->Velocity);
					o->Scale += 0.05f;
					break;
				case 2:
					Luminosity = (mu_float)(o->LifeTime) / 50.0f;
					Vector(Luminosity, Luminosity, Luminosity, o->Light);
					o->Gravity -= 0.1f;
					o->Position[0] -= o->Gravity*0.2f;
					o->Position[2] += o->Gravity;
					o->Scale -= 0.01f;
					break;
				case 16:
					Luminosity = (mu_float)(o->LifeTime) / 50.0f;
					Vector(Luminosity, Luminosity, Luminosity, o->Light);
					o->Gravity -= 0.1f;
					o->Position[0] -= o->Gravity*0.2f;
					o->Position[2] += o->Gravity;
					o->Scale -= 0.01f;
					break;
				case 12:
					Luminosity = (mu_float)(o->LifeTime) / 40.0f;
					Vector(Luminosity, Luminosity, Luminosity, o->Light);
					o->Scale += 0.05f;
					break;
				case 13:
					Luminosity = (mu_float)(o->LifeTime) / 20.0f;
					Vector(Luminosity*1.0f, Luminosity*0.5f, Luminosity*0.1f, o->Light);
					o->Scale += 0.09f;
					break;
				case 18:
					Luminosity = (mu_float)(o->LifeTime) / 20.0f;
					Vector(Luminosity*1.0f, Luminosity*1.0f, Luminosity*1.0f, o->Light);
					o->Scale += 0.09f;
					break;
				case 4:
					Luminosity = (mu_float)(o->LifeTime) / 8.0f;
					Vector(Luminosity*120.0f / 255.0f, Luminosity*100.7f / 255.0f, Luminosity*80.0f / 255.0f, o->Light);
					o->Gravity += 0.2f;
					o->Position[2] += o->Gravity;
					o->Scale += 0.05f;
					break;
				case 5:
					Luminosity = (mu_float)(o->LifeTime) / 10.0f;
					Vector(Luminosity, Luminosity, Luminosity, o->Light);
					o->Gravity -= 0.1f;
					o->Position[0] -= o->Gravity*(0.2f*(GetLargeRand(2) + 1));
					o->Position[2] += o->Gravity;
					o->Scale -= 0.01f;
					break;
				case 6:
					Luminosity = 0.6f;
					o->LifeTime = 10;
					o->Position[2] = o->Rotation + EMath::Sin((FWorldTime + o->Gravity) / 5000.0f)*20.0f;
					o->Scale = EMath::Sin(((mu_int32)(o->Gravity + FWorldTime) % 1800 * 0.1f*(Q_PI / 180.0f)))*0.5f + 1.8f;

					Vector(Luminosity*0.6f, Luminosity*0.5f, Luminosity*0.4f, o->Light);
					break;
				case 7:
					Luminosity = 1.0f;

					o->Scale += 0.03f;
					o->Gravity += 1.0f;
					o->Velocity[1] -= 0.1f;
					o->Position[2] -= o->Gravity;
					if (o->LifeTime < 5)
					{
						Luminosity = (mu_float)(o->LifeTime) / 8.0f;
						o->Scale -= 0.1f;
					}
					Vector(Luminosity*0.725f, Luminosity*0.572f, Luminosity*0.333f, o->Light);
					break;
				case 8: //  나태.
					o->Gravity += 0.02f;
					if (o->LifeTime > 5)
					{
						o->Scale += o->Gravity;
						o->Position[2] += o->Gravity*20.0f;

						VectorScale(o->Velocity, 1.05f, o->Velocity);
						Luminosity = (mu_float)(o->LifeTime) / 24.0f;
						Vector(Luminosity, Luminosity, Luminosity, o->Light);
						VectorScale(o->Velocity, 0.4f, o->Velocity);
					}
					else
					{
						Vector(o->Light[0] * 0.5f, o->Light[1] * 0.5f, o->Light[2] * 0.5f, o->Light);
						Vector(0.0f, 0.0f, 0.0f, o->Velocity);
					}
					break;
				case 9:
					Luminosity = (mu_float)(o->LifeTime) / 10.0f;
					Vector(Luminosity*250.0f / 255.0f, Luminosity*156.7f / 255.0f, 0.0f, o->Light);
					o->Gravity += 0.5f;
					o->Position[2] += o->Gravity;
					o->Scale += 0.1f;
					break;
				case 10:    //  블러드캐슬의 연기
					Luminosity = (mu_float)(o->LifeTime) / 16.0f;
					Vector(Luminosity*1.0f, Luminosity*0.1f, Luminosity*0.1f, o->Light);
					o->Scale += 0.05f;
					break;
				case 19:
					Luminosity = (mu_float)(o->LifeTime) / 40.0f;
					Vector(Luminosity*0.5f, Luminosity*0.5f, Luminosity*0.5f, o->Light);
					o->Gravity += (GetLargeRand(20)) / 500.0f;
					o->Position[0] += (GetLargeRand(100) - 20) / 100.0f;
					o->Position[1] += (GetLargeRand(100) - 20) / 100.0f;
					o->Position[2] += o->Gravity;
					o->Scale += (GetLargeRand(10)) / 1000.0f;
					o->Rotation = o->Scale;
					break;
				case 20:
					Luminosity = (mu_float)(o->LifeTime) / 40.0f;
					Vector(Luminosity, Luminosity, Luminosity, o->Light);
					o->Gravity += (GetLargeRand(20)) / 500.0f;
					o->Position[0] += (GetLargeRand(100) - 20) / 100.0f;
					o->Position[1] += (GetLargeRand(100) - 20) / 100.0f;
					o->Position[2] += o->Gravity;
					o->Scale += (GetLargeRand(10)) / 1000.0f;
					o->Rotation = o->Scale;
					break;
				case 21:	//. 검은연기 박스 Fixed : 2004/11/05
					Luminosity = (mu_float)(o->LifeTime) / 50.0f;
					Vector(Luminosity, Luminosity, Luminosity, o->Light);
					o->Gravity -= 0.1f;
					o->Position[0] -= o->Gravity*0.2f;
					o->Position[2] += o->Gravity;
					o->Scale -= 0.01f;
					break;
				case 22:	//. 붉은연기 박스 : 2004/11/26
					Luminosity = (mu_float)(o->LifeTime) / 50.0f;
					Vector(Luminosity*0.9f, Luminosity*0.5f, Luminosity*0.5f, o->Light);
					o->Gravity -= 0.1f;
					o->Position[0] += o->Gravity*EMath::Sin(FWorldTime)*0.05f;
					o->Position[2] += o->Gravity;
					o->Scale += 0.04f;
					break;
				case 24:	//. 일반(녹색, 스케일조정가능)
					Luminosity = (mu_float)(o->LifeTime) / 32.0f;
					Vector(Luminosity*0.2f, Luminosity*0.5f, Luminosity*0.35f, o->Light);
					o->Gravity += 0.1f;
					o->Position[2] += o->Gravity;
					o->Scale += 0.05f;
					break;
				case 25:
					Luminosity = (mu_float)(o->LifeTime) / 20.0f;
					Vector(Luminosity*1.0f, Luminosity*1.0f, Luminosity*1.0f, o->Light);
					o->Scale += 0.09f;
					break;
				case 35:
					o->Light[0] /= 1.04f;
					o->Light[1] /= 1.04f;
					o->Light[2] /= 1.04f;
					o->Scale += 0.06f;
					break;
				case 34:
					o->Light[0] /= 1.04f;
					o->Light[1] /= 1.04f;
					o->Light[2] /= 1.04f;
					o->Scale += 0.06f;
					break;
				case 26:
					Luminosity = (mu_float)(o->LifeTime) / 20.0f;
					Vector(o->Light[0] * Luminosity*1.0f, o->Light[1] * Luminosity*1.0f, o->Light[2] * Luminosity*1.0f, o->Light);
					o->Scale += 0.15f;
					break;
				case 27:
					Vector(o->Light[0] * 0.92f, o->Light[1] * 0.92f, o->Light[2] * 0.92f, o->Light);
					o->Scale -= 0.05f;
					break;
				case 28:
					o->Position[2] += o->Gravity;
					if (o->LifeTime >= 9) o->Scale -= 0.4f;
					else o->Scale *= 1.2f;
					Vector(o->Light[0] * 0.92f, o->Light[1] * 0.92f, o->Light[2] * 0.92f, o->Light);
					break;
				case 29:
					o->Position[2] += o->Gravity;
					if (o->LifeTime >= 9) o->Scale -= 0.4f;
					else o->Scale *= 1.2f;
					Vector(1.0f, 1.0f, 1.0f, o->Light);
					break;
				case 30:
					o->Scale += 0.15f;
					o->Position[2] += o->Gravity;
					Vector(1.0f, 3.0f, 1.0f, o->Light);
					break;
				case 31:
					o->Scale -= 0.05f;
					o->Light[0] /= 1.1f;
					o->Light[1] /= 1.1f;
					o->Light[2] /= 1.1f;
					if (o->Scale <= 0.0f && o->Light[2] <= 0.0f)
						oh->Flags.Clear(PARTICLE::ePF_LIVE);
					break;
				case 40:
					{
						Luminosity = (mu_float)(o->LifeTime) / 50.0f;
						Vector(Luminosity*0.5f, Luminosity*0.5f, Luminosity*1.0f, o->Light);
						VectorScale(o->Velocity, 0.4f, o->Velocity);
						o->Scale += 0.05f;
					}
					break;
				case 41:
					{
						Luminosity = (mu_float)(o->LifeTime) / 50.0f;
						Vector(Luminosity*0.5f, Luminosity*1.0f, Luminosity*0.8f, o->Light);
						VectorScale(o->Velocity, 0.4f, o->Velocity);
						o->Scale += 0.05f;
						o->Gravity += 0.2f;
						o->Position[2] += o->Gravity;
					}
					break;
				case 42:	// case 4:와 동일하지만 o->Light 세팅 부분만 틀림(버서커 형광색).
					Luminosity = (mu_float)(o->LifeTime) / 8.0f;
					Vector(Luminosity*127.0f / 255.0f, Luminosity*255.0f / 255.0f,
						   Luminosity*200.0f / 255.0f, o->Light);
					o->Gravity += 0.2f;
					o->Position[2] += o->Gravity;
					o->Scale += 0.05f;
					break;
				case 43:
					{
						o->Light[0] /= 1.02f;
						o->Light[1] /= 1.02f;
						o->Light[2] /= 1.02f;
						o->Scale += 0.08f;
					}
					break;
				case 44:
					{
						if (o->LifeTime >= 30)
						{
							o->Light[0] *= 1.07f;
							o->Light[1] *= 1.07f;
							o->Light[2] *= 1.07f;
						}
						else
						{
							o->Light[0] /= 1.07f;
							o->Light[1] /= 1.07f;
							o->Light[2] /= 1.07f;
						}
						o->Scale += 0.02f;
						o->Position[2] += 0.8f;
						o->Rotation += o->Gravity / 50.0f;
					}
					break;
				case 45:
					{
						o->Light[0] /= 1.1f;
						o->Light[1] /= 1.1f;
						o->Light[2] /= 1.1f;
					}
					break;
				case 46:
					{
						o->Light[0] /= 1.02f;
						o->Light[1] /= 1.02f;
						o->Light[2] /= 1.02f;

						if (o->Light[0] <= 0.05f)
							o->LifeTime = 0;

						o->Gravity -= 0.05f;
						o->Position[2] += (o->Scale + o->Gravity)*1.5f;
						o->Scale += o->Scale / 100.0f;
					}
					break;
				case 47:
					{
						o->Light[0] /= 1.02f;
						o->Light[1] /= 1.02f;
						o->Light[2] /= 1.02f;

						if (o->Light[0] <= 0.05f)
							o->LifeTime = 0;

						o->Scale += 1.0f;
					}
					break;
				case 59:
					Luminosity = (mu_float)(o->LifeTime) / 40.0f;
					Vector(Luminosity*0.9f, Luminosity*0.9f, Luminosity*0.9f, o->Light);
					o->Scale += 0.09f;
					break;
				case 48:
					Luminosity = (mu_float)(o->LifeTime) / 40.0f;
					Vector(Luminosity*0.4f, Luminosity*0.4f, Luminosity*0.4f, o->Light);
					o->Scale += 0.09f;
					break;
				case 49:
					{
						if (o->LifeTime < 100)
						{
							o->Light[0] *= 0.97f;
							o->Light[1] *= 0.97f;
							o->Light[2] *= 0.97f;
						}
						else
						{
							o->Light[0] *= 1.03f;
							o->Light[1] *= 1.03f;
							o->Light[2] *= 1.03f;
						}

						if (o->Light[0] <= 0.01f)
							o->LifeTime = 0;

						VectorScale(o->Velocity, 0.001f, o->Velocity);
						o->Scale += (GetLargeRand(15) + 4)*(0.001f);
						o->Position[2] += o->Scale*6.0f;
						o->Rotation += (GetLargeRand(10) + 10)*0.1f*o->Angle[0];
					}
					break;
				case 50:
					if (o->LifeTime < 10)
					{
						o->Alpha -= 0.1f;
					}
					else if (o->Alpha < 0.5f)
					{
						o->Alpha += (GetLargeRand(3) + 2)*0.1f;
					}
					else
					{
						o->Alpha = 0.5f;
					}
					if (o->Alpha < 0.1f)
					{
						oh->Flags.Clear(PARTICLE::ePF_LIVE);
					}
					Luminosity = o->Alpha;
					Vector(o->TurningForce[0] * Luminosity, o->TurningForce[1] * Luminosity, o->TurningForce[2] * Luminosity, o->Light);
					o->Scale += 0.02f;
					o->Position[2] += o->Gravity;
					break;
				case 51:
					Luminosity = (mu_float)(o->LifeTime) / 20.0f;
					VectorScale(o->TurningForce, Luminosity, o->Light);
					o->Scale += 0.09f;
					break;
				case 52:
					{
						VectorAdd(o->Position, o->Velocity, o->Position);
						o->Scale += 0.06f;
						o->Light[0] /= 1.07f;
						o->Light[1] /= 1.07f;
						o->Light[2] /= 1.07f;
					}
					break;
				case 53:
					Luminosity = (mu_float)(o->LifeTime) / 50.0f;
					Vector(Luminosity*1.0f, Luminosity*1.0f, Luminosity*1.0f, o->Light);
					VectorScale(o->Velocity, 0.4f, o->Velocity);
					o->Scale += 0.05f;
					break;
				case 54:
					{
						o->Light[0] /= 1.02f;
						o->Light[1] /= 1.02f;
						o->Light[2] /= 1.02f;

						if (o->Light[0] <= 0.05f)
							o->LifeTime = 0;

						o->Gravity -= 0.05f;
						o->Position[2] += (o->Scale + o->Gravity)*1.5f;
						o->Scale += o->Scale / 100.0f;
					}
					break;
				case 55:
					{
						o->Gravity += 0.1f;
						o->Position[2] += o->Gravity;
						o->Scale += 0.05f;

						o->Light[0] /= 1.08f;
						o->Light[1] /= 1.08f;
						o->Light[2] /= 1.08f;
					}
					break;
				case 56:	// case 1:과 동일하나 색상만 다름.
					Luminosity = (mu_float)(o->LifeTime) / 50.0f;
					Vector(Luminosity*0.5f, Luminosity*0.1f, Luminosity*0.8f, o->Light);
					VectorScale(o->Velocity, 0.4f, o->Velocity);
					o->Scale += 0.05f;
					break;
				case 57:	//case 24:와 동일하나 색상만 다름.
					Luminosity = (mu_float)(o->LifeTime) / 32.0f;
					Vector(Luminosity*0.5f, Luminosity*0.1f, Luminosity*0.8f, o->Light);
					o->Gravity += 0.1f;
					o->Position[2] += o->Gravity;
					o->Scale += 0.05f;
					break;
				case 58:
					Luminosity = (mu_float)(o->LifeTime) / 50.0f;
					Vector(Luminosity*o->StartPosition[0], Luminosity*o->StartPosition[1],
						   Luminosity*o->StartPosition[2], o->Light);
					VectorScale(o->Velocity, 0.4f, o->Velocity);
					o->Scale += 0.05f;
					break;
				case 60:
					{
						Luminosity = (mu_float)(o->LifeTime) / 50.0f;
						Vector(Luminosity*0.4f, Luminosity*0.4f, Luminosity*0.4f, o->Light);
						o->Gravity -= 0.1f;
						o->Position[0] += o->Gravity*EMath::Sin(FWorldTime)*0.05f;
						o->Position[2] += o->Gravity;
						o->Scale += 0.04f;
					}
					break;
				case 61:
					{
						Luminosity = (mu_float)(o->LifeTime) / 8.0f;
						o->Gravity += 0.2f;
						o->Position[2] += o->Gravity;
						o->Scale += 0.05f;
					}
					break;
				case 62:
					{
						Luminosity = (mu_float)(o->LifeTime) / 50.0f;
						//Vector(Luminosity*0.9f,Luminosity*0.4f,Luminosity*0.1f,o->Light);
						Vector(Luminosity*0.9f, Luminosity*0.9f, Luminosity*0.9f, o->Light);
						VectorScale(o->Velocity, 0.001f, o->Velocity);
						o->Scale += 0.03f;
					}
					break;
				case 63:
					{
						o->Light[0] /= 1.1f;
						o->Light[1] /= 1.1f;
						o->Light[2] /= 1.1f;
						o->Scale += 0.5f;
						o->Velocity[1] += 1.5f;
					}
					break;
				case 64:
					{
						Luminosity = (mu_float)(o->LifeTime) / 24.0f;
						Vector(Luminosity*0.1f, Luminosity*0.7f, Luminosity*0.4f, o->Light);

						MovePosition(o->Position, o->Angle, o->Velocity);

						o->Rotation += 0.05f;

						o->Gravity -= 0.1f;
						o->Position[0] += o->Gravity*EMath::Sin(FWorldTime)*0.05f;
						o->Position[2] += o->Gravity;

						o->Scale += (mu_float)(GetLargeRand(15) + 15)*0.01f;

						if (16 > o->LifeTime)
						{
							o->Alpha -= 0.1f;
						}
					}
					break;
				case 65:
					{
						Luminosity = (mu_float)(o->LifeTime) / 40.0f;
						Vector(Luminosity*0.4f, Luminosity*0.4f, Luminosity*0.4f, o->Light);
						o->Gravity -= 0.05f;
						o->Position[0] += o->Gravity*EMath::Sin(FWorldTime)*0.05f;
						o->Position[2] += o->Gravity;
					}
					break;
				case 66:
					{
						o->Light[0] /= 1.02f;
						o->Light[1] /= 1.02f;
						o->Light[2] /= 1.02f;

						if (o->Light[0] <= 0.05f)
							o->LifeTime = 0;

						o->Gravity -= 0.05f;
						o->Position[0] += o->TurningForce[0] * (o->Scale + o->Gravity)*1.0f;
						o->Position[1] += o->TurningForce[1] * (o->Scale + o->Gravity)*1.0f;
						o->Scale += o->Scale / 100.0f;
					}
					break;
				case 67:
					{
						o->Alpha -= 0.01f;

						if (o->Alpha < 0.1f)
						{
							oh->Flags.Clear(PARTICLE::ePF_LIVE);
						}
						o->Light[0] *= o->Alpha;
						o->Light[1] *= o->Alpha;
						o->Light[2] *= o->Alpha;

						o->Scale += 0.05f;
						o->Rotation += 0.01f;
					}
					break;
				case 68:		// 방향성 연기 Angle 값 넣으면 조정.
					{
						Luminosity = (mu_float)(o->LifeTime) / 40.0f;
						Vector(Luminosity*0.4f, Luminosity*0.4f, Luminosity*0.4f, o->Light);
						o->Velocity[0] += (0.03f * o->Velocity[0]);
						o->Velocity[1] += (0.03f * o->Velocity[1]);
						o->Velocity[2] += (0.03f * o->Velocity[2]);

						MovePosition(o->Position, o->Angle, o->Velocity);

						if (30 > o->LifeTime)
						{
							o->Alpha -= 0.02f;
						}
					}
					break;
				}
				break;
			case BITMAP_SMOKE + 2:
				Luminosity = 1.0f;
				if (o->LifeTime < 10) Luminosity -= (mu_float)(10 - o->LifeTime)*0.1f;
				Vector(Luminosity, Luminosity, Luminosity, o->Light);
				AngleMatrix(o->Angle, Matrix);
				VectorRotate(o->StartPosition, Matrix, Position);
				VectorCopy(o->Target->Position, TargetPosition);
				VectorAdd(TargetPosition, Position, o->Position);
				o->Angle[1] += 5.0f;
				o->Scale -= 0.01f;
				break;
			case BITMAP_SMOKE + 3:
				switch (o->SubType)
				{
				case 0:
					if (o->TurningForce[0] > 0.1f) o->TurningForce[0] -= 0.015f;
					if (o->TurningForce[1] > 0.1f) o->TurningForce[0] -= 0.005f;
					//o->TurningForce[1] -= 0.001f;
					Luminosity = (mu_float)(o->LifeTime) / 55.0f;
					Vector(o->TurningForce[0] * Luminosity, o->TurningForce[1] * Luminosity, o->TurningForce[2] * Luminosity, o->Light);
					o->Gravity += 0.2f;
					o->Position[2] += o->Gravity;
					o->Scale += 0.03f;
					o->Rotation += o->Angle[0];
					break;
				case 1:
					Luminosity = (mu_float)(o->LifeTime) / 55.0f;
					Vector(o->TurningForce[0] * Luminosity, o->TurningForce[1] * Luminosity, o->TurningForce[2] * Luminosity, o->Light);
					o->Gravity += 0.1f;
					o->Position[2] += o->Gravity;
					o->Scale += 0.03f;
					o->Rotation += o->Angle[0];
					break;
				case 2:
					if (o->TurningForce[1] > 0.1f) o->TurningForce[1] -= 0.005f;
					if (o->TurningForce[2] > 0.1f) o->TurningForce[2] -= 0.015f;
					Luminosity = (mu_float)(o->LifeTime) / 55.0f;
					Vector(o->TurningForce[0] * Luminosity, o->TurningForce[1] * Luminosity, o->TurningForce[2] * Luminosity, o->Light);
					o->Gravity += 0.2f;
					o->Position[2] += o->Gravity;
					o->Scale += 0.03f;
					o->Rotation += o->Angle[0];
					break;
				case 3:
					if (o->Light[0] <= 0.05f)
						oh->Flags.Clear(PARTICLE::ePF_LIVE);
					o->Light[0] /= 1.012f;
					o->Light[1] /= 1.012f;
					o->Light[2] /= 1.012f;
					o->Scale += 0.01f;
					o->Rotation += o->Gravity * 0.5f;

					if (o->Gravity <= 0.0f)
						o->Gravity = -o->Gravity;
					o->Position[2] += o->Gravity;
					break;
				case 4:
					if (o->Light[0] <= 0.05f)
						oh->Flags.Clear(PARTICLE::ePF_LIVE);
					o->Light[0] /= 1.012f;
					o->Light[1] /= 1.012f;
					o->Light[2] /= 1.012f;
					o->Scale += 0.01f;
					o->Rotation += o->Gravity * 0.5f;

					if (o->Gravity <= 0.0f)
						o->Gravity = -o->Gravity;
					o->Position[2] += o->Gravity;
					break;
				}
				break;
			case BITMAP_SMOKELINE1:
			case BITMAP_SMOKELINE2:
			case BITMAP_SMOKELINE3:
				if (o->SubType == 0)
				{
					if (o->LifeTime < 10)
					{
						o->Alpha -= 0.1f;
					}
					else if (o->Alpha < 0.7f)
					{
						o->Alpha += (GetLargeRand(5) + 2)*0.1f;
					}
					else
					{
						o->Alpha = 0.7f;
					}

					if (o->Alpha < 0.1f)
					{
						oh->Flags.Clear(PARTICLE::ePF_LIVE);
					}
					Luminosity = o->Alpha;
					Vector(o->TurningForce[0] * Luminosity, o->TurningForce[1] * Luminosity, o->TurningForce[2] * Luminosity, o->Light);
					o->Scale += (GetLargeRand(3) + 6)*0.01f;//0.07f;
					o->Position[2] += o->Gravity;
				}
				else if (o->SubType == 1)	// ◎
				{
					if (o->LifeTime < 10)
					{
						o->Alpha -= 0.2f;
					}
					else if (o->Alpha < 1.0f)
					{
						o->Alpha += (GetLargeRand(5) + 2)*0.1f;
					}
					else
					{
						o->Alpha = 1.0f;
					}

					if (o->Alpha < 0.1f)
					{
						oh->Flags.Clear(PARTICLE::ePF_LIVE);
					}
					Luminosity = o->Alpha;
					Vector(o->TurningForce[0] * Luminosity, o->TurningForce[1] * Luminosity, o->TurningForce[2] * Luminosity, o->Light);
					if (o->Scale > 0)
					{
						o->Scale -= (GetLargeRand(10) + 10)*0.001f;//0.07f;
					}
					else
					{
						oh->Flags.Clear(PARTICLE::ePF_LIVE);
					}
					o->Position[2] += o->Gravity;
					VectorSubtract(o->Position, o->StartPosition, o->Position);
					VectorCopy(o->Target->Position, o->StartPosition);
					VectorAdd(o->Position, o->StartPosition, o->Position);
				}
				else if ((o->SubType == 2) || (o->SubType == 3))
				{
					if (o->LifeTime < 10)
					{
						o->Alpha -= 0.1f;
					}
					else if (o->Alpha < 0.7f)
					{
						o->Alpha += (GetLargeRand(5) + 2)*0.1f;
					}
					else
					{
						o->Alpha = 0.7f;
					}

					if (o->Alpha < 0.1f)
					{
						oh->Flags.Clear(PARTICLE::ePF_LIVE);
					}
					Luminosity = o->Alpha;
					Vector(o->TurningForce[0] * Luminosity, o->TurningForce[1] * Luminosity, o->TurningForce[2] * Luminosity, o->Light);
					o->Scale += (GetLargeRand(3) + 6)*0.01f;
					o->Position[2] += o->Gravity;
				}
				else if (o->SubType == 4)
				{
					o->Position[2] += o->Gravity;
					o->Rotation += 0.8f;
					o->Scale += 0.01f;

					o->Alpha -= 0.01f;

					if (o->Alpha < 0.1f)
					{
						oh->Flags.Clear(PARTICLE::ePF_LIVE);
					}
					o->Light[0] *= o->Alpha;
					o->Light[1] *= o->Alpha;
					o->Light[2] *= o->Alpha;
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
							o->Alpha -= 0.15f;

							if (o->Alpha < 0.1f)
							{
								oh->Flags.Clear(PARTICLE::ePF_LIVE);
							}
							Luminosity = o->Alpha;
							Vector(o->TurningForce[0] * Luminosity, o->TurningForce[1] * Luminosity, o->TurningForce[2] * Luminosity, o->Light);
						}
						break;
					}
				}
				break;
			case BITMAP_FIRE_HIK1:
			case BITMAP_FIRE_HIK1_MONO:
				if (o->SubType == 0)
				{
					if (o->LifeTime < 15)
					{
						o->Alpha -= 0.2f;
					}
					else if (o->Alpha < 1.0f)
					{
						o->Alpha += (GetLargeRand(2) + 2)*0.1f;
					}
					else
					{
						o->Alpha = 1.0f;
					}

					if (o->Alpha < 0.1f)
						oh->Flags.Clear(PARTICLE::ePF_LIVE);
					Luminosity = o->Alpha;
					Vector(o->TurningForce[0] * Luminosity, o->TurningForce[1] * Luminosity, o->TurningForce[2] * Luminosity, o->Light);
					if (o->Scale > 0)
					{
						o->Scale -= (GetLargeRand(3) + 5)*0.01f;
					}
					else
					{
						oh->Flags.Clear(PARTICLE::ePF_LIVE);
					}
					o->Position[2] += o->Gravity;
					o->Rotation += 3.0f;
				}
				else if (o->SubType == 1)
				{
					if (o->LifeTime < 20)
					{
						o->Alpha -= 0.2f;
					}
					else if (o->Alpha < 1.0f)
					{
						o->Alpha += (GetLargeRand(2) + 2)*0.1f;
					}
					else
					{
						o->Alpha = 1.0f;
					}

					if (o->Alpha < 0.1f)
						oh->Flags.Clear(PARTICLE::ePF_LIVE);
					Luminosity = o->Alpha;
					Vector(o->TurningForce[0] * Luminosity, o->TurningForce[1] * Luminosity, o->TurningForce[2] * Luminosity, o->Light);
					if (o->Scale > 0)
					{
						o->Scale -= (GetLargeRand(2) + 4)*0.01f;
					}
					else
					{
						oh->Flags.Clear(PARTICLE::ePF_LIVE);
					}
					o->Position[2] += o->Gravity;
					o->Rotation += 3.0f;
				}
				else if (o->SubType == 2)
				{
					if (o->LifeTime < 10)
					{
						o->Alpha -= 0.2f;
					}
					else if (o->Alpha < 1.0f)
					{
						o->Alpha += (GetLargeRand(2) + 2)*0.1f;
					}
					else
					{
						o->Alpha = 1.0f;
					}

					if (o->Alpha < 0.1f)
						oh->Flags.Clear(PARTICLE::ePF_LIVE);
					Luminosity = o->Alpha;
					Vector(o->TurningForce[0] * Luminosity, o->TurningForce[1] * Luminosity, o->TurningForce[2] * Luminosity, o->Light);
					if (o->Scale > 0)
					{
						o->Scale -= (GetLargeRand(3) + 7)*0.01f;
					}
					else
					{
						oh->Flags.Clear(PARTICLE::ePF_LIVE);
					}
					o->Position[0] += o->Velocity[0];
					o->Position[1] += o->Velocity[1];
					o->Position[2] += o->Gravity;
					o->Rotation += 3.0f;
				}
				else if (o->SubType == 10)
				{
					if (o->LifeTime < 15)
					{
						o->Alpha -= 0.2f;
					}
					else if (o->Alpha < 1.0f)
					{
						o->Alpha += (GetLargeRand(2) + 2)*0.1f;
					}
					else
					{
						o->Alpha = 1.0f;
					}

					if (o->Alpha < 0.1f)
						oh->Flags.Clear(PARTICLE::ePF_LIVE);
					Luminosity = o->Alpha;
					Vector(o->TurningForce[0] * Luminosity, o->TurningForce[1] * Luminosity, o->TurningForce[2] * Luminosity, o->Light);
					if (o->Scale > 0)
					{
						o->Scale -= (GetLargeRand(3) + 5)*0.01f;
					}
					else
					{
						oh->Flags.Clear(PARTICLE::ePF_LIVE);
					}
					o->Position[2] += o->Gravity;
					o->Rotation += 3.0f;
				}
				else if (o->SubType == 3)
				{
					if (o->LifeTime < 15)
					{
						o->Alpha -= 0.2f;
					}
					else if (o->Alpha < 1.0f)
					{
						o->Alpha += (GetLargeRand(2) + 2)*0.1f;
					}
					else
					{
						o->Alpha = 1.0f;
					}

					if (o->Alpha < 0.1f)
						oh->Flags.Clear(PARTICLE::ePF_LIVE);
					Luminosity = o->Alpha;
					Vector(o->TurningForce[0] * Luminosity, o->TurningForce[1] * Luminosity, o->TurningForce[2] * Luminosity, o->Light);
					if (o->Scale > 0)
					{
						o->Scale -= (GetLargeRand(3) + 2)*0.01f;
					}
					else
					{
						oh->Flags.Clear(PARTICLE::ePF_LIVE);
					}
					o->Position[2] += o->Gravity;
					o->Rotation += 3.0f;
				}
				else if (o->SubType == 4)
				{
					if (o->LifeTime < 5)
					{
						o->Alpha -= 0.1f;
					}
					else if (o->Alpha < 1.0f)
					{
						o->Alpha += (GetLargeRand(2) + 2)*0.1f;
					}
					else
					{
						o->Alpha = 1.0f;
					}

					if (o->Alpha < 0.1f)
						oh->Flags.Clear(PARTICLE::ePF_LIVE);
					Luminosity = o->Alpha;
					Vector(o->TurningForce[0] * Luminosity, o->TurningForce[1] * Luminosity, o->TurningForce[2] * Luminosity, o->Light);

					o->Scale += (GetLargeRand(3) + 8)*0.01f;
					if (o->LifeTime < 3)
					{
						o->Scale += (10 - o->LifeTime)*0.02f;
					}

					AngleMatrix(o->Angle, Matrix);
					EVector3 vMoveDir, vMoveVec;
					Vector(0.0f, -1.0f, 0.0f, vMoveDir);
					VectorRotate(vMoveDir, Matrix, vMoveVec);
					VectorScale(vMoveVec, 8.0f, vMoveVec);
					VectorAdd(o->Position, vMoveVec, o->Position);

					VectorSubtract(o->Position, o->StartPosition, o->Position);
					VectorCopy(o->Target->Position, o->StartPosition);
					VectorAdd(o->Position, o->Target->Position, o->Position);

					o->Rotation += 5.0f;
				}
				break;
			case BITMAP_FIRE_HIK3:
			case BITMAP_FIRE_HIK3_MONO:
				if (o->SubType == 0)
				{
					if (o->LifeTime < 10)
					{
						o->Alpha -= 0.2f;
					}
					else if (o->Alpha < 1.0f)
					{
						o->Alpha += (GetLargeRand(2) + 2)*0.1f;
					}
					else
					{
						o->Alpha = 1.0f;
					}

					if (o->Alpha < 0.1f)
						oh->Flags.Clear(PARTICLE::ePF_LIVE);
					Luminosity = o->Alpha;
					Vector(o->TurningForce[0] * Luminosity, o->TurningForce[1] * Luminosity, o->TurningForce[2] * Luminosity, o->Light);
					if (o->Scale > 0)
					{
						o->Scale -= (GetLargeRand(3) + 7)*0.01f;
					}
					else
					{
						oh->Flags.Clear(PARTICLE::ePF_LIVE);
					}
					o->Position[2] += o->Gravity;
					o->Rotation += 3.0f;
				}
				else if (o->SubType == 1)
				{
					if (o->LifeTime < 15)
					{
						o->Alpha -= 0.2f;
					}
					else if (o->Alpha < 1.0f)
					{
						o->Alpha += (GetLargeRand(2) + 2)*0.1f;
					}
					else
					{
						o->Alpha = 1.0f;
					}

					if (o->Alpha < 0.1f)
						oh->Flags.Clear(PARTICLE::ePF_LIVE);
					Luminosity = o->Alpha;
					Vector(o->TurningForce[0] * Luminosity, o->TurningForce[1] * Luminosity, o->TurningForce[2] * Luminosity, o->Light);
					if (o->Scale > 0)
					{
						o->Scale -= (GetLargeRand(2) + 6)*0.01f;
					}
					else
					{
						oh->Flags.Clear(PARTICLE::ePF_LIVE);
					}
					o->Position[2] += o->Gravity;
					o->Rotation += 3.0f;
				}
				else if (o->SubType == 2)
				{
					if (o->LifeTime < 10)
					{
						o->Alpha -= 0.2f;
					}
					else if (o->Alpha < 1.0f)
					{
						o->Alpha += (GetLargeRand(2) + 2)*0.1f;
					}
					else
					{
						o->Alpha = 1.0f;
					}

					if (o->Alpha < 0.1f)
						oh->Flags.Clear(PARTICLE::ePF_LIVE);
					Luminosity = o->Alpha;
					Vector(o->TurningForce[0] * Luminosity, o->TurningForce[1] * Luminosity, o->TurningForce[2] * Luminosity, o->Light);
					if (o->Scale > 0)
					{
						o->Scale -= (GetLargeRand(3) + 7)*0.01f;
					}
					else
					{
						oh->Flags.Clear(PARTICLE::ePF_LIVE);
					}
					o->Position[0] += o->Velocity[0];
					o->Position[1] += o->Velocity[1];
					o->Position[2] += o->Gravity;
					o->Rotation += 3.0f;
				}
				else if (o->SubType == 3)
				{
					if (o->LifeTime < 10)
					{
						o->Alpha -= 0.2f;
					}
					else if (o->Alpha < 1.0f)
					{
						o->Alpha += (GetLargeRand(2) + 2)*0.1f;
					}
					else
					{
						o->Alpha = 1.0f;
					}

					if (o->Alpha < 0.1f)
						oh->Flags.Clear(PARTICLE::ePF_LIVE);
					Luminosity = o->Alpha;
					Vector(o->TurningForce[0] * Luminosity, o->TurningForce[1] * Luminosity, o->TurningForce[2] * Luminosity, o->Light);
					if (o->Scale > 0)
					{
						o->Scale -= (GetLargeRand(3) + 7)*0.01f;
					}
					else
					{
						oh->Flags.Clear(PARTICLE::ePF_LIVE);
					}
					o->Position[2] += o->Gravity;
					o->Rotation += 3.0f;
				}
				else if (o->SubType == 4)
				{
					if (o->LifeTime < 15)
					{
						o->Alpha -= 0.2f;
					}
					else if (o->Alpha < 1.0f)
					{
						o->Alpha += (GetLargeRand(2) + 2)*0.1f;
					}
					else
					{
						o->Alpha = 1.0f;
					}

					if (o->Alpha < 0.1f)
						oh->Flags.Clear(PARTICLE::ePF_LIVE);
					Luminosity = o->Alpha;
					Vector(o->TurningForce[0] * Luminosity, o->TurningForce[1] * Luminosity, o->TurningForce[2] * Luminosity, o->Light);
					if (o->Scale > 0)
					{
						o->Scale -= (GetLargeRand(4) + 5)*0.01f;
					}
					else
					{
						oh->Flags.Clear(PARTICLE::ePF_LIVE);
					}
					o->Position[2] += (o->Gravity);//*((mu_float)o->LifeTime/39.0f));
					o->Rotation += 3.0f;
				}
				else if (o->SubType == 5)
				{
					if (o->LifeTime < 4)
					{
						o->Alpha -= 0.1f;
					}
					else if (o->Alpha < 1.0f)
					{
						o->Alpha += (GetLargeRand(2) + 2)*0.1f;
					}
					else
					{
						o->Alpha = 1.0f;
					}

					if (o->Alpha < 0.1f)
						oh->Flags.Clear(PARTICLE::ePF_LIVE);
					Luminosity = o->Alpha;
					Vector(o->TurningForce[0] * Luminosity, o->TurningForce[1] * Luminosity, o->TurningForce[2] * Luminosity, o->Light);

					o->Scale += (GetLargeRand(3) + 8)*0.01f;
					if (o->LifeTime < 3)
					{
						o->Scale += (10 - o->LifeTime)*0.02f;
					}

					AngleMatrix(o->Angle, Matrix);
					EVector3 vMoveDir, vMoveVec;
					Vector(0.0f, -1.0f, 0.0f, vMoveDir);
					VectorRotate(vMoveDir, Matrix, vMoveVec);
					VectorScale(vMoveVec, 8.0f, vMoveVec);
					VectorAdd(o->Position, vMoveVec, o->Position);

					VectorSubtract(o->Position, o->StartPosition, o->Position);
					VectorCopy(o->Target->Position, o->StartPosition);
					VectorAdd(o->Position, o->Target->Position, o->Position);

					o->Rotation += 5.0f;
				}
				break;
			case BITMAP_LIGHT + 1:
				if (o->SubType == 2)
				{
					o->Scale *= 0.92f;
				}
				else if (o->SubType == 3)
				{
					o->Scale *= 1.3f;
					o->Light[0] *= 0.9f;
					o->Light[1] *= 0.9f;
					o->Light[2] *= 0.9f;
				}
				else if (o->SubType == 5)
				{
					o->LifeTime = 5;
					o->Light[0] *= 0.7f;
					o->Light[1] *= 0.7f;
					o->Light[2] *= 0.7f;
				}
				else if (o->SubType == 4)
				{

				}
				else
				{
					o->Position[2] -= o->Gravity;
					o->Position[1] -= 1.0f;
					o->Gravity += 1.0f;
					o->Scale *= 0.95f;
				}
				break;
			case BITMAP_BLOOD:
			case BITMAP_BLOOD + 1:
				o->Frame = (12 - o->LifeTime) / 3;
				//o->Position[2] += o->Gravity;
				//o->Gravity -= 1.5f;
				VectorScale(o->Velocity, 0.95f, o->Velocity);
				VectorAdd(o->Position, o->Velocity, o->Position);
				break;

				//////////////////////////////////////////////////////////////////////////

			case BITMAP_SPARK:
				Luminosity = (mu_float)(o->LifeTime) / 16.0f;
				if (o->LifeTime < 0)
				{
					oh->Flags.Clear(PARTICLE::ePF_LIVE);
				}
				if (o->SubType == 11)
				{
					Vector(Luminosity, Luminosity*0.3f, Luminosity*0.3f, o->Light);
				}
				else if (o->SubType != 8)
					Vector(Luminosity, Luminosity, Luminosity, o->Light);
				o->Position[2] += o->Gravity;
				if (o->SubType == 5)
				{
					o->Gravity -= 0.3f;
				}
				else if (o->SubType == 6)
				{
					o->Gravity -= 0.3f;
					Vector(0.9f, 0.9f, 0.9f, o->Light);
				}
				else if (o->SubType == 8
						 || o->SubType == 10)
				{
					o->Light[0] /= 1.02f;
					o->Light[1] /= 1.02f;
					o->Light[2] /= 1.02f;
				}
				if (o->SubType == 7)
					o->Gravity -= 0.6f;
				else if (o->SubType == 9)
					o->Gravity -= 0.4f;
				else
					o->Gravity -= 2.0f;

				Height = RequestTerrainHeight(o->Position[0], o->Position[1]);
				if (o->Position[2] < Height)
				{
					o->Position[2] = Height;
					o->Gravity = -o->Gravity*0.6f;
					o->LifeTime -= 4;
				}
				VectorAdd(o->Position, o->Velocity, o->Position);
				if (o->SubType == 5 || o->SubType == 6)
				{
					EVector3 p;
					VectorSubtract(o->StartPosition, o->Target->Position, p);
					VectorCopy(o->Target->Position, o->StartPosition);
					VectorSubtract(o->Position, p, o->Position);
				}
				break;

				//////////////////////////////////////////////////////////////////////////

			case BITMAP_SPARK + 1:
				switch (o->SubType)
				{
				case 0:
					o->Scale -= 0.5f;
					if (o->Scale < 0.2f) oh->Flags.Clear(PARTICLE::ePF_LIVE);
					break;
				case 1:
					o->Scale -= 2.0f;
					if (o->Scale < 0.2f) oh->Flags.Clear(PARTICLE::ePF_LIVE);
					break;
				case 2:
					if (o->LifeTime <= 0) oh->Flags.Clear(PARTICLE::ePF_LIVE);
					if (o->LifeTime > 5 && o->Target->Flags.Get(OBJECT::eOF_LIVE) == 0) o->LifeTime = 5;

					o->Light[0] = o->LifeTime / 5.0f;
					o->Light[1] = o->LifeTime / 5.0f;
					o->Light[2] = o->LifeTime / 5.0f;

					o->Scale += 0.05f;
					VectorAdd(o->StartPosition, o->Velocity, o->Position);
					VectorCopy(o->Position, o->StartPosition);
					break;
				case 3:
					o->Scale *= 0.8f;
					break;
				case 4:
					{
						if (o->LifeTime <= 0) oh->Flags.Clear(PARTICLE::ePF_LIVE);
						if (o->LifeTime > 5 && o->Target->Flags.Get(OBJECT::eOF_LIVE) == 0) o->LifeTime = 5;

						o->Light[0] = o->LifeTime / 5.0f;
						o->Light[1] = o->LifeTime / 5.0f;
						o->Light[2] = o->LifeTime / 5.0f;

						o->Scale += 0.08f;

						VectorAdd(o->StartPosition, o->Velocity, o->Position);
						VectorCopy(o->Position, o->StartPosition);

						EVector3 p;
						VectorSubtract(o->Target->Position, o->Target->StartPosition, p);
						VectorAdd(o->Position, p, o->Position);
					}
					break;
				case 5:
					o->Scale *= 0.9f;
					o->Light[0] /= 1.03f;
					o->Light[1] /= 1.03f;
					o->Light[2] /= 1.03f;
					break;
				case 6:
					{

						if (o->LifeTime <= 0) oh->Flags.Clear(PARTICLE::ePF_LIVE);
						o->Position[2] -= o->Gravity;
						o->Position[1] -= 1.0f;
						o->Gravity += 0.1f;
						mu_float fLight = (mu_float)(GetLargeRand(10)) / 100.0f + 0.7f;
						Vector(o->Light[0], fLight, o->Light[2], o->Light);
					}
					break;
				case 7:
					{
						o->Scale -= 0.02f;
						o->Alpha -= 0.001f;
						if (o->Alpha <= 0.0f) oh->Flags.Clear(PARTICLE::ePF_LIVE);
						o->Position[2] -= o->Gravity;
						mu_float fLight = (mu_float)(GetLargeRand(50)) / 100.0f + 0.2f;
						Vector(0.0f, fLight, 0.0f, o->Light);
					}
					break;
				case 8:
					{
						o->Scale -= 0.5f;
						if (o->Scale < 0.2f) oh->Flags.Clear(PARTICLE::ePF_LIVE);
					}
					break;
				case 9:
					{
						o->Velocity[0] -= 1.2f;
						o->Velocity[2] -= 1.0f;
						o->Scale -= 0.08f;
						if (o->LifeTime <= 0)
							oh->Flags.Clear(PARTICLE::ePF_LIVE);
						if (o->Scale < 0.2f)
							oh->Flags.Clear(PARTICLE::ePF_LIVE);
					}
					break;
				case 10:
					{
						o->Light[0] /= 1.08f;
						o->Light[1] /= 1.08f;
						o->Light[2] /= 1.08f;
						o->Scale -= 0.03f;
						o->Alpha -= 0.001f;
						if (o->Alpha <= 0.0f) oh->Flags.Clear(PARTICLE::ePF_LIVE);
					}
					break;
				case 13:
				case 11:
					{
						o->Light[0] /= 1.05f;
						o->Light[1] /= 1.05f;
						o->Light[2] /= 1.05f;
						o->Scale -= 0.04f;
						o->Alpha -= 0.001f;
						if (o->Alpha <= 0.0f) oh->Flags.Clear(PARTICLE::ePF_LIVE);
					}
					break;
				case 12:
					{
						mu_float fLight = GetLargeRand(2);
						Vector(fLight - 0.6f, fLight - 0.6f, fLight - 0.8f, o->Light);
						o->Scale -= 0.04f;
						o->Alpha -= 0.001f;
						if (o->Alpha <= 0.0f) oh->Flags.Clear(PARTICLE::ePF_LIVE);
					}
					break;
				case 14:
					{
						o->Light[0] /= 1.05f;
						o->Light[1] /= 1.05f;
						o->Light[2] /= 1.05f;
						o->Scale -= 0.01f;
						o->Alpha -= 0.001f;
						if (o->Alpha <= 0.0f) oh->Flags.Clear(PARTICLE::ePF_LIVE);
					}
					break;
				case 15:	//^ 펜릴 이펙트 관련
					{
						o->Light[0] /= 1.05f;
						o->Light[1] /= 1.05f;
						o->Light[2] /= 1.05f;
						o->Alpha -= 0.001f;
						if (o->Alpha <= 0.0f) oh->Flags.Clear(PARTICLE::ePF_LIVE);
					}
					break;
				case 16:
				case 18:
					{
						mu_float Height = RequestTerrainHeight(o->Position[0], o->Position[1]);

						if (Height + 2.0f >= o->Position[2])
							o->Frame = 88;

						if (o->Frame == 77)
						{
							o->Gravity *= 1.03f;
							o->Position[2] -= o->Gravity;
						}
						else if (o->Frame == 88)
						{
							o->Gravity /= 1.2f;
							if (o->Gravity <= 0.1f)
							{
								o->Gravity = 0.1f;
								o->Frame = 99;
							}
							o->Position[2] += o->Gravity;
						}
						else if (o->Frame == 99)
						{
							o->Gravity *= 1.2f;
							o->Position[2] -= o->Gravity;
						}

						o->Alpha -= 0.007f;
						if (o->Alpha <= 0.0f)
							oh->Flags.Clear(PARTICLE::ePF_LIVE);
					}
					break;
				case 17:
					{
						if (o->Position[2] >= o->StartPosition[2] + 350.0f)
						{
							o->Light[0] /= 1.05f;
							o->Light[1] /= 1.05f;
							o->Light[2] /= 1.05f;
						}

						if (o->Light[0] <= 0.05f)
							oh->Flags.Clear(PARTICLE::ePF_LIVE);

						o->Position[0] += EMath::Sin(FWorldTime*o->Rotation)*o->Gravity*0.3f;
						//o->Position[1] += EMath::Sin(FWorldTime*o->Rotation)*o->Gravity*0.3f;
						o->Position[2] += o->Gravity;

					}
					break;
				case 19:
					{
						if (o->LifeTime <= 0)
							oh->Flags.Clear(PARTICLE::ePF_LIVE);

						o->Position[2] += o->Gravity;
						o->Gravity += 0.1f;

						o->Light[0] /= 1.1f;
						o->Light[1] /= 1.1f;
						o->Light[2] /= 1.1f;
					}
					break;
				case 20:
					{
						o->Light[0] /= 1.02f;
						o->Light[1] /= 1.02f;
						o->Light[2] /= 1.02f;

						if (o->Light[0] <= 0.05f)
							oh->Flags.Clear(PARTICLE::ePF_LIVE);

						o->Position[2] += (o->Gravity * 0.5f);
						o->Gravity -= 1.5f;

						Height = RequestTerrainHeight(o->Position[0], o->Position[1]);
						if (o->Position[2] < Height + 3.0f)
						{
							o->Position[2] = Height + 3.0f;
							o->Gravity = -o->Gravity*0.3f;
							o->LifeTime -= 2;
						}
					}
					break;

				case 21:
					{
						o->Light[0] /= 1.02f;
						o->Light[1] /= 1.02f;
						o->Light[2] /= 1.02f;
					}
					break;
				case 22:
					{
						o->Light[0] /= 1.02f;
						o->Light[1] /= 1.02f;
						o->Light[2] /= 1.02f;

						if (o->Light[0] <= 0.05f)
							oh->Flags.Clear(PARTICLE::ePF_LIVE);

						o->Position[2] += (o->Gravity * 0.5f);
						o->Gravity -= 1.5f;

						Height = RequestTerrainHeight(o->Position[0], o->Position[1]);
						if (o->Position[2] < Height + 3.0f)
						{
							o->Position[2] = Height + 3.0f;
							o->Gravity = -o->Gravity*0.3f;
							o->LifeTime -= 2;
						}
					}
					break;

				case 23:
					{
						if (o->LifeTime < 10 && o->Rotation == 0)
						{
							VectorScale(o->Velocity, -1.0f, o->Velocity);
							o->Rotation = 1;
						}
						o->Light[0] /= 1.05f;
						o->Light[1] /= 1.05f;
						o->Light[2] /= 1.05f;
						o->Scale -= 0.02f;
						o->Alpha -= 0.0001f;
						if (o->Alpha <= 0.0f) oh->Flags.Clear(PARTICLE::ePF_LIVE);
					}
					break;
				case 24:
					{
						if (o->LifeTime < 10 && o->Rotation == 0)
						{
							VectorScale(o->Velocity, -1.0f, o->Velocity);
							o->Rotation = 1;
						}

						if (o->LifeTime <= 10)
						{
							o->Alpha -= 0.05f;
							o->Light[0] *= o->Alpha;
							o->Light[1] *= o->Alpha;
							o->Light[2] *= o->Alpha;
						}

						o->Scale -= 0.02f;
					}
					break;
				case 25:
					{
						if (o->LifeTime == 19)
						{
							o->Velocity[0] -= 1.0f;
							o->Velocity[1] -= 1.0f;
							o->Velocity[2] -= 1.0f;
						}

						o->Light[0] /= 1.05f;
						o->Light[1] /= 1.05f;
						o->Light[2] /= 1.05f;
						o->Scale = EMath::Sin(o->LifeTime*(Q_PI / 40.0f));
						o->Alpha -= 0.001f;
						if (o->Alpha <= 0.0f) oh->Flags.Clear(PARTICLE::ePF_LIVE);
					}
					break;
				case 26:
					{
						if (o->LifeTime <= 0)
							oh->Flags.Clear(PARTICLE::ePF_LIVE);

						o->Light[0] /= 1.1f;
						o->Light[1] /= 1.1f;
						o->Light[2] /= 1.1f;

						o->Scale -= 0.05f;
						VectorAdd(o->StartPosition, o->Velocity, o->Position);
						VectorCopy(o->Position, o->StartPosition);
					}
					break;
				case 27:
					{
						o->Light[0] /= 1.02f;
						o->Light[1] /= 1.02f;
						o->Light[2] /= 1.02f;

						o->Scale /= 1.03f;
					}
					break;
				case 28:
					{
						o->Light[0] /= 1.02f;
						o->Light[1] /= 1.02f;
						o->Light[2] /= 1.02f;

						o->Scale /= 1.01f;

						if (o->Light[0] <= 0.03f)
							oh->Flags.Clear(PARTICLE::ePF_LIVE);

						o->Position[2] += (o->Gravity * 0.5f);
						o->Gravity -= 1.5f;

						Height = RequestTerrainHeight(o->Position[0], o->Position[1]);
						if (o->Position[2] < Height + 3.0f)
						{
							o->Position[2] = Height + 3.0f;
							o->Gravity = -o->Gravity*0.3f;
							o->LifeTime -= 2;
						}
					}
					break;
				case 29:
					{
						o->Light[0] /= 1.02f;
						o->Light[1] /= 1.02f;
						o->Light[2] /= 1.02f;

						o->Scale /= 1.01f;

						if (o->Light[0] <= 0.03f)
							oh->Flags.Clear(PARTICLE::ePF_LIVE);

						o->Position[2] += (o->Gravity * 0.5f);
						o->Gravity -= 1.5f;

						Height = RequestTerrainHeight(o->Position[0], o->Position[1]);
						if (o->Position[2] < Height + 3.0f)
						{
							o->Position[2] = Height + 3.0f;
							o->Gravity = -o->Gravity*0.3f;
							o->LifeTime -= 2;
						}
					}
					break;
				case 30:
					{

						if (o->LifeTime <= 45)
						{
							o->Velocity[0] += EMath::Sin(Q_PI / 180.0f * mu_float(GetLargeRand(360)));
							o->Velocity[1] += EMath::Cos(Q_PI / 180.0f * mu_float(GetLargeRand(360)));
						}

						o->Light[0] /= 1.05f;
						o->Light[1] /= 1.05f;
						o->Light[2] /= 1.05f;

						o->Scale -= 0.01f;

						o->LifeTime -= 1;
						if (o->LifeTime <= 0) oh->Flags.Clear(PARTICLE::ePF_LIVE);
					}
					break;
				case 31:
					{
						o->Position[2] += o->Gravity;
						o->Gravity -= 2.0f;

						Height = RequestTerrainHeight(o->Position[0], o->Position[1]);
						if (o->Position[2] < Height)
						{
							o->Position[2] = Height;
							o->Gravity = -o->Gravity*0.6f;
							o->LifeTime -= 4;
						}
						VectorAdd(o->Position, o->Velocity, o->Position);
					}
					break;
				}
				break;

				//////////////////////////////////////////////////////////////////////////

			case BITMAP_SPARK + 2:
				{
					if (o->SubType == 0)
					{
						o->Frame = (16 - o->LifeTime) / 4;
					}
					else if (o->SubType == 1)
					{
						if (o->LifeTime % 3 == 0)
						{
							EVector3 vPos;
							VectorCopy(o->Position, vPos);
							vPos[0] += (mu_float)(GetLargeRand(100) - 50);
							vPos[1] += (mu_float)(GetLargeRand(100) - 50);
							vPos[2] += (mu_float)(GetLargeRand(100) - 50);
							CreateBomb(ThreadIndex, vPos, true);
						}
					}
					else if ((o->SubType == 2 || o->SubType == 3) && o->Target != nullptr)
					{
						o->Frame = (16 - o->LifeTime) / 4;

						// 플레이어 모델
						MUModel* pModel = &MODELS::Data[o->Target->Type];
						EVector3 vPos;

						switch (o->SubType)
						{
						case 2:
							// 플레이어 왼손
							pModel->TransformByObjectBone(ThreadIndex, vPos, o->Target, 37);
							break;
						case 3:
							// 플레이어 오른손
							pModel->TransformByObjectBone(ThreadIndex, vPos, o->Target, 28);
							break;
						}

						VectorCopy(vPos, o->Position);
					}
				}
				break;


			case BITMAP_SMOKE + 1:
			case BITMAP_SMOKE + 4:
				Luminosity = (mu_float)(o->LifeTime) / 32.0f;
				Vector(Luminosity, Luminosity, Luminosity, o->Light);
				o->Scale += 0.08f;
				//o->Angle[0] += 4.0f;
				VectorAdd(o->Position, o->Velocity, o->Position);
				VectorScale(o->Velocity, 0.9f, o->Velocity);
				if (o->SubType == 6)
				{
					o->Scale -= 0.05f;
					o->Position[2] += 4.0f;
				}
				else if (o->SubType != 5)
				{
					o->Position[2] = RequestTerrainHeight(o->Position[0], o->Position[1]) + TEXTURES::Data[o->Type].GetFloatHeight()*o->Scale*0.5f;
				}
				break;

			case BITMAP_SHINY:
				if (o->SubType == 2)
				{
					o->Rotation -= 12.0f;

					o->Velocity[2] -= (mu_float)(GetLargeRand(8))*0.05f;
					VectorAdd(o->Position, o->Velocity, o->Position);

					o->Light[0] /= 1.05f;
					o->Light[1] /= 1.05f;
					o->Light[2] /= 1.05f;

					CreateSprite(BITMAP_LIGHT, o->Position, o->Scale / 1.5f, o->Light, nullptr);
				}
				else if (o->SubType == 3)
				{
					o->Light[0] /= 1.05f;
					o->Light[1] /= 1.05f;
					o->Light[2] /= 1.05f;
					o->Scale -= 0.04f;
					o->Alpha -= 0.001f;
					if (o->Alpha <= 0.0f) oh->Flags.Clear(PARTICLE::ePF_LIVE);
				}
				else if (o->SubType == 4)
				{
					o->Rotation += 20.0f;
					o->Position[2] += (o->Gravity * 0.5f);

					o->Light[0] /= 1.02f;
					o->Light[1] /= 1.02f;
					o->Light[2] /= 1.02f;

					o->Gravity -= 1.5f;
					o->Scale -= 0.01f;

					VectorAdd(o->Position, o->Velocity, o->Position);
				}
				else if (o->SubType == 5)
				{
					o->Rotation += 10.0f;
					o->Position[2] += o->Gravity;

					o->Light[0] /= 1.02f;
					o->Light[1] /= 1.02f;
					o->Light[2] /= 1.02f;

					o->Scale -= 0.01f;
				}
				else if (o->SubType == 6)
				{
					o->Light[0] /= 1.01f;
					o->Light[1] /= 1.01f;
					o->Light[2] /= 1.01f;

					if (o->Light[0] < 0.2f)
					{
						oh->Flags.Clear(PARTICLE::ePF_LIVE);
					}

					o->StartPosition[0] /= 1.01f;
					if (o->LifeTime < 60 && GetLargeRand(2) == 0)
					{
						o->Scale = 0;
					}
					else
					{
						o->Scale = o->StartPosition[0];
					}

					if (o->LifeTime < 60)
					{
						o->Velocity[0] /= 1.04f;
						o->Velocity[1] /= 1.04f;
						o->Velocity[2] /= 1.04f;

						o->Position[2] -= (o->Gravity * 0.1f);
						o->Gravity += 0.5f;
					}
				}
				else if (o->SubType == 7)
				{
					o->Scale = EMath::Sin(o->LifeTime*2.0f*(Q_PI / 180.0f));
				}
				else if (o->SubType == 8)
				{
					o->Light[0] /= 1.01f;
					o->Light[1] /= 1.01f;
					o->Light[2] /= 1.01f;

					if (o->Light[0] < 0.2f)
					{
						oh->Flags.Clear(PARTICLE::ePF_LIVE);
					}

					o->StartPosition[0] /= 1.01f;
					if (o->LifeTime < 60 && GetLargeRand(2) == 0)
					{
						o->Scale = 0;
					}
					else
					{
						o->Scale = o->StartPosition[0];
					}

					if (o->LifeTime < 60)
					{
						o->Velocity[0] /= 1.04f;
						o->Velocity[1] /= 1.04f;
						o->Velocity[2] /= 1.04f;

						o->Position[2] -= (o->Gravity * 0.1f);
						o->Gravity += 0.5f;
					}
				}
				else if (o->SubType == 9)
				{
					o->Light[0] /= 1.01f;
					o->Light[1] /= 1.01f;
					o->Light[2] /= 1.01f;

					if (o->Light[0] < 0.2f)
					{
						oh->Flags.Clear(PARTICLE::ePF_LIVE);
					}

					o->StartPosition[0] /= 1.01f;
					if (o->LifeTime < 60 && GetLargeRand(2) == 0)
					{
						o->Scale = 0;
					}
					else
					{
						o->Scale = o->StartPosition[0];
					}

					if (o->LifeTime < 60)
					{
						o->Velocity[0] /= 1.04f;
						o->Velocity[1] /= 1.04f;
						o->Velocity[2] /= 1.04f;

						o->Position[2] -= (o->Gravity * 0.1f);
						o->Gravity += 0.5f;

						if (GetLargeRand(5) == 0)
						{
							Position[0] = o->Position[0] + GetLargeRand(40) - 20;
							Position[1] = o->Position[1] + GetLargeRand(40) - 20;
							Position[2] = o->Position[2];
							Vector(0.8f + (GetLargeRand(200))*0.001f, 0.5f + (GetLargeRand(200))*0.001f, 0.1f + (GetLargeRand(100))*0.001f, Light);
							// 							Vector(1.0f,0.7f,0.2f, Light);
							CreateParticle(ThreadIndex, BITMAP_SHINY, Position, o->Angle, Light, 8);
						}
					}
				}
				else
				{
					o->Scale = EMath::Sin(o->LifeTime*10.0f*(Q_PI / 180.0f));
					if (o->SubType == 1)
					{
						o->Scale *= 0.75f;
						o->Rotation -= 12.0f;
					}
				}
				break;
			case BITMAP_CHERRYBLOSSOM_EVENT_PETAL:
				{
					if (o->SubType == 0)
					{
						o->Rotation = (mu_float)(GetLargeRand(360));
						o->Scale = EMath::Sin(o->LifeTime*(Q_PI / 180.0f));
						o->Alpha -= 0.001f;
						if (o->Alpha <= 0.0f) oh->Flags.Clear(PARTICLE::ePF_LIVE);
					}
					else if (o->SubType == 1)
					{
						if (o->LifeTime == 69)
						{
							o->Velocity[0] -= 1.0f;
							o->Velocity[1] -= 1.0f;
							o->Velocity[2] -= 1.0f;
						}

						o->Light[0] /= 1.01f;
						o->Light[1] /= 1.01f;
						o->Light[2] /= 1.01f;

						o->Alpha -= 0.001f;
						if (o->Alpha <= 0.0f) oh->Flags.Clear(PARTICLE::ePF_LIVE);
					}
				}
				break;
			case BITMAP_CHERRYBLOSSOM_EVENT_FLOWER:
				{
					if (o->SubType == 0)
					{
						o->Scale = EMath::Sin(o->LifeTime*(Q_PI / 180.0f));
						o->Alpha -= 0.002f;
						if (o->Alpha <= 0.0f) oh->Flags.Clear(PARTICLE::ePF_LIVE);

						//o->Light[0] /= 1.045f;
						//o->Light[1] /= 1.045f;
						//o->Light[2] /= 1.045f;

						o->Position[2] += (o->Gravity * 0.5f);
						o->Gravity -= 1.5f;

						o->Rotation = (mu_float)(GetLargeRand(360));

						Height = RequestTerrainHeight(o->Position[0], o->Position[1]);
						if (o->Position[2] < Height + 3.0f)
						{
							o->Position[2] = Height + 3.0f;
							o->Gravity = -o->Gravity*0.3f;
							o->LifeTime -= 2;
						}
					}
				}
				break;
			case BITMAP_SHINY + 1:
				if (o->SubType == 99)
				{
					VectorCopy(o->Target->Position, o->Position);
					o->Position[2] += 50.0f;
					//                    o->Scale *= 0.75f;
					o->Rotation -= 1.0f;
				}
				else
				{
					if (o->SubType == 5)
					{
						VectorCopy(o->Target->Angle, o->Angle);
						o->Scale -= 0.06f;
						o->Position[2] += o->Gravity*10.0f;
						//o->Light[0] = o->LifeTime/10.0f;
						//o->Light[1] = o->Light[0];
						//o->Light[2] = o->Light[0];
					}
					else
					{
						o->Scale = EMath::Sin(o->LifeTime*5.0f*(Q_PI / 180.0f))*5.0f;
						//Luminosity = (mu_float)(o->LifeTime)/36.0f;
						//Vector(Luminosity*0.6f,Luminosity*0.8f,Luminosity,o->Light);
						if (o->SubType >= 2) {
							o->Scale *= 0.75f;
							o->Rotation -= 12.0f;
						}
						HandPosition(ThreadIndex, o);
					}
				}
				break;
			case BITMAP_SHINY + 2:
				o->Scale = EMath::Sin(o->LifeTime*10.0f*(Q_PI / 180.0f))*3.0f;
				HandPosition(ThreadIndex, o);
				break;
			case BITMAP_SHINY + 4:
				if (o->SubType == 0)
				{
					o->Scale = EMath::Sin(o->LifeTime*10.0f*(Q_PI / 180.0f))*3.0f + 2.0f;
					Luminosity = (mu_float)(o->LifeTime) / 5.0f;
					Vector(Luminosity, Luminosity, Luminosity, o->Light);
				}
				else if (o->SubType == 1)
				{
					o->Scale = EMath::Sin(o->Gravity*(Q_PI / 180.0f))*3.0f + 1.5f;
					o->Gravity += (15 - o->LifeTime)*6.0f;
					if (o->Gravity > 90.0f)
					{
						o->Gravity = 90.0f;
					}
					if (o->LifeTime < 6)
					{
						o->Light[0] *= 0.5f;
						o->Light[1] *= 0.5f;
						o->Light[2] *= 0.5f;
					}
				}
				break;
			case BITMAP_SHINY + 6:
				if (o->SubType == 0)
				{
					o->Rotation += 5.0f;
					o->Scale -= 0.02f;
					o->Alpha -= 0.001f;
					if (o->Alpha <= 0.0f)
						oh->Flags.Clear(PARTICLE::ePF_LIVE);
					o->Position[2] -= o->Gravity;
					mu_float fLight = (mu_float)(GetLargeRand(10)) / 100.0f - 0.05f;
					o->Light[0] += fLight;
					o->Light[1] += fLight;
					o->Light[2] += fLight;
				}
				else if (o->SubType == 1)
				{
					o->Scale -= 0.01f;
					o->Rotation -= 5.0f;
				}
				break;
			case BITMAP_PIN_LIGHT:
				{
					o->Scale -= 0.02f;
					o->Alpha -= 0.001f;
					if (o->Alpha <= 0.0f)
						oh->Flags.Clear(PARTICLE::ePF_LIVE);
					if (o->SubType == 1)
					{

						EBone Matrix;
						EVector3 p;
						Vector(0.0f, -150.0f, 0.0f, p);
						AngleMatrix(o->Angle, Matrix);
						VectorRotate(o->Position, Matrix, o->Position);
					}
					else
					{
						o->Position[2] -= o->Gravity;
					}
					mu_float fLight = (mu_float)(GetLargeRand(10)) / 100.0f - 0.05f;
					o->Light[0] += fLight;
					o->Light[1] += fLight;
					o->Light[2] += fLight;
				}
				break;
			case BITMAP_ORORA:	// 플레이어 손목 주변을 돌며 커지면서 사라지는 이펙트.
				{
					mu_float fScale, fLight;	// 각 증감값.
											// 플레이어 모델
					MUModel* pModel = &MODELS::Data[o->Target->Type];
					EVector3 vRelativePos, vPos;
					Vector(0.0f, 0.0f, 0.0f, vRelativePos);

					switch (o->SubType)
					{
					case 0:
						fScale = 0.01f;
						fLight = 1.05f;
						o->Rotation += 5.0f;
						// 플레이어 왼손
						pModel->TransformByObjectBone(ThreadIndex, vPos, o->Target, 37);
						break;
					case 1:
						fScale = 0.01f;
						fLight = 1.05f;
						o->Rotation -= 5.0f;
						// 플레이어 오른손
						pModel->TransformByObjectBone(ThreadIndex, vPos, o->Target, 28);
						break;
					case 2:
						fScale = 0.04f;
						//						fLight = 1.12f;
						fLight = 1.33f;
						o->Rotation += 20.0f;
						// 플레이어 왼손
						pModel->TransformByObjectBone(ThreadIndex, vPos, o->Target, 37);
						break;
					case 3:
						fScale = 0.04f;
						//						fLight = 1.12f;
						fLight = 1.33f;
						o->Rotation -= 20.0f;
						// 플레이어 오른손
						pModel->TransformByObjectBone(ThreadIndex, vPos, o->Target, 28);
						break;
					}

					VectorCopy(vPos, o->Position);
					o->Scale += fScale;
					if (o->Scale >= 0.8f)	// 투명해지는 시점.
					{
						o->Light[0] /= fLight;
						o->Light[1] /= fLight;
						o->Light[2] /= fLight;
					}
				}
				break;
			case BITMAP_SNOW_EFFECT_1:
			case BITMAP_SNOW_EFFECT_2:
				o->Rotation += 20.0f;
				o->Position[2] += (o->Gravity * 0.5f);

				o->Light[0] /= 1.02f;
				o->Light[1] /= 1.02f;
				o->Light[2] /= 1.02f;

				o->Gravity -= 1.5f;
				if (o->LifeTime < 10)
					o->Scale += 0.01f;
				else
					o->Scale -= 0.01f;

				VectorAdd(o->Position, o->Velocity, o->Position);
				break;

			case BITMAP_DS_EFFECT:
				o->Rotation += 10.0f;

				if (o->Target != nullptr)
				{
					if (o->Target->CurrentAction != PLAYER_SANTA_2)
					{
						oh->Flags.Clear(PARTICLE::ePF_LIVE);
					}
				}
				break;
			case BITMAP_FIRECRACKER:
				o->Velocity[2] -= .5f;
				break;
			case BITMAP_SWORD_FORCE:
				o->Gravity += 0.01f;
				o->Scale += o->Gravity;
				Luminosity = (mu_float)(o->LifeTime) / 10.0f;
				Vector(Luminosity, Luminosity, Luminosity, o->Light);
				break;
			case BITMAP_TORCH_FIRE:
				if (o->LifeTime <= 0)
				{
					oh->Flags.Clear(PARTICLE::ePF_LIVE);
				}
				else
				{
					o->Position[2] += o->Gravity;

					if (o->Position[2] >= 500.0f)
					{
						oh->Flags.Clear(PARTICLE::ePF_LIVE);
					}
					if (o->LifeTime >= 20.0f)
					{
						o->Scale -= 0.02f;
						if (o->Scale <= 0.02f)
						{
							oh->Flags.Clear(PARTICLE::ePF_LIVE);
						}
					}
				}
				break;

			case BITMAP_GHOST_CLOUD1:
			case BITMAP_GHOST_CLOUD2:
				{
					if (o->SubType == 0)
					{
						VectorAdd(o->Position, o->Velocity, o->Position);

						EVector3 vTemp;
						VectorSubtract(o->Position, o->StartPosition, vTemp);
						mu_float fTemp = VectorLength(vTemp);

						mu_float fSin = EMath::Sin(FWorldTime*0.001f);
						if (fSin > 0.0f)
							o->Rotation += 0.2f + (GetLargeRand(4) * 0.1f);
						else
							o->Rotation -= 0.2f + (GetLargeRand(4) * 0.1f);

						if (fTemp <= 20.0f)
						{
							for (mu_int32 i = 0; i < 3; ++i)
							{
								o->Light[i] = o->TurningForce[i] * 0.1f * (500 - o->LifeTime);
								if (o->Light[i] > o->TurningForce[i])
								{
									o->Light[i] = o->TurningForce[i];
								}
							}
						}
						else
						{
							o->Light[0] /= 1.02f;
							o->Light[1] /= 1.02f;
							o->Light[2] /= 1.02f;
						}

						if (fTemp > 500.0f)
						{
							oh->Flags.Clear(PARTICLE::ePF_LIVE);
						}
					}
				}
				break;
			case BITMAP_CLOUD:
				if (o->SubType == 6)
				{
					VectorAdd(o->Position, o->Velocity, o->Position);
					o->Velocity[0] *= 0.95f;
					o->Velocity[1] *= 0.95f;
					o->Velocity[2] += 0.6f;
					o->Position[0] += (mu_float)(GetLargeRand(4) - 2);
					o->Position[1] += (mu_float)(GetLargeRand(4) - 2);
					o->Position[2] += (mu_float)(GetLargeRand(4) - 2)*0.8f;
					o->Scale += 0.05f;
					o->Light[0] = (mu_float)(o->LifeTime) / 50.0f;
					o->Light[1] = o->Light[0];
					o->Light[2] = o->Light[0];
					o->Rotation += 1.0f + GetLargeRand(2);
				}
				else if (o->SubType == 8)
				{
					if (o->LifeTime <= 0) {
						oh->Flags.Clear(PARTICLE::ePF_LIVE);
					}
					else if (o->LifeTime > 150) {
						if (o->Alpha < 1.0f)
							o->Alpha += 0.04;
						o->Position[2] += 1.0f;
					}
					else {
						if (o->Alpha > 0.1f)
							o->Alpha -= 0.025f;
						o->Position[2] -= 0.5f;
					}
				}
				else if (o->SubType == 9)
				{
					if (o->LifeTime <= 0) {
						oh->Flags.Clear(PARTICLE::ePF_LIVE);
					}
					else if (o->LifeTime > 150) {
						if (o->Alpha < 1.0f)
							o->Alpha += 0.04;
						o->Position[2] += 1.0f;
					}
					else {
						if (o->Alpha > 0.1f)
							o->Alpha -= 0.025f;
						o->Position[2] -= 0.5f;
					}
				}
				else if (o->SubType == 10)
				{
					VectorAdd(o->Position, o->Velocity, o->Position);
					o->TurningForce[0] = 1.5f;

					EVector3 vTemp;
					VectorSubtract(o->Position, o->StartPosition, vTemp);
					mu_float fTemp = VectorLength(vTemp);

					if (fTemp <= 300.0f)
					{
						o->Light[0] += 0.01f;
						o->Light[1] += 0.01f;
						o->Light[2] += 0.01f;
						if (o->Light[0] >= 0.15f)
							Vector(0.15f, 0.15f, 0.15f, o->Light);
					}
					else if (fTemp > 300.0f && fTemp <= 500.0f)
					{
						o->Light[0] /= 1.05f;
						o->Light[1] /= 1.05f;
						o->Light[2] /= 1.05f;
					}
					else
					{
						oh->Flags.Clear(PARTICLE::ePF_LIVE);
					}
				}
				else if (o->SubType == 11)	//@@ MovePaticle
				{
					VectorAdd(o->Position, o->Velocity, o->Position);
					o->TurningForce[0] = 1.5f;

					EVector3 vTemp;
					VectorSubtract(o->Position, o->StartPosition, vTemp);
					mu_float fTemp = VectorLength(vTemp);

					o->Rotation += 0.5f + GetLargeRand(2);

					if (fTemp > 500.0f)
					{
						oh->Flags.Clear(PARTICLE::ePF_LIVE);
					}
				}
				else if (o->SubType == 13)
				{
					o->Rotation += o->Gravity*1.5f;
				}
				else if (o->SubType == 14)
				{
					VectorAdd(o->Position, o->Velocity, o->Position);
					o->Velocity[0] *= 0.95f;
					o->Velocity[1] *= 0.95f;
					o->Velocity[2] += 0.6f;
					o->Position[0] += (mu_float)(GetLargeRand(4) - 2);
					o->Position[1] += (mu_float)(GetLargeRand(4) - 2);
					o->Position[2] += (mu_float)(GetLargeRand(4) - 2)*0.8f;
					o->Scale += 0.05f;
					o->Light[0] = (mu_float)(o->LifeTime) / 50.0f;
					o->Light[1] = o->Light[0];
					o->Light[2] = o->Light[0];
					o->Rotation += 1.0f + GetLargeRand(2);
				}
				else if (o->SubType == 15)
				{
					if (o->LifeTime <= 10)
					{
						oh->Flags.Clear(PARTICLE::ePF_LIVE);
					}

					VectorAdd(o->Position, o->Velocity, o->Position);

					EVector3 vTemp;
					VectorSubtract(o->Position, o->StartPosition, vTemp);
					mu_float fTemp = VectorLength(vTemp);

					o->Rotation += 0.5f + GetLargeRand(2);

					if (fTemp <= 50.0f)
					{
						for (mu_int32 i = 0; i < 3; ++i)
						{
							o->Light[i] = o->TurningForce[i] * 0.1f * (500 - o->LifeTime);
							if (o->Light[i] > o->TurningForce[i])
							{
								o->Light[i] = o->TurningForce[i];
							}
						}
					}
					else
					{
						o->Light[0] /= 1.02f;
						o->Light[1] /= 1.02f;
						o->Light[2] /= 1.02f;

						if (o->Light[0] < 0.001f)
							oh->Flags.Clear(PARTICLE::ePF_LIVE);
					}

					if (fTemp > 500.0f)
					{
						oh->Flags.Clear(PARTICLE::ePF_LIVE);
					}
				}
				else if (o->SubType == 16)
				{
					if (o->LifeTime <= 10)
					{
						oh->Flags.Clear(PARTICLE::ePF_LIVE);
					}

					VectorAdd(o->Position, o->Velocity, o->Position);

					EVector3 vTemp;
					VectorSubtract(o->Position, o->StartPosition, vTemp);
					mu_float fTemp = VectorLength(vTemp);

					o->Rotation += 0.5f + GetLargeRand(2);

					if (fTemp <= 50.0f)
					{
						o->Light[0] += 0.01f;
						o->Light[1] += 0.01f;
						o->Light[2] += 0.01f;
						if (o->Light[0] >= 0.2f)
							Vector(0.2f, 0.2f, 0.2f, o->Light);
					}
					else
					{
						o->Light[0] /= 1.05f;
						o->Light[1] /= 1.05f;
						o->Light[2] /= 1.05f;

						if (o->Light[0] < 0.001f)
							oh->Flags.Clear(PARTICLE::ePF_LIVE);
					}

					if (fTemp >= 100.0f)
					{
						oh->Flags.Clear(PARTICLE::ePF_LIVE);
					}
				}
				else if (o->SubType == 17)
				{
					if (o->LifeTime <= 10)
					{
						oh->Flags.Clear(PARTICLE::ePF_LIVE);
					}
					VectorAdd(o->Position, o->Velocity, o->Position);

					EVector3 vTemp;
					VectorSubtract(o->Position, o->StartPosition, vTemp);
					mu_float fTemp = VectorLength(vTemp);

					o->Rotation += 0.08f + GetLargeRand(2) * 0.15f;

					if (o->LifeTime >= 400.0f)
					{
						for (mu_int32 i = 0; i < 3; ++i)
						{
							o->Light[i] = o->TurningForce[i] * 0.02f * (500 - o->LifeTime);
							if (o->Light[i] > o->TurningForce[i])
							{
								o->Light[i] = o->TurningForce[i];
							}
						}
					}
					else
					{
						o->Light[0] /= 1.02f;
						o->Light[1] /= 1.02f;
						o->Light[2] /= 1.02f;

						if (o->Light[0] < 0.015f)
							oh->Flags.Clear(PARTICLE::ePF_LIVE);
					}

					if (fTemp > 500.0f)
					{
						oh->Flags.Clear(PARTICLE::ePF_LIVE);
					}
				}
				else if (o->SubType == 18)
				{
					Luminosity = 1.0f;
					if (o->LifeTime > 90)
					{
						o->Light[0] += 0.002f;
						o->Light[1] += 0.002f;
						o->Light[2] += 0.0017f;
					}
					else if (o->LifeTime < 30)
					{
						o->Light[0] -= 0.004f;
						o->Light[1] -= 0.004f;
						o->Light[2] -= 0.0033f;
					}
					if (o->Scale > 0) o->Scale -= 0.001f;
					if (o->TurningForce[1] > 0) o->TurningForce[1] -= 0.5f;
					o->Position[0] = o->Target->Position[0] + o->StartPosition[0] + EMath::Cos((FWorldTime + o->Rotation)*o->TurningForce[0])*o->TurningForce[1];
					o->Position[1] = o->Target->Position[1] + o->StartPosition[1] + EMath::Sin((FWorldTime + o->Rotation)*o->TurningForce[0])*o->TurningForce[1];
					o->Position[2] += (GetLargeRand(10)) / 10.0f;

					if (o->LifeTime <= 0)
					{
						o->Target->HiddenMesh = 0;
						oh->Flags.Clear(PARTICLE::ePF_LIVE);
					}
				}
				else if (o->SubType == 19)
				{
					Luminosity = 1.0f;
					if (o->LifeTime > 30)
					{
						o->Light[0] += 0.01f;
						o->Light[1] += 0.01f;
						o->Light[2] += 0.01f;
						o->Scale += 0.01f;
					}
					else
					{
						o->Light[0] -= 0.01f;
						o->Light[1] -= 0.01f;
						o->Light[2] -= 0.01f;
					}

					o->Position[1] += 8.0f + (GetLargeRand(200))*0.1f;

					if (o->LifeTime <= 0)
					{
						o->Target->HiddenMesh = 0;
						oh->Flags.Clear(PARTICLE::ePF_LIVE);
					}
				}
				else if (o->SubType == 20)
				{
					if (o->LifeTime <= 0) {
						oh->Flags.Clear(PARTICLE::ePF_LIVE);
					}
					else if (o->LifeTime > 150) {
						if (o->Alpha < 1.0f)
							o->Alpha += 0.04;
						o->Position[2] += 1.0f;
					}
					else {
						if (o->Alpha > 0.1f)
							o->Alpha -= 0.025f;
						o->Position[2] -= 0.5f;
					}
				}
				else if (o->SubType == 21)
				{
					if (o->LifeTime <= 0) {
						oh->Flags.Clear(PARTICLE::ePF_LIVE);
					}
					else if (o->LifeTime > 50) {
						if (o->Alpha < 1.0f)
							o->Alpha += 0.04;
						o->Position[2] += 2.0f;
					}
					else {
						if (o->Alpha > 0.1f)
							o->Alpha -= 0.005f;
						o->Position[2] -= 1.0f;
					}
				}
				else if (o->SubType == 22)
				{
					if (o->LifeTime <= 0) {
						oh->Flags.Clear(PARTICLE::ePF_LIVE);
					}
					else if (o->LifeTime > 40)
					{
						if (o->Alpha < 1.0f)
							o->Alpha += 0.1f;
						o->Position[2] += 0.5f;
					}
					else {
						if (o->Alpha > 0.0f)
							o->Alpha -= 0.025f;
						else
							oh->Flags.Clear(PARTICLE::ePF_LIVE);
						o->Position[2] -= 0.5f;
					}

					o->Scale += 0.001f;
				}
				else if (o->SubType == 23)
				{
					if (o->LifeTime <= 0) {
						oh->Flags.Clear(PARTICLE::ePF_LIVE);
					}
					else if (o->LifeTime > 40)
					{
						if (o->Alpha < 1.0f)
							o->Alpha += 0.2f;
						//o->Position[2] += 0.5f;

						o->Scale += 0.003f;
					}
					else
					{
						o->Velocity[0] *= 0.7f;
						o->Velocity[1] *= 0.7f;

						if (o->Alpha > 0.0f)
							o->Alpha -= (GetLargeRand(10) * 0.01f);
						else
							oh->Flags.Clear(PARTICLE::ePF_LIVE);
						o->Position[2] += 0.2f;

						//o->Scale += 0.0005f;
					}
					o->Rotation += o->TurningForce[0] * 5.0f;;
				}
				else
				{
					Luminosity = 0.6f;
					if (o->Target->Flags.Get(OBJECT::eOF_VISIBLE) == true)
					{
						o->LifeTime = 50;
					}
					if (o->LifeTime <= 0)
					{
						o->Target->HiddenMesh = 0;
						oh->Flags.Clear(PARTICLE::ePF_LIVE);
					}

					o->Position[2] = o->StartPosition[2] + EMath::Sin((FWorldTime + o->Gravity) / 5000.0f)*20.0f;
				}
				switch (o->SubType)
				{
				case    1:
				case    4:
					o->Rotation = (FWorldTime*0.02f*o->TurningForce[0]) + o->StartPosition[1];
					break;

				case    2:
				case    5:
					o->Rotation = (FWorldTime*(-0.02f)*o->TurningForce[0]) + o->StartPosition[1];
					break;
				}
				break;

			case BITMAP_LIGHT:
				if (0 == o->SubType || 2 == o->SubType || o->SubType == 7 || o->SubType == 8)
				{
					mu_float fScale = (0 == o->SubType || o->SubType == 8) ? 0.5f : 4.0f;
					EVector3 vRandom;

					if (o->SubType == 7)
					{
						fScale = EMath::Sin(FWorldTime + o->LifeTime)*5.0f + 10.0f;
						o->Position[2] += fScale;
					}
					else
					{
						vRandom[0] = ((mu_float)(GetLargeRand(2001) - 1000)) * (0.001f * 0.4f);
						vRandom[1] = ((mu_float)(GetLargeRand(2001) - 1000)) * (0.001f * 0.4f);
						vRandom[2] = 0.0f;
						VectorAdd(vRandom, g_vParticleWind, vRandom);
						VectorScale(vRandom, fScale, vRandom);
						VectorAdd(o->Position, vRandom, o->Position);
						o->Position[2] += 5.0f * fScale;
					}
					if (0 == o->SubType || o->SubType == 8)
					{
						o->Scale -= 0.05f;
					}
					else if (o->SubType == 7)
					{
						//                        o->Scale = EMath::Sin ( o->Gravity+FWorldTime*0.001f )*0.2f+0.5f;
						o->Rotation += 30.0f;
					}
					else if (1 == o->SubType)
					{
						o->Scale *= 0.98f;
					}
					else if (5 == o->SubType)
					{
						o->Scale *= 0.95f;
					}
					else
					{
						o->Scale *= 0.95f;
					}
					if (o->SubType == 7)
					{
						if (o->LifeTime < 10)
						{
							o->Light[0] /= 1.2f;
							o->Light[1] /= 1.2f;
							o->Light[2] /= 1.2f;
						}
					}
					else
					{
						Vector(o->Scale, o->Scale, o->Scale, o->Light);
					}
					if (1 == o->SubType)
					{
						Vector(0.3f*o->Scale, 0.3f*o->Scale, 0.3f*o->Scale, o->Light);
					}
					if (5 == o->SubType)
					{
						Vector(0.3f*o->Scale, 0.3f*o->Scale, 0.3f*o->Scale, o->Light);
					}
					if (((0 == o->SubType || o->SubType == 8) && o->Scale <= 0.1f) || (o->SubType == 7 && o->Scale <= 0.1f) || (2 == o->SubType && o->Scale <= 0.3f))
					{
						o->LifeTime = -1;
						oh->Flags.Clear(PARTICLE::ePF_LIVE);
					}
					if (8 == o->SubType)
					{
						Vector(1.0f*o->Scale, 0.5f*o->Scale, 0.3f*o->Scale, o->Light);
					}
				}
				else if (o->SubType == 6)
				{
					mu_float fScale = 0.5f;
					EVector3 vRandom;
					vRandom[0] = ((mu_float)(GetLargeRand(2001) - 1000)) * (0.001f * 0.4f);
					vRandom[1] = ((mu_float)(GetLargeRand(2001) - 1000)) * (0.001f * 0.4f);
					vRandom[2] = 0.0f;
					VectorScale(vRandom, fScale, vRandom);
					VectorAdd(o->Position, vRandom, o->Position);
					o->Position[2] += 5.0f * fScale;
					o->Scale *= 0.95f;

					//                    Vector(o->Scale,o->Scale,o->Scale,o->Light);
					o->Light[0] *= 0.95f;
					o->Light[1] *= 0.95f;
					o->Light[2] *= 0.95f;
					if (o->Scale <= 0.1f)
					{
						o->LifeTime = -1;
						oh->Flags.Clear(PARTICLE::ePF_LIVE);
					}
				}
				else if (1 == o->SubType)
				{
					VectorScale(o->Light, 0.9f, o->Light);
					o->Scale *= 0.95f;
				}
				else if (5 == o->SubType)
				{
					VectorScale(o->Light, 0.9f, o->Light);
					o->Scale *= 0.95f;
				}
				else if (3 == o->SubType)
				{
					VectorScale(o->Light, 0.9f, o->Light);
					o->Scale *= 0.85f;
					o->Gravity += 5.0f;

					VectorCopy(o->Target->Position, o->Position);

					o->Position[0] += 2.0f;
					o->Position[1] -= 2.0f;
					o->Position[2] += o->Gravity;
				}
				else if (o->SubType == 4)   //  최대 생명력 증가 상태.
				{
					if (o->Target != nullptr &&
						o->Target->Type == MODEL_PLAYER &&
						o->Target->Kind == KIND_PLAYER &&
						o->Target->BoneTransform != nullptr)
					{
						OBJECT *Owner = o->Target;
						MUModel *b = &MODELS::Data[Owner->Type];

						b->TransformPosition(ThreadIndex, Owner->BoneTransform[b->GetBoneIndex((mu_int32)o->Rotation)], o->Angle, o->Position, false);

						VectorAdd(o->Position, Owner->Position, o->Position);
					}
					else
					{
						oh->Flags.Clear(PARTICLE::ePF_LIVE);
					}

					o->Gravity += (GetLargeRand(40) + 60) / 100.0f*9.5f;
					o->Scale -= (GetLargeRand(400) + 400) / 10000.0f;
					o->Position[2] += o->Gravity;

					o->Light[0] /= 1.35f;
					o->Light[1] /= 1.35f;
					o->Light[2] /= 1.35f;
				}
				else if (o->SubType == 9)
				{
					o->Scale -= 0.011f;
					if (!oh->Flags.Get(PARTICLE::ePF_LIVE))
						o->LifeTime = 0;
					if (o->Scale <= 0.0f)
						o->LifeTime = 0;

					if (o->LifeTime >= 75)
					{
						o->Light[0] *= 1.05f;
						o->Light[1] *= 1.05f;
						o->Light[2] *= 1.05f;
					}
					else
					{
						o->Light[0] /= 1.05f;
						o->Light[1] /= 1.05f;
						o->Light[2] /= 1.05f;
					}

				}
				else if (o->SubType == 10)
				{
					o->Rotation -= 10;
					if (o->Target != nullptr)
					{
						if (o->Target->CurrentAction != PLAYER_SANTA_2)
						{
							oh->Flags.Clear(PARTICLE::ePF_LIVE);
						}
					}
				}
				else if (o->SubType == 11)
				{
					o->Position[2] += o->Gravity;
					o->Scale -= 0.0005f;
				}
				else if (o->SubType == 12)
				{
					if (o->LifeTime > 80)
						o->Scale += 0.02f;
					else
						o->Scale -= 0.005f;
				}
				else if (o->SubType == 13)
				{
					o->Position[1] -= o->Gravity;
					o->Position[2] += o->Gravity;
					o->Scale -= 0.001f;
				}
				else if (o->SubType == 14)
				{
					VectorScale(o->Light, 0.98f, o->Light);
					o->Scale *= 0.95f;
				}
				else if (o->SubType == 15)
				{
					o->StartPosition[2] += (GetLargeRand(10) + 5) * 0.01f;
					o->Position[0] = o->StartPosition[0] + EMath::Sin(o->StartPosition[2]) * o->Gravity * 2;
					o->Position[1] = o->StartPosition[1] + EMath::Cos(o->StartPosition[2]) * o->Gravity * 2;
					o->Position[2] += o->Gravity;
					o->Scale -= 0.001f;
					if (o->LifeTime > 20)
					{
						o->Alpha += 0.1f;
						if (o->Alpha > 1.0f) o->Alpha = 1.0f;
					}
					else
					{
						o->Scale -= 0.01f;
						o->Alpha -= 0.1f;
						if (o->Alpha > 1.0f) o->Alpha = 1.0f;
					}
					o->Light[0] = o->TurningForce[0] * o->Alpha;
					o->Light[1] = o->TurningForce[1] * o->Alpha;
					o->Light[2] = o->TurningForce[2] * o->Alpha;
				}
				break;
			case BITMAP_POUNDING_BALL:  //  블러드 캐슬 자이언트 오거의 통통볼
				if (o->SubType == 0 || o->SubType == 1) // FIX Changed && to ||
				{
					o->Gravity += 0.004f;
					o->Scale -= 0.02f;

					o->Frame = (23 - o->LifeTime) / 6;
					o->Position[2] += o->Gravity*10.0f;

					if (o->SubType == 1)
					{
						o->Light[0] = o->LifeTime / 10.0f;
						o->Light[1] = o->Light[0];
						o->Light[2] = o->Light[0];
					}
				}
				else if (o->SubType == 2)
				{
					VectorCopy(o->Target->Angle, o->Angle);

					o->Scale -= 0.06f;
					o->Position[2] += o->Gravity*10.0f;
					o->Light[0] = o->LifeTime / 10.0f;
					o->Light[1] = o->Light[0];
					o->Light[2] = o->Light[0];
				}
				else if (o->SubType == 3)
				{
					if (o->LifeTime < 15)
					{
						o->Alpha -= 0.2f;
					}
					else if (o->Alpha < 1.0f)
					{
						o->Alpha += (GetLargeRand(2) + 2)*0.1f;
					}
					else
					{
						o->Alpha = 1.0f;
					}

					if (o->Alpha < 0.1f)
						oh->Flags.Clear(PARTICLE::ePF_LIVE);
					Luminosity = o->Alpha;
					Vector(o->TurningForce[0] * Luminosity, o->TurningForce[1] * Luminosity, o->TurningForce[2] * Luminosity, o->Light);
					if (o->Scale > 0)
					{
						o->Scale -= (GetLargeRand(3) + 5)*0.01f;
					}
					else
					{
						oh->Flags.Clear(PARTICLE::ePF_LIVE);
					}
					o->Position[2] += o->Gravity;
					o->Rotation += 3.0f;
				}
				break;
			case BITMAP_ADV_SMOKE:      //  블러드 캐슬의 불 연기.
				VectorAdd(o->Position, o->Velocity, o->Position);
				o->Velocity[0] *= 0.95f;
				o->Velocity[1] *= 0.95f;
				o->Light[0] = (mu_float)(o->LifeTime) / 10.0f;
				o->Light[1] = o->Light[0];
				o->Light[2] = o->Light[0];
				if (o->SubType == 0)
				{
					o->Velocity[2] += 0.3f;
					o->Scale += 0.07f;
				}
				else if (o->SubType == 2)
				{
					o->Velocity[2] += 0.3f;
					o->Scale += 0.07f;
				}
				else if (o->SubType == 3)
				{
					o->Velocity[1] += 0.1f;
					o->Scale += 0.05f;
					o->Alpha -= 0.2f;
				}
				else
				{
					o->Velocity[2] += 0.3f;
					o->Scale += 0.09f;
				}
				break;
			case BITMAP_ADV_SMOKE + 1:    //  블러드 캐슬의 불 연기.
				VectorAdd(o->Position, o->Velocity, o->Position);
				o->Velocity[0] *= 0.95f;
				o->Velocity[1] *= 0.95f;
				o->Velocity[2] += 0.6f;
				o->Position[0] += (mu_float)(GetLargeRand(4) - 2);
				o->Position[1] += (mu_float)(GetLargeRand(4) - 2);
				o->Position[2] += (mu_float)(GetLargeRand(4) - 2)*0.8f;
				o->Scale += 0.05f;
				if (o->SubType == 2)
				{
					//~-1.0f, ~1.0f사이 
					o->Light[0] = (mu_float)(o->LifeTime) / 25.0f * 2 - 1.0f;
					o->Light[1] = o->Light[0];
					o->Light[2] = o->Light[0];
				}
				else
				{
					o->Light[0] = (mu_float)(o->LifeTime) / 25.0f;
					o->Light[1] = o->Light[0];
					o->Light[2] = o->Light[0];
				}
				o->Rotation += 1.0f + GetLargeRand(2);
				break;

			case BITMAP_TRUE_FIRE:
			case BITMAP_TRUE_BLUE:
				if ((o->SubType == 1 || o->SubType == 2) && o->Target != nullptr)
				{
					if (o->Target->CurrentAction == 1)
						o->LifeTime -= 2;
				}

				if (o->SubType >= 3 && o->SubType < 5 && o->Target != nullptr)	//. 불골렘에 붙는 불..
				{
					switch (o->SubType)
					{
					case 3: BoneManager::GetBonePosition(o->Target, 34, TargetPosition); break;
					case 4: BoneManager::GetBonePosition(o->Target, 45, TargetPosition); break;
					}

					EVector3 Direction;
					VectorSubtract(o->Position, TargetPosition, Direction);

					mu_float length = VectorLength(Direction);
					//. 스케일 보정
					o->Scale -= (length * 0.003f);
					//. 라이트 보정
					o->Light[0] -= (length * 0.08f);
					o->Light[1] -= (length * 0.08f);
					o->Light[2] -= (length * 0.08f);
				}

				if (o->SubType == 8)
				{
					EVector3 vPos, vRelativePos;
					if (o->Target != nullptr)
					{
						MUModel* pModel = &MODELS::Data[o->Target->Type];
						VectorCopy(o->Target->Position, pModel->BodyOrigin(ThreadIndex));
						Vector(6.0f, 6.0f, 0.0f, vRelativePos);
						pModel->TransformPosition(ThreadIndex, o->Target->BoneTransform[pModel->GetBoneIndex(20)], vRelativePos, vPos, true);
					}

					o->Position[0] = vPos[0];
					o->Position[1] = vPos[1];
				}

				if (o->SubType == 7)
				{
					o->Scale -= 0.1f;
					o->Position[2] += 2.0f;
				}
				else
					o->Scale -= 0.02f;

				if (o->Scale < 0)
					o->Scale = 0.0f;

				o->Velocity[0] *= 0.95f;
				o->Velocity[1] *= 0.95f;

				if (o->Type == 6)
					o->Position[2] += 2.0f;
				else
					o->Position[2] += 1.0f;

				o->Light[0] = (mu_float)(o->LifeTime) / 25.0f;
				o->Light[1] = o->Light[0];
				o->Light[2] = o->Light[0];
				break;

			case BITMAP_HOLE:
				o->Rotation += 5.0f;
				if (o->LifeTime < 20)
				{
					o->Scale -= 0.08f;
					if (o->LifeTime < 10)
					{
						o->Light[0] /= 1.3f;
						o->Light[1] /= 1.3f;
						o->Light[2] /= 1.3f;
					}
				}
				else if (o->LifeTime < 30)
				{
					o->Light[0] *= 1.1f;
					o->Light[1] *= 1.1f;
					o->Light[2] *= 1.1f;
				}
				break;
			case BITMAP_WATERFALL_1:
				o->Scale -= 0.005f;
				if (o->LifeTime < 5)
				{
					o->Light[0] /= 1.2f;
					o->Light[1] /= 1.2f;
					o->Light[2] /= 1.2f;
				}
				else if (o->LifeTime > 20)
				{
					o->Light[0] *= 1.1f;
					o->Light[1] *= 1.1f;
					o->Light[2] *= 1.1f;
				}
				o->Velocity[2] += 0.1f;

				if (o->SubType == 1)
				{
					if (o->Light[0] > 0.5f)
					{
						Vector(0.5f, 0.5f, 0.5f, o->Light);
					}
					++o->Rotation;
					o->Scale += 0.01f;
					o->Velocity[2] -= 0.4f;
				}
				break;

			case BITMAP_WATERFALL_5:
				if (o->SubType == 0)
				{
					o->Scale -= 0.005f;
					o->Velocity[2] += 0.1f;
				}
				else if (o->SubType == 1)
				{
					o->Scale += 0.1f;
					o->Position[0] += GetLargeRand(10) - 5.0f;
					o->Position[1] += GetLargeRand(10) - 5.0f;
				}
				else if (o->SubType == 2)
				{
					if (o->Scale < 1.0f)
						o->Scale += 0.1f;
					o->Position[0] += GetLargeRand(10) - 5.0f;
					o->Position[1] += GetLargeRand(10) - 5.0f;
					o->Velocity[2] -= 1.0f;
				}
				else if (o->SubType == 3)
				{
					o->Scale -= 0.005f;
					o->Rotation += 4.0f;
					o->Velocity[2] += 0.1f;
				}
				else if (o->SubType == 4)
				{
					o->Position[0] += (mu_float)(EMath::Cos(o->Angle[2]) * 20.0f);
					o->Position[1] += (mu_float)(EMath::Sin(o->Angle[2]) * 20.0f);
					o->Position[2] += o->Gravity;
					o->Rotation += 1;
				}
				else if (o->SubType == 5)
				{
					o->Scale -= 0.005f;
					o->Velocity[2] += 0.1f;
				}
				else if (o->SubType == 7)
				{
					o->Scale -= 0.005f;
					o->Rotation += 1;
					o->Position[0] += o->Velocity[0];
					o->Position[1] += o->Velocity[1];
					o->Position[2] += o->Velocity[2];

					o->Light[0] /= 1.1f;
					o->Light[1] /= 1.1f;
					o->Light[2] /= 1.1f;
				}
				else if (o->SubType == 8)
				{
					o->Scale += 0.05f;
					o->Velocity[2] -= 0.6f;

					o->Light[0] /= 1.1f;
					o->Light[1] /= 1.1f;
					o->Light[2] /= 1.1f;
					break;
				}
				else if (o->SubType == 9)
				{
					o->Scale -= 0.005f;
					o->Velocity[2] += 0.1f;
				}

				if (o->LifeTime < 8)
				{
					o->Light[0] /= 1.2f;
					o->Light[1] /= 1.2f;
					o->Light[2] /= 1.2f;
				}
				else if (o->LifeTime > 20)
				{
					o->Light[0] *= 1.1f;
					o->Light[1] *= 1.1f;
					o->Light[2] *= 1.1f;
				}
				break;

			case BITMAP_PLUS:
				o->Scale -= 0.01f;
				o->Position[0] += GetLargeRand(2) - 1;
				o->Position[1] += GetLargeRand(2) - 1;
				o->Position[2] += 2.0f;

				o->Light[0] = o->LifeTime / 20.0f;
				o->Light[1] = o->LifeTime / 20.0f;
				o->Light[2] = o->LifeTime / 20.0f;
				break;

			case BITMAP_WATERFALL_2:
				if (o->SubType == 5)
				{
					o->Scale += 0.03f;
					o->Velocity[2] += 0.1f;

					o->Position[0] += o->Velocity[0];
					o->Position[1] += o->Velocity[1];
					o->Position[2] += o->Gravity + o->Velocity[2];

					o->Light[0] /= 1.1f;
					o->Light[1] /= 1.1f;
					o->Light[2] /= 1.1f;
					break;
				}

				o->Scale += 0.03f;
				o->Velocity[0] = (GetLargeRand(20) - 10)*0.1f;
				o->Velocity[1] = (GetLargeRand(20) - 10)*0.1f;
				o->Velocity[2] += 0.1f;
				if (o->LifeTime < 10)
				{
					o->Light[0] /= 1.1f;
					o->Light[1] /= 1.1f;
					o->Light[2] /= 1.1f;
				}

				if (o->SubType == 1)
				{
					o->Light[0] /= 1.05f;
					o->Light[1] /= 1.05f;
					o->Light[2] /= 1.05f;
				}
				if (o->SubType == 3)
				{
					o->Scale -= 0.038f;
					o->Velocity[2] -= 0.02f;
					o->Light[0] /= 1.02f;
					o->Light[1] /= 1.02f;
					o->Light[2] /= 1.02f;
				}
				if (o->SubType == 4)
				{
					o->Rotation -= 1.1f;
				}
				if (o->SubType == 6)
				{
					o->Gravity += (GetLargeRand(5) + 1.5f*(o->LifeTime / 5.0f));

					o->Velocity[0] += 1;

					o->Velocity[0] -= 100 * (20.0f / o->LifeTime);
					o->Rotation += 50.0f;
					AngleMatrix(o->Angle, Matrix);
					VectorRotate(o->Velocity, Matrix, TargetPosition);
					VectorAdd(o->StartPosition, TargetPosition, o->Position);
					o->Scale += (1.0 / o->LifeTime)*2.0f;

					o->Light[0] *= 0.8f;
					o->Light[1] *= 0.77f;
					o->Light[2] *= 0.77f;

					//	o->Gravity += 1.0f;
					VectorCopy(o->StartPosition, o->Position);
					o->StartPosition[2] = o->StartPosition[2] + GetLargeRand(5) + 10.0f;
					o->Position[2] = o->StartPosition[2] + o->Gravity;
				}
				break;

			case BITMAP_WATERFALL_3:
			case BITMAP_WATERFALL_4:
				if (o->SubType == 2)
				{
					o->Position[0] += (mu_float)(EMath::Cos(o->Angle[2]) * 20.0f);
					o->Position[1] += (mu_float)(EMath::Sin(o->Angle[2]) * 20.0f);
					o->Position[2] += o->Gravity;
					o->Rotation += 1;
					o->Light[0] /= 1.1f;
					o->Light[1] /= 1.1f;
					o->Light[2] /= 1.1f;
					break;
				}
				else if (o->SubType == 3)		//. 먼지불빛 박스 : 2004/11/09
				{
					o->Scale += 0.005f;
					o->Velocity[2] -= 0.05f;
					o->Light[0] *= 0.97f;
					o->Light[1] *= 0.97f;
					o->Light[2] *= 0.97f;
					break;
				}
				else if (o->SubType == 5)
				{
					o->Position[2] -= o->Gravity;
					o->Gravity -= 0.05f;
					//o->Scale    += 0.005f;
					//o->Velocity[2] -= 0.05f;

					o->Light[0] /= 1.1f;
					o->Light[1] /= 1.1f;
					o->Light[2] /= 1.1f;
					break;
				}
				else if (o->SubType == 6)
				{
					o->Position[2] += o->Gravity;
					o->Gravity += 0.05f;

					o->Alpha -= 0.01f;

					o->Light[0] /= 1.1f;
					o->Light[1] /= 1.1f;
					o->Light[2] /= 1.1f;
					break;
				}
				else if (o->SubType == 7)	//@@ MoveParticle
				{
					o->Velocity[2] += 0.01f;
					o->Light[0] *= 0.97f;
					o->Light[1] *= 0.97f;
					o->Light[2] *= 0.97f;
					break;
				}
				else if (o->SubType == 8)
				{
					o->Scale += 0.05f;
					o->Velocity[2] -= 0.6f;
					o->Light[0] /= 1.1f;
					o->Light[1] /= 1.1f;
					o->Light[2] /= 1.1f;
					break;
				}
				else if (o->SubType == 10)
				{
					o->Position[0] += o->Velocity[0];
					o->Position[1] += o->Velocity[1];
					o->Position[2] -= o->Gravity;
					o->Gravity -= 0.05f;
					o->Light[0] /= 1.1f;
					o->Light[1] /= 1.1f;
					o->Light[2] /= 1.1f;
					break;
				}
				else if (o->SubType == 11)
				{
					VectorAdd(o->Position, o->Velocity, o->Position);
					o->Scale += 0.01f;
					o->Light[0] /= 1.1f;
					o->Light[1] /= 1.1f;
					o->Light[2] /= 1.1f;
					break;
				}
				// 소드브레이커
				else if (o->SubType == 12)
				{
					o->Rotation += (IWorldTime % 360)*0.01f;
					o->Scale *= 0.92f;
					Vector(o->Light[0] *= 0.92f, o->Light[1] *= 0.92f, o->Light[2] *= 0.92f, o->Light);
					o->Alpha *= 0.8f;

					break;
				}
				else if (o->SubType == 13)
				{
					o->Position[2] -= o->Gravity;
					o->Scale += 0.001f;
					o->Light[0] /= 1.1f;
					o->Light[1] /= 1.1f;
					o->Light[2] /= 1.1f;

					break;
				}
				else if (o->SubType == 14)
				{
					o->Scale += 0.05f;
					o->Velocity[2] -= 0.6f;
					o->Light[0] /= 1.1f;
					o->Light[1] /= 1.1f;
					o->Light[2] /= 1.1f;
				}
				else if (o->SubType == 15)
				{
					o->Position[2] -= o->Gravity;
					o->Gravity -= 0.05f;
					o->Scale -= 0.05f;
					//o->Velocity[2] -= 0.05f;

					o->Light[0] /= 1.1f;
					o->Light[1] /= 1.1f;
					o->Light[2] /= 1.1f;
					break;
				}

				o->Scale += 0.005f;
				o->Velocity[2] -= 2.5f;
				o->Light[0] /= 1.1f;
				o->Light[1] /= 1.1f;
				o->Light[2] /= 1.1f;
				break;

			case BITMAP_SHOCK_WAVE:
				{
					if (o->SubType == 3)
					{
						o->Light[0] /= 1.8f;
						o->Light[1] /= 1.8f;
						o->Light[2] /= 1.8f;
						o->Scale += 0.3f;
					}
					else if (o->SubType == 0)
					{
						o->Light[0] /= 1.5f;
						o->Light[1] /= 1.5f;
						o->Light[2] /= 1.5f;
						o->Scale += 0.8f;
					}
					if (o->SubType == 4)
					{
						o->Alpha -= 0.001f;
						if (o->Alpha <= 0.0f) oh->Flags.Clear(PARTICLE::ePF_LIVE);
						o->Light[0] /= 1.01f;
						o->Light[1] /= 1.03f;
						o->Light[2] /= 1.03f;
						o->Scale += (o->Gravity * 0.015f);
						o->Gravity += 2.4f;
					}
				}
				break;
			case BITMAP_CURSEDTEMPLE_EFFECT_MASKER:
				{
					o->Light[0] /= 1.1f;
					o->Light[1] /= 1.1f;
					o->Light[2] /= 1.1f;

					o->Scale += 0.02f;

					if (o->SubType == 0)
					{
						EVector3 Light;
						Vector(0.8f, 0.3f, 0.3f, Light);
						if (GetLargeRand(2) == 1)
							CreateParticle(ThreadIndex, BITMAP_CURSEDTEMPLE_EFFECT_MASKER, o->StartPosition, o->Angle, Light, 1, 1.3f);
					}
				}
				break;
			case BITMAP_RAKLION_CLOUDS:
				{
					// A. 다음 위치 지정
					// 1. 방향 벡터를 y = 1인 단위 벡터 로 설정한뒤
					// 2. 오브텍트가 바라보는 방향으로 
					// 3. Velocity 값을 지정한뒤
					// 4. 2,3 요소를 곱해서 방향_크기벡터를 만들고,
					// 5. 기존 위치 에서 변위위치를 더해준다.
					// B. 다음 알파 값 지정 
					// C. 다음 크기 지정

					// A. 다음 위치 지정
					const	mu_float FORCESCALAR = 13.0f;
					const	mu_float SCALEFACTOR = 0.065f;
					const	mu_float LIGHTDIVIDEDFACTOR = 1.6f;
					const	mu_float EPSILON_ = 0.01f;

					EVector3	vForceVec_, vForceVec;
					mu_float	fForceScalar;	fForceScalar = FORCESCALAR;

					EMatrix4 MatRotation;
					vForceVec_[0] = 0.0f; vForceVec_[1] = -1.0f; vForceVec_[2] = 0.0f;

					AngleMatrix(o->Angle, MatRotation);
					VectorRotate(vForceVec_, MatRotation, vForceVec);
					vForceVec[0] = vForceVec[0] * fForceScalar;
					vForceVec[1] = vForceVec[1] * fForceScalar;
					vForceVec[2] = vForceVec[2] * fForceScalar;

					VectorAdd(o->Position, vForceVec, o->Position);
					// B. 다음 크기 지정
					o->Scale = o->Scale + SCALEFACTOR;

					// C. ALPHA 감산 효과
					o->Light[0] = o->Light[0] / LIGHTDIVIDEDFACTOR;
					o->Light[1] = o->Light[1] / LIGHTDIVIDEDFACTOR;
					o->Light[2] = o->Light[2] / LIGHTDIVIDEDFACTOR;
					//o->Alpha	= o->Alpha / LIGHTDIVIDEDFACTOR;

					if (o->Light[0] + o->Light[1] + o->Light[2] <= EPSILON_)
					{
						oh->Flags.Clear(PARTICLE::ePF_LIVE);
					}

					// ShinyStar 드문드문 하게 하나씩 반짝.
					if (GetLargeRand(10) == 0)
					{
						// ShinyStar 효과 부분 
						const	mu_float BASERANGE_SHINYSTAR = 70.0f * o->Scale;
						mu_float	fRand1, fRand2, fRand3;
						EVector3	v3Pos_ShinyStar, v3OffsetPos_ShinyStar;

						fRand1 = (mu_float)((GetLargeRand(2000)) - 1000) * 0.001f;
						fRand2 = (mu_float)((GetLargeRand(2000)) - 1000) * 0.001f;
						fRand3 = (mu_float)((GetLargeRand(2000)) - 1000) * 0.001f;

						v3OffsetPos_ShinyStar[0] = BASERANGE_SHINYSTAR * fRand1;
						v3OffsetPos_ShinyStar[1] = BASERANGE_SHINYSTAR * fRand2;
						v3OffsetPos_ShinyStar[2] = BASERANGE_SHINYSTAR * fRand3;

						VectorAdd(o->Position, v3OffsetPos_ShinyStar, v3Pos_ShinyStar);
						CreateParticle(ThreadIndex, BITMAP_SHINY + 6, v3Pos_ShinyStar, o->Angle, o->Light, 0, 0.5f);
					}
				}
				break;
			case BITMAP_CHROME2:
				{
					if (o->Scale > 0) o->Scale -= 0.1f;
					else o->Scale = 0;

					o->Rotation += 1.0f;
					o->Alpha -= 0.05f;
					if (o->Target != nullptr)
					{
						o->Position[0] = o->Target->Position[0] - o->StartPosition[0];
						o->Position[1] = o->Target->Position[1] - o->StartPosition[1];
						o->Position[2] = o->Target->Position[2] - o->StartPosition[2];
					}
				}
				break;
			case BITMAP_AG_ADDITION_EFFECT:
				{
					if (o->LifeTime < 10)
						o->Alpha -= 0.1f;
					else
						o->Alpha = 0.7f;

					if (o->Alpha < 0.1f)
						oh->Flags.Clear(PARTICLE::ePF_LIVE);

					Luminosity = o->Alpha;
					Vector(o->TurningForce[0] * Luminosity, o->TurningForce[1] * Luminosity, o->TurningForce[2] * Luminosity, o->Light);
					EVector3 Temp_Pos;

					MUModel* b = &MODELS::Data[o->Target->Type];
					b->TransformByObjectBone(ThreadIndex, Temp_Pos, o->Target, 18);

					if (o->SubType == 0)
					{
						o->Scale *= 1.05f;
						Temp_Pos[2] -= 10.0f;
					}
					else if (o->SubType == 1)
					{
						o->Scale *= 1.02f;
						Temp_Pos[2] += 10.0f;
					}
					else if (o->SubType == 2)
					{
						o->Scale *= 1.03f;
						Temp_Pos[2] += 25.0f;
					}
					VectorCopy(Temp_Pos, o->Position);

					if (!oh->Flags.Get(PARTICLE::ePF_LIVE) && g_isCharacterBuff(o->Target, eBuff_AG_Addition))
					{
						if (o->SubType == 0)
							CreateParticle(ThreadIndex, BITMAP_AG_ADDITION_EFFECT, Temp_Pos, o->Angle, o->Light, 0, 1.0f, o->Target);
						else if (o->SubType == 1)
							CreateParticle(ThreadIndex, BITMAP_AG_ADDITION_EFFECT, Temp_Pos, o->Angle, o->Light, 1, 1.0f, o->Target);
						else if (o->SubType == 2)
							CreateParticle(ThreadIndex, BITMAP_AG_ADDITION_EFFECT, Temp_Pos, o->Angle, o->Light, 2, 1.0f, o->Target);
					}
				}
				break;
			}

			if (o->LifeTime <= 0 ||
				oh->Flags.Get(PARTICLE::ePF_LIVE) == false)
			{
				g_ParticlesDeleteCount.fetch_add(1);

				oh->Flags.Clear(PARTICLE::ePF_LIVE);
			}
		}

		q -= queueCount;
	}
}

void MUTasking::CheckParticles(const mu_uint32 ThreadIndex)
{
	mu_uint32 beginIndex, endIndex;
	GetIndexTasking(ThreadIndex, MAX_PARTICLES, beginIndex, endIndex);

	for (mu_uint32 i = beginIndex; i < endIndex; ++i)
	{
		const PARTICLE_HEADER *oh = &g_ParticlesHeader[i];

		if (oh->Flags.Get(PARTICLE::ePF_LIVE) == false)
		{
			continue;
		}

		const PARTICLE *o = &g_Particles[i];

		switch (o->Type)
		{
		case BITMAP_FLARE + 1:
			{
				if (o->SubType != 0)
				{
					continue;
				}
			}
			break;

		case BITMAP_FLARE:
			{
				if (o->SubType != 11 &&
					o->SubType != 4 &&
					o->LifeTime == 60)
				{
					continue;
				}
			}
			break;

		case BITMAP_SPARK + 2:
			{
				if (o->SubType != 0 &&
					o->SubType != 2 &&
					o->SubType != 3)
				{
					continue;
				}
			}
			break;

		case BITMAP_FLARE_BLUE:
			{
				if (o->SubType != 0 &&
					o->SubType != 1)
				{
					continue;
				}
			}
			break;
		}

		EEffectCulledData &instanceData = g_EffectsCulledData[g_EffectsCulledDataCount.fetch_add(1)];
		instanceData.Type = ECastEnum(EEffectProxyEnum::eParticles);
		instanceData.Index = i;
	}
}