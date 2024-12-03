#ifndef __MU_AI_H__
#define __MU_AI_H__

#pragma once

class MUModel;
class CHARACTER;
class OBJECT;
struct PATH_t;
struct PART_t;

#include "mu_ai_inline.h"

mu_uint8 GetDestValue(mu_int32 xPos, mu_int32 yPos, mu_int32 xDst, mu_int32 yDst);
mu_uint8 CalcTargetPos(mu_float x, mu_float y, mu_int32 Tx, mu_int32 Ty);

mu_boolean IsGMCharacter();

mu_boolean CheckMouseInX(mu_int32 x, mu_int32 width);
mu_boolean CheckMouseInY(mu_int32 y, mu_int32 height);
mu_boolean CheckMouseIn(mu_int32 x, mu_int32 y, mu_int32 width, mu_int32 height);
mu_boolean CheckMouseIn2(mu_int32 x, mu_int32 y, mu_int32 width, mu_int32 height, mu_float aspectWidth, mu_float aspectHeight);

mu_boolean TestFrustrum2D(mu_float posx, mu_float posy, mu_float Range);
mu_boolean TestFrustrum2D(EVector3 Position, mu_float Range);

mu_float CreateAngle(mu_float x1, mu_float y1, mu_float x2, mu_float y2);
mu_int32 TurnAngle(mu_int32 iTheta, mu_int32 iHeading, mu_int32 maxTURN);
mu_float TurnAngle2(mu_float angle, mu_float a, mu_float d);
mu_float FarAngle(mu_float angle1, mu_float angle2, mu_boolean abs = true);
mu_int32 CalcAngle(mu_float PositionX, mu_float PositionY, mu_float TargetX, mu_float TargetY);

namespace BoneManager
{
	void GetBonePosition(OBJECT *pObject, mu_uint32 BoneIndex, EVector3 &Position);
	void GetBonePosition(OBJECT *pObject, mu_uint32 BoneIndex, EVector3 &Relative, EVector3 &Position);
};

mu_float MoveHumming(EVector3 Position, EVector3 &Angle, EVector3 TargetPosition, mu_float Turn);

void SetAction_Fenrir_Skill(CHARACTER* c, OBJECT* o);
void SetAction_Fenrir_Damage(CHARACTER* c, OBJECT* o);
void SetAction_Fenrir_Run(CHARACTER* c, OBJECT* o);
void SetAction_Fenrir_Walk(CHARACTER* c, OBJECT* o);
mu_boolean IsAliceRideAction_UniDino(mu_uint16 byAction);
mu_boolean IsAliceRideAction_Fenrir(mu_uint16 byAction);

void SetAction(OBJECT *o, mu_int32 Action, mu_boolean bBlending = true);
void SetActionClass(CHARACTER *c, OBJECT *o, mu_int32 Action, mu_int32 ActionType);
void SetAllAction(mu_int32 Action);

void Attack(const mu_uint32 ThreadIndex, CHARACTER *c);

void AppearMonster(const mu_uint32 ThreadIndex, CHARACTER *c);

void LookAtTarget(OBJECT *o, CHARACTER *tc);
void MoveHead(CHARACTER *c);

mu_boolean MovePath(CHARACTER *c, mu_boolean Turn = true);
mu_boolean PathFinding2(const mu_uint32 ThreadIndex, mu_int32 sx, mu_int32 sy, mu_int32 tx, mu_int32 ty, PATH_t *a, mu_float fDistance = 0.0f, mu_int32 iDefaultWall = TW_CHARACTER, mu_uint8 extraAddr = 0);
mu_boolean TestPath(const mu_uint32 ThreadIndex, mu_int32 sx, mu_int32 sy, mu_int32 tx, mu_int32 ty, mu_float fDistance = 0.0f, mu_int32 iDefaultWall = TW_CHARACTER);

void BodyLight(const mu_uint32 ThreadIndex, OBJECT *o, MUModel *b);

void MoveEye(const mu_uint32 ThreadIndex, OBJECT *o, MUModel *b, mu_int32 Right, mu_int32 Left, mu_int32 Right2 = -1, mu_int32 Left2 = -1, mu_int32 Right3 = -1, mu_int32 Left3 = -1);
void MonsterDieSandSmoke(const mu_uint32 ThreadIndex, OBJECT *o);
void MonsterMoveSandSmoke(const mu_uint32 ThreadIndex, OBJECT *o);

