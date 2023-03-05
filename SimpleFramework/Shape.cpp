#include "Shape.h"

// Moves the object by adding its velocity with forward Euler integration
void Shape::Update(float deltaTime)
{	
	m_velocity += m_gravity * deltaTime;
	Move(m_velocity * deltaTime);
	Rotate(m_rotationalVelocity * deltaTime);
}

// Applies a displacement to the shapes position
void Shape::Move(Vec2 displacement)
{
	m_position += displacement;
}

void Shape::Rotate(float degrees)
{
	m_rotation += degrees;
}

// Applies an Impulse. impulse should be in newtons/s
void Shape::ApplyImpulse(Vec2 impulse)
{
	if (m_inverseMass == 0)
		return;

	m_velocity += impulse / GetMass();
}