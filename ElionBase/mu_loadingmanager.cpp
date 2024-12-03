#include "stdafx.h"
#include "mu_loadingmanager.h"

MURWLock g_GPUDataLock;
MULoadingGPUData g_GPUData[2][GPUDATA_SIZE];
mu_atomic_uint32_t g_GPUDataCount[2];
mu_uint32 g_GPUQueueIndex = 0;

void ClearLoadingGPU()
{
	g_GPUDataLock.LockExclusive();
	g_GPUDataCount[0] = 0;
	g_GPUDataCount[1] = 0;
	g_GPUQueueIndex = 0;
	g_GPUDataLock.UnlockExclusive();
}

void ProcessLoadingGPU()
{
	// Critical Area
	{
		g_GPUDataLock.LockExclusive();
		mu_uint32 gpuQueueIndex = g_GPUQueueIndex;
		g_GPUQueueIndex = (gpuQueueIndex + 1) % 2;
		g_GPUDataLock.UnlockExclusive();

		mu_uint32 dataCount = g_GPUDataCount[gpuQueueIndex].exchange(0);

		for (mu_uint32 n = 0; n < dataCount; ++n)
		{
			MULoadingGPUData &gpuData = g_GPUData[gpuQueueIndex][n];

			switch (gpuData.DataType)
			{
			case GPU_DATATYPE::GPUDT_MODEL:
				{
					MUModel *m = (MUModel*)gpuData.Object;

					if (m->TransferCPUtoGPU((const Model::VertexData1*)gpuData.Data[0], gpuData.DataSize[0], gpuData.Data[1], gpuData.DataSize[1]) == false)
					{
						mu_error("Failed to transfer model to GPU");
					}

					mu_free(gpuData.Data[0]);
					mu_free(gpuData.Data[1]);
				}
				break;

			case GPU_DATATYPE::GPUDT_TEXTURE:
				{
					MUTexture *t = (MUTexture*)gpuData.Object;

					if (t->TransferCPUtoGPU(gpuData.Data[0], gpuData.DataSize[0]) == false)
					{
						mu_error("Failed to transfer texture to GPU");
					}

					mu_free(gpuData.Data[0]);
				}
				break;
			}
		}

		mu_zeromem(g_GPUData[gpuQueueIndex], dataCount * sizeof(MULoadingGPUData));
		EVideoPipeline::UnbindVAO();
	}
}

MULoadingManager g_LoadingManager;

MULoadingManager::MULoadingManager()
{
	/* Multithreading Loading System */
	_ThreadBarrier = nullptr;
	_WaitThreadBarrier = nullptr;
	_ThreadCount = 0;
	_Threads = nullptr;
	_ThreadDestroy = false;
	_ThreadRequestDestroy = false;

	_DataAvailable = false;
	_IgnoreData = false;
	_Data = nullptr;
}

MULoadingManager::~MULoadingManager()
{
	Destroy();
}

mu_boolean MULoadingManager::AddOnce(MULoadingData *Data, const mu_uint32 DataCount)
{
	_ThreadCritical.Lock();

	for (mu_uint32 index = 0, dataIndex = 0;dataIndex < DataCount; ++dataIndex)
	{
		if (Data[dataIndex].SubType == MULoadingSubType::ONCE)
		{
			_Data[index % _ThreadCount].push_back(&Data[dataIndex]);
			index = (index + 1) % _ThreadCount;
			++_TotalRequest;
		}
	}

	_ThreadCritical.Unlock();

	_DataAvailable = true;

	return true;
}

void MULoadingManager::AddRuntime(MULoadingData *Data)
{
	if (Data == nullptr) return;
	MUAutoCriticalSpinLock lock(_QueueCritical);
	_Queue.push_back(Data);
}

void MULoadingManager::AddTextureRuntime(const MUTexture *Texture)
{
	if(Texture >= &TEXTURES::Data[0] &&
		Texture < &TEXTURES::Data[MAX_TEXTURES])
	{
		const mu_uint32 textureIndex = (reinterpret_cast<mu_size>(Texture) - reinterpret_cast<mu_size>(&TEXTURES::Data[0])) / sizeof(MUTexture);
		const_cast<MUTexture*>(Texture)->RequestLoad();
		g_LoadingManager.AddRuntime(TEXTURES::LoadData[textureIndex]);
	}
}

