#include "stdafx.h"
#include "mu_changeringmanager.h"

CChangeRingManager g_ChangeRingMgr;

CChangeRingManager::CChangeRingManager()
{

}

CChangeRingManager::~CChangeRingManager()
{

}

mu_boolean CChangeRingManager::CheckDarkLordHair(mu_int32 iType)
{
	if ((iType >= MODEL_SKELETON1 && iType <= MODEL_SKELETON3) || iType == MODEL_SKELETON_PCBANG)
	{
		return true;
	}

	if (iType == MODEL_HALLOWEEN)
	{
		return true;
	}

	if (iType == MODEL_XMAS_EVENT_CHANGE_GIRL)
	{
		return true;
	}

	if (iType == MODEL_GM_CHARACTER)
	{
		return true;
	}

	if (iType == MODEL_CURSEDTEMPLE_ALLIED_PLAYER || iType == MODEL_CURSEDTEMPLE_ILLUSION_PLAYER)
	{
		return true;
	}


	if (iType == MODEL_XMAS2008_SNOWMAN)
	{
		return true;
	}

	if (iType == MODEL_PANDA)
	{
		return true;
	}

	if (iType == MODEL_SKELETON_CHANGED)
	{
		return true;
	}

	return false;
}

mu_boolean CChangeRingManager::CheckDarkCloak(mu_int32 iClass, mu_int32 iType)
{
	if (GetCharacterClass(iClass) == GAME::CLASS_DARK)
	{
		if (iType == MODEL_HALLOWEEN)
		{
			return true;
		}

		if (iType == MODEL_XMAS_EVENT_CHANGE_GIRL)
		{
			return true;
		}

		if (iType == MODEL_GM_CHARACTER)
		{
			return true;
		}

		if (iType == MODEL_XMAS2008_SNOWMAN)
		{
			return true;
		}

		if (iType == MODEL_PANDA)
		{
			return true;
		}

		if (iType == MODEL_SKELETON_CHANGED)
		{
			return true;
		}
	}

	return false;
}

mu_boolean CChangeRingManager::CheckChangeRing(mu_int16 RingType)
{
	if (RingType == ITEM_HELPER + 10)
	{
		return true;
	}

	if (RingType == ITEM_HELPER + 39)
	{
		return true;
	}

	if (RingType == ITEM_HELPER + 40)
	{
		return true;
	}

	if (RingType == ITEM_HELPER + 41)
	{
		return true;
	}

	if (RingType == ITEM_HELPER + 42)
	{
		return true;
	}

	if (RingType == ITEM_HELPER + 68)
	{
		return true;
	}

	if (RingType == ITEM_HELPER + 76)
	{
		return true;
	}

	if (RingType == ITEM_HELPER + 122)
	{
		return true;
	}

	return false;
}

mu_boolean CChangeRingManager::CheckMoveMap(mu_int16 sLeftRingType, mu_int16 sRightRingType)
{
	if (sLeftRingType == ITEM_HELPER + 10 || sRightRingType == ITEM_HELPER + 10)
	{
		return true;
	}

	if (sLeftRingType == ITEM_HELPER + 39 || sRightRingType == ITEM_HELPER + 39)
	{
		return true;
	}

	if (sLeftRingType == ITEM_HELPER + 40 || sRightRingType == ITEM_HELPER + 40)
	{
		return true;
	}

	if (sLeftRingType == ITEM_HELPER + 41 || sRightRingType == ITEM_HELPER + 41)
	{
		return true;
	}

	if (sLeftRingType == ITEM_HELPER + 42 || sRightRingType == ITEM_HELPER + 42)
	{
		return true;
	}

	if (sLeftRingType == ITEM_HELPER + 68 || sRightRingType == ITEM_HELPER + 68)
	{
		return true;
	}

	if (sLeftRingType == ITEM_HELPER + 76 || sRightRingType == ITEM_HELPER + 76)
	{
		return true;
	}

	if (sLeftRingType == ITEM_HELPER + 122 || sRightRingType == ITEM_HELPER + 122)
	{
		return true;
	}

	return false;
}

mu_boolean CChangeRingManager::CheckBanMoveIcarusMap(mu_int16 sLeftRingType, mu_int16 sRightRingType)
{
	if (sLeftRingType == ITEM_HELPER + 10 || sRightRingType == ITEM_HELPER + 10)
	{
		return true;
	}

	if (sLeftRingType == ITEM_HELPER + 39 || sRightRingType == ITEM_HELPER + 39)
	{
		return true;
	}

	if (sLeftRingType == ITEM_HELPER + 40 || sRightRingType == ITEM_HELPER + 40)
	{
		return true;
	}

	if (sLeftRingType == ITEM_HELPER + 41 || sRightRingType == ITEM_HELPER + 41)
	{
		return true;
	}

	if (sLeftRingType == ITEM_HELPER + 42 || sRightRingType == ITEM_HELPER + 42)
	{
		return true;
	}

	if (sLeftRingType == ITEM_HELPER + 68 || sRightRingType == ITEM_HELPER + 68)
	{
		return true;
	}

	return false;
}