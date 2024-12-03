#ifndef __MU_MUSIC_BACKEND_H__
#define __MU_MUSIC_BACKEND_H__

#pragma once

enum EMusicStatusFlag : mu_uint32
{
	MSF_Initialized			= (1 << 0),
	MSF_Active				= (MSF_Initialized << 1),
	MSF_Playing				= (MSF_Active << 1),
	MSF_FileStreaming		= (MSF_Playing << 1),
	MSF_AudioStreaming		= (MSF_FileStreaming << 1),
	MSF_FileAvailable		= (MSF_AudioStreaming << 1),
	MSF_DeviceAvailable		= (MSF_FileAvailable << 1),

	MSF_MaskAll = 0xFFFFFFFF,
};

enum EMusicRequestFlag : mu_uint32
{
	MRF_ChangeDevice	= (1 << 0),
	MRF_Pause			= (MRF_ChangeDevice << 1),
	MRF_Play			= (MRF_Pause << 1),

	MRF_MaskAll			= 0xFFFFFFFF,
};

enum EMusicStopFlag : mu_uint32
{
	Stop_FileStream = (1 << 0),
	Stop_AudioStream = (1 << 1),
};

namespace EMusicBackend
{
	// Multiplier used for convert to Integer 32 Bits
	constexpr mu_float MasterVolumeMultiplier = 10000.0f;
	constexpr mu_float MasterVolumeMultiplierInverse = 1.0f / MasterVolumeMultiplier;
	constexpr mu_int32 WordSize = 2;
	constexpr mu_int32 ReadBufferSize = 8 * 1024; // 8KB

	extern mu_atomic_int32_t MasterVolume;
	extern mu_uint32 MusicStatusFlag;
	extern mu_uint32 MusicRequestFlag;

	const mu_boolean Initialize();
	void Destroy();

	void Update();

	void Play(const mu_text *filename = nullptr);
	void Stop(const mu_text *filename = nullptr, const mu_uint32 flag = EMusicStopFlag::Stop_FileStream | EMusicStopFlag::Stop_AudioStream);
	void Resume();
	void Pause();

	void DefaultDeviceChanged();
	void DeviceAdded(const SDL_AudioDeviceEvent &event);
	void DeviceRemoved(const SDL_AudioDeviceEvent &event);

	EINLINE void SetMusicVolume(const mu_float volume)
	{
		MasterVolume = static_cast<mu_int32>(volume * MasterVolumeMultiplier);
	}

	EINLINE void SetMusicStatus(const mu_uint32 flag)
	{
		MusicStatusFlag |= flag;
	}

	EINLINE void UnsetMusicStatus(const mu_uint32 flag)
	{
		MusicStatusFlag &= ~flag;
	}

	EINLINE const mu_boolean GetMusicStatus(const mu_uint32 flag)
	{
		return (MusicStatusFlag & flag) == flag;
	}

	EINLINE const mu_boolean GetMusicStatusAny(const mu_uint32 flag)
	{
		return (MusicStatusFlag & flag) != 0;
	}

	EINLINE void SetMusicRequest(const mu_uint32 flag)
	{
		MusicRequestFlag |= flag;
	}

	EINLINE void UnsetMusicRequest(const mu_uint32 flag)
	{
		MusicRequestFlag &= ~flag;
	}

	EINLINE const mu_boolean GetMusicRequest(const mu_uint32 flag)
	{
		return (MusicRequestFlag & flag) == flag;
	}

	EINLINE const mu_boolean GetMusicRequestAny(const mu_uint32 flag)
	{
		return (MusicRequestFlag & flag) != flag;
	}
};

#endif