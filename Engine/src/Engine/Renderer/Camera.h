#pragma once

#include <glm/glm.hpp>

namespace Engine
{
	class Camera
	{
	public:
		Camera() = default;
		Camera(const glm::mat4& projectionMatrix)
			: m_projection(projectionMatrix) {}

		const glm::mat4& GetProjection() const { return m_projection; }
	protected:
		glm::mat4 m_projection = glm::mat4(1.0f);
	};
}
