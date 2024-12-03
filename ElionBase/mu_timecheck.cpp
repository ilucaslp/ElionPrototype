#include "stdafx.h"
#include "mu_timecheck.h"

MUTimeCheck g_Time;

MUTimeCheck::MUTimeCheck()
{

}

MUTimeCheck::~MUTimeCheck()
{

}

mu_uint32 MUTimeCheck::CheckIndex(mu_uint32 index)
{
	mu_uint32 i = 0;
	for (; i < _TimeList.size(); ++i)
	{
		if (_TimeList[i].Index == index)
			return i;
	}

	STimeCheck T;

	T.Index = index;
	T.BackupTime = 0;
	T.TimeCheck = true;

	_TimeList.push_back(T);

	return i;
}

mu_boolean MUTimeCheck::GetTimeCheck(mu_uint32 index, mu_uint64 DelayTime)
{
	mu_uint32 I = CheckIndex(index);
	STimeCheck &timeCheck = _TimeList[I];

	mu_uint64 PresentTime = g_CurrentTime;

	if (timeCheck.TimeCheck)
	{
		timeCheck.BackupTime = PresentTime;
		timeCheck.TimeCheck = false;
	}

	if (timeCheck.BackupTime + DelayTime <= PresentTime)
	{
		timeCheck.TimeCheck = true;
		return true;
	}

	return false;
}

void MUTimeCheck::DeleteTimeIndex(mu_uint32 index)
{
	for (auto it = _TimeList.begin();it != _TimeList.end(); ++it)
	{
		if ((*it).Index == index)
		{
			_TimeList.erase(it);
			break;
		}
	}
}