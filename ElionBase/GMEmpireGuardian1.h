// GMEmpireGuardian1.h: interface for the GMEmpireGuardian1 class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __GMEMPIREGUARDIAN1_H__
#define __GMEMPIREGUARDIAN1_H__

#pragma once

class PARTICLE_HEADER;

EINLINE mu_boolean IsEmpireGuardian1()
{
	if (GET_WORLD == WD_69EMPIREGUARDIAN1)
	{
		return true;
	}
	return false;
}

EINLINE mu_boolean IsEmpireGuardian2()
{
	if (GET_WORLD == WD_70EMPIREGUARDIAN2)
	{
		return true;
	}
	return false;
}

EINLINE mu_boolean IsEmpireGuardian3()
{
	if (GET_WORLD == WD_71EMPIREGUARDIAN3)
	{
		return true;
	}
	return false;
}

EINLINE mu_boolean IsEmpireGuardian4()
{
	if (GET_WORLD == WD_72EMPIREGUARDIAN4 ||
		GET_WORLD == WD_73NEW_LOGIN_SCENE ||
		GET_WORLD == WD_74NEW_CHARACTER_SCENE)
	{
		return true;
	}
	return false;
}

EINLINE mu_boolean IsEmpireGuardian()
{
	if (IsEmpireGuardian1() == false &&
		IsEmpireGuardian2() == false &&
		IsEmpireGuardian3() == false &&
		IsEmpireGuardian4() == false)
	{
		return false;
	}

	return true;
}

class GMEmpireGuardian1
{
public:
	GMEmpireGuardian1();
	~GMEmpireGuardian1();

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
	void PlayBGM();

public:
	enum WEATHER_TYPE
	{
		WEATHER_SUN		= 0,
		WEATHER_RAIN	= 1,	
		WEATHER_FOG		= 2,
		WEATHER_STORM	= 3,
	};

	mu_boolean CreateRain(const mu_uint32 ThreadIndex, PARTICLE_HEADER *oh, PARTICLE* o);

	void SetWeather(mu_int32 weather) { m_iWeather = weather; }
	mu_int32 GetWeather() { return m_iWeather; }

private:
	mu_int32 m_iWeather;

private:
	mu_boolean m_bCurrentIsRage_Raymond;
	mu_boolean m_bCurrentIsRage_Ercanne;
	mu_boolean m_bCurrentIsRage_Daesuler;
	mu_boolean m_bCurrentIsRage_Gallia;
};

extern GMEmpireGuardian1 g_EmpireGuardian1;
extern std::array<mu_float, MAX_TASKING_THREADS> g_EmpireGuardianAnimationFrame;
extern std::array<mu_float, MAX_TASKING_THREADS> g_EmpireGuardianPriorAnimationFrame;
extern std::array<mu_uint16, MAX_TASKING_THREADS> g_EmpireGuardianPriorAction;

#endif