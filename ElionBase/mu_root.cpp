#include "stdafx.h"
#include "mu_root.h"

#if defined(WIN32) || defined(WIN64)
#include <dbt.h>
#include <Psapi.h>
#include <windowsx.h>
#endif

mu_uint32 g_DeviceCoreCount = 1;
mu_int32 g_DeviceCores[MAX_TASKING_THREADS] = { -1 };

MURoot *MURoot::_Root = nullptr;
MUWindowManager *MURoot::_WindowManager = nullptr;
MUVideoManager *MURoot::_VideoManager = nullptr;
#ifndef DISABLE_AUDIO
MUAudioManager *MURoot::_AudioManager = nullptr;
#endif
MUFontFactory *MURoot::_FontFactory = nullptr;
MUInputManager *MURoot::_InputManager = nullptr;
MUResourceManager *MURoot::_ResourceManager = nullptr;
MUFpsTimer MURoot::_GlobalTimer;

MUTextSimpleRender *g_pNormalWrapper = nullptr;
MUTextSimpleRender *g_pBoldWrapper = nullptr;
MUTextSimpleRender *g_pBigWrapper = nullptr;
MUTextSimpleRender *g_pNumberWrapper = nullptr;

mu_uint32 g_ELFocusEventID = (mu_uint32)-1;

//#define USE_SINGLE_BUFFER
#define TOTAL_OBJECTS (MAX_OBJECTS + MAX_BOIDS + MAX_FISHS + MAX_BUTTERFLES + MAX_EFFECTS)
#define ENVIRONMENT_OFFSET (0)
#define BOIDS_OFFSET (ENVIRONMENT_OFFSET + MAX_OBJECTS)
#define FISHS_OFFSET (BOIDS_OFFSET + MAX_BOIDS)
#define BUTTERFLES_OFFSET (FISHS_OFFSET + MAX_FISHS)
#define EFFECTS_OFFSET (BUTTERFLES_OFFSET + MAX_BUTTERFLES)

MURoot::MURoot() :
#if defined(WIN32) || defined(WIN64)
	_WindowHandle(nullptr),
#endif
	_CurrentScene(SCENES::DefaultScene)
{
	_Root = this;
#if defined(WIN32) || defined(WIN64)
	_Enumerator = nullptr;
#endif
}

//#define RELEASE_CONSOLE
mu_boolean MURoot::Initialize()
{
	mu_zeromem(MODELS::LoadData, sizeof(MODELS::LoadData));
	mu_zeromem(TEXTURES::LoadData, sizeof(TEXTURES::LoadData));

	for(mu_uint32 n = 0;n < MODELS::LoadCount;++n)
	{
		MULoadingData *data = &MODELS::Load[n];
		MODELS::LoadData[data->Index] = data;
	}

	for (mu_uint32 n = 0; n < TEXTURES::LoadCount; ++n)
	{
		MULoadingData *data = &TEXTURES::Load[n];
		TEXTURES::LoadData[data->Index] = data;
	}

	for (mu_uint32 n = 0; n < TEXTURES::LoadRuntimeCount; ++n)
	{
		MULoadingData *data = &TEXTURES::LoadRuntime[n];
		TEXTURES::LoadData[data->Index] = data;
	}

	cpuinfo_initialize();

#if __ANDROID__
	const mu_uint32 clustersCount = cpuinfo_get_clusters_count();
	const cpuinfo_cluster *clusters = cpuinfo_get_clusters();
	mu_int32 clusterIndex = 0;
	for (mu_uint32 n = 1; n < clustersCount; ++n)
	{
		if (clusters[clusterIndex].frequency < clusters[n].frequency)
		{
			clusterIndex = n;
		}
	}

	if (clustersCount == 0)
	{
		g_DeviceCoreCount = 1;
		g_DeviceCores[0] = 0;
	}
	else
	{
		const cpuinfo_cluster &cluster = clusters[clusterIndex];
		g_DeviceCoreCount = cluster.processor_count;

		mu_boolean sharedCores = false;
		if(clustersCount == 1 && g_DeviceCoreCount > 1)
		{
			sharedCores = true;
			g_DeviceCoreCount /= 2;
		}

		if (g_DeviceCoreCount > 1)
		{
			cpu_set_t cpu_affinity;
			CPU_ZERO(&cpu_affinity);

			const cpuinfo_processor *cores = cpuinfo_get_processors();
			if(sharedCores == true)
			{
				for (mu_uint32 n = 0; n < g_DeviceCoreCount; ++n)
				{
					g_DeviceCores[n] = cores[cluster.processor_start + n + 1].linux_id;
					CPU_SET(g_DeviceCores[n], &cpu_affinity);
				}
			}
			else
			{
				for (mu_uint32 n = 0; n < g_DeviceCoreCount; ++n)
				{
					g_DeviceCores[n] = cores[cluster.processor_start + n].linux_id;
					CPU_SET(g_DeviceCores[n], &cpu_affinity);
				}
			}

			sched_setaffinity(gettid(), CPU_SETSIZE, &cpu_affinity);
		}
		else
		{
			g_DeviceCores[0] = 0;
		}
	}
#else
	g_DeviceCoreCount = cpuinfo_get_processors_count();
#endif

	ClearLoadingGPU();

#if DIRECTX_BACKEND_ENABLE == 1
	if (FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED)))
	{
		mu_error("Failed to initialize COM.");
		return false;
	}
