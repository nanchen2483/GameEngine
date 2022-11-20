#pragma once
#include "Gui/Widget.h"
#include "Engine.h"

#include <filesystem>

namespace Engine
{
	class ContentBrowserPanel : public Widget
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
