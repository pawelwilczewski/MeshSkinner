#include "pch.h"
#include "Time.h"

std::chrono::high_resolution_clock::time_point Time::s_StartTime = std::chrono::high_resolution_clock::now();
std::chrono::high_resolution_clock::time_point Time::s_LastFrameTime = std::chrono::high_resolution_clock::now();
std::chrono::high_resolution_clock::time_point Time::s_CurrentFrameTime = std::chrono::high_resolution_clock::now();

void Time::Init()
{
	s_StartTime = std::chrono::high_resolution_clock::now();
	s_LastFrameTime = s_StartTime;
	s_CurrentFrameTime = s_StartTime;
}

void Time::FrameStart()
{
	s_LastFrameTime = s_CurrentFrameTime;
	s_CurrentFrameTime = std::chrono::high_resolution_clock::now();
}

float Time::GetTimeSeconds()
{
	auto currentTime = std::chrono::high_resolution_clock::now();
	return std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - s_StartTime).count() / 1000000000.f;
}

float Time::GetFPS()
{
	return 1.f / GetDeltaSeconds();
}

float Time::GetDeltaSeconds()
{
	auto delta = std::chrono::duration_cast<std::chrono::nanoseconds>(s_CurrentFrameTime - s_LastFrameTime).count() / 1000000000.f;
	return delta < 0.000001f ? 1.f / 60.f : delta;
}
