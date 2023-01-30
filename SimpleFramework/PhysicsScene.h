#pragma once
#include <vector>
#include <glm.hpp>

class PhysicsObject;
class LineRenderer;

class PhysicsScene
{
public:
	PhysicsScene();
	~PhysicsScene();

	void addActor(PhysicsObject* actor);
	void removeActor(PhysicsObject* actor);

	void Update(float delta);
	void Draw(LineRenderer* lr);

	void setGravity(const glm::vec2 gravity) { m_gravity = gravity; }
	glm::vec2 getGravity() const { return m_gravity; }

	void setTimeStep(const float timeStep) { m_timeStep = timeStep; }
	float getTimeStep() const { return m_timeStep; }

	PhysicsObject* GetActor(int index) { return m_actors[index]; }

	static bool sphere2sphere(PhysicsObject* obj1, PhysicsObject* obj2);
protected:
	float accumulatedTime = 0.0f;
	glm::vec2 m_gravity;
	float m_timeStep = 0.0f;
	std::vector<PhysicsObject*> m_actors;
};

