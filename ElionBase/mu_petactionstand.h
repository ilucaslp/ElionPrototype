#ifndef __MU_PETACTIONSTAND_H__
#define __MU_PETACTIONSTAND_H__

#pragma once

class MUPetActionStand : public MUPetAction
{
public:
	MUPetActionStand();
	~MUPetActionStand();

	virtual mu_boolean Release(OBJECT* obj, CHARACTER *Owner) override;

	virtual mu_boolean Move(const mu_uint32 ThreadIndex, OBJECT* obj, CHARACTER *Owner, mu_int32 targetKey, mu_uint64 tick, mu_boolean bForceRender) override;
	virtual mu_boolean Effect(const mu_uint32 ThreadIndex, OBJECT* obj, CHARACTER *Owner, mu_int32 targetKey, mu_uint64 tick, mu_boolean bForceRender) override;
};

extern MUPetActionStand g_PetActionStand;

#endif