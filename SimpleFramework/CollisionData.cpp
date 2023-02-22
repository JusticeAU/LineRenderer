#include "CollisionData.h"
#include "Shape.h"

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

	if (massA == INFINITY)
		massA = 0.0f;

	if (massB == INFINITY)
		massB = 0.0f;

	// Calculate the mass ratio between the two objects to minamize visual jitter when depenetrating little objects from big objects.
	float totalMass = massA + massB;
	float shapeARatio = massA / totalMass;
	float shapeBRatio = massB / totalMass;

	// Perform depetration
	shapeA->Move(-normal * depth * shapeARatio);
	shapeB->Move(normal * depth * shapeBRatio);

	// Calculate the transfer of energy between the two colliding objects. We will generate an Impulse to apply along the collision normal using Newton's Law of Restituion
	Vec2 relativeVelocity = shapeB->m_velocity - shapeA->m_velocity;

	float elasticity = 0.5f; // This is a hardcoded value to lose some energy on collision so that things will eventually settle.

	float k = -(1 + elasticity) * glm::dot(relativeVelocity, normal); // Calculate direction the objects should be moving based on their relative velocity and the collision normal
	
	if(k < 0.0f) // If they are already moving away from each other then bail out of this collision
		return;

	float j =  k / (shapeB->GetInverseMass() + shapeA->GetInverseMass()); // Calcualate the Impulse magnitude (j)

	shapeA->ApplyImpulse(normal * -j);
	shapeB->ApplyImpulse(normal * j);

}