#endif

#if __ANDROID__
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
#else
	if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0)
#endif
	{
		mu_error("Failed to initialize SDL.");
		return false;
	}

	SDL_EventState(SDL_AUDIODEVICEADDED, SDL_ENABLE);
	SDL_EventState(SDL_AUDIODEVICEREMOVED, SDL_ENABLE);

#if (defined(__ANDROID__) || defined(USE_MALIGLES)) && !defined(USE_MALIGLES)
#if 1
	boost::system::error_code ec;
	boost::filesystem::path selfPrimaryPath("/storage/self/primary/Android/");
	boost::filesystem::path sdcard0Path("/storage/sdcard0/Android/");

	if(boost::filesystem::exists(selfPrimaryPath, ec) == true)
	{
		MUBasePrefPath = "/storage/self/primary/Android/data/com.ElionMobile.ElionMobile/files/";
	}
	else if(boost::filesystem::exists(sdcard0Path, ec) == true)
	{
		MUBasePrefPath = "/storage/sdcard0/Android/data/com.ElionMobile.ElionMobile/files/";
	}
	else
	{
		mu_error("Failed to find available path.");
		return false;
	}
#else
	MUBasePrefPath = SDL_GetPrefPath("ElionGames", "ElionMobile");
#endif
	MUBasePrefPathUnicode = boost::locale::conv::utf_to_utf<mu_text>(MUBasePrefPath);
#endif

#if defined(WIN32) || defined(WIN64)
#if !defined(NDEBUG) && 0
	RedirectIOToConsole();
#elif defined(RELEASE_CONSOLE)
	RedirectIOToConsole();
#endif
#endif

#ifndef DISABLE_AUDIO
	_AudioManager = new MUAudioManager();

	if (_AudioManager == nullptr)
	{
		mu_error("Failed to initialize audio.");
		return false;
	}

	_AudioManager->Initialize();
#endif

	if (EMusicBackend::Initialize() == false)
	{
		mu_error("Failed to initialize music.");
		return false;
	}

	_ResourceManager = new MUResourceManager();

	if (_ResourceManager == nullptr || _ResourceManager->LoadSettings() == false)
	{
		mu_error("Failed to initialize resources.");
		return false;
	}

	_InputManager = new MUInputManager();

	if (_InputManager == nullptr)
	{
		mu_error("Failed to initialize input.");
		return false;
	}

	_WindowManager = new MUWindowManager();

	if (_WindowManager == nullptr || _WindowManager->Initialize() == false)
	{
		mu_error("Failed to initialize window.");
		return false;
	}

	_VideoManager = new MUVideoManager();

	if (_VideoManager == nullptr)
	{
		mu_error("Failed to initialize video.");
		return false;
	}

	_FontFactory = new MUFontFactory();

	if (_FontFactory == nullptr || _FontFactory->Initialize() == false)
	{
		mu_error("Failed to initialize font factory.");
		return false;
	}

	InitializeGlobalCulling();

	if (g_Tasking.Create() == false)
	{
		mu_error("Failed to initialize tasking.");
		return false;
	}

	_GlobalTimer.Initialize();

	if (g_LoadingManager.Initialize() == false)
	{
		mu_error("Failed to initialize loading manager.");
		return false;
	}

