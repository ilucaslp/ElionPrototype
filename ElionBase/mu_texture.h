#ifndef __MU_TEXTURE_H__
#define __MU_TEXTURE_H__

#pragma once

namespace Texture
{
	/*
	Structure:
	Texture File(etex):
		[Header Signature] (4 bytes) (uint)
		[CRC32] (4 bytes) (uint)
		[Encrypted Only Data] (aligned 8 bytes)
			(Begin)
				[Texture Data Size] (4 bytes) (uint)
				[Texture Width] (4 bytes) (uint)
				[Texture Height] (4 bytes) (uint)
				[Texture Data] (1 bytes array)[] (uint8)
			(End)
	*/

	MU_ALIGN(8, struct) EncryptedHeader
	{
		mu_uint32 Size;
		mu_uint16 Width;
		mu_uint16 Height;
		mu_uint16 ArraySize;
		mu_uint16 Sampler;
		mu_uint8 Components;
	};
};

class ETextureRenderProxy
{
public:
	ETextureRenderProxy()
	{
		ClearInstancesModesCount();
	}

public:
	FORCEINLINE void ClearInstancesModesCount()
	{
		ModeEnabled = 0;
		mu_zeromem(ModeCount, sizeof(ModeCount));
	}

	template<std::memory_order order = std::memory_order_seq_cst>
	FORCEINLINE const mu_uint32 RequestInstanceIndex(const mu_uint32 type, const mu_uint32 count)
	{
		const mu_uint32 c = ModeCount[type + 1];
		ModeCount[type + 1] += count;
		return c;
	}

	FORCEINLINE const mu_uint32 RegisterInstancesMode(const mu_uint32 flag)
	{
		const mu_uint32 m = ModeEnabled;
		ModeEnabled |= flag;
		return m;
	}

	FORCEINLINE const mu_uint32 ExchangeInstancesModeCount(const mu_uint32 type, const mu_uint32 count)
	{
		const mu_uint32 c = ModeCount[type + 1];
		ModeCount[type + 1] += count;
		return c;
	}

	FORCEINLINE void RegisterInstancesBase(const mu_uint32 base)
	{
		InstancesBase = base;
	}

	FORCEINLINE const mu_uint32 GetInstancesModeBase(const mu_uint32 type)
	{
		return InstancesBase + ModeCount[type];
	}

	FORCEINLINE const mu_uint32 GetInstancesModeCount(const mu_uint32 type)
	{
		return ModeCount[type + 1] - ModeCount[type];
	}

protected:
	friend class MUTasking;
	mu_uint32 InstancesBase;
	mu_uint32 ModeEnabled;
	mu_uint16 ModeCount[BLEND::TXB_MAX + 1];
};

class MUTexture : public ETextureBackend
{
private:
	static const mu_uint8 CryptKey[];
	static MUBlowfish Crypt;

public:
	MUTexture();
	~MUTexture();

	void Release(const mu_boolean dataOnly = true);

	mu_boolean LoadHeaderFromFile(const mu_text *filename);
	mu_boolean LoadFromFile(const mu_text *filename, const EImageViewType imageViewType);
	mu_boolean LoadFromFileEx(const mu_text *filename);
	mu_boolean TransferCPUtoGPU(const mu_uint8 *dataBuffer, const mu_size dataSize);

public:
	EINLINE void IncreaseReference()
	{
		++_ReferenceCount;
	}

	EINLINE mu_boolean DecreaseReference()
	{
		return --_ReferenceCount == 0;
	}

	EINLINE const mu_boolean IsPendingLoaded() const
	{
		return _IsPendingLoaded;
	}

public:
	mu_boolean _IsHeaderLoaded;
	mu_boolean _IsDataLoaded;
	mu_boolean _IsPendingLoaded;

	mu_uint32 _Index;
	mu_uint32 _DataSize;

	mu_uint32 _ReferenceCount;
};

#endif