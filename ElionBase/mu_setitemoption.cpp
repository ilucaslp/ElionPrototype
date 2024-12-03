#include "stdafx.h"
#include "mu_setitemoption.h"

CSItemOption g_csItemOption;

mu_boolean CSItemOption::OpenItemSetScript()
{
	if (OpenItemSetType(_T("./data/local/itemsettype.datx")) == false)
	{
		return false;
	}

	if (OpenItemSetOption(_T("./data/local/itemsetoption.datx")) == false)
	{
		return false;
	}

	return true;
}

mu_boolean CSItemOption::OpenItemSetType(const mu_text *filename)
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

	if (compressedHeader->DecompressedSize != sizeof(m_ItemSetType))
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

	mu_memcpy(m_ItemSetType, decompressedBuffer, sizeof(m_ItemSetType));

	delete[] decompressedBuffer;

	return true;
}

mu_boolean CSItemOption::OpenItemSetOption(const mu_text *filename)
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

	for (mu_uint32 n = 0; n < MAX_SET_OPTION; ++n)
	{
		ITEM_SET_OPTION &data = m_ItemSetOption[n];
		mu_uint32 textLength = 0;
		FileHeaders::ReadBuffer(readBuffer, textLength);
		textData.resize(textLength);
		FileHeaders::ReadBuffer(readBuffer, (mu_char*)textData.data(), textLength);
#ifdef UNICODE
		mu_strcpy(data.strSetName, mu_countof(data.strSetName), boost::locale::conv::utf_to_utf<mu_text>(textData).c_str());
#else
		mu_strcpy(data.strSetName, mu_countof(data.strSetName), textData.c_str());
#endif
		FileHeaders::ReadBuffer(readBuffer, data.byStandardOption, sizeof(data.byStandardOption));
		FileHeaders::ReadBuffer(readBuffer, data.byStandardOptionValue, sizeof(data.byStandardOptionValue));
		FileHeaders::ReadBuffer(readBuffer, data.byExtOption, sizeof(data.byExtOption));
		FileHeaders::ReadBuffer(readBuffer, data.byExtOptionValue, sizeof(data.byExtOptionValue));
		FileHeaders::ReadBuffer(readBuffer, data.byOptionCount);
		FileHeaders::ReadBuffer(readBuffer, data.byFullOption, sizeof(data.byFullOption));
		FileHeaders::ReadBuffer(readBuffer, data.byFullOptionValue, sizeof(data.byFullOptionValue));
		FileHeaders::ReadBuffer(readBuffer, data.byRequireClass, sizeof(data.byRequireClass));
	}

	delete[] decompressedBuffer;

	return true;
}

mu_boolean CSItemOption::IsDisableSkill(mu_int32 Type, mu_int32 Energy, mu_int32 Charisma)
{
	mu_int32 SkillEnergy = 20 + SkillAttribute[Type].Energy*(SkillAttribute[Type].Level) * 4 / 100;

	if (Type == AT_SKILL_SUMMON_EXPLOSION || Type == AT_SKILL_SUMMON_REQUIEM)
	{
		SkillEnergy = 20 + SkillAttribute[Type].Energy*(SkillAttribute[Type].Level) * 3 / 100;
	}

	if (GetBaseClass(Hero->Class) == GAME::CLASS_KNIGHT)
	{
		SkillEnergy = 10 + SkillAttribute[Type].Energy*(SkillAttribute[Type].Level) * 4 / 100;
	}

	switch (Type)
	{
	case 17:SkillEnergy = 0; break;
	case 30:SkillEnergy = 30; break;
	case 31:SkillEnergy = 60; break;
	case 32:SkillEnergy = 90; break;
	case 33:SkillEnergy = 130; break;
	case 34:SkillEnergy = 170; break;
	case 35:SkillEnergy = 210; break;
	case 36:SkillEnergy = 300; break; 
	case 60:SkillEnergy = 15; break;
	case AT_SKILL_ASHAKE_UP:
	case AT_SKILL_ASHAKE_UP + 1:
	case AT_SKILL_ASHAKE_UP + 2:
	case AT_SKILL_ASHAKE_UP + 3:
	case AT_SKILL_ASHAKE_UP + 4:
	case AT_SKILL_DARK_HORSE:    SkillEnergy = 0; break;
	case AT_PET_COMMAND_DEFAULT: SkillEnergy = 0; break;
	case AT_PET_COMMAND_RANDOM:  SkillEnergy = 0; break;
	case AT_PET_COMMAND_OWNER:   SkillEnergy = 0; break;
	case AT_PET_COMMAND_TARGET:  SkillEnergy = 0; break;
	case AT_SKILL_PLASMA_STORM_FENRIR: SkillEnergy = 0; break;
	case AT_SKILL_INFINITY_ARROW: SkillEnergy = 0;
		break;

	case AT_SKILL_BLOW_OF_DESTRUCTION:
	case AT_SKILL_RECOVER:
	case AT_SKILL_GAOTIC:
	case AT_SKILL_MULTI_SHOT:
	case AT_SKILL_FIRE_SCREAM_UP:
	case AT_SKILL_FIRE_SCREAM_UP + 1:
	case AT_SKILL_FIRE_SCREAM_UP + 2:
	case AT_SKILL_FIRE_SCREAM_UP + 3:
	case AT_SKILL_FIRE_SCREAM_UP + 4:
	case AT_SKILL_DARK_SCREAM:
		SkillEnergy = 0;
		break;

	case AT_SKILL_EXPLODE:
		SkillEnergy = 0;
		break;
	}

	if (Type >= AT_SKILL_STUN && Type <= AT_SKILL_REMOVAL_BUFF)
	{
		SkillEnergy = 0;
	}
	else if ((Type >= 18 && Type <= 23) || (Type >= 41 && Type <= 43) || (Type >= 47 && Type <= 49) || Type == 24 || Type == 51 || Type == 52 || Type == 55 || Type == 56)
	{
		SkillEnergy = 0;
	}
	else if (Type == 44 || Type == 45 || Type == 46 || Type == 57 || Type == 73 || Type == 74)
	{
		SkillEnergy = 0;
	}

	if (Charisma > 0)
	{
		mu_int32 SkillCharisma = SkillAttribute[Type].Charisma;
		if (Charisma < SkillCharisma)
		{
			return true;
		}
	}

	if (Energy < SkillEnergy)
	{
		return true;
	}

	return false;
}

mu_uint8 CSItemOption::IsChangeSetItem(const mu_int32 Type, const mu_int32 SubType = -1)
{
	ITEM_SET_TYPE& itemSType = m_ItemSetType[Type];

	if (SubType == -1)
	{
		if (itemSType.byOption[0] == 255 && itemSType.byOption[1] == 255)
			return 0;

		return 255;
	}
	else
	{
		if (itemSType.byOption[SubType] == 255)
			return 0;

		return SubType + 1;
	}
}

