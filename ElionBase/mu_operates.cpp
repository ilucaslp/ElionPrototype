#include "stdafx.h"
#include "mu_operates.h"

mu_uint16 g_OperatesCount = 0;
MUOperates g_Operates[MAX_OPERATES];

void CreateOperate(mu_uint32 objectIndex, OBJECT *Owner)
{
	if (g_OperatesCount < MAX_OPERATES)
	{
		MUOperates &operate = g_Operates[g_OperatesCount++];
		operate.Index = objectIndex;
		operate.Owner = Owner;
	}
}