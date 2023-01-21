#pragma once
#include "Engine/Physics/BoundingBox/BoundingBox.h"
#include "Engine/Renderer/Buffer/VertexArray.h"
#include "Engine/Renderer/Shader/Shader.h"

namespace Engine
{
	class BoundingBoxDebug
	{
	public:
		BoundingBoxDebug();

		void Draw(Transform &transform, BoundingValue boundingValue);
	private:
		Ptr<Shader> m_shader;
		Ptr<VertexArray> m_vertexArray;
	};
}