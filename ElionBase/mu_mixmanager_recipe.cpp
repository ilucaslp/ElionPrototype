#include "stdafx.h"
#include "mu_mixmanager.h"

MUMixRecipe::MUMixRecipe()
{
	Reset();
}

MUMixRecipe::~MUMixRecipe()
{
	Reset();
}

void MUMixRecipe::Reset()
{
	ClearCheckRecipeResult();
	SetRecipes(nullptr, 0);
}

void MUMixRecipe::SetRecipes(MUMixRecipeData *Recipes, mu_int32 RecipesCount)
{
	m_Recipes = Recipes;
	m_RecipesCount = RecipesCount;
}

void MUMixRecipe::ClearCheckRecipeResult()
{
	m_iCurMixIndex = 0;
	m_iMostSimilarMixIndex = 0;
	m_iSuccessRate = 0;
	m_dwRequiredZen = 0;
	m_bFindMixLuckItem = false;
	m_dwTotalItemValue = 0;
	m_dwExcellentItemValue = 0;
	m_dwEquipmentItemValue = 0;
	m_dwWingItemValue = 0;
	m_dwSetItemValue = 0;
	m_iFirstItemLevel = 0;
	m_iFirstItemType = 0;
	m_dwTotalNonJewelItemValue = 0;
	m_byFirstItemSocketCount = 0;
	for (mu_int32 i = 0; i < MAX_SOCKETS; ++i)
	{
		m_byFirstItemSocketSeedID[i] = SOCKET_EMPTY;
		m_byFirstItemSocketSphereLv[i] = 0;
	}
	m_wTotalCharmBonus = 0;
}

mu_int32 MUMixRecipe::CheckRecipe(mu_int32 iNumMixItems, MUMixItem * pMixItems)
{
	m_iCurMixIndex = 0;

	for (mu_int32 n = 0; n < m_RecipesCount; ++n)
	{
		MUMixRecipeData *recipe = &m_Recipes[n];

		for (mu_int32 i = 0; i < iNumMixItems; ++i)
		{
			pMixItems[i].m_iTestCount = pMixItems[i].m_iCount;
		}
		if (CheckRecipeSub(recipe, iNumMixItems, pMixItems) == true)
		{
			m_iCurMixIndex = recipe->MixIndex + 1;
			EvaluateMixItems(iNumMixItems, pMixItems);
			CalcCharmBonusRate(iNumMixItems, pMixItems);
			CalcMixRate(iNumMixItems, pMixItems);
			CalcMixReqZen(iNumMixItems, pMixItems);
			return GetCurRecipe()->MixID;
		}
		else
		{
			m_iSuccessRate = 0;
			m_dwRequiredZen = 0;
		}
	}

	return -1;
}

mu_int32 MUMixRecipe::CheckRecipeSimilarity(mu_int32 iNumMixItems, MUMixItem * pMixItems)
{
	if (iNumMixItems == 0 && m_RecipesCount == 1)
	{
		m_iMostSimilarMixIndex = 1;
		MUMixRecipeData *recipe = &m_Recipes[0];
		for (mu_int32 i = 0; i < recipe->NumMixSoruces; ++i)
		{
			m_iMostSimilarMixSourceTest[i] = recipe->MixSources[0].CountMax;
		}
		return m_iMostSimilarMixIndex;
	}

	mu_int32 iMostSimiliarRecipe = 0;
	mu_int32 iMostSimiliarityPoint = 0;
	mu_zeromem(m_iMostSimilarMixSourceTest, sizeof(m_iMostSimilarMixSourceTest));
	mu_zeromem(m_iMixSourceTest, sizeof(m_iMixSourceTest));

	mu_int32 iSimilarityPoint;
	for (mu_int32 n = 0; n < m_RecipesCount; ++n)
	{
		MUMixRecipeData *recipe = &m_Recipes[n];
		mu_zeromem(m_iMixSourceTest, sizeof(m_iMixSourceTest));
		for (mu_int32 i = 0; i < recipe->NumMixSoruces; ++i)
			m_iMixSourceTest[i] = recipe->MixSources[i].CountMax;

		for (mu_int32 i = 0; i < iNumMixItems; ++i)
			pMixItems[i].m_iTestCount = pMixItems[i].m_iCount;

		iSimilarityPoint = CheckRecipeSimilaritySub(recipe, iNumMixItems, pMixItems);
		if (iSimilarityPoint == 1 && m_RecipesCount > 1) iSimilarityPoint = 0;
		if (iSimilarityPoint > iMostSimiliarityPoint ||
			(iSimilarityPoint == iMostSimiliarityPoint && iSimilarityPoint > 0 && m_iCurMixIndex == recipe->MixIndex + 1))
		{
			iMostSimiliarityPoint = iSimilarityPoint;
			iMostSimiliarRecipe = recipe->MixIndex + 1;
			mu_zeromem(m_iMostSimilarMixSourceTest, sizeof(m_iMostSimilarMixSourceTest));
			for (mu_int32 i = 0; i < recipe->NumMixSoruces; ++i)
			{
				m_iMostSimilarMixSourceTest[i] = m_iMixSourceTest[i];
			}
		}
	}

	m_iMostSimilarMixIndex = iMostSimiliarRecipe;

	return iMostSimiliarRecipe;
}

