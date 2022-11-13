#include "Toolbar.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace Engine
{
	Toolbar::Toolbar()
	{
		m_playIcon = Texture2D::Create("resources/Icons/play-button.png");
		m_pauseIcon = Texture2D::Create("resources/Icons/pause-button.png");
		m_stopIcon = Texture2D::Create("resources/Icons/stop-button.png");
	}

	void Toolbar::OnImGuiRender()
	{
		static float thumbnailSize = 32.0f;
		if (ImGui::Begin("Toolbar", 0, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
		{
			float panelWidth = ImGui::GetContentRegionAvail().x;
			ImGui::SetCursorPosX((panelWidth - thumbnailSize) * 0.5f);
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			if (m_play)
			{
				ImGui::ImageButton((ImTextureID)(uint64_t)m_pauseIcon->GetRendererId(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
				if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
				{
					m_pause = true;
				}
				ImGui::SameLine();
				ImGui::ImageButton((ImTextureID)(uint64_t)m_stopIcon->GetRendererId(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
				if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
				{
					m_play = false;
				}
			}
			else
			{
				ImGui::ImageButton((ImTextureID)(uint64_t)m_playIcon->GetRendererId(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
				if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
				{
					m_play = true;
				}
			}
			ImGui::PopStyleColor();
			ImGui::End();
		}
	}
}
