#pragma once
#include "Engine/Renderer/VertexArray/IVertexArray.h"

namespace Engine
{
	class VertexArrayFactory
	{
	public:
		static Ptr<IVertexArray> Create();
	};
}