mu_uint16 CSItemOption::GetMixItemLevel(const mu_int32 Type)
{
	if (Type < 0) return 0;

	mu_uint16 MixLevel = 0;
	ITEM_SET_TYPE& itemSType = m_ItemSetType[Type];

	MixLevel = MakeWord(itemSType.byMixItemLevel[0], itemSType.byMixItemLevel[1]);

	return MixLevel;
}

mu_boolean CSItemOption::GetSetItemName(mu_text* strName, const mu_int32 iType, const mu_int32 setType)
{
	mu_int32 setItemType = (setType % 0x04);

	if (setItemType > 0)
	{
		ITEM_SET_TYPE& itemSType = m_ItemSetType[iType];
		if (itemSType.byOption[setItemType - 1] != 255 && itemSType.byOption[setItemType - 1] != 0)
		{
			ITEM_SET_OPTION& itemOption = m_ItemSetOption[itemSType.byOption[setItemType - 1]];

			memcpy(strName, itemOption.strSetName, sizeof(itemOption.strSetName));

			mu_int32 length = mu_strlen(strName);
			strName[length] = ' ';
			strName[length + 1] = 0;
			return true;
		}
	}

	return false;
}

void CSItemOption::checkItemType(mu_uint8* optionList, const mu_int32 iType, const mu_int32 setType)
{
	mu_int32 setItemType = (setType % 0x04);

	if (setItemType > 0)
	{
		ITEM_SET_TYPE& itemSType = m_ItemSetType[iType];

		if (itemSType.byOption[setItemType - 1] != 255 && itemSType.byOption[setItemType - 1] != 0)
		{
			for (mu_int32 i = 0; i < 30; i += 3)
			{
				if (optionList[i] == 0)
				{
					optionList[i] = itemSType.byOption[setItemType - 1];
					++optionList[i + 1];
					optionList[i + 2] = setItemType - 1;
					break;
				}
				else if (optionList[i] == itemSType.byOption[setItemType - 1])
				{
					++optionList[i + 1];
					optionList[i + 2] = setItemType - 1;
					break;
				}
			}
		}
	}
}

