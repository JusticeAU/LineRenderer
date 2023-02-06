#include "CollisionFunctions.h"
#include <iostream>

CollisionFunction COLLISION_FUNCTIONS[(int)SHAPE::COUNT][(int)SHAPE::COUNT]
{
	{CircleOnCircle, CircleOnPlane, CircleOnAABB},
	{PlaneOnCircle, PlaneOnPlane, PlaneOnAABB},
	{AABBOnCircle, AABBOnPlane, AABBOnAABB}
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
CollisionData CircleOnAABB(Shape* a, Shape* b)
{
	Circle* circleA = static_cast<Circle*>(a);
	AABB* aabbB = static_cast<AABB*>(b);

	CollisionData col;
	// Get the closest point
	Vec2 closestPoint = aabbB->GetClosestPoint(circleA->m_position);
	col.worldPosition = closestPoint;

	// test if the circle centre is inside the AABB
	if (closestPoint == circleA->m_position)
	{
		// Circle centre is inside box so we will calculate normal differently
		// get min and max coords
		float aabbMaxY = aabbB->m_position.y + aabbB->m_halfHeight;
		float aabbMinY = aabbB->m_position.y - aabbB->m_halfHeight;
		float aabbMaxX = aabbB->m_position.x + aabbB->m_halfWidth;
		float aabbMinX = aabbB->m_position.x - aabbB->m_halfWidth;

		// find the smallest direction
		float distToUp = glm::abs(circleA->m_position.y - aabbMaxY);
		float distToDown = circleA->m_position.y - aabbMinY;
		float distToRight = glm::abs(circleA->m_position.x - aabbMaxX);
		float distToLeft = circleA->m_position.x - aabbMinX;

		//std::cout << distToUp << "\t" << distToDown << "\t" << distToLeft << "\t" << distToRight << std::endl;

		if (distToUp < distToDown && distToUp < distToRight && distToUp < distToLeft)
		{
			//std::cout << "Up is closest" << std::endl;
			col.depth = distToUp + circleA->m_radius;
			col.normal = { 0, 1 };
		}
		else if (distToDown < distToRight && distToDown < distToLeft)
		{
			//std::cout << "Down is closest" << std::endl;
			col.depth = distToDown + circleA->m_radius;
			col.normal = { 0, -1 };
		}
		else if (distToRight < distToLeft)
		{
			//std::cout << "Right is closest" << std::endl;
			col.depth = distToRight + circleA->m_radius;
			col.normal = { 1, 0 };
		}
		else
		{
			//std::cout << "Left is closest" << std::endl;
			col.depth = distToLeft + circleA->m_radius;
			col.normal = { -1, 0 };
		}
	}
	else
	{
		col.depth = circleA->m_radius - glm::distance(closestPoint, circleA->m_position);
		col.normal = glm::normalize(circleA->m_position - closestPoint);
	}
	
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
CollisionData PlaneOnAABB(Shape* a, Shape* b)
{
	return CollisionData();
}

CollisionData AABBOnCircle(Shape* a, Shape* b)
{
	return CollisionData();
}
CollisionData AABBOnPlane(Shape* a, Shape* b)
{
	return CollisionData();
}
CollisionData AABBOnAABB(Shape* a, Shape* b)
{
	return CollisionData();
}