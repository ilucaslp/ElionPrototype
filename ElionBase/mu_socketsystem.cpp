#include "stdafx.h"
#include "mu_socketsystem.h"

CSocketItemMgr g_SocketItemMgr;

CSocketItemMgr::CSocketItemMgr()
{
	m_iNumEquitSetBonusOptions = 0;
	mu_zeromem(m_SocketOptionInfo, sizeof(m_SocketOptionInfo));
	mu_zeromem(&m_StatusBonus, sizeof(m_StatusBonus));
}

CSocketItemMgr::~CSocketItemMgr()
{

}

mu_boolean CSocketItemMgr::IsSocketItem(const ITEM* pItem)
{
	return IsSocketItem(pItem->Type);
}

mu_boolean CSocketItemMgr::IsSocketItem(const OBJECT* pObject)
{
	return IsSocketItem(pObject->Type - MODEL_SWORD);
}

mu_boolean CSocketItemMgr::IsSocketItem(mu_int32 iItemType)
{
	switch (iItemType)
	{
	case ITEM_SWORD + 26:
	case ITEM_SWORD + 27:
	case ITEM_SWORD + 28:
	case ITEM_MACE + 16:
	case ITEM_MACE + 17:
	case ITEM_BOW + 23:
	case ITEM_STAFF + 30:
	case ITEM_STAFF + 31:
	case ITEM_STAFF + 32:
	case ITEM_SHIELD + 17:
	case ITEM_SHIELD + 18:
	case ITEM_SHIELD + 19:
	case ITEM_SHIELD + 20:
	case ITEM_HELM + 45: case ITEM_ARMOR + 45: case ITEM_PANTS + 45: case ITEM_GLOVES + 45: case ITEM_BOOTS + 45:
	case ITEM_HELM + 46: case ITEM_ARMOR + 46: case ITEM_PANTS + 46: case ITEM_GLOVES + 46: case ITEM_BOOTS + 46:
	case ITEM_HELM + 47: case ITEM_ARMOR + 47: case ITEM_PANTS + 47: case ITEM_GLOVES + 47: case ITEM_BOOTS + 47:
	case ITEM_HELM + 48: case ITEM_ARMOR + 48: case ITEM_PANTS + 48: case ITEM_GLOVES + 48: case ITEM_BOOTS + 48:
	case ITEM_HELM + 49: case ITEM_ARMOR + 49: case ITEM_PANTS + 49: case ITEM_GLOVES + 49: case ITEM_BOOTS + 49:
	case ITEM_HELM + 50: case ITEM_ARMOR + 50: case ITEM_PANTS + 50: case ITEM_GLOVES + 50: case ITEM_BOOTS + 50:
	case ITEM_HELM + 51: case ITEM_ARMOR + 51: case ITEM_PANTS + 51: case ITEM_GLOVES + 51: case ITEM_BOOTS + 51:
	case ITEM_HELM + 52: case ITEM_ARMOR + 52: case ITEM_PANTS + 52: case ITEM_GLOVES + 52: case ITEM_BOOTS + 52:
	case ITEM_HELM + 53: case ITEM_ARMOR + 53: case ITEM_PANTS + 53: case ITEM_GLOVES + 53: case ITEM_BOOTS + 53:
		return true;
	default:
		return false;
	}
}

mu_int32 CSocketItemMgr::GetSocketCategory(mu_int32 iSeedID)
{
	if (iSeedID == SOCKET_EMPTY) return SOCKET_EMPTY;

	SOCKET_OPTION_INFO * pInfo = &m_SocketOptionInfo[SOT_SOCKET_ITEM_OPTIONS][iSeedID];
	return pInfo->m_iOptionCategory;
}

