#pragma once
#include "Engine/Core/Events/ApplicationEvent.h"
#include "Engine/Core/Events/MouseEvent.h"
#include "OrthographicCamera.h"

namespace Engine
{
	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		OrthographicCamera& GetCamera() { return m_camera; };
		const OrthographicCamera& GetCamera() const { return m_camera; };

		void OnUpdate();
		void OnEvent(Event& e);
		void OnResize(float width, float height);
	private:
		bool OmMouseScrolled(MouseScrolledEvent& e);
		bool OmWindowResized(WindowResizeEvent& e);
	private:
		float m_aspectRatio;
		float m_zoomLevel = 1.0f;
		OrthographicCamera m_camera;

		bool m_rotation;

		glm::vec3 m_cameraPosition = glm::vec3(0.0f);
		float m_cameraMoveSpeed = 1.0f;
		float m_cameraRotation = 0.0f;
		float m_cameraRotationSpeed = 180.0f;
	};
}