#if defined(WIN32) || defined(WIN64)
	_Enumerator = nullptr;
	HRESULT hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_INPROC_SERVER, __uuidof(IMMDeviceEnumerator), (void**)&_Enumerator);
	if (SUCCEEDED(hr))
	{
		_Enumerator->RegisterEndpointNotificationCallback(this);
	}
#endif

	ZeroArray(g_EnvironmentBlocks);
	ZeroArray(g_EnvironmentBlocksCount);

	CharacterMachine = new CHARACTER_MACHINE();
	CharacterAttribute = &CharacterMachine->Character;
	CharacterMachine->Init();

	g_pNormalWrapper = new MUTextSimpleRender(MU_FONT->GetFont(NORMAL_FONT), ECombinedShader::eFont);
	g_pBoldWrapper = new MUTextSimpleRender(MU_FONT->GetFont(BOLD_FONT), ECombinedShader::eFont);
	g_pBigWrapper = new MUTextSimpleRender(MU_FONT->GetFont(BIG_FONT), ECombinedShader::eFont);
	g_pNumberWrapper = new MUTextSimpleRender(MU_FONT->GetFont(NUMBER_FONT), ECombinedShader::eFont);

	g_ELFocusEventID = SDL_RegisterEvents(1);

	g_NetworkManager = new ElionNetwork::ELNetworkManager(2);
	if (g_NetworkManager == nullptr)
	{
		mu_error("Failed to allocate network manager.");
		return false;
	}

	if (g_NetworkManager->Initialize() == false)
	{
		mu_error("Failed to initialize network manager.");
		return false;
	}

	g_NetworkClient = new MUNetworkClient(g_NetworkManager);

	if (g_NetworkClient == nullptr)
	{
		mu_error("Failed to allocate network client.");
		return false;
	}

	g_ConnectServerObject = new CSNetworkObject(g_NetworkManager, -1, NETWORK_BUFFER_SIZE);
	g_GameServerObject = new GSNetworkObject(g_NetworkManager, -1, NETWORK_BUFFER_SIZE);

	if (g_ConnectServerObject == nullptr ||
		g_GameServerObject == nullptr)
	{
		mu_error("Failed to allocate network objects.");
		return false;
	}

	CharacterViewLive = true;
	CharacterViewVisible = false;
	CreateCharacterPointer(&CharacterView, MODEL_FACE + 1, 0, 0);
	CharacterView.Object.Flags.Set(OBJECT::eOF_LIVE);
	CharacterView.Object.Flags.Clear(OBJECT::eOF_LIGHTENABLE);
	CharacterView.Class = 1;
	CharacterView.Object.Kind = 0;

	for (mu_int16 n = 0; n < MAX_CHARACTERS_CLIENT; ++n)
	{
		CharactersClient[n].Index = n;
	}

	mu_memset(CharactersKeys.data(), 0xFF, SizeOfArray(CharactersKeys));

	for (mu_uint32 n = 0; n < mu_countof(RandomTable); ++n)
	{
		RandomTable[n] = GetRandom(0, 360);
	}

	return true;
}

static mu_boolean isFinished = false;

