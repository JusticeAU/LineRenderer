#include "Circle.h"
#include "CircleCollisionData.h"

CircleCollisionData Circle::CollisionWithCircle(Circle* other)
{
	CircleCollisionData col;
	// Do collision Check
	float distanceToOther = glm::distance(m_position, other->m_position);
	if (distanceToOther < m_radius + other->m_radius)
	{
		Vec2 displacement = other->m_position - m_position;
		col.normal = glm::normalize(displacement);
		col.penetration = m_radius + other->m_radius - distanceToOther;
		col.other = other;
	}

	return col;
}