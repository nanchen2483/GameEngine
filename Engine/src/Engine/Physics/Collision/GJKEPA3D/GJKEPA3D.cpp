#include "enginepch.h"
#include "GJKEPA3D.h"
#include "Engine/Math/Math.h"

namespace Engine
{
	bool GJKEPA3D::Detect(const ShapeInfo& shapeA, const ShapeInfo& shapeB)
	{
		m_distanceBetweenAToB = 0;
		m_directionFromAToB = {};

		m_shapeA = shapeA;
		m_shapeB = shapeB;

		// Create Deltahedron
		glm::dvec3 center = m_shapeA.position - m_shapeB.position;
		glm::dvec3 pointA = V0 + center;
		glm::dvec3 pointB = V1 + center;
		glm::dvec3 pointC = V2 + center;
		glm::dvec3 pointD = V3 + center;
		m_deltahedron = CreateUniq<GJK3DDeltahedron>(pointA, pointB, pointC, pointD);

		m_isCollided = Solve();

		return m_isCollided;
	}
	
	bool GJKEPA3D::Solve()
	{
		uint32_t iteration = 0;
		while (++iteration < MAX_ITERATION)
		{
			const glm::dvec3 newSupportPoint = CreateNewSupportPoint();
			GJK3DStatus status = m_deltahedron->ExpandWithNewPoint(newSupportPoint);
			if (status == GJK3DStatus::FINISHED)
			{
				m_directionFromAToB = GetDistanceBetweenShapes();

				// Shapes completely overlap
				if (m_directionFromAToB == glm::dvec3(0.0f))
				{
					const glm::dvec3& direction = m_deltahedron->GetSearchDirection();
					glm::dvec3 result
					{
						Math::Sign(direction.x),
						Math::Sign(direction.y),
						Math::Sign(direction.z),
					};
					
					m_distanceBetweenAToB = -0.1f;
					m_directionFromAToB = result * m_distanceBetweenAToB;
				}
				else
				{
					m_distanceBetweenAToB = m_deltahedron->GetClosestDistanceToOrigin();
				}

				return m_distanceBetweenAToB <= 0.0f;
			}

			if (status == NOT_OVERLAP)
			{
				return false;
			}
		}

		return false;
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

	glm::dvec3 GJKEPA3D::GetDistanceBetweenShapes()
	{
		const GJK3DTriangle* closestTriangle = m_deltahedron->GetClosestTriangleToOrigin();
		glm::dvec3 baryCentric = m_deltahedron->GetBarycentric();

		glm::dvec3& closestPointOnA = GetPointFromShape(m_shapeA, closestTriangle, baryCentric);
		glm::dvec3& closestPointOnB = GetPointFromShape(m_shapeB, closestTriangle, baryCentric);

		return closestPointOnB - closestPointOnA;
	}
	
	glm::dvec3 GJKEPA3D::GetPointFromShape(ShapeInfo& shape, const GJK3DTriangle* triangle, glm::dvec3 baryCentric)
	{
		glm::dvec3 a = shape.pointMap[triangle->GetA()];
		glm::dvec3 b = shape.pointMap[triangle->GetB()];
		glm::dvec3 c = shape.pointMap[triangle->GetC()];

		glm::mat3 matrix(a, b, c);

		return matrix * baryCentric;
	}
	
	glm::dvec3 GJKEPA3D::GetPointFromShape(const ShapeInfo& shape, glm::dvec3 direction)
	{
		glm::dvec3 supportPointDirection = glm::dvec3(glm::dvec4(direction, 1.0) * glm::transpose(shape.orientation));
		glm::dvec3 point = shape.boundingValue.GetSupportPoint(supportPointDirection);
		point = glm::dvec3(glm::dvec4(point, 1.0) * shape.orientation * shape.scale);
		point += shape.position;
		
		return point;
	}
}
