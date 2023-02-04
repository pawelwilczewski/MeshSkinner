#pragma once

#include <chrono>

class Time
{
	friend class Application;

private:
	static void Init();
	static void FrameStart();

public:
	static float GetTimeSeconds();
	static float GetFPS();
	static float GetDeltaSeconds();

private:
	static std::chrono::high_resolution_clock::time_point s_StartTime;
	static std::chrono::high_resolution_clock::time_point s_LastFrameTime;
	static std::chrono::high_resolution_clock::time_point s_CurrentFrameTime;
};
