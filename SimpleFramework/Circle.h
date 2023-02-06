#pragma once
#include "Maths.h"
#include "Shape.h"

class Circle : public Shape
{
public:
	Circle(Vec2 position, float radius, Vec3 colour = { 1,1,1 })
		: Shape(SHAPE_CIRCLE, colour), m_position(position), m_radius(radius) {}

	Vec2 m_position;
	float m_radius;

	void Draw(LineRenderer& lines) const override;

};