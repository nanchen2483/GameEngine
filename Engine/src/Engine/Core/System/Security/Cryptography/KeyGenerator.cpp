#include "KeyGenerator.h"
#include <functional>

namespace Engine
{
	Key KeyGenerator::HashKey(const std::string& str)
	{
		Key hashKey = std::hash<uint64_t, std::string>{}(str);
		return hashKey;
	}
}
