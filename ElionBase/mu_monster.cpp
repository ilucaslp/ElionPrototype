#include "stdafx.h"
#include "mu_monster.h"

MONSTER_SCRIPT MonsterScript[MAX_MONSTER];
Script_Skill MonsterSkill[MAX_MONSTER];

mu_boolean OpenMonsterScript(const mu_text *filename)
{
	SDL_RWops *file = nullptr;

	if (mu_rwfromfile(&file, filename, "rb") == false)
	{
		return false;
	}

	MUReadScript readScript(file);

	while (true)
	{
		readScript.GetToken();

		if (readScript.CurrentToken == MUReadScript::END)
		{
			break;
		}

		if (readScript.CurrentToken == MUReadScript::NAME &&
			mu_strcmp(_T("end"), readScript.TokenString) == 0)
		{
			break;
		}

		mu_uint16 Type = readScript.TokenNumber;
		MONSTER_SCRIPT *m = &MonsterScript[Type];

		readScript.GetToken();
		readScript.GetToken();

		mu_strcpy_s(m->Name, mu_countof(m->Name), readScript.TokenString);

		m->Used = true;
	}

	return true;
}

mu_boolean OpenMonsterSkillScript(const mu_text *filename)
{
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
		SDL_RWclose(file);
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

	mu_uint32 monsterCount = 0;
	FileHeaders::ReadBuffer(readBuffer, monsterCount);

	for (mu_int32 i = 0; i < monsterCount; ++i)
	{
		mu_int32 Type;
		FileHeaders::ReadBuffer(readBuffer, Type);
		FileHeaders::ReadBuffer(readBuffer, MonsterSkill[Type]);
	}

	delete[] decompressedBuffer;

	return true;
}

const mu_text *GetMonsterName(mu_int32 MonsterType)
{
	if(MonsterType < 0 || MonsterType >= MAX_MONSTER)
		return _T("");

	return MonsterScript[MonsterType].Name;
}