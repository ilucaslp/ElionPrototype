#include "stdafx.h"

#if OPENGL_BACKEND_ENABLE == 1
namespace EOpenGLShader
{
	const mu_string RootPath = _T("./data/shaders/opengl/");

	std::array<GLuint, EVertexShaderMax> VertexShader = { { 0 } };
	std::array<GLuint, EFragmentShaderMax> FragmentShader = { { 0 } };
	std::array<GLuint, EGeometryShaderMax> GeometryShader = { { 0 } };
	std::array<GLuint, ECombinedShaderMax> ProgramShader = { { 0 } };

	const mu_boolean LoadShader(const mu_text *path, GLint shader, const EShaderType shaderType)
	{
		mu_uint8 *shaderBuffer = nullptr;
		mu_int64 shaderLength = 0;

		if (LoadEncryptedShader(path, &shaderBuffer, shaderLength) == false)
		{
			return false;
		}

		std::stringstream defines;
		mu_asciistr shaderData;

		const mu_uint32 *version = EOpenGLInit::GetVersion();
#if defined(__ANDROID__)
		defines << "#version " << version[0] << version[1] << "0 es" << std::endl;
#else
		defines << "#version " << version[0] << version[1] << "0" << std::endl;
#endif

		switch (shaderType)
		{
		case EShaderType::eVertex:
			{
				if (EOpenGLExtensions::IsExtensionSupported(OpenGLExtensions::TextureBufferObjectEXT) == true &&
					EOpenGLExtensions::IsExtensionSupported(OpenGLExtensions::TextureBufferObjectARB) == false)
				{
					defines << "#extension GL_EXT_texture_buffer_object : enable" << std::endl;
				}
				else if (EOpenGLExtensions::IsExtensionSupported(OpenGLExtensions::TextureBufferEXT) == true &&
					EOpenGLExtensions::IsExtensionSupported(OpenGLExtensions::TextureBufferARB) == false)
				{
					defines << "#extension GL_EXT_texture_buffer : enable" << std::endl;
				}

				defines << "precision highp float;" << std::endl;
				defines << "precision highp int;" << std::endl;
				defines << "precision highp sampler2D;" << std::endl;
				defines << "precision highp usampler2D;" << std::endl;

				if (EOpenGLExtensions::IsFeatureSupported(OpenGLFeatures::TextureBuffer) == true)
				{
					defines << "precision highp samplerBuffer;" << std::endl;
				}
			}
			break;

		case EShaderType::eFragment:
			{
				defines << "#ifdef GL_ES" << std::endl;
				defines << "#ifdef GL_FRAGMENT_PRECISION_HIGH" << std::endl;
				defines << "precision highp float;" << std::endl;
				defines << "#else" << std::endl;
				defines << "precision mediump float;" << std::endl;
				defines << "#endif" << std::endl;
				defines << "#else" << std::endl;
				defines << "precision mediump float;" << std::endl;
				defines << "#endif" << std::endl;
				defines << "precision mediump int;" << std::endl;
				defines << "precision mediump sampler2D;" << std::endl;
				defines << "precision mediump sampler2DArray;" << std::endl;
				defines << "precision mediump sampler2DShadow;" << std::endl;
			}
			break;

		default: break;
		}

		defines << "#define SHADOW_TYPE (";
		defines << static_cast<mu_uint32>(MU_RESOURCE->GetShadowType());
		defines << ")" << std::endl;

#ifdef USE_DUAL_QUATERNION
		defines << "#define USE_DUAL_QUATERNION (1)" << std::endl;
#endif
#if OPENGL_REVERSED_DEPTH == 1
		defines << "#define USE_INVERSEMAPPING (1)" << std::endl;
#endif
		defines << "const float SHADOWMAP_RESOLUTION = (" << SHADOWMAP_RESOLUTION << ".0);" << std::endl;
		defines << "#define SHADOWMAP_TRANSPARENCY (" << SHADOWMAP_TRANSPARENCY << ")" << std::endl;
		defines << "const uint SKELETON_TEXTURE_WIDTH = (" << SKELETON_TEXTURE_WIDTH << "u);" << std::endl;
		defines << "const uint SKELETON_TEXTURE_HEIGHT = (" << SKELETON_TEXTURE_HEIGHT << "u);" << std::endl;
		if (EOpenGLExtensions::IsFeatureSupported(OpenGLFeatures::BonesTextureBuffer) == true)
		{
			defines << "#define USE_TEXTURE_BUFFER (1)" << std::endl;
			defines << "const uint SKELETON_TEXTURE_SIZE = (" << SKELETON_TEXTURE_WIDTH << "u * " << SKELETON_TEXTURE_HEIGHT << "u);" << std::endl;
		}

		shaderData.append(defines.str());
		shaderData.append((const mu_char*)shaderBuffer, (const size_t)shaderLength);

		MU_SAFE_DELETE_ARRAY(shaderBuffer);

		const GLchar *source[] = { shaderData.data() };
		const GLint length[] = { static_cast<GLint>(shaderData.size()) };

		glShaderSource(shader, mu_countof(source), source, length);
		glCompileShader(shader);

#ifndef NDEBUG
		EOpenGLDebug::ReportCompileShader(shader);
#endif

		GLint success = GL_FALSE;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

		if (success == GL_FALSE)
		{
			return false;
		}

		return true;
	}

