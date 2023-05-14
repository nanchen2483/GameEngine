#include "enginepch.h"
#include "Window.h"

#include "Engine/Core/System/System.h"
#include "Platform/Windows/WindowsWindow.h"

namespace Engine
{
	Uniq<Window> Window::Create(const WindowProps& props)
	{
		switch (System::GetOperatingSystem())
		{
		case OperatingSystem::Windows:		return CreateUniq<WindowsWindow>(props);
		}

		ENGINE_CORE_ASSERT(false, "Unknown operating system");
		return nullptr;
	}
}
