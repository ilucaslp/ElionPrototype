#include "stdafx.h"
#include "mu_fpstimer.h"

#include <array>
#include <bitset>

MUFpsTimer::MUFpsTimer() : _LastTime(std::chrono::steady_clock::now()), _ElapsedTime(0)
{
}

MUFpsTimer::~MUFpsTimer()
{
}

void MUFpsTimer::Initialize()
{
	_FpsTime = MINIMUM_FPS_TIME;
	_BaseTime = std::chrono::steady_clock::now();
	_LastTime = _BaseTime;
	_ElapsedTime = 0.0;
	_LimitFPS = true;
}

void MUFpsTimer::Wait()
{
	std::chrono::steady_clock::time_point lastTime = _LastTime;
	_LastTime = std::chrono::steady_clock::now();

	std::chrono::duration<mu_double, std::milli> work_time = _LastTime - lastTime;
	_WorkTime = work_time.count();

	const mu_boolean active = EVideoBackend::GetVideoStatus(EVideoStatusFlag::VSF_Active | EVideoStatusFlag::VSF_Visible | EVideoStatusFlag::VSF_Focused | EVideoStatusFlag::VSF_Available);
	if((_LimitFPS == true &&
		MU_RESOURCE->IsVerticalSync() == false) ||
	   active == false)
	{
		static const mu_double BackgroundFpsTime = 1000.0 / 30.0;
		mu_double fpsTime;
		
		if (active == false)
		{
			fpsTime = BackgroundFpsTime;
		}
		else
		{
			fpsTime = _FpsTime;
		}

		if (work_time.count() < fpsTime)
		{
			std::chrono::duration<mu_double, std::milli> delta_ms(fpsTime - work_time.count());
			auto delta_ms_duration = std::chrono::duration_cast<std::chrono::milliseconds>(delta_ms);
			std::this_thread::sleep_for(delta_ms_duration);
			_LastTime = std::chrono::steady_clock::now();
		}
	}

	_ElapsedTime = std::chrono::duration<mu_double, std::milli>(_LastTime - lastTime).count();
}

mu_double MUFpsTimer::GetWorkTime()
{
	return _WorkTime;
}

mu_double MUFpsTimer::GetElapsedTime()
{
	return _ElapsedTime;
}

mu_uint64 MUFpsTimer::GetTime()
{
	return std::chrono::duration<mu_double, std::milli>(_LastTime - _BaseTime).count();
}