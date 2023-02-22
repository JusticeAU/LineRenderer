#include "CollisionData.h"
#include "Shape.h"
#include "LineRenderer.h"
#include <iostream>

void CollisionData::Resolve()
{
	if (IsCollision() != true)
		return;

	// check for two static items colliding.
	if (shapeA->GetInverseMass() == 0.0f && shapeB->GetInverseMass() == 0.0f)
		return;


	float massA = 1.0f / shapeA->GetInverseMass();
	float massB = 1.0f / shapeB->GetInverseMass();

	if (massA == INFINITY)
		massA = 0.0f;

	if (massB == INFINITY)
		massB = 0.0f;

	float totalMass = massA + massB;
	float shapeARatio = massA / totalMass;
	float shapeBRatio = massB / totalMass;

	shapeA->Move(-normal * depth * shapeARatio);
	shapeB->Move(normal * depth * shapeBRatio);

	Vec2 relativeVelocity = shapeB->m_velocity - shapeA->m_velocity;

	float elasticity = .5f;
	float k = -(1 + elasticity) * glm::dot(relativeVelocity, normal);
	if(k < 0.0f)
		return;
	//float j =  k / (shapeB->m_inverseMass + shapeA->m_inverseMass);
	float j = glm::dot(-(1 + elasticity) * (relativeVelocity), normal) / (shapeA->GetInverseMass() + (shapeB->GetInverseMass()));

	float EkPre = 0.5f * (
		massA * (glm::dot(shapeA->m_velocity,shapeA->m_velocity)) +
		(massB * (glm::dot(shapeB->m_velocity,shapeB->m_velocity)))
		);

	shapeA->ApplyImpulse(normal * -j);
	shapeB->ApplyImpulse(normal * j);

	float EkAfter = 0.5f * (
		massA * (glm::dot(shapeA->m_velocity, shapeA->m_velocity)) +
		(massB * (glm::dot(shapeB->m_velocity, shapeB->m_velocity)))
		);
	
	//std::cout << EkAfter / EkPre << std::endl;

}