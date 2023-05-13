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
			info.penetrationDepth = -0.1;
			info.collisionNormal = glm::dvec3(-1.0, 0.0, 0.0);
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
		m_deltahedron = CreateUniq<GJK3DDeltahedron>(pointA, pointB, pointC, pointD);

		return Solve();
	}
	
	const CollisionInfo GJKEPA3D::Solve()
	{
		CollisionInfo info;
		info.iterations = 0;
		while (++info.iterations < MAX_ITERATIONS)
		{
			const glm::dvec3 newSupportPoint = CreateNewSupportPoint();
			GJK3DStatus status = m_deltahedron->ExpandWithNewPoint(newSupportPoint);
			if (status == GJK3DStatus::Finished)
			{
				const double& distance = m_deltahedron->GetClosestDistanceToOrigin();
				if (distance < 0.0)
				{
					const Ptr<GJK3DTriangle>& closestTriangle = m_deltahedron->GetClosestTriangleToOrigin();
					const glm::dvec3& collisionNormal = glm::normalize(closestTriangle->GetNormalVector());
					info.isCollided = distance < 0.0;
					info.penetrationDepth = distance;
					info.collisionNormal = collisionNormal;
				}
				else
				{
					info.separation = distance;
				}

				break;
			}

			if (status == GJK3DStatus::NotOverlap)
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
		
		return supportPoint;
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