void CSItemOption::calcSetOptionList(mu_uint8* optionList)
{
	mu_int32 Class = GetBaseClass(Hero->Class);
	mu_int32 ExClass = IsSecondClass(Hero->Class);

	mu_uint8 bySetOptionListTmp[2][16];
	mu_uint8 bySetOptionListTmp2[2][16];
	mu_int32 iSetOptionListValue[2][16];

	mu_uint32 arruiSetItemTypeSequence[2][16];
	mu_zeromem(arruiSetItemTypeSequence, sizeof(arruiSetItemTypeSequence));

	mu_uint8 optionCount[2] = { 0, 0 };
	mu_uint8 setType = 0;

	m_bySameSetItem = 0;
	mu_zeromem(m_strSetName, sizeof(m_strSetName));

	m_bySetOptionIndex[0] = 0;
	m_bySetOptionIndex[1] = 0;
	m_bySetOptionANum = 0;
	m_bySetOptionBNum = 0;
	Hero->ExtendState = 0;

	mu_uint32 iSetItemTypeSequence = 0, iCurrentSetItemTypeSequence = 0;

	m_mapEquippedSetItemName.clear();
	m_mapEquippedSetItemSequence.clear();

	for (mu_int32 i = 0; i < 30; i += 3)
	{
		if (optionList[i + 1] >= 2)
		{
			mu_int32 count = optionList[i + 1] - 1;
			mu_int32 standardCount = EMath::Min(count, 6);
			ITEM_SET_OPTION& itemOption = m_ItemSetOption[optionList[i]];

			mu_uint8 RequireClass = 0;
			if (itemOption.byRequireClass[0] == 1 && Class == GAME::CLASS_WIZARD) RequireClass = 1;
			if (itemOption.byRequireClass[0] == 2 && Class == GAME::CLASS_WIZARD && ExClass) RequireClass = 1;
			if (itemOption.byRequireClass[1] == 1 && Class == GAME::CLASS_KNIGHT) RequireClass = 1;
			if (itemOption.byRequireClass[1] == 2 && Class == GAME::CLASS_KNIGHT && ExClass) RequireClass = 1;
			if (itemOption.byRequireClass[2] == 1 && Class == GAME::CLASS_ELF) RequireClass = 1;
			if (itemOption.byRequireClass[2] == 2 && Class == GAME::CLASS_ELF && ExClass) RequireClass = 1;
			if (itemOption.byRequireClass[3] == 1 && Class == GAME::CLASS_DARK) RequireClass = 1;
			if (itemOption.byRequireClass[3] == 1 && Class == GAME::CLASS_DARK && ExClass) RequireClass = 1;
			if (itemOption.byRequireClass[4] == 1 && Class == GAME::CLASS_DARK_LORD) RequireClass = 1;
			if (itemOption.byRequireClass[4] == 1 && Class == GAME::CLASS_DARK_LORD && ExClass) RequireClass = 1;
			if (itemOption.byRequireClass[5] == 1 && Class == GAME::CLASS_SUMMONER) RequireClass = 1;
			if (itemOption.byRequireClass[5] == 1 && Class == GAME::CLASS_SUMMONER && ExClass) RequireClass = 1;
			setType = optionList[i + 2];
			m_bySetOptionIndex[setType] = optionList[i];
			if (m_strSetName[setType][0] != 0 && mu_strcmp(m_strSetName[setType], itemOption.strSetName) != 0)
			{
				if (m_strSetName[0][0] == 0)
					mu_strcpy(m_strSetName[0], mu_countof(m_strSetName[0]), itemOption.strSetName);
				else
					mu_strcpy(m_strSetName[1], mu_countof(m_strSetName[0]), itemOption.strSetName);
				m_bySameSetItem = count;
			}
			else
			{
				mu_strcpy(m_strSetName[setType], mu_countof(m_strSetName[0]), itemOption.strSetName);
			}

			mu_boolean bFind = false;
			for (m_iterESIN = m_mapEquippedSetItemName.begin(); m_iterESIN != m_mapEquippedSetItemName.end(); ++m_iterESIN)
			{
				mu_string &strCur = m_iterESIN->second;

				if (mu_strcmp(itemOption.strSetName, strCur.c_str()) == 0)
				{
					bFind = true;
					break;
				}
			}

			if (false == bFind)
			{
				iCurrentSetItemTypeSequence = iSetItemTypeSequence++;
				m_mapEquippedSetItemName.insert(std::make_pair(iCurrentSetItemTypeSequence, itemOption.strSetName));
			}

			mu_uint8 option[2];
			mu_int32 value[2];
			for (mu_int32 j = 0; j < count; ++j)
			{
				option[0] = 255; option[1] = 255;
				if (j < standardCount)
				{
					option[0] = itemOption.byStandardOption[j][0];
					value[0] = itemOption.byStandardOptionValue[j][0];
					option[1] = itemOption.byStandardOption[j][1];
					value[1] = itemOption.byStandardOptionValue[j][1];
				}
				else if (j < (count - standardCount))
				{
					option[0] = itemOption.byExtOption[j];
					value[0] = itemOption.byExtOptionValue[j];
				}

				if (option[0] != 255)
				{
					if (option[0] < MASTERY_OPTION)
					{
						arruiSetItemTypeSequence[setType][optionCount[setType]] = iCurrentSetItemTypeSequence;

						bySetOptionListTmp[setType][optionCount[setType]] = option[0];
						bySetOptionListTmp2[setType][optionCount[setType]] = RequireClass;
						iSetOptionListValue[setType][optionCount[setType]] = value[0];
						++optionCount[setType];

					}
					else if (itemOption.byRequireClass[Class] && ExClass >= itemOption.byRequireClass[Class] - 1)
					{
						arruiSetItemTypeSequence[setType][optionCount[setType]] = iCurrentSetItemTypeSequence;

						bySetOptionListTmp[setType][optionCount[setType]] = option[0];
						bySetOptionListTmp2[setType][optionCount[setType]] = RequireClass;
						iSetOptionListValue[setType][optionCount[setType]] = value[0];
						++optionCount[setType];
					}
				}

				if (option[1] != 255)
				{
					if (option[1] < MASTERY_OPTION)
					{
						arruiSetItemTypeSequence[setType][optionCount[setType]] = iCurrentSetItemTypeSequence;

						bySetOptionListTmp[setType][optionCount[setType]] = option[1];
						bySetOptionListTmp2[setType][optionCount[setType]] = RequireClass;
						iSetOptionListValue[setType][optionCount[setType]] = value[1];
						++optionCount[setType];
					}
					else if (itemOption.byRequireClass[Class] && ExClass >= itemOption.byRequireClass[Class] - 1)
					{
						arruiSetItemTypeSequence[setType][optionCount[setType]] = iCurrentSetItemTypeSequence;

						bySetOptionListTmp[setType][optionCount[setType]] = option[1];
						bySetOptionListTmp2[setType][optionCount[setType]] = RequireClass;
						iSetOptionListValue[setType][optionCount[setType]] = value[1];
						++optionCount[setType];
					}
				}
			}

			if (count >= itemOption.byOptionCount - 2)
			{
				for (mu_int32 j = 0; j < 5; ++j)
				{
					option[0] = itemOption.byFullOption[j];
					value[0] = itemOption.byFullOptionValue[j];
					if (option[0] != 255)
					{
						if (option[0] < MASTERY_OPTION)
						{
							arruiSetItemTypeSequence[setType][optionCount[setType]] = iCurrentSetItemTypeSequence;
							
							bySetOptionListTmp[setType][optionCount[setType]] = option[0];
							bySetOptionListTmp2[setType][optionCount[setType]] = RequireClass;
							iSetOptionListValue[setType][optionCount[setType]] = value[0];
							++optionCount[setType];

							if (m_bySameSetItem != 0) ++m_bySameSetItem;
						}
						else if (itemOption.byRequireClass[Class] && ExClass >= itemOption.byRequireClass[Class] - 1)
						{
							arruiSetItemTypeSequence[setType][optionCount[setType]] = iCurrentSetItemTypeSequence;

							bySetOptionListTmp[setType][optionCount[setType]] = option[0];
							bySetOptionListTmp2[setType][optionCount[setType]] = RequireClass;
							iSetOptionListValue[setType][optionCount[setType]] = value[0];
							++optionCount[setType];

							if (m_bySameSetItem != 0) ++m_bySameSetItem;
						}
					}
				}

				Hero->ExtendState = 1;
			}
		}
	}

	for (mu_int32 i = 0; i < 2; ++i)
	{
		for (mu_int32 j = 0; j < optionCount[i]; ++j)
		{
			m_mapEquippedSetItemSequence.insert(std::make_pair((i*optionCount[0]) + j, arruiSetItemTypeSequence[i][j]));
			m_bySetOptionList[(i*optionCount[0]) + j][0] = bySetOptionListTmp[i][j];
			m_bySetOptionList[(i*optionCount[0]) + j][1] = bySetOptionListTmp2[i][j];
			m_iSetOptionListValue[(i*optionCount[0]) + j][0] = iSetOptionListValue[i][j];
			m_iSetOptionListValue[(i*optionCount[0]) + j][1] = iSetOptionListValue[i][j];
		}
	}

	m_bySetOptionANum = optionCount[0];
	m_bySetOptionBNum = optionCount[1];
}

void CSItemOption::getExplainText(mu_text* text, const mu_size textLength, const mu_uint8 option, const mu_uint8 value, const mu_uint8 SetIndex)
{
	switch (option + AT_SET_OPTION_IMPROVE_STRENGTH)
	{
	case AT_SET_OPTION_IMPROVE_MAGIC_POWER:
		mu_sprintf_s(text, textLength, GlobalText[632], value);
		break;

	case AT_SET_OPTION_IMPROVE_STRENGTH:
	case AT_SET_OPTION_IMPROVE_DEXTERITY:
	case AT_SET_OPTION_IMPROVE_ENERGY:
	case AT_SET_OPTION_IMPROVE_VITALITY:
	case AT_SET_OPTION_IMPROVE_CHARISMA:
	case AT_SET_OPTION_IMPROVE_ATTACK_MIN:
	case AT_SET_OPTION_IMPROVE_ATTACK_MAX:
		mu_sprintf_s(text, textLength, GlobalText[950 + option], value);
		break;

	case AT_SET_OPTION_IMPROVE_DAMAGE:
	case AT_SET_OPTION_IMPROVE_ATTACKING_PERCENT:
	case AT_SET_OPTION_IMPROVE_DEFENCE:
	case AT_SET_OPTION_IMPROVE_MAX_LIFE:
	case AT_SET_OPTION_IMPROVE_MAX_MANA:
	case AT_SET_OPTION_IMPROVE_MAX_AG:
	case AT_SET_OPTION_IMPROVE_ADD_AG:
	case AT_SET_OPTION_IMPROVE_CRITICAL_DAMAGE_PERCENT:
	case AT_SET_OPTION_IMPROVE_CRITICAL_DAMAGE:
	case AT_SET_OPTION_IMPROVE_EXCELLENT_DAMAGE_PERCENT:
	case AT_SET_OPTION_IMPROVE_EXCELLENT_DAMAGE:
	case AT_SET_OPTION_IMPROVE_SKILL_ATTACK:
	case AT_SET_OPTION_DOUBLE_DAMAGE:
		mu_sprintf_s(text, textLength, GlobalText[949 + option], value);
		break;

	case AT_SET_OPTION_DISABLE_DEFENCE:
		mu_sprintf_s(text, textLength, GlobalText[970], value);
		break;

	case AT_SET_OPTION_TWO_HAND_SWORD_IMPROVE_DAMAGE:
		mu_sprintf_s(text, textLength, GlobalText[983], value);
		break;

	case AT_SET_OPTION_IMPROVE_SHIELD_DEFENCE:
		mu_sprintf_s(text, textLength, GlobalText[984], value);
		break;


	case AT_SET_OPTION_IMPROVE_ATTACK_1:
	case AT_SET_OPTION_IMPROVE_ATTACK_2:
	case AT_SET_OPTION_IMPROVE_MAGIC:
	case AT_SET_OPTION_IMPROVE_DEFENCE_3:
	case AT_SET_OPTION_IMPROVE_DEFENCE_4:
	case AT_SET_OPTION_FIRE_MASTERY:
	case AT_SET_OPTION_ICE_MASTERY:
	case AT_SET_OPTION_THUNDER_MASTERY:
	case AT_SET_OPTION_POSION_MASTERY:
	case AT_SET_OPTION_WATER_MASTERY:
	case AT_SET_OPTION_WIND_MASTERY:
	case AT_SET_OPTION_EARTH_MASTERY:
		mu_sprintf_s(text, textLength, GlobalText[971 + (option + AT_SET_OPTION_IMPROVE_STRENGTH - AT_SET_OPTION_IMPROVE_ATTACK_2)], value);
		break;
	}
}

