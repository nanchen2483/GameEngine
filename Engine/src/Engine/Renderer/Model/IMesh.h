#pragma once
#include "Engine/Renderer/Buffer/IVertexArray.h"
#include "Engine/Renderer/Model/Vertex.h"
#include "Engine/Renderer/Material/Material.h"

namespace Engine
{
	class IMesh
	{
	public:
		virtual Ptr<IVertexArray> GetVertexArray() = 0;
		virtual Ptr<Material> GetMaterial() = 0;
	};
}
