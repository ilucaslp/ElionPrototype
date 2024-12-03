#ifndef _GM_PK_FIELD_H_
#define _GM_PK_FIELD_H_

EINLINE mu_boolean IsPKField()
{
	return GET_WORLD == WD_63PK_FIELD;
}

class CGM_PK_Field
{
public:
	mu_boolean CreateObject(mu_uint32 objectIndex, OBJECT* o);
	mu_boolean MoveObject(OBJECT* o);
	mu_boolean RenderObjectVisual(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b);
	mu_boolean RenderObjectMesh(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b, mu_boolean ExtraMon = 0);
	
public:
	mu_boolean CreateFireSpark(PARTICLE* o);

public:
	CHARACTER* CreateMonster(mu_int32 iType, mu_int32 PosX, mu_int32 PosY, mu_int32 Key);
	mu_boolean MoveMonsterVisual(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b);
	void MoveBlurEffect(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b);
	mu_boolean RenderMonsterVisual(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b);
	mu_boolean RenderMonster(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b, mu_boolean ExtraMon = 0);

public:
	mu_boolean PlayMonsterSound(OBJECT* o);
	void PlayObjectSound(OBJECT* o);
	void PlayBGM();
};

extern CGM_PK_Field g_PKField;

#endif //_GM_PK_FIELD_H_