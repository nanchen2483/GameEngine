#pragma once
#include "Engine/Core/Events/Event.h"

namespace Engine
{
	using EventCallbackFn = std::function<void(Event&)>;
	struct WindowUserData
	{
		std::string title{};
		uint32_t width{};
		uint32_t height{};
		bool vSync{};
		EventCallbackFn EventCallback{};
	};
}
