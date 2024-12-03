#ifndef __MU_CHARACTERS_H__
#define __MU_CHARACTERS_H__

#pragma once

class CPhysicsCloth;
class CSIPartsMDL;
class CSPetDarkSpirit;

struct PATH_t
{
	mu_uint16 CurrentPath;
	mu_uint8 CurrentPathFloat;
	mu_uint16 PathNum;
	mu_uint8 PathX[MAX_COUNT_PATH + 1];
	mu_uint8 PathY[MAX_COUNT_PATH + 1];

	mu_boolean Success;
	mu_boolean Error;
	mu_uint8 x, y;
	mu_uint8 Direction;
	mu_uint8 Run;
	mu_int32 Count;

	PATH_t()
	{
		CurrentPath = 0;
		CurrentPathFloat = 0;
		PathNum = 0;

		mu_zeromem(PathX, sizeof(PathX));
		mu_zeromem(PathY, sizeof(PathY));

		Success = 0;
		Error = 0;
		x = 0, y = 0;
		Direction = 0;
		Run = 0;
		Count = 0;
	}
};

struct PART_t
{
	mu_uint8  Level;
	mu_uint8  ExcOption;
	mu_uint8  SetOption;
	mu_uint8  LinkBone;
	mu_uint8  CurrentAction;
	mu_uint8 m_byNumCloth;

	mu_int16 Type;
	mu_uint16  PriorAction;

	mu_float AnimationFrame;
	mu_float PriorAnimationFrame;
	mu_float PlaySpeed;

	CPhysicsCloth *m_pCloth[2];

	PART_t();
	~PART_t();
};

#define DEFAULT_MAX_POSTMOVEPCOCESS		15

struct ST_POSTMOVE_PROCESS
{
	mu_boolean		bProcessingPostMoveEvent;
	mu_uint32		uiProcessingCount_PostMoveEvent;

	ST_POSTMOVE_PROCESS()
	{
		bProcessingPostMoveEvent = false;
		uiProcessingCount_PostMoveEvent = 0;
	}

	~ST_POSTMOVE_PROCESS()
	{
	}
};

class CHARACTER
{
public:
	CHARACTER();
	~CHARACTER();

	void Initialize();
	void Destroy();

public:
	OBJECT	    Object;

	mu_boolean			Blood;
	mu_boolean			Ride;
	mu_boolean			SkillSuccess;
	mu_boolean			m_bFixForm;
	mu_boolean			Foot[2];
	mu_boolean			SafeZone;
	mu_boolean			Change;
	mu_boolean			HideShadow;
	mu_boolean			m_bIsSelected;
	mu_boolean			Decoy;
	mu_boolean			m_CursedTempleCurSkillPacket;

	mu_uint8			Class;
	mu_uint8			Skin;
	mu_uint8			CtlCode;
	mu_uint8			ExtendState;
	mu_uint8			EtcPart;
	mu_uint8			GuildType;
	mu_uint8			GuildRelationShip;
	mu_uint8			GuildSkill;
	mu_uint8			GuildMasterKillCount;
	mu_uint8			BackupCurrentSkill;
	mu_uint8			GuildTeam;
	mu_uint8			m_byGensInfluence;
	mu_uint8			PK;
	mu_uint8			AttackFlag;
	mu_uint8			AttackTime;
	mu_uint8			Dead;
	mu_uint8			Run;
	mu_uint8			SwordCount;
	mu_uint8			byExtensionSkill;
	mu_uint8			m_byDieType;
	mu_uint8			StormTime;
	mu_uint8			JumpTime;
	mu_uint8			TargetX;
	mu_uint8			TargetY;
	mu_uint8			SkillX;
	mu_uint8			SkillY;
	mu_uint8			Appear;
	mu_uint8			CurrentSkill;
	mu_uint8			CastRenderTime;
	mu_uint8			m_byFriend;
	mu_uint8			m_byRankIndex;
	
