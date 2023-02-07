#pragma once
#include "Shape.h"
#include "Maths.h"

class Plane : public Shape
{
public:
	Plane(Vec2 normal, float distance, Vec3 colour);

	Vec2 m_normal;
	float m_distance;

	void Draw(LineRenderer& lines) const override;

	float DepthInPlane(Vec2 point);

	void Move(Vec2 displacement) override { } // just overrides move to do nothing.
};