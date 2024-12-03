#include "stdafx.h"
#include "CComGem.h"

namespace COMGEM
{
	mu_int32 iInvWidth = 190.0f;
	mu_int32 iInvHeight = 433.0f;
	mu_int32 iUnMixIndex = -1;
	mu_int32 iUnMixLevel = -1;

	mu_boolean m_bType = ATTACH;
	mu_int8 m_cState = STATE_READY;
	mu_int8 m_cErr = NOERR;
	mu_int8 m_cGemType = -1;
	mu_int8 m_cComType = -1;
	mu_uint8 m_cCount = 0;
	mu_int32 m_iValue = 0;
	mu_uint8 m_cPercent = 0;

#if 0 // FIX
	CUIUnmixgemList m_UnmixTarList;
#endif
}

void COMGEM::SendReqUnMix()
{
	iUnMixIndex += 12;
#if 0 // FIX
	SendRequestGemUnMix(m_cGemType, iUnMixLevel, iUnMixIndex);
#endif
}

void COMGEM::SendReqMix()
{
#if 0 // FIX
	SendRequestGemMix(m_cGemType, (m_cComType/10-1));
#endif
}

void COMGEM::ProcessCSAction()
{
	if(m_cState == STATE_HOLD || m_cErr != NOERR)
		return;
	SetState(STATE_HOLD);

	if(isComMode())
		SendReqMix();
	else
		SendReqUnMix();
}

void COMGEM::ResetWantedList()
{
#if 0 // FIX
	m_UnmixTarList.Clear();
#endif
}

mu_boolean COMGEM::FindWantedList()
{
#if 0 // FIX
	SEASON3B::CNewUIInventoryCtrl * pNewInventoryCtrl = g_pMyInventory->GetInventoryCtrl();
	ITEM * pItem = nullptr;

	for (mu_int32 i = 0; i < (mu_int32)pNewInventoryCtrl->GetNumberOfItems(); ++i)
	{
		pItem = pNewInventoryCtrl->GetItem(i);
		if (!pItem) continue;

		if ((pItem->Type == INDEX_COMPILED_CELE && m_cGemType == CELE)
			|| (pItem->Type == INDEX_COMPILED_SOUL && m_cGemType == SOUL))
		{
			INTBYTEPAIR p;
			mu_int32 Level = pItem->Level;
			p.first = pItem->y*pNewInventoryCtrl->GetNumberOfColumn() + pItem->x;
			p.second = Level;
			m_UnmixTarList.AddText(p.first, p.second);
		}
	}
#endif

	return false;
}

void COMGEM::SelectFromList(mu_int32 iIndex, mu_int32 iLevel)
{
	iUnMixIndex = iIndex;
	iUnMixLevel = iLevel;

	if(CheckInv())
	{

	}
}

mu_int32 COMGEM::GetUnMixGemLevel()
{
	return iUnMixLevel;
}

void COMGEM::MoveUnMixList()
{
#if 0 // FIX
	g_dwActiveUIID = m_UnmixTarList.GetUIID();
	m_UnmixTarList.DoAction();
	g_dwActiveUIID = 0;
#endif
}

void COMGEM::RenderUnMixList()
{
#if 0 // FIX
	m_UnmixTarList.Render();
#endif
}

mu_int8 COMGEM::CheckOneItem(const ITEM* p)
{
	if (p->Type == INDEX_COMPILED_CELE) return COMCELE;
	else if (p->Type == INDEX_COMPILED_SOUL) return COMSOUL;
	else if (p->Type == ITEM_POTION + 13) return CELE;
	else if (p->Type == ITEM_POTION + 14) return SOUL;

	return NOGEM;
}

mu_boolean COMGEM::CheckInv()
{
	if (!CheckMyInvValid())
	{
		switch (GetError())
		{
		case COMERROR_NOTALLOWED:
			g_pChatListBox->AddText(_T(""), GlobalText[1817], TYPE_ERROR_MESSAGE);
			break;
		case DEERROR_NOTALLOWED:
			g_pChatListBox->AddText(_T(""), GlobalText[1818], TYPE_ERROR_MESSAGE);
			break;
		}

		GetBack();

		return false;
	}

	return true;
}

