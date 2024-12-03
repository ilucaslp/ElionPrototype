#include "stdafx.h"
#include "mu_globaltext.h"

MUGlobalText GlobalText;

MUGlobalText::MUGlobalText()
{

}

MUGlobalText::~MUGlobalText()
{

}

mu_boolean MUGlobalText::Load(const mu_text *filename)
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

	mu_uint32 textCount = 0;
	FileHeaders::ReadBuffer(readBuffer, textCount);

	mu_asciistr textData;

	for (mu_int32 i = 0; i < textCount; ++i)
	{
		mu_uint32 Key;
		mu_uint32 Length;

		FileHeaders::ReadBuffer(readBuffer, Key);
		FileHeaders::ReadBuffer(readBuffer, Length);

		if (Length == 0)
		{
			_TextMap.insert(std::make_pair(Key, _T("")));
		}
		else
		{
			textData.resize(Length);
			FileHeaders::ReadBuffer(readBuffer, (mu_char*)textData.data(), Length);
#ifdef UNICODE
			_TextMap.insert(std::make_pair(Key, boost::locale::conv::utf_to_utf<mu_text>(textData)));
#else
			_TextMap.insert(std::make_pair(Key, textData));
#endif
		}
	}

	delete[] decompressedBuffer;

	return true;
}