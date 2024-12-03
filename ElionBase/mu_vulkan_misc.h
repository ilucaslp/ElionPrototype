#ifndef __MU_VULKAN_MISC_H__
#define __MU_VULKAN_MISC_H__

#pragma once

#if defined(_M_ARM) || defined(__arm__) || defined(_M_ARM64) || defined(__aarch64__)
FORCEINLINE void ESimdArmCastTo8x8x2(const uint32x4_t &in, uint8x8x2_t &out)
{
	const uint8x16_t &fr = vreinterpretq_u8_u32(in);
	out.val[0] = vget_low_u8(fr);
	out.val[1] = vget_high_u8(fr);
}
#endif

FORCEINLINE void ConvertLightToRGB8(const std::array<EVector3, MAX_TERRAIN_SIZE> &primaryTerrainLight, mu_uint32 *dataOut)
{
	static const ESimd128 MinValue = ESimdSet1(0.0f);
	static const ESimd128 MaxValue = ESimdSet1(1.0f);
	static const ESimd128 RGB8Mul = ESimdSet1(255.0f);
#if defined(_M_ARM) || defined(__arm__) || defined(_M_ARM64) || defined(__aarch64__)
	static const uint8x8_t Mask = { 0, 4, 8, 12, 0, 0, 0, 0 };
#else
	static const __m128i Mask = _mm_setr_epi8(0, 4, 8, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
#endif
	const float *dataIn = (float*)primaryTerrainLight.data();
	const float *dataEnd = dataIn + (MAX_TERRAIN_SIZE * 3);
	mu_uint32 savedByte = 0x00;

	ESimd128 data;
#if defined(_M_ARM) || defined(__arm__) || defined(_M_ARM64) || defined(__aarch64__)
	uint8x8x2_t conv;
	uint8x8_t masked;
#else
	const mu_uint32 roundMask = _MM_GET_ROUNDING_MODE();
	_MM_SET_ROUNDING_MODE(_MM_ROUND_DOWN);
	__m128i masked;
#endif

	for (; dataIn != dataEnd;)
	{
#if __ANDROID__
		__builtin_prefetch(dataIn, 0, 1);
#else
		PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, dataIn);
#endif

		data = ESimdSetR(dataIn[0], dataIn[1], dataIn[2], dataIn[3]);
		data = ESimdClamp(data, MinValue, MaxValue);
		data = ESimdMul(data, RGB8Mul);
#if defined(_M_ARM) || defined(__arm__) || defined(_M_ARM64) || defined(__aarch64__)
		ESimdArmCastTo8x8x2(vcvtq_u32_f32(data), conv);
		masked = vtbl2_u8(conv, Mask);
		vst1_lane_u32(dataOut, vreinterpret_u32_u8(masked), 0);
#else
		masked = _mm_shuffle_epi8(_mm_cvtps_epi32(data), Mask);
		*dataOut = _mm_extract_epi32(masked, 0);
#endif
		savedByte = *dataOut >> 24;
		++dataOut;
		dataIn += 4;

		data = ESimdSetR(dataIn[0], dataIn[1], dataIn[2], dataIn[3]);
		data = ESimdClamp(data, MinValue, MaxValue);
		data = ESimdMul(data, RGB8Mul);
#if defined(_M_ARM) || defined(__arm__) || defined(_M_ARM64) || defined(__aarch64__)
		ESimdArmCastTo8x8x2(vcvtq_u32_f32(data), conv);
		masked = vtbl2_u8(conv, Mask);
		vst1_lane_u32(dataOut, vreinterpret_u32_u8(masked), 0);
#else
		masked = _mm_shuffle_epi8(_mm_cvtps_epi32(data), Mask);
		*dataOut = _mm_extract_epi32(masked, 0);
#endif
		mu_uint32 newBytes = *dataOut >> 16;
		*dataOut <<= 8;
		*dataOut |= savedByte;
		savedByte = newBytes;
		++dataOut;
		dataIn += 4;

		data = ESimdSetR(dataIn[0], dataIn[1], dataIn[2], dataIn[3]);
		data = ESimdClamp(data, MinValue, MaxValue);
		data = ESimdMul(data, RGB8Mul);
#if defined(_M_ARM) || defined(__arm__) || defined(_M_ARM64) || defined(__aarch64__)
		ESimdArmCastTo8x8x2(vcvtq_u32_f32(data), conv);
		masked = vtbl2_u8(conv, Mask);
		vst1_lane_u32(dataOut, vreinterpret_u32_u8(masked), 0);
#else
		masked = _mm_shuffle_epi8(_mm_cvtps_epi32(data), Mask);
		*dataOut = _mm_extract_epi32(masked, 0);
#endif
		newBytes = *dataOut >> 8;
		*dataOut <<= 16;
		*dataOut |= savedByte;
		++dataOut;
		*dataOut = newBytes;
		++dataOut;
		dataIn += 4;

		data = ESimdSetR(dataIn[0], dataIn[1], dataIn[2], dataIn[3]);
		data = ESimdClamp(data, MinValue, MaxValue);
		data = ESimdMul(data, RGB8Mul);
#if defined(_M_ARM) || defined(__arm__) || defined(_M_ARM64) || defined(__aarch64__)
		ESimdArmCastTo8x8x2(vcvtq_u32_f32(data), conv);
		masked = vtbl2_u8(conv, Mask);
		vst1_lane_u32(dataOut, vreinterpret_u32_u8(masked), 0);
#else
		masked = _mm_shuffle_epi8(_mm_cvtps_epi32(data), Mask);
		*dataOut = _mm_extract_epi32(masked, 0);
#endif
		savedByte = *dataOut >> 24;
		++dataOut;
		dataIn += 4;

		data = ESimdSetR(dataIn[0], dataIn[1], dataIn[2], dataIn[3]);
		data = ESimdClamp(data, MinValue, MaxValue);
		data = ESimdMul(data, RGB8Mul);
#if defined(_M_ARM) || defined(__arm__) || defined(_M_ARM64) || defined(__aarch64__)
		ESimdArmCastTo8x8x2(vcvtq_u32_f32(data), conv);
		masked = vtbl2_u8(conv, Mask);
		vst1_lane_u32(dataOut, vreinterpret_u32_u8(masked), 0);
#else
		masked = _mm_shuffle_epi8(_mm_cvtps_epi32(data), Mask);
		*dataOut = _mm_extract_epi32(masked, 0);
#endif
		newBytes = *dataOut >> 16;
		*dataOut <<= 8;
		*dataOut |= savedByte;
		savedByte = newBytes;
		++dataOut;
		dataIn += 4;

		data = ESimdSetR(dataIn[0], dataIn[1], dataIn[2], dataIn[3]);
		data = ESimdClamp(data, MinValue, MaxValue);
		data = ESimdMul(data, RGB8Mul);
#if defined(_M_ARM) || defined(__arm__) || defined(_M_ARM64) || defined(__aarch64__)
		ESimdArmCastTo8x8x2(vcvtq_u32_f32(data), conv);
		masked = vtbl2_u8(conv, Mask);
		vst1_lane_u32(dataOut, vreinterpret_u32_u8(masked), 0);
#else
		masked = _mm_shuffle_epi8(_mm_cvtps_epi32(data), Mask);
		*dataOut = _mm_extract_epi32(masked, 0);
#endif
		newBytes = *dataOut >> 8;
		*dataOut <<= 16;
		*dataOut |= savedByte;
		++dataOut;
		*dataOut = newBytes;
		++dataOut;
		dataIn += 4;
	}

#if defined(_M_ARM) || defined(__arm__) || defined(_M_ARM64) || defined(__aarch64__)
#else
	_MM_SET_ROUNDING_MODE(roundMask);
#endif
}

#endif