void MULoadingManager::ProcessQueue()
{
	MUAutoCriticalSpinLock lock(_QueueCritical);
	mu_size queueSize = _Queue.size();
	if (queueSize > 0)
	{
		_ThreadCritical.Lock();

		for (mu_size index = 0, dataIndex = 0; dataIndex < queueSize; ++dataIndex)
		{
			_Data[index % _ThreadCount].push_back(_Queue[dataIndex]);
			index = (index + 1) % _ThreadCount;
			++_TotalRequest;
		}

		_ThreadCritical.Unlock();

		_Queue.clear();

		_DataAvailable = true;
	}
}

void MULoadingManager::Run(mu_uint8 Flags)
{
	if (_DataAvailable == false)
	{
		return;
	}

	if (_Threads == nullptr)
	{
		return;
	}

	_DataAvailable = false;
	_Flags = Flags;

	_ThreadBarrier->SyncSemaphore();
}

void MULoadingManager::RunLocked(mu_uint8 Flags)
{
	if (Flags == 0 && _DataAvailable == false)
	{
		return;
	}

	if (_Threads == nullptr)
	{
		return;
	}

	_DataAvailable = false;
	_Flags = Flags | LOADING_FLAGS::LF_LOCKED;

	_ThreadBarrier->SyncSemaphore();
	_WaitThreadBarrier->SyncSemaphore();
}

void MULoadingManager::WaitTillFinished()
{
	if (_Threads == nullptr)
	{
		return;
	}

	_Flags = LOADING_FLAGS::LF_LOCKED;

	_ThreadBarrier->SyncSemaphore();
	_WaitThreadBarrier->SyncSemaphore();
}

mu_boolean MULoadingManager::Initialize()
{
	if (_Threads == nullptr)
	{
		_ThreadDestroy = false;
#if DIRECTX_BACKEND_ENABLE == 1
		_ThreadCount = EMath::Min(g_DeviceCoreCount, _MaxLoadingThreads);
#else
		_ThreadCount = 1;
#endif

		_ThreadBarrier = new MUThreadBarrier(_ThreadCount + 1);
		_WaitThreadBarrier = new MUThreadBarrier(_ThreadCount + 1);

		if (_ThreadBarrier == nullptr ||
			_WaitThreadBarrier == nullptr)
		{
			Destroy();
			return false;
		}

		_Data = new std::vector<MULoadingData*>[_ThreadCount];
		_Threads = new std::thread[_ThreadCount];

		for (mu_uint32 index = 0; index < _ThreadCount; ++index)
		{
			_Threads[index] = std::thread(&MULoadingManager::Process, this, index);
		}
	}

	return true;
}

void MULoadingManager::Destroy()
{
	if (_ThreadDestroy == true)
	{
		return;
	}

	_ThreadDestroy = true;

	if(_ThreadBarrier != nullptr)
	{
		_Flags = LF_NORMAL;

		_ThreadBarrier->SyncSemaphore();

		if (_Threads != nullptr)
		{
			for (mu_uint32 n = 0; n < _ThreadCount; n++)
			{
				_Threads[n].join();
			}

			MU_SAFE_DELETE_ARRAY(_Threads);
		}
	}

	MU_SAFE_DELETE(_ThreadBarrier);
	MU_SAFE_DELETE(_WaitThreadBarrier);

	MU_SAFE_DELETE_ARRAY(_Data);
}

void MULoadingManager::RequestDestroy(LOADING_ERRORCODE errorCode)
{
	MUAutoCriticalSpinLock lock(_ThreadCritical);

	if (_ThreadRequestDestroy == false)
	{
		_ThreadRequestDestroy = true;
		_ThreadErrorCode = errorCode;
	}
}

mu_boolean MULoadingManager::RequiredDestroy()
{
	return _ThreadRequestDestroy;
}

mu_boolean MULoadingManager::IsDestroyed()
{
	return _ThreadDestroy;
}

const mu_uint32 ReportLoadingCount = 30;

