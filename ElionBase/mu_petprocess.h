#ifndef __MU_PETPROCESS_H__
#define __MU_PETPROCESS_H__

#pragma once

class PetInfo
{
public:
	PetInfo()
	{
		Type = -1;
		BlendMesh = -1;
		Scale = 0.0f;

		Count = 0;
		Actions = nullptr;
		Speed = nullptr;
	}

	~PetInfo()
	{
		MU_SAFE_DELETE_ARRAY(Actions);
		MU_SAFE_DELETE_ARRAY(Speed);
	}

public:
	mu_int16 Type;
	mu_int16 Index;
	mu_int32 BlendMesh;

	mu_float Scale;

	mu_uint16 Count;
	mu_uint8 *Actions;
	mu_float *Speed;
};

#include "mu_petobject.h"

extern std::map<mu_int32, PetInfo*> g_PetInfo;
extern std::map<mu_int32, MUPetAction*> g_PetActions;

extern MUCriticalSectionSpinLock g_PetObjectsLock;
extern std::array<MUPetObject, MAX_PETS> g_PetObjects;
extern std::array<mu_uint16, MAX_PETS> g_PetObjectsFree;
extern std::array<mu_uint16, MAX_PETS + MAX_TASKING_THREADS> g_PetObjectsQueue;
extern mu_uint16 g_PetObjectsCurCount;
extern mu_uint16 g_PetObjectsFreeCount;

namespace PetProcess
{
	enum
	{
		PC4_ELF = 1,
		PC4_TEST,
		PC4_SATAN,
		XMAS_RUDOLPH,
		PANDA,
		UNICORN,
		SKELETON,
	};

	mu_boolean Load(const mu_text *filename);
	void Destroy();

	mu_boolean IsPet(mu_int32 key);
	MUPetAction *FindAction(mu_int32 key);

	mu_boolean CreatePet(mu_int32 ItemType, mu_int32 ModelType, EVector3 &Position, CHARACTER *Owner, mu_int32 SubType = 0, mu_int32 LinkBone = 0);
	void DeletePet(CHARACTER *Owner, mu_int32 ItemType = -1, mu_boolean AllDelete = false);
};

#endif