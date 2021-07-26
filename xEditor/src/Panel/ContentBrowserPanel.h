#pragma once

#include <filesystem>

#include "Engine/Core/Base.h"
#include <Engine/Renderer/Texture.h>

namespace Engine
{
	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();
		void OnImGuiRender();
	private:
		std::filesystem::path m_currentDirectory;

		Ptr<Texture2D> m_folderIcon;
		Ptr<Texture2D> m_fileIcon;
		Ptr<Texture2D> m_backIcon;
	};
}
