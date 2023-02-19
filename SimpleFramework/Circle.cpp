#include "Circle.h"
#include "CollisionData.h"
#include "LineRenderer.h"
#include <iostream>
#include <string>
#include "glm.hpp"

void Circle::CalculateMassFromArea()
{
	float mass = glm::pi<float>() * (m_radius * m_radius);
	std::cout << "Setting Circles mass to: " + std::to_string(mass) << std::endl;
	m_inverseMass = 1.0f / mass;
}

void Circle::Draw(LineRenderer& lines) const
{
	lines.SetColour(m_colour);
	lines.DrawCircle(m_position, m_radius, 18);

	//lines.DrawLineSegment(m_position, m_position + m_velocity);
}

bool Circle::PointInShape(Vec2 point)
{
	return glm::distance(m_position, point) < m_radius;
}