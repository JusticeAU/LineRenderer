#pragma once
#include "Maths.h"
#include "Shape.h"

class Circle : public Shape
{
public:
	Circle(Vec2 position, float radius, float inverseMass, Vec3 colour = { 1,1,1 })
		: Shape(SHAPE::CIRCLE, position, inverseMass, colour) { SetRadius(radius); }

protected:
	float m_radius;

public:
	void CalculateMassFromArea() override;
	void Draw(LineRenderer& lines) const override;

	bool LineIntersects(Vec2 a, Vec2 b) override;
	bool PointInShape(Vec2 point) const override;
	void Slice(Vec2 a, Vec2 b, std::vector<Shape*>* shapes) override;

	float GetRadius() const { return m_radius; }
	// Ensures radius is a positive number
	void SetRadius(float radius) { m_radius = radius < 0.0f ? radius * -1.0f : radius; }
};