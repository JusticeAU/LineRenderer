#include "Circle.h"
#include "CollisionData.h"
#include "LineRenderer.h"

void Circle::Draw(LineRenderer& lines) const
{
	lines.SetColour(m_colour);
	lines.DrawCircle(m_position, m_radius);
}

CollisionData Circle::CollisionWithCircle(Circle* shapeB)
{
	CollisionData col;
	float distanceFromAToB = glm::distance(m_position, shapeB->m_position);
	if (distanceFromAToB < m_radius + shapeB->m_radius)
	{
		Vec2 displacement = shapeB->m_position - m_position;
		if (distanceFromAToB == 0) displacement = Vec2(1, 0); // if the circles are on top of each other, we just decide that they will move away from each other on the X plane.
		col.depth = m_radius + shapeB->m_radius - distanceFromAToB;
		col.normal = glm::normalize(displacement);
		col.worldPosition = m_position + col.normal * (m_radius - col.depth / 2);
	}
	col.shapeA = this;
	col.shapeB = shapeB;
	return col;
}