#ifndef __MU_MATH_HELPER_H__
#define __MU_MATH_HELPER_H__

#pragma once

#define Vector(a,b,c,d) (d = EVector3(a, b, c))
#define VectorAvg(a) ( ( (a)[0] + (a)[1] + (a)[2] ) / 3 )
#define VectorSubtract(a,b,c) (c = (a) - (b))
#define VectorAdd(a,b,c) (c = (a) + (b))
#define VectorCopy(a,b) (b = a)
#define QuaternionCopy(a,b) (b = a)
#define VectorScale(a,b,c) (c = (a) * (b))
#define DotProduct(x,y) (EMath::Dot(x, y))
#define VectorFill(a,b) ((a)=(b))
#define VectorNormalize(a) (a = EMath::Normalize(a))
#define VectorLength(a) EMath::Length(a)
#define VectorMul(a,b,c) (c = (a)*(b))
#define AngleMatrix(a, b) (b = EMath::EulerToMatrix(a))
#define VectorRotate(a, b, c) (c = EMath::TransformNormal(b, a))
#define VectorMulF(a, b, c) (c = (a) * (b))
#define VectorDivF(a, b, c) (c = (a) / (b))
#define VectorDivFSelf(a, b) (a /= b)
#define VectorDistNormalize(a, b, c) (c = EMath::Normalize(a - b))
#define VectorMA(a, b, c, d) (d = (a) + ((c)*(b)))
#define VectorTransform(a, b, c) (c = EMath::Transform(b, a))
#define R_ConcatTransforms(a, b, c) (c = a * b)
#define AngleQuaternion(a, b) (b = EQuat(a))
#define CrossProduct(a, b, c) (c = EMath::Cross(a, b))
#define Q_PI EMath::PI
#define Q_RAD 0.017453292519943294f
#define Q_EULER 57.29577951308232089f
#define EQUAL_EPSILON 0.001

#if 1
#define QuaternionCompare(a, b) (a == b)
#define QuaternionSlerp(a, b, c, d) (d = EMath::Slerp(a, b, c))
#else
EINLINE mu_boolean QuaternionCompare(EQuat &v1, EQuat &v2)
{
	for (mu_int32 i = 0; i < 4; ++i)
	{
		if (EMath::Abs(v1[i] - v2[i]) > EQUAL_EPSILON)
			return false;
	}

	return true;
}

EINLINE void QuaternionSlerp(const EQuat &p, EQuat &q, mu_float t, EQuat &qt)
{
	mu_float omega, cosom, sinom, sclp, sclq;

	// decide if one of the quaternions is backwards
	mu_float a = 0;
	mu_float b = 0;
	for (mu_int32 i = 0; i < 4; ++i)
	{
		a += (p[i] - q[i])*(p[i] - q[i]);
		b += (p[i] + q[i])*(p[i] + q[i]);
	}

	if (a > b)
	{
		q = -q;
	}

	cosom = p[0] * q[0] + p[1] * q[1] + p[2] * q[2] + p[3] * q[3];

	if ((1.0 + cosom) > 0.00000001)
	{
		if ((1.0 - cosom) > 0.00000001)
		{
			omega = EMath::Acos(cosom);
			sinom = EMath::Sin(omega);
			sclp = EMath::Sin((1.0 - t)*omega) / sinom;
			sclq = EMath::Sin(t*omega) / sinom;
		}
		else
		{
			sclp = 1.0 - t;
			sclq = t;
		}

		qt = sclp * p + sclq * q;
	}
	else
	{
		qt[0] = -p[1];
		qt[1] = p[0];
		qt[2] = -p[3];
		qt[3] = p[2];
		sclp = EMath::Sin((1.0 - t) * 0.5 * Q_PI);
		sclq = EMath::Sin(t * 0.5 * Q_PI);
		qt = sclp * p + sclq * qt;
	}
}
#endif

EINLINE void FaceNormalize(EVector3 &v1, EVector3 &v2, EVector3 &v3, EVector3 &Normal)
{
	Normal = EMath::Normalize(EMath::Cross(v2 - v1, v3 - v1));
}

