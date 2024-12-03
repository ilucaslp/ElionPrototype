#include "stdafx.h"

#if DIRECTX_BACKEND_ENABLE == 1
#include "mu_directx_pipeline_structs.h"

namespace EDirectXPipeline
{
	std::array<ID3D11BlendState*, EBlendStateMax> BlendStates = {{ nullptr }};
	std::array<ID3D11DepthStencilState*, EDepthStencilMax> DepthStencilStates = {{ nullptr }};
	std::array<ID3D11RasterizerState*, ERasterizerMax * 2> RasterizerStates = {{ nullptr }};
	std::array<ID3D11SamplerState*, ESamplerMax> SamplerStates = {{ nullptr }};
	EBlendState CurrentBlendState = EBlendState::eInvalid;
	EDepthStencil CurrentDepthStencil = EDepthStencil::eInvalid;
	mu_uint32 CurrentRasterizer = ECastEnum(ERasterizer::eInvalid);

	std::array<ESampler, DIRECTX_MAX_TEXTURES> CurrentVertexSamplers = { { ESampler::eInvalid } };
	std::array<const MUTexture*, DIRECTX_MAX_TEXTURES> CurrentVertexTextures = { {nullptr } };
	std::array<ESampler, DIRECTX_MAX_TEXTURES> CurrentFragmentSamplers = { { ESampler::eInvalid } };
	std::array<const MUTexture*, DIRECTX_MAX_TEXTURES> CurrentFragmentTextures = { {nullptr } };

	constexpr mu_uint32 MaxConstantBuffers = 4;
	std::array<ID3D11Buffer*, MaxConstantBuffers> VertexConstantBuffer = { { nullptr } };
	std::array<ID3D11Buffer*, MaxConstantBuffers> FragmentConstantBuffer = { { nullptr } };
	std::array<ID3D11Buffer*, MaxConstantBuffers> GeometryConstantBuffer = { { nullptr } };

	constexpr mu_uint32 MaxVertexBuffers = 4;
	std::array<ID3D11Buffer*, MaxVertexBuffers> VertexBuffers = { { nullptr } };
	std::array<mu_uint32, MaxVertexBuffers> VertexBuffersStrides = { { 0 } };
	std::array<mu_uint32, MaxVertexBuffers> VertexBuffersOffsets = { { 0 } };
	ID3D11Buffer *IndexBuffer = nullptr;
	DXGI_FORMAT IndexBufferFormat = DXGI_FORMAT_UNKNOWN;
	mu_uint32 IndexBufferOffset = 0;

	const mu_boolean Create()
	{
		HRESULT result;

		for (mu_uint32 n = 0; n < BlendStates.size(); ++n)
		{
			auto &data = BlendStateData[n];
			result = EDirectXInit::Device->CreateBlendState(&data, &BlendStates[n]);
			if (FAILED(result))
			{
				return false;
			}
		}

		for (mu_uint32 n = 0; n < DepthStencilStates.size(); ++n)
		{
			auto &data = DepthStencilData[n];
			result = EDirectXInit::Device->CreateDepthStencilState(&data, &DepthStencilStates[n]);
			if (FAILED(result))
			{
				return false;
			}
		}

		for (mu_uint32 n = 0; n < RasterizerStates.size(); ++n)
		{
			auto &data = RasterizerData[n];
			result = EDirectXInit::Device->CreateRasterizerState(&data, &RasterizerStates[n]);
			if (FAILED(result))
			{
				return false;
			}
		}

		for (mu_uint32 n = 0; n < SamplerStates.size(); ++n)
		{
			auto &data = SamplerData[n];
			result = EDirectXInit::Device->CreateSamplerState(&data, &SamplerStates[n]);
			if (FAILED(result))
			{
				return false;
			}
		}

		return true;
	}

