#pragma once
#include "Application.h"
#include "PhysicsScene.h"

class Sphere;

class PhysicsApplication : public Application
{
public:
	PhysicsApplication();

	virtual void Update(float delta) override;
private:
	PhysicsScene* m_physicsScene;
	Sphere* sphere;
};

