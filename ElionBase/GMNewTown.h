#ifndef __GMNEWTOWN_H__
#define __GMNEWTOWN_H__

#pragma once

namespace GMNewTown
{
	EINLINE mu_boolean IsCurrentMap()
	{
		return (GET_WORLD == WD_51HOME_6TH_CHAR ||
			GET_WORLD == WD_73NEW_LOGIN_SCENE ||
			GET_WORLD == WD_74NEW_CHARACTER_SCENE);
	}

	EINLINE mu_boolean IsNewMap73_74()
	{
		return (GET_WORLD == WD_73NEW_LOGIN_SCENE || GET_WORLD == WD_74NEW_CHARACTER_SCENE);
	}

	void CreateObject(mu_uint32 objectIndex, OBJECT* pObject);
	mu_boolean MoveObject(OBJECT* pObject);
	mu_boolean RenderObjectVisual(const mu_uint32 ThreadIndex, OBJECT* pObject, MUModel* pModel);
	mu_boolean RenderObject(const mu_uint32 ThreadIndex, OBJECT* pObject, MUModel* pModel, mu_boolean ExtraMon = 0);

	CHARACTER* CreateNewTownMonster(mu_int32 iType, mu_int32 PosX, mu_int32 PosY, mu_int32 Key);

	void MoveBlurEffect(const mu_uint32 ThreadIndex, CHARACTER* pCharacter, OBJECT* pObject, MUModel* pModel);
	mu_boolean RenderMonsterVisual(const mu_uint32 ThreadIndex, CHARACTER* pCharacter, OBJECT* pObject, MUModel* pModel);

	mu_boolean PlayMonsterSound(OBJECT* pObject);
	void PlayObjectSound(OBJECT* pObject);
};

#endif