#ifndef __MU_OPENGL_PIPELINE_H__
#define __MU_OPENGL_PIPELINE_H__

#pragma once

class MUTexture;

namespace EOpenGLPipeline
{
	const mu_boolean Create();
	void Destroy();

	void CleanBindBuffers();

#if !defined(GL_ES_VERSION_3_0)
	void SetMultiSample(const mu_boolean enable);
#endif
	void SetBlendState(const EBlendState blendState);
	void SetDepthStencil(const EDepthStencil depthStencil);
	void SetRasterizer(const ERasterizer rasterizer, const mu_boolean enableScissor);
	void SetScissorArea(const mu_int32 x, const mu_int32 y, const mu_int32 width, const mu_int32 height);
	void SetVertexSampler(const mu_uint32 index, const ESampler sampler, const mu_boolean useMipmap);
	void SetVertexSampler(const mu_uint32 baseIndex, const mu_uint32 count, const ESampler *sampler, const mu_boolean *useMipmap);
	void SetFragmentSampler(const mu_uint32 index, const ESampler sampler, const mu_boolean useMipmap);
	void SetFragmentSampler(const mu_uint32 baseIndex, const mu_uint32 count, const ESampler *sampler, const mu_boolean *useMipmap);
	void SetActiveTexture(const GLenum index);
	void SetUploadTexture(const MUTexture *texture);
	void SetVertexTexture(const mu_uint32 index, const MUTexture *texture);
	void SetVertexTexture(const mu_uint32 baseIndex, const mu_uint32 count, const MUTexture **textures);
	void SetFragmentTexture(const mu_uint32 index, const MUTexture *texture);
	void SetFragmentTexture(const mu_uint32 baseIndex, const mu_uint32 count, const MUTexture **textures);

	void BindVAO(const GLuint VAO);
};

#endif