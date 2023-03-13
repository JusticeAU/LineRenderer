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
	shapeA->Move(-normal * depth * shapeARatio);
	shapeB->Move(normal * depth * shapeBRatio);
	
	// https://www.chrishecker.com/images/e/e7/Gdmphys3.pdf
	Vec2 perp(normal.y, -normal.x);

	Vec2 rAP = worldPosition - shapeA->m_position;
	if (shapeA->GetShape() == SHAPE::PLANE)
		rAP = worldPosition;
	Vec2 rBP = worldPosition - shapeB->m_position;
	if (shapeB->GetShape() == SHAPE::PLANE)
		rBP = worldPosition;


	Vec2 vAP = shapeB->m_velocity + rAP + shapeA->m_rotationalVelocity*-perp;
	Vec2 vBP = shapeB->m_velocity + rBP + shapeB->m_rotationalVelocity*perp;;
	Vec2 vAB = vBP - vAP;

	std::cout << glm::dot(vAB, normal) << std::endl;
	if (glm::dot(vAB, normal) < 0.0f)
	{
		float elasticity = 1.0f; // This is a hardcoded value to lose some energy on collision so that things will eventually settle.
		float force = (-(1.0f + elasticity) * glm::dot(vAB, normal))
			/ glm::dot(normal, normal) * (shapeA->GetInverseMass() + shapeB->GetInverseMass()) + ((glm::dot(rAP, normal) * glm::dot(rAP, normal)) / shapeA->GetMoment()) + ((glm::dot(rBP, normal) * glm::dot(rBP, normal) / shapeB->GetMoment()));

		shapeA->ApplyImpulse(-force * normal, worldPosition - shapeA->m_position);
		shapeB->ApplyImpulse(force * normal, worldPosition - shapeB->m_position);

		std::cout << force << std::endl;
	}
}