#pragma once
#include "Engine/Core/Events/Event.h"

namespace Engine
{
	using EventCallbackFn = std::function<void(Event&)>;
	struct WindowUserData
	{
		std::string title;
		uint32_t width;
		uint32_t height;
		bool vSync;
		EventCallbackFn EventCallback;

		WindowUserData()
			: title(), width(0), height(0), vSync(false)
		{
		}
	};
}
