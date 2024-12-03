#ifndef __MU_MIXMANAGER_H__
#define __MU_MIXMANAGER_H__

#pragma once

enum MIX_TYPES
{
	MIXTYPE_GOBLIN_NORMAL,
	MIXTYPE_GOBLIN_CHAOSITEM,
	MIXTYPE_GOBLIN_ADD380,
	MIXTYPE_CASTLE_SENIOR,
	MIXTYPE_TRAINER,
	MIXTYPE_OSBOURNE,
	MIXTYPE_JERRIDON,
	MIXTYPE_ELPIS,
	MIXTYPE_CHAOS_CARD,
	MIXTYPE_CHERRYBLOSSOM,
	MIXTYPE_EXTRACT_SEED,
	MIXTYPE_SEED_SPHERE,
	MIXTYPE_ATTACH_SOCKET,
	MIXTYPE_DETACH_SOCKET,
	MAX_MIX_TYPES
};

#define MAX_MIX_SOURCES 8
#define MAX_MIX_NAMES 3
#define MAX_MIX_DESCRIPTIONS 3
#define MAX_MIX_RATE_TOKEN 32

enum MIXRATE_OPS
{
	MRCP_NUMBER = 0,
	MRCP_ADD,
	MRCP_SUB,
	MRCP_MUL,
	MRCP_DIV,
	MRCP_LP,
	MRCP_RP,
	MRCP_INT,

	MRCP_MAXRATE = 32,
	MRCP_ITEM,
	MRCP_WING,
	MRCP_EXCELLENT,
	MRCP_EQUIP,
	MRCP_SET,
	MRCP_LEVEL1,
	MRCP_NONJEWELITEM,

	MRCP_LUCKOPT = 64,
};

enum _SPECIAL_ITEM_RECIPE_
{
	RCP_SP_EXCELLENT = 1,
	RCP_SP_ADD380ITEM = 2,
	RCP_SP_SETITEM = 4,
	RCP_SP_HARMONY = 8,
	RCP_SP_SOCKETITEM = 16,
};

enum _MIX_SOURCE_STATUS
{
	MIX_SOURCE_ERROR,
	MIX_SOURCE_NO,
	MIX_SOURCE_PARTIALLY,
	MIX_SOURCE_YES,
};

struct MUMixRateToken
{
	MIXRATE_OPS op;
	mu_float value;
};

struct MUMixRecipeItem
{
	mu_int16 TypeMin;
	mu_int16 TypeMax;
	mu_int32 LevelMin;
	mu_int32 LevelMax;
	mu_int32 OptionMin;
	mu_int32 OptionMax;
	mu_int32 DurabilityMin;
	mu_int32 DurabilityMax;
	mu_int32 CountMin;
	mu_int32 CountMax;
	mu_uint32 SpecialItem;
};

struct MUMixRecipeData
{
	mu_int32 MixIndex;
	mu_int32 MixID;
	mu_int32 MixName[MAX_MIX_NAMES];
	mu_int32 MixDesc[MAX_MIX_DESCRIPTIONS];
	mu_int32 MixAdvice[MAX_MIX_DESCRIPTIONS];
	mu_int32 Width;
	mu_int32 Height;
	mu_int32 RequiredLevel;
	mu_uint8 RequiredZenType;
	mu_uint32 RequiredZen;
	mu_int32 NumRateData;
	MUMixRateToken RateToken[MAX_MIX_RATE_TOKEN];
	mu_int32 SuccessRate;
	mu_uint8 MixOption;
	mu_uint8 CharmOption;
	mu_uint8 ChaosCharmOption;
	MUMixRecipeItem MixSources[MAX_MIX_SOURCES];
	mu_int32 NumMixSoruces;
};

class MUMixItem
{
public:
	MUMixItem();
	MUMixItem(ITEM *Item, mu_uint32 MixValue);
	~MUMixItem();

