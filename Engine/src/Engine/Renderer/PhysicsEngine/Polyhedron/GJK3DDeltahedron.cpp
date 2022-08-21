#include "enginepch.h"
#include "GJK3DDeltahedron.h"

namespace Engine
{
	void GJK3DDeltahedron::CreateTetrahedron(const glm::dvec3& a, const glm::dvec3& b, const glm::dvec3& c, const glm::dvec3& d)
	{
		m_triangleHeadIndex = -1;
		m_numOfSupportPoint = 0;
		m_numOfTriagnles = 0;
		m_numOfExpandedTriagnles = 0;
		m_originEnclosed = false;

		const Vertex3D vertexA = { -1, a };
		const Vertex3D vertexB = { -1, b };
		const Vertex3D vertexC = { -1, c };
		const Vertex3D vertexD = { -1, d };
		
		const uint32_t frontTriangleIndex = CreateTriangle(vertexA, vertexC, vertexB);
		const uint32_t leftTriangleIndex = CreateTriangle(vertexA, vertexB, vertexD);
		const uint32_t rightTriangleIndex = CreateTriangle(vertexA, vertexD, vertexC);
		const uint32_t bottomTriangleIndex = CreateTriangle(vertexB, vertexC, vertexD);

		m_triangles[frontTriangleIndex]->SetNeighbors(leftTriangleIndex, rightTriangleIndex, bottomTriangleIndex);
		m_triangles[leftTriangleIndex]->SetNeighbors(rightTriangleIndex, frontTriangleIndex, bottomTriangleIndex);
		m_triangles[rightTriangleIndex]->SetNeighbors(frontTriangleIndex, leftTriangleIndex, bottomTriangleIndex);
		m_triangles[bottomTriangleIndex]->SetNeighbors(leftTriangleIndex, frontTriangleIndex, rightTriangleIndex);
	}

	glm::dvec3 GJK3DDeltahedron::GetSearchDirection()
	{
		glm::dvec3 searchDirection = m_triangles[m_triangleHeadIndex]->GetClosestPointToOrigin();
		if (m_originEnclosed)
		{
			return -searchDirection;
		}

		return searchDirection;
	}

	const Uniq<GJK3DTriangle>& GJK3DDeltahedron::GetClosestTriangleToOrigin()
	{
		return m_triangles[m_triangleHeadIndex];
	}

	float GJK3DDeltahedron::GetClosestDistanceToOrigin()
	{
		float distance = m_triangles[m_triangleHeadIndex]->GetClosestDistanceToOrigin();
		if (m_originEnclosed)
		{
			return -distance;
		}

		return distance;
	}

	int32_t GJK3DDeltahedron::GetTriangleShouldBeReplaced(const Vertex3D& newSupportPoint)
	{
		for (int32_t triangleIndex = m_triangleHeadIndex; triangleIndex != -1; triangleIndex = m_triangles[triangleIndex]->GetNextTriangle())
		{
			if (InTheSameDirection(triangleIndex, newSupportPoint))
			{
				return triangleIndex;
			}
		}

		return -1;
	}

	bool GJK3DDeltahedron::IsValidSupportPoint(const Vertex3D& newSupportPoint)
	{
		if (AlreadyExists(newSupportPoint))
		{
			return false;
		}

		const Uniq<GJK3DTriangle>& closestTriangle = m_triangles[m_triangleHeadIndex];
		double deltaDistance = closestTriangle->GetClosestDistanceToOriginSquare() - glm::dot((glm::dvec3)newSupportPoint, closestTriangle->GetClosestPointToOrigin());
		bool isCloseEnough = deltaDistance * deltaDistance < COLLIDE_EPSILON * COLLIDE_EPSILON* closestTriangle->GetClosestDistanceToOriginSquare();
		if (isCloseEnough)
		{
			return false;
		}

		return true;
	}

