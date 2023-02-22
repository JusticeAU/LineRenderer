#include "CollisionFunctions.h"

// Lookup table for collision functions. X and Y corrospond to the SHAPE enum.
CollisionFunction COLLISION_FUNCTIONS[(int)SHAPE::COUNT][(int)SHAPE::COUNT]
{
	{CircleOnCircle, CircleOnAABB, CircleOnConvexPoly, CircleOnPlane},
	{AABBOnCircle, AABBOnAABB, AABBOnConvexPoly, AABBOnPlane},
	{ConvexPolyOnCircle, ConvexPolyOnAABB, ConvexPolyOnConvexPoly, ConvexPolyOnPlane},
	{PlaneOnCircle, PlaneOnAABB, PlaneOnConvexPoly, PlaneOnPlane},
};

// Looks up the shape types, performs the required collision check and returns a CollisionData.
CollisionData TestCollisions(Shape* a, Shape* b)
{
	return COLLISION_FUNCTIONS[(int)a->GetShape()][(int)b->GetShape()](a, b);
}

CollisionData CircleOnCircle(Shape* a, Shape* b)
{
	Circle* circleA = (Circle*)a;
	Circle* circleB = (Circle*)b;

	CollisionData col;
	col.shapeA = a;
	col.shapeB = b;

	
	float distanceFromAToB = glm::distance(circleA->m_position, circleB->m_position);
	if (distanceFromAToB < circleA->GetRadius() + circleB->GetRadius())
	{
		Vec2 displacement = b->m_position - circleA->m_position;

		// if the circles are on top of each other, we just decide that they will move away from each other on the X plane.
		if (distanceFromAToB == 0) displacement = Vec2(1, 0);
		
		col.depth = circleA->GetRadius() + circleB->GetRadius() - distanceFromAToB;
		col.normal = glm::normalize(displacement);
		col.worldPosition = (a->m_position + b->m_position) * 0.5f; // half way between the two centre points
	}
	return col;
}
CollisionData CircleOnPlane(Shape* a, Shape* b)
{
	Circle* circleA = (Circle*)a;
	Plane* planeB = (Plane*)b;

	CollisionData col;
	col.shapeA = a;
	col.shapeB = b;

	// Project circle pos on to plane normal and subtract plane distance
	float distance = glm::dot(circleA->m_position, planeB->m_normal) - planeB->m_distance;

	col.normal = -planeB->m_normal;
	col.depth = -(distance - circleA->GetRadius());
	col.worldPosition = circleA->m_position - (planeB->m_normal * distance);
	return col;
}
CollisionData CircleOnAABB(Shape* a, Shape* b)
{
	Circle* circleA = (Circle*)a;
	AABB* aabbB = (AABB*)b;

	CollisionData col;
	col.shapeA = a;
	col.shapeB = b;
	
	// Get the closest point
	Vec2 closestPoint = aabbB->GetClosestPoint(circleA->m_position);
	col.worldPosition = closestPoint;

	// Test if the circle centre is inside the AABB
	if (closestPoint == circleA->m_position)
	{
		// Circle centre is inside box so we will calculate normal differently
		// get min and max coords
		float aabbMaxY = aabbB->m_position.y + aabbB->HalfHeight();
		float aabbMinY = aabbB->m_position.y - aabbB->HalfHeight();
		float aabbMaxX = aabbB->m_position.x + aabbB->HalfWidth();
		float aabbMinX = aabbB->m_position.x - aabbB->HalfWidth();

		// find the smallest direction
		float distToUp = glm::abs(circleA->m_position.y - aabbMaxY);
		float distToDown = circleA->m_position.y - aabbMinY;
		float distToRight = glm::abs(circleA->m_position.x - aabbMaxX);
		float distToLeft = circleA->m_position.x - aabbMinX;
		if (distToUp < distToDown && distToUp < distToRight && distToUp < distToLeft)
		{
			col.depth = distToUp + circleA->GetRadius();
			col.normal = { 0, 1 };
		}
		else if (distToDown < distToRight && distToDown < distToLeft)
		{
			col.depth = distToDown + circleA->GetRadius();
			col.normal = { 0, -1 };
		}
		else if (distToRight < distToLeft)
		{
			col.depth = distToRight + circleA->GetRadius();
			col.normal = { 1, 0 };
		}
		else
		{
			col.depth = distToLeft + circleA->GetRadius();
			col.normal = { -1, 0 };
		}
	}
	else
	{
		col.depth = circleA->GetRadius() - glm::distance(closestPoint, circleA->m_position);
		col.normal = -glm::normalize(circleA->m_position - closestPoint);
	}
	
	return col;
}
CollisionData CircleOnConvexPoly(Shape* a, Shape* b)
{
	return ConvexPolyOnCircle(b, a);
}

