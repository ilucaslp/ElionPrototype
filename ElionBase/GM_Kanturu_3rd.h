#ifndef _GM_Kanturu_3rd_H_
#define _GM_Kanturu_3rd_H_

namespace M39Kanturu3rd
{
	EINLINE mu_boolean IsInKanturu3rd()
	{
		return (GET_WORLD == WD_39KANTURU_3RD);
	}

	mu_boolean CreateKanturu3rdObject(mu_uint32 objectIndex, OBJECT* o);
	mu_boolean MoveKanturu3rdObject(OBJECT* o);
	mu_boolean RenderKanturu3rdObjectVisual(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b);
	mu_boolean RenderKanturu3rdObjectMesh(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b, mu_boolean ExtraMon = 0);
	
	CHARACTER* CreateKanturu3rdMonster(const mu_uint32 ThreadIndex, mu_int32 iType, mu_int32 PosX, mu_int32 PosY, mu_int32 Key);

	mu_boolean MoveKanturu3rdMonsterVisual(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b);
	void MoveKanturu3rdBlurEffect(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b);

	mu_boolean RenderKanturu3rdMonsterObjectMesh(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b,mu_boolean ExtraMon);
	mu_boolean RenderKanturu3rdMonsterVisual(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b);
	
	mu_boolean AttackEffectKanturu3rdMonster(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b);
	mu_boolean SetCurrentActionKanturu3rdMonster(CHARACTER* c, OBJECT* o);
	
	void Kanturu3rdInit();
	mu_boolean IsSuccessBattle();
	void CheckSuccessBattle(mu_uint8 State, mu_uint8 DetailState);
	void Kanturu3rdState(mu_uint8 State, mu_uint8 DetailState);
	void MayaSceneMayaAction(mu_uint8 Skill);
	void MayaAction(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b);

	void Kanturu3rdUserandMonsterCount(mu_int32 Count1, mu_int32 Count2);

	void RenderKanturu3rdinterface();
	void RenderKanturu3rdResultInterface();
	void Kanturu3rdResult(mu_uint8 Result);
	void Kanturu3rdSuccess();
	void Kanturu3rdFailed();
	
	void PlayBGM();
}

extern mu_int32 UserCount;
extern mu_int32 MonsterCount;

#endif