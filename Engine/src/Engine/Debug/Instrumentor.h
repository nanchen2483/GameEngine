#pragma once
#include "Engine/Core/Log/Log.h"

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>

namespace Engine
{
	using FloatingPointMicroseconds = std::chrono::duration<double, std::micro>;

	struct ProfileResult
	{
		std::string name;

		FloatingPointMicroseconds start;
		std::chrono::microseconds elapsedTime;
		std::thread::id threadID;
	};

	struct InstrumentationSession
	{
		std::string name;
	};

	class Instrumentor
	{
	public:
		Instrumentor(const Instrumentor&) = delete;
		Instrumentor(Instrumentor&&) = delete;

		void BeginSession(const std::string& name, const std::string& filepath = "results.json")
		{
			std::lock_guard lock(m_mutex);
			if (m_currentSession)
			{
				if (Log::GetCoreLogger()) // Edge case: BeginSession() might be before Log::Init()
				{
					ENGINE_CORE_ERROR("Instrumentor::BeginSession('{0}') when session '{1}' already open.", name, m_currentSession->name);
				}
				InternalEndSession();
			}
			m_outputStream.open(filepath);

			if (m_outputStream.is_open())
			{
				m_currentSession = new InstrumentationSession({ name });
				WriteHeader();
			}
			else
			{
				if (Log::GetCoreLogger())
				{
					ENGINE_CORE_ERROR("Instrumentor could not open results file '{0}'.", filepath);
				}
			}
		}

		void EndSession()
		{
			std::lock_guard lock(m_mutex);
			InternalEndSession();
		}

		void WriteProfile(const ProfileResult& result)
		{
			std::stringstream json;

			json << std::setprecision(3) << std::fixed;
			json << ",{";
			json << "\"cat\":\"function\",";
			json << "\"dur\":" << (result.elapsedTime.count()) << ',';
			json << "\"name\":\"" << result.name << "\",";
			json << "\"ph\":\"X\",";
			json << "\"pid\":0,";
			json << "\"tid\":" << result.threadID << ",";
			json << "\"ts\":" << result.start.count();
			json << "}";

			std::lock_guard lock(m_mutex);
			if (m_currentSession)
			{
				m_outputStream << json.str();
				m_outputStream.flush();
			}
		}

		static Instrumentor& GetInstance()
		{
			static Instrumentor instance;
			return instance;
		}
	private:
		Instrumentor()
			: m_currentSession(nullptr)
		{
		}

		~Instrumentor()
		{
			EndSession();
		}

		void WriteHeader()
		{
			m_outputStream << "{\"otherData\": {},\"traceEvents\":[{}";
			m_outputStream.flush();
		}

		void WriteFooter()
		{
			m_outputStream << "]}";
			m_outputStream.flush();
		}

		void InternalEndSession()
		{
			if (m_currentSession)
			{
				WriteFooter();
				m_outputStream.close();
				delete m_currentSession;
				m_currentSession = nullptr;
			}
		}
	private:
		std::mutex m_mutex;
		InstrumentationSession* m_currentSession;
		std::ofstream m_outputStream;
	};

	class InstrumentationTimer
	{
	public:
		InstrumentationTimer(const char* name)
			: m_name(name), m_stopped(false)
		{
			m_startTimepoint = std::chrono::steady_clock::now();
		}

		~InstrumentationTimer()
		{
			if (!m_stopped)
			{
				Stop();
			}
		}

		void Stop()
		{
			std::chrono::steady_clock::time_point endTimepoint = std::chrono::steady_clock::now();
			FloatingPointMicroseconds highResStart = FloatingPointMicroseconds{ m_startTimepoint.time_since_epoch() };
			std::chrono::microseconds elapsedTime = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch() - std::chrono::time_point_cast<std::chrono::microseconds>(m_startTimepoint).time_since_epoch();

			Instrumentor::GetInstance().WriteProfile({ m_name, highResStart, elapsedTime, std::this_thread::get_id() });

			m_stopped = true;
		}
	private:
		const char* m_name;
		std::chrono::time_point<std::chrono::steady_clock> m_startTimepoint;
		bool m_stopped;
	};

	namespace InstrumentorUtils
	{
		template <size_t N>
		struct ChangeResult
		{
			char data[N];
		};

		template <size_t N, size_t K>
		constexpr InstrumentorUtils::ChangeResult<N> CleanupOutputString(const char(&expr)[N], const char(&remove)[K])
		{
			ChangeResult<N> result = {};

			size_t srcIndex = 0;
			size_t dstIndex = 0;
			while (srcIndex < N)
			{
				size_t matchIndex = 0;
				while (matchIndex < K - 1 && srcIndex + matchIndex < N - 1 && expr[srcIndex + matchIndex] == remove[matchIndex])
				{
					matchIndex++;
				}

				if (matchIndex == K - 1)
				{
					srcIndex += matchIndex;
				}

				result.data[dstIndex++] = expr[srcIndex] == '"' ? '\'' : expr[srcIndex];
				srcIndex++;
			}
			return result;
		}
	}
}

#define ENGINE_PROFILE 1
#if ENGINE_PROFILE
#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
#define ENGINE_FUNC_SIG __PRETTY_FUNCTION__
#elif defined(__DMC__) && (__DMC__ >= 0x810)
#define ENGINE_FUNC_SIG __PRETTY_FUNCTION__
#elif (defined(__FUNCSIG__) || (_MSC_VER))
#define ENGINE_FUNC_SIG __FUNCSIG__
#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
#define ENGINE_FUNC_SIG __FUNCTION__
#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
#define ENGINE_FUNC_SIG __FUNC__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
#define ENGINE_FUNC_SIG __func__
#elif defined(__cplusplus) && (__cplusplus >= 201103)
#define ENGINE_FUNC_SIG __func__
#else
#define ENGINE_FUNC_SIG "ENGINE_FUNC_SIG unknown!"
#endif

#define ENGINE_PROFILE_BEGIN_SESSION(name, filepath) ::Engine::Instrumentor::GetInstance().BeginSession(name, filepath)
#define ENGINE_PROFILE_END_SESSION() ::Engine::Instrumentor::GetInstance().EndSession()
#define ENGINE_PROFILE_SCOPE_LINE2(name, line) constexpr auto fixedName##line = ::Engine::InstrumentorUtils::CleanupOutputString(name, "__cdecl ");\
											   ::Engine::InstrumentationTimer timer##line(fixedName##line.data)
#define ENGINE_PROFILE_SCOPE_LINE(name, line) ENGINE_PROFILE_SCOPE_LINE2(name, line)
#define ENGINE_PROFILE_SCOPE(name) ENGINE_PROFILE_SCOPE_LINE(name, __LINE__)
#define ENGINE_PROFILE_FUNCTION() ENGINE_PROFILE_SCOPE(ENGINE_FUNC_SIG)
#else
#define ENGINE_PROFILE_BEGIN_SESSION(name, filepath)
#define ENGINE_PROFILE_END_SESSION()
#define ENGINE_PROFILE_SCOPE(name)
#define ENGINE_PROFILE_FUNCTION()
#endif
