#include "ConvexPolygon.h"
#include "LineRenderer.h"
#include "Spawner.h"

void ConvexPolygon::CalculateMassFromArea()
{
	// intentionally left blank for now
}

void ConvexPolygon::Draw(LineRenderer& lines) const
{
	lines.SetColour(m_colour);
	for (auto& point : m_points)
		lines.AddPointToLine(m_position + point);

	lines.FinishLineLoop();

	// Draw its centrepoint
	lines.DrawCircle(m_position, 0.1f);

	//Draw its normals
	/*for (int i = 0; i < m_points.size(); i++)
	{
		lines.DrawLineSegment(m_position + GetSurfaceMidPoint(i), m_position + GetSurfaceMidPoint(i) + (GetSurfaceNormal(i) * 0.3f));
	}*/
}

bool ConvexPolygon::PointInShape(Vec2 point)
{
	// Get
	Vec2 testPlane = glm::normalize(m_position - point);

	float min = FLT_MAX;
	float max = -FLT_MAX;
	for (int i = 0; i < m_points.size(); i++)
	{
		float dot = glm::dot(GetVertexInWorldspace(i), testPlane);
		if (dot < min)
			min = dot;
		if (dot > max)
			max = dot;
	}

	float pointDot = glm::dot(point, testPlane);

	if (pointDot > min && pointDot < max)
		return true;
	else
		return false;
}

bool ConvexPolygon::LineIntersects(Vec2 lineFrom, Vec2 lineTo)
{
	int cutsFound = 0;
	int cutIndexes[2];
	Vec2 cutPositions[2];

	for (int i = 0; i < m_points.size(); i++)
	{
		Vec2 vertA = GetVertexInWorldspace(i);
		Vec2 vertB = GetVertexInWorldspace(i+1);

		Vec2 point;
		if (Spawner::LineIntersection(lineFrom, lineTo, vertA, vertB, &point))
		{
			cutIndexes[cutsFound] = i;
			cutPositions[cutsFound] = point;
			cutsFound += 1;
		}

	}

	if (cutsFound == 2) // valid cut on this shape
		return true;
	else
		return false;
}

void ConvexPolygon::Slice(Vec2 lineFrom, Vec2 lineTo, std::vector<Shape*>* shapes)
{
	int cutsFound = 0;
	int cutIndexes[2];
	Vec2 cutPositions[2];

	for (int i = 0; i < m_points.size(); i++)
	{
		Vec2 vertA = GetVertexInWorldspace(i);
		Vec2 vertB = GetVertexInWorldspace(i + 1);

		Vec2 point;
		if (Spawner::LineIntersection(lineFrom, lineTo, vertA, vertB, &point))
		{
			cutIndexes[cutsFound] = i;
			cutPositions[cutsFound] = point;
			cutsFound += 1;
		}

	}

	if (cutsFound == 2) // valid cut on this shape
	{
		// construct the new shape and add it to our environment
		std::vector<Vec2> newShape = std::vector<Vec2>();
		newShape.push_back(cutPositions[0] - m_position);
		for (int i = cutIndexes[0] + 1; i <= cutIndexes[1]; i++)
		{
			Vec2 point = m_points.at(i);
			newShape.push_back(point);
		}
		newShape.push_back(cutPositions[1] - m_position);
		ConvexPolygon* newPoly = new ConvexPolygon(m_position, 1, newShape, m_colour);

		Vec2 cutDirection = glm::normalize(lineTo - lineFrom);
		Vec2 cutPerp = { cutDirection.y, -cutDirection.x };
		newPoly->ApplyImpulse(cutPerp);
		ApplyImpulse(-cutPerp);

		shapes->push_back(newPoly);

		// remove the verts from our current shape.
		m_points.erase(m_points.begin() + cutIndexes[0] + 1, m_points.begin() + cutIndexes[1] + 1);
		m_points.insert(m_points.begin() + cutIndexes[0] + 1, cutPositions[0] - m_position);
		m_points.insert(m_points.begin() + cutIndexes[0] + 2, cutPositions[1] - m_position);

		RecalculateCentroid();
		newPoly->RecalculateCentroid();
	}
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
	Vec2 vertex = GetVertexDirection(vertIndex);
	Vec2 normal = { -vertex.y, vertex.x };
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

void ConvexPolygon::RecalculateCentroid()
{
	Vec2 centroid(0);
	Vec2 anOldPos = GetVertexInWorldspace(0);

	for (auto& point : m_points)
		centroid += point;

	centroid /= m_points.size();

	for (auto& point : m_points)
		point -= centroid;

	m_position = centroid;

	// Correct the position based on a known quantity from before
	Vec2 aNewPos = GetVertexInWorldspace(0);
	Vec2 change = aNewPos - anOldPos;
	m_position -= change;
}