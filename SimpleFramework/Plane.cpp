#include "Plane.h"
#include "LineRenderer.h"

Plane::Plane(Vec2 normal, float distance, Vec3 colour)
	: Shape(SHAPE::PLANE, { 0,0 }, 0, colour)
{
	m_normal = normal;
	m_distance = distance;
}

void Plane::CalculateMassFromArea()
{
	m_inverseMass = 0; // Planes do not have a mass.
}

void Plane::Draw(LineRenderer& lines) const
{
	float virtualLength = 50.0f; // line will be twice this as it is drawn from 'origin' in both direction
	Vec2 planeOrigin = m_normal * m_distance;
	Vec2 perpendicularDirection = { -m_normal.y, m_normal.x };
	
	// Normal from plane origin
	lines.SetColour(m_colour);
	lines.DrawLineSegment(planeOrigin, planeOrigin + m_normal * 0.5f);

	// Draw the plane
	lines.DrawLineSegment(
		planeOrigin + perpendicularDirection * virtualLength,
		planeOrigin - perpendicularDirection * virtualLength);

}

// Returns true if the provided point is behind the plane.
bool Plane::PointInShape(Vec2 point) const
{
	float distance = m_distance - glm::dot(point, m_normal);
	if (distance > 0)
		return true;
	else
		return false;
}

// returns the depth of the point in to the plane. Will return a negative value if the point is not inside the plane.
float Plane::DepthInPlane(Vec2 point)
{
	return m_distance - glm::dot(point, m_normal);
}

Shape* Plane::Clone()
{
	Plane* clone = new Plane(m_normal, m_distance, m_colour);
	return (Shape*)clone;
}