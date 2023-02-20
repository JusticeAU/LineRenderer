#include "Shape.h"

void Shape::Update(float deltaTime)
{	
	m_velocity += m_gravityForNow * deltaTime;
	Move(m_velocity * deltaTime);
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

bool Shape::LineIntersects(Vec2 a, Vec2 b)
{
	return false; // default if not implemented
}
void Shape::Slice(Vec2 a, Vec2 b, std::vector<Shape*>* shapes)
{
	// default is not sliceable
}