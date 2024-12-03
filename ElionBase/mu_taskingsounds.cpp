#include "stdafx.h"
#include "mu_tasking.h"

void MUTasking::UpdateSounds(const mu_uint32 ThreadIndex)
{
	if (ThreadIndex == 0)
	{
#ifndef DISABLE_AUDIO
		MU_AUDIO->Update();
#endif
	}

	_ThreadBarrier->Sync();

	for (mu_uint32 n = ThreadIndex; n < MAX_SOUNDS; n += _ThreadsCount)
	{
		MUWavData *wav = &SOUNDS::Data[n];

		if (wav->IsFailed() == true) continue;

		mu_int32 referenceCount = wav->GetReferenceCount();
		if (wav->IsLoaded() == false)
		{
			if (referenceCount > 0)
			{
				wav->LoadWav();
			}
		}
		else
		{
			if (referenceCount <= 0 &&
				wav->GetUnloadTime() <= g_CurrentTime)
			{
				wav->Unload();
			}
		}
	}

	_ThreadBarrier->Sync();

#ifndef DISABLE_AUDIO
	if(MU_AUDIO->_Running == true)
	{
		HRESULT hr;

#ifdef DIVIDE_TASKING
		mu_uint32 beginIndex, endIndex;
		GetIndexTasking(ThreadIndex, g_SoundsCurCount, beginIndex, endIndex);

		for (mu_uint32 i = beginIndex; i < endIndex; ++i)
#else
		for (mu_uint32 i = ThreadIndex; i < g_SoundsCurCount; i += _ThreadsCount)
#endif
		{
			MUAudioSource *o = &g_Sounds[i];

			if (o->Live == false)
			{
				continue;
			}

			if (o->Started == false)
			{
				MUWavData *wav = &SOUNDS::Data[o->Index];

				o->Started = true;

				if (o->Index >= MAX_SOUNDS)
				{
					o->Delete();
				}
				else
				{

#ifdef ENABLE_REVERB
					XAUDIO2_SEND_DESCRIPTOR sendDescriptors[2];
					sendDescriptors[0].Flags = XAUDIO2_SEND_USEFILTER;
					sendDescriptors[0].pOutputVoice = MU_AUDIO->_MasteringVoice;
					sendDescriptors[1].Flags = XAUDIO2_SEND_USEFILTER;
					sendDescriptors[1].pOutputVoice = MU_AUDIO->_SubmixVoice;
					const XAUDIO2_VOICE_SENDS sendList = { 2, sendDescriptors };
#else
					XAUDIO2_SEND_DESCRIPTOR sendDescriptors[1];
					sendDescriptors[0].Flags = XAUDIO2_SEND_USEFILTER;
					sendDescriptors[0].pOutputVoice = MU_AUDIO->_MasteringVoice;
					const XAUDIO2_VOICE_SENDS sendList = { 1, sendDescriptors };
#endif

					hr = MU_AUDIO->_AudioEngine->CreateSourceVoice(&o->Source, wav->_WavFormat, 0, XAUDIO2_DEFAULT_FREQ_RATIO, MU_AUDIO, &sendList);

					if (FAILED(hr))
					{
						o->Delete();
					}
					else
					{
						XAUDIO2_BUFFER buffer;
						buffer.Flags = XAUDIO2_END_OF_STREAM;
						buffer.AudioBytes = wav->_AudioBytes;
						buffer.pAudioData = wav->_AudioBuffer;
						buffer.PlayBegin = 0;
						buffer.PlayLength = 0;
						buffer.LoopBegin = 0;
						buffer.LoopLength = 0;
						buffer.pContext = nullptr;

						if (o->Looped)
						{
							buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
						}
						else
						{
							buffer.LoopCount = 0;
						}

						hr = o->Source->SubmitSourceBuffer(&buffer);

						if (FAILED(hr))
						{
							o->Delete();
						}
						else
						{
							o->Source->SetVolume(o->Volume);

							if (o->ObjectType >= SOUND_OBJECT)
							{
								o->Apply3D();
							}

							hr = o->Source->Start();

							if (FAILED(hr))
							{
								o->Delete();
							}
						}
					}
				}
			}
			else
			{
				if (o->Stop == true || o->IsUsed() == false)
				{
					o->Stop = true;
					o->Volume -= 0.05f * (_ElapsedTime / GAME_FPS);

					if (o->Volume <= 0.0f)
					{
						o->Delete();
					}
					else
					{
						o->Source->SetVolume(o->Volume);
					}
				}
				else if (o->ObjectType >= SOUND_OBJECT &&
					MU_AUDIO->_FinishedVoices == true)
				{
					XAUDIO2_VOICE_STATE state;
					o->Source->GetState(&state);

					if (state.BuffersQueued == 0)
					{
						o->Delete();
					}
				}
				else if (o->ObjectType == SOUND_BACKGROUND)
				{
					if (o->Volume < 1.0f)
					{
						o->Volume += 0.05f * (_ElapsedTime / GAME_FPS);

						if (o->Volume > 1.0f)
						{
							o->Volume = 1.0f;
						}

						o->Source->SetVolume(o->Volume);
					}

					if (o->Looped == false)
					{
						XAUDIO2_VOICE_STATE state;
						o->Source->GetState(&state);

						if (state.BuffersQueued == 0)
						{
							o->Delete();
						}
					}
				}

				if (o->Live == true &&
					o->ObjectType >= SOUND_OBJECT)
				{
					o->Apply3D();
				}
			}
		}

		_ThreadBarrier->Sync();

		if (ThreadIndex == 0)
		{
			MU_AUDIO->_FinishedVoices = false;

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
		}

		_ThreadBarrier->Sync();
	}
#endif
}

