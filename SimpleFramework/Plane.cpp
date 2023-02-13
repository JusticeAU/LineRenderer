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
	m_inverseMass = 0;
}

void Plane::Draw(LineRenderer& lines) const
{
	float virtualLength = 50.0f; // line will be twice this as it is drawn from 'origin' in botyh direction
	Vec2 planeOrigin = m_normal * m_distance;
	Vec2 perpendicularDirection = { -m_normal.y, m_normal.x };
		
	
	// Draw some bullshit behind the plane
	lines.SetColour(m_colour * 0.5f);
	for (int i = -virtualLength; i <= virtualLength; i++)
		lines.DrawLineSegment(planeOrigin + (perpendicularDirection * (float)i), planeOrigin + (perpendicularDirection * (float)i) + (-m_normal * 5.0f));

	// Draw some Debug.
	// Normal from plane origin
	lines.SetColour(m_colour);
	lines.DrawLineSegment(planeOrigin, planeOrigin + m_normal * 0.5f);

	// Draw the plane
	lines.DrawLineSegment(
		planeOrigin + perpendicularDirection * virtualLength,
		planeOrigin - perpendicularDirection * virtualLength);

}

float Plane::DepthInPlane(Vec2 point)
{
	float distance = m_distance - glm::dot(point, m_normal);

	return distance;
}
