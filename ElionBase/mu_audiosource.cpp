#include "stdafx.h"
#include "mu_audiosource.h"

#ifndef DISABLE_AUDIO
// Supported speaker positions, represented as azimuth angles.
//
// Here's a picture of the azimuth angles for the 8 cardinal points,
// seen from above.  The emitter's base position is at the origin 0.
//
//           FRONT
//             | 0  <-- azimuth
//             |
//    7pi/4 \  |  / pi/4
//           \ | /
// LEFT       \|/      RIGHT
// 3pi/2-------0-------pi/2
//            /|\
    //           / | \
    //    5pi/4 /  |  \ 3pi/4
//             |
//             | pi
//           BACK
//
#define LEFT_AZIMUTH                    (3*X3DAUDIO_PI/2)
#define RIGHT_AZIMUTH                   (X3DAUDIO_PI/2)
#define FRONT_LEFT_AZIMUTH              (7*X3DAUDIO_PI/4)
#define FRONT_RIGHT_AZIMUTH             (X3DAUDIO_PI/4)
#define FRONT_CENTER_AZIMUTH            0.0f
#define LOW_FREQUENCY_AZIMUTH           X3DAUDIO_2PI
#define BACK_LEFT_AZIMUTH               (5*X3DAUDIO_PI/4)
#define BACK_RIGHT_AZIMUTH              (3*X3DAUDIO_PI/4)
#define BACK_CENTER_AZIMUTH             X3DAUDIO_PI
#define FRONT_LEFT_OF_CENTER_AZIMUTH    (15*X3DAUDIO_PI/8)
#define FRONT_RIGHT_OF_CENTER_AZIMUTH   (X3DAUDIO_PI/8)


//--------------<D-A-T-A---T-Y-P-E-S>---------------------------------------//
// Supported emitter channel layouts:
static const mu_float aStereoLayout[] =
{
	LEFT_AZIMUTH,
	RIGHT_AZIMUTH
};
static const mu_float a2Point1Layout[] =
{
	LEFT_AZIMUTH,
	RIGHT_AZIMUTH,
	LOW_FREQUENCY_AZIMUTH
};
static const mu_float aQuadLayout[] =
{
	FRONT_LEFT_AZIMUTH,
	FRONT_RIGHT_AZIMUTH,
	BACK_LEFT_AZIMUTH,
	BACK_RIGHT_AZIMUTH
};
static const mu_float a4Point1Layout[] =
{
	FRONT_LEFT_AZIMUTH,
	FRONT_RIGHT_AZIMUTH,
	LOW_FREQUENCY_AZIMUTH,
	BACK_LEFT_AZIMUTH,
	BACK_RIGHT_AZIMUTH
};
static const mu_float a5Point1Layout[] =
{
	FRONT_LEFT_AZIMUTH,
	FRONT_RIGHT_AZIMUTH,
	FRONT_CENTER_AZIMUTH,
	LOW_FREQUENCY_AZIMUTH,
	BACK_LEFT_AZIMUTH,
	BACK_RIGHT_AZIMUTH
};
static const mu_float a7Point1Layout[] =
{
	FRONT_LEFT_AZIMUTH,
	FRONT_RIGHT_AZIMUTH,
	FRONT_CENTER_AZIMUTH,
	LOW_FREQUENCY_AZIMUTH,
	BACK_LEFT_AZIMUTH,
	BACK_RIGHT_AZIMUTH,
	LEFT_AZIMUTH,
	RIGHT_AZIMUTH
};

MUCriticalSectionSpinLock g_SoundsLock;
MUAudioSource g_Sounds[MAX_SOUND_BUFFER];
mu_uint16 g_SoundsFree[MAX_SOUND_BUFFER];
mu_uint16 g_SoundsCurCount = 0;
mu_uint16 g_SoundsFreeCount = 0;
mu_atomic_uint32_t g_SoundsDeleteCount(0);

MUAudioSource::MUAudioSource()
{
	Object = nullptr;
	Source = nullptr;
}

MUAudioSource::~MUAudioSource()
{
}

void MUAudioSource::Delete()
{
	++g_SoundsDeleteCount;

	Live = false;

	switch (ObjectType)
	{
	case SOUND_OBJECT:
		{
			OBJECT *o = (OBJECT*)Object;

			if (this->IsUsed() == true)
			{
				o->PlayingSound = -1;
			}
		}
		break;

	case SOUND_PARTICLE:
		{
			PARTICLE *o = (PARTICLE*)Object;
		}
		break;

	case SOUND_JOINT:
		{
			JOINT *o = (JOINT*)Object;
		}
		break;
	}

	MUWavData *wav = &SOUNDS::Data[Index];
	wav->DecreaseReference();

	if (Object == nullptr)
	{
		wav->_VoiceLocked = false;
		wav->_VoiceIndex = (mu_uint32)-1;
	}

	MU_SAFE_DESTROY_VOICE(Source);
}

