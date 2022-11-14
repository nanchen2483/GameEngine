#pragma once
#include "Widget.h"
#include "Engine.h"

namespace Engine
{
	class Toolbar : public Widget
	{
	public:
		Toolbar();

		virtual void OnImGuiRender() override;
		bool IsPlaying() const { return m_play; }
	private:
		bool m_play = false;
		bool m_pause = false;

		Ptr<Texture2D> m_playIcon;
		Ptr<Texture2D> m_pauseIcon;
		Ptr<Texture2D> m_stopIcon;
	};
}
