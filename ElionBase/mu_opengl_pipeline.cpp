#include "stdafx.h"

#if OPENGL_BACKEND_ENABLE == 1
#include "mu_opengl_pipeline_structs.h"

namespace EOpenGLPipeline
{
	std::array<GLuint, ESamplerMax * 2> Samplers;

	GLuint CurrentVAO = EInvalidUInt32;
	EBlendState CurrentBlendState = EBlendState::eInvalid;
	EDepthStencil CurrentDepthStencil = EDepthStencil::eInvalid;
	ERasterizer CurrentRasterizer = ERasterizer::eInvalid;
	std::array<ESampler, OPENGL_MAX_TEXTURES> CurrentVertexSamplers = { { ESampler::eInvalid } };
	std::array<mu_boolean, OPENGL_MAX_TEXTURES> CurrentVertexSamplersMipmap = { { false } };
	std::array<ESampler, OPENGL_MAX_TEXTURES> CurrentFragmentSamplers = { { ESampler::eInvalid } };
	std::array<mu_boolean, OPENGL_MAX_TEXTURES> CurrentFragmentSamplersMipmap = { { false } };
	std::array<const MUTexture*, OPENGL_MAX_TEXTURES> CurrentVertexTextures = { { nullptr } };
	std::array<const MUTexture*, OPENGL_MAX_TEXTURES> CurrentFragmentTextures = { { nullptr } };
	GLenum CurrentActiveTexture = OPENGL_INVALID_ENUM;

	mu_boolean MultiSample = false;
	mu_boolean DepthEnabled = true;
	mu_boolean DepthMask = true;
	GLenum DepthFunc = GL_LESS;
	mu_boolean StencilEnable = false;
	GLenum CullFace = GL_BACK;
	GLenum FrontFace = GL_CW;
	mu_boolean ScissorEnable = false;

	const mu_boolean Create()
	{
		// Blend State
		glEnable(GL_BLEND);

		// Depth Stencil
		glEnable(GL_DEPTH_TEST);
		glDepthMask(DepthMask);
		glDepthFunc(DepthFunc);
		glDisable(GL_STENCIL_TEST);

		// Rasterizer
		glEnable(GL_CULL_FACE);
		glCullFace(CullFace);
		glFrontFace(FrontFace);
		glDisable(GL_SCISSOR_TEST);

		// Sampler
		glGenSamplers(Samplers.size(), Samplers.data());

		const mu_boolean anisotropyFilter = EOpenGLExtensions::IsFeatureSupported(OpenGLFeatures::AnisotropyFilter);
		const mu_float maxAnisotropy = EOpenGLExtensions::GetMaxAnisotropy();
		for(mu_uint32 n = 0;n < Samplers.size();++n)
		{
			ESamplerData &data = SamplerData[n];

			const mu_uint32 realIndex = n < ESamplerMax ? n : n - ESamplerMax;
			if(anisotropyFilter == true &&
				realIndex >= static_cast<mu_uint32>(ESampler::eBeginAnisotropy) &&
				realIndex <= static_cast<mu_uint32>(ESampler::eEndAnisotropy))
			{
				glSamplerParameterf(Samplers[n], GL_TEXTURE_MAX_ANISOTROPY, maxAnisotropy);
			}

			glSamplerParameteri(Samplers[n], GL_TEXTURE_MIN_FILTER, data.MinMapFilter);
			glSamplerParameteri(Samplers[n], GL_TEXTURE_MAG_FILTER, data.MagFilter);
			glSamplerParameteri(Samplers[n], GL_TEXTURE_WRAP_S, data.AddressU);
			glSamplerParameteri(Samplers[n], GL_TEXTURE_WRAP_T, data.AddressV);
			glSamplerParameteri(Samplers[n], GL_TEXTURE_WRAP_R, data.AddressW);
			glSamplerParameteri(Samplers[n], GL_TEXTURE_COMPARE_MODE, data.CompareMode);
			glSamplerParameteri(Samplers[n], GL_TEXTURE_COMPARE_FUNC, data.CompareFunc);
		}

		return true;
	}

	void Destroy()
	{
		glDeleteSamplers(Samplers.size(), Samplers.data());
	}

	void CleanBindBuffers()
	{
		EOpenGLShader::SetProgramShader(ECombinedShader::eInvalid);

		CurrentBlendState = EBlendState::eInvalid;

		CurrentDepthStencil = EDepthStencil::eInvalid;
		DepthEnabled = true;
		glEnable(GL_BLEND);
		DepthMask = true;
		DepthFunc = GL_LESS;
		StencilEnable = false;
		glEnable(GL_DEPTH_TEST);
		glDepthMask(DepthMask);
		glDepthFunc(DepthFunc);
		glDisable(GL_STENCIL_TEST);

		CurrentRasterizer = ERasterizer::eInvalid;
		CullFace = GL_BACK;
		FrontFace = GL_CW;
		ScissorEnable = false;
		glEnable(GL_CULL_FACE);
		glCullFace(CullFace);
		glFrontFace(FrontFace);
		glDisable(GL_SCISSOR_TEST);

		FillArray(CurrentVertexSamplers, 0, CurrentVertexSamplers.size(), ESampler::eInvalid);
		FillArray(CurrentFragmentSamplers, 0, CurrentFragmentSamplers.size(), ESampler::eInvalid);
		CurrentActiveTexture = OPENGL_INVALID_ENUM;
		ZeroArray(CurrentVertexTextures);
		ZeroArray(CurrentFragmentTextures);

		glBindVertexArray(0);
		CurrentVAO = EInvalidUInt32;
	}

#if !defined(GL_ES_VERSION_3_0)
	void SetMultiSample(const mu_boolean enable)
	{
		if (MultiSample != enable)
		{
			if (enable == true)
			{
				glEnable(GL_MULTISAMPLE);
			}
			else
			{
				glDisable(GL_MULTISAMPLE);
			}
			MultiSample = enable;
		}
	}
#endif

