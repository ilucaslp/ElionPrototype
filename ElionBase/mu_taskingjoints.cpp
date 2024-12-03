#include "stdafx.h"
#include "mu_tasking.h"

void MUTasking::CheckMoveJoints(const mu_uint32 ThreadIndex)
{
	mu_uint32 queueCount = 0;
	const mu_uint32 queueIndex = MUThreadQueues::JointsQueueIndex[ThreadIndex];

	mu_uint32 beginIndex, endIndex;
	GetIndexTasking(ThreadIndex, MAX_JOINTS, beginIndex, endIndex);

#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	mu_uint32 i = beginIndex;

#define RETRIEVE_HEADERFLAG(n) g_JointsHeader[i + n].Flags.GetFlags()
#if defined(_MSC_VER)
	if(cpuinfo_has_x86_avx2())
	{
#define CHECK_FLAG(n) if((value & (1 << n)) != 0) MUThreadQueues::JointsQueue[queueIndex + queueCount++] = i + n;
		const __m256i AVXFlags = _mm256_set1_epi8(JOINT::eJF_LIVE);
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

		const __m128i SSE2Flags = _mm_set1_epi8(JOINT::eJF_LIVE);
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
#define CHECK_FLAG(n) if((value & (1 << n)) != 0) MUThreadQueues::JointsQueue[queueIndex + queueCount++] = i + n;
		const __m128i SSE2Flags = _mm_set1_epi8(JOINT::eJF_LIVE);
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
		const JOINT_HEADER *oh = &g_JointsHeader[i];

		if (oh->Flags.Get(JOINT::eJF_LIVE) == true)
		{
			MUThreadQueues::JointsQueue[queueIndex + queueCount++] = i;
		}
	}
#elif CPUINFO_ARCH_ARM || CPUINFO_ARCH_ARM64
	mu_uint32 i = beginIndex;

#define RETRIEVE_HEADERFLAG(n) g_JointsHeader[i + n].Flags.GetFlags()
#define CHECK_FLAG(n) if((value & (1 << n)) != 0) MUThreadQueues::JointsQueue[queueIndex + queueCount++] = i + n;
	const uint8x16_t NEONFlags = vdupq_n_u8(JOINT::eJF_LIVE);
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
		const JOINT_HEADER *oh = &g_JointsHeader[i];

		if (oh->Flags.Get(JOINT::eJF_LIVE) == true)
		{
			MUThreadQueues::JointsQueue[queueIndex + queueCount++] = i;
		}
	}
#else
	for (mu_uint32 i = beginIndex; i < endIndex; ++i)
	{
		const JOINT_HEADER *oh = &g_JointsHeader[i];

		if (oh->Flags.Get(JOINT::eJF_LIVE) == true)
		{
			MUThreadQueues::JointsQueue[queueIndex + queueCount++] = i;
		}
	}
#endif

	MUThreadQueues::JointsQueueCount[ThreadIndex] = queueCount;
}

