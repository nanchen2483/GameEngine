#pragma once
#include "Engine/Core/Base.h"
#include "LogSink.h"

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace Engine
{
	class ENGINE_API Log
	{
	public:
		static void Init();

		inline static const std::vector<LogMessage>& GetMessages() { return s_logSink->GetMessages(); }
		inline static Ptr<spdlog::logger>& GetCoreLogger() { return s_coreLogger; }
		inline static Ptr<spdlog::logger>& GetClientLogger() { return s_clientLogger; }
	private:
		static Ptr<spdlog::logger> s_coreLogger;
		static Ptr<spdlog::logger> s_clientLogger;
		static Ptr<LogSink> s_logSink;
	};
}

#define ENGINE_CORE_TRACE(...)		::Engine::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define ENGINE_CORE_DEBUG(...)		::Engine::Log::GetCoreLogger()->debug(__VA_ARGS__)
#define ENGINE_CORE_INFO(...)		::Engine::Log::GetCoreLogger()->info(__VA_ARGS__)
#define ENGINE_CORE_WARN(...)		::Engine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define ENGINE_CORE_ERROR(...)		::Engine::Log::GetCoreLogger()->error(__VA_ARGS__)
#define ENGINE_CORE_CRITICAL(...)	::Engine::Log::GetCoreLogger()->critical(__VA_ARGS__)

#define ENGINE_TRACE(...)			::Engine::Log::GetClientLogger()->trace(__VA_ARGS__)
#define ENGINE_INFO(...)			::Engine::Log::GetClientLogger()->info(__VA_ARGS__)
#define ENGINE_WARN(...)			::Engine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define ENGINE_ERROR(...)			::Engine::Log::GetClientLogger()->error(__VA_ARGS__)
