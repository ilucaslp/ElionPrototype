#ifndef __MU_BUFFSTATEVALUECONTROL_H__
#define __MU_BUFFSTATEVALUECONTROL_H__

#pragma once

class BuffStateValueControl
{
private:
	struct BuffStateValueInfo
	{
		mu_uint32 s_Value1;
		mu_uint32 s_Value2;
		mu_uint32 s_Time;

		BuffStateValueInfo() : s_Value1(0), s_Value2(0), s_Time(0) {}
	};

public:
	BuffStateValueControl();
	~BuffStateValueControl();

public:
	const BuffStateValueInfo GetValue(eBuffState bufftype);
	void GetBuffInfoString(std::list<mu_string>& outstr, eBuffState bufftype);
	void GetBuffValueString(mu_string& outstr, eBuffState bufftype);
	eBuffValueLoadType CheckValue(eBuffState bufftype);

private:
	void SetValue(eBuffState bufftype, BuffStateValueInfo& valueinfo);
	void Initialize();
	void Destroy();

private:
	typedef std::map<eBuffState, BuffStateValueInfo> BuffStateValueMap;

private:
	BuffStateValueMap m_BuffStateValue;
};

#endif