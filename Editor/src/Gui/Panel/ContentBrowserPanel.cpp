#include "ContentBrowserPanel.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <IconFontCppHeaders/IconsFontAwesome4.h>

namespace Engine
{
	static const std::filesystem::path s_assetPath = "assets";

	ContentBrowserPanel::ContentBrowserPanel()
		: m_currentDirectory(s_assetPath)
	{
		m_folderIcon = Texture2D::Create("resources/Icons/folder.png");
		m_fileIcon = Texture2D::Create("resources/Icons/file.png");
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		if (ImGui::Begin(ICON_FA_FOLDER " Content Browser"))
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			if (!m_backToPathStack.empty())
			{
				ImGui::Button(ICON_FA_ARROW_CIRCLE_O_LEFT, { 20, 20 });
				if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
				{
					m_forwardToPathStack.push_back(m_currentDirectory);
					m_currentDirectory = m_backToPathStack.back();
					m_backToPathStack.pop_back();
				}
			}
			else
			{
				ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
				ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
				ImGui::Button(ICON_FA_ARROW_CIRCLE_O_LEFT, { 20, 20 });
				ImGui::PopItemFlag();
				ImGui::PopStyleVar();
			}
			ImGui::SameLine(0, 0);
			if (!m_forwardToPathStack.empty())
			{
				ImGui::Button(ICON_FA_ARROW_CIRCLE_O_RIGHT, { 20, 20 });
				if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
				{
					m_backToPathStack.push_back(m_currentDirectory);
					m_currentDirectory = m_forwardToPathStack.back();
					m_forwardToPathStack.pop_back();
				}
			}
			else
			{
				ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
				ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
				ImGui::Button(ICON_FA_ARROW_CIRCLE_O_RIGHT, { 20, 20 });
				ImGui::PopItemFlag();
				ImGui::PopStyleVar();
			}
			ImGui::PopStyleColor();
			ImGui::SameLine();
			ImGui::Text(ICON_FA_FOLDER);
			for (const std::filesystem::path& folder : m_currentDirectory)
			{
				ImGui::SameLine();
				ImGui::Text(folder.string().c_str());
				ImGui::SameLine();
				ImGui::Text(ICON_FA_CHEVRON_RIGHT);
			}

			ImGui::Separator();
			ImGui::NewLine();

			static float padding = 32.0f;
			static float thumbnailSize = 48.0f;
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
				ImGui::ImageButton((ImTextureID)(uint64_t)icon->GetRendererId(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 }, 10);
			
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
						m_backToPathStack.push_back(m_currentDirectory);
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
