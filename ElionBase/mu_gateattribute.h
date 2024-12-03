#ifndef __MU_GATEATTRIBUTE_H__
#define __MU_GATEATTRIBUTE_H__

#pragma once

typedef struct
{
	mu_uint8 Flag;
	mu_uint8 Map;
	mu_uint8 x1;
	mu_uint8 y1;
	mu_uint8 x2;
	mu_uint8 y2;
	mu_uint16 Target;
	mu_uint8 Angle;
	mu_uint16 Level;
	mu_uint16 m_wMaxLevel;
} GATE_ATTRIBUTE;

extern GATE_ATTRIBUTE GateAttribute[MAX_GATES];

mu_boolean OpenGateScript(const mu_text *filename);

#endif