#ifndef __MU_QUESTMANAGER_H__
#define __MU_QUESTMANAGER_H__

#pragma once

#define MAX_QUESTS (GAME::MAX_DBQUEST*4)

enum
{
	TYPE_QUEST = 0,
	TYPE_DEVILSQUARE,
	TYPE_BLOODCASTLE,
	TYPE_CURSEDTEMPLE,
	TYPE_QUEST_END
};

enum
{
	QUEST_CHANGE_UP_1 = 0,
	QUEST_CHANGE_UP_2,
	QUEST_CHANGE_UP_3,
	QUEST_COMBO,
	QUEST_3RD_CHANGE_UP_1,
	QUEST_3RD_CHANGE_UP_2,
	QUEST_3RD_CHANGE_UP_3,
	QUEST_LIST_END
};

enum
{
	QUEST_NONE = 0,
	QUEST_ING = 1,
	QUEST_END,
	QUEST_NO,
	QUEST_READY,
	QUEST_ERROR,
};

enum
{
	QUEST_VIEW_NONE = 0,
	QUEST_VIEW_NPC,
	QUEST_VIEW_PREVIEW,
	QUEST_VIEW_END
};

enum
{
	QUEST_ITEM = 1,
	QUEST_MONSTER,
};

#pragma pack(push, 4)
class MUQuestClassAct
{
public:
	mu_uint8 QuestType;
	mu_uint16 ItemType;
	mu_uint16 ItemSubType;
	mu_uint8 ItemLevel;
	mu_uint8 ItemNum;
	mu_uint8 RequestType;
	mu_uint8 RequestClass[GAME::MAX_CLASS];
	mu_int16 QuestStartText[4];
};

class MUQuestClassRequest
{
public:
	mu_uint8 Type;
	mu_uint16 CompleteQuestIndex;
	mu_uint16 LevelMin;
	mu_uint16 LevelMax;
	mu_uint16 RequestStrength;
	mu_int64 Zen;
	mu_int16 ErrorText;
};
#pragma pack(pop)

class MUQuestData
{
public:
	mu_uint16 NpcType;

	mu_string QuestName;

	std::vector<MUQuestClassAct> QuestActs;
	std::vector<MUQuestClassRequest> QuestRequests;
};

class MUQuestManager
{
public:
	MUQuestManager();

	mu_boolean Load(const mu_text *filename);

	void SetQuestLists(mu_uint8 *QuestData, mu_uint32 Class);
	void SetQuestList(mu_uint8 questIndex, mu_uint8 state);
	mu_uint8 GetQuestState(mu_int32 questIndex = -1);
	mu_uint8 GetQuestState2(mu_int32 questIndex = -1);
	mu_uint8 CheckQuestState(mu_uint8 questState = -1);

	const MUQuestData *GetQuest(mu_int32 questIndex);

	const mu_text *GetQuestTitle(mu_int32 questIndex);
	const mu_text *GetQuestTitle();
	const mu_text *GetQuestTitleWindow();

	mu_int32 CheckQuestsItems(mu_int32 Type, mu_int32 Count, mu_int32 Level);
	mu_boolean CheckRequestCondition(const MUQuestData *questData, mu_boolean lastCheck = false);
	mu_boolean CheckActCondition(const MUQuestData *questData);
	mu_int16 FindQuestContext(const MUQuestData *questData, mu_int32 index);

	void ShowQuestPreviewWindow(mu_int32 questIndex);
	void ShowQuestNpcWindow(mu_int32 questIndex);

public:
	EINLINE mu_uint8 GetCurrentQuestIndex()
	{
		return _CurrentQuestIndex;
	}

	EINLINE mu_int32 GetCurrentPage()
	{
		return _CurrentPage;
	}

	EINLINE mu_uint8 GetCurrentQuestState()
	{
		return CheckQuestState();
	}

	EINLINE void SetEventCount(mu_uint8 Type, mu_int32 Count)
	{
		if (Type < TYPE_QUEST_END)
		{
			_EventCount[Type] = Count;
		}
	}

	EINLINE mu_int32 GetEventCount(mu_uint8 Type)
	{
		return Type >= TYPE_QUEST_END ? 0 : _EventCount[Type];
	}

	EINLINE mu_int64 GetNeedZen()
	{
		return _NeedZen;
	}

	EINLINE const mu_text *GetNpcName(mu_uint8 questIndex)
	{
		auto it = _QuestsMap.find(questIndex);
		if (it == _QuestsMap.end())
			return _T("");

		return GetMonsterName(it->second->NpcType);
	}

	EINLINE mu_boolean GetCheckNpc()
	{
		return _CheckNpc;
	}

	EINLINE void SetCheckNpc(mu_boolean checkNpc)
	{
		_CheckNpc = checkNpc;
	}

	EINLINE mu_int32 GetKillMobCount(mu_int32 MobType)
	{
		for (mu_int32 n = 0; n < 5; ++n)
		{
			if (MobType == _KillMonsterType[n])
			{
				return _KillMonsterCount[n];
			}
		}

		return 0;
	}

private:
	std::vector<MUQuestData> _Quests;
	std::map<mu_int32, MUQuestData*> _QuestsMap;

	mu_boolean _Once;
	mu_boolean _CheckNpc;
	mu_uint8 _Class;
	mu_uint8 _QuestsData[GAME::MAX_DBQUEST];
	mu_uint8 _CurrentQuestIndex;
	mu_uint8 _CurrentQuestIndexWindow;
	mu_uint8 _CurrentState;
	mu_uint8 _StartQuestList;
	mu_uint8 _CurrentPage;
	mu_uint8 _ViewQuest;
	mu_uint8 _QuestType;
	mu_int32 _EventCount[TYPE_QUEST_END];
	mu_int32 _StartX;
	mu_int32 _StartY;
	mu_int64 _NeedZen;

	mu_int32 _KillMonsterType[5];
	mu_int32 _KillMonsterCount[5];
};

extern MUQuestManager g_QuestManager;

#endif