	mu_boolean 			Movement;
	mu_int8				CollisionTime;
	
	mu_text				ID[64];
	mu_text				OwnerID[32];

	mu_int16			GuildStatus;
	mu_int16			GuildMarkIndex;
	mu_int16			Key;
	mu_int16   			TargetCharacter;

	mu_uint16			Skill;
	mu_uint16			MonsterSkill;
	mu_uint16			Level;
	mu_uint16			MonsterIndex;
	mu_uint16			Damage;
	mu_uint16			Hit;
	mu_uint16			MoveSpeed;
	mu_uint16			m_CursedTempleCurSkill;
	
	mu_int32			Action;
	mu_int32			ExtendStateTime;
	mu_int32			LongRangeAttack;
	mu_int32			SelectItem;
	mu_int32			Item;
	mu_int32			PriorPositionX;
	mu_int32			PriorPositionY;
	mu_int32			PositionX;
	mu_int32			PositionY;
	mu_int32			m_iDeleteTime;
	mu_int32			m_iFenrirSkillTarget;
	mu_int32			m_iTempKey;
	mu_int32			m_nContributionPoint;
	mu_int32			m_PetType;

	mu_int16			Index;

	mu_float			ProtectGuildMarkWorldTime;
	mu_float			AttackRange;
	mu_float			Duplication;
	mu_float			Rot;
	mu_float			TargetAngle;

	EVector3			TargetPosition;
	EVector3			Light;

	PART_t    			BodyPart[MAX_BODYPART];
	PART_t				Weapon[2];
	PART_t				Wing;
	PART_t				Helper;
	PART_t				Flag;
	PATH_t				Path;

	CSIPartsMDL*		m_pParts;
	CSIPartsMDL*		m_pTempParts;
	CSPetDarkSpirit*	m_pPet;

	mu_uint8			EnabledInternalAction;
	mu_uint8			InternalActionPacket[64]; // MAX 64 Bytes
};

namespace INTERNAL_ACTION
{
	enum
	{
		IA_NONE,
		IA_NORMAL,
		IA_PATH,
	};
};

typedef struct
{
	mu_boolean m_bMagic;
	mu_int32 m_iSkill;
	mu_int32 m_iTarget;
} MovementSkill;

extern std::array<mu_int16, MAX_KEYS> CharactersKeys;
extern std::array<CHARACTER, MAX_CHARACTERS_CLIENT + 1> CharactersClient;
extern std::array<mu_uint16, MAX_CHARACTERS_CLIENT + MAX_TASKING_THREADS> g_CharactersQueue;

extern mu_boolean CharacterViewLive;
extern mu_boolean CharacterViewVisible;
extern CHARACTER CharacterView;
extern MUCriticalSectionSpinLock CharactersClientLock;

extern mu_int32 HeroKey;
extern const mu_int32 HeroIndex;
extern CHARACTER *Hero;
extern mu_int32 HeroAngle;

extern mu_boolean Teleport;

extern mu_int32 AttackPlayer;

extern mu_int32 StandTime;
extern mu_int32 ActionTarget;

extern MovementSkill g_MovementSkill;

extern mu_uint8 vec_list[35];
extern mu_uint8 wingLeft[15][2];
extern mu_uint8 wingRight[15][2];
extern mu_uint8 arm_leg_Left[4][2];
extern mu_uint8 arm_leg_Right[4][2];
extern mu_uint8 g_chStar[10];

mu_int32 GetFenrirType(CHARACTER* c);
void FallingMonster(CHARACTER *c, OBJECT* o);
mu_boolean PlayMonsterSoundGlobal(OBJECT * pObject);
mu_boolean PlayMonsterSound(const mu_uint32 ThreadIndex, OBJECT * pObject);

mu_float GetPlaySpeed(OBJECT *o, mu_int32 Type, mu_uint16 CurrentAction);

