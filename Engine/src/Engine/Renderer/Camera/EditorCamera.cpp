#include "enginepch.h"
#include "EditorCamera.h"

#include "Engine/Core/Enum/KeyCodes.h"
#include "Engine/Core/Enum/MouseButtonCodes.h"
#include "Engine/Core/Window/Input.h"

#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Engine
{
	EditorCamera::EditorCamera()
	{
		UpdateRotation();
		UpdateProjection();
		UpdateView();
	}

	EditorCamera::EditorCamera(float fov, float viewportWidth, float viewportHeight, float nearClip, float farClip)
		: EditorCamera()
	{
		m_FOV = fov;
		m_nearClip = nearClip;
		m_farClip = farClip;
		m_viewportWidth = viewportWidth;
		m_viewportHeight = viewportHeight;
	}

	void EditorCamera::OnUpdate(TimeStep ts)
	{
		if (Input::IsKeyPressed(KeyCode::LEFT_ALT))
		{
			const glm::vec2& mouse{ Input::GetMouseX(), Input::GetMouseY() };
			glm::vec2 delta = (mouse - m_initialMousePosition) * 0.003f;
			m_initialMousePosition = mouse;

			if (Input::IsMouseButtonPressed(MouseButton::MIDDLE_BUTTON))
			{
				MousePan(delta);
			}
			else if (Input::IsMouseButtonPressed(MouseButton::LEFT_BUTTON))
			{
				MouseRotate(delta);
			}
			else if (Input::IsMouseButtonPressed(MouseButton::RIGHT_BUTTON))
			{
				MouseZoom(delta.y);
			}
		}

		UpdateView();
	}
	
	void EditorCamera::OnEvent(Event& e)
	{
		EventDispatcher dispacther(e);
		dispacther.Dispatch<MouseScrolledEvent>(ENGINE_BIND_EVENT_FN(EditorCamera::OnMouseScroll));
	}

	Frustum EditorCamera::GetFrustum(const Transform& transform) const
	{
		return Frustum(m_position, m_rotation, m_FOV, m_nearClip, m_farClip, m_aspectRatio);
	}

	void EditorCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		ENGINE_CORE_ASSERT(height > 0, "Viewport height cannot be less or equal to 0");

		m_viewportWidth = (float)width;
		m_viewportHeight = (float)height;
		UpdateProjection();
	}
	
	void EditorCamera::UpdateRotation()
	{
		m_rotation = glm::vec3(-m_pitch, -m_yaw, 0.0f);
		m_orientation = glm::quat(m_rotation);

		m_upDirection = glm::rotate(m_orientation, glm::vec3(0.0f, 1.0f, 0.0f));
		m_rightDirection = glm::rotate(m_orientation, glm::vec3(1.0f, 0.0f, 0.0f));
		m_forwardDirection = glm::rotate(m_orientation, glm::vec3(0.0f, 0.0f, -1.0f));
	}

	void EditorCamera::UpdateProjection()
	{
		m_aspectRatio = m_viewportWidth / m_viewportHeight;
		m_projection = glm::perspective(glm::radians(m_FOV), m_aspectRatio, m_nearClip, m_farClip);
		UpdateViewProjection();
	}

	void EditorCamera::UpdateView()
	{
		m_position = CalculatePosition();
		 
		m_viewMatrix = glm::translate(glm::mat4(1.0f), m_position) * glm::toMat4(m_orientation);
		m_viewMatrix = glm::inverse(m_viewMatrix);
		UpdateViewProjection();
	}

	void EditorCamera::UpdateViewProjection()
	{
 		m_viewProjection = m_projection * m_viewMatrix;
	}
	
	bool EditorCamera::OnMouseScroll(MouseScrolledEvent& e)
	{
		float delta = e.GetYOffset() * 0.1f;
		MouseZoom(delta);
		UpdateView();
		return false;
	}
	
	void EditorCamera::MousePan(const glm::vec2& delta)
	{
		auto [xSpeed, ySpeed] = PanSpeed();
		m_focusPoint -= GetRightDirection() * delta.x * xSpeed * m_distance;
		m_focusPoint += GetUpDirection() * delta.y * ySpeed * m_distance;
	}
	
	void EditorCamera::MouseRotate(const glm::vec2& delta)
	{
		float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
		m_yaw += yawSign * delta.x * RotationSpeed();
		m_pitch += delta.y * RotationSpeed();

		UpdateRotation();
	}
	
	void EditorCamera::MouseZoom(float delta)
	{
		m_distance -= delta * ZoomSpeed();
		if (m_distance < 1.0f)
		{
			m_focusPoint += GetForwardDirection();
			m_distance = 1.0f;
		}
	}
	
	glm::vec3 EditorCamera::CalculatePosition() const
	{
		return m_focusPoint - GetForwardDirection() * m_distance;
	}
	
	std::pair<float, float> EditorCamera::PanSpeed() const
	{
		float x = std::min(m_viewportWidth / 1000.0f, 2.4f);
		float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

		float y = std::min(m_viewportHeight / 1000.0f, 2.4f);
		float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

		return { xFactor, yFactor };
	}
	
	float EditorCamera::RotationSpeed() const
	{
		return 0.8f;
	}
	
	float EditorCamera::ZoomSpeed() const
	{
		float distance = m_distance * 0.25f;
		distance = std::max(distance, 0.0f);
		float speed = distance * distance;
		speed = std::min(speed, 100.0f);
		return speed;
	}
}
