#ifndef __MU_PETACTIONROUND_H__
#define __MU_PETACTIONROUND_H__

#pragma once

class MUPetActionRound : public MUPetAction
{
public:
	MUPetActionRound();
	~MUPetActionRound();

	virtual mu_boolean Move(const mu_uint32 ThreadIndex, OBJECT* obj, CHARACTER *Owner, mu_int32 targetKey, mu_uint64 tick, mu_boolean bForceRender) override;
};

extern MUPetActionRound g_PetActionRound;

#endif