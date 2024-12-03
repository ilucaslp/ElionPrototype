#ifndef __GM_RAKLION_H__
#define __GM_RAKLION_H__

EINLINE mu_boolean IsIceCity()
{
	if (GET_WORLD >= WD_57ICECITY && GET_WORLD <= WD_58ICECITY_BOSS)
	{
		return true;
	}
	else if (GET_WORLD == WD_65DOPPLEGANGER1)
	{
		return true;
	}

	return false;
}

namespace SEASON4A
{
	class CGM_Raklion
	{
	public:
		enum RAKLION_STATE
		{
			RAKLION_STATE_IDLE				= 0,
			RAKLION_STATE_NOTIFY_1			= 1,
			RAKLION_STATE_STANDBY			= 2,
			RAKLION_STATE_NOTIFY_2			= 3,
			RAKLION_STATE_READY				= 4,
			RAKLION_STATE_START_BATTLE		= 5,
			RAKLION_STATE_NOTIFY_3			= 6,
			RAKLION_STATE_CLOSE_DOOR		= 7,
			RAKLION_STATE_ALL_USER_DIE		= 8,
			RAKLION_STATE_NOTIFY_4			= 9,
			RAKLION_STATE_END				= 10,
			RAKLION_STATE_DETAIL_STATE		= 11,
			RAKLION_STATE_MAX				= 12,
		};

		enum RAKLION_BATTLE_OF_SELUPAN_PATTERN
		{
			BATTLE_OF_SELUPAN_NONE					= 0,
			BATTLE_OF_SELUPAN_STANDBY				= 1,
			BATTLE_OF_SELUPAN_PATTERN_1				= 2,
			BATTLE_OF_SELUPAN_PATTERN_2				= 3,
			BATTLE_OF_SELUPAN_PATTERN_3				= 4,
			BATTLE_OF_SELUPAN_PATTERN_4				= 5,
			BATTLE_OF_SELUPAN_PATTERN_5				= 6,
			BATTLE_OF_SELUPAN_PATTERN_6				= 7,
			BATTLE_OF_SELUPAN_PATTERN_7				= 8,
			BATTLE_OF_SELUPAN_DIE					= 9,
			BATTLE_OF_SELUPAN_MAX					= 10,
		};

	public:
		CGM_Raklion();
		~CGM_Raklion();

	public:
		mu_boolean CreateObject(const mu_uint32 ThreadIndex, mu_uint32 objectIndex, OBJECT* o);
		mu_boolean MoveObject(const mu_uint32 ThreadIndex, OBJECT* o);
		mu_boolean RenderObjectVisual(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b);
		mu_boolean RenderObjectMesh(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b, mu_boolean ExtraMon = 0);
		
	public:
		CHARACTER* CreateMonster(mu_int32 iType, mu_int32 PosX, mu_int32 PosY, mu_int32 Key);
		mu_boolean MoveMonsterVisual(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b);
		void MoveBlurEffect(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b);
		mu_boolean RenderMonsterVisual(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b);
		mu_boolean AttackEffectMonster(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b);
		mu_boolean SetCurrentActionMonster(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o);
		
	public:
		mu_boolean PlayMonsterSound(OBJECT* o);
		void PlayObjectSound(OBJECT* o);
		void PlayBGM();

	public:
		mu_boolean RenderMonster(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b, mu_boolean ExtraMon = 0);

		void SetBossMonsterAction(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o);

	public:
		mu_boolean CreateSnow( PARTICLE* o );
		void SetState(mu_uint8 byState, mu_uint8 byDetailState);
		mu_boolean CanGoBossMap();
		void SetCanGoBossMap();
		void SetEffect();
		void MoveEffect(const mu_uint32 ThreadIndex);
		void CreateMapEffect(const mu_uint32 ThreadIndex);
	
	private:
		MUCriticalSectionSpinLock m_Critical;
		MUTimer2 m_Timer;
		mu_uint8 m_byState;
		mu_uint8 m_byDetailState;
		mu_boolean m_bCanGoBossMap;
		mu_boolean m_bVisualEffect;
		mu_boolean m_bMusicBossMap;
		mu_boolean m_bBossHeightMove;
	};
}

extern SEASON4A::CGM_Raklion g_Raklion;

#endif