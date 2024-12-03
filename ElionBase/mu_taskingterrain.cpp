#include "stdafx.h"
#include "mu_tasking.h"

#ifdef USE_FAST_TERRAIN_COLLISION
SIMDGLOBALCONST ESimd128 g_RayEpsilon = { 1e-20f, 1e-20f, 1e-20f, 1e-20f };
SIMDGLOBALCONST ESimd128 g_RayNegEpsilon = { -1e-20f, -1e-20f, -1e-20f, -1e-20f };
SIMDGLOBALCONST ESimd128 g_FltMin = { -FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX };
SIMDGLOBALCONST ESimd128 g_FltMax = { FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX };

FORCEINLINE mu_boolean EVECTORCALL FastRayTriangle(const ESimd128 Origin, const ESimd128 Direction, const ESimd128 V0, const ESimd128 V1, const ESimd128 V2, mu_float &Dist)
{
	ESimd128 Zero = g_SimdZero;

	ESimd128 e1 = ESimdSub(V1, V0);
	ESimd128 e2 = ESimdSub(V2, V0);
	ESimd128 p = ESimdVector3Cross(Direction, e2);
	ESimd128 det = ESimdVector3Dot(e1, p);

	ESimd128 u, v, t;

	if (ESimdVector3GreaterOrEqual(det, g_RayEpsilon))
	{
		ESimd128 s = ESimdSub(Origin, V0);
		u = ESimdVector3Dot(s, p);

		ESimd128 NoIntersection = ESimdLess(u, Zero);
		NoIntersection = ESimdOrInt(NoIntersection, ESimdGreater(u, det));

		ESimd128 q = ESimdVector3Cross(s, e1);

		v = ESimdVector3Dot(Direction, q);

		NoIntersection = ESimdOrInt(NoIntersection, ESimdLess(v, Zero));
		NoIntersection = ESimdOrInt(NoIntersection, ESimdGreater(ESimdAdd(u, v), det));

		t = ESimdVector3Dot(e2, q);

		NoIntersection = ESimdOrInt(NoIntersection, ESimdLess(t, Zero));

		if (ESimdVector4EqualInt(NoIntersection, g_SimdAllMaskVector))
		{
			Dist = 0.0f;
			return false;
		}
	}
	else if (ESimdVector3LessOrEqual(det, g_RayNegEpsilon))
	{
		ESimd128 s = ESimdSub(Origin, V0);
		u = ESimdVector3Dot(s, p);

		ESimd128 NoIntersection = ESimdGreater(u, Zero);
		NoIntersection = ESimdOrInt(NoIntersection, ESimdLess(u, det));

		ESimd128 q = ESimdVector3Cross(s, e1);

		v = ESimdVector3Dot(Direction, q);

		NoIntersection = ESimdOrInt(NoIntersection, ESimdGreater(v, Zero));
		NoIntersection = ESimdOrInt(NoIntersection, ESimdLess(ESimdAdd(u, v), det));

		t = ESimdVector3Dot(e2, q);

		NoIntersection = ESimdOrInt(NoIntersection, ESimdGreater(t, Zero));

		if (ESimdVector4EqualInt(NoIntersection, g_SimdAllMaskVector))
		{
			Dist = 0.0f;
			return false;
		}
	}
	else
	{
		Dist = 0.0f;
		return false;
	}

	t = ESimdDiv(t, det);

	ESimdStoreFloat(&Dist, t);

	return true;
}

#ifdef USE_SIMD_COLLISION
FORCEINLINE mu_boolean DetectTerrainCollisionFast(const mu_uint32 ThreadIndex, mu_int32 x, mu_int32 y, const ESimd128 Origin, const ESimd128 Direction, const std::array<ESimd128, 4> &Vertices)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	mu_float Dist = 0.0f;
	mu_boolean Success = FastRayTriangle(Origin, Direction, Vertices[0], Vertices[1], Vertices[2], Dist);
	if (Success == false)
	{
		Success = FastRayTriangle(Origin, Direction, Vertices[0], Vertices[2], Vertices[3], Dist);
	}

	if (Success && Dist < ThreadInfo.CollisionDistance)
	{
		ThreadInfo.SelectFlag = true;
		ThreadInfo.SelectXF = x;
		ThreadInfo.SelectYF = y;
		ThreadInfo.CollisionDistance = Dist;
		ThreadInfo.CollisionPosition = MouseDirection * Dist + MousePosition;
	}

	return Success;
}
#else
#include <glm/gtx/intersect.hpp>
FORCEINLINE mu_boolean DetectTerrainCollisionFast(const mu_uint32 ThreadIndex, mu_int32 x, mu_int32 y, const std::array<EVector3, 4> &Vertices)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	mu_float Dist = 0.0f;
	EVector2 Position;
	mu_boolean Success = glm::intersectRayTriangle(MousePosition, MouseDirection, Vertices[0], Vertices[1], Vertices[2], Position, Dist);
	if (Success == false)
	{
		Success = glm::intersectRayTriangle(MousePosition, MouseDirection, Vertices[0], Vertices[2], Vertices[3], Position, Dist);
	}

	if (Success && Dist < ThreadInfo.CollisionDistance)
	{
		ThreadInfo.SelectFlag = true;
		ThreadInfo.SelectXF = x;
		ThreadInfo.SelectYF = y;
		ThreadInfo.CollisionDistance = Dist;
		ThreadInfo.CollisionPosition = MouseDirection * Dist + MousePosition;
	}

	return Success;
}
#endif
#endif