mu_boolean MUMixRecipe::IsMixSource(ITEM * pItem)
{
	MUMixItem mixitem;
	mixitem.SetItem(pItem, 0);

	if (IsCharmItem(mixitem))
	{
		if ((GetCurRecipe() == nullptr || GetCurRecipe()->CharmOption == 'A')
			&& m_wTotalCharmBonus + mixitem.m_iCount <= 10)
		{
			return true;
		}
	}

	if (IsChaosCharmItem(mixitem))
	{
		if ((GetCurRecipe() == nullptr || GetCurRecipe()->CharmOption == 'A')
			&& m_wTotalChaosCharmCount < 1)
		{
			return true;
		}
	}

	for (mu_int32 n = 0; n < m_RecipesCount; ++n)
	{
		MUMixRecipeData *recipe = &m_Recipes[n];
		for (mu_int32 j = 0; j < recipe->NumMixSoruces; ++j)
		{
			if (m_wTotalCharmBonus > 0 && recipe->CharmOption != 'A')
			{
				continue;
			}

			if (CheckItem(recipe->MixSources[j], mixitem)
				&& !(recipe->MixOption == 'B' && IsChaosItem(mixitem))
				&& !(recipe->MixOption == 'C' && Is380AddedItem(mixitem))
				&& !(recipe->MixOption == 'D' && IsFenrirAddedItem(mixitem))
				&& !(recipe->MixOption == 'E' && !IsUpgradableItem(mixitem))
				&& !(recipe->MixOption == 'G' && !IsSourceOfRefiningStone(mixitem))
				)
			{
				return true;
			}
		}
	}
	return false;
}

MUMixRecipeData *MUMixRecipe::GetCurRecipe()
{
	if (m_iCurMixIndex == 0) return nullptr;
	return &m_Recipes[m_iCurMixIndex - 1];
}

mu_int32 MUMixRecipe::GetCurMixID()
{
	if (m_iCurMixIndex == 0) return 0;
	return m_Recipes[m_iCurMixIndex - 1].MixID;
}

mu_boolean MUMixRecipe::GetCurRecipeName(mu_text * pszNameOut, mu_size NameLength, mu_int32 iNameLine)
{
	if (!IsReadyToMix())
	{
		if (iNameLine == 1)
		{
			switch (g_MixManager.GetMixType())
			{
			case MIXTYPE_TRAINER:
				mu_sprintf_s(pszNameOut, NameLength, GlobalText[1213], GlobalText[1212]);
				break;
			case MIXTYPE_OSBOURNE:
				mu_sprintf_s(pszNameOut, NameLength, GlobalText[1213], GlobalText[2061]);
				break;
			case MIXTYPE_JERRIDON:
				mu_sprintf_s(pszNameOut, NameLength, GlobalText[1213], GlobalText[2062]);
				break;
			case MIXTYPE_ELPIS:
				mu_sprintf_s(pszNameOut, NameLength, GlobalText[1213], GlobalText[2063]);
				break;
			default:
				mu_sprintf_s(pszNameOut, NameLength, _T("") SPF_STRING _T(""), GlobalText[601]);
				break;
			}
			return true;
		}
		else return false;
	}
	return GetRecipeName(GetCurRecipe(), pszNameOut, NameLength, iNameLine, false);
}

mu_boolean MUMixRecipe::GetCurRecipeDesc(mu_text * pszDescOut, mu_size DescLength, mu_int32 iDescLine)
{
	if (iDescLine > 3 || iDescLine < 1) return false;
	if (GetCurRecipe() == nullptr) return false;
	if (GetCurRecipe()->MixDesc[iDescLine - 1] > 0)
		mu_strcpy_s(pszDescOut, DescLength, GlobalText[GetCurRecipe()->MixDesc[iDescLine - 1]]);
	else
		return false;
	return true;
}

MUMixRecipeData *MUMixRecipe::GetMostSimilarRecipe()
{
	if (m_iMostSimilarMixIndex == 0) return nullptr;
	return &m_Recipes[m_iMostSimilarMixIndex - 1];
}

mu_boolean MUMixRecipe::GetMostSimilarRecipeName(mu_text * pszNameOut, mu_size NameLength, mu_int32 iNameLine)
{
	return GetRecipeName(GetMostSimilarRecipe(), pszNameOut, NameLength, iNameLine, true);
}

mu_boolean MUMixRecipe::GetRecipeAdvice(mu_text * pszAdviceOut, mu_size AdviceLength, mu_int32 iAdivceLine)
{
	if (GetMostSimilarRecipe() == nullptr) return false;
	if (iAdivceLine > 3 || iAdivceLine < 1) return false;

	if (GetMostSimilarRecipe()->MixAdvice[iAdivceLine - 1] > 0)
		mu_strcpy_s(pszAdviceOut, AdviceLength, GlobalText[GetMostSimilarRecipe()->MixAdvice[iAdivceLine - 1]]);
	else
		return false;
	return true;
}

