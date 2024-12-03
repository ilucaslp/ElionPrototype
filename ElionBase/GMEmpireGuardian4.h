// GMEmpireGuardian4.h: interface for the GMEmpireGuardian4 class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __GMEMPIREGUARDIAN4_H__
#define __GMEMPIREGUARDIAN4_H__

#pragma once

class GMEmpireGuardian4
{
public:
	GMEmpireGuardian4();
	~GMEmpireGuardian4();
	
public:
	mu_boolean CreateObject(mu_uint32 objectIndex, OBJECT* o);
	mu_boolean MoveObject(const mu_uint32 ThreadIndex, OBJECT* o);
	mu_boolean RenderObjectVisual(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b);
	mu_boolean RenderObjectMesh(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b, mu_boolean ExtraMon = 0);

	mu_boolean RenderMonster(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b, mu_boolean ExtraMon = 0);
	
public:
	CHARACTER* CreateMonster(const mu_uint32 ThreadIndex, mu_int32 iType, mu_int32 PosX, mu_int32 PosY, mu_int32 Key);
	mu_boolean MoveMonsterVisual(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b);

	void MoveBlurEffect(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b);
	mu_boolean RenderMonsterVisual(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b);
	mu_boolean SetCurrentActionMonster(CHARACTER* c, OBJECT* o);
	
public:
	mu_boolean PlayMonsterSound(OBJECT* o);
	
private:
	mu_boolean m_bCurrentIsRage_BossGaion;
	mu_boolean m_bCurrentIsRage_Jerint;
};

extern GMEmpireGuardian4 g_EmpireGuardian4;

#endif