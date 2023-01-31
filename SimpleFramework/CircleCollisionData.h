#include "Maths.h"

class Circle;

class CircleCollisionData
{
public:
	Vec2 normal = {0,0};
	float penetration = 0;

	Circle* us = nullptr;
	Circle* other = nullptr;
};