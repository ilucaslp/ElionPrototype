#ifndef __MU_MAPPROCESS_H__
#define __MU_MAPPROCESS_H__

#pragma once

class MUMapProcess
{
public:
	MUMapProcess();
	~MUMapProcess();

	mu_boolean CreateObject(const mu_uint32 ThreadIndex, mu_uint32 objectIndex, OBJECT *o);
	mu_boolean MoveObject(const mu_uint32 ThreadIndex, OBJECT *o);
	mu_boolean RenderObjectVisual(const mu_uint32 ThreadIndex, OBJECT *o, MUModel *b);
	mu_boolean RenderObjectMesh(const mu_uint32 ThreadIndex, OBJECT *o, MUModel *b, mu_int32 ExtraMon);

	CHARACTER *CreateMonster(const mu_uint32 ThreadIndex, mu_int32 Type, mu_int32 PositionX, mu_int32 PositionY, mu_int32 Key);
	mu_boolean MoveMonsterVisual(const mu_uint32 ThreadIndex, CHARACTER *c, OBJECT* o, MUModel* b);
	void MoveBlurEffect(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b);
	mu_boolean RenderMonsterVisual(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b);
	mu_boolean AttackEffectMonster(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b);
	mu_boolean SetCurrentActionMonster(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o);
	mu_boolean PlayMonsterSound(const mu_uint32 ThreadIndex, OBJECT* o);
};

extern MUMapProcess g_MapProcess;

EINLINE MUMapProcess &TheMapProcess()
{
	return g_MapProcess;
}

#endif