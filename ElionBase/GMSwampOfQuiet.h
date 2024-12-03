#ifndef __GMSWAMPOFQUIET_H__
#define __GMSWAMPOFQUIET_H__

#pragma once

namespace GMSwampOfQuiet
{
	EINLINE mu_boolean IsCurrentMap()
	{
		return (GET_WORLD == WD_56MAP_SWAMP_OF_QUIET);
	}

	mu_boolean MoveObject(OBJECT* pObject);
	mu_boolean RenderObjectVisual(const mu_uint32 ThreadIndex, OBJECT* pObject, MUModel* pModel);
	mu_boolean RenderObject(const mu_uint32 ThreadIndex, OBJECT* pObject, MUModel* pModel, mu_boolean ExtraMon = 0);

	CHARACTER* CreateSwampOfQuietMonster(mu_int32 iType, mu_int32 PosX, mu_int32 PosY, mu_int32 Key);

	mu_boolean MoveMonsterVisual(const mu_uint32 ThreadIndex, OBJECT* pObject, MUModel* pModel);
	void MoveBlurEffect(const mu_uint32 ThreadIndex, CHARACTER* pCharacter, OBJECT* pObject, MUModel* pModel);
	mu_boolean RenderMonsterVisual(const mu_uint32 ThreadIndex, CHARACTER* pCharacter, OBJECT* pObject, MUModel* pModel);

	mu_boolean SetCurrentActionMonster(CHARACTER* pCharacter, OBJECT* pObject);

	mu_boolean PlayMonsterSound(OBJECT* pObject);
};

#endif