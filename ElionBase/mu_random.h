#ifndef __MU_RANDOM_H__
#define __MU_RANDOM_H__

#pragma once

#include <random>

template<typename T>
T GetRandom(T min, T max)
{
	static thread_local std::mt19937 rengine(std::random_device{}());
	std::uniform_int_distribution<T> uniform_dist(min, max);

	return uniform_dist(rengine);
}

template<typename T>
EINLINE T GetLargeRand(T max, T add = 0)
{
	return add + GetRandom((T)0, max > 0 ? max - 1 : max);
}

#endif