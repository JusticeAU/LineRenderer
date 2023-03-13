#include "Shape.h"
#include <iostream>

// Moves the object by adding its velocity with forward Euler integration
void Shape::Update(float deltaTime)
{	
	Move(m_velocity * deltaTime);
	Rotate(m_rotationalVelocity * deltaTime);
	
	//m_velocity += m_gravity * deltaTime;
	//std::cout << GetMoment() << std::endl;
}

// Applies a displacement to the shapes position
void Shape::Move(Vec2 displacement)
{
	m_position += displacement;
}

void Shape::Rotate(float radians)
{
	m_rotation += radians;
}

// Applies an Impulse. impulse should be in newtons/s
void Shape::ApplyImpulse(Vec2 impulse, Vec2 position)
{
	if (m_inverseMass == 0)
		return;

	m_velocity += impulse * GetInverseMass();
	m_rotationalVelocity += (impulse.y * position.x - impulse.x * position.y) * GetInverseMoment();
}