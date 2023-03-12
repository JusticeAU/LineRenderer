#include "Application.h"
#include "Shape.h"
#include <vector>

class Spawner;
class Toolbox;

class CollisionFramework : public Application
{
public:
	CollisionFramework();

protected:
	std::vector<Shape*> shapes;
	Spawner* spawner;
	Toolbox* toolbox;

	const int MAX_COLLISION_PASSES = 6;

public:
	void Update(float delta) override;
	void OnLeftClick() override;
	void OnLeftRelease() override;

	void OnRightClick() override;
	void OnRightRelease() override;

	void OnMouseScroll(double delta) override;
};