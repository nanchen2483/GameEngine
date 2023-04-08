#include "enginepch.h"
#include "GJKEPA3D.h"
#include "Engine/Math/Math.h"

namespace Engine
{
	bool GJKEPA3D::Detect(Transform transformA, Transform transformB, BoundingValue boundingValueA, BoundingValue boundingValueB)
	{
		m_distanceBetweenAToB = 0;
		m_directionFromAToB = {};

		m_convexA.position = transformA.translation;
		m_convexB.position = transformB.translation;
		m_convexA.orientation = glm::toMat3(glm::quat(transformA.rotation));
		m_convexB.orientation = glm::toMat3(glm::quat(transformB.rotation));
		m_convexA.transform = glm::toMat4(glm::quat(transformA.rotation)) *
					   glm::scale(glm::mat4(1.0f), transformA.scale * boundingValueA.extents);
		m_convexB.transform = glm::toMat4(glm::quat(transformB.rotation)) *
					   glm::scale(glm::mat4(1.0f), transformB.scale * boundingValueB.extents);
		m_convexA.boundingValue = boundingValueA;
		m_convexB.boundingValue = boundingValueB;

		glm::dvec3 center = m_convexA.position - m_convexB.position;
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
			const Vertex3D newSupportPoint = CreateNewSupportPoint();
			GJK3DTriangle* triangleToBeReplaced = m_deltahedron->GetTriangleToBeReplaced(newSupportPoint);
			if (triangleToBeReplaced != nullptr)
			{
				// Not overlap
				bool expanded = m_deltahedron->ExpandDeltahedron(triangleToBeReplaced, newSupportPoint);
				if (!expanded)
				{
					return false;
				}
			}
			else
			{
				// Overlap
				const GJK3DTriangle* closestTriangle = m_deltahedron->GetClosestTriangleToOrigin();
				glm::dvec3 barycentric = m_deltahedron->GetBarycentric();
				glm::dvec3 closestPointOnA = CalcPointA(closestTriangle, barycentric);
				glm::dvec3 closestPointOnB = CalcPointB(closestTriangle, barycentric);
				m_directionFromAToB = glm::vec3(closestPointOnB - closestPointOnA);

				// Shapes completely overlap
				if (m_directionFromAToB == glm::vec3(0.0f))
				{
					glm::dvec3 direction = m_deltahedron->GetSearchDirection();

					glm::vec3 result{};
					result.x = Math::Sign(direction.x);
					result.y = Math::Sign(direction.y);
					result.z = Math::Sign(direction.z);
					
					m_distanceBetweenAToB = -0.1f;
					m_directionFromAToB = result * m_distanceBetweenAToB;
				}
				else
				{
					m_distanceBetweenAToB = m_deltahedron->GetClosestDistanceToOrigin();
				}

				return true;
			}
		}

		return false;
	}

	Vertex3D GJKEPA3D::CreateNewSupportPoint()
	{
		glm::dvec3 direction = m_deltahedron->GetSearchDirection();

		glm::dvec3 pointA = GetSupportPointOnA(-direction);
		glm::dvec3 pointB = GetSupportPointOnB(direction);
		Vertex3D supportPoint = m_deltahedron->AddSupportPoint(pointA - pointB);
		
		m_convexA.pointMap[supportPoint.id] = pointA;
		m_convexB.pointMap[supportPoint.id] = pointB;
		
		return supportPoint;
	}
	
	glm::dvec3 GJKEPA3D::CalcPointA(const GJK3DTriangle* triangle, glm::dvec3 baryCentric)
	{
		glm::dvec3 a = m_convexA.pointMap[triangle->GetA().id];
		glm::dvec3 b = m_convexA.pointMap[triangle->GetB().id];
		glm::dvec3 c = m_convexA.pointMap[triangle->GetC().id];

		glm::mat3 matrix(a, b, c);

		return matrix * baryCentric;
	}

	glm::dvec3 GJKEPA3D::CalcPointB(const GJK3DTriangle* triangle, glm::dvec3 baryCentric)
	{
		glm::dvec3 a = m_convexB.pointMap[triangle->GetA().id];
		glm::dvec3 b = m_convexB.pointMap[triangle->GetB().id];
		glm::dvec3 c = m_convexB.pointMap[triangle->GetC().id];

		glm::mat3 matrix(a, b, c);

		return matrix * baryCentric;
	}
	
	glm::dvec3 GJKEPA3D::GetSupportPointOnA(glm::dvec3 direction)
	{
		glm::dvec3 supportPointDirection = direction * glm::transpose(m_convexA.orientation);
		glm::dvec3 point = m_convexA.boundingValue.GetSupportPoint(supportPointDirection);
		point = glm::vec3(glm::vec4(point, 1.0) * m_convexA.transform);
		point += m_convexA.position;
		
		return point;
	}
	
	glm::dvec3 GJKEPA3D::GetSupportPointOnB(glm::dvec3 direction)
	{
		glm::dvec3 supportPointDirection = direction * glm::transpose(m_convexB.orientation);
		glm::dvec3 point = m_convexB.boundingValue.GetSupportPoint(supportPointDirection);
		point = glm::vec3(glm::vec4(point, 1.0) * m_convexB.transform);
		point += m_convexB.position;

		return point;
	}
}
