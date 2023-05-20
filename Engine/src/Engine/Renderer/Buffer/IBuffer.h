#pragma once

namespace Engine
{
	class IBuffer
	{
	public:
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
	};
}
