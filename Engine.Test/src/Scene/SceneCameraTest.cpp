#include "enginetestpch.h"

namespace EngineTest
{
	struct SceneCameraTest : testing::Test
	{
		glm::mat4 CalculateOrthoProjection(float orthoSize, float aspectRatio, float orthoNear, float orthoFar)
		{
			float orthoLeft = -orthoSize * aspectRatio * 0.5f;
			float orthoRight = orthoSize * aspectRatio * 0.5f;
			float orthoBottom = -orthoSize * 0.5f;
			float orthoTop = orthoSize * 0.5f;
			return glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, orthoNear, orthoFar);
		}

		glm::mat4 CalculatePerspectiveProjection(float fov, float aspectRatio, float orthoNear, float orthoFar)
		{
			return glm::perspective(fov, aspectRatio, orthoNear, orthoFar);
		}
	};

	TEST_F(SceneCameraTest, CreateSuccessfully)
	{
		// Arrange

		// Act
		Engine::SceneCamera camera;

		// Assert
		EXPECT_EQ(-1.0f, camera.GetOrthographicNearClip());
		EXPECT_EQ(1.0f, camera.GetOrthographicFarClip());
		EXPECT_EQ(10.0f, camera.GetOrthographicSize());
		EXPECT_EQ(0.01f, camera.GetPerspectiveNearClip());
		EXPECT_EQ(1000.0f, camera.GetPerspectiveFarClip());
		EXPECT_EQ(glm::radians(45.0f), camera.GetPerspectiveFOV());
		EXPECT_EQ(Engine::SceneCamera::ProjectionType::Perspective, camera.GetProjectionType());
		auto& perspective = glm::perspective(glm::radians(45.0f), 0.0f, 0.01f, 1000.0f);
		EXPECT_EQ(perspective, camera.GetProjection());
	}

	TEST_F(SceneCameraTest, UpdateOrthographicSuccessfully)
	{
		// Arrange
		uint32_t expectedSize = 15;
		float expectedNear = -1.5f;
		float expectedFar = 1.5f;
		Engine::SceneCamera camera;
		camera.SetProjectionType(Engine::SceneCamera::ProjectionType::Orthographic);
		glm::vec2 viewportSize = { 1280.0f, 720.0f };
		camera.SetViewportSize(viewportSize.x, viewportSize.y);

		// Act
		camera.SetOrthographicSize(expectedSize);
		camera.SetOrthographicNearClip(expectedNear);
		camera.SetOrthographicFarClip(expectedFar);
		glm::mat4 expectedProjection = camera.GetProjection();

		// Assert
		EXPECT_EQ(expectedNear, camera.GetOrthographicNearClip());
		EXPECT_EQ(expectedFar, camera.GetOrthographicFarClip());
		EXPECT_EQ(expectedSize, camera.GetOrthographicSize());
		auto actualProjection = CalculateOrthoProjection(expectedSize, viewportSize.x / viewportSize.y, expectedNear, expectedFar);
		EXPECT_EQ(expectedProjection, actualProjection);

		// Act
		camera.SetOrthographic(expectedSize, expectedNear, expectedFar);
		expectedProjection = camera.GetProjection();

		// Assert
		EXPECT_EQ(expectedNear, camera.GetOrthographicNearClip());
		EXPECT_EQ(expectedFar, camera.GetOrthographicFarClip());
		EXPECT_EQ(expectedSize, camera.GetOrthographicSize());
		actualProjection = CalculateOrthoProjection(expectedSize, viewportSize.x / viewportSize.y, expectedNear, expectedFar);
		EXPECT_EQ(expectedProjection, actualProjection);
	}

	TEST_F(SceneCameraTest, UpdatePerspectiveSuccessfully)
	{
		// Arrange
		uint32_t fov = 15;
		float expectedNear = -1.5f;
		float expectedFar = 1.5f;
		Engine::SceneCamera camera;
		glm::vec2 viewportSize = { 1280.0f, 720.0f };
		camera.SetViewportSize(viewportSize.x, viewportSize.y);

		// Act
		camera.SetPerspectiveFOV(fov);
		camera.SetPerspectiveNearClip(expectedNear);
		camera.SetPerspectiveFarClip(expectedFar);
		glm::mat4 expectedProjection = camera.GetProjection();

		// Assert
		EXPECT_EQ(expectedNear, camera.GetPerspectiveNearClip());
		EXPECT_EQ(expectedFar, camera.GetPerspectiveFarClip());
		EXPECT_EQ(fov, camera.GetPerspectiveFOV());
		auto actualProjection = CalculatePerspectiveProjection(fov, viewportSize.x / viewportSize.y, expectedNear, expectedFar);
		EXPECT_EQ(expectedProjection, actualProjection);

		// Act
		camera.SetPerspective(fov, expectedNear, expectedFar);
		expectedProjection = camera.GetProjection();

		// Assert
		EXPECT_EQ(expectedNear, camera.GetPerspectiveNearClip());
		EXPECT_EQ(expectedFar, camera.GetPerspectiveFarClip());
		EXPECT_EQ(fov, camera.GetPerspectiveFOV());
		actualProjection = CalculatePerspectiveProjection(fov, viewportSize.x / viewportSize.y, expectedNear, expectedFar);
		EXPECT_EQ(expectedProjection, actualProjection);
	}
}
