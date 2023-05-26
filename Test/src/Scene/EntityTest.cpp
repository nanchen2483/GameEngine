#include <gtest/gtest.h>
#include "Engine.h"

namespace EngineTest
{
	struct EntityTest : testing::Test
	{
		Engine::Entity entity;
		Engine::Ptr<Engine::Scene> scene;
		glm::vec2 viewportSize{ 1280.0f, 720.0f };

		EntityTest()
		{
			scene = Engine::CreatePtr<Engine::Scene>();
			scene->OnViewportResize(viewportSize.x, viewportSize.y);
			entity = scene->CreateEntity();
		}
	};

	TEST_F(EntityTest, AddSpriteRendererComponentSuccessfully)
	{
		// Arrange

		// Act
		entity.AddComponent<Engine::SpriteRendererComponent>();

		// Assert
		EXPECT_TRUE(entity.HasComponent<Engine::SpriteRendererComponent>());
		Engine::SpriteRendererComponent& actualSpriteComponent = entity.GetComponent<Engine::SpriteRendererComponent>();
		EXPECT_EQ(glm::vec4(1.0f), actualSpriteComponent.color);
	}

	TEST_F(EntityTest, AddCameraComponentSuccessfully)
	{
		// Arrange
		float expectedOrthographicNearClip = -1.0f;
		float expectedOrthographicFarClip = 1.0f;
		float expectedOrthographicSize = 10.0f;
		float expectedPerspectiveNearClip = 0.1f;
		float expectedPerspectiveFarClip = 1000.0f;
		float expectedFOV = 45.0f;
		Engine::CameraProjectionType expectedProjectionType = Engine::CameraProjectionType::Perspective;
		const glm::mat4& expectedPerspective = glm::perspective(
			glm::radians(expectedFOV),
			viewportSize.x / viewportSize.y, 
			expectedPerspectiveNearClip, 
			expectedPerspectiveFarClip);

		// Act
		entity.AddComponent<Engine::CameraComponent>();
		Engine::CameraComponent& actualCameraComponent = entity.GetComponent<Engine::CameraComponent>();

		// Assert
		EXPECT_TRUE(entity.HasComponent<Engine::CameraComponent>());
		EXPECT_FALSE(actualCameraComponent.primary);
		EXPECT_FALSE(actualCameraComponent.fixedAspectRatio);
		EXPECT_EQ(expectedOrthographicNearClip, actualCameraComponent.camera.GetOrthographicNearClip());
		EXPECT_EQ(expectedOrthographicFarClip, actualCameraComponent.camera.GetOrthographicFarClip());
		EXPECT_EQ(expectedOrthographicSize, actualCameraComponent.camera.GetOrthographicSize());
		EXPECT_EQ(expectedPerspectiveNearClip, actualCameraComponent.camera.GetPerspectiveNearClip());
		EXPECT_EQ(expectedPerspectiveFarClip, actualCameraComponent.camera.GetPerspectiveFarClip());
		EXPECT_EQ(expectedFOV, actualCameraComponent.camera.GetPerspectiveFOV());
		EXPECT_EQ(expectedProjectionType, actualCameraComponent.camera.GetProjectionType());
		EXPECT_EQ(expectedPerspective, actualCameraComponent.camera.GetProjection());
	}

	TEST_F(EntityTest, RemoveComponentSuccessfully)
	{
		// Arrange
		entity.AddComponent<Engine::SpriteRendererComponent>();
		entity.AddComponent<Engine::CameraComponent>();

		// Act
		entity.RemoveComponent<Engine::SpriteRendererComponent>();
		entity.RemoveComponent<Engine::CameraComponent>();
		 
		// Assert
		EXPECT_FALSE(entity.HasComponent<Engine::CameraComponent>());
		EXPECT_FALSE(entity.HasComponent<Engine::SpriteRendererComponent>());
		EXPECT_TRUE(entity.HasComponent<Engine::TagComponent>());
		EXPECT_TRUE(entity.HasComponent<Engine::TransformComponent>());
	}

	TEST_F(EntityTest, NullableOperator)
	{
		// Arrange

		// Act
		Engine::Entity entity;
		bool entityHandlerIsNull = !entity;
		
		// Assert
		EXPECT_TRUE(entityHandlerIsNull);
	}

	TEST_F(EntityTest, GetEnttEntityOperator)
	{
		// Arrange

		// Act
		entt::entity entityHandler = entity;

		// Assert
		EXPECT_TRUE(entityHandler != entt::null);
	}

	TEST_F(EntityTest, GetIntegerIdOperator)
	{
		// Arrange

		// Act
		int32_t entityId = entity;
		uint32_t entityUid = entity;

		// Assert
		EXPECT_TRUE(entityId >= 0);
		EXPECT_TRUE(entityUid >= 0);
	}

	TEST_F(EntityTest, CompareOperator)
	{
		// Arrange
		Engine::Entity uninitializedEntity;
		Engine::Entity entityWithInvalidEntityId((entt::entity)123, scene.get());
		Engine::Entity entityWithIncorrectScene(entity, new Engine::Scene());
		Engine::Entity copyEntity(entity, scene.get());

		// Act

		// Assert
		EXPECT_FALSE(entity == uninitializedEntity);
		EXPECT_TRUE(entity != uninitializedEntity);
		EXPECT_TRUE(entity == entity);
		EXPECT_FALSE(entity != entity);
		EXPECT_FALSE(entity == entityWithInvalidEntityId);
		EXPECT_TRUE(entity != entityWithInvalidEntityId);
		EXPECT_FALSE(entity == entityWithIncorrectScene);
		EXPECT_TRUE(entity != entityWithIncorrectScene);
		EXPECT_TRUE(entity == copyEntity);
		EXPECT_FALSE(entity != copyEntity);
	}
}