void MUTasking::TerrainCollision(const mu_uint32 ThreadIndex)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

#ifdef USE_SIMD_COLLISION
	ESimd128 Origin = ESimdSetR(MousePosition.x, MousePosition.y, MousePosition.z, 0.0f);
	ESimd128 Direction = ESimdSetR(MouseDirection.x, MouseDirection.y, MouseDirection.z, 0.0f);
#endif

	InitDetectTerrainCollision;

	mu_uint32 q = ThreadIndex;
	for (mu_uint32 t = 0; t < _ThreadsCount; ++t)
	{
		const mu_uint32 queueCount = MUThreadQueues::TerrainCollisionQueueCount[t];
		const mu_uint16 *queue = MUThreadQueues::TerrainCollisionQueue.data() + MUThreadQueues::TerrainCollisionQueueIndex[t];

		for (; q < queueCount; q += _ThreadsCount)
		{
			const mu_uint32 blockIndex = queue[q];

			const mu_uint32 x = blockIndex % TERRAIN_SIZE;
			const mu_uint32 y = blockIndex / TERRAIN_SIZE;

#ifdef USE_FAST_TERRAIN_COLLISION
			const mu_float xf = static_cast<mu_float>(x) * g_TerrainScale;
			const mu_float yf = static_cast<mu_float>(y) * g_TerrainScale;

			auto &quadHeight = g_TerrainQuadHeights[blockIndex];

#ifdef USE_SIMD_COLLISION
#if __ANDROID__
			//__builtin_prefetch(g_TerrainQuadHeights[blockIndex], 0, 1);
#else
			//PreFetchCacheLine(PF_NON_TEMPORAL_LEVEL_ALL, g_TerrainQuadHeights[blockIndex].data());
#endif
			MU_ALIGN(16, std::array<ESimd128, 4>) vertices;
			vertices[0] = ESimdSetR(xf, yf, static_cast<mu_float>(quadHeight[0]) * 0.1f, 0.0f);
			vertices[1] = ESimdSetR(xf + g_TerrainScale, yf, static_cast<mu_float>(quadHeight[1]) * 0.1f, 0.0f);
			vertices[2] = ESimdSetR(xf + g_TerrainScale, yf + g_TerrainScale, static_cast<mu_float>(quadHeight[2]) * 0.1f, 0.0f);
			vertices[3] = ESimdSetR(xf, yf + g_TerrainScale, static_cast<mu_float>(quadHeight[3]) * 0.1f, 0.0f);

			DetectTerrainCollisionFast(ThreadIndex, x, y, Origin, Direction, vertices);
#else
#if __ANDROID__
			//__builtin_prefetch(g_TerrainQuadHeights[blockIndex], 0, 1);
#else
			//PreFetchCacheLine(PF_TEMPORAL_LEVEL_1, g_TerrainQuadHeights[blockIndex]);
#endif
			std::array<EVector3, 4> vertices;
			vertices[0] = EVector3(xf, yf, static_cast<mu_float>(quadHeight[0]) * 0.1f);
			vertices[1] = EVector3(xf + g_TerrainScale, yf, static_cast<mu_float>(quadHeight[1]) * 0.1f);
			vertices[2] = EVector3(xf + g_TerrainScale, yf + g_TerrainScale, static_cast<mu_float>(quadHeight[2]) * 0.1f);
			vertices[3] = EVector3(xf, yf + g_TerrainScale, static_cast<mu_float>(quadHeight[3]) * 0.1f);

			DetectTerrainCollisionFast(ThreadIndex, x, y, vertices);
#endif
#else
			DetectTerrainCollision(ThreadIndex, x, y);
#endif
		}

		q -= queueCount;
	}
}