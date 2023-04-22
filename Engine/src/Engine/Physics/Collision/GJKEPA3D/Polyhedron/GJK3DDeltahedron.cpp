#include "enginepch.h"
#include "GJK3DDeltahedron.h"

namespace Engine
{
	GJK3DDeltahedron::GJK3DDeltahedron(const glm::dvec3& pointA, const glm::dvec3& pointB, const glm::dvec3& pointC, const glm::dvec3& pointD)
		: m_numOfSupportPoint(0), m_numOfExpandedTriagnles(0), m_originEnclosed(false)
	{
		const Ptr<GJK3DTriangle>& frontTriangle = CreateTriangle(pointA, pointC, pointB);
		const Ptr<GJK3DTriangle>& leftTriangle = CreateTriangle(pointA, pointB, pointD);
		const Ptr<GJK3DTriangle>& rightTriangle = CreateTriangle(pointA, pointD, pointC);
		const Ptr<GJK3DTriangle>& bottomTriangle = CreateTriangle(pointB, pointC, pointD);

		frontTriangle->SetNeighbors(leftTriangle, rightTriangle, bottomTriangle);
		leftTriangle->SetNeighbors(rightTriangle, frontTriangle, bottomTriangle);
		rightTriangle->SetNeighbors(frontTriangle, leftTriangle, bottomTriangle);
		bottomTriangle->SetNeighbors(leftTriangle, frontTriangle, rightTriangle);
	}

	const Ptr<GJK3DTriangle> GJK3DDeltahedron::CreateTriangle(const glm::dvec3& vectorA, const glm::dvec3& vectorB, const glm::dvec3& vectorC)
	{
		Ptr<GJK3DTriangle> newTriangle = CreatePtr<GJK3DTriangle>(vectorA, vectorB, vectorC, m_originEnclosed);
		m_triangles.Add(newTriangle);

		return newTriangle;
	}

	double GJK3DDeltahedron::GetClosestDistanceToOrigin()
	{
		double distance = m_triangles.GetHeadValue()->GetClosestDistanceToOrigin();
		if (m_originEnclosed)
		{
			return -distance;
		}

		return distance;
	}

	const glm::dvec3 GJK3DDeltahedron::GetBarycentric()
	{
		return m_triangles.GetHeadValue()->GetBarycentric(m_originEnclosed);
	}

	const glm::dvec3 GJK3DDeltahedron::GetSearchDirection()
	{
		glm::dvec3 searchDirection = m_triangles.GetHeadValue()->GetClosestPointToOrigin();
		if (m_originEnclosed)
		{
			return -searchDirection;
		}

		return searchDirection;
	}

	GJK3DStatus GJK3DDeltahedron::ExpandWithNewPoint(const glm::dvec3& newPoint)
	{
		if (!IsValidSupportPoint(newPoint))
		{
			return GJK3DStatus::FINISHED;
		}

		AddSupportPoint(newPoint);
		const Ptr<GJK3DTriangle> expandTriangle = GetTriangleToBeExpanded(newPoint);
		if (expandTriangle == nullptr)
		{
			return GJK3DStatus::FINISHED;
		}

		ExpandWithNewPoint(newPoint, expandTriangle);
		UpdateOriginEnclosed(expandTriangle);

		return UpdateNeighbors();
	}

	void GJK3DDeltahedron::AddSupportPoint(const glm::dvec3& newSupportPoint)
	{
		uint32_t index = m_numOfSupportPoint;
		m_supportPoints[index] = newSupportPoint;
		m_numOfSupportPoint += 1;
	}

	const Ptr<GJK3DTriangle> GJK3DDeltahedron::GetTriangleToBeExpanded(const glm::dvec3& newSupportPoint)
	{
		for (const Ptr<GJK3DTriangle>& triangle : m_triangles)
		{
			if (triangle->IsExpandable(newSupportPoint))
			{
				return triangle;
			}
		}

		return nullptr;
	}

	bool GJK3DDeltahedron::IsValidSupportPoint(const glm::dvec3& newSupportPoint)
	{
		if (AlreadyExists(newSupportPoint))
		{
			return false;
		}

		const Ptr<GJK3DTriangle> closestTriangle = m_triangles.GetHeadValue();
		double deltaDistance = closestTriangle->GetClosestDistanceToOriginSquare() - glm::dot(newSupportPoint, closestTriangle->GetClosestPointToOrigin());
		bool isCloseEnough = deltaDistance * deltaDistance < COLLIDE_EPSILON * COLLIDE_EPSILON * closestTriangle->GetClosestDistanceToOriginSquare();
		if (isCloseEnough)
		{
			return false;
		}

		return true;
	}

	bool GJK3DDeltahedron::AlreadyExists(const glm::dvec3& newSupportPoint)
	{
		for (uint32_t i = 0; i < m_numOfSupportPoint; ++i)
		{
			const glm::dvec3& currentSupportPoint = m_supportPoints[i];
			if (std::abs(currentSupportPoint.x - newSupportPoint.x) > NUMERIC_EPSILON) continue;
			if (std::abs(currentSupportPoint.y - newSupportPoint.y) > NUMERIC_EPSILON) continue;
			if (std::abs(currentSupportPoint.z - newSupportPoint.z) > NUMERIC_EPSILON) continue;

			return true;
		}

		return false;
	}