mu_int32 MUMixRecipe::GetSourceName(mu_int32 iItemNum, mu_text * pszNameOut, mu_size NameLength, mu_int32 iNumMixItems, MUMixItem * pMixItems)
{
	if (iNumMixItems < 0)	return MIX_SOURCE_ERROR;
	if (pMixItems == nullptr)	return MIX_SOURCE_ERROR;
	if (GetMostSimilarRecipe() == nullptr) return MIX_SOURCE_ERROR;
	if (iItemNum >= GetMostSimilarRecipe()->NumMixSoruces) return MIX_SOURCE_ERROR;

	MUMixRecipeItem * pMixRecipeItem = &GetMostSimilarRecipe()->MixSources[iItemNum];

	mu_text szTempName[100];
	GetItemName(pMixRecipeItem->TypeMin, pMixRecipeItem->LevelMin, szTempName, mu_countof(szTempName));

	if (pMixRecipeItem->TypeMin == pMixRecipeItem->TypeMax &&
		(pMixRecipeItem->LevelMin == pMixRecipeItem->LevelMax || (pMixRecipeItem->LevelMin == 0 && pMixRecipeItem->LevelMax == 255)) &&
		(pMixRecipeItem->OptionMin == pMixRecipeItem->OptionMax || (pMixRecipeItem->OptionMin == 0 && pMixRecipeItem->OptionMax == 255)))
	{
		if (pMixRecipeItem->DurabilityMin == pMixRecipeItem->DurabilityMax)
			mu_sprintf_s(szTempName, mu_countof(szTempName),_T("") SPF_STRING _T("(%d)"), szTempName, pMixRecipeItem->DurabilityMin);
	}
	else
	{
		if (pMixRecipeItem->SpecialItem & RCP_SP_ADD380ITEM)
			mu_sprintf_s(szTempName, mu_countof(szTempName),GlobalText[2335]);
		else if (pMixRecipeItem->TypeMin == 0 && pMixRecipeItem->TypeMax == ITEM_BOOTS + MAX_ITEM_INDEX - 1)
			mu_sprintf_s(szTempName, mu_countof(szTempName),GlobalText[2336]);
		else if (pMixRecipeItem->TypeMin == 0 && pMixRecipeItem->TypeMax == ITEM_HELPER + MAX_ITEM_INDEX - 1)
			mu_sprintf_s(szTempName, mu_countof(szTempName),GlobalText[2336]);
		else if (pMixRecipeItem->TypeMin == 0 && pMixRecipeItem->TypeMax == ITEM_STAFF + MAX_ITEM_INDEX - 1)
			mu_sprintf_s(szTempName, mu_countof(szTempName),GlobalText[2337]);
		else if (pMixRecipeItem->TypeMin == ITEM_SHIELD && pMixRecipeItem->TypeMax == ITEM_BOOTS + MAX_ITEM_INDEX - 1)
			mu_sprintf_s(szTempName, mu_countof(szTempName),GlobalText[2338]);
		else if (pMixRecipeItem->TypeMin == ITEM_WING && pMixRecipeItem->TypeMax == ITEM_WING + 2)
			mu_sprintf_s(szTempName, mu_countof(szTempName),GlobalText[2339]);
		else if (pMixRecipeItem->TypeMin == ITEM_WING + 3 && pMixRecipeItem->TypeMax == ITEM_WING + 6)
			mu_sprintf_s(szTempName, mu_countof(szTempName),GlobalText[2348]);
		else if (pMixRecipeItem->TypeMin == ITEM_WING + 41 && pMixRecipeItem->TypeMax == ITEM_WING + 41)
			mu_sprintf_s(szTempName, mu_countof(szTempName),GlobalText[2339]);
		else if (pMixRecipeItem->TypeMin == ITEM_WING + 42 && pMixRecipeItem->TypeMax == ITEM_WING + 42)
			mu_sprintf_s(szTempName, mu_countof(szTempName),GlobalText[2348]);
		else if (pMixRecipeItem->TypeMin == pMixRecipeItem->TypeMax &&
			(pMixRecipeItem->TypeMin == ITEM_MACE + 6 || pMixRecipeItem->TypeMin == ITEM_BOW + 6 || pMixRecipeItem->TypeMin == ITEM_STAFF + 7))
			mu_sprintf_s(szTempName, mu_countof(szTempName),GlobalText[2340]);
		else if (pMixRecipeItem->TypeMin == ITEM_WING + 60 && pMixRecipeItem->TypeMax == ITEM_WING + 65)
			mu_sprintf_s(szTempName, mu_countof(szTempName),GlobalText[2680]);
		else if (pMixRecipeItem->TypeMin == ITEM_WING + 70 && pMixRecipeItem->TypeMax == ITEM_WING + 74)
			mu_sprintf_s(szTempName, mu_countof(szTempName),GlobalText[2681]);
		else if (pMixRecipeItem->TypeMin == ITEM_WING + 100 && pMixRecipeItem->TypeMax == ITEM_WING + 129)
			mu_sprintf_s(szTempName, mu_countof(szTempName),GlobalText[2682]);
		else if (pMixRecipeItem->TypeMin == ITEM_WING + 100 && pMixRecipeItem->TypeMax == ITEM_WING + 128)
			mu_sprintf_s(szTempName, mu_countof(szTempName),_T("") SPF_STRING _T(" (") SPF_STRING _T(")"), GlobalText[2682], GlobalText[2684]);
		else if (pMixRecipeItem->TypeMin == ITEM_WING + 101 && pMixRecipeItem->TypeMax == ITEM_WING + 129)
			mu_sprintf_s(szTempName, mu_countof(szTempName),_T("") SPF_STRING _T(" (") SPF_STRING _T(")"), GlobalText[2682], GlobalText[2685]);
		else
		{
			mu_int32 iNameLen = mu_strlen(szTempName);
			for (mu_int32 j = 1; j <= 3 && iNameLen - j - 1 >= 0; ++j)
				if (szTempName[iNameLen - j] == '+') szTempName[iNameLen - j - 1] = '\0';
		}

		if (pMixRecipeItem->DurabilityMin == pMixRecipeItem->DurabilityMax)
			mu_sprintf_s(szTempName, mu_countof(szTempName),_T("") SPF_STRING _T("(%d)"), szTempName, pMixRecipeItem->DurabilityMin);

		if (pMixRecipeItem->LevelMin == 0 && pMixRecipeItem->LevelMax == 255);
		else if (pMixRecipeItem->LevelMin == pMixRecipeItem->LevelMax)
			mu_sprintf_s(szTempName, mu_countof(szTempName),_T("") SPF_STRING _T(" +%d"), szTempName, pMixRecipeItem->LevelMin);
		else if (pMixRecipeItem->LevelMin == 0)
			mu_sprintf_s(szTempName, mu_countof(szTempName),_T("") SPF_STRING _T(" +%d") SPF_STRING _T(""), szTempName, pMixRecipeItem->LevelMax, GlobalText[2342]);
		else if (pMixRecipeItem->LevelMax == 255)
			mu_sprintf_s(szTempName, mu_countof(szTempName),_T("") SPF_STRING _T(" +%d") SPF_STRING _T(""), szTempName, pMixRecipeItem->LevelMin, GlobalText[2341]);
		else
			mu_sprintf_s(szTempName, mu_countof(szTempName),_T("") SPF_STRING _T(" +%d~%d"), szTempName, pMixRecipeItem->LevelMin, pMixRecipeItem->LevelMax);

		if (pMixRecipeItem->OptionMin == 0 && pMixRecipeItem->OptionMax == 255);
		else if (pMixRecipeItem->OptionMin == pMixRecipeItem->OptionMax)
			mu_sprintf_s(szTempName, mu_countof(szTempName),_T("") SPF_STRING _T(" +%d") SPF_STRING _T(""), szTempName, pMixRecipeItem->OptionMin, GlobalText[2343]);
		else if (pMixRecipeItem->OptionMin == 0)
			mu_sprintf_s(szTempName, mu_countof(szTempName),_T("") SPF_STRING _T(" +%d") SPF_STRING _T("") SPF_STRING _T(""), szTempName, pMixRecipeItem->OptionMax, GlobalText[2343], GlobalText[2342]);
		else if (pMixRecipeItem->OptionMax == 255)
			mu_sprintf_s(szTempName, mu_countof(szTempName),_T("") SPF_STRING _T(" +%d") SPF_STRING _T("") SPF_STRING _T(""), szTempName, pMixRecipeItem->OptionMin, GlobalText[2343], GlobalText[2341]);
		else
			mu_sprintf_s(szTempName, mu_countof(szTempName),_T("") SPF_STRING _T(" +%d~%d") SPF_STRING _T(""), szTempName, pMixRecipeItem->OptionMin, pMixRecipeItem->OptionMax, GlobalText[2343]);
	}

	if (pMixRecipeItem->CountMin == 0 && pMixRecipeItem->CountMax == 255)
		mu_sprintf_s(szTempName, mu_countof(szTempName),_T("") SPF_STRING _T(" (") SPF_STRING _T(")"), szTempName, GlobalText[2344]);
	else if (pMixRecipeItem->CountMin == pMixRecipeItem->CountMax)
		mu_sprintf_s(szTempName, mu_countof(szTempName),_T("") SPF_STRING _T(" %d ") SPF_STRING _T(""), szTempName, pMixRecipeItem->CountMin, GlobalText[2345]);
	else if (pMixRecipeItem->CountMin == 0)
		mu_sprintf_s(szTempName, mu_countof(szTempName),_T("") SPF_STRING _T(" %d ") SPF_STRING _T(" ") SPF_STRING _T(""), szTempName, pMixRecipeItem->CountMax, GlobalText[2345], GlobalText[2342]);
	else if (pMixRecipeItem->CountMax == 255)
		mu_sprintf_s(szTempName, mu_countof(szTempName),_T("") SPF_STRING _T(" %d ") SPF_STRING _T(" ") SPF_STRING _T(""), szTempName, pMixRecipeItem->CountMin, GlobalText[2345], GlobalText[2341]);
	else
		mu_sprintf_s(szTempName, mu_countof(szTempName),_T("") SPF_STRING _T(" %d~%d ") SPF_STRING _T(""), szTempName, pMixRecipeItem->CountMin, pMixRecipeItem->CountMax, GlobalText[2345]);

	mu_boolean bPreName = false;
	if (pMixRecipeItem->SpecialItem & RCP_SP_EXCELLENT)
	{
		mu_sprintf_s(pszNameOut, NameLength, _T("") SPF_STRING _T(" ") SPF_STRING _T(""), GlobalText[620], szTempName);
		bPreName = true;
	}
	if (pMixRecipeItem->SpecialItem & RCP_SP_SETITEM)
	{
		mu_sprintf_s(pszNameOut, NameLength, _T("") SPF_STRING _T(" ") SPF_STRING _T(""), GlobalText[1089], szTempName);
		bPreName = true;
	}
	if (pMixRecipeItem->SpecialItem & RCP_SP_HARMONY)
	{
		mu_sprintf_s(pszNameOut, NameLength, _T("") SPF_STRING _T(" ") SPF_STRING _T(""), GlobalText[1550], szTempName);
		bPreName = true;
	}
	if (pMixRecipeItem->SpecialItem & RCP_SP_SOCKETITEM)
	{
		mu_sprintf_s(pszNameOut, NameLength, _T("") SPF_STRING _T(" ") SPF_STRING _T(""), GlobalText[2650], szTempName);
		bPreName = true;
	}
	if (bPreName == false)
	{
		mu_strcpy_s(pszNameOut, NameLength, szTempName);
	}

	if (g_MixManager.IsMixInit())
	{
		if (pMixRecipeItem->CountMin == 0) return MIX_SOURCE_PARTIALLY;
		else return MIX_SOURCE_NO;
	}

	if (m_iMostSimilarMixSourceTest[iItemNum] == 0)
		return MIX_SOURCE_YES;

	else if (m_iMostSimilarMixSourceTest[iItemNum] < pMixRecipeItem->CountMax)
	{
		if (pMixRecipeItem->CountMin <= 1) return MIX_SOURCE_YES;
		else return MIX_SOURCE_PARTIALLY;
	}
	else
	{
		if (pMixRecipeItem->CountMin == 0) return MIX_SOURCE_PARTIALLY;
		else return MIX_SOURCE_NO;
	}
}

