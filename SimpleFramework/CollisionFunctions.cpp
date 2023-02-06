#include "CollisionFunctions.h"

CollisionData CircleOnCircle(Circle& a, Circle& b)
{
	CollisionData col;
	float distanceFromAToB = glm::distance(a.m_position, b.m_position);
	if (distanceFromAToB < a.m_radius + b.m_radius)
	{
		Vec2 displacement = b.m_position - a.m_position;
		if (distanceFromAToB == 0) displacement = Vec2(1, 0); // if the circles are on top of each other, we just decide that they will move away from each other on the X plane.
		col.depth = a.m_radius + b.m_radius - distanceFromAToB;
		col.normal = glm::normalize(displacement);
		col.worldPosition = a.m_position + col.normal * (a.m_radius - col.depth / 2);
	}
	col.shapeA = &a;
	col.shapeB = &b;
	return col;
}