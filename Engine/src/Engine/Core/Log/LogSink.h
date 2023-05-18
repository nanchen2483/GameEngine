#pragma once

#include <vector>

#include <spdlog/sinks/base_sink.h>
#include <spdlog/spdlog.h>

namespace Engine
{
	enum class LogLevel
	{
		Trace = 0,
		Debug,
		Info,
		Warn,
		Error
	};

	struct LogMessage
	{
		std::string payload;
		LogLevel level;
	};

	class LogSink : public spdlog::sinks::base_sink<std::mutex>
	{
	public:
		LogSink() = default;
		const std::vector<LogMessage>& GetMessages() const { return m_messages; }
	protected:
		void sink_it_(const spdlog::details::log_msg& msg) override;
		void flush_() override {}
	private:
		LogLevel GetLogLevel(spdlog::level::level_enum level);

		std::vector<LogMessage> m_messages;
	};
}
