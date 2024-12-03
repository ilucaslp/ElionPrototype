#ifndef __MU_TIMER_H__
#define __MU_TIMER_H__

#pragma once

class MUTimer
{
public:
	MUTimer();
	~MUTimer();

	void ResetTimer();
	mu_double GetAbsTime();
	mu_double GetTimeElapsed();

private:
	mu_double _Resolution;
	mu_uint64 _Frequency;
	mu_uint64 _TimerStart;
	mu_uint64 _AbsTimerStart;
};

class MUTimer2
{
public:
	MUTimer2();
	~MUTimer2();

	void SetTimer(mu_uint64 Delay);
	mu_uint64 GetDelay();
	void ResetTimer();
	void UpdateTime();

	mu_boolean IsTime();

private:
	mu_double _Resolution;
	mu_uint64 _StartTickCount;
	mu_uint64 _Delay;
	mu_boolean _Time;
};

#endif