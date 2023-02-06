#include "Application.h"
#include "Circle.h"
#include "Plane.h"
#include <vector>

class CollisionFramework : public Application
{
public:
	CollisionFramework();

	void Update(float delta) override;

	Circle* yourCircle = new Circle({ -5,0 }, 2.0f, { 1,0,0 });
	std::vector<Shape*> shapes;
	bool rightDown = false;
	const int MAX_COLLISION_PASSES = 3;
};