	void Reset();
	void SetItem(ITEM *Item, mu_uint32 MixValue);

public:
	EINLINE mu_boolean IsSameItem(const MUMixItem rhs)
	{
		return (m_sType == rhs.m_sType && m_iLevel == rhs.m_iLevel &&
			(m_bCanStack || m_iDurability == rhs.m_iDurability) && m_iOption == rhs.m_iOption &&
			m_dwSpecialItem == rhs.m_dwSpecialItem);
	}

	EINLINE mu_boolean operator==(ITEM *rhs)
	{
		return IsSameItem(MUMixItem(rhs, 0));
	}

public:
	mu_int16 m_sType;
	mu_int32 m_iLevel;
	mu_int32 m_iOption;
	mu_int32 m_iDurability;
	mu_uint32 m_dwSpecialItem;
	mu_int32 m_iCount;
	mu_int32 m_iTestCount;

	mu_boolean m_bMixLuck;
	mu_boolean m_bIsEquipment;
	mu_boolean m_bCanStack;
	mu_boolean m_bIsWing;
	mu_boolean m_bIsUpgradedWing;
	mu_boolean m_bIs3rdUpgradedWing;
	mu_uint32 m_dwMixValue;
	mu_boolean m_bIsCharmItem;
	mu_boolean m_bIsChaosCharmItem;
	mu_boolean m_bIsJewelItem;

	mu_boolean m_b380AddedItem;
	mu_boolean m_bFenrirAddedItem;
	mu_uint16 m_wHarmonyOption;
	mu_uint16 m_wHarmonyOptionLevel;

	mu_uint8 m_bySocketCount;
	mu_uint8 m_bySocketSeedID[MAX_SOCKETS];
	mu_uint8 m_bySocketSphereLv[MAX_SOCKETS];
	mu_uint8 m_bySeedSphereID;
};

class MUMixInventory
{
public:
	MUMixInventory();
	~MUMixInventory();

	void Reset();

	mu_int32 AddItem(ITEM *pItem);
	mu_int32 GetNumMixItems();
	MUMixItem *GetMixItems();

	mu_uint32 EvaluateMixItemValue(ITEM *pItem);

private:
	MUMixItem m_MixItems[MAX_MIX_INVENTORY];
	mu_int32 m_iNumMixItems;
};

class MUMixRecipe
{
public:
	MUMixRecipe();
	~MUMixRecipe();

	void Reset();
	void SetRecipes(MUMixRecipeData *Recipes, mu_int32 RecipesCount);
	void ClearCheckRecipeResult();
	mu_int32 CheckRecipe(mu_int32 iNumMixItems, MUMixItem * pMixItems);
	mu_int32 CheckRecipeSimilarity(mu_int32 iNumMixItems, MUMixItem * pMixItems);

	mu_boolean IsMixSource(ITEM * pItem);
	MUMixRecipeData *GetCurRecipe();
	mu_int32 GetCurMixID();
	mu_boolean GetCurRecipeName(mu_text * pszNameOut, mu_size NameLength, mu_int32 iNameLine);
	mu_boolean GetCurRecipeDesc(mu_text * pszDescOut, mu_size DescLength, mu_int32 iDescLine);
	MUMixRecipeData *GetMostSimilarRecipe();
	mu_boolean GetMostSimilarRecipeName(mu_text * pszNameOut, mu_size NameLength, mu_int32 iNameLine);
	mu_boolean GetRecipeAdvice(mu_text * pszAdviceOut, mu_size AdviceLength, mu_int32 iAdivceLine);
	mu_int32 GetSourceName(mu_int32 iItemNum, mu_text * pszNameOut, mu_size NameLength, mu_int32 iNumMixItems, MUMixItem * pMixItems);