mu_boolean COMGEM::CheckMyInvValid()
{
	m_cPercent = 0;
	m_cCount = 0;

#if 0 // FIX
	if (m_bType == ATTACH)
	{
		SEASON3B::CNewUIInventoryCtrl * pNewInventoryCtrl = g_pMyInventory->GetInventoryCtrl();
		ITEM * pItem = nullptr;

		for (mu_int32 i = 0; i < (mu_int32)pNewInventoryCtrl->GetNumberOfItems(); ++i)
		{
			pItem = pNewInventoryCtrl->GetItem(i);
			if ((pItem->Type == ITEM_POTION + 14 && m_cGemType == SOUL) ||
				(pItem->Type == ITEM_POTION + 13 && m_cGemType == CELE)) ++m_cCount;

			if (m_cCount == m_cComType)
			{
				m_cPercent = 100;
				CalcGen();
				return true;
			}
		}
		if ((m_cCount < m_cComType) || (m_cComType == NOCOM))
		{
			m_cPercent = 0;
			m_cErr = COMERROR_NOTALLOWED;
			return false;
		}
	}
	else if (m_bType == DETACH)
	{
		if (iUnMixIndex == -1 || iUnMixIndex >= MAX_INVENTORY)
		{
			m_cErr = DEERROR_NOTALLOWED;
			m_cPercent = 0;
			return false;
		}

		ITEM * pItem = g_pMyInventory->FindItem(iUnMixIndex);

		if ((pItem->Type == INDEX_COMPILED_CELE && m_cGemType == CELE) ||
			(pItem->Type == INDEX_COMPILED_SOUL && m_cGemType == SOUL))
		{
			++m_cCount;
			m_cPercent = 100;
			CalcGen();
			return true;
		}
		else
		{
			m_cErr = DEERROR_NOTALLOWED;
			m_cPercent = 0;
			return false;
		}
	}
	m_cErr = ERROR_UNKNOWN;
#endif

	return false;
}

void COMGEM::CalcGen()
{
	m_iValue = 0;

	if(m_bType)
	{
		m_iValue = 1000000;
	}
	else
	{
		m_iValue = m_cComType * 50000;
	}
}

mu_int8 COMGEM::CalcCompiledCount(const ITEM* p)
{
	mu_int32 Level = p->Level;

	switch (CheckOneItem(p))
	{
	case COMCELE:
	case COMSOUL:
		return (Level + 1) * FIRST;
	default:
		return 0;
	}
}

mu_int32	COMGEM::CalcItemValue(const ITEM* p)
{
	mu_int32 Level = p->Level;
	switch (CheckOneItem(p))
	{
	case NOGEM:
		return 0;
	case CELE:
		return 9000000;
	case SOUL:
		return 6000000;
	case COMCELE:
		return 9000000 * (Level + 1) * FIRST;
	case COMSOUL:
		return 6000000 * (Level + 1) * FIRST;
	default:
		return 0;
	}
}

mu_int32 COMGEM::CalcEmptyInv()
{
#if 0 // FIX
	SEASON3B::CNewUIInventoryCtrl * pNewInventoryCtrl = g_pMyInventory->GetInventoryCtrl();
	return pNewInventoryCtrl->GetEmptySlotCount();
#else
	return 0;
#endif
}

void COMGEM::Init()
{
	m_bType = ATTACH;
	m_cState = STATE_READY;
	m_cErr = NOERR;
	m_cGemType = -1;
	m_cComType = -1;
	m_cCount = 0;
	m_iValue = -1;
	m_cPercent = 0;
	iUnMixIndex = -1;
	iUnMixLevel = -1;
}

void COMGEM::GetBack()
{
	if (m_cState == STATE_HOLD)
	{
		m_cState = STATE_READY;
	}

	m_cErr = NOERR;
	m_cGemType = NOGEM;

	if (m_bType != ATTACH && m_bType != DETACH)
	{
		Exit();
	}
}

void COMGEM::Exit()
{
	Init();

#if 0 // FIX
	SendExitInventory();
#endif
}