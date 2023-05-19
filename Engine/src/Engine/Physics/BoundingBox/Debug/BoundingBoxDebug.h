#pragma once
#include "Engine/Physics/BoundingBox/IBoundingBox.h"
#include "Engine/Renderer/VertexArray/VertexArray.h"
#include "Engine/Renderer/Shader/IShader.h"

namespace Engine
{
	class BoundingBoxDebug
	{
	public:
		BoundingBoxDebug();

		void Draw(Transform &transform, BoundingValue boundingValue);
	private:
		Ptr<IShader> m_shader;
		Ptr<VertexArray> m_vertexArray;
	};
}