	void CalcCharmBonusRate(mu_int32 iNumMixItems, MUMixItem * pMixItems);
	void CalcChaosCharmCount(mu_int32 iNumMixItems, MUMixItem * pMixItems);

protected:
	mu_boolean CheckRecipeSub(MUMixRecipeData *Recipe, mu_int32 iNumMixItems, MUMixItem * pMixItems);
	mu_int32 CheckRecipeSimilaritySub(MUMixRecipeData *Recipe, mu_int32 iNumMixItems, MUMixItem * pMixItems);
	bool CheckItem(MUMixRecipeItem & rItem, MUMixItem & rSource);
	void EvaluateMixItems(mu_int32 iNumMixItems, MUMixItem * pMixItems);
	void CalcMixRate(mu_int32 iNumMixItems, MUMixItem * pMixItems);
	void CalcMixReqZen(mu_int32 iNumMixItems, MUMixItem * pMixItems);
	mu_boolean GetRecipeName(MUMixRecipeData * pRecipe, mu_text * pszNameOut, mu_size NameLength, mu_int32 iNameLine, mu_boolean bSimilarRecipe);

	mu_boolean IsChaosItem(MUMixItem & rSource);
	mu_boolean IsChaosJewel(MUMixItem & rSource);
	mu_boolean Is380AddedItem(MUMixItem & rSource);
	mu_boolean IsFenrirAddedItem(MUMixItem & rSource);
	mu_boolean IsUpgradableItem(MUMixItem & rSource);
	mu_boolean IsSourceOfRefiningStone(MUMixItem & rSource);
	mu_boolean IsCharmItem(MUMixItem & rSource);
	mu_boolean IsChaosCharmItem(MUMixItem & rSource);
	mu_boolean IsJewelItem(MUMixItem & rSource);
	mu_boolean IsSourceOfAttachSeedSphereToWeapon(MUMixItem & rSource);
	mu_boolean IsSourceOfAttachSeedSphereToArmor(MUMixItem & rSource);

	mu_float MixrateAddSub();
	mu_float MixrateMulDiv();
	mu_float MixrateFactor();

public:
	EINLINE mu_boolean IsReadyToMix()
	{
		return (m_iCurMixIndex > 0);
	}
	EINLINE mu_int32 GetSuccessRate()
	{
		return m_iSuccessRate;
	}
	EINLINE mu_int64 GetReqiredZen()
	{
		return m_dwRequiredZen;
	}
	EINLINE mu_int32 GetFirstItemSocketCount()
	{
		return m_byFirstItemSocketCount;
	}
	EINLINE mu_int32 GetFirstItemSocketSeedID(mu_int32 iIndex)
	{
		if (iIndex >= m_byFirstItemSocketCount)
			return SOCKET_EMPTY;
		else
			return m_byFirstItemSocketSeedID[iIndex];
	}
	EINLINE mu_int32 GetFirstItemSocketShpereLv(mu_int32 iIndex)
	{
		if (iIndex >= m_byFirstItemSocketCount)
			return 0;
		else
			return m_byFirstItemSocketSphereLv[iIndex];
	}

	EINLINE bool IsOptionItem(MUMixRecipeItem & rItem)
	{
		return (rItem.CountMin == 0);
	}

protected:
	mu_int32 m_RecipesCount;
	MUMixRecipeData *m_Recipes;
	mu_int32 m_iCurMixIndex;
	mu_int32 m_iMostSimilarMixIndex;

	mu_int32 m_iSuccessRate;
	mu_int64 m_dwRequiredZen;

	mu_boolean m_bFindMixLuckItem;
	mu_uint32 m_dwTotalItemValue;
	mu_uint32 m_dwExcellentItemValue;
	mu_uint32 m_dwEquipmentItemValue;
	mu_uint32 m_dwWingItemValue;
	mu_uint32 m_dwSetItemValue;
	mu_uint32 m_iFirstItemLevel;
	mu_int32 m_iFirstItemType;
	mu_uint32 m_dwTotalNonJewelItemValue;

