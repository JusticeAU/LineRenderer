#pragma once

#include "Maths.h"
#include "Circle.h"
#include "Plane.h"
#include "AABB.h"
#include "CollisionData.h"

CollisionData CircleOnCircle(Circle& ShapeA, Circle& ShapeB);
CollisionData CircleOnPlane(Circle& a, Plane& b);