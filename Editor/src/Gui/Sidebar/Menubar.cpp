#include "Menubar.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace Engine
{
	Menubar::Menubar()
	{
		m_hierachyPanel = CreatePtr<HierarchyPanel>();
	}

	void Menubar::SetFunctions(Func newScene, Func openScene, Func saveSceneAs)
	{
		NewScene = newScene;
		OpenScene = openScene;
		SaveSceneAs = saveSceneAs;
	}

	void Menubar::OnImGuiRender()
	{
		m_showOutliner = m_hierachyPanel->IsOutlinerWindowOpen();
		m_showDetails = m_hierachyPanel->IsDetailsWindowOpen();

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New", "Ctrl+N"))
				{
					NewScene();
				}

				if (ImGui::MenuItem("Open...", "Ctrl+O"))
				{
					OpenScene();
				}

				if (ImGui::MenuItem("Save as...", "Ctrl+Shift+S"))
				{
					SaveSceneAs();
				}

				if (ImGui::MenuItem("Exit"))
				{
					Application::Get().Close();
				}

				ImGui::Separator();
				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("View"))
			{
				ImGui::MenuItem("Outliner", nullptr, &m_showOutliner);
				m_hierachyPanel->SetOutlinerWindowStatus(m_showOutliner);
				ImGui::MenuItem("Details", nullptr, &m_showDetails);
				m_hierachyPanel->SetDetailsWindowStatus(m_showDetails);
				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("Debug"))
			{
				ImGui::MenuItem("ShadowMap", nullptr, &m_showShadowMap);
				ImGui::MenuItem("BoundingBox", nullptr, &m_showBoundingBox);

				if (ImGui::BeginMenu("PolygonMode"))
				{
					static std::array<std::string, 3> s_polygonModes = { "Point", "Line", "Fill" };
					for (int i = 0; i < s_polygonModes.size(); i++)
					{
						ImGui::TableNextColumn();
						ImGui::PushID(i);
						static int32_t selectedPolygonMode = (uint32_t)m_polygonMode - 1;
						if (ImGui::RadioButton(s_polygonModes[i].c_str(), &selectedPolygonMode, i))
						{
							RendererCommand::SetPolygonMode((PolygonMode)(selectedPolygonMode + 1));
						}
						ImGui::PopID();
					}
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		if (m_showShadowMap)
		{
			if (ImGui::Begin("Shadow map", &m_showShadowMap))
			{
				m_shadowDebug.Draw();

				ImGui::InputInt("Shadow level", &m_shadowLevel);
				ImVec2 windowSize = ImGui::GetWindowSize();
				uint64_t textureId = m_shadowDebug.GetShadowLayer(m_shadowLevel);
				ImGui::Image((void*)textureId, ImVec2(windowSize.x - 15, windowSize.y - 60), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
			}

			ImGui::End();
		}

		if (m_showBoundingBox)
		{
			Configuration::SetShowBoundingBox(true);
		}
		else
		{
			Configuration::SetShowBoundingBox(false);
		}

		m_hierachyPanel->OnImGuiRender();
	}
}
