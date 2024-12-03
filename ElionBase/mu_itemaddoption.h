#ifndef __MU_ITEMADDOPTION_H__
#define __MU_ITEMADDOPTION_H__

#pragma once

typedef struct _ITEM_ADD_OPTION
{
	mu_uint8	m_byOption1;
	mu_uint16	m_byValue1;

	mu_uint8	m_byOption2;
	mu_uint16	m_byValue2;

	mu_uint8	m_Type;
	mu_uint32	m_Time;
} ITEM_ADD_OPTION;

typedef struct _ITEM_ADD_OPTION_RESULT
{
	mu_int32				m_Width;
	mu_int32				m_Height;
	mu_int32				m_Count;
	mu_int32				m_Type;

	_ITEM_ADD_OPTION_RESULT()
	{
		m_Width = 0;
		m_Height = 0;
		m_Count = 0;
		m_Type = 0;
	}

	mu_boolean IsSuccess()
	{
		if (m_Width * m_Height == m_Count && m_Count != 0)
		{
			return true;
		}
		return false;
	}
} ITEM_ADD_OPTION_RESULT;

class ItemAddOptioninfo
{
public:
	ItemAddOptioninfo();
	~ItemAddOptioninfo();

	const mu_boolean OpenItemAddOptionInfoFile(const mu_text *filename);

public:
	void GetItemAddOtioninfoText(std::vector<mu_string>& outtextlist, mu_int32 type);

public://inline
	const ITEM_ADD_OPTION& GetItemAddOtioninfo(mu_int32 type);

private:
	ITEM_ADD_OPTION m_ItemAddOption[MAX_ITEM];
};

inline
const ITEM_ADD_OPTION& ItemAddOptioninfo::GetItemAddOtioninfo(mu_int32 type)
{
	return m_ItemAddOption[type];
}

extern ItemAddOptioninfo g_pItemAddOptioninfo;

#endif