#include "stdafx.h"
#include "mu_information.h"

#define DISABLE_FILTERS

mu_boolean EnableGuildWar = false;
mu_int32 GuildWarIndex = -1;
mu_text GuildWarName[16 + 1];
mu_int32 GuildWarScore[2];

mu_boolean EnableSoccer = false;
mu_uint8 HeroSoccerTeam = 0;
mu_int32 SoccerTime;
mu_text SoccerTeamName[2][16 + 1];
mu_boolean SoccerObserver = false;

std::vector<mu_string> AbuseFilter;
std::vector<mu_string> AbuseNameFilter;

mu_boolean IsStrifeMap(mu_int32 nMapIndex)
{
#if 0 // FIX
	if (!g_ServerListManager->IsNonPvP())
	{
		switch (nMapIndex)
		{
		case WD_63PK_FIELD:
			return true;
		}
	}
#endif

	return false;
}

void GetSkillInformation(mu_int32 iType, mu_int32 iLevel, mu_text *lpszName, const mu_size NameLength, mu_int32 *piMana, mu_int32 *piDistance, mu_int32 *piSkillMana)
{
	SKILL_ATTRIBUTE *p = &SkillAttribute[iType];
	if (lpszName)
	{
		mu_strcpy_s(lpszName, NameLength, p->Name);
	}
	if (piMana)
	{
		*piMana = p->Mana;
	}
	if (piDistance)
	{
		*piDistance = p->Distance;
	}
	if (piSkillMana)
	{
		*piSkillMana = p->AbilityGuage;
	}

}

void GetSkillInformation_Energy(mu_int32 iType, mu_int32 *piEnergy)
{
	SKILL_ATTRIBUTE *p = &SkillAttribute[iType];

	if (piEnergy)
	{
		if (p->Energy == 0)
		{
			*piEnergy = 0;
		}
		else
		{
			*piEnergy = 20 + (p->Energy*p->Level * 4 / 100);

			if (iType == AT_SKILL_SUMMON_EXPLOSION || iType == AT_SKILL_SUMMON_REQUIEM)
			{
				*piEnergy = 20 + (p->Energy*p->Level * 3 / 100);
			}

			if (GetBaseClass(Hero->Class) == GAME::CLASS_KNIGHT)
			{
				*piEnergy = 10 + (p->Energy*p->Level * 4 / 100);
			}
		}
	}

}

void GetSkillInformation_Charisma(mu_int32 iType, mu_int32 *piCharisma)
{
	SKILL_ATTRIBUTE *p = &SkillAttribute[iType];

	if (piCharisma)
	{
		*piCharisma = p->Charisma;
	}

}

void GetSkillInformation_Damage(mu_int32 iType, mu_int32 *piDamage)
{
	SKILL_ATTRIBUTE *p = &SkillAttribute[iType];

	if (piDamage)
	{
		*piDamage = p->Damage;
	}

}

mu_float GetSkillDistance(mu_int32 Index, CHARACTER* c)
{
	mu_float Distance = (mu_float)(SkillAttribute[Index].Distance);

	if (c != nullptr)
	{
		if (c->Helper.Type == MODEL_HELPER + 4)
		{
			Distance += 2;
		}
	}

	return Distance;
}

mu_boolean CheckSkillDelay(mu_int32 SkillIndex)
{
	mu_int32 Skill = CharacterAttribute->Skill[SkillIndex];
	mu_int32 Delay = SkillAttribute[Skill].Delay;

	if (Delay > 0)
	{
		if (CharacterAttribute->SkillDelay[SkillIndex] > 0)
		{
			return false;
		}

		mu_int32 iCharisma;
		GetSkillInformation_Charisma(Skill, &iCharisma);
		if (iCharisma > (CharacterAttribute->Charisma + CharacterAttribute->AddCharisma))
		{
			return false;
		}

		CharacterAttribute->SkillDelay[SkillIndex] = Delay;
	}
	return true;
}

void CalcSkillDelay(mu_int32 time)
{
	mu_int32 iSkillNumber;
	iSkillNumber = CharacterAttribute->SkillNumber + 2;
	iSkillNumber = EMath::Min(iSkillNumber, MAX_SKILLS);

	for (mu_int32 i = 0; i < iSkillNumber; ++i)
	{
		if (CharacterAttribute->SkillDelay[i] <= 0)
			continue;

		CharacterAttribute->SkillDelay[i] -= time;
		if (CharacterAttribute->SkillDelay[i] < 0)
		{
			CharacterAttribute->SkillDelay[i] = 0;
		}
	}
}

mu_boolean IsGuildMaster()
{
	return (Hero->GuildStatus == GAME::G_MASTER);
}

mu_boolean IsSubGuildMaster()
{
	return (Hero->GuildStatus == GAME::G_SUB_MASTER);
}

mu_boolean IsBattleMaster()
{
	return (Hero->GuildStatus == GAME::G_BATTLE_MASTER);
}

mu_boolean g_bTimeCheck = false;
mu_uint64 g_iBackupTime = 0;

mu_boolean GetTimeCheck(mu_uint64 DelayTime)
{
	mu_uint64 PresentTime = g_CurrentTime;

	if (g_bTimeCheck)
	{
		g_iBackupTime = PresentTime;
		g_bTimeCheck = false;
	}

	if (g_iBackupTime + DelayTime <= PresentTime)
	{
		g_bTimeCheck = true;
		return true;
	}
	return false;
}

