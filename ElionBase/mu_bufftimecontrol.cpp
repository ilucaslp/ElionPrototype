#include "stdafx.h"
#include "mu_bufftimecontrol.h"

BuffTimeControl::BuffTimeControl()
{

}

BuffTimeControl::~BuffTimeControl()
{
	for (BuffTimeInfoMap::iterator iter = m_BuffTimeList.begin(); iter != m_BuffTimeList.end(); )
	{
		BuffTimeInfoMap::iterator tempiter = iter;
		++iter;

		m_BuffTimeList.erase(tempiter);
	}

	m_BuffTimeList.clear();
}

eBuffTimeType BuffTimeControl::CheckBuffTimeType(eBuffState bufftype)
{
	if (g_IsBuffClass(bufftype) == eBuffClass_Count)
	{
		return eBuffTime_None;
	}

	BuffInfo bInfo = g_BuffInfo(bufftype);

	return eBuffTimeType(1005 + bInfo.s_BuffEffectType);
}

mu_uint32 BuffTimeControl::GetBuffEventTime(eBuffTimeType bufftimetype)
{
	return 1000;
}

mu_uint32 BuffTimeControl::GetBuffMaxTime(eBuffState bufftype, mu_uint32 curbufftime)
{
	if (curbufftime == 0)
	{
		const BuffInfo& buffinfo = g_BuffInfo(bufftype);

		if (buffinfo.s_ItemType == 255)
		{
			return -1;
		}
		else
		{
			const ITEM_ADD_OPTION& Item_data = g_pItemAddOptioninfo.GetItemAddOtioninfo(ITEMINDEX(buffinfo.s_ItemType, buffinfo.s_ItemIndex));

			if (Item_data.m_Time == 0) return -1;

			return Item_data.m_Time;
		}
	}

	return curbufftime;
}

mu_boolean BuffTimeControl::IsBuffTime(eBuffTimeType bufftype)
{
	BuffTimeInfoMap::iterator iter = m_BuffTimeList.find(bufftype);

	if (iter == m_BuffTimeList.end())
	{
		return false;
	}
	else
	{
		return true;
	}
}

void BuffTimeControl::RegisterBuffTime(eBuffState bufftype, mu_uint32 curbufftime)
{
	eBuffTimeType  bufftimetype = CheckBuffTimeType(bufftype);

	curbufftime = GetBuffMaxTime(bufftype, curbufftime);

	if (bufftimetype == eBuffTime_None || curbufftime == -1)
	{
		return;
	}

	auto iter = m_BuffTimeList.find(bufftimetype);

	if (iter != m_BuffTimeList.end())
	{
		iter->second.s_EndBuffTime = g_CurrentTime + curbufftime * 1000;
	}
	else
	{
		BuffTimeInfo  buffinfo;
		buffinfo.s_BuffType = bufftype;
		buffinfo.s_EndBuffTime = g_CurrentTime + curbufftime * 1000;

		m_BuffTimeList.insert(std::make_pair(bufftimetype, buffinfo));
	}
}

mu_boolean BuffTimeControl::UnRegisterBuffTime(eBuffState bufftype)
{
	eBuffTimeType  bufftimetype = CheckBuffTimeType(bufftype);

	BuffTimeInfoMap::iterator iter = m_BuffTimeList.find(bufftimetype);

	if (iter != m_BuffTimeList.end())
	{
		m_BuffTimeList.erase(iter);
		return true;
	}

	return false;
}

void BuffTimeControl::GetBuffStringTime(eBuffState bufftype, mu_string& timeText)
{
	for (BuffTimeInfoMap::iterator iter = m_BuffTimeList.begin(); iter != m_BuffTimeList.end(); ++iter)
	{
		BuffTimeInfo& bufftimeinfo = (*iter).second;

		if (bufftimeinfo.s_BuffType == bufftype)
		{
			mu_float fTime = 0.0f;
			if (bufftimeinfo.s_EndBuffTime < g_CurrentTime)
			{
				fTime = (mu_float)(bufftimeinfo.s_EndBuffTime - g_CurrentTime) * 0.001f;
			}
			GetStringTime(fTime, timeText, true);
		}
	}
}

