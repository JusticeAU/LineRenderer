#include "Shape.h"

void Shape::Update(float deltaTime)
{	
	//m_velocity += m_gravityForNow * deltaTime;
	Move(m_velocity * deltaTime);
}

void Shape::Move(Vec2 displacement)
{
	m_position += displacement;
}

void Shape::ApplyImpulse(Vec2 impulse)
{
	float mass = 1.0f / m_inverseMass;
	m_velocity = impulse / mass;
}