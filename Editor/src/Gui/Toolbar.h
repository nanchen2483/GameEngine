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
		float m_thumbnailSize = 32.0f;
		
		Ptr<Texture2D> m_playIcon;
		Ptr<Texture2D> m_pauseIcon;
		Ptr<Texture2D> m_stopIcon;
	};
}
