#include "CollisionFunctions.h"

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
	col.shapeA = a;
	col.shapeB = b;

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

		if (distToUp < distToDown && distToUp < distToRight && distToUp < distToLeft)
		{
			col.depth = distToUp + circleA->m_radius;
			col.normal = { 0, 1 };
		}
		else if (distToDown < distToRight && distToDown < distToLeft)
		{
			col.depth = distToDown + circleA->m_radius;
			col.normal = { 0, -1 };
		}
		else if (distToRight < distToLeft)
		{
			col.depth = distToRight + circleA->m_radius;
			col.normal = { 1, 0 };
		}
		else
		{
			col.depth = distToLeft + circleA->m_radius;
			col.normal = { -1, 0 };
		}
	}
	else
	{
		col.depth = circleA->m_radius - glm::distance(closestPoint, circleA->m_position);
		col.normal = -glm::normalize(circleA->m_position - closestPoint);
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
	Plane* planeA = static_cast<Plane*>(a);
	AABB* aabbB = static_cast<AABB*>(b);
	CollisionData col;
	col.shapeA = a;
	col.shapeB = b;
	col.normal = -planeA->m_normal;

	// for each aabb point, get depth
	Vec2* points = aabbB->GetCorners();
	float depths[4];
	for (int i = 0; i < 4; i++)
		depths[i] = planeA->DepthInPlane(points[i]);

	// find greatest depths - 0-3 will be TopRight, BottomRight, Bottom Left, TopLeft
	if (depths[0] > depths[1] && depths[0] > depths[2] && depths[0] > depths[3])
	{
		col.depth = depths[0];
		col.worldPosition = points[0];
	}
	else if (depths[1] > depths[2] && depths[1] > depths[3])
	{
		col.depth = depths[1];
		col.worldPosition = points[1];
	}
	else if (depths[2] > depths[3])
	{
		col.depth = depths[2];
		col.worldPosition = points[2];
	}
	else
	{
		col.depth = depths[3];
		col.worldPosition = points[3];
	}
	
	delete points;
	// get the greatest depth and push out on the reverse normal of the plane
	return col;
}

CollisionData AABBOnCircle(Shape* a, Shape* b)
{
	return CircleOnAABB(b, a);
}
CollisionData AABBOnPlane(Shape* a, Shape* b)
{
	return PlaneOnAABB(b, a);
}
CollisionData AABBOnAABB(Shape* a, Shape* b)
{
	AABB* aabbA = static_cast<AABB*>(a);
	AABB* aabbB = static_cast<AABB*>(b);
	CollisionData col;
	col.shapeA = a;
	col.shapeB = b;

	// Get Each Overlap
	float upOverlap = aabbA->Top() - aabbB->Bottom();
	float downOverlap = aabbB->Top() - aabbA->Bottom();
	float leftOverlap = aabbB->Right() - aabbA->Left();
	float rightOverlap = aabbA->Right() - aabbB->Left();

	// find the lowest depth (negative means no overlap)
	if (upOverlap < downOverlap && upOverlap < leftOverlap && upOverlap < rightOverlap)
	{
		col.depth = upOverlap;
		col.normal = { 0, 1 };
	}
	else if(downOverlap < leftOverlap && downOverlap < rightOverlap)
	{
		col.depth = downOverlap;
		col.normal = { 0, -1 };
	}
	else if (leftOverlap < rightOverlap)
	{
		col.depth = leftOverlap;
		col.normal = { -1, 0 };
	}
	else
	{
		col.depth = rightOverlap;
		col.normal = { 1, 0 };
	}
	return col;
}