void MUMixRecipe::CalcCharmBonusRate(mu_int32 iNumMixItems, MUMixItem * pMixItems)
{
	m_wTotalCharmBonus = 0;
	for (mu_int32 i = 0; i < iNumMixItems; ++i)
	{
		if (pMixItems[i].m_bIsCharmItem == true) m_wTotalCharmBonus += pMixItems[i].m_iCount;
	}
}

void MUMixRecipe::CalcChaosCharmCount(mu_int32 iNumMixItems, MUMixItem * pMixItems)
{
	m_wTotalChaosCharmCount = 0;
	for (mu_int32 i = 0; i < iNumMixItems; ++i)
	{
		if (pMixItems[i].m_bIsChaosCharmItem == true) m_wTotalChaosCharmCount += 1;
	}
}

mu_boolean MUMixRecipe::CheckRecipeSub(MUMixRecipeData *Recipe, mu_int32 iNumMixItems, MUMixItem * pMixItems)
{
	mu_boolean bFind = false;
	mu_int32 iMixRecipeTest[MAX_MIX_SOURCES];
	memset(iMixRecipeTest, 0, sizeof(mu_int32) * MAX_MIX_SOURCES);

	for (mu_int32 j = 0; j < Recipe->NumMixSoruces; ++j)
	{
		if (!IsOptionItem(Recipe->MixSources[j])) bFind = false;
		for (mu_int32 i = 0; i < iNumMixItems; ++i)
		{
			if (CheckItem(Recipe->MixSources[j], pMixItems[i]) && pMixItems[i].m_iTestCount > 0 &&
				Recipe->MixSources[j].CountMax >= iMixRecipeTest[j] + pMixItems[i].m_iTestCount
				&& !(Recipe->MixOption == 'H' && IsSourceOfAttachSeedSphereToArmor(pMixItems[i]))
				&& !(Recipe->MixOption == 'I' && IsSourceOfAttachSeedSphereToWeapon(pMixItems[i]))
				)
			{
				if (pMixItems[i].m_iTestCount >= Recipe->MixSources[j].CountMax)
				{
					iMixRecipeTest[j] += Recipe->MixSources[j].CountMax;
					pMixItems[i].m_iTestCount -= Recipe->MixSources[j].CountMax;
				}
				else
				{
					iMixRecipeTest[j] += pMixItems[i].m_iTestCount;
					pMixItems[i].m_iTestCount = 0;
				}
				bFind = true;
				if (j == 0)
				{
					m_iFirstItemLevel = pMixItems[i].m_iLevel;
					m_iFirstItemType = pMixItems[i].m_sType;

					m_byFirstItemSocketCount = pMixItems[i].m_bySocketCount;
					if (m_byFirstItemSocketCount > 0)
					{
						for (mu_int32 k = 0; k < MAX_SOCKETS; ++k)
						{
							m_byFirstItemSocketSeedID[k] = pMixItems[i].m_bySocketSeedID[k];
							m_byFirstItemSocketSphereLv[k] = pMixItems[i].m_bySocketSphereLv[k];
						}
					}
				}
			}
		}
		if (bFind == false || Recipe->MixSources[j].CountMin > iMixRecipeTest[j])
		{
			return false;
		}
	}

	for (mu_int32 i = 0; i < iNumMixItems; ++i)
	{
		if (pMixItems[i].m_iTestCount > 0)
		{
			if (pMixItems[i].m_bIsCharmItem && Recipe->CharmOption == 'A');
			else if (pMixItems[i].m_bIsChaosCharmItem && Recipe->ChaosCharmOption == 'A');
			else return false;
		}
	}
	return true;
}

