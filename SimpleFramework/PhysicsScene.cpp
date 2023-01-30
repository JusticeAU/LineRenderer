#include "PhysicsScene.h"
#include "PhysicsObject.h"
#include "Sphere.h"
#include <algorithm>
#include "LineRenderer.h"

PhysicsScene::PhysicsScene()
{
	// Construct

}

PhysicsScene::~PhysicsScene()
{
	for (auto pActor : m_actors)
	{
		delete pActor;
	}
}

void PhysicsScene::addActor(PhysicsObject* actor)
{
	m_actors.push_back(actor);
}

void PhysicsScene::removeActor(PhysicsObject* actor)
{
	std::vector<PhysicsObject*>::iterator it;
	it = std::find(m_actors.begin(), m_actors.end(), actor);

	if (it != m_actors.end())
	{
		m_actors.erase(it);
	}
}

void PhysicsScene::Update(float delta)
{
	// Update physics at fixed time step
	
	accumulatedTime += delta;

	while (accumulatedTime >= m_timeStep)
	{
		for (auto pActor : m_actors)
		{
			pActor->fixedUpdate(m_gravity, m_timeStep);

			//TODO Capture previous state before updating so we can interpolate/extrapolate visuals for smooth drawing on a lower fixed time step.
		}

		accumulatedTime -= m_timeStep;
	}
}

void PhysicsScene::Draw(LineRenderer* lr)
{
	for (auto pActor : m_actors)
	{
		pActor->draw(lr);
	}
}

bool PhysicsScene::sphere2sphere(PhysicsObject* obj1, PhysicsObject* obj2)
{
	Sphere* sphere1 = dynamic_cast<Sphere*>(obj1);
	Sphere* sphere2 = dynamic_cast<Sphere*>(obj2);

	if (sphere1 != nullptr && sphere2 != nullptr)
	{
		// test for collision
		glm::vec2 displacement = sphere1->getPosition() - sphere2->getPosition();
		float distance = glm::length(displacement);
		if (distance - sphere1->getRadius() - sphere2->getRadius() <= 0)
		{
			// Overlap
			sphere1->SetVelocity(glm::vec2(0, 0));
			sphere2->SetVelocity(glm::vec2(0, 0));

		}

	}

	return false;
}