void MURoot::Run()
{
	while(!isFinished)
	{
		MU_INPUT->ProcessKeys();

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			MURoot::WindowProcess(&event);
		}

		if (EVideoBackend::GetVideoStatus(EVideoStatusFlag::VSF_Active) == true)
		{
			if (EVideoBackend::GetVideoStatus(EVideoStatusFlag::VSF_Visible) == true &&
				EVideoBackend::GetVideoStatus(EVideoStatusFlag::VSF_Available) == false)
			{
				if (_VideoManager->Initialize(_WindowManager->GetSDLWindow()) == false)
				{
					mu_error("Failed to initialize video.");
					break;
				}

				if (_FontFactory->CreateFonts() == false)
				{
					mu_error("Failed to initialize fonts.");
					break;
				}

				if (g_pMessageRoot != nullptr)
				{
					g_pMessageRoot->InitializeGPU();
				}

				if (g_pOptionRoot != nullptr)
				{
					g_pOptionRoot->InitializeGPU();
				}

				if (g_pLoginRoot != nullptr)
				{
					g_pLoginRoot->InitializeGPU();
				}

				if (g_pCharacterRoot != nullptr)
				{
					g_pCharacterRoot->InitializeGPU();
				}

				if (g_pNewUISystem != nullptr)
				{
					g_pNewUISystem->InitializeGPU();
				}

				for (mu_uint32 n = 0; n < MAX_MARKS; ++n)
				{
					TEXTURES::Data[BITMAP_GUILD_BEGIN + n].Create2D(GUILDMARK_WIDTH, GUILDMARK_HEIGHT, ETextureFormat::eRGBA8_Unorm, ETextureUsage::eDynamic, ESampler::eUcVcWcMinNMagNMipN, nullptr, 0);
					TEXTURES::Data[BITMAP_GUILD2_BEGIN + n].Create2D(GUILDMARK_WIDTH, GUILDMARK_HEIGHT, ETextureFormat::eRGBA8_Unorm, ETextureUsage::eDynamic, ESampler::eUcVcWcMinNMagNMipN, nullptr, 0);
					GuildMarkStagingBuffer[n][0].Allocate(sizeof(mu_uint8) * 4 * GUILDMARK_WIDTH * GUILDMARK_HEIGHT, true);
					GuildMarkStagingBuffer[n][1].Allocate(sizeof(mu_uint8) * 4 * GUILDMARK_WIDTH * GUILDMARK_HEIGHT, true);
				}

				if (_CurrentScene != &SCENES::LoadingScene)
				{
					mu_assert(g_LoadingManager.AddOnce(TEXTURES::Load, TEXTURES::LoadCount) == true);
					mu_assert(g_LoadingManager.AddOnce(INTERFACES::Load, INTERFACES::LoadCount) == true);

					g_LoadingManager.ProcessQueue();
					g_LoadingManager.RunLocked(LOADING_FLAGS::LF_NORMAL | LOADING_FLAGS::LF_MAPS);

					ProcessLoadingGPU();
					g_Terrain.InitializeGraphics();

					EVideoBackend::PrepareShaders();

					// Configure Ratios
					{
						MU_RESOURCE->CalculateAspectRatios();

						MU_VIDEO->ConfigureViewport2D();

						MUVideoManager::Prepare2DCamera();

						MU_ROOT->_FontFactory->CreateFonts();

						if (FONT_NORMAL != nullptr)
						{
							FONT_NORMAL->SystemConfigure();
						}
						if (FONT_BOLD != nullptr)
						{
							FONT_BOLD->SystemConfigure();
						}
						if (FONT_BIG != nullptr)
						{
							FONT_BIG->SystemConfigure();
						}
						if (FONT_NUMBER != nullptr)
						{
							FONT_NUMBER->SystemConfigure();
						}

						MU_INPUT->SystemConfigure();

						if (g_pMessageRoot != nullptr)
						{
							g_pMessageRoot->SystemConfigure();
						}

						if (g_pOptionRoot != nullptr)
						{
							g_pOptionRoot->SystemConfigure();
						}

						if (g_pLoginRoot != nullptr)
						{
							g_pLoginRoot->SystemConfigure();
						}

						if (g_pCharacterRoot != nullptr)
						{
							g_pCharacterRoot->SystemConfigure();
						}

						if (g_pNewUISystem != nullptr)
						{
							g_pNewUISystem->SystemConfigure();
						}
					}
				}

				EVideoBackend::SetVideoStatus(EVideoStatusFlag::VSF_Available);
			}
		}
		else
		{
			if (EVideoBackend::GetVideoStatus(EVideoStatusFlag::VSF_Available) == true)
			{
				g_LoadingManager._IgnoreData = true;
				g_LoadingManager.ProcessQueue();
				g_LoadingManager.WaitTillFinished();
				g_LoadingManager._IgnoreData = false;
				ClearLoadingGPU();
				EVideoBackend::UnsetVideoStatus(EVideoStatusFlag::VSF_Available);

				if (_FontFactory != nullptr)
				{
					_FontFactory->DestroyFonts();
				}

				g_Terrain.DestroyGraphics();

				g_pNormalWrapper->ClearCache();
				g_pBoldWrapper->ClearCache();
				g_pBigWrapper->ClearCache();
				g_pNumberWrapper->ClearCache();

				if (g_pMessageRoot != nullptr)
				{
					g_pMessageRoot->ReleaseGPU();
				}

				if (g_pOptionRoot != nullptr)
				{
					g_pOptionRoot->ReleaseGPU();
				}

				if (g_pLoginRoot != nullptr)
				{
					g_pLoginRoot->ReleaseGPU();
				}

				if (g_pCharacterRoot != nullptr)
				{
					g_pCharacterRoot->ReleaseGPU();
				}

				if (g_pNewUISystem != nullptr)
				{
					g_pNewUISystem->ReleaseGPU();
				}

				for (mu_uint32 index = 0; index < MAX_MODELS; ++index)
				{
					MODELS::Data[index].ReleaseGPU(true);
					MODELS::Data[index]._IsRequested.exchange(false, std::memory_order_relaxed);
				}

				for (mu_uint32 index = 0; index < MAX_TEXTURES; ++index)
				{
					TEXTURES::Data[index].Release(true);
				}

				for (mu_uint32 index = 0; index < INTERFACES::MAX; ++index)
				{
					INTERFACES::Data[index].Release(true);
				}

				for (mu_uint32 n = 0; n < MAX_MARKS; ++n)
				{
					GuildMarkStagingBuffer[n][0].Deallocate();
					GuildMarkStagingBuffer[n][1].Deallocate();
				}

				if (MU_VIDEO != nullptr)
				{
					MU_VIDEO->Shutdown(true);
				}
			}
		}

		if (_CurrentScene->IsLoaded() == false)
		{
#if __ANDROID__
			if (SDL_IsTextInputActive() == true)
			{
				SDL_StopTextInput();
			}
#endif

			_CurrentScene->OnLoad();
		}

		_GlobalTimer.Wait();
		g_LoadingManager.ProcessQueue();
		g_LoadingManager.Run();

		if (EVideoBackend::GetVideoStatus(EVideoStatusFlag::VSF_Available) == true)
		{
			ProcessLoadingGPU();
		}

		mu_double elapsedTime = _GlobalTimer.GetElapsedTime();
		mu_uint64 currentTime = _GlobalTimer.GetTime();

		EMusicBackend::Update();
		_InputManager->Update();
		_CurrentScene->OnUpdateGFX(currentTime);

		if (EVideoBackend::PreReload() == true)
		{
			while (SDL_PollEvent(&event))
			{
				MURoot::WindowProcess(&event);
			}

			if (EVideoBackend::ReloadWindowSize() == true)
			{
				while (SDL_PollEvent(&event))
				{
					MURoot::WindowProcess(&event);
				}
			}

			if (EVideoBackend::ReloadWindowMode() == true)
			{
				while (SDL_PollEvent(&event))
				{
					MURoot::WindowProcess(&event);
				}
			}

			EVideoBackend::Reload();
			EVideoBackend::ClearReloadRequests();
		}

		ProcessNetworkState();

		EVideoBackend::BeginFrame();

		EVideoBackend::DefragmentBuffer();
		EVideoBackend::UIPreUpdateBuffers();
		EVideoBackend::FontPreUpdateBuffers();

		_CurrentScene->OnUpdate(elapsedTime, currentTime);

		EVideoBackend::UIPostUpdateBuffers();
		EVideoBackend::FontPostUpdateBuffers();
		EVideoBackend::EndFrame();
		EVideoBackend::SubmitFrame();
		EVideoBackend::PresentFrame();
		EVideoBackend::CollectGarbage();

		_CurrentScene->OnPostUpdate();

		_InputManager->PostUpdate();

		if (_CurrentScene->RequireSceneSwitch() == true)
		{
			MUSceneBase *NextScene = _CurrentScene->NextScene();

			_CurrentScene->OnUnload();

			if (NextScene != _CurrentScene)
			{
				_CurrentScene = NextScene;
			}

			g_pOptionRoot->HideAll();
		}
	}

	ProcessLoadingGPU();

	// Prevent some mistakes with DirectX and Vulkan
	EVideoBackend::FinishAndWaitDevice();

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		MURoot::WindowProcess(&event);
	}

	if (_CurrentScene != nullptr)
	{
		_CurrentScene->OnUnload();
		_CurrentScene = nullptr;
	}
}

