#include "enginepch.h"
#include "OrthographicCameraController.h"

#include "Engine/Core/Enum/KeyCodes.h"
#include "Engine/Core/Window/Input.h"

namespace Engine
{
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		: m_aspectRatio(aspectRatio), m_camera(-m_aspectRatio * m_zoomLevel, m_aspectRatio* m_zoomLevel, -m_zoomLevel, m_zoomLevel), m_rotation(rotation)
	{
	}

	void OrthographicCameraController::OnUpdate(TimeStep timeStep)
	{
		ENGINE_PROFILE_FUNCTION();

		if (Input::IsKeyPressed(KeyCode::A))
		{
			m_cameraPosition.x -= m_cameraMoveSpeed * timeStep;
		}
		else if (Input::IsKeyPressed(KeyCode::D))
		{
			m_cameraPosition.x += m_cameraMoveSpeed * timeStep;
		}

		if (Input::IsKeyPressed(KeyCode::W))
		{
			m_cameraPosition.y += m_cameraMoveSpeed * timeStep;
		}
		else if (Input::IsKeyPressed(KeyCode::S))
		{
			m_cameraPosition.y -= m_cameraMoveSpeed * timeStep;
		}

		if (m_rotation)
		{
			if (Input::IsKeyPressed(KeyCode::Q))
			{
				m_cameraRotation += m_cameraRotationSpeed * timeStep;
			}
			else if (Input::IsKeyPressed(KeyCode::E))
			{
				m_cameraRotation -= m_cameraRotationSpeed * timeStep;
			}

			m_camera.SetRotation(m_cameraRotation);
		}

		m_camera.SetPosition(m_cameraPosition);
		m_cameraMoveSpeed = m_zoomLevel;
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		ENGINE_PROFILE_FUNCTION();

		EventDispatcher dispacher(e);
		dispacher.Dispatch<MouseScrolledEvent>(ENGINE_BIND_FN(OrthographicCameraController::OmMouseScrolled));
		dispacher.Dispatch<WindowResizeEvent>(ENGINE_BIND_FN(OrthographicCameraController::OmWindowResized));
	}

	void OrthographicCameraController::OnResize(float width, float height)
	{
		m_aspectRatio = (float)width / (float)height;
		m_camera.SetProjectionMatrix(-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel);
	}

	bool OrthographicCameraController::OmMouseScrolled(MouseScrolledEvent& e)
	{
		ENGINE_PROFILE_FUNCTION();

		m_zoomLevel -= e.GetYOffset();
		m_zoomLevel = std::max(m_zoomLevel, 0.25f);
		m_camera.SetProjectionMatrix(-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel);
		return false;
	}

	bool OrthographicCameraController::OmWindowResized(WindowResizeEvent& e)
	{
		ENGINE_PROFILE_FUNCTION();

		OnResize((float)e.GetWidth(), (float)e.GetHeight());
		return false;
	}
}
