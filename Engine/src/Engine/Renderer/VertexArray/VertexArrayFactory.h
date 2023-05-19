#pragma once
#include "IVertexArray.h"

namespace Engine
{
	class VertexArrayFactory
	{
	public:
		static Ptr<IVertexArray> Create();
	};
}