mu_int32 MUMixRecipe::CheckRecipeSimilaritySub(MUMixRecipeData *Recipe, mu_int32 iNumMixItems, MUMixItem * pMixItems)
{
	mu_int32 iFindTotalPoint = 0;
	mu_int32 iFindPoint = 0;

	for (mu_int32 j = 0; j < Recipe->NumMixSoruces; ++j)
	{
		for (mu_int32 i = 0; i < iNumMixItems; ++i)
		{
			if (CheckItem(Recipe->MixSources[j], pMixItems[i]) && pMixItems[i].m_iTestCount > 0
				&& !(Recipe->MixOption == 'H' && IsSourceOfAttachSeedSphereToArmor(pMixItems[i]))
				&& !(Recipe->MixOption == 'I' && IsSourceOfAttachSeedSphereToWeapon(pMixItems[i]))
				)
			{
				if (IsChaosJewel(pMixItems[i])) iFindPoint = 1;
				else if (Recipe->MixSources[j].CountMax < pMixItems[i].m_iTestCount) iFindPoint = 1;
				else if (j == 0) iFindPoint = 10;
				else if (j == 1) iFindPoint = 5;
				else iFindPoint = 3;

				iFindTotalPoint += iFindPoint;

				if (pMixItems[i].m_iTestCount > 0 && m_iMixSourceTest[j] > 0)
				{
					m_iMixSourceTest[j] -= pMixItems[i].m_iTestCount;
					pMixItems[i].m_iTestCount -= Recipe->MixSources[j].CountMax;
				}
			}
		}
	}
	for (mu_int32 i = 0; i < iNumMixItems; ++i)
	{
		if (pMixItems[i].m_iTestCount > 0)
		{
			if (pMixItems[i].m_bIsCharmItem && Recipe->CharmOption == 'A');
			else if (pMixItems[i].m_bIsChaosCharmItem && Recipe->ChaosCharmOption == 'A');
			else return 0;
		}
	}
	return iFindTotalPoint;
}

