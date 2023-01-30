#pragma once
#include <glm.hpp>

enum ShapeType {
	PLANE = 0,
	SPHERE,
	BOX
};

class LineRenderer;

class PhysicsObject
{
protected:
	PhysicsObject(ShapeType a_shapeID) : m_shapeID(a_shapeID) {}
public:
	virtual void fixedUpdate(glm::vec2 gravity, float timeStep) = 0;
	virtual void draw(LineRenderer* lr) = 0;
	//virtual void resetPosition() = 0;

protected:
	ShapeType m_shapeID;
};

