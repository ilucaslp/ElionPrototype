#include "stdafx.h"
#include "mu_jewelharmonyinfo.h"

JewelHarmonyInfo g_pUIJewelHarmonyinfo;

JewelHarmonyInfo::JewelHarmonyInfo()
{
}

JewelHarmonyInfo::~JewelHarmonyInfo()
{

}

const mu_boolean JewelHarmonyInfo::OpenJewelHarmonyInfoFile(const mu_text *filename)
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

	for (mu_uint32 i = 0; i < MAXHARMONYJEWELOPTIONTYPE; ++i)
	{
		for (mu_uint32 j = 0; j < MAXHARMONYJEWELOPTIONINDEX; ++j)
		{
			HarmonyJewelOption &data = m_OptionData[i][j];

			FileHeaders::ReadBuffer(readBuffer, data.OptionType);
			FileHeaders::ReadBuffer(readBuffer, data.Minlevel);
			FileHeaders::ReadBuffer(readBuffer, data.HarmonyJewelLevel, sizeof(data.HarmonyJewelLevel));
			FileHeaders::ReadBuffer(readBuffer, data.Zen, sizeof(data.Zen));

			mu_uint32 textLength = 0;
			FileHeaders::ReadBuffer(readBuffer, textLength);
			textData.resize(textLength);
			FileHeaders::ReadBuffer(readBuffer, (mu_char*)textData.data(), textLength);
#ifdef UNICODE
			data.Name = boost::locale::conv::utf_to_utf<mu_text>(textData);
#else
			data.Name = textData;
#endif
		}
	}

	delete[] decompressedBuffer;

	return true;
}

const StrengthenItem JewelHarmonyInfo::GetItemType(mu_int32 type)
{
	StrengthenItem itemtype = SI_None;

	if (ITEM_SWORD <= type && ITEM_STAFF > type)
	{
		itemtype = SI_Weapon;
		return itemtype;
	}
	else if (ITEM_STAFF <= type && ITEM_SHIELD > type)
	{
		itemtype = SI_Staff;
		return itemtype;
	}
	else if (ITEM_SHIELD <= type && ITEM_WING > type)
	{
		itemtype = SI_Defense;
		return itemtype;
	}

	return itemtype;
}

const mu_boolean JewelHarmonyInfo::IsHarmonyJewelOption(mu_int32 type, mu_int32 option)
{
	mu_boolean isSuccess = false;

	switch (type)
	{
	case SI_Weapon:
		{
			if (0 <= option && (MAXHARMONYJEWELOPTIONINDEX + 1) > option)
			{
				isSuccess = true;
			}
		}
		break;
	case SI_Staff:
		{
			if (0 <= option && (MAXHARMONYJEWELOPTIONINDEX + 1) - 2 > option)
			{
				isSuccess = true;
			}
		}
		break;
	case SI_Defense:
		{
			if (0 <= option && (MAXHARMONYJEWELOPTIONINDEX + 1) - 2 > option)
			{
				isSuccess = true;
			}
		}
		break;
	}

	return isSuccess;
}

