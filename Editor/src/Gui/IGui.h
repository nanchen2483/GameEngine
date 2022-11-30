#pragma once

namespace Engine
{
	class IGui
	{
	public:
		virtual ~IGui() = default;
		virtual void OnImGuiRender() = 0;
	};
}
