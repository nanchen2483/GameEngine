#pragma once

#include "Camera.h"
#include "Engine/Events/Event.h"
#include "Engine/Events/MouseEvent.h"
#include "Engine/Core/TimeStep.h"

#include <glm/glm.hpp>

namespace Engine {
	class EditorCamera : public Camera
	{
	public:
		EditorCamera() = default;
		EditorCamera(float fov, float aspectRatio, float nearClip, float farClip);

		void OnUpdate(TimeStep ts);
		void OnEvent(Event& e);

		virtual const glm::mat4& GetProjection() const { return m_projection; };
		virtual const float GetFOV() const override { return m_FOV; };
		virtual const float GetAspectRatio() const override { return m_aspectRatio; }

		inline float GetDistance() const { return m_distance; }
		inline float SetDistance(float distance) { m_distance = distance; }

		inline void SetViewportSize(float width, float height) { m_viewportWidth = width; m_viewportHeight = height; }

		const glm::mat4& GetViewMatrix() const { return m_viewMatrix; }
		glm::mat4 GetViewProjection() const { return m_projection * m_viewMatrix; }

		glm::vec3 GetUpDirection() const;
		glm::vec3 GetRightDirection() const;
		glm::vec3 GetForwardDirection() const;
		const glm::vec3& GetPosition() const { return m_position; }
		glm::quat GetOrientation() const;

		float GetPitch() const { return m_pitch; }
		float GetYaw() const { return m_yaw; }
	private:
		void UpdateProjection();
		void UpdateView();
		bool OnMouseScroll(MouseScrolledEvent& e);

		void MousePan(const glm::vec2& data);
		void MouseRotate(const glm::vec2& delta);
		void MouseZoom(float delta);

		glm::vec3 CalculatePosition() const;
		std::pair<float, float> PanSpeed() const;
		float RotationSpeed() const;
		float ZoomSpeed() const;
		
		float m_nearClip = 0.1f, m_farClip = 1000.0f;
		float m_distance = 10.0f;
		float m_pitch = 0.0f, m_yaw = 0.0f;
		
		float m_viewportWidth = 1280, m_viewportHeight = 720;

		glm::vec2 m_initialMousePosition = glm::vec2(0.0f);
		glm::vec3 m_position = glm::vec3(0.0f);
		glm::vec3 m_focusPoint = glm::vec3(0.0f);
		glm::mat4 m_viewMatrix = glm::mat4(1.0f);
	};
}
