#include "stdafx.h"
#include "mu_datalist.h"

namespace GAMEDATA
{
	MULoadingData Load[] =
	{
		DEFINE_SHADERDATA()
		DEFINE_GAMEDATA(TEXT_DATA)
		DEFINE_GAMEDATA(FILTER_DATA)
		DEFINE_GAMEDATA(FILTERNAME_DATA)
		DEFINE_GAMEDATA(DIALOG_DATA)
		DEFINE_GAMEDATA(MOVEREQ_DATA)
		DEFINE_GAMEDATA(GATE_DATA)
		DEFINE_GAMEDATA(BUFFEFFECT_DATA)
		DEFINE_GAMEDATA(ITEM_DATA)
		DEFINE_GAMEDATA(ITEMCATEGORIES_DATA)
		DEFINE_GAMEDATA(ITEMADDOPTION_DATA)
		DEFINE_GAMEDATA(ITEMSETOPTION_DATA)
		DEFINE_GAMEDATA(SOCKETITEM_DATA)
		DEFINE_GAMEDATA(SKILL_DATA)
		DEFINE_GAMEDATA(PET_DATA)
		DEFINE_GAMEDATA(MIX_DATA)
		DEFINE_GAMEDATA(JEWELOFHARMONYOPTION_DATA)
		DEFINE_GAMEDATA(MONSTERSCRIPT_DATA)
		DEFINE_GAMEDATA(QUEST_DATA)
	};
	const mu_uint32 LoadCount = mu_countof(Load);
};