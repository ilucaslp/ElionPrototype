#ifndef __MU_SHADER_STRUCTURES_H__
#define __MU_SHADER_STRUCTURES_H__

#pragma once

#include "Shaders/sh_bbox.h"
#include "Shaders/sh_effect.h"
#include "Shaders/sh_font.h"
#include "Shaders/sh_uidefault.h"
#include "Shaders/sh_model.h"
#include "Shaders/sh_terrain.h"

MU_ALIGN(32, struct) SHGlobalConstantBuffer
{
	EVector4 ShadowColor;
	EVector4 ShadowPosition;
	EMatrix4 Shadow;
	EMatrix4 Game;
	EMatrix4 UI;
	EQuat BillboardQ;
	EVector3 LightPosition;
	mu_float WorldTime;
	EVector3 ChromeWave;
	mu_float Random;
	EVector2 Wave;
	mu_float WaterMove;
	mu_float WindScale;
	mu_float WindSpeed;
	mu_int32 WaterIndex;
	mu_float Padding1[2];
};

#endif