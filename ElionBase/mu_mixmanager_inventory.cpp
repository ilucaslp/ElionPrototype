#include "stdafx.h"
#include "mu_mixmanager.h"

MUMixInventory::MUMixInventory()
{
	Reset();
}

MUMixInventory::~MUMixInventory()
{
	Reset();
}

void MUMixInventory::Reset()
{
	m_iNumMixItems = 0;
}

mu_int32 MUMixInventory::AddItem(ITEM *pItem)
{
	mu_boolean bFind = false;

	for (int i = 0; i < m_iNumMixItems; ++i)
	{
		if (m_MixItems[i] == pItem)
		{
			bFind = true;
			m_MixItems[i].m_dwMixValue += EvaluateMixItemValue(pItem);
			if (m_MixItems[i].m_bCanStack == true)
			{
				m_MixItems[i].m_iCount += pItem->Durability;
			}
			else
			{
				++m_MixItems[i].m_iCount;
			}
			break;
		}
	}

	if (bFind == false)
	{
		m_MixItems[m_iNumMixItems++].SetItem(pItem, EvaluateMixItemValue(pItem));
	}

	return 0;
}

mu_int32 MUMixInventory::GetNumMixItems()
{
	return m_iNumMixItems;
}

MUMixItem *MUMixInventory::GetMixItems()
{
	return m_MixItems;
}

mu_uint32 MUMixInventory::EvaluateMixItemValue(ITEM *pItem)
{
	mu_uint32 dwMixValue = 0;
	switch (pItem->Type)
	{
	case ITEM_WING + 15:
		dwMixValue = 40000;
		break;
	case ITEM_POTION + 13:
		dwMixValue = 100000;
		break;
	case ITEM_POTION + 14:
		dwMixValue = 70000;
		break;
	case ITEM_POTION + 22:
		dwMixValue = 450000;
		break;
	case ITEM_POTION + 16:
		dwMixValue = 0;
		break;
	case ITEM_POTION + 31:
	default:
		dwMixValue = ItemValue(pItem, 0);
		break;
	}
	return dwMixValue;
}