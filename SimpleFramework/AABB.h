#pragma once
#include "Maths.h"
#include "Shape.h"

class AABB : public Shape
{
public:
	AABB(Vec2 position, float width, float height, float inverseMass, Vec3 colour = { 1,1,1 })
		: Shape(SHAPE::AABB, position, inverseMass, colour), m_halfWidth(width), m_halfHeight(height) {}

	float m_halfWidth;
	float m_halfHeight;

	void Draw(LineRenderer& lines) const override;
};