void CSItemOption::PlusSpecial(mu_uint16* Value, mu_int32 Special)
{
	Special -= AT_SET_OPTION_IMPROVE_STRENGTH;
	mu_int32 optionValue = 0;
	for (mu_int32 i = 0; i < m_bySetOptionANum + m_bySetOptionBNum; ++i)
	{
		if (m_bySetOptionList[i][0] == Special && m_bySetOptionListOnOff[i] == 0)
		{
			optionValue += m_iSetOptionListValue[i][0];
			m_bySetOptionListOnOff[i] = 1;
		}
	}

	if (optionValue)
	{
		*Value += optionValue;
	}
}

void CSItemOption::PlusSpecialPercent(mu_uint16* Value, mu_int32 Special)
{
	Special -= AT_SET_OPTION_IMPROVE_STRENGTH;
	mu_int32 optionValue = 0;
	for (mu_int32 i = 0; i < m_bySetOptionANum + m_bySetOptionBNum; ++i)
	{
		if (m_bySetOptionList[i][0] == Special && m_bySetOptionListOnOff[i] == 0)
		{
			optionValue += m_iSetOptionListValue[i][0];
			m_bySetOptionListOnOff[i] = 1;
		}
	}

	if (optionValue)
	{
		*Value += ((*Value)*optionValue) / 100;
	}
}

void CSItemOption::PlusSpecialLevel(mu_uint16* Value, const mu_uint16 SrcValue, mu_int32 Special)
{
	Special -= AT_SET_OPTION_IMPROVE_STRENGTH;
	mu_int32 optionValue = 0;
	mu_int32 count = 0;
	for (mu_int32 i = 0; i < m_bySetOptionANum + m_bySetOptionBNum; ++i)
	{
		if (m_bySetOptionList[i][0] == Special && m_bySetOptionList[i][1] != 0 && m_bySetOptionListOnOff[i] == 0)
		{
			optionValue = m_iSetOptionListValue[i][0];
			m_bySetOptionListOnOff[i] = 1;
			count++;
		}
	}

	if (optionValue)
	{
		optionValue = SrcValue*optionValue / 100;
		*Value += (optionValue*count);
	}
}

void CSItemOption::PlusMastery(mu_int32* Value, const mu_uint8 MasteryType)
{
	mu_int32 optionValue = 0;
	for (mu_int32 i = 0; i < m_bySetOptionANum + m_bySetOptionBNum; ++i)
	{
		if (m_bySetOptionList[i][0] >= MASTERY_OPTION && (m_bySetOptionList[i][0] - MASTERY_OPTION - 5) == MasteryType && m_bySetOptionList[i][1] != 0 && m_bySetOptionListOnOff[i] == 0)
		{
			optionValue += m_iSetOptionListValue[i][0];
			m_bySetOptionListOnOff[i] = 1;
		}
	}

	if (optionValue)
	{
		*Value += optionValue;
	}
}

void CSItemOption::MinusSpecialPercent(mu_int32* Value, mu_int32 Special)
{
	Special -= AT_SET_OPTION_IMPROVE_STRENGTH;
	mu_int32 optionValue = 0;
	for (mu_int32 i = 0; i < m_bySetOptionANum + m_bySetOptionBNum; ++i)
	{
		if (m_bySetOptionList[i][0] == Special && m_bySetOptionList[i][1] != 0 && m_bySetOptionListOnOff[i] == 0)
		{
			optionValue += m_iSetOptionListValue[i][0];
			m_bySetOptionListOnOff[i] = 1;
		}
	}

	if (optionValue)
	{
		*Value -= *Value*optionValue / 100;
	}
}

void CSItemOption::GetSpecial(mu_uint16* Value, mu_int32 Special)
{
	Special -= AT_SET_OPTION_IMPROVE_STRENGTH;
	mu_int32 optionValue = 0;
	for (mu_int32 i = 0; i < m_bySetOptionANum + m_bySetOptionBNum; ++i)
	{
		if (m_bySetOptionList[i][0] == Special && m_bySetOptionListOnOff[i] == 0)
		{
			optionValue += m_iSetOptionListValue[i][0];
			m_bySetOptionListOnOff[i] = 1;
		}
	}

	if (optionValue)
	{
		*Value += optionValue;
	}
}

void CSItemOption::GetSpecialPercent(mu_uint16* Value, mu_int32 Special)
{
	Special -= AT_SET_OPTION_IMPROVE_STRENGTH;
	mu_int32 optionValue = 0;
	for (mu_int32 i = 0; i < m_bySetOptionANum + m_bySetOptionBNum; ++i)
	{
		if (m_bySetOptionList[i][0] == Special && m_bySetOptionListOnOff[i] == 0)
		{
			optionValue += m_iSetOptionListValue[i][0];
			m_bySetOptionListOnOff[i] = 1;
		}
	}

	if (optionValue)
	{
		*Value += *Value*optionValue / 100;
	}
}

