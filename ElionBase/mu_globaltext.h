#ifndef __MU_GLOBALTEXT_H__
#define __MU_GLOBALTEXT_H__

#pragma once

class MUGlobalText
{
public:
	MUGlobalText();
	~MUGlobalText();

	mu_boolean Load(const mu_text *filename);

	const mu_text *operator[](mu_int32 Key)
	{
		return _TextMap[Key].c_str();
	}

	const mu_string &Get(mu_int32 Key)
	{
		return _TextMap[Key];
	}
	
public:
	std::map<mu_uint32, mu_string> _TextMap;
};

extern MUGlobalText GlobalText;

#endif