void JewelHarmonyInfo::GetStrengthenCapability(StrengthenCapability* pitemSC, const ITEM* pitem, const mu_int32 index)
{
	if (pitem->HarmonyOption != 0)
	{
		StrengthenItem type = GetItemType(static_cast<mu_int32>(pitem->Type));

		if (type < SI_None)
		{
			if (IsHarmonyJewelOption(type, pitem->HarmonyOption))
			{
				HARMONYJEWELOPTION harmonyjewel = GetHarmonyJewelOptionInfo(type, pitem->HarmonyOption);

				if (index == 0)
				{
					if (type == SI_Weapon)
					{
						pitemSC->SI_isNB = true;

						if (pitem->HarmonyOption == 3)
						{
							pitemSC->SI_NB.SI_force = harmonyjewel.HarmonyJewelLevel[pitem->HarmonyOptionLevel];
						}
						else if (pitem->HarmonyOption == 4)
						{
							pitemSC->SI_NB.SI_activity = harmonyjewel.HarmonyJewelLevel[pitem->HarmonyOptionLevel];
						}
					}
					else if (type == SI_Staff)
					{
						pitemSC->SI_isNB = true;

						if (pitem->HarmonyOption == 2)
						{
							pitemSC->SI_NB.SI_force = harmonyjewel.HarmonyJewelLevel[pitem->HarmonyOptionLevel];
						}
						else if (pitem->HarmonyOption == 3)
						{
							pitemSC->SI_NB.SI_activity = harmonyjewel.HarmonyJewelLevel[pitem->HarmonyOptionLevel];
						}
					}
				}
				else if (index == 1)
				{
					if (type == SI_Weapon)
					{
						pitemSC->SI_isSP = true;

						if (pitem->HarmonyOption == 1)
						{
							pitemSC->SI_SP.SI_minattackpower = harmonyjewel.HarmonyJewelLevel[pitem->HarmonyOptionLevel];
						}
						else if (pitem->HarmonyOption == 2)
						{
							pitemSC->SI_SP.SI_maxattackpower = harmonyjewel.HarmonyJewelLevel[pitem->HarmonyOptionLevel];
						}
						else if (pitem->HarmonyOption == 5)
						{
							pitemSC->SI_SP.SI_minattackpower = harmonyjewel.HarmonyJewelLevel[pitem->HarmonyOptionLevel];
							pitemSC->SI_SP.SI_maxattackpower = harmonyjewel.HarmonyJewelLevel[pitem->HarmonyOptionLevel];
						}
						else if (pitem->HarmonyOption == 7)
						{
							pitemSC->SI_SP.SI_skillattackpower = harmonyjewel.HarmonyJewelLevel[pitem->HarmonyOptionLevel];
						}
						else if (pitem->HarmonyOption == 8)
						{
							pitemSC->SI_SP.SI_attackpowerRate = harmonyjewel.HarmonyJewelLevel[pitem->HarmonyOptionLevel];
						}
					}
					else if (type == SI_Staff)
					{
						pitemSC->SI_isSP = true;

						if (pitem->HarmonyOption == 1)
						{
							pitemSC->SI_SP.SI_magicalpower = harmonyjewel.HarmonyJewelLevel[pitem->HarmonyOptionLevel];
						}
						else if (pitem->HarmonyOption == 4)
						{
							pitemSC->SI_SP.SI_skillattackpower = harmonyjewel.HarmonyJewelLevel[pitem->HarmonyOptionLevel];
						}
						else if (pitem->HarmonyOption == 7)
						{
							pitemSC->SI_SP.SI_attackpowerRate = harmonyjewel.HarmonyJewelLevel[pitem->HarmonyOptionLevel];
						}
					}
				}
				else if (index == 2)
				{
					if (SI_Defense == type)
					{
						pitemSC->SI_isSD = true;

						if (pitem->HarmonyOption == 1)
						{
							pitemSC->SI_SD.SI_defense = harmonyjewel.HarmonyJewelLevel[pitem->HarmonyOptionLevel];
						}
						else if (pitem->HarmonyOption == 2)
						{
							pitemSC->SI_SD.SI_AG = harmonyjewel.HarmonyJewelLevel[pitem->HarmonyOptionLevel];
						}
						else if (pitem->HarmonyOption == 3)
						{
							pitemSC->SI_SD.SI_HP = harmonyjewel.HarmonyJewelLevel[pitem->HarmonyOptionLevel];
						}
						else if (pitem->HarmonyOption == 6)
						{
							pitemSC->SI_SD.SI_defenseRate = harmonyjewel.HarmonyJewelLevel[pitem->HarmonyOptionLevel];
						}
					}
				}
			}
		}
	}
}