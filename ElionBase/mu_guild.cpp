#include "stdafx.h"
#include "mu_guild.h"

MARK_t GuildMark[MAX_MARKS];
EStagingBufferBackend GuildMarkStagingBuffer[MAX_MARKS][2];
mu_int32 GuildTotalScore = 0;

MUGuildCache g_GuildCache;

MUGuildCache::MUGuildCache()
{
	Reset();
}

MUGuildCache::~MUGuildCache()
{
}

void MUGuildCache::Reset()
{
	_CurrentIndex = 0;
	while (_FreeIndexes.empty() == false) _FreeIndexes.pop();
	_GuildToMarkMap.clear();
}

mu_boolean MUGuildCache::ExistsGuildMark(mu_int32 GuildIndex)
{
	auto it = _GuildToMarkMap.find(GuildIndex);

	return it != _GuildToMarkMap.end();
}

mu_int32 MUGuildCache::GetGuildMarkIndex(mu_int32 GuildIndex)
{
	auto it = _GuildToMarkMap.find(GuildIndex);

	if (it == _GuildToMarkMap.end())
		return -1;

	return it->second;
}

mu_int32 MUGuildCache::AddGuildMarkIndex(mu_int32 GuildIndex)
{
	mu_int32 markIndex = GetGuildMarkIndex(GuildIndex);

	if (markIndex == -1)
	{
		if (_FreeIndexes.empty() == false)
		{
			markIndex = _FreeIndexes.top();
			_FreeIndexes.pop();
		}
		else if (_CurrentIndex < MAX_MARKS)
		{
			markIndex = _CurrentIndex++;
		}

		if (markIndex == -1)
		{
			return -1;
		}

		GuildMark[markIndex].GuildIndex = GuildIndex;
		_GuildToMarkMap.insert(std::make_pair(GuildIndex, markIndex));
	}

	MARK_t &mark = GuildMark[markIndex];

	++mark.ReferenceCount;

	return markIndex;
}

void MUGuildCache::ReleaseGuildMarkIndex(mu_int32 GuildIndex)
{
	mu_int32 markIndex = GetGuildMarkIndex(GuildIndex);

	if (markIndex != -1)
	{
		MARK_t &mark = GuildMark[markIndex];

		if (mark.ReferenceCount > 0)
		{
			if (--mark.ReferenceCount == 0)
			{
				_GuildToMarkMap.erase(mark.GuildIndex);
				mark.Reset();
			}
		}
	}
}

void MUGuildCache::SetGuildMark(mu_int32 GuildIndex, const mu_string &UnionName, const mu_string &GuildName, const mu_uint8 MarkBuffer[MAX_GUILDMARKBUFFER])
{
	mu_int32 markIndex = GetGuildMarkIndex(GuildIndex);

	if (markIndex != -1)
	{
		MARK_t &mark = GuildMark[markIndex];

		mark.GuildName = GuildName;
		mark.UnionName = UnionName;
		mu_memcpy(mark.Mark, MarkBuffer, MAX_GUILDMARKBUFFER);

		if (Hero->GuildMarkIndex == markIndex)
		{
			g_pGuildWindow->SetMarkTexture(&TEXTURES::Data[BITMAP_GUILD2_BEGIN + markIndex]);

			CreateGuildMark(MarkBuffer, true, &TEXTURES::Data[BITMAP_GUILD_BEGIN + markIndex], &GuildMarkStagingBuffer[markIndex][0]);
			CreateGuildMark(MarkBuffer, false, &TEXTURES::Data[BITMAP_GUILD2_BEGIN + markIndex], &GuildMarkStagingBuffer[markIndex][1]);
		}
	}
}