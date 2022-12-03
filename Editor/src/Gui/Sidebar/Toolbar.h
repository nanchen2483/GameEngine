#pragma once
#include "Gui/IGui.h"
#include "Engine.h"

namespace Engine
{
	class Toolbar : public IGui
	{
	public:
		Toolbar();

		virtual void OnImGuiRender() override;
		bool IsPlaying() const { return m_play; }
	private:
		void ImageButton(Ptr<Texture2D> texture, bool isDisabled, std::function<void(void)> OnClick);

		bool m_play = false;
		bool m_pause = false;
		const glm::vec2 m_buttonPadding = { 5.0f, 5.0f };
		const glm::vec2 m_buttonSize = { 20.0f, 20.0f };
		const glm::vec2 m_buttonSizeWithPadding = m_buttonSize + (m_buttonPadding * glm::vec2(2));
		const glm::vec2 m_toolbarPadding = { 7.0f, 7.0f };
		const float m_toolbarHeight = m_buttonSizeWithPadding.y + (m_toolbarPadding.y * 2);
		
		Ptr<Texture2D> m_playIcon;
		Ptr<Texture2D> m_pauseIcon;
		Ptr<Texture2D> m_stopIcon;
	};
}
