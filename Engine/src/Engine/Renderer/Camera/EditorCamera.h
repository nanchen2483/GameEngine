#pragma once
#include "Camera.h"
#include "Engine/Core/TimeStep.h"
#include "Engine/Events/Event.h"
#include "Engine/Events/MouseEvent.h"

#include <glm/glm.hpp>

namespace Engine
{
	class EditorCamera : public Camera
	{
	public:
		EditorCamera();
		EditorCamera(float fov, float viewportWidth, float viewportHeight, float nearClip, float farClip);

		void OnUpdate(TimeStep ts);
		void OnEvent(Event& e);

		virtual const glm::mat4& GetProjection() const { return m_projection; };
		virtual const float GetFOV() const override { return m_FOV; };
		virtual const float GetAspectRatio() const override { return m_aspectRatio; }
		virtual Frustum GetFrustum(const Transform& transform = {}) const override;
		virtual void SetViewportSize(uint32_t width, uint32_t height) override;

		inline float GetDistance() const { return m_distance; }
		inline float SetDistance(float distance) { m_distance = distance; }

		inline const glm::mat4& GetViewMatrix() const { return m_viewMatrix; }
		inline const glm::mat4& GetViewProjection() const { return m_viewProjection; }

		inline const glm::vec3& GetPosition() const { return m_position; }
		inline const glm::vec3& GetRotation() const { return m_rotation; }
		inline const glm::quat& GetOrientation() const { return m_orientation; }
		inline const glm::vec3& GetUpDirection() const { return m_upDirection; }
		inline const glm::vec3& GetRightDirection() const { return m_rightDirection; }
		inline const glm::vec3& GetForwardDirection() const { return m_forwardDirection; }

		inline float GetPitch() const { return m_pitch; }
		inline float GetYaw() const { return m_yaw; }
	private:
		void UpdateRotation();
		void UpdateProjection();
		void UpdateView();
		void UpdateViewProjection();
		bool OnMouseScroll(MouseScrolledEvent& e);

		void MousePan(const glm::vec2& data);
		void MouseRotate(const glm::vec2& delta);
		void MouseZoom(float delta);

		glm::vec3 CalculatePosition() const;
		std::pair<float, float> PanSpeed() const;
		float RotationSpeed() const;
		float ZoomSpeed() const;
		
		float m_distance = 10.0f;
		float m_pitch = 0.0f, m_yaw = 0.0f;
		glm::vec3 m_rotation = glm::vec3(0.0f);
		glm::quat m_orientation = glm::quat();
		
		float m_viewportWidth = 1280, m_viewportHeight = 720;

		glm::vec3 m_upDirection;
		glm::vec3 m_rightDirection;
		glm::vec3 m_forwardDirection;

		glm::vec2 m_initialMousePosition = glm::vec2(0.0f);
		glm::vec3 m_position = glm::vec3(0.0f);
		glm::vec3 m_focusPoint = glm::vec3(0.0f);
		glm::mat4 m_viewMatrix = glm::mat4(1.0f);
		glm::mat4 m_viewProjection = glm::mat4(1.0f);
	};
}