mu_int32 CSocketItemMgr::GetSeedShpereSeedID(const ITEM* pItem)
{
	mu_uint8 bySocketSeedID = SOCKET_EMPTY;

	if (pItem->Type >= ITEM_WING + 100 && pItem->Type <= ITEM_WING + 129)
	{
		mu_int32 iCategoryIndex = (pItem->Type - (ITEM_WING + 100)) % 6 + 1;
		mu_int32 iLevel = pItem->Level;
		switch (iCategoryIndex)
		{
		case 1:
			bySocketSeedID = 0 + iLevel;
			break;
		case 2:
			bySocketSeedID = 10 + iLevel;
			break;
		case 3:
			bySocketSeedID = 16 + iLevel;
			break;
		case 4:
			bySocketSeedID = 21 + iLevel;
			break;
		case 5:
			bySocketSeedID = 29 + iLevel;
			break;
		case 6:
			bySocketSeedID = 34 + iLevel;
			break;
		}
	}

	return bySocketSeedID;
}

mu_int64 CSocketItemMgr::CalcSocketBonusItemValue(const ITEM * pItem, mu_int64 iOrgGold)
{
	mu_int64 iGoldResult = 0;

	if (IsSocketItem(pItem))
	{
		iGoldResult += iOrgGold * (pItem->SocketCount * 0.8f);

		ITEM TempSeedSphere;
		for (mu_int32 i = 0; i < pItem->SocketCount; ++i)
		{
			if (pItem->SocketSeedID[i] == SOCKET_EMPTY) continue;

			mu_int32 iSeedSphereType = 0;
			if (pItem->SocketSeedID[i] >= 0 && pItem->SocketSeedID[i] <= 9) iSeedSphereType = 0;
			else if (pItem->SocketSeedID[i] >= 10 && pItem->SocketSeedID[i] <= 15) iSeedSphereType = 1;
			else if (pItem->SocketSeedID[i] >= 16 && pItem->SocketSeedID[i] <= 20) iSeedSphereType = 2;
			else if (pItem->SocketSeedID[i] >= 21 && pItem->SocketSeedID[i] <= 28) iSeedSphereType = 3;
			else if (pItem->SocketSeedID[i] >= 29 && pItem->SocketSeedID[i] <= 33) iSeedSphereType = 4;
			else if (pItem->SocketSeedID[i] >= 34 && pItem->SocketSeedID[i] <= 40) iSeedSphereType = 5;

			TempSeedSphere.Type = ITEM_WING + 100 + (pItem->SocketSphereLv[i] - 1) * MAX_SOCKET_TYPES + iSeedSphereType;

			iGoldResult += ItemValue(&TempSeedSphere, 0);
		}
	}

	return iGoldResult;
}

mu_int32 CSocketItemMgr::CalcSocketOptionValue(mu_int32 iOptionType, mu_float fOptionValue)
{
	switch (iOptionType)
	{
	case 1:
		return mu_int32(fOptionValue);
	case 2:
		return mu_int32(fOptionValue);
	case 3:
		{
			mu_uint16 wLevel;

			if (IsMasterLevel(CharacterAttribute->Class) == true)
				wLevel = CharacterAttribute->Level + Master_Level_Data.nMLevel;
			else
				wLevel = CharacterAttribute->Level;

			return mu_int32((mu_float)wLevel / fOptionValue);
		}
	case 4:
		{
			return mu_int32(CharacterAttribute->LifeMax / fOptionValue);
		}
	case 5:
		{
			return mu_int32(CharacterAttribute->ManaMax / fOptionValue);
		}
	default:
		return 0;
	}
}

void CSocketItemMgr::CalcSocketOptionValueText(mu_text * pszOptionValueText, const mu_size OptionValueTextLength, mu_int32 iOptionType, mu_float fOptionValue)
{
	switch (iOptionType)
	{
	case 1:
		mu_sprintf_s(pszOptionValueText, OptionValueTextLength, _T("+%d"), CalcSocketOptionValue(iOptionType, fOptionValue));
		break;
	case 2:
		mu_sprintf_s(pszOptionValueText, OptionValueTextLength, _T("+%d%%"), CalcSocketOptionValue(iOptionType, fOptionValue));
		break;
	case 3:
		mu_sprintf_s(pszOptionValueText, OptionValueTextLength, _T("+%d"), CalcSocketOptionValue(iOptionType, fOptionValue));
		break;
	case 4:
		mu_sprintf_s(pszOptionValueText, OptionValueTextLength, _T("+%d"), CalcSocketOptionValue(iOptionType, fOptionValue));
		break;
	case 5:
		mu_sprintf_s(pszOptionValueText, OptionValueTextLength, _T("+%d"), CalcSocketOptionValue(iOptionType, fOptionValue));
		break;
	}
}

