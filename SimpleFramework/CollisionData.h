#pragma once
#include "Maths.h"

class Shape;
class LineRenderer;

class CollisionData
{
public:
	Vec2 normal = {0,0}; // The normal here will point from A to B. To resolve the collision we would move ShapeA along the normal in the negative direction, and move B in the positive.
	float depth = -1.0f; // Negative depth means no collision
	Vec2 worldPosition;

	Shape* shapeA = nullptr;
	Shape* shapeB = nullptr;

	bool IsCollision() const { return depth > 0.0f; }
	// Performs depenetration, calculates and applies an impulse to bound the colliding objects off each other. Should only be called if IsCollision returns true;
	void Resolve();
};