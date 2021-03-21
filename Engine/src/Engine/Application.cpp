#include "enginepch.h"

#include "Application.h"
#include "Engine/Event/ApplicationEvent.h"
#include "Engine/Log.h"

namespace Engine
{
	Application::Application()
	{
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		WindowResizeEvent e(1280, 720);
		if (e.IsIncategory(EventCategoryApplication))
		{
			ENGINE_TRACE(e);
		}
		if (e.IsIncategory(EventCategoryInput))
		{
			ENGINE_WARN(e);
		}
		while (true);
	}
}