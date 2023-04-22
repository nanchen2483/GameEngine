#pragma once
#include "GJK3DTriangle.h"
#include "Engine/Util/OrderedLinkedList.h"

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

		const Ptr<GJK3DTriangle> GetClosestTriangleToOrigin() const { return m_triangles.GetHeadValue(); }
		double GetClosestDistanceToOrigin();
		const glm::dvec3 GetBarycentric();
		const glm::dvec3 GetSearchDirection();

		GJK3DStatus ExpandWithNewPoint(const glm::dvec3& newSupportPoint);
	private:
		const Ptr<GJK3DTriangle> CreateTriangle(const glm::dvec3& pointA, const glm::dvec3& pointB, const glm::dvec3& pointC);
		void AddSupportPoint(const glm::dvec3& newSupportPoint);
		bool IsValidSupportPoint(const glm::dvec3& newSupportPoint);
		bool AlreadyExists(const glm::dvec3& newSupportPoint);
		const Ptr<GJK3DTriangle> GetTriangleToBeExpanded(const glm::dvec3& newSupportPoint);
		void UpdateOriginEnclosed(const Ptr<GJK3DTriangle>& removeTriangle);
		void ExpandWithNewPoint(const glm::dvec3& newPoint, const Ptr<GJK3DTriangle>& removeTriangle);
		GJK3DStatus UpdateNeighbors();

		OrderedLinkedList<Ptr<GJK3DTriangle>> m_triangles{ 1024 };

		uint32_t m_numOfSupportPoint;
		glm::dvec3 m_supportPoints[512];

		uint32_t m_numOfExpandedTriagnles;
		Weak<GJK3DTriangle> m_expandedTriangles[1024];
		
		bool m_originEnclosed;

		const double COLLIDE_EPSILON = 1e-6;
		const double NUMERIC_EPSILON = 1e-12;
	};
}
