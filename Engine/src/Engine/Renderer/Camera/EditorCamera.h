#pragma once
#include "Camera.h"
#include "Engine/Core/Events/Event.h"
#include "Engine/Core/Events/MouseEvent.h"
#include "Engine/Core/TimeStep.h"

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

		inline const float GetDistance() const { return m_distance; }
		inline void SetDistance(float distance) { m_distance = distance; }

		inline float GetRotationSpeed() const { return m_rotationSpeed; }
		inline float GetMoveSpeed() const { return m_moveSpeed; }

		inline const glm::mat4& GetViewMatrix() const { return m_viewMatrix; }
		inline const glm::mat4& GetViewProjection() const { return m_viewProjection; }

		inline const glm::vec3& GetPosition() const { return m_position; }
		inline const glm::vec3& GetRotation() const { return m_rotation; }
		inline const glm::quat& GetOrientation() const { return m_orientation; }
		inline const glm::vec3& GetUpDirection() const { return m_upDirection; }
		inline const glm::vec3& GetRightDirection() const { return m_rightDirection; }
		inline const glm::vec3& GetForwardDirection() const { return m_forwardDirection; }

		inline const float GetPitch() const { return m_pitch; }
		inline const float GetYaw() const { return m_yaw; }

		void UpdateFocusPoint(const Transform& transform);
		bool IsCursorInsideViewport() const;
		inline bool InUse() const { return m_type != CameraType::None; }
	private:
		void OnFixPointUpdate(const glm::vec2& delta);
		void OnFreeLookUpdate(TimeStep deltaTime, const glm::vec2& delta);
		bool OnMouseScroll(MouseScrolledEvent& e);
		void OnMousePan(const glm::vec2& data);
		void OnMouseRotate(const glm::vec2& delta);
		void OnMouseZoom(float delta);
		std::pair<float, float> GetPanSpeed() const;
		float GetZoomSpeed() const;

		void UpdateRotation();
		void UpdatePosition(bool forceUpdate = false);
		void UpdateProjection();
		void UpdateView();
		void UpdateViewProjection();
	private:
		enum class CameraType
		{
			None = 0,
			FixPoint,
			FreeLook,
		} m_type = CameraType::None;
		
		float m_distance = 10.0f;
		float m_rotationSpeed = 0.8f, m_moveSpeed = 20.0f;

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
