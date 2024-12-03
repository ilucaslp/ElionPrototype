#ifndef __MU_INTERFACE_H__
#define __MU_INTERFACE_H__

#pragma once

#define TRADELIMITLEVEL 6

#define TEXTLIST_COUNT 50
#define TEXTLIST_LENGTH 256

#define TEXT_COLOR_WHITE    0 
#define TEXT_COLOR_BLUE     1
#define TEXT_COLOR_RED      2
#define TEXT_COLOR_YELLOW   3
#define TEXT_COLOR_GREEN    4
#define TEXT_COLOR_DARKRED  5
#define TEXT_COLOR_PURPLE   6
#define TEXT_COLOR_DARKBLUE 7
#define TEXT_COLOR_DARKYELLOW 8
#define TEXT_COLOR_GREEN_BLUE 9
#define TEXT_COLOR_GRAY       10
#define TEXT_COLOR_REDPURPLE  11
#define TEXT_COLOR_VIOLET	12
#define TEXT_COLOR_ORANGE	13

class CHARACTER;

extern mu_int32 TextNum;
extern mu_int32 SkipNum;
extern mu_unicode TextList[TEXTLIST_COUNT][TEXTLIST_LENGTH];
extern mu_uint32 TextListColor[TEXTLIST_COUNT];
extern mu_boolean TextBold[TEXTLIST_COUNT];
extern EVector2 Size[TEXTLIST_COUNT];

extern mu_unicode MacroText[10][256];

extern mu_int32 MouseX;
extern mu_int32 MouseY;
extern mu_int32 BackMouseX;
extern mu_int32 BackMouseY;
extern mu_uint32 MouseRButtonPress;
extern mu_int32 MouseUpdateTime;
extern mu_int32 MouseUpdateTimeMax;

extern const mu_float AutoMouseLimitTime;
extern mu_float LButtonPopTime;
extern mu_float LButtonPressTime;
extern mu_float RButtonPopTime;
extern mu_float RButtonPressTime;

extern mu_boolean LogOut;

extern mu_int32 SelectMonster;
extern mu_int32 SelectModel;
extern mu_int32 SelectMapping;
extern mu_int32 SelectColor;
extern mu_int32 SelectWall;
extern mu_float SelectMappingAngle;
extern mu_int32 SelectedItem;
extern mu_int32 SelectedNpc;
extern mu_int32 SelectedCharacter;
extern mu_int32 SelectedHero;
extern mu_int32 SelectedOperate;
extern mu_int32 Attacking;
extern mu_int32 g_iFollowCharacter;

extern mu_int32 RequestedAttackCharacter;

extern mu_int32 HeroTile;
extern mu_int32 TargetNpc;
extern mu_int32 TargetType;
extern mu_int32 TargetX;
extern mu_int32 TargetY;
extern mu_float TargetAngle;

extern mu_boolean g_bAutoGetItem;
extern mu_boolean bCheckNPC;
extern mu_boolean DontMove;

extern mu_boolean g_Ashies;
extern mu_boolean g_GMObservation;

extern mu_int32 LoadingWorld;

extern EVector3 MousePosition;
extern EVector3 MouseTarget;
extern EVector3 MouseMin;
extern EVector3 MouseMax;
extern EVector3 MouseDirection;
extern EVector3 MouseAbsDirection;
extern EFloat MouseAbsMin;
extern mu_uint8 MouseAbsMinIndex;

extern mu_float CollisionDistance;
extern EVector3 CollisionPosition;

extern mu_boolean SelectFlag;
extern mu_float SelectXF;
extern mu_float SelectYF;

extern mu_int32 ItemHelp;

void RenderTipTextList(mu_float sx, mu_float sy, mu_int32 TextNum, mu_int32 Tab, mu_int32 iSort, mu_int32 iRenderPoint = STRP_NONE, mu_boolean bUseBG = true);

mu_int32 FindHotKey(mu_int32 Skill);

mu_boolean CheckTile(CHARACTER *c, OBJECT *o, mu_float Range);
mu_boolean CheckWall(mu_int32 sx1, mu_int32 sy1, mu_int32 sx2, mu_int32 sy2);

mu_boolean CheckAttack_Fenrir(CHARACTER* c);
mu_boolean CheckAttack();
mu_int32 getTargetCharacterKey(CHARACTER* c, mu_int32 selected);
mu_boolean CheckTarget(const mu_uint32 ThreadIndex, CHARACTER *c);

mu_int32 GetScreenWidth();

void CheckChatText(const mu_unicode *Text);
mu_boolean CheckMacroLimit(const mu_unicode* Text);
mu_boolean CheckAbuseFilter(mu_string Text, mu_boolean bCheckSlash = true);
mu_boolean CheckAbuseNameFilter(mu_string Text);
mu_boolean CheckCommand(const mu_unicode *Text, mu_boolean isMacroText = false);

mu_boolean InsideTest(EVector3 &dirPos, EVector3 vIn[4], mu_int32 flag, mu_float type);
mu_boolean CollisionDetectLineToFace(EVector3 v[4], EVector3 &Normal, mu_boolean Collision, mu_float &Distance, EVector3 &CollisionPosition);
mu_boolean ProjectLineBox(EVector3 ax, EVector3 p1, EVector3 p2, OBB_t obb);
mu_boolean CollisionDetectLineToOBB(EVector3 p1, EVector3 p2, OBB_t obb);

#define InitDetectTerrainCollision ThreadInfo.SelectFlag = false; \
									ThreadInfo.CollisionDistance = FLT_MAX;

#ifdef USE_SIMD_COLLISION
mu_boolean DetectTerrainCollisionFast(const mu_uint32 ThreadIndex, mu_int32 x, mu_int32 y, const ESimd128 Origin, const ESimd128 Direction, const std::array<ESimd128, 4> &Vertices);
#else
mu_boolean DetectTerrainCollisionFast(const mu_uint32 ThreadIndex, mu_int32 x, mu_int32 y, const std::array<EVector3, 4> &Vertices);
#endif
mu_boolean DetectTerrainCollision(const mu_uint32 ThreadIndex, mu_int32 x, mu_int32 y);

mu_int32 SelectItem();
mu_int32 SelectCharacter(mu_uint8 Kind);
mu_int32 SelectOperate();
void SelectObjects();

namespace CHARACTERSCENE
{
	void SelectCharacter();
};

void ConvertGold(mu_double dGold, mu_unicode *szText, const mu_uint32 TextLength, mu_int32 iDecimals = 0);
void ConvertGold64(mu_int64 Gold, mu_unicode *Text, const mu_uint32 TextLength);
void ConvertTaxGold(mu_int64 Gold, mu_unicode *Text, const mu_uint32 TextLength);
void ConvertChaosTaxGold(mu_int64 Gold, mu_unicode *Text, const mu_uint32 TextLength);
mu_int64 ConvertRepairGold(mu_int64 Gold, mu_int32 Durability, mu_int32 MaxDurability, mu_int16 Type, mu_unicode *Text, const mu_uint32 TextLength);

mu_uint32 getGoldColor(mu_int64 Gold);

const mu_text* GetMapName(mu_int32 iMap);

#endif