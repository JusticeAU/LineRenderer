#include "Plane.h"
#include "LineRenderer.h"

Plane::Plane(Vec2 normal, float distance, Vec3 colour)
	: Shape(SHAPE::PLANE, { 0,0 }, 0, colour)
{
	m_normal = normal;
	m_distance = distance;
}

void Plane::Draw(LineRenderer& lines) const
{
	float virtualLength = 100.0f; // line will be twice this as it is drawn from 'origin' in botyh direction
	Vec2 planeOrigin = m_normal * m_distance;
	Vec2 perpendicularDirection = { -m_normal.y, m_normal.x };
		
	// Draw the plane
	lines.SetColour(m_colour);
	lines.DrawLineSegment(
		planeOrigin + perpendicularDirection * virtualLength,
		planeOrigin - perpendicularDirection * virtualLength);

	// Draw some Debug.
	// Normal from plane origin
	lines.DrawLineSegment(planeOrigin, planeOrigin + m_normal);
}
