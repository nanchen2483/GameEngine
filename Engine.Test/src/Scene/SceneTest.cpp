#include "enginetestpch.h"

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
			glm::mat4& rotationMatrix = glm::rotate(glm::mat4(1.0f), rotation.x, { 1, 0, 0 })
				* glm::rotate(glm::mat4(1.0f), rotation.y, { 0, 1, 0 })
				* glm::rotate(glm::mat4(1.0f), rotation.z, { 0, 0, 1 });
			return glm::translate(glm::mat4(1.0f), translation)
				* rotationMatrix
				* glm::scale(glm::mat4(1.0f), scale);
		}
	};

	TEST_F(SceneTest, CreateEntitySuccessfully)
	{
		// Arrange

		// Act
		Engine::Entity defaultEntity = scene->CreateEntity();
		std::string tagName = "New entity";
		Engine::Entity entityWithGivenTag = scene->CreateEntity(tagName);

		// Assert
		EXPECT_TRUE(entityWithGivenTag);
		EXPECT_TRUE(entityWithGivenTag.HasComponent<Engine::TagComponent>());
		EXPECT_EQ(tagName, entityWithGivenTag.GetComponent<Engine::TagComponent>().tag);

		EXPECT_TRUE(entityWithGivenTag);
		EXPECT_TRUE(defaultEntity.HasComponent<Engine::TagComponent>());
		EXPECT_EQ("Entity", defaultEntity.GetComponent<Engine::TagComponent>().tag);
		EXPECT_TRUE(defaultEntity.HasComponent<Engine::TransformComponent>());
		Engine::TransformComponent& transformComponent = defaultEntity.GetComponent<Engine::TransformComponent>();
		EXPECT_EQ(glm::vec3(0.0f, 0.0f, 0.0f), transformComponent.rotation);
		EXPECT_EQ(glm::vec3(1.0f, 1.0f, 1.0f), transformComponent.scale);
		EXPECT_EQ(glm::vec3(0.0f, 0.0f, 0.0f), transformComponent.translation);
		glm::mat4& transform = GetTransform(transformComponent.translation, transformComponent.rotation, transformComponent.scale);
		EXPECT_EQ(transform, transformComponent.GetTransform());
	}

	TEST_F(SceneTest, GetPrimaryCameraSuccessfully)
	{
		// Arrange
		Engine::Entity entity = scene->CreateEntity();
		glm::vec2 viewportSize = { 1280.0f, 720.0f };
		scene->OnViewportResize(viewportSize.x, viewportSize.y);
		entity.AddComponent<Engine::CameraComponent>();

		// Act
		Engine::Entity primaryCameraEntity = scene->GetPrimaryCameraEntity();

		// Assert
		EXPECT_EQ(entity, primaryCameraEntity);
	}
}
