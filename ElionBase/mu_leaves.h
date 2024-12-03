#ifndef __MU_LEAVES_H__
#define __MU_LEAVES_H__

#pragma once

extern std::array<mu_uint16, MAX_LEAVES_DOUBLE + MAX_TASKING_THREADS> g_LeavesQueue;

extern std::array<PARTICLE_HEADER, MAX_LEAVES_DOUBLE> LeavesHeader;
extern std::array<PARTICLE, MAX_LEAVES_DOUBLE> Leaves;
extern mu_int32 MaxLeaves;

extern mu_int32 RainTarget;
extern EParallelVariable<mu_int32, MAX_TASKING_THREADS> RainCurrent;

extern EParallelVariable<mu_int32, MAX_TASKING_THREADS> RainSpeed;
extern EParallelVariable<mu_int32, MAX_TASKING_THREADS> RainAngle;
extern EParallelVariable<mu_int32, MAX_TASKING_THREADS> RainPosition;

extern mu_int32 weather;

#endif