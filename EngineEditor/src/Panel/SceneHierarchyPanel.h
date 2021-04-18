#pragma once

#include "Engine.h"

namespace Engine
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ptr<Scene>& context);

		void SetContext(const Ptr<Scene>& context);

		void OmImGuiRender();
	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);
	private:
		Ptr<Scene> m_context;
		Entity m_selectionContext;
	};
}

