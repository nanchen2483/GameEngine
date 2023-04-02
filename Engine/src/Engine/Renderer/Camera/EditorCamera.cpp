#include "enginepch.h"
#include "EditorCamera.h"

#include "Engine/Core/Enum/KeyCodes.h"
#include "Engine/Core/Enum/MouseButtonCodes.h"
#include "Engine/Core/System/System.h"
#include "Engine/Core/Window/Input.h"

#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Engine
{
	EditorCamera::EditorCamera()
		: EditorCamera(45.0f, 1280.0f, 720.0f, 0.1f, 1000.0f)
	{
	}

	EditorCamera::EditorCamera(float fov, float viewportWidth, float viewportHeight, float nearClip, float farClip)
		: m_FOV(fov), m_nearClip(nearClip), m_farClip(farClip), m_viewportWidth(viewportWidth), m_viewportHeight(viewportHeight)
	{
		m_type = CameraType::None;
		m_projectionType = CameraProjectionType::Perspective;

		m_pitch = 0.0f;
		m_yaw = 0.0f;
		m_moveSpeed = 20.0f;
		m_rotationSpeed = 0.8f;
		m_distanceToFocusPoint = 10.0f;

		m_rotation = glm::vec3(0.0f);
		m_orientation = glm::quat();

		m_upDirection = glm::vec3(0.0f);
		m_rightDirection = glm::vec3(0.0f);
		m_forwardDirection = glm::vec3(0.0f);

		m_mousePosition = glm::vec2(0.0f);
		m_position = glm::vec3(0.0f);
		m_focusPoint = glm::vec3(0.0f);
		m_viewMatrix = glm::mat4(1.0f);
		m_projection = glm::mat4(1.0f);
		m_viewProjection = glm::mat4(1.0f);

		UpdateRotation();
		UpdatePosition(true);
		UpdateProjection();
	}

	void EditorCamera::OnUpdate()
	{
		const glm::vec2& mouse{ Input::GetMouseX(), Input::GetMouseY() };
		glm::vec2 delta = (mouse - m_mousePosition) * 0.003f;
		m_mousePosition = mouse;

		if (Input::IsKeyPressed(KeyCode::LEFT_ALT) &&
			(Input::IsMouseButtonPressed(MouseButton::MIDDLE_BUTTON) ||
			 Input::IsMouseButtonPressed(MouseButton::LEFT_BUTTON) ||
			 Input::IsMouseButtonPressed(MouseButton::RIGHT_BUTTON)))
		{
			m_type = CameraType::FocusPoint;
			Input::HideCursor();
			OnFocusPointUpdate(delta);
		}
		else if (Input::IsMouseButtonPressed(MouseButton::RIGHT_BUTTON))
		{
			m_type = CameraType::FreeLook;
			Input::HideCursor();
			OnFreeLookUpdate(delta);
		}
		else
		{
			m_type = CameraType::None;
			Input::ShowCursor();
			UpdatePosition();
		}
	}

	void EditorCamera::OnFocusPointUpdate(const glm::vec2& delta)
	{
		if (Input::IsMouseButtonPressed(MouseButton::MIDDLE_BUTTON))
		{
			OnMousePan(delta);
		}
		else if (Input::IsMouseButtonPressed(MouseButton::LEFT_BUTTON))
		{
			OnMouseRotate(delta);
		}
		else if (Input::IsMouseButtonPressed(MouseButton::RIGHT_BUTTON))
		{
			OnMouseZoom(delta.y);
		}
	}

	void EditorCamera::OnFreeLookUpdate(const glm::vec2& delta)
	{
		float velocity = m_moveSpeed * System::GetDeltaTime();
		if (Input::IsKeyPressed(KeyCode::W))
		{
			m_position += m_forwardDirection * velocity;
		}

		if (Input::IsKeyPressed(KeyCode::S))
		{
			m_position -= m_forwardDirection * velocity;
		}

		if (Input::IsKeyPressed(KeyCode::A))
		{
			m_position -= m_rightDirection * velocity;
		}

		if (Input::IsKeyPressed(KeyCode::D))
		{
			m_position += m_rightDirection * velocity;
		}

		if (Input::IsKeyPressed(KeyCode::E))
		{
			m_position += m_upDirection * velocity;
		}

		if (Input::IsKeyPressed(KeyCode::Q))
		{
			m_position -= m_upDirection * velocity;
		}

		m_focusPoint = m_position + m_forwardDirection * m_distanceToFocusPoint;

		OnMouseRotate(delta);
	}

	bool EditorCamera::OnMouseScroll(MouseScrolledEvent& e)
	{
		float delta = e.GetYOffset() * 0.1f;
		OnMouseZoom(delta);
		UpdateView();

		return false;
	}

	void EditorCamera::OnMousePan(const glm::vec2& delta)
	{
		auto [xSpeed, ySpeed] = GetPanSpeed();
		m_focusPoint -= m_rightDirection * delta.x * xSpeed * m_distanceToFocusPoint;
		m_focusPoint += m_upDirection * delta.y * ySpeed * m_distanceToFocusPoint;

		UpdatePosition();
	}

	void EditorCamera::OnMouseRotate(const glm::vec2& delta)
	{
		float yawSign = m_upDirection.y < 0 ? -1.0f : 1.0f;
		m_yaw += delta.x * m_rotationSpeed * yawSign;
		m_pitch += delta.y * m_rotationSpeed;

		UpdateRotation();
		UpdatePosition();
	}

	void EditorCamera::OnMouseZoom(float delta)
	{
		m_distanceToFocusPoint -= delta * GetZoomSpeed();
		if (m_distanceToFocusPoint < 1.0f)
		{
			m_focusPoint += m_forwardDirection;
			m_distanceToFocusPoint = 1.0f;
		}

		UpdatePosition();
	}

	std::pair<float, float> EditorCamera::GetPanSpeed() const
	{
		float x = std::min(m_viewportWidth / 1000.0f, 2.4f);
		float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

		float y = std::min(m_viewportHeight / 1000.0f, 2.4f);
		float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

		return { xFactor, yFactor };
	}

	float EditorCamera::GetZoomSpeed() const
	{
		float distance = m_distanceToFocusPoint * 0.25f;
		distance = std::max(distance, 0.0f);
		float speed = distance * distance;
		speed = std::min(speed, 100.0f);

		return speed;
	}
	
	void EditorCamera::OnEvent(Event& e)
	{
		EventDispatcher dispacther(e);
		dispacther.Dispatch<MouseScrolledEvent>(ENGINE_BIND_FN(EditorCamera::OnMouseScroll));
	}

	Frustum EditorCamera::GetFrustum() const
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

	void EditorCamera::UpdateFocusPoint(const Transform& transform)
	{
		m_type = CameraType::FocusPoint;
		m_focusPoint = transform.translation;
		m_distanceToFocusPoint = 10.0f;

		UpdatePosition();
	}

	bool EditorCamera::IsCursorInsideViewport() const
	{
		return !Input::IsCursorVisible();
	}
	
	void EditorCamera::UpdateRotation()
	{
		m_rotation = glm::vec3(-m_pitch, -m_yaw, 0.0f);
		m_orientation = glm::quat(m_rotation);

		m_upDirection = glm::rotate(m_orientation, glm::vec3(0.0f, 1.0f, 0.0f));
		m_rightDirection = glm::rotate(m_orientation, glm::vec3(1.0f, 0.0f, 0.0f));
		m_forwardDirection = glm::rotate(m_orientation, glm::vec3(0.0f, 0.0f, -1.0f));
	}

	void EditorCamera::UpdatePosition(bool forceUpdate)
	{
		if (m_type == CameraType::FocusPoint || forceUpdate)
		{
			m_position = m_focusPoint - m_forwardDirection * m_distanceToFocusPoint;
		}
		
		UpdateView();
	}

	void EditorCamera::UpdateProjection()
	{
		m_aspectRatio = m_viewportWidth / m_viewportHeight;
		m_projection = glm::perspective(glm::radians(m_FOV), m_aspectRatio, m_nearClip, m_farClip);
		
		UpdateViewProjection();
	}

	void EditorCamera::UpdateView()
	{
		m_viewMatrix = glm::translate(glm::mat4(1.0f), m_position) * glm::toMat4(m_orientation);
		m_viewMatrix = glm::inverse(m_viewMatrix);
		
		UpdateViewProjection();
	}

	void EditorCamera::UpdateViewProjection()
	{
 		m_viewProjection = m_projection * m_viewMatrix;
	}
}
