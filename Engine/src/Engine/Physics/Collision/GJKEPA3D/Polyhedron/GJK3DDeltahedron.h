#pragma once
#include "GJK3DTriangle.h"
#include <vector>

namespace Engine
{
	class GJK3DDeltahedron
	{
	public:
		void CreateTetrahedron(const glm::dvec3& a, const glm::dvec3& b, const glm::dvec3& c, const glm::dvec3& d);
		
		const Uniq<GJK3DTriangle>& GetClosestTriangleToOrigin();
		float GetClosestDistanceToOrigin();
		glm::dvec3 GetBarycentric();

		bool IsValidSupportPoint(const Vertex3D& newSupportPoint);
		int32_t GetTriangleShouldBeReplaced(const Vertex3D& newSupportPoint);

		glm::dvec3 GetSearchDirection();
		const Vertex3D& AddSupportPoint(glm::dvec3 vertex);
		bool ExpandDeltahedron(const int32_t removeTriangleIndex, const Vertex3D& newVertex);
	private:
		uint32_t CreateTriangle(const Vertex3D& a, const Vertex3D& b, const Vertex3D& c);
		void SortTriangleByDistanceToOrigin(const int32_t triangleIndex);
		bool AlreadyExists(const Vertex3D& vertex);
		
		void ExpandWithNewVertex(const int32_t removeTriangleIndex, const Vertex3D& newVertex);
		void RemoveTriangle(const int32_t removeTriangleIndex);
		bool InTheSameDirection(const int32_t triangleIndex, Vertex3D vertex);
		void UpdateOriginEnclosed(const int32_t removeTriangleIndex);
		bool UpdateNeighbors();

		int32_t m_triangleHeadIndex = -1;

		uint32_t m_numOfSupportPoint = 0;
		Vertex3D m_supportPoints[512];

		uint32_t m_numOfTriagnles = 0;
		Uniq<GJK3DTriangle> m_triangles[1024];

		uint32_t m_numOfExpandedTriagnles = 0;
		uint32_t m_expandedTriangleIndices[1024];
		
		bool m_originEnclosed = false;

		const double COLLIDE_EPSILON = 1e-6;
		const double NUMERIC_EPSILON = 1e-12;
	};
}
