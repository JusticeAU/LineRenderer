#include "Application.h"
#include "Shape.h"
#include <vector>

class Spawner;

class CollisionFramework : public Application
{
public:
	CollisionFramework();

	Shape* testPlane = nullptr;
	Shape* testCircle = nullptr;

protected:
	std::vector<Shape*> shapes;
	Spawner* spawner;

	const int MAX_COLLISION_PASSES = 6;

public:
	void Update(float delta) override;
	void OnLeftClick() override;
	void OnLeftRelease() override;

	void OnRightClick() override;
	void OnRightRelease() override;

	void OnMouseScroll(double delta) override;
};