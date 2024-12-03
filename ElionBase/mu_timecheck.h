#ifndef __MU_TIMECHECK_H__
#define __MU_TIMECHECK_H__

#pragma once

struct STimeCheck
{
	mu_uint32 Index;
	mu_uint64 BackupTime;
	mu_boolean TimeCheck;
};

class MUTimeCheck
{
public:
	MUTimeCheck();
	~MUTimeCheck();

	mu_uint32 CheckIndex(mu_uint32 index);
	mu_boolean GetTimeCheck(mu_uint32 index, mu_uint64 DelayTime);
	void DeleteTimeIndex(mu_uint32 index);

public:
	std::vector<STimeCheck> _TimeList;
};

extern MUTimeCheck g_Time;

#endif