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
	


	float mag1 = glm::length(shapeA->m_velocity);
	float mag2 = glm::length(shapeB->m_velocity);

	// Perform depenetration
	shapeA->Move(-normal * depth * shapeARatio);
	shapeB->Move(normal * depth * shapeBRatio);

	if (v1 > v2) // moving closer
	{
		//std::cout << "collision" << std::endl;
		// calculate effective mass at contact point for each object
		// ie how much the contact point will move due to the force applied
		float eIM1 = (shapeA->GetInverseMass() + ((r1 * r1) * shapeA->GetInverseMoment()));
		float eIM2 = (shapeB->GetInverseMass() + ((r2 * r2) * shapeB->GetInverseMoment()));

		float elasticity = 1.0f; // This is a hardcoded value to lose some energy on collision so that things will eventually settle.
		//Vec2 force = (1.0f + elasticity) * eIM1 * eIM2 / (eIM1 + eIM2) * (v1 - v2) * normal;
		Vec2 vAP = worldPosition - shapeA->m_position;
		Vec2 vBP = worldPosition - shapeB->m_position;
		Vec2 vAB = vBP - vAP;
		float force = (-(1.0f + elasticity) * glm::dot(vAB, normal))
			/ glm::dot(normal, normal) * (shapeA->GetInverseMass() + shapeB->GetInverseMass()) + (eIM1) + (eIM2);
		//supplementaryPoints.push_back(vAB);
		//Vec2 force = -((1.0f + elasticity) * glm::dot(vAB, normal) / glm::dot(normal, normal) * (eMass1 + eMass2)) * normal;

		shapeA->ApplyImpulse(-force * normal, worldPosition - shapeA->m_position);
		shapeB->ApplyImpulse(force * normal, worldPosition - shapeB->m_position);
	}	

}