EINLINE mu_uint32 FloorLog2(mu_uint32 value)
{
	mu_uint32 pos = 0;

	if (value >= 1 << 16)
	{
		value >>= 16; pos += 16;
	}
	if (value >= 1 << 8)
	{
		value >>= 8; pos += 8;
	}
	if (value >= 1 << 4)
	{
		value >>= 4; pos += 4;
	}
	if (value >= 1 << 2)
	{
		value >>= 2; pos += 2;
	}
	if (value >= 1 << 1)
	{
		pos += 1;
	}

	return (value == 0) ? 0 : pos;
}

EINLINE mu_uint32 CountLeadingZeros(mu_uint32 Value)
{
	if (Value == 0)
	{
		return 32;
	}

	return 31 - FloorLog2(Value);
}

EINLINE mu_uint32 CeilLogTwo(mu_uint32 Arg)
{
	mu_int32 Bitmask = ((mu_int32)(CountLeadingZeros(Arg) << 26)) >> 31;
	return (32 - CountLeadingZeros(Arg - 1)) & (~Bitmask);
}

EINLINE mu_uint32 RoundUpToPowerOfTwo(mu_uint32 Arg)
{
	return 1 << CeilLogTwo(Arg);
}

EINLINE void SETLIMITS(mu_float& VALUE_, const mu_float MAX_, const mu_float MIN_)
{
	if (VALUE_ > MAX_)
	{
		VALUE_ = MAX_;
	}
	else if (VALUE_ < MIN_)
	{
		VALUE_ = MIN_;
	}
}

EINLINE void LInterpolationF(mu_float& fout, const mu_float& f01, const mu_float& f02, const mu_float fWeight)
{
	fout = f01 + ((f02 - f01) * fWeight);
}

EINLINE void VectorInterpolation(EVector3& v_out, const EVector3& v_1, const EVector3& v_2, const mu_float fWeight)
{
	LInterpolationF(v_out[0], v_1[0], v_2[0], fWeight);
	LInterpolationF(v_out[1], v_1[1], v_2[1], fWeight);
	LInterpolationF(v_out[2], v_1[2], v_2[2], fWeight);
}

EINLINE void VectorInterpolation_F(EVector3& v_out,
	const EVector3& v_1,
	const EVector3& v_2,
	const mu_float fArea,
	const mu_float fCurrent)
{
	mu_float	fWeight = fCurrent / fArea;

	VectorInterpolation(v_out, v_1, v_2, fWeight);
}

EINLINE void VectorInterpolation_W(EVector3& v_out,
	const EVector3& v_1,
	const EVector3& v_2,
	const mu_float fWeight)
{
	VectorInterpolation(v_out, v_1, v_2, fWeight);
}

EINLINE void VectorDistanceInterpolation_F(EVector3& v_out,
	const EVector3& v_Distance,
	const mu_float fRate)
{
	v_out[0] = v_Distance[0] * fRate;
	v_out[1] = v_Distance[1] * fRate;
	v_out[2] = v_Distance[2] * fRate;
}

EINLINE mu_float VectorDistance2D(EVector3 &va, EVector3 &vb)
{
	return EMath::Length(EVector2(va) - EVector2(vb));
}

EINLINE mu_float VectorDistance3D(const EVector3& vPosStart, const EVector3& vPosEnd)
{
	EVector3	v3Dist;
	VectorSubtract(vPosEnd, vPosStart, v3Dist);
	return EMath::Sqrt(v3Dist[0] * v3Dist[0] + v3Dist[1] * v3Dist[1] + v3Dist[2] * v3Dist[2]);
}

EINLINE void VectorDistance3D_Dir(const EVector3& vPosStart, const EVector3& vPosEnd, EVector3 &vDirDist)
{
	VectorSubtract(vPosStart, vPosEnd, vDirDist);
}

EINLINE mu_float VectorDistance3D_DirDist(const EVector3& vPosStart, const EVector3& vPosEnd, EVector3& vOut)
{
	VectorSubtract(vPosEnd, vPosStart, vOut);
	return EMath::Sqrt(vOut[0] * vOut[0] + vOut[1] * vOut[1] + vOut[2] * vOut[2]);
}

constexpr mu_uint32 ComputeBitsNeeded(const mu_uint32 ValuesCount)
{
	if (ValuesCount == 0) return 0;
	mu_uint32 bitsCount = 1;
	mu_uint32 n = 1;
	while (n < ValuesCount - 1)
	{
		n <<= 1;
		n |= 1;
		++bitsCount;
	}
	return bitsCount;
}

#endif