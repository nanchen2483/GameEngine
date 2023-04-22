#include "enginepch.h"
#include "GJKEPA3D.h"
#include "Engine/Math/Math.h"

namespace Engine
{
	const CollisionInfo GJKEPA3D::Detect(const ShapeInfo& shapeA, const ShapeInfo& shapeB)
	{
		m_shapeA = shapeA;
		m_shapeB = shapeB;

		// Create Deltahedron
		glm::dvec3 center = m_shapeA.position - m_shapeB.position;
		glm::dvec3 pointA = V0 + center;
		glm::dvec3 pointB = V1 + center;
		glm::dvec3 pointC = V2 + center;
		glm::dvec3 pointD = V3 + center;
		m_shapeA.pointMap = m_shapeB.pointMap = { {pointA, {}}, {pointB, {}}, {pointC, {}}, {pointD, {}} };
		m_deltahedron = CreateUniq<GJK3DDeltahedron>(pointA, pointB, pointC, pointD);

		return Solve();
	}
	
	const CollisionInfo GJKEPA3D::Solve()
	{
		CollisionInfo info;
		uint32_t iteration = 0;
		while (++iteration < MAX_ITERATION)
		{
			const glm::dvec3 newSupportPoint = CreateNewSupportPoint();
			GJK3DStatus status = m_deltahedron->ExpandWithNewPoint(newSupportPoint);
			if (status == GJK3DStatus::FINISHED)
			{
				const Ptr<GJK3DTriangle>& closestTriangle = m_deltahedron->GetClosestTriangleToOrigin();
				glm::dvec3 baryCentric = m_deltahedron->GetBarycentric();

				glm::dvec3 closestPointA = GetPointFromShape(m_shapeA, closestTriangle, baryCentric);
				glm::dvec3 closestPointB = GetPointFromShape(m_shapeB, closestTriangle, baryCentric);

				info.separation = m_deltahedron->GetClosestDistanceToOrigin();
				info.isCollided = info.separation < 0.0;
				info.collisionNormal = glm::normalize(closestPointB - closestPointA);

				return info;
			}

			if (status == NOT_OVERLAP)
			{
				break;
			}
		}

		return info;
	}

	glm::dvec3 GJKEPA3D::CreateNewSupportPoint()
	{
		glm::dvec3 direction = m_deltahedron->GetSearchDirection();

		glm::dvec3& pointA = GetPointFromShape(m_shapeA, -direction);
		glm::dvec3& pointB = GetPointFromShape(m_shapeB, direction);
		glm::dvec3 supportPoint = pointA - pointB;
		
		m_shapeA.pointMap[supportPoint] = pointA;
		m_shapeB.pointMap[supportPoint] = pointB;
		
		return supportPoint;
	}
	
	glm::dvec3 GJKEPA3D::GetPointFromShape(const ShapeInfo& shape, const Ptr<GJK3DTriangle>& triangle, const glm::dvec3& barycentric)
	{
		glm::dvec3 a = shape.pointMap.at(triangle->GetA());
		glm::dvec3 b = shape.pointMap.at(triangle->GetB());
		glm::dvec3 c = shape.pointMap.at(triangle->GetC());

		glm::mat3 matrix(a, b, c);

		return matrix * barycentric;
	}
	
	glm::dvec3 GJKEPA3D::GetPointFromShape(const ShapeInfo& shape, const glm::dvec3& direction)
	{
		glm::dvec3 supportPointDirection = glm::dvec3(glm::dvec4(direction, 1.0) * glm::transpose(shape.orientation));
		glm::dvec3 point = shape.boundingValue.GetSupportPoint(supportPointDirection);
		point = glm::dvec3(glm::dvec4(point, 1.0) * shape.orientation * shape.scale);
		point += shape.position;
		
		return point;
	}
}
