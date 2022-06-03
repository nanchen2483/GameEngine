#pragma once
#include "Engine/Core/Base.h"
#include "Engine/Renderer/Texture/Texture.h"

#include <filesystem>

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
