#pragma once
#include "Gui/IGui.h"
#include "Engine.h"

namespace Engine
{
	class HierarchyPanel : public IGui
	{
	public:
		HierarchyPanel() = default;
		HierarchyPanel(const Ptr<Scene>& context);

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

