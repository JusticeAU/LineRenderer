#pragma once

#include "Maths.h"
#include "Circle.h"
#include "Plane.h"
#include "AABB.h"
#include "ConvexPolygon.h"
#include "CollisionData.h"

#include <functional>

typedef std::function<CollisionData(Shape*, Shape*)> CollisionFunction;

CollisionData TestCollisions(Shape* a, Shape* b);


CollisionData CircleOnCircle(Shape* a, Shape* b);
CollisionData CircleOnPlane(Shape* a, Shape* b);
CollisionData CircleOnAABB(Shape* a, Shape* b);
CollisionData CircleOnConvexPoly(Shape* a, Shape* b);

CollisionData PlaneOnCircle(Shape* a, Shape* b);
CollisionData PlaneOnPlane(Shape* a, Shape* b);
CollisionData PlaneOnAABB(Shape* a, Shape* b);
CollisionData PlaneOnConvexPoly(Shape* a, Shape* b);

CollisionData AABBOnCircle(Shape* a, Shape* b);
CollisionData AABBOnPlane(Shape* a, Shape* b);
CollisionData AABBOnAABB(Shape* a, Shape* b);
CollisionData AABBOnConvexPoly(Shape* a, Shape* b);

CollisionData ConvexPolyOnCircle(Shape* a, Shape* b);
CollisionData ConvexPolyOnPlane(Shape* a, Shape* b);
CollisionData ConvexPolyOnAABB(Shape* a, Shape* b);
CollisionData ConvexPolyOnConvexPoly(Shape* a, Shape* b);