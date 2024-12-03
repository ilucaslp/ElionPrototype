#include "stdafx.h"
#include "mu_audiomanager.h"

#ifndef DISABLE_AUDIO
/**
* Derives the output matrix to use based on the channel mask and the number of channels
*/
const mu_float OutputMatrixMono[SPEAKER_COUNT] =
{
	0.7f, 0.7f, 0.5f, 0.0f, 0.5f, 0.5f
};

const mu_float OutputMatrix2dot0[SPEAKER_COUNT * 2] =
{
	1.0f, 0.0f, 0.7f, 0.0f, 1.25f, 0.0f, // FL 
	0.0f, 1.0f, 0.7f, 0.0f, 0.0f, 1.25f  // FR
};

const mu_float OutputMatrix2dot1[SPEAKER_COUNT * 3] =
{
	// Same as stereo, but also passing in LFE signal
	1.0f, 0.0f, 0.7f, 0.0f, 1.25f, 0.0f, // FL
	0.0f, 1.0f, 0.7f, 0.0f, 0.0f, 1.25f, // FR
	0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // LFE
};

const mu_float OutputMatrix4dot0s[SPEAKER_COUNT * 4] =
{
	// Combine both rear channels to make a rear center channel
	1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // FL
	0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // FR
	0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // FC
	0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f  // RC
};

const mu_float OutputMatrix4dot0[SPEAKER_COUNT * 4] =
{
	// Split the center channel to the front two speakers
	1.0f, 0.0f, 0.7f, 0.0f, 0.0f, 0.0f, // FL
	0.0f, 1.0f, 0.7f, 0.0f, 0.0f, 0.0f, // FR
	0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, // RL
	0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f  // RR
};

const mu_float OutputMatrix4dot1[SPEAKER_COUNT * 5] =
{
	// Split the center channel to the front two speakers
	1.0f, 0.0f, 0.7f, 0.0f, 0.0f, 0.0f, // FL
	0.0f, 1.0f, 0.7f, 0.0f, 0.0f, 0.0f, // FR
	0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // LFE
	0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, // RL
	0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f  // RR
};

const mu_float OutputMatrix5dot0[SPEAKER_COUNT * 5] =
{
	// Split the center channel to the front two speakers
	1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // FL
	0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // FR
	0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // FC
	0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, // SL
	0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f  // SR
};

const mu_float OutputMatrix5dot1[SPEAKER_COUNT * 6] =
{
	// Classic 5.1 setup
	1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // FL
	0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // FR
	0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // FC
	0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // LFE
	0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, // RL
	0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f  // RR
};

const mu_float OutputMatrix5dot1s[SPEAKER_COUNT * 6] =
{
	// Classic 5.1 setup
	1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // FL
	0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // FR
	0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // FC
	0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // LFE
	0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, // SL
	0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f  // SR
};

const mu_float OutputMatrix6dot1[SPEAKER_COUNT * 7] =
{
	1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // FL
	0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // FR
	0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // FC
	0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // LFE
	0.0f, 0.0f, 0.0f, 0.0f, 0.8f, 0.0f, // RL
	0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.8f, // RR
	0.0f, 0.0f, 0.0f, 0.0f, 0.5f, 0.5f, // RC
};

const mu_float OutputMatrix7dot1[SPEAKER_COUNT * 8] =
{
	0.7f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // FL
	0.0f, 0.7f, 0.0f, 0.0f, 0.0f, 0.0f, // FR
	0.0f, 0.0f, 0.7f, 0.0f, 0.0f, 0.0f, // FC
	0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // LFE
	0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, // RL
	0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, // RR
	0.7f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, // FCL
	0.0f, 0.7f, 0.5f, 0.0f, 0.0f, 0.0f  // FCR
};

const mu_float OutputMatrix7dot1s[SPEAKER_COUNT * 8] =
{
	// Split the rear channels evenly between side and rear
	1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // FL
	0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // FR
	0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // FC
	0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // LFE
	0.0f, 0.0f, 0.0f, 0.0f, 0.7f, 0.0f, // RL
	0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.7f, // RR
	0.0f, 0.0f, 0.0f, 0.0f, 0.7f, 0.0f, // SL
	0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.7f  // SR
};

typedef struct SOuputMapping
{
	mu_uint32 NumChannels;
	mu_uint32 SpeakerMask;
	const mu_float* OuputMatrix;
} TOuputMapping;