	void GJK3DDeltahedron::UpdateOriginEnclosed(const Ptr<GJK3DTriangle>& removeTriangle)
	{
		if (!m_originEnclosed)
		{
			for (uint32_t i = 0; i < m_numOfExpandedTriagnles; ++i)
			{
				const Ptr<GJK3DTriangle> triangle = m_expandedTriangles[i].lock();
				bool originNotEnclosed = glm::dot(triangle->GetNormalVector(), triangle->GetA()) < 0.0;
				if (originNotEnclosed)
				{
					return;
				}
			}

			m_originEnclosed = glm::dot(removeTriangle->GetNormalVector(), removeTriangle->GetA()) < 0.0;
		}
	}

	void GJK3DDeltahedron::ExpandWithNewPoint(const glm::dvec3& newPoint, const Ptr<GJK3DTriangle>& expandTriangle)
	{
		if (expandTriangle->IsDeleted())
		{
			return;
		}

		// Soft-delete
		m_triangles.Delete(expandTriangle);
		expandTriangle->MarkAsDeleted();

		const Ptr<GJK3DTriangle>& leftTriangle = expandTriangle->GetLeftTriangle();
		bool leftTriangleShouldBeExpanded = leftTriangle->IsExpandable(newPoint);
		if (!leftTriangleShouldBeExpanded)
		{
			const Ptr<GJK3DTriangle>& newTriangle = CreateTriangle(expandTriangle->GetC(), expandTriangle->GetA(), newPoint);
			newTriangle->SetRightTriangle(leftTriangle);
			m_expandedTriangles[m_numOfExpandedTriagnles++] = newTriangle;

			if (leftTriangle->GetLeftTriangle() == expandTriangle)
			{
				leftTriangle->SetLeftTriangle(newTriangle);
			}
			else if (leftTriangle->GetRightTriangle() == expandTriangle)
			{
				leftTriangle->SetRightTriangle(newTriangle);
			}
			else
			{
				leftTriangle->SetBottomTriangle(newTriangle);
			}
		}

		const Ptr<GJK3DTriangle>& rightTriangle = expandTriangle->GetRightTriangle();
		bool rightTriangleShouldBeExpanded = rightTriangle->IsExpandable(newPoint);
		if (!rightTriangleShouldBeExpanded)
		{
			const Ptr<GJK3DTriangle>& newTriangle = CreateTriangle(expandTriangle->GetA(), expandTriangle->GetB(), newPoint);
			newTriangle->SetRightTriangle(rightTriangle);
			m_expandedTriangles[m_numOfExpandedTriagnles++] = newTriangle;

			if (rightTriangle->GetLeftTriangle() == expandTriangle)
			{
				rightTriangle->SetLeftTriangle(newTriangle);
			}
			else if (rightTriangle->GetRightTriangle() == expandTriangle)
			{
				rightTriangle->SetRightTriangle(newTriangle);
			}
			else
			{
				rightTriangle->SetBottomTriangle(newTriangle);
			}
		}

		const Ptr<GJK3DTriangle>& bottomTriangle = expandTriangle->GetBottomTriangle();
		bool bottomTriangleShouldBeExpanded = bottomTriangle->IsExpandable(newPoint);
		if (!bottomTriangleShouldBeExpanded)
		{
			const Ptr<GJK3DTriangle>& newTriangle = CreateTriangle(expandTriangle->GetB(), expandTriangle->GetC(), newPoint);
			newTriangle->SetRightTriangle(bottomTriangle);
			m_expandedTriangles[m_numOfExpandedTriagnles++] = newTriangle;

			if (bottomTriangle->GetLeftTriangle() == expandTriangle)
			{
				bottomTriangle->SetLeftTriangle(newTriangle);
			}
			else if (bottomTriangle->GetRightTriangle() == expandTriangle)
			{
				bottomTriangle->SetRightTriangle(newTriangle);
			}
			else
			{
				bottomTriangle->SetBottomTriangle(newTriangle);
			}
		}

		if (leftTriangleShouldBeExpanded)
		{
			ExpandWithNewPoint(newPoint, leftTriangle);
		}

		if (rightTriangleShouldBeExpanded)
		{
			ExpandWithNewPoint(newPoint, rightTriangle);
		}

		if (bottomTriangleShouldBeExpanded)
		{
			ExpandWithNewPoint(newPoint, bottomTriangle);
		}
	}
	
	GJK3DStatus GJK3DDeltahedron::UpdateNeighbors()
	{
		const Ptr<GJK3DTriangle> firstTriangle = m_expandedTriangles[0].lock();
		Ptr<GJK3DTriangle> lastTriangle = firstTriangle;

		m_expandedTriangles[0] = m_expandedTriangles[--m_numOfExpandedTriagnles];

		while (m_numOfExpandedTriagnles > 0)
		{
			int32_t foundTriangleIndex = -1;
			for (int32_t i = 0; i < m_numOfExpandedTriagnles; i++)
			{
				if (m_expandedTriangles[i].lock()->GetA() == lastTriangle->GetB())
				{
					foundTriangleIndex = i;
					break;
				}
			}

			if (foundTriangleIndex == -1)
			{
				return GJK3DStatus::NOT_OVERLAP;
			}

			const Ptr<GJK3DTriangle> foundTriangle = m_expandedTriangles[foundTriangleIndex].lock();
			lastTriangle->SetBottomTriangle(foundTriangle);
			foundTriangle->SetLeftTriangle(lastTriangle);

			lastTriangle = foundTriangle;
			m_expandedTriangles[foundTriangleIndex] = m_expandedTriangles[--m_numOfExpandedTriagnles];
		}

		firstTriangle->SetLeftTriangle(lastTriangle);
		lastTriangle->SetBottomTriangle(firstTriangle);

		return GJK3DStatus::NOT_FINISHED;
	}
}
