#include "stdafx.h"
#include "mu_mixmanager.h"

MUMixItem::MUMixItem()
{
	Reset();
}

MUMixItem::MUMixItem(ITEM *Item, mu_uint32 MixValue)
{
	SetItem(Item, MixValue);
}

MUMixItem::~MUMixItem()
{

}

void MUMixItem::Reset()
{
	m_sType = 0;
	m_iLevel = 0;
	m_iOption = 0;
	m_iDurability = 0;
	m_dwSpecialItem = 0;
	m_b380AddedItem = false;
	m_bFenrirAddedItem = false;
	m_bIsCharmItem = false;
	m_bIsChaosCharmItem = false;
	m_bIsJewelItem = false;
	m_wHarmonyOption = 0;
	m_wHarmonyOptionLevel = 0;
	m_bMixLuck = false;
	m_bIsEquipment = false;
	m_bIsWing = false;
	m_bIsUpgradedWing = false;
	m_bIs3rdUpgradedWing = false;
	m_bySocketCount = 0;
	for (int i = 0; i < MAX_SOCKETS; ++i)
	{
		m_bySocketSeedID[i] = SOCKET_EMPTY;
		m_bySocketSphereLv[i] = 0;
	}
	m_bCanStack = false;
	m_dwMixValue = 0;
	m_iCount = 0;
	m_iTestCount = 0;
}

void MUMixItem::SetItem(ITEM *Item, mu_uint32 MixValue)
{
	Reset();

	m_sType = Item->Type;
	m_iLevel = Item->Level;
	m_iDurability = Item->Durability;
	for (int i = 0; i < Item->SpecialNum; ++i)
	{
		switch (Item->Special[i])
		{
		case AT_IMPROVE_MAGIC:
		case AT_IMPROVE_CURSE:
		case AT_IMPROVE_DAMAGE:
		case AT_IMPROVE_DEFENSE:
		case AT_IMPROVE_BLOCKING:
			m_iOption = Item->SpecialValue[i];
			break;
		case AT_LIFE_REGENERATION:
			m_iOption = Item->SpecialValue[i] * 4;
			break;
		case AT_LUCK:
			m_bMixLuck = true;
			break;
		}
	}
	if (Item->ExcOption > 0) m_dwSpecialItem |= RCP_SP_EXCELLENT;
	if (Item->RequireLevel >= 380) m_dwSpecialItem |= RCP_SP_ADD380ITEM;
	if ((Item->SetOption % 0x04) == EXT_A_SET_OPTION || (Item->SetOption % 0x04) == EXT_B_SET_OPTION) m_dwSpecialItem |= RCP_SP_SETITEM;
	m_b380AddedItem = Item->ItemOption;

	if (Item->Type >= ITEM_SWORD && Item->Type <= ITEM_BOOTS + MAX_ITEM_INDEX - 1)
		m_bIsEquipment = true;

	if (Item->Type == ITEM_HELPER + 37 && Item->ExcOption != 0)
		m_bFenrirAddedItem = true;

	if (Item->Type == ITEM_POTION + 53)
		m_bIsCharmItem = true;

	if (Item->Type == ITEM_POTION + 96)
		m_bIsChaosCharmItem = true;

	if (Item->Type == ITEM_WING + 15
		|| Item->Type == ITEM_WING + 30
		|| Item->Type == ITEM_WING + 31
		|| Item->Type == ITEM_POTION + 13
		|| Item->Type == ITEM_POTION + 14
		|| Item->Type == ITEM_POTION + 16
		|| Item->Type == ITEM_POTION + 22
		|| Item->Type == ITEM_POTION + 31
		|| Item->Type == ITEM_POTION + 42
		)
		m_bIsJewelItem = true;

	m_bySocketCount = Item->SocketCount;
	if (m_bySocketCount > 0)
	{
		m_dwSpecialItem |= RCP_SP_SOCKETITEM;
		for (int i = 0; i < MAX_SOCKETS; ++i)
		{
			m_bySocketSeedID[i] = Item->SocketSeedID[i];
			m_bySocketSphereLv[i] = Item->SocketSphereLv[i];
		}

		m_dwSpecialItem ^= RCP_SP_ADD380ITEM;
	}
	m_bySeedSphereID = g_SocketItemMgr.GetSeedShpereSeedID(Item);

	if (Item->HarmonyOption > 0)
	{
		m_dwSpecialItem |= RCP_SP_HARMONY;
		m_wHarmonyOption = Item->HarmonyOption;
		m_wHarmonyOptionLevel = Item->HarmonyOptionLevel;
	}

	switch (Item->Type)
	{
	case ITEM_WING:
	case ITEM_WING + 1:
	case ITEM_WING + 2:
	case ITEM_WING + 41:
		m_bIsWing = true;
		break;
	case ITEM_WING + 3:
	case ITEM_WING + 4:
	case ITEM_WING + 5:
	case ITEM_WING + 6:
	case ITEM_HELPER + 30:
	case ITEM_WING + 42:
		m_bIsUpgradedWing = true;
		break;
	case ITEM_WING + 36:
	case ITEM_WING + 37:
	case ITEM_WING + 38:
	case ITEM_WING + 39:
	case ITEM_WING + 40:
	case ITEM_WING + 43:
		m_bIs3rdUpgradedWing = true;
		break;
	}

	if (m_bIsWing || m_bIsUpgradedWing || m_bIs3rdUpgradedWing)
	{
		if (m_dwSpecialItem & RCP_SP_EXCELLENT)
			m_dwSpecialItem ^= RCP_SP_EXCELLENT;
	}

	switch (Item->Type)
	{
	case ITEM_POTION + 3:
	case ITEM_POTION + 38:
	case ITEM_POTION + 39:
	case ITEM_POTION + 53:
	case ITEM_POTION + 88:
	case ITEM_POTION + 89:
	case ITEM_POTION + 90:
	case ITEM_POTION + 100:
		m_bCanStack = true;
		break;
	}
	m_dwMixValue = MixValue;

	if (m_bCanStack == true) m_iCount = m_iDurability;
	else m_iCount = 1;
}