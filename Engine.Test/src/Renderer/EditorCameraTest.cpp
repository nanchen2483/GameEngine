#include "enginetestpch.h"

namespace EngineTest
{
	struct EditorCameraTest : testing::Test
	{
		float pitch = 0.0f, yaw = 0.0f;
		EditorCameraTest()
		{
		}

		glm::vec3 GetUpDirection() const
		{
			return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
		}

		glm::vec3 GetRightDirection() const
		{
			return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
		}

		glm::vec3 GetForwardDirection() const
		{
			return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
		}

		glm::quat GetOrientation() const
		{
			return glm::quat(glm::vec3(-pitch, -yaw, 0.0f));
		}
	};

	TEST_F(EditorCameraTest, CreateSuccessfully)
	{
		float expectedDistance = 10.0f;
		float expectedPitch = 0.0f;
		float expectedYaw = 0.0f;
		glm::vec3 expectedPosition(0.0f);
		Engine::EditorCamera camera;

		EXPECT_EQ(GetOrientation(), camera.GetOrientation());
		EXPECT_EQ(GetUpDirection(), camera.GetUpDirection());
		EXPECT_EQ(GetRightDirection(), camera.GetRightDirection());
		EXPECT_EQ(GetForwardDirection(), camera.GetForwardDirection());
		EXPECT_EQ(expectedDistance, camera.GetDistance());
		EXPECT_EQ(expectedPitch, camera.GetPitch());
		EXPECT_EQ(expectedYaw, camera.GetYaw());
		EXPECT_EQ(expectedPosition, camera.GetPosition());
		EXPECT_EQ(glm::mat4(1.0f), camera.GetViewMatrix());
		EXPECT_EQ(glm::mat4(1.0f), camera.GetProjection());
	}
}