void MUTasking::MoveJoints(const mu_uint32 ThreadIndex)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	mu_uint32 q = ThreadIndex;
	for (mu_uint32 t = 0; t < _ThreadsCount; ++t)
	{
		const mu_uint32 queueCount = MUThreadQueues::JointsQueueCount[t];
		const mu_uint16 *queue = MUThreadQueues::JointsQueue.data() + MUThreadQueues::JointsQueueIndex[t];

		for (; q < queueCount; q += _ThreadsCount)
		{
			const mu_uint32 i = queue[q];

			JOINT_HEADER *oh = &g_JointsHeader[i];
			JOINT *o = &g_Joints[i];

REPEAT_MOVE:
			mu_float Height;
			EVector3 Light;
			mu_float Luminosity;
			EVector3 Position, p;
			mu_float Distance;
			mu_float dx = o->Position[0] - o->TargetPosition[0];
			mu_float dy = o->Position[1] - o->TargetPosition[1];

			EBone Matrix;

			if (!(o->Type == BITMAP_JOINT_FORCE && o->SubType == 0) && !(o->Type == BITMAP_JOINT_THUNDER && o->SubType == 15) && o->Type != BITMAP_LIGHT
				&& o->Type != BITMAP_PIERCING && o->Type != BITMAP_FLARE_FORCE
				&& !((o->Type == BITMAP_FLARE || o->Type == BITMAP_FLARE_BLUE) && o->SubType == 1)
				&& !(o->Type == MODEL_SPEARSKILL && (o->SubType == 5 || o->SubType == 6 || o->SubType == 7 || o->SubType == 8))
				&& !((o->Type == BITMAP_FLARE) && (o->SubType == 42))
				&& !((o->Type == BITMAP_FLASH) && (o->SubType == 6))
				)
			{
				AngleMatrix(o->Angle, Matrix);
				if (o->Velocity != 0.0f)
				{
					Vector(0.0f, -o->Velocity, 0.0f, p);
					VectorRotate(p, Matrix, Position);
					VectorAdd(o->Position, Position, o->Position);
				}
			}

			switch (o->Type)
			{
			case BITMAP_JOINT_LASER:
				VectorCopy(o->Target->Position, o->TargetPosition);
				o->TargetPosition[2] += 130.0f;

				Distance = EMath::Sqrt(dx*dx + dy * dy);
				MoveHumming(o->Position, o->Angle, o->TargetPosition, 3000.0f / Distance);
				//MoveHumming(o->Position,o->Angle,o->TargetPosition,10.0f);
				//MoveHumming(o->Position,o->Angle,o->TargetPosition,0.0f);
				if (!o->Features.Get(JOINT::eJFS_COLLISION) && Distance <= o->Velocity*2.0f)
				{
					o->Features.Set(JOINT::eJFS_COLLISION);
					o->LifeTime = 5;
				}
				//light
				Luminosity = o->LifeTime*0.1f;
				Vector(Luminosity, Luminosity, Luminosity, o->Light);
				Luminosity = -(mu_float)(GetLargeRand(4) + 4)*0.01f;
				Vector(Luminosity, Luminosity, Luminosity, Light);
				AddTerrainLight(o->Position[0], o->Position[1], Light, 4, PrimaryTerrainLight);
				break;
			case BITMAP_SCOLPION_TAIL:
				VectorCopy(o->Target->EyeLeft, o->Position);
				if (!o->Target->Flags.Get(OBJECT::eOF_LIVE))
				{
					o->LifeTime = 1;
					break;
				}
				break;
			case BITMAP_JOINT_ENERGY:
				switch (o->SubType)
				{
				case 2:
				case 3:
				case 4:
				case 5:
				case 8:
				case 10:
				case 11:
				case 14:
				case 15:
				case 17:
				case 18:
				case 19:
				case 20:
				case 21:
				case 26:
				case 27:
				case 28:
				case 29:
				case 30:
				case 31:
				case 32:
				case 33:
				case 22:
				case 23:
				case 24:
				case 25:
				case 47:
				case 48:
				case 49:
				case 50:
				case 51:
				case 52:
				case 53:
				case 54:
				case 55:
				case 56:
				case 57:

					if (!o->Target->Flags.Get(OBJECT::eOF_LIVE))
					{
						o->LifeTime = 1;
						break;
					}
					switch (o->SubType)
					{
					case 2:
					case 4:
					case 5:
					case 8:
					case 10:
					case 14:
					case 18:
					case 28:
					case 22:
					case 24:
						VectorCopy(o->Target->EyeLeft, o->Position);
						if (o->SubType == 8)
						{
							o->Scale += 10.1f;
						}
						break;
					case 17:
						VectorCopy(o->Target->EyeLeft2, o->Position);
						if (o->SubType == 8)
						{
							o->Scale += 10.1f;
						}
						break;
					case 20:
					case 30:
						VectorCopy(o->Target->EyeLeft2, o->Position);
						break;
					case 26:
					case 32:
						VectorCopy(o->Target->EyeLeft3, o->Position);
						break;
					case 3:
					case 11:
					case 15:
					case 19:
					case 29:
					case 23:
					case 25:
					case 47:
						VectorCopy(o->Target->EyeRight, o->Position);
						break;
					case 21:
					case 31:
						VectorCopy(o->Target->EyeRight2, o->Position);
						break;
					case 54:
						switch (o->PKKey)
						{
						case 0:
							VectorCopy(o->Target->EyeRight2, o->Position);
							break;
						case 1:
							VectorCopy(o->Target->EyeLeft2, o->Position);
							break;
						case 2:
							VectorCopy(o->Target->EyeRight3, o->Position);
							break;
						case 3:
							VectorCopy(o->Target->EyeLeft3, o->Position);
							break;
						}
						break;
					case 27:
					case 33:
						VectorCopy(o->Target->EyeRight3, o->Position);
						break;
					case 55:
						VectorCopy(o->Target->EyeLeft, o->Position);
						break;
					case 56:
						VectorCopy(o->Target->EyeRight, o->Position);
						break;
					case 57:
						MODELS::Data[o->Target->Type].Animation(ThreadIndex, ThreadInfo.BoneTransform, o->Target->AnimationFrame,
							o->Target->PriorAnimationFrame, o->Target->PriorAction, o->Target->Angle,
							o->Target->HeadAngle, false, false);
						MODELS::Data[o->Target->Type].TransformByObjectBone(ThreadIndex, o->Position, o->Target, o->PKKey);
						break;
					case 48:
						MODELS::Data[o->Target->Type].Animation(ThreadIndex, ThreadInfo.BoneTransform, o->Target->AnimationFrame,
							o->Target->PriorAnimationFrame, o->Target->PriorAction, o->Target->Angle,
							o->Target->HeadAngle, false, false);
						MODELS::Data[o->Target->Type].TransformByObjectBone(ThreadIndex, o->Position, o->Target, 24);
						break;
					case 49:
						MODELS::Data[o->Target->Type].Animation(ThreadIndex, ThreadInfo.BoneTransform, o->Target->AnimationFrame,
							o->Target->PriorAnimationFrame, o->Target->PriorAction, o->Target->Angle,
							o->Target->HeadAngle, false, false);
						MODELS::Data[o->Target->Type].TransformByObjectBone(ThreadIndex, o->Position, o->Target, 28);
						break;
					case 50:
						MODELS::Data[o->Target->Type].Animation(ThreadIndex, ThreadInfo.BoneTransform, o->Target->AnimationFrame,
							o->Target->PriorAnimationFrame, o->Target->PriorAction, o->Target->Angle,
							o->Target->HeadAngle, false, false);
						MODELS::Data[o->Target->Type].TransformByObjectBone(ThreadIndex, o->Position, o->Target, 32);
						break;
					case 51:
						MODELS::Data[o->Target->Type].Animation(ThreadIndex, ThreadInfo.BoneTransform, o->Target->AnimationFrame,
							o->Target->PriorAnimationFrame, o->Target->PriorAction, o->Target->Angle,
							o->Target->HeadAngle, false, false);
						MODELS::Data[o->Target->Type].TransformByObjectBone(ThreadIndex, o->Position, o->Target, 44);
						break;
					case 52:
						MODELS::Data[o->Target->Type].Animation(ThreadIndex, ThreadInfo.BoneTransform, o->Target->AnimationFrame,
							o->Target->PriorAnimationFrame, o->Target->PriorAction, o->Target->Angle,
							o->Target->HeadAngle, false, false);
						MODELS::Data[o->Target->Type].TransformByObjectBone(ThreadIndex, o->Position, o->Target, 48);
						break;
					case 53:
						MODELS::Data[o->Target->Type].Animation(ThreadIndex, ThreadInfo.BoneTransform, o->Target->AnimationFrame,
							o->Target->PriorAnimationFrame, o->Target->PriorAction, o->Target->Angle,
							o->Target->HeadAngle, false, false);
						MODELS::Data[o->Target->Type].TransformByObjectBone(ThreadIndex, o->Position, o->Target, 52);
						break;
					}
					switch (o->SubType)
					{
					case 2:
					case 3:
						Luminosity = EMath::Sin(FWorldTime*0.002f)*0.3f + 0.8f;
						Vector(Luminosity*0.5f, Luminosity*0.1f, Luminosity, Light);
						VectorMul(Light, o->Light, Light);
						CreateParticle(ThreadIndex, BITMAP_LIGHTNING + 1, o->Position, o->Angle, Light);
						break;
					case 14:
					case 15:
						Luminosity = EMath::Sin(FWorldTime*0.002f)*0.3f + 0.8f;
						Vector(Luminosity, Luminosity*0.1f, Luminosity*0.1f, Light);
						CreateParticle(ThreadIndex, BITMAP_LIGHTNING + 1, o->Position, o->Angle, Light, 4);
						break;
					case 22:
					case 23:
						Luminosity = EMath::Sin(FWorldTime*0.002f)*0.2f + 0.8f;
						Vector(Luminosity, Luminosity, Luminosity, Light);
						VectorMul(Light, o->Light, Light);
						CreateParticle(ThreadIndex, BITMAP_LIGHTNING + 1, o->Position, o->Angle, Light, 4);
						break;
					case 24:
					case 25:
						Luminosity = EMath::Sin(FWorldTime*0.002f)*0.1f + 0.2f;
						Vector(Luminosity, Luminosity, Luminosity, Light);
						VectorMul(Light, o->Light, Light);
						CreateSprite(BITMAP_FLARE + 1, o->Position, Luminosity, Light, o->Target);
						break;
					case 54:
						Luminosity = EMath::Sin(FWorldTime*0.002f)*0.1f;
						Vector(Luminosity, Luminosity, Luminosity, Light);
						VectorMul(Light, o->Light, Light);
						CreateSprite(BITMAP_BLUR, o->Position, Luminosity, Light, o->Target);
						break;
					}
					break;
				case 0:
				case 1:
				case 6:
				case 9:
				case 12:
				case 13:
				case 16:
				case 44:
				case 45:
				case 46:
					{
						if (o->LifeTime >= 100 && o->SubType != 44)
						{
							if (o->SubType == 0)
							{
								o->Angle[2] += 10.0f;
								o->Position[2] += 6.0f;
							}
							else if (o->SubType == 45)
							{
								o->Angle[2] += 10.0f;
								o->Position[2] += 6.0f;
								//VectorCopy(o->Target->Position, o->TargetPosition);
								//o->TargetPosition[2] += 120.0f;
								//Distance = MoveHumming(o->Position, o->Angle, o->TargetPosition, o->Velocity);
							}
							else if (o->SubType == 12)
							{
								o->Angle[2] += 15.0f;
								o->Position[0] += o->MultiUse * GetLargeRand(5);
								o->Position[1] += o->MultiUse * GetLargeRand(5);
								o->Position[2] += 6.0f;
							}
							else if (o->SubType == 16
								|| o->SubType == 46
								)
							{
								o->Angle[2] -= o->MultiUse * 20.0f;
								o->Position[0] += o->MultiUse * (GetLargeRand(5) + 3);
								o->Position[1] += o->MultiUse * (GetLargeRand(5) + 3);
								o->Position[2] += 8.0f;
							}
							else if (o->SubType == 13)
							{
								o->Angle[2] -= 20.0f;
								o->Position[0] += o->MultiUse * GetLargeRand(5);
								o->Position[1] += o->MultiUse * GetLargeRand(5);
								o->Position[2] += 6.0f;
							}
							else if (o->SubType == 44)
							{
								o->Angle[2] -= 20.0f;
								o->Position[0] += o->MultiUse * GetLargeRand(5) + EMath::Cos(Q_PI / (mu_float)(GetLargeRand(180)) * 20.0f);
								o->Position[1] += o->MultiUse * GetLargeRand(5) + EMath::Sin(Q_PI / (mu_float)(GetLargeRand(180)) * 20.0f);
								o->Position[2] += 6.0f;
							}
							else
							{
								o->Angle[2] -= 10.0f;
								o->Position[2] += 5.0f;
							}
						}
						else if (o->SubType == 44 && o->LifeTime <= 120 && o->LifeTime >= 10)
						{
							if (o->LifeTime >= 20)
							{
								o->Velocity += 10.0f;
								if (o->Velocity >= 20.0f) o->Velocity = 20.0f;

								VectorCopy(o->Target->Position, o->TargetPosition);
								o->TargetPosition[2] += 120.0f;
								Distance = MoveHumming(o->Position, o->Angle, o->TargetPosition, o->Velocity);
							}
							else
							{
								o->Velocity += 12.0f;
								if (o->Velocity >= 20.0f) o->Velocity = 20.0f;

								o->Position[0] += EMath::Cos(Q_PI / (mu_float)(GetLargeRand(180) + 150));
								o->Position[1] += EMath::Sin(Q_PI / (mu_float)(GetLargeRand(180) + 150));

								Distance = MoveHumming(o->Position, o->Angle, o->TargetPosition, o->Velocity);
							}
						}
						else
						{
							if (o->SubType == 12 || o->SubType == 13 || o->SubType == 16
								|| o->SubType == 46
								)
							{
								o->Velocity += 7.0f;
								if (o->Velocity >= 40.0f)
									o->Velocity = 40.0f;
							}
							else
							{
								o->Velocity += 5.0f;
								if (o->Velocity >= 30.0f)
									o->Velocity = 30.0f;
							}

							mu_float fOldAngle = o->Angle[2];
							if (o->SubType == 6 || o->SubType == 9)
							{
								VectorCopy(o->TargetPosition, Position);
								Distance = MoveHumming(o->Position, o->Angle, Position, o->Velocity);
							}
							else if (o->SubType == 7)
							{
								o->Position[2] += o->Velocity;
								Distance = MoveHumming(o->Position, o->Angle, o->TargetPosition, o->Velocity);
							}
							else if (o->SubType == 44)
							{
								VectorCopy(o->Target->Position, o->TargetPosition);
								o->TargetPosition[2] += 120.0f;
								Distance = MoveHumming(o->Position, o->Angle, o->TargetPosition, o->Velocity);
							}
							else
							{
								VectorCopy(o->Target->Position, o->TargetPosition);
								o->TargetPosition[2] += 120.0f;
								Distance = MoveHumming(o->Position, o->Angle, o->TargetPosition, o->Velocity);
							}

							if (Distance <= 35.0f)
							{
								o->LifeTime = 1;
								CreateParticle(ThreadIndex, BITMAP_LIGHTNING + 1, o->Position, o->Angle, o->Light, 1);
								PlayBackground(SOUND_GET_ENERGY);
							}
							else if (Distance <= 70.0f && EMath::Abs(fOldAngle - o->Angle[2]) > 20.0f)
							{
								if (o->Velocity >= 20.0f)
									o->Velocity -= 10.0f;
							}
						}
						Luminosity = (mu_float)(GetLargeRand(4) + 8)*0.03f;
						Vector(Luminosity*0.4f, Luminosity, Luminosity*0.8f, Light);
						AddTerrainLight(o->Position[0], o->Position[1], Light, 2, PrimaryTerrainLight);

						if (o->SubType == 6 || o->SubType == 9)
						{
							CreateParticle(ThreadIndex, BITMAP_LIGHTNING + 1, o->Position, o->Angle, o->Light, 3, 0.05f);
						}
						else if (o->SubType == 12 || o->SubType == 13)
						{
							CreateSprite(BITMAP_LIGHT, o->Position, 1.0f, o->Light, nullptr);
							CreateSprite(BITMAP_SHINY + 1, o->Position, 1.0f, o->Light, nullptr, (mu_float)(GetLargeRand(360)));
						}
						else if (o->SubType == 44)
						{
							CreateSprite(BITMAP_LIGHT, o->Position, 0.2f, o->Light, nullptr);
							CreateSprite(BITMAP_SHINY + 1, o->Position, 0.1f, o->Light, nullptr, (mu_float)(GetLargeRand(360)));
						}
						else if (o->SubType == 16)
						{
							Vector(0.4f, 0.8f, 1.0f, Light);
							CreateParticle(ThreadIndex, BITMAP_SMOKE, o->Position, o->Angle, Light, 31, 0.8f);
						}
						else if (o->SubType == 46)
						{
							Vector(0.4f, 1.0f, 0.4f, Light);
							CreateParticle(ThreadIndex, BITMAP_SMOKE, o->Position, o->Angle, Light, 31, 0.8f);
						}
						else if (o->SubType == 45)
						{
							Vector(1.0f, 1.0f, 1.0f, Light);
							CreateSprite(BITMAP_FLARE_RED, o->Position, 0.3f, Light, o->Target);
						}
						else
						{
							CreateParticle(ThreadIndex, BITMAP_LIGHTNING + 1, o->Position, o->Angle, o->Light);
						}
					}
					break;
				case 42:
					{
						if (o->LifeTime >= 60)
						{
							o->Position[2] += 1.0f;
							o->Light[0] += 0.4f * 0.005f;
							o->Light[1] += 0.3f * 0.005f;
							o->Light[2] += 2.2f * 0.005f;
						}
						else
						{
							Vector(0.4f, 0.3f, 2.2f, o->Light);
							o->Velocity += 5.0f;
							if (o->Velocity >= 30.0f)
							{
								o->Velocity = 30.0f;
							}
						}

						mu_float fOldAngle = o->Angle[2];
						VectorCopy(o->Target->Position, o->TargetPosition);
						o->TargetPosition[2] += 260.0f;
						Distance = MoveHumming(o->Position, o->Angle, o->TargetPosition, o->Velocity);

						if (Distance <= 35.0f)
						{
							o->LifeTime = 1;
							PlayBackground(SOUND_GET_ENERGY);
						}
						else if (Distance <= 70.0f && EMath::Abs(fOldAngle - o->Angle[2]) > 20.0f)
						{
							if (o->Velocity >= 20.0f)
								o->Velocity -= 10.0f;
						}

						Luminosity = (mu_float)(GetLargeRand(4) + 8)*0.03f;
						Vector(Luminosity*0.4f, Luminosity, Luminosity*0.8f, Light);
						AddTerrainLight(o->Position[0], o->Position[1], Light, 2, PrimaryTerrainLight);

						CreateParticle(ThreadIndex, BITMAP_LIGHTNING + 1, o->Position, o->Angle, o->Light, 5, 0.12f);
					}
					break;
				case 43:
					{
						o->Velocity += 5.0f;
						if (o->Velocity >= 40.0f)
						{
							o->Velocity = 40.0f;
						}

						VectorCopy(o->Target->Position, o->TargetPosition);
						o->TargetPosition[2] += 100.0f;
						mu_float TargetAngle;
						TargetAngle = CreateAngle(o->Position[0], o->Position[1], o->TargetPosition[0], o->TargetPosition[1]);
						o->Angle[2] = TargetAngle;
						Distance = MoveHumming(o->Position, o->Angle, o->TargetPosition, o->Velocity);

						if (Distance <= 60.0f)
						{
							o->LifeTime = 1;
							EVector3 vPos;
							VectorCopy(o->Position, vPos);
							vPos[2] -= 60.0f;
							CreateBomb(ThreadIndex, vPos, true, 4);
						}
						else if (Distance >= 550.0f)
						{
							o->LifeTime = 1;
						}

						Luminosity = (mu_float)(GetLargeRand(4) + 8)*0.03f;
						Vector(Luminosity*1.0f, Luminosity*0.4, Luminosity*0.3f, Light);
						AddTerrainLight(o->Position[0], o->Position[1], Light, 1, PrimaryTerrainLight);

						mu_float fLumi;
						EVector3 vPos, vLight;
						VectorCopy(o->Position, vPos);
						fLumi = (EMath::Sin(FWorldTime*0.002f) + 1.0f) * 0.1f;
						Vector(2.0f + fLumi, 1.0f + fLumi, 1.0f + fLumi, vLight);
						CreateSprite(BITMAP_POUNDING_BALL, vPos, 0.7f + fLumi, vLight, nullptr, (FWorldTime / 10.0f));
						fLumi = (EMath::Sin(FWorldTime*0.002f) + 1.0f) * 1.0f;
						Vector(2.0f + (GetLargeRand(10))*0.03f, 0.4f + (GetLargeRand(10))*0.03f, 0.4f + (GetLargeRand(10))*0.03f, vLight);
						CreateSprite(BITMAP_LIGHT, vPos, 2.0f, vLight, nullptr, -(FWorldTime*0.1f));
						CreateSprite(BITMAP_LIGHT, vPos, 2.0f, vLight, nullptr, (FWorldTime*0.12f));
						Vector(1.0f, 0.6f, 0.4f, vLight);
						CreateParticle(ThreadIndex, BITMAP_SMOKE, vPos, o->Angle, vLight, 31, 1.0f);
					}
					break;
				}
				break;

			case BITMAP_JOINT_HEALING:
				if (o->SubType == 4)
				{
					VectorCopy(o->Target->Position, o->Position);
					VectorAdd(o->TargetPosition, o->Position, o->Position);

					o->Position[2] += o->Velocity;
					o->Velocity += 10.0f;
					Luminosity = (12 - o->LifeTime)*0.1f;
					Vector(Luminosity*0.4f, Luminosity*0.6f, Luminosity*1.0f, o->Light);
				}
				else if (o->SubType == 5)
				{
					Luminosity = (12 - o->LifeTime)*0.1f;
					Vector(Luminosity*0.4f, Luminosity*0.6f, Luminosity*1.0f, o->Light);
				}
				else if (o->SubType == 9)
				{
					VectorSubtract(o->TargetPosition, o->Target->Position, Position);
					VectorCopy(o->Target->Position, o->TargetPosition);
					for (mu_int32 j = o->BeginTails, m = o->NumTails - 1; m >= 0; --m, j = (j + 1) % o->MaxTails)
					{
						for (mu_int32 k = 0; k < 4; ++k)
							VectorSubtract(o->Tails[j][k], Position, o->Tails[j][k]);
					}

					o->Position[0] = o->TargetPosition[0] + EMath::Sin(o->MultiUse*0.1f)*o->Direction[0];
					o->Position[1] = o->TargetPosition[1] + EMath::Cos(o->MultiUse*0.1f)*o->Direction[0];
					o->Position[2] = o->TargetPosition[2] + 10 + (90 - o->LifeTime);

					o->MultiUse += 2;
					o->Direction[0] -= 1.0f;

					if (o->Direction[0] < 0)
					{
						o->Direction[0] = 0.0f;
						o->LifeTime = 0;
					}

					if (o->LifeTime < 50)
					{
						o->Light[0] /= 1.1f;
						o->Light[1] /= 1.1f;
						o->Light[2] /= 1.1f;
					}
					else if (o->LifeTime > 80)
					{
						VectorScale(o->Light, 1.25f, o->Light);
					}
				}
				else if (o->SubType == 10)
				{
					VectorSubtract(o->TargetPosition, o->Target->Position, Position);
					VectorCopy(o->Target->Position, o->TargetPosition);
					for (mu_int32 j = o->BeginTails, m = o->NumTails - 1; m >= 0; --m, j = (j + 1) % o->MaxTails)
					{
						for (mu_int32 k = 0; k < 4; ++k)
							VectorSubtract(o->Tails[j][k], Position, o->Tails[j][k]);
					}

					o->Position[0] = o->TargetPosition[0] + EMath::Sin(o->MultiUse*0.1f)*o->Direction[0];
					o->Position[1] = o->TargetPosition[1] + EMath::Cos(o->MultiUse*0.1f)*o->Direction[0];
					o->Position[2] = o->TargetPosition[2] + (300 * (o->LifeTime / 80.0f));

					if (o->Features.Get(JOINT::eJFS_COLLISION))
					{
						o->MultiUse += 2;
					}
					else
					{
						o->MultiUse -= 2;
					}
					o->Direction[0] -= 1.0f;

					if (o->Direction[0] < 0)
					{
						o->Direction[0] = 0.0f;
						o->LifeTime = 0;
					}

					if (o->LifeTime < 40)
					{
						o->Light[0] /= 1.1f;
						o->Light[1] /= 1.1f;
						o->Light[2] /= 1.1f;
					}
					else if (o->LifeTime > 68)
					{
						VectorScale(o->Light, 1.2f, o->Light);
					}
				}
				else if (o->SubType == 14)
				{
					if (o->LifeTime < 5)
					{
						mu_float fLumi = o->LifeTime / 5.0f;
						VectorScale(o->Light, fLumi, o->Light);
					}
					o->Scale *= 1.05f;
					VectorCopy(o->Target->Position, o->Position);
				}
				else if (o->SubType == 15)
				{
					o->Velocity += 4.0f;

					VectorCopy(o->Target->Position, p);

					MoveHumming(o->Position, o->Angle, p, 10.0f);

					o->Light[0] /= 1.08f;
					o->Light[1] /= 1.08f;
					o->Light[2] /= 1.08f;

					CreateSprite(BITMAP_SHINY + 1, p, (mu_float)(GetLargeRand(4) + 4)*0.2f, o->Light, o->Target, (mu_float)(GetLargeRand(360)));
				}
				else if (o->SubType == 16)
				{
					o->Velocity += 4.0f;

					VectorCopy(o->Target->Position, p);

					MoveHumming(o->Position, o->Angle, p, 10.0f);

					o->Light[0] /= 1.08f;
					o->Light[1] /= 1.08f;
					o->Light[2] /= 1.08f;

					CreateSprite(BITMAP_SHINY + 1, p, (mu_float)(GetLargeRand(4) + 4)*0.2f, o->Light, o->Target, (mu_float)(GetLargeRand(360)), 1);
				}
				else
				{
					if (o->SubType == 6 || o->SubType == 7)
					{
						o->Velocity += 2.0f;
					}
					else
					{
						o->Velocity += 4.0f;
					}

					if (o->SubType == 6 || o->SubType == 7 || o->SubType == 12)
					{
						VectorCopy(o->TargetPosition, p);
					}
					else
					{
						if (o->SubType == 8 || o->SubType == 13 || o->SubType == 17)
						{
							VectorCopy(o->TargetPosition, p);

							if (o->LifeTime > 10)
							{
								MoveHumming(o->Position, o->Angle, p, 10.0f);
							}
							else if (o->LifeTime < 6)
							{
								o->Light[0] /= 1.8f;
								o->Light[1] /= 1.8f;
								o->Light[2] /= 1.8f;
							}

							if (o->SubType == 13)
								CreateSprite(BITMAP_LIGHT, o->Position, 1.0f, o->Light, nullptr);
							else
								CreateSprite(BITMAP_LIGHT, o->Position, 1.0f, o->Light, nullptr);
						}
						else
						{
							VectorCopy(o->Target->Position, p);
							p[2] += 120.0f;

							MoveHumming(o->Position, o->Angle, p, 10.0f);
						}
					}
					Luminosity = (12 - o->LifeTime)*0.1f;
					switch (o->SubType)
					{
					case 1:Vector(Luminosity*0.4f, Luminosity*0.6f, Luminosity*1.0f, o->Light); break;
					case 2:Vector(Luminosity*0.4f, Luminosity*1.0f, Luminosity*0.6f, o->Light); break;
					case 3:Vector(Luminosity*1.0f, Luminosity*0.6f, Luminosity*0.4f, o->Light); break;
					case 11:Vector(Luminosity*0.9f, Luminosity*0.49f, Luminosity*0.04f, o->Light); break;
					case 12:Vector(Luminosity*0.9f, Luminosity*0.39f, Luminosity*0.03f, o->Light); break;
					}
					if (o->SubType == 6)
					{
						if (o->LifeTime <= 10)
						{
							mu_float fAlpha = (mu_float)(6 - EMath::Abs(o->LifeTime - 6)) * 0.15f;
							EVector3 Light;
							Light[0] = Light[1] = Light[2] = fAlpha;
							CreateSprite(BITMAP_SHINY + 1, p, (mu_float)(GetLargeRand(8) + 8)*0.05f, Light, o->Target, (mu_float)(GetLargeRand(360)));
						}
					}
					else if (o->SubType != 7 && o->SubType != 8 && o->SubType != 12)
					{
						if (o->LifeTime == 1)
						{
							CreateSprite(BITMAP_SHINY + 1, p, (mu_float)(GetLargeRand(8) + 8)*0.2f, o->Light, o->Target, (mu_float)(GetLargeRand(360)));
						}
						if (o->SubType == 11)
						{
							Luminosity = (mu_float)(GetLargeRand(4) + 4)*0.01f;
							Vector(Luminosity, Luminosity, Luminosity, Light);
							AddTerrainLight(p[0], p[1], Light, 1, PrimaryTerrainLight);
						}
					}
				}
				break;

			case BITMAP_2LINE_GHOST:
				{
					if (o->SubType == 0)
					{
						if (o->LifeTime % 16 <= 7)
						{
							o->m_sTargetIndex = 10;
							o->Angle[2] += o->m_sTargetIndex;
						}
						else
						{
							o->m_sTargetIndex = -10;
							o->Angle[2] += o->m_sTargetIndex;
						}
						//					if(o->LifeTime%2 == 0)
						CreateParticle(ThreadIndex, BITMAP_SMOKE, o->Position, o->Angle, o->Light, 59, 1.0f);
					}
					else if (o->SubType == 1)
					{
						mu_float TargetAngleH;
						TargetAngleH = CreateAngle(o->Position[0], o->Position[1], o->TargetPosition[0], o->TargetPosition[1]);
						o->Angle[2] = TargetAngleH - 65;

						mu_float TargetAngleV;
						EVector3 Range;
						VectorSubtract(o->Position, o->TargetPosition, Range);
						mu_float Distance = EMath::Sqrt(Range[0] * Range[0] + Range[1] * Range[1]);
						if (Distance > 60)
						{
							TargetAngleV = 360.0f - CreateAngle(o->Position[2], Distance, o->TargetPosition[2], 0.0f);
							o->Angle[0] = TurnAngle2(o->Angle[0], TargetAngleV, 3.0f);
						}
						else
						{
							o->Angle[2] = TargetAngleH - 90;
						}
					}
				}
				break;
			case BITMAP_JOINT_SPIRIT:
			case BITMAP_JOINT_SPIRIT2:
				if (0 == o->SubType || o->SubType == 5 || o->SubType == 19)
				{
					if (o->Scale == 80.0f)
					{
						if (o->SubType == 5)
							CreateEffect(ThreadIndex, MODEL_LASER, o->Position, o->Angle, o->Light, 3);
						else
							CreateEffect(ThreadIndex, MODEL_LASER, o->Position, o->Angle, o->Light);

						if (battleCastle::IsBattleCastleStart())
						{
							mu_uint32 att = TERRAIN_ATTRIBUTE(o->Position[0], o->Position[1]);
							if ((att&TW_NOATTACKZONE) == TW_NOATTACKZONE)
							{
								o->Velocity = 0.0f;
								o->LifeTime /= 5.0f;
								break;
							}
						}
					}
					else if (o->SubType == 19)
					{
						CreateEffect(ThreadIndex, MODEL_SKULL, o->Position, o->Angle, o->Light);
					}
					VectorCopy(o->Target->Position, o->TargetPosition);
					o->TargetPosition[2] += 80.0f;

					Distance = EMath::Sqrt(dx*dx + dy * dy);
					if (o->SubType == 5)
						MoveHumming(o->Position, o->Angle, o->TargetPosition, 2.0f);
					else
						MoveHumming(o->Position, o->Angle, o->TargetPosition, 10.0f);
					if (!o->Features.Get(JOINT::eJFS_COLLISION) && Distance <= o->Velocity*2.0f)
					{
						o->Features.Set(JOINT::eJFS_COLLISION);
					}

					if (o->SubType == 5)
					{
					}
					else
					{
						o->Direction[0] += (mu_float)(GetLargeRand(32) - 16)*0.2f;
						o->Direction[2] += (mu_float)(GetLargeRand(32) - 16)*0.8f;
						o->Angle[0] += o->Direction[0];
						o->Angle[2] += o->Direction[2];
						o->Direction[0] *= 0.6f;
						o->Direction[2] *= 0.8f;
					}

					Height = RequestTerrainHeight(o->Position[0], o->Position[1]);
					if (o->Position[2] < Height + 100.0f)
					{
						o->Direction[0] = 0.0f;
						o->Angle[0] = -5.0f;
					}
					if (o->Position[2] > Height + 400.0f)
					{
						o->Direction[0] = 0.0f;
						o->Angle[0] = 5.0f;
					}
					if (o->SubType != 19)
					{
						Luminosity = o->LifeTime*0.1f;
						Vector(Luminosity, Luminosity, Luminosity, o->Light);
						Luminosity = -(mu_float)(GetLargeRand(4) + 4)*0.01f;
						Vector(Luminosity, Luminosity, Luminosity, Light);
						AddTerrainLight(o->Position[0], o->Position[1], Light, 4, PrimaryTerrainLight);
					}
				}
				else if (1 == o->SubType)
				{
					Vector(.8f, 0.4f, 1.0f, Light);
					CreateSprite(BITMAP_LIGHT, o->Position, 4.0f, Light, o->Target, (mu_float)(GetLargeRand(360)), 0);
				}
				else if (o->SubType == 3 || o->SubType == 13)
				{
					mu_float Scale = 3.0f;
					if (o->SubType != 13 || o->LifeTime > 28)
					{
						EVector3 vRot;
						GetMagicScrew(i, vRot);
						VectorScale(vRot, 50.0f, vRot);
						VectorAdd(o->Position, vRot, o->Position);
						Vector(1.0f, 0.5f, 0.1f, Light);
					}
					else if (o->Target != nullptr)
					{
						if (GetLargeRand(2) == 0)
						{
							if (o->Angle[0] < -90)
								o->Angle[0] += 20.0f;
							else
								o->Angle[0] -= 20.0f;
						}
						mu_float Distance;
						if (o->SubType == 13)
						{
							VectorCopy(o->Target->Position, Position);
							Position[0] += GetLargeRand(150) - 70.0f;
							Position[1] += GetLargeRand(150) - 70.0f;
							Position[2] += 50.0f;
							Distance = MoveHumming(o->Position, o->Angle, Position, (o->Velocity - 20));
						}
						else
						{
							Distance = MoveHumming(o->Position, o->Angle, o->Target->Position, (o->Velocity - 20));
						}
						if (o->Velocity > 50.0f)
							o->Velocity = 50.0f;
						else
							++o->Velocity;

						if (Distance < 50.0f)
						{
							--o->LifeTime;
						}
						if (o->LifeTime < 10)
						{
							o->Light[0] /= 1.2f;
							o->Light[1] /= 1.2f;
							o->Light[2] /= 1.2f;
							VectorCopy(o->Light, Light);
						}
						if (o->SubType == 13)
						{
							VectorCopy(o->Light, Light);

							Scale = 2.0f*(o->LifeTime / 10) + 2.0f;
						}
					}
					CreateSprite(BITMAP_LIGHT, o->Position, Scale, Light, o->Target, (mu_float)(GetLargeRand(360)), 0);
					CreateSprite(BITMAP_SHINY + 1, o->Position, Scale * 0.5f, Light, o->Target, (mu_float)(GetLargeRand(360)), 0);
				}
				else if (o->SubType == 2 || o->SubType == 6 || o->SubType == 7 || o->SubType == 21
					|| o->SubType == 22 || o->SubType == 23)
				{
					EVector3 Angle;

					switch (o->SubType)
					{
					case 2:	Vector(1.0f, 0.5f, 0.1f, Light); break;
					case 21: Vector(0.1f, 0.5f, 1.0f, Light); break;
					case 22:
					case 23:
						switch (o->Skill)
						{
						case 0: Vector(1.0f, 1.0f, 1.0f, Light); break;
						case 1: Vector(1.0f, 1.0f, 1.0f, Light); break;
						}
						break;
					case 7:
					case 6:
						switch (o->Skill)
						{
						case 0: Vector(0.3f, 0.3f, 1.0f, Light); break;
						case 1: Vector(0.5f, 0.5f, 0.5f, Light); break;
						}
						break;
					}

					if (o->LifeTime < 10)
					{
						o->Light[0] /= 1.2f;
						o->Light[1] /= 1.2f;
						o->Light[2] /= 1.2f;
						VectorCopy(o->Light, Light);
					}
					else if (o->LifeTime > 18 && o->LifeTime < 20 && (o->SubType == 2 || o->SubType == 21))
					{
						Vector(0.0f, 0.0f, (mu_float)(GetLargeRand(360)), Angle);

						Position[0] = o->StartPosition[0] + GetLargeRand(200) - 100;
						Position[1] = o->StartPosition[1] + GetLargeRand(200) - 100;
						Position[2] = o->StartPosition[2] - 200;

						if (o->SubType == 2)
							CreateJoint(ThreadIndex, BITMAP_FLARE, Position, Position, Angle, 2, nullptr, 40);
					}

					if (o->SubType == 22 || o->SubType == 23)
					{
						CreateSprite(BITMAP_FLAME, o->Position, (o->Scale + (20 - o->LifeTime) / 5), Light, o->Target, (mu_float)(GetLargeRand(360)), 0);
					}
					else
						CreateSprite(BITMAP_LIGHT, o->Position, (4.0f + (20 - o->LifeTime) / 5), Light, o->Target, (mu_float)(GetLargeRand(360)), 0);

					o->Velocity += 5.0f;
				}
				else if (8 == o->SubType)
				{
					if (o->LifeTime < 42)
					{
						o->Velocity = 50.0f;
						if (o->Angle[0] <= 180.0f)
							o->Angle[0] = 180.0f;
						if (o->Angle[0] <= 360.0f)
							o->Angle[0] += GetLargeRand(3) + 2.0f;
					}
					else
					{
						o->Velocity = 10.0f;
						o->Angle[0] = 110.0f;
					}
				}
				else if (10 == o->SubType)
				{
					o->Velocity = 50.0f;
					o->Angle[2] -= 50.0f;
				}
				else if (o->SubType == 9)
				{
					o->Position[0] += GetLargeRand(10) - 5.0f;
					o->Position[1] += GetLargeRand(10) - 5.0f;
					o->Position[2] += o->Velocity;
					o->Velocity += 1.0f;

					if (o->LifeTime < 10)
					{
						o->Light[0] /= 1.15f;
						o->Light[1] /= 1.15f;
						o->Light[2] /= 1.15f;
					}
				}
				else if (o->SubType == 11)
				{
					Vector(1, 1, 1, Light);
					EVector3 Position;
					VectorCopy(o->Position, Position);

					o->Angle[0] = (mu_float)o->LifeTime;
					CreateParticle(ThreadIndex, BITMAP_FIRE + 1, Position, o->Angle, Light, 5, 0.9f);
					if (GetLargeRand(200) == 0)
					{
						CreateJoint(ThreadIndex, BITMAP_JOINT_SPIRIT, Position, Position, o->Angle, 12, nullptr, 0.9f);
					}

					o->Position[0] += EMath::Cos(o->Angle[2]) * 20.0f;
					o->Position[1] += EMath::Sin(o->Angle[2]) * 20.0f;
				}
				else if (o->SubType == 12)
				{
					Vector(1, 1, 1, Light);
					EVector3 Position;
					VectorCopy(o->Position, Position);

					Vector(1.0f, 0.2f, 0.1f, Light);
					CreateParticle(ThreadIndex, BITMAP_SMOKE + 3, Position, o->Angle, Light, 0, (mu_float)(GetLargeRand(32) + 48)*0.01f);

					Vector(1, 1, 1, Light);
					CreateParticle(ThreadIndex, BITMAP_FIRE + 1, Position, o->Angle, Light, 6, 0.6f);
				}
				else if (o->SubType == 14)
				{
					Vector(1, 1, 1, Light);
					EVector3 Position;
					VectorCopy(o->Position, Position);

					o->Angle[0] -= 2.0f;
					mu_int32 r = GetLargeRand(5);
					if (r == 0)
						CreateParticle(ThreadIndex, BITMAP_WATERFALL_3, o->Position, o->Angle, o->Light, 2);
					else if (r <= 2)
						CreateParticle(ThreadIndex, BITMAP_WATERFALL_4, o->Position, o->Angle, o->Light, 2);
					else
						CreateParticle(ThreadIndex, BITMAP_WATERFALL_5, o->Position, o->Angle, o->Light, 4);
				}
				else if (o->SubType == 15)
				{
					Vector(1, 1, 1, Light);
					EVector3 Position;
					VectorCopy(o->Position, Position);
					o->PKKey += 30;

					if (o->LifeTime < 35)
					{
						CreateParticle(ThreadIndex, BITMAP_FIRE, Position, o->Angle, Light, 7, 1.0f);

						if (GetLargeRand(2) == 0)
						{
							Position[0] -= EMath::Sin(o->Angle[2]) * (40.0f - o->PKKey * 0.1f);
							Position[1] += EMath::Cos(o->Angle[2]) * (40.0f - o->PKKey * 0.1f);
							Position[2] = 350;
							EVector3 Angle;
							for (mu_int32 i = 0; i < 2; ++i)
							{
								Vector(0.0f, 0.0f, i*3.0f, Angle);
								CreateJoint(ThreadIndex, BITMAP_JOINT_SPIRIT, Position, Position, Angle, 14, nullptr, 0.9f);
							}
						}
						if (o->Target != nullptr)
						{
							if (o->Target->Angle[0] != 0)
								o->Target->Angle[0] = 0;
							if (o->Target->LifeTime > 5)
								o->Target->Alpha = 1.0f;
							VectorCopy(o->Position, Position);
							Position[0] += EMath::Sin(o->Angle[2]) * (30.0f + o->PKKey * 0.1f);
							Position[1] -= EMath::Cos(o->Angle[2]) * (30.0f + o->PKKey * 0.1f);
							VectorCopy(Position, o->Target->Position);
							if (o->Target->Angle[0] != 0)
								o->Target->Angle[0] = 0;
							o->Target->Angle[1] = 0;
						}
					}

					o->Position[0] += EMath::Sin(o->Angle[2]) * (30.0f + o->PKKey * 0.1f);
					o->Position[1] -= EMath::Cos(o->Angle[2]) * (30.0f + o->PKKey * 0.1f);
				}
				else if (o->SubType == 16)
				{
					Vector(1, 1, 1, Light);
					EVector3 Position;
					VectorCopy(o->Position, Position);

					o->Angle[0] = (mu_float)o->LifeTime;
					CreateParticle(ThreadIndex, BITMAP_WATERFALL_5, o->Position, o->Angle, o->Light, 4);
					o->Position[0] += EMath::Cos(o->Angle[2]) * 30.0f;
					o->Position[1] += EMath::Sin(o->Angle[2]) * 30.0f;
				}
				else if (o->SubType == 17)
				{
					Vector(1, 1, 1, Light);
					EVector3 Position, Angle;
					VectorCopy(o->Position, Position);

					if (o->LifeTime == 100)//% 10 == 0)
					{
						for (mu_int32 i = 0; i < 120; ++i)
						{
							Vector(0.0f, 0.0f, i*3.0f, Angle);
							CreateJoint(ThreadIndex, BITMAP_JOINT_SPIRIT, Position, o->Position, Angle, 16, nullptr, 0.9f);
						}
					}
				}
				else if (o->SubType == 18)
				{
					o->Velocity += 3.0f;
					o->Angle[0] = -90.0f;

					mu_int32 add = o->LifeTime % 12;
					if (add <= 5)
						o->PKKey += 6;
					else
						o->PKKey -= 6;

					o->Angle[0] -= o->PKKey;
					o->Angle[2] += 10;

					if (o->Target != nullptr)
					{
						if (o->Target->LifeTime > 5)
							o->Target->Alpha = 1.0f;
						VectorCopy(o->Position, o->Target->Position);
					}

					VectorCopy(o->Position, Position);
					Vector(1, 1, 1, Light);
					CreateParticle(ThreadIndex, BITMAP_SMOKE, Position, o->Angle, Light, 19, 7.0f);
				}
				else if (o->SubType == 20)
				{
					if (o->LifeTime < 42)
					{
						o->Velocity = 50.0f;
						if (o->Angle[0] <= 180.0f)
							o->Angle[0] = 180.0f;
						if (o->Angle[0] <= 360.0f)
							o->Angle[0] += GetLargeRand(3) + 2.0f;
					}
					else
					{
						o->Velocity = 10.0f;
						o->Angle[0] = 110.0f;
					}
				}
				else if (o->SubType == 24)
				{
					if (o->Target == nullptr || o->Target->Flags.Get(OBJECT::eOF_LIVE) == false)
					{
						o->LifeTime = 1;
						break;
					}
					EVector3 vPos;
					VectorCopy(o->TargetPosition, vPos);
					vPos[2] += 80.0f;

					MoveHumming(o->Position, o->Angle, vPos, 5.0f);

					o->Direction[0] += (mu_float)(GetLargeRand(32) - 16)*0.2f;
					o->Direction[2] += (mu_float)(GetLargeRand(32) - 16)*0.8f;
					o->Angle[0] += o->Direction[0];
					o->Angle[2] += o->Direction[2];
					o->Direction[0] *= 0.6f;
					o->Direction[2] *= 0.8f;

					if (o->LifeTime < 30)
						Luminosity = o->LifeTime / 30.0f;
					else if (o->LifeTime > 144)
						Luminosity = (160 - o->LifeTime) / 15.0f;
					else
						Luminosity = 1.0f;
					Vector(0.7f*Luminosity, 0.7f*Luminosity, 0.9f*Luminosity, o->Light);

					if (o->Target != nullptr)
					{
						VectorCopy(o->Position, o->Target->Position);
						VectorCopy(o->Angle, o->Target->Angle);
					}
				}
				else if (o->SubType == 25)
				{
					if (o->LifeTime < 10)
					{
						o->Light[0] /= 1.45f;
						o->Light[1] /= 1.45f;
						o->Light[2] /= 1.45f;

						if (o->Light[0] < 0.2f)
							o->LifeTime = 1;
					}
					else
					{
						o->Position[0] += GetLargeRand(16) - 8.0f;
						o->Position[1] += GetLargeRand(16) - 8.0f;
						o->Position[2] += o->Velocity;
						//o->Velocity += 1.0f;

						o->Scale += 3.0f;
					}

					CreateSprite(BITMAP_LIGHT, o->Position, 0.5f, o->Light, o->Target);
					CreateSprite(BITMAP_DS_SHOCK, o->Position, 0.15f, o->Light, o->Target);

					if (o->LifeTime == 10)
					{
						CreateEffect(ThreadIndex, BITMAP_FIRECRACKER0002, o->Position, o->Angle, o->Light, o->Skill);
					}
				}
				break;
			case BITMAP_JOINT_SPARK:
				if (o->SubType == 1)
				{
					o->Velocity += 0.3f;
					o->Light[0] /= 1.4f;
					o->Light[1] = o->Light[0];
					o->Light[2] = o->Light[0];
				}
				else if (o->SubType == 3)
				{
					o->Velocity += 0.1f;
					o->Light[0] /= 1.1f;
					o->Light[1] = o->Light[0];
					o->Light[2] = o->Light[0];
				}
				else if (o->SubType == 4)
				{
					o->Velocity += 0.1f;
					o->Light[0] /= 1.1f;
					o->Light[1] = o->Light[0];
					o->Light[2] = o->Light[0];
				}
				break;
			case BITMAP_JOINT_FIRE:
				{
					Distance = MoveHumming(o->Position, o->Angle, o->TargetPosition, 0.0f);
					EBone Matrix;
					AngleMatrix(o->Angle, Matrix);
					CreateTail(o, Matrix);
					if (Distance <= o->Velocity)
					{
						o->LifeTime = 1;
						CreateParticle(ThreadIndex, BITMAP_EXPLOTION, o->Position, o->Angle, o->Light);
						break;
					}
					Luminosity = (mu_float)(GetLargeRand(4) + 4)*0.1f;
					Vector(Luminosity, Luminosity*0.6f, Luminosity*0.2f, Light);
					AddTerrainLight(o->Position[0], o->Position[1], Light, 4, PrimaryTerrainLight);
					Vector(0.0f, -o->Velocity, 0.0f, Position);
					VectorRotate(Position, Matrix, p);
					VectorAdd(o->Position, p, o->Position);
				}
				break;
			case MODEL_SPEARSKILL:
				CHARACTER * c;
				if (o->m_iChaIndex != -1)
				{
					c = &CharactersClient[o->m_iChaIndex];
					if (o->SubType == 4)
					{
						if (c->Helper.Type == MODEL_HELPER + 37 && !c->SafeZone)
							o->SubType = 9;
					}
					else if (o->SubType == 9)
					{
						if (c->Helper.Type != MODEL_HELPER + 37 || c->SafeZone)
							o->SubType = 4;
					}
					else if (o->SubType == 10)
					{
						if ((c->Helper.Type == MODEL_HELPER + 2
							|| c->Helper.Type == MODEL_HELPER + 3
							|| c->Helper.Type == MODEL_HELPER + 4
							|| c->Helper.Type == MODEL_HELPER + 37) && c->SafeZone == false)
						{
							o->SubType = 11;
						}
					}
					else if (o->SubType == 11)
					{
						if ((c->Helper.Type != MODEL_HELPER + 2
							&& c->Helper.Type != MODEL_HELPER + 3
							&& c->Helper.Type != MODEL_HELPER + 4
							&& c->Helper.Type != MODEL_HELPER + 37) || c->SafeZone == true)
						{
							o->SubType = 10;
						}
					}
				}
				if (2 == o->SubType)
				{
					if (!o->Target || !o->Target->Flags.Get(OBJECT::eOF_LIVE))
					{
						o->Target = nullptr;
						break;
					}
					o->Scale = (mu_float)o->LifeTime * 3.0f;
					mu_float fRate1 = EMath::Max(0.0f, EMath::Min((mu_float)(o->LifeTime - 10) / (mu_float)10, 1.0f));
					mu_float fRate2 = 1.0f - fRate1;

					EVector3 MagicPos;
					GetMagicScrew(i * 17721, MagicPos, 1.4f);
					VectorScale(MagicPos, 300.0f, MagicPos);
					VectorAdd(MagicPos, o->TargetPosition, MagicPos);
					EVector3 TargetPos;
					VectorCopy(o->Target->m_vPosSword, TargetPos);

					for (mu_int32 i = 0; i < 3; ++i)
					{
						o->Position[i] = fRate2 * TargetPos[i] + fRate1 * MagicPos[i];
					}
				}
				else if (o->SubType == 3)
				{
					if (!o->Target->Flags.Get(OBJECT::eOF_LIVE))
					{
						o->LifeTime = 1;
						break;
					}
					if ((GetLargeRand(5)) == 0)
					{
						o->LifeTime -= 5;
					}
					else if ((GetLargeRand(2)) == 0)
					{
						++o->LifeTime;
					}
					o->Direction[0] = EMath::Sin((o->LifeTime - o->Direction[1])*0.05f)*3.0f;
					o->Position[0] += o->Direction[0];

					mu_float fAlpha;
					if (o->Target != nullptr)
					{
						if (o->Target->LifeTime > 50)
						{
							fAlpha = (100 - o->Target->LifeTime) / 40.0f;
						}
						else
						{
							fAlpha = o->Target->LifeTime / 10.0f;
						}

						if (fAlpha > 1.0f) fAlpha = 1.0f;

						o->Light[0] = o->Target->Light[0] * fAlpha;
						o->Light[1] = o->Target->Light[1] * fAlpha;
						o->Light[2] = o->Target->Light[2] * fAlpha;
					}
				}
				else if (o->SubType == 5 || o->SubType == 6 || o->SubType == 7)
				{
					for (mu_int32 i = 0; i < 3; ++i)
					{
						o->Angle[2] += 10.0f;
						AngleMatrix(o->Angle, Matrix);
						VectorRotate(o->Direction, Matrix, Position);
						VectorAdd(o->StartPosition, Position, o->Position);
						CreateTail(o, Matrix);

						if (o->Weapon == 0)
						{
							o->Direction[1] *= 0.95f;
							if (o->Direction[1] < 10.0f) o->Direction[1] = -10.0f;
						}
						if (o->Weapon > 40)
						{
							o->Direction[1] -= 20.0f;
							o->Angle[2] -= 5.0f;
							o->Scale += 15.0f;
						}
						if (o->Direction[1] < 0)
						{
							if (o->Weapon == 40 && o->SubType <= 6)
							{
								CreateEffect(ThreadIndex, BITMAP_SHOCK_WAVE, o->StartPosition, o->Angle, o->Light, 3);
							}
							++o->Weapon;

							if (o->Weapon < 20)
							{
								o->Direction[1] = -30.0f;
								o->Scale = 40.0f;
							}
							else
							{
								o->StartPosition[2] += 5.0f;
							}
						}
					}
					if (o->LifeTime < 10)
					{
						o->Light[0] /= 1.2f;
						o->Light[1] /= 1.2f;
						o->Light[2] /= 1.2f;
					}
					o->Scale -= 5.0f;
				}
				else if (o->SubType == 8)
				{
					o->Angle[2] += 25.0f;
					AngleMatrix(o->Angle, Matrix);
					VectorRotate(o->Direction, Matrix, Position);
					VectorAdd(o->StartPosition, Position, o->Position);
					CreateTail(o, Matrix);

					o->StartPosition[2] += 15.0f;
				}
				else
				{
					if (!o->Target->Flags.Get(OBJECT::eOF_LIVE))
					{
						o->LifeTime = 1;
						break;
					}

					if (o->SubType == 10 || o->SubType == 11)
					{
						mu_float fLumi = (mu_float)(GetLargeRand(4) + 4)*0.1f;
						Vector(fLumi*1.5f, fLumi*0.6f, fLumi*0.6f, Light);
						AddTerrainLight(o->Position[0], o->Position[1], Light, 1, PrimaryTerrainLight);

						if (GetLargeRand(10) == 0)
						{
							fLumi *= 0.2f;
							Vector(0.9f + fLumi, 0.5f + fLumi, 0.5f + fLumi, Light);
							CreateParticle(ThreadIndex, BITMAP_SPARK + 1, o->Position, o->Angle, Light, 19);
						}
					}

					if (o->SubType == 4
						|| o->SubType == 9)
					{
						if (g_isCharacterBuff(o->Target, eBuff_Defense) || g_isCharacterBuff(o->Target, eBuff_HelpNpc))
						{
							o->LifeTime = 100;
						}
						else
						{
							o->LifeTime = 1;
							break;
						}
					}

					if (0 == o->SubType || o->SubType == 4 || o->SubType == 9
						|| o->SubType == 10 || o->SubType == 11
						|| o->SubType == 14
						|| o->SubType == 16)
					{
						for (mu_int32 j = o->BeginTails, m = o->NumTails - 1; m >= 0; --m, j = (j + 1) % o->MaxTails)
						{
							for (mu_int32 k = 0; k < 4; ++k)
								VectorSubtract(o->Tails[j][k], o->TargetPosition, o->Tails[j][k]);
						}
					}

					if (o->SubType == 14)
					{
						EVector3 vRelative;
						Vector(0, 0, 0, vRelative);
						MUModel * pModel = &MODELS::Data[o->Target->Type];
						pModel->TransformPosition(ThreadIndex, o->Target->BoneTransform[pModel->GetBoneIndex(37)], vRelative, o->TargetPosition, false);
						VectorScale(o->TargetPosition, pModel->BodyScale(ThreadIndex), o->TargetPosition);
						VectorAdd(o->Target->Position, o->TargetPosition, o->TargetPosition);
					}
					else if (o->SubType == 15
						|| o->SubType == 17
						)
					{
						if (o->Target->Owner == nullptr)
						{
							o->LifeTime = 1;
							break;
						}

						for (mu_int32 j = o->BeginTails, m = o->NumTails - 1; m >= 0; --m, j = (j + 1) % o->MaxTails)
						{
							for (mu_int32 k = 0; k < 4; ++k)
								VectorSubtract(o->Tails[j][k], o->StartPosition, o->Tails[j][k]);
						}
						VectorCopy(o->Target->Owner->Position, o->StartPosition);
						VectorCopy(o->Target->Position, o->TargetPosition);
						for (mu_int32 j = o->BeginTails, m = o->NumTails - 1; m >= 0; --m, j = (j + 1) % o->MaxTails)
						{
							for (mu_int32 k = 0; k < 4; ++k)
								VectorAdd(o->Tails[j][k], o->StartPosition, o->Tails[j][k]);
						}
					}
					else
					{
						VectorCopy(o->Target->Position, o->TargetPosition);
						o->TargetPosition[2] += 10.0f;
					}

					if (0 == o->SubType || o->SubType == 4 || o->SubType == 9
						|| o->SubType == 10 || o->SubType == 11
						|| o->SubType == 14
						|| o->SubType == 16
						)
					{
						for (mu_int32 j = o->BeginTails, m = o->NumTails - 1; m >= 0; --m, j = (j + 1) % o->MaxTails)
						{
							for (mu_int32 k = 0; k < 4; ++k)
								VectorAdd(o->Tails[j][k], o->TargetPosition, o->Tails[j][k]);
						}
					}
					mu_int32 iFrame = MoveSceneFrame;

					iFrame = ((i % 2) ? iFrame : -iFrame) + i * 53731;

					EVector3 vDir, vDirTemp;

					//mu_float fSpeed[3] = { 0.097f, 0.1213f, 0.2313f};
					mu_float fSpeed[3] = { 0.048f, 0.0613f, 0.1113f };
					if (o->SubType == 1)
					{
						fSpeed[0] *= 0.5f;
						fSpeed[1] *= 0.5f;
						fSpeed[2] *= 0.5f;
					}
					vDirTemp[0] = EMath::Sin((mu_float)(iFrame + 55555)*fSpeed[0]) * EMath::Cos((mu_float)iFrame*fSpeed[1]);
					vDirTemp[1] = EMath::Sin((mu_float)(iFrame + 55555)*fSpeed[0]) * EMath::Sin((mu_float)iFrame*fSpeed[1]);
					vDirTemp[2] = EMath::Cos((mu_float)(iFrame + 55555)*fSpeed[0]);

					mu_float fSinAdd = EMath::Sin((mu_float)(iFrame + 11111)*fSpeed[2]);
					mu_float fCosAdd = EMath::Cos((mu_float)(iFrame + 11111)*fSpeed[2]);
					vDir[2] = vDirTemp[0];
					vDir[1] = fSinAdd * vDirTemp[1] + fCosAdd * vDirTemp[2];
					vDir[0] = fCosAdd * vDirTemp[1] - fSinAdd * vDirTemp[2];

					switch (o->SubType)
					{
					case 0:
					case 4:
						o->Position[0] = o->TargetPosition[0] + vDir[0] * 80.0f;// + ( mu_float)( GetLargeRand(11) - 5);
						o->Position[1] = o->TargetPosition[1] + vDir[1] * 80.0f;// + ( mu_float)( GetLargeRand(11) - 5);
						o->Position[2] = 110.0f + o->TargetPosition[2] + vDir[2] * 120.0f;
						break;
					case 9:
						o->Position[0] = o->TargetPosition[0] + vDir[0] * 80.0f;// + ( mu_float)( GetLargeRand(11) - 5);
						o->Position[1] = o->TargetPosition[1] + vDir[1] * 80.0f;// + ( mu_float)( GetLargeRand(11) - 5);
						o->Position[2] = 140.0f + o->TargetPosition[2] + vDir[2] * 120.0f;
						break;
					case 10:
						o->Position[0] = o->TargetPosition[0] + vDir[0] * 60.0f;// + ( mu_float)( GetLargeRand(11) - 5);
						o->Position[1] = o->TargetPosition[1] + vDir[1] * 60.0f;// + ( mu_float)( GetLargeRand(11) - 5);
						o->Position[2] = 50.0f + o->TargetPosition[2] + vDir[2] * 60.0f;
						break;
					case 11:
						o->Position[0] = o->TargetPosition[0] + vDir[0] * 60.0f;// + ( mu_float)( GetLargeRand(11) - 5);
						o->Position[1] = o->TargetPosition[1] + vDir[1] * 60.0f;// + ( mu_float)( GetLargeRand(11) - 5);
						o->Position[2] = 100.0f + o->TargetPosition[2] + vDir[2] * 60.0f;
						break;
					case 1:
						o->Position[0] = o->TargetPosition[0] + vDir[0] * 70.0f;// + ( mu_float)( GetLargeRand(11) - 5);
						o->Position[1] = o->TargetPosition[1] + vDir[1] * 70.0f;// + ( mu_float)( GetLargeRand(11) - 5);
						o->Position[2] = o->TargetPosition[2] + vDir[2] * 140.0f;

						Vector(0.2f, 0.2f, 0.4f + 0.2f*fSinAdd, o->Light);
						break;
					case 14:
						if (o->Target != nullptr)
						{
							if (o->Target->Flags.Get(OBJECT::eOF_LIVE))
								o->LifeTime = 100.0f;
							else
							{
								DeleteJoint(MODEL_SPEARSKILL, o->Target, 14);
								break;
							}

							o->Position[0] = o->TargetPosition[0] + vDir[0] * 15.0f;
							o->Position[1] = o->TargetPosition[1] + vDir[1] * 15.0f;
							o->Position[2] = o->TargetPosition[2] + vDir[2] * 15.0f;
						}
						break;
					case 15:
						if (o->Target != nullptr)
						{
							if (o->Target->Flags.Get(OBJECT::eOF_LIVE))
							{
								o->LifeTime = 100.0f;
								Vector(o->Target->Alpha, o->Target->Alpha, o->Target->Alpha, o->Light);
							}
							else
							{
								DeleteJoint(MODEL_SPEARSKILL, o->Target, 15);
								break;
							}
							VectorCopy(o->TargetPosition, o->Position);
						}
						break;
					case 16:
						{
							if (o->Target != nullptr)
							{
								o->Position[0] = o->TargetPosition[0] + vDir[0] * 20.0f;// + ( mu_float)( GetLargeRand(11) - 5);
								o->Position[1] = o->TargetPosition[1] + vDir[1] * 20.0f;// + ( mu_float)( GetLargeRand(11) - 5);
								o->Position[2] = 100.0f + o->TargetPosition[2] + vDir[2] * 40.0f;
							}
						}
						break;
					case 17:
						if (o->Target != nullptr)
						{
							if (o->Target->Flags.Get(OBJECT::eOF_LIVE))
							{
								o->LifeTime = 100.0f;
							}
							else
							{
								DeleteJoint(MODEL_SPEARSKILL, o->Target, 17);
								break;
							}
							VectorCopy(o->TargetPosition, o->Position);
						}
						break;
					}
				}
				break;

			case BITMAP_SMOKE:
				if (o->Target != nullptr && o->Target->Flags.Get(OBJECT::eOF_LIVE) &&
					(o->Target->Type == MODEL_MAYASTONE1
						|| o->Target->Type == MODEL_MAYASTONE2
						|| o->Target->Type == MODEL_MAYASTONE3
						|| o->Target->Type == MODEL_FIRE)
					)
				{
					VectorCopy(o->Target->Position, o->Position);
				}
				break;

			case MODEL_FENRIR_SKILL_THUNDER:
				for (mu_int32 j = 0; j < o->MaxTails; ++j)
				{
					if (o->Target)
					{
						VectorCopy(o->Target->Position, o->TargetPosition);
						o->TargetPosition[2] += 80.0f;
					}

					Distance = MoveHumming(o->Position, o->Angle, o->TargetPosition, 50.0f);

					o->Direction[0] = (mu_float)(GetLargeRand(1024) - 512) / o->Scale;
					o->Direction[2] = (mu_float)(GetLargeRand(1024) - 512) / o->Scale;

					EVector3 Angle;
					VectorAdd(o->Angle, o->Direction, Angle);
					EBone Matrix;
					AngleMatrix(Angle, Matrix);
					CreateTail(o, Matrix);

					if (Distance < o->Velocity*1.5f)
					{
						if (o->Scale == 50.0f)
						{
							CreateParticle(ThreadIndex, BITMAP_ENERGY, o->Position, o->Angle, o->Light);
							EVector3 Position;
							if (GetLargeRand(8) == 0)
							{
								Vector((mu_float)(GetLargeRand(64) - 32), (mu_float)(GetLargeRand(64) - 32), (mu_float)(GetLargeRand(64) - 32), Position);
								VectorAdd(Position, o->TargetPosition, Position);
								CreateParticle(ThreadIndex, BITMAP_SMOKE, o->Position, o->Angle, o->Light);
							}
						}
						break;
					}

					if (o->Scale >= 50.0f)
					{
						Luminosity = (mu_float)(GetLargeRand(4) + 4)*0.04f;
						Vector(Luminosity*0.4f, Luminosity*0.4f, Luminosity*0.4f, Light);
						AddTerrainLight(o->Position[0], o->Position[1], Light, 2, PrimaryTerrainLight);
					}

					Vector(0.0f, -o->Velocity, 0.0f, Position);
					VectorRotate(Position, Matrix, p);
					VectorAdd(o->Position, p, o->Position);
				}
				break;

			case BITMAP_BLUR + 1:
			case BITMAP_JOINT_LASER + 1:
				for (mu_int32 j = 0; j < o->MaxTails; ++j)
				{
					if (o->SubType == 2)
					{
					}
					else
					{
						VectorCopy(o->Target->Position, o->TargetPosition);
						o->TargetPosition[2] += 80.0f;
					}
					Distance = MoveHumming(o->Position, o->Angle, o->TargetPosition, 25.0f);
					//mu_float Distance = MoveHumming(o,0.0f);
					o->Direction[0] += (mu_float)(GetLargeRand(256) - 128) / o->Scale;
					o->Direction[2] += (mu_float)(GetLargeRand(256) - 128) / o->Scale;
					VectorScale(o->Direction, 0.8f, o->Direction);

					EVector3 Angle;
					VectorAdd(o->Angle, o->Direction, Angle);
					EBone Matrix;
					AngleMatrix(Angle, Matrix);
					CreateTail(o, Matrix);

					if (Distance <= o->Velocity*2.0f)
					{
						EVector3 Position;
						if (GetLargeRand(2) == 0)
						{
							Vector((mu_float)(GetLargeRand(64) - 32), (mu_float)(GetLargeRand(64) - 32), (mu_float)(GetLargeRand(64) - 32), Position);
							VectorAdd(Position, o->TargetPosition, Position);
							CreateParticle(ThreadIndex, BITMAP_FIRE, o->Position, o->Angle, o->Light);
						}
						break;
					}

					Luminosity = (mu_float)(GetLargeRand(4) + 4)*0.05f;
					if (o->Type == BITMAP_JOINT_LASER + 1)
					{
						if (o->SubType == 1)
						{
							Vector(Luminosity*1.0f, Luminosity*.1f, Luminosity*.1f, Light);
						}
						else
						{
							Vector(Luminosity*1.0f, Luminosity*0.6f, Luminosity*0.3f, Light);
						}
					}
					else
					{
						Vector(Luminosity*0.0f, Luminosity*0.1f, Luminosity*0.2f, Light);
					}
					AddTerrainLight(o->Position[0], o->Position[1], Light, 2, PrimaryTerrainLight);
					Vector(0.0f, -o->Velocity, 0.0f, Position);
					VectorRotate(Position, Matrix, p);
					VectorAdd(o->Position, p, o->Position);
				}
				break;

			case BITMAP_JOINT_THUNDER:
				if ((o->SubType == 6 && o->LifeTime > 4) || o->SubType == 8)
					break;

				switch (o->SubType)
				{
				case 4:
				case 5:
					VectorCopy(o->TargetPosition, o->Position);
					if (o->SubType == 4)
					{
						VectorCopy(o->TargetPosition, Position);
						Position[2] += 30.0f;
						CreateSprite(BITMAP_SHINY + 1, Position, (mu_float)(GetLargeRand(8) + 8)*0.2f, o->Light, nullptr, (mu_float)(GetLargeRand(360)));
					}
					break;

				case 9:
					Position[0] = o->TargetPosition[0] + GetLargeRand(200) - 100;
					Position[1] = o->TargetPosition[1] + GetLargeRand(200) - 100;
					Position[2] = o->TargetPosition[2];
				case 6:
				case 7:
				case 18:
				case 19:
					VectorCopy(o->StartPosition, o->Position);
					break;
				}

				for (mu_int32 j = 0; j < o->MaxTails; ++j)
				{
					if (o->SubType == 15)
						break;

					switch (o->SubType)
					{
					case 0:
					case 1:
					case 2:
					case 16:
					case 21:
					case 27:
					case 28:
					case 33:
						if (o->Target)
						{
							VectorCopy(o->Target->Position, o->TargetPosition);
							o->TargetPosition[2] += 80.0f;
						}

						Distance = MoveHumming(o->Position, o->Angle, o->TargetPosition, 50.0f);

						break;
					case 3:
						Distance = MoveHumming(o->Position, o->Angle, o->TargetPosition, 50.0f);
						break;

					case 4:
						Position[0] = o->TargetPosition[0];
						Position[1] = o->TargetPosition[1];
						Position[2] = o->TargetPosition[2] - 300.0f;

						Distance = MoveHumming(o->Position, o->Angle, Position, -10);
						break;

					case 5:
						Position[0] = o->TargetPosition[0];
						Position[1] = o->TargetPosition[1];
						Position[2] = o->TargetPosition[2] + 600.0f;

						Distance = MoveHumming(Position, o->Angle, o->Position, -10);
						break;

					case 6:
						VectorCopy(o->Target->Position, o->TargetPosition);
						o->TargetPosition[0] += (2050.0f + GetLargeRand(200));
						o->TargetPosition[1] += (2050.0f + GetLargeRand(200));
						o->TargetPosition[2] -= 10000.0f;

						Distance = MoveHumming(o->Position, o->Angle, o->TargetPosition, (mu_float)(GetLargeRand(100) + 50));//-25.0f);
						break;

					case 7:
						Distance = MoveHumming(o->Position, o->Angle, o->TargetPosition, (mu_float)(GetLargeRand(100) + 50));
						break;

					case 9:
						Distance = MoveHumming(o->Position, o->Angle, Position, (mu_float)(GetLargeRand(80) + 60.0f));
						break;
					case 12:
						{
#if 0 // FIX
							MUModel *b = &MODELS::Data[o->Target->Type];
							EVector3 Pos1;
							Vector(0.0f, -100.0f, 0.0f, Pos1);
							b->TransformPosition(ThreadIndex, g_fBoneSave[3], Pos1, o->Position, true);
#endif
						}
						break;
					case 18:
						Distance = MoveHumming(o->Position, o->Angle, o->TargetPosition, 110.0f);
						break;
					case 19:
						Distance = MoveHumming(o->Position, o->Angle, o->TargetPosition, 25.0f + o->Scale);
						break;
					case 20:
						o->TargetPosition[2] += 100.0f;
						Distance = MoveHumming(o->Position, o->Angle, o->TargetPosition, 100.0f);
						break;
					case 22:
					case 23:
					case 24:
						{
							if (o->Target)
							{
								if (j == 0)
								{
									OBJECT* pSourceObj = o->Target;
									CHARACTER* pTargetChar = &CharactersClient[FindCharacterIndex(o->m_sTargetIndex)];
									OBJECT* pTargetObj = &pTargetChar->Object;
									EVector3 vRelativePos, vPos, vAngle;
									MUModel *pModel = &MODELS::Data[pSourceObj->Type];
									Vector(0.0f, 0.0f, 0.0f, vRelativePos);
									if (o->SubType == 22)
									{
										pModel->TransformPosition(ThreadIndex, pSourceObj->BoneTransform[pModel->GetBoneIndex(37)], vRelativePos, vPos, true);
									}
									else if (o->SubType == 23)
									{
										pModel->TransformPosition(ThreadIndex, pSourceObj->BoneTransform[pModel->GetBoneIndex(28)], vRelativePos, vPos, true);
									}
									else if (o->SubType == 24)
									{
										VectorCopy(pSourceObj->Position, vPos);
										vPos[2] += 80.0f;
									}

									o->Direction[0] = (mu_float)(GetLargeRand(1024) - 512 / o->Scale);
									o->Direction[2] = (mu_float)(GetLargeRand(1024) - 512 / o->Scale);
									VectorAdd(o->Angle, o->Direction, vAngle);
									EMatrix4 fMatrix;
									AngleMatrix(vAngle, fMatrix);
									CreateTail(o, fMatrix);


									VectorCopy(vPos, o->Position);
									VectorCopy(pTargetObj->Position, o->TargetPosition);

									o->TargetPosition[2] += 80.0f;
								}

								Distance = MoveHumming(o->Position, o->Angle, o->TargetPosition, o->Velocity);
							}
						}
						break;
					case 25:
						{
							if (o->Target)
							{
								VectorCopy(o->Target->Position, o->TargetPosition);
								o->TargetPosition[2] += 80.0f;
							}

							Distance = MoveHumming(o->Position, o->Angle, o->TargetPosition, 40.0f);

						}
						break;
					default:
						Distance = MoveHumming(o->Position, o->Angle, o->TargetPosition, 25.0f);
						break;
					}

					if (o->SubType == 1
						|| o->SubType == 28)
					{
						o->Direction[0] = (mu_float)(GetLargeRand(256) - 128);
						o->Direction[2] = (mu_float)(GetLargeRand(256) - 128);
					}
					else if (o->SubType == 4)
					{
						o->Direction[0] = (mu_float)(GetLargeRand(64) - 32);
					}
					else if (o->SubType == 5)
					{
						o->Direction[0] = (mu_float)(GetLargeRand(32) - 16);
					}
					else if (o->SubType == 6)
					{
						o->Direction[0] = (mu_float)(GetLargeRand(100) + 20);
						o->Direction[2] = (mu_float)(GetLargeRand(100) + 20);
					}
					else if (o->SubType == 7)
					{
						o->Direction[0] = (mu_float)(GetLargeRand(100) + 20);
						o->Direction[2] = (mu_float)(GetLargeRand(100) + 20);
					}
					else if (o->SubType == 11)
					{
						o->Direction[0] = (mu_float)(GetLargeRand(1024) - 512)*0.7f / o->Scale;
						o->Direction[2] = (mu_float)(GetLargeRand(1024) - 512)*0.7f / o->Scale;
					}
					else if (o->SubType == 18)
					{
						o->Direction[0] = (mu_float)(GetLargeRand(64) - 32);
						o->Direction[2] = (mu_float)(GetLargeRand(64) - 32);
					}
					else if (o->SubType == 19)
					{
						o->Direction[0] = (mu_float)(GetLargeRand(1024) - 512) / o->Scale;
						o->Direction[1] = (mu_float)(GetLargeRand(1024) - 512) / o->Scale;
						o->Direction[2] = (mu_float)(GetLargeRand(1024) - 512) / o->Scale;
					}
					else
					{
						o->Direction[0] = (mu_float)(GetLargeRand(1024) - 512) / o->Scale;
						o->Direction[2] = (mu_float)(GetLargeRand(1024) - 512) / o->Scale;
					}

					EVector3 Angle;
					VectorAdd(o->Angle, o->Direction, Angle);
					EBone Matrix;
					AngleMatrix(Angle, Matrix);
					CreateTail(o, Matrix);

					if (o->SubType == 3)
					{
						if (Distance > 150)
						{
							o->LifeTime = 0;
						}
					}
					else if (Distance < o->Velocity * 1.5f &&
						(o->SubType != 6 && o->SubType != 9 && o->SubType != 7 && o->SubType != 18 && o->SubType != 19))
					{
						if (o->Scale == 50.0f)
						{
							CreateParticle(ThreadIndex, BITMAP_ENERGY, o->Position, o->Angle, o->Light);
							EVector3 Position;
							if (GetLargeRand(8) == 0)
							{
								Vector((mu_float)(GetLargeRand(64) - 32), (mu_float)(GetLargeRand(64) - 32), (mu_float)(GetLargeRand(64) - 32), Position);
								VectorAdd(Position, o->TargetPosition, Position);
								CreateParticle(ThreadIndex, BITMAP_SMOKE, o->Position, o->Angle, o->Light);
							}
							if (o->SubType == 0
								|| o->SubType == 27)
							{
								if (IWorldTime % 1000 < 500 && GetLargeRand(16) == 0)
								{
									Vector((mu_float)(GetLargeRand(100) - 50), (mu_float)(GetLargeRand(100) - 50), (mu_float)(GetLargeRand(120) - 60), Position);
									VectorAdd(Position, o->TargetPosition, Position);
									CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, Position, o->TargetPosition, o->Angle, 28, o->Target, 6.0f + (mu_float)(GetLargeRand(8)), -1, 0, 0, 0, &o->Light);
								}
							}
						}
						break;
					}

					if (o->Scale >= 50.0f && o->SubType != 4 && o->SubType != 7)
					{
						if (o->SubType == 0 || o->SubType == 11 || o->SubType == 2 || o->SubType == 18
							|| o->SubType == 27)
						{
							Luminosity = (mu_float)(GetLargeRand(4) + 4)*0.04f;
							if (o->SubType == 0 || o->SubType == 11 || o->SubType == 18
								|| o->SubType == 27)
							{
								Vector(Luminosity*0.1f, Luminosity*0.1f, Luminosity*0.5f, Light);
							}
							else
							{
								Vector(Luminosity*0.4f, Luminosity*0.1f, Luminosity*0.1f, Light);
							}
							if (o->SubType == 27)
								VectorCopy(o->Light, Light);
							AddTerrainLight(o->Position[0], o->Position[1], Light, 2, PrimaryTerrainLight);
						}
					}

					Vector(0.0f, -o->Velocity, 0.0f, Position);
					VectorRotate(Position, Matrix, p);
					VectorAdd(o->Position, p, o->Position);
				}

				if (o->SubType == 7 || o->SubType == 18)
				{
					VectorCopy(o->TargetPosition, o->Position);
				}
				else if (o->SubType == 15 && (o->LifeTime % 2) == 0)
				{
					CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, o->Position, o->StartPosition, o->Angle, 0, nullptr, 50.0f);
					CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, o->Position, o->StartPosition, o->Angle, 0, nullptr, 10.0f);
					CreateParticle(ThreadIndex, BITMAP_ENERGY, o->Position, o->Angle, Light);

					EVector3 Angle;
					VectorCopy(o->StartPosition, Position);
					Position[2] += 100.0f;
					for (mu_int32 i = 0; i < o->MultiUse; ++i)
					{
						if ((i % 15) == 0)
						{
							CHARACTER *tc = &CharactersClient[o->TargetIndex[i / 15]];
							OBJECT    *to = &tc->Object;

							if (to->Flags.Get(OBJECT::eOF_LIVE) && tc->Dead == 0 && to->Kind == KIND_MONSTER && to->Flags.Get(OBJECT::eOF_VISIBLE))
							{
								VectorCopy(to->Position, o->TargetPosition);
								VectorCopy(o->Angle, Angle);
								o->TargetPosition[2] += 100.0f;
								Angle[2] = CreateAngle(o->Position[0], o->Position[1], o->TargetPosition[0], o->TargetPosition[1]);

								CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, Position, o->TargetPosition, Angle, 0, nullptr, 50.0f);
								CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, Position, o->TargetPosition, Angle, 0, nullptr, 10.0f);

								VectorCopy(o->TargetPosition, Position);
							}
						}
					}

					if (o->MultiUse < o->Weapon)
					{
						if ((o->LifeTime % 2) == 0)
							++o->MultiUse;
					}
				}
				break;

			case BITMAP_JOINT_THUNDER + 1:
				if (o->SubType == 0)
				{
					if (o->LifeTime > 15)
					{
						Position[0] = o->TargetPosition[0] + GetLargeRand(200) - 100;
						Position[1] = o->TargetPosition[1] + GetLargeRand(100) - 50;
						Position[2] = o->TargetPosition[2];

						VectorCopy(o->StartPosition, o->Position);

						for (mu_int32 j = 0; j < o->MaxTails; ++j)
						{
							Distance = MoveHumming(o->Position, o->Angle, Position, (mu_float)(GetLargeRand(80) + 60.0f));

							o->Direction[0] = (mu_float)(GetLargeRand(1400) - 700) / o->Scale;
							o->Direction[2] = (mu_float)(GetLargeRand(1400) - 700) / o->Scale;

							EVector3 Angle;
							VectorAdd(o->Angle, o->Direction, Angle);
							EBone Matrix;
							AngleMatrix(Angle, Matrix);
							CreateTail(o, Matrix);

							Vector(0.0f, -o->Velocity, 0.0f, p);
							EVector3 vTempPos;
							VectorCopy(p, vTempPos);
							VectorRotate(vTempPos, Matrix, p);
							VectorAdd(o->Position, p, o->Position);
						}
					}
					else
					{
						o->Light[2] -= 10.12f;
					}
				}
				else if (o->SubType == 1 || o->SubType == 2 || o->SubType == 3 || o->SubType == 5 || o->SubType == 6 || o->SubType == 7)

				{
					VectorCopy(o->StartPosition, o->Position);
					AngleMatrix(o->Angle, Matrix);
					for (mu_int32 i = 0; i < o->MaxTails - 5; ++i)
					{
						if (o->SubType == 6)
						{
							o->Position[0] += GetLargeRand(20) - 10;
							o->Position[1] += GetLargeRand(20) - 10;
							o->Position[2] -= 13.0f;
						}
						else
						{
							o->Position[0] += GetLargeRand(20) - 10;
							o->Position[1] += GetLargeRand(20) - 10;

							if (o->SubType == 7)
							{
								o->Position[2] -= 20.0f;
							}
							else
							{
								o->Position[2] -= 16.0f;
							}
						}
						CreateTail(o, Matrix);
					}
					VectorSubtract(o->TargetPosition, o->Position, Position);
					VectorScale(Position, 0.2f, Position);
					for (mu_int32 i = o->MaxTails - 5; i < o->MaxTails - 1; ++i)
					{
						VectorAdd(o->Position, Position, o->Position);
						o->Position[0] += GetLargeRand(20) - 10;
						o->Position[1] += GetLargeRand(20) - 10;
						CreateTail(o, Matrix);
					}
					VectorCopy(o->TargetPosition, o->Position);
					CreateTail(o, Matrix);

					if (o->LifeTime < 4)
					{
						o->Light[0] /= 1.2f;
						o->Light[1] /= 1.2f;
						o->Light[2] /= 1.2f;
					}

					if (o->SubType == 2)
					{
						Vector(o->Light[0] * 0.5f, o->Light[1] * 0.6f, o->Light[2], Light);

						CreateSprite(BITMAP_SHINY + 1, o->TargetPosition, 1.5f, Light, nullptr, (mu_float)((GetLargeRand(360))));
						CreateSprite(BITMAP_SHINY + 1, o->TargetPosition, 1.5f, Light, nullptr, (mu_float)((GetLargeRand(360))));
						CreateParticle(ThreadIndex, BITMAP_TRUE_BLUE, o->TargetPosition, o->Angle, Light);
					}
					else if (o->SubType == 3)
					{
						Vector(o->Light[0] * 0.5f, o->Light[1] * 0.6f, o->Light[2], Light);

						CreateEffect(ThreadIndex, MODEL_ICE, o->TargetPosition, o->Angle, Light);
					}
					else if (o->SubType == 6)
					{
						o->TargetPosition[2] = RequestTerrainHeight(o->TargetPosition[0], o->TargetPosition[1]) + 30.0f;
						CreateParticle(ThreadIndex, BITMAP_TRUE_BLUE, o->TargetPosition, o->Angle, Light, 0, 2.0f);
					}
				}
				else if (o->SubType == 4)
				{
					if (o->Target != nullptr)
					{
						VectorCopy(o->Target->Position, o->Position);
						AngleMatrix(o->Angle, Matrix);

						VectorSubtract(o->TargetPosition, o->Position, Position);
						VectorScale(Position, o->StartPosition[0], Position);
						for (mu_int32 i = 0; i < o->TargetIndex[0]; ++i)
						{
							CreateTail(o, Matrix);
							VectorAdd(o->Position, Position, o->Position);
							o->Position[0] += GetLargeRand(20) - 10;
							o->Position[1] += GetLargeRand(20) - 10;
							o->Position[2] += GetLargeRand(20) - 10;
						}
						VectorSubtract(o->TargetPosition, o->Position, Position);
						VectorScale(Position, o->StartPosition[1], Position);
						for (mu_int32 i = o->TargetIndex[0]; i < o->TargetIndex[1]; ++i)
						{
							VectorAdd(o->Position, Position, o->Position);
							CreateTail(o, Matrix);
						}
						VectorCopy(o->TargetPosition, o->Position);
						CreateTail(o, Matrix);

						CreateSprite(BITMAP_SHINY + 1, o->TargetPosition, 1.5f, o->Light, nullptr, (mu_float)((GetLargeRand(360))));
						CreateSprite(BITMAP_SHINY + 1, o->TargetPosition, 1.5f, o->Light, nullptr, (mu_float)((GetLargeRand(360))));
						if (o->LifeTime < 8)
						{
							o->Light[0] /= 1.4f;
							o->Light[1] /= 1.4f;
							o->Light[2] /= 1.4f;
						}
					}
					else
					{
						o->LifeTime = 1;
					}
				}
				else if (o->SubType == 8)
				{
					VectorCopy(o->StartPosition, o->Position);
					AngleMatrix(o->Angle, Matrix);
					for (mu_int32 i = 0; i < o->MaxTails - 5; ++i)
					{
						o->Position[0] += GetLargeRand(10) - 5;
						o->Position[1] += GetLargeRand(10) - 5;
						o->Position[2] -= 20.0f;
						CreateTail(o, Matrix);
					}
					VectorSubtract(o->TargetPosition, o->Position, Position);
					VectorScale(Position, 0.2f, Position);
					for (mu_int32 i = o->MaxTails - 5; i < o->MaxTails - 1; ++i)
					{
						VectorAdd(o->Position, Position, o->Position);
						o->Position[0] += GetLargeRand(10) - 5;
						o->Position[1] += GetLargeRand(10) - 5;
						CreateTail(o, Matrix);
					}
					VectorCopy(o->TargetPosition, o->Position);
					CreateTail(o, Matrix);

					if (o->LifeTime < 4)
					{
						o->Light[0] /= 1.2f;
						o->Light[1] /= 1.2f;
						o->Light[2] /= 1.2f;
					}
				}
				else if (o->SubType == 9)
				{
					VectorCopy(o->StartPosition, o->Position);
					AngleMatrix(o->Angle, Matrix);
					for (mu_int32 i = 0; i < o->MaxTails - 5; ++i)
					{
						o->Position[0] += 21.0f;
						CreateTail(o, Matrix);
					}
					VectorSubtract(o->TargetPosition, o->Position, Position);
					VectorScale(Position, 0.2f, Position);
					for (mu_int32 i = o->MaxTails - 5; i < o->MaxTails - 1; ++i)
					{
						VectorAdd(o->Position, Position, o->Position);
						o->Position[1] += GetLargeRand(20) - 10;
						o->Position[2] += GetLargeRand(20) - 10;
						CreateTail(o, Matrix);
					}
					VectorCopy(o->TargetPosition, o->Position);
					CreateTail(o, Matrix);

					if (o->LifeTime < 4)
					{
						o->Light[0] /= 1.2f;
						o->Light[1] /= 1.2f;
						o->Light[2] /= 1.2f;
					}
				}
				else if (o->SubType == 10)
				{
					VectorCopy(o->StartPosition, o->Position);
					AngleMatrix(o->Angle, Matrix);
					for (mu_int32 i = 0; i < o->MaxTails - 5; ++i)
					{
						o->Position[1] += 20.0f;
						CreateTail(o, Matrix);
					}
					VectorSubtract(o->TargetPosition, o->Position, Position);
					VectorScale(Position, 0.2f, Position);
					for (mu_int32 i = o->MaxTails - 5; i < o->MaxTails - 1; ++i)
					{
						VectorAdd(o->Position, Position, o->Position);
						o->Position[0] += GetLargeRand(20) - 10;
						o->Position[2] += GetLargeRand(20) - 10;
						CreateTail(o, Matrix);
					}
					VectorCopy(o->TargetPosition, o->Position);
					CreateTail(o, Matrix);

					if (o->LifeTime < 4)
					{
						o->Light[0] /= 1.2f;
						o->Light[1] /= 1.2f;
						o->Light[2] /= 1.2f;
					}
				}
				else if (o->SubType == 11)
				{
					EVector3 p1, p2;
					VectorCopy(o->StartPosition, o->Position);
					Vector(0.0f, o->Scale / 1.3f, 0.0f, p1);
					AngleMatrix(o->Angle, Matrix);
					VectorRotate(p1, Matrix, p2);

					for (mu_int32 i = 0; i < o->MaxTails - 5; ++i)
					{
						VectorAdd(o->Position, p2, o->Position);
						CreateTail(o, Matrix);
						VectorCopy(o->Position, o->TargetPosition);
					}
					VectorCopy(o->StartPosition, o->Position);
					for (mu_int32 i = 0; i < o->MaxTails - 5; ++i)
					{
						mu_int32 iScale = 1;
						VectorAdd(o->Position, p2, o->Position);
						iScale = (mu_int32)(o->Scale / 8.0f);
						o->Position[0] += GetLargeRand(iScale * 2) - iScale;
						o->Position[1] += GetLargeRand(iScale * 2) - iScale;
						o->Position[2] += GetLargeRand(iScale * 2) - iScale;
						CreateTail(o, Matrix);
					}
					VectorCopy(o->TargetPosition, o->Position);
					CreateTail(o, Matrix);
				}
				else if (o->SubType == 12)
				{
					VectorCopy(o->StartPosition, o->Position);
					AngleMatrix(o->Angle, Matrix);
					for (mu_int32 i = 0; i < o->MaxTails - 1; ++i)
					{
						mu_int32 iScale = 1;
						iScale = (mu_int32)(o->Scale / 5.0f);
						o->Position[0] += GetLargeRand(iScale * 2) - iScale;
						o->Position[1] += GetLargeRand(iScale * 2) - iScale;
						o->Position[2] += GetLargeRand(iScale * 2) - iScale;
						CreateTail(o, Matrix);
						VectorSubtract(o->TargetPosition, o->Position, Position);
						VectorScale(Position, 0.08f, Position);
						VectorAdd(o->Position, Position, o->Position);
					}
					VectorCopy(o->TargetPosition, o->Position);
					CreateTail(o, Matrix);
				}
				else if (o->SubType == 0
					|| o->SubType == 2 || o->SubType == 3 || o->SubType == 4 || o->SubType == 5
					|| o->SubType == 6 || o->SubType == 8 || o->SubType == 9 || o->SubType == 10)
				{
					if (o->LifeTime <= 10)
					{
						o->Features.Clear(JOINT::eJFS_CREATETAILS);
					}
					else
					{
						o->Features.Set(JOINT::eJFS_CREATETAILS);
					}
				}
				break;
			case BITMAP_SPARK + 1:
				if (o->SubType == 0)
				{
					o->Direction[2] += 5.0f;
					o->Position[0] = o->TargetPosition[0];
					o->Position[1] = o->TargetPosition[1];
					o->Position[2] += o->Direction[2];
				}
				if (o->SubType == 1)
				{
					o->Velocity += 0.1f;
					o->Light[0] /= 1.1f;
					o->Light[1] = o->Light[0];
					o->Light[2] = o->Light[0];
				}
				else if (o->LifeTime < 5)
				{
					o->Light[0] /= 1.3f;
					o->Light[1] /= 1.3f;
					o->Light[2] /= 1.3f;
				}
				break;
			case BITMAP_FLARE:
			case BITMAP_FLARE_BLUE:
				if (o->SubType == 0 || o->SubType == 10 || o->SubType == 18)
				{
					mu_float count;
					if (o->PKKey != -1)
					{
						count = (o->Direction[1] + o->LifeTime) / (mu_float)(o->PKKey);
					}
					else if (o->SubType == 18)
					{
						count = (o->Direction[1] + (o->LifeTime * 2))*0.1f;
						if (o->Skill == 1)
						{
							count *= -1;
						}
					}
					else
					{
						count = (o->Direction[1] + o->LifeTime)*0.1f;
					}
					o->Position[0] = o->TargetPosition[0] + EMath::Cos(count)*o->Velocity;
					o->Position[1] = o->TargetPosition[1] - EMath::Sin(count)*o->Velocity;

					if (o->SubType == 0)
						o->Position[2] += o->Direction[2];
					else if (o->SubType == 10)
						o->Position[2] -= o->Direction[2];
					else if (o->SubType == 18)
					{
						o->Position[2] += (o->Direction[2] * 1.1f);
						o->Velocity += 0.1f;
						o->Scale += 0.2f;
					}
				}
				else if (o->SubType == 20)
				{
					if (o->Target == nullptr)
					{
						o->LifeTime = 1;
						break;
					}

					MUModel* b = &MODELS::Data[o->Target->Type];
					EVector3 p;
					Vector(0.0f, 0.0f, 0.0f, p);
					b->TransformPosition(ThreadIndex, o->Target->BoneTransform[b->GetBoneIndex(33)], p, o->Position, true);
				}
				else if (o->SubType == 14 || o->SubType == 15)
				{
					if (o->Target != nullptr)
					{
						mu_float count;

						count = o->Direction[1] * 0.1f;
						if (o->SubType == 15)
						{
							count *= -1;
						}
						o->Position[0] = o->TargetPosition[0] + EMath::Cos(count)*o->Velocity;
						o->Position[1] = o->TargetPosition[1] - EMath::Sin(count)*o->Velocity;
						o->Position[2] += o->Direction[2];
						if (o->LifeTime < 20)
						{
							o->Velocity += 5.0f;
						}
						o->Scale += 1;
						o->Direction[1] += 4;
						o->Direction[2] += (GetLargeRand(200) + 200) / 100.0f;
					}
				}
				else if (o->SubType == 2 || o->SubType == 24
					|| o->SubType == 50 || o->SubType == 51
					)
				{
					if (o->SubType == 51)
					{
						o->Direction[2] += 10.0f;
						o->Position[0] = o->TargetPosition[0];
						o->Position[1] = o->TargetPosition[1];
						o->Position[2] += o->Direction[2];
					}
					if (o->LifeTime <= 25
						|| o->SubType == 50
						)
					{
						o->Direction[2] += 5.0f;
						o->Position[0] = o->TargetPosition[0];
						o->Position[1] = o->TargetPosition[1];
						o->Position[2] += o->Direction[2];
					}
				}
				else if (o->SubType == 41)
				{
					if (o->LifeTime <= 20)
					{
						o->Direction[2] += 1.0f;
						o->Position[0] = o->TargetPosition[0];
						o->Position[1] = o->TargetPosition[1];
						o->Position[2] += o->Direction[2];
						o->Light[0] /= 1.1f;
						o->Light[1] /= 1.1f;
						o->Light[2] /= 1.1f;
					}
					AddTerrainLight(o->Position[0], o->Position[1], o->Light, 1, PrimaryTerrainLight);
				}
				else if (o->SubType == 42)
				{
					o->Angle[2] += 15.0f;
					if (o->Target->Flags.Get(OBJECT::eOF_LIVE))
						o->LifeTime = 100.0f;
					else
					{
						DeleteJoint(BITMAP_FLARE, o->Target, 42);
						break;
					}

					AngleMatrix(o->Angle, Matrix);
					VectorRotate(o->Direction, Matrix, Position);
					VectorAdd(o->StartPosition, Position, o->Position);

					mu_int32 tBias = GetLargeRand(100);
					if (((mu_int32)o->Angle[2] % 45 == 0) && tBias <= 50)
					{
						EVector3 Angle;
						Vector(0.0f, 0.0f, 0.0f, Angle);
						CreateJoint(ThreadIndex, BITMAP_FLARE, o->Position, o->Position, Angle, 41, nullptr, o->Scale);
					}
					AddTerrainLight(o->TargetPosition[0], o->TargetPosition[1], o->Light, 2, PrimaryTerrainLight);
				}
				else if (o->SubType == 19)
				{
					if (o->LifeTime <= 25)
					{
						o->Direction[2] -= 5.0f;
						o->Position[0] = o->TargetPosition[0];
						o->Position[1] = o->TargetPosition[1];
						o->Position[2] += o->Direction[2];
					}
				}
				else if (o->SubType == 3)
				{
					//          o->Velocity += 20.0f;
				}
				else if (o->SubType == 40)
				{
					mu_int32 lMax = o->LifeTime;
					mu_int32 radious = 50.0f;
					EVector3 vPos;
					EVector3 backPos;
					o->Features.Clear(JOINT::eJFS_CREATETAILS);
					if (o->NumTails != 0) o->NumTails = 0;
					VectorCopy(o->Position, backPos);
					for (mu_int32 i = 0; i < MAX_TAILS; ++i)
					{
						mu_float theta = i * Q_PI / 6;
						mu_float rCos = radious * EMath::Cos(theta);
						mu_float rSin = radious * EMath::Sin(theta);
						vPos[0] = rCos * EMath::Cos((90.0f - o->Angle[2]) * Q_PI / 180.0f);
						vPos[1] = rCos * EMath::Sin((90.0f - o->Angle[2]) * Q_PI / 180.0f);
						vPos[2] = rSin;
						o->TargetPosition[0] += o->Direction[0] * (lMax - o->LifeTime);
						o->TargetPosition[1] += o->Direction[1] * (lMax - o->LifeTime);
						VectorAdd(o->TargetPosition, vPos, o->Position);
						AngleMatrix(o->Angle, Matrix);
						CreateTail(o, Matrix);
					}
					VectorCopy(backPos, o->Position);
					o->Position[0] += o->Direction[0] * 12;
					o->Position[1] += o->Direction[1] * 12;
					VectorCopy(o->Position, o->TargetPosition);

					o->Scale *= 1.1f;

					o->Light[0] = 0.5f + (mu_float)(GetLargeRand(64)) / 255;
					o->Light[1] = 0.5f + (mu_float)(GetLargeRand(64)) / 255;
					o->Light[2] = 0.5f + (mu_float)(GetLargeRand(128)) / 255;
				}
				else if (o->SubType == 4 || o->SubType == 6 || o->SubType == 12)
				{
					mu_float count;
					EVector3 vPos;
					if (o->SubType == 6)
					{
						if (o->Type == BITMAP_FLARE_BLUE)
						{
							count = (o->Direction[0] + (o->LifeTime*0.8f));
						}
						else
						{
							count = (o->Direction[0] + (o->LifeTime*0.5f));
						}
						vPos[0] = -EMath::Cos(count)*((mu_float)EMath::Max(o->LifeTime, 1)*2.0f);
						vPos[1] = EMath::Sin(count)*((mu_float)EMath::Max(o->LifeTime, 1)*2.0f);
					}
					else if (o->SubType == 12)
					{
						count = (o->Direction[0] + o->LifeTime)*0.1f;
						vPos[0] = -EMath::Cos(count)*(40.0f*.65f);
						vPos[1] = EMath::Sin(count)*(40.0f*.65f) - (mu_float)(90 - o->LifeTime) * 0.3f;
					}
					else
					{
						count = (o->Direction[0] + o->LifeTime)*0.1f;
						vPos[0] = -EMath::Cos(count)*((mu_float)EMath::Max(o->LifeTime + 40, 10)*.65f);
						vPos[1] = EMath::Sin(count)*((mu_float)EMath::Max(o->LifeTime + 40, 10)*.65f);
					}
					vPos[2] = 0.0f;
					o->TargetPosition[0] += o->Direction[1];
					o->TargetPosition[1] += o->Direction[2];
					o->Position[0] = vPos[0] * EMath::Sin((90.0f - o->Angle[2])*Q_PI / 180.0f) + o->TargetPosition[0];
					o->Position[1] = vPos[0] * EMath::Cos((90.0f - o->Angle[2])*Q_PI / 180.0f) + o->TargetPosition[1];
					o->Position[2] = vPos[1] + o->TargetPosition[2];
				}
				else if (o->SubType == 5)
				{
					o->Direction[2] -= 60.0f;
					o->Position[0] = o->TargetPosition[0];
					o->Position[1] = o->TargetPosition[1];
					o->Position[2] += o->Direction[2];
					o->Scale = GetLargeRand(4) + 6.0f;
				}
				else if (o->SubType == 7 || o->SubType == 11 || o->SubType == 25
					|| o->SubType == 45 || o->SubType == 46
					|| o->SubType == 47
					)
				{
					if (o->SubType == 11
						|| o->SubType == 25
						)
					{
						if (o->Target->Flags.Get(OBJECT::eOF_LIVE) == false)
						{
							o->LifeTime = 1;
							break;
						}
					}

					mu_int32 iFrame = MoveSceneFrame;

					iFrame = ((i % 2) ? iFrame : -iFrame) + i * 53731;

					EVector3 vDir, vDirTemp;

					mu_float fSpeed[3] = { 0.048f, 0.0613f, 0.1113f };
					if (o->SubType == 11)
					{
						fSpeed[0] *= 1.5f;
						fSpeed[1] *= 1.5f;
						fSpeed[2] *= 1.5f;
					}
					vDirTemp[0] = EMath::Sin((mu_float)(iFrame + 55555)*fSpeed[0]) * EMath::Cos((mu_float)iFrame*fSpeed[1]);
					vDirTemp[1] = EMath::Sin((mu_float)(iFrame + 55555)*fSpeed[0]) * EMath::Sin((mu_float)iFrame*fSpeed[1]);
					vDirTemp[2] = EMath::Cos((mu_float)(iFrame + 55555)*fSpeed[0]);

					mu_float fSinAdd = EMath::Sin((mu_float)(iFrame + 11111)*fSpeed[2]);
					mu_float fCosAdd = EMath::Cos((mu_float)(iFrame + 11111)*fSpeed[2]);
					vDir[2] = vDirTemp[0];
					vDir[1] = fSinAdd * vDirTemp[1] + fCosAdd * vDirTemp[2];
					vDir[0] = fCosAdd * vDirTemp[1] - fSinAdd * vDirTemp[2];

					mu_float fLife = (mu_float)o->LifeTime * 40.0f / 30.0f;
					mu_float fPos;

					if (o->SubType == 11
						|| o->SubType == 25
						)
					{
						if (fLife < 20.0f)
						{
							fPos = fLife * 4.0f;
						}
						else
						{
							fPos = fLife * 1.0f + 60.0f;
						}
					}
					else
					{
						if (fLife < 10.0f)
						{
							fPos = fLife * 7.0f;
						}
						else
						{
							fPos = fLife * 1.0f + 60.0f;
						}
					}
					fPos = fPos / (mu_float)(30 + o->MultiUse) * 30.0f;
					if (o->LifeTime == 30)
					{
						PlayBackground(SOUND_METEORITE01);
					}

					mu_float fCircle;
					if (o->SubType == 11
						|| o->SubType == 25
						)
					{
						fCircle = EMath::Min(EMath::Max(0.0f, fLife - 10) * 5.0f, 150.0f);
					}
					else
					{
						fCircle = EMath::Min(EMath::Max(0.0f, 40.0f - fLife) * 15.0f, 150.0f);
					}
					o->Position[0] = o->TargetPosition[0] + vDir[0] * fCircle;
					o->Position[1] = o->TargetPosition[1] + vDir[1] * fCircle;
					o->Position[2] = o->TargetPosition[2] + vDir[2] * fCircle;

					mu_float fLastTarget;
					for (mu_int32 k = 0; k < 3; ++k)
					{
						if (o->SubType == 11)
							fLastTarget = (100.0f - fPos) * (o->Target->StartPosition[k] + 25.0f * EMath::Cos((mu_float)(i * 51231 + k * 3711 + iFrame / 10) * 0.01f));
						else if (o->SubType == 25)
							fLastTarget = (100.0f - fPos) * (o->Target->StartPosition[k] + 25.0f * EMath::Cos((mu_float)(i * 51231 + k * 3711 + iFrame / 10) * 0.01f));
						else
							fLastTarget = (100.0f - fPos) * (o->Target->Position[k] + 25.0f * EMath::Cos((mu_float)(i * 51231 + k * 3711 + iFrame / 10) * 0.01f));
						o->Position[k] = (fPos * o->Position[k] + fLastTarget) * 0.01f;
					}
					if (o->SubType != 11
						&& o->SubType != 25)
					{
						o->Position[2] += 100.0f;
					}

					EVector3 Light = { .5f, .5f, 1.0f };

					if (o->SubType == 11)
					{
						Vector(0.3f, 0.3f, 0.5f, Light);
						if (o->Skill == 1)
						{
							VectorCopy(o->Position, o->Target->Position);
							CheckClientArrow(ThreadIndex, o->Target);
						}
					}
					if (o->SubType == 25)
					{
						Vector(0.9f, 0.4f, 0.6f, Light);
						if (o->Skill == 1)
						{
							VectorCopy(o->Position, o->Target->Position);
							CheckClientArrow(ThreadIndex, o->Target);
						}
					}
					if (o->SubType == 45 && o->LifeTime <= 1)
					{
						CreateEffect(ThreadIndex, MODEL_CHANGE_UP_EFF, o->Target->Position, o->Target->Angle, o->Target->Light, 1, o->Target);
						CreateEffect(ThreadIndex, MODEL_CHANGE_UP_CYLINDER, o->Target->Position, o->Target->Angle, o->Target->Light, 1, o->Target);
					}
					{
						if (o->SubType != 47)
						{
							CreateSprite(BITMAP_SHINY + 1, o->Position, (mu_float)(GetLargeRand(2) + 8)*0.10f, Light, o->Target, (mu_float)(GetLargeRand(360)));
							CreateSprite(BITMAP_LIGHT, o->Position, (mu_float)(GetLargeRand(2) + 8)*0.18f, Light, o->Target, (mu_float)(GetLargeRand(360)));
							CreateSprite(BITMAP_LIGHT, o->Position, (mu_float)(GetLargeRand(2) + 8)*0.18f, Light, o->Target, (mu_float)(GetLargeRand(360)));
						}
					}

					if (o->SubType != 11 && 1 == o->LifeTime && o->SubType != 45 && o->SubType != 46
						&& o->SubType != 47
						)
					{
						EVector3 Angle = { 0.0f, 0.0f, 0.0f };
						EVector3 Light = { 1.0f, 1.0f, 1.0f };
						CreateParticle(ThreadIndex, BITMAP_EXPLOTION, o->Position, Angle, Light, 1, 0.6f);
					}
				}
				else if (o->SubType == 8)
				{
					Vector(0.0f, -50.0f, 0.0f, p);
					AngleMatrix(o->TargetPosition, Matrix);
					VectorRotate(p, Matrix, Position);
					VectorAdd(o->StartPosition, Position, o->Position);
					AngleMatrix(o->Angle, Matrix);

					o->TargetPosition[2] += 10.0f;
				}
				else if (o->SubType == 9)
				{
					AngleMatrix(o->Angle, Matrix);
					for (mu_int32 i = 0; i < o->MaxTails; ++i)
					{
						VectorAdd(o->Position, o->StartPosition, o->Position);
						CreateTail(o, Matrix);
					}
				}
				else if (o->SubType == 13)
				{
					if (o->LifeTime <= 25)
					{
						o->Direction[2] += 1.0f;
						o->Position[0] = o->TargetPosition[0];
						o->Position[1] = o->TargetPosition[1];
						o->Position[2] += o->Direction[2];
					}
				}
				else if (o->SubType == 17)
				{
					if (((o->Target->CurrentAction >= PLAYER_WALK_MALE /*&& o->Target->CurrentAction<=PLAYER_RUN_RIDE_WEAPON*/) ||
						(o->Target->CurrentAction >= PLAYER_ATTACK_SKILL_SWORD1 && o->Target->CurrentAction <= PLAYER_ATTACK_SKILL_SWORD5)))
					{
						o->LifeTime -= 10;
					}
					else
					{
						o->Direction[0] += EMath::Sin(GetLargeRand(360) * 0.1f)*4.0f;
						o->Direction[1] += EMath::Cos(GetLargeRand(360) * 0.1f)*4.0f;
						o->Direction[2] += o->Velocity;
						o->Position[0] = o->TargetPosition[0] + o->Direction[0];
						o->Position[1] = o->TargetPosition[1] + o->Direction[1];
						o->Position[2] = o->TargetPosition[2] + o->Direction[2];
					}

					if (o->LifeTime < 12)
					{
						o->Light[0] /= 1.3f;
						o->Light[1] /= 1.3f;
						o->Light[2] /= 1.3f;
					}
				}
				else if (o->SubType == 16 && o->Target != nullptr)
				{
					VectorSubtract(o->TargetPosition, o->Target->Position, Position);
					VectorCopy(o->Target->Position, o->TargetPosition);

					for (mu_int32 j = o->BeginTails, m = o->NumTails - 1; m >= 0; --m, j = (j + 1) % o->MaxTails)
					{
						for (mu_int32 k = 0; k < 4; ++k)
							VectorSubtract(o->Tails[j][k], Position, o->Tails[j][k]);
					}

					if (o->LifeTime < 20)
					{
						o->Light[0] /= 1.1f;
						o->Light[1] /= 1.1f;
						o->Light[2] /= 1.1f;
					}
					else if (o->LifeTime < 40)
					{
						o->Light[0] = 0.25f;
						o->Light[1] = 0.25f;
						o->Light[2] = 0.25f;
					}
					else
					{
						o->Light[0] *= 1.04f;
						o->Light[1] *= 1.04f;
						o->Light[2] *= 1.04f;
					}
				}
				else if (o->SubType == 23)
				{
					if (o->LifeTime < 20)
					{
						o->Velocity = 200.0f;
						if (o->LifeTime < 8.0f)
						{
							o->Velocity = 0.0f;
							AngleMatrix(o->HeadAngle, Matrix);
							CreateTail(o, Matrix);
						}
						else
						{
							for (mu_float i = 0; i < o->Direction[2]; ++i)
							{
								o->Direction[1] += 1.5f;
								switch (o->MultiUse)
								{
								case 0: o->HeadAngle[0] += 3.0f; o->HeadAngle[2] -= o->Direction[1]; break;
								case 1: o->HeadAngle[0] -= 3.0f; o->HeadAngle[2] -= o->Direction[1]; break;
								case 2: o->HeadAngle[0] += 3.0f; o->HeadAngle[2] += o->Direction[1]; break;
								case 3: o->HeadAngle[0] -= 3.0f; o->HeadAngle[2] += o->Direction[1]; break;
								case 4: o->HeadAngle[2] -= o->Direction[1]; break;
								case 5: o->HeadAngle[0] -= o->Direction[1]; break;
								}
								if (o->Direction[1] > 15.0f)
								{
									o->Direction[1] = 15.0f;
								}

								AngleMatrix(o->HeadAngle, Matrix);
								Vector(0.0f, -o->Velocity, 0.0f, p);
								VectorRotate(p, Matrix, Position);
								VectorAdd(o->StartPosition, Position, o->Position);

								CreateTail(o, Matrix);

								if (o->NumTails >= o->MaxTails - 1)
								{
									o->Velocity = 0.0f;
									break;
								}
							}
							o->Direction[2] += 4.0f;
						}
					}
				}
				else if (o->SubType == 43)
				{
					if (o->Target->Flags.Get(OBJECT::eOF_LIVE))
					{
						o->LifeTime = 100.0f;

						if (GetLargeRand(3) == 0)
						{
							VectorCopy(o->Target->Position, o->Position);
							o->Position[2] += 30.0f;
							o->Position[0] += (GetLargeRand(60) - 30);
							o->Position[1] += (GetLargeRand(60) - 30);
							CreateJoint(ThreadIndex, BITMAP_FLARE, o->Position, o->Position, o->Angle, 44, o->Target, o->Scale, 0, 0, 0, 0, &o->Light);
						}
					}
					else
					{
						DeleteJoint(BITMAP_FLARE, o->Target, 43);
						break;
					}
				}
				else if (o->SubType == 44)
				{
					if (o->Target->Flags.Get(OBJECT::eOF_LIVE) == false)
					{
						o->LifeTime = 1;
						break;
					}
					else
					{
						if (o->LifeTime <= 20)
						{
							o->Direction[2] += 1.0f;
							o->Position[0] = o->TargetPosition[0];
							o->Position[1] = o->TargetPosition[1];
							o->Position[2] += o->Direction[2];
							o->Light[0] /= 1.05f;
							o->Light[1] /= 1.05f;
							o->Light[2] /= 1.05f;

							CreateSprite(BITMAP_PIN_LIGHT, o->Position, 1.5f, o->Light, nullptr);
							CreateSprite(BITMAP_PIN_LIGHT, o->Position, 0.5f, o->Light, nullptr);

							if (o->Position[2] >= 400.0f)
							{
								o->LifeTime = 1;
							}
						}
					}
				}

				if (o->SubType != 5 &&
					o->SubType != 7 &&
					o->SubType != 11)
				{
					if (o->LifeTime < 10)
					{
						o->Light[0] /= 1.3f;
						o->Light[1] /= 1.3f;
						o->Light[2] /= 1.3f;
					}
				}

				if ((o->SubType == 0 || o->SubType == 10 || o->SubType == 18) && o->Scale < 40.0f)
				{
					if (((o->Target->CurrentAction >= PLAYER_WALK_MALE && o->Target->CurrentAction <= PLAYER_RUN_RIDE_WEAPON) ||
						(o->Target->CurrentAction >= PLAYER_ATTACK_SKILL_SWORD1 && o->Target->CurrentAction <= PLAYER_ATTACK_SKILL_SWORD5)))
					{
						o->SubType = 1;
						o->Features.Clear(JOINT::eJFS_CREATETAILS);
						o->LifeTime = EMath::Min(10, o->LifeTime);
					}
				}
				break;

			case BITMAP_FLARE + 1:
				if (o->SubType == 5)
				{
					if (o->LifeTime >= 25)
					{
						o->Angle[2] -= 10.0f;
						o->Position[2] += 2.5f;
					}
					else
					{
						o->Velocity += 5.0f;
						if (o->Velocity >= 30.0f)
							o->Velocity = 30.0f;

						mu_float fOldAngle = o->Angle[2];
						VectorCopy(o->TargetPosition, Position);
						Distance = MoveHumming(o->Position, o->Angle, Position, o->Velocity);

						if (Distance <= 70.0f && EMath::Abs(fOldAngle - o->Angle[2]) > 20.0f)
						{
							if (o->Velocity >= 20.0f)
								o->Velocity -= 5.0f;
						}

						if (o->LifeTime < 10)
						{
							o->Light[0] /= 1.2f;
							o->Light[1] = o->Light[0];
							o->Light[2] = o->Light[0];
						}
					}
					Luminosity = (mu_float)(GetLargeRand(4) + 8)*0.03f;
					Vector(Luminosity*0.4f, Luminosity*0.8f, Luminosity*0.4f, Light);
					AddTerrainLight(o->Position[0], o->Position[1], Light, 2, PrimaryTerrainLight);
				}
				else if (o->SubType == 16)
				{
					if (o->LifeTime >= 25)
					{
						o->Angle[2] -= 10.0f;
						o->Position[2] += 2.5f;
					}
					else
					{
						o->Velocity += 5.0f;
						if (o->Velocity >= 30.0f)
							o->Velocity = 30.0f;

						mu_float fOldAngle = o->Angle[2];
						VectorCopy(o->Target->Position, Position);
						Distance = MoveHumming(o->Position, o->Angle, Position, o->Velocity);

						if (Distance <= 70.0f && EMath::Abs(fOldAngle - o->Angle[2]) > 20.0f)
						{
							if (o->Velocity >= 20.0f)
								o->Velocity -= 10.0f;
						}

						if (o->LifeTime < 10)
						{
							o->Light[0] /= 1.2f;
							o->Light[1] = o->Light[0];
							o->Light[2] = o->Light[0];
						}
					}
					Luminosity = 1.0f;
					Vector(Luminosity*1.0f, Luminosity*1.0f, Luminosity*1.0f, Light);
					AddTerrainLight(o->Position[0], o->Position[1], Light, 1, PrimaryTerrainLight);
				}
				else if (o->SubType == 6)
				{
					if (o->Target != nullptr)
					{
						if (o->LifeTime >= 30)
						{
							o->Angle[2] -= 10.0f;
							o->Position[2] += 2.5f;
						}

						o->Velocity += 5.0f;
						if (o->Velocity >= 30.0f)
							o->Velocity = 30.0f;

						mu_float fOldAngle = o->Angle[2];
						VectorCopy(o->Target->Position, Position);
						Position[2] += 150.0f;
						Distance = MoveHumming(o->Position, o->Angle, Position, o->Velocity);

						if (Distance <= 70.0f && EMath::Abs(fOldAngle - o->Angle[2]) > 20.0f)
						{
							if (o->Velocity >= 20.0f)
								o->Velocity -= 10.0f;
						}

						if (o->LifeTime < 15)
						{
							o->Light[0] /= 1.2f;
							o->Light[1] = o->Light[0];
							o->Light[2] = o->Light[0];
						}
						Luminosity = (mu_float)(GetLargeRand(4) + 8)*0.03f;
						Vector(Luminosity*0.4f, Luminosity, Luminosity*0.8f, Light);
						AddTerrainLight(o->Position[0], o->Position[1], Light, 2, PrimaryTerrainLight);
					}
					else
					{
						o->LifeTime = 1;
					}
				}
				else if (o->SubType == 7)
				{
					o->Position[0] = o->StartPosition[0] + EMath::Sin(o->LifeTime / 2.5f)*o->Direction[0];
					o->Position[1] = o->StartPosition[1];
					o->Position[2] += o->Velocity;
					o->Velocity -= 0.1f;
					o->Direction[0] -= 0.1f;

					if (o->LifeTime < 15)
					{
						o->Light[0] /= 1.2f;
						o->Light[1] /= 1.2f;
						o->Light[2] /= 1.2f;
					}
				}
				else if (o->SubType == 8 || o->SubType == 9)
				{
					if (o->Target == nullptr)
					{
						o->LifeTime = 1;
						break;
					}
					oh->Flags.Set(o->Target->Flags.GetDirect(OBJECT::eOF_LIVE));
					VectorCopy(o->Target->Position, o->Position);

					if (o->SubType == 8)
					{
						o->Position[2] = 300.0f;
					}
					else if (o->SubType == 9)
					{
						o->Position[2] += o->MultiUse;
					}
				}
				else if (o->SubType == 12
					|| o->SubType == 13
					|| o->SubType == 14
					|| o->SubType == 17
					|| o->SubType == 15
					|| o->SubType == 18
					)
				{
					if (o->Target != nullptr)
					{
						if (o->Target->Flags.Get(OBJECT::eOF_LIVE) == false)
						{
							o->LifeTime = 1;
						}
						else if (o->SubType == 17)
						{
							Vector(1.0f, 1.0f, 0.2f, o->Light);
						}
						else if (o->SubType == 14)
						{
							CreateSprite(BITMAP_SPARK + 1, o->Position, 5.0f, o->Light, nullptr);

							CreateParticle(ThreadIndex, BITMAP_SMOKE, o->Position, o->Angle, o->Light, 34, 1.0f);
							CreateParticle(ThreadIndex, BITMAP_SMOKE, o->Position, o->Angle, o->Light, 35, 1.0f);
							//CreateParticle(ThreadIndex, BITMAP_SPARK+1, o->Position, o->Angle, o->Light, 12, 1.0f);
							Vector(0.7f, 0.3f, 0.3f, o->Light);
							CreateParticle(ThreadIndex, BITMAP_SPARK + 1, o->Position, o->Angle, o->Light, 10, 4.0f);
							Vector(1.0f, 1.0f, 0.2f, o->Light);
						}
						else if (o->SubType == 15)
						{
							CreateParticle(ThreadIndex, BITMAP_SHINY, o->Position, o->Angle, o->Light, 3, 0.5f);
							CreateParticle(ThreadIndex, BITMAP_SHINY, o->Position, o->Angle, o->Light, 3, 0.5f);
						}

						if (o->LifeTime < 15)
						{
							o->Light[0] /= 1.2f;
							o->Light[1] /= 1.2f;
							o->Light[2] /= 1.2f;
						}

						if (o->SubType != 18)
						{
							Luminosity = (mu_float)(GetLargeRand(4) + 8)*0.03f;
							Vector(Luminosity*0.4f, Luminosity, Luminosity*0.8f, Light);
							AddTerrainLight(o->Position[0], o->Position[1], Light, 2, PrimaryTerrainLight);
						}
					}
					else
					{
						o->LifeTime = 1;
					}
				}
				else if (o->SubType == 19)
				{
					EVector3  pos;
					EMatrix4 Mat;

					EVector3  Angle;
					VectorSubtract(o->Target->Position, o->Target->StartPosition, pos);
					VectorCopy(o->Target->StartPosition, o->TargetPosition);
					//VectorCopy( o->Target->HeadAngle, Angle );
					AngleMatrix(o->Angle, Mat);

					pos[0] /= 3.0f; pos[1] /= 3.0f; pos[2] /= 3.0f;
					//Angle[1] += o->Velocity - 90;

					for (mu_int32 j = 0; j < 3; ++j)
					{
						Angle[1] += 30.0f;
						VectorAdd(o->TargetPosition, pos, o->TargetPosition);

						EVector3  position;
						AngleMatrix(Angle, Matrix);
						VectorRotate(o->Direction, Matrix, position);
						VectorAdd(o->TargetPosition, position, o->Position);

						if (o->NumTails < (o->MaxTails - 1) || o->Skill != 0)
							CreateTail(o, Mat);
					}
				}
				else if (o->SubType == 20)
				{
					EVector3  pos;
					EMatrix4 Mat;

					EVector3  Angle;
					VectorSubtract(o->Target->Position, o->Target->StartPosition, pos);
					VectorCopy(o->Target->StartPosition, o->TargetPosition);
					//VectorCopy( o->Target->HeadAngle, Angle );
					AngleMatrix(o->Angle, Mat);

					pos[0] /= 3.0f; pos[1] /= 3.0f; pos[2] /= 3.0f;
					//Angle[1] += o->Velocity - 90;

					for (mu_int32 j = 0; j < 3; ++j)
					{
						Angle[1] += 30.0f;
						VectorAdd(o->TargetPosition, pos, o->TargetPosition);

						EVector3  position;
						AngleMatrix(Angle, Matrix);
						VectorRotate(o->Direction, Matrix, position);
						VectorAdd(o->TargetPosition, position, o->Position);

						if (o->NumTails < (o->MaxTails - 1) || o->Skill != 0)
							CreateTail(o, Mat);
					}
				}
				else
				{
					EVector3  pos;
					EMatrix4 Mat;
					if (o->Target->Flags.Get(OBJECT::eOF_LIVE))
					{
						EVector3  Angle;
						VectorSubtract(o->Target->Position, o->Target->StartPosition, pos);
						VectorCopy(o->Target->StartPosition, o->TargetPosition);
						VectorCopy(o->Target->HeadAngle, Angle);
						AngleMatrix(o->Angle, Mat);

						pos[0] /= 3.0f; pos[1] /= 3.0f; pos[2] /= 3.0f;
						Angle[1] += o->Velocity - 90;

						for (mu_int32 j = 0; j < 3; ++j)
						{
							Angle[1] += 30.0f;
							VectorAdd(o->TargetPosition, pos, o->TargetPosition);

							EVector3  position;
							AngleMatrix(Angle, Matrix);
							VectorRotate(o->Direction, Matrix, position);
							VectorAdd(o->TargetPosition, position, o->Position);

							if (o->NumTails < (o->MaxTails - 1) || o->Skill != 0)
								CreateTail(o, Mat);
						}

						if (o->LifeTime < 15)
						{
							o->Light[0] /= 1.5f;
							o->Light[1] /= 1.5f;
							o->Light[2] /= 1.5f;
						}
					}
					else
					{
						o->LifeTime = -1;
						VectorCopy(o->TargetPosition, o->Position);
						o->Position[2] += 50.0f;
					}
				}
				break;

			case BITMAP_JOINT_FORCE:
				if (o->SubType == 10 || o->SubType == 8)
				{
					Vector(0.0f, -145.0f, 0.0f, p);
					for (mu_int32 j = 0; j < 8; ++j)
					{
						if ((mu_int32)o->NumTails < (mu_int32)o->MaxTails - 1)
						{
							VectorCopy(o->Position, Light);
							AngleMatrix(o->Direction, Matrix);
							VectorRotate(p, Matrix, Position);
							VectorAdd(o->TargetPosition, Position, o->Position);
							AngleMatrix(o->Angle, Matrix);
							CreateTail(o, Matrix);

							o->Direction[2] -= 11.0f;
							if (o->SubType == 10)
							{
								if ((GetLargeRand(2)) == 0)
								{
									CreateParticle(ThreadIndex, BITMAP_FIRE, o->Position, o->Angle, o->Light, 0);
								}

								CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, Light, o->Position, o->Angle, 3, nullptr, GetLargeRand(10) + 5.0f, 5, 10);
								CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, Light, o->Position, o->Angle, 3, nullptr, GetLargeRand(8) + 4.0f, 5, 10);
							}
							else
								if (o->SubType == 8)
								{
									CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, o->Position, o->Position, o->Angle, 3, nullptr, GetLargeRand(10) + 5.0f, 5, 10);
									CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, o->Position, o->Position, o->Angle, 3, nullptr, GetLargeRand(8) + 4.0f, 5, 10);
								}
						}
					}
					if (o->SubType == 10)
					{
						if (o->Features.Get(JOINT::eJFS_ALTERNATIVE) == false)
						{
							if (o->LifeTime < 10)
								Luminosity = o->Light[0] / 1.5f;
							else
								Luminosity = o->LifeTime / 30.0f;
							Vector(Luminosity, Luminosity, Luminosity, o->Light);
						}
						else
						{
							if (o->LifeTime < 10)
							{
								o->Light[0] = o->Light[0] / 1.5f;
								o->Light[1] = o->Light[1] / 1.5f;
								o->Light[2] = o->Light[2] / 1.5f;
							}
						}
					}
				}
				else if (o->SubType == 0)
				{
					Vector(0.0f, -145.0f, 0.0f, p);
					for (mu_int32 j = 0; j < 8; ++j)
					{
						if ((mu_int32)o->NumTails < (mu_int32)o->MaxTails - 1)
						{
							VectorCopy(o->Position, Light);
							AngleMatrix(o->Direction, Matrix);
							VectorRotate(p, Matrix, Position);
							VectorAdd(o->TargetPosition, Position, o->Position);
							AngleMatrix(o->Angle, Matrix);
							CreateTail(o, Matrix);

							o->Direction[2] -= 11.0f;
							if (o->SubType == 0)
							{
								if ((GetLargeRand(2)) == 0)
								{
									CreateParticle(ThreadIndex, BITMAP_FIRE, o->Position, o->Angle, o->Light, 0);
								}

								CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, Light, o->Position, o->Angle, 3, nullptr, GetLargeRand(10) + 5.0f, 5, 10);
								CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, Light, o->Position, o->Angle, 3, nullptr, GetLargeRand(8) + 4.0f, 5, 10);
							}
						}
					}
					if (o->LifeTime < 10)
						Luminosity = o->Light[0] / 1.5f;
					else
						Luminosity = o->LifeTime / 30.0f;
					Vector(Luminosity, Luminosity, Luminosity, o->Light);
				}
				else if (o->SubType == 1)
				{
					if (o->Target == nullptr)
					{
						o->LifeTime = 1;
						break;
					}
					oh->Flags.Set(o->Target->Flags.GetDirect(OBJECT::eOF_LIVE));
					VectorCopy(o->Target->Position, o->Position);

					o->Scale -= 1.0f;
					o->Position[2] += 100.0f;

					if (o->LifeTime < 15)
					{
						o->Light[0] /= 1.3f;
						o->Light[1] /= 1.3f;
						o->Light[2] /= 1.3f;
					}
				}
				else if (o->SubType >= 2 && o->SubType <= 6)
				{
					o->Velocity += o->Direction[2];
					if (o->LifeTime < 15)
					{
						o->Direction[2] += 0.5f;
					}
					else
					{
						o->Direction[2] += o->Direction[0];
					}
					if (o->SubType != 4)
					{
						if (o->NumTails >= o->MaxTails - 1)
						{
							o->Features.Clear(JOINT::eJFS_CREATETAILS);
						}
					}
					if (o->LifeTime < o->MultiUse)
					{
						o->Light[0] /= 1.3f;
						o->Light[1] /= 1.3f;
						o->Light[2] /= 1.3f;
					}
					if (o->LifeTime % 5 == 0 && o->SubType == 2)
					{
						CreateEffect(ThreadIndex, MODEL_SKILL_INFERNO, o->StartPosition, o->HeadAngle, o->Light, 6, nullptr, 20, 0);
					}
					else if (o->SubType == 4)
					{
						Vector(0.1f, 0.6f, 1.0f, Light);

						CreateJoint(ThreadIndex, BITMAP_FLARE_BLUE, o->Position, o->Position, o->Angle, 6, nullptr, 30.0f);
						CreateSprite(BITMAP_LIGHT, o->Position, 1.6f, Light, nullptr);
						CreateSprite(BITMAP_SHINY + 1, o->Position, 1.5f, Light, nullptr, FWorldTime*0.1f);
					}
				}
				else if (o->SubType == 7 || o->SubType == 20)
				{
					o->Velocity += o->Direction[2];
					if (o->LifeTime < 20)
					{
						o->Direction[2] += 0.5f;
					}
					else
					{
						o->Direction[2] += o->Direction[0];
					}
					if (o->NumTails >= o->MaxTails - 1)
					{
						o->Features.Clear(JOINT::eJFS_CREATETAILS);
					}
					if (o->LifeTime < o->MultiUse)
					{
						o->Light[0] /= 1.3f;
						o->Light[1] /= 1.3f;
						o->Light[2] /= 1.3f;
					}
					o->Position[2] = RequestTerrainHeight(o->Position[0], o->Position[1]) + 3.0f;
				}
				break;

			case BITMAP_LIGHT:
				if (o->SubType == 0)
				{
					if (o->LifeTime > 16)
					{
						AngleMatrix(o->Angle, Matrix);
						for (mu_int32 i = 0; i < 10; ++i)
						{
							CreateTail(o, Matrix);

							Vector(0.0f, -o->Velocity, 0.0f, p);
							EVector3 vPos;
							VectorRotate(p, Matrix, vPos);
							VectorAdd(o->Position, vPos, o->Position);
						}
					}
				}
				else if (o->SubType == 1)
				{
					if (o->Skill == 0 || o->LifeTime > 3)
					{
						AngleMatrix(o->Angle, Matrix);
						for (mu_int32 i = 0; i < 5; ++i)
						{
							CreateTail(o, Matrix);

							Vector(0.0f, -o->Velocity, 0.0f, p);
							EVector3 vPos;
							VectorRotate(p, Matrix, vPos);
							VectorAdd(o->Position, vPos, o->Position);
							o->Velocity += 0.5f;

						}
					}
				}
				o->Light[0] /= 1.2f;
				o->Light[1] /= 1.2f;
				o->Light[2] /= 1.2f;
				break;
			case BITMAP_PIERCING:
				if (o->SubType == 0)
				{
					if (o->LifeTime > 9)
					{
						AngleMatrix(o->Angle, Matrix);
						for (mu_int32 i = 0; i < 30; ++i)
						{
							CreateTail(o, Matrix);

							Vector(0.0f, 0.0f, -o->Velocity, p);
							VectorRotate(p, Matrix, Position);
							VectorAdd(o->Position, Position, o->Position);
						}
					}
					o->Velocity -= 2.0f;

					o->Light[0] /= 1.4f;
					o->Light[1] /= 1.4f;
					o->Light[2] /= 1.4f;
				}
				else if (o->SubType == 1)
				{
					if (o->LifeTime > 9)
					{
						AngleMatrix(o->Angle, Matrix);
						for (mu_int32 i = 0; i < 30; ++i)
						{
							CreateTail(o, Matrix);

							Vector(0.0f, 0.0f, -o->Velocity, p);
							VectorRotate(p, Matrix, Position);
							VectorAdd(o->Position, Position, o->Position);
						}
					}
					o->Velocity -= 2.0f;

					o->Light[0] /= 1.4f;
					o->Light[1] /= 1.4f;
					o->Light[2] /= 1.4f;
				}
				break;

			case BITMAP_FLARE_FORCE:
				if (o->SubType == 5 || o->SubType == 6 || o->SubType == 7)
				{
					if (o->Target != nullptr && o->Target->BoneTransform != nullptr)
					{
						EVector3 Direction, Angle;

						Vector(0.0f, 20.0f, 0.0f, p);
						VectorCopy(o->Direction, Direction);
						MUModel* b = &MODELS::Data[o->Target->Type];
						b->BodyScale(ThreadIndex) = o->Target->Scale;
						VectorCopy(o->Target->Position, b->BodyOrigin(ThreadIndex));
						b->TransformPosition(ThreadIndex, o->Target->BoneTransform[b->GetBoneIndex(o->MultiUse)], p, o->StartPosition, true);
						VectorCopy(o->StartPosition, o->Position);

						o->NumTails = 0;
						mu_int32 MaxTails = EMath::Min((mu_int32)o->MaxTails, (mu_int32)o->Weapon);
						o->MaxTails = MaxTails;
						o->TargetPosition[1] = o->TargetPosition[2];
						for (mu_int32 i = 0; i < MaxTails; ++i)
						{
#ifdef USE_DUAL_QUATERNION
							o->Target->BoneTransform[b->GetBoneIndex(o->MultiUse)].TransformNormalOut(Direction, Position);
#else
							VectorRotate(Direction, o->Target->BoneTransform[b->GetBoneIndex(o->MultiUse)], Position);
#endif
							VectorAdd(o->StartPosition, Position, o->StartPosition);

							if (o->SubType % 2)
							{
								o->TargetPosition[1] += 40.0f;
							}
							else
							{
								o->TargetPosition[1] -= 40.0f;
							}
							Vector(o->TargetPosition[1], 0.0f, 0.0f, Angle);
							AngleMatrix(Angle, Matrix);
							Vector(0.0f, 0.0f, o->TargetPosition[0], p);
							VectorRotate(p, Matrix, Position);
							VectorAdd(o->StartPosition, Position, o->Position);

							CreateTail(o, o->Target->BoneTransform[b->GetBoneIndex(o->MultiUse)]);

							o->TargetPosition[0] -= 0.15f;

							if (o->PKKey == -1)
								Direction[1] -= 0.1f;

							if ((i % 2) == 0)
							{
								CreateSprite(BITMAP_FLARE, o->StartPosition, o->Light[0] * 0.5f, o->Light, nullptr);
							}
						}
						++o->MaxTails;
					}
					else
					{
						o->LifeTime = 1;
					}
					if (o->LifeTime < 7)
					{
						o->Light[0] /= 1.5f;
						o->Light[1] /= 1.5f;
						o->Light[2] /= 1.5f;
					}
				}
				else if ((mu_int32)o->NumTails < (mu_int32)o->MaxTails - 1)
				{
					if (o->SubType == 0)
					{
						for (mu_int32 i = 1; i < o->MultiUse; ++i)
						{
							AngleMatrix(o->Angle, Matrix);

							CreateTail(o, Matrix);
							Vector(0.0f, o->Velocity, 0.0f, p);
							VectorRotate(p, Matrix, Position);
							VectorAdd(o->StartPosition, Position, o->StartPosition);

							o->Position[0] = o->StartPosition[0];
							o->Position[1] = o->StartPosition[1];
							o->Position[2] = o->StartPosition[2];
							o->Velocity -= 2.0f;
						}
						o->MultiUse += 2;
					}
					else if ((o->SubType >= 1 && o->SubType <= 4)
						|| (o->SubType >= 11 && o->SubType <= 13)
						)
					{
						if (o->Weapon <= 0)
						{
							EVector3 Angle;
							for (mu_int32 i = 1; i < o->MultiUse; ++i)
							{
								AngleMatrix(o->Angle, Matrix);
								CreateTail(o, Matrix);
								Vector(0.0f, o->Velocity, 0.0f, p);
								VectorRotate(p, Matrix, Position);
								VectorAdd(o->StartPosition, Position, o->StartPosition);

								if (o->SubType == 2 || o->SubType == 4)
								{
									o->TargetPosition[1] -= 20.0f;
								}
								else if (o->SubType >= 11 || o->SubType <= 13)
								{
									o->TargetPosition[1] -= 20.0f;
								}
								else
								{
									o->TargetPosition[1] += 20.0f;
								}
								Vector(0.0f, o->TargetPosition[1], o->Angle[2], Angle);
								AngleMatrix(Angle, Matrix);
								Vector(0.0f, 0.0f, o->TargetPosition[0], p);
								VectorRotate(p, Matrix, Position);

								if (o->SubType == 3 || o->SubType == 4)
								{
									VectorSubtract(o->StartPosition, Position, Position);
								}
								else
								{
									VectorAdd(o->StartPosition, Position, Position);
								}

								o->Position[0] = Position[0];
								o->Position[1] = Position[1];
								o->Position[2] = Position[2];
								o->Velocity -= 2.0f;
								o->TargetPosition[0] -= 2.5f;
							}
							o->MultiUse += 2;
						}
						else
						{
							--o->Weapon;
						}
					}
				}

				if ((o->SubType <= 4
					|| (o->SubType >= 11 && o->SubType <= 13)
					) && o->LifeTime < 10)
				{
					o->Light[0] /= 1.3f;
					o->Light[1] /= 1.3f;
					o->Light[2] /= 1.3f;
				}
				break;

			case BITMAP_FLASH:
				if (o->SubType <= 3 || o->SubType == 5)
				{
					if (o->SubType == 2)
					{
						o->Angle[2] += GetLargeRand(10) + 10.0f;
					}

					o->Velocity += 10.0f;
					if (o->Features.Get(JOINT::eJFS_CREATETAILS) == true)
					{
						AngleMatrix(o->Angle, Matrix);
						CreateTail(o, Matrix);
					}

					o->Velocity += 10.0f;
					AngleMatrix(o->HeadAngle, Matrix);
					Vector(0.0f, -o->Velocity, 0.0f, p);
					VectorRotate(p, Matrix, Position);
					VectorAdd(o->Position, Position, o->Position);

					Height = RequestTerrainHeight(o->Position[0], o->Position[1]) - o->MultiUse;
					if (o->Position[2] < Height)
					{
						o->Position[2] = Height;
						if (o->Features.Get(JOINT::eJFS_CREATETAILS) == true &&
							o->PKKey != 1)
						{
							if (o->SubType < 2)
							{
								if (o->Target != nullptr)
								{
									MUModel* b = &MODELS::Data[MODEL_SHADOW_BODY];
									b->Animation(ThreadIndex, ThreadInfo.BoneTransform, 0.0f, 0.0f, 0, o->Target->Angle, o->Target->HeadAngle, false, true);
									b->Transform(ThreadIndex, o->Target->BoundingBoxMin, o->Target->BoundingBoxMax, &o->Target->OBB, false);

									if (o->SubType == 0)
									{
										b->RenderMeshEffect(ThreadIndex, ThreadInfo.BoneTransform, 0, MODEL_SKIN_SHELL);
									}
									else if (o->SubType == 1)
									{
										b->RenderMeshEffect(ThreadIndex, ThreadInfo.BoneTransform, 0, MODEL_SKIN_SHELL, 1);
									}
								}
							}
							else
							{
								if (o->SubType == 5)
								{
									Vector(1.0f, 0.8f, 0.3f, Light);
								}
								else
								{
									Vector(0.3f, 0.8f, 1.0f, Light);
								}
								CreateEffect(ThreadIndex, BITMAP_SHOCK_WAVE, o->Position, o->Angle, Light, 4);
							}
						}
						if (o->SubType == 5)
						{
							o->PKKey = 1;
							o->Velocity = -20.0f;
						}
						else
						{
							o->Features.Clear(JOINT::eJFS_CREATETAILS);
						}

						o->Light[0] /= 1.2f;
						o->Light[1] /= 1.2f;
						o->Light[2] /= 1.2f;
					}
					if (o->SubType < 2)
					{
						CreateSprite(BITMAP_FLARE, o->Position, 1.0f, o->Light, nullptr, (mu_float)(GetLargeRand(360)));
					}
					else if (o->SubType == 5)
					{
						CreateSprite(BITMAP_FLARE, o->Position, 2.5f, o->Light, nullptr, (mu_float)(GetLargeRand(360)));
					}
					else
					{
						Vector(0.0f, 1.0f, 1.0f, Light);
						CreateSprite(BITMAP_SHINY + 2, o->Position, 1.6f, Light, nullptr, (mu_float)(GetLargeRand(360)));
					}
				}
				else if (o->SubType == 4)
				{
					if (o->Target == nullptr)
					{
						o->LifeTime = 1;
						break;
					}
					oh->Flags.Set(o->Target->Flags.GetDirect(OBJECT::eOF_LIVE));
					VectorCopy(o->Target->Position, o->Position);

					if (o->LifeTime < 15)
					{
						o->Light[0] /= 1.3f;
						o->Light[1] /= 1.3f;
						o->Light[2] /= 1.3f;
					}
				}
				else if (o->SubType == 6)
				{
					mu_int32 fr = GetLargeRand(5) - 2;
					o->Direction[1] += fr;
					o->Direction[2] -= 10.0f;

					AngleMatrix(o->Angle, Matrix);
					VectorRotate(o->Direction, Matrix, Position);
					VectorAdd(o->Position, Position, o->Position);

					if ((o->Weapon - o->LifeTime) < 10)
					{
						CreateParticle(ThreadIndex, BITMAP_EXPLOTION + 1, o->StartPosition, o->Angle, o->Light, 0, 3.0f);
						o->Scale = 100.0f;
					}
					else
					{
						o->Scale = 40.0f;
					}

					mu_float height = RequestTerrainHeight(o->Position[0], o->Position[1]);
					if (o->Position[2] <= height)
					{
						EVector3 tPos;
						VectorCopy(o->Position, tPos);
						CreateJoint(ThreadIndex, BITMAP_JOINT_THUNDER, o->Position, o->Position, o->Angle, 4, nullptr, 60.0f);
						Vector(1.0f, 1.0f, 1.0f, Light);
						if (o->Direction[2] <= 0) o->Direction[2] = EMath::Sin(o->Velocity) * 20.0f;
						o->Position[2] = height + 5.0f;

						tPos[2] = o->Position[2] + 30.0f;
						CreateParticle(ThreadIndex, BITMAP_ADV_SMOKE + 1, tPos, o->Angle, o->Light, 2, 3.0f);
					}

					CreateTailAxis(o, Matrix, 1);

					if (o->LifeTime < (o->Weapon / 2))
					{
						o->Light[0] /= 1.3f;
						o->Light[1] /= 1.3f;
						o->Light[2] /= 1.3f;
					}
				}
				break;
			case BITMAP_DRAIN_LIFE_GHOST:
				{
					switch (o->SubType)
					{
					case 0:
						{
							OBJECT* pSourceObj = o->Target;
							OBJECT* pTargetObj = pSourceObj->Owner;
							EVector3 vTargetPos;
							VectorCopy(pTargetObj->Position, vTargetPos);
							vTargetPos[2] = o->TargetPosition[2];

							Distance = MoveHumming(o->Position, o->Angle, vTargetPos, o->Velocity);

							o->Velocity += 2.0f;

							if (o->LifeTime < 10)
							{
								o->Light[0] /= 1.2f;
								o->Light[1] /= 1.2f;
								o->Light[2] /= 1.2f;
								VectorCopy(o->Light, Light);
							}

							VectorCopy(o->Light, Light);
						}
						break;
					}
				}
				break;
			}

			if (o->Features.Get(JOINT::eJFS_CREATETAILS) == true)
			{
				if (o->Type == BITMAP_JOINT_ENERGY && o->SubType == 54)
					CreateTail(o, Matrix, true);
				else
					CreateTail(o, Matrix);
			}

			--o->LifeTime;

			if (o->LifeTime <= 0)
			{
				g_JointsDeleteCount.fetch_add(1);

				oh->Flags.Clear(JOINT::eJF_LIVE);
			}
			else if (0 != (o->LifeTime % 12))
			{
				switch (o->Type)
				{
				case BITMAP_FLARE:
				case BITMAP_FLARE_BLUE:
					if (o->SubType == 4 || o->SubType == 12)
					{
						goto REPEAT_MOVE;
					}
					break;
				}
			}
		}

		q -= queueCount;
	}
}

