#include "stdafx.h"
#include "mu_itemcategoriesdb.h"

MUItemCategoriesDatabase g_ItemCategoriesDatabase;

MUItemCategoriesDatabase::MUItemCategoriesDatabase()
{
}

mu_boolean MUItemCategoriesDatabase::Load(const mu_text *filename)
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

	// Reading
	{
		mu_uint8 *readBuffer = decompressedBuffer;
		mu_uint32 categoriesCount = 0;
		FileHeaders::ReadBuffer(readBuffer, categoriesCount);
		_Categories.resize(categoriesCount);
		for (auto it = _Categories.begin(); it != _Categories.end(); ++it)
		{
			MUItemCategoriesData &typeData = *it;
			FileHeaders::ReadBuffer(readBuffer, typeData.CategoryName);
			mu_uint32 itemsCount = 0;
			FileHeaders::ReadBuffer(readBuffer, itemsCount);
			typeData.Items.resize(itemsCount);
			FileHeaders::ReadBuffer(readBuffer, typeData.Items.data(), sizeof(MUItemCategoryItemData) * itemsCount);
		}
	}

	delete[] decompressedBuffer;

	return true;
}