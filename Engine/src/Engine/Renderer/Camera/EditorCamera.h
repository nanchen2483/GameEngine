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

		inline const float GetDistance() const { return m_distanceToFocusPoint; }
		inline void SetDistance(float distance) { m_distanceToFocusPoint = distance; }

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
		void OnFocusPointUpdate(const glm::vec2& delta);
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

		CameraType m_type;
		CameraProjectionType m_projectionType;

		float m_FOV;
		float m_nearClip;
		float m_farClip;
		float m_aspectRatio;
		float m_viewportWidth;
		float m_viewportHeight;

		float m_pitch;
		float m_yaw;
		float m_moveSpeed;
		float m_rotationSpeed;
		float m_distanceToFocusPoint;

		glm::vec3 m_rotation;
		glm::quat m_orientation;

		glm::vec3 m_upDirection;
		glm::vec3 m_rightDirection;
		glm::vec3 m_forwardDirection;

		glm::vec2 m_mousePosition;
		glm::vec3 m_position;
		glm::vec3 m_focusPoint;
		glm::mat4 m_viewMatrix;
		glm::mat4 m_projection;
		glm::mat4 m_viewProjection;
	};
}
