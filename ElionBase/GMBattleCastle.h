#ifndef __GMBATTLECASTLE_H__
#define __GMBATTLECASTLE_H__

namespace battleCastle
{
	extern mu_uint64 g_iMp3PlayTime;

	EINLINE mu_boolean InBattleCastle(mu_int32 iMap = GET_WORLD) { return (iMap == WD_30BATTLECASTLE); }
	EINLINE mu_boolean InBattleCastle2(const EVector3 &Position) { return (Position[0] >= 16100.0f && Position[0] <= 19000.0f && Position[1] >= 18900.0f && Position[1] <= 21700.0f); }
	EINLINE mu_boolean InBattleCastle3(const EVector3 &Position) { return (Position[0] >= 4700.0f && Position[0] <= 14800.0f && Position[1] >= 5300.0f && Position[1] <= 23900.0f); }

	mu_boolean IsBattleCastleStart();
	void SetBattleCastleStart(mu_boolean bResult);

	void Init(const mu_uint32 ThreadIndex);
	mu_boolean SettingBattleFormation(CHARACTER* c, eBuffState state);
	mu_boolean GetGuildMaster(CHARACTER* c);
	void SettingBattleKing(CHARACTER* c);
	void DeleteBattleFormation(CHARACTER* c, eBuffState state);
	void ChangeBattleFormation(mu_text* GuildName, mu_boolean bEffect = false);
	void DeleteTmpCharacter();
	void CreateGuardStoneHealingVisual(const mu_uint32 ThreadIndex, CHARACTER* c, mu_float Range);
	void CollisionHeroCharacter(EVector3 Position, mu_float Range, mu_int32 AniType);
	void CollisionTempCharacter(EVector3 Position, mu_float Range, mu_int32 AniType);
	mu_boolean CollisionEffectToObject(OBJECT* eff, mu_float Range, mu_float RangeZ, mu_boolean bCollisionGround, mu_boolean bRealCollision = false);
	void SetCastleGate_Attribute(mu_int32 x, mu_int32 y, mu_uint8 Operator, mu_boolean bAllClear = false);
	void SetBuildTimeLocation(OBJECT* o);
	void RenderBuildTimes();
	void InitEtcSetting();
	void StartFog(EVector3 Color);
	void EndFog();
	mu_boolean CreateFireSnuff(PARTICLE* o);
	mu_boolean MoveBattleCastleObject(const mu_uint32 ThreadIndex, OBJECT* o);
	mu_boolean CreateBattleCastleObject(const mu_uint32 ThreadIndex, mu_uint32 objectIndex, OBJECT* o);
	mu_boolean MoveBattleCastleVisual(OBJECT* o);
	mu_boolean RenderBattleCastleVisual(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b);
	mu_boolean RenderBattleCastleObjectMesh(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b);
	CHARACTER* CreateBattleCastleMonster(const mu_uint32 ThreadIndex, mu_int32 Type, mu_int32 PositionX, mu_int32 PositionY, mu_int32 Key);
	mu_boolean SettingBattleCastleMonsterLinkBone(CHARACTER* c, mu_int32 Type);
	mu_boolean StopBattleCastleMonster(CHARACTER* c, OBJECT* o);
	mu_boolean MoveBattleCastleMonster(CHARACTER* c, OBJECT* o);
	mu_boolean SetCurrentAction_BattleCastleMonster(CHARACTER* c, OBJECT* o);
	mu_boolean AttackEffect_BattleCastleMonster(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b);
	mu_boolean MoveBattleCastleMonsterVisual(OBJECT* o, MUModel* b);
	mu_boolean RenderBattleCastleMonsterVisual(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b);
	mu_boolean RenderBattleCastleMonsterObjectMesh(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b);
	void RenderMonsterHitEffect(const mu_uint32 ThreadIndex, OBJECT* o);
}

#endif