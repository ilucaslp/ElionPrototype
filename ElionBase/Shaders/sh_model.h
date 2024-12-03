#ifndef __SHADER_MODEL_H__
#define __SHADER_MODEL_H__

#pragma once

#define ENABLED_SHADOW (1.0f)
#define DISABLED_SHADOW (0.0f)

struct SHModelInstance
{
	EVector3 BodyOrigin;
#ifdef USE_COMPRESSED_INSTANCES
	mu_uint16 BodyScale[2];
	mu_uint16 Data[2]; // EnableLight, MinAlpha
	mu_uint16 Data2[4]; // BlendMeshTexCoordU, BlendMeshTexCoordV, EnableShadow, ShadowHeight
#else
	EVector2 BodyScale;
	EVector2 Data; // EnableLight, MinAlpha
	EVector4 Data2; // BlendMeshTexCoordU, BlendMeshTexCoordV, EnableShadow, ShadowHeight
#endif
	mu_uint32 BodyLight;
	mu_uint32 ShadowLight;
	mu_uint32 BoneIndex;
};

MU_ALIGN(4, struct) SHClothVertexData
{
#ifdef USE_COMPRESSED_VERTICES
	mu_uint16 Position[4];
	mu_uint16 UV[2];
	mu_uint16 Normal[4];
#else
	EVector3 Position;
	EVector2 UV;
	EVector3 Normal;
#endif
};

FORCEINLINE void SetModelInstance(SHModelInstance *Instance,
	const EVector3 &BodyOrigin,
	const EVector2 &BodyScale,
	const EVector2 Data,
	const EVector4 Data2,
	const mu_uint32 BodyLight,
	const mu_uint32 ShadowLight,
	const mu_uint32 BoneIndex)
{
	Instance->BodyOrigin = BodyOrigin;
#ifdef USE_COMPRESSED_INSTANCES
	Instance->BodyScale[0] = ESimdConvertFloatToHalf(BodyScale[0]);
	Instance->BodyScale[1] = ESimdConvertFloatToHalf(BodyScale[1]);

	Instance->Data[0] = ESimdConvertFloatToHalf(Data[0]);
	Instance->Data[1] = ESimdConvertFloatToHalf(Data[1]);

	Instance->Data2[0] = ESimdConvertFloatToHalf(Data2[0]);
	Instance->Data2[1] = ESimdConvertFloatToHalf(Data2[1]);
	Instance->Data2[2] = ESimdConvertFloatToHalf(Data2[2]);
	Instance->Data2[3] = ESimdConvertFloatToHalf(Data2[3]);

	Instance->BodyLight = BodyLight;
	Instance->ShadowLight = ShadowLight;
	Instance->BoneIndex = BoneIndex;
#else
	Instance->BodyScale = BodyScale;
	Instance->Data = Data;
	Instance->Data2 = Data2;
	Instance->BodyLight = BodyLight;
	Instance->ShadowLight = ShadowLight;
	Instance->BoneIndex = BoneIndex;
#endif
}

#endif