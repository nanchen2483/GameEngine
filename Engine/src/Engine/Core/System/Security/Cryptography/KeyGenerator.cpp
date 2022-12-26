#include "enginepch.h"
#include "KeyGenerator.h"
#include <functional>

namespace Engine
{
	Key KeyGenerator::HashKey(const std::string& str)
	{
		Key hashKey = std::hash<std::string>{}(str);
		return hashKey;
	}
}
