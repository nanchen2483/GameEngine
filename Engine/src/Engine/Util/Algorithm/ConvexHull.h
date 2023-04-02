#pragma once
#include <glm/glm.hpp>

namespace Engine
{
	enum OrientationType
	{
		Collinear = 1,
		Clockwise = 2,
		Counterclockwise = 3,
	};

	class ConvexHull
	{
	public:
		static std::vector<glm::vec3> GetConvexSet(std::vector<glm::vec3> points, glm::vec3 planeNormal);
	private:
		ConvexHull() = default;
		static bool Compare(glm::vec3 pointA, glm::vec3 pointB);
		static OrientationType GetOrientationType(glm::vec3 origin, glm::vec3 pointA, glm::vec3 pointB);

		static glm::vec3 s_pivotPoint;
		static glm::vec3 s_planeNormal;
	};
}
