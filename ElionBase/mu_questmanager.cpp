#include "stdafx.h"
#include "mu_questmanager.h"

MUQuestManager g_QuestManager;

MUQuestManager::MUQuestManager()
{
	_Class = -1;
	_CurrentQuestIndex = 0;
	_CurrentQuestIndexWindow = 0;

	mu_zeromem(_EventCount, sizeof(_EventCount));
}

mu_boolean MUQuestManager::Load(const mu_text *filename)
{
	SDL_RWops *file = nullptr;

	if (mu_rwfromfile(&file, filename, "rb") == false)
	{
		return false;
	}

	static const mu_size HeaderSize = FileHeaders::HEADER_SIZE;
	static const mu_size CompressedHeaderSize = FileHeaders::COMPRESSED_HEADER_SIZE;

	mu_int64 fileLength = SDL_RWsize(file);
	mu_int64 encryptedLength = fileLength - HeaderSize;

	if (encryptedLength % MUBlowfish::BytesLength != 0)
	{
		return false;
	}

	mu_uint8 *dataBuffer = new mu_uint8[fileLength];
	SDL_RWread(file, dataBuffer, fileLength, 1);
	SDL_RWclose(file);

	FileHeaders::FileHeader *header = (FileHeaders::FileHeader*)dataBuffer;

	if (header->Signature != FILE_HEADERS::DATA_HEADER)
	{
		delete[] dataBuffer;
		return false;
	}

	if (GenerateCRC32(dataBuffer + HeaderSize, encryptedLength) != header->CRC)
	{
		delete[] dataBuffer;
		return false;
	}

	g_DataCrypt.Decode(dataBuffer + HeaderSize, dataBuffer + HeaderSize, encryptedLength);

	FileHeaders::CompressedHeader *compressedHeader = (FileHeaders::CompressedHeader*)(dataBuffer + HeaderSize);

	mu_uint8 *decompressedBuffer = new mu_uint8[compressedHeader->DecompressedSize];

	mu_boolean decompressed = FileHeaders::Decompress(dataBuffer + HeaderSize + CompressedHeaderSize,
		decompressedBuffer,
		compressedHeader->CompressedSize,
		compressedHeader->DecompressedSize);
	delete[] dataBuffer;

	if (decompressed == false)
	{
		delete[] decompressedBuffer;
		return false;
	}

	mu_uint8 *readBuffer = decompressedBuffer;
	{
		mu_uint32 questsCount = 0;
		FileHeaders::ReadBuffer(readBuffer, questsCount);

		_Quests.resize(questsCount);
		mu_asciistr textData;

		for (auto it = _Quests.begin(); it != _Quests.end(); ++it)
		{
			MUQuestData &quest = *it;

			mu_int32 index;
			FileHeaders::ReadBuffer(readBuffer, index);
			FileHeaders::ReadBuffer(readBuffer, quest.NpcType);

			mu_uint32 textLength = 0;
			FileHeaders::ReadBuffer(readBuffer, textLength);
			textData.resize(textLength);
			FileHeaders::ReadBuffer(readBuffer, (mu_char*)textData.data(), textLength);
#ifdef UNICODE
			quest.QuestName = boost::locale::conv::utf_to_utf<mu_text>(textData);
#else
			quest.QuestName = textData;
#endif

			mu_uint32 actsCount = 0;
			FileHeaders::ReadBuffer(readBuffer, actsCount);
			quest.QuestActs.resize(actsCount);
			FileHeaders::ReadBuffer(readBuffer, (void*)quest.QuestActs.data(), actsCount * sizeof(MUQuestClassAct));

			mu_uint32 requestsCount = 0;
			FileHeaders::ReadBuffer(readBuffer, requestsCount);
			quest.QuestRequests.resize(requestsCount);
			FileHeaders::ReadBuffer(readBuffer, (void*)quest.QuestRequests.data(), requestsCount * sizeof(MUQuestClassRequest));

			_QuestsMap.insert(std::make_pair(index, &quest));
		}
	}

	delete[] decompressedBuffer;

	return true;
}

