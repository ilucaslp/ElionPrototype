#include "stdafx.h"
#include "mu_petprocess.h"

std::map<mu_int32, PetInfo*> g_PetInfo;
std::map<mu_int32, MUPetAction*> g_PetActions;

MUCriticalSectionSpinLock g_PetObjectsLock;
std::array<MUPetObject, MAX_PETS> g_PetObjects;
std::array<mu_uint16, MAX_PETS> g_PetObjectsFree;
std::array<mu_uint16, MAX_PETS + MAX_TASKING_THREADS> g_PetObjectsQueue;
mu_uint16 g_PetObjectsCurCount = 0;
mu_uint16 g_PetObjectsFreeCount = 0;

mu_boolean PetProcess::Load(const mu_text *filename)
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

	mu_uint32 petCount = 0;
	FileHeaders::ReadBuffer(readBuffer, petCount);

	for (mu_int32 n = 0; n < petCount; ++n)
	{
		PetInfo *petInfo = new PetInfo;

		FileHeaders::ReadBuffer(readBuffer, petInfo->Type);
		FileHeaders::ReadBuffer(readBuffer, petInfo->Index);
		FileHeaders::ReadBuffer(readBuffer, petInfo->BlendMesh);
		FileHeaders::ReadBuffer(readBuffer, petInfo->Scale);

		FileHeaders::ReadBuffer(readBuffer, petInfo->Count);
		petInfo->Actions = new mu_uint8[petInfo->Count];
		FileHeaders::ReadBuffer(readBuffer, petInfo->Actions, sizeof(mu_uint8) * petInfo->Count);
		petInfo->Speed = new mu_float[petInfo->Count];
		FileHeaders::ReadBuffer(readBuffer, petInfo->Speed, sizeof(mu_float) * petInfo->Count);

		g_PetInfo.insert(std::make_pair(petInfo->Type * MAX_ITEM_INDEX + petInfo->Index, petInfo));
	}

	delete[] decompressedBuffer;

	// Init
	{
		g_PetActions.insert(std::make_pair(PC4_ELF, &g_PetActionStand));
		g_PetActions.insert(std::make_pair(PC4_TEST, &g_PetActionRound));
		g_PetActions.insert(std::make_pair(PC4_SATAN, &g_PetActionDemon));
		g_PetActions.insert(std::make_pair(XMAS_RUDOLPH, &g_PetActionCollecter));
		g_PetActions.insert(std::make_pair(PANDA, &g_PetActionCollecterAdd));
		g_PetActions.insert(std::make_pair(UNICORN, &g_PetActionUnicorn));
		g_PetActions.insert(std::make_pair(SKELETON, &g_PetActionCollecterSkeleton));
	}

	return true;
}

void PetProcess::Destroy()
{
	for (auto it = g_PetInfo.begin(); it != g_PetInfo.end(); ++it)
	{
		delete it->second;
	}
}

mu_boolean PetProcess::IsPet(mu_int32 key)
{
	return (g_PetActions.find(key) != g_PetActions.end());
}

MUPetAction *PetProcess::FindAction(mu_int32 key)
{
	auto iter = g_PetActions.find(key);

	if (iter != g_PetActions.end())
	{
		return iter->second;
	}

	return nullptr;
}

mu_boolean PetProcess::CreatePet(mu_int32 ItemType, mu_int32 ModelType, EVector3 &Position, CHARACTER *Owner, mu_int32 SubType, mu_int32 LinkBone)
{
	auto iter = g_PetInfo.find(ItemType);

	if (iter == g_PetInfo.end()) return false;

	MUAutoCriticalSpinLock lock(g_PetObjectsLock);

	mu_uint32 i;

	if (g_PetObjectsFreeCount > 0)
	{
		i = g_PetObjectsFree[--g_PetObjectsFreeCount];
	}
	else if (g_PetObjectsCurCount < MAX_PETS)
	{
		i = g_PetObjectsCurCount++;
	}
	else
	{
		return false;
	}

	MUPetObject *p = &g_PetObjects[i];

	p->Live = true;
	p->Create(ItemType, ModelType, Position, Owner, SubType, LinkBone);

	PetInfo *petInfo = iter->second;

	for (mu_uint32 n = 0; n < petInfo->Count; ++n)
	{
		p->SetActions((MUPetObject::ActionType)n, FindAction(petInfo->Actions[n]), petInfo->Speed[n]);
	}

	return true;
}

void PetProcess::DeletePet(CHARACTER *Owner, mu_int32 ItemType, mu_boolean AllDelete)
{
	if (Owner == nullptr)
	{
		return;
	}

	MUAutoCriticalSpinLock lock(g_PetObjectsLock);

	if (g_PetObjectsCurCount == 0)
	{
		return;
	}

	mu_uint32 i = 0;

REPEAT:
	mu_uint16 j = (mu_uint16)-1;

	for (; i < g_PetObjectsCurCount; ++i)
	{
		MUPetObject *b = &g_PetObjects[i];

		if (b->Live == true &&
			((ItemType == -1 && b->IsSameOwner(&Owner->Object) == true) ||
			b->IsSameObject(&Owner->Object, ItemType) == true))
		{
			j = i;
			break;
		}
	}

	if (j == (mu_uint16)-1) return;

	MUPetObject *b = &g_PetObjects[j];
	b->Live = false;
	b->Release();
	b->Initialize();

	g_PetObjectsFree[g_PetObjectsFreeCount++] = j;

	if (ItemType != -1 && AllDelete == true)
		goto REPEAT;
}