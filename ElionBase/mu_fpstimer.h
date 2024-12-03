#ifndef __MU_FPS_TIMER_H__
#define __MU_FPS_TIMER_H__

#pragma once

#include <chrono>

#if defined(__ANDROID__)
#define MINIMUM_FPS_TIME (1000.0 / 30.0) // 30 FPS (Mobile devices only)
#else
#define MINIMUM_FPS_TIME (1000.0 / 60.0) // 60 FPS
#endif

class MUFpsTimer
{
public:
	MUFpsTimer();
	~MUFpsTimer();

	void Initialize();
	void Wait();

	mu_double GetWorkTime();
	mu_double GetElapsedTime();
	mu_uint64 GetTime();
	
public:
	EINLINE mu_boolean GetLimitFPS()
	{
		return _LimitFPS;
	}

	EINLINE void SetLimitFPS(mu_boolean limitFps)
	{
		_LimitFPS = limitFps;
	}

	EINLINE void SetFPS(mu_int32 maxFps)
	{
		maxFps = EMath::Clamp(maxFps, 21, 1000);
		_FpsTime = 1000.0 / (mu_double)maxFps;
	}

private:
	std::chrono::steady_clock::time_point _BaseTime;
	std::chrono::steady_clock::time_point _LastTime;
	mu_double _FpsTime;
	mu_double _ElapsedTime;
	mu_double _WorkTime;
	mu_boolean _LimitFPS;
};

#endif