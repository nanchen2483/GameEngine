#include "enginepch.h"
#include "SceneCamera.h"
#include "Engine/Core/Constant/CameraConstant.h"
#include "Engine/Core/Window/Input.h"
#include "Engine/Core/Enum/KeyCodes.h"
#include "Engine/Core/Enum/MouseButtonCodes.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Engine
{
	SceneCamera::SceneCamera()
		: m_type(CameraType::FreeLook), m_projectionType(CameraProjectionType::Perspective)
	{
		m_FOV = CameraConstant::FOV;
		m_aspectRatio = 1.778f;
		m_perspectiveNearClip = CameraConstant::NearPlane;
		m_perspectiveFarClip = CameraConstant::FarPlane;

		m_orthographicSize = 10.0f;
		m_orthographicNear = -1.0f;
		m_orthographicFar = 1.0f;

		m_moveSpeed = CameraConstant::MoveSpeed;
		m_rotationSpeed = CameraConstant::RotationSpeed;
		m_upDirection = glm::vec3(0.0f);

		UpdateProjection();
	}

	SceneCamera::~SceneCamera()
	{
	}

	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		ENGINE_CORE_ASSERT(height > 0, "Viewport height cannot be less or equal to 0");
		m_aspectRatio = (float)width / (float)height;

		UpdateProjection();
	}

	void SceneCamera::SetOrthographic(uint32_t size, float nearClip, float farClip)
	{
		m_projectionType = CameraProjectionType::Orthographic;
		m_orthographicSize = (float)size;
		m_orthographicNear = nearClip;
		m_orthographicFar = farClip;

		UpdateProjection();
	}

	void SceneCamera::SetPerspective(float FOV, float nearClip, float farClip)
	{
		m_projectionType = CameraProjectionType::Perspective;
		m_FOV = FOV;
		m_perspectiveNearClip = nearClip;
		m_perspectiveFarClip = farClip;

		UpdateProjection();
	}

	void SceneCamera::SetPerspectiveFOV(float FOV)
	{
		m_FOV = FOV;
		UpdateProjection();
	}

	void SceneCamera::SetPerspectiveNearClip(float nearClip)
	{
		m_perspectiveNearClip = nearClip;
		UpdateProjection();
	}

	void SceneCamera::SetPerspectiveFarClip(float farClip)
	{
		m_perspectiveFarClip = farClip;
		UpdateProjection();
	}

	void SceneCamera::SetOrthographicSize(float size)
	{
		m_orthographicSize = size;
		UpdateProjection();
	}

	void SceneCamera::SetOrthographicNearClip(float nearClip)
	{
		m_orthographicNear = nearClip;
		UpdateProjection();
	}

	void SceneCamera::SetOrthographicFarClip(float farClip)
	{
		m_orthographicFar = farClip;
		UpdateProjection();
	}

	void SceneCamera::SetMoveSpeed(float speed)
	{
		m_moveSpeed = speed;
	}

	void SceneCamera::SetRotationSpeed(float speed)
	{
		m_rotationSpeed = speed;
	}

	void SceneCamera::SetUpDirection(glm::vec3 direction)
	{
		m_upDirection = direction;
	}

	void SceneCamera::SetProjectionType(CameraProjectionType type)
	{
		m_projectionType = type;
		UpdateProjection();
	}

	void SceneCamera::UpdateProjection()
	{
		if (m_projectionType == CameraProjectionType::Perspective)
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
