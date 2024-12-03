#ifndef __GMEMPIREGUARDIAN3_H__
#define __GMEMPIREGUARDIAN3_H__

#pragma once

class GMEmpireGuardian3
{
public:
	GMEmpireGuardian3();
	~GMEmpireGuardian3();

public:
	mu_boolean CreateObject(mu_uint32 objectIndex, OBJECT* o);
	mu_boolean MoveObject(const mu_uint32 ThreadIndex, OBJECT* o);
	mu_boolean RenderObjectVisual(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b);
	mu_boolean RenderObjectMesh(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b, mu_boolean ExtraMon = 0);

public:
	CHARACTER* CreateMonster(const mu_uint32 ThreadIndex, mu_int32 iType, mu_int32 PosX, mu_int32 PosY, mu_int32 Key);
	mu_boolean MoveMonsterVisual(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b);
	void MoveBlurEffect(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b);
	mu_boolean RenderMonsterVisual(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b);
	mu_boolean SetCurrentActionMonster(CHARACTER* c, OBJECT* o);

public:
	mu_boolean PlayMonsterSound(OBJECT* o);
	void PlayBGM();

private:
	mu_boolean m_bCurrentIsRage_Kato;
};

extern GMEmpireGuardian3 g_EmpireGuardian3;

#endif