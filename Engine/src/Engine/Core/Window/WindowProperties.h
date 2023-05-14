#pragma once
#include <string>

namespace Engine
{
	struct WindowProperties
	{
		std::string title;
		uint32_t width;
		uint32_t height;

		WindowProperties(const std::string& title = "My Engine",
			uint32_t width = 1280,
			uint32_t height = 720)
			: title(title), width(width), height(height)
		{
		}
	};
}
