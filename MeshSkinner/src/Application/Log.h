#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

class Log
{
	friend class Application;

private:
	static void Init();

public:
	static std::shared_ptr<spdlog::logger> &GetLogger() { return s_Logger; }

	template<typename... Args> static void Trace(const char *format, Args &&... args) { GetLogger()->trace(format, std::forward<Args>(args)...); }
	template<typename... Args> static void Info(const char *format, Args &&... args) { GetLogger()->info(format, std::forward<Args>(args)...); }
	template<typename... Args> static void Warn(const char *format, Args &&... args) { GetLogger()->warn(format, std::forward<Args>(args)...); }
	template<typename... Args> static void Error(const char *format, Args &&... args) { GetLogger()->error(format, std::forward<Args>(args)...); }
	template<typename... Args> static void Critical(const char *format, Args &&... args) { GetLogger()->critical(format, std::forward<Args>(args)...); }

private:
	static std::shared_ptr<spdlog::logger> s_Logger;
};
