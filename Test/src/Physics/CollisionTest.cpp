#include <gtest/gtest.h>
#include "Engine.h"
#include "Engine/Physics/Collision/ICollision.h"

namespace EngineTest
{
	struct CollisionTest : testing::Test
	{
		Engine::Uniq<Engine::ICollision> collision;
		Engine::Ptr<Engine::IBoundingBox> boudingBox;

		CollisionTest()
		{
			collision = Engine::ICollision::Create(Engine::CollisionType::GjkEpa3D);
			boudingBox = Engine::IBoundingBox::Create(Engine::BoundingBoxType::AABB, glm::vec3(-0.5), glm::vec3(0.5));
		}
	};

	TEST_F(CollisionTest, CheckCollisionDetectionOverlap)
	{
		// Arrange
		Engine::Transform transformA;
		transformA.translation = glm::vec3(-0.23, 0.0, 0.0);
		Engine::ShapeInfo shapeA(transformA, boudingBox->GetBoundingValue());

		Engine::Transform transformB;
		transformB.translation = glm::vec3(0.1, 0.2, 0.0);
		Engine::ShapeInfo shapeB(transformB, boudingBox->GetBoundingValue());

		// Act
		const Engine::CollisionInfo info = collision->Detect(shapeA, shapeB);

		// Assert
		EXPECT_EQ(info.isCollided, true);
		EXPECT_EQ(info.separation, 0.0);
		EXPECT_EQ(info.penetrationDepth, -0.66999999433755875);
		EXPECT_EQ(info.collisionNormal, glm::dvec3(1.0, 0.0, 0.0));
		EXPECT_EQ(info.iterations, 8);
	}

	TEST_F(CollisionTest, CheckCollisionDetectionNotOverlap)
	{
		// Arrange
		Engine::Transform transformA;
		transformA.translation = glm::vec3(-0.5, 1.0, 0.0);
		Engine::ShapeInfo shapeA(transformA, boudingBox->GetBoundingValue());

		Engine::Transform transformB;
		transformB.translation = glm::vec3(1.2, .0, 0.2);
		Engine::ShapeInfo shapeB(transformB, boudingBox->GetBoundingValue());

		// Act
		const Engine::CollisionInfo info = collision->Detect(shapeA, shapeB);

		// Assert
		EXPECT_EQ(info.isCollided, false);
		EXPECT_EQ(info.separation, 0.70000004768371582);
		EXPECT_EQ(info.penetrationDepth, 0.0);
		EXPECT_EQ(info.collisionNormal, glm::dvec3(0.0));
		EXPECT_EQ(info.iterations, 3);
	}

	TEST_F(CollisionTest, CheckCollisionDetectionSideBySide)
	{
		// Arrange
		Engine::Transform transformA;
		transformA.translation = glm::vec3(-0.5, 0.0, 0.0);
		Engine::ShapeInfo shapeA(transformA, boudingBox->GetBoundingValue());

		Engine::Transform transformB;
		transformB.translation = glm::vec3(0.5, 0.0, 0.0);
		Engine::ShapeInfo shapeB(transformB, boudingBox->GetBoundingValue());

		// Act
		const Engine::CollisionInfo info = collision->Detect(shapeA, shapeB);

		// Assert
		EXPECT_EQ(info.isCollided, false);
		EXPECT_EQ(info.separation, 0.0);
		EXPECT_EQ(info.penetrationDepth, 0.0);
		EXPECT_EQ(info.collisionNormal, glm::dvec3(0.0));
		EXPECT_EQ(info.iterations, 3);
	}

	TEST_F(CollisionTest, CheckCollisionDetectionTotallyOverlap)
	{
		// Arrange
		Engine::Transform transformA;
		Engine::ShapeInfo shapeA(transformA, boudingBox->GetBoundingValue());

		Engine::Transform transformB;
		Engine::ShapeInfo shapeB(transformB, boudingBox->GetBoundingValue());

		// Act
		const Engine::CollisionInfo info = collision->Detect(shapeA, shapeB);

		// Assert
		EXPECT_EQ(info.isCollided, true);
		EXPECT_EQ(info.separation, 0.0);
		EXPECT_EQ(info.penetrationDepth, -0.10000000000000001);
		EXPECT_EQ(info.collisionNormal, glm::dvec3(-1.0, 0.0, 0.0));
		EXPECT_EQ(info.iterations, 0);
	}
}
