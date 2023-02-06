#pragma once
#include "Maths.h"

class LineRenderer;

enum SHAPE
{
	SHAPE_CIRCLE,
	SHAPE_PLANE,
	SHAPE_AABB
};

class Shape
{
public:
	SHAPE m_type;
	Vec3 m_colour = { 1,1,1 };

	Shape(SHAPE shape, Vec3 colour) : m_type(shape), m_colour(colour) {}

	virtual void Draw(LineRenderer& lines) const = 0;
};