bool MUMixRecipe::CheckItem(MUMixRecipeItem & rItem, MUMixItem & rSource)
{
	if (rItem.TypeMin <= rSource.m_sType && rItem.TypeMax >= rSource.m_sType &&
		rItem.LevelMin <= rSource.m_iLevel && rItem.LevelMax >= rSource.m_iLevel &&
		rItem.DurabilityMin <= rSource.m_iDurability && rItem.DurabilityMax >= rSource.m_iDurability &&
		rItem.OptionMin <= rSource.m_iOption && rItem.OptionMax >= rSource.m_iOption &&
		(rItem.SpecialItem & RCP_SP_EXCELLENT) <= (rSource.m_dwSpecialItem & RCP_SP_EXCELLENT) &&
		(rItem.SpecialItem & RCP_SP_ADD380ITEM) <= (rSource.m_dwSpecialItem & RCP_SP_ADD380ITEM) &&
		(rItem.SpecialItem & RCP_SP_SETITEM) <= (rSource.m_dwSpecialItem & RCP_SP_SETITEM)
		&& (rItem.SpecialItem & RCP_SP_HARMONY) <= (rSource.m_dwSpecialItem & RCP_SP_HARMONY)
		&& (rItem.SpecialItem & RCP_SP_SOCKETITEM) <= (rSource.m_dwSpecialItem & RCP_SP_SOCKETITEM)
		)
	{
		return true;
	}
	return false;
}

void MUMixRecipe::EvaluateMixItems(mu_int32 iNumMixItems, MUMixItem * pMixItems)
{
	m_bFindMixLuckItem = false;
	m_dwTotalItemValue = 0;
	m_dwExcellentItemValue = 0;
	m_dwEquipmentItemValue = 0;
	m_dwWingItemValue = 0;
	m_dwSetItemValue = 0;
	m_dwTotalNonJewelItemValue = 0;

	for (mu_int32 i = 0; i < iNumMixItems; ++i)
	{
		if (pMixItems[i].m_bMixLuck == true) m_bFindMixLuckItem = true;
		if (pMixItems[i].m_dwSpecialItem & RCP_SP_EXCELLENT) m_dwExcellentItemValue += pMixItems[i].m_dwMixValue;
		if (pMixItems[i].m_bIsEquipment == true) m_dwEquipmentItemValue += pMixItems[i].m_dwMixValue;
		if (pMixItems[i].m_bIsWing == true) m_dwWingItemValue += pMixItems[i].m_dwMixValue;
		if (pMixItems[i].m_dwSpecialItem & RCP_SP_SETITEM) m_dwSetItemValue += pMixItems[i].m_dwMixValue;
		if (pMixItems[i].m_bIsJewelItem == false) m_dwTotalNonJewelItemValue += pMixItems[i].m_dwMixValue;
		m_dwTotalItemValue += pMixItems[i].m_dwMixValue;
	}
}

void MUMixRecipe::CalcMixRate(mu_int32 iNumMixItems, MUMixItem * pMixItems)
{
	if (iNumMixItems < 0)	return;
	if (pMixItems == nullptr)	return;

	m_iSuccessRate = 0;
	if (GetCurRecipe() == nullptr) return;

	m_pMixRates = GetCurRecipe()->RateToken;
	m_iMixRateIter = 0;
	m_iSuccessRate = (mu_int32)MixrateAddSub();

	if (m_iSuccessRate > GetCurRecipe()->SuccessRate)
	{
		m_iSuccessRate = GetCurRecipe()->SuccessRate;
	}
	if (GetCurRecipe()->CharmOption == 'A')
	{
		m_iSuccessRate += m_wTotalCharmBonus;
	}
	if (m_iSuccessRate > 100)
	{
		m_iSuccessRate = 100;
	}
}

void MUMixRecipe::CalcMixReqZen(mu_int32 iNumMixItems, MUMixItem * pMixItems)
{
	if (iNumMixItems < 0)	return;
	if (pMixItems == nullptr)	return;

	m_dwRequiredZen = 0;
	if (GetCurRecipe() == nullptr) return;
	switch (GetCurRecipe()->RequiredZenType)
	{
	case 'A':
	case 'C':
		m_dwRequiredZen = GetCurRecipe()->RequiredZen;
		break;
	case 'B':
		m_dwRequiredZen = (mu_int64)m_iSuccessRate * (mu_int64)GetCurRecipe()->RequiredZen;
		break;
	case 'D':
		{
			mu_int32 iItemType = 0;
			if (ITEM_SWORD <= pMixItems[0].m_sType && ITEM_STAFF > pMixItems[0].m_sType)
			{
				iItemType = SI_Weapon;
			}
			else if (ITEM_STAFF <= pMixItems[0].m_sType && ITEM_SHIELD > pMixItems[0].m_sType)
			{
				iItemType = SI_Staff;
			}
			else if (ITEM_SHIELD <= pMixItems[0].m_sType && ITEM_WING > pMixItems[0].m_sType)
			{
				iItemType = SI_Defense;
			}
			m_dwRequiredZen = g_pUIJewelHarmonyinfo.GetHarmonyJewelOptionInfo(iItemType,
				pMixItems[0].m_wHarmonyOption).Zen[pMixItems[0].m_wHarmonyOptionLevel];
		}
		break;
	}
}

