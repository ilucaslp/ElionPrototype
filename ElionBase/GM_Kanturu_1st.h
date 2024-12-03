#ifndef _GM_KANTURU_1ST_H_
#define _GM_KANTURU_1ST_H_

#pragma once

namespace M37Kanturu1st
{
	EINLINE mu_boolean IsKanturu1st()
	{
		return (GET_WORLD == WD_37KANTURU_1ST);
	}
	
	mu_boolean MoveKanturu1stObject(const mu_uint32 ThreadIndex, OBJECT* pObject);
	mu_boolean RenderKanturu1stObjectVisual(const mu_uint32 ThreadIndex, OBJECT* pObject, MUModel* pModel);
	mu_boolean RenderKanturu1stObjectMesh(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b, mu_boolean ExtraMon = 0);
	
	CHARACTER* CreateKanturu1stMonster(const mu_uint32 ThreadIndex, mu_int32 iType, mu_int32 PosX, mu_int32 PosY, mu_int32 Key);
	mu_boolean SetCurrentActionKanturu1stMonster(CHARACTER* c, OBJECT* o);
	mu_boolean AttackEffectKanturu1stMonster(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b);
	mu_boolean MoveKanturu1stMonsterVisual(CHARACTER* c, OBJECT* o, MUModel* b);
	void MoveKanturu1stBlurEffect(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b);
	mu_boolean RenderKanturu1stMonsterObjectMesh(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b, mu_int32 ExtraMon);
	mu_boolean RenderKanturu1stMonsterVisual(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b);

	void PlayBGM();
};

#endif