	mu_uint8 m_byFirstItemSocketCount;
	mu_uint8 m_byFirstItemSocketSeedID[MAX_SOCKETS];
	mu_uint8 m_byFirstItemSocketSphereLv[MAX_SOCKETS];

	mu_uint16 m_wTotalCharmBonus;
	mu_uint16 m_wTotalChaosCharmCount;

	mu_int32 m_iMixSourceTest[MAX_MIX_SOURCES];
	mu_int32 m_iMostSimilarMixSourceTest[MAX_MIX_SOURCES];

	mu_int32 m_iMixRateIter;
	MUMixRateToken *m_pMixRates;
};

class MUMixManager
{
public:
	MUMixManager();
	~MUMixManager();

	mu_boolean Load(const mu_text *filename);

	void ResetMixItemInventory();
	void AddItemToMixItemInventory(ITEM * pItem);
	void CheckMixInventory();

	mu_int32 GetSeedSphereID(mu_int32 iOrder);

public:
	EINLINE void SetMixType(mu_int32 mixType)
	{
		_MixType = mixType;
		_Recipe.SetRecipes(_Recipes[mixType].data(), _Recipes[mixType].size());
	}

	EINLINE mu_int32 GetMixType()
	{
		return _MixType;
	}

	EINLINE void SetMixSubType(mu_int32 MixSubType)
	{
		_MixSubType = MixSubType;
	}

	EINLINE mu_int32 GetMixSubType()
	{
		return _MixSubType;
	}

	EINLINE mu_int32 GetMixInventoryEquipmentIndex()
	{
		switch (GetMixType())
		{
		case MIXTYPE_GOBLIN_NORMAL:
		case MIXTYPE_GOBLIN_CHAOSITEM:
		case MIXTYPE_GOBLIN_ADD380:
			return REQUEST_EQUIPMENT_CHAOS_MIX;
		case MIXTYPE_CASTLE_SENIOR:
			return REQUEST_EQUIPMENT_CHAOS_MIX;
		case MIXTYPE_TRAINER:
			return REQUEST_EQUIPMENT_TRAINER_MIX;
		case MIXTYPE_OSBOURNE:
			return REQUEST_EQUIPMENT_OSBOURNE_MIX;
		case MIXTYPE_JERRIDON:
			return REQUEST_EQUIPMENT_JERRIDON_MIX;
		case MIXTYPE_ELPIS:
			return REQUEST_EQUIPMENT_ELPIS_MIX;
		case MIXTYPE_CHAOS_CARD:
			return REQUEST_EQUIPMENT_CHAOS_CARD_MIX;
		case MIXTYPE_CHERRYBLOSSOM:
			return REQUEST_EQUIPMENT_CHERRYBLOSSOM_MIX;
		case MIXTYPE_EXTRACT_SEED:
			return REQUEST_EQUIPMENT_EXTRACT_SEED_MIX;
		case MIXTYPE_SEED_SPHERE:
			return REQUEST_EQUIPMENT_SEED_SPHERE_MIX;
		case MIXTYPE_ATTACH_SOCKET:
			return REQUEST_EQUIPMENT_ATTACH_SOCKET_MIX;
		case MIXTYPE_DETACH_SOCKET:
			return REQUEST_EQUIPMENT_DETACH_SOCKET_MIX;
		default:
			return REQUEST_EQUIPMENT_CHAOS_MIX;
		}
	}

	EINLINE void SetPlusChaosRate(mu_uint8 plusRate)
	{
		_PlusChaosRate = plusRate;
	}

	EINLINE mu_uint8 GetPlusChaosRate()
	{
		return _PlusChaosRate;
	}

	EINLINE mu_boolean IsReadyToMix()
	{
		return _Recipe.IsReadyToMix();
	}

	EINLINE mu_boolean IsMixInit()
	{
		return _IsMixInit;
	}

