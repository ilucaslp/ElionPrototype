#ifndef __SH_HELPER_H__
#define __SH_HELPER_H__

#pragma once

FORCEINLINE void SetEffectInstance(SHEffectInstance *Instance,
	const EMatrix4 &Matrix,
	const EVector4 &UV,
	const EVector2 &Size,
	const mu_uint32 Color,
	const mu_uint32 Tex)
{
#if DIRECTX_BACKEND_ENABLE == 1
	if (EVideoBackend::GetBackend() == EVideoBackendType::eDirectX)
	{
		// ... DirectX matrix layout...
		EMatrix4 m = EMath::Transpose(Matrix);
		mu_memcpy(Instance->Matrix, &m, sizeof(m));
	}
	else
#endif
	{
		mu_memcpy(Instance->Matrix, &Matrix, sizeof(Matrix));
	}

#ifdef USE_COMPRESSED_INSTANCES
	Instance->UV[0] = ESimdConvertFloatToHalf(UV[0]);
	Instance->UV[1] = ESimdConvertFloatToHalf(UV[1]);
	Instance->UV[2] = ESimdConvertFloatToHalf(UV[2]);
	Instance->UV[3] = ESimdConvertFloatToHalf(UV[3]);

	Instance->Color = Color;

	Instance->Size[0] = ESimdConvertFloatToHalf(Size[0]);
	Instance->Size[1] = ESimdConvertFloatToHalf(Size[1]);
#else
	Instance->UV = UV;
	Instance->Color = Color;
	Instance->Size = Size;
#endif

	Instance->Texture = Tex;
}

FORCEINLINE void SetEffectInstance(SHEffectInstance *Instance,
	const EDualQuat &Matrix,
	const EVector4 &UV,
	const EVector2 &Size,
	const mu_uint32 Color,
	const mu_uint32 Tex)
{
#if DIRECTX_BACKEND_ENABLE == 1
	if (EVideoBackend::GetBackend() == EVideoBackendType::eDirectX)
	{
		// ... DirectX matrix layout...
		Instance->Matrix[0][0] = Matrix.Rotation[0];
		Instance->Matrix[1][0] = Matrix.Rotation[1];
		Instance->Matrix[2][0] = Matrix.Rotation[2];
		Instance->Matrix[3][0] = Matrix.Rotation[3];
		Instance->Matrix[0][1] = Matrix.Position[0];
		Instance->Matrix[1][1] = Matrix.Position[1];
		Instance->Matrix[2][1] = Matrix.Position[2];
		Instance->Matrix[3][1] = Matrix.Scale;
	}
	else
#endif
	{
		mu_memcpy(Instance->Matrix, &Matrix, sizeof(Matrix));
	}

#ifdef USE_COMPRESSED_INSTANCES
	Instance->UV[0] = ESimdConvertFloatToHalf(UV[0]);
	Instance->UV[1] = ESimdConvertFloatToHalf(UV[1]);
	Instance->UV[2] = ESimdConvertFloatToHalf(UV[2]);
	Instance->UV[3] = ESimdConvertFloatToHalf(UV[3]);

	Instance->Color = Color;

	Instance->Size[0] = ESimdConvertFloatToHalf(Size[0]);
	Instance->Size[1] = ESimdConvertFloatToHalf(Size[1]);
#else
	Instance->UV = UV;
	Instance->Color = Color;
	Instance->Size = Size;
#endif

	Instance->Texture = Tex;
}

#endif