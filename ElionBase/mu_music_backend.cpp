#include "stdafx.h"
#include "mu_music_backend.h"

namespace EMusicBackend
{
	mu_atomic_int32_t MasterVolume(1.0f * MasterVolumeMultiplier);
	mu_uint32 MusicStatusFlag = 0;
	mu_uint32 MusicRequestFlag = 0;
	mu_uint32 AudioDevicesCount = 0;

	std::thread StreamThread;
	MUThreadBarrier *StreamThreadBarrier = nullptr;
	mu_atomic_bool StreamThreadTerminate(false);
	mu_atomic_bool StreamThreadAbort(false);
	mu_atomic_bool StreamCallbackAbort(false);

	MUCriticalSectionSpinLock FileCritical;
	mu_string FilePath;
	mu_uint8 *FileBuffer = nullptr;
	mu_uint32 FileBufferSize = 0;
	mu_uint32 FileBufferTotalSize = 0;
	mu_atomic_uint32_t FileBufferAvailableSize = 0;
	mu_atomic_uint32_t FileBufferProcessedSize = 0;
	vorbis_info FileInfo;

	PaStream *Stream = nullptr;

	void StreamProcessing();
	int StreamCallback(const void *input,
					   void *output,
					   unsigned long frameCount,
					   const PaStreamCallbackTimeInfo *timeInfo,
					   PaStreamCallbackFlags statusFlags,
					   void *userData);

	const mu_boolean Initialize()
	{
		if (GetMusicStatus(EMusicStatusFlag::MSF_Initialized) == true)
		{
			return true;
		}

		if (Pa_Initialize() != paNoError)
		{
			return false;
		}

		SetMusicStatus(EMusicStatusFlag::MSF_Initialized | EMusicStatusFlag::MSF_Active);

		StreamThreadBarrier = new MUThreadBarrier(2);
		StreamThread = std::thread(StreamProcessing);

		if (Pa_GetDefaultOutputDevice() != paNoDevice)
		{
			SetMusicStatus(EMusicStatusFlag::MSF_DeviceAvailable);
		}

		return true;
	}

	void Destroy()
	{
		if (GetMusicStatus(EMusicStatusFlag::MSF_Initialized) == true)
		{
			Stop();

			StreamThread.join();
			MU_SAFE_DELETE(StreamThreadBarrier);

			Pa_Terminate();
			UnsetMusicStatus(EMusicStatusFlag::MSF_Initialized);

			if (FileBuffer != nullptr)
			{
				mu_free(FileBuffer);
				FileBuffer = nullptr;
			}
			FileBufferSize = 0;
			UnsetMusicStatus(EMusicStatusFlag::MSF_MaskAll);
		}
	}

	void Update()
	{
		if (GetMusicRequest(EMusicRequestFlag::MRF_ChangeDevice) == true)
		{
			if (GetMusicStatus(EMusicStatusFlag::MSF_FileAvailable) == true)
			{
				Stop(nullptr, EMusicStopFlag::Stop_AudioStream);

				UnsetMusicStatus(EMusicStatusFlag::MSF_DeviceAvailable);
			}

			UnsetMusicRequest(EMusicRequestFlag::MRF_ChangeDevice);
		}

		if (GetMusicStatus(EMusicStatusFlag::MSF_Active | EMusicStatusFlag::MSF_FileAvailable) == true &&
			GetMusicStatus(EMusicStatusFlag::MSF_DeviceAvailable) == false)
		{
			if (Pa_GetDefaultOutputDevice() != paNoDevice)
			{
				SetMusicStatus(EMusicStatusFlag::MSF_DeviceAvailable);
				Resume();
			}
		}
	}