mu_boolean MUMixRecipe::GetRecipeName(MUMixRecipeData * pRecipe, mu_text * pszNameOut, mu_size NameLength, mu_int32 iNameLine, mu_boolean bSimilarRecipe)
{
	if (pRecipe == nullptr) return false;
	if (iNameLine > 2 || iNameLine < 1) return false;
	if (pRecipe->MixOption == 'C')
	{
		std::vector<mu_string> optionTextlist;

		g_pItemAddOptioninfo.GetItemAddOtioninfoText(optionTextlist, m_iFirstItemType);
		if (optionTextlist.empty() || bSimilarRecipe)
		{
			if (iNameLine == 1)
			{
				mu_sprintf_s(pszNameOut, NameLength, _T("") SPF_STRING _T(""), GlobalText[2194]);
				return true;
			}
			return false;
		}

		if (iNameLine == 1)
		{
			mu_strcpy_s(pszNameOut, NameLength, optionTextlist[0].c_str());
			return true;
		}
		else if (iNameLine == 2)
		{
			mu_strcpy_s(pszNameOut, NameLength, optionTextlist[1].c_str());
			return true;
		}
		return false;
	}
	else
	{
		if (iNameLine == 1)
		{
			if (pRecipe->MixName[1] == 0)
				mu_sprintf_s(pszNameOut, NameLength, _T("") SPF_STRING _T(""), GlobalText[pRecipe->MixName[0]]);
			else if (pRecipe->MixName[2] == 0)
				mu_sprintf_s(pszNameOut, NameLength, _T("") SPF_STRING _T(" ") SPF_STRING _T(""), GlobalText[pRecipe->MixName[0]], GlobalText[pRecipe->MixName[1]]);
			else
				mu_sprintf_s(pszNameOut, NameLength, _T("") SPF_STRING _T(" ") SPF_STRING _T(" ") SPF_STRING _T(""), GlobalText[pRecipe->MixName[0]],
					GlobalText[pRecipe->MixName[1]], GlobalText[pRecipe->MixName[2]]);
			return true;
		}
		return false;
	}
}

mu_boolean MUMixRecipe::IsChaosItem(MUMixItem & rSource)
{
	if (rSource.m_sType == ITEM_MACE + 6 || rSource.m_sType == ITEM_BOW + 6 || rSource.m_sType == ITEM_STAFF + 7) return true;
	return false;
}

mu_boolean MUMixRecipe::IsChaosJewel(MUMixItem & rSource)
{
	if (rSource.m_sType == ITEM_WING + 15) return true;
	return false;
}

mu_boolean MUMixRecipe::Is380AddedItem(MUMixItem & rSource)
{
	return rSource.m_b380AddedItem;
}

mu_boolean MUMixRecipe::IsFenrirAddedItem(MUMixItem & rSource)
{
	return rSource.m_bFenrirAddedItem;
}

mu_boolean MUMixRecipe::IsUpgradableItem(MUMixItem & rSource)
{
	return (rSource.m_bIsEquipment || rSource.m_bIsWing || rSource.m_bIsUpgradedWing || rSource.m_bIs3rdUpgradedWing);
}

mu_boolean MUMixRecipe::IsSourceOfRefiningStone(MUMixItem & rSource)
{
	if (rSource.m_iLevel < 4)
	{
		switch (rSource.m_sType)
		{
		case ITEM_SWORD + 0: case ITEM_SWORD + 1: case ITEM_SWORD + 2: case ITEM_SWORD + 4:
		case ITEM_AXE + 0: case ITEM_AXE + 1: case ITEM_AXE + 2:
		case ITEM_MACE + 0: case ITEM_MACE + 1: case ITEM_MACE + 2:
		case ITEM_SPEAR + 1: case ITEM_SPEAR + 2: case ITEM_SPEAR + 3: case ITEM_SPEAR + 5: case ITEM_SPEAR + 6:
		case ITEM_BOW + 0: case ITEM_BOW + 1: case ITEM_BOW + 2: case ITEM_BOW + 3:
		case ITEM_BOW + 8: case ITEM_BOW + 9: case ITEM_BOW + 10: case ITEM_BOW + 11:
		case ITEM_STAFF + 0: case ITEM_STAFF + 1: case ITEM_STAFF + 2:
		case ITEM_SHIELD + 0: case ITEM_SHIELD + 1: case ITEM_SHIELD + 2: case ITEM_SHIELD + 3: case ITEM_SHIELD + 4:
		case ITEM_SHIELD + 6: case ITEM_SHIELD + 7: case ITEM_SHIELD + 9: case ITEM_SHIELD + 10:
		case ITEM_HELM + 0: case ITEM_HELM + 2: case ITEM_HELM + 4: case ITEM_HELM + 5: case ITEM_HELM + 6:
		case ITEM_HELM + 7: case ITEM_HELM + 8: case ITEM_HELM + 10: case ITEM_HELM + 11: case ITEM_HELM + 12:
		case ITEM_ARMOR + 0: case ITEM_ARMOR + 2: case ITEM_ARMOR + 4: case ITEM_ARMOR + 5: case ITEM_ARMOR + 6:
		case ITEM_ARMOR + 7: case ITEM_ARMOR + 8: case ITEM_ARMOR + 10: case ITEM_ARMOR + 11: case ITEM_ARMOR + 12:
		case ITEM_PANTS + 0: case ITEM_PANTS + 2: case ITEM_PANTS + 4: case ITEM_PANTS + 5: case ITEM_PANTS + 6:
		case ITEM_PANTS + 7: case ITEM_PANTS + 8: case ITEM_PANTS + 10: case ITEM_PANTS + 11: case ITEM_PANTS + 12:
		case ITEM_GLOVES + 0: case ITEM_GLOVES + 2: case ITEM_GLOVES + 4: case ITEM_GLOVES + 5: case ITEM_GLOVES + 6:
		case ITEM_GLOVES + 7: case ITEM_GLOVES + 8: case ITEM_GLOVES + 10: case ITEM_GLOVES + 11: case ITEM_GLOVES + 12:
		case ITEM_BOOTS + 0: case ITEM_BOOTS + 2: case ITEM_BOOTS + 4: case ITEM_BOOTS + 5: case ITEM_BOOTS + 6:
		case ITEM_BOOTS + 7: case ITEM_BOOTS + 8: case ITEM_BOOTS + 10: case ITEM_BOOTS + 11: case ITEM_BOOTS + 12:
			return false;
		}
	}
	return true;
}

