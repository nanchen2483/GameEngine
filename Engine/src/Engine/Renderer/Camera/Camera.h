#pragma once

#include <glm/glm.hpp>

namespace Engine
{
	class Camera
	{
	public:
		virtual ~Camera() = default;
		
		virtual const glm::mat4& GetProjection() const = 0;
		virtual const float GetFOV() const = 0;
		virtual const float GetAspectRatio() const = 0;
	protected:
		glm::mat4 m_projection = glm::mat4(1.0f);
		float m_FOV = 45.0f;
		float m_aspectRatio = 1.778f;
	};
}
