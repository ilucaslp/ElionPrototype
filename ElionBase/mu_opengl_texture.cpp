#include "stdafx.h"

#if OPENGL_BACKEND_ENABLE == 1
#include "mu_opengl_texture_formats.h"

namespace EOpenGLTexture
{
	const GLenum RetrieveFormat(const ETextureFormat format)
	{
		return OpenGLFormats[ECastEnum(format)];
	}

	const GLenum RetrieveFlatFormat(const ETextureFormat format)
	{
		return OpenGLFlatFormats[ECastEnum(format)];
	}

	const GLenum RetrieveScalarFormat(const ETextureFormat format)
	{
		return OpenGLScalarFormats[ECastEnum(format)];
	}

	const GLenum RetrieveTarget(const EImageViewType viewtype)
	{
		return OpenGLTargetType[ECastEnum(viewtype)];
	}

#if ELION_TEXTURETYPE_FORMAT == ELION_TEXTURETYPE_DDS
	const gli::target RetrieveGliTarget(const EImageViewType viewtype)
	{
		return GLiTargetType[ECastEnum(viewtype)];
	}
#endif

	const mu_boolean LoadFromBuffer(const mu_uint8 *DataBuffer, const mu_uint32 DataSize, const EImageViewType ImageViewType, ETextureBackend *Texture)
	{
		EOpenGLPipeline::SetUploadTexture(nullptr);

#if ELION_TEXTURETYPE_FORMAT == ELION_TEXTURETYPE_DDS
		gli::texture &texture = gli::load_dds((char*)DataBuffer, (mu_size)DataSize);
		if (texture.empty() == true)
		{
			return false;
		}

		gli::gl GL(gli::gl::PROFILE_GL33);
		const gli::gl::format texFormat = GL.translate(texture.format(), texture.swizzles());
		const gli::target srcTarget = ImageViewType == EImageViewType::eNone ? texture.target() : RetrieveGliTarget(ImageViewType);
		const GLenum texTarget = GL.translate(srcTarget);
		const mu_boolean isCompressed = gli::is_compressed(texture.format());

		GLuint texResource = 0;
		EOpenGLHelpers::GenerateTextures(1, &texResource);
		glBindTexture(texTarget, texResource);

		glm::tvec3<GLsizei> const Extent(texture.extent());
		GLsizei const Layers = static_cast<GLsizei>(texture.layers());
		GLsizei const FaceTotal = static_cast<GLsizei>(texture.layers() * texture.faces());

		if(EOpenGLExtensions::IsFeatureSupported(OpenGLFeatures::TextureStorage) == true)
		{
			switch (srcTarget)
			{
			case gli::TARGET_1D:
				{
					glTexStorage1D(
						texTarget, static_cast<GLint>(texture.levels()), texFormat.Internal, Extent.x);
				}
				break;
			case gli::TARGET_1D_ARRAY:
				{
					glTexStorage2D(
						texTarget, static_cast<GLint>(texture.levels()), texFormat.Internal,
						Extent.x, Layers);
				}
				break;
			case gli::TARGET_2D:
				{
					glTexStorage2D(
						texTarget, static_cast<GLint>(texture.levels()), texFormat.Internal,
						Extent.x, Extent.y);
				}
				break;
			case gli::TARGET_CUBE:
				{
					glTexStorage2D(
						texTarget, static_cast<GLint>(texture.levels()), texFormat.Internal,
						Extent.x, FaceTotal);
				}
				break;
			case gli::TARGET_2D_ARRAY:
				{
					glTexStorage3D(
						texTarget, static_cast<GLint>(texture.levels()), texFormat.Internal,
						Extent.x, Extent.y, Layers);
				}
				break;
			case gli::TARGET_3D:
				{
					glTexStorage3D(
						texTarget, static_cast<GLint>(texture.levels()), texFormat.Internal,
						Extent.x, Extent.y, Extent.z);
				}
				break;
			case gli::TARGET_CUBE_ARRAY:
				{
					glTexStorage3D(
						texTarget, static_cast<GLint>(texture.levels()), texFormat.Internal,
						Extent.x, Extent.y, FaceTotal);
				}
				break;
			}
		}
		else
		{
			const mu_int32 levels = static_cast<mu_int32>(texture.levels());
			GLsizei width = Extent.x;
			GLsizei height = Extent.y;
			GLsizei depth = Extent.z;

			switch (srcTarget)
			{
			case gli::TARGET_1D:
				{
					for (mu_int32 n = 0;n < levels;++n)
					{
						glTexImage1D(texTarget, n, texFormat.Internal, width, 0, texFormat.External, texFormat.Type, nullptr);
						width = EMath::Max(1, width >> 1);
					}
				}
				break;
			case gli::TARGET_1D_ARRAY:
				{
					for (mu_int32 n = 0; n < levels; ++n)
					{
						glTexImage2D(texTarget, n, texFormat.Internal, width, Layers, 0, texFormat.External, texFormat.Type, nullptr);
						width = EMath::Max(1, width >> 1);
						height = EMath::Max(1, height >> 1);
					}
				}
				break;
			case gli::TARGET_2D:
				{
					for (mu_int32 n = 0; n < levels; ++n)
					{
						glTexImage2D(texTarget, n, texFormat.Internal, width, height, 0, texFormat.External, texFormat.Type, nullptr);
						width = EMath::Max(1, width >> 1);
						height = EMath::Max(1, height >> 1);
					}
				}
				break;
			case gli::TARGET_CUBE:
				{
					const GLenum facesTarget[] =
					{
						GL_TEXTURE_CUBE_MAP_POSITIVE_X,
						GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
						GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
						GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
						GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
						GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
					};
					for (mu_int32 n = 0; n < levels; ++n)
					{
						for(mu_uint32 f = 0;f < mu_countof(facesTarget);++f)
						{
							glTexImage2D(texTarget, n, texFormat.Internal, width, height, 0, texFormat.External, texFormat.Type, nullptr);
						}
						width = EMath::Max(1, width >> 1);
						height = EMath::Max(1, height >> 1);
					}
				}
				break;
			case gli::TARGET_2D_ARRAY:
				{
					for (mu_int32 n = 0; n < levels; ++n)
					{
						glTexImage3D(texTarget, n, texFormat.Internal, Extent.x, Extent.y, Layers, 0, texFormat.External, texFormat.Type, nullptr);
						width = EMath::Max(1, width >> 1);
						height = EMath::Max(1, height >> 1);
					}
				}
				break;
			case gli::TARGET_3D:
				{
					for (mu_int32 n = 0; n < levels; ++n)
					{
						glTexImage3D(texTarget, n, texFormat.Internal, width, height, Extent.z, 0, texFormat.External, texFormat.Type, nullptr);
						width = EMath::Max(1, width >> 1);
						height = EMath::Max(1, height >> 1);
						depth = EMath::Max(1, depth >> 1);
					}
				}
				break;
			case gli::TARGET_CUBE_ARRAY:
				{
					const GLenum facesTarget[] =
					{
						GL_TEXTURE_CUBE_MAP_POSITIVE_X,
						GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
						GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
						GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
						GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
						GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
					};
					for (mu_int32 n = 0; n < levels; ++n)
					{
						glTexImage3D(facesTarget[n], n, texFormat.Internal, width, height, FaceTotal, 0, texFormat.External, texFormat.Type, nullptr);
						width = EMath::Max(1, width >> 1);
						height = EMath::Max(1, height >> 1);
					}
				}
				break;
			}
		}

		glTexParameteriv(texTarget, GL_TEXTURE_SWIZZLE_RGBA, &texFormat.Swizzles[0]);

		mu_boolean isTextureCube = gli::is_target_cube(srcTarget);
		mu_boolean isTextureArray = (srcTarget == gli::TARGET_1D_ARRAY || srcTarget == gli::TARGET_2D_ARRAY);

		if (gli::is_compressed(texture.format()) == true)
		{
			for (std::size_t Face = 0; Face < texture.faces(); ++Face)
			{
				for (std::size_t Level = 0; Level < texture.levels(); ++Level)
				{
					for (std::size_t Layer = 0; Layer < texture.layers(); ++Layer)
					{
						glm::tvec3<GLsizei> Extent(texture.extent(Level));
						const GLenum target = isTextureCube
							? static_cast<GLenum>(GL_TEXTURE_CUBE_MAP_POSITIVE_X + Face)
							: texTarget;

						switch (srcTarget)
						{
						case gli::TARGET_1D:
							{
								glCompressedTexSubImage1D(
									target, static_cast<GLint>(Level), 0, Extent.x,
									texFormat.Internal, static_cast<GLsizei>(texture.size(Level)),
									texture.data(Layer, Face, Level));
							}
							break;
						case gli::TARGET_1D_ARRAY:
						case gli::TARGET_2D:
						case gli::TARGET_CUBE:
							{
								glCompressedTexSubImage2D(
									target, static_cast<GLint>(Level),
									0, Layer,
									Extent.x,
									isTextureArray ? 1 : Extent.y,
									texFormat.Internal, static_cast<GLsizei>(texture.size(Level)),
									texture.data(Layer, Face, Level));
							}
							break;
						case gli::TARGET_2D_ARRAY:
						case gli::TARGET_3D:
						case gli::TARGET_CUBE_ARRAY:
							{
								glCompressedTexSubImage3D(
									target, static_cast<GLint>(Level),
									0, 0, Layer,
									Extent.x, Extent.y,
									isTextureArray ? 1 : Extent.z,
									texFormat.Internal, static_cast<GLsizei>(texture.size(Level)),
									texture.data(Layer, Face, Level));
							}
							break;
						}
					}
				}
			}
		}
		else
		{
			for (std::size_t Layer = 0; Layer < texture.layers(); ++Layer)
			{
				for (std::size_t Face = 0; Face < texture.faces(); ++Face)
				{
					for (std::size_t Level = 0; Level < texture.levels(); ++Level)
					{
						glm::tvec3<GLsizei> Extent(texture.extent(Level));
						const GLenum target = isTextureCube
							? static_cast<GLenum>(GL_TEXTURE_CUBE_MAP_POSITIVE_X + Face)
							: texTarget;

						switch (srcTarget)
						{
						case gli::TARGET_1D:
							{
								glTexSubImage1D(
									target, static_cast<GLint>(Level), 0, Extent.x,
									texFormat.External, texFormat.Type,
									texture.data(Layer, Face, Level));
							}
							break;
						case gli::TARGET_1D_ARRAY:
						case gli::TARGET_2D:
						case gli::TARGET_CUBE:
							{
								glTexSubImage2D(
									target, static_cast<GLint>(Level),
									0, Layer,
									Extent.x,
									isTextureArray ? 1 : Extent.y,
									texFormat.External, texFormat.Type,
									texture.data(Layer, Face, Level));
							}
							break;
						case gli::TARGET_2D_ARRAY:
						case gli::TARGET_3D:
						case gli::TARGET_CUBE_ARRAY:
							{
								glTexSubImage3D(
									target, static_cast<GLint>(Level),
									0, 0, Layer,
									Extent.x, Extent.y,
									isTextureArray ? 1 : Extent.z,
									texFormat.External, texFormat.Type,
									texture.data(Layer, Face, Level));
							}
							break;
						}
					}
				}
			}
		}

		glBindTexture(texTarget, 0);

		Texture->OpenGLInternal.TargetType = texTarget;
		Texture->OpenGLInternal.TextureResource = texResource;

		if(EOpenGLError::ReportCheck() == false)
		{
			return false;
		}
#else	
		if (ktxLoadTextureM(DataBuffer, DataSize, &Texture->OpenGLInternal.TextureResource, &Texture->OpenGLInternal.TargetType, nullptr, nullptr, nullptr, nullptr, nullptr, RetrieveTarget(ImageViewType)) != KTX_SUCCESS)
		{
			return false;
		}
#endif

		return true;
	}

