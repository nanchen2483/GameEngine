#pragma once
#include "GJK3DTriangle.h"
#include "Engine/Util/LinkedList.h"

namespace Engine
{
	enum GJK3DStatus
	{
		FINISHED = 1,
		NOT_FINISHED = 2,
		NOT_OVERLAP = 3,
	};

	class GJK3DDeltahedron
	{
	public:
		GJK3DDeltahedron(const glm::dvec3& pointA, const glm::dvec3& poiuntB, const glm::dvec3& pointC, const glm::dvec3& pointD);

		const GJK3DTriangle* GetClosestTriangleToOrigin() const { return m_triangles.GetHeadValue(); }
		float GetClosestDistanceToOrigin();
		const glm::dvec3 GetBarycentric();
		const glm::dvec3 GetSearchDirection();

		GJK3DStatus ExpandWithNewPoint(const glm::dvec3& newSupportPoint);
	private:
		GJK3DTriangle* CreateTriangle(const glm::dvec3& pointA, const glm::dvec3& pointB, const glm::dvec3& pointC);
		void AddSupportPoint(glm::dvec3 newSupportPoint);
		bool IsValidSupportPoint(const glm::dvec3& newSupportPoint);
		bool AlreadyExists(const glm::dvec3& newSupportPoint);
		GJK3DTriangle* GetTriangleToBeExpanded(const glm::dvec3& newSupportPoint);
		void UpdateOriginEnclosed(const GJK3DTriangle* removeTriangle);
		void ExpandWithNewPoint(const glm::dvec3& newPoint, GJK3DTriangle* removeTriangle);
		GJK3DStatus UpdateNeighbors();

		LinkedList<GJK3DTriangle*> m_triangles;

		uint32_t m_numOfSupportPoint;
		glm::dvec3 m_supportPoints[512];

		uint32_t m_numOfExpandedTriagnles;
		GJK3DTriangle* m_expandedTriangles[1024];
		
		bool m_originEnclosed;

		const double COLLIDE_EPSILON = 1e-6;
		const double NUMERIC_EPSILON = 1e-12;
	};
}
