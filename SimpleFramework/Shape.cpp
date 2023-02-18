#include "Shape.h"

void Shape::Update(float deltaTime)
{	
	m_velocity += m_gravityForNow * deltaTime;
	Move(m_velocity * deltaTime);
	//m_velocity *= .99;
}

void Shape::Move(Vec2 displacement)
{
	m_position += displacement;
}

void Shape::ApplyImpulse(Vec2 impulse)
{
	if (m_inverseMass == 0)
		return;

	float mass = 1.0f / m_inverseMass;
	m_velocity += impulse / mass;
}

bool Shape::PointInShape(Vec2 point)
{
	return false;
}