#ifndef __MU_SETITEMOPTION_H__
#define __MU_SETITEMOPTION_H__

#pragma once

class ITEM;

const mu_uint8 MAX_SET_OPTION = 64;
const mu_uint8 MASTERY_OPTION = 24;

const mu_uint8 EXT_A_SET_OPTION = 1;
const mu_uint8 EXT_B_SET_OPTION = 2;

typedef	struct
{
	mu_uint8	byOption[2];
	mu_uint8	byMixItemLevel[2];
}ITEM_SET_TYPE;

typedef struct
{
	mu_text		strSetName[64];
	mu_uint8	byStandardOption[6][2];
	mu_uint8	byStandardOptionValue[6][2];
	mu_uint8	byExtOption[2];
	mu_uint8	byExtOptionValue[2];
	mu_uint8	byOptionCount;
	mu_uint8	byFullOption[5];
	mu_uint8	byFullOptionValue[5];
	mu_uint8	byRequireClass[GAME::MAX_CLASS];
}ITEM_SET_OPTION;

class CSItemOption
{
private:
	ITEM_SET_TYPE	m_ItemSetType[MAX_ITEM];
	ITEM_SET_OPTION	m_ItemSetOption[MAX_SET_OPTION];

	mu_boolean	m_bViewOptionList;

	mu_uint8    m_byRenderOptionList;
	mu_uint8    m_bySelectedItemOption;

	mu_uint8    m_bySameSetItem;
	mu_text		m_strSetName[2][64];

	typedef std::map<mu_int32, mu_string> MAP_EQUIPPEDSETITEMNAME;
	typedef std::map<mu_uint8, mu_int32> MAP_EQUIPPEDSETITEM_SEQUENCE;

	MAP_EQUIPPEDSETITEMNAME m_mapEquippedSetItemName;
	MAP_EQUIPPEDSETITEMNAME::iterator m_iterESIN;

	MAP_EQUIPPEDSETITEM_SEQUENCE m_mapEquippedSetItemSequence;
	MAP_EQUIPPEDSETITEM_SEQUENCE::iterator m_iterESIS;

private:
	mu_int32 Search_From_EquippedSetItemNameMapTable(mu_text *szSetItemname)
	{
		mu_int32 iSizeFindName = mu_strlen(szSetItemname);

		for (m_iterESIN = m_mapEquippedSetItemName.begin(); m_iterESIN != m_mapEquippedSetItemName.end(); ++m_iterESIN)
		{
			mu_string	strCur;

			strCur = m_iterESIN->second;
			mu_int32 iSizeCurName = strCur.size();

			if (!mu_strncmp(szSetItemname, strCur.c_str(), iSizeFindName)
				&& !mu_strncmp(szSetItemname, strCur.c_str(), iSizeCurName))
			{
				return m_iterESIN->first;
			}
		}

		return -1;
	}

public:
	mu_boolean	Find_From_EquippedSetItemNameMapTable(mu_text *szSetItemname)
	{
		mu_int32 iResult = Search_From_EquippedSetItemNameMapTable(szSetItemname);

		if (-1 == iResult)
		{
			return false;
		}
		else
		{
			return true;
		}
	}

#ifndef BYTE_MAX
#define	BYTE_MAX 0xFF
#endif

	mu_uint8 Search_From_EquippedSetItemNameSequence(mu_text *szSetItemname)
	{
		mu_int32 iResult = Search_From_EquippedSetItemNameMapTable(szSetItemname);

		if (-1 != iResult)
		{
			for (m_iterESIS = m_mapEquippedSetItemSequence.begin(); m_iterESIS != m_mapEquippedSetItemSequence.end(); ++m_iterESIS)
			{
				if ((mu_int32)(m_iterESIS->second) == iResult)
				{
					return (mu_uint8)m_iterESIS->first;
				}
			}
		}

		return (mu_uint8)BYTE_MAX;
	}

	mu_uint8	m_bySetOptionANum;
	mu_uint8	m_bySetOptionBNum;
	mu_uint8    m_bySetOptionIndex[2];
	mu_uint8	m_bySetOptionList[16][3];
	mu_uint8	m_bySetOptionListOnOff[16];
	mu_int32    m_iSetOptionListValue[16][2];

	mu_boolean OpenItemSetType(const mu_text *filename);
	mu_boolean OpenItemSetOption(const mu_text *filename);

