#include "enginepch.h"
#include "Uid.h"
#include <chrono>
#include <functional>
#include <type_traits>
#include <format>

namespace Engine
{
	Uid::Uid()
		: m_uid(0)
	{
	}

	Uid::Uid(const std::string& str)
		: m_uid(std::hash<std::string>{}(str))
	{
	}

	Uid Uid::NewUid()
	{
		std::chrono::system_clock::time_point currentTime = std::chrono::system_clock::now();
		long miliseconds = std::chrono::time_point_cast<std::chrono::milliseconds>(currentTime).time_since_epoch().count();

		return Uid(std::to_string(miliseconds));
	}

	Uid Uid::NewUid(const std::string& str)
	{
		return Uid(str);
	}
}
