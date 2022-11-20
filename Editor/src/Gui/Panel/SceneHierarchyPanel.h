#pragma once
#include "Gui/Widget.h"
#include "Engine.h"

namespace Engine
{
	class SceneHierarchyPanel : public Widget
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ptr<Scene>& context);

		void SetContext(const Ptr<Scene>& context);
		virtual void OnImGuiRender() override;
		Entity GetSelectedEntity() const { return m_selectionContext; };
		void SetSelectedEntity(Entity& entity);
	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);
		
		Ptr<Scene> m_context;
		Entity m_selectionContext;
	};
}

