#include "LogSink.h"

namespace Engine
{
	void LogSink::sink_it_(const spdlog::details::log_msg& msg)
	{
		LogMessage message;
		spdlog::memory_buf_t formatted;
		formatter_->format(msg, formatted);
		message.payload = std::string(formatted.data(), formatted.size());
		message.level = GetLogLevel(msg.level);
		m_messages.push_back(message);
	}

	LogLevel LogSink::GetLogLevel(spdlog::level::level_enum level)
	{
		switch (level)
		{
		case spdlog::level::trace:
			return LogLevel::Trace;
		case spdlog::level::debug:
			return LogLevel::Debug;
		case spdlog::level::info:
			return LogLevel::Info;
		case spdlog::level::warn:
			return LogLevel::Warn;
		case spdlog::level::err:
		case spdlog::level::critical:
		case spdlog::level::off:
		case spdlog::level::n_levels:
		default:
			return LogLevel::Error;
		}
	}
}
