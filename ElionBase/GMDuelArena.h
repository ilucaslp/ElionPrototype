#ifndef __GMDUELARENA_H__
#define __GMDUELARENA_H__

#pragma once

EINLINE mu_boolean IsDuelArena()
{
	return GET_WORLD == WD_64DUELARENA;
}

class CGMDuelArena
{
public:
	CGMDuelArena();
	~CGMDuelArena();

public:
	mu_boolean CreateObject(mu_uint32 objectIndex, OBJECT* o);
	mu_boolean MoveObject(OBJECT* o);
	mu_boolean RenderObjectVisual(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b);
};

extern CGMDuelArena g_DuelArena;

#endif