void CSItemOption::GetSpecialLevel(mu_uint16* Value, const mu_uint16 SrcValue, mu_int32 Special)
{
	Special -= AT_SET_OPTION_IMPROVE_STRENGTH;
	mu_int32 optionValue = 0;
	mu_int32 count = 0;
	for (mu_int32 i = 0; i < m_bySetOptionANum + m_bySetOptionBNum; ++i)
	{
		if (m_bySetOptionList[i][0] == Special && m_bySetOptionList[i][1] != 0 && m_bySetOptionListOnOff[i] == 0)
		{
			optionValue = m_iSetOptionListValue[i][0];
			m_bySetOptionListOnOff[i] = 1;
			count++;
		}
	}

	if (optionValue)
	{
		optionValue = SrcValue*optionValue / 100;
		*Value += (optionValue*count);
	}
}

mu_int32 CSItemOption::GetDefaultOptionValue(ITEM* ip, mu_uint16* Value)
{
	*Value = ((ip->SetOption >> 2) % 0x04);

	ITEM_ATTRIBUTE* p = &ItemAttribute[ip->Type];

	return p->AttType;
}

mu_boolean CSItemOption::GetDefaultOptionText(const ITEM* ip, mu_text* Text, const mu_size TextLength)
{
	if (((ip->SetOption >> 2) % 0x04) <= 0) return false;

	switch (ItemAttribute[ip->Type].AttType)
	{
	case SET_OPTION_STRENGTH:
		mu_sprintf_s(Text, TextLength, GlobalText[950], ((ip->SetOption >> 2) % 0x04) * 5);
		break;

	case SET_OPTION_DEXTERITY:
		mu_sprintf_s(Text, TextLength, GlobalText[951], ((ip->SetOption >> 2) % 0x04) * 5);
		break;

	case SET_OPTION_ENERGY:
		mu_sprintf_s(Text, TextLength, GlobalText[952], ((ip->SetOption >> 2) % 0x04) * 5);
		break;

	case SET_OPTION_VITALITY:
		mu_sprintf_s(Text, TextLength, GlobalText[953], ((ip->SetOption >> 2) % 0x04) * 5);
		break;

	default:
		return false;
	}

	return true;
}

mu_boolean CSItemOption::Special_Option_Check(mu_int32 Kind)
{
	mu_int32 i, j;
	for (i = 0; i < 2; ++i)
	{
		ITEM* item = nullptr;
		item = &CharacterMachine->Equipment[GAME::EQUIPMENT_WEAPON_RIGHT + i];
		if (item == nullptr || item->Type <= -1)
			continue;

		if (Kind == 0)
		{
			for (j = 0; j < item->SpecialNum; ++j)
			{
				if (item->Special[j] == AT_SKILL_ICE_BLADE)
					return true;
			}
		}
		else if (Kind == 1)
		{
			for (j = 0; j < item->SpecialNum; ++j)
			{
				if (item->Special[j] == AT_SKILL_CROSSBOW)
					return true;
			}
		}
	}

	return false;
}

mu_int32 CSItemOption::RenderDefaultOptionText(const ITEM* ip, mu_int32 TextNum)
{
	mu_int32 TNum = TextNum;

	if (GetDefaultOptionText(ip, TextList[TNum], TEXTLIST_LENGTH))
	{
		TextListColor[TNum] = TEXT_COLOR_BLUE;
		++TNum;

		if ((ip->Type >= ITEM_HELPER + 8 && ip->Type <= ITEM_HELPER + 9) || (ip->Type >= ITEM_HELPER + 12 && ip->Type <= ITEM_HELPER + 13) || (ip->Type >= ITEM_HELPER + 21 && ip->Type <= ITEM_HELPER + 27))
		{
			mu_sprintf_s(TextList[TNum], TEXTLIST_LENGTH, GlobalText[1165]);
			TextListColor[TNum] = TEXT_COLOR_BLUE;
			++TNum;
		}
	}

	return TNum;
}

void CSItemOption::getAllAddState(mu_uint16* Strength, mu_uint16* Dexterity, mu_uint16* Energy, mu_uint16* Vitality, mu_uint16* Charisma)
{
	for (mu_int32 i = GAME::EQUIPMENT_WEAPON_RIGHT; i < GAME::MAX_EQUIPMENT; ++i)
	{
		ITEM* item = &CharacterMachine->Equipment[i];

		if (item->Durability <= 0)
		{
			continue;
		}

		mu_uint16 Result = 0;
		switch (GetDefaultOptionValue(item, &Result))
		{
		case SET_OPTION_STRENGTH:
			*Strength += Result * 5;
			break;

		case SET_OPTION_DEXTERITY:
			*Dexterity += Result * 5;
			break;

		case SET_OPTION_ENERGY:
			*Energy += Result * 5;
			break;

		case SET_OPTION_VITALITY:
			*Vitality += Result * 5;
			break;
		}
	}

	GetSpecial(Strength, AT_SET_OPTION_IMPROVE_STRENGTH);
	GetSpecial(Dexterity, AT_SET_OPTION_IMPROVE_DEXTERITY);
	GetSpecial(Energy, AT_SET_OPTION_IMPROVE_ENERGY);
	GetSpecial(Vitality, AT_SET_OPTION_IMPROVE_VITALITY);
	GetSpecial(Charisma, AT_SET_OPTION_IMPROVE_CHARISMA);
}

void CSItemOption::getAllAddStateOnlyAddValue(mu_uint16* AddStrength, mu_uint16* AddDexterity, mu_uint16* AddEnergy, mu_uint16* AddVitality, mu_uint16* AddCharisma)
{
	*AddStrength = *AddDexterity = *AddEnergy = *AddVitality = *AddCharisma = 0;
	mu_zeromem(m_bySetOptionListOnOff, sizeof(m_bySetOptionListOnOff));

	getAllAddState(AddStrength, AddDexterity, AddEnergy, AddVitality, AddCharisma);
}

void CSItemOption::getAllAddOptionStatesbyCompare(mu_uint16* Strength, mu_uint16* Dexterity, mu_uint16* Energy, mu_uint16* Vitality, mu_uint16* Charisma,
	mu_uint16 iCompareStrength, mu_uint16 iCompareDexterity, mu_uint16 iCompareEnergy, mu_uint16 iCompareVitality, mu_uint16 iCompareCharisma)
{
	for (mu_int32 i = GAME::EQUIPMENT_WEAPON_RIGHT; i < GAME::MAX_EQUIPMENT; ++i)
	{
		ITEM* item = &CharacterMachine->Equipment[i];

		if (item->RequireStrength > iCompareStrength ||
			item->RequireDexterity > iCompareDexterity ||
			item->RequireEnergy > iCompareEnergy)
		{
			continue;
		}

		if (item->Durability <= 0)
		{
			continue;
		}

		mu_uint16 Result = 0;
		switch (GetDefaultOptionValue(item, &Result))
		{
		case SET_OPTION_STRENGTH:
			*Strength += Result * 5;
			break;

		case SET_OPTION_DEXTERITY:
			*Dexterity += Result * 5;
			break;

		case SET_OPTION_ENERGY:
			*Energy += Result * 5;
			break;

		case SET_OPTION_VITALITY:
			*Vitality += Result * 5;
			break;
		}
	}

	mu_zeromem(m_bySetOptionListOnOff, sizeof(m_bySetOptionListOnOff));

	GetSpecial(Strength, AT_SET_OPTION_IMPROVE_STRENGTH);
	GetSpecial(Dexterity, AT_SET_OPTION_IMPROVE_DEXTERITY);
	GetSpecial(Energy, AT_SET_OPTION_IMPROVE_ENERGY);
	GetSpecial(Vitality, AT_SET_OPTION_IMPROVE_VITALITY);
	GetSpecial(Charisma, AT_SET_OPTION_IMPROVE_CHARISMA);
}

