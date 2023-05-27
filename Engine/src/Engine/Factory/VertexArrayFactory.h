#pragma once
#include "Engine/Renderer/VertexArray/IVertexArray.h"

namespace Engine
{
	class ENGINE_API VertexArrayFactory
	{
	public:
		static Ptr<IVertexArray> Create();
	};
}