mu_boolean OpenFilterFile(const mu_text *filename)
{
#ifndef DISABLE_FILTERS
	SDL_RWops *file = nullptr;

	if (mu_rwfromfile(&file, filename, "rb") == false)
	{
		return false;
	}

	static const mu_size HeaderSize = FileHeaders::HEADER_SIZE;
	static const mu_size CompressedHeaderSize = FileHeaders::COMPRESSED_HEADER_SIZE;

	mu_int64 fileLength = SDL_RWsize(file);
	mu_int64 encryptedLength = fileLength - HeaderSize;

	if (encryptedLength % MUBlowfish::BytesLength != 0)
	{
		return false;
	}

	mu_uint8 *dataBuffer = new mu_uint8[fileLength];
	SDL_RWread(file, dataBuffer, fileLength, 1);
	SDL_RWclose(file);

	FileHeaders::FileHeader *header = (FileHeaders::FileHeader*)dataBuffer;

	if (header->Signature != FILE_HEADERS::DATA_HEADER)
	{
		delete[] dataBuffer;
		return false;
	}

	if (GenerateCRC32(dataBuffer + HeaderSize, encryptedLength) != header->CRC)
	{
		delete[] dataBuffer;
		return false;
	}

	g_DataCrypt.Decode(dataBuffer + HeaderSize, dataBuffer + HeaderSize, encryptedLength);

	FileHeaders::CompressedHeader *compressedHeader = (FileHeaders::CompressedHeader*)(dataBuffer + HeaderSize);

	mu_uint8 *decompressedBuffer = new mu_uint8[compressedHeader->DecompressedSize];

	mu_boolean decompressed = FileHeaders::Decompress(dataBuffer + HeaderSize + CompressedHeaderSize,
		decompressedBuffer,
		compressedHeader->CompressedSize,
		compressedHeader->DecompressedSize);
	delete[] dataBuffer;

	if (decompressed == false)
	{
		delete[] decompressedBuffer;
		return false;
	}

	mu_uint8 *readBuffer = decompressedBuffer;

	mu_char textBuffer[4096];
	mu_uint32 textCount = 0, textLength = 0;
	FileHeaders::ReadBuffer(readBuffer, textCount);

	for (mu_uint32 i = 0; i < textCount; ++i)
	{
		FileHeaders::ReadBuffer(readBuffer, textLength);

		if (textLength > 0)
		{
			FileHeaders::ReadBuffer(readBuffer, textBuffer, textLength);
			textBuffer[textLength] = _T('\0');
#ifdef UNICODE
			mu_string textData = boost::locale::conv::utf_to_utf<mu_text>(textBuffer);
			AbuseFilter.push_back(textData);
#else
			AbuseFilter.push_back(textBuffer);
#endif
		}
	}

	delete[] decompressedBuffer;
#endif

	return true;
}

mu_boolean OpenNameFilterFile(const mu_text *filename)
{
#ifndef DISABLE_FILTERS
	SDL_RWops *file = nullptr;

	if (mu_rwfromfile(&file, filename, "rb") == false)
	{
		return false;
	}

	static const mu_size HeaderSize = FileHeaders::HEADER_SIZE;
	static const mu_size CompressedHeaderSize = FileHeaders::COMPRESSED_HEADER_SIZE;

	mu_int64 fileLength = SDL_RWsize(file);
	mu_int64 encryptedLength = fileLength - HeaderSize;

	if (encryptedLength % MUBlowfish::BytesLength != 0)
	{
		return false;
	}

	mu_uint8 *dataBuffer = new mu_uint8[fileLength];
	SDL_RWread(file, dataBuffer, fileLength, 1);
	SDL_RWclose(file);

	FileHeaders::FileHeader *header = (FileHeaders::FileHeader*)dataBuffer;

	if (header->Signature != FILE_HEADERS::DATA_HEADER)
	{
		delete[] dataBuffer;
		return false;
	}

	if (GenerateCRC32(dataBuffer + HeaderSize, encryptedLength) != header->CRC)
	{
		delete[] dataBuffer;
		return false;
	}

	g_DataCrypt.Decode(dataBuffer + HeaderSize, dataBuffer + HeaderSize, encryptedLength);

	FileHeaders::CompressedHeader *compressedHeader = (FileHeaders::CompressedHeader*)(dataBuffer + HeaderSize);

	mu_uint8 *decompressedBuffer = new mu_uint8[compressedHeader->DecompressedSize];

	mu_boolean decompressed = FileHeaders::Decompress(dataBuffer + HeaderSize + CompressedHeaderSize,
		decompressedBuffer,
		compressedHeader->CompressedSize,
		compressedHeader->DecompressedSize);
	delete[] dataBuffer;

	if (decompressed == false)
	{
		delete[] decompressedBuffer;
		return false;
	}

	mu_uint8 *readBuffer = decompressedBuffer;

	mu_char textBuffer[4096];
	mu_uint32 textCount = 0, textLength = 0;
	FileHeaders::ReadBuffer(readBuffer, textCount);

	for (mu_uint32 i = 0; i < textCount; ++i)
	{
		FileHeaders::ReadBuffer(readBuffer, textLength);

		if (textLength > 0)
		{
			FileHeaders::ReadBuffer(readBuffer, textBuffer, textLength);
			textBuffer[textLength] = _T('\0');
#ifdef UNICODE
			mu_string textData = boost::locale::conv::utf_to_utf<mu_text>(textBuffer);
			AbuseNameFilter.push_back(textData);
#else
			AbuseNameFilter.push_back(textBuffer);
#endif
		}
	}

	delete[] decompressedBuffer;
#endif

	return true;
}