void MULoadingManager::Process(const mu_uint32 ThreadIndex)
{
	std::vector<MULoadingData*> localData;
	mu_uint32 localCounters[LOADING_COUNTER::MAX] = { 0 };

	mu_text buffer[512];
	while (true)
	{
		_ThreadBarrier->SyncSemaphore();

		mu_uint32 Flags = _Flags;
		mu_boolean IgnoreData = _IgnoreData;

		if (_ThreadDestroy == true || _ThreadRequestDestroy == true)
		{
			break;
		}

		_ThreadCritical.Lock();

		if (_Data[ThreadIndex].empty() == false)
		{
			std::move(_Data[ThreadIndex].begin(), _Data[ThreadIndex].end(), std::back_inserter(localData));
			_Data[ThreadIndex].clear();
		}

		_ThreadCritical.Unlock();

		mu_uint32 loadCount = 0;

		std::map<mu_atomic_uint32_t*, mu_uint32> mapCounter;

		if (IgnoreData == false)
		{
			for (auto it = localData.begin(); it != localData.end(); ++it)
			{
				if (_ThreadDestroy == true || _ThreadRequestDestroy == true)
				{
					break;
				}

				if (loadCount >= ReportLoadingCount)
				{
					for (mu_uint32 index = 0; index < LOADING_COUNTER::MAX; ++index)
					{
						LoadingData::Counter[index] += localCounters[index];
						localCounters[index] = 0;
					}

					loadCount = 0;
				}

				MULoadingData *data = *it;

				switch (data->Type)
				{
				case MULoadingType::LOADING_SKIP:
					break;

				case MULoadingType::LOADING_SHADER:
					{
						if (EVideoBackend::PrepareShaders() == false)
						{
							RequestDestroy(LOADING_ERRORCODE::FAILED_SHADER);
							break;
						}
					}
					break;

				case MULoadingType::LOADING_TEXTURE:
					{
						MUTexture *texture = &((MUTexture*)data->Data)[data->Index];

						texture->RequestLoad();

						if (texture->LoadHeaderFromFile(data->Path) == false)
						{
							RequestDestroy(LOADING_ERRORCODE::FAILED_TEXTURE);
							break;
						}

						if (texture->LoadFromFileEx(data->Path) == false)
						{
							RequestDestroy(LOADING_ERRORCODE::FAILED_TEXTURE);
							break;
						}
					}
					break;

				case MULoadingType::LOADING_TEXTURE_RUNTIME:
					{
						MUTexture *texture = &((MUTexture*)data->Data)[data->Index];

						if (texture->LoadHeaderFromFile(data->Path) == false)
						{
							RequestDestroy(LOADING_ERRORCODE::FAILED_TEXTURE);
							break;
						}

						if (texture->IsRequested() == true &&
							texture->LoadFromFileEx(data->Path) == false)
						{
							RequestDestroy(LOADING_ERRORCODE::FAILED_TEXTURE);
							break;
						}
					}
					break;

				case MULoadingType::LOADING_MODEL:
					{
						MUModel *model = &((MUModel*)data->Data)[data->Index];

						model->_IsRequested = true;

						if (model->LoadHeaderFromFileEx(data->Path) == false)
						{
							RequestDestroy(LOADING_ERRORCODE::FAILED_MODEL);
							break;
						}

						if (model->LoadFromFileEx(data->Path) == false)
						{
							RequestDestroy(LOADING_ERRORCODE::FAILED_MODEL);
							break;
						}
					}
					break;

				case MULoadingType::LOADING_MODEL_RUNTIME:
					{
						MUModel *model = &((MUModel*)data->Data)[data->Index];

						if (model->LoadHeaderFromFileEx(data->Path) == false)
						{
							RequestDestroy(LOADING_ERRORCODE::FAILED_MODEL);
							break;
						}

						if (model->IsRequestedLoad() == true &&
							model->LoadFromFileEx(data->Path) == false)
						{
							RequestDestroy(LOADING_ERRORCODE::FAILED_MODEL);
							break;
						}
					}
					break;

				case MULoadingType::LOADING_SOUND:
					{
						MUWavData *arrayData = (MUWavData*)data->Data;
						arrayData[data->Index].SetWavPath(data->Path);
					}
					break;

				case MULoadingType::LOADING_DATA:
					{
						switch (data->Index)
						{
						case GAMEDATA::TEXT_DATA:
							{
								if (GlobalText.Load(_T("./data/local/text.datx")) == false)
								{
									RequestDestroy(LOADING_ERRORCODE::FAILED_DATA);
									break;
								}
							}
							break;

						case GAMEDATA::FILTER_DATA:
							{
								if (OpenFilterFile(_T("./data/local/filter.datx")) == false)
								{
									RequestDestroy(LOADING_ERRORCODE::FAILED_DATA);
									break;
								}
							}
							break;

						case GAMEDATA::FILTERNAME_DATA:
							{
								if (OpenNameFilterFile(_T("./data/local/filtername.datx")) == false)
								{
									RequestDestroy(LOADING_ERRORCODE::FAILED_DATA);
									break;
								}
							}
							break;

						case GAMEDATA::DIALOG_DATA:
							{
								if (g_DialogManager.Load(_T("./data/local/dialog.datx")) == false)
								{
									RequestDestroy(LOADING_ERRORCODE::FAILED_DATA);
									break;
								}
							}
							break;

						case GAMEDATA::MOVEREQ_DATA:
							{
								if (g_MoveManager.Open(_T("./data/local/movereq.datx")) == false)
								{
									RequestDestroy(LOADING_ERRORCODE::FAILED_DATA);
									break;
								}
							}
							break;

						case GAMEDATA::GATE_DATA:
							{
								if (OpenGateScript(_T("./data/local/gate.datx")) == false)
								{
									RequestDestroy(LOADING_ERRORCODE::FAILED_DATA);
									break;
								}
							}
							break;

						case GAMEDATA::BUFFEFFECT_DATA:
							{
								if (g_BuffSystem.GetBuffInfo().Load(_T("./data/local/buffeffect.datx")) == false)
								{
									RequestDestroy(LOADING_ERRORCODE::FAILED_DATA);
									break;
								}
							}
							break;

						case GAMEDATA::ITEM_DATA:
							{
								if (OpenItemScript(_T("./data/local/item.datx")) == false)
								{
									RequestDestroy(LOADING_ERRORCODE::FAILED_DATA);
									break;
								}
							}
							break;

						case GAMEDATA::ITEMCATEGORIES_DATA:
							{
								if (g_ItemCategoriesDatabase.Load(_T("./data/local/itemcategoriesdb.datx")) == false)
								{
									RequestDestroy(LOADING_ERRORCODE::FAILED_DATA);
									break;
								}
							}
							break;

						case GAMEDATA::ITEMADDOPTION_DATA:
							{
								if (g_pItemAddOptioninfo.OpenItemAddOptionInfoFile(_T("./data/local/itemaddoption.datx")) == false)
								{
									RequestDestroy(LOADING_ERRORCODE::FAILED_DATA);
									break;
								}
							}
							break;

						case GAMEDATA::ITEMSETOPTION_DATA:
							{
								if (g_csItemOption.OpenItemSetScript() == false)
								{
									RequestDestroy(LOADING_ERRORCODE::FAILED_DATA);
									break;
								}
							}
							break;

						case GAMEDATA::SOCKETITEM_DATA:
							{
								if (g_SocketItemMgr.OpenSocketItemScript(_T("./data/local/socketitem.datx")) == false)
								{
									RequestDestroy(LOADING_ERRORCODE::FAILED_DATA);
									break;
								}
							}
							break;

						case GAMEDATA::SKILL_DATA:
							{
								if (OpenSkillScript(_T("./data/local/skill.datx")) == false)
								{
									RequestDestroy(LOADING_ERRORCODE::FAILED_DATA);
									break;
								}
							}
							break;

						case GAMEDATA::PET_DATA:
							{
								if (PetProcess::Load(_T("./data/local/pet.datx")) == false)
								{
									RequestDestroy(LOADING_ERRORCODE::FAILED_DATA);
									break;
								}
							}
							break;

						case GAMEDATA::MIX_DATA:
							{
								if (g_MixManager.Load(_T("./data/local/mix.datx")) == false)
								{
									RequestDestroy(LOADING_ERRORCODE::FAILED_DATA);
									break;
								}
							}
							break;

						case GAMEDATA::JEWELOFHARMONYOPTION_DATA:
							{
								if (g_pUIJewelHarmonyinfo.OpenJewelHarmonyInfoFile(_T("./data/local/jewelofharmonyoption.datx")) == false)
								{
									RequestDestroy(LOADING_ERRORCODE::FAILED_DATA);
									break;
								}
							}
							break;

						case GAMEDATA::MONSTERSCRIPT_DATA:
							{
								if (OpenMonsterScript(_T("./data/local/npcname.txt")) == false ||
									OpenMonsterSkillScript(_T("./data/local/monsterskill.datx")) == false)
								{
									RequestDestroy(LOADING_ERRORCODE::FAILED_DATA);
									break;
								}
							}
							break;

						case GAMEDATA::QUEST_DATA:
							{
								if (g_QuestManager.Load(_T("./data/local/quest.datx")) == false)
								{
									RequestDestroy(LOADING_ERRORCODE::FAILED_DATA);
									break;
								}
							}
							break;
						}
					}
					break;
				}

				if (_ThreadRequestDestroy == false)
				{
					++loadCount;
					++localCounters[data->Counter];
				}
			}
		}

		localData.clear();

		if (loadCount > 0)
		{
			for (mu_uint32 index = 0;index < LOADING_COUNTER::MAX; ++index)
			{
				LoadingData::Counter[index] += localCounters[index];
				localCounters[index] = 0;
			}
		}

		if (IgnoreData == false &&
			(Flags & LOADING_FLAGS::LF_MAPS) != 0)
		{
			if (GET_WORLD == WD_0LORENCIA)
			{
				for (mu_uint32 i = ThreadIndex; i < MODELS::LorenciaCount; i += _ThreadCount)
				{
					MULoadingData *data = &MODELS::Lorencia[i];

					MODELS::Data[data->Index].LoadHeaderFromFileEx(data->Path);
					MODELS::Data[data->Index].LoadFromFileEx(data->Path);
				}
			}
			else if(GET_WORLD != -1)
			{
				mu_text BasePath[512] = { 0 };
				mu_text Path[512] = { 0 };

				mu_int32 WorldID = GET_WORLD + 1;

				if (InBloodCastle() == true)
				{
					WorldID = WD_11BLOODCASTLE1 + 1;
				}
				else if (InChaosCastle() == true)
				{
					WorldID = WD_18CHAOS_CASTLE + 1;
				}
				else if (InHellas() == true)
				{
					WorldID = WD_24HELLAS + 1;
				}
				else if (g_CursedTemple.IsCursedTemple())
				{
					WorldID = WD_45CURSEDTEMPLE_LV1 + 2;
				}

				mu_sprintf_s(BasePath, mu_countof(BasePath), _T("./data/object%d/"), WorldID);

				for (mu_uint32 i = ThreadIndex; i < MAX_WORLD_OBJECTS; i += _ThreadCount)
				{
					mu_sprintf_s(Path, mu_countof(Path), _T("") SPF_STRING _T("object%02d"), BasePath, i + 1);

					MODELS::Data[i].LoadHeaderFromFileEx(Path);
					MODELS::Data[i].LoadFromFileEx(Path);

					switch (GET_WORLD)
					{
					case WD_1DUNGEON:
						switch (i)
						{
						case 22:
						case 23:
						case 24:
							{
								for (mu_uint32 k = 0; k < MAX_TASKING_THREADS; ++k)
								{
									MODELS::Data[i].StreamMesh(k) = 1;
								}
							}
							break;
						}
						break;

					case WD_3NORIA:
						switch (i)
						{
						case 42:
						case 43:
							{
								for (mu_uint32 k = 0; k < MAX_TASKING_THREADS; ++k)
								{
									MODELS::Data[i].StreamMesh(k) = 0;
								}
							}
							break;
						}
						break;

					case WD_8TARKAN:
						switch (i)
						{
						case 11:
						case 12:
						case 13:
						case 73:
						case 75:
						case 79:
							{
								for (mu_uint32 k = 0; k < MAX_TASKING_THREADS; ++k)
								{
									MODELS::Data[i].StreamMesh(k) = 0;
								}
							}
							break;
						}
						break;
					}
				}
			}
		}

		if ((Flags & LOADING_FLAGS::LF_LOCKED) != 0)
		{
			_WaitThreadBarrier->SyncSemaphore();
		}
	}
}