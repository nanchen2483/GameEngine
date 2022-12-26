#pragma once
#include "Key.h"
#include <string>

namespace Engine
{
	class KeyGenerator
	{
	public:
		static Key HashKey(const std::string& str);
	};
}
