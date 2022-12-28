#include <gtest/gtest.h>
#include "Engine.h"

namespace EngineTest
{
	struct EditorCameraTest : testing::Test
	{
		float distance = 10.0f;
		float pitch = 0.0f;
		float yaw = 0.0f;
		glm::vec3 focusPoint = glm::vec3(0.0f);
		glm::mat4 viewMatrix = glm::mat4(1.0f);

		float FOV = 45.0f;
		float viewportWidth = 1280, viewportHeight = 720;
		float nearClip = 0.1f, farClip = 1000.0f;

		EditorCameraTest()
		{
		}

		glm::vec3 GetForwardDirection() const
		{
			glm::quat orientation = GetOrientation();
			return glm::rotate(orientation, glm::vec3(0.0f, 0.0f, -1.0f));
		}

		glm::quat GetOrientation() const
		{
			return glm::quat(glm::vec3(-pitch, -yaw, 0.0f));
		}

		glm::vec3 GetPosition() const
		{
			return focusPoint - GetForwardDirection() * distance;
		}

		glm::mat4 GetViewMatrix() const
		{
			glm::vec3 position = GetPosition();
			glm::quat orientation = GetOrientation();

			return glm::inverse(glm::translate(glm::mat4(1.0f), position) * glm::toMat4(orientation));
		}

		glm::mat4 GetProjection() const
		{
			float aspectRatio = viewportWidth / viewportHeight;
			return glm::perspective(glm::radians(FOV), aspectRatio, nearClip, farClip);
		}
	};

	TEST_F(EditorCameraTest, CreateSuccessfully)
	{
		// Arrange
		glm::vec3 expectedPosition = GetPosition();
		glm::mat4 expectedViewMatrix = GetViewMatrix();
		glm::mat4 expectedProjection = GetProjection();
		glm::mat4 expectedViewProjection = expectedProjection * expectedViewMatrix;

		// Act
		Engine::EditorCamera camera;

		// Assert
		EXPECT_EQ(FOV, camera.GetFOV());
		EXPECT_EQ(expectedPosition, camera.GetPosition());
		EXPECT_EQ(expectedViewMatrix, camera.GetViewMatrix());
		EXPECT_EQ(expectedProjection, camera.GetProjection());
		EXPECT_EQ(expectedViewProjection, camera.GetViewProjection());
	}
}
