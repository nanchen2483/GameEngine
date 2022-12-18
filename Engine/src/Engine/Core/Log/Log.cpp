#include "enginepch.h"
#include "Log.h"
#include "LogSink.h"

#include <spdlog/pattern_formatter.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Engine {
	Ptr<spdlog::logger> Log::s_coreLogger;
	Ptr<spdlog::logger> Log::s_clientLogger;
	Ptr<LogSink> Log::s_logSink;

	void Log::Init()
	{
		s_logSink = CreatePtr<LogSink>();

		std::vector<spdlog::sink_ptr> engineSinks;
		engineSinks.push_back(CreatePtr<spdlog::sinks::stdout_color_sink_mt>());
		engineSinks.push_back(s_logSink);
		s_coreLogger = CreatePtr<spdlog::logger>("Engine", begin(engineSinks), end(engineSinks));
		s_coreLogger->set_level(spdlog::level::trace);
		s_coreLogger->set_formatter(std::unique_ptr<spdlog::formatter>(
			new spdlog::pattern_formatter("%^[%T] %n: %v%$", spdlog::pattern_time_type::local)));

		std::vector<spdlog::sink_ptr> editorSinks;
		editorSinks.push_back(CreatePtr<spdlog::sinks::stdout_color_sink_mt>());
		editorSinks.push_back(s_logSink);
		s_clientLogger = CreatePtr<spdlog::logger>("Editor", begin(editorSinks), end(editorSinks));
		s_clientLogger->set_level(spdlog::level::trace);
		s_clientLogger->set_formatter(std::unique_ptr<spdlog::formatter>(
			new spdlog::pattern_formatter("%^[%T] %n: %v%$", spdlog::pattern_time_type::local)));
	}
}
