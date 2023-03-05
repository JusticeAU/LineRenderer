#pragma once
#include "Maths.h"
#include <vector>

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
	Shape(SHAPE shape, Vec2 position, float inverseMass, Vec3 colour = {1, 1, 1}) : m_type(shape), m_position(position), m_inverseMass(inverseMass), m_colour(colour) {}
	Vec2 m_velocity = { 0,0 };
	Vec2 m_position = { 0,0 };
	Vec3 m_colour = { 1,1,1 };
	float m_rotation = 0.0f; // Stored in degrees for human reading - will convert to radians for most operations.
	float m_rotationalVelocity = 30.0f;
	float m_momentOfInertia = 0.0f;
protected:
	SHAPE m_type;
	Vec2 m_gravity = { 0, -9.81 };
	float m_inverseMass = 0.0f;
	bool m_toBeDeleted = false;

public:
	SHAPE GetShape() { return m_type; }
	
	virtual void Move(Vec2 displacement);
	virtual void Rotate(float degrees);
	virtual void ApplyImpulse(Vec2 impulse, Vec2 position);

	virtual void Update(float deltaTime);
	virtual void Draw(LineRenderer& lines) const = 0;
	
	virtual void CalculateMassFromArea() = 0;
	virtual void CalculateMomentOfInertia() = 0;
	virtual bool PointInShape(Vec2 point) const = 0;
	virtual bool LineIntersects(Vec2 a, Vec2 b) const { return false; } // default behaviour
	virtual void Slice(Vec2 a, Vec2 b, std::vector<Shape*>* shapes) {}
	
	void MarkForDeletion() { m_toBeDeleted = true; };
	bool IsMarkedForDeletion()  const { return m_toBeDeleted; }

	void SetMass(float mass) { m_inverseMass = 1 / mass; }
	void MakeKinematic() { m_inverseMass = 0.0f; }
	// may return INFINITY for a kinematic/static object
	float GetMass() const { return 1.0f / m_inverseMass; }
	float GetInverseMass() const { return m_inverseMass; }
	float GetMoment() const { return m_momentOfInertia; }
};