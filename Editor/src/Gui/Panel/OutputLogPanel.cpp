#include "OutputLogPanel.h"

#include <imgui.h>
#include <IconFontCppHeaders/IconsFontAwesome6.h>

namespace Engine
{
	OutputLogPanel::OutputLogPanel()
	{
		m_logLevelFilter = std::set<LogLevel>
		{
			LogLevel::Trace,
			LogLevel::Debug,
			LogLevel::Info,
			LogLevel::Warn,
			LogLevel::Error
		};
	}

	void OutputLogPanel::OnImGuiRender()
	{
		if (m_open)
		{
			if (ImGui::Begin(ICON_FA_TERMINAL " Output Log", &m_open))
			{
				m_logMessageFilter.Draw("##Filter");
				ImGui::SameLine();
				if (ImGui::Button(ICON_FA_FILTER " Filter"))
				{
					ImGui::OpenPopup("LogLevelsPopup");
				}
				if (ImGui::BeginPopup("LogLevelsPopup"))
				{
					DrawCheckbox("Trace", &m_showTrace, LogLevel::Trace);
					DrawCheckbox("Debug", &m_showDebug, LogLevel::Debug);
					DrawCheckbox("Info", &m_showInfo, LogLevel::Info);
					DrawCheckbox("Warn", &m_showWarn, LogLevel::Warn);
					DrawCheckbox("Error", &m_showError, LogLevel::Error);
					ImGui::EndPopup();
				}
				ImGui::Separator();
				if (ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar))
				{
					ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 1));
					const std::vector<LogMessage>& messages = Log::GetMessages();
					for (LogMessage message : messages)
					{
						if ((!m_logMessageFilter.IsActive() ||
							m_logMessageFilter.PassFilter(message.payload.c_str(),
								message.payload.c_str() + message.payload.size())) &&
							std::find(m_logLevelFilter.begin(), m_logLevelFilter.end(), message.level) != m_logLevelFilter.end())
						{
							ImGui::BeginGroup();
							ImGui::PushStyleColor(ImGuiCol_Text, GetColorByLogLevel(message.level));
							ImGui::TextUnformatted(message.payload.c_str());
							ImGui::PopStyleColor();
							ImGui::EndGroup();
						}
					}
					ImGui::PopStyleVar();
					if (m_messageSize != messages.size())
					{
						ImGui::SetScrollHereY(1.0F);
						m_messageSize = messages.size();
					}
				}
				ImGui::EndChild();
			}
			ImGui::End();
		}
	}

	void OutputLogPanel::DrawCheckbox(const char* label, bool *checked, LogLevel level)
	{
		ImGui::Checkbox(label, checked);
		auto it = m_logLevelFilter.find(level);
		if (*checked)
		{
			if (it == m_logLevelFilter.end())
			{
				m_logLevelFilter.insert(level);
			}
		}
		else
		{
			if (it != m_logLevelFilter.end())
			{
				m_logLevelFilter.erase(it);
			}
		}
	}

	ImVec4 OutputLogPanel::GetColorByLogLevel(LogLevel level)
	{
		switch (level)
		{
		case LogLevel::Trace:
			return ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
		case LogLevel::Debug:
			return ImVec4(0.22f, 0.58f, 0.86f, 1.0f);
		case LogLevel::Info:
			return ImVec4(0.07f, 0.63f, 0.05f, 1.0f);
		case LogLevel::Warn:
			return ImVec4(0.97f, 0.94f, 0.64f, 1.0f);
		case LogLevel::Error:
		default:
			return ImVec4(0.9f, 0.28f, 0.33f, 1.0f);
		}
	}
}
