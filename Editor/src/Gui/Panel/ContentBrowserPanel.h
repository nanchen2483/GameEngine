#pragma once
#include "Gui/IGui.h"
#include "Engine.h"

#include <filesystem>

namespace Engine
{
	class ContentBrowserPanel : public IGui
	{
	public:
		ContentBrowserPanel();
		void ToggleWindowStatus() { m_open = !m_open; }
		virtual void OnImGuiRender() override;
	private:
		bool m_open = true;
		std::filesystem::path m_currentDirectory;

		Ptr<Texture2D> m_folderIcon;
		Ptr<Texture2D> m_fileIcon;

		std::vector<std::filesystem::path> m_backToPathStack;
		std::vector<std::filesystem::path> m_forwardToPathStack;
	};
}
