#pragma once
#include "Maths.h"

class CircleCollisionData;

class Circle
{
public:
	Circle(Vec2 position, float radius) : m_position(position), m_radius(radius) {}
	Vec2 m_position = { 0,0 };
	float m_radius = 0;

	CircleCollisionData CollisionWithCircle(Circle* other);
};