void BuffTimeControl::GetBuffStringTime(mu_uint32 type, mu_string& timeText, mu_boolean issecond)
{
	BuffTimeInfoMap::iterator iter = m_BuffTimeList.find(type);

	if (iter != m_BuffTimeList.end())
	{
		BuffTimeInfo& bufftimeinfo = (*iter).second;

		mu_float fTime = 0.0f;
		if (bufftimeinfo.s_EndBuffTime < g_CurrentTime)
		{
			fTime = (mu_float)(bufftimeinfo.s_EndBuffTime - g_CurrentTime) * 0.001f;
		}
		GetStringTime(fTime, timeText, issecond);
	}
}

void BuffTimeControl::GetStringTime(mu_uint32 time, mu_string& timeText, mu_boolean isSecond)
{
	mu_text FormatBuffer[128] = { 0 };

	if (isSecond)
	{
		mu_uint32 day = time / (1440 * 60);
		mu_uint32 oClock = (time - (day*(1440 * 60))) / 3600;
		mu_uint32 minutes = (time - ((oClock * 3600) + (day*(1440 * 60)))) / 60;
		mu_uint32 second = time % 60;

		if (day != 0)
		{
			mu_sprintf_s(FormatBuffer, mu_countof(FormatBuffer), _T("%d ") SPF_STRING _T(" %d ") SPF_STRING _T(" %d ") SPF_STRING _T(" %d ") SPF_STRING _T(""), day, GlobalText[2298], oClock, GlobalText[2299], minutes, GlobalText[2300], second, GlobalText[2301]);
		}
		else if (day == 0 && oClock != 0)
		{
			mu_sprintf_s(FormatBuffer, mu_countof(FormatBuffer), _T("%d ") SPF_STRING _T(" %d ") SPF_STRING _T(" %d ") SPF_STRING _T(""), oClock, GlobalText[2299], minutes, GlobalText[2300], second, GlobalText[2301]);
		}
		else if (day == 0 && oClock == 0 && minutes != 0)
		{
			mu_sprintf_s(FormatBuffer, mu_countof(FormatBuffer), _T("%d ") SPF_STRING _T(" %d ") SPF_STRING _T(""), minutes, GlobalText[2300], second, GlobalText[2301]);
		}
		else if (day == 0 && oClock == 0 && minutes == 0)
		{
			mu_sprintf_s(FormatBuffer, mu_countof(FormatBuffer), _T("") SPF_STRING _T(""), GlobalText[2308]);
		}
	}
	else
	{
		mu_uint32 day = time / 1440;
		mu_uint32 oClock = (time - (day * 1440)) / 60;
		mu_uint32 minutes = time % 60;

		if (day != 0)
		{
			mu_sprintf_s(FormatBuffer, mu_countof(FormatBuffer), _T("%d ") SPF_STRING _T(" %d ") SPF_STRING _T(" %d ") SPF_STRING _T(""), day, GlobalText[2298], oClock, GlobalText[2299], minutes, GlobalText[2300]);
		}
		else if (day == 0 && oClock != 0)
		{
			mu_sprintf_s(FormatBuffer, mu_countof(FormatBuffer), _T("%d ") SPF_STRING _T(" %d ") SPF_STRING _T(""), oClock, GlobalText[2299], minutes, GlobalText[2300]);
		}
		else if (day == 0 && oClock == 0 && minutes != 0)
		{
			mu_sprintf_s(FormatBuffer, mu_countof(FormatBuffer), _T("%d ") SPF_STRING _T(""), minutes, GlobalText[2300]);
		}
	}

	timeText = FormatBuffer;
}