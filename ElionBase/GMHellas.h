#ifndef __GMHELLAS_H__
#define __GMHELLAS_H__

#pragma once

#define NUM_HELLAS	7
#define KUNDUN_ZONE NUM_HELLAS

extern MUWaterTerrain *g_pCSWaterTerrain;

EINLINE mu_boolean InHellas(mu_int32 World = GET_WORLD)
{
	return ((World >= WD_24HELLAS && World <= WD_24HELLAS_END) || (World == WD_24HELLAS_7));
}

EINLINE mu_boolean InHiddenHellas(mu_int32 World = GET_WORLD)
{
	return (World == WD_24HELLAS_7);
}

mu_boolean IsWaterTerrain();
void AddWaterWave(mu_int32 x, mu_int32 y, mu_int32 range, mu_int32 height);
mu_float GetWaterTerrain(mu_float x, mu_float y);
void RenderWaterTerrain(mu_int32 Texture, mu_float xf, mu_float yf, mu_float SizeX, mu_float SizeY, EVector3 Light, mu_float Rotation = 0.0f, mu_float Alpha = 1.0f, mu_float Height = 0.0f);

mu_uint8 GetHellasLevel(mu_int32 Class, mu_int32 Level);
mu_boolean GetUseLostMap(mu_boolean bDrawAlert = false);
mu_int32 RenderHellasItemInfo(ITEM* ip, mu_int32 textNum);
mu_boolean EnableKalima(mu_int32 Class, mu_int32 Level, mu_int32 ItemLevel);

void AddObjectDescription(mu_text *Text, EVector3 position);
void RenderObjectDescription();

mu_boolean MoveHellasVisual(OBJECT* o);
mu_boolean RenderHellasVisual(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b);
mu_boolean RenderHellasObjectMesh(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b);

mu_int32 CreateBigMon(OBJECT* o);
void MoveBigMon(OBJECT* o);

void CreateMonsterSkill_ReduceDef(const mu_uint32 ThreadIndex, OBJECT* o, mu_int32 AttackTime, mu_uint8 time, mu_float Height);
void CreateMonsterSkill_Poison(const mu_uint32 ThreadIndex, OBJECT* o, mu_int32 AttackTime, mu_uint8 time);
void CreateMonsterSkill_Summon(const mu_uint32 ThreadIndex, OBJECT* o, mu_int32 AttackTime, mu_uint8 time);

void SetActionDestroy_Def(OBJECT* o);
void RenderDestroy_Def(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b);
CHARACTER* CreateHellasMonster(mu_int32 Type, mu_int32 PositionX, mu_int32 PositionY, mu_int32 Key);

mu_boolean SettingHellasMonsterLinkBone(CHARACTER* c, mu_int32 Type);

mu_boolean SetCurrentAction_HellasMonster(CHARACTER* c, OBJECT* o);
mu_boolean AttackEffect_HellasMonster(const mu_uint32 ThreadIndex, CHARACTER* c, CHARACTER* tc, OBJECT* o, OBJECT* to, MUModel* b);
mu_boolean MoveHellasMonsterVisual(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b);
mu_boolean RenderHellasMonsterObjectMesh(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b);
mu_boolean RenderHellasMonsterVisual(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b);

#endif