TOuputMapping OutputMappings[] =
{
	{ 1, SPEAKER_MONO, OutputMatrixMono },
	{ 2, SPEAKER_STEREO, OutputMatrix2dot0 },
	{ 3, SPEAKER_2POINT1, OutputMatrix2dot1 },
	{ 4, SPEAKER_SURROUND, OutputMatrix4dot0s },
	{ 4, SPEAKER_QUAD, OutputMatrix4dot0 },
	{ 5, SPEAKER_4POINT1, OutputMatrix4dot1 },
	{ 5, SPEAKER_5POINT0, OutputMatrix5dot0 },
	{ 6, SPEAKER_5POINT1, OutputMatrix5dot1 },
	{ 6, SPEAKER_5POINT1_SURROUND, OutputMatrix5dot1s },
	{ 7, SPEAKER_6POINT1, OutputMatrix6dot1 },
	{ 8, SPEAKER_7POINT1, OutputMatrix7dot1 },
	{ 8, SPEAKER_7POINT1_SURROUND, OutputMatrix7dot1s }
};

const mu_float *GetOutputMatrix(mu_uint32 ChannelMask, mu_uint32 NumChannels)
{
	const mu_float *OutputMatrix = OutputMatrix2dot0;

	for (mu_uint32 MappingIndex = 0; MappingIndex < mu_countof(OutputMappings); ++MappingIndex)
	{
		if (NumChannels != OutputMappings[MappingIndex].NumChannels)
		{
			continue;
		}

		if ((ChannelMask & OutputMappings[MappingIndex].SpeakerMask) != ChannelMask)
		{
			continue;
		}

		OutputMatrix = OutputMappings[MappingIndex].OuputMatrix;
		break;
	}

	return OutputMatrix;
}

HMODULE MUAudioManager::_XAudio2Dll = nullptr;

MUAudioManager::MUAudioManager()
{
	_AudioEngine = nullptr;
	_MasteringVoice = nullptr;
#ifdef ENABLE_REVERB
	_ReverbEffect = nullptr;
	_SubmixVoice = nullptr;
#endif

	mu_zeromem(&_X3DAudio, X3DAUDIO_HANDLE_BYTESIZE);
	mu_zeromem(&_Listener, sizeof(_Listener));
	mu_zeromem(&_Emitter, sizeof(_Emitter));

	_Listener.OrientFront.x = 0.0f;
	_Listener.OrientFront.y = 0.0f;
	_Listener.OrientFront.z = -1.0f;

	_Listener.OrientTop.x = 0.0f;
	_Listener.OrientTop.y = 1.0f;
	_Listener.OrientTop.z = 0.0f;

	_Emitter.OrientFront.x = 0.0f;
	_Emitter.OrientFront.y = 0.0f;
	_Emitter.OrientFront.z = -1.0f;

	_Emitter.OrientTop.x = 0.0f;
	_Emitter.OrientTop.y = 1.0f;
	_Emitter.OrientTop.z = 0.0f;

	_Emitter.CurveDistanceScaler = 1.0f;
	_Emitter.DopplerScaler = 1.0f;
	_Emitter.ChannelRadius = 1.0f;

	_Emitter.InnerRadiusAngle = X3DAUDIO_PI / 4.0f;

	_Running = false;
	_DeviceChanged = false;
	_DeviceReport = false;
	_FinishedVoices = false;

	_MatrixCoefficients = nullptr;

#ifdef CHANGE_AUDIO_BEHAVIOR
	_CriticalError = 0;
	_VoiceFinished = 0;
#else
	_CriticalError = CreateEventEx(nullptr, nullptr, 0, EVENT_MODIFY_STATE | SYNCHRONIZE);
	_VoiceFinished = CreateEventEx(nullptr, nullptr, 0, EVENT_MODIFY_STATE | SYNCHRONIZE);
#endif
}

MUAudioManager::~MUAudioManager()
{
	Shutdown();

	if (_XAudio2Dll != nullptr)
	{
		FreeLibrary(_XAudio2Dll);
	}

#ifndef CHANGE_AUDIO_BEHAVIOR
	MU_SAFE_CLOSE(_CriticalError);
	MU_SAFE_CLOSE(_VoiceFinished);
#endif
}

