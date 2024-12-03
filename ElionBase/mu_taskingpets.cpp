#include "stdafx.h"
#include "mu_tasking.h"

void MUTasking::MovePets(const mu_uint32 ThreadIndex)
{
#ifdef DIVIDE_TASKING
	mu_uint32 beginIndex, endIndex;
	GetIndexTasking(ThreadIndex, MAX_PETS, beginIndex, endIndex);

	for (mu_uint32 i = beginIndex; i < endIndex; ++i)
#else
	for (mu_uint32 i = ThreadIndex; i < MAX_PETS; i += _ThreadsCount)
#endif
	{
		MUPetObject *p = &g_PetObjects[i];

		if(p->Live == true)
		{
			p->Update(ThreadIndex);
		}
	}
}

void MUTasking::CheckPets(const mu_uint32 ThreadIndex)
{
	mu_uint32 queueCount = 0;
	const mu_uint32 queueIndex = MUThreadQueues::PetObjectsQueueIndex[ThreadIndex];

	mu_uint32 beginIndex, endIndex;
	GetIndexTasking(ThreadIndex, MAX_PETS, beginIndex, endIndex);

	for (mu_uint32 i = beginIndex; i < endIndex; ++i)
	{
		const MUPetObject *p = &g_PetObjects[i];

		if (p->Live == true)
		{
			MUThreadQueues::PetObjectsQueue[queueIndex + queueCount++] = i;
		}
	}

	MUThreadQueues::PetObjectsQueueCount[ThreadIndex] = queueCount;
}

void MUTasking::PreparePets(const mu_uint32 ThreadIndex)
{
	MUThreadInfo &ThreadInfo = g_ThreadInfo[ThreadIndex];

	mu_uint32 q = ThreadIndex;
	for (mu_uint32 t = 0; t < _ThreadsCount; ++t)
	{
		const mu_uint32 queueCount = MUThreadQueues::PetObjectsQueueCount[t];
		const mu_uint16 *queue = MUThreadQueues::PetObjectsQueue.data() + MUThreadQueues::PetObjectsQueueIndex[t];

		for (; q < queueCount; q += _ThreadsCount)
		{
			const mu_uint32 petIndex = queue[q];
			MUPetObject *p = &g_PetObjects[petIndex];

			ThreadInfo.RenderIndex = 0;
			ThreadInfo.CulledType = CULLED_PETS;
			ThreadInfo.ObjectIndex = petIndex;

			p->Render(ThreadIndex);
		}

		q -= queueCount;
	}
}