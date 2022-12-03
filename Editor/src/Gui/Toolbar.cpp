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
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse
										| ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs;
		m_thumbnailSize = 32.0f;
		if (ImGui::Begin("Toolbar", 0, windowFlags))
		{
			float panelWidth = ImGui::GetContentRegionAvail().x;
			ImGui::SetCursorPosX((panelWidth - m_thumbnailSize) * 0.5f);
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ImageButton(m_playIcon, m_play, [=]()
				{
					m_play = true;
				});
			ImGui::SameLine();
			ImageButton(m_pauseIcon, !m_play, [=]()
				{
					m_pause = true;
				});
			ImGui::SameLine();
			ImageButton(m_stopIcon, !m_play, [=]()
				{
					m_play = false;
				});
			ImGui::PopStyleColor();
		}
		ImGui::End();
	}
	
	void Toolbar::ImageButton(Ptr<Texture2D> texture, bool isDisabled, std::function<void(void)> OnClick)
	{
		if (isDisabled)
		{
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
		}
		
		ImGui::ImageButton((ImTextureID)(uint64_t)texture->GetRendererId(), { m_thumbnailSize, m_thumbnailSize }, { 0, 1 }, { 1, 0 });
		
		if (isDisabled)
		{
			ImGui::PopItemFlag();
			ImGui::PopStyleVar();
		}

		if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
		{
			OnClick();
		}
	}
}
