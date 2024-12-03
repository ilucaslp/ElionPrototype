#include "stdafx.h"
#include "mu_leaves.h"

std::array<mu_uint16, MAX_LEAVES_DOUBLE + MAX_TASKING_THREADS> g_LeavesQueue;

std::array<PARTICLE_HEADER, MAX_LEAVES_DOUBLE> LeavesHeader;
std::array<PARTICLE, MAX_LEAVES_DOUBLE> Leaves;
mu_int32 MaxLeaves = MAX_LEAVES;

mu_int32 RainTarget = 0;
EParallelVariable<mu_int32, MAX_TASKING_THREADS> RainCurrent(0);

EParallelVariable<mu_int32, MAX_TASKING_THREADS> RainSpeed(30);
EParallelVariable<mu_int32, MAX_TASKING_THREADS> RainAngle(0);
EParallelVariable<mu_int32, MAX_TASKING_THREADS> RainPosition(0);

mu_int32 weather = GetLargeRand(3);
