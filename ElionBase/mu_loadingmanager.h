#ifndef __MU_LOADING_MANAGER_H__
#define __MU_LOADING_MANAGER_H__

#pragma once

enum class LOADING_ERRORCODE : mu_uint32
{
	FAILED_STOPPED = 0,
	FAILED_TEXTURE,
	FAILED_MODEL,
	FAILED_SOUND,
	FAILED_SHADER,
	FAILED_DATA,
};

enum LOADING_FLAGS : mu_uint32
{
	LF_NORMAL = 0,
	LF_MAPS = 1,
	LF_LOCKED = 2,
};

enum GPU_DATATYPE : mu_uint8
{
	GPUDT_MODEL,
	GPUDT_TEXTURE,
};

#define GPUDATA_SIZE (25000)

struct MULoadingGPUData
{
	GPU_DATATYPE DataType;
	void *Object;
	mu_uint8 *Data[2];
	mu_uint32 DataSize[2];
};

extern MURWLock g_GPUDataLock;
extern MULoadingGPUData g_GPUData[2][GPUDATA_SIZE];
extern mu_atomic_uint32_t g_GPUDataCount[2];
extern mu_uint32 g_GPUQueueIndex;

void ClearLoadingGPU();
void ProcessLoadingGPU();

class MULoadingManager
{
public:
	MULoadingManager();
	~MULoadingManager();

	mu_boolean AddOnce(MULoadingData *Data, const mu_uint32 DataCount);
	void AddRuntime(MULoadingData *Data);
	void AddTextureRuntime(const MUTexture *Texture);

	void ProcessQueue();
	void Run(mu_uint8 Flags = LOADING_FLAGS::LF_NORMAL);
	void RunLocked(mu_uint8 Flags = LOADING_FLAGS::LF_NORMAL);
	void WaitTillFinished();
	mu_boolean Initialize();
	void Destroy();
	void RequestDestroy(LOADING_ERRORCODE errorCode = LOADING_ERRORCODE::FAILED_STOPPED);
	mu_boolean RequiredDestroy();
	mu_boolean IsDestroyed();
	void Process(const mu_uint32 ThreadIndex);

	mu_uint64 GetTotalRequest()
	{
		return _TotalRequest;
	}

public:
	/* Multithreading Loading System */
	static const mu_uint32 _MaxLoadingThreads = 2;

private:
	friend class MURoot;
	MUCriticalSectionSpinLock _QueueCritical;
	MUCriticalSectionSpinLock _ThreadCritical;
	MUThreadBarrier *_ThreadBarrier;
	MUThreadBarrier *_WaitThreadBarrier;
	mu_uint32 _ThreadCount;
	std::thread *_Threads;
	mu_atomic_bool _ThreadDestroy;
	mu_atomic_bool _ThreadRequestDestroy;
	LOADING_ERRORCODE _ThreadErrorCode;

	// Loading Information
	mu_uint64 _TotalRequest;
	mu_atomic_bool _DataAvailable;
	mu_atomic_bool _IgnoreData;
	mu_uint8 _Flags;
	std::vector<MULoadingData*> *_Data;
	std::vector<MULoadingData*> _Queue;
};

extern MULoadingManager g_LoadingManager;

#define ADD_RUNTIME_TEXTURE(index) g_LoadingManager.AddRuntime(&GAME_TEXTURES::Data[index]);
#define ADD_RUNTIME_MODEL(index) g_LoadingManager.AddRuntime(&GAME_MODELS::Data[index]);

#endif