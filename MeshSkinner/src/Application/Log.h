#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
#include <spdlog/fmt/bundled/format.h>

class Log
{
	friend class Application;

private:
	static void Init();

public:
	template<typename... Args> static void Trace(const char *format, Args &&... args) { logger->trace(format, std::forward<Args>(args)...); }
	template<typename... Args> static void Info(const char *format, Args &&... args) { logger->info(format, std::forward<Args>(args)...); }
	template<typename... Args> static void Warn(const char *format, Args &&... args) { logger->warn(format, std::forward<Args>(args)...); }
	template<typename... Args> static void Error(const char *format, Args &&... args) { logger->error(format, std::forward<Args>(args)...); }
	template<typename... Args> static void Critical(const char *format, Args &&... args) { logger->critical(format, std::forward<Args>(args)...); }

private:
	static std::shared_ptr<spdlog::logger> logger;
};

// -----------------------------------
//      Custom types formatting
// https://fmt.dev/latest/api.html#udt
// -----------------------------------

template <> struct fmt::formatter<glm::vec2>
{
	constexpr auto parse(format_parse_context &ctx) -> decltype(ctx.begin()) { return ctx.end(); }

	template <typename FormatContext>
	auto format(const glm::vec2 &in, FormatContext &ctx) const -> decltype(ctx.out())
	{
		return fmt::format_to(ctx.out(), "v2({:.3f}, {:.3f})", in.x, in.y);
	}
};

template <> struct fmt::formatter<glm::ivec2>
{
	constexpr auto parse(format_parse_context &ctx) -> decltype(ctx.begin()) { return ctx.end(); }

	template <typename FormatContext>
	auto format(const glm::ivec2 &in, FormatContext &ctx) const -> decltype(ctx.out())
	{
		return fmt::format_to(ctx.out(), "v2({}, {})", in.x, in.y);
	}
};

template <> struct fmt::formatter<glm::vec3>
{
	constexpr auto parse(format_parse_context &ctx) -> decltype(ctx.begin()) { return ctx.end(); }

	template <typename FormatContext>
	auto format(const glm::vec3 &in, FormatContext &ctx) const -> decltype(ctx.out())
	{
		return fmt::format_to(ctx.out(), "v3({:.3f}, {:.3f}, {:.3f})", in.x, in.y, in.z);
	}
};

template <> struct fmt::formatter<glm::vec4>
{
	constexpr auto parse(format_parse_context &ctx) -> decltype(ctx.begin()) { return ctx.end(); }

	template <typename FormatContext>
	auto format(const glm::vec4 &in, FormatContext &ctx) const -> decltype(ctx.out())
	{
		return fmt::format_to(ctx.out(), "v4({:.3f}, {:.3f}, {:.3f}, {:.3f})", in.x, in.y, in.z, in.w);
	}
};

template <> struct fmt::formatter<glm::quat>
{
	constexpr auto parse(format_parse_context &ctx) -> decltype(ctx.begin()) { return ctx.end(); }

	template <typename FormatContext>
	auto format(const glm::quat &in, FormatContext &ctx) const -> decltype(ctx.out())
	{
		return fmt::format_to(ctx.out(), "q({:.3f}, {:.3f}, {:.3f}, {:.3f})", in.x, in.y, in.z, in.w);
	}
};

template <> struct fmt::formatter<glm::mat4>
{
	constexpr auto parse(format_parse_context &ctx) -> decltype(ctx.begin()) { return ctx.end(); }

	template <typename FormatContext>
	auto format(const glm::mat4 &in, FormatContext &ctx) const -> decltype(ctx.out())
	{
		return fmt::format_to(
			ctx.out(),
			"mat4\n"
			"\t{:.3f}\t{:.3f}\t{:.3f}\t{:.3f}\n"
			"\t{:.3f}\t{:.3f}\t{:.3f}\t{:.3f}\n"
			"\t{:.3f}\t{:.3f}\t{:.3f}\t{:.3f}\n"
			"\t{:.3f}\t{:.3f}\t{:.3f}\t{:.3f}",
			in[0][0], in[1][0], in[2][0], in[3][0],
			in[0][1], in[1][1], in[2][1], in[3][1],
			in[0][2], in[1][2], in[2][2], in[3][2],
			in[0][3], in[1][3], in[2][3], in[3][3]
		);
	}
};

template <> struct fmt::formatter<glm::mat3>
{
	constexpr auto parse(format_parse_context &ctx) -> decltype(ctx.begin()) { return ctx.end(); }

	template <typename FormatContext>
	auto format(const glm::mat3 &in, FormatContext &ctx) const -> decltype(ctx.out())
	{
		return fmt::format_to(
			ctx.out(),
			"mat3\n"
			"\t{:.3f}\t{:.3f}\t{:.3f}\n"
			"\t{:.3f}\t{:.3f}\t{:.3f}\n"
			"\t{:.3f}\t{:.3f}\t{:.3f}",
			in[0][0], in[1][0], in[2][0],
			in[0][1], in[1][1], in[2][1],
			in[0][2], in[1][2], in[2][2]
		);
	}
};
