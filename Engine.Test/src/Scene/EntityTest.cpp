#include "enginetestpch.h"

namespace EngineTest
{
	struct EntityTest : testing::Test
	{
		Engine::Entity entity;
		Engine::Ptr<Engine::Scene> scene;
		glm::vec2 viewportSize = { 1280.0f, 720.0f };

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

		// Act
		entity.AddComponent<Engine::CameraComponent>();

		// Assert
		EXPECT_TRUE(entity.HasComponent<Engine::CameraComponent>());
		Engine::CameraComponent& expectedCameraComponent = entity.GetComponent<Engine::CameraComponent>();
		EXPECT_TRUE(expectedCameraComponent.primary);
		EXPECT_FALSE(expectedCameraComponent.fixedAspectRatio);
		EXPECT_EQ(-1.0f, expectedCameraComponent.camera.GetOrthographicNearClip());
		EXPECT_EQ(1.0f, expectedCameraComponent.camera.GetOrthographicFarClip());
		EXPECT_EQ(10.0f, expectedCameraComponent.camera.GetOrthographicSize());
		EXPECT_EQ(0.01f, expectedCameraComponent.camera.GetPerspectiveNearClip());
		EXPECT_EQ(1000.0f, expectedCameraComponent.camera.GetPerspectiveFarClip());
		EXPECT_EQ(glm::radians(45.0f), expectedCameraComponent.camera.GetPerspectiveFOV());
		EXPECT_EQ(Engine::SceneCamera::ProjectionType::Perspective, expectedCameraComponent.camera.GetProjectionType());
		glm::mat4& perspective = glm::perspective(glm::radians(45.0f), viewportSize.x / viewportSize.y, 0.01f, 1000.0f);
		EXPECT_EQ(perspective, expectedCameraComponent.camera.GetProjection());
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
		uint32_t entityId = entity;

		// Assert
		EXPECT_TRUE(entityId >= 0);
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
