#include "AABB.h"
#include "LineRenderer.h"
#include <iostream>
#include <string>

void AABB::CalculateMassFromArea()
{
	float mass = (m_halfWidth * 2) * (m_halfHeight * 2);
	std::cout << "Setting AABB mass to: " + std::to_string(mass) << std::endl;
	m_inverseMass = 1 / mass;
}

void AABB::Draw(LineRenderer& lines) const
{
	float minX = m_position.x - m_halfWidth;
	float maxX = m_position.x + m_halfWidth;
	float minY = m_position.y - m_halfHeight;
	float maxY = m_position.y + m_halfHeight;

	lines.SetColour(m_colour);
	lines.AddPointToLine({ minX, minY });
	lines.AddPointToLine({ minX, maxY });
	lines.AddPointToLine({ maxX, maxY });
	lines.AddPointToLine({ maxX, minY });
	lines.FinishLineLoop();

	//lines.DrawLineSegment(m_position, m_position + m_velocity);
}

bool AABB::PointInShape(Vec2 point)
{
	if (point.x > Left() && point.x < Right()
		&&
		point.y > Bottom() && point.y < Top()
		)
		return true;
	else
		return false;
}

Vec2 AABB::GetClosestPoint(Vec2 point)
{
	float minX, minY, maxX, maxY;
	minX = m_position.x - m_halfWidth;
	maxX = m_position.x + m_halfWidth;
	minY = m_position.y - m_halfHeight;
	maxY = m_position.y + m_halfHeight;

	Vec2 closestPoint;

	closestPoint.x = point.x < minX ? minX : point.x > maxX ? maxX : point.x;
	closestPoint.y = point.y < minY ? minY : point.y > maxY ? maxY : point.y;

	return closestPoint;
}

// 0-3 will be TopRight, BottomRight, Bottom Left, TopLeft
Vec2* AABB::GetCorners()
{
	Vec2* depths = new Vec2[4];
	depths[0] = m_position + Vec2(m_halfWidth, m_halfHeight);
	depths[1] = m_position + Vec2(m_halfWidth, -m_halfHeight);
	depths[2] = m_position + Vec2(-m_halfWidth, -m_halfHeight);
	depths[3] = m_position + Vec2(-m_halfWidth, m_halfHeight);
	return depths;
}
