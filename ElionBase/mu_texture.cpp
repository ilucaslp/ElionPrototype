#include "stdafx.h"
#include "mu_texture.h"

const mu_uint8 MUTexture::CryptKey[] = { 0x3a, 0xf7, 0xa3, 0xf6, 0xd3, 0x2a, 0x05, 0x49, 0x6a, 0xa5, 0x0f, 0x27, 0x48, 0x07, 0x11, 0x18, 0x23, 0x09, 0x82, 0x96, 0x4f, 0xd9, 0x91, 0x3d, 0xc4, 0x81, 0x5c, 0xfd, 0xfe, 0xcd, 0xec, 0x77, 0x86, 0xf2, 0xff, 0x36, 0x10, 0x6f, 0xbf, 0x70, 0x8b, 0x8f, 0x61, 0xea, 0x01, 0xf1, 0x18, 0x39, 0x69, 0xb0, 0xd0, 0x22, 0x27, 0x5f, 0xaf, 0x00 };
MUBlowfish MUTexture::Crypt(CryptKey, mu_countof(CryptKey));

MUTexture::MUTexture() :
	ETextureBackend(),
	_IsHeaderLoaded(false),
	_IsDataLoaded(false),
	_IsPendingLoaded(false),
	_Index(TEXTURE::INVALID_TEXTURE),
	_DataSize(0),
	_ReferenceCount(0)
{
}

MUTexture::~MUTexture()
{
	Release();
}

void MUTexture::Release(const mu_boolean dataOnly)
{
	ETextureBackend::ReleaseTexture();

	_IsDataLoaded = false;
	_IsPendingLoaded = false;

	if (dataOnly == false)
	{
		_IsHeaderLoaded = false;
	}
}

mu_boolean MUTexture::LoadHeaderFromFile(const mu_text *filename)
{
	if (_IsHeaderLoaded == true)
	{
		return true;
	}

	mu_string headerPath(filename);
	headerPath += _T("h");

	SDL_RWops *file = nullptr;

	if (mu_rwfromfile(&file, headerPath.c_str(), "rb") == false)
	{
		return false;
	}

	static const mu_size HeaderSize = FileHeaders::HEADER_SIZE;

	mu_int64 fileLength = SDL_RWsize(file);
	mu_int64 encryptedLength = fileLength - HeaderSize;

	if (encryptedLength % MUBlowfish::BytesLength != 0)
	{
		return false;
	}

	std::vector<mu_uint8> dataBuffer(fileLength);
	SDL_RWread(file, (mu_char*)dataBuffer.data(), fileLength, 1);
	SDL_RWclose(file);

	FileHeaders::FileHeader *header = (FileHeaders::FileHeader*)dataBuffer.data();

	if (header->Signature != FILE_HEADERS::TEXTURE_HEADER)
	{
		return false;
	}

	if (GenerateCRC32(dataBuffer.data() + HeaderSize, encryptedLength) != header->CRC)
	{
		return false;
	}

	Crypt.Decode(dataBuffer.data() + HeaderSize, dataBuffer.data() + HeaderSize, encryptedLength);

	Texture::EncryptedHeader *data = (Texture::EncryptedHeader*)(dataBuffer.data() + HeaderSize);

	TextureInternal.Components = data->Components;
	TextureInternal.Depth = data->ArraySize;
	_DataSize = data->Size;

	TextureInternal.Width = data->Width;
	TextureInternal.Height = data->Height;
	TextureInternal.Sampler = TextureEnumToSamplers[data->Sampler];

	_IsHeaderLoaded = true;

	return true;
}

mu_boolean MUTexture::LoadFromFile(const mu_text *filename, const EImageViewType imageViewType)
{
	if (LoadHeaderFromFile(filename) == false)
	{
		return false;
	}

	SDL_RWops *file = nullptr;

	if (mu_rwfromfile(&file, filename, "rb") == false)
	{
		return false;
	}

	static const mu_size HeaderSize = FileHeaders::HEADER_SIZE;

	mu_int64 fileLength = SDL_RWsize(file);
	mu_int64 encryptedLength = fileLength - HeaderSize;

	if (encryptedLength % MUBlowfish::BytesLength != 0)
	{
		return false;
	}

	std::vector<mu_uint8> dataBuffer(fileLength);
	SDL_RWread(file, (mu_char*)dataBuffer.data(), fileLength, 1);
	SDL_RWclose(file);

	FileHeaders::FileHeader *header = (FileHeaders::FileHeader*)dataBuffer.data();

	if (header->Signature != FILE_HEADERS::TEXTURE_HEADER)
	{
		return false;
	}

	if (GenerateCRC32(dataBuffer.data() + HeaderSize, encryptedLength) != header->CRC)
	{
		return false;
	}

	Crypt.Decode(dataBuffer.data() + HeaderSize, dataBuffer.data() + HeaderSize, encryptedLength);

	if (LoadFromBuffer(dataBuffer.data() + HeaderSize, _DataSize, imageViewType) == false)
	{
		return false;
	}

	TextureInternal.Loaded = true;

	return true;
}

mu_boolean MUTexture::LoadFromFileEx(const mu_text *filename)
{
	if (IsLoaded() == true ||
		IsPendingLoaded() == true)
	{
		return true;
	}

	SDL_RWops *file = nullptr;

	if (mu_rwfromfile(&file, filename, "rb") == false)
	{
		return false;
	}

	static const mu_size HeaderSize = FileHeaders::HEADER_SIZE;

	mu_int64 fileLength = SDL_RWsize(file);
	mu_int64 encryptedLength = fileLength - HeaderSize;

	if (encryptedLength % MUBlowfish::BytesLength != 0)
	{
		return false;
	}

	mu_uint8 *dataBuffer = (mu_uint8*)mu_malloc(fileLength);
	SDL_RWread(file, (mu_char*)dataBuffer, fileLength, 1);
	SDL_RWclose(file);

	FileHeaders::FileHeader *header = (FileHeaders::FileHeader*)dataBuffer;

	if (header->Signature != FILE_HEADERS::TEXTURE_HEADER)
	{
		return false;
	}

	if (GenerateCRC32(dataBuffer + HeaderSize, encryptedLength) != header->CRC)
	{
		return false;
	}

	Crypt.Decode(dataBuffer + HeaderSize, dataBuffer + HeaderSize, encryptedLength);

	// Critical Area
	{
		MURWLockShared lock(g_GPUDataLock);
		mu_uint32 gpuQueueIndex = g_GPUQueueIndex;
		mu_uint32 gpuDataIndex = g_GPUDataCount[gpuQueueIndex]++;
		if (gpuDataIndex >= GPUDATA_SIZE)
		{
			DEBUG_BREAK();
		}

		MULoadingGPUData &gpuData = g_GPUData[gpuQueueIndex][gpuDataIndex];
		gpuData.DataType = GPU_DATATYPE::GPUDT_TEXTURE;
		gpuData.Object = this;
		gpuData.Data[0] = dataBuffer;
		gpuData.DataSize[0] = fileLength;
	}

	_IsPendingLoaded = true;

	return true;
}

mu_boolean MUTexture::TransferCPUtoGPU(const mu_uint8 *dataBuffer, const mu_size dataSize)
{
	constexpr mu_size HeaderSize = FileHeaders::HEADER_SIZE;

	if(LoadFromBuffer(dataBuffer + HeaderSize, _DataSize) == false)
	{
		return false;
	}

	TextureInternal.Loaded = true;

	return true;
}