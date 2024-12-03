// DuelMgr.cpp: implementation of the CDuelMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "mu_duelmanager.h"

CDuelMgr g_DuelMgr;

CDuelMgr::CDuelMgr()
{
	Reset();
}

CDuelMgr::~CDuelMgr()
{
	Reset();
}

void CDuelMgr::Reset()
{
	m_bIsDuelEnabled = false;
	m_bIsPetDuelEnabled = false;
	for (mu_uint32 n = 0; n < mu_countof(m_DuelPlayer); ++n)
	{
		m_DuelPlayer[n].Clear();
	}
	for (mu_uint32 n = 0; n < mu_countof(m_DuelChannels); ++n)
	{
		m_DuelChannels[n].Clear();
	}
	m_iCurrentChannel = -1;
	m_bRegenerated = false;
	RemoveAllDuelWatchUser();
}

void CDuelMgr::EnableDuel(mu_boolean bEnable)
{
	m_bIsDuelEnabled = bEnable;

	if (bEnable == false)
	{
		Reset();
	}
}

mu_boolean CDuelMgr::IsDuelEnabled()
{
	return m_bIsDuelEnabled;
}

void CDuelMgr::EnablePetDuel(mu_boolean bEnable)
{
	m_bIsPetDuelEnabled = bEnable;
}

mu_boolean CDuelMgr::IsPetDuelEnabled()
{
	return m_bIsPetDuelEnabled;
}

void CDuelMgr::SetDuelPlayer(mu_int32 iPlayerNum, mu_int32 iIndex, mu_text *pszID)
{
	m_DuelPlayer[iPlayerNum].m_iIndex = iIndex;
	m_DuelPlayer[iPlayerNum].m_szID = pszID;
}

void CDuelMgr::SetHeroAsDuelPlayer(mu_int32 iPlayerNum)
{
	m_DuelPlayer[iPlayerNum].m_iIndex = Hero->Key;
	m_DuelPlayer[iPlayerNum].m_szID = Hero->ID;
}

void CDuelMgr::SetScore(mu_int32 iPlayerNum, mu_int32 iScore)
{
	m_DuelPlayer[iPlayerNum].m_iScore = iScore;
}

void CDuelMgr::SetHP(mu_int32 iPlayerNum, mu_int32 iRate)
{
	m_DuelPlayer[iPlayerNum].m_fHPRate = iRate * 0.01f;
}

void CDuelMgr::SetSD(mu_int32 iPlayerNum, mu_int32 iRate)
{
	m_DuelPlayer[iPlayerNum].m_fSDRate = iRate * 0.01f;
}

const mu_text *CDuelMgr::GetDuelPlayerID(mu_int32 iPlayerNum)
{
	return m_DuelPlayer[iPlayerNum].m_szID.c_str();
}

mu_int32 CDuelMgr::GetScore(mu_int32 iPlayerNum)
{
	return m_DuelPlayer[iPlayerNum].m_iScore;
}

mu_float CDuelMgr::GetHP(mu_int32 iPlayerNum)
{
	return m_DuelPlayer[iPlayerNum].m_fHPRate;
}

mu_float CDuelMgr::GetSD(mu_int32 iPlayerNum)
{
	return m_DuelPlayer[iPlayerNum].m_fSDRate;
}

mu_boolean CDuelMgr::IsDuelPlayer(CHARACTER * pCharacter, mu_int32 iPlayerNum, mu_boolean bIncludeSummon)
{
	if (pCharacter->Key == m_DuelPlayer[iPlayerNum].m_iIndex &&
		mu_strncmp(pCharacter->ID, m_DuelPlayer[iPlayerNum].m_szID.c_str(), ACCOUNT_MAXIMUM) == 0)
	{
		return true;
	}
	else if (bIncludeSummon == true &&
		GetBaseClass(pCharacter->Class) == 0 &&
		mu_strncmp(pCharacter->OwnerID, m_DuelPlayer[iPlayerNum].m_szID.c_str(), ACCOUNT_MAXIMUM) == 0)
	{
		return true;
	}

	return false;
}

mu_boolean CDuelMgr::IsDuelPlayer(mu_uint16 wIndex, mu_int32 iPlayerNum)
{
	return (m_DuelPlayer[iPlayerNum].m_iIndex == wIndex);
}

void CDuelMgr::SendDuelRequestAnswer(mu_int32 iPlayerNum, mu_boolean bOK)
{
#if 0 // FIX
	SendRequestDuelOk(bOK, m_DuelPlayer[iPlayerNum].m_iIndex, m_DuelPlayer[iPlayerNum].m_szID);
#endif
}

void CDuelMgr::SetDuelChannel(mu_int32 iChannelIndex, mu_boolean bEnable, mu_boolean bJoinable, mu_text *pszID1, mu_text *pszID2)
{
	m_DuelChannels[iChannelIndex].m_bEnable = bEnable;
	m_DuelChannels[iChannelIndex].m_bJoinable = bJoinable;
	m_DuelChannels[iChannelIndex].m_szID1 = pszID1;
	m_DuelChannels[iChannelIndex].m_szID2 = pszID2;
}

void CDuelMgr::RemoveAllDuelWatchUser()
{
	m_DuelWatchUserList.clear();
}

void CDuelMgr::AddDuelWatchUser(mu_text *pszUserID)
{
	m_DuelWatchUserList.push_back(pszUserID);
}

void CDuelMgr::RemoveDuelWatchUser(mu_text *pszUserID)
{
	for (auto iter = m_DuelWatchUserList.begin(); iter != m_DuelWatchUserList.end(); ++iter)
	{
		if (mu_strncmp(iter->c_str(), pszUserID, ACCOUNT_MAXIMUM) == 0)
		{
			m_DuelWatchUserList.erase(iter);
			return;
		}
	}
}

const mu_text *CDuelMgr::GetDuelWatchUser(mu_int32 iIndex)
{
	if (m_DuelWatchUserList.size() <= iIndex)
		return nullptr;

	mu_int32 i = 0;
	for (auto iter = m_DuelWatchUserList.begin(); iter != m_DuelWatchUserList.end(); ++iter, ++i)
	{
		if (i == iIndex)
		{
			return iter->c_str();
		}
	}

	return nullptr;
}