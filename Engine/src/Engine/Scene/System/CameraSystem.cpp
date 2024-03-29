#include "enginepch.h"
#include "CameraSystem.h"
#include "Engine/Core/Enum/KeyCodes.h"
#include "Engine/Core/System/System.h"
#include "Engine/Core/Window/Input.h"

namespace Engine
{
	void CameraSystem::OnUpdate(Transform &transform, SceneCamera &camera)
	{
		// Update rotation
		const glm::vec2& deltaMousePosition = System::GetMouseDeltaPosition();
		float yawSign = camera.GetUpDirection().y < 0 ? -1.0f : 1.0f;
		transform.rotation.x -= deltaMousePosition.y * camera.GetRotationSpeed();
		transform.rotation.y -= deltaMousePosition.x * camera.GetRotationSpeed() * yawSign;

		// Update translation
		const glm::quat& orientation = glm::quat(transform.rotation);
		const glm::vec3& upDirection = glm::rotate(orientation, glm::vec3(0.0f, 1.0f, 0.0f));
		const glm::vec3& rightDirection = glm::rotate(orientation, glm::vec3(1.0f, 0.0f, 0.0f));
		const glm::vec3& forwardDirection = glm::rotate(orientation, glm::vec3(0.0f, 0.0f, -1.0f));

		float velocity = camera.GetMoveSpeed() * System::GetDeltaTime();
		glm::vec3 previousPosition = transform.translation;
		if (Input::IsKeyPressed(KeyCode::W))
		{
			transform.translation += forwardDirection * velocity;
		}

		if (Input::IsKeyPressed(KeyCode::S))
		{
			transform.translation -= forwardDirection * velocity;
		}

		if (Input::IsKeyPressed(KeyCode::A))
		{
			transform.translation -= rightDirection * velocity;
		}

		if (Input::IsKeyPressed(KeyCode::D))
		{
			transform.translation += rightDirection * velocity;
		}

		if (Input::IsKeyPressed(KeyCode::E))
		{
			transform.translation += upDirection * velocity;
		}

		if (Input::IsKeyPressed(KeyCode::Q))
		{
			transform.translation -= upDirection * velocity;
		}

		transform.velocity = (transform.translation - previousPosition) / (float)System::GetDeltaTime();

		camera.SetUpDirection(upDirection);
	}

	Frustum CameraSystem::GetFrustum(const Transform& transform, const SceneCamera& camera)
	{
		return Frustum(
			transform.translation,
			transform.rotation,
			camera.GetFOV(),
			camera.GetPerspectiveNearClip(),
			camera.GetPerspectiveFarClip(),
			camera.GetAspectRatio());
	}
}
