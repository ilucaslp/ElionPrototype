#ifndef __MU_GUILD_H__
#define __MU_GUILD_H__

#pragma once

#define GUILDMARK_WIDTH 8
#define GUILDMARK_HEIGHT 8

typedef struct
{
	mu_string Name;
	mu_int32 CharacterIndex;
	mu_int16 GroupIndex;
	mu_int16 ServerIndex;
	mu_int16 GuildStatus;
} GUILD_LIST_t;

typedef struct 
{
	mu_uint32 GuildIndex;
	mu_uint32 MembersCount;
	mu_uint8 MarkBuffer[MAX_GUILDMARKBUFFER];
	mu_string GuildName;
} ALLIANCE_LIST_t;

class MARK_t
{
public:
	MARK_t()
	{
		Reset();
	}

	void Reset()
	{
		GuildIndex = -1;
		mu_zeromem(Mark, sizeof(Mark));
		ReferenceCount = 0;
	}

public:
	mu_int32 GuildIndex;
	mu_string UnionName;
	mu_string GuildName;
	mu_string RivalName;
	mu_uint8 Mark[64];
	mu_uint32 ReferenceCount;
};

extern MARK_t GuildMark[MAX_MARKS];
extern EStagingBufferBackend GuildMarkStagingBuffer[MAX_MARKS][2];
extern mu_int32 GuildTotalScore;

class MUGuildCache
{
public:
	MUGuildCache();
	~MUGuildCache();

	void Reset();

	mu_boolean ExistsGuildMark(mu_int32 GuildIndex);
	mu_int32 GetGuildMarkIndex(mu_int32 GuildIndex);
	mu_int32 AddGuildMarkIndex(mu_int32 GuildIndex);
	void ReleaseGuildMarkIndex(mu_int32 GuildIndex);

	void SetGuildMark(mu_int32 GuildIndex, const mu_string &UnionName, const mu_string &GuildName, const mu_uint8 MarkBuffer[MAX_GUILDMARKBUFFER]);

private:
	mu_int32 _CurrentIndex;
	std::stack<mu_int32> _FreeIndexes;
	std::map<mu_int32, mu_int32> _GuildToMarkMap;
};

extern MUGuildCache g_GuildCache;

#endif