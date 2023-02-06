#pragma once

#include "Maths.h"
#include "Circle.h"
#include "Plane.h"
#include "AABB.h"
#include "CollisionData.h"

#include <functional>

typedef std::function<CollisionData(Shape*, Shape*)> CollisionFunction;

CollisionData TestCollisions(Shape* a, Shape* b);


CollisionData CircleOnCircle(Shape* a, Shape* b);
CollisionData CircleOnPlane(Shape* a, Shape* b);

CollisionData PlaneOnCircle(Shape* a, Shape* b);
CollisionData PlaneOnPlane(Shape* a, Shape* b);