mu_boolean MUAudioManager::Initialize()
{
	mu_uint32 flags = 0;

#if (_WIN32_WINNT < _WIN32_WINNT_WIN8) && !defined(NDEBUG)
	//flags |= XAUDIO2_DEBUG_ENGINE;
#endif

	if (_XAudio2Dll == nullptr)
	{
#ifndef NDEBUG
		_XAudio2Dll = (flags & XAUDIO2_DEBUG_ENGINE) != 0 ?
						LoadLibraryExA("XAudioD2_7.dll", nullptr, 0x00000800 /* LOAD_LIBRARY_SEARCH_SYSTEM32 */) :
						LoadLibraryExA("XAudio2_7.dll", nullptr, 0x00000800 /* LOAD_LIBRARY_SEARCH_SYSTEM32 */);
#else
		_XAudio2Dll = LoadLibraryExA("XAudio2_7.dll", nullptr, 0x00000800 /* LOAD_LIBRARY_SEARCH_SYSTEM32 */);
#endif

		if (_XAudio2Dll == nullptr)
		{
			return false;
		}
	}

	HRESULT hr = XAudio2Create(&_AudioEngine, flags);

	if (FAILED(hr))
	{
		Shutdown();
		return false;
	}

#if !defined(NDEBUG)
 	//XAUDIO2_DEBUG_CONFIGURATION debug = { 0 };
 	//debug.TraceMask = XAUDIO2_LOG_ERRORS | XAUDIO2_LOG_WARNINGS;
 	//debug.BreakMask = XAUDIO2_LOG_ERRORS;
 	//_AudioEngine->SetDebugConfiguration(&debug, nullptr);
#endif

	hr = _AudioEngine->RegisterForCallbacks(this);
	if (FAILED(hr))
	{
		Shutdown();
		return false;
	}

#if (_WIN32_WINNT >= _WIN32_WINNT_WIN8)
	hr = _AudioEngine->CreateMasteringVoice(&_MasteringVoice,
											XAUDIO2_DEFAULT_CHANNELS,
											XAUDIO2_DEFAULT_SAMPLERATE,
											0, nullptr, nullptr, AudioCategory_GameEffects);
	if (FAILED(hr))
	{
		Shutdown();
		return false;
	}

	hr = _MasteringVoice->GetChannelMask(&channelMask);
	if (FAILED(hr))
	{
		Shutdown();
		return false;
	}

	XAUDIO2_VOICE_DETAILS details;
	_MasteringVoice->GetVoiceDetails(&details);

	_MasterChannelMask = dwChannelMask;
	_MasterChannels = details.InputChannels;
	_MasterRate = details.InputSampleRate;
#else
	XAUDIO2_DEVICE_DETAILS details = { 0 };
	hr = _AudioEngine->GetDeviceDetails(0, &details);
	if (FAILED(hr))
	{
		Shutdown();
		return false;
	}

	_MasterChannelMask = details.OutputFormat.dwChannelMask;

	hr = _AudioEngine->CreateMasteringVoice(&_MasteringVoice,
		XAUDIO2_DEFAULT_CHANNELS,
		XAUDIO2_DEFAULT_SAMPLERATE,
		0, 0, nullptr);
	if (FAILED(hr))
	{
		Shutdown();
		return false;
	}

	XAUDIO2_VOICE_DETAILS voiceDetails;
	_MasteringVoice->GetVoiceDetails(&voiceDetails);

	_MasterChannels = voiceDetails.InputChannels;
	_MasterRate = voiceDetails.InputSampleRate;
#endif

#ifdef ENABLE_REVERB
	mu_uint32 reverbFlags = 0;

#if (_WIN32_WINNT < _WIN32_WINNT_WIN8) && !defined(NDEBUG)
	reverbFlags |= XAUDIO2FX_DEBUG;
#endif

	hr = XAudio2CreateReverb(&_ReverbEffect, reverbFlags);

	if (FAILED(hr))
	{
		Shutdown();
		return false;
	}

	XAUDIO2_EFFECT_DESCRIPTOR effects[] = { {_ReverbEffect, true, 2} };
	XAUDIO2_EFFECT_CHAIN effectChain = { mu_countof(effects), effects };

	hr = _AudioEngine->CreateSubmixVoice(&_SubmixVoice, 2, _MasterRate, 0, 0, nullptr, &effectChain);

	if (FAILED(hr))
	{
		Shutdown();
		return false;
	}

	static XAUDIO2FX_REVERB_I3DL2_PARAMETERS ReverbPreset = XAUDIO2FX_I3DL2_PRESET_FOREST;

	XAUDIO2FX_REVERB_PARAMETERS native;
	ReverbConvertI3DL2ToNative(&ReverbPreset, &native);
	hr = _SubmixVoice->SetEffectParameters(0, &native, sizeof(XAUDIO2FX_REVERB_PARAMETERS));

	if (FAILED(hr))
	{
		Shutdown();
		return false;
	}
#endif

#if (_WIN32_WINNT >= _WIN32_WINNT_WIN8)
	hr = X3DAudioInitialize(_MasterChannelMask, X3DAUDIO_SPEED_OF_SOUND, _X3DAudio);
	if (FAILED(hr))
	{
		Shutdown();
		return hr;
	}
#else
	X3DAudioInitialize(_MasterChannelMask, X3DAUDIO_SPEED_OF_SOUND, _X3DAudio);
#endif

	_Running = true;

	_MatrixCoefficients = GetOutputMatrix(_MasterChannelMask, _MasterChannels);

	return true;
}

