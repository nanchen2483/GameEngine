#pragma once
#include "Gui/IGui.h"
#include "Engine/Core/Log/Log.h"

#include <imgui_internal.h>
#include <set>

namespace Engine
{
	class OutputLogPanel : public IGui
	{
	public:
		OutputLogPanel();
		void ToggleWindowStatus() { m_open = !m_open; }
		virtual void OnImGuiRender() override;
	private:
		void DrawCheckbox(const char * label, bool *checked, LogLevel level);
		ImVec4 GetColorByLogLevel(LogLevel level);

		bool m_open = false;
		uint32_t m_messageSize = 0;
		ImGuiTextFilter m_logMessageFilter;
		std::set<LogLevel> m_logLevelFilter;

		bool m_showTrace = true;
		bool m_showDebug = true;
		bool m_showInfo = true;
		bool m_showWarn = true;
		bool m_showError = true;
	};
}
