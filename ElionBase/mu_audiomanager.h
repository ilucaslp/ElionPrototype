#ifndef __MU_AUDIO_MANAGER_H__
#define __MU_AUDIO_MANAGER_H__

#pragma once

#ifndef DISABLE_AUDIO
#if !defined(__ANDROID__)
#include <mmdeviceapi.h>
#endif

#define CHANGE_AUDIO_BEHAVIOR

#define SPEAKER_5POINT0          ( SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT | SPEAKER_FRONT_CENTER | SPEAKER_SIDE_LEFT | SPEAKER_SIDE_RIGHT )
#define SPEAKER_6POINT1          ( SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT | SPEAKER_FRONT_CENTER | SPEAKER_LOW_FREQUENCY | SPEAKER_SIDE_LEFT | SPEAKER_SIDE_RIGHT | SPEAKER_BACK_CENTER )

#define INPUTCHANNELS 1
//#define ENABLE_REVERB

#define MU_SAFE_DESTROY_VOICE(p) \
	if (p) \
			{ \
		p->DestroyVoice(); \
		p = nullptr; \
			}

#define SPEAKER_COUNT 6

/**
* Derives the output matrix to use based on the channel mask and the number of channels
*/
extern const mu_float OutputMatrixMono[SPEAKER_COUNT];
extern const mu_float OutputMatrix2dot0[SPEAKER_COUNT * 2];
extern const mu_float OutputMatrix2dot1[SPEAKER_COUNT * 3];
extern const mu_float OutputMatrix4dot0s[SPEAKER_COUNT * 4];
extern const mu_float OutputMatrix4dot0[SPEAKER_COUNT * 4];
extern const mu_float OutputMatrix4dot1[SPEAKER_COUNT * 5];
extern const mu_float OutputMatrix5dot0[SPEAKER_COUNT * 5];
extern const mu_float OutputMatrix5dot1[SPEAKER_COUNT * 6];
extern const mu_float OutputMatrix5dot1s[SPEAKER_COUNT * 6];
extern const mu_float OutputMatrix6dot1[SPEAKER_COUNT * 7];
extern const mu_float OutputMatrix7dot1[SPEAKER_COUNT * 8];
extern const mu_float OutputMatrix7dot1s[SPEAKER_COUNT * 8];

class MUAudioManager : public IXAudio2EngineCallback, public IXAudio2VoiceCallback
{
public:
	MUAudioManager();
	~MUAudioManager();

	mu_boolean Initialize();
	void Shutdown();

	void Update();

	void SetMasterVolume(mu_float volume);

protected:
	friend class MURoot;
	void OnDefaultDeviceChanged(EDataFlow flow, ERole role, LPCWSTR pwstrDeviceId);

	STDMETHOD_(void, OnProcessingPassStart) () override {}
	STDMETHOD_(void, OnProcessingPassEnd)() override {}

	STDMETHOD_(void, OnCriticalError) (THIS_ HRESULT error);

	STDMETHOD_(void, OnVoiceProcessingPassStart) (UINT32) override {}
	STDMETHOD_(void, OnVoiceProcessingPassEnd)() override {}
	STDMETHOD_(void, OnStreamEnd)() override {}
	STDMETHOD_(void, OnBufferStart)(void*) override {}
	STDMETHOD_(void, OnBufferEnd)(void *context) override;
	STDMETHOD_(void, OnLoopEnd)(void *context) override;
	STDMETHOD_(void, OnVoiceError)(void*, HRESULT) override {}

private:
	friend class MURoot;
	friend class MUTasking;
	friend class MUAudioSource;

	IXAudio2 *_AudioEngine;
	IXAudio2MasteringVoice *_MasteringVoice;
#ifdef ENABLE_REVERB
	IUnknown *_ReverbEffect;
	IXAudio2SubmixVoice *_SubmixVoice;
#endif

	X3DAUDIO_HANDLE _X3DAudio;

	mu_boolean _Running;
	mu_boolean _DeviceChanged;
	mu_boolean _DeviceReport;
	mu_boolean _FinishedVoices;

	mu_uint16 _MasterChannels;
	mu_uint32 _MasterChannelMask;
	mu_uint32 _MasterRate;

	X3DAUDIO_LISTENER _Listener;
	X3DAUDIO_EMITTER _Emitter;

	IMMDeviceEnumerator *_Enumerator;
	mu_unicodestr _LastDevice;
	MUCriticalSectionSpinLock _DeviceCritical;

	const mu_float *_MatrixCoefficients;

#ifdef CHANGE_AUDIO_BEHAVIOR
	volatile LONG _CriticalError, _VoiceFinished;
#else
	HANDLE _CriticalError, _VoiceFinished;
#endif

	static HMODULE _XAudio2Dll;
};
#endif

#endif