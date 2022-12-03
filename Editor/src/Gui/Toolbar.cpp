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
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollWithMouse;
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { m_toolbarPadding, m_toolbarPadding });
		if (ImGui::BeginViewportSideBar("##ToolMenuBar", viewport, ImGuiDir_Up, m_toolbarSize, windowFlags))
		{
			float panelWidth = ImGui::GetContentRegionAvail().x;
			ImGui::SetCursorPosX((panelWidth - m_buttonSize) * 0.5f);
			ImageButton(m_playIcon, m_play, [=]()
				{
					m_play = true;
				});
			ImGui::SameLine(0, 0);
			ImageButton(m_pauseIcon, !m_play, [=]()
				{
					m_pause = true;
				});
			ImGui::SameLine(0, 0);
			ImageButton(m_stopIcon, !m_play, [=]()
				{
					m_play = false;
				});
			ImGui::End();
		}
		ImGui::PopStyleVar();
	}
	
	void Toolbar::ImageButton(Ptr<Texture2D> texture, bool isDisabled, std::function<void(void)> OnClick)
	{
		if (isDisabled)
		{
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
		}
		
		ImGui::ImageButton((ImTextureID)(uint64_t)texture->GetRendererId(), { m_buttonSize, m_buttonSize }, { 0, 1 }, { 1, 0 }, m_buttonPadding);

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
