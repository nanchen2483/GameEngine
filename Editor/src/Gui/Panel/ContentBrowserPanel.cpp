#include "ContentBrowserPanel.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <IconFontCppHeaders/IconsFontAwesome4.h>

namespace Engine
{
	static const std::filesystem::path s_rootPath = "assets";

	ContentBrowserPanel::ContentBrowserPanel()
		: m_currentDirectory(s_rootPath)
	{
		m_folderIcon = TextureLibrary::Load("resources/Icons/folder.png");
		m_fileIcon = TextureLibrary::Load("resources/Icons/file.png");
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		if (m_open)
		{
			if (ImGui::Begin(ICON_FA_FOLDER " Content Browser", &m_open))
			{
				ImVec2 regionSize = ImGui::GetContentRegionAvail();
				static float splitterSize = 2;
				static float folderOverviewHeight = 20.0f;
				static float folderHierarchyWidth = 200;
				float folderHierarchyHeight = regionSize.y - folderOverviewHeight - splitterSize - 10;

				if (ImGui::BeginChild("folderOverviewChild", ImVec2(0, folderOverviewHeight), false))
				{
					AddressBar();
					ImGui::EndChild();
				}
				ImGui::Button("vsplitter", ImVec2(-1, splitterSize));
				{
					if (ImGui::BeginChild("folderHierarcyChild", ImVec2(folderHierarchyWidth, folderHierarchyHeight + splitterSize), false))
					{
						NavigationPanel(s_rootPath);
						ImGui::EndChild();
					}
					ImGui::SameLine();
					ImGui::Button("hsplitter", ImVec2(splitterSize, -1));
					if (ImGui::IsItemActive())
					{
						float deltaX = ImGui::GetIO().MouseDelta.x;
						float newWidth = deltaX + folderHierarchyWidth;
						if (newWidth > 0 && newWidth < regionSize.x - 20)
						{
							folderHierarchyWidth = newWidth;
						}
					}
					ImGui::SameLine();
					if (ImGui::BeginChild("FolderContentChild", ImVec2(0, folderHierarchyHeight + splitterSize), false))
					{
						FolderContent();
						ImGui::EndChild();
					}
				}
			}
			ImGui::End();
		}
	}
	
	void ContentBrowserPanel::AddressBar()
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
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		std::filesystem::path currentSubDir = {};
		for (const std::filesystem::path& folder : m_currentDirectory)
		{
			currentSubDir /= folder;
			ImGui::SameLine();
			if (ImGui::Button(folder.string().c_str()))
			{
				if (m_currentDirectory != currentSubDir)
				{
					m_backToPathStack.push_back(m_currentDirectory);
					m_forwardToPathStack = {};
					m_currentDirectory = currentSubDir;
					break;
				}
			}
			ImGui::SameLine();
			ImGui::Text(ICON_FA_CHEVRON_RIGHT);
		}
		ImGui::PopStyleColor();
	}

	void ContentBrowserPanel::NavigationPanel(std::filesystem::path directory)
	{
		if (directory.empty())
		{
			return;
		}

		static bool isRootNode = directory == s_rootPath;
		if (isRootNode)
		{
			// Open root node as default
			isRootNode = false;
			ImGui::SetNextItemOpen(true);
		}

		std::vector<std::filesystem::path> subdirs;
		for (const std::filesystem::directory_entry& directoryEntry : std::filesystem::directory_iterator(directory))
		{
			const std::filesystem::path& path = directoryEntry.path();
			if (directoryEntry.is_directory())
			{
				subdirs.push_back(path);
			}
		}

		ImGuiTreeNodeFlags treeNodeFlag = ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_SpanAvailWidth;
		if (m_currentDirectory.string().find(directory.string()) != std::string::npos)
		{
			treeNodeFlag |= ImGuiTreeNodeFlags_Selected;
		}

		if (subdirs.empty())
		{
			treeNodeFlag |= ImGuiTreeNodeFlags_Leaf;
		}

		size_t treeNodeId = std::hash<std::string>()(directory.string());
		std::string icon;
		if (ImGui::TreeNodeBehaviorIsOpen(ImGui::GetID((void*)treeNodeId)))
		{
			icon = ICON_FA_FOLDER_OPEN " ";
		}
		else
		{
			icon = ICON_FA_FOLDER " ";
		}

		if (ImGui::TreeNodeEx((void*)treeNodeId, treeNodeFlag, "%s", (icon + directory.filename().string()).c_str()))
		{
			if (ImGui::IsItemFocused())
			{
				if (m_currentDirectory != directory)
				{
					m_backToPathStack.push_back(m_currentDirectory);
					m_currentDirectory = directory;
				}
			}

			for (const std::filesystem::path& subdir : subdirs)
			{
				ImGui::PushID(subdir.string().c_str());
				NavigationPanel(subdir);
				ImGui::PopID();
			}
			ImGui::TreePop();
		}
	}

	void ContentBrowserPanel::FolderContent()
	{
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

		static uint32_t buttonPadding = 10;
		for (const std::filesystem::directory_entry& directoryEntry : std::filesystem::directory_iterator(m_currentDirectory))
		{
			const std::filesystem::path& path = directoryEntry.path();
			std::filesystem::path relativePath = std::filesystem::relative(path, s_rootPath);
			std::string filenameString = relativePath.filename().string();

			ImGui::PushID(filenameString.c_str());
			Ptr<ITexture2D> icon = directoryEntry.is_directory() ? m_folderIcon : m_fileIcon;
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ImGui::ImageButton((ImTextureID)(uint64_t)icon->GetRendererId(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 }, buttonPadding);

			if (ImGui::BeginDragDropSource())
			{
				const wchar_t* itemPath = (const wchar_t*)path.c_str();
				ImGui::SetDragDropPayload(File::GetFileType(path).c_str(), itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
				ImGui::Text("%s", filenameString.c_str());
				ImGui::EndDragDropSource();
			}

			ImGui::PopStyleColor();
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (directoryEntry.is_directory())
				{
					m_backToPathStack.push_back(m_currentDirectory);
					m_forwardToPathStack = {};
					m_currentDirectory /= path.filename();
				}
			}

			float textWidth = ImGui::CalcTextSize(filenameString.c_str()).x;
			if (textWidth < (thumbnailSize + buttonPadding))
			{
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (thumbnailSize + buttonPadding - textWidth) * 0.5f);
			}

			ImGui::TextWrapped("%s", filenameString.c_str());
			ImGui::NextColumn();
			ImGui::PopID();
		}
		ImGui::Columns(1);
	}
}
