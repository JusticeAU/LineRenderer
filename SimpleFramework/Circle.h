#pragma once
#include "Maths.h"
#include "Shape.h"

class Circle : public Shape
{
public:
	Circle(Vec2 position, float radius, Vec3 colour = { 1,1,1 })
		: Shape(SHAPE_CIRCLE, position, colour), m_radius(radius) {}

	float m_radius = 0;

	void Draw(LineRenderer& lines) const override;
};