	bool GJK3DDeltahedron::AlreadyExists(const Vertex3D& vertex)
	{
		const Vertex3D newSupportPoint = m_supportPoints[vertex.id];
		for (uint32_t i = 0; i < m_numOfSupportPoint; ++i)
		{
			const Vertex3D currentSupportPoint = m_supportPoints[i];
			if (vertex.id != i)
			{
				if (std::abs(currentSupportPoint.x() - newSupportPoint.x()) > NUMERIC_EPSILON) continue;
				if (std::abs(currentSupportPoint.y() - newSupportPoint.y()) > NUMERIC_EPSILON) continue;
				if (std::abs(currentSupportPoint.z() - newSupportPoint.z()) > NUMERIC_EPSILON) continue;

				return true;
			}
		}

		return false;
	}

	glm::dvec3 GJK3DDeltahedron::GetBarycentric()
	{
		return m_triangles[m_triangleHeadIndex]->GetBarycentric(m_originEnclosed);
	}

	const Vertex3D& GJK3DDeltahedron::AddSupportPoint(glm::dvec3 vertex)
	{
		const int32_t index = m_numOfSupportPoint;
		m_supportPoints[index] = Vertex3D{ index, vertex };
		m_numOfSupportPoint += 1;

		return m_supportPoints[index];
	}
	
	uint32_t GJK3DDeltahedron::CreateTriangle(const Vertex3D& a, const Vertex3D& b, const Vertex3D& c)
	{
		const uint32_t currentTriangleIndex = m_numOfTriagnles;
		m_triangles[currentTriangleIndex] = CreateUniq<GJK3DTriangle>(a, b, c);
		m_triangles[currentTriangleIndex]->SetClosestPointToOrigin(m_originEnclosed);
		m_numOfTriagnles += 1;

		SortTriangleByDistanceToOrigin(currentTriangleIndex);

		return currentTriangleIndex;
	}

	void GJK3DDeltahedron::SortTriangleByDistanceToOrigin(const int32_t triangleIndex)
	{
		int32_t currentTriangleIndex = -1;
		for (int32_t nextTriangleIndex = m_triangleHeadIndex; nextTriangleIndex != -1; nextTriangleIndex = m_triangles[currentTriangleIndex]->GetNextTriangle())
		{
			currentTriangleIndex = nextTriangleIndex;
			if (m_triangles[triangleIndex]->GetClosestDistanceToOriginSquare() < m_triangles[currentTriangleIndex]->GetClosestDistanceToOriginSquare())
			{
				int32_t previousTraingleIndex = m_triangles[currentTriangleIndex]->GetPreviousTriangle();
				m_triangles[triangleIndex]->SetNextTriangle(currentTriangleIndex);
				m_triangles[triangleIndex]->SetPreviousTriangle(previousTraingleIndex);
				m_triangles[currentTriangleIndex]->SetPreviousTriangle(triangleIndex);
				if (previousTraingleIndex != -1)
				{
					m_triangles[previousTraingleIndex]->SetNextTriangle(triangleIndex);
				}
				else
				{
					m_triangleHeadIndex = triangleIndex;
				}

				return;
			}
		}

		m_triangles[triangleIndex]->SetPreviousTriangle(currentTriangleIndex);
		if (currentTriangleIndex != -1)
		{
			m_triangles[currentTriangleIndex]->SetNextTriangle(triangleIndex);
		}
		else
		{
			m_triangleHeadIndex = triangleIndex;
		}
	}

	bool GJK3DDeltahedron::ExpandDeltahedron(const int32_t removeTriangleIndex, const Vertex3D& newVertex)
	{
		ExpandWithNewVertex(removeTriangleIndex, newVertex);
		UpdateOriginEnclosed(removeTriangleIndex);
		return UpdateNeighbors();
	}

