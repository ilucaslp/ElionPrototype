#ifndef __MU_PARTY_H__
#define __MU_PARTY_H__

#pragma once

class CHARACTER;

class PARTY_t
{
public:
	PARTY_t()
	{
		Clear();
	}

	void Clear()
	{
		Name.clear();
		Key = -1;
		index = -2;
	}

public:
	mu_string Name;
	mu_int16 Key;
	mu_uint8 Number;
	mu_uint8 Map;
	mu_uint8 x;
	mu_uint8 y;
	mu_float currHP;
	mu_float maxHP;
	mu_uint8 stepHP;
	mu_int32 index;
};

extern PARTY_t Party[MAX_PARTYS];
extern mu_int32 PartyNumber;

namespace PartyManager
{
	mu_boolean IsPartyMember(mu_int32 index);
	mu_boolean IsPartyMemberChar(CHARACTER * c);
}

#endif