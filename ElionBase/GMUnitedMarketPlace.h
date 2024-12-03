#ifndef __GMUNITEDMARKETPLACE_H__
#define __GMUNITEDMARKETPLACE_H__

#pragma once

class PARTICLE_HEADER;

EINLINE mu_boolean IsUnitedMarketPlace()
{
	return GET_WORLD == WD_79UNITEDMARKETPLACE;
}

class GMUnitedMarketPlace
{
public:
	GMUnitedMarketPlace();
	~GMUnitedMarketPlace();

public:
	mu_boolean CreateObject(mu_uint32 objectIndex, OBJECT* o);
	mu_boolean MoveObject(const mu_uint32 ThreadIndex, OBJECT* o);
	mu_boolean RenderObjectVisual(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b);
	mu_boolean RenderObjectMesh(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b, mu_boolean ExtraMon = 0);

public:
	mu_boolean RenderMonsterVisual(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b);

public:
	mu_boolean CreateRain(PARTICLE* o);
	mu_boolean MoveRain(const mu_uint32 ThreadIndex, PARTICLE_HEADER *oh, PARTICLE* o);
};

extern GMUnitedMarketPlace g_UnitedMarketPlace;

#endif