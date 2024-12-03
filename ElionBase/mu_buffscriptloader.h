#ifndef __MU_BUFFSCRIPTLOADER_H__
#define __MU_BUFFSCRIPTLOADER_H__

#pragma once

#define MAX_BUFF_NAME_LENGTH	50
#define MAX_DESCRIPT_LENGTH		100

class BuffInfoBase
{
public:
	mu_int16	s_BuffIndex;
	mu_uint8	s_BuffEffectType;
	mu_uint8	s_ItemType;
	mu_uint8	s_ItemIndex;
	mu_string	s_BuffName;
	mu_uint8	s_BuffClassType;
	mu_uint8	s_NoticeType;
	mu_uint8	s_ClearType;
	mu_string	s_BuffDescript;
};

class BuffInfo : public BuffInfoBase
{
public:
	std::list<mu_string> s_BuffDescriptlist;
};

class BuffScriptLoader
{
public:
	BuffScriptLoader();
	~BuffScriptLoader();

	mu_boolean Load(const mu_text *filename);

public:
	const BuffInfo GetBuffinfo(eBuffState type) const;
	eBuffClass IsBuffClass(eBuffState type) const;

private:
	typedef std::map<eBuffState, BuffInfo> BuffInfoMap;

private:
	BuffInfoMap m_Info;
};

#endif