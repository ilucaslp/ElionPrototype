#ifndef _GMAIDA_H_
#define _GMAIDA_H_

#pragma once

namespace M33Aida
{
	EINLINE mu_boolean IsInAida()
	{
		return (GET_WORLD == WD_33AIDA);
	}

	EINLINE mu_boolean IsInAidaSection2(const EVector3 &Position)
	{
		if (Position[0] > 5449.0f && Position[0] < 17822.0f && Position[1] > 6784.0f && Position[1] < 22419.0f)
			return true;

		return false;
	}

	mu_boolean CreateAidaObject(OBJECT* pObject);
	mu_boolean MoveAidaObject(OBJECT* pObject);
	mu_boolean RenderAidaObjectVisual(const mu_uint32 ThreadIndex, OBJECT* pObject, MUModel* pModel);
	mu_boolean RenderAidaObjectMesh(const mu_uint32 ThreadIndex, OBJECT* pObject, MUModel* pModel,mu_boolean ExtraMon = 0);
	
	CHARACTER* CreateAidaMonster(mu_int32 iType, mu_int32 PosX, mu_int32 PosY, mu_int32 Key);

	mu_boolean MoveAidaMonsterVisual(const mu_uint32 ThreadIndex, OBJECT* pObject, MUModel* pModel);
	void MoveAidaBlurEffect(const mu_uint32 ThreadIndex, CHARACTER* pCharacter, OBJECT* pObject, MUModel* pModel);
	mu_boolean RenderAidaMonsterObjectMesh(const mu_uint32 ThreadIndex, OBJECT* pObject, MUModel* pModel,mu_boolean ExtraMon);
	mu_boolean RenderAidaMonsterVisual(const mu_uint32 ThreadIndex, CHARACTER* pCharacter, OBJECT* pObject, MUModel* pModel);
	
	mu_boolean AttackEffectAidaMonster(const mu_uint32 ThreadIndex, CHARACTER* pCharacter, OBJECT* pObject, MUModel* pModel);
	mu_boolean SetCurrentActionAidaMonster(CHARACTER* pCharacter, OBJECT* pObject);

	void PlayBGM();
}

#endif