	const mu_boolean Create(const mu_boolean earlyShaders)
	{
		for (mu_uint32 n = 0; n < EVertexShaderMax; ++n)
		{
			EShaderData &data = VertexShaderData[n];

			if(data.Early != earlyShaders)
			{
				continue;
			}

			VertexShader[n] = glCreateShader(GL_VERTEX_SHADER);
			if (VertexShader[n] == 0)
			{
				EOpenGLError::ReportCheck();
				return false;
			}

			mu_string fullPath = RootPath + data.Path;

#if defined(__ANDROID__)
			__android_log_print(ANDROID_LOG_INFO, "OpenGL", "[Shader] %ls\n", fullPath.c_str());
#else
			std::stringstream debugMessage;
			debugMessage << "[Shader] " << fullPath.c_str() << std::endl;
			OutputDebugStringA(debugMessage.str().c_str());
#endif

			if (LoadShader(fullPath.c_str(), VertexShader[n], EShaderType::eVertex) == false)
			{
				return false;
			}
		}

		for (mu_uint32 n = 0; n < EFragmentShaderMax; ++n)
		{
			EShaderData &data = FragmentShaderData[n];

			if (data.Early != earlyShaders)
			{
				continue;
			}

			FragmentShader[n] = glCreateShader(GL_FRAGMENT_SHADER);
			if (FragmentShader[n] == 0)
			{
				EOpenGLError::ReportCheck();
				return false;
			}

			mu_string fullPath = RootPath + data.Path;

#if defined(__ANDROID__)
			__android_log_print(ANDROID_LOG_INFO, "OpenGL", "[Shader] %ls\n", fullPath.c_str());
#else
			std::stringstream debugMessage;
			debugMessage << "[Shader] " << fullPath.c_str() << std::endl;
			OutputDebugStringA(debugMessage.str().c_str());
#endif

			if (LoadShader(fullPath.c_str(), FragmentShader[n], EShaderType::eFragment) == false)
			{
				return false;
			}
		}

#if GEOMETRY_SHADER_ENABLE == 1
		for (mu_uint32 n = 0; n < EGeometryShaderMax; ++n)
		{
			EShaderData &data = GeometryShaderData[n];

			if (data.Early != earlyShaders)
			{
				continue;
			}

			GeometryShader[n] = glCreateShader(GL_GEOMETRY_SHADER);
			if (GeometryShader[n] == 0)
			{
				EOpenGLError::ReportCheck();
				return false;
			}

			mu_string fullPath = RootPath + data.Path;

#if defined(__ANDROID__)
			__android_log_print(ANDROID_LOG_INFO, "OpenGL", "[Shader] %ls\n", fullPath.c_str());
#else
			std::stringstream debugMessage;
			debugMessage << "[Shader] " << fullPath.c_str() << std::endl;
			OutputDebugStringA(debugMessage.str().c_str());
#endif

			if (LoadShader(fullPath.c_str(), GeometryShader[n], EShaderType::eGeometry) == false)
			{
				return false;
			}
		}
#endif

		for (mu_uint32 n = 0; n < ECombinedShaderMax; ++n)
		{
			ECombinedShaderData &data = CombinedShaderList[n];

			if (data.Early != earlyShaders)
			{
				continue;
			}

			ProgramShader[n] = glCreateProgram();
			if(ProgramShader[n] == 0)
			{
				EOpenGLError::ReportCheck();
				return false;
			}

			if(data.Vertex != EVertexShader::eInvalid)
			{
				glAttachShader(ProgramShader[n], VertexShader[static_cast<mu_uint32>(data.Vertex)]);
			}

			if(data.Fragment != EFragmentShader::eInvalid)
			{
				glAttachShader(ProgramShader[n], FragmentShader[static_cast<mu_uint32>(data.Fragment)]);
			}

			if(data.Geometry != EGeometryShader::eInvalid)
			{
				glAttachShader(ProgramShader[n], GeometryShader[static_cast<mu_uint32>(data.Geometry)]);
			}

			glLinkProgram(ProgramShader[n]);
			EOpenGLDebug::ReportLinkProgram(ProgramShader[n]);

			GLint isLinked = GL_FALSE;
			glGetProgramiv(ProgramShader[n], GL_LINK_STATUS, &isLinked);
			if(isLinked == GL_FALSE)
			{
				return false;
			}

			glUseProgram(ProgramShader[n]);

			GLuint uniformBlockIndex = glGetUniformBlockIndex(ProgramShader[n], "GlobalConstants");
			if (uniformBlockIndex != GL_INVALID_INDEX)
			{
				glUniformBlockBinding(ProgramShader[n], uniformBlockIndex, 0);
			}

			GLint uniformSamplerIndex;
			switch(data.UniformLayout)
			{
			case EUniformLayout::eUIDefault:
				{
					uniformSamplerIndex = glGetUniformLocation(ProgramShader[n], "Tex1");
					if (uniformSamplerIndex != -1)
					{
						glUniform1i(uniformSamplerIndex, OPENGL_FRAGMENT_TEXTUREBASE + 0);
					}
				}
				break;

			case EUniformLayout::eFont:
				{
					uniformBlockIndex = glGetUniformBlockIndex(ProgramShader[n], "FontsConstants");
					if(uniformBlockIndex != GL_INVALID_INDEX)
					{
						glUniformBlockBinding(ProgramShader[n], uniformBlockIndex, 1);
					}

					uniformSamplerIndex = glGetUniformLocation(ProgramShader[n], "FontsTex1");
					if (uniformSamplerIndex != -1)
					{
						glUniform1i(uniformSamplerIndex, OPENGL_FRAGMENT_TEXTUREBASE + 0);
					}
				}
				break;

			case EUniformLayout::eTerrainCommon:
				{
					uniformSamplerIndex = glGetUniformLocation(ProgramShader[n], "LightTexture");
					if (uniformSamplerIndex != -1)
					{
						glUniform1i(uniformSamplerIndex, OPENGL_VERTEX_TEXTUREBASE + 0);
					}

					uniformSamplerIndex = glGetUniformLocation(ProgramShader[n], "AlphaTexture");
					if (uniformSamplerIndex != -1)
					{
						glUniform1i(uniformSamplerIndex, OPENGL_VERTEX_TEXTUREBASE + 1);
					}

					uniformSamplerIndex = glGetUniformLocation(ProgramShader[n], "HeightTexture");
					if (uniformSamplerIndex != -1)
					{
						glUniform1i(uniformSamplerIndex, OPENGL_VERTEX_TEXTUREBASE + 2);
					}

					uniformSamplerIndex = glGetUniformLocation(ProgramShader[n], "AttributeTexture");
					if (uniformSamplerIndex != -1)
					{
						glUniform1i(uniformSamplerIndex, OPENGL_VERTEX_TEXTUREBASE + 3);
					}

					uniformSamplerIndex = glGetUniformLocation(ProgramShader[n], "TerrainTexture");
					if (uniformSamplerIndex != -1)
					{
						glUniform1i(uniformSamplerIndex, OPENGL_FRAGMENT_TEXTUREBASE + 0);
					}

					uniformSamplerIndex = glGetUniformLocation(ProgramShader[n], "WaterTexture");
					if (uniformSamplerIndex != -1)
					{
						glUniform1i(uniformSamplerIndex, OPENGL_FRAGMENT_TEXTUREBASE + 1);
					}

					uniformSamplerIndex = glGetUniformLocation(ProgramShader[n], "ShadowComparison");
					if (uniformSamplerIndex != -1)
					{
						glUniform1i(uniformSamplerIndex, OPENGL_FRAGMENT_TEXTUREBASE + 2);
					}
				}
				break;

			case EUniformLayout::eGrass:
				{
					uniformSamplerIndex = glGetUniformLocation(ProgramShader[n], "LightTexture");
					if (uniformSamplerIndex != -1)
					{
						glUniform1i(uniformSamplerIndex, OPENGL_VERTEX_TEXTUREBASE + 0);
					}

					uniformSamplerIndex = glGetUniformLocation(ProgramShader[n], "AlphaTexture");
					if (uniformSamplerIndex != -1)
					{
						glUniform1i(uniformSamplerIndex, OPENGL_VERTEX_TEXTUREBASE + 1);
					}

					uniformSamplerIndex = glGetUniformLocation(ProgramShader[n], "HeightTexture");
					if (uniformSamplerIndex != -1)
					{
						glUniform1i(uniformSamplerIndex, OPENGL_VERTEX_TEXTUREBASE + 2);
					}

					uniformSamplerIndex = glGetUniformLocation(ProgramShader[n], "AttributeTexture");
					if (uniformSamplerIndex != -1)
					{
						glUniform1i(uniformSamplerIndex, OPENGL_VERTEX_TEXTUREBASE + 3);
					}

					uniformSamplerIndex = glGetUniformLocation(ProgramShader[n], "GrassTexture");
					if (uniformSamplerIndex != -1)
					{
						glUniform1i(uniformSamplerIndex, OPENGL_FRAGMENT_TEXTUREBASE + 0);
					}

					uniformSamplerIndex = glGetUniformLocation(ProgramShader[n], "ShadowComparison");
					if (uniformSamplerIndex != -1)
					{
						glUniform1i(uniformSamplerIndex, OPENGL_FRAGMENT_TEXTUREBASE + 2);
					}
				}
				break;

			case EUniformLayout::eWater:
				{
					uniformSamplerIndex = glGetUniformLocation(ProgramShader[n], "Tex1");
					if (uniformSamplerIndex != -1)
					{
						glUniform1i(uniformSamplerIndex, OPENGL_FRAGMENT_TEXTUREBASE + 0);
					}

					uniformSamplerIndex = glGetUniformLocation(ProgramShader[n], "Tex2");
					if (uniformSamplerIndex != -1)
					{
						glUniform1i(uniformSamplerIndex, OPENGL_FRAGMENT_TEXTUREBASE + 1);
					}

					uniformSamplerIndex = glGetUniformLocation(ProgramShader[n], "ShadowComparison");
					if (uniformSamplerIndex != -1)
					{
						glUniform1i(uniformSamplerIndex, OPENGL_FRAGMENT_TEXTUREBASE + 2);
					}
				}
				break;

			case EUniformLayout::eModel:
				{
					uniformSamplerIndex = glGetUniformLocation(ProgramShader[n], "BonesTexture");
					if (uniformSamplerIndex != -1)
					{
						glUniform1i(uniformSamplerIndex, OPENGL_VERTEX_TEXTUREBASE + 0);
					}

					uniformSamplerIndex = glGetUniformLocation(ProgramShader[n], "Tex1");
					if (uniformSamplerIndex != -1)
					{
						glUniform1i(uniformSamplerIndex, OPENGL_FRAGMENT_TEXTUREBASE + 0);
					}
				}
				break;

			case EUniformLayout::eShadow:
				{
					uniformSamplerIndex = glGetUniformLocation(ProgramShader[n], "BonesTexture");
					if (uniformSamplerIndex != -1)
					{
						glUniform1i(uniformSamplerIndex, OPENGL_VERTEX_TEXTUREBASE + 0);
					}

					if (MU_RESOURCE->GetShadowType() >= EShadowType::eNonShadowMappingBegin &&
						MU_RESOURCE->GetShadowType() <= EShadowType::eNonShadowMappingEnd)
					{
						uniformSamplerIndex = glGetUniformLocation(ProgramShader[n], "HeightTexture");
						if (uniformSamplerIndex != -1)
						{
							glUniform1i(uniformSamplerIndex, OPENGL_VERTEX_TEXTUREBASE + 2);
						}
					}

					uniformSamplerIndex = glGetUniformLocation(ProgramShader[n], "Tex1");
					if (uniformSamplerIndex != -1)
					{
						glUniform1i(uniformSamplerIndex, OPENGL_FRAGMENT_TEXTUREBASE + 0);
					}
				}
				break;

			case EUniformLayout::eEffect:
				{
					uniformSamplerIndex = glGetUniformLocation(ProgramShader[n], "LightTexture");
					if (uniformSamplerIndex != -1)
					{
						glUniform1i(uniformSamplerIndex, OPENGL_VERTEX_TEXTUREBASE + 0);
					}

					uniformSamplerIndex = glGetUniformLocation(ProgramShader[n], "EffectsSampler0");
					if (uniformSamplerIndex != -1)
					{
						glUniform1i(uniformSamplerIndex, OPENGL_FRAGMENT_TEXTUREBASE + 0);
					}

					uniformSamplerIndex = glGetUniformLocation(ProgramShader[n], "EffectsSampler1");
					if (uniformSamplerIndex != -1)
					{
						glUniform1i(uniformSamplerIndex, OPENGL_FRAGMENT_TEXTUREBASE + 1);
					}

					uniformSamplerIndex = glGetUniformLocation(ProgramShader[n], "EffectsSampler2");
					if (uniformSamplerIndex != -1)
					{
						glUniform1i(uniformSamplerIndex, OPENGL_FRAGMENT_TEXTUREBASE + 2);
					}
				}
				break;

			default: break;
			}

#ifndef NDEBUG
			if(EOpenGLDebug::ReportValidateProgram(ProgramShader[n]) == false)
			{
				return false;
			}
#endif
		}

		glUseProgram(0);
		
		return true;
	}

