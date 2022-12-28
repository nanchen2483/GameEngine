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

		void SetOutlinerWindowStatus(bool open) { m_openOutliner = open; }
		void SetDetailsWindowStatus(bool open) { m_openDetails = open; }
		bool IsOutlinerWindowOpen() const { return m_openOutliner; }
		bool IsDetailsWindowOpen() const { return m_openDetails; }

		void SetContext(const Ptr<Scene>& context);
		virtual void OnImGuiRender() override;
		Entity GetSelectedEntity() const { return m_selectionContext; };
		void SetSelectedEntity(Entity& entity);
	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);
		
		bool m_openOutliner = true;
		bool m_openDetails = true;
		Ptr<Scene> m_context;
		Entity m_selectionContext;
	};
}