	void GJK3DDeltahedron::ExpandWithNewVertex(const int32_t removeTriangleIndex, const Vertex3D& newVertex)
	{
		const Uniq<GJK3DTriangle>& removeTriangle = m_triangles[removeTriangleIndex];
		if (removeTriangle->IsDeleted())
		{
			return;
		}

		RemoveTriangle(removeTriangleIndex);

		bool leftTriangleShouldBeRemoved = InTheSameDirection(removeTriangle->GetLeftTriangle(), newVertex);
		if (!leftTriangleShouldBeRemoved)
		{
			int32_t newTriangleIndex = CreateTriangle(removeTriangle->GetC(), removeTriangle->GetA(), newVertex);
			m_triangles[newTriangleIndex]->SetRightTriangle(removeTriangle->GetLeftTriangle());
			m_expandedTriangleIndices[m_numOfExpandedTriagnles++] = newTriangleIndex;

			const Uniq<GJK3DTriangle>& leftTriangle = m_triangles[removeTriangle->GetLeftTriangle()];
			if (leftTriangle->GetLeftTriangle() == removeTriangleIndex)
			{
				leftTriangle->SetLeftTriangle(newTriangleIndex);
			}
			else if (leftTriangle->GetRightTriangle() == removeTriangleIndex)
			{
				leftTriangle->SetRightTriangle(newTriangleIndex);
			}
			else
			{
				leftTriangle->SetBottomTriangle(newTriangleIndex);
			}
		}

		bool rightTriangleShouldBeRemoved = InTheSameDirection(removeTriangle->GetRightTriangle(), newVertex);
		if (!rightTriangleShouldBeRemoved)
		{
			int32_t newTriangleIndex = CreateTriangle(removeTriangle->GetA(), removeTriangle->GetB(), newVertex);
			m_triangles[newTriangleIndex]->SetRightTriangle(removeTriangle->GetRightTriangle());
			m_expandedTriangleIndices[m_numOfExpandedTriagnles++] = newTriangleIndex;

			const Uniq<GJK3DTriangle>& rightTriangle = m_triangles[removeTriangle->GetRightTriangle()];
			if (rightTriangle->GetLeftTriangle() == removeTriangleIndex)
			{
				rightTriangle->SetLeftTriangle(newTriangleIndex);
			}
			else if (rightTriangle->GetRightTriangle() == removeTriangleIndex)
			{
				rightTriangle->SetRightTriangle(newTriangleIndex);
			}
			else
			{
				rightTriangle->SetBottomTriangle(newTriangleIndex);
			}
		}

		bool bottomTriangleShouldBeRemoved = InTheSameDirection(removeTriangle->GetBottomTriangle(), newVertex);
		if (!bottomTriangleShouldBeRemoved)
		{
			int32_t newTriangleIndex = CreateTriangle(removeTriangle->GetB(), removeTriangle->GetC(), newVertex);
			m_triangles[newTriangleIndex]->SetRightTriangle(removeTriangle->GetBottomTriangle());
			m_expandedTriangleIndices[m_numOfExpandedTriagnles++] = newTriangleIndex;

			const Uniq<GJK3DTriangle>& bottomTriangle = m_triangles[removeTriangle->GetBottomTriangle()];
			if (bottomTriangle->GetLeftTriangle() == removeTriangleIndex)
			{
				bottomTriangle->SetLeftTriangle(newTriangleIndex);
			}
			else if (bottomTriangle->GetRightTriangle() == removeTriangleIndex)
			{
				bottomTriangle->SetRightTriangle(newTriangleIndex);
			}
			else
			{
				bottomTriangle->SetBottomTriangle(newTriangleIndex);
			}
		}

		if (leftTriangleShouldBeRemoved)
		{
			ExpandWithNewVertex(removeTriangle->GetLeftTriangle(), newVertex);
		}

		if (rightTriangleShouldBeRemoved)
		{
			ExpandWithNewVertex(removeTriangle->GetRightTriangle(), newVertex);
		}

		if (bottomTriangleShouldBeRemoved)
		{
			ExpandWithNewVertex(removeTriangle->GetBottomTriangle(), newVertex);
		}
	}

