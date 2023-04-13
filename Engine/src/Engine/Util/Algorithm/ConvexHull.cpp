#include "enginepch.h"
#include "ConvexHull.h"
#include <glm/gtx/quaternion.hpp>

namespace Engine
{
	glm::vec3 ConvexHull::s_pivotPoint = {};
	glm::vec3 ConvexHull::s_planeNormal = {};
	std::vector<glm::vec3> ConvexHull::GetConvexSet(std::vector<glm::vec3> points, glm::vec3 planeNormal)
	{
		if (points.size() < 3)
		{
			return points;
		}

		uint32_t minYIndex = 0;
		for (uint32_t i = 1; i < points.size(); ++i)
		{
			if (points[i].y < points[minYIndex].y)
			{
				minYIndex = i;
			}
		}

		std::swap(points[0], points[minYIndex]);

		s_planeNormal = planeNormal;
		s_pivotPoint = points[0];
		std::sort(points.begin() + 1, points.end(), Compare);

		std::vector<glm::vec3> convexSet = std::vector { points[0], points[1], points[2] };
		for (uint32_t i = 3; i < points.size(); i++)
		{
			glm::vec3 pointA = convexSet.back();
			convexSet.pop_back();

			while (GetOrientationType(convexSet.back(), pointA, points[i]) != OrientationType::Counterclockwise)
			{
				pointA = convexSet.back();
				convexSet.pop_back();
			}

			convexSet.push_back(pointA);
			convexSet.push_back(points[i]);
		}

		return convexSet;
	}

	bool ConvexHull::Compare(glm::vec3 pointA, glm::vec3 pointB)
	{
		OrientationType type = GetOrientationType(s_pivotPoint, pointA, pointB);
		if (type == OrientationType::Collinear)
		{
			return glm::distance2(s_pivotPoint, pointA) < glm::distance2(s_pivotPoint, pointB);
		}
		else
		{
			return type == OrientationType::Counterclockwise;
		}
	}

	OrientationType ConvexHull::GetOrientationType(glm::vec3 origin, glm::vec3 pointA, glm::vec3 pointB)
	{
		const glm::vec3 vectorOA = pointA - origin;
		const glm::vec3 vectorAB = pointB - pointA;

		glm::vec3 crossProduct = glm::cross(vectorOA, vectorAB);
		if (glm::length(crossProduct) == 0.0f)
		{
			return OrientationType::Collinear;
		}
		else
		{
			if (glm::dot(crossProduct, s_planeNormal) > 0.0f)
			{
				return OrientationType::Counterclockwise;
			}
			else
			{
				return OrientationType::Clockwise;
			}
		}
	}
}