mu_boolean IsAliceRideAction_UniDino(mu_uint16 Action);
mu_boolean IsAliceRideAction_Fenrir(mu_uint16 Action);

mu_int32 FindCharacterIndex(mu_int32 Key, mu_boolean checkAlive = true);
mu_int32 HangerBloodCastleQuestItem(mu_int32 Key);

void FallingStartCharacter(CHARACTER* c, OBJECT* o);

void CreateBonfire(const mu_uint32 ThreadIndex, EVector3 Position, EVector3 Angle);
void CreateFire(const mu_uint32 ThreadIndex, mu_int32 Type, OBJECT *o, mu_float x, mu_float y, mu_float z);
void CheckSkull(OBJECT *o);
void RenderAurora(mu_int32 Type, mu_int32 RenderType, mu_float x, mu_float y, mu_float sx, mu_float sy, EVector3 Light);
void RenderPartObjectEdge(const mu_uint32 ThreadIndex, MUModel *b, OBJECT *o, mu_int32 Flag, mu_boolean Translate, mu_float Scale);

enum RENDER_STATUS
{
	RENDER_NOSLOT,
	RENDER_DISABLED,
	RENDER_HIDDEN,
	RENDER_SUCCEED,
};

mu_boolean Calc_RenderObject(const mu_uint32 ThreadIndex, OBJECT *o, mu_boolean Translate, mu_int32 Select, mu_int32 ExtraMon);
mu_boolean Calc_ObjectAnimation(const mu_uint32 ThreadIndex, OBJECT* o, mu_boolean Translate, mu_int32 Select);
RENDER_STATUS Draw_RenderObject(const mu_uint32 ThreadIndex, OBJECT *o, mu_boolean Translate, mu_int32 Select, mu_int32 ExtraMon);
RENDER_STATUS RenderObject(mu_int32 ThreadIndex, OBJECT *o, mu_boolean Translate = false, mu_int32 Select = 0, mu_int32 ExtraMon = 0);
void RenderObject_AfterImage(const mu_uint32 ThreadIndex, OBJECT* o, mu_boolean Translate = false, mu_int32 Select = 0, mu_int32 ExtraMon = 0);

void PartObjectColor(mu_int32 Type, mu_float Alpha, mu_float Bright, EVector3 &Light, mu_boolean ExtraMon = false);
void PartObjectColor2(mu_int32 Type, mu_float Alpha, mu_float Bright, EVector3 &Light, mu_boolean ExtraMon = false);
void PartObjectColor3(mu_int32 Type, mu_float Alpha, mu_float Bright, EVector3 &Light, mu_boolean ExtraMon = false);
void RenderPartObjectBody(const mu_uint32 ThreadIndex, MUModel *b, OBJECT *o, mu_int32 Type, mu_float Alpha, mu_int32 RenderType);
void RenderPartObjectBodyColor(const mu_uint32 ThreadIndex, MUModel *b, OBJECT *o, mu_int32 Type, mu_float Alpha, mu_int32 RenderType, mu_float Bright, mu_int32 Texture = -1, mu_int32 iMonsterIndex = -1);
void RenderPartObjectBodyColor2(const mu_uint32 ThreadIndex, MUModel *b, OBJECT *o, mu_int32 Type, mu_float Alpha, mu_int32 RenderType, mu_float Bright, mu_int32 Texture = -1);
void RenderPartObjectEffect(const mu_uint32 ThreadIndex, OBJECT *o, mu_int32 Type, EVector3 Light, mu_float Alpha = 0.0f, mu_int32 ItemLevel = 0, mu_int32 ExcOption = 0, mu_int32 SetOption = 0, mu_int32 Select = 0, mu_int32 RenderType = RENDER_TEXTURE);
void RenderPartObject(const mu_uint32 ThreadIndex, OBJECT *o, mu_int32 Type, void *p2, EVector3 Light, mu_float Alpha = 0.0f, mu_int32 ItemLevel = 0, mu_int32 ExcOption = 0, mu_int32 SetOption = 0, mu_boolean GlobalTransform = false, mu_boolean HideSkin = false, mu_boolean Translate = false, mu_int32 Select = 0, mu_int32 RenderType = RENDER_TEXTURE);
void RenderWheelWeapon(const mu_uint32 ThreadIndex, OBJECT *o);
void RenderFuryStrike(const mu_uint32 ThreadIndex, OBJECT *o);
void RenderSkillSpear(const mu_uint32 ThreadIndex, OBJECT *o);
void RenderBrightEffect(const mu_uint32 ThreadIndex, MUModel *b, mu_int32 Bitmap, mu_int32 Link, mu_float Scale, EVector3 Light, OBJECT *o);
void RenderLight(const mu_uint32 ThreadIndex, OBJECT *o, mu_int32 Texture, mu_float Scale, mu_int32 Bone, mu_float x, mu_float y, mu_float z);
void RenderEye(const mu_uint32 ThreadIndex, OBJECT *o, mu_int32 Left, mu_int32 Right, mu_float fSize = 1.0f);
void RenderLinkObject(const mu_uint32 ThreadIndex, mu_float x, mu_float y, mu_float z, CHARACTER *c, PART_t *f, mu_int32 Type, mu_int32 Level, mu_int32 ExcOption, mu_boolean Link, mu_boolean Translate, mu_int32 RenderType = 0, mu_boolean bRightHandItem = true);
void NextGradeObjectRender(const mu_uint32 ThreadIndex, CHARACTER *c);
mu_boolean RenderCharacterBackItem(const mu_uint32 ThreadIndex, CHARACTER *c, OBJECT* o, mu_boolean bTranslate);

