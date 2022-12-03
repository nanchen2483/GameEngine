#pragma once
#include "IGui.h"
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
		const float m_buttonPadding = 5.0f;
		const float m_buttonSize = 20.0f;
		const float m_toolbarPadding = 7.0f;
		const float m_toolbarSize = m_buttonSize + (m_buttonPadding * 2) + (m_toolbarPadding * 2);
		
		Ptr<Texture2D> m_playIcon;
		Ptr<Texture2D> m_pauseIcon;
		Ptr<Texture2D> m_stopIcon;
	};
}
