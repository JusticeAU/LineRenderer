#pragma once
#include "Maths.h"
#include "Shape.h"

class AABB : public Shape
{
public:
	AABB(Vec2 position, float width, float height, Vec3 colour = { 1,1,1 })
		: Shape(SHAPE_AABB, colour), m_position(position), m_halfWidth(width), m_halfHeight(height) {}

	Vec2 m_position;
	float m_halfWidth;
	float m_halfHeight;

	void Draw(LineRenderer& lines) const override;
};