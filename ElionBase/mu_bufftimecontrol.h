#ifndef __MU_BUFFTIMECONTROL_H__
#define __MU_BUFFTIMECONTROL_H__

#pragma once

class BuffTimeControl
{
public:
	BuffTimeControl();
	~BuffTimeControl();

public:
	void RegisterBuffTime(eBuffState bufftype, mu_uint32 curbufftime);
	mu_boolean UnRegisterBuffTime(eBuffState bufftype);
	mu_boolean IsBuffTime(eBuffTimeType bufftype);

public:
	void GetBuffStringTime(eBuffState bufftype, mu_string& timeText);
	void GetBuffStringTime(mu_uint32 type, mu_string& timeText, mu_boolean issecond = true);
	void GetStringTime(mu_uint32 time, mu_string& timeText, mu_boolean isSecond = true);

private:
	eBuffTimeType CheckBuffTimeType(eBuffState bufftype);
	mu_uint32 GetBuffEventTime(eBuffTimeType bufftimetype);
	mu_uint32 GetBuffMaxTime(eBuffState bufftype, mu_uint32 curbufftime = 0);

private:
	struct BuffTimeInfo
	{
		eBuffState s_BuffType;
		mu_uint64 s_EndBuffTime;

		BuffTimeInfo() : s_BuffType(eBuffNone), s_EndBuffTime(0) {}
	};
	typedef std::map<mu_uint32, BuffTimeInfo> BuffTimeInfoMap;

private:
	BuffTimeInfoMap m_BuffTimeList;
};

#endif