void CSocketItemMgr::CreateSocketOptionText(mu_text * pszOptionText, const mu_size OptionTextLength, mu_int32 iSeedID, mu_int32 iSphereLv)
{
	if (pszOptionText == nullptr) return;

	mu_text szOptionValueText[16] = { 0, };

	SOCKET_OPTION_INFO * pInfo = &m_SocketOptionInfo[SOT_SOCKET_ITEM_OPTIONS][iSeedID];
	mu_float fOptionValue = (mu_float)pInfo->m_iOptionValue[iSphereLv - 1];

	CalcSocketOptionValueText(szOptionValueText, mu_countof(szOptionValueText), pInfo->m_bOptionType, fOptionValue);

	mu_sprintf_s(pszOptionText, OptionTextLength, _T("") SPF_STRING _T("(") SPF_STRING _T(" ") SPF_STRING _T(")"), GlobalText[2640 + pInfo->m_iOptionCategory - 1], pInfo->m_szOptionName, szOptionValueText);
}

mu_int32 CSocketItemMgr::AttachToolTipForSocketItem(const ITEM* pItem, mu_int32 iTextNum)
{
	if (pItem->SocketCount == 0) return iTextNum;

	mu_sprintf_s(TextList[iTextNum], TEXTLIST_LENGTH, _T("\n")); ++iTextNum; ++SkipNum;
	mu_sprintf_s(TextList[iTextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(" ") SPF_STRING _T(""), GlobalText[2650], GlobalText[159]);
	TextListColor[iTextNum] = TEXT_COLOR_PURPLE;
	TextBold[iTextNum] = false;
	++iTextNum;
	mu_sprintf_s(TextList[iTextNum], TEXTLIST_LENGTH, _T("\n")); ++iTextNum; ++SkipNum;

	mu_text szOptionText[64] = { 0, };
	mu_text szOptionValueText[16] = { 0, };
	SOCKET_OPTION_INFO * pInfo = nullptr;
	for (mu_int32 i = 0; i < pItem->SocketCount; ++i)
	{
		if (pItem->SocketSeedID[i] == SOCKET_EMPTY)
		{
			mu_sprintf_s(szOptionText, mu_countof(szOptionText), GlobalText[2652]);
			TextListColor[iTextNum] = TEXT_COLOR_GRAY;
		}
		else if (pItem->SocketSeedID[i] < MAX_SOCKET_OPTION)
		{
			CreateSocketOptionText(szOptionText, mu_countof(szOptionText), pItem->SocketSeedID[i], pItem->SocketSphereLv[i]);
			TextListColor[iTextNum] = TEXT_COLOR_BLUE;
		}

		mu_sprintf_s(TextList[iTextNum], TEXTLIST_LENGTH, GlobalText[2655], i + 1, szOptionText);
		TextBold[iTextNum] = false;
		++iTextNum;
	}

	if (pItem->SocketSeedSetOption < MAX_SOCKET_OPTION)
	{
		mu_sprintf_s(TextList[iTextNum], TEXTLIST_LENGTH, _T("\n")); ++iTextNum; ++SkipNum;

		mu_sprintf_s(TextList[iTextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(""), GlobalText[2656]);
		TextListColor[iTextNum] = TEXT_COLOR_PURPLE;
		TextBold[iTextNum] = false;
		++iTextNum;
		mu_sprintf_s(TextList[iTextNum], TEXTLIST_LENGTH, _T("\n")); ++iTextNum; ++SkipNum;

		pInfo = &m_SocketOptionInfo[SOT_MIX_SET_BONUS_OPTIONS][pItem->SocketSeedSetOption];
		CalcSocketOptionValueText(szOptionValueText, mu_countof(szOptionValueText), pInfo->m_bOptionType, (mu_float)pInfo->m_iOptionValue[0]);
		mu_sprintf_s(TextList[iTextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(" ") SPF_STRING _T(""), pInfo->m_szOptionName, szOptionValueText);
		TextListColor[iTextNum] = TEXT_COLOR_BLUE;
		TextBold[iTextNum] = false;
		++iTextNum;
	}

	return iTextNum;
}

mu_int32 CSocketItemMgr::AttachToolTipForSeedSphereItem(const ITEM* pItem, mu_int32 iTextNum)
{
	SOCKET_OPTION_INFO * pInfo = nullptr;

	if (pItem->Type >= ITEM_WING + 60 && pItem->Type <= ITEM_WING + 65)
	{
		mu_int32 iCategoryIndex = pItem->Type - (ITEM_WING + 60) + 1;
		mu_sprintf_s(TextList[iTextNum], TEXTLIST_LENGTH, GlobalText[2653], GlobalText[2640 + iCategoryIndex - 1]);
		TextListColor[iTextNum] = TEXT_COLOR_WHITE;
		TextBold[iTextNum] = false;
		++iTextNum;

		mu_sprintf_s(TextList[iTextNum], TEXTLIST_LENGTH, _T("\n")); ++iTextNum; ++SkipNum;

		mu_int32 iSocketSeedID = 0;
		mu_int32 iLevel = pItem->Level;
		switch (iCategoryIndex)
		{
		case 1:
			iSocketSeedID = 0 + iLevel;
			break;
		case 2:
			iSocketSeedID = 10 + iLevel;
			break;
		case 3:
			iSocketSeedID = 16 + iLevel;
			break;
		case 4:
			iSocketSeedID = 21 + iLevel;
			break;
		case 5:
			iSocketSeedID = 29 + iLevel;
			break;
		case 6:
			iSocketSeedID = 34 + iLevel;
			break;
		}
		pInfo = &m_SocketOptionInfo[SOT_SOCKET_ITEM_OPTIONS][iSocketSeedID];
		mu_sprintf_s(TextList[iTextNum], TEXTLIST_LENGTH, pInfo->m_szOptionName);
		TextListColor[iTextNum] = TEXT_COLOR_BLUE;
		TextBold[iTextNum] = false;
		++iTextNum;
	}
	else if (pItem->Type >= ITEM_WING + 70 && pItem->Type <= ITEM_WING + 74)
	{
		mu_int32 iSphereLevel = pItem->Type - (ITEM_WING + 70) + 1;
		mu_sprintf_s(TextList[iTextNum], TEXTLIST_LENGTH, GlobalText[2654], iSphereLevel);
		TextListColor[iTextNum] = TEXT_COLOR_WHITE;
		TextBold[iTextNum] = false;
		++iTextNum;
	}
	else if (pItem->Type >= ITEM_WING + 100 && pItem->Type <= ITEM_WING + 129)
	{
		mu_int32 iCategoryIndex = (pItem->Type - (ITEM_WING + 100)) % 6 + 1;
		mu_sprintf_s(TextList[iTextNum], TEXTLIST_LENGTH, GlobalText[2653], GlobalText[2640 + iCategoryIndex - 1]);
		TextListColor[iTextNum] = TEXT_COLOR_WHITE;
		TextBold[iTextNum] = false;
		++iTextNum;

		mu_sprintf_s(TextList[iTextNum], TEXTLIST_LENGTH, _T("\n")); ++iTextNum; ++SkipNum;

		mu_int32 iSocketSeedID = 0;
		mu_int32 iLevel = pItem->Level;
		switch (iCategoryIndex)
		{
		case 1:
			iSocketSeedID = 0 + iLevel;
			break;
		case 2:
			iSocketSeedID = 10 + iLevel;
			break;
		case 3:
			iSocketSeedID = 16 + iLevel;
			break;
		case 4:
			iSocketSeedID = 21 + iLevel;
			break;
		case 5:
			iSocketSeedID = 29 + iLevel;
			break;
		case 6:
			iSocketSeedID = 34 + iLevel;
			break;
		}

		pInfo = &m_SocketOptionInfo[SOT_SOCKET_ITEM_OPTIONS][iSocketSeedID];

		mu_text szOptionValueText[16] = { 0, };
		mu_float fOptionValue = (mu_float)pInfo->m_iOptionValue[(pItem->Type - (ITEM_WING + 100)) / 6];
		CalcSocketOptionValueText(szOptionValueText, mu_countof(szOptionValueText), pInfo->m_bOptionType, fOptionValue);

		mu_sprintf_s(TextList[iTextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(" ") SPF_STRING _T(""), pInfo->m_szOptionName, szOptionValueText);
		TextListColor[iTextNum] = TEXT_COLOR_BLUE;
		TextBold[iTextNum] = false;
		++iTextNum;
	}

	return iTextNum;
}

mu_boolean CSocketItemMgr::IsSocketSetOptionEnabled()
{
	return (!m_EquipSetBonusList.empty());
}

void CSocketItemMgr::RenderToolTipForSocketSetOption(mu_float iPos_x, mu_float iPos_y)
{
	if (IsSocketSetOptionEnabled())
	{
		mu_float PosX, PosY;

		PosX = iPos_x + 95;
		PosY = iPos_y + 40;

		mu_uint8 TextNum = 0;
		mu_uint8 SkipNum = 0;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("\n")); TextListColor[TextNum] = 0; TextBold[TextNum] = false; ++TextNum; SkipNum++;
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("\n")); TextListColor[TextNum] = 0; TextBold[TextNum] = false; ++TextNum; SkipNum++;
		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("\n")); TextListColor[TextNum] = 0; TextBold[TextNum] = false; ++TextNum; SkipNum++;

		mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, GlobalText[2657]);
		TextListColor[TextNum] = TEXT_COLOR_PURPLE;
		TextBold[TextNum] = true;
		++TextNum;

		mu_text szOptionValueText[16] = { 0, };
		SOCKET_OPTION_INFO * pInfo = nullptr;
		for (auto iter = m_EquipSetBonusList.begin(); iter != m_EquipSetBonusList.end(); ++iter)
		{
			pInfo = &m_SocketOptionInfo[SOT_EQUIP_SET_BONUS_OPTIONS][*iter];
			CalcSocketOptionValueText(szOptionValueText, mu_countof(szOptionValueText), pInfo->m_bOptionType, (mu_float)pInfo->m_iOptionValue[0]);
			mu_sprintf_s(TextList[TextNum], TEXTLIST_LENGTH, _T("") SPF_STRING _T(" ") SPF_STRING _T(""), pInfo->m_szOptionName, szOptionValueText);
			TextListColor[TextNum] = TEXT_COLOR_BLUE;
			TextBold[TextNum] = false;
			++TextNum;
		}

		RenderTipTextList(PosX, PosY, TextNum, 140, ETA_CENTER);
	}
}