void CSItemOption::CheckItemSetOptions(void)
{
	mu_uint8 byOptionList[30] = { 0, };
	ITEM itemTmp;

	mu_zeromem(m_bySetOptionList, sizeof(m_bySetOptionList));
	mu_zeromem(&itemTmp, sizeof(itemTmp));

	for (mu_int32 i = 0; i < MAX_EQUIPMENT_INDEX; ++i)
	{
		if (i == GAME::EQUIPMENT_WING || i == GAME::EQUIPMENT_HELPER) continue;

		ITEM *ip = &CharacterMachine->Equipment[i];

		if (ip->Durability <= 0)
		{
			continue;
		}

		if ((i == GAME::EQUIPMENT_WEAPON_LEFT || i == GAME::EQUIPMENT_RING_LEFT) && itemTmp.Type == ip->Type && itemTmp.SetOption == (ip->SetOption % 0x04))
		{
			continue;
		}

		if (ip->Type > -1)
		{
			checkItemType(byOptionList, ip->Type, ip->SetOption);
		}

		if (i == GAME::EQUIPMENT_WEAPON_RIGHT || i == GAME::EQUIPMENT_RING_RIGHT)
		{
			itemTmp.Type = ip->Type;
			itemTmp.SetOption = (ip->SetOption % 0x04);
		}
	}

	calcSetOptionList(byOptionList);

	getAllAddStateOnlyAddValue(&CharacterAttribute->AddStrength, &CharacterAttribute->AddDexterity,
		&CharacterAttribute->AddEnergy, &CharacterAttribute->AddVitality,
		&CharacterAttribute->AddCharisma
	);

	mu_uint16 AllStrength = CharacterAttribute->Strength + CharacterAttribute->AddStrength;
	mu_uint16 AllDexterity = CharacterAttribute->Dexterity + CharacterAttribute->AddDexterity;
	mu_uint16 AllEnergy = CharacterAttribute->Energy + CharacterAttribute->AddEnergy;
	mu_uint16 AllVitality = CharacterAttribute->Vitality + CharacterAttribute->AddVitality;
	mu_uint16 AllCharisma = CharacterAttribute->Charisma + CharacterAttribute->AddCharisma;
	mu_uint16 AllLevel = CharacterAttribute->Level;

	mu_memset(byOptionList, 0, sizeof(byOptionList));
	mu_memset(m_bySetOptionList, 255, sizeof(m_bySetOptionList));


	for (mu_int32 i = 0; i < MAX_EQUIPMENT_INDEX; ++i)
	{
		if (i == GAME::EQUIPMENT_WING || i == GAME::EQUIPMENT_HELPER) continue;

		ITEM *ip = &CharacterMachine->Equipment[i];

		if (ip->RequireDexterity > AllDexterity ||
			ip->RequireEnergy > AllEnergy ||
			ip->RequireStrength > AllStrength ||
			ip->RequireLevel > AllLevel ||
			ip->RequireCharisma > AllCharisma ||
			ip->Durability <= 0 ||
			(IsRequireEquipItem(ip) == false))
		{
			continue;
		}

		if ((i == GAME::EQUIPMENT_WEAPON_LEFT || i == GAME::EQUIPMENT_RING_LEFT) && itemTmp.Type == ip->Type && itemTmp.SetOption == (ip->SetOption % 0x04))
		{
			continue;
		}

		if (ip->Type > -1)
		{
			checkItemType(byOptionList, ip->Type, ip->SetOption);
		}

		if (i == GAME::EQUIPMENT_WEAPON_RIGHT || i == GAME::EQUIPMENT_RING_RIGHT)
		{
			itemTmp.Type = ip->Type;
			itemTmp.SetOption = (ip->SetOption % 0x04);
		}
	}

	UpdateCount_SetOptionPerEquippedSetItem(byOptionList, m_arLimitSetItemOptionCount, CharacterMachine->Equipment);

	calcSetOptionList(byOptionList);
}

void CSItemOption::RenderSetOptionList(mu_float StartX, mu_float StartY)
{
	if (m_bViewOptionList && (m_bySetOptionANum > 0 || m_bySetOptionBNum > 0))
	{
		mu_float PosX, PosY;

		PosX = StartX + 95.0f;
		PosY = StartY + 40.0f;

		mu_uint8 TextNum = 0;
		mu_uint8 SkipNum = 0;
		mu_uint8 setIndex = 0;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("\n")); TextListColor[TextNum] = 0; TextBold[TextNum] = false; ++TextNum; SkipNum++;
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("\n")); TextListColor[TextNum] = 0; TextBold[TextNum] = false; ++TextNum; SkipNum++;
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("\n")); TextListColor[TextNum] = 0; TextBold[TextNum] = false; ++TextNum; SkipNum++;

		mu_int32 iCurSetItemTypeSequence = 0, iCurSetItemType = -1;

		for (mu_int32 i = 0; i < m_bySetOptionANum + m_bySetOptionBNum; ++i)
		{
			mu_string strCurrent;

			m_iterESIS = m_mapEquippedSetItemSequence.find(i);

			if (m_mapEquippedSetItemSequence.end() != m_iterESIS)
			{
				iCurSetItemTypeSequence = m_iterESIS->second;
			}
			else
			{
				iCurSetItemTypeSequence = -1;
			}

			if (iCurSetItemType != iCurSetItemTypeSequence)
			{
				iCurSetItemType = iCurSetItemTypeSequence;

				m_iterESIN = m_mapEquippedSetItemName.find(iCurSetItemTypeSequence);

				if (m_iterESIN != m_mapEquippedSetItemName.end())
				{
					strCurrent = m_iterESIN->second;

					mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(" ") SPF_STRING _T(""), strCurrent.c_str(), GlobalText[1089]);
					TextListColor[TextNum] = 3;
					TextBold[TextNum] = true;
					++TextNum;
				}
			}

			getExplainText(TextList[TextNum], TEXTLIST_LENGTH, m_bySetOptionList[i][0], m_iSetOptionListValue[i][0], setIndex);
			if (m_bySetOptionList[i][0] >= AT_SET_OPTION_IMPROVE_ATTACK_1 && m_bySetOptionList[i][1] == 0)
				TextListColor[TextNum] = 2;
			else
				TextListColor[TextNum] = 1;
			TextBold[TextNum] = false;
			++TextNum;
		}

		RenderTipTextList(PosX, PosY, TextNum, 120, ETA_CENTER);
		m_bViewOptionList = false;
	}
}

