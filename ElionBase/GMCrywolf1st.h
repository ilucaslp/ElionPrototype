#ifndef _GMCRYWOLF1ST_H_
#define _GMCRYWOLF1ST_H_

class PARTICLE_HEADER;

namespace M34CryWolf1st
{
	EINLINE mu_boolean IsCyrWolf1st()
	{
		return (GET_WORLD == WD_34CRYWOLF_1ST);
	}

	void MoveCryWolf1stObject(OBJECT* o);
	void RenderCryWolf1stObjectVisual(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b);
	mu_boolean RenderCryWolf1stObjectMesh(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b, mu_int32 ExtraMon);

	CHARACTER* CreateCryWolf1stMonster(const mu_uint32 ThreadIndex, mu_int32 iType, mu_int32 PosX, mu_int32 PosY, mu_int32 Key);
	mu_boolean MoveCryWolf1stMonsterVisual(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b);
	void MoveCryWolf1stBlurEffect(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b);
	mu_boolean RenderCryWolf1stMonsterObjectMesh(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b, mu_int32 ExtraMon);
	mu_boolean RenderCryWolf1stMonsterVisual(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b);
	mu_boolean AttackEffectCryWolf1stMonster(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b);
	mu_boolean SetCurrentActionCrywolfMonster(CHARACTER* c, OBJECT* o);
	mu_boolean CreateMist(const mu_uint32 ThreadIndex, PARTICLE_HEADER *pParticleHeader, PARTICLE *pParticleObj);

	void RenderNoticesCryWolf();
	void CryWolfMVPInit();
	mu_int32 IsCryWolf1stMVPStart();
	mu_boolean IsCryWolf1stMVPStatePeace();
	void CheckCryWolf1stMVPAltarfInfo(mu_int32 StatueHP, mu_uint8 AltarState1, mu_uint8 AltarState2, mu_uint8 AltarState3, mu_uint8 AltarState4, mu_uint8 AltarState5);
	void CheckCryWolf1stMVP(mu_uint8 btOccupationState, mu_uint8 btCrywolfState);
	void DoTankerFireFixStartPosition(const mu_uint32 ThreadIndex, mu_int32 SourceX, mu_int32 SourceY, mu_int32 PositionX, mu_int32 PositionY);

	void Check_AltarState(mu_int32 Num, mu_int32 State);
	void Set_Message_Box(mu_int32 Str, mu_int32 Num, mu_int32 Key, mu_int32 ObjNum = -1);
	void Set_Val_Hp(mu_int32 State);
	void Set_BossMonster(mu_int32 Val_Hp, mu_int32 Dl_Num);
	void SetTime(mu_uint8 byHour, mu_uint8 byMinute);
	mu_boolean Get_State();
	mu_boolean Get_State_Only_Elf();
	mu_boolean Get_AltarState_State(mu_int32 Num);

	void Set_MyRank(mu_uint8 MyRank, mu_int32 GettingExp);
	void Set_WorldRank(mu_uint8 Rank, mu_uint8 Class, mu_int32 Score, mu_text *szHeroName);	

	void PlayBGM();
}

extern mu_uint8 m_AltarState[];
extern mu_boolean View_Bal;
extern mu_int8 Suc_Or_Fail, Message_Box;
extern mu_uint8 m_OccupationState, m_CrywolfState;
extern mu_int32 m_StatueHP, Val_Hp, Dark_elf_Num;
extern mu_boolean View_End_Result;

#endif