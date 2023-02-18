#include "ConvexPolygon.h"
#include "LineRenderer.h"

void ConvexPolygon::CalculateMassFromArea()
{
	// intentionall left blank for now
}

void ConvexPolygon::Draw(LineRenderer& lines) const
{
	lines.SetColour(m_colour);
	for (auto& point : m_points)
		lines.AddPointToLine(m_position + point);

	lines.FinishLineLoop();

	// Draw its centrepoint
	//lines.DrawCircle(m_position, 0.1f);

	//Draw its normals
	/*for (int i = 0; i < m_points.size(); i++)
	{
		lines.DrawLineSegment(m_position + GetSurfaceMidPoint(i), m_position + GetSurfaceMidPoint(i) + (GetSurfaceNormal(i) * 0.3f));
	}*/
}

Vec2 ConvexPolygon::GetVertexDirection(int vertIndex) const
{
	Vec2 a = m_points[vertIndex];
	Vec2 b = vertIndex + 1 >= m_points.size() ? m_points[0] : m_points[vertIndex + 1];

	Vec2 aToB = b - a;
	aToB = glm::normalize(aToB);
	return aToB;
}

Vec2 ConvexPolygon::GetSurfaceNormal(int vertIndex) const
{
	Vec2 a = m_points[vertIndex];
	Vec2 b = vertIndex + 1 >= m_points.size() ? m_points[0] : m_points[vertIndex + 1];

	Vec2 aToB = b - a;
	aToB = glm::normalize(aToB);
	Vec2 normal = { -aToB.y, aToB.x };
	return normal;
}

Vec2 ConvexPolygon::GetSurfaceMidPoint(int vertIndex) const
{
	Vec2 a = m_points[vertIndex];
	Vec2 b = vertIndex + 1 >= m_points.size() ? m_points[0] : m_points[vertIndex + 1];

	Vec2 point = (a + b) * 0.5f;
	return point;
}

Vec2 ConvexPolygon::GetVertexInWorldspace(int vertIndex) const
{
	if (vertIndex >= m_points.size())
		vertIndex -= m_points.size();
	return m_position + m_points[vertIndex];
}


void ConvexPolygon::CalculateCentroid()
{
	Vec2 centroid(0);

	for (auto& point : m_points)
		centroid += point;

	centroid /= m_points.size();

	for (auto& point : m_points)
		point -= centroid;

	m_position = centroid;
}