#ifndef __CSCHAOSCASTLE_H__
#define __CSCHAOSCASTLE_H__

#pragma once

extern EParallelVariable<mu_boolean, MAX_TASKING_THREADS> g_ClearActionObject;

EINLINE mu_boolean InChaosCastle(mu_int32 iMap = GET_WORLD)
{
	if ((iMap >= WD_18CHAOS_CASTLE && iMap <= WD_18CHAOS_CASTLE_END) || iMap == WD_53CAOSCASTLE_MASTER_LEVEL)
	{
		return true;
	}

	return false;
}

EINLINE mu_boolean InBloodCastle(mu_int32 iMap = GET_WORLD)
{
	if ((iMap >= WD_11BLOODCASTLE1 && iMap <= WD_11BLOODCASTLE_END) || iMap == WD_52BLOODCASTLE_MASTER_LEVEL)
	{
		return true;
	}

	return false;
}

EINLINE mu_boolean InDevilSquare()
{
	return GET_WORLD == WD_9DEVILSQUARE;
}

void ClearActionObject(const mu_uint32 ThreadIndex);
void SetActionObject(mu_int32 iWorld, mu_int32 iType, mu_int32 iLifeTime, mu_int32 iVel);
void PostProcessActionObject(const mu_uint32 ThreadIndex);
void ActionObject(const mu_uint32 ThreadIndex, OBJECT* o);

void ClearChaosCastleHelper(CHARACTER* c);
void ChangeChaosCastleUnit(CHARACTER* c);
void CreateChaosCastleObject(mu_uint32 objectIndex, OBJECT* o);
void MoveChaosCastleObject(OBJECT* o);
void RenderChaosCastleVisual(const mu_uint32 ThreadIndex, OBJECT* o, MUModel* b);
void RenderChaosCastleVisual(const mu_uint32 ThreadIndex);
void SetCurrCastleLevel(mu_uint8 byCurrCastleLevel);
void SetActionMatch(mu_boolean bActionMatch);

#endif