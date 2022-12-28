#pragma once
#include "Engine/Renderer/Camera/Camera.h"

#include <map>

namespace Engine
{
	class SceneCamera : public Camera
	{
	public:
		SceneCamera();
		virtual ~SceneCamera();

		virtual const glm::mat4& GetProjection() const { return m_projection; }
		virtual const float GetFOV() const override { return m_FOV; }
		virtual const float GetAspectRatio() const override { return m_aspectRatio; }
		virtual Frustum GetFrustum(const Transform& transform) const override;
		virtual void SetViewportSize(uint32_t width, uint32_t height) override;

		void SetOrthographic(uint32_t size, float nearClip, float farClip);
		void SetPerspective(float FOV, float nearClip, float farClip);

		float GetPerspectiveFOV() const { return m_FOV; }
		void SetPerspectiveFOV(float FOV);
		float GetPerspectiveNearClip() const { return m_perspectiveNearClip; }
		void SetPerspectiveNearClip(float nearClip);
		float GetPerspectiveFarClip() const { return m_perspectiveFarClip; }
		void SetPerspectiveFarClip(float farClip);

		float GetOrthographicSize() const { return m_orthographicSize; }
		void SetOrthographicSize(float size);
		float GetOrthographicNearClip() const { return m_orthographicNear; }
		void SetOrthographicNearClip(float nearClip);
		float GetOrthographicFarClip() const { return m_orthographicFar; }
		void SetOrthographicFarClip(float farClip);

		float GetMoveSpeed() const { return m_moveSpeed; }
		void SetMoveSpeed(float speed);
		float GetRotationSpeed() const { return m_rotationSpeed; }
		void SetRotationSpeed(float speed);

		glm::vec3 GetUpDirection() const { return m_upDirection; }
		void SetUpDirection(glm::vec3 direction);

		CameraProjectionType GetProjectionType() const { return m_projectionType; }
		void SetProjectionType(CameraProjectionType type);
	private:
		void UpdateProjection();

		CameraType m_type;
		CameraProjectionType m_projectionType;

		float m_FOV;
		float m_aspectRatio;
		float m_perspectiveNearClip;
		float m_perspectiveFarClip;

		float m_orthographicSize;
		float m_orthographicNear;
		float m_orthographicFar;
		
		float m_moveSpeed;
		float m_rotationSpeed;

		glm::vec3 m_upDirection;
		glm::mat4 m_projection;
	};
}
