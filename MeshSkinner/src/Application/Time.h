#pragma once

#include <chrono>

class Time
{
	friend class Application;

private:
	static void Init();
	static void FrameBegin();

public:
	static float GetTimeSeconds();
	static float GetFPS();
	static float GetDeltaSeconds();

private:
	static std::chrono::high_resolution_clock::time_point startTime;
	static std::chrono::high_resolution_clock::time_point lastFrameTime;
	static std::chrono::high_resolution_clock::time_point currentFrameTime;
};