mu_boolean MUMixRecipe::IsCharmItem(MUMixItem & rSource)
{
	return rSource.m_bIsCharmItem;
}

mu_boolean MUMixRecipe::IsChaosCharmItem(MUMixItem & rSource)
{
	return rSource.m_bIsChaosCharmItem;
}

mu_boolean MUMixRecipe::IsJewelItem(MUMixItem & rSource)
{
	return rSource.m_bIsJewelItem;
}

mu_boolean MUMixRecipe::IsSourceOfAttachSeedSphereToWeapon(MUMixItem & rSource)
{
	if (rSource.m_sType >= ITEM_WING + 100 && rSource.m_sType <= ITEM_WING + 129)
	{
		mu_int32 iSeedSphereType = rSource.m_sType - ITEM_WING;

		if (iSeedSphereType % 2 == 0) return true;
	}
	return false;
}

mu_boolean MUMixRecipe::IsSourceOfAttachSeedSphereToArmor(MUMixItem & rSource)
{
	if (rSource.m_sType >= ITEM_WING + 100 && rSource.m_sType <= ITEM_WING + 129)
	{
		mu_int32 iSeedSphereType = rSource.m_sType - ITEM_WING;

		if (iSeedSphereType % 2 == 1) return true;
	}
	return false;
}

mu_float MUMixRecipe::MixrateAddSub()
{
	mu_float fLvalue = 0;
	while (1)
	{
		if (m_iMixRateIter >= GetCurRecipe()->NumRateData || m_pMixRates[m_iMixRateIter].op == MRCP_RP)
		{
			return fLvalue;
		}
		switch (m_pMixRates[m_iMixRateIter].op)
		{
		case MRCP_ADD:
			++m_iMixRateIter;
			fLvalue += MixrateMulDiv();
			break;
		case MRCP_SUB:
			++m_iMixRateIter;
			fLvalue -= MixrateMulDiv();
			break;
		default:
			fLvalue = MixrateMulDiv();
			break;
		};
	}
}

mu_float MUMixRecipe::MixrateMulDiv()
{
	mu_float fLvalue = 0;
	while (1)
	{
		if (m_iMixRateIter >= GetCurRecipe()->NumRateData || m_pMixRates[m_iMixRateIter].op == MRCP_RP)
		{
			return fLvalue;
		}
		switch (m_pMixRates[m_iMixRateIter].op)
		{
		case MRCP_ADD:
		case MRCP_SUB:
			return fLvalue;
		case MRCP_MUL:
			++m_iMixRateIter;
			fLvalue *= MixrateFactor();
			break;
		case MRCP_DIV:
			++m_iMixRateIter;
			fLvalue /= MixrateFactor();
			break;
		default:
			fLvalue = MixrateFactor();
			break;
		};
	}
}

mu_float MUMixRecipe::MixrateFactor()
{
	mu_float fValue = 0;
	switch (m_pMixRates[m_iMixRateIter].op)
	{
	default: break;

	case MRCP_LP:
		++m_iMixRateIter;
		fValue = MixrateAddSub();
		break;
	case MRCP_INT:
		++m_iMixRateIter;
		++m_iMixRateIter;
		fValue = mu_int32(MixrateAddSub());
		break;
	case MRCP_NUMBER:
		fValue = (mu_float)m_pMixRates[m_iMixRateIter].value;
		break;
	case MRCP_MAXRATE:
		fValue = GetCurRecipe()->SuccessRate;
		break;
	case MRCP_ITEM:
		fValue = m_dwTotalItemValue;
		break;
	case MRCP_WING:	
		fValue = m_dwWingItemValue;
		break;
	case MRCP_EXCELLENT:
		fValue = m_dwExcellentItemValue;
		break;
	case MRCP_EQUIP:
		fValue = m_dwEquipmentItemValue;
		break;
	case MRCP_SET:
		fValue = m_dwSetItemValue;
		break;
	case MRCP_LUCKOPT:
		if (m_bFindMixLuckItem) fValue = 25;
		else fValue = 0;
		break;
	case MRCP_LEVEL1:
		fValue = m_iFirstItemLevel;
		break;
	case MRCP_NONJEWELITEM:
		fValue = m_dwTotalNonJewelItemValue;
		break;
	}
	++m_iMixRateIter;
	return fValue;
}