	const mu_boolean CreateBones(ETextureBackend *Texture)
	{
		EOpenGLHelpers::GenerateTextures(1, &Texture->OpenGLInternal.TextureResource);

		if (EOpenGLExtensions::IsFeatureSupported(OpenGLFeatures::BonesTextureBuffer) == true)
		{
			Texture->OpenGLInternal.TargetType = GL_TEXTURE_BUFFER;
			EOpenGLHelpers::GenerateBuffers(1, &Texture->OpenGLInternal.BufferResource);

			if (EOpenGLExtensions::IsFeatureSupported(OpenGLFeatures::BufferStorage) == true)
			{
				glBindBuffer(GL_TEXTURE_BUFFER, Texture->OpenGLInternal.BufferResource);
				glBufferStorage(GL_TEXTURE_BUFFER, SkeletonMemorySize, nullptr, GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT);

				if (EOpenGLError::ReportCheck() == false)
				{
					glBindBuffer(GL_TEXTURE_BUFFER, 0);
					return false;
				}

				glBindBuffer(GL_TEXTURE_BUFFER, 0);
			}
			else
			{
				glBindBuffer(GL_TEXTURE_BUFFER, Texture->OpenGLInternal.BufferResource);
				glBufferData(GL_TEXTURE_BUFFER, SkeletonMemorySize, nullptr, GL_DYNAMIC_DRAW);

				if (EOpenGLError::ReportCheck() == false)
				{
					glBindBuffer(GL_TEXTURE_BUFFER, 0);
					return false;
				}

				glBindBuffer(GL_TEXTURE_BUFFER, 0);
			}

			EOpenGLPipeline::SetUploadTexture(nullptr); // bind Texture 0 as active

			glBindTexture(GL_TEXTURE_BUFFER, Texture->OpenGLInternal.TextureResource);
			glTexBuffer(GL_TEXTURE_BUFFER, RetrieveFormat(SKELETON_TEXTURE_FORMAT), Texture->OpenGLInternal.BufferResource);
			glBindTexture(GL_TEXTURE_BUFFER, 0);
		}
		else
		{
			Texture->OpenGLInternal.TargetType = GL_TEXTURE_2D;

			EOpenGLPipeline::SetUploadTexture(nullptr); // bind Texture 0 as active

			glBindTexture(GL_TEXTURE_2D, Texture->OpenGLInternal.TextureResource);
			if(EOpenGLExtensions::IsFeatureSupported(OpenGLFeatures::TextureStorage) == true)
			{
				glTexStorage2D(GL_TEXTURE_2D, 1, RetrieveFormat(SKELETON_TEXTURE_FORMAT), SKELETON_TEXTURE_WIDTH, SKELETON_TEXTURE_HEIGHT);
			}
			else
			{
				glTexImage2D(GL_TEXTURE_2D, 0, RetrieveFormat(SKELETON_TEXTURE_FORMAT), SKELETON_TEXTURE_WIDTH, SKELETON_TEXTURE_HEIGHT, 0, RetrieveFlatFormat(SKELETON_TEXTURE_FORMAT), RetrieveScalarFormat(SKELETON_TEXTURE_FORMAT), nullptr);
			}
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			glBindTexture(GL_TEXTURE_2D, 0);
		}

		Texture->TextureInternal.Width = SKELETON_TEXTURE_WIDTH;
		Texture->TextureInternal.Height = SKELETON_TEXTURE_HEIGHT;
		Texture->TextureInternal.Depth = 1;
		Texture->TextureInternal.Components = GetTextureComponents(SKELETON_TEXTURE_FORMAT);
		Texture->TextureInternal.Sampler = ESampler::eUcVcWcMinNMagNMipN;
		Texture->TextureInternal.Loaded = true;

		return true;
	}

