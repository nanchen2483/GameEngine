#pragma once
#include "Engine/Renderer/Buffer/VertexArray.h"
#include "Engine/Renderer/Model/Vertex.h"
#include "Engine/Renderer/Material/Material.h"

namespace Engine
{
	class Mesh
	{
	public:
		virtual Ptr<VertexArray> GetVertexArray() = 0;
		virtual Ptr<Material> GetMaterial() = 0;
	};
}
