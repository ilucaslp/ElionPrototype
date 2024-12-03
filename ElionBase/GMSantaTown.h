#ifndef __GMSANTATOWN_H__
#define __GMSANTATOWN_H__

#pragma once

EINLINE mu_boolean IsSantaTown()
{
	return GET_WORLD == WD_62SANTA_TOWN;
}

class CGMSantaTown 
{
public:
	CGMSantaTown();
	~CGMSantaTown();

public:
	mu_boolean CreateObject(mu_uint32 objectIndex, OBJECT* o);
	mu_boolean MoveObject(OBJECT* o);
	mu_boolean RenderObjectVisual(OBJECT* o, MUModel* b);
	mu_boolean RenderObjectMesh(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b, mu_boolean ExtraMon = 0);
	
public:
	CHARACTER* CreateMonster(mu_int32 iType, mu_int32 PosX, mu_int32 PosY, mu_int32 Key);
	
public:
	void PlayBGM();

public:
	mu_boolean CreateSnow( PARTICLE* o );
};

extern CGMSantaTown g_SantaTown;

#endif