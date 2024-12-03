#include "stdafx.h"

#if OPENGL_BACKEND_ANDROID == 1
// GL_EXT_buffer_storage
PFNGLBUFFERSTORAGEEXTPROC glBufferStorage = nullptr;

// GL_EXT_base_instance
PFNGLDRAWARRAYSINSTANCEDBASEINSTANCEEXTPROC glDrawArraysInstancedBaseInstance = nullptr;
PFNGLDRAWELEMENTSINSTANCEDBASEINSTANCEEXTPROC glDrawElementsInstancedBaseInstance = nullptr;
PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXBASEINSTANCEEXTPROC glDrawElementsInstancedBaseVertexBaseInstance = nullptr;

// GL_EXT_texture_buffer_object
PFNGLTEXBUFFEREXTPROC glTexBuffer = nullptr;
PFNGLTEXBUFFERRANGEEXTPROC glTexBufferRange = nullptr;

// GL_KHR_debug
PFNGLDEBUGMESSAGECALLBACKKHRPROC glDebugMessageCallback = nullptr;
PFNGLDEBUGMESSAGECONTROLKHRPROC glDebugMessageControl = nullptr;
#endif

namespace EOpenGLExtensions
{
	std::array<mu_boolean, OpenGLExtensionsMax> Extensions = { { false } };
	std::array<mu_boolean, OpenGLFeaturesMax> Features = { { false } };
	mu_float MaxAnisotropy = 0.0f;

	EINLINE void SetExtension(const OpenGLExtensions extension, const mu_boolean value)
	{
		Extensions[static_cast<mu_uint32>(extension)] = value;
	}

	EINLINE void SetFeature(const OpenGLFeatures feature, const mu_boolean value)
	{
		Features[static_cast<mu_uint32>(feature)] = value;
	}

