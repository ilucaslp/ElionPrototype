#ifndef __MU_PETACTION_H__
#define __MU_PETACTION_H__

#pragma once

class MUPetAction
{
public:
	MUPetAction() {}
	~MUPetAction() {}

	virtual mu_boolean Release(OBJECT* obj, CHARACTER *Owner) { return false; }

	virtual mu_boolean Move(const mu_uint32 ThreadIndex, OBJECT *obj, CHARACTER *Owner, mu_int32 targetKey, mu_uint64 tick, mu_boolean bForceRender) { return false; }
	virtual mu_boolean Effect(const mu_uint32 ThreadIndex, OBJECT *obj, CHARACTER *Owner, mu_int32 targetKey, mu_uint64 tick, mu_boolean bForceRender) { return false; }
	virtual mu_boolean Sound(const mu_uint32 ThreadIndex, OBJECT *obj, CHARACTER *Owner, mu_int32 targetKey, mu_uint64 tick, mu_boolean bForceRender) { return false; }
};

#endif