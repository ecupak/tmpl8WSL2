#pragma once
#include <chrono>

// timer

struct Timer
{
	Timer();

	float elapsed() const;

	void reset();

	std::chrono::high_resolution_clock::time_point start;
};
