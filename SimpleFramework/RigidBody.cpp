#include "RigidBody.h"

RigidBody::RigidBody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float mass, float orientation)
	: PhysicsObject(shapeID)
{
	m_position = position;
	m_velocity = velocity;
	m_mass = mass;
	m_orientation = orientation;
}

void RigidBody::fixedUpdate(glm::vec2 gravity, float timeStep)
{
	m_position += m_velocity * timeStep;
	this->applyForce(gravity * m_mass * timeStep);
}

void RigidBody::applyForce(glm::vec2 force)
{
	m_velocity += force / m_mass;
}

void RigidBody::applyForceToActor(RigidBody* actor, glm::vec2 force)
{
	actor->applyForce(force);
	applyForce(-force);
}