void CSocketItemMgr::CheckSocketSetOption()
{
	m_EquipSetBonusList.clear();

	mu_int32 iSeedSum[6] = { 0, 0, 0, 0, 0, 0 };
	ITEM * pItem = nullptr;
	SOCKET_OPTION_INFO * pInfo = nullptr;

	for (mu_int32 i = 0; i < GAME::MAX_EQUIPMENT; ++i)
	{
		pItem = &CharacterMachine->Equipment[i];
		for (mu_int32 j = 0; j < pItem->SocketCount; ++j)
		{
			if (pItem->SocketSeedID[j] != SOCKET_EMPTY)
			{
				pInfo = &m_SocketOptionInfo[SOT_SOCKET_ITEM_OPTIONS][pItem->SocketSeedID[j]];
				++iSeedSum[pInfo->m_iOptionCategory - 1];
			}
		}
	}

	for (mu_int32 i = 0; i < m_iNumEquitSetBonusOptions; ++i)
	{
		mu_int32 icnt = 0;
		mu_uint8 * pbySetTest = m_SocketOptionInfo[SOT_EQUIP_SET_BONUS_OPTIONS][i].m_bySocketCheckInfo;
		for (mu_int32 j = 0; j < 6; ++j)
		{
			icnt = j;
			if (iSeedSum[j] < pbySetTest[j]) break;
		}
		if (icnt < 6) continue;

		m_EquipSetBonusList.push_back(m_SocketOptionInfo[SOT_EQUIP_SET_BONUS_OPTIONS][i].m_iOptionID);
	}
}

