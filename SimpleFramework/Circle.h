#pragma once
#include "Maths.h"
#include "Shape.h"

class Circle : public Shape
{
public:
	Circle(Vec2 position, float radius, float inverseMass, Vec3 colour = { 1,1,1 })
		: Shape(SHAPE::CIRCLE, position, inverseMass, colour), m_radius(radius) {}

	float m_radius;

	void CalculateMassFromArea() override;
	void Draw(LineRenderer& lines) const override;

	bool PointInShape(Vec2 point) override;

};