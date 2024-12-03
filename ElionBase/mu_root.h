#ifndef __MU_ROOT_H__
#define __MU_ROOT_H__

#pragma once

class MURoot
#if defined(WIN32) || defined(WIN64)
	: public IMMNotificationClient
#endif
{
public:
	MURoot();

	mu_boolean Initialize();

	void Run();
	void Destroy();

#if defined(WIN32) || defined(WIN64)
	HWND GetHwnd()
	{
		return _WindowHandle;
	}
#endif

private:
	static void WindowProcess(const SDL_Event *event);

protected:
#if defined(WIN32) || defined(WIN64)
	HRESULT STDMETHODCALLTYPE OnDefaultDeviceChanged(EDataFlow flow, ERole role, LPCWSTR pwstrDeviceId);

	HRESULT STDMETHODCALLTYPE OnDeviceAdded(LPCWSTR pwstrDeviceId) { return S_OK; }
	HRESULT STDMETHODCALLTYPE OnDeviceRemoved(LPCWSTR pwstrDeviceId) { return S_OK; }
	HRESULT STDMETHODCALLTYPE OnDeviceStateChanged(LPCWSTR pwstrDeviceId, DWORD dwNewState) { return S_OK; }
	HRESULT STDMETHODCALLTYPE OnPropertyValueChanged(LPCWSTR pwstrDeviceId, const PROPERTYKEY key) { return S_OK; }

	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, VOID **ppvInterface)
	{
		if (IID_IUnknown == riid)
		{
			*ppvInterface = (IUnknown*)this;
		}
		else if (__uuidof(IMMNotificationClient) == riid)
		{
			*ppvInterface = (IMMNotificationClient*)this;
		}
		else
		{
			*ppvInterface = nullptr;
			return E_NOINTERFACE;
		}
		return S_OK;
	}

	ULONG STDMETHODCALLTYPE AddRef()
	{
		return 1;
	}

	ULONG STDMETHODCALLTYPE Release()
	{
		return 1;
	}
#endif

private:
#if defined(WIN32) || defined(WIN64)
	HWND _WindowHandle;
	HINSTANCE _InstanceHandle, _PreviousInstanceHandle;
	IMMDeviceEnumerator *_Enumerator;
#endif

private:
	MUSceneBase *_CurrentScene;

public:
	static MURoot *_Root;
	static MUWindowManager *_WindowManager;
	static MUVideoManager *_VideoManager;
#ifndef DISABLE_AUDIO
	static MUAudioManager *_AudioManager;
#endif
	static MUFontFactory *_FontFactory;
	static MUInputManager *_InputManager;
	static MUResourceManager *_ResourceManager;
	static MUFpsTimer _GlobalTimer;
};

#define MU_ROOT MURoot::_Root
#define MU_WINDOW MURoot::_WindowManager
#define MU_VIDEO MURoot::_VideoManager
#ifndef DISABLE_AUDIO
#define MU_AUDIO MURoot::_AudioManager
#endif
#define MU_MUSIC MURoot::_MusicManager
#define MU_FONT MURoot::_FontFactory
#define MU_INPUT MURoot::_InputManager
#define MU_RESOURCE MURoot::_ResourceManager
#define MU_GLOBALTIMER MURoot::_GlobalTimer

extern MUTextSimpleRender *g_pNormalWrapper;
extern MUTextSimpleRender *g_pBoldWrapper;
extern MUTextSimpleRender *g_pBigWrapper;
extern MUTextSimpleRender *g_pNumberWrapper;

#define FONT_NORMAL g_pNormalWrapper
#define FONT_BOLD g_pBoldWrapper
#define FONT_BIG g_pBigWrapper
#define FONT_NUMBER g_pNumberWrapper

extern mu_uint32 g_ELFocusEventID;

extern mu_uint32 g_DeviceCoreCount;
extern mu_int32 g_DeviceCores[MAX_TASKING_THREADS];

#endif