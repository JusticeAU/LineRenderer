#include "Sphere.h"
#include "LineRenderer.h"

Sphere::Sphere(glm::vec2 position, glm::vec2 velocity, float mass, float radius, glm::vec4 colour)
	: RigidBody(SPHERE, position, velocity, mass, 0.0f)
{
	m_radius = radius;
	m_colour = colour;
}

void Sphere::draw(LineRenderer* lr)
{
	lr->DrawCircle(m_position, m_radius);
}
