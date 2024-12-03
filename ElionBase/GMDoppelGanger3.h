#ifndef __GMDOPPELGANGER3_H__
#define __GMDOPPELGANGER3_H__

#pragma once

EINLINE mu_boolean IsDoppelGanger3()
{
	if (GET_WORLD == WD_67DOPPLEGANGER3)
	{
		return true;
	}

	return false;
}

class CGMDoppelGanger3
{
public:
	CGMDoppelGanger3();
	~CGMDoppelGanger3();

public:
	mu_boolean MoveObject(const mu_uint32 ThreadIndex, OBJECT* o);
	mu_boolean RenderObjectVisual(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b);
	mu_boolean RenderObjectMesh(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b, mu_boolean ExtraMon = 0);
	
public:
	mu_boolean MoveMonsterVisual(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b);
	void MoveBlurEffect(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b);
	mu_boolean RenderMonsterVisual(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b);
};

extern CGMDoppelGanger3 g_DoppelGanger3;

#endif