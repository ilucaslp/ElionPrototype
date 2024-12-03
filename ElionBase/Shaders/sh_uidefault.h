#ifndef __SHADER_UI_DEFAULT_H__
#define __SHADER_UI_DEFAULT_H__

#pragma once

MU_ALIGN(4, struct) SHUIDefaultVertex
{
	EVector2 Position;
	EVector2 UV;
	mu_uint32 Color;
};

#endif