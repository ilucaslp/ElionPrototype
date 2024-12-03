#include "stdafx.h"
#include "mu_movemanager.h"

MUMoveManager g_MoveManager;

MUMoveManager::MUMoveManager()
{

}

MUMoveManager::~MUMoveManager()
{

}

mu_boolean MUMoveManager::Open(const mu_text *filename)
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

	mu_uint32 moveCount = 0;
	FileHeaders::ReadBuffer(readBuffer, moveCount);
	MoveData.resize(moveCount);

	mu_asciistr textData;

	for (mu_uint32 n = 0; n < moveCount; ++n)
	{
		MoveRequirementDataEx &data = MoveData[n];
		FileHeaders::ReadBuffer(readBuffer, data.Index);
		FileHeaders::ReadBuffer(readBuffer, data.MinimumLevel);
		FileHeaders::ReadBuffer(readBuffer, data.MaximumLevel);
		FileHeaders::ReadBuffer(readBuffer, data.MoveCost);
		FileHeaders::ReadBuffer(readBuffer, data.GateNumber);

		mu_uint32 textLength = 0;
		FileHeaders::ReadBuffer(readBuffer, textLength);
		textData.resize(textLength);
		FileHeaders::ReadBuffer(readBuffer, (mu_char*)textData.data(), textLength);
#ifdef UNICODE
		mu_strcpy(data.MainMapName, mu_countof(data.MainMapName), boost::locale::conv::utf_to_utf<mu_text>(textData).c_str());
#else
		mu_strcpy(data.MainMapName, mu_countof(data.MainMapName), textData.c_str());
#endif

		textLength = 0;
		FileHeaders::ReadBuffer(readBuffer, textLength);
		textData.resize(textLength);
		FileHeaders::ReadBuffer(readBuffer, (mu_char*)textData.data(), textLength);
#ifdef UNICODE
		mu_strcpy(data.SubMapName, mu_countof(data.SubMapName), boost::locale::conv::utf_to_utf<mu_text>(textData).c_str());
#else
		mu_strcpy(data.SubMapName, mu_countof(data.SubMapName), textData.c_str());
#endif
	}

	delete[] decompressedBuffer;

	return true;
}

void MUMoveManager::Update()
{
	mu_int32 PlayerLevel = CharacterAttribute->Level;
	mu_int64 CurrentGold = CharacterMachine->Gold;

	mu_boolean isSpecialLevel = false;
	if (GetBaseClass(CharacterAttribute->Class) == GAME::CLASS_DARK ||
		GetBaseClass(CharacterAttribute->Class) == GAME::CLASS_DARK_LORD)
	{
		isSpecialLevel = true;
	}

	for (auto it = MoveData.begin(); it != MoveData.end(); ++it)
	{
		MoveRequirementDataEx &data = *it;

		data.CanMove = false;

		mu_int32 RequiredLevel = data.MinimumLevel;

		if (isSpecialLevel == true &&
			data.MinimumLevel != MAX_LEVEL)
		{
			mu_boolean isKalima = false;
			mu_uint32 kalimaLevel = 0;

			const mu_string &kalimaName = GlobalText.Get(58);
			if (mu_strnicmp(data.MainMapName, kalimaName.c_str(), kalimaName.size()) == 0)
			{
				if (kalimaName.empty() == false &&
					kalimaName.size() < mu_countof(data.MainMapName) &&
					mu_isdigit(data.MainMapName[kalimaName.size()]))
				{
					isKalima = true;
					kalimaLevel = data.MainMapName[kalimaName.size()] - _T('0');
				}
			}

			if (isKalima == true)
			{
				if (kalimaLevel <= 6)
				{
					RequiredLevel = RequiredLevel - 20;
				}
			}
			else
			{
				RequiredLevel = ((RequiredLevel / 3) * 2);
			}
		}

		if(data.IsStrife == false ||
			Hero->m_byGensInfluence != 0)
		{
			if (PlayerLevel >= RequiredLevel &&
				CurrentGold >= data.MoveCost &&
				Hero->PK < PVP_MURDERER1)
			{
				ITEM* pEquipedRightRing = &CharacterMachine->Equipment[GAME::EQUIPMENT_RING_RIGHT];
				ITEM* pEquipedLeftRing = &CharacterMachine->Equipment[GAME::EQUIPMENT_RING_LEFT];
				ITEM* pEquipedHelper = &CharacterMachine->Equipment[GAME::EQUIPMENT_HELPER];
				ITEM* pEquipedWing = &CharacterMachine->Equipment[GAME::EQUIPMENT_WING];

				if (mu_stricmp(data.MainMapName, GlobalText[55]) == 0)
				{
					if ((pEquipedHelper->Type == ITEM_HELPER + 37
							|| pEquipedHelper->Type == ITEM_HELPER + 3
							|| pEquipedHelper->Type == ITEM_HELPER + 4
							|| pEquipedWing->Type == ITEM_HELPER + 30
							|| (pEquipedWing->Type >= ITEM_WING + 36 && pEquipedWing->Type <= ITEM_WING + 43)
							|| (pEquipedWing->Type >= ITEM_WING && pEquipedWing->Type <= ITEM_WING + 6)
							|| (ITEM_WING + 130 <= pEquipedWing->Type && pEquipedWing->Type <= ITEM_WING + 134))
						&& pEquipedHelper->Type != ITEM_HELPER + 2
						&& g_ChangeRingMgr.CheckBanMoveIcarusMap(pEquipedRightRing->Type, pEquipedLeftRing->Type) == false)
					{
						data.CanMove = true;
					}
				}
				else if (mu_stricmp(data.MainMapName, GlobalText[37]) == 0)
				{
					if (pEquipedHelper->Type != ITEM_HELPER + 2 &&
						pEquipedHelper->Type != ITEM_HELPER + 3)
					{
						data.CanMove = true;
					}
				}
				else if (mu_stricmp(data.MainMapName, GlobalText[669]) == 0)
				{
					if (IsCastleOwner == true)
					{
						data.CanMove = true;
					}
				}
				else if ((g_pServerList->GetSelectedServerFlag() & CS2CL::SERVER_FLAG::SF_NONPVP) != 0 &&
					mu_stricmp(data.MainMapName, GlobalText[2686]) == 0)
				{
					data.CanMove = false;
				}
				else
				{
					data.CanMove = true;
				}
			}
		}
	}
}