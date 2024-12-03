#ifndef __MU_ENVIRONMENT_OBJECT_H__
#define __MU_ENVIRONMENT_OBJECT_H__

#pragma once

extern EParallelVariable<mu_int32, MAX_TASKING_THREADS> g_ActionObjectType;
extern EParallelVariable<mu_int32, MAX_TASKING_THREADS> g_ActionWorld;
extern EParallelVariable<mu_int32, MAX_TASKING_THREADS> g_ActionTime;
extern EParallelVariable<mu_float, MAX_TASKING_THREADS> g_ActionObjectVelocity;

extern mu_uint32 g_EnvironmentObjectsCount;
extern EObjectCullingData g_EnvironmentObjectsNodes[MAX_OBJECTS];
extern std::array<OBJECT, MAX_OBJECTS> g_EnvironmentObjects;
extern std::array<std::array<mu_uint16, MAX_OBJECTS / 4>, CPUCulling::BLOCK_COUNT> g_EnvironmentBlocks;
extern std::array<mu_uint16, CPUCulling::BLOCK_COUNT> g_EnvironmentBlocksCount;

void CreateObject(const mu_uint32 ThreadIndex, mu_uint32 Type, const EVector3 &Position, const EVector3 &Angle, mu_float Scale = 1.0f);

#endif