	void DestroyNonEarly()
	{
		for (mu_uint32 n = 0; n < ECombinedShaderMax; ++n)
		{
			ECombinedShaderData &data = CombinedShaderList[n];

			if(data.Early == true)
			{
				continue;
			}

			if (ProgramShader[n] != 0)
			{
				glDeleteProgram(ProgramShader[n]);
				ProgramShader[n] = 0;
			}
		}

		for (mu_uint32 n = 0; n < EVertexShaderMax; ++n)
		{
			EShaderData &data = VertexShaderData[n];

			if (data.Early == true)
			{
				continue;
			}

			if (VertexShader[n] != 0)
			{
				glDeleteShader(VertexShader[n]);
				VertexShader[n] = 0;
			}
		}

		for (mu_uint32 n = 0; n < EFragmentShaderMax; ++n)
		{
			EShaderData &data = FragmentShaderData[n];

			if (data.Early == true)
			{
				continue;
			}

			if (FragmentShader[n] != 0)
			{
				glDeleteShader(FragmentShader[n]);
				FragmentShader[n] = 0;
			}
		}

#if GEOMETRY_SHADER_ENABLE == 1
		for (mu_uint32 n = 0; n < EGeometryShaderMax; ++n)
		{
			EShaderData &data = GeometryShaderData[n];

			if (data.Early == true)
			{
				continue;
			}

			if (GeometryShader[n] != 0)
			{
				glDeleteShader(GeometryShader[n]);
				GeometryShader[n] = 0;
			}
		}
#endif
	}

