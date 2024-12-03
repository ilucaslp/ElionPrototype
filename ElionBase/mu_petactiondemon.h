#ifndef __MU_PETACTIONDEMON_H__
#define __MU_PETACTIONDEMON_H__

#pragma once

class MUPetActionDemon : public MUPetAction
{
public:
	MUPetActionDemon();
	~MUPetActionDemon();

	virtual mu_boolean Release(OBJECT* obj, CHARACTER *Owner) override;

	virtual mu_boolean Move(const mu_uint32 ThreadIndex, OBJECT* obj, CHARACTER *Owner, mu_int32 targetKey, mu_uint64 tick, mu_boolean bForceRender) override;
	virtual mu_boolean Effect(const mu_uint32 ThreadIndex, OBJECT* obj, CHARACTER *Owner, mu_int32 targetKey, mu_uint64 tick, mu_boolean bForceRender) override;
};

extern MUPetActionDemon g_PetActionDemon;

#endif