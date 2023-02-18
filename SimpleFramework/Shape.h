#pragma once
#include "Maths.h"

class LineRenderer;

enum class SHAPE
{
	CIRCLE,
	AABB,
	CONVEX_POLY,
	PLANE,

	COUNT
};

class Shape
{
public:
	SHAPE m_type;
	Vec2 m_position = { 0,0 };
	Vec2 m_velocity = { 0,0 };
	Vec2 m_gravityForNow = { 0, -9.81 };
	float m_inverseMass = 0.0f;
	Vec3 m_colour = { 1,1,1 };

	Shape(SHAPE shape, Vec2 position, float inverseMass, Vec3 colour = {1, 1, 1}) : m_type(shape), m_position(position), m_inverseMass(inverseMass), m_colour(colour) {}

	SHAPE GetShape() { return m_type; }

	virtual void Update(float deltaTime);
	virtual void Draw(LineRenderer& lines) const = 0;
	
	virtual void CalculateMassFromArea() = 0;
	virtual void Move(Vec2 displacement);
	virtual void ApplyImpulse(Vec2 impulse);

	virtual bool PointInShape(Vec2 point);
};