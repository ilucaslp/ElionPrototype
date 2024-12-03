#include "stdafx.h"
#include "mu_buffstatevaluecontrol.h"

BuffStateValueControl::BuffStateValueControl()
{
	Initialize();
}

BuffStateValueControl::~BuffStateValueControl()
{
	Destroy();
}

void BuffStateValueControl::Initialize()
{
	for (mu_int32 i = eBuff_Attack; i < eBuff_Count; ++i)
	{
		CheckValue(static_cast<eBuffState>(i));
	}
}

void BuffStateValueControl::Destroy()
{

}

eBuffValueLoadType BuffStateValueControl::CheckValue(eBuffState bufftype)
{
	if (bufftype >= eBuff_Attack && bufftype <= eBuff_GMEffect)
	{
		return eBuffValueLoad_None;
	}
	else
	{
		return eBuffValueLoad_ItemAddOption;
	}
}

void BuffStateValueControl::SetValue(eBuffState bufftype, BuffStateValueInfo& valueinfo)
{
	eBuffValueLoadType loadtype = CheckValue(bufftype);

	if (eBuffValueLoad_ItemAddOption == loadtype)
	{
		const BuffInfo buffinfo = g_BuffInfo(bufftype);
		const ITEM_ADD_OPTION& Item_data = g_pItemAddOptioninfo.GetItemAddOtioninfo(ITEMINDEX(buffinfo.s_ItemType, buffinfo.s_ItemIndex));

		valueinfo.s_Value1 = Item_data.m_byValue1;
		valueinfo.s_Value2 = Item_data.m_byValue2;
		valueinfo.s_Value2 = Item_data.m_Time;
	}
	else
	{
		valueinfo.s_Value1 = 0;
		valueinfo.s_Value2 = 0;
		valueinfo.s_Value2 = 0;
	}
}

const BuffStateValueControl::BuffStateValueInfo BuffStateValueControl::GetValue(eBuffState bufftype)
{
	BuffStateValueInfo tempvalueinfo;

	BuffStateValueMap::iterator iter = m_BuffStateValue.find(bufftype);

	if (iter == m_BuffStateValue.end())
	{
		const BuffInfo buffinfo = g_BuffInfo(bufftype);

		if (buffinfo.s_ItemType != 255)
		{
			SetValue(bufftype, tempvalueinfo);
		}

		m_BuffStateValue.insert(std::make_pair(bufftype, tempvalueinfo));
	}
	else
	{
		tempvalueinfo = (*iter).second;
	}

	return tempvalueinfo;
}

void BuffStateValueControl::GetBuffInfoString(std::list<mu_string>& outstr, eBuffState bufftype)
{
	BuffStateValueInfo tempvalueinfo;
	tempvalueinfo = GetValue(bufftype);

	const BuffInfo buffinfo = g_BuffInfo(bufftype);

	outstr = buffinfo.s_BuffDescriptlist;
	outstr.push_front(_T("\n"));
	outstr.push_front(buffinfo.s_BuffName);
}

void BuffStateValueControl::GetBuffValueString(mu_string& outstr, eBuffState bufftype)
{
	BuffStateValueInfo tempvalueinfo;
	tempvalueinfo = GetValue(bufftype);

	mu_text FormatBuffer[16] = { 0 };

	if (tempvalueinfo.s_Value1 != 0)
	{
		mu_sprintf_s(FormatBuffer, mu_countof(FormatBuffer), _T("+%d"), tempvalueinfo.s_Value1);
	}

	outstr = FormatBuffer;
}