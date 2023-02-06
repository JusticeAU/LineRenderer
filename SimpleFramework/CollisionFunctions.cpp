#include "CollisionFunctions.h"

CollisionFunction COLLISION_FUNCTIONS[(int)SHAPE::COUNT][(int)SHAPE::COUNT]
{
	{CircleOnCircle, CircleOnPlane},
	{PlaneOnCircle, PlaneOnPlane}
};

CollisionData TestCollisions(Shape* a, Shape* b)
{
	return COLLISION_FUNCTIONS[(int)a->GetShape()][(int)b->GetShape()](a, b);
}

CollisionData CircleOnCircle(Shape* a, Shape* b)
{
	Circle* circleA = static_cast<Circle*>(a);
	Circle* circleB = static_cast<Circle*>(b);


	CollisionData col;
	float distanceFromAToB = glm::distance(circleA->m_position, circleB->m_position);
	if (distanceFromAToB < circleA->m_radius + circleB->m_radius)
	{
		Vec2 displacement = b->m_position - circleA->m_position;
		if (distanceFromAToB == 0) displacement = Vec2(1, 0); // if the circles are on top of each other, we just decide that they will move away from each other on the X plane.
		col.depth = circleA->m_radius + circleB->m_radius - distanceFromAToB;
		col.normal = glm::normalize(displacement);
		col.worldPosition = a->m_position + col.normal * (circleA->m_radius - col.depth / 2);
	}
	col.shapeA = a;
	col.shapeB = b;
	return col;
}
CollisionData CircleOnPlane(Shape* a, Shape* b)
{
	Circle* circleA = static_cast<Circle*>(a);
	Plane* planeB = static_cast<Plane*>(b);

	CollisionData col;
	float distance = glm::dot(circleA->m_position, planeB->m_normal) - planeB->m_distance;
	col.normal = planeB->m_normal;
	col.depth = distance + circleA->m_radius;
	col.worldPosition = circleA->m_position - (planeB->m_normal * distance);
	col.shapeA = a;
	col.shapeB = b;
	return col;
}

CollisionData PlaneOnCircle(Shape* a, Shape* b)
{
	return CircleOnPlane(b, a);
}
CollisionData PlaneOnPlane(Shape* a, Shape* b)
{
	CollisionData col;
	col.shapeA = a;
	col.shapeA = b;

	return col;

}