void MURoot::Destroy()
{
	if (g_ConnectServerObject != nullptr &&
		g_ConnectServerObject->IsConnected() == true)
	{
		g_NetworkClient->Disconnect(g_ConnectServerObject);
	}

	if (g_GameServerObject != nullptr &&
		g_GameServerObject->IsConnected() == true)
	{
		g_NetworkClient->Disconnect(g_GameServerObject);
	}

	g_NetworkManager->Destroy();

	MU_SAFE_DELETE(g_ConnectServerObject);
	MU_SAFE_DELETE(g_GameServerObject);
	MU_SAFE_DELETE(g_NetworkClient);
	MU_SAFE_DELETE(g_NetworkManager);

	MU_SAFE_DELETE(g_pNormalWrapper);
	MU_SAFE_DELETE(g_pBoldWrapper);
	MU_SAFE_DELETE(g_pBigWrapper);
	MU_SAFE_DELETE(g_pNumberWrapper);

	MU_SAFE_DELETE(g_pMessageRoot);
	MU_SAFE_DELETE(g_pOptionRoot);
	MU_SAFE_DELETE(g_pLoginRoot);
	MU_SAFE_DELETE(g_pCharacterRoot);
	MU_SAFE_DELETE(g_pNewUISystem);

	MU_SAFE_DELETE(CharacterMachine);

	g_Terrain.Unload(false);
	g_LoadingManager.Destroy();

#ifndef DISABLE_AUDIO
	for (mu_uint32 index = 0; index < MAX_SOUND_BUFFER; ++index)
	{
		MUAudioSource *o = &g_Sounds[index];
		MU_SAFE_DESTROY_VOICE(o->Source);
	}
#endif

	for (mu_uint32 index = 0; index < MAX_SOUNDS; ++index)
	{
		SOUNDS::Data[index].Unload();
	}

	for (mu_uint32 index = 0; index < MAX_MODELS; ++index)
	{
		MODELS::Data[index].Release();
	}

	for (mu_uint32 index = 0; index < MAX_TEXTURES; ++index)
	{
		TEXTURES::Data[index].Release(false);
	}

	for (mu_uint32 index = 0; index < INTERFACES::MAX; ++index)
	{
		INTERFACES::Data[index].Release(false);
	}

	for (mu_uint32 n = 0; n < MAX_MARKS; ++n)
	{
		GuildMarkStagingBuffer[n][0].Deallocate();
		GuildMarkStagingBuffer[n][1].Deallocate();
	}

	g_Tasking.DestroyInterface();
	g_Tasking.Destroy();

	DestroyGlobalCulling();

	if (_ResourceManager != nullptr)
	{
		_ResourceManager->DestroyTextures();

		delete _ResourceManager;
		_ResourceManager = nullptr;
	}

	MU_SAFE_DELETE(_InputManager);
	MU_SAFE_DELETE(_FontFactory);
	MU_SAFE_DELETE(_VideoManager);
#ifndef DISABLE_AUDIO
	MU_SAFE_DELETE(_AudioManager);
#endif
	EMusicBackend::Destroy();
	MU_SAFE_DELETE(_WindowManager);

#if defined(WIN32) || defined(WIN64)
	if (_Enumerator)
	{
		_Enumerator->UnregisterEndpointNotificationCallback(this);
		_Enumerator->Release();
		_Enumerator = nullptr;
	}
#endif

	SDL_Quit();

#if DIRECTX_BACKEND_ENABLE == 1
	CoUninitialize();
#endif
}