void CSItemOption::UpdateCount_SetOptionPerEquippedSetItem(const mu_uint8* byOptionList,
	mu_int32* arLimitSetItemOptionCount,
	ITEM* ItemsEquipment)
{
	for (mu_int32 iE = 0; iE < MAX_EQUIPMENT_INDEX; ++iE)
	{
		mu_int32 &iCurCount = arLimitSetItemOptionCount[iE];
		ITEM &CurItem = ItemsEquipment[iE];

		iCurCount = GetCurrentTypeSetitemCount(CurItem, byOptionList);
	}
}

mu_int32 CSItemOption::GetCurrentTypeSetitemCount(const ITEM &CurItem_, const mu_uint8* byOptionList)
{
	mu_uint8 bySetType = CurItem_.SetOption;

	mu_int32 setItemType = (bySetType % 0x04);

	ITEM_SET_TYPE& itemSType = m_ItemSetType[CurItem_.Type];

	for (mu_int32 i = 0; i < 30; i += 3)
	{
		ITEM_SET_OPTION& itemOption = m_ItemSetOption[byOptionList[i]];

		if (byOptionList[i] == itemSType.byOption[setItemType - 1])
		{
			mu_int32 iEquippedCount = byOptionList[i + 1];

			if (iEquippedCount >= itemOption.byOptionCount - 1)
			{
				return BYTE_MAX;
			}

			return iEquippedCount;
		}
	}

	return 0;
}

void CSItemOption::CheckRenderOptionHelper(const mu_text* FilterName)
{
	mu_text Name[256];

	if (FilterName[0] != '/') return;

	mu_int32 Length1 = mu_strlen(FilterName);
	for (mu_int32 i = 0; i < MAX_SET_OPTION; ++i)
	{
		ITEM_SET_OPTION& setOption = m_ItemSetOption[i];
		if (setOption.byOptionCount < 255)
		{
			mu_sprintf_s(Name, mu_countof(Name), _T("/") SPF_STRING _T(""), setOption.strSetName);

			mu_int32 Length2 = mu_strlen(Name);

			m_byRenderOptionList = 0;
			if (mu_strncmp(FilterName, Name, Length1) == 0 &&
				mu_strncmp(FilterName, Name, Length2) == 0)
			{
				g_pNewUISystem->Hide(GAMEUI::INTERFACE_ITEM_EXPLANATION);
				g_pNewUISystem->Hide(GAMEUI::INTERFACE_HELP);
				g_pNewUISystem->Show(GAMEUI::INTERFACE_SETITEM_EXPLANATION);

				m_byRenderOptionList = i + 1;
				return;
			}
		}
	}
}

void CSItemOption::RenderOptionHelper()
{
	if (m_byRenderOptionList <= 0) return;

	mu_int32 TextNum = 0;
	mu_float sx = 0, sy = 0;

	ITEM_SET_OPTION& setOption = m_ItemSetOption[m_byRenderOptionList - 1];
	if (setOption.byOptionCount >= 255)
	{
		m_byRenderOptionList = 0;
		return;
	}

	mu_uint8    option1 = 255;
	mu_uint8    option2 = 255;
	mu_uint8    value1 = 255;
	mu_uint8    value2 = 255;
	mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("\n"));
	++TextNum;

	mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(" ") SPF_STRING _T(" ") SPF_STRING _T(""), setOption.strSetName, GlobalText[1089], GlobalText[159]);
	TextListColor[TextNum] = TEXT_COLOR_YELLOW;
	++TextNum;

	mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("\n"));
	++TextNum;

	mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("\n"));
	++TextNum;

	for (mu_int32 i = 0; i < 13; ++i)
	{
		if (i < 6)
		{
			option1 = setOption.byStandardOption[i][0];
			option2 = setOption.byStandardOption[i][1];
			value1 = setOption.byStandardOptionValue[i][0];
			value2 = setOption.byStandardOptionValue[i][1];
		}
		else if (i < 8)
		{
			option1 = setOption.byExtOption[i - 6];
			value1 = setOption.byExtOptionValue[i - 6];
		}
		else
		{
			option1 = setOption.byFullOption[i - 8];
			value1 = setOption.byFullOptionValue[i - 8];
		}

		if (option1 != 255)
		{
			getExplainText(TextList[TextNum], TEXTLIST_LENGTH, option1, value1, 0);
			TextListColor[TextNum] = TEXT_COLOR_BLUE;
			TextBold[TextNum] = false; ++TextNum;
		}

		if (option2 != 255)
		{
			getExplainText(TextList[TextNum], TEXTLIST_LENGTH, option2, value2, 0);
			TextListColor[TextNum] = TEXT_COLOR_BLUE;
			TextBold[TextNum] = false; ++TextNum;
		}
	}

	mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("\n"));
	++TextNum;
	mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("\n"));
	++TextNum;

	RenderTipTextList(sx, sy, TextNum, 0, ETA_CENTER);
}

mu_int32 CSItemOption::GetSetItmeCount(const ITEM* pselecteditem)
{
	ITEM_SET_TYPE& itemsettype = m_ItemSetType[pselecteditem->Type];
	mu_uint8 subtype = itemsettype.byOption[(pselecteditem->SetOption % 0x04) - 1];

	mu_int32 setitemcount = 0;

	for (mu_int32 j = 0; j < MAX_ITEM; ++j)
	{
		ITEM_SET_TYPE& temptype = m_ItemSetType[j];
		for (mu_int32 i = 0; i < 2; ++i)
		{
			mu_uint8 tempsubtype = temptype.byOption[i];

			if (subtype == tempsubtype)
			{
				++setitemcount;
			}
		}
	}

	return setitemcount;
}

