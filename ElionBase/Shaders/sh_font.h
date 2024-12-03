#ifndef __SH_FONT_H__
#define __SH_FONT_H__

#pragma once

#define FONT_INVALIDTEXTURE (255)

MU_ALIGN(4, struct) SHFontInstance
{
	EVector4 Position;
	EVector4 UV;
	mu_uint32 Color;
	mu_uint8 TextureIndex;
};

FORCEINLINE void SetFontInstance(SHFontInstance *vertex, const EVector4 Position, const EVector4 UV, const mu_uint32 Color, const mu_uint8 TextureIndex)
{
	vertex->Position = Position;
	vertex->UV = UV;
	vertex->Color = Color;
	vertex->TextureIndex = TextureIndex;
}

MU_ALIGN(32, struct) SHFontConstant
{
	EMatrix4 PositionMatrix;
	EVector4 OutlineColor;
};

#endif