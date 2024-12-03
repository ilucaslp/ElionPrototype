#include "stdafx.h"
#include <SDL_syswm.h>

#define MU_WINDOW_TITLE ("Elion Online")

MUWindowManager::MUWindowManager()
{
	_window_title = MU_WINDOW_TITLE;
	_SDLWindow = nullptr;
#if defined(WIN32) || defined(WIN64)
	_hWnd = nullptr;
	_hInstance = nullptr;
	_hDC = nullptr;
	_WindowStyle = WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_CAPTION | WS_BORDER | WS_SYSMENU | WS_GROUP;
	_WindowStyleEx = WS_EX_WINDOWEDGE;
#endif
}

MUWindowManager::~MUWindowManager()
{
	Destroy();
}

mu_boolean MUWindowManager::Initialize()
{
	const EVideoBackendType backends[] =
	{
#if VULKAN_BACKEND_ENABLE == 1
		EVideoBackendType::eVulkan,
#endif
#if DIRECTX_BACKEND_ENABLE == 1
		EVideoBackendType::eDirectX,
#endif
#if OPENGL_BACKEND_ENABLE == 1
		EVideoBackendType::eOpenGL,
#endif
	};
	const mu_uint32 flags[] =
	{
#if VULKAN_BACKEND_ENABLE == 1
		SDL_WINDOW_VULKAN,
#endif
#if DIRECTX_BACKEND_ENABLE == 1
		0,
#endif
#if OPENGL_BACKEND_ENABLE == 1
		SDL_WINDOW_OPENGL,
#endif
	};
	
	for (mu_uint32 n = 0; n < mu_countof(backends); ++n)
	{
#if __ANDROID__
		_SDLWindow = SDL_CreateWindow(_window_title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, MU_RESOURCE->GetWindowWidth(), MU_RESOURCE->GetWindowHeight(), flags[n] | SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN | SDL_WINDOW_BORDERLESS);
#else
		if (MU_RESOURCE->IsWindowMode() == true)
		{
			_SDLWindow = SDL_CreateWindow(_window_title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, MU_RESOURCE->GetWindowWidth(), MU_RESOURCE->GetWindowHeight(), flags[n] | SDL_WINDOW_SHOWN);
		}
		else
		{
			_SDLWindow = SDL_CreateWindow(_window_title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, MU_RESOURCE->GetWindowWidth(), MU_RESOURCE->GetWindowHeight(), flags[n] | SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP);
		}
#endif

		if (_SDLWindow != nullptr)
		{
			EVideoBackend::SetBackend(backends[n]);
			break;
		}
	}

	if (_SDLWindow == nullptr)
	{
		return false;
	}

#if defined(WIN32) || defined(WIN64)
	SDL_SysWMinfo wmInfo;
	SDL_VERSION(&wmInfo.version);
	SDL_GetWindowWMInfo(_SDLWindow, &wmInfo);

	_hWnd = wmInfo.info.win.window;
	_hInstance = wmInfo.info.win.hinstance;
	_hDC = wmInfo.info.win.hdc;
#endif

	SDL_ShowCursor(SDL_DISABLE);

	return true;
}

void MUWindowManager::Destroy()
{
	if (_SDLWindow != nullptr)
	{
		if(_SDLWindow != nullptr)
			SDL_DestroyWindow(_SDLWindow);

#if defined(WIN32) || defined(WIN64)
		_hWnd = nullptr;
		_hDC = nullptr;
#endif
	}
}