// GMDoppelGanger1.h: interface for the CGMDoppelGanger1 class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __GMDOPPELGANGER1_H__
#define __GMDOPPELGANGER1_H__

#pragma once

EINLINE mu_boolean IsDoppelGanger1()
{
	if (GET_WORLD == WD_65DOPPLEGANGER1)
	{
		return true;
	}

	return false;
}

class CGMDoppelGanger1
{
public:
	CGMDoppelGanger1();
	~CGMDoppelGanger1();

public:
	mu_boolean MoveObject(OBJECT* o);
	mu_boolean RenderObjectVisual(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b);
	mu_boolean RenderObjectMesh(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b, mu_boolean ExtraMon = 0);
	
public:
	CHARACTER* CreateMonster(mu_int32 iType, mu_int32 PosX, mu_int32 PosY, mu_int32 Key);
	mu_boolean MoveMonsterVisual(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b);
	void MoveBlurEffect(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b);
	mu_boolean RenderMonsterVisual(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b);
	
public:
	mu_boolean PlayMonsterSound(OBJECT* o);
	void PlayBGM();

protected:
	mu_boolean m_bIsMP3Playing;
};

extern CGMDoppelGanger1 g_DoppelGanger1;

#endif // !defined(AFX_GMDOPPELGANGER1_H__12D0B73D_137C_40E6_81C6_D9495961AE3D__INCLUDED_)
