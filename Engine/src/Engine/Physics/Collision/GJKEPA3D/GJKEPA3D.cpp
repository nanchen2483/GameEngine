#include "enginepch.h"
#include "GJKEPA3D.h"
#include "Engine/Math/Math.h"

namespace Engine
{
	bool GJKEPA3D::Detect(Transform transformA, Transform transformB, BoundingValue boundingValueA, BoundingValue boundingValueB)
	{
		m_distanceBetweenAToB = 0;
		m_directionFromAToB = {};

		m_objectA.position = transformA.translation;
		m_objectB.position = transformB.translation;
		m_objectA.orientation = glm::toMat3(glm::quat(transformA.rotation));
		m_objectB.orientation = glm::toMat3(glm::quat(transformB.rotation));
		m_objectA.transform = glm::toMat4(glm::quat(transformA.rotation)) *
					   glm::scale(glm::mat4(1.0f), transformA.scale * boundingValueA.extents);
		m_objectB.transform = glm::toMat4(glm::quat(transformB.rotation)) *
					   glm::scale(glm::mat4(1.0f), transformB.scale * boundingValueB.extents);
		m_objectA.boundingValue = boundingValueA;
		m_objectB.boundingValue = boundingValueB;

		glm::dvec3 center = m_objectA.position - m_objectB.position;
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
		
		m_objectA.pointMap[supportPoint.id] = pointA;
		m_objectB.pointMap[supportPoint.id] = pointB;
		
		return supportPoint;
	}
	
	glm::dvec3 GJKEPA3D::CalcPointA(const GJK3DTriangle* triangle, glm::dvec3 baryCentric)
	{
		glm::dvec3 a = m_objectA.pointMap[triangle->GetA().id];
		glm::dvec3 b = m_objectA.pointMap[triangle->GetB().id];
		glm::dvec3 c = m_objectA.pointMap[triangle->GetC().id];

		glm::mat3 matrix(a, b, c);

		return matrix * baryCentric;
	}

	glm::dvec3 GJKEPA3D::CalcPointB(const GJK3DTriangle* triangle, glm::dvec3 baryCentric)
	{
		glm::dvec3 a = m_objectB.pointMap[triangle->GetA().id];
		glm::dvec3 b = m_objectB.pointMap[triangle->GetB().id];
		glm::dvec3 c = m_objectB.pointMap[triangle->GetC().id];

		glm::mat3 matrix(a, b, c);

		return matrix * baryCentric;
	}
	
	glm::dvec3 GJKEPA3D::GetSupportPointOnA(glm::dvec3 direction)
	{
		glm::dvec3 supportPointDirection = direction * glm::transpose(m_objectA.orientation);
		glm::dvec3 point = m_objectA.boundingValue.GetSupportPoint(supportPointDirection);
		point = glm::vec3(glm::vec4(point, 1.0) * m_objectA.transform);
		point += m_objectA.position;
		
		return point;
	}
	
	glm::dvec3 GJKEPA3D::GetSupportPointOnB(glm::dvec3 direction)
	{
		glm::dvec3 supportPointDirection = direction * glm::transpose(m_objectB.orientation);
		glm::dvec3 point = m_objectB.boundingValue.GetSupportPoint(supportPointDirection);
		point = glm::vec3(glm::vec4(point, 1.0) * m_objectB.transform);
		point += m_objectB.position;

		return point;
	}
}
