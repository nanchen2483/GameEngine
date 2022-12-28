#include "Statusbar.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <IconFontCppHeaders/IconsFontAwesome6.h>

namespace Engine
{
	Statusbar::Statusbar()
	{
		m_contentBrowserPanel = CreateUniq<ContentBrowserPanel>();
		m_outputLogPanel = CreateUniq<OutputLogPanel>();
	}

	void Statusbar::OnImGuiRender()
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar;
		float height = ImGui::GetFrameHeight();
		if (ImGui::BeginViewportSideBar("##StatusMenuBar", viewport, ImGuiDir_Down, height, windowFlags))
		{
			if (ImGui::BeginMenuBar())
			{
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
				if (ImGui::Button(ICON_FA_FOLDER " Content Browser"))
				{
					m_contentBrowserPanel->ToggleWindowStatus();
				}

				if (ImGui::Button(ICON_FA_TERMINAL " Output Log"))
				{
					m_outputLogPanel->ToggleWindowStatus();
				}
				ImGui::PopStyleColor();
				ImGui::EndMenuBar();
			}
			ImGui::End();
		}

		m_contentBrowserPanel->OnImGuiRender();
		m_outputLogPanel->OnImGuiRender();
	}
}
