#include "enginepch.h"
#include "GJKEPA3D.h"
#include "Engine/Math/Math.h"

namespace Engine
{
	bool GJKEPA3D::Detect(Transform transformA, Transform transformB, BoundingValue boundingValueA, BoundingValue boundingValueB)
	{
		m_distanceBetweenAToB = 0;
		m_directionFromAToB = {};

		// Convex A
		m_shapeA.position = transformA.translation;
		m_shapeA.orientation = glm::toMat3(glm::quat(transformA.rotation));
		m_shapeA.transform = glm::toMat4(glm::quat(transformA.rotation)) *
					   glm::scale(glm::mat4(1.0f), transformA.scale * boundingValueA.extents);
		m_shapeA.boundingValue = boundingValueA;
		m_shapeA.pointMap.clear();

		// Convex B
		m_shapeB.position = transformB.translation;
		m_shapeB.orientation = glm::toMat3(glm::quat(transformB.rotation));
		m_shapeB.transform = glm::toMat4(glm::quat(transformB.rotation)) *
					   glm::scale(glm::mat4(1.0f), transformB.scale * boundingValueB.extents);
		m_shapeB.boundingValue = boundingValueB;
		m_shapeB.pointMap.clear();

		// Create Deltahedron
		glm::dvec3 center = m_shapeA.position - m_shapeB.position;
		glm::dvec3 pointA = V0 + center;
		glm::dvec3 pointB = V1 + center;
		glm::dvec3 pointC = V2 + center;
		glm::dvec3 pointD = V3 + center;
		m_deltahedron = CreateUniq<GJK3DDeltahedron>(pointA, pointB, pointC, pointD);

		return Solve();
	}
	
	bool GJKEPA3D::Solve()
	{
		uint32_t iteration = 0;
		while (++iteration < MAX_ITERATION)
		{
			const glm::dvec3 newSupportPoint = CreateNewSupportPoint();
			GJK3DStatus status = m_deltahedron->ExpandWithNewPoint(newSupportPoint);
			if (status == GJK3DStatus::OVERLAP)
			{
				m_directionFromAToB = GetDistanceBetweenShapes();

				// Shapes completely overlap
				if (m_directionFromAToB == glm::vec3(0.0f))
				{
					const glm::dvec3& direction = m_deltahedron->GetSearchDirection();
					glm::vec3 result
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

				return true;
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
		glm::dvec3 supportPointDirection = direction * glm::transpose(shape.orientation);
		glm::dvec3 point = shape.boundingValue.GetSupportPoint(supportPointDirection);
		point = glm::vec3(glm::vec4(point, 1.0) * shape.transform);
		point += shape.position;
		
		return point;
	}
}
