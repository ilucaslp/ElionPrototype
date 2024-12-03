#include "stdafx.h"
#include "mu_itemaddoption.h"

ItemAddOptioninfo g_pItemAddOptioninfo;

ItemAddOptioninfo::ItemAddOptioninfo()
{
}

ItemAddOptioninfo::~ItemAddOptioninfo()
{
}

const mu_boolean ItemAddOptioninfo::OpenItemAddOptionInfoFile(const mu_text *filename)
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

	if (compressedHeader->DecompressedSize != sizeof(m_ItemAddOption))
	{
		delete[] dataBuffer;
		return false;
	}

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

	mu_memcpy(m_ItemAddOption, decompressedBuffer, sizeof(m_ItemAddOption));

	delete[] decompressedBuffer;

	return true;
}

void ItemAddOptioninfo::GetItemAddOtioninfoText(std::vector<mu_string>& outtextlist, mu_int32 type)
{
	mu_int32 optiontype = 0;
	mu_int32 optionvalue = 0;

	for (mu_int32 i = 0; i < 2; ++i)
	{
		mu_text TempText[256];

		if (i == 0)
		{
			optiontype = m_ItemAddOption[type].m_byOption1;
			optionvalue = m_ItemAddOption[type].m_byValue1;
		}
		else
		{
			optiontype = m_ItemAddOption[type].m_byOption2;
			optionvalue = m_ItemAddOption[type].m_byValue2;
		}

		switch (optiontype)
		{
		case 1: mu_sprintf_s(TempText, mu_countof(TempText), GlobalText[2184], optionvalue);
			break;
		case 2: mu_sprintf_s(TempText, mu_countof(TempText), GlobalText[2185], optionvalue);
			break;
		case 3: mu_sprintf_s(TempText, mu_countof(TempText), GlobalText[2186], optionvalue);
			break;
		case 4: mu_sprintf_s(TempText, mu_countof(TempText), GlobalText[2187], optionvalue);
			break;
		case 5: mu_sprintf_s(TempText, mu_countof(TempText), GlobalText[2188], optionvalue);
			break;
		case 6: mu_sprintf_s(TempText, mu_countof(TempText), GlobalText[2189], optionvalue);
			break;
		case 7: mu_sprintf_s(TempText, mu_countof(TempText), GlobalText[2190]);
			break;
		case 8: mu_sprintf_s(TempText, mu_countof(TempText), GlobalText[2191], optionvalue);
			break;
		}

		outtextlist.push_back(TempText);
	}
}