void ItemObjectAttribute(OBJECT *o);

mu_boolean CheckCharacterRange(OBJECT *so, mu_float Range, mu_int16 PKKey, mu_uint8 Kind = 0);
void CheckClientArrow(const mu_uint32 ThreadIndex, OBJECT *o);

void CheckGrass(OBJECT* o);

mu_int32 GetEquipedBowType(CHARACTER *pChar);
mu_int32 GetEquipedBowType(ITEM* pItem);
mu_int32 GetEquipedBowType();
mu_int32 GetEquipedBowType_Skill();

mu_boolean IsEquipedWing();

void RenderSkillEarthQuake(const mu_uint32 ThreadIndex, CHARACTER* c, OBJECT* o, MUModel* b, mu_int32 iMaxSkill = 30);

void RenderObjectScreen(mu_int32 Type, mu_int32 ItemLevel, mu_int32 ExcOption, mu_int32 SetOption, EVector3 Target, mu_int32 Select, mu_boolean PickUp);
void RenderItem3D(mu_float sx, mu_float sy, mu_float Width, mu_float Height, mu_int32 Type, mu_int32 Level, mu_int32 ExcOption, mu_int32 SetOption, mu_boolean PickUp = false);

#define MONSTER01_STOP1    0
#define MONSTER01_STOP2    1
#define MONSTER01_WALK     2
#define MONSTER01_ATTACK1  3
#define MONSTER01_ATTACK2  4
#define MONSTER01_SHOCK    5
#define MONSTER01_DIE      6
#define MONSTER01_APEAR    7
#define MONSTER01_ATTACK3  8
#define MONSTER01_ATTACK4  9
#define MONSTER01_RUN      10
#define MONSTER01_ATTACK5  11

#define FENRIR_STAND		0
#define FENRIR_WALK			1
#define FENRIR_RUN			2
#define FENRIR_ATTACK		3
#define FENRIR_ATTACK_SKILL	4
#define FENRIR_DAMAGE		5
#define FENRIR_RUN_DELAY	20

enum
{
	PLAYER_SET,
	PLAYER_STOP_MALE,
	PLAYER_STOP_FEMALE,
	PLAYER_STOP_SUMMONER,
	PLAYER_STOP_SWORD,
	PLAYER_STOP_TWO_HAND_SWORD,
	PLAYER_STOP_SPEAR,
	PLAYER_STOP_SCYTHE,
	PLAYER_STOP_BOW,
	PLAYER_STOP_CROSSBOW,
	PLAYER_STOP_WAND,
	PLAYER_STOP_FLY,
	PLAYER_STOP_FLY_CROSSBOW,
	PLAYER_STOP_RIDE,
	PLAYER_STOP_RIDE_WEAPON,