CollisionData PlaneOnCircle(Shape* a, Shape* b)
{
	return CircleOnPlane(b, a);
}
CollisionData PlaneOnPlane(Shape* a, Shape* b)
{
	// Planes are kinematic and don't collide
	CollisionData col;
	col.shapeA = a;
	col.shapeA = b;

	return col;

}
CollisionData PlaneOnAABB(Shape* a, Shape* b)
{
	Plane* planeA = (Plane*)a;
	AABB* aabbB = (AABB*)b;

	CollisionData col;
	col.shapeA = a;
	col.shapeB = b;

	col.normal = planeA->m_normal; // Normal will just be the plane normal

	// For each aabb point, get depth
	Vec2* points = aabbB->GetCorners();
	float depths[4];
	for (int i = 0; i < 4; i++)
		depths[i] = planeA->DepthInPlane(points[i]);

	// Find greatest depths - 0-3 will be TopRight, BottomRight, Bottom Left, TopLeft
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
	return col;
}
CollisionData PlaneOnConvexPoly(Shape* a, Shape* b)
{
	return ConvexPolyOnPlane(b,a);
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
	AABB* aabbA = (AABB*)a;
	AABB* aabbB = (AABB*)b;

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
CollisionData AABBOnConvexPoly(Shape* a, Shape* b)
{
	return ConvexPolyOnAABB(b,a);
}

CollisionData ConvexPolyOnCircle(Shape* a, Shape* b)
{
	ConvexPolygon* poly1 = (ConvexPolygon*)a;
	Circle* circle2 = (Circle*)b;

	CollisionData col;
	col.shapeA = a;
	col.shapeB = b;

	// Check AABB first as more performant and likely not close enough.
	CollisionData broadCollision = AABBOnCircle(&poly1->aabb, b);
	if (broadCollision.IsCollision() == false)
		return broadCollision;

	// Use SAT to find smallest overlap (or any gap)
	// Get all poly edge directions and line from circle centre to each vertex
	std::vector<Vec2> vertexDirections = std::vector<Vec2>();
	for (int i = 0; i < poly1->GetVertexCount(); i++)
	{
		vertexDirections.push_back(poly1->GetVertexDirection(i));
		Vec2 vertexToCircle = poly1->GetVertexInWorldspace(i) - circle2->m_position;
		Vec2 perpendicular = { vertexToCircle.y, -vertexToCircle.x };
		vertexDirections.push_back(-glm::normalize(perpendicular));
	}
	// Find the minimum overlap - if any edge has a gap then there is no collision and we can bail out.
	float minOverlap = FLT_MAX;
	int minOverlapIndex;
	Vec2 vertexDirection;
	for (int i = 0; i < vertexDirections.size(); i++)
	{
		// Set up the plane we're projecting on to.
		Vec2 testPlaneNormal = -vertexDirections[i];

		// Set up a plane perpendicular to the normal to project on to.
		Vec2 planePerpendicular = { -testPlaneNormal.y, testPlaneNormal.x };

		// Set up our min and max extents for this cycle.
		float poly1min = FLT_MAX;
		float poly1max = -FLT_MAX;
		float circle2min = FLT_MAX;
		float circle2max = -FLT_MAX;

		// project all of poly1s points
		for (int i = 0; i < poly1->GetVertexCount(); i++)
		{
			float point = glm::dot(poly1->GetVertexInWorldspace(i), planePerpendicular);
			poly1min = glm::min(poly1min, point);
			poly1max = glm::max(poly1max, point);
		}

		// project circle2s points (min and max are position - + radius
		float point = glm::dot(circle2->m_position, planePerpendicular);
		circle2min = point - circle2->GetRadius();
		circle2max = point + circle2->GetRadius();


		// Get the overlap for this iteration. If there's a gap when can bail out.
		float overlapA = poly1max - circle2min;
		float overlapB = circle2max - poly1min;
		float overlap = glm::min(overlapA, overlapB);
		if (overlap > 0.0f)
		{
			if (minOverlap > overlap)
			{
				minOverlap = overlap;
				minOverlapIndex = i;
				if (overlapA > overlapB)
				{
					vertexDirection = vertexDirections[i];
				}
				else
				{

					vertexDirection = -vertexDirections[i];
				}
				vertexDirection = { vertexDirection.y, -vertexDirection.x };
			}
		}
		else
			return col;

	}
	// Wasn't a gap - there is a collision.
	col.depth = minOverlap;
	col.normal = -vertexDirection;
	return col;
}
CollisionData ConvexPolyOnPlane(Shape* a, Shape* b)
{
	ConvexPolygon* poly1 = (ConvexPolygon*)a;
	Plane* plane2 = (Plane*)b;

	CollisionData col;
	col.shapeA = a;
	col.shapeB = b;

	// Check AABB first as more performant (assuming polys have large number of points) and likely not close enough.
	CollisionData broadCollision = AABBOnPlane(&poly1->aabb, b);
	if (broadCollision.IsCollision() == false)
		return broadCollision;

	// Use SAT to find smallest overlap (or any gap)
	// Get all poly points and find which (if any) have the largest penetration in to the Plane
	float maxDepth = -FLT_MAX;
	for (int i = 0; i < poly1->GetVertexCount(); i++)
	{
		float depth = plane2->DepthInPlane(poly1->GetVertexInWorldspace(i));
		if (maxDepth < depth)
		{
			maxDepth = depth;
			col.depth = maxDepth;
		}
	}
	col.normal = -plane2->m_normal;

	return col;
}
CollisionData ConvexPolyOnAABB(Shape* a, Shape* b)
{
	ConvexPolygon* poly1 = (ConvexPolygon*)a;
	AABB* aabb2 = (AABB*)b;

	CollisionData col;
	col.shapeA = a;
	col.shapeB = b;

	// Build a list of verts to test
	std::vector<Vec2> vertexDirections = std::vector<Vec2>();

	// Add the polys verts
	for (int i = 0; i < poly1->GetVertexCount(); i++)
		vertexDirections.push_back(-poly1->GetVertexDirection(i));

	// Add cardinal x/y for the AABB
	vertexDirections.push_back({ -1,0 });
	vertexDirections.push_back({ 0,1 });
	vertexDirections.push_back({ 1,0 });
	vertexDirections.push_back({ 0,-1 });
	
	// Test projections against all verts
	float minOverlap = FLT_MAX;
	int minOverlapIndex;
	Vec2 vertexDirection;
	bool gap = false;
	Vec2* corners = aabb2->GetCorners();
	for (int i = 0; i < vertexDirections.size(); i++)
	{
		// Set up the plane we're projecting on to.
		Vec2 testPlaneNormal = -vertexDirections[i];

		// Set up a plane perpendicular to the normal to project on to.
		Vec2 planePerpendicular = { -testPlaneNormal.y, testPlaneNormal.x };

		float poly1min = FLT_MAX;
		float poly1max = -FLT_MAX;
		float poly2min = FLT_MAX;
		float poly2max = -FLT_MAX;

		// check all of AABBs points
		for (int aabbi = 0; aabbi < 4; aabbi++)
		{
			float point = glm::dot(corners[aabbi], planePerpendicular);
			poly1min = glm::min(poly1min, point);
			poly1max = glm::max(poly1max, point);
		}

		// check all of the Polyss points
		for (int polyi = 0; polyi < poly1->GetVertexCount(); polyi++)
		{
			float point = glm::dot(poly1->GetVertexInWorldspace(polyi), planePerpendicular);
			poly2min = glm::min(poly2min, point);
			poly2max = glm::max(poly2max, point);
		}

		// Get the overlap for this iteration
		float overlapA = poly1max - poly2min;
		float overlapB = poly2max - poly1min;
		float overlap = glm::min(overlapA, overlapB);
		if (overlap > 0.0f)
		{
			if (minOverlap > overlap)
			{
				minOverlap = overlap;
				minOverlapIndex = i;
				if (overlapA > overlapB)
					vertexDirection = -vertexDirections[i];
				else
					vertexDirection = vertexDirections[i];

				vertexDirection = { vertexDirection.y, -vertexDirection.x };
			}
		}
		else
		{
			gap = true;
			break;
		}
	}

	if (!gap)
	{
		col.depth = minOverlap;
		col.normal = -vertexDirection;
	}

	delete corners;
	return col;
}
CollisionData ConvexPolyOnConvexPoly(Shape* a, Shape* b)
{
	ConvexPolygon* poly1 = (ConvexPolygon*)a;
	ConvexPolygon* poly2 = (ConvexPolygon*)b;

	CollisionData col;
	col.shapeA = a;
	col.shapeB = b;

	// Check AABB first as more performant and likely not close enough.
	CollisionData broadCollision = AABBOnAABB(&poly1->aabb, &poly2->aabb);
	if (broadCollision.IsCollision() == false)
		return broadCollision;

	// Use SAT to find smallest overlap (or any gap)
	// Get all poly edge directions
	std::vector<Vec2> vertexDirections;
	vertexDirections.reserve(poly1->GetVertexCount() + poly2->GetVertexCount());
	for (int i = 0; i < poly1->GetVertexCount(); i++)
		vertexDirections.push_back(poly1->GetVertexDirection(i));
	for (int i = 0; i < poly2->GetVertexCount(); i++)
		vertexDirections.push_back(-poly2->GetVertexDirection(i));

	// Find the minimum overlap
	float minOverlap = FLT_MAX;
	int minOverlapIndex;
	Vec2 vertexDirection;
	for (int i = 0; i < vertexDirections.size(); i++)
	{
		// Set up the plane we're projecting on to.
		Vec2 testPlaneNormal = -vertexDirections[i];

		// Set up a plane perpendicular to the normal to project on to.
		Vec2 planePerpendicular = { -testPlaneNormal.y, testPlaneNormal.x };

		// Set up our min and max extents for this cycle.
		float poly1min = FLT_MAX;
		float poly1max = -FLT_MAX;
		float poly2min = FLT_MAX;
		float poly2max = -FLT_MAX;

		// project all of poly1s points
		for (int i = 0; i < poly1->GetVertexCount(); i++)
		{
			float point = glm::dot(poly1->GetVertexInWorldspace(i), planePerpendicular);
			poly1min = glm::min(poly1min, point);
			poly1max = glm::max(poly1max, point);
		}

		// project all of poly2s points
		for (int i = 0; i < poly2->GetVertexCount(); i++)
		{
			float point = glm::dot(poly2->GetVertexInWorldspace(i), planePerpendicular);
			poly2min = glm::min(poly2min, point);
			poly2max = glm::max(poly2max, point);
		}

		// Get the overlap for this iteration
		float overlapA = poly1max - poly2min;
		float overlapB = poly2max - poly1min;
		float overlap = glm::min(overlapA, overlapB);
		if (overlap > 0.0f)
		{
			if (minOverlap > overlap)
			{
				minOverlap = overlap;
				minOverlapIndex = i;
				if (overlapA > overlapB)
					vertexDirection = vertexDirections[i];
				else
					vertexDirection = -vertexDirections[i];

				vertexDirection = { vertexDirection.y, -vertexDirection.x };
			}
		}
		else
			return col; // There was a gap so we can bail out of the collision test

	}

	// There wasn't a gap so we have a collision
	col.depth = minOverlap;
	col.normal = -vertexDirection;
	return col;
}