	void Destroy()
	{
		for (mu_uint32 n = 0; n < BlendStates.size(); ++n)
		{
			EDirectXHelpers::Release(BlendStates[n]);
		}

		for (mu_uint32 n = 0; n < DepthStencilStates.size(); ++n)
		{
			EDirectXHelpers::Release(DepthStencilStates[n]);
		}

		for (mu_uint32 n = 0; n < RasterizerStates.size(); ++n)
		{
			EDirectXHelpers::Release(RasterizerStates[n]);
		}

		for (mu_uint32 n = 0; n < SamplerStates.size(); ++n)
		{
			EDirectXHelpers::Release(SamplerStates[n]);
		}
	}

	void CleanBindBuffers()
	{
		SetBlendState(EBlendState::eInvalid);
		SetDepthStencil(EDepthStencil::eInvalid);
		SetRasterizer(ERasterizer::eInvalid, false);

		FillArray(CurrentVertexSamplers, 0, CurrentVertexSamplers.size(), ESampler::eInvalid);
		ZeroArray(CurrentVertexTextures);
		FillArray(CurrentFragmentSamplers, 0, CurrentFragmentSamplers.size(), ESampler::eInvalid);
		ZeroArray(CurrentFragmentTextures);

		ZeroArray(VertexBuffers);
		ZeroArray(VertexBuffersStrides);
		ZeroArray(VertexBuffersOffsets);

		EDirectXInit::DeviceContext->IASetVertexBuffers(0, MaxVertexBuffers, VertexBuffers.data(), VertexBuffersStrides.data(), VertexBuffersOffsets.data());

		IndexBuffer = nullptr;
		IndexBufferFormat = DXGI_FORMAT_UNKNOWN;
		IndexBufferOffset = 0;
	}

	void SetBlendState(const EBlendState blendState)
	{
		if (CurrentBlendState != blendState)
		{
			if(blendState != EBlendState::eInvalid)
			{
				const mu_float blendFactor[4] = {1.0f, 1.0f, 1.0f, 1.0f};
				EDirectXInit::DeviceContext->OMSetBlendState(BlendStates[ECastEnum(blendState)], blendFactor, 0xFFFFFFFF);
			}

			CurrentBlendState = blendState;
		}
	}

	void SetDepthStencil(const EDepthStencil depthStencil)
	{
		if (CurrentDepthStencil != depthStencil)
		{
			if (depthStencil != EDepthStencil::eInvalid)
			{
				const mu_float blendFactor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
				EDirectXInit::DeviceContext->OMSetDepthStencilState(DepthStencilStates[ECastEnum(depthStencil)], 0);
			}

			CurrentDepthStencil = depthStencil;
		}
	}

	void SetRasterizer(const ERasterizer rasterizer, const mu_boolean enableScissor)
	{
		const mu_uint32 rasterizerIndex = rasterizer == ERasterizer::eInvalid ?
			ECastEnum(ERasterizer::eInvalid) :
			ECastEnum(rasterizer) + ERasterizerMax * static_cast<mu_uint32>(enableScissor);

		if (CurrentRasterizer != rasterizerIndex)
		{
			if (rasterizerIndex != ECastEnum(ERasterizer::eInvalid))
			{
				EDirectXInit::DeviceContext->RSSetState(RasterizerStates[rasterizerIndex]);
			}

			CurrentRasterizer = rasterizerIndex;
		}
	}

	void SetScissorArea(const mu_int32 x, const mu_int32 y, const mu_int32 width, const mu_int32 height)
	{
		D3D11_RECT ScissorRect;

		ScissorRect.left = x;
		ScissorRect.top = y;
		ScissorRect.right = x + width;
		ScissorRect.bottom = y + height;

		EDirectXInit::DeviceContext->RSSetScissorRects(1, &ScissorRect);
	}

	void SetVertexSampler(const mu_uint32 index, const ESampler sampler)
	{
		if (CurrentVertexSamplers[index] != sampler)
		{
			EDirectXInit::DeviceContext->VSSetSamplers(index, 1, &SamplerStates[ECastEnum(sampler)]);
			CurrentVertexSamplers[index] = sampler;
		}
	}

