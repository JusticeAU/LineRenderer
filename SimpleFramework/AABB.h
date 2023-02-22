#pragma once
#include "Maths.h"
#include "Shape.h"

class AABB : public Shape
{
public:
	AABB() : Shape(SHAPE::AABB, { 0,0 }, 0) {}
	AABB(Vec2 position, float width, float height, float inverseMass, Vec3 colour = { 1,1,1 })
		: Shape(SHAPE::AABB, position, inverseMass, colour), m_halfWidth(width/2), m_halfHeight(height/2) {}
protected:
	float m_halfWidth = 0.0f;
	float m_halfHeight = 0.0f;

public:
	void Draw(LineRenderer& lines) const override;
	void CalculateMassFromArea() override;
	bool PointInShape(Vec2 point) const override;

	Vec2* GetCorners() const;
	Vec2 GetClosestPoint(Vec2 point);

	float Top() const { return m_position.y + m_halfHeight; }
	float Bottom() const { return m_position.y - m_halfHeight; }
	float Left() const { return m_position.x - m_halfWidth; }
	float Right() const { return m_position.x + m_halfWidth; } 
	float HalfWidth() const { return m_halfWidth; }
	float HalfHeight() const { return m_halfHeight; }

	// Will enforce positive values
	void SetHalfWidth(float halfWidth) { m_halfWidth = halfWidth < 0.0f ? halfWidth * -1.0f : halfWidth; }
	// Will enforce positive values
	void SetHalfHeight(float halfHeight) { m_halfHeight = halfHeight < 0.0f ? halfHeight * -1.0f : halfHeight; } 


};