	void checkItemType(mu_uint8* optioList, const mu_int32 iType, const mu_int32 setType);

	void calcSetOptionList(mu_uint8* optioList);

	void getExplainText(mu_text* text, const mu_size textLength, const mu_uint8 option, const mu_uint8 value, const mu_uint8 SetA);

	void getAllAddState(mu_uint16* Strength, mu_uint16* Dexterity, mu_uint16* Energy, mu_uint16* Vitality, mu_uint16* Charisma);

	mu_int32 GetSetItmeCount(const ITEM* pselecteditem);
	mu_boolean isFullseteffect(const ITEM* ip);

public:
	void getAllAddOptionStatesbyCompare(mu_uint16* Strength, mu_uint16* Dexterity, mu_uint16* Energy, mu_uint16* Vitality, mu_uint16* Charisma,
		mu_uint16 iCompareStrength, mu_uint16 iCompareDexterity, mu_uint16 iCompareEnergy, mu_uint16 iCompareVitality, mu_uint16 iC);

	void	getAllAddStateOnlyAddValue(mu_uint16* AddStrength,
		mu_uint16* AddDexterity,
		mu_uint16* AddEnergy,
		mu_uint16* AddVitality,
		mu_uint16* AddCharisma);

public:
	CSItemOption() { init(); };
	~CSItemOption() {};

	void init(void)
	{
		mu_zeromem(m_strSetName, sizeof(m_strSetName));
		m_bViewOptionList = false;
		m_byRenderOptionList = 0;
		m_bySelectedItemOption = 0;
		m_bySameSetItem = 0;
	}

	void ClearListOnOff(void)
	{
		mu_zeromem(m_bySetOptionListOnOff, sizeof(m_bySetOptionListOnOff));
	}

	mu_boolean OpenItemSetScript();

	mu_boolean IsDisableSkill(mu_int32 Type, mu_int32 Energy, mu_int32 Charisma = 0);
	mu_uint8 IsChangeSetItem(const mu_int32 Type, const mu_int32 SubType);
	mu_uint16 GetMixItemLevel(const mu_int32 Type);
	mu_boolean GetSetItemName(mu_text* strName, const mu_int32 iType, const mu_int32 setType);

	void PlusSpecial(mu_uint16* Value, const mu_int32 Special);
	void PlusSpecialPercent(mu_uint16* Value, const mu_int32 Special);
	void PlusSpecialLevel(mu_uint16* Value, const mu_uint16 SrcValue, const mu_int32 Special);
	void PlusMastery(mu_int32* Value, const mu_uint8 MasteryType);

	void MinusSpecialPercent(mu_int32* Value, const mu_int32 Special);

	void GetSpecial(mu_uint16* Value, mu_int32 Special);

	void GetSpecialPercent(mu_uint16* Value, mu_int32 Special);
	void GetSpecialLevel(mu_uint16* Value, const mu_uint16 SrcValue, mu_int32 Special);
	mu_int32 GetDefaultOptionValue(ITEM* ip, mu_uint16* Value);
	mu_boolean GetDefaultOptionText(const ITEM* ip, mu_text* Text, const mu_size TextLength);
	mu_int32 RenderDefaultOptionText(const ITEM* ip, mu_int32 TextNum);

	mu_boolean Special_Option_Check(mu_int32 Kind = 0);

	void CheckItemSetOptions();
	void RenderSetOptionList(mu_float StartX, mu_float StartY);

	mu_int32 m_arLimitSetItemOptionCount[GAME::MAX_EQUIPMENT];

	void UpdateCount_SetOptionPerEquippedSetItem(const mu_uint8* byOptionList,
		mu_int32* arLimitSetItemOptionCount,
		ITEM* ItemsEquipment);
	mu_int32 GetCurrentTypeSetitemCount(const ITEM &CurItem_, const mu_uint8* byOptionList);

	mu_int32 RenderSetOptionListInItem(const ITEM* ip, mu_int32 TextNum, mu_boolean bIsEquippedItem = false);

	void ClearOptionHelper() { m_byRenderOptionList = 0; }
	void CheckRenderOptionHelper(const mu_text* FilterName);
	void RenderOptionHelper();

	mu_uint8 GetSetOptionANum();
	mu_uint8 GetSetOptionBNum();

	void SetViewOptionList(mu_boolean bView);
	mu_boolean IsViewOptionList();
};

extern CSItemOption g_csItemOption;

#endif