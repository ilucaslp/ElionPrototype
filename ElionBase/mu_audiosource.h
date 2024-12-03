#ifndef __MU_AUDIO_SOURCE_H__
#define __MU_AUDIO_SOURCE_H__

#pragma once

#define MAX_SOUND_BUFFER 256

class OBJECT;
class PARTICLE;
class JOINT;

enum
{
	SOUND_BACKGROUND,
	SOUND_OBJECT,
	SOUND_PARTICLE,
	SOUND_JOINT,
	SOUND_POSITION,
};

#ifndef DISABLE_AUDIO
class MUAudioSource
{
public:
	MUAudioSource();
	~MUAudioSource();

	void Delete();
	void Apply3D();

	mu_boolean IsUsed();

public:
	mu_boolean IsFree;
	mu_boolean Live;
	mu_boolean Looped;
	mu_boolean Started;
	mu_boolean Stop;

	mu_uint8 ObjectType;

	mu_uint32 Index;

	mu_float Volume;

	EVector3 Position;

	void *Object;

	IXAudio2SourceVoice *Source;
};

extern MUCriticalSectionSpinLock g_SoundsLock;
extern MUAudioSource g_Sounds[MAX_SOUND_BUFFER];
extern mu_uint16 g_SoundsFree[MAX_SOUND_BUFFER];
extern mu_uint16 g_SoundsCurCount;
extern mu_uint16 g_SoundsFreeCount;
extern mu_atomic_uint32_t g_SoundsDeleteCount;
#endif

void PlayBackground(mu_uint32 Index, mu_boolean Looped = false);
void PlayObject(mu_uint32 Index, OBJECT *Object, mu_boolean Looped = false);
void PlayParticle(mu_uint32 Index, PARTICLE *Object, mu_boolean Looped = false);
void PlayJoint(mu_uint32 Index, JOINT *Object, mu_boolean Looped = false);
void PlayPosition(mu_uint32 Index, EVector3 &Position);
void StopBackground(mu_uint32 Index);

#endif