mu_int32 CSocketItemMgr::GetSocketOptionValue(const ITEM * pItem, mu_int32 iSocketIndex)
{
	if (pItem->SocketCount > 0 && pItem->SocketSeedID[iSocketIndex] != SOCKET_EMPTY)
	{
		SOCKET_OPTION_INFO * pInfo = nullptr;
		pInfo = &m_SocketOptionInfo[SOT_SOCKET_ITEM_OPTIONS][pItem->SocketSeedID[iSocketIndex]];
		mu_float fOptionValue = (mu_float)pInfo->m_iOptionValue[pItem->SocketSphereLv[iSocketIndex] - 1];

		return CalcSocketOptionValue(pInfo->m_bOptionType, fOptionValue);
	}
	else
	{
		return 0;
	}
}

void CSocketItemMgr::CalcSocketStatusBonus()
{
	memset(&m_StatusBonus, 0, sizeof(SOCKET_OPTION_STATUS_BONUS));
	m_StatusBonus.m_fDefenceRateBonus = 1.0f;

	ITEM * pItem = nullptr;
	SOCKET_OPTION_INFO * pInfo = nullptr;

	for (mu_int32 i = 0; i < GAME::MAX_EQUIPMENT; ++i)
	{
		pItem = &CharacterMachine->Equipment[i];

		if (!IsSocketItem(pItem)) continue;

		for (mu_int32 j = 0; j < pItem->SocketCount; ++j)
		{
			if (pItem->SocketSeedID[j] != SOCKET_EMPTY)
			{
				pInfo = &m_SocketOptionInfo[SOT_SOCKET_ITEM_OPTIONS][pItem->SocketSeedID[j]];
				mu_float fOptionValue = (mu_float)pInfo->m_iOptionValue[pItem->SocketSphereLv[j] - 1];
				mu_int32 iBonus = CalcSocketOptionValue(pInfo->m_bOptionType, fOptionValue);

				switch (pInfo->m_iOptionID)
				{
				case SOPT_ATTACK_N_MAGIC_DAMAGE_BONUS_BY_LEVEL:
				case SOPT_ATTACK_N_MAGIC_DAMAGE_BONUS:
					m_StatusBonus.m_iAttackDamageMinBonus += iBonus;
					m_StatusBonus.m_iAttackDamageMaxBonus += iBonus;
					break;
				case SOPT_ATTACK_SPEED_BONUS:
					m_StatusBonus.m_iAttackSpeedBonus += iBonus;
					break;
				case SOPT_ATTACT_N_MAGIC_DAMAGE_MAX_BONUS:
					m_StatusBonus.m_iAttackDamageMaxBonus += iBonus;
					break;
				case SOPT_ATTACK_N_MAGIC_DAMAGE_MIN_BONUS:
					m_StatusBonus.m_iAttackDamageMinBonus += iBonus;
					break;

				case SOPT_DEFENCE_RATE_BONUS:
					m_StatusBonus.m_fDefenceRateBonus *= 1.0f + iBonus * 0.01f;
					break;
				case SOPT_DEFENCE_BONUS:
					m_StatusBonus.m_iDefenceBonus += iBonus;
					break;
				case SOPT_SHIELD_DEFENCE_BONUS:
					m_StatusBonus.m_iShieldDefenceBonus += iBonus;
					break;
				case SOPT_SKILL_DAMAGE_BONUS:
					m_StatusBonus.m_iSkillAttackDamageBonus += iBonus;
					break;
				case SOPT_ATTACK_RATE_BONUS:
					m_StatusBonus.m_iAttackRateBonus += iBonus;
					break;
				case SOPT_STRENGTH_BONUS:
					m_StatusBonus.m_iStrengthBonus += iBonus;
					break;
				case SOPT_DEXTERITY_BONUS:
					m_StatusBonus.m_iDexterityBonus += iBonus;
					break;
				case SOPT_VITALITY_BONUS:
					m_StatusBonus.m_iVitalityBonus += iBonus;
					break;
				case SOPT_ENERGY_BONUS:
					m_StatusBonus.m_iEnergyBonus += iBonus;
					break;
				}
			}
		}

		if (pItem->SocketSeedSetOption != SOCKET_EMPTY)
		{
			pInfo = &m_SocketOptionInfo[SOT_MIX_SET_BONUS_OPTIONS][pItem->SocketSeedSetOption];
			mu_int32 iBonus = CalcSocketOptionValue(pInfo->m_bOptionType, (mu_float)pInfo->m_iOptionValue[0]);

			switch (pInfo->m_iOptionID)
			{
			case SBOPT_ATTACK_DAMAGE_BONUS:
				m_StatusBonus.m_iAttackDamageMinBonus += iBonus;
				m_StatusBonus.m_iAttackDamageMaxBonus += iBonus;
				break;
			case SBOPT_SKILL_DAMAGE_BONUS:
			case SBOPT_SKILL_DAMAGE_BONUS_2:
				m_StatusBonus.m_iSkillAttackDamageBonus += iBonus;
				break;
			case SBOPT_MAGIC_POWER_BONUS:
				m_StatusBonus.m_iAttackDamageMinBonus += iBonus;
				m_StatusBonus.m_iAttackDamageMaxBonus += iBonus;
				m_StatusBonus.m_iMagicPowerBonus += iBonus;
				break;
			case SBOPT_DEFENCE_BONUS:
				m_StatusBonus.m_iDefenceBonus += iBonus;
				break;
			}
		}
	}
}