void MUTasking::CheckJoints(const mu_uint32 ThreadIndex)
{
	mu_uint32 beginIndex, endIndex;
	GetIndexTasking(ThreadIndex, MAX_JOINTS, beginIndex, endIndex);

	for (mu_uint32 i = beginIndex; i < endIndex; ++i)
	{
		const JOINT_HEADER *oh = &g_JointsHeader[i];

		if (oh->Flags.Get(JOINT::eJF_LIVE) == false)
		{
			continue;
		}

		const JOINT *o = &g_Joints[i];

		if(o->Features.GetAny(JOINT::eJFS_FACEONE | JOINT::eJFS_FACETWO) == false)
		{
			continue;
		}

		switch(o->Type)
		{
		case BITMAP_JOINT_ENERGY:
			{
				if (o->SubType == 54 &&
					o->Target->CurrentAction != MONSTER01_ATTACK1)
				{
					continue;
				}
			}
			break;

		case BITMAP_SMOKE:
			{
				if (o->SubType == 0 &&
					o->NumTails <= 1)
				{
					continue;
				}
			}
			break;
		}

		if (o->NumTails <= 0)
		{
			continue;
		}

		EEffectCulledData &instanceData = g_EffectsCulledData[g_EffectsCulledDataCount.fetch_add(1)];
		instanceData.Type = ECastEnum(EEffectProxyEnum::eJoints);
		instanceData.Index = i;
	}
}