	EINLINE mu_boolean IsMixSource(ITEM * pItem)
	{
		_Recipe.CalcCharmBonusRate(_MixItemInventory.GetNumMixItems(), _MixItemInventory.GetMixItems());
		_Recipe.CalcChaosCharmCount(_MixItemInventory.GetNumMixItems(), _MixItemInventory.GetMixItems());
		return _Recipe.IsMixSource(pItem);
	}

	EINLINE mu_int32 CheckRecipe(mu_int32 iNumMixItems, MUMixItem *pMixItems)
	{
		return _Recipe.CheckRecipe(iNumMixItems, pMixItems);
	}

	EINLINE mu_int32 CheckRecipeSimilarity(mu_int32 iNumMixItems, MUMixItem *pMixItems)
	{
		return _Recipe.CheckRecipeSimilarity(iNumMixItems, pMixItems);
	}

	EINLINE MUMixRecipeData * GetCurRecipe()
	{
		return _Recipe.GetCurRecipe();
	}

	EINLINE mu_int32 GetSuccessRate()
	{
		return _Recipe.GetSuccessRate();
	}

	EINLINE mu_int64 GetReqiredZen()
	{
		return _Recipe.GetReqiredZen();
	}

	EINLINE mu_int32 GetCurMixID()
	{
		return _Recipe.GetCurMixID();
	}

	EINLINE mu_boolean GetCurRecipeName(mu_text * pszNameOut, mu_size NameLength, mu_int32 iNameLine)
	{
		return _Recipe.GetCurRecipeName(pszNameOut, NameLength, iNameLine);
	}

	EINLINE mu_boolean GetCurRecipeDesc(mu_text * pszDescOut, mu_size DescLength, mu_int32 iDescLine)
	{
		return _Recipe.GetCurRecipeDesc(pszDescOut, DescLength, iDescLine);
	}

	EINLINE MUMixRecipeData *GetMostSimilarRecipe()
	{
		return _Recipe.GetMostSimilarRecipe();
	}

	EINLINE mu_boolean GetMostSimilarRecipeName(mu_text * pszNameOut, mu_size NameLength, mu_int32 iNameLine)
	{
		return _Recipe.GetMostSimilarRecipeName(pszNameOut, NameLength, iNameLine);
	}

	EINLINE mu_boolean GetRecipeAdvice(mu_text * pszAdviceOut, mu_size AdviceLength, mu_int32 iAdivceLine)
	{
		return _Recipe.GetRecipeAdvice(pszAdviceOut, AdviceLength, iAdivceLine);
	}

	EINLINE mu_int32 GetSourceName(mu_int32 iItemNum, mu_text * pszNameOut, mu_size NameLength)
	{
		return _Recipe.GetSourceName(iItemNum, pszNameOut, NameLength, _MixItemInventory.GetNumMixItems(), _MixItemInventory.GetMixItems());
	}

	EINLINE void ClearCheckRecipeResult()
	{
		_Recipe.ClearCheckRecipeResult();
	}

	EINLINE mu_int32 GetFirstItemSocketCount()
	{
		return _Recipe.GetFirstItemSocketCount();
	}

	EINLINE mu_int32 GetFirstItemSocketSeedID(mu_int32 iIndex)
	{
		if (iIndex >= GetFirstItemSocketCount())
			return SOCKET_EMPTY;
		else
			return _Recipe.GetFirstItemSocketSeedID(iIndex);
	}

	EINLINE mu_int32 GetFirstItemSocketShpereLv(mu_int32 iIndex)
	{
		if (iIndex >= GetFirstItemSocketCount())
			return 0;
		else
			return _Recipe.GetFirstItemSocketShpereLv(iIndex);
	}

private:
	std::vector<MUMixRecipeData> _Recipes[MAX_MIX_TYPES];
	MUMixRecipe _Recipe;
	MUMixInventory _MixItemInventory;
	mu_int32 _MixType;
	mu_int32 _MixSubType;
	mu_boolean _IsMixInit;
	mu_uint8 _PlusChaosRate;
};

extern MUMixManager g_MixManager;

#endif