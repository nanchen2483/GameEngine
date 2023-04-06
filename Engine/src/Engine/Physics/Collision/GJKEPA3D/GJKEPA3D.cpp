#include "enginepch.h"
#include "GJKEPA3D.h"
#include "Engine/Math/Math.h"

namespace Engine
{
	GJKEPA3D::GJKEPA3D()
	{
		m_deltahedron = CreateUniq<GJK3DDeltahedron>();
		m_pointOnAMap[-1] = glm::dvec3(0.0);
		m_pointOnBMap[-1] = glm::dvec3(0.0);
	}

	bool GJKEPA3D::Detect(Transform transformA, Transform transformB, BoundingValue boundingValueA, BoundingValue boundingValueB)
	{
		m_distanceBetweenAToB = 0;
		m_directionFromAToB = {};

		m_positionA = transformA.translation;
		m_positionB = transformB.translation;
		m_orientationA = glm::toMat3(glm::quat(transformA.rotation));
		m_orientationB = glm::toMat3(glm::quat(transformB.rotation));
		m_transformA = glm::toMat4(glm::quat(transformA.rotation)) *
					   glm::scale(glm::mat4(1.0f), transformA.scale * boundingValueA.extents);
		m_transformB = glm::toMat4(glm::quat(transformB.rotation)) *
					   glm::scale(glm::mat4(1.0f), transformB.scale * boundingValueB.extents);
		m_boundingValueA = boundingValueA;
		m_boundingValueB = boundingValueB;

		glm::dvec3 center = SupportCenter();
		glm::dvec3 a = V0 + center;
		glm::dvec3 b = V1 + center;
		glm::dvec3 c = V2 + center;
		glm::dvec3 d = V3 + center;
		m_deltahedron->CreateTetrahedron(a, b, c, d);

		return Solve();
	}
	
	bool GJKEPA3D::Solve()
	{
		uint32_t iteration = 0;
		while (++iteration < MAX_ITERATION)
		{
			glm::dvec3 direction = m_deltahedron->GetSearchDirection();
			const Vertex3D mewSupportPoint = CreateNewSupportPoint(direction);
			bool isValidPoint = m_deltahedron->IsValidSupportPoint(mewSupportPoint);
			int32_t removeTriangleIndex = m_deltahedron->GetTriangleShouldBeReplaced(mewSupportPoint);

			if (removeTriangleIndex == -1 || !isValidPoint)
			{
				m_distanceBetweenAToB = m_deltahedron->GetClosestDistanceToOrigin();
				
				const Uniq<GJK3DTriangle>& closestTriangle = m_deltahedron->GetClosestTriangleToOrigin();
				glm::dvec3 barycentric = m_deltahedron->GetBarycentric();
				glm::dvec3 closestPointOnA = CalcPointA(closestTriangle, barycentric);
				glm::dvec3 closestPointOnB = CalcPointB(closestTriangle, barycentric);
				m_directionFromAToB = glm::vec3(closestPointOnB - closestPointOnA);
				if (m_directionFromAToB == glm::vec3(0.0f))
				{
					// Shapes completely overlap
					glm::vec3 result;
					result.x = Math::Sign(direction.x);
					result.y = Math::Sign(direction.y);
					result.z = Math::Sign(direction.z);
					
					m_distanceBetweenAToB = -0.1f;
					m_directionFromAToB = result * m_distanceBetweenAToB;
				}

				return true;
			}

			bool expanded = m_deltahedron->ExpandDeltahedron(removeTriangleIndex, mewSupportPoint);
			if (!expanded)
			{
				return false;
			}
		}

		return false;
	}

	glm::dvec3 GJKEPA3D::SupportCenter()
	{
		return m_positionA - m_positionB;
	}
	
	Vertex3D GJKEPA3D::CreateNewSupportPoint(glm::dvec3 direction)
	{
		glm::dvec3 vertexA = GetSupportPointOnA(-direction);
		glm::dvec3 vertexB = GetSupportPointOnB(direction);
		Vertex3D supportPoint = m_deltahedron->AddSupportPoint(vertexA - vertexB);
		
		m_pointOnAMap[supportPoint.id] = vertexA;
		m_pointOnBMap[supportPoint.id] = vertexB;
		
		return supportPoint;
	}
	
	glm::dvec3 GJKEPA3D::CalcPointA(const Uniq<GJK3DTriangle>& triangle, glm::dvec3 baryCentric)
	{
		glm::dvec3 a = m_pointOnAMap[triangle->GetA().id];
		glm::dvec3 b = m_pointOnAMap[triangle->GetB().id];
		glm::dvec3 c = m_pointOnAMap[triangle->GetC().id];

		glm::mat3 matrix(a, b, c);

		return matrix * baryCentric;
	}

	glm::dvec3 GJKEPA3D::CalcPointB(const Uniq<GJK3DTriangle>& triangle, glm::dvec3 baryCentric)
	{
		glm::dvec3 a = m_pointOnBMap[triangle->GetA().id];
		glm::dvec3 b = m_pointOnBMap[triangle->GetB().id];
		glm::dvec3 c = m_pointOnBMap[triangle->GetC().id];

		glm::mat3 matrix(a, b, c);

		return matrix * baryCentric;
	}
	
	glm::dvec3 GJKEPA3D::GetSupportPointOnA(glm::dvec3 direction)
	{
		glm::dvec3 supportPointDirection = direction * glm::transpose(m_orientationA);
		glm::dvec3 point = m_boundingValueA.GetSupportPoint(supportPointDirection);
		point = glm::vec3(glm::vec4(point, 1.0) * m_transformA);
		point += m_positionA;
		
		return point;
	}
	
	glm::dvec3 GJKEPA3D::GetSupportPointOnB(glm::dvec3 direction)
	{
		glm::dvec3 supportPointDirection = direction * glm::transpose(m_orientationB);
		glm::dvec3 point = m_boundingValueB.GetSupportPoint(supportPointDirection);
		point = glm::vec3(glm::vec4(point, 1.0) * m_transformB);
		point += m_positionB;

		return point;
	}
}
