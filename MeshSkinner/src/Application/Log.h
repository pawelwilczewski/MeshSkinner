#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

class Log
{
	friend class Application;

private:
	static void Init();

public:
	template<typename... Args> static void Trace(const char *format, Args &&... args) { s_Logger->trace(format, std::forward<Args>(args)...); }
	template<typename... Args> static void Info(const char *format, Args &&... args) { s_Logger->info(format, std::forward<Args>(args)...); }
	template<typename... Args> static void Warn(const char *format, Args &&... args) { s_Logger->warn(format, std::forward<Args>(args)...); }
	template<typename... Args> static void Error(const char *format, Args &&... args) { s_Logger->error(format, std::forward<Args>(args)...); }
	template<typename... Args> static void Critical(const char *format, Args &&... args) { s_Logger->critical(format, std::forward<Args>(args)...); }

private:
	static std::shared_ptr<spdlog::logger> s_Logger;
};
