#pragma once
#include <string>

namespace Engine
{
	class ENGINE_API FileDialog
	{
	public:
		static std::string OpenFile(const char* filter);
		static std::string SaveFile(const char* filter);
	};
}
