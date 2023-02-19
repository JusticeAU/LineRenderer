#pragma once
#include "Maths.h"
#include "Shape.h"

class AABB : public Shape
{
public:
	AABB(Vec2 position, float width, float height, float inverseMass, Vec3 colour = { 1,1,1 })
		: Shape(SHAPE::AABB, position, inverseMass, colour), m_halfWidth(width/2), m_halfHeight(height/2) {}

	float m_halfWidth;
	float m_halfHeight;

	void CalculateMassFromArea() override;

	void Draw(LineRenderer& lines) const override;

	bool PointInShape(Vec2 point) override;
	Vec2 GetClosestPoint(Vec2 point);
	Vec2* GetCorners();

	float Top() { return m_position.y + m_halfHeight; }
	float Bottom() { return m_position.y - m_halfHeight; }
	float Left() { return m_position.x - m_halfWidth; }
	float Right() { return m_position.x + m_halfWidth; }
};