	void Destroy()
	{
		for(mu_uint32 n = 0;n < ECombinedShaderMax;++n)
		{
			if(ProgramShader[n] != 0)
			{
				glDeleteProgram(ProgramShader[n]);
				ProgramShader[n] = 0;
			}
		}

		for (mu_uint32 n = 0; n < EVertexShaderMax; ++n)
		{
			if (VertexShader[n] != 0)
			{
				glDeleteShader(VertexShader[n]);
				VertexShader[n] = 0;
			}
		}

		for (mu_uint32 n = 0; n < EFragmentShaderMax; ++n)
		{
			if (FragmentShader[n] != 0)
			{
				glDeleteShader(FragmentShader[n]);
				FragmentShader[n] = 0;
			}
		}

#if GEOMETRY_SHADER_ENABLE == 1
		for (mu_uint32 n = 0; n < EGeometryShaderMax; ++n)
		{
			if (GeometryShader[n] != 0)
			{
				glDeleteShader(GeometryShader[n]);
				GeometryShader[n] = 0;
			}
		}
#endif
	}

	ECombinedShader CurrentProgramShader = ECombinedShader::eInvalid;

	void SetProgramShader(const ECombinedShader programShader)
	{
		if (CurrentProgramShader != programShader)
		{
			if (programShader == ECombinedShader::eInvalid)
			{
				glUseProgram(0);
			}
			else
			{
				glUseProgram(ProgramShader[static_cast<mu_uint32>(programShader)]);
			}
			CurrentProgramShader = programShader;
		}
	}
};
#endif