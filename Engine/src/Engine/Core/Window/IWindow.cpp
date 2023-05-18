#include "enginepch.h"
#include "IWindow.h"

#include "Engine/Core/System/System.h"
#include "Platform/Windows/WindowsWindow.h"

namespace Engine
{
	Uniq<IWindow> IWindow::Create(const WindowProperties& props)
	{
		switch (System::GetOperatingSystem())
		{
		case OperatingSystem::Windows:
		case OperatingSystem::Linux:
			return CreateUniq<WindowsWindow>(props);
		}

		ENGINE_CORE_ASSERT(false, "Unknown operating system");
		return nullptr;
	}
}
