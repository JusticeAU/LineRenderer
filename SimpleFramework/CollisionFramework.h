#include "Application.h"
#include "Circle.h"
#include "Plane.h"
#include "AABB.h"
#include <vector>

class Spawner;

class CollisionFramework : public Application
{
public:
	CollisionFramework();

	void Update(float delta) override;
	void OnLeftClick() override;
	void OnLeftRelease() override;

	void OnRightClick() override;
	void OnRightRelease() override;

	void OnMouseScroll(double delta) override;

	std::vector<Shape*> shapes;
	bool rightDown = false;
	const int MAX_COLLISION_PASSES = 6;

	Spawner* spawner;

	int vertIndex = 0;
	bool indexing = false;
};