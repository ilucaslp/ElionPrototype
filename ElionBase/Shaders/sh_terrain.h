#ifndef __TERRAIN_SHADER_H__
#define __TERRAIN_SHADER_H__

#pragma once

MU_ALIGN(4, struct) SHTerrainInstance
{
	mu_uint32 Position;
#ifdef USE_COMPRESSED_VERTICES
	mu_uint16 UV[2];
#else
	EVector2 UV;
#endif
};

FORCEINLINE void SetTerrainInstance(SHTerrainInstance *instance, const mu_uint32 Position, const mu_float U, const mu_float V)
{
	instance->Position = Position;
#ifdef USE_COMPRESSED_VERTICES
	instance->UV[0] = ESimdConvertFloatToHalf(U);
	instance->UV[1] = ESimdConvertFloatToHalf(V);
#else
	instance->UV = EVector2(U,V);
#endif
}

MU_ALIGN(4, struct) SHGrassInstance
{
#ifdef USE_COMPRESSED_VERTICES
	mu_uint16 Position[2];
#else
	EVector2 Position;
#endif
	mu_uint32 Data;
};

FORCEINLINE void SetGrassInstance(SHGrassInstance *instance, const EVector2 Position, const mu_uint32 Data)
{
#ifdef USE_COMPRESSED_VERTICES
	instance->Position[0] = ESimdConvertFloatToHalf(Position[0]);
	instance->Position[1] = ESimdConvertFloatToHalf(Position[1]);
#else
	instance->Position = Position;
#endif
	instance->Data = Data;
}

MU_ALIGN(4, struct) SHWaterVertex
{
#ifdef USE_COMPRESSED_VERTICES
	mu_uint16 Position[4];
	mu_uint16 Normal[4];
#else
	EVector3 Position;
	EVector3 Normal;
#endif
};

FORCEINLINE void SetWaterTerrainVertex(SHWaterVertex *vertex, const EVector3 Position, const EVector3 Normal)
{
#ifdef USE_COMPRESSED_VERTICES
	vertex->Position[0] = ESimdConvertFloatToHalf(Position[0]);
	vertex->Position[1] = ESimdConvertFloatToHalf(Position[1]);
	vertex->Position[2] = ESimdConvertFloatToHalf(Position[2]);
	vertex->Position[3] = ESimdConvertFloatToHalf(1.0f);
	vertex->Normal[0] = ESimdConvertFloatToHalf(Normal[0]);
	vertex->Normal[1] = ESimdConvertFloatToHalf(Normal[1]);
	vertex->Normal[2] = ESimdConvertFloatToHalf(Normal[2]);
#else
	vertex->Position = Position;
	vertex->Normal = Normal;
#endif
}

#endif