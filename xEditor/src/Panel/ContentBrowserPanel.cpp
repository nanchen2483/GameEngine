#include "ContentBrowserPanel.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace Engine
{
	static const std::filesystem::path s_assetPath = "assets";

	ContentBrowserPanel::ContentBrowserPanel()
		: m_currentDirectory(s_assetPath)
	{
		m_folderIcon = Texture2D::Create("resources/Icons/folder_icon.png");
		m_fileIcon = Texture2D::Create("resources/Icons/file_icon.png");
		m_backIcon = Texture2D::Create("resources/Icons/left-arrow_icon.png");
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		if (ImGui::Begin("Content Browser"))
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			bool onRootDir = m_currentDirectory == std::filesystem::path(s_assetPath);
			if (!onRootDir)
			{
				ImGui::ImageButton((ImTextureID)(uint64_t)m_backIcon->GetRendererId(), { 15, 15 }, { 0, 1 }, { 1, 0 });
				if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
				{
					m_currentDirectory = m_currentDirectory.parent_path();
				}
			}
			else
			{
				ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
				ImGui::ImageButton((ImTextureID)(uint64_t)m_backIcon->GetRendererId(), { 15, 15 }, { 0, 1 }, { 1, 0 });
				ImGui::PopItemFlag();
			}
			ImGui::PopStyleColor();

			static float padding = 64.0f;
			static float thumbnailSize = 32.0f;
			float cellSize = thumbnailSize + padding;

			float panelWidth = ImGui::GetContentRegionAvail().x;
			int columnCount = (int)(panelWidth / cellSize);
			if (columnCount < 1)
			{
				columnCount = 1;
			}

			ImGui::Columns(columnCount, 0, false);

			for (const std::filesystem::directory_entry& directoryEntry : std::filesystem::directory_iterator(m_currentDirectory))
			{
				const std::filesystem::path& path = directoryEntry.path();
				std::filesystem::path relativePath = std::filesystem::relative(path, s_assetPath);
				std::string filenameString = relativePath.filename().string();

				ImGui::PushID(filenameString.c_str());
				Ptr<Texture2D> icon = directoryEntry.is_directory() ? m_folderIcon : m_fileIcon;
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
				ImGui::ImageButton((ImTextureID)(uint64_t)icon->GetRendererId(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
			
				if (ImGui::BeginDragDropSource())
				{
					const wchar_t* itemPath = path.c_str();
					ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
					ImGui::EndDragDropSource();
				}

				ImGui::PopStyleColor();
				if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
				{
					if (directoryEntry.is_directory())
					{
						m_currentDirectory /= path.filename();
					}
				}
			
				ImGui::TextWrapped(filenameString.c_str());
				ImGui::NextColumn();
				ImGui::PopID();
			}

			ImGui::Columns(1);

			//ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);
			//ImGui::SliderFloat("Padding", &padding, 0, 128);

			ImGui::End();
		}
	}
}