void MUQuestManager::SetQuestLists(mu_uint8 *QuestData, mu_uint32 Class)
{
	if (Class != (mu_uint32)-1)
	{
		_Once = true;
		_Class = GetBaseClass(Class);
	}

	mu_memcpy(_QuestsData, QuestData, sizeof(_QuestsData));

	mu_uint32 i;

	if (_Class == GAME::CLASS_KNIGHT)
	{
		for (i = 0; i < MAX_QUESTS; ++i)
		{
			if (GetQuestState(i) != QUEST_END)
			{
				break;
			}
		}
	}
	else if (_Class == GAME::CLASS_DARK ||
		_Class == GAME::CLASS_DARK_LORD)
	{
		for (i = 4; i < MAX_QUESTS; ++i)
		{
			if (GetQuestState(i) != QUEST_END)
			{
				break;
			}
		}
	}
	else
	{
		for (i = 0; i < MAX_QUESTS; ++i)
		{
			if (i == QUEST_COMBO) continue;
			if (GetQuestState(i) != QUEST_END)
			{
				break;
			}
		}
	}

	_CurrentQuestIndex = i;
	_CurrentQuestIndexWindow = i;
}

void MUQuestManager::SetQuestList(mu_uint8 questIndex, mu_uint8 state)
{
	_CurrentQuestIndex = questIndex;
	_CurrentQuestIndexWindow = EMath::Max(questIndex, _CurrentQuestIndexWindow);

	mu_int32 index = (mu_int32)_CurrentQuestIndex / 4;
	_QuestsData[index] = state;
}

mu_uint8 MUQuestManager::GetQuestState(mu_int32 questIndex)
{
	mu_int32 Index;
	mu_uint8 byCurrState;
	mu_int32 SubIndex;

	if (questIndex == -1)
	{
		Index = (mu_int32)_CurrentQuestIndex / 4;
		SubIndex = _CurrentQuestIndex % 4;
	}
	else
	{
		Index = questIndex / 4;
		SubIndex = questIndex % 4;
	}

	byCurrState = (_QuestsData[Index] >> (SubIndex * 2)) & 0x03;

	if (questIndex == -1)
	{
		_CurrentState = byCurrState;
	}

	return byCurrState;
}

mu_uint8 MUQuestManager::GetQuestState2(mu_int32 questIndex)
{
	mu_int32 Index = questIndex / 4;
	mu_int32 SubIndex = questIndex % 4;
	
	return (_QuestsData[Index] >> (SubIndex * 2)) & 0x03;
}

mu_uint8 MUQuestManager::CheckQuestState(mu_uint8 questState)
{
	if (Hero->Class != (mu_uint8)-1 &&
		_Once == true)
	{
		_Once = false;
		_Class = GetBaseClass(Hero->Class);
	}

	const MUQuestData *questData = GetQuest(_CurrentQuestIndex);

	if (questData == nullptr)
	{
		return QUEST_ERROR;
	}

	if (questState == (mu_uint8)-1)
	{
		GetQuestState();
	}
	else
	{
		_CurrentState = questState;
	}

	switch (_CurrentState)
	{
	case QUEST_NO:
		{
			if (CheckRequestCondition(questData) == true)
			{
				_CurrentPage = FindQuestContext(questData, 0);
			}
		}
		break;

	case QUEST_ING:
		{
			if (CheckActCondition(questData) == true)
			{
				_CurrentPage = FindQuestContext(questData, 2);
				_CurrentState = QUEST_ITEM;
			}
		}
		break;

	case QUEST_END:
		{
			_CurrentPage = FindQuestContext(questData, 3);
		}
		break;
	}

	return _CurrentState;
}

const MUQuestData *MUQuestManager::GetQuest(mu_int32 questIndex)
{
	auto it = _QuestsMap.find(questIndex);
	if (it == _QuestsMap.end())
		return nullptr;

	return it->second;
}

const mu_text *MUQuestManager::GetQuestTitle(mu_int32 questIndex)
{
	auto it = _QuestsMap.find(questIndex);
	if (it == _QuestsMap.end())
		return _T("");

	return it->second->QuestName.c_str();
}

const mu_text *MUQuestManager::GetQuestTitle()
{
	return GetQuestTitle(_CurrentQuestIndex);
}

const mu_text *MUQuestManager::GetQuestTitleWindow()
{
	return GetQuestTitle(_CurrentQuestIndexWindow);
}

mu_int32 MUQuestManager::CheckQuestsItems(mu_int32 Type, mu_int32 Count, mu_int32 Level)
{
	UIInventoryControl *inventoryControl = g_pMyInventory->GetInventoryControl();

	mu_int32 FoundItemCount = 0;

	auto &items = inventoryControl->GetItems();
	for (auto it = items.begin(); it != items.end(); ++it)
	{
		ITEM *item = it->second;

		if (item->Type == Type &&
			(Level == -1 || Level == item->Level))
		{
			if (Count <= ++FoundItemCount)
			{
				return 0;
			}
		}
	}

	return Count - FoundItemCount;
}

