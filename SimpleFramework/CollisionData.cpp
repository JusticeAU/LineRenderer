#include "CollisionData.h"
#include "Shape.h"
#include "LineRenderer.h"

void CollisionData::Resolve()
{
	if (IsCollision() != true)
		return;

	// check for two static items colliding.
	if (shapeA->m_inverseMass == 0.0f && shapeB->m_inverseMass == 0.0f)
		return;


	float massA = 1.0f / shapeA->m_inverseMass;
	float massB = 1.0f / shapeB->m_inverseMass;

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
	float j = glm::dot(-(1 + elasticity) * (relativeVelocity), normal) / (shapeA->m_inverseMass + (shapeB->m_inverseMass));

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

	//EkPre += 1;

}

void CollisionData::DebugDrawCircleInfo(LineRenderer& lines) const
{
	if (IsCollision())
	{
		lines.SetColour({ 1,0,0 });
		lines.DrawCross(worldPosition - normal * depth / 2.0f, 0.1);
		lines.SetColour({ 0.5,0.5,0 });
		lines.DrawCross(worldPosition, 0.1);
		lines.SetColour({ 0,1,0 });
		lines.DrawCross(worldPosition + normal * depth / 2.0f, 0.1);
		lines.SetColour({ 1,1,1 });
		lines.DrawCircle(worldPosition, depth / 2);
	}
}

void CollisionData::DebugDrawPlaneInfo(LineRenderer& lines) const
{
	lines.SetColour({ 0, 1, 0 });
	lines.DrawLineSegment({ 0,0 }, normal*depth);
}