void MUAudioManager::Shutdown()
{
	if (_AudioEngine != nullptr)
	{
		_AudioEngine->UnregisterForCallbacks(this);

		_AudioEngine->StopEngine();

#ifdef ENABLE_REVERB
		MU_SAFE_DESTROY_VOICE(_SubmixVoice);
#endif
		MU_SAFE_DESTROY_VOICE(_MasteringVoice);
	}

#ifdef ENABLE_REVERB
	MU_SAFE_RELEASE(_ReverbEffect);
#endif
	MU_SAFE_RELEASE(_AudioEngine);

	mu_zeromem(&_X3DAudio, X3DAUDIO_HANDLE_BYTESIZE);

	_Running = false;
}

void MUAudioManager::Update()
{
#if 1
	_DeviceCritical.Lock();
	if (_DeviceReport == true)
	{
		_DeviceChanged = true;
		_DeviceReport = false;
	}
	_DeviceCritical.Unlock();
#endif

	LONG CriticalFound = 0;

	if(_Running == true)
	{
#ifdef CHANGE_AUDIO_BEHAVIOR
		CriticalFound = InterlockedExchange(&_CriticalError, 0);

		if (CriticalFound == 0)
		{
			_FinishedVoices = InterlockedExchange(&_VoiceFinished, 0) == 1;
		}
#else
		HANDLE events[] = { _CriticalError, _VoiceFinished };
		DWORD result = WaitForMultipleObjectsEx(mu_countof(events), events, FALSE, 0, FALSE);
		switch (result)
		{
		case WAIT_OBJECT_0:
			CriticalFound = true;
			break;

		case WAIT_OBJECT_0 + 1:
			_FinishedVoices = true;
			break;
		}
#endif
	}

	if (CriticalFound == 1 || _DeviceChanged == true)
	{
		mu_uint32 beginIndex, endIndex;
		HRESULT hr;

		for (mu_uint32 i = 0; i < g_SoundsCurCount; ++i)
		{
			MUAudioSource *o = &g_Sounds[i];

			if (o->Live == true)
			{
				o->Delete();
			}
		}

		if (g_SoundsDeleteCount.exchange(0) > 0)
		{
			for (mu_uint32 i = 0; i < g_SoundsCurCount; ++i)
			{
				MUAudioSource *o = &g_Sounds[i];

				if (o->Live == false &&
					o->IsFree == false)
				{
					o->IsFree = true;

					g_SoundsFree[g_SoundsFreeCount++] = i;
				}
			}
		}

		MU_AUDIO->Shutdown();

		if (_DeviceChanged == true)
		{
			MU_AUDIO->Initialize();
			MU_AUDIO->SetMasterVolume(MU_RESOURCE->GetSoundVolume());
			_DeviceChanged = false;
		}
	}
}

void MUAudioManager::SetMasterVolume(mu_float volume)
{
	if (_MasteringVoice != nullptr)
	{
		_MasteringVoice->SetVolume(volume);
	}
}

void MUAudioManager::OnDefaultDeviceChanged(EDataFlow flow, ERole role, LPCWSTR pwstrDeviceId)
{
	mu_string deviceId;
	if (pwstrDeviceId == nullptr)
	{
		deviceId = _T("");
	}
	else
	{
		deviceId = pwstrDeviceId;
	}

	if (flow == eRender &&
		_LastDevice.compare(deviceId) != 0)
	{
		_LastDevice = deviceId;

		_DeviceCritical.Lock();
		_DeviceReport = true;
		_DeviceCritical.Unlock();
	}
}

void MUAudioManager::OnCriticalError(HRESULT error)
{
	UNREFERENCED_PARAMETER(error);

#ifdef CHANGE_AUDIO_BEHAVIOR
	InterlockedExchange(&_CriticalError, 1);
#else
	SetEvent(_CriticalError);
#endif
}

void MUAudioManager::OnBufferEnd(void *context)
{
#ifdef CHANGE_AUDIO_BEHAVIOR
	InterlockedExchange(&_VoiceFinished, 1);
#else
	SetEvent(_VoiceFinished);
#endif
}

void MUAudioManager::OnLoopEnd(void *context)
{
	
}
#endif