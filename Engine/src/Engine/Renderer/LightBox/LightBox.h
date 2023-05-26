#pragma once
#include "Engine/Core/Base.h"
#include "Engine/Renderer/Shader/IShader.h"
#include "Engine/Renderer/VertexArray/IVertexArray.h"

namespace Engine
{
	class LightBox
	{
	public:
		LightBox();
		void Draw(glm::mat4 model, glm::vec3 color);
	private:
		Ptr<IShader> m_shader;
		Ptr<IVertexArray> m_vertexArray;
	};
}
