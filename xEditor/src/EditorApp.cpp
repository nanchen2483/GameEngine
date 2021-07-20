#pragma once

#include "Engine.h"
#include "Engine/Core/EntryPoint.h"

#include "EditorLayer.h"

namespace Engine
{
	class Editor : public Application
	{
	public:
		Editor()
			: Application("Editor")
		{
			PushLayer(new EditorLayer());
		}

		~Editor()
		{
		}
	};

	Application* CreateApplication()
	{
		return new Editor();
}
}