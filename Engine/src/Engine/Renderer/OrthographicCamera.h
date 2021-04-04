#pragma once

#include <glm/glm.hpp>

namespace Engine
{
	class OrthographicCamera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);

		void SetProjectionMatrix(float left, float right, float bottom, float top);
		
		const glm::vec3& GetPosition() const { return m_position; }
		void SetPosition(const glm::vec3& position) { m_position = position; RecalculateViewMatrix(); }

		float GetRotation() { return m_rotation; };
		void SetRotation(float rotation) { m_rotation = rotation; RecalculateViewMatrix(); }

		const glm::mat4& GetViewMatrix() { return m_viewMatrix; }
		const glm::mat4& GetProjectionMatrix() { return m_projectionMatrix; }
		const glm::mat4& GetViewProjectionMatrix() { return m_viewProjectionMatrix; }
	private:
		void RecalculateViewMatrix();
	private:
		glm::mat4 m_viewMatrix;
		glm::mat4 m_projectionMatrix;
		glm::mat4 m_viewProjectionMatrix;

		glm::vec3 m_position;
		float m_rotation = 0.0f;
	};
}