	void GJK3DDeltahedron::RemoveTriangle(const int32_t removeTriangleIndex)
	{
		const Uniq<GJK3DTriangle>& removeTriangle = m_triangles[removeTriangleIndex];
		const int32_t previousTriangleIndex = removeTriangle->GetPreviousTriangle();
		const int32_t nextTriangleIndex = removeTriangle->GetNextTriangle();

		if (previousTriangleIndex != -1)
		{
			m_triangles[previousTriangleIndex]->SetNextTriangle(nextTriangleIndex);
		}
		else
		{
			m_triangleHeadIndex = nextTriangleIndex;
		}

		if (nextTriangleIndex != -1)
		{
			m_triangles[nextTriangleIndex]->SetPreviousTriangle(previousTriangleIndex);
		}

		removeTriangle->MarkAsDeleted();
	}
	
	bool GJK3DDeltahedron::InTheSameDirection(const int32_t triangleIndex, Vertex3D vertex)
	{
		const Uniq<GJK3DTriangle>& triangle = m_triangles[triangleIndex];
		glm::dvec3 direction = vertex - triangle->GetA();
		return glm::dot(direction, triangle->GetNormalVector()) > 0;
	}
	
	void GJK3DDeltahedron::UpdateOriginEnclosed(const int32_t removeTriangleIndex)
	{
		if (!m_originEnclosed)
		{
			for (uint32_t i = 0; i < m_numOfExpandedTriagnles; ++i)
			{
				const Uniq<GJK3DTriangle>& triangle = m_triangles[m_expandedTriangleIndices[i]];
				bool doNotIncludeOriginPoint = glm::dot(triangle->GetNormalVector(), (glm::dvec3)triangle->GetA()) < 0.0;
				if (doNotIncludeOriginPoint)
				{
					return;
				}
			}

			const Uniq<GJK3DTriangle>& removeTriangle = m_triangles[removeTriangleIndex];
			bool oldTriangleNotIncludeOriginPoint = glm::dot(removeTriangle->GetNormalVector(), (glm::dvec3)removeTriangle->GetA()) < 0.0;

			m_originEnclosed = oldTriangleNotIncludeOriginPoint;
		}
	}
	
	bool GJK3DDeltahedron::UpdateNeighbors()
	{
		const int32_t firstTriangleIndex = m_expandedTriangleIndices[0];
		int32_t lastTriangleIndex = firstTriangleIndex;

		m_expandedTriangleIndices[0] = m_expandedTriangleIndices[--m_numOfExpandedTriagnles];

		while (m_numOfExpandedTriagnles > 0)
		{
			int32_t foundTriangleIndex = -1;
			for (int32_t i = 0; i < m_numOfExpandedTriagnles; i++)
			{
				if (m_triangles[m_expandedTriangleIndices[i]]->GetA().value == m_triangles[lastTriangleIndex]->GetB().value)
				{
					foundTriangleIndex = i;
					break;
				}
			}

			if (foundTriangleIndex == -1)
			{
				return false;
			}

			m_triangles[lastTriangleIndex]->SetBottomTriangle(m_expandedTriangleIndices[foundTriangleIndex]);
			m_triangles[m_expandedTriangleIndices[foundTriangleIndex]]->SetLeftTriangle(lastTriangleIndex);

			lastTriangleIndex = m_expandedTriangleIndices[foundTriangleIndex];
			m_expandedTriangleIndices[foundTriangleIndex] = m_expandedTriangleIndices[--m_numOfExpandedTriagnles];
		}

		m_triangles[firstTriangleIndex]->SetLeftTriangle(lastTriangleIndex);
		m_triangles[lastTriangleIndex]->SetBottomTriangle(firstTriangleIndex);

		return true;
	}
}
