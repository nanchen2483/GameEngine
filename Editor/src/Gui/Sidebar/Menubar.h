#pragma once
#include "Gui/IGui.h"
#include "Engine.h"

#include <functional>

namespace Engine
{
	class Menubar : public IGui
	{
	public:
		Menubar() = default;
		
		virtual void OnImGuiRender() override;
		typedef std::function<void()> Func;
		void SetFunctions(Func newScene, Func openScene, Func saveSceneAs);
	private:
		PolygonMode m_polygonMode = PolygonMode::FILL;
		ShadowDebug m_shadowDebug;
		bool m_showShadowMap = false;
		int m_shadowLevel = -1;

		std::function<void()> NewScene;
		std::function<void()> OpenScene;
		std::function<void()> SaveSceneAs;
	};
}
