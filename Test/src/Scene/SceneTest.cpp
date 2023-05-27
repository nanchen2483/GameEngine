#include <gtest/gtest.h>
#include "Engine.h"

namespace EngineTest
{
	struct SceneTest : testing::Test
	{
		Engine::Ptr<Engine::Scene> scene;

		SceneTest()
		{
			scene = Engine::CreatePtr<Engine::Scene>();
		}

		glm::mat4 GetTransform(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale)
		{
			const glm::mat4& rotationMatrix = glm::rotate(glm::mat4(1.0f), rotation.x, { 1, 0, 0 })
				* glm::rotate(glm::mat4(1.0f), rotation.y, { 0, 1, 0 })
				* glm::rotate(glm::mat4(1.0f), rotation.z, { 0, 0, 1 });
			return glm::translate(glm::mat4(1.0f), translation)
				* rotationMatrix
				* glm::scale(glm::mat4(1.0f), scale);
		}

		glm::mat4 GetViewMatrix(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale)
		{
			return glm::translate(glm::mat4(1.0f), -translation) *
				glm::toMat4(glm::quat(rotation)) *
				glm::scale(glm::mat4(1.0f), scale);
		}
	};

	TEST_F(SceneTest, CreateEntitySuccessfully)
	{
		// Arrange
		std::string expectedTagName = "New entity";
		glm::vec3 expectedTranslation = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 expectedRotation = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 expectedScale = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::mat4 expectedTransform = GetTransform(expectedTranslation, expectedRotation, expectedScale);
		glm::mat4 expectedViewMatrix = GetViewMatrix(expectedTranslation, expectedRotation, expectedScale);

		// Act
		Engine::Entity actualDefaultEntity = scene->CreateEntity();
		Engine::Entity actualEntityWithGivenTag = scene->CreateEntity(expectedTagName);
		Engine::TransformComponent& actualTransformComponent = actualDefaultEntity.GetComponent<Engine::TransformComponent>();

		// Assert
		EXPECT_TRUE(actualDefaultEntity);
		EXPECT_TRUE(actualDefaultEntity.HasComponent<Engine::TagComponent>());
		EXPECT_EQ("Entity", actualDefaultEntity.GetComponent<Engine::TagComponent>().tag);
		EXPECT_TRUE(actualDefaultEntity.HasComponent<Engine::TransformComponent>());

		EXPECT_TRUE(actualEntityWithGivenTag);
		EXPECT_TRUE(actualEntityWithGivenTag.HasComponent<Engine::TagComponent>());
		EXPECT_EQ(expectedTagName, actualEntityWithGivenTag.GetComponent<Engine::TagComponent>().tag);
		EXPECT_TRUE(actualEntityWithGivenTag.HasComponent<Engine::TransformComponent>());

		EXPECT_EQ(expectedTranslation, actualTransformComponent.GetTranslation());
		EXPECT_EQ(expectedRotation, actualTransformComponent.GetRotation());
		EXPECT_EQ(expectedScale, actualTransformComponent.GetScale());
		EXPECT_EQ(expectedTransform, (glm::mat4)actualTransformComponent);
		EXPECT_EQ(expectedViewMatrix, glm::inverse((glm::mat4)actualTransformComponent));
	}

	TEST_F(SceneTest, GetPrimaryCameraSuccessfully)
	{
		// Arrange
		Engine::Entity entity = scene->CreateEntity();
		glm::vec2 viewportSize = { 1280.0f, 720.0f };
		scene->OnViewportResize(viewportSize.x, viewportSize.y);
		entity.AddComponent<Engine::CameraComponent>().primary = true;

		// Act
		Engine::Entity primaryCameraEntity = scene->GetPrimaryCameraEntity();

		// Assert
		EXPECT_EQ(entity, primaryCameraEntity);
	}
}