	void SetBlendState(const EBlendState blendState)
	{
		if(CurrentBlendState != blendState)
		{
			EBlendStateData &data = BlendStateData[static_cast<mu_uint32>(blendState)];

			glBlendEquationSeparate(data.RGBMode, data.AlphaMode);
			glBlendFuncSeparate(data.RGBSrc, data.RGBDest, data.AlphaSrc, data.AlphaDest);

			CurrentBlendState = blendState;
		}
	}

	void SetDepthStencil(const EDepthStencil depthStencil)
	{
		if(CurrentDepthStencil != depthStencil)
		{
			EDepthStencilData &data = DepthStencilData[static_cast<mu_uint32>(depthStencil)];

			if(DepthEnabled != data.DepthEnable)
			{
				if(data.DepthEnable == true)
				{
					glEnable(GL_DEPTH_TEST);
				}
				else
				{
					glDisable(GL_DEPTH_TEST);
				}
				DepthEnabled = data.DepthEnable;
			}

			if(DepthMask != data.DepthMask)
			{
				glDepthMask(data.DepthMask);
				DepthMask = data.DepthMask;
			}

			if (DepthFunc != data.DepthFunc)
			{
				glDepthFunc(data.DepthFunc);
				DepthFunc = data.DepthFunc;
			}

			if (StencilEnable != data.StencilEnable)
			{
				if (data.StencilEnable == true)
				{
					glEnable(GL_STENCIL_TEST);
				}
				else
				{
					glDisable(GL_STENCIL_TEST);
				}
				StencilEnable = data.StencilEnable;
			}

			CurrentDepthStencil = depthStencil;
		}
	}

	void SetRasterizer(const ERasterizer rasterizer, const mu_boolean enableScissor)
	{
		if(CurrentRasterizer != rasterizer)
		{
			ERasterizerData &data = RasterizerData[static_cast<mu_uint32>(rasterizer)];

			if (CullFace != data.CullFace)
			{
				if (data.CullFace == GL_NONE)
				{
					glDisable(GL_CULL_FACE);
				}
				else
				{
					if (data.CullFace != GL_NONE)
					{
						glEnable(GL_CULL_FACE);
					}

					glCullFace(data.CullFace);
				}

				CullFace = data.CullFace;
			}

			if (FrontFace != data.FrontFace)
			{
				glFrontFace(data.FrontFace);
				FrontFace = data.FrontFace;
			}

			CurrentRasterizer = rasterizer;
		}

		if (ScissorEnable != enableScissor)
		{
			if (enableScissor == false)
			{
				glDisable(GL_SCISSOR_TEST);
			}
			else
			{
				glEnable(GL_SCISSOR_TEST);
			}
			ScissorEnable = enableScissor;
		}
	}

	void SetScissorArea(const mu_int32 x, const mu_int32 y, const mu_int32 width, const mu_int32 height)
	{
		const mu_float ry = (mu_float)MU_RESOURCE->GetWindowHeight() - y - height;
		glScissor(x, ry, width, height);
	}

	void SetVertexSampler(const mu_uint32 index, const ESampler sampler, const mu_boolean useMipmap)
	{
		if(CurrentVertexSamplers[index] != sampler ||
			CurrentVertexSamplersMipmap[index] != useMipmap)
		{
			const mu_uint32 samplerIndex = ESamplerMax * static_cast<mu_uint32>(useMipmap) + static_cast<mu_uint32>(sampler);
			glBindSampler(OPENGL_VERTEX_TEXTUREBASE + index, Samplers[samplerIndex]);

			CurrentVertexSamplers[index] = sampler;
			CurrentVertexSamplersMipmap[index] = useMipmap;
		}
	}

	void SetVertexSampler(const mu_uint32 baseIndex, const mu_uint32 count, const ESampler *sampler, const mu_boolean *useMipmap)
	{
		for(mu_uint32 n = 0;n < count;++n)
		{
			if (CurrentVertexSamplers[baseIndex + n] != sampler[n] ||
				CurrentVertexSamplersMipmap[baseIndex + n] != useMipmap[n])
			{
				const mu_uint32 samplerIndex = ESamplerMax * static_cast<mu_uint32>(useMipmap[n]) + static_cast<mu_uint32>(sampler[n]);
				glBindSampler(OPENGL_VERTEX_TEXTUREBASE + baseIndex + n, Samplers[samplerIndex]);

				CurrentVertexSamplers[baseIndex + n] = sampler[n];
				CurrentVertexSamplersMipmap[baseIndex + n] = useMipmap[n];
			}
		}
	}