	const mu_boolean CreateShadowMap(ETextureBackend *Texture)
	{
		constexpr ETextureFormat Format = SHADOWMAP_FORMAT;
		constexpr mu_uint32 Width = SHADOWMAP_RESOLUTION;
		constexpr mu_uint32 Height = SHADOWMAP_RESOLUTION;

		Texture->OpenGLInternal.TargetType = GL_TEXTURE_2D;
		EOpenGLHelpers::GenerateTextures(1, &Texture->OpenGLInternal.TextureResource);
		EOpenGLPipeline::SetUploadTexture(nullptr); // bind Texture 0 as active

		glBindTexture(GL_TEXTURE_2D, Texture->OpenGLInternal.TextureResource);
		if (EOpenGLExtensions::IsFeatureSupported(OpenGLFeatures::TextureStorage) == true)
		{
			glTexStorage2D(GL_TEXTURE_2D, 1, RetrieveFormat(Format), Width, Height);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, RetrieveFormat(Format), Width, Height, 0, RetrieveFlatFormat(Format), RetrieveScalarFormat(Format), nullptr);
		}
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		if(EOpenGLError::ReportCheck() == false)
		{
			return false;
		}

		EOpenGLHelpers::GenerateFramebuffers(1, &Texture->OpenGLInternal.FramebufferResource);
		glBindFramebuffer(GL_FRAMEBUFFER, Texture->OpenGLInternal.FramebufferResource);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, Texture->OpenGLInternal.TextureResource, 0);
		GLenum buffs[1] = { GL_NONE };
		glDrawBuffers(1, buffs);
		glReadBuffer(GL_NONE);
		GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		if (result != GL_FRAMEBUFFER_COMPLETE)
		{
			return false;
		}

		Texture->TextureInternal.Width = Width;
		Texture->TextureInternal.Height = Height;
		Texture->TextureInternal.Depth = 1;
		Texture->TextureInternal.Components = GetTextureComponents(Format);
		Texture->TextureInternal.Sampler = ESampler::eUcVcWcMinNMagNMipN_Anisotropy;
		Texture->TextureInternal.Loaded = true;

		return true;
	}

	const mu_boolean CreateMultisample(ETextureBackend *Texture, mu_uint32 Samples)
	{
		constexpr ETextureFormat Format = ETextureFormat::eRGBA8_Unorm;
		constexpr ETextureFormat DepthFormat = ETextureFormat::eDepth32_Float;
		const mu_uint32 Width = MU_RESOURCE->GetWindowWidth();
		const mu_uint32 Height = MU_RESOURCE->GetWindowHeight();

		Texture->OpenGLInternal.TargetType = GL_RENDERBUFFER;
		EOpenGLHelpers::GenerateRenderbuffers(1, &Texture->OpenGLInternal.TextureResource);
		EOpenGLHelpers::GenerateRenderbuffers(1, &Texture->OpenGLInternal.DepthResource);
		EOpenGLPipeline::SetUploadTexture(nullptr); // bind Texture 0 as active

		glBindRenderbuffer(GL_RENDERBUFFER, Texture->OpenGLInternal.TextureResource);
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, Samples, RetrieveFormat(Format), Width, Height);
		mu_boolean isSuccess = EOpenGLError::ReportCheck();
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		if (isSuccess == false)
		{
			return false;
		}

		glBindRenderbuffer(GL_RENDERBUFFER, Texture->OpenGLInternal.DepthResource);
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, Samples, RetrieveFormat(DepthFormat), Width, Height);
		isSuccess = EOpenGLError::ReportCheck();
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		if (isSuccess == false)
		{
			return false;
		}

		EOpenGLHelpers::GenerateFramebuffers(1, &Texture->OpenGLInternal.FramebufferResource);
		glBindFramebuffer(GL_FRAMEBUFFER, Texture->OpenGLInternal.FramebufferResource);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, Texture->OpenGLInternal.TextureResource);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, Texture->OpenGLInternal.DepthResource);

		GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		if (result != GL_FRAMEBUFFER_COMPLETE)
		{
			return false;
		}

		Texture->TextureInternal.Width = Width;
		Texture->TextureInternal.Height = Height;
		Texture->TextureInternal.Depth = 1;
		Texture->TextureInternal.Components = GetTextureComponents(Format);
		Texture->TextureInternal.Sampler = ESampler::eUcVcWcMinNMagNMipN_Anisotropy;
		Texture->TextureInternal.Loaded = true;

		return true;
	}

	const mu_boolean Create1D(const mu_uint32 Width,
		const ETextureFormat Format,
		const ETextureUsage Usage,
		const ESampler Sampler,
		const mu_uint8 *DataBuffer,
		ETextureBackend *Texture)
	{
		Texture->OpenGLInternal.TargetType = GL_TEXTURE_2D;
		EOpenGLHelpers::GenerateTextures(1, &Texture->OpenGLInternal.TextureResource);
		EOpenGLPipeline::SetUploadTexture(nullptr);

		glBindTexture(GL_TEXTURE_2D, Texture->OpenGLInternal.TextureResource);
		if (EOpenGLExtensions::IsFeatureSupported(OpenGLFeatures::TextureStorage) == true)
		{
			glTexStorage2D(GL_TEXTURE_2D, 1, RetrieveFormat(Format), Width, 1);

			if (DataBuffer != nullptr)
			{
				glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, Width, 1, RetrieveFlatFormat(Format), RetrieveScalarFormat(Format), DataBuffer);

				if (EOpenGLError::ReportCheck() == false)
				{
					glBindTexture(GL_TEXTURE_2D, 0);
					return false;
				}
			}
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

			glTexImage2D(GL_TEXTURE_2D, 0, RetrieveFormat(Format), Width, 1, 0, RetrieveFlatFormat(Format), RetrieveScalarFormat(Format), DataBuffer);
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glBindTexture(GL_TEXTURE_2D, 0);

		if (EOpenGLError::ReportCheck() == false)
		{
			return false;
		}

		Texture->TextureInternal.Width = Width;
		Texture->TextureInternal.Height = 1;
		Texture->TextureInternal.Depth = 1;
		Texture->TextureInternal.Components = GetTextureComponents(Format);
		Texture->TextureInternal.Sampler = Sampler;
		Texture->TextureInternal.Loaded = true;

		return true;
	}

	const mu_boolean Create2D(const mu_uint32 Width,
		const mu_uint32 Height,
		const ETextureFormat Format,
		const ETextureUsage Usage,
		const ESampler Sampler,
		const mu_uint8 *DataBuffer,
		const mu_uint32 Pitch,
		ETextureBackend *Texture)
	{
		Texture->OpenGLInternal.TargetType = GL_TEXTURE_2D;
		EOpenGLHelpers::GenerateTextures(1, &Texture->OpenGLInternal.TextureResource);
		EOpenGLPipeline::SetUploadTexture(nullptr);

		glBindTexture(GL_TEXTURE_2D, Texture->OpenGLInternal.TextureResource);
		if (EOpenGLExtensions::IsFeatureSupported(OpenGLFeatures::TextureStorage) == true)
		{
			glTexStorage2D(GL_TEXTURE_2D, 1, RetrieveFormat(Format), Width, Height);

			if (DataBuffer != nullptr)
			{
				glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
				glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, Width, Height, RetrieveFlatFormat(Format), RetrieveScalarFormat(Format), DataBuffer);
				glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

				if (EOpenGLError::ReportCheck() == false)
				{
					glBindTexture(GL_TEXTURE_2D, 0);
					return false;
				}
			}
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glTexImage2D(GL_TEXTURE_2D, 0, RetrieveFormat(Format), Width, Height, 0, RetrieveFlatFormat(Format), RetrieveScalarFormat(Format), DataBuffer);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glBindTexture(GL_TEXTURE_2D, 0);

		if (EOpenGLError::ReportCheck() == false)
		{
			return false;
		}

		Texture->TextureInternal.Width = Width;
		Texture->TextureInternal.Height = Height;
		Texture->TextureInternal.Depth = 1;
		Texture->TextureInternal.Components = GetTextureComponents(Format);
		Texture->TextureInternal.Sampler = Sampler;
		Texture->TextureInternal.Loaded = true;

		return true;
	}

	const mu_boolean Create2DArray(const mu_uint32 Width,
		const mu_uint32 Height,
		const mu_uint32 Layers,
		const ETextureFormat Format,
		const ETextureUsage Usage,
		const ESampler Sampler,
		const mu_uint8 *DataBuffer,
		const mu_uint32 Pitch,
		ETextureBackend *Texture)
	{
		Texture->OpenGLInternal.TargetType = GL_TEXTURE_2D_ARRAY;
		EOpenGLHelpers::GenerateTextures(1, &Texture->OpenGLInternal.TextureResource);
		EOpenGLPipeline::SetUploadTexture(nullptr);

		glBindTexture(GL_TEXTURE_2D_ARRAY, Texture->OpenGLInternal.TextureResource);
		if (EOpenGLExtensions::IsFeatureSupported(OpenGLFeatures::TextureStorage) == true)
		{
			glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, RetrieveFormat(Format), Width, Height, Layers);

			if (DataBuffer != nullptr)
			{
				glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, Width, Height, Layers, RetrieveFlatFormat(Format), RetrieveScalarFormat(Format), DataBuffer);

				if (EOpenGLError::ReportCheck() == false)
				{
					glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
					return false;
				}
			}
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BASE_LEVEL, 0);
			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_LEVEL, 0);

			glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, RetrieveFormat(Format), Width, Height, Layers, 0, RetrieveFlatFormat(Format), RetrieveScalarFormat(Format), DataBuffer);
		}

		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

		if (EOpenGLError::ReportCheck() == false)
		{
			return false;
		}

		Texture->TextureInternal.Width = Width;
		Texture->TextureInternal.Height = Height;
		Texture->TextureInternal.Depth = Layers;
		Texture->TextureInternal.Components = GetTextureComponents(Format);
		Texture->TextureInternal.Sampler = Sampler;
		Texture->TextureInternal.Loaded = true;

		return true;
	}
};
#endif