void SetPlayerStop(const mu_uint32 ThreadIndex, CHARACTER *c);
void SetPlayerWalk(const mu_uint32 ThreadIndex, CHARACTER *c);
void SetPlayerHighBowAttack(const mu_uint32 ThreadIndex, CHARACTER* c);
void SetPlayerAttack(const mu_uint32 ThreadIndex, CHARACTER *c);
void SetPlayerMagic(const mu_uint32 ThreadIndex, CHARACTER *c);
void SetPlayerTeleport(const mu_uint32 ThreadIndex, CHARACTER *c);
void SetPlayerShock(const mu_uint32 ThreadIndex, CHARACTER *c, mu_int32 Hit);
void SetPlayerDie(const mu_uint32 ThreadIndex, CHARACTER *c);
void SetPlayerBow(CHARACTER* c);
void SetPlayerHighBow(CHARACTER* c);

void CalcAddPosition(OBJECT *o, mu_float x, mu_float y, mu_float z, EVector3 &Position);
void AttackEffect(const mu_uint32 ThreadIndex, CHARACTER *c);

mu_boolean CheckMonsterSkill(CHARACTER* pCharacter, OBJECT* pObject);
mu_boolean CharacterAnimation(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o);
mu_int32 GetHandOfWeapon(OBJECT* o);
mu_boolean AttackStage(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o);

void SetCharacterScale(CHARACTER *c);
void SetCharacterClass(const mu_uint32 ThreadIndex, CHARACTER *c);
mu_boolean CheckFullSet(const mu_uint32 ThreadIndex, CHARACTER *c);

void TerminateOwnerEffectObject(mu_int32 iOwnerObjectType);

void DeleteCloth(CHARACTER *c, OBJECT *o = nullptr, PART_t *p2 = nullptr);

void ReleaseCharacters();
void CreateCharacterPointer(CHARACTER *c, mu_int32 Type, mu_uint8 PositionX, mu_uint8 PositionY, mu_float Rotation = 0.0f);
CHARACTER *CreateCharacter(mu_int32 Key, mu_int32 Type, mu_uint8 PositionX, mu_uint8 PositionY, mu_float Rotation = 0.0f, mu_boolean Live = true);
CHARACTER *CreateHero(const mu_uint32 ThreadIndex, mu_int32 Key, mu_int32 Class = 0, mu_int32 Skin = 0, mu_float x = 0.0f, mu_float y = 0.0f, mu_float Rotate = 0.0f);
CHARACTER *CreateMonster(const mu_uint32 ThreadIndex, mu_int32 Type, mu_int32 PositionX, mu_int32 PositionY, mu_int32 Key = 0);
CHARACTER*  CreateHellGate(const mu_uint32 ThreadIndex, const mu_text *ID, mu_int32 Key, mu_int32 Index, mu_int32 x, mu_int32 y, mu_int32 CreateFlag);
void DeleteCharacter(mu_int32 Key);
void DeleteCharacter(CHARACTER* c, OBJECT* o);
mu_float CharacterMoveSpeed(CHARACTER *c);
void MoveCharacterPosition(CHARACTER *c);

void SetMonstersSounds();

void RenderProtectGuildMark(const mu_uint32 ThreadIndex, CHARACTER * c);

const mu_text *GetCharacterClassText(const mu_uint32 byClass);

void SetPlayerColor(mu_uint8 PK, mu_uint32 &Color);
mu_uint32 GetGuildRelationShipTextColor(mu_uint8 GuildRelationShip);
mu_uint32 GetGuildRelationShipBGColor(mu_uint8 GuildRelationShip);

void ChangeCharacterExt(mu_int32 Key, SHARED::CHARSET_PACKET Equipment[GAME::MAX_EQUIPMENT], CHARACTER * pCharacter = nullptr, OBJECT * pHelper = nullptr);

void GuildTeam(CHARACTER *c);

#endif