	void SetFragmentSampler(const mu_uint32 index, const ESampler sampler, const mu_boolean useMipmap)
	{
		if (CurrentFragmentSamplers[index] != sampler ||
			CurrentFragmentSamplersMipmap[index] != useMipmap)
		{
			const mu_uint32 samplerIndex = ESamplerMax * static_cast<mu_uint32>(useMipmap) + static_cast<mu_uint32>(sampler);
			glBindSampler(OPENGL_FRAGMENT_TEXTUREBASE + index, Samplers[samplerIndex]);

			CurrentFragmentSamplers[index] = sampler;
			CurrentFragmentSamplersMipmap[index] = useMipmap;
		}
	}

	void SetFragmentSampler(const mu_uint32 baseIndex, const mu_uint32 count, const ESampler *sampler, const mu_boolean *useMipmap)
	{
		for (mu_uint32 n = 0; n < count; ++n)
		{
			if (CurrentFragmentSamplers[baseIndex + n] != sampler[n] ||
				CurrentFragmentSamplersMipmap[baseIndex + n] != useMipmap[n])
			{
				const mu_uint32 samplerIndex = ESamplerMax * static_cast<mu_uint32>(useMipmap[n]) + static_cast<mu_uint32>(sampler[n]);
				glBindSampler(OPENGL_FRAGMENT_TEXTUREBASE + baseIndex + n, Samplers[samplerIndex]);

				CurrentFragmentSamplers[baseIndex + n] = sampler[n];
				CurrentFragmentSamplersMipmap[baseIndex + n] = useMipmap[n];
			}
		}
	}

	void SetActiveTexture(const GLenum index)
	{
		if(CurrentActiveTexture != index)
		{
			glActiveTexture(index);
			CurrentActiveTexture = index;
		}
	}

	void SetUploadTexture(const MUTexture *texture)
	{
		if (CurrentFragmentTextures[0] != texture)
		{
			SetActiveTexture(OPENGL_FRAGMENT_TEXTURE + 0);
			if (texture != nullptr)
			{
				glBindTexture(texture->OpenGLInternal.TargetType, texture->OpenGLInternal.TextureResource);
			}

			CurrentFragmentTextures[0] = texture;
		}
	}

	void SetVertexTexture(const mu_uint32 index, const MUTexture *texture)
	{
		if (CurrentVertexTextures[index] != texture)
		{
			SetActiveTexture(OPENGL_VERTEX_TEXTURE + index);
			if (texture != nullptr)
			{
				glBindTexture(texture->OpenGLInternal.TargetType, texture->OpenGLInternal.TextureResource);
			}

			CurrentVertexTextures[index] = texture;
		}
	}

	void SetVertexTexture(const mu_uint32 baseIndex, const mu_uint32 count, const MUTexture **textures)
	{
		for(mu_uint32 n = 0;n < count;++n)
		{
			const mu_uint32 index = baseIndex + n;
			const MUTexture *texture = textures[n];
			if(CurrentVertexTextures[index] != texture)
			{
				SetActiveTexture(OPENGL_VERTEX_TEXTURE + index);
				if(texture != nullptr)
				{
					glBindTexture(texture->OpenGLInternal.TargetType, texture->OpenGLInternal.TextureResource);
				}

				CurrentVertexTextures[index] = textures[n];
			}
		}
	}

	void SetFragmentTexture(const mu_uint32 index, const MUTexture *texture)
	{
		if (CurrentFragmentTextures[index] != texture)
		{
			SetActiveTexture(OPENGL_FRAGMENT_TEXTURE + index);
			if (texture != nullptr)
			{
				glBindTexture(texture->OpenGLInternal.TargetType, texture->OpenGLInternal.TextureResource);
			}

			CurrentFragmentTextures[index] = texture;
		}
	}

	void SetFragmentTexture(const mu_uint32 baseIndex, const mu_uint32 count, const MUTexture **textures)
	{
		for (mu_uint32 n = 0; n < count; ++n)
		{
			const mu_uint32 index = baseIndex + n;
			const MUTexture *texture = textures[n];
			if (CurrentFragmentTextures[index] != texture)
			{
				SetActiveTexture(OPENGL_FRAGMENT_TEXTURE + index);
				if (texture != nullptr)
				{
					glBindTexture(texture->OpenGLInternal.TargetType, texture->OpenGLInternal.TextureResource);
				}

				CurrentFragmentTextures[index] = textures[n];
			}
		}
	}

	void BindVAO(const GLuint VAO)
	{
		if(CurrentVAO != VAO)
		{
			if(VAO == EInvalidUInt32)
			{
				glBindVertexArray(0);
			}
			else
			{
				glBindVertexArray(VAO);
			}
			CurrentVAO = VAO;
		}
	}
};
#endif