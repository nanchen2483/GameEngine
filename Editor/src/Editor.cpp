#include "Engine.h"
#include "Engine/Core/EntryPoint.h"

#include "EditorLayer.h"
#include "ExampleLayer.h"

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

		virtual ~Editor() = default;
	};

	Application* CreateApplication()
	{
		return new Editor();
	}
}
