#pragma once
#include <filesystem>

namespace Engine
{
	class ENGINE_API File
	{
	public:
		static std::string GetFileType(std::filesystem::path file);
	};
}
