#include "stdafx.h"
#include "mu_party.h"

PARTY_t Party[MAX_PARTYS];
mu_int32 PartyNumber = 0;

mu_boolean PartyManager::IsPartyMember(mu_int32 index)
{
	CHARACTER* c = &CharactersClient[index];
	return IsPartyMemberChar(c);
}

mu_boolean PartyManager::IsPartyMemberChar(CHARACTER * c)
{
	for (mu_int32 i = 0; i < PartyNumber; ++i)
	{
		PARTY_t *p = &Party[i];

		if (p->Key == c->Key) return true;
	}

	return false;
}
