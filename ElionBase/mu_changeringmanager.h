#ifndef __MU_CHANGERINGMANAGER_H__
#define __MU_CHANGERINGMANAGER_H__

#pragma once

class CChangeRingManager  
{
public:
	CChangeRingManager();
	~CChangeRingManager();

	mu_boolean CheckDarkLordHair(mu_int32 iType);
	mu_boolean CheckDarkCloak(mu_int32 iClass, mu_int32 iType);
	mu_boolean CheckChangeRing(mu_int16 RingType);
	mu_boolean CheckMoveMap(mu_int16 sLeftRingType, mu_int16 sRightRingType);
	mu_boolean CheckBanMoveIcarusMap(mu_int16 sLeftRingType, mu_int16 sRightRingType);
};

extern CChangeRingManager g_ChangeRingMgr;

#endif