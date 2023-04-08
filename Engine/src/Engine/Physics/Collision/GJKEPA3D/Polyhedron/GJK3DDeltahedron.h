#pragma once
#include "GJK3DTriangle.h"
#include <vector>

namespace Engine
{
	class GJK3DDeltahedron
	{
	public:
		GJK3DDeltahedron(const glm::dvec3& pointA, const glm::dvec3& poiuntB, const glm::dvec3& pointC, const glm::dvec3& pointD);
		~GJK3DDeltahedron();

		const GJK3DTriangle* GetClosestTriangleToOrigin() { return m_triangleHead; }
		float GetClosestDistanceToOrigin();
		glm::dvec3 GetBarycentric();

		bool IsValidSupportPoint(const Vertex3D& newSupportPoint);
		GJK3DTriangle* GetTriangleToBeReplaced(const Vertex3D& newSupportPoint);

		glm::dvec3 GetSearchDirection();
		const Vertex3D& AddSupportPoint(glm::dvec3 newSupportPoint);
		bool ExpandDeltahedron(GJK3DTriangle* removeTriangle, const Vertex3D& newSupportPoint);
	private:
		GJK3DTriangle* CreateTriangle(const Vertex3D& pointA, const Vertex3D& pointB, const Vertex3D& pointC);
		void SortTriangleByDistanceToOrigin(GJK3DTriangle* triangle);
		bool AlreadyExists(const Vertex3D& newSupportPoint);
		
		/// <summary>
		/// Remove a triangle from this deltahedron and add 3 new triangles based on the new point
		/// </summary>
		/// <param name="removeTriangle">The triangle to be removed</param>
		/// <param name="newPoint">The new point</param>
		void ExpandWithNewPoint(GJK3DTriangle* removeTriangle, const Vertex3D& newPoint);
		void RemoveTriangle(GJK3DTriangle* removeTriangle);
		bool InTheSameDirection(const GJK3DTriangle* triangle, Vertex3D point);
		void UpdateOriginEnclosed(const GJK3DTriangle* removeTriangle);
		bool UpdateNeighbors();

		GJK3DTriangle* m_triangleHead;

		uint32_t m_numOfSupportPoint;
		Vertex3D m_supportPoints[512];

		uint32_t m_numOfExpandedTriagnles;
		GJK3DTriangle* m_expandedTriangles[1024];
		
		bool m_originEnclosed;

		const double COLLIDE_EPSILON = 1e-6;
		const double NUMERIC_EPSILON = 1e-12;
	};
}
