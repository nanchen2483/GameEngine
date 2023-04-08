#include "enginepch.h"
#include "GJK3DTriangle.h"

namespace Engine
{
	GJK3DTriangle::GJK3DTriangle(const Vertex3D& a, const Vertex3D& b, const Vertex3D& c)
		: Triangle3D(a, b, c)
	{
	}

	void GJK3DTriangle::SetLeftTriangle(GJK3DTriangle* leftTriangle)
	{
		m_leftTriangle = leftTriangle;
	}
	
	void GJK3DTriangle::SetRightTriangle(GJK3DTriangle* rightTriangle)
	{
		m_rightTriangle = rightTriangle;
	}
	
	void GJK3DTriangle::SetBottomTriangle(GJK3DTriangle* bottomTriangle)
	{
		m_bottomTriangle = bottomTriangle;
	}
	
	void GJK3DTriangle::SetNeighbors(GJK3DTriangle* leftTriangle, GJK3DTriangle* rightTriangle, GJK3DTriangle* bottomTriangle)
	{
		m_leftTriangle = leftTriangle;
		m_rightTriangle = rightTriangle;
		m_bottomTriangle = bottomTriangle;
	}
	
	void GJK3DTriangle::SetPreviousTriangle(GJK3DTriangle* previousTriangle)
	{
		m_previousTriangle = previousTriangle;
	}
	
	void GJK3DTriangle::SetNextTriangle(GJK3DTriangle* nextTriangle)
	{
		m_nextTriangle = nextTriangle;
	}
	
	void GJK3DTriangle::MarkAsDeleted()
	{
		m_isDeleted = true;
	}
}
