#ifndef __GMEMPIREGUARDIAN2_H__
#define __GMEMPIREGUARDIAN2_H__

#pragma once

class GMEmpireGuardian2
{
public:
	GMEmpireGuardian2();
	~GMEmpireGuardian2();
	
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
	mu_boolean AttackEffectMonster(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b);
	mu_boolean SetCurrentActionMonster(CHARACTER* c, OBJECT* o);
	
public:
	mu_boolean PlayMonsterSound(OBJECT* o);
	void PlayBGM();

private:
	mu_boolean m_bCurrentIsRage_Bermont;
};

extern GMEmpireGuardian2 g_EmpireGuardian2;

#endif