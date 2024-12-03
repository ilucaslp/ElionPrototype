#ifndef __MU_OPERATES_H__
#define __MU_OPERATES_H__

#pragma once

#define MAX_OPERATES 200

struct MUOperates
{
	mu_uint32 Index;
	OBJECT *Owner;
};

extern mu_uint16 g_OperatesCount;
extern MUOperates g_Operates[MAX_OPERATES];

void CreateOperate(mu_uint32 objectIndex, OBJECT *Owner);

#endif