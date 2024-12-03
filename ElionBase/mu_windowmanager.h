#ifndef __MU_WINDOW_MANAGER_H__
#define __MU_WINDOW_MANAGER_H__

#pragma once

class MUWindowManager
{
public:
	MUWindowManager();
	~MUWindowManager();

	mu_boolean Initialize();
	void Destroy();
	
public:
	FORCEINLINE SDL_Window *GetSDLWindow()
	{
		return _SDLWindow;
	}

#if defined(WIN32) || defined(WIN64)
	FORCEINLINE HWND &GetHWND()
	{
		return _hWnd;
	}

	FORCEINLINE HDC &GetDC()
	{
		return _hDC;
	}
#endif

public:
#if defined(WIN32) || defined(WIN64)
	FORCEINLINE const mu_uint32 GetWindowStyle()
	{
		return _WindowStyle;
	}

	FORCEINLINE const mu_uint32 GetWindowStyleEx()
	{
		return _WindowStyleEx;
	}
#endif

private:
	const mu_char *_window_title;
	SDL_Window *_SDLWindow;
#if defined(WIN32) || defined(WIN64)
	HWND _hWnd;
	HINSTANCE _hInstance;
	HDC _hDC;
	mu_uint32 _WindowStyle;
	mu_uint32 _WindowStyleEx;
#endif
};

#endif