void MUAudioSource::Apply3D()
{
	MUWavData *wav = &SOUNDS::Data[Index];

	mu_float MatrixCoefficients[XAUDIO2_MAX_AUDIO_CHANNELS * 8];

	X3DAUDIO_DSP_SETTINGS dspSetting = { 0 };
	dspSetting.SrcChannelCount = wav->_WavFormat->nChannels;
	dspSetting.DstChannelCount = MU_AUDIO->_MasterChannels;
	dspSetting.pMatrixCoefficients = MatrixCoefficients;

#ifdef ENABLE_REVERB
	mu_uint32 calcFlags = X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER | X3DAUDIO_CALCULATE_LPF_DIRECT | X3DAUDIO_CALCULATE_LPF_REVERB | X3DAUDIO_CALCULATE_REVERB;
#else
	mu_uint32 calcFlags = X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER | X3DAUDIO_CALCULATE_LPF_DIRECT;
#endif

	X3DAUDIO_EMITTER emitter;
	mu_memcpy(&emitter, &MU_AUDIO->_Emitter, sizeof(emitter));

	switch (ObjectType)
	{
	case SOUND_OBJECT:
		{
			OBJECT *o = (OBJECT*)Object;

			emitter.Position.x = o->Position.x * g_TerrainScaleRev;
			emitter.Position.y = -o->Position.y * g_TerrainScaleRev;
			emitter.Position.z = o->Position.z * g_TerrainScaleRev;
		}
		break;

	case SOUND_PARTICLE:
		{
			PARTICLE *o = (PARTICLE*)Object;

			emitter.Position.x = o->Position.x * g_TerrainScaleRev;
			emitter.Position.y = -o->Position.y * g_TerrainScaleRev;
			emitter.Position.z = o->Position.z * g_TerrainScaleRev;
		}
		break;

	case SOUND_JOINT:
		{
			JOINT *o = (JOINT*)Object;

			emitter.Position.x = o->Position.x * g_TerrainScaleRev;
			emitter.Position.y = -o->Position.y * g_TerrainScaleRev;
			emitter.Position.z = o->Position.z * g_TerrainScaleRev;
		}
		break;

	case SOUND_POSITION:
		{
			emitter.Position.x = Position.x * g_TerrainScaleRev;
			emitter.Position.y = -Position.y * g_TerrainScaleRev;
			emitter.Position.z = Position.z * g_TerrainScaleRev;
		}
		break;
	}

	mu_float emitterAzimuths[XAUDIO2_MAX_AUDIO_CHANNELS];

	emitter.ChannelCount = wav->_WavFormat->nChannels;

	if (emitter.ChannelCount > 1)
	{
		switch (emitter.ChannelCount)
		{
		case 2: emitter.pChannelAzimuths = (mu_float*)&aStereoLayout[0]; break;
		case 3: emitter.pChannelAzimuths = (mu_float*)&a2Point1Layout[0]; break;
		case 4: emitter.pChannelAzimuths = (mu_float*)&aQuadLayout[0]; break;
		case 5: emitter.pChannelAzimuths = (mu_float*)&a4Point1Layout[0]; break;
		case 6: emitter.pChannelAzimuths = (mu_float*)&a5Point1Layout[0]; break;
		case 8: emitter.pChannelAzimuths = (mu_float*)&a7Point1Layout[0]; break;
		}
	}

	static const X3DAUDIO_DISTANCE_CURVE_POINT Emitter_LinearCurvePoints[2] = { 0.0f, 1.0f, 1.0f, 0.0f };
	static const X3DAUDIO_DISTANCE_CURVE       Emitter_LinearCurve = { (X3DAUDIO_DISTANCE_CURVE_POINT*)&X3DAudioDefault_LinearCurvePoints[0], 2 };
	static const X3DAUDIO_DISTANCE_CURVE_POINT Emitter_LFE_CurvePoints[3] = { 0.0f, 1.0f, 0.25f, 0.0f, 1.0f, 0.0f };
	static const X3DAUDIO_DISTANCE_CURVE       Emitter_LFE_Curve = { (X3DAUDIO_DISTANCE_CURVE_POINT*)&Emitter_LFE_CurvePoints[0], 3 };
	static const X3DAUDIO_DISTANCE_CURVE_POINT Emitter_Reverb_CurvePoints[3] = { 0.0f, 0.5f, 0.75f, 1.0f, 1.0f, 0.0f };
	static const X3DAUDIO_DISTANCE_CURVE       Emitter_Reverb_Curve = { (X3DAUDIO_DISTANCE_CURVE_POINT*)&Emitter_Reverb_CurvePoints[0], 3 };

#if 1
	emitter.pVolumeCurve = (X3DAUDIO_DISTANCE_CURVE*)&Emitter_LinearCurve;
	emitter.pLFECurve = (X3DAUDIO_DISTANCE_CURVE*)&Emitter_LFE_Curve;
#ifdef ENABLE_REVERB
	//emitter.pReverbCurve = (X3DAUDIO_DISTANCE_CURVE*)&Emitter_Reverb_Curve;
#endif
#endif

	emitter.CurveDistanceScaler = g_TerrainScale;

	X3DAudioCalculate(MU_AUDIO->_X3DAudio, &MU_AUDIO->_Listener, &emitter, calcFlags, &dspSetting);

	dspSetting.pMatrixCoefficients = nullptr;

	if (FAILED(Source->SetFrequencyRatio(dspSetting.DopplerFactor)))
	{
		return;
	}

	if (FAILED(Source->SetOutputMatrix(MU_AUDIO->_MasteringVoice, dspSetting.SrcChannelCount, dspSetting.DstChannelCount, MatrixCoefficients)))
	{
		return;
	}
#ifdef ENABLE_REVERB
	if (FAILED(Source->SetOutputMatrix(MU_AUDIO->_SubmixVoice, dspSetting.SrcChannelCount, 2, &dspSetting.ReverbLevel)))
	{
		return;
	}
#endif

#if 1
	XAUDIO2_FILTER_PARAMETERS filterDirect = { LowPassFilter, 2.0f * EMath::Sin(X3DAUDIO_PI / 6.0f * dspSetting.LPFDirectCoefficient), 1.0f };
	// see XAudio2CutoffFrequencyToRadians() in XAudio2.h for more information on the formula used here
	if (FAILED(Source->SetOutputFilterParameters(MU_AUDIO->_MasteringVoice, &filterDirect)))
	{
		return;
	}

#ifdef ENABLE_REVERB
	XAUDIO2_FILTER_PARAMETERS filterReverb = { LowPassFilter, 2.0f * EMath::Sin(X3DAUDIO_PI / 6.0f * dspSetting.LPFReverbCoefficient), 1.0f };
	// see XAudio2CutoffFrequencyToRadians() in XAudio2.h for more information on the formula used here
	if (FAILED(Source->SetOutputFilterParameters(MU_AUDIO->_SubmixVoice, &filterReverb)))
	{
		return;
	}
#endif
#endif
}

