#include "Maths.h"

class Circle;
class LineRenderer;

class CollisionData
{
public:
	Vec2 normal = {0,0}; // The normal here will point from A to B. To resolve the collision we would move ShapeA along the normal in the negative direction, and move B in the positive.
	float depth = -1.0f; // Negative depth means no collision
	Vec2 worldPosition;

	Circle* shapeA = nullptr;
	Circle* shapeB = nullptr;

	bool IsCollision() const { return depth > 0.0f; }

	void Resolve();

	void DebugDraw(LineRenderer& lines) const;
};