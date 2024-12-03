#ifndef __SUMMONSYSTEM_H__
#define __SUMMONSYSTEM_H__

#pragma once

class CSummonSystem
{
public:
	CSummonSystem();
	~CSummonSystem();

	void MoveEquipEffect(const mu_uint32 ThreadIndex, CHARACTER * pCharacter, mu_int32 iItemType, mu_int32 iItemLevel, mu_int32 iItemExcOption);
	void RemoveEquipEffects(CHARACTER * pCharacter);
	void RemoveEquipEffect_Summon(CHARACTER * pCharacter);

	mu_boolean SendRequestSummonSkill(const mu_uint32 ThreadIndex, mu_int32 iSkill, CHARACTER * pCharacter, OBJECT * pObject);
	void CastSummonSkill(const mu_uint32 ThreadIndex, mu_int32 iSkill, CHARACTER * pCharacter, OBJECT * pObject, mu_int32 iTargetPos_X, mu_int32 iTargetPos_Y);

	void CreateDamageOfTimeEffect(const mu_uint32 ThreadIndex, mu_int32 iSkill, OBJECT * pObject);
	void RemoveDamageOfTimeEffect(mu_int32 iSkill, OBJECT * pObject);
	void RemoveAllDamageOfTimeEffect(OBJECT * pObject);

protected:
	void CreateEquipEffect_WristRing(const mu_uint32 ThreadIndex, CHARACTER * pCharacter, mu_int32 iItemType, mu_int32 iItemLevel, mu_int32 iItemExcOption);
	void RemoveEquipEffect_WristRing(CHARACTER * pCharacter);
	void CreateEquipEffect_Summon(const mu_uint32 ThreadIndex, CHARACTER * pCharacter, mu_int32 iItemType, mu_int32 iItemLevel, mu_int32 iItemExcOption);

	void CreateCastingEffect(const mu_uint32 ThreadIndex, EVector3 &vPosition, EVector3 &vAngle, mu_int32 iSubType);

	void CreateSummonObject(const mu_uint32 ThreadIndex, mu_int32 iSkill, CHARACTER * pCharacter, OBJECT * pObject, mu_float fTargetPos_X, mu_float fTargetPos_Y);
	void SetPlayerSummon(CHARACTER * pCharacter, OBJECT * pObject);

protected:
	std::map<mu_int16, mu_uint8> m_EquipEffectRandom;
};

extern CSummonSystem g_SummonSystem;

#endif