mu_boolean MUAudioSource::IsUsed()
{
	switch (ObjectType)
	{
	case SOUND_OBJECT:
		{
			OBJECT *o = (OBJECT*)Object;

			return o->PlayingSound == this->Index;
		}
		break;

	case SOUND_PARTICLE:
		{
			PARTICLE *o = (PARTICLE*)Object;
		}
		break;

	case SOUND_JOINT:
		{
			JOINT *o = (JOINT*)Object;
		}
		break;
	}

	return true;
}
#endif

void PlayBackground(mu_uint32 Index, mu_boolean Looped)
{
#ifndef DISABLE_AUDIO
	MUWavData *wav = &SOUNDS::Data[Index];

	if (wav->_Failed == true || wav->_VoiceLocked == true)
	{
		return;
	}

	mu_uint32 i = 0;

	g_SoundsLock.Lock();

	if (g_SoundsFreeCount > 0)
	{
		if (wav->_VoiceLocked.exchange(true) != false)
		{
			g_SoundsLock.Unlock();
			return;
		}

		i = g_SoundsFree[--g_SoundsFreeCount];

		mu_assert(g_Sounds[i].Live == false);
	}
	else if (g_SoundsCurCount < MAX_SOUND_BUFFER)
	{
		if (wav->_VoiceLocked.exchange(true) != false)
		{
			g_SoundsLock.Unlock();
			return;
		}

		i = g_SoundsCurCount++;

		mu_assert(g_Sounds[i].Live == false);
	}
	else
	{
		g_SoundsLock.Unlock();
		return;
	}

	g_SoundsLock.Unlock();

	MUAudioSource *o = &g_Sounds[i];

	mu_assert(o->Live == false);

	o->IsFree = false;
	o->Live = true;
	o->Looped = Looped;
	o->Started = false;
	o->Stop = false;
	o->Index = Index;
	o->ObjectType = SOUND_BACKGROUND;
	o->Object = nullptr;
	o->Volume = Looped == true ? 0.0f : 1.0f;

	wav->IncreaseReference();
	wav->_VoiceIndex = i;
#endif
}

