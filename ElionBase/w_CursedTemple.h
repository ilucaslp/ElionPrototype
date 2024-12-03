#ifndef __W_CURSEDTEMPLE_H__
#define __W_CURSEDTEMPLE_H__

#pragma once

class CursedTemple
{
public:
	CursedTemple();
	~CursedTemple();

private:
	void Initialize();
	void Destroy();

public:
	mu_boolean GetInterfaceState(mu_int32 type, mu_int32 subtype = -1);
	void SetInterfaceState(mu_boolean state, mu_int32 subtype = -1);

public:
	mu_boolean IsHolyItemPickState();
	mu_boolean IsCursedTemple();
	mu_boolean IsPartyMember(mu_uint32 selectcharacterindex);
	void ReceiveCursedTempleInfo(mu_uint8* ReceiveBuffer);
	void ReceiveCursedTempleState(const eCursedTempleState state);

public:
	mu_boolean CreateObject(OBJECT* o);
	CHARACTER* CreateCharacters(mu_int32 iType, mu_int32 iPosX, mu_int32 iPosY, mu_int32 iKey);

public:
	mu_boolean SetCurrentActionMonster(CHARACTER* c, OBJECT* o);
	mu_boolean AttackEffectMonster(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b);
	void PlayBGM();
	void ResetCursedTemple();

public:
	mu_boolean MoveObject(OBJECT* o);
	void MoveBlurEffect(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b);
	mu_boolean MoveMonsterVisual(OBJECT* o, MUModel* b);
	void MoveMonsterSoundVisual(OBJECT* o, MUModel* b);

public:
	mu_boolean RenderObjectVisual(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b);
	mu_boolean RenderMonsterVisual(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b);
	mu_boolean RenderObjectMesh(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b, mu_boolean ExtraMon = 0);
	void UpdateTempleSystemMsg(mu_int32 _Value);
	void SetGaugebarEnabled(mu_boolean bFlag);
	void SetGaugebarCloseTimer();
	mu_boolean IsGaugebarEnabled();

private:
	mu_boolean				m_IsTalkEnterNpc;
	mu_boolean				m_InterfaceState;
	mu_uint16				m_HolyItemPlayerIndex;
	eCursedTempleState		m_CursedTempleState;
	std::list<mu_int32>		m_TerrainWaterIndex;
	mu_uint16				m_AlliedPoint;
	mu_uint16				m_IllusionPoint;
	mu_boolean				m_ShowAlliedPointEffect;
	mu_boolean				m_ShowIllusionPointEffect;
	mu_boolean				m_bGaugebarEnabled;
	mu_float				m_fGaugebarCloseTimer;
};

EINLINE mu_boolean CursedTemple::IsCursedTemple()
{
	return (GET_WORLD >= WD_45CURSEDTEMPLE_LV1 && GET_WORLD <= WD_45CURSEDTEMPLE_LV6) ? true : false;
}

#define g_TimeController SEASON3A::TimeController::GetInstance()
extern CursedTemple g_CursedTemple;

#endif // !defined(AFX_URSEDTEMPLE_H__82D1D8BF_A93D_49BE_B384_159381C2495D__INCLUDED_)
