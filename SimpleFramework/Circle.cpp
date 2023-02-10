#include "Circle.h"
#include "CollisionData.h"
#include "LineRenderer.h"
#include <iostream>
#include <string>

void Circle::CalculateMassFromArea()
{
	float mass = glm::pi<float>() * (m_radius * m_radius);
	std::cout << "Setting Circles mass to: " + std::to_string(mass) << std::endl;
	m_inverseMass = 1.0f / mass;
}

void Circle::Draw(LineRenderer& lines) const
{
	lines.SetColour(m_colour);
	lines.DrawCircle(m_position, m_radius);
}