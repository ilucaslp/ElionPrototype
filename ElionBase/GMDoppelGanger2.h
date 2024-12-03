#ifndef __GMDOPPELGANGER2_H__
#define __GMDOPPELGANGER2_H__

#pragma once

EINLINE mu_boolean IsDoppelGanger2()
{
	if (GET_WORLD == WD_66DOPPLEGANGER2)
	{
		return true;
	}

	return false;
}

class CGMDoppelGanger2
{
public:
	CGMDoppelGanger2();
	~CGMDoppelGanger2();

public:
	mu_boolean CreateObject(mu_uint32 objectIndex, OBJECT* o);
	mu_boolean MoveObject(OBJECT* o);
	mu_boolean RenderObjectVisual(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b);
	mu_boolean RenderObjectMesh(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b, mu_boolean ExtraMon = 0);

public:
	mu_boolean CreateFireSpark(PARTICLE* o);
	
public:
	mu_boolean MoveMonsterVisual(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b);
	void MoveBlurEffect(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b);
	mu_boolean RenderMonsterVisual(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b);
};

extern CGMDoppelGanger2 g_DoppelGanger2;

#endif