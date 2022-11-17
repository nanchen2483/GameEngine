#include "enginepch.h"
#include "SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Engine
{
	SceneCamera::SceneCamera()
		: m_type(CameraType::FreeLook), m_projectionType(ProjectionType::Perspective)
	{
		m_FOV = 45.0f;
		m_aspectRatio = 1.778f;
		m_perspectiveNearClip = 0.1f;
		m_perspectiveFarClip = 1000.0f;

		m_orthographicSize = 10.0f;
		m_orthographicNear = -1.0f;
		m_orthographicFar = 1.0f;

		RecalculateProjection();
	}

	SceneCamera::~SceneCamera()
	{
	}

	Frustum SceneCamera::GetFrustum(const Transform& transform) const
	{
		return Frustum(transform.translation, transform.rotation, m_FOV, m_perspectiveNearClip, m_perspectiveFarClip, m_aspectRatio);
	}

	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		ENGINE_CORE_ASSERT(height > 0, "Viewport height cannot be less or equal to 0");
		m_aspectRatio = (float)width / (float)height;

		RecalculateProjection();
	}

	void SceneCamera::SetOrthographic(uint32_t size, float nearClip, float farClip)
	{
		m_projectionType = ProjectionType::Orthographic;
		m_orthographicSize = (float)size;
		m_orthographicNear = nearClip;
		m_orthographicFar = farClip;

		RecalculateProjection();
	}

	void SceneCamera::SetPerspective(float FOV, float nearClip, float farClip)
	{
		m_projectionType = ProjectionType::Perspective;
		m_FOV = FOV;
		m_perspectiveNearClip = nearClip;
		m_perspectiveFarClip = farClip;

		RecalculateProjection();
	}

	void SceneCamera::SetPerspectiveFOV(float FOV)
	{
		m_FOV = FOV;
		RecalculateProjection();
	}

	void SceneCamera::SetPerspectiveNearClip(float nearClip)
	{
		m_perspectiveNearClip = nearClip;
		RecalculateProjection();
	}

	void SceneCamera::SetPerspectiveFarClip(float farClip)
	{
		m_perspectiveFarClip = farClip;
		RecalculateProjection();
	}

	void SceneCamera::SetOrthographicSize(float size)
	{
		m_orthographicSize = size;
		RecalculateProjection();
	}

	void SceneCamera::SetOrthographicNearClip(float nearClip)
	{
		m_orthographicNear = nearClip;
		RecalculateProjection();
	}

	void SceneCamera::SetOrthographicFarClip(float farClip)
	{
		m_orthographicFar = farClip;
		RecalculateProjection();
	}

	void SceneCamera::SetProjectionType(ProjectionType type)
	{
		m_projectionType = type;
		RecalculateProjection();
	}

	void SceneCamera::RecalculateProjection()
	{
		if (m_projectionType == ProjectionType::Perspective)
		{
			m_projection = glm::perspective(glm::radians(m_FOV), m_aspectRatio, m_perspectiveNearClip, m_perspectiveFarClip);
		}
		else
		{
			float orthoLeft = -m_orthographicSize * m_aspectRatio * 0.5f;
			float orthoRight = m_orthographicSize * m_aspectRatio * 0.5f;
			float orthoBottom = -m_orthographicSize * 0.5f;
			float orthoTop = m_orthographicSize * 0.5f;

			m_projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, m_orthographicNear, m_orthographicFar);
		}
	}
}
