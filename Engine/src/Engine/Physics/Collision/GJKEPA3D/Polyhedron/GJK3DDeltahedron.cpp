#include "enginepch.h"
#include "GJK3DDeltahedron.h"

namespace Engine
{
	GJK3DDeltahedron::GJK3DDeltahedron(const glm::dvec3& pointA, const glm::dvec3& pointB, const glm::dvec3& pointC, const glm::dvec3& pointD)
		: m_triangleHead(nullptr), m_numOfSupportPoint(0), m_numOfExpandedTriagnles(0), m_originEnclosed(false)
	{
		GJK3DTriangle* frontTriangle = CreateTriangle(pointA, pointC, pointB);
		GJK3DTriangle* leftTriangle = CreateTriangle(pointA, pointB, pointD);
		GJK3DTriangle* rightTriangle = CreateTriangle(pointA, pointD, pointC);
		GJK3DTriangle* bottomTriangle = CreateTriangle(pointB, pointC, pointD);

		frontTriangle->SetNeighbors(leftTriangle, rightTriangle, bottomTriangle);
		leftTriangle->SetNeighbors(rightTriangle, frontTriangle, bottomTriangle);
		rightTriangle->SetNeighbors(frontTriangle, leftTriangle, bottomTriangle);
		bottomTriangle->SetNeighbors(leftTriangle, frontTriangle, rightTriangle);
	}

	GJK3DDeltahedron::~GJK3DDeltahedron()
	{
		while (m_triangleHead != nullptr)
		{
			GJK3DTriangle* nextTriangle = m_triangleHead->GetNextTriangle();
			delete m_triangleHead;
			m_triangleHead = nextTriangle;
		}
	}

	glm::dvec3 GJK3DDeltahedron::GetSearchDirection()
	{
		glm::dvec3 searchDirection = m_triangleHead->GetClosestPointToOrigin();
		if (m_originEnclosed)
		{
			return -searchDirection;
		}

		return searchDirection;
	}

	float GJK3DDeltahedron::GetClosestDistanceToOrigin()
	{
		float distance = m_triangleHead->GetClosestDistanceToOrigin();
		if (m_originEnclosed)
		{
			return -distance;
		}

		return distance;
	}

