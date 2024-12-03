#ifndef __MU_DUELMANAGER_H__
#define __MU_DUELMANAGER_H__

#pragma once

struct DUEL_PLAYER_INFO
{
	DUEL_PLAYER_INFO()
	{
		Clear();
	}

	void Clear()
	{
		m_szID.clear();
		m_iIndex = 0;
		m_iScore = 0;
		m_fHPRate = 0;
		m_fSDRate = 0;
	}

	mu_string m_szID;
	mu_int32 m_iIndex;
	mu_int32 m_iScore;
	mu_float m_fHPRate;
	mu_float m_fSDRate;
};

enum _DUEL_PLAYER_TYPE
{
	DUEL_HERO,
	DUEL_ENEMY,
	MAX_DUEL_PLAYERS
};

struct DUEL_CHANNEL_INFO
{
	DUEL_CHANNEL_INFO()
	{
		Clear();
	}

	void Clear()
	{
		m_bEnable = false;
		m_bJoinable = false;
		m_szID1.clear();
		m_szID2.clear();
	}

	mu_boolean m_bEnable;
	mu_boolean m_bJoinable;
	mu_string m_szID1;
	mu_string m_szID2;
};

#define MAX_DUEL_CHANNELS 4

class CDuelMgr
{
public:
	CDuelMgr();
	~CDuelMgr();

	void Reset();

public:
	void EnableDuel(mu_boolean bEnable);
	mu_boolean IsDuelEnabled();

	void EnablePetDuel(mu_boolean bEnable);
	mu_boolean IsPetDuelEnabled();

	void SetDuelPlayer(mu_int32 iPlayerNum, mu_int32 iIndex, mu_text * pszID);
	void SetHeroAsDuelPlayer(mu_int32 iPlayerNum);
	void SetScore(mu_int32 iPlayerNum, mu_int32 iScore);
	void SetHP(mu_int32 iPlayerNum, mu_int32 iRate);
	void SetSD(mu_int32 iPlayerNum, mu_int32 iRate);

	const mu_text *GetDuelPlayerID(mu_int32 iPlayerNum);
	mu_int32 GetScore(mu_int32 iPlayerNum);
	mu_float GetHP(mu_int32 iPlayerNum);
	mu_float GetSD(mu_int32 iPlayerNum);

	mu_boolean IsDuelPlayer(CHARACTER * pCharacter, mu_int32 iPlayerNum, mu_boolean bIncludeSummon = true);
	mu_boolean IsDuelPlayer(mu_uint16 wIndex, mu_int32 iPlayerNum);

	void SendDuelRequestAnswer(mu_int32 iPlayerNum, mu_boolean bOK);

protected:
	mu_boolean m_bIsDuelEnabled;
	mu_boolean m_bIsPetDuelEnabled;
	DUEL_PLAYER_INFO m_DuelPlayer[MAX_DUEL_PLAYERS];

public:
	void SetDuelChannel(mu_int32 iChannelIndex, mu_boolean bEnable, mu_boolean bJoinable, mu_text *pszID1, mu_text *pszID2);
	mu_boolean IsDuelChannelEnabled(mu_int32 iChannelIndex) { return m_DuelChannels[iChannelIndex].m_bEnable; }
	mu_boolean IsDuelChannelJoinable(mu_int32 iChannelIndex) { return m_DuelChannels[iChannelIndex].m_bJoinable; }
	const mu_text *GetDuelChannelUserID1(mu_int32 iChannelIndex) { return m_DuelChannels[iChannelIndex].m_szID1.c_str(); }
	const mu_text *GetDuelChannelUserID2(mu_int32 iChannelIndex) { return m_DuelChannels[iChannelIndex].m_szID2.c_str(); }

	void SetCurrentChannel(mu_int32 iChannel = -1) { m_iCurrentChannel = iChannel; }
	mu_int32 GetCurrentChannel() { return m_iCurrentChannel; }

	void RemoveAllDuelWatchUser();
	void AddDuelWatchUser(mu_text *pszUserID);
	void RemoveDuelWatchUser(mu_text *pszUserID);
	const mu_text *GetDuelWatchUser(mu_int32 iIndex);
	mu_int32 GetDuelWatchUserCount() { return m_DuelWatchUserList.size(); }

	mu_boolean GetFighterRegenerated() { return m_bRegenerated; }
	void SetFighterRegenerated(mu_boolean bFlag) { m_bRegenerated = bFlag; }

protected:
	DUEL_CHANNEL_INFO m_DuelChannels[MAX_DUEL_CHANNELS];
	mu_int32 m_iCurrentChannel;

	mu_boolean m_bRegenerated;

	std::list<mu_string> m_DuelWatchUserList;
};

extern CDuelMgr g_DuelMgr;

#endif