void MURoot::WindowProcess(const SDL_Event *event)
{
	mu_boolean isAnyPressed = false;

#if 1
#if defined(__ANDROID__)
	switch (event->type)
	{
	case SDL_MOUSEMOTION:
	case SDL_MOUSEWHEEL:
	case SDL_MOUSEBUTTONDOWN:
	case SDL_KEYDOWN:
	case SDL_KEYUP:
		break;

	case SDL_WINDOWEVENT:
		{
			__android_log_print(ANDROID_LOG_INFO, "Elion-Messages", "[Window Event] [0x%04X]\n", event->window.event);
		}
		break;

	default:
		{
			__android_log_print(ANDROID_LOG_INFO, "Elion-Messages", "[0x%04X]\n", event->type);
		}
		break;
	}
#else
	switch (event->type)
	{
	case SDL_MOUSEMOTION:
	case SDL_MOUSEWHEEL:
	case SDL_MOUSEBUTTONDOWN:
	case SDL_KEYDOWN:
	case SDL_KEYUP:
		break;

	case SDL_WINDOWEVENT:
		{
			mu_text buffer[1024];
			mu_sprintf_s(buffer, mu_countof(buffer), _T("[Window Event] [0x%04X]\n"), event->window.event);
			OutputDebugStringW(buffer);
		}
		break;

	default:
		{
			mu_text buffer[1024];
			mu_sprintf_s(buffer, mu_countof(buffer), _T("[0x%04X]\n"), event->type);
			OutputDebugStringW(buffer);
		}
		break;
	}
#endif
#endif

	switch (event->type)
	{
	case SDL_MOUSEMOTION:
		{
			MU_INPUT->SetMouse(event->motion.x, event->motion.y);

			MouseX = MU_TO_VIEWPORT(MU_INPUT->GetMouseX(), MU_RESOURCE->_640x480Rate[0]);
			MouseY = MU_TO_VIEWPORT(MU_INPUT->GetMouseY(), MU_RESOURCE->_640x480Rate[1]);

			if (MouseX < 0)
				MouseX = 0;
			else if (MouseX > 640)
				MouseX = 640;

			if (MouseY < 0)
				MouseY = 0;
			else if (MouseY > 480)
				MouseY = 480;
		}
		break;

	case SDL_MOUSEWHEEL:
		{
			MU_INPUT->SetMouseWheel(event->wheel.y);
		}
		break;

	case SDL_MOUSEBUTTONDOWN:
		{
			isAnyPressed = MU_INPUT->IsAnyMousePressed();
		}
		break;

	case SDL_KEYDOWN:
		{
			MU_INPUT->SetKeyDown(event->key.keysym.scancode);

			switch (event->key.keysym.scancode)
			{
			case SDL_SCANCODE_PRINTSCREEN:
				{
					// Avoid process when key is being hold
					if (MU_INPUT->IsKeyPressed(SDL_SCANCODE_PRINTSCREEN) == true)
					{
						g_Tasking.RequestTakeScreenshot();
					}
				}
				break;
			}
		}
		break;

	case SDL_KEYUP:
		{
			MU_INPUT->SetKeyUp(event->key.keysym.scancode);
		}
		break;
	}

	switch (event->type)
	{
#if ELION_MOBILE_DEVICE == 1
	case SDL_APP_WILLENTERBACKGROUND:
		{
			EVideoBackend::UnsetVideoStatus(EVideoStatusFlag::VSF_Active);
			EMusicBackend::Pause();
		}
		break;
#endif

#if ELION_MOBILE_DEVICE == 1
	case SDL_APP_WILLENTERFOREGROUND:
		{
			EVideoBackend::SetVideoStatus(EVideoStatusFlag::VSF_Active);
			EMusicBackend::Resume();
		}
		break;
#endif

	case SDL_WINDOWEVENT:
		{
			switch (event->window.event)
			{
			case SDL_WINDOWEVENT_FOCUS_GAINED:
				{
					EVideoBackend::SetVideoStatus(EVideoStatusFlag::VSF_Focused);
#if ELION_WINDOWS_DEVICE == 1
					EMusicBackend::Resume();
#endif
				}
				break;

			case SDL_WINDOWEVENT_FOCUS_LOST:
				{
					EVideoBackend::UnsetVideoStatus(EVideoStatusFlag::VSF_Focused);
#if ELION_WINDOWS_DEVICE == 1
					EMusicBackend::Pause();
#endif
					MU_INPUT->ClearKeyState();
					MU_INPUT->ClearMouseButton();
				}
				break;

			case SDL_WINDOWEVENT_MINIMIZED:
				{
					EVideoBackend::UnsetVideoStatus(EVideoStatusFlag::VSF_Visible);
				}
				break;

			case SDL_WINDOWEVENT_RESTORED:
				{
					EVideoBackend::SetVideoStatus(EVideoStatusFlag::VSF_Visible);
				}
				break;
			}
		}
		break;

	case SDL_MOUSEBUTTONDOWN:
		{
			switch (event->button.button)
			{
			case SDL_BUTTON_LEFT:
				{
					if (event->button.clicks < 2)
					{
						MU_INPUT->SetMouseButton(MOUSE_BUTTON_LEFT, MOUSE_STATE_CLICK);
					}
					else
					{
						MU_INPUT->SetMouseButton(MOUSE_BUTTON_LEFT, MOUSE_STATE_DOUBLECLICK);
					}
				}
				break;
			case SDL_BUTTON_MIDDLE:
				{
					if (event->button.clicks < 2)
					{
						MU_INPUT->SetMouseButton(MOUSE_BUTTON_MIDDLE, MOUSE_STATE_CLICK);
					}
					else
					{
						MU_INPUT->SetMouseButton(MOUSE_BUTTON_MIDDLE, MOUSE_STATE_DOUBLECLICK);
					}
				}
				break;
			case SDL_BUTTON_RIGHT:
				{
					if (event->button.clicks < 2)
					{
						MU_INPUT->SetMouseButton(MOUSE_BUTTON_RIGHT, MOUSE_STATE_CLICK);
					}
					else
					{
						MU_INPUT->SetMouseButton(MOUSE_BUTTON_RIGHT, MOUSE_STATE_DOUBLECLICK);
					}
				}
				break;
			}
		}
		break;

	case SDL_MOUSEBUTTONUP:
		{
			switch (event->button.button)
			{
			case SDL_BUTTON_LEFT:
				{
					MU_INPUT->SetMouseButton(MOUSE_BUTTON_LEFT, MOUSE_STATE_NONE);
				}
				break;
			case SDL_BUTTON_MIDDLE:
				{
					MU_INPUT->SetMouseButton(MOUSE_BUTTON_MIDDLE, MOUSE_STATE_NONE);
				}
				break;
			case SDL_BUTTON_RIGHT:
				{
					MU_INPUT->SetMouseButton(MOUSE_BUTTON_RIGHT, MOUSE_STATE_NONE);
				}
				break;
			}
		}
		break;

	case SDL_QUIT:
		{
			isFinished = true;
		}
		break;
	}

	mu_size result = 0;

	// Here should process 2D input(game user interface)
	mu_uint32 isProcessed = GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE;

	if (MU_ROOT->_CurrentScene != nullptr)
	{
		switch (event->type)
		{
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			{
				if (isAnyPressed == false ||
					MU_INPUT->Is3DMouse() == false)
				{
					isProcessed = MU_ROOT->_CurrentScene->OnWindowMessage(event, &result);
				}
			}
			break;

		default:
			isProcessed = MU_ROOT->_CurrentScene->OnWindowMessage(event, &result);
			break;
		}
	}

	switch (event->type)
	{
	case SDL_MOUSEBUTTONDOWN:
		{
			if (isAnyPressed == false)
			{
				if (isProcessed != GFX_MESSAGE_RETURN::GFX_MESSAGE_NONE)
				{
					MU_INPUT->Set3DMouse(false);
				}
				else
				{
					MU_INPUT->Set3DMouse(true);
				}
			}
		}
		break;
	}
}

#ifndef DISABLE_AUDIO
#if defined(WIN32) || defined(WIN64)
HRESULT MURoot::OnDefaultDeviceChanged(EDataFlow flow, ERole role, LPCWSTR pwstrDeviceId)
{
	MU_AUDIO->OnDefaultDeviceChanged(flow, role, pwstrDeviceId);
	EMusicBackend::DefaultDeviceChanged();

	return S_OK;
}
#endif
#endif