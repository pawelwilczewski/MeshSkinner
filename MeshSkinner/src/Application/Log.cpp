#include "pch.h"
#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "Application/Application.h"

std::shared_ptr<spdlog::logger> Log::logger;

void Log::Init()
{
	spdlog::set_pattern("%^[%T] %n: %v%$");
	logger = spdlog::stdout_color_mt("LOG");
	logger->set_level(spdlog::level::trace);
}
