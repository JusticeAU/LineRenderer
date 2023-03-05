#include "CollisionData.h"
#include "Shape.h"
#include <iostream>

void CollisionData::Resolve()
{
	

	// We should not be peforming this routine where a collision hasn't happened.
	if (IsCollision() != true)
		return;

	// check for two static/kinematic items colliding.
	if (shapeA->GetInverseMass() == 0.0f && shapeB->GetInverseMass() == 0.0f)
		return;

	float massA = shapeA->GetMass();
	float massB = shapeB->GetMass();


	// Calculate the mass ratio between the two objects to minamize visual jitter when depenetrating little objects from big objects.
	float shapeARatio, shapeBRatio;
	float totalMass = massA + massB;
	if (massA == INFINITY)
	{
		shapeARatio = 0.0f;
		shapeBRatio = 1.0f;
	}
	else if (massB == INFINITY)
	{
		shapeARatio = 1.0f;
		shapeBRatio = 0.0f;
	}
	else
	{
		shapeARatio = massB / totalMass;
		shapeBRatio = massA / totalMass;
	}

	// Check of the objects are moving toward each other
	Vec2 perp = Vec2(normal.y, -normal.x);
	// Determine total velocity of contact points for two objects, both linear and rotaiton.

	// r is radius from axis to application of force
	float r1 = glm::dot(worldPosition - shapeA->m_position, -perp);
	float r2 = glm::dot(worldPosition - shapeB->m_position, perp);

	// v is velocity of the contact point on this object
	float v1 = glm::dot(shapeA->m_velocity, normal) - r1 * shapeA->m_rotationalVelocity;
	float v2 = glm::dot(shapeB->m_velocity, normal) + r2 * shapeB->m_rotationalVelocity;

	// Perform depenetration
	shapeA->Move(-normal * depth * shapeARatio);
	shapeB->Move(normal * depth * shapeBRatio);

	if (v1 > v2) // moving closer
	{
		// calculate effective mass at contact point for each object
		// ie how much the contact point will move due to the force applied
		float mass1 = shapeA->GetMass() + (r1 * r1) / shapeA->GetMoment();
		float mass2 = shapeB->GetMass() + (r2 * r2) / shapeB->GetMoment();

		// temporary work around for infinity mass objects
		if (mass1 == INFINITY)
			mass1 = 10.0f;
		if (mass2 == INFINITY)
			mass2 = 10.0f;

		float elasticity = 0.5f; // This is a hardcoded value to lose some energy on collision so that things will eventually settle.
		Vec2 force = (1.0f + elasticity) * mass1 * mass2 / (mass1 + mass2) * (v1 - v2) * normal;

		shapeA->ApplyImpulse(-force, worldPosition - shapeA->m_position);
		shapeB->ApplyImpulse(force, worldPosition - shapeB->m_position);
	}	

}