mu_boolean MUQuestManager::CheckRequestCondition(const MUQuestData *questData, mu_boolean lastCheck)
{
	mu_uint32 conditionCount = (mu_uint32)questData->QuestActs.size();
	mu_uint32 requestCount = (mu_uint32)questData->QuestRequests.size();
	for (mu_uint32 i = 0; i < conditionCount; ++i)
	{
		const MUQuestClassAct &condition = questData->QuestActs[i];
		if (condition.RequestClass[_Class] != 0)
		{
			mu_int32 requestType = condition.RequestType;
			for (mu_int32 j = 0; j < requestCount; ++j)
			{
				const MUQuestClassRequest &request = questData->QuestRequests[j];

				if (request.Type == requestType || request.Type == 255)
				{
					if (request.CompleteQuestIndex != 65535)
					{
						if (GetQuestState2(request.CompleteQuestIndex) != QUEST_END)
						{
							_CurrentPage = request.ErrorText;
							_CurrentState = QUEST_ERROR;
							return false;
						}
					}

					if (request.LevelMin > 0)
					{
						mu_uint16 level = CharacterAttribute->Level;

						if (request.LevelMin > level)
						{
							_CurrentPage = request.ErrorText;
							_CurrentState = QUEST_ERROR;
							return false;
						}
					}

					if (request.LevelMax > 0)
					{
						mu_uint16 level = CharacterAttribute->Level;

						if (request.LevelMax < level)
						{
							_CurrentPage = request.ErrorText;
							_CurrentState = QUEST_ERROR;
							return false;
						}
					}

					if (request.Zen > 0)
					{
						_NeedZen = request.Zen;

						if (lastCheck)
						{
							if (_NeedZen > CharacterMachine->Gold)
							{
								_CurrentPage = request.ErrorText;
								_CurrentState = QUEST_ERROR;
								return false;
							}
						}
					}
				}
			}
		}
	}
	return true;
}

mu_boolean MUQuestManager::CheckActCondition(const MUQuestData *questData)
{
	mu_uint32 conditionCount = (mu_uint32)questData->QuestActs.size();
	for (mu_int32 i = 0; i < conditionCount; ++i)
	{
		const MUQuestClassAct &condition = questData->QuestActs[i];
		if (condition.RequestClass[_Class] >= 1)
		{
			switch (condition.QuestType)
			{
			case QUEST_ITEM:
				{
					mu_int32 itemType = (condition.ItemType*MAX_ITEM_INDEX) + condition.ItemSubType;
					mu_int32 itemLevel = -1;
					mu_int32 itemNum = condition.ItemNum;

					itemLevel = condition.ItemLevel;

					if (CheckQuestsItems(itemType, itemNum, itemLevel) > 0)
					{
						_CurrentPage = FindQuestContext(questData, 1);
						return false;
					}
				}
				break;

			case QUEST_MONSTER:
				{
					mu_boolean completed = false;

					for (mu_int32 j = 0; j < 5; ++j)
					{
						if (_KillMonsterType[j] == (mu_int32)condition.ItemType
							&& _KillMonsterCount[j] >= (mu_int32)condition.ItemNum)
						{
							completed = true;
							break;
						}
					}

					if (!completed)
					{
						_CurrentPage = FindQuestContext(questData, 1);
						return false;
					}
				}
				break;
			}
		}
	}
	return true;
}

mu_int16 MUQuestManager::FindQuestContext(const MUQuestData *questData, mu_int32 index)
{
	mu_uint32 conditionCount = (mu_uint32)questData->QuestActs.size();
	for (mu_int32 i = 0; i < conditionCount; ++i)
	{
		const MUQuestClassAct &condition = questData->QuestActs[i];
		if (condition.RequestClass[_Class] >= 1)
		{
			return condition.QuestStartText[index];
		}
	}

	--_CurrentQuestIndex;
	CheckQuestState();

	return _CurrentPage;
}

void MUQuestManager::ShowQuestPreviewWindow(mu_int32 questIndex)
{
	if (questIndex != -1)
	{
		_CurrentQuestIndex = questIndex;
	}

	_ViewQuest = QUEST_VIEW_PREVIEW;

	mu_uint8 tmp = _CurrentQuestIndex;
	_CurrentQuestIndex = _CurrentQuestIndexWindow;

	CheckQuestState();
	//ShowDialogText(_CurrentPage);

	_CurrentQuestIndex = tmp;
}

void MUQuestManager::ShowQuestNpcWindow(mu_int32 questIndex)
{
	if (questIndex != -1)
	{
		_CurrentQuestIndex = questIndex;
	}

	CheckQuestState();

	//ShowDialogText(_CurrentPage);
}