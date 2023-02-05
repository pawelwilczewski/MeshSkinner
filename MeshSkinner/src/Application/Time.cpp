#include "pch.h"
#include "Time.h"

std::chrono::high_resolution_clock::time_point Time::startTime = std::chrono::high_resolution_clock::now();
std::chrono::high_resolution_clock::time_point Time::lastFrameTime = std::chrono::high_resolution_clock::now();
std::chrono::high_resolution_clock::time_point Time::currentFrameTime = std::chrono::high_resolution_clock::now();

void Time::Init()
{
	startTime = std::chrono::high_resolution_clock::now();
	lastFrameTime = startTime;
	currentFrameTime = startTime;
}

void Time::FrameBegin()
{
	lastFrameTime = currentFrameTime;
	currentFrameTime = std::chrono::high_resolution_clock::now();
}

float Time::GetTimeSeconds()
{
	auto currentTime = std::chrono::high_resolution_clock::now();
	return std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - startTime).count() / 1000000000.f;
}

float Time::GetFPS()
{
	return 1.f / GetDeltaSeconds();
}

float Time::GetDeltaSeconds()
{
	auto delta = std::chrono::duration_cast<std::chrono::nanoseconds>(currentFrameTime - lastFrameTime).count() / 1000000000.f;
	return delta < 0.000001f ? 1.f / 60.f : delta;
}
