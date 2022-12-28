#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Engine
{
	struct Plan
	{
		glm::vec3 normal = { 0.f, 1.f, 0.f }; // Unit vector
		float distance = 0.f;

		Plan() = default;
		Plan(const glm::vec3& position, const glm::vec3& norm)
			: normal(glm::normalize(norm)), distance(glm::dot(normal, position)) {}

		float GetSignedDistanceToPlan(const glm::vec3& point) const { return glm::dot(normal, point) - distance; }
	};

	struct Frustum
	{
		Plan topFace;
		Plan bottomFace;

		Plan rightFace;
		Plan leftFace;

		Plan farFace;
		Plan nearFace;

		Frustum(glm::vec3 position, glm::vec3 rotation, float FOV, float nearClip, float farClip, float aspectRatio)
		{
			const glm::quat orientation			= glm::quat(rotation);
			const glm::vec3 rightDirection		= glm::rotate(orientation, glm::vec3(1.0f, 0.0f, 0.0f));
			const glm::vec3 upDirection			= glm::rotate(orientation, glm::vec3(0.0f, 1.0f, 0.0f));
			const glm::vec3 forwardDirection	= glm::rotate(orientation, glm::vec3(0.0f, 0.0f, -1.0f));
			const float halfVSide				= farClip * tanf(glm::radians(FOV) * 0.5f);
			const float halfHSide				= halfVSide * aspectRatio;
			const glm::vec3 frontMultFar		= farClip * forwardDirection;

			nearFace	= Plan(position + nearClip * forwardDirection, forwardDirection);
			farFace		= Plan(position + frontMultFar, -forwardDirection);
			rightFace	= Plan(position, glm::cross(upDirection, frontMultFar + rightDirection * halfHSide));
			leftFace	= Plan(position, glm::cross(frontMultFar - rightDirection * halfHSide, upDirection));
			bottomFace	= Plan(position, glm::cross(rightDirection, frontMultFar - upDirection * halfVSide));
			topFace		= Plan(position, glm::cross(frontMultFar + upDirection * halfVSide, rightDirection));
		}
	};
}
