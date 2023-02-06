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
};