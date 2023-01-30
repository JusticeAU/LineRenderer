#include "PhysicsApplication.h"
#include "Sphere.h"

PhysicsApplication::PhysicsApplication()
{
	m_physicsScene = new PhysicsScene();
	m_physicsScene->setTimeStep(0.01f);
	m_physicsScene->setGravity(glm::vec2(0, -10));

	sphere = new Sphere(glm::vec2(0, 0), glm::vec2(0, 10), 200, .5f, glm::vec4(1, 0, 0, 1));
	m_physicsScene->addActor(sphere);
}

void PhysicsApplication::Update(float delta)
{

	if (leftMouseDown)
	{
		sphere->SetVelocity({ 0, 5 });
	}

	m_physicsScene->Update(delta);
	m_physicsScene->Draw(lines);
}