	void SetVertexSampler(const mu_uint32 baseIndex, const mu_uint32 count, const ESampler *sampler)
	{
		std::array<ID3D11SamplerState*, DIRECTX_MAX_TEXTURES> states = {{nullptr}};

		mu_uint32 n = 0;
		for (; n < count; ++n)
		{
			if (CurrentVertexSamplers[baseIndex + n] != sampler[n])
			{
				const mu_uint32 maxJ = count - n;
				for (mu_uint32 j = 0;j < maxJ; ++j)
				{
					states[j] = SamplerStates[ECastEnum(sampler[n + j])];
				}
				break;
			}
		}

		if (n < count)
		{
			EDirectXInit::DeviceContext->VSSetSamplers(baseIndex + n, count - n, states.data());
			mu_memcpy(&CurrentVertexSamplers[baseIndex + n], &sampler[n], sizeof(ESampler) * (count - n));
		}
	}

	void SetFragmentSampler(const mu_uint32 index, const ESampler sampler)
	{
		if (CurrentFragmentSamplers[index] != sampler)
		{
			EDirectXInit::DeviceContext->PSSetSamplers(index, 1, &SamplerStates[ECastEnum(sampler)]);
			CurrentFragmentSamplers[index] = sampler;
		}
	}

	void SetFragmentSampler(const mu_uint32 baseIndex, const mu_uint32 count, const ESampler *sampler)
	{
		std::array<ID3D11SamplerState*, DIRECTX_MAX_TEXTURES> states = { {nullptr} };

		mu_uint32 n = 0;
		for (; n < count; ++n)
		{
			if (CurrentFragmentSamplers[baseIndex + n] != sampler[n])
			{
				const mu_uint32 maxJ = count - n;
				for (mu_uint32 j = 0; j < maxJ; ++j)
				{
					states[j] = SamplerStates[ECastEnum(sampler[n + j])];
				}
				break;
			}
		}

		if (n < count)
		{
			EDirectXInit::DeviceContext->PSSetSamplers(baseIndex + n, count - n, states.data());
			mu_memcpy(&CurrentFragmentSamplers[baseIndex + n], &sampler[n], sizeof(ESampler) * (count - n));
		}
	}

	void SetVertexTexture(const mu_uint32 index, const MUTexture *texture)
	{
		if (CurrentVertexTextures[index] != texture)
		{
			EDirectXInit::DeviceContext->VSSetShaderResources(index, 1, &texture->DirectXInternal.ShaderView);
			CurrentVertexTextures[index] = texture;
		}
	}

	void SetVertexTexture(const mu_uint32 baseIndex, const mu_uint32 count, const MUTexture **textures)
	{
		std::array<ID3D11ShaderResourceView*, DIRECTX_MAX_TEXTURES> states = { {nullptr} };

		mu_uint32 n = 0;
		for (; n < count; ++n)
		{
			if (CurrentVertexTextures[baseIndex + n] != textures[n])
			{
				const mu_uint32 maxJ = count - n;
				for (mu_uint32 j = 0; j < maxJ; ++j)
				{
					states[j] = textures[n + j]->DirectXInternal.ShaderView;
				}
				break;
			}
		}

		if (n < count)
		{
			EDirectXInit::DeviceContext->VSSetShaderResources(baseIndex + n, count - n, states.data());
			mu_memcpy(&CurrentVertexTextures[baseIndex + n], &textures[n], sizeof(MUTexture*) * (count - n));
		}
	}

	void SetFragmentTexture(const mu_uint32 index, const MUTexture *texture)
	{
		if (CurrentFragmentTextures[index] != texture)
		{
			EDirectXInit::DeviceContext->PSSetShaderResources(index, 1, &texture->DirectXInternal.ShaderView);
			CurrentFragmentTextures[index] = texture;
		}
	}