	void Play(const mu_text *filename)
	{
		if (filename != nullptr)
		{
			mu_string path = filename;
			std::transform(path.begin(), path.end(), path.begin(), mu_tolower);

			if (GetMusicStatus(EMusicStatusFlag::MSF_FileAvailable) == true &&
				FilePath.compare(path) == 0)
			{
				if (GetMusicStatus(EMusicStatusFlag::MSF_DeviceAvailable) == false)
				{
					return;
				}

				if (Stream != nullptr &&
					Pa_IsStreamActive(Stream) == 1)
				{
					return;
				}

				mu_uint32 expectedSize = FileBufferTotalSize;
				if (FileBufferProcessedSize.compare_exchange_strong(expectedSize, 0) == false)
				{
					SetMusicStatus(EMusicStatusFlag::MSF_DeviceAvailable);
					return;
				}

				UnsetMusicStatus(EMusicStatusFlag::MSF_AudioStreaming);
			}
			else
			{
				if (GetMusicStatusAny(EMusicStatusFlag::MSF_Playing | EMusicStatusFlag::MSF_AudioStreaming | EMusicStatusFlag::MSF_FileStreaming) == true)
				{
					Stop();
				}

				FILE *file = nullptr;

				if (mu_fopen_s(&file, path.c_str(), "rb") == false)
				{
					return;
				}

				OggVorbis_File ovfile = {};
				if (ov_open(file, &ovfile, nullptr, 0) < 0)
				{
					mu_fclose(file);
					return;
				}

				mu_memcpy(&FileInfo, ov_info(&ovfile, -1), sizeof(vorbis_info));

				mu_int64 samplesCount = ov_pcm_total(&ovfile, -1);
				mu_uint32 bufferSize = static_cast<mu_uint32>(samplesCount * FileInfo.channels * WordSize);

				if (FileBufferSize < bufferSize)
				{
					mu_uint8 *buffer = reinterpret_cast<mu_uint8*>(mu_realloc(FileBuffer, bufferSize));
					if (buffer != nullptr)
					{
						FileBuffer = buffer;
						FileBufferSize = bufferSize;
					}
				}

				ov_clear(&ovfile);
				mu_fclose(file);

				if (FileBufferSize < bufferSize)
				{
					return;
				}

				FileCritical.Lock();
				FilePath = path;
				FileCritical.Unlock();

				FileBufferTotalSize = bufferSize;
				FileBufferAvailableSize = 0;
				FileBufferProcessedSize = 0;

				SetMusicStatus(EMusicStatusFlag::MSF_FileAvailable | EMusicStatusFlag::MSF_FileStreaming);
				StreamThreadBarrier->SyncSemaphore();
			}
		}

		if (GetMusicStatus(EMusicStatusFlag::MSF_FileAvailable) == true &&
			GetMusicStatus(EMusicStatusFlag::MSF_AudioStreaming) == false)
		{
			SetMusicStatus(EMusicStatusFlag::MSF_Playing);

			if (GetMusicStatus(EMusicStatusFlag::MSF_Active) == true)
			{
				PaError result = Pa_OpenDefaultStream(&Stream, 0, FileInfo.channels, paInt16, FileInfo.rate, paFramesPerBufferUnspecified, StreamCallback, nullptr);

				if (result != paNoError)
				{
					UnsetMusicStatus(EMusicStatusFlag::MSF_DeviceAvailable);
					return;
				}

				Pa_StartStream(Stream);
				SetMusicStatus(EMusicStatusFlag::MSF_AudioStreaming);
			}
		}
	}

	void Stop(const mu_text *filename, const mu_uint32 flag)
	{
		if (filename != nullptr)
		{
			mu_string filePath = filename;
			std::transform(filePath.begin(), filePath.end(), filePath.begin(), mu_tolower);

			if (FilePath.compare(filePath) != 0)
			{
				return;
			}
		}

		if ((flag & EMusicStopFlag::Stop_FileStream) != 0)
		{
			FileCritical.Lock();
			FilePath.clear();
			FileCritical.Unlock();

			if (GetMusicStatus(EMusicStatusFlag::MSF_FileStreaming) == true)
			{
				StreamThreadAbort = true;
				StreamThreadBarrier->SyncSemaphore();
				UnsetMusicStatus(EMusicStatusFlag::MSF_FileStreaming);
			}
		}

		if ((flag & EMusicStopFlag::Stop_AudioStream) != 0)
		{
			if (GetMusicStatus(EMusicStatusFlag::MSF_Playing) == true)
			{
				if (Stream != nullptr)
				{
					if (GetMusicStatus(EMusicStatusFlag::MSF_AudioStreaming) == true &&
						Pa_IsStreamActive(Stream) == 1)
					{
						StreamCallbackAbort = true;
						do
						{
							if (Pa_IsStreamActive(Stream) != 1)
							{
								break;
							}

							mu_sleep(0);
						} while (true);
						StreamCallbackAbort = false;
					}

					Pa_CloseStream(Stream);
					Stream = nullptr;
				}

				UnsetMusicStatus(EMusicStatusFlag::MSF_AudioStreaming);

				if ((flag & EMusicStopFlag::Stop_FileStream) != 0)
				{
					UnsetMusicStatus(EMusicStatusFlag::MSF_Playing);
				}
			}
		}
	}

	void Resume()
	{
		SetMusicStatus(EMusicStatusFlag::MSF_Active);
		Play(nullptr);
	}

	void Pause()
	{
		UnsetMusicStatus(EMusicStatusFlag::MSF_Active);
		Stop(nullptr, EMusicStopFlag::Stop_AudioStream);
	}

	void DefaultDeviceChanged()
	{
		SetMusicRequest(EMusicRequestFlag::MRF_ChangeDevice);
	}

