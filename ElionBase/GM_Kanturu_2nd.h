#ifndef _GM_KANTURU_2ND_H_
#define _GM_KANTURU_2ND_H_

#pragma once

namespace M38Kanturu2nd
{
	mu_boolean Create_Kanturu2nd_Object(mu_uint32 objectIndex, OBJECT* o);
	mu_boolean Move_Kanturu2nd_Object(OBJECT* o);
	mu_boolean Render_Kanturu2nd_ObjectVisual(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b);
	mu_boolean Render_Kanturu2nd_ObjectMesh(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b, mu_boolean ExtraMon = 0);

	CHARACTER* Create_Kanturu2nd_Monster(mu_int32 iType, mu_int32 PosX, mu_int32 PosY, mu_int32 Key);
	mu_boolean Set_CurrentAction_Kanturu2nd_Monster(CHARACTER* c, OBJECT* o);
	mu_boolean AttackEffect_Kanturu2nd_Monster(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b);
	mu_boolean Move_Kanturu2nd_MonsterVisual(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b);
	void Move_Kanturu2nd_BlurEffect(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b);
	mu_boolean Render_Kanturu2nd_MonsterObjectMesh(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b, mu_int32 ExtraMon);
	mu_boolean Render_Kanturu2nd_MonsterVisual(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b);

	EINLINE mu_boolean Is_Kanturu2nd()
	{
		return (GET_WORLD == WD_38KANTURU_2ND);
	}
	EINLINE mu_boolean Is_Kanturu2nd_3rd()
	{
		return (GET_WORLD == WD_38KANTURU_2ND || GET_WORLD == WD_39KANTURU_3RD);
	}

	void Sound_Kanturu2nd_Object(OBJECT* o);
	void PlayBGM();
};

class CTrapCanon
{
public:
	CTrapCanon();
	~CTrapCanon();

private:
	void Initialize();
	void Destroy();

public:
	void Open_TrapCanon();
	CHARACTER* Create_TrapCanon(mu_int32 iPosX, mu_int32 iPosY, mu_int32 iKey);
	void Render_Object(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b);
	void Render_Object_Visual(CHARACTER* c, OBJECT* o, MUModel* b);
	void Render_AttackEffect(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b);
};

extern CTrapCanon g_TrapCanon;

#endif