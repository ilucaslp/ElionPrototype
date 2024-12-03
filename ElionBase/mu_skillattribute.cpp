#include "stdafx.h"
#include "mu_skillattribute.h"

SKILL_ATTRIBUTE SkillAttribute[MAX_SKILLS];

mu_boolean OpenSkillScript(const mu_text *filename)
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
	mu_asciistr textData;

	for (mu_uint32 n = 0; n < MAX_SKILLS; ++n)
	{
		SKILL_ATTRIBUTE &data = SkillAttribute[n];
		mu_uint32 textLength = 0;
		FileHeaders::ReadBuffer(readBuffer, textLength);
		textData.resize(textLength);
		FileHeaders::ReadBuffer(readBuffer, (mu_char*)textData.data(), textLength);
#ifdef UNICODE
		mu_strcpy(data.Name, mu_countof(data.Name), boost::locale::conv::utf_to_utf<mu_text>(textData).c_str());
#else
		mu_strcpy(data.Name, mu_countof(data.Name), textData.c_str());
#endif
		FileHeaders::ReadBuffer(readBuffer, data.Level);
		FileHeaders::ReadBuffer(readBuffer, data.Damage);
		FileHeaders::ReadBuffer(readBuffer, data.Mana);
		FileHeaders::ReadBuffer(readBuffer, data.AbilityGuage);
		FileHeaders::ReadBuffer(readBuffer, data.Distance);
		FileHeaders::ReadBuffer(readBuffer, data.Delay);
		FileHeaders::ReadBuffer(readBuffer, data.Strength);
		FileHeaders::ReadBuffer(readBuffer, data.Dexterity);
		FileHeaders::ReadBuffer(readBuffer, data.Energy);
		FileHeaders::ReadBuffer(readBuffer, data.Charisma);
		FileHeaders::ReadBuffer(readBuffer, data.MasteryType);
		FileHeaders::ReadBuffer(readBuffer, data.SkillUseType);
		FileHeaders::ReadBuffer(readBuffer, data.SkillBrand);
		FileHeaders::ReadBuffer(readBuffer, data.KillCount);
		FileHeaders::ReadBuffer(readBuffer, data.RequireDutyClass, sizeof(data.RequireDutyClass));
		FileHeaders::ReadBuffer(readBuffer, data.RequireClass, sizeof(data.RequireClass));
		FileHeaders::ReadBuffer(readBuffer, data.Magic_Icon);
		FileHeaders::ReadBuffer(readBuffer, data.TypeSkill);
	}

	delete[] decompressedBuffer;

	return true;
}