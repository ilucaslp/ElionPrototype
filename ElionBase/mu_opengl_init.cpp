#include "stdafx.h"

namespace EOpenGLInit
{
	mu_uint32 Version[2] = {0, 0};
	SDL_GLContext Context = nullptr;

	const mu_boolean Initialize(SDL_Window *SDLWindow)
	{
		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

#if defined(__ANDROID__)
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
		const mu_int32 ContextVersions[][2] = { {3, 2}, {3, 1}, {3, 0} };
#elif OPENGL_BACKEND_MALIGLES == 1
		SDL_SetHint(SDL_HINT_OPENGL_ES_DRIVER, "1");
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
		const mu_int32 ContextVersions[][2] = { {3, 2}, {3, 1}, {3, 0} };
#else
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		const mu_int32 ContextVersions[][2] = { {4, 6}, {4, 5}, {4, 4}, {4, 3}, {4, 2}, {4, 1}, {4, 0}, {3, 3}, {3, 2}, {3, 1}, {3, 0} };
#endif

#if OPENGL_BACKEND_DEBUG == 1
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
#else
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_NO_ERROR, SDL_TRUE);
#endif

		for(mu_uint32 n = 0;n < mu_countof(ContextVersions);++n)
		{
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, ContextVersions[n][0]);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, ContextVersions[n][1]);

			Context = SDL_GL_CreateContext(SDLWindow);
			if(Context != nullptr)
			{
				Version[0] = ContextVersions[n][0];
				Version[1] = ContextVersions[n][1];
				break;
			}
		}

#if OPENGL_BACKEND_DEBUG == 0
		if(Context == nullptr)
		{
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_NO_ERROR, SDL_FALSE);
			for (mu_uint32 n = 0; n < mu_countof(ContextVersions); ++n)
			{
				SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, ContextVersions[n][0]);
				SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, ContextVersions[n][1]);

				Context = SDL_GL_CreateContext(SDLWindow);
				if (Context != nullptr)
				{
					Version[0] = ContextVersions[n][0];
					Version[1] = ContextVersions[n][1];
					break;
				}
			}
		}
#endif
	
		if (Context == nullptr)
		{
			mu_error("Failed to create opengl context.");
			return false;
		}

		SDL_GL_MakeCurrent(SDLWindow, Context);
		SDL_GL_SetSwapInterval(MU_RESOURCE->IsVerticalSync() == true ? 1 : 0);

#if !defined(__ANDROID__) && OPENGL_BACKEND_MALIGLES == 0
		GLenum glewErr = glewInit();
		if (glewErr != GLEW_OK)
		{
			OutputDebugStringA((LPCSTR)glewGetErrorString(glewErr));
			mu_error("Failed to initialize glew.");
			return false;
		}
#endif

		EOpenGLExtensions::Initialize();
		EOpenGLDebug::Initialize();

		ConfigureFeatures();

		if(EOpenGLShader::Create(true) == false ||
			EOpenGLShader::Create(false) == false)
		{
			return false;
		}

		if(EOpenGLPipeline::Create() == false)
		{
			return false;
		}

		if(EOpenGLBuffer::Create() == false)
		{
			return false;
		}

		if(EOpenGLBuffer::CreateShadowMap() == false)
		{
			return false;
		}

		if (EOpenGLBuffer::CreateMultisample() == false)
		{
			return false;
		}

		EOpenGLPipeline::SetRasterizer(ERasterizer::eBackOnly, false);

		return true;
	}

	void Destroy(const mu_boolean gpuOnly)
	{
		EOpenGLBuffer::DestroyMultisample();
		EOpenGLBuffer::DestroyShadowMap();
		EOpenGLBuffer::Destroy(gpuOnly);
		EOpenGLPipeline::Destroy();
		EOpenGLShader::Destroy();
		if(Context != nullptr)
		{
			SDL_GL_DeleteContext(Context);
			Context = nullptr;
		}
	}

	void ConfigureFeatures()
	{
		GLint texture2DMaxLayers = 256;
		glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &texture2DMaxLayers);
		GLint texture2DMaxSize = 2048;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &texture2DMaxSize);
		GLint texture3DMaxSize = 2048;
		glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &texture3DMaxSize);
		GLint textureCubeMaxSize = 2048;
		glGetIntegerv(GL_MAX_CUBE_MAP_TEXTURE_SIZE, &textureCubeMaxSize);

		EVideoBackend::SetTextureCapabilities(ETextureType::eTexture1D, texture2DMaxSize, texture2DMaxLayers);
		EVideoBackend::SetTextureCapabilities(ETextureType::eTexture2D, texture2DMaxSize, texture2DMaxLayers);
		EVideoBackend::SetTextureCapabilities(ETextureType::eTexture3D, texture3DMaxSize, 1);
		EVideoBackend::SetTextureCapabilities(ETextureType::eTextureCube, textureCubeMaxSize, 1);
	}

	const mu_uint32 *GetVersion()
	{
		return Version;
	}
};