mu_boolean CSocketItemMgr::OpenSocketItemScript(const mu_text *szFileName)
{
	SDL_RWops *file = nullptr;

	if (mu_rwfromfile(&file, szFileName, "rb") == false)
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

	for (mu_uint32 n = 0; n < MAX_SOCKET_OPTION_TYPES; ++n)
	{
		for (mu_uint32 j = 0; j < MAX_SOCKET_OPTION; ++j)
		{
			SOCKET_OPTION_INFO &b = m_SocketOptionInfo[n][j];

			mu_uint32 textLength = 0;
			FileHeaders::ReadBuffer(readBuffer, textLength);
			textData.resize(textLength);
			FileHeaders::ReadBuffer(readBuffer, (mu_char*)textData.data(), textLength);
#ifdef UNICODE
			mu_strcpy_s(b.m_szOptionName, mu_countof(b.m_szOptionName), boost::locale::conv::utf_to_utf<mu_text>(textData).c_str());
#else
			mu_strcpy_s(b.m_szOptionName, mu_countof(b.m_szOptionName), textData.c_str());
#endif
			FileHeaders::ReadBuffer(readBuffer, b.m_iOptionID);
			FileHeaders::ReadBuffer(readBuffer, b.m_iOptionCategory);
			FileHeaders::ReadBuffer(readBuffer, b.m_bOptionType);
			FileHeaders::ReadBuffer(readBuffer, b.m_iOptionValue, sizeof(b.m_iOptionValue));
			FileHeaders::ReadBuffer(readBuffer, b.m_bySocketCheckInfo, sizeof(b.m_bySocketCheckInfo));
		}
	}

	for (mu_int32 i = 0; i < MAX_SOCKET_OPTION; ++i)
	{
		m_iNumEquitSetBonusOptions = i;
		mu_uint8 * pbySetTest = m_SocketOptionInfo[SOT_EQUIP_SET_BONUS_OPTIONS][i].m_bySocketCheckInfo;
		if (pbySetTest[0] +
			pbySetTest[1] +
			pbySetTest[2] +
			pbySetTest[3] +
			pbySetTest[4] +
			pbySetTest[5] == 0) break;
	}

	delete[] decompressedBuffer;

	return true;
}