void MUTasking::PrepareJoints(const mu_uint32 ThreadIndex)
{
	const mu_uint32 jointsCount = g_JointsRenderCount;
	for (mu_uint32 n = ThreadIndex; n < jointsCount; n += _ThreadsCount)
	{
		JOINT *o = &g_Joints[n];

		if (o->Type == MODEL_SPEARSKILL &&
			(o->SubType == 0 || o->SubType == 4 || o->SubType == 9))
		{
			mu_float scale = 0.7f;

			if (o->NumTails > 0 && _UpdateCounter > 0)
			{
				mu_uint32 j = (o->BeginTails + (o->NumTails / 2)) % o->MaxTails;
				EVector3  Position, Light;

				if (o->Target != nullptr)
				{
					mu_float fJointHeight;

					if (o->SubType == 9)
						fJointHeight = (o->Tails[j][0][2] - (o->Target->Position[2] + 180))*0.01f;
					else
						fJointHeight = (o->Tails[j][0][2] - (o->Target->Position[2] + 50))*0.01f;

					if (fJointHeight > 0)
					{
						Vector(o->Light[0] - fJointHeight, o->Light[1] - fJointHeight, o->Light[2] - fJointHeight, Light);
					}
					else
					{
						VectorCopy(o->Light, Light);
					}
				}
				else
				{
					Light = EVector3(1.0f, 1.0f, 1.0f);
				}

				Vector(0.0f, 0.0f, 0.0f, Position);
				for (mu_int32 k = 0; k < 4; ++k)
				{
					VectorAdd(o->Tails[j][k], Position, Position);
				}
				VectorScale(Position, 0.25f, Position);

				CreateSprite(BITMAP_FLARE_BLUE, Position, scale, Light, nullptr);
			}
		}
		else if (o->Type == BITMAP_JOINT_HEALING && (o->SubType == 9 || o->SubType == 10))
		{
			if (o->Target != nullptr && _UpdateCounter > 0)
			{
				for (mu_int32 j = o->BeginTails, m = 0; m < o->NumTails; ++m, j = (j + 1) % o->MaxTails)
				{
					mu_float  scale = 0.7f;
					EVector3 Light;
					mu_float  fJointHeight = (j)*0.01f;
					VectorScale(o->Light, 0.9978f, o->Light);
					Vector(o->Light[0] - fJointHeight, o->Light[1] - fJointHeight, o->Light[2] - fJointHeight, Light);

					EVector3  Position;

					Vector(0.0f, 0.0f, 0.0f, Position);
					for (mu_int32 k = 0; k < 4; ++k)
					{
						VectorAdd(o->Tails[j][k], Position, Position);
					}
					VectorScale(Position, 0.25f, Position);

					if (o->SubType == 9) scale = 0.5f;
					CreateSprite(BITMAP_FLARE_BLUE, Position, scale, Light, nullptr);
				}
			}
		}
		else if (o->Type == BITMAP_FLARE + 1 && o->SubType == 6)
		{
			if (_UpdateCounter > 0)
			{
				EVector3  Position;
				Vector(0.0f, 0.0f, 0.0f, Position);
				for (mu_int32 k = 0; k < 4; ++k)
				{
					VectorAdd(o->Tails[o->BeginTails][k], Position, Position);
				}
				VectorScale(Position, 0.25f, Position);

				CreateSprite(BITMAP_FLARE_BLUE, Position, 0.5f, o->Light, nullptr, (mu_float)(GetLargeRand(360)), 3);
				CreateSprite(BITMAP_SHINY + 1, Position, 1.5f, o->Light, nullptr, (mu_float)(GetLargeRand(360)), 3);
			}
		}
		else if (o->Type == BITMAP_FLARE && o->SubType == 8)
		{
			if (_UpdateCounter > 0)
			{
				EVector3  Position;
				Vector(0.0f, 0.0f, 0.0f, Position);
				for (mu_int32 k = 0; k < 4; ++k)
				{
					VectorAdd(o->Tails[o->BeginTails][k], Position, Position);
				}
				VectorScale(Position, 0.25f, Position);

				CreateSprite(BITMAP_FLARE_BLUE, Position, 0.3f, o->Light, nullptr, (mu_float)(GetLargeRand(360)), 3);
				CreateSprite(BITMAP_SHINY + 1, Position, 1.0f, o->Light, nullptr, (mu_float)(GetLargeRand(360)), 3);
			}
		}
	}
}