#ifndef __MU_RESOURCE_MANAGER_H__
#define __MU_RESOURCE_MANAGER_H__

#pragma once

#define DEFAULT_WIDTH 800
#define DEFAULT_HEIGHT 600

class MUResourceManager
{
public:
	MUResourceManager();
	~MUResourceManager();

	mu_boolean LoadSettings();
	void SaveSettings();
	void CalculateAspectRatios();

	void UpdateWorldTime(mu_uint64 CurrentTime);

	mu_float GetAspectRatio()
	{
		return (mu_float)GetWindowWidth() / (mu_float)GetWindowHeight();
	}

	void SetWindowWidth(mu_int32 Width);
	void SetWindowHeight(mu_int32 Height);
	mu_int32 GetWindowWidth();
	mu_int32 GetWindowHeight();
	mu_float GetWindowHalfWidth();
	mu_float GetWindowHalfHeight();

	void SetWindowMode(mu_boolean isWindowMode);
	mu_boolean IsWindowMode();

	void SetAntialiasing(mu_boolean isAntialiasing);
	mu_boolean IsAntialiasing();

	void SetVerticalSync(mu_boolean isVerticalSync);
	mu_boolean IsVerticalSync();

	void SetShadowType(const EShadowType shadowType);
	const EShadowType GetShadowType();

	void SetMusicVolume(mu_float musicVolume);
	mu_float GetMusicVolume();
	void SetSoundVolume(mu_float soundVolume);
	mu_float GetSoundVolume();

	void DestroyTextures();
	mu_uint32 LoadTexture2D(mu_string path);
	void UnloadTexture2D(mu_uint32 textureIndex);

	MUTexture *CreateTexture2D(mu_string path, const EImageViewType imageViewType = EImageViewType::eNone);
	void DeleteTexture(mu_uint32 index);

public:
	EINLINE mu_boolean IsAutoAttack()
	{
		return _IsAutoAttack;
	}

	EINLINE void SetAutoAttack(mu_boolean isAutoAttack)
	{
		_IsAutoAttack = isAutoAttack;
	}

public:
	EINLINE mu_uint32 GetMaxThreadsCount()
	{
		return _Threads;
	}

public:
	mu_boolean _IsChangeResolutionAvailable;

private:
	enum
	{
		TEXTURE_GROW_SIZE = 50,
	};

private:
	mu_int32 _WindowWidth, _WindowHeight;

public:
	mu_float _640x480Rate[2];
	mu_float _1024x768Rate[2];
	mu_float _1024x1024Rate[2];
	mu_float _1280x720Rate[2];

private:
	mu_boolean _IsAutoAttack;

	mu_boolean _IsWindowMode;
	mu_boolean _IsAntialiasing;
	mu_boolean _IsVerticalSync;
	EShadowType _ShadowType;

	mu_int32 _Threads;

	mu_float _MusicVolume;
	mu_float _SoundVolume;

	mu_string _LastLoginID;

	struct
	{
		std::vector<MUTexture*> _Data;
		std::stack<mu_uint32> _IndexFree;
		mu_atomic_uint32_t _IndexCounter;
		MUCriticalSectionSpinLock _IndexLock;
	} _Textures;

	MUCriticalSectionSpinLock _RandomTexturesLock;
};

#define MU_TO_VIEWPORT(value, rate) ((mu_float)(value)/(mu_float)(rate))
#define MU_TO_WINDOW(value, rate) ((mu_float)(value)*(mu_float)(rate))
#define MU_VIEWPORT_RATE(window, viewport) ((mu_float)(window) / (mu_float)(viewport)) // Return Aspect Ratio value
#define MU_RVIEWPORT_RATE(window, aspectratio) ((mu_float)(window) / (mu_float)(aspectratio)) // Return Viewport Value from Window Size and Aspect Ratio

extern mu_float FWorldTime;
extern mu_int64 IWorldTime;
extern mu_int32 ChatTime;
extern mu_int32 MacroTime;
extern mu_uint32 WaterTextureNumber;
extern mu_uint32 MoveSceneFrame;

extern mu_uint32 EventMask;
extern mu_int32 MaxBoids;
extern mu_int32 MaxFishs;

class MUAtomicFloat // Atomic Write only, be careful don't read from different threads
{
public:
	MUAtomicFloat() : _Value(0)
	{
	}

	void Store(const mu_float value)
	{
		_Value.store(*(const mu_uint32*)(&value), std::memory_order_seq_cst);
	}

	void StoreDirect(const mu_float value)
	{
		_Value.store(*(const mu_uint32*)(&value), std::memory_order_relaxed);
	}

	mu_float Load()
	{
		const mu_uint32 val = _Value.load(std::memory_order_relaxed);
		return *reinterpret_cast<const mu_float*>(&val);
	}

public:
	mu_atomic_uint32_t _Value;
};

extern MUAtomicFloat EarthQuake;

EINLINE mu_boolean IsEventMark(mu_uint32 eventMask)
{
	return (EventMask & (1 << eventMask)) != 0;
}

#endif