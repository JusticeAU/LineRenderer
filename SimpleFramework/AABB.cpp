#include "AABB.h"
#include "LineRenderer.h"

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
}

// Calculates the area of the the AABB and sets the mass accordingly.
void AABB::CalculateMassFromArea()
{
	float mass = (m_halfWidth * 2) * (m_halfHeight * 2);
	m_inverseMass = 1 / mass;
}

// Returns true if the point is within the bounds of the AABB.
bool AABB::PointInShape(Vec2 point) const
{
	if (point.x > Left() && point.x < Right()
		&&
		point.y > Bottom() && point.y < Top()
		)
		return true;
	else
		return false;
}

// Returns the closest point on the AABB to point. If point is inside the AABB, it will return the same Vec2.
Vec2 AABB::GetClosestPoint(Vec2 point)
{
	float minX, minY, maxX, maxY;
	minX = m_position.x - m_halfWidth;
	maxX = m_position.x + m_halfWidth;
	minY = m_position.y - m_halfHeight;
	maxY = m_position.y + m_halfHeight;

	Vec2 closestPoint;
	// Clamp to bounds of AABB.
	closestPoint.x = point.x < minX ? minX : point.x > maxX ? maxX : point.x;
	closestPoint.y = point.y < minY ? minY : point.y > maxY ? maxY : point.y;

	return closestPoint;
}

// Returns a pointer to an array of 4 Vec2s for each corner in worldspace. 0-3 will be TopRight, BottomRight, Bottom Left, TopLeft
Vec2* AABB::GetCorners() const
{
	Vec2* depths = new Vec2[4];
	depths[0] = m_position + Vec2(m_halfWidth, m_halfHeight);
	depths[1] = m_position + Vec2(m_halfWidth, -m_halfHeight);
	depths[2] = m_position + Vec2(-m_halfWidth, -m_halfHeight);
	depths[3] = m_position + Vec2(-m_halfWidth, m_halfHeight);
	return depths;
}

Shape* AABB::Clone()
{
	AABB* clone = new AABB(m_position, m_halfWidth * 2.0f, m_halfHeight * 2.0f, m_inverseMass, m_colour);
	return (Shape*)clone;
}
