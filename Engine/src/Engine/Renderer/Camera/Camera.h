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
		const float GetFOV() const { return m_FOV; }
		const float GetAspectRatio() const { return m_aspectRatio; }
	protected:
		glm::mat4 m_projection = glm::mat4(1.0f);
		float m_FOV = 45.0f;
		float m_aspectRatio = 1.778f;
	};
}
