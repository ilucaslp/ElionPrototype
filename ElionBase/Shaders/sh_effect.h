#ifndef __SHADER_EFFECTS_H__
#define __SHADER_EFFECTS_H__

#pragma once

struct SHEffectInstance
{
	SHEffectInstance() {}

	EFloat Matrix[4][4];
#ifdef USE_COMPRESSED_INSTANCES
	mu_uint16 UV[4];
	mu_uint16 Size[2];
#else
	EVector4 UV;
	EVector2 Size;
#endif
	mu_uint32 Color;
	mu_uint32 Texture;
};

#endif