	void SetFragmentTexture(const mu_uint32 baseIndex, const mu_uint32 count, const MUTexture **textures)
	{
		std::array<ID3D11ShaderResourceView*, DIRECTX_MAX_TEXTURES> states = { {nullptr} };

		mu_uint32 n = 0;
		for (; n < count; ++n)
		{
			if (CurrentFragmentTextures[baseIndex + n] != textures[n])
			{
				const mu_uint32 maxJ = count - n;
				for (mu_uint32 j = 0; j < maxJ; ++j)
				{
					states[j] = textures[n + j]->DirectXInternal.ShaderView;
				}
				break;
			}
		}

		if (n < count)
		{
			EDirectXInit::DeviceContext->PSSetShaderResources(baseIndex + n, count - n, states.data());
			mu_memcpy(&CurrentFragmentTextures[baseIndex + n], &textures[n], sizeof(MUTexture*) * (count - n));
		}
	}

	void SetConstantBuffers(const mu_uint32 index, const mu_uint32 count, ID3D11Buffer * const *buffers, const mu_uint32 mask)
	{
		if ((mask & D3D_CONSTANT_MASK_VS) != 0)
		{
			mu_uint32 n = 0;
			for (;n < count;++n)
			{
				if (VertexConstantBuffer[index + n] != buffers[n])
				{
					break;
				}
			}

			if (n < count)
			{
				const mu_uint32 cnt = count - n;
				mu_memcpy(&VertexConstantBuffer[index + n], &buffers[n], sizeof(ID3D11Buffer*) * cnt);
				EDirectXInit::DeviceContext->VSSetConstantBuffers(index + n, cnt, &buffers[n]);
			}
		}

		if ((mask & D3D_CONSTANT_MASK_PS) != 0)
		{
			mu_uint32 n = 0;
			for (; n < count; ++n)
			{
				if (FragmentConstantBuffer[index + n] != buffers[n])
				{
					break;
				}
			}

			if (n < count)
			{
				const mu_uint32 cnt = count - n;
				mu_memcpy(&FragmentConstantBuffer[index + n], &buffers[n], sizeof(ID3D11Buffer*) * cnt);
				EDirectXInit::DeviceContext->PSSetConstantBuffers(index + n, cnt, &buffers[n]);
			}
		}

		if ((mask & D3D_CONSTANT_MASK_GS) != 0)
		{
			mu_uint32 n = 0;
			for (; n < count; ++n)
			{
				if (GeometryConstantBuffer[index + n] != buffers[n])
				{
					break;
				}
			}

			if (n < count)
			{
				const mu_uint32 cnt = count - n;
				mu_memcpy(&GeometryConstantBuffer[index + n], &buffers[n], sizeof(ID3D11Buffer*) * cnt);
				EDirectXInit::DeviceContext->GSSetConstantBuffers(index + n, cnt, &buffers[n]);
			}
		}
	}

	void SetVertexBuffers(const mu_uint32 index, const mu_uint32 count, ID3D11Buffer * const *buffers, const mu_uint32 *strides, const mu_uint32 *offsets)
	{
		mu_uint32 n = 0;
		for (; n < count; ++n)
		{
			if (buffers[n] != VertexBuffers[index + n] ||
				strides[n] != VertexBuffersStrides[index + n] ||
				offsets[n] != VertexBuffersOffsets[index + n])
			{
				break;
			}
		}

		if (n < count)
		{
			const mu_uint32 cnt = count - n;
			mu_memcpy(&VertexBuffers[index + n], &buffers[n], sizeof(ID3D11Buffer*) * cnt);
			mu_memcpy(&VertexBuffersStrides[index + n], &strides[n], sizeof(mu_uint32) * cnt);
			mu_memcpy(&VertexBuffersOffsets[index + n], &offsets[n], sizeof(mu_uint32) * cnt);

			EDirectXInit::DeviceContext->IASetVertexBuffers(index + n, cnt, &buffers[n], &strides[n], &offsets[n]);
		}
	}

	void SetIndexBuffer(ID3D11Buffer *buffer, const DXGI_FORMAT format, const mu_uint32 offset)
	{
		if (IndexBuffer != buffer ||
			IndexBufferFormat != format ||
			IndexBufferOffset != offset)
		{
			IndexBuffer = buffer;
			IndexBufferFormat = format;
			IndexBufferOffset = offset;

			EDirectXInit::DeviceContext->IASetIndexBuffer(buffer, format, offset);
		}
	}
};
#endif