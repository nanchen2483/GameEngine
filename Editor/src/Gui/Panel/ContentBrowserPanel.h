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
		virtual void OnImGuiRender() override;
	private:
		std::filesystem::path m_currentDirectory;

		Ptr<Texture2D> m_folderIcon;
		Ptr<Texture2D> m_fileIcon;
		Ptr<Texture2D> m_backIcon;
	};
}