#include "stdafx.h"
#include "mu_timer.h"

MUTimer::MUTimer()
{
	SDL_GetPerformanceFrequency();
	_AbsTimerStart = _TimerStart = SDL_GetPerformanceCounter();
	_Resolution = (1.0 / (mu_double)_Frequency) * 1000.0;
}

MUTimer::~MUTimer()
{
}

void MUTimer::ResetTimer()
{
	_TimerStart = SDL_GetPerformanceCounter();
}

mu_double MUTimer::GetTimeElapsed()
{
	return (mu_double)(SDL_GetPerformanceCounter() - _TimerStart) * _Resolution;
}

mu_double MUTimer::GetAbsTime()
{
	return (mu_double)(SDL_GetPerformanceCounter() - _AbsTimerStart) * _Resolution;
}

////////////////////////////////////////////////////////////////////////////

MUTimer2::MUTimer2() : _StartTickCount(0), _Delay(0), _Time(false)
{
	_Resolution = (1.0 / (mu_double)SDL_GetPerformanceFrequency()) * 1000.0;
}

MUTimer2::~MUTimer2()
{
}

void MUTimer2::SetTimer(mu_uint64 Delay)
{
	_Delay = Delay;
	_StartTickCount = 0;
}

mu_uint64 MUTimer2::GetDelay()
{
	return _Delay;
}

void MUTimer2::ResetTimer()
{
	_StartTickCount = 0;
}

void MUTimer2::UpdateTime()
{
	if (_Delay == 0)
	{
		_Time = true;
	}
	else
	{
		_Time = false;

		mu_uint64 counter = SDL_GetPerformanceCounter() / _Resolution;
		if (_StartTickCount == 0)
		{
			_StartTickCount = counter;
		}
		else if (counter - _StartTickCount > _Delay)
		{
			_StartTickCount = counter;
			_Time = true;
		}
	}
}

mu_boolean MUTimer2::IsTime() 
{
	return _Time;
}