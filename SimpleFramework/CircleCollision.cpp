#include "CircleCollision.h"
#include "CircleCollisionData.h"

CircleCollision::CircleCollision()
{

}

void CircleCollision::Update(float delta)
{

	if (leftMouseDown)
		m_CircleA->m_position = cursorPos;
	else if (rightMouseDown)
		m_CircleB->m_position = cursorPos;

	CircleCollisionData col = m_CircleA->CollisionWithCircle(m_CircleB);
	if (col.other != nullptr)
	{
		//Draw that collision
		lines->DrawCross(m_CircleA->m_position + (col.normal * (m_CircleA->m_radius - col.penetration)), .1);
		lines->DrawCross(m_CircleB->m_position + (-col.normal * (m_CircleB->m_radius - col.penetration)), .1);
		lines->DrawLineSegment(
			m_CircleA->m_position + (col.normal * (m_CircleA->m_radius - col.penetration)),
			m_CircleB->m_position + (-col.normal * (m_CircleB->m_radius - col.penetration))
		);

		// Crudly move point b
		m_CircleB->m_position += col.normal * col.penetration;
	}

	//Draw
	lines->DrawCircle(m_CircleA->m_position, m_CircleA->m_radius);
	lines->DrawCircle(m_CircleB->m_position, m_CircleB->m_radius);

}