	GJK3DTriangle* GJK3DDeltahedron::GetTriangleToBeReplaced(const glm::dvec3& newSupportPoint)
	{
		if (!IsValidSupportPoint(newSupportPoint))
		{
			return nullptr;
		}

		AddSupportPoint(newSupportPoint);

		for (GJK3DTriangle* triangle = m_triangleHead; triangle != nullptr; triangle = triangle->GetNextTriangle())
		{
			if (InTheSameDirection(triangle, newSupportPoint))
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

		GJK3DTriangle* closestTriangle = m_triangleHead;
		double deltaDistance = closestTriangle->GetClosestDistanceToOriginSquare() - glm::dot((glm::dvec3)newSupportPoint, closestTriangle->GetClosestPointToOrigin());
		bool isCloseEnough = deltaDistance * deltaDistance < COLLIDE_EPSILON * COLLIDE_EPSILON* closestTriangle->GetClosestDistanceToOriginSquare();
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

	glm::dvec3 GJK3DDeltahedron::GetBarycentric()
	{
		return m_triangleHead->GetBarycentric(m_originEnclosed);
	}

	const glm::dvec3& GJK3DDeltahedron::AddSupportPoint(glm::dvec3 newSupportPoint)
	{
		const int32_t index = m_numOfSupportPoint;
		m_supportPoints[index] = newSupportPoint;
		m_numOfSupportPoint += 1;

		return m_supportPoints[index];
	}
	
	GJK3DTriangle* GJK3DDeltahedron::CreateTriangle(const glm::dvec3& pointA, const glm::dvec3& pointB, const glm::dvec3& pointC)
	{
		GJK3DTriangle* currentTriangle = new GJK3DTriangle(pointA, pointB, pointC);
		currentTriangle->SetClosestPointToOrigin(m_originEnclosed);

		SortTriangleByDistanceToOrigin(currentTriangle);

		return currentTriangle;
	}

	void GJK3DDeltahedron::SortTriangleByDistanceToOrigin(GJK3DTriangle* triangle)
	{
		GJK3DTriangle* currentTriangle = nullptr;
		for (GJK3DTriangle* nextTriangle = m_triangleHead; nextTriangle != nullptr; nextTriangle = currentTriangle->GetNextTriangle())
		{
			currentTriangle = nextTriangle;
			if (triangle->GetClosestDistanceToOriginSquare() < currentTriangle->GetClosestDistanceToOriginSquare())
			{
				GJK3DTriangle* previousTraingle = currentTriangle->GetPreviousTriangle();
				triangle->SetNextTriangle(currentTriangle);
				triangle->SetPreviousTriangle(previousTraingle);
				currentTriangle->SetPreviousTriangle(triangle);
				if (previousTraingle != nullptr)
				{
					previousTraingle->SetNextTriangle(triangle);
				}
				else
				{
					m_triangleHead = triangle;
				}

				return;
			}
		}

		triangle->SetPreviousTriangle(currentTriangle);
		if (currentTriangle != nullptr)
		{
			currentTriangle->SetNextTriangle(triangle);
		}
		else
		{
			m_triangleHead = triangle;
		}
	}

	bool GJK3DDeltahedron::ExpandDeltahedron(GJK3DTriangle* removeTriangle, const glm::dvec3& newPoint)
	{
		ExpandWithNewPoint(removeTriangle, newPoint);
		UpdateOriginEnclosed(removeTriangle);
		delete removeTriangle;

		return UpdateNeighbors();
	}

	void GJK3DDeltahedron::ExpandWithNewPoint(GJK3DTriangle* removeTriangle, const glm::dvec3& newPoint)
	{
		if (removeTriangle->IsDeleted())
		{
			return;
		}

		// Soft-delete
		RemoveTriangle(removeTriangle);

		GJK3DTriangle* leftTriangle = removeTriangle->GetLeftTriangle();
		bool leftTriangleShouldBeRemoved = InTheSameDirection(leftTriangle, newPoint);
		if (!leftTriangleShouldBeRemoved)
		{
			GJK3DTriangle* newTriangle = CreateTriangle(removeTriangle->GetC(), removeTriangle->GetA(), newPoint);
			newTriangle->SetRightTriangle(leftTriangle);
			m_expandedTriangles[m_numOfExpandedTriagnles++] = newTriangle;

			if (leftTriangle->GetLeftTriangle() == removeTriangle)
			{
				leftTriangle->SetLeftTriangle(newTriangle);
			}
			else if (leftTriangle->GetRightTriangle() == removeTriangle)
			{
				leftTriangle->SetRightTriangle(newTriangle);
			}
			else
			{
				leftTriangle->SetBottomTriangle(newTriangle);
			}
		}

		GJK3DTriangle* rightTriangle = removeTriangle->GetRightTriangle();
		bool rightTriangleShouldBeRemoved = InTheSameDirection(rightTriangle, newPoint);
		if (!rightTriangleShouldBeRemoved)
		{
			GJK3DTriangle* newTriangle = CreateTriangle(removeTriangle->GetA(), removeTriangle->GetB(), newPoint);
			newTriangle->SetRightTriangle(rightTriangle);
			m_expandedTriangles[m_numOfExpandedTriagnles++] = newTriangle;

			if (rightTriangle->GetLeftTriangle() == removeTriangle)
			{
				rightTriangle->SetLeftTriangle(newTriangle);
			}
			else if (rightTriangle->GetRightTriangle() == removeTriangle)
			{
				rightTriangle->SetRightTriangle(newTriangle);
			}
			else
			{
				rightTriangle->SetBottomTriangle(newTriangle);
			}
		}

		GJK3DTriangle* bottomTriangle = removeTriangle->GetBottomTriangle();
		bool bottomTriangleShouldBeRemoved = InTheSameDirection(bottomTriangle, newPoint);
		if (!bottomTriangleShouldBeRemoved)
		{
			GJK3DTriangle* newTriangle = CreateTriangle(removeTriangle->GetB(), removeTriangle->GetC(), newPoint);
			newTriangle->SetRightTriangle(bottomTriangle);
			m_expandedTriangles[m_numOfExpandedTriagnles++] = newTriangle;

			if (bottomTriangle->GetLeftTriangle() == removeTriangle)
			{
				bottomTriangle->SetLeftTriangle(newTriangle);
			}
			else if (bottomTriangle->GetRightTriangle() == removeTriangle)
			{
				bottomTriangle->SetRightTriangle(newTriangle);
			}
			else
			{
				bottomTriangle->SetBottomTriangle(newTriangle);
			}
		}

		if (leftTriangleShouldBeRemoved)
		{
			ExpandWithNewPoint(leftTriangle, newPoint);
		}

		if (rightTriangleShouldBeRemoved)
		{
			ExpandWithNewPoint(rightTriangle, newPoint);
		}

		if (bottomTriangleShouldBeRemoved)
		{
			ExpandWithNewPoint(bottomTriangle, newPoint);
		}
	}

	void GJK3DDeltahedron::RemoveTriangle(GJK3DTriangle* removeTriangle)
	{
		GJK3DTriangle* previousTriangle = removeTriangle->GetPreviousTriangle();
		GJK3DTriangle* nextTriangle = removeTriangle->GetNextTriangle();

		if (previousTriangle != nullptr)
		{
			previousTriangle->SetNextTriangle(nextTriangle);
		}
		else
		{
			m_triangleHead = nextTriangle;
		}

		if (nextTriangle != nullptr)
		{
			nextTriangle->SetPreviousTriangle(previousTriangle);
		}

		removeTriangle->MarkAsDeleted();
	}
	
	bool GJK3DDeltahedron::InTheSameDirection(const GJK3DTriangle* triangle, glm::dvec3 point)
	{
		glm::dvec3 direction = point - triangle->GetA();
		return glm::dot(direction, triangle->GetNormalVector()) > 0;
	}
	
	void GJK3DDeltahedron::UpdateOriginEnclosed(const GJK3DTriangle* removeTriangle)
	{
		if (!m_originEnclosed)
		{
			for (uint32_t i = 0; i < m_numOfExpandedTriagnles; ++i)
			{
				const GJK3DTriangle* triangle = m_expandedTriangles[i];
				bool originNotEnclosed = glm::dot(triangle->GetNormalVector(), (glm::dvec3)triangle->GetA()) < 0.0;
				if (originNotEnclosed)
				{
					return;
				}
			}

			m_originEnclosed = glm::dot(removeTriangle->GetNormalVector(), (glm::dvec3)removeTriangle->GetA()) < 0.0;
		}
	}
	
	bool GJK3DDeltahedron::UpdateNeighbors()
	{
		GJK3DTriangle* firstTriangle = m_expandedTriangles[0];
		GJK3DTriangle* lastTriangle = firstTriangle;

		m_expandedTriangles[0] = m_expandedTriangles[--m_numOfExpandedTriagnles];

		while (m_numOfExpandedTriagnles > 0)
		{
			int32_t foundTriangleIndex = -1;
			for (int32_t i = 0; i < m_numOfExpandedTriagnles; i++)
			{
				if (m_expandedTriangles[i]->GetA() == lastTriangle->GetB())
				{
					foundTriangleIndex = i;
					break;
				}
			}

			if (foundTriangleIndex == -1)
			{
				return false;
			}

			lastTriangle->SetBottomTriangle(m_expandedTriangles[foundTriangleIndex]);
			m_expandedTriangles[foundTriangleIndex]->SetLeftTriangle(lastTriangle);

			lastTriangle = m_expandedTriangles[foundTriangleIndex];
			m_expandedTriangles[foundTriangleIndex] = m_expandedTriangles[--m_numOfExpandedTriagnles];
		}

		firstTriangle->SetLeftTriangle(lastTriangle);
		lastTriangle->SetBottomTriangle(firstTriangle);

		return true;
	}
}
