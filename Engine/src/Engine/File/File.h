#pragma once
#include <filesystem>

namespace Engine
{
	class File
	{
	public:
		static std::string GetFileType(std::filesystem::path file);
	};
}
