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


	std::vector<Shape*> shapes;
	bool rightDown = false;
	const int MAX_COLLISION_PASSES = 6;

	std::vector<Shape*> shapeTemplates;
	int templateIndex = 0;
	bool spawnStarted = false;
	Vec2 spawnShapeStartPos;
	Shape* spawn = nullptr;
};