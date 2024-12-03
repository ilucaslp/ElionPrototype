#ifndef __GMDOPPELGANGER4_H__
#define __GMDOPPELGANGER4_H__

#pragma once

EINLINE mu_boolean IsDoppelGanger4()
{
	if (GET_WORLD == WD_68DOPPLEGANGER4)
	{
		return true;
	}

	return false;
}

class CGMDoppelGanger4
{
public:
	CGMDoppelGanger4();
	~CGMDoppelGanger4();

public:
	mu_boolean MoveObject(const mu_uint32 ThreadIndex, OBJECT* o);
	mu_boolean RenderObjectVisual(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b);
	mu_boolean RenderObjectMesh(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b, mu_boolean ExtraMon = 0);
	
public:
	mu_boolean MoveMonsterVisual(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b);
	void MoveBlurEffect(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b);
	mu_boolean RenderMonsterVisual(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b);
};

extern CGMDoppelGanger4 g_DoppelGanger4;

#endif