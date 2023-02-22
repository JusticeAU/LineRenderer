#include "ConvexPolygon.h"
#include "LineRenderer.h"
#include "Spawner.h"

#include <iostream>

void ConvexPolygon::Update(float deltaTime)
{
	Shape::Update(deltaTime);
	aabb.m_position = m_position;
}

void ConvexPolygon::Draw(LineRenderer& lines) const
{
	lines.SetColour(m_colour);

	for (auto& point : m_points)
		lines.AddPointToLine(m_position + point);

	lines.FinishLineLoop();

	// Draw its centrepoint
	lines.DrawCircle(m_position, 0.1f);

	//aabb.Draw(lines);
}

void ConvexPolygon::CalculateMassFromArea()
{
	// We will construct triangles from the centrepoint to each face and sum their area for a total area, which will be our mass.
	float totalArea = 0;
	float lengthAB = glm::distance(m_position, GetVertexInWorldspace(0));
	for (int i = 0; i < m_points.size(); i++)
	{
		float lengthAC = glm::distance(m_position, GetVertexInWorldspace(i+1));
		float lengthBC = glm::distance(GetVertexInWorldspace(i), GetVertexInWorldspace(i + 1));


		// Heron's Formula for area of a triangle with 3 known lengths.
		float s = (lengthAB + lengthAC + lengthBC) / 2; // semi-perimiter
		float area = glm::sqrt(s * (s - lengthAB) * (s - lengthAC) * (s - lengthBC));
		
		totalArea += area;

		// reuse AC as AB for next iteration
		lengthAB = lengthAC;
	}

	m_inverseMass = 1 / totalArea;
}

bool ConvexPolygon::PointInShape(Vec2 point) const
{
	// Get
	//Vec2 testPlane = glm::normalize(m_position - point);

	for (int edge = 0; edge < m_points.size(); edge++)
	{
		Vec2 testEdge = GetSurfaceNormal(edge);
		float max = -FLT_MAX;
		for (int i = 0; i < m_points.size(); i++)
		{
			float dot = glm::dot(GetVertexInWorldspace(i), testEdge);
			if (dot > max)
				max = dot;
		}

		float pointDot = glm::dot(point, testEdge);

		if (pointDot > max)
			return false;
	}
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

	if (cutsFound == 2) // valid cut on this Poly
	{
		// construct the new Poly and add it to our environment
		std::vector<Vec2> newShape = std::vector<Vec2>();
		newShape.push_back(cutPositions[0] - m_position);
		for (int i = cutIndexes[0] + 1; i <= cutIndexes[1]; i++)
		{
			Vec2 point = m_points.at(i);
			newShape.push_back(point);
		}
		newShape.push_back(cutPositions[1] - m_position);
		ConvexPolygon* newPoly = new ConvexPolygon(m_position, m_inverseMass, newShape, m_colour);
		newPoly->m_velocity = m_velocity;
		newPoly->UpdateAABB();
		shapes->push_back(newPoly);

		// remove the verts from our current Poly and reprocess.
		m_points.erase(m_points.begin() + cutIndexes[0] + 1, m_points.begin() + cutIndexes[1] + 1);
		m_points.insert(m_points.begin() + cutIndexes[0] + 1, cutPositions[0] - m_position);
		m_points.insert(m_points.begin() + cutIndexes[0] + 2, cutPositions[1] - m_position);
		RecalculateCentre();
		UpdateAABB();
		CalculateMassFromArea();

		// Send the objects away from each other - looks cool and ninja!
		Vec2 cutDirection = glm::normalize(lineTo - lineFrom);
		Vec2 cutPerp = { cutDirection.y, -cutDirection.x };
		newPoly->ApplyImpulse(cutPerp);
		ApplyImpulse(-cutPerp);
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

void ConvexPolygon::RecalculateCentre()
{
	float minX = FLT_MAX;
	float minY = FLT_MAX;
	float maxX = -FLT_MAX;
	float maxY = -FLT_MAX;

	Vec2 anOldPos = GetVertexInWorldspace(0); // We're going to take a reference point to ensure we dont cause a jump when we update the world position of the centre point

	for (auto& point : m_points)
	{
		minX = glm::min(minX, point.x);
		minY = glm::min(minY, point.y);
		maxX = glm::max(maxX, point.x);
		maxY = glm::max(maxY, point.y);
	}

	Vec2 centre = { (minX + maxX) * 0.5f, (minY + maxY) * 0.5f };
	for (auto& point : m_points)
		point -= centre;

	m_position = centre;

	// Correct the position based on a known quantity from before
	Vec2 aNewPos = GetVertexInWorldspace(0);
	Vec2 change = aNewPos - anOldPos;
	m_position -= change;
}

void ConvexPolygon::UpdateAABB()
{
	float maxHeight = -FLT_MAX;
	float maxWidth = -FLT_MAX;
	for (auto& point : m_points)
	{
		//max = glm::max(glm::max(glm::abs(point.y), glm::abs(point.x)), max); // Old ver for kicks
		maxHeight = glm::max(glm::abs(point.y), maxHeight);
		maxWidth = glm::max(glm::abs(point.x), maxWidth);
	}

	aabb.SetHalfHeight(maxHeight);
	aabb.SetHalfWidth(maxWidth);

}