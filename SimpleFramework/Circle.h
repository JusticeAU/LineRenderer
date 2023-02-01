#pragma once
#include "Maths.h"

class CollisionData;
class LineRenderer;

class Circle
{
public:
	Circle(Vec2 position, float radius, Vec3 colour = { 1,1,1 }) : m_position(position), m_radius(radius), m_colour(colour) {}
	Vec2 m_position = { 0,0 };
	float m_radius = 0;
	Vec3 m_colour = { 1,1,1 };

	void Draw(LineRenderer& lines) const;

	CollisionData CollisionWithCircle(Circle* shapeB);
};