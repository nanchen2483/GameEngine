#include "enginepch.h"
#include "SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Engine
{
	SceneCamera::SceneCamera()
	{
		RecalculateProjection();
	}

	SceneCamera::~SceneCamera()
	{
	}

	Frustum SceneCamera::GetFrustum(const Transform& transform) const
	{
		return Frustum(transform.translation, transform.rotation, m_FOV, m_nearClip, m_farClip, m_aspectRatio);
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
		m_nearClip = nearClip;
		m_farClip = farClip;

		RecalculateProjection();
	}
	
	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		ENGINE_CORE_ASSERT(height > 0, "Viewport height cannot be less or equal to 0");
		m_aspectRatio = (float)width / (float)height;
		
		RecalculateProjection();
	}
	
	void SceneCamera::RecalculateProjection()
	{
		if (m_projectionType == ProjectionType::Perspective)
		{
			m_projection = glm::perspective(glm::radians(m_FOV), m_aspectRatio, m_nearClip, m_farClip);
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