void MUTasking::ProcessBGM()
{
	switch (GET_WORLD)
	{
	case WD_0LORENCIA:
		if (HeroTile == 4)
		{
			StopBackground(SOUND_WIND01);
			StopBackground(SOUND_RAIN01);
		}
		else
		{
			PlayBackground(SOUND_WIND01, true);
			if (RainCurrent[0] > 0)
				PlayBackground(SOUND_RAIN01, true);
			else
				StopBackground(SOUND_RAIN01);
		}
		break;
	case WD_1DUNGEON:
		PlayBackground(SOUND_DUNGEON01, true);
		break;
	case WD_2DEVIAS:
		if (HeroTile == 3 || HeroTile >= 10)
			StopBackground(SOUND_WIND01);
		else
			PlayBackground(SOUND_WIND01, true);
		break;
	case WD_3NORIA:
		PlayBackground(SOUND_WIND01, true);
		if (GetLargeRand(512) == 0)
			PlayBackground(SOUND_FOREST01);
		break;
	case WD_4LOSTTOWER:
		PlayBackground(SOUND_TOWER01, true);
		break;
	case WD_5UNKNOWN:
		break;
	case WD_7ATLANSE:
		PlayBackground(SOUND_WATER01, true);
		break;
	case WD_8TARKAN:
		PlayBackground(SOUND_DESERT01, true);
		break;
	case WD_10HEAVEN:
		PlayBackground(SOUND_HEAVEN01, true);
		break;
	case WD_58ICECITY_BOSS:
		PlayBackground(SOUND_WIND01, true);
		break;
	case WD_79UNITEDMARKETPLACE:
		{
			PlayBackground(SOUND_WIND01, true);
			PlayBackground(SOUND_RAIN01, true);
		}
		break;
	}

	if (GET_WORLD != WD_0LORENCIA && GET_WORLD != WD_2DEVIAS && GET_WORLD != WD_3NORIA
		&& GET_WORLD != WD_58ICECITY_BOSS
		&& GET_WORLD != WD_79UNITEDMARKETPLACE
		)
	{
		StopBackground(SOUND_WIND01);
	}

	if (GET_WORLD != WD_0LORENCIA && InDevilSquare() == false
		&& GET_WORLD != WD_79UNITEDMARKETPLACE
		)
	{
		StopBackground(SOUND_RAIN01);
	}

	if (GET_WORLD != WD_1DUNGEON)
	{
		StopBackground(SOUND_DUNGEON01);
	}

	if (GET_WORLD != WD_3NORIA)
	{
		StopBackground(SOUND_FOREST01);
	}

	if (GET_WORLD != WD_4LOSTTOWER)
	{
		StopBackground(SOUND_TOWER01);
	}

	if (GET_WORLD != WD_7ATLANSE)
	{
		StopBackground(SOUND_WATER01);
	}

	if (GET_WORLD != WD_8TARKAN)
	{
		StopBackground(SOUND_DESERT01);
	}

	if (GET_WORLD != WD_10HEAVEN)
	{
		StopBackground(SOUND_HEAVEN01);
	}

	if (GET_WORLD != WD_51HOME_6TH_CHAR)
	{
		StopBackground(SOUND_ELBELAND_VILLAGEPROTECTION01);
		StopBackground(SOUND_ELBELAND_WATERFALLSMALL01);
		StopBackground(SOUND_ELBELAND_WATERWAY01);
		StopBackground(SOUND_ELBELAND_ENTERDEVIAS01);
		StopBackground(SOUND_ELBELAND_WATERSMALL01);
		StopBackground(SOUND_ELBELAND_RAVINE01);
		StopBackground(SOUND_ELBELAND_ENTERATLANCE01);
	}

	if (IsEmpireGuardian1() ||
		IsEmpireGuardian2() ||
		IsEmpireGuardian3())
	{
		switch (g_EmpireGuardian1.GetWeather())
		{
		case GMEmpireGuardian1::WEATHER_TYPE::WEATHER_SUN:
			break;

		case GMEmpireGuardian1::WEATHER_TYPE::WEATHER_RAIN:
			PlayBackground(SOUND_EMPIREGUARDIAN_WEATHER_RAIN);
			break;

		case GMEmpireGuardian1::WEATHER_TYPE::WEATHER_FOG:
			PlayBackground(SOUND_EMPIREGUARDIAN_WEATHER_FOG);
			break;

		case GMEmpireGuardian1::WEATHER_TYPE::WEATHER_STORM:
			PlayBackground(SOUND_EMPIREGUARDIAN_WEATHER_STORM);
			break;
		}
	}
	else
	{
		switch (g_EmpireGuardian1.GetWeather())
		{
		case GMEmpireGuardian1::WEATHER_TYPE::WEATHER_SUN:
			break;

		case GMEmpireGuardian1::WEATHER_TYPE::WEATHER_RAIN:
			StopBackground(SOUND_EMPIREGUARDIAN_WEATHER_RAIN);
			break;

		case GMEmpireGuardian1::WEATHER_TYPE::WEATHER_FOG:
			StopBackground(SOUND_EMPIREGUARDIAN_WEATHER_FOG);
			break;

		case GMEmpireGuardian1::WEATHER_TYPE::WEATHER_STORM:
			StopBackground(SOUND_EMPIREGUARDIAN_WEATHER_STORM);
			break;
		}
	}
	
	if (IsEmpireGuardian4())
	{
		PlayBackground(SOUND_EMPIREGUARDIAN_INDOOR_SOUND);
	}
	else
	{
		StopBackground(SOUND_EMPIREGUARDIAN_INDOOR_SOUND);
	}

	if (GET_WORLD == WD_0LORENCIA)
	{
		if (Hero->SafeZone)
		{
			if (HeroTile == 4)
				EMusicBackend::Play(MUSICS::Data[MUSIC_PUB]);
			else
				EMusicBackend::Play(MUSICS::Data[MUSIC_MAIN_THEME]);
		}
	}
	else
	{
		EMusicBackend::Stop(MUSICS::Data[MUSIC_PUB]);
		EMusicBackend::Stop(MUSICS::Data[MUSIC_MAIN_THEME]);
	}
	if (GET_WORLD == WD_2DEVIAS)
	{
		if (Hero->SafeZone)
		{
			if ((Hero->PositionX) >= 205 && (Hero->PositionX) <= 214 &&
				(Hero->PositionY) >= 13 && (Hero->PositionY) <= 31)
			{
				EMusicBackend::Play(MUSICS::Data[MUSIC_CHURCH]);
			}
			else
			{
				EMusicBackend::Play(MUSICS::Data[MUSIC_DEVIAS]);
			}
		}
	}
	else
	{
		EMusicBackend::Stop(MUSICS::Data[MUSIC_CHURCH]);
		EMusicBackend::Stop(MUSICS::Data[MUSIC_DEVIAS]);
	}
	if (GET_WORLD == WD_3NORIA)
	{
		if (Hero->SafeZone)
			EMusicBackend::Play(MUSICS::Data[MUSIC_NORIA]);
	}
	else
	{
		EMusicBackend::Stop(MUSICS::Data[MUSIC_NORIA]);
	}
	if (GET_WORLD == WD_1DUNGEON || GET_WORLD == WD_5UNKNOWN)
	{
		EMusicBackend::Play(MUSICS::Data[MUSIC_DUNGEON]);
	}
	else
	{
		EMusicBackend::Stop(MUSICS::Data[MUSIC_DUNGEON]);
	}

	if (GET_WORLD == WD_7ATLANSE) {
		EMusicBackend::Play(MUSICS::Data[MUSIC_ATLANS]);
	}
	else
	{
		EMusicBackend::Stop(MUSICS::Data[MUSIC_ATLANS]);
	}
	if (GET_WORLD == WD_10HEAVEN) {
		EMusicBackend::Play(MUSICS::Data[MUSIC_ICARUS]);
	}
	else
	{
		EMusicBackend::Stop(MUSICS::Data[MUSIC_ICARUS]);
	}
	if (GET_WORLD == WD_8TARKAN) {
		EMusicBackend::Play(MUSICS::Data[MUSIC_TARKAN]);
	}
	else
	{
		EMusicBackend::Stop(MUSICS::Data[MUSIC_TARKAN]);
	}
	if (GET_WORLD == WD_4LOSTTOWER) {
		EMusicBackend::Play(MUSICS::Data[MUSIC_LOSTTOWER_A]);
	}
	else
	{
		EMusicBackend::Stop(MUSICS::Data[MUSIC_LOSTTOWER_A]);
	}

	if (InHellas(GET_WORLD)) {
		EMusicBackend::Play(MUSICS::Data[MUSIC_KALIMA]);
	}
	else
	{
		EMusicBackend::Stop(MUSICS::Data[MUSIC_KALIMA]);
	}

	if (GET_WORLD == WD_31HUNTING_GROUND)
	{
		EMusicBackend::Play(MUSICS::Data[MUSIC_BC_HUNTINGGROUND]);
	}
	else
	{
		EMusicBackend::Stop(MUSICS::Data[MUSIC_BC_HUNTINGGROUND]);
	}

	M33Aida::PlayBGM();
	M34CryWolf1st::PlayBGM();
	M37Kanturu1st::PlayBGM();
	M38Kanturu2nd::PlayBGM();
	M39Kanturu3rd::PlayBGM();
	g_3rdChangeUp.PlayBGM();

	if (g_CursedTemple.IsCursedTemple())
	{
		g_CursedTemple.PlayBGM();
	}

	if (GET_WORLD == WD_51HOME_6TH_CHAR)
	{
		EMusicBackend::Play(MUSICS::Data[MUSIC_ELBELAND]);
	}
	else
	{
		EMusicBackend::Stop(MUSICS::Data[MUSIC_ELBELAND]);
	}

	if (GET_WORLD == WD_56MAP_SWAMP_OF_QUIET)
	{
		EMusicBackend::Play(MUSICS::Data[MUSIC_SWAMP_OF_QUIET]);
	}
	else
	{
		EMusicBackend::Stop(MUSICS::Data[MUSIC_SWAMP_OF_QUIET]);
	}

	g_Raklion.PlayBGM();
	g_SantaTown.PlayBGM();
	g_PKField.PlayBGM();
	g_DoppelGanger1.PlayBGM();
	g_EmpireGuardian1.PlayBGM();
}