	PLAYER_WALK_MALE,
	PLAYER_WALK_FEMALE,
	PLAYER_WALK_SWORD,
	PLAYER_WALK_TWO_HAND_SWORD,
	PLAYER_WALK_SPEAR,
	PLAYER_WALK_SCYTHE,
	PLAYER_WALK_BOW,
	PLAYER_WALK_CROSSBOW,
	PLAYER_WALK_WAND,
	PLAYER_WALK_SWIM,
	PLAYER_RUN,
	PLAYER_RUN_SWORD,
	PLAYER_RUN_TWO_SWORD,
	PLAYER_RUN_TWO_HAND_SWORD,
	PLAYER_RUN_SPEAR,
	PLAYER_RUN_BOW,
	PLAYER_RUN_CROSSBOW,
	PLAYER_RUN_WAND,
	PLAYER_RUN_SWIM,
	PLAYER_FLY,
	PLAYER_FLY_CROSSBOW,
	PLAYER_RUN_RIDE,
	PLAYER_RUN_RIDE_WEAPON,

	PLAYER_ATTACK_FIST,
	PLAYER_ATTACK_SWORD_RIGHT1,
	PLAYER_ATTACK_SWORD_RIGHT2,
	PLAYER_ATTACK_SWORD_LEFT1,
	PLAYER_ATTACK_SWORD_LEFT2,
	PLAYER_ATTACK_TWO_HAND_SWORD1,
	PLAYER_ATTACK_TWO_HAND_SWORD2,
	PLAYER_ATTACK_TWO_HAND_SWORD3,
	PLAYER_ATTACK_SPEAR1,
	PLAYER_ATTACK_SCYTHE1,
	PLAYER_ATTACK_SCYTHE2,
	PLAYER_ATTACK_SCYTHE3,
	PLAYER_ATTACK_BOW,
	PLAYER_ATTACK_CROSSBOW,
	PLAYER_ATTACK_FLY_BOW,
	PLAYER_ATTACK_FLY_CROSSBOW,
	PLAYER_ATTACK_RIDE_SWORD,
	PLAYER_ATTACK_RIDE_TWO_HAND_SWORD,
	PLAYER_ATTACK_RIDE_SPEAR,
	PLAYER_ATTACK_RIDE_SCYTHE,
	PLAYER_ATTACK_RIDE_BOW,
	PLAYER_ATTACK_RIDE_CROSSBOW,

	PLAYER_ATTACK_SKILL_SWORD1,
	PLAYER_ATTACK_SKILL_SWORD2,
	PLAYER_ATTACK_SKILL_SWORD3,
	PLAYER_ATTACK_SKILL_SWORD4,
	PLAYER_ATTACK_SKILL_SWORD5,

	PLAYER_ATTACK_SKILL_WHEEL,
	PLAYER_ATTACK_SKILL_FURY_STRIKE,
	PLAYER_SKILL_VITALITY,
	PLAYER_SKILL_RIDER,
	PLAYER_SKILL_RIDER_FLY,
	PLAYER_ATTACK_SKILL_SPEAR,
	PLAYER_ATTACK_ONETOONE,
	PLAYER_SKILL_HELL_BEGIN,
	PLAYER_SKILL_HELL_START,

	PLAYER_ATTACK_END,