void PlayObject(mu_uint32 Index, OBJECT *Object, mu_boolean Looped)
{
#ifndef DISABLE_AUDIO
	MUWavData *wav = &SOUNDS::Data[Index];

	if (wav->_Failed == true || Object->PlayingSound == Index)
	{
		return;
	}

	mu_uint32 i = 0;

	g_SoundsLock.Lock();

	if (g_SoundsFreeCount > 0)
	{
		i = g_SoundsFree[--g_SoundsFreeCount];

		mu_assert(g_Sounds[i].Live == false);
	}
	else if (g_SoundsCurCount < MAX_SOUND_BUFFER)
	{
		i = g_SoundsCurCount++;

		mu_assert(g_Sounds[i].Live == false);
	}
	else
	{
		g_SoundsLock.Unlock();
		return;
	}

	g_SoundsLock.Unlock();

	MUAudioSource *o = &g_Sounds[i];

	o->IsFree = false;
	o->Live = true;
	o->Looped = Looped;
	o->Started = false;
	o->Stop = false;
	o->Index = Index;
	o->ObjectType = SOUND_OBJECT;
	o->Object = Object;
	o->Volume = 1.0f;

	wav->IncreaseReference();
	Object->PlayingSound = Index;
#endif
}

void PlayParticle(mu_uint32 Index, PARTICLE *Object, mu_boolean Looped)
{
#ifndef DISABLE_AUDIO
	MUWavData *wav = &SOUNDS::Data[Index];

	if (wav->_Failed == true)
	{
		return;
	}

	mu_uint32 i = 0;

	g_SoundsLock.Lock();

	if (g_SoundsFreeCount > 0)
	{
		i = g_SoundsFree[--g_SoundsFreeCount];

		mu_assert(g_Sounds[i].Live == false);
	}
	else if (g_SoundsCurCount < MAX_SOUND_BUFFER)
	{
		i = g_SoundsCurCount++;

		mu_assert(g_Sounds[i].Live == false);
	}
	else
	{
		g_SoundsLock.Unlock();
		return;
	}

	g_SoundsLock.Unlock();

	MUAudioSource *o = &g_Sounds[i];

	o->IsFree = false;
	o->Live = true;
	o->Looped = Looped;
	o->Started = false;
	o->Stop = false;
	o->Index = Index;
	o->ObjectType = SOUND_PARTICLE;
	o->Object = Object;
	o->Volume = 1.0f;

	wav->IncreaseReference();
#endif
}

void PlayJoint(mu_uint32 Index, JOINT *Object, mu_boolean Looped)
{
#ifndef DISABLE_AUDIO
	MUWavData *wav = &SOUNDS::Data[Index];

	if (wav->_Failed == true)
	{
		return;
	}

	mu_uint32 i = 0;

	g_SoundsLock.Lock();

	if (g_SoundsFreeCount > 0)
	{
		i = g_SoundsFree[--g_SoundsFreeCount];

		mu_assert(g_Sounds[i].Live == false);
	}
	else if (g_SoundsCurCount < MAX_SOUND_BUFFER)
	{
		i = g_SoundsCurCount++;

		mu_assert(g_Sounds[i].Live == false);
	}
	else
	{
		g_SoundsLock.Unlock();
		return;
	}

	g_SoundsLock.Unlock();

	MUAudioSource *o = &g_Sounds[i];

	o->IsFree = false;
	o->Live = true;
	o->Looped = Looped;
	o->Started = false;
	o->Stop = false;
	o->Index = Index;
	o->ObjectType = SOUND_JOINT;
	o->Object = Object;
	o->Volume = 1.0f;

	wav->IncreaseReference();
#endif
}

void PlayPosition(mu_uint32 Index, EVector3 &Position)
{
#ifndef DISABLE_AUDIO
	MUWavData *wav = &SOUNDS::Data[Index];

	if (wav->_Failed == true)
	{
		return;
	}

	mu_uint32 i = 0;

	g_SoundsLock.Lock();

	if (g_SoundsFreeCount > 0)
	{
		i = g_SoundsFree[--g_SoundsFreeCount];

		mu_assert(g_Sounds[i].Live == false);
	}
	else if (g_SoundsCurCount < MAX_SOUND_BUFFER)
	{
		i = g_SoundsCurCount++;

		mu_assert(g_Sounds[i].Live == false);
	}
	else
	{
		g_SoundsLock.Unlock();
		return;
	}

	g_SoundsLock.Unlock();

	MUAudioSource *o = &g_Sounds[i];

	o->IsFree = false;
	o->Live = true;
	o->Looped = false;
	o->Started = false;
	o->Stop = false;
	o->Index = Index;
	o->ObjectType = SOUND_POSITION;
	o->Object = nullptr;
	o->Position = Position;
	o->Volume = 1.0f;

	wav->IncreaseReference();
#endif
}

void StopBackground(mu_uint32 Index)
{
#ifndef DISABLE_AUDIO
	MUWavData *wav = &SOUNDS::Data[Index];

	if (wav->_VoiceIndex != (mu_uint32)-1)
	{
		MUAudioSource *o = &g_Sounds[wav->_VoiceIndex];

		o->Stop = true;
	}
#endif
}