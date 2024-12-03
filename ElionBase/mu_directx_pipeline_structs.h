#ifndef __MU_DIRECTX_PIPELINE_STRUCTS_H__
#define __MU_DIRECTX_PIPELINE_STRUCTS_H__

#pragma once

namespace EDirectXPipeline
{
	extern D3D11_BLEND_DESC BlendStateData[EBlendStateMax];
	extern D3D11_DEPTH_STENCIL_DESC DepthStencilData[EDepthStencilMax];
	extern D3D11_RASTERIZER_DESC RasterizerData[ERasterizerMax * 2];
	extern D3D11_SAMPLER_DESC SamplerData[ESamplerMax];
};

#endif