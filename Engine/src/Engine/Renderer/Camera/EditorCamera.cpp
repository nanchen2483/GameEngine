#include "enginepch.h"
#include "EditorCamera.h"
#include "Engine/Core/Input.h"
#include "Engine/Core/Enum/KeyCodes.h"
#include "Engine/Core/Enum/MouseButtonCodes.h"

#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>


namespace Engine {

	EditorCamera::EditorCamera(float fov, float aspectRatio, float nearClip, float farClip)
		: m_nearClip(nearClip), m_farClip(farClip)
	{
		m_FOV = fov;
		m_aspectRatio = aspectRatio;
		m_projection = glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);

		UpdateView();
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
	
	glm::vec3 EditorCamera::GetUpDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
	}
	
	glm::vec3 EditorCamera::GetRightDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
	}
	
	glm::vec3 EditorCamera::GetForwardDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
	}
	
	glm::quat EditorCamera::GetOrientation() const
	{
		return glm::quat(glm::vec3(-m_pitch, -m_yaw, 0.0f));
	}
	
	void EditorCamera::UpdateProjection()
	{
		m_aspectRatio = m_viewportWidth / m_viewportHeight;
		m_projection = glm::perspective(glm::radians(m_FOV), m_aspectRatio, m_nearClip, m_farClip);
	}

	void EditorCamera::UpdateView()
	{
		m_position = CalculatePosition();
		 
		glm::quat oritation = GetOrientation();
		m_viewMatrix = glm::translate(glm::mat4(1.0f), m_position) * glm::toMat4(oritation);
		m_viewMatrix = glm::inverse(m_viewMatrix);
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
