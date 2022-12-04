#pragma once
#include "Gui/IGui.h"
#include "Gui/Panel/ContentBrowserPanel.h"

namespace Engine
{
	class Statusbar : public IGui
	{
	public:
		Statusbar();
		virtual void OnImGuiRender() override;
	private:
		Uniq<ContentBrowserPanel> m_contentBrowserPanel;
	};
}
