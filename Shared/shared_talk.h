#ifndef __SHARED_TALK_H__
#define __SHARED_TALK_H__

#pragma once

#define MIX_LEVEL 7

namespace TALK
{
	enum
	{
		NT_SHOP,
		NT_STORAGE,
		NT_MIX,
		NT_GUILDCREATE,
		NT_DEVILSQUARE,
		NT_BLOODCASTLE,
		NT_BATTLECASTLE,
		NT_PETTRAINER,
		NT_GEMMIX,
		NT_SENATUS,
		NT_ELPIS,
		NT_OSBOURNE,
		NT_JERRIDON,
		NT_CURSEDTEMPLE,
		NT_CHAOSCARD,
		NT_CHERRYBLOSSOM,
		NT_SEEDMASTER,
		NT_SEEDINVESTIGATOR,
		NT_DELGARDO,
		NT_DUELWATCH,
		NT_GAMBLE,
		NT_DOPPELGANGER,
		NT_EMPIREGUARDIAN,
		NT_MARKETJULIA,
		NT_NPCDIALOG,
		NT_QUESTDIALOG,
	};

	class SHOP_DATA
	{
	public:
		mu_boolean IsRepairShop;
	};

	class MIX_DATA
	{
	public:
		mu_uint8 level[MIX_LEVEL];
	};

	class DIALOG_DATA
	{
	public:
		mu_uint16 NpcIndex;
		mu_uint32 ContributePoint;
	};

	class QUEST_DATA
	{
	public:
		mu_boolean CheckNpc;
		mu_uint8 QuestIndex;
		mu_uint8 State;
	};
};

#endif