	void Initialize()
	{
#if OPENGL_BACKEND_MALIGLES == 0
		SetExtension(OpenGLExtensions::Debug, SDL_GL_ExtensionSupported("GL_KHR_debug"));
		SetExtension(OpenGLExtensions::BufferStorageEXT, SDL_GL_ExtensionSupported("GL_EXT_buffer_storage"));
		SetExtension(OpenGLExtensions::BufferStorageARB, SDL_GL_ExtensionSupported("GL_ARB_buffer_storage"));
		SetExtension(OpenGLExtensions::TextureStorageEXT, SDL_GL_ExtensionSupported("GL_EXT_texture_storage"));
		SetExtension(OpenGLExtensions::TextureStorageARB, SDL_GL_ExtensionSupported("GL_ARB_texture_storage"));
		SetExtension(OpenGLExtensions::BaseInstanceEXT, SDL_GL_ExtensionSupported("GL_EXT_base_instance"));
		SetExtension(OpenGLExtensions::BaseInstanceARB, SDL_GL_ExtensionSupported("GL_ARB_base_instance"));
		SetExtension(OpenGLExtensions::TextureBufferObjectEXT, SDL_GL_ExtensionSupported("GL_EXT_texture_buffer_object"));
		SetExtension(OpenGLExtensions::TextureBufferObjectARB, SDL_GL_ExtensionSupported("GL_ARB_texture_buffer_object"));
		SetExtension(OpenGLExtensions::TextureBufferEXT, SDL_GL_ExtensionSupported("GL_EXT_texture_buffer"));
		SetExtension(OpenGLExtensions::TextureBufferARB, SDL_GL_ExtensionSupported("GL_ARB_texture_buffer"));
		SetExtension(OpenGLExtensions::AnisotropyFilterEXT, SDL_GL_ExtensionSupported("GL_EXT_texture_filter_anisotropic"));
		SetExtension(OpenGLExtensions::AnisotropyFilterARB, SDL_GL_ExtensionSupported("GL_ARB_texture_filter_anisotropic"));

		SetFeature(OpenGLFeatures::Debug, IsExtensionSupported(OpenGLExtensions::Debug));
		SetFeature(OpenGLFeatures::BufferStorage, IsExtensionSupported(OpenGLExtensions::BufferStorageEXT) || IsExtensionSupported(OpenGLExtensions::BufferStorageARB));
		SetFeature(OpenGLFeatures::TextureStorage, IsExtensionSupported(OpenGLExtensions::TextureStorageEXT) || IsExtensionSupported(OpenGLExtensions::TextureStorageARB));
		SetFeature(OpenGLFeatures::BaseInstance, IsExtensionSupported(OpenGLExtensions::BaseInstanceEXT) || IsExtensionSupported(OpenGLExtensions::BaseInstanceARB));
		SetFeature(OpenGLFeatures::TextureBuffer, IsExtensionSupported(OpenGLExtensions::TextureBufferEXT) || IsExtensionSupported(OpenGLExtensions::TextureBufferARB) || IsExtensionSupported(OpenGLExtensions::TextureBufferObjectEXT) || IsExtensionSupported(OpenGLExtensions::TextureBufferObjectARB));
		SetFeature(OpenGLFeatures::AnisotropyFilter, IsExtensionSupported(OpenGLExtensions::AnisotropyFilterEXT) || IsExtensionSupported(OpenGLExtensions::AnisotropyFilterARB));
#endif

		if(IsFeatureSupported(OpenGLFeatures::TextureBuffer) == true)
		{
			GLint maxTextureSize = 0;
			glGetIntegerv(GL_MAX_TEXTURE_BUFFER_SIZE, &maxTextureSize);

			if(maxTextureSize >= SKELETON_TEXTURE_WIDTH * SKELETON_TEXTURE_HEIGHT)
			{
				SetFeature(OpenGLFeatures::BonesTextureBuffer, true);
			}
		}

		if(IsFeatureSupported(OpenGLFeatures::AnisotropyFilter) == true)
		{
			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &MaxAnisotropy);
		}

#if OPENGL_BACKEND_ANDROID == 1
		if (IsFeatureSupported(OpenGLFeatures::Debug) == true)
		{
			glDebugMessageControl = (PFNGLDEBUGMESSAGECONTROLKHRPROC)SDL_GL_GetProcAddress("glDebugMessageControlKHR");
			glDebugMessageCallback = (PFNGLDEBUGMESSAGECALLBACKKHRPROC)SDL_GL_GetProcAddress("glDebugMessageCallbackKHR");
		}
		if (IsFeatureSupported(OpenGLFeatures::BufferStorage) == true)
		{
			glBufferStorage = (PFNGLBUFFERSTORAGEEXTPROC)SDL_GL_GetProcAddress("glBufferStorageEXT");
		}
		if (IsFeatureSupported(OpenGLFeatures::BaseInstance) == true)
		{
			glDrawArraysInstancedBaseInstance = (PFNGLDRAWARRAYSINSTANCEDBASEINSTANCEEXTPROC)SDL_GL_GetProcAddress("glDrawArraysInstancedBaseInstanceEXT");
			glDrawElementsInstancedBaseInstance = (PFNGLDRAWELEMENTSINSTANCEDBASEINSTANCEEXTPROC)SDL_GL_GetProcAddress("glDrawElementsInstancedBaseInstanceEXT");
			glDrawElementsInstancedBaseVertexBaseInstance = (PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXBASEINSTANCEEXTPROC)SDL_GL_GetProcAddress("glDrawElementsInstancedBaseVertexBaseInstanceEXT");
		}
		if (IsFeatureSupported(OpenGLFeatures::TextureBuffer) == true)
		{
			glTexBuffer = (PFNGLTEXBUFFEREXTPROC)SDL_GL_GetProcAddress("glTexBufferEXT");
			glTexBufferRange = (PFNGLTEXBUFFERRANGEEXTPROC)SDL_GL_GetProcAddress("glTexBufferRangeEXT");
		}
#endif
	}

	const mu_boolean IsExtensionSupported(const OpenGLExtensions extension)
	{
		return Extensions[ECastEnum(extension)];
	}

	const mu_boolean IsFeatureSupported(const OpenGLFeatures feature)
	{
		return Features[ECastEnum(feature)];
	}

	const mu_float GetMaxAnisotropy()
	{
		return MaxAnisotropy;
	}
};