mu_boolean CSItemOption::isFullseteffect(const ITEM* pselecteditem)
{
	mu_int32 mysetitemcount = 0;

	ITEM_SET_TYPE& selectedItemType = m_ItemSetType[pselecteditem->Type];
	mu_uint8 selectedItemOption = selectedItemType.byOption[(pselecteditem->SetOption % 0x04) - 1];
	ITEM_SET_OPTION& selecteditemoption = m_ItemSetOption[selectedItemOption];
	mu_int32 Cmp_Buff[10] = { -1,-1,-1,-1,-1,-1,-1,-1,-1,-1 };

	for (mu_int32 i = 0; i < GAME::MAX_EQUIPMENT; ++i)
	{
		ITEM *p = &CharacterMachine->Equipment[i];

		if (p)
		{
			mu_boolean Continue_Set = false;
			for (mu_int32 ipjh = 0; ipjh < mysetitemcount; ++ipjh)
			{
				if (p->Type == Cmp_Buff[ipjh])
				{
					Continue_Set = true;
					break;
				}
			}
			if (Continue_Set == true)
				continue;
			ITEM_SET_TYPE& myitemSType = m_ItemSetType[p->Type];
			mu_uint8 myItemOption = myitemSType.byOption[(p->SetOption % 0x04) - 1];
			ITEM_SET_OPTION& setOption = m_ItemSetOption[myItemOption];

			if (mu_strcmp(selecteditemoption.strSetName, setOption.strSetName) == 0)
			{
				Cmp_Buff[mysetitemcount++] = p->Type;
			}
		}
	}

	if (mysetitemcount == GetSetItmeCount(pselecteditem))
		return true;
	else
		return false;
}

mu_int32 CSItemOption::RenderSetOptionListInItem(const ITEM* ip, mu_int32 TextNum, mu_boolean bIsEquippedItem)
{
	ITEM_SET_TYPE& itemSType = m_ItemSetType[ip->Type];

	m_bySelectedItemOption = itemSType.byOption[(ip->SetOption % 0x04) - 1];

	if (m_bySelectedItemOption <= 0 || m_bySelectedItemOption == 255) return TextNum;

	mu_int32 TNum = TextNum;

	ITEM_SET_OPTION& setOption = m_ItemSetOption[m_bySelectedItemOption];
	if (setOption.byOptionCount >= 255)
	{
		m_bySelectedItemOption = 0;
		return TNum;
	}

	mu_uint8    option1 = 255;
	mu_uint8    option2 = 255;
	mu_uint8    value1 = 255;
	mu_uint8    value2 = 255;
	mu_uint8    count1 = 0;

	mu_uint8    byLimitOptionNum;

	if (m_bySetOptionANum > 0)
		byLimitOptionNum = m_bySetOptionANum - m_bySameSetItem;
	else
		byLimitOptionNum = 0;

	if (m_bySetOptionBNum > 0)
		byLimitOptionNum += m_bySetOptionBNum - m_bySameSetItem;

	count1 = Search_From_EquippedSetItemNameSequence(setOption.strSetName);

	if (BYTE_MAX == count1)
	{
		byLimitOptionNum = 0;
	}
	else
	{
		byLimitOptionNum = EMath::Abs(((mu_int32)m_bySetOptionANum + (mu_int32)m_bySetOptionBNum) - (mu_int32)m_bySameSetItem);
	}

	mu_sprintf_s(TextList[TNum], TEXTLIST_LENGTH, _T("\n")); TNum += 1;
	mu_sprintf_s(TextList[TNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(" ") SPF_STRING _T(""), GlobalText[1089], GlobalText[159]);
	TextListColor[TNum] = TEXT_COLOR_YELLOW;
	++TNum;

	mu_sprintf_s(TextList[TNum], TEXTLIST_LENGTH, _T("\n")); ++TNum;
	mu_sprintf_s(TextList[TNum], TEXTLIST_LENGTH, _T("\n")); ++TNum;

	mu_boolean isfulloption = isFullseteffect(ip);

	if (isfulloption)
	{
		byLimitOptionNum = 13;
	}

	mu_uint8 byCurrentSelectedSlotIndex = ip->bySelectedSlotIndex;

	mu_int32 iLimitOptionCount = m_arLimitSetItemOptionCount[byCurrentSelectedSlotIndex] - 1;

	for (mu_int32 i = 0; i <= MAX_SETITEM_OPTIONS; ++i)
	{
		if (i < 6)
		{
			option1 = setOption.byStandardOption[i][0];
			option2 = setOption.byStandardOption[i][1];
			value1 = setOption.byStandardOptionValue[i][0];
			value2 = setOption.byStandardOptionValue[i][1];
		}
		else if (i < 8)
		{
			if (((ip->SetOption % 0x04) - 1) == 0)
			{
				option1 = setOption.byExtOption[i - 6];
				value1 = setOption.byExtOptionValue[i - 6];
			}
			else
			{
				option2 = setOption.byExtOption[i - 6];
				value2 = setOption.byExtOptionValue[i - 6];
			}
		}
		else
		{
			if (((ip->SetOption % 0x04) - 1) == 0)
			{
				option1 = setOption.byFullOption[i - 8];
				value1 = setOption.byFullOptionValue[i - 8];
			}
			else
			{
				option2 = setOption.byFullOption[i - 8];
				value2 = setOption.byFullOptionValue[i - 8];
			}

			if (isfulloption)
			{
				byLimitOptionNum = 13;
			}
			else
			{
				byLimitOptionNum = 255;
			}
		}

		if (option1 != 255)
		{
			getExplainText(TextList[TNum], TEXTLIST_LENGTH, option1, value1, 0);


			if (m_bySetOptionList[count1][0] == option1
				&& byLimitOptionNum != 255
				&& iLimitOptionCount > i
				&& byLimitOptionNum != 0
				&& bIsEquippedItem == false)
			{
				TextListColor[TNum] = TEXT_COLOR_BLUE;
				++count1;
			}
			else
			{
				TextListColor[TNum] = TEXT_COLOR_GRAY;
			}
			TextBold[TNum] = false; ++TNum;
		}
		if (option2 != 255)
		{
			getExplainText(TextList[TNum], TEXTLIST_LENGTH, option2, value2, 0);

			if (m_bySetOptionList[count1][0] == option2
				&& byLimitOptionNum != 255
				&& iLimitOptionCount > i
				&& byLimitOptionNum != 0
				)
			{
				TextListColor[TNum] = TEXT_COLOR_BLUE;
				++count1;
			}
			else
			{
				TextListColor[TNum] = TEXT_COLOR_GRAY;
			}
			TextBold[TNum] = false; ++TNum;
		}
	}
	mu_sprintf_s(TextList[TNum], TEXTLIST_LENGTH, _T("\n")); ++TNum;
	mu_sprintf_s(TextList[TNum], TEXTLIST_LENGTH, _T("\n")); ++TNum;

	return TNum;
}

mu_uint8 CSItemOption::GetSetOptionANum()
{
	return m_bySetOptionANum;
}

mu_uint8 CSItemOption::GetSetOptionBNum()
{
	return m_bySetOptionBNum;
}

void CSItemOption::SetViewOptionList(mu_boolean bView)
{
	m_bViewOptionList = bView;
}

mu_boolean CSItemOption::IsViewOptionList()
{
	return m_bViewOptionList;
}
