#pragma once
#include "PhysicsObject.h"

class RigidBody : public PhysicsObject
{
public:
	RigidBody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float mass, float orientation);
	//~RigidBody();

	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
	void applyForce(glm::vec2 force);
	void applyForceToActor(RigidBody* actor, glm::vec2 force);

	glm::vec2 getPosition() { return m_position; }
	glm::vec2 getVelocity() { return m_velocity; }
	void SetVelocity(glm::vec2 velocity) { m_velocity = velocity; }
	float getMass() { return m_mass; }
	void SetMass(float newMass) { m_mass = newMass; }
	float getOrientation() { return m_orientation; }

protected:
	glm::vec2 m_position;
	glm::vec2 m_velocity;
	float m_mass;
	float m_orientation; // 2D, so a single float will suffice.
};