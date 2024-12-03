#ifndef __MU_DIRECTX_PIPELINE_H__
#define __MU_DIRECTX_PIPELINE_H__

#pragma once

enum
{
	D3D_CONSTANT_MASK_VS = (1 << 0),
	D3D_CONSTANT_MASK_PS = (1 << 1),
	D3D_CONSTANT_MASK_GS = (1 << 2),
};

namespace EDirectXPipeline
{
	const mu_boolean Create();
	void Destroy();

	void CleanBindBuffers();

	void SetBlendState(const EBlendState blendState);
	void SetDepthStencil(const EDepthStencil depthStencil);
	void SetRasterizer(const ERasterizer rasterizer, const mu_boolean enableScissor);
	void SetScissorArea(const mu_int32 x, const mu_int32 y, const mu_int32 width, const mu_int32 height);
	void SetVertexSampler(const mu_uint32 index, const ESampler sampler);
	void SetVertexSampler(const mu_uint32 baseIndex, const mu_uint32 count, const ESampler *sampler);
	void SetFragmentSampler(const mu_uint32 index, const ESampler sampler);
	void SetFragmentSampler(const mu_uint32 baseIndex, const mu_uint32 count, const ESampler *sampler);
	void SetVertexTexture(const mu_uint32 index, const MUTexture *texture);
	void SetVertexTexture(const mu_uint32 baseIndex, const mu_uint32 count, const MUTexture **textures);
	void SetFragmentTexture(const mu_uint32 index, const MUTexture *texture);
	void SetFragmentTexture(const mu_uint32 baseIndex, const mu_uint32 count, const MUTexture **textures);

	void SetConstantBuffers(const mu_uint32 index, const mu_uint32 count, ID3D11Buffer * const *buffers, const mu_uint32 mask);
	void SetVertexBuffers(const mu_uint32 index, const mu_uint32 count, ID3D11Buffer * const *buffers, const mu_uint32 *strides, const mu_uint32 *offsets);
	void SetIndexBuffer(ID3D11Buffer *buffer, const DXGI_FORMAT format, const mu_uint32 offset);
};

#endif