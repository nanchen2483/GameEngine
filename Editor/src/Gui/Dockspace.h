#pragma once
#include <functional>

namespace Engine
{
	class Dockspace
	{
	public:
		Dockspace() = default;
		void OnImGuiRender(std::function<void(void)> callbackFunction);
	private:
		const float m_toolbarSize = 50;
	};
}
