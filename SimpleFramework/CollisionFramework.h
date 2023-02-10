#include "Application.h"
#include "Circle.h"
#include "Plane.h"
#include "AABB.h"
#include <vector>

class CollisionFramework : public Application
{
public:
	CollisionFramework();

	void Update(float delta) override;
	void OnLeftClick() override;
	void OnLeftRelease() override;

	void OnRightClick() override;
	void OnRightRelease() override;

	void ProcessObjectSpawner();

	std::vector<Shape*> shapes;
	bool rightDown = false;
	const int MAX_COLLISION_PASSES = 6;

	std::vector<Shape*> shapeTemplates;
	int templateIndex = 0;
	//bool spawnStarted = false;
	Shape* spawn = nullptr;
	Vec2 spawnStartPos;
	float timeSinceStart;
};