	PLAYER_FLY_RIDE = PLAYER_ATTACK_END,
	PLAYER_FLY_RIDE_WEAPON,
	PLAYER_DARKLORD_STAND,
	PLAYER_DARKLORD_WALK,
	PLAYER_STOP_RIDE_HORSE,
	PLAYER_RUN_RIDE_HORSE,
	PLAYER_ATTACK_STRIKE,
	PLAYER_ATTACK_TELEPORT,
	PLAYER_ATTACK_RIDE_STRIKE,
	PLAYER_ATTACK_RIDE_TELEPORT,
	PLAYER_ATTACK_RIDE_HORSE_SWORD,
	PLAYER_ATTACK_RIDE_ATTACK_FLASH,
	PLAYER_ATTACK_RIDE_ATTACK_MAGIC,
	PLAYER_ATTACK_DARKHORSE,
	PLAYER_IDLE1_DARKHORSE,
	PLAYER_IDLE2_DARKHORSE,
	PLAYER_FENRIR_ATTACK,
	PLAYER_FENRIR_ATTACK_DARKLORD_AQUA,
	PLAYER_FENRIR_ATTACK_DARKLORD_STRIKE,
	PLAYER_FENRIR_ATTACK_DARKLORD_SWORD,
	PLAYER_FENRIR_ATTACK_DARKLORD_TELEPORT,
	PLAYER_FENRIR_ATTACK_DARKLORD_FLASH,
	PLAYER_FENRIR_ATTACK_TWO_SWORD,
	PLAYER_FENRIR_ATTACK_MAGIC,
	PLAYER_FENRIR_ATTACK_CROSSBOW,
	PLAYER_FENRIR_ATTACK_SPEAR,
	PLAYER_FENRIR_ATTACK_ONE_SWORD,
	PLAYER_FENRIR_ATTACK_BOW,
	PLAYER_FENRIR_SKILL,
	PLAYER_FENRIR_SKILL_TWO_SWORD,
	PLAYER_FENRIR_SKILL_ONE_RIGHT,
	PLAYER_FENRIR_SKILL_ONE_LEFT,
	PLAYER_FENRIR_DAMAGE,
	PLAYER_FENRIR_DAMAGE_TWO_SWORD,
	PLAYER_FENRIR_DAMAGE_ONE_RIGHT,
	PLAYER_FENRIR_DAMAGE_ONE_LEFT,
	PLAYER_FENRIR_RUN,
	PLAYER_FENRIR_RUN_TWO_SWORD,
	PLAYER_FENRIR_RUN_ONE_RIGHT,
	PLAYER_FENRIR_RUN_ONE_LEFT,
	PLAYER_FENRIR_RUN_MAGOM,
	PLAYER_FENRIR_RUN_TWO_SWORD_MAGOM,
	PLAYER_FENRIR_RUN_ONE_RIGHT_MAGOM,
	PLAYER_FENRIR_RUN_ONE_LEFT_MAGOM,
	PLAYER_FENRIR_RUN_ELF,
	PLAYER_FENRIR_RUN_TWO_SWORD_ELF,
	PLAYER_FENRIR_RUN_ONE_RIGHT_ELF,
	PLAYER_FENRIR_RUN_ONE_LEFT_ELF,
	PLAYER_FENRIR_STAND,
	PLAYER_FENRIR_STAND_TWO_SWORD,
	PLAYER_FENRIR_STAND_ONE_RIGHT,
	PLAYER_FENRIR_STAND_ONE_LEFT,
	PLAYER_FENRIR_WALK,
	PLAYER_FENRIR_WALK_TWO_SWORD,
	PLAYER_FENRIR_WALK_ONE_RIGHT,
	PLAYER_FENRIR_WALK_ONE_LEFT,

	PLAYER_ATTACK_BOW_UP,
	PLAYER_ATTACK_CROSSBOW_UP,
	PLAYER_ATTACK_FLY_BOW_UP,
	PLAYER_ATTACK_FLY_CROSSBOW_UP,
	PLAYER_ATTACK_RIDE_BOW_UP,
	PLAYER_ATTACK_RIDE_CROSSBOW_UP,
	PLAYER_ATTACK_ONE_FLASH,
	PLAYER_ATTACK_RUSH,
	PLAYER_ATTACK_DEATH_CANNON,
	PLAYER_ATTACK_REMOVAL,
	PLAYER_ATTACK_STUN,
	PLAYER_HIGH_SHOCK,

	PLAYER_STOP_TWO_HAND_SWORD_TWO,
	PLAYER_WALK_TWO_HAND_SWORD_TWO,
	PLAYER_RUN_TWO_HAND_SWORD_TWO,
	PLAYER_ATTACK_TWO_HAND_SWORD_TWO,

	PLAYER_SKILL_HAND1,
	PLAYER_SKILL_HAND2,
	PLAYER_SKILL_WEAPON1,
	PLAYER_SKILL_WEAPON2,
	PLAYER_SKILL_ELF1,
	PLAYER_SKILL_TELEPORT,
	PLAYER_SKILL_FLASH,
	PLAYER_SKILL_INFERNO,
	PLAYER_SKILL_HELL,
	PLAYER_RIDE_SKILL,
	PLAYER_SKILL_SLEEP,
	PLAYER_SKILL_SLEEP_UNI,
	PLAYER_SKILL_SLEEP_DINO,
	PLAYER_SKILL_SLEEP_FENRIR,
	PLAYER_SKILL_CHAIN_LIGHTNING,
	PLAYER_SKILL_CHAIN_LIGHTNING_UNI,
	PLAYER_SKILL_CHAIN_LIGHTNING_DINO,
	PLAYER_SKILL_CHAIN_LIGHTNING_FENRIR,
	PLAYER_SKILL_LIGHTNING_ORB,
	PLAYER_SKILL_LIGHTNING_ORB_UNI,
	PLAYER_SKILL_LIGHTNING_ORB_DINO,
	PLAYER_SKILL_LIGHTNING_ORB_FENRIR,
	PLAYER_SKILL_DRAIN_LIFE,
	PLAYER_SKILL_DRAIN_LIFE_UNI,
	PLAYER_SKILL_DRAIN_LIFE_DINO,
	PLAYER_SKILL_DRAIN_LIFE_FENRIR,
	PLAYER_SKILL_SUMMON,
	PLAYER_SKILL_SUMMON_UNI,
	PLAYER_SKILL_SUMMON_DINO,
	PLAYER_SKILL_SUMMON_FENRIR,

	PLAYER_SKILL_BLOW_OF_DESTRUCTION,
	PLAYER_SKILL_SWELL_OF_MP,
	PLAYER_SKILL_MULTISHOT_BOW_STAND,
	PLAYER_SKILL_MULTISHOT_BOW_FLYING,
	PLAYER_SKILL_MULTISHOT_CROSSBOW_STAND,
	PLAYER_SKILL_MULTISHOT_CROSSBOW_FLYING,
	PLAYER_SKILL_RECOVERY,
	PLAYER_SKILL_GIGANTICSTORM,
	PLAYER_SKILL_FLAMESTRIKE,
	PLAYER_SKILL_LIGHTNING_SHOCK,
	PLAYER_DEFENSE1,
	PLAYER_GREETING1,
	PLAYER_GREETING_FEMALE1,
	PLAYER_GOODBYE1,
	PLAYER_GOODBYE_FEMALE1,
	PLAYER_CLAP1,
	PLAYER_CLAP_FEMALE1,
	PLAYER_CHEER1,
	PLAYER_CHEER_FEMALE1,
	PLAYER_DIRECTION1,
	PLAYER_DIRECTION_FEMALE1,
	PLAYER_GESTURE1,
	PLAYER_GESTURE_FEMALE1,
	PLAYER_UNKNOWN1,
	PLAYER_UNKNOWN_FEMALE1,
	PLAYER_CRY1,
	PLAYER_CRY_FEMALE1,
	PLAYER_AWKWARD1,
	PLAYER_AWKWARD_FEMALE1,
	PLAYER_SEE1,
	PLAYER_SEE_FEMALE1,
	PLAYER_WIN1,
	PLAYER_WIN_FEMALE1,
	PLAYER_SMILE1,
	PLAYER_SMILE_FEMALE1,
	PLAYER_SLEEP1,
	PLAYER_SLEEP_FEMALE1,
	PLAYER_COLD1,
	PLAYER_COLD_FEMALE1,
	PLAYER_AGAIN1,
	PLAYER_AGAIN_FEMALE1,
	PLAYER_RESPECT1,
	PLAYER_SALUTE1,
	PLAYER_SCISSORS,
	PLAYER_ROCK,
	PLAYER_PAPER,
	PLAYER_HUSTLE,
	PLAYER_PROVOCATION,
	PLAYER_LOOK_AROUND,
	PLAYER_CHEERS,
	PLAYER_KOREA_HANDCLAP,
	PLAYER_POINT_DANCE,
	PLAYER_RUSH1,
	PLAYER_COME_UP,
	PLAYER_SHOCK,
	PLAYER_DIE1,
	PLAYER_DIE2,
	PLAYER_SIT1,
	PLAYER_SIT2,
	PLAYER_SIT_FEMALE1,
	PLAYER_SIT_FEMALE2,
	PLAYER_HEALING1,
	PLAYER_HEALING_FEMALE1,
	PLAYER_POSE1,
	PLAYER_POSE_FEMALE1,
	PLAYER_JACK_1,
	PLAYER_JACK_2,
	PLAYER_SANTA_1,
	PLAYER_SANTA_2,
	PLAYER_CHANGE_UP,
	PLAYER_RECOVER_SKILL,
	MAX_PLAYER_ACTION,
};

#endif