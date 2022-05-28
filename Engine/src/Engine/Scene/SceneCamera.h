#pragma once

#include "Engine/Renderer/Camera/Camera.h"

#include <map>

namespace Engine
{
	class SceneCamera : public Camera
	{
	public:
		enum class ProjectionType
		{
			Perspective = 1,
			Orthographic = 2
		};
	public:
		SceneCamera();
		virtual ~SceneCamera();

		virtual const glm::mat4& GetProjection() const { return m_projection; };
		virtual const float GetFOV() const override { return m_FOV; };
		virtual const float GetAspectRatio() const override { return m_aspectRatio; }
		virtual Frustum GetFrustum(const Transform& transform) const override;
		virtual void SetViewportSize(uint32_t width, uint32_t height) override;

		void SetOrthographic(uint32_t size, float nearClip, float farClip);
		void SetPerspective(float FOV, float nearClip, float farClip);

		float GetPerspectiveFOV() const { return m_FOV; }
		void SetPerspectiveFOV(float FOV) { m_FOV = FOV; RecalculateProjection(); }
		float GetPerspectiveNearClip() const { return m_nearClip; }
		void SetPerspectiveNearClip(float nearClip) { m_nearClip = nearClip; RecalculateProjection(); }
		float GetPerspectiveFarClip() const { return m_farClip; }
		void SetPerspectiveFarClip(float farClip) { m_farClip = farClip; RecalculateProjection(); }

		float GetOrthographicSize() const { return m_orthographicSize; }
		void SetOrthographicSize(float size) { m_orthographicSize = size; RecalculateProjection(); }
		float GetOrthographicNearClip() const { return m_orthographicNear; }
		void SetOrthographicNearClip(float nearClip) { m_orthographicNear = nearClip; RecalculateProjection(); }
		float GetOrthographicFarClip() const { return m_orthographicFar; }
		void SetOrthographicFarClip(float farClip) { m_orthographicFar = farClip; RecalculateProjection(); }

		ProjectionType GetProjectionType() { return m_projectionType; }
		std::string GetProjectionTypeString() { return m_projectionTypeMap[m_projectionType]; }
		std::map<ProjectionType, std::string> GetProjectionTypeMap() { return m_projectionTypeMap; }
		void SetProjectionType(ProjectionType type) { m_projectionType = type; RecalculateProjection(); }
	private:
		void RecalculateProjection();
		ProjectionType m_projectionType = ProjectionType::Perspective;

		float m_orthographicSize = 10.0f;
		float m_orthographicNear = -1.0f, m_orthographicFar = 1.0f;

		std::map<ProjectionType, std::string> m_projectionTypeMap =
		{
			{ ProjectionType::Perspective, "Perspective" },
			{ ProjectionType::Orthographic, "Orthographic" }
		};
	};
}
