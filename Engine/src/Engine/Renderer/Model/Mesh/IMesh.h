#pragma once
#include "Engine/Renderer/VertexArray/VertexArray.h"
#include "Engine/Renderer/Vertex/Vertex.h"
#include "Engine/Renderer/Material/Material.h"

namespace Engine
{
	class IMesh
	{
	public:
		virtual Ptr<VertexArray> GetVertexArray() = 0;
		virtual Ptr<Material> GetMaterial() = 0;
	};
}
