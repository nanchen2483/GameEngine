#pragma once

namespace Engine
{
	class Widget
	{
	public:
		virtual ~Widget() = default;
		virtual void OnImGuiRender() = 0;
	};
}
