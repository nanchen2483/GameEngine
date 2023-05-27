#pragma once
#include "Engine/Core/Constant/DirectionalLightConstant.h"
#include "Engine/Math/Math.h"
#include "Engine/Util/Algorithm/ConvexHull.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Engine
{
	struct Plane
	{
		glm::vec3 normal{ 0.f, 1.f, 0.f }; // Unit vector
		float distance{};

		Plane() = default;
		Plane(const glm::vec3& position, const glm::vec3& norm)
			: normal(glm::normalize(norm))
			, distance(glm::dot(normal, position))
		{
		}

		float GetSignedDistanceToPlan(const glm::vec3& point) const
		{
			return glm::dot(normal, point) - distance;
		}
	};

	struct Frustum
	{
		Plane topFace{};
		Plane bottomFace{};

		Plane rightFace{};
		Plane leftFace{};

		Plane farFace{};
		Plane nearFace{};

		std::vector<glm::vec3> points{};

		Frustum(glm::vec3 position, glm::vec3 rotation, float FOV, float nearClip, float farClip, float aspectRatio)
		{
			const glm::quat orientation			= glm::quat(rotation);
			const glm::vec3 rightDirection		= glm::rotate(orientation, glm::vec3(1.0f, 0.0f, 0.0f));
			const glm::vec3 upDirection			= glm::rotate(orientation, glm::vec3(0.0f, 1.0f, 0.0f));
			const glm::vec3 forwardDirection	= glm::rotate(orientation, glm::vec3(0.0f, 0.0f, -1.0f));
			const float halfVSide				= farClip * tanf(glm::radians(FOV) * 0.5f);
			const float halfHSide				= halfVSide * aspectRatio;
			const glm::vec3 frontMultFar		= farClip * forwardDirection;

			glm::vec3 farLeftPoint = frontMultFar - rightDirection * halfHSide;
			glm::vec3 farRightPoint = frontMultFar + rightDirection * halfHSide;

			// Planes
			nearFace	= Plane(position + nearClip * forwardDirection, forwardDirection);
			farFace		= Plane(position + frontMultFar, -forwardDirection);
			rightFace	= Plane(position, glm::cross(upDirection, farRightPoint));
			leftFace	= Plane(position, glm::cross(farLeftPoint, upDirection));
			bottomFace	= Plane(position, glm::cross(rightDirection, frontMultFar - upDirection * halfVSide));
			topFace		= Plane(position, glm::cross(frontMultFar + upDirection * halfVSide, rightDirection));

			// Points
			points.push_back(position);
			points.push_back(position + farLeftPoint + upDirection * halfVSide);
			points.push_back(position + farLeftPoint - upDirection * halfVSide);
			points.push_back(position + farRightPoint + upDirection * halfVSide);
			points.push_back(position + farRightPoint - upDirection * halfVSide);
		}

		Frustum GetLightViewFrustum()
		{
			return Frustum(points);
		}
	private:
		Frustum(std::vector<glm::vec3> frustumPoints)
		{
			ENGINE_ASSERT(frustumPoints.size() == 5, "The size of frustum points is not equal to 5");

			glm::vec3 lightDirection = DirectionalLightConstant::Direction;

			std::vector<glm::vec3> pointsOnPlane = ClostestPointsOnPlane(lightDirection, frustumPoints);
			std::vector<glm::vec3> convexSet = ConvexHull::GetConvexSet(pointsOnPlane, lightDirection);

			// Planes
			if (convexSet.size() == 4)
			{
				nearFace	= Plane(convexSet[0], glm::cross(lightDirection, convexSet[1] - convexSet[0]));
				farFace		= Plane(convexSet[1], glm::cross(lightDirection, convexSet[2] - convexSet[1]));
				rightFace	= Plane(convexSet[2], glm::cross(lightDirection, convexSet[3] - convexSet[2]));
				leftFace	= Plane(convexSet[3], glm::cross(lightDirection, convexSet[0] - convexSet[3]));
				bottomFace	= leftFace;
				topFace		= leftFace;
			}
			else
			{
				nearFace	= Plane(convexSet[0], glm::cross(lightDirection, convexSet[1] - convexSet[0]));
				farFace		= Plane(convexSet[1], glm::cross(lightDirection, convexSet[2] - convexSet[1]));
				rightFace	= Plane(convexSet[2], glm::cross(lightDirection, convexSet[3] - convexSet[2]));
				leftFace	= Plane(convexSet[3], glm::cross(lightDirection, convexSet[4] - convexSet[3]));
				bottomFace	= Plane(convexSet[4], glm::cross(lightDirection, convexSet[0] - convexSet[4]));
				topFace		= leftFace;
			}
		}

		std::vector<glm::vec3> ClostestPointsOnPlane(glm::vec3 normal, std::vector<glm::vec3> points)
		{
			float normalDotProduct = glm::dot(normal, normal);
			for (uint32_t i = 0; i < points.size(); i++)
			{
				glm::vec3 point = points[i];
				float t = glm::dot(normal, point) / normalDotProduct;

				points[i] = point + normal * -t;
			}

			return points;
		}
	};
}