	void DeviceAdded(const SDL_AudioDeviceEvent &event)
	{
		if (event.iscapture != 0) return;

		++AudioDevicesCount;
		if (GetMusicStatus(EMusicStatusFlag::MSF_FileAvailable) == true &&
			GetMusicStatus(EMusicStatusFlag::MSF_DeviceAvailable) == false)
		{
			Update();
		}
	}

	void DeviceRemoved(const SDL_AudioDeviceEvent &event)
	{
		if (event.iscapture != 0) return;

		--AudioDevicesCount;
		if (GetMusicStatus(EMusicStatusFlag::MSF_FileAvailable | EMusicStatusFlag::MSF_DeviceAvailable) == true)
		{
			if (Stream != nullptr &&
				Pa_IsStreamActive(Stream) == 1)
			{
				return;
			}

			UnsetMusicStatus(EMusicStatusFlag::MSF_DeviceAvailable);

			if (AudioDevicesCount > 0)
			{
				Update();
			}
		}
	}

	void StreamProcessing()
	{
		mu_int8 *buffer = new mu_int8[ReadBufferSize];

		while (true)
		{
			StreamThreadBarrier->SyncSemaphore();

			if (StreamThreadTerminate == true)
			{
				break;
			}

			FileCritical.Lock();
			mu_string filePath = FilePath;
			FileCritical.Unlock();

			if (StreamThreadAbort.exchange(false) == true ||
				filePath.empty() == true)
			{
				continue;
			}

			FILE *file = nullptr;
			if (mu_fopen_s(&file, filePath.c_str(), "rb") == false)
			{
				continue;
			}

			OggVorbis_File ovfile = {};
			if (ov_open(file, &ovfile, nullptr, 0) == 0)
			{
				const mu_uint32 bufferTotalSize = FileBufferTotalSize;
				mu_uint32 bufferIndex = FileBufferAvailableSize;
				ov_pcm_seek(&ovfile, static_cast<mu_int64>(bufferIndex));

				mu_int32 bitStream = 0;
				while (bufferIndex < bufferTotalSize)
				{
					if (StreamThreadAbort.exchange(false) == true)
					{
						break;
					}

					mu_int32 readSize = ov_read(&ovfile, reinterpret_cast<char*>(buffer), ReadBufferSize, 0, 2, 1, &bitStream);
					if (readSize <= 0)
					{
						break;
					}

					mu_memcpy(FileBuffer + bufferIndex, buffer, readSize);
					bufferIndex += readSize;
				}

				FileBufferAvailableSize = bufferIndex;
				ov_clear(&ovfile);
			}

			mu_fclose(file);
		}

		MU_SAFE_DELETE_ARRAY(buffer);
	}

	int StreamCallback(const void *input,
							void *output,
							unsigned long frameCount,
							const PaStreamCallbackTimeInfo *timeInfo,
							PaStreamCallbackFlags statusFlags,
							void *userData)
	{
		if (StreamCallbackAbort == true)
		{
			return PaStreamCallbackResult::paAbort;
		}

		const mu_uint32 frameSize = static_cast<mu_uint32>(FileInfo.channels * WordSize);
		const mu_uint32 bufferIndex = FileBufferProcessedSize;
		const mu_uint32 totalSize = FileBufferTotalSize;
		const mu_uint32 availableSize = FileBufferAvailableSize;
		const mu_uint32 availableFrames = (availableSize - bufferIndex) / frameSize;
		const mu_uint32 readFrames = frameCount > availableFrames ? availableFrames : frameCount;
		const mu_uint32 zeroFrames = frameCount > availableFrames ? frameCount - availableFrames : 0;
		const mu_uint32 readSize = readFrames * frameSize;

		const mu_float masterVolume = static_cast<mu_float>(MasterVolume.load()) * MasterVolumeMultiplierInverse;
		const mu_float volumeLevelDb = -6.0f; //cut amplitude in half; same as 0.5 above
		const mu_float volumeMultiplier = (masterVolume * EMath::Pow(10.0f, (volumeLevelDb / 40.f)));

		mu_int16 *audioOutput = reinterpret_cast<mu_int16*>(output);
		mu_int16 *audioInput = reinterpret_cast<mu_int16*>(FileBuffer + bufferIndex);
		const mu_uint32 valuesCount = readFrames * static_cast<mu_uint32>(FileInfo.channels);
		for (mu_uint32 n = 0; n < valuesCount; ++n)
		{
			audioOutput[n] = static_cast<mu_int16>(static_cast<mu_float>(audioInput[n]) * volumeMultiplier);
		}

		mu_uint8 *outputBuffer = reinterpret_cast<mu_uint8*>(output);
		mu_zeromem(outputBuffer + readSize, zeroFrames * frameSize);

		FileBufferProcessedSize += readSize;

		return bufferIndex + readSize >= totalSize ?
			PaStreamCallbackResult::paComplete :
			PaStreamCallbackResult::paContinue;
	}
};