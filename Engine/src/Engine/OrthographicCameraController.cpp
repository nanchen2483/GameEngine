#include "enginepch.h"
#include "OrthographicCameraController.h"

#include "Engine/Input.h"
#include "Engine/KeyCodes.h"

namespace Engine
{
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		: m_aspectRatio(aspectRatio), m_camera(-m_aspectRatio * m_zoomLevel, m_aspectRatio* m_zoomLevel, -m_zoomLevel, m_zoomLevel), m_rotation(rotation)
	{
	}

	void OrthographicCameraController::OnUpdate(TimeStep timeStep)
	{
		if (Input::IsKeyPressed(ENGINE_KEY_A))
		{
			m_cameraPosition.x -= m_cameraMoveSpeed * timeStep;
		}
		else if (Input::IsKeyPressed(ENGINE_KEY_D))
		{
			m_cameraPosition.x += m_cameraMoveSpeed * timeStep;
		}

		if (Input::IsKeyPressed(ENGINE_KEY_W))
		{
			m_cameraPosition.y += m_cameraMoveSpeed * timeStep;
		}
		else if (Input::IsKeyPressed(ENGINE_KEY_S))
		{
			m_cameraPosition.y -= m_cameraMoveSpeed * timeStep;
		}

		if (m_rotation)
		{
			if (Input::IsKeyPressed(ENGINE_KEY_Q))
			{
				m_cameraRotation += m_cameraRotationSpeed * timeStep;
			}
			else if (Input::IsKeyPressed(ENGINE_KEY_E))
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
		EventDispatcher dispacher(e);
		dispacher.Dispatch<MouseScrolledEvent>(ENGINE_BIND_EVENT_FN(OrthographicCameraController::OmMouseScrolled));
		dispacher.Dispatch<WindowResizeEvent>(ENGINE_BIND_EVENT_FN(OrthographicCameraController::OmWindowResized));
	}

	bool OrthographicCameraController::OmMouseScrolled(MouseScrolledEvent& e)
	{
		m_zoomLevel -= e.GetYOffset();
		m_zoomLevel = std::max(m_zoomLevel, 0.25f);
		m_camera.SetProjectionMatrix(-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel);
		return false;
	}

	bool OrthographicCameraController::OmWindowResized(WindowResizeEvent& e)
	{
		m_aspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_camera.SetProjectionMatrix(-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel);
		return false;
	}



}
