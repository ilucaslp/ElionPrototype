#include "stdafx.h"

#if DIRECTX_BACKEND_ENABLE == 1
namespace EDirectXBuffer
{
	const mu_boolean Allocate(const mu_size Size, const EBufferUsage Usage, const EBufferType Type, const void *DataBuffer, EBufferBackend *Buffer)
	{
		D3D11_BUFFER_DESC bufferDesc = {};

		switch (Type)
		{
		case EBufferType::eUniform:
			{
				bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			}
			break;

		case EBufferType::eVertex:
		case EBufferType::eInstance:
			{
				bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			}
			break;

		case EBufferType::eIndex:
			{
				bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			}
			break;

		default: break;
		}

		switch (Usage)
		{
		case EBufferUsage::eStatic:
			{
				bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
			}
			break;

		case EBufferUsage::eDynamic:
			{
				if (Type == EBufferType::eUniform)
				{
					bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
					bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
				}
				else
				{
					bufferDesc.Usage = D3D11_USAGE_DEFAULT;
				}
			}
			break;

		case EBufferUsage::eStream:
			{
				bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
				bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			}
			break;
		};

		bufferDesc.ByteWidth = Size;

		HRESULT result;
		if (bufferDesc.Usage == D3D11_USAGE_IMMUTABLE ||
			DataBuffer != nullptr)
		{
			D3D11_SUBRESOURCE_DATA initialData = {};
			initialData.pSysMem = DataBuffer;

			result = EDirectXInit::Device->CreateBuffer(&bufferDesc, &initialData, &Buffer->DirectXInternal.Resource);
		}
		else
		{
			result = EDirectXInit::Device->CreateBuffer(&bufferDesc, nullptr, &Buffer->DirectXInternal.Resource);
		}

		if (FAILED(result))
		{
			return false;
		}

		if (Usage != EBufferUsage::eStatic)
		{
			Buffer->DirectXInternal.MappedMemory = mu_malloc(Size);
		}

		Buffer->DirectXInternal.Usage = bufferDesc.Usage;

		return true;
	}

	void Deallocate(EBufferBackend *Buffer)
	{
		if (Buffer->DirectXInternal.MappedMemory != nullptr)
		{
			mu_free(Buffer->DirectXInternal.MappedMemory);
			Buffer->DirectXInternal.MappedMemory = nullptr;
		}

		EDirectXHelpers::Release(Buffer->DirectXInternal.Resource);
	}

	void Flush(const mu_size StagingOffset, const mu_size BufferOffset, const mu_size Size, EBufferBackend *Buffer)
	{
		if (Size == 0) return;

		if (Buffer->DirectXInternal.Usage == D3D11_USAGE_DEFAULT)
		{
			D3D11_BOX destBox = { static_cast<const mu_uint32>(BufferOffset), 0, 0, static_cast<const mu_uint32>(BufferOffset + Size), 1, 1 };
			EDirectXInit::DeviceContext->UpdateSubresource(Buffer->DirectXInternal.Resource, 0, &destBox, reinterpret_cast<mu_uint8*>(Buffer->DirectXInternal.MappedMemory) + StagingOffset, 0, 0);
		}
		else
		{
			D3D11_MAPPED_SUBRESOURCE mappedResource = {};
			HRESULT result = EDirectXInit::DeviceContext->Map(Buffer->DirectXInternal.Resource, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			if (FAILED(result))
			{
				return;
			}

			mu_memcpy(reinterpret_cast<mu_uint8*>(mappedResource.pData) + BufferOffset, reinterpret_cast<mu_uint8*>(Buffer->DirectXInternal.MappedMemory) + StagingOffset, Size);

			EDirectXInit::DeviceContext->Unmap(Buffer->DirectXInternal.Resource, 0);
		}
	}

	const mu_boolean AllocateStaging(const mu_size Size, const mu_boolean ForceAllocate, EStagingBufferBackend *StagingBuffer)
	{
		StagingBuffer->BufferInternal.MappedMemory = mu_malloc(Size);
		if (StagingBuffer->BufferInternal.MappedMemory == nullptr)
		{
			return false;
		}

		return true;
	}

	void DeallocateStaging(EStagingBufferBackend *StagingBuffer)
	{
		if (StagingBuffer->BufferInternal.MappedMemory != nullptr)
		{
			mu_free(StagingBuffer->BufferInternal.MappedMemory);
			StagingBuffer->BufferInternal.MappedMemory = nullptr;
		}
	}
};
#endif