#include "enginepch.h"
#include "GJKEPA3D.h"
#include "Engine/Math/Math.h"

namespace Engine
{
	const CollisionInfo GJKEPA3D::Detect(const ShapeInfo& shapeA, const ShapeInfo& shapeB)
	{
		if (shapeA.position == shapeB.position)
		{
			CollisionInfo info;
			info.isCollided = true;
			info.separation = -0.1;
			info.collisionNormal = glm::dvec3(1.0, 0.0, 0.0);
			info.iterations = 0;

			return info;
		}

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
		uint32_t iterations = 0;
		while (++iterations < MAX_ITERATIONS)
		{
			const glm::dvec3 newSupportPoint = CreateNewSupportPoint();
			GJK3DStatus status = m_deltahedron->ExpandWithNewPoint(newSupportPoint);
			if (status == GJK3DStatus::FINISHED)
			{
				const Ptr<GJK3DTriangle>& closestTriangle = m_deltahedron->GetClosestTriangleToOrigin();
				glm::dvec3 baryCentric = m_deltahedron->GetBarycentric();

				glm::dvec3 closestPointA = GetPointFromShape(m_shapeA, closestTriangle, baryCentric);
				glm::dvec3 closestPointB = GetPointFromShape(m_shapeB, closestTriangle, baryCentric);
				glm::dvec3 deltaPoint = closestPointB - closestPointA;
				if (deltaPoint != glm::dvec3(0.0))
				{
					info.collisionNormal = glm::normalize(closestPointB - closestPointA);
				}

				double separation = m_deltahedron->GetClosestDistanceToOrigin();
				info.separation = separation;
				info.isCollided = separation < 0.0;
				info.iterations = iterations;


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
		
		m_shapeA.pointMap.insert({ supportPoint, pointA });
		m_shapeB.pointMap.insert({ supportPoint, pointB });
		
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
