#include "stdafx.h"
#include "mu_mixmanager.h"

MUMixManager g_MixManager;

MUMixManager::MUMixManager() : _MixType(MIXTYPE_GOBLIN_NORMAL)
{

}

MUMixManager::~MUMixManager()
{

}

mu_boolean MUMixManager::Load(const mu_text *filename)
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

	if (compressedHeader->DecompressedSize < sizeof(mu_uint32) * MAX_MIX_TYPES)
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

	mu_uint8 *readBuffer = decompressedBuffer;

	for (mu_uint32 n = 0; n < MAX_MIX_TYPES; ++n)
	{
		mu_uint32 recipesCount = 0;
		FileHeaders::ReadBuffer(readBuffer, recipesCount);

		if (recipesCount > 0)
		{
			_Recipes[n].resize(recipesCount);
			FileHeaders::ReadBuffer(readBuffer, _Recipes[n].data(), sizeof(MUMixRecipeData) * recipesCount);
		}
	}

	delete[] decompressedBuffer;

	return true;
}

void MUMixManager::ResetMixItemInventory()
{
	_MixItemInventory.Reset();
}

void MUMixManager::AddItemToMixItemInventory(ITEM * pItem)
{
	_MixItemInventory.AddItem(pItem);
}

void MUMixManager::CheckMixInventory()
{
	if (_MixItemInventory.GetNumMixItems() == 0) _IsMixInit = true;
	else _IsMixInit = false;

	CheckRecipe(_MixItemInventory.GetNumMixItems(), _MixItemInventory.GetMixItems());
	CheckRecipeSimilarity(_MixItemInventory.GetNumMixItems(), _MixItemInventory.GetMixItems());
}

mu_int32 MUMixManager::GetSeedSphereID(mu_int32 iOrder)
{
	mu_int32 iCurrOrder = 0;
	MUMixItem * pItems = _MixItemInventory.GetMixItems();
	for (int i = 0; i < _MixItemInventory.GetNumMixItems(); ++i)
	{
		if (pItems[i].m_bySeedSphereID != SOCKET_EMPTY)
		{
			if (iCurrOrder == iOrder)
			{
				return pItems[i].m_bySeedSphereID;
			}
